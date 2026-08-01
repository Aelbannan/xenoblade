"""Skip-guard recognition and SMT discharge for counted loops (doc 30 Phase B).

A *skip guard* is a conditional branch on the straight-line path between a
loop's ``mtctr`` materialization and its header that, when taken, skips the
entire loop. For ``bdnz`` counted loops this is the only sound way to handle a
zero trip count: ``bdnz`` with ``CTR == 0`` wraps to ``0xFFFFFFFF`` iterations,
so a summary may only assume ``trip >= 1`` when the guard provably skips the
loop exactly when the trip is zero.

Two retail shapes are recognized (verified against ``build/us/asm``):

* **Guard between mtctr and header** — ``[srwi. rT][mtctr rT][beq skip][header]``
* **Guard before mtctr** — ``[andi. rT][beq skip][mtctr rT][(nop)][header]``

Recognition is purely syntactic (no solver) and unifies both shapes by
requiring, over the window between the trip-register definition and the
header:

* exactly one conditional CR0-test branch (the guard) plus ``mtctr`` plus
  padding from ``{nop, cror, crand, mcrf, mr}`` — nothing else;
* the guard reads CR0 last written by the trip-definition dot-form
  (``andi.`` / ``andis.`` / ``rlwinm.``), so the guard condition is exactly
  the branch decode applied to the symbolic trip expression;
* the guard target can never re-enter the loop (``header`` unreachable from
  the target in the CFG) — a guard that jumps into the loop body is not a
  skip guard (the plan's loop-rotation check; implemented as reachability
  rather than the stricter "dominates the unique exit" so bulk loops whose
  guard jumps to a *later* loop, e.g. ``__AXVPBInitCommon``'s bulk loop,
  still validate).

The SMT discharge runs at apply/discharge time against the symbolic entry
state and proves two independent UNSAT checks:

    check 1: guard_taken AND trip >= 1   -- UNSAT  (guard_taken => trip == 0)
    check 2: NOT guard_taken AND trip == 0 -- UNSAT (reach_header => trip >= 1)

Together they establish ``guard_taken <=> trip == 0``. ``trip >= 1`` is
expressed as ``trip != 0`` because the ``bdnz`` zero-trip hazard is exactly
``CTR == 0``. Neither check alone authorizes anything; the summary
application additionally records a premise-violation terminal under
``~entry_guard`` so every path the guard does not cover stays fail-closed.
"""

from __future__ import annotations

from collections.abc import Mapping, Sequence
from dataclasses import dataclass
from typing import Any

from tools.ppc_equivalence.deadline import Deadline
from tools.ppc_equivalence.discharge import UnsatDischarge, discharge_bad_conditions
from tools.ppc_equivalence.ir import Instruction, Opcode
from tools.ppc_equivalence.loop_cfg import analyze_loop_cfg
from tools.ppc_equivalence.trip_expression import TripExpr, evaluate_symbolic

_CTR_SPR = 9

# Branch families: BO bits 2..4 (masking the prediction-hint bits) select
# branch-if-true (12) vs branch-if-false (4); BI 0 = CR0.LT, 1 = CR0.GT,
# 2 = CR0.EQ. The hint bits yield BO values 13/15 and 5/7 alongside 12 and 4.
_BO_MASK = 0x1C
_BO_TRUE = 12
_BO_FALSE = 4
_CR0_LT_BI = 0
_CR0_GT_BI = 1
_CR0_EQ_BI = 2

# Instructions allowed between the trip definition / mtctr and the header,
# besides the single guard branch. ``nop`` is ``ori r0, r0, 0`` (decoded as
# ORI) and ``mr`` is ``or rD, rS, rS`` (decoded as OR with equal sources).
# ``mcrf`` is not decodable by the engine decoder, so it is omitted here.
_PADDING_OPCODES = frozenset({Opcode.ORI, Opcode.OR, Opcode.CROR, Opcode.CRAND})

# Trip-definition dot-forms whose result both defines the trip register and
# sets CR0 (the guard's condition source). Mirrors the opcodes
# ``trip_expression.recognize_trip_expr`` accepts for AND/ANDIS/RLWINM.
_DOT_FORM_OPCODES = frozenset({Opcode.ANDI_DOT, Opcode.ANDIS_DOT})

