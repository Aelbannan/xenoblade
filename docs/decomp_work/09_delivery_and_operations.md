# Delivery Plan, Rollout, and Operations

**Source:** Sections 22–30 of the LLM Decompilation Improvement Plan  
**Covers:** PR delivery breakdown, rollout strategy with feature flags, observability events, security requirements, documentation updates, definition of done, immediate first tasks, reference paths, final principle

---

## 22. Delivery plan by pull request

### PR 1 — Baseline-aware transactional promotion

**Scope:**

- Baseline capture and evaluation
- Candidate rank model
- Promotion policy
- Global promotion lock
- Fresh-root revalidation
- Transactional source write
- Rollback
- Promotion journal
- `coop cycle` exit-code enforcement
- Documentation correction

**Main files:**

- `tools/llm_harness/core.py`
- `tools/llm_harness/types.py`
- `tools/llm_harness/promotion.py`
- `tools/llm_harness/ranking.py`
- `tools/llm_harness/xenoblade_project.py`
- `tools/llm_harness/tests/test_promotion.py`

**Acceptance criteria:**

- Existing auto-promotion remains enabled.
- Worse candidates cannot replace a better baseline.
- Concurrent promotions cannot mutate canonical source simultaneously.
- Any failed validation restores exact original source bytes.
- Promotion result is persisted.

---

### PR 2 — Typed dossier and deterministic assembly context

**Scope:**

- Target dossier dataclasses
- Decoded retail assembly
- CFG
- Relocation annotations
- Existing accepted sibling source integration
- Signature and declaration extraction
- Caller snippets
- Allowed-symbol inventory
- Dossier JSON and Markdown artifacts

**Main files:**

- `tools/llm_harness/dossier.py`
- `tools/llm_harness/types.py`
- `tools/llm_harness/xenoblade_project.py`
- `tools/llm_harness/prompts/common.md`
- `tools/llm_harness/prompts/reconstruct.md`
- `tools/llm_harness/tests/test_dossier.py`

**Acceptance criteria:**

- The model no longer needs to decode raw hex manually.
- Every generation prompt has an exact authoritative signature.
- Accepted sibling bodies are visible, not only identifiers.
- Context limits are enforced and tested.
- Dossier generation is deterministic.

---

### PR 3 — Compile diagnostic classifier and repair loop

**Scope:**

- Diagnostic parser
- Stable fingerprints
- Root-cause selection
- Three reconstruction strategies
- Candidate lineage
- Compile-repair prompt
- Sequential repair
- Repeated-error blocking
- Budget accounting and resume support

**Main files:**

- `tools/llm_harness/pipeline.py`
- `tools/llm_harness/diagnostics.py`
- `tools/llm_harness/types.py`
- `tools/llm_harness/prompts/compile_repair.md`
- `tools/llm_harness/tests/test_diagnostics.py`
- `tools/llm_harness/tests/test_pipeline.py`

**Acceptance criteria:**

- Compiler output from one candidate guides its next repair.
- The same diagnostic is not retried indefinitely.
- The experiment state shows complete candidate lineage.
- Old independent-run workflow remains available behind a temporary compatibility flag until benchmarked.

---

### PR 4 — Structural comparison and semantic routing

**Scope:**

- Retail/candidate machine-code fingerprints
- Calls, relocations, memory, CFG, constants, returns, and class scores
- Structural report
- Semantic-repair prompt
- Stage router
- Structural promotion gate

**Main files:**

- `tools/llm_harness/structural.py`
- `tools/llm_harness/pipeline.py`
- `tools/llm_harness/prompts/semantic_repair.md`
- `tools/llm_harness/tests/test_structural.py`
- `tools/llm_harness/tests/test_pipeline.py`

**Acceptance criteria:**

- Compiling but semantically unrelated candidates are not sent directly to MWCC tuning.
- Reports identify concrete missing and unexpected behavior.
- First-compile promotion uses structural plausibility.
- Match improvement cannot significantly regress structure.

---

### PR 5 — Match improvement and equivalence-aware ranking

**Scope:**

- Normalized objdiff evidence
- Improved match prompt
- Independent semantic/static/proof dimensions
- Proof-aware rank
- Counterexample summaries
- Stop-on-proof configuration
- Per-dimension best candidates

**Main files:**

- `tools/llm_harness/pipeline.py`
- `tools/llm_harness/ranking.py`
- `tools/llm_harness/xenoblade_project.py`
- `tools/llm_harness/prompts/improve.md`
- `tools/llm_harness/tests/test_ranking.py`
- PPC equivalence integration tests

**Acceptance criteria:**

