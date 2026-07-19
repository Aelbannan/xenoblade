"""Pipeline Orchestration — state machine tying all phases together.

See docs/decomp_work/07_pipeline_orchestration.md
"""
from __future__ import annotations

import hashlib
import json
import sys
import time
import uuid
from concurrent.futures import FIRST_COMPLETED, ThreadPoolExecutor, wait
from dataclasses import asdict
from datetime import datetime, timezone
from pathlib import Path
from typing import Any, Dict, List, Optional

from .promotion import PromotionManager, capture_baseline, evaluation_to_candidate
from .reconstruction import parse_reconstruction_response
from .types import (
    Candidate,
    CandidateEvaluation,
    Evaluation,
    ExperimentRecord,
    ExperimentState,
    ModelConfig,
    PipelineConfig,
    PipelineStage,
    PipelineStopOn,
    ProjectAdapter,
    ProviderResult,
)


def _debug(message: str) -> None:
    timestamp = datetime.now().astimezone().strftime("%H:%M:%S")
    print(f"[pipeline {timestamp}] {message}", file=sys.stderr, flush=True)


class PipelineRunner:
    """Full pipeline state machine chaining reconstruction → compile → structural
    → semantic → match-improve → equivalence → promotion.

    Each stage is checkpointed so the pipeline can be resumed from any point.
    """

    def __init__(
        self,
        adapter: ProjectAdapter,
        models: List[ModelConfig],
        providers: Dict[str, Any],
        promotion_manager: PromotionManager,
        config: PipelineConfig,
        output_dir: Path,
        max_parallel: int = 1,
    ) -> None:
        self.adapter = adapter
        self.models = models
        self.providers = providers
        self.promotion_manager = promotion_manager
        self.config = config
        self.output_dir = output_dir.resolve()
        self.max_parallel = max_parallel

    def _emit(self, event: str, experiment: ExperimentState, *,
              stage: str = "", status: str = "", detail: str = "",
              match_percent: float = 0.0, structural_score: float = 0.0,
              duration_seconds: float = 0.0) -> None:
        """Emit a structured pipeline event."""
        from .types import PipelineEvent
        ev = PipelineEvent(
            event=event,
            experiment_id=experiment.experiment_id,
            target_id=experiment.target_id,
            stage=stage or experiment.status.value,
            status=status,
            detail=detail,
            match_percent=match_percent,
            structural_score=structural_score,
            duration_seconds=duration_seconds,
            timestamp=datetime.now(timezone.utc).isoformat(),
        )
        json.dump(ev.to_dict(), sys.stderr)
        sys.stderr.write("\n")
        sys.stderr.flush()

    # ------------------------------------------------------------------
    # Top-level run
    # ------------------------------------------------------------------

    def run(
        self,
        target_id: str,
        experiment_dir: Optional[Path] = None,
    ) -> ExperimentState:
        if experiment_dir is None:
            experiment_id = (
                f"{datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%SZ')}"
                f"-{uuid.uuid4().hex[:8]}"
            )
            experiment_dir = self.output_dir / target_id / experiment_id
            experiment_dir.mkdir(parents=True, exist_ok=True)

        state_path = experiment_dir / "pipeline_state.json"
        if state_path.is_file():
            experiment = ExperimentState.load(state_path)
            _debug(
                f"resuming target={target_id} "
                f"status={experiment.status.value} "
                f"records={len(experiment.records)}"
            )
        else:
            experiment = ExperimentState(
                experiment_id=experiment_dir.name,
                workflow="new",
                target_id=target_id,
                status=PipelineStage.BASELINE,
                started_at=datetime.now(timezone.utc).isoformat(),
                budgets={
                    "reconstruct": self.config.budgets.reconstruct,
                    "compile_repair": self.config.budgets.compile_repair,
                    "semantic_repair": self.config.budgets.semantic_repair,
                    "match_improve": self.config.budgets.match_improve,
                },
            )
            _debug(f"starting pipeline target={target_id}")

        # §16.1 — Run each stage if not yet completed
        if experiment.status == PipelineStage.BASELINE:
            experiment = self._stage_baseline(target_id, experiment, experiment_dir)
            experiment.status = PipelineStage.RECONSTRUCT
            experiment.save(state_path)

        if experiment.status == PipelineStage.RECONSTRUCT:
            experiment = self._stage_reconstruct(
                target_id, experiment, experiment_dir,
            )
            experiment.status = PipelineStage.COMPILE_REPAIR
            experiment.save(state_path)

        if not self.config.features.staged_pipeline:
            # Legacy mode: skip post-reconstruction pipeline stages.
            # COMPILE_REPAIR still runs if applicable.
            if experiment.status == PipelineStage.COMPILE_REPAIR:
                experiment = self._stage_compile_repair(experiment, experiment_dir)
            experiment.status = PipelineStage.COMPLETE
            experiment.updated_at = datetime.now(timezone.utc).isoformat()
            experiment.save(state_path)
            _debug(
                f"pipeline completed (legacy) target={target_id} "
                f"records={len(experiment.records)}"
            )
            self._write_summary(experiment, experiment_dir)
            return experiment

        if experiment.status == PipelineStage.COMPILE_REPAIR:
            experiment = self._stage_compile_repair(experiment, experiment_dir)
            experiment.status = PipelineStage.STRUCTURAL_ANALYSIS
            experiment.save(state_path)

        if experiment.status == PipelineStage.STRUCTURAL_ANALYSIS:
            experiment = self._stage_structural_analysis(experiment, experiment_dir)
            experiment.status = PipelineStage.SEMANTIC_REPAIR
            experiment.save(state_path)

        if experiment.status == PipelineStage.SEMANTIC_REPAIR:
            experiment = self._stage_semantic_repair(experiment, experiment_dir)
            experiment.status = PipelineStage.MATCH_IMPROVE
            experiment.save(state_path)

        if experiment.status == PipelineStage.MATCH_IMPROVE:
            experiment = self._stage_match_improve(experiment, experiment_dir)
            experiment.status = PipelineStage.EQUIVALENCE
            experiment.save(state_path)

        if experiment.status == PipelineStage.EQUIVALENCE:
            experiment = self._stage_equivalence(experiment, experiment_dir)
            experiment.status = PipelineStage.PROMOTION
            experiment.save(state_path)

        if experiment.status == PipelineStage.PROMOTION:
            experiment = self._stage_promotion(experiment, experiment_dir)
            experiment.status = PipelineStage.COMPLETE
            experiment.updated_at = datetime.now(timezone.utc).isoformat()
            experiment.save(state_path)
            _debug(
                f"pipeline completed target={target_id} "
                f"status={experiment.status_detail or 'ok'} "
                f"records={len(experiment.records)}"
            )

        self._write_summary(experiment, experiment_dir)
        return experiment

    # ------------------------------------------------------------------
    # Stage: Baseline capture
    # ------------------------------------------------------------------

    def _stage_baseline(
        self,
        target_id: str,
        experiment: ExperimentState,
        experiment_dir: Path,
    ) -> ExperimentState:
        read_source = getattr(self.adapter, "read_target_source", None)
        source = read_source(target_id) if read_source else ""
        source_hash = hashlib.sha256(source.encode("utf-8")).hexdigest()

        # Build dossier
        history: List[Dict[str, Any]] = []
        build_dossier = getattr(self.adapter, "build_dossier", None)
        if build_dossier:
            dossier = build_dossier(target_id, history)
        else:
            dossier = {}

        # §8.1 — Evaluate canonical source
        baseline_evaluation: Optional[Dict[str, Any]] = None
        evaluate_canon = getattr(self.adapter, "evaluate_canon", None)
        if evaluate_canon:
            try:
                result = evaluate_canon(target_id, experiment_dir / "baseline")
                if hasattr(result, "__dataclass_fields__"):
                    baseline_evaluation = asdict(result)
                elif isinstance(result, dict):
                    baseline_evaluation = result
            except Exception:
                _debug(f"baseline evaluation failed target={target_id}")

        experiment.baseline = {
            "source_hash": source_hash,
            "source_text": source,
            "captured_at": datetime.now(timezone.utc).isoformat(),
            "evaluation": baseline_evaluation,
        }
        experiment.dossier = dossier
        _debug(f"baseline captured target={target_id} hash={source_hash[:12]}")
        self._emit("baseline_captured", experiment,
                    stage="baseline", detail=source_hash)
        return experiment

    # ------------------------------------------------------------------
    # Stage: Reconstruction (Phase 1 initial model calls)
    # ------------------------------------------------------------------

    def _stage_reconstruct(
        self,
        target_id: str,
        experiment: ExperimentState,
        experiment_dir: Path,
    ) -> ExperimentState:
        models = self.models
        if not models:
            _debug("no models configured for reconstruction")
            return experiment

        if not self.config.features.staged_pipeline:
            _debug("staged pipeline disabled by feature flag; running simple reconstruction")
            return experiment

        prompt = self._build_reconstruct_prompt(target_id, experiment)
        records = self._run_model_calls(
            target_id, experiment, models, prompt, experiment_dir,
            "reconstruct",
            stop_on=self.config.stop_on,
        )
        experiment.records.extend(records)

        # Select best from initial candidates
        best = self._select_best_record(records)
        if best:
            experiment.best_candidate = best.get("candidate_summary", {})
            experiment.best_evaluation = best.get("evaluation")

        _debug(
            f"reconstruction complete target={target_id} "
            f"candidates={len(records)} "
            f"best_match={experiment.best_evaluation.get('match_percent', 0.0) if experiment.best_evaluation else 0.0}%"
        )
        self._emit("reconstruction_complete", experiment,
                    stage="reconstruct",
                    detail=f"records={len(experiment.records)}")
        return experiment

    # ------------------------------------------------------------------
    # Stage: Compile repair (Phase 2)
    # ------------------------------------------------------------------

    def _stage_compile_repair(
        self,
        experiment: ExperimentState,
        experiment_dir: Path,
    ) -> ExperimentState:
        best_eval = experiment.best_evaluation or {}
        if best_eval.get("status") != "COMPILE_ERROR":
            _debug("compile repair skipped: best candidate does not have COMPILE_ERROR")
            return experiment

        budget = self.config.budgets.compile_repair
        _debug(
            f"compile repair starting target={experiment.target_id} budget={budget}"
        )

        # Use existing repair infrastructure: read repairs from Harness.repair()
        # output written to experiment_dir/repairs/
        repairs_dir = experiment_dir / "repairs"
        if not repairs_dir.is_dir():
            _debug("compile repair skipped: no repairs directory found")
            return experiment

        repair_files = sorted(repairs_dir.glob("repair-*.json"))
        if not repair_files:
            _debug("compile repair skipped: no repair artifacts")
            return experiment

        best_compile_status = "skipped"
        for rf in repair_files:
            try:
                data = json.loads(rf.read_text(encoding="utf-8"))
                ev = data.get("evaluation", {})
                cand = data.get("candidate", {})
                if not ev or not cand:
                    continue
                ev_status = ev.get("status", "")
                if ev_status == "COMPILE_ERROR":
                    continue
                experiment.best_candidate = {
                    "source": cand.get("source", ""),
                    "hypothesis": cand.get("hypothesis", ""),
                    "notes": cand.get("notes", []),
                }
                experiment.best_evaluation = ev
                best_compile_status = ev_status
                _debug(
                    f"compile repair result: "
                    f"status={ev_status} "
                    f"match={ev.get('match_percent')}%"
                )
                break
            except (json.JSONDecodeError, OSError):
                continue

        self._emit("compile_repair", experiment, status=best_compile_status)
        return experiment

    # ------------------------------------------------------------------
    # Stage: Structural analysis (Phase 3)
    # ------------------------------------------------------------------

    def _stage_structural_analysis(
        self,
        experiment: ExperimentState,
        experiment_dir: Path,
    ) -> ExperimentState:
        if not self.config.features.structural_routing:
            _debug("structural routing disabled by feature flag; skipping analysis")
            return experiment

        best_eval = experiment.best_evaluation or {}
        if not best_eval.get("metrics", {}).get("structural"):
            _debug("structural analysis skipped: no structural metrics available")
            return experiment

        structural = best_eval["metrics"]["structural"]
        total = structural.get("total_score", 0.0)
        threshold = self.config.structural.semantic_repair_below

        _debug(
            f"structural analysis target={experiment.target_id} "
            f"score={total:.3f} threshold={threshold}"
        )

        if total >= threshold:
            experiment.status_detail = (
                f"strong_structural_alignment:{total:.3f}"
            )
        else:
            experiment.status_detail = (
                f"weak_structural_alignment:{total:.3f}"
            )

        self._emit("structural_analysis", experiment,
                    stage="structural_analysis",
                    structural_score=total)
        return experiment

    # ------------------------------------------------------------------
    # Stage: Semantic repair (Phase 4)
    # ------------------------------------------------------------------

    def _stage_semantic_repair(
        self,
        experiment: ExperimentState,
        experiment_dir: Path,
    ) -> ExperimentState:
        status_detail = experiment.status_detail or ""
        if "weak_structural_alignment" not in status_detail:
            _debug("semantic repair skipped: structural alignment is adequate")
            return experiment

        _debug(
            f"semantic repair triggered for target={experiment.target_id} "
            f"status_detail={status_detail}"
        )
        _debug("semantic repair: route to run_semantic_repairs() with callbacks")
        # The callback-based run_semantic_repairs() accepts:
        #   branch, max_repairs, threshold,
        #   build_prompt_fn, call_model_fn, evaluate_fn, structural_fn
        # Construct these from the adapter + providers + models and call it.
        # For now, the pipeline records the routing decision.

        repairs_dir = experiment_dir / "semantic_repairs"
        repairs_dir.mkdir(exist_ok=True)
        routing = {
            "stage": "semantic_repair",
            "status_detail": status_detail,
            "budget": self.config.budgets.semantic_repair,
        }
        (repairs_dir / "routing.json").write_text(
            json.dumps(routing, indent=2) + "\n", encoding="utf-8"
        )

        return experiment

    # ------------------------------------------------------------------
    # Stage: Match improvement (Phase 5-6)
    # ------------------------------------------------------------------

    def _stage_match_improve(
        self,
        experiment: ExperimentState,
        experiment_dir: Path,
    ) -> ExperimentState:
        # §13.1 — Skip if structural alignment is weak;
        # semantic repair must run first.
        status_detail = experiment.status_detail or ""
        if "weak_structural_alignment" in status_detail:
            _debug(
                f"match improve skipped target={experiment.target_id}: "
                f"structural alignment is weak ({status_detail})"
            )
            return experiment

        best_eval = experiment.best_evaluation or {}
        match_percent = float(best_eval.get("match_percent", 0.0) or 0.0)

        if match_percent >= 100.0:
            _debug("match improve skipped: already at FULL_MATCH")
            return experiment

        _debug(
            f"match improve triggered target={experiment.target_id} "
            f"current_match={match_percent:.1f}% budget={self.config.budgets.match_improve}"
        )
        _debug("match improve: route to run_match_improve() with callbacks")
        # The callback-based run_match_improve() accepts:
        #   branch, budget, threshold,
        #   evaluate_fn, feedback_fn, prompt_fn, model_fn
        # Construct these from the adapter + providers + models and call it.
        # For now, the pipeline records the routing decision.

        improves_dir = experiment_dir / "match_improves"
        improves_dir.mkdir(exist_ok=True)
        routing = {
            "stage": "match_improve",
            "current_match_percent": match_percent,
            "budget": self.config.budgets.match_improve,
        }
        (improves_dir / "routing.json").write_text(
            json.dumps(routing, indent=2) + "\n", encoding="utf-8"
        )

        return experiment

    # ------------------------------------------------------------------
    # Stage: Equivalence check (Phase 6)
    # ------------------------------------------------------------------

    def _stage_equivalence(
        self,
        experiment: ExperimentState,
        experiment_dir: Path,
    ) -> ExperimentState:
        best_eval = experiment.best_evaluation or {}
        match_percent = float(best_eval.get("match_percent", 0.0) or 0.0)
        equivalence = best_eval.get("equivalence")

        _debug(
            f"equivalence check target={experiment.target_id} "
            f"match={match_percent:.1f}% equivalence={equivalence}"
        )

        if match_percent >= 100.0:
            experiment.status_detail = "full_match_achieved"
            _debug("equivalence: FULL_MATCH achieved")
        elif equivalence and "EQUIVALENT" in str(equivalence):
            experiment.status_detail = "equivalent_match_achieved"
            _debug("equivalence: EQUIVALENT_MATCH achieved")
        else:
            experiment.status_detail = (
                f"match_improved:{match_percent:.1f}%"
            )
            _debug(f"equivalence: no proof, best match={match_percent:.1f}%")

        self._emit("equivalence_check", experiment,
                    stage="equivalence", match_percent=match_percent)
        return experiment

    # ------------------------------------------------------------------
    # Stage: Promotion (§16.1 final step)
    # ------------------------------------------------------------------

    def _stage_promotion(
        self,
        experiment: ExperimentState,
        experiment_dir: Path,
    ) -> ExperimentState:
        # §16.2 — Recapture fresh baseline before promoting
        fresh_baseline = capture_baseline(
            self.adapter, experiment.target_id, experiment_dir,
        )
        fresh_eval: Optional[Dict[str, Any]] = None
        if fresh_baseline.evaluation is not None:
            if hasattr(fresh_baseline.evaluation, "__dataclass_fields__"):
                fresh_eval = asdict(fresh_baseline.evaluation)
            elif isinstance(fresh_baseline.evaluation, dict):
                fresh_eval = fresh_baseline.evaluation
        # Update experiment baseline with fresh snapshot
        experiment.baseline = {
            "source_hash": fresh_baseline.source_hash,
            "source_text": fresh_baseline.source_text,
            "captured_at": fresh_baseline.captured_at,
            "evaluation": fresh_eval,
        }
        _debug(
            f"fresh baseline captured target={experiment.target_id} "
            f"hash={fresh_baseline.source_hash[:12]}"
        )

        best_eval = experiment.best_evaluation or {}
        accepted = bool(best_eval.get("accepted"))
        status = str(best_eval.get("status", ""))

        if not accepted:
            _debug(
                f"promotion skipped target={experiment.target_id}: "
                f"candidate not accepted (status={status})"
            )
            return experiment

        if not self.config.promotion.enabled:
            _debug("promotion skipped: promotion disabled in config")
            return experiment

        _debug(
            f"promotion starting target={experiment.target_id} "
            f"status={status} match={best_eval.get('match_percent', 0.0)}%"
        )

        candidate_eval = evaluation_to_candidate(best_eval)
        baseline_data = experiment.baseline or {}
        baseline_eval_data = baseline_data.get("evaluation")
        baseline_eval: Optional[CandidateEvaluation] = None
        if baseline_eval_data:
            baseline_eval = evaluation_to_candidate(baseline_eval_data)

        promote_fn = getattr(self.adapter, "promote", None)
        if promote_fn is not None:
            candidate_source = ""
            candidate_summary = experiment.best_candidate or {}
            if candidate_summary:
                candidate_source = candidate_summary.get("source", "")

            try:
                result = self.promotion_manager.promote(
                    self.adapter,
                    experiment.workflow,
                    experiment.target_id,
                    candidate_source,
                    experiment_dir,
                    write=True,
                )
                experiment.promotion = {
                    "promoted": result.promoted,
                    "rolled_back": result.rolled_back,
                    "reason": result.reason,
                }
                _debug(
                    f"promotion result target={experiment.target_id} "
                    f"promoted={result.promoted} reason={result.reason}"
                )
            except Exception as exc:
                _debug(f"promotion failed: {exc}")
        else:
            _debug("promotion skipped: adapter has no promote method")

        prom = experiment.promotion or {}
        detail_str = (
            f"promoted={prom.get('promoted', False)},"
            f"rolled_back={prom.get('rolled_back', False)}"
        )
        self._emit("promotion_result", experiment,
                    stage="promotion", detail=detail_str)
        return experiment

    # ------------------------------------------------------------------
    # Helpers
    # ------------------------------------------------------------------

    def _build_reconstruct_prompt(
        self,
        target_id: str,
        experiment: ExperimentState,
    ) -> str:
        build_prompt = getattr(self.adapter, "build_prompt", None)
        if build_prompt:
            return build_prompt(
                "new",
                target_id,
                experiment.records,
                {"full_context": False},
            )
        dossier = experiment.dossier or {}
        return json.dumps(dossier, indent=2)

    def _run_model_calls(
        self,
        target_id: str,
        experiment: ExperimentState,
        models: List[ModelConfig],
        prompt: str,
        experiment_dir: Path,
        stage_label: str,
        stop_on: Optional[PipelineStopOn] = None,
    ) -> List[Dict[str, Any]]:
        specs = [
            (model, index)
            for model in models
            for index in range(1, model.runs + 1)
        ]
        if not specs:
            return []

        parallel = min(self.max_parallel, len(specs))
        records: List[Dict[str, Any]] = []

        if parallel > 1 and len(specs) > 1:
            with ThreadPoolExecutor(max_workers=parallel) as executor:
                pending = iter(specs)
                futures = {}

                def submit_next() -> None:
                    try:
                        m, idx = next(pending)
                    except StopIteration:
                        return
                    future = executor.submit(
                        self._run_one_model_call,
                        target_id, prompt, experiment_dir, m, idx, stage_label,
                    )
                    futures[future] = (m.id, idx)

                for _ in range(min(parallel, len(specs))):
                    submit_next()

                stop = False
                while futures:
                    done, _ = wait(futures, return_when=FIRST_COMPLETED)
                    for future in done:
                        record = future.result()
                        record_dict = record.to_json() if hasattr(record, 'to_json') else record
                        records.append(record_dict)
                        # §16.4 — Cancel on FULL_MATCH
                        if _record_is_full_match(record_dict):
                            stop = True
                        # §16.4 — Cancel on proved_equivalent when configured
                        if (
                            not stop
                            and stop_on
                            and stop_on.proved_equivalent
                            and _record_is_equivalent_match(record_dict)
                        ):
                            stop = True
                        del futures[future]
                    if stop:
                        for pending_future in list(futures):
                            if pending_future.cancel():
                                del futures[pending_future]
                    else:
                        for _ in done:
                            submit_next()
        else:
            for model, index in specs:
                record = self._run_one_model_call(
                    target_id, prompt, experiment_dir, model, index, stage_label,
                )
                record_dict = record.to_json() if hasattr(record, 'to_json') else record
                records.append(record_dict)
                if _record_is_full_match(record_dict):
                    break
                if (
                    stop_on
                    and stop_on.proved_equivalent
                    and _record_is_equivalent_match(record_dict)
                ):
                    break

        return records

    def _run_one_model_call(
        self,
        target_id: str,
        prompt: str,
        experiment_dir: Path,
        model: ModelConfig,
        index: int,
        stage_label: str,
    ) -> ExperimentRecord:
        provider = self.providers.get(model.provider)
        if provider is None:
            raise ValueError(f"Unsupported provider: {model.provider}")

        started = time.monotonic()
        error: Optional[str] = None
        evaluation: Dict[str, Any] = {}
        candidate_summary: Dict[str, Any] = {}
        result: Optional[ProviderResult] = None

        try:
            result = provider.invoke(
                prompt, model,
                experiment_dir / "context" if (experiment_dir / "context").is_dir() else experiment_dir,
            )
            candidate = parse_reconstruction_response(result.text)
            candidate_summary = {
                "source": candidate.source,
                "hypothesis": candidate.hypothesis,
                "notes": [],
            }

            ev = self.adapter.evaluate(
                "new",
                target_id,
                Candidate(
                    source=candidate.source,
                    hypothesis=candidate.hypothesis,
                    notes=candidate.notes,
                ),
            )
            if isinstance(ev, Evaluation):
                evaluation = asdict(ev)
            elif isinstance(ev, dict):
                evaluation = ev

            artifact_path = experiment_dir / f"{stage_label}-{model.id}-{index}.json"
            artifact_path.write_text(
                json.dumps({
                    "source": candidate.source,
                    "hypothesis": candidate.hypothesis,
                    "notes": candidate.notes,
                    "evaluation": evaluation,
                }, indent=2) + "\n",
                encoding="utf-8",
            )
        except Exception as exc:
            error = f"{type(exc).__name__}: {exc}"
            _debug(f"model call failed target={target_id} model={model.id} run={index}: {error}")

        return ExperimentRecord(
            schema_version=4,
            experiment_id=experiment_dir.name,
            timestamp=datetime.now(timezone.utc).isoformat(),
            workflow=stage_label,
            target_id=target_id,
            model_id=model.id,
            model=model.model,
            run_index=index,
            duration_seconds=time.monotonic() - started,
            input_tokens=result.input_tokens if result else None,
            output_tokens=result.output_tokens if result else None,
            cost=result.cost if result else None,
            evaluation=evaluation,
            artifact=str(
                (experiment_dir / f"{stage_label}-{model.id}-{index}.json").relative_to(self.output_dir.parent)
            ),
            candidate_summary=candidate_summary,
            error=error,
        )

    @staticmethod
    def _select_best_record(
        records: List[Dict[str, Any]],
    ) -> Optional[Dict[str, Any]]:
        successful = [r for r in records if not r.get("error")]
        if not successful:
            return None
        return max(
            successful,
            key=lambda r: (
                bool(r.get("evaluation", {}).get("accepted")),
                float(r.get("evaluation", {}).get("match_percent", 0.0) or 0.0),
            ),
        )

    def _write_summary(self, experiment: ExperimentState,
                       experiment_dir: Path) -> None:
        """Write experiment summary.md."""
        best_eval = experiment.best_evaluation or {}
        best_candidate = experiment.best_candidate or {}
        status_str = str(best_eval.get("status", ""))
        match_pct = float(best_eval.get("match_percent", 0.0) or 0.0)
        detail = best_eval.get("detail", "")
        structural_score = (
            best_eval.get("metrics", {})
            .get("structural", {})
            .get("total_score", 0.0)
        )
        accepted = bool(best_eval.get("accepted", False))

        total_calls = len(experiment.records)
        total_tokens = sum(
            (r.get("input_tokens") or 0) + (r.get("output_tokens") or 0)
            for r in experiment.records
        )
        total_cost = sum(r.get("cost") or 0.0 for r in experiment.records)

        summary_path = experiment_dir / "summary.md"
        lines = [
            "# Experiment summary",
            "",
            f"- Target: `{experiment.target_id}`",
            f"- Status: {experiment.status.value}",
            f"- Status detail: {experiment.status_detail or '—'}",
            f"- Started: {experiment.started_at}",
            f"- Updated: {experiment.updated_at}",
            "",
            "### Best candidate",
            "",
            f"- Status: {status_str}",
            f"- Match: {match_pct:.1f}%",
            f"- Accepted: {accepted}",
            f"- Structural score: {structural_score:.3f}",
            f"- Detail: {detail}",
            "",
            "### Records",
            "",
            f"- Total model calls: {total_calls}",
            f"- Total tokens: {total_tokens}",
            f"- Total cost: {total_cost:.6f}",
            "",
            "### Promotion",
            "",
        ]

        prom = experiment.promotion or {}
        if prom:
            lines.append(f"- Promoted: {prom.get('promoted', False)}")
            lines.append(f"- Rolled back: {prom.get('rolled_back', False)}")
            lines.append(f"- Reason: {prom.get('reason', '—')}")
        else:
            lines.append("- Not promoted")

        lines.append("")
        summary_path.write_text("\n".join(lines) + "\n", encoding="utf-8")
        _debug(f"summary written to {summary_path}")


def _record_is_full_match(record: Dict[str, Any]) -> bool:
    evaluation = record.get("evaluation") or {}
    return (
        not record.get("error")
        and evaluation.get("status") == "FULL_MATCH"
        and float(evaluation.get("match_percent") or 0.0) >= 100.0
        and bool(evaluation.get("accepted"))
    )


def _record_is_equivalent_match(record: Dict[str, Any]) -> bool:
    evaluation = record.get("evaluation") or {}
    return (
        not record.get("error")
        and evaluation.get("status") == "EQUIVALENT_MATCH"
        and bool(evaluation.get("equivalence"))
        and bool(evaluation.get("accepted"))
    )


def parse_config(config_data: Dict[str, Any]) -> PipelineConfig:
    raw_pipeline = config_data.get("pipeline", {})
    config = PipelineConfig.from_dict(raw_pipeline)
    errors = config.validate()
    if errors:
        raise ValueError(
            f"Pipeline config validation failed ({len(errors)} errors): "
            + "; ".join(errors)
        )
    return config
