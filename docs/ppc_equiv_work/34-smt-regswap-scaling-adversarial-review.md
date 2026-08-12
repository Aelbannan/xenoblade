# 34 — SMT / reg-swap scaling: adversarial review of the merging and product-exploration proposals

Status: **record of the 2026-08-12 adversarial review** (GLM-5.2 + Kimi K3 via
pi/OpenRouter, high thinking, both independent — no coordination). Both
reviewers AGREE on the core verdict: the diagnosis was partially wrong, the
proposals as specified are not shippable, and a strictly smaller witness-scoped
merge is the sound minimum change. Companion design plan:
`35-witness-boundary-merge-plan.md`.

## Question reviewed

Why do the `ppc_equivalence` paths (SMT probe and register-renaming witness)
struggle with mid-size and larger (branchy) functions — SMT takes too long, the
witness hits too many paths — and is the proposed way out sound? The way out
under review had three parts:

- **P1.** Join-point state merging (ITE consolidation) in `execute_cfg`:
  merge frontier entries at the same PC into one entry with condition `c1 ∨ c2`
  and per-lane `ite(c1, v1, v2)`. Claimed exact because single-program paths
  reaching a join have pairwise-disjoint path conditions. Claimed cost: linear
  in instructions × live merge points. Wrinkles claimed: (a) memory arrays
  merged per-address via nested `Store` over the union of `memory_touches`;
  (b) `force=True` premise terminals must stay separate.
- **P2.** Product automaton / co-inductive relational exploration: joint
  frontier `(pc_L, pc_R, joint_condition)`, memoized visited pairs, per-edge
  UNSAT lemmas (precedent claimed: `discharge_jump_table_obligations`,
  `try_discharge_relational`).
- **P3.** Cheaper tier (no executor change): (i) collapse witness exits by
  exit-kind before the pairwise compare; (ii) finer region slicing; (iii)
  concrete sampling as a pre-SMT filter.

## Claimed diagnosis (C1–C5) — verification results

| # | Claim | Verdict (both reviewers) | Evidence |
|---|---|---|---|
| C1 | Fork at every `BC` (`taken`/`fall_condition`, both enqueued); symbolic conditions rarely prune; ~2^k paths capped by `max_paths` (4096 SMT / 2048 witness) | **Correct, with two omissions**: `_path_condition_feasible` is `z3.simplify` + `is_false`, not a solver check (so prune is purely syntactic); `force=True` premise terminals bypass feasibility and count against `max_paths` unconditionally | `semantics.py:5756-5757, 5825, 5856, 5240-5241, 5259-5260, 4913-4957, 5377-5379, 5413-5415, 5529-5537`; `equivalence_check.py:222, 230, 244` |
| C2 | Divergence query is `z3.Or` over all original×candidate terminal pairs, each with per-observable differences over 96 lanes + memory | **Correct, understated**: the SMT path has NO disjointness pre-filter (unlike the witness), every n×m pair reaches the solver; memory iteration is over un-deduped per-byte touch tuples | `engine.py:1729-1733, 1868, 522, 512-541` |
| C3 | Terminal memory comparison is array-extensionality over N-deep ITE chains, plus `build_memory_constraints` axioms | **Wrong mechanism**: comparison is per-touched-byte `Select` with per-side private-stack `If` masking, plus a `memory_effects` length+`zip` compare; `build_memory_constraints` returns `[]` under the default `assumed-ordinary-ram` profile. The N-deep Store-chain cost per `Select` stands | `engine.py:512-541, 536-540`; `memory_profile.py:140-167`; `memory_semantics.py:60-83` |
| C4 | BV hardness (multipliers, symbolic shifts, carry) → default solver unknown → bit-blast portfolio explodes | **Correct** | `engine.py:1142-1191` |
| C5 | Witness inherits the fork explosion; identical control flow ⇒ identical conditions ⇒ both the feasibility pruner and the disjoint-pair filter do nothing; O(n×m) structural compare unavoidable | **Half wrong — refuted by doc 32 A4's own measurements**: the disjoint-pair filter prunes 4^k down to the 2^k diagonal (not nothing); the measured witness failures are `pairs=1` structural divergence (WPADiExcludeButton, 10.8 s — genuine code/global-rho gap) and `pairs=0` cfg-exploration deadline (`__wudStackCheckDeviceInfo`, 30 s — dies inside `execute_cfg`, never reaches the compare). The O(n×m) compare is NOT the observed witness bottleneck | `renaming_witness.py:2817-2825, 2498`; `32-witness-improvements-plan.md:317, 359, 561-572` |

