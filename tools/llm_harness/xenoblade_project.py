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
from contextlib import AbstractContextManager, nullcontext
from pathlib import Path
from typing import Any, Dict, List, Optional

from tools.coop.lib.attempts import AttemptRecord, append_attempt
from tools.coop.lib.config import load_config
from tools.coop.lib.equivalence_check import (
    EQUIVALENT_MATCH_MIN_PERCENT,
    prove_unit_symbol,
)
from tools.coop.lib.objdiff_report import evaluate_unit_match, meets_required_level, report_unit
from tools.coop.lib.object_size import check_object_size
from tools.coop.lib.project import Project
from tools.coop.lib.targets import (
    ACCEPTED_MATCH_STATUSES,
    Target,
    claim_target,
    equivalence_certificate_error,
    get_target,
    harness_targets,
    load_targets,
    load_targets_document,
    update_target_result,
    write_targets_document,
)
from tools.llm_harness.dossier import (
    PromptConstraints,
    TargetDossier,
    build_data_flow_summary,
    build_target_dossier,
    compact_model_facing_dossier,
    dossier_to_dict,
)
from tools.llm_harness.match_improve import (
    normalize_objdiff_feedback,
    format_objdiff_feedback_text,
)
from tools.llm_harness.match_improve import normalize_objdiff_feedback
from tools.llm_harness.types import (
    Candidate,
    CandidateEvaluation,
    CandidateStatus,
    CompileReport,
    Evaluation,
    SourcePatch,
    StructuralReport,
    ValidationStepResult,
    fingerprint_binary_feedback,
)
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
from tools.llm_harness.asm_listings import (
    assembly_function_block as _assembly_function_block,
    compact_type_snippets,
    format_instruction_listing,
    select_similar_siblings,
    strip_listing_bytecode_comments,
)
from tools.llm_harness.source_regions import (
    SourceRegion,
    TuSlot,
    apply_strip_redundant_externs_to_regions,
    begin_marker as _begin_marker,
    end_marker as _end_marker,
    find_function_region as _find_function_region,
    find_tu_slots as _find_tu_slots,
    matching_brace as _matching_brace,
    replace_function_source as _replace_function_source,
    signature_open_brace as _signature_open_brace,
    tu_begin_marker as _tu_begin_marker,
    tu_end_marker as _tu_end_marker,
)

# Already-compiling registry statuses worth SMT-probing (not accepted yet).
PROBE_COMPILE_STATUSES = {
    "COMPILES",
    "STRUCTURAL",
    "HIGH_MATCH",
    "CODE_MATCH",
}


def _unit_matches(unit: Optional[str], tu: str) -> bool:
    """Check if a target's unit hint matches a given TU name.

    Strips a trailing ``.o`` suffix from *tu* for convenience, since users
    naturally pass the object-file name (e.g. ``monolibdata1.o``).
    """
    if unit is None:
        return False
    if tu.endswith(".o"):
        tu = tu[:-2]
    return unit == tu or unit.endswith("/" + tu) or tu.endswith("/" + unit)


