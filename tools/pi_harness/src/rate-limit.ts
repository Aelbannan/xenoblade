// ---------------------------------------------------------------------------
// Process-wide request-rate limiter (PACER) for the pi-harness.
//
// ONE HTTP request per `60_000 / rpm` ms, with NO burst allowance. This is a
// strict emission-interval pacer, NOT a token bucket with capacity=rpm (which
// would permit a 2x-rpm burst — the startup thundering herd). The pacer wraps
// `ModelRuntime.streamSimple` / `completeSimple` (the per-HTTP-request choke
// points: every assistant tool-call turn inside a session.prompt() is a separate
// streamSimple call), so it is genuinely process-wide, covering all 4
// runAgentSession call sites, every turn within rounds, agent-level retries and
// compaction.
//
// Semantics:
//  - FIFO resolve-queue. A single timer fires at each emission slot; it grants
//    the head waiter and schedules the next slot only while waiters remain.
//  - `acquire()` grants immediately when the pacer is idle and the current slot
//    is open (first request after init); otherwise it enqueues.
//  - `acquire()` is AbortSignal-interruptible: an aborted waiter is removed from
//    the queue, rejected with an AbortError, and does NOT consume a token (the
//    slot is re-served to the promoted waiter).
//  - When `rpm === 0` the pacer is inert (no pacing, immediate grants) and the
//    shared singleton is not installed — behavior is unchanged.
//  - Injectable `now()` + `tick()` for deterministic tests (no real timers).
// ---------------------------------------------------------------------------

import {
  lazyStream,
  type Api,
  type AssistantMessage,
  type AssistantMessageEventStream,
  type Context,
  type Model,
  type ModelsSimpleStreamOptions,
} from "@earendil-works/pi-ai";
import { SettingsManager } from "@earendil-works/pi-coding-agent";

export interface RateLimiter {
  acquire(opts?: { signal?: AbortSignal }): Promise<void>;
  dispose(): void;
  readonly rpm: number;
  readonly queueDepth: number;
}

interface Waiter {
  resolve: () => void;
  reject: (err: Error) => void;
  signal?: AbortSignal;
  onAbort?: () => void;
}

export interface RateLimiterOptions {
  /** Injected monotonic clock. When provided the limiter runs in MANUAL mode:
   *  no real timers are scheduled and time only advances via `tick(ms)`. */
  now?: () => number;
}

export class RateLimiterImpl implements RateLimiter {
  readonly rpm: number;
  readonly intervalMs: number;
  /** Manual mode (injected clock): `now()` reads this fake clock; `tick()`
   *  advances it. Real mode uses `performance.now()` + setTimeout. */
  private readonly manual: boolean;
  private time: number;
  private readonly queue: Waiter[] = [];
  /** Absolute (clock-unit) time the NEXT request may start. Advanced only on
   *  grant — never on enqueue — so aborted waiters consume no token. */
  private nextFreeAt: number;
  private disposed = false;
  private pumpTimer?: ReturnType<typeof setTimeout>;
  private pumpPending = false;

  constructor(rpm: number, opts: RateLimiterOptions = {}) {
    if (!Number.isInteger(rpm) || rpm < 0) {
      throw new Error(`rpm must be a non-negative integer (got ${String(rpm)})`);
    }
    this.rpm = rpm;
    this.intervalMs = rpm > 0 ? 60_000 / rpm : Infinity;
    this.manual = typeof opts.now === "function";
    this.time = this.manual ? (opts.now as () => number)() : 0;
    this.nextFreeAt = this.now();
  }

  get queueDepth(): number {
    return this.queue.length;
  }

  /** Current clock time (fake time in manual mode). Exposed for observability
   *  and so tests can record the exact grant timestamp. */
  get nowMs(): number {
    return this.now();
  }

  private now(): number {
    return this.manual ? this.time : performance.now();
  }

