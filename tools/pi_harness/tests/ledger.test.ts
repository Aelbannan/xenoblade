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

// ── Restatement filtering (Kimi M2: one failed run must not count twice) ──

test("scanExhaustedTargets: target-skipped restatement reasons are excluded", () => {
  // One failed rebatch run logs batch-session-exhausted (targetIds) AND the
  // caller then target-skipped each member with the "below singletonMinSize"
  // restatement. Only the batch-session-exhausted record may count.
  const path = makeLedger([
    { event: "batch-session-exhausted", detail: { targetIds: ["us-F1", "us-F2"], phase: "rebatch" } },
    { event: "target-skipped", detail: { targetId: "us-F1", reason: "below singletonMinSize — rebatch was the only retry" } },
    { event: "target-skipped", detail: { targetId: "us-F2", reason: "below singletonMinSize — rebatch was the only retry" } },
    { event: "batch-session-exhausted", detail: { targetIds: ["us-F1", "us-F2"], phase: "rebatch" } },
    { event: "batch-session-exhausted", detail: { targetIds: ["us-F1", "us-F2"], phase: "rebatch" } },
  ]);
  // 3 real exhaustions (batch-session-exhausted rows) → exhausted; the 6
  // target-skipped restatements must NOT inflate the count.
  assert.deepEqual(scanExhaustedTargets(path), new Set(["us-F1", "us-F2"]));
});

test("scanExhaustedTargets: singleton retry restatements are excluded", () => {
  // runSingleton budget-exhausted calls handleSkipped internally AND the
  // Pass-2 caller calls it again — both are restatements of the same run.
  const path = makeLedger([
    { event: "batch-session-exhausted", detail: { targetId: "us-G1", singleton: true } },
    { event: "target-skipped", detail: { targetId: "us-G1", reason: "exhausted singleton retries" } },
    { event: "batch-session-exhausted", detail: { targetId: "us-G1", singleton: true } },
    { event: "target-skipped", detail: { targetId: "us-G1", reason: "maxAttemptsPerTarget exhausted (ledger)" } },
    { event: "batch-session-exhausted", detail: { targetId: "us-G1", singleton: true } },
  ]);
  assert.deepEqual(scanExhaustedTargets(path), new Set(["us-G1"]));
});

test("scanExhaustedTargets: non-restatement target-skipped reasons still count", () => {
  const path = makeLedger([
    { event: "target-skipped", detail: { targetId: "us-H1", reason: "budget" } },
    { event: "target-skipped", detail: { targetId: "us-H1", reason: "budget" } },
    { event: "target-skipped", detail: { targetId: "us-H1", reason: "budget" } },
  ]);
  assert.deepEqual(scanExhaustedTargets(path), new Set(["us-H1"]));
});

test("scanExhaustedTargets: mix of restatement and genuine skips needs 3 genuine records", () => {
  const path = makeLedger([
    { event: "batch-session-exhausted", detail: { targetIds: ["us-I1"] } },
    { event: "target-skipped", detail: { targetId: "us-I1", reason: "singleton retry disabled" } },
    { event: "batch-session-exhausted", detail: { targetIds: ["us-I1"] } },
    { event: "target-skipped", detail: { targetId: "us-I1", reason: "singleton retry disabled" } },
  ]);
  // 2 genuine + 2 restatements → NOT exhausted at threshold 3.
  assert.deepEqual(scanExhaustedTargets(path), new Set());
  assert.deepEqual(scanExhaustedTargets(path, 2), new Set(["us-I1"]));
});