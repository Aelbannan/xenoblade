"""MMIO device models for the opt-in MemoryBus path.

Provides ``DeviceModel`` implementations for register banks and GX FIFO
write-stream recording. Live under Tier **C** when attached via
``build_memory_bus(..., devices=)`` and a matching MMIO ``device_id``:
concrete loads/stores through ``execute_cfg(..., memory_bus=)`` hit the device.
Symbolic ``check_equivalence`` still treats MMIO as fail-closed (no device
semantics in SMT). Unknown widths, alignments, or ``AccessOutcome.UNSUPPORTED``
must fail closed rather than degrade to RAM.
"""

from __future__ import annotations

from abc import ABC, abstractmethod
from dataclasses import dataclass, field
from enum import Enum
from typing import Any

_SUPPORTED_WIDTHS = frozenset({1, 2, 4})


class AccessOutcome(str, Enum):
    OK = "ok"
    UNSUPPORTED = "unsupported"
    MISALIGNED = "misaligned"
    OUT_OF_RANGE = "out-of-range"


@dataclass(frozen=True, slots=True)
class DeviceReadResult:
    outcome: AccessOutcome
    value: int | None = None


@dataclass(frozen=True, slots=True)
class DeviceWriteResult:
    outcome: AccessOutcome


class DeviceModel(ABC):
    """Abstract MMIO device surface serviced by ``MemoryBus`` MMIO regions."""

    @abstractmethod
    def validate_access(self, addr: int, width: int, *, is_write: bool) -> AccessOutcome:
        """Pre-flight check for an access without mutating device state."""

    @abstractmethod
    def read(self, addr: int, width: int) -> DeviceReadResult:
        """Perform a read at ``addr`` with byte ``width``."""

    @abstractmethod
    def write(self, addr: int, width: int, value: int) -> DeviceWriteResult:
        """Perform a write at ``addr`` with byte ``width``."""

    @abstractmethod
    def visible_state(self) -> dict[str, Any]:
        """Return a JSON-serializable snapshot of observable device state."""


@dataclass(frozen=True, slots=True)
class RegisterSpec:
    """Static metadata for one register slot in a ``RegisterBankDevice``."""

    offset: int
    initial: int = 0
    w1c: bool = False
    read_clear: bool = False


def _mask_width(value: int, width: int) -> int:
    return value & ((1 << (8 * width)) - 1)


def _validate_width(width: int) -> AccessOutcome | None:
    if width not in _SUPPORTED_WIDTHS:
        return AccessOutcome.UNSUPPORTED
    return None


@dataclass
class RegisterBankDevice(DeviceModel):
    """Fixed-width register bank with optional W1C and read-clear semantics."""

    base: int
    reg_width: int = 4
    registers: tuple[RegisterSpec, ...] = ()
    _values: dict[int, int] = field(default_factory=dict, repr=False)

    def __post_init__(self) -> None:
        if self.reg_width not in _SUPPORTED_WIDTHS:
            raise ValueError(f"unsupported reg_width {self.reg_width}")
        for spec in self.registers:
            if spec.offset < 0 or spec.offset % self.reg_width != 0:
                raise ValueError(
                    f"register offset {spec.offset:#x} misaligned for width {self.reg_width}"
                )
            if spec.initial != _mask_width(spec.initial, self.reg_width):
                raise ValueError(
                    f"register initial {spec.initial:#x} exceeds width {self.reg_width}"
                )
            self._values.setdefault(spec.offset, spec.initial)

    def _spec_for_offset(self, offset: int) -> RegisterSpec | None:
        for spec in self.registers:
            if spec.offset == offset:
                return spec
        return None

    def _device_offset(self, addr: int) -> int | None:
        if addr < self.base:
            return None
        offset = addr - self.base
        if offset % self.reg_width != 0:
            return None
        return offset

    def validate_access(self, addr: int, width: int, *, is_write: bool) -> AccessOutcome:
        del is_write
        width_err = _validate_width(width)
        if width_err is not None:
            return width_err
        if width != self.reg_width:
            return AccessOutcome.UNSUPPORTED
        offset = self._device_offset(addr)
        if offset is None:
            return AccessOutcome.MISALIGNED if addr >= self.base else AccessOutcome.OUT_OF_RANGE
        if self._spec_for_offset(offset) is None:
            return AccessOutcome.OUT_OF_RANGE
        return AccessOutcome.OK

    def read(self, addr: int, width: int) -> DeviceReadResult:
        outcome = self.validate_access(addr, width, is_write=False)
        if outcome is not AccessOutcome.OK:
            return DeviceReadResult(outcome=outcome)
        offset = self._device_offset(addr)
        assert offset is not None
        spec = self._spec_for_offset(offset)
        assert spec is not None
        value = self._values.get(offset, 0)
        if spec.read_clear:
            self._values[offset] = 0
        return DeviceReadResult(outcome=AccessOutcome.OK, value=value)

    def write(self, addr: int, width: int, value: int) -> DeviceWriteResult:
        outcome = self.validate_access(addr, width, is_write=True)
        if outcome is not AccessOutcome.OK:
            return DeviceWriteResult(outcome=outcome)
        offset = self._device_offset(addr)
        assert offset is not None
        spec = self._spec_for_offset(offset)
        assert spec is not None
        masked = _mask_width(value, self.reg_width)
        current = self._values.get(offset, 0)
        if spec.w1c:
            self._values[offset] = current & ~masked
        else:
            self._values[offset] = masked
        return DeviceWriteResult(outcome=AccessOutcome.OK)

    def visible_state(self) -> dict[str, Any]:
        return {
            "kind": "register-bank",
            "base": hex(self.base),
            "reg_width": self.reg_width,
            "registers": {
                hex(offset): hex(value)
                for offset, value in sorted(self._values.items())
            },
        }


@dataclass
class GxFifoStreamDevice(DeviceModel):
    """Stub GX FIFO device that records write-stream events only."""

    base: int
    span: int = 0x100
    write_events: list[dict[str, Any]] = field(default_factory=list, repr=False)

    def __post_init__(self) -> None:
        if self.span <= 0:
            raise ValueError("span must be positive")

    def _in_span(self, addr: int, width: int) -> bool:
        last = addr + width - 1
        if last < addr:
            return False
        return self.base <= addr and last < self.base + self.span

    def validate_access(self, addr: int, width: int, *, is_write: bool) -> AccessOutcome:
        width_err = _validate_width(width)
        if width_err is not None:
            return width_err
        if not is_write:
            return AccessOutcome.UNSUPPORTED
        if not self._in_span(addr, width):
            return AccessOutcome.OUT_OF_RANGE
        return AccessOutcome.OK

    def read(self, addr: int, width: int) -> DeviceReadResult:
        outcome = self.validate_access(addr, width, is_write=False)
        return DeviceReadResult(outcome=outcome)

    def write(self, addr: int, width: int, value: int) -> DeviceWriteResult:
        outcome = self.validate_access(addr, width, is_write=True)
        if outcome is not AccessOutcome.OK:
            return DeviceWriteResult(outcome=outcome)
        self.write_events.append(
            {
                "addr": hex(addr),
                "width": width,
                "value": hex(_mask_width(value, width)),
            }
        )
        return DeviceWriteResult(outcome=AccessOutcome.OK)

    def visible_state(self) -> dict[str, Any]:
        return {
            "kind": "gxfifo-stream",
            "base": hex(self.base),
            "span": hex(self.span),
            "write_events": list(self.write_events),
        }
