from __future__ import annotations

import json
import subprocess
from dataclasses import dataclass
from pathlib import Path
from typing import Any, Dict, List, Optional

from tools.coop.lib.config import CoopConfig
from tools.coop.lib.equivalence_check import (
    EQUIVALENT_MATCH_MIN_PERCENT,
    EquivalenceProbe,
    prove_unit_symbol,
    should_probe_equivalence,
)
from tools.coop.lib.project import ObjdiffUnit, Project
from tools.ppc_equivalence.result import ProofStatus


@dataclass
class FunctionMatch:
    name: str
    demangled_name: Optional[str]
    match_percent: float
    size: int


@dataclass
class UnitReport:
    unit_name: str
    code_match_percent: float
    data_match_percent: float
    fuzzy_match_percent: float
    total_functions: int
    matched_functions: int
    functions: List[FunctionMatch]


# Status ladder (best → worst). FULL_MATCH remains the strongest static claim;
# EQUIVALENT_MATCH is the default acceptance bar when policy is equivalent_match.
STATUS_RANK = {
    "FULL_MATCH": 100,
    "EQUIVALENT_MATCH": 90,
    "CODE_MATCH": 80,
    "HIGH_MATCH": 70,
    "STRUCTURAL": 50,
    "BEHAVIOR_VERIFIED": 45,
    "COMPILES": 20,
    "NOT_STARTED": 0,
}


def meets_required_level(
    required_level: str,
    status: str,
    *,
    function_match: Optional[float],
    unit: UnitReport,
    symbol: Optional[str],
    equivalence: Optional[ProofStatus] = None,
) -> bool:
    if required_level == "FULL_MATCH":
        if symbol:
            return function_match is not None and function_match >= 100.0
        return unit.code_match_percent >= 100.0 and unit.data_match_percent >= 100.0
    if required_level == "EQUIVALENT_MATCH":
        if status in {"EQUIVALENT_MATCH", "FULL_MATCH"}:
            return True
        if symbol:
            if function_match is not None and function_match >= 100.0:
                return True
            return (
                function_match is not None
                and function_match >= EQUIVALENT_MATCH_MIN_PERCENT
                and equivalence == ProofStatus.EQUIVALENT
            )
        # Unit-level: still require full code+data until unit-wide proofs exist.
        return unit.code_match_percent >= 100.0 and unit.data_match_percent >= 100.0
    if required_level == "CODE_MATCH":
        if symbol and function_match is not None:
            return function_match >= 95.0 or status in {"EQUIVALENT_MATCH", "FULL_MATCH"}
        return unit.fuzzy_match_percent >= 95.0 or status in {"EQUIVALENT_MATCH", "FULL_MATCH"}
    if required_level == "STRUCTURAL":
        return status in {
            "STRUCTURAL",
            "HIGH_MATCH",
            "CODE_MATCH",
            "EQUIVALENT_MATCH",
            "FULL_MATCH",
            "BEHAVIOR_VERIFIED",
        }
    if required_level == "TRACE_ONLY":
        return status != "NOT_STARTED"
    return status == required_level or STATUS_RANK.get(status, -1) >= STATUS_RANK.get(required_level, 999)


def classify_status(
    function_match: Optional[float],
    unit: UnitReport,
    *,
    symbol: Optional[str],
    equivalence: Optional[ProofStatus] = None,
) -> str:
    if symbol:
        if function_match is None:
            return "NOT_STARTED"
        if function_match >= 100.0:
            return "FULL_MATCH"
        if (
            function_match >= EQUIVALENT_MATCH_MIN_PERCENT
            and equivalence == ProofStatus.EQUIVALENT
        ):
            return "EQUIVALENT_MATCH"
        if function_match >= 95.0:
            return "CODE_MATCH"
        if function_match >= 70.0:
            return "HIGH_MATCH"
        if function_match > 0.0:
            return "STRUCTURAL"
        return "COMPILES"
    if unit.code_match_percent >= 100.0 and unit.data_match_percent >= 100.0:
        return "FULL_MATCH"
    if unit.fuzzy_match_percent >= 95.0:
        return "CODE_MATCH"
    if unit.fuzzy_match_percent >= 70.0:
        return "HIGH_MATCH"
    if unit.fuzzy_match_percent > 0.0:
        return "STRUCTURAL"
    return "COMPILES"


