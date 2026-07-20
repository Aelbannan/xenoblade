"""fp-scalar-arithmetic obligation schema (Wave 3 foundations).

Builds and validates the JSON-shaped obligation used by capability assurance.
Discharge remains incomplete: validators accept well-formed obligations but
``fp-scalar-oracle-v1`` never grades promotion-grade until oracle / RN / NI /
FPSCR coverage lands.
"""

from __future__ import annotations

from typing import Any, Mapping, Sequence

from tools.ppc_equivalence.capability_assurance import (
    STATUS_INCOMPLETE,
    CapabilityAttestation,
    build_attestation,
)
from tools.ppc_equivalence.fp_capabilities import (
    FP_OUTCOME_UNIFY_ALGORITHM,
    FP_SCALAR_MODEL_VERSION,
    FP_SCALAR_ORACLE_ALGORITHM,
)
from tools.ppc_equivalence.fp_rounding import (
    BROADWAY_RN_MODES,
    normalize_rn_mode,
)
from tools.ppc_equivalence.provenance import canonical_json_sha256

FP_SCALAR_CAPABILITY = "fp-scalar-arithmetic"
FP_SCALAR_ORACLE_VERSION_STUB = "broadway-exact-fp-v2-incomplete"

_SHA256_LEN = 64
_HEX64 = frozenset("0123456789abcdef")

KNOWN_OBLIGATION_KEYS = frozenset(
    {
        "capability",
        "model_version",
        "opcodes",
        "oracle_version",
        "modes",
        "coverage",
        # Optional evidence digests (accepted when present).
        "oracle_sha256",
        "validation_corpus_sha256",
        "status",
    }
)
KNOWN_MODES_KEYS = frozenset({"rn", "ni", "traps"})
KNOWN_COVERAGE_KEYS = frozenset({"unsupported_remainder"})
KNOWN_REMAINDER_KEYS = frozenset({"result", "query_sha256"})
KNOWN_REMAINDER_RESULTS = frozenset(
    {"unsat", "sat", "unknown", "incomplete", "timeout"}
)
KNOWN_TRAP_MODE_LABELS = frozenset(
    {
        "disabled-by-proof",
        "disabled",
        "enabled",
        "incomplete",
        "unmodeled",
    }
)


def _is_sha256(value: Any) -> bool:
    return (
        isinstance(value, str)
        and len(value) == _SHA256_LEN
        and all(ch in _HEX64 for ch in value)
    )


def build_fp_scalar_obligation(
    *,
    opcodes: Sequence[str],
    oracle_version: str = FP_SCALAR_ORACLE_VERSION_STUB,
    rn: Sequence[str] | None = None,
    ni: Sequence[int] | None = None,
    traps: str = "incomplete",
    unsupported_remainder_result: str = "incomplete",
    unsupported_remainder_query_sha256: str = "",
    oracle_sha256: str | None = None,
    validation_corpus_sha256: str | None = None,
    status: str = STATUS_INCOMPLETE,
) -> dict[str, Any]:
    """Build a JSON-shaped fp-scalar-arithmetic obligation (may be incomplete)."""
    rn_modes = list(rn) if rn is not None else list(BROADWAY_RN_MODES)
    ni_modes = list(ni) if ni is not None else [0, 1]
    remainder: dict[str, Any] = {"result": unsupported_remainder_result}
    if unsupported_remainder_query_sha256:
        remainder["query_sha256"] = unsupported_remainder_query_sha256
    elif unsupported_remainder_result == "incomplete":
        # Placeholder digest so schema can be validated during foundations.
        remainder["query_sha256"] = "0" * 64
    obligation: dict[str, Any] = {
        "capability": FP_SCALAR_CAPABILITY,
        "model_version": FP_SCALAR_MODEL_VERSION,
        "opcodes": [str(op) for op in opcodes],
        "oracle_version": str(oracle_version),
        "modes": {
            "rn": rn_modes,
            "ni": ni_modes,
            "traps": str(traps),
        },
        "coverage": {
            "unsupported_remainder": remainder,
        },
        "status": str(status),
    }
    if oracle_sha256 is not None:
        obligation["oracle_sha256"] = oracle_sha256
    if validation_corpus_sha256 is not None:
        obligation["validation_corpus_sha256"] = validation_corpus_sha256
    return obligation


