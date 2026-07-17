from __future__ import annotations

from dataclasses import dataclass, field
from typing import Any, Callable, Dict, List, Optional, Protocol

from .types import StructuralComponent, StructuralReport


@dataclass
class SemanticFrontier:
    target_id: str
    call_agreement: float = 0.0
    memory_agreement: float = 0.0
    cfg_agreement: float = 0.0
    overall_structural_score: float = 0.0
    static_match: float = 0.0
    proof_status: str = "none"

    def dominates(self, other: SemanticFrontier) -> bool:
        return (
            self.overall_structural_score >= other.overall_structural_score
            and self.call_agreement >= other.call_agreement
            and self.memory_agreement >= other.memory_agreement
        )


@dataclass
class SemanticRepairAttempt:
    repair_index: int
    primary_discrepancy: str
    source: str
    expected_effect: Dict[str, Any]
    evaluation: Dict[str, Any] | None = None
    structural_report: StructuralReport | None = None
    improved: bool = False
    error: str = ""


@dataclass
class SemanticRepairBranch:
    target_id: str
    initial_source: str
    initial_frontier: SemanticFrontier
    attempts: List[SemanticRepairAttempt] = field(default_factory=list)
    current_frontier: SemanticFrontier | None = None
    blocked: bool = False
    blocked_reason: str = ""

    @property
    def latest(self) -> SemanticRepairAttempt | None:
        return self.attempts[-1] if self.attempts else None

    def block(self, reason: str, detail: str = "") -> None:
        self.blocked = True
        self.blocked_reason = f"{reason}: {detail}" if detail else reason


RepairModelFn = Callable[[str, int], str]
EvaluateFn = Callable[[str], Dict[str, Any]]
StructuralEvalFn = Callable[[str], StructuralReport | None]


def compute_frontier(
    structural_report: StructuralReport,
    static_match: float = 0.0,
    proof_status: str = "none",
) -> SemanticFrontier:
    return SemanticFrontier(
        target_id="",
        call_agreement=structural_report.calls.score,
        memory_agreement=structural_report.memory_accesses.score,
        cfg_agreement=structural_report.cfg.score,
        overall_structural_score=structural_report.total_score,
        static_match=static_match,
        proof_status=proof_status,
    )


def improves_semantic_frontier(
    current_frontier: SemanticFrontier,
    new_frontier: SemanticFrontier,
) -> bool:
    if new_frontier.overall_structural_score > current_frontier.overall_structural_score:
        return True
    if new_frontier.overall_structural_score >= current_frontier.overall_structural_score:
        if new_frontier.dominates(current_frontier):
            return True
    return False


def record_non_improving_attempt(
    branch: SemanticRepairBranch,
    attempt: SemanticRepairAttempt,
) -> None:
    branch.block(
        "non_improving",
        f"attempt {attempt.repair_index}: {attempt.primary_discrepancy}",
    )


def run_semantic_repairs(
    branch: SemanticRepairBranch,
    max_repairs: int,
    threshold: float,
    build_prompt_fn: Callable[..., str],
    call_model_fn: RepairModelFn,
    evaluate_fn: EvaluateFn,
    structural_fn: StructuralEvalFn | None = None,
) -> SemanticRepairBranch:
    current_frontier = branch.current_frontier or branch.initial_frontier

    for repair_index in range(1, max_repairs + 1):
        if current_frontier.overall_structural_score >= threshold:
            break

        prompt = build_prompt_fn(branch, repair_index)
        response = call_model_fn(prompt, repair_index)

        import re
        import json
        cleaned = response.strip()
        fenced = re.search(r"```(?:json)?\s*(\{.*\})\s*```", cleaned, re.DOTALL)
        if fenced:
            cleaned = fenced.group(1)
        else:
            start, end = cleaned.find("{"), cleaned.rfind("}")
            if start >= 0 and end > start:
                cleaned = cleaned[start:end + 1]

        try:
            data = json.loads(cleaned)
        except (json.JSONDecodeError, ValueError):
            branch.block("model_error", f"invalid JSON in attempt {repair_index}")
            return branch

        primary = data.get("primary_discrepancy", "unknown")
        source = data.get("source", "")

        if not source.strip():
            branch.block("model_error", f"empty source in attempt {repair_index}")
            return branch

        # Evaluate the repaired source
        evaluation = evaluate_fn(source)
        ev_status = str(evaluation.get("status", evaluation.get("evaluation", {}).get("status", "")))

        # Get structural report if available
        structural_report = None
        if structural_fn is not None and (ev_status == "COMPILES" or ev_status == "CODE_MATCH"):
            try:
                structural_report = structural_fn(source)
            except Exception:
                structural_report = None

        if structural_report is not None:
            new_frontier = compute_frontier(
                structural_report,
                static_match=float(evaluation.get("match_percent", 0.0)),
                proof_status=evaluation.get("equivalence", "none"),
            )
        else:
            # Fall back to using match_percent as proxy when structural
            # comparison is not available
            improved = float(evaluation.get("match_percent", 0.0)) > current_frontier.static_match
            if not improved:
                attempt = SemanticRepairAttempt(
                    repair_index=repair_index,
                    primary_discrepancy=primary,
                    source=source,
                    expected_effect=data.get("expected_effect", {}),
                    evaluation=evaluation,
                    structural_report=None,
                    improved=False,
                )
                branch.attempts.append(attempt)
                record_non_improving_attempt(branch, attempt)
                return branch

            new_frontier = SemanticFrontier(
                target_id=branch.target_id,
                call_agreement=current_frontier.call_agreement,
                memory_agreement=current_frontier.memory_agreement,
                cfg_agreement=current_frontier.cfg_agreement,
                overall_structural_score=current_frontier.overall_structural_score,
                static_match=float(evaluation.get("match_percent", 0.0)),
                proof_status=evaluation.get("equivalence", "none"),
            )

        improved = improves_semantic_frontier(current_frontier, new_frontier)

        attempt = SemanticRepairAttempt(
            repair_index=repair_index,
            primary_discrepancy=primary,
            source=source,
            expected_effect=data.get("expected_effect", {}),
            evaluation=evaluation,
            structural_report=structural_report,
            improved=improved,
        )
        branch.attempts.append(attempt)

        if not improved:
            record_non_improving_attempt(branch, attempt)
            return branch

        current_frontier = new_frontier
        branch.current_frontier = current_frontier

    return branch
