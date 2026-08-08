import { test, describe } from "node:test";
import assert from "node:assert/strict";
import { mkdtemp, mkdir, writeFile, rm, utimes } from "node:fs/promises";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { unitHasActionableWork, loadAllUnitSummaries, readTargetsFile, invalidateTargetsCache } from "../src/targets.ts";

async function makeRepo(targets: unknown[], ledgerLines: string[] = []) {
  const dir = await mkdtemp(join(tmpdir(), "pi-harness-targets-"));
  await mkdir(join(dir, "tools/coop"), { recursive: true });
  await mkdir(join(dir, "build/pi-harness"), { recursive: true });
  await writeFile(
    join(dir, "tools/coop/targets.json"),
    JSON.stringify({ targets }),
  );
  await writeFile(join(dir, "build/pi-harness/ledger.jsonl"), ledgerLines.join("\n"));
  return dir;
}

const T = (id: string, unit: string, status = "NOT_STARTED") => ({
  id, unit, source: `src/${unit.split("/").pop()}.cpp`,
  symbol: `func_${id}`, function: id, status, kind: "function", region: "us",
});

describe("targets.json in-memory cache", () => {
  test("readTargetsFile caches across calls and invalidates on mtime change", async () => {
    const dir = await makeRepo([T("a", "KY/Unit1"), T("b", "KY/Unit1")]);
    try {
      const r1 = readTargetsFile(dir);
      assert.equal(r1.length, 2);
      // Second call hits the cache (same content returned by reference).
      const r2 = readTargetsFile(dir);
      assert.equal(r2, r1, "cache should return the same array reference");
      // Invalidate, and the next read re-parses the (unchanged) file.
      invalidateTargetsCache(dir);
      const r3 = readTargetsFile(dir);
      assert.equal(r3.length, 2);
      assert.notEqual(r3, r1, "after invalidation a fresh parse occurs");
    } finally {
      await rm(dir, { recursive: true, force: true });
    }
  });

  test("readTargetsFile re-reads when the file mtime changes (new content served)", async () => {
    const dir = await makeRepo([T("a", "KY/U1")]);
    try {
      const before = readTargetsFile(dir);
      assert.equal(before.length, 1);
      // Rewrite with a different target, then force a future mtime.
      await writeFile(join(dir, "tools/coop/targets.json"), JSON.stringify({ targets: [T("a", "KY/U1"), T("b", "KY/U1")] }));
      const future = Date.now() / 1000 + 5;
      await utimes(join(dir, "tools/coop/targets.json"), future, future);
      const after = readTargetsFile(dir);
      assert.equal(after.length, 2, "mtime-driven invalidation should serve the new content");
    } finally {
      await rm(dir, { recursive: true, force: true });
    }
  });

  test("cached array is frozen (a caller cannot mutate the shared cache)", async () => {
    const dir = await makeRepo([T("a", "KY/U1"), T("b", "KY/U1")]);
    try {
      const r1 = readTargetsFile(dir);
      assert.ok(Object.isFrozen(r1), "returned cached array should be frozen");
      assert.throws(() => {
        (r1 as { pop(): unknown }).pop();
      });
      // Second read still returns the intact array.
      const r2 = readTargetsFile(dir);
      assert.equal(r2.length, 2);
    } finally {
      await rm(dir, { recursive: true, force: true });
    }
  });
});

