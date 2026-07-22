"""Production-grade FP obligation schema v2 (Phase 10 — SCALAR_FP_V2.md).

Replaces foundations-only ``fp-scalar-oracle-v1`` with independently recomputed
promotion status keyed on model/algorithm versions, domain metadata, no-host-float,
UNSAT unsupported remainder, and corpus/ledger digests.

Example obligation (schema_version 2)::

    {
      "schema_version": 2,
      "capability": "fp-fused-arithmetic",
      "model_version": "broadway-fp-fused-v2",
      "algorithm": "fp-fused-exact-v2",
      "domain": {
        "no_host_float": true,
        "fused_input_domain": "exact-expanded-binary32"
      },
      "opcodes": ["fmadd", "fmadds"],
      "modes": {
        "rn": ["nearest-even"],
        "ni": [0, 1],
        "traps": "disabled-by-proof"
      },
      "dimensions": {
        "midpoint_residual": true,
        "sticky_residue": true,
        "result_bits": true,
        "nan_payloads": true,
        "traps": true
      },
      "coverage": {
        "unsupported_remainder": {
          "result": "unsat",
          "query_sha256": "<64-hex>"
        },
        "corpus_sha256": "<64-hex>",
        "validation_ledger_hash": "<64-hex>"
      },
      "status": "incomplete"
    }
"""

from __future__ import annotations

from typing import Any, Mapping, Sequence

from tools.ppc_equivalence.capability_assurance import (
    STATUS_INCOMPLETE,
    STATUS_PROMOTION_GRADE,
)
from tools.ppc_equivalence.fp_capabilities import (
    FP_EXPERIMENTAL_SUBCAPABILITY_MODEL_VERSIONS,
    FP_FUSED_ARITH_OPS,
    FP_LOAD_STORE_OPS,
)
from tools.ppc_equivalence.fp_rounding import BROADWAY_RN_MODES, normalize_rn_mode
from tools.ppc_equivalence.provenance import canonical_json_sha256

FP_SCALAR_OBLIGATION_SCHEMA_VERSION_V2 = 2

FP_SCALAR_EXACT_V2_ALGORITHM = "broadway-exact-fp-v2"
FP_FUSED_EXACT_V2_ALGORITHM = "fp-fused-exact-v2"
FP_LOAD_STORE_EXACT_V2_ALGORITHM = "fp-load-store-exact-v2"

_SHA256_LEN = 64
_HEX64 = frozenset("0123456789abcdef")

KNOWN_OBLIGATION_KEYS_V2 = frozenset(
    {
        "schema_version",
        "capability",
        "model_version",
        "algorithm",
        "domain",
        "opcodes",
        "modes",
        "dimensions",
        "coverage",
        "status",
        "notes",
    }
)
KNOWN_DOMAIN_KEYS_V2 = frozenset(
    {"no_host_float", "fused_input_domain", "load_store_domain"}
)
KNOWN_MODES_KEYS_V2 = frozenset({"rn", "ni", "traps"})
KNOWN_COVERAGE_KEYS_V2 = frozenset(
    {"unsupported_remainder", "corpus_sha256", "validation_ledger_hash"}
)
KNOWN_REMAINDER_KEYS_V2 = frozenset({"result", "query_sha256"})
KNOWN_REMAINDER_RESULTS_V2 = frozenset(
    {"unsat", "sat", "unknown", "incomplete", "timeout", "vacuous"}
)
KNOWN_TRAP_MODE_LABELS_V2 = frozenset(
    {
        "disabled-by-proof",
        "disabled",
        "enabled",
        "incomplete",
        "unmodeled",
    }
)
KNOWN_FUSED_INPUT_DOMAINS = frozenset({"exact-expanded-binary32", "full-architectural"})
KNOWN_LOAD_STORE_DOMAINS = frozenset({"bit-transform-exact-v2"})

FUSED_V2_DIMENSIONS: dict[str, bool] = {
    "midpoint_residual": False,
    "sticky_residue": False,
    "result_bits": False,
    "nan_payloads": False,
    "traps": False,
}

LOAD_STORE_V2_DIMENSIONS: dict[str, bool] = {
    "bit_expansion_lfs": False,
    "preserve_lfd": False,
    "stfs_rounding": False,
    "stfiwx_raw": False,
    "all_rn_modes_stfs": False,
    "bounded_memory": False,
    "symbolic_backend": False,
    "traps": False,
}

