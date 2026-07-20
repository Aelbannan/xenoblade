"""MMIO capability obligations for capability-assurance Wave 3.

Extends schema-v2 memory-bus discharge evidence into per-capability
attestations (``mmio-register-bank``, ``gx-fifo-write-trace``, …). Promotion
grade requires a reviewed ``hardware_profile_sha256`` match, complete per-side
UNSAT digests, and an allowlisted model version. Ad-hoc bus maps and
``gx-fifo-read`` / DMA remain incomplete.
"""

from __future__ import annotations

from typing import Any, Mapping, Sequence

from tools.ppc_equivalence.hardware_profile import (
    REVIEWED_HARDWARE_PROFILES,
    SOURCE_AD_HOC_BUS,
    SOURCE_HARDWARE_PROFILE,
    compute_hardware_profile_sha256,
    device_models_sha256,
    is_reviewed_hardware_profile,
    load_hardware_profile,
)
from tools.ppc_equivalence.provenance import canonical_json_sha256

_SHA256_LEN = 64

MMIO_CAPABILITY_OBLIGATION_SCHEMA_VERSION = 2

MMIO_REGISTER_BANK_CAPABILITY = "mmio-register-bank"
MMIO_REGISTER_BANK_MODEL_VERSION = "wii-mmio-bank-v2"
MMIO_REGISTER_BANK_ALGORITHM = "mmio-register-bank-v2"

GX_FIFO_WRITE_TRACE_CAPABILITY = "gx-fifo-write-trace"
GX_FIFO_TRACE_MODEL_VERSION = "gx-fifo-trace-v1"
GX_FIFO_TRACE_ALGORITHM = "gx-fifo-trace-v1"

GX_FIFO_READ_CAPABILITY = "gx-fifo-read"
GX_FIFO_READ_MODEL_VERSION = "gx-fifo-read-v0"
GX_FIFO_READ_ALGORITHM = "gx-fifo-read-incomplete-v0"

MMIO_READ_SIDE_EFFECTS_CAPABILITY = "mmio-read-side-effects"
MMIO_EXTERNAL_INPUT_CAPABILITY = "mmio-external-input"
MMIO_LOOP_EMISSION_CAPABILITY = "mmio-loop-emission"
MIXED_ADDRESS_SPACE_CAPABILITY = "mixed-address-space-routing"
DMA_INTERRUPT_CAPABILITY = "dma-interrupt-effects"

# Algorithms known to capability_assurance (promotion-grade path only for
# register-bank / fifo-write when fully discharged + allowlisted).
MMIO_ATTESTATION_ALGORITHMS = frozenset(
    {
        MMIO_REGISTER_BANK_ALGORITHM,
        GX_FIFO_TRACE_ALGORITHM,
        GX_FIFO_READ_ALGORITHM,
        "mmio-read-side-effects-incomplete-v0",
        "mmio-external-input-incomplete-v0",
        "mmio-loop-emission-incomplete-v0",
        "mixed-address-space-incomplete-v0",
        "dma-interrupt-incomplete-v0",
    }
)

# Never promotion-grade under Wave 3 foundations.
ALWAYS_INCOMPLETE_MMIO_CAPABILITIES = frozenset(
    {
        GX_FIFO_READ_CAPABILITY,
        MMIO_LOOP_EMISSION_CAPABILITY,
        DMA_INTERRUPT_CAPABILITY,
    }
)

_SIDE_QUERY_KEYS = (
    "address_coverage",
    "unsupported_access",
    "event_trace_complete",
)


def _is_sha256(value: Any) -> bool:
    if not isinstance(value, str) or len(value) != _SHA256_LEN:
        return False
    try:
        int(value, 16)
    except ValueError:
        return False
    return True


def _side_query_block(
    *,
    result: str = "pending",
    query_sha256: str | None = None,
) -> dict[str, Any]:
    block: dict[str, Any] = {"result": result}
    if query_sha256 is not None:
        block["query_sha256"] = query_sha256
    return block


