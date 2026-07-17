# Testing and Benchmark Plan

**Source:** Sections 20–21 of the LLM Decompilation Improvement Plan  
**Covers:** Unit tests by module, integration tests, benchmark dataset, hidden-source setup, ablations, metrics, acceptance targets  
**PR:** 7 — Held-out benchmark and calibration

---

## 20. Testing plan

Tests are mandatory for each phase.

### 20.1 Promotion unit tests

1. Worse compiling candidate does not replace a compiling baseline.
2. First compiling candidate can replace a non-compiling baseline.
3. First compiling candidate below structural threshold is rejected.
4. Placeholder baseline can be replaced by a plausible compiling candidate.
5. Full match promotes.
6. Proven-equivalent improving candidate promotes.
7. Equivalent but integration-regressing candidate is rejected.
8. Higher fuzzy match with severe structural regression is rejected.
9. Accepted sibling regression blocks promotion.
10. Nonzero `coop cycle` result causes rollback.
11. Build failure after source write causes rollback.
12. Source hash mismatch triggers fresh re-evaluation.
13. Concurrent promotions are serialized.
14. Rollback journal is written.
15. Rollback failure is reported as a critical error.
16. Canonical source equals original bytes after rollback.
17. Candidate source is preserved in experiment evidence after rejection.

### 20.2 Dossier tests

1. Decoded instructions are present.
2. Raw words map to correct instruction addresses.
3. Branch targets map to correct basic blocks.
4. Relocations attach to the correct instructions.
5. Exact function signature is included.
6. Signature is marked authoritative.
7. Caller count respects configuration.
8. Accepted sibling count respects configuration.
9. Sibling source bodies are included.
10. Relevant declarations are included.
11. Unrelated large declarations are excluded.
12. Allowed symbol inventory is deterministic.
13. Missing retail instructions blocks generation.
14. Dossier JSON is stable across repeated runs.
15. Dossier Markdown contains all required sections.
16. Context truncation is explicitly marked.

### 20.3 Diagnostic tests

Use real captured MWCC output fixtures.

1. Syntax error classification.
2. Unknown identifier classification.
3. Unknown member classification.
4. Wrong argument count classification.
5. Invalid conversion classification.
6. Incomplete type classification.
7. Access-control classification.
8. Linkage-error classification.
9. Workspace path normalization.
10. Experiment ID normalization.
11. Duplicate diagnostic grouping.
12. Root diagnostic selection.
13. Stable fingerprint across different worktree paths.
14. Repeated fingerprint detection.

### 20.4 Pipeline tests

1. Three initial branches are generated.
2. Initial branches receive different strategy labels.
3. Initial calls do not receive compiler feedback from sibling calls.
4. Compile repair receives parent source and exact diagnostics.
5. Repair lineage points to parent candidate.
6. A successful compile ends compile repair for that branch.
7. Repeated diagnostic blocks the branch.
8. Second branch is tried when first branch blocks.
9. Low structural score routes to semantic repair.
10. High structural score routes to match improvement.
11. Semantic repair that breaks compile receives at most one local compile repair.
12. Full match stops new submissions.
13. Running futures are drained and recorded.
14. Resume does not repeat completed calls.
15. Budget accounting survives resume.
16. Blocked context stops inappropriate repairs.
17. Winner selection is independent from promotion acceptance.

### 20.5 Structural tests

Create synthetic decoded functions.

1. Identical fingerprints score 1.0.
2. Missing call lowers call score.
3. Unexpected call applies penalty.
4. Byte/word access mismatch is reported.
5. Different register assignment does not count as memory mismatch.
6. Equivalent branch inversion retains reasonable CFG score.
7. Missing return path lowers return score.
8. Prologue constants do not dominate constant score.
9. Global store mismatch applies a strong penalty.
10. Scores remain in `[0, 1]`.
11. Weights sum correctly.
12. Reports are deterministic.

### 20.6 Equivalence integration tests

1. Proven equivalence sets semantic proof status.
2. Counterexample sets disproved status.
3. Timeout sets inconclusive status.
4. Unsupported instruction sets unsupported status.
5. Missing callee certificate remains inconclusive.
6. Inconclusive proof cannot promote as equivalent.
7. Counterexample summary is persisted.
8. Proof outranks fuzzy match in ranking.

### 20.7 Knowledge tests

1. Exact target records rank first.
2. Same-class records outrank unrelated recent records.
3. Record count cap is enforced.
4. Attempt clustering groups equivalent signedness hypotheses.
5. Conflicting lessons remain separate.
6. Proven lessons outrank single-attempt speculation.
7. Prompt excludes full unrelated history.
8. Retrieval is deterministic.

### 20.8 Integration tests

Use a small fixture repository with a fake compiler and adapter.

Scenarios:

- Stub → compile repair → plausible compile → auto-promotion
- Compile error → repeated error → blocked
- Compile → low structural score → semantic repair → high structure
- High structure → match improvement → full match
- Candidate prepared against stale root → fresh re-evaluation
- Concurrent target runs → serialized promotions
- Promotion integration failure → successful rollback
- Resume halfway through compile repair
- Model provider error without corrupting state
- Cost limit reached before promotion

---

## 21. Benchmark plan

### 21.1 Dataset

Select 30–50 accepted functions, stratified by:

- Function size
- Number of basic blocks
- Number of calls
- Loops
- Member access count
- Global access
- Integer versus floating point
- Instruction support level
- Existing exact-match status
- Class/subsystem

Exclude functions whose source appears in context through an accepted sibling or knowledge record.

### 21.2 Hidden-source setup

For each benchmark target:

- Replace accepted source with its pre-decomp placeholder in an isolated benchmark branch.
- Preserve headers and surrounding project state.
- Remove exact-target attempt history.
- Prevent retrieval of the accepted implementation.
- Verify the source does not leak through caller snippets or duplicated functions.

### 21.3 Ablations

Run at least:

1. Raw hex, independent sampling
2. Decoded assembly, independent sampling
3. Full dossier, independent sampling
4. Full dossier, staged repair
5. Full dossier and staged repair with retrieved knowledge
6. Full dossier and staged repair without sibling bodies
7. Structural routing disabled
8. Equivalence-aware ranking disabled

### 21.4 Metrics

Primary:

- Compile rate within budget
- Median calls to first compile
- Nonzero static-match rate
- Median best match
- Structural alignment rate
- Proven-equivalent rate
- Full-match rate
- Cost per promoted function
- Promotion survival rate

Secondary:

- First-call compile rate
- Repeated diagnostic rate
- Blocked-context rate
- Rollback rate
- Mean prompt tokens
- Mean output tokens
- Time spent compiling
- Unsupported verifier rate
- Source-quality review score

### 21.5 Promotion survival rate

Define:

```text
promotions that remain canonical or are monotonically improved
---------------------------------------------------------------
total automatic promotions
```

Measure after:

- Next promotion to the same target
- TU closure
- Periodic full build
- A fixed number of later experiments

### 21.6 Acceptance targets

Initial success criteria relative to current baseline:

- At least 2× compile-within-budget rate
- At least 50% reduction in repeated identical compiler failures
- At least 2× nonzero-match rate among compiled candidates
- Zero known stale-root promotion corruption
- Zero unrolled-back failed promotions
- At least 90% promotion survival in benchmark fixtures
- No reduction in full-match discovery rate

These values should be calibrated after recording the current baseline.
