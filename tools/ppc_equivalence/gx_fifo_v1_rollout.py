"""Wave 5 rollout plumbing for GX FIFO Tier-A v1 (GX_FIFO_TIER_A.md).

Pattern mirrors ``scalar_fp_v2_rollout.py``: honest readiness probes plus a
gated, ordered allowlist-expansion staging helper. This module never mutates
the default (``capability_manifest.json``) or authoritative
(``capability_manifest.authoritative.json``) manifests — those keep every
gx-fifo / mmio-loop-emission allowlist entry empty until an operator
deliberately points ``coop.json`` at one of the dedicated canary manifests
below and reruns bottom-up recertification. ``automatic_promotion`` is always
``false`` for every canary stage.

Rollout order (``GX_FIFO_TIER_A.md`` "Rollout order"):

1. ``land`` — profile, corpus, models, obligations, and tests land with every
   allowlist empty (``capability_manifest.gx_fifo_v1_canary.json``).
2. ``gx_fifo_write_trace_v2`` — allowlist ``gx-fifo-write-trace`` →
   ``gx-fifo-trace-v2`` only.
3. ``gx_fifo_read_v1`` — additionally allowlist ``gx-fifo-read`` →
   ``gx-fifo-read-v1`` (unsupported-read *policy* attestation; never a
   FIFO-load value model).
4. ``gx_fifo_loop_emission_v1`` — additionally allowlist
   ``mmio-loop-emission`` → ``mmio-loop-emission-v1``.

Each stage after ``land`` has its own dedicated canary manifest file so the
staged allowlist state is a reviewable, static artifact rather than a runtime
mutation of a single shared file.
"""

from __future__ import annotations

import argparse
import importlib
import json
from dataclasses import dataclass
from pathlib import Path
from typing import Any, Mapping

from tools.ppc_equivalence.mmio_capability_obligations import (
    GX_FIFO_READ_MODEL_VERSION,
    GX_FIFO_TRACE_MODEL_VERSION,
    MMIO_LOOP_EMISSION_MODEL_VERSION,
)
from tools.ppc_equivalence.result import ARCHITECTURE_MODEL

_REPO_ROOT = Path(__file__).resolve().parents[2]
_COOP_DIR = _REPO_ROOT / "tools" / "coop"

# Reviewed hardware profile bound to the Tier-A GX FIFO domain (immutable;
# see GX_FIFO_TIER_A.md and hardware_profile.py — do not mutate v1).
GX_FIFO_HARDWARE_PROFILE = "wii-broadway-xenoblade-us-v2"

# Capability -> model-version identity (mirrors mmio_capability_obligations.py).
GX_FIFO_MODEL_VERSIONS: dict[str, str] = {
    "gx-fifo-write-trace": GX_FIFO_TRACE_MODEL_VERSION,
    "gx-fifo-read": GX_FIFO_READ_MODEL_VERSION,
    "mmio-loop-emission": MMIO_LOOP_EMISSION_MODEL_VERSION,
}

# Capability -> recommended canary target id (tools/coop/targets.json) used to
# actually discharge the capability's obligation under GX_FIFO_HARDWARE_PROFILE
# before an operator expands that capability's allowlist. Advisory metadata
# only (surfaced under ``gx_fifo_exact_v1.recommended_canary_targets`` in every
# staged manifest) — it never gates readiness or allowlist validation beyond
# the shape/consistency checks in ``validate_gx_fifo_manifest``.
GX_FIFO_V1_RECOMMENDED_CANARY_TARGETS: dict[str, str] = {
    # us-8031bfc0 == GXInvalidateVtxCache: leaf FULL_MATCH, single `stb` to
    # WGPIPE (0xCC008000) via `lis r3,0xcc01; stb r0,-0x8000(r3)`.
    "gx-fifo-write-trace": "us-8031bfc0",
}

