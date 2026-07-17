"""Xenoblade-specific prompts, dossier extraction, commands, and acceptance policy.

The generic harness deliberately knows none of these repository details.
"""
from __future__ import annotations

import json
import hashlib
import os
import random
import re
import subprocess
import shutil
import struct
import sys
import difflib
from dataclasses import dataclass
from pathlib import Path
from typing import Any, Dict, List, Optional

from tools.coop.lib.config import load_config
from tools.coop.lib.equivalence_check import prove_unit_symbol
from tools.coop.lib.mwcc_knowledge import (
    KnowledgeEntry,
    infer_tags,
    parse_attempts,
    parse_reference,
)
from tools.coop.lib.objdiff_report import evaluate_unit_match, meets_required_level, report_unit
from tools.coop.lib.object_size import check_object_size
from tools.coop.lib.project import Project
from tools.coop.lib.targets import (
    ACCEPTED_MATCH_STATUSES,
    Target,
    equivalence_certificate_error,
    get_target,
    harness_targets,
    load_targets,
    load_targets_document,
    write_targets_document,
)
from tools.llm_harness.types import Candidate, Evaluation, SourcePatch
from tools.ppc_equivalence.elf_symbols import (
    ET_REL,
    SHT_NOBITS,
    SHT_REL,
    SHT_RELA,
    _require_elf32_be,
    _section_table,
    _symbol_name,
    extract_function,
    list_text_functions,
)
from tools.ppc_equivalence.result import ProofStatus


@dataclass(frozen=True)
class SourceRegion:
    start: int
    end: int
    content_start: int
    content_end: int
    marked: bool


@dataclass(frozen=True)
class TuSlot:
    slot_id: str
    kind: str
    start: int
    end: int
    content_start: int
    content_end: int


def _frozen_kb_cache_key(sources: list[Path]) -> str:
    """Content-hash of KB source files for content-addressable frozen-KB cache.

    Uses SHA-256 of each source file's full content, so identical source
    material always produces the same cache key regardless of mtime or
    inode.  Returns a hex digest suitable as a filename.
    """
    h = hashlib.sha256()
    for path in sorted(sources):  # deterministic order regardless of caller
        h.update(path.name.encode("utf-8"))
        if path.is_file():
            h.update(path.read_bytes())
    return h.hexdigest()[:32]


