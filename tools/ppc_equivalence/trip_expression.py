"""Symbolic trip-count AST for ``mtctr`` materialization (PR10).

Recognizes only explicit straight-line instruction patterns immediately before
``mtctr``. Entry-parameter registers stay as :class:`TripEntryReg` (entry-state
semantics), not late mutated values. Prefer false negatives.
"""

from __future__ import annotations

from collections.abc import Mapping, Sequence
from dataclasses import dataclass
from typing import Any, Union

from tools.ppc_equivalence.ir import Instruction, Opcode

_DEFAULT_LOOKBACK = 12

TripExpr = Union["TripConstant", "TripEntryReg", "TripAnd", "TripLshr", "TripAdd", "TripCountdown"]


@dataclass(frozen=True, slots=True)
class TripConstant:
  value: int

  def __post_init__(self) -> None:
    object.__setattr__(self, "value", int(self.value) & 0xFFFFFFFF)


@dataclass(frozen=True, slots=True)
class TripEntryReg:
  reg: int

  def __post_init__(self) -> None:
    reg = int(self.reg)
    if reg < 0 or reg > 31:
      raise ValueError(f"TripEntryReg reg out of range: {reg}")
    object.__setattr__(self, "reg", reg)


@dataclass(frozen=True, slots=True)
class TripAnd:
  left: TripExpr
  right: TripExpr


@dataclass(frozen=True, slots=True)
class TripLshr:
  left: TripExpr
  shift: int

  def __post_init__(self) -> None:
    shift = int(self.shift)
    if not 1 <= shift <= 31:
      raise ValueError(f"TripLshr shift out of range: {shift}")
    object.__setattr__(self, "shift", shift)


@dataclass(frozen=True, slots=True)
class TripAdd:
  left: TripExpr
  right: TripExpr


@dataclass(frozen=True, slots=True)
class TripCountdown:
  """Do-while countdown trip (doc 30 Phase C2).

  Models a bottom-tested ``addi/subi rT, rT, ±step; cmp rT, bound;
  b<family> latch`` loop. The counter is checked *after* each step, so the
  body executes at least once whenever the header is reached (do-while).

  The trip is the smallest ``k >= 1`` with ``NOT continue(entry + k*step)``
  (the exit relation), or non-terminating when the counter can never reach
  the exit region before wrapping. ``signed`` selects the compare form's
  signedness (``cmpwi``/``cmpw`` = signed, ``cmplwi``/``cmplw`` = unsigned).
  """

  entry: TripExpr
  bound: TripExpr
  step: int  # signed increment per iteration (negative = countdown)
  family: str  # "bne" | "beq" | "blt" | "bgt" | "ble" | "bge" (continue relation)
  signed: bool

  def __post_init__(self) -> None:
    if int(self.step) == 0:
      raise ValueError("TripCountdown step must be nonzero")
    if self.family not in ("bne", "beq", "blt", "bgt", "ble", "bge"):
      raise ValueError(f"unknown TripCountdown family {self.family!r}")
    object.__setattr__(self, "step", int(self.step))
    object.__setattr__(self, "signed", bool(self.signed))


