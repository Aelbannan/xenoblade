"""Extensional symbolic register-bank theory (Track C / PR 14).

Models a finite MMIO register bank as per-register bitvectors with nested
``addr == base + offset`` routing. Reachable ``Not(supported)`` is a *separate*
unsupported-access obligation (query ``path ∧ ¬supported``); it must **not** be
assumed into the equivalence query.

CFG routing: ``SymbolicBusState`` + ``apply_symbolic_bus_access`` hook into
``SymbolicOps`` loads/stores when ``memory_bus=`` is active. Final device
compare via ``symbolic_bus_difference``. Obligation/evidence path lives in
``memory_bus_obligations``. ``memory-bus`` authorizes ``EQUIVALENT`` only with
engine-generated ``status=discharged`` (Track D).
Concrete ``MemoryBus`` routing is unchanged.
"""

from __future__ import annotations

from dataclasses import dataclass
from enum import Enum
from typing import Any, Literal, Mapping, Sequence

from tools.ppc_equivalence.address_space import AddressSpace, Region, RegionKind
from tools.ppc_equivalence.bus_spec import DeviceSpecification
from tools.ppc_equivalence.deadline import Deadline
from tools.ppc_equivalence.device_model import RegisterSpec
from tools.ppc_equivalence.discharge import discharge_bad_conditions
from tools.ppc_equivalence.provenance import canonical_json_sha256

ALGORITHM = "register-bank-extensional-v1"
THEORY = "register-bank-v1"

__all__ = [
    "ALGORITHM",
    "THEORY",
    "RegisterMasks",
    "SymbolicRegisterBankState",
    "SymbolicAccessResult",
    "UnsupportedAccessStatus",
    "UnsupportedAccessQuery",
    "SymbolicBusState",
    "SymbolicBusAccessOutcome",
    "register_masks",
    "width_mask",
    "initial_symbolic_register_bank",
    "symbolic_bank_from_device_spec",
    "access_supported",
    "register_match_predicates",
    "normal_write_next",
    "w1c_write_next",
    "read_returned_value",
    "read_clear_next",
    "apply_register_write",
    "symbolic_read",
    "symbolic_write",
    "query_unsupported_access",
    "discharge_unsupported_access",
    "MmioTouchEvidence",
    "RegisterBankObservability",
    "route_symbolic_mmio_access",
    "collect_mmio_touches_from_terminals",
    "register_bank_observability_from_state",
    "build_register_bank_extensional_obligation",
    "initial_symbolic_bus_state",
    "apply_symbolic_bus_access",
    "symbolic_bus_difference",
    "discharge_cfg_unsupported_accesses",
    "aggregate_unsupported_access_query_sha256",
    "cfg_trace_sha256",
    "vacuous_unsupported_access_block",
    "concrete_u32",
]


@dataclass(frozen=True, slots=True)
class RegisterMasks:
    """Bitmasks derived from immutable ``RegisterSpec`` metadata."""

    writable: int
    w1c: int
    readable: int
    read_clear: int


@dataclass(frozen=True, slots=True)
class SymbolicRegisterBankState:
    """Per-register bitvector state for one register-bank device."""

    values: Mapping[int, Any]
    base: int
    reg_width: int
    registers: tuple[RegisterSpec, ...]
    device_id: str = ""

    def value_at(self, offset: int) -> Any:
        try:
            return self.values[offset]
        except KeyError as exc:
            raise KeyError(f"no symbolic register at offset {offset:#x}") from exc

    def with_values(self, values: Mapping[int, Any]) -> SymbolicRegisterBankState:
        return SymbolicRegisterBankState(
            values=dict(values),
            base=self.base,
            reg_width=self.reg_width,
            registers=self.registers,
            device_id=self.device_id,
        )


@dataclass(frozen=True, slots=True)
class SymbolicAccessResult:
    """Result of a symbolic register-bank load or store."""

    value: Any
    supported: Any
    next_state: SymbolicRegisterBankState
    reason: str | None = None


class UnsupportedAccessStatus(str, Enum):
    """Outcome of the separate unsupported-access obligation query."""

    UNSAT = "unsat"
    SAT = "sat"
    UNKNOWN = "unknown"


@dataclass(frozen=True, slots=True)
class UnsupportedAccessQuery:
    """Result of querying ``path_condition ∧ ¬supported`` (never assumed)."""

    status: UnsupportedAccessStatus
    query_sha256: str
    inconclusive: bool
    elapsed_ms: float = 0.0

    @property
    def result(self) -> str:
        return self.status.value


