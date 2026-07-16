from __future__ import annotations

import importlib
import importlib.util
import json
import re
import uuid
from dataclasses import asdict
from datetime import datetime, timezone
from pathlib import Path
from typing import Any, Dict, List, Optional

from .providers import OpenCodeProvider
from .types import Candidate, ExperimentRecord, ModelConfig, ProjectAdapter


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
        self.adapter = self._load_adapter()
        provider_cfg = self.config.get("providers", {}).get("opencode", {})
        self.providers = {
            "opencode": OpenCodeProvider(
                binary=provider_cfg.get("binary", "opencode"),
                timeout_seconds=int(provider_cfg.get("timeout_seconds", 900)),
            )
        }

    def _load_adapter(self) -> ProjectAdapter:
        ref = self.config["project_adapter"]
        if ref.endswith(".py"):
            path = (self.root / ref).resolve()
            spec = importlib.util.spec_from_file_location("llm_harness_project_adapter", path)
            if spec is None or spec.loader is None:
                raise ImportError(f"Cannot load project adapter: {path}")
            module = importlib.util.module_from_spec(spec)
            spec.loader.exec_module(module)
        else:
            module = importlib.import_module(ref)
        return module.create_adapter(self.root, self.config.get("project", {}))

    def run(self, workflow: str, target_id: str, runs: Optional[int] = None, dry_run: bool = False) -> Path:
        if workflow not in {"new", "improve"}:
            raise ValueError(f"Unknown workflow: {workflow}")
        experiment_id = f"{datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%SZ')}-{uuid.uuid4().hex[:8]}"
        experiment_dir = self.output_dir / target_id / experiment_id
        experiment_dir.mkdir(parents=True, exist_ok=True)
        history = self.records(target_id=target_id)
        prompt = self.adapter.build_prompt(workflow, target_id, history)
        (experiment_dir / "prompt.md").write_text(prompt, encoding="utf-8")
        if dry_run:
            return experiment_dir

        records: List[ExperimentRecord] = []
        try:
            for model in self.models:
                provider = self.providers.get(model.provider)
                if provider is None:
                    raise ValueError(f"Unsupported provider: {model.provider}")
                count = runs if runs is not None else model.runs
                for index in range(1, count + 1):
                    artifact = experiment_dir / f"{model.id}-{index}.json"
                    error: Optional[str] = None
                    evaluation: Dict[str, Any] = {}
                    candidate_summary: Dict[str, Any] = {}
                    result = None
                    try:
                        result = provider.invoke(prompt, model, Path(self.adapter.root))
                        candidate = parse_candidate(result.text)
                        candidate_summary = {
                            "hypothesis": candidate.hypothesis,
                            "notes": candidate.notes,
                            "next_change": candidate.next_change,
                            "confidence": candidate.confidence,
                        }
                        evaluation_obj = self.adapter.evaluate(target_id, candidate)
                        evaluation = asdict(evaluation_obj)
                        artifact.write_text(
                            json.dumps({"candidate": asdict(candidate), "evaluation": evaluation}, indent=2) + "\n",
                            encoding="utf-8",
                        )
                    except Exception as exc:  # Preserve failed model/build attempts as experiment data.
                        error = f"{type(exc).__name__}: {exc}"
                        artifact.write_text(json.dumps({"error": error}, indent=2) + "\n", encoding="utf-8")
                    record = ExperimentRecord(
                        schema_version=1,
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
                    records.append(record)

            successful = [r for r in records if not r.error]
            if successful:
                best = max(
                    successful,
                    key=lambda r: (
                        bool(r.evaluation.get("accepted")),
                        r.evaluation.get("match_percent") or 0.0,
                    ),
                )
                best.winner = True
                (experiment_dir / "best.json").write_text(
                    (self.root / best.artifact).read_text(encoding="utf-8"), encoding="utf-8"
                )
            for record in records:
                self._append_record(record)
        finally:
            self.adapter.finalize()
        return experiment_dir

    def records(self, target_id: Optional[str] = None) -> List[Dict[str, Any]]:
        if not self.log_path.is_file():
            return []
        rows = [json.loads(line) for line in self.log_path.read_text(encoding="utf-8").splitlines() if line]
        return [r for r in rows if target_id is None or r.get("target_id") == target_id]

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
                "cost": sum(float(r.get("cost") or 0) for r in rows),
                "errors": sum(bool(r.get("error")) for r in rows),
            })
        return output


def parse_candidate(text: str) -> Candidate:
    cleaned = text.strip()
    fenced = re.search(r"```(?:json)?\s*(\{.*\})\s*```", cleaned, re.DOTALL)
    if fenced:
        cleaned = fenced.group(1)
    else:
        start, end = cleaned.find("{"), cleaned.rfind("}")
        if start >= 0 and end > start:
            cleaned = cleaned[start : end + 1]
    data = json.loads(cleaned)
    if not isinstance(data.get("full_source"), str) or not data["full_source"].strip():
        raise ValueError("Model output must contain non-empty string 'full_source'")
    notes = data.get("notes", [])
    if isinstance(notes, str):
        notes = [notes]
    if not isinstance(notes, list) or not all(isinstance(v, str) for v in notes):
        raise ValueError("Model output 'notes' must be a list of strings")
    confidence = data.get("confidence")
    return Candidate(
        full_source=data["full_source"],
        hypothesis=str(data.get("hypothesis", "")),
        notes=notes,
        next_change=str(data.get("next_change", "")),
        confidence=float(confidence) if confidence is not None else None,
    )
