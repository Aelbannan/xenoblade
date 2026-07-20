from __future__ import annotations

from dataclasses import dataclass, field
from typing import Any

from tools.ppc_equivalence.loop_summary import validate_loop_summary_obligation
from tools.ppc_equivalence.memory_bus_obligations import validate_memory_bus_obligation
from tools.ppc_equivalence.memory_loop import validate_memory_loop_obligation
from tools.ppc_equivalence.relational_induction import (
    validate_relational_induction_obligation,
)
from tools.ppc_equivalence.result import ProofResult, ProofStatus

# Reserved features that may appear in certificates but cannot yet justify
# EQUIVALENT until the engine implements them soundly.
# PR0 safety freeze: affine/memory-loop/memory-bus stay unsupported until their
# foundation repairs land. Jump-table readonly-image + indirect-target-closure
# are discharged independently (PR3). Relational-induction is discharged via
# five independent UNSAT queries (PR7) and may authorize EQUIVALENT when
# obligations validate with status discharged.
UNSUPPORTED_FOR_EQUIVALENT: frozenset[str] = frozenset({
    "affine-loop-summary",
    "memory-loop-summary",
    "memory-bus",
})

# Canonical proof-feature names and their required top-level obligation keys.
FEATURE_OBLIGATION_KEYS: dict[str, str] = {
    "readonly-image": "address_space",
    "indirect-target-closure": "indirect_targets",
    "affine-loop-summary": "loop_summary",
    "relational-induction": "relational_induction",
    "memory-loop-summary": "memory_loop",
    "memory-bus": "memory_bus",
}

KNOWN_PROOF_FEATURES: frozenset[str] = frozenset(FEATURE_OBLIGATION_KEYS)

# Stable top-level obligation field order for result/certificate copy helpers.
PROOF_OBLIGATION_FIELDS: tuple[str, ...] = (
    "address_space",
    "indirect_targets",
    "loop_summary",
    "relational_induction",
    "memory_loop",
    "memory_bus",
)

_OBLIGATION_KEYS: frozenset[str] = frozenset(FEATURE_OBLIGATION_KEYS.values())


@dataclass(frozen=True)
class ParsedProofFeatures:
    features: tuple[str, ...]
    obligations: dict[str, dict[str, Any]] = field(default_factory=dict)


def proof_obligations_from_result(result: ProofResult) -> dict[str, dict[str, Any]]:
    """Collect present obligation blocks from a ``ProofResult``."""
    obligations: dict[str, dict[str, Any]] = {}
    for key in PROOF_OBLIGATION_FIELDS:
        value = getattr(result, key, None)
        if isinstance(value, dict):
            obligations[key] = value
    return obligations


def apply_proof_obligations(
    result: ProofResult,
    data: dict[str, dict[str, Any]],
) -> None:
    """Copy obligation blocks from ``data`` onto ``result`` (in-place)."""
    for key in PROOF_OBLIGATION_FIELDS:
        if key not in data:
            continue
        value = data[key]
        if value is None:
            setattr(result, key, None)
        elif isinstance(value, dict):
            setattr(result, key, value)


def proof_obligations_from_dict(data: dict[str, Any]) -> dict[str, dict[str, Any]]:
    """Collect well-typed obligation blocks from a certificate/result dict."""
    obligations: dict[str, dict[str, Any]] = {}
    for key in PROOF_OBLIGATION_FIELDS:
        value = data.get(key)
        if isinstance(value, dict):
            obligations[key] = value
    return obligations


def parse_proof_features(data: dict[str, Any]) -> ParsedProofFeatures:
    """Parse proof-feature names and all known obligation blocks."""
    raw_features = data.get("proof_features")
    features: list[str] = []
    if isinstance(raw_features, list):
        features = [str(item) for item in raw_features if item]
    return ParsedProofFeatures(
        features=tuple(features),
        obligations=proof_obligations_from_dict(data),
    )


def _extract_payload(payload: dict[str, Any] | ProofResult) -> dict[str, Any]:
    if isinstance(payload, ProofResult):
        data: dict[str, Any] = {}
        if payload.proof_features:
            data["proof_features"] = list(payload.proof_features)
        data.update(proof_obligations_from_result(payload))
        return data
    return payload


