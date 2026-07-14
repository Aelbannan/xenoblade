from __future__ import annotations

from dataclasses import dataclass, replace
from typing import Any


@dataclass(frozen=True, slots=True)
class XerState:
    ca: Any
    ov: Any
    so: Any


@dataclass(frozen=True, slots=True)
class MachineState:
    gpr: tuple[Any, ...]
    cr: Any
    xer: XerState
    lr: Any
    ctr: Any

    def with_gpr(self, index: int, value: Any) -> "MachineState":
        registers = list(self.gpr)
        registers[index] = value
        return replace(self, gpr=tuple(registers))

    def with_cr(self, value: Any) -> "MachineState":
        return replace(self, cr=value)


def concrete_state(values: dict[str, object] | None = None) -> MachineState:
    values = values or {}
    gpr_values = values.get("gpr", {})
    assert isinstance(gpr_values, dict)
    gpr = tuple(int(gpr_values.get(f"r{i}", 0), 0) & 0xFFFFFFFF if isinstance(gpr_values.get(f"r{i}", 0), str) else int(gpr_values.get(f"r{i}", 0)) & 0xFFFFFFFF for i in range(32))
    xer_values = values.get("xer", {})
    assert isinstance(xer_values, dict)
    cr_value = values.get("cr", 0)
    lr_value = values.get("lr", 0)
    ctr_value = values.get("ctr", 0)
    parse = lambda value: int(value, 0) if isinstance(value, str) else int(value)
    return MachineState(
        gpr,
        parse(cr_value) & 0xFFFFFFFF,
        XerState(bool(parse(xer_values.get("ca", 0))), bool(parse(xer_values.get("ov", 0))), bool(parse(xer_values.get("so", 0)))),
        parse(lr_value) & 0xFFFFFFFF,
        parse(ctr_value) & 0xFFFFFFFF,
    )
