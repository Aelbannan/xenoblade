import { test, describe } from "node:test";
import assert from "node:assert/strict";
import { mkdtemp, mkdir, writeFile, rm } from "node:fs/promises";
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
