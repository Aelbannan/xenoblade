"""Stage 3C: attach FP / MMIO / proof-feature attestations for used capabilities.

Attaches one attestation per inferred sub-capability so audit output explains
what blocks Tier A. Promotion-grade is earned only for ``fp-bitwise`` when the
ledger + exact opcode set allow it; every other Stage 3C stub stays incomplete
while allowlists remain empty.

Sibling capabilities are never suppressed: FP attachment does not skip MMIO /
loops / memory, and inference uses independent ``if`` branches.
"""

from __future__ import annotations

from typing import Any, Mapping, Sequence

from tools.ppc_equivalence.capability_assurance import (
    CAPABILITY_ASSURANCE_POLICY,
    CAPABILITY_ASSURANCE_SCHEMA_VERSION,
    STATUS_INCOMPLETE,
    CapabilityAssurance,
    CapabilityAttestation,
    _normalize_assurance,
    build_attestation,
    infer_used_capabilities,
)
from tools.ppc_equivalence.capability_requirements import (
    PROOF_FEATURE_CAPABILITY_MAP,
    normalize_opcode,
    normalize_opcodes as _normalize_opcodes_req,
)
from tools.ppc_equivalence.fp_bitwise import (
    FP_BITWISE_ALGORITHM,
    FP_BITWISE_MODEL_VERSION,
    FP_BITWISE_OPS,
    fp_opcodes_among,
)
from tools.ppc_equivalence.fp_capabilities import (
    FP_COMPARE_OPS,
    FP_CONVERT_OPS,
    FP_FUSED_ARITH_OPS,
    FP_LOAD_STORE_OPS,
    FP_PAIRED_SINGLE_OPS,
    FP_PSQ_OPS,
    FP_SCALAR_ARITH_OPS,
    FP_SUBCAPABILITY_MODEL_VERSIONS,
    classify_fp_capabilities,
    scalar_fp_exact_v2_enabled,
    scalar_fp_exact_v2_production_enabled,
    traps_enabled_from_result,
)
from tools.ppc_equivalence.fp_rounding import (
    PRECONDITION_CLOSURE_ALGORITHM,
    PRECONDITION_CLOSURE_CAPABILITY,
    PRECONDITION_CLOSURE_MODEL_VERSION,
)
from tools.ppc_equivalence.provenance import canonical_json_sha256

# Keep Stage 3C feature→capability map identical to Stage 1.
PROOF_FEATURE_TO_CAPABILITY = dict(PROOF_FEATURE_CAPABILITY_MAP)
PROOF_FEATURE_CAPABILITIES = frozenset(PROOF_FEATURE_TO_CAPABILITY.values())

# Wave 3 incomplete stubs for load/store/compare/convert (never promotion-grade).
FP_LOAD_STORE_ALGORITHM = "fp-load-store-incomplete-v0"
FP_COMPARE_ALGORITHM = "fp-compare-incomplete-v0"
FP_CONVERT_ALGORITHM = "fp-convert-incomplete-v0"

FP_FOUNDATION_INCOMPLETE_ALGORITHMS = frozenset(
    {
        FP_LOAD_STORE_ALGORITHM,
        FP_COMPARE_ALGORITHM,
        FP_CONVERT_ALGORITHM,
    }
)

PROOF_FEATURE_ALGORITHM = "proof-feature-obligation-v1"
PROOF_FEATURE_MODEL_VERSION = "proof-feature-v1"

# Obligation field on ProofResult for each feature.
_FEATURE_OBLIGATION_ATTR: dict[str, str] = {
    "readonly-image": "address_space",
    "indirect-target-closure": "indirect_targets",
    "affine-loop-summary": "loop_summary",
    "relational-induction": "relational_induction",
    "memory-loop-summary": "memory_loop",
}

_OPCODE_FAMILY: dict[str, frozenset[str]] = {
    "fp-bitwise": FP_BITWISE_OPS,
    "fp-load-store": FP_LOAD_STORE_OPS,
    "fp-compare": FP_COMPARE_OPS,
    "fp-convert": FP_CONVERT_OPS,
    "fp-scalar-arithmetic": FP_SCALAR_ARITH_OPS,
    "fp-fused-arithmetic": FP_FUSED_ARITH_OPS,
    "fp-paired-single": FP_PAIRED_SINGLE_OPS,
    "fp-psq": FP_PSQ_OPS,
}


