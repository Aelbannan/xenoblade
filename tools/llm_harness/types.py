from __future__ import annotations

import hashlib
import json
from dataclasses import asdict, dataclass, field
from datetime import datetime, timezone
from enum import Enum
from pathlib import Path
from typing import Any, Dict, List, Literal, Optional, Protocol, Union


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
    PRECHECK_ERROR = "precheck_error"
    BUILD_ERROR = "build_error"
    SYMBOL_EXTRACTION_ERROR = "symbol_extraction_error"
    MATCH_EVALUATION_ERROR = "match_evaluation_error"
    EQUIVALENCE_ERROR = "equivalence_error"
    RESTORE_ERROR = "restore_error"


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
class CandidateId:
    experiment_id: str
    branch_id: str
    iteration: int


@dataclass(frozen=True)
class InstructionWindow:
    start_offset: int
    retail: List[str]
    candidate: List[str]


@dataclass(frozen=True)
class InstructionDifference:
    offset: int
    retail_raw: str
    candidate_raw: str
    retail_mnemonic: str
    candidate_mnemonic: str
    likely_cause: str = ""


@dataclass(frozen=True)
class SizeComparison:
    retail: int
    candidate: int


@dataclass(frozen=True)
class StackFrameComparison:
    retail: int
    candidate: int


@dataclass(frozen=True)
class ComponentFinding:
    score: float
    matched: int
    expected: int
    details: List[str] = field(default_factory=list)


@dataclass(frozen=True)
class BinaryDiffFeedback:
    kind: Literal["compile", "binary_diff", "accepted", "evaluation_error"]
    first_difference_offset: Optional[int] = None
    instruction_windows: List[InstructionWindow] = field(default_factory=list)
    differences: List[InstructionDifference] = field(default_factory=list)
    relocation_differences: Dict[str, List[str]] = field(default_factory=dict)
    function_size: Optional[SizeComparison] = None
    stack_frame: Optional[StackFrameComparison] = None
    structural_findings: Dict[str, ComponentFinding] = field(default_factory=dict)
    fingerprint: str = ""


def fingerprint_binary_feedback(feedback: Union[BinaryDiffFeedback, Dict[str, Any]]) -> str:
    """Build a stable mismatch fingerprint from binary diff feedback."""
    # Handle both BinaryDiffFeedback object and dict
    if isinstance(feedback, BinaryDiffFeedback):
        differences = feedback.differences
        relocation_differences = feedback.relocation_differences
        function_size = feedback.function_size
    else:
        differences = feedback.get("differences", [])
        relocation_differences = feedback.get("relocation_differences", {})
        function_size = feedback.get("function_size", {})
    
    payload = {
        "instruction_pairs": [
            (d.offset, d.retail_raw, d.candidate_raw) if hasattr(d, 'offset') else (d.get("offset", 0), d.get("retail", ""), d.get("candidate", ""))
            for d in differences[:20]
        ],
        "relocations": relocation_differences,
        "size_delta": (
            function_size.candidate - function_size.retail
            if function_size and hasattr(function_size, 'candidate') and hasattr(function_size, 'retail')
            else (function_size.get("candidate", 0) - function_size.get("retail", 0) if isinstance(function_size, dict) else 0)
        ),
    }
    canonical = json.dumps(payload, separators=(",", ":"), sort_keys=True)
    return hashlib.sha256(canonical.encode("utf-8")).hexdigest()[:16]


@dataclass
class CandidateArtifact:
    candidate_id: CandidateId
    parent_id: Optional[CandidateId]
    strategy: str
    source: str
    hypothesis: str
    notes: List[str]
    next_change: str
    evaluation: Optional[Dict[str, Any]] = None
    feedback: Optional[BinaryDiffFeedback] = None


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
    # Phase 4 — explicit acceptance semantics (do not overload accepted)
    symbol_accepted: bool = False
    project_ready: bool | None = None
    promising: bool = False
    blocked_reason: str | None = None
    function_size_delta: int | None = None
    object_split_fit: bool | None = None


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


@dataclass
class PipelineBudgets:
    reconstruct: int = 3
    compile_repair: int = 3
    semantic_repair: int = 2
    match_improve: int = 2


@dataclass
class PipelineStopOn:
    full_match: bool = True
    proved_equivalent: bool = True


@dataclass
class PipelineStructuralConfig:
    weights: Dict[str, float] = field(default_factory=lambda: {
        "calls": 0.20,
        "relocations": 0.10,
        "memory_accesses": 0.25,
        "cfg": 0.20,
        "constants": 0.10,
        "returns": 0.10,
        "instruction_classes": 0.05,
    })
    semantic_repair_below: float = 0.75
    first_compile_promotion_min: float = 0.60
    max_structural_regression_for_match_improve: float = 0.02


@dataclass
class PipelinePromotionConfig:
    enabled: bool = True
    lock_scope: str = "global"
    require_monotonic_rank: bool = True
    allow_first_compile: bool = True
    first_compile_min_structural_score: float = 0.60
    protect_accepted_functions: bool = True
    revalidate_against_latest_root: bool = True
    rollback_on_failure: bool = True
    run_coop_cycle: bool = True


