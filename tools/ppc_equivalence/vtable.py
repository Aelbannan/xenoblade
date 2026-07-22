"""Conservative MWCC-style virtual-call pattern recognition (descriptors only).

Recognizes the linked indirect dispatch shape::

    lwz vptr, off(this)
    lwz target, slot(vptr)
    mtctr target
    bctrl

and the tail-call thunk shape (``bctr``, LK=0)::

    lwz[/u] vptr, off(this)
    lwz target, slot(vptr)
    mtctr target
    bctr

Jump-table recognition (``jump_table.find_jump_table_candidates``) stays limited
to ``cmplwi`` / ``lwzx`` / ``bctr`` tables; plain virtual thunks use
:func:`find_virtual_thunk_candidates`.
"""

from __future__ import annotations

from collections.abc import Sequence
from dataclasses import dataclass

from .ir import Instruction, Opcode

_CTR_SPR = 9


@dataclass(frozen=True)
class VirtualCallCandidate:
    branch_pc: int
    this_reg: int
    this_offset: int
    vptr_reg: int
    slot_offset: int
    target_reg: int
    vptr_load_pc: int
    slot_load_pc: int
    mtctr_pc: int
    instruction_indexes: tuple[int, ...]
    confidence: str
    notes: tuple[str, ...]
    tail_call: bool = False


def find_virtual_call_candidates(
    instructions: Sequence[Instruction],
) -> list[VirtualCallCandidate]:
    """Scan decoded instructions for lwz/lwz/mtctr/bctrl virtual calls.

    Returns an empty list when nothing matches; never raises on unmatched code.
    """
    return _find_virtual_dispatch_candidates(instructions, require_link=True)


def find_virtual_thunk_candidates(
    instructions: Sequence[Instruction],
) -> list[VirtualCallCandidate]:
    """Scan for MWCC virtual thunks ending in ``mtctr`` / ``bctr`` (LK=0).

    Accepts ``lwz`` or ``lwzu`` for the vptr load so forms like
    ``lwzu r12, off(r3); lwz r12, slot(r12); mtctr r12; bctr`` match.
    """
    return _find_virtual_dispatch_candidates(
        instructions,
        require_link=False,
        allow_lwzu_vptr=True,
    )


def _find_virtual_dispatch_candidates(
    instructions: Sequence[Instruction],
    *,
    require_link: bool,
    allow_lwzu_vptr: bool = False,
) -> list[VirtualCallCandidate]:
    if not instructions:
        return []

    candidates: list[VirtualCallCandidate] = []
    for branch_index, branch in enumerate(instructions):
        if require_link:
            if not _is_linked_bctrl(branch):
                continue
        else:
            if not _is_unlinked_bctr(branch):
                continue
        tail = _match_virtual_call_tail(
            instructions,
            branch_index,
            allow_lwzu_vptr=allow_lwzu_vptr,
        )
        if tail is None:
            continue
        notes: list[str] = []
        if not _register_chain_is_clean(
            instructions,
            tail.vptr_load_index,
            tail.vptr_reg,
            tail.slot_load_index,
        ):
            notes.append("unmodeled vptr modification between vptr load and slot load")
            continue
        if not _register_chain_is_clean(
            instructions,
            tail.slot_load_index,
            tail.target_reg,
            tail.mtctr_index,
        ):
            notes.append("unmodeled target modification between slot load and mtctr")
            continue
        instruction_indexes = (
            tail.vptr_load_index,
            tail.slot_load_index,
            tail.mtctr_index,
            branch_index,
        )
        candidates.append(
            VirtualCallCandidate(
                branch_pc=branch.address,
                this_reg=tail.this_reg,
                this_offset=tail.this_offset,
                vptr_reg=tail.vptr_reg,
                slot_offset=tail.slot_offset,
                target_reg=tail.target_reg,
                vptr_load_pc=instructions[tail.vptr_load_index].address,
                slot_load_pc=instructions[tail.slot_load_index].address,
                mtctr_pc=instructions[tail.mtctr_index].address,
                instruction_indexes=instruction_indexes,
                confidence="exact-pattern",
                notes=tuple(notes),
                tail_call=not require_link,
            ),
        )
    return candidates


