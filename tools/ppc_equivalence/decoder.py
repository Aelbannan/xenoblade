from __future__ import annotations

import re
from collections.abc import Sequence

from .ir import DecodeError, Instruction, Opcode, UnsupportedInstruction


def _signed(value: int, width: int) -> int:
    sign = 1 << (width - 1)
    return value - (1 << width) if value & sign else value


def parse_hex(text: str) -> bytes:
    cleaned = re.sub(r"0[xX]", "", text.strip())
    cleaned = re.sub(r"[\s,_:\-]", "", cleaned)
    if re.search(r"[^0-9a-fA-F]", cleaned):
        raise DecodeError("hex input contains a non-hexadecimal character")
    if not cleaned or len(cleaned) % 8:
        raise DecodeError("PowerPC hex must contain one or more complete 4-byte words")
    try:
        return bytes.fromhex(cleaned)
    except ValueError as exc:
        raise DecodeError(f"invalid hexadecimal input: {exc}") from exc


def _decode_word(word: int, address: int) -> Instruction:
    primary = word >> 26
    rt = (word >> 21) & 31
    ra = (word >> 16) & 31
    rb = (word >> 11) & 31
    imm = word & 0xFFFF

    d_forms: dict[int, tuple[Opcode, bool, bool]] = {
        7: (Opcode.MULLI, True, False),
        14: (Opcode.ADDI, True, False),
        15: (Opcode.ADDIS, True, False),
        24: (Opcode.ORI, False, False),
        25: (Opcode.ORIS, False, False),
        26: (Opcode.XORI, False, False),
        27: (Opcode.XORIS, False, False),
        28: (Opcode.ANDI_DOT, False, True),
        29: (Opcode.ANDIS_DOT, False, True),
    }
    if primary in d_forms:
        opcode, signed_imm, record = d_forms[primary]
        value = _signed(imm, 16) if signed_imm else imm
        operands = (ra, rt, value) if primary in (24, 25, 26, 27, 28, 29) else (rt, ra, value)
        return Instruction(address, word, opcode, operands, record)

    if primary in (10, 11):
        bf = (word >> 23) & 7
        l_bit = (word >> 21) & 1
        if (word >> 22) & 1:
            raise UnsupportedInstruction(address, word, "reserved compare bit is set")
        if l_bit:
            raise UnsupportedInstruction(address, word, "64-bit compare form is unsupported")
        opcode = Opcode.CMPLWI if primary == 10 else Opcode.CMPWI
        value = imm if primary == 10 else _signed(imm, 16)
        return Instruction(address, word, opcode, (bf, ra, value))

    if primary in (20, 21):
        sh = (word >> 11) & 31
        mb = (word >> 6) & 31
        me = (word >> 1) & 31
        opcode = Opcode.RLWIMI if primary == 20 else Opcode.RLWINM
        return Instruction(address, word, opcode, (ra, rt, sh, mb, me), bool(word & 1))

    if primary == 31:
        xo = (word >> 1) & 0x3FF
        rc = bool(word & 1)
        logical = {
            28: Opcode.AND,
            124: Opcode.NOR,
            316: Opcode.XOR,
            444: Opcode.OR,
        }
        unary = {26: Opcode.CNTLZW, 922: Opcode.EXTSH, 954: Opcode.EXTSB}
        if xo in logical:
            return Instruction(address, word, logical[xo], (ra, rt, rb), rc)
        if xo in unary:
            return Instruction(address, word, unary[xo], (ra, rt), rc)
        if xo in (0, 32):
            bf = (word >> 23) & 7
            l_bit = (word >> 21) & 1
            if word & 1 or ((word >> 22) & 1):
                raise UnsupportedInstruction(address, word, "reserved compare bit is set")
            if l_bit:
                raise UnsupportedInstruction(address, word, "64-bit compare form is unsupported")
            opcode = Opcode.CMPW if xo == 0 else Opcode.CMPLW
            return Instruction(address, word, opcode, (bf, ra, rb))

        # XO-form OE is bit 10 and therefore part of the 10-bit XO above.
        oe = bool(xo & 0x200)
        xo_base = xo & 0x1FF
        arithmetic = {40: Opcode.SUBF, 266: Opcode.ADD}
        if xo_base in arithmetic:
            if oe:
                raise UnsupportedInstruction(address, word, "overflow-enabled arithmetic is unsupported")
            return Instruction(address, word, arithmetic[xo_base], (rt, ra, rb), rc)

    raise UnsupportedInstruction(address, word, f"unsupported primary opcode {primary}")


def _capstone_validate(code: bytes, base: int) -> list[str]:
    try:
        import capstone
    except ImportError as exc:
        raise RuntimeError(
            "Capstone is required for decoder cross-checks; install tools/ppc_equivalence/requirements.txt"
        ) from exc
    engine = capstone.Cs(capstone.CS_ARCH_PPC, capstone.CS_MODE_32 | capstone.CS_MODE_BIG_ENDIAN)
    decoded = list(engine.disasm(code, base))
    if len(decoded) * 4 != len(code):
        offset = len(decoded) * 4
        raise DecodeError(f"Capstone could not decode the word at 0x{base + offset:08x}")
    for index, item in enumerate(decoded):
        if item.address != base + index * 4 or item.size != 4:
            raise DecodeError("Capstone returned an inconsistent PPC32 instruction boundary")
    return [item.mnemonic for item in decoded]


def decode_block(
    code: bytes,
    base: int = 0,
    *,
    validate_with_capstone: bool = True,
) -> list[Instruction]:
    if not code or len(code) % 4:
        raise DecodeError("PowerPC input must be a non-empty multiple of four bytes")
    if base < 0 or base > 0xFFFFFFFF or base % 4:
        raise DecodeError("base address must be a four-byte-aligned 32-bit value")
    if base + len(code) - 4 > 0xFFFFFFFF:
        raise DecodeError("instruction addresses wrap the 32-bit address space")
    mnemonics: Sequence[str | None]
    mnemonics = _capstone_validate(code, base) if validate_with_capstone else [None] * (len(code) // 4)
    result: list[Instruction] = []
    for index in range(0, len(code), 4):
        address = (base + index) & 0xFFFFFFFF
        insn = _decode_word(int.from_bytes(code[index : index + 4], "big"), address)
        mnemonic = mnemonics[index // 4]
        result.append(
            Instruction(
                insn.address,
                insn.raw,
                insn.opcode,
                insn.operands,
                insn.record,
                mnemonic,
            )
        )
    return result
