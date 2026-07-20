"""Bounded, fail-closed GPR constant recovery for ``mtctr`` trip counts.

Recognizes only straight-line materialization in a small lookback window
immediately before ``mtctr``. Prefer false negatives: symbolic or relocated
sources stay unknown. Includes ``andi.``/``andis.`` remainder masks when the
source register is already concrete.
"""

from __future__ import annotations

from collections.abc import Mapping, Sequence

from tools.ppc_equivalence.ir import Instruction, Opcode

_DEFAULT_LOOKBACK = 12


def recover_gpr_constant(
    instructions: Sequence[Instruction],
    use_index: int,
    reg: int,
    *,
    max_lookback: int = _DEFAULT_LOOKBACK,
    readonly_words: Mapping[int, int] | None = None,
) -> tuple[int | None, list[str]]:
    """Return the concrete u32 in ``reg`` immediately before ``use_index``."""
    if use_index <= 0:
        return None, ["missing GPR materialization before use"]
    if max_lookback < 1:
        return None, ["GPR materialization lookback disabled"]

    lower = max(0, use_index - max_lookback)
    for index in range(use_index - 1, lower - 1, -1):
        insn = instructions[index]
        if not _defines_gpr(insn, reg):
            continue
        return _eval_defining_insn(
            instructions,
            index,
            insn,
            reg,
            max_lookback=max_lookback,
            readonly_words=readonly_words,
            depth=0,
        )
    return None, [f"no concrete materialization for r{reg} within lookback"]


