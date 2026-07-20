"""SMT obligations and proof metadata for opt-in ``MemoryBus`` routing.

MMIO register-bank / FIFO formulas live in ``symbolic_bus`` /
``symbolic_event_trace``. Reachable unsupported accesses are separate proof
queries (``path ∧ ¬supported``); ``supported`` is never assumed into the
equivalence query. CFG routing is live for SymbolicOps. ``memory-bus`` may
authorize ``EQUIVALENT`` only when the engine attaches ``status=discharged``
with schema v2 attestations (Track D enablement gate).

Discharged obligations (``status=discharged``) require schema v2 attestations:
``bus_spec_sha256``, per-side unsupported-access UNSAT or vacuous digests,
theory blocks (or explicit none), device-state-in-compare attestation, and
access-coverage attestation. Caller-forged discharged claims without digests
fail closed.
"""

from __future__ import annotations

from dataclasses import dataclass
from typing import Any, Sequence

from tools.ppc_equivalence.address_space import RegionKind
from tools.ppc_equivalence.jump_table_obligations import (
    rom_image_byte_constraints,
    rom_image_no_write_constraints,
)
from tools.ppc_equivalence.memory_bus import MemoryBus
from tools.ppc_equivalence.memory_profile import access_within_any_range
from tools.ppc_equivalence.provenance import canonical_json_sha256

# Schema v2 adds discharged attestations (vacuous coverage, theory blocks,
# device-state-in-compare). Architecture model is unchanged (Track D).
MEMORY_BUS_OBLIGATION_SCHEMA_VERSION = 2
MEMORY_BUS_ALGORITHM = "memory-bus-v1"

# Track C coverage / rejection policy (additive; discharged shape owned by Track A).
LOOP_FIFO_POLICY = "hard-reject"
LOOP_FIFO_EMISSION = "unsupported"
LOOP_FIFO_REJECTION_REASON = "symbolic-loop-fifo-emission"
MIXED_SPACE_POLICY = "fail-closed"
MIXED_SPACE_REJECTION_REASON = "symbolic-mmio-mixed-address-space"

_SHA256_LEN = 64


def _bus_has_fifo_devices(memory_bus: MemoryBus) -> bool:
    spec = memory_bus.specification
    if spec is None:
        return False
    return any(device.theory == "gxfifo-stream" for device in spec.devices)


def _bus_has_mmio_regions(memory_bus: MemoryBus) -> bool:
    return any(
        region.kind is RegionKind.MMIO for region in memory_bus.address_space.regions
    )


def _bus_has_non_mmio_regions(memory_bus: MemoryBus) -> bool:
    return any(
        region.kind is not RegionKind.MMIO
        for region in memory_bus.address_space.regions
    )


def memory_bus_coverage_policies(memory_bus: MemoryBus) -> dict[str, Any]:
    """Static coverage attestation for loop×FIFO and mixed-space policies.

    Bounded summarized FIFO emission is intentionally unsupported: the CFG
    hard-rejects when loop summaries coexist with FIFO devices. Pure-MMIO
    symbolic ``addr == register`` routing remains allowed; mixed RAM/ROM+MMIO
    symbolic addresses stay fail-closed.
    """
    coverage: dict[str, Any] = {
        "mixed_space_symbolic_mmio": MIXED_SPACE_POLICY,
        "pure_mmio_symbolic": "supported",
        "bounded_summarized_fifo_emission": LOOP_FIFO_EMISSION,
        "note": (
            "Pure-MMIO symbolic routing exists; mixed RAM/ROM/MMIO symbolic "
            "addresses remain fail-closed. Loop-summary × FIFO has a hard "
            "rejection; bounded summarized emission is not supported."
        ),
    }
    if _bus_has_fifo_devices(memory_bus):
        coverage["loop_fifo_policy"] = LOOP_FIFO_POLICY
        coverage["loop_fifo_emission"] = LOOP_FIFO_EMISSION
    return coverage


@dataclass(frozen=True, slots=True)
class RomImageBinding:
    """Minimal ROM image handle for ``rom_image_*`` constraint helpers."""

    base: int
    image_bytes: bytes
    source: str = "memory-bus"
    label: str | None = None

    @property
    def byte_count(self) -> int:
        return len(self.image_bytes)