# Authoritative allowlist order (Wave 5 — GX_FIFO_TIER_A.md "Rollout order").
GX_FIFO_V1_ALLOWLIST_ORDER: tuple[str, ...] = (
    "gx-fifo-write-trace",
    "gx-fifo-read",
    "mmio-loop-emission",
)

# validation_ledger.yaml capability status values that mean "obligations,
# validators, and tests are complete; only the allowlist is pending".
READY_LEDGER_STATUSES: frozenset[str] = frozenset(
    {"ready_for_allowlist", "implemented_pending_allowlist"}
)

# Shadow-default manifest whose gx-fifo / mmio-loop-emission allowlists must
# stay empty (default policy never auto-graduates). The authoritative
# manifest may carry graduated GX allowlists after an explicit canary
# (see capability_manifest.authoritative.json + GX_FIFO_TIER_A.md).
DEFAULT_POLICY_MANIFESTS: tuple[Path, ...] = (
    _COOP_DIR / "capability_manifest.json",
)
AUTHORITATIVE_MANIFEST = _COOP_DIR / "capability_manifest.authoritative.json"
# apply-canary must never overwrite these production policy files.
PROTECTED_POLICY_MANIFESTS: tuple[Path, ...] = (
    _COOP_DIR / "capability_manifest.json",
    AUTHORITATIVE_MANIFEST,
)

# Wave 5 GX FIFO unit-test modules the readiness gate must be able to import.
GX_FIFO_V1_TEST_MODULES: tuple[str, ...] = (
    "tools.ppc_equivalence.tests.test_gx_fifo_semantics",
    "tools.ppc_equivalence.tests.test_gx_fifo_device_v2",
    "tools.ppc_equivalence.tests.test_gx_fifo_loop",
    "tools.ppc_equivalence.tests.test_gx_fifo_loop_discharge",
    "tools.ppc_equivalence.tests.test_mmio_capability_assurance",
    "tools.ppc_equivalence.tests.test_gx_fifo_corpus",
    "tools.ppc_equivalence.tests.test_gx_fifo_read",
    "tools.ppc_equivalence.tests.test_gx_fifo_mutation_soundness",
    "tools.ppc_equivalence.tests.test_gx_fifo_write_trace_canary",
)


@dataclass(frozen=True, slots=True)
class RolloutStage:
    """One ordered allowlist-expansion stage of the GX FIFO Tier-A rollout."""

    name: str
    capability: str | None
    model_version: str | None
    manifest_path: Path
    cumulative_capabilities: tuple[str, ...]
    description: str


GX_FIFO_V1_STAGES: tuple[RolloutStage, ...] = (
    RolloutStage(
        name="land",
        capability=None,
        model_version=None,
        manifest_path=_COOP_DIR / "capability_manifest.gx_fifo_v1_canary.json",
        cumulative_capabilities=(),
        description=(
            "Profile, corpus, models, obligations, and tests land with every "
            "gx-fifo / mmio-loop-emission allowlist empty."
        ),
    ),
    RolloutStage(
        name="gx_fifo_write_trace_v2",
        capability="gx-fifo-write-trace",
        model_version=GX_FIFO_TRACE_MODEL_VERSION,
        manifest_path=_COOP_DIR / "capability_manifest.gx_fifo_write_trace_v2_canary.json",
        cumulative_capabilities=("gx-fifo-write-trace",),
        description="Allowlist ordinary FIFO write-trace obligations only.",
    ),
    RolloutStage(
        name="gx_fifo_read_v1",
        capability="gx-fifo-read",
        model_version=GX_FIFO_READ_MODEL_VERSION,
        manifest_path=_COOP_DIR / "capability_manifest.gx_fifo_read_v1_canary.json",
        cumulative_capabilities=("gx-fifo-write-trace", "gx-fifo-read"),
        description=(
            "Additionally allowlist the gx-fifo-read-v1 unsupported-read "
            "policy attestation (outcome 3; never a value model)."
        ),
    ),
    RolloutStage(
        name="gx_fifo_loop_emission_v1",
        capability="mmio-loop-emission",
        model_version=MMIO_LOOP_EMISSION_MODEL_VERSION,
        manifest_path=_COOP_DIR / "capability_manifest.gx_fifo_loop_emission_v1_canary.json",
        cumulative_capabilities=(
            "gx-fifo-write-trace",
            "gx-fifo-read",
            "mmio-loop-emission",
        ),
        description=(
            "Additionally allowlist mmio-loop-emission-v1 (authorization "
            "attachment; recognition never authorizes equivalence on its own)."
        ),
    ),
)

