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
