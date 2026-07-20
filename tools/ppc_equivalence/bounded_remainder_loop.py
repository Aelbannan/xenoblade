"""Bounded symbolic remainder loops (PR11).

Supports small upper-bound trip forms (``N & 7``, ``N & 3``, concrete constants)
with guarded finite expansion — no quantifiers. Trip count zero under ``bdnz``
requires a proven skip guard or concrete nonzero trip; limits are never assumed
as preconditions.
"""

from __future__ import annotations

from collections.abc import Sequence
from dataclasses import dataclass, replace
from typing import Any

from tools.ppc_equivalence.ir import Instruction, Opcode
from tools.ppc_equivalence.memory_semantics import (
  StoreEffect,
  apply_memory_loop_transition,
  apply_store_effect,
  build_memory_loop_transition,
  footprint_ok_for_summary,
)
from tools.ppc_equivalence.model import MachineState
from tools.ppc_equivalence.trip_expression import (
  TripAnd,
  TripConstant,
  TripEntryReg,
  TripExpr,
  canonical_dict,
  evaluate_concrete,
  evaluate_symbolic,
  normalize_trip_expr,
  recognize_trip_expr,
  remainder_upper_bound,
)

# Fail closed above this symbolic remainder bound.
MAX_SYMBOLIC_REMAINDER_BOUND = 7

_CTR_SPR = 9
_BDNZ_BO = 16


@dataclass(frozen=True)
class ZeroTripGuard:
  """How a zero-trip ``bdnz`` hazard is ruled out."""

  kind: str  # "concrete-nonzero" | "skip-branch" | "unsupported"
  notes: tuple[str, ...] = ()


@dataclass(frozen=True)
class BoundedRemainderTrip:
  """Recognized symbolic or concrete remainder trip before ``mtctr``."""

  expr: TripExpr
  expr_canonical: dict[str, Any]
  upper_bound: int
  concrete_trip: int | None
  zero_guard: ZeroTripGuard
  notes: tuple[str, ...]


def recover_bounded_remainder_trip(
  instructions: Sequence[Instruction],
  mtctr_index: int,
  trip_reg: int,
  *,
  header_pc: int,
  readonly_words: dict[int, int] | None = None,
) -> BoundedRemainderTrip | None:
  """Recognize a bounded remainder trip expression before ``mtctr``."""
  expr, notes = recognize_trip_expr(
    instructions,
    mtctr_index,
    trip_reg,
    readonly_words=readonly_words,
  )
  if expr is None:
    return None

  expr = normalize_trip_expr(expr)
  upper = remainder_upper_bound(expr)
  if upper is None or upper > MAX_SYMBOLIC_REMAINDER_BOUND:
    return None

  concrete = evaluate_concrete(expr, {})
  zero_guard = _classify_zero_guard(
    instructions,
    mtctr_index,
    header_pc,
    expr,
    concrete_trip=concrete,
  )
  if zero_guard.kind == "unsupported":
    return None

  return BoundedRemainderTrip(
    expr=expr,
    expr_canonical=canonical_dict(expr),
    upper_bound=int(upper),
    concrete_trip=concrete,
    zero_guard=zero_guard,
    notes=tuple(notes),
  )