_STAGES_BY_NAME: dict[str, RolloutStage] = {stage.name: stage for stage in GX_FIFO_V1_STAGES}


def ordered_stage_names() -> tuple[str, ...]:
    return tuple(stage.name for stage in GX_FIFO_V1_STAGES)


def stage_by_name(name: str) -> RolloutStage:
    try:
        return _STAGES_BY_NAME[name]
    except KeyError as exc:
        raise KeyError(
            f"unknown GX FIFO v1 rollout stage {name!r}; known stages: "
            f"{', '.join(ordered_stage_names())}"
        ) from exc


def load_canary_manifest(path: Path | None = None) -> dict[str, Any]:
    """Load a GX FIFO v1 canary manifest (defaults to the ``land`` stage)."""
    manifest_path = path or GX_FIFO_V1_STAGES[0].manifest_path
    with manifest_path.open(encoding="utf-8") as handle:
        return json.load(handle)


def _manifest_allowlists(manifest: Mapping[str, Any]) -> dict[str, list[str]]:
    raw = manifest.get("allowed_tier_a_capabilities") or {}
    if not isinstance(raw, Mapping):
        return {}
    return {str(key): list(value or []) for key, value in raw.items()}


def validate_gx_fifo_manifest(manifest: Mapping[str, Any]) -> tuple[bool, list[str]]:
    """Validate the shared shape of a GX FIFO v1 rollout manifest."""
    reasons: list[str] = []
    if manifest.get("automatic_promotion") is not False:
        reasons.append("automatic_promotion must be exactly false")
    shadow_mode = manifest.get("shadow_mode")
    if not isinstance(shadow_mode, bool):
        reasons.append("shadow_mode must be a boolean")
    allowlists = _manifest_allowlists(manifest)
    if not allowlists:
        reasons.append("allowed_tier_a_capabilities must be a mapping")
    for capability in GX_FIFO_V1_ALLOWLIST_ORDER:
        if capability not in allowlists:
            reasons.append(f"missing allowlist key {capability!r}")
        elif not isinstance(allowlists[capability], list):
            reasons.append(f"allowlist for {capability!r} must be a list")
    meta = manifest.get("gx_fifo_exact_v1")
    if not isinstance(meta, Mapping):
        reasons.append("gx_fifo_exact_v1 metadata section required")
    else:
        if meta.get("hardware_profile") != GX_FIFO_HARDWARE_PROFILE:
            reasons.append(
                f"gx_fifo_exact_v1.hardware_profile must be {GX_FIFO_HARDWARE_PROFILE!r}"
            )
        order = meta.get("allowlist_order")
        if list(order or []) != list(GX_FIFO_V1_ALLOWLIST_ORDER):
            reasons.append("gx_fifo_exact_v1.allowlist_order mismatch")
        model_versions = meta.get("model_versions")
        if dict(model_versions or {}) != GX_FIFO_MODEL_VERSIONS:
            reasons.append("gx_fifo_exact_v1.model_versions mismatch")
        recommended_targets = meta.get("recommended_canary_targets")
        if dict(recommended_targets or {}) != GX_FIFO_V1_RECOMMENDED_CANARY_TARGETS:
            reasons.append("gx_fifo_exact_v1.recommended_canary_targets mismatch")
    return (not reasons, reasons)


