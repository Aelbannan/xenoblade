"""Capability-assurance schema and evaluation (Wave 1–2).

Tier A is redefined around promotion-grade attestations for every capability
a proof uses, rather than hard-coding effect-type demotions. Wave 1 lands the
schema, validators, and shadow-mode tier path. Wave 2 adds ``fp-bitwise``
(fmr/fabs/fneg/fnabs) promotion-grade attestation under
``fp-bitwise-ledger-v1``; scalar/compare/convert/trap/fused FP remain
non-promotable.
"""

from __future__ import annotations

import json
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any, Mapping

from tools.ppc_equivalence.provenance import canonical_json_sha256

CAPABILITY_ASSURANCE_SCHEMA_VERSION = 1
CAPABILITY_ASSURANCE_POLICY = "capability-assurance-v1"

STATUS_PROMOTION_GRADE = "promotion-grade"
STATUS_SCOPED_ASSUMPTION = "scoped-assumption"
STATUS_INCOMPLETE = "incomplete"
STATUS_UNMODELED = "unmodeled"

KNOWN_ATTESTATION_STATUSES = frozenset(
    {
        STATUS_PROMOTION_GRADE,
        STATUS_SCOPED_ASSUMPTION,
        STATUS_INCOMPLETE,
        STATUS_UNMODELED,
    }
)

# Weakest → strongest (index order).
_STATUS_RANK = {
    STATUS_UNMODELED: 0,
    STATUS_INCOMPLETE: 1,
    STATUS_SCOPED_ASSUMPTION: 2,
    STATUS_PROMOTION_GRADE: 3,
}

KNOWN_CAPABILITIES = frozenset(
    {
        "integer-core",
        "bounded-memory",
        "assumed-ordinary-ram",
        "certified-calls",
        "precondition-closure",
        "fp-bitwise",
        "fp-load-store",
        "fp-compare",
        "fp-convert",
        "fp-scalar-arithmetic",
        "fp-fused-arithmetic",
        "fp-paired-single",
        "fp-psq",
        "fp-traps",
        "mmio-register-bank",
        "mmio-read-side-effects",
        "mmio-external-input",
        "gx-fifo-write-trace",
        "gx-fifo-read",
        "mmio-loop-emission",
        "mixed-address-space-routing",
        "dma-interrupt-effects",
        "domain-exception",
        "provenance",
    }
)

KNOWN_ATTESTATION_ALGORITHMS = frozenset(
    {
        "opcode-ledger-v2",
        "provenance-binding-v1",
        "fp-bitwise-ledger-v1",
        # Wave 2 certified-calls refinement; promotion-grade only with real UNSAT.
        "certified-calls-refinement-v1",
        # Wave 2: independent address-coverage / wraparound UNSAT digests.
        "bounded-memory-coverage-v2",
        # Shadow / incomplete only — never promotion-grade.
        "legacy-effect-gate-v1",
    }
)

PROVENANCE_REQUIRED_EVIDENCE_FIELDS = (
    "engine_hash",
    "certifier_hash",
    "source_hash",
    "git_commit",
)


@dataclass(frozen=True)
class CapabilityAttestation:
    capability: str
    model_version: str
    algorithm: str
    status: str
    assumptions: tuple[str, ...]
    unsupported: tuple[str, ...]
    evidence: dict[str, Any]
    attestation_sha256: str

    def to_dict(self) -> dict[str, Any]:
        return {
            "capability": self.capability,
            "model_version": self.model_version,
            "algorithm": self.algorithm,
            "status": self.status,
            "assumptions": list(self.assumptions),
            "unsupported": list(self.unsupported),
            "evidence": dict(self.evidence),
            "attestation_sha256": self.attestation_sha256,
        }

    @classmethod
    def from_dict(cls, data: Mapping[str, Any]) -> "CapabilityAttestation":
        if not isinstance(data, Mapping):
            raise TypeError("capability attestation must be a mapping")
        # Caller-supplied status is stored but never trusted for tiering;
        # evaluate_capability_assurance recomputes grades from evidence.
        status = str(data.get("status", STATUS_INCOMPLETE))
        assumptions = data.get("assumptions", ())
        unsupported = data.get("unsupported", ())
        evidence = data.get("evidence", {})
        if not isinstance(evidence, dict):
            raise TypeError("capability attestation evidence must be a dict")
        return cls(
            capability=str(data.get("capability", "")),
            model_version=str(data.get("model_version", "")),
            algorithm=str(data.get("algorithm", "")),
            status=status,
            assumptions=tuple(str(item) for item in assumptions),
            unsupported=tuple(str(item) for item in unsupported),
            evidence=dict(evidence),
            attestation_sha256=str(data.get("attestation_sha256", "")),
        )

    def without_hash(self) -> dict[str, Any]:
        payload = self.to_dict()
        payload.pop("attestation_sha256", None)
        return payload


