from __future__ import annotations

from dataclasses import asdict, dataclass, field
from enum import Enum
from typing import Any, Dict, List, Optional, Protocol


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


class BlockedReason(str, Enum):
    MISSING_DECLARATION = "missing_declaration"
    UNKNOWN_TYPE = "unknown_type"
    UNCERTAIN_SIGNATURE = "uncertain_signature"
    SOURCE_BOUNDARY = "source_boundary"
    UNSUPPORTED_CONTEXT = "unsupported_context"
    REPEATED_DIAGNOSTIC = "repeated_diagnostic"
    UNSUPPORTED_INSTRUCTION = "unsupported_instruction"
    BUDGET_EXHAUSTED = "budget_exhausted"


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
    exit_code: int = 0
    diagnostics: List[CompilerDiagnostic] = field(default_factory=list)
    raw_output_path: str | None = None
    root_fingerprint: str | None = None


@dataclass
class StructuralComponent:
    score: float
    matched: int
    expected: int
    details: List[str] = field(default_factory=list)


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
    unexpected_effects: List[str] = field(default_factory=list)
    missing_effects: List[str] = field(default_factory=list)


@dataclass
class CandidateEvaluation:
    status: CandidateStatus
    compile_report: CompileReport
    match_percent: float = 0.0
    structural_report: StructuralReport | None = None
    equivalence_status: str | None = None
    function_size: int | None = None
    retail_size: int | None = None
    object_regressions: List[str] = field(default_factory=list)
    accepted_function_regressions: List[str] = field(default_factory=list)
    warnings: List[str] = field(default_factory=list)


@dataclass
class CandidateLineage:
    candidate_id: str
    parent_candidate_id: str | None
    root_candidate_id: str
    stage: PipelineStage
    generation_index: int
    repair_depth: int
    model_id: str


@dataclass
class BaselineSnapshot:
    target_id: str
    source_text: str
    source_hash: str
    evaluation: CandidateEvaluation | None
    captured_at: str
    root_revision: str | None = None
    dirty_paths: List[str] = field(default_factory=list)


@dataclass
class PromotionResult:
    promoted: bool
    rolled_back: bool
    reason: str
    target_id: str = ""
    source_hash_before: str = ""
    source_hash_after: str | None = None
    baseline_rank: tuple = ()
    candidate_rank: tuple = ()
    validation_steps: List[Dict[str, Any]] = field(default_factory=list)
    rollback_error: str | None = None


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


@dataclass
class ValidationStepResult:
    name: str
    succeeded: bool
    exit_code: int | None
    detail: str
    artifact_paths: List[str] = field(default_factory=list)


@dataclass(frozen=True)
class ModelConfig:
    id: str
    provider: str
    model: str
    runs: int = 1
    agent: Optional[str] = None
    variant: Optional[str] = None


@dataclass
class SourcePatch:
    slot_id: str
    source: str


@dataclass
class Candidate:
    source: str
    hypothesis: str
    notes: List[str] = field(default_factory=list)
    next_change: str = ""
    patches: List[SourcePatch] = field(default_factory=list)

    def __post_init__(self) -> None:
        self.patches = [
            value if isinstance(value, SourcePatch) else SourcePatch(**value)
            for value in self.patches
        ]


@dataclass
class Evaluation:
    status: str
    match_percent: Optional[float]
    accepted: bool
    size_ok: Optional[bool] = None
    equivalence: Optional[str] = None
    detail: str = ""
    metrics: Dict[str, Any] = field(default_factory=dict)

    def rank(self) -> tuple[int, float]:
        # Project policy decides acceptance. Match percentage is only a tie-breaker.
        return (1 if self.accepted else 0, self.match_percent or 0.0)


@dataclass
class ProviderResult:
    text: str
    duration_seconds: float
    input_tokens: Optional[int] = None
    output_tokens: Optional[int] = None
    cache_read_tokens: Optional[int] = None
    cache_write_tokens: Optional[int] = None
    cost: Optional[float] = None
    raw_events: List[Dict[str, Any]] = field(default_factory=list)


@dataclass
class ExperimentRecord:
    schema_version: int
    experiment_id: str
    timestamp: str
    workflow: str
    target_id: str
    model_id: str
    model: str
    run_index: int
    duration_seconds: float
    input_tokens: Optional[int]
    output_tokens: Optional[int]
    cost: Optional[float]
    evaluation: Dict[str, Any]
    artifact: str
    candidate_summary: Dict[str, Any] = field(default_factory=dict)
    winner: bool = False
    error: Optional[str] = None
    cache_read_tokens: Optional[int] = None
    cache_write_tokens: Optional[int] = None
    timed_out: bool = False

    def to_json(self) -> Dict[str, Any]:
        return asdict(self)


class ProjectAdapter(Protocol):
    root: Any

    def build_prompt(
        self,
        workflow: str,
        target_id: str,
        history: List[Dict[str, Any]],
        options: Optional[Dict[str, Any]] = None,
    ) -> str: ...

    def evaluate(self, workflow: str, target_id: str, candidate: Candidate) -> Evaluation: ...

    def finalize(self) -> None: ...

    # Optional — accessed via getattr in Harness:
    # def select_new_targets(self, number, history, *, ...) -> List[str]: ...
    # def select_targets(self, workflow, number, *, ...) -> List[str]: ...
    # def target_ids_for_unit(self, unit_name, workflow) -> List[str]: ...
    # def prepare(self, target_id, *, write) -> str: ...
    # def prepare_workspace(self, path) -> None: ...
    # def create_slot(self, target_id, file, ...) -> str: ...
    # def create_tu_slot(self, unit, slot_id, file, ...) -> str: ...
    # def promote(self, workflow, target_id, candidate, *, write) -> str: ...
    # def rank_candidate(self, workflow, evaluation) -> tuple: ...
    # def model_context_mode(self, workflow) -> str: ...
    # def build_context_files(self, ...) -> Dict[str, str]: ...
    # --- Phase 0 / Promotion safety optional methods ---
    # def read_target_source(self, target_id: str) -> str: ...
    # def target_source_path(self, target_id: str) -> Path: ...
    # def evaluate_canon(self, target_id: str, artifact_dir: Path) -> CandidateEvaluation | Evaluation: ...
