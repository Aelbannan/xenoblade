"""Object-base MEM1 constraints and RAM-only bus projection helpers.

Opt-in helpers for ordinary member-access proofs under hardware/platform
profiles that include both RAM and MMIO. Callers (coop equivalence_check)
may:

1. Constrain symbolic GPRs (esp. ``r3``) into MEM1 / profile RAM via
   ``mem1_gpr_ranges`` → ``check_equivalence(..., initial_gpr_ranges=)``.
2. Strip MMIO regions/devices from the bus when neither side can form an
   MMIO address from immediates/literals (``should_use_ram_only_bus`` /
   ``ram_only_memory_bus``).

These are assumption-bearing projections: they reduce
``symbolic-mmio-mixed-address-space`` inconclusive outcomes on RAM-only
member loads, at the cost of excluding MMIO-touching interpretations.
"""

from __future__ import annotations

from typing import Any, Mapping, Sequence

from tools.ppc_equivalence.address_space import AddressSpace, Region, RegionKind
from tools.ppc_equivalence.ir import Instruction, Opcode
from tools.ppc_equivalence.memory_bus import MemoryBus, build_memory_bus

# Inclusive end matching platform profiles (MEM1).
MEM1_RANGE: tuple[int, int] = (0x80000000, 0x817FFFFF)

# Common Wii Broadway MMIO high-halves used by ``lis`` / ``addis``.
_KNOWN_MMIO_HIGH_HALVES: frozenset[int] = frozenset({0xCC00, 0xCD00})

_MMIO_RELOC_NEEDLES: tuple[str, ...] = ("gx", "fifo", "mmio", "wgpipe")

_ADDIS_STYLE: frozenset[Opcode] = frozenset({Opcode.ADDIS})


def _addr_int(value: Any) -> int:
    if isinstance(value, int):
        return value & 0xFFFFFFFF
    text = str(value).strip()
    if text.lower().startswith("0x"):
        return int(text, 16) & 0xFFFFFFFF
    return int(text, 0) & 0xFFFFFFFF


def _range_contains(lo: int, hi: int, addr: int) -> bool:
    return lo <= addr <= hi


def _high_half_in_mmio(imm16: int, mmio_ranges: Sequence[tuple[int, int]]) -> bool:
    """True when ``imm16 << 16`` lands in (or is the high half of) an MMIO span."""
    base = (imm16 & 0xFFFF) << 16
    if imm16 & 0xFFFF in _KNOWN_MMIO_HIGH_HALVES:
        return True
    for start, end in mmio_ranges:
        if _range_contains(start, end, base):
            return True
        # High-half equals the MMIO region's high 16 bits (e.g. 0xCC00 for 0xCC00xxxx).
        if (start >> 16) & 0xFFFF == (imm16 & 0xFFFF):
            return True
        if (end >> 16) & 0xFFFF == (imm16 & 0xFFFF):
            return True
    return False


def ram_ranges_from_platform_profile(
    profile: Mapping[str, Any] | None,
) -> list[tuple[int, int]]:
    """Extract inclusive ``(start, end)`` for ``kind==ram`` regions.

    Falls back to ``[MEM1_RANGE]`` when ``profile`` is missing or has no RAM.
    """
    if not profile:
        return [MEM1_RANGE]
    ranges: list[tuple[int, int]] = []
    for entry in profile.get("regions", []) or []:
        if not isinstance(entry, Mapping):
            continue
        if str(entry.get("kind", "")).lower() != "ram":
            continue
        try:
            start = _addr_int(entry["start"])
            end = _addr_int(entry["end"])
        except (KeyError, TypeError, ValueError):
            continue
        ranges.append((start, end))
    return ranges or [MEM1_RANGE]


def mem1_gpr_ranges(
    *,
    registers: tuple[int, ...] = (3,),
    profile: Mapping[str, Any] | None = None,
) -> dict[int, tuple[int, int]]:
    """Map each register to the primary MEM1-like / first RAM range.

    When multiple RAM regions exist, prefers the region with
    ``start == 0x80000000`` (MEM1); otherwise uses the first RAM region.
    """
    ranges = ram_ranges_from_platform_profile(profile)
    primary = ranges[0]
    for start, end in ranges:
        if start == MEM1_RANGE[0]:
            primary = (start, end)
            break
    out: dict[int, tuple[int, int]] = {}
    for reg in registers:
        r = int(reg)
        if not 0 <= r < 32:
            raise ValueError(f"GPR register out of range: {r}")
        out[r] = (int(primary[0]) & 0xFFFFFFFF, int(primary[1]) & 0xFFFFFFFF)
    return out


