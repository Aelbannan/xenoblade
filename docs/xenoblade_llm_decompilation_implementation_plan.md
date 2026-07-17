# Xenoblade LLM Decompilation Harness Improvement Plan

**Repository:** `https://github.com/Aelbannan/xenoblade`  
**Primary area:** `tools/llm_harness`  
**Related verifier:** `tools/ppc_equivalence`  
**Plan status:** Implementation-ready  
**Review date:** 2026-07-17

---

## 1. Purpose

This document defines a staged implementation plan for improving the Xenoblade LLM-assisted decompilation workflow.

The existing system has a strong evaluation foundation:

- Isolated candidate worktrees
- Real compiler execution
- Objdiff-based similarity measurements
- Whole-object checks
- Persistent experiment records
- Bounded model calls
- A fail-closed PPC equivalence verifier
- Existing automatic candidate promotion

The main weakness is candidate generation. Current runs commonly:

1. Fail to compile, or
2. Compile but produce little or no meaningful match.

The proposed design addresses those failures in order:

1. Make automatic promotion safe and monotonic.
2. Give the model deterministic, useful program context.
3. Replace independent guesses with sequential compiler-guided repair.
4. Distinguish semantic mismatches from compiler-code-generation mismatches.
5. Use MWCC-specific matching only after semantic structure is plausible.
6. Retrieve relevant knowledge instead of inserting the complete history into every prompt.
7. Benchmark the pipeline on held-out accepted functions.

The final workflow remains fully automatic. Successful candidates continue to be promoted without human approval, but promotion becomes transactional, baseline-aware, and reversible.

---

## 2. Agreed design decisions

The following decisions are resolved and should be treated as requirements unless changed in a later design revision.

### 2.1 Automatic promotion remains enabled

The harness must continue to apply successful candidates to canonical source automatically.

Automatic promotion must not mean unconditional promotion. A candidate must measurably improve the current canonical baseline and pass integration checks.

### 2.2 Canonical function signatures are authoritative by default

The model must preserve the registered source signature.

Signature inference may be emitted as diagnostic metadata, but the model must not change the signature during normal function reconstruction.

A target may explicitly opt into an `uncertain_signature` mode later, but that is outside the initial implementation.

### 2.3 `COMPILES` candidates may auto-promote only under constrained conditions

A candidate whose strongest result is merely `COMPILES` may promote when:

- The canonical baseline does not compile, or
- The canonical baseline is a recognized placeholder/stub, and
- The candidate passes a minimum structural plausibility threshold, and
- It does not cause regressions.

Once the canonical implementation already compiles and is not a placeholder, a new candidate must improve structural similarity, static match, proof status, or another configured monotonic metric.

### 2.4 Prompt context is self-contained but bounded

The harness may inline relevant source excerpts even if agents are forbidden from browsing the repository themselves.

The default context bundle includes:

- Exact function declaration
- Containing class or relevant type declarations
- Direct callee declarations
- Two or three representative callers
- Up to four accepted sibling implementations
- Relevant enums, constants, globals, and typedefs
- Decoded retail assembly and relocation annotations

### 2.5 Function reconstruction does not edit headers initially

The first implementation only replaces the target function body or complete target function, according to the existing source patch contract.

When a valid implementation requires an external declaration, header edit, type recovery, or source-boundary change, the pipeline returns a blocked status instead of inventing declarations.

### 2.6 Model-call budget is staged

The default ten-call budget is reallocated as:

- 3 initial reconstruction attempts
- Up to 3 compile-repair calls
- Up to 2 semantic-repair calls
- Up to 2 MWCC match-improvement calls

Calls stop early when:

- A full match is found
- A sufficiently strong proven-equivalent candidate is found
- The branch repeats the same failure
- The target is blocked by missing context
- Budget or cost limits are reached

### 2.7 Promotion is serialized initially

Model calls and isolated compilation may remain parallel.

Canonical source writes and post-promotion validation must use a single global promotion lock in the first implementation.

A per-translation-unit lock may be introduced later after shared build and state mutation behavior is proven safe.

### 2.8 Validation scope

Every promotion must:

1. Rebuild the owning object or configured minimum unit.
2. Re-evaluate the target against the latest canonical root.
3. Protect already accepted functions in the same object.
4. Run the configured `coop cycle` integration command.
5. Roll back on any failure.

A complete project build is not required for every function unless configured. Full builds should run periodically and at translation-unit closure.

---

## 3. Current-state diagnosis

### 3.1 Evaluation is stronger than generation

The harness can determine whether generated source:

- Parses as a model response
- Applies to a target
- Compiles
- Produces a partial or full code match
- Satisfies size constraints
- Passes PPC equivalence checks
- Preserves a translation unit at closure

However, the initial prompt asks the model to perform too many tasks simultaneously:

- Decode raw PPC words
- Recover instruction boundaries
- Resolve branch targets
- Interpret relocations
- Infer a control-flow graph
- Infer types and members
- Reconstruct semantics
- Produce valid repository-compatible C++
- Shape source for MWCC

A failure at any early task prevents later evaluation from being useful.

### 3.2 Independent calls do not learn from compiler output

Multiple candidates in one experiment currently receive substantially the same task context.

The compiler output generated after one candidate does not guide the next candidate. This is useful for sampling diversity, but inefficient for compile closure.

The desired behavior is:

```text
initial candidate
    ↓
compiler diagnostics
    ↓
minimal compile repair
    ↓
new diagnostics or successful compile
    ↓
structural comparison
    ↓
semantic or MWCC-specific repair
```

### 3.3 Raw bytecode is an inappropriate primary representation

Raw words should remain in experiment artifacts for reproducibility, but the model should primarily receive:

- Decoded instructions
- Addresses
- Branch targets
- Basic block membership
- Relocation symbols
- Control-flow edges
- Known call information

The harness already contains or can reuse much of the required decoding logic through the PPC equivalence tooling.

### 3.4 Compilation and code matching are currently conflated

A compiling candidate can fail to match for two fundamentally different reasons:

#### Semantic mismatch

Examples:

- Missing calls
- Extra calls
- Wrong field offsets
- Wrong access widths
- Missing early return
- Incorrect branch structure
- Wrong constants
- Incorrect observable writes

#### MWCC code-generation mismatch

Examples:

- Correct operations in a different order
- Different temporary lifetimes
- Signed versus unsigned comparison lowering
- Different branch inversion
- Different expression grouping
- Register allocation differences
- Different loop syntax
- Extra copy or extension instructions

These need different prompts, evidence, routing, and stop conditions.

### 3.5 Current automatic promotion needs stronger guarantees

The harness should not auto-promote solely because a generated candidate is the best candidate in a weak batch.

The relevant comparison is not:

> Is this the best generated candidate?

It is:

> Is this candidate demonstrably better than the current canonical implementation when evaluated against the latest root?

---

## 4. Target architecture

### 4.1 High-level workflow

