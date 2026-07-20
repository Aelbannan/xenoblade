"""Authoritative capability-requirements records (Stages 1–2).

Requirements are derived from proof execution evidence — terminals, callees,
opcodes, proof features, memory bus — not from caller-supplied attestations.
Attestations must bind ``requirement_sha256`` and cover exact opcode/subject
sets during ``evaluate_capability_assurance``.
"""

from __future__ import annotations

import re
from dataclasses import dataclass
from typing import Any, Iterable, Mapping, Sequence

from tools.ppc_equivalence.provenance import canonical_json_sha256

CAPABILITY_REQUIREMENTS_SCHEMA_VERSION = 1

_SHA256_HEX_RE = re.compile(r"^[0-9a-f]{64}$")

# Shared with capability_assurance (defined here to avoid import cycles).
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

# Proof-feature name → capability-assurance demand name.
PROOF_FEATURE_CAPABILITY_MAP: dict[str, str] = {
    "memory-loop-summary": "memory-loop-summary",
    "readonly-image": "immutable-address-space",
    "indirect-target-closure": "indirect-target-closure",
    "affine-loop-summary": "affine-loop-summary",
    "relational-induction": "relational-loop-induction",
}

ADVANCED_PROOF_FEATURE_CAPABILITIES = frozenset(PROOF_FEATURE_CAPABILITY_MAP.values())


def normalize_opcode(opcode: str) -> str:
    """Canonical opcode token for requirement / attestation equality."""
    return str(opcode).strip().lower()


def normalize_opcodes(opcodes: Iterable[str]) -> tuple[str, ...]:
    return tuple(sorted({normalize_opcode(op) for op in opcodes if str(op).strip()}))


def normalize_subjects(subjects: Iterable[str]) -> tuple[str, ...]:
    return tuple(sorted({str(item).strip() for item in subjects if str(item).strip()}))


def normalize_proof_features(features: Iterable[str]) -> tuple[str, ...]:
    return tuple(sorted({str(item).strip() for item in features if str(item).strip()}))


def _is_normalized_str_tuple(values: Any) -> bool:
    if not isinstance(values, tuple):
        return False
    if not all(isinstance(item, str) and item for item in values):
        return False
    return values == tuple(sorted(set(values)))


def _validate_raw_str_list(field_name: str, value: Any) -> None:
    if not isinstance(value, (list, tuple)):
        raise ValueError(f"capability requirement {field_name} must be a list of strings")
    for item in value:
        if not isinstance(item, str) or not item:
            raise ValueError(
                f"capability requirement {field_name} entries must be nonempty strings"
            )
    as_tuple = tuple(value)
    if as_tuple != tuple(sorted(set(as_tuple))):
        raise ValueError(
            f"capability requirement {field_name} must be sorted and duplicate-free"
        )


def _validate_raw_evidence_identity(value: Any) -> None:
    if not isinstance(value, Mapping):
        raise ValueError("capability requirement evidence_identity must be a mapping")
    for key, item in value.items():
        if not isinstance(key, str) or not isinstance(item, str):
            raise ValueError(
                "capability requirement evidence_identity must be string-to-string"
            )


def _validate_sha256_hex(field_name: str, value: Any) -> None:
    if not isinstance(value, str) or _SHA256_HEX_RE.fullmatch(value) is None:
        raise ValueError(f"{field_name} must be a lowercase 64-char hex digest")


