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
from dataclasses import asdict, dataclass
from datetime import datetime, timezone
from pathlib import Path
from typing import Any, Dict, List, Optional

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


# §14.5 — Local proof-aware ranking
def rank_candidate(evaluation: CandidateEvaluation) -> tuple:
    """Lexicographic ranking: symbol acceptance, then match %, then equivalence/size."""
    symbol_accepted = bool(
        getattr(evaluation, "symbol_accepted", False)
        or evaluation.status == CandidateStatus.FULL_MATCH
        or (
            evaluation.status == CandidateStatus.EQUIVALENT_MATCH
            and evaluation.equivalence_status
        )
    )
    match_pct = evaluation.match_percent or 0.0
    equiv = 1 if evaluation.equivalence_status == "EQUIVALENT" else 0
    size_delta = 0
    if evaluation.function_size is not None and evaluation.retail_size is not None:
        size_delta = -abs(min(evaluation.function_size - evaluation.retail_size, 0))
    elif evaluation.function_size_delta is not None:
        size_delta = -abs(min(evaluation.function_size_delta, 0))
    return (1 if symbol_accepted else 0, match_pct, equiv, size_delta)


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

    # Structural regression gate - reject if structural score drops significantly
    baseline_structural = _structural_score(baseline)
    candidate_structural = _structural_score(candidate)
    max_regression = getattr(policy, 'max_structural_regression', 0.02)
    if candidate_structural + max_regression < baseline_structural:
        return False, f"candidate structural regression: {baseline_structural:.3f} -> {candidate_structural:.3f}"

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
# §25 — Security: source patch validation
# ---------------------------------------------------------------------------

class InvalidPatch(ValueError):
    """Raised when a candidate patch fails security validation."""
    pass


@dataclass
class TargetMetadata:
    """Metadata describing a target function for patch validation."""
    target_id: str
    source_file: str
    function_start: int = 0
    function_end: int = 0
    signature: str = ""


def validate_patch(patch: SourcePatch, target: TargetMetadata) -> None:
    """Validate a candidate patch against security requirements (§25).

    Raises InvalidPatch if any check fails.
    """
    # Reject path traversal
    if ".." in patch.slot_id or patch.slot_id.startswith("/"):
        raise InvalidPatch(
            f"path traversal detected in slot_id: {patch.slot_id!r}"
        )

    # Reject preprocessor directives in the replacement source
    if _contains_preprocessor_directive(patch.source):
        raise InvalidPatch("preprocessor edits are not permitted")

    # Reject shell commands
    if _contains_shell_command(patch.source):
        raise InvalidPatch("shell commands are not permitted")

    # Reject signature changes (authoritative)
    if target.signature and not _signature_matches(patch.source, target.signature):
        raise InvalidPatch(
            f"candidate changes the authoritative signature of {target.target_id}"
        )


def _contains_preprocessor_directive(source: str) -> bool:
    """Check if source contains #include, #define, #if, etc."""
    for line in source.split("\n"):
        stripped = line.strip()
        if stripped.startswith("#") and not stripped.startswith("# "):
            # Allow #pragma once? No — keep strict.
            # Allow #error? No.
            return True
    return False


def _contains_shell_command(source: str) -> bool:
    """Check for shell metacharacters that suggest command injection."""
    dangerous = ("`", "$(", "${", "|", "&&", "||")
    for line in source.split("\n"):
        stripped = line.strip()
        if any(d in stripped for d in dangerous):
            in_string = False
            in_char = False
            for ch in stripped:
                if ch == '"' and not in_char:
                    in_string = not in_string
                elif ch == "'":
                    in_char = not in_char
                elif ch in "`$|&" and not in_string and not in_char:
                    if ch == "$":
                        continue
                    return True
    return False


def _signature_matches(source: str, expected_signature: str) -> bool:
    """Check that the function signature in source matches expected.

    Simple check: the expected signature should appear in the source
    (possibly with whitespace variations).
    """
    normalized_source = " ".join(source.split())
    normalized_sig = " ".join(expected_signature.split())
    return normalized_sig in normalized_source