# CR-transfer opcodes whose destination can be CR0 and that therefore clobber
# the guard's condition if executed between the dot-form and the guard.
_CR_WRITE_OPCODES = frozenset({
    Opcode.CRAND,
    Opcode.CRANDC,
    Opcode.CREQV,
    Opcode.CRNAND,
    Opcode.CRNOR,
    Opcode.CROR,
    Opcode.CRORC,
    Opcode.CRXOR,
})

_MAX_MTCTR_LOOKBACK = 4


@dataclass(frozen=True)
class SkipGuardInfo:
    """Syntactic description of a recognized skip guard (no solver involved)."""

    guard_index: int
    bo: int
    bi: int
    target_pc: int
    family: str  # "beq" | "bne" | "blt" | "bgt" | "ble" | "bge"
    mtctr_index: int
    header_pc: int
    trip_def_index: int
    trip_reg: int

    def to_dict(self) -> dict[str, Any]:
        return {
            "guard_index": int(self.guard_index),
            "bo": int(self.bo),
            "bi": int(self.bi),
            "target_pc": int(self.target_pc) & 0xFFFFFFFF,
            "family": self.family,
            "mtctr_index": int(self.mtctr_index),
            "header_pc": int(self.header_pc) & 0xFFFFFFFF,
            "trip_def_index": int(self.trip_def_index),
            "trip_reg": int(self.trip_reg),
        }


@dataclass(frozen=True)
class SkipGuardDischarge:
    """Two-UNSAT guard discharge: ``guard_taken <=> trip == 0``."""

    guard_taken_implies_trip_zero: UnsatDischarge
    not_guard_taken_implies_trip_positive: UnsatDischarge

    def all_unsat(self) -> bool:
        return (
            self.guard_taken_implies_trip_zero.status == "unsat"
            and self.not_guard_taken_implies_trip_positive.status == "unsat"
        )

    def to_dict(self) -> dict[str, Any]:
        return {
            "guard_taken_implies_trip_zero": self.guard_taken_implies_trip_zero.to_obligation_digest(),
            "not_guard_taken_implies_trip_positive": self.not_guard_taken_implies_trip_positive.to_obligation_digest(),
        }


def is_mtctr(insn: Instruction) -> bool:
    return (
        insn.opcode == Opcode.MTSPR
        and len(insn.operands) == 2
        and int(insn.operands[1]) == _CTR_SPR
    )


def find_mtctr_with_guard(
    instructions: Sequence[Instruction],
    header_index: int,
    *,
    max_lookback: int = _MAX_MTCTR_LOOKBACK,
) -> tuple[int | None, SkipGuardInfo | None]:
    """Locate the ``mtctr`` feeding ``header_index`` (nearest first).

    The ``mtctr`` may sit at ``header_index - 1..-max_lookback``; every
    instruction between ``mtctr`` and the header must be padding or the single
    skip-guard branch (grammar check). Returns ``(mtctr_index, guard)`` or
    ``(None, None)`` when no valid adjacency exists.
    """
    if header_index <= 0:
        return None, None
    for distance in range(1, max_lookback + 1):
        mtctr_index = header_index - distance
        if mtctr_index < 0:
            return None, None
        if not is_mtctr(instructions[mtctr_index]):
            continue
        between = instructions[mtctr_index + 1:header_index]
        # Between mtctr and the header: padding plus at most one conditional
        # CR0-test branch (the guard, retail shape 1). Anything else rejects
        # the adjacency; ``find_skip_guard`` re-validates the full window.
        branches = [insn for insn in between if _is_cr0_test_branch(insn)]
        if len(branches) > 1:
            continue
        if any(
            not _is_padding(insn) and not _is_cr0_test_branch(insn)
            for insn in between
        ):
            continue
        guard = find_skip_guard(instructions, header_index, mtctr_index)
        return mtctr_index, guard
    return None, None


