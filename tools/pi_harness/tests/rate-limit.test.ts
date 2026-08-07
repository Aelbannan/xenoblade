import { test, describe } from "node:test";
import assert from "node:assert/strict";
import { Pacer, paceStream } from "../src/rate-limit.js";
import { createThrottledSettingsManager } from "../src/session.js";

describe("conditional SDK retry policy", () => {
  test("throttled settings manager disables SDK auto-retry (rpmLimit > 0)", () => {
    const sm = createThrottledSettingsManager();
    const retry = sm.getRetrySettings();
    assert.equal(retry.enabled, false, "SDK auto-retry must be OFF when throttled");
  });
});

// Manual clock: tests drive time + scheduled callbacks deterministically.
class ManualClock {
  nowValue = 0;
  timers = new Map<number, { at: number; fn: () => void }>();
  nextId = 1;

  now = (): number => this.nowValue;
  schedule = (fn: () => void, delayMs: number) => {
    const id = this.nextId++;
    this.timers.set(id, { at: this.nowValue + delayMs, fn });
    return id as unknown as ReturnType<typeof setTimeout>;
  };
  clear = (t: ReturnType<typeof setTimeout>) => {
    this.timers.delete(t as unknown as number);
  };

  /** Advance the clock and fire due timers (in order, repeatedly until stable). */
  advance(ms: number): void {
    const target = this.nowValue + ms;
    while (true) {
      const due = [...this.timers.values()]
        .filter((t) => t.at <= target)
        .sort((a, b) => a.at - b.at)[0];
      if (!due) break;
      this.nowValue = Math.max(this.nowValue, due.at);
      this.timers.delete([...this.timers.entries()].find(([, v]) => v === due)![0]);
      due.fn();
    }
    this.nowValue = target;
  }
}

describe("Pacer (process-wide request-rate limiter)", () => {
  test("first acquire grants immediately (no queue, interval elapsed)", async () => {
    const clock = new ManualClock();
    const p = new Pacer(20, clock); // 1 per 3000ms
    let granted = false;
    const promise = p.acquire().then(() => {
      granted = true;
    });
    await promise;
    assert.equal(granted, true);
    assert.equal(p.queueDepth, 0);
    p.dispose();
  });

  test("queued acquires are FIFO and paced at 1 per interval", async () => {
    const clock = new ManualClock();
    const p = new Pacer(20, clock); // interval 3000ms
    const order: number[] = [];
    // First acquire grants immediately.
    await p.acquire();
    // Next three queue.
    const a2 = p.acquire().then(() => order.push(2));
    const a3 = p.acquire().then(() => order.push(3));
    const a4 = p.acquire().then(() => order.push(4));
    assert.equal(p.queueDepth, 3);
    // No grant before the interval elapses.
    clock.advance(2999);
    assert.equal(order.length, 0);
    clock.advance(1);
    await Promise.resolve(); // let timer callbacks settle
    assert.deepEqual(order, [2]);
    clock.advance(3000);
    await Promise.resolve();
    assert.deepEqual(order, [2, 3]);
    clock.advance(3000);
    await Promise.resolve();
    assert.deepEqual(order, [2, 3, 4]);
    assert.equal(p.queueDepth, 0);
    p.dispose();
  });

  test("grants are spaced at least intervalMs apart (rate guarantee)", async () => {
    const clock = new ManualClock();
    const p = new Pacer(6, clock); // 1 per 10000ms
    const granted: number[] = [];
    // Keep a full queue for 2 minutes.
    const pend = (): void => {
      p.acquire().then(
        () => granted.push(clock.nowValue),
        () => {
          /* disposed at end */
        },
      );
    };
    for (let i = 0; i < 20; i++) pend();
    await Promise.resolve(); // drain microtasks: the immediate grant records at t=0
    for (let t = 0; t <= 120_000; t += 10_000) {
      clock.advance(10_000);
      await Promise.resolve();
      for (let i = 0; i < 3; i++) pend(); // keep demand
    }
    // Every pair of consecutive grants must be >= intervalMs apart.
    for (let i = 1; i < granted.length; i++) {
      const gap = granted[i] - granted[i - 1];
      assert.ok(
        gap >= 10_000,
        `grant ${i} at t=${granted[i]} is only ${gap}ms after t=${granted[i - 1]} (need >= 10000)`,
      );
    }
    p.dispose();
  });

  test("abort during queue rejects without consuming a token", async () => {
    const clock = new ManualClock();
    const p = new Pacer(20, clock);
    await p.acquire(); // slot taken
    const ac = new AbortController();
    const promise = p.acquire({ signal: ac.signal }).then(
      () => "resolved",
      (err) => `rejected:${(err as Error).message}`,
    );
    assert.equal(p.queueDepth, 1);
    ac.abort();
    const result = await promise;
    assert.ok(result.startsWith("rejected"), `expected reject, got ${result}`);
    // The aborted waiter must be removed — the next acquire should still need
    // to wait for the ORIGINAL slot, not steal the aborted one's position.
    assert.equal(p.queueDepth, 0);
    p.dispose();
  });

  test("dispose clears timers and rejects waiters", async () => {
    const clock = new ManualClock();
    const p = new Pacer(20, clock);
    await p.acquire();
    const promise = p.acquire().then(
      () => "resolved",
      () => "rejected",
    );
    p.dispose();
    assert.equal(await promise, "rejected");
    assert.equal(p.queueDepth, 0);
  });

  test("no timer leak after queue drains", async () => {
    const clock = new ManualClock();
    const p = new Pacer(20, clock);
    await p.acquire();
    const a2 = p.acquire();
    clock.advance(3000);
    await a2;
    // After all grants, no timers should remain scheduled.
    assert.equal(clock.timers.size, 0);
    p.dispose();
  });
});

describe("paceStream (provider stream gate)", () => {
  test("delays start until the gate resolves", async () => {
    const clock = new ManualClock();
    const p = new Pacer(20, clock);
    await p.acquire(); // take the open slot so the next acquire queues
    let started = false;
    const events: string[] = [];
    // Fake provider stream.
    const makeInner = () => {
      started = true;
      const inner: AsyncIterable<string> & { result(): Promise<unknown> } = {
        async *[Symbol.asyncIterator]() {
          yield "a";
          yield "b";
        },
        result: async () => "final",
      };
      return inner;
    };
    const gated = paceStream(p.acquire(), makeInner);
    assert.equal(started, false, "provider must not start before gate");
    const consumed: string[] = [];
    const done = (async () => {
      for await (const ev of gated) consumed.push(ev);
    })();
    await Promise.resolve();
    assert.equal(started, false, "still gated (slot not open yet)");
    clock.advance(3000); // open the slot
    await done;
    assert.equal(started, true);
    assert.deepEqual(consumed, ["a", "b"]);
    assert.equal(await gated.result(), "final");
    p.dispose();
  });

  test("aborted gate terminates the stream without starting the provider", async () => {
    const clock = new ManualClock();
    const p = new Pacer(20, clock);
    await p.acquire(); // slot taken
    let started = false;
    const ac = new AbortController();
    const gated = paceStream(p.acquire({ signal: ac.signal }), () => {
      started = true;
      return {
        async *[Symbol.asyncIterator]() {
          yield "x";
        },
        result: async () => "x",
      };
    });
    ac.abort();
    const consumed: string[] = [];
    for await (const ev of gated) consumed.push(ev);
    assert.equal(started, false, "provider must not start after abort");
    assert.deepEqual(consumed, []);
    p.dispose();
  });
});