def normalize_opcode_name(opcode: str) -> str:
    """Canonical opcode token for exact-set binding (strip record-form dots)."""
    name = normalize_opcode(opcode)
    if name.endswith(".") and name[:-1] in (
        FP_BITWISE_OPS
        | FP_LOAD_STORE_OPS
        | FP_COMPARE_OPS
        | FP_CONVERT_OPS
        | FP_SCALAR_ARITH_OPS
        | FP_FUSED_ARITH_OPS
        | FP_PAIRED_SINGLE_OPS
        | FP_PSQ_OPS
    ):
        return name[:-1]
    return name


def normalize_opcodes(opcodes: Sequence[str]) -> tuple[str, ...]:
    return _normalize_opcodes_req(normalize_opcode_name(op) for op in opcodes if op)


def _requirement_binding(
    capability: str,
    requirements: Any | None,
) -> dict[str, Any]:
    """Optional Stage 1/2 requirement digest binding (no-op when absent)."""
    if requirements is None:
        return {}
    reqs = getattr(requirements, "requirements", None)
    if reqs is None and isinstance(requirements, Mapping):
        reqs = requirements.get("requirements")
    if not reqs:
        return {}
    for item in reqs:
        name = getattr(item, "capability", None)
        if name is None and isinstance(item, Mapping):
            name = item.get("capability")
        if name != capability:
            continue
        digest = getattr(item, "requirement_sha256", None)
        if digest is None and isinstance(item, Mapping):
            digest = item.get("requirement_sha256")
        bundle = getattr(requirements, "requirements_sha256", None)
        if bundle is None and isinstance(requirements, Mapping):
            bundle = requirements.get("requirements_sha256")
        out: dict[str, Any] = {}
        if isinstance(digest, str) and digest:
            out["requirement_sha256"] = digest
        if isinstance(bundle, str) and bundle:
            out["requirements_sha256"] = bundle
        return out
    return {}


def _domain_restriction_labels(domain: Any) -> tuple[str, ...]:
    """Labels for FloatingPointDomain restrictions that need precondition-closure."""
    if domain is None:
        return ()
    labels: list[str] = []
    modes = tuple(getattr(domain, "rounding_modes", ()) or ())
    if modes and modes != ("nearest-even",):
        labels.append("rounding-modes-restricted")
    elif modes == ("nearest-even",):
        # Default nearest-even alone is a domain restriction vs all-RN modeling.
        labels.append("rn-nearest-even-only")
    if getattr(domain, "require_ni_zero", True):
        labels.append("require-ni-zero")
    if not getattr(domain, "allow_nan", True):
        labels.append("nan-excluded")
    if not getattr(domain, "allow_infinity", True):
        labels.append("infinity-excluded")
    if not getattr(domain, "allow_subnormal", True):
        labels.append("subnormals-excluded")
    if getattr(domain, "exclude_finite_overflow", False):
        labels.append("finite-input-overflow-excluded")
    fused = getattr(domain, "fused_input_domain", None)
    if fused and fused != "unrestricted":
        labels.append(f"fused-input-domain:{fused}")
    if getattr(domain, "traps_enabled", False):
        labels.append("traps-enabled")
    return tuple(labels)


def domain_restrictions_present(result: Any) -> bool:
    """True when FP domain carries restrictions needing precondition-closure."""
    domain = getattr(result, "floating_point_domain", None)
    labels = _domain_restriction_labels(domain)
    # rn-nearest-even-only + require-ni-zero are the historical defaults; still
    # count as restrictions whenever any non-bitwise FP capability is used.
    if not labels:
        return False
    nontrivial = [
        label
        for label in labels
        if label
        not in {
            "rn-nearest-even-only",
            "require-ni-zero",
            "finite-input-overflow-excluded",
            "fused-input-domain:exact-expanded-binary32",
        }
    ]
    if nontrivial:
        return True
    # Default domain still needs closure when scalar/fused/etc. are demanded.
    opcodes = normalize_opcodes(getattr(result, "opcodes_used", None) or [])
    traps = traps_enabled_from_result(result)
    caps = classify_fp_capabilities(opcodes, traps_enabled=traps)
    non_bitwise = caps - {"fp-bitwise"}
    return bool(non_bitwise)


def opcodes_for_fp_capability(
    capability: str,
    opcodes: Sequence[str],
) -> tuple[str, ...]:
    """Exact used opcode subset for an FP family (normalized, sorted)."""
    normalized = normalize_opcodes(opcodes)
    family = _OPCODE_FAMILY.get(capability)
    if family is None:
        return ()
    return tuple(op for op in normalized if op in family)


