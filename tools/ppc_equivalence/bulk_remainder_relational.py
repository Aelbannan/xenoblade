"""Bulk + remainder relational loop scaffolding (PR12).

Conservative pairing of ``N >> k`` bulk loops with ``N & ((1<<k)-1)`` remainder
loops that share entry register, width, stride, base, source, and count shape.
Algebraic identity and remainder bounds are checked; discharge stays scaffold-only
under the PR0 freeze.
"""

from __future__ import annotations

from collections.abc import Sequence
from dataclasses import dataclass
from typing import Any

from tools.ppc_equivalence.memory_loop import ConstantStrideStoreLoop, find_constant_stride_store_loops
from tools.ppc_equivalence.ir import Instruction
from tools.ppc_equivalence.relational_induction import (
  ExitAgreementObligation,
  HoudiniTemplateName,
  InitiationObligation,
  InvariantTemplateRef,
  PostconditionObligation,
  PreservationObligation,
  RelationalInductionSketch,
  RelationalLoopSide,
  TerminationObligation,
)
from tools.ppc_equivalence.trip_expression import (
  TripAnd,
  TripConstant,
  TripEntryReg,
  TripLshr,
  bulk_shift_amount,
  canonical_dict,
  normalize_trip_expr,
  recognize_trip_expr,
  remainder_mask_for_shift,
  trip_exprs_equal,
)


@dataclass(frozen=True)
class BulkRemainderLoopPair:
  """Recognized bulk + remainder store-loop pair in one function."""

  entry_reg: int
  shift_k: int
  bulk: ConstantStrideStoreLoop
  remainder: ConstantStrideStoreLoop
  identity_notes: tuple[str, ...]


@dataclass(frozen=True)
class BulkRemainderRelationalUnsupported:
  reason: str


def find_bulk_remainder_pairs(
  instructions: Sequence[Instruction],
  *,
  readonly_words: dict[int, int] | None = None,
) -> list[BulkRemainderLoopPair]:
  """Scan constant-stride store loops for bulk+remainder shapes."""
  loops = find_constant_stride_store_loops(
    instructions,
    readonly_words=readonly_words,
  )
  pairs: list[BulkRemainderLoopPair] = []
  for bulk in loops:
    for remainder in loops:
      if bulk.header_pc == remainder.header_pc:
        continue
      pair = _match_bulk_remainder_pair(instructions, bulk, remainder)
      if pair is not None:
        pairs.append(pair)
  return pairs


def prove_bulk_remainder_identity(pair: BulkRemainderLoopPair) -> bool:
  """Check algebraic identity and remainder bound without wrap/overlap anomalies."""
  if pair.bulk.confidence != "exact-pattern":
    return False
  if pair.remainder.confidence not in ("exact-pattern", "partial"):
    return False
  if not _loops_shape_compatible(pair.bulk, pair.remainder):
    return False
  if pair.remainder.trip_count is None:
    return False
  expected_mask = remainder_mask_for_shift(pair.shift_k)
  if pair.remainder.trip_count > expected_mask:
    return False
  chunk = 1 << pair.shift_k
  if pair.remainder.trip_count >= chunk:
    return False
  if pair.bulk.trip_expr is None or pair.remainder.trip_expr is None:
    return False
  from tools.ppc_equivalence.trip_expression import (
    TripAnd,
    TripConstant,
    TripEntryReg,
    TripLshr,
    canonical_dict,
    normalize_trip_expr,
  )

  bulk_expr = normalize_trip_expr(_expr_from_canonical(pair.bulk.trip_expr))
  rem_expr = normalize_trip_expr(_expr_from_canonical(pair.remainder.trip_expr))
  if not isinstance(bulk_expr, TripLshr) or not isinstance(bulk_expr.left, TripEntryReg):
    return False
  if bulk_expr.left.reg != pair.entry_reg or bulk_expr.shift != pair.shift_k:
    return False
  expected_rem = TripAnd(TripEntryReg(pair.entry_reg), TripConstant(expected_mask))
  if canonical_dict(rem_expr) != canonical_dict(expected_rem):
    return False
  return True