```text
┌──────────────────────┐
│ Capture fresh baseline│
└──────────┬───────────┘
           │
           ▼
┌──────────────────────┐
│ Build target dossier  │
└──────────┬───────────┘
           │
           ▼
┌──────────────────────────────┐
│ Generate 3 reconstructions    │
└──────────┬───────────────────┘
           │
           ▼
┌──────────────────────────────┐
│ Evaluate each candidate       │
└──────────┬───────────────────┘
           │
           ├── compile failure ───────────┐
           │                              ▼
           │                    ┌─────────────────────┐
           │                    │ Compile repair loop │
           │                    └──────────┬──────────┘
           │                               │
           └─────────────── compiles ◄─────┘
                                           │
                                           ▼
                              ┌─────────────────────────┐
                              │ Structural comparison   │
                              └────────────┬────────────┘
                                           │
                          low structure ────┼──── high structure
                                           │
                          ▼                │              ▼
              ┌────────────────────┐       │   ┌────────────────────┐
              │ Semantic repair     │       │   │ MWCC match improve │
              └──────────┬─────────┘       │   └──────────┬─────────┘
                         └─────────────────┴───────────────┘
                                           │
                                           ▼
                              ┌─────────────────────────┐
                              │ Final evaluation/proof  │
                              └────────────┬────────────┘
                                           │
                                           ▼
                              ┌─────────────────────────┐
                              │ Transactional promotion │
                              └─────────────────────────┘
```

### 4.2 Core separation of responsibilities

The system should have explicit components for:

- **Dossier construction:** deterministic program context
- **Candidate generation:** initial semantic reconstruction
- **Compiler diagnosis:** normalized error classification
- **Compile repair:** minimal source corrections
- **Structural comparison:** semantic-shape evidence
- **Semantic repair:** correction of observable behavior
- **Match improvement:** MWCC-specific source shaping
- **Equivalence verification:** proof where supported
- **Candidate ranking:** stage-aware comparison
- **Promotion:** serialized canonical mutation and rollback
- **Knowledge retrieval:** relevant prior evidence
- **Benchmarking:** fixed evaluation corpus

Avoid placing all of this directly in `core.py`. The existing harness should orchestrate these components rather than absorb their implementation details.

---

## 5. Proposed module layout

The exact filenames can be adjusted to repository conventions, but the following split is recommended.

```text
tools/llm_harness/
├── core.py
├── types.py
├── xenoblade_project.py
├── workspace.py
├── pipeline.py
├── dossier.py
├── diagnostics.py
├── structural.py
├── ranking.py
├── promotion.py
├── knowledge.py
├── benchmark.py
├── prompts/
│   ├── common.md
│   ├── reconstruct.md
│   ├── compile_repair.md
│   ├── semantic_repair.md
│   ├── improve.md
│   └── tu_complete.md
└── tests/
    ├── test_pipeline.py
    ├── test_dossier.py
    ├── test_diagnostics.py
    ├── test_structural.py
    ├── test_ranking.py
    ├── test_promotion.py
    ├── test_knowledge.py
    └── fixtures/
```

### 5.1 `pipeline.py`

Owns the state machine and budget allocation.

It should not know Xenoblade-specific build commands. Those remain behind `ProjectAdapter`.

### 5.2 `dossier.py`

Defines typed target-context structures and deterministic serialization.

### 5.3 `diagnostics.py`

Normalizes MWCC and linker diagnostics into stable categories and fingerprints.

### 5.4 `structural.py`

Builds comparable fingerprints from retail and candidate machine code.

### 5.5 `ranking.py`

Defines candidate rank ordering, promotion gates, and stage-specific tie-breaking.

### 5.6 `promotion.py`

Provides global locking, stale-baseline detection, transactional application, revalidation, rollback, and promotion journals.

### 5.7 `knowledge.py`

Retrieves and summarizes target-specific references and prior attempts.

### 5.8 `benchmark.py`

Runs fixed ablations and emits JSON/Markdown reports.

---

## 6. Data model changes

Extend `types.py` rather than passing unstructured dictionaries through the pipeline.

### 6.1 Pipeline stages

```python
from enum import Enum


class PipelineStage(str, Enum):
    BASELINE = "baseline"
    RECONSTRUCT = "reconstruct"
    COMPILE_REPAIR = "compile_repair"
    STRUCTURAL_ANALYSIS = "structural_analysis"
    SEMANTIC_REPAIR = "semantic_repair"
    MATCH_IMPROVE = "match_improve"
    EQUIVALENCE = "equivalence"
    PROMOTION = "promotion"
    COMPLETE = "complete"
    BLOCKED = "blocked"
```

### 6.2 Candidate statuses

Retain existing externally visible statuses where compatibility matters, but add finer internal statuses.

```python
class CandidateStatus(str, Enum):
    MODEL_ERROR = "model_error"
    INVALID_RESPONSE = "invalid_response"
    PATCH_ERROR = "patch_error"
    COMPILE_ERROR = "compile_error"
    BLOCKED = "blocked"
    COMPILES = "compiles"
    STRUCTURALLY_ALIGNED = "structurally_aligned"
    CODE_MATCH = "code_match"
    EQUIVALENT_MATCH = "equivalent_match"
    FULL_MATCH = "full_match"
```

Do not redefine `CODE_MATCH` only by a loose fuzzy threshold if that conflicts with existing semantics. Add explicit fields for static and structural measurements so status remains backward-compatible.

### 6.3 Blocked reasons

```python
class BlockedReason(str, Enum):
    MISSING_DECLARATION = "missing_declaration"
    UNKNOWN_TYPE = "unknown_type"
    UNCERTAIN_SIGNATURE = "uncertain_signature"
    SOURCE_BOUNDARY = "source_boundary"
    UNSUPPORTED_CONTEXT = "unsupported_context"
    REPEATED_DIAGNOSTIC = "repeated_diagnostic"
    UNSUPPORTED_INSTRUCTION = "unsupported_instruction"
    BUDGET_EXHAUSTED = "budget_exhausted"
```

### 6.4 Compiler diagnostics

```python
from dataclasses import dataclass, field


@dataclass(frozen=True)
class CompilerDiagnostic:
    category: str
    severity: str
    message: str
    file: str | None = None
    line: int | None = None
    column: int | None = None
    symbol: str | None = None
    fingerprint: str = ""


@dataclass
class CompileReport:
    succeeded: bool
    exit_code: int
    diagnostics: list[CompilerDiagnostic] = field(default_factory=list)
    raw_output_path: str | None = None
    root_fingerprint: str | None = None
```

The raw compiler output should be stored as a file artifact. State files should keep normalized metadata and a relative path, not duplicate long compiler logs.

### 6.5 Structural report

```python
@dataclass
class StructuralComponent:
    score: float
    matched: int
    expected: int
    details: list[str] = field(default_factory=list)


@dataclass
class StructuralReport:
    total_score: float
    calls: StructuralComponent
    relocations: StructuralComponent
    memory_accesses: StructuralComponent
    cfg: StructuralComponent
    constants: StructuralComponent
    returns: StructuralComponent
    instruction_classes: StructuralComponent
    unexpected_effects: list[str] = field(default_factory=list)
    missing_effects: list[str] = field(default_factory=list)
```

### 6.6 Candidate evaluation

```python
@dataclass
class CandidateEvaluation:
    status: CandidateStatus
    compile_report: CompileReport
    match_percent: float = 0.0
    structural_report: StructuralReport | None = None
    equivalence_status: str | None = None
    function_size: int | None = None
    retail_size: int | None = None
    object_regressions: list[str] = field(default_factory=list)
    accepted_function_regressions: list[str] = field(default_factory=list)
    warnings: list[str] = field(default_factory=list)
```

### 6.7 Candidate lineage

Every repair must retain its parent.