def _empty_side() -> dict[str, Any]:
    return {
        key: _side_query_block()
        for key in _SIDE_QUERY_KEYS
    }


def classify_mmio_source(
    *,
    hardware_profile: Mapping[str, Any] | None,
    ad_hoc: bool = False,
) -> str:
    if ad_hoc:
        return SOURCE_AD_HOC_BUS
    if hardware_profile is not None and is_reviewed_hardware_profile(hardware_profile):
        return SOURCE_HARDWARE_PROFILE
    return SOURCE_AD_HOC_BUS


def build_mmio_capability_obligation(
    *,
    capability: str = MMIO_REGISTER_BANK_CAPABILITY,
    model_version: str = MMIO_REGISTER_BANK_MODEL_VERSION,
    algorithm: str = MMIO_REGISTER_BANK_ALGORITHM,
    hardware_profile: Mapping[str, Any] | str | None = None,
    bus_spec_sha256: str | None = None,
    memory_bus_obligation: Mapping[str, Any] | None = None,
    original: Mapping[str, Any] | None = None,
    candidate: Mapping[str, Any] | None = None,
    ad_hoc: bool = False,
    status: str = "incomplete",
) -> dict[str, Any]:
    """Build a schema-v2+ MMIO capability obligation block.

    When ``hardware_profile`` is a reviewed profile name/object, bind its
    digests. Otherwise mark source as ad-hoc (never promotion-grade).
    Per-side query blocks may be pending; only ``result=unsat`` with digests
    can authorize promotion when allowlisted.
    """
    profile_obj: dict[str, Any] | None = None
    if isinstance(hardware_profile, str):
        try:
            profile_obj = load_hardware_profile(hardware_profile)
        except (OSError, ValueError):
            profile_obj = None
            ad_hoc = True
    elif isinstance(hardware_profile, Mapping):
        profile_obj = dict(hardware_profile)

    source = classify_mmio_source(hardware_profile=profile_obj, ad_hoc=ad_hoc)

    resolved_bus_spec = bus_spec_sha256
    if resolved_bus_spec is None and isinstance(memory_bus_obligation, Mapping):
        raw = memory_bus_obligation.get("bus_spec_sha256")
        if isinstance(raw, str):
            resolved_bus_spec = raw

    obligation: dict[str, Any] = {
        "schema_version": MMIO_CAPABILITY_OBLIGATION_SCHEMA_VERSION,
        "capability": capability,
        "model_version": model_version,
        "algorithm": algorithm,
        "status": status,
        "source": source,
        "original": dict(original) if original is not None else _empty_side(),
        "candidate": dict(candidate) if candidate is not None else _empty_side(),
    }

    if profile_obj is not None:
        digest = profile_obj.get("profile_sha256")
        if not _is_sha256(digest):
            digest = compute_hardware_profile_sha256(profile_obj)
        obligation["hardware_profile"] = str(
            profile_obj.get("profile") or profile_obj.get("platform_profile")
        )
        obligation["hardware_profile_sha256"] = digest
        obligation["device_models_sha256"] = device_models_sha256(profile_obj)
    else:
        obligation["hardware_profile"] = None
        obligation["hardware_profile_sha256"] = None
        obligation["device_models_sha256"] = None

    if resolved_bus_spec is not None:
        obligation["bus_spec_sha256"] = resolved_bus_spec

    if isinstance(memory_bus_obligation, Mapping):
        # Attach discharged schema-v2 bus evidence by reference (not re-hashed).
        obligation["memory_bus"] = {
            key: memory_bus_obligation[key]
            for key in (
                "schema_version",
                "algorithm",
                "status",
                "bus_spec_sha256",
                "unsupported_access",
                "access_coverage",
            )
            if key in memory_bus_obligation
        }

    return obligation


