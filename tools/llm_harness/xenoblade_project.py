"""Xenoblade-specific prompts, dossier extraction, commands, and acceptance policy.

The generic harness deliberately knows none of these repository details.
"""
from __future__ import annotations

import json
import sqlite3
import subprocess
from pathlib import Path
from typing import Any, Dict, List

from tools.coop.lib.config import load_config
from tools.coop.lib.mwcc_knowledge import (
    build_database,
    connect,
    database_is_fresh,
    get_entry,
    infer_tags,
    search,
)
from tools.coop.lib.objdiff_report import evaluate_unit_match, meets_required_level
from tools.coop.lib.object_size import check_object_size
from tools.coop.lib.project import Project
from tools.coop.lib.targets import Target, get_target, load_targets
from tools.llm_harness.types import Candidate, Evaluation
from tools.ppc_equivalence.elf_symbols import extract_function


class XenobladeAdapter:
    def __init__(self, root: Path, settings: Dict[str, Any]) -> None:
        self.root = root
        coop_path = settings.get("coop_config")
        self.config = load_config(root / coop_path if coop_path else None, root)
        self.project = Project(self.config)
        self.prompt_dir = root / settings.get("prompt_dir", "tools/llm_harness/prompts")
        self.max_source_chars = int(settings.get("max_source_chars", 120000))
        self.knowledge_enabled = bool(settings.get("mwcc_knowledge_enabled", True))
        self.knowledge_database = root / settings.get(
            "mwcc_knowledge_database", "build/mwcc_knowledge.sqlite"
        )
        self.knowledge_reference = root / settings.get(
            "mwcc_knowledge_reference", "docs/MWCC_REFERENCE.md"
        )
        self.knowledge_limit = int(settings.get("mwcc_knowledge_limit", 8))
        self.knowledge_body_chars = int(settings.get("mwcc_knowledge_body_chars", 5000))

    def _target(self, target_id: str) -> Target:
        target = get_target(load_targets(self.config), target_id)
        if not target.buildable or target.source is None or target.unit is None or not target.symbol:
            raise ValueError(f"Target {target_id!r} must be a buildable function with a symbol")
        return target

    def build_prompt(self, workflow: str, target_id: str, history: List[Dict[str, Any]]) -> str:
        target = self._target(target_id)
        assert target.source is not None and target.unit is not None and target.symbol is not None
        unit = self.project.resolve_unit(target.unit)
        if unit.target_path is None or not unit.target_path.is_file():
            raise FileNotFoundError(f"Retail split object is missing for {target.unit}")
        function = extract_function(unit.target_path, target.symbol)
        source = target.source.read_text(encoding="utf-8")
        if len(source) > self.max_source_chars:
            raise ValueError(
                f"Source file is {len(source)} chars; limit is {self.max_source_chars}. "
                "Raise project.max_source_chars or use a smaller translation unit."
            )
        relevant_history = [
            {
                "model_id": row.get("model_id"),
                "workflow": row.get("workflow"),
                "candidate_summary": row.get("candidate_summary"),
                "evaluation": row.get("evaluation"),
                "error": row.get("error"),
            }
            for row in history[-12:]
        ]
        knowledge = self._mwcc_knowledge(target, relevant_history)
        same_unit = [
            {"id": item.id, "function": item.function, "status": item.status}
            for item in load_targets(self.config)
            if item.unit == target.unit and item.id != target.id and item.status in {"FULL_MATCH", "EQUIVALENT_MATCH"}
        ][:12]
        dossier = {
            "target": {
                "id": target.id,
                "function": target.function,
                "symbol": target.symbol,
                "address": target.address,
                "unit": target.unit,
                "source": str(target.source.relative_to(self.root)),
                "current_status": target.status,
                "required_level": target.required_level,
            },
            "retail_function": {
                "base": f"0x{function.base:08X}",
                "size": function.size,
                "bytecode_hex": function.code.hex(" "),
                "relocations": [
                    {
                        "offset": f"0x{rel.offset:X}",
                        "type": rel.relocation_type,
                        "symbol": rel.symbol,
                        "addend": rel.addend,
                    }
                    for rel in function.relocations
                ],
            },
            "accepted_functions_in_same_unit": same_unit,
            "prior_harness_attempts": relevant_history if workflow == "improve" else [],
            "mwcc_knowledge": knowledge,
        }
        common = (self.prompt_dir / "common.md").read_text(encoding="utf-8")
        workflow_prompt = (self.prompt_dir / f"{workflow}.md").read_text(encoding="utf-8")
        return (
            common.replace("{{WORKFLOW_PROMPT}}", workflow_prompt)
            .replace("{{DOSSIER_JSON}}", json.dumps(dossier, indent=2))
            .replace("{{CURRENT_SOURCE}}", source)
        )

    def _mwcc_knowledge(
        self, target: Target, history: List[Dict[str, Any]]
    ) -> Dict[str, Any]:
        if not self.knowledge_enabled:
            return {"enabled": False, "queries": [], "records": []}
        attempts = self.config.resolve(self.config.attempt_log)
        sources = [self.knowledge_reference, attempts]
        try:
            if not database_is_fresh(self.knowledge_database, sources):
                build_database(
                    self.knowledge_database,
                    self.knowledge_reference,
                    attempts,
                    root=self.root,
                )
            queries = _knowledge_queries(target, history)
            records: List[Dict[str, Any]] = []
            seen: set[str] = set()
            with connect(self.knowledge_database) as connection:
                for query in queries:
                    effective_mode = query.get("mode", "all")
                    rows = search(
                        connection,
                        query["query"],
                        tag=query.get("tag", ""),
                        mode=effective_mode,
                        limit=min(4, self.knowledge_limit),
                    )
                    if not rows and effective_mode == "all":
                        effective_mode = "any"
                        rows = search(
                            connection,
                            query["query"],
                            tag=query.get("tag", ""),
                            mode=effective_mode,
                            limit=min(4, self.knowledge_limit),
                        )
                    query["effective_mode"] = effective_mode
                    query["result_ids"] = [str(row["id"]) for row in rows]
                    for row in rows:
                        entry_id = str(row["id"])
                        if entry_id in seen:
                            continue
                        full = get_entry(connection, entry_id)
                        if full is None:
                            continue
                        seen.add(entry_id)
                        records.append(_knowledge_record(full, self.knowledge_body_chars))
                        if len(records) >= self.knowledge_limit:
                            break
                    if len(records) >= self.knowledge_limit:
                        break
            return {"enabled": True, "queries": queries, "records": records}
        except (FileNotFoundError, OSError, sqlite3.Error, ValueError) as exc:
            return {
                "enabled": True,
                "queries": [],
                "records": [],
                "error": f"{type(exc).__name__}: {exc}",
            }

    def evaluate(self, target_id: str, candidate: Candidate) -> Evaluation:
        target = self._target(target_id)
        assert target.source is not None and target.unit is not None
        original = target.source.read_text(encoding="utf-8")
        self._validate_source(original, candidate.full_source)
        unit = self.project.resolve_unit(target.unit)
        original_object = unit.base_path.read_bytes() if unit.base_path and unit.base_path.is_file() else None
        try:
            target.source.write_text(candidate.full_source, encoding="utf-8")
            if unit.base_path is None:
                raise ValueError(f"Objdiff unit {unit.name!r} has no candidate object path")
            try:
                self.project.ninja_build(str(unit.base_path.relative_to(self.root)))
            except subprocess.CalledProcessError as exc:
                return Evaluation(
                    status="COMPILE_ERROR",
                    match_percent=0.0,
                    accepted=False,
                    detail=f"build exited {exc.returncode}",
                )
            try:
                evaluation = evaluate_unit_match(self.project, unit, target.symbol)
            except (subprocess.CalledProcessError, FileNotFoundError, ValueError) as exc:
                return Evaluation(
                    status="EVALUATION_ERROR",
                    match_percent=0.0,
                    accepted=False,
                    detail=(
                        f"objdiff evaluation failed: {exc}. Run the repository baseline/full build "
                        "so every object referenced by objdiff.json exists."
                    ),
                )
            match_percent = (
                evaluation.fn_match.match_percent
                if evaluation.fn_match
                else evaluation.unit_report.fuzzy_match_percent
            )
            size = check_object_size(
                project_root=self.root,
                region=self.config.region,
                unit_hint=target.unit,
                retail_object=unit.target_path,
                decomp_object=unit.base_path,
            )
            accepted = size.ok and meets_required_level(
                target.required_level,
                evaluation.status,
                function_match=match_percent,
                unit=evaluation.unit_report,
                symbol=target.symbol,
                equivalence=evaluation.equivalence,
            )
            return Evaluation(
                status=evaluation.status,
                match_percent=match_percent,
                accepted=accepted,
                size_ok=size.ok,
                equivalence=evaluation.equivalence.value if evaluation.equivalence else None,
                detail="; ".join(v for v in (evaluation.equivalence_detail, size.notes) if v),
            )
        finally:
            target.source.write_text(original, encoding="utf-8")
            if unit.base_path:
                if original_object is None:
                    unit.base_path.unlink(missing_ok=True)
                else:
                    unit.base_path.write_bytes(original_object)

    def _validate_source(self, original: str, candidate: str) -> None:
        if not candidate.strip():
            raise ValueError("Candidate source is empty")
        if len(candidate) > self.max_source_chars * 2:
            raise ValueError("Candidate source exceeds the safety limit")
        forbidden = ("asm {", "asm void", "register r", "asm(\"r", "sp[")
        added = [token for token in forbidden if candidate.count(token) > original.count(token)]
        if added:
            raise ValueError(f"Candidate adds forbidden low-level source patterns: {', '.join(added)}")

    def finalize(self) -> None:
        # Each candidate evaluation restores both source and its prior object bytes.
        pass


