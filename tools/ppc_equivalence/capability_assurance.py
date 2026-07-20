"""Capability-assurance schema and evaluation (Wave 1–5).

Tier A is redefined around promotion-grade attestations for every capability
a proof uses, rather than hard-coding effect-type demotions. Wave 1 lands the
schema, validators, and shadow-mode tier path. Wave 2 adds ``fp-bitwise``
(fmr/fabs/fneg/fnabs) promotion-grade attestation under
``fp-bitwise-ledger-v1``. Wave 3 splits remaining FP into distinct
sub-capabilities (load/store, compare, convert, scalar, fused, paired, psq,
traps) with fail-closed stubs; ``fp-scalar-oracle-v1`` /
``fp-outcome-unify-v1`` / ``precondition-closure-v1`` never promote until
evidence is complete. Scalar allowlist stays empty. Wave 3 also lands MMIO
capability split + hardware-profile-bound obligations
(``mmio-register-bank-v2``, ``gx-fifo-trace-v1``); FIFO reads / DMA / loop
emission stay incomplete and the MMIO allowlist remains empty until canary.
Wave 4 advanced stubs: fused/paired/psq/traps obligations
(``fp-*-incomplete-v0``) never promotion-grade while midpoint/sticky-residue
and FE0/FE1 remain incomplete; ``gx-fifo-loop-refinement-v1`` structural
schema for ordinary-N ≡ summarized-N FIFO traces; read-side-effects /
external-input / DMA attestations always incomplete until Dolphin harness.
Wave 5 ships an optional authoritative manifest
(``tools/coop/capability_manifest.authoritative.json``) with
``shadow_mode=false``; the default manifest stays shadow for safety.
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
        # Advanced proof-feature capabilities (empty allowlists → fail closed).
        "memory-loop-summary",
        "immutable-address-space",
        "indirect-target-closure",
        "affine-loop-summary",
        "relational-loop-induction",
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
        # Wave 3 FP foundations — incomplete until oracle / RN / NI land.
        "fp-scalar-oracle-v1",
        "fp-outcome-unify-v1",
        # Wave 3: RN/NI/trap entry-precondition closure (UNSAT required).
        "precondition-closure-v1",
        # Wave 3 MMIO foundations (promotion-grade only with hardware profile
        # + complete obligations + allowlist; default allowlist empty).
        "mmio-register-bank-v2",
        "gx-fifo-trace-v1",
        "gx-fifo-loop-refinement-v1",
        "gx-fifo-read-incomplete-v0",
        "mmio-read-side-effects-incomplete-v0",
        "mmio-external-input-incomplete-v0",
        "mmio-loop-emission-incomplete-v0",
        "mixed-address-space-incomplete-v0",
        "dma-interrupt-incomplete-v0",
        # Wave 4 FP advanced — never promotion-grade (midpoint / FE0/FE1).
        "fp-fused-incomplete-v0",
        "fp-paired-incomplete-v0",
        "fp-psq-incomplete-v0",
        "fp-traps-incomplete-v0",
        # Stage 3C FP foundation stubs (load/store/compare/convert).
        "fp-load-store-incomplete-v0",
        "fp-compare-incomplete-v0",
        "fp-convert-incomplete-v0",
        # Stage 3C proof-feature obligation binding.
        "proof-feature-obligation-v1",
        # Shadow / incomplete only — never promotion-grade.
        "legacy-effect-gate-v1",
    }
)

PROVENANCE_REQUIRED_EVIDENCE_FIELDS = (
    "architecture_model",
    "result_format",
    "engine_hash",
    "certifier_hash",
    "source_hash",
    "proof_request_hash",
    "validation_ledger_hash",
    "git_commit",
)

# Compared to live ProofResult when both sides are nonempty.
PROVENANCE_RESULT_COMPARE_FIELDS = (
    "architecture_model",
    "engine_hash",
    "certifier_hash",
    "source_hash",
    "proof_request_hash",
    "validation_ledger_hash",
    "git_commit",
)

OPCODE_EQUALITY_CAPABILITIES = frozenset(
    {
        "integer-core",
        "fp-bitwise",
        "fp-load-store",
        "fp-compare",
        "fp-convert",
        "fp-scalar-arithmetic",
        "fp-fused-arithmetic",
        "fp-paired-single",
        "fp-psq",
    }
)

SUBJECT_EQUALITY_CAPABILITIES = frozenset(
    {
        "certified-calls",
        "bounded-memory",
        "assumed-ordinary-ram",
        "mmio-register-bank",
        "mmio-read-side-effects",
        "mmio-external-input",
        "gx-fifo-write-trace",
        "gx-fifo-read",
        "mmio-loop-emission",
        "mixed-address-space-routing",
        "dma-interrupt-effects",
        "memory-loop-summary",
        "immutable-address-space",
        "indirect-target-closure",
        "affine-loop-summary",
        "relational-loop-induction",
    }
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


def authoritative_capability_manifest_path() -> Path:
    """Optional Wave 5 canary: ``shadow_mode=false`` + require assurance."""
    return (
        Path(__file__).resolve().parents[1]
        / "coop"
        / "capability_manifest.authoritative.json"
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

    def capabilities_used(self) -> tuple[str, ...]:
        return tuple(sorted(self.recomputed_statuses))


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


def build_capability_assurance_audit(
    assurance: CapabilityAssuranceResult | None,
    *,
    shadow_mode: bool,
    authoritative_tier: str | None,
    shadow_legacy_tier: str | None = None,
) -> dict[str, Any]:
    """Compact audit dict for promotion / equivalence decision records."""
    if assurance is None:
        return {
            "capabilities_used": [],
            "weakest_status": STATUS_UNMODELED,
            "shadow_mode": shadow_mode,
            "authoritative_tier": authoritative_tier,
            "shadow_legacy_tier": shadow_legacy_tier,
            "assurance_tier": None,
        }
    assurance_tier = compute_confidence_tier_from_assurance(assurance)
    legacy = (
        shadow_legacy_tier
        if shadow_legacy_tier is not None
        else assurance.shadow_legacy_tier
    )
    return {
        "capabilities_used": list(assurance.capabilities_used()),
        "weakest_status": assurance.weakest_status,
        "recomputed_statuses": dict(sorted(assurance.recomputed_statuses.items())),
        "shadow_mode": shadow_mode,
        "authoritative_tier": authoritative_tier,
        "shadow_legacy_tier": legacy,
        "assurance_tier": assurance_tier,
    }


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
    """Capabilities implied by proof effects (Wave 1–4 + Stage 3C inference).

    Separate ``if`` branches so FP never suppresses memory, calls, loops, or MMIO.
    """
    from tools.ppc_equivalence.capability_requirements import (
        PROOF_FEATURE_CAPABILITY_MAP,
        ordinary_memory_was_accessed,
    )
    from tools.ppc_equivalence.fp_capabilities import (
        classify_fp_capabilities,
        traps_enabled_from_result,
    )
    from tools.ppc_equivalence.fp_outcome import capability_tags_for_opcodes
    from tools.ppc_equivalence.fp_traps import capability_tags_for_trap_domain
    from tools.ppc_equivalence.mmio_capability_obligations import (
        infer_mmio_capabilities_from_memory_bus,
    )

    flags = _proof_effect_flags(result)
    used: set[str] = {"integer-core", "provenance"}
    if flags["has_domain_exceptions"]:
        used.add("domain-exception")
    if flags["has_assumed_ram"]:
        used.add("assumed-ordinary-ram")
    if flags["has_callees"]:
        used.add("certified-calls")
    if flags["has_memory_bus"]:
        # Wave 3: split MMIO demands from memory_bus / proof_features.
        proof_features = getattr(result, "proof_features", None) or []
        memory_bus = getattr(result, "memory_bus", None)
        if isinstance(memory_bus, dict) or "memory-bus" in proof_features:
            used.update(
                infer_mmio_capabilities_from_memory_bus(
                    memory_bus if isinstance(memory_bus, dict) else None,
                    proof_features=tuple(str(item) for item in proof_features),
                )
            )
        else:
            used.add("mmio-register-bank")
    if flags["has_fp"]:
        opcodes = getattr(result, "opcodes_used", None) or []
        traps_enabled = traps_enabled_from_result(result)
        fp_caps = set(
            classify_fp_capabilities(
                opcodes,
                traps_enabled=traps_enabled,
            )
        )
        # Wave 4 tagging hooks (paired/psq opcodes + trap domain).
        fp_caps |= set(capability_tags_for_opcodes(opcodes))
        fp_caps |= set(capability_tags_for_trap_domain(traps_enabled=traps_enabled))
        if fp_caps:
            used.update(fp_caps)
        else:
            # FP domain / coverage present but opcodes not classifiable — fail closed.
            used.add("fp-scalar-arithmetic")
        # Stage 3C: domain restrictions demand precondition-closure explicitly.
        try:
            from tools.ppc_equivalence.capability_attachment import (
                domain_restrictions_present,
            )

            if domain_restrictions_present(result):
                used.add("precondition-closure")
        except ImportError:
            pass
    # Independent of FP — do not elif-suppress sibling memory assurance.
    if (
        not flags["has_assumed_ram"]
        and not flags["has_memory_bus"]
        and (
            flags["has_memory_access"]
            or ordinary_memory_was_accessed(result)
        )
    ):
        used.add("bounded-memory")
    # Stage 3C: advanced proof features demand their own capabilities.
    for feature in getattr(result, "proof_features", None) or []:
        mapped = PROOF_FEATURE_CAPABILITY_MAP.get(str(feature))
        if mapped:
            used.add(mapped)
    return frozenset(used)


def _normalize_requirement(
    raw: Any | None,
) -> Any | None:
    if raw is None:
        return None
    if hasattr(raw, "requirement_sha256") and hasattr(raw, "required_opcodes"):
        return raw
    if isinstance(raw, Mapping):
        from tools.ppc_equivalence.capability_requirements import CapabilityRequirement

        return CapabilityRequirement.from_dict(raw)
    return None


def _attested_opcodes(evidence: Mapping[str, Any]) -> set[str]:
    from tools.ppc_equivalence.capability_requirements import normalize_opcode

    opcodes = evidence.get("opcodes")
    if not isinstance(opcodes, list):
        return set()
    return {normalize_opcode(op) for op in opcodes}


def _attested_subjects(
    attestation: CapabilityAttestation,
) -> set[str]:
    evidence = attestation.evidence
    subjects = evidence.get("subjects")
    if isinstance(subjects, list):
        return {str(item) for item in subjects}
    device_ids = evidence.get("device_ids")
    if isinstance(device_ids, list):
        return {str(item) for item in device_ids}
    obligation = evidence.get("obligation")
    if isinstance(obligation, Mapping):
        callees = obligation.get("callees")
        if isinstance(callees, list):
            return {
                str(entry.get("target_id"))
                for entry in callees
                if isinstance(entry, Mapping) and entry.get("target_id") is not None
            }
        devices = obligation.get("devices") or obligation.get("device_ids")
        if isinstance(devices, list):
            return {str(item) for item in devices}
    mmio = evidence.get("mmio")
    if isinstance(mmio, Mapping):
        devices = mmio.get("device_ids") or mmio.get("devices")
        if isinstance(devices, list):
            return {str(item) for item in devices}
    return set()


def _requirement_binding_ok(
    attestation: CapabilityAttestation,
    requirement: Any | None,
) -> bool:
    """Exact requirement digest + opcode/subject coverage when bound."""
    if requirement is None:
        return True
    evidence = attestation.evidence
    expected = getattr(requirement, "requirement_sha256", "") or ""
    got = evidence.get("requirement_sha256")
    if not isinstance(got, str) or not got or got != expected:
        return False
    required_opcodes = tuple(getattr(requirement, "required_opcodes", ()) or ())
    if (
        attestation.capability in OPCODE_EQUALITY_CAPABILITIES
        or required_opcodes
    ):
        if attestation.capability in OPCODE_EQUALITY_CAPABILITIES or required_opcodes:
            attested = _attested_opcodes(evidence)
            required = set(required_opcodes)
            if attested != required:
                return False
    required_subjects = tuple(getattr(requirement, "required_subjects", ()) or ())
    if (
        attestation.capability in SUBJECT_EQUALITY_CAPABILITIES
        and required_subjects
    ):
        if _attested_subjects(attestation) != set(required_subjects):
            return False
    return True


def _recompute_attestation_status(
    attestation: CapabilityAttestation,
    *,
    requirement: Any | None = None,
    result: Any | None = None,
    ledger: Any | None,
    manifest: CapabilityManifest,
) -> str:
    """Independently recompute grade; ignore caller-supplied status."""
    if attestation.algorithm == "legacy-effect-gate-v1":
        return STATUS_INCOMPLETE

    resolved_requirement = _normalize_requirement(requirement)
    if resolved_requirement is not None and not _requirement_binding_ok(
        attestation, resolved_requirement
    ):
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
            if value is None or value == "":
                return STATUS_INCOMPLETE
            if field_name == "result_format":
                # Accept int or digit string.
                try:
                    int(value)
                except (TypeError, ValueError):
                    return STATUS_INCOMPLETE
            elif not isinstance(value, (str, int)):
                return STATUS_INCOMPLETE
        if bool(evidence.get("git_dirty")):
            return STATUS_INCOMPLETE
        if result is not None:
            for field_name in PROVENANCE_RESULT_COMPARE_FIELDS:
                expected = getattr(result, field_name, None)
                if field_name == "architecture_model" and expected is None:
                    continue
                if expected is None or expected == "":
                    continue
                got = evidence.get(field_name)
                if field_name == "result_format":
                    try:
                        if int(got) != int(expected):
                            return STATUS_INCOMPLETE
                    except (TypeError, ValueError):
                        return STATUS_INCOMPLETE
                elif str(got) != str(expected):
                    return STATUS_INCOMPLETE
            if bool(getattr(result, "git_dirty", False)):
                return STATUS_INCOMPLETE
            # Live ledger content hash when available.
            live_ledger_hash = None
            if ledger is not None:
                live_ledger_hash = getattr(ledger, "content_sha256", None)
            if isinstance(live_ledger_hash, str) and live_ledger_hash:
                if evidence.get("validation_ledger_hash") != live_ledger_hash:
                    return STATUS_INCOMPLETE
            fp_oracle = getattr(result, "fp_oracle_version", None)
            if fp_oracle:
                if evidence.get("fp_oracle_version") != str(fp_oracle):
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

    if attestation.algorithm == "precondition-closure-v1":
        if attestation.capability != "precondition-closure":
            return STATUS_INCOMPLETE
        allowed = manifest.allowed_versions(attestation.capability)
        if attestation.model_version not in allowed:
            return STATUS_INCOMPLETE
        from tools.ppc_equivalence.fp_rounding import (
            evaluate_precondition_closure_status,
        )

        return evaluate_precondition_closure_status(
            attestation.evidence,
            assumptions=attestation.assumptions,
            unsupported=attestation.unsupported,
        )

    if attestation.algorithm in {"fp-scalar-oracle-v1", "fp-outcome-unify-v1"}:
        # Wave 3 foundations: never promotion-grade. Host float → incomplete.
        if attestation.capability != "fp-scalar-arithmetic":
            return STATUS_INCOMPLETE
        if attestation.evidence.get("host_float") is True:
            return STATUS_INCOMPLETE
        allowed = manifest.allowed_versions(attestation.capability)
        # Empty allowlist (current policy) → incomplete even with perfect evidence.
        if attestation.model_version not in allowed:
            return STATUS_INCOMPLETE
        if attestation.algorithm == "fp-scalar-oracle-v1":
            from tools.ppc_equivalence.fp_scalar_obligations import (
                evaluate_fp_scalar_obligation_status,
            )

            obligation = attestation.evidence.get("obligation")
            return evaluate_fp_scalar_obligation_status(
                obligation if isinstance(obligation, dict) else None,
                host_float=attestation.evidence.get("host_float"),
            )
        # fp-outcome-unify-v1 records type unification only.
        return STATUS_INCOMPLETE

    if attestation.algorithm in {
        "fp-fused-incomplete-v0",
        "fp-paired-incomplete-v0",
        "fp-psq-incomplete-v0",
        "fp-traps-incomplete-v0",
    }:
        from tools.ppc_equivalence.fp_advanced_obligations import (
            FP_ADVANCED_ALGORITHMS,
            recompute_fp_advanced_attestation_status,
        )

        if attestation.algorithm not in FP_ADVANCED_ALGORITHMS:
            return STATUS_INCOMPLETE
        return recompute_fp_advanced_attestation_status(
            attestation.evidence,
            capability=attestation.capability,
            algorithm=attestation.algorithm,
            model_version=attestation.model_version,
            assumptions=attestation.assumptions,
            unsupported=attestation.unsupported,
            allowed_versions=manifest.allowed_versions(attestation.capability),
        )

    if attestation.algorithm in {
        "fp-load-store-incomplete-v0",
        "fp-compare-incomplete-v0",
        "fp-convert-incomplete-v0",
    }:
        from tools.ppc_equivalence.capability_attachment import (
            recompute_fp_foundation_incomplete_status,
        )

        return recompute_fp_foundation_incomplete_status(
            attestation.evidence,
            capability=attestation.capability,
            algorithm=attestation.algorithm,
            model_version=attestation.model_version,
            assumptions=attestation.assumptions,
            unsupported=attestation.unsupported,
            allowed_versions=manifest.allowed_versions(attestation.capability),
        )

    if attestation.algorithm == "proof-feature-obligation-v1":
        from tools.ppc_equivalence.capability_attachment import (
            recompute_proof_feature_attestation_status,
        )

        return recompute_proof_feature_attestation_status(
            attestation.evidence,
            capability=attestation.capability,
            algorithm=attestation.algorithm,
            model_version=attestation.model_version,
            assumptions=attestation.assumptions,
            unsupported=attestation.unsupported,
            allowed_versions=manifest.allowed_versions(attestation.capability),
        )

    if attestation.algorithm in {
        "mmio-register-bank-v2",
        "gx-fifo-trace-v1",
        "gx-fifo-loop-refinement-v1",
        "gx-fifo-read-incomplete-v0",
        "mmio-read-side-effects-incomplete-v0",
        "mmio-external-input-incomplete-v0",
        "mmio-loop-emission-incomplete-v0",
        "mixed-address-space-incomplete-v0",
        "dma-interrupt-incomplete-v0",
    }:
        from tools.ppc_equivalence.mmio_capability_obligations import (
            MMIO_ATTESTATION_ALGORITHMS,
            recompute_mmio_attestation_status,
        )

        if attestation.algorithm not in MMIO_ATTESTATION_ALGORITHMS:
            return STATUS_INCOMPLETE
        return recompute_mmio_attestation_status(
            attestation.evidence,
            capability=attestation.capability,
            algorithm=attestation.algorithm,
            model_version=attestation.model_version,
            assumptions=attestation.assumptions,
            unsupported=attestation.unsupported,
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


def _normalize_requirements_block(raw: Any | None) -> Any | None:
    if raw is None:
        return None
    if hasattr(raw, "requirements") and hasattr(raw, "requirements_sha256"):
        return raw
    if isinstance(raw, Mapping):
        from tools.ppc_equivalence.capability_requirements import (
            CapabilityRequirements,
        )

        return CapabilityRequirements.from_dict(raw)
    return None


def _requirement_lookup(
    requirements: Any | None,
    capability: str,
) -> Any | None:
    if requirements is None:
        return None
    block = _normalize_requirements_block(requirements)
    if block is None:
        return None
    for item in block.requirements:
        if item.capability == capability:
            return item
    return None


def _bind_requirement_evidence(
    evidence: dict[str, Any],
    requirement: Any | None,
    *,
    requirements_sha256: str | None = None,
) -> dict[str, Any]:
    if requirement is None:
        return evidence
    evidence = dict(evidence)
    digest = getattr(requirement, "requirement_sha256", None)
    if digest is None and isinstance(requirement, Mapping):
        digest = requirement.get("requirement_sha256")
    if digest:
        evidence["requirement_sha256"] = str(digest)
    if requirements_sha256:
        evidence["requirements_sha256"] = requirements_sha256
    return evidence


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

    requirements_block = None
    try:
        requirements_block = _normalize_requirements_block(
            getattr(result, "capability_requirements", None)
        )
    except (TypeError, ValueError, KeyError):
        requirements_block = None

    if requirements_block is not None:
        req_by_name = {
            item.capability: item for item in requirements_block.requirements
        }
        used = frozenset(req_by_name)
    else:
        req_by_name = {}
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
            requirement = req_by_name.get(item.capability)
            # When authoritative requirements are present, attestations without
            # a matching requirement are incomplete (stale / unexplained).
            if requirements_block is not None and requirement is None:
                status = STATUS_INCOMPLETE
            else:
                status = _recompute_attestation_status(
                    item,
                    requirement=requirement,
                    result=result,
                    ledger=ledger,
                    manifest=resolved_manifest,
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
    # Extra attestations outside the requirement set also count as incomplete.
    if requirements_block is not None:
        for name, status in recomputed.items():
            if name not in used and status == STATUS_INCOMPLETE:
                has_unmodeled = True
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
    requirements: Any | None = None,
) -> CapabilityAssurance | None:
    """Optional draft for EQUIVALENT proofs: integer-core (+ fp-bitwise when used).

    Status is advisory; ``evaluate_capability_assurance`` recomputes the grade.
    Does **not** suppress sibling capabilities: memory-bus / scalar FP / MMIO
    proofs still receive integer-core (and exact ``fp-bitwise`` opcode binding
    when bitwise ops appear). Other FP/MMIO/feature attestations are attached
    by Stage 3C ``maybe_attach_stage3c_drafts``.
    """
    from tools.ppc_equivalence.capability_requirements import normalize_opcodes
    from tools.ppc_equivalence.fp_bitwise import (
        FP_BITWISE_ALGORITHM,
        FP_BITWISE_MODEL_VERSION,
        FP_BITWISE_OPS,
        fp_opcodes_among,
    )
    from tools.ppc_equivalence.result import ProofStatus

    if getattr(result, "status", None) is not ProofStatus.EQUIVALENT:
        return None
    opcodes = [str(op) for op in (getattr(result, "opcodes_used", None) or [])]
    if not opcodes:
        return None

    # Skip only when both integer-core and (if needed) fp-bitwise already exist
    # with ledger binding (when a ledger digest is available to attach).
    existing = getattr(result, "capability_assurance", None)
    prior_names: set[str] = set()
    prior_by_name: dict[str, CapabilityAttestation] = {}
    if existing is not None:
        try:
            prior = _normalize_assurance(existing)
            if prior is not None:
                prior_by_name = {item.capability: item for item in prior.capabilities}
                prior_names = set(prior_by_name)
        except (TypeError, ValueError, KeyError):
            prior_names = set()

    resolved_ledger_sha = ledger_sha256
    if not resolved_ledger_sha and ledger is not None:
        content = getattr(ledger, "content_sha256", None)
        if isinstance(content, str) and content:
            resolved_ledger_sha = content
        elif hasattr(ledger, "dolphin_validated_opcodes"):
            resolved_ledger_sha = canonical_json_sha256(
                sorted(str(op) for op in ledger.dolphin_validated_opcodes)
            )

    def _needs_ledger_refresh(capability: str) -> bool:
        if not resolved_ledger_sha:
            return False
        prior_item = prior_by_name.get(capability)
        if prior_item is None:
            return False
        existing_sha = prior_item.evidence.get("ledger_sha256")
        return existing_sha != resolved_ledger_sha

    bitwise_present = bool(fp_opcodes_among(opcodes) & FP_BITWISE_OPS)
    integer_done = "integer-core" in prior_names and not _needs_ledger_refresh(
        "integer-core"
    )
    bitwise_done = (
        not bitwise_present
        or (
            "fp-bitwise" in prior_names
            and not _needs_ledger_refresh("fp-bitwise")
        )
    )
    if integer_done and bitwise_done:
        return None

    # When refreshing, drop the prior name so the draft is re-emitted.
    if _needs_ledger_refresh("integer-core"):
        prior_names.discard("integer-core")
    if bitwise_present and _needs_ledger_refresh("fp-bitwise"):
        prior_names.discard("fp-bitwise")

    req_block = _normalize_requirements_block(
        requirements
        if requirements is not None
        else getattr(result, "capability_requirements", None)
    )
    req_sha = (
        str(getattr(req_block, "requirements_sha256", "") or "")
        if req_block is not None
        else None
    )
    integer_req = _requirement_lookup(req_block, "integer-core")
    fp_req = _requirement_lookup(req_block, "fp-bitwise")

    fp_ops = fp_opcodes_among(opcodes)
    if integer_req is not None and integer_req.required_opcodes:
        integer_opcodes = list(integer_req.required_opcodes)
    else:
        integer_opcodes = sorted(op for op in opcodes if op not in fp_ops)
        if not integer_opcodes:
            integer_opcodes = sorted(op for op in opcodes if op not in FP_BITWISE_OPS)
            if not integer_opcodes and "blr" in opcodes:
                integer_opcodes = ["blr"]
        integer_opcodes = list(normalize_opcodes(integer_opcodes))

    attestations: list[CapabilityAttestation] = []
    if "integer-core" not in prior_names and integer_opcodes:
        integer_evidence: dict[str, Any] = {"opcodes": integer_opcodes}
        if resolved_ledger_sha:
            integer_evidence["ledger_sha256"] = resolved_ledger_sha
        integer_evidence = _bind_requirement_evidence(
            integer_evidence, integer_req, requirements_sha256=req_sha
        )
        attestations.append(
            build_attestation(
                capability="integer-core",
                model_version="integer-core-v1",
                algorithm="opcode-ledger-v2",
                status=STATUS_INCOMPLETE,
                evidence=integer_evidence,
            )
        )

    if bitwise_present and "fp-bitwise" not in prior_names:
        if fp_req is not None and fp_req.required_opcodes:
            bitwise_ops = list(fp_req.required_opcodes)
        else:
            bitwise_ops = list(
                normalize_opcodes(op for op in opcodes if op in FP_BITWISE_OPS)
            )
        fp_evidence: dict[str, Any] = {
            "opcodes": bitwise_ops,
            "host_float": False,
        }
        if resolved_ledger_sha:
            fp_evidence["ledger_sha256"] = resolved_ledger_sha
        fp_evidence = _bind_requirement_evidence(
            fp_evidence, fp_req, requirements_sha256=req_sha
        )
        attestations.append(
            build_attestation(
                capability="fp-bitwise",
                model_version=FP_BITWISE_MODEL_VERSION,
                algorithm=FP_BITWISE_ALGORITHM,
                status=STATUS_INCOMPLETE,
                evidence=fp_evidence,
            )
        )

    if not attestations:
        return None
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
    requirements: Any | None = None,
) -> CapabilityAttestation | None:
    """Draft a provenance-binding attestation from proof hash fields.

    Status is advisory; ``evaluate_capability_assurance`` recomputes the grade.
    """
    from tools.ppc_equivalence.result import ARCHITECTURE_MODEL, RESULT_FORMAT, ProofStatus

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
    architecture_model = str(
        getattr(result, "architecture_model", "") or ARCHITECTURE_MODEL
    )
    result_format = getattr(result, "format", RESULT_FORMAT)

    evidence: dict[str, Any] = {
        "architecture_model": architecture_model,
        "result_format": int(result_format) if result_format is not None else RESULT_FORMAT,
        "engine_hash": engine_hash,
        "certifier_hash": certifier_hash,
        "source_hash": source_hash,
        "proof_request_hash": proof_request,
        "validation_ledger_hash": ledger_hash,
        "git_commit": git_commit,
        "git_dirty": git_dirty,
    }
    fp_oracle = getattr(result, "fp_oracle_version", None)
    if fp_oracle:
        evidence["fp_oracle_version"] = str(fp_oracle)
    if allowed_engine_sha256:
        evidence["allowed_engine_sha256"] = allowed_engine_sha256

    req_block = _normalize_requirements_block(
        requirements if requirements is not None
        else getattr(result, "capability_requirements", None)
    )
    req_sha = (
        str(getattr(req_block, "requirements_sha256", "") or "")
        if req_block is not None
        else None
    )
    evidence = _bind_requirement_evidence(
        evidence,
        _requirement_lookup(req_block, "provenance"),
        requirements_sha256=req_sha,
    )

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
    requirements: Any | None = None,
) -> Any:
    """Attach a draft integer-core / fp-bitwise assurance block when applicable."""
    draft = draft_integer_core_assurance(
        result,
        ledger=ledger,
        ledger_sha256=ledger_sha256,
        requirements=requirements,
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


def _bounded_memory_requirement_from(requirements: Any) -> Any | None:
    """Return the bounded-memory requirement, or None to skip / proceed.

    Compatible with upcoming ``CapabilityRequirements`` (Stage 1) and plain
    mappings. When *requirements* is supplied but has no bounded-memory entry,
    returns ``False`` so the draft helper can skip attachment.
    """
    if requirements is None:
        return None
    reqs = getattr(requirements, "requirements", None)
    if reqs is None and isinstance(requirements, Mapping):
        reqs = requirements.get("requirements")
    if reqs is None:
        return None
    for item in reqs:
        if isinstance(item, Mapping):
            capability = item.get("capability")
        else:
            capability = getattr(item, "capability", None)
        if capability == "bounded-memory":
            return item
    # Requirements present but bounded-memory not demanded.
    return False


def draft_bounded_memory_assurance(
    result: Any,
    *,
    obligation: Mapping[str, Any] | None = None,
    requirements: Any | None = None,
    original_terminals: Any | None = None,
    candidate_terminals: Any | None = None,
    platform_profile: Any | None = None,
    environment: Any | None = None,
    deadline: Any | None = None,
    z3: Any | None = None,
) -> CapabilityAssurance | None:
    """Optional bounded-memory attestation for Stage 3A / Stage 4 orchestrator.

    Prefer a pre-built *obligation*. Otherwise, when terminals (+ optional
    platform profile) are supplied, build via
    ``build_bounded_memory_obligation_from_terminals``.

    Status is advisory; ``evaluate_capability_assurance`` recomputes the grade.
    Assumed-ordinary-ram proofs never receive a promotion-grade bounded-memory
    attestation from this helper. Generation failure returns ``None`` (caller
    preserves base EQUIVALENT and blocks Tier A separately).
    """
    from tools.ppc_equivalence.bounded_memory_obligations import (
        SOURCE_ASSUMED,
        build_bounded_memory_attestation,
        build_bounded_memory_obligation_from_terminals,
        classify_range_source,
    )
    from tools.ppc_equivalence.memory_profile import MemoryProfile
    from tools.ppc_equivalence.result import ProofStatus

    if getattr(result, "status", None) is not ProofStatus.EQUIVALENT:
        return None

    req = _bounded_memory_requirement_from(requirements)
    if req is False:
        return None

    env = environment if environment is not None else getattr(result, "environment", None)
    if env is not None and getattr(env, "profile", None) is not None:
        if env.profile == MemoryProfile.ASSUMED_ORDINARY_RAM:
            return None

    built = obligation
    if built is None and (
        original_terminals is not None or candidate_terminals is not None
    ):
        try:
            built = build_bounded_memory_obligation_from_terminals(
                original_terminals or (),
                candidate_terminals or (),
                platform_profile=platform_profile,
                environment=env,
                deadline=deadline,
                z3=z3,
            )
        except Exception:
            # Stage 4 will surface capability-assurance-generation-failed;
            # do not erase base equivalence here.
            return None
    if built is None:
        return None
    if built.get("source") == SOURCE_ASSUMED:
        return None
    if env is not None and classify_range_source(environment=env) == SOURCE_ASSUMED:
        return None

    attestation = build_bounded_memory_attestation(built)
    # Bind upcoming requirement digest when Stage 1 types are present.
    if req is not None and req is not False:
        evidence = dict(attestation.evidence)
        req_digest = (
            req.get("requirement_sha256")
            if isinstance(req, Mapping)
            else getattr(req, "requirement_sha256", None)
        )
        if req_digest:
            evidence["requirement_sha256"] = str(req_digest)
        reqs_digest = None
        if requirements is not None:
            reqs_digest = getattr(requirements, "requirements_sha256", None)
            if reqs_digest is None and isinstance(requirements, Mapping):
                reqs_digest = requirements.get("requirements_sha256")
        if reqs_digest:
            evidence["requirements_sha256"] = str(reqs_digest)
        proof_request = str(
            getattr(result, "proof_request_hash", "")
            or getattr(result, "source_hash", "")
            or ""
        )
        if proof_request:
            evidence["proof_request_hash"] = proof_request
        attestation = build_attestation(
            capability=attestation.capability,
            model_version=attestation.model_version,
            algorithm=attestation.algorithm,
            status=attestation.status,
            assumptions=attestation.assumptions,
            unsupported=attestation.unsupported,
            evidence=evidence,
        )

    return _merge_assurance_attestation(
        getattr(result, "capability_assurance", None),
        attestation,
    )


def maybe_attach_bounded_memory_draft(
    result: Any,
    *,
    obligation: Mapping[str, Any] | None = None,
    requirements: Any | None = None,
    original_terminals: Any | None = None,
    candidate_terminals: Any | None = None,
    platform_profile: Any | None = None,
    environment: Any | None = None,
    deadline: Any | None = None,
    z3: Any | None = None,
) -> Any:
    """Attach / merge a bounded-memory attestation when discharge evidence exists.

    Safe to call when Stage 0–2 APIs are absent: failures leave *result*
    unchanged (base EQUIVALENT preserved).
    """
    try:
        draft = draft_bounded_memory_assurance(
            result,
            obligation=obligation,
            requirements=requirements,
            original_terminals=original_terminals,
            candidate_terminals=candidate_terminals,
            platform_profile=platform_profile,
            environment=environment,
            deadline=deadline,
            z3=z3,
        )
    except Exception:
        return result
    if draft is None:
        return result
    # draft_bounded_memory_assurance already merged into a CapabilityAssurance.
    result.capability_assurance = (
        draft.to_dict() if hasattr(draft, "to_dict") else draft
    )
    return result


def maybe_attach_provenance_draft(
    result: Any,
    *,
    allowed_engine_sha256: str | None = None,
    ledger_sha256: str | None = None,
    requirements: Any | None = None,
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
        requirements=requirements,
    )
    if attestation is None:
        return result
    merged = _merge_assurance_attestation(
        getattr(result, "capability_assurance", None),
        attestation,
    )
    result.capability_assurance = merged.to_dict()
    return result


@dataclass(frozen=True)
class CapabilityBuildResult:
    """Outcome of Stage 4 capability-assurance orchestration.

    Callers re-evaluate grades via ``evaluate_capability_assurance``; this
    builder only derives requirements and attaches draft attestations.
    Generation failures leave the base proof status intact and surface
    ``errors`` (warning codes without the common prefix).
    """

    assurance: CapabilityAssurance | None
    requirements: Any | None  # CapabilityRequirements | Mapping | None
    errors: tuple[str, ...]


def _append_generation_warning(result: Any, code: str) -> None:
    warning = f"capability-assurance-generation-failed:{code}"
    warnings = list(getattr(result, "warnings", None) or [])
    if warning not in warnings:
        warnings.append(warning)
        result.warnings = warnings


def _resolve_terminals_meta(
    terminals_meta: Any | None,
    *,
    original_terminals: Any | None = None,
    candidate_terminals: Any | None = None,
) -> Any | None:
    if terminals_meta is not None:
        return terminals_meta
    if original_terminals is None and candidate_terminals is None:
        return None
    return {
        "original": list(original_terminals or ()),
        "candidate": list(candidate_terminals or ()),
    }


def build_capability_assurance(
    result: Any,
    *,
    ledger: Any | None = None,
    manifest: Any | None = None,
    callee_context: Any | None = None,
    terminals_meta: Any | None = None,
    requirements: Any | None = None,
    ledger_sha256: str | None = None,
    allowed_engine_sha256: str | None = None,
    original_terminals: Any | None = None,
    candidate_terminals: Any | None = None,
    platform_profile: Any | None = None,
    environment: Any | None = None,
    deadline: Any | None = None,
    z3: Any | None = None,
    callees: Any | None = None,
    call_context: Any | None = None,
    live_certificates: Mapping[str, Any] | None = None,
    missing_callees: Any | None = None,
    allow_zero_hash_placeholders: bool = False,
) -> CapabilityBuildResult:
    """Derive requirements and attach capability attestations (Stage 4).

    Order: requirements → integer-core → provenance → certified-calls →
    bounded-memory → stage3c. Each stage is isolated; failures append
    ``capability-assurance-generation-failed:<code>`` warnings and collect
    error codes without erasing base EQUIVALENT.

    ``manifest`` is accepted for API symmetry with evaluate callers; attachment
    does not consult it (grades are recomputed later).
    """
    del manifest  # evaluate_capability_assurance is the caller's responsibility

    errors: list[str] = []
    resolved_terminals = _resolve_terminals_meta(
        terminals_meta,
        original_terminals=original_terminals,
        candidate_terminals=candidate_terminals,
    )

    resolved_requirements = _normalize_requirements_block(
        requirements
        if requirements is not None
        else getattr(result, "capability_requirements", None)
    )
    if resolved_requirements is None:
        try:
            from tools.ppc_equivalence.capability_requirements import (
                derive_capability_requirements,
            )

            resolved_requirements = derive_capability_requirements(
                result,
                terminals_meta=resolved_terminals,
            )
            result.capability_requirements = resolved_requirements.to_dict()
        except Exception as exc:
            code = f"requirements:{type(exc).__name__}"
            errors.append(code)
            _append_generation_warning(result, code)
            resolved_requirements = None
    else:
        # Keep an authoritative dict on the result for certificate / cache audit.
        if hasattr(resolved_requirements, "to_dict"):
            result.capability_requirements = resolved_requirements.to_dict()
        elif isinstance(resolved_requirements, Mapping):
            result.capability_requirements = dict(resolved_requirements)

    req_arg = resolved_requirements

    def _run_stage(code: str, fn) -> None:  # type: ignore[no-untyped-def]
        try:
            fn()
        except Exception as exc:
            detail = f"{code}:{type(exc).__name__}"
            errors.append(detail)
            _append_generation_warning(result, detail)

    _run_stage(
        "integer-core",
        lambda: maybe_attach_integer_core_draft(
            result,
            ledger=ledger,
            ledger_sha256=ledger_sha256,
            requirements=req_arg,
        ),
    )
    _run_stage(
        "provenance",
        lambda: maybe_attach_provenance_draft(
            result,
            allowed_engine_sha256=allowed_engine_sha256,
            ledger_sha256=ledger_sha256,
            requirements=req_arg,
        ),
    )

    def _attach_certified_calls() -> None:
        from tools.ppc_equivalence.certified_calls_obligations import (
            maybe_attach_certified_calls_draft,
            select_used_callee_inputs,
        )

        used_callees = list(getattr(result, "assumed_callees", None) or [])
        resolved_callees = list(callees or ())
        resolved_missing = list(missing_callees or ())
        resolved_context = call_context
        resolved_live = live_certificates
        if callee_context is not None:
            context_inputs = list(getattr(callee_context, "callee_inputs", ()) or ())
            address_map = getattr(callee_context, "address_to_target_id", None) or {}
            if not resolved_callees and (context_inputs or used_callees):
                selected, missing = select_used_callee_inputs(
                    context_inputs,
                    used_callees,
                    address_to_target_id=address_map,
                )
                resolved_callees = list(selected)
                if not resolved_missing:
                    resolved_missing = list(missing)
            if resolved_context is None:
                resolved_context = getattr(callee_context, "call_context", None)
            if resolved_live is None:
                resolved_live = getattr(callee_context, "live_certificates", None)
        maybe_attach_certified_calls_draft(
            result,
            callees=resolved_callees,
            context=resolved_context,
            live_certificates=resolved_live,
            missing_callees=resolved_missing,
            allow_zero_hash_placeholders=allow_zero_hash_placeholders,
            requirements=req_arg,
        )

    _run_stage("certified-calls", _attach_certified_calls)

    _run_stage(
        "bounded-memory",
        lambda: maybe_attach_bounded_memory_draft(
            result,
            requirements=req_arg,
            original_terminals=original_terminals,
            candidate_terminals=candidate_terminals,
            platform_profile=platform_profile,
            environment=environment
            if environment is not None
            else getattr(result, "environment", None),
            deadline=deadline,
            z3=z3,
        ),
    )

    def _attach_stage3c() -> None:
        from tools.ppc_equivalence.capability_attachment import (
            maybe_attach_stage3c_drafts,
        )

        maybe_attach_stage3c_drafts(
            result,
            ledger=ledger,
            ledger_sha256=ledger_sha256,
            requirements=req_arg,
        )

    _run_stage("stage3c", _attach_stage3c)

    assurance = None
    raw = getattr(result, "capability_assurance", None)
    if raw is not None:
        try:
            assurance = _normalize_assurance(raw)
        except (TypeError, ValueError, KeyError):
            assurance = None

    return CapabilityBuildResult(
        assurance=assurance,
        requirements=resolved_requirements,
        errors=tuple(errors),
    )
