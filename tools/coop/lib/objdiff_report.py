from __future__ import annotations

import json
import subprocess
from dataclasses import dataclass
from pathlib import Path
from typing import Any, Dict, List, Optional

from tools.coop.lib.config import CoopConfig
from tools.coop.lib.project import ObjdiffUnit, Project


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


def meets_required_level(
    required_level: str,
    status: str,
    *,
    function_match: Optional[float],
    unit: UnitReport,
    symbol: Optional[str],
) -> bool:
    if required_level == "FULL_MATCH":
        if symbol:
            return function_match is not None and function_match >= 100.0
        return unit.code_match_percent >= 100.0 and unit.data_match_percent >= 100.0
    if required_level == "CODE_MATCH":
        if symbol and function_match is not None:
            return function_match >= 95.0
        return unit.fuzzy_match_percent >= 95.0
    if required_level == "STRUCTURAL":
        return status in {"STRUCTURAL", "HIGH_MATCH", "CODE_MATCH", "FULL_MATCH", "BEHAVIOR_VERIFIED"}
    if required_level == "TRACE_ONLY":
        return status != "NOT_STARTED"
    return status == required_level


def classify_status(
    function_match: Optional[float],
    unit: UnitReport,
    *,
    symbol: Optional[str],
) -> str:
    if symbol:
        if function_match is None:
            return "NOT_STARTED"
        if function_match >= 100.0:
            return "FULL_MATCH"
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
