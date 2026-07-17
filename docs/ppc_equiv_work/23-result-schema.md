# Suggested result schema

## Example proof result envelope

The exact schema should be reviewed and versioned. This example shows the required separation of solver answer, theorem scope, evidence, and policy:

```json
{
  "result_format": 8,
  "architecture_model": "broadway-ppc32-be-vNEXT",
  "status": "equivalent",
  "solver": {
    "name": "z3",
    "version": "...",
    "answer": "unsat",
    "phases": [
      {"name": "default", "result": "unsat", "elapsed_ms": 123}
    ]
  },
  "scope": {
    "contract": "auto",
    "partial_equivalence": true,
    "observables": {
      "gpr": [3, 4],
      "fpr": [],
      "cr_fields": [0],
      "xer": ["ca"],
      "memory": true,
      "exit": true,
      "definedness": true
    },
    "memory": {
      "comparison": "touched-byte-extensional",
      "profile": "bounded-ordinary-ram",
      "private_stack_masking": "per-implementation-independent-v1"
    },
    "floating_point": {
      "used": false
    },
    "calls": {
      "used": false,
      "callee_root_sha256": null
    }
  },
  "assumptions": [],
  "provenance": {
    "git_commit": "...",
    "dirty": false,
    "engine_tree_sha256": "...",
    "dependency_lock_sha256": "...",
    "proof_request_sha256": "...",
    "original_function_sha256": "...",
    "candidate_function_sha256": "..."
  },
  "validation": {
    "ledger_sha256": "...",
    "dolphin_version": "...",
    "corpus_sha256": "...",
    "all_used_opcodes_independently_validated": true
  },
  "confidence": {
    "tier": "B",
    "policy": "auto-promotion-v2",
    "promotion_allowed": true,
    "blockers": [],
    "warnings": []
  },
  "resources": {
    "deadline_ms": 120000,
    "elapsed_ms": 145
  }
}
```

### Design rules

- `status` reports the proof engine's answer.
- `confidence` reports evidence quality and policy.
- `promotion_allowed` is derived, not trusted if read from an unvalidated artifact.
- assumptions are structured identifiers with parameters, not only prose.
- exact hashes bind every relevant input.
- old readers must reject unknown required semantics rather than guessing.

## Source links used for this plan

- `tools/ppc_equivalence/README.md`
- `tools/ppc_equivalence/engine.py`
- `tools/ppc_equivalence/semantics.py`
- `tools/ppc_equivalence/result.py`
- `tools/ppc_equivalence/contract.py`
- `tools/ppc_equivalence/tests`
- `tools/coop/lib/equivalence_check.py`
- `tools/coop/lib/objdiff_report.py`
- `tools/coop/lib/targets.py`
- `.github/workflows/build.yml`
- `tools/ppc_equivalence/REFERENCES.md`
- `tools/ppc_equivalence/requirements.txt`

## Final implementation principle

The project should preserve automatic promotion, but only after making the promotion a conclusion of a complete evidence policy:

```text
current sound model
+ explicit theorem scope
+ definitive UNSAT
+ valid contract
+ valid environment profile
+ current transitive certificates
+ independent opcode validation
+ complete provenance
+ allowed confidence tier
= automatic EQUIVALENT_MATCH acceptance
```

Anything less may still be useful diagnostic evidence, but it must not silently become an accepted equivalence claim.