_CAPABILITY_DEFAULTS: dict[str, dict[str, Any]] = {
    "fp-fused-arithmetic": {
        "model_version": FP_EXPERIMENTAL_SUBCAPABILITY_MODEL_VERSIONS[
            "fp-fused-arithmetic"
        ],
        "algorithm": FP_FUSED_EXACT_V2_ALGORITHM,
        "opcode_set": FP_FUSED_ARITH_OPS,
        "dimensions": FUSED_V2_DIMENSIONS,
        "domain": {
            "no_host_float": True,
            "fused_input_domain": "exact-expanded-binary32",
        },
    },
    "fp-load-store": {
        "model_version": FP_EXPERIMENTAL_SUBCAPABILITY_MODEL_VERSIONS[
            "fp-load-store"
        ],
        "algorithm": FP_LOAD_STORE_EXACT_V2_ALGORITHM,
        "opcode_set": FP_LOAD_STORE_OPS,
        "dimensions": LOAD_STORE_V2_DIMENSIONS,
        "domain": {
            "no_host_float": True,
            "load_store_domain": "bit-transform-exact-v2",
        },
    },
}


def _is_sha256(value: Any) -> bool:
    return (
        isinstance(value, str)
        and len(value) == _SHA256_LEN
        and all(ch in _HEX64 for ch in value)
    )


def build_fp_scalar_obligation_v2(
    capability: str,
    *,
    opcodes: Sequence[str] = (),
    algorithm: str | None = None,
    model_version: str | None = None,
    domain: Mapping[str, Any] | None = None,
    dimensions: Mapping[str, bool] | None = None,
    rn: Sequence[str] | None = None,
    ni: Sequence[int] | None = None,
    traps: str = "incomplete",
    unsupported_remainder_result: str = "incomplete",
    unsupported_remainder_query_sha256: str = "",
    corpus_sha256: str = "",
    validation_ledger_hash: str = "",
    status: str = STATUS_INCOMPLETE,
    notes: str | None = None,
) -> dict[str, Any]:
    """Build a schema-v2 obligation for an FP sub-capability."""
    meta = _CAPABILITY_DEFAULTS.get(capability)
    if meta is None:
        raise ValueError(f"unsupported v2 FP capability {capability!r}")

    resolved_opcodes = [str(op) for op in opcodes]
    if not resolved_opcodes:
        resolved_opcodes = sorted(meta["opcode_set"])

    remainder: dict[str, Any] = {"result": unsupported_remainder_result}
    if unsupported_remainder_query_sha256:
        remainder["query_sha256"] = unsupported_remainder_query_sha256
    elif unsupported_remainder_result == "incomplete":
        remainder["query_sha256"] = "0" * 64

    coverage: dict[str, Any] = {"unsupported_remainder": remainder}
    if corpus_sha256:
        coverage["corpus_sha256"] = corpus_sha256
    if validation_ledger_hash:
        coverage["validation_ledger_hash"] = validation_ledger_hash

    return {
        "schema_version": FP_SCALAR_OBLIGATION_SCHEMA_VERSION_V2,
        "capability": capability,
        "model_version": model_version or meta["model_version"],
        "algorithm": algorithm or meta["algorithm"],
        "domain": dict(domain or meta["domain"]),
        "opcodes": resolved_opcodes,
        "modes": {
            "rn": list(rn) if rn is not None else list(BROADWAY_RN_MODES),
            "ni": list(ni) if ni is not None else [0, 1],
            "traps": str(traps),
        },
        "dimensions": dict(dimensions or meta["dimensions"]),
        "coverage": coverage,
        "status": str(status),
        "notes": notes,
    }


def live_scalar_fp_v2_corpus_sha256() -> str:
    """Recompute the live aggregate digest for schema-v2 obligation binding."""
    from tools.ppc_equivalence.scalar_fp_v2_corpus import (
        compute_scalar_fp_v2_corpus_sha256,
    )

    return compute_scalar_fp_v2_corpus_sha256()