class XenobladeAdapter:
    def __init__(self, root: Path, settings: Dict[str, Any]) -> None:
        self.root = root.resolve()
        coop_path = settings.get("coop_config")
        self.config = load_config(self.root / coop_path if coop_path else None, self.root)
        self.project = Project(self.config)
        self.prompt_dir = self.root / settings.get("prompt_dir", "tools/llm_harness/prompts")
        self.max_source_chars = int(settings.get("max_source_chars", 120000))
        self.knowledge_enabled = bool(settings.get("mwcc_knowledge_enabled", True))
        self.knowledge_reference = self.root / settings.get(
            "mwcc_knowledge_reference", "docs/MWCC_REFERENCE.md"
        )
        self.context_similar_limit = int(settings.get("context_similar_limit", 4))
        self.tu_context_chars = int(settings.get("tu_context_chars", 300))
        self.tu_section_byte_limit = int(settings.get("tu_section_byte_limit", 16384))
        # Frozen corpus-wide knowledge prefix. Computed once per adapter (i.e. once
        # per harness process) so every prompt in a campaign shares a byte-identical
        # cacheable prefix regardless of workflow or target. The KB is snapshotted at
        # this moment; subsequent attempt-log appends during the campaign do not
        # perturb the frozen bytes — only the next harness invocation refreshes it.
        self._frozen_kb_prefix = ""
        self._frozen_kb_sha = ""
        self._frozen_kb_error: Optional[str] = None
        self._frozen_kb_ready = False
        self._build_frozen_knowledge()

    # ------------------------------------------------------------------
    # Frozen knowledge base
    # ------------------------------------------------------------------

    def _build_frozen_knowledge(self) -> None:
        """Snapshot every reference and attempt record as a compact markdown block.

        The block is byte-stable for the lifetime of this adapter and is spliced
        into every prompt via the `{{FROZEN_KB}}` placeholder, so its bytes land
        in DeepSeek's cacheable prefix rather than in the per-target dossier.
        """
        if not self.knowledge_enabled:
            self._frozen_kb_ready = True
            return
        try:
            attempts_path = self.config.resolve(self.config.attempt_log)
            sources = [self.knowledge_reference, attempts_path]

            # Content-addressable cache: hash source contents so the same KB
            # source produces the same bytes across every harness invocation.
            # This keeps DeepSeek's prompt prefix cache valid across runs.
            cache_key = _frozen_kb_cache_key(sources)
            cache_dir = self.root / "build" / "mwcc-knowledge-frozen-cache"
            cache_path = cache_dir / f"{cache_key}.txt"
            if cache_path.is_file():
                cached = cache_path.read_text(encoding="utf-8")
                self._frozen_kb_prefix = cached
                self._frozen_kb_sha = hashlib.sha1(cached.encode("utf-8")).hexdigest()[:12]
                self._frozen_kb_ready = True
                return

            # Parse source files directly — no SQLite database.
            entries: list[KnowledgeEntry] = []
            if self.knowledge_reference.is_file():
                entries.extend(parse_reference(self.knowledge_reference))
            if attempts_path.is_file():
                entries.extend(parse_attempts(attempts_path))

            # Sort stable: references before attempts, then by id, then line_start.
            entries.sort(key=lambda e: (e.source_kind, e.id, e.line_start))

            lines: list[str] = [
                f"Frozen MWCC knowledge base ({len(entries)} entries). "
                "Reference any entry below by its stable ID in `hypothesis` or "
                "`notes`; do not quote bodies back."
            ]
            for entry in entries:
                lines.append("")
                lines.append(f"### {entry.id}")
                if entry.title:
                    lines.append(entry.title)
                if entry.body:
                    lines.append(entry.body)
            prefix = "\n".join(lines)
            self._frozen_kb_prefix = prefix
            self._frozen_kb_sha = hashlib.sha1(prefix.encode("utf-8")).hexdigest()[:12]
            self._frozen_kb_ready = True

            # Persist to cache so subsequent harness runs reuse the same bytes.
            cache_dir.mkdir(parents=True, exist_ok=True)
            cache_path.write_text(prefix, encoding="utf-8")
        except (FileNotFoundError, OSError, ValueError) as exc:
            # The campaign can still run; the dossier's hints carry the model.
            self._frozen_kb_error = f"{type(exc).__name__}: {exc}"

    def _frozen_kb_block(self) -> str:
        if not self.knowledge_enabled or not self._frozen_kb_ready:
            return ""
        if self._frozen_kb_prefix:
            sha_line = f"snapshot_sha={self._frozen_kb_sha}"
            if self._frozen_kb_error:
                sha_line += f" error={self._frozen_kb_error}"
            return f"{sha_line}\n\n{self._frozen_kb_prefix}"
        if self._frozen_kb_error:
            return f"snapshot_error={self._frozen_kb_error}"
        return ""

    def _target(self, target_id: str) -> Target:
        target = self._any_target(target_id)
        if not target.buildable or target.source is None or target.unit is None or not target.symbol:
            raise ValueError(f"Target {target_id!r} must be a buildable function with a symbol")
        return target

    def _any_target(self, target_id: str) -> Target:
        return get_target(load_targets(self.config), target_id)

    def select_new_targets(
        self,
        number: int,
        history: List[Dict[str, Any]],
        *,
        ignore_called_functions: bool = False,
        certified_funcs: bool = False,
    ) -> List[str]:
        """Return fresh functions from the call-graph-safe catalog frontier."""
        attempted = {
            str(row.get("target_id"))
            for row in history
            if row.get("workflow") in {"new", "improve"} and row.get("target_id")
        }
        selection = "pending" if ignore_called_functions else "ready"
        raw_targets = load_targets(self.config)
        candidates = [
            target
            for target in harness_targets(
                raw_targets, selection=selection, include_catalog=True
            )
            if target.status == "NOT_STARTED"
            and target.symbol
            and target.source is not None
            and target.source.is_file()
            and target.id not in attempted
        ]
        if certified_funcs:
            candidates = self._filter_certified_funcs(raw_targets, candidates)
        selected: List[str] = []
        source_cache: Dict[Path, str] = {}
        for target in candidates:
            assert (
                target.source is not None
                and target.unit is not None
                and target.symbol is not None
            )
            source = source_cache.get(target.source)
            if source is None:
                source = target.source.read_text(encoding="utf-8")
                source_cache[target.source] = source
            try:
                _find_function_region(source, target)
                unit = self.project.resolve_unit(target.unit)
                if unit.target_path is None or not unit.target_path.is_file():
                    continue
                extract_function(unit.target_path, target.symbol)
            except (OSError, ValueError):
                continue
            selected.append(target.id)
            if len(selected) == number:
                return selected
        raise ValueError(
            f"Only {len(selected)} unattempted, ready functions are available; requested {number}"
        )

    def select_targets(
        self, workflow: str, number: int, *, randomize: bool = False, certified_funcs: bool = False
    ) -> List[str]:
        if workflow == "improve":
            candidates = self._improve_candidates(certified_funcs=certified_funcs)
        elif workflow == "tu-complete":
            candidates = self._tu_completion_candidates()
        else:
            raise ValueError(f"Unsupported automatic workflow selection: {workflow}")
        if randomize:
            random.shuffle(candidates)
        if len(candidates) < number:
            raise ValueError(
                f"Only {len(candidates)} eligible {workflow} targets are available; "
                f"requested {number}"
            )
        return candidates[:number]

    @staticmethod
    def _filter_certified_funcs(
        all_targets: List[Target], candidates: List[Target]
    ) -> List[Target]:
        rows_by_id = {
            target.id: {"id": target.id, **target.extra}
            for target in all_targets
        }
        certified_ids = {
            target.id
            for target in all_targets
            if target.status in ACCEPTED_MATCH_STATUSES
            and equivalence_certificate_error(rows_by_id[target.id], rows_by_id) is None
        }
        return [
            t for t in candidates
            if not t.extra.get("called_functions", [])
            or all(fid in certified_ids for fid in t.extra.get("called_functions", []))
        ]

    def _improve_candidates(self, *, certified_funcs: bool = False) -> List[str]:
        selected: List[str] = []
        source_cache: Dict[Path, str] = {}
        units = self.project.load_objdiff_units()
        unit_by_hint = {
            hint: unit
            for unit in units
            for hint in {unit.name, unit.name.removeprefix("main/")}
        }
        retail_symbols: Dict[Path, set[str]] = {}
        raw_targets = load_targets(self.config)
        for target in raw_targets:
            if (
                target.status in {"FULL_MATCH", "EQUIVALENT_MATCH"}
                or not target.buildable
                or not target.symbol
                or target.source is None
                or not target.source.is_file()
                or not target.unit
            ):
                continue
            source = source_cache.get(target.source)
            if source is None:
                source = target.source.read_text(encoding="utf-8")
                source_cache[target.source] = source
            try:
                _find_function_region(source, target)
                unit = unit_by_hint.get(target.unit)
                if unit is None:
                    continue
                if unit.target_path is None or not unit.target_path.is_file():
                    continue
                symbols = retail_symbols.get(unit.target_path)
                if symbols is None:
                    symbols = {fn.name for fn in list_text_functions(unit.target_path)}
                    retail_symbols[unit.target_path] = symbols
                if target.symbol not in symbols:
                    continue
            except (OSError, ValueError):
                continue
            selected.append(target.id)
        if certified_funcs:
            selected = [
                tid for tid in selected
                if tid in {t.id for t in self._filter_certified_funcs(raw_targets, raw_targets)}
            ]
        return selected

    def _tu_completion_candidates(self) -> List[str]:
        report_path = self.config.resolve(self.config.report_cache)
        report = json.loads(report_path.read_text(encoding="utf-8"))
        units = {unit.name: unit for unit in self.project.load_objdiff_units()}
        targets = load_targets(self.config)
        targets_by_symbol: Dict[str, List[Target]] = {}
        for target in targets:
            if target.symbol:
                targets_by_symbol.setdefault(target.symbol, []).append(target)
        selected: List[str] = []
        for entry in report.get("units", []):
            name = str(entry.get("name", ""))
            unit = units.get(name)
            functions = entry.get("functions", [])
            measures = entry.get("measures", {})
            if (
                unit is None
                or unit.source_path is None
                or not unit.source_path.is_file()
                or unit.base_path is None
                or not unit.base_path.is_file()
                or unit.target_path is None
                or not unit.target_path.is_file()
                or not functions
                or any(float(fn.get("fuzzy_match_percent", 0.0) or 0.0) <= 0.0 for fn in functions)
                or (
                    float(measures.get("matched_code_percent", 0.0) or 0.0) >= 100.0
                    and float(measures.get("matched_data_percent", 0.0) or 0.0) >= 100.0
                )
            ):
                continue
            source = unit.source_path.read_text(encoding="utf-8")
            if _find_tu_slots(source) or _has_mapped_imperfect_function(
                name, unit.source_path, source, functions, targets_by_symbol
            ):
                selected.append(name.removeprefix("main/"))
        return selected

    def build_prompt(
        self,
        workflow: str,
        target_id: str,
        history: List[Dict[str, Any]],
        options: Optional[Dict[str, Any]] = None,
    ) -> str:
        if workflow == "tu-complete":
            return self._build_tu_prompt(
                target_id,
                history,
                full_context=bool((options or {}).get("full_context")),
            )
        return self._build_function_prompt(workflow, target_id, history)

    def build_context_files(
        self,
        workflow: str,
        target_id: str,
        history: List[Dict[str, Any]],
        prompt: str,
        options: Optional[Dict[str, Any]] = None,
    ) -> Dict[str, str]:
        """All workflows use one frozen, self-contained prompt."""
        return {}

    @staticmethod
    def model_context_mode(workflow: str) -> str:
        return "inline"

    def _retail_assembly(self, target: Target) -> str:
        assert target.unit is not None and target.symbol is not None
        unit = self.project.resolve_unit(target.unit)
        relative = unit.name.removeprefix("main/")
        candidates = [
            self.root / "build" / self.config.region / "asm" / f"{relative}.s",
            self.root / "build" / self.config.region / "asm" / f"{target.unit}.s",
        ]
        for path in candidates:
            if not path.is_file():
                continue
            block = _assembly_function_block(path.read_text(encoding="utf-8"), target.symbol)
            if block:
                return block
        return ""

    def _accepted_function_context(self, target: Target) -> str:
        assert target.unit is not None
        blocks: List[str] = []
        for item in load_targets(self.config):
            if (
                item.id == target.id
                or item.unit != target.unit
                or item.status not in {"FULL_MATCH", "EQUIVALENT_MATCH"}
                or item.source is None
                or not item.source.is_file()
            ):
                continue
            source = item.source.read_text(encoding="utf-8")
            try:
                region = _find_function_region(source, item)
            except ValueError:
                continue
            body = source[region.content_start : region.content_end].strip()
            blocks.append(f"// {item.id}: {item.status}\n{body}")
            if len(blocks) >= self.context_similar_limit:
                break
        return "\n\n".join(blocks) + ("\n" if blocks else "")

    def _build_function_prompt(
        self, workflow: str, target_id: str, history: List[Dict[str, Any]]
    ) -> str:
        target = self._target(target_id)
        assert target.source is not None and target.unit is not None and target.symbol is not None
        unit = self.project.resolve_unit(target.unit)
        if unit.target_path is None or not unit.target_path.is_file():
            raise FileNotFoundError(f"Retail split object is missing for {target.unit}")
        function = extract_function(unit.target_path, target.symbol)
        source = target.source.read_text(encoding="utf-8")
        region = _find_function_region(source, target)
        current_function = source[region.content_start : region.content_end].strip()
        # Oldest→newest so the cacheable prefix through the older attempts stays
        # byte-stable as new attempts append at the tail. Slice the most recent 12
        # after the sort to bound prompt size and keep the oldest portion stable.
        chronological = sorted(history, key=_attempt_sort_key)
        prior_attempts = (
            [_compact_prior_attempt(row) for row in chronological[-12:]]
            if workflow == "improve"
            else []
        )
        # Sort the per-unit accepted-sibling list deterministically; a mid-batch
        # target insertion must not perturb the order of this cacheable block.
        same_unit = sorted(
            (
                {"id": item.id, "function": item.function, "status": item.status}
                for item in load_targets(self.config)
                if item.unit == target.unit
                and item.id != target.id
                and item.status in {"FULL_MATCH", "EQUIVALENT_MATCH"}
            ),
            key=lambda row: row["id"],
        )[:12]
        # Dossier field order is deliberate: cache-stable per-target fields first
        # (target identity, retail bytecode, edit boundary), unit-scoped stable
        # fields next (accepted siblings), volatile tail last (prior attempts and
        # knowledge hints). DeepSeek's prefix cache breaks at the first divergent
        # byte, so placing stable bytes first maximises the cacheable prefix for
        # both repeated `new` samples and the `new`→`improve` transition on the
        # same target. The frozen knowledge bodies live in the prompt prefix via
        # `{{FROZEN_KB}}`, so the dossier only carries short hint strings.
        dossier = {
            "schema_version": 2,
            "repository": "xenoblade-wii-us",
            "workflow": workflow,
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
                "bytecode_hex": function.code.hex(),
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
            "edit_boundary": {
                "kind": "comment-markers" if region.marked else "detected-function-definition",
                "begin_marker": _begin_marker(target.id),
                "end_marker": _end_marker(target.id),
            },
            "accepted_functions_in_same_unit": same_unit,
            "prior_harness_attempts": prior_attempts,
            "knowledge_hints": _knowledge_hints(target, chronological[-12:]),
        }
        common = (self.prompt_dir / "common.md").read_text(encoding="utf-8")
        workflow_prompt = (self.prompt_dir / f"{workflow}.md").read_text(encoding="utf-8")
        return (
            common.replace("{{WORKFLOW_PROMPT}}", workflow_prompt)
            .replace("{{FROZEN_KB}}", self._frozen_kb_block())
            .replace("{{DOSSIER_JSON}}", json.dumps(dossier, separators=(",", ":")))
            .replace("{{CURRENT_FUNCTION}}", current_function)
        )

    def _build_tu_prompt(
        self, unit_hint: str, history: List[Dict[str, Any]], *, full_context: bool = False
    ) -> str:
        unit = self.project.resolve_unit(unit_hint)
        if unit.source_path is None or unit.base_path is None:
            raise ValueError(f"Unit {unit.name!r} must have source and candidate object paths")
        report = report_unit(self.project, unit)
        _require_tu_completion_ready(report)
        source = unit.source_path.read_text(encoding="utf-8")
        if len(source) > self.max_source_chars:
            raise ValueError(
                f"Source file is {len(source)} chars; limit is {self.max_source_chars}. "
                "Raise project.max_source_chars for TU completion."
            )
        chronological = sorted(history, key=_attempt_sort_key)
        recent = chronological[-12:]
        if full_context:
            template = (self.prompt_dir / "tu-complete-full.md").read_text(encoding="utf-8")
            dossier = self._tu_dossier(unit, report, recent)
            _append_tu_volatile_tail(dossier, recent, _tu_knowledge_hints(unit.name, recent))
            return (
                template.replace("{{FROZEN_KB}}", self._frozen_kb_block())
                .replace("{{DOSSIER_JSON}}", json.dumps(dossier, separators=(",", ":")))
                .replace("{{CURRENT_SOURCE}}", source)
            )

        unit_entry = _load_unit_entry(self.config, unit.name)
        mismatched_functions = [
            str(function.get("name", ""))
            for function in unit_entry.get("functions", [])
            if float(function.get("fuzzy_match_percent", 0.0)) < 100.0
        ]
        slots = _find_tu_slots(source)
        slots.update(self._function_tu_slots(unit, source, mismatched_functions, slots))
        if not slots:
            raise ValueError(
                "Targeted TU completion found no imperfect mapped functions or "
                "LLM-HARNESS-TU slots; add a slot with `run.py tu-slot`, or opt in "
                "to --full-context"
            )
        dossier = self._tu_dossier(unit, report, recent)
        dossier["source_slots"] = [
            {
                "slot_id": slot.slot_id,
                "kind": slot.kind,
                "source": source[slot.content_start : slot.content_end].strip(),
                "context_before": source[
                    max(0, slot.start - self.tu_context_chars) : slot.start
                ],
                "context_after": source[
                    slot.end : min(len(source), slot.end + self.tu_context_chars)
                ],
            }
            for slot in sorted(slots.values(), key=lambda value: value.slot_id)
        ]
        residual_names = _residual_section_names(unit_entry)
        dossier["residual_sections"] = {
            "retail": _elf_section_evidence(
                unit.target_path, residual_names, self.tu_section_byte_limit
            ),
            "candidate": _elf_section_evidence(
                unit.base_path, residual_names, self.tu_section_byte_limit
            ),
        }
        dossier["residual_functions"] = {
            "retail": _function_evidence(unit.target_path, mismatched_functions),
            "candidate": _function_evidence(unit.base_path, mismatched_functions),
        }
        text_section = next(
            (section for section in unit_entry.get("sections", []) if section.get("name") == ".text"),
            {},
        )
        if float(text_section.get("fuzzy_match_percent", 0.0)) < 100.0:
            dossier["residual_text_layout"] = {
                "retail": _text_layout_evidence(unit.target_path, self.tu_section_byte_limit),
                "candidate": _text_layout_evidence(unit.base_path, self.tu_section_byte_limit),
            }
        _append_tu_volatile_tail(dossier, recent, _tu_knowledge_hints(unit.name, recent))
        template = (self.prompt_dir / "tu-complete.md").read_text(encoding="utf-8")
        return template.replace(
            "{{FROZEN_KB}}", self._frozen_kb_block()
        ).replace(
            "{{DOSSIER_JSON}}", json.dumps(dossier, separators=(",", ":"))
        )

    def _function_tu_slots(
        self,
        unit: Any,
        source: str,
        mismatched_symbols: List[str],
        existing: Dict[str, TuSlot],
    ) -> Dict[str, TuSlot]:
        wanted = set(mismatched_symbols)
        occupied = {(slot.content_start, slot.content_end) for slot in existing.values()}
        output: Dict[str, TuSlot] = {}
        for target in load_targets(self.config):
            if not target.symbol or target.symbol not in wanted or not target.unit or target.source is None:
                continue
            try:
                if (
                    self.project.resolve_unit(target.unit).name != unit.name
                    or unit.source_path is None
                    or target.source.resolve() != unit.source_path.resolve()
                ):
                    continue
                region = _find_function_region(source, target)
            except ValueError:
                continue
            bounds = (region.content_start, region.content_end)
            if bounds in occupied:
                continue
            slot_id = f"function:{target.id}"
            output[slot_id] = TuSlot(
                slot_id=slot_id,
                kind="function",
                start=region.start,
                end=region.end,
                content_start=region.content_start,
                content_end=region.content_end,
            )
            occupied.add(bounds)
        return output

    def _tu_dossier(
        self, unit: Any, report: Any, history: List[Dict[str, Any]]
    ) -> Dict[str, Any]:
        # Leave the volatile tail (`prior_harness_attempts`, `knowledge_hints`)
        # to the caller: per-build fields like residual_sections and source_slots
        # are more stable than prior attempts (they only change when the build
        # changes, not on every experiment) and must come before them so the
        # cacheable prefix extends through the more stable fields.
        functions = sorted(
            (
                {"symbol": fn.name, "match_percent": fn.match_percent, "size": fn.size}
                for fn in report.functions
            ),
            key=lambda row: row["symbol"],
        )
        return {
            "schema_version": 2,
            "repository": "xenoblade-wii-us",
            "workflow": "tu-complete",
            "acceptance": "100% unit code and data plus split-size fit",
            "unit": unit.name,
            "source": str(unit.source_path.relative_to(self.root)),
            "code_match_percent": report.code_match_percent,
            "data_match_percent": report.data_match_percent,
            "fuzzy_match_percent": report.fuzzy_match_percent,
            "matched_functions": report.matched_functions,
            "total_functions": report.total_functions,
            "functions": functions,
        }

    def evaluate(self, workflow: str, target_id: str, candidate: Candidate) -> Evaluation:
        if workflow == "tu-complete":
            return self._evaluate_tu(target_id, candidate)
        return self._evaluate_function(target_id, candidate)

    def _evaluate_function(self, target_id: str, candidate: Candidate) -> Evaluation:
        target = self._target(target_id)
        assert target.source is not None and target.unit is not None
        original = target.source.read_text(encoding="utf-8")
        region = _find_function_region(original, target)
        self._validate_source(original[region.content_start : region.content_end], candidate.source)
        candidate_file = _replace_function_source(original, region, candidate.source)
        unit = self.project.resolve_unit(target.unit)
        original_object = unit.base_path.read_bytes() if unit.base_path and unit.base_path.is_file() else None
        try:
            target.source.write_text(candidate_file, encoding="utf-8")
            if unit.base_path is None:
                raise ValueError(f"Objdiff unit {unit.name!r} has no candidate object path")
            unit.base_path.unlink(missing_ok=True)
            build_error = self._build_object(unit.base_path)
            if build_error:
                return Evaluation(
                    status="COMPILE_ERROR",
                    match_percent=0.0,
                    accepted=False,
                    detail=build_error,
                )
            try:
                evaluation = evaluate_unit_match(
                    self.project,
                    unit,
                    target.symbol,
                    target_id=target.id,
                    certify_full_match=False,
                )
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
            if unit.target_path is None:
                raise ValueError(f"Objdiff unit {unit.name!r} has no retail object path")
            try:
                retail_function = extract_function(unit.target_path, target.symbol)
                candidate_function = extract_function(unit.base_path, target.symbol)
            except (FileNotFoundError, ValueError) as exc:
                return Evaluation(
                    status="EVALUATION_ERROR",
                    match_percent=match_percent,
                    accepted=False,
                    detail=f"function-size extraction failed: {exc}",
                )
            size_ok, size_detail = _function_size_comparison(
                retail_function.size, candidate_function.size
            )
            mismatch_detail = (
                _binary_mismatch_summary(retail_function, candidate_function)
                if match_percent < 100.0
                else ""
            )
            meets_bar = meets_required_level(
                target.required_level,
                evaluation.status,
                function_match=match_percent,
                unit=evaluation.unit_report,
                symbol=target.symbol,
                equivalence=evaluation.equivalence,
            )
            # An otherwise strong candidate can be usable before every call-graph
            # dependency has been matched. Keep it in the lower CODE_MATCH band;
            # this must not make it eligible for EQUIVALENT_MATCH certification.
            callee_pending = (
                evaluation.status == "CODE_MATCH"
                and evaluation.equivalence == ProofStatus.INCONCLUSIVE_UNVALIDATED_CALLEE
            )
            accepted = size_ok and (meets_bar or callee_pending)
            return Evaluation(
                status=evaluation.status,
                match_percent=match_percent,
                accepted=accepted,
                size_ok=size_ok,
                equivalence=evaluation.equivalence.value if evaluation.equivalence else None,
                detail="; ".join(
                    value
                    for value in (
                        evaluation.equivalence_detail,
                        size_detail,
                        mismatch_detail,
                    )
                    if value
                ),
                metrics={
                    "retail_function_size": retail_function.size,
                    "candidate_function_size": candidate_function.size,
                    "function_size_delta": candidate_function.size - retail_function.size,
                },
            )
        finally:
            target.source.write_text(original, encoding="utf-8")
            if unit.base_path:
                if original_object is None:
                    unit.base_path.unlink(missing_ok=True)
                else:
                    unit.base_path.write_bytes(original_object)

    def _evaluate_tu(self, unit_hint: str, candidate: Candidate) -> Evaluation:
        unit = self.project.resolve_unit(unit_hint)
        if unit.source_path is None or unit.base_path is None:
            raise ValueError(f"Unit {unit.name!r} must have source and candidate object paths")
        original = unit.source_path.read_text(encoding="utf-8")
        candidate_file = self._tu_candidate_source(original, candidate, unit)
        original_object = unit.base_path.read_bytes() if unit.base_path.is_file() else None
        baseline_report = report_unit(self.project, unit)
        baseline_fingerprints = _object_function_fingerprints(unit.base_path)
        try:
            unit.source_path.write_text(candidate_file, encoding="utf-8")
            unit.base_path.unlink(missing_ok=True)
            build_error = self._build_object(unit.base_path)
            if build_error:
                return Evaluation(
                    status="COMPILE_ERROR",
                    match_percent=0.0,
                    accepted=False,
                    detail=build_error,
                )
            evaluation = evaluate_unit_match(self.project, unit, None)
            candidate_fingerprints = _object_function_fingerprints(unit.base_path)
            guard = self._tu_regression_guard(
                unit.name,
                unit,
                evaluation.unit_report,
                baseline_fingerprints,
                candidate_fingerprints,
            )
            size = check_object_size(
                project_root=self.root,
                region=self.config.region,
                unit_hint=unit.name,
                retail_object=unit.target_path,
                decomp_object=unit.base_path,
            )
            accepted = guard["ok"] and size.ok and meets_required_level(
                "EQUIVALENT_MATCH",
                evaluation.status,
                function_match=None,
                unit=evaluation.unit_report,
                symbol=None,
            )
            return Evaluation(
                status=evaluation.status,
                match_percent=evaluation.unit_report.fuzzy_match_percent,
                accepted=accepted,
                size_ok=size.ok,
                detail=(
                    f"code={evaluation.unit_report.code_match_percent:.3f}%; "
                    f"data={evaluation.unit_report.data_match_percent:.3f}%; "
                    f"guard={'PASS' if guard['ok'] else 'FAIL'}; {size.notes}"
                ),
                metrics={
                    "guard_ok": guard["ok"],
                    "accepted_function_regressions": guard["regressions"],
                    "changed_equivalence_proofs": guard["proofs"],
                    "accepted_function_count": guard["accepted_function_count"],
                    "baseline_code_percent": baseline_report.code_match_percent,
                    "baseline_data_percent": baseline_report.data_match_percent,
                    "code_percent": evaluation.unit_report.code_match_percent,
                    "data_percent": evaluation.unit_report.data_match_percent,
                    "fuzzy_percent": evaluation.unit_report.fuzzy_match_percent,
                },
            )
        finally:
            unit.source_path.write_text(original, encoding="utf-8")
            if original_object is None:
                unit.base_path.unlink(missing_ok=True)
            else:
                unit.base_path.write_bytes(original_object)

    def _tu_candidate_source(self, original: str, candidate: Candidate, unit: Any) -> str:
        if candidate.patches:
            if candidate.source.strip():
                raise ValueError("TU candidate cannot contain both source and slot patches")
            slots = _find_tu_slots(original)
            for patch in candidate.patches:
                if not patch.slot_id.startswith("function:"):
                    continue
                target_id = patch.slot_id.removeprefix("function:")
                target = self._target(target_id)
                assert target.unit is not None and target.source is not None
                if (
                    self.project.resolve_unit(target.unit).name != unit.name
                    or unit.source_path is None
                    or target.source.resolve() != unit.source_path.resolve()
                ):
                    raise ValueError(
                        f"Function slot {patch.slot_id!r} does not belong to unit {unit.name!r}"
                    )
                region = _find_function_region(original, target)
                slots[patch.slot_id] = TuSlot(
                    slot_id=patch.slot_id,
                    kind="function",
                    start=region.start,
                    end=region.end,
                    content_start=region.content_start,
                    content_end=region.content_end,
                )
            if not slots:
                raise ValueError("TU candidate patches do not resolve to any source slots")
            seen: set[str] = set()
            for patch in candidate.patches:
                if patch.slot_id in seen:
                    raise ValueError(f"TU candidate repeats slot {patch.slot_id!r}")
                seen.add(patch.slot_id)
                slot = slots.get(patch.slot_id)
                if slot is None:
                    raise ValueError(f"TU candidate references unknown slot {patch.slot_id!r}")
                self._validate_source(
                    original[slot.content_start : slot.content_end], patch.source
                )
            return _apply_tu_patches(original, slots, candidate.patches)
        self._validate_source(original, candidate.source)
        return candidate.source

    def _tu_regression_guard(
        self,
        unit_name: str,
        unit: Any,
        candidate_report: Any,
        baseline_fingerprints: Dict[str, str],
        candidate_fingerprints: Dict[str, str],
    ) -> Dict[str, Any]:
        functions = {fn.name: fn for fn in candidate_report.functions}
        accepted_targets: List[Target] = []
        for target in load_targets(self.config):
            if not target.unit or target.status not in {"FULL_MATCH", "EQUIVALENT_MATCH"} or not target.symbol:
                continue
            try:
                same_unit = self.project.resolve_unit(target.unit).name == unit_name
            except ValueError:
                continue
            if same_unit:
                accepted_targets.append(target)
        regressions: List[str] = []
        proofs: List[Dict[str, str]] = []
        preserved_equivalent = 0
        for target in accepted_targets:
            assert target.symbol is not None
            fn = functions.get(target.symbol)
            percent = fn.match_percent if fn else 0.0
            if percent >= 100.0:
                continue
            if target.status == "FULL_MATCH":
                regressions.append(f"{target.id}: FULL_MATCH -> {percent:.3f}%")
                continue
            unchanged = (
                baseline_fingerprints.get(target.symbol) is not None
                and baseline_fingerprints.get(target.symbol) == candidate_fingerprints.get(target.symbol)
            )
            if unchanged:
                preserved_equivalent += 1
                continue
            probe = prove_unit_symbol(self.project, unit, target.symbol)
            proofs.append({"target_id": target.id, "status": probe.status.value})
            if probe.status == ProofStatus.EQUIVALENT:
                preserved_equivalent += 1
            else:
                regressions.append(
                    f"{target.id}: changed EQUIVALENT_MATCH -> {percent:.3f}%/{probe.status.value}"
                )
        full_functions = sum(fn.match_percent >= 100.0 for fn in candidate_report.functions)
        return {
            "ok": not regressions,
            "regressions": regressions,
            "proofs": proofs,
            "accepted_function_count": full_functions + preserved_equivalent,
        }

    def rank_candidate(self, workflow: str, evaluation: Dict[str, Any]) -> tuple[Any, ...]:
        if workflow != "tu-complete":
            return (bool(evaluation.get("accepted")), evaluation.get("match_percent") or 0.0)
        metrics = evaluation.get("metrics") or {}
        return (
            bool(evaluation.get("accepted")),
            bool(metrics.get("guard_ok")),
            int(metrics.get("accepted_function_count") or 0),
            float(metrics.get("data_percent") or 0.0),
            float(metrics.get("code_percent") or 0.0),
            float(metrics.get("fuzzy_percent") or 0.0),
        )

    def _validate_source(self, original: str, candidate: str) -> None:
        if not candidate.strip():
            raise ValueError("Candidate source is empty")
        if len(candidate) > self.max_source_chars * 2:
            raise ValueError("Candidate source exceeds the safety limit")
        forbidden = ("asm {", "asm void", "register r", "asm(\"r", "sp[")
        added = [token for token in forbidden if candidate.count(token) > original.count(token)]
        if added:
            raise ValueError(f"Candidate adds forbidden low-level source patterns: {', '.join(added)}")

    def _build_object(self, object_path: Path) -> str:
        completed = subprocess.run(
            [self.project.ninja_bin(), str(object_path.relative_to(self.root))],
            cwd=self.root,
            text=True,
            capture_output=True,
            check=False,
        )
        if completed.returncode == 0:
            return ""
        detail = "\n".join(
            value.strip() for value in (completed.stdout, completed.stderr) if value.strip()
        )
        return f"build exited {completed.returncode}: {detail[-4000:]}"

    def prepare(self, target_id: str, *, write: bool = False) -> str:
        """Add stable comments around an existing definition for future replacements."""
        target = self._target(target_id)
        assert target.source is not None
        source = target.source.read_text(encoding="utf-8")
        region = _find_function_region(source, target)
        if region.marked:
            return f"{target.source}: markers already present"
        marked = (
            source[: region.start]
            + _begin_marker(target.id)
            + "\n"
            + source[region.start : region.end]
            + "\n"
            + _end_marker(target.id)
            + source[region.end :]
        )
        if write:
            target.source.write_text(marked, encoding="utf-8")
        return _source_diff(target.source, source, marked)

    def create_slot(
        self,
        target_id: str,
        file: Path,
        *,
        before: str = "",
        after: str = "",
        unit: str = "",
        write: bool = False,
    ) -> str:
        target = self._any_target(target_id)
        path = file if file.is_absolute() else self.root / file
        path = path.resolve()
        path.relative_to(self.root)
        if path.suffix not in {".c", ".cc", ".cpp", ".cxx"} or not path.is_file():
            raise ValueError("Marker slots require an existing C/C++ source file")
        if bool(before) == bool(after):
            raise ValueError("Specify exactly one of before or after")
        source = path.read_text(encoding="utf-8")
        if _begin_marker(target.id) in source or _end_marker(target.id) in source:
            raise ValueError(f"Markers for {target.id} already exist in {path}")
        updated = _insert_marker_slot(source, target, before=before, after=after)
        if target.source is not None and target.source.resolve() != path:
            raise ValueError(f"Target already belongs to a different source: {target.source}")
        if target.source is not None:
            try:
                _find_function_region(source, target)
            except ValueError:
                pass
            else:
                raise ValueError(
                    f"Target {target.id!r} already has a definition; use prepare instead"
                )
        selected_unit = unit or target.unit or ""
        if not selected_unit:
            raise ValueError("An unassigned target requires --unit")
        self.project.resolve_unit(selected_unit)
        if write:
            path.write_text(updated, encoding="utf-8")
            try:
                self._update_target_location(target.id, path, selected_unit)
            except Exception:
                path.write_text(source, encoding="utf-8")
                raise
        note = f"\n# registry: source={path.relative_to(self.root)} unit={selected_unit}\n"
        return _source_diff(path, source, updated) + note

    def _update_target_location(self, target_id: str, source: Path, unit: str) -> None:
        document = load_targets_document(self.config)
        for row in document.get("targets", []):
            if row.get("id") != target_id:
                continue
            row["source"] = source.relative_to(self.root).as_posix()
            row["unit"] = unit
            if row.get("workflow_status") in {None, "DISCOVERY", "BACKLOG"}:
                row["workflow_status"] = "CLAIMED" if row.get("claim") else "BACKLOG"
            write_targets_document(self.config, document)
            return
        raise KeyError(f"Unknown target id: {target_id}")

    def create_tu_slot(
        self,
        unit_hint: str,
        slot_id: str,
        file: Path,
        *,
        before: str = "",
        after: str = "",
        start: str = "",
        end: str = "",
        write: bool = False,
    ) -> str:
        if not re.fullmatch(r"[A-Za-z0-9][A-Za-z0-9._-]*", slot_id):
            raise ValueError("TU slot IDs may contain only letters, digits, dot, underscore, and dash")
        unit = self.project.resolve_unit(unit_hint)
        if unit.source_path is None:
            raise ValueError(f"Unit {unit.name!r} has no source path")
        path = file if file.is_absolute() else self.root / file
        path = path.resolve()
        path.relative_to(self.root)
        if path != unit.source_path.resolve():
            raise ValueError(f"TU slots must be in the unit source: {unit.source_path}")
        if path.suffix not in {".c", ".cc", ".cpp", ".cxx"} or not path.is_file():
            raise ValueError("TU slots require an existing C/C++ source file")
        source = path.read_text(encoding="utf-8")
        if slot_id in _find_tu_slots(source):
            raise ValueError(f"TU slot {slot_id!r} already exists in {path}")
        insertion_mode = bool(before) + bool(after)
        wrapping_mode = bool(start) or bool(end)
        if insertion_mode == 1 and not wrapping_mode:
            updated = _insert_empty_tu_slot(source, slot_id, before=before, after=after)
        elif insertion_mode == 0 and bool(start) and bool(end):
            updated = _wrap_tu_slot(source, slot_id, start=start, end=end)
        else:
            raise ValueError(
                "Use exactly one of --before/--after, or use --start and --end together"
            )
        if write:
            path.write_text(updated, encoding="utf-8")
        return _source_diff(path, source, updated)

    def promote(
        self,
        workflow: str,
        target_id: str,
        candidate: Candidate,
        *,
        write: bool = False,
    ) -> str:
        if workflow == "tu-complete":
            unit = self.project.resolve_unit(target_id)
            if unit.source_path is None:
                raise ValueError(f"Unit {unit.name!r} has no source path")
            source = unit.source_path.read_text(encoding="utf-8")
            updated = self._tu_candidate_source(source, candidate, unit)
            if write:
                unit.source_path.write_text(updated, encoding="utf-8")
            return _source_diff(unit.source_path, source, updated)
        target = self._target(target_id)
        assert target.source is not None
        source = target.source.read_text(encoding="utf-8")
        region = _find_function_region(source, target)
        updated = _replace_function_source(source, region, candidate.source)
        if write:
            target.source.write_text(updated, encoding="utf-8")
        return _source_diff(target.source, source, updated)

    @staticmethod
    def _require_claim(target: Target, owner: str) -> None:
        pass

    @staticmethod
    def _require_unit_claims(unit_name: str, owner: str) -> None:
        pass

    def finalize(self) -> None:
        # Each candidate evaluation restores both source and its prior object bytes.
        pass

    def prepare_workspace(self, workspace: Path) -> None:
        """Share immutable retail/toolchain inputs, then configure an isolated build tree."""
        coop_source = self.config.resolve("coop.json")
        if coop_source.is_file():
            shutil.copy2(coop_source, workspace / "coop.json")
        _replace_with_symlink(
            self.root / "orig" / self.config.region,
            workspace / "orig" / self.config.region,
        )
        build = workspace / "build"
        build.mkdir(exist_ok=True)
        for name in ("tools", "compilers", "binutils"):
            _symlink_if_present(self.root / "build" / name, build / name)
        region_build = build / self.config.region
        region_build.mkdir(parents=True, exist_ok=True)
        for name in ("obj", "include", "asm", "config.json"):
            _symlink_if_present(
                self.root / "build" / self.config.region / name,
                region_build / name,
            )
        _hardlink_objects(
            self.root / "build" / self.config.region / "src",
            region_build / "src",
        )
        command = [sys.executable, "configure.py", "--version", self.config.region]
        command.extend(self.config.configure_args)
        shared_tools = self.root / "build" / "tools"
        explicit_paths = [
            ("--dtk", shared_tools / "dtk"),
            ("--objdiff", shared_tools / "objdiff-cli"),
            ("--binutils", self.root / "build" / "binutils"),
            ("--compilers", self.root / "build" / "compilers"),
            ("--sjiswrap", shared_tools / "sjiswrap.exe"),
            ("--wrapper", shared_tools / "wibo"),
        ]
        for flag, path in explicit_paths:
            if path.exists():
                command.extend([flag, str(path)])
        # These are intentional retail data-only split units.  They have no
        # source-side Object entries, so configure.py reports them while
        # generating the isolated worktree build files.  Keep the normal
        # configure.py diagnostics intact elsewhere, but avoid repeating this
        # known noise for every harness candidate worktree.
        hidden_config_warnings = {
            "Missing configuration for split1.s",
            "Missing configuration for criware_data.s",
            "Missing configuration for nw4r_data.s",
            "Missing configuration for monolibdata1.s",
            "Missing configuration for monolibdata2.s",
        }
        try:
            result = subprocess.run(
                command,
                cwd=workspace,
                check=True,
                text=True,
                capture_output=True,
            )
        except subprocess.CalledProcessError as exc:
            if exc.stdout:
                sys.stdout.write(exc.stdout)
            if exc.stderr:
                sys.stderr.write(exc.stderr)
            raise
        if result.stdout:
            for line in result.stdout.splitlines(keepends=True):
                if line.strip() not in hidden_config_warnings:
                    sys.stdout.write(line)
        if result.stderr:
            sys.stderr.write(result.stderr)


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


