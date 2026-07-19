"""Auto-build ``JumpTableProofContext`` from pattern + linked image hydration.

Pattern recognition alone is not a proof. This module only succeeds when:

1. both sides expose one ``exact-pattern`` candidate with matching bounds/index;
2. the table base VA is recoverable from concrete ``lis``/``addi`` immediates;
3. linked ELF/DOL hydration yields aligned ADDR32 words for ``bound_imm + 1`` entries.
"""

from __future__ import annotations

from collections.abc import Sequence
from pathlib import Path

from tools.ppc_equivalence.ir import Instruction, Opcode
from tools.ppc_equivalence.jump_table import JumpTableCandidate, find_jump_table_candidates
from tools.ppc_equivalence.jump_table_image import (
    JumpTableImageError,
    hydrate_jump_table,
    jump_table_words_from_image,
)
from tools.ppc_equivalence.jump_table_obligations import JumpTableProofContext
from tools.ppc_equivalence.jump_table_pairing import JumpTablePairingError, pair_jump_table_cases

_DEFAULT_BASE_LOOKBACK = 48


def table_base_reg(candidate: JumpTableCandidate, instructions: Sequence[Instruction]) -> int | None:
    """Return the ``lwzx`` base register (``rA``) for a recognized candidate."""
    if candidate.load_pc is None:
        return None
    for insn in instructions:
        if insn.address != candidate.load_pc:
            continue
        if insn.opcode != Opcode.LWZX or len(insn.operands) < 2:
            return None
        return int(insn.operands[1])
    return None


def resolve_table_base_va(
    instructions: Sequence[Instruction],
    candidate: JumpTableCandidate,
    *,
    max_lookback: int = _DEFAULT_BASE_LOOKBACK,
) -> int | None:
    """Recover the linked table VA from ``addis``/``addi`` feeding the ``lwzx`` base.

    Returns ``None`` when immediates are missing, relocated, or not a simple
    ``lis`` + ``addi`` materialization (no SDA / complex arithmetic).
    """
    if candidate.load_pc is None:
        return None
    load_index = next(
        (index for index, insn in enumerate(instructions) if insn.address == candidate.load_pc),
        None,
    )
    if load_index is None:
        return None
    load = instructions[load_index]
    if load.opcode != Opcode.LWZX or len(load.operands) < 2:
        return None
    if load.relocation is not None:
        return None
    base_reg = int(load.operands[1])
    lower = max(0, load_index - max_lookback)

    lo_imm: int | None = None
    mid_reg: int | None = None
    addi_index: int | None = None
    for index in range(load_index - 1, lower - 1, -1):
        insn = instructions[index]
        if insn.relocation is not None:
            continue
        if insn.opcode == Opcode.ADDIS:
            rt, ra, imm = insn.operands
            if rt == base_reg and ra == 0:
                return (int(imm) << 16) & 0xFFFFFFFF
        if insn.opcode in (Opcode.ADDI, Opcode.ADDIC, Opcode.ADDIC_DOT):
            rt, ra, imm = insn.operands
            if rt != base_reg:
                continue
            lo_imm = int(imm)
            mid_reg = int(ra)
            addi_index = index
            break

    if lo_imm is None or mid_reg is None:
        return None
    if mid_reg == 0:
        return lo_imm & 0xFFFFFFFF

    search_end = addi_index if addi_index is not None else load_index
    for index in range(search_end - 1, lower - 1, -1):
        insn = instructions[index]
        if insn.relocation is not None:
            continue
        if insn.opcode != Opcode.ADDIS:
            continue
        rt, ra, imm = insn.operands
        if rt == mid_reg and ra == 0:
            return ((int(imm) << 16) + lo_imm) & 0xFFFFFFFF
    return None


def try_auto_jump_table_context(
    original: Sequence[Instruction],
    candidate: Sequence[Instruction],
    *,
    dol_path: Path | str | None = None,
    elf_path: Path | str | None = None,
) -> JumpTableProofContext | None:
    """Build a proof context when both sides match and the table image hydrates.

    Fail-closed: returns ``None`` rather than raising on incomplete evidence.
    """
    if dol_path is None and elf_path is None:
        return None

    left = find_jump_table_candidates(original)
    right = find_jump_table_candidates(candidate)
    if len(left) != 1 or len(right) != 1:
        return None
    left_c, right_c = left[0], right[0]
    if left_c.confidence != "exact-pattern" or right_c.confidence != "exact-pattern":
        return None
    if left_c.bound_imm is None or left_c.bound_imm != right_c.bound_imm:
        return None
    if left_c.index_reg != right_c.index_reg:
        return None

    base_va = resolve_table_base_va(original, left_c)
    if base_va is None:
        return None
    cand_va = resolve_table_base_va(candidate, right_c)
    if cand_va is not None and cand_va != base_va:
        return None

    base_reg = table_base_reg(left_c, original)
    if base_reg is None:
        return None
    right_base_reg = table_base_reg(right_c, candidate)
    if right_base_reg is not None and right_base_reg != base_reg:
        return None

    entry_count = int(left_c.bound_imm) + 1
    try:
        image = hydrate_jump_table(
            base=base_va,
            entry_count=entry_count,
            elf_path=Path(elf_path) if elf_path is not None else None,
            dol_path=Path(dol_path) if dol_path is not None else None,
        )
    except JumpTableImageError:
        return None
    if any(word & 3 for word in image.words):
        return None

    try:
        pair_jump_table_cases(
            original_words=image.words,
            candidate_words=image.words,
            table_base_original=base_va,
            table_base_candidate=base_va,
        )
    except JumpTablePairingError:
        return None

    table = jump_table_words_from_image(image)
    return JumpTableProofContext(
        table=table,
        branch_pc=left_c.branch_pc,
        candidate_branch_pc=(
            right_c.branch_pc if right_c.branch_pc != left_c.branch_pc else None
        ),
        table_base_reg=base_reg,
        index_reg=left_c.index_reg,
    )
