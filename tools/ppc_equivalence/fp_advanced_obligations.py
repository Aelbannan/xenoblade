"""Wave 4 advanced FP obligation / attestation stubs.

Fail-closed foundations for ``fp-fused-arithmetic``, ``fp-paired-single``,
``fp-psq``, and ``fp-traps``. These capabilities are demanded when proofs use
the corresponding opcodes (or ``traps_enabled``), but attestations are
**never promotion-grade** while:

- fused midpoint-tie residues and near-cancellation sticky residues remain
  incomplete (see ``fp_oracle`` / ``fp_outcome`` deferred list);
- MSR FE0/FE1 precise vs imprecise trap modes remain incomplete
  (see ``fp_traps``).

Allowlists for these model versions stay empty in the capability manifest.
"""

from __future__ import annotations

from typing import Any, Mapping, Sequence

from tools.ppc_equivalence.capability_assurance import (
    STATUS_INCOMPLETE,
    CapabilityAttestation,
    build_attestation,
)
from tools.ppc_equivalence.fp_capabilities import (
    FP_FUSED_ARITH_OPS,
    FP_FUSED_MODEL_VERSION,
    FP_PAIRED_MODEL_VERSION,
    FP_PAIRED_SINGLE_OPS,
    FP_PSQ_MODEL_VERSION,
    FP_PSQ_OPS,
    FP_TRAPS_MODEL_VERSION,
)
from tools.ppc_equivalence.provenance import canonical_json_sha256

FP_FUSED_CAPABILITY = "fp-fused-arithmetic"
FP_PAIRED_CAPABILITY = "fp-paired-single"
FP_PSQ_CAPABILITY = "fp-psq"
FP_TRAPS_CAPABILITY = "fp-traps"

FP_FUSED_ALGORITHM = "fp-fused-incomplete-v0"
FP_PAIRED_ALGORITHM = "fp-paired-incomplete-v0"
FP_PSQ_ALGORITHM = "fp-psq-incomplete-v0"
FP_TRAPS_ALGORITHM = "fp-traps-incomplete-v0"

FP_ADVANCED_ALGORITHMS = frozenset(
    {
        FP_FUSED_ALGORITHM,
        FP_PAIRED_ALGORITHM,
        FP_PSQ_ALGORITHM,
        FP_TRAPS_ALGORITHM,
    }
)

# Capabilities that must never grade promotion-grade under Wave 4 stubs.
ALWAYS_INCOMPLETE_FP_ADVANCED = frozenset(
    {
        FP_FUSED_CAPABILITY,
        FP_PAIRED_CAPABILITY,
        FP_PSQ_CAPABILITY,
        FP_TRAPS_CAPABILITY,
    }
)

FP_ADVANCED_OBLIGATION_SCHEMA_VERSION = 1

# Documented incomplete dimensions — promotion requires every flag true.
FUSED_INCOMPLETE_DIMENSIONS: dict[str, bool] = {
    "midpoint_residual": False,
    "sticky_residue": False,
    "result_bits": False,
    "nan_payloads": False,
    "traps": False,
}

PAIRED_INCOMPLETE_DIMENSIONS: dict[str, bool] = {
    "lane_aggregation": False,
    "symbolic_outcome": False,
    "traps": False,
}

PSQ_INCOMPLETE_DIMENSIONS: dict[str, bool] = {
    "quantize": False,
    "dequantize": False,
    "gqr": False,
    "traps": False,
}

TRAPS_INCOMPLETE_DIMENSIONS: dict[str, bool] = {
    "ve_ze_oe_ue_xe": False,
    "destination_suppression": False,
    "srr0_srr1": False,
    "fex_reraise": False,
    "fe0_fe1": False,
    "traps": False,
}

_SHA256_LEN = 64
_HEX64 = frozenset("0123456789abcdef")