# Exits that terminate by crossing a half-line (all families except ``bne``)
# have a closed-form trip; ``bne`` terminates only when the counter lands
# exactly on the bound. This is the ``terminates`` predicate used by the
# recognizer and by ``evaluate_concrete``.
def countdown_trip(
  entry: int,
  bound: int,
  step: int,
  family: str,
  *,
  signed: bool,
) -> int | None:
  """Exact do-while trip for a countdown/count-up latch, or ``None`` when the
  loop cannot be shown to terminate before wrap.

  ``entry``/``bound`` are u32 values; the comparison uses ``signed``.
  ``step`` is the signed per-iteration delta. The counter is checked after
  each step (``rT_k = entry + k*step``), so the body executes at least once
  whenever the header is reached. Returns the number of iterations (>= 1) or
  ``None`` (fail-closed: the continue relation holds forever).
  """
  def _sub(a: int, b: int) -> int:
    if signed:
      sa = a - 0x100000000 if a >= 0x80000000 else a
      sb = b - 0x100000000 if b >= 0x80000000 else b
      return sa - sb
    return (a - b) & 0xFFFFFFFF

  s = abs(int(step))
  if s == 0:
    return None
  if step < 0:
    # Countdown: values entry - k*s. D = entry - bound.
    d = _sub(entry, bound)
    if family == "bge":  # continue while >= ; exit when < bound
      if d < 0:
        return 1
      return d // s + 1
    if family == "bgt":  # exit when <= bound
      if d < 0:
        return 1
      return (d - 1) // s + 1
    if family == "ble":  # exit when > bound
      # The counter only decreases: once <= bound it stays <= forever.
      if d > s:
        return 1
      return None
    if family == "blt":  # exit when >= bound
      # Once < bound the counter stays < forever.
      if d >= s:
        return 1
      return None
    if family == "bne":  # exit when == bound
      if d % s != 0 or d < s:
        return None
      return d // s
    if family == "beq":  # exit when != bound (degenerate)
      return 2 if d == s else 1
  else:
    # Count-up: values entry + k*s. D = bound - entry.
    d = _sub(bound, entry)
    if family == "blt":  # continue while < ; exit when >= bound
      if d <= 0:
        return 1
      return (d - 1) // s + 1
    if family == "ble":  # exit when > bound
      if d < 0:
        return 1
      return d // s + 1
    if family == "bgt":  # exit when <= bound
      # The counter only increases: once > bound it stays > forever.
      if d >= s:
        return 1
      return None
    if family == "bge":  # exit when < bound
      # Once >= bound the counter stays >= forever.
      if d > s:
        return 1
      return None
    if family == "bne":  # exit when == bound
      if d % s != 0 or d < s:
        return None
      return d // s
    if family == "beq":
      return 2 if d == s else 1
  raise ValueError(f"unknown TripCountdown family {family!r}")


def canonical_dict(expr: TripExpr) -> dict[str, Any]:
  """Normalized JSON-serializable AST for stable comparison."""
  expr = normalize_trip_expr(expr)
  if isinstance(expr, TripConstant):
    return {"kind": "const", "value": int(expr.value)}
  if isinstance(expr, TripEntryReg):
    return {"kind": "entry", "reg": int(expr.reg)}
  if isinstance(expr, TripAnd):
    return {
      "kind": "and",
      "left": canonical_dict(expr.left),
      "right": canonical_dict(expr.right),
    }
  if isinstance(expr, TripLshr):
    return {
      "kind": "lshr",
      "left": canonical_dict(expr.left),
      "shift": int(expr.shift),
    }
  if isinstance(expr, TripAdd):
    return {
      "kind": "add",
      "left": canonical_dict(expr.left),
      "right": canonical_dict(expr.right),
    }
  if isinstance(expr, TripCountdown):
    return {
      "kind": "countdown",
      "entry": canonical_dict(expr.entry),
      "bound": canonical_dict(expr.bound),
      "step": int(expr.step),
      "family": expr.family,
      "signed": bool(expr.signed),
    }
  raise TypeError(f"unsupported TripExpr {type(expr)!r}")


def trip_exprs_equal(left: TripExpr, right: TripExpr) -> bool:
  """Compare trip ASTs via normalized canonical forms."""
  return canonical_dict(left) == canonical_dict(right)


def normalize_trip_expr(expr: TripExpr) -> TripExpr:
  """Fold constant-only subtrees."""
  if isinstance(expr, (TripConstant, TripEntryReg)):
    return expr
  if isinstance(expr, TripAnd):
    left = normalize_trip_expr(expr.left)
    right = normalize_trip_expr(expr.right)
    if isinstance(left, TripConstant) and isinstance(right, TripConstant):
      return TripConstant(left.value & right.value)
    return TripAnd(left, right)
  if isinstance(expr, TripLshr):
    left = normalize_trip_expr(expr.left)
    if isinstance(left, TripConstant):
      return TripConstant(left.value >> int(expr.shift))
    return TripLshr(left, expr.shift)
  if isinstance(expr, TripAdd):
    left = normalize_trip_expr(expr.left)
    right = normalize_trip_expr(expr.right)
    if isinstance(left, TripConstant) and isinstance(right, TripConstant):
      return TripConstant((left.value + right.value) & 0xFFFFFFFF)
    return TripAdd(left, right)
  if isinstance(expr, TripCountdown):
    entry = normalize_trip_expr(expr.entry)
    bound = normalize_trip_expr(expr.bound)
    folded = TripCountdown(entry, bound, expr.step, expr.family, expr.signed)
    if isinstance(entry, TripConstant) and isinstance(bound, TripConstant):
      trip = countdown_trip(
        int(entry.value), int(bound.value), int(folded.step), folded.family,
        signed=folded.signed,
      )
      if trip is not None:
        return TripConstant(trip)
    return folded
  raise TypeError(f"unsupported TripExpr {type(expr)!r}")