describe("unitHasActionableWork (skip exhausted TUs in --all)", () => {
  test("unit with an unmatched non-exhausted target is actionable", async () => {
    const dir = await makeRepo([T("a", "KY/Unit1")]);
    try {
      // No ledger exhaustion for target "a" -> actionable.
      assert.equal(unitHasActionableWork(dir, "us", "KY/Unit1", "build/pi-harness/ledger.jsonl", 3), true);
    } finally {
      await rm(dir, { recursive: true, force: true });
    }
  });

  test("unit whose only remaining target is ledger-exhausted is NOT actionable", async () => {
    const led = [
      JSON.stringify({ ts: "2026-01-01T00:00:00Z", event: "batch-session-exhausted", tu: "KY/Unit1", detail: { targetIds: ["a"], targetId: "a" } }),
      JSON.stringify({ ts: "2026-01-01T00:00:01Z", event: "batch-session-exhausted", tu: "KY/Unit1", detail: { targetIds: ["a"], targetId: "a" } }),
      JSON.stringify({ ts: "2026-01-01T00:00:02Z", event: "batch-session-exhausted", tu: "KY/Unit1", detail: { targetIds: ["a"], targetId: "a" } }),
    ];
    const dir = await makeRepo([T("a", "KY/Unit1")], led);
    try {
      // 3 batch-session-exhausted for "a" >= threshold 3 -> exhausted -> not actionable.
      assert.equal(unitHasActionableWork(dir, "us", "KY/Unit1", "build/pi-harness/ledger.jsonl", 3), false);
    } finally {
      await rm(dir, { recursive: true, force: true });
    }
  });

  test("unit with a FULL_MATCH target + one NOT_STARTED remains actionable", async () => {
    const dir = await makeRepo([T("a", "KY/Unit1", "FULL_MATCH"), T("b", "KY/Unit1", "NOT_STARTED")]);
    try {
      assert.equal(unitHasActionableWork(dir, "us", "KY/Unit1", "build/pi-harness/ledger.jsonl", 3), true);
    } finally {
      await rm(dir, { recursive: true, force: true });
    }
  });

  test("fully-matched unit is not actionable", async () => {
    const dir = await makeRepo([T("a", "KY/Full", "FULL_MATCH"), T("b", "KY/Full", "FULL_MATCH")]);
    try {
      assert.equal(unitHasActionableWork(dir, "us", "KY/Full", "build/pi-harness/ledger.jsonl", 3), false);
    } finally {
      await rm(dir, { recursive: true, force: true });
    }
  });

  test("unit with all remaining targets at the session budget is kept (phase-3 TU-final escape)", async () => {
    // 3 dead-end records -> scanExhausted(threshold 3) marks it exhausted.
    // But countLedgerSessions counts batch-accept/batch-cycle too, so bump the
    // session count to maxAttemptsPerTarget=4 and the unit is kept for TU-final.
    const led = [
      JSON.stringify({ ts: "2026-01-01T00:00:00Z", event: "batch-session-exhausted", tu: "KY/Ex", detail: { targetIds: ["a"], targetId: "a" } }),
      JSON.stringify({ ts: "2026-01-01T00:00:01Z", event: "batch-session-exhausted", tu: "KY/Ex", detail: { targetIds: ["a"], targetId: "a" } }),
      JSON.stringify({ ts: "2026-01-01T00:00:02Z", event: "batch-session-exhausted", tu: "KY/Ex", detail: { targetIds: ["a"], targetId: "a" } }),
      JSON.stringify({ ts: "2026-01-01T00:00:03Z", event: "batch-cycle", tu: "KY/Ex", detail: { results: [{ targetId: "a", status: "COMPILES" }] } }),
    ];
    const dir = await makeRepo([T("a", "KY/Ex")], led);
    try {
      // exhausted (3 dead-end >=3) AND session count (4) >= maxAttempts(4)
      // -> phase-3 escape fires -> actionable (kept for TU-final).
      assert.equal(
        unitHasActionableWork(dir, "us", "KY/Ex", "build/pi-harness/ledger.jsonl", 3, false, 4),
        true,
        "TU-final-escape unit must be kept",
      );
      // Without maxAttempts (escape not considered), it is not actionable.
      assert.equal(
        unitHasActionableWork(dir, "us", "KY/Ex", "build/pi-harness/ledger.jsonl", 3, false),
        false,
      );
    } finally {
      await rm(dir, { recursive: true, force: true });
    }
  });
});

describe("loadAllUnitSummaries", () => {
  test("computes remaining > 0 for partially-matched units", async () => {
    const dir = await makeRepo([T("a", "KY/U", "FULL_MATCH"), T("b", "KY/U", "NOT_STARTED"), T("c", "KY/U", "COMPILES")]);
    try {
      const once = () => loadAllUnitSummaries(dir, "us", "smallest");
      const s = once();
      assert.equal(s.length, 1);
      assert.equal(s[0].remaining, 2);
      assert.equal(s[0].total, 3);
      assert.equal(s[0].matched, 1);
    } finally {
      await rm(dir, { recursive: true, force: true });
    }
  });
});
