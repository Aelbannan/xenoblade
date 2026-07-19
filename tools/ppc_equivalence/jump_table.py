"""Conservative MWCC-style jump-table pattern recognition (descriptors only)."""

from __future__ import annotations

from collections.abc import Sequence
from dataclasses import dataclass

from .ir import Instruction, Opcode

_CTR_SPR = 9
_MAX_CMPLWI_LOOKBACK = 8


@dataclass(frozen=True)
class JumpTableCandidate:
    branch_pc: int
    index_reg: int
    table_base_expr: str | None
    bound_imm: int | None
    load_pc: int | None
    mtctr_pc: int | None
    instruction_indexes: tuple[int, ...]
    confidence: str
    notes: tuple[str, ...]


def find_jump_table_candidates(instructions: Sequence[Instruction]) -> list[JumpTableCandidate]:
    """Scan decoded instructions for cmplwi/slwi/lwzx/mtctr/bctr jump tables.

    Returns an empty list when nothing matches; never raises on unmatched code.
    """
    if not instructions:
        return []

    candidates: list[JumpTableCandidate] = []
    for branch_index, branch in enumerate(instructions):
        if not _is_unconditional_bctr(branch):
            continue
        tail = _match_jump_table_tail(instructions, branch_index)
        if tail is None:
            continue
        cmplwi_index, bound_imm = _find_cmplwi_bounds(
            instructions,
            tail.index_reg,
            start=tail.first_index,
        )
        instruction_indexes = _ordered_indexes(
            cmplwi_index,
            tail.shift_index,
            tail.load_index,
            tail.mtctr_index,
            branch_index,
        )
        confidence = "exact-pattern" if cmplwi_index is not None and tail.shift_index is not None else "partial"
        notes: list[str] = []
        if cmplwi_index is None:
            notes.append("missing cmplwi bounds check")
        if tail.shift_index is None:
            notes.append("missing index scale shift")
        candidates.append(
            JumpTableCandidate(
                branch_pc=branch.address,
                index_reg=tail.index_reg,
                table_base_expr=tail.table_base_expr,
                bound_imm=bound_imm,
                load_pc=instructions[tail.load_index].address if tail.load_index is not None else None,
                mtctr_pc=instructions[tail.mtctr_index].address,
                instruction_indexes=instruction_indexes,
                confidence=confidence,
                notes=tuple(notes),
            ),
        )
    return candidates


@dataclass(frozen=True)
class _JumpTableTail:
    first_index: int
    shift_index: int | None
    load_index: int
    mtctr_index: int
    index_reg: int
    table_base_expr: str | None


def _match_jump_table_tail(
    instructions: Sequence[Instruction],
    branch_index: int,
) -> _JumpTableTail | None:
    if branch_index < 2:
        return None

    mtctr = instructions[branch_index - 1]
    if not _is_mtctr(mtctr):
        return None

    load_index = branch_index - 2
    load = instructions[load_index]
    if load.opcode != Opcode.LWZX:
        return None

    load_rt, load_ra, load_rb = load.operands
    if load_rt != mtctr.operands[0]:
        return None

    shift_index: int | None = None
    index_reg = load_rb
    first_index = load_index

    if load_index >= 1:
        shift = instructions[load_index - 1]
        parsed = _parse_left_shift(shift)
        if parsed is not None:
            source_reg, dest_reg, _scale = parsed
            if dest_reg == load_rb and source_reg == dest_reg:
                shift_index = load_index - 1
                index_reg = source_reg
                first_index = shift_index

    table_base_expr = f"r{load_ra}"
    if load.relocation is not None:
        table_base_expr = (
            f"{load.relocation.canonical_symbol}"
            f"+r{load_ra}+r{load_rb}"
        )

    return _JumpTableTail(
        first_index=first_index,
        shift_index=shift_index,
        load_index=load_index,
        mtctr_index=branch_index - 1,
        index_reg=index_reg,
        table_base_expr=table_base_expr,
    )


def _find_cmplwi_bounds(
    instructions: Sequence[Instruction],
    index_reg: int,
    *,
    start: int,
) -> tuple[int | None, int | None]:
    lower = max(0, start - _MAX_CMPLWI_LOOKBACK)
    for index in range(start - 1, lower - 1, -1):
        insn = instructions[index]
        if insn.opcode != Opcode.CMPLWI:
            continue
        bf, ra, imm = insn.operands
        if bf != 0 or ra != index_reg:
            continue
        return index, imm
    return None, None


def _ordered_indexes(*indexes: int | None) -> tuple[int, ...]:
    return tuple(index for index in indexes if index is not None)


def _is_unconditional_bctr(insn: Instruction) -> bool:
    if insn.opcode != Opcode.BCCTR or insn.link:
        return False
    bo = insn.operands[0]
    return bool(bo & 0b10000)


def _is_mtctr(insn: Instruction) -> bool:
    return insn.opcode == Opcode.MTSPR and len(insn.operands) == 2 and insn.operands[1] == _CTR_SPR


def _parse_left_shift(insn: Instruction) -> tuple[int, int, int] | None:
    """Return (source_reg, dest_reg, scale) for a left-shift by a constant amount."""
    if insn.opcode == Opcode.SLW:
        ra, rt, rb = insn.operands
        return ra, rt, rb

    if insn.opcode != Opcode.RLWINM:
        return None

    ra, rt, sh, mb, me = insn.operands
    if mb != 0:
        return None
    if me != 31 - sh:
        return None
    return ra, rt, sh
