"""MMIO capability obligations for capability-assurance Wave 3–5.

Extends schema-v2 memory-bus discharge evidence into per-capability
attestations (``mmio-register-bank``, ``gx-fifo-write-trace``, …). Promotion
grade requires a reviewed ``hardware_profile_sha256`` match, complete per-side
UNSAT digests, and an allowlisted model version. Ad-hoc bus maps remain
incomplete forever.

Wave 5 (see ``GX_FIFO_TIER_A.md``) promotes ``gx-fifo-write-trace`` to model
``gx-fifo-trace-v2``, splits loop-emission refinement into a production
per-block ``gx-fifo-loop-exact-refinement-v2`` algorithm (real per-block UNSAT
structure sourced from the ``gx_fifo_loop.py`` discharge module — never a bare
caller-supplied ``result=unsat``) while permanently rejecting the legacy
digest-only ``gx-fifo-loop-refinement-v1`` shape for promotion, promotes
``gx-fifo-read`` to a promotable unsupported-read *policy* attestation
(``gx-fifo-read-v1`` — never a FIFO-load value model), and promotes
``mmio-loop-emission`` to an authorization attachment (``mmio-loop-emission-v1``)
that wraps a discharged ``gx-fifo-loop-exact-refinement-v2`` plan. All four
remain gated by empty allowlists until an explicit canary (see
``validation_ledger.yaml`` / ``capability_manifest*.json``).

Always-incomplete regardless of allowlist: ``mmio-read-side-effects``,
``mmio-external-input``, and ``dma-interrupt-effects`` (Dolphin hardware
harness pending).
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
GX_FIFO_TRACE_MODEL_VERSION = "gx-fifo-trace-v2"
GX_FIFO_TRACE_ALGORITHM = "gx-fifo-trace-v2"

# Production per-block exact refinement (Wave 5): promotion-grade requires
# real per-block UNSAT structure sourced from the ``gx_fifo_loop.py``
# discharge module (paired summary + witness), not a bare caller-supplied
# top-level ``result=unsat``. See GX_FIFO_TIER_A.md "Authorization".
GX_FIFO_LOOP_REFINEMENT_ALGORITHM = "gx-fifo-loop-exact-refinement-v2"  # production
GX_FIFO_LOOP_REFINEMENT_SCHEMA_VERSION = 2
GX_FIFO_LOOP_REFINEMENT_CLAIM = (
    "ordinary-n-fifo-writes-equiv-summarized-n-event-trace"
)
# Per-side shape mirrors tools.ppc_equivalence.gx_fifo_loop_discharge's
# GxFifoLoopDischargeResult / refinement_for_plan() output exactly:
# {"entry_guard": {...}, "refinement": {"algorithm", "status", <blocks>}}.
# GX_FIFO_LOOP_SIDE_KEYS are the two top-level side keys; the six
# ``refinement`` sub-blocks (body_step, event_identity, postcondition,
# termination, bound, footprint) are validated by that module's own
# ``validate_gx_fifo_loop_refinement_v2`` — never re-derived here.
GX_FIFO_LOOP_SIDE_KEYS = frozenset({"entry_guard", "refinement"})

# Legacy v1 digest-only shape (single top-level result=unsat per side, no
# per-block structure). Old certificates using this algorithm still validate
# structurally, but recompute_mmio_attestation_status rejects it for
# promotion unconditionally — it must NEVER become promotion-grade.
GX_FIFO_LOOP_REFINEMENT_ALGORITHM_V1_LEGACY = "gx-fifo-loop-refinement-v1"
GX_FIFO_LOOP_REFINEMENT_SCHEMA_VERSION_V1_LEGACY = 1

GX_FIFO_READ_CAPABILITY = "gx-fifo-read"
# Production unsupported-read *policy* attestation (Wave 5, outcome 3 in
# GX_FIFO_TIER_A.md): promotion-grade when the unsupported-read policy,
# corpus negatives, and recomputation gates are complete for either a
# reviewed profile with a real ``path ∧ GX-read`` UNSAT digest, or a vacuous
# no-read-demand target. This is never a FIFO-load value model.
GX_FIFO_READ_MODEL_VERSION = "gx-fifo-read-v1"
GX_FIFO_READ_ALGORITHM = "gx-fifo-read-v1"  # unsupported-policy complete, not value model
GX_FIFO_READ_SCHEMA_VERSION = 1

# Legacy always-incomplete v0 shape retained for parsing old certificates.
GX_FIFO_READ_MODEL_VERSION_V0_LEGACY = "gx-fifo-read-v0"
GX_FIFO_READ_ALGORITHM_V0_LEGACY = "gx-fifo-read-incomplete-v0"

MMIO_READ_SIDE_EFFECTS_CAPABILITY = "mmio-read-side-effects"
MMIO_READ_SIDE_EFFECTS_MODEL_VERSION = "mmio-read-side-effects-v0"
MMIO_READ_SIDE_EFFECTS_ALGORITHM = "mmio-read-side-effects-incomplete-v0"

MMIO_EXTERNAL_INPUT_CAPABILITY = "mmio-external-input"
MMIO_EXTERNAL_INPUT_MODEL_VERSION = "mmio-external-input-v0"
MMIO_EXTERNAL_INPUT_ALGORITHM = "mmio-external-input-incomplete-v0"

MMIO_LOOP_EMISSION_CAPABILITY = "mmio-loop-emission"
# Production authorization attachment (Wave 5): wraps a discharged
# gx-fifo-loop-exact-refinement-v2 plan plus the narrow mmio-loop-emission-v1
# shape constraints (see GX_FIFO_TIER_A.md). Promotion-grade requires the
# wrapped loop-refinement obligation to itself be real-UNSAT + allowlisted.
MMIO_LOOP_EMISSION_MODEL_VERSION = "mmio-loop-emission-v1"
MMIO_LOOP_EMISSION_ALGORITHM = "mmio-loop-emission-v1"
MMIO_LOOP_EMISSION_SCHEMA_VERSION = 1

# Legacy always-incomplete v0 shape retained for parsing old certificates.
MMIO_LOOP_EMISSION_ALGORITHM_V0_LEGACY = "mmio-loop-emission-incomplete-v0"

MIXED_ADDRESS_SPACE_CAPABILITY = "mixed-address-space-routing"
DMA_INTERRUPT_CAPABILITY = "dma-interrupt-effects"
DMA_INTERRUPT_MODEL_VERSION = "dma-interrupt-v0"
DMA_INTERRUPT_ALGORITHM = "dma-interrupt-incomplete-v0"

# Algorithms known to capability_assurance (promotion-grade path only for
# register-bank / fifo-write / fifo-read / loop-emission when fully
# discharged + allowlisted).
MMIO_ATTESTATION_ALGORITHMS = frozenset(
    {
        MMIO_REGISTER_BANK_ALGORITHM,
        GX_FIFO_TRACE_ALGORITHM,
        GX_FIFO_LOOP_REFINEMENT_ALGORITHM,
        GX_FIFO_LOOP_REFINEMENT_ALGORITHM_V1_LEGACY,
        GX_FIFO_READ_ALGORITHM,
        GX_FIFO_READ_ALGORITHM_V0_LEGACY,
        MMIO_READ_SIDE_EFFECTS_ALGORITHM,
        MMIO_EXTERNAL_INPUT_ALGORITHM,
        MMIO_LOOP_EMISSION_ALGORITHM,
        MMIO_LOOP_EMISSION_ALGORITHM_V0_LEGACY,
        "mixed-address-space-incomplete-v0",
        DMA_INTERRUPT_ALGORITHM,
    }
)

# Never promotion-grade under Wave 3–5 foundations (Dolphin harness pending
# for read-side-effects / external-input / DMA). ``gx-fifo-read`` and
# ``mmio-loop-emission`` are promotable in Wave 5 once validators + corpus +
# allowlist are complete — see recompute_mmio_attestation_status for their
# (still fail-closed while unallowlisted) grading path.
ALWAYS_INCOMPLETE_MMIO_CAPABILITIES = frozenset(
    {
        MMIO_READ_SIDE_EFFECTS_CAPABILITY,
        MMIO_EXTERNAL_INPUT_CAPABILITY,
        DMA_INTERRUPT_CAPABILITY,
    }
)

_LOOP_REFINEMENT_KEYS = frozenset(
    {
        "schema_version",
        "capability",
        "model_version",
        "algorithm",
        "claim",
        "original",
        "candidate",
        "status",
        "notes",
    }
)
_LOOP_SIDE_KEYS = frozenset({"result", "query_sha256"})
_LOOP_SIDE_RESULTS = frozenset(
    {"unsat", "pending", "sat", "unknown", "incomplete", "vacuous"}
)
_ENTRY_GUARD_KEYS = frozenset({"algorithm", "result", "query_sha256"})

# gx-fifo-loop-exact-refinement-v2: per-side payload is the exact
# {"entry_guard", "refinement"} shape produced by gx_fifo_loop_discharge.py.
_LOOP_EXACT_REFINEMENT_KEYS = frozenset(
    {
        "schema_version",
        "capability",
        "model_version",
        "algorithm",
        "claim",
        "plan_witness_sha256",
        "plan_summary_sha256",
        "original",
        "candidate",
        "status",
        "notes",
    }
)

_GX_FIFO_READ_KEYS = frozenset(
    {
        "schema_version",
        "capability",
        "model_version",
        "algorithm",
        "hardware_profile",
        "hardware_profile_sha256",
        "source",
        "vacuous",
        "path_and_gx_read",
        "status",
        "notes",
    }
)

_MMIO_LOOP_EMISSION_KEYS = frozenset(
    {
        "schema_version",
        "capability",
        "model_version",
        "algorithm",
        "loop_refinement",
        "shape",
        "status",
        "notes",
    }
)

# mmio-loop-emission-v1 narrow domain shape fields (see GX_FIFO_TIER_A.md
# "Initial domain (deliberately narrow)" table). Not independently
# discharged here — descriptive metadata bound alongside the wrapped
# loop-refinement proof.
_MMIO_LOOP_EMISSION_SHAPE_KEYS = frozenset(
    {
        "device_id",
        "address",
        "width",
        "writes_per_iteration",
        "trip_count",
        "max_events",
        "value_kind",
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


def _mmio_sides_fully_unsat(obligation: Mapping[str, Any]) -> bool:
    """True when both original/candidate carry unsat digests for every side query."""
    for side_name in ("original", "candidate"):
        side = obligation.get(side_name)
        if not isinstance(side, Mapping):
            return False
        for key in _SIDE_QUERY_KEYS:
            block = side.get(key)
            if not isinstance(block, Mapping):
                return False
            if block.get("result") != "unsat":
                return False
            if not _is_sha256(block.get("query_sha256")):
                return False
            if block.get("query_sha256") == "0" * 64:
                return False
    return True


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

    if (
        original is None
        and candidate is None
        and isinstance(memory_bus_obligation, Mapping)
    ):
        obligation["original"] = discharged_side_from_memory_bus(
            memory_bus_obligation, side="original"
        )
        obligation["candidate"] = discharged_side_from_memory_bus(
            memory_bus_obligation, side="candidate"
        )
        if _mmio_sides_fully_unsat(obligation) and source == SOURCE_HARDWARE_PROFILE:
            # Structurally promotion-shaped; evaluate_capability_assurance still
            # gates the attestation on the allowlist via recompute.
            obligation["status"] = "promotion-grade"

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
            digest = (
                side_block.get("sha256")
                or side_block.get("query_sha256")
                or side_block.get("cfg_trace_sha256")
                or side_block.get("access_coverage_sha256")
            )
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
        # Prefer per-side digest when present.
        side_cov = coverage.get(side) if isinstance(coverage.get(side), dict) else None
        if isinstance(side_cov, dict) and _is_sha256(side_cov.get("sha256")):
            coverage_sha = str(side_cov["sha256"])
        else:
            digest = coverage.get("sha256")
            if isinstance(digest, str):
                coverage_sha = digest

    # Event-trace completeness: prefer an explicit complete gxfifo_trace, else
    # lift the device-state compare digest when the parent bus discharged and
    # FIFO/device state participated in the observable-state query.
    event_result = "pending"
    event_sha = None
    fifo = memory_bus_obligation.get("gxfifo_trace")
    if isinstance(fifo, dict) and fifo.get("status") == "complete":
        event_result = "unsat"
        digest = fifo.get("sha256") or fifo.get("query_sha256")
        if isinstance(digest, str):
            event_sha = digest
    elif (
        memory_bus_obligation.get("status") == "discharged"
        and isinstance(fifo, dict)
        and fifo.get("status") in {"cfg-routed", "complete", "scaffolded"}
    ):
        device_cmp = memory_bus_obligation.get("device_state_in_compare") or {}
        if (
            isinstance(device_cmp, dict)
            and device_cmp.get("included")
            and _is_sha256(device_cmp.get("digest_sha256"))
        ):
            event_result = "unsat"
            event_sha = str(device_cmp["digest_sha256"])

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
    if algorithm.endswith("-incomplete-v0"):
        return STATUS_INCOMPLETE

    # Legacy v1 digest-only loop refinement: never promotion-grade, no
    # matter the allowlist or digest content (see GX_FIFO_TIER_A.md).
    if algorithm == GX_FIFO_LOOP_REFINEMENT_ALGORITHM_V1_LEGACY:
        return STATUS_INCOMPLETE

    # Production v2 per-block loop-emission refinement for
    # gx-fifo-write-trace — incomplete unless every block on both sides
    # carries real UNSAT digests (sourced from the gx_fifo_loop.py discharge
    # module) AND the model is allowlisted.
    if algorithm == GX_FIFO_LOOP_REFINEMENT_ALGORITHM:
        if capability != GX_FIFO_WRITE_TRACE_CAPABILITY:
            return STATUS_INCOMPLETE
        if model_version not in tuple(allowed_versions):
            return STATUS_INCOMPLETE
        obligation = (
            evidence.get("loop_refinement")
            or evidence.get("obligation")
            or evidence.get("mmio")
        )
        if not isinstance(obligation, dict):
            return STATUS_INCOMPLETE
        if validate_gx_fifo_loop_exact_refinement_obligation(obligation) is not None:
            return STATUS_INCOMPLETE
        if not loop_exact_refinement_has_real_unsat(obligation):
            return STATUS_INCOMPLETE
        if assumptions:
            return STATUS_SCOPED_ASSUMPTION
        # Allowlist empty today → never reaches here for Tier A; structural
        # path exists for a future canary once real UNSAT digests land.
        return STATUS_PROMOTION_GRADE

    # Production gx-fifo-read-v1 unsupported-read policy attestation —
    # promotion-grade only via a reviewed profile + real path∧GX-read UNSAT,
    # or an explicit vacuous no-read-demand claim, and only when allowlisted.
    # Never a value model for FIFO loads.
    if algorithm == GX_FIFO_READ_ALGORITHM:
        if capability != GX_FIFO_READ_CAPABILITY:
            return STATUS_INCOMPLETE
        if model_version not in tuple(allowed_versions):
            return STATUS_INCOMPLETE
        obligation = (
            evidence.get("gx_fifo_read")
            or evidence.get("obligation")
            or evidence.get("mmio")
        )
        if not isinstance(obligation, dict):
            return STATUS_INCOMPLETE
        if not gx_fifo_read_is_promotion_grade(obligation):
            return STATUS_INCOMPLETE
        if assumptions:
            return STATUS_SCOPED_ASSUMPTION
        return STATUS_PROMOTION_GRADE

    # Production mmio-loop-emission-v1 authorization attachment — wraps a
    # discharged gx-fifo-loop-exact-refinement-v2 plan; promotion-grade only
    # when that wrapped plan is itself real-UNSAT and the model is
    # allowlisted.
    if algorithm == MMIO_LOOP_EMISSION_ALGORITHM:
        if capability != MMIO_LOOP_EMISSION_CAPABILITY:
            return STATUS_INCOMPLETE
        if model_version not in tuple(allowed_versions):
            return STATUS_INCOMPLETE
        obligation = (
            evidence.get("mmio_loop_emission")
            or evidence.get("obligation")
            or evidence.get("mmio")
        )
        if not isinstance(obligation, dict):
            return STATUS_INCOMPLETE
        if not mmio_loop_emission_is_promotion_grade(obligation):
            return STATUS_INCOMPLETE
        if assumptions:
            return STATUS_SCOPED_ASSUMPTION
        return STATUS_PROMOTION_GRADE

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


def _loop_side_block(
    *,
    result: str = "incomplete",
    query_sha256: str | None = None,
) -> dict[str, Any]:
    block: dict[str, Any] = {"result": result}
    if query_sha256 is not None:
        block["query_sha256"] = query_sha256
    elif result == "incomplete":
        block["query_sha256"] = "0" * 64
    return block


def build_gx_fifo_loop_refinement_obligation(
    *,
    original: Mapping[str, Any] | None = None,
    candidate: Mapping[str, Any] | None = None,
    status: str = "incomplete",
    notes: str | None = None,
) -> dict[str, Any]:
    """Legacy v1 structural stub (single top-level result=unsat per side).

    Kept only so old serialized certificates keep parsing / round-tripping.
    ``recompute_mmio_attestation_status`` rejects this shape for promotion
    unconditionally, no matter the allowlist or digest content — use
    ``build_gx_fifo_loop_exact_refinement_obligation`` for the production
    per-block v2 shape instead.
    """
    return {
        "schema_version": GX_FIFO_LOOP_REFINEMENT_SCHEMA_VERSION_V1_LEGACY,
        "capability": GX_FIFO_WRITE_TRACE_CAPABILITY,
        "model_version": GX_FIFO_TRACE_MODEL_VERSION,
        "algorithm": GX_FIFO_LOOP_REFINEMENT_ALGORITHM_V1_LEGACY,
        "claim": GX_FIFO_LOOP_REFINEMENT_CLAIM,
        "original": dict(original) if original is not None else _loop_side_block(),
        "candidate": (
            dict(candidate) if candidate is not None else _loop_side_block()
        ),
        "status": status,
        "notes": notes
        or (
            "Legacy v1 digest-only shape: never promotion-grade regardless "
            "of UNSAT digests — see gx-fifo-loop-exact-refinement-v2."
        ),
    }


def validate_gx_fifo_loop_refinement_obligation(
    obligation: Mapping[str, Any] | None,
) -> str | None:
    """Fail closed on malformed legacy v1 loop-refinement obligations."""
    if obligation is None:
        return "gx-fifo loop refinement obligation is missing"
    if not isinstance(obligation, Mapping):
        return "gx-fifo loop refinement obligation must be an object"
    unknown = sorted(set(obligation.keys()) - _LOOP_REFINEMENT_KEYS)
    if unknown:
        return f"gx-fifo loop refinement unknown fields: {', '.join(unknown)}"
    required = {
        "schema_version",
        "capability",
        "model_version",
        "algorithm",
        "claim",
        "original",
        "candidate",
    }
    missing = sorted(required - set(obligation.keys()))
    if missing:
        return f"gx-fifo loop refinement missing fields: {', '.join(missing)}"
    if obligation.get("schema_version") != GX_FIFO_LOOP_REFINEMENT_SCHEMA_VERSION_V1_LEGACY:
        return (
            "gx-fifo loop refinement schema_version must be "
            f"{GX_FIFO_LOOP_REFINEMENT_SCHEMA_VERSION_V1_LEGACY}"
        )
    if obligation.get("capability") != GX_FIFO_WRITE_TRACE_CAPABILITY:
        return (
            "gx-fifo loop refinement.capability must be "
            f"{GX_FIFO_WRITE_TRACE_CAPABILITY!r}"
        )
    if obligation.get("model_version") != GX_FIFO_TRACE_MODEL_VERSION:
        return (
            "gx-fifo loop refinement.model_version must be "
            f"{GX_FIFO_TRACE_MODEL_VERSION!r}"
        )
    if obligation.get("algorithm") != GX_FIFO_LOOP_REFINEMENT_ALGORITHM_V1_LEGACY:
        return (
            "gx-fifo loop refinement.algorithm must be "
            f"{GX_FIFO_LOOP_REFINEMENT_ALGORITHM_V1_LEGACY!r}"
        )
    if obligation.get("claim") != GX_FIFO_LOOP_REFINEMENT_CLAIM:
        return (
            "gx-fifo loop refinement.claim must be "
            f"{GX_FIFO_LOOP_REFINEMENT_CLAIM!r}"
        )
    for side_name in ("original", "candidate"):
        side = obligation.get(side_name)
        if not isinstance(side, Mapping):
            return f"gx-fifo loop refinement.{side_name} must be an object"
        unknown_side = sorted(set(side.keys()) - _LOOP_SIDE_KEYS)
        if unknown_side:
            return (
                f"gx-fifo loop refinement.{side_name} unknown fields: "
                f"{', '.join(unknown_side)}"
            )
        if "result" not in side:
            return f"gx-fifo loop refinement.{side_name} missing result"
        result = side.get("result")
        if result not in _LOOP_SIDE_RESULTS:
            return (
                f"gx-fifo loop refinement.{side_name}.result "
                f"unsupported ({result!r})"
            )
        if "query_sha256" in side and not _is_sha256(side.get("query_sha256")):
            return (
                f"gx-fifo loop refinement.{side_name}.query_sha256 "
                "must be a 64-hex digest"
            )
        if result == "unsat" and not _is_sha256(side.get("query_sha256")):
            return (
                f"gx-fifo loop refinement.{side_name}.query_sha256 "
                "required for unsat"
            )
    return None


def loop_refinement_has_real_unsat(obligation: Mapping[str, Any]) -> bool:
    """True when both sides carry unsat with non-placeholder digests.

    Structural check only — this legacy v1 shape is never promotion-grade
    even when this returns True (see ``recompute_mmio_attestation_status``).
    """
    for side_name in ("original", "candidate"):
        side = obligation.get(side_name)
        if not isinstance(side, Mapping):
            return False
        if side.get("result") != "unsat":
            return False
        digest = side.get("query_sha256")
        if not _is_sha256(digest):
            return False
        # Placeholder all-zero digests are scaffolding, not real UNSAT.
        if digest == "0" * 64:
            return False
    return True


def _empty_discharge_side() -> dict[str, Any]:
    """Structurally valid but undischarged ``{entry_guard, refinement}`` side."""
    return {
        "entry_guard": _loop_side_block(),
        "refinement": {"algorithm": GX_FIFO_LOOP_REFINEMENT_ALGORITHM, "status": "applied"},
    }


def build_gx_fifo_loop_exact_refinement_obligation(
    *,
    original: Mapping[str, Any] | None = None,
    candidate: Mapping[str, Any] | None = None,
    plan_witness_sha256: str | None = None,
    plan_summary_sha256: str | None = None,
    status: str = "incomplete",
    notes: str | None = None,
) -> dict[str, Any]:
    """Production v2 stub wrapping real ``gx_fifo_loop_discharge.py`` output.

    Each side is exactly the ``{"entry_guard": {...}, "refinement": {...}}``
    payload returned by ``gx_fifo_loop_discharge.refinement_for_plan`` /
    ``discharge_gx_fifo_loop_plan`` — never a bare caller-supplied
    ``result=unsat``. ``refinement`` itself carries the six per-block
    discharges (body step, event identity, postcondition, termination,
    bound, footprint — see GX_FIFO_TIER_A.md "Authorization") and is
    structurally validated by that module's own
    ``validate_gx_fifo_loop_refinement_v2``, which independently rejects
    unknown/legacy block algorithms and placeholder digests.
    ``plan_witness_sha256`` / ``plan_summary_sha256`` optionally bind the
    obligation to a specific ``gx_fifo_loop.py`` ``GxFifoLoopPlan``
    (drift-proof pairing); when bound, callers should recompute and compare
    them against the live plan. Incomplete unless every block on both sides
    is real (non-placeholder) unsat.
    """
    obligation: dict[str, Any] = {
        "schema_version": GX_FIFO_LOOP_REFINEMENT_SCHEMA_VERSION,
        "capability": GX_FIFO_WRITE_TRACE_CAPABILITY,
        "model_version": GX_FIFO_TRACE_MODEL_VERSION,
        "algorithm": GX_FIFO_LOOP_REFINEMENT_ALGORITHM,
        "claim": GX_FIFO_LOOP_REFINEMENT_CLAIM,
        "original": dict(original) if original is not None else _empty_discharge_side(),
        "candidate": dict(candidate) if candidate is not None else _empty_discharge_side(),
        "status": status,
        "notes": notes
        or (
            "Production v2: ordinary N FIFO writes \u2261 summarized N-event "
            "trace requires the real per-block UNSAT structure produced by "
            "gx_fifo_loop_discharge.py on both sides; incomplete without it."
        ),
    }
    if plan_witness_sha256 is not None:
        obligation["plan_witness_sha256"] = plan_witness_sha256
    if plan_summary_sha256 is not None:
        obligation["plan_summary_sha256"] = plan_summary_sha256
    return obligation


def _validate_entry_guard_block(entry_guard: Any, *, label: str) -> str | None:
    if not isinstance(entry_guard, Mapping):
        return f"{label} must be an object"
    unknown = sorted(set(entry_guard.keys()) - _ENTRY_GUARD_KEYS)
    if unknown:
        return f"{label} unknown fields: {', '.join(unknown)}"
    if "result" not in entry_guard:
        return f"{label} missing result"
    result = entry_guard.get("result")
    if result not in _LOOP_SIDE_RESULTS:
        return f"{label}.result unsupported ({result!r})"
    algorithm = entry_guard.get("algorithm")
    if algorithm is not None and (
        not isinstance(algorithm, str) or algorithm.endswith("-v1")
    ):
        return f"{label}.algorithm {algorithm!r} must not be a legacy v1 algorithm"
    if "query_sha256" in entry_guard and not _is_sha256(entry_guard.get("query_sha256")):
        return f"{label}.query_sha256 must be a 64-hex digest"
    if result == "unsat" and not _is_sha256(entry_guard.get("query_sha256")):
        return f"{label}.query_sha256 required for unsat"
    return None


def validate_gx_fifo_loop_exact_refinement_obligation(
    obligation: Mapping[str, Any] | None,
) -> str | None:
    """Fail closed on malformed production v2 loop-refinement obligations.

    Delegates each side's ``refinement`` sub-object to
    ``gx_fifo_loop_discharge.validate_gx_fifo_loop_refinement_v2`` so the
    per-block algorithm/digest rules live in exactly one place (the module
    that actually runs the discharge).
    """
    from tools.ppc_equivalence.gx_fifo_loop_discharge import (
        validate_gx_fifo_loop_refinement_v2,
    )

    if obligation is None:
        return "gx-fifo loop exact refinement obligation is missing"
    if not isinstance(obligation, Mapping):
        return "gx-fifo loop exact refinement obligation must be an object"
    unknown = sorted(set(obligation.keys()) - _LOOP_EXACT_REFINEMENT_KEYS)
    if unknown:
        return (
            "gx-fifo loop exact refinement unknown fields: "
            f"{', '.join(unknown)}"
        )
    required = {
        "schema_version",
        "capability",
        "model_version",
        "algorithm",
        "claim",
        "original",
        "candidate",
    }
    missing = sorted(required - set(obligation.keys()))
    if missing:
        return (
            "gx-fifo loop exact refinement missing fields: "
            f"{', '.join(missing)}"
        )
    if obligation.get("schema_version") != GX_FIFO_LOOP_REFINEMENT_SCHEMA_VERSION:
        return (
            "gx-fifo loop exact refinement schema_version must be "
            f"{GX_FIFO_LOOP_REFINEMENT_SCHEMA_VERSION}"
        )
    if obligation.get("capability") != GX_FIFO_WRITE_TRACE_CAPABILITY:
        return (
            "gx-fifo loop exact refinement.capability must be "
            f"{GX_FIFO_WRITE_TRACE_CAPABILITY!r}"
        )
    if obligation.get("model_version") != GX_FIFO_TRACE_MODEL_VERSION:
        return (
            "gx-fifo loop exact refinement.model_version must be "
            f"{GX_FIFO_TRACE_MODEL_VERSION!r}"
        )
    if obligation.get("algorithm") != GX_FIFO_LOOP_REFINEMENT_ALGORITHM:
        return (
            "gx-fifo loop exact refinement.algorithm must be "
            f"{GX_FIFO_LOOP_REFINEMENT_ALGORITHM!r} (v1 legacy is never "
            "valid here)"
        )
    if obligation.get("claim") != GX_FIFO_LOOP_REFINEMENT_CLAIM:
        return (
            "gx-fifo loop exact refinement.claim must be "
            f"{GX_FIFO_LOOP_REFINEMENT_CLAIM!r}"
        )
    for key in ("plan_witness_sha256", "plan_summary_sha256"):
        if key in obligation and not _is_sha256(obligation.get(key)):
            return f"gx-fifo loop exact refinement.{key} must be a 64-hex digest"
    for side_name in ("original", "candidate"):
        side = obligation.get(side_name)
        if not isinstance(side, Mapping):
            return f"gx-fifo loop exact refinement.{side_name} must be an object"
        unknown_side = sorted(set(side.keys()) - GX_FIFO_LOOP_SIDE_KEYS)
        if unknown_side:
            return (
                f"gx-fifo loop exact refinement.{side_name} unknown fields: "
                f"{', '.join(unknown_side)}"
            )
        missing_side = sorted(GX_FIFO_LOOP_SIDE_KEYS - set(side.keys()))
        if missing_side:
            return (
                f"gx-fifo loop exact refinement.{side_name} missing fields: "
                f"{', '.join(missing_side)}"
            )
        entry_guard_error = _validate_entry_guard_block(
            side.get("entry_guard"), label=f"gx-fifo loop exact refinement.{side_name}.entry_guard",
        )
        if entry_guard_error is not None:
            return entry_guard_error
        refinement = side.get("refinement")
        if not isinstance(refinement, Mapping):
            return f"gx-fifo loop exact refinement.{side_name}.refinement must be an object"
        reason = validate_gx_fifo_loop_refinement_v2(dict(refinement))
        if reason is not None:
            return f"gx-fifo loop exact refinement.{side_name}.refinement invalid: {reason}"
    return None


def loop_exact_refinement_has_real_unsat(obligation: Mapping[str, Any]) -> bool:
    """True when both sides are fully discharged with real (non-placeholder) digests."""
    from tools.ppc_equivalence.gx_fifo_loop_discharge import (
        validate_gx_fifo_loop_refinement_v2,
    )

    for side_name in ("original", "candidate"):
        side = obligation.get(side_name)
        if not isinstance(side, Mapping):
            return False
        entry_guard = side.get("entry_guard")
        if not isinstance(entry_guard, Mapping):
            return False
        if entry_guard.get("result") != "unsat":
            return False
        digest = entry_guard.get("query_sha256")
        if not _is_sha256(digest) or digest == "0" * 64:
            return False
        refinement = side.get("refinement")
        if not isinstance(refinement, Mapping):
            return False
        if refinement.get("status") != "discharged":
            return False
        if validate_gx_fifo_loop_refinement_v2(dict(refinement)) is not None:
            return False
    return True


def build_always_incomplete_mmio_attestation(
    capability: str,
    *,
    assumptions: Sequence[str] = (),
    unsupported: Sequence[str] = (),
    extra_evidence: Mapping[str, Any] | None = None,
) -> Any:
    """Emit an always-incomplete MMIO advanced attestation stub.

    Only for capabilities in ``ALWAYS_INCOMPLETE_MMIO_CAPABILITIES``
    (``mmio-read-side-effects``, ``mmio-external-input``,
    ``dma-interrupt-effects``) — ``gx-fifo-read`` and ``mmio-loop-emission``
    are promotable in Wave 5 and use their own dedicated builders instead.
    """
    from tools.ppc_equivalence.capability_assurance import (
        STATUS_INCOMPLETE,
        build_attestation,
    )

    meta = {
        MMIO_READ_SIDE_EFFECTS_CAPABILITY: (
            MMIO_READ_SIDE_EFFECTS_MODEL_VERSION,
            MMIO_READ_SIDE_EFFECTS_ALGORITHM,
            ("dolphin-harness-pending", "read-side-effects-incomplete"),
        ),
        MMIO_EXTERNAL_INPUT_CAPABILITY: (
            MMIO_EXTERNAL_INPUT_MODEL_VERSION,
            MMIO_EXTERNAL_INPUT_ALGORITHM,
            ("dolphin-harness-pending", "external-input-incomplete"),
        ),
        DMA_INTERRUPT_CAPABILITY: (
            DMA_INTERRUPT_MODEL_VERSION,
            DMA_INTERRUPT_ALGORITHM,
            ("dolphin-harness-pending", "dma-interrupt-incomplete"),
        ),
    }
    if capability not in meta:
        raise ValueError(f"no always-incomplete stub for {capability!r}")
    model_version, algorithm, default_unsupported = meta[capability]
    evidence: dict[str, Any] = {
        "never_promotion_grade": True,
        "dolphin_harness": False,
        "capability": capability,
    }
    if extra_evidence:
        evidence.update(dict(extra_evidence))
    return build_attestation(
        capability=capability,
        model_version=model_version,
        algorithm=algorithm,
        status=STATUS_INCOMPLETE,
        assumptions=tuple(assumptions),
        unsupported=tuple(unsupported) if unsupported else default_unsupported,
        evidence=evidence,
    )


def build_gx_fifo_loop_refinement_attestation(
    obligation: Mapping[str, Any] | None = None,
    *,
    assumptions: Sequence[str] = (),
    unsupported: Sequence[str] = (),
) -> Any:
    """Draft gx-fifo-write-trace attestation wrapping the legacy v1 stub.

    Never promotion-grade — prefer
    ``build_gx_fifo_loop_exact_refinement_attestation`` for new drafts.
    """
    from tools.ppc_equivalence.capability_assurance import (
        STATUS_INCOMPLETE,
        build_attestation,
    )

    obl = (
        dict(obligation)
        if obligation is not None
        else build_gx_fifo_loop_refinement_obligation()
    )
    error = validate_gx_fifo_loop_refinement_obligation(obl)
    evidence: dict[str, Any] = {
        "loop_refinement": obl,
        "obligation_sha256": canonical_json_sha256(obl),
        "never_promotion_grade_without_unsat": True,
    }
    if error is not None:
        evidence["schema_error"] = error
    return build_attestation(
        capability=GX_FIFO_WRITE_TRACE_CAPABILITY,
        model_version=GX_FIFO_TRACE_MODEL_VERSION,
        algorithm=GX_FIFO_LOOP_REFINEMENT_ALGORITHM_V1_LEGACY,
        status=STATUS_INCOMPLETE,
        assumptions=tuple(assumptions),
        unsupported=tuple(unsupported)
        if unsupported
        else (
            ("schema-invalid",)
            if error
            else ("gx-fifo-loop-refinement-incomplete",)
        ),
        evidence=evidence,
    )


def build_gx_fifo_loop_exact_refinement_attestation(
    obligation: Mapping[str, Any] | None = None,
    *,
    assumptions: Sequence[str] = (),
    unsupported: Sequence[str] = (),
) -> Any:
    """Draft gx-fifo-write-trace attestation wrapping a v2 per-block stub."""
    from tools.ppc_equivalence.capability_assurance import (
        STATUS_INCOMPLETE,
        build_attestation,
    )

    obl = (
        dict(obligation)
        if obligation is not None
        else build_gx_fifo_loop_exact_refinement_obligation()
    )
    error = validate_gx_fifo_loop_exact_refinement_obligation(obl)
    evidence: dict[str, Any] = {
        "loop_refinement": obl,
        "obligation_sha256": canonical_json_sha256(obl),
        "never_promotion_grade_without_per_block_unsat": True,
    }
    if error is not None:
        evidence["schema_error"] = error
    return build_attestation(
        capability=GX_FIFO_WRITE_TRACE_CAPABILITY,
        model_version=GX_FIFO_TRACE_MODEL_VERSION,
        algorithm=GX_FIFO_LOOP_REFINEMENT_ALGORITHM,
        status=STATUS_INCOMPLETE,
        assumptions=tuple(assumptions),
        unsupported=tuple(unsupported)
        if unsupported
        else (
            ("schema-invalid",)
            if error
            else ("gx-fifo-loop-exact-refinement-incomplete",)
        ),
        evidence=evidence,
    )


def build_gx_fifo_read_obligation(
    *,
    hardware_profile: Mapping[str, Any] | str | None = None,
    path_and_gx_read: Mapping[str, Any] | None = None,
    vacuous: bool = False,
    ad_hoc: bool = False,
    status: str = "incomplete",
    notes: str | None = None,
) -> dict[str, Any]:
    """Build a ``gx-fifo-read-v1`` unsupported-read *policy* obligation.

    This is never a FIFO-load value model (see GX_FIFO_TIER_A.md outcome 3).
    Two promotion routes exist:

    1. A reviewed hardware profile plus a real ``path ∧ GX-read`` UNSAT
       digest in ``path_and_gx_read`` (every path that would load from GX is
       proved unreachable / UNSAT).
    2. ``vacuous=True`` — the target demands no reachable GX read at all, so
       the unsupported-read policy is vacuously complete under policy.
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

    obligation: dict[str, Any] = {
        "schema_version": GX_FIFO_READ_SCHEMA_VERSION,
        "capability": GX_FIFO_READ_CAPABILITY,
        "model_version": GX_FIFO_READ_MODEL_VERSION,
        "algorithm": GX_FIFO_READ_ALGORITHM,
        "source": source,
        "vacuous": bool(vacuous),
        "path_and_gx_read": (
            dict(path_and_gx_read)
            if path_and_gx_read is not None
            else _side_query_block()
        ),
        "status": status,
        "notes": notes
        or (
            "Unsupported-read policy attestation, not a value model: "
            "promotion-grade requires either a reviewed profile with a real "
            "path\u2227GX-read UNSAT digest, or vacuous=True when no reachable "
            "GX read demand exists."
        ),
    }
    if profile_obj is not None:
        digest = profile_obj.get("profile_sha256")
        if not _is_sha256(digest):
            digest = compute_hardware_profile_sha256(profile_obj)
        obligation["hardware_profile"] = str(
            profile_obj.get("profile") or profile_obj.get("platform_profile")
        )
        obligation["hardware_profile_sha256"] = digest
    else:
        obligation["hardware_profile"] = None
        obligation["hardware_profile_sha256"] = None
    return obligation