def _validate_side_block(side: Any, *, label: str, require_unsat: bool) -> str | None:
    if not isinstance(side, dict):
        return f"{label} must be an object"
    for key in _SIDE_QUERY_KEYS:
        block = side.get(key)
        if not isinstance(block, dict):
            return f"{label}.{key} must be an object"
        result = block.get("result")
        if result not in {"unsat", "pending", "sat", "unknown", "vacuous"}:
            return f"{label}.{key}.result unsupported ({result!r})"
        if require_unsat:
            if result != "unsat":
                return f"{label}.{key}.result must be unsat for promotion-grade"
            if not _is_sha256(block.get("query_sha256")):
                return f"{label}.{key}.query_sha256 must be a 64-hex digest"
        elif result == "unsat" and "query_sha256" in block:
            if not _is_sha256(block.get("query_sha256")):
                return f"{label}.{key}.query_sha256 must be a 64-hex digest"
    return None


def validate_mmio_capability_obligation(
    obligation: Mapping[str, Any],
    *,
    require_promotion_shape: bool = False,
) -> str | None:
    """Structural validation; fail closed on malformed payloads.

    When ``require_promotion_shape`` is true (or ``status`` claims
    ``promotion-grade``), enforce reviewed profile digests + complete UNSAT
    side queries. Ad-hoc sources never pass the promotion shape.
    """
    if not isinstance(obligation, Mapping):
        return "mmio capability obligation must be a mapping"
    schema = obligation.get("schema_version")
    if schema != MMIO_CAPABILITY_OBLIGATION_SCHEMA_VERSION:
        return (
            f"mmio capability obligation schema_version must be "
            f"{MMIO_CAPABILITY_OBLIGATION_SCHEMA_VERSION}"
        )
    capability = obligation.get("capability")
    if not isinstance(capability, str) or not capability:
        return "mmio capability obligation missing capability"
    model_version = obligation.get("model_version")
    if not isinstance(model_version, str) or not model_version:
        return "mmio capability obligation missing model_version"
    algorithm = obligation.get("algorithm")
    if not isinstance(algorithm, str) or not algorithm:
        return "mmio capability obligation missing algorithm"

    status = obligation.get("status")
    want_promotion = require_promotion_shape or status == "promotion-grade"

    for side_name in ("original", "candidate"):
        reason = _validate_side_block(
            obligation.get(side_name),
            label=f"mmio.{side_name}",
            require_unsat=want_promotion,
        )
        if reason is not None:
            return reason

    source = obligation.get("source")
    if source not in {SOURCE_HARDWARE_PROFILE, SOURCE_AD_HOC_BUS}:
        return f"mmio capability obligation source unsupported ({source!r})"

    if want_promotion:
        if source != SOURCE_HARDWARE_PROFILE:
            return "mmio promotion-grade requires source=hardware-profile"
        profile_name = obligation.get("hardware_profile")
        profile_digest = obligation.get("hardware_profile_sha256")
        if not isinstance(profile_name, str) or profile_name not in REVIEWED_HARDWARE_PROFILES:
            return "mmio promotion-grade requires a reviewed hardware_profile"
        if not _is_sha256(profile_digest):
            return "mmio.hardware_profile_sha256 must be a 64-hex digest"
        try:
            reviewed = load_hardware_profile(profile_name)
        except (OSError, ValueError) as exc:
            return f"mmio hardware profile load failed: {exc}"
        if reviewed.get("profile_sha256") != profile_digest:
            return (
                "mmio.hardware_profile_sha256 does not match reviewed "
                f"hardware profile {profile_name}"
            )
        expected_models = device_models_sha256(reviewed)
        if obligation.get("device_models_sha256") != expected_models:
            return "mmio.device_models_sha256 does not match reviewed hardware profile"
        if not _is_sha256(obligation.get("bus_spec_sha256")):
            return "mmio promotion-grade requires bus_spec_sha256 (64-hex)"
        if capability in ALWAYS_INCOMPLETE_MMIO_CAPABILITIES:
            return f"mmio capability {capability!r} cannot be promotion-grade"

    elif obligation.get("hardware_profile_sha256") is not None:
        if not _is_sha256(obligation.get("hardware_profile_sha256")):
            return "mmio.hardware_profile_sha256 must be a 64-hex digest or null"
        profile_name = obligation.get("hardware_profile")
        if isinstance(profile_name, str) and profile_name in REVIEWED_HARDWARE_PROFILES:
            try:
                reviewed = load_hardware_profile(profile_name)
            except (OSError, ValueError) as exc:
                return f"mmio hardware profile load failed: {exc}"
            if reviewed.get("profile_sha256") != obligation.get("hardware_profile_sha256"):
                return (
                    "mmio.hardware_profile_sha256 does not match reviewed "
                    f"hardware profile {profile_name}"
                )

    if obligation.get("bus_spec_sha256") is not None and not _is_sha256(
        obligation.get("bus_spec_sha256")
    ):
        return "mmio.bus_spec_sha256 must be a 64-hex digest"
    if obligation.get("device_models_sha256") is not None and not _is_sha256(
        obligation.get("device_models_sha256")
    ):
        return "mmio.device_models_sha256 must be a 64-hex digest"

    return None


