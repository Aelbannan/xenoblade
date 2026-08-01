# 30 — Loop support: symbolic trips, skip guards, compare countdowns, body grammar

Status: **planned** (investigation complete — fresh retail census + engine verification;
see §1 for the numbers that drove this plan). No code changes yet.

Companion work docs: [28-loop-jt-corpus-hits.md](28-loop-jt-corpus-hits.md) (baseline census),
[SOUNDNESS.md](../SOUNDNESS.md) §loop obligations (traceability), README §supported model.

---

## 0. Problem statement

The equivalence engine's loop handling covers three closed forms — CTR-affine
(`loop_summary.py`), compare-affine (`loop_summary.py`), constant-stride store
loops (`memory_loop.py`) — plus a 256-iteration unrolling fallback
(`semantics.py::_execute_cfg_body`, `DEFAULT_MAX_LOOP_ITERATIONS = 256`). A
fresh census of the retail binary shows **zero exact recognizer hits** on any
loop shape (0 compare-affine, 0 CTR-affine, 0 memory-loop) and that every
retail loop fails closed at equivalence time unless it is a constant-trip
`bdnz` loop with trip ≤ 256. The shapes MWCC actually emits are structurally
recognizable but rejected for four concrete reasons, each mapped to a phase
below.

## 1. Findings recap (whole-codebase census, 19,308 functions)

| Metric | Value |
|---|---|
| Functions with ≥ 1 loop | 3,843 (19.9%) |
| Total loops (backward conditional branches) | 8,022 |
| Latch: `blt` / `bne` / `beq` / `ble` / `bgt` / `bge` | 3,412 / 1,810 / 329 / 269 / 72 / 47 |
| Latch: `bdnz` (CTR counted) | 2,083 (26%) |
| `bdnz` trip: constant ≤ 256 (unrollable **today**) | ~1,166 (56% of bdnz) |
| `bdnz` trip: constant > 256 (fails at bound) | ~59 |
| `bdnz` trip: symbolic (`srwi.`/`andi.`/reg-arith/`lwz`-of-var) | ~858 (fail closed) |
| mtctr ↔ header adjacency | 1,002 adjacent; **968 with a guard branch between** (46%) |
| Store-window bodies | 188 pure fill, 167 pure copy, 840 complex codec, 189 other |

Engine verification (live): CalcPitchRatio's symbolic-trip bulk+remainder loop
is `inconclusive_unsupported` against itself (`loop iteration limit exceeded`);
the identical loop with a concrete `li` trip is `equivalent` with
`affine-loop-summary` + `relational-induction`. Confirms symbolic trips are the
sole failure point for the 6 census-recognized hits.

**In-practice cost so far is low** (2 recorded `loop iteration limit exceeded`
in 13,834 attempts) but that is a selection artifact — equivalence only runs at
fuzzy ≥ 50 % and the matched corpus skews simple. The latent cost is ~20 % of
all functions.

## 2. Goals and non-goals

**Goals**

1. Make the engine prove `EQUIVALENT` for the retail loop shapes MWCC actually
   emits: symbolic-trip CTR loops (with proven zero-trip guards), guard-bearing
   loops, and compare-countdown loops — without weakening the existing
   discharged-obligation gating.
2. Widen affine/store body grammars to the memcpy/fill families.
3. Keep every new path fail-closed unless an obligation discharges, exactly as
   the current `UNSUPPORTED_FOR_EQUIVALENT` (empty ⇒ discharged-only) policy does.

**Non-goals (this iteration)**

- Full natural-loop relational induction with SMT-discharged while-loops
  (Phase E, deferred — largest population of complex bodies, hardest proof).
- General "side-effect-free body" acceptance without a per-op summary
  refinement proof.
- FP affine accumulation (`fmuls` chains in bodies) — rounding makes closed
  forms unsound; out of scope.

## 3. Design

### Phase A — Symbolic trip counts in the CTR-affine closed form

**Files:** `loop_summary.py`, `trip_expression.py`, `semantics.py`, `relational_discharge.py`,
`relational_induction.py`, `engine.py`, `proof_features.py`, tests.

**A1. Carry a `TripExpr` on candidates/summaries.**

- `CtrAffineLoopCandidate` gains `trip_expr: dict | None` (canonical AST) in
  addition to the concrete `trip_count`.
- Replace the `_concrete_trip_count` call in `find_ctr_affine_loop_candidates`
  with `recognize_trip_expr` (already implemented in `trip_expression.py`).
  Concrete materialization folds to `TripConstant` and keeps
  `confidence="exact-pattern"`; symbolic forms (TripEntryReg, TripAnd,
  TripLshr, TripAdd) get `confidence="symbolic-trip"`.