def _assembly_function_block(source: str, symbol: str) -> str:
    start_pattern = re.compile(rf'^\.fn\s+"?{re.escape(symbol)}"?\s*,.*$', re.MULTILINE)
    start = start_pattern.search(source)
    if start is None:
        return ""
    end_pattern = re.compile(rf'^\.endfn\s+"?{re.escape(symbol)}"?\s*$', re.MULTILINE)
    end = end_pattern.search(source, start.end())
    if end is None:
        return ""
    line_end = source.find("\n", end.end())
    return source[start.start() : (line_end + 1 if line_end >= 0 else end.end())]


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


def _tu_knowledge_queries(unit_name: str, history: List[Dict[str, Any]]) -> List[Dict[str, Any]]:
    queries: List[Dict[str, Any]] = [
        {"query": unit_name, "mode": "all", "reason": "translation-unit identity"},
        {
            "query": "relocation literal_pool vtable size",
            "mode": "any",
            "reason": "whole-unit closure categories",
        },
    ]
    for tag in infer_tags(json.dumps(history, ensure_ascii=False)):
        queries.append(
            {
                "query": tag,
                "tag": tag,
                "mode": "all",
                "reason": "prior TU attempt mismatch category",
            }
        )
    return queries[:6]


def _attempt_sort_key(row: Dict[str, Any]) -> str:
    """Oldest→newest ordering key for prior attempts.

    `timestamp` is the ISO-formatted completion time the harness records for
    every run. Sorting prior attempts oldest→newest keeps the head of the
    `prior_harness_attempts` array byte-identical as new attempts append at
    the tail: the cacheable prefix grows through the older entries and only
    the trailing newly-appended entry perturbs DeepSeek's prefix cache.
    """
    return str(row.get("timestamp") or "")


