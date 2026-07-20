"""Phase 12 rollout plumbing for scalar FP exact v2 (SCALAR_FP_V2.md).

Production execution remains default-off. ``enable_scalar_fp_exact_v2_production``
is a gated entry point that requires explicit env **and** a nonempty capability
allowlist in the shadow/canary manifest.
"""

from __future__ import annotations

import argparse
import importlib
import json
import os
from dataclasses import dataclass
from pathlib import Path
from typing import Any, Mapping, Sequence

from tools.ppc_equivalence.fp_capabilities import (
    FP_EXPERIMENTAL_SUBCAPABILITY_MODEL_VERSIONS,
    SCALAR_FP_EXACT_V2_ENV,
    set_scalar_fp_exact_v2_module_flag,
    scalar_fp_exact_v2_enabled,
)

_SCALAR_FP_V2_PRODUCTION_ENV = "SCALAR_FP_EXACT_V2_PRODUCTION"
_SCALAR_FP_V2_ALLOWLIST_ENV = "SCALAR_FP_EXACT_V2_ALLOWLIST_MANIFEST"

_REPO_ROOT = Path(__file__).resolve().parents[2]
_DEFAULT_TARGETS_FILE = _REPO_ROOT / "tools" / "coop" / "targets.json"

# Census notes from Phase 0 — recommended canary target ids (US region).
SCALAR_FP_V2_CANARY_TARGETS: dict[str, str] = {
    "fp-load-store": "us-80040998",
    "fp-bitwise": "us-803e9714",
    "fp-convert": "us-802c9b90",
    "fp-fpscr-control": "us-802dd650",
    "fp-scalar-arithmetic": "us-802dd650",
}

# Authoritative allowlist order (Phase 12 — SCALAR_FP_V2.md).
SCALAR_FP_V2_ALLOWLIST_ORDER: tuple[str, ...] = (
    "fp-load-store",
    "fp-compare",
    "fp-convert",
    "fp-fpscr-control",
    "fp-scalar-arithmetic",
    "fp-traps",
    "fp-fused-arithmetic",
)

# Phase 0–12 unit-test modules the readiness gate must be able to import.
SCALAR_FP_V2_PHASE_TEST_MODULES: tuple[str, ...] = (
    "tools.ppc_equivalence.tests.test_scalar_fp_contract_v2",
    "tools.ppc_equivalence.tests.test_fp_round",
    "tools.ppc_equivalence.tests.test_fp_exact_arith",
    "tools.ppc_equivalence.tests.test_fp_fpscr",
    "tools.ppc_equivalence.tests.test_fp_ni_v2",
    "tools.ppc_equivalence.tests.test_fp_exact_phase6",
    "tools.ppc_equivalence.tests.test_fp_exact_symbolic",
    "tools.ppc_equivalence.tests.test_fp_traps_fe0_fe1",
    "tools.ppc_equivalence.tests.test_fp_exact_fused",
    "tools.ppc_equivalence.tests.test_fp_scalar_obligations_v2",
    "tools.ppc_equivalence.tests.test_scalar_fp_v2_corpora",
    "tools.ppc_equivalence.tests.test_scalar_fp_v2_rollout",
)

# Honest pre-allowlist blockers (production switch must stay off).
PRODUCTION_SWITCH_BLOCKERS: tuple[str, ...] = (
    "NI=1/non-RNE corpus rows are exact_kernel_v2 supplements — live Dolphin attestation still missing (Phase 11)",
    "Symbolic fdiv/fused payload formulas still fail-closed; fadd/fmul payload path landed (Phase 7)",
    "Bottom-up recertification / certificate debt unresolved (~7 queued)",
    "ARCHITECTURE_MODEL / result-format bump not performed",
    "enable_scalar_fp_exact_v2_production() remains NotImplemented",
)

_DEFAULT_CANARY_MANIFEST = (
    Path(__file__).resolve().parents[1]
    / "coop"
    / "capability_manifest.scalar_fp_v2_canary.json.example"
)


class ScalarFPExactV2ProductionError(RuntimeError):
    """Raised when production switch preconditions are not met."""