@dataclass(frozen=True)
class _VirtualCallTail:
    vptr_load_index: int
    slot_load_index: int
    mtctr_index: int
    this_reg: int
    this_offset: int
    vptr_reg: int
    slot_offset: int
    target_reg: int


def _match_virtual_call_tail(
    instructions: Sequence[Instruction],
    branch_index: int,
    *,
    allow_lwzu_vptr: bool = False,
) -> _VirtualCallTail | None:
    if branch_index < 3:
        return None

    mtctr = instructions[branch_index - 1]
    if not _is_mtctr(mtctr):
        return None

    slot_load = instructions[branch_index - 2]
    if slot_load.opcode != Opcode.LWZ:
        return None

    vptr_load = instructions[branch_index - 3]
    if vptr_load.opcode == Opcode.LWZ:
        pass
    elif allow_lwzu_vptr and vptr_load.opcode == Opcode.LWZU:
        pass
    else:
        return None

    target_reg, vptr_reg, slot_offset = slot_load.operands
    target_reg = int(target_reg)
    vptr_reg = int(vptr_reg)
    slot_offset = int(slot_offset)

    if target_reg != mtctr.operands[0]:
        return None
    if vptr_reg != slot_load.operands[1]:
        return None

    vptr_reg_def, this_reg, this_offset = vptr_load.operands
    vptr_reg_def = int(vptr_reg_def)
    this_reg = int(this_reg)
    this_offset = int(this_offset)
    if vptr_reg_def != vptr_reg:
        return None

    return _VirtualCallTail(
        vptr_load_index=branch_index - 3,
        slot_load_index=branch_index - 2,
        mtctr_index=branch_index - 1,
        this_reg=this_reg,
        this_offset=this_offset,
        vptr_reg=vptr_reg,
        slot_offset=slot_offset,
        target_reg=target_reg,
    )


def _register_chain_is_clean(
    instructions: Sequence[Instruction],
    def_index: int,
    reg: int,
    use_index: int,
) -> bool:
    """Return True when ``reg`` is not clobbered between def and use indices."""
    for index in range(def_index + 1, use_index):
        if _clobbers_gpr(instructions[index], reg):
            return False
    return True


def _clobbers_gpr(insn: Instruction, reg: int) -> bool:
    opcode = insn.opcode
    if opcode in (
        Opcode.ADDI,
        Opcode.ADDIS,
        Opcode.ADDIC,
        Opcode.ADDIC_DOT,
        Opcode.SUBFIC,
        Opcode.MULLI,
        Opcode.ORI,
        Opcode.ORIS,
        Opcode.XORI,
        Opcode.XORIS,
        Opcode.ANDI_DOT,
        Opcode.ANDIS_DOT,
        Opcode.RLWINM,
        Opcode.RLWIMI,
        Opcode.RLWNM,
        Opcode.LWZ,
        Opcode.LWZU,
        Opcode.LHA,
        Opcode.LHZ,
        Opcode.LBZ,
        Opcode.ADD,
        Opcode.ADDC,
        Opcode.SUBF,
        Opcode.SUBFC,
        Opcode.MULLW,
        Opcode.MULHW,
        Opcode.MULHWU,
        Opcode.AND,
        Opcode.ANDC,
        Opcode.OR,
        Opcode.ORC,
        Opcode.XOR,
        Opcode.NAND,
        Opcode.NOR,
        Opcode.SLW,
        Opcode.SRW,
        Opcode.SRAW,
        Opcode.SRAWI,
        Opcode.NEG,
        Opcode.EXTSH,
        Opcode.EXTSB,
    ):
        return int(insn.operands[0]) == reg
    return False


def _is_linked_bctrl(insn: Instruction) -> bool:
    if insn.opcode != Opcode.BCCTR or not insn.link:
        return False
    bo = insn.operands[0]
    return bool(bo & 0b10000)


def _is_unlinked_bctr(insn: Instruction) -> bool:
    if insn.opcode != Opcode.BCCTR or insn.link:
        return False
    bo = insn.operands[0]
    return bool(bo & 0b10000)


def _is_mtctr(insn: Instruction) -> bool:
    return (
        insn.opcode == Opcode.MTSPR
        and len(insn.operands) == 2
        and insn.operands[1] == _CTR_SPR
    )
