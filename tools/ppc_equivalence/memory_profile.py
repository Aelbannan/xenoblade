from __future__ import annotations

from dataclasses import dataclass, field
from enum import Enum
from typing import Any

from tools.ppc_equivalence.model import InvalidReason


class MemoryProfile(str, Enum):
    ASSUMED_ORDINARY_RAM = "assumed-ordinary-ram"
    BOUNDED_ORDINARY_RAM = "bounded-ordinary-ram"
    STACK_AND_KNOWN_GLOBALS = "stack-and-known-globals"
    HARDWARE_AWARE = "hardware-aware"


PROFILE_CHOICES = [p.value for p in MemoryProfile]

# Constrained profiles use the same range-builder once nonempty ranges are
# supplied (documented in SOUNDNESS.md). Claiming any of these without
# explicit ranges fail-closes — never silently degrades to unconstrained RAM.
PROFILES_RANGE_CONSTRAINED = frozenset({
    MemoryProfile.BOUNDED_ORDINARY_RAM,
    MemoryProfile.STACK_AND_KNOWN_GLOBALS,
    MemoryProfile.HARDWARE_AWARE,
})

# Alias: every range-constrained profile requires nonempty ranges.
PROFILES_REQUIRING_RANGES = PROFILES_RANGE_CONSTRAINED

MEMORY_PROFILE_VIOLATION = InvalidReason.MEMORY_PROFILE_VIOLATION


@dataclass(slots=True)
class MemoryEnvironment:
    profile: MemoryProfile = MemoryProfile.ASSUMED_ORDINARY_RAM
    ranges: list[tuple[int, int]] = field(default_factory=list)
    alignment: str = "instruction-natural"

    def requires_ranges(self) -> bool:
        return self.profile in PROFILES_REQUIRING_RANGES

    def is_fail_closed_empty(self) -> bool:
        """True when the profile forbids unconstrained RAM and ranges are empty."""
        return self.requires_ranges() and not self.ranges

    def to_dict(self) -> dict[str, Any]:
        if self.ranges:
            mmio = "excluded-by-range"
        elif self.is_fail_closed_empty():
            mmio = "fail-closed-no-ranges"
        else:
            mmio = "unrestricted"
        return {
            "memory_profile": self.profile.value,
            "ranges": [
                {"start": hex(low), "end": hex(high)}
                for low, high in self.ranges
            ],
            "alignment": self.alignment,
            "mmio": mmio,
            "address_wraparound": "rejected",
            "fail_closed_empty_ranges": self.is_fail_closed_empty(),
            "invalid_reason_code": MEMORY_PROFILE_VIOLATION.value,
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
        # Empty range list is not a universal accept — callers must decide
        # profile policy before invoking this helper.
        return z3.BoolVal(False)
    return z3.Or(*[
        access_within_range(addr, width_bytes, low, high)
        for low, high in ranges
    ])


def is_bounded_with_ranges(environment: MemoryEnvironment | None) -> bool:
    """True when *environment* is a range-constrained profile with nonempty ranges."""
    if environment is None:
        return False
    return (
        environment.profile in PROFILES_RANGE_CONSTRAINED
        and bool(environment.ranges)
        and not environment.is_fail_closed_empty()
    )


def build_memory_constraints(
    original_exits: list[Any],
    candidate_exits: list[Any],
    environment: MemoryEnvironment,
    ops: Any,
) -> list[Any]:
    """Build SMT constraints for the selected memory profile.

    Profile policy:
    - ``assumed-ordinary-ram``: no range constraints (external assumption).
    - ``bounded-ordinary-ram``, ``stack-and-known-globals``, ``hardware-aware``:
      require nonempty explicit ranges. With ranges, every touched byte address
      on a feasible path must lie in a configured range (no wraparound). Without
      ranges, fail closed via an unsat domain constraint (engine reports
      ``INCONCLUSIVE_LAYOUT``) and never silently degrade to unconstrained RAM.
      Out-of-range accesses are excluded from the quantified domain;
      ``MEMORY_PROFILE_VIOLATION`` is the reserved InvalidReason code for
      per-access validity tagging.
    """
    z3 = ops.z3

    if environment.profile == MemoryProfile.ASSUMED_ORDINARY_RAM:
        return []

    if environment.profile not in PROFILES_RANGE_CONSTRAINED:
        return []

    if not environment.ranges:
        # Fail closed: do not silently treat as unconstrained RAM.
        # Force layout/domain infeasibility so the engine cannot claim
        # EQUIVALENT under an incomplete profile configuration.
        return [z3.BoolVal(False)]

    constraints: list[Any] = []
    seen: set[int] = set()
    for terminal in original_exits + candidate_exits:
        for addr in terminal.state.memory_touches:
            h = hash(addr)
            if h in seen:
                continue
            seen.add(h)
            # memory_touches are already per-byte; width=1 is correct.
            constraints.append(
                z3.Implies(
                    terminal.condition,
                    access_within_any_range(addr, 1, environment.ranges),
                )
            )
    return constraints
