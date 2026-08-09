from __future__ import annotations

import json
import subprocess
import sys
import time
from contextlib import AbstractContextManager, nullcontext
from dataclasses import dataclass
from pathlib import Path
from typing import Any, Callable, Dict, List, Optional

PhaseTimer = Callable[[str], AbstractContextManager[Any]]

# Backoff schedule (s) for the objdiff report retries in evaluate_unit_match.
# Sized for the multi-agent build race: two sessions' global ninja runs (under
# different lock schemes) can double-write the same dirty decomp .o, keeping
# it mid-write for ~0.5-2s. A single 0.25s re-read was not enough (run 3:
# 0 recoveries, and a byte-identical draft was mis-evaluated NOT_STARTED —
# the acceptance was lost). Three attempts with growing backoff escape most
# windows while keeping the retry cheap (report_unit is an isolated objdiff
# run, ~10-50ms each).
_REPORT_RETRY_BACKOFF_S = (0.25, 0.75, 1.75)

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
    # False when objdiff omitted fuzzy_match_percent: the symbol could not be
    # paired between retail and candidate objects (name/linkage mismatch), so
    # match_percent is a meaningless 0.0 default rather than a real diff score.
    mapped: bool = True


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
        # A certificate means a sound equivalence proof ALREADY succeeded
        # (register-renaming witness or full-instruction-match). That is the
        # source of truth even when the objdiff match% is broken/None — some
        # units' objdiff reports return 0%/None for byte-identical functions
        # (null retail symbols, e.g. CBattleManager), which would otherwise
        # classify as NOT_STARTED/COMPILES despite the witness certifying.
        if certificate is not None and equivalence == ProofStatus.EQUIVALENT:
            if certificate.get("evidence") == "full-instruction-match":
                return "FULL_MATCH"
            return "EQUIVALENT_MATCH"
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
    # r8 WS-1: witness rejection telemetry — ``witness_gate`` (reloc/rho/
    # execute/structural/...) + ``witness_reason`` (the failing slot detail),
    # surfaced to the model so it gets an actionable fix hint instead of a
    # bare "NOT certifiable".
    witness_gate: Optional[str] = None
    witness_reason: Optional[str] = None


