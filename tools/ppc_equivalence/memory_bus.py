"""Fail-closed memory bus routing loads/stores through AddressSpace regions.

Routes byte accesses to RAM backing, immutable ROM images, or live MMIO
``DeviceModel`` instances keyed by region ``device_id``. Opt-in Tier C:
pass ``memory_bus=`` to ``execute_cfg`` with ``ConcreteOps`` only, or to
``check_equivalence`` to bind ROM/RAM symbolic constraints and route concrete
sampling (MMIO remains fail-closed in SMT). Default proofs remain on
unconstrained ``ConcreteMemory`` unless callers explicitly opt in.
"""

from __future__ import annotations

from dataclasses import dataclass, field
from enum import Enum

from tools.ppc_equivalence.address_space import AddressSpace, Region, RegionKind
from tools.ppc_equivalence.device_model import (
    AccessOutcome,
    DeviceModel,
    DeviceReadResult,
    DeviceWriteResult,
)
from tools.ppc_equivalence.model import ConcreteMemory

_SUPPORTED_WIDTHS = frozenset({1, 2, 4})


class BusOutcome(str, Enum):
    OK = "ok"
    UNSUPPORTED = "unsupported"
    MISALIGNED = "misaligned"
    OUT_OF_RANGE = "out-of-range"
    UNMAPPED = "unmapped"
    SPAN_MULTIPLE_REGIONS = "span-multiple-regions"
    READ_ONLY = "read-only"


@dataclass(frozen=True, slots=True)
class BusReadResult:
    outcome: BusOutcome
    value: int | None = None


@dataclass(frozen=True, slots=True)
class BusWriteResult:
    outcome: BusOutcome


def _validate_width(width: int) -> BusOutcome | None:
    if width not in _SUPPORTED_WIDTHS:
        return BusOutcome.UNSUPPORTED
    return None


def _validate_alignment(addr: int, width: int) -> BusOutcome | None:
    if width == 1:
        return None
    if addr % width != 0:
        return BusOutcome.MISALIGNED
    return None


def _read_be_bytes(data: bytes, offset: int, width: int) -> int:
    value = 0
    for index in range(width):
        value = (value << 8) | data[offset + index]
    return value


def _load_from_memory(memory: ConcreteMemory, addr: int, width: int) -> int:
    value = 0
    for offset in range(width):
        value = (value << 8) | (memory.read(addr + offset) & 0xFF)
    return value


def _store_to_memory(memory: ConcreteMemory, addr: int, width: int, value: int) -> ConcreteMemory:
    result = memory
    for offset in range(width):
        shift = (width - 1 - offset) * 8
        byte = (value >> shift) & 0xFF
        result = result.write(addr + offset, byte)
    return result


def _device_outcome_to_bus(outcome: AccessOutcome) -> BusOutcome:
    mapping = {
        AccessOutcome.OK: BusOutcome.OK,
        AccessOutcome.UNSUPPORTED: BusOutcome.UNSUPPORTED,
        AccessOutcome.MISALIGNED: BusOutcome.MISALIGNED,
        AccessOutcome.OUT_OF_RANGE: BusOutcome.OUT_OF_RANGE,
    }
    return mapping[outcome]


