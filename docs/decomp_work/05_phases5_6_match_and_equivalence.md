# Phases 5 & 6 — MWCC Match Improvement and Equivalence Integration

**Source:** Sections 13–14 of the LLM Decompilation Improvement Plan  
**Covers:** Match improvement entry conditions, feedback normalization, improvement response, non-regression, proof/similarity separation, proof invocation, counterexample feedback, proof-aware ranking  
**PR:** 5 — Match improvement and equivalence-aware ranking

---

## 13. MWCC match improvement

### 13.1 Entry condition

Only run MWCC match improvement when:

- Candidate compiles
- Structural score is at or above the threshold
- Candidate is not already a full match
- Budget remains

### 13.2 Preserve current improve guidance

The existing improvement philosophy is appropriate at this stage:

- Signedness
- Load/store width
- Expression ordering
- Variable lifetimes
- Temporary introduction or removal
- Branch inversion
- Loop form
- Declaration placement
- Cast placement
- Inlining boundaries
- Return-expression form

The main change is routing.

### 13.3 Normalize objdiff feedback

Provide:

- First differing basic block
- First differing instruction
- Retail/candidate instruction windows
- Relocation differences
- Register-map hints where reliable
- Stack-frame differences
- Candidate and retail function size
- Structural report summary

Example:

```text
First meaningful difference:
Retail:
  lwz r4, 0x14(r3)
  cmpwi r4, 0
  beq ...

Candidate:
  lbz r4, 0x14(r3)
  cmplwi r4, 0
  beq ...

Likely source-level causes:
- Field width differs: 32-bit versus 8-bit
- Signed compare differs: signed versus unsigned
```

Deterministic hints should be labeled as observations or likely causes, not certainties.

### 13.4 Improvement response

```json
{
  "stage": "match_improve",
  "source": "...",
  "bounded_change": "Changed the local field type use from u8 to s32.",
  "evidence": [
    "Retail uses lwz rather than lbz",
    "Retail uses cmpwi rather than cmplwi"
  ],
  "expected_instruction_effect": [
    "32-bit load",
    "signed compare"
  ]
}
```

### 13.5 Non-regression requirement

A match-improvement candidate must not reduce structural score beyond a small tolerance.

Suggested:

```python
candidate.structural_score + 0.02 >= parent.structural_score
```

Any larger drop routes back to semantic analysis or rejects the candidate.

---

## 14. Equivalence integration

### 14.1 Separation of proof and similarity

Store independent dimensions:

```json
{
  "semantic_proof": "proved | disproved | inconclusive | unsupported",
  "structural_score": 0.86,
  "match_percent": 54.2,
  "full_match": false,
  "size_delta": -4
}
```

Do not describe structural similarity or fuzzy match as semantic proof.

### 14.2 Proof invocation

Invoke the PPC equivalence checker when:

- Both functions compile to supported analyzable code
- Required callee certificates exist, or compositional assumptions are allowed
- Structural score is sufficiently high, unless testing a deliberate proof-only path
- Budget permits

### 14.3 Inconclusive behavior

The verifier remains fail-closed:

- Unsupported instruction → inconclusive/unsupported
- Timeout → inconclusive
- Unknown external effect → inconclusive
- Missing certificate → inconclusive
- Solver `unknown` → inconclusive

None of these may promote as `EQUIVALENT_MATCH`.

### 14.4 Counterexample feedback

When equivalence fails and a useful counterexample exists, store a minimized summary for future semantic repair:

```json
{
  "proof_status": "disproved",
  "counterexample": {
    "input_registers": {
      "r3": "0x...",
      "r4": "0x..."
    },
    "differing_observable": "store at this+0x21",
    "retail_value": "0x01",
    "candidate_value": "0x00"
  }
}
```

Do not expose enormous raw solver models directly to the model. Summarize the smallest actionable discrepancy.

### 14.5 Proof-aware ranking

A proven-equivalent candidate outranks an unproven candidate even if its fuzzy match is somewhat lower.

Among two proven-equivalent candidates:

1. Higher static match
2. Better whole-object integration
3. Smaller non-retail size growth
4. Better source-quality score
5. Lower complexity