@dataclass(frozen=True)
class CapabilityRequirement:
    capability: str
    required_opcodes: tuple[str, ...] = ()
    required_subjects: tuple[str, ...] = ()
    proof_features: tuple[str, ...] = ()
    evidence_identity: tuple[tuple[str, str], ...] = ()
    requirement_sha256: str = ""

    def to_dict(self) -> dict[str, Any]:
        return {
            "capability": self.capability,
            "required_opcodes": list(self.required_opcodes),
            "required_subjects": list(self.required_subjects),
            "proof_features": list(self.proof_features),
            "evidence_identity": {
                key: value for key, value in self.evidence_identity
            },
            "requirement_sha256": self.requirement_sha256,
        }

    def without_hash(self) -> dict[str, Any]:
        payload = self.to_dict()
        payload.pop("requirement_sha256", None)
        return payload

    def validate_structure(self) -> None:
        """Strict checks on an already-constructed requirement (no coercion)."""
        if not isinstance(self.capability, str) or not self.capability:
            raise ValueError("capability requirement capability must be a nonempty string")
        if self.capability not in KNOWN_CAPABILITIES:
            raise ValueError(f"unknown capability {self.capability!r}")
        if not _is_normalized_str_tuple(self.required_opcodes):
            raise ValueError(
                f"required_opcodes for {self.capability!r} must be a sorted unique str tuple"
            )
        if self.required_opcodes != normalize_opcodes(self.required_opcodes):
            raise ValueError(
                f"required_opcodes for {self.capability!r} must be normalized lowercase"
            )
        if not _is_normalized_str_tuple(self.required_subjects):
            raise ValueError(
                f"required_subjects for {self.capability!r} must be a sorted unique str tuple"
            )
        if not _is_normalized_str_tuple(self.proof_features):
            raise ValueError(
                f"proof_features for {self.capability!r} must be a sorted unique str tuple"
            )
        if not isinstance(self.evidence_identity, tuple):
            raise ValueError(
                f"evidence_identity for {self.capability!r} must be a tuple of pairs"
            )
        for pair in self.evidence_identity:
            if (
                not isinstance(pair, tuple)
                or len(pair) != 2
                or not isinstance(pair[0], str)
                or not isinstance(pair[1], str)
            ):
                raise ValueError(
                    f"evidence_identity for {self.capability!r} must be string pairs"
                )
        keys = tuple(key for key, _ in self.evidence_identity)
        if keys != tuple(sorted(keys)):
            raise ValueError(
                f"evidence_identity for {self.capability!r} keys must be sorted"
            )
        _validate_sha256_hex("requirement_sha256", self.requirement_sha256)
        expected = requirement_digest(self)
        if self.requirement_sha256 != expected:
            raise ValueError(
                f"requirement_sha256 mismatch for {self.capability!r}"
            )

    @classmethod
    def from_dict(cls, data: Mapping[str, Any]) -> "CapabilityRequirement":
        if not isinstance(data, Mapping):
            raise TypeError("capability requirement must be a mapping")
        raw_identity = data.get("evidence_identity", {})
        if isinstance(raw_identity, dict):
            identity = tuple(
                (str(key), str(value))
                for key, value in sorted(raw_identity.items(), key=lambda item: str(item[0]))
            )
        elif isinstance(raw_identity, (list, tuple)):
            identity = tuple(
                (str(pair[0]), str(pair[1]))
                for pair in raw_identity
                if isinstance(pair, (list, tuple)) and len(pair) == 2
            )
        else:
            identity = ()
        opcodes = data.get("required_opcodes", ()) or ()
        subjects = data.get("required_subjects", ()) or ()
        features = data.get("proof_features", ()) or ()
        return cls(
            capability=str(data.get("capability", "")),
            required_opcodes=normalize_opcodes(opcodes),
            required_subjects=normalize_subjects(subjects),
            proof_features=normalize_proof_features(features),
            evidence_identity=identity,
            requirement_sha256=str(data.get("requirement_sha256", "")),
        )


def requirement_digest(requirement: CapabilityRequirement | Mapping[str, Any]) -> str:
    if isinstance(requirement, CapabilityRequirement):
        payload = requirement.without_hash()
    else:
        payload = {
            key: value
            for key, value in dict(requirement).items()
            if key != "requirement_sha256"
        }
    return canonical_json_sha256(payload)