def create_adapter(root: Path, settings: Dict[str, Any]) -> XenobladeAdapter:
    return XenobladeAdapter(root, settings)


def _knowledge_queries(target: Target, history: List[Dict[str, Any]]) -> List[Dict[str, Any]]:
    """Build small, auditable KB queries following docs/MWCC_KNOWLEDGE_BASE.md."""
    queries: List[Dict[str, Any]] = []
    for value in (target.function, target.symbol or ""):
        value = value.strip()
        if value and value not in {row["query"] for row in queries}:
            queries.append({"query": value, "mode": "all", "reason": "target identity"})

    history_text = json.dumps(history, ensure_ascii=False)
    for tag in infer_tags(history_text):
        queries.append(
            {
                "query": tag,
                "tag": tag,
                "mode": "all",
                "reason": "prior attempt mismatch category",
            }
        )
    return queries[:6]


def _knowledge_record(row: Any, body_limit: int) -> Dict[str, Any]:
    body = str(row["body"] or "")
    truncated = len(body) > body_limit
    if truncated:
        body = body[:body_limit].rstrip() + "\n[truncated]"
    return {
        "id": row["id"],
        "kind": row["source_kind"],
        "title": row["title"],
        "body": body,
        "status": row["status"] or None,
        "match_percent": row["match_percent"],
        "target_id": row["target_id"] or None,
        "symbol": row["symbol"] or None,
        "tags": str(row["tags"] or "").split(),
        "source": f"{row['source_path']}:{row['line_start']}",
        "truncated": truncated,
    }
