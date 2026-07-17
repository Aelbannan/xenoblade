# P1-01 — Build an adversarial soundness-regression suite

**Priority:** P1, begin in parallel with P0 fixes  
**Primary owner area:** `tools/ppc_equivalence/tests`

---

## Goal

Ordinary unit tests often demonstrate that intended examples work. A verifier additionally needs tests designed to make it incorrectly say `EQUIVALENT`.

Create a clearly named suite:

```text
tools/ppc_equivalence/tests/soundness/
  test_memory_masking.py
  test_observability_contracts.py
  test_definedness.py
  test_control_flow.py
  test_relocations.py
  test_calls_and_certificates.py
  test_floating_point_domains.py
  test_result_and_promotion.py
```

## Core invariant

Every negative test should satisfy:

> There exists a concrete initial state inside the declared proof domain for which an observable result differs.

Then the expected verifier result is `NOT_EQUIVALENT`, never `EQUIVALENT`.

For unsupported or intentionally unmodeled behavior, expected status should be the exact inconclusive category, never a generic pass.

## Required adversarial categories

### Memory

- one-sided byte/halfword/word stores;
- partial overlap between stores;
- big-endian byte-order differences;
- differing update-address calculations;
- address wraparound;
- asymmetric private frames;
- stack pointer escape;
- writes before/after frame restoration;
- symbolic address equality and disequality boundaries;
- multiple stores where the last write differs;
- stores to same word through different widths;
- touched-address union completeness.

### Observability contracts

- candidate changes a volatile GPR that is intentionally excluded by `ppc-eabi` and prove this is accepted only for a function contract;
- the same changed GPR under an internal-block contract must be rejected;
- CR field live-out omissions;
- `XER.CA`, `XER.OV`, `XER.SO` boundaries;
- FPSCR and FPR changes when written by only one side;
- exit target/kind differences;
- LR/CTR behavior under appropriate contracts.

### Definedness and invalid states

- one side invalid and the other valid;
- both invalid for different reasons;
- divide-by-zero/overflow boundaries;
- unaligned memory access;
- FP inputs outside the modeled domain;
- verify exact intended partial-equivalence policy.

The test names must state whether the expected theorem is partial equivalence or exception equivalence. Do not let accidental current behavior become an undocumented specification.

### Control flow

- branch predicate differs only at signed/unsigned boundary;
- condition-register field indexing differences;
- link bit and return behavior;
- one side has an extra feasible exit;
- infeasible paths caused by contradictory constraints;
- loop/back-edge rejection;
- indirect branch/call rejection;
- path-limit and instruction-limit outcomes.

### Relocations and layout

- inconsistent relocation equality must not create vacuous proof;
- branch displacement exact lower/upper bounds;
- alignment boundaries;
- SDA range boundaries;
- same symbol/addend canonicalization;
- different symbols must not be unified;
- impossible layout must report inconclusive layout, not equivalence.

### Calls and certificates

- missing callee certificate;
- stale direct certificate;
- stale transitive certificate;
- callee summary omits a read dependency;
- callee summary omits a write effect;
- changed helper hash;
- wrong architecture/result format;
- absolute-LR/return-address-sensitive callee rejected or downgraded;
- SAT through an opaque abstraction remains `INCONCLUSIVE_ABSTRACTION`.

### Floating point and paired-single

Boundary corpus:

- `+0` and `-0`;
- smallest/largest subnormal;
- smallest normal;
- largest finite;
- positive/negative infinity;
- quiet/signaling NaN patterns where modeled;
- rounding tie cases;
- fused vs non-fused differences;
- reciprocal/rsqrt estimate table boundaries;
- paired-single quantization type/scale bounds;
- FPSCR compare/classification bits;
- legal inputs excluded by current assumptions must yield explicit restricted scope, not silent universal claims.

## Test helper requirements

Provide one canonical helper that:

- decodes bytes through the production decoder;
- runs the production proof engine;
- verifies expected status;
- optionally checks a concrete witness with `ConcreteOps`;
- serializes a reproduction bundle when the assertion fails.

Avoid tests that manually construct internal state in ways real callers cannot.

## Acceptance criteria

- Every historical soundness bug has a permanent regression test.
- Negative tests fail against a deliberately reintroduced defect.
- Status assertions are exact, not broad `!= EQUIVALENT` checks unless multiple inconclusive outcomes are truly allowed.
- Soundness tests run in required PR CI.