def obligation_is_promotion_grade(obligation: Mapping[str, Any]) -> bool:
    """True when obligation validates and claims promotion-grade honestly."""
    if obligation.get("status") != "promotion-grade":
        return False
    return validate_mmio_capability_obligation(
        obligation, require_promotion_shape=True
    ) is None


def discharged_side_from_memory_bus(
    memory_bus_obligation: Mapping[str, Any],
    *,
    side: str,
) -> dict[str, Any]:
    """Lift schema-v2 bus unsupported-access into an MMIO side query block."""
    unsupported = memory_bus_obligation.get("unsupported_access") or {}
    side_block = unsupported.get(side) if isinstance(unsupported, dict) else None
    query_sha = None
    result = "pending"
    if isinstance(side_block, dict):
        if side_block.get("status") in {"discharged", "vacuously-discharged"}:
            result = "unsat"
            digest = side_block.get("sha256") or side_block.get("query_sha256")
            if isinstance(digest, str):
                query_sha = digest
        elif side_block.get("result") == "unsat":
            result = "unsat"
            digest = side_block.get("query_sha256") or side_block.get("sha256")
            if isinstance(digest, str):
                query_sha = digest

    coverage = memory_bus_obligation.get("access_coverage") or {}
    coverage_result = "pending"
    coverage_sha = None
    if isinstance(coverage, dict) and coverage.get("status") == "complete":
        coverage_result = "unsat"
        digest = coverage.get("sha256")
        if isinstance(digest, str):
            coverage_sha = digest

    # Event-trace completeness is not yet engine-discharged for Wave 3.
    event_result = "pending"
    event_sha = None
    fifo = memory_bus_obligation.get("gxfifo_trace")
    if isinstance(fifo, dict) and fifo.get("status") == "complete":
        event_result = "unsat"
        digest = fifo.get("sha256") or fifo.get("query_sha256")
        if isinstance(digest, str):
            event_sha = digest

    return {
        "address_coverage": _side_query_block(
            result=coverage_result, query_sha256=coverage_sha
        ),
        "unsupported_access": _side_query_block(
            result=result, query_sha256=query_sha
        ),
        "event_trace_complete": _side_query_block(
            result=event_result, query_sha256=event_sha
        ),
    }


