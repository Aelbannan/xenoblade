# 35 — Witness-scoped state merging: boundary and join-point design (rev 0)

Status: **design draft** — the minimum sound change agreed by the 2026-08-12
adversarial review (GLM-5.2 + Kimi K3, both independent; findings and verdicts
in `34-smt-regswap-scaling-adversarial-review.md`). Not implemented. Plan
conventions follow `31-reg-swap-witness.md` / `32-witness-improvements-plan.md`.

## 0. Problem statement

The register-renaming witness (`tools/coop/lib/renaming_witness.py`) certifies
position-aligned, same-mnemonic pairs whose instruction streams differ only in
register colors, by executing BOTH sides through the audited `execute_cfg`
(semantics.py) with rho-shared symbolic variables and comparing terminals with
structural `z3.eq`. Branchy mid-size functions fail it not at the compare but
in exploration (doc 32 A4 measurements, re-confirmed in doc 34 F5):

- `__wudStackCheckDeviceInfo` (us-8037a710): **`cfg-exploration` deadline,
  `pairs=0`** — dies inside `execute_cfg` before any terminal pair exists
  (`32-witness-improvements-plan.md:563-565`). 30 s deadline
  (`_WITNESS_DEADLINE_MS`, equivalence_check.py:244), `max_paths=2048`
  (`_WITNESS_MAX_PATHS`, :230).
- `WPADiExcludeButton` (us-8036fb80): **`structural`, `pairs=1`** — a genuine
  code / global-rho divergence, *not* a resource case; out of scope for this
  plan (separate investigation, doc 34 verdict item 3).

Root cause of the deadline case: `execute_cfg` forks at every conditional
branch (`semantics.py:5756-5757`, both edges enqueued) and never merges at
reconvergence points, so the frontier is ~2^k for k branches; with symbolic
inputs the feasibility filter (`_path_condition_feasible`, `semantics.py:4913`)
prunes almost nothing; each enqueue pays a whole-formula `z3.simplify`
(`:4943-4948`) on a condition that grows by one `And` per fork.

Goal: reduce the witness's exploration cost **soundly and with minimal surface
change** so branchy-but-equivalent pairs reach the structural compare (and
certify) instead of burning the deadline — without changing the SMT path and
without relaxing any gate.

## 1. Soundness contract

### 1.1 Merge-simulation lemma

If two frontier entries at the same PC `p` have conditions `c1`, `c2` with
`c1 ∧ c2` UNSAT and states `s1`, `s2`, then the merged entry
`(p, ite(c1, s1, s2), c1 ∨ c2)` simulates both original paths: every concrete
input satisfying `c1` follows path 1 and evaluates the merged state to `s1`;
every input satisfying `c2` evaluates it to `s2`; no input satisfies both
(disjointness). Hence the merged terminal set is semantically identical to the
unmerged set, and any equivalence verdict obtained from merged terminals is
exactly the verdict obtainable from unmerged terminals.

**Disjointness is a fact of this executor, not an assumption.** Verified at
every fork site (doc 34, finding 11): BC (`semantics.py:5756-5757`), TWI
(`:5558-5566`), FP-trap (~:5700s), jump-table closure incl. the remainder
terminal (`:5783-5830`), RFI (`:5576-5590`) — all split `condition` into
`condition ∧ p` / `condition ∧ ¬p` or pairwise-exclusive `ctr == addr`
disjuncts. Call summaries do not fork. Fail-open feasibility keeps only *unsat*
extras, which are disjoint from everything. The merge function re-checks
disjointness (`z3.is_false(simplify(c1 ∧ c2))`) and **refuses to merge on any
non-false result** (defensive; the lemma says it cannot happen).

### 1.2 Merge key

Merge is permitted only for entries that agree on:

1. **PC** — the frontier program counter.
2. **`visit_counts` projected onto `counted_visits`** (the set computed at
   `semantics.py:5194-5218`: function start + static B/BC targets + summary
   headers). This preserves the semantics of `prior_visits` for the
   `max_loop_iterations` cap (`:5551`) and for any summary `prior_visits == 0`
   gate (`:5285, 5398, 5441`) — without it, merging a count-0 entry with a
   count-1 entry could re-apply a closed-form summary to a path that already
   consumed it (unsound) or suppress it (loss of coverage). Doc 34 finding 6.
