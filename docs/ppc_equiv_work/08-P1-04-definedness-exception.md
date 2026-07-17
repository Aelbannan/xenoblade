# P1-04 — Clarify and test definedness and exception semantics

**Priority:** P1  
**Primary owner areas:** semantics, engine terminal comparison, `SOUNDNESS.md`

---

## Required decision

Choose and document one of these models:

1. **Partial functional equivalence:** compare outputs only over states where both sides satisfy modeled validity preconditions.
2. **Definedness-preserving partial equivalence:** require both sides to agree on whether execution is defined, then compare outputs when defined.
3. **Exception-aware total equivalence:** model and compare actual exception classes/effects.

The current implementation appears intended to compare definedness while suppressing ordinary output differences when both sides are invalid. That is a legitimate model, but it must be precise.

## Recommended near-term model

Use definedness-preserving partial equivalence:

- if exactly one side is valid, report difference;
- if both are valid, compare all observables;
- if both are invalid, require agreement on a normalized invalid reason class if that class is modeled;
- if invalid reasons are not modeled reliably, report `INCONCLUSIVE_UNMODELED_EXCEPTION` rather than universal equivalence for exception-heavy code.

## Extend state if necessary

Instead of a single Boolean `valid`, consider:

```python
class InvalidReason(Enum):
    NONE = "none"
    UNALIGNED_ACCESS = "unaligned-access"
    DIVIDE_UNDEFINED = "divide-undefined"
    FP_DOMAIN_EXCLUDED = "fp-domain-excluded"
    UNSUPPORTED_SIDE_EFFECT = "unsupported-side-effect"
    MEMORY_PROFILE_VIOLATION = "memory-profile-violation"
```

Symbolic representation may use one-hot predicates or a small bitvector enum. Do not pretend to distinguish hardware exception vectors unless implemented.

## Tests

- same invalid reason on both sides;
- different invalid reasons;
- valid vs invalid;
- both invalid but different earlier public writes;
- trap-enabled FP mode remains outside scope;
- unaligned load vs aligned load;
- divide edge cases.

## Promotion impact

Exception-sensitive or domain-excluded proofs must be Tier C until exception behavior is modeled and independently validated.