def evaluate_symbolic(
  expr: TripExpr,
  entry: Mapping[int, Any],
  ops: Any,
) -> Any:
  """Evaluate ``expr`` using entry-state GPR symbols and ``WordOps``."""
  expr = normalize_trip_expr(expr)
  if isinstance(expr, TripConstant):
    return ops.const(int(expr.value))
  if isinstance(expr, TripEntryReg):
    try:
      return entry[expr.reg]
    except KeyError as exc:
      raise KeyError(f"entry GPR r{expr.reg} missing for trip expression") from exc
  if isinstance(expr, TripAnd):
    return ops.band(
      evaluate_symbolic(expr.left, entry, ops),
      evaluate_symbolic(expr.right, entry, ops),
    )
  if isinstance(expr, TripLshr):
    return ops.lshr(
      evaluate_symbolic(expr.left, entry, ops),
      ops.const(int(expr.shift)),
    )
  if isinstance(expr, TripAdd):
    return ops.add(
      evaluate_symbolic(expr.left, entry, ops),
      evaluate_symbolic(expr.right, entry, ops),
    )
  if isinstance(expr, TripCountdown):
    return _countdown_trip_symbolic(
      evaluate_symbolic(expr.entry, entry, ops),
      evaluate_symbolic(expr.bound, entry, ops),
      int(expr.step),
      expr.family,
      expr.signed,
      ops,
    )
  raise TypeError(f"unsupported TripExpr {type(expr)!r}")


def trip_expr_from_canonical(data: Mapping[str, Any]) -> TripExpr:
    """Rebuild a :class:`TripExpr` from its canonical dict form."""
    kind = data.get("kind")
    if kind == "const":
        return TripConstant(int(data["value"]))
    if kind == "entry":
        return TripEntryReg(int(data["reg"]))
    if kind == "and":
        return TripAnd(trip_expr_from_canonical(data["left"]), trip_expr_from_canonical(data["right"]))
    if kind == "lshr":
        return TripLshr(trip_expr_from_canonical(data["left"]), int(data["shift"]))
    if kind == "add":
        return TripAdd(trip_expr_from_canonical(data["left"]), trip_expr_from_canonical(data["right"]))
    if kind == "countdown":
        return TripCountdown(
            trip_expr_from_canonical(data["entry"]),
            trip_expr_from_canonical(data["bound"]),
            int(data["step"]),
            str(data["family"]),
            bool(data["signed"]),
        )
    raise ValueError(f"unknown trip_expr kind {kind!r}")


def evaluate_concrete(
  expr: TripExpr,
  entry_values: Mapping[int, int],
) -> int | None:
  """Concrete evaluation when every :class:`TripEntryReg` has a known value."""
  expr = normalize_trip_expr(expr)
  if isinstance(expr, TripConstant):
    return int(expr.value)
  if isinstance(expr, TripEntryReg):
    if expr.reg not in entry_values:
      return None
    return int(entry_values[expr.reg]) & 0xFFFFFFFF
  if isinstance(expr, TripAnd):
    left = evaluate_concrete(expr.left, entry_values)
    right = evaluate_concrete(expr.right, entry_values)
    if left is None or right is None:
      return None
    return (left & right) & 0xFFFFFFFF
  if isinstance(expr, TripLshr):
    left = evaluate_concrete(expr.left, entry_values)
    if left is None:
      return None
    return (left >> int(expr.shift)) & 0xFFFFFFFF
  if isinstance(expr, TripAdd):
    left = evaluate_concrete(expr.left, entry_values)
    right = evaluate_concrete(expr.right, entry_values)
    if left is None or right is None:
      return None
    return (left + right) & 0xFFFFFFFF
  if isinstance(expr, TripCountdown):
    entry_val = evaluate_concrete(expr.entry, entry_values)
    if entry_val is None:
      return None
    bound_val = evaluate_concrete(expr.bound, entry_values)
    if bound_val is None:
      return None
    return countdown_trip(
      entry_val, bound_val, int(expr.step), expr.family, signed=expr.signed,
    )
  return None


