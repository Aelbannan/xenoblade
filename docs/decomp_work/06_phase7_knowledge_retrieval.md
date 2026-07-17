# Phase 7 — Knowledge Retrieval

**Source:** Section 15 of the LLM Decompilation Improvement Plan  
**Covers:** Stable compiler cookbook, target-specific retrieval, attempt clustering, knowledge provenance  
**PR:** 6 — Knowledge retrieval and attempt clustering

---

## 15.1 Stable compiler cookbook

Create a compact stable prefix containing:

- PowerPC EABI essentials
- MWCC source-shaping guidance
- Common compiler lowering patterns
- Project patch constraints
- Output schemas
- Promotion and proof terminology

Do not include every historical attempt.

## 15.2 Target-specific retrieval

Retrieve up to ten records using weighted features:

```python
score = (
    5.0 * exact_target
    + 3.0 * same_class
    + 2.5 * same_translation_unit
    + 2.0 * same_diagnostic_category
    + 2.0 * same_relocation_shape
    + 1.5 * cfg_similarity
    + 1.5 * instruction_mismatch_similarity
    + 1.0 * recency_decay
)
```

Use deterministic tie-breaking.

## 15.3 Attempt clustering

Cluster repeated attempts by normalized hypothesis and observed effect.

Example source records:

```text
Attempt 1: change parameter to unsigned → 18.2%
Attempt 2: use u32 local → 18.3%
Attempt 3: cast comparison to unsigned → 18.2%
```

Cluster:

```json
{
  "cluster_id": "signedness-comparison",
  "attempt_count": 3,
  "best_match_percent": 18.3,
  "best_structural_score": 0.81,
  "conclusion": "Unsigned comparison changes did not materially improve codegen.",
  "avoid_without_new_evidence": true
}
```

## 15.4 Knowledge provenance

Every retrieved record should include:

- Source experiment
- Target
- Date
- Model
- Compiler version/config hash
- Dossier version
- Outcome
- Confidence
- Whether the lesson was confirmed more than once

Do not treat a single failed attempt as a universal compiler rule.