def build_memory_bus_constraints(
    memory_bus: MemoryBus,
    initial_memory: Any,
    terminals: Sequence[Any],
    ops: Any,
) -> list[Any]:
    """Bind symbolic memory to the bus address space (Tier C, fail-closed).

    MMIO touches that hit declared devices are excluded from the RAM/ROM range
    check — device routing / unsupported-access digests own those addresses.
    """
    z3 = ops.z3
    constraints: list[Any] = []
    allowed_ranges: list[tuple[int, int]] = []
    mmio_ranges: list[tuple[int, int]] = []

    for region in memory_bus.address_space.regions:
        if region.kind is RegionKind.ROM_IMAGE:
            assert region.image_bytes is not None
            binding = RomImageBinding(
                base=region.start,
                image_bytes=region.image_bytes,
                label=region.label,
            )
            constraints.extend(
                rom_image_byte_constraints(initial_memory, binding, ops)
            )
            constraints.extend(
                rom_image_no_write_constraints(
                    terminals, initial_memory, binding, ops,
                )
            )
            allowed_ranges.append((region.start, region.end))
        elif region.kind is RegionKind.RAM:
            allowed_ranges.append((region.start, region.end))
        elif region.kind is RegionKind.MMIO and region.device_id is not None:
            mmio_ranges.append((region.start, region.end))

    if not allowed_ranges and not mmio_ranges:
        return [z3.BoolVal(False)]

    seen: set[int] = set()
    for terminal in terminals:
        for addr in terminal.state.memory_touches:
            key = hash(addr)
            if key in seen:
                continue
            seen.add(key)
            if allowed_ranges and mmio_ranges:
                in_ram_rom = access_within_any_range(addr, 1, allowed_ranges)
                in_mmio = access_within_any_range(addr, 1, mmio_ranges)
                constraints.append(
                    z3.Implies(terminal.condition, z3.Or(in_ram_rom, in_mmio))
                )
            elif allowed_ranges:
                constraints.append(
                    z3.Implies(
                        terminal.condition,
                        access_within_any_range(addr, 1, allowed_ranges),
                    )
                )
            else:
                constraints.append(
                    z3.Implies(
                        terminal.condition,
                        access_within_any_range(addr, 1, mmio_ranges),
                    )
                )
    return constraints


def build_memory_bus_obligation(memory_bus: MemoryBus) -> dict[str, Any]:
    """Proof obligation block for ``proof_features: [\"memory-bus\"]``."""
    regions: list[dict[str, Any]] = []
    for region in memory_bus.address_space.regions:
        entry: dict[str, Any] = {
            "kind": region.kind.value,
            "start": region.start,
            "end": region.end,
        }
        if region.label is not None:
            entry["label"] = region.label
        if region.kind is RegionKind.ROM_IMAGE and region.image_sha256 is not None:
            entry["image_sha256"] = region.image_sha256
        if region.kind is RegionKind.MMIO and region.device_id is not None:
            entry["device_id"] = region.device_id
        regions.append(entry)
    obligation: dict[str, Any] = {
        "schema_version": MEMORY_BUS_OBLIGATION_SCHEMA_VERSION,
        "algorithm": MEMORY_BUS_ALGORITHM,
        "status": "pending",
        "regions": regions,
        "load_kinds": ["ram", "rom-image", "mmio-register-bank"],
        "store_kinds": ["ram", "mmio-register-bank", "gxfifo-stream"],
        "mmio": "cfg-routed",
        "unmapped": "fail-closed",
        "cfg_rejection_reasons": [],
        "loop_fifo_reject_markers": [],
        "mixed_space_symbolic_mmio": MIXED_SPACE_POLICY,
        "coverage": memory_bus_coverage_policies(memory_bus),
    }
    if _bus_has_fifo_devices(memory_bus):
        obligation["loop_fifo_policy"] = LOOP_FIFO_POLICY
        obligation["loop_fifo_emission"] = LOOP_FIFO_EMISSION
    spec = memory_bus.specification
    if spec is not None:
        obligation["bus_spec_sha256"] = spec.sha256()
    return obligation


def symbolic_mmio_still_fail_closed() -> bool:
    """Hook marker for pure-MMIO promotion authorization.

    Returns ``False`` after Track D unfreeze: ``memory-bus`` may authorize
    ``EQUIVALENT`` when the engine builds ``status=discharged``. Mixed
    RAM/ROM/MMIO symbolic addresses remain fail-closed at the CFG layer.
    """
    from tools.ppc_equivalence.proof_features import UNSUPPORTED_FOR_EQUIVALENT

    return "memory-bus" in UNSUPPORTED_FOR_EQUIVALENT


def _device_theory_map(memory_bus: MemoryBus) -> dict[str, str]:
    spec = memory_bus.specification
    if spec is None:
        return {}
    return {device.device_id: device.theory for device in spec.devices}


def _serialize_mmio_touch(touch: Any) -> dict[str, Any]:
    payload: dict[str, Any] = {
        "device_id": touch.device_id,
        "theory": touch.theory,
        "access": touch.access,
        "width": touch.width,
        "addr": touch.addr,
    }
    if touch.register_offset is not None:
        payload["register_offset"] = hex(touch.register_offset)
    return payload