def recompute_mmio_attestation_status(
    evidence: Mapping[str, Any],
    *,
    capability: str,
    algorithm: str,
    model_version: str = "",
    assumptions: Sequence[str] = (),
    unsupported: Sequence[str] = (),
    allowed_versions: Sequence[str] = (),
) -> str:
    """Recompute MMIO attestation grade; ignore caller-supplied status."""
    from tools.ppc_equivalence.capability_assurance import (
        STATUS_INCOMPLETE,
        STATUS_PROMOTION_GRADE,
        STATUS_SCOPED_ASSUMPTION,
    )

    if unsupported:
        return STATUS_INCOMPLETE
    if capability in ALWAYS_INCOMPLETE_MMIO_CAPABILITIES:
        return STATUS_INCOMPLETE
    if algorithm == GX_FIFO_READ_ALGORITHM:
        return STATUS_INCOMPLETE
    if algorithm.endswith("-incomplete-v0"):
        return STATUS_INCOMPLETE

    # Empty allowlist fail-closes (Wave 3 default until canary).
    if model_version not in tuple(allowed_versions):
        return STATUS_INCOMPLETE

    obligation = (
        evidence.get("mmio")
        or evidence.get("obligation")
        or evidence.get(capability)
    )
    if not isinstance(obligation, dict):
        return STATUS_INCOMPLETE

    reason = validate_mmio_capability_obligation(obligation)
    if reason is not None:
        return STATUS_INCOMPLETE

    source = obligation.get("source")
    if source == SOURCE_AD_HOC_BUS:
        # Ad-hoc CLI / caller bus maps never promotion-grade.
        return STATUS_SCOPED_ASSUMPTION if assumptions else STATUS_INCOMPLETE

    if obligation_is_promotion_grade(obligation):
        if assumptions:
            return STATUS_SCOPED_ASSUMPTION
        return STATUS_PROMOTION_GRADE
    return STATUS_INCOMPLETE


def build_mmio_attestation(
    obligation: Mapping[str, Any],
    *,
    assumptions: Sequence[str] = (),
    unsupported: Sequence[str] = (),
) -> Any:
    """Emit a CapabilityAttestation from an MMIO capability obligation."""
    from tools.ppc_equivalence.capability_assurance import (
        STATUS_INCOMPLETE,
        build_attestation,
    )

    capability = str(obligation.get("capability", MMIO_REGISTER_BANK_CAPABILITY))
    algorithm = str(obligation.get("algorithm", MMIO_REGISTER_BANK_ALGORITHM))
    model_version = str(
        obligation.get("model_version", MMIO_REGISTER_BANK_MODEL_VERSION)
    )
    status = recompute_mmio_attestation_status(
        {"mmio": dict(obligation)},
        capability=capability,
        algorithm=algorithm,
        model_version=model_version,
        assumptions=assumptions,
        unsupported=unsupported,
        allowed_versions=(),  # advisory; evaluate_capability_assurance recomputes
    )
    return build_attestation(
        capability=capability,
        model_version=model_version,
        algorithm=algorithm,
        status=status if status else STATUS_INCOMPLETE,
        assumptions=tuple(assumptions),
        unsupported=tuple(unsupported),
        evidence={
            "mmio": dict(obligation),
            "hardware_profile": obligation.get("hardware_profile"),
            "hardware_profile_sha256": obligation.get("hardware_profile_sha256"),
            "bus_spec_sha256": obligation.get("bus_spec_sha256"),
            "device_models_sha256": obligation.get("device_models_sha256"),
            "source": obligation.get("source"),
        },
    )


