from __future__ import annotations

import re
from collections.abc import Sequence
from dataclasses import replace
from typing import Any

from .ir import (
    DecodeError,
    Instruction,
    Opcode,
    RelocationRef,
    R_PPC_ADDR16_HA,
    R_PPC_ADDR16_HI,
    R_PPC_ADDR16_LO,
    R_PPC_EMB_SDA21,
    R_PPC_REL14,
    R_PPC_REL24,
    SUPPORTED_TEXT_RELOCATIONS,
    UnsupportedInstruction,
)
from .spr import READABLE_SPRS, WRITABLE_SPRS


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


def _decode_word(
    word: int,
    address: int,
    *,
    allow_broadway_lmw_overlap: bool,
) -> Instruction:
    primary = word >> 26
    rt = (word >> 21) & 31
    ra = (word >> 16) & 31
    rb = (word >> 11) & 31
    imm = word & 0xFFFF

    if primary == 3:
        return _insn(address, word, Opcode.TWI, (rt, ra, _signed(imm, 16)))
    if primary == 17:
        if word != 0x44000002:
            raise UnsupportedInstruction(address, word, "reserved system-call encoding")
        return _insn(address, word, Opcode.SC, ())
    if primary == 4 and ((word >> 1) & 0x3FF) == 1014 and rt == 0:
        if word & 1:
            raise UnsupportedInstruction(address, word, "reserved dcbz_l Rc bit is set")
        return _insn(address, word, Opcode.DCBZ_L, (ra, rb))

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
        if opcode == Opcode.LMW and ra >= rt and not allow_broadway_lmw_overlap:
            raise UnsupportedInstruction(address, word, "lmw base register is in the destination range")
        return _insn(address, word, opcode, (rt, ra, _signed(imm, 16)))

    fp_d_memory = {
        48: Opcode.LFS, 49: Opcode.LFSU, 50: Opcode.LFD, 51: Opcode.LFDU,
        52: Opcode.STFS, 53: Opcode.STFSU, 54: Opcode.STFD, 55: Opcode.STFDU,
    }
    if primary in fp_d_memory:
        opcode = fp_d_memory[primary]
        update = opcode in (Opcode.LFSU, Opcode.LFDU, Opcode.STFSU, Opcode.STFDU)
        if update and ra == 0:
            raise UnsupportedInstruction(address, word, "update-form FP memory access has RA=0")
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
        if xo == 50:
            if word != 0x4C000064:
                raise UnsupportedInstruction(address, word, "reserved rfi fields are nonzero")
            return _insn(address, word, Opcode.RFI, ())
        if xo == 150:
            if word != 0x4C00012C:
                raise UnsupportedInstruction(address, word, "reserved isync fields are nonzero")
            return _insn(address, word, Opcode.ISYNC, ())
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
            if (word & 0x00100801) != 0:
                raise UnsupportedInstruction(address, word, "reserved mtcrf bits are set")
            return _insn(address, word, Opcode.MTCRF, (rt, fxm))
        cache_ops = {
            54: Opcode.DCBST, 86: Opcode.DCBF, 278: Opcode.DCBT,
            470: Opcode.DCBI, 982: Opcode.ICBI, 1014: Opcode.DCBZ,
        }
        if xo in cache_ops:
            if rt != 0 or rc:
                raise UnsupportedInstruction(address, word, "reserved cache-operation fields are nonzero")
            return _insn(address, word, cache_ops[xo], (ra, rb))
        if xo == 598:
            if word != 0x7C0004AC:
                raise UnsupportedInstruction(address, word, "reserved sync fields are nonzero")
            return _insn(address, word, Opcode.SYNC, ())
        if xo == 83:
            if word & 0x001FFFFF != 0x000000A6:
                raise UnsupportedInstruction(address, word, "reserved mfmsr fields are nonzero")
            return _insn(address, word, Opcode.MFMSR, (rt,))
        if xo == 146:
            if word & 0x001FFFFF != 0x00000124:
                raise UnsupportedInstruction(address, word, "reserved mtmsr fields are nonzero")
            return _insn(address, word, Opcode.MTMSR, (rt,))
        if xo == 595:
            if word & 0x0010FFFF != 0x000004A6:
                raise UnsupportedInstruction(address, word, "reserved mfsr fields are nonzero")
            return _insn(address, word, Opcode.MFSR, (rt, (word >> 16) & 0xF))
        if xo == 210:
            if word & 0x0010FFFF != 0x000001A4:
                raise UnsupportedInstruction(address, word, "reserved mtsr fields are nonzero")
            return _insn(address, word, Opcode.MTSR, (rt, (word >> 16) & 0xF))
        if xo == 371:
            tbr = ((word >> 16) & 31) | (((word >> 11) & 31) << 5)
            if rc or tbr not in (268, 269):
                raise UnsupportedInstruction(address, word, f"unsupported time-base register {tbr}")
            return _insn(address, word, Opcode.MFTB, (rt, tbr))
        if xo in (339, 467):
            spr = ((word >> 16) & 31) | (((word >> 11) & 31) << 5)
            if rc:
                raise UnsupportedInstruction(address, word, "reserved SPR transfer Rc bit is set")
            allowed = READABLE_SPRS if xo == 339 else WRITABLE_SPRS
            if spr not in allowed:
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

        fp_x_memory = {
            535: Opcode.LFSX, 567: Opcode.LFSUX, 599: Opcode.LFDX, 631: Opcode.LFDUX,
            663: Opcode.STFSX, 695: Opcode.STFSUX, 727: Opcode.STFDX, 759: Opcode.STFDUX,
            983: Opcode.STFIWX,
        }
        if xo in fp_x_memory:
            opcode = fp_x_memory[xo]
            if rc:
                raise UnsupportedInstruction(address, word, "reserved FP memory Rc bit is set")
            update = opcode in (Opcode.LFSUX, Opcode.LFDUX, Opcode.STFSUX, Opcode.STFDUX)
            if update and ra == 0:
                raise UnsupportedInstruction(address, word, "update-form FP memory access has RA=0")
            return _insn(address, word, opcode, (rt, ra, rb))

    # Paired-single operations (primary 4).
    if primary == 4:
        xo5 = (word >> 1) & 0x1F
        xo10 = (word >> 1) & 0x3FF
        fd = (word >> 21) & 31
        fa = (word >> 16) & 31
        fb = (word >> 11) & 31
        fc = (word >> 6) & 31
        ps_5bit = {
            18: Opcode.PS_DIV, 20: Opcode.PS_SUB, 21: Opcode.PS_ADD,
            23: Opcode.PS_SEL, 24: Opcode.PS_RES, 25: Opcode.PS_MUL,
            26: Opcode.PS_RSQRTE, 28: Opcode.PS_MSUB, 29: Opcode.PS_MADD,
            30: Opcode.PS_NMSUB, 31: Opcode.PS_NMADD,
            10: Opcode.PS_SUM0, 11: Opcode.PS_SUM1, 12: Opcode.PS_MULS0,
            13: Opcode.PS_MULS1, 14: Opcode.PS_MADDS0, 15: Opcode.PS_MADDS1,
        }
        if xo5 == 6:
            if word & 1:
                raise UnsupportedInstruction(address, word, "reserved indexed PSQ Rc bit is set")
            opcode = Opcode.PSQ_LUX if word & 0x40 else Opcode.PSQ_LX
            ix = (word >> 7) & 7
            wx = (word >> 10) & 1
            if opcode == Opcode.PSQ_LUX and ra == 0:
                raise UnsupportedInstruction(address, word, "update-form PSQ memory access has RA=0")
            return _insn(address, word, opcode, (fd, ra, rb, wx, ix))
        if xo5 == 7:
            if word & 1:
                raise UnsupportedInstruction(address, word, "reserved indexed PSQ Rc bit is set")
            opcode = Opcode.PSQ_STUX if word & 0x40 else Opcode.PSQ_STX
            ix = (word >> 7) & 7
            wx = (word >> 10) & 1
            if opcode == Opcode.PSQ_STUX and ra == 0:
                raise UnsupportedInstruction(address, word, "update-form PSQ memory access has RA=0")
            return _insn(address, word, opcode, (fd, ra, rb, wx, ix))
        if xo5 in ps_5bit:
            opcode = ps_5bit[xo5]
            if opcode in (Opcode.PS_DIV, Opcode.PS_ADD, Opcode.PS_SUB):
                if fc:
                    raise UnsupportedInstruction(address, word, "reserved paired divide/add/sub FC field is nonzero")
                return _insn(address, word, opcode, (fd, fa, fb), record=bool(word & 1))
            if opcode in (Opcode.PS_RES, Opcode.PS_RSQRTE):
                if fa or fc:
                    raise UnsupportedInstruction(address, word, "reserved paired estimate FA/FC field is nonzero")
                return _insn(address, word, opcode, (fd, fb), record=bool(word & 1))
            if opcode in (Opcode.PS_MUL, Opcode.PS_MULS0, Opcode.PS_MULS1):
                if fb:
                    raise UnsupportedInstruction(address, word, "reserved paired multiply FB field is nonzero")
                return _insn(address, word, opcode, (fd, fa, fc), record=bool(word & 1))
            return _insn(address, word, opcode, (fd, fa, fb, fc), record=bool(word & 1))
        ps_10bit = {
            40: Opcode.PS_NEG, 72: Opcode.PS_MR, 136: Opcode.PS_NABS, 264: Opcode.PS_ABS,
            0: Opcode.PS_CMPU0, 32: Opcode.PS_CMPO0, 64: Opcode.PS_CMPU1, 96: Opcode.PS_CMPO1,
            528: Opcode.PS_MERGE00, 560: Opcode.PS_MERGE01,
            592: Opcode.PS_MERGE10, 624: Opcode.PS_MERGE11,
        }
        if xo10 in ps_10bit:
            opcode = ps_10bit[xo10]
            bf = (word >> 23) & 7
            if opcode in (Opcode.PS_CMPU0, Opcode.PS_CMPO0, Opcode.PS_CMPU1, Opcode.PS_CMPO1):
                if word & 0x00600001:
                    raise UnsupportedInstruction(address, word, "reserved paired-single compare field is nonzero")
                return _insn(address, word, opcode, (bf, fa, fb))
            if opcode in (Opcode.PS_NEG, Opcode.PS_MR, Opcode.PS_NABS, Opcode.PS_ABS):
                if fa:
                    raise UnsupportedInstruction(address, word, "reserved paired-single FA field is nonzero")
                return _insn(address, word, opcode, (fd, fb), record=bool(word & 1))
            return _insn(address, word, opcode, (fd, fa, fb), record=bool(word & 1))
        raise UnsupportedInstruction(address, word, f"unsupported paired-single instruction (xo5={xo5}, xo10={xo10})")

    # PSQ load/store D-form (primary 56-61).
    psq_d = {56: Opcode.PSQ_L, 57: Opcode.PSQ_LU, 60: Opcode.PSQ_ST, 61: Opcode.PSQ_STU}
    if primary in psq_d:
        opcode = psq_d[primary]
        rs = rt
        w = (word >> 15) & 1
        i = (word >> 12) & 7
        d12 = word & 0xFFF
        if opcode in (Opcode.PSQ_LU, Opcode.PSQ_STU) and ra == 0:
            raise UnsupportedInstruction(address, word, "update-form PSQ memory access has RA=0")
        return _insn(address, word, opcode, (rs, ra, _signed(d12, 12), w, i))

    # Single-precision floating-point arithmetic (primary 59) — A-form.
    if primary == 59:
        fd = rt
        fa = ra
        fb = rb
        fc = (word >> 6) & 31
        rc = bool(word & 1)
        sub = word & 0x3E
        sp_arith = {
            36: Opcode.FDIVS, 40: Opcode.FSUBS, 42: Opcode.FADDS,
            48: Opcode.FRES, 50: Opcode.FMULS,
            56: Opcode.FMSUBS, 58: Opcode.FMADDS, 60: Opcode.FNMSUBS, 62: Opcode.FNMADDS,
        }
        if sub not in sp_arith:
            raise UnsupportedInstruction(address, word, f"unsupported single-precision FP opcode (sub={sub})")
        opcode = sp_arith[sub]
        if opcode in (Opcode.FADDS, Opcode.FSUBS, Opcode.FDIVS) and fc != 0:
            raise UnsupportedInstruction(address, word, "reserved FP FC field is nonzero")
        if opcode == Opcode.FRES and (fa != 0 or fc != 0):
            raise UnsupportedInstruction(address, word, "reserved FP FA/FC field is nonzero")
        if opcode == Opcode.FMULS and fb != 0:
            raise UnsupportedInstruction(address, word, "reserved FP FB field is nonzero")
        return _insn(address, word, opcode, (fd, fa, fb, fc), record=rc)

    # Double-precision FP arithmetic + auxiliary (primary 63).
    if primary == 63:
        fd = rt
        fa = ra
        fb = rb
        fc = (word >> 6) & 31
        rc = bool(word & 1)
        is_double = bool(word & 32)
        if is_double:
            sub = word & 0x1E
            dp_arith = {
                4: Opcode.FDIV, 8: Opcode.FSUB, 10: Opcode.FADD,
                14: Opcode.FSEL, 18: Opcode.FMUL, 20: Opcode.FRSQRTE,
                24: Opcode.FMSUB, 26: Opcode.FMADD, 28: Opcode.FNMSUB, 30: Opcode.FNMADD,
            }
            if sub not in dp_arith:
                raise UnsupportedInstruction(address, word, f"unsupported double-precision FP opcode (sub={sub})")
            opcode = dp_arith[sub]
            if opcode in (Opcode.FADD, Opcode.FSUB, Opcode.FDIV) and fc != 0:
                raise UnsupportedInstruction(address, word, "reserved FP FC field is nonzero")
            if opcode == Opcode.FRSQRTE and (fa != 0 or fc != 0):
                raise UnsupportedInstruction(address, word, "reserved FP FA/FC field is nonzero")
            if opcode == Opcode.FMUL and fb != 0:
                raise UnsupportedInstruction(address, word, "reserved FP FB field is nonzero")
            return _insn(address, word, opcode, (fd, fa, fb, fc), record=rc)
        else:
            xo = (word >> 1) & 0x3FF
            aux = {
                0: Opcode.FCMPU, 12: Opcode.FRSP, 14: Opcode.FCTIW, 15: Opcode.FCTIWZ,
                32: Opcode.FCMPO, 38: Opcode.MTFSB1, 40: Opcode.FNEG,
                64: Opcode.MCRFS, 70: Opcode.MTFSB0, 72: Opcode.FMR,
                134: Opcode.MTFSFI, 136: Opcode.FNABS, 264: Opcode.FABS,
                583: Opcode.MFFS, 711: Opcode.MTFSF,
            }
            if xo in aux:
                opcode = aux[xo]
                if opcode in (Opcode.FCMPU, Opcode.FCMPO):
                    if word & 0x00600001:
                        raise UnsupportedInstruction(address, word, "reserved FP compare bits are set")
                    bf = (word >> 23) & 7
                    return _insn(address, word, opcode, (bf, fa, fb))
                if opcode == Opcode.MCRFS:
                    if (word & 0xFC63FFFF) != 0xFC000080:
                        raise UnsupportedInstruction(address, word, "reserved mcrfs fields are nonzero")
                    bf = (word >> 23) & 7
                    bfa = (word >> 18) & 7
                    return _insn(address, word, opcode, (bf, bfa))
                if opcode in (Opcode.MTFSB0, Opcode.MTFSB1):
                    expected = 0xFC00008C if opcode == Opcode.MTFSB0 else 0xFC00004C
                    if (word & 0xFC1FFFFE) != expected:
                        raise UnsupportedInstruction(address, word, "reserved mtfsb fields are nonzero")
                    bt = fd
                    return _insn(address, word, opcode, (bt,), record=rc)
                if opcode == Opcode.MTFSFI:
                    if (word & 0xFC7F0FFE) != 0xFC00010C:
                        raise UnsupportedInstruction(address, word, "reserved mtfsfi fields are nonzero")
                    bf = (word >> 23) & 7
                    imm4 = (word >> 12) & 0xF
                    return _insn(address, word, opcode, (bf, imm4), record=rc)
                if opcode == Opcode.MFFS:
                    if fa != 0 or fb != 0:
                        raise UnsupportedInstruction(address, word, "reserved mffs fields are nonzero")
                    return _insn(address, word, opcode, (fd,), record=rc)
                if opcode == Opcode.MTFSF:
                    if (word & 0xFE0107FE) != 0xFC00058E:
                        raise UnsupportedInstruction(address, word, "reserved mtfsf fields are nonzero")
                    fm = (word >> 17) & 0xFF
                    return _insn(address, word, opcode, (fm, fb), record=rc)
                if opcode in (Opcode.FRSP, Opcode.FCTIW, Opcode.FCTIWZ) and (fa != 0 or fc != 0):
                    raise UnsupportedInstruction(address, word, "reserved FP FA/FC field is nonzero")
                if opcode in (Opcode.FNEG, Opcode.FMR, Opcode.FNABS, Opcode.FABS) and fa != 0:
                    raise UnsupportedInstruction(address, word, "reserved FP FA field is nonzero")
                return _insn(address, word, opcode, (fd, fa, fb, fc), record=rc)
            raise UnsupportedInstruction(address, word, f"unsupported FP auxiliary opcode (xo={xo})")

    raise UnsupportedInstruction(address, word, f"unsupported primary opcode {primary}")