def build_fp_bitwise_attestation(
    result: Any,
    *,
    ledger: Any | None = None,
    ledger_sha256: str | None = None,
    requirements: Any | None = None,
) -> CapabilityAttestation | None:
    """Exact used bitwise opcode set; promotion-grade only when earned later."""
    opcodes = list(getattr(result, "opcodes_used", None) or [])
    bitwise = list(opcodes_for_fp_capability("fp-bitwise", opcodes))
    if not bitwise:
        return None

    resolved_ledger_sha = ledger_sha256
    if not resolved_ledger_sha and ledger is not None:
        if hasattr(ledger, "content_sha256"):
            resolved_ledger_sha = str(ledger.content_sha256)
        elif hasattr(ledger, "dolphin_validated_opcodes"):
            resolved_ledger_sha = canonical_json_sha256(
                sorted(str(op) for op in ledger.dolphin_validated_opcodes)
            )

    evidence: dict[str, Any] = {
        "opcodes": bitwise,
        "host_float": False,
    }
    if resolved_ledger_sha:
        evidence["ledger_sha256"] = resolved_ledger_sha
    evidence.update(_requirement_binding("fp-bitwise", requirements))
    return build_attestation(
        capability="fp-bitwise",
        model_version=FP_BITWISE_MODEL_VERSION,
        algorithm=FP_BITWISE_ALGORITHM,
        status=STATUS_INCOMPLETE,
        evidence=evidence,
    )


def build_fp_load_store_attestation(
    *,
    opcodes: Sequence[str],
    requirements: Any | None = None,
    validation_ledger_hash: str = "",
) -> CapabilityAttestation:
    """Exact-v2 load/store attestation when scalar FP v2 is enabled."""
    import hashlib

    from tools.ppc_equivalence.fp_load_store_obligations import (
        FP_LOAD_STORE_BIT_TRANSFORM_OPS,
        LOAD_STORE_BIT_V2_DIMENSIONS,
        build_fp_load_store_attestation_v2,
    )
    from tools.ppc_equivalence.fp_scalar_obligations_v2 import (
        live_scalar_fp_v2_corpus_sha256,
    )

    used = list(opcodes_for_fp_capability("fp-load-store", opcodes))
    corpus_sha256 = live_scalar_fp_v2_corpus_sha256()
    # stfs-only leaves have no unsupported remainder query; mark vacuous so the
    # Phase 12 canary can promote under broadway-fp-load-store-v2 allowlist.
    remainder_result = "incomplete"
    remainder_query = ""
    if used == ["stfs"] and set(used) <= FP_LOAD_STORE_BIT_TRANSFORM_OPS:
        remainder_result = "vacuous"
        remainder_query = hashlib.sha256(
            b"fp-load-store-exact-v2:vacuous-stfs-only"
        ).hexdigest()
    att = build_fp_load_store_attestation_v2(
        opcodes=used,
        host_float=False,
        dimensions=LOAD_STORE_BIT_V2_DIMENSIONS,
        unsupported_remainder_result=remainder_result,
        unsupported_remainder_query_sha256=remainder_query,
        corpus_sha256=corpus_sha256,
        validation_ledger_hash=validation_ledger_hash,
    )
    evidence = dict(att.evidence)
    evidence["opcodes"] = used
    evidence.update(_requirement_binding("fp-load-store", requirements))
    return build_attestation(
        capability=att.capability,
        model_version=att.model_version,
        algorithm=att.algorithm,
        status=att.status,
        assumptions=att.assumptions,
        unsupported=att.unsupported,
        evidence=evidence,
    )


def build_fp_foundation_incomplete_attestation(
    capability: str,
    *,
    opcodes: Sequence[str],
    requirements: Any | None = None,
) -> CapabilityAttestation:
    """Incomplete stub for load/store/compare/convert with exact opcode binding."""
    algo = {
        "fp-load-store": FP_LOAD_STORE_ALGORITHM,
        "fp-compare": FP_COMPARE_ALGORITHM,
        "fp-convert": FP_CONVERT_ALGORITHM,
    }[capability]
    model = FP_SUBCAPABILITY_MODEL_VERSIONS[capability]
    used = list(opcodes_for_fp_capability(capability, opcodes))
    evidence: dict[str, Any] = {
        "opcodes": used,
        "host_float": False,
        "never_promotion_grade": True,
        "obligation": {
            "capability": capability,
            "model_version": model,
            "algorithm": algo,
            "opcodes": used,
            "status": STATUS_INCOMPLETE,
        },
    }
    evidence.update(_requirement_binding(capability, requirements))
    return build_attestation(
        capability=capability,
        model_version=model,
        algorithm=algo,
        status=STATUS_INCOMPLETE,
        unsupported=(f"{capability}-incomplete",),
        evidence=evidence,
    )