def attestation_digest(attestation_without_hash: Mapping[str, Any] | CapabilityAttestation) -> str:
    """SHA-256 over canonical JSON of an attestation excluding its digest field."""
    if isinstance(attestation_without_hash, CapabilityAttestation):
        payload = attestation_without_hash.without_hash()
    else:
        payload = {
            key: value
            for key, value in dict(attestation_without_hash).items()
            if key != "attestation_sha256"
        }
    return canonical_json_sha256(payload)


def build_attestation(
    *,
    capability: str,
    model_version: str,
    algorithm: str,
    status: str = STATUS_INCOMPLETE,
    assumptions: tuple[str, ...] | list[str] = (),
    unsupported: tuple[str, ...] | list[str] = (),
    evidence: dict[str, Any] | None = None,
) -> CapabilityAttestation:
    """Construct an attestation with a fresh digest (status is advisory only)."""
    draft = CapabilityAttestation(
        capability=capability,
        model_version=model_version,
        algorithm=algorithm,
        status=status,
        assumptions=tuple(assumptions),
        unsupported=tuple(unsupported),
        evidence=dict(evidence or {}),
        attestation_sha256="",
    )
    digest = attestation_digest(draft)
    return CapabilityAttestation(
        capability=draft.capability,
        model_version=draft.model_version,
        algorithm=draft.algorithm,
        status=draft.status,
        assumptions=draft.assumptions,
        unsupported=draft.unsupported,
        evidence=draft.evidence,
        attestation_sha256=digest,
    )


@dataclass(frozen=True)
class CapabilityAssurance:
    schema_version: int = CAPABILITY_ASSURANCE_SCHEMA_VERSION
    policy: str = CAPABILITY_ASSURANCE_POLICY
    capabilities: tuple[CapabilityAttestation, ...] = ()

    def to_dict(self) -> dict[str, Any]:
        return {
            "schema_version": self.schema_version,
            "policy": self.policy,
            "capabilities": [item.to_dict() for item in self.capabilities],
        }

    @classmethod
    def from_dict(cls, data: Mapping[str, Any] | None) -> "CapabilityAssurance":
        if data is None:
            raise TypeError("capability_assurance must be a mapping")
        if not isinstance(data, Mapping):
            raise TypeError("capability_assurance must be a mapping")
        raw_caps = data.get("capabilities", [])
        if not isinstance(raw_caps, list):
            raise TypeError("capability_assurance.capabilities must be a list")
        return cls(
            schema_version=int(data.get("schema_version", CAPABILITY_ASSURANCE_SCHEMA_VERSION)),
            policy=str(data.get("policy", CAPABILITY_ASSURANCE_POLICY)),
            capabilities=tuple(CapabilityAttestation.from_dict(item) for item in raw_caps),
        )

    def validate_structure(self) -> None:
        """Strict schema check; unknown schema/algorithm/capability fail closed."""
        if self.schema_version != CAPABILITY_ASSURANCE_SCHEMA_VERSION:
            raise ValueError(
                f"unsupported capability_assurance schema_version {self.schema_version}"
            )
        if self.policy != CAPABILITY_ASSURANCE_POLICY:
            raise ValueError(f"unsupported capability_assurance policy {self.policy!r}")
        seen: set[str] = set()
        for item in self.capabilities:
            if item.capability not in KNOWN_CAPABILITIES:
                raise ValueError(f"unknown capability {item.capability!r}")
            if item.capability in seen:
                raise ValueError(f"duplicate capability attestation {item.capability!r}")
            seen.add(item.capability)
            if item.algorithm not in KNOWN_ATTESTATION_ALGORITHMS:
                raise ValueError(f"unknown attestation algorithm {item.algorithm!r}")
            if item.status not in KNOWN_ATTESTATION_STATUSES:
                raise ValueError(f"unknown attestation status {item.status!r}")
            if not item.model_version:
                raise ValueError(f"missing model_version for {item.capability!r}")
            expected = attestation_digest(item)
            if item.attestation_sha256 != expected:
                raise ValueError(
                    f"attestation_sha256 mismatch for {item.capability!r}"
                )