_CAPABILITY_META: dict[str, dict[str, Any]] = {
    FP_FUSED_CAPABILITY: {
        "model_version": FP_FUSED_MODEL_VERSION,
        "algorithm": FP_FUSED_ALGORITHM,
        "opcode_set": FP_FUSED_ARITH_OPS,
        "dimensions": FUSED_INCOMPLETE_DIMENSIONS,
        "unsupported": (
            "fused-midpoint-residual-incomplete",
            "fused-sticky-residue-incomplete",
        ),
    },
    FP_PAIRED_CAPABILITY: {
        "model_version": FP_PAIRED_MODEL_VERSION,
        "algorithm": FP_PAIRED_ALGORITHM,
        "opcode_set": FP_PAIRED_SINGLE_OPS,
        "dimensions": PAIRED_INCOMPLETE_DIMENSIONS,
        "unsupported": ("paired-single-symbolic-incomplete",),
    },
    FP_PSQ_CAPABILITY: {
        "model_version": FP_PSQ_MODEL_VERSION,
        "algorithm": FP_PSQ_ALGORITHM,
        "opcode_set": FP_PSQ_OPS,
        "dimensions": PSQ_INCOMPLETE_DIMENSIONS,
        "unsupported": ("psq-quantize-incomplete",),
    },
    FP_TRAPS_CAPABILITY: {
        "model_version": FP_TRAPS_MODEL_VERSION,
        "algorithm": FP_TRAPS_ALGORITHM,
        "opcode_set": frozenset(),
        "dimensions": TRAPS_INCOMPLETE_DIMENSIONS,
        "unsupported": ("msr-fe0-fe1-incomplete",),
    },
}

KNOWN_OBLIGATION_KEYS = frozenset(
    {
        "schema_version",
        "capability",
        "model_version",
        "algorithm",
        "opcodes",
        "dimensions",
        "coverage",
        "status",
        "notes",
    }
)
KNOWN_COVERAGE_KEYS = frozenset({"unsupported_remainder"})
KNOWN_REMAINDER_KEYS = frozenset({"result", "query_sha256"})
KNOWN_REMAINDER_RESULTS = frozenset(
    {"unsat", "sat", "unknown", "incomplete", "timeout"}
)


def _is_sha256(value: Any) -> bool:
    return (
        isinstance(value, str)
        and len(value) == _SHA256_LEN
        and all(ch in _HEX64 for ch in value)
    )


def build_fp_advanced_obligation(
    capability: str,
    *,
    opcodes: Sequence[str] = (),
    status: str = STATUS_INCOMPLETE,
    unsupported_remainder_result: str = "incomplete",
    unsupported_remainder_query_sha256: str = "",
    notes: str | None = None,
) -> dict[str, Any]:
    """Build a JSON-shaped Wave 4 advanced FP obligation (always incomplete)."""
    meta = _CAPABILITY_META.get(capability)
    if meta is None:
        raise ValueError(f"unknown Wave 4 FP advanced capability {capability!r}")

    remainder: dict[str, Any] = {"result": unsupported_remainder_result}
    if unsupported_remainder_query_sha256:
        remainder["query_sha256"] = unsupported_remainder_query_sha256
    elif unsupported_remainder_result == "incomplete":
        remainder["query_sha256"] = "0" * 64

    resolved_opcodes = [str(op) for op in opcodes]
    if not resolved_opcodes and capability != FP_TRAPS_CAPABILITY:
        # Default to the full opcode family when callers omit a subset.
        resolved_opcodes = sorted(meta["opcode_set"])

    default_notes = {
        FP_FUSED_CAPABILITY: (
            "Fused midpoint-tie residues and near-cancellation sticky "
            "residues remain incomplete; never promotion-grade."
        ),
        FP_PAIRED_CAPABILITY: (
            "Paired-single symbolic / lane aggregation incomplete; "
            "never promotion-grade under Wave 4 stubs."
        ),
        FP_PSQ_CAPABILITY: (
            "PSQ quantize/dequantize/GQR incomplete; never promotion-grade."
        ),
        FP_TRAPS_CAPABILITY: (
            "MSR FE0/FE1 precise vs imprecise modes remain incomplete; "
            "fp-traps never promotion-grade until modeled."
        ),
    }

    return {
        "schema_version": FP_ADVANCED_OBLIGATION_SCHEMA_VERSION,
        "capability": capability,
        "model_version": meta["model_version"],
        "algorithm": meta["algorithm"],
        "opcodes": resolved_opcodes,
        "dimensions": dict(meta["dimensions"]),
        "coverage": {"unsupported_remainder": remainder},
        "status": str(status),
        "notes": notes if notes is not None else default_notes[capability],
    }