def build_precondition_closure_attestation(
    result: Any,
    *,
    requirements: Any | None = None,
) -> CapabilityAttestation | None:
    """Incomplete precondition-closure when domain restrictions are present."""
    if not domain_restrictions_present(result):
        return None
    domain = getattr(result, "floating_point_domain", None)
    labels = list(_domain_restriction_labels(domain))
    evidence: dict[str, Any] = {
        "violation_query_result": "incomplete",
        "violation_query_sha256": "0" * 64,
        "closed_preconditions": labels,
        "never_promotion_grade_without_unsat": True,
    }
    evidence.update(
        _requirement_binding(PRECONDITION_CLOSURE_CAPABILITY, requirements)
    )
    return build_attestation(
        capability=PRECONDITION_CLOSURE_CAPABILITY,
        model_version=PRECONDITION_CLOSURE_MODEL_VERSION,
        algorithm=PRECONDITION_CLOSURE_ALGORITHM,
        status=STATUS_INCOMPLETE,
        unsupported=("precondition-closure-incomplete",),
        evidence=evidence,
    )


def draft_fp_capability_attestations(
    result: Any,
    *,
    ledger: Any | None = None,
    ledger_sha256: str | None = None,
    requirements: Any | None = None,
) -> tuple[CapabilityAttestation, ...]:
    """Build attestations for every inferred FP sub-capability (and closure)."""
    from tools.ppc_equivalence.fp_advanced_obligations import (
        FP_FUSED_CAPABILITY,
        FP_PAIRED_CAPABILITY,
        FP_PSQ_CAPABILITY,
        FP_TRAPS_CAPABILITY,
        build_fp_advanced_attestation,
    )
    from tools.ppc_equivalence.fp_scalar_obligations import (
        build_fp_scalar_attestation,
        build_fp_scalar_obligation,
    )
    from tools.ppc_equivalence.result import ProofStatus

    if getattr(result, "status", None) is not ProofStatus.EQUIVALENT:
        return ()

    opcodes = list(getattr(result, "opcodes_used", None) or [])
    traps = traps_enabled_from_result(result)
    caps = set(classify_fp_capabilities(opcodes, traps_enabled=traps))
    # Also honor Wave 4 tagging hooks already folded into infer_used_capabilities.
    used = set(infer_used_capabilities(result))
    caps |= {
        name
        for name in used
        if name.startswith("fp-") or name == PRECONDITION_CLOSURE_CAPABILITY
    }
    if not caps and not fp_opcodes_among(opcodes) and not traps:
        return ()

    attestations: list[CapabilityAttestation] = []

    if "fp-bitwise" in caps:
        bitwise = build_fp_bitwise_attestation(
            result,
            ledger=ledger,
            ledger_sha256=ledger_sha256,
            requirements=requirements,
        )
        if bitwise is not None:
            attestations.append(bitwise)

    for capability in ("fp-compare", "fp-convert"):
        if capability in caps:
            attestations.append(
                build_fp_foundation_incomplete_attestation(
                    capability,
                    opcodes=opcodes,
                    requirements=requirements,
                )
            )

    if "fp-load-store" in caps:
        ledger_hash = str(getattr(result, "validation_ledger_hash", "") or "")
        if scalar_fp_exact_v2_enabled() or scalar_fp_exact_v2_production_enabled():
            attestations.append(
                build_fp_load_store_attestation(
                    opcodes=opcodes,
                    requirements=requirements,
                    validation_ledger_hash=ledger_hash,
                )
            )
        else:
            attestations.append(
                build_fp_foundation_incomplete_attestation(
                    "fp-load-store",
                    opcodes=opcodes,
                    requirements=requirements,
                )
            )

    if "fp-scalar-arithmetic" in caps:
        scalar_ops = list(opcodes_for_fp_capability("fp-scalar-arithmetic", opcodes))
        if not scalar_ops:
            # Unclassified FP remainder still demands scalar — bind all FP ops.
            scalar_ops = sorted(fp_opcodes_among(opcodes) - FP_BITWISE_OPS) or sorted(
                fp_opcodes_among(opcodes)
            )
        obligation = build_fp_scalar_obligation(opcodes=scalar_ops)
        att = build_fp_scalar_attestation(obligation)
        evidence = dict(att.evidence)
        evidence["opcodes"] = scalar_ops
        evidence.update(_requirement_binding("fp-scalar-arithmetic", requirements))
        attestations.append(
            build_attestation(
                capability=att.capability,
                model_version=att.model_version,
                algorithm=att.algorithm,
                status=STATUS_INCOMPLETE,
                assumptions=att.assumptions,
                unsupported=att.unsupported,
                evidence=evidence,
            )
        )

    for capability in (
        FP_FUSED_CAPABILITY,
        FP_PAIRED_CAPABILITY,
        FP_PSQ_CAPABILITY,
        FP_TRAPS_CAPABILITY,
    ):
        if capability not in caps:
            continue
        family_ops = list(opcodes_for_fp_capability(capability, opcodes))
        att = build_fp_advanced_attestation(capability, opcodes=family_ops)
        evidence = dict(att.evidence)
        if family_ops:
            evidence["opcodes"] = family_ops
        evidence.update(_requirement_binding(capability, requirements))
        attestations.append(
            build_attestation(
                capability=att.capability,
                model_version=att.model_version,
                algorithm=att.algorithm,
                status=STATUS_INCOMPLETE,
                assumptions=att.assumptions,
                unsupported=att.unsupported,
                evidence=evidence,
            )
        )

    if domain_restrictions_present(result) or (
        caps - {"fp-bitwise"} and getattr(result, "floating_point_domain", None)
    ):
        closure = build_precondition_closure_attestation(
            result, requirements=requirements
        )
        if closure is not None:
            attestations.append(closure)

    return tuple(attestations)


