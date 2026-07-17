# P1-05 — Strengthen call summaries and certificate soundness

**Priority:** P1  
**Primary owner areas:** `semantics.py`, `equivalence_check.py`, `targets.py`

---

## Threat model

A caller proof that uses a callee summary is only as sound as:

- the callee proof;
- the inferred read/write contract;
- the summary's state transition;
- the certificate validator;
- every transitive dependency;
- the premise that the callee does not depend on omitted context such as absolute return address.

## Required certificate payload

At minimum:

```json
{
  "certificate_version": 2,
  "status": "equivalent",
  "architecture_model": "...",
  "result_format": 8,
  "target_id": "...",
  "original_function_sha256": "...",
  "candidate_function_sha256": "...",
  "contract_sha256": "...",
  "engine_tree_sha256": "...",
  "environment_profile_sha256": "...",
  "proof_request_sha256": "...",
  "direct_callees": [],
  "transitive_callee_root_sha256": "...",
  "helpers": [],
  "validation_ledger_sha256": "...",
  "confidence_tier": "B",
  "certificate_sha256": "..."
}
```

## Read/write contract validation

For each summary:

- every state component read before being overwritten must be in inputs;
- every state component potentially written must be in outputs/effects;
- memory reads and writes need explicit region/effect representation;
- preserved nonvolatile state must be asserted, not assumed from naming alone;
- exit kind/target and definedness must be represented;
- nondeterministic or environment-dependent results must not be summarized as deterministic uninterpreted functions unless the nondeterminism is part of the function's input token.

## Return-address/location dependence

The summary currently relies on normal location-independent EABI behavior. Add detection/policy for callees that:

- read LR as data;
- spill and inspect the absolute return address;
- use PC/LR-derived tables in a way not covered by relocations;
- compare call-site identity.

Such functions should be rejected from ordinary location-independent summaries unless the call-site address is included as an explicit input.

## Certificate-chain validation tests

- direct stale hash;
- transitive stale hash;
- cycle in certificate graph;
- changed contract with same code;
- changed environment profile;
- changed validation ledger;
- changed helper;
- missing memory effect;
- malformed canonical JSON/hash ambiguity;
- duplicate or reordered dependencies produce a stable canonical root.

## Abstraction result handling

Preserve the safe current principle:

- `unsat` under sound summaries may prove caller equivalence;
- `sat` involving opaque summary values may not provide a concrete machine witness and must remain `INCONCLUSIVE_ABSTRACTION` unless concretized.

Document this in the result schema and UI.