3. **`memory_effects` identity** — the ordered tuple of summarized-call tokens
   (`semantics.py:4882`). Two paths with different token sequences (one took a
   memory-writing call, the other did not) have no mergeable effect history.
   Doc 34 finding 8.

Entries disagreeing on any key component are **not merged**; each proceeds as
today. The witness passes no `symbolic_bus`, no `jump_table_targets`, and no
loop summaries (execute_cfg calls at `renaming_witness.py:2752, 2763`), so the
remaining Python-level components (`symbolic_bus` FIFO traces; touch tuples)
are inert in this scope — merge refusal on `symbolic_bus` presence is kept as
a defensive guard for future callers.

### 1.3 Side symmetry (mandatory)

The merge decision must be a **pure function of the key** — never of frontier
size, state size, or write-set size. Both sides execute position-aligned,
rho-shared streams, so a key-pure policy fires identically on both sides and
the merged ITE trees are built from identical ASTs — preserving the witness's
structural `z3.eq` certificate (`_terminals_agree`, `renaming_witness.py:2498`).
A state-dependent trigger would fire asymmetrically (touch/effect tuples
diverge across any summarized call) and produce spurious `structural` failures
(doc 34 finding 10). This is the single most important design constraint.

## 2. Design

### 2.1 Site A — region-boundary merge (rev 0; no executor change)

`run_region_sliced_witness` (`renaming_witness.py:3193`) pauses frontier
entries at region boundaries via `stop_at_pcs` / `paused_out` and resumes each
one separately through `initial_seed` (`:3328-3429`), accumulating a driver
cross-region frontier+terminal budget (`cumulative > max_paths`, `:3432-3438`).
Entries pausing at the **same boundary PC** are exactly the reconverging paths
the lemma applies to.

Change: after each region's `execute_cfg` returns its `paused` list, group the
paused entries by merge key `(pc, visits, effects)`; merge groups of size ≥ 2
(§1.1); resume one `initial_seed` per merged group (and one per unmerged
entry). Effects:

- the per-region re-execution frontier shrinks — the next region's
  `execute_cfg` seeds fewer entries;
- the cumulative budget (`:3432-3438`) counts the smaller merged frontier, so
  branchy rho-conflicted functions trip it later;
- no change to `semantics.py` at all — the executor is untouched, the TCB
  audit surface is the region driver only.

Scope limit (honest): Site A reaches only functions routed to the region path
(the `gate == "rho"` fallback class). The global-path deadline case
(`__wudStackCheckDeviceInfo`, `mode=None`) needs Site B.

### 2.2 Site B — executor join-point merge on the global witness path (rev 1; after measurement)

The global path (`run_structural_witness`) executes both sides through
`execute_cfg` with the plain LIFO worklist (`work.pop()`, `semantics.py:5265`).
Arrival-triggered merging does not work there: the first arm of a diamond is
expanded to completion before the sibling is enqueued (doc 34 finding 7).
Site B therefore has two parts:

1. **A static join-point set** computed from the CFG (PCs with in-degree > 1,
   i.e. reconvergence after any taken/fall-through split; cheap — the by-PC
   successor map already exists in the executor). Entries at non-join PCs
   behave exactly as today; entries at join PCs are **held** in a
   `pending[key]` table until the current exploration round drains (one
   "round" = the set of entries reachable without passing through a join PC),
   then merged per key and expanded. This is a bounded, local change to
   `_execute_cfg_body`'s scheduling, not a new interpreter.
2. **A witness-mode flag** on the execute_cfg entry so the SMT path keeps
   current behavior until separately re-audited. Under witness mode, no
   summaries / jump tables / bus are present, so the refusal conditions of
   §1.2 are almost never exercised and the merge is effectively always keyed
   on `(pc, visits)`.

Complementary targeted fix (GLM-5.2, doc 34 verdict): independent of merging,
the per-enqueue `z3.simplify` on a condition growing by one `And` per fork is
itself superlinear. A persistent path-condition trie / suffix-sharing AST
representation makes the per-fork simplify sublinear and attacks the same
deadline from the cost side. Evaluate as an alternative or companion to 2.2-1;
it is smaller than the scheduler change and helps the SMT path's exploration
cost too (but only under its own re-audit).

