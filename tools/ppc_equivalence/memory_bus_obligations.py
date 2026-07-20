"""SMT obligations and proof metadata for opt-in ``MemoryBus`` routing.

MMIO remains fail-closed in the equivalence solver. Extensional symbolic
register-bank formulas live in ``symbolic_bus`` (separate unsupported-access
queries; do not assume ``supported`` into equivalence). Obligation/evidence
hooks attach scaffold blocks and MMIO touch observability without authorizing
``EQUIVALENT``.
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
    """Bind symbolic memory to the bus address space (Tier C, fail-closed)."""
    z3 = ops.z3
    constraints: list[Any] = []
    allowed_ranges: list[tuple[int, int]] = []

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

    if not allowed_ranges:
        return [z3.BoolVal(False)]

    seen: set[int] = set()
    for terminal in terminals:
        for addr in terminal.state.memory_touches:
            key = hash(addr)
            if key in seen:
                continue
            seen.add(key)
            constraints.append(
                z3.Implies(
                    terminal.condition,
                    access_within_any_range(addr, 1, allowed_ranges),
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
    return {
        "algorithm": "memory-bus-v1",
        "regions": regions,
        "load_kinds": ["ram", "rom-image"],
        "store_kinds": ["ram"],
        "mmio": "fail-closed",
        "unmapped": "fail-closed",
    }


def symbolic_mmio_still_fail_closed() -> bool:
    """Hook marker: symbolic MMIO theory is scaffolded but not solver-bound.

    ``build_memory_bus_constraints`` continues to exclude MMIO from feasible
    ranges. When PR 14 fully wires ``symbolic_bus`` into terminal compare and
    discharges obligations, this returns ``False``.
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
        "touches": {"original": [], "candidate": []},
    }

    if original_terminals is not None:
        for touch in collect_mmio_touches_from_terminals(
            original_terminals,
            memory_bus.address_space,
            side="original",
            device_theories=theories,
        ):
            observability["touches"]["original"].append(_serialize_mmio_touch(touch))
    if candidate_terminals is not None:
        for touch in collect_mmio_touches_from_terminals(
            candidate_terminals,
            memory_bus.address_space,
            side="candidate",
            device_theories=theories,
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


def enrich_memory_bus_obligation_with_symbolic_mmio(
    obligation: dict[str, Any],
    memory_bus: MemoryBus,
    *,
    original_terminals: Sequence[Any] | None = None,
    candidate_terminals: Sequence[Any] | None = None,
) -> dict[str, Any]:
    """Attach PR-14/15 scaffold blocks and MMIO observability (fail-closed)."""
    from tools.ppc_equivalence.symbolic_bus import build_register_bank_extensional_obligation
    from tools.ppc_equivalence.symbolic_event_trace import build_gxfifo_trace_obligation

    spec = memory_bus.specification
    if spec is None:
        return obligation

    enriched = dict(obligation)
    enriched["symbolic_mmio"] = "scaffolded"
    enriched["observability"] = collect_bus_mmio_observability(
        memory_bus,
        original_terminals=original_terminals,
        candidate_terminals=candidate_terminals,
    )

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

    if register_devices:
        enriched["register_bank_extensional"] = build_register_bank_extensional_obligation(
            bus_spec_sha256=spec.sha256(),
            devices=register_devices,
            observability=enriched["observability"],
            status="scaffolded",
        )
    if fifo_devices:
        enriched["gxfifo_trace"] = build_gxfifo_trace_obligation(
            bus_spec_sha256=spec.sha256(),
            devices=fifo_devices,
            status="scaffolded",
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