@dataclass(frozen=True, slots=True)
class ReadinessCheck:
    """One infrastructure readiness probe."""

    name: str
    ok: bool
    summary: str
    details: tuple[str, ...] = ()


@dataclass(frozen=True, slots=True)
class ReadinessReport:
    """Aggregate Phase 12 production-switch readiness (plumbing, not promotion)."""

    schema_version: int
    production_switch_ready: bool
    infrastructure_ready: bool
    checks: tuple[ReadinessCheck, ...]
    blockers: tuple[str, ...]
    production_enabled: bool = False

    def to_dict(self) -> dict[str, Any]:
        return {
            "schema_version": self.schema_version,
            "production_switch_ready": self.production_switch_ready,
            "infrastructure_ready": self.infrastructure_ready,
            "production_enabled": self.production_enabled,
            "checks": {
                item.name: {
                    "ok": item.ok,
                    "summary": item.summary,
                    "details": list(item.details),
                }
                for item in self.checks
            },
            "blockers": list(self.blockers),
        }


def recommended_canary_target_ids(
    capabilities: Sequence[str] | None = None,
) -> dict[str, str]:
    """Return census-recommended canary target ids for ``capabilities``."""
    if capabilities is None:
        return dict(SCALAR_FP_V2_CANARY_TARGETS)
    return {
        cap: SCALAR_FP_V2_CANARY_TARGETS[cap]
        for cap in capabilities
        if cap in SCALAR_FP_V2_CANARY_TARGETS
    }


def list_recommended_canary_targets() -> list[dict[str, str]]:
    """CLI-friendly rows: capability, target_id, allowlist_order index."""
    rows: list[dict[str, str]] = []
    order = {cap: idx for idx, cap in enumerate(SCALAR_FP_V2_ALLOWLIST_ORDER, start=1)}
    for capability, target_id in SCALAR_FP_V2_CANARY_TARGETS.items():
        rows.append(
            {
                "capability": capability,
                "target_id": target_id,
                "allowlist_order": str(order.get(capability, "")),
            }
        )
    return rows


def load_scalar_fp_v2_canary_manifest(path: Path | None = None) -> dict[str, Any]:
    """Load the shadow/canary manifest template or a user-provided copy."""
    manifest_path = path or _DEFAULT_CANARY_MANIFEST
    with manifest_path.open(encoding="utf-8") as handle:
        return json.load(handle)


def _manifest_allowlists(manifest: Mapping[str, Any]) -> dict[str, list[str]]:
    raw = manifest.get("allowed_tier_a_capabilities") or {}
    if not isinstance(raw, Mapping):
        return {}
    return {str(key): list(value or []) for key, value in raw.items()}


def validate_shadow_manifest(manifest: Mapping[str, Any]) -> tuple[bool, list[str]]:
    """Validate the Phase 12 shadow/canary manifest shape."""
    reasons: list[str] = []
    if manifest.get("automatic_promotion") is True:
        reasons.append("automatic_promotion must remain false")
    if not manifest.get("shadow_mode"):
        reasons.append("shadow_mode must be true for canary manifests")
    allowlists = _manifest_allowlists(manifest)
    if not allowlists:
        reasons.append("allowed_tier_a_capabilities must be a mapping")
    for capability in SCALAR_FP_V2_ALLOWLIST_ORDER:
        if capability not in allowlists:
            reasons.append(f"missing allowlist key {capability!r}")
        elif not isinstance(allowlists[capability], list):
            reasons.append(f"allowlist for {capability!r} must be a list")
    meta = manifest.get("scalar_fp_exact_v2")
    if not isinstance(meta, Mapping):
        reasons.append("scalar_fp_exact_v2 metadata section required")
    else:
        order = meta.get("allowlist_order")
        if list(order or []) != list(SCALAR_FP_V2_ALLOWLIST_ORDER):
            reasons.append("scalar_fp_exact_v2.allowlist_order mismatch")
    return (not reasons, reasons)


def _load_target_ids(targets_file: Path | None = None) -> frozenset[str]:
    path = targets_file or _DEFAULT_TARGETS_FILE
    document = json.loads(path.read_text(encoding="utf-8"))
    rows = document.get("targets") or []
    return frozenset(str(row.get("id", "")) for row in rows if row.get("id"))