def _capstone_validate(code: bytes, base: int) -> list[str | None]:
    try:
        import capstone
    except ImportError:
        return [None] * (len(code) // 4)
    engine = capstone.Cs(capstone.CS_ARCH_PPC, capstone.CS_MODE_32 | capstone.CS_MODE_BIG_ENDIAN)
    # Capstone 5.0.6 has known PPC32 coverage gaps for valid OE forms. Decode
    # each fixed-width word independently so a missing display mnemonic does
    # not veto the field decoder that owns validity and semantics.
    mnemonics: list[str | None] = []
    for index in range(0, len(code), 4):
        address = base + index
        decoded = list(engine.disasm(code[index:index + 4], address, count=1))
        if not decoded:
            mnemonics.append(None)
            continue
        item = decoded[0]
        if item.address != address or item.size != 4:
            raise DecodeError("Capstone returned an inconsistent PPC32 instruction boundary")
        mnemonics.append(item.mnemonic)
    return mnemonics


def decode_block(
    code: bytes,
    base: int = 0,
    *,
    validate_with_capstone: bool = True,
    allow_broadway_lmw_overlap: bool = True,
    relocations: Sequence[Any] = (),
    canonical_symbols: dict[str, str] | None = None,
    local_symbol: str | None = None,
) -> list[Instruction]:
    if not code or len(code) % 4:
        raise DecodeError("PowerPC input must be a non-empty multiple of four bytes")
    if base < 0 or base > 0xFFFFFFFF or base % 4:
        raise DecodeError("base address must be a four-byte-aligned 32-bit value")
    if base + len(code) - 4 > 0xFFFFFFFF:
        raise DecodeError("instruction addresses wrap the 32-bit address space")
    if validate_with_capstone:
        mnemonics: Sequence[str | None] = _capstone_validate(code, base)
    else:
        mnemonics = [None] * (len(code) // 4)
    result: list[Instruction] = []
    for index in range(0, len(code), 4):
        insn = _decode_word(
            int.from_bytes(code[index:index + 4], "big"),
            base + index,
            allow_broadway_lmw_overlap=allow_broadway_lmw_overlap,
        )
        result.append(Instruction(
            insn.address, insn.raw, insn.opcode, insn.operands, insn.record,
            insn.overflow, insn.link, mnemonics[index // 4],
        ))
    if relocations:
        result = _attach_relocations(
            result, len(code), relocations,
            canonical_symbols=canonical_symbols or {},
            local_symbol=local_symbol,
        )
    return result


_ADDR16_OPCODES = frozenset({
    Opcode.ADDI, Opcode.ADDIS, Opcode.ADDIC, Opcode.ADDIC_DOT,
    Opcode.ORI,
    Opcode.LBZ, Opcode.LBZU, Opcode.LHZ, Opcode.LHZU, Opcode.LHA,
    Opcode.LHAU, Opcode.LWZ, Opcode.LWZU, Opcode.STB, Opcode.STBU,
    Opcode.STH, Opcode.STHU, Opcode.STW, Opcode.STWU,
    Opcode.LFS, Opcode.LFSU, Opcode.LFD, Opcode.LFDU,
    Opcode.STFS, Opcode.STFSU, Opcode.STFD, Opcode.STFDU,
})

_SDA21_OPCODES = frozenset({
    Opcode.ADDI,
    Opcode.LBZ, Opcode.LHZ, Opcode.LHA, Opcode.LWZ,
    Opcode.STB, Opcode.STH, Opcode.STW,
    Opcode.LFS, Opcode.LFD, Opcode.STFS,
})


def _attach_relocations(
    instructions: list[Instruction],
    code_size: int,
    relocations: Sequence[Any],
    *,
    canonical_symbols: dict[str, str],
    local_symbol: str | None,
) -> list[Instruction]:
    """Attach validated ELF relocation records to decoded instruction fields."""
    result = list(instructions)
    occupied: set[int] = set()
    for item in relocations:
        offset = int(item.offset)
        relocation_type = int(item.relocation_type)
        symbol = str(item.symbol)
        addend_value = item.addend
        if relocation_type not in SUPPORTED_TEXT_RELOCATIONS:
            raise DecodeError(
                f"unsupported text relocation type {relocation_type} "
                f"for {symbol or '<section>'} at +0x{offset:x}"
            )
        if addend_value is None:
            # The checked objects currently use RELA. Recovering implicit REL
            # addends is type-specific; guessing zero would be unsound.
            raise DecodeError(
                f"implicit REL addend is unsupported for relocation type "
                f"{relocation_type} at +0x{offset:x}"
            )
        if relocation_type in (R_PPC_ADDR16_LO, R_PPC_ADDR16_HI, R_PPC_ADDR16_HA):
            if offset % 4 != 2:
                raise DecodeError(
                    f"16-bit relocation at +0x{offset:x} does not address an instruction low half"
                )
            instruction_offset = offset - 2
        else:
            if offset % 4:
                raise DecodeError(
                    f"word relocation at +0x{offset:x} is not instruction-aligned"
                )
            instruction_offset = offset
        if instruction_offset < 0 or instruction_offset + 4 > code_size:
            raise DecodeError(f"relocation at +0x{offset:x} lies outside function bytes")
        index = instruction_offset // 4
        if index in occupied:
            raise DecodeError(f"multiple relocations affect instruction at +0x{instruction_offset:x}")
        insn = result[index]
        if relocation_type == R_PPC_ADDR16_LO and insn.opcode not in _ADDR16_OPCODES:
            raise DecodeError(f"ADDR16_LO is unsupported on {insn.opcode.value} at 0x{insn.address:08x}")
        if relocation_type in (R_PPC_ADDR16_HI, R_PPC_ADDR16_HA) and insn.opcode != Opcode.ADDIS:
            raise DecodeError(f"ADDR16_HI/HA requires addis at 0x{insn.address:08x}")
        if relocation_type == R_PPC_REL24 and insn.opcode != Opcode.B:
            raise DecodeError(f"REL24 requires b/bl at 0x{insn.address:08x}")
        if relocation_type == R_PPC_REL14 and insn.opcode != Opcode.BC:
            raise DecodeError(f"REL14 requires bc/bcl at 0x{insn.address:08x}")
        if relocation_type == R_PPC_EMB_SDA21 and insn.opcode not in _SDA21_OPCODES:
            raise DecodeError(f"EMB_SDA21 is unsupported on {insn.opcode.value} at 0x{insn.address:08x}")
        if (
            local_symbol is not None
            and symbol == local_symbol
            and relocation_type in (R_PPC_REL24, R_PPC_REL14)
        ):
            target = (instructions[0].address + int(addend_value)) & 0xFFFFFFFF
            operands = (
                (target, insn.operands[1])
                if insn.opcode == Opcode.B
                else (insn.operands[0], insn.operands[1], target, insn.operands[3])
            )
            result[index] = replace(insn, operands=operands)
            occupied.add(index)
            continue
        canonical = canonical_symbols.get(symbol, symbol)
        if not canonical:
            raise DecodeError(f"relocation at +0x{offset:x} has no canonical symbol identity")
        result[index] = replace(
            insn,
            relocation=RelocationRef(
                offset=offset,
                relocation_type=relocation_type,
                symbol=symbol,
                canonical_symbol=canonical,
                addend=int(addend_value),
            ),
        )
        occupied.add(index)
    return result
