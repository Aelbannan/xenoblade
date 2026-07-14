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
    cr: Any
    xer: XerState
    lr: Any
    ctr: Any
    memory: Any
    valid: Any
    memory_touches: tuple[Any, ...] = ()

    def with_gpr(self, index: int, value: Any) -> "MachineState":
        registers = list(self.gpr)
        registers[index] = value
        return replace(self, gpr=tuple(registers))

    def with_cr(self, value: Any) -> "MachineState":
        return replace(self, cr=value)

    def with_xer(self, **changes: Any) -> "MachineState":
        return replace(self, xer=replace(self.xer, **changes))


def _parse(value: object) -> int:
    return int(value, 0) if isinstance(value, str) else int(value)


def concrete_state(values: dict[str, object] | None = None) -> MachineState:
    values = values or {}
    gpr_values = values.get("gpr", {})
    assert isinstance(gpr_values, dict)
    gpr = tuple(_parse(gpr_values.get(f"r{i}", 0)) & 0xFFFFFFFF for i in range(32))
    xer_values = values.get("xer", {})
    assert isinstance(xer_values, dict)
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
        _parse(values.get("cr", 0)) & 0xFFFFFFFF,
        XerState(
            bool(_parse(xer_values.get("ca", 0))),
            bool(_parse(xer_values.get("ov", 0))),
            bool(_parse(xer_values.get("so", 0))),
        ),
        _parse(values.get("lr", 0)) & 0xFFFFFFFF,
        _parse(values.get("ctr", 0)) & 0xFFFFFFFF,
        memory,
        True,
    )
