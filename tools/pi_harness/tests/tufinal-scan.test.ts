/**
 * Tests for the TU-final regression-aware tracking (tufinal-scan.ts):
 * scoring priority (match regressions > size/data > lint), witness-cert
 * awareness, and the loud feedback builder.
 */
import { test, describe } from "node:test";
import assert from "node:assert/strict";

import {
  targetOrder, isMatched, isCertified, diffUnitScans, scoreState,
  buildUnitFeedback, type UnitScan,
} from "../src/tufinal-scan.js";

function makeScan(over: Partial<UnitScan> = {}): UnitScan {
  return {
    unit: "demo/T",
    functions: [],
    matched: 0,
    total: 0,
    size_check: null,
    dataPercent: null,
    sizeOutput: "",
    witnessCert: new Map(),
    ...over,
  };
}

function fn(symbol: string, structural: number, mismatch: number) {
  return {
    symbol, present: true, match: structural === 0 && mismatch === 0,
    mismatch, structural, reg_swap: mismatch, pure_reg_swap: 0, reloc: 0,
    retail_size: 4, decomp_size: 4,
  };
}

// ---------------------------------------------------------------------------
describe("targetOrder / isMatched / isCertified", () => {
  test("targetOrder is lexicographic (structural dominates mismatch)", () => {
    assert.ok(targetOrder({ structural: 1, mismatch: 0 }) > targetOrder({ structural: 0, mismatch: 100 }));
    assert.ok(targetOrder({ structural: 0, mismatch: 5 }) > targetOrder({ structural: 0, mismatch: 3 }));
  });
  test("isMatched: only structural:0 AND mismatch:0", () => {
    assert.equal(isMatched({ structural: 0, mismatch: 0 }), true);
    assert.equal(isMatched({ structural: 0, mismatch: 3 }), false);
    assert.equal(isMatched({ structural: 2, mismatch: 0 }), false);
  });
  test("isCertified: reg-swap-only needs the witness cert", () => {
    assert.equal(isCertified({ structural: 0, mismatch: 0 }, undefined), true);
    assert.equal(isCertified({ structural: 0, mismatch: 4 }, true), true);
    assert.equal(isCertified({ structural: 0, mismatch: 4 }, false), false);
    assert.equal(isCertified({ structural: 0, mismatch: 4 }, undefined), false);
    assert.equal(isCertified({ structural: 3, mismatch: 0 }, true), false);
  });
});

// ---------------------------------------------------------------------------
describe("diffUnitScans", () => {
  test("flags a structural regression", () => {
    const base = makeScan({ functions: [fn("f1", 0, 0)] });
    const cur = makeScan({ functions: [fn("f1", 5, 5)], witnessCert: new Map() });
    const d = diffUnitScans(base, cur);
    assert.equal(d.length, 1);
    assert.equal(d[0].regressed, true);
    assert.equal(d[0].symbol, "f1");
    assert.deepEqual(d[0].before, { structural: 0, mismatch: 0 });
    assert.deepEqual(d[0].after, { structural: 5, mismatch: 5 });
  });

  test("flags a lost witness cert as a regression (structural:0 unchanged)", () => {
    const base = makeScan({ functions: [fn("f2", 0, 4)], witnessCert: new Map([["f2", true]]) });
    const cur = makeScan({ functions: [fn("f2", 0, 4)], witnessCert: new Map([["f2", false]]) });
    const d = diffUnitScans(base, cur);
    assert.equal(d.length, 1);
    assert.equal(d[0].regressed, true, "lost witness cert must be a regression");
    assert.equal(d[0].witnessBefore, true);
    assert.equal(d[0].witnessAfter, false);
  });

  test("flags an improvement (mismatch reduced)", () => {
    const base = makeScan({ functions: [fn("f3", 3, 9)] });
    const cur = makeScan({ functions: [fn("f3", 2, 4)] });
    const d = diffUnitScans(base, cur);
    assert.equal(d.length, 1);
    assert.equal(d[0].regressed, false);
  });

  test("no delta when unchanged", () => {
    const base = makeScan({ functions: [fn("f4", 0, 0)], witnessCert: new Map() });
    const cur = makeScan({ functions: [fn("f4", 0, 0)], witnessCert: new Map() });
    assert.equal(diffUnitScans(base, cur).length, 0);
  });
});