def validate_fp_advanced_obligation(
    obligation: Mapping[str, Any] | None,
) -> str | None:
    """Strict schema check; unknown fields / capabilities fail closed."""
    if obligation is None:
        return "fp-advanced obligation is missing"
    if not isinstance(obligation, Mapping):
        return "fp-advanced obligation must be an object"

    unknown = sorted(set(obligation.keys()) - KNOWN_OBLIGATION_KEYS)
    if unknown:
        return f"fp-advanced unknown fields: {', '.join(unknown)}"

    required = {
        "schema_version",
        "capability",
        "model_version",
        "algorithm",
        "opcodes",
        "dimensions",
        "coverage",
    }
    missing = sorted(required - set(obligation.keys()))
    if missing:
        return f"fp-advanced missing fields: {', '.join(missing)}"

    if obligation.get("schema_version") != FP_ADVANCED_OBLIGATION_SCHEMA_VERSION:
        return (
            "fp-advanced.schema_version must be "
            f"{FP_ADVANCED_OBLIGATION_SCHEMA_VERSION}"
        )

    capability = obligation.get("capability")
    if capability not in _CAPABILITY_META:
        return f"fp-advanced.capability unsupported ({capability!r})"

    meta = _CAPABILITY_META[capability]
    if obligation.get("model_version") != meta["model_version"]:
        return (
            f"fp-advanced.model_version must be {meta['model_version']!r} "
            f"for {capability}"
        )
    if obligation.get("algorithm") != meta["algorithm"]:
        return (
            f"fp-advanced.algorithm must be {meta['algorithm']!r} "
            f"for {capability}"
        )

    opcodes = obligation.get("opcodes")
    if not isinstance(opcodes, list):
        return "fp-advanced.opcodes must be a list"
    if capability != FP_TRAPS_CAPABILITY and not opcodes:
        return "fp-advanced.opcodes must be a nonempty list"
    if not all(isinstance(op, str) and op for op in opcodes):
        return "fp-advanced.opcodes entries must be nonempty strings"
    opcode_set: frozenset[str] = meta["opcode_set"]
    if opcode_set:
        bad = sorted(op for op in opcodes if op not in opcode_set)
        if bad:
            return f"fp-advanced.opcodes outside family: {', '.join(bad)}"

    dimensions = obligation.get("dimensions")
    if not isinstance(dimensions, Mapping):
        return "fp-advanced.dimensions must be an object"
    expected_dims = set(meta["dimensions"].keys())
    unknown_dims = sorted(set(dimensions.keys()) - expected_dims)
    if unknown_dims:
        return f"fp-advanced.dimensions unknown fields: {', '.join(unknown_dims)}"
    missing_dims = sorted(expected_dims - set(dimensions.keys()))
    if missing_dims:
        return f"fp-advanced.dimensions missing fields: {', '.join(missing_dims)}"
    for key, value in dimensions.items():
        if not isinstance(value, bool):
            return f"fp-advanced.dimensions.{key} must be a bool"

    coverage = obligation.get("coverage")
    if not isinstance(coverage, Mapping):
        return "fp-advanced.coverage must be an object"
    unknown_cov = sorted(set(coverage.keys()) - KNOWN_COVERAGE_KEYS)
    if unknown_cov:
        return f"fp-advanced.coverage unknown fields: {', '.join(unknown_cov)}"
    if "unsupported_remainder" not in coverage:
        return "fp-advanced.coverage missing fields: unsupported_remainder"
    remainder = coverage.get("unsupported_remainder")
    if not isinstance(remainder, Mapping):
        return "fp-advanced.coverage.unsupported_remainder must be an object"
    unknown_rem = sorted(set(remainder.keys()) - KNOWN_REMAINDER_KEYS)
    if unknown_rem:
        return (
            "fp-advanced.coverage.unsupported_remainder unknown fields: "
            f"{', '.join(unknown_rem)}"
        )
    result = remainder.get("result")
    if result not in KNOWN_REMAINDER_RESULTS:
        return (
            "fp-advanced.coverage.unsupported_remainder.result "
            f"unsupported ({result!r})"
        )
    if "query_sha256" in remainder and not _is_sha256(remainder.get("query_sha256")):
        return (
            "fp-advanced.coverage.unsupported_remainder.query_sha256 "
            "must be a 64-hex digest"
        )

    status = obligation.get("status")
    if status is not None and status not in {
        STATUS_INCOMPLETE,
        "scoped-assumption",
        "unmodeled",
        "promotion-grade",  # accepted structurally; recomputed to incomplete
    }:
        return f"fp-advanced.status unsupported ({status!r})"

    return None


