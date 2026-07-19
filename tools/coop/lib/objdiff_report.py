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
    certify_unit_symbol,
    prove_unit_symbol,
    should_probe_equivalence,
)
from tools.coop.lib.equivalence_policy import classify_for_promotion_legacy
from tools.coop.lib.project import ObjdiffUnit, Project
from tools.coop.lib.targets import EQUIVALENCE_PROMOTION_POLICY
from tools.ppc_equivalence.result import ProofResult, ProofStatus


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
    policy: Optional[CoopConfig] = None,
    certificate: Optional[dict] = None,
    proof: Optional[ProofResult] = None,
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
            if (
                function_match is not None
                and function_match >= EQUIVALENT_MATCH_MIN_PERCENT
                and equivalence == ProofStatus.EQUIVALENT
            ):
                if policy is None:
                    return True
                decision = classify_for_promotion_legacy(
                    equivalence,
                    function_match,
                    policy,
                    certificate=certificate,
                    proof=proof,
                )
                return decision.allowed
            return False
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
    policy: Optional[CoopConfig] = None,
    certificate: Optional[dict] = None,
    proof: Optional[ProofResult] = None,
) -> str:
    if symbol:
        if function_match is None:
            return "NOT_STARTED"
        if function_match >= 100.0:
            return "FULL_MATCH"
        can_equivalent = (
            function_match >= EQUIVALENT_MATCH_MIN_PERCENT
            and equivalence == ProofStatus.EQUIVALENT
        )
        if can_equivalent:
            if policy is None:
                return "EQUIVALENT_MATCH"
            decision = classify_for_promotion_legacy(
                equivalence,
                function_match,
                policy,
                certificate=certificate,
                proof=proof,
            )
            if decision.allowed:
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
    equivalence_certificate: dict | None = None
    certificate_checked: bool = False
    proof: Optional[ProofResult] = None
    equivalence_confidence: Optional[str] = None
    equivalence_policy: Optional[str] = None


def evaluate_unit_match(
    project: Project,
    unit: ObjdiffUnit,
    symbol: Optional[str],
    *,
    run_equivalence: bool = True,
    certify_full_match: bool = True,
    linked: bool = False,
    target_id: str | None = None,
) -> MatchEvaluation:
    unit_report = report_unit(project, unit)
    fn_match = find_function_match(unit_report, symbol)
    equivalence: Optional[ProofStatus] = None
    detail = ""
    certificate = None
    proof: Optional[ProofResult] = None
    certificate_checked = bool(target_id and symbol and fn_match and run_equivalence)
    pct = fn_match.match_percent if fn_match else None
    if run_equivalence and symbol and fn_match and should_probe_equivalence(pct):
        probe: EquivalenceProbe = prove_unit_symbol(
            project, unit, fn_match.name, linked=linked, target_id=target_id,
        )
        equivalence = probe.status
        detail = probe.detail
        certificate = probe.certificate
        proof = probe.proof
    elif (
        run_equivalence
        and certify_full_match
        and target_id
        and symbol
        and fn_match
        and pct is not None
        and pct >= 100.0
    ):
        probe = certify_unit_symbol(project, unit, fn_match.name, target_id)
        equivalence = probe.status
        detail = probe.detail
        certificate = probe.certificate
        proof = probe.proof
    status = classify_status(
        pct,
        unit_report,
        symbol=symbol,
        equivalence=equivalence,
        policy=project.config,
        certificate=certificate,
        proof=proof,
    )
    confidence: Optional[str] = None
    policy_id: Optional[str] = None
    if status == "EQUIVALENT_MATCH":
        decision = classify_for_promotion_legacy(
            equivalence,
            pct,
            project.config,
            certificate=certificate,
            proof=proof,
        )
        confidence = decision.confidence_tier
        policy_id = EQUIVALENCE_PROMOTION_POLICY
    return MatchEvaluation(
        unit_report=unit_report,
        fn_match=fn_match,
        status=status,
        equivalence=equivalence,
        equivalence_detail=detail,
        equivalence_certificate=certificate,
        certificate_checked=certificate_checked,
        proof=proof,
        equivalence_confidence=confidence,
        equivalence_policy=policy_id,
    )


def report_unit(project: Project, unit: ObjdiffUnit, *, isolated: bool = True) -> UnitReport:
    """Generate an objdiff unit report.

    When *isolated* is True (default), run against a temporary single-unit
    objdiff.json so missing unrelated objects cannot make the target unscorable.
    """
    config = project.config
    if isolated:
        return _report_unit_isolated(project, unit)

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
    return _parse_unit_report(report_path, unit.name)


def _report_unit_isolated(project: Project, unit: ObjdiffUnit) -> UnitReport:
    """Option B: temporary objdiff project containing only the owning unit."""
    import tempfile

    root_config = project.root / "objdiff.json"
    if not root_config.is_file():
        raise FileNotFoundError(f"objdiff.json missing at {root_config}")
    data = json.loads(root_config.read_text(encoding="utf-8"))
    matching = [entry for entry in data.get("units", []) if entry.get("name") == unit.name]
    if not matching:
        raise ValueError(f"Unit '{unit.name}' not present in objdiff.json")

    def _absolutize(entry: dict) -> dict:
        out = dict(entry)
        for key in ("target_path", "base_path"):
            value = out.get(key)
            if isinstance(value, str) and value and not Path(value).is_absolute():
                out[key] = str((project.root / value).resolve())
        scratch = out.get("scratch")
        if isinstance(scratch, dict):
            scratch = dict(scratch)
            ctx = scratch.get("ctx_path")
            if isinstance(ctx, str) and ctx and not Path(ctx).is_absolute():
                scratch["ctx_path"] = str((project.root / ctx).resolve())
            out["scratch"] = scratch
        metadata = out.get("metadata")
        if isinstance(metadata, dict):
            metadata = dict(metadata)
            src = metadata.get("source_path")
            if isinstance(src, str) and src and not Path(src).is_absolute():
                metadata["source_path"] = str((project.root / src).resolve())
            out["metadata"] = metadata
        return out

    single = {key: value for key, value in data.items() if key != "units"}
    single["units"] = [_absolutize(matching[0])]

    with tempfile.TemporaryDirectory(prefix="objdiff-unit-") as tmp:
        tmp_path = Path(tmp)
        (tmp_path / "objdiff.json").write_text(
            json.dumps(single, indent=2) + "\n", encoding="utf-8"
        )
        report_path = tmp_path / "report.json"
        cmd = [
            project.objdiff_bin(),
            "report",
            "generate",
            "-p",
            str(tmp_path),
            "-o",
            str(report_path),
            "-f",
            "json-pretty",
        ]
        cmd.extend(project.config.objdiff_report_args)
        subprocess.run(cmd, cwd=project.root, check=True)
        return _parse_unit_report(report_path, unit.name)


def _parse_unit_report(report_path: Path, unit_name: str) -> UnitReport:
    with report_path.open(encoding="utf-8") as f:
        report = json.load(f)

    for entry in report.get("units", []):
        if entry.get("name") != unit_name:
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
            unit_name=unit_name,
            code_match_percent=_percent(measures.get("matched_code"), measures.get("total_code")),
            data_match_percent=_percent(measures.get("matched_data"), measures.get("total_data")),
            fuzzy_match_percent=float(measures.get("fuzzy_match_percent", 0.0)),
            total_functions=int(measures.get("total_functions", 0)),
            matched_functions=int(measures.get("matched_functions", 0)),
            functions=functions,
        )

    raise ValueError(f"Unit '{unit_name}' not present in objdiff report")


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