def _serialize_symbolic_bus_observability(
    terminals: Sequence[Any] | None,
) -> dict[str, Any]:
    """Collect final symbolic register-bank / FIFO snapshots from terminals."""
    banks: dict[str, Any] = {}
    fifo: dict[str, Any] = {}
    if not terminals:
        return {"register_banks": banks, "fifo_traces": fifo}
    for terminal in terminals:
        bus = getattr(terminal.state, "symbolic_bus", None)
        if bus is None:
            continue
        for device_id, bank in bus.banks.items():
            if device_id not in bus.touched_devices:
                continue
            banks[device_id] = {
                hex(offset): str(value)
                for offset, value in sorted(bank.values.items())
            }
        for device_id, trace in bus.fifo_traces.items():
            if device_id not in bus.touched_devices:
                continue
            fifo[device_id] = [
                {
                    "addr": str(event.addr),
                    "width": event.width,
                    "value": str(event.value),
                }
                for event in trace.writes
            ]
    return {"register_banks": banks, "fifo_traces": fifo}


def collect_bus_mmio_observability(
    memory_bus: MemoryBus,
    *,
    original_terminals: Sequence[Any] | None = None,
    candidate_terminals: Sequence[Any] | None = None,
) -> dict[str, Any]:
    """Collect touched MMIO evidence and live device snapshots for obligations."""
    from tools.ppc_equivalence.symbolic_bus import collect_mmio_touches_from_terminals

    theories = _device_theory_map(memory_bus)
    observability: dict[str, Any] = {
        "register_banks": {},
        "fifo_traces": {},
        "symbolic": {
            "original": _serialize_symbolic_bus_observability(original_terminals),
            "candidate": _serialize_symbolic_bus_observability(candidate_terminals),
        },
        "touches": {"original": [], "candidate": []},
    }

    if original_terminals is not None:
        for touch in collect_mmio_touches_from_terminals(
            original_terminals,
            memory_bus.address_space,
            side="original",
            device_theories=theories,
            device_widths=_device_width_map(memory_bus),
        ):
            observability["touches"]["original"].append(_serialize_mmio_touch(touch))
    if candidate_terminals is not None:
        for touch in collect_mmio_touches_from_terminals(
            candidate_terminals,
            memory_bus.address_space,
            side="candidate",
            device_theories=theories,
            device_widths=_device_width_map(memory_bus),
        ):
            observability["touches"]["candidate"].append(_serialize_mmio_touch(touch))

    state = memory_bus.snapshot_state()
    for device_id, values in state.device_values.items():
        observability["register_banks"][device_id] = {
            hex(offset): hex(value) for offset, value in sorted(values.items())
        }
    for device_id, events in state.event_logs.items():
        observability["fifo_traces"][device_id] = list(events)
    return observability


def _device_width_map(memory_bus: MemoryBus) -> dict[str, int]:
    spec = memory_bus.specification
    if spec is None:
        return {}
    widths: dict[str, int] = {}
    for device in spec.devices:
        if device.theory == "register-bank":
            widths[device.device_id] = device.reg_width
        elif device.theory == "gxfifo-stream":
            widths[device.device_id] = 4
    return widths


def _terminals_have_symbolic_bus(terminals: Sequence[Any] | None) -> bool:
    if not terminals:
        return False
    return any(getattr(t.state, "symbolic_bus", None) is not None for t in terminals)


def _touched_device_ids(
    original_terminals: Sequence[Any] | None,
    candidate_terminals: Sequence[Any] | None,
) -> set[str]:
    touched: set[str] = set()
    for terminals in (original_terminals, candidate_terminals):
        if not terminals:
            continue
        for terminal in terminals:
            bus = getattr(terminal.state, "symbolic_bus", None)
            if bus is None:
                continue
            touched.update(bus.touched_devices)
    return touched


def build_access_coverage_attestation(
    *,
    attested: bool = True,
    opcode_families: Sequence[str] | None = None,
    rejections: Sequence[str] | None = None,
    status: str = "stubbed",
) -> dict[str, Any]:
    """Access-coverage attestation (Track B opcode families + rejects).

    When ``opcode_families`` is omitted, prefer the latest
    ``bus_access.last_bus_access_coverage()`` snapshot from a
    ``memory_bus=`` CFG run. Vacuous unsupported-access discharge is only
    valid when ``attested`` is true and ``sha256`` is bound into the vacuous
    side block.
    """
    rejection_list = list(rejections) if rejections is not None else []
    if opcode_families is None:
        from tools.ppc_equivalence.bus_access import last_bus_access_coverage

        snap = last_bus_access_coverage()
        if snap is not None:
            opcode_families = list(snap.get("families") or [])
            if rejections is None:
                rejection_list = list(snap.get("rejections") or [])
            if status == "stubbed":
                status = "observed"
    families = list(opcode_families) if opcode_families is not None else []
    identity = {
        "schema_version": 1,
        "status": status,
        "attested": attested,
        "opcode_families": families,
        "rejections": rejection_list,
    }
    return {
        **identity,
        "sha256": canonical_json_sha256(identity),
    }


