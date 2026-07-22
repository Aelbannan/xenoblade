"""Load/store obligation grading (Phase 6 / 12 — SCALAR_FP_V2.md).

Provides ``evaluate_fp_load_store_obligation_status`` that **can** return
promotion-grade for bit-transform opcodes when dimensions, UNSAT/vacuous
coverage, digests, and allowlist gates pass. Legacy
``fp-load-store-incomplete-v0`` remains always-incomplete via
:mod:`capability_attachment`.
"""

from __future__ import annotations

from typing import Any, Mapping, Sequence

from tools.ppc_equivalence.capability_assurance import (
    STATUS_INCOMPLETE,
    STATUS_PROMOTION_GRADE,
    CapabilityAttestation,
    build_attestation,
)
from tools.ppc_equivalence.fp_capabilities import FP_LOAD_STORE_MODEL_VERSION
from tools.ppc_equivalence.fp_scalar_obligations_v2 import (
    FP_LOAD_STORE_EXACT_V2_ALGORITHM,
    LOAD_STORE_V2_DIMENSIONS,
    build_fp_scalar_obligation_v2,
    obligation_v2_sha256,
    recompute_obligation_digests,
    validate_fp_scalar_obligation_v2,
)

FP_LOAD_STORE_INCOMPLETE_ALGORITHM = "fp-load-store-incomplete-v0"

FP_LOAD_STORE_CAPABILITY = "fp-load-store"

# Base opcodes with exact bit-transform kernels (no indexed/update addressing).
FP_LOAD_STORE_BIT_TRANSFORM_OPS: frozenset[str] = frozenset(
    {"lfs", "lfd", "stfs", "stfd", "stfiwx"}
)

LOAD_STORE_BIT_DIMENSION_KEYS: frozenset[str] = frozenset(
    {
        "bit_expansion_lfs",
        "preserve_lfd",
        "stfs_rounding",
        "stfiwx_raw",
        "all_rn_modes_stfs",
    }
)

LOAD_STORE_DEFERRED_DIMENSION_KEYS: frozenset[str] = frozenset(
    {"bounded_memory", "symbolic_backend", "traps"}
)

LOAD_STORE_BIT_V2_DIMENSIONS: dict[str, bool] = {
    **{key: True for key in LOAD_STORE_BIT_DIMENSION_KEYS},
    **{key: False for key in LOAD_STORE_DEFERRED_DIMENSION_KEYS},
}


def _required_bit_dimensions(opcodes: Sequence[str]) -> frozenset[str]:
    """Bit dimensions required for the attested opcode subset."""
    required: set[str] = set()
    for op in opcodes:
        if op == "lfs":
            required.add("bit_expansion_lfs")
        elif op in ("lfd", "stfd"):
            required.add("preserve_lfd")
        elif op == "stfs":
            required.update({"stfs_rounding", "all_rn_modes_stfs"})
        elif op == "stfiwx":
            required.add("stfiwx_raw")
    return frozenset(required)


def _bit_transform_opcodes_only(opcodes: Sequence[str]) -> bool:
    return bool(opcodes) and all(
        str(op) in FP_LOAD_STORE_BIT_TRANSFORM_OPS for op in opcodes
    )


def _remainder_acceptable(
    remainder: Mapping[str, Any],
    opcodes: Sequence[str],
) -> bool:
    result = remainder.get("result")
    if result == "unsat":
        return True
    if result == "vacuous":
        return list(opcodes) == ["stfs"]
    return False


def _dimensions_satisfied(opcodes: Sequence[str], dimensions: Mapping[str, bool]) -> bool:
    if not _bit_transform_opcodes_only(opcodes):
        return False
    required = _required_bit_dimensions(opcodes)
    if not required.issubset(set(dimensions.keys())):
        return False
    return all(bool(dimensions[key]) for key in required)


def build_fp_load_store_obligation_v2(
    *,
    opcodes: Sequence[str] = (),
    unsupported_remainder_result: str = "incomplete",
    unsupported_remainder_query_sha256: str = "",
    corpus_sha256: str = "",
    validation_ledger_hash: str = "",
    dimensions: Mapping[str, bool] | None = None,
    status: str = STATUS_INCOMPLETE,
) -> dict[str, Any]:
    """Build a schema-v2 load/store obligation."""
    return build_fp_scalar_obligation_v2(
        FP_LOAD_STORE_CAPABILITY,
        opcodes=opcodes,
        dimensions=dimensions,
        unsupported_remainder_result=unsupported_remainder_result,
        unsupported_remainder_query_sha256=unsupported_remainder_query_sha256,
        corpus_sha256=corpus_sha256,
        validation_ledger_hash=validation_ledger_hash,
        status=status,
        notes=(
            "Phase 6/12 load/store exact bit transforms; promotion requires "
            "bit-transform opcodes + dimensions + UNSAT/vacuous + allowlist + digests."
        ),
    )


