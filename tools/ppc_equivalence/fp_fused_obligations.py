"""Fused-arithmetic obligation grading (Phase 9 — SCALAR_FP_V2.md).

Provides a production-style grader ``evaluate_fp_fused_obligation_status`` that
**can** return promotion-grade when all dimensions and UNSAT coverage are true.
Legacy ``fp-fused-incomplete-v0`` remains always-incomplete via
:mod:`fp_advanced_obligations`.
"""

from __future__ import annotations

from typing import Any, Mapping, Sequence

from tools.ppc_equivalence.capability_assurance import (
    STATUS_INCOMPLETE,
    STATUS_PROMOTION_GRADE,
    CapabilityAttestation,
    build_attestation,
)
from tools.ppc_equivalence.fp_advanced_obligations import (
    FP_FUSED_ALGORITHM,
    FP_FUSED_CAPABILITY,
    evaluate_fp_advanced_obligation_status,
)
from tools.ppc_equivalence.fp_capabilities import FP_FUSED_MODEL_VERSION_V2
from tools.ppc_equivalence.fp_scalar_obligations_v2 import (
    FP_FUSED_EXACT_V2_ALGORITHM,
    FUSED_V2_DIMENSIONS,
    build_fp_scalar_obligation_v2,
    evaluate_fp_scalar_obligation_v2_status,
    obligation_v2_sha256,
    validate_fp_scalar_obligation_v2,
)

FUSED_V2_COMPLETE_DIMENSIONS: dict[str, bool] = {key: True for key in FUSED_V2_DIMENSIONS}


def build_fp_fused_obligation_v2(
    *,
    opcodes: Sequence[str] = (),
    unsupported_remainder_result: str = "incomplete",
    unsupported_remainder_query_sha256: str = "",
    corpus_sha256: str = "",
    validation_ledger_hash: str = "",
    dimensions: Mapping[str, bool] | None = None,
    status: str = STATUS_INCOMPLETE,
) -> dict[str, Any]:
    """Build a schema-v2 fused obligation."""
    return build_fp_scalar_obligation_v2(
        FP_FUSED_CAPABILITY,
        opcodes=opcodes,
        dimensions=dimensions,
        unsupported_remainder_result=unsupported_remainder_result,
        unsupported_remainder_query_sha256=unsupported_remainder_query_sha256,
        corpus_sha256=corpus_sha256,
        validation_ledger_hash=validation_ledger_hash,
        status=status,
        notes=(
            "Phase 9 fused exact kernel; promotion requires dimensions + UNSAT + "
            "allowlist + corpus/ledger digests."
        ),
    )


def evaluate_fp_fused_obligation_status(
    obligation: Mapping[str, Any] | None,
    *,
    host_float: bool | None = None,
    live_corpus_sha256: str = "",
    live_validation_ledger_hash: str = "",
    allowlist_contains_model: bool = False,
) -> str:
    """Production-style fused grader (schema v2 only).

    Legacy v1 obligations and ``fp-fused-incomplete-v0`` attestations must use
    :func:`evaluate_fp_advanced_obligation_status` instead.
    """
    if obligation is None:
        return STATUS_INCOMPLETE
    schema = obligation.get("schema_version")
    if schema == 2:
        if obligation.get("algorithm") != FP_FUSED_EXACT_V2_ALGORITHM:
            return STATUS_INCOMPLETE
        return evaluate_fp_scalar_obligation_v2_status(
            obligation,
            host_float=host_float,
            live_corpus_sha256=live_corpus_sha256,
            live_validation_ledger_hash=live_validation_ledger_hash,
            allowlist_contains_model=allowlist_contains_model,
        )
    if schema == 1 and obligation.get("algorithm") == FP_FUSED_ALGORITHM:
        return evaluate_fp_advanced_obligation_status(
            obligation,
            host_float=host_float,
        )
    return STATUS_INCOMPLETE


def build_fp_fused_attestation_v2(
    *,
    opcodes: Sequence[str] = (),
    host_float: bool = False,
    dimensions: Mapping[str, bool] | None = None,
    unsupported_remainder_result: str = "incomplete",
    unsupported_remainder_query_sha256: str = "",
    corpus_sha256: str = "",
    validation_ledger_hash: str = "",
) -> CapabilityAttestation:
    """Draft a Phase 9 fused attestation (status recomputed from obligation)."""
    obl = build_fp_fused_obligation_v2(
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
        "host_float": bool(host_float),
        "dimensions": dict(obl.get("dimensions") or {}),
    }
    if error is not None:
        evidence["schema_error"] = error
    status = evaluate_fp_fused_obligation_status(
        obl,
        host_float=host_float,
        live_corpus_sha256=corpus_sha256,
        live_validation_ledger_hash=validation_ledger_hash,
    )
    return build_attestation(
        capability=FP_FUSED_CAPABILITY,
        model_version=FP_FUSED_MODEL_VERSION_V2,
        algorithm=FP_FUSED_EXACT_V2_ALGORITHM,
        status=status,
        unsupported=("schema-invalid",) if error else (),
        evidence=evidence,
    )


def recompute_fp_fused_attestation_status(
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
    """Recompute fused grade; v2 can promote when allowlisted + complete."""
    if capability != FP_FUSED_CAPABILITY:
        return STATUS_INCOMPLETE
    if unsupported:
        return STATUS_INCOMPLETE
    if evidence.get("host_float") is True:
        return STATUS_INCOMPLETE

    if algorithm == FP_FUSED_ALGORITHM:
        obligation = evidence.get("obligation")
        return evaluate_fp_advanced_obligation_status(
            obligation if isinstance(obligation, dict) else None,
            host_float=evidence.get("host_float"),
        )

    if algorithm != FP_FUSED_EXACT_V2_ALGORITHM:
        return STATUS_INCOMPLETE
    if model_version and model_version != FP_FUSED_MODEL_VERSION_V2:
        return STATUS_INCOMPLETE
    allowlisted = bool(allowed_versions) and FP_FUSED_MODEL_VERSION_V2 in allowed_versions
    obligation = evidence.get("obligation")
    return evaluate_fp_fused_obligation_status(
        obligation if isinstance(obligation, dict) else None,
        host_float=evidence.get("host_float"),
        live_corpus_sha256=live_corpus_sha256,
        live_validation_ledger_hash=live_validation_ledger_hash,
        allowlist_contains_model=allowlisted,
    )


__all__ = [
    "FUSED_V2_COMPLETE_DIMENSIONS",
    "build_fp_fused_attestation_v2",
    "build_fp_fused_obligation_v2",
    "evaluate_fp_fused_obligation_status",
    "recompute_fp_fused_attestation_status",
]
