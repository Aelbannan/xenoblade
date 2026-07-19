"""Auto-build ``JumpTableProofContext`` from pattern + linked image hydration.

Pattern recognition alone is not a proof. This module only succeeds when:

1. both sides expose one ``exact-pattern`` candidate with matching bounds/index;
2. the table base VA is recoverable from concrete ``lis``/``addi`` immediates or
   from recoverable SDA materialization (see ``resolve_table_base_va``);
3. linked ELF/DOL hydration yields aligned ADDR32 words for ``bound_imm + 1`` entries.

SDA recovery is fail-closed unless one of these holds for the ``lwzx`` base reg:

* ``addi rt, r2/r13, disp`` with ``sda_bases`` supplying the small-data anchor;
* ``lwz`` / ``addi`` carrying ``R_PPC_EMB_SDA21`` and ``symbol_addresses`` for
  the table symbol (absolute table VA);
* classic ``addis``/``addi`` materialization without relocation.

``lwz rt, disp(r2/r13)`` without a resolvable SDA21 symbol is rejected: the slot
holds a pointer word, not the table VA itself, and this module does not read
linked images to chase that pointer.
"""

from __future__ import annotations

from collections.abc import Sequence
from pathlib import Path

from tools.ppc_equivalence.decoder import decode_block
from tools.ppc_equivalence.dol_symbols import DolSymbolError, extract_by_address
from tools.ppc_equivalence.ir import Instruction, Opcode, R_PPC_EMB_SDA21
from tools.ppc_equivalence.jump_table import JumpTableCandidate, find_jump_table_candidates
from tools.ppc_equivalence.jump_table_image import (
    JumpTableImageError,
    hydrate_jump_table,
    jump_table_words_from_image,
)
from tools.ppc_equivalence.jump_table_obligations import JumpTableProofContext
from tools.ppc_equivalence.jump_table_pairing import JumpTablePairingError, pair_jump_table_cases

_DEFAULT_BASE_LOOKBACK_INSNS = 128
"""Instruction slots searched backward from ``lwzx`` for table-base materialization."""

_LINKED_TEXT_LOOKBACK_BYTES = 512
"""Linked-image text bytes re-decoded before ``lwzx`` when hydrating coop proofs."""

_SDA_BASE_REGS = frozenset({2, 13})


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


def _sign_extend_16(value: int) -> int:
    word = int(value) & 0xFFFF
    if word >= 0x8000:
        word -= 0x10000
    return word & 0xFFFFFFFF


def _sda_symbol_va(
    insn: Instruction,
    *,
    symbol_addresses: dict[str, int] | None,
) -> int | None:
    relocation = insn.relocation
    if relocation is None or relocation.relocation_type != R_PPC_EMB_SDA21:
        return None
    if not symbol_addresses:
        return None
    for key in (relocation.canonical_symbol, relocation.symbol):
        if key in symbol_addresses:
            return (
                int(symbol_addresses[key]) + int(relocation.addend)
            ) & 0xFFFFFFFF
    return None


def _sda_addi_va(
    ra: int,
    imm: int,
    *,
    sda_bases: dict[int, int] | None,
) -> int | None:
    if ra not in _SDA_BASE_REGS or not sda_bases or ra not in sda_bases:
        return None
    return (int(sda_bases[ra]) + _sign_extend_16(imm)) & 0xFFFFFFFF


def _resolve_sda_materialization(
    insn: Instruction,
    *,
    sda_bases: dict[int, int] | None,
    symbol_addresses: dict[str, int] | None,
) -> int | None:
    symbol_va = _sda_symbol_va(insn, symbol_addresses=symbol_addresses)
    if symbol_va is not None:
        return symbol_va
    if insn.opcode not in (Opcode.ADDI, Opcode.ADDIC, Opcode.ADDIC_DOT, Opcode.LWZ, Opcode.LWZU):
        return None
    if len(insn.operands) < 3:
        return None
    ra = int(insn.operands[1])
    imm = int(insn.operands[2])
    if insn.opcode in (Opcode.ADDI, Opcode.ADDIC, Opcode.ADDIC_DOT):
        return _sda_addi_va(ra, imm, sda_bases=sda_bases)
    # lwz without resolvable SDA21 symbol needs a memory chase — fail closed.
    return None


def _decode_text_skipping_zero_padding(code: bytes, base: int) -> list[Instruction]:
    """Decode PPC text while skipping ``0x00000000`` alignment gaps in DOL slices."""
    insns: list[Instruction] = []
    span_start = 0
    offset = 0
    while offset < len(code):
        if code[offset : offset + 4] == b"\x00" * 4:
            if offset > span_start:
                insns.extend(
                    decode_block(
                        code[span_start:offset],
                        base + span_start,
                        validate_with_capstone=False,
                    )
                )
            while offset < len(code) and code[offset : offset + 4] == b"\x00" * 4:
                offset += 4
            span_start = offset
            continue
        offset += 4
    if span_start < len(code):
        insns.extend(
            decode_block(
                code[span_start:],
                base + span_start,
                validate_with_capstone=False,
            )
        )
    return insns