```python
@dataclass
class CandidateLineage:
    candidate_id: str
    parent_candidate_id: str | None
    root_candidate_id: str
    stage: PipelineStage
    generation_index: int
    repair_depth: int
    model_id: str
```

This permits branch-aware analysis:

```text
reconstruct-1
  ├── compile-repair-1
  │     └── semantic-repair-1
  └── compile-repair-2
reconstruct-2
reconstruct-3
```

### 6.8 Baseline snapshot

```python
@dataclass
class BaselineSnapshot:
    target_id: str
    source_text: str
    source_hash: str
    object_hash: str | None
    evaluation: CandidateEvaluation
    captured_at: str
    root_revision: str | None
    dirty_paths: list[str]
```

This baseline is evidence, not a permanent lock. It must be refreshed before promotion.

### 6.9 Promotion result

```python
@dataclass
class PromotionResult:
    promoted: bool
    rolled_back: bool
    reason: str
    baseline_rank: tuple
    candidate_rank: tuple
    source_hash_before: str
    source_hash_after: str | None
    validation_steps: list[dict]
    rollback_error: str | None = None
```

---

## 7. State schema migration

The current experiment state should be migrated to a new schema version while preserving old experiments.

### 7.1 New state shape

```json
{
  "schema_version": 2,
  "experiment_id": "20260717T120000Z-abcd1234",
  "workflow": "new",
  "target_id": "SomeClass__func",
  "status": "running",
  "current_stage": "compile_repair",
  "baseline": {
    "source_hash": "sha256:...",
    "evaluation": {
      "status": "compiles",
      "match_percent": 0.0
    }
  },
  "budget": {
    "initial": 3,
    "compile_repairs": 3,
    "semantic_repairs": 2,
    "match_improvements": 2,
    "used": {
      "initial": 3,
      "compile_repairs": 1,
      "semantic_repairs": 0,
      "match_improvements": 0
    }
  },
  "candidates": [],
  "frontier": [],
  "promotion": null,
  "context": {
    "dossier_schema_version": 1,
    "context_dir": "build/llm-harness/.../context"
  }
}
```

### 7.2 Compatibility strategy

Implement:

```python
def load_experiment_state(path: Path) -> ExperimentState:
    raw = json.loads(path.read_text())

    version = int(raw.get("schema_version", 1))
    if version == 1:
        raw = migrate_v1_to_v2(raw)
    elif version != 2:
        raise UnsupportedStateVersion(version)

    return ExperimentState.from_json(raw)
```

Do not rewrite historical state files automatically on read. Migration may occur in memory and only be persisted when an experiment is resumed.

### 7.3 Append-only evidence

Candidate artifacts and promotion journals should remain append-only.

Canonical summaries may point to the newest record, but old candidate source, prompts, compiler logs, and evaluations must not be overwritten.

---

## 8. Phase 0: promotion safety

This is the first implementation phase because promotion mutates canonical source.

## 8.1 Capture and evaluate a baseline

Before any model call:

1. Read the current canonical target source.
2. Hash it.
3. Build/evaluate it using the same path used for generated candidates.
4. Store its result in the experiment state.
5. Include it as a ranking candidate.

Pseudo-code:

```python
def capture_baseline(
    adapter: ProjectAdapter,
    target_id: str,
    experiment_dir: Path,
) -> BaselineSnapshot:
    source = adapter.read_target_source(target_id)
    source_hash = sha256_text(source)

    evaluation = adapter.evaluate_source(
        target_id=target_id,
        source=source,
        workspace_root=adapter.root,
        artifact_dir=experiment_dir / "baseline",
    )

    return BaselineSnapshot(
        target_id=target_id,
        source_text=source,
        source_hash=source_hash,
        object_hash=evaluation.object_hash,
        evaluation=evaluation,
        captured_at=utc_now(),
        root_revision=git_head_or_none(adapter.root),
        dirty_paths=git_dirty_paths(adapter.root),
    )
```

If evaluating canonical source requires no patch, add a dedicated adapter method rather than fabricating a candidate patch.

## 8.2 Separate experiment winner from promotion winner

```python
generated_winner = max(
    generated_candidates,
    key=rank_candidate,
)

decision = compare_for_promotion(
    baseline=baseline.evaluation,
    candidate=generated_winner.evaluation,
    policy=config.promotion,
)
```

The generated winner must always be written to experiment evidence even if it is not promoted.

## 8.3 Candidate ranking

Use lexicographic ranking for hard priorities and weighted scoring only within comparable tiers.

Recommended rank:

```python
def rank_candidate(e: CandidateEvaluation) -> tuple:
    return (
        int(not e.object_regressions),
        semantic_tier(e),
        structural_bucket(e),
        round(e.match_percent, 4),
        int(size_is_acceptable(e)),
        -positive_size_delta(e),
        -warning_count(e),
    )
```

Suggested semantic tier:

```python
def semantic_tier(e: CandidateEvaluation) -> int:
    if e.status == CandidateStatus.FULL_MATCH:
        return 5
    if e.status == CandidateStatus.EQUIVALENT_MATCH:
        return 4
    if (
        e.compile_report.succeeded
        and e.structural_report is not None
        and e.structural_report.total_score >= 0.75
    ):
        return 3
    if e.compile_report.succeeded:
        return 2
    if e.status == CandidateStatus.BLOCKED:
        return 1
    return 0
```

Do not make fuzzy match percentage outrank a sound equivalence proof.

## 8.4 Promotion gate

```python
@dataclass(frozen=True)
class PromotionPolicy:
    enabled: bool = True
    require_monotonic_rank: bool = True
    allow_first_compile: bool = True
    first_compile_min_structural_score: float = 0.60
    structural_alignment_threshold: float = 0.75
    protect_accepted_functions: bool = True
    revalidate_against_latest_root: bool = True
    rollback_on_failure: bool = True
```

```python
def passes_promotion_gate(
    baseline: CandidateEvaluation,
    candidate: CandidateEvaluation,
    policy: PromotionPolicy,
    baseline_is_placeholder: bool,
) -> tuple[bool, str]:
    if not policy.enabled:
        return False, "automatic promotion is disabled"

    if candidate.object_regressions:
        return False, "candidate has object regressions"

    if candidate.accepted_function_regressions:
        return False, "candidate regresses accepted functions"

    candidate_rank = rank_candidate(candidate)
    baseline_rank = rank_candidate(baseline)

    if candidate.status == CandidateStatus.FULL_MATCH:
        return True, "full match"

    if candidate.status == CandidateStatus.EQUIVALENT_MATCH:
        if candidate_rank > baseline_rank:
            return True, "proved equivalent and rank improved"
        return False, "equivalent candidate did not improve canonical rank"

    if candidate.compile_report.succeeded and not baseline.compile_report.succeeded:
        score = structural_score(candidate)
        if (
            policy.allow_first_compile
            and score >= policy.first_compile_min_structural_score
        ):
            return True, "first compiling structurally plausible implementation"

    if baseline_is_placeholder and candidate.compile_report.succeeded:
        score = structural_score(candidate)
        if score >= policy.first_compile_min_structural_score:
            return True, "replaces placeholder with plausible compiling implementation"

    if policy.require_monotonic_rank and candidate_rank <= baseline_rank:
        return False, "candidate does not improve canonical rank"

    return True, "candidate improves canonical rank"
```

## 8.5 Placeholder detection

