// Unit tests for the process-wide request PACER (tools/pi_harness/src/rate-limit.ts).
//
// Uses the injected manual clock (now + tick) for deterministic pacing tests —
// no real timers fire unless a test explicitly exercises the real-timer path.
import { test } from "node:test";
import assert from "node:assert/strict";
import {
  RateLimiterImpl,
  configureSharedRateLimiter,
  getSharedRateLimiter,
  sessionSettingsManagerForThrottle,
} from "../src/rate-limit.js";

const INTERVAL = (rpm: number): number => 60_000 / rpm;

test("pacer grants the first request immediately when idle", async () => {
  const limiter = new RateLimiterImpl(60, { now: () => 0 });
  await limiter.acquire();
  assert.equal(limiter.nowMs, 0); // granted at t=0, no wait
  assert.equal(limiter.queueDepth, 0);
  limiter.dispose();
});

test("pacer admits ~rpm per 60s with strict spacing (no bursts)", async () => {
  const rpm = 5; // 1 request per 12000ms
  const interval = INTERVAL(rpm);
  const limiter = new RateLimiterImpl(rpm, { now: () => 0 });

  const times: number[] = [];
  await limiter.acquire(); // immediate at t=0
  times.push(limiter.nowMs);

  const N = 12;
  const grants: Array<Promise<void>> = [];
  for (let i = 0; i < N; i++) {
    grants.push(limiter.acquire().then(() => times.push(limiter.nowMs)));
  }

  for (let i = 0; i < N; i++) {
    limiter.tick(interval); // advance one slot
    await grants[i];
  }

  // Strict spacing: every pair of consecutive grants is >= interval apart.
  for (let i = 1; i < times.length; i++) {
    assert.ok(
      times[i] - times[i - 1] >= interval,
      `grant ${i}: gap ${times[i] - times[i - 1]}ms < interval ${interval}ms`,
    );
  }

  // Sliding-window cap: at most `rpm` grants in ANY 60s (60000ms) window.
  for (let i = 0; i < times.length; i++) {
    const count = times.filter((t) => t > times[i] && t < times[i] + 60_000).length;
    assert.ok(count <= rpm, `window starting ${times[i]}: ${count} > rpm ${rpm}`);
  }

  limiter.dispose();
});

test("pacer is FIFO (fairness): waiters granted in arrival order", async () => {
  const limiter = new RateLimiterImpl(10, { now: () => 0 });
  const order: string[] = [];

  const a = limiter.acquire().then(() => order.push("a"));
  const b = limiter.acquire().then(() => order.push("b"));
  const c = limiter.acquire().then(() => order.push("c"));

  const interval = INTERVAL(10); // 6000ms
  limiter.tick(interval);
  await a;
  limiter.tick(interval);
  await b;
  limiter.tick(interval);
  await c;

  assert.deepEqual(order, ["a", "b", "c"]);
  limiter.dispose();
});

test("abort of the queued HEAD rejects without consuming a token", async () => {
  const limiter = new RateLimiterImpl(10, { now: () => 0 });
  const interval = INTERVAL(10); // 6000ms

  await limiter.acquire(); // immediate at t=0, nextFreeAt = 6000

  // Two queued: the first (b) holds the head slot at 6000.
  const bCtrl = new AbortController();
  const b = limiter.acquire({ signal: bCtrl.signal });
  const c = limiter.acquire();
  assert.equal(limiter.queueDepth, 2);

  // Abort b (the head) while queued.
  bCtrl.abort();
  await assert.rejects(b, (err: unknown) => (err as Error).name === "AbortError");

  // c was promoted to the head and must still be granted at the SAME slot
  // (6000) — the aborted waiter consumed no token (no extra wait).
  assert.equal(limiter.queueDepth, 1);
  limiter.tick(interval);
  await c;
  assert.equal(limiter.nowMs, interval); // granted at 6000 as originally scheduled

  assert.equal(limiter.queueDepth, 0);
  limiter.dispose();
});