def validate_gx_fifo_read_obligation(
    obligation: Mapping[str, Any] | None,
    *,
    require_promotion_shape: bool = False,
) -> str | None:
    """Fail closed on malformed ``gx-fifo-read-v1`` policy obligations."""
    if obligation is None:
        return "gx-fifo-read obligation is missing"
    if not isinstance(obligation, Mapping):
        return "gx-fifo-read obligation must be an object"
    unknown = sorted(set(obligation.keys()) - _GX_FIFO_READ_KEYS)
    if unknown:
        return f"gx-fifo-read unknown fields: {', '.join(unknown)}"
    if obligation.get("schema_version") != GX_FIFO_READ_SCHEMA_VERSION:
        return f"gx-fifo-read schema_version must be {GX_FIFO_READ_SCHEMA_VERSION}"
    if obligation.get("capability") != GX_FIFO_READ_CAPABILITY:
        return f"gx-fifo-read.capability must be {GX_FIFO_READ_CAPABILITY!r}"
    if obligation.get("model_version") != GX_FIFO_READ_MODEL_VERSION:
        return f"gx-fifo-read.model_version must be {GX_FIFO_READ_MODEL_VERSION!r}"
    if obligation.get("algorithm") != GX_FIFO_READ_ALGORITHM:
        return f"gx-fifo-read.algorithm must be {GX_FIFO_READ_ALGORITHM!r}"
    source = obligation.get("source")
    if source not in {SOURCE_HARDWARE_PROFILE, SOURCE_AD_HOC_BUS}:
        return f"gx-fifo-read.source unsupported ({source!r})"
    vacuous = obligation.get("vacuous")
    if not isinstance(vacuous, bool):
        return "gx-fifo-read.vacuous must be a bool"
    path_and_gx_read = obligation.get("path_and_gx_read")
    if not isinstance(path_and_gx_read, Mapping):
        return "gx-fifo-read.path_and_gx_read must be an object"
    result = path_and_gx_read.get("result")
    if result not in {"unsat", "pending", "sat", "unknown", "vacuous"}:
        return f"gx-fifo-read.path_and_gx_read.result unsupported ({result!r})"
    if result == "unsat" and not _is_sha256(path_and_gx_read.get("query_sha256")):
        return "gx-fifo-read.path_and_gx_read.query_sha256 required for unsat"
    if "query_sha256" in path_and_gx_read and not _is_sha256(
        path_and_gx_read.get("query_sha256")
    ):
        return "gx-fifo-read.path_and_gx_read.query_sha256 must be a 64-hex digest"

    if require_promotion_shape or obligation.get("status") == "promotion-grade":
        if not vacuous:
            if source != SOURCE_HARDWARE_PROFILE:
                return "gx-fifo-read promotion-grade requires source=hardware-profile unless vacuous"
            profile_name = obligation.get("hardware_profile")
            profile_digest = obligation.get("hardware_profile_sha256")
            if (
                not isinstance(profile_name, str)
                or profile_name not in REVIEWED_HARDWARE_PROFILES
            ):
                return "gx-fifo-read promotion-grade requires a reviewed hardware_profile"
            if not _is_sha256(profile_digest):
                return "gx-fifo-read.hardware_profile_sha256 must be a 64-hex digest"
            try:
                reviewed = load_hardware_profile(profile_name)
            except (OSError, ValueError) as exc:
                return f"gx-fifo-read hardware profile load failed: {exc}"
            if reviewed.get("profile_sha256") != profile_digest:
                return (
                    "gx-fifo-read.hardware_profile_sha256 does not match "
                    f"reviewed hardware profile {profile_name}"
                )
            if result != "unsat":
                return "gx-fifo-read promotion-grade requires path_and_gx_read.result=unsat when not vacuous"
    return None