Do not rely only on zero match.

Support configured and heuristic detection:

```python
class PlaceholderDetector:
    def is_placeholder(self, source: str, metadata: dict) -> bool:
        if metadata.get("placeholder") is True:
            return True

        normalized = remove_comments_and_whitespace(source)

        known_patterns = (
            "return 0;",
            "return false;",
            "{}",
            "abort();",
            "UNIMPLEMENTED",
        )
        return any(pattern in normalized for pattern in known_patterns)
```

Prefer explicit target metadata over heuristic detection.

## 8.6 Promotion lock

Add a dedicated lock:

```python
class Harness:
    def __init__(self, ...):
        ...
        self._adapter_lock = threading.RLock()
        self._promotion_lock = threading.Lock()
```

Do not reuse `_adapter_lock` without carefully auditing its lock ordering. A separate lock makes canonical mutation obvious.

All canonical mutation must occur inside:

```python
with self._promotion_lock:
    result = self.promotion_manager.promote(...)
```

## 8.7 Revalidate against the latest root

A candidate built in an isolated worktree may be stale after other promotions.

Inside the promotion lock:

1. Re-read canonical source.
2. Re-hash target and owning file.
3. Capture a fresh baseline if the source changed.
4. Apply the candidate to a fresh temporary worktree based on the latest root.
5. Re-evaluate it.
6. Re-run the promotion comparison.
7. Only then write to canonical source.

```python
def prepare_latest_candidate(
    target_id: str,
    candidate_patch: SourcePatch,
) -> PreparedPromotion:
    latest_baseline = capture_baseline(...)
    workspace = workspace_manager.create_from_current_root(...)

    adapter.apply_patch(workspace, candidate_patch)
    latest_evaluation = adapter.evaluate(workspace, target_id)

    return PreparedPromotion(
        baseline=latest_baseline,
        candidate_evaluation=latest_evaluation,
        workspace=workspace,
    )
```

## 8.8 Transactional write and rollback

Before mutation:

- Save original file bytes.
- Save mode bits if relevant.
- Record current hash.
- Record any generated state files that promotion changes.
- Record affected object artifact paths if the build overwrites shared output.

After mutation:

1. Build the owning object.
2. Re-evaluate the target.
3. Verify protected functions.
4. Run `coop cycle`.
5. Confirm success exit codes.
6. Confirm source still contains the expected candidate.
7. Write promotion journal.

On failure:

- Restore original file atomically.
- Rebuild or clean affected object output if required.
- Re-run a minimal baseline validation.
- Record rollback.
- Surface rollback failures prominently.

```python
@contextmanager
def source_transaction(path: Path):
    original = path.read_bytes()
    original_mode = path.stat().st_mode

    try:
        yield
    except BaseException:
        atomic_write_bytes(path, original)
        os.chmod(path, stat.S_IMODE(original_mode))
        raise
```

Use temporary files plus `os.replace` for atomic source writes.

## 8.9 Promotion validation steps

Represent every step explicitly:

```python
@dataclass
class ValidationStepResult:
    name: str
    succeeded: bool
    exit_code: int | None
    detail: str
    artifact_paths: list[str]
```

Default steps:

```text
1. source_hash_precondition
2. apply_candidate
3. compile_owning_object
4. evaluate_target
5. verify_target_rank
6. verify_protected_functions
7. verify_object_size/layout constraints
8. coop_cycle
9. source_hash_postcondition
10. commit_promotion_journal
```

## 8.10 Promotion journal

Write to:

```text
build/llm-harness/promotions/YYYY-MM-DD/<promotion-id>.json
```

Example:

```json
{
  "schema_version": 1,
  "promotion_id": "prom-20260717T120000Z-abcd1234",
  "target_id": "SomeClass__func",
  "experiment_id": "20260717T115800Z-1234abcd",
  "candidate_id": "candidate-07",
  "source_file": "src/...",
  "baseline_source_hash": "sha256:...",
  "candidate_source_hash": "sha256:...",
  "baseline_rank": [1, 2, 31.4, 1, 0, 0],
  "candidate_rank": [1, 3, 72.0, 1, 0, 0],
  "reason": "structural alignment and match improved",
  "validation_steps": [],
  "promoted": true,
  "rolled_back": false,
  "timestamp": "2026-07-17T12:00:00Z"
}
```

---

## 9. Phase 1: target dossier

The dossier is the most important candidate-quality change.

## 9.1 Dossier schema

```python
@dataclass
class TargetDossier:
    schema_version: int
    target: TargetIdentity
    signature: SignatureContext
    retail: RetailProgramContext
    source: SourceContext
    calls: CallContext
    types: TypeContext
    symbols: SymbolInventory
    accepted_examples: list[AcceptedExample]
    knowledge: list[KnowledgeRecord]
    prior_attempt_summary: list[AttemptCluster]
    constraints: PromptConstraints
```

Serialize it to both:

- `dossier.json` for machine consumption
- `DOSSIER.md` for model readability and debugging

## 9.2 Target identity

```json
{
  "target_id": "SomeClass__func",
  "mangled_name": "...",
  "demangled_name": "SomeClass::func(int)",
  "source_file": "src/...",
  "object_file": "build/...",
  "translation_unit": "...",
  "retail_address": "0x80123450",
  "retail_size": 96
}
```

## 9.3 Exact signature

Include:

- Return type
- Function name
- Parameter names where known
- Parameter types
- Qualifiers
- Namespace/class scope
- Calling convention assumptions
- Whether the signature is authoritative

Example:

```json
{
  "declaration": "bool SomeClass::func(const Item& item, int index)",
  "authoritative": true,
  "implicit_this": {
    "type": "SomeClass*",
    "register": "r3"
  },
  "parameters": [
    {"name": "item", "type": "const Item&", "register": "r4"},
    {"name": "index", "type": "int", "register": "r5"}
  ],
  "return": {
    "type": "bool",
    "register": "r3"
  }
}
```

## 9.4 Decoded retail instructions

Use a stable format:

```json
{
  "address": "0x80123450",
  "offset": 0,
  "raw_word": "9421fff0",
  "mnemonic": "stwu",
  "operands": "r1, -0x10(r1)",
  "instruction_class": "store",
  "basic_block": "B0",
  "branch_target": null,
  "relocation": null
}
```

For relocated calls:

```json
{
  "address": "0x80123468",
  "offset": 24,
  "raw_word": "48000001",
  "mnemonic": "bl",
  "operands": "SomeNamespace::callee",
  "instruction_class": "call",
  "basic_block": "B1",
  "branch_target": null,
  "relocation": {
    "type": "R_PPC_REL24",
    "symbol": "SomeNamespace::callee",
    "addend": 0
  }
}
```

## 9.5 Control-flow graph

At minimum include:

- Entry block
- Basic blocks
- Instruction ranges
- Successors
- Predecessors
- Branch condition when recognized
- Exit blocks
- Backedges
- Loop headers
- Unreachable regions if any

```json
{
  "entry": "B0",
  "blocks": [
    {
      "id": "B0",
      "start": "0x80123450",
      "end": "0x80123464",
      "successors": ["B1", "B3"],
      "predecessors": [],
      "terminator": {
        "kind": "conditional",
        "condition_register": "cr0",
        "taken": "B3",
        "fallthrough": "B1"
      }
    }
  ],
  "exit_blocks": ["B3"],
  "backedges": []
}
```

