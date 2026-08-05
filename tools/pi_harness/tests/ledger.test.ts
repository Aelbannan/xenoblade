import { mkdtempSync, writeFileSync } from "node:fs";
import { tmpdir } from "node:os";
import { join } from "node:path";
import { test } from "node:test";
import assert from "node:assert/strict";
import { scanExhaustedTargets } from "../src/ledger.ts";

function makeLedger(events: Array<{ event: string; detail: unknown }>): string {
  const dir = mkdtempSync(join(tmpdir(), "pi-ledger-"));
  const path = join(dir, "ledger.jsonl");
  const lines = events.map((e) => JSON.stringify({ ts: "2026-08-05T00:00:00Z", ...e }));
  writeFileSync(path, lines.join("\n") + "\n");
  return path;
}

test("scanExhaustedTargets: single record does NOT exhaust (threshold 3)", () => {
  const path = makeLedger([
    {
      event: "batch-session-exhausted",
      detail: { targetIds: ["us-AAAA"], outcome: "failed" },
    },
  ]);
  assert.deepEqual(scanExhaustedTargets(path), new Set());
});

test("scanExhaustedTargets: 3 records exhausts a target", () => {
  const path = makeLedger([
    { event: "batch-session-exhausted", detail: { targetIds: ["us-AAAA"], outcome: "failed" } },
    { event: "batch-session-exhausted", detail: { targetIds: ["us-AAAA"], outcome: "failed" } },
    { event: "batch-session-exhausted", detail: { targetIds: ["us-AAAA"], outcome: "failed" } },
  ]);
  assert.deepEqual(scanExhaustedTargets(path), new Set(["us-AAAA"]));
});

test("scanExhaustedTargets: mixed record types count toward the threshold", () => {
  const path = makeLedger([
    { event: "batch-session-exhausted", detail: { targetIds: ["us-BBBB"], outcome: "failed" } },
    { event: "target-skipped", detail: { targetId: "us-BBBB", reason: "budget" } },
    {
      event: "batch-cycle",
      detail: { results: [{ targetId: "us-BBBB", status: "NOT_BUILDABLE" }] },
    },
  ]);
  assert.deepEqual(scanExhaustedTargets(path), new Set(["us-BBBB"]));
});

test("scanExhaustedTargets: NOT_FOUND batch-cycle dead-ends count", () => {
  const path = makeLedger([
    { event: "batch-cycle", detail: { results: [{ targetId: "us-CCCC", status: "NOT_FOUND" }] } },
    { event: "batch-cycle", detail: { results: [{ targetId: "us-CCCC", status: "NOT_FOUND" }] } },
    { event: "batch-cycle", detail: { results: [{ targetId: "us-CCCC", status: "NOT_FOUND" }] } },
  ]);
  assert.deepEqual(scanExhaustedTargets(path), new Set(["us-CCCC"]));
});

test("scanExhaustedTargets: 2 records + custom threshold 2 exhausts; default 3 does not", () => {
  const path = makeLedger([
    { event: "batch-session-exhausted", detail: { targetIds: ["us-DDDD"], outcome: "failed" } },
    { event: "target-skipped", detail: { targetId: "us-DDDD", reason: "budget" } },
  ]);
  assert.deepEqual(scanExhaustedTargets(path, 2), new Set(["us-DDDD"]));
  assert.deepEqual(scanExhaustedTargets(path), new Set());
});

test("scanExhaustedTargets: target-hit in many BATCHES (not target-specific) counts once each batch", () => {
  // A single target drifting through 3 independent failed batches should
  // exhaust; but a target that only ever appeared in failed batches with
  // < threshold is retryable.
  const path = makeLedger([
    { event: "batch-session-exhausted", detail: { targetIds: ["us-E1", "us-E2"] } },
    { event: "batch-session-exhausted", detail: { targetIds: ["us-E1", "us-E3"] } },
    { event: "batch-session-exhausted", detail: { targetIds: ["us-E1", "us-E4"] } },
    { event: "batch-session-exhausted", detail: { targetIds: ["us-E5"] } },
  ]);
  assert.deepEqual(scanExhaustedTargets(path), new Set(["us-E1"]));
});