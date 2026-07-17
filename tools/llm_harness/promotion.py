"""Phase 0 — Promotion Safety.

Baseline capture, candidate ranking, promotion gate, placeholder detection,
transactional promotion with revalidation, rollback, and promotion journal.

See docs/decomp_work/01_phase0_promotion_safety.md
"""
from __future__ import annotations

import hashlib
import json
import os
import stat
import subprocess
import sys
import threading
from contextlib import contextmanager
from dataclasses import asdict
from datetime import datetime, timezone
from pathlib import Path
from typing import Any, Dict, List, Optional

from .ranking import (
    ProofSimilarity,
    extract_proof_similarity,
    rank_candidate as rank_candidate_proof_aware,
)
from .types import (
    BaselineSnapshot,
    CandidateEvaluation,
    CandidateStatus,
    CompileReport,
    Evaluation,
    PromotionPolicy,
    PromotionResult,
    SourcePatch,
    ValidationStepResult,
)


# §14.5 — Proof-aware ranking delegates to ranking module
def rank_candidate(evaluation: CandidateEvaluation) -> tuple:
    """Lexicographic ranking: hard priorities, then proof-aware tiers."""
    return rank_candidate_proof_aware(evaluation)


# ---------------------------------------------------------------------------
# 8.4 — Promotion gate
# ---------------------------------------------------------------------------

def passes_promotion_gate(
    baseline: CandidateEvaluation,
    candidate: CandidateEvaluation,
    policy: PromotionPolicy,
    baseline_is_placeholder: bool,
) -> tuple[bool, str]:
    """Return (pass, reason) — whether *candidate* may replace *baseline*."""
    if not policy.enabled:
        return False, "automatic promotion is disabled"

    if candidate.object_regressions:
        return False, "candidate has object regressions"

    if candidate.accepted_function_regressions:
        return False, "candidate regresses accepted functions"

    candidate_rank = rank_candidate(candidate)
    baseline_rank = rank_candidate(baseline)

    # Full match always promoted
    if candidate.status == CandidateStatus.FULL_MATCH:
        return True, "full match"

    # Proved-equivalent candidate must improve rank
    if candidate.status == CandidateStatus.EQUIVALENT_MATCH:
        if candidate_rank > baseline_rank:
            return True, "proved equivalent and rank improved"
        return False, "equivalent candidate did not improve canonical rank"

    # First ever compile from a non-compiling baseline
    if candidate.compile_report.succeeded and not baseline.compile_report.succeeded:
        score = _structural_score(candidate)
        if policy.allow_first_compile and score >= policy.first_compile_min_structural_score:
            return True, "first compiling structurally plausible implementation"

    # Replace a recognized placeholder
    if baseline_is_placeholder and candidate.compile_report.succeeded:
        score = _structural_score(candidate)
        if score >= policy.first_compile_min_structural_score:
            return True, "replaces placeholder with plausible compiling implementation"

    # Monotonic rank gate
    if policy.require_monotonic_rank and candidate_rank <= baseline_rank:
        return False, "candidate does not improve canonical rank"

    return True, "candidate improves canonical rank"


def _structural_score(evaluation: CandidateEvaluation) -> float:
    if evaluation.structural_report is None:
        return 0.0
    return evaluation.structural_report.total_score


# ---------------------------------------------------------------------------
# 8.5 — Placeholder detection
# ---------------------------------------------------------------------------

class PlaceholderDetector:
    """Heuristic and metadata-driven placeholder-source detection."""

    def is_placeholder(self, source: str, metadata: Optional[Dict[str, Any]] = None) -> bool:
        metadata = metadata or {}
        if metadata.get("placeholder") is True:
            return True

        normalized = self._normalize(source)
        known_patterns = (
            "return 0;",
            "return false;",
            "{}",
            "abort();",
            "UNIMPLEMENTED",
        )
        return any(pattern in normalized for pattern in known_patterns)

    @staticmethod
    def _normalize(source: str) -> str:
        lines = source.split("\n")
        cleaned: List[str] = []
        for line in lines:
            comment = line.find("//")
            cleaned.append(line if comment < 0 else line[:comment])
        normalized = " ".join(cleaned)
        while "/*" in normalized:
            start = normalized.index("/*")
            end = normalized.index("*/", start + 2)
            normalized = normalized[:start] + normalized[end + 2:]
        return " ".join(normalized.split())