@dataclass
class PipelineKnowledgeConfig:
    mode: str = "retrieved"
    max_records: int = 10
    cluster_attempts: bool = True


@dataclass
class FeaturesConfig:
    """§23.1 — Temporary feature flags for progressive rollout."""
    transactional_promotion: bool = True
    typed_dossier: bool = True
    staged_pipeline: bool = False
    structural_routing: bool = False
    retrieved_knowledge: bool = False


@dataclass
class PipelineEvent:
    """§24.1 — Structured event for observability."""
    event: str
    experiment_id: str = ""
    target_id: str = ""
    stage: str = ""
    status: str = ""
    detail: str = ""
    match_percent: float = 0.0
    structural_score: float = 0.0
    duration_seconds: float = 0.0
    timestamp: str = ""

    def to_dict(self) -> Dict[str, Any]:
        return asdict(self)


@dataclass
class PipelineConfig:
    budgets: PipelineBudgets = field(default_factory=PipelineBudgets)
    stop_on: PipelineStopOn = field(default_factory=PipelineStopOn)
    structural: PipelineStructuralConfig = field(default_factory=PipelineStructuralConfig)
    promotion: PipelinePromotionConfig = field(default_factory=PipelinePromotionConfig)
    knowledge: PipelineKnowledgeConfig = field(default_factory=PipelineKnowledgeConfig)
    features: FeaturesConfig = field(default_factory=FeaturesConfig)

    @classmethod
    def from_dict(cls, data: Dict[str, Any]) -> PipelineConfig:
        raw_budgets = data.get("budgets", {})
        raw_stop = data.get("stop_on", {})
        raw_structural = data.get("structural", {})
        raw_promotion = data.get("promotion", {})
        raw_knowledge = data.get("knowledge", {})
        raw_features = data.get("features", {})

        budgets = PipelineBudgets(
            reconstruct=int(raw_budgets.get("reconstruct", 3)),
            compile_repair=int(raw_budgets.get("compile_repair", 3)),
            semantic_repair=int(raw_budgets.get("semantic_repair", 2)),
            match_improve=int(raw_budgets.get("match_improve", 2)),
        )
        stop_on = PipelineStopOn(
            full_match=bool(raw_stop.get("full_match", True)),
            proved_equivalent=bool(raw_stop.get("proved_equivalent", True)),
        )
        structural = PipelineStructuralConfig(
            weights=raw_structural.get("weights", PipelineStructuralConfig().weights),
            semantic_repair_below=float(raw_structural.get("semantic_repair_below", 0.75)),
            first_compile_promotion_min=float(raw_structural.get("first_compile_promotion_min", 0.60)),
            max_structural_regression_for_match_improve=float(
                raw_structural.get("max_structural_regression_for_match_improve", 0.02)
            ),
        )
        promotion = PipelinePromotionConfig(
            enabled=bool(raw_promotion.get("enabled", True)),
            lock_scope=str(raw_promotion.get("lock_scope", "global")),
            require_monotonic_rank=bool(raw_promotion.get("require_monotonic_rank", True)),
            allow_first_compile=bool(raw_promotion.get("allow_first_compile", True)),
            first_compile_min_structural_score=float(
                raw_promotion.get("first_compile_min_structural_score", 0.60)
            ),
            protect_accepted_functions=bool(raw_promotion.get("protect_accepted_functions", True)),
            revalidate_against_latest_root=bool(raw_promotion.get("revalidate_against_latest_root", True)),
            rollback_on_failure=bool(raw_promotion.get("rollback_on_failure", True)),
            run_coop_cycle=bool(raw_promotion.get("run_coop_cycle", True)),
        )
        knowledge = PipelineKnowledgeConfig(
            mode=str(raw_knowledge.get("mode", "retrieved")),
            max_records=int(raw_knowledge.get("max_records", 10)),
            cluster_attempts=bool(raw_knowledge.get("cluster_attempts", True)),
        )
        features = FeaturesConfig(
            transactional_promotion=bool(raw_features.get("transactional_promotion", True)),
            typed_dossier=bool(raw_features.get("typed_dossier", True)),
            staged_pipeline=bool(raw_features.get("staged_pipeline", False)),
            structural_routing=bool(raw_features.get("structural_routing", False)),
            retrieved_knowledge=bool(raw_features.get("retrieved_knowledge", False)),
        )

        return cls(budgets=budgets, stop_on=stop_on, structural=structural,
                   promotion=promotion, knowledge=knowledge, features=features)

    def validate(self) -> List[str]:
        errors: List[str] = []
        weights = self.structural.weights
        total = sum(weights.values())
        if abs(total - 1.0) > 0.001:
            errors.append(f"structural.weights sum to {total:.3f}, expected 1.0")
        for key in ("calls", "relocations", "memory_accesses", "cfg", "constants", "returns", "instruction_classes"):
            if key not in weights:
                errors.append(f"structural.weights missing key: {key}")
        for name, value in [
            ("budgets.reconstruct", self.budgets.reconstruct),
            ("budgets.compile_repair", self.budgets.compile_repair),
            ("budgets.semantic_repair", self.budgets.semantic_repair),
            ("budgets.match_improve", self.budgets.match_improve),
        ]:
            if value < 0:
                errors.append(f"{name} must be nonnegative, got {value}")
        for name, value in [
            ("structural.semantic_repair_below", self.structural.semantic_repair_below),
            ("structural.first_compile_promotion_min", self.structural.first_compile_promotion_min),
            ("structural.max_structural_regression_for_match_improve",
             self.structural.max_structural_regression_for_match_improve),
        ]:
            if not 0.0 <= value <= 1.0:
                errors.append(f"{name} must be in [0, 1], got {value}")
        known_scopes = {"global", "unit", "none"}
        if self.promotion.lock_scope not in known_scopes:
            errors.append(f"promotion.lock_scope must be one of {known_scopes}, got {self.promotion.lock_scope!r}")
        if self.promotion.allow_first_compile and sum(self.structural.weights.values()) < 0.001:
            errors.append("first_compile promotion requires structural analysis weights to be configured")
        if self.knowledge.mode not in ("retrieved", "frozen", "none"):
            errors.append(f"knowledge.mode must be one of retrieved/frozen/none, got {self.knowledge.mode!r}")
        return errors


