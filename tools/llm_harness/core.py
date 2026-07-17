from __future__ import annotations

import importlib
import importlib.util
import inspect
import json
import re
import subprocess
import sys
import threading
import time
import uuid
from concurrent.futures import FIRST_COMPLETED, ThreadPoolExecutor, as_completed, wait
from dataclasses import asdict
from datetime import datetime, timezone
from pathlib import Path
from typing import Any, Dict, List, Optional

from .providers import OpenCodeProvider, ReasonixProvider
from .types import Candidate, ExperimentRecord, ModelConfig, ProjectAdapter, SourcePatch
from .workspace import GitWorktreeManager


def _debug_value(value: Any) -> str:
    """Keep progress messages single-line and bounded when errors contain paths/output."""
    return " ".join(str(value).split())[:300]


class Harness:
    def __init__(self, config_path: Path) -> None:
        self.config_path = config_path.resolve()
        self.config = json.loads(self.config_path.read_text(encoding="utf-8"))
        self.root = self.config_path.parent.resolve()
        self.output_dir = (self.root / self.config.get("output_dir", "build/llm-harness")).resolve()
        self.log_path = self.output_dir / "experiments.jsonl"
        self.models, self.workflow_models = self._load_models(
            self.config.get("models", [])
        )
        self.adapter = self._load_adapter(self.root)
        execution = self.config.get("execution", {})
        self.max_parallel = int(execution.get("max_parallel", 1))
        self.max_target_parallel = int(execution.get("max_target_parallel", 1))
        self.batch_model_parallel = int(execution.get("batch_model_parallel", 1))
        self.workflow_execution = self._load_execution_pipelines(
            execution.get("pipelines", {})
        )
        self.max_retries = int(execution.get("max_retries", 1))
        self._adapter_lock = threading.RLock()
        self._log_lock = threading.Lock()
        self._debug_lock = threading.Lock()
        isolation = execution.get("isolation", {})
        self.isolation_mode = isolation.get("mode", "none")
        self.workspace_manager = (
            GitWorktreeManager(self.root, copy_dirty=bool(isolation.get("copy_dirty", True)))
            if self.isolation_mode == "git-worktree"
            else None
        )
        _PROVIDER_CLASSES = {
            "opencode": OpenCodeProvider,
            "reasonix": ReasonixProvider,
        }
        self.providers = {}
        for name, cfg in self.config.get("providers", {}).items():
            cls = _PROVIDER_CLASSES.get(name)
            if cls is None:
                continue
            self.providers[name] = cls(
                binary=cfg.get("binary", name),
                timeout_seconds=int(cfg.get("timeout_seconds", 900)),
                pure=bool(cfg.get("pure", True)),
            )

    def _load_adapter(self, root: Path) -> ProjectAdapter:
        ref = self.config["project_adapter"]
        if ref.endswith(".py"):
            path = (root / ref).resolve()
            spec = importlib.util.spec_from_file_location("llm_harness_project_adapter", path)
            if spec is None or spec.loader is None:
                raise ImportError(f"Cannot load project adapter: {path}")
            module = importlib.util.module_from_spec(spec)
            spec.loader.exec_module(module)
        else:
            module = importlib.import_module(ref)
        return module.create_adapter(root, self.config.get("project", {}))

    @staticmethod
    def _load_models(
        configured: Any,
    ) -> tuple[List[ModelConfig], Dict[str, List[ModelConfig]]]:
        if isinstance(configured, list):
            models = [ModelConfig(**row) for row in configured]
            if not models:
                raise ValueError("Harness config must define at least one model")
            return models, {}
        if not isinstance(configured, dict):
            raise ValueError("Harness config 'models' must be a list or object")

        allowed = {"default", "new", "improve", "tu-complete"}
        unknown = sorted(set(configured) - allowed)
        if unknown:
            raise ValueError(f"Unknown models pipeline(s): {', '.join(unknown)}")
        pipelines: Dict[str, List[ModelConfig]] = {}
        for workflow, rows in configured.items():
            if not isinstance(rows, list):
                raise ValueError(f"Harness models.{workflow} must be a list")
            pipelines[workflow] = [ModelConfig(**row) for row in rows]
        if not any(pipelines.values()):
            raise ValueError("Harness config must define at least one model")
        return pipelines.get("default", []), pipelines

    def models_for_workflow(self, workflow: str) -> List[ModelConfig]:
        models = self.workflow_models.get(workflow, self.models)
        if not models:
            raise ValueError(
                f"Harness config must define models.{workflow} or models.default"
            )
        return models

    @staticmethod
    def _load_execution_pipelines(configured: Any) -> Dict[str, Dict[str, int]]:
        if not isinstance(configured, dict):
            raise ValueError("Harness execution.pipelines must be an object")
        allowed_workflows = {"new", "improve", "tu-complete"}
        unknown_workflows = sorted(set(configured) - allowed_workflows)
        if unknown_workflows:
            raise ValueError(
                f"Unknown execution pipeline(s): {', '.join(unknown_workflows)}"
            )
        allowed_options = {
            "max_parallel",
            "max_target_parallel",
            "batch_model_parallel",
        }
        pipelines: Dict[str, Dict[str, int]] = {}
        for workflow, options in configured.items():
            if not isinstance(options, dict):
                raise ValueError(f"Harness execution.pipelines.{workflow} must be an object")
            unknown_options = sorted(set(options) - allowed_options)
            if unknown_options:
                raise ValueError(
                    f"Unknown execution option(s) for {workflow}: "
                    f"{', '.join(unknown_options)}"
                )
            values = {key: int(value) for key, value in options.items()}
            if any(value < 1 for value in values.values()):
                raise ValueError(
                    f"Harness execution.pipelines.{workflow} values must be positive"
                )
            pipelines[workflow] = values
        return pipelines

    def parallelism_for_workflow(self, workflow: str, option: str) -> int:
        global_values = {
            "max_parallel": self.max_parallel,
            "max_target_parallel": self.max_target_parallel,
            "batch_model_parallel": self.batch_model_parallel,
        }
        return self.workflow_execution.get(workflow, {}).get(
            option, global_values[option]
        )

    def run(
        self,
        workflow: str,
        target_id: str,
        runs: Optional[int] = None,
        dry_run: bool = False,
        *,
        resume: Optional[Path] = None,
        max_cost: Optional[float] = None,
        max_tokens: Optional[int] = None,
        max_parallel: Optional[int] = None,
        retry_errors: bool = False,
        full_context: bool = False,
    ) -> Path:
        if workflow not in {"new", "improve", "tu-complete"}:
            raise ValueError(f"Unknown workflow: {workflow}")
        if resume:
            experiment_dir = resume.resolve()
            state = json.loads((experiment_dir / "state.json").read_text(encoding="utf-8"))
            if "logged_keys" not in state:
                state["logged_keys"] = (
                    [_record_key(row) for row in state["records"]] if state.get("logged") else []
                )
            if state["workflow"] != workflow or state["target_id"] != target_id:
                raise ValueError("Resume state workflow/target does not match the command")
            experiment_id = state["experiment_id"]
            prompt = (experiment_dir / "prompt.md").read_text(encoding="utf-8")
            if retry_errors:
                error_keys = {_record_key(row) for row in state["records"] if row.get("error")}
                state["records"] = [row for row in state["records"] if not row.get("error")]
                state["logged_keys"] = [
                    key for key in state.get("logged_keys", []) if key not in error_keys
                ]
                state["status"] = "running"
                self._write_state(experiment_dir, state)
        else:
            experiment_id = f"{datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%SZ')}-{uuid.uuid4().hex[:8]}"
            experiment_dir = self.output_dir / target_id / experiment_id
            experiment_dir.mkdir(parents=True, exist_ok=True)
            with self._adapter_lock:
                history = self.records(target_id=target_id)
                build_prompt = self.adapter.build_prompt
                options = {"full_context": full_context}
                if "options" in inspect.signature(build_prompt).parameters:
                    prompt = build_prompt(workflow, target_id, history, options)
                else:
                    prompt = build_prompt(workflow, target_id, history)
            (experiment_dir / "prompt.md").write_text(prompt, encoding="utf-8")
            context_dir = self._write_model_context(
                workflow, target_id, history, prompt, experiment_dir, options
            )
            context_mode_fn = getattr(self.adapter, "model_context_mode", None)
            context_mode = context_mode_fn(workflow) if context_mode_fn else "files"
            state = {
                "schema_version": 1,
                "experiment_id": experiment_id,
                "workflow": workflow,
                "target_id": target_id,
                "status": "running",
                "logged": False,
                "logged_keys": [],
                "records": [],
                "workflow_options": {"full_context": full_context},
                "context_dir": str(context_dir.relative_to(self.root)),
                "context_mode": context_mode,
            }
            self._write_state(experiment_dir, state)
        self._debug(
            "function decompile started "
            f"target={target_id} workflow={workflow} experiment={experiment_id} "
            f"dry_run={str(dry_run).lower()}"
        )
        if dry_run:
            return experiment_dir

        if state.get("status") == "complete":
            return experiment_dir
        effective_full_context = bool(
            state.get("workflow_options", {}).get(
                "full_context",
                workflow == "tu-complete"
                and "complete replacement translation-unit source" in prompt,
            )
        )
        context_dir = self.root / state.get(
            "context_dir", str(experiment_dir.relative_to(self.root) / "context")
        )
        context_dir.mkdir(parents=True, exist_ok=True)
        if state.get("context_mode", "files") != "inline":
            task_path = context_dir / "TASK.md"
            if not task_path.is_file():
                task_path.write_text(prompt, encoding="utf-8")
        completed = {(row["model_id"], int(row["run_index"])) for row in state["records"]}
        workflow_models = self.models_for_workflow(workflow)
        specs = [
            (model, index)
            for model in workflow_models
            for index in range(1, (runs if runs is not None else model.runs) + 1)
            if (model.id, index) not in completed
        ]
        parallel = (
            max_parallel
            if max_parallel is not None
            else self.parallelism_for_workflow(workflow, "max_parallel")
        )
        if max_cost is not None or max_tokens is not None:
            parallel = 1
        try:
            if parallel > 1 and len(specs) > 1:
                with ThreadPoolExecutor(max_workers=parallel) as executor:
                    pending_specs = iter(specs)
                    futures = {}

                    def submit_next() -> None:
                        try:
                            model, index = next(pending_specs)
                        except StopIteration:
                            return
                        future = executor.submit(
                            self._run_one,
                            workflow,
                            target_id,
                            prompt,
                            experiment_id,
                            experiment_dir,
                            model,
                            index,
                            effective_full_context,
                            context_dir,
                        )
                        futures[future] = (model.id, index)

                    for _ in range(min(parallel, len(specs))):
                        submit_next()
                    stop_submitting = False
                    while futures:
                        done, _ = wait(futures, return_when=FIRST_COMPLETED)
                        for future in done:
                            state["records"].append(future.result().to_json())
                            self._write_state(experiment_dir, state)
                            del futures[future]
                            if self._record_is_full_match(state["records"][-1]):
                                stop_submitting = True
                        if stop_submitting:
                            # Do not launch more agents after FULL_MATCH. Futures
                            # that have not started can be cancelled; already-running
                            # agents must be drained so their evidence is persisted.
                            for pending in list(futures):
                                if pending.cancel():
                                    del futures[pending]
                        else:
                            for _ in range(len(done)):
                                submit_next()
            else:
                for model, index in specs:
                    if self._budget_reached(state["records"], max_cost, max_tokens):
                        state["status"] = "budget_exhausted"
                        self._write_state(experiment_dir, state)
                        self._debug(
                            f"function decompile budget exhausted target={target_id} "
                            f"experiment={experiment_id}"
                        )
                        return experiment_dir
                    record = self._run_one(
                        workflow,
                        target_id,
                        prompt,
                        experiment_id,
                        experiment_dir,
                        model,
                        index,
                        effective_full_context,
                        context_dir,
                    )
                    state["records"].append(record.to_json())
                    self._write_state(experiment_dir, state)
                    if self._record_is_full_match(record.to_json()):
                        self._debug(
                            f"full match reached; stopping remaining agents target={target_id}"
                        )
                        break

            records = [ExperimentRecord(**row) for row in state["records"]]
            successful = [r for r in records if not r.error]
            best_record = None
            if successful:
                best_record = max(
                    successful,
                    key=lambda r: self._rank_record(workflow, r),
                )
                best_record.winner = True
                (experiment_dir / "best.json").write_text(
                    (self.root / best_record.artifact).read_text(encoding="utf-8"), encoding="utf-8"
                )
            state["records"] = [record.to_json() for record in records]
            state["status"] = "complete"
            logged_keys = set(state.get("logged_keys", []))
            for record in records:
                key = _record_key(record.to_json())
                if key not in logged_keys:
                    self._append_record(record)
                    logged_keys.add(key)
            state["logged_keys"] = sorted(logged_keys)
            state["logged"] = True
            self._write_state(experiment_dir, state)
            # Auto-promote winning candidate if it reached COMPILES or better
            if best_record is not None and workflow in {"new", "improve", "tu-complete"}:
                ev = best_record.evaluation or {}
                bstatus = ev.get("status", "")
                if bstatus in ("COMPILES", "CODE_MATCH", "FULL_MATCH", "EQUIVALENT_MATCH"):
                    try:
                        promote_diff = self.promote(experiment_dir, write=True)
                        self._debug(
                            f"auto-promoted {target_id} ({bstatus}): "
                            + promote_diff.replace("\n", " ")[:200]
                        )
                        import subprocess
                        cycle_result = subprocess.run(
                            [sys.executable, "tools/coop/run.py", "cycle", target_id,
                             "--hypothesis", f"Auto-promote {bstatus}",
                             "--next-change", "None"],
                            cwd=self.root, text=True, capture_output=True, timeout=600,
                        )
                        cycle_summary = (
                            cycle_result.stdout.split("\n")[-10:]
                            if cycle_result.stdout
                            else []
                        )
                        self._debug(
                            f"cycle {target_id}: exit={cycle_result.returncode} "
                            + " ".join(l.strip() for l in cycle_summary if l.strip())[:200]
                        )
                    except Exception as exc:
                        self._debug(f"auto-promotion failed for {target_id}: {exc}")
        finally:
            self.adapter.finalize()
        self._debug(
            f"function decompile completed target={target_id} workflow={workflow} "
            f"experiment={experiment_id} status={state['status']}"
        )
        return experiment_dir

    def run_batch(
        self,
        workflow: str,
        target_ids: List[str],
        *,
        runs: Optional[int] = None,
        dry_run: bool = False,
        max_target_parallel: Optional[int] = None,
        model_parallel: Optional[int] = None,
        full_context: bool = False,
    ) -> Path:
        if workflow not in {"new", "improve", "tu-complete"}:
            raise ValueError("Batch mode supports new, improve, or tu-complete")
        targets = list(dict.fromkeys(target_ids))
        if not targets:
            raise ValueError("Batch mode requires at least one target")
        target_parallel = (
            max_target_parallel
            if max_target_parallel is not None
            else self.parallelism_for_workflow(workflow, "max_target_parallel")
        )
        per_target_parallel = (
            model_parallel
            if model_parallel is not None
            else self.parallelism_for_workflow(workflow, "batch_model_parallel")
        )
        if target_parallel < 1 or per_target_parallel < 1:
            raise ValueError("Parallelism values must be positive")
        if target_parallel > 1 and not dry_run and self.workspace_manager is None:
            raise ValueError(
                "Parallel target evaluation requires execution.isolation.mode=git-worktree"
            )
        batch_id = f"{datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%SZ')}-{uuid.uuid4().hex[:8]}"
        batch_dir = self.output_dir / "batches" / batch_id
        batch_dir.mkdir(parents=True, exist_ok=True)
        self._debug(
            f"batch started workflow={workflow} batch={batch_id} "
            f"targets={len(targets)} target_parallel={target_parallel} "
            f"model_parallel={per_target_parallel}"
        )
        manifest: Dict[str, Any] = {
            "schema_version": 1,
            "batch_id": batch_id,
            "workflow": workflow,
            "status": "running",
            "max_target_parallel": target_parallel,
            "model_parallel": per_target_parallel,
            "targets": {},
        }
        self._write_batch_manifest(batch_dir, manifest)

        def run_target(target_id: str) -> tuple[str, str, Optional[str]]:
            try:
                output = self.run(
                    workflow,
                    target_id,
                    runs=runs,
                    dry_run=dry_run,
                    max_parallel=per_target_parallel,
                    full_context=full_context,
                )
                return target_id, str(output.relative_to(self.root)), None
            except Exception as exc:
                return target_id, "", f"{type(exc).__name__}: {exc}"

        with ThreadPoolExecutor(max_workers=min(target_parallel, len(targets))) as executor:
            futures = [executor.submit(run_target, target_id) for target_id in targets]
            for future in as_completed(futures):
                target_id, output, error = future.result()
                manifest["targets"][target_id] = {
                    "experiment": output or None,
                    "error": error,
                }
                self._debug(
                    f"batch target completed batch={batch_id} target={target_id} "
                    f"status={'error' if error else 'ok'}"
                    + (f" detail={_debug_value(error)}" if error else "")
                )
                self._write_batch_manifest(batch_dir, manifest)
        manifest["status"] = (
            "complete"
            if all(not row["error"] for row in manifest["targets"].values())
            else "partial"
        )
        self._write_batch_manifest(batch_dir, manifest)
        self._debug(f"batch completed batch={batch_id} status={manifest['status']}")
        return batch_dir

    def select_new_targets(
        self, number: int, *, ignore_called_functions: bool = False, certified_funcs: bool = False
    ) -> List[str]:
        """Ask the project adapter for fresh function targets."""
        if number < 1:
            raise ValueError("number must be positive")
        select = getattr(self.adapter, "select_new_targets", None)
        if select is None:
            raise ValueError("Configured project adapter does not support automatic target selection")
        with self._adapter_lock:
            target_ids = list(
                select(
                    number,
                    self.records(),
                    ignore_called_functions=ignore_called_functions,
                    certified_funcs=certified_funcs,
                )
            )
        if len(target_ids) != number:
            raise ValueError(
                f"Project adapter returned {len(target_ids)} targets; expected {number}"
            )
        return target_ids

    def select_targets(
        self, workflow: str, number: int, *, randomize: bool = False, certified_funcs: bool = False
    ) -> List[str]:
        """Ask the project adapter for an automatic workflow target selection."""
        if workflow not in {"improve", "tu-complete"}:
            raise ValueError("Automatic selection supports improve or tu-complete")
        if number < 1:
            raise ValueError("number must be positive")
        select = getattr(self.adapter, "select_targets", None)
        if select is None:
            raise ValueError("Configured project adapter does not support automatic target selection")
        with self._adapter_lock:
            target_ids = list(
                select(workflow, number, randomize=randomize, certified_funcs=certified_funcs)
            )
        if len(target_ids) != number:
            raise ValueError(
                f"Project adapter returned {len(target_ids)} targets; expected {number}"
            )
        return target_ids

    def _write_model_context(
        self,
        workflow: str,
        target_id: str,
        history: List[Dict[str, Any]],
        prompt: str,
        experiment_dir: Path,
        options: Dict[str, Any],
    ) -> Path:
        context_dir = experiment_dir / "context"
        context_dir.mkdir(parents=True, exist_ok=True)
        context_mode_fn = getattr(self.adapter, "model_context_mode", None)
        context_mode = context_mode_fn(workflow) if context_mode_fn else "files"
        files: Dict[str, str] = {"TASK.md": prompt} if context_mode != "inline" else {}
        build_files = getattr(self.adapter, "build_context_files", None)
        if build_files is not None:
            with self._adapter_lock:
                files.update(build_files(workflow, target_id, history, prompt, options))
        for relative, content in files.items():
            path = Path(relative)
            if path.is_absolute() or ".." in path.parts:
                raise ValueError(f"Unsafe model context path: {relative}")
            destination = context_dir / path
            destination.parent.mkdir(parents=True, exist_ok=True)
            destination.write_text(content, encoding="utf-8")
        return context_dir

    @staticmethod
    def _write_batch_manifest(batch_dir: Path, manifest: Dict[str, Any]) -> None:
        path = batch_dir / "batch.json"
        temporary = path.with_suffix(".json.tmp")
        temporary.write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")
        temporary.replace(path)

    def _run_one(
        self,
        workflow: str,
        target_id: str,
        prompt: str,
        experiment_id: str,
        experiment_dir: Path,
        model: ModelConfig,
        index: int,
        full_context: bool = False,
        context_dir: Optional[Path] = None,
    ) -> ExperimentRecord:
        provider = self.providers.get(model.provider)
        if provider is None:
            raise ValueError(f"Unsupported provider: {model.provider}")
        artifact = experiment_dir / f"{model.id}-{index}.json"
        error: Optional[str] = None
        timed_out = False
        evaluation: Dict[str, Any] = {}
        candidate_summary: Dict[str, Any] = {}
        result = None
        provider_attempts = 0
        candidate = None
        started = time.monotonic()
        agent = model.agent or "default"
        self._debug(
            f"agent started target={target_id} agent={agent} model={model.model} "
            f"run={index}"
        )
        try:
            while True:
                provider_attempts += 1
                try:
                    result = provider.invoke(
                        prompt, model, context_dir or Path(self.adapter.root)
                    )
                    break
                except Exception:
                    if provider_attempts > self.max_retries:
                        raise
                    self._debug(
                        f"agent retry target={target_id} agent={agent} model={model.model} "
                        f"run={index} attempt={provider_attempts + 1}"
                    )
                    time.sleep(min(2 ** (provider_attempts - 1), 4))
            candidate = parse_candidate(
                result.text, workflow=workflow, full_context=full_context
            )
            candidate_summary = {
                "hypothesis": candidate.hypothesis,
                "notes": candidate.notes,
                "next_change": candidate.next_change,
                "confidence": candidate.confidence,
                "provider_attempts": provider_attempts,
                "patch_ids": [patch.slot_id for patch in candidate.patches],
            }
            evaluation_obj = self._evaluate_candidate(
                workflow, target_id, candidate, f"{model.id}-{index}"
            )
            evaluation = asdict(evaluation_obj)
            artifact.write_text(
                json.dumps({"candidate": asdict(candidate), "evaluation": evaluation}, indent=2) + "\n",
                encoding="utf-8",
            )
        except Exception as exc:
            timed_out = isinstance(exc, subprocess.TimeoutExpired)
            error = f"{type(exc).__name__}: {exc}"
            self._debug(
                f"agent result target={target_id} agent={agent} model={model.model} "
                f"run={index} status=error duration={time.monotonic() - started:.1f}s "
                f"detail={_debug_value(error)}"
            )
            payload: Dict[str, Any] = {"error": error, "provider_attempts": provider_attempts}
            if candidate is not None:
                payload["candidate"] = asdict(candidate)
            artifact.write_text(json.dumps(payload, indent=2) + "\n", encoding="utf-8")
        else:
            self._debug(
                f"agent result target={target_id} agent={agent} model={model.model} "
                f"run={index} status={evaluation.get('status', 'unknown')} "
                f"match={evaluation.get('match_percent')} accepted="
                f"{evaluation.get('accepted')} duration={time.monotonic() - started:.1f}s"
            )
        return ExperimentRecord(
            schema_version=4,
            experiment_id=experiment_id,
            timestamp=datetime.now(timezone.utc).isoformat(),
            workflow=workflow,
            target_id=target_id,
            model_id=model.id,
            model=model.model,
            run_index=index,
            duration_seconds=result.duration_seconds if result else time.monotonic() - started,
            input_tokens=result.input_tokens if result else None,
            output_tokens=result.output_tokens if result else None,
            cache_read_tokens=result.cache_read_tokens if result else None,
            cache_write_tokens=result.cache_write_tokens if result else None,
            timed_out=timed_out,
            cost=result.cost if result else None,
            evaluation=evaluation,
            artifact=str(artifact.relative_to(self.root)),
            candidate_summary=candidate_summary,
            error=error,
        )

    def _evaluate_candidate(
        self, workflow: str, target_id: str, candidate: Candidate, label: str
    ) -> Any:
        if self.workspace_manager is None:
            return self.adapter.evaluate(workflow, target_id, candidate)
        workspace = self.workspace_manager.create(label)
        try:
            prepare = getattr(self.adapter, "prepare_workspace", None)
            if prepare is None:
                raise ValueError("Project adapter does not support isolated workspaces")
            prepare(workspace)
            adapter = self._load_adapter(workspace)
            try:
                return adapter.evaluate(workflow, target_id, candidate)
            finally:
                adapter.finalize()
        finally:
            self.workspace_manager.remove(workspace)

    def _write_state(self, experiment_dir: Path, state: Dict[str, Any]) -> None:
        path = experiment_dir / "state.json"
        temporary = path.with_suffix(".json.tmp")
        temporary.write_text(json.dumps(state, indent=2) + "\n", encoding="utf-8")
        temporary.replace(path)

    def _debug(self, message: str) -> None:
        """Emit flushed, serialized progress output for interactive harness runs."""
        timestamp = datetime.now().astimezone().strftime("%H:%M:%S")
        with self._debug_lock:
            print(
                f"[llm-harness {timestamp}] {message}",
                file=sys.stderr,
                flush=True,
            )

    @staticmethod
    def _record_is_full_match(record: Dict[str, Any]) -> bool:
        evaluation = record.get("evaluation") or {}
        return (
            not record.get("error")
            and evaluation.get("status") == "FULL_MATCH"
            and float(evaluation.get("match_percent") or 0.0) >= 100.0
            and bool(evaluation.get("accepted"))
        )

    @staticmethod
    def _budget_reached(
        records: List[Dict[str, Any]], max_cost: Optional[float], max_tokens: Optional[int]
    ) -> bool:
        cost = sum(float(row.get("cost") or 0) for row in records)
        tokens = sum(
            int(row.get("input_tokens") or 0) + int(row.get("output_tokens") or 0)
            for row in records
        )
        return (max_cost is not None and cost >= max_cost) or (
            max_tokens is not None and tokens >= max_tokens
        )

    def records(self, target_id: Optional[str] = None) -> List[Dict[str, Any]]:
        if not self.log_path.is_file():
            return []
        with self._log_lock:
            text = self.log_path.read_text(encoding="utf-8")
        rows = [json.loads(line) for line in text.splitlines() if line]
        latest: Dict[tuple[Any, ...], Dict[str, Any]] = {}
        for offset, row in enumerate(rows):
            key = (
                row.get("experiment_id", f"legacy-{offset}"),
                row.get("model_id"),
                row.get("run_index"),
            )
            latest[key] = row
        return [r for r in latest.values() if target_id is None or r.get("target_id") == target_id]

    def _append_record(self, record: ExperimentRecord) -> None:
        self.log_path.parent.mkdir(parents=True, exist_ok=True)
        with self._log_lock:
            with self.log_path.open("a", encoding="utf-8") as f:
                f.write(json.dumps(record.to_json(), separators=(",", ":")) + "\n")

    def stats(self) -> List[Dict[str, Any]]:
        groups: Dict[str, List[Dict[str, Any]]] = {}
        for row in self.records():
            groups.setdefault(row["model_id"], []).append(row)
        output = []
        for model_id, rows in sorted(groups.items()):
            completed = [r for r in rows if not r.get("error")]
            measured = [r for r in completed if not r.get("timed_out")]
            matches = [r["evaluation"].get("match_percent") for r in measured]
            matches = [float(v) for v in matches if v is not None]
            output.append({
                "model_id": model_id,
                "attempts": len(rows),
                "completed": len(completed),
                "wins": sum(bool(r.get("winner")) for r in rows),
                "accepted_wins": sum(
                    bool(r.get("winner")) and bool(r.get("evaluation", {}).get("accepted"))
                    for r in rows
                ),
                "accepted": sum(bool(r.get("evaluation", {}).get("accepted")) for r in completed),
                "average_match_percent": sum(matches) / len(matches) if matches else None,
                "average_seconds": (
                    sum(float(r.get("duration_seconds", 0)) for r in measured) / len(measured)
                    if measured
                    else None
                ),
                "input_tokens": sum(int(r.get("input_tokens") or 0) for r in rows),
                "output_tokens": sum(int(r.get("output_tokens") or 0) for r in rows),
                "non_cached_tokens": sum(
                    max(0, int(r.get("input_tokens") or 0) - int(r.get("cache_read_tokens") or 0))
                    for r in rows
                ),
                "cache_read_tokens": sum(int(r.get("cache_read_tokens") or 0) for r in rows),
                "cache_write_tokens": sum(int(r.get("cache_write_tokens") or 0) for r in rows),
                "total_cost": sum(float(r.get("cost") or 0) for r in rows),
                "average_cost": (
                    sum(float(r.get("cost") or 0) for r in measured) / len(measured)
                    if measured
                    else None
                ),
                "errors": sum(bool(r.get("error")) for r in rows),
                "timeouts": sum(
                    bool(r.get("timed_out"))
                    or "TimeoutExpired" in str(r.get("error") or "")
                    for r in rows
                ),
            })
        return output

    def promote(self, experiment_dir: Path, *, write: bool = False) -> str:
        directory = experiment_dir.resolve()
        state = json.loads((directory / "state.json").read_text(encoding="utf-8"))
        best = json.loads((directory / "best.json").read_text(encoding="utf-8"))
        candidate = Candidate(**best["candidate"])
        promote_fn = getattr(self.adapter, "promote", None)
        if promote_fn is None:
            raise ValueError("Configured project adapter does not support promotion")
        return promote_fn(
            state["workflow"], state["target_id"], candidate, write=write
        )

    def rescore(self, experiment_dir: Path, *, max_parallel: Optional[int] = None) -> Path:
        directory = experiment_dir.resolve()
        state = json.loads((directory / "state.json").read_text(encoding="utf-8"))
        rows = state.get("records", [])
        candidates = []
        for row in rows:
            artifact = self.root / row["artifact"]
            payload = json.loads(artifact.read_text(encoding="utf-8"))
            if isinstance(payload.get("candidate"), dict):
                candidates.append((row, artifact, Candidate(**payload["candidate"])))
        parallel = max_parallel if max_parallel is not None else self.max_parallel

        def score(item):
            row, artifact, candidate = item
            evaluation = asdict(self._evaluate_candidate(
                state["workflow"], state["target_id"], candidate,
                f"rescore-{row['model_id']}-{row['run_index']}",
            ))
            artifact.write_text(
                json.dumps({"candidate": asdict(candidate), "evaluation": evaluation}, indent=2) + "\n",
                encoding="utf-8",
            )
            row["evaluation"] = evaluation
            row["error"] = None
            row["winner"] = False
            row["timestamp"] = datetime.now(timezone.utc).isoformat()
            return row

        if parallel > 1:
            with ThreadPoolExecutor(max_workers=parallel) as executor:
                updated = list(executor.map(score, candidates))
        else:
            updated = [score(item) for item in candidates]
        by_key = {(row["model_id"], row["run_index"]): row for row in updated}
        state["records"] = [by_key.get((row["model_id"], row["run_index"]), row) for row in rows]
        records = [ExperimentRecord(**row) for row in state["records"]]
        successful = [record for record in records if not record.error]
        if successful:
            best = max(successful, key=lambda record: self._rank_record(state["workflow"], record))
            best.winner = True
            (directory / "best.json").write_text(
                (self.root / best.artifact).read_text(encoding="utf-8"), encoding="utf-8"
            )
        state["records"] = [record.to_json() for record in records]
        state["status"] = "complete"
        state["rescored_at"] = datetime.now(timezone.utc).isoformat()
        self._write_state(directory, state)
        for record in records:
            self._append_record(record)
        return directory

    def _rank_record(self, workflow: str, record: ExperimentRecord) -> tuple[Any, ...]:
        rank_fn = getattr(self.adapter, "rank_candidate", None)
        if rank_fn is not None:
            return tuple(rank_fn(workflow, record.evaluation))
        return (
            bool(record.evaluation.get("accepted")),
            record.evaluation.get("match_percent") or 0.0,
        )