def build_device_state_in_compare_attestation(
    *,
    included: bool,
    observability: dict[str, Any] | None = None,
) -> dict[str, Any]:
    """Attest that touched device/FIFO state entered the final compare."""
    digest = None
    if observability is not None:
        digest = canonical_json_sha256(
            {
                "kind": "device-state-in-compare",
                "observability": observability,
            }
        )
    return {
        "included": included,
        "digest_sha256": digest,
    }


def _theory_none_block() -> dict[str, Any]:
    return {"status": "none", "devices": []}


def _merge_unsupported_sides(
    cfg_side: dict[str, Any],
    observed_side: dict[str, Any],
) -> dict[str, Any]:
    """Prefer CFG digests (including vacuous); else observed-touch queries.

    Never rewrite empty CFG predicates into forged ``result=unsat``.
    """
    if cfg_side.get("status") == "vacuously-discharged":
        return cfg_side
    if cfg_side.get("result") not in (None, "not-queried"):
        return cfg_side
    return observed_side


def discharge_observed_mmio_unsupported_accesses(
    memory_bus: MemoryBus,
    *,
    original_terminals: Sequence[Any] | None = None,
    candidate_terminals: Sequence[Any] | None = None,
    ops: Any | None = None,
    deadline: Any | None = None,
    access_coverage_sha256: str | None = None,
) -> dict[str, Any]:
    """Per-terminal unsupported-access queries for MMIO touches / CFG predicates.

    Prefers CFG-collected ``unsupported_predicates`` on ``SymbolicBusState``.
    Falls back to concrete observed-touch discharge. Does not assume
    ``supported`` into the equivalence query. Empty CFG predicates emit
    vacuous discharge (not forged UNSAT) when coverage is attested.
    """
    from tools.ppc_equivalence.bus_spec import lift_symbolic_register_banks
    from tools.ppc_equivalence.deadline import Deadline
    from tools.ppc_equivalence.symbolic_bus import (
        access_supported,
        collect_mmio_touches_from_terminals,
        discharge_cfg_unsupported_accesses,
        discharge_unsupported_access,
    )

    empty = {
        "original": {"result": "not-queried", "query_sha256": None, "terminals": []},
        "candidate": {"result": "not-queried", "query_sha256": None, "terminals": []},
    }
    spec = memory_bus.specification
    if spec is None or ops is None or getattr(ops, "z3", None) is None:
        return empty

    z3 = ops.z3
    effective_deadline = deadline if deadline is not None else Deadline.after_ms(5_000)

    cfg_original = discharge_cfg_unsupported_accesses(
        original_terminals or (),
        side="original",
        deadline=effective_deadline,
        z3=z3,
        access_coverage_sha256=access_coverage_sha256,
    )
    cfg_candidate = discharge_cfg_unsupported_accesses(
        candidate_terminals or (),
        side="candidate",
        deadline=effective_deadline,
        z3=z3,
        access_coverage_sha256=access_coverage_sha256,
    )

    banks = lift_symbolic_register_banks(spec, z3)
    theories = _device_theory_map(memory_bus)
    widths = _device_width_map(memory_bus)

    def _observed_side(
        terminals: Sequence[Any] | None,
        *,
        side: str,
    ) -> dict[str, Any]:
        if terminals is None:
            return {"result": "not-queried", "query_sha256": None, "terminals": []}
        touches = collect_mmio_touches_from_terminals(
            terminals,
            memory_bus.address_space,
            side=side,
            device_theories=theories,
            device_widths=widths,
        )
        if not touches:
            return {"result": "not-queried", "query_sha256": None, "terminals": []}

        terminal_results: list[dict[str, Any]] = []
        worst = "unsat"
        last_hash: str | None = None
        total_elapsed_ms = 0.0
        for touch in touches:
            if touch.theory != "register-bank":
                continue
            bank = banks.get(touch.device_id)
            if bank is None:
                continue
            try:
                addr_int = int(touch.addr, 0) & 0xFFFFFFFF
            except (TypeError, ValueError):
                continue
            addr = z3.BitVecVal(addr_int, 32)
            supported = access_supported(addr, bank, z3, width=max(1, int(touch.width)))
            query = discharge_unsupported_access(
                path_condition=z3.BoolVal(True),
                supported=supported,
                deadline=effective_deadline,
                z3=z3,
            )
            last_hash = query.query_sha256
            total_elapsed_ms += query.elapsed_ms
            terminal_results.append(
                {
                    "device_id": touch.device_id,
                    "addr": touch.addr,
                    "width": touch.width,
                    "result": query.result,
                    "query_sha256": query.query_sha256,
                    "inconclusive": query.inconclusive,
                    "solver": {
                        "name": "z3",
                        "version": z3.get_version_string(),
                        "elapsed_ms": query.elapsed_ms,
                    },
                }
            )
            if query.status.value == "sat":
                worst = "sat"
            elif query.status.value == "unknown" and worst == "unsat":
                worst = "unknown"
        if not terminal_results:
            return {"result": "not-queried", "query_sha256": None, "terminals": []}
        return {
            "result": worst,
            "query_sha256": last_hash,
            "solver": {
                "name": "z3",
                "version": z3.get_version_string(),
                "elapsed_ms": total_elapsed_ms,
            },
            "terminals": terminal_results,
        }

    return {
        "original": _merge_unsupported_sides(
            cfg_original,
            _observed_side(original_terminals, side="original"),
        ),
        "candidate": _merge_unsupported_sides(
            cfg_candidate,
            _observed_side(candidate_terminals, side="candidate"),
        ),
    }