def evaluate_fp_load_store_obligation_status(
    obligation: Mapping[str, Any] | None,
    *,
    host_float: bool | None = None,
    live_corpus_sha256: str = "",
    live_validation_ledger_hash: str = "",
    allowlist_contains_model: bool = False,
) -> str:
    """Production-style load/store grader (schema v2 only).

    Legacy v0 obligations and ``fp-load-store-incomplete-v0`` must use
    :func:`recompute_fp_load_store_attestation_status` with the incomplete algorithm.
    """
    if host_float is True:
        return STATUS_INCOMPLETE
    if obligation is None:
        return STATUS_INCOMPLETE
    schema = obligation.get("schema_version")
    if schema != 2:
        return STATUS_INCOMPLETE
    if obligation.get("algorithm") != FP_LOAD_STORE_EXACT_V2_ALGORITHM:
        return STATUS_INCOMPLETE

    error = validate_fp_scalar_obligation_v2(obligation)
    if error is not None:
        return STATUS_INCOMPLETE

    ok, _reasons = recompute_obligation_digests(
        obligation,
        live_corpus_sha256=live_corpus_sha256,
        live_validation_ledger_hash=live_validation_ledger_hash,
    )
    if not ok:
        return STATUS_INCOMPLETE

    opcodes = [str(op) for op in obligation.get("opcodes") or []]
    if not _bit_transform_opcodes_only(opcodes):
        return STATUS_INCOMPLETE

    dims = obligation.get("dimensions") or {}
    if not _dimensions_satisfied(opcodes, dims):
        return STATUS_INCOMPLETE

    remainder = (obligation.get("coverage") or {}).get("unsupported_remainder") or {}
    if not _remainder_acceptable(remainder, opcodes):
        return STATUS_INCOMPLETE

    if not allowlist_contains_model:
        return STATUS_INCOMPLETE

    return STATUS_PROMOTION_GRADE


def build_fp_load_store_attestation_v2(
    *,
    opcodes: Sequence[str] = (),
    host_float: bool = False,
    dimensions: Mapping[str, bool] | None = None,
    unsupported_remainder_result: str = "incomplete",
    unsupported_remainder_query_sha256: str = "",
    corpus_sha256: str = "",
    validation_ledger_hash: str = "",
) -> CapabilityAttestation:
    """Draft a Phase 6/12 load/store attestation (status recomputed from obligation)."""
    obl = build_fp_load_store_obligation_v2(
        opcodes=opcodes,
        dimensions=dimensions,
        unsupported_remainder_result=unsupported_remainder_result,
        unsupported_remainder_query_sha256=unsupported_remainder_query_sha256,
        corpus_sha256=corpus_sha256,
        validation_ledger_hash=validation_ledger_hash,
    )
    error = validate_fp_scalar_obligation_v2(obl)
    evidence: dict[str, Any] = {
        "obligation": obl,
        "obligation_sha256": obligation_v2_sha256(obl),
        "opcodes": list(opcodes),
        "host_float": bool(host_float),
        "dimensions": dict(obl.get("dimensions") or {}),
    }
    if error is not None:
        evidence["schema_error"] = error
    status = evaluate_fp_load_store_obligation_status(
        obl,
        host_float=host_float,
        live_corpus_sha256=corpus_sha256,
        live_validation_ledger_hash=validation_ledger_hash,
    )
    return build_attestation(
        capability=FP_LOAD_STORE_CAPABILITY,
        model_version=FP_LOAD_STORE_MODEL_VERSION,
        algorithm=FP_LOAD_STORE_EXACT_V2_ALGORITHM,
        status=status,
        unsupported=("schema-invalid",) if error else (),
        evidence=evidence,
    )


def recompute_fp_load_store_attestation_status(
    evidence: Mapping[str, Any],
    *,
    capability: str,
    algorithm: str,
    model_version: str = "",
    unsupported: Sequence[str] = (),
    allowed_versions: Sequence[str] = (),
    live_corpus_sha256: str = "",
    live_validation_ledger_hash: str = "",
) -> str:
    """Recompute load/store grade; exact-v2 can promote when allowlisted + complete."""
    if capability != FP_LOAD_STORE_CAPABILITY:
        return STATUS_INCOMPLETE
    if unsupported:
        return STATUS_INCOMPLETE
    if evidence.get("host_float") is True:
        return STATUS_INCOMPLETE

    if algorithm == FP_LOAD_STORE_INCOMPLETE_ALGORITHM:
        return STATUS_INCOMPLETE

    if algorithm != FP_LOAD_STORE_EXACT_V2_ALGORITHM:
        return STATUS_INCOMPLETE
    if model_version and model_version != FP_LOAD_STORE_MODEL_VERSION:
        return STATUS_INCOMPLETE
    allowlisted = bool(allowed_versions) and FP_LOAD_STORE_MODEL_VERSION in allowed_versions
    obligation = evidence.get("obligation")
    return evaluate_fp_load_store_obligation_status(
        obligation if isinstance(obligation, dict) else None,
        host_float=evidence.get("host_float"),
        live_corpus_sha256=live_corpus_sha256,
        live_validation_ledger_hash=live_validation_ledger_hash,
        allowlist_contains_model=allowlisted,
    )


__all__ = [
    "FP_LOAD_STORE_BIT_TRANSFORM_OPS",
    "FP_LOAD_STORE_CAPABILITY",
    "FP_LOAD_STORE_INCOMPLETE_ALGORITHM",
    "LOAD_STORE_BIT_V2_DIMENSIONS",
    "LOAD_STORE_V2_DIMENSIONS",
    "build_fp_load_store_attestation_v2",
    "build_fp_load_store_obligation_v2",
    "evaluate_fp_load_store_obligation_status",
    "recompute_fp_load_store_attestation_status",
]
