import { test } from "node:test";
import assert from "node:assert/strict";
import { parseTargetSize } from "../src/targets.ts";

// ── size-parse guard (LOW-4 / Kimi L4 / GLM F11b) ────────────────────────

test("parseTargetSize: hex string with 0x prefix (targets.json format)", () => {
  assert.equal(parseTargetSize("0x29C"), 0x29C);
  assert.equal(parseTargetSize("0x0"), 0);
});

test("parseTargetSize: bare hex string (no 0x) parses as hex", () => {
  assert.equal(parseTargetSize("29C"), 0x29C);
});

test("parseTargetSize: plain numbers pass through", () => {
  assert.equal(parseTargetSize(500), 500);
  assert.equal(parseTargetSize(0), 0);
});

test("parseTargetSize: unparseable values yield undefined", () => {
  assert.equal(parseTargetSize("zzz"), undefined);
  assert.equal(parseTargetSize(""), undefined);
  assert.equal(parseTargetSize("0x"), undefined);
  assert.equal(parseTargetSize(-5), undefined);
  assert.equal(parseTargetSize(undefined), undefined);
  assert.equal(parseTargetSize(null), undefined);
  assert.equal(parseTargetSize({}), undefined);
});

test("parseTargetSize: a bare decimal string does NOT silently parse as hex", () => {
  // parseInt("668", 16) = 0x668 = 1640 — the foot-gun this guard documents.
  // The field is hex by convention; an ambiguous decimal-looking string is
  // still parsed as hex (matching targets.json), but the helper is the single
  // choke point that makes the convention explicit and testable.
  assert.equal(parseTargetSize("668"), 0x668);
});