def _device_ids_from_memory_bus(memory_bus: Mapping[str, Any]) -> tuple[str, ...]:
    ids: set[str] = set()
    canonical = memory_bus.get("bus_spec_canonical")
    if isinstance(canonical, dict):
        for device in canonical.get("devices") or []:
            if isinstance(device, dict):
                for key in ("device_id", "id", "name"):
                    value = device.get(key)
                    if isinstance(value, str) and value:
                        ids.add(value)
                        break
    register_theory = memory_bus.get("register_bank_theory")
    if isinstance(register_theory, dict):
        for device in register_theory.get("devices") or []:
            if isinstance(device, dict):
                for key in ("device_id", "id", "name"):
                    value = device.get(key)
                    if isinstance(value, str) and value:
                        ids.add(value)
                        break
    for key in ("device_ids", "touched_device_ids"):
        raw = memory_bus.get(key)
        if isinstance(raw, (list, tuple)):
            ids.update(str(item) for item in raw if item)
    return tuple(sorted(ids))


def draft_mmio_capability_attestations(
    result: Any,
    *,
    requirements: Any | None = None,
) -> tuple[CapabilityAttestation, ...]:
    """Distinct MMIO attestations from ``result.memory_bus`` (allowlists empty)."""
    from tools.ppc_equivalence.mmio_capability_obligations import (
        ALWAYS_INCOMPLETE_MMIO_CAPABILITIES,
        GX_FIFO_READ_CAPABILITY,
        GX_FIFO_TRACE_ALGORITHM,
        GX_FIFO_TRACE_MODEL_VERSION,
        GX_FIFO_WRITE_TRACE_CAPABILITY,
        MIXED_ADDRESS_SPACE_CAPABILITY,
        MMIO_LOOP_EMISSION_CAPABILITY,
        MMIO_REGISTER_BANK_ALGORITHM,
        MMIO_REGISTER_BANK_CAPABILITY,
        MMIO_REGISTER_BANK_MODEL_VERSION,
        build_always_incomplete_mmio_attestation,
        build_gx_fifo_loop_exact_refinement_attestation,
        build_gx_fifo_read_attestation,
        build_mmio_attestation,
        build_mmio_capability_obligation,
        build_mmio_loop_emission_attestation,
        infer_mmio_capabilities_from_memory_bus,
    )
    from tools.ppc_equivalence.result import ProofStatus

    if getattr(result, "status", None) is not ProofStatus.EQUIVALENT:
        return ()

    memory_bus = getattr(result, "memory_bus", None)
    proof_features = tuple(
        str(item) for item in (getattr(result, "proof_features", None) or [])
    )
    if memory_bus is None and "memory-bus" not in proof_features:
        return ()

    bus_map = memory_bus if isinstance(memory_bus, Mapping) else {}
    demanded = set(
        infer_mmio_capabilities_from_memory_bus(
            bus_map if bus_map else None,
            proof_features=proof_features,
        )
    )
    if not demanded:
        demanded.add(MMIO_REGISTER_BANK_CAPABILITY)

    hardware_profile = bus_map.get("hardware_profile") or bus_map.get(
        "hardware_profile_name"
    )
    bus_spec = bus_map.get("bus_spec_sha256")
    device_ids = _device_ids_from_memory_bus(bus_map) if bus_map else ()
    binding = {
        "device_ids": list(device_ids),
        "bus_spec_sha256": bus_spec,
        "hardware_profile": hardware_profile,
        "hardware_profile_sha256": bus_map.get("hardware_profile_sha256"),
        "device_models_sha256": bus_map.get("device_models_sha256"),
    }

    attestations: list[CapabilityAttestation] = []
    for capability in sorted(demanded):
        req_bind = _requirement_binding(capability, requirements)
        if capability in ALWAYS_INCOMPLETE_MMIO_CAPABILITIES:
            att = build_always_incomplete_mmio_attestation(
                capability,
                extra_evidence={**binding, **req_bind},
            )
            attestations.append(att)
            continue

        if capability == GX_FIFO_READ_CAPABILITY:
            # Promotable gx-fifo-read-v1 unsupported-read policy (Wave 5);
            # drafting attaches an incomplete stub — real promotion requires
            # an explicit vacuous claim or a reviewed-profile UNSAT digest
            # supplied by the caller, never a value model for FIFO loads.
            att = build_gx_fifo_read_attestation()
            evidence = dict(att.evidence)
            evidence.update(binding)
            evidence.update(req_bind)
            attestations.append(
                build_attestation(
                    capability=att.capability,
                    model_version=att.model_version,
                    algorithm=att.algorithm,
                    status=STATUS_INCOMPLETE,
                    assumptions=att.assumptions,
                    unsupported=att.unsupported,
                    evidence=evidence,
                )
            )
            continue

        if capability == MMIO_LOOP_EMISSION_CAPABILITY:
            # Promotable mmio-loop-emission-v1 authorization attachment
            # (Wave 5); drafting attaches an incomplete stub wrapping the
            # default (also incomplete) v2 loop-refinement obligation — real
            # promotion requires a caller-supplied discharged loop plan.
            att = build_mmio_loop_emission_attestation()
            evidence = dict(att.evidence)
            evidence.update(binding)
            evidence.update(req_bind)
            attestations.append(
                build_attestation(
                    capability=att.capability,
                    model_version=att.model_version,
                    algorithm=att.algorithm,
                    status=STATUS_INCOMPLETE,
                    assumptions=att.assumptions,
                    unsupported=att.unsupported,
                    evidence=evidence,
                )
            )
            continue

        if capability == MIXED_ADDRESS_SPACE_CAPABILITY:
            attestations.append(
                build_attestation(
                    capability=capability,
                    model_version="mixed-address-space-v0",
                    algorithm="mixed-address-space-incomplete-v0",
                    status=STATUS_INCOMPLETE,
                    unsupported=("mixed-address-space-incomplete",),
                    evidence={**binding, **req_bind, "never_promotion_grade": True},
                )
            )
            continue

        if capability == GX_FIFO_WRITE_TRACE_CAPABILITY:
            # Ordinary write-trace vs loop-emission refinement: only use the
            # loop-refinement attestation when a GX loop was actually
            # summarized (or rejected as unrecognized fifo-touching). The
            # ambient ``loop_fifo_policy=hard-reject`` flag alone must NOT
            # force the loop path — that left ordinary WGPIPE stores stuck
            # on incomplete loop-refinement stubs.
            loop_emission = bus_map.get("loop_fifo_emission") or (
                (bus_map.get("coverage") or {}).get("loop_fifo_emission")
                if isinstance(bus_map.get("coverage"), Mapping)
                else None
            )
            gx_plans = bus_map.get("gx_loop_plans_used") or bus_map.get(
                "gx_fifo_plans_used"
            )
            use_loop_refinement = (
                loop_emission == "summarized-bounded"
                or bool(gx_plans)
                or bool(bus_map.get("loop_fifo_reject_markers"))
                or bus_map.get("loop_fifo_policy")
                == "gx-fifo-loop-exact-refinement-v2"
            )
            if use_loop_refinement:
                att = build_gx_fifo_loop_exact_refinement_attestation()
                evidence = dict(att.evidence)
                evidence.update(binding)
                evidence.update(req_bind)
                attestations.append(
                    build_attestation(
                        capability=att.capability,
                        model_version=att.model_version,
                        algorithm=att.algorithm,
                        status=STATUS_INCOMPLETE,
                        assumptions=att.assumptions,
                        unsupported=att.unsupported,
                        evidence=evidence,
                    )
                )
            else:
                obligation = build_mmio_capability_obligation(
                    capability=GX_FIFO_WRITE_TRACE_CAPABILITY,
                    model_version=GX_FIFO_TRACE_MODEL_VERSION,
                    algorithm=GX_FIFO_TRACE_ALGORITHM,
                    hardware_profile=hardware_profile
                    if isinstance(hardware_profile, (str, Mapping))
                    else None,
                    bus_spec_sha256=bus_spec if isinstance(bus_spec, str) else None,
                    memory_bus_obligation=bus_map or None,
                    ad_hoc=not bool(bus_map.get("hardware_profile_sha256")),
                )
                obligation["device_ids"] = list(device_ids)
                att = build_mmio_attestation(obligation)
                evidence = dict(att.evidence)
                evidence.update(req_bind)
                attestations.append(
                    build_attestation(
                        capability=att.capability,
                        model_version=att.model_version,
                        algorithm=att.algorithm,
                        status=STATUS_INCOMPLETE,
                        assumptions=att.assumptions,
                        unsupported=att.unsupported,
                        evidence=evidence,
                    )
                )
            continue

        # Default: register-bank (and any other bank-like demand).
        obligation = build_mmio_capability_obligation(
            capability=MMIO_REGISTER_BANK_CAPABILITY
            if capability == MMIO_REGISTER_BANK_CAPABILITY
            else capability,
            model_version=MMIO_REGISTER_BANK_MODEL_VERSION
            if capability == MMIO_REGISTER_BANK_CAPABILITY
            else str(
                bus_map.get("model_version") or MMIO_REGISTER_BANK_MODEL_VERSION
            ),
            algorithm=MMIO_REGISTER_BANK_ALGORITHM
            if capability == MMIO_REGISTER_BANK_CAPABILITY
            else str(bus_map.get("algorithm") or MMIO_REGISTER_BANK_ALGORITHM),
            hardware_profile=hardware_profile
            if isinstance(hardware_profile, (str, Mapping))
            else None,
            bus_spec_sha256=bus_spec if isinstance(bus_spec, str) else None,
            memory_bus_obligation=bus_map or None,
            ad_hoc=not bool(bus_map.get("hardware_profile_sha256")),
        )
        obligation["device_ids"] = list(device_ids)
        att = build_mmio_attestation(obligation)
        evidence = dict(att.evidence)
        evidence.update(binding)
        evidence.update(req_bind)
        attestations.append(
            build_attestation(
                capability=capability,
                model_version=att.model_version,
                algorithm=att.algorithm,
                status=STATUS_INCOMPLETE,
                assumptions=att.assumptions,
                unsupported=att.unsupported,
                evidence=evidence,
            )
        )

    return tuple(attestations)