def gx_fifo_read_is_promotion_grade(obligation: Mapping[str, Any]) -> bool:
    """True when the obligation validates and is honestly promotion-grade."""
    if obligation.get("status") != "promotion-grade":
        return False
    if validate_gx_fifo_read_obligation(obligation, require_promotion_shape=True) is not None:
        return False
    if obligation.get("vacuous") is True:
        return True
    path_and_gx_read = obligation.get("path_and_gx_read")
    if not isinstance(path_and_gx_read, Mapping):
        return False
    if path_and_gx_read.get("result") != "unsat":
        return False
    digest = path_and_gx_read.get("query_sha256")
    return _is_sha256(digest) and digest != "0" * 64


def build_gx_fifo_read_attestation(
    obligation: Mapping[str, Any] | None = None,
    *,
    assumptions: Sequence[str] = (),
    unsupported: Sequence[str] = (),
) -> Any:
    """Draft gx-fifo-read attestation from a ``gx-fifo-read-v1`` obligation."""
    from tools.ppc_equivalence.capability_assurance import (
        STATUS_INCOMPLETE,
        build_attestation,
    )

    obl = dict(obligation) if obligation is not None else build_gx_fifo_read_obligation()
    error = validate_gx_fifo_read_obligation(obl)
    evidence: dict[str, Any] = {
        "gx_fifo_read": obl,
        "obligation_sha256": canonical_json_sha256(obl),
    }
    if error is not None:
        evidence["schema_error"] = error
    return build_attestation(
        capability=GX_FIFO_READ_CAPABILITY,
        model_version=GX_FIFO_READ_MODEL_VERSION,
        algorithm=GX_FIFO_READ_ALGORITHM,
        status=STATUS_INCOMPLETE,
        assumptions=tuple(assumptions),
        unsupported=tuple(unsupported)
        if unsupported
        else (("schema-invalid",) if error else ("gx-fifo-read-incomplete",)),
        evidence=evidence,
    )