def width_mask(width: int) -> int:
    if width not in (1, 2, 4):
        raise ValueError(f"unsupported width {width}")
    return (1 << (8 * width)) - 1


def register_masks(spec: RegisterSpec, reg_width: int) -> RegisterMasks:
    """Derive write/read masks from ``RegisterSpec`` flags and width."""
    full = width_mask(reg_width)
    read_clear = full if spec.read_clear else 0
    if spec.w1c:
        return RegisterMasks(writable=0, w1c=full, readable=full, read_clear=read_clear)
    return RegisterMasks(
        writable=full,
        w1c=0,
        readable=full,
        read_clear=read_clear,
    )


def _bv(z3: Any, value: int, width_bits: int) -> Any:
    return z3.BitVecVal(value & ((1 << width_bits) - 1), width_bits)


def initial_symbolic_register_bank(
    *,
    base: int,
    registers: tuple[RegisterSpec, ...],
    reg_width: int,
    z3: Any,
    prefix: str,
    device_id: str = "",
) -> SymbolicRegisterBankState:
    """Allocate a fresh symbolic bitvector per declared register."""
    bits = 8 * reg_width
    values: dict[int, Any] = {}
    for spec in registers:
        name = f"{prefix}.reg.{spec.offset:x}"
        values[spec.offset] = z3.BitVec(name, bits)
    return SymbolicRegisterBankState(
        values=values,
        base=base,
        reg_width=reg_width,
        registers=registers,
        device_id=device_id,
    )


def symbolic_bank_from_device_spec(
    device: DeviceSpecification,
    z3: Any,
    *,
    prefix: str | None = None,
) -> SymbolicRegisterBankState:
    """Lift an immutable ``DeviceSpecification`` into symbolic bank state."""
    if device.theory != "register-bank":
        raise TypeError(
            f"symbolic register bank requires theory register-bank, got {device.theory!r}"
        )
    label = prefix if prefix is not None else f"mmio.{device.device_id}"
    return initial_symbolic_register_bank(
        base=device.base,
        registers=device.registers,
        reg_width=device.reg_width,
        z3=z3,
        prefix=label,
        device_id=device.device_id,
    )


def register_match_predicates(addr: Any, bank: SymbolicRegisterBankState, z3: Any) -> dict[int, Any]:
    """Map register offset → ``addr == base + offset``."""
    predicates: dict[int, Any] = {}
    for spec in bank.registers:
        target = _bv(z3, (bank.base + spec.offset) & 0xFFFFFFFF, 32)
        predicates[spec.offset] = addr == target
    return predicates


def access_supported(
    addr: Any,
    bank: SymbolicRegisterBankState,
    z3: Any,
    *,
    width: int,
) -> Any:
    """Boolean: access hits a declared register at the bank's native width."""
    if width != bank.reg_width:
        return z3.BoolVal(False)
    matches = list(register_match_predicates(addr, bank, z3).values())
    if not matches:
        return z3.BoolVal(False)
    return z3.Or(*matches) if len(matches) > 1 else matches[0]


def normal_write_next(old: Any, value: Any, writable_mask: int, z3: Any, *, bits: int) -> Any:
    """``next = (old & ~writable_mask) | (value & writable_mask)``."""
    mask = _bv(z3, writable_mask, bits)
    return (old & ~mask) | (value & mask)


def w1c_write_next(old: Any, value: Any, w1c_mask: int, z3: Any, *, bits: int) -> Any:
    """``next = old & ~(value & w1c_mask)``."""
    mask = _bv(z3, w1c_mask, bits)
    return old & ~(value & mask)


def read_returned_value(old: Any, readable_mask: int, z3: Any, *, bits: int) -> Any:
    """``returned = old & readable_mask``."""
    return old & _bv(z3, readable_mask, bits)


def read_clear_next(old: Any, read_clear_mask: int, z3: Any, *, bits: int) -> Any:
    """``next = old & ~read_clear_mask``."""
    return old & ~_bv(z3, read_clear_mask, bits)


def apply_register_write(
    old: Any,
    value: Any,
    spec: RegisterSpec,
    reg_width: int,
    z3: Any,
) -> Any:
    """Apply normal or W1C write formula from ``RegisterSpec`` metadata."""
    bits = 8 * reg_width
    masks = register_masks(spec, reg_width)
    masked_value = value & _bv(z3, width_mask(reg_width), bits)
    if spec.w1c:
        return w1c_write_next(old, masked_value, masks.w1c, z3, bits=bits)
    return normal_write_next(old, masked_value, masks.writable, z3, bits=bits)


