"""Immutable bus specification and isolated mutable bus state.

PR 9 scaffolding (Track D): separate the static bus identity (regions + device
theories + register flags) from mutable concrete state (RAM + device values +
event logs). Concrete sampling must clone ``BusState`` before original and
candidate execution so MMIO side effects never leak across sides.

Symbolic register-bank theory (toward PR 14) lives in ``symbolic_bus`` and
lifts from ``DeviceSpecification`` via ``symbolic_bank_from_device_spec``.
It is not bound into ``check_equivalence`` yet; ``memory-bus`` stays
unsupported for ``EQUIVALENT``.
"""

from __future__ import annotations

from dataclasses import dataclass, field
from typing import Any

from tools.ppc_equivalence.address_space import AddressSpace, Region, RegionKind
from tools.ppc_equivalence.device_model import (
    DeviceModel,
    GxFifoStreamDevice,
    RegisterBankDevice,
    RegisterSpec,
)
from tools.ppc_equivalence.model import ConcreteMemory
from tools.ppc_equivalence.provenance import canonical_json_sha256

# Re-export for callers that import identity types from this module.
__all__ = [
    "RegisterSpec",
    "DeviceSpecification",
    "BusSpecification",
    "BusState",
    "device_specification_from_model",
    "build_bus_specification",
    "snapshot_bus_state",
    "materialize_devices",
    "lift_symbolic_register_banks",
]


@dataclass(frozen=True, slots=True)
class DeviceSpecification:
    """Static identity of one MMIO device attached to the bus."""

    device_id: str
    theory: str
    base: int
    registers: tuple[RegisterSpec, ...] = ()
    reg_width: int = 4
    span: int = 0
    schema_version: int = 1

    def canonical_dict(self) -> dict[str, Any]:
        registers = [
            {
                "offset": spec.offset,
                "initial": spec.initial,
                "w1c": bool(spec.w1c),
                "read_clear": bool(spec.read_clear),
            }
            for spec in self.registers
        ]
        payload: dict[str, Any] = {
            "base": self.base,
            "device_id": self.device_id,
            "registers": registers,
            "reg_width": self.reg_width,
            "schema_version": self.schema_version,
            "span": self.span,
            "theory": self.theory,
        }
        return payload


@dataclass(frozen=True, slots=True)
class BusSpecification:
    """Immutable address-space + device-theory identity for a memory bus."""

    address_space: AddressSpace
    devices: tuple[DeviceSpecification, ...] = ()

    def canonical_dict(self) -> dict[str, Any]:
        return {
            "devices": [device.canonical_dict() for device in self.devices],
            "regions": [_region_canonical(region) for region in self.address_space.regions],
            "schema_version": 1,
        }

    def sha256(self) -> str:
        return canonical_json_sha256(self.canonical_dict())


@dataclass
class BusState:
    """Mutable concrete bus state isolated per CFG execution."""

    ram: ConcreteMemory = field(default_factory=ConcreteMemory)
    device_values: dict[str, dict[int, int]] = field(default_factory=dict)
    event_logs: dict[str, list[dict[str, Any]]] = field(default_factory=dict)

    def clone(self) -> BusState:
        return BusState(
            ram=self.ram,
            device_values={
                device_id: dict(values)
                for device_id, values in self.device_values.items()
            },
            event_logs={
                device_id: [dict(event) for event in events]
                for device_id, events in self.event_logs.items()
            },
        )


def _region_canonical(region: Region) -> dict[str, Any]:
    entry: dict[str, Any] = {
        "end": region.end,
        "kind": region.kind.value,
        "start": region.start,
    }
    if region.label is not None:
        entry["label"] = region.label
    if region.kind is RegionKind.ROM_IMAGE and region.image_sha256 is not None:
        entry["image_sha256"] = region.image_sha256
    if region.kind is RegionKind.MMIO and region.device_id is not None:
        entry["device_id"] = region.device_id
    return entry


def device_specification_from_model(
    device_id: str,
    device: DeviceModel,
) -> DeviceSpecification:
    """Lift a live ``DeviceModel`` into an immutable device specification."""
    if isinstance(device, RegisterBankDevice):
        return DeviceSpecification(
            device_id=device_id,
            theory="register-bank",
            base=device.base,
            registers=tuple(device.registers),
            reg_width=device.reg_width,
        )
    if isinstance(device, GxFifoStreamDevice):
        return DeviceSpecification(
            device_id=device_id,
            theory="gxfifo-stream",
            base=device.base,
            span=device.span,
        )
    raise TypeError(f"unsupported device model for specification: {type(device)!r}")


def build_bus_specification(
    address_space: AddressSpace,
    devices: dict[str, DeviceModel] | None = None,
) -> BusSpecification:
    """Build an immutable bus specification from an address space and devices."""
    device_map = devices or {}
    specs = tuple(
        device_specification_from_model(device_id, device)
        for device_id, device in sorted(device_map.items())
    )
    return BusSpecification(address_space=address_space, devices=specs)


def snapshot_bus_state(
    ram: ConcreteMemory,
    devices: dict[str, DeviceModel],
) -> BusState:
    """Capture mutable RAM + device values + event logs into a ``BusState``."""
    device_values: dict[str, dict[int, int]] = {}
    event_logs: dict[str, list[dict[str, Any]]] = {}
    for device_id, device in devices.items():
        if isinstance(device, RegisterBankDevice):
            device_values[device_id] = dict(device._values)
        elif isinstance(device, GxFifoStreamDevice):
            event_logs[device_id] = [dict(event) for event in device.write_events]
    return BusState(ram=ram, device_values=device_values, event_logs=event_logs)


def materialize_devices(
    devices: dict[str, DeviceModel],
    state: BusState,
) -> dict[str, DeviceModel]:
    """Clone devices and apply ``state`` device values / event logs."""
    materialized: dict[str, DeviceModel] = {}
    for device_id, device in devices.items():
        cloned = device.clone()
        if isinstance(cloned, RegisterBankDevice):
            values = state.device_values.get(device_id)
            if values is not None:
                cloned._values = dict(values)
        elif isinstance(cloned, GxFifoStreamDevice):
            events = state.event_logs.get(device_id)
            if events is not None:
                cloned.write_events = [dict(event) for event in events]
        materialized[device_id] = cloned
    return materialized


def lift_symbolic_register_banks(
    specification: BusSpecification,
    z3: Any,
    *,
    prefix: str = "mmio",
) -> dict[str, Any]:
    """Hook: lift register-bank ``DeviceSpecification`` entries to symbolic state.

    Does not alter concrete ``MemoryBus`` routing or equivalence constraints.
    Returns ``device_id → SymbolicRegisterBankState`` for register-bank
    theories only; other theories are skipped.
    """
    # Local import keeps concrete bus tests free of z3/symbolic_bus coupling.
    from tools.ppc_equivalence.symbolic_bus import symbolic_bank_from_device_spec

    banks: dict[str, Any] = {}
    for device in specification.devices:
        if device.theory != "register-bank":
            continue
        banks[device.device_id] = symbolic_bank_from_device_spec(
            device,
            z3,
            prefix=f"{prefix}.{device.device_id}",
        )
    return banks