def mmio_ranges_from_bus_or_profile(
    memory_bus: MemoryBus | None = None,
    profile: Mapping[str, Any] | None = None,
) -> list[tuple[int, int]]:
    """Collect inclusive MMIO ``(start, end)`` from a bus and/or profile."""
    ranges: list[tuple[int, int]] = []
    seen: set[tuple[int, int]] = set()

    def _add(start: int, end: int) -> None:
        key = (start & 0xFFFFFFFF, end & 0xFFFFFFFF)
        if key not in seen:
            seen.add(key)
            ranges.append(key)

    if memory_bus is not None:
        for region in memory_bus.address_space.regions:
            if region.kind is RegionKind.MMIO:
                _add(region.start, region.end)

    if profile:
        for entry in profile.get("regions", []) or []:
            if not isinstance(entry, Mapping):
                continue
            if str(entry.get("kind", "")).lower() != "mmio":
                continue
            try:
                _add(_addr_int(entry["start"]), _addr_int(entry["end"]))
            except (KeyError, TypeError, ValueError):
                continue

    return ranges


def instruction_may_form_mmio_address(
    instructions: list[Instruction],
    mmio_ranges: list[tuple[int, int]],
) -> bool:
    """Heuristic: True if any immediate / reloc could land in MMIO.

    Practical v1 checks:

    * Low 16 bits of each raw word (unsigned and signed) as high-half bases
      via ``imm << 16``.
    * ``addis`` / ``lis``-style immediates (operand or raw low 16).
    * Known Wii MMIO high-halves ``0xCC00`` / ``0xCD00``.
    * Relocation symbol / canonical names containing ``gx``, ``fifo``,
      ``mmio``, or ``WGPIPE``.
    """
    effective_ranges = list(mmio_ranges)
    # Always consult known Wii high-halves even when ranges are empty so a
    # lone ``lis ..., 0xCC00`` is flagged.
    for insn in instructions:
        if insn.relocation is not None:
            for name in (insn.relocation.symbol, insn.relocation.canonical_symbol):
                lower = str(name).lower()
                if any(needle in lower for needle in _MMIO_RELOC_NEEDLES):
                    return True

        raw_imm = insn.raw & 0xFFFF

        if _high_half_in_mmio(raw_imm, effective_ranges):
            return True

        if insn.opcode in _ADDIS_STYLE:
            # Prefer decoded immediate when present (operands[-1] for D-form).
            if insn.operands:
                imm = int(insn.operands[-1]) & 0xFFFF
            else:
                imm = raw_imm
            if _high_half_in_mmio(imm, effective_ranges):
                return True

        # Operand immediates on D-form loads/stores / addi (last operand):
        # only known / MMIO high-halves count — ordinary small displacements
        # (e.g. member offsets) must not trip this.
        if len(insn.operands) >= 3:
            imm = int(insn.operands[-1]) & 0xFFFF
            if imm in _KNOWN_MMIO_HIGH_HALVES:
                return True
            if any((start >> 16) & 0xFFFF == imm for start, _ in effective_ranges):
                return True

    return False


def _bus_has_mmio(memory_bus: MemoryBus) -> bool:
    if memory_bus.devices:
        return True
    return any(r.kind is RegionKind.MMIO for r in memory_bus.address_space.regions)


def ram_only_memory_bus(memory_bus: MemoryBus) -> MemoryBus:
    """Return a new ``MemoryBus`` with only RAM/ROM regions and no MMIO devices.

    Rebuilds via ``build_memory_bus``. Preserves reviewed-profile identity
    fields when present so callers can still see provenance; the projection
    itself is an explicit RAM-only assumption recorded by the integrator.
    """
    kept: list[Region] = [
        region
        for region in memory_bus.address_space.regions
        if region.kind in (RegionKind.RAM, RegionKind.ROM_IMAGE)
    ]
    projected = build_memory_bus(AddressSpace(tuple(kept)), devices={})
    projected.hardware_profile_name = memory_bus.hardware_profile_name
    projected.hardware_profile_sha256 = memory_bus.hardware_profile_sha256
    # Device models were stripped; clear the digest so promotion cannot treat
    # this as the full reviewed MMIO profile.
    projected.device_models_sha256 = None
    return projected


def should_use_ram_only_bus(
    original: list[Instruction],
    candidate: list[Instruction],
    memory_bus: MemoryBus | None,
) -> bool:
    """True when ``memory_bus`` has MMIO and neither side can form MMIO addresses."""
    if memory_bus is None or not _bus_has_mmio(memory_bus):
        return False
    mmio_ranges = mmio_ranges_from_bus_or_profile(memory_bus=memory_bus)
    if instruction_may_form_mmio_address(original, mmio_ranges):
        return False
    if instruction_may_form_mmio_address(candidate, mmio_ranges):
        return False
    return True


def format_object_base_assumption(reg: int, lo: int, hi: int) -> str:
    """Assumption string recorded on ``ProofResult`` when a GPR range is applied."""
    return f"object-base-mem1:r{int(reg)}:[0x{lo & 0xFFFFFFFF:08x},0x{hi & 0xFFFFFFFF:08x}]"