### 2.3 Merge function

```
def try_merge(left, right, ops, budget):
    if (left.pc, left.visits, left.effects) != (right.pc, right.visits, right.effects):
        return None                      # §1.2 refusal
    if not is_false(simplify(And(left.cond, right.cond), budget)):
        return None                      # §1.1 defensive re-check
    state = ite_per_lane(left.state, right.state, left.cond)   # GPR/FPR/PS1/CR/XER/FPSCR/SPRs
    memory = merge_arrays(left.state.memory, right.state.memory, left.cond,
                          union(left.state.memory_touches, right.state.memory_touches))
    return entry(pc, ite(state), Or(left.cond, right.cond),
                 left.visits, max(left.steps, right.steps))
```

Notes:

- **Memory**: per-address nested `Store` over the union of the two states'
  touched addresses, with `ite(c1, v1, v2)` values; both states derive from
  the same `initial.memory` and differ only on touched addresses, so the
  union is complete. Bound: union size ≤ per-path touches × 2 per merge.
  Array-level `If(cond, arr1, arr2)` is also supported by z3 4.15.3.0 — either
  form is acceptable; per-address keeps the existing per-byte `Select`
  terminal compare (`engine.py:512-541`) unchanged. The merged state's own
  touch tuple must be the union, and read/write separation
  (`memory_reads`/`memory_writes`) must union independently (doc 34 finding 8).
- **`steps`** merges as `max` (fail-closed vs. the instruction cap,
  `semantics.py:5273`).
- **`stack_low` / `stack_layout_valid` / `stack_private`** are ASTs and merge
  via `ite`; `stack_private = False` after any summarized call
  (`semantics.py:4773/4883`) is preserved as an AST value, and the terminal
  private-stack masking (`engine.py:512-541`) reads it per side. Cost note:
  merging defeats the A3 "r1 unchanged → reuse identical AST" skip
  (`semantics.py:5583-5604`) — accepted; the r1-liveness skip still applies on
  straight-line code.
- **Budget**: the merge runs under the existing `_SimplifyBudget` /
  `_simplify_timeout` (`renaming_witness.py`); on budget exhaustion → no merge.

## 3. Interaction with existing machinery

| Machinery | Interaction |
|---|---|
| `_terminals_agree` (`renaming_witness.py:2498`) | Merged states compare structurally; key-pure policy (§1.3) ⇒ identical ITE construction on both sides ⇒ AST identity preserved. Exit-kind check (`:2520-2522`) unchanged. |
| F1 nonvolatile preservation (`:2422`) | Per-exit `z3.eq(simplify(state.lane), initial.lane)`: merged `ite` folds to `initial` when both paths preserve; rejects when either clobbers (conditional-restore shapes already rejected unmerged) — sound. |
| `pairs_checked == 0` anti-vacuity guard (`:2842-2855`) | Preserve its intent: a merge collapsing many terminals into one must not mask the vacuous case. The guard examines the merged condition set (a `c1 ∨ c2` condition is comparable, not skipped). |
| Driver cumulative budget (`:3432-3438`) | Counts the merged (smaller) frontier — the intended win. |
| Four-lane deadness gates (`:3108-3192`) | Path-insensitive may-liveness fixpoint; merging preserves the union of live sets — unchanged per boundary. |
| `max_loop_iterations` cap | Preserved by `visit_counts` keying (§1.2). |
| F3 callee-token canonicalization (`semantics.py:4664+`) | Merge-before-call is sound per side; policy symmetry ensures both sides merge at the same pre-call PCs, so canonicalized trees stay AST-identical. |
| `_memory_arrays_agree` word walk | Merged per-address Store chains with `ite` values still match across sides; walk cost grows with union-touch size (bounded per §2.3). |
| Provenance / certifier hash | `renaming_witness.py` is in `CERTIFIER_SOURCE_PATHS` (doc 32 I5); any merge change bumps the certifier hash and invalidates stored certificates — plan a recertification pass. |