def _knowledge_hints(target: Target, history: List[Dict[str, Any]]) -> List[str]:
    """Short symptom strings the model uses to pick frozen-KB entries by ID.

    Replaces per-experiment SQLite retrieval. The bodies of every KB entry are
    already inlined in the prompt's frozen prefix, so the dossier only needs to
    flag which entries are likely relevant (target identity + tags inferred
    from prior attempt notes). Order is deterministic: identity tokens first,
    then sorted inferred tags.
    """
    raw: List[str] = []
    seen: set[str] = set()
    for value in (target.function, target.symbol or ""):
        value = value.strip()
        if value and value not in seen:
            seen.add(value)
            raw.append(value)
    if history:
        history_text = json.dumps(history, ensure_ascii=False)
        for tag in sorted(infer_tags(history_text)):
            if tag not in seen:
                seen.add(tag)
                raw.append(tag)
    return raw[:12]


def _tu_knowledge_hints(unit_name: str, history: List[Dict[str, Any]]) -> List[str]:
    """Same as `_knowledge_hints` but for the TU workflow."""
    raw: List[str] = []
    seen: set[str] = set()
    name = unit_name.strip()
    if name:
        raw.append(name)
        seen.add(name)
    raw.append("relocation")
    raw.append("literal_pool")
    raw.append("vtable")
    raw.append("size")
    seen.update(raw)
    if history:
        history_text = json.dumps(history, ensure_ascii=False)
        for tag in sorted(infer_tags(history_text)):
            if tag not in seen:
                seen.add(tag)
                raw.append(tag)
    return raw[:16]