def _nested_ite(z3: Any, cases: list[tuple[Any, Any]], default: Any) -> Any:
    """Build nested ``If`` from first case to last (priority order)."""
    expr = default
    for predicate, then_value in reversed(cases):
        expr = z3.If(predicate, then_value, expr)
    return expr


def symbolic_read(
    addr: Any,
    width: int,
    bank: SymbolicRegisterBankState,
    z3: Any,
) -> SymbolicAccessResult:
    """Route a symbolic load; update state on read-clear hits."""
    bits = 8 * bank.reg_width
    supported = access_supported(addr, bank, z3, width=width)
    if width != bank.reg_width or not bank.registers:
        return SymbolicAccessResult(
            value=_bv(z3, 0, bits),
            supported=supported,
            next_state=bank,
            reason="unsupported-width-or-empty",
        )

    matches = register_match_predicates(addr, bank, z3)
    value_cases: list[tuple[Any, Any]] = []
    next_values = dict(bank.values)
    for spec in bank.registers:
        predicate = matches[spec.offset]
        old = bank.value_at(spec.offset)
        masks = register_masks(spec, bank.reg_width)
        returned = read_returned_value(old, masks.readable, z3, bits=bits)
        value_cases.append((predicate, returned))
        if masks.read_clear:
            cleared = read_clear_next(old, masks.read_clear, z3, bits=bits)
            next_values[spec.offset] = z3.If(predicate, cleared, old)
        else:
            next_values[spec.offset] = old

    value = _nested_ite(z3, value_cases, _bv(z3, 0, bits))
    return SymbolicAccessResult(
        value=value,
        supported=supported,
        next_state=bank.with_values(next_values),
    )


def symbolic_write(
    addr: Any,
    width: int,
    value: Any,
    bank: SymbolicRegisterBankState,
    z3: Any,
) -> SymbolicAccessResult:
    """Route a symbolic store with normal / W1C write formulas."""
    bits = 8 * bank.reg_width
    supported = access_supported(addr, bank, z3, width=width)
    if width != bank.reg_width or not bank.registers:
        return SymbolicAccessResult(
            value=value,
            supported=supported,
            next_state=bank,
            reason="unsupported-width-or-empty",
        )

    matches = register_match_predicates(addr, bank, z3)
    next_values = dict(bank.values)
    for spec in bank.registers:
        predicate = matches[spec.offset]
        old = bank.value_at(spec.offset)
        written = apply_register_write(old, value, spec, bank.reg_width, z3)
        next_values[spec.offset] = z3.If(predicate, written, old)

    return SymbolicAccessResult(
        value=value,
        supported=supported,
        next_state=bank.with_values(next_values),
    )


@dataclass(frozen=True, slots=True)
class MmioTouchEvidence:
    """One MMIO touch extracted from a terminal path (obligation/evidence)."""

    device_id: str
    theory: str
    access: Literal["read", "write"]
    width: int
    addr: Any
    register_offset: int | None = None


@dataclass(frozen=True, slots=True)
class RegisterBankObservability:
    """Observable register-bank snapshot for one device."""

    device_id: str
    base: int
    reg_width: int
    registers: dict[str, str]
    touches: tuple[MmioTouchEvidence, ...] = ()


def register_bank_observability_from_state(
    *,
    device_id: str,
    bank: SymbolicRegisterBankState,
    touches: Sequence[MmioTouchEvidence] = (),
) -> RegisterBankObservability:
    """Serialize symbolic or concrete register values for obligation evidence."""
    registers = {
        hex(offset): str(bank.values[offset])
        for offset in sorted(bank.values)
    }
    return RegisterBankObservability(
        device_id=device_id,
        base=bank.base,
        reg_width=bank.reg_width,
        registers=registers,
        touches=tuple(touches),
    )


def _mmio_region_for_addr(
    address_space: AddressSpace,
    addr: int,
    width: int,
) -> tuple[Any, int | None]:
    """Return ``(region, concrete_addr)`` when ``addr`` is a concrete u32."""
    addr &= 0xFFFFFFFF
    classification = address_space.classify_range(addr, width)
    if classification.spans_multiple_regions or classification.region is None:
        return None, addr
    region = classification.region
    if region.kind is not RegionKind.MMIO or region.device_id is None:
        return None, addr
    return region, addr