def _eval_defining_insn(
    instructions: Sequence[Instruction],
    index: int,
    insn: Instruction,
    reg: int,
    *,
    max_lookback: int,
    readonly_words: Mapping[int, int] | None,
    depth: int,
) -> tuple[int | None, list[str]]:
    if depth >= max_lookback:
        return None, ["GPR materialization lookback exceeded"]
    if insn.relocation is not None:
        return None, ["relocated GPR materialization is not concrete"]

    opcode = insn.opcode
    if opcode == Opcode.ADDI:
        rt, ra, imm = (int(v) for v in insn.operands)
        if rt != reg:
            return None, [f"addi defines r{rt}, expected r{reg}"]
        base, notes = _gpr_value_before(
            instructions, index, ra, max_lookback=max_lookback,
            readonly_words=readonly_words, depth=depth + 1,
        )
        if base is None:
            return None, notes or [f"addi r{rt}, r{ra}, {imm} base not concrete"]
        return (base + _sign_extend_16(imm)) & 0xFFFFFFFF, notes

    if opcode == Opcode.ADDIS:
        rt, ra, imm = (int(v) for v in insn.operands)
        if rt != reg:
            return None, [f"addis defines r{rt}, expected r{reg}"]
        base, notes = _gpr_value_before(
            instructions, index, ra, max_lookback=max_lookback,
            readonly_words=readonly_words, depth=depth + 1,
        )
        if base is None:
            return None, notes or [f"addis r{rt}, r{ra}, {imm} base not concrete"]
        return (base + ((int(imm) & 0xFFFF) << 16)) & 0xFFFFFFFF, notes

    if opcode == Opcode.ORI:
        rt, ra, imm = (int(v) for v in insn.operands)
        if rt != reg:
            return None, [f"ori defines r{rt}, expected r{reg}"]
        base, notes = _gpr_value_before(
            instructions, index, ra, max_lookback=max_lookback,
            readonly_words=readonly_words, depth=depth + 1,
        )
        if base is None:
            return None, notes or [f"ori r{rt}, r{ra}, {imm} base not concrete"]
        return (base | (int(imm) & 0xFFFF)) & 0xFFFFFFFF, notes

    if opcode == Opcode.ORIS:
        rt, ra, imm = (int(v) for v in insn.operands)
        if rt != reg:
            return None, [f"oris defines r{rt}, expected r{reg}"]
        base, notes = _gpr_value_before(
            instructions, index, ra, max_lookback=max_lookback,
            readonly_words=readonly_words, depth=depth + 1,
        )
        if base is None:
            return None, notes or [f"oris r{rt}, r{ra}, {imm} base not concrete"]
        return (base | ((int(imm) & 0xFFFF) << 16)) & 0xFFFFFFFF, notes

    if opcode == Opcode.ANDI_DOT:
        rt, ra, imm = (int(v) for v in insn.operands)
        if rt != reg:
            return None, [f"andi. defines r{rt}, expected r{reg}"]
        base, notes = _gpr_value_before(
            instructions, index, ra, max_lookback=max_lookback,
            readonly_words=readonly_words, depth=depth + 1,
        )
        if base is None:
            return None, notes or [f"andi. r{rt}, r{ra}, {imm} base not concrete"]
        return (base & (int(imm) & 0xFFFF)) & 0xFFFFFFFF, notes

    if opcode == Opcode.ANDIS_DOT:
        rt, ra, imm = (int(v) for v in insn.operands)
        if rt != reg:
            return None, [f"andis. defines r{rt}, expected r{reg}"]
        base, notes = _gpr_value_before(
            instructions, index, ra, max_lookback=max_lookback,
            readonly_words=readonly_words, depth=depth + 1,
        )
        if base is None:
            return None, notes or [f"andis. r{rt}, r{ra}, {imm} base not concrete"]
        return (base & ((int(imm) & 0xFFFF) << 16)) & 0xFFFFFFFF, notes

    if opcode == Opcode.OR:
        rt, ra, rb = (int(v) for v in insn.operands)
        if rt != reg:
            return None, [f"or defines r{rt}, expected r{reg}"]
        if ra != rb:
            return None, ["or CTR materialization is not a register copy"]
        return _gpr_value_before(
            instructions, index, ra, max_lookback=max_lookback,
            readonly_words=readonly_words, depth=depth + 1,
        )

    if opcode == Opcode.XOR:
        rt, ra, rb = (int(v) for v in insn.operands)
        if rt == reg and ra == rb:
            return 0, []
        return None, ["xor CTR materialization is not self-clearing"]

    if opcode == Opcode.LWZ:
        rt, ra, disp = (int(v) for v in insn.operands)
        if rt != reg:
            return None, [f"lwz defines r{rt}, expected r{reg}"]
        if readonly_words is None:
            return None, ["lwz CTR load requires proven readonly constant (unsupported)"]
        base, notes = _gpr_value_before(
            instructions, index, ra, max_lookback=max_lookback,
            readonly_words=readonly_words, depth=depth + 1,
        )
        if base is None:
            return None, notes or [f"lwz r{rt}, {disp}(r{ra}) base not concrete"]
        address = (base + _sign_extend_16(disp)) & 0xFFFFFFFF
        if ra == 0:
            address = _sign_extend_16(disp) & 0xFFFFFFFF
        word = readonly_words.get(address)
        if word is None:
            return None, [f"lwz address 0x{address:08X} not in readonly image"]
        return int(word) & 0xFFFFFFFF, notes

    return None, [f"unsupported GPR materialization opcode {opcode.value}"]


def _gpr_value_before(
    instructions: Sequence[Instruction],
    use_index: int,
    reg: int,
    *,
    max_lookback: int,
    readonly_words: Mapping[int, int] | None,
    depth: int,
) -> tuple[int | None, list[str]]:
    if reg == 0:
        return 0, []
    return recover_gpr_constant(
        instructions,
        use_index,
        reg,
        max_lookback=max_lookback,
        readonly_words=readonly_words,
    )


def _defines_gpr(insn: Instruction, reg: int) -> bool:
    opcode = insn.opcode
    if opcode in (
        Opcode.ADDI,
        Opcode.ADDIS,
        Opcode.ORI,
        Opcode.ORIS,
        Opcode.ANDI_DOT,
        Opcode.ANDIS_DOT,
        Opcode.LWZ,
        Opcode.LWZU,
    ):
        return int(insn.operands[0]) == reg
    if opcode in (Opcode.OR, Opcode.XOR):
        return int(insn.operands[0]) == reg
    return False