@dataclass
class MemoryBus:
    """Route concrete-width loads/stores through an ``AddressSpace``."""

    address_space: AddressSpace
    devices: dict[str, DeviceModel] = field(default_factory=dict)
    ram: ConcreteMemory = field(default_factory=ConcreteMemory)

    def load(self, addr: int, width: int) -> BusReadResult:
        width_err = _validate_width(width)
        if width_err is not None:
            return BusReadResult(outcome=width_err)

        classification = self.address_space.classify_range(addr, width)
        if classification.spans_multiple_regions:
            return BusReadResult(outcome=BusOutcome.SPAN_MULTIPLE_REGIONS)
        region = classification.region
        if region is None:
            return BusReadResult(outcome=BusOutcome.UNMAPPED)

        align_err = _validate_alignment(addr, width)
        if align_err is not None:
            return BusReadResult(outcome=align_err)

        return self._load_region(region, addr, width)

    def store(self, addr: int, width: int, value: int) -> BusWriteResult:
        width_err = _validate_width(width)
        if width_err is not None:
            return BusWriteResult(outcome=width_err)

        classification = self.address_space.classify_range(addr, width)
        if classification.spans_multiple_regions:
            return BusWriteResult(outcome=BusOutcome.SPAN_MULTIPLE_REGIONS)
        region = classification.region
        if region is None:
            return BusWriteResult(outcome=BusOutcome.UNMAPPED)

        align_err = _validate_alignment(addr, width)
        if align_err is not None:
            return BusWriteResult(outcome=align_err)

        return self._store_region(region, addr, width, value)

    def _load_region(self, region: Region, addr: int, width: int) -> BusReadResult:
        if region.kind is RegionKind.UNMAPPED:
            return BusReadResult(outcome=BusOutcome.UNMAPPED)

        if region.kind is RegionKind.RAM:
            return BusReadResult(
                outcome=BusOutcome.OK,
                value=_load_from_memory(self.ram, addr, width),
            )

        if region.kind is RegionKind.ROM_IMAGE:
            assert region.image_bytes is not None
            offset = addr - region.start
            last = offset + width - 1
            if offset < 0 or last >= len(region.image_bytes):
                return BusReadResult(outcome=BusOutcome.OUT_OF_RANGE)
            return BusReadResult(
                outcome=BusOutcome.OK,
                value=_read_be_bytes(region.image_bytes, offset, width),
            )

        if region.kind is RegionKind.MMIO:
            return self._load_mmio(region, addr, width)

        return BusReadResult(outcome=BusOutcome.UNSUPPORTED)

    def _store_region(self, region: Region, addr: int, width: int, value: int) -> BusWriteResult:
        if region.kind is RegionKind.UNMAPPED:
            return BusWriteResult(outcome=BusOutcome.UNMAPPED)

        if region.kind is RegionKind.RAM:
            self.ram = _store_to_memory(self.ram, addr, width, value)
            return BusWriteResult(outcome=BusOutcome.OK)

        if region.kind is RegionKind.ROM_IMAGE:
            return BusWriteResult(outcome=BusOutcome.READ_ONLY)

        if region.kind is RegionKind.MMIO:
            return self._store_mmio(region, addr, width, value)

        return BusWriteResult(outcome=BusOutcome.UNSUPPORTED)

    def _resolve_device(self, region: Region) -> DeviceModel | None:
        device_id = region.device_id
        if device_id is None:
            return None
        return self.devices.get(device_id)

    def _load_mmio(self, region: Region, addr: int, width: int) -> BusReadResult:
        device = self._resolve_device(region)
        if device is None:
            return BusReadResult(outcome=BusOutcome.UNSUPPORTED)
        result: DeviceReadResult = device.read(addr, width)
        bus_outcome = _device_outcome_to_bus(result.outcome)
        if bus_outcome is not BusOutcome.OK:
            return BusReadResult(outcome=bus_outcome)
        return BusReadResult(outcome=BusOutcome.OK, value=result.value)

    def _store_mmio(self, region: Region, addr: int, width: int, value: int) -> BusWriteResult:
        device = self._resolve_device(region)
        if device is None:
            return BusWriteResult(outcome=BusOutcome.UNSUPPORTED)
        result: DeviceWriteResult = device.write(addr, width, value)
        return BusWriteResult(outcome=_device_outcome_to_bus(result.outcome))


def build_memory_bus(
    address_space: AddressSpace,
    devices: dict[str, DeviceModel] | None = None,
    *,
    ram: ConcreteMemory | None = None,
) -> MemoryBus:
    """Construct a ``MemoryBus`` from an address space and optional device map."""
    return MemoryBus(
        address_space=address_space,
        devices=dict(devices or {}),
        ram=ram if ram is not None else ConcreteMemory(),
    )