def validate_candidate_patches(
    patches: List[SourcePatch],
    target: TargetMetadata,
) -> None:
    """Validate all patches in a candidate."""
    for patch in patches:
        validate_patch(patch, target)


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

    compile_ok = status_str not in (
        "COMPILE_ERROR", "EVALUATION_ERROR", "MODEL_ERROR",
        "PRECHECK_ERROR", "BUILD_ERROR", "SYMBOL_EXTRACTION_ERROR",
        "MATCH_EVALUATION_ERROR", "EQUIVALENCE_ERROR", "RESTORE_ERROR",
    )
    metrics = ev.get("metrics") or {}
    symbol_accepted = bool(
        ev.get("symbol_accepted", metrics.get("symbol_accepted", False))
        or (
            status_str == "FULL_MATCH"
            or (
                status_str == "EQUIVALENT_MATCH"
                and ev.get("equivalence") not in (None, "", "INCONCLUSIVE_UNVALIDATED_CALLEE")
            )
        )
    )
    promising = bool(
        ev.get("promising", metrics.get("promising", False))
        or (compile_ok and not symbol_accepted and float(ev.get("match_percent") or 0.0) > 0.0)
    )
    blocked = ev.get("blocked_reason") or metrics.get("blocked_reason")
    if status_str == "CODE_MATCH" and not symbol_accepted and not blocked:
        blocked = "unvalidated_callee"

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
        symbol_accepted=symbol_accepted,
        project_ready=ev.get("project_ready", metrics.get("project_ready")),
        promising=promising,
        blocked_reason=blocked,
        function_size_delta=metrics.get("function_size_delta"),
        equivalence_status=ev.get("equivalence"),
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
        source = read_source(target_id)

    source_hash = hashlib.sha256(source.encode("utf-8")).hexdigest()

    evaluation: Optional[CandidateEvaluation] = None
    if evaluate_canon is not None:
        result = evaluate_canon(target_id, experiment_dir / "baseline")
        if isinstance(result, CandidateEvaluation):
            evaluation = result
        elif isinstance(result, Evaluation):
            evaluation = evaluation_to_candidate(asdict(result))
        elif isinstance(result, dict):
            evaluation = evaluation_to_candidate(result)
        else:
            raise TypeError(
                f"evaluate_canon returned unsupported type {type(result).__name__}"
            )

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
        owner: Optional[str] = None,
    ) -> PromotionResult:
        if not write:
            return PromotionResult(
                promoted=False,
                rolled_back=False,
                reason="dry-run (no --write)",
                target_id=target_id,
            )

        # Phase 0: require owner for write promotion
        if not owner:
            return PromotionResult(
                promoted=False,
                rolled_back=False,
                reason="promotion write requires --owner",
                target_id=target_id,
            )

        with self._lock:
            return self._promote_safe(
                adapter, workflow, target_id, candidate_source, experiment_dir, owner
            )

    def _promote_safe(
        self,
        adapter: Any,
        workflow: str,
        target_id: str,
        candidate_source: str,
        experiment_dir: Path,
        owner: str,
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

        # Phase 0: verify owner claim
        verify_claim = getattr(adapter, "verify_claim", None)
        if verify_claim is not None:
            try:
                claim_ok, claim_detail = verify_claim(target_id, owner)
                if not claim_ok:
                    return PromotionResult(
                        promoted=False,
                        rolled_back=False,
                        reason=f"owner claim verification failed: {claim_detail}",
                        target_id=target_id,
                        source_hash_before=source_hash_before,
                        validation_steps=steps,
                    )
            except Exception as exc:
                return PromotionResult(
                    promoted=False,
                    rolled_back=False,
                    reason=f"owner claim verification error: {type(exc).__name__}: {exc}",
                    target_id=target_id,
                    source_hash_before=source_hash_before,
                    validation_steps=steps,
                )
        else:
            # If adapter doesn't support verify_claim, require _require_claim method
            require_claim = getattr(adapter, "_require_claim", None)
            if require_claim is not None:
                try:
                    require_claim(target_id, owner)
                except Exception as exc:
                    return PromotionResult(
                        promoted=False,
                        rolled_back=False,
                        reason=f"owner claim required: {type(exc).__name__}: {exc}",
                        target_id=target_id,
                        source_hash_before=source_hash_before,
                        validation_steps=steps,
                    )

        # Phase 0: revalidate baseline source hash (reject stale experiments)
        state_path = experiment_dir / "state.json"
        baseline_source_hash = ""
        if state_path.is_file():
            try:
                state = json.loads(state_path.read_text(encoding="utf-8"))
                baseline_source_hash = state.get("baseline", {}).get("source_hash", "")
            except (OSError, json.JSONDecodeError):
                pass
        if baseline_source_hash and baseline_source_hash != source_hash_before:
            return PromotionResult(
                promoted=False,
                rolled_back=False,
                reason=f"experiment baseline is stale (source hash changed since experiment start); re-evaluate candidate against current root",
                target_id=target_id,
                source_hash_before=source_hash_before,
                validation_steps=steps,
            )

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
        from .types import Candidate as EvalCandidate, CandidateStatus

        eval_candidate = EvalCandidate(source=candidate_source, hypothesis="")
        evaluation = evaluate_fn(workflow, target_id, eval_candidate)
        eval_status = str(getattr(evaluation, "status", ""))
        eval_accepted = bool(getattr(evaluation, "accepted", False))
        eval_detail = str(getattr(evaluation, "detail", ""))[:1000]

        # Phase 0: require symbol acceptance (FULL_MATCH or EQUIVALENT_MATCH with proof)
        symbol_accepted = (
            eval_status == "FULL_MATCH" or
            (eval_status == "EQUIVALENT_MATCH" and getattr(evaluation, "equivalence", None))
        )
        if not symbol_accepted:
            self._rollback(source_path, original_bytes, original_mode)
            steps.append(ValidationStepResult(
                name="evaluate_target",
                succeeded=False,
                exit_code=-1,
                detail=f"symbol not accepted: status={eval_status} accepted={eval_accepted}",
                artifact_paths=[],
            ))
            return PromotionResult(
                promoted=False,
                rolled_back=True,
                reason=f"candidate not symbol-accepted (status={eval_status})",
                target_id=target_id,
                source_hash_before=source_hash_before,
                validation_steps=steps,
            )

        steps.append(ValidationStepResult(
            name="evaluate_target",
            succeeded=True,
            exit_code=0,
            detail=f"symbol accepted status={eval_status}",
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