## Findings on the proposals (BLOCKER / MAJOR / MINOR / NIT)

### P1 — join-point ITE merging

- **BLOCKER (both): the merge key must include per-path `visit_counts`.**
  Summary gates fire on `prior_visits == 0` and the iteration cap on
  `prior_visits >= max_loop_iterations`; merging a count-0 entry with a count-1
  entry at a header either re-applies a closed-form summary to a path that
  already consumed it (**unsound** — the entry premise is evaluated against the
  pre-first-entry state) or suppresses the summary. Sound key:
  `(pc, visit_counts projected onto counted_visits)`.
  `semantics.py:5284-5287, 5398, 5441, 5551, 5194-5218, 5297-5334`.
- **BLOCKER (both): the DFS worklist defeats arrival-triggered merging.**
  `work.pop()` is LIFO — the first arm of a diamond is expanded to completion
  before the sibling is enqueued, so "merge when a frontier entry arrives at a
  PC that already has a pending entry" fires only by scheduling accident. Real
  join-point merging needs computed merge points / a hold-until-complete
  policy (or BFS layering), which the proposal did not specify.
  `semantics.py:5265`.
- **BLOCKER (both): Python-level state components cannot be ITE-merged.**
  `memory_effects` is an ordered tuple compared by length + `zip`
  (`engine.py:536-540`; appended per memory-writing summarized call,
  `semantics.py:4882`); `symbolic_bus` is a Python object (FIFO traces), not a
  Z3 AST; `memory_touches/reads/writes` are per-byte address-tuples consumed by
  `_memory_difference`, `build_memory_constraints`, and the auto-contract
  bookkeeping. Merging must be **refused** (fail-closed to unmerged paths)
  when these diverge. The proposal handled only the array, not the tuples.
- **BLOCKER (Kimi) / MAJOR (GLM): side-asymmetric merge triggers destroy the
  witness's structural certificate.** The witness backstops on `z3.eq` after
  simplify (`_terminals_agree`). A state-dependent trigger ("when the incoming
  write sets are small") fires at different moments on the two sides (touch /
  effect tuples diverge across any summarized call), so retail's merged
  `ite(c1, v1, v2)` never simplifies to decomp's unmerged `v1` → spurious
  `structural` failure → total loss of witness coverage (fail-closed, but the
  feature dies). The policy must be a pure function of
  `(pc, visit_counts, condition-AST identity)` so two rho-shared executions of
  the same CFG merge identically.
- **MAJOR (both): the "linear" cost claim is false.** Merged conditions grow by
  one `Or` per merge; every later fork conjoins onto the growing formula, and
  `_path_condition_feasible` runs `z3.simplify` on the whole condition at every
  enqueue; the ast_id memo never hits because each merged condition is a fresh
  AST. This is the same MIX/CfPadTask superlinear wall the executor's own
  comment documents. Cost is polynomial (~quadratic in merges), not linear;
  ITE-DAG growth inflates every downstream instruction's AST building.
  `semantics.py:4943-4948, 4940-4956, 4915-4921`.
- **MINOR (both): the disjointness lemma itself holds.** Every fork site
  verified: BC (`5756-5757`), TWI (`5558-5566`), FP-trap (~5700s), jump-table
  closure incl. remainder (`5783-5830`), RFI (`5576-5590`); all split into
  `condition ∧ p` / `condition ∧ ¬p` or pairwise-exclusive `ctr == addr`
  disjuncts; call summaries do not fork; fail-open feasibility keeps only
  *unsat* extras, which are disjoint from everything. Wrinkle (b) is
  confused-but-harmless: `force=True` premise records are terminals, never
  frontier entries, so they cannot be merged anyway; a merged entry hitting a
  summary header records one merged premise terminal with condition
  `(c1 ∨ c2) ∧ ¬guard` — sound, contradicting "must stay separate".