def validate_fp_scalar_obligation(obligation: Mapping[str, Any] | None) -> str | None:
    """Strict schema check; unknown fields fail closed. ``None`` = ok."""
    if obligation is None:
        return "fp-scalar obligation is missing"
    if not isinstance(obligation, Mapping):
        return "fp-scalar obligation must be an object"

    unknown = sorted(set(obligation.keys()) - KNOWN_OBLIGATION_KEYS)
    if unknown:
        return f"fp-scalar unknown fields: {', '.join(unknown)}"
    required = {
        "capability",
        "model_version",
        "opcodes",
        "oracle_version",
        "modes",
        "coverage",
    }
    missing = sorted(required - set(obligation.keys()))
    if missing:
        return f"fp-scalar missing fields: {', '.join(missing)}"

    if obligation.get("capability") != FP_SCALAR_CAPABILITY:
        return f"fp-scalar.capability must be {FP_SCALAR_CAPABILITY!r}"
    if obligation.get("model_version") != FP_SCALAR_MODEL_VERSION:
        return (
            f"fp-scalar.model_version must be {FP_SCALAR_MODEL_VERSION!r}"
        )
    if not isinstance(obligation.get("oracle_version"), str) or not obligation["oracle_version"]:
        return "fp-scalar.oracle_version must be a nonempty string"

    opcodes = obligation.get("opcodes")
    if not isinstance(opcodes, list) or not opcodes:
        return "fp-scalar.opcodes must be a nonempty list"
    if not all(isinstance(op, str) and op for op in opcodes):
        return "fp-scalar.opcodes entries must be nonempty strings"

    for digest_key in ("oracle_sha256", "validation_corpus_sha256"):
        if digest_key in obligation and not _is_sha256(obligation.get(digest_key)):
            return f"fp-scalar.{digest_key} must be a 64-hex digest"

    modes = obligation.get("modes")
    if not isinstance(modes, Mapping):
        return "fp-scalar.modes must be an object"
    unknown_modes = sorted(set(modes.keys()) - KNOWN_MODES_KEYS)
    if unknown_modes:
        return f"fp-scalar.modes unknown fields: {', '.join(unknown_modes)}"
    missing_modes = sorted(KNOWN_MODES_KEYS - set(modes.keys()))
    if missing_modes:
        return f"fp-scalar.modes missing fields: {', '.join(missing_modes)}"

    rn = modes.get("rn")
    if not isinstance(rn, list) or not rn:
        return "fp-scalar.modes.rn must be a nonempty list"
    for item in rn:
        if normalize_rn_mode(str(item)) is None:
            return f"fp-scalar.modes.rn unknown mode {item!r}"

    ni = modes.get("ni")
    if not isinstance(ni, list) or not ni:
        return "fp-scalar.modes.ni must be a nonempty list"
    for item in ni:
        if item not in (0, 1):
            return "fp-scalar.modes.ni entries must be 0 or 1"

    traps = modes.get("traps")
    if traps not in KNOWN_TRAP_MODE_LABELS:
        return f"fp-scalar.modes.traps unknown label {traps!r}"

    coverage = obligation.get("coverage")
    if not isinstance(coverage, Mapping):
        return "fp-scalar.coverage must be an object"
    unknown_cov = sorted(set(coverage.keys()) - KNOWN_COVERAGE_KEYS)
    if unknown_cov:
        return f"fp-scalar.coverage unknown fields: {', '.join(unknown_cov)}"
    if "unsupported_remainder" not in coverage:
        return "fp-scalar.coverage missing fields: unsupported_remainder"

    remainder = coverage.get("unsupported_remainder")
    if not isinstance(remainder, Mapping):
        return "fp-scalar.coverage.unsupported_remainder must be an object"
    unknown_rem = sorted(set(remainder.keys()) - KNOWN_REMAINDER_KEYS)
    if unknown_rem:
        return (
            "fp-scalar.coverage.unsupported_remainder unknown fields: "
            + ", ".join(unknown_rem)
        )
    missing_rem = sorted(KNOWN_REMAINDER_KEYS - set(remainder.keys()))
    if missing_rem:
        return (
            "fp-scalar.coverage.unsupported_remainder missing fields: "
            + ", ".join(missing_rem)
        )
    if remainder.get("result") not in KNOWN_REMAINDER_RESULTS:
        return (
            "fp-scalar.coverage.unsupported_remainder.result must be one of "
            + ", ".join(sorted(KNOWN_REMAINDER_RESULTS))
        )
    if not _is_sha256(remainder.get("query_sha256")):
        return (
            "fp-scalar.coverage.unsupported_remainder.query_sha256 "
            "must be a 64-hex digest"
        )

    status = obligation.get("status")
    if status is not None and not isinstance(status, str):
        return "fp-scalar.status must be a string when present"

    return None