test("abort of a mid-queue waiter does not disturb its neighbors", async () => {
  const limiter = new RateLimiterImpl(10, { now: () => 0 });
  const interval = INTERVAL(10);

  await limiter.acquire(); // t=0
  const a = limiter.acquire(); // slot 6000
  const midCtrl = new AbortController();
  const mid = limiter.acquire({ signal: midCtrl.signal }); // slot 12000
  const tail = limiter.acquire(); // slot 18000

  midCtrl.abort();
  await assert.rejects(mid, /abort/i);

  limiter.tick(interval);
  await a;
  limiter.tick(interval);
  await tail; // tail moves up, granted at 12000 (not 18000) — no wasted slot
  assert.equal(limiter.nowMs, interval * 2);
  assert.equal(limiter.queueDepth, 0);
  limiter.dispose();
});

test("dispose() clears the real timer and rejects queued waiters (no leak)", async () => {
  // Real clock path (no injected now) — exercises setTimeout scheduling.
  const limiter = new RateLimiterImpl(60);
  await limiter.acquire(); // immediate — no timer yet expected
  const q = limiter.acquire(); // queued → pump timer armed
  assert.equal((limiter as unknown as { pumpPending: boolean }).pumpPending, true);

  limiter.dispose();
  assert.equal((limiter as unknown as { pumpPending: boolean }).pumpPending, false);
  await assert.rejects(q, /disposed|aborted/i);
  assert.equal(limiter.queueDepth, 0);
});

test("real-timer pacer drains and leaves no pending timer", async () => {
  const limiter = new RateLimiterImpl(120); // 500ms interval
  await limiter.acquire(); // immediate
  const marks: number[] = [];
  const started = Date.now();
  // The pump timer is unref()ed (correct for production: it must not keep the
  // process alive), so the test keeps its OWN ref'd handle alive so the
  // unreffed pump timer actually fires while we await.
  const keepAlive = setInterval(() => {}, 1000);
  const second = limiter.acquire().then(() => marks.push(Date.now() - started));
  const third = limiter.acquire().then(() => marks.push(Date.now() - started));

  await second;
  await third;
  clearInterval(keepAlive);
  assert.equal(limiter.queueDepth, 0);
  assert.equal((limiter as unknown as { pumpPending: boolean }).pumpPending, false);
  // second and third must be spaced by the interval (500ms), with tolerance.
  assert.ok(marks[1] - marks[0] >= 400, `spacing ${marks[1] - marks[0]}ms`);
  limiter.dispose();
});

test("injected clock: no real timers are started in manual mode", async () => {
  const before = (process as unknown as { _getActiveHandles(): unknown[] })._getActiveHandles().length;
  const limiter = new RateLimiterImpl(10, { now: () => 0 });
  const a = limiter.acquire();
  limiter.tick(INTERVAL(10));
  await a;
  const after = (process as unknown as { _getActiveHandles(): unknown[] })._getActiveHandles().length;
  assert.ok(after <= before, "manual mode should not grow active handles");
  limiter.dispose();
});

// ── Shared singleton + conditional SDK auto-retry policy ──

test("rpmLimit=0: shared limiter is NOT installed and SDK auto-retry is NOT disabled", () => {
  configureSharedRateLimiter(0); // disabled → null
  assert.equal(getSharedRateLimiter(), null);
  // HARD requirement: when the throttle is off, we pass NO settingsManager,
  // so createAgentSession's default (SDK auto-retry ON) is used.
  assert.equal(sessionSettingsManagerForThrottle(), undefined);
});

test("rpmLimit>0: shared limiter installed and SDK auto-retry disabled in-memory", () => {
  configureSharedRateLimiter(60);
  const limiter = getSharedRateLimiter();
  assert.ok(limiter);
  assert.equal(limiter!.rpm, 60);

  const sm = sessionSettingsManagerForThrottle();
  assert.ok(sm, "throttle active → a settings manager is provided");
  assert.equal(sm!.getRetrySettings().enabled, false, "SDK auto-retry must be off when throttling");
  configureSharedRateLimiter(0); // cleanup
  assert.equal(getSharedRateLimiter(), null);
});