def evaluate_unit_match(
    project: Project,
    unit: ObjdiffUnit,
    symbol: Optional[str],
    *,
    run_equivalence: bool = True,
    run_smt: bool = True,
    certify_full_match: bool = True,
    linked: bool = False,
    target_id: str | None = None,
    phase_timer: Optional[PhaseTimer] = None,
    declared_return: str | None = None,
    contract: str = "auto",
    witness_timeout_ms: int = 0,
    witness_enabled: bool = True,
) -> MatchEvaluation:
    """Score a unit (and optionally SMT-prove one symbol).

    ``run_equivalence=False`` skips both the register-renaming witness and the
    SMT solver.  ``run_smt=False`` (with ``run_equivalence=True``) still runs
    the cheap pre-SMT register-renaming witness, but skips the Z3 probe: a
    function the witness cannot certify reports
    ``inconclusive_smt_disabled`` and can never reach ``EQUIVALENT_MATCH``
    without an explicit ``--smt`` run.  ``witness_enabled=False`` additionally
    skips the witness probe entirely — only byte-identical ``FULL_MATCH``
    (with reloc-site equality) can be accepted; the probe reports
    ``inconclusive_smt_disabled`` for everything else.

    ``phase_timer`` is an optional ``lambda phase: contextmanager`` used by the
    LLM harness to attribute wall time to ``objdiff`` vs ``smt``.
    """
    timer = phase_timer or (lambda _phase: nullcontext())
    with timer("objdiff"):
        unit_report = report_unit(project, unit)
        fn_match = find_function_match(unit_report, symbol)
        # Retry the objdiff read once when the target symbol is missing from
        # the report. Under concurrent rebuilds (multi-agent harness), the
        # decomp .o can be read mid-write -> objdiff reports nothing for it
        # (find_function_match -> None -> classify_status -> NOT_STARTED,
        # which the caller then writes back — a silent no-op that blinds the
        # registry to the target's real match%). A second read a moment later
        # lands a settled object. Mirrors the harness hexdiff
        # allowBuildRetry for the same race. Cheap: report_unit is an isolated
        # copy + objdiff run (~10-50ms); only fires when the symbol is missing
        # AND a registry write is pending (target_id + symbol).
        if fn_match is None and target_id and symbol:
            for _delay in _REPORT_RETRY_BACKOFF_S:
                time.sleep(_delay)
                unit_report = report_unit(project, unit)
                fn_match = find_function_match(unit_report, symbol)
                if fn_match is not None:
                    print(
                        f"objdiff report retry recovered {symbol} "
                        f"({fn_match.match_percent:.1f}%) — first read raced a concurrent rebuild",
                        file=sys.stderr,
                    )
                    break
    equivalence: Optional[ProofStatus] = None
    detail = ""
    certificate = None
    proof: Optional[ProofResult] = None
    certificate_checked = bool(target_id and symbol and fn_match and run_equivalence)
    witness_gate: Optional[str] = None
    witness_reason: Optional[str] = None
    pct = fn_match.match_percent if fn_match else None
    # Witness-only path (run_smt=False, the no-SMT pipeline): the objdiff
    # match% gate (should_probe_equivalence) is unreliable — some units' objdiff
    # reports under-report or find NOTHING (0%/None for byte-identical
    # functions: null retail symbols, e.g. CBattleManager), which silently
    # skipped certification and made certify requests fail. certify_unit_symbol
    # reads the ELF bytes directly: byte-identical -> FULL_MATCH
    # (full-instruction-match cert), reg-swap-only -> register-renaming witness
    # internally. Route the witness-only path through it whenever a target_id
    # is present (the harness's certify path), even when objdiff found no
    # fn_match (the registry symbol is the lookup key). NOT gated on
    # witness_enabled (adversarial review F1): with the witness disabled the
    # byte-identity direct read is the ONLY acceptance path left — gating it on
    # witness_enabled made byte-identical targets with a broken/None objdiff
    # report (CBattleManager class) un-acceptable by ANY path (the 2nd/3rd
    # branches require fn_match). certify_unit_symbol(witness_enabled=False)
    # does the right thing: byte-identical -> FULL_MATCH, else ->
    # INCONCLUSIVE_SMT_DISABLED (never EQUIVALENT, never Z3). For run_smt=True
    # keep the historical gate.
    if (not run_smt) and target_id and symbol and (
        (fn_match is not None and (pct is None or pct < 100.0))
        or (fn_match is None and symbol)
    ):
        with timer("smt"):
            witness_diag: dict[str, Any] = {}
            probe = certify_unit_symbol(
                project, unit, symbol, target_id, diag=witness_diag,
                witness_timeout_ms=witness_timeout_ms,
                witness_enabled=witness_enabled,
            )
        equivalence = probe.status
        detail = probe.detail
        certificate = probe.certificate
        proof = probe.proof
        witness_gate = witness_diag.get("witness_gate")
        witness_reason = witness_diag.get("witness_reason")
    elif run_equivalence and symbol and fn_match and should_probe_equivalence(pct):
        with timer("smt"):
            probe: EquivalenceProbe = prove_unit_symbol(
                project, unit, fn_match.name, linked=linked, target_id=target_id,
                declared_return=declared_return, contract=contract, smt=run_smt,
                witness_timeout_ms=witness_timeout_ms,
                witness_enabled=witness_enabled,
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
        with timer("smt"):
            probe = certify_unit_symbol(project, unit, fn_match.name, target_id,
                                       witness_timeout_ms=witness_timeout_ms,
                                       witness_enabled=witness_enabled)
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
        witness_gate=witness_gate,
        witness_reason=witness_reason,
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
                    mapped="fuzzy_match_percent" in fn,
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
