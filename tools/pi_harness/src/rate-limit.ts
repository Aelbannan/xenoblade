// ---------------------------------------------------------------------------
// Global cross-session request-rate PACER.
//
// Problem: the nube provider serves DeepSeek-V4-Flash via LiteLLM with a hard
// `rpm_limit` (observed ~20 req/min) on the serving deployment. 14-20 parallel
// harness sessions each issue many model requests/min (every assistant tool-call
// turn inside one `session.prompt()` is a separate HTTP request via
// `ModelRuntime.streamSimple`). Aggregate rate far exceeds the cap, so the
// gateway 429s; the SDK retry-amplifier then multiplies each failed request
// into ~5 gateway calls, consuming the whole rpm budget on retries. Measured:
// 79% of session wall-clock spent in 429/empty stall gaps.
//
// This is a PACER, not a token bucket with capacity=rpm (which permits 2x rpm
// in any window / startup bursts). Exactly one request is admitted every
// `60_000 / rpm` ms. Semantics:
//   - If no waiter is queued AND the inter-request interval has elapsed,
//     acquire() grants immediately.
//   - Otherwise the request enqueues; a single self-rescheduling timer fires
//     every `60_000/rpm` ms, grants the FRONT waiter (FIFO), and reschedules
//     only while waiters remain.
//   - acquire() is AbortSignal-interruptible: an aborted waiter is removed and
//     rejects without consuming a token (dead-session / timeout safety).
//
// The module singleton (configureSharedRateLimiter, called once at orchestrator
// startup) is shared by all sessions because the harness runs in a single
// `tsx src/index.ts` process.
// ---------------------------------------------------------------------------

export interface RateLimiterAcquireOptions {
  /** Aborting the signal rejects a queued acquire without consuming a token. */
  signal?: AbortSignal;
}

export interface RateLimiter {
  readonly rpm: number;
  /** Number of waiters currently queued (observability). */
  readonly queueDepth: number;
  acquire(opts?: RateLimiterAcquireOptions): Promise<void>;
  dispose(): void;
}

interface Waiter {
  resolve: () => void;
  reject: (err: unknown) => void;
  signal?: AbortSignal;
  onSignal?: () => void;
}

interface PacerClock {
  now(): number;
  schedule(fn: () => void, delayMs: number): ReturnType<typeof setTimeout>;
  clear(t: ReturnType<typeof setTimeout>): void;
}

const realClock: PacerClock = {
  now: () => performance.now(),
  schedule: (fn, delayMs) => setTimeout(fn, delayMs),
  clear: (t) => clearTimeout(t),
};

export class Pacer implements RateLimiter {
  private readonly intervalMs: number;
  private readonly clock: PacerClock;
  private waiters: Waiter[] = [];
  private nextAvailableAt = Number.NEGATIVE_INFINITY;
  private disposed = false;
  private tickTimer: ReturnType<typeof setTimeout> | null = null;

  constructor(rpm: number, clock: PacerClock = realClock) {
    this.intervalMs = Math.max(1, 60_000 / rpm);
    this.clock = clock;
  }

  get rpm(): number {
    return Math.round(60_000 / this.intervalMs);
  }

  get queueDepth(): number {
    return this.waiters.length;
  }

  acquire(opts?: RateLimiterAcquireOptions): Promise<void> {
    if (this.disposed) return Promise.resolve();
    const signal = opts?.signal;
    return new Promise<void>((resolve, reject) => {
      const now = this.clock.now();
      if (now >= this.nextAvailableAt) {
        // Slot open right now — grant immediately, no queue.
        this.nextAvailableAt = now + this.intervalMs;
        return resolve();
      }

      const waiter: Waiter = { resolve, reject, signal };
      const onAbort = () => this.removeWaiter(waiter, new Error("aborted while queued"));
      if (signal) {
        if (signal.aborted) {
          return reject(new Error("aborted while queued"));
        }
        waiter.onSignal = onAbort;
        signal.addEventListener("abort", onAbort);
      }
      this.waiters.push(waiter);
      this.ensureTicker();
    });
  }

  private removeWaiter(waiter: Waiter, err: unknown): void {
    const idx = this.waiters.indexOf(waiter);
    if (idx >= 0) this.waiters.splice(idx, 1);
    if (waiter.signal && waiter.onSignal) {
      waiter.signal.removeEventListener("abort", waiter.onSignal);
    }
    waiter.reject(err);
    // If nothing left queued, stop ticking (no timer leak).
    if (this.waiters.length === 0 && this.tickTimer) {
      this.stopTicking();
    }
  }

  /** Start a timer for the next grant, aligned to nextAvailableAt (exact
   *  pacing: a queued request waits until the slot actually opens, not a tick
   *  earlier). If a timer is already scheduled, leave it. */
  private ensureTicker(): void {
    if (this.tickTimer !== null) return;
    const waitMs = Math.max(1, this.nextAvailableAt - this.clock.now());
    this.tickTimer = this.clock.schedule(() => this.tick(), waitMs);
  }

