import { test } from "node:test";
import assert from "node:assert/strict";
import { stubRestoreWouldRegressRealBodies } from "../src/nearmiss.ts";

// ── Stub-poison guard (Kimi H2) ──────────────────────────────────────────
// A banked draft that still stubs `void func_XXXXXXXX(){}` must never be
// restored over a worktree that has since implemented that function.

test("stub guard: draft stub + worktree real body -> block restore", () => {
  const draft = "void func_80123456(){}";
  const worktree = "void func_80123456() { return compute(4); }";
  assert.equal(stubRestoreWouldRegressRealBodies(draft, worktree), true);
});

test("stub guard: draft stub + worktree stub too -> restore is harmless", () => {
  const draft = "void func_80123456(){}";
  const worktree = "extern \"C\" void func_80123456(){}";
  assert.equal(stubRestoreWouldRegressRealBodies(draft, worktree), false);
});

test("stub guard: no stubs in the draft -> never blocks", () => {
  const draft = "void func_80123456() { real(); }";
  const worktree = "void func_80123456() { real(); }";
  assert.equal(stubRestoreWouldRegressRealBodies(draft, worktree), false);
});

test("stub guard: extern \"C\" stub + whitespace variants detected", () => {
  const draft = 'extern "C" void func_8022ABCD() { }';
  const worktree = 'extern "C" void func_8022ABCD() { return other(); }';
  assert.equal(stubRestoreWouldRegressRealBodies(draft, worktree), true);
});

test("stub guard: multi-function file — one stale stub blocks the whole file", () => {
  const draft = [
    "void func_80000001() { done(); }",
    "void func_80000002(){}", // stale stub
  ].join("\n");
  const worktree = [
    "void func_80000001() { done(); }",
    "void func_80000002() { implemented(); }",
  ].join("\n");
  assert.equal(stubRestoreWouldRegressRealBodies(draft, worktree), true);
});

test("stub guard: symbol absent from the worktree also blocks (re-adds dead stub)", () => {
  const draft = "void func_80000003(){}";
  const worktree = "void func_80000001() { done(); }";
  assert.equal(stubRestoreWouldRegressRealBodies(draft, worktree), true);
});

test("stub guard: lowercase hex symbols are matched case-insensitively", () => {
  const draft = "void func_80123456(){}";
  const worktree = "void func_80123456() { implemented(); }";
  assert.equal(stubRestoreWouldRegressRealBodies(draft, worktree), true);
});