## 9.6 Data-flow summary

Initial implementation can be conservative.

Include:

- Likely argument register use
- Return register writes
- Stack frame size
- Saved/restored registers
- Observed loads and stores
- Referenced offsets
- Direct calls
- Constants
- Global symbols
- Condition register usage

Do not claim high-level semantics not proven by deterministic analysis.

```json
{
  "stack_frame_size": 16,
  "saved_registers": ["lr", "r31"],
  "memory_accesses": [
    {
      "kind": "load",
      "width_bits": 8,
      "base": "r31",
      "offset": 33,
      "likely_expression": "this+0x21"
    }
  ],
  "constants": ["0x20", "0xFF"],
  "calls": ["SomeNamespace::callee"]
}
```

## 9.7 Relevant declarations

Build a bounded declaration extractor.

Start from:

- Target function declaration
- Containing class declaration
- Types appearing in target signature
- Types appearing in direct callee declarations
- Symbols referenced by relocations
- Types used in selected caller snippets
- Explicit project metadata hints

Cap recursively included declarations by:

- Depth
- Character count
- Number of types
- Relevance

Avoid inserting implementation-heavy headers wholesale.

## 9.8 Caller snippets

Select two or three representative callsites.

Prioritize:

1. Callers in the same subsystem
2. Callers with clearly typed arguments
3. Callers that inspect the return value
4. Accepted callers
5. Small snippets with minimal unrelated code

Each snippet should contain enough surrounding source to show argument meaning without exposing an entire file.

## 9.9 Accepted sibling functions

Wire the existing accepted-context helper into dossier construction.

Each example should include:

```json
{
  "target_id": "...",
  "demangled_name": "...",
  "relationship": "same_class",
  "reason_selected": [
    "same class",
    "similar call relocation",
    "same field offsets"
  ],
  "source": "...",
  "retail_summary": {
    "size": 64,
    "calls": ["..."],
    "cfg_shape": "single conditional"
  }
}
```

Default maximum: four.

## 9.10 Allowed symbol inventory

The prompt should distinguish known symbols from model guesses.

```json
{
  "members": [
    {
      "name": "mState",
      "type": "u8",
      "offset": 33,
      "source": "class declaration"
    }
  ],
  "functions": [
    {
      "name": "setState",
      "declaration": "void setState(State state)",
      "source": "header"
    }
  ],
  "globals": [],
  "enums": []
}
```

Prompt rule:

> Use only symbols and declarations in this dossier. If the retail behavior cannot be represented using the available declarations, return a blocked reason rather than inventing a member or function.

## 9.11 Context size limits

Suggested defaults:

```json
{
  "dossier": {
    "max_decoded_instructions": 400,
    "max_declaration_chars": 20000,
    "max_callers": 3,
    "max_caller_chars_each": 3000,
    "max_accepted_siblings": 4,
    "max_accepted_chars_each": 5000,
    "max_knowledge_records": 10,
    "max_attempt_clusters": 8,
    "include_raw_hex": true
  }
}
```

Large functions above instruction limits should be explicitly marked truncated and may require a different workflow.

## 9.12 Dossier validation

Before any model call:

```python
def validate_dossier(d: TargetDossier) -> list[str]:
    errors = []

    if not d.signature.declaration:
        errors.append("missing target declaration")

    if not d.retail.instructions:
        errors.append("missing decoded retail instructions")

    if d.signature.authoritative is False:
        errors.append("signature is not authoritative")

    unresolved_call_symbols = [
        call.symbol
        for call in d.retail.calls
        if call.symbol not in d.symbols.functions
    ]

    if unresolved_call_symbols:
        d.warnings.append(
            f"unresolved call declarations: {unresolved_call_symbols}"
        )

    return errors
```

Fatal dossier errors should block generation rather than spend model budget.

---

## 10. Phase 2: staged generation and compile closure

## 10.1 Initial reconstruction strategies

Run three candidates in parallel with explicit strategy labels.

### Strategy A: literal operations

Prompt emphasis:

- Preserve observed calls
- Preserve memory offsets and widths
- Preserve branch count and return paths
- Prefer direct, simple C++
- Avoid creative abstraction

### Strategy B: type-aware idiomatic source

Prompt emphasis:

- Use known members and enums
- Follow accepted sibling conventions
- Use repository-compatible types
- Avoid casts unless required by observed widths

### Strategy C: alternative control-flow hypothesis

Prompt emphasis:

- Explore a distinct but evidence-supported high-level CFG
- Consider early return versus nested branch
- Consider loop forms only if backedges exist
- Do not change observable effects

Record the strategy in candidate lineage.

## 10.2 Reconstruction response schema

```json
{
  "response_schema_version": 2,
  "stage": "reconstruct",
  "source": "bool SomeClass::func(...) { ... }",
  "semantic_summary": {
    "calls": ["SomeNamespace::callee"],
    "memory_reads": [
      {"base": "this", "offset": 33, "width_bits": 8}
    ],
    "memory_writes": [],
    "return_paths": [
      {"condition": "mState == 0", "value": "false"},
      {"condition": "otherwise", "value": "result"}
    ],
    "constants": [0]
  },
  "assumptions": [
    "mState corresponds to the byte read at this+0x21"
  ],
  "blocked_on": [],
  "hypothesis": "The function checks state before delegating to callee.",
  "confidence": 0.72
}
```

The parser must reject:

- Missing source
- Multiple unrelated functions
- Signature changes
- Markdown outside the allowed JSON envelope, if strict mode is enabled
- Unsupported external file edits

## 10.3 Compile diagnostic normalization

Compiler output changes across path names and generated worktrees. Normalize it before fingerprinting.

Normalization steps:

1. Replace workspace roots with `<WORKSPACE>`.
2. Replace experiment IDs with `<EXPERIMENT>`.
3. Normalize slash direction.
4. Remove caret lines and repeated context when unnecessary.
5. Preserve compiler error code if present.
6. Extract file, line, column, symbol, and main message.
7. Remove unstable numeric addresses unless relevant.

Example:

```text
/tmp/worktree-123/src/foo.cpp:91: error: 'mFoo' was not declared
```

becomes:

```json
{
  "category": "unknown_identifier",
  "file": "src/foo.cpp",
  "line": 91,
  "symbol": "mFoo",
  "message": "'mFoo' was not declared",
  "fingerprint": "unknown_identifier:mFoo"
}
```

## 10.4 Diagnostic categories

Implement ordered matching:

```python
DIAGNOSTIC_RULES = [
    DiagnosticRule(
        category="syntax_error",
        patterns=[r"parse error", r"expected .+ before"],
    ),
    DiagnosticRule(
        category="unknown_identifier",
        patterns=[r"'(?P<symbol>[^']+)' was not declared"],
    ),
    DiagnosticRule(
        category="unknown_member",
        patterns=[r"has no member named '(?P<symbol>[^']+)'"],
    ),
    DiagnosticRule(
        category="argument_count",
        patterns=[r"too (?:few|many) arguments"],
    ),
    DiagnosticRule(
        category="invalid_conversion",
        patterns=[r"cannot convert", r"invalid conversion"],
    ),
    DiagnosticRule(
        category="incomplete_type",
        patterns=[r"incomplete type"],
    ),
    DiagnosticRule(
        category="access_control",
        patterns=[r"is private", r"is protected"],
    ),
    DiagnosticRule(
        category="linkage_error",
        patterns=[r"undefined reference", r"unresolved external"],
    ),
]
```