@dataclass(frozen=True)
class CapabilityManifest:
    """Promotion allowlist and shadow-mode knobs for capability assurance."""

    schema_version: int = CAPABILITY_ASSURANCE_SCHEMA_VERSION
    policy: str = CAPABILITY_ASSURANCE_POLICY
    allowed_tier_a_capabilities: dict[str, tuple[str, ...]] = field(default_factory=dict)
    shadow_mode: bool = True
    require_capability_assurance: bool = False

    def allowed_versions(self, capability: str) -> tuple[str, ...]:
        return tuple(self.allowed_tier_a_capabilities.get(capability, ()))

    def to_dict(self) -> dict[str, Any]:
        return {
            "schema_version": self.schema_version,
            "policy": self.policy,
            "allowed_tier_a_capabilities": {
                key: list(values)
                for key, values in sorted(self.allowed_tier_a_capabilities.items())
            },
            "shadow_mode": self.shadow_mode,
            "require_capability_assurance": self.require_capability_assurance,
        }

    @classmethod
    def from_dict(cls, data: Mapping[str, Any] | None) -> "CapabilityManifest":
        if not data:
            return cls()
        raw_allowed = data.get("allowed_tier_a_capabilities", {})
        allowed: dict[str, tuple[str, ...]] = {}
        if isinstance(raw_allowed, dict):
            for key, values in raw_allowed.items():
                if isinstance(values, list):
                    allowed[str(key)] = tuple(str(item) for item in values)
                elif values is None:
                    allowed[str(key)] = ()
                else:
                    raise TypeError(
                        f"allowed_tier_a_capabilities[{key!r}] must be a list"
                    )
        return cls(
            schema_version=int(
                data.get("schema_version", CAPABILITY_ASSURANCE_SCHEMA_VERSION)
            ),
            policy=str(data.get("policy", CAPABILITY_ASSURANCE_POLICY)),
            allowed_tier_a_capabilities=allowed,
            shadow_mode=bool(data.get("shadow_mode", True)),
            require_capability_assurance=bool(
                data.get("require_capability_assurance", False)
            ),
        )


def default_capability_manifest_path() -> Path:
    return (
        Path(__file__).resolve().parents[1]
        / "coop"
        / "capability_manifest.json"
    )


def load_capability_manifest(path: Path | None = None) -> CapabilityManifest:
    target = path if path is not None else default_capability_manifest_path()
    if target is None or not target.is_file():
        return CapabilityManifest()
    data = json.loads(target.read_text(encoding="utf-8"))
    if not isinstance(data, dict):
        raise ValueError(f"capability manifest {target} must be a JSON object")
    return CapabilityManifest.from_dict(data)


@dataclass(frozen=True)
class CapabilityAssuranceResult:
    attestations: tuple[CapabilityAttestation, ...]
    has_unmodeled_or_unproven_capability: bool
    has_explicit_scoped_assumptions: bool
    all_used_capabilities_promotion_grade: bool
    weakest_status: str
    shadow_legacy_tier: str | None
    recomputed_statuses: dict[str, str]


def compute_confidence_tier_from_assurance(
    assurance_result: CapabilityAssuranceResult,
) -> str | None:
    """Map an assurance evaluation to A/B/C (None only when caller supplies it)."""
    if assurance_result.has_unmodeled_or_unproven_capability:
        return "C"
    if assurance_result.has_explicit_scoped_assumptions:
        return "B"
    if assurance_result.all_used_capabilities_promotion_grade:
        return "A"
    return "C"


def _normalize_assurance(
    raw: CapabilityAssurance | Mapping[str, Any] | None,
) -> CapabilityAssurance | None:
    if raw is None:
        return None
    if isinstance(raw, CapabilityAssurance):
        return raw
    return CapabilityAssurance.from_dict(raw)


def _proof_effect_flags(result: Any) -> dict[str, bool]:
    """Mirror legacy effect detection without importing coop policy (avoid cycles)."""
    from tools.ppc_equivalence.memory_profile import MemoryProfile
    from tools.ppc_equivalence.result import (
        FP_COVERAGE_STATUS_ASSUMED,
        FP_COVERAGE_STATUS_NONE,
        proof_fp_coverage_status,
    )

    fp_coverage = proof_fp_coverage_status(result)
    has_fp = fp_coverage != FP_COVERAGE_STATUS_NONE
    has_assumed_fp = fp_coverage == FP_COVERAGE_STATUS_ASSUMED
    has_callees = bool(getattr(result, "assumed_callees", None))
    observables = getattr(result, "observables", None) or []
    has_memory_access = "memory" in observables
    proof_features = getattr(result, "proof_features", None) or []
    has_memory_bus = (
        "memory-bus" in proof_features
        or getattr(result, "memory_bus", None) is not None
    )
    assumptions = getattr(result, "assumptions", None) or []
    has_domain_exceptions = (
        getattr(result, "counterexample_kind", None) == "definedness"
        or bool(getattr(result, "invalid_reasons", None))
        or any(str(item).startswith("domain-exception:") for item in assumptions)
    )
    environment = getattr(result, "environment", None)
    has_assumed_ram = (
        has_memory_access
        and environment is not None
        and getattr(environment, "profile", None) is not None
        and environment.profile.value == MemoryProfile.ASSUMED_ORDINARY_RAM.value
    )
    has_complete_provenance = bool(
        getattr(result, "engine_hash", None)
        and getattr(result, "certifier_hash", None)
        and getattr(result, "source_hash", None)
        and getattr(result, "git_commit", None)
        and not bool(getattr(result, "git_dirty", False))
    )
    return {
        "has_fp": has_fp or has_assumed_fp,
        "has_callees": has_callees,
        "has_memory_access": has_memory_access,
        "has_memory_bus": has_memory_bus,
        "has_domain_exceptions": has_domain_exceptions,
        "has_assumed_ram": has_assumed_ram,
        "has_complete_provenance": has_complete_provenance,
    }


