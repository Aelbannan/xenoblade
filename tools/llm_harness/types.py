from __future__ import annotations

from dataclasses import asdict, dataclass, field
from typing import Any, Dict, List, Optional, Protocol


@dataclass(frozen=True)
class ModelConfig:
    id: str
    provider: str
    model: str
    runs: int = 1
    agent: Optional[str] = None
    variant: Optional[str] = None


@dataclass
class Candidate:
    full_source: str
    hypothesis: str
    notes: List[str] = field(default_factory=list)
    next_change: str = ""
    confidence: Optional[float] = None


@dataclass
class Evaluation:
    status: str
    match_percent: Optional[float]
    accepted: bool
    size_ok: Optional[bool] = None
    equivalence: Optional[str] = None
    detail: str = ""

    def rank(self) -> tuple[int, float]:
        # Project policy decides acceptance. Match percentage is only a tie-breaker.
        return (1 if self.accepted else 0, self.match_percent or 0.0)


@dataclass
class ProviderResult:
    text: str
    duration_seconds: float
    input_tokens: Optional[int] = None
    output_tokens: Optional[int] = None
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

    def to_json(self) -> Dict[str, Any]:
        return asdict(self)


class ProjectAdapter(Protocol):
    root: Any

    def build_prompt(self, workflow: str, target_id: str, history: List[Dict[str, Any]]) -> str: ...

    def evaluate(self, target_id: str, candidate: Candidate) -> Evaluation: ...

    def finalize(self) -> None: ...