def _countdown_trip_symbolic(
  entry: Any,
  bound: Any,
  step: int,
  family: str,
  signed: bool,
  ops: Any,
) -> Any:
  """Symbolic do-while trip for a countdown/count-up latch (z3/Concrete).

  Mirrors ``countdown_trip`` exactly; the terminating families (``bne`` and
  the degenerate directions of ``blt``/``ble``/``bgt``/``bge``) are handled
  by the caller via ``countdown_termination_premise``.
  """
  one = ops.const(1)
  zero = ops.const(0)
  s = abs(int(step))
  s_bv = ops.const(s)
  if step < 0:
    d = ops.sub(entry, bound)
    negative = ops.signed_lt(d, zero) if signed else ops.bool(False)
    if family == "bge":
      return ops.ite(negative, one, ops.add(ops.unsigned_div(d, s_bv), one))
    if family == "bgt":
      le_zero = ops.lnot(ops.signed_lt(zero, d)) if signed else ops.eq(d, zero)
      return ops.ite(le_zero, one, ops.add(ops.unsigned_div(ops.sub(d, one), s_bv), one))
    if family in ("ble", "blt"):
      # Terminating cases only (premise enforced by the caller): T == 1.
      return one
    if family == "bne":
      return ops.unsigned_div(d, s_bv)
    if family == "beq":
      return ops.ite(ops.eq(d, s_bv), ops.const(2), one)
  else:
    d = ops.sub(bound, entry)
    negative = ops.signed_lt(d, zero) if signed else ops.bool(False)
    if family == "blt":
      le_zero = ops.lnot(ops.signed_lt(zero, d)) if signed else ops.eq(d, zero)
      return ops.ite(le_zero, one, ops.add(ops.unsigned_div(ops.sub(d, one), s_bv), one))
    if family == "ble":
      return ops.ite(negative, one, ops.add(ops.unsigned_div(d, s_bv), one))
    if family in ("bgt", "bge"):
      return one
    if family == "bne":
      return ops.unsigned_div(d, s_bv)
    if family == "beq":
      return ops.ite(ops.eq(d, s_bv), ops.const(2), one)
  raise ValueError(f"unknown TripCountdown family {family!r}")


def remainder_upper_bound(expr: TripExpr) -> int | None:
  """Return a conservative small upper bound for supported remainder forms."""
  expr = normalize_trip_expr(expr)
  if isinstance(expr, TripConstant):
    return int(expr.value)
  if isinstance(expr, TripAnd):
    mask = _andi_mask(expr)
    if mask is not None:
      return mask
    return None
  if isinstance(expr, TripLshr):
    # Bulk ``N >> k`` is not a remainder bound for PR11.
    return None
  if isinstance(expr, TripEntryReg):
    return None
  if isinstance(expr, TripAdd):
    return None
  if isinstance(expr, TripCountdown):
    return None
  return None


