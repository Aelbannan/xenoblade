# Implementation plan: register-renaming witness expansion (position-dependent rho)

**Status:** **implemented** (rev 5, after four adversarial review rounds; committed 2026-08-03) · **Scope:**
`tools/coop/lib/renaming_witness.py`, `tools/ppc_equivalence/semantics.py` (bounded executor
extension), `equivalence_check.py` (witness caller), `docs/ppc_equiv_work/31-reg-swap-witness.md`,
`SOUNDNESS.md`, tests, registry audit tooling
**Motivation:** reproducible audit (2026-08-03, `tools/coop/audit_witness_ws0.py` once committed
per §4-WS3.7; filters: `status ∈ {CODE_MATCH,HIGH_MATCH,STRUCTURAL}` ∧ `instruction_match ≥ 95` ∧
`has_indirect_calls = false`) on the current registry yields **283 near-match non-indirect
targets; ≈74 fail the witness only on the rho gate (Gate 4)** because their remaining diffs are
*local* register-allocation differences with no single global bijection. Of those: **≈55 are
callee-clean** (verified by running the *real* gate `_load_certified_callees` with transitive
re-attestation + self-recursion carveout, exactly as `_try_renaming_witness` does) and **≈19 are
callee-blocked** before the witness runs. **First-cut correction (rev 4):** of the callee-clean
rho-fail targets, **only ≈19 are loop-free** (no backward direct branch, no non-return
`bctr`/`bcctr`) and therefore reachable by the first-cut loop policy (§2.2) — the remainder are
loop-containing or indirect-branch targets that fall to SMT. Verified first-cut rollout examples:
`us-8003aa6c`, `us-801ccf18-2`, `us-8006c548` (NOT `us-8003ae8c` — loop-containing; see §4.1).
The counts in this header are **run-time snapshots** — the registry is live and the audit
subcommand re-computes them (drift observed between runs: 74↔75 rho-fail); never treat them as
fixed.

**Implementation status (2026-08-03):** WS1 (CFG-liveness fixpoint, PS1 defs,
first-cut loop predicate), WS2 (executor `stop_at_pcs` + `initial_seed` +
`pairs_checked == 0` guard), WS3 (region-sliced witness B: region splitting,
four-lane boundary deadness, per-exit region rho, cert payload
`rho_mode`/`regions` + amended assumptions), WS3.7 (committed audit tool
`tools/coop/audit_witness_ws0.py`), and WS4 (region-sliced test corpus) are
implemented. The audit reproduces the expected population: 275 near-match
non-indirect / 77 rho-fail / 55 callee-clean / 22 callee-blocked / 17
not_equivalent / 20 loop-free / 18 first-cut candidate pool (snapshot; registry
is live). The global-bijection witness path is byte-identical when no regions
split (existing certificates untouched); region slicing engages only when the
global path fails on the rho gate. First-cut loop / CX-2 (r0-as-temp) /
opaque-contract targets fail closed to SMT.