def build_requirement(
    *,
    capability: str,
    required_opcodes: Iterable[str] = (),
    required_subjects: Iterable[str] = (),
    proof_features: Iterable[str] = (),
    evidence_identity: Mapping[str, str] | Iterable[tuple[str, str]] | None = None,
) -> CapabilityRequirement:
    if evidence_identity is None:
        identity: tuple[tuple[str, str], ...] = ()
    elif isinstance(evidence_identity, Mapping):
        identity = tuple(
            (str(key), str(value))
            for key, value in sorted(evidence_identity.items(), key=lambda item: str(item[0]))
        )
    else:
        identity = tuple((str(key), str(value)) for key, value in evidence_identity)
    draft = CapabilityRequirement(
        capability=capability,
        required_opcodes=normalize_opcodes(required_opcodes),
        required_subjects=normalize_subjects(required_subjects),
        proof_features=normalize_proof_features(proof_features),
        evidence_identity=identity,
        requirement_sha256="",
    )
    return CapabilityRequirement(
        capability=draft.capability,
        required_opcodes=draft.required_opcodes,
        required_subjects=draft.required_subjects,
        proof_features=draft.proof_features,
        evidence_identity=draft.evidence_identity,
        requirement_sha256=requirement_digest(draft),
    )


@dataclass(frozen=True)
class CapabilityRequirements:
    schema_version: int
    requirements: tuple[CapabilityRequirement, ...]
    requirements_sha256: str

    def to_dict(self) -> dict[str, Any]:
        return {
            "schema_version": self.schema_version,
            "requirements": [item.to_dict() for item in self.requirements],
            "requirements_sha256": self.requirements_sha256,
        }

    def without_hash(self) -> dict[str, Any]:
        payload = self.to_dict()
        payload.pop("requirements_sha256", None)
        return payload

    @classmethod
    def from_dict(cls, data: Mapping[str, Any] | None) -> "CapabilityRequirements":
        if data is None:
            raise TypeError("capability_requirements must be a mapping")
        if not isinstance(data, Mapping):
            raise TypeError("capability_requirements must be a mapping")
        raw = data.get("requirements", [])
        if not isinstance(raw, (list, tuple)):
            raise TypeError("capability_requirements.requirements must be a list")
        return cls(
            schema_version=int(
                data.get("schema_version", CAPABILITY_REQUIREMENTS_SCHEMA_VERSION)
            ),
            requirements=tuple(CapabilityRequirement.from_dict(item) for item in raw),
            requirements_sha256=str(data.get("requirements_sha256", "")),
        )

    def by_capability(self) -> dict[str, CapabilityRequirement]:
        return {item.capability: item for item in self.requirements}

    def validate_structure(self) -> None:
        """Strict schema + digest checks; never coerce untrusted fields."""
        if not isinstance(self.schema_version, int):
            raise ValueError("capability_requirements schema_version must be an int")
        if self.schema_version != CAPABILITY_REQUIREMENTS_SCHEMA_VERSION:
            raise ValueError(
                f"unsupported capability_requirements schema_version {self.schema_version}"
            )
        if not isinstance(self.requirements, tuple):
            raise ValueError("capability_requirements.requirements must be a tuple")
        _validate_sha256_hex("requirements_sha256", self.requirements_sha256)
        seen: set[str] = set()
        for item in self.requirements:
            if not isinstance(item, CapabilityRequirement):
                raise ValueError("capability_requirements entries must be CapabilityRequirement")
            item.validate_structure()
            if item.capability in seen:
                raise ValueError(f"duplicate capability requirement {item.capability!r}")
            seen.add(item.capability)
        expected = requirements_digest(self)
        if self.requirements_sha256 != expected:
            raise ValueError("requirements_sha256 mismatch for capability_requirements block")