- `LoopSummary` gains `trip_expr`; `summarize_ctr_affine_loop` builds a
  parametric summary when the trip is symbolic **and** the zero-trip premise is
  established (Phase B). GPR-affine closed form
  `final = entry + trip_expr * stride` is exact in 32-bit BV semantics for any
  symbolic trip — repeated `addi` accumulates modulo 2³² exactly as one
  `mul`+`add` does — so no wrap gate is needed for GPR-only bodies (the module
  docstring's wrap caveat applies to memory loops only, see Phase D).

**A2. Symbolic application with the entry premise.**

- `apply_affine_loop_summary` evaluates `trip_expr` against the header entry
  state (`evaluate_symbolic(expr, state.gpr, ops)`), computes
  `delta = trip * stride` via `ops.mul`, updates summarized GPRs, sets
  `ctr = 0`.
- `_execute_cfg_body`'s affine branch mirrors the memory-loop pattern for
  symbolic trips: record a **premise-violation terminal**
  `condition ∧ ¬(CTR == trip ∧ trip ≥ 1)` (never pruned locally; the full
  solver context proves it unreachable). Concrete trips keep the current
  direct-apply path.

**A3. Parametric relational discharge.**

- `try_smt_discharge_ctr_affine` today pins `left0.ctr == BitVecVal(trip)`
  (a constant). Add a parametric mode: fresh symbolic `N` on both sides,
  premises `N ≥ 1`, `left0.ctr == N`, `right0.ctr == N`; all five UNSAT blocks
  run against `N`. Termination keeps proving CTR descent for all `N ≥ 1`.
- `_affine_bodies_match_for_discharge` accepts: both-concrete (equal), or
  both-symbolic with equal canonical trip expressions. Mixed concrete/symbolic
  is accepted when the concrete side folds to an equal `TripConstant`.
- Obligation impact: `loop_summary` keeps proof-feature name
  `affine-loop-summary`; `algorithm` becomes `affine-closed-form-v2` /
  `compare-affine-closed-form-v2`; obligation gains `trip_expr` and
  `zero_guard` fields. `relational_induction` blocks are unchanged in schema;
  their `algorithm` records the parametric variant. Result/certificate format
  bumps (see §5).

### Phase B — Skip-guard recognition (SMT-gated) + adjacency relaxation

**Files:** new `skip_guard.py`, `loop_summary.py`, `memory_loop.py`,
`bounded_remainder_loop.py`, `gx_fifo_loop.py`, tests.

**B1. Adjacency relaxation in all three finders.**

- Currently every finder requires `mtctr` at `header_index - 1`; retail puts a
  guard branch between mtctr and the header in ~46 % of loops. Allow mtctr at
  `header_index - 2..-4` when the instructions between are a single conditional
  branch that the guard recognizer validates (below).

**B2. Sound re-enablement of the zero-trip guard.**

- `bounded_remainder_loop._classify_zero_guard` currently hard-rejects every
  symbolic trip ("Heuristic skip-branch recognition is DISABLED for soundness")
  and `_has_skip_guard_before_header` is dead code. Re-enable only under an
  SMT discharge: prove `reach_header ⇒ trip ≥ 1`, i.e. the guard's
  not-taken condition implies the loop executes at least once. Use the existing
  `discharge_bad_conditions` machinery with two UNSAT checks
  (`guard_taken ∧ trip ≥ 1` and `¬guard_taken ∧ trip = 0`). The `beq` after
  `srwi.`/`andi.` (dot-form sets CR0) is the canonical shape; `ble`/`bge`/
  `blt` bound guards (skip bulk when `N < 8`) are the generalized predicate
  "skip when trip < 1".
- A guard that fails the SMT check leaves the loop `partial`/fail-closed —
  exactly today's behavior, never a new hole.

**B3. Guarded finite expansion reuse.**

- `apply_bounded_remainder_memory_loop` already does ite-guarded expansion for
  `N & mask` store loops; Phase B makes its `skip-branch` branch (currently
  unreachable dead code at `memory_loop.py:315`) live under the B2 discharge,
  and extends the same guarded expansion to affine GPR bodies
  (`trip` bounded by `TripAnd` mask ≤ 31, or `TripLshr` bulk with the
  bulk+remainder identity from `bulk_remainder_relational.py`).

### Phase C — Compare-countdown latch generalization

**Files:** `loop_summary.py`, `trip_expression.py`, `relational_discharge.py`, tests.

**C1. Latch grammar.** `find_compare_affine_loop_candidates` accepts only
`bne cr0.EQ` with an exact `addi rT,rT,-1; cmpwi rT,0` prelude — a shape with
**zero** retail hits. Extend the latch BO/BI set (AA=0):
`bne (4,2)`, `beq (12,2)`, `blt (12,0)`, `bgt (12,1)`, `ble (4,1)`,
`bge (4,0)`, with prelude `addi/subi rT, rT, ±step` then
`cmpwi rT, bound` (immediate, typically 0) **or** `cmpw rT, rB` (symbolic
bound). This covers the `blt` 3,412 / `bne` 1,810 populations.

**C2. Countdown trip expression.** New `TripExpr` nodes or composition
(`TripSub`, `TripDiv`-free form using the countdown step): for
`addi rT,rT,-step; cmpw rT, rB; bge latch`,
`trip = (entry - bound + step - 1) / step` (direction/sign per latch); final
counter value and CR field at exit are deterministic functions of
`entry, bound, step` and the signed/unsigned comparison — reuse
`FinalCompare` for CR agreement.

**C3. Closed form + discharge.** Same summary/application path as Phase A
(symbolic trip, entry premise, premise-violation terminal). Relational
discharge is parametric in `entry`/`bound` exactly as in A3. Body restriction
starts as ADDI-pure (Phase D widens it).

**C4. Pre-flight measurement (open question O1).** Before committing effort,
run the census body classifier on `blt`/`bne` latches to measure the
ADDI-pure subset — the 3,412 `blt` loops are CriWare-heavy and many have
complex bodies that Phase C will not cover.

### Phase D — Body grammar widening (memcpy/fill)

**Files:** `loop_summary.py` (`_parse_affine_body`), `memory_loop.py`
(`_parse_constant_stride_store_body`), `memory_loop_discharge.py` (reuse),
tests.

**D1. Store bodies.** Current parser accepts only `stwu` alone or exact
`store(disp==0); addi`. Add:
- multi-store fill bodies (same source reg/value, e.g. the `sth`-burst shapes);
- copy bodies `load; store` (and indexed `lhzx/sthx` pairs) where source and
  destination ranges are proven disjoint (or exactly-overlapping forward copy)
  via SMT or static range reasoning — the memmove/memcpy soundness check.

**D2. Affine bodies.** Replace "ADDI-only" with a whitelist of GPR-pure,
side-effect-free opcodes (addi/subi/add/subf/mulli/mr/or/xor/slwi/srwi/
rlwinm/extsb) under a **per-op summary refinement** in the
`memory_loop_discharge` style: build the per-iteration transition `T`,
prove `T^trip ≡ closed form` by induction UNSAT. Ops that write loop-invariant
registers or are dead at exit are summarized as no-ops; nothing is assumed
"side-effect-free" without the refinement proof.

**D3. FP bodies explicitly excluded** (fmuls chains) — rounding makes closed
forms unsound; leave on the unrolling fallback.

### Phase E — Deferred: natural-loop relational induction

`_sketch_natural_pair` remains a pending scaffold. When phases A–D land,
re-measure the residual: complex-body loops (the 840 codec windows, ~935
"mixed" bodies) are the remaining population. Requires ranking-function
inference + memory-aware invariants — the largest proof lift; do not start
before A–D are in and measured.

## 4. Soundness obligations

1. **Zero-trip `bdnz` wrap.** `CTR = 0` under `bdnz` wraps to `0xFFFFFFFF`
   (2³² − 1 iterations) — never a zero-trip loop. Every symbolic-trip summary
   requires a discharged guard (B2) or the premise-violation terminal (A2).
2. **Entry premise.** Summaries apply only under `CTR == trip ∧ trip ≥ 1`;
   the violation path is recorded as a terminal and must be proven unreachable
   by the full solver context — never pruned by local simplification.
3. **Parametric discharge.** EQUIVALENT authorization for symbolic trips
   requires all five relational UNSAT blocks discharged against symbolic `N`
   (A3), not a single concrete instance.
4. **Memory aliasing (Phase D).** Copy-body summaries require disjoint or
   exactly-overlapping-forward source/destination proof; overlap in the wrong
   direction or unknown aliasing fails closed.
5. **Compare countdowns (Phase C).** Exit agreement includes the counter's
   final value and the CR field (FinalCompare), not just the summarized GPRs.
6. **Obligation schema.** New fields (`trip_expr`, `zero_guard`) must be bound
   into `summary_sha256`; algorithm ids bump to `*-v2`. `status=discharged`
   remains reserved for engine paths with matching relational/refinement
   evidence; recognition or `coverage=applied` alone never authorizes
   EQUIVALENT (unchanged policy).

## 5. Certificate / result impact

- Result format 24 → 25: `loop_summary` obligation gains `trip_expr`,
  `zero_guard`, `algorithm: affine-closed-form-v2 | compare-affine-closed-form-v2`;
  relational blocks record the parametric algorithm.
- `validate_loop_summary_obligation` and `validate_relational_induction_obligation`
  extended; old certificates invalidated → recertification run
  (`targets recertify --bottom-up`).
- README architecture-model version and supported-model notes updated;
  SOUNDNESS.md traceability table gains rows for symbolic-trip closed form,
  skip-guard discharge, compare countdowns, and body refinement.

## 6. Test plan

| Gate | Phase | Content |
|---|---|---|
| Unit (recognition) | A–D | symbolic trip on candidate/summary; guard between mtctr and header; latch grammar BO/BI table; fill/copy bodies |
| Unit (closed form vs unrolling) | A–C | ConcreteOps: summarize vs unroll for `N ∈ {0..8}` on bulk+remainder, guard, and countdown shapes; states must match bit-for-bit |
| Unit (SMT discharge) | A, C | parametric discharge positive; soundness negatives: guard-not-proven fails closed, zero-trip wrap, mixed trip mismatch, aliased copy |
| Retail self-equivalence | A–C | decode the 6 known retail loops (CalcPitchRatio, AXVPB, OSExceptionInit) and the census `top_hits`; self-check must turn `equivalent` from `inconclusive_unsupported` |
| Census | A–D | `census_loop_patterns.py`: add `symbolic-trip` and compare-countdown columns; expect exact counts to rise from 0/2/4 to hundreds; track in doc 28 |
| Differential | all | `equivalence differential` (ConcreteOps corpus) stays green |
| Dolphin | n/a | fixtures gate ISA ops (mul/add/bc) which are unchanged; the closed forms are engine abstractions — no new Dolphin gate beyond the existing suite |
| CI | all | `gen_fixture_blob.py --check`, full unittest suite, docs_sync |

## 7. Acceptance criteria

- Census exact-pattern counts move from (compare-affine 0, ctr-affine 2,
  memory-loop 4) to: ctr-affine ≥ 400, memory-loop ≥ 200, compare-affine ≥ 100,
  measured on the current `build/us/asm` tree (exact targets to be pinned in
  doc 28 after the first Phase A/B landing).
- All 69 existing loop tests remain green; new negative tests (soundness) pass.
- Self-equivalence on the 6 known retail loops and on the census `top_hits`
  turns `equivalent` with `affine-loop-summary`/`memory-loop-summary` +
  discharged companions.
- No new `UNSUPPORTED_FOR_EQUIVALENT` entries needed — every new feature is
  fully obligated before it can authorize EQUIVALENT.

## 8. Sequencing and effort (estimate, agent-sessions)

| Step | Effort | Depends on |
|---|---|---|
| B1+B2 guard recognition + adjacency relaxation | 1–2 | — |
| A1+A2 symbolic trip closed form | 1 | B (zero-trip premise) |
| A3 parametric relational discharge | 1 | A1+A2 |
| Census tool: `symbolic-trip`/countdown columns + relative-root bugfix | 0.25 | — |
| C1+C2 compare countdown grammar + trip expr | 1–2 | O1 measurement, A3 discharge pattern |
| D1 fill/copy store bodies + aliasing | 1 | — |
| D2 affine body refinement | 1–2 | memory_loop_discharge pattern |
| Recertification + docs (SOUNDNESS, README, doc 28) | 0.5 | all |

Phases A+B are coupled for the zero-trip premise; C and D are independent.
Suggested order: B → A → C → D, re-running the census after each to validate
coverage deltas before the next phase.

## 9. Risks and open questions

- **O1 (blocks C):** quantify the ADDI-pure subset of `blt`/`bne` latches
  before investing in C — the 3,412 `blt` loops are CriWare-heavy and may be
  mostly complex bodies.
- **O2:** TripExpr countdown arithmetic — new AST nodes (`TripSub`,
  countdown-division encoded without general division) vs composition of
  existing nodes; prefer the smallest AST that canonicalizes for the discharge
  equality checks.
- **O3:** the per-op body refinement (D2) reuses the `memory_loop_discharge`
  "instructions ≡ summary" pattern; confirm it composes with the relational
  companion obligations before widening the whitelist beyond addi.
- **R1:** parametric discharge adds solver load (symbolic N); monitor
  `contract.timeout_ms` on the retail self-equivalence cases — the 20 s default
  should be ample for the narrow invariants.
- **R2:** mixed concrete/symbolic trip pairs in relational discharge must fold
  `TripConstant` correctly on both sides or they silently fail closed —
  covered by a negative test.
- **R3:** census `relative_to` crash on relative roots (found during
  investigation) — fix as part of the census-tool chore, not a phase.

## 10. References

- `tools/ppc_equivalence/loop_summary.py`, `trip_expression.py`,
  `relational_induction.py`, `relational_discharge.py`,
  `bulk_remainder_relational.py`, `bounded_remainder_loop.py`, `memory_loop.py`,
  `memory_loop_discharge.py`, `loop_cfg.py`, `semantics.py::_execute_cfg_body`
- `docs/ppc_equiv_work/28-loop-jt-corpus-hits.md` (baseline census)
- `SOUNDNESS.md` (obligation traceability), `README.md` (supported model)
- Census CLI: `tools/ppc_equivalence/census_loop_patterns.py`
