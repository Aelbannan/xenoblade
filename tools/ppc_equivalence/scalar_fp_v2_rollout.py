"""Phase 12 rollout plumbing for scalar FP exact v2 (SCALAR_FP_V2.md).

Production execution remains default-off. ``enable_scalar_fp_exact_v2_production``
is a gated entry point that requires explicit env **and** a nonempty capability
allowlist in the shadow/canary manifest.
"""

from __future__ import annotations

import json
import os
from pathlib import Path
from typing import Any, Mapping, Sequence

from tools.ppc_equivalence.fp_capabilities import (
    SCALAR_FP_EXACT_V2_ENV,
    scalar_fp_exact_v2_enabled,
)

_SCALAR_FP_V2_PRODUCTION_ENV = "SCALAR_FP_EXACT_V2_PRODUCTION"
_SCALAR_FP_V2_ALLOWLIST_ENV = "SCALAR_FP_EXACT_V2_ALLOWLIST_MANIFEST"

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

_DEFAULT_CANARY_MANIFEST = (
    Path(__file__).resolve().parents[1]
    / "coop"
    / "capability_manifest.scalar_fp_v2_canary.json.example"
)


class ScalarFPExactV2ProductionError(RuntimeError):
    """Raised when production switch preconditions are not met."""


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
    if loaded.get("automatic_promotion") is True:
        reasons.append("automatic_promotion must remain false for canary manifests")
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


def main_canary_targets(argv: Sequence[str] | None = None) -> int:
    """Print recommended canary targets (``python -m ...scalar_fp_v2_rollout``)."""
    del argv
    for row in list_recommended_canary_targets():
        order = row["allowlist_order"]
        suffix = f" (allowlist #{order})" if order else ""
        print(f"{row['capability']}: {row['target_id']}{suffix}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main_canary_targets())