def apply_bounded_remainder_memory_loop(
  state: MachineState,
  *,
  trip: BoundedRemainderTrip,
  base_reg: int,
  source_reg: int,
  stride: int,
  store_width: int,
  store_kind: str,
  final_ctr: int,
  ops: Any,
) -> MachineState:
  """Guarded finite expansion of a remainder store loop (no quantifiers)."""
  if store_kind not in ("stwu", "d-form-addi"):
    raise ValueError(f"unsupported store_kind {store_kind!r}")
  if stride != store_width:
    raise ValueError("stride must equal store width for remainder expansion")
  if not footprint_ok_for_summary(
    trip_count=max(1, trip.upper_bound),
    stride=stride,
    store_width=store_width,
    store_kind=store_kind,
  ):
    raise ValueError("remainder loop footprint fails static gate")

  if trip.concrete_trip is not None and trip.concrete_trip >= 1:
    transition = build_memory_loop_transition(
      state,
      trip_count=int(trip.concrete_trip),
      base_reg=base_reg,
      source_reg=source_reg,
      stride=stride,
      store_width=store_width,
      store_kind=store_kind,
      final_ctr=final_ctr,
      ops=ops,
    )
    return apply_memory_loop_transition(state, transition, ops)

  entry_regs = {reg: state.gpr[reg] for reg in range(32)}
  trip_value = evaluate_symbolic(trip.expr, entry_regs, ops)
  current = state
  for index in range(trip.upper_bound):
    guard = ops.unsigned_gt(trip_value, ops.const(index))
    if store_kind == "stwu":
      offset = (index + 1) * stride
    else:
      offset = index * stride
    address = ops.add(state.gpr[base_reg], ops.const(offset & 0xFFFFFFFF))
    effect = StoreEffect(
      address=address,
      value=state.gpr[source_reg],
      width=store_width,
    )
    stepped = apply_store_effect(current, effect, ops)
    current = _ite_machine_state(ops, guard, stepped, current)

  delta = ops.mul(trip_value, ops.const(stride & 0xFFFFFFFF))
  gprs = list(current.gpr)
  gprs[base_reg] = ops.add(state.gpr[base_reg], delta)
  return replace(
    current,
    gpr=tuple(gprs),
    ctr=ops.const(int(final_ctr) & 0xFFFFFFFF),
  )


def _ite_machine_state(
  ops: Any,
  condition: Any,
  yes: MachineState,
  no: MachineState,
) -> MachineState:
  if getattr(ops, "z3", None) is None and condition in (True, False):
    return yes if condition else no

  gpr = tuple(ops.ite(condition, y, n) for y, n in zip(yes.gpr, no.gpr))
  memory = ops.ite(condition, yes.memory, no.memory)
  valid = ops.ite(condition, yes.valid, no.valid)
  memory_writes = yes.memory_writes + no.memory_writes
  memory_touches = yes.memory_touches + no.memory_touches
  if getattr(ops, "z3", None) is not None:
    z3 = ops.z3
    invalid_reason = z3.If(condition, yes.invalid_reason, no.invalid_reason)
  else:
    invalid_reason = yes.invalid_reason if condition else no.invalid_reason
  return replace(
    yes,
    gpr=gpr,
    memory=memory,
    valid=valid,
    invalid_reason=invalid_reason,
    memory_writes=memory_writes,
    memory_touches=memory_touches,
  )


def _classify_zero_guard(
  instructions: Sequence[Instruction],
  mtctr_index: int,
  header_pc: int,
  expr: TripExpr,
  *,
  concrete_trip: int | None,
) -> ZeroTripGuard:
  if concrete_trip is not None:
    if concrete_trip == 0:
      if _has_skip_guard_before_header(instructions, mtctr_index, header_pc):
        return ZeroTripGuard("skip-branch", ("zero concrete trip with skip guard",))
      return ZeroTripGuard("unsupported", ("CTR load of 0 wraps under bdnz",))
    return ZeroTripGuard("concrete-nonzero", ())

  if isinstance(expr, TripAnd) and isinstance(expr.left, TripEntryReg):
    if _has_skip_guard_before_header(instructions, mtctr_index, header_pc):
      return ZeroTripGuard("skip-branch", ("symbolic remainder with skip guard",))
    return ZeroTripGuard(
      "unsupported",
      ("symbolic remainder may be zero under bdnz without skip guard",),
    )

  if isinstance(expr, TripConstant):
    if int(expr.value) == 0:
      if _has_skip_guard_before_header(instructions, mtctr_index, header_pc):
        return ZeroTripGuard("skip-branch", ())
      return ZeroTripGuard("unsupported", ("constant zero trip",))
    return ZeroTripGuard("concrete-nonzero", ())

  return ZeroTripGuard("unsupported", ("zero-trip guard not established",))


def _has_skip_guard_before_header(
  instructions: Sequence[Instruction],
  mtctr_index: int,
  header_pc: int,
) -> bool:
  """True when a conditional branch may skip the loop when trip is zero."""
  for index in range(max(0, mtctr_index - 8), mtctr_index + 1):
    insn = instructions[index]
    if insn.opcode != Opcode.BC or insn.link:
      continue
    _bo, _bi, target, _aa = (int(v) for v in insn.operands)
    target_pc = int(target) & 0xFFFFFFFC
    if target_pc > header_pc:
      return True
  return False