def validate_capability_requirements_dict(raw: Any) -> None:
    """Strict certificate-path validation of a raw requirements mapping.

    Raises ``ValueError`` with a clear message. Does not coerce untrusted
    field types to "fix" invalid payloads during validation.
    """
    if not isinstance(raw, Mapping):
        raise ValueError("capability_requirements must be a mapping")
    schema_version = raw.get("schema_version", CAPABILITY_REQUIREMENTS_SCHEMA_VERSION)
    if not isinstance(schema_version, int):
        raise ValueError("capability_requirements schema_version must be an int")
    if schema_version != CAPABILITY_REQUIREMENTS_SCHEMA_VERSION:
        raise ValueError(
            f"unsupported capability_requirements schema_version {schema_version}"
        )
    requirements = raw.get("requirements", [])
    if not isinstance(requirements, (list, tuple)):
        raise ValueError("capability_requirements.requirements must be a list")
    _validate_sha256_hex("requirements_sha256", raw.get("requirements_sha256"))

    seen: set[str] = set()
    normalized_entries: list[CapabilityRequirement] = []
    for index, entry in enumerate(requirements):
        if not isinstance(entry, Mapping):
            raise ValueError(
                f"capability_requirements.requirements[{index}] must be a mapping"
            )
        capability = entry.get("capability")
        if not isinstance(capability, str) or not capability:
            raise ValueError(
                f"capability_requirements.requirements[{index}].capability "
                "must be a nonempty string"
            )
        if capability not in KNOWN_CAPABILITIES:
            raise ValueError(f"unknown capability {capability!r}")
        if capability in seen:
            raise ValueError(f"duplicate capability requirement {capability!r}")
        seen.add(capability)

        opcodes = entry.get("required_opcodes", [])
        subjects = entry.get("required_subjects", [])
        features = entry.get("proof_features", [])
        if opcodes is None:
            opcodes = []
        if subjects is None:
            subjects = []
        if features is None:
            features = []
        _validate_raw_str_list("required_opcodes", opcodes)
        _validate_raw_str_list("required_subjects", subjects)
        _validate_raw_str_list("proof_features", features)
        # Opcode tokens must already be lowercase-normalized.
        if tuple(opcodes) != normalize_opcodes(opcodes):
            raise ValueError(
                f"required_opcodes for {capability!r} must be normalized lowercase"
            )
        identity = entry.get("evidence_identity", {})
        if identity is None:
            identity = {}
        _validate_raw_evidence_identity(identity)
        _validate_sha256_hex(
            f"requirement_sha256 for {capability!r}",
            entry.get("requirement_sha256"),
        )

        # Digest over the declared payload (excluding hash), not a coerced rewrite.
        digest_payload = {
            key: value
            for key, value in dict(entry).items()
            if key != "requirement_sha256"
        }
        expected_req = canonical_json_sha256(digest_payload)
        if entry.get("requirement_sha256") != expected_req:
            raise ValueError(f"requirement_sha256 mismatch for {capability!r}")

        normalized_entries.append(
            CapabilityRequirement(
                capability=capability,
                required_opcodes=tuple(opcodes),
                required_subjects=tuple(subjects),
                proof_features=tuple(features),
                evidence_identity=tuple(
                    (key, identity[key]) for key in sorted(identity)
                ),
                requirement_sha256=entry["requirement_sha256"],
            )
        )

    block_payload = {
        key: value
        for key, value in dict(raw).items()
        if key != "requirements_sha256"
    }
    expected_block = canonical_json_sha256(block_payload)
    if raw.get("requirements_sha256") != expected_block:
        raise ValueError("requirements_sha256 mismatch for capability_requirements block")

    # Cross-check dataclass path remains consistent with producer digests.
    rebuilt = CapabilityRequirements(
        schema_version=schema_version,
        requirements=tuple(normalized_entries),
        requirements_sha256=raw["requirements_sha256"],
    )
    rebuilt.validate_structure()


def requirements_digest(payload: CapabilityRequirements | Mapping[str, Any]) -> str:
    if isinstance(payload, CapabilityRequirements):
        body = payload.without_hash()
    else:
        body = {
            key: value
            for key, value in dict(payload).items()
            if key != "requirements_sha256"
        }
    return canonical_json_sha256(body)