def find_skip_guard(
    instructions: Sequence[Instruction],
    header_index: int,
    mtctr_index: int,
) -> SkipGuardInfo | None:
    """Recognize the skip guard for a counted loop (syntactic, fail-closed).

    Looks in the window ``(trip_def, header)`` (which contains ``mtctr`` for
    either retail shape) for exactly one conditional CR0-test branch, with
    every other instruction being ``mtctr`` or padding. The guard must read
    CR0 last written by the trip-definition dot-form so its condition is the
    branch decode of the trip expression.
    """
    mtctr = instructions[mtctr_index]
    if not is_mtctr(mtctr):
        return None
    trip_reg = int(mtctr.operands[0])
    trip_def_index = _find_trip_def_index(instructions, mtctr_index, trip_reg)
    if trip_def_index is None:
        return None

    branch_indices = [
        index
        for index in range(trip_def_index + 1, header_index)
        if index != mtctr_index and _is_cr0_test_branch(instructions[index])
    ]
    if len(branch_indices) != 1:
        return None
    guard_index = branch_indices[0]

    for index in range(trip_def_index + 1, header_index):
        if index == mtctr_index or index == guard_index:
            continue
        if not _is_padding(instructions[index]):
            return None

    guard = instructions[guard_index]
    bo, bi, target, aa = (int(v) for v in guard.operands)
    if aa != 0:
        return None
    family = guard_family(bo, bi)
    if family is None:
        return None

    # The guard's CR0 must be last written by the trip-definition dot-form.
    if not _is_trip_def_dot_form(instructions[trip_def_index]):
        return None
    if any(
        _writes_cr0(instructions[index])
        for index in range(trip_def_index + 1, guard_index)
    ):
        return None

    header_pc = instructions[header_index].address
    info = SkipGuardInfo(
        guard_index=guard_index,
        bo=bo,
        bi=bi,
        target_pc=int(target) & 0xFFFFFFFC,
        family=family,
        mtctr_index=mtctr_index,
        header_pc=header_pc,
        trip_def_index=trip_def_index,
        trip_reg=trip_reg,
    )
    if not guard_skips_loop(instructions, info):
        return None
    return info


def guard_family(bo: int, bi: int) -> str | None:
    """Decode ``(bo, bi)`` into a CR0 branch family, or ``None``."""
    bo = int(bo)
    bi = int(bi)
    kind = bo & _BO_MASK
    if kind == _BO_TRUE:
        if bi == _CR0_LT_BI:
            return "blt"
        if bi == _CR0_GT_BI:
            return "bgt"
        if bi == _CR0_EQ_BI:
            return "beq"
        return None
    if kind == _BO_FALSE:
        if bi == _CR0_LT_BI:
            return "bge"
        if bi == _CR0_GT_BI:
            return "ble"
        if bi == _CR0_EQ_BI:
            return "bne"
        return None
    return None


def guard_taken_formula(family: str, value: Any, ops: Any) -> Any:
    """The branch-taken condition for ``family`` given the trip value."""
    zero = ops.const(0)
    if family == "beq":
        return ops.eq(value, zero)
    if family == "bne":
        return ops.lnot(ops.eq(value, zero))
    if family == "blt":
        return ops.signed_lt(value, zero)
    if family == "bgt":
        return ops.signed_lt(zero, value)
    if family == "ble":
        return ops.bor(ops.eq(value, zero), ops.signed_lt(value, zero))
    if family == "bge":
        return ops.lnot(ops.signed_lt(value, zero))
    raise ValueError(f"unknown guard family {family!r}")


