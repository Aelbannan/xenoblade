import { test } from "node:test";
import assert from "node:assert/strict";
import {
  sessionsNeededForPool,
  effectiveRebatchBudget,
  partitionFailedTargets,
} from "../src/routing.ts";

// ── Rebatch budget derivation (LOW-4 / MEDIUM-1) ─────────────────────────

test("sessionsNeededForPool: pool 12 / batchSize 10 -> 2 sessions", () => {
  assert.equal(sessionsNeededForPool(12, 10), 2);
});

test("sessionsNeededForPool: pool 3 -> 1 session", () => {
  assert.equal(sessionsNeededForPool(3, 10), 1);
});

test("sessionsNeededForPool: exact multiples and empty pools", () => {
  assert.equal(sessionsNeededForPool(20, 10), 2);
  assert.equal(sessionsNeededForPool(0, 10), 0);
  assert.equal(sessionsNeededForPool(5, 0), 0);
});

test("effectiveRebatchBudget: configured 0 auto-derives to cover the pool once", () => {
  assert.equal(effectiveRebatchBudget(0, 12, 10), 2); // max(0, ceil(12/10))
  assert.equal(effectiveRebatchBudget(0, 3, 10), 1);
});

test("effectiveRebatchBudget: configured budget wins when larger than needed", () => {
  assert.equal(effectiveRebatchBudget(5, 12, 10), 5); // max(5, 2)
  assert.equal(effectiveRebatchBudget(5, 3, 10), 5); // max(5, 1)
});

test("effectiveRebatchBudget: never below sessions needed (no starvation)", () => {
  // Configured 1 but the pool needs 3 sessions -> 3 (each failed function
  // still gets its single rebatch attempt).
  assert.equal(effectiveRebatchBudget(1, 25, 10), 3);
});

// ── Pass-2 near-match partition (LOW-4 / r8 WS-2) ────────────────────────

function byId(entries: Array<[string, { status?: string; instructionMatch?: number }]>): Map<string, { status?: string; instructionMatch?: number }> {
  return new Map(entries);
}

test("partitionFailedTargets: near-match by instructionMatch >= 90, sorted desc", () => {
  const { nearMatch, rest } = partitionFailedTargets(
    ["us-A", "us-B", "us-C", "us-D"],
    byId([
      ["us-A", { instructionMatch: 95 }],
      ["us-B", { instructionMatch: 50 }],
      ["us-C", { instructionMatch: 90 }],
      ["us-D", { instructionMatch: 99 }],
    ]),
  );
  assert.deepEqual(nearMatch, ["us-D", "us-A", "us-C"]); // 99, 95, 90
  assert.deepEqual(rest, ["us-B"]);
});

test("partitionFailedTargets: CODE_MATCH / HIGH_MATCH status counts as near-match", () => {
  const { nearMatch, rest } = partitionFailedTargets(
    ["us-A", "us-B", "us-C"],
    byId([
      ["us-A", { status: "CODE_MATCH" }],
      ["us-B", { status: "STRUCTURAL" }],
      ["us-C", { status: "HIGH_MATCH", instructionMatch: 40 }],
    ]),
  );
  // Near-match sort is instructionMatch DESC: us-C (40) before us-A (none=0).
  assert.deepEqual(nearMatch, ["us-C", "us-A"]);
  assert.deepEqual(rest, ["us-B"]);
});

test("partitionFailedTargets: unknown targets / no metadata stay in rest, input order kept", () => {
  const { nearMatch, rest } = partitionFailedTargets(
    ["us-A", "us-B", "us-C"],
    byId([
      ["us-B", { instructionMatch: 91 }],
      // us-A and us-C are missing from byId
    ]),
  );
  assert.deepEqual(nearMatch, ["us-B"]);
  assert.deepEqual(rest, ["us-A", "us-C"]);
});