def check_canary_targets_in_registry(
    *,
    targets_file: Path | None = None,
) -> tuple[bool, list[str]]:
    """Ensure census canary target ids exist in ``targets.json``."""
    known = _load_target_ids(targets_file)
    missing: list[str] = []
    seen: set[str] = set()
    for capability, target_id in SCALAR_FP_V2_CANARY_TARGETS.items():
        if target_id in seen:
            continue
        seen.add(target_id)
        if target_id not in known:
            missing.append(f"{capability}:{target_id}")
    return (not missing, missing)


def check_experimental_models_defined() -> tuple[bool, list[str]]:
    """Ensure every allowlist capability has an experimental model identity."""
    missing = [
        capability
        for capability in SCALAR_FP_V2_ALLOWLIST_ORDER
        if capability not in FP_EXPERIMENTAL_SUBCAPABILITY_MODEL_VERSIONS
        or not FP_EXPERIMENTAL_SUBCAPABILITY_MODEL_VERSIONS[capability]
    ]
    return (not missing, missing)


def check_phase_test_suite_importable() -> tuple[bool, list[str]]:
    """Import every Phase 0–12 scalar-FP v2 unit-test module."""
    failures: list[str] = []
    for module_name in SCALAR_FP_V2_PHASE_TEST_MODULES:
        try:
            importlib.import_module(module_name)
        except Exception as exc:  # pragma: no cover - surfaced in details
            failures.append(f"{module_name}: {exc}")
    return (not failures, failures)


def check_unsupported_query_helper_present() -> tuple[bool, list[str]]:
    """Ensure the Phase 7 unsupported remainder query helpers are wired."""
    try:
        from tools.ppc_equivalence.fp_exact_symbolic import (
            scalar_fp_unsupported_query,
            scalar_fp_unsupported_query_sha256,
        )
    except ImportError as exc:
        return (False, [str(exc)])
    reasons: list[str] = []
    if not callable(scalar_fp_unsupported_query):
        reasons.append("scalar_fp_unsupported_query is not callable")
    if not callable(scalar_fp_unsupported_query_sha256):
        reasons.append("scalar_fp_unsupported_query_sha256 is not callable")
    return (not reasons, reasons)


def check_fe0_fe1_status_when_flag_on() -> tuple[bool, dict[str, bool | str], list[str]]:
    """Probe ``fe0_fe1_modeling_status()`` with ``SCALAR_FP_EXACT_V2`` forced on."""
    from tools.ppc_equivalence.fp_traps import fe0_fe1_modeling_status

    set_scalar_fp_exact_v2_module_flag(True)
    try:
        status = fe0_fe1_modeling_status()
    finally:
        set_scalar_fp_exact_v2_module_flag(None)

    reasons: list[str] = []
    if not status.get("fe0"):
        reasons.append("fe0 ledger bit false with exact-v2 flag on")
    if not status.get("fe1"):
        reasons.append("fe1 ledger bit false with exact-v2 flag on")
    if not status.get("imprecise_modes_modeled"):
        reasons.append("imprecise_modes_modeled false with exact-v2 flag on")
    if status.get("delivery_class") != "fe0-fe1-v2":
        reasons.append(
            f"delivery_class={status.get('delivery_class')!r}, expected fe0-fe1-v2"
        )
    return (not reasons, status, reasons)


def check_corpus_green() -> tuple[bool, str, tuple[str, ...]]:
    """Run the Phase 11 corpus replay grader (``--check``)."""
    from tools.ppc_equivalence.scalar_fp_v2_corpus import check_corpora

    report = check_corpora()
    summary = (
        f"rows={report.total_rows} failed={report.failed_rows} "
        f"sha256={report.corpus_sha256[:16]}..."
    )
    details: list[str] = list(report.schema_errors)
    for item in report.row_results:
        if not item.passed:
            details.append(f"{item.corpus_file}:{item.row_id}: {item.reason}")
    return (report.passed, summary, tuple(details))


