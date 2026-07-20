"""SMT obligations and proof metadata for opt-in ``MemoryBus`` routing.

MMIO register-bank / FIFO formulas live in ``symbolic_bus`` /
``symbolic_event_trace``. Reachable unsupported accesses are separate proof
queries (``path ∧ ¬supported``); ``supported`` is never assumed into the
equivalence query. CFG routing is live for SymbolicOps; ``memory-bus`` remains
in ``UNSUPPORTED_FOR_EQUIVALENT`` until the enablement gate is met.
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
        "algorithm": "memory-bus-v1",
        "regions": regions,
        "load_kinds": ["ram", "rom-image", "mmio-register-bank"],
        "store_kinds": ["ram", "mmio-register-bank", "gxfifo-stream"],
        "mmio": "cfg-routed-frozen",
        "unmapped": "fail-closed",
    }
    spec = memory_bus.specification
    if spec is not None:
        obligation["bus_spec_sha256"] = spec.sha256()
    return obligation


def symbolic_mmio_still_fail_closed() -> bool:
    """Hook marker: MMIO CFG routing is live but not promotion-authorized.

    Returns ``True`` while ``memory-bus`` remains in
    ``UNSUPPORTED_FOR_EQUIVALENT``. CFG routing and obligation digests do not
    by themselves clear the enablement gate.
    """
    return True


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


def _merge_unsupported_sides(
    cfg_side: dict[str, Any],
    observed_side: dict[str, Any],
    *,
    cfg_active: bool,
) -> dict[str, Any]:
    """Prefer CFG digests; fall back to post-hoc observed-touch queries."""
    if cfg_side.get("result") not in (None, "not-queried"):
        return cfg_side
    if cfg_active and cfg_side.get("result") == "not-queried":
        # CFG ran with no recorded unsupported predicates ⇒ all routed accesses
        # were supported (separate from observed width-1 touch fallback).
        return {
            "result": "unsat",
            "query_sha256": None,
            "terminals": [],
            "note": "cfg-no-unsupported-predicates",
        }
    return observed_side


def discharge_observed_mmio_unsupported_accesses(
    memory_bus: MemoryBus,
    *,
    original_terminals: Sequence[Any] | None = None,
    candidate_terminals: Sequence[Any] | None = None,
    ops: Any | None = None,
    deadline: Any | None = None,
) -> dict[str, Any]:
    """Per-terminal unsupported-access queries for MMIO touches / CFG predicates.

    Prefers CFG-collected ``unsupported_predicates`` on ``SymbolicBusState``.
    Falls back to concrete observed-touch discharge. Does not assume
    ``supported`` into the equivalence query.
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
    )
    cfg_candidate = discharge_cfg_unsupported_accesses(
        candidate_terminals or (),
        side="candidate",
        deadline=effective_deadline,
        z3=z3,
    )

    banks = lift_symbolic_register_banks(spec, z3)
    theories = _device_theory_map(memory_bus)
    widths = _device_width_map(memory_bus)
    cfg_active_original = _terminals_have_symbolic_bus(original_terminals)
    cfg_active_candidate = _terminals_have_symbolic_bus(candidate_terminals)

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
            terminal_results.append(
                {
                    "device_id": touch.device_id,
                    "addr": touch.addr,
                    "width": touch.width,
                    "result": query.result,
                    "query_sha256": query.query_sha256,
                    "inconclusive": query.inconclusive,
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
            "terminals": terminal_results,
        }

    return {
        "original": _merge_unsupported_sides(
            cfg_original,
            _observed_side(original_terminals, side="original"),
            cfg_active=cfg_active_original,
        ),
        "candidate": _merge_unsupported_sides(
            cfg_candidate,
            _observed_side(candidate_terminals, side="candidate"),
            cfg_active=cfg_active_candidate,
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


def enrich_memory_bus_obligation_with_symbolic_mmio(
    obligation: dict[str, Any],
    memory_bus: MemoryBus,
    *,
    original_terminals: Sequence[Any] | None = None,
    candidate_terminals: Sequence[Any] | None = None,
    ops: Any | None = None,
    deadline: Any | None = None,
) -> dict[str, Any]:
    """Attach PR-14/15 CFG routing blocks and MMIO observability."""
    from tools.ppc_equivalence.symbolic_bus import build_register_bank_extensional_obligation
    from tools.ppc_equivalence.symbolic_event_trace import build_gxfifo_trace_obligation

    spec = memory_bus.specification
    if spec is None:
        return obligation

    enriched = dict(obligation)
    rejections = _cfg_rejections(original_terminals) + _cfg_rejections(candidate_terminals)
    if rejections:
        enriched["symbolic_mmio"] = "cfg-routed-rejected"
        enriched["cfg_rejections"] = sorted(set(rejections))
    else:
        enriched["symbolic_mmio"] = "cfg-routed"
    enriched["observability"] = collect_bus_mmio_observability(
        memory_bus,
        original_terminals=original_terminals,
        candidate_terminals=candidate_terminals,
    )

    unsupported_access = discharge_observed_mmio_unsupported_accesses(
        memory_bus,
        original_terminals=original_terminals,
        candidate_terminals=candidate_terminals,
        ops=ops,
        deadline=deadline,
    )
    enriched["unsupported_access"] = unsupported_access
    if "bus_spec_sha256" not in enriched:
        enriched["bus_spec_sha256"] = spec.sha256()

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

    status = "cfg-routed" if not rejections else "cfg-routed-rejected"
    if register_devices:
        enriched["register_bank_extensional"] = build_register_bank_extensional_obligation(
            bus_spec_sha256=spec.sha256(),
            devices=register_devices,
            unsupported_access=unsupported_access,
            observability=enriched["observability"],
            status=status,
        )
    if fifo_devices:
        enriched["gxfifo_trace"] = build_gxfifo_trace_obligation(
            bus_spec_sha256=spec.sha256(),
            devices=fifo_devices,
            status=status,
        )
    return enriched


def validate_memory_bus_obligation(obligation: dict[str, Any]) -> str | None:
    if obligation.get("algorithm") != "memory-bus-v1":
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
    return None
