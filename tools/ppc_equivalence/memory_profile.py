from __future__ import annotations

from dataclasses import dataclass, field
from enum import Enum
from typing import Any


class MemoryProfile(str, Enum):
    ASSUMED_ORDINARY_RAM = "assumed-ordinary-ram"
    BOUNDED_ORDINARY_RAM = "bounded-ordinary-ram"
    STACK_AND_KNOWN_GLOBALS = "stack-and-known-globals"
    HARDWARE_AWARE = "hardware-aware"


PROFILE_CHOICES = [p.value for p in MemoryProfile]


@dataclass(slots=True)
class MemoryEnvironment:
    profile: MemoryProfile = MemoryProfile.ASSUMED_ORDINARY_RAM
    ranges: list[tuple[int, int]] = field(default_factory=list)
    alignment: str = "instruction-natural"

    def to_dict(self) -> dict[str, Any]:
        return {
            "memory_profile": self.profile.value,
            "ranges": [
                {"start": hex(low), "end": hex(high)}
                for low, high in self.ranges
            ],
            "alignment": self.alignment,
            "mmio": "excluded-by-range" if self.ranges else "unrestricted",
            "address_wraparound": "rejected",
        }

    @classmethod
    def from_dict(cls, d: dict[str, Any]) -> MemoryEnvironment:
        profile = MemoryProfile(d.get("memory_profile", "assumed-ordinary-ram"))
        ranges_raw = d.get("ranges", [])
        ranges = [
            (int(r["start"], 0), int(r["end"], 0))
            for r in ranges_raw
        ]
        alignment = d.get("alignment", "instruction-natural")
        return cls(profile=profile, ranges=ranges, alignment=alignment)


def parse_ranges(range_args: list[str]) -> list[tuple[int, int]]:
    parsed: list[tuple[int, int]] = []
    for arg in range_args:
        parts = arg.split(",")
        if len(parts) != 2:
            raise ValueError(
                f"invalid range {arg!r}: expected low,high (hex or decimal)"
            )
        low = int(parts[0], 0)
        high = int(parts[1], 0)
        if low < 0 or high < 0 or low > 0xFFFFFFFF or high > 0xFFFFFFFF:
            raise ValueError(f"range {arg!r} out of 32-bit address space")
        if low > high:
            raise ValueError(f"range {arg!r} low > high")
        parsed.append((low, high))
    return parsed


def access_within_range(addr: Any, width_bytes: int, low: int, high: int) -> Any:
    import z3

    if width_bytes <= 0:
        raise ValueError("width_bytes must be positive")
    if width_bytes == 1:
        return z3.And(
            z3.UGE(addr, z3.BitVecVal(low, 32)),
            z3.ULE(addr, z3.BitVecVal(high, 32)),
        )
    width = z3.BitVecVal(width_bytes - 1, 32)
    last = addr + width
    return z3.And(
        z3.UGE(addr, z3.BitVecVal(low, 32)),
        z3.ULE(last, z3.BitVecVal(high, 32)),
        z3.UGE(last, addr),
    )


def access_within_any_range(addr: Any, width_bytes: int, ranges: list[tuple[int, int]]) -> Any:
    import z3

    if not ranges:
        return z3.BoolVal(True)
    return z3.Or(*[
        access_within_range(addr, width_bytes, low, high)
        for low, high in ranges
    ])


def build_memory_constraints(
    original_exits: list[Any],
    candidate_exits: list[Any],
    environment: MemoryEnvironment,
    ops: Any,
) -> list[Any]:
    if environment.profile == MemoryProfile.ASSUMED_ORDINARY_RAM:
        return []
    if not environment.ranges:
        return []
    z3 = ops.z3
    constraints: list[Any] = []
    seen: set[int] = set()
    for terminal in original_exits + candidate_exits:
        for addr in terminal.state.memory_touches:
            h = hash(addr)
            if h in seen:
                continue
            seen.add(h)
            constraints.append(
                z3.Implies(
                    terminal.condition,
                    access_within_any_range(addr, 1, environment.ranges),
                )
            )
    return constraints
