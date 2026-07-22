"""Fail-closed memory bus routing loads/stores through AddressSpace regions.

Routes byte accesses to RAM backing, immutable ROM images, or live MMIO
``DeviceModel`` instances keyed by region ``device_id``. Opt-in Tier C:
pass ``memory_bus=`` to ``execute_cfg`` with ``ConcreteOps`` only, or to
``check_equivalence`` to bind ROM/RAM symbolic constraints and route concrete
sampling (MMIO remains fail-closed in SMT). Default proofs remain on
unconstrained ``ConcreteMemory`` unless callers explicitly opt in.

PR 9: ``MemoryBus`` is a lightweight router over an immutable
``BusSpecification`` plus mutable ``BusState``. Concrete sampling clones
state before original and candidate so device mutations never leak.
"""

from __future__ import annotations

from dataclasses import dataclass, field
from enum import Enum
from pathlib import Path
from typing import Any, Mapping

from tools.ppc_equivalence.address_space import AddressSpace, Region, RegionKind, mmio_region
from tools.ppc_equivalence.bus_spec import (
    BusSpecification,
    BusState,
    build_bus_specification,
    materialize_devices,
    snapshot_bus_state,
)
from tools.ppc_equivalence.device_model import (
    AccessOutcome,
    DeviceModel,
    DeviceReadResult,
    DeviceWriteResult,
    GxFifoStreamDevice,
    RegisterBankDevice,
    RegisterSpec,
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
    """Route concrete-width loads/stores through an ``AddressSpace``.

    Holds an immutable ``specification`` (identity) and mutable ``devices`` /
    ``ram`` (state). Prefer ``snapshot_state`` / ``with_state`` / ``clone`` when
    running isolated original vs candidate sampling.
    """

    address_space: AddressSpace
    devices: dict[str, DeviceModel] = field(default_factory=dict)
    ram: ConcreteMemory = field(default_factory=ConcreteMemory)
    specification: BusSpecification | None = field(default=None, repr=False)
    # Reviewed hardware-profile identity (Stage: GX FIFO Tier-A pre-allowlist
    # wiring). Set only by ``build_memory_bus_from_hardware_profile``; ad-hoc
    # buses built via ``build_memory_bus`` leave these ``None`` so obligations
    # built from them classify as ``SOURCE_AD_HOC_BUS`` (never promotion-grade).
    hardware_profile_name: str | None = None
    hardware_profile_sha256: str | None = None
    device_models_sha256: str | None = None

    def __post_init__(self) -> None:
        if self.specification is None:
            self.specification = build_bus_specification(
                self.address_space,
                self.devices,
            )

    def snapshot_state(self) -> BusState:
        """Capture current RAM + device mutable state."""
        return snapshot_bus_state(self.ram, self.devices)

    def with_state(self, state: BusState) -> MemoryBus:
        """Return a new bus sharing the immutable specification with ``state``."""
        return MemoryBus(
            address_space=self.address_space,
            devices=materialize_devices(self.devices, state),
            ram=state.ram,
            specification=self.specification,
            hardware_profile_name=self.hardware_profile_name,
            hardware_profile_sha256=self.hardware_profile_sha256,
            device_models_sha256=self.device_models_sha256,
        )

    def clone(self) -> MemoryBus:
        """Deep-clone mutable state for an isolated CFG execution."""
        return self.with_state(self.snapshot_state().clone())

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
    device_map = dict(devices or {})
    return MemoryBus(
        address_space=address_space,
        devices=device_map,
        ram=ram if ram is not None else ConcreteMemory(),
        specification=build_bus_specification(address_space, device_map),
    )


def _addr_int(value: Any) -> int:
    if isinstance(value, int):
        return value & 0xFFFFFFFF
    text = str(value).strip()
    if text.lower().startswith("0x"):
        return int(text, 16) & 0xFFFFFFFF
    return int(text, 0) & 0xFFFFFFFF


def _register_specs_for_device(
    device_id: str,
    *,
    register_specs: list[Mapping[str, Any]],
    reset_state: Mapping[str, Any],
) -> tuple[RegisterSpec, ...]:
    """Merge profile ``register_specs`` with per-device ``reset_state`` overrides."""
    by_offset: dict[int, RegisterSpec] = {}
    for spec in register_specs:
        if str(spec.get("device_id")) != device_id:
            continue
        offset = _addr_int(spec["offset"])
        by_offset[offset] = RegisterSpec(
            offset=offset,
            initial=int(spec.get("initial", 0)),
            w1c=bool(spec.get("w1c", False)),
            read_clear=bool(spec.get("read_clear", False)),
        )
    for offset_key, value in dict(reset_state).items():
        try:
            offset = _addr_int(offset_key)
        except (TypeError, ValueError):
            continue
        existing = by_offset.get(offset)
        if existing is not None:
            by_offset[offset] = RegisterSpec(
                offset=offset,
                initial=int(value),
                w1c=existing.w1c,
                read_clear=existing.read_clear,
            )
        else:
            by_offset[offset] = RegisterSpec(offset=offset, initial=int(value))
    return tuple(by_offset[offset] for offset in sorted(by_offset))


def _build_device_from_profile(
    device_id: str,
    device_entry: Mapping[str, Any] | None,
    *,
    register_specs: list[Mapping[str, Any]],
    reset_state: Mapping[str, Any],
) -> DeviceModel:
    """Materialize a ``RegisterBankDevice`` / ``GxFifoStreamDevice`` from profile JSON."""
    if device_entry is None:
        raise ValueError(
            f"hardware profile missing devices[] entry for device_id {device_id!r}"
        )
    theory = str(device_entry["theory"])
    base = _addr_int(device_entry["base"])
    if theory == "mmio-register-bank":
        return RegisterBankDevice(
            base=base,
            reg_width=int(device_entry.get("reg_width", 4)),
            registers=_register_specs_for_device(
                device_id, register_specs=register_specs, reset_state=reset_state,
            ),
        )
    if theory == "gxfifo-stream":
        widths = device_entry.get("supported_widths", [1, 2, 4])
        return GxFifoStreamDevice(
            base=base,
            span=int(device_entry.get("span", 0x100)),
            device_id=device_id,
            endian=str(device_entry.get("endian", "big")),
            supported_widths=frozenset(int(w) for w in widths),
            alignment_required=bool(device_entry.get("alignment_required", True)),
            read_policy=str(device_entry.get("read_policy", "unsupported")),
            read_model_version=str(
                device_entry.get("read_model_version", "gx-fifo-read-v1")
            ),
            write_event_semantics=str(
                device_entry.get("write_event_semantics", "gx-fifo-trace-v2")
            ),
            read_side_effects=bool(device_entry.get("read_side_effects", False)),
            external_input=bool(device_entry.get("external_input", False)),
            max_fifo_events=int(device_entry.get("max_fifo_events", 256)),
        )
    raise ValueError(
        f"unsupported hardware profile device theory {theory!r} for device_id {device_id!r}"
    )


def build_memory_bus_from_hardware_profile(
    name_or_profile: str | Path | Mapping[str, Any],
    *,
    profiles_dir: Path | None = None,
) -> MemoryBus:
    """Materialize a ``MemoryBus`` from a reviewed hardware profile.

    Builds the ``AddressSpace`` from ``profile["regions"]`` (``ram`` + ``mmio``)
    and ``RegisterBankDevice`` / ``GxFifoStreamDevice`` instances from
    ``profile["devices"]`` (merged with ``register_specs`` / ``reset_state``).
    Binds ``hardware_profile_name`` / ``hardware_profile_sha256`` /
    ``device_models_sha256`` onto the returned bus so
    ``build_memory_bus_obligation`` (and, downstream, capability attachment)
    can see the reviewed-profile source without a separate wrapper.

    Fail-closed: an invalid/unreviewable profile raises rather than degrading
    to an unconstrained or ad-hoc bus. Callers that want fail-closed ``None``
    behavior (e.g. coop config wiring) must catch ``OSError`` / ``ValueError``
    themselves — this helper never swallows errors silently.
    """
    from tools.ppc_equivalence.hardware_profile import (
        compute_hardware_profile_sha256,
        device_models_sha256 as compute_device_models_sha256,
        load_hardware_profile,
    )

    if isinstance(name_or_profile, (str, Path)):
        profile = load_hardware_profile(name_or_profile, profiles_dir=profiles_dir)
    elif isinstance(name_or_profile, Mapping):
        profile = dict(name_or_profile)
        expected = compute_hardware_profile_sha256(profile)
        declared = profile.get("profile_sha256")
        if declared is not None and declared != expected:
            raise ValueError(
                "hardware profile profile_sha256 mismatch "
                f"(declared={declared}, recomputed={expected})"
            )
        profile.setdefault("profile_sha256", expected)
        if not profile.get("profile") and not profile.get("platform_profile"):
            raise ValueError("hardware profile missing profile name")
        profile.setdefault("profile", profile.get("profile") or profile.get("platform_profile"))
    else:
        raise TypeError(
            "name_or_profile must be a profile name/path or a loaded mapping"
        )

    profile_name = str(profile.get("profile") or profile.get("platform_profile"))
    profile_sha256 = str(profile["profile_sha256"])

    device_entries: dict[str, Mapping[str, Any]] = {
        str(entry["device_id"]): entry for entry in profile.get("devices", [])
    }
    register_specs = list(profile.get("register_specs", []))
    reset_state_all = profile.get("reset_state", {})

    regions: list[Region] = []
    devices: dict[str, DeviceModel] = {}
    for region_entry in profile.get("regions", []):
        kind = str(region_entry["kind"])
        start = _addr_int(region_entry["start"])
        end = _addr_int(region_entry["end"])
        label = region_entry.get("label")
        if kind == "ram":
            regions.append(Region(start=start, end=end, kind=RegionKind.RAM, label=label))
        elif kind == "mmio":
            device_id = region_entry.get("device_id")
            if not isinstance(device_id, str) or not device_id:
                raise ValueError(
                    f"hardware profile mmio region [{start:#x}, {end:#x}] missing device_id"
                )
            regions.append(mmio_region(start, end, device_id=device_id, label=label))
            if device_id not in devices:
                devices[device_id] = _build_device_from_profile(
                    device_id,
                    device_entries.get(device_id),
                    register_specs=register_specs,
                    reset_state=reset_state_all.get(device_id, {}),
                )
        else:
            raise ValueError(f"unsupported hardware profile region kind {kind!r}")

    address_space = AddressSpace(tuple(regions))
    bus = build_memory_bus(address_space, devices)
    bus.hardware_profile_name = profile_name
    bus.hardware_profile_sha256 = profile_sha256
    bus.device_models_sha256 = compute_device_models_sha256(profile)
    return bus