- Proof outranks fuzzy similarity.
- Structural alignment is preserved during codegen tuning.
- Inconclusive verification never becomes acceptance.
- Full match still has highest priority.

---

### PR 6 — Knowledge retrieval and attempt clustering

**Scope:**

- Stable compiler cookbook
- Target-specific retrieval
- Attempt clustering
- Provenance
- Context-token metrics
- Removal of full-history prompt insertion

**Main files:**

- `tools/llm_harness/knowledge.py`
- `tools/llm_harness/xenoblade_project.py`
- `tools/llm_harness/tests/test_knowledge.py`
- Knowledge/reference storage documentation

**Acceptance criteria:**

- Prompt contains only bounded relevant records.
- Exact target history is summarized, not copied in full.
- Retrieval is deterministic and auditable.
- Every retrieved lesson has provenance.

---

### PR 7 — Held-out benchmark and calibration

**Scope:**

- Benchmark manifest
- Hidden-source preparation
- Ablation runner
- JSON and Markdown reports
- Baseline comparison
- Threshold calibration

**Main files:**

- `tools/llm_harness/benchmark.py`
- `tools/llm_harness/tests/benchmark/`
- `docs/llm-harness-benchmark.md`

**Acceptance criteria:**

- Current and staged pipelines run on the same fixed corpus.
- Reports include compile, structural, match, proof, cost, and promotion metrics.
- Promotion thresholds and structural weights are justified by data.
- New pipeline becomes default only after meeting agreed success criteria.

---

## 23. Rollout strategy

### 23.1 Feature flags

Add temporary flags:

```json
{
  "features": {
    "transactional_promotion": true,
    "typed_dossier": true,
    "staged_pipeline": false,
    "structural_routing": false,
    "retrieved_knowledge": false
  }
}
```

Enable features progressively.

### 23.2 Shadow mode

Before allowing the new rank and promotion gate to mutate source:

- Run promotion decision in shadow mode.
- Compare old and new decisions.
- Record disagreements.
- Inspect cases where old logic would promote and new logic would reject.
- Inspect cases where new logic would promote first-compile candidates.

### 23.3 Default transition

Recommended order:

1. Transactional promotion on, old pipeline
2. New dossier on, old independent calls
3. Compile-repair pipeline on
4. Structural routing on
5. Retrieved knowledge on
6. Remove old pipeline after benchmark and a deprecation window

### 23.4 Failure fallback

If staged orchestration fails unexpectedly:

- Preserve experiment state.
- Do not mutate canonical source.
- Permit resume.
- Optionally fall back to independent generation only when explicitly configured.
- Do not silently switch behavior because that corrupts benchmark comparability.

---

## 24. Observability

### 24.1 Event log

Emit structured events:

```json
{
  "event": "candidate_evaluated",
  "experiment_id": "...",
  "target_id": "...",
  "candidate_id": "...",
  "stage": "compile_repair",
  "status": "compiles",
  "match_percent": 12.4,
  "structural_score": 0.66,
  "timestamp": "..."
}
```

Important events:

- Baseline captured
- Dossier built
- Model call started/completed
- Candidate parsed
- Compile diagnostic classified
- Candidate evaluated
- Stage routed
- Branch blocked
- Proof started/completed
- Promotion requested
- Promotion revalidated
- Promotion applied
- Promotion rolled back
- Experiment completed

### 24.2 Summary report

Every experiment should generate `summary.md`:

```markdown
# Experiment summary

- Baseline: COMPILES, 0.0%, structural 0.18
- Initial candidates: 3
- Compile repairs: 2
- First compile: candidate-05
- Best structural score: 0.82
- Best match: 31.7%
- Proof: inconclusive — unsupported instruction
- Promotion: yes
- Promotion reason: replaced placeholder; structural score 0.82
- Rollback: no
- Total calls: 7/10
- Total tokens: ...
- Total cost: ...
```

### 24.3 Aggregate metrics

Extend existing stats with:

- Compile-repair success by diagnostic category
- Calls to first compile
- Structural routing counts
- Semantic-repair success
- Match-improvement success
- Blocked reasons
- Promotion rejection reasons
- Rollback reasons
- Promotion lock wait time
- Stale-root revalidation count
- Prompt context composition and token count

---

## 25. Security and trustworthiness considerations

Although this is not a security-sensitive production service, the harness mutates source automatically. Treat model output as untrusted.

Requirements:

- Parse strict schemas.
- Restrict patches to the expected target file and source range.
- Reject path traversal.
- Reject additional file edits.
- Reject shell commands in model output.
- Run compilation in isolated worktrees.
- Record exact model/provider/config versions.
- Hash prompts and responses.
- Do not treat model confidence as acceptance evidence.
- Do not treat structural score as proof.
- Keep verifier unsupported/unknown results fail-closed.
- Roll back any failed canonical mutation.

