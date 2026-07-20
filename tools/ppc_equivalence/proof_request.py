"""Canonical ProofRequest identity for PPC equivalence proofs.

Obligation identity and copy helpers live in ``proof_features``;
``ProofRequest`` / ``cache_key`` / ``proof_request_hash`` provide the
canonical request bundle. Coop wiring uses these via:

- ``equivalence_check._cache_key`` / ``_proof_audit_dict`` /
  ``_build_equivalence_certificate`` / ``_cache_get`` (ProofResult restore)
- ``equivalence_policy.proof_result_from_certificate`` /
  ``classify_for_promotion_legacy`` (dict → ProofResult)
- ``provenance.proof_request_identity`` (all ``PROOF_OBLIGATION_FIELDS``)
"""

from __future__ import annotations

from dataclasses import dataclass, field
from typing import Any

from tools.ppc_equivalence.provenance import (
    canonical_json_sha256,
    canonical_obligation_dict,
)


@dataclass(frozen=True)
class ProofRequest:
    original_hex: str
    candidate_hex: str
    original_base: int
    candidate_base: int
    contract: str
    observables: tuple[str, ...]
    limits: dict[str, int]
    memory_environment: dict[str, Any]
    floating_point_domain: dict[str, Any] | None
    assumed_callees: tuple[int | str, ...]
    callee_contracts: dict[str, dict[str, Any]]
    relocations: dict[str, list[dict[str, Any]]]
    proof_features: tuple[str, ...] = ()
    obligations: dict[str, dict[str, Any]] = field(default_factory=dict)


def _canonical_nested(value: Any) -> Any:
    if isinstance(value, dict):
        return {str(key): _canonical_nested(item) for key, item in sorted(value.items(), key=lambda pair: str(pair[0]))}
    if isinstance(value, list):
        return [_canonical_nested(item) for item in value]
    if isinstance(value, tuple):
        return [_canonical_nested(item) for item in value]
    return value


def canonical_request_dict(request: ProofRequest) -> dict[str, Any]:
    """Return a deep-canonical dict suitable for stable hashing."""
    obligations = {
        key: canonical_obligation_dict(value)
        for key, value in sorted(request.obligations.items())
        if isinstance(value, dict)
    }
    relocations = {
        str(side): [
            canonical_obligation_dict(entry) if isinstance(entry, dict) else entry
            for entry in entries
        ]
        for side, entries in sorted(request.relocations.items(), key=lambda pair: str(pair[0]))
    }
    return {
        "original_hex": request.original_hex,
        "candidate_hex": request.candidate_hex,
        "original_base": int(request.original_base),
        "candidate_base": int(request.candidate_base),
        "contract": request.contract,
        "observables": sorted(request.observables),
        "limits": {
            str(key): int(value)
            for key, value in sorted(request.limits.items(), key=lambda pair: str(pair[0]))
        },
        "memory_environment": _canonical_nested(request.memory_environment),
        "floating_point_domain": (
            None
            if request.floating_point_domain is None
            else _canonical_nested(request.floating_point_domain)
        ),
        "assumed_callees": sorted(request.assumed_callees, key=str),
        "callee_contracts": {
            str(name): _canonical_nested(contract)
            for name, contract in sorted(
                request.callee_contracts.items(), key=lambda pair: str(pair[0])
            )
        },
        "relocations": relocations,
        "proof_features": sorted(request.proof_features),
        "obligations": obligations,
    }


def proof_request_hash(request: ProofRequest) -> str:
    """SHA-256 of :func:`canonical_request_dict`."""
    return canonical_json_sha256(canonical_request_dict(request))


def cache_key(request: ProofRequest, engine_hash: str, certifier_hash: str) -> str:
    """Cache identity: canonical request plus engine/certifier provenance."""
    return canonical_json_sha256(
        {
            "request": canonical_request_dict(request),
            "engine_hash": engine_hash,
            "certifier_hash": certifier_hash,
        }
    )
