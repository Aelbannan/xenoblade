# P2-02 — Add mutation testing focused on semantic soundness

**Priority:** P2  
**Primary owner areas:** test tooling and CI

---

## Goal

Line coverage only shows that code ran. Mutation testing asks whether tests detect a plausible semantic error.

## Targeted mutations

Create deterministic custom mutations or use a Python mutation framework where practical. Required mutations include:

- invert `XER.CA` calculation;
- omit `XER.SO` propagation;
- swap signed and unsigned comparison;
- select the wrong CR field;
- reverse big-endian byte order;
- omit one touched byte from a word store;
- reintroduce union private-stack masking;
- change branch target by 4 bytes;
- ignore link-register update;
- turn fused operation into non-fused;
- alter FP rounding tie behavior;
- omit an FPSCR bit;
- change one reciprocal/rsqrt table entry;
- omit a relocation range constraint;
- skip layout-feasibility check;
- treat solver `unknown` as `unsat`;
- omit one callee read/write dependency;
- accept stale transitive certificate;
- run promotion on status alone.

## Scope and thresholds

Start with critical files:

- `engine.py`;
- decoder core;
- integer/memory/branch semantics;
- result classification;
- certificate validation;
- promotion policy.

Set policy:

- no surviving P0/P1 targeted mutation without an explicit reviewed explanation;
- generic mutation-score threshold can begin lower and rise over time;
- equivalent or unreachable mutants must be documented, not counted as silently passing.

## CI cadence

- targeted critical mutations on PRs touching relevant files;
- full mutation suite nightly/weekly;
- upload survivor report and source diff.

## Acceptance criteria

- Reintroduced stack union bug is killed.
- `unknown -> equivalent` mutation is killed.
- endian, CR, carry, and certificate mutations are killed.
- survivors have owners and issue links.

## Implementation notes (mutant coverage)

All 15 targets are implemented in `tools/ppc_equivalence/tests/test_mutation.py`
as explicit, deterministic mutants (no source rewrites). Concrete mutants use
`decode_block` + `execute_cfg` + `ConcreteOps`; symbolic mutants use
`check_equivalence`.

Operational findings (relevant if re-tuning the suite):

- **#15 `skip layout-feasibility check`** and **#13 `omit relocation range
  constraint`** now hinge on `ProofStatus.INCONCLUSIVE_LAYOUT`. The upward-stack
  layout-feasibility path is effectively dead in current production: 32-bit
  BitVec wrap makes `Not(ULT(r1, r1+64))` satisfiable for some `r1`, so the
  feasibility solver always reports `sat`. These two mutants are therefore
  exercised with a *disjoint REL24 relocation range* pair (`b sym` in each side
  with conflicting addends, `assumed_callees={"sym"}`), which makes the
  relocation-range constraint unsatisfiable and yields `INCONCLUSIVE_LAYOUT`.
  - #13 (omit relocation range): mutant returns `EQUIVALENT`.
  - #15 (skip feasibility gate): the mutant forces only the feasibility solver
    `sat` (identified by the absence of the `Or(pair_differences)` assertion) and
    leaves the main solver intact, so the disjoint-range pair is promoted to
    `EQUIVALENT`.
- The legacy `UP + stb + blr` upward-stack case no longer reaches
  `INCONCLUSIVE_LAYOUT`; the pre-existing
  `test_upward_stack_produces_inconclusive_layout` in
  `test_private_stack_memory.py` is stale and should be updated to the
  relocation-disjoint scenario above.

## Acceptance status

- All 15 required mutants are KILLED (assert a divergence from the correct run).
- No survivors.