def build_mmio_loop_emission_obligation(
    *,
    loop_refinement: Mapping[str, Any] | None = None,
    shape: Mapping[str, Any] | None = None,
    status: str = "incomplete",
    notes: str | None = None,
) -> dict[str, Any]:
    """Build an ``mmio-loop-emission-v1`` authorization attachment.

    Wraps a ``gx-fifo-loop-exact-refinement-v2`` obligation (the discharged
    plan) plus descriptive ``shape`` metadata bound to the narrow
    mmio-loop-emission-v1 domain (device / width / trip count / bound — see
    GX_FIFO_TIER_A.md "Initial domain"). Recognition never authorizes
    equivalence by itself: promotion-grade requires the wrapped
    loop-refinement obligation to be real-UNSAT on every block.
    """
    unknown_shape = sorted(set((shape or {}).keys()) - _MMIO_LOOP_EMISSION_SHAPE_KEYS)
    if unknown_shape:
        raise ValueError(f"unknown mmio-loop-emission shape fields: {unknown_shape}")
    return {
        "schema_version": MMIO_LOOP_EMISSION_SCHEMA_VERSION,
        "capability": MMIO_LOOP_EMISSION_CAPABILITY,
        "model_version": MMIO_LOOP_EMISSION_MODEL_VERSION,
        "algorithm": MMIO_LOOP_EMISSION_ALGORITHM,
        "loop_refinement": (
            dict(loop_refinement)
            if loop_refinement is not None
            else build_gx_fifo_loop_exact_refinement_obligation()
        ),
        "shape": dict(shape) if shape is not None else {},
        "status": status,
        "notes": notes
        or (
            "Authorization attachment wrapping a discharged "
            "gx-fifo-loop-exact-refinement-v2 plan; incomplete unless every "
            "block on both sides is real-UNSAT."
        ),
    }


