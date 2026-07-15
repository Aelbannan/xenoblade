from __future__ import annotations

from dataclasses import dataclass, replace
from typing import Any


@dataclass(frozen=True, slots=True)
class XerState:
    ca: Any
    ov: Any
    so: Any


@dataclass(frozen=True, slots=True)
class ConcreteMemory:
    default: int = 0
    bytes: tuple[tuple[int, int], ...] = ()

    def read(self, address: int) -> int:
        return dict(self.bytes).get(address & 0xFFFFFFFF, self.default)

    def write(self, address: int, value: int) -> "ConcreteMemory":
        items = dict(self.bytes)
        address &= 0xFFFFFFFF
        value &= 0xFF
        if value == self.default:
            items.pop(address, None)
        else:
            items[address] = value
        return ConcreteMemory(self.default, tuple(sorted(items.items())))


@dataclass(frozen=True, slots=True)
class MachineState:
    gpr: tuple[Any, ...]
    fpr: tuple[Any, ...]
    ps1: tuple[Any, ...]
    gqr: tuple[Any, ...]
    cr: Any
    xer: XerState
    fpscr: Any
    lr: Any
    ctr: Any
    memory: Any
    valid: Any
    memory_touches: tuple[Any, ...] = ()

    def with_gpr(self, index: int, value: Any) -> "MachineState":
        registers = list(self.gpr)
        registers[index] = value
        return replace(self, gpr=tuple(registers))

    def with_fpr(self, index: int, value: Any) -> "MachineState":
        registers = list(self.fpr)
        registers[index] = value
        return replace(self, fpr=tuple(registers))

    def with_ps1(self, index: int, value: Any) -> "MachineState":
        registers = list(self.ps1)
        registers[index] = value
        return replace(self, ps1=tuple(registers))

    def with_gqr(self, index: int, value: Any) -> "MachineState":
        registers = list(self.gqr)
        registers[index] = value
        return replace(self, gqr=tuple(registers))

    def with_cr(self, value: Any) -> "MachineState":
        return replace(self, cr=value)

    def with_xer(self, **changes: Any) -> "MachineState":
        return replace(self, xer=replace(self.xer, **changes))

    def with_fpscr(self, value: Any) -> "MachineState":
        return replace(self, fpscr=value)


def _parse(value: object) -> int:
    return int(value, 0) if isinstance(value, str) else int(value)


def _parse_fpr_bits(value: object) -> int:
    """Parse an FPR value that may be an IEEE 754 float or hex bitpattern."""
    import struct
    if isinstance(value, (int, float)):
        if isinstance(value, float):
            return struct.unpack(">Q", struct.pack(">d", value))[0]
        return int(value) & 0xFFFFFFFFFFFFFFFF
    if isinstance(value, str):
        try:
            return int(value, 0) & 0xFFFFFFFFFFFFFFFF
        except ValueError:
            f = float(value)
            return struct.unpack(">Q", struct.pack(">d", f))[0]
    return 0


def _parse_fpscr(value: object, field: str) -> int:
    if isinstance(value, dict):
        return int(bool(value.get(field, 0)))
    if isinstance(value, str):
        reg = int(value, 0) & 0xFFFFFFFF
    else:
        reg = int(value) & 0xFFFFFFFF
    bits = {
        "rn": (reg >> 30) & 3,
        "ni": (reg >> 29) & 1,
        "fx": (reg >> 31) & 1,
        "fex": (reg >> 26) & 1,
        "vx": (reg >> 25) & 1,
        "ox": (reg >> 24) & 1,
        "ux": (reg >> 23) & 1,
        "zx": (reg >> 22) & 1,
        "xx": (reg >> 21) & 1,
        "vxsnan": (reg >> 20) & 1,
        "vxisi": (reg >> 19) & 1,
        "vxidi": (reg >> 18) & 1,
        "vxzdz": (reg >> 17) & 1,
        "vximz": (reg >> 16) & 1,
        "vxvc": (reg >> 15) & 1,
        "fr": (reg >> 14) & 1,
        "fi": (reg >> 13) & 1,
        "fprf": (reg >> 6) & 0x1F,
        "vxsoft": (reg >> 5) & 1,
        "vxcvi": (reg >> 4) & 1,
        "reserved": reg & 0xF,
    }
    return bits.get(field, 0)


def concrete_state(values: dict[str, object] | None = None) -> MachineState:
    values = values or {}
    gpr_values = values.get("gpr", {})
    assert isinstance(gpr_values, dict)
    gpr = tuple(_parse(gpr_values.get(f"r{i}", 0)) & 0xFFFFFFFF for i in range(32))
    fpr_values = values.get("fpr", {})
    assert isinstance(fpr_values, dict)
    fpr = tuple(_parse_fpr_bits(fpr_values.get(f"f{i}", 0)) for i in range(32))
    ps1_values = values.get("ps1", {})
    assert isinstance(ps1_values, dict)
    ps1 = tuple(_parse_fpr_bits(ps1_values.get(f"f{i}", 0)) for i in range(32))
    gqr_values = values.get("gqr", {})
    assert isinstance(gqr_values, dict)
    gqr = tuple(_parse(gqr_values.get(f"gqr{i}", 0)) & 0xFFFFFFFF for i in range(8))
    xer_values = values.get("xer", {})
    assert isinstance(xer_values, dict)
    fpscr = _parse(values.get("fpscr", 0)) & 0xFFFFFFFF
    memory_values = values.get("memory", {})
    assert isinstance(memory_values, dict)
    if "bytes" in memory_values or "default" in memory_values:
        byte_values = memory_values.get("bytes", {})
        assert isinstance(byte_values, dict)
        default = _parse(memory_values.get("default", 0)) & 0xFF
    else:
        byte_values = memory_values
        default = 0
    memory = ConcreteMemory(
        default,
        tuple(sorted((_parse(address) & 0xFFFFFFFF, _parse(byte) & 0xFF) for address, byte in byte_values.items() if (_parse(byte) & 0xFF) != default)),
    )
    return MachineState(
        gpr,
        fpr,
        ps1,
        gqr,
        _parse(values.get("cr", 0)) & 0xFFFFFFFF,
        XerState(
            bool(_parse(xer_values.get("ca", 0))),
            bool(_parse(xer_values.get("ov", 0))),
            bool(_parse(xer_values.get("so", 0))),
        ),
        fpscr,
        _parse(values.get("lr", 0)) & 0xFFFFFFFF,
        _parse(values.get("ctr", 0)) & 0xFFFFFFFF,
        memory,
        True,
    )