def discharge_skip_guard(
    guard: SkipGuardInfo,
    trip_expr: TripExpr,
    entry_gpr: Mapping[int, Any],
    ops: Any,
    *,
    deadline: Deadline | None = None,
    z3_module: Any | None = None,
) -> SkipGuardDischarge | None:
    """Prove ``guard_taken <=> trip == 0`` via two independent UNSAT queries.

    ``trip_expr`` is evaluated against ``entry_gpr`` (the symbolic header-entry
    state) with ``ops``. Returns ``None`` only when the query cannot be stated
    (never happens after syntactic recognition); a failed discharge has at
    least one non-UNSAT check and must be treated as fail-closed.
    """
    value = evaluate_symbolic(trip_expr, entry_gpr, ops)
    guard_taken = guard_taken_formula(guard.family, value, ops)
    trip_zero = ops.eq(value, ops.const(0))
    trip_positive = ops.lnot(trip_zero)

    if z3_module is None:
        import z3 as z3_module  # type: ignore[no-redef]
    if deadline is None:
        deadline = Deadline.after_ms(15_000)

    check_1 = discharge_bad_conditions(
        premises=[],
        bad_conditions=[ops.land(guard_taken, trip_positive)],
        deadline=deadline,
        algorithm="skip-guard-v1:guard-taken-implies-trip-zero",
        z3_module=z3_module,
    )
    check_2 = discharge_bad_conditions(
        premises=[],
        bad_conditions=[ops.land(ops.lnot(guard_taken), trip_zero)],
        deadline=deadline,
        algorithm="skip-guard-v1:not-guard-taken-implies-trip-positive",
        z3_module=z3_module,
    )
    return SkipGuardDischarge(
        guard_taken_implies_trip_zero=check_1,
        not_guard_taken_implies_trip_positive=check_2,
    )


def guard_skips_loop(
    instructions: Sequence[Instruction],
    guard: SkipGuardInfo,
) -> bool:
    """True when the guard target cannot re-enter the loop (loop-rotation check).

    The header must be unreachable from the guard target in the static CFG.
    A guard jumping into the loop body (e.g. to the latch, which would wrap
    ``CTR == 0`` under ``bdnz``) is rejected.
    """
    if guard.target_pc == guard.header_pc:
        return False
    cfg = analyze_loop_cfg(instructions)
    if cfg.unsupported_reason is not None:
        return False
    visited: set[int] = set()
    stack = [guard.target_pc]
    while stack:
        pc = stack.pop()
        if pc in visited:
            continue
        visited.add(pc)
        if pc == guard.header_pc:
            return False
        stack.extend(cfg.successors.get(pc, ()))
    return True


def _is_padding(insn: Instruction) -> bool:
    """True for ``nop`` / ``mr`` / ``cror`` / ``crand``."""
    if insn.opcode == Opcode.ORI:
        rt, ra, imm = (int(v) for v in insn.operands)
        return rt == 0 and ra == 0 and imm == 0
    if insn.opcode == Opcode.OR:
        rt, ra, rb = (int(v) for v in insn.operands)
        return rt == ra and ra == rb
    return insn.opcode in (Opcode.CROR, Opcode.CRAND)


def _is_cr0_test_branch(insn: Instruction) -> bool:
    if insn.opcode != Opcode.BC or insn.link:
        return False
    bo, bi, _target, _aa = (int(v) for v in insn.operands)
    return guard_family(bo, bi) is not None


def _is_trip_def_dot_form(insn: Instruction) -> bool:
    """True when ``insn`` defines the trip register and sets CR0 from its result."""
    if insn.opcode in _DOT_FORM_OPCODES:
        return True
    if insn.opcode == Opcode.RLWINM:
        return bool(insn.record)
    return False


def _writes_cr0(insn: Instruction) -> bool:
    """True when ``insn`` may write a bit of CR0."""
    if insn.opcode in (Opcode.ANDI_DOT, Opcode.ANDIS_DOT) or insn.record:
        return True
    if insn.opcode in (Opcode.CMPWI, Opcode.CMPLWI, Opcode.CMPW, Opcode.CMPLW):
        return int(insn.operands[0]) == 0
    if insn.opcode in _CR_WRITE_OPCODES:
        return int(insn.operands[0]) in (0, 1, 2, 3)
    if insn.opcode == Opcode.MTCRF:
        return bool(int(insn.operands[0]) & 0x80)
    return False


def _find_trip_def_index(
    instructions: Sequence[Instruction],
    mtctr_index: int,
    trip_reg: int,
) -> int | None:
    """Index of the last instruction defining ``trip_reg`` before ``mtctr``."""
    for index in range(mtctr_index - 1, -1, -1):
        if _defines_gpr(instructions[index], trip_reg):
            return index
    return None


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
