# Phases 3 & 4 — Structural Comparison and Semantic Repair

**Source:** Sections 11–12 of the LLM Decompilation Improvement Plan  
**Covers:** Structural fingerprint inputs, calls/relocations, memory accesses, CFG shape, constants, return paths, instruction classes, weights, penalties, routing thresholds, semantic repair prompt, one-discrepancy rule, semantic frontier  
**PR:** 4 — Structural comparison and semantic routing

---

## 11. Structural comparison

### 11.1 Objective

Structural comparison estimates whether candidate and retail machine code represent similar observable behavior.

It is not a proof and must not be described as one.

It exists to:

- Route low-structure candidates to semantic repair
- Route high-structure candidates to MWCC match improvement
- Prevent accidental prologue/epilogue similarity from dominating
- Gate first-compile auto-promotion
- Provide precise feedback

### 11.2 Fingerprint inputs

For retail and candidate functions extract:

- Direct calls
- Relocation types and symbols
- Memory accesses
- Constants
- CFG shape
- Return paths
- Instruction classes
- Stack frame
- Callee-saved register pattern
- Unsupported instructions
- Indirect control flow

### 11.3 Calls and relocations

Compare as multisets, not only sets.

Example:

```python
retail_calls = Counter([
    ("R_PPC_REL24", "funcA"),
    ("R_PPC_REL24", "funcB"),
])

candidate_calls = Counter([
    ("R_PPC_REL24", "funcA"),
])
```

Report:

```text
missing call: funcB ×1
```

Unexpected calls should receive a stronger penalty than a missing compiler helper known to be equivalent. Support an allowlist for compiler-generated helpers.

### 11.4 Memory accesses

Represent access identity conservatively:

```python
@dataclass(frozen=True)
class MemoryAccess:
    kind: str            # load/store
    width_bits: int
    base_class: str      # this, stack, global, argument, unknown
    offset: int | None
    symbol: str | None
    signed: bool | None
```

Initial comparison should not require exact register identity.

Important mismatches:

- Load versus store
- 8/16/32-bit width
- Offset
- Global symbol
- Number of accesses
- Signed extension after load
- Volatile or unknown side effects where detectable

### 11.5 CFG shape

A first implementation can compare:

- Number of blocks
- Number of edges
- Number of conditional branches
- Number of unconditional branches
- Number of returns
- Presence and count of backedges
- Exit block count
- Dominator-tree shape hash
- Loop nesting count

Later, graph edit distance or block signatures can be added.

Avoid requiring identical block order. Source transformations may invert branches while preserving the same shape.

### 11.6 Constants

Ignore routine prologue constants such as stack offsets when scoring semantic constants.

Classify:

- Immediate compare values
- Bit masks
- Shift amounts
- Field offsets
- Enum-like constants
- Floating-point constants
- Address addends

### 11.7 Return paths

Capture:

- Return register assignments
- Distinct constant returns
- Number of exit paths
- Whether a returned call value is passed through
- Whether return is conditional

### 11.8 Instruction classes

Use coarse classes:

```text
integer arithmetic
logical
shift/rotate
compare
conditional branch
unconditional branch
call
load
store
sign/zero extension
floating point
paired single
special register
return
```

This component should have the lowest semantic weight because correct source can compile into different instruction sequences.

### 11.9 Initial weights

```python
STRUCTURAL_WEIGHTS = {
    "calls": 0.20,
    "relocations": 0.10,
    "memory_accesses": 0.25,
    "cfg": 0.20,
    "constants": 0.10,
    "returns": 0.10,
    "instruction_classes": 0.05,
}
```

Calls and relocations are separated because non-call global relocations are also meaningful.

Calibrate weights using held-out known matches before treating thresholds as stable.

### 11.10 Penalties

Apply explicit penalties for high-risk discrepancies:

```python
PENALTIES = {
    "unexpected_call": 0.10,
    "unexpected_global_store": 0.15,
    "missing_global_store": 0.15,
    "indirect_call_difference": 0.20,
    "return_path_difference": 0.10,
}
```

Clamp total score to `[0, 1]`.

### 11.11 Routing thresholds

Initial defaults:

```json
{
  "structural": {
    "semantic_repair_below": 0.75,
    "match_improve_at_or_above": 0.75,
    "first_compile_promotion_min": 0.60
  }
}
```

The 0.60 first-compile threshold is intentionally below the 0.75 MWCC-improvement threshold. A plausible first implementation can be worth promoting over a stub even when semantic repair remains useful.

### 11.12 Structural report example

```json
{
  "total_score": 0.58,
  "calls": {
    "score": 0.5,
    "matched": 1,
    "expected": 2,
    "details": ["missing call: funcB"]
  },
  "memory_accesses": {
    "score": 0.4,
    "matched": 2,
    "expected": 4,
    "details": [
      "retail stores 8-bit value at this+0x21",
      "candidate stores 32-bit value at this+0x20"
    ]
  },
  "cfg": {
    "score": 0.8,
    "matched": 4,
    "expected": 5,
    "details": ["candidate has one exit; retail has two"]
  },
  "unexpected_effects": [
    "candidate calls global logging function"
  ],
  "missing_effects": [
    "missing store at this+0x21"
  ]
}
```

---

## 12. Semantic repair

### 12.1 When semantic repair runs

Run semantic repair when:

- Candidate compiles, and
- Structural score is below the configured alignment threshold, and
- The candidate is not blocked, and
- Semantic-repair budget remains.

### 12.2 Prompt objective

The model must fix the largest observable discrepancy, not broadly rewrite the function.

Create `prompts/semantic_repair.md`.

```markdown
The function compiles but its generated machine-code structure differs
materially from retail.

Correct the largest observable semantic discrepancy shown in the structural
report.

Prioritize:
1. Missing or extra calls
2. Missing or extra memory writes
3. Incorrect access widths or offsets
4. Incorrect return paths
5. Incorrect branch or loop structure
6. Incorrect constants

Do not optimize register allocation, instruction ordering, or exact MWCC
matching until structural alignment is reached.
```

### 12.3 Evidence supplied

- Current source
- Current semantic summary
- Structural component scores
- Concrete missing/extra observations
- Retail decoded assembly
- Candidate decoded assembly
- Relevant declarations
- Parent lineage
- Previously attempted semantic changes

### 12.4 One-discrepancy rule

Require the response to identify one primary correction:

```json
{
  "stage": "semantic_repair",
  "primary_discrepancy": "missing call to funcB on the nonzero state path",
  "source": "...",
  "expected_effect": {
    "calls": ["add funcB"],
    "cfg": ["preserve early return"]
  }
}
```

This makes attempt history clusterable and reduces uncontrolled rewrites.

### 12.5 Semantic repair loop

```python
for repair_index in range(max_semantic_repairs):
    report = current.evaluation.structural_report

    if report.total_score >= threshold:
        break

    response = semantic_repair(current, report)
    repaired = evaluate(response)

    if repaired.evaluation.compile_report.succeeded is False:
        repaired = run_limited_compile_repair(
            repaired,
            budget=1,
        )

    if not improves_semantic_frontier(current, repaired):
        record_non_improving_attempt(...)
        maybe_try_next_branch()
        break

    current = repaired
```

A semantic repair that temporarily breaks compilation may receive one compile repair, but it must not consume the full compile-repair pool repeatedly.

### 12.6 Semantic frontier

Track the best candidate independently for:

- Call agreement
- Memory-effect agreement
- CFG agreement
- Overall structural score
- Static match
- Proof status

This prevents loss of a candidate that is strong in one semantic dimension.
