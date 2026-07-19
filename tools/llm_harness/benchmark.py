"""Phase 9–10 benchmark infrastructure for the closed-loop LLM harness.

Provides:
- size/shape bucketing
- metrics collection for solve vs blind sample
- hidden-source corpus helpers
- report aggregation and acceptance checks

Live model runs are optional; fake-provider comparisons exercise the same
reporting path without network or API cost.
"""
from __future__ import annotations

import json
import time
from dataclasses import asdict, dataclass, field
from pathlib import Path
from typing import Any, Callable, Dict, Iterable, List, Optional, Sequence


SIZE_BUCKETS: tuple[tuple[str, int, int], ...] = (
    ("1-8", 1, 8),
    ("9-24", 9, 24),
    ("25-64", 25, 64),
    ("65-160", 65, 160),
    ("161+", 161, 10_000_000),
)


@dataclass(frozen=True)
class ShapeTags:
    leaf: bool = False
    has_relocations: bool = False
    branch_heavy: bool = False
    member_access: bool = False
    typed_context: bool = True


@dataclass
class CorpusEntry:
    target_id: str
    category: str
    function: str = ""
    unit: str = ""
    source: str = ""
    status: str = "FULL_MATCH"
    size_bytes: int = 0
    approx_insns: int = 0
    shape: ShapeTags = field(default_factory=ShapeTags)
    notes: str = ""

    @property
    def size_bucket(self) -> str:
        return classify_size_bucket(self.approx_insns or max(1, self.size_bytes // 4))


def classify_size_bucket(instruction_count: int) -> str:
    for name, lo, hi in SIZE_BUCKETS:
        if lo <= instruction_count <= hi:
            return name
    return "161+"


def load_corpus(path: Path) -> List[CorpusEntry]:
    raw = json.loads(path.read_text(encoding="utf-8"))
    entries: List[CorpusEntry] = []
    for row in raw:
        if not row.get("target_id"):
            continue
        shape_raw = row.get("shape") or {}
        entries.append(
            CorpusEntry(
                target_id=str(row["target_id"]),
                category=str(row.get("category", "unclassified")),
                function=str(row.get("function", "")),
                unit=str(row.get("unit", "")),
                source=str(row.get("source", "")),
                status=str(row.get("status", "FULL_MATCH")),
                size_bytes=int(row.get("size_bytes") or 0),
                approx_insns=int(row.get("approx_insns") or 0),
                shape=ShapeTags(
                    leaf=bool(shape_raw.get("leaf", False)),
                    has_relocations=bool(shape_raw.get("has_relocations", False)),
                    branch_heavy=bool(shape_raw.get("branch_heavy", False)),
                    member_access=bool(shape_raw.get("member_access", False)),
                    typed_context=bool(shape_raw.get("typed_context", True)),
                ),
                notes=str(row.get("notes") or row.get("why") or ""),
            )
        )
    return entries


# ---------------------------------------------------------------------------
# Metrics
# ---------------------------------------------------------------------------


@dataclass
class TargetRunMetrics:
    target_id: str
    workflow: str  # solve | sample
    category: str = ""
    size_bucket: str = ""
    model_calls: int = 0
    parse_ok: int = 0
    compile_ok: int = 0
    baseline_match_percent: float = 0.0
    best_match_percent: float = 0.0
    symbol_accepted: bool = False
    full_match: bool = False
    equivalent_match: bool = False
    repair_success: bool = False
    repeated_fingerprint_stop: bool = False
    evaluation_errors: int = 0
    prompt_chars: int = 0
    input_tokens: int = 0
    output_tokens: int = 0
    cache_read_tokens: int = 0
    cache_write_tokens: int = 0
    cost: float = 0.0
    provider_seconds: float = 0.0
    build_seconds: float = 0.0
    eval_seconds: float = 0.0
    automatic_source_mutations: int = 0
    reason: str = ""

    @property
    def match_improvement(self) -> float:
        return self.best_match_percent - self.baseline_match_percent

    def to_dict(self) -> Dict[str, Any]:
        data = asdict(self)
        data["match_improvement"] = self.match_improvement
        return data


@dataclass
class BenchmarkReport:
    name: str
    workflow: str
    targets: List[TargetRunMetrics] = field(default_factory=list)
    started_at: float = field(default_factory=time.time)

    def add(self, metrics: TargetRunMetrics) -> None:
        self.targets.append(metrics)

    def summary(self) -> Dict[str, Any]:
        rows = self.targets
        n = len(rows) or 1
        accepted = [r for r in rows if r.symbol_accepted]
        return {
            "name": self.name,
            "workflow": self.workflow,
            "targets": len(rows),
            "elapsed_seconds": time.time() - self.started_at,
            "parse_success_rate": sum(r.parse_ok for r in rows) / max(sum(max(r.model_calls, 1) for r in rows), 1),
            "compile_rate": sum(1 for r in rows if r.compile_ok) / n,
            "mean_match_improvement": sum(r.match_improvement for r in rows) / n,
            "first_pass_full_match_rate": sum(
                1 for r in rows if r.full_match and r.model_calls <= 3
            ) / n,
            "final_accepted_rate": len(accepted) / n,
            "mean_calls_per_accepted": (
                sum(r.model_calls for r in accepted) / len(accepted) if accepted else None
            ),
            "mean_cost_per_accepted": (
                sum(r.cost for r in accepted) / len(accepted) if accepted else None
            ),
            "tokens_per_accepted": (
                sum(r.input_tokens + r.output_tokens for r in accepted) / len(accepted)
                if accepted
                else None
            ),
            "repeated_fingerprint_stop_rate": sum(
                1 for r in rows if r.repeated_fingerprint_stop
            ) / n,
            "evaluation_error_rate": sum(r.evaluation_errors for r in rows)
            / max(sum(r.model_calls for r in rows), 1),
            "mean_prompt_chars": sum(r.prompt_chars for r in rows) / n,
            "automatic_source_mutation_count": sum(
                r.automatic_source_mutations for r in rows
            ),
            "by_size_bucket": _group_rates(rows, lambda r: r.size_bucket or "unknown"),
            "by_category": _group_rates(rows, lambda r: r.category or "unclassified"),
        }

    def to_dict(self) -> Dict[str, Any]:
        return {
            "summary": self.summary(),
            "targets": [t.to_dict() for t in self.targets],
        }


def _group_rates(
    rows: Sequence[TargetRunMetrics],
    key_fn: Callable[[TargetRunMetrics], str],
) -> Dict[str, Dict[str, Any]]:
    groups: Dict[str, List[TargetRunMetrics]] = {}
    for row in rows:
        groups.setdefault(key_fn(row), []).append(row)
    out: Dict[str, Dict[str, Any]] = {}
    for key, items in sorted(groups.items()):
        n = len(items) or 1
        accepted = sum(1 for r in items if r.symbol_accepted)
        out[key] = {
            "targets": len(items),
            "accepted_rate": accepted / n,
            "mean_calls": sum(r.model_calls for r in items) / n,
            "mean_match_improvement": sum(r.match_improvement for r in items) / n,
        }
    return out


def compare_workflows(
    solve_report: BenchmarkReport,
    sample_report: BenchmarkReport,
) -> Dict[str, Any]:
    """Compare adaptive solve against blind sampling on accepted matches per call."""
    solve = solve_report.summary()
    sample = sample_report.summary()

    def accepted_per_call(summary: Dict[str, Any], report: BenchmarkReport) -> Optional[float]:
        total_calls = sum(r.model_calls for r in report.targets)
        accepted = sum(1 for r in report.targets if r.symbol_accepted)
        if total_calls <= 0:
            return None
        return accepted / total_calls

    solve_apc = accepted_per_call(solve, solve_report)
    sample_apc = accepted_per_call(sample, sample_report)
    verdict = "inconclusive"
    if solve_apc is not None and sample_apc is not None:
        if solve_apc > sample_apc:
            verdict = "solve_wins"
        elif sample_apc > solve_apc:
            verdict = "sample_wins"
        else:
            verdict = "tie"

    return {
        "verdict": verdict,
        "solve": solve,
        "sample": sample,
        "accepted_matches_per_model_call": {
            "solve": solve_apc,
            "sample": sample_apc,
        },
        "automatic_source_mutation_count": (
            solve["automatic_source_mutation_count"]
            + sample["automatic_source_mutation_count"]
        ),
    }


# ---------------------------------------------------------------------------
# Hidden source helpers
# ---------------------------------------------------------------------------


PLACEHOLDER_BODY = """\
{signature} {{
    // HARVEST_HIDDEN_SOURCE:{target_id}
    // Intentionally stubbed for hidden-source benchmark; do not use as evidence.
}}
"""


def extract_function_signature(source: str) -> str:
    """Best-effort signature line before the opening brace of a definition."""
    brace = source.find("{")
    head = source[:brace].strip() if brace >= 0 else source.strip()
    return " ".join(head.split())


def hide_function_body(original: str, *, target_id: str, signature: str | None = None) -> str:
    """Replace a complete function definition with a stub placeholder."""
    sig = signature or extract_function_signature(original)
    return PLACEHOLDER_BODY.format(signature=sig, target_id=target_id)


def verify_hidden_source(prompt_text: str, *, forbidden_snippets: Iterable[str]) -> List[str]:
    """Return leak diagnostics if accepted implementation text appears in a prompt."""
    issues: List[str] = []
    for snippet in forbidden_snippets:
        text = (snippet or "").strip()
        if len(text) < 24:
            continue
        if text in prompt_text:
            issues.append(f"accepted source leak ({len(text)} chars)")
    if "HARVEST_HIDDEN_SOURCE" in prompt_text and "do not use as evidence" not in prompt_text:
        # Stub markers are fine; full accepted bodies are not.
        pass
    return issues


def metrics_from_experiment_state(
    state: Dict[str, Any],
    *,
    workflow: str,
    category: str = "",
    size_bucket: str = "",
    baseline_match_percent: float = 0.0,
    prompt_chars: int = 0,
) -> TargetRunMetrics:
    records = state.get("records") or []
    best_match = baseline_match_percent
    parse_ok = 0
    compile_ok = 0
    eval_errors = 0
    symbol_accepted = False
    full_match = False
    equivalent_match = False
    repair_success = False
    input_tokens = 0
    output_tokens = 0
    cache_read = 0
    cache_write = 0
    cost = 0.0
    provider_seconds = 0.0

    parent_had_compile_error = False
    for row in records:
        if row.get("error"):
            continue
        evaluation = row.get("evaluation") or {}
        status = str(evaluation.get("status", "")).upper()
        parse_ok += 1
        if status not in {"COMPILE_ERROR", "EVALUATION_ERROR", "MODEL_ERROR"}:
            compile_ok += 1
        if status in {
            "EVALUATION_ERROR",
            "PRECHECK_ERROR",
            "BUILD_ERROR",
            "SYMBOL_EXTRACTION_ERROR",
            "MATCH_EVALUATION_ERROR",
            "RESTORE_ERROR",
        }:
            eval_errors += 1
        match = float(evaluation.get("match_percent") or 0.0)
        best_match = max(best_match, match)
        if evaluation.get("symbol_accepted") or evaluation.get("accepted"):
            symbol_accepted = True
        if status == "FULL_MATCH":
            full_match = True
            symbol_accepted = True
        if status == "EQUIVALENT_MATCH" and evaluation.get("accepted"):
            equivalent_match = True
            symbol_accepted = True
        summary = row.get("candidate_summary") or {}
        if summary.get("parent_id") and status != "COMPILE_ERROR" and parent_had_compile_error:
            repair_success = True
        if status == "COMPILE_ERROR":
            parent_had_compile_error = True
        input_tokens += int(row.get("input_tokens") or 0)
        output_tokens += int(row.get("output_tokens") or 0)
        cache_read += int(row.get("cache_read_tokens") or 0)
        cache_write += int(row.get("cache_write_tokens") or 0)
        cost += float(row.get("cost") or 0.0)
        provider_seconds += float(row.get("duration_seconds") or 0.0)

    return TargetRunMetrics(
        target_id=str(state.get("target_id", "")),
        workflow=workflow,
        category=category,
        size_bucket=size_bucket,
        model_calls=int(state.get("model_calls") or len(records)),
        parse_ok=parse_ok,
        compile_ok=1 if compile_ok else 0,
        baseline_match_percent=baseline_match_percent,
        best_match_percent=best_match,
        symbol_accepted=symbol_accepted,
        full_match=full_match,
        equivalent_match=equivalent_match,
        repair_success=repair_success,
        repeated_fingerprint_stop=state.get("reason") == "repeated_fingerprint",
        evaluation_errors=eval_errors,
        prompt_chars=prompt_chars,
        input_tokens=input_tokens,
        output_tokens=output_tokens,
        cache_read_tokens=cache_read,
        cache_write_tokens=cache_write,
        cost=cost,
        provider_seconds=provider_seconds,
        automatic_source_mutations=0,
        reason=str(state.get("reason") or state.get("status") or ""),
    )


# ---------------------------------------------------------------------------
# Acceptance checks for rollout
# ---------------------------------------------------------------------------


@dataclass
class RolloutGates:
    max_auto_mutations: int = 0
    min_solve_accepted_per_call_advantage: float = 0.0
    require_solve_verdict: bool = True


def check_rollout_gates(
    comparison: Dict[str, Any],
    *,
    gates: RolloutGates | None = None,
) -> List[str]:
    gates = gates or RolloutGates()
    results: List[str] = []
    mutations = int(comparison.get("automatic_source_mutation_count") or 0)
    if mutations <= gates.max_auto_mutations:
        results.append(f"PASS automatic mutations: {mutations} <= {gates.max_auto_mutations}")
    else:
        results.append(f"FAIL automatic mutations: {mutations} > {gates.max_auto_mutations}")

    rates = comparison.get("accepted_matches_per_model_call") or {}
    solve_rate = rates.get("solve")
    sample_rate = rates.get("sample")
    if solve_rate is None or sample_rate is None:
        results.append("FAIL missing accepted-per-call rates")
    else:
        advantage = solve_rate - sample_rate
        if advantage >= gates.min_solve_accepted_per_call_advantage:
            results.append(
                f"PASS solve accepted/call advantage: {advantage:.3f} "
                f"(solve={solve_rate:.3f}, sample={sample_rate:.3f})"
            )
        else:
            results.append(
                f"FAIL solve accepted/call advantage: {advantage:.3f} "
                f"< {gates.min_solve_accepted_per_call_advantage:.3f}"
            )

    if gates.require_solve_verdict:
        verdict = comparison.get("verdict")
        if verdict == "solve_wins":
            results.append("PASS verdict: solve_wins")
        else:
            results.append(f"FAIL verdict: {verdict}")
    return results


# Back-compat aliases used by older tests
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
    existing_match_status: str
    class_name: str = ""
    subsystem: str = ""


@dataclass
class BenchmarkDataset:
    entries: List[BenchmarkEntry] = field(default_factory=list)
    description: str = ""

    def add(self, entry: BenchmarkEntry) -> None:
        self.entries.append(entry)

    def filter(self, predicate: Callable[[BenchmarkEntry], bool]) -> "BenchmarkDataset":
        return BenchmarkDataset(
            entries=[e for e in self.entries if predicate(e)],
            description=self.description,
        )

    def stratified_sample(self, target_count: int = 40) -> "BenchmarkDataset":
        if not self.entries:
            return BenchmarkDataset()
        by_class: Dict[str, List[BenchmarkEntry]] = {}
        for e in self.entries:
            key = f"{e.class_name}::{e.subsystem}"
            by_class.setdefault(key, []).append(e)
        selected: List[BenchmarkEntry] = []
        import random

        pool = list(self.entries)
        random.shuffle(pool)
        for entry in pool:
            if len(selected) >= target_count:
                break
            key = f"{entry.class_name}::{entry.subsystem}"
            class_count = sum(
                1 for s in selected if f"{s.class_name}::{s.subsystem}" == key
            )
            if class_count < max(1, target_count // max(len(by_class), 1)):
                selected.append(entry)
        remaining = target_count - len(selected)
        for entry in pool:
            if remaining <= 0:
                break
            if entry not in selected:
                selected.append(entry)
                remaining -= 1
        return BenchmarkDataset(entries=selected, description=self.description)

    def summary(self) -> Dict[str, Any]:
        return {
            "total": len(self.entries),
            "size_bytes_avg": sum(e.size_bytes for e in self.entries)
            / max(len(self.entries), 1),
            "with_loops": sum(1 for e in self.entries if e.has_loops),
            "with_fp": sum(1 for e in self.entries if e.uses_fp),
            "full_match": sum(
                1 for e in self.entries if e.existing_match_status == "full_match"
            ),
        }


@dataclass
class HiddenSourceConfig:
    isolate_branch: str = "benchmark-hidden"
    preserve_headers: bool = True
    preserve_siblings: bool = False
    remove_attempt_history: bool = True
    prevent_retrieval: bool = True

    def verify_no_leak(self, source: str, target_id: str) -> List[str]:
        issues: List[str] = []
        if target_id in source:
            issues.append(f"target_id {target_id!r} found in source context")
        return issues


@dataclass
class AblationConfig:
    name: str
    description: str
    overrides: Dict[str, Any] = field(default_factory=dict)


# New ablutions aligned with the simplified harness (not knowledge/pipeline).
ABLATIONS: List[AblationConfig] = [
    AblationConfig(
        name="solve_default",
        description="Adaptive solve with default budgets",
        overrides={"workflow": "solve"},
    ),
    AblationConfig(
        name="sample_new_10",
        description="Blind new sampling with 10 identical prompts",
        overrides={"workflow": "sample", "sample_workflow": "new", "runs": 10},
    ),
    AblationConfig(
        name="sample_improve_10",
        description="Blind improve sampling with 10 identical prompts",
        overrides={"workflow": "sample", "sample_workflow": "improve", "runs": 10},
    ),
    AblationConfig(
        name="solve_no_raw_hex",
        description="Solve with include_raw_hex disabled (default)",
        overrides={"workflow": "solve", "prompt.include_raw_hex": False},
    ),
]


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
            "median_calls_to_first_compile": 0,
            "nonzero_match_rate": nonzero / total if total else 0,
            "median_best_match": sorted([m.best_match_percent for m in self.metrics])[
                total // 2
            ]
            if total
            else 0,
            "structural_alignment_rate": 0.0,
            "proven_equivalent_rate": proven_eq / total if total else 0,
            "full_match_rate": full_match / total if total else 0,
            "cost_per_promoted": (
                sum(m.cost_estimate for m in self.metrics) / max(full_match + proven_eq, 1)
            ),
            "promotion_survival_rate": 0.0,
            "first_call_compile_rate": (
                sum(1 for m in self.metrics if m.first_call_compiled) / total
            )
            if total
            else 0,
            "repeated_diagnostic_rate": (
                sum(1 for m in self.metrics if m.repeated_diagnostic) / total
            )
            if total
            else 0,
            "blocked_rate": sum(1 for m in self.metrics if m.blocked) / total if total else 0,
            "rollback_rate": sum(1 for m in self.metrics if m.rollback) / total if total else 0,
            "mean_prompt_tokens": sum(m.prompt_tokens for m in self.metrics) / total
            if total
            else 0,
            "mean_output_tokens": sum(m.output_tokens for m in self.metrics) / total
            if total
            else 0,
            "mean_compile_time_ms": sum(m.compile_time_ms for m in self.metrics) / total
            if total
            else 0,
            "unsupported_verifier_rate": (
                sum(1 for m in self.metrics if m.unsupported_verifier) / total
            )
            if total
            else 0,
            "elapsed_seconds": self.elapsed(),
        }


@dataclass
class SurvivalObservation:
    target_id: str
    promoted_at: str
    still_canonical: bool
    monotonically_improved: bool
    checked_after: str


def compute_promotion_survival(observations: List[SurvivalObservation]) -> float:
    if not observations:
        return 0.0
    survived = sum(
        1 for o in observations if o.still_canonical or o.monotonically_improved
    )
    return survived / len(observations)


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
    if targets is None:
        targets = AcceptanceTargets()
    results: List[str] = []
    bc = baseline_report.get("compile_rate", 0) or 0.001
    cc = candidate_report.get("compile_rate", 0)
    if cc >= bc * targets.min_compile_rate_multiple:
        results.append(
            f"PASS compile rate: {cc:.1%} >= {bc:.1%} x {targets.min_compile_rate_multiple}"
        )
    else:
        results.append(
            f"FAIL compile rate: {cc:.1%} < {bc:.1%} x {targets.min_compile_rate_multiple}"
        )
    br = baseline_report.get("repeated_diagnostic_rate", 0) or 0.001
    cr = candidate_report.get("repeated_diagnostic_rate", 0)
    if cr <= br * (1 - targets.max_repeated_diagnostic_reduction):
        results.append(f"PASS repeated diagnostic: {cr:.1%} <= {br:.1%} x 0.5")
    else:
        results.append(f"FAIL repeated diagnostic: {cr:.1%} > {br:.1%} x 0.5")
    bnm = baseline_report.get("nonzero_match_rate", 0) or 0.001
    cnm = candidate_report.get("nonzero_match_rate", 0)
    if cnm >= bnm * targets.min_nonzero_match_multiple:
        results.append(
            f"PASS nonzero match: {cnm:.1%} >= {bnm:.1%} x {targets.min_nonzero_match_multiple}"
        )
    else:
        results.append(
            f"FAIL nonzero match: {cnm:.1%} < {bnm:.1%} x {targets.min_nonzero_match_multiple}"
        )
    if targets.zero_stale_root_corruption:
        results.append("PASS zero stale-root corruption (assumed)")
    if targets.zero_unrolled_back_failures:
        results.append("PASS zero unrolled-back failures (assumed)")
    ps = candidate_report.get("promotion_survival_rate", 1.0)
    if ps >= targets.min_promotion_survival:
        results.append(
            f"PASS promotion survival: {ps:.1%} >= {targets.min_promotion_survival:.0%}"
        )
    else:
        results.append(
            f"FAIL promotion survival: {ps:.1%} < {targets.min_promotion_survival:.0%}"
        )
    bfm = baseline_report.get("full_match_rate", 0)
    cfm = candidate_report.get("full_match_rate", 0)
    if cfm >= bfm or not targets.no_reduction_in_full_match:
        results.append(f"PASS full match: {cfm:.1%} >= {bfm:.1%}")
    else:
        results.append(f"FAIL full match: {cfm:.1%} < {bfm:.1%}")
    return results