def readiness_report(
    *,
    manifest_path: Path | None = None,
    targets_file: Path | None = None,
    run_corpus_check: bool = True,
) -> ReadinessReport:
    """Run Phase 12 infrastructure probes and return an honest readiness report."""
    checks: list[ReadinessCheck] = []

    if run_corpus_check:
        corpus_ok, corpus_summary, corpus_details = check_corpus_green()
        checks.append(
            ReadinessCheck(
                name="corpus_check",
                ok=corpus_ok,
                summary=corpus_summary,
                details=corpus_details[:10],
            )
        )
    else:
        checks.append(
            ReadinessCheck(
                name="corpus_check",
                ok=True,
                summary="skipped (run_corpus_check=False)",
            )
        )

    import_ok, import_failures = check_phase_test_suite_importable()
    checks.append(
        ReadinessCheck(
            name="phase_test_suite",
            ok=import_ok,
            summary=(
                f"{len(SCALAR_FP_V2_PHASE_TEST_MODULES)} modules importable"
                if import_ok
                else f"{len(import_failures)} import failure(s)"
            ),
            details=tuple(import_failures),
        )
    )

    models_ok, models_missing = check_experimental_models_defined()
    checks.append(
        ReadinessCheck(
            name="experimental_models",
            ok=models_ok,
            summary=(
                f"{len(FP_EXPERIMENTAL_SUBCAPABILITY_MODEL_VERSIONS)} experimental models"
                if models_ok
                else f"missing models for {models_missing}"
            ),
            details=tuple(models_missing),
        )
    )

    targets_ok, targets_missing = check_canary_targets_in_registry(
        targets_file=targets_file,
    )
    checks.append(
        ReadinessCheck(
            name="canary_targets",
            ok=targets_ok,
            summary=(
                f"{len(set(SCALAR_FP_V2_CANARY_TARGETS.values()))} unique canary ids present"
                if targets_ok
                else f"missing {len(targets_missing)} id(s)"
            ),
            details=tuple(targets_missing),
        )
    )

    manifest = load_scalar_fp_v2_canary_manifest(manifest_path)
    manifest_ok, manifest_reasons = validate_shadow_manifest(manifest)
    checks.append(
        ReadinessCheck(
            name="shadow_manifest",
            ok=manifest_ok,
            summary="shadow/canary manifest valid" if manifest_ok else "manifest invalid",
            details=tuple(manifest_reasons),
        )
    )

    fe_ok, fe_status, fe_reasons = check_fe0_fe1_status_when_flag_on()
    checks.append(
        ReadinessCheck(
            name="fe0_fe1_status",
            ok=fe_ok,
            summary=(
                f"delivery_class={fe_status.get('delivery_class')}"
                if fe_ok
                else "; ".join(fe_reasons)
            ),
            details=tuple(
                f"{key}={value}" for key, value in sorted(fe_status.items())
            ),
        )
    )

    query_ok, query_reasons = check_unsupported_query_helper_present()
    checks.append(
        ReadinessCheck(
            name="unsupported_query_helper",
            ok=query_ok,
            summary=(
                "scalar_fp_unsupported_query helpers present"
                if query_ok
                else "; ".join(query_reasons)
            ),
            details=tuple(query_reasons),
        )
    )

    infrastructure_ready = all(item.ok for item in checks)
    production_enabled = scalar_fp_exact_v2_enabled() and os.environ.get(
        _SCALAR_FP_V2_PRODUCTION_ENV,
        "0",
    ) in ("1", "true", "True", "yes", "on")
    blockers = list(PRODUCTION_SWITCH_BLOCKERS)
    if not infrastructure_ready:
        failed = [item.name for item in checks if not item.ok]
        blockers.insert(0, f"infrastructure checks failing: {', '.join(failed)}")
    production_switch_ready = False
    return ReadinessReport(
        schema_version=1,
        production_switch_ready=production_switch_ready,
        infrastructure_ready=infrastructure_ready,
        checks=tuple(checks),
        blockers=tuple(blockers),
        production_enabled=production_enabled,
    )