Add MWCC-specific fixture samples gathered from real experiments.

## 10.5 Root diagnostic

The repair prompt should focus on the first causal diagnostic, not every cascading error.

Heuristics:

- Prefer first syntax error before later parse cascades.
- Prefer unknown type before member errors caused by that type.
- Prefer declaration mismatch before argument conversion cascades.
- Prefer compiler errors over linker errors when both exist.
- Group duplicate messages.

## 10.6 Compile-repair prompt

Create `prompts/compile_repair.md`.

Required content:

```markdown
You are repairing one candidate function so that it compiles.

Preserve:
- The exact target signature
- Intended calls
- Memory access widths and offsets
- Constants
- Branch and return behavior

Change only what is required by the compiler diagnostics.

Do not:
- Perform instruction matching yet
- Invent members, functions, types, or globals
- Modify headers or unrelated functions
- Remove behavior merely to silence an error

When the necessary declaration is unavailable, return an appropriate
blocked reason instead of fabricating code.
```

Input sections:

- Dossier
- Candidate source
- Compiler diagnostic report
- Parent semantic summary
- Prior repair fingerprints
- Allowed symbol inventory

## 10.7 Repair-loop algorithm

```python
def run_compile_repairs(
    branch: CandidateBranch,
    budget: int,
) -> CandidateBranch:
    seen_fingerprints: set[str] = set()

    for repair_index in range(1, budget + 1):
        report = branch.latest.evaluation.compile_report

        if report.succeeded:
            return branch

        fingerprint = report.root_fingerprint
        if fingerprint in seen_fingerprints:
            branch.block(
                reason=BlockedReason.REPEATED_DIAGNOSTIC,
                detail=fingerprint,
            )
            return branch

        seen_fingerprints.add(fingerprint)

        response = call_model(
            stage=PipelineStage.COMPILE_REPAIR,
            dossier=branch.dossier,
            parent=branch.latest,
            diagnostic_report=report,
            repair_index=repair_index,
        )

        candidate = evaluate_response(response)
        branch.append(candidate)

    return branch
```

A repeated fingerprint should normally stop that branch, but not necessarily the entire target if another branch remains.

## 10.8 Selecting branches for repair

After initial evaluation, rank compile failures by repairability.

Suggested tuple:

```python
def compile_repair_priority(candidate: CandidateRecord) -> tuple:
    report = candidate.evaluation.compile_report
    return (
        int(candidate.response_parsed),
        int(candidate.signature_preserved),
        repairability_tier(report),
        -len(report.diagnostics),
        int(report.root_fingerprint not in candidate.ancestor_fingerprints),
        candidate.model_confidence,
    )
```

Repair the best branch first. If it becomes blocked and budget remains, repair the second-best branch.

## 10.9 Early compile success

When one branch compiles:

- Do not automatically cancel already-running initial calls; preserve their evidence.
- Do not spend additional compile-repair budget on that branch.
- Continue to structural analysis.
- Consider another compiling initial branch if it has a better preliminary match.

---

## 11. Phase 3: structural comparison

## 11.1 Objective

Structural comparison estimates whether candidate and retail machine code represent similar observable behavior.

It is not a proof and must not be described as one.

It exists to:

- Route low-structure candidates to semantic repair
- Route high-structure candidates to MWCC match improvement
- Prevent accidental prologue/epilogue similarity from dominating
- Gate first-compile auto-promotion
- Provide precise feedback

## 11.2 Fingerprint inputs

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

## 11.3 Calls and relocations

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

## 11.4 Memory accesses

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

## 11.5 CFG shape

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

## 11.6 Constants

Ignore routine prologue constants such as stack offsets when scoring semantic constants.

Classify:

- Immediate compare values
- Bit masks
- Shift amounts
- Field offsets
- Enum-like constants
- Floating-point constants
- Address addends

## 11.7 Return paths

Capture:

- Return register assignments
- Distinct constant returns
- Number of exit paths
- Whether a returned call value is passed through
- Whether return is conditional

## 11.8 Instruction classes

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

## 11.9 Initial weights

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

## 11.10 Penalties

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

## 11.11 Routing thresholds

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

## 11.12 Structural report example

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

## 12. Phase 4: semantic repair

## 12.1 When semantic repair runs

Run semantic repair when:

- Candidate compiles, and
- Structural score is below the configured alignment threshold, and
- The candidate is not blocked, and
- Semantic-repair budget remains.

## 12.2 Prompt objective

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

## 12.3 Evidence supplied

- Current source
- Current semantic summary
- Structural component scores
- Concrete missing/extra observations
- Retail decoded assembly
- Candidate decoded assembly
- Relevant declarations
- Parent lineage
- Previously attempted semantic changes

## 12.4 One-discrepancy rule

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

## 12.5 Semantic repair loop

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

## 12.6 Semantic frontier

Track the best candidate independently for:

- Call agreement
- Memory-effect agreement
- CFG agreement
- Overall structural score
- Static match
- Proof status

This prevents loss of a candidate that is strong in one semantic dimension.

---

## 13. Phase 5: MWCC match improvement

## 13.1 Entry condition

Only run MWCC match improvement when:

- Candidate compiles
- Structural score is at or above the threshold
- Candidate is not already a full match
- Budget remains

## 13.2 Preserve current improve guidance

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

## 13.3 Normalize objdiff feedback

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

## 13.4 Improvement response

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

## 13.5 Non-regression requirement

A match-improvement candidate must not reduce structural score beyond a small tolerance.

Suggested:

```python
candidate.structural_score + 0.02 >= parent.structural_score
```

Any larger drop routes back to semantic analysis or rejects the candidate.

---

## 14. Phase 6: equivalence integration

## 14.1 Separation of proof and similarity

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

## 14.2 Proof invocation

Invoke the PPC equivalence checker when:

- Both functions compile to supported analyzable code
- Required callee certificates exist, or compositional assumptions are allowed
- Structural score is sufficiently high, unless testing a deliberate proof-only path
- Budget permits

## 14.3 Inconclusive behavior

The verifier remains fail-closed:

- Unsupported instruction → inconclusive/unsupported
- Timeout → inconclusive
- Unknown external effect → inconclusive
- Missing certificate → inconclusive
- Solver `unknown` → inconclusive

None of these may promote as `EQUIVALENT_MATCH`.

## 14.4 Counterexample feedback

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

## 14.5 Proof-aware ranking

A proven-equivalent candidate outranks an unproven candidate even if its fuzzy match is somewhat lower.

Among two proven-equivalent candidates:

1. Higher static match
2. Better whole-object integration
3. Smaller non-retail size growth
4. Better source-quality score
5. Lower complexity

---

## 15. Phase 7: knowledge retrieval

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

---

## 16. Orchestration implementation

## 16.1 `PipelineRunner`

