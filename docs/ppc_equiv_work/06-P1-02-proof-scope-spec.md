# P1-02 — Write a formal, auditable proof-scope specification

**Priority:** P1  
**Primary output:** `tools/ppc_equivalence/SOUNDNESS.md`

---

## Purpose

The README is user documentation. Add a separate normative specification that maintainers can review against code.

## Required theorem statement

Use a statement similar to:

> For all shared initial machine states satisfying the selected contract preconditions, environmental profile, relocation/layout constraints, and instruction-domain restrictions, there exists no feasible pair of modeled terminal executions whose exit behavior, definedness state, or selected observable architectural outputs differ after applying only the explicitly documented per-implementation private-storage abstraction.

Then define every term.

## Required definitions

### Initial-state quantification

- Which GPRs/FPRs/CR fields/FPSCR/XER/LR/CTR/memory values are symbolic?
- Which state is constrained by ABI or environment?
- Are both functions started from exactly the same state?

### Feasible execution

- How path conditions are formed.
- How branches are paired.
- How loops, indirect branches, and path limits fail closed.

### Terminal behavior

- normal return;
- direct branch/tail call;
- trap/invalid state;
- target address observability;
- fallthrough, if allowed.

### `valid` and partial-domain equivalence

State explicitly whether the verifier proves partial equivalence:

```text
For all states where modeled operations remain valid, outputs agree.
```

or total exception equivalence:

```text
For all states, both value outputs and exception behavior agree.
```

The current design appears closer to partial equivalence. Document how pairs where both sides become invalid are treated, and prohibit marketing language that implies full hardware exception equivalence.

### Memory model

- 32-bit byte-addressed array;
- big-endian loads/stores;
- touched-byte extensional comparison;
- alignment policy;
- ordinary RAM/MMIO assumptions;
- address-space wrapping;
- private-stack ownership and escape.

### Contracts

- `auto` function contract;
- `ppc-eabi` limitations;
- internal-block live-outs;
- memory observability;
- one-sided writes included through union analysis.

### Calls

- summary determinism;
- input/output state components;
- preserved state;
- memory token/effects;
- return-address/location-independence premise;
- certificate-chain validity.

### Floating point

- rounding mode;
- NI mode;
- traps;
- overflow/underflow/inexact modeling;
- fused operation input restrictions;
- exact scope of estimates and paired-single operations.

### Relocations

- canonical symbol identity;
- supported relocation formulas;
- linkability constraints;
- independent layout-feasibility check.

## Traceability table

Include a table like:

| Soundness claim / assumption | Implementation | Tests | Result field |
|---|---|---|---|
| Independent stack masking | `engine._memory_difference` | `test_private_stack_memory.py` | `memory_scope.private_stack.masking_semantics` |
| Ordinary RAM range | memory-profile constraints | memory profile tests | `environment.memory_profile` |
| No unsupported loop | CFG executor | control-flow tests | `limits/unsupported_reason` |
| Nearest-even FP | FP preconditions | FP domain tests | `assumptions.floating_point.rounding` |
| Current callee chain | certificate validator | stale-transitive tests | `premises.callee_root_sha256` |

CI should fail if a normative assumption lacks an implementation or test reference.

## Acceptance criteria

- A reviewer can reconstruct the exact theorem without reading source.
- Every result assumption has a normative definition.
- The README links to `SOUNDNESS.md` and uses the same terminology.
- Promotion policy cites explicit proof-scope fields rather than parsing prose.