def validate_fp_scalar_obligation_v2(
    obligation: Mapping[str, Any] | None,
) -> str | None:
    """Strict schema-v2 check; unknown fields fail closed."""
    if obligation is None:
        return "fp-scalar-v2 obligation is missing"
    if not isinstance(obligation, Mapping):
        return "fp-scalar-v2 obligation must be an object"

    unknown = sorted(set(obligation.keys()) - KNOWN_OBLIGATION_KEYS_V2)
    if unknown:
        return f"fp-scalar-v2 unknown fields: {', '.join(unknown)}"

    if obligation.get("schema_version") != FP_SCALAR_OBLIGATION_SCHEMA_VERSION_V2:
        return (
            "fp-scalar-v2.schema_version must be "
            f"{FP_SCALAR_OBLIGATION_SCHEMA_VERSION_V2}"
        )

    capability = obligation.get("capability")
    meta = _CAPABILITY_DEFAULTS.get(str(capability))
    if meta is None:
        return f"fp-scalar-v2.capability unsupported ({capability!r})"

    if obligation.get("model_version") != meta["model_version"]:
        return (
            f"fp-scalar-v2.model_version must be {meta['model_version']!r} "
            f"for {capability}"
        )
    if obligation.get("algorithm") != meta["algorithm"]:
        return (
            f"fp-scalar-v2.algorithm must be {meta['algorithm']!r} "
            f"for {capability}"
        )

    domain = obligation.get("domain")
    if not isinstance(domain, Mapping):
        return "fp-scalar-v2.domain must be an object"
    unknown_domain = sorted(set(domain.keys()) - KNOWN_DOMAIN_KEYS_V2)
    if unknown_domain:
        return f"fp-scalar-v2.domain unknown fields: {', '.join(unknown_domain)}"
    if domain.get("no_host_float") is not True:
        return "fp-scalar-v2.domain.no_host_float must be true"
    if str(capability) == "fp-fused-arithmetic":
        fused_domain = domain.get("fused_input_domain")
        if fused_domain not in KNOWN_FUSED_INPUT_DOMAINS:
            return (
                f"fp-scalar-v2.domain.fused_input_domain unsupported ({fused_domain!r})"
            )
    elif str(capability) == "fp-load-store":
        load_store_domain = domain.get("load_store_domain")
        if load_store_domain not in KNOWN_LOAD_STORE_DOMAINS:
            return (
                "fp-scalar-v2.domain.load_store_domain unsupported "
                f"({load_store_domain!r})"
            )

    opcodes = obligation.get("opcodes")
    if not isinstance(opcodes, list) or not opcodes:
        return "fp-scalar-v2.opcodes must be a nonempty list"
    opcode_set: frozenset[str] = meta["opcode_set"]
    bad = sorted(op for op in opcodes if op not in opcode_set)
    if bad:
        return f"fp-scalar-v2.opcodes outside family: {', '.join(bad)}"

    modes = obligation.get("modes")
    if not isinstance(modes, Mapping):
        return "fp-scalar-v2.modes must be an object"
    unknown_modes = sorted(set(modes.keys()) - KNOWN_MODES_KEYS_V2)
    if unknown_modes:
        return f"fp-scalar-v2.modes unknown fields: {', '.join(unknown_modes)}"
    for item in modes.get("rn") or []:
        if normalize_rn_mode(str(item)) is None:
            return f"fp-scalar-v2.modes.rn unknown mode {item!r}"
    for item in modes.get("ni") or []:
        if item not in (0, 1):
            return "fp-scalar-v2.modes.ni entries must be 0 or 1"
    if modes.get("traps") not in KNOWN_TRAP_MODE_LABELS_V2:
        return f"fp-scalar-v2.modes.traps unknown label {modes.get('traps')!r}"

    dimensions = obligation.get("dimensions")
    if not isinstance(dimensions, Mapping):
        return "fp-scalar-v2.dimensions must be an object"
    expected_dims = set(meta["dimensions"].keys())
    if set(dimensions.keys()) != expected_dims:
        missing = sorted(expected_dims - set(dimensions.keys()))
        extra = sorted(set(dimensions.keys()) - expected_dims)
        if missing:
            return f"fp-scalar-v2.dimensions missing fields: {', '.join(missing)}"
        return f"fp-scalar-v2.dimensions unknown fields: {', '.join(extra)}"
    for key, value in dimensions.items():
        if not isinstance(value, bool):
            return f"fp-scalar-v2.dimensions.{key} must be a bool"

    coverage = obligation.get("coverage")
    if not isinstance(coverage, Mapping):
        return "fp-scalar-v2.coverage must be an object"
    unknown_cov = sorted(set(coverage.keys()) - KNOWN_COVERAGE_KEYS_V2)
    if unknown_cov:
        return f"fp-scalar-v2.coverage unknown fields: {', '.join(unknown_cov)}"
    if "unsupported_remainder" not in coverage:
        return "fp-scalar-v2.coverage missing fields: unsupported_remainder"
    remainder = coverage.get("unsupported_remainder")
    if not isinstance(remainder, Mapping):
        return "fp-scalar-v2.coverage.unsupported_remainder must be an object"
    if remainder.get("result") not in KNOWN_REMAINDER_RESULTS_V2:
        return "fp-scalar-v2.coverage.unsupported_remainder.result unsupported"
    if not _is_sha256(remainder.get("query_sha256")):
        return (
            "fp-scalar-v2.coverage.unsupported_remainder.query_sha256 "
            "must be a 64-hex digest"
        )
    for digest_key in ("corpus_sha256", "validation_ledger_hash"):
        if digest_key in coverage and not _is_sha256(coverage.get(digest_key)):
            return f"fp-scalar-v2.coverage.{digest_key} must be a 64-hex digest"

    status = obligation.get("status")
    if status is not None and not isinstance(status, str):
        return "fp-scalar-v2.status must be a string when present"

    return None