def countdown_termination_premise(
  expr: TripCountdown,
  entry_gpr: Mapping[int, Any],
  ops: Any,
) -> Any | None:
  """The premise under which a countdown latch terminates before wrap.

  Returns the family-specific condition (``bne`` divisibility, degenerate
  directions) conjoined with the ranking-wrap exclusion (the movement
  ``trip * step`` must stay in the unsigned range so the termination ranking
  holds on every continued step). ``None`` when the family always terminates
  wrap-free (never happens in practice once the wrap conjunct is included,
  except for families whose trip is exactly 1).
  """
  entry = evaluate_symbolic(expr.entry, entry_gpr, ops)
  bound = evaluate_symbolic(expr.bound, entry_gpr, ops)
  s = abs(int(expr.step))
  s_bv = ops.const(s)
  zero = ops.const(0)
  if expr.step < 0:
    d = ops.sub(entry, bound)
    if expr.family == "ble":
      family_premise = ops.signed_lt(s_bv, d) if expr.signed else ops.unsigned_lt(
        s_bv, d,
      )
    elif expr.family == "blt":
      family_premise = ops.lnot(ops.signed_lt(d, s_bv)) if expr.signed else ops.lnot(
        ops.unsigned_lt(d, s_bv),
      )
    elif expr.family == "bne":
      divisible = ops.eq(ops.sub(d, ops.mul(ops.unsigned_div(d, s_bv), s_bv)), zero)
      far_enough = (
        ops.lnot(ops.signed_lt(d, s_bv)) if expr.signed else ops.lnot(
          ops.unsigned_lt(d, s_bv),
        )
      )
      family_premise = ops.land(divisible, far_enough)
    else:
      family_premise = None
  else:
    d = ops.sub(bound, entry)
    if expr.family == "bgt":
      family_premise = ops.lnot(ops.signed_lt(d, s_bv)) if expr.signed else ops.lnot(
        ops.unsigned_lt(d, s_bv),
      )
    elif expr.family == "bge":
      family_premise = ops.signed_lt(s_bv, d) if expr.signed else ops.unsigned_lt(
        s_bv, d,
      )
    elif expr.family == "bne":
      divisible = ops.eq(ops.sub(d, ops.mul(ops.unsigned_div(d, s_bv), s_bv)), zero)
      far_enough = (
        ops.lnot(ops.signed_lt(d, s_bv)) if expr.signed else ops.lnot(
          ops.unsigned_lt(d, s_bv),
        )
      )
      family_premise = ops.land(divisible, far_enough)
    else:
      family_premise = None
  trip_value = evaluate_symbolic(expr, entry_gpr, ops)
  if expr.step < 0:
    # (trip - 1) * s <= entry — only the continue steps need the ranking.
    wrap_free = ops.lnot(
      ops.unsigned_lt(
        entry,
        ops.mul(ops.sub(trip_value, ops.const(1)), s_bv),
      ),
    )
  else:
    # (trip - 1) * s <= 0xffffffff - entry (no modulo wrap in the check).
    wrap_free = ops.lnot(
      ops.unsigned_lt(
        ops.sub(ops.const(0xFFFFFFFF), entry),
        ops.mul(ops.sub(trip_value, ops.const(1)), s_bv),
      ),
    )
  if family_premise is None:
    return wrap_free
  return ops.land(family_premise, wrap_free)


def bulk_shift_amount(expr: TripExpr) -> int | None:
  """Return ``k`` when ``expr`` is ``entry >> k`` (PR12 bulk scaffold)."""
  expr = normalize_trip_expr(expr)
  if isinstance(expr, TripLshr) and isinstance(expr.left, TripEntryReg):
    return int(expr.shift)
  return None


def remainder_mask_for_shift(k: int) -> int:
  """``N & ((1 << k) - 1)`` mask for a bulk shift ``k``."""
  if not 1 <= k <= 31:
    raise ValueError(f"bulk shift k out of range: {k}")
  return (1 << k) - 1


def recognize_trip_expr(
  instructions: Sequence[Instruction],
  use_index: int,
  reg: int,
  *,
  max_lookback: int = _DEFAULT_LOOKBACK,
  readonly_words: Mapping[int, int] | None = None,
) -> tuple[TripExpr | None, list[str]]:
  """Build a trip AST for ``reg`` immediately before ``use_index``."""
  if use_index <= 0:
    return None, ["missing GPR materialization before use"]
  if max_lookback < 1:
    return None, ["GPR materialization lookback disabled"]

  lower = max(0, use_index - max_lookback)
  for index in range(use_index - 1, lower - 1, -1):
    insn = instructions[index]
    if not _defines_gpr(insn, reg):
      continue
    return _recognize_defining_insn(
      instructions,
      index,
      insn,
      reg,
      max_lookback=max_lookback,
      readonly_words=readonly_words,
      depth=0,
    )
  return None, [f"no trip expression for r{reg} within lookback"]