def infer_used_capabilities(result: Any) -> frozenset[str]:
    """Capabilities implied by proof effects (Wave 1–2 inference)."""
    from tools.ppc_equivalence.fp_bitwise import classify_fp_capabilities

    flags = _proof_effect_flags(result)
    used: set[str] = {"integer-core", "provenance"}
    if flags["has_domain_exceptions"]:
        used.add("domain-exception")
    if flags["has_assumed_ram"]:
        used.add("assumed-ordinary-ram")
    if flags["has_callees"]:
        used.add("certified-calls")
    if flags["has_memory_bus"]:
        # Coarse MMIO demand — Wave 1 fails closed without a matching attestation.
        used.add("mmio-register-bank")
    if flags["has_fp"]:
        opcodes = getattr(result, "opcodes_used", None) or []
        fp_caps = classify_fp_capabilities(opcodes)
        if fp_caps:
            used.update(fp_caps)
        else:
            # FP domain / coverage present but opcodes not classifiable — fail closed.
            used.add("fp-scalar-arithmetic")
    elif flags["has_memory_access"] and not flags["has_assumed_ram"]:
        used.add("bounded-memory")
    return frozenset(used)


def _recompute_attestation_status(
    attestation: CapabilityAttestation,
    *,
    ledger: Any | None,
    manifest: CapabilityManifest,
) -> str:
    """Independently recompute grade; ignore caller-supplied status."""
    if attestation.algorithm == "legacy-effect-gate-v1":
        return STATUS_INCOMPLETE

    if attestation.capability == "assumed-ordinary-ram":
        # Assumed RAM is never promotion-grade under capability-assurance-v1.
        return STATUS_SCOPED_ASSUMPTION if attestation.assumptions else STATUS_INCOMPLETE

    if attestation.algorithm == "opcode-ledger-v2":
        if attestation.capability != "integer-core":
            return STATUS_INCOMPLETE
        opcodes = attestation.evidence.get("opcodes")
        if not isinstance(opcodes, list) or not opcodes:
            return STATUS_INCOMPLETE
        opcode_list = [str(op) for op in opcodes]
        if ledger is None:
            return STATUS_INCOMPLETE
        missing = []
        if hasattr(ledger, "missing_dolphin_opcodes"):
            missing = list(ledger.missing_dolphin_opcodes(opcode_list))
        elif hasattr(ledger, "dolphin_validated_opcodes"):
            validated = set(ledger.dolphin_validated_opcodes)
            missing = sorted(op for op in opcode_list if op not in validated)
        else:
            return STATUS_INCOMPLETE
        if missing:
            return STATUS_INCOMPLETE
        if hasattr(ledger, "is_absent") and ledger.is_absent():
            return STATUS_INCOMPLETE
        if hasattr(ledger, "promotion_metadata_invalid") and ledger.promotion_metadata_invalid():
            return STATUS_INCOMPLETE
        allowed = manifest.allowed_versions(attestation.capability)
        if attestation.model_version not in allowed:
            return STATUS_INCOMPLETE
        if attestation.unsupported:
            return STATUS_INCOMPLETE
        if attestation.assumptions:
            return STATUS_SCOPED_ASSUMPTION
        return STATUS_PROMOTION_GRADE

    if attestation.algorithm == "provenance-binding-v1":
        if attestation.capability != "provenance":
            return STATUS_INCOMPLETE
        evidence = attestation.evidence
        for field_name in PROVENANCE_REQUIRED_EVIDENCE_FIELDS:
            value = evidence.get(field_name)
            if not isinstance(value, str) or not value:
                return STATUS_INCOMPLETE
        if bool(evidence.get("git_dirty")):
            return STATUS_INCOMPLETE
        allowed_engine = evidence.get("allowed_engine_sha256")
        if isinstance(allowed_engine, str) and allowed_engine:
            if evidence.get("engine_hash") != allowed_engine:
                return STATUS_INCOMPLETE
        allowed = manifest.allowed_versions(attestation.capability)
        if attestation.model_version not in allowed:
            return STATUS_INCOMPLETE
        if attestation.unsupported:
            return STATUS_INCOMPLETE
        if attestation.assumptions:
            return STATUS_SCOPED_ASSUMPTION
        return STATUS_PROMOTION_GRADE

    if attestation.algorithm == "fp-bitwise-ledger-v1":
        return _recompute_fp_bitwise_status(
            attestation, ledger=ledger, manifest=manifest
        )

    if attestation.algorithm == "certified-calls-refinement-v1":
        if attestation.capability != "certified-calls":
            return STATUS_INCOMPLETE
        allowed = manifest.allowed_versions(attestation.capability)
        if attestation.model_version not in allowed:
            return STATUS_INCOMPLETE
        from tools.ppc_equivalence.certified_calls_obligations import (
            evaluate_certified_calls_status,
        )

        obligation = attestation.evidence.get("obligation")
        rejection_reasons = attestation.evidence.get("rejection_reasons") or ()
        if not isinstance(rejection_reasons, (list, tuple)):
            return STATUS_INCOMPLETE
        # Opaque EABI / incomplete refinements never promote, even if allowlisted.
        return evaluate_certified_calls_status(
            obligation if isinstance(obligation, dict) else None,
            rejection_reasons=tuple(str(item) for item in rejection_reasons),
            assumptions=attestation.assumptions,
        )

    if attestation.algorithm == "bounded-memory-coverage-v2":
        if attestation.capability != "bounded-memory":
            return STATUS_INCOMPLETE
        from tools.ppc_equivalence.bounded_memory_obligations import (
            recompute_bounded_memory_attestation_status,
        )

        return recompute_bounded_memory_attestation_status(
            attestation.evidence,
            assumptions=attestation.assumptions,
            unsupported=attestation.unsupported,
            model_version=attestation.model_version,
            allowed_versions=manifest.allowed_versions(attestation.capability),
        )

    # Unknown algorithms are rejected by validate_structure; treat as incomplete.
    return STATUS_INCOMPLETE