## 4. Fail-closed guards

- Merge refusal on: key mismatch (§1.2), non-disjoint conditions (§1.1
  re-check), `symbolic_bus` presence (defensive), budget exhaustion (§2.3).
- Refusal degrades to today's per-entry behavior — never a false certificate:
  merging is exact when it applies (§1.1) and refusal is the current code path.
- The SMT path is untouched by rev 0 (Site A is driver-only) and by rev 1 only
  behind the witness-mode flag (Site B 2.2-2), so no SMT soundness surface
  changes without a separate audit.
- SOUNDNESS.md: add the merge-simulation lemma (§1.1) and restate the witness
  renaming lemma (`SOUNDNESS.md:266-287`) for disjunctive terminal conditions
  when Site B lands (doc 34 finding 16).

## 5. Tests

Unit (new `tools/coop/tests/test_witness_merge.py`, plus the existing
`test_renaming_witness.py` suite must stay green):

1. **Diamond merge**: a branchy pair that currently bails on `max_paths`
   certifies with merging; the merged-terminal verdict equals the unmerged
   verdict on the same pair (regression harness runs both paths).
2. **Per-iteration latch merge**: loop body with an internal diamond, several
   concrete trips — witness certifies with a merged frontier.
3. **Refusal — divergent `memory_effects`**: one path takes a memory-writing
   summarized call, the other does not → no merge, falls through unchanged.
4. **Side symmetry**: property test over the fixture corpus — the merge policy
   fires identically on rho-shared streams (merge-count and merged-AST hash
   equal across sides).
5. **Anti-vacuity**: a merged single terminal must not bypass the
   `pairs_checked == 0` guard.
6. **Defensive disjointness**: artificially overlapping conditions → merge
   refused.

Corpus measurement (doc 32 A4 methodology, `:589-608`): the 93-target witness
set plus the two cited failures. Success criteria: `__wudStackCheckDeviceInfo`-
class targets certify (`cycle` verdict `EQUIVALENT_MATCH` without `--smt`,
split-size fit) or at least reach the structural compare within the deadline;
`WPADiExcludeButton` stays `structural` (expected — separate track); zero
regression on the existing witness test suite; certifier-hash recertification
passes bottom-up.

## 6. Sequencing and acceptance (rev 0)

1. **P3(iii)** — demote-only concrete-sampling pre-filter (doc 34 verdict item
   1): zero soundness risk, ship first.
2. **Site A** — region-boundary merge (§2.1): no executor change; measure on
   the region-routed corpus.
3. **Measure.** If the global-path deadline class remains, land **Site B**
   (§2.2) behind the witness-mode flag, with the SOUNDNESS.md lemma update and
   the full fixture/corpus/Dolphin gates per the new-opcode checklist.
4. **WPADiExcludeButton** `pairs=1` structural divergence — separate
   investigation (declared-return / global-rho / S1 base-offset classes), not
   this plan.

Acceptance bar: witness cert rate on the doc-32 A4 target set at production
budgets (`max_paths=4096` → witness 2048, `deadline_ms=30000`), no
soundness-surface regression, provenance hashes re-bound.

## 7. Files to read

- `tools/coop/lib/renaming_witness.py` — `run_structural_witness`,
  `run_region_sliced_witness` (paused frontier / `initial_seed` / cumulative
  budget), `_terminals_agree`, `_nonvolatile_preservation_failure`,
  `_SimplifyBudget`, region-span guard
- `tools/ppc_equivalence/semantics.py` — `_execute_cfg_body` (worklist,
  `counted_visits`, `visit_counts`, fork sites, `force=True` premise
  terminals, `_apply_call_summary` effects)
- `tools/coop/lib/equivalence_check.py` — witness wiring and budget constants
- `docs/ppc_equiv_work/34-smt-regswap-scaling-adversarial-review.md` —
  findings and verdicts this plan implements
- `docs/ppc_equiv_work/31-reg-swap-witness.md` (renaming lemma, expansion B),
  `docs/ppc_equiv_work/32-witness-improvements-plan.md` (A4 measurements,
  I5 provenance), `docs/ppc_equivalence/SOUNDNESS.md`