test("configureSharedRateLimiter: first config wins (no first-call-wins hazard)", () => {
  configureSharedRateLimiter(60);
  const first = getSharedRateLimiter();
  configureSharedRateLimiter(30); // would-be second config at a different rpm
  assert.equal(getSharedRateLimiter(), first, "keeps the first-configured limiter");
  assert.equal(first!.rpm, 60);
  configureSharedRateLimiter(0); // cleanup
});

// ── ModelRuntime wrapper ──

test("throttleModelRuntime: streamSimple paces the request before it starts", async () => {
  const { throttleModelRuntime, RateLimiterImpl } = await import("../src/rate-limit.js");
  const { createAssistantMessageEventStream } = await import("@earendil-works/pi-ai");

  let acquireCount = 0;
  const limiter = new RateLimiterImpl(60, { now: () => 0 });
  const origAcquire = limiter.acquire.bind(limiter);
  limiter.acquire = (o?: { signal?: AbortSignal }) => {
    acquireCount++;
    return origAcquire(o);
  };

  const msg = {
    role: "assistant", content: [], api: "x", provider: "p", model: "m",
    usage: { input: 0, output: 0, cacheRead: 0, cacheWrite: 0 },
    stopReason: "stop", timestamp: Date.now(),
  };
  let streamCalls = 0;
  const fakeRuntime = {
    streamSimple: () => {
      streamCalls++;
      const s = createAssistantMessageEventStream();
      queueMicrotask(() => { s.push({ type: "done", reason: "stop", message: msg }); s.end(msg); });
      return s;
    },
    completeSimple: async () => {
      // Mirrors the real ModelRuntime: routes through streamSimple.
      return (fakeRuntime.streamSimple() as { result(): Promise<unknown> }).result();
    },
  };

  throttleModelRuntime(fakeRuntime as never, limiter);

  // streamSimple: the acquire happens in the lazy setup (before the request).
  const stream = fakeRuntime.streamSimple();
  await new Promise((r) => setImmediate(r)); // let the lazy setup run
  assert.equal(acquireCount, 1, "streamSimple must acquire before starting");
  assert.equal(streamCalls, 1);
  await stream.result();
  limiter.dispose();
});

test("throttleModelRuntime: completeSimple acquires exactly ONCE (no double token)", async () => {
  const { throttleModelRuntime, RateLimiterImpl } = await import("../src/rate-limit.js");
  const { createAssistantMessageEventStream } = await import("@earendil-works/pi-ai");

  let acquireCount = 0;
  const limiter = new RateLimiterImpl(60, { now: () => 0 });
  const origAcquire = limiter.acquire.bind(limiter);
  limiter.acquire = (o?: { signal?: AbortSignal }) => {
    acquireCount++;
    return origAcquire(o);
  };

  const msg = {
    role: "assistant", content: [], api: "x", provider: "p", model: "m",
    usage: { input: 0, output: 0, cacheRead: 0, cacheWrite: 0 },
    stopReason: "stop", timestamp: Date.now(),
  };
  const fakeRuntime = {
    streamSimple: () => {
      const s = createAssistantMessageEventStream();
      queueMicrotask(() => { s.push({ type: "done", reason: "stop", message: msg }); s.end(msg); });
      return s;
    },
    completeSimple: async () => {
      // Mirrors the real ModelRuntime: routes through streamSimple.
      return (fakeRuntime.streamSimple() as { result(): Promise<unknown> }).result();
    },
  };

  throttleModelRuntime(fakeRuntime as never, limiter);

  const result = await fakeRuntime.completeSimple();
  assert.equal(acquireCount, 1, "completeSimple must consume exactly one token");
  assert.equal(result, msg);
  limiter.dispose();
});