@dataclass
class MatchEvaluation:
    unit_report: UnitReport
    fn_match: Optional[FunctionMatch]
    status: str
    equivalence: Optional[ProofStatus]
    equivalence_detail: str = ""


def evaluate_unit_match(
    project: Project,
    unit: ObjdiffUnit,
    symbol: Optional[str],
    *,
    run_equivalence: bool = True,
    linked: bool = False,
) -> MatchEvaluation:
    unit_report = report_unit(project, unit)
    fn_match = find_function_match(unit_report, symbol)
    equivalence: Optional[ProofStatus] = None
    detail = ""
    pct = fn_match.match_percent if fn_match else None
    if run_equivalence and symbol and fn_match and should_probe_equivalence(pct):
        probe: EquivalenceProbe = prove_unit_symbol(
            project, unit, fn_match.name, linked=linked
        )
        equivalence = probe.status
        detail = probe.detail
    status = classify_status(pct, unit_report, symbol=symbol, equivalence=equivalence)
    return MatchEvaluation(
        unit_report=unit_report,
        fn_match=fn_match,
        status=status,
        equivalence=equivalence,
        equivalence_detail=detail,
    )


def report_unit(project: Project, unit: ObjdiffUnit) -> UnitReport:
    config = project.config
    report_path = config.resolve(config.report_cache)
    report_path.parent.mkdir(parents=True, exist_ok=True)

    cmd = [
        project.objdiff_bin(),
        "report",
        "generate",
        "-p",
        str(project.root),
        "-o",
        str(report_path),
        "-f",
        "json-pretty",
    ]
    cmd.extend(config.objdiff_report_args)
    subprocess.run(cmd, cwd=project.root, check=True)

    with report_path.open(encoding="utf-8") as f:
        report = json.load(f)

    for entry in report.get("units", []):
        if entry.get("name") != unit.name:
            continue
        measures = entry.get("measures") or {}
        functions = []
        for fn in entry.get("functions", []):
            meta = fn.get("metadata") or {}
            functions.append(
                FunctionMatch(
                    name=fn.get("name", ""),
                    demangled_name=meta.get("demangled_name"),
                    match_percent=float(fn.get("fuzzy_match_percent", 0.0)),
                    size=int(fn.get("size", 0)),
                )
            )
        return UnitReport(
            unit_name=unit.name,
            code_match_percent=_percent(measures.get("matched_code"), measures.get("total_code")),
            data_match_percent=_percent(measures.get("matched_data"), measures.get("total_data")),
            fuzzy_match_percent=float(measures.get("fuzzy_match_percent", 0.0)),
            total_functions=int(measures.get("total_functions", 0)),
            matched_functions=int(measures.get("matched_functions", 0)),
            functions=functions,
        )

    raise ValueError(f"Unit '{unit.name}' not present in objdiff report")


def find_function_match(unit_report: UnitReport, symbol: Optional[str]) -> Optional[FunctionMatch]:
    if not symbol:
        return None
    for fn in unit_report.functions:
        if fn.name == symbol or fn.demangled_name == symbol:
            return fn
    lowered = symbol.lower()
    for fn in unit_report.functions:
        if lowered in fn.name.lower():
            return fn
        if fn.demangled_name and lowered in fn.demangled_name.lower():
            return fn
    return None


def diff_function_json(
    project: Project,
    unit: ObjdiffUnit,
    symbol: str,
    output_path: Path,
) -> None:
    cmd = [
        project.objdiff_bin(),
        "diff",
        "-p",
        str(project.root),
        "-u",
        unit.name,
        "-o",
        str(output_path),
        "--format",
        "json-pretty",
        symbol,
    ]
    subprocess.run(cmd, cwd=project.root, check=True)


def _percent(matched: Any, total: Any) -> float:
    if not total:
        return 0.0
    return 100.0 * float(matched or 0) / float(total)
