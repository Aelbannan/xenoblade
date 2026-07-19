from __future__ import annotations

from typing import Any

from tools.ppc_equivalence.result import ProofResult, ProofStatus

# Canonical proof-feature names and their required top-level obligation keys.
FEATURE_OBLIGATION_KEYS: dict[str, str] = {
    "readonly-image": "address_space",
    "indirect-target-closure": "indirect_targets",
}

KNOWN_PROOF_FEATURES: frozenset[str] = frozenset(FEATURE_OBLIGATION_KEYS)

# Reserved features that may appear in certificates but cannot yet justify
# EQUIVALENT until the engine implements them soundly.
UNSUPPORTED_FOR_EQUIVALENT: frozenset[str] = frozenset(KNOWN_PROOF_FEATURES)

_OBLIGATION_KEYS: frozenset[str] = frozenset(FEATURE_OBLIGATION_KEYS.values())


def _extract_payload(payload: dict[str, Any] | ProofResult) -> dict[str, Any]:
    if isinstance(payload, ProofResult):
        data: dict[str, Any] = {}
        if payload.proof_features:
            data["proof_features"] = list(payload.proof_features)
        if payload.address_space is not None:
            data["address_space"] = payload.address_space
        if payload.indirect_targets is not None:
            data["indirect_targets"] = payload.indirect_targets
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
    """Parse optional proof-feature fields from a certificate or result dict."""
    raw_features = data.get("proof_features")
    features: list[str] = []
    if isinstance(raw_features, list):
        features = [str(item) for item in raw_features if item]

    address_space = data.get("address_space")
    if address_space is not None and not isinstance(address_space, dict):
        address_space = None

    indirect_targets = data.get("indirect_targets")
    if indirect_targets is not None and not isinstance(indirect_targets, dict):
        indirect_targets = None

    return features, address_space, indirect_targets