def validate_proof_features(
    payload: dict[str, Any] | ProofResult,
    *,
    require_equivalent_ready: bool = False,
) -> str | None:
    """Return None when proof-feature obligations are well-formed.

    When ``require_equivalent_ready`` is True (EQUIVALENT gating), any declared
    feature must be supported and fully obligated; otherwise return a reason
    string suitable for warnings / certificate rejection.
    """
    data = _extract_payload(payload)

    raw_features = data.get("proof_features")
    has_features = raw_features is not None
    if has_features and not isinstance(raw_features, list):
        return "proof_features must be a list of feature names"
    features: list[str] = []
    if isinstance(raw_features, list):
        for index, item in enumerate(raw_features):
            if not isinstance(item, str) or not item:
                return f"proof_features[{index}] must be a non-empty string"
            features.append(item)

    obligations_present = {
        key: data.get(key)
        for key in _OBLIGATION_KEYS
        if data.get(key) is not None
    }

    if not features and not obligations_present:
        return None

    if obligations_present and not features:
        orphan = ", ".join(sorted(obligations_present))
        return (
            "obligation block(s) present without proof_features "
            f"({orphan})"
        )

    seen: set[str] = set()
    for feature in features:
        if feature in seen:
            return f"duplicate proof feature {feature!r}"
        seen.add(feature)
        if feature not in KNOWN_PROOF_FEATURES:
            return f"unknown proof feature {feature!r}"

    for feature in features:
        obligation_key = FEATURE_OBLIGATION_KEYS[feature]
        if obligation_key not in data:
            return (
                f"proof feature {feature!r} requires obligation block "
                f"{obligation_key!r}"
            )
        obligation = data[obligation_key]
        if not isinstance(obligation, dict):
            return f"{obligation_key} must be an object"
        if feature == "affine-loop-summary":
            reason = validate_loop_summary_obligation(obligation)
            if reason is not None:
                return reason
        if feature == "relational-induction":
            reason = validate_relational_induction_obligation(obligation)
            if reason is not None:
                return reason
            if require_equivalent_ready and obligation.get("status") != "discharged":
                return (
                    "relational_induction.status must be 'discharged' "
                    "for EQUIVALENT proofs"
                )
        if feature == "memory-loop-summary":
            reason = validate_memory_loop_obligation(obligation)
            if reason is not None:
                return reason
        if feature == "memory-bus":
            reason = validate_memory_bus_obligation(obligation)
            if reason is not None:
                return reason
        if feature == "readonly-image":
            from tools.ppc_equivalence.jump_table_obligations import (
                validate_readonly_image_obligation,
            )

            reason = validate_readonly_image_obligation(obligation)
            if reason is not None:
                return reason
        if feature == "indirect-target-closure":
            from tools.ppc_equivalence.jump_table_obligations import (
                validate_indirect_targets_obligation,
            )

            reason = validate_indirect_targets_obligation(obligation)
            if reason is not None:
                return reason

    for obligation_key, obligation in obligations_present.items():
        if not isinstance(obligation, dict):
            return f"{obligation_key} must be an object"
        expected_features = [
            feature
            for feature, key in FEATURE_OBLIGATION_KEYS.items()
            if key == obligation_key
        ]
        if not any(feature in seen for feature in expected_features):
            return (
                f"obligation block {obligation_key!r} present without a "
                "matching proof_features entry"
            )

    if "affine-loop-summary" in seen:
        reason = validate_loop_summary_obligation(data["loop_summary"])
        if reason is not None:
            return reason

    if require_equivalent_ready:
        for feature in features:
            if feature in UNSUPPORTED_FOR_EQUIVALENT:
                return (
                    f"proof feature {feature!r} is not yet supported for "
                    "EQUIVALENT proofs"
                )

    return None


def enforce_equivalent_proof_features(result: ProofResult) -> ProofResult:
    """Demote EQUIVALENT results with invalid or unsupported proof features."""
    if result.status is not ProofStatus.EQUIVALENT:
        return result

    reason = validate_proof_features(result, require_equivalent_ready=True)
    if reason is None:
        return result

    result.status = ProofStatus.INCONCLUSIVE_UNSUPPORTED
    result.warnings.append(f"proof_features: {reason}")
    if "proof feature" in reason and "not yet supported" in reason:
        result.unsupported.append(reason)
    return result


def proof_features_from_dict(data: dict[str, Any]) -> tuple[
    list[str],
    dict[str, Any] | None,
    dict[str, Any] | None,
]:
    """Back-compat wrapper over :func:`parse_proof_features`.

    Returns ``(features, address_space, indirect_targets)`` only. Prefer
    :class:`ParsedProofFeatures` / :func:`proof_obligations_from_dict` for
    full obligation coverage (loop_summary, relational_induction, memory_loop,
    memory_bus).
    """
    parsed = parse_proof_features(data)
    return (
        list(parsed.features),
        parsed.obligations.get("address_space"),
        parsed.obligations.get("indirect_targets"),
    )