def collect_mmio_touches_from_terminals(
    terminals: Sequence[Any],
    address_space: AddressSpace,
    *,
    side: str,
    device_theories: Mapping[str, str] | None = None,
    device_widths: Mapping[str, int] | None = None,
) -> tuple[MmioTouchEvidence, ...]:
    """Collect MMIO ``memory_touches`` / ``memory_writes`` from CFG terminals."""
    theories = device_theories or {}
    widths = device_widths or {}
    seen: set[tuple[str, str, int, int]] = set()
    evidence: list[MmioTouchEvidence] = []

    for terminal in terminals:
        state = terminal.state
        read_addrs = set(getattr(state, "memory_reads", ()))
        for addr in state.memory_touches:
            addr_int: int | None
            try:
                if hasattr(addr, "as_long"):
                    addr_int = int(addr.as_long()) & 0xFFFFFFFF
                elif isinstance(addr, int):
                    addr_int = addr & 0xFFFFFFFF
                else:
                    continue
            except Exception:
                continue

            region, concrete_addr = _mmio_region_for_addr(address_space, addr_int, 1)
            if region is None or concrete_addr is None:
                continue
            device_id = region.device_id
            assert device_id is not None
            access: Literal["read", "write"] = (
                "read" if addr in read_addrs else "write"
            )
            access_width = max(1, int(widths.get(device_id, 4)))
            # Prefer word-aligned base for register-bank native width.
            if access_width > 1:
                concrete_addr = concrete_addr - (concrete_addr % access_width)
            key = (device_id, access, concrete_addr, access_width)
            if key in seen:
                continue
            seen.add(key)

            theory = theories.get(device_id, "mmio")
            register_offset: int | None = None
            if theory == "register-bank":
                register_offset = concrete_addr - region.start
            evidence.append(
                MmioTouchEvidence(
                    device_id=device_id,
                    theory=theory,
                    access=access,
                    width=access_width,
                    addr=hex(concrete_addr),
                    register_offset=register_offset,
                )
            )
    del side
    return tuple(evidence)


def route_symbolic_mmio_access(
    *,
    addr: Any,
    width: int,
    bank: SymbolicRegisterBankState,
    z3: Any,
    value: Any | None = None,
    is_write: bool = False,
) -> SymbolicAccessResult:
    """Route one symbolic MMIO access through the register-bank formulas."""
    if is_write:
        if value is None:
            raise ValueError("symbolic MMIO write requires value")
        return symbolic_write(addr, width, value, bank, z3)
    return symbolic_read(addr, width, bank, z3)


def discharge_unsupported_access(
    *,
    path_condition: Any,
    supported: Any,
    deadline: Deadline,
    z3: Any,
    extra_constraints: Sequence[Any] | None = None,
) -> UnsupportedAccessQuery:
    """Discharge ``path ∧ ¬supported`` via the independent discharge helper."""
    premises = [path_condition]
    if extra_constraints:
        premises.extend(extra_constraints)
    discharge = discharge_bad_conditions(
        premises=premises,
        bad_conditions=[z3.Not(supported)],
        deadline=deadline,
        algorithm=ALGORITHM,
        z3_module=z3,
    )
    if discharge.status == "unsat":
        status = UnsupportedAccessStatus.UNSAT
        inconclusive = False
    elif discharge.status == "sat":
        status = UnsupportedAccessStatus.SAT
        inconclusive = True
    else:
        status = UnsupportedAccessStatus.UNKNOWN
        inconclusive = True
    payload = {
        "algorithm": ALGORITHM,
        "kind": "unsupported-access",
        "schema_version": 1,
        "discharge_query_sha256": discharge.query_sha256,
    }
    return UnsupportedAccessQuery(
        status=status,
        query_sha256=canonical_json_sha256(payload),
        inconclusive=inconclusive,
        elapsed_ms=discharge.elapsed_ms,
    )


def query_unsupported_access(
    path_condition: Any,
    supported: Any,
    z3: Any,
    *,
    extra_constraints: list[Any] | None = None,
) -> UnsupportedAccessQuery:
    """Query ``path ∧ ¬supported`` as a separate obligation.

    SAT ⇒ reachable unsupported access ⇒ proof must be inconclusive.
    Do **not** add ``supported`` as an assumption to the equivalence query.
    """
    formula = z3.And(path_condition, z3.Not(supported))
    payload = {
        "algorithm": ALGORITHM,
        "kind": "unsupported-access",
        "schema_version": 1,
    }
    query_hash = canonical_json_sha256(payload)
    solver = z3.Solver()
    if extra_constraints:
        for constraint in extra_constraints:
            solver.add(constraint)
    solver.add(formula)
    check = solver.check()
    if check == z3.unsat:
        status = UnsupportedAccessStatus.UNSAT
        inconclusive = False
    elif check == z3.sat:
        status = UnsupportedAccessStatus.SAT
        inconclusive = True
    else:
        status = UnsupportedAccessStatus.UNKNOWN
        inconclusive = True
    return UnsupportedAccessQuery(
        status=status,
        query_sha256=query_hash,
        inconclusive=inconclusive,
    )