def parse_candidate(
    text: str, *, workflow: str = "new", full_context: bool = False
) -> Candidate:
    cleaned = text.strip()
    fenced = re.search(r"```(?:json)?\s*(\{.*\})\s*```", cleaned, re.DOTALL)
    if fenced:
        cleaned = fenced.group(1)
    else:
        start, end = cleaned.find("{"), cleaned.rfind("}")
        if start >= 0 and end > start:
            cleaned = cleaned[start : end + 1]
    data = json.loads(cleaned)
    source = data.get("source", "")
    if not isinstance(source, str):
        raise ValueError("Model output 'source' must be a string")
    patches_data = data.get("patches", [])
    if not isinstance(patches_data, list):
        raise ValueError("Model output 'patches' must be a list")
    patches: List[SourcePatch] = []
    for value in patches_data:
        if not isinstance(value, dict):
            raise ValueError("Each model output patch must be an object")
        slot_id = value.get("slot_id")
        patch_source = value.get("source")
        if not isinstance(slot_id, str) or not slot_id.strip():
            raise ValueError("Each model output patch requires non-empty string 'slot_id'")
        if not isinstance(patch_source, str) or not patch_source.strip():
            raise ValueError("Each model output patch requires non-empty string 'source'")
        patches.append(SourcePatch(slot_id=slot_id, source=patch_source))
    if source.strip() and patches:
        raise ValueError("Model output must use either 'source' or 'patches', not both")
    if workflow == "tu-complete":
        if not source.strip() and not patches:
            raise ValueError("TU model output requires non-empty 'patches' or fallback 'source'")
        if full_context and patches:
            raise ValueError("Full-context TU output must contain complete 'source', not patches")
        if not full_context and source.strip():
            raise ValueError("Targeted TU output must contain bounded 'patches', not full source")
    elif not source.strip():
        raise ValueError("Model output must contain non-empty string 'source'")
    notes = data.get("notes", [])
    if isinstance(notes, str):
        notes = [notes]
    if not isinstance(notes, list) or not all(isinstance(v, str) for v in notes):
        raise ValueError("Model output 'notes' must be a list of strings")
    confidence = data.get("confidence")
    return Candidate(
        source=source,
        hypothesis=str(data.get("hypothesis", "")),
        notes=notes,
        next_change=str(data.get("next_change", "")),
        confidence=float(confidence) if confidence is not None else None,
        patches=patches,
    )


def _record_key(row: Dict[str, Any]) -> str:
    return f"{row.get('model_id')}:{int(row.get('run_index', 0))}"