def validate_stage_manifest(
    stage: RolloutStage,
    manifest: Mapping[str, Any] | None = None,
) -> tuple[bool, list[str]]:
    """Validate ``stage``'s manifest shape *and* its cumulative allowlist state."""
    loaded = manifest if manifest is not None else load_canary_manifest(stage.manifest_path)
    ok, reasons = validate_gx_fifo_manifest(loaded)
    reasons = list(reasons)
    allowlists = _manifest_allowlists(loaded)
    for capability in GX_FIFO_V1_ALLOWLIST_ORDER:
        expected = (
            [GX_FIFO_MODEL_VERSIONS[capability]]
            if capability in stage.cumulative_capabilities
            else []
        )
        actual = allowlists.get(capability, [])
        if actual != expected:
            reasons.append(
                f"stage {stage.name!r}: allowlist[{capability!r}]={actual!r}, "
                f"expected {expected!r}"
            )
    meta = loaded.get("gx_fifo_exact_v1")
    if isinstance(meta, Mapping) and meta.get("stage") != stage.name:
        reasons.append(
            f"gx_fifo_exact_v1.stage={meta.get('stage')!r}, expected {stage.name!r}"
        )
    return (not reasons, reasons)


def check_all_stage_manifests() -> tuple[bool, dict[str, list[str]]]:
    """Validate every staged canary manifest file on disk."""
    failures: dict[str, list[str]] = {}
    for stage in GX_FIFO_V1_STAGES:
        if not stage.manifest_path.is_file():
            failures[stage.name] = [f"missing manifest file {stage.manifest_path}"]
            continue
        ok, reasons = validate_stage_manifest(stage)
        if not ok:
            failures[stage.name] = reasons
    return (not failures, failures)


def check_default_manifests_still_empty(
    paths: tuple[Path, ...] = DEFAULT_POLICY_MANIFESTS,
) -> tuple[bool, list[str]]:
    """Ensure the shadow-default manifest keeps every Tier-A gx allowlist empty."""
    reasons: list[str] = []
    for path in paths:
        if not path.is_file():
            reasons.append(f"missing manifest file {path}")
            continue
        with path.open(encoding="utf-8") as handle:
            manifest = json.load(handle)
        allowlists = _manifest_allowlists(manifest)
        for capability in GX_FIFO_V1_ALLOWLIST_ORDER:
            value = allowlists.get(capability)
            if value:
                reasons.append(f"{path.name}: allowlist[{capability!r}]={value!r} is not empty")
    return (not reasons, reasons)


def check_authoritative_gx_graduation(
    path: Path = AUTHORITATIVE_MANIFEST,
) -> tuple[bool, list[str]]:
    """Confirm authoritative carries the full graduated GX Tier-A allowlists."""
    reasons: list[str] = []
    if not path.is_file():
        return False, [f"missing authoritative manifest {path}"]
    with path.open(encoding="utf-8") as handle:
        manifest = json.load(handle)
    allowlists = _manifest_allowlists(manifest)
    for capability, model_version in GX_FIFO_MODEL_VERSIONS.items():
        value = allowlists.get(capability) or []
        if list(value) != [model_version]:
            reasons.append(
                f"{path.name}: allowlist[{capability!r}]={value!r}, "
                f"expected {[model_version]!r}"
            )
    auto = manifest.get("automatic_promotion")
    if auto is True:
        reasons.append(f"{path.name}: automatic_promotion must not be true")
    return (not reasons, reasons)


