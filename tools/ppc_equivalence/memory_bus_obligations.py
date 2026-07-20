"""SMT obligations and proof metadata for opt-in ``MemoryBus`` routing."""

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