def _cfg_rejections(terminals: Sequence[Any] | None) -> list[str]:
    reasons: list[str] = []
    if not terminals:
        return reasons
    for terminal in terminals:
        bus = getattr(terminal.state, "symbolic_bus", None)
        if bus is None:
            continue
        reasons.extend(bus.rejections)
    return reasons


def _loop_fifo_reject_markers(terminals: Sequence[Any] | None) -> list[str]:
    """Track C hook: collect loop×FIFO rejection markers from CFG state."""
    markers: list[str] = []
    if not terminals:
        return markers
    for terminal in terminals:
        bus = getattr(terminal.state, "symbolic_bus", None)
        if bus is None:
            continue
        for reason in bus.rejections:
            if "fifo" in reason or "loop" in reason:
                markers.append(reason)
    return markers


def _side_is_discharged_form(side: dict[str, Any]) -> bool:
    if side.get("status") == "vacuously-discharged":
        return True
    return side.get("result") == "unsat" and isinstance(side.get("query_sha256"), str)


def enrich_memory_bus_obligation_with_symbolic_mmio(
    obligation: dict[str, Any],
    memory_bus: MemoryBus,
    *,
    original_terminals: Sequence[Any] | None = None,
    candidate_terminals: Sequence[Any] | None = None,
    ops: Any | None = None,
    deadline: Any | None = None,
    loop_summaries_active: bool = False,
) -> dict[str, Any]:
    """Attach PR-14/15 CFG routing blocks and MMIO observability.

    ``loop_summaries_active`` records the CFG hard-reject when affine/memory
    loop summaries coexist with FIFO devices (even if ``execute_cfg`` raised
    before producing terminals).
    """
    from tools.ppc_equivalence.symbolic_bus import build_register_bank_extensional_obligation
    from tools.ppc_equivalence.symbolic_event_trace import build_gxfifo_trace_obligation

    spec = memory_bus.specification
    if spec is None:
        return obligation

    enriched = dict(obligation)
    enriched["schema_version"] = MEMORY_BUS_OBLIGATION_SCHEMA_VERSION
    enriched.setdefault("algorithm", MEMORY_BUS_ALGORITHM)

    # Track C static policy attestation (additive / idempotent).
    coverage = dict(enriched.get("coverage") or {})
    coverage.update(memory_bus_coverage_policies(memory_bus))
    enriched["coverage"] = coverage
    enriched["mixed_space_symbolic_mmio"] = MIXED_SPACE_POLICY
    if _bus_has_fifo_devices(memory_bus):
        enriched["loop_fifo_policy"] = LOOP_FIFO_POLICY
        enriched["loop_fifo_emission"] = LOOP_FIFO_EMISSION

    rejections = _cfg_rejections(original_terminals) + _cfg_rejections(candidate_terminals)

    # Loop-summary × FIFO: hard-reject attestation (CFG raises before terminals).
    if _bus_has_fifo_devices(memory_bus) and loop_summaries_active:
        if LOOP_FIFO_REJECTION_REASON not in rejections:
            rejections.append(LOOP_FIFO_REJECTION_REASON)
        enriched["loop_fifo_rejection"] = {
            "reason": LOOP_FIFO_REJECTION_REASON,
            "policy": LOOP_FIFO_POLICY,
            "status": "hard-rejected",
            "bounded_summarized_emission": LOOP_FIFO_EMISSION,
        }
        coverage["loop_fifo"] = "hard-rejected"
        enriched["coverage"] = coverage

    # Mixed-space symbolic MMIO: explicit coverage/rejection reason (not silent).
    if MIXED_SPACE_REJECTION_REASON in rejections:
        enriched["mixed_space_rejection"] = MIXED_SPACE_REJECTION_REASON
        coverage["mixed_space"] = "rejected"
        enriched["coverage"] = coverage
    elif _bus_has_mmio_regions(memory_bus) and _bus_has_non_mmio_regions(memory_bus):
        coverage.setdefault("mixed_space", "fail-closed-policy")
        enriched["coverage"] = coverage

    rejection_reasons = sorted(set(rejections))
    enriched["cfg_rejection_reasons"] = rejection_reasons
    # Back-compat alias used by engine demotion path.
    if rejection_reasons:
        enriched["cfg_rejections"] = rejection_reasons
        enriched["symbolic_mmio"] = "cfg-routed-rejected"
    else:
        enriched["symbolic_mmio"] = "cfg-routed"

    loop_markers = sorted(
        set(
            _loop_fifo_reject_markers(original_terminals)
            + _loop_fifo_reject_markers(candidate_terminals)
        )
    )
    if LOOP_FIFO_REJECTION_REASON in rejection_reasons:
        if LOOP_FIFO_REJECTION_REASON not in loop_markers:
            loop_markers.append(LOOP_FIFO_REJECTION_REASON)
        loop_markers = sorted(set(loop_markers))
    enriched["loop_fifo_reject_markers"] = loop_markers

    observability = collect_bus_mmio_observability(
        memory_bus,
        original_terminals=original_terminals,
        candidate_terminals=candidate_terminals,
    )
    enriched["observability"] = observability

    access_coverage = build_access_coverage_attestation(attested=True, status="stubbed")
    enriched["access_coverage"] = access_coverage

    cfg_active = _terminals_have_symbolic_bus(original_terminals) or _terminals_have_symbolic_bus(
        candidate_terminals
    )
    enriched["device_state_in_compare"] = build_device_state_in_compare_attestation(
        included=cfg_active,
        observability=observability if cfg_active else None,
    )

    unsupported_access = discharge_observed_mmio_unsupported_accesses(
        memory_bus,
        original_terminals=original_terminals,
        candidate_terminals=candidate_terminals,
        ops=ops,
        deadline=deadline,
        access_coverage_sha256=access_coverage["sha256"],
    )
    # RAM/ROM-only buses never lift a SymbolicBusState. When both sides ran a
    # CFG (terminals present), access coverage is attested, and there are no
    # MMIO devices, emit vacuous unsupported-access so discharge can complete.
    if (
        not _bus_has_mmio_regions(memory_bus)
        and access_coverage.get("attested") is True
        and isinstance(access_coverage.get("sha256"), str)
        and original_terminals
        and candidate_terminals
    ):
        from tools.ppc_equivalence.symbolic_bus import (
            cfg_trace_sha256,
            vacuous_unsupported_access_block,
        )

        for side_name, terminals in (
            ("original", original_terminals),
            ("candidate", candidate_terminals),
        ):
            side = unsupported_access.get(side_name) or {}
            if side.get("result") in (None, "not-queried") and side.get("status") != (
                "vacuously-discharged"
            ):
                unsupported_access[side_name] = vacuous_unsupported_access_block(
                    cfg_trace_sha256=cfg_trace_sha256(terminals),
                    access_coverage_sha256=access_coverage["sha256"],
                )
    enriched["unsupported_access"] = unsupported_access
    if "bus_spec_sha256" not in enriched:
        enriched["bus_spec_sha256"] = spec.sha256()

    touched = _touched_device_ids(original_terminals, candidate_terminals)
    register_devices: list[dict[str, Any]] = []
    fifo_devices: list[dict[str, Any]] = []
    for device in spec.devices:
        for region in spec.address_space.regions:
            if (
                region.kind is RegionKind.MMIO
                and region.device_id == device.device_id
            ):
                entry = {
                    "device_id": device.device_id,
                    "theory": device.theory,
                    "region": {"start": region.start, "end": region.end},
                }
                if device.theory == "register-bank":
                    register_devices.append(entry)
                elif device.theory == "gxfifo-stream":
                    fifo_devices.append(entry)

    # Theory blocks: include when devices exist (touched or declared); else none.
    if register_devices:
        enriched["register_bank_theory"] = {
            "status": "present",
            "devices": register_devices,
            "touched": sorted(d["device_id"] for d in register_devices if d["device_id"] in touched),
        }
    else:
        enriched["register_bank_theory"] = _theory_none_block()
    if fifo_devices:
        enriched["fifo_theory"] = {
            "status": "present",
            "devices": fifo_devices,
            "touched": sorted(d["device_id"] for d in fifo_devices if d["device_id"] in touched),
        }
    else:
        enriched["fifo_theory"] = _theory_none_block()

    status = "cfg-routed" if not rejection_reasons else "cfg-routed-rejected"
    if register_devices:
        enriched["register_bank_extensional"] = build_register_bank_extensional_obligation(
            bus_spec_sha256=spec.sha256(),
            devices=register_devices,
            unsupported_access=unsupported_access,
            observability=enriched["observability"],
            status=status,
        )
    if fifo_devices:
        gxfifo = build_gxfifo_trace_obligation(
            bus_spec_sha256=spec.sha256(),
            devices=fifo_devices,
            status=status,
        )
        gxfifo["loop_fifo_policy"] = LOOP_FIFO_POLICY
        gxfifo["bounded_summarized_emission"] = LOOP_FIFO_EMISSION
        enriched["gxfifo_trace"] = gxfifo

    # Engine-proven discharge: both sides discharged form, no CFG rejections.
    # Loop×FIFO / mixed-space rejects also block discharged (Track C).
    original_ok = _side_is_discharged_form(unsupported_access.get("original") or {})
    candidate_ok = _side_is_discharged_form(unsupported_access.get("candidate") or {})
    hard_reject = bool(rejection_reasons) or bool(loop_markers) or bool(
        enriched.get("loop_fifo_rejection") or enriched.get("mixed_space_rejection")
    )
    if original_ok and candidate_ok and not hard_reject:
        enriched["status"] = "discharged"
    elif hard_reject:
        enriched["status"] = "cfg-routed-rejected"
    else:
        enriched["status"] = "pending"

    return enriched