def evaluate_fp_scalar_obligation_status(
    obligation: Mapping[str, Any] | None,
    *,
    host_float: bool | None = None,
) -> str:
    """Foundations grader: well-formed obligations stay incomplete.

    Host-float participation or malformed schema → incomplete (never
    promotion-grade under Wave 3).
    """
    error = validate_fp_scalar_obligation(obligation)
    if error is not None:
        return STATUS_INCOMPLETE
    if host_float is True:
        return STATUS_INCOMPLETE
    # Wave 3: scalar oracle / all-RN / NI / FPSCR FX-FEX unfinished.
    return STATUS_INCOMPLETE


def build_fp_scalar_attestation(
    obligation: Mapping[str, Any],
    *,
    algorithm: str = FP_SCALAR_ORACLE_ALGORITHM,
    host_float: bool = False,
    assumptions: Sequence[str] = (),
    unsupported: Sequence[str] = (),
) -> CapabilityAttestation:
    """Draft an fp-scalar-arithmetic attestation wrapping an obligation."""
    error = validate_fp_scalar_obligation(obligation)
    evidence: dict[str, Any] = {
        "obligation": dict(obligation),
        "obligation_sha256": canonical_json_sha256(dict(obligation)),
        "host_float": bool(host_float),
    }
    if error is not None:
        evidence["schema_error"] = error
    return build_attestation(
        capability=FP_SCALAR_CAPABILITY,
        model_version=FP_SCALAR_MODEL_VERSION,
        algorithm=algorithm,
        status=STATUS_INCOMPLETE,
        assumptions=tuple(assumptions),
        unsupported=tuple(unsupported) if unsupported else (
            ("schema-invalid",) if error else ("scalar-oracle-incomplete",)
        ),
        evidence=evidence,
    )


def build_fp_outcome_unify_attestation(
    *,
    opcodes: Sequence[str],
    host_float: bool = False,
) -> CapabilityAttestation:
    """Draft attestation that SymbolicOps/ConcreteOps share FPOutcome.

    Algorithm ``fp-outcome-unify-v1`` records type unification progress; it is
    never promotion-grade on its own for scalar arithmetic.
    """
    return build_attestation(
        capability=FP_SCALAR_CAPABILITY,
        model_version=FP_SCALAR_MODEL_VERSION,
        algorithm=FP_OUTCOME_UNIFY_ALGORITHM,
        status=STATUS_INCOMPLETE,
        unsupported=("fp-outcome-unify-foundations-only",),
        evidence={
            "opcodes": [str(op) for op in opcodes],
            "host_float": bool(host_float),
            "symbolic_outcome": True,
            "concrete_outcome": True,
        },
    )