def _compact_prior_attempt(row: Dict[str, Any]) -> Dict[str, Any]:
    """Project a prior attempt into a small, cache-stable shape.

    Drops the verbose `evaluation` and `candidate_summary` blobs (which can
    each be thousands of tokens) in favour of the few fields the model uses to
    decide what to change next. Keeps the cache break point small and the
    older-but-shared tail entries nearly byte-identical across experiments.
    """
    evaluation = row.get("evaluation") or {}
    summary = row.get("candidate_summary") or {}
    notes = summary.get("notes") if isinstance(summary, dict) else None
    if not isinstance(notes, list):
        notes = []
    return {
        "m": str(row.get("model_id") or ""),
        "workflow": str(row.get("workflow") or ""),
        "status": str(evaluation.get("status") or ""),
        "match": evaluation.get("match_percent"),
        "accepted": bool(evaluation.get("accepted")),
        "size_ok": evaluation.get("size_ok"),
        "eq": evaluation.get("equivalence"),
        "h": str(summary.get("hypothesis") or ""),
        "next": str(summary.get("next_change") or ""),
        "notes": [str(item) for item in notes],
        "e": str(row.get("error") or "") or None,
    }


def _append_tu_volatile_tail(
    dossier: Dict[str, Any],
    recent_history: List[Dict[str, Any]],
    knowledge_hints: List[str],
) -> None:
    """Attach the TU dossier volatile tail (oldest→newest attempts, hints last).

    Called after all per-build residual evidence has been written into the
    dossier, so these two fields land as the final keys — matching the
    function-workflow ordering convention of stable-before-volatile.
    """
    dossier["prior_harness_attempts"] = [_compact_prior_attempt(row) for row in recent_history]
    dossier["knowledge_hints"] = knowledge_hints