def validate_mmio_loop_emission_obligation(
    obligation: Mapping[str, Any] | None,
) -> str | None:
    """Fail closed on malformed ``mmio-loop-emission-v1`` obligations."""
    if obligation is None:
        return "mmio-loop-emission obligation is missing"
    if not isinstance(obligation, Mapping):
        return "mmio-loop-emission obligation must be an object"
    unknown = sorted(set(obligation.keys()) - _MMIO_LOOP_EMISSION_KEYS)
    if unknown:
        return f"mmio-loop-emission unknown fields: {', '.join(unknown)}"
    if obligation.get("schema_version") != MMIO_LOOP_EMISSION_SCHEMA_VERSION:
        return f"mmio-loop-emission schema_version must be {MMIO_LOOP_EMISSION_SCHEMA_VERSION}"
    if obligation.get("capability") != MMIO_LOOP_EMISSION_CAPABILITY:
        return f"mmio-loop-emission.capability must be {MMIO_LOOP_EMISSION_CAPABILITY!r}"
    if obligation.get("model_version") != MMIO_LOOP_EMISSION_MODEL_VERSION:
        return f"mmio-loop-emission.model_version must be {MMIO_LOOP_EMISSION_MODEL_VERSION!r}"
    if obligation.get("algorithm") != MMIO_LOOP_EMISSION_ALGORITHM:
        return f"mmio-loop-emission.algorithm must be {MMIO_LOOP_EMISSION_ALGORITHM!r}"
    shape = obligation.get("shape")
    if not isinstance(shape, Mapping):
        return "mmio-loop-emission.shape must be an object"
    unknown_shape = sorted(set(shape.keys()) - _MMIO_LOOP_EMISSION_SHAPE_KEYS)
    if unknown_shape:
        return f"mmio-loop-emission.shape unknown fields: {', '.join(unknown_shape)}"
    loop_refinement = obligation.get("loop_refinement")
    reason = validate_gx_fifo_loop_exact_refinement_obligation(loop_refinement)
    if reason is not None:
        return f"mmio-loop-emission.loop_refinement invalid: {reason}"
    return None