def _recognize_defining_insn(
  instructions: Sequence[Instruction],
  index: int,
  insn: Instruction,
  reg: int,
  *,
  max_lookback: int,
  readonly_words: Mapping[int, int] | None,
  depth: int,
) -> tuple[TripExpr | None, list[str]]:
  if depth >= max_lookback:
    return None, ["trip-expression lookback exceeded"]
  if insn.relocation is not None:
    return None, ["relocated GPR materialization is not symbolic"]

  opcode = insn.opcode
  if opcode == Opcode.ADDI:
    rt, ra, imm = (int(v) for v in insn.operands)
    if rt != reg:
      return None, [f"addi defines r{rt}, expected r{reg}"]
    if ra == 0:
      return TripConstant(_sign_extend_16(imm)), []
    base, notes = _recognize_gpr_before(
      instructions, index, ra, max_lookback=max_lookback,
      readonly_words=readonly_words, depth=depth + 1,
    )
    if base is None:
      return None, notes or [f"addi r{rt}, r{ra}, {imm} base not recognized"]
    return TripAdd(base, TripConstant(_sign_extend_16(imm))), notes

  if opcode == Opcode.ADDIS:
    rt, ra, imm = (int(v) for v in insn.operands)
    if rt != reg:
      return None, [f"addis defines r{rt}, expected r{reg}"]
    base, notes = _recognize_gpr_before(
      instructions, index, ra, max_lookback=max_lookback,
      readonly_words=readonly_words, depth=depth + 1,
    )
    if base is None:
      return None, notes or [f"addis r{rt}, r{ra}, {imm} base not recognized"]
    return TripAdd(base, TripConstant((int(imm) & 0xFFFF) << 16)), notes

  if opcode == Opcode.ORI:
    rt, ra, imm = (int(v) for v in insn.operands)
    if rt != reg:
      return None, [f"ori defines r{rt}, expected r{reg}"]
    base, notes = _recognize_gpr_before(
      instructions, index, ra, max_lookback=max_lookback,
      readonly_words=readonly_words, depth=depth + 1,
    )
    if base is None:
      return None, notes or [f"ori r{rt}, r{ra}, {imm} base not recognized"]
    if isinstance(base, TripConstant):
      return TripConstant(base.value | (int(imm) & 0xFFFF)), notes
    return None, ["ori trip expression requires concrete high half"]

  if opcode == Opcode.ORIS:
    rt, ra, imm = (int(v) for v in insn.operands)
    if rt != reg:
      return None, [f"oris defines r{rt}, expected r{reg}"]
    base, notes = _recognize_gpr_before(
      instructions, index, ra, max_lookback=max_lookback,
      readonly_words=readonly_words, depth=depth + 1,
    )
    if base is None:
      return None, notes or [f"oris r{rt}, r{ra}, {imm} base not recognized"]
    if isinstance(base, TripConstant):
      return TripConstant(base.value | ((int(imm) & 0xFFFF) << 16)), notes
    return None, ["oris trip expression requires concrete low half"]

  if opcode == Opcode.ANDI_DOT:
    rt, ra, imm = (int(v) for v in insn.operands)
    if rt != reg:
      return None, [f"andi. defines r{rt}, expected r{reg}"]
    mask = int(imm) & 0xFFFF
    base, notes = _recognize_gpr_before(
      instructions, index, ra, max_lookback=max_lookback,
      readonly_words=readonly_words, depth=depth + 1,
    )
    if base is None and rt == ra:
      base = TripEntryReg(ra)
      notes = []
    if base is None:
      return None, notes or [f"andi. r{rt}, r{ra}, {imm} source not recognized"]
    return TripAnd(base, TripConstant(mask)), notes

  if opcode == Opcode.ANDIS_DOT:
    rt, ra, imm = (int(v) for v in insn.operands)
    if rt != reg:
      return None, [f"andis. defines r{rt}, expected r{reg}"]
    mask = (int(imm) & 0xFFFF) << 16
    base, notes = _recognize_gpr_before(
      instructions, index, ra, max_lookback=max_lookback,
      readonly_words=readonly_words, depth=depth + 1,
    )
    if base is None and rt == ra:
      base = TripEntryReg(ra)
      notes = []
    if base is None:
      return None, notes or [f"andis. r{rt}, r{ra}, {imm} source not recognized"]
    return TripAnd(base, TripConstant(mask)), notes

  if opcode == Opcode.RLWINM:
    ra, rs, sh, mb, me = (int(v) for v in insn.operands)
    if ra != reg:
      return None, [f"rlwinm defines r{ra}, expected r{reg}"]
    shift = _srwi_shift_amount(sh, mb, me)
    if shift is None:
      return None, ["rlwinm trip expression is not an srwi form"]
    base, notes = _recognize_gpr_before(
      instructions, index, rs, max_lookback=max_lookback,
      readonly_words=readonly_words, depth=depth + 1,
    )
    if base is None:
      base = TripEntryReg(rs)
      notes = []
    return TripLshr(base, shift), notes

  if opcode == Opcode.OR:
    rt, ra, rb = (int(v) for v in insn.operands)
    if rt != reg:
      return None, [f"or defines r{rt}, expected r{reg}"]
    if ra != rb:
      return None, ["or trip expression is not a register copy"]
    base, notes = _recognize_gpr_before(
      instructions, index, ra, max_lookback=max_lookback,
      readonly_words=readonly_words, depth=depth + 1,
    )
    if base is None:
      # Register-copy materialization from an entry register (``mr rT, rS``):
      # the source becomes the entry-state value, exactly like the rlwinm /
      # andi. fallbacks below.
      base = TripEntryReg(ra)
      notes = []
    return base, notes

  if opcode == Opcode.XOR:
    rt, ra, rb = (int(v) for v in insn.operands)
    if rt == reg and ra == rb:
      return TripConstant(0), []
    return None, ["xor trip expression is not self-clearing"]

  if opcode == Opcode.LWZ:
    rt, ra, disp = (int(v) for v in insn.operands)
    if rt != reg:
      return None, [f"lwz defines r{rt}, expected r{reg}"]
    if readonly_words is None:
      return None, ["lwz trip load requires proven readonly constant (unsupported)"]
    base, notes = _recognize_gpr_before(
      instructions, index, ra, max_lookback=max_lookback,
      readonly_words=readonly_words, depth=depth + 1,
    )
    if ra != 0 and base is None:
      return None, notes or [f"lwz r{rt}, {disp}(r{ra}) base not recognized"]
    address = _effective_lwz_address(ra, disp, base)
    if address is None:
      return None, notes or [f"lwz address for r{rt} not concrete"]
    word = readonly_words.get(address)
    if word is None:
      return None, [f"lwz address 0x{address:08X} not in readonly image"]
    return TripConstant(int(word)), notes

  return None, [f"unsupported trip-expression opcode {opcode.value}"]