def infer_mmio_capabilities_from_memory_bus(
    memory_bus: Mapping[str, Any] | None,
    *,
    proof_features: Sequence[str] = (),
) -> frozenset[str]:
    """Derive distinct MMIO capability demands from a memory_bus obligation."""
    features = set(proof_features)
    if "memory-bus" not in features and not memory_bus:
        return frozenset()

    used: set[str] = set()
    if not isinstance(memory_bus, Mapping):
        # Coarse fallback when only the feature flag is present.
        used.add(MMIO_REGISTER_BANK_CAPABILITY)
        return frozenset(used)

    theories: set[str] = set()
    canonical = memory_bus.get("bus_spec_canonical")
    if isinstance(canonical, dict):
        for device in canonical.get("devices") or []:
            if isinstance(device, dict) and device.get("theory"):
                theories.add(str(device["theory"]))

    register_theory = memory_bus.get("register_bank_theory")
    if isinstance(register_theory, dict) and register_theory.get("status") == "present":
        theories.add("mmio-register-bank")
        for device in register_theory.get("devices") or []:
            if isinstance(device, dict) and device.get("theory"):
                theories.add(str(device["theory"]))

    fifo_theory = memory_bus.get("fifo_theory")
    if isinstance(fifo_theory, dict) and fifo_theory.get("status") == "present":
        theories.add("gxfifo-stream")

    if "mmio-register-bank" in theories or memory_bus.get("register_bank_extensional"):
        used.add(MMIO_REGISTER_BANK_CAPABILITY)
    if "gxfifo-stream" in theories or memory_bus.get("gxfifo_trace"):
        used.add(GX_FIFO_WRITE_TRACE_CAPABILITY)

    # FIFO reads are unsupported — demand the incomplete capability when
    # attested on the obligation or when store_kinds omit reads but load hits.
    rejects = [
        str(item)
        for item in (memory_bus.get("cfg_rejection_reasons") or [])
    ]
    load_kinds = {
        str(item) for item in (memory_bus.get("load_kinds") or [])
    }
    if any("fifo-read" in item or "gxfifo-read" in item for item in rejects):
        used.add(GX_FIFO_READ_CAPABILITY)
    if memory_bus.get("fifo_read_attempted") or memory_bus.get("gx_fifo_read"):
        used.add(GX_FIFO_READ_CAPABILITY)

    coverage = memory_bus.get("coverage") or {}
    if isinstance(coverage, dict):
        if coverage.get("mixed_space_symbolic_mmio") == "rejected":
            used.add(MIXED_ADDRESS_SPACE_CAPABILITY)
        if coverage.get("bounded_summarized_fifo_emission") == "unsupported":
            if "gxfifo-stream" in theories or memory_bus.get("loop_fifo_policy"):
                used.add(MMIO_LOOP_EMISSION_CAPABILITY)

    if memory_bus.get("loop_fifo_policy") or memory_bus.get("loop_fifo_reject_markers"):
        if "gxfifo-stream" in theories or memory_bus.get("loop_fifo_reject_markers"):
            used.add(MMIO_LOOP_EMISSION_CAPABILITY)

    mixed_policy = memory_bus.get("mixed_space_symbolic_mmio")
    regions = memory_bus.get("regions") or []
    kinds = {
        str(region.get("kind"))
        for region in regions
        if isinstance(region, dict)
    }
    if mixed_policy == "fail-closed" and "mmio" in kinds and (
        "ram" in kinds or "rom-image" in kinds
    ):
        # Mixed map present — capability demanded when symbolic routing runs.
        if any("mixed" in item for item in rejects) or memory_bus.get(
            "mixed_space_accessed"
        ):
            used.add(MIXED_ADDRESS_SPACE_CAPABILITY)

    # W1C / read-clear register specs imply read side-effect capability.
    if memory_bus.get("has_read_side_effects") or memory_bus.get("w1c_or_read_clear"):
        used.add(MMIO_READ_SIDE_EFFECTS_CAPABILITY)
    if memory_bus.get("has_external_input") or memory_bus.get("external_input"):
        used.add(MMIO_EXTERNAL_INPUT_CAPABILITY)
    if memory_bus.get("dma_or_interrupt") or memory_bus.get("has_dma_interrupt"):
        used.add(DMA_INTERRUPT_CAPABILITY)

    # Default coarse demand for any memory-bus proof.
    if not used:
        used.add(MMIO_REGISTER_BANK_CAPABILITY)
    elif MMIO_REGISTER_BANK_CAPABILITY not in used and "gxfifo-stream" not in theories:
        # Pure feature-flag / empty theories still demand register-bank.
        if not theories and not load_kinds:
            used.add(MMIO_REGISTER_BANK_CAPABILITY)

    return frozenset(used)


def mmio_obligation_digest(obligation: Mapping[str, Any]) -> str:
    """Canonical digest helper for tests / certificate binding."""
    return canonical_json_sha256(dict(obligation))