def build_register_bank_extensional_obligation(
    *,
    bus_spec_sha256: str,
    devices: list[dict[str, Any]],
    unsupported_access: dict[str, Any] | None = None,
    observability: dict[str, Any] | None = None,
    status: str = "scaffolded",
) -> dict[str, Any]:
    """Build the PR-14 obligation block shape (not yet promotion-discharged)."""
    block: dict[str, Any] = {
        "schema_version": 1,
        "algorithm": ALGORITHM,
        "status": status,
        "bus_spec_sha256": bus_spec_sha256,
        "devices": devices,
        "unsupported_access": unsupported_access
        if unsupported_access is not None
        else {
            "original": {"result": "not-queried", "query_sha256": None},
            "candidate": {"result": "not-queried", "query_sha256": None},
        },
    }
    if observability is not None:
        block["observability"] = observability
    return block


@dataclass(frozen=True, slots=True)
class SymbolicBusState:
    """Per-path symbolic MMIO device state carried on ``MachineState``."""

    banks: Mapping[str, SymbolicRegisterBankState]
    fifo_traces: Mapping[str, Any]
    touched_devices: frozenset[str] = frozenset()
    unsupported_predicates: tuple[Any, ...] = ()
    rejections: tuple[str, ...] = ()

    def with_bank(self, device_id: str, bank: SymbolicRegisterBankState) -> SymbolicBusState:
        banks = dict(self.banks)
        banks[device_id] = bank
        return SymbolicBusState(
            banks=banks,
            fifo_traces=dict(self.fifo_traces),
            touched_devices=self.touched_devices | {device_id},
            unsupported_predicates=self.unsupported_predicates,
            rejections=self.rejections,
        )

    def with_fifo(self, device_id: str, trace: Any) -> SymbolicBusState:
        traces = dict(self.fifo_traces)
        traces[device_id] = trace
        return SymbolicBusState(
            banks=dict(self.banks),
            fifo_traces=traces,
            touched_devices=self.touched_devices | {device_id},
            unsupported_predicates=self.unsupported_predicates,
            rejections=self.rejections,
        )

    def with_unsupported(self, predicate: Any) -> SymbolicBusState:
        return SymbolicBusState(
            banks=dict(self.banks),
            fifo_traces=dict(self.fifo_traces),
            touched_devices=self.touched_devices,
            unsupported_predicates=self.unsupported_predicates + (predicate,),
            rejections=self.rejections,
        )

    def with_rejection(self, reason: str) -> SymbolicBusState:
        return SymbolicBusState(
            banks=dict(self.banks),
            fifo_traces=dict(self.fifo_traces),
            touched_devices=self.touched_devices,
            unsupported_predicates=self.unsupported_predicates,
            rejections=self.rejections + (reason,),
        )


@dataclass(frozen=True, slots=True)
class SymbolicBusAccessOutcome:
    """Result of one SymbolicOps MMIO/FIFO access attempt."""

    handled: bool
    value: Any
    next_state: SymbolicBusState
    reason: str | None = None


def concrete_u32(addr: Any, z3: Any) -> int | None:
    """Return a concrete u32 when ``addr`` simplifies to a bitvector constant."""
    try:
        simplified = z3.simplify(addr)
    except Exception:
        return None
    if hasattr(simplified, "as_long"):
        try:
            return int(simplified.as_long()) & 0xFFFFFFFF
        except Exception:
            return None
    if isinstance(addr, int):
        return addr & 0xFFFFFFFF
    return None