def _recognize_gpr_before(
  instructions: Sequence[Instruction],
  use_index: int,
  reg: int,
  *,
  max_lookback: int,
  readonly_words: Mapping[int, int] | None,
  depth: int,
) -> tuple[TripExpr | None, list[str]]:
  if reg == 0:
    return TripConstant(0), []
  return recognize_trip_expr(
    instructions,
    use_index,
    reg,
    max_lookback=max_lookback,
    readonly_words=readonly_words,
  )


def _andi_mask(expr: TripExpr) -> int | None:
  expr = normalize_trip_expr(expr)
  if not isinstance(expr, TripAnd):
    return None
  mask_expr = expr.right if isinstance(expr.right, TripConstant) else (
    expr.left if isinstance(expr.left, TripConstant) else None
  )
  if not isinstance(mask_expr, TripConstant):
    return None
  mask = int(mask_expr.value) & 0xFFFFFFFF
  # PR11: only small power-of-two-minus-one remainder masks.
  if mask in (3, 7, 15, 31):
    return mask
  if mask <= 7:
    return mask
  return None


def _srwi_shift_amount(sh: int, mb: int, me: int) -> int | None:
  if me != 31:
    return None
  if not 1 <= mb <= 31:
    return None
  if sh != ((32 - mb) & 31):
    return None
  return mb


def _defines_gpr(insn: Instruction, reg: int) -> bool:
  opcode = insn.opcode
  if opcode in (
    Opcode.ADDI,
    Opcode.ADDIS,
    Opcode.ORI,
    Opcode.ORIS,
    Opcode.ANDI_DOT,
    Opcode.ANDIS_DOT,
    Opcode.RLWINM,
    Opcode.LWZ,
    Opcode.LWZU,
  ):
    return int(insn.operands[0]) == reg
  if opcode in (Opcode.OR, Opcode.XOR):
    return int(insn.operands[0]) == reg
  return False


def _effective_lwz_address(
  ra: int,
  disp: int,
  base_expr: TripExpr | None,
) -> int | None:
  if ra == 0:
    return _sign_extend_16(disp) & 0xFFFFFFFF
  if not isinstance(base_expr, TripConstant):
    return None
  return (base_expr.value + _sign_extend_16(disp)) & 0xFFFFFFFF


def _sign_extend_16(value: int) -> int:
  word = int(value) & 0xFFFF
  if word >= 0x8000:
    word -= 0x10000
  return word & 0xFFFFFFFF