def _expr_from_canonical(data: dict[str, Any]) -> Any:
  kind = data.get("kind")
  if kind == "const":
    return TripConstant(int(data["value"]))
  if kind == "entry":
    return TripEntryReg(int(data["reg"]))
  if kind == "and":
    return TripAnd(_expr_from_canonical(data["left"]), _expr_from_canonical(data["right"]))
  if kind == "lshr":
    return TripLshr(_expr_from_canonical(data["left"]), int(data["shift"]))
  if kind == "add":
    from tools.ppc_equivalence.trip_expression import TripAdd

    return TripAdd(_expr_from_canonical(data["left"]), _expr_from_canonical(data["right"]))
  raise ValueError(f"unknown trip_expr kind {kind!r}")


def build_bulk_remainder_relational_sketch(
  original: BulkRemainderLoopPair,
  candidate: BulkRemainderLoopPair,
) -> RelationalInductionSketch | BulkRemainderRelationalUnsupported:
  """Build a pending relational sketch for matched bulk+remainder pairs."""
  if original.shift_k != candidate.shift_k:
    return BulkRemainderRelationalUnsupported(
      f"mismatched bulk shift: {original.shift_k} vs {candidate.shift_k}"
    )
  if original.entry_reg != candidate.entry_reg:
    return BulkRemainderRelationalUnsupported(
      f"mismatched entry register: r{original.entry_reg} vs r{candidate.entry_reg}"
    )
  if not prove_bulk_remainder_identity(original):
    return BulkRemainderRelationalUnsupported("original bulk+remainder pair fails identity")
  if not prove_bulk_remainder_identity(candidate):
    return BulkRemainderRelationalUnsupported("candidate bulk+remainder pair fails identity")
  if not _loops_shape_compatible(original.bulk, candidate.bulk):
    return BulkRemainderRelationalUnsupported("bulk loop bodies differ between sides")
  if not _loops_shape_compatible(original.remainder, candidate.remainder):
    return BulkRemainderRelationalUnsupported("remainder loop bodies differ between sides")

  templates = (
    HoudiniTemplateName.ENTRY_PLUS_K_STRIDE.value,
    HoudiniTemplateName.CTR_REMAINING.value,
    HoudiniTemplateName.REGISTER_EQUALITY.value,
  )
  initiation = (
    InvariantTemplateRef(
      HoudiniTemplateName.REGISTER_EQUALITY.value,
      {
        "register": original.entry_reg,
        "side": "both",
        "role": "count-entry",
      },
    ),
    InvariantTemplateRef(
      HoudiniTemplateName.ENTRY_PLUS_K_STRIDE.value,
      {
        "bulk_shift_k": original.shift_k,
        "remainder_mask": remainder_mask_for_shift(original.shift_k),
        "phase": "initiation",
      },
    ),
  )
  return RelationalInductionSketch(
    original=RelationalLoopSide(
      header_pc=original.bulk.header_pc,
      latch_pc=original.bulk.latch_pc,
      exit_pc=original.remainder.exit_pc,
    ),
    candidate=RelationalLoopSide(
      header_pc=candidate.bulk.header_pc,
      latch_pc=candidate.bulk.latch_pc,
      exit_pc=candidate.remainder.exit_pc,
    ),
    initiation=InitiationObligation(initiation),
    preservation=PreservationObligation(
      (
        InvariantTemplateRef(
          HoudiniTemplateName.CTR_REMAINING.value,
          {
            "bulk_header": original.bulk.header_pc,
            "remainder_header": original.remainder.header_pc,
            "status": "pending",
          },
        ),
      ),
    ),
    exit_agreement=ExitAgreementObligation(
      (
        InvariantTemplateRef(
          HoudiniTemplateName.REGISTER_EQUALITY.value,
          {"scope": "post-remainder", "status": "pending"},
        ),
      ),
    ),
    postcondition=PostconditionObligation(
      (
        InvariantTemplateRef(
          HoudiniTemplateName.CONSTANT_POINTER_DIFF.value,
          {"scope": "final-memory", "status": "pending"},
        ),
      ),
    ),
    termination=TerminationObligation(
      witness="bulk-remainder-ctr-descending",
      notes=(
        f"bulk N>>{original.shift_k} then remainder N&{remainder_mask_for_shift(original.shift_k)}",
        "relational discharge not implemented",
      ),
    ),
    templates=templates,
    status="pending",
    notes=(
      "bulk+remainder relational scaffold",
      *original.identity_notes,
      "discharge blocked under PR0 freeze",
    ),
  )