def build_requirements(
    requirements: Sequence[CapabilityRequirement],
    *,
    schema_version: int = CAPABILITY_REQUIREMENTS_SCHEMA_VERSION,
) -> CapabilityRequirements:
    ordered = tuple(
        sorted(requirements, key=lambda item: item.capability)
    )
    draft = CapabilityRequirements(
        schema_version=schema_version,
        requirements=ordered,
        requirements_sha256="",
    )
    return CapabilityRequirements(
        schema_version=draft.schema_version,
        requirements=draft.requirements,
        requirements_sha256=requirements_digest(draft),
    )


def _terminal_states(terminals_meta: Any) -> list[Any]:
    if terminals_meta is None:
        return []
    if isinstance(terminals_meta, Mapping):
        # Allow {"original": [...], "candidate": [...]} or {"terminals": [...]}
        collected: list[Any] = []
        for key in ("original", "candidate", "terminals"):
            items = terminals_meta.get(key)
            if isinstance(items, (list, tuple)):
                collected.extend(items)
        if collected:
            return collected
        return []
    if isinstance(terminals_meta, (list, tuple)):
        return list(terminals_meta)
    return []


def _state_of(terminal: Any) -> Any:
    return getattr(terminal, "state", terminal)


def terminals_have_memory_touches(terminals_meta: Any) -> bool:
    """True when any reachable terminal recorded a memory touch/read/write."""
    for terminal in _terminal_states(terminals_meta):
        state = _state_of(terminal)
        for attr in ("memory_touches", "memory_reads", "memory_writes"):
            if getattr(state, attr, None):
                return True
    return False


def terminals_have_summarized_memory_effects(terminals_meta: Any) -> bool:
    for terminal in _terminal_states(terminals_meta):
        state = _state_of(terminal)
        if getattr(state, "memory_effects", None):
            return True
    return False


def result_reports_summarized_memory(result: Any) -> bool:
    """True when loop / callee summaries report memory effects on the result."""
    if getattr(result, "memory_loop", None) is not None:
        return True
    features = getattr(result, "proof_features", None) or []
    if "memory-loop-summary" in features:
        return True
    return False


def ordinary_memory_was_accessed(
    result: Any,
    *,
    terminals_meta: Any = None,
) -> bool:
    """Conservative memory-access signal — not ``\"memory\" in observables``.

    Prefer CFG terminal touches. When terminals are unavailable (e.g. FULL_MATCH
    certificate synthesis), fall back to load/store opcodes in ``opcodes_used``.
    """
    if terminals_have_memory_touches(terminals_meta):
        return True
    if terminals_have_summarized_memory_effects(terminals_meta):
        return True
    if result_reports_summarized_memory(result):
        return True
    # FULL_MATCH / no-CFG fallback: load/store mnemonics imply memory access.
    if terminals_meta is None and opcodes_imply_memory_access(
        getattr(result, "opcodes_used", None) or ()
    ):
        return True
    return False


# Integer / FP load-store families used when CFG terminals are absent.
_MEMORY_ACCESS_OPCODE_PREFIXES: tuple[str, ...] = (
    "lbz", "lhz", "lha", "lwz", "lmw", "lsw", "lwarx",
    "stb", "sth", "stw", "stmw", "stsw", "stwcx",
    "lfs", "lfd", "stfs", "stfd",
    "psq_l", "psq_st",
    "dcbz",
)


def opcodes_imply_memory_access(opcodes: Sequence[str]) -> bool:
    for raw in opcodes:
        name = normalize_opcode(raw)
        if any(name == prefix or name.startswith(prefix) for prefix in _MEMORY_ACCESS_OPCODE_PREFIXES):
            return True
    return False


def _fp_opcodes(opcodes: Sequence[str]) -> frozenset[str]:
    from tools.ppc_equivalence.fp_bitwise import fp_opcodes_among

    return fp_opcodes_among(opcodes)


def _integer_opcodes(opcodes: Sequence[str]) -> tuple[str, ...]:
    fp_ops = _fp_opcodes(opcodes)
    return normalize_opcodes(op for op in opcodes if normalize_opcode(op) not in fp_ops)