class XenobladeAdapter:
    def __init__(self, root: Path, settings: Dict[str, Any]) -> None:
        self.root = root.resolve()
        coop_path = settings.get("coop_config")
        self.config = load_config(self.root / coop_path if coop_path else None, self.root)
        self.project = Project(self.config)
        self.prompt_dir = self.root / settings.get("prompt_dir", "tools/llm_harness/prompts")
        self.max_source_chars = int(settings.get("max_source_chars", 120000))
        self.context_similar_limit = int(settings.get("context_similar_limit", 4))
        self.tu_context_chars = int(settings.get("tu_context_chars", 1500))
        self.tu_section_byte_limit = int(settings.get("tu_section_byte_limit", 16384))
        self.prompt_budget = {
            "max_chars": 60000,
            "max_decoded_instructions": 400,
            "max_declaration_chars": 12000,
            "max_callers": 3,
            "max_sibling_bodies": 3,
            "include_raw_hex": False,
        }
        self.prompt_budget.update(dict(settings.get("prompt") or {}))
        # Set by Harness before evaluate() so ninja/objdiff/smt wall time aggregates.
        self.timings: Any = None

    def _phase(self, name: str) -> AbstractContextManager[Any]:
        """Record ``name`` into ``self.timings`` when the harness attached a recorder."""
        if self.timings is None:
            return nullcontext()
        measure = getattr(self.timings, "measure", None)
        if callable(measure):
            return measure(name)
        return nullcontext()

    def _phase_timer(self):
        return lambda phase: self._phase(phase)

    def _target(self, target_id: str) -> Target:
        target = self._any_target(target_id)
        if not target.buildable or target.source is None or target.unit is None or not target.symbol:
            raise ValueError(f"Target {target_id!r} must be a buildable function with a symbol")
        return target

    def _any_target(self, target_id: str) -> Target:
        return get_target(load_targets(self.config), target_id)

    def target_ids_for_unit(self, unit_name: str, workflow: str) -> List[str]:
        """Return all eligible target IDs in a translation unit for a given workflow."""
        if workflow not in {"new", "improve", "solve", "tu-complete", "tu-decomp", "probe"}:
            raise ValueError(f"Unsupported workflow: {workflow}")
        raw_targets = load_targets(self.config)
        rows_by_id = {
            target.id: {"id": target.id, **target.extra} for target in raw_targets
        }
        source_cache: Dict[Path, str] = {}
        done = {"FULL_MATCH", "EQUIVALENT_MATCH"}
        result: List[str] = []
        matched_any_unit = False
        for target in raw_targets:
            if not target.buildable or not target.symbol or not target.unit:
                continue
            if not _unit_matches(target.unit, unit_name):
                continue
            matched_any_unit = True
            if target.source is None or not target.source.is_file():
                continue
            if workflow == "new":
                # Greenfield slots only — empty/placeholder NOT_STARTED work.
                if target.status in done or target.status != "NOT_STARTED":
                    continue
            elif workflow in {"improve", "solve"}:
                # Any non-accepted function with a source region.
                if target.status in done:
                    continue
            elif workflow == "probe":
                if target.status == "FULL_MATCH":
                    continue
                if target.status == "EQUIVALENT_MATCH":
                    if equivalence_certificate_error(
                        rows_by_id[target.id], rows_by_id
                    ) is None:
                        continue
                elif target.status not in PROBE_COMPILE_STATUSES:
                    continue
            if workflow != "tu-complete" and target.status in done and workflow != "probe":
                continue
            try:
                source = source_cache.get(target.source)
                if source is None:
                    source = target.source.read_text(encoding="utf-8")
                    source_cache[target.source] = source
                _find_function_region(source, target)
            except (OSError, ValueError):
                continue
            result.append(target.id)
        if not matched_any_unit:
            # Check if the unit exists at all (e.g. a data-only split) to offer a better hint
            try:
                unit = self.project.resolve_unit(unit_name)
                hint = (
                    f"Unit {unit_name!r} resolves to {unit.name} but has no function targets "
                    f"in the registry (it may be a data-only split). "
                    f"Use `tu-complete {unit_name}` for data-level work instead."
                )
            except ValueError:
                hint = (
                    f"No targets found for unit {unit_name!r}. "
                    "Use a unit name from splits.txt (strip .o suffix) or a "
                    "source-path-style name like monolib/src/core/CSchedule."
                )
            raise ValueError(hint)
        return result

    def select_new_targets(
        self,
        number: int,
        history: List[Dict[str, Any]],
        *,
        ignore_called_functions: bool = False,
        certified_funcs: bool = False,
        tu: Optional[str] = None,
    ) -> List[str]:
        """Return placeholder/empty functions from the call-graph-safe catalog frontier."""
        attempted = {
            str(row.get("target_id"))
            for row in history
            if row.get("target_id")
            and (
                row.get("workflow") in {"new", "improve", "solve"}
                or _history_row_symbol_accepted(row)
            )
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
            and (tu is None or _unit_matches(target.unit, tu))
        ]
        if certified_funcs:
            candidates = self._filter_certified_funcs(raw_targets, candidates)
        from .promotion import PlaceholderDetector

        detector = PlaceholderDetector()
        selected: List[str] = []
        source_cache: Dict[Path, str] = {}
        decomp_symbol_cache: Dict[Path, set[str]] = {}
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
                region = _find_function_region(source, target)
                body = source[region.content_start : region.content_end].strip()
                # `new` is for empty/placeholder slots; real bodies belong to improve
                # (and already-matching NOT_STARTED work should not consume a pick).
                if body and not detector.is_placeholder(body):
                    continue
                unit = self.project.resolve_unit(target.unit)
                if unit.target_path is None or not unit.target_path.is_file():
                    continue
                retail = extract_function(unit.target_path, target.symbol)
                # Skip stubs that already byte-match the built decomp object.
                if unit.base_path is not None and unit.base_path.is_file():
                    try:
                        decomp = extract_function(unit.base_path, target.symbol)
                    except ValueError:
                        decomp = None
                    if decomp is not None and decomp.code == retail.code:
                        continue
                    # Unmarked overloads can resolve to the wrong body (e.g. two
                    # cancel() methods). Skip when that body already produces a
                    # different symbol present in the decomp object.
                    if (
                        decomp is None
                        and not region.marked
                        and self._region_owned_by_other_decomp_symbol(
                            target,
                            region,
                            source,
                            raw_targets,
                            unit.base_path,
                            decomp_symbol_cache,
                            source_cache,
                        )
                    ):
                        continue
            except (OSError, ValueError):
                continue
            selected.append(target.id)
            if len(selected) == number:
                return selected
        raise ValueError(
            f"Only {len(selected)} unattempted, ready placeholder functions are available; "
            f"requested {number}"
        )

    @staticmethod
    def _region_owned_by_other_decomp_symbol(
        target: Target,
        region: SourceRegion,
        source: str,
        raw_targets: List[Target],
        decomp_object: Path,
        decomp_symbol_cache: Dict[Path, set[str]],
        source_cache: Dict[Path, str],
    ) -> bool:
        """True when an unmarked region is already claimed by another decomp symbol."""
        symbols = decomp_symbol_cache.get(decomp_object)
        if symbols is None:
            symbols = {fn.name for fn in list_text_functions(decomp_object)}
            decomp_symbol_cache[decomp_object] = symbols
        if target.symbol in symbols:
            return False
        for other in raw_targets:
            if (
                other.id == target.id
                or other.source != target.source
                or not other.symbol
                or other.symbol not in symbols
            ):
                continue
            other_source = source_cache.get(other.source) if other.source else None
            if other_source is None:
                if other.source is None or not other.source.is_file():
                    continue
                other_source = other.source.read_text(encoding="utf-8")
                source_cache[other.source] = other_source
            try:
                other_region = _find_function_region(other_source, other)
            except (OSError, ValueError):
                continue
            if (
                other_region.content_start == region.content_start
                and other_region.content_end == region.content_end
            ):
                return True
        return False

    def record_baseline_accepted(
        self, target_id: str, evaluation: CandidateEvaluation
    ) -> None:
        """Persist an already-accepted baseline into the target registry."""
        status = evaluation.status.value.upper()
        if status not in ACCEPTED_MATCH_STATUSES:
            status = (
                "FULL_MATCH"
                if float(evaluation.match_percent or 0.0) >= 100.0
                else "EQUIVALENT_MATCH"
            )
        update_target_result(
            self.config,
            target_id,
            status=status,
            instruction_match=float(evaluation.match_percent or 0.0),
            equivalence_status=evaluation.equivalence_status,
        )

    def accepted_target_ids(self) -> List[str]:
        """Return target ids already at FULL_MATCH / EQUIVALENT_MATCH in the registry."""
        return [
            target.id
            for target in load_targets(self.config)
            if target.status in ACCEPTED_MATCH_STATUSES
        ]

    def ensure_auto_promote_claim(self, target_id: str, owner: str) -> None:
        """Claim target for auto-promote when unclaimed; error if owned by someone else."""
        target = self._any_target(target_id)
        current = _claim_owner(target.extra.get("claim"))
        if current is None:
            paths: List[str] = []
            if target.source is not None:
                try:
                    paths = [str(target.source.relative_to(self.root))]
                except ValueError:
                    paths = [str(target.source)]
            claim_target(
                self.config,
                target_id,
                owner=owner,
                allowed_paths=paths,
                note="llm-harness auto-promote",
            )
            return
        if current != owner:
            raise ValueError(
                f"target {target_id} claimed by {current!r}, not {owner!r}"
            )

    def record_auto_promotion(self, target_id: str, evaluation: Dict[str, Any]) -> None:
        """Update targets.json after a successful auto-promote write."""
        status = str(evaluation.get("status") or "").upper()
        if status not in ACCEPTED_MATCH_STATUSES:
            match = float(evaluation.get("match_percent") or 0.0)
            status = "FULL_MATCH" if match >= 100.0 else "EQUIVALENT_MATCH"
        update_target_result(
            self.config,
            target_id,
            status=status,
            instruction_match=float(evaluation.get("match_percent") or 0.0),
            equivalence_status=evaluation.get("equivalence"),
        )

    def run_auto_promote_cycle(
        self, target_id: str, evaluation: Dict[str, Any]
    ) -> None:
        """Best-effort coop cycle after auto-promote; does not roll back the write."""
        status = str(evaluation.get("status") or "FULL_MATCH")
        try:
            subprocess.run(
                [
                    sys.executable,
                    "tools/coop/run.py",
                    "cycle",
                    target_id,
                    "--hypothesis",
                    f"llm-harness auto-promote {status}",
                    "--next-change",
                    "None",
                ],
                cwd=self.root,
                text=True,
                capture_output=True,
                timeout=600,
                check=False,
            )
        except (OSError, subprocess.TimeoutExpired):
            pass

    def select_targets(
        self,
        workflow: str,
        number: int,
        *,
        randomize: bool = False,
        certified_funcs: bool = False,
        tu: Optional[str] = None,
        selection: Optional[str] = None,
        min_fuzzy: Optional[float] = None,
    ) -> List[str]:
        if workflow in {"improve", "solve"}:
            # solve defaults to the ready call-graph frontier; improve keeps all non-accepted.
            if selection is None:
                selection = "ready" if workflow == "solve" else "pending"
            candidates = self._non_accepted_candidates(
                certified_funcs=certified_funcs, tu=tu, selection=selection
            )
        elif workflow == "tu-complete":
            candidates = self._tu_completion_candidates()
        elif workflow == "probe":
            if selection is None:
                selection = "ready"
            if min_fuzzy is None:
                min_fuzzy = EQUIVALENT_MATCH_MIN_PERCENT
            candidates = self.select_probe_targets(
                certified_funcs=certified_funcs,
                tu=tu,
                selection=selection,
                min_fuzzy=min_fuzzy,
            )
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

    def select_probe_targets(
        self,
        *,
        certified_funcs: bool = True,
        tu: Optional[str] = None,
        selection: str = "ready",
        min_fuzzy: float = EQUIVALENT_MATCH_MIN_PERCENT,
    ) -> List[str]:
        """Compiling targets that lack FULL_MATCH / current EQUIVALENT_MATCH cert.

        Requires a built decomp object that still exports the symbol. Prefers
        known fuzzy scores in ``[min_fuzzy, 100)`` (SMT can still promote), then
        unknown scores, then the call-graph frontier order.
        """
        raw_targets = load_targets(self.config)
        rows_by_id = {
            target.id: {"id": target.id, **target.extra} for target in raw_targets
        }
        frontier = harness_targets(
            raw_targets, selection=selection, include_catalog=True
        )
        # Stale EQUIVALENT_MATCH is excluded from harness_targets; append those.
        stale_equiv: List[Target] = []
        for target in raw_targets:
            if target.status != "EQUIVALENT_MATCH" or not target.buildable:
                continue
            if equivalence_certificate_error(rows_by_id[target.id], rows_by_id) is None:
                continue
            if tu is not None and not _unit_matches(target.unit, tu):
                continue
            stale_equiv.append(target)

        units = self.project.load_objdiff_units()
        unit_by_hint = {
            hint: unit
            for unit in units
            for hint in {unit.name, unit.name.removeprefix("main/")}
        }
        decomp_symbols: Dict[Path, set[str]] = {}
        retail_sizes: Dict[str, int] = {}
        source_cache: Dict[Path, str] = {}
        scored: List[tuple[float, Target]] = []

        def consider(target: Target) -> None:
            if (
                not target.buildable
                or not target.symbol
                or target.source is None
                or not target.source.is_file()
                or not target.unit
            ):
                return
            if tu is not None and not _unit_matches(target.unit, tu):
                return
            if target.status == "FULL_MATCH":
                return
            if target.status == "EQUIVALENT_MATCH":
                if equivalence_certificate_error(rows_by_id[target.id], rows_by_id) is None:
                    return
            elif target.status not in PROBE_COMPILE_STATUSES:
                return
            source = source_cache.get(target.source)
            if source is None:
                source = target.source.read_text(encoding="utf-8")
                source_cache[target.source] = source
            try:
                _find_function_region(source, target)
            except (OSError, ValueError):
                return
            unit = unit_by_hint.get(target.unit)
            if unit is None or unit.base_path is None or not unit.base_path.is_file():
                return
            if unit.target_path is None or not unit.target_path.is_file():
                return
            symbols = decomp_symbols.get(unit.base_path)
            if symbols is None:
                try:
                    symbols = {fn.name for fn in list_text_functions(unit.base_path)}
                except (OSError, ValueError):
                    return
                decomp_symbols[unit.base_path] = symbols
            if target.symbol not in symbols:
                return
            known = target.extra.get("instruction_match")
            fuzzy = float(known) if known is not None else -1.0
            if fuzzy >= 0.0 and fuzzy < float(min_fuzzy):
                return
            if fuzzy >= 100.0 and target.status != "EQUIVALENT_MATCH":
                # Byte-identical but not yet FULL_MATCH — still worth probing/certifying.
                pass
            try:
                retail = extract_function(unit.target_path, target.symbol)
                retail_sizes[target.id] = int(getattr(retail, "size", 0) or 0)
            except (OSError, ValueError):
                retail_sizes[target.id] = 0
            scored.append((fuzzy, target))

        for target in frontier:
            consider(target)
        seen = {target.id for _, target in scored}
        for target in stale_equiv:
            if target.id not in seen:
                consider(target)

        if certified_funcs:
            filtered = self._filter_certified_funcs(
                raw_targets, [target for _, target in scored]
            )
            allowed = {target.id for target in filtered}
            scored = [(fuzzy, target) for fuzzy, target in scored if target.id in allowed]

        tier_order = {"P0": 0, "P1": 1, "P2": 2, "P3": 3, "P9": 9}

        def sort_key(item: tuple[float, Target]) -> tuple:
            fuzzy, target = item
            kind = self._frontier_kind(target)
            leaf_rank = 0 if kind == "leaf" else 1
            # Prefer known in-range fuzzy, then unknown (-1), then everything else.
            if fuzzy >= float(min_fuzzy):
                fuzzy_rank = 0
                fuzzy_sort = -fuzzy  # higher first
            elif fuzzy < 0.0:
                fuzzy_rank = 1
                fuzzy_sort = 0.0
            else:
                fuzzy_rank = 2
                fuzzy_sort = fuzzy
            size = retail_sizes.get(target.id) or 10**9
            return (
                tier_order.get(target.tier, 99),
                fuzzy_rank,
                fuzzy_sort,
                leaf_rank,
                size,
                target.id,
            )

        scored.sort(key=sort_key)
        return [target.id for _, target in scored]

    def probe_target(
        self,
        target_id: str,
        *,
        write: bool = False,
        linked: bool = False,
        rebuild: bool = True,
    ) -> Dict[str, Any]:
        """Build + objdiff + ppc_equivalence for one compiling non-accepted target.

        When ``write`` is true and the probe reaches ``FULL_MATCH`` /
        ``EQUIVALENT_MATCH`` with a certificate (for equivalent), update
        ``targets.json`` and append an attempt log row. Does not call an LLM.
        """
        target = self._any_target(target_id)
        if target.unit is None or target.symbol is None:
            raise ValueError(f"Target {target_id!r} needs unit and symbol")
        unit = self.project.resolve_unit(target.unit)
        if unit.base_path is None:
            raise ValueError(f"Unit {unit.name!r} has no decomp object path")

        build_error = ""
        if rebuild:
            build_error = self._build_object(unit.base_path)
        if build_error:
            return {
                "target_id": target_id,
                "status": "COMPILE_ERROR",
                "match_percent": 0.0,
                "accepted": False,
                "equivalence": None,
                "detail": build_error,
                "written": False,
            }
        if not unit.base_path.is_file():
            return {
                "target_id": target_id,
                "status": "COMPILE_ERROR",
                "match_percent": 0.0,
                "accepted": False,
                "equivalence": None,
                "detail": f"missing decomp object {unit.base_path}",
                "written": False,
            }

        try:
            evaluation = evaluate_unit_match(
                self.project,
                unit,
                target.symbol,
                target_id=target.id,
                certify_full_match=True,
                linked=linked,
                phase_timer=self._phase_timer(),
            )
        except (subprocess.CalledProcessError, FileNotFoundError, ValueError) as exc:
            return {
                "target_id": target_id,
                "status": "EVALUATION_ERROR",
                "match_percent": 0.0,
                "accepted": False,
                "equivalence": None,
                "detail": str(exc),
                "written": False,
            }

        match_percent = (
            evaluation.fn_match.match_percent
            if evaluation.fn_match
            else evaluation.unit_report.fuzzy_match_percent
        )
        equiv = evaluation.equivalence.value if evaluation.equivalence else None
        certificate = evaluation.equivalence_certificate
        symbol_accepted = evaluation.status in ACCEPTED_MATCH_STATUSES and (
            evaluation.status == "FULL_MATCH"
            or (
                evaluation.status == "EQUIVALENT_MATCH"
                and evaluation.equivalence == ProofStatus.EQUIVALENT
                and certificate is not None
            )
        )
        result: Dict[str, Any] = {
            "target_id": target_id,
            "status": evaluation.status,
            "match_percent": match_percent,
            "accepted": symbol_accepted,
            "equivalence": equiv,
            "detail": evaluation.equivalence_detail or "",
            "equivalence_confidence": evaluation.equivalence_confidence,
            "equivalence_policy": evaluation.equivalence_policy,
            "certificate_sha256": (
                certificate.get("certificate_sha256") if certificate else None
            ),
            "written": False,
        }

        if not write or not symbol_accepted:
            return result

        # Validate certificate chain before persisting EQUIVALENT_MATCH.
        if evaluation.status == "EQUIVALENT_MATCH":
            document = load_targets_document(self.config)
            rows_by_id = {
                str(row["id"]): row
                for row in document.get("targets", [])
                if isinstance(row, dict) and isinstance(row.get("id"), str)
            }
            trial = dict(rows_by_id.get(target.id, {"id": target.id}))
            trial["status"] = "EQUIVALENT_MATCH"
            trial["equivalence_certificate"] = certificate
            rows_by_id[target.id] = trial
            cert_error = equivalence_certificate_error(trial, rows_by_id)
            if cert_error:
                result["accepted"] = False
                result["detail"] = f"certificate rejected: {cert_error}"
                return result

        update_target_result(
            self.config,
            target.id,
            status=evaluation.status,
            instruction_match=match_percent,
            equivalence_status=equiv,
            equivalence_certificate=certificate if evaluation.status == "EQUIVALENT_MATCH" else None,
            certificate_checked=evaluation.status == "EQUIVALENT_MATCH",
            equivalence_confidence=evaluation.equivalence_confidence,
            equivalence_policy=evaluation.equivalence_policy,
        )
        append_attempt(
            self.config.resolve(self.config.attempt_log),
            AttemptRecord(
                target_id=target.id,
                function=target.function,
                region=self.config.region,
                unit=target.unit or "",
                symbol=target.symbol,
                status=evaluation.status,
                instruction_match=match_percent,
                relocation_match=None,
                code_match_percent=evaluation.unit_report.code_match_percent,
                data_match_percent=evaluation.unit_report.data_match_percent,
                hypothesis="llm-harness probe: ppc_equivalence on compiling target",
                next_change="",
                equivalence_status=equiv,
                equivalence_detail=evaluation.equivalence_detail or "",
                equivalence_confidence=evaluation.equivalence_confidence,
                equivalence_policy=evaluation.equivalence_policy,
            ),
        )
        result["written"] = True
        return result

    def describe_frontier(self, target_ids: List[str]) -> List[Dict[str, str]]:
        """Return leaf/ready annotations for dry-run frontier printing."""
        by_id = {target.id: target for target in load_targets(self.config)}
        rows: List[Dict[str, str]] = []
        for target_id in target_ids:
            target = by_id.get(target_id)
            kind = "unknown"
            if target is not None:
                kind = self._frontier_kind(target)
            rows.append({"id": target_id, "kind": kind})
        return rows

    @staticmethod
    def _frontier_kind(target: Target) -> str:
        called = target.extra.get("called_functions", []) or []
        unresolved = target.extra.get("unresolved_called_functions", []) or []
        indirect = bool(target.extra.get("has_indirect_calls", False))
        complete = target.extra.get("callgraph_status") == "complete"
        if complete and not called and not unresolved and not indirect:
            return "leaf"
        if complete and called and not unresolved and not indirect:
            return "callees-accepted"
        return "pending"

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

    def _non_accepted_candidates(
        self,
        *,
        certified_funcs: bool = False,
        tu: Optional[str] = None,
        selection: str = "pending",
    ) -> List[str]:
        """Mismatching buildable targets from a call-graph frontier selection.

        Starts from ``harness_targets(..., include_catalog=True)`` (tier-sorted),
        then applies the same retail/decomp mismatch checks as before. Within a
        tier, leaves are preferred over callees-accepted, then smaller retail
        size when available.
        """
        source_cache: Dict[Path, str] = {}
        units = self.project.load_objdiff_units()
        unit_by_hint = {
            hint: unit
            for unit in units
            for hint in {unit.name, unit.name.removeprefix("main/")}
        }
        retail_symbols: Dict[Path, set[str]] = {}
        retail_sizes: Dict[str, int] = {}
        raw_targets = load_targets(self.config)
        frontier = harness_targets(
            raw_targets, selection=selection, include_catalog=True
        )
        candidates: List[Target] = []
        for target in frontier:
            if (
                not target.buildable
                or not target.symbol
                or target.source is None
                or not target.source.is_file()
                or not target.unit
            ):
                continue
            if tu is not None and not _unit_matches(target.unit, tu):
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
                if unit.base_path is None or not unit.base_path.is_file():
                    # Need a built decomp object to know the function still mismatches.
                    continue
                symbols = retail_symbols.get(unit.target_path)
                if symbols is None:
                    symbols = {fn.name for fn in list_text_functions(unit.target_path)}
                    retail_symbols[unit.target_path] = symbols
                if target.symbol not in symbols:
                    continue
                retail = extract_function(unit.target_path, target.symbol)
                retail_sizes[target.id] = int(getattr(retail, "size", 0) or 0)
                try:
                    decomp = extract_function(unit.base_path, target.symbol)
                except ValueError:
                    # Symbol missing from decomp — still worth improving/reconstructing.
                    decomp = None
                if decomp is not None and decomp.code == retail.code:
                    continue
            except (OSError, ValueError):
                continue
            candidates.append(target)
        if certified_funcs:
            candidates = self._filter_certified_funcs(raw_targets, candidates)
        tier_order = {"P0": 0, "P1": 1, "P2": 2, "P3": 3, "P9": 9}

        def sort_key(target: Target) -> tuple:
            kind = self._frontier_kind(target)
            leaf_rank = 0 if kind == "leaf" else 1
            size = retail_sizes.get(target.id)
            size_rank = size if size is not None and size > 0 else 10**9
            return (
                tier_order.get(target.tier, 99),
                leaf_rank,
                size_rank,
                target.id,
            )

        candidates.sort(key=sort_key)
        return [target.id for target in candidates]

    def _improve_candidates(
        self,
        *,
        certified_funcs: bool = False,
        tu: Optional[str] = None,
        selection: str = "pending",
    ) -> List[str]:
        """Alias kept for callers/tests; prefer `_non_accepted_candidates`."""
        return self._non_accepted_candidates(
            certified_funcs=certified_funcs, tu=tu, selection=selection
        )

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
        if workflow == "tu-decomp":
            return self._build_tu_decomp_prompt(
                target_id,
                history,
                options,
            )
        return self._build_function_prompt(workflow, target_id, history, options)

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
                return strip_listing_bytecode_comments(block)
        return ""

    def get_retail_asm(self, target_id: str) -> str:
        """Retail assembly excerpt for compile-repair ABI/signature checks."""
        return self._retail_assembly(self._target(target_id))

    def suggest_max_output_tokens(self, target_id: str) -> int:
        """Heuristic generation budget from retail function size.

        Stub/blr functions should not be allowed to emit multi-thousand-token
        essays. Larger bodies need room for `source`, but metadata must stay tiny.
        """
        target = self._target(target_id)
        assert target.unit is not None and target.symbol is not None
        unit = self.project.resolve_unit(target.unit)
        if unit.target_path is None or not unit.target_path.is_file():
            return 2048
        function = extract_function(unit.target_path, target.symbol)
        # ~8 tokens/byte covers dense C++ plus JSON escaping; +512 for schema fields.
        return max(768, min(8192, int(function.size) * 8 + 512))

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
        self, workflow: str, target_id: str, history: List[Dict[str, Any]], options: Optional[Dict[str, Any]] = None
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

        # Phase 2: prefer exact best-candidate repair context from the harness.
        repair_context = (options or {}).get("repair_context") if options else None
        if isinstance(repair_context, dict) and repair_context.get("source"):
            current_function = str(repair_context["source"]).strip() or current_function

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
        # Find header file in the same directory
        header_text = ""
        if target.source is not None:
            header_candidates = [
                target.source.with_suffix(".hpp"),
                target.source.with_suffix(".h"),
                target.source.with_suffix(".hxx"),
            ]
            for hp in header_candidates:
                if hp.is_file():
                    try:
                        header_text = hp.read_text(encoding="utf-8")
                    except OSError:
                        pass
                    break
        # Prompt budgets come from harness `prompt` (passed via create_adapter).
        constraints = PromptConstraints.from_prompt_config(self.prompt_budget)
        dossier = build_target_dossier(
            target_id=target.id,
            mangled=target.symbol,
            demangled=target.function,
            source_file=str(target.source.relative_to(self.root)),
            object_file=str(
                (unit.base_path or unit.target_path or "?").relative_to(self.root)
            ),
            unit=target.unit,
            address=function.base,
            retail_size=function.size,
            function_bytes=function,
            source_text=source,
            header_text=header_text,
            callee_symbols=list(target.extra.get("called_functions", [])),
            constraints=constraints,
        )
        # Convert to plain dict and reshape for the model prompt: retail ASM
        # listing instead of the verbose decoded-instruction JSON array.
        dossier_dict = dossier_to_dict(dossier)
        dossier_dict["schema_version"] = 3
        dossier_dict["repository"] = "xenoblade-wii-us"
        dossier_dict["workflow"] = workflow
        dossier_dict["prior_harness_attempts"] = prior_attempts

        retail_asm = self._retail_assembly(target)
        if not retail_asm and dossier.retail and dossier.retail.instructions:
            retail_asm = format_instruction_listing(
                dossier.retail.instructions,
                max_instructions=constraints.max_decoded_instructions,
            )
        dossier_dict["retail_asm"] = retail_asm
        dossier_dict["retail"] = {
            "base": f"0x{function.base:08X}",
            "size": function.size,
            "truncated": bool(
                constraints.max_decoded_instructions > 0
                and (function.size // 4) > constraints.max_decoded_instructions
            ),
        }

        reloc_symbols = sorted(
            {
                str(rel.symbol)
                for rel in (function.relocations or [])
                if getattr(rel, "symbol", None)
            }
        )
        type_snippets = compact_type_snippets(
            header_text=header_text,
            source_text=source,
            demangled=target.function,
            reloc_symbols=reloc_symbols,
            max_chars=min(int(self.prompt_budget.get("max_declaration_chars", 12000)), 3000),
        )
        dossier_dict["types"] = {
            "snippets": type_snippets,
            "total_chars": sum(len(s) + 1 for s in type_snippets),
        }

        # Drop empty / tooling-only fields from the model-facing dossier.
        for noise_key in (
            "accepted_examples",
            "prior_attempt_summary",
            "constraints",
            "warnings",
            "symbols",
            "calls",
            "accepted_functions_in_same_unit",
            "owner_declaration",
            "callee_declarations",
        ):
            dossier_dict.pop(noise_key, None)
        if not dossier_dict.get("prior_harness_attempts"):
            dossier_dict.pop("prior_harness_attempts", None)

        # Structured repair context for improve/solve repair turns.
        # Omit candidate_source when it matches the editable footer — avoid
        # shipping the same function body twice.
        if isinstance(repair_context, dict) and repair_context:
            evaluation = repair_context.get("evaluation") or {}
            repair_payload: Dict[str, Any] = {
                "candidate_status": evaluation.get("status", ""),
                "candidate_match_percent": evaluation.get("match_percent", 0.0),
                "candidate_hypothesis": repair_context.get("hypothesis", ""),
                "binary_feedback": repair_context.get("binary_feedback"),
                "rejected_fingerprints": repair_context.get("rejected_fingerprints") or [],
            }
            rc_source = str(repair_context.get("source") or "").strip()
            if rc_source and rc_source != current_function:
                repair_payload["candidate_source"] = rc_source
            dossier_dict["repair_context"] = repair_payload
            baseline_source = source[region.content_start : region.content_end].strip()
            if baseline_source != current_function:
                dossier_dict["baseline_source"] = baseline_source

        common = (self.prompt_dir / "common.md").read_text(encoding="utf-8")
        prompt_workflow = "improve" if workflow == "solve" else workflow
        workflow_file = self.prompt_dir / f"{prompt_workflow}.md"
        if not workflow_file.is_file():
            workflow_file = self.prompt_dir / "new.md"
        workflow_prompt = workflow_file.read_text(encoding="utf-8")

        max_sibling_bodies = int(self.prompt_budget.get("max_sibling_bodies", 3))
        sibling_candidates = []
        if same_unit and max_sibling_bodies > 0:
            for sibling in same_unit:
                sibling_target = self._any_target(sibling["id"])
                if sibling_target.source and sibling_target.source.is_file():
                    try:
                        sib_source = sibling_target.source.read_text(encoding="utf-8")
                        sib_region = _find_function_region(sib_source, sibling_target)
                        sib_body = sib_source[
                            sib_region.content_start : sib_region.content_end
                        ].strip()
                        sibling_candidates.append({
                            "id": sibling["id"],
                            "function": sibling["function"],
                            "status": sibling["status"],
                            "body": sib_body,
                        })
                    except (OSError, ValueError):
                        pass
        sibling_bodies = select_similar_siblings(
            sibling_candidates,
            demangled=target.function,
            max_bodies=max_sibling_bodies,
            # Style exemplars only — large unrelated methods are noise.
            max_chars_each=min(constraints.max_accepted_chars_each, 400),
        )
        if sibling_bodies:
            dossier_dict["sibling_bodies"] = sibling_bodies

        max_callers = constraints.max_callers
        callers = target.extra.get("caller_functions", [])
        caller_excerpts = []
        if callers and max_callers > 0:
            for caller_id in callers[:max_callers]:
                caller_target = self._any_target(caller_id)
                if caller_target.source and caller_target.source.is_file():
                    try:
                        caller_source = caller_target.source.read_text(encoding="utf-8")
                        caller_region = _find_function_region(caller_source, caller_target)
                        caller_body = caller_source[
                            caller_region.content_start : caller_region.content_end
                        ].strip()
                        caller_excerpts.append({
                            "id": caller_id,
                            "excerpt": caller_body[: constraints.max_caller_chars_each],
                        })
                    except (OSError, ValueError):
                        pass
        if caller_excerpts:
            dossier_dict["caller_excerpts"] = caller_excerpts

        dossier_dict = compact_model_facing_dossier(dossier_dict)

        max_chars = int(self.prompt_budget.get("max_chars", 60000))

        def _render() -> str:
            return (
                common.replace("{{WORKFLOW_PROMPT}}", workflow_prompt)
                .replace("{{DOSSIER_JSON}}", json.dumps(dossier_dict, separators=(",", ":")))
                .replace("{{CURRENT_FUNCTION}}", current_function)
            )

        prompt = _render()

        # Truncate bulky dossier fields until under budget (keep identity + feedback).
        if len(prompt) > max_chars:
            asm = str(dossier_dict.get("retail_asm") or "")
            if len(asm) > 8000:
                dossier_dict["retail_asm"] = asm[:8000] + "\n... truncated"
                if isinstance(dossier_dict.get("retail"), dict):
                    dossier_dict["retail"]["truncated"] = True
            dossier_dict.pop("sibling_bodies", None)
            dossier_dict.pop("caller_excerpts", None)
            prompt = _render()

        if len(prompt) > max_chars:
            types = dossier_dict.get("types")
            if isinstance(types, dict):
                snippets = types.get("snippets")
                if isinstance(snippets, list) and len(snippets) > 8:
                    types["snippets"] = snippets[:8]
            prompt = _render()

        return prompt

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
        shared = ""
        shared_path = self.prompt_dir / "tu-shared.md"
        if shared_path.is_file():
            shared = shared_path.read_text(encoding="utf-8")
        if full_context:
            template = (self.prompt_dir / "tu-complete-full.md").read_text(encoding="utf-8")
            dossier = self._tu_dossier(unit, report, recent)
            _append_tu_volatile_tail(dossier, recent)
            return (
                template.replace("{{TU_SHARED}}", shared)
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
        _append_tu_volatile_tail(dossier, recent)
        template = (self.prompt_dir / "tu-complete.md").read_text(encoding="utf-8")
        return (
            template.replace("{{TU_SHARED}}", shared)
            .replace("{{DOSSIER_JSON}}", json.dumps(dossier, separators=(",", ":")))
        )

    def _build_tu_decomp_prompt(
        self, unit_hint: str, history: List[Dict[str, Any]], options: Optional[Dict[str, Any]] = None
    ) -> str:
        """Build a full-TU decompilation prompt using the tu_context gatherer.

        This is the 'tu-decomp' workflow: the model gets ALL functions in a TU
        with retail ASM, current stubs, header, matched siblings, callee summaries,
        and struct context. The model produces a two-phase output:
          Phase 1: improved header (.hpp)
          Phase 2: complete .cpp with all function bodies filled.
        """
        unit = self.project.resolve_unit(unit_hint)
        if unit.source_path is None:
            raise ValueError(f"Unit {unit.name!r} has no source file")

        # Gather TU context
        from tools.llm_harness.tu_context import gather_tu_context, format_tu_prompt, render_tu_prompt

        prompt_budget = dict(self.prompt_budget)
        if options:
            prompt_budget.update(dict(options.get("prompt", {})))
        context = gather_tu_context(
            root=self.root,
            unit_hint=unit_hint,
            config_path=None,
            prompt_budget=prompt_budget,
        )

        # Format the plaintext dossier and render the final prompt.
        dossier_text = format_tu_prompt(context)
        prompt = render_tu_prompt(dossier_text, self.prompt_dir)

        return prompt

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
        # Leave the volatile tail (`prior_harness_attempts`)
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
        if workflow == "tu-decomp":
            return self._evaluate_tu_decomp(target_id, candidate)
        return self._evaluate_function(target_id, candidate)

    def _evaluate_function(self, target_id: str, candidate: Candidate) -> Evaluation:
        target = self._target(target_id)
        assert target.source is not None and target.unit is not None
        original = target.source.read_text(encoding="utf-8")
        region = _find_function_region(original, target)
        self._validate_source(original[region.content_start : region.content_end], candidate.source)
        candidate_file = _replace_function_source(
            original,
            region,
            candidate.source,
            target_function=target.function or "",
            target_symbol=target.symbol or "",
            source_path=target.source,
            target_id=target.id,
        )
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
                    phase_timer=self._phase_timer(),
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
                    status="SYMBOL_EXTRACTION_ERROR",
                    match_percent=match_percent,
                    accepted=False,
                    detail=_symbol_extraction_detail(
                        exc,
                        candidate_source=candidate.source,
                        target_id=target.id,
                        target_symbol=target.symbol or "",
                        object_path=unit.base_path,
                    ),
                )

            structural_report = None
            binary_feedback = None
            mismatch_fingerprint = ""
            try:
                structural_report = compare_structural(
                    retail_function, candidate_function,
                )
                # Phase 2: persist structured binary feedback
                binary_feedback = normalize_objdiff_feedback(
                    retail_function,
                    candidate_function,
                    max_window=4,
                )
                mismatch_fingerprint = fingerprint_binary_feedback(binary_feedback)
            except Exception as exc:
                pass

            size_ok, size_detail = _function_size_comparison(
                retail_function.size, candidate_function.size
            )
            mismatch_detail = (
                format_objdiff_feedback_text(binary_feedback)
                if match_percent < 100.0 and binary_feedback is not None
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
            # Phase 4: Fix acceptance semantics
            # - FULL_MATCH: 100% static match = accepted
            # - EQUIVALENT_MATCH: fuzzy >= 50% + proved equivalence = accepted  
            # - CODE_MATCH: NOT accepted (unresolved callees)
            # - Local function size delta does NOT reject symbol match; final object fit checked at promotion
            symbol_accepted = (
                evaluation.status == "FULL_MATCH" or
                (evaluation.status == "EQUIVALENT_MATCH" and evaluation.equivalence and 
                 evaluation.equivalence != ProofStatus.INCONCLUSIVE_UNVALIDATED_CALLEE.value)
            )
            # Project readiness requires symbol acceptance AND containing object split fit (checked at TU level)
            project_ready = None  # Determined at TU completion, not function level
            # size_ok is recorded but does not gate symbol acceptance
            accepted = symbol_accepted
            promising = (
                not symbol_accepted
                and evaluation.status not in {"COMPILE_ERROR", "EVALUATION_ERROR"}
                and match_percent > 0.0
            )
            blocked_reason = "unvalidated_callee" if callee_pending else None
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
                symbol_accepted=symbol_accepted,
                project_ready=project_ready,
                promising=promising,
                blocked_reason=blocked_reason,
                metrics={
                    "retail_function_size": retail_function.size,
                    "candidate_function_size": candidate_function.size,
                    "function_size_delta": candidate_function.size - retail_function.size,
                    "binary_feedback": binary_feedback,
                    "mismatch_fingerprint": mismatch_fingerprint,
                    "symbol_accepted": symbol_accepted,
                    "project_ready": project_ready,
                    "promising": promising,
                    "blocked_reason": blocked_reason,
                    "structural": (
                        {
                            "total_score": structural_report.total_score,
                            "calls": {
                                "score": structural_report.calls.score,
                                "matched": structural_report.calls.matched,
                                "expected": structural_report.calls.expected,
                                "details": structural_report.calls.details,
                            },
                            "relocations": {
                                "score": structural_report.relocations.score,
                                "matched": structural_report.relocations.matched,
                                "expected": structural_report.relocations.expected,
                                "details": structural_report.relocations.details,
                            },
                            "memory_accesses": {
                                "score": structural_report.memory_accesses.score,
                                "matched": structural_report.memory_accesses.matched,
                                "expected": structural_report.memory_accesses.expected,
                                "details": structural_report.memory_accesses.details,
                            },
                            "cfg": {
                                "score": structural_report.cfg.score,
                                "matched": structural_report.cfg.matched,
                                "expected": structural_report.cfg.expected,
                                "details": structural_report.cfg.details,
                            },
                            "constants": {
                                "score": structural_report.constants.score,
                                "matched": structural_report.constants.matched,
                                "expected": structural_report.constants.expected,
                                "details": structural_report.constants.details,
                            },
                            "returns": {
                                "score": structural_report.returns.score,
                                "matched": structural_report.returns.matched,
                                "expected": structural_report.returns.expected,
                                "details": structural_report.returns.details,
                            },
                            "instruction_classes": {
                                "score": structural_report.instruction_classes.score,
                                "matched": structural_report.instruction_classes.matched,
                                "expected": structural_report.instruction_classes.expected,
                                "details": structural_report.instruction_classes.details,
                            },
                            "unexpected_effects": structural_report.unexpected_effects,
                            "missing_effects": structural_report.missing_effects,
                        }
                        if structural_report
                        else None
                    ),
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
            evaluation = evaluate_unit_match(
                self.project, unit, None, phase_timer=self._phase_timer()
            )
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

    def _evaluate_tu_decomp(self, unit_hint: str, candidate: Candidate) -> Evaluation:
        """Evaluate a TU-decomp candidate (two-phase header + cpp).

        Prefers ``candidate.phase1_header`` / ``phase2_cpp`` fields. Falls back
        to legacy JSON stuffed into ``candidate.source``. Builds per-function
        patches via ``tu_eval``, then delegates to ``_evaluate_tu``.
        """
        from tools.llm_harness.tu_eval import (
            TuDecompResponse,
            build_tu_candidate,
            extract_tu_slots_and_targets,
            parse_tu_response,
        )

        # If candidate already has patches (pre-parsed by harness), use directly.
        if candidate.patches:
            return self._evaluate_tu(unit_hint, candidate)

        unit = self.project.resolve_unit(unit_hint)
        if unit.source_path is None:
            raise ValueError(f"Unit {unit.name!r} has no source file")
        original_source = unit.source_path.read_text(encoding="utf-8")

        slots, target_map = extract_tu_slots_and_targets(self, unit_hint)
        if not slots:
            raise ValueError(f"No TU slots found for unit {unit_hint!r}")

        tu_response: Optional[TuDecompResponse] = None
        if candidate.phase2_cpp.strip():
            tu_response = TuDecompResponse(
                phase1_header=candidate.phase1_header,
                phase2_cpp=candidate.phase2_cpp,
                hypothesis=candidate.hypothesis,
                notes=list(candidate.notes),
                next_change=candidate.next_change,
            )
        else:
            text = candidate.source.strip()
            if text.startswith("{\"phase1_header\"") or text.startswith("{\"phase2_cpp"):
                tu_response = parse_tu_response(text)

        if tu_response is not None:
            parsed_candidate = build_tu_candidate(
                tu_response,
                original_source,
                slots,
                target_map,
            )
            evaluation = self._evaluate_tu(unit_hint, parsed_candidate)
            if tu_response.phase1_header.strip():
                evaluation.metrics["phase1_header_chars"] = len(tu_response.phase1_header)
            return evaluation

        # Fallback: treat candidate.source as a complete .cpp replacement
        return self._evaluate_tu(unit_hint, candidate)

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
            probe = None
            with self._phase("smt"):
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
        if workflow not in {"tu-complete", "tu-decomp"}:
            metrics = evaluation.get("metrics") or {}
            symbol_accepted = bool(
                evaluation.get("symbol_accepted", metrics.get("symbol_accepted", False))
            )
            match_pct = float(evaluation.get("match_percent") or 0.0)
            equiv = evaluation.get("equivalence") == "EQUIVALENT"
            size_delta = metrics.get("function_size_delta", 0) or 0
            # Ranking: symbol_accepted > match_pct > equivalence > size delta
            # Do not use weighted structural totals as the primary rank key.
            return (
                1 if symbol_accepted else 0,
                match_pct,
                1 if equiv else 0,
                -abs(min(size_delta, 0)),
            )
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
        with self._phase("ninja"):
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

    def strip_accepted_redundant_externs(
        self,
        *,
        dry_run: bool = False,
        tu: str = "",
        target_id: str = "",
    ) -> List[Dict[str, Any]]:
        """Strip duplicate ``extern`` object decls from accepted function slots.

        Only ``FULL_MATCH`` / ``EQUIVALENT_MATCH`` targets are considered. Within
        each file the earliest declaration of a symbol is kept; later accepted
        slots lose their redundant ``extern`` lines. Dry-run reports planned
        edits without writing.
        """
        if target_id and tu:
            raise ValueError("Specify at most one of target_id or tu")

        selected: List[Target] = []
        for target in load_targets(self.config):
            if target.status not in ACCEPTED_MATCH_STATUSES:
                continue
            if not target.buildable or target.source is None or not target.symbol:
                continue
            if not target.source.is_file():
                continue
            if target_id and target.id != target_id:
                continue
            if tu and not _unit_matches(target.unit, tu):
                continue
            selected.append(target)

        if target_id and not selected:
            raise ValueError(
                f"Target {target_id!r} is not an accepted FULL/EQUIVALENT_MATCH "
                "buildable function with source"
            )

        by_path: Dict[Path, List[Target]] = {}
        for target in selected:
            assert target.source is not None
            by_path.setdefault(target.source.resolve(), []).append(target)

        results: List[Dict[str, Any]] = []
        for path, targets in sorted(by_path.items(), key=lambda item: str(item[0])):
            source = path.read_text(encoding="utf-8")
            try:
                rel = str(path.relative_to(self.root))
            except ValueError:
                rel = str(path)

            regions: List[tuple[str, SourceRegion, str, str]] = []
            for target in sorted(targets, key=lambda item: item.id):
                try:
                    region = _find_function_region(source, target)
                except ValueError as exc:
                    results.append(
                        {
                            "target_id": target.id,
                            "source": rel,
                            "status": target.status,
                            "action": "skipped_no_region",
                            "error": str(exc),
                            "removed_externs": [],
                        }
                    )
                    continue
                regions.append(
                    (
                        target.id,
                        region,
                        target.function or "",
                        target.symbol or "",
                    )
                )

            if not regions:
                continue

            updated, rows = apply_strip_redundant_externs_to_regions(
                source,
                regions,
                earlier_only=True,
                source_path=path,
            )
            status_by_id = {target.id: target.status for target in targets}
            for row in rows:
                entry = {
                    "target_id": row["target_id"],
                    "source": rel,
                    "status": status_by_id.get(row["target_id"], ""),
                    "action": row["action"],
                    "removed_externs": list(row.get("removed_externs") or []),
                }
                if entry["action"] == "stripped" and dry_run:
                    entry["action"] = "would_strip"
                results.append(entry)

            if not dry_run and updated != source:
                path.write_text(updated, encoding="utf-8")

        results.sort(key=lambda row: (str(row.get("source") or ""), str(row.get("target_id") or "")))
        return results

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
        owner: str = "",
    ) -> str:
        if workflow == "tu-complete":
            unit = self.project.resolve_unit(target_id)
            if unit.source_path is None:
                raise ValueError(f"Unit {unit.name!r} has no source path")
            source = unit.source_path.read_text(encoding="utf-8")
            updated = self._tu_candidate_source(source, candidate, unit)
            if write:
                self._require_unit_claims(unit.name, owner)
                unit.source_path.write_text(updated, encoding="utf-8")
            return _source_diff(unit.source_path, source, updated)
        target = self._target(target_id)
        assert target.source is not None
        source = target.source.read_text(encoding="utf-8")
        region = _find_function_region(source, target)
        updated = _replace_function_source(
            source,
            region,
            candidate.source,
            target_function=target.function or "",
            target_symbol=target.symbol or "",
            source_path=target.source,
            target_id=target.id,
        )
        if write:
            self._require_claim(target, owner)
            target.source.write_text(updated, encoding="utf-8")
        return _source_diff(target.source, source, updated)

    def verify_claim(self, target_id: str, owner: str) -> tuple[bool, str]:
        """Verify that the owner matches the canonical claim for the target."""
        target = self._any_target(target_id)
        current = _claim_owner(target.extra.get("claim"))
        if not current:
            return False, f"target {target_id} has no claim"
        if current != owner:
            return False, f"target {target_id} claimed by {current!r}, not {owner!r}"
        return True, "claim verified"

    def _require_claim(self, target: Target, owner: str) -> None:
        if not owner:
            raise ValueError("owner required for promotion write")
        current = _claim_owner(target.extra.get("claim"))
        if not current:
            raise ValueError(f"target {target.id} has no claim; cannot promote without owner")
        if current != owner:
            raise ValueError(f"target {target.id} claimed by {current!r}, not {owner!r}")

    def _require_unit_claims(self, unit_name: str, owner: str) -> None:
        if not owner:
            raise ValueError("owner required for TU promotion write")
        # For TU completion, check all function claims in the unit
        raw_targets = load_targets(self.config)
        for target in raw_targets:
            if target.unit and self.project.resolve_unit(target.unit).name == unit_name:
                current = _claim_owner(target.extra.get("claim"))
                if current and current != owner:
                    raise ValueError(
                        f"unit {unit_name} has function {target.id} claimed by "
                        f"{current!r}, not {owner!r}"
                    )

    def finalize(self) -> None:
        # Each candidate evaluation restores both source and its prior object bytes.
        pass

    def read_target_source(self, target_id: str) -> str:
        """Read the current canonical source for a function target."""
        target = self._target(target_id)
        assert target.source is not None
        return target.source.read_text(encoding="utf-8")

    def build_dossier(
        self,
        target_id: str,
        history: List[Dict[str, Any]],
        options: Optional[Dict[str, Any]] = None,
    ) -> Dict[str, Any]:
        """Build a dossier dict for pipeline target."""
        return self._build_function_prompt("new", target_id, history)

    def detect_placeholder(self, target_id: str, source: str) -> bool:
        """Check if a target's source is a recognizable placeholder."""
        from .promotion import PlaceholderDetector
        return PlaceholderDetector().is_placeholder(source)

    def evaluate_canon(
        self,
        target_id: str,
        artifact_dir: Path,
        *,
        light: Optional[bool] = None,
    ) -> CandidateEvaluation:
        """Evaluate the canonical (current) source for a baseline snapshot.

        When ``light=True``, or when auto-detected as safe (placeholder body or
        known ``instruction_match`` of 0), skip the full MWCC build/equivalence
        path and return a COMPILES-style snapshot. Pass ``light=False`` to force
        a full evaluation.
        """
        from dataclasses import asdict

        from .promotion import PlaceholderDetector, evaluation_to_candidate

        target = self._target(target_id)
        assert target.source is not None and target.unit is not None
        source = target.source.read_text(encoding="utf-8")
        region = _find_function_region(source, target)
        current = source[region.content_start : region.content_end].strip()
        known_match = target.extra.get("instruction_match")
        is_placeholder = PlaceholderDetector().is_placeholder(current)
        if light is None:
            light = is_placeholder or (
                known_match is not None and float(known_match) <= 0.0
            )
        if light:
            match_percent = (
                0.0 if is_placeholder else float(known_match if known_match is not None else 0.0)
            )
            return CandidateEvaluation(
                status=CandidateStatus.COMPILES,
                compile_report=CompileReport(succeeded=True),
                match_percent=match_percent,
                symbol_accepted=False,
                promising=False,
                warnings=[
                    "light_baseline: skipped full MWCC evaluate_canon "
                    f"(placeholder={is_placeholder}, instruction_match={known_match!r})"
                ],
            )
        candidate = Candidate(source=current, hypothesis="canonical evaluation")
        evaluation = self._evaluate_function(target_id, candidate)
        return evaluation_to_candidate(asdict(evaluation))

    def target_source_path(self, target_id: str) -> Path:
        """Return the source file path for a target."""
        target = self._target(target_id)
        assert target.source is not None
        return target.source

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
        # Avoid ninja re-running dtk SPLIT through a shared config.json symlink.
        from tools.llm_harness.eval_cache import stamp_split_config

        stamp_split_config(workspace, self.config.region)


    # ------------------------------------------------------------------
    # §18 — Pipeline adapter methods (worktree-based evaluation)
    # ------------------------------------------------------------------

    def apply_candidate(self, root: Path, target_id: str, candidate: Candidate) -> SourcePatch:
        target = self._any_target(target_id)
        if target.source is None:
            raise ValueError(f"Target {target_id!r} has no source path")
        source_path = root / target.source.relative_to(self.root)
        source = source_path.read_text(encoding="utf-8")
        region = _find_function_region(source, target)
        self._validate_source(source[region.content_start : region.content_end], candidate.source)
        updated = _replace_function_source(
            source,
            region,
            candidate.source,
            target_function=target.function or "",
            target_symbol=target.symbol or "",
            source_path=target.source,
            target_id=target.id,
        )
        source_path.write_text(updated, encoding="utf-8")
        return SourcePatch(slot_id=target_id, source=candidate.source)

    def evaluate_candidate(
        self, root: Path, target_id: str, candidate: Candidate, artifact_dir: Path,
    ) -> CandidateEvaluation:
        target = self._any_target(target_id)
        if target.source is None or target.unit is None or target.symbol is None:
            raise ValueError(f"Target {target_id!r} must have source, unit, and symbol")

        source_path = root / target.source.relative_to(self.root)
        original = source_path.read_text(encoding="utf-8")
        region = _find_function_region(original, target)
        self._validate_source(original[region.content_start : region.content_end], candidate.source)
        candidate_file = _replace_function_source(
            original,
            region,
            candidate.source,
            target_function=target.function or "",
            target_symbol=target.symbol or "",
            source_path=target.source,
            target_id=target.id,
        )

        unit = self.project.resolve_unit(target.unit)

        def _ws(p: Optional[Path]) -> Optional[Path]:
            if p is None:
                return None
            try:
                return root / p.relative_to(self.root)
            except ValueError:
                return p

        ws_source = _ws(unit.source_path)
        ws_base = _ws(unit.base_path)
        ws_target = _ws(unit.target_path)

        original_object = ws_base.read_bytes() if ws_base and ws_base.is_file() else None

        try:
            source_path.write_text(candidate_file, encoding="utf-8")
            if ws_base is None:
                raise ValueError(f"Unit {unit.name!r} has no candidate object path")
            ws_base.unlink(missing_ok=True)

            build_error = self._build_object_at(root, ws_base)
            if build_error:
                return CandidateEvaluation(
                    status=CandidateStatus.COMPILE_ERROR,
                    compile_report=CompileReport(succeeded=False, exit_code=-1),
                    match_percent=0.0,
                )

            report_path = artifact_dir / "objdiff-report.json"
            report_path.parent.mkdir(parents=True, exist_ok=True)
            with self._phase("objdiff"):
                objdiff_result = subprocess.run(
                    [
                        self.project.objdiff_bin(),
                        "report", "generate",
                        "-p", str(root),
                        "-o", str(report_path),
                        "-f", "json-pretty",
                    ],
                    cwd=root, capture_output=True, text=True,
                )
                match_percent = 0.0
                if objdiff_result.returncode == 0 and report_path.is_file():
                    report = json.loads(report_path.read_text(encoding="utf-8"))
                    for entry in report.get("units", []):
                        if entry.get("name") == unit.name:
                            for fn in entry.get("functions", []):
                                if fn.get("name") == target.symbol:
                                    match_percent = float(fn.get("match_percent", 0.0) or 0.0)
                                    break

            if ws_target is None or not ws_target.is_file():
                return CandidateEvaluation(
                    status=CandidateStatus.INVALID_RESPONSE,
                    compile_report=CompileReport(succeeded=True),
                    match_percent=match_percent,
                    warnings=["retail object not found in workspace"],
                )
            if ws_base is None or not ws_base.is_file():
                return CandidateEvaluation(
                    status=CandidateStatus.INVALID_RESPONSE,
                    compile_report=CompileReport(succeeded=True),
                    match_percent=match_percent,
                    warnings=["candidate object not found after build"],
                )

            try:
                retail_function = extract_function(ws_target, target.symbol)
                candidate_function = extract_function(ws_base, target.symbol)
            except (FileNotFoundError, ValueError) as exc:
                return CandidateEvaluation(
                    status=CandidateStatus.INVALID_RESPONSE,
                    compile_report=CompileReport(succeeded=True),
                    match_percent=match_percent,
                    warnings=[
                        _symbol_extraction_detail(
                            exc,
                            candidate_source=candidate.source,
                            target_id=target.id,
                            target_symbol=target.symbol,
                            object_path=ws_base,
                        )
                    ],
                )

            structural_report = None
            try:
                structural_report = compare_structural(retail_function, candidate_function)
            except Exception:
                pass

            size_ok, size_detail = _function_size_comparison(
                retail_function.size, candidate_function.size
            )

            cstatus = CandidateStatus.COMPILES
            if match_percent >= 100.0:
                cstatus = CandidateStatus.FULL_MATCH
            elif match_percent > 0.0:
                cstatus = CandidateStatus.CODE_MATCH
            if structural_report is not None and structural_report.total_score >= 0.75 and cstatus == CandidateStatus.COMPILES:
                cstatus = CandidateStatus.STRUCTURALLY_ALIGNED

            return CandidateEvaluation(
                status=cstatus,
                compile_report=CompileReport(succeeded=True),
                match_percent=match_percent,
                structural_report=structural_report,
                function_size=candidate_function.size,
                retail_size=retail_function.size,
            )
        finally:
            source_path.write_text(original, encoding="utf-8")
            if ws_base:
                if original_object is None:
                    ws_base.unlink(missing_ok=True)
                else:
                    ws_base.write_bytes(original_object)

    def decode_function(self, root: Path, target_id: str, side: str) -> Any:
        target = self._any_target(target_id)
        if target.unit is None or target.symbol is None:
            raise ValueError(f"Target {target_id!r} must have unit and symbol")
        unit = self.project.resolve_unit(target.unit)
        if side == "retail":
            obj_path = unit.target_path
        elif side == "candidate":
            obj_path = unit.base_path
        else:
            raise ValueError(f"Unknown side: {side!r}, expected 'retail' or 'candidate'")
        if obj_path is None:
            raise ValueError(f"Unit {unit.name!r} has no {side} object path")
        try:
            ws_path = root / obj_path.relative_to(self.root)
        except ValueError:
            ws_path = obj_path
        if not ws_path.is_file():
            raise FileNotFoundError(f"{side} object not found: {ws_path}")
        return extract_function(ws_path, target.symbol)

    def protected_functions(self, target_id: str) -> List[str]:
        target = self._any_target(target_id)
        if target.unit is None:
            return []
        result: List[str] = []
        for item in load_targets(self.config):
            if (
                item.id == target_id
                or item.unit != target.unit
                or item.status not in {"FULL_MATCH", "EQUIVALENT_MATCH"}
            ):
                continue
            result.append(item.id)
        return result

    def run_promotion_validation(self, target_id: str, artifact_dir: Path) -> List[ValidationStepResult]:
        steps: List[ValidationStepResult] = []
        target = self._any_target(target_id)

        try:
            unit = self.project.resolve_unit(target.unit) if target.unit else None
        except ValueError:
            unit = None

        # Step 1 — Object size check
        if unit and unit.target_path and unit.base_path:
            try:
                size_result = check_object_size(
                    project_root=self.root,
                    region=self.config.region,
                    unit_hint=unit.name,
                    retail_object=unit.target_path,
                    decomp_object=unit.base_path,
                )
                steps.append(ValidationStepResult(
                    name="object_size_check",
                    succeeded=size_result.ok,
                    exit_code=0 if size_result.ok else -1,
                    detail=size_result.notes,
                ))
            except Exception as exc:
                steps.append(ValidationStepResult(
                    name="object_size_check",
                    succeeded=False,
                    exit_code=-1,
                    detail=f"size check raised {type(exc).__name__}: {exc}",
                ))
        else:
            steps.append(ValidationStepResult(
                name="object_size_check",
                succeeded=False,
                exit_code=-1,
                detail="unresolvable unit or missing object paths",
            ))

        # Step 2 — Protected function regression check
        if unit and target.unit:
            protected_ids = self.protected_functions(target_id)
            if protected_ids:
                try:
                    unit_report = report_unit(self.project, unit)
                    functions = {fn.name: fn for fn in unit_report.functions}
                    regressions: List[str] = []
                    for tid in protected_ids:
                        protected = self._any_target(tid)
                        if protected.symbol:
                            fn = functions.get(protected.symbol)
                            percent = fn.match_percent if fn else 0.0
                            if percent < 100.0:
                                regressions.append(
                                    f"{tid} ({protected.status}): {percent:.3f}%"
                                )
                    ok = not regressions
                    steps.append(ValidationStepResult(
                        name="protected_function_regression_check",
                        succeeded=ok,
                        exit_code=0 if ok else -1,
                        detail="; ".join(regressions) if regressions else "no regressions",
                    ))
                except Exception as exc:
                    steps.append(ValidationStepResult(
                        name="protected_function_regression_check",
                        succeeded=False,
                        exit_code=-1,
                        detail=f"regression check raised {type(exc).__name__}: {exc}",
                    ))
            else:
                steps.append(ValidationStepResult(
                    name="protected_function_regression_check",
                    succeeded=True,
                    exit_code=0,
                    detail="no protected functions in this unit",
                ))
        else:
            steps.append(ValidationStepResult(
                name="protected_function_regression_check",
                succeeded=True,
                exit_code=0,
                detail="no unit — skipping protected function check",
            ))

        # Step 3 — Coop cycle integration
        try:
            cycle_result = subprocess.run(
                [sys.executable, "tools/coop/run.py", "cycle", target_id,
                 "--hypothesis", "promotion validation",
                 "--next-change", "None"],
                cwd=self.root,
                text=True, capture_output=True, timeout=600,
            )
            cycle_ok = cycle_result.returncode == 0
            cycle_lines = cycle_result.stdout.strip().split("\n")[-5:] if cycle_result.stdout else []
            cycle_detail = " ".join(l.strip() for l in cycle_lines if l.strip())[:500]
            steps.append(ValidationStepResult(
                name="coop_cycle",
                succeeded=cycle_ok,
                exit_code=cycle_result.returncode,
                detail=cycle_detail or ("passed" if cycle_ok else cycle_result.stderr.strip()[:500]),
            ))
        except subprocess.TimeoutExpired:
            steps.append(ValidationStepResult(
                name="coop_cycle",
                succeeded=False,
                exit_code=-1,
                detail="coop cycle timed out (600 s)",
            ))
        except Exception as exc:
            steps.append(ValidationStepResult(
                name="coop_cycle",
                succeeded=False,
                exit_code=-1,
                detail=f"coop cycle raised {type(exc).__name__}: {exc}",
            ))

        return steps

    def _build_object_at(self, root: Path, object_path: Path) -> str:
        with self._phase("ninja"):
            completed = subprocess.run(
                [self.project.ninja_bin(), str(object_path.relative_to(root))],
                cwd=root,
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


def create_adapter(root: Path, settings: Dict[str, Any]) -> XenobladeAdapter:
    return XenobladeAdapter(root, settings)


def _claim_owner(claim: Any) -> Optional[str]:
    """Extract owner from a claim dict or legacy string claim."""
    if isinstance(claim, dict):
        owner = claim.get("owner")
        return str(owner) if owner else None
    if isinstance(claim, str) and claim:
        return claim
    return None


def _history_row_symbol_accepted(row: Dict[str, Any]) -> bool:
    """True when an experiments.jsonl row already reached symbol acceptance."""
    evaluation = row.get("evaluation") or {}
    if row.get("error"):
        return False
    if evaluation.get("symbol_accepted"):
        return True
    metrics = evaluation.get("metrics") or {}
    if metrics.get("symbol_accepted"):
        return True
    status = str(evaluation.get("status") or "").upper()
    if status in {"FULL_MATCH", "EQUIVALENT_MATCH"} and bool(evaluation.get("accepted")):
        return True
    return False


def _attempt_sort_key(row: Dict[str, Any]) -> str:
    """Oldest→newest ordering key for prior attempts.

    `timestamp` is the ISO-formatted completion time the harness records for
    every run. Sorting prior attempts oldest→newest keeps the head of the
    `prior_harness_attempts` array byte-identical as new attempts append at
    the tail: the cacheable prefix grows through the older entries and only
    the trailing newly-appended entry perturbs DeepSeek's prefix cache.
    """
    return str(row.get("timestamp") or "")


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
) -> None:
    """Attach the TU dossier volatile tail (oldest→newest attempts).

    Called after all per-build residual evidence has been written into the
    dossier, so this field lands as the final key — matching the
    function-workflow ordering convention of stable-before-volatile.
    """
    dossier["prior_harness_attempts"] = [_compact_prior_attempt(row) for row in recent_history]


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
            f"0x{retail_size:X} by 0x{delta:X} ({delta} bytes) -- recorded but not gating"
        )
    spare = -delta
    return True, (
        f"candidate function size 0x{candidate_size:X} fits retail 0x{retail_size:X} "
        f"(0x{spare:X} spare)"
    )


def _symbol_extraction_detail(
    exc: BaseException,
    *,
    candidate_source: str,
    target_id: str,
    target_symbol: str,
    object_path: Optional[Path],
) -> str:
    """Explain missing linker symbols so match_repair can rename stubs."""
    parts = [f"function-size extraction failed: {exc}"]
    stub = "harness_stub_" + target_id.replace("-", "_") if target_id else ""
    if stub and stub in (candidate_source or ""):
        parts.append(
            f"candidate still defines placeholder {stub!r}; emit linker symbol "
            f"{target_symbol!r} instead"
        )
    elif target_symbol and target_symbol not in (candidate_source or ""):
        parts.append(
            f"candidate source does not mention expected linker symbol {target_symbol!r}"
        )
    if object_path is not None and object_path.is_file() and target_symbol:
        try:
            present = sorted(fn.name for fn in list_text_functions(object_path))
        except Exception:
            present = []
        if present and target_symbol not in present:
            hints = [
                name
                for name in present
                if name.startswith("harness_stub_")
                or name.endswith(target_symbol.split("__", 1)[0])
            ][:8]
            if hints:
                parts.append("object has related symbols: " + ", ".join(hints))
    return "; ".join(parts)


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