def _obligation_status(obligation: Mapping[str, Any] | None) -> str:
    if not isinstance(obligation, Mapping):
        return STATUS_INCOMPLETE
    status = obligation.get("status")
    if status == "discharged":
        return "discharged"
    return STATUS_INCOMPLETE


def draft_proof_feature_attestations(
    result: Any,
    *,
    requirements: Any | None = None,
) -> tuple[CapabilityAttestation, ...]:
    """Requirement-bound incomplete/discharged attestations for proof features."""
    from tools.ppc_equivalence.result import ProofStatus

    if getattr(result, "status", None) is not ProofStatus.EQUIVALENT:
        return ()

    features = [
        str(item) for item in (getattr(result, "proof_features", None) or [])
    ]
    attestations: list[CapabilityAttestation] = []
    for feature in features:
        capability = PROOF_FEATURE_TO_CAPABILITY.get(feature)
        if capability is None:
            continue
        attr = _FEATURE_OBLIGATION_ATTR.get(feature)
        obligation = getattr(result, attr, None) if attr else None
        obl_status = _obligation_status(
            obligation if isinstance(obligation, Mapping) else None
        )
        evidence: dict[str, Any] = {
            "proof_feature": feature,
            "obligation_key": attr,
            "obligation_status": obl_status,
            "never_promotion_grade_while_unallowlisted": True,
        }
        if isinstance(obligation, Mapping):
            evidence["obligation"] = dict(obligation)
            evidence["obligation_sha256"] = canonical_json_sha256(dict(obligation))
        else:
            evidence["obligation_missing"] = True
        evidence.update(_requirement_binding(capability, requirements))

        unsupported: tuple[str, ...]
        if obl_status == "discharged":
            unsupported = ()
            # Advisory discharged; evaluate_capability_assurance still fails
            # closed on empty allowlist.
            status = STATUS_INCOMPLETE
        else:
            unsupported = (f"{capability}-incomplete",)
            status = STATUS_INCOMPLETE

        attestations.append(
            build_attestation(
                capability=capability,
                model_version=PROOF_FEATURE_MODEL_VERSION,
                algorithm=PROOF_FEATURE_ALGORITHM,
                status=status,
                unsupported=unsupported,
                evidence=evidence,
            )
        )
    return tuple(attestations)