def _recompute_fp_bitwise_status(
    attestation: CapabilityAttestation,
    *,
    ledger: Any | None,
    manifest: CapabilityManifest,
) -> str:
    """Grade ``fp-bitwise`` from evidence + ledger; never trust caller status."""
    from tools.ppc_equivalence.fp_bitwise import (
        FP_BITWISE_MODEL_VERSION,
        FP_BITWISE_OPS,
    )

    if attestation.capability != "fp-bitwise":
        return STATUS_INCOMPLETE
    if attestation.model_version != FP_BITWISE_MODEL_VERSION:
        # Allowlist check below also gates; mismatch vs known model is incomplete.
        pass
    opcodes = attestation.evidence.get("opcodes")
    if not isinstance(opcodes, list) or not opcodes:
        return STATUS_INCOMPLETE
    opcode_list = [str(op) for op in opcodes]
    if any(op not in FP_BITWISE_OPS for op in opcode_list):
        return STATUS_INCOMPLETE
    # Explicit no-host-float claim required for promotion-grade bitwise.
    if attestation.evidence.get("host_float") is not False:
        return STATUS_INCOMPLETE
    ledger_sha = attestation.evidence.get("ledger_sha256")
    if not isinstance(ledger_sha, str) or not ledger_sha:
        return STATUS_INCOMPLETE
    if ledger is None:
        return STATUS_INCOMPLETE
    if hasattr(ledger, "is_absent") and ledger.is_absent():
        return STATUS_INCOMPLETE
    if hasattr(ledger, "promotion_metadata_invalid") and ledger.promotion_metadata_invalid():
        return STATUS_INCOMPLETE
    missing: list[str] = []
    if hasattr(ledger, "missing_fp_bitwise_opcodes"):
        missing = list(ledger.missing_fp_bitwise_opcodes(opcode_list))
    elif hasattr(ledger, "missing_dolphin_opcodes"):
        missing = list(ledger.missing_dolphin_opcodes(opcode_list))
    elif hasattr(ledger, "dolphin_validated_opcodes"):
        validated = set(ledger.dolphin_validated_opcodes)
        missing = sorted(op for op in opcode_list if op not in validated)
    else:
        return STATUS_INCOMPLETE
    if missing:
        return STATUS_INCOMPLETE
    allowed = manifest.allowed_versions(attestation.capability)
    if attestation.model_version not in allowed:
        return STATUS_INCOMPLETE
    # Unsupported remainder must be empty (UNSAT / none).
    if attestation.unsupported:
        return STATUS_INCOMPLETE
    if attestation.assumptions:
        return STATUS_SCOPED_ASSUMPTION
    return STATUS_PROMOTION_GRADE