// ---------------------------------------------------------------------------
describe("scoreState priority", () => {
  const base = makeScan({ functions: [fn("a", 0, 0), fn("b", 0, 0)] });

  test("zero regressions + ok size + clean lint beats everything", () => {
    const good = makeScan({
      functions: [fn("a", 0, 0), fn("b", 0, 0)],
      size_check: { ok: true, budget: 100, retail_text: 100, decomp_text: 90, over_by: null, notes: "" },
      dataPercent: 95,
      witnessCert: new Map(),
    });
    const regressed = makeScan({
      functions: [fn("a", 0, 0), fn("b", 2, 2)],
      size_check: { ok: true, budget: 100, retail_text: 100, decomp_text: 90, over_by: null, notes: "" },
      dataPercent: 95,
      witnessCert: new Map(),
    });
    assert.ok(scoreState(good, base, 0) < scoreState(regressed, base, 0));
  });

  test("a match regression outweighs ANY lint improvement", () => {
    const regressedCleanLint = makeScan({ functions: [fn("a", 0, 0), fn("b", 4, 4)] });
    const improvedButLinty = makeScan({ functions: [fn("a", 0, 0), fn("b", 0, 0)], witnessCert: new Map() });
    assert.ok(
      scoreState(regressedCleanLint, base, 0) > scoreState(improvedButLinty, base, 50),
      "regression must dominate lint count",
    );
  });

  test("size over budget is worse than data% at same match state", () => {
    const over = makeScan({
      functions: [fn("a", 0, 0), fn("b", 0, 0)], witnessCert: new Map(),
      size_check: { ok: false, budget: 100, retail_text: 100, decomp_text: 150, over_by: 50, notes: "" },
      dataPercent: 95,
    });
    const under = makeScan({
      functions: [fn("a", 0, 0), fn("b", 0, 0)], witnessCert: new Map(),
      size_check: { ok: true, budget: 100, retail_text: 100, decomp_text: 90, over_by: null, notes: "" },
      dataPercent: 90,
    });
    assert.ok(scoreState(over, base, 0) > scoreState(under, base, 0));
  });

  test("lint violations are the tiebreaker after size/data", () => {
    const s1 = makeScan({ functions: [fn("a", 0, 0), fn("b", 0, 0)], witnessCert: new Map(), dataPercent: 90 });
    const s2 = makeScan({ functions: [fn("a", 0, 0), fn("b", 0, 0)], witnessCert: new Map(), dataPercent: 90 });
    assert.ok(scoreState(s1, base, 2) > scoreState(s2, base, 0));
  });
});

// ---------------------------------------------------------------------------
describe("buildUnitFeedback", () => {
  const base = makeScan({
    functions: [fn("fA", 0, 0), fn("fB", 0, 0)],
    size_check: { ok: true, budget: 100, retail_text: 100, decomp_text: 90, over_by: null, notes: "" },
    dataPercent: 92,
    witnessCert: new Map([["fA", true], ["fB", true]]),
  });

  test("states regressions loudly with symbols + before→after", () => {
    const cur = makeScan({
      functions: [fn("fA", 0, 0), fn("fB", 6, 6)],
      size_check: { ok: true, budget: 100, retail_text: 100, decomp_text: 90, over_by: null, notes: "" },
      dataPercent: 92,
      witnessCert: new Map([["fA", true], ["fB", true]]),
    });
    const text = buildUnitFeedback(base, cur, 0);
    assert.match(text, /MATCH REGRESSION/);
    assert.match(text, /`fB`/);
    assert.match(text, /structural 0→6/);
    assert.match(text, /mismatch 0→6/);
    assert.match(text, /TU size/);
    assert.match(text, /data section match/);
  });

  test("states a lost witness cert explicitly", () => {
    const cur = makeScan({
      functions: [fn("fA", 0, 0), fn("fB", 0, 4)],
      size_check: { ok: true, budget: 100, retail_text: 100, decomp_text: 90, over_by: null, notes: "" },
      dataPercent: 92,
      witnessCert: new Map([["fA", true], ["fB", false]]),
    });
    const text = buildUnitFeedback(base, cur, 0);
    assert.match(text, /MATCH REGRESSION/);
    assert.match(text, /WITNESS NO LONGER CERTIFIES/);
  });

  test("states improvements + size/data deltas", () => {
    const prev = makeScan({
      functions: [fn("fX", 4, 8)],
      size_check: { ok: true, budget: 100, retail_text: 100, decomp_text: 120, over_by: null, notes: "" },
      dataPercent: 80,
      witnessCert: new Map(),
    });
    const cur = makeScan({
      functions: [fn("fX", 0, 0)],
      size_check: { ok: true, budget: 100, retail_text: 100, decomp_text: 95, over_by: null, notes: "" },
      dataPercent: 95,
      witnessCert: new Map(),
    });
    const text = buildUnitFeedback(prev, cur, 0);
    assert.match(text, /Match improvements/);
    assert.match(text, /structural 4→0/);
    assert.match(text, /shrank by 25B/);
    assert.match(text, /data section match improved/);
  });

  test("no regression/improvement sections when identical", () => {
    const text = buildUnitFeedback(base, base, 0);
    assert.doesNotMatch(text, /MATCH REGRESSION/);
    assert.doesNotMatch(text, /Match improvements/);
    assert.match(text, /TU Unit Status/);
  });
});