# ---------------------------------------------------------------------------
# Conversion helpers
# ---------------------------------------------------------------------------

def evaluation_to_candidate(ev: Dict[str, Any]) -> CandidateEvaluation:
    """Convert a legacy ``Evaluation`` dict to ``CandidateEvaluation`` for ranking."""
    status_str = str(ev.get("status", "")).upper()
    try:
        cstatus = CandidateStatus(status_str.lower())
    except ValueError:
        cstatus = CandidateStatus.COMPILES if ev.get("accepted") else CandidateStatus.BLOCKED

    compile_ok = status_str not in ("COMPILE_ERROR", "EVALUATION_ERROR", "MODEL_ERROR")
    metrics = ev.get("metrics") or {}

    return CandidateEvaluation(
        status=cstatus,
        compile_report=CompileReport(
            succeeded=compile_ok,
            exit_code=0 if compile_ok else -1,
        ),
        match_percent=float(ev.get("match_percent") or 0.0),
        function_size=metrics.get("candidate_function_size"),
        retail_size=metrics.get("retail_function_size"),
        object_regressions=metrics.get("object_regressions", []),
        accepted_function_regressions=metrics.get("accepted_function_regressions", []),
    )


# ---------------------------------------------------------------------------
# 8.1 — Baseline snapshot
# ---------------------------------------------------------------------------

def capture_baseline(
    adapter: Any,
    target_id: str,
    experiment_dir: Path,
) -> BaselineSnapshot:
    """Read and store the current canonical target source + evaluation."""
    read_source = getattr(adapter, "read_target_source", None)
    evaluate_canon = getattr(adapter, "evaluate_canon", None)

    source = ""
    if read_source is not None:
        try:
            source = read_source(target_id)
        except Exception:
            pass

    source_hash = hashlib.sha256(source.encode("utf-8")).hexdigest()

    evaluation: Optional[CandidateEvaluation] = None
    if evaluate_canon is not None:
        try:
            result = evaluate_canon(target_id, experiment_dir / "baseline")
            if isinstance(result, CandidateEvaluation):
                evaluation = result
            elif isinstance(result, Evaluation):
                evaluation = evaluation_to_candidate(asdict(result) if hasattr(result, "__dataclass_fields__") else {})
        except Exception:
            pass

    return BaselineSnapshot(
        target_id=target_id,
        source_text=source,
        source_hash=source_hash,
        evaluation=evaluation,
        captured_at=datetime.now(timezone.utc).isoformat(),
    )


# ---------------------------------------------------------------------------
# 8.8 — Transactional source write
# ---------------------------------------------------------------------------

@contextmanager
def source_transaction(path: Path):
    """Atomic-write context manager: restore original bytes on exception."""
    original = path.read_bytes()
    original_mode = path.stat().st_mode
    try:
        yield
    except BaseException:
        _atomic_write_bytes(path, original)
        os.chmod(path, stat.S_IMODE(original_mode))
        raise


def _atomic_write_bytes(path: Path, data: bytes) -> None:
    tmp = path.with_suffix(path.suffix + ".tmp")
    tmp.write_bytes(data)
    tmp.replace(path)


# ---------------------------------------------------------------------------
# 8.6, 8.7, 8.9, 8.10 — Promotion manager
# ---------------------------------------------------------------------------