**Rev 5 changes (from round-4 reviews, `/tmp/witness_review4_glm52.md`,
`/tmp/witness_review4_kimi_k3.md`):** (1) PS1-def rule extended to the `lfs`/`lfsu`/`lfsx`/
`lfsux` single-precision FP **load** family (semantics.py:3613 defines `ps1[rt]`; modeled at
:5687) and the unknown-opcode over-approximation now covers **GPR/FPR/PS1**; (2) BCLR clause
reworded to **"non-link `bclr`"** (any `bclr` with `link=False` is a possibly-predicated return
tterminal, semantics.py:5516–5518 — `beqlr`/`bnelr` BO=4 included); (3) the `pairs_checked == 0`
guard message corrected to "all path conditions disjoint" and the two-status distinction
(empty-exits vs all-disjoint) stated; (4) loop scan stated as over the **full function CFG**,
not per-region; (5) loop predicate pinned as committed code + test fixture in the audit
subcommand (motivated by the round-3 Kimi/GLM 21-vs-19 disagreement, resolved in GLM's favor).

**Out of scope:** indirect-call targets (`bctrl`) — the witness's `execute_cfg` raises
`ExecutionInconclusive("call target None has no matched-callee lemma")` (semantics.py:5610)
regardless of rho; that is WS2 of the parked `indirect_call_certification_plan.md`. Also out of
scope: size / reloc / fields / mnemonic / reject-list / abi-boundary gate failures (real diffs),
spill-offset differences (Gate 3 rejects non-register immediates), and loop-containing targets in
the first cut (deferred to the fixpoint-enabled second cut).

---

## 1. Problem statement

`renaming_witness.py` certifies position-aligned, same-mnemonic pairs via *symbolic-variable
substitution on the initial state* (doc 31 §1, mandatory): `_symbolic_initial_pair` (line 664)
binds `retail.r_i == decomp.r_perm[i] == X_i` for **one global permutation**, both sides execute
through `execute_cfg`, and `_terminals_agree` (line 806) compares all state structurally under
that permutation.

Gate 4 (`check_gates`, line 595–613) builds rho as a **single injective bijection consistent at
every slot**. This is over-constrained: soundness only requires the correspondence to be
consistent **within a live range**. A register whose value dies before being redefined can legally
map to different candidate registers in different regions (the values never co-exist, so the
shared-variable model never observes a conflict). MWCC routinely does this — a temp lives in r5 in
one loop and r4 in another.

**Scope restriction (register-resident values only):** the model rebinds *registers*. Values that
flow through memory (spill `stw`/`lwz` pairs) are already handled by the shared memory array
(same offsets) or are genuinely out of scope (different offsets ⇒ Gate 3 non-register-bit reject).

---

## 2. Soundness formulation (mandatory)

### 2.1 Invariant: one shared variable per (value, live range), with two-direction deadness

The expanded model binds **`retail.r_i` (range k) ↔ `decomp.r_rho_k(i)` (range k)**, and
rebinding at a region boundary from `rho_k` to `rho_{k+1}` is sound **iff every changed binding is
dead on both sides at the boundary**. The condition, stated over **all changed bindings in both
directions**:

- **Retail direction:** for *every retail register `r` whose mapping changes*
  (`rho_k(r) ≠ rho_{k+1}(r)`): the old decomp lane `rho_k(r)` must be dead at the boundary on
  both sides.
- **Decomp direction:** for *every decomp register `d` whose retail preimage changes*
  (`rho_k^{-1}(d) ≠ rho_{k+1}^{-1}(d)`): the old retail lane `rho_k^{-1}(d)` must be dead at the
  boundary on both sides.

The rev-2 four-lane list is **superseded**: read literally it omitted the stale lane `m =
rho_k(i')` in its own counterexample (`rho_k(i)=j`, `rho_{k+1}(i')=j` with `i≠i'`). The two
directions together catch `m` (retail direction applied to `i'` requires `m = rho_k(i')` dead)
and every other changed binding (coverage proven in round-3 review; no counterexample found). The
invariant is literally "one shared variable per (value, live range)" — never "(register, range)".

### 2.2 Liveness is load-bearing — it must be a CFG fixpoint, with PS1 defs modeled

The existing `_liveness_sets` (`renaming_witness.py:407`) is **straight-line and ignores branches**
(its own docstring). A loop-carried value defined at the tail and used at the header is computed
*dead* at the tail ⇒ the deadness assertion would wrongly permit a rebinding on a live value —
silent divergence.

**Mandatory:** replace `_liveness_sets` with a **backwards dataflow fixpoint over the real CFG**
(the streams are position-aligned; branch targets are static operands decoded into
`Instruction.operands`; a backward edge is any direct `B`/`BC` whose target address < its own PC).
Unknown opcodes are treated as **use+def of all GPR/FPR/PS1** (over-approximate both directions;
the current `(), ()` default at `renaming_witness.py:306` under-approximates uses, which is the
unsound direction). Including PS1 in the default makes the over-approximation safe-by-construction
for any future non-reject opcode that reads `ps1` without entering `_use_def` (round-4 finding 7).

**PS1 liveness (rev-5 correction):** PS1 lanes are **not** "entry-liveness only". Three writer
classes define `ps1` and must be modeled by the WS1 fixpoint:

1. Scalar single-precision FP **arithmetic** — `fadds, fsubs, fmuls, fdivs, fres, fmadds, fmsubs,
   fnmadds, fnmsubs, frsp` (`_FP_SINGLE_ARITH`, semantics.py:2116–2118) — define `ps1[fd]`
   (semantics.py:4487–4488 `if is_single: state = state.with_ps1(fd, d_bits)`; fused-s at
   4313/4454; FRES exact-estimate at 2541; exact-v2 at 224).
2. Single-precision FP **loads** — `lfs, lfsu, lfsx, lfsux` — define `ps1[rt]`
   (semantics.py:3613 `if width == 4: state = state.with_ps1(rt, result)`; already modeled by the
   engine's own use-def table at semantics.py:5687).
3. `ps_*` paired-single ops (reject-listed, but still define `ps1` if the reject is ever lifted).

None of classes 1–2 are on the reject list (REJECT_OPCODES, renaming_witness.py:245–272). This
matters concretely: first-cut candidate `us-8006c548` uses both `lfs` and `frsp`. `FRSQRTE` is
correctly absent (it is in `_FP_DOUBLE_ARITH` and does not write `ps1`). No non-reject opcode
*reads* `ps1` (the only readers are reject-listed `ps_*`/`psq_*`), so missing a `ps1` def is the
over-approximate (safe) direction — completeness, never soundness.

**First-cut loop policy (per-target, unambiguous):**

1. Any target whose CFG contains a **backward direct branch** (`B`/`BC` with target < PC) is
   **rejected for expansion** in the first cut (falls through to SMT) — *regardless of whether
   the fixpoint could prove the conflicting lanes dead*. The "unless the fixpoint proves dead"
   clause of rev 2 is struck; a later cut may re-admit loop cases the fixpoint covers.
2. Any target containing a **non-return indirect branch** — `bctr`/`bcctr`, or a `bclr` with
   `link=True` (`blrl`) — fails closed to SMT in the first cut. **Any `bclr` with `link=False`
   is a (possibly predicated) return terminal, not an indirect branch** — the executor treats
   every non-link `bclr` as `return` regardless of BO/BI (semantics.py:5516–5518), including
   predicated returns `beqlr`/`bnelr` (BO=4) — and is **not** rejected: every function ends in
   `blr`, and a literal "any BCLR" reading would reject the entire population (round-3 finding
   (a); rollout example `us-801ccf18-2` contains a BO=4 `bclr`).
3. The `has_indirect_calls=false` WS0 filter is about *calls*; it does not exclude non-return
   indirect *branches* — the explicit check in (2) covers them.

**Audit consequence:** `check_gates` does **not** inspect branch targets, so the audit must run
its own loop predicate over the **full function CFG** (not per-region: the stream is decoded whole
and every `B`/`BC` target is scanned; a back-edge crossing a region boundary is still a backward
branch in the full CFG, so per-target rejection catches stitching-induced re-entry). Expected:
≈19 of the ≈55 callee-clean targets (round-3 census: 19 of 38 loop-containing; round-4
re-confirmed 19, resolving the Kimi 21-vs-GLM 19 disagreement in GLM's favor via correct operand
indexing — the predicate is pinned as committed code + a test fixture, never eyeball disasm).

**Escape-hatch caveat:** witness refusal degrades to SMT only when the probe is invoked with
`smt=True`; with `smt=False` the caller returns `INCONCLUSIVE_SMT_DISABLED`
(equivalence_check.py:2800–2804). Several candidate-pool targets sit at
`inconclusive_smt_disabled` today; for those, "falls to SMT" is aspirational until the workflow
runs them with `--smt`.

### 2.3 The structural backstop stays intact — and gets a hard guard

`_terminals_agree` compares **all 32 GPR lanes, all 32 FPR lanes, all 32 PS1 lanes, and the
memory array unconditionally**. B must **not** remove lanes from this comparison *except* lanes
rebound at a boundary **and** dead at the exit (their fresh shared variable is unobservable).
Precisely: at each terminal, a lane is compared under the exit's region rho; lanes dead at that
exit may hold divergent fresh variables but are masked **only** by the per-exit live-out set from
the §2.2 fixpoint. The plan does **not** weaken the comparison for any lane live at the exit.

**Hard guard (rev-4, soundness hole fix):** add to `run_structural_witness`
(renaming_witness.py:967–976, before the final `return WitnessOutcome(True, …)`):

```python
if pairs_checked == 0:
    return WitnessOutcome(False, rho=rho, failure=WitnessFailure(
        "structural", "all path conditions disjoint (no comparable terminal pairs)"))
```

**Two-status distinction (rev-5):** with the seed-time feasibility pre-check (§3.2) and the
region driver's ≥1-terminal assertion (§3.2) in place, the guard fires only for the
all-disjoint case (exits non-empty, every pair's combined condition provably unsat) — the
empty-exits case is caught earlier by those two mechanisms. Rejecting the all-disjoint case is
correct: under the shared-variable model, total disjointness means the sides partition the input
space differently ⇒ genuine divergence ⇒ fail closed to SMT (never a vacuous-true certificate).
Both sub-cases are sound-rejects; the guard is the backstop, the other two are defense-in-depth.

**Masking/memory soundness argument (must appear in SOUNDNESS.md):** a dead-at-exit lane's
divergent value can escape into observable state only via (i) memory — caught, because the memory
array is shared verbatim and compared unconditionally (`renaming_witness.py:890`); (ii) LR /
exit_target — never masked (`_value_equal` path); (iii) CR/XER/FPSCR — compared directly, never
renamed. A spill of a divergent lane exists on both sides (stores are position-aligned
same-mnemonic) and diverges in the shared array ⇒ caught.

### 2.4 Memory and non-register state are shared verbatim, untouched by rebinding

Memory, CR, XER, FPSCR, LR, CTR, GQR/SR/SPR tuples, and all non-register state are shared
verbatim between the sides (as today) and are **never** rebound. The memory comparison at the
terminal stays structural.

### 2.5 Call model: rebinding across calls only for contract-written / proven-dead lanes

`semantics.py:1539`: an opaque `"*"` contract's call token takes **every** register as an
argument — rebinding any lane across such a call changes the callee's inputs and is **unsound
regardless of caller-side deadness**. Across-call rebinding is permitted **only** when:

- the callee contract is **precise** (no `"*"` in `reads`) **and** the rebound lane is not in
  `contract.reads` (so it does not enter the token), **and**
- the lane is dead at the call site by the §2.2 liveness, or is in `contract.writes` (clobbered —
  `_apply_call_summary` (semantics.py:4622) freshens only GPRs 0,3–12 **listed in `contract.writes`**;
  a volatile not in `writes` keeps its pre-call value and may be read afterwards).

### 2.6 The CX counterexamples stay rejected

The correspondence is between *values*, never operand positions (doc 31 CX-1: `slw r3,r4,r5` vs
`slw r3,r5,r4` must still reject). Both sides execute through the audited `execute_cfg` (never
operand rewriting), and Gate 3 still requires non-register bit equality. CX-1..CX-4 remain
regression tests, unchanged.

### 2.7 The `not_equivalent` population is not permanently excluded

The SMT probe runs both sides from a **single shared identity initial state**
(`_symbolic_initial`, engine.py:83–110; used at `_check_equivalence_impl` 1386 and
`execute_cfg` calls 1568/1586) — it proves equivalence under the **identity** register mapping
with no renaming freedom. A `not_equivalent` verdict therefore means "divergent when retail and
decomp start from identical register values", which is exactly what *any* register-renaming
difference produces — it does **not** distinguish true-Chaitin interference (unrescuable) from a
per-region rename that region-sliced rho can capture (rescuable). **Rev-4 policy:** the region-rho
witness **runs on the `not_equivalent` targets too** and decides (sound: certifies the rescuable
subset, rejects the rest). The rev-3 "permanently out" bucket is **struck**. Where a concrete
counterexample exists (`counterexample_kind == "concrete"` with a successful ConcreteOps replay,
engine.py:2522–2530), the exclusion can be recorded as "replay-validated not_equivalent" — but
symbolic-only `not_equivalent` (replay blocked) must not be treated as conclusive.

---

## 3. Design

### 3.1 Expansion B (the core): position-dependent rho / per-live-range renaming

1. **Region segmentation:** split the instruction stream at (a) every Gate-4 conflict slot,
   (b) call sites, (c) any slot where a live-across-call volatile's binding changes. Between
   split points a single consistent bijection holds (per-region Gate-4 accumulation).
2. **Boundary assertion:** at each boundary, prove the §2.1 **two-direction** deadness via the
   §2.2 fixpoint. Any lane live across the boundary where a binding changes ⇒ fail closed.
3. **Execution:** region-boundary capture/resume via the WS2 executor extension (§3.2). State is
   carried across regions; fresh shared variables inserted at the boundary for changed lanes.
4. **Terminal comparison:** `_terminals_agree` indexes lanes by the *exit's* region rho; masked
   lanes (§2.3) use the per-exit live-out set; the `pairs_checked == 0` guard (§2.3) applies.
5. **Gate 5 per region:** `_check_abi_fixedness` (line 625) iterates every region's rho — r0, r1,
   r2, r13, args/returns (r3–r10, f1–f8) identity in *every* region; live-across-call volatiles
   fixed within their region (the function-wide `live_across` union, lines 431–436, is the
   conservative base).

### 3.2 Executor extension (explicit core change; rev-4 corrected)

`_execute_cfg_body` (semantics.py:4997) uses a worklist
`work: list[tuple[pc, state, condition, visit_counts, steps]]` (5001–5003), LIFO `work.pop()`,
`enqueue` closure (5015) that threads `(next_pc, next_state, next_condition, next_visits,
next_steps)` and prunes by `_path_condition_feasible` + `max_paths`. Path conditions are threaded
per-frontier-entry, accumulated via `ops.land` at each branch.

**Extension (two additive kwargs, defaulted None → all existing callers unchanged; verified: 30+
callers pass keywords only; all params after `ops` are keyword-only):**

1. `stop_at_pcs: frozenset[int] | None = None` — when a popped frontier entry's `pc ∈
   stop_at_pcs`, **collect `(pc, state, condition, visit_counts, steps)` into a paused list and
   do not enqueue successors** (the pause is at pop-time, *before* executing the boundary
   instruction). This single interception point covers every enqueue site.
2. `initial_seed: tuple[int, MachineState, Any, dict, int] | None = None` — overrides the
   hardcoded `(start, state, ops.bool(True), {}, 0)` seed (the **only** construction point,
   semantics.py:5001–5003), so the region driver can resume with
   `(boundary_pc, paused_state, conjoined_condition, paused_visits, paused_steps)`.

**Resume semantics:** because the pause happens at pop-time *before* the boundary instruction
executes, the region driver resumes the next region **at the boundary PC** (not `pc+4`),
re-executing the boundary instruction under the next region's binding. This matters doubly: when
the boundary is a Gate-4 conflict slot (§3.1.1(a)), that is precisely the instruction whose
renamed operands matter. When the boundary is a call site, the pause must land on the
**call-side** of call-summary application. The driver must also:
- thread the **function base** (not the region's first address) into `_value_equal` for the LR
  carve-out (bases come from `original[0].address`, renaming_witness.py:945–946);
- account `max_paths`, `max_instructions`, and **`visit_counts` carryover** across the
  cross-product of region path counts (`visit_counts` guards `max_loop_iterations`,
  semantics.py:4999–5000 — dropping it silently disables the loop bound). The mechanism must be
  driver-side (pass a reduced `max_paths` per region, or track cumulative counts): `enqueue`'s
  `max_paths` accounting is `len(work) + len(terminals)` **per region**, so without driver-side
  summation the cross-region budget silently resets each region;
- **pre-check the resumed condition's feasibility at seed time** (the seed bypasses `enqueue`'s
  feasibility check — it is appended literally at 5001–5003 — so an infeasible resumed frontier
  would otherwise execute until its first branch/terminal prune);
- on budget exhaustion, fall to SMT (never weaken a comparison).

**Fail-safety analysis (rev-4 corrected):** a *missing* conjunct in the resumed condition is
fail-safe for soundness (more satisfiable ⇒ disjoint-pair skip under-fires ⇒ over-checking ⇒
spurious rejection, never a false certificate — renaming_witness.py:913). A *contradictory*
condition is **not** fail-safe: it prunes both sides' frontiers → empty exits → the §2.3
`pairs_checked == 0` guard is what prevents a false EQUIVALENT. Both directions get WS4 tests.

This is the repo's most soundness-sensitive file; the change must be additive, code-reviewed, and
covered by the corpus + differential suites **before** any witness code builds on it. Doc 31's
"do not write a new interpreter" rule is respected — this is a bounded extension of the existing
executor, not a new one.

### 3.3 Expansion A is not a deliverable

Standalone A (liveness-gated Gate-4 conflicts with a single global permutation) cannot certify
anything: `_terminals_agree` compares all 32 lanes regardless of deadness, so a conflict lane left
unpaired under a single permutation fails structurally. A's Gate-4 relaxation survives only as the
region-split *trigger* inside B (§3.1.1).

### 3.4 Spill-based differences: not register renaming

- Different spill offsets (`stw r5,8(r1)` vs `stw r6,16(r1)`): Gate 3 rejects (non-register
  immediate). Out of scope, documented.
- Same-offset spill-rename: value flows through the shared memory array — **already handled
  today**, no expansion needed.

---

## 4. Implementation steps

### Workstream 1 — CFG-liveness foundation (2–3 days)

1. Replace `_liveness_sets` with a **backwards dataflow fixpoint** over the real CFG; emits
   per-slot live-in/live-out and per-exit live-out.
2. Unknown opcodes → **use+def of all GPR/FPR** (both directions). Verify reject-list opcodes
   (Gate 6) are never granted rebindings.
3. **PS1 defs (rev-5):** every scalar-s FP arithmetic op (`_FP_SINGLE_ARITH`,
   semantics.py:2116–2118), every single-precision FP load (`lfs, lfsu, lfsx, lfsux`,
   semantics.py:3613 / :5687), and every `ps_*` op is a **def of `ps1[fd]`/`ps1[rt]`** (§2.2).
   Unknown opcodes: use+def of all GPR/FPR/**PS1** (§2.2).
4. **First-cut loop policy:** per-target rejection of (a) any backward direct branch, (b) any
   non-return `bctr`/`bcctr`/`bclr` (§2.2.1–2.2.2, unambiguous; return `blr` is a terminal and
   NOT rejected).
5. **Tests:** straight-line fixtures, loop back-edge fixtures (must reject), call boundaries with
   `live_across`, live-in-at-entry, use-before-def, unknown-opcode fixture, PS1 fixture (scalar-s
   def then use across a boundary).

### Workstream 2 — bounded executor region extension (3–5 days + review)

1. `stop_at_pcs` pause mode (§3.2.1) — pop-time interception, paused-list collection.
2. `initial_seed` resume mode (§3.2.2) — entry-pc/state/condition/visits/steps injection.
3. **`pairs_checked == 0 ⇒ fail` guard** in `run_structural_witness` (§2.3) — lands in this
   workstream with the executor changes it protects (or earlier, standalone: it is a one-line
   hardening of today's path).
4. Region driver: resume **at the boundary PC** (not `pc+4`), call-side pause placement,
   function-base threading, `max_paths`/`max_instructions`/`visit_counts` cross-product
   accounting (driver-side mechanism), seed-time feasibility pre-check, SMT fallback on budget
   exhaustion.
5. Additive guarantee: all existing callers unchanged (kwarg defaults None); corpus + differential
   suites pass before witness work builds on it.

### Workstream 3 — witness B (3–5 days + review)

1. `split_rho_regions` (§3.1.1) + per-region Gate-4 accumulation.
2. Two-direction boundary deadness assertion (§2.1) over WS1 liveness.
3. `_check_abi_fixedness` per-region (§3.1.5).
4. `_terminals_agree` per-exit region rho + masked-lane logic (§2.3) + the `pairs_checked` guard.
5. `Rho` dataclass: `regions: tuple[RhoRegion, ...] | None` — `None` keeps the current
   single-bijection path byte-identical.
6. **Certificate format:** `rho.to_dict()` is **not** a `proof_request_hash` input today
   (verified: equivalence_check.py:1505–1531 passes code hex, contracts, limits, callees, bases,
   relocs, target_id — no rho; `proof_request_hash` hashes only `proof_request_identity`,
   provenance.py:367). **Do not add rho/regions to hash inputs** — rho is a proof *output*,
   code-derived. Keep `regions`/`rho_mode` in the certificate payload only. Update the
   certificate `assumptions` text (equivalence_check.py:1542–1556: "consistent across all
   mnemonics and positions" becomes false under region-slicing) — the text change is body-only,
   no hash impact. Evidence string stays `register-renaming-witness` (whitelist untouched,
   targets.py:104). `result_format` (24) may stay if the payload change is additive, **with
   explicit reviewer sign-off** on the semantic change of recorded rho.
7. **Registry audit tooling (committed, not /tmp):** add `tools/coop/audit_witness_ws0.py` (or a
   `run.py` subcommand) that computes the population by calling `check_gates` **and**
   `_load_certified_callees` (transitive re-attestation + self-recursion carveout) **and** the
   WS1 first-cut loop predicate — implemented as **committed executable code with a test fixture**
   (known-loop and known-loop-free targets), never prose/eyeball disasm (round-4 finding 8:
   two reviewers disagreed 21-vs-19 on the same targets; the code must be machine-checkable and
   reviewer-independent). The predicate: full-function-CFG backward `B`/`BC` scan (relocated
   branches excluded from the static-target test **except** relocated branches whose symbol is
   the local function — direct recursion — which must be flagged as back-edges, not silently
   skipped) + non-return-`bclr`/`bctr`/`bcctr` scan. Emits counts with a "snapshot, recomputed
   at run time" header and records per-target `witness_failure_gate` (including `loop` /
   `indirect-branch` classification buckets, not just `rho`).

### Workstream 4 — soundness, tests, rollout (2–3 days)

1. **Positive cases:**
   - Synthetic: straight-line `li r5,..; …; mr r3,r5` vs `li r4,..; …; mr r3,r4`, temp dead at
     exit (single region — regression).
   - Region case: value in r5 region 1 (decomp r4), dead, redefined in r5 region 2 (decomp r5) —
     the core B positive.
   - Call-boundary rebind: volatile in `contract.writes` (or dead + not in `contract.reads`)
     under a **precise** contract.
   - FPR/PS1 lane positive (PS1 compared under `fpr_perm`; scalar-s def then use).
   - Real: from the **first-cut-reachable pool (≈19, recomputed)** land the first several with
     `cycle`/`hexdiff` evidence. **Verified rollout examples: `us-8003aa6c`, `us-801ccf18-2`,
     `us-8006c548`** (all callee-clean, non-loop, non-not_equivalent at commit; re-verify via the
     audit subcommand at rollout time). Do NOT use `us-8003ae8c` (loop-containing) or
     `us-80381cf8`/`us-802e21ac` (transitively callee-blocked).
2. **Negative cases (must reject / fall to SMT):**
   - CX-1 shift-count swap (`slw r3,r4,r5` vs `slw r3,r5,r4`) — regression.
   - Loop back-edge carried value (canary; first cut rejects, later cut requires fixpoint proof).
   - Non-return `bctr`/`bcctr` (jump-table) target — rejected by §2.2.2; plain return `blr` —
     **not** rejected (regression that the BCLR carve-out works).
   - Inverse-lane staleness (§2.1: `rho_k(i)=j`, `rho_{k+1}(i')=j`, stale decomp lane `rho_k(i')`
     read in region k+1).
   - Live-in rebind at entry (region-0 rho differing on a lane live-in at entry, use before def).
   - Live-in-at-boundary (decomp side): the rebind *introduces* a clobber of an already-live
     decomp lane at an arbitrary boundary.
   - Sibling-lane conflict: conflicting lane dead at a slot but a *different* lane of the same
     instruction live (Gate 4 must still accumulate rho from live lanes).
   - Opaque `reads=['*']` contract with any non-identity/rebound lane (existing
     `test_opaque_eabi_callee_rejected` regression, asserted under the region path).
   - Precise contract whose `reads` includes the rebound lane.
   - True Chaitin interfere: two live values in one register simultaneously — must reject (the
     witness decides; see §2.7).
   - Spill pair with differing offsets (Gate-3 reject, documented out of scope).
   - Multi-exit function with per-exit region rhos (terminal pairing uses each exit's own rho).
   - Region split coinciding with a call site (ordering of call-summary vs rebind).
   - Masked-lane memory-spill negative: a lane dead at exit whose divergent value was stored to
     memory before death — must be caught by the structural memory comparison.
   - `max_paths` cross-product exhaustion → SMT fallback.
   - Boundary-resume condition-pairing test: a boundary reachable under two different path
     conditions must produce two resumed frontiers with disjoint conditions.
   - **Unsat `initial_seed` negative (rev-4):** seed the region driver with a contradictory
     boundary condition and assert the witness **rejects** (pairs_checked==0 guard fires) —
     never certifies.
3. **Regression guards:** the four existing call tests
   (`test_nonvolatile_permutation_across_call_accepted`,
   `test_volatile_live_across_call_must_be_fixed`, `test_opaque_eabi_callee_rejected`,
   `test_swap_other_register_while_r11_live_across_call`) must hold unchanged under the region
   path (`rho_mode="region-sliced"`), asserted explicitly.
4. **Regression suites:** `python -m unittest discover -s tools/coop/tests -p "test_*.py"`; the
   `ppc_equivalence` corpus + differential suites (mandatory after WS2); `run.py targets
   recertify --bottom-up --dry-run` then real — existing witness certificates must keep
   byte-identical `proof_request_hash` (no hash-input change).
5. **Rollout order:** WS1 → WS2 (executor, standalone-reviewed; the `pairs_checked` guard can
   land first as standalone hardening) → WS3 (witness B) → WS4. Each merged separately.

---

## 5. Success criteria

- **Measurable population:** the committed audit subcommand (WS3.7) reproduces the current
  counts at run time — expected ≈74 rho-fail / ≈55 callee-clean / ≈19 callee-blocked / **≈19
  first-cut-reachable** / 17 not_equivalent (re-evaluated per §2.7, not excluded) — **with the
  drift caveat stated** (registry is live; numbers are snapshots).
- Every first-cut-reachable target is either **certified** via the expanded witness or
  **classified** with a machine-checkable reason (live-conflict witness, loop case, callee-
  blocked, replay-validated not_equivalent).
- The `not_equivalent` targets are re-run through the region-rho witness (§2.7); the rescuable
  subset certifies, the rest reject — no "permanently out" classification.
- CX-1..CX-4 and the four call tests still pass unchanged; all WS4 negatives reject, including
  the unsat-`initial_seed` negative (the `pairs_checked == 0` guard fires).
- No previously-certified witness target regresses; existing certificates' `proof_request_hash`
  unchanged (no hash-input change).
- At least 5 real targets land EQUIVALENT_MATCH via the expanded witness with `cycle` evidence,
  chosen from the verified first-cut-reachable subset (e.g. `us-8003aa6c`, `us-801ccf18-2`,
  `us-8006c548`), re-verified via the audit subcommand at rollout time.

---

## 6. Out of scope / remaining blockers

- **Indirect-call targets** (incl. `us-802e7f00`, `us-802e7a48`): `execute_cfg` raises
  `ExecutionInconclusive("call target None has no matched-callee lemma")` (semantics.py:5610) —
  WS2 of the parked `indirect_call_certification_plan.md`. No rho expansion helps.
- **Size / reloc / fields / mnemonic / reject-list / abi-boundary failures:** real semantic or
  structural diffs — never renaming.
- **Spill-offset differences:** Gate-3 non-register-immediate rejects; documented, not in scope.
- **Callee-blocked targets (≈19 of the ≈74):** blocked at `equivalence_check.py:1448` before the
  witness; WS1 of the parked plan (declared-callee plumbing) is the fix.
- **Loop-containing targets (first cut):** fall to SMT; re-admitted by the fixpoint-enabled
  second cut (future workstream after B ships).
- **Opaque `reads=['*']` contracts with non-identity rho:** stay rejected (call token covers
  every register); out of scope unless a narrowed-contract path is built.

---

## 7. Effort estimate (rev 4)

| Workstream | Estimate | Notes |
|---|---|---|
| WS1 CFG-liveness fixpoint | 2–3 days | rewrite; includes PS1 defs, loop-free policy, unknown-opcode, tests |
| WS2 executor region extension | 3–5 days + review | `stop_at_pcs` + `initial_seed` + `pairs_checked` guard; standalone-merged; corpus-gated |
| WS3 witness B (regions, gates, cert, audit tooling) | 3–5 days + review | includes committed audit subcommand + assumptions text |
| WS4 tests / rollout | 2–3 days | incl. recertify wave + ≥5 landings from ≈19 pool |
| Soundness review | 2–3 days (parallel) | sign-off on §2.1/§2.2/§2.3/§2.7 and the executor change |

Total ≈ **3–4 weeks** for one witness/engine owner with review. Risk is concentrated in the
executor extension (WS2) and the liveness fixpoint (WS1) — both bounded, both merged with their
own regression corpora.

---

## 8. Risks

- **Liveness fixpoint correctness (highest):** a wrong deadness decision at a boundary or
  conflict is a silent divergence, not a crash. The loop back-edge and inverse-lane negatives are
  CI canaries; the per-target loop-free first cut limits exposure.
- **Executor regression:** `stop_at_pcs`/`initial_seed` must be strictly additive; the full corpus
  + differential suite gates the merge before any witness code depends on it. The pause is at
  pop-time (before executing the boundary instruction) — the resume must start at the boundary PC,
  and call-site pauses must land on the call side of summary application. The `pairs_checked == 0`
  guard is the backstop against empty-exits false certificates.
- **Path-condition/`max_paths`/`visit_counts` carryover:** region counts multiply; budget
  exhaustion must fall to SMT, never force a weakened comparison. Dropping `visit_counts`
  carryover silently disables the loop bound; the per-region `max_paths` reset must be
  driver-side summed.
- **PS1 liveness:** scalar-s FP ops define `ps1` (semantics.py:4487–4488) — WS1 must model those
  defs or ps1 deadness is wrong at any boundary near a scalar-s op (first-cut candidate
  `us-8006c548` uses `frsp`).
- **Audit drift:** the registry is live; the population numbers in this plan are a snapshot
  (74↔75 rho-fail observed between runs minutes apart). The committed audit subcommand
  re-computes at run time; success criteria reference the *mechanism* (real gate calls + loop
  predicate), not a fixed count. The rollout ids are re-verified via the subcommand at rollout
  time.
- **Cert semantic change:** `result_format` may stay 24 only with reviewer sign-off on the
  `rho_mode` semantic change; the `assumptions` text must be updated in the same change as the
  payload.
- **Scope creep toward WS2 of the parked plan:** any `bctrl` handling inside this work is out of
  scope; the executor extension here is for region boundaries only.