def _begin_marker(target_id: str) -> str:
    return f"// LLM-HARNESS-BEGIN: {target_id}"


def _end_marker(target_id: str) -> str:
    return f"// LLM-HARNESS-END: {target_id}"


def _tu_begin_marker(slot_id: str) -> str:
    return f"// LLM-HARNESS-TU-BEGIN: {slot_id}"


def _tu_end_marker(slot_id: str) -> str:
    return f"// LLM-HARNESS-TU-END: {slot_id}"


def _find_tu_slots(source: str) -> Dict[str, TuSlot]:
    begin_pattern = re.compile(
        r"(?m)^[ \t]*// LLM-HARNESS-TU-BEGIN: ([A-Za-z0-9][A-Za-z0-9._-]*)[ \t]*$"
    )
    end_pattern = re.compile(
        r"(?m)^[ \t]*// LLM-HARNESS-TU-END: ([A-Za-z0-9][A-Za-z0-9._-]*)[ \t]*$"
    )
    begins = list(begin_pattern.finditer(source))
    ends = list(end_pattern.finditer(source))
    slots: Dict[str, TuSlot] = {}
    used_ends: set[int] = set()
    for begin in begins:
        slot_id = begin.group(1)
        if slot_id in slots:
            raise ValueError(f"Duplicate TU begin marker for {slot_id!r}")
        matching = [
            (index, end)
            for index, end in enumerate(ends)
            if index not in used_ends and end.group(1) == slot_id and end.start() > begin.end()
        ]
        if len(matching) != 1:
            raise ValueError(f"TU slot {slot_id!r} must have exactly one following end marker")
        end_index, end = matching[0]
        nested = next(
            (other.group(1) for other in begins if begin.end() < other.start() < end.start()),
            None,
        )
        if nested is not None:
            raise ValueError(f"TU slots may not nest ({slot_id!r} contains {nested!r})")
        content_start = source.find("\n", begin.end())
        if content_start < 0:
            raise ValueError(f"TU begin marker for {slot_id!r} has no following source")
        content_start += 1
        slots[slot_id] = TuSlot(
            slot_id=slot_id,
            kind="tu",
            start=begin.start(),
            end=end.end(),
            content_start=content_start,
            content_end=end.start(),
        )
        used_ends.add(end_index)
    if len(used_ends) != len(ends):
        stray = next(end.group(1) for index, end in enumerate(ends) if index not in used_ends)
        raise ValueError(f"Stray TU end marker for {stray!r}")
    return slots


def _apply_tu_patches(
    source: str, slots: Dict[str, TuSlot], patches: List[SourcePatch]
) -> str:
    updated = source
    replacements = sorted(
        ((slots[patch.slot_id], patch.source.strip()) for patch in patches),
        key=lambda value: value[0].content_start,
        reverse=True,
    )
    for slot, replacement in replacements:
        updated = (
            updated[: slot.content_start]
            + replacement
            + "\n"
            + updated[slot.content_end :]
        )
    return updated