```python
class PipelineRunner:
    def __init__(
        self,
        adapter: ProjectAdapter,
        model_runner: ModelRunner,
        promotion_manager: PromotionManager,
        config: PipelineConfig,
    ):
        self.adapter = adapter
        self.model_runner = model_runner
        self.promotion_manager = promotion_manager
        self.config = config

    def run(self, target_id: str, experiment: ExperimentState) -> ExperimentState:
        experiment.baseline = self.capture_baseline(target_id)
        experiment.dossier = self.build_dossier(target_id)

        initial = self.generate_initial_candidates(experiment)
        self.evaluate_candidates(initial)

        compile_frontier = self.select_compile_frontier(initial)
        compiled = self.close_compilation(compile_frontier, experiment)

        structural_frontier = self.analyze_structure(compiled)

        semantically_repaired = self.repair_semantics(
            structural_frontier,
            experiment,
        )

        match_frontier = self.improve_match(
            semantically_repaired,
            experiment,
        )

        final = self.final_evaluation(match_frontier)
        winner = self.select_generated_winner(final)

        experiment.promotion = self.promotion_manager.try_promote(
            target_id=target_id,
            experiment=experiment,
            candidate=winner,
        )

        experiment.status = "complete"
        return experiment
```

## 16.2 Resume behavior

State must be persisted after:

- Baseline capture
- Dossier generation
- Every model response
- Every candidate evaluation
- Every stage transition
- Promotion preparation
- Promotion completion or rollback

On resume:

- Do not regenerate completed initial calls.
- Do not repeat a repair whose response and evaluation are complete.
- Re-evaluate stale candidates only when necessary.
- Always recapture baseline before promotion.
- Preserve cost/token accounting.

## 16.3 Parallelism

Safe parallel operations:

- Initial model calls
- Candidate evaluation in isolated worktrees
- Structural analysis of immutable artifacts
- Knowledge retrieval
- Benchmarks across independent repositories/worktrees

Serialized operations:

- Canonical source mutation
- Shared target state mutation
- `coop cycle` if it mutates shared files
- Promotion journal finalization if a single ordered log is required

## 16.4 Cancellation

When a full match appears:

- Stop submitting new model calls.
- Cancel futures not yet started.
- Drain already-running futures.
- Persist all completed evidence.
- Select full match.
- Enter transactional promotion.

When an equivalent candidate appears, optionally stop lower-value stages according to configuration:

```json
{
  "stop_on": {
    "full_match": true,
    "proved_equivalent": true
  }
}
```

If exact matching remains a project priority even after proof, set `proved_equivalent` to false.

---

## 17. Configuration changes

Suggested `llm-harness.json` additions:

```json
{
  "pipeline": {
    "budgets": {
      "reconstruct": 3,
      "compile_repair": 3,
      "semantic_repair": 2,
      "match_improve": 2
    },
    "stop_on": {
      "full_match": true,
      "proved_equivalent": true
    }
  },
  "dossier": {
    "schema_version": 1,
    "max_decoded_instructions": 400,
    "max_declaration_chars": 20000,
    "max_callers": 3,
    "max_caller_chars_each": 3000,
    "max_accepted_siblings": 4,
    "max_accepted_chars_each": 5000,
    "max_knowledge_records": 10,
    "max_attempt_clusters": 8,
    "include_raw_hex": true
  },
  "structural": {
    "weights": {
      "calls": 0.20,
      "relocations": 0.10,
      "memory_accesses": 0.25,
      "cfg": 0.20,
      "constants": 0.10,
      "returns": 0.10,
      "instruction_classes": 0.05
    },
    "semantic_repair_below": 0.75,
    "first_compile_promotion_min": 0.60,
    "max_structural_regression_for_match_improve": 0.02
  },
  "promotion": {
    "enabled": true,
    "lock_scope": "global",
    "require_monotonic_rank": true,
    "allow_first_compile": true,
    "first_compile_min_structural_score": 0.60,
    "protect_accepted_functions": true,
    "revalidate_against_latest_root": true,
    "rollback_on_failure": true,
    "run_coop_cycle": true
  },
  "knowledge": {
    "mode": "retrieved",
    "max_records": 10,
    "cluster_attempts": true
  }
}
```

Validate:

- Weights sum to 1.0.
- Budgets are nonnegative.
- Thresholds are in `[0, 1]`.
- `lock_scope` supports known values.
- Promotion cannot enable first-compile promotion without structural analysis.
- Prompt files exist for enabled stages.

---

## 18. Adapter interface changes

Add methods to `ProjectAdapter` with default fallbacks where possible.

```python
class ProjectAdapter(Protocol):
    def read_target_source(self, target_id: str) -> str:
        ...

    def build_dossier(
        self,
        target_id: str,
        history: list[ExperimentRecord],
        options: dict,
    ) -> TargetDossier:
        ...

    def apply_candidate(
        self,
        root: Path,
        target_id: str,
        candidate: Candidate,
    ) -> SourcePatch:
        ...

    def evaluate_candidate(
        self,
        root: Path,
        target_id: str,
        candidate: Candidate,
        artifact_dir: Path,
    ) -> CandidateEvaluation:
        ...

    def decode_function(
        self,
        root: Path,
        target_id: str,
        side: str,
    ) -> DecodedFunction:
        ...

    def detect_placeholder(
        self,
        target_id: str,
        source: str,
    ) -> bool:
        ...

    def protected_functions(
        self,
        target_id: str,
    ) -> list[str]:
        ...

    def run_promotion_validation(
        self,
        target_id: str,
        artifact_dir: Path,
    ) -> list[ValidationStepResult]:
        ...
```

Where backward compatibility is needed, wrap existing `build_prompt`, `evaluate`, and `promote` methods during migration.

---

## 19. Prompt templates

## 19.1 Common prompt

Keep common instructions short and stable.

Include:

- Exact source-output contract
- No repository browsing
- No invented declarations
- Preserve authoritative signature
- Evidence terminology
- Blocked behavior
- JSON response schema

Do not include all prior attempts.

## 19.2 Reconstruction prompt skeleton

```markdown
# Task

Reconstruct the target function from the supplied retail program evidence.

## Priority

1. Produce valid repository-compatible C++.
2. Preserve observed calls, memory effects, constants, branches, and returns.
3. Use only supplied symbols and declarations.
4. Prefer simple source that directly explains the assembly.
5. Do not perform fine-grained MWCC matching yet.

## Required response

Return one JSON object matching the response schema.

## Target dossier

{{ dossier }}
```

## 19.3 Compile repair prompt skeleton

```markdown
# Task

Repair the candidate so it compiles.

Make the smallest necessary source change.

Do not alter behavior merely to silence the compiler.
Do not invent declarations.
Do not perform instruction matching.

## Candidate

{{ candidate_source }}

## Compiler diagnostics

{{ compile_report }}

## Allowed declarations and symbols

{{ allowed_symbols }}
```

## 19.4 Semantic repair prompt skeleton

```markdown
# Task

The candidate compiles but differs materially from retail behavior.

Correct the single largest discrepancy in the structural report.

Do not optimize exact register allocation or instruction ordering yet.

## Candidate

{{ candidate_source }}

## Structural differences

{{ structural_report }}

## Retail and candidate instruction windows

{{ comparison }}
```

## 19.5 Match improve prompt skeleton

```markdown
# Task

The candidate is structurally aligned with retail.

Make one bounded source-level change intended to improve MWCC output.

Preserve semantic structure.

## Current metrics

{{ metrics }}

## First meaningful codegen difference

{{ normalized_objdiff }}

## Candidate source

{{ candidate_source }}
```

---

## 20. Testing plan

Tests are mandatory for each phase.

## 20.1 Promotion unit tests

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

## 20.2 Dossier tests

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