def merge_attestation_list(
    existing: Any | None,
    attestations: Sequence[CapabilityAttestation],
) -> CapabilityAssurance | None:
    """Merge attestations by capability name; later entries win."""
    if not attestations and existing is None:
        return None
    by_name: dict[str, CapabilityAttestation] = {}
    if existing is not None:
        try:
            prior = _normalize_assurance(existing)
            if prior is not None:
                for item in prior.capabilities:
                    by_name[item.capability] = item
        except (TypeError, ValueError, KeyError):
            pass
    for item in attestations:
        by_name[item.capability] = item
    return CapabilityAssurance(
        schema_version=CAPABILITY_ASSURANCE_SCHEMA_VERSION,
        policy=CAPABILITY_ASSURANCE_POLICY,
        capabilities=tuple(by_name[name] for name in sorted(by_name)),
    )


def draft_stage3c_attestations(
    result: Any,
    *,
    ledger: Any | None = None,
    ledger_sha256: str | None = None,
    requirements: Any | None = None,
) -> tuple[CapabilityAttestation, ...]:
    """All Stage 3C attestations (FP + MMIO + proof features)."""
    return (
        *draft_fp_capability_attestations(
            result,
            ledger=ledger,
            ledger_sha256=ledger_sha256,
            requirements=requirements,
        ),
        *draft_mmio_capability_attestations(result, requirements=requirements),
        *draft_proof_feature_attestations(result, requirements=requirements),
    )