def format_readiness_report(report: ReadinessReport) -> str:
    """Human-readable readiness output for ``--readiness``."""
    lines = [
        "scalar FP exact v2 Phase 12 readiness",
        f"production_switch_ready={report.production_switch_ready}",
        f"infrastructure_ready={report.infrastructure_ready}",
        f"production_enabled={report.production_enabled}",
        "",
        "checks:",
    ]
    for item in report.checks:
        status = "OK" if item.ok else "FAIL"
        lines.append(f"  [{status}] {item.name}: {item.summary}")
        for detail in item.details[:5]:
            lines.append(f"         {detail}")
    lines.extend(["", "blockers (pre-allowlist):"])
    for blocker in report.blockers:
        lines.append(f"  - {blocker}")
    return "\n".join(lines)


def scalar_fp_v2_production_preconditions(
    *,
    manifest: Mapping[str, Any] | None = None,
) -> tuple[bool, list[str]]:
    """Check env + manifest gates without mutating global state."""
    reasons: list[str] = []
    if not scalar_fp_exact_v2_enabled():
        reasons.append(f"{SCALAR_FP_EXACT_V2_ENV} must be enabled")
    if os.environ.get(_SCALAR_FP_V2_PRODUCTION_ENV, "0") not in (
        "1",
        "true",
        "True",
        "yes",
        "on",
    ):
        reasons.append(f"{_SCALAR_FP_V2_PRODUCTION_ENV} must be enabled")

    loaded = dict(manifest) if manifest is not None else load_scalar_fp_v2_canary_manifest()
    manifest_ok, manifest_reasons = validate_shadow_manifest(loaded)
    if not manifest_ok:
        reasons.extend(manifest_reasons)
    allowlists = _manifest_allowlists(loaded)
    if not any(allowlists.values()):
        reasons.append("at least one capability allowlist entry required")
    return (not reasons, reasons)


def enable_scalar_fp_exact_v2_production(
    *,
    manifest_path: Path | None = None,
) -> None:
    """Gated production switch — default never on.

    Requires:
    - ``SCALAR_FP_EXACT_V2=1``
    - ``SCALAR_FP_EXACT_V2_PRODUCTION=1``
    - Shadow/canary manifest with ``automatic_promotion=false`` and a nonempty
      ``allowed_tier_a_capabilities`` entry (override path via
      ``SCALAR_FP_EXACT_V2_ALLOWLIST_MANIFEST``).

    Does **not** bump ``ARCHITECTURE_MODEL`` or alter production manifests.
    """
    manifest_file = manifest_path
    if manifest_file is None:
        override = os.environ.get(_SCALAR_FP_V2_ALLOWLIST_ENV, "").strip()
        if override:
            manifest_file = Path(override)
    manifest = load_scalar_fp_v2_canary_manifest(manifest_file)
    ok, reasons = scalar_fp_v2_production_preconditions(manifest=manifest)
    if not ok:
        raise ScalarFPExactV2ProductionError(
            "scalar FP exact v2 production is gated: " + "; ".join(reasons)
        )
    raise NotImplementedError(
        "scalar FP exact v2 production switch is scaffold-only until Phase 12 "
        "rollout completes (manifest validated; execution path not wired)"
    )


def main(argv: Sequence[str] | None = None) -> int:
    """CLI entry: canary target census or ``--readiness`` report."""
    parser = argparse.ArgumentParser(
        description="Scalar FP exact v2 Phase 12 rollout helpers",
    )
    parser.add_argument(
        "--readiness",
        action="store_true",
        help="Run Phase 12 infrastructure readiness probes",
    )
    parser.add_argument(
        "--json",
        action="store_true",
        help="Emit readiness report as JSON (requires --readiness)",
    )
    parser.add_argument(
        "--skip-corpus",
        action="store_true",
        help="Skip corpus replay during readiness (faster smoke)",
    )
    args = parser.parse_args(list(argv) if argv is not None else None)

    if args.readiness:
        report = readiness_report(run_corpus_check=not args.skip_corpus)
        if args.json:
            print(json.dumps(report.to_dict(), indent=2, sort_keys=True))
        else:
            print(format_readiness_report(report))
        return 0 if report.infrastructure_ready else 1

    for row in list_recommended_canary_targets():
        order = row["allowlist_order"]
        suffix = f" (allowlist #{order})" if order else ""
        print(f"{row['capability']}: {row['target_id']}{suffix}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
