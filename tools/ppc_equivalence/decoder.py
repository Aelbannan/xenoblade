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


def _insn(
    address: int,
    word: int,
    opcode: Opcode,
    operands: tuple[int, ...],
    *,
    record: bool = False,
    overflow: bool = False,
    link: bool = False,
) -> Instruction:
    return Instruction(address, word, opcode, operands, record, overflow, link)


def _decode_word(word: int, address: int) -> Instruction:
    primary = word >> 26
    rt = (word >> 21) & 31
    ra = (word >> 16) & 31
    rb = (word >> 11) & 31
    imm = word & 0xFFFF

    immediate = {
        7: (Opcode.MULLI, True),
        8: (Opcode.SUBFIC, True),
        12: (Opcode.ADDIC, True),
        13: (Opcode.ADDIC_DOT, True),
        14: (Opcode.ADDI, True),
        15: (Opcode.ADDIS, True),
    }
    if primary in immediate:
        opcode, signed_imm = immediate[primary]
        return _insn(address, word, opcode, (rt, ra, _signed(imm, 16) if signed_imm else imm))

    logical_immediate = {
        24: Opcode.ORI,
        25: Opcode.ORIS,
        26: Opcode.XORI,
        27: Opcode.XORIS,
        28: Opcode.ANDI_DOT,
        29: Opcode.ANDIS_DOT,
    }
    if primary in logical_immediate:
        opcode = logical_immediate[primary]
        return _insn(address, word, opcode, (ra, rt, imm), record=primary in (28, 29))

    if primary in (10, 11):
        bf = (word >> 23) & 7
        if (word >> 22) & 1:
            raise UnsupportedInstruction(address, word, "reserved compare bit is set")
        if (word >> 21) & 1:
            raise UnsupportedInstruction(address, word, "64-bit compare form is unsupported")
        opcode = Opcode.CMPLWI if primary == 10 else Opcode.CMPWI
        return _insn(address, word, opcode, (bf, ra, imm if primary == 10 else _signed(imm, 16)))

    if primary in (20, 21):
        sh = (word >> 11) & 31
        mb = (word >> 6) & 31
        me = (word >> 1) & 31
        opcode = Opcode.RLWIMI if primary == 20 else Opcode.RLWINM
        return _insn(address, word, opcode, (ra, rt, sh, mb, me), record=bool(word & 1))
    if primary == 23:
        mb = (word >> 6) & 31
        me = (word >> 1) & 31
        return _insn(address, word, Opcode.RLWNM, (ra, rt, rb, mb, me), record=bool(word & 1))

    d_memory = {
        32: Opcode.LWZ, 33: Opcode.LWZU, 34: Opcode.LBZ, 35: Opcode.LBZU,
        36: Opcode.STW, 37: Opcode.STWU, 38: Opcode.STB, 39: Opcode.STBU,
        40: Opcode.LHZ, 41: Opcode.LHZU, 42: Opcode.LHA, 43: Opcode.LHAU,
        44: Opcode.STH, 45: Opcode.STHU, 46: Opcode.LMW, 47: Opcode.STMW,
    }
    if primary in d_memory:
        opcode = d_memory[primary]
        update = opcode in (Opcode.LWZU, Opcode.LBZU, Opcode.LHZU, Opcode.LHAU, Opcode.STWU, Opcode.STBU, Opcode.STHU)
        if update and ra == 0:
            raise UnsupportedInstruction(address, word, "update-form memory access has RA=0")
        if opcode in (Opcode.LWZU, Opcode.LBZU, Opcode.LHZU, Opcode.LHAU) and ra == rt:
            raise UnsupportedInstruction(address, word, "update-form load has RA=RT")
        if opcode == Opcode.LMW and ra >= rt:
            raise UnsupportedInstruction(address, word, "lmw base register is in the destination range")
        return _insn(address, word, opcode, (rt, ra, _signed(imm, 16)))

    if primary == 18:
        displacement = _signed(word & 0x03FFFFFC, 26)
        absolute = (word >> 1) & 1
        target = displacement & 0xFFFFFFFF if absolute else (address + displacement) & 0xFFFFFFFF
        return _insn(address, word, Opcode.B, (target, absolute), link=bool(word & 1))
    if primary == 16:
        bo = (word >> 21) & 31
        bi = (word >> 16) & 31
        displacement = _signed(word & 0xFFFC, 16)
        absolute = (word >> 1) & 1
        target = displacement & 0xFFFFFFFF if absolute else (address + displacement) & 0xFFFFFFFF
        return _insn(address, word, Opcode.BC, (bo, bi, target, absolute), link=bool(word & 1))

    if primary == 19:
        xo = (word >> 1) & 0x3FF
        bo = (word >> 21) & 31
        bi = (word >> 16) & 31
        bh = (word >> 11) & 3
        reserved = (word >> 13) & 7
        if xo in (16, 528):
            if reserved:
                raise UnsupportedInstruction(address, word, "reserved branch-register bits are set")
            if xo == 528 and not (bo & 0b00100):
                raise UnsupportedInstruction(address, word, "bcctr form requests a CTR decrement")
            return _insn(address, word, Opcode.BCLR if xo == 16 else Opcode.BCCTR, (bo, bi, bh), link=bool(word & 1))
        cr_logical = {
            33: Opcode.CRNOR, 129: Opcode.CRANDC, 193: Opcode.CRXOR, 225: Opcode.CRNAND,
            257: Opcode.CRAND, 289: Opcode.CREQV, 417: Opcode.CRORC, 449: Opcode.CROR,
        }
        if xo in cr_logical:
            if word & 1:
                raise UnsupportedInstruction(address, word, "reserved CR logical Rc bit is set")
            return _insn(address, word, cr_logical[xo], (rt, ra, rb))

    if primary == 31:
        xo = (word >> 1) & 0x3FF
        rc = bool(word & 1)
        logical = {
            28: Opcode.AND, 60: Opcode.ANDC, 124: Opcode.NOR, 284: Opcode.EQV,
            316: Opcode.XOR, 412: Opcode.ORC, 444: Opcode.OR, 476: Opcode.NAND,
        }
        unary = {26: Opcode.CNTLZW, 922: Opcode.EXTSH, 954: Opcode.EXTSB}
        shifts = {24: Opcode.SLW, 536: Opcode.SRW, 792: Opcode.SRAW}
        if xo in logical:
            return _insn(address, word, logical[xo], (ra, rt, rb), record=rc)
        if xo in unary:
            if rb != 0:
                raise UnsupportedInstruction(address, word, "reserved unary RB field is nonzero")
            return _insn(address, word, unary[xo], (ra, rt), record=rc)
        if xo in shifts:
            return _insn(address, word, shifts[xo], (ra, rt, rb), record=rc)
        if xo == 824:
            return _insn(address, word, Opcode.SRAWI, (ra, rt, rb), record=rc)
        if xo in (0, 32):
            bf = (word >> 23) & 7
            if word & 1 or ((word >> 22) & 1):
                raise UnsupportedInstruction(address, word, "reserved compare bit is set")
            if (word >> 21) & 1:
                raise UnsupportedInstruction(address, word, "64-bit compare form is unsupported")
            return _insn(address, word, Opcode.CMPW if xo == 0 else Opcode.CMPLW, (bf, ra, rb))

        # XO-form arithmetic includes OE in bit 10 of this extracted field.
        oe = bool(xo & 0x200)
        base_xo = xo & 0x1FF
        arithmetic = {
            8: Opcode.SUBFC, 10: Opcode.ADDC, 11: Opcode.MULHWU, 40: Opcode.SUBF,
            75: Opcode.MULHW, 104: Opcode.NEG, 136: Opcode.SUBFE, 138: Opcode.ADDE,
            200: Opcode.SUBFZE, 202: Opcode.ADDZE, 232: Opcode.SUBFME, 234: Opcode.ADDME,
            235: Opcode.MULLW, 266: Opcode.ADD, 459: Opcode.DIVWU, 491: Opcode.DIVW,
        }
        if base_xo in arithmetic:
            opcode = arithmetic[base_xo]
            if oe and opcode in (Opcode.MULHW, Opcode.MULHWU):
                raise UnsupportedInstruction(address, word, "reserved multiply-high OE bit is set")
            if opcode in (Opcode.NEG, Opcode.ADDME, Opcode.ADDZE, Opcode.SUBFME, Opcode.SUBFZE):
                if rb != 0:
                    raise UnsupportedInstruction(address, word, "reserved arithmetic RB field is nonzero")
                operands = (rt, ra)
            else:
                operands = (rt, ra, rb)
            return _insn(address, word, opcode, operands, record=rc, overflow=oe)

        if xo == 19:
            if ra != 0 or rb != 0 or rc:
                raise UnsupportedInstruction(address, word, "reserved mfcr bits are set")
            return _insn(address, word, Opcode.MFCR, (rt,))
        if xo == 144:
            fxm = (word >> 12) & 0xFF
            if ((word >> 11) & 1) or rc:
                raise UnsupportedInstruction(address, word, "reserved mtcrf bits are set")
            return _insn(address, word, Opcode.MTCRF, (rt, fxm))
        if xo in (339, 467):
            spr = ((word >> 16) & 31) | (((word >> 11) & 31) << 5)
            if rc:
                raise UnsupportedInstruction(address, word, "reserved SPR transfer Rc bit is set")
            if spr not in (1, 8, 9):
                raise UnsupportedInstruction(address, word, f"unsupported special-purpose register {spr}")
            return _insn(address, word, Opcode.MFSPR if xo == 339 else Opcode.MTSPR, (rt, spr))

        x_memory = {
            23: Opcode.LWZX, 55: Opcode.LWZUX, 87: Opcode.LBZX, 119: Opcode.LBZUX,
            151: Opcode.STWX, 183: Opcode.STWUX, 215: Opcode.STBX, 247: Opcode.STBUX,
            279: Opcode.LHZX, 311: Opcode.LHZUX, 343: Opcode.LHAX, 375: Opcode.LHAUX,
            407: Opcode.STHX, 439: Opcode.STHUX, 534: Opcode.LWBRX, 662: Opcode.STWBRX,
            790: Opcode.LHBRX, 918: Opcode.STHBRX,
        }
        if xo in x_memory:
            opcode = x_memory[xo]
            if rc:
                raise UnsupportedInstruction(address, word, "reserved memory Rc bit is set")
            update = opcode in (Opcode.LWZUX, Opcode.LBZUX, Opcode.LHZUX, Opcode.LHAUX, Opcode.STWUX, Opcode.STBUX, Opcode.STHUX)
            if update and ra == 0:
                raise UnsupportedInstruction(address, word, "update-form memory access has RA=0")
            if opcode in (Opcode.LWZUX, Opcode.LBZUX, Opcode.LHZUX, Opcode.LHAUX) and ra == rt:
                raise UnsupportedInstruction(address, word, "update-form load has RA=RT")
            return _insn(address, word, opcode, (rt, ra, rb))

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


def decode_block(code: bytes, base: int = 0, *, validate_with_capstone: bool = True) -> list[Instruction]:
    if not code or len(code) % 4:
        raise DecodeError("PowerPC input must be a non-empty multiple of four bytes")
    if base < 0 or base > 0xFFFFFFFF or base % 4:
        raise DecodeError("base address must be a four-byte-aligned 32-bit value")
    if base + len(code) - 4 > 0xFFFFFFFF:
        raise DecodeError("instruction addresses wrap the 32-bit address space")
    mnemonics: Sequence[str | None] = _capstone_validate(code, base) if validate_with_capstone else [None] * (len(code) // 4)
    result: list[Instruction] = []
    for index in range(0, len(code), 4):
        insn = _decode_word(int.from_bytes(code[index:index + 4], "big"), base + index)
        result.append(Instruction(
            insn.address, insn.raw, insn.opcode, insn.operands, insn.record,
            insn.overflow, insn.link, mnemonics[index // 4],
        ))
    return result