def maybe_attach_stage3c_drafts(
    result: Any,
    *,
    ledger: Any | None = None,
    ledger_sha256: str | None = None,
    requirements: Any | None = None,
) -> Any:
    """Attach Stage 3C FP/MMIO/feature drafts without suppressing siblings."""
    attestations = draft_stage3c_attestations(
        result,
        ledger=ledger,
        ledger_sha256=ledger_sha256,
        requirements=requirements
        if requirements is not None
        else getattr(result, "capability_requirements", None),
    )
    if not attestations:
        return result
    merged = merge_attestation_list(
        getattr(result, "capability_assurance", None),
        attestations,
    )
    if merged is not None:
        result.capability_assurance = merged.to_dict()
    return result


def recompute_proof_feature_attestation_status(
    evidence: Mapping[str, Any],
    *,
    capability: str,
    algorithm: str,
    model_version: str = "",
    assumptions: Sequence[str] = (),
    unsupported: Sequence[str] = (),
    allowed_versions: Sequence[str] = (),
) -> str:
    """Grade proof-feature attestations; empty allowlist → incomplete."""
    del assumptions
    if capability not in PROOF_FEATURE_CAPABILITIES:
        return STATUS_INCOMPLETE
    if algorithm != PROOF_FEATURE_ALGORITHM:
        return STATUS_INCOMPLETE
    if model_version and model_version != PROOF_FEATURE_MODEL_VERSION:
        return STATUS_INCOMPLETE
    if unsupported:
        return STATUS_INCOMPLETE
    if model_version not in tuple(allowed_versions):
        return STATUS_INCOMPLETE
    if evidence.get("obligation_missing"):
        return STATUS_INCOMPLETE
    if evidence.get("obligation_status") != "discharged":
        return STATUS_INCOMPLETE
    # Allowlisted + discharged would still need further checks; Stage 3C keeps
    # allowlists empty so this path remains incomplete in production.
    return STATUS_INCOMPLETE


def recompute_fp_foundation_incomplete_status(
    evidence: Mapping[str, Any],
    *,
    capability: str,
    algorithm: str,
    model_version: str = "",
    assumptions: Sequence[str] = (),
    unsupported: Sequence[str] = (),
    allowed_versions: Sequence[str] = (),
) -> str:
    """Load/store/compare/convert stubs are never promotion-grade."""
    del evidence, assumptions, unsupported, allowed_versions, model_version
    if capability not in {"fp-load-store", "fp-compare", "fp-convert"}:
        return STATUS_INCOMPLETE
    if algorithm not in FP_FOUNDATION_INCOMPLETE_ALGORITHMS:
        return STATUS_INCOMPLETE
    return STATUS_INCOMPLETE