def _is_sha256(value: Any) -> bool:
    if not isinstance(value, str) or len(value) != _SHA256_LEN:
        return False
    try:
        int(value, 16)
    except ValueError:
        return False
    return True


def _validate_solver_metadata(solver: Any, *, label: str) -> str | None:
    if not isinstance(solver, dict):
        return f"{label} must be an object"
    if solver.get("name") != "z3":
        return f"{label}.name must be 'z3'"
    if not isinstance(solver.get("version"), str) or not solver["version"]:
        return f"{label}.version must be a non-empty string"
    if not isinstance(solver.get("elapsed_ms"), (int, float)):
        return f"{label}.elapsed_ms must be a number"
    return None


def _validate_unsupported_access_side(
    side: Any,
    *,
    label: str,
    access_coverage: dict[str, Any] | None,
) -> str | None:
    if not isinstance(side, dict):
        return f"{label} must be an object"

    if side.get("status") == "vacuously-discharged":
        if side.get("reason") != "no-unsupported-predicates":
            return (
                f"{label}.reason must be 'no-unsupported-predicates' "
                "for vacuous discharge"
            )
        if not _is_sha256(side.get("cfg_trace_sha256")):
            return f"{label}.cfg_trace_sha256 must be a 64-hex digest"
        coverage_digest = side.get("access_coverage_sha256")
        if not _is_sha256(coverage_digest):
            return f"{label}.access_coverage_sha256 must be a 64-hex digest"
        if access_coverage is None:
            return f"{label} vacuous discharge requires parent access_coverage"
        if access_coverage.get("attested") is not True:
            return (
                f"{label} vacuous discharge requires "
                "access_coverage.attested=true"
            )
        expected = access_coverage.get("sha256")
        if expected != coverage_digest:
            return (
                f"{label}.access_coverage_sha256 must match "
                "access_coverage.sha256"
            )
        return None

    if side.get("result") == "unsat":
        if not _is_sha256(side.get("query_sha256")):
            return f"{label}.query_sha256 must be a 64-hex digest"
        return _validate_solver_metadata(side.get("solver"), label=f"{label}.solver")

    return (
        f"{label} must be result=unsat with digests or "
        "status=vacuously-discharged"
    )