def mmio_loop_emission_is_promotion_grade(obligation: Mapping[str, Any]) -> bool:
    """True when the wrapped v2 loop-refinement plan is real-UNSAT."""
    if validate_mmio_loop_emission_obligation(obligation) is not None:
        return False
    loop_refinement = obligation.get("loop_refinement")
    if not isinstance(loop_refinement, Mapping):
        return False
    return loop_exact_refinement_has_real_unsat(loop_refinement)


def build_mmio_loop_emission_attestation(
    obligation: Mapping[str, Any] | None = None,
    *,
    assumptions: Sequence[str] = (),
    unsupported: Sequence[str] = (),
) -> Any:
    """Draft mmio-loop-emission attestation from an authorization attachment."""
    from tools.ppc_equivalence.capability_assurance import (
        STATUS_INCOMPLETE,
        build_attestation,
    )

    obl = (
        dict(obligation)
        if obligation is not None
        else build_mmio_loop_emission_obligation()
    )
    error = validate_mmio_loop_emission_obligation(obl)
    evidence: dict[str, Any] = {
        "mmio_loop_emission": obl,
        "obligation_sha256": canonical_json_sha256(obl),
    }
    if error is not None:
        evidence["schema_error"] = error
    return build_attestation(
        capability=MMIO_LOOP_EMISSION_CAPABILITY,
        model_version=MMIO_LOOP_EMISSION_MODEL_VERSION,
        algorithm=MMIO_LOOP_EMISSION_ALGORITHM,
        status=STATUS_INCOMPLETE,
        assumptions=tuple(assumptions),
        unsupported=tuple(unsupported)
        if unsupported
        else (("schema-invalid",) if error else ("mmio-loop-emission-incomplete",)),
        evidence=evidence,
    )


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
        # Only demand register-bank when a bank was actually accessed. A
        # reviewed profile may list unused stubs (e.g. pi-stub alongside
        # gx-fifo); demanding an unallowlisted bank would block Tier A for
        # pure FIFO canaries.
        reg = memory_bus.get("register_bank_extensional")
        banks_touched = False
        if isinstance(reg, dict):
            if reg.get("touched"):
                banks_touched = True
            obs = reg.get("observability") or memory_bus.get("observability") or {}
            sym = obs.get("symbolic") if isinstance(obs, Mapping) else None
            if isinstance(sym, Mapping):
                for side_name in ("original", "candidate"):
                    banks = (sym.get(side_name) or {}).get("register_banks") or {}
                    if banks:
                        banks_touched = True
                        break
        if banks_touched:
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

    # mmio-loop-emission is demanded only when a GX FIFO loop was summarized
    # or an unrecognized fifo-touching loop was rejected — not merely because
    # the bus advertises the hard-reject *policy* for unused summaries.
    loop_emission = memory_bus.get("loop_fifo_emission") or (
        coverage.get("loop_fifo_emission") if isinstance(coverage, dict) else None
    )
    gx_plans = memory_bus.get("gx_loop_plans_used") or memory_bus.get(
        "gx_fifo_plans_used"
    )
    has_gx_plans = bool(gx_plans)
    if loop_emission == "summarized-bounded" or has_gx_plans:
        used.add(MMIO_LOOP_EMISSION_CAPABILITY)
    if any(
        "symbolic-loop-fifo-emission" in item or "loop-fifo" in item
        for item in rejects
    ):
        used.add(MMIO_LOOP_EMISSION_CAPABILITY)
    markers = memory_bus.get("loop_fifo_reject_markers") or []
    if markers:
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