- **MINOR:** `steps` must merge as `max` (vs. the instruction cap,
  `semantics.py:5273`); merging defeats the A3 "r1 unchanged → reuse identical
  AST" skip (`5583-5604`) so r1-touching instructions pay full cost after the
  first merge; z3-solver 4.15.3.0 is a non-issue (per-address Store / `If`
  merging needs nothing version-specific; array-level `If(cond, arr1, arr2)` is
  supported, so the stated reason for the nested-Store workaround is wrong —
  the workaround is still fine, and the bound "union size ≤ N × per-path
  touches" should be stated).
- **MINOR (a missed P1 benefit):** `_symbolic_persistent_writes`
  (`engine.py:158-198`) runs one 2 s-timeout solver query per AUTO_PERSISTENT
  observable per terminal; merging shrinks terminal count and directly cuts
  this SMT-path cost.
- **MINOR (r1/stack):** `stack_low` (per-r1-def ite-chain), `stack_layout_valid`
  (conjunct accumulation), `stack_private` (set False after any summarized
  call, `semantics.py:4773/4883`) and the register-publish escape
  (`stack_escape.py:74-122`, which descends into `If` branches) are all
  ITE-mergeable and sound — over-conservative (more addresses compared), never
  fewer. Precision loss only.

### P2 — product automaton / co-inductive relational exploration

- **BLOCKER (scope, both): product exploration is only defined where a
  coupling exists — i.e. the witness class.** "Explore `(pc_L, pc_R)` jointly"
  presupposes knowing which PCs correspond; for the SMT path — whose entire
  purpose is pairs with *different* instruction streams — no coupling is
  supplied. The cited precedents do not generalize: the relational sketch
  machinery requires exactly one CTR/compare-affine loop per side with
  structurally matching bodies (`_affine_bodies_match_for_discharge`), and
  `discharge_jump_table_obligations` discharges one specific remainder-terminal
  shape. Whole-function per-edge lemmas over divergent control flow *is*
  relational invariant inference — a research project, not a patch.
  `relational_induction.py:493-563, 570-680`; `jump_table_obligations.py:850-902`.
- **MAJOR (both): "carry the joint condition" is sound only in the degenerate
  form (carry both full symbolic states, compare at jointly-reached terminals
  under `c_L ∧ c_R`)** — that is the cross product with early feasibility
  pruning: sound, sometimes much cheaper, worst-case identical, no relational
  invariant needed. Anything beyond it (pruning on rho-relatedness, merging
  joint states) needs the relational state invariant the proposal waves at:
  the two states are rho-related, not equal. For identical CFGs the joint
  frontier without merging still forks 2^k (`c_L ≡ c_R`, both edges feasible).
  Early-exit asymmetry re-creates a cross product along the suspended-terminal
  dimension unless suspended terminals are merged per exit-kind — which is P1
  again. **P2 neither subsumes nor replaces P1.**
- **MAJOR:** the SOUNDNESS.md enumeration theorem (`:77-80`) and the witness
  renaming lemma (`:266-287`) are stated over pathwise terminals; merged
  execution consumes the pairwise-disjointness lemma, so the theorem needs a
  merge-simulation lemma and a restated witness lemma for disjunctive
  conditions. Not budgeted in the proposal.

### P3 — cheaper tier

- **BLOCKER (both, on the claim): P3(i) cannot collapse exits by exit-kind
  without merging states.** Each terminal carries its own condition and state;
  the witness must compare every co-feasible pair (the `pairs_checked == 0`
  anti-vacuity guard exists precisely because skipping pairs is dangerous).
  "Collapse by exit-kind" is either a no-op (per-kind conditions still force
  per-terminal structural compares — `Σ n_k × m_k == n × m` when all exits
  share one kind, the common case) or it is P1 in disguise, which is not "no
  executor change". `renaming_witness.py:2842-2855, 2520-2522`.
- **MAJOR (both): P3(ii) finer region slicing is a pessimization without
  merging.** The region driver pauses frontier entries at boundaries and
  resumes each one separately via `initial_seed` (`renaming_witness.py:3328-3429`);
  the cumulative budget counts frontier + terminals across ALL regions
  (`:3432-3438`), so more regions reach `max_paths` sooner, and each region
  re-executes per frontier entry. With merging at the pause points it becomes
  the natural vehicle for witness-side merging; without it, it makes the budget
  problem worse. The four-lane deadness gates (`:3108-3192`) are unaffected per
  boundary (they constrain only changed lanes) but multiply with boundary count.
- **MINOR (both): P3(iii) concrete sampling as a pre-filter is sound but
  orthogonal.** `_apply_concrete_sampling_defense` (`engine.py:1014-1094`)
  already treats a concrete mismatch as ground truth sufficient to demote an
  SMT `EQUIVALENT`; moving it earlier changes no soundness property (never
  promotes). It only accelerates *negative* answers; for branchy-but-equivalent
  targets it is pure overhead. Ship it as a cheap filter; do not count it
  toward the path-explosion problem.

## Things the proposal missed entirely

- **MAJOR (both): the measured witness failures are not path explosion.**
  WPADiExcludeButton = single-pair structural divergence (`pairs=1`) — no path
  machinery certifies a genuinely diverging pair; investigate the divergence
  (declared-return / global-rho / S1 base-offset class) separately.
  `__wudStackCheckDeviceInfo` = `cfg-exploration` deadline at `pairs=0` — dies
  in exploration, where merging helps, not in the n×m compare.
- **MAJOR (GLM): the witness's F3 callee-token canonicalization amplifies the
  correlated-merge requirement.** `_apply_call_summary` with
  `witness_register_perm` rewrites decomp lanes to retail order around calls;
  if the two sides merge at different pre-call PCs, the canonicalized trees are
  not AST-identical and the witness rejects. Merge-before-call is sound per
  side; merge-policy symmetry is the requirement.
- **MINOR (both):** loop-summary re-entry into a merged header is safe only if
  the merge key is `(pc, visit_counts)` (first-entry paths are diverted to
  `exit_pc` and never re-enter); taking `max(visit_counts)` caps the
  lower-visit path early (fail-closed). The `pairs_checked == 0` guard's intent
  must survive merging (a merge collapsing all terminals into one with
  condition `True` must not mask a vacuous case).

## Verdict (both reviewers, independently)

- **Diagnosis:** C1, C2, C4 correct (C2 understated); C3's mechanism wrong in
  detail; C5's "unavoidable O(n×m) witness cross-product / it's all path
  count" refuted by the doc-32 measurements.
- **P1:** sound in principle (disjointness lemma verified at every fork site),
  **infeasible as specified** — needs (pc, visit_counts) keying, a real
  merge-point scheduler, merge refusal on divergent Python-level components, a
  side-symmetric state-independent policy, an honest polynomial cost model, and
  a SOUNDNESS.md theorem update.
- **P2:** sound only in its degenerate early-pruned-cross-product form and only
  well-defined for the aligned/witness class; infeasible as a general SMT-path
  mechanism; does not subsume P1.
- **P3:** (i) P1-in-disguise / misdiagnoses the witness bottleneck; (ii) a
  pessimization without P1; (iii) sound and orthogonal.

## Minimum sound change (both reviewers)

1. **P3(iii)** as a demote-only pre-filter (zero soundness risk).
2. **Witness-scoped merging only:** merge paused frontier entries at region
   boundaries (the `stop_at_pcs` / `paused_out` / `initial_seed` machinery
   already exists) keyed by `(pc, visit_counts)`, with a policy pure in
   `(pc, counts, condition-AST identity)`. The witness passes no
   `symbolic_bus`, no jump-table closures, and no loop summaries, so the
   Python-component blocker reduces to requiring equal `memory_effects` token
   sequences, and the DFS / visit-count / symmetry blockers have local,
   testable answers. Directly attacks the `cfg-exploration` deadline failure
   mode. Design: `35-witness-boundary-merge-plan.md`.
3. **Defer P1-for-SMT and P2** until the witness-scoped variant has
   measurements (doc 32 A4 methodology); fix the WPADiExcludeButton `pairs=1`
   structural divergence as a separate investigation — no amount of path
   machinery certifies a genuinely diverging pair.

## Files to read

- `tools/ppc_equivalence/semantics.py` — `execute_cfg`, `_execute_cfg_body`
  (worklist, fork sites, `visit_counts`, `force=True` premise terminals),
  `_path_condition_feasible`, `_apply_call_summary`
- `tools/ppc_equivalence/engine.py` — `_check_equivalence_impl` (pairwise
  cross product), `_terminal_difference`, `_memory_difference`,
  `check_with_portfolio`, `_symbolic_persistent_writes`,
  `_apply_register_publish_escape_to_terminals`
- `tools/coop/lib/renaming_witness.py` — `run_structural_witness`,
  `certify_renaming_witness`, `run_region_sliced_witness` (paused-frontier /
  cumulative budget), `_terminals_agree`, `_nonvolatile_preservation_failure`
- `tools/coop/lib/equivalence_check.py` — budget constants and witness wiring
- `docs/ppc_equiv_work/31-reg-swap-witness.md`,
  `docs/ppc_equiv_work/32-witness-improvements-plan.md` (A4 measurements),
  `docs/ppc_equivalence/SOUNDNESS.md`
- `tools/ppc_equivalence/relational_induction.py`,
  `tools/ppc_equivalence/jump_table_obligations.py`,
  `tools/ppc_equivalence/memory_loop.py`