  acquire(opts: { signal?: AbortSignal } = {}): Promise<void> {
    if (this.disposed || this.rpm <= 0) return Promise.resolve();
    const now = this.now();
    // Idle pacer with an open slot → grant immediately (no burst: the slot is
    // reserved for the NEXT caller, so stack-up here just queues).
    if (this.queue.length === 0 && now >= this.nextFreeAt) {
      this.nextFreeAt = now + this.intervalMs;
      return Promise.resolve();
    }
    return new Promise<void>((resolve, reject) => {
      const waiter: Waiter = { resolve, reject, signal: opts.signal };
      waiter.onAbort = () => {
        const i = this.queue.indexOf(waiter);
        // Aborted while queued → remove WITHOUT consuming a token; the slot is
        // re-served to the promoted waiter.
        if (i >= 0) {
          this.queue.splice(i, 1);
          this.schedulePump();
        }
        waiter.signal?.removeEventListener("abort", waiter.onAbort!);
        const err = new Error("rate-limiter acquire aborted");
        err.name = "AbortError";
        reject(err);
      };
      if (opts.signal) {
        if (opts.signal.aborted) {
          waiter.onAbort();
          return;
        }
        opts.signal.addEventListener("abort", waiter.onAbort, { once: true });
      }
      this.queue.push(waiter);
      this.schedulePump();
    });
  }

  /** Advance the manual clock by `ms` and deliver any grants whose slots are
   *  now due. Only valid when an injected `now` was provided. */
  tick(ms: number): void {
    if (!this.manual) {
      throw new Error("RateLimiter.tick() requires an injected clock (pass now: () => number)");
    }
    if (!Number.isFinite(ms) || ms < 0) {
      throw new Error(`tick(ms) must be a non-negative number (got ${String(ms)})`);
    }
    this.time += ms;
    this.drainManual();
  }

  /** Deliver grants due as of the current fake time. Strict pacing means at
   *  most one grant is ever due at a time: after each grant the next slot is
   *  exactly `intervalMs` later, so a large tick still emits only one (the
   *  remainder requires more ticks) — no burst even after a long idle. */
  private drainManual(): void {
    while (!this.disposed && this.queue.length > 0) {
      const now = this.time;
      const slot = Math.max(this.nextFreeAt, now);
      if (slot > now) return; // nothing due yet — wait for more tick()
      this.time = slot; // keep consecutive grants intervalMs apart in fake time
      this.grantHead(slot);
    }
  }

  private grantHead(slot: number): void {
    const waiter = this.queue.shift()!;
    this.nextFreeAt = slot + this.intervalMs;
    waiter.signal?.removeEventListener("abort", waiter.onAbort!);
    waiter.resolve();
  }

  /** Ensure a real-timer pump is scheduled for the head waiter's slot. */
  private schedulePump(): void {
    if (this.disposed || this.manual || this.pumpPending || this.queue.length === 0) return;
    const now = this.now();
    const slot = Math.max(this.nextFreeAt, now);
    const wait = Math.max(0, slot - now);
    this.pumpPending = true;
    this.pumpTimer = setTimeout(() => {
      this.pumpPending = false;
      this.pump();
    }, wait);
    if (typeof this.pumpTimer.unref === "function") this.pumpTimer.unref();
  }

  private pump(): void {
    if (this.disposed || this.queue.length === 0) {
      this.pumpPending = false;
      return;
    }
    const now = this.now();
    const slot = Math.max(this.nextFreeAt, now);
    if (now < slot) {
      // Fired early (event-loop lag). Re-arm for the true slot.
      this.schedulePump();
      return;
    }
    this.grantHead(slot);
    this.schedulePump(); // schedule the NEXT head (queue non-empty)
  }

  dispose(): void {
    if (this.disposed) return;
    this.disposed = true;
    if (this.pumpTimer) clearTimeout(this.pumpTimer);
    this.pumpTimer = undefined;
    this.pumpPending = false;
    // Reject any still-queued waiters so nothing hangs on a disposed limiter.
    for (const waiter of this.queue.splice(0)) {
      waiter.signal?.removeEventListener("abort", waiter.onAbort!);
      const err = new Error("rate-limiter disposed");
      err.name = "AbortError";
      waiter.reject(err);
    }
  }
}

// ---------------------------------------------------------------------------
// Shared singleton — configured ONCE at orchestrator startup (right after
// ModelRuntime.create). Configured with rpmLimit>0 installs a live pacer;
// rpmLimit=0 (or never configured) leaves the singleton null, so downstream
// code (e.g. SDK auto-retry) is untouched.
// ---------------------------------------------------------------------------