def _opcodes_for_fp_capability(capability: str, opcodes: Sequence[str]) -> tuple[str, ...]:
    from tools.ppc_equivalence.fp_bitwise import FP_BITWISE_OPS
    from tools.ppc_equivalence.fp_capabilities import (
        FP_COMPARE_OPS,
        FP_CONVERT_OPS,
        FP_FUSED_ARITH_OPS,
        FP_LOAD_STORE_OPS,
        FP_PAIRED_SINGLE_OPS,
        FP_PSQ_OPS,
        FP_SCALAR_ARITH_OPS,
    )

    family = {
        "fp-bitwise": FP_BITWISE_OPS,
        "fp-load-store": FP_LOAD_STORE_OPS,
        "fp-compare": FP_COMPARE_OPS,
        "fp-convert": FP_CONVERT_OPS,
        "fp-scalar-arithmetic": FP_SCALAR_ARITH_OPS,
        "fp-fused-arithmetic": FP_FUSED_ARITH_OPS,
        "fp-paired-single": FP_PAIRED_SINGLE_OPS,
        "fp-psq": FP_PSQ_OPS,
    }.get(capability)
    if family is None:
        return ()
    return normalize_opcodes(op for op in opcodes if normalize_opcode(op) in family)


def _provenance_identity(result: Any) -> dict[str, str]:
    source_hash = str(getattr(result, "source_hash", "") or "")
    proof_request = str(getattr(result, "proof_request_hash", "") or "") or source_hash
    return {
        "architecture_model": str(getattr(result, "architecture_model", "") or ""),
        "result_format": str(getattr(result, "format", "") or ""),
        "engine_hash": str(getattr(result, "engine_hash", "") or ""),
        "certifier_hash": str(getattr(result, "certifier_hash", "") or ""),
        "source_hash": source_hash,
        "proof_request_hash": proof_request,
        "validation_ledger_hash": str(
            getattr(result, "validation_ledger_hash", "") or ""
        ),
        "git_commit": str(getattr(result, "git_commit", "") or ""),
        "git_dirty": "true" if bool(getattr(result, "git_dirty", False)) else "false",
    }