  private tick(): void {
    this.tickTimer = null;
    if (this.disposed) return;
    if (this.waiters.length === 0) {
      return;
    }
    const now = this.clock.now();
    // Slot opens at nextAvailableAt; if we fired a hair early (clock drift),
    // defer to exactly nextAvailableAt.
    if (now < this.nextAvailableAt) {
      this.ensureTicker();
      return;
    }
    this.nextAvailableAt = now + this.intervalMs;
    const waiter = this.waiters.shift()!;
    if (waiter.signal && waiter.onSignal) {
      waiter.signal.removeEventListener("abort", waiter.onSignal);
    }
    waiter.resolve();
    if (this.waiters.length > 0) {
      this.ensureTicker();
    }
  }

  private stopTicking(): void {
    if (this.tickTimer !== null) {
      this.clock.clear(this.tickTimer);
      this.tickTimer = null;
    }
  }

  dispose(): void {
    this.disposed = true;
    this.stopTicking();
    for (const w of this.waiters) {
      if (w.signal && w.onSignal) w.signal.removeEventListener("abort", w.onSignal);
      w.reject(new Error("rate limiter disposed"));
    }
    this.waiters = [];
  }
}

let shared: Pacer | null = null;

/** Configure the process-wide pacer ONCE at orchestrator startup. First config
 *  wins; rmp<=0 disables (returns null). */
export function configureSharedRateLimiter(rpm: number): void {
  if (rpm <= 0) {
    shared = null;
    return;
  }
  if (shared) {
    if (shared.rpm !== Math.round(rpm)) {
      console.warn(
        `[rate-limit] configureSharedRateLimiter: already configured with rpm=${shared.rpm}; ignoring rpm=${rpm} (first config wins)`,
      );
    }
    return;
  }
  shared = new Pacer(rpm);
}

export function getSharedRateLimiter(): RateLimiter | null {
  return shared;
}

export { realClock };

// ---------------------------------------------------------------------------
// Paced stream wrapper: gate a provider stream behind the shared pacer without
// touching SDK internals. The SDK's streamFn calls modelRuntime.streamSimple
// once per model turn; each call returns a push-based stream whose events the
// agent-session consumes. We wrap it so the REAL provider call (and its HTTP
// request) only starts once a pacer slot opens.
//
// The wrapper mirrors the public EventStream contract (push/end/asyncIterator/
// result) so the SDK consumes it identically. It is deliberately minimal:
// events are buffered and delivered FIFO; `result()` resolves with the final
// message extracted from the stream's done/error event, matching the SDK's
// AssistantMessageEventStream semantics.
// ---------------------------------------------------------------------------

export interface PacedStreamLike<T> extends AsyncIterable<T> {
  push(event: T): void;
  end(result?: unknown): void;
  result(): Promise<unknown>;
}

/** Cast a PacedStreamLike to the SDK's concrete stream type. The wrapper
 *  reimplements the EventStream public contract (push/end/asyncIterator/
 *  result); the private fields the class uses internally are not accessed by
 *  consumers, so the cast is safe. Kept here so callers don't spread casts. */

/** Wrap a lazy provider stream so its setup (HTTP request) starts only after
 *  `gate` resolves. `gate` is the pacer acquire (abort-aware). */
export function paceStream<T>(
  gate: Promise<void>,
  start: () => AsyncIterable<T> & { result(): Promise<unknown> },
): PacedStreamLike<T> {
  const queue: T[] = [];
  const waiting: Array<(r: { value?: T; done: boolean }) => void> = [];
  let done = false;
  let finalResult: Promise<unknown>;
  let resolveFinal: (r: unknown) => void;
  finalResult = new Promise((res) => {
    resolveFinal = res;
  });

  let started = false;
  const ensureStarted = (): Promise<void> => {
    if (started) return Promise.resolve();
    started = true;
    return gate
      .then(() => start())
      .then(async (inner) => {
        for await (const event of inner) {
          push(event);
        }
        try {
          const res = await inner.result();
          end(res);
        } catch {
          end(undefined);
        }
      })
      .catch((err) => {
        // Gate rejected (abort) or provider setup failed — terminate the
        // stream with an error marker like the SDK's lazyStream does.
        end(undefined);
        queue.length = 0;
        // Surface as a done with undefined result; the SDK treats a missing
        // final message as a failed round.
        void err;
      });
  };

  const push = (event: T): void => {
    if (done) return;
    const waiter = waiting.shift();
    if (waiter) waiter({ value: event, done: false });
    else queue.push(event);
  };

  const end = (result?: unknown): void => {
    if (done) return;
    done = true;
    if (result !== undefined) resolveFinal(result);
    while (waiting.length > 0) {
      const waiter = waiting.shift()!;
      waiter({ value: undefined, done: true });
    }
  };

  return {
    push,
    end,
    result: () => {
      void ensureStarted();
      return finalResult;
    },
    async *[Symbol.asyncIterator]() {
      void ensureStarted();
      while (true) {
        if (queue.length > 0) {
          yield queue.shift() as T;
        } else if (done) {
          return;
        } else {
          const r = await new Promise<{ value?: T; done: boolean }>((res) =>
            waiting.push(res),
          );
          if (r.done) return;
          yield r.value as T;
        }
      }
    },
  } as unknown as PacedStreamLike<T>;
}
