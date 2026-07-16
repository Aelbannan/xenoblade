from __future__ import annotations

import importlib
import importlib.util
import inspect
import json
import re
import time
import uuid
from concurrent.futures import ThreadPoolExecutor, as_completed
from dataclasses import asdict
from datetime import datetime, timezone
from pathlib import Path
from typing import Any, Dict, List, Optional

from .providers import OpenCodeProvider
from .types import Candidate, ExperimentRecord, ModelConfig, ProjectAdapter, SourcePatch
from .workspace import GitWorktreeManager


class Harness:
    def __init__(self, config_path: Path) -> None:
        self.config_path = config_path.resolve()
        self.config = json.loads(self.config_path.read_text(encoding="utf-8"))
        self.root = self.config_path.parent.resolve()
        self.output_dir = (self.root / self.config.get("output_dir", "build/llm-harness")).resolve()
        self.log_path = self.output_dir / "experiments.jsonl"
        self.models = [ModelConfig(**row) for row in self.config.get("models", [])]
        if not self.models:
            raise ValueError("Harness config must define at least one model")
        self.adapter = self._load_adapter(self.root)
        execution = self.config.get("execution", {})
        self.max_parallel = int(execution.get("max_parallel", 1))
        self.max_retries = int(execution.get("max_retries", 1))
        isolation = execution.get("isolation", {})
        self.isolation_mode = isolation.get("mode", "none")
        self.workspace_manager = (
            GitWorktreeManager(self.root, copy_dirty=bool(isolation.get("copy_dirty", True)))
            if self.isolation_mode == "git-worktree"
            else None
        )
        provider_cfg = self.config.get("providers", {}).get("opencode", {})
        self.providers = {
            "opencode": OpenCodeProvider(
                binary=provider_cfg.get("binary", "opencode"),
                timeout_seconds=int(provider_cfg.get("timeout_seconds", 900)),
            )
        }

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
            history = self.records(target_id=target_id)
            build_prompt = self.adapter.build_prompt
            if "options" in inspect.signature(build_prompt).parameters:
                prompt = build_prompt(
                    workflow,
                    target_id,
                    history,
                    {"full_context": full_context},
                )
            else:
                prompt = build_prompt(workflow, target_id, history)
            (experiment_dir / "prompt.md").write_text(prompt, encoding="utf-8")
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
            }
            self._write_state(experiment_dir, state)
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
        completed = {(row["model_id"], int(row["run_index"])) for row in state["records"]}
        specs = [
            (model, index)
            for model in self.models
            for index in range(1, (runs if runs is not None else model.runs) + 1)
            if (model.id, index) not in completed
        ]
        parallel = max_parallel if max_parallel is not None else self.max_parallel
        if max_cost is not None or max_tokens is not None:
            parallel = 1
        try:
            if parallel > 1 and len(specs) > 1:
                with ThreadPoolExecutor(max_workers=parallel) as executor:
                    futures = {
                        executor.submit(
                            self._run_one, workflow, target_id, prompt, experiment_id,
                            experiment_dir, model, index, effective_full_context,
                        ): (model.id, index)
                        for model, index in specs
                    }
                    for future in as_completed(futures):
                        state["records"].append(future.result().to_json())
                        self._write_state(experiment_dir, state)
            else:
                for model, index in specs:
                    if self._budget_reached(state["records"], max_cost, max_tokens):
                        state["status"] = "budget_exhausted"
                        self._write_state(experiment_dir, state)
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
                    )
                    state["records"].append(record.to_json())
                    self._write_state(experiment_dir, state)

            records = [ExperimentRecord(**row) for row in state["records"]]
            successful = [r for r in records if not r.error]
            if successful:
                best = max(
                    successful,
                    key=lambda r: self._rank_record(workflow, r),
                )
                best.winner = True
                (experiment_dir / "best.json").write_text(
                    (self.root / best.artifact).read_text(encoding="utf-8"), encoding="utf-8"
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
        finally:
            self.adapter.finalize()
        return experiment_dir

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
    ) -> ExperimentRecord:
        provider = self.providers.get(model.provider)
        if provider is None:
            raise ValueError(f"Unsupported provider: {model.provider}")
        artifact = experiment_dir / f"{model.id}-{index}.json"
        error: Optional[str] = None
        evaluation: Dict[str, Any] = {}
        candidate_summary: Dict[str, Any] = {}
        result = None
        provider_attempts = 0
        candidate = None
        try:
            while True:
                provider_attempts += 1
                try:
                    result = provider.invoke(prompt, model, Path(self.adapter.root))
                    break
                except Exception:
                    if provider_attempts > self.max_retries:
                        raise
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
            error = f"{type(exc).__name__}: {exc}"
            payload: Dict[str, Any] = {"error": error, "provider_attempts": provider_attempts}
            if candidate is not None:
                payload["candidate"] = asdict(candidate)
            artifact.write_text(json.dumps(payload, indent=2) + "\n", encoding="utf-8")
        return ExperimentRecord(
            schema_version=3,
            experiment_id=experiment_id,
            timestamp=datetime.now(timezone.utc).isoformat(),
            workflow=workflow,
            target_id=target_id,
            model_id=model.id,
            model=model.model,
            run_index=index,
            duration_seconds=result.duration_seconds if result else 0.0,
            input_tokens=result.input_tokens if result else None,
            output_tokens=result.output_tokens if result else None,
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
        rows = [json.loads(line) for line in self.log_path.read_text(encoding="utf-8").splitlines() if line]
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
        with self.log_path.open("a", encoding="utf-8") as f:
            f.write(json.dumps(record.to_json(), separators=(",", ":")) + "\n")

    def stats(self) -> List[Dict[str, Any]]:
        groups: Dict[str, List[Dict[str, Any]]] = {}
        for row in self.records():
            groups.setdefault(row["model_id"], []).append(row)
        output = []
        for model_id, rows in sorted(groups.items()):
            completed = [r for r in rows if not r.get("error")]
            matches = [r["evaluation"].get("match_percent") for r in completed]
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
                "average_seconds": sum(float(r.get("duration_seconds", 0)) for r in rows) / len(rows),
                "input_tokens": sum(int(r.get("input_tokens") or 0) for r in rows),
                "output_tokens": sum(int(r.get("output_tokens") or 0) for r in rows),
                "total_cost": sum(float(r.get("cost") or 0) for r in rows),
                "average_cost": (
                    sum(float(r.get("cost") or 0) for r in rows) / len(rows)
                    if rows
                    else None
                ),
                "errors": sum(bool(r.get("error")) for r in rows),
            })
        return output

    def promote(self, experiment_dir: Path, *, write: bool = False, owner: str = "") -> str:
        directory = experiment_dir.resolve()
        state = json.loads((directory / "state.json").read_text(encoding="utf-8"))
        best = json.loads((directory / "best.json").read_text(encoding="utf-8"))
        candidate = Candidate(**best["candidate"])
        promote_fn = getattr(self.adapter, "promote", None)
        if promote_fn is None:
            raise ValueError("Configured project adapter does not support promotion")
        return promote_fn(
            state["workflow"], state["target_id"], candidate, write=write, owner=owner
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