Source patch validation example:

```python
def validate_patch(patch: SourcePatch, target: TargetMetadata) -> None:
    if patch.path != target.source_file:
        raise InvalidPatch("candidate changed an unexpected file")

    if patch.start < target.function_start or patch.end > target.function_end:
        raise InvalidPatch("candidate changed outside the target function")

    if contains_preprocessor_directive(patch.replacement):
        raise InvalidPatch("preprocessor edits are not permitted")

    if changes_signature(patch.replacement, target.signature):
        raise InvalidPatch("target signature is authoritative")
```

---

## 26. Documentation updates

Update the harness README to accurately state:

- Automatic promotion is enabled.
- Promotion is baseline-aware and transactional.
- Candidate worktrees are isolated.
- Canonical promotion is serialized.
- Prompt knowledge is retrieved and bounded.
- Function signatures are authoritative by default.
- Blocked targets do not consume endless repair attempts.
- Structural similarity is routing evidence, not proof.
- PPC equivalence remains fail-closed.
- Exact match and proven equivalence are separate outcomes.

Add developer docs for:

- Adding a diagnostic rule
- Adding a structural component
- Adding a prompt stage
- Creating a benchmark target
- Investigating a rollback
- Replaying an experiment
- Updating state schema

---

## 27. Definition of done

### Promotion

- Automatic promotion remains operational.
- Every promotion compares against a fresh baseline.
- Promotion is monotonic under the configured rank.
- Canonical writes are serialized.
- Failed integration always triggers rollback.
- Promotion journals are complete and replayable.

### Candidate generation

- The primary prompt contains decoded assembly and CFG.
- Exact signatures and relevant declarations are included.
- Accepted sibling source bodies are available.
- Agents do not need to decode raw hex manually.
- Three distinct initial strategies are generated.

### Compile closure

- Compiler diagnostics are classified.
- Sequential repair uses exact prior errors.
- Repeated failures stop a branch.
- Missing context yields a blocked status.
- Compile-repair success is measurable.

### Semantic routing

- Structural reports compare observable program shape.
- Low-structure candidates go to semantic repair.
- High-structure candidates go to MWCC improvement.
- Structural reports are not misrepresented as proof.

### Verification

- Equivalence proof remains independent from fuzzy similarity.
- Unsupported and inconclusive results fail closed.
- Proof results affect ranking correctly.
- Counterexamples are summarized when available.

### Knowledge and benchmarking

- Prompt history is bounded and relevant.
- Repeated attempts are clustered.
- A held-out benchmark exists.
- The new pipeline demonstrates measurable improvement.
- Thresholds are calibrated from benchmark data.

---

## 28. Immediate first tasks

The first implementing agent should begin with the following sequence:

1. Add `BaselineSnapshot`, `CandidateEvaluation`, and `PromotionResult` types.
2. Add a `ranking.py` module with explicit tested ordering.
3. Add a `promotion.py` manager with a global lock and source transaction.
4. Change `core.py` so the generated winner is not automatically synonymous with a promotion winner.
5. Capture and evaluate canonical baseline before model calls.
6. Re-evaluate the winning candidate against the latest root inside the promotion lock.
7. Treat nonzero `coop cycle` as promotion failure.
8. Roll back exact original source bytes.
9. Add promotion journal artifacts.
10. Add concurrency and rollback integration tests.
11. Only after PR 1 is stable, wire decoded assembly and accepted sibling bodies into the dossier.
12. Record current benchmark metrics before enabling staged generation by default.

This order protects the repository before increasing candidate quality and automatic throughput.

---

## 29. Reference paths

Primary repository areas referenced by this plan:

- `tools/llm_harness/core.py`
- `tools/llm_harness/xenoblade_project.py`
- `tools/llm_harness/types.py`
- `tools/llm_harness/workspace.py`
- `tools/llm_harness/prompts/common.md`
- `tools/llm_harness/prompts/new.md`
- `tools/llm_harness/prompts/improve.md`
- `tools/ppc_equivalence/`
- `llm-harness.json`

Repository:

- `https://github.com/Aelbannan/xenoblade`

---

## 30. Final implementation principle

The harness should spend model calls only when each call receives new evidence.

```text
Deterministic context before speculation.
Compiler feedback before another compile guess.
Semantic alignment before code-generation tuning.
Proof before trust.
Fresh baseline before automatic promotion.
Rollback before repository corruption.
```