def check_validation_ledger_ready() -> tuple[bool, dict[str, str], list[str]]:
    """Confirm ``validation_ledger.yaml`` marks every capability allowlist-ready."""
    from tools.coop.lib.equivalence_policy import ValidationLedger, default_validation_ledger_path

    ledger = ValidationLedger.load(default_validation_ledger_path())
    reasons: list[str] = []
    statuses: dict[str, str] = {}
    if ledger.architecture_model is not None and ledger.architecture_model != ARCHITECTURE_MODEL:
        reasons.append(
            f"validation_ledger.yaml architecture_model={ledger.architecture_model!r} "
            f"!= live {ARCHITECTURE_MODEL!r}"
        )
    for capability in GX_FIFO_V1_ALLOWLIST_ORDER:
        entry = ledger.capabilities.get(capability) or {}
        status = str(entry.get("status", ""))
        statuses[capability] = status
        if status not in READY_LEDGER_STATUSES:
            reasons.append(
                f"{capability}: validation_ledger status={status!r}, "
                f"expected one of {sorted(READY_LEDGER_STATUSES)}"
            )
        model_version = entry.get("model_version")
        if model_version != GX_FIFO_MODEL_VERSIONS[capability]:
            reasons.append(
                f"{capability}: validation_ledger model_version={model_version!r}, "
                f"expected {GX_FIFO_MODEL_VERSIONS[capability]!r}"
            )
    return (not reasons, statuses, reasons)


def check_hardware_profile_reviewed() -> tuple[bool, list[str]]:
    """Confirm the Tier-A hardware profile is a reviewed, hash-bound artifact."""
    from tools.ppc_equivalence.hardware_profile import (
        is_reviewed_hardware_profile,
        load_hardware_profile,
    )

    reasons: list[str] = []
    try:
        profile = load_hardware_profile(GX_FIFO_HARDWARE_PROFILE)
    except (OSError, ValueError) as exc:
        return (False, [str(exc)])
    if not is_reviewed_hardware_profile(profile):
        reasons.append(f"{GX_FIFO_HARDWARE_PROFILE} is not a reviewed hardware profile")
    return (not reasons, reasons)


def check_gx_fifo_test_modules_importable() -> tuple[bool, list[str]]:
    """Import every Wave 5 GX FIFO Tier-A unit-test module."""
    failures: list[str] = []
    for module_name in GX_FIFO_V1_TEST_MODULES:
        try:
            importlib.import_module(module_name)
        except Exception as exc:  # pragma: no cover - surfaced in details
            failures.append(f"{module_name}: {exc}")
    return (not failures, failures)


@dataclass(frozen=True, slots=True)
class ReadinessCheck:
    name: str
    ok: bool
    summary: str
    details: tuple[str, ...] = ()