def initial_symbolic_bus_state(
    memory_bus: Any,
    z3: Any,
    *,
    prefix: str = "mmio",
) -> SymbolicBusState | None:
    """Lift declared bus devices into a fresh symbolic CFG state.

    Returns ``None`` when the bus has no register-bank / FIFO devices (RAM/ROM
    only) — callers keep ordinary array routing.
    """
    from tools.ppc_equivalence.bus_spec import (
        lift_symbolic_fifo_traces,
        lift_symbolic_register_banks,
    )

    spec = getattr(memory_bus, "specification", None)
    if spec is None:
        return None
    banks = lift_symbolic_register_banks(spec, z3, prefix=prefix)
    traces = lift_symbolic_fifo_traces(spec)
    if not banks and not traces:
        return None
    return SymbolicBusState(banks=banks, fifo_traces=traces)


def _mmio_regions(address_space: AddressSpace) -> tuple[Region, ...]:
    return tuple(r for r in address_space.regions if r.kind is RegionKind.MMIO)


def _region_contains_concrete(region: Region, addr: int, width: int) -> bool:
    last = (addr + width - 1) & 0xFFFFFFFF
    if last < addr:
        return False
    return region.start <= addr and last <= region.end


def apply_symbolic_bus_access(
    bus: SymbolicBusState,
    *,
    address_space: AddressSpace,
    addr: Any,
    width: int,
    z3: Any,
    value: Any | None = None,
    is_write: bool = False,
) -> SymbolicBusAccessOutcome:
    """Route one SymbolicOps load/store through declared MMIO devices.

    Concrete addresses outside MMIO return ``handled=False`` (use RAM/ROM array).
    Unsupported addresses inside an MMIO region record a separate unsupported
    predicate and do **not** silently assume support into equivalence.
    """
    if width not in (1, 2, 4):
        return SymbolicBusAccessOutcome(
            handled=True,
            value=value if value is not None else _bv(z3, 0, 32),
            next_state=bus.with_rejection(f"unsupported-access-width-{width}"),
            reason="unsupported-width",
        )

    concrete = concrete_u32(addr, z3)
    mmio_regions = _mmio_regions(address_space)

    if concrete is not None:
        hit_region: Region | None = None
        for region in mmio_regions:
            if _region_contains_concrete(region, concrete, width):
                hit_region = region
                break
        if hit_region is None:
            return SymbolicBusAccessOutcome(
                handled=False,
                value=value if value is not None else _bv(z3, 0, 32),
                next_state=bus,
            )
        device_id = hit_region.device_id
        if device_id is None:
            return SymbolicBusAccessOutcome(
                handled=True,
                value=value if value is not None else _bv(z3, 0, 32),
                next_state=bus.with_unsupported(z3.BoolVal(True)).with_rejection(
                    "mmio-missing-device-id"
                ),
                reason="mmio-missing-device-id",
            )

        bank = bus.banks.get(device_id)
        if bank is not None:
            access = route_symbolic_mmio_access(
                addr=addr,
                width=width,
                bank=bank,
                z3=z3,
                value=value,
                is_write=is_write,
            )
            next_bus = bus.with_bank(device_id, access.next_state)
            # Concrete addr in MMIO: if not a declared register/width, unsupported.
            if z3.is_false(z3.simplify(access.supported)):
                next_bus = next_bus.with_unsupported(z3.BoolVal(True))
                return SymbolicBusAccessOutcome(
                    handled=True,
                    value=access.value,
                    next_state=next_bus,
                    reason="unsupported-register-access",
                )
            return SymbolicBusAccessOutcome(
                handled=True,
                value=access.value,
                next_state=next_bus,
            )

        trace = bus.fifo_traces.get(device_id)
        if trace is not None:
            if not is_write:
                return SymbolicBusAccessOutcome(
                    handled=True,
                    value=_bv(z3, 0, 32),
                    next_state=bus.with_unsupported(z3.BoolVal(True)).with_rejection(
                        "gxfifo-read-unsupported"
                    ),
                    reason="gxfifo-read-unsupported",
                )
            if value is None:
                raise ValueError("FIFO write requires value")
            next_trace, reject = trace.append_write(addr, width, value)
            if next_trace is None:
                return SymbolicBusAccessOutcome(
                    handled=True,
                    value=value,
                    next_state=bus.with_rejection(reject or "gxfifo-append-failed"),
                    reason=reject,
                )
            return SymbolicBusAccessOutcome(
                handled=True,
                value=value,
                next_state=bus.with_fifo(device_id, next_trace),
            )

        return SymbolicBusAccessOutcome(
            handled=True,
            value=value if value is not None else _bv(z3, 0, 32),
            next_state=bus.with_unsupported(z3.BoolVal(True)).with_rejection(
                f"mmio-unknown-device:{device_id}"
            ),
            reason="mmio-unknown-device",
        )

    # Symbolic address with MMIO present: route through register-bank formulas
    # using nested addr==reg conditions. Unsupported = in_mmio ∧ ¬supported is
    # recorded separately and never assumed into equivalence.
    if not mmio_regions:
        return SymbolicBusAccessOutcome(
            handled=False,
            value=value if value is not None else _bv(z3, 0, 32),
            next_state=bus,
        )

    # Only MMIO (no RAM/ROM): fully route. Mixed spaces with symbolic addresses
    # stay fail-closed — region path-splitting is deferred.
    non_mmio = [r for r in address_space.regions if r.kind is not RegionKind.MMIO]
    if non_mmio:
        return SymbolicBusAccessOutcome(
            handled=True,
            value=value if value is not None else _bv(z3, 0, 32),
            next_state=bus.with_unsupported(z3.BoolVal(True)).with_rejection(
                "symbolic-mmio-mixed-address-space"
            ),
            reason="symbolic-mmio-mixed-address-space",
        )

    next_bus = bus
    default_value = value if value is not None else _bv(z3, 0, 32)
    routed_value = default_value
    any_supported = z3.BoolVal(False)

    for region in mmio_regions:
        device_id = region.device_id
        if device_id is None:
            continue
        bank = next_bus.banks.get(device_id)
        if bank is not None:
            access = route_symbolic_mmio_access(
                addr=addr,
                width=width,
                bank=bank,
                z3=z3,
                value=value,
                is_write=is_write,
            )
            any_supported = z3.Or(any_supported, access.supported)
            next_bus = next_bus.with_bank(device_id, access.next_state)
            routed_value = z3.If(access.supported, access.value, routed_value)
            continue

        trace = next_bus.fifo_traces.get(device_id)
        if trace is not None:
            if not is_write:
                next_bus = next_bus.with_rejection("gxfifo-read-unsupported")
                continue
            if value is None:
                raise ValueError("FIFO write requires value")
            # Symbolic-address FIFO writes are unsupported (unbounded emission risk).
            next_bus = next_bus.with_rejection("symbolic-fifo-emission")
            continue

    next_bus = next_bus.with_unsupported(z3.Not(any_supported))
    return SymbolicBusAccessOutcome(
        handled=True,
        value=routed_value,
        next_state=next_bus,
    )