def recompute_obligation_digests(
    obligation: Mapping[str, Any],
    *,
    live_corpus_sha256: str = "",
    live_validation_ledger_hash: str = "",
) -> tuple[bool, list[str]]:
    """Return (ok, mismatch reasons) for corpus/ledger digest checks."""
    if not live_corpus_sha256:
        live_corpus_sha256 = live_scalar_fp_v2_corpus_sha256()
    reasons: list[str] = []
    coverage = obligation.get("coverage") or {}
    corpus = coverage.get("corpus_sha256")
    ledger = coverage.get("validation_ledger_hash")
    if corpus and live_corpus_sha256 and corpus != live_corpus_sha256:
        reasons.append("corpus_sha256 mismatch")
    if ledger and live_validation_ledger_hash and ledger != live_validation_ledger_hash:
        reasons.append("validation_ledger_hash mismatch")
    return (not reasons, reasons)


def evaluate_fp_scalar_obligation_v2_status(
    obligation: Mapping[str, Any] | None,
    *,
    host_float: bool | None = None,
    live_corpus_sha256: str = "",
    live_validation_ledger_hash: str = "",
    allowlist_contains_model: bool = False,
) -> str:
    """Independently recompute promotion status for schema-v2 obligations."""
    if host_float is True:
        return STATUS_INCOMPLETE
    error = validate_fp_scalar_obligation_v2(obligation)
    if error is not None:
        return STATUS_INCOMPLETE
    assert obligation is not None

    ok, _reasons = recompute_obligation_digests(
        obligation,
        live_corpus_sha256=live_corpus_sha256,
        live_validation_ledger_hash=live_validation_ledger_hash,
    )
    if not ok:
        return STATUS_INCOMPLETE

    dims = obligation.get("dimensions") or {}
    if not all(bool(value) for value in dims.values()):
        return STATUS_INCOMPLETE

    remainder = (obligation.get("coverage") or {}).get("unsupported_remainder") or {}
    if remainder.get("result") != "unsat":
        return STATUS_INCOMPLETE

    if not allowlist_contains_model:
        return STATUS_INCOMPLETE

    return STATUS_PROMOTION_GRADE


def obligation_v2_sha256(obligation: Mapping[str, Any]) -> str:
    """Canonical digest for attestation binding."""
    return canonical_json_sha256(dict(obligation))


def validation_ledger_scalar_fp_v2_hook() -> dict[str, Any]:
    """Minimal ValidationLedger extension notes for scalar FP v2 corpora.

    Full per-capability rows land in Phase 11; this hook documents the expected
    ledger keys beyond the fp-bitwise special case.
    """
    return {
        "scalar_fp_v2_corpus_roots": [
            "tools/ppc_equivalence/corpora/scalar_fp_v2/scalar_rn.jsonl",
            "tools/ppc_equivalence/corpora/scalar_fp_v2/fpscr.jsonl",
            "tools/ppc_equivalence/corpora/scalar_fp_v2/ni.jsonl",
            "tools/ppc_equivalence/corpora/scalar_fp_v2/compare_convert_control.jsonl",
            "tools/ppc_equivalence/corpora/scalar_fp_v2/traps_fe.jsonl",
            "tools/ppc_equivalence/corpora/scalar_fp_v2/fused_residual.jsonl",
        ],
        "binding": "corpus_sha256 + validation_ledger_hash in obligation v2",
        "status": "phase11_replay",
    }


__all__ = [
    "FP_FUSED_EXACT_V2_ALGORITHM",
    "FP_LOAD_STORE_EXACT_V2_ALGORITHM",
    "FP_SCALAR_EXACT_V2_ALGORITHM",
    "FP_SCALAR_OBLIGATION_SCHEMA_VERSION_V2",
    "FUSED_V2_DIMENSIONS",
    "LOAD_STORE_V2_DIMENSIONS",
    "build_fp_scalar_obligation_v2",
    "evaluate_fp_scalar_obligation_v2_status",
    "live_scalar_fp_v2_corpus_sha256",
    "obligation_v2_sha256",
    "recompute_obligation_digests",
    "validate_fp_scalar_obligation_v2",
    "validation_ledger_scalar_fp_v2_hook",
]