def _weakest(statuses: list[str]) -> str:
    if not statuses:
        return STATUS_UNMODELED
    return min(statuses, key=lambda item: _STATUS_RANK.get(item, 0))


def evaluate_capability_assurance(
    result: Any,
    ledger: Any | None = None,
    manifest: CapabilityManifest | Mapping[str, Any] | None = None,
    *,
    shadow_legacy_tier: str | None = None,
) -> CapabilityAssuranceResult:
    """Evaluate attestations against used capabilities; recompute all statuses."""
    if isinstance(manifest, CapabilityManifest) or manifest is None:
        resolved_manifest = manifest or CapabilityManifest()
    else:
        resolved_manifest = CapabilityManifest.from_dict(manifest)

    raw = getattr(result, "capability_assurance", None)
    assurance: CapabilityAssurance | None = None
    structural_error = False
    if raw is not None:
        try:
            assurance = _normalize_assurance(raw)
            assurance.validate_structure()
        except (TypeError, ValueError, KeyError):
            structural_error = True
            assurance = None

    used = infer_used_capabilities(result)
    recomputed: dict[str, str] = {}
    recomputed_attestations: list[CapabilityAttestation] = []

    if structural_error:
        for capability in sorted(used):
            recomputed[capability] = STATUS_UNMODELED
        return CapabilityAssuranceResult(
            attestations=(),
            has_unmodeled_or_unproven_capability=True,
            has_explicit_scoped_assumptions=False,
            all_used_capabilities_promotion_grade=False,
            weakest_status=STATUS_UNMODELED,
            shadow_legacy_tier=shadow_legacy_tier,
            recomputed_statuses=recomputed,
        )

    attested_by_name: dict[str, CapabilityAttestation] = {}
    if assurance is not None:
        for item in assurance.capabilities:
            status = _recompute_attestation_status(
                item, ledger=ledger, manifest=resolved_manifest
            )
            recomputed[item.capability] = status
            recomputed_attestations.append(
                CapabilityAttestation(
                    capability=item.capability,
                    model_version=item.model_version,
                    algorithm=item.algorithm,
                    status=status,
                    assumptions=item.assumptions,
                    unsupported=item.unsupported,
                    evidence=dict(item.evidence),
                    attestation_sha256=item.attestation_sha256,
                )
            )
            attested_by_name[item.capability] = recomputed_attestations[-1]

    # Legacy certificates without capability_assurance: do not invent attestations.
    if assurance is None:
        for capability in sorted(used):
            recomputed[capability] = STATUS_UNMODELED
        return CapabilityAssuranceResult(
            attestations=(),
            has_unmodeled_or_unproven_capability=True,
            has_explicit_scoped_assumptions=False,
            all_used_capabilities_promotion_grade=False,
            weakest_status=STATUS_UNMODELED,
            shadow_legacy_tier=shadow_legacy_tier,
            recomputed_statuses=recomputed,
        )

    for capability in used:
        if capability not in attested_by_name:
            recomputed[capability] = STATUS_UNMODELED

    used_statuses = [recomputed[name] for name in used]
    weakest = _weakest(used_statuses)
    has_unmodeled = any(
        recomputed.get(name) in {STATUS_UNMODELED, STATUS_INCOMPLETE}
        for name in used
    )
    has_scoped = any(
        recomputed.get(name) == STATUS_SCOPED_ASSUMPTION for name in used
    )
    all_promotion = bool(used) and all(
        recomputed.get(name) == STATUS_PROMOTION_GRADE for name in used
    )

    return CapabilityAssuranceResult(
        attestations=tuple(recomputed_attestations),
        has_unmodeled_or_unproven_capability=has_unmodeled,
        has_explicit_scoped_assumptions=has_scoped and not has_unmodeled,
        all_used_capabilities_promotion_grade=all_promotion,
        weakest_status=weakest,
        shadow_legacy_tier=shadow_legacy_tier,
        recomputed_statuses=recomputed,
    )