def collect_lwz_readonly_addresses(
    instructions: Sequence[Instruction],
    use_index: int,
    reg: int,
    *,
    max_lookback: int = _DEFAULT_LOOKBACK,
) -> frozenset[int]:
    """Return effective ``lwz`` addresses needed to concretize ``reg`` before ``use_index``."""
    if use_index <= 0 or max_lookback < 1:
        return frozenset()
    lower = max(0, use_index - max_lookback)
    for index in range(use_index - 1, lower - 1, -1):
        insn = instructions[index]
        if not _defines_gpr(insn, reg):
            continue
        return _collect_lwz_from_defining_insn(
            instructions,
            index,
            insn,
            reg,
            max_lookback=max_lookback,
            depth=0,
        )
    return frozenset()


def _collect_lwz_from_defining_insn(
    instructions: Sequence[Instruction],
    index: int,
    insn: Instruction,
    reg: int,
    *,
    max_lookback: int,
    depth: int,
) -> frozenset[int]:
    if depth >= max_lookback or insn.relocation is not None:
        return frozenset()

    opcode = insn.opcode
    if opcode == Opcode.ADDI:
        rt, ra, _imm = (int(v) for v in insn.operands)
        if rt != reg:
            return frozenset()
        return _collect_lwz_before(
            instructions, index, ra, max_lookback=max_lookback, depth=depth + 1,
        )

    if opcode == Opcode.ADDIS:
        rt, ra, _imm = (int(v) for v in insn.operands)
        if rt != reg:
            return frozenset()
        return _collect_lwz_before(
            instructions, index, ra, max_lookback=max_lookback, depth=depth + 1,
        )

    if opcode == Opcode.ORI:
        rt, ra, _imm = (int(v) for v in insn.operands)
        if rt != reg:
            return frozenset()
        return _collect_lwz_before(
            instructions, index, ra, max_lookback=max_lookback, depth=depth + 1,
        )

    if opcode == Opcode.ORIS:
        rt, ra, _imm = (int(v) for v in insn.operands)
        if rt != reg:
            return frozenset()
        return _collect_lwz_before(
            instructions, index, ra, max_lookback=max_lookback, depth=depth + 1,
        )

    if opcode == Opcode.ANDI_DOT:
        rt, ra, _imm = (int(v) for v in insn.operands)
        if rt != reg:
            return frozenset()
        return _collect_lwz_before(
            instructions, index, ra, max_lookback=max_lookback, depth=depth + 1,
        )

    if opcode == Opcode.ANDIS_DOT:
        rt, ra, _imm = (int(v) for v in insn.operands)
        if rt != reg:
            return frozenset()
        return _collect_lwz_before(
            instructions, index, ra, max_lookback=max_lookback, depth=depth + 1,
        )

    if opcode == Opcode.OR:
        rt, ra, rb = (int(v) for v in insn.operands)
        if rt != reg or ra != rb:
            return frozenset()
        return _collect_lwz_before(
            instructions, index, ra, max_lookback=max_lookback, depth=depth + 1,
        )

    if opcode == Opcode.XOR:
        rt, ra, rb = (int(v) for v in insn.operands)
        if rt == reg and ra == rb:
            return frozenset()
        return frozenset()

    if opcode == Opcode.LWZ:
        rt, ra, disp = (int(v) for v in insn.operands)
        if rt != reg:
            return frozenset()
        nested = _collect_lwz_before(
            instructions, index, ra, max_lookback=max_lookback, depth=depth + 1,
        )
        if ra == 0:
            address = _sign_extend_16(disp) & 0xFFFFFFFF
            return frozenset({address}) | nested
        base, _notes = recover_gpr_constant(
            instructions, index, ra, max_lookback=max_lookback,
        )
        if base is None:
            return nested
        address = (base + _sign_extend_16(disp)) & 0xFFFFFFFF
        return frozenset({address}) | nested

    return frozenset()


def _collect_lwz_before(
    instructions: Sequence[Instruction],
    use_index: int,
    reg: int,
    *,
    max_lookback: int,
    depth: int,
) -> frozenset[int]:
    if reg == 0:
        return frozenset()
    if depth >= max_lookback:
        return frozenset()
    return collect_lwz_readonly_addresses(
        instructions,
        use_index,
        reg,
        max_lookback=max_lookback,
    )


def _sign_extend_16(value: int) -> int:
    word = int(value) & 0xFFFF
    if word >= 0x8000:
        word -= 0x10000
    return word & 0xFFFFFFFF