class PromotionManager:
    """Phase 0 promotion safety: baseline capture, ranking, gating, rollback."""

    def __init__(
        self,
        root: Path,
        policy: Optional[PromotionPolicy] = None,
    ) -> None:
        self._root = root.resolve()
        self._policy = policy or PromotionPolicy()
        self._lock = threading.Lock()
        self._detector = PlaceholderDetector()

    @property
    def policy(self) -> PromotionPolicy:
        return self._policy

    # ------------------------------------------------------------------
    # Ranking and gate
    # ------------------------------------------------------------------

    def rank(self, evaluation: CandidateEvaluation) -> tuple:
        return rank_candidate(evaluation)

    def compare_for_promotion(
        self,
        baseline: CandidateEvaluation,
        candidate: CandidateEvaluation,
        baseline_source: str = "",
        baseline_metadata: Optional[Dict[str, Any]] = None,
    ) -> tuple[bool, str]:
        baseline_is_placeholder = self._detector.is_placeholder(
            baseline_source, baseline_metadata
        )
        return passes_promotion_gate(baseline, candidate, self._policy, baseline_is_placeholder)

    # ------------------------------------------------------------------
    # Transactional promotion with validation steps
    # ------------------------------------------------------------------

    def promote(
        self,
        adapter: Any,
        workflow: str,
        target_id: str,
        candidate_source: str,
        experiment_dir: Path,
        *,
        write: bool = False,
    ) -> PromotionResult:
        if not write:
            return PromotionResult(
                promoted=False,
                rolled_back=False,
                reason="dry-run (no --write)",
                target_id=target_id,
            )

        with self._lock:
            return self._promote_safe(
                adapter, workflow, target_id, candidate_source, experiment_dir
            )

    def _promote_safe(
        self,
        adapter: Any,
        workflow: str,
        target_id: str,
        candidate_source: str,
        experiment_dir: Path,
    ) -> PromotionResult:
        steps: List[ValidationStepResult] = []
        read_source = getattr(adapter, "read_target_source", None)
        target_source_path = getattr(adapter, "target_source_path", None)
        promote_fn = getattr(adapter, "promote", None)
        evaluate_fn = getattr(adapter, "evaluate", None)
        build_fn = getattr(adapter, "_build_object", None)
        rank_fn = getattr(adapter, "rank_candidate", None)

        if read_source is None or promote_fn is None or evaluate_fn is None:
            return PromotionResult(
                promoted=False,
                rolled_back=False,
                reason="adapter does not support required promotion methods",
                target_id=target_id,
            )

        # Determine the source file path
        source_path: Optional[Path] = None
        if target_source_path is not None:
            try:
                source_path = target_source_path(target_id)
            except Exception:
                pass

        # 1. Precondition: hash current source
        source_before = read_source(target_id)
        source_hash_before = hashlib.sha256(source_before.encode("utf-8")).hexdigest()
        steps.append(ValidationStepResult(
            name="source_hash_precondition",
            succeeded=True,
            exit_code=0,
            detail=f"source hash: {source_hash_before[:12]}",
            artifact_paths=[str(source_path)] if source_path else [],
        ))

        # Save original bytes for rollback
        original_bytes = source_path.read_bytes() if source_path and source_path.is_file() else None
        original_mode = source_path.stat().st_mode if source_path and source_path.is_file() else None

        # 2. Apply candidate
        from .types import Candidate, SourcePatch

        candidate = Candidate(
            source=candidate_source if workflow != "tu-complete" else "",
            hypothesis="promotion from harness winner",
            patches=[SourcePatch(slot_id="source", source=candidate_source)]
            if workflow == "tu-complete" and candidate_source
            else [],
        )
        promote_fn(workflow, target_id, candidate, write=True)

        steps.append(ValidationStepResult(
            name="apply_candidate",
            succeeded=True,
            exit_code=0,
            detail="candidate written to canonical source",
            artifact_paths=[str(source_path)] if source_path else [],
        ))

        # 3. Build owning object
        build_ok = True
        build_detail = ""
        if build_fn is not None:
            try:
                build_error = build_fn(experiment_dir)
                if build_error:
                    build_ok = False
                    build_detail = str(build_error)[:2000]
            except Exception as exc:
                build_ok = False
                build_detail = f"{type(exc).__name__}: {exc}"

        if not build_ok:
            self._rollback(source_path, original_bytes, original_mode)
            steps.append(ValidationStepResult(
                name="compile_owning_object",
                succeeded=False,
                exit_code=-1,
                detail=build_detail or "build failed",
                artifact_paths=[],
            ))
            return PromotionResult(
                promoted=False,
                rolled_back=True,
                reason=f"build failed: {build_detail}",
                target_id=target_id,
                source_hash_before=source_hash_before,
                validation_steps=steps,
            )

        steps.append(ValidationStepResult(
            name="compile_owning_object",
            succeeded=True,
            exit_code=0,
            detail=build_detail or "build succeeded",
            artifact_paths=[],
        ))

        # 4. Re-evaluate target
        from .types import Candidate as EvalCandidate

        eval_candidate = EvalCandidate(source=candidate_source, hypothesis="")
        evaluation = evaluate_fn(workflow, target_id, eval_candidate)
        eval_accepted = bool(getattr(evaluation, "accepted", False))
        eval_detail = str(getattr(evaluation, "detail", ""))[:1000]

        steps.append(ValidationStepResult(
            name="evaluate_target",
            succeeded=eval_accepted,
            exit_code=0 if eval_accepted else -1,
            detail=f"accepted={eval_accepted} {eval_detail}",
            artifact_paths=[],
        ))

        # 5. Verify target rank improvement
        baseline_source_hash = ""
        baseline_eval = None
        state_path = experiment_dir / "state.json"
        if state_path.is_file():
            try:
                state = json.loads(state_path.read_text(encoding="utf-8"))
                baseline_source_hash = state.get("baseline", {}).get("source_hash", "")
                bl = state.get("baseline", {}).get("evaluation")
                if bl:
                    baseline_eval = evaluation_to_candidate(bl)
            except (OSError, json.JSONDecodeError):
                pass

        eval_dict = asdict(evaluation) if hasattr(evaluation, "__dataclass_fields__") else {}
        candidate_eval = evaluation_to_candidate(eval_dict)

        rank_improved = True
        if baseline_eval is not None:
            candidate_rank = self.rank(candidate_eval)
            baseline_rank = self.rank(baseline_eval)
            rank_improved = candidate_rank > baseline_rank

        steps.append(ValidationStepResult(
            name="verify_target_rank",
            succeeded=rank_improved,
            exit_code=0 if rank_improved else -1,
            detail=f"rank improved={rank_improved}",
            artifact_paths=[],
        ))

        if not eval_accepted or not rank_improved:
            # Rollback: restore original source and rebuild
            self._rollback(source_path, original_bytes, original_mode)
            steps.append(ValidationStepResult(
                name="verify_protected_functions",
                succeeded=False,
                exit_code=-1,
                detail="verification failed; source rolled back",
                artifact_paths=[],
            ))
            return PromotionResult(
                promoted=False,
                rolled_back=True,
                reason=f"verification failed: accepted={eval_accepted} rank_improved={rank_improved}",
                target_id=target_id,
                source_hash_before=source_hash_before,
                validation_steps=steps,
            )

        # 6. Verify protected functions in the same TU (if supported)
        check_protected = getattr(adapter, "check_protected_functions", None)
        if check_protected is not None:
            try:
                protected_ok, protected_detail = check_protected(target_id)
            except Exception:
                protected_ok, protected_detail = False, "exception during check"

            if not protected_ok:
                self._rollback(source_path, original_bytes, original_mode)
                steps.append(ValidationStepResult(
                    name="verify_protected_functions",
                    succeeded=False,
                    exit_code=-1,
                    detail=protected_detail or "protected function regression",
                    artifact_paths=[],
                ))
                return PromotionResult(
                    promoted=False,
                    rolled_back=True,
                    reason=f"protected function regression: {protected_detail}",
                    target_id=target_id,
                    source_hash_before=source_hash_before,
                    validation_steps=steps,
                )

            steps.append(ValidationStepResult(
                name="verify_protected_functions",
                succeeded=True,
                exit_code=0,
                detail=protected_detail or "all protected functions preserved",
                artifact_paths=[],
            ))

        # 7. Run coop cycle integration
        cycle_ok = False
        cycle_detail = ""
        try:
            cycle_result = subprocess.run(
                [sys.executable, "tools/coop/run.py", "cycle", target_id,
                 "--hypothesis", "Phase 0 transactional promotion",
                 "--next-change", "None"],
                cwd=self._root,
                text=True, capture_output=True, timeout=600,
            )
            cycle_ok = cycle_result.returncode == 0
            cycle_lines = cycle_result.stdout.strip().split("\n")[-5:]
            cycle_detail = " ".join(l.strip() for l in cycle_lines if l.strip())[:500]
        except subprocess.TimeoutExpired:
            cycle_detail = "cycle timed out"
        except Exception as exc:
            cycle_detail = f"{type(exc).__name__}: {exc}"

        if not cycle_ok:
            self._rollback(source_path, original_bytes, original_mode)
        steps.append(ValidationStepResult(
            name="coop_cycle",
            succeeded=cycle_ok,
            exit_code=0 if cycle_ok else -1,
            detail=cycle_detail or ("passed" if cycle_ok else "failed"),
            artifact_paths=[],
        ))

        if not cycle_ok:
            return PromotionResult(
                promoted=False,
                rolled_back=True,
                reason=f"coop cycle failed: {cycle_detail}",
                target_id=target_id,
                source_hash_before=source_hash_before,
                validation_steps=steps,
            )

        # 8. Postcondition: confirm source hash
        source_after = read_source(target_id)
        source_hash_after = hashlib.sha256(source_after.encode("utf-8")).hexdigest()
        steps.append(ValidationStepResult(
            name="source_hash_postcondition",
            succeeded=True,
            exit_code=0,
            detail=f"source hash: {source_hash_after[:12]}",
            artifact_paths=[str(source_path)] if source_path else [],
        ))

        # 9. Write promotion journal
        result = PromotionResult(
            promoted=True,
            rolled_back=False,
            reason="promotion completed with all validation steps passing",
            target_id=target_id,
            source_hash_before=source_hash_before,
            source_hash_after=source_hash_after,
            baseline_rank=(
                self.rank(baseline_eval) if baseline_eval is not None else ()
            ),
            candidate_rank=self.rank(candidate_eval),
            validation_steps=steps,
        )
        self._write_journal(result, experiment_dir)
        return result

    # ------------------------------------------------------------------
    # Rollback
    # ------------------------------------------------------------------

    @staticmethod
    def _rollback(
        path: Optional[Path],
        original_bytes: Optional[bytes],
        original_mode: Optional[int],
    ) -> bool:
        if path is None or original_bytes is None:
            return False
        try:
            path.write_bytes(original_bytes)
            if original_mode is not None:
                os.chmod(path, stat.S_IMODE(original_mode))
            return True
        except OSError:
            return False

    # ------------------------------------------------------------------
    # 8.10 — Promotion journal
    # ------------------------------------------------------------------

    def _write_journal(self, result: PromotionResult, experiment_dir: Path) -> None:
        journal_dir = self._root / "build" / "llm-harness" / "promotions"
        today = datetime.now(timezone.utc).strftime("%Y-%m-%d")
        journal_dir = journal_dir / today
        journal_dir.mkdir(parents=True, exist_ok=True)

        promotion_id = (
            f"prom-{datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%SZ')}"
        )
        journal = {
            "schema_version": 1,
            "promotion_id": promotion_id,
            "target_id": result.target_id,
            "baseline_source_hash": result.source_hash_before,
            "candidate_source_hash": result.source_hash_after or "",
            "baseline_rank": list(result.baseline_rank),
            "candidate_rank": list(result.candidate_rank),
            "reason": result.reason,
            "promoted": result.promoted,
            "rolled_back": result.rolled_back,
            "validation_steps": [
                {
                    "name": step.name,
                    "succeeded": step.succeeded,
                    "exit_code": step.exit_code,
                    "detail": step.detail,
                }
                for step in result.validation_steps
            ],
            "timestamp": datetime.now(timezone.utc).isoformat(),
        }

        path = journal_dir / f"{promotion_id}.json"
        path.write_text(json.dumps(journal, indent=2) + "\n", encoding="utf-8")