def draft_integer_core_assurance(
    result: Any,
    *,
    ledger: Any | None = None,
    ledger_sha256: str | None = None,
) -> CapabilityAssurance | None:
    """Optional non-authoritative draft for EQUIVALENT integer / fp-bitwise proofs.

    Status is advisory; ``evaluate_capability_assurance`` recomputes the grade.
    Returns ``None`` when non-bitwise FP / memory-bus effects are present or
    opcodes missing. When only ``fmr``/``fabs``/``fneg``/``fnabs`` FP ops appear,
    emits ``integer-core`` plus ``fp-bitwise`` attestations.
    """
    from tools.ppc_equivalence.fp_bitwise import (
        FP_BITWISE_ALGORITHM,
        FP_BITWISE_MODEL_VERSION,
        FP_BITWISE_OPS,
        is_fp_bitwise_only,
        non_bitwise_fp_opcodes,
    )
    from tools.ppc_equivalence.result import ProofStatus

    if getattr(result, "status", None) is not ProofStatus.EQUIVALENT:
        return None
    flags = _proof_effect_flags(result)
    if flags["has_memory_bus"]:
        return None
    opcodes = [str(op) for op in (getattr(result, "opcodes_used", None) or [])]
    if not opcodes:
        return None

    if flags["has_fp"]:
        if non_bitwise_fp_opcodes(opcodes) or not is_fp_bitwise_only(opcodes):
            return None

    # Do not replace an existing integer-core / fp-bitwise attestation block;
    # callers merge via maybe_attach_integer_core_draft.
    existing = getattr(result, "capability_assurance", None)
    if existing is not None:
        try:
            prior = _normalize_assurance(existing)
            names = {item.capability for item in prior.capabilities}
            if "integer-core" in names or "fp-bitwise" in names:
                return None
        except (TypeError, ValueError, KeyError):
            pass

    resolved_ledger_sha = ledger_sha256
    if not resolved_ledger_sha and ledger is not None:
        if hasattr(ledger, "dolphin_validated_opcodes"):
            resolved_ledger_sha = canonical_json_sha256(
                sorted(str(op) for op in ledger.dolphin_validated_opcodes)
            )

    integer_opcodes = sorted(op for op in opcodes if op not in FP_BITWISE_OPS)
    if not integer_opcodes:
        integer_opcodes = sorted(opcodes)

    integer_evidence: dict[str, Any] = {"opcodes": integer_opcodes}
    if resolved_ledger_sha:
        integer_evidence["ledger_sha256"] = resolved_ledger_sha
    attestations = [
        build_attestation(
            capability="integer-core",
            model_version="integer-core-v1",
            algorithm="opcode-ledger-v2",
            status=STATUS_INCOMPLETE,
            evidence=integer_evidence,
        )
    ]

    if flags["has_fp"] and is_fp_bitwise_only(opcodes):
        bitwise_ops = sorted(op for op in opcodes if op in FP_BITWISE_OPS)
        fp_evidence: dict[str, Any] = {
            "opcodes": bitwise_ops,
            "host_float": False,
        }
        if resolved_ledger_sha:
            fp_evidence["ledger_sha256"] = resolved_ledger_sha
        attestations.append(
            build_attestation(
                capability="fp-bitwise",
                model_version=FP_BITWISE_MODEL_VERSION,
                algorithm=FP_BITWISE_ALGORITHM,
                status=STATUS_INCOMPLETE,
                evidence=fp_evidence,
            )
        )

    return CapabilityAssurance(
        schema_version=CAPABILITY_ASSURANCE_SCHEMA_VERSION,
        policy=CAPABILITY_ASSURANCE_POLICY,
        capabilities=tuple(attestations),
    )


def draft_provenance_attestation(
    result: Any,
    *,
    allowed_engine_sha256: str | None = None,
    ledger_sha256: str | None = None,
) -> CapabilityAttestation | None:
    """Draft a provenance-binding attestation from proof hash fields.

    Status is advisory; ``evaluate_capability_assurance`` recomputes the grade.
    """
    from tools.ppc_equivalence.result import ProofStatus

    if getattr(result, "status", None) is not ProofStatus.EQUIVALENT:
        return None

    engine_hash = str(getattr(result, "engine_hash", "") or "")
    certifier_hash = str(getattr(result, "certifier_hash", "") or "")
    source_hash = str(getattr(result, "source_hash", "") or "")
    proof_request = str(getattr(result, "proof_request_hash", "") or "") or source_hash
    ledger_hash = str(
        getattr(result, "validation_ledger_hash", "") or ledger_sha256 or ""
    )
    git_commit = str(getattr(result, "git_commit", "") or "")
    git_dirty = bool(getattr(result, "git_dirty", False))

    evidence: dict[str, Any] = {
        "engine_hash": engine_hash,
        "certifier_hash": certifier_hash,
        "source_hash": source_hash,
        "proof_request_hash": proof_request,
        "validation_ledger_hash": ledger_hash,
        "git_commit": git_commit,
        "git_dirty": git_dirty,
    }
    if allowed_engine_sha256:
        evidence["allowed_engine_sha256"] = allowed_engine_sha256

    return build_attestation(
        capability="provenance",
        model_version="provenance-v1",
        algorithm="provenance-binding-v1",
        status=STATUS_INCOMPLETE,
        evidence=evidence,
    )