def symbolic_bus_difference(
    left: SymbolicBusState | None,
    right: SymbolicBusState | None,
    z3: Any,
) -> Any:
    """Boolean: visible MMIO/FIFO observables differ (automatic once touched)."""
    if left is None and right is None:
        return z3.BoolVal(False)
    if left is None or right is None:
        return z3.BoolVal(True)

    touched = left.touched_devices | right.touched_devices
    if not touched:
        return z3.BoolVal(False)

    diffs: list[Any] = []
    for device_id in sorted(touched):
        left_bank = left.banks.get(device_id)
        right_bank = right.banks.get(device_id)
        if left_bank is not None or right_bank is not None:
            if left_bank is None or right_bank is None:
                diffs.append(z3.BoolVal(True))
                continue
            offsets = sorted(set(left_bank.values) | set(right_bank.values))
            for offset in offsets:
                lv = left_bank.values.get(offset)
                rv = right_bank.values.get(offset)
                if lv is None or rv is None:
                    diffs.append(z3.BoolVal(True))
                else:
                    diffs.append(lv != rv)

        left_fifo = left.fifo_traces.get(device_id)
        right_fifo = right.fifo_traces.get(device_id)
        if left_fifo is not None or right_fifo is not None:
            if left_fifo is None or right_fifo is None:
                diffs.append(z3.BoolVal(True))
                continue
            compare = left_fifo.compare_equal(right_fifo, z3)
            if not compare.supported:
                diffs.append(z3.BoolVal(True))
            else:
                diffs.append(z3.Not(compare.equal))

    if left.rejections != right.rejections:
        diffs.append(z3.BoolVal(True))

    return z3.Or(*diffs) if diffs else z3.BoolVal(False)


def cfg_trace_sha256(terminals: Sequence[Any]) -> str:
    """Stable digest of CFG symbolic-bus instrumentation across terminals."""
    entries: list[dict[str, Any]] = []
    for index, terminal in enumerate(terminals):
        bus = getattr(terminal.state, "symbolic_bus", None)
        if bus is None:
            continue
        entries.append(
            {
                "terminal": index,
                "touched_devices": sorted(bus.touched_devices),
                "unsupported_count": len(bus.unsupported_predicates),
                "rejections": list(bus.rejections),
            }
        )
    return canonical_json_sha256(
        {"kind": "memory-bus-cfg-trace", "schema_version": 1, "terminals": entries}
    )


