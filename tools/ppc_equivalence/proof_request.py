"""Canonical ProofRequest identity for PPC equivalence proofs (Track A scaffold).

PR1 scaffolding only — not yet the sole input to ``_prove_bytes``.

Obligation-copy call sites still needing centralization onto
``proof_obligations_from_result`` / ``apply_proof_obligations`` /
``canonical_request_dict`` (integration owner):

- ``tools/coop/lib/equivalence_check.py``
  ``_cache_key`` (binds only address_space/indirect_targets today),
  ``_build_equivalence_certificate`` (manual copies; omits memory_loop /
  memory_bus), and ``_prove_bytes`` premise plumbing.
- ``tools/coop/lib/equivalence_policy.py``
  ``proof_result_from_certificate`` via ``proof_features_from_dict``
  (address_space/indirect_targets only).
- ``tools/ppc_equivalence/provenance.py``
  ``proof_request_identity`` (address_space/indirect_targets only).
- ``tools/ppc_equivalence/result.py``
  ``ProofResult.to_dict`` per-field obligation omit logic.
- ``tools/ppc_equivalence/engine.py``
  ``_finalize`` attaches obligation blocks field-by-field.
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