def _decode_linked_text_window(
    code: bytes,
    base: int,
) -> list[Instruction]:
    try:
        return decode_block(code, base, validate_with_capstone=False)
    except ValueError:
        return _decode_text_skipping_zero_padding(code, base)


def _select_jump_table_candidate(
    instructions: Sequence[Instruction],
    expanded: Sequence[Instruction],
) -> JumpTableCandidate | None:
    """Pick the jump-table candidate anchored to this function's dispatch tail."""
    branch_pc = _branch_pc_for_expansion(instructions)
    candidates = find_jump_table_candidates(expanded)
    if branch_pc is not None:
        matching = [
            item for item in candidates if item.branch_pc == branch_pc
        ]
        if len(matching) == 1:
            return matching[0]
    exact = [item for item in candidates if item.confidence == "exact-pattern"]
    if len(exact) == 1:
        return exact[0]
    if len(candidates) == 1:
        return candidates[0]
    return None


def _branch_pc_for_expansion(instructions: Sequence[Instruction]) -> int | None:
    """Pick a jump-table ``bctr`` PC to anchor linked-image lookback."""
    candidates = find_jump_table_candidates(instructions)
    exact = [item for item in candidates if item.confidence == "exact-pattern"]
    if len(exact) == 1 and exact[0].branch_pc is not None:
        return exact[0].branch_pc
    with_branch = [item for item in candidates if item.branch_pc is not None]
    if len(with_branch) == 1:
        return with_branch[0].branch_pc
    for insn in reversed(instructions):
        if insn.opcode == Opcode.BCCTR:
            return insn.address
    return None


def _extract_elf_text_window(
    path: Path,
    address: int,
    size: int,
) -> tuple[bytes, int] | None:
    from tools.ppc_equivalence.elf_symbols import ElfSymbolError, list_allocatable_sections

    try:
        sections = list_allocatable_sections(path)
    except ElfSymbolError:
        return None
    for section in sections:
        if not section.executable:
            continue
        section_end = section.addr + section.size
        if section.addr <= address and address + size <= section_end:
            offset = address - section.addr
            payload = section.data[offset : offset + size]
            if len(payload) == size:
                return payload, address
    return None


def expand_jump_table_instructions(
    instructions: Sequence[Instruction],
    *,
    dol_path: Path | str | None = None,
    elf_path: Path | str | None = None,
    lookback_bytes: int = _LINKED_TEXT_LOOKBACK_BYTES,
) -> Sequence[Instruction]:
    """Re-decode a wider linked text window around the jump-table dispatch tail.

    Function slices from coop/objdiff often omit ``addis``/``addi`` table-base
    materialization that sits earlier in the linked image. When ``dol_path`` or
    ``elf_path`` is available, decode ``[branch_pc - lookback, branch_pc + 4)``
    from linked text and use that stream for pattern/base recovery.
    """
    branch_pc = _branch_pc_for_expansion(instructions)
    if branch_pc is None:
        return instructions
    start = max(0, int(branch_pc) - lookback_bytes)
    size = int(branch_pc) + 4 - start
    if size <= 0 or size % 4:
        return instructions

    dol = Path(dol_path) if dol_path is not None else None
    if dol is not None and dol.is_file():
        try:
            slice_ = extract_by_address(dol, start, size)
            return _decode_linked_text_window(slice_.code, slice_.base)
        except DolSymbolError:
            pass

    elf = Path(elf_path) if elf_path is not None else None
    if elf is not None and elf.is_file():
        extracted = _extract_elf_text_window(elf, start, size)
        if extracted is not None:
            code, window_base = extracted
            try:
                return _decode_linked_text_window(code, window_base)
            except ValueError:
                pass
    return instructions