def _validate_theory_block(block: Any, *, label: str) -> str | None:
    if not isinstance(block, dict):
        return f"{label} must be an object"
    status = block.get("status")
    if status == "none":
        devices = block.get("devices")
        if devices is not None and devices != []:
            return f"{label}.devices must be empty when status=none"
        return None
    if status != "present":
        return f"{label}.status must be 'none' or 'present'"
    devices = block.get("devices")
    if not isinstance(devices, list) or not devices:
        return f"{label}.devices must be a nonempty list when status=present"
    return None


def validate_memory_bus_obligation(obligation: dict[str, Any]) -> str | None:
    """Return None when a ``memory_bus`` obligation is well-formed.

    Structural checks always apply. ``status=discharged`` fails closed unless
    schema v2 attestations and per-side unsupported-access digests are present.
    Unknown / missing schema versions on discharged claims are rejected.
    """
    if obligation.get("algorithm") != MEMORY_BUS_ALGORITHM:
        return "memory_bus.algorithm must be memory-bus-v1"
    regions = obligation.get("regions")
    if not isinstance(regions, list) or not regions:
        return "memory_bus.regions must be a nonempty list"
    for index, region in enumerate(regions):
        if not isinstance(region, dict):
            return f"memory_bus.regions[{index}] must be an object"
        kind = region.get("kind")
        if not isinstance(kind, str) or not kind:
            return f"memory_bus.regions[{index}].kind must be a nonempty string"
        for field in ("start", "end"):
            value = region.get(field)
            if not isinstance(value, int) or value < 0 or value > 0xFFFFFFFF:
                return f"memory_bus.regions[{index}].{field} must be a u32"

    status = obligation.get("status")
    if status is None or status in ("pending", "cfg-routed", "cfg-routed-rejected", "failed"):
        return None

    if status != "discharged":
        return (
            "memory_bus.status must be pending|cfg-routed|cfg-routed-rejected|"
            "failed|discharged"
        )

    schema = obligation.get("schema_version")
    if schema != MEMORY_BUS_OBLIGATION_SCHEMA_VERSION:
        return (
            "memory_bus.status=discharged requires "
            f"schema_version={MEMORY_BUS_OBLIGATION_SCHEMA_VERSION}"
        )
    if not _is_sha256(obligation.get("bus_spec_sha256")):
        return "memory_bus.discharged requires bus_spec_sha256 (64-hex)"

    unsupported = obligation.get("unsupported_access")
    if not isinstance(unsupported, dict):
        return "memory_bus.discharged requires unsupported_access object"
    access_coverage = obligation.get("access_coverage")
    if not isinstance(access_coverage, dict):
        return "memory_bus.discharged requires access_coverage object"
    if access_coverage.get("attested") is not True:
        return "memory_bus.discharged requires access_coverage.attested=true"
    if not _is_sha256(access_coverage.get("sha256")):
        return "memory_bus.discharged requires access_coverage.sha256"

    for side_name in ("original", "candidate"):
        reason = _validate_unsupported_access_side(
            unsupported.get(side_name),
            label=f"memory_bus.unsupported_access.{side_name}",
            access_coverage=access_coverage,
        )
        if reason is not None:
            return reason

    for key in ("register_bank_theory", "fifo_theory"):
        reason = _validate_theory_block(obligation.get(key), label=f"memory_bus.{key}")
        if reason is not None:
            return reason

    compare = obligation.get("device_state_in_compare")
    if not isinstance(compare, dict):
        return "memory_bus.discharged requires device_state_in_compare object"
    if not isinstance(compare.get("included"), bool):
        return "memory_bus.device_state_in_compare.included must be a bool"
    digest = compare.get("digest_sha256")
    if digest is not None and not _is_sha256(digest):
        return "memory_bus.device_state_in_compare.digest_sha256 must be 64-hex or null"

    cfg_reasons = obligation.get("cfg_rejection_reasons")
    if not isinstance(cfg_reasons, list):
        return "memory_bus.discharged requires cfg_rejection_reasons list"
    if cfg_reasons:
        joined = ",".join(str(item) for item in cfg_reasons)
        if MIXED_SPACE_REJECTION_REASON in cfg_reasons:
            return (
                "memory_bus.discharged incompatible with mixed-space CFG "
                f"rejection ({MIXED_SPACE_REJECTION_REASON})"
            )
        if LOOP_FIFO_REJECTION_REASON in cfg_reasons:
            return (
                "memory_bus.discharged incompatible with loop×FIFO "
                f"hard-reject ({LOOP_FIFO_REJECTION_REASON})"
            )
        return f"memory_bus.discharged requires empty cfg_rejection_reasons ({joined})"

    loop_markers = obligation.get("loop_fifo_reject_markers")
    if not isinstance(loop_markers, list):
        return "memory_bus.discharged requires loop_fifo_reject_markers list"
    if loop_markers:
        return (
            "memory_bus.discharged incompatible with loop×FIFO reject markers "
            f"({','.join(str(item) for item in loop_markers)})"
        )

    # Track C policy fields: when present, must not contradict discharge.
    if obligation.get("loop_fifo_rejection") is not None:
        return "memory_bus.discharged incompatible with loop×FIFO rejection attestation"
    if obligation.get("mixed_space_rejection") is not None:
        return "memory_bus.discharged incompatible with mixed-space rejection attestation"
    coverage = obligation.get("coverage")
    if isinstance(coverage, dict):
        if coverage.get("mixed_space") == "rejected":
            return "memory_bus.discharged incompatible with mixed-space coverage=rejected"
        if coverage.get("loop_fifo") == "hard-rejected":
            return "memory_bus.discharged incompatible with loop×FIFO coverage=hard-rejected"

    return None
