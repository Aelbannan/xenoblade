# P0-04 — Version the semantic fix and invalidate stale evidence

**Priority:** P0  
**Primary owner areas:** `result.py`, certificate code, caches, registry validation  
**Dependencies:** P0-01 and schema decisions from P0-02/P1-05

---

## Required version changes

The private-stack fix changes the theorem encoded by the engine. Therefore:

- bump `ARCHITECTURE_MODEL` to the next project-approved identifier;
- bump `RESULT_FORMAT` if result fields or serialized semantics change;
- bump `EQUIVALENCE_CERTIFICATE_VERSION` if certificate payload/validation changes;
- invalidate all proof caches keyed to the prior model;
- reject old certificates in automatic acceptance.

Do not hardcode `v19` in implementation plans if another semantic version lands first. Use "next model version" and let a single release commit choose the concrete identifier.

## Why this is mandatory

A proof result is not merely a solver answer. It means:

```text
UNSAT under architecture model X, result schema Y, contract C, assumptions A,
and exact implementation inputs I.
```

Changing memory comparison changes `X`. Reusing an old result under a new model is unsound even if source bytes are unchanged.

## Tests

- Old architecture model certificate is rejected.
- Old result format is rejected when required fields are absent.
- Old cache entry is not returned.
- Transitive caller certificate is rejected when any callee uses old model.
- Full-match registry row is not incorrectly invalidated as a byte match.
- Migration report enumerates every affected accepted equivalence row.

## Implementation log

### Version bumps (2026-07-18)

| Constant | Old | New | File |
|---|---|---|---|
| `ARCHITECTURE_MODEL` | `broadway-ppc32-be-v18` | `broadway-ppc32-be-v19` | `tools/ppc_equivalence/result.py:7` |
| `RESULT_FORMAT` | 7 | 8 | `tools/ppc_equivalence/result.py:8` |
| `EQUIVALENCE_CERTIFICATE_VERSION` | 1 | 2 | `tools/coop/lib/targets.py:40` |

### Certificate validation

`equivalence_certificate_error()` (`targets.py:50-123`) checks version, architecture model, and result format. Transitive callee certificates are validated recursively through the dependency chain. Old v18/format-7/version-1 certificates are automatically rejected by `validate_targets()`.

### Cache invalidation

`_cache_key()` (`equivalence_check.py:253-300`) includes `ARCHITECTURE_MODEL` and `RESULT_FORMAT` in the hash — old entries produce a different key and are never found. `_cache_get()` (`equivalence_check.py:303-322`) additionally rejects entries with mismatched architecture/format at read time as defense in depth.

### Tests added

**`tools/coop/tests/test_targets.py`** (5 tests):
- `test_old_architecture_model_certificate_rejected` — v18 cert rejected
- `test_old_result_format_certificate_rejected` — format-7 cert rejected
- `test_wrong_certificate_version_rejected` — version-1 cert rejected
- `test_transitive_callee_old_model_rejected` — callee with v18 rejected through chain
- `test_full_match_not_invalidated_by_stale_certificate` — FULL_MATCH not flagged

**`tools/coop/tests/test_equivalent_match.py`** (3 tests):
- `test_cache_rejects_old_architecture_model` — cache entry with v18 not returned
- `test_cache_rejects_old_result_format` — cache entry with format 7 not returned
- `test_cache_returns_current_entry` — positive control

### Known gaps (deferred)

- **Migration report**: No command or test to enumerate affected accepted equivalence rows. Deferred because zero `EQUIVALENT_MATCH` targets with certificates exist in the registry.
- **No live certificates affected**: Audit confirms 435 valid, 0 affected.