def _merge_assurance_attestation(
    existing: Any | None,
    attestation: CapabilityAttestation,
) -> CapabilityAssurance:
    """Replace any prior attestation for the same capability and keep the rest."""
    capabilities: list[CapabilityAttestation] = []
    if existing is not None:
        assurance = (
            existing
            if isinstance(existing, CapabilityAssurance)
            else CapabilityAssurance.from_dict(existing)
        )
        capabilities = [
            item
            for item in assurance.capabilities
            if item.capability != attestation.capability
        ]
    capabilities.append(attestation)
    return CapabilityAssurance(
        schema_version=CAPABILITY_ASSURANCE_SCHEMA_VERSION,
        policy=CAPABILITY_ASSURANCE_POLICY,
        capabilities=tuple(capabilities),
    )


def maybe_attach_integer_core_draft(
    result: Any,
    *,
    ledger: Any | None = None,
    ledger_sha256: str | None = None,
) -> Any:
    """Attach a draft integer-core / fp-bitwise assurance block when applicable."""
    draft = draft_integer_core_assurance(
        result, ledger=ledger, ledger_sha256=ledger_sha256
    )
    if draft is None:
        return result
    # Merge with any prior attestations (e.g. provenance) rather than replace.
    existing = getattr(result, "capability_assurance", None)
    merged = draft
    if existing is not None:
        try:
            prior = _normalize_assurance(existing)
            by_name = {item.capability: item for item in prior.capabilities}
            for item in draft.capabilities:
                by_name[item.capability] = item
            merged = CapabilityAssurance(
                schema_version=CAPABILITY_ASSURANCE_SCHEMA_VERSION,
                policy=CAPABILITY_ASSURANCE_POLICY,
                capabilities=tuple(by_name[name] for name in sorted(by_name)),
            )
        except (TypeError, ValueError, KeyError):
            merged = draft
    result.capability_assurance = merged.to_dict()
    return result


def draft_bounded_memory_assurance(
    result: Any,
    *,
    obligation: Mapping[str, Any] | None = None,
) -> CapabilityAssurance | None:
    """Optional bounded-memory attestation when a v2 obligation is available.

    Status is advisory; ``evaluate_capability_assurance`` recomputes the grade.
    Assumed-ordinary-ram proofs never receive a promotion-grade bounded-memory
    attestation from this helper.
    """
    from tools.ppc_equivalence.bounded_memory_obligations import (
        SOURCE_ASSUMED,
        build_bounded_memory_attestation,
        classify_range_source,
    )
    from tools.ppc_equivalence.memory_profile import MemoryProfile
    from tools.ppc_equivalence.result import ProofStatus

    if getattr(result, "status", None) is not ProofStatus.EQUIVALENT:
        return None
    environment = getattr(result, "environment", None)
    if environment is not None and getattr(environment, "profile", None) is not None:
        if environment.profile == MemoryProfile.ASSUMED_ORDINARY_RAM:
            return None
    if obligation is None:
        return None
    if obligation.get("source") == SOURCE_ASSUMED:
        return None
    # Prefer explicit classify when environment present.
    if environment is not None and classify_range_source(environment=environment) == SOURCE_ASSUMED:
        return None
    attestation = build_bounded_memory_attestation(obligation)
    return _merge_assurance_attestation(
        getattr(result, "capability_assurance", None),
        attestation,
    )


def maybe_attach_provenance_draft(
    result: Any,
    *,
    allowed_engine_sha256: str | None = None,
    ledger_sha256: str | None = None,
) -> Any:
    """Attach or merge a provenance attestation on EQUIVALENT proofs (mutates result)."""
    if ledger_sha256 and not getattr(result, "validation_ledger_hash", ""):
        result.validation_ledger_hash = ledger_sha256
    if not getattr(result, "proof_request_hash", "") and getattr(result, "source_hash", ""):
        result.proof_request_hash = str(result.source_hash)

    attestation = draft_provenance_attestation(
        result,
        allowed_engine_sha256=allowed_engine_sha256,
        ledger_sha256=ledger_sha256,
    )
    if attestation is None:
        return result
    merged = _merge_assurance_attestation(
        getattr(result, "capability_assurance", None),
        attestation,
    )
    result.capability_assurance = merged.to_dict()
    return result