@dataclass(frozen=True, slots=True)
class ReadinessReport:
    schema_version: int
    infrastructure_ready: bool
    checks: tuple[ReadinessCheck, ...]
    blockers: tuple[str, ...]

    def to_dict(self) -> dict[str, Any]:
        return {
            "schema_version": self.schema_version,
            "infrastructure_ready": self.infrastructure_ready,
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


# Residual promotion blockers even once every readiness probe is green.
# Wiring is mechanical; the allowlist decision (per GX_FIFO_TIER_A.md
# "Rollout order") is a deliberate operator action, never automatic.
ROLLOUT_BLOCKERS: tuple[str, ...] = (
    "automatic_promotion stays false; further MMIO allowlist expansion still "
    "requires a dedicated canary + bottom-up recertification before changing "
    "default (shadow) policy",
)


def readiness_report() -> ReadinessReport:
    """Run Wave 5 GX FIFO Tier-A infrastructure probes; never mutates state."""
    checks: list[ReadinessCheck] = []

    import_ok, import_failures = check_gx_fifo_test_modules_importable()
    checks.append(
        ReadinessCheck(
            name="gx_fifo_test_modules",
            ok=import_ok,
            summary=(
                f"{len(GX_FIFO_V1_TEST_MODULES)} modules importable"
                if import_ok
                else f"{len(import_failures)} import failure(s)"
            ),
            details=tuple(import_failures),
        )
    )

    ledger_ok, ledger_statuses, ledger_reasons = check_validation_ledger_ready()
    checks.append(
        ReadinessCheck(
            name="validation_ledger",
            ok=ledger_ok,
            summary=(
                "every capability ready_for_allowlist / implemented_pending_allowlist"
                if ledger_ok
                else "; ".join(ledger_reasons)
            ),
            details=tuple(f"{cap}={status}" for cap, status in sorted(ledger_statuses.items())),
        )
    )

    profile_ok, profile_reasons = check_hardware_profile_reviewed()
    checks.append(
        ReadinessCheck(
            name="hardware_profile",
            ok=profile_ok,
            summary=(
                f"{GX_FIFO_HARDWARE_PROFILE} reviewed and hash-bound"
                if profile_ok
                else "; ".join(profile_reasons)
            ),
            details=tuple(profile_reasons),
        )
    )

    stage_ok, stage_failures = check_all_stage_manifests()
    checks.append(
        ReadinessCheck(
            name="stage_manifests",
            ok=stage_ok,
            summary=(
                f"{len(GX_FIFO_V1_STAGES)} staged canary manifests valid"
                if stage_ok
                else f"{len(stage_failures)} stage manifest failure(s)"
            ),
            details=tuple(
                f"{stage}: {'; '.join(reasons)}" for stage, reasons in stage_failures.items()
            ),
        )
    )

    default_ok, default_reasons = check_default_manifests_still_empty()
    checks.append(
        ReadinessCheck(
            name="default_manifests_unchanged",
            ok=default_ok,
            summary=(
                "shadow-default capability_manifest.json keeps gx allowlists empty"
                if default_ok
                else "; ".join(default_reasons)
            ),
            details=tuple(default_reasons),
        )
    )

    auth_ok, auth_reasons = check_authoritative_gx_graduation()
    checks.append(
        ReadinessCheck(
            name="authoritative_gx_graduated",
            ok=auth_ok,
            summary=(
                "authoritative allowlists gx-fifo-trace-v2 + gx-fifo-read-v1 + "
                "mmio-loop-emission-v1"
                if auth_ok
                else "; ".join(auth_reasons)
            ),
            details=tuple(auth_reasons),
        )
    )

    infrastructure_ready = all(item.ok for item in checks)
    blockers = list(ROLLOUT_BLOCKERS)
    if not infrastructure_ready:
        failed = [item.name for item in checks if not item.ok]
        blockers.insert(0, f"infrastructure checks failing: {', '.join(failed)}")
    return ReadinessReport(
        schema_version=1,
        infrastructure_ready=infrastructure_ready,
        checks=tuple(checks),
        blockers=tuple(blockers),
    )


def format_readiness_report(report: ReadinessReport) -> str:
    lines = [
        "GX FIFO Tier-A v1 rollout readiness",
        f"infrastructure_ready={report.infrastructure_ready}",
        "",
        "checks:",
    ]
    for item in report.checks:
        status = "OK" if item.ok else "FAIL"
        lines.append(f"  [{status}] {item.name}: {item.summary}")
        for detail in item.details[:5]:
            lines.append(f"         {detail}")
    lines.extend(["", "blockers:"])
    for blocker in report.blockers:
        lines.append(f"  - {blocker}")
    return "\n".join(lines)


def build_stage_manifest(stage: RolloutStage) -> dict[str, Any]:
    """Regenerate ``stage``'s canary manifest content from the ``land`` base.

    Deterministic helper for ``--apply-canary``: recomputes the manifest a
    stage *should* contain so drift from the checked-in stage file is
    detectable and easy to re-materialize.
    """
    base = load_canary_manifest(GX_FIFO_V1_STAGES[0].manifest_path)
    manifest = dict(base)
    allowlists = dict(base.get("allowed_tier_a_capabilities") or {})
    for capability in GX_FIFO_V1_ALLOWLIST_ORDER:
        allowlists[capability] = (
            [GX_FIFO_MODEL_VERSIONS[capability]]
            if capability in stage.cumulative_capabilities
            else []
        )
    manifest["allowed_tier_a_capabilities"] = allowlists
    manifest["automatic_promotion"] = False
    manifest["shadow_mode"] = stage.name == "land"
    manifest["require_capability_assurance"] = stage.name != "land"
    manifest["description"] = (
        f"Wave 5 GX FIFO Tier-A v1 rollout (GX_FIFO_TIER_A.md) — stage "
        f"{stage.name!r}. {stage.description}"
    )
    meta = dict(manifest.get("gx_fifo_exact_v1") or {})
    meta["stage"] = stage.name
    meta.setdefault("hardware_profile", GX_FIFO_HARDWARE_PROFILE)
    meta.setdefault("allowlist_order", list(GX_FIFO_V1_ALLOWLIST_ORDER))
    meta.setdefault("model_versions", dict(GX_FIFO_MODEL_VERSIONS))
    meta.setdefault("recommended_canary_targets", dict(GX_FIFO_V1_RECOMMENDED_CANARY_TARGETS))
    manifest["gx_fifo_exact_v1"] = meta
    return manifest


def apply_canary_stage(name: str, *, out_path: Path | None = None) -> Path:
    """Materialize ``name``'s canary manifest to disk (defaults to its own path).

    Never targets ``capability_manifest.json`` or
    ``capability_manifest.authoritative.json`` — those stay outside this
    helper's write surface so default policy can never change here.
    """
    stage = stage_by_name(name)
    destination = out_path or stage.manifest_path
    if destination in PROTECTED_POLICY_MANIFESTS:
        raise ValueError(
            f"refusing to write the protected policy manifest {destination}"
        )
    manifest = build_stage_manifest(stage)
    destination.write_text(
        json.dumps(manifest, indent=2, ensure_ascii=False) + "\n", encoding="utf-8"
    )
    return destination


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(
        description="GX FIFO Tier-A v1 rollout helpers (GX_FIFO_TIER_A.md)",
    )
    sub = parser.add_subparsers(dest="command")

    readiness_parser = sub.add_parser("readiness", help="Run readiness probes")
    readiness_parser.add_argument("--json", action="store_true")

    stage_parser = sub.add_parser("stage", help="Describe one rollout stage")
    stage_parser.add_argument("name", choices=ordered_stage_names())
    stage_parser.add_argument("--json", action="store_true")

    apply_parser = sub.add_parser(
        "apply-canary", help="(Re)materialize a stage's canary manifest file"
    )
    apply_parser.add_argument("name", choices=ordered_stage_names())
    apply_parser.add_argument("--out", type=Path, default=None)

    sub.add_parser("list-stages", help="List rollout stages in order")

    args = parser.parse_args(argv)

    if args.command == "readiness":
        report = readiness_report()
        if args.json:
            print(json.dumps(report.to_dict(), indent=2, sort_keys=True))
        else:
            print(format_readiness_report(report))
        return 0 if report.infrastructure_ready else 1

    if args.command == "stage":
        stage = stage_by_name(args.name)
        ok, reasons = validate_stage_manifest(stage)
        payload = {
            "name": stage.name,
            "capability": stage.capability,
            "model_version": stage.model_version,
            "manifest_path": str(stage.manifest_path),
            "cumulative_capabilities": list(stage.cumulative_capabilities),
            "description": stage.description,
            "manifest_valid": ok,
            "manifest_errors": reasons,
        }
        if args.json:
            print(json.dumps(payload, indent=2, sort_keys=True))
        else:
            for key, value in payload.items():
                print(f"{key}: {value}")
        return 0 if ok else 1

    if args.command == "apply-canary":
        destination = apply_canary_stage(args.name, out_path=args.out)
        print(f"wrote {destination}")
        return 0

    if args.command == "list-stages" or args.command is None:
        for stage in GX_FIFO_V1_STAGES:
            caps = ", ".join(stage.cumulative_capabilities) or "(none)"
            print(f"{stage.name}: {caps}")
        return 0

    parser.print_help()
    return 1


if __name__ == "__main__":
    raise SystemExit(main())
