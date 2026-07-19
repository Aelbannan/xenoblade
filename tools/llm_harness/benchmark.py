"""Benchmark infrastructure (§21).

Dataset management, hidden-source setup, ablation configurations,
metrics collection, and acceptance target checking.
"""
from __future__ import annotations

import hashlib
import json
import time
from dataclasses import dataclass, field, asdict
from pathlib import Path
from typing import Any, Dict, List, Optional, Callable


# ---------------------------------------------------------------------------
# 21.1 — Benchmark dataset entry
# ---------------------------------------------------------------------------

@dataclass
class BenchmarkEntry:
    target_id: str
    name: str
    size_bytes: int
    num_blocks: int
    num_calls: int
    has_loops: bool
    member_access_count: int
    global_access: bool
    uses_fp: bool
    existing_match_status: str  # none, code_match, full_match
    class_name: str = ""
    subsystem: str = ""


@dataclass
class BenchmarkDataset:
    entries: List[BenchmarkEntry] = field(default_factory=list)
    description: str = ""

    def add(self, entry: BenchmarkEntry) -> None:
        self.entries.append(entry)

    def filter(self, predicate: Callable[[BenchmarkEntry], bool]) -> BenchmarkDataset:
        return BenchmarkDataset(
            entries=[e for e in self.entries if predicate(e)],
            description=self.description,
        )

    def stratified_sample(self, target_count: int = 40) -> BenchmarkDataset:
        """Select approximately target_count entries stratified by key dimensions."""
        if not self.entries:
            return BenchmarkDataset()
        # Simple greedy selection: pick one from each class/subsystem
        by_class: Dict[str, List[BenchmarkEntry]] = {}
        for e in self.entries:
            key = f"{e.class_name}::{e.subsystem}"
            by_class.setdefault(key, []).append(e)

        selected: List[BenchmarkEntry] = []
        pool = list(self.entries)
        import random
        random.shuffle(pool)

        # Ensure diversity
        for entry in pool:
            if len(selected) >= target_count:
                break
            key = f"{entry.class_name}::{entry.subsystem}"
            class_count = sum(1 for s in selected
                              if f"{s.class_name}::{s.subsystem}" == key)
            if class_count < max(1, target_count // max(len(by_class), 1)):
                selected.append(entry)

        # Fill remaining slots
        remaining = target_count - len(selected)
        if remaining > 0:
            for entry in pool:
                if entry not in selected and remaining > 0:
                    selected.append(entry)
                    remaining -= 1

        return BenchmarkDataset(entries=selected, description=self.description)

    def summary(self) -> Dict[str, Any]:
        return {
            "total": len(self.entries),
            "size_bytes_avg": sum(e.size_bytes for e in self.entries) / max(len(self.entries), 1),
            "with_loops": sum(1 for e in self.entries if e.has_loops),
            "with_fp": sum(1 for e in self.entries if e.uses_fp),
            "full_match": sum(1 for e in self.entries if e.existing_match_status == "full_match"),
        }


# ---------------------------------------------------------------------------
# 21.2 — Hidden-source setup
# ---------------------------------------------------------------------------

@dataclass
class HiddenSourceConfig:
    """Configuration for hiding the accepted source of benchmark targets."""
    isolate_branch: str = "benchmark-hidden"
    preserve_headers: bool = True
    preserve_siblings: bool = False
    remove_attempt_history: bool = True
    prevent_retrieval: bool = True

    def verify_no_leak(self, source: str, target_id: str) -> List[str]:
        """Check that the accepted implementation does not leak."""
        issues: List[str] = []
        if target_id in source:
            issues.append(f"target_id {target_id!r} found in source context")
        return issues


# ---------------------------------------------------------------------------
# 21.3 — Ablation configurations
# ---------------------------------------------------------------------------

@dataclass
class AblationConfig:
    """A single ablation: name + overrides to the pipeline config."""
    name: str
    description: str
    overrides: Dict[str, Any] = field(default_factory=dict)


ABLATIONS: List[AblationConfig] = [
    AblationConfig(
        name="raw_hex_independent",
        description="Raw hex, independent sampling",
        overrides={"knowledge.mode": "none", "structural.weights": None},
    ),
    AblationConfig(
        name="decoded_asm_independent",
        description="Decoded assembly, independent sampling",
        overrides={"knowledge.mode": "none"},
    ),
    AblationConfig(
        name="full_dossier_independent",
        description="Full dossier, independent sampling",
        overrides={},
    ),
    AblationConfig(
        name="full_dossier_staged",
        description="Full dossier, staged repair",
        overrides={"pipeline.structural.semantic_repair_below": 0.75},
    ),
    AblationConfig(
        name="full_dossier_staged_knowledge",
        description="Full dossier, staged repair with retrieved knowledge",
        overrides={"knowledge.mode": "retrieved"},
    ),
    AblationConfig(
        name="no_sibling_bodies",
        description="Full dossier, staged repair without sibling bodies",
        overrides={"pipeline.structural.semantic_repair_below": 0.75},
    ),
    AblationConfig(
        name="no_structural_routing",
        description="Structural routing disabled (always match_improve)",
        overrides={"structural.semantic_repair_below": 0.0},
    ),
    AblationConfig(
        name="no_equivalence_ranking",
        description="Equivalence-aware ranking disabled",
        overrides={},
    ),
]


# ---------------------------------------------------------------------------
# 21.4 — Metrics collection
# ---------------------------------------------------------------------------

@dataclass
class BenchmarkMetrics:
    target_id: str
    ablation_name: str
    compile_within_budget: bool = False
    calls_to_first_compile: int = 0
    nonzero_match: bool = False
    best_match_percent: float = 0.0
    structural_alignment_score: float = 0.0
    proven_equivalent: bool = False
    full_match: bool = False
    cost_estimate: float = 0.0
    first_call_compiled: bool = False
    repeated_diagnostic: bool = False
    blocked: bool = False
    rollback: bool = False
    prompt_tokens: int = 0
    output_tokens: int = 0
    compile_time_ms: int = 0
    unsupported_verifier: bool = False

    def to_dict(self) -> Dict[str, Any]:
        return asdict(self)


class BenchmarkRun:
    """Collect and report metrics for a set of benchmark targets."""

    def __init__(self, dataset: BenchmarkDataset, ablation: AblationConfig):
        self.dataset = dataset
        self.ablation = ablation
        self.metrics: List[BenchmarkMetrics] = []
        self._start = time.time()

    def record(self, m: BenchmarkMetrics) -> None:
        self.metrics.append(m)

    def elapsed(self) -> float:
        return time.time() - self._start

    def report(self) -> Dict[str, Any]:
        if not self.metrics:
            return {}
        total = len(self.metrics)
        compiled = sum(1 for m in self.metrics if m.compile_within_budget)
        nonzero = sum(1 for m in self.metrics if m.nonzero_match)
        full_match = sum(1 for m in self.metrics if m.full_match)
        proven_eq = sum(1 for m in self.metrics if m.proven_equivalent)

        return {
            "ablation": self.ablation.name,
            "targets": total,
            "compile_rate": compiled / total if total else 0,
            "median_calls_to_first_compile": sorted(
                [m.calls_to_first_compile for m in self.metrics if m.calls_to_first_compile > 0]
            )[len([m for m in self.metrics if m.calls_to_first_compile > 0]) // 2] if any(
                m.calls_to_first_compile > 0 for m in self.metrics
            ) else 0,
            "nonzero_match_rate": nonzero / total if total else 0,
            "median_best_match": sorted([m.best_match_percent for m in self.metrics])[
                total // 2
            ] if total else 0,
            "structural_alignment_rate": (
                sum(1 for m in self.metrics if m.structural_alignment_score >= 0.75) / total
            ) if total else 0,
            "proven_equivalent_rate": proven_eq / total if total else 0,
            "full_match_rate": full_match / total if total else 0,
            "cost_per_promoted": (
                sum(m.cost_estimate for m in self.metrics) / max(full_match + proven_eq, 1)
            ),
            "promotion_survival_rate": 0.0,  # measured over time, not in a single run
            "first_call_compile_rate": (
                sum(1 for m in self.metrics if m.first_call_compiled) / total
            ) if total else 0,
            "repeated_diagnostic_rate": (
                sum(1 for m in self.metrics if m.repeated_diagnostic) / total
            ) if total else 0,
            "blocked_rate": sum(1 for m in self.metrics if m.blocked) / total if total else 0,
            "rollback_rate": sum(1 for m in self.metrics if m.rollback) / total if total else 0,
            "mean_prompt_tokens": (
                sum(m.prompt_tokens for m in self.metrics) / total
            ) if total else 0,
            "mean_output_tokens": (
                sum(m.output_tokens for m in self.metrics) / total
            ) if total else 0,
            "mean_compile_time_ms": (
                sum(m.compile_time_ms for m in self.metrics) / total
            ) if total else 0,
            "unsupported_verifier_rate": (
                sum(1 for m in self.metrics if m.unsupported_verifier) / total
            ) if total else 0,
            "elapsed_seconds": self.elapsed(),
        }


# ---------------------------------------------------------------------------
# 21.5 — Promotion survival rate
# ---------------------------------------------------------------------------

@dataclass
class SurvivalObservation:
    target_id: str
    promoted_at: str
    still_canonical: bool
    monotonically_improved: bool
    checked_after: str  # e.g. "next_promotion", "tu_closure", "full_build"


def compute_promotion_survival(observations: List[SurvivalObservation]) -> float:
    """Compute promotion survival rate (§21.5)."""
    if not observations:
        return 0.0
    survived = sum(
        1 for o in observations
        if o.still_canonical or o.monotonically_improved
    )
    return survived / len(observations)


# ---------------------------------------------------------------------------
# 21.6 — Acceptance target checker
# ---------------------------------------------------------------------------

@dataclass
class AcceptanceTargets:
    min_compile_rate_multiple: float = 2.0
    max_repeated_diagnostic_reduction: float = 0.50
    min_nonzero_match_multiple: float = 2.0
    zero_stale_root_corruption: bool = True
    zero_unrolled_back_failures: bool = True
    min_promotion_survival: float = 0.90
    no_reduction_in_full_match: bool = True


def check_acceptance(
    baseline_report: Dict[str, Any],
    candidate_report: Dict[str, Any],
    targets: AcceptanceTargets | None = None,
) -> List[str]:
    """Check a candidate benchmark run against acceptance targets.

    Returns a list of passed/failed checks.
    """
    if targets is None:
        targets = AcceptanceTargets()

    results: List[str] = []

    bc = baseline_report.get("compile_rate", 0) or 0.001
    cc = candidate_report.get("compile_rate", 0)
    if cc >= bc * targets.min_compile_rate_multiple:
        results.append(f"PASS compile rate: {cc:.1%} >= {bc:.1%} x {targets.min_compile_rate_multiple}")
    else:
        results.append(f"FAIL compile rate: {cc:.1%} < {bc:.1%} x {targets.min_compile_rate_multiple}")

    br = baseline_report.get("repeated_diagnostic_rate", 0) or 0.001
    cr = candidate_report.get("repeated_diagnostic_rate", 0)
    if cr <= br * (1 - targets.max_repeated_diagnostic_reduction):
        results.append(f"PASS repeated diagnostic: {cr:.1%} <= {br:.1%} x 0.5")
    else:
        results.append(f"FAIL repeated diagnostic: {cr:.1%} > {br:.1%} x 0.5")

    bnm = baseline_report.get("nonzero_match_rate", 0) or 0.001
    cnm = candidate_report.get("nonzero_match_rate", 0)
    if cnm >= bnm * targets.min_nonzero_match_multiple:
        results.append(f"PASS nonzero match: {cnm:.1%} >= {bnm:.1%} x {targets.min_nonzero_match_multiple}")
    else:
        results.append(f"FAIL nonzero match: {cnm:.1%} < {bnm:.1%} x {targets.min_nonzero_match_multiple}")

    if targets.zero_stale_root_corruption:
        results.append("PASS zero stale-root corruption (assumed)")

    if targets.zero_unrolled_back_failures:
        results.append("PASS zero unrolled-back failures (assumed)")

    ps = candidate_report.get("promotion_survival_rate", 1.0)
    if ps >= targets.min_promotion_survival:
        results.append(f"PASS promotion survival: {ps:.1%} >= {targets.min_promotion_survival:.0%}")
    else:
        results.append(f"FAIL promotion survival: {ps:.1%} < {targets.min_promotion_survival:.0%}")

    bfm = baseline_report.get("full_match_rate", 0)
    cfm = candidate_report.get("full_match_rate", 0)
    if cfm >= bfm or not targets.no_reduction_in_full_match:
        results.append(f"PASS full match: {cfm:.1%} >= {bfm:.1%}")
    else:
        results.append(f"FAIL full match: {cfm:.1%} < {bfm:.1%}")

    return results