def _insert_empty_tu_slot(
    source: str, slot_id: str, *, before: str = "", after: str = ""
) -> str:
    if bool(before) == bool(after):
        raise ValueError("Specify exactly one of before or after")
    anchor = before or after
    count = source.count(anchor)
    if count != 1:
        raise ValueError(f"Exact anchor must occur once; found {count}")
    position = source.index(anchor) if before else source.index(anchor) + len(anchor)
    block = (
        "\n"
        + _tu_begin_marker(slot_id)
        + "\n"
        + f"// LLM-HARNESS-TU-SLOT: {slot_id}\n"
        + _tu_end_marker(slot_id)
        + "\n"
    )
    return source[:position] + block + source[position:]


def _wrap_tu_slot(source: str, slot_id: str, *, start: str, end: str) -> str:
    start_count = source.count(start)
    end_count = source.count(end)
    if start_count != 1 or end_count != 1:
        raise ValueError(
            "Exact wrap anchors must each occur once; "
            f"start={start_count}, end={end_count}"
        )
    begin_at = source.index(start)
    end_at = source.index(end) + len(end)
    if end_at <= begin_at:
        raise ValueError("TU slot end anchor must follow its start anchor")
    return (
        source[:begin_at]
        + _tu_begin_marker(slot_id)
        + "\n"
        + source[begin_at:end_at]
        + "\n"
        + _tu_end_marker(slot_id)
        + source[end_at:]
    )


def _load_unit_entry(config: Any, unit_name: str) -> Dict[str, Any]:
    report_path = config.resolve(config.report_cache)
    report = json.loads(report_path.read_text(encoding="utf-8"))
    for entry in report.get("units", []):
        if entry.get("name") == unit_name:
            return entry
    raise ValueError(f"Unit {unit_name!r} is missing from the objdiff report")


def _residual_section_names(entry: Dict[str, Any]) -> List[str]:
    names = []
    for section in entry.get("sections", []):
        percent = section.get("fuzzy_match_percent")
        if percent is None or float(percent) < 100.0:
            name = str(section.get("name", ""))
            if name and name != ".text":
                names.append(name)
    return names


def _function_evidence(
    path: Optional[Path], names: List[str]
) -> List[Dict[str, Any]]:
    if path is None or not path.is_file():
        return [{"error": f"object is missing: {path}"}]
    functions = {function.name: function for function in list_text_functions(path)}
    output: List[Dict[str, Any]] = []
    for name in names:
        function = functions.get(name)
        if function is None:
            output.append({"name": name, "missing": True})
            continue
        output.append(
            {
                "name": name,
                "offset": f"0x{function.value:X}",
                "size": function.size,
                "bytecode_hex": function.code.hex(),
                "relocations": [
                    {
                        "offset": f"0x{relocation.offset:X}",
                        "type": relocation.relocation_type,
                        "symbol": relocation.symbol,
                        "addend": relocation.addend,
                    }
                    for relocation in function.relocations
                ],
            }
        )
    return output


def _text_layout_evidence(path: Optional[Path], byte_limit: int) -> Dict[str, Any]:
    if path is None or not path.is_file():
        return {"error": f"object is missing: {path}"}
    data = path.read_bytes()
    _require_elf32_be(data, path)
    sections, by_name = _section_table(data)
    text_index = by_name.get(".text")
    if text_index is None:
        return {"error": ".text section is missing"}
    text = sections[text_index]
    raw = data[int(text["offset"]) : int(text["offset"]) + int(text["size"])]
    functions = sorted(list_text_functions(path), key=lambda function: function.value)
    gaps: List[Dict[str, Any]] = []
    cursor = 0
    remaining = byte_limit
    for function in functions:
        if function.value > cursor:
            gap = raw[cursor : function.value]
            shown = gap[:remaining]
            gaps.append(
                {
                    "offset": f"0x{cursor:X}",
                    "size": len(gap),
                    "bytes_hex": shown.hex(),
                    "bytes_truncated": len(shown) < len(gap),
                }
            )
            remaining = max(0, remaining - len(shown))
        cursor = max(cursor, function.value + function.size)
    if cursor < len(raw):
        gap = raw[cursor:]
        shown = gap[:remaining]
        gaps.append(
            {
                "offset": f"0x{cursor:X}",
                "size": len(gap),
                "bytes_hex": shown.hex(),
                "bytes_truncated": len(shown) < len(gap),
            }
        )
    text_metadata = _elf_section_evidence(path, [".text"], 0)[0]
    spans = [(function.value, function.value + function.size) for function in functions]
    gap_relocations = [
        relocation
        for relocation in text_metadata.get("relocations", [])
        if not any(begin <= int(relocation["offset"], 16) < end for begin, end in spans)
    ]
    return {
        "section_size": len(raw),
        "symbols": [
            {"name": function.name, "offset": f"0x{function.value:X}", "size": function.size}
            for function in functions
        ],
        "non_function_gaps": gaps,
        "non_function_relocations": gap_relocations,
    }


