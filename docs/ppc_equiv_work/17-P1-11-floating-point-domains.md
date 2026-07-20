# P1-11 — Make floating-point proof domains machine-readable

**Priority:** P1/P2  
**Primary owner areas:** FP semantics, result schema, promotion policy, tests

> **Superseding promotion bar:** Tier A full scalar FP is now tracked as the
> architecture-model release in
> [`tools/ppc_equivalence/SCALAR_FP_V2.md`](../../tools/ppc_equivalence/SCALAR_FP_V2.md).
> This document remains useful for structured domain metadata; Phases 1 and 10
> of that release absorb the machine-readable domain / obligation work.

---

## Problem statement

The FP/paired-single model is useful but restricted by assumptions such as nearest-even rounding, NI state, trap exclusion, finite-overflow restrictions, incomplete status flags, and input-shape restrictions for some fused operations.

These restrictions must be represented as predicates and structured proof-domain metadata, not only prose.

## Domain object

```python
@dataclass(frozen=True)
class FloatingPointDomain:
    rounding_modes: tuple[str, ...]
    require_ni_zero: bool
    traps_enabled: bool
    allow_nan: bool
    allow_infinity: bool
    allow_subnormal: bool
    exclude_finite_overflow: bool
    model_underflow_flag: bool
    model_inexact_flag: bool
    fused_input_domain: str
```

## Result example

```json
{
  "floating_point_scope": {
    "used": true,
    "rounding_modes": ["nearest-even"],
    "ni": "required-zero",
    "traps": "excluded",
    "finite_overflow": "excluded",
    "underflow_flag": "not-fully-modeled",
    "inexact_flag": "not-fully-modeled",
    "fused_input_domain": "exact-expanded-binary32"
  }
}
```

## Domain coverage query

For each proof, distinguish:

- no FP instructions;
- FP instructions but restrictions proven automatically from caller/input constraints;
- FP restrictions merely assumed;
- unsupported FP behavior encountered.

Only the second category should be eligible for stronger confidence without manual policy.

## Testing

- every excluded domain has a boundary test;
- result includes exactly the restrictions actually used;
- promotion tier falls when assumptions are merely external;
- NaN/sign-zero comparisons follow reference behavior;
- FPSCR output observability is tested independently.
