# Pipeline Orchestration, Configuration, Adapter & Prompts

**Source:** Sections 16–19 of the LLM Decompilation Improvement Plan  
**Covers:** PipelineRunner, resume behavior, parallelism, cancellation, config schema, adapter interface, prompt template skeletons

---

## 16. Orchestration implementation

### 16.1 `PipelineRunner`

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

### 16.2 Resume behavior

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

### 16.3 Parallelism

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

### 16.4 Cancellation

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

### 19.1 Common prompt

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

### 19.2 Reconstruction prompt skeleton

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

### 19.3 Compile repair prompt skeleton

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

### 19.4 Semantic repair prompt skeleton

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

### 19.5 Match improve prompt skeleton

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
