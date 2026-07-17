# Plan Overview — Design Context, Architecture & Data Model

**Source:** Sections 1–7 of the LLM Decompilation Improvement Plan  
**Covers:** Purpose, design decisions, diagnosis, target architecture, module layout, data model, state schema  
**Related PRs:** All — this is foundational context.

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