def _elf_section_evidence(
    path: Optional[Path], names: List[str], byte_limit: int
) -> List[Dict[str, Any]]:
    if path is None or not path.is_file():
        return [{"error": f"object is missing: {path}"}]
    data = path.read_bytes()
    elf_type = _require_elf32_be(data, path)
    sections, by_name = _section_table(data)
    sym_index = by_name.get(".symtab")
    symtab = sections[sym_index] if sym_index is not None else None
    strtab = sections[int(symtab["link"])] if symtab is not None else None
    evidence: List[Dict[str, Any]] = []
    for name in names:
        section_index = by_name.get(name)
        if section_index is None:
            evidence.append({"name": name, "missing": True})
            continue
        section = sections[section_index]
        size = int(section["size"])
        raw = (
            b""
            if int(section["type"]) == SHT_NOBITS
            else data[int(section["offset"]) : int(section["offset"]) + size]
        )
        relocations: List[Dict[str, Any]] = []
        if sym_index is not None and symtab is not None and strtab is not None:
            sym_entsize = int(symtab["entsize"]) or 16
            for relocation_section in sections:
                relocation_type = int(relocation_section["type"])
                if (
                    relocation_type not in {SHT_REL, SHT_RELA}
                    or int(relocation_section["info"]) != section_index
                    or int(relocation_section["link"]) != sym_index
                ):
                    continue
                entry_size = 12 if relocation_type == SHT_RELA else 8
                entsize = int(relocation_section["entsize"]) or entry_size
                for index in range(int(relocation_section["size"]) // entsize):
                    offset = int(relocation_section["offset"]) + index * entsize
                    relocation_offset, info = struct.unpack_from(">II", data, offset)
                    symbol_index = info >> 8
                    symbol_offset = int(symtab["offset"]) + symbol_index * sym_entsize
                    symbol_name_offset = struct.unpack_from(">I", data, symbol_offset)[0]
                    symbol_name = _symbol_name(
                        data,
                        int(strtab["offset"]),
                        int(strtab["size"]),
                        symbol_name_offset,
                    )
                    addend = (
                        struct.unpack_from(">i", data, offset + 8)[0]
                        if relocation_type == SHT_RELA
                        else None
                    )
                    section_offset = (
                        relocation_offset
                        if elf_type == ET_REL
                        else relocation_offset - int(section["addr"])
                    )
                    relocations.append(
                        {
                            "offset": f"0x{section_offset:X}",
                            "type": info & 0xFF,
                            "symbol": symbol_name,
                            "addend": addend,
                        }
                    )
        shown = raw[:byte_limit]
        evidence.append(
            {
                "name": name,
                "size": size,
                "alignment": int(section["align"]),
                "bytes_hex": shown.hex(),
                "bytes_truncated": len(raw) > len(shown),
                "relocations": relocations,
            }
        )
    return evidence


def _find_function_region(source: str, target: Target) -> SourceRegion:
    begin = _begin_marker(target.id)
    end = _end_marker(target.id)
    begin_pos = source.find(begin)
    end_pos = source.find(end)
    if begin_pos >= 0 or end_pos >= 0:
        if begin_pos < 0 or end_pos < 0 or end_pos <= begin_pos:
            raise ValueError(f"Malformed harness markers for {target.id}")
        content_start = source.find("\n", begin_pos)
        if content_start < 0:
            raise ValueError(f"Begin marker for {target.id} has no following source")
        content_start += 1
        return SourceRegion(
            start=begin_pos,
            end=end_pos + len(end),
            content_start=content_start,
            content_end=end_pos,
            marked=True,
        )

    function_identity = target.function.split("(", 1)[0].strip()
    parts = function_identity.split("::")
    qualified = "::".join(parts[-2:]) if len(parts) >= 2 else function_identity
    pattern = re.compile(re.escape(qualified) + r"\s*\(")
    for match in pattern.finditer(source):
        brace = _signature_open_brace(source, match.end() - 1)
        if brace is None:
            continue
        close = _matching_brace(source, brace)
        line_start = source.rfind("\n", 0, match.start()) + 1
        return SourceRegion(
            start=line_start,
            end=close + 1,
            content_start=line_start,
            content_end=close + 1,
            marked=False,
        )
    raise ValueError(
        f"Could not locate {target.function} in {target.source}; add stable harness markers first"
    )


def _replace_function_source(source: str, region: SourceRegion, replacement: str) -> str:
    value = replacement.strip()
    if region.marked:
        value += "\n"
    return source[: region.content_start] + value + source[region.content_end :]


def _require_tu_completion_ready(report: Any) -> None:
    untouched = [fn.name for fn in report.functions if fn.match_percent <= 0.0]
    if not report.functions or untouched:
        sample = ", ".join(untouched[:8]) or "no reported functions"
        raise ValueError(
            "TU completion requires every function to have a nonzero first match; "
            f"remaining: {sample}"
        )


def _has_mapped_imperfect_function(
    unit_name: str,
    source_path: Path,
    source: str,
    functions: List[Dict[str, Any]],
    targets_by_symbol: Dict[str, List[Target]],
) -> bool:
    imperfect = {
        str(fn.get("name", ""))
        for fn in functions
        if float(fn.get("fuzzy_match_percent", 0.0) or 0.0) < 100.0
    }
    candidates = [
        target
        for symbol in imperfect
        for target in targets_by_symbol.get(symbol, [])
    ]
    for target in candidates:
        if (
            not target.symbol
            or target.symbol not in imperfect
            or not target.unit
            or target.source is None
            or target.source.resolve() != source_path.resolve()
            or not (unit_name == target.unit or unit_name.endswith("/" + target.unit))
        ):
            continue
        try:
            _find_function_region(source, target)
        except ValueError:
            continue
        return True
    return False


def _function_size_comparison(retail_size: int, candidate_size: int) -> tuple[bool, str]:
    delta = candidate_size - retail_size
    if delta > 0:
        return False, (
            f"candidate function size 0x{candidate_size:X} exceeds retail "
            f"0x{retail_size:X} by 0x{delta:X} ({delta} bytes)"
        )
    spare = -delta
    return True, (
        f"candidate function size 0x{candidate_size:X} fits retail 0x{retail_size:X} "
        f"(0x{spare:X} spare)"
    )


def _binary_mismatch_summary(retail: Any, candidate: Any) -> str:
    """Return bounded function-local word and relocation diagnostics for history."""
    retail_words = [
        int.from_bytes(retail.code[offset : offset + 4], "big")
        for offset in range(0, len(retail.code), 4)
    ]
    candidate_words = [
        int.from_bytes(candidate.code[offset : offset + 4], "big")
        for offset in range(0, len(candidate.code), 4)
    ]
    lines = ["binary mismatch summary:"]
    matcher = difflib.SequenceMatcher(
        None, retail_words, candidate_words, autojunk=False
    )
    shown = 0
    total_regions = 0
    for tag, left_begin, left_end, right_begin, right_end in matcher.get_opcodes():
        if tag == "equal":
            continue
        total_regions += 1
        if shown >= 6:
            continue
        left = " ".join(f"{word:08X}" for word in retail_words[left_begin:left_end][:4])
        right = " ".join(
            f"{word:08X}" for word in candidate_words[right_begin:right_end][:4]
        )
        left_more = " …" if left_end - left_begin > 4 else ""
        right_more = " …" if right_end - right_begin > 4 else ""
        lines.append(
            f"- {tag}: retail +0x{left_begin * 4:X} [{left}{left_more}] "
            f"candidate +0x{right_begin * 4:X} [{right}{right_more}]"
        )
        shown += 1
    if total_regions > shown:
        lines.append(f"- {total_regions - shown} additional instruction regions omitted")

    def relocation_key(item: Any) -> tuple[int, int, str, Any]:
        return (item.offset, item.relocation_type, item.symbol, item.addend)

    retail_relocations = {relocation_key(item) for item in retail.relocations}
    candidate_relocations = {relocation_key(item) for item in candidate.relocations}
    relocation_deltas = [
        ("retail-only", item)
        for item in sorted(retail_relocations - candidate_relocations, key=repr)
    ] + [
        ("candidate-only", item)
        for item in sorted(candidate_relocations - retail_relocations, key=repr)
    ]
    for side, (offset, relocation_type, symbol, addend) in relocation_deltas[:8]:
        lines.append(
            f"- relocation {side}: +0x{offset:X} type={relocation_type} "
            f"symbol={symbol} addend={addend}"
        )
    if len(relocation_deltas) > 8:
        lines.append(f"- {len(relocation_deltas) - 8} additional relocation deltas omitted")
    return "\n".join(lines)[:3000]


def _object_function_fingerprints(path: Path) -> Dict[str, str]:
    if not path.is_file():
        return {}
    output: Dict[str, str] = {}
    for function in list_text_functions(path):
        digest = hashlib.sha256()
        digest.update(function.code)
        for relocation in function.relocations:
            digest.update(
                repr(
                    (
                        relocation.offset,
                        relocation.relocation_type,
                        relocation.symbol,
                        relocation.addend,
                    )
                ).encode("utf-8")
            )
        output[function.name] = digest.hexdigest()
    return output


def _symlink_if_present(source: Path, destination: Path) -> None:
    if not source.exists() or destination.exists() or destination.is_symlink():
        return
    destination.parent.mkdir(parents=True, exist_ok=True)
    destination.symlink_to(source, target_is_directory=source.is_dir())


def _replace_with_symlink(source: Path, destination: Path) -> None:
    if not source.exists():
        return
    if destination.is_symlink() or destination.is_file():
        destination.unlink()
    elif destination.is_dir():
        shutil.rmtree(destination)
    destination.parent.mkdir(parents=True, exist_ok=True)
    destination.symlink_to(source, target_is_directory=source.is_dir())


def _hardlink_objects(source: Path, destination: Path) -> None:
    if not source.is_dir():
        return
    for object_path in source.rglob("*.o"):
        target = destination / object_path.relative_to(source)
        target.parent.mkdir(parents=True, exist_ok=True)
        if not target.exists():
            os.link(object_path, target)


def _source_diff(path: Path, before: str, after: str) -> str:
    relative = path.as_posix()
    return "".join(
        difflib.unified_diff(
            before.splitlines(keepends=True),
            after.splitlines(keepends=True),
            fromfile=relative,
            tofile=relative,
        )
    ) or f"{relative}: no changes\n"


def _insert_marker_slot(
    source: str, target: Target, *, before: str = "", after: str = ""
) -> str:
    if bool(before) == bool(after):
        raise ValueError("Specify exactly one of before or after")
    anchor = before or after
    if source.count(anchor) != 1:
        raise ValueError(f"Exact anchor must occur once; found {source.count(anchor)}")
    position = source.index(anchor) if before else source.index(anchor) + len(anchor)
    block = (
        "\n" + _begin_marker(target.id) + "\n"
        f"// LLM-HARNESS-SLOT: {target.function}\n"
        + _end_marker(target.id) + "\n"
    )
    return source[:position] + block + source[position:]


def _signature_open_brace(source: str, opening_paren: int) -> int | None:
    depth = 0
    state = "code"
    index = opening_paren
    while index < len(source):
        char = source[index]
        nxt = source[index + 1] if index + 1 < len(source) else ""
        if state == "line":
            if char == "\n":
                state = "code"
        elif state == "block":
            if char == "*" and nxt == "/":
                state = "code"
                index += 1
        elif state in {"string", "char"}:
            if char == "\\":
                index += 1
            elif (state == "string" and char == '"') or (state == "char" and char == "'"):
                state = "code"
        elif char == "/" and nxt == "/":
            state = "line"
            index += 1
        elif char == "/" and nxt == "*":
            state = "block"
            index += 1
        elif char == '"':
            state = "string"
        elif char == "'":
            state = "char"
        elif char == "(":
            depth += 1
        elif char == ")":
            depth -= 1
        elif depth == 0 and char == "{":
            return index
        elif depth == 0 and char == ";":
            return None
        index += 1
    return None


def _matching_brace(source: str, opening_brace: int) -> int:
    depth = 0
    state = "code"
    index = opening_brace
    while index < len(source):
        char = source[index]
        nxt = source[index + 1] if index + 1 < len(source) else ""
        if state == "line":
            if char == "\n":
                state = "code"
        elif state == "block":
            if char == "*" and nxt == "/":
                state = "code"
                index += 1
        elif state in {"string", "char"}:
            if char == "\\":
                index += 1
            elif (state == "string" and char == '"') or (state == "char" and char == "'"):
                state = "code"
        elif char == "/" and nxt == "/":
            state = "line"
            index += 1
        elif char == "/" and nxt == "*":
            state = "block"
            index += 1
        elif char == '"':
            state = "string"
        elif char == "'":
            state = "char"
        elif char == "{":
            depth += 1
        elif char == "}":
            depth -= 1
            if depth == 0:
                return index
        index += 1
    raise ValueError("Unbalanced braces while locating function")