@dataclass(frozen=True)
class ModelConfig:
    id: str
    provider: str
    model: str
    runs: int = 1
    agent: Optional[str] = None
    variant: Optional[str] = None
    max_tokens: Optional[int] = None
    enable_thinking: Optional[bool] = None
    thinking_budget: Optional[int] = None
    reasoning_effort: Optional[str] = None


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
    symbol_accepted: bool = False
    project_ready: Optional[bool] = None
    promising: bool = False
    blocked_reason: Optional[str] = None

    def rank(self) -> tuple[int, float]:
        # Project policy decides acceptance. Match percentage is only a tie-breaker.
        return (1 if self.symbol_accepted or self.accepted else 0, self.match_percent or 0.0)


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

    # §18 — Pipeline adapter methods (optional, accessed via getattr)

    def read_target_source(self, target_id: str) -> str: ...

    def build_dossier(
        self,
        target_id: str,
        history: List[Dict[str, Any]],
        options: Optional[Dict[str, Any]] = None,
    ) -> Dict[str, Any]: ...

    def apply_candidate(
        self,
        root: Path,
        target_id: str,
        candidate: Candidate,
    ) -> SourcePatch: ...

    def evaluate_candidate(
        self,
        root: Path,
        target_id: str,
        candidate: Candidate,
        artifact_dir: Path,
    ) -> CandidateEvaluation: ...

    def decode_function(
        self,
        root: Path,
        target_id: str,
        side: str,
    ) -> Any: ...

    def detect_placeholder(
        self,
        target_id: str,
        source: str,
    ) -> bool: ...

    def protected_functions(
        self,
        target_id: str,
    ) -> List[str]: ...

    def run_promotion_validation(
        self,
        target_id: str,
        artifact_dir: Path,
    ) -> List[ValidationStepResult]: ...

    # Legacy optional methods preserved for backward compatibility:
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
    # def evaluate_canon(self, target_id, artifact_dir) -> CandidateEvaluation | Evaluation: ...


@dataclass
class ExperimentState:
    """Serialisable pipeline experiment state with resume support."""
    schema_version: int = 2
    experiment_id: str = ""
    workflow: str = "new"
    target_id: str = ""
    status: PipelineStage = PipelineStage.BASELINE
    status_detail: str = ""
    started_at: str = ""
    updated_at: str = ""
    baseline: Optional[Dict[str, Any]] = None
    dossier: Optional[Dict[str, Any]] = None
    records: List[Dict[str, Any]] = field(default_factory=list)
    best_candidate: Optional[Dict[str, Any]] = None
    best_evaluation: Optional[Dict[str, Any]] = None
    promotion: Optional[Dict[str, Any]] = None
    logged_keys: List[str] = field(default_factory=list)
    budgets: Optional[Dict[str, int]] = None

    def to_json(self) -> Dict[str, Any]:
        return asdict(self)

    @staticmethod
    def from_json(data: Dict[str, Any]) -> ExperimentState:
        # Convert string status to enum
        status_val = data.get("status", "baseline")
        if isinstance(status_val, str):
            try:
                data["status"] = PipelineStage(status_val)
            except ValueError:
                data["status"] = PipelineStage.BASELINE
        return ExperimentState(**data)

    def save(self, path: Path) -> None:
        path.parent.mkdir(parents=True, exist_ok=True)
        tmp = path.with_suffix(".json.tmp")
        tmp.write_text(json.dumps(self.to_json(), indent=2) + "\n", encoding="utf-8")
        tmp.replace(path)

    @staticmethod
    def load(path: Path) -> ExperimentState:
        data = json.loads(path.read_text(encoding="utf-8"))
        return ExperimentState.from_json(data)