def resolve_table_base_va(
    instructions: Sequence[Instruction],
    candidate: JumpTableCandidate,
    *,
    max_lookback: int = _DEFAULT_BASE_LOOKBACK_INSNS,
    sda_bases: dict[int, int] | None = None,
    symbol_addresses: dict[str, int] | None = None,
) -> int | None:
    """Recover the linked table VA feeding the ``lwzx`` base register.

    Returns ``None`` when immediates are missing, relocated without bindings,
    or not a simple ``lis`` + ``addi`` / recoverable SDA materialization.
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
        if insn.relocation is not None and insn.relocation.relocation_type != R_PPC_EMB_SDA21:
            continue
        if insn.opcode == Opcode.ADDIS:
            rt, ra, imm = insn.operands
            if rt == base_reg and ra == 0:
                return (int(imm) << 16) & 0xFFFFFFFF
        if insn.opcode in (Opcode.ADDI, Opcode.ADDIC, Opcode.ADDIC_DOT, Opcode.LWZ, Opcode.LWZU):
            rt, ra, imm = insn.operands
            if rt != base_reg:
                continue
            sda_va = _resolve_sda_materialization(
                insn,
                sda_bases=sda_bases,
                symbol_addresses=symbol_addresses,
            )
            if sda_va is not None:
                return sda_va
            if insn.opcode in (Opcode.LWZ, Opcode.LWZU):
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


def _hydrate_side(
    base_va: int,
    entry_count: int,
    *,
    dol_path: Path | None,
    elf_path: Path | None,
):
    return hydrate_jump_table(
        base=base_va,
        entry_count=entry_count,
        elf_path=elf_path,
        dol_path=dol_path,
    )


def try_auto_jump_table_context(
    original: Sequence[Instruction],
    candidate: Sequence[Instruction],
    *,
    dol_path: Path | str | None = None,
    elf_path: Path | str | None = None,
    original_dol_path: Path | str | None = None,
    original_elf_path: Path | str | None = None,
    candidate_dol_path: Path | str | None = None,
    candidate_elf_path: Path | str | None = None,
    sda_bases: dict[int, int] | None = None,
    symbol_addresses: dict[str, int] | None = None,
) -> JumpTableProofContext | None:
    """Build a proof context when both sides match and table images hydrate.

    Fail-closed: returns ``None`` rather than raising on incomplete evidence.
    """
    left_dol = Path(original_dol_path) if original_dol_path is not None else (
        Path(dol_path) if dol_path is not None else None
    )
    left_elf = Path(original_elf_path) if original_elf_path is not None else (
        Path(elf_path) if elf_path is not None else None
    )
    right_dol = Path(candidate_dol_path) if candidate_dol_path is not None else (
        Path(dol_path) if dol_path is not None else None
    )
    right_elf = Path(candidate_elf_path) if candidate_elf_path is not None else (
        Path(elf_path) if elf_path is not None else None
    )
    if (
        left_dol is None and left_elf is None
        and right_dol is None and right_elf is None
    ):
        return None

    original_for_analysis = expand_jump_table_instructions(
        original,
        dol_path=left_dol,
        elf_path=left_elf,
    )
    candidate_for_analysis = expand_jump_table_instructions(
        candidate,
        dol_path=right_dol,
        elf_path=right_elf,
    )

    left_c = _select_jump_table_candidate(original, original_for_analysis)
    right_c = _select_jump_table_candidate(candidate, candidate_for_analysis)
    if left_c is None or right_c is None:
        return None
    if left_c.confidence != "exact-pattern" or right_c.confidence != "exact-pattern":
        return None
    if left_c.bound_imm is None or left_c.bound_imm != right_c.bound_imm:
        return None
    if left_c.index_reg != right_c.index_reg:
        return None

    base_va = resolve_table_base_va(
        original_for_analysis,
        left_c,
        sda_bases=sda_bases,
        symbol_addresses=symbol_addresses,
    )
    if base_va is None:
        return None
    cand_va = resolve_table_base_va(
        candidate_for_analysis,
        right_c,
        sda_bases=sda_bases,
        symbol_addresses=symbol_addresses,
    )
    if cand_va is None:
        return None

    base_reg = table_base_reg(left_c, original_for_analysis)
    if base_reg is None:
        return None
    right_base_reg = table_base_reg(right_c, candidate_for_analysis)
    if right_base_reg is not None and right_base_reg != base_reg:
        return None

    entry_count = int(left_c.bound_imm) + 1
    try:
        orig_image = _hydrate_side(
            base_va,
            entry_count,
            dol_path=left_dol,
            elf_path=left_elf,
        )
        if base_va == cand_va and left_dol == right_dol and left_elf == right_elf:
            cand_image = orig_image
        else:
            cand_image = _hydrate_side(
                cand_va,
                entry_count,
                dol_path=right_dol,
                elf_path=right_elf,
            )
    except JumpTableImageError:
        return None
    if any(word & 3 for word in orig_image.words):
        return None
    if any(word & 3 for word in cand_image.words):
        return None

    try:
        pairing = pair_jump_table_cases(
            original_words=orig_image.words,
            candidate_words=cand_image.words,
            table_base_original=base_va,
            table_base_candidate=cand_va,
        )
    except JumpTablePairingError:
        return None

    orig_table = jump_table_words_from_image(orig_image)
    cand_table = (
        orig_table
        if base_va == cand_va and orig_image.words == cand_image.words
        else jump_table_words_from_image(cand_image)
    )
    return JumpTableProofContext(
        table=orig_table,
        candidate_table=None if cand_table is orig_table else cand_table,
        pairing=pairing,
        branch_pc=left_c.branch_pc,
        candidate_branch_pc=(
            right_c.branch_pc if right_c.branch_pc != left_c.branch_pc else None
        ),
        table_base_reg=base_reg,
        candidate_table_base_reg=(
            right_base_reg if right_base_reg is not None and right_base_reg != base_reg else None
        ),
        index_reg=left_c.index_reg,
    )