def vacuous_unsupported_access_block(
    *,
    cfg_trace_sha256: str,
    access_coverage_sha256: str,
) -> dict[str, Any]:
    """Vacuous discharge when CFG collected no unsupported predicates.

    Must not be silently rewritten as ``result=unsat``. Valid only when
    access-instrumentation completeness is attested (coverage digest bound).
    """
    return {
        "status": "vacuously-discharged",
        "reason": "no-unsupported-predicates",
        "cfg_trace_sha256": cfg_trace_sha256,
        "access_coverage_sha256": access_coverage_sha256,
    }


def aggregate_unsupported_access_query_sha256(
    terminal_query_sha256s: Sequence[str],
) -> str:
    """Aggregate digest over every terminal unsupported-access query.

    Top-level ``unsupported_access.*.query_sha256`` must bind *all* terminal
    digests, not only the last query.
    """
    return canonical_json_sha256(
        {
            "kind": "unsupported-access-aggregate",
            "schema_version": 1,
            "query_sha256s": list(terminal_query_sha256s),
        }
    )


def discharge_cfg_unsupported_accesses(
    terminals: Sequence[Any],
    *,
    side: str,
    deadline: Deadline,
    z3: Any,
    access_coverage_sha256: str | None = None,
) -> dict[str, Any]:
    """Per-side digest of CFG-collected ``path ∧ unsupported`` obligations.

    When a symbolic bus is present but no unsupported predicates were
    collected, emit a vacuous block (not a forged UNSAT). Vacuous requires
    ``access_coverage_sha256`` attesting instrumentation completeness.
    """
    del side
    if not terminals:
        return {"result": "not-queried", "query_sha256": None, "terminals": []}

    terminal_results: list[dict[str, Any]] = []
    worst = "unsat"
    query_hashes: list[str] = []
    total_elapsed_ms = 0.0
    had_symbolic_bus = False
    for terminal in terminals:
        bus = getattr(terminal.state, "symbolic_bus", None)
        if bus is None:
            continue
        had_symbolic_bus = True
        if not bus.unsupported_predicates:
            continue
        path = terminal.condition
        for index, predicate in enumerate(bus.unsupported_predicates):
            # Query path ∧ unsupported_predicate (already ¬supported shaped).
            discharge = discharge_bad_conditions(
                premises=[path],
                bad_conditions=[predicate],
                deadline=deadline,
                algorithm=ALGORITHM,
                z3_module=z3,
            )
            total_elapsed_ms += discharge.elapsed_ms
            if discharge.status == "unsat":
                status = "unsat"
                inconclusive = False
            elif discharge.status == "sat":
                status = "sat"
                inconclusive = True
            else:
                status = "unknown"
                inconclusive = True
            payload = {
                "algorithm": ALGORITHM,
                "kind": "unsupported-access",
                "schema_version": 1,
                "discharge_query_sha256": discharge.query_sha256,
                "terminal_index": index,
            }
            query_hash = canonical_json_sha256(payload)
            query_hashes.append(query_hash)
            terminal_results.append(
                {
                    "result": status,
                    "query_sha256": query_hash,
                    "inconclusive": inconclusive,
                    "solver": {
                        "name": "z3",
                        "version": z3.get_version_string(),
                        "elapsed_ms": discharge.elapsed_ms,
                    },
                }
            )
            if status == "sat":
                worst = "sat"
            elif status == "unknown" and worst == "unsat":
                worst = "unknown"

    if not terminal_results:
        if had_symbolic_bus:
            if not isinstance(access_coverage_sha256, str) or len(access_coverage_sha256) != 64:
                return {
                    "result": "not-queried",
                    "query_sha256": None,
                    "terminals": [],
                    "note": "vacuous-requires-access-coverage-attestation",
                }
            return vacuous_unsupported_access_block(
                cfg_trace_sha256=cfg_trace_sha256(terminals),
                access_coverage_sha256=access_coverage_sha256,
            )
        return {"result": "not-queried", "query_sha256": None, "terminals": []}
    return {
        "result": worst,
        "query_sha256": aggregate_unsupported_access_query_sha256(query_hashes),
        "solver": {
            "name": "z3",
            "version": z3.get_version_string(),
            "elapsed_ms": total_elapsed_ms,
        },
        "terminals": terminal_results,
    }