## 20.3 Diagnostic tests

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

## 20.4 Pipeline tests

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

## 20.5 Structural tests

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

## 20.6 Equivalence integration tests

1. Proven equivalence sets semantic proof status.
2. Counterexample sets disproved status.
3. Timeout sets inconclusive status.
4. Unsupported instruction sets unsupported status.
5. Missing callee certificate remains inconclusive.
6. Inconclusive proof cannot promote as equivalent.
7. Counterexample summary is persisted.
8. Proof outranks fuzzy match in ranking.

## 20.7 Knowledge tests

1. Exact target records rank first.
2. Same-class records outrank unrelated recent records.
3. Record count cap is enforced.
4. Attempt clustering groups equivalent signedness hypotheses.
5. Conflicting lessons remain separate.
6. Proven lessons outrank single-attempt speculation.
7. Prompt excludes full unrelated history.
8. Retrieval is deterministic.

## 20.8 Integration tests

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

## 21.1 Dataset

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

## 21.2 Hidden-source setup

For each benchmark target:

- Replace accepted source with its pre-decomp placeholder in an isolated benchmark branch.
- Preserve headers and surrounding project state.
- Remove exact-target attempt history.
- Prevent retrieval of the accepted implementation.
- Verify the source does not leak through caller snippets or duplicated functions.

## 21.3 Ablations

Run at least:

1. Raw hex, independent sampling
2. Decoded assembly, independent sampling
3. Full dossier, independent sampling
4. Full dossier, staged repair
5. Full dossier and staged repair with retrieved knowledge
6. Full dossier and staged repair without sibling bodies
7. Structural routing disabled
8. Equivalence-aware ranking disabled

## 21.4 Metrics

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

## 21.5 Promotion survival rate

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

## 21.6 Acceptance targets

Initial success criteria relative to current baseline:

- At least 2× compile-within-budget rate
- At least 50% reduction in repeated identical compiler failures
- At least 2× nonzero-match rate among compiled candidates
- Zero known stale-root promotion corruption
- Zero unrolled-back failed promotions
- At least 90% promotion survival in benchmark fixtures
- No reduction in full-match discovery rate

These values should be calibrated after recording the current baseline.

---

## 22. Delivery plan by pull request

## PR 1: Baseline-aware transactional promotion

### Scope

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

### Main files

- `tools/llm_harness/core.py`
- `tools/llm_harness/types.py`
- `tools/llm_harness/promotion.py`
- `tools/llm_harness/ranking.py`
- `tools/llm_harness/xenoblade_project.py`
- `tools/llm_harness/tests/test_promotion.py`

### Acceptance criteria

- Existing auto-promotion remains enabled.
- Worse candidates cannot replace a better baseline.
- Concurrent promotions cannot mutate canonical source simultaneously.
- Any failed validation restores exact original source bytes.
- Promotion result is persisted.

---

## PR 2: Typed dossier and deterministic assembly context

### Scope

- Target dossier dataclasses
- Decoded retail assembly
- CFG
- Relocation annotations
- Existing accepted sibling source integration
- Signature and declaration extraction
- Caller snippets
- Allowed-symbol inventory
- Dossier JSON and Markdown artifacts

### Main files

- `tools/llm_harness/dossier.py`
- `tools/llm_harness/types.py`
- `tools/llm_harness/xenoblade_project.py`
- `tools/llm_harness/prompts/common.md`
- `tools/llm_harness/prompts/reconstruct.md`
- `tools/llm_harness/tests/test_dossier.py`

### Acceptance criteria

- The model no longer needs to decode raw hex manually.
- Every generation prompt has an exact authoritative signature.
- Accepted sibling bodies are visible, not only identifiers.
- Context limits are enforced and tested.
- Dossier generation is deterministic.

---

## PR 3: Compile diagnostic classifier and repair loop

### Scope

- Diagnostic parser
- Stable fingerprints
- Root-cause selection
- Three reconstruction strategies
- Candidate lineage
- Compile-repair prompt
- Sequential repair
- Repeated-error blocking
- Budget accounting and resume support

### Main files

- `tools/llm_harness/pipeline.py`
- `tools/llm_harness/diagnostics.py`
- `tools/llm_harness/types.py`
- `tools/llm_harness/prompts/compile_repair.md`
- `tools/llm_harness/tests/test_diagnostics.py`
- `tools/llm_harness/tests/test_pipeline.py`

### Acceptance criteria

- Compiler output from one candidate guides its next repair.
- The same diagnostic is not retried indefinitely.
- The experiment state shows complete candidate lineage.
- Old independent-run workflow remains available behind a temporary compatibility flag until benchmarked.

---

## PR 4: Structural comparison and semantic routing

### Scope

- Retail/candidate machine-code fingerprints
- Calls, relocations, memory, CFG, constants, returns, and class scores
- Structural report
- Semantic-repair prompt
- Stage router
- Structural promotion gate

### Main files

- `tools/llm_harness/structural.py`
- `tools/llm_harness/pipeline.py`
- `tools/llm_harness/prompts/semantic_repair.md`
- `tools/llm_harness/tests/test_structural.py`
- `tools/llm_harness/tests/test_pipeline.py`

### Acceptance criteria

- Compiling but semantically unrelated candidates are not sent directly to MWCC tuning.
- Reports identify concrete missing and unexpected behavior.
- First-compile promotion uses structural plausibility.
- Match improvement cannot significantly regress structure.

---

## PR 5: Match improvement and equivalence-aware ranking

### Scope

- Normalized objdiff evidence
- Improved match prompt
- Independent semantic/static/proof dimensions
- Proof-aware rank
- Counterexample summaries
- Stop-on-proof configuration
- Per-dimension best candidates

### Main files

- `tools/llm_harness/pipeline.py`
- `tools/llm_harness/ranking.py`
- `tools/llm_harness/xenoblade_project.py`
- `tools/llm_harness/prompts/improve.md`
- `tools/llm_harness/tests/test_ranking.py`
- PPC equivalence integration tests

### Acceptance criteria

- Proof outranks fuzzy similarity.
- Structural alignment is preserved during codegen tuning.
- Inconclusive verification never becomes acceptance.
- Full match still has highest priority.

---

## PR 6: Knowledge retrieval and attempt clustering

### Scope

- Stable compiler cookbook
- Target-specific retrieval
- Attempt clustering
- Provenance
- Context-token metrics
- Removal of full-history prompt insertion

### Main files

- `tools/llm_harness/knowledge.py`
- `tools/llm_harness/xenoblade_project.py`
- `tools/llm_harness/tests/test_knowledge.py`
- Knowledge/reference storage documentation

### Acceptance criteria

- Prompt contains only bounded relevant records.
- Exact target history is summarized, not copied in full.
- Retrieval is deterministic and auditable.
- Every retrieved lesson has provenance.

---

## PR 7: Held-out benchmark and calibration

### Scope

- Benchmark manifest
- Hidden-source preparation
- Ablation runner
- JSON and Markdown reports
- Baseline comparison
- Threshold calibration

### Main files

- `tools/llm_harness/benchmark.py`
- `tools/llm_harness/tests/benchmark/`
- `docs/llm-harness-benchmark.md`

### Acceptance criteria

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

## 24.1 Event log

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

## 24.2 Summary report

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

## 24.3 Aggregate metrics

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

The project is complete when all of the following are true.

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
