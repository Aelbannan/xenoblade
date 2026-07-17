# P1-09 — Synchronize versioned documentation automatically

**Priority:** P1  
**Primary owner areas:** README generation/checks, CI

---

## Goal

Prevent architecture/result identifiers and support claims from drifting between code and documentation.

## Generated markers

In the README:

```markdown
<!-- BEGIN GENERATED PPC_EQUIVALENCE_VERSION -->
- Architecture model: `broadway-ppc32-be-vNEXT`
- Result format: `N`
- Certificate format: `M`
<!-- END GENERATED PPC_EQUIVALENCE_VERSION -->
```

Add a script:

```bash
python -m tools.ppc_equivalence.docs_sync --write
python -m tools.ppc_equivalence.docs_sync --check
```

`--check` exits nonzero when generated sections differ.

## Generate more than version numbers

Where feasible, generate:

- `ProofStatus` table;
- supported opcode list from decoder registration;
- result schema version;
- known proof profiles;
- current assumptions sourced from structured constants;
- validation-ledger summary.

Do not generate normative explanatory prose that requires human review.

## Tests/CI

- version change without regenerated README fails;
- newly supported opcode without ledger entry fails;
- documented opcode absent from decoder fails;
- result schema example validates against current schema.