let shared: RateLimiterImpl | null = null;

export function configureSharedRateLimiter(rpm: number): void {
  if (rpm <= 0) {
    shared = null;
    return;
  }
  if (shared) {
    if (shared.rpm !== rpm) {
      process.stderr.write(
        `[rate-limit] WARNING: configureSharedRateLimiter called with rpm=${rpm} but ` +
          `${shared.rpm} is already configured; keeping the first (must configure once at startup)\n`,
      );
    }
    return;
  }
  shared = new RateLimiterImpl(rpm);
}

export function getSharedRateLimiter(): RateLimiterImpl | null {
  return shared;
}

/** True when the shared limiter is installed and currently has queued waiters
 *  (a session waiting for a rate-limit slot emits no events, so the heartbeat
 *  must not flag it dead). Always false when throttling is off. */
export function sharedLimiterPending(): boolean {
  return shared !== null && shared.queueDepth > 0;
}

/**
 * Settings manager for a session, mirroring the throttle state of the shared
 * limiter (configured once at orchestrator startup).
 *
 * - rpmLimit = 0 (shared limiter not installed) → returns undefined →
 *   createAgentSession uses its default (file-backed) SettingsManager and SDK
 *   auto-retry stays ON (current behavior). HARD requirement: retry is never
 *   disabled on this path.
 * - rpmLimit > 0 (limiter installed) → returns an IN-MEMORY SettingsManager
 *   with `retry.enabled = false`. When the limiter owns pacing, an SDK
 *   auto-retry that 429s would merely re-enter the queue and re-consume a
 *   token (double-counting and letting retries slip past the pacer), so
 *   auto-retry is turned off and the harness+limiter own all backoff.
 *
 * InMemory is used (rather than SettingsManager.create + setRetryEnabled,
 * which calls save() and would PERSIST retry:false into the user's real
 * ~/.pi global settings file) so this has zero side effects on the user's
 * pi configuration.
 */
export function sessionSettingsManagerForThrottle(): SettingsManager | undefined {
  if (!shared) return undefined;
  return SettingsManager.inMemory({ retry: { enabled: false } });
}

// ---------------------------------------------------------------------------
// ModelRuntime wrapper. Installed once, immediately after ModelRuntime.create.
// Wraps streamSimple + completeSimple (the per-HTTP-request choke points) so a
// token is acquired BEFORE the underlying request is issued. `lazyStream` lets
// us return a live AssistantMessageEventStream synchronously while running the
// async acquire in its setup phase; an aborted acquire terminates the stream
// with an error event (the calling session was aborted anyway) and never issues
// a request. When throttling is off the runtime is returned untouched.
// ---------------------------------------------------------------------------

export function throttleModelRuntime(
  modelRuntime: {
    streamSimple: (
      model: Model<Api>,
      context: Context,
      options?: ModelsSimpleStreamOptions,
    ) => AssistantMessageEventStream;
    completeSimple: (
      model: Model<Api>,
      context: Context,
      options?: ModelsSimpleStreamOptions,
    ) => Promise<AssistantMessage>;
  },
  limiter: RateLimiter,
): void {
  const rawStream = modelRuntime.streamSimple.bind(modelRuntime);

  // Note: we deliberately use `rawStream(...).result()` here, NOT the original
  // completeSimple — the original completeSimple routes through
  // `this.streamSimple()`, which has been replaced by the wrapper above, so
  // calling it would acquire the pacer a SECOND time (double token). By using
  // the bound ORIGINAL streamSimple we pace the completion exactly once and
  // still deliver the same AssistantMessage. (compaction and every other
  // complete/completeSimple path ultimately run through streamSimple, so
  // wrapping that alone would also cover them — this explicit wrap keeps the
  // pacing of the completion path obvious and bounded.)
  modelRuntime.streamSimple = (model, context, options) =>
    lazyStream(model, async () => {
      await limiter.acquire({ signal: options?.signal });
      return rawStream(model, context, options);
    });

  modelRuntime.completeSimple = async (model, context, options) => {
    await limiter.acquire({ signal: options?.signal });
    return rawStream(model, context, options).result() as Promise<AssistantMessage>;
  };
}