def derive_capability_requirements(
    result: Any,
    *,
    terminals_meta: Any = None,
    callees_used: Iterable[int | str] | None = None,
    opcodes_used: Iterable[str] | None = None,
    proof_features: Iterable[str] | None = None,
    memory_bus: Mapping[str, Any] | None = None,
) -> CapabilityRequirements:
    """Derive authoritative capability requirements from proof execution evidence.

    Separate ``if`` branches so FP never suppresses memory, calls, loops, or MMIO.
    """
    from tools.ppc_equivalence.fp_capabilities import (
        classify_fp_capabilities,
        traps_enabled_from_result,
    )
    from tools.ppc_equivalence.fp_outcome import capability_tags_for_opcodes
    from tools.ppc_equivalence.fp_traps import capability_tags_for_trap_domain
    from tools.ppc_equivalence.memory_profile import MemoryProfile
    from tools.ppc_equivalence.mmio_capability_obligations import (
        infer_mmio_capabilities_from_memory_bus,
    )
    from tools.ppc_equivalence.result import ProofStatus

    opcodes = normalize_opcodes(
        opcodes_used
        if opcodes_used is not None
        else (getattr(result, "opcodes_used", None) or [])
    )
    features = tuple(
        str(item)
        for item in (
            proof_features
            if proof_features is not None
            else (getattr(result, "proof_features", None) or [])
        )
    )
    if callees_used is not None:
        callees = normalize_subjects(str(item) for item in callees_used)
    else:
        callees = normalize_subjects(
            str(item) for item in (getattr(result, "assumed_callees", None) or [])
        )
    bus = memory_bus if memory_bus is not None else getattr(result, "memory_bus", None)
    if bus is not None and not isinstance(bus, Mapping):
        bus = None

    requirements: list[CapabilityRequirement] = []

    # Always demanded on EQUIVALENT-path derivation (callers may still derive
    # for non-equivalent proofs for audit; integer-core + provenance stay).
    _ = getattr(result, "status", None) is ProofStatus.EQUIVALENT

    requirements.append(
        build_requirement(
            capability="integer-core",
            required_opcodes=_integer_opcodes(opcodes),
        )
    )
    requirements.append(
        build_requirement(
            capability="provenance",
            evidence_identity=_provenance_identity(result),
        )
    )

    if ordinary_memory_was_accessed(result, terminals_meta=terminals_meta):
        environment = getattr(result, "environment", None)
        profile = getattr(environment, "profile", None) if environment else None
        if profile is None or profile == MemoryProfile.ASSUMED_ORDINARY_RAM:
            requirements.append(
                build_requirement(
                    capability="assumed-ordinary-ram",
                    required_subjects=("assumed-ordinary-ram",),
                )
            )
        else:
            subjects: list[str] = []
            if environment is not None and hasattr(environment, "to_dict"):
                subjects.append(canonical_json_sha256(environment.to_dict()))
            elif profile is not None:
                subjects.append(str(getattr(profile, "value", profile)))
            requirements.append(
                build_requirement(
                    capability="bounded-memory",
                    required_subjects=subjects or ("bounded-memory",),
                )
            )

    traps_enabled = traps_enabled_from_result(result)
    fp_caps = set(classify_fp_capabilities(opcodes, traps_enabled=traps_enabled))
    fp_caps |= set(capability_tags_for_opcodes(opcodes))
    fp_caps |= set(capability_tags_for_trap_domain(traps_enabled=traps_enabled))
    # FP domain / coverage without classifiable opcodes still demands scalar.
    from tools.ppc_equivalence.result import (
        FP_COVERAGE_STATUS_NONE,
        proof_fp_coverage_status,
    )

    if proof_fp_coverage_status(result) != FP_COVERAGE_STATUS_NONE and not fp_caps:
        if _fp_opcodes(opcodes) or getattr(result, "floating_point_domain", None):
            fp_caps.add("fp-scalar-arithmetic")
    for capability in sorted(fp_caps):
        req_ops = _opcodes_for_fp_capability(capability, opcodes)
        requirements.append(
            build_requirement(
                capability=capability,
                required_opcodes=req_ops,
            )
        )

    if callees:
        requirements.append(
            build_requirement(
                capability="certified-calls",
                required_subjects=callees,
            )
        )

    if bus is not None or "memory-bus" in features:
        mmio_caps = infer_mmio_capabilities_from_memory_bus(
            bus if isinstance(bus, Mapping) else None,
            proof_features=features,
        )
        if not mmio_caps:
            mmio_caps = frozenset({"mmio-register-bank"})
        device_ids: list[str] = []
        if isinstance(bus, Mapping):
            canonical = bus.get("bus_spec_canonical")
            if isinstance(canonical, dict):
                for device in canonical.get("devices") or []:
                    if isinstance(device, dict) and device.get("id") is not None:
                        device_ids.append(str(device["id"]))
            for key in ("device_ids", "touched_devices"):
                raw = bus.get(key)
                if isinstance(raw, (list, tuple)):
                    device_ids.extend(str(item) for item in raw)
        for capability in sorted(mmio_caps):
            requirements.append(
                build_requirement(
                    capability=capability,
                    required_subjects=device_ids,
                )
            )

    for feature in features:
        mapped = PROOF_FEATURE_CAPABILITY_MAP.get(feature)
        if mapped is None:
            continue
        requirements.append(
            build_requirement(
                capability=mapped,
                proof_features=(feature,),
            )
        )

    # Deduplicate by capability (last write wins for stability — rebuild unique).
    by_name: dict[str, CapabilityRequirement] = {}
    for item in requirements:
        by_name[item.capability] = item
    return build_requirements(tuple(by_name.values()))