def try_build_bulk_remainder_relational_sketch(
  original: Sequence[Instruction],
  candidate: Sequence[Instruction],
  *,
  readonly_words: dict[int, int] | None = None,
) -> RelationalInductionSketch | None:
  """Return a bulk+remainder relational sketch when exactly one pair matches per side."""
  left = find_bulk_remainder_pairs(original, readonly_words=readonly_words)
  right = find_bulk_remainder_pairs(candidate, readonly_words=readonly_words)
  if len(left) != 1 or len(right) != 1:
    return None
  built = build_bulk_remainder_relational_sketch(left[0], right[0])
  if isinstance(built, BulkRemainderRelationalUnsupported):
    return None
  return built


def _match_bulk_remainder_pair(
  instructions: Sequence[Instruction],
  bulk: ConstantStrideStoreLoop,
  remainder: ConstantStrideStoreLoop,
) -> BulkRemainderLoopPair | None:
  if not instructions:
    return None

  by_address = {insn.address: index for index, insn in enumerate(instructions)}
  bulk_mtctr = by_address.get(bulk.mtctr_pc)
  rem_mtctr = by_address.get(remainder.mtctr_pc)
  if bulk_mtctr is None or rem_mtctr is None:
    return None

  bulk_expr, _notes = recognize_trip_expr(
    instructions,
    bulk_mtctr,
    bulk.trip_count_reg or 0,
  )
  rem_expr, _rem_notes = recognize_trip_expr(
    instructions,
    rem_mtctr,
    remainder.trip_count_reg or 0,
  )
  if bulk_expr is None or rem_expr is None:
    return None

  bulk_expr = normalize_trip_expr(bulk_expr)
  rem_expr = normalize_trip_expr(rem_expr)
  shift_k = bulk_shift_amount(bulk_expr)
  if shift_k is None:
    return None

  mask = remainder_mask_for_shift(shift_k)
  expected_rem = TripAnd(TripEntryReg(_entry_reg_of(bulk_expr)), TripConstant(mask))
  if not trip_exprs_equal(rem_expr, expected_rem):
    return None

  entry_reg = _entry_reg_of(bulk_expr)
  if entry_reg is None:
    return None

  if not _loops_shape_compatible(bulk, remainder):
    return None

  notes = (
    f"entry r{entry_reg}",
    f"bulk trip {canonical_dict(bulk_expr)}",
    f"remainder trip {canonical_dict(rem_expr)}",
    f"remainder bound <= {mask}",
  )
  return BulkRemainderLoopPair(
    entry_reg=entry_reg,
    shift_k=shift_k,
    bulk=bulk,
    remainder=remainder,
    identity_notes=notes,
  )


def _entry_reg_of(expr: Any) -> int | None:
  expr = normalize_trip_expr(expr)
  if isinstance(expr, TripLshr) and isinstance(expr.left, TripEntryReg):
    return expr.left.reg
  if isinstance(expr, TripAnd) and isinstance(expr.left, TripEntryReg):
    return expr.left.reg
  return None


def _loops_shape_compatible(
  left: ConstantStrideStoreLoop,
  right: ConstantStrideStoreLoop,
) -> bool:
  return (
    left.stride == right.stride
    and left.store_width == right.store_width
    and left.base_reg == right.base_reg
    and left.source_reg == right.source_reg
    and left.store_kind == right.store_kind
  )
