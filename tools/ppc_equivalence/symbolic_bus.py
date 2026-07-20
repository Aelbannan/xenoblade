"""Extensional symbolic register-bank theory scaffold (Track D / toward PR 14).

Models a finite MMIO register bank as per-register bitvectors with nested
``addr == base + offset`` routing. Reachable ``Not(supported)`` is a *separate*
unsupported-access obligation (query ``path ∧ ¬supported``); it must **not** be
assumed into the equivalence query.

Scaffolding only: ``memory-bus`` remains in ``UNSUPPORTED_FOR_EQUIVALENT``.
Symbolic MMIO is not yet bound into ``check_equivalence`` / ``WordOps`` CFG
execution. Concrete ``MemoryBus`` routing is unchanged.
"""

from __future__ import annotations

from dataclasses import dataclass
from enum import Enum
from typing import Any, Mapping

from tools.ppc_equivalence.bus_spec import DeviceSpecification
from tools.ppc_equivalence.device_model import RegisterSpec
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
    "build_register_bank_extensional_obligation",
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
    status: str = "scaffolded",
) -> dict[str, Any]:
    """Build the PR-14 obligation block shape (not yet promotion-discharged)."""
    return {
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