def evaluate_fp_advanced_obligation_status(
    obligation: Mapping[str, Any] | None,
    *,
    host_float: Any = None,
) -> str:
    """Always incomplete under Wave 4; host_float / malformed → incomplete."""
    if host_float is True:
        return STATUS_INCOMPLETE
    reason = validate_fp_advanced_obligation(obligation)
    if reason is not None:
        return STATUS_INCOMPLETE
    assert obligation is not None
    # Even a well-formed obligation with forged promotion-grade status stays
    # incomplete while midpoint / sticky-residue / FE0/FE1 remain unfinished.
    dims = obligation.get("dimensions") or {}
    if not all(bool(value) for value in dims.values()):
        return STATUS_INCOMPLETE
    remainder = (obligation.get("coverage") or {}).get("unsupported_remainder") or {}
    if remainder.get("result") != "unsat":
        return STATUS_INCOMPLETE
    # Dimensions all true + UNSAT still cannot promote: FE0/FE1 and fused
    # residual gaps are architectural policy for Wave 4 stubs.
    return STATUS_INCOMPLETE


def build_fp_advanced_attestation(
    capability: str,
    *,
    opcodes: Sequence[str] = (),
    host_float: bool = False,
    assumptions: Sequence[str] = (),
    unsupported: Sequence[str] = (),
    obligation: Mapping[str, Any] | None = None,
) -> CapabilityAttestation:
    """Draft a Wave 4 advanced FP attestation (status advisory / incomplete)."""
    meta = _CAPABILITY_META.get(capability)
    if meta is None:
        raise ValueError(f"unknown Wave 4 FP advanced capability {capability!r}")

    obl = (
        dict(obligation)
        if obligation is not None
        else build_fp_advanced_obligation(capability, opcodes=opcodes)
    )
    error = validate_fp_advanced_obligation(obl)
    default_unsupported = tuple(meta["unsupported"])
    evidence: dict[str, Any] = {
        "obligation": obl,
        "obligation_sha256": canonical_json_sha256(obl),
        "host_float": bool(host_float),
        "dimensions": dict(obl.get("dimensions") or meta["dimensions"]),
        "never_promotion_grade": True,
    }
    if error is not None:
        evidence["schema_error"] = error
    return build_attestation(
        capability=capability,
        model_version=str(meta["model_version"]),
        algorithm=str(meta["algorithm"]),
        status=STATUS_INCOMPLETE,
        assumptions=tuple(assumptions),
        unsupported=tuple(unsupported)
        if unsupported
        else (
            ("schema-invalid",) if error else default_unsupported
        ),
        evidence=evidence,
    )


def recompute_fp_advanced_attestation_status(
    evidence: Mapping[str, Any],
    *,
    capability: str,
    algorithm: str,
    model_version: str = "",
    assumptions: Sequence[str] = (),
    unsupported: Sequence[str] = (),
    allowed_versions: Sequence[str] = (),
) -> str:
    """Recompute grade; Wave 4 advanced caps are never promotion-grade."""
    del assumptions, allowed_versions  # allowlist empty; never promote anyway
    if capability not in ALWAYS_INCOMPLETE_FP_ADVANCED:
        return STATUS_INCOMPLETE
    if algorithm not in FP_ADVANCED_ALGORITHMS:
        return STATUS_INCOMPLETE
    meta = _CAPABILITY_META[capability]
    if model_version and model_version != meta["model_version"]:
        return STATUS_INCOMPLETE
    if unsupported:
        return STATUS_INCOMPLETE
    if evidence.get("host_float") is True:
        return STATUS_INCOMPLETE
    obligation = evidence.get("obligation") or evidence.get(capability)
    return evaluate_fp_advanced_obligation_status(
        obligation if isinstance(obligation, dict) else None,
        host_float=evidence.get("host_float"),
    )
