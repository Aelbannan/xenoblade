#!/usr/bin/env python3
"""Headless instruction-level diff for a single function symbol.

Usage:
  python3 tools/coop/hexdiff.py <unit-or-source> --symbol <mangled-name>
  python3 tools/coop/hexdiff.py kyoshin/COccCulling --symbol func_801A1188__11COccCullingFP12CCullFrustum
  python3 tools/coop/hexdiff.py src/kyoshin/COccCulling.cpp -s cbRenderBefore

Outputs a side-by-side hex diff with highlighting.
Green = match, Red = mismatch (retail vs decomp), Yellow = reloc placeholder.

If --build is passed, runs `ninja` on the decomp object first.
"""

from __future__ import annotations

import argparse
import subprocess
import sys
from pathlib import Path
from typing import Optional

# Ensure the repo root is on sys.path.
_REPO = Path(__file__).resolve().parent.parent.parent
sys.path.insert(0, str(_REPO))

from tools.coop.lib.config import CoopConfig, load_config
from tools.coop.lib.project import Project
from tools.ppc_equivalence.elf_symbols import list_text_functions, FunctionBytes

# ── mini PowerPC disassembler ──────────────────────────────────────────────

# Opcode fields: primary opcode (bits 0-5), extended opcode varies.
# We decode just enough to show readable mnemonics next to hex.

_GPR = [
    "r0", "sp", "r2", "r3", "r4", "r5", "r6", "r7",
    "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15",
    "r16", "r17", "r18", "r19", "r20", "r21", "r22", "r23",
    "r24", "r25", "r26", "r27", "r28", "r29", "r30", "r31",
]
_FPR = [f"f{i}" for i in range(32)]
_CR = ["cr0", "cr1", "cr2", "cr3", "cr4", "cr5", "cr6", "cr7"]


def _simm16(insn: int) -> int:
    v = insn & 0xFFFF
    return v - 0x10000 if v & 0x8000 else v


def _uimm16(insn: int) -> int:
    return insn & 0xFFFF


def _rd(insn: int) -> int:
    return (insn >> 21) & 0x1F


def _ra(insn: int) -> int:
    return (insn >> 16) & 0x1F


def _rb(insn: int) -> int:
    return (insn >> 11) & 0x1F


def _rc(insn: int) -> int:
    return insn & 1


def _li(insn: int) -> int:
    return _simm16(insn)


def _bd(insn: int) -> int:
    return _simm16(insn)


def _aa(insn: int) -> int:
    return (insn >> 1) & 1


def _lk(insn: int) -> int:
    return insn & 1


def _branch_target(insn: int, addr: int) -> int:
    aa, lk = _aa(insn), _lk(insn)
    disp = _li(insn)
    if aa:
        return disp
    return addr + disp


def _crb(insn: int) -> int:
    return (insn >> 21) & 0x1F  # crbA for branches


def _cr_bit(cr: int) -> str:
    base = cr // 4
    bit = cr % 4
    names = ["lt", "gt", "eq", "so"]
    return f"cr{base}[{names[bit]}]"


def disasm_one(insn: int, addr: int = 0) -> str:
    """Decode a single PowerPC instruction to a readable mnemonic + operands."""
    opc = insn >> 26
    oe = (insn >> 21) & 1  # OE bit in extended opcodes

    # ── primary opcodes ──
    if opc == 14:  # addi / addic / subi / li
        rD, rA = _rd(insn), _ra(insn)
        simm = _simm16(insn)
        if rA == 0:
            return f"li {_GPR[rD]}, {simm}"
        if simm < 0:
            return f"subi {_GPR[rD]}, {_GPR[rA]}, {-simm}"
        return f"addi {_GPR[rD]}, {_GPR[rA]}, {simm}"
    if opc == 15:  # addis / lis
        rD, rA = _rd(insn), _ra(insn)
        simm = _simm16(insn)
        if rA == 0:
            return f"lis {_GPR[rD]}, {simm}"
        return f"addis {_GPR[rD]}, {_GPR[rA]}, {simm}"
    if opc == 18:  # b / bl / ba / bla
        target = _branch_target(insn, addr)
        mnem = "b"
        if _aa(insn):
            mnem += "a"
        if _lk(insn):
            mnem += "l"
        return f"{mnem} 0x{target & 0xFFFFFFFF:08X}"
    if opc == 16:  # bc / bcl / bca / bcla
        bo = (insn >> 21) & 0x1F
        bi = (insn >> 16) & 0x1F
        target = _branch_target(insn, addr)
        mnem = "bc"
        if _aa(insn):
            mnem += "a"
        if _lk(insn):
            mnem += "l"
        return f"{mnem} {bo}, {bi}, 0x{target & 0xFFFFFFFF:08X}"
    if opc == 19:  # extended opcode
        xo = (insn >> 1) & 0x3FF
        if xo == 16:  # bclr
            bo, bi = (insn >> 21) & 0x1F, (insn >> 16) & 0x1F
            suffix = "l" if _lk(insn) else ""
            return f"bclr{suffix} {bo}, {bi}"
        if xo == 528:  # bcctr
            bo, bi = (insn >> 21) & 0x1F, (insn >> 16) & 0x1F
            suffix = "l" if _lk(insn) else ""
            return f"bcctr{suffix} {bo}, {bi}"
        if xo == 8:  # mtcrf
            return f"mtcrf 0x{_rd(insn):02X}, {_GPR[_ra(insn)]}"
        if xo == 144:  # mtcr
            return f"mtcr {_GPR[_rd(insn)]}"
        if xo == 150:  # stwcx.
            return f"stwcx. {_GPR[_rd(insn)]}, {_GPR[_ra(insn)]}, {_GPR[_rb(insn)]}"
        if xo == 0:  # mcrf
            crD = _rd(insn) >> 2
            crS = _ra(insn) >> 2
            return f"mcrf cr{crD}, cr{crS}"
        if xo == 33:  # crnor
            crD, crA, crB = _rd(insn), _ra(insn), _rb(insn)
            return f"crnor {_cr_bit(crD)}, {_cr_bit(crA)}, {_cr_bit(crB)}"
        if xo == 193:  # crxor
            crD, crA, crB = _rd(insn), _ra(insn), _rb(insn)
            return f"crxor {_cr_bit(crD)}, {_cr_bit(crA)}, {_cr_bit(crB)}"
        if xo == 225:  # crandc
            crD, crA, crB = _rd(insn), _ra(insn), _rb(insn)
            return f"crandc {_cr_bit(crD)}, {_cr_bit(crA)}, {_cr_bit(crB)}"
        if xo == 257:  # crand
            crD, crA, crB = _rd(insn), _ra(insn), _rb(insn)
            return f"crand {_cr_bit(crD)}, {_cr_bit(crA)}, {_cr_bit(crB)}"
        if xo == 289:  # creqv
            crD, crA, crB = _rd(insn), _ra(insn), _rb(insn)
            return f"creqv {_cr_bit(crD)}, {_cr_bit(crA)}, {_cr_bit(crB)}"
        if xo == 417:  # crorc
            crD, crA, crB = _rd(insn), _ra(insn), _rb(insn)
            return f"crorc {_cr_bit(crD)}, {_cr_bit(crA)}, {_cr_bit(crB)}"
        if xo == 449:  # cror
            crD, crA, crB = _rd(insn), _ra(insn), _rb(insn)
            return f"cror {_cr_bit(crD)}, {_cr_bit(crA)}, {_cr_bit(crB)}"
        if xo == 50:  # cntlzw
            return f"cntlzw {_GPR[_ra(insn)]}, {_GPR[_rd(insn)]}"
        return f"op19_{xo} r{_rd(insn)}, r{_ra(insn)}, r{_rb(insn)}"
    if opc == 21:  # rlwinm
        rD, rA = _rd(insn), _ra(insn)
        sh = (insn >> 11) & 0x1F
        mb = (insn >> 6) & 0x1F
        me = (insn >> 1) & 0x1F
        return f"rlwinm {_GPR[rA]}, {_GPR[rD]}, {sh}, {mb}, {me}"
    if opc == 31:  # extended opcode 2
        xo = (insn >> 1) & 0x3FF
        rD, rA, rB = _rd(insn), _ra(insn), _rb(insn)
        rc = "."
        if xo == 444:  # or
            return f"or {_GPR[rA]}, {_GPR[rD]}, {_GPR[rB]}"
        if xo == 124:  # nor
            return f"nor {_GPR[rA]}, {_GPR[rD]}, {_GPR[rB]}"
        if xo == 28:  # and
            return f"and {_GPR[rA]}, {_GPR[rD]}, {_GPR[rB]}"
        if xo == 60:  # andc
            return f"andc {_GPR[rA]}, {_GPR[rD]}, {_GPR[rB]}"
        if xo == 476:  # nand
            return f"nand {_GPR[rA]}, {_GPR[rD]}, {_GPR[rB]}"
        if xo == 412:  # orc
            return f"orc {_GPR[rA]}, {_GPR[rD]}, {_GPR[rB]}"
        if xo == 284:  # eqv / xor (eqv=(A&~B)|(~A&B)? no, xor=A^B)
            pass  # fall through
        if xo == 316:  # xor
            return f"xor {_GPR[rA]}, {_GPR[rD]}, {_GPR[rB]}"
        if xo == 339:  # mfspr
            spr = ((insn >> 11) & 0x1F) << 5 | ((insn >> 1) & 0x1F)
            return f"mfspr {_GPR[rD]}, {spr}"
        if xo == 467:  # mtspr
            spr = ((insn >> 11) & 0x1F) << 5 | ((insn >> 1) & 0x1F)
            return f"mtspr {spr}, {_GPR[rD]}"
        if xo == 19:  # mfcr
            return f"mfcr {_GPR[rD]}"
        if xo == 83:  # mfmsr
            return f"mfmsr {_GPR[rD]}"
        if xo == 144:  # mtcrf
            crm = (insn >> 12) & 0xFF
            return f"mtcrf 0x{crm:02X}, {_GPR[rD]}"
        if xo == 146:  # mtmsr
            return f"mtmsr {_GPR[rD]}"
        if xo == 210:  # mtsr
            sr = (insn >> 16) & 0xF
            return f"mtsr {sr}, {_GPR[rD]}"
        if xo == 242:  # mtsrin
            return f"mtsrin {_GPR[rD]}, {_GPR[rB]}"
        if xo == 306:  # tlbie
            return f"tlbie {_GPR[rB]}"
        if xo == 370:  # tlbia
            return f"tlbia"
        if xo == 566:  # tlbsync
            return f"tlbsync"
        if xo == 854:  # eieio
            return f"eieio"
        if xo == 246:  # dcbtst
            return f"dcbtst {_GPR[rA]}, {_GPR[rB]}"
        if xo == 54:  # dcbst
            return f"dcbst {_GPR[rA]}, {_GPR[rB]}"
        if xo == 86:  # dcbf
            return f"dcbf {_GPR[rA]}, {_GPR[rB]}"
        if xo == 758:  # dcba
            return f"dcba {_GPR[rA]}, {_GPR[rB]}"
        if xo == 1014:  # dcbz
            return f"dcbz {_GPR[rA]}, {_GPR[rB]}"
        if xo == 470:  # dcbi
            return f"dcbi {_GPR[rA]}, {_GPR[rB]}"
        if xo == 598:  # sync
            l = (insn >> 10) & 1
            return f"sync{l}"
        if xo == 150:  # stwcx.
            return f"stwcx. {_GPR[rD]}, 0({_GPR[rA]})"
        if xo == 20:  # lwarx
            return f"lwarx {_GPR[rD]}, 0({_GPR[rA]})"
        if xo == 23:  # lwzx
            return f"lwzx {_GPR[rD]}, {_GPR[rA]}, {_GPR[rB]}"
        if xo == 55:  # lwzux
            return f"lwzux {_GPR[rD]}, {_GPR[rA]}, {_GPR[rB]}"
        if xo == 87:  # lbzx
            return f"lbzx {_GPR[rD]}, {_GPR[rA]}, {_GPR[rB]}"
        if xo == 119:  # lbzux
            return f"lbzux {_GPR[rD]}, {_GPR[rA]}, {_GPR[rB]}"
        if xo == 279:  # lhzx
            return f"lhzx {_GPR[rD]}, {_GPR[rA]}, {_GPR[rB]}"
        if xo == 311:  # lhzux
            return f"lhzux {_GPR[rD]}, {_GPR[rA]}, {_GPR[rB]}"
        if xo == 343:  # lhax
            return f"lhax {_GPR[rD]}, {_GPR[rA]}, {_GPR[rB]}"
        if xo == 375:  # lhaux
            return f"lhaux {_GPR[rD]}, {_GPR[rA]}, {_GPR[rB]}"
        if xo == 23:  # lwzx
            return f"lwzx {_GPR[rD]}, {_GPR[rA]}, {_GPR[rB]}"
        if xo == 151:  # stwx
            return f"stwx {_GPR[rD]}, {_GPR[rA]}, {_GPR[rB]}"
        if xo == 183:  # stwux
            return f"stwux {_GPR[rD]}, {_GPR[rA]}, {_GPR[rB]}"
        if xo == 215:  # stbx
            return f"stbx {_GPR[rD]}, {_GPR[rA]}, {_GPR[rB]}"
        if xo == 247:  # stbux
            return f"stbux {_GPR[rD]}, {_GPR[rA]}, {_GPR[rB]}"
        if xo == 407:  # sthx
            return f"sthx {_GPR[rD]}, {_GPR[rA]}, {_GPR[rB]}"
        if xo == 439:  # sthux
            return f"sthux {_GPR[rD]}, {_GPR[rA]}, {_GPR[rB]}"
        if xo == 24:  # slw
            return f"slw {_GPR[rA]}, {_GPR[rD]}, {_GPR[rB]}"
        if xo == 536:  # srw
            return f"srw {_GPR[rA]}, {_GPR[rD]}, {_GPR[rB]}"
        if xo == 792:  # sraw
            return f"sraw {_GPR[rA]}, {_GPR[rD]}, {_GPR[rB]}"
        if xo == 824:  # srawi
            sh = (insn >> 11) & 0x1F
            return f"srawi {_GPR[rA]}, {_GPR[rD]}, {sh}"
        if xo == 266:  # add
            return f"add {_GPR[rD]}, {_GPR[rA]}, {_GPR[rB]}"
        if xo == 40:  # subf
            return f"subf {_GPR[rD]}, {_GPR[rA]}, {_GPR[rB]}"
        if xo == 75:  # mulhw
            return f"mulhw {_GPR[rD]}, {_GPR[rA]}, {_GPR[rB]}"
        if xo == 235:  # mullw
            return f"mullw {_GPR[rD]}, {_GPR[rA]}, {_GPR[rB]}"
        if xo == 459:  # divwu ?
            return f"divwu {_GPR[rD]}, {_GPR[rA]}, {_GPR[rB]}"
        if xo == 491:  # divw
            return f"divw {_GPR[rD]}, {_GPR[rA]}, {_GPR[rB]}"
        if xo == 8:  # subfc
            return f"subfc {_GPR[rD]}, {_GPR[rA]}, {_GPR[rB]}"
        if xo == 10:  # addc
            return f"addc {_GPR[rD]}, {_GPR[rA]}, {_GPR[rB]}"
        if xo == 104:  # neg
            return f"neg {_GPR[rD]}, {_GPR[rA]}"
        if xo == 0:  # cmp
            l = (insn >> 10) & 1
            cr = _rd(insn) >> 2
            return f"cmp cr{cr}, {l}, {_GPR[rA]}, {_GPR[rB]}"
        if xo == 32:  # cmpl
            l = (insn >> 10) & 1
            cr = _rd(insn) >> 2
            return f"cmpl cr{cr}, {l}, {_GPR[rA]}, {_GPR[rB]}"
        if xo == 144:  # mtcrf
            crm = (insn >> 12) & 0xFF
            return f"mtcrf 0x{crm:02X}, {_GPR[rD]}"
        return f"op31_{xo} r{_rd(insn)}, r{_ra(insn)}, r{_rb(insn)}"
    if opc == 32:  # lwz
        return f"lwz {_GPR[_rd(insn)]}, {_simm16(insn)}({_GPR[_ra(insn)]})"
    if opc == 33:  # lwzu
        return f"lwzu {_GPR[_rd(insn)]}, {_simm16(insn)}({_GPR[_ra(insn)]})"
    if opc == 34:  # lbz
        return f"lbz {_GPR[_rd(insn)]}, {_simm16(insn)}({_GPR[_ra(insn)]})"
    if opc == 35:  # lbzu
        return f"lbzu {_GPR[_rd(insn)]}, {_simm16(insn)}({_GPR[_ra(insn)]})"
    if opc == 36:  # stw
        return f"stw {_GPR[_rd(insn)]}, {_simm16(insn)}({_GPR[_ra(insn)]})"
    if opc == 37:  # stwu
        return f"stwu {_GPR[_rd(insn)]}, {_simm16(insn)}({_GPR[_ra(insn)]})"
    if opc == 38:  # stb
        return f"stb {_GPR[_rd(insn)]}, {_simm16(insn)}({_GPR[_ra(insn)]})"
    if opc == 39:  # stbu
        return f"stbu {_GPR[_rd(insn)]}, {_simm16(insn)}({_GPR[_ra(insn)]})"
    if opc == 40:  # lhz
        return f"lhz {_GPR[_rd(insn)]}, {_simm16(insn)}({_GPR[_ra(insn)]})"
    if opc == 41:  # lhzu
        return f"lhzu {_GPR[_rd(insn)]}, {_simm16(insn)}({_GPR[_ra(insn)]})"
    if opc == 42:  # lha
        return f"lha {_GPR[_rd(insn)]}, {_simm16(insn)}({_GPR[_ra(insn)]})"
    if opc == 43:  # lhau
        return f"lhau {_GPR[_rd(insn)]}, {_simm16(insn)}({_GPR[_ra(insn)]})"
    if opc == 44:  # sth
        return f"sth {_GPR[_rd(insn)]}, {_simm16(insn)}({_GPR[_ra(insn)]})"
    if opc == 45:  # sthu
        return f"sthu {_GPR[_rd(insn)]}, {_simm16(insn)}({_GPR[_ra(insn)]})"
    if opc == 46:  # lmw
        return f"lmw {_GPR[_rd(insn)]}, {_simm16(insn)}({_GPR[_ra(insn)]})"
    if opc == 47:  # stmw
        return f"stmw {_GPR[_rd(insn)]}, {_simm16(insn)}({_GPR[_ra(insn)]})"
    if opc == 48:  # lfs
        return f"lfs {_FPR[_rd(insn)]}, {_simm16(insn)}({_GPR[_ra(insn)]})"
    if opc == 49:  # lfsu
        return f"lfsu {_FPR[_rd(insn)]}, {_simm16(insn)}({_GPR[_ra(insn)]})"
    if opc == 50:  # lfd
        return f"lfd {_FPR[_rd(insn)]}, {_simm16(insn)}({_GPR[_ra(insn)]})"
    if opc == 51:  # lfdu
        return f"lfdu {_FPR[_rd(insn)]}, {_simm16(insn)}({_GPR[_ra(insn)]})"
    if opc == 52:  # stfs
        return f"stfs {_FPR[_rd(insn)]}, {_simm16(insn)}({_GPR[_ra(insn)]})"
    if opc == 53:  # stfsu
        return f"stfsu {_FPR[_rd(insn)]}, {_simm16(insn)}({_GPR[_ra(insn)]})"
    if opc == 54:  # stfd
        return f"stfd {_FPR[_rd(insn)]}, {_simm16(insn)}({_GPR[_ra(insn)]})"
    if opc == 55:  # stfdu
        return f"stfdu {_FPR[_rd(insn)]}, {_simm16(insn)}({_GPR[_ra(insn)]})"
    if opc == 56:  # psq_l
        w = (insn >> 16) & 1
        qr = (_rd(insn) >> 3) & 0x1F if w else 0
        uimm12 = insn & 0xFFF
        return f"psq_l {_FPR[_rd(insn) & 0x1F]}, {uimm12}({_GPR[_ra(insn)]}), {w}, qr{qr}"
    if opc == 57:  # psq_lu
        w = (insn >> 16) & 1
        qr = (_rd(insn) >> 3) & 0x1F if w else 0
        return f"psq_lu {_FPR[_rd(insn) & 0x1F]}, {_simm16(insn) & 0xFFF}({_GPR[_ra(insn)]}), {w}, qr{qr}"
    if opc == 60:  # psq_st
        w = (insn >> 16) & 1
        qr = (_rd(insn) >> 3) & 0x1F if w else 0
        uimm12 = insn & 0xFFF
        return f"psq_st {_FPR[_rd(insn) & 0x1F]}, {uimm12}({_GPR[_ra(insn)]}), {w}, qr{qr}"
    if opc == 61:  # psq_stu
        w = (insn >> 16) & 1
        qr = (_rd(insn) >> 3) & 0x1F if w else 0
        return f"psq_stu {_FPR[_rd(insn) & 0x1F]}, {_simm16(insn) & 0xFFF}({_GPR[_ra(insn)]}), {w}, qr{qr}"
    if opc == 10:  # cmpli
        cr = _rd(insn) >> 2
        return f"cmpli cr{cr}, 0, {_GPR[_ra(insn)]}, {_uimm16(insn)}"
    if opc == 11:  # cmpi
        cr = _rd(insn) >> 2
        return f"cmpi cr{cr}, 0, {_GPR[_ra(insn)]}, {_simm16(insn)}"
    if opc == 12:  # addic
        return f"addic {_GPR[_rd(insn)]}, {_GPR[_ra(insn)]}, {_simm16(insn)}"
    if opc == 13:  # addic. (addic with Rc=1)
        return f"addic. {_GPR[_rd(insn)]}, {_GPR[_ra(insn)]}, {_simm16(insn)}"
    if opc == 7:  # mulli
        return f"mulli {_GPR[_rd(insn)]}, {_GPR[_ra(insn)]}, {_simm16(insn)}"
    if opc == 8:  # subfic
        return f"subfic {_GPR[_rd(insn)]}, {_GPR[_ra(insn)]}, {_simm16(insn)}"
    if opc == 24:  # ori
        return f"ori {_GPR[_ra(insn)]}, {_GPR[_rd(insn)]}, 0x{_uimm16(insn):04X}"
    if opc == 25:  # oris
        return f"oris {_GPR[_ra(insn)]}, {_GPR[_rd(insn)]}, 0x{_uimm16(insn):04X}"
    if opc == 26:  # xori
        return f"xori {_GPR[_ra(insn)]}, {_GPR[_rd(insn)]}, 0x{_uimm16(insn):04X}"
    if opc == 27:  # xoris
        return f"xoris {_GPR[_ra(insn)]}, {_GPR[_rd(insn)]}, 0x{_uimm16(insn):04X}"
    if opc == 28:  # andi.
        return f"andi. {_GPR[_ra(insn)]}, {_GPR[_rd(insn)]}, 0x{_uimm16(insn):04X}"
    if opc == 29:  # andis.
        return f"andis. {_GPR[_ra(insn)]}, {_GPR[_rd(insn)]}, 0x{_uimm16(insn):04X}"
    if opc == 58:  # lwzu x-form (paired single)
        pass  # fall through
    if opc == 4:  # ps_cmpo0 / paired single compare
        return f"ps_cmpo0 f{_rd(insn)}, f{_ra(insn)}"

    if opc == 63:  # FP arithmetic
        xo = (insn >> 1) & 0x1F
        rc = insn & 1
        rD = _rd(insn)
        rA = _ra(insn)
        rB = _rb(insn)
        rc_s = "." if rc else ""
        fmt = (insn >> 21) & 0x1F  # fmt field for FP
        if fmt == 21:  # fadds / fsubs / etc (single-precision)
            if xo == 21:
                return f"fadds{rc_s} f{rD}, f{rA}, f{rB}"
            if xo == 20:
                return f"fsubs{rc_s} f{rD}, f{rA}, f{rB}"
            if xo == 25:
                return f"fmuls{rc_s} f{rD}, f{rA}, f{rC if 'C' in str(locals()) else rB}"
            if xo == 18:
                return f"fdivs{rc_s} f{rD}, f{rA}, f{rB}"
            if xo == 12:
                return f"frsp{rc_s} f{rD}, f{rB}"
            if xo == 14:
                return f"fctiwz{rc_s} f{rD}, f{rB}"
            if xo == 15:
                return f"fctiw{rc_s} f{rD}, f{rB}"
            if xo == 32:
                return f"fcmpo cr{_rd(insn) >> 2}, f{rA}, f{rB}"
            if xo == 0:
                return f"fcmpu cr{_rd(insn) >> 2}, f{rA}, f{rB}"
            if xo == 40:
                return f"fneg{rc_s} f{rD}, f{rB}"
            if xo == 72:
                return f"fmr{rc_s} f{rD}, f{rB}"
            if xo == 136:
                return f"fnabs{rc_s} f{rD}, f{rB}"
            if xo == 264:
                return f"fabs{rc_s} f{rD}, f{rB}"
            # ps_* instructions
            if xo == 6:  # psq_lx
                return f"psq_lx f{rD}, r{rA}, r{rB}"
            if xo == 7:  # psq_stx
                return f"psq_stx f{rD}, r{rA}, r{rB}"
            if xo == 10:  # ps_sum0
                return f"ps_sum0 f{rD}, f{rA}, f{rC if 'C' in str(locals()) else rB}, f{rB}"
            if xo == 11:  # ps_sum1
                return f"ps_sum1 f{rD}, f{rA}, f{rB}, f{rC if 'C' in str(locals()) else rB}"
            if xo == 12:  # ps_muls0
                return f"ps_muls0 f{rD}, f{rA}, f{rC if 'C' in str(locals()) else rB}"
            if xo == 13:  # ps_muls1
                return f"ps_muls1 f{rD}, f{rA}, f{rC if 'C' in str(locals()) else rB}"
            if xo == 14:  # ps_madds0
                return f"ps_madds0 f{rD}, f{rA}, f{rC if 'C' in str(locals()) else rB}, f{rB}"
            if xo == 15:  # ps_madds1
                return f"ps_madds1 f{rD}, f{rA}, f{rC if 'C' in str(locals()) else rB}, f{rB}"
            if xo == 16:  # ps_nabs
                return f"ps_nabs f{rD}, f{rB}"
            if xo == 18:  # ps_div
                return f"ps_div f{rD}, f{rA}, f{rB}"
            if xo == 20:  # ps_sub
                return f"ps_sub f{rD}, f{rA}, f{rB}"
            if xo == 21:  # ps_add
                return f"ps_add f{rD}, f{rA}, f{rB}"
            if xo == 22:  # ps_sel
                return f"ps_sel f{rD}, f{rA}, f{rC if 'C' in str(locals()) else rB}"
            if xo == 23:  # ps_res
                return f"ps_res f{rD}, f{rB}"
            if xo == 24:  # ps_mul
                return f"ps_mul f{rD}, f{rA}, f{rC if 'C' in str(locals()) else rB}"
            if xo == 25:  # ps_msub
                return f"ps_msub f{rD}, f{rA}, f{rC if 'C' in str(locals()) else rB}, f{rB}"
            if xo == 26:  # ps_mr
                return f"ps_mr f{rD}, f{rB}"
            if xo == 28:  # ps_nmadd
                return f"ps_nmadd f{rD}, f{rA}, f{rC if 'C' in str(locals()) else rB}, f{rB}"
            if xo == 29:  # ps_madd
                return f"ps_madd f{rD}, f{rA}, f{rC if 'C' in str(locals()) else rB}, f{rB}"
            if xo == 30:  # ps_neg
                return f"ps_neg f{rD}, f{rB}"
            if xo == 31:  # ps_cmpo0
                return f"ps_cmpo0 cr{_rd(insn)>>2}, f{rA}, f{rB}"
            if xo == 40:  # ps_cmpu0
                return f"ps_cmpu0 cr{_rd(insn)>>2}, f{rA}, f{rB}"
    if opc == 59:  # FP single/double paired single
        xo = (insn >> 1) & 0x1F
        rD, rA, rB, rC = _rd(insn), _ra(insn), _rb(insn), (insn >> 6) & 0x1F
        if xo == 18:  # fdivs
            return f"fdivs f{rD}, f{rA}, f{rB}"
        if xo == 20:  # fsubs
            return f"fsubs f{rD}, f{rA}, f{rB}"
        if xo == 21:  # fadds
            return f"fadds f{rD}, f{rA}, f{rB}"
        if xo == 25:  # fmuls
            return f"fmuls f{rD}, f{rA}, f{rC}"
        if xo == 28:  # fmsubs
            return f"fmsubs f{rD}, f{rA}, f{rC}, f{rB}"
        if xo == 29:  # fmadds
            return f"fmadds f{rD}, f{rA}, f{rC}, f{rB}"
        if xo == 30:  # fnmsubs
            return f"fnmsubs f{rD}, f{rA}, f{rC}, f{rB}"
        if xo == 31:  # fnmadds
            return f"fnmadds f{rD}, f{rA}, f{rC}, f{rB}"

    return f".4byte 0x{insn:08X}"


# ── terminal colour helpers ────────────────────────────────────────────────

_GREEN  = "\033[32m"
_RED    = "\033[31m"
_YELLOW = "\033[33m"
_CYAN   = "\033[36m"
_DIM    = "\033[2m"
_RESET  = "\033[0m"


def _colour_diff(retail_word: int, decomp_word: int, decomp_has_reloc: bool) -> str:
    if decomp_has_reloc:
        return _YELLOW
    if retail_word == decomp_word:
        return _GREEN
    return _RED


# ── main entry point ────────────────────────────────────────────────────────

def _parse_args(argv: list[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Headless instruction-level hex diff for one function.",
    )
    parser.add_argument(
        "unit",
        help="objdiff unit hint (e.g. kyoshin/COccCulling) or source path",
    )
    parser.add_argument(
        "-s", "--symbol",
        required=True,
        help="function symbol (mangled name, e.g. func_801A1188__11COccCullingFP12CCullFrustum)",
    )
    parser.add_argument(
        "--no-build",
        action="store_true",
        help="do not build the decomp object before diffing",
    )
    parser.add_argument(
        "--json",
        action="store_true",
        help="output machine-readable JSON instead of colour terminal diff",
    )
    parser.add_argument(
        "--relocs",
        action="store_true",
        help="show relocation table for both retail and decomp objects",
    )
    return parser.parse_args(argv)


def run(argv: list[str] | None = None) -> int:
    args = _parse_args(argv)

    config = load_config(None, _REPO)
    project = Project(config)

    # Resolve unit → retail + decomp .o paths
    unit = project.resolve_unit(args.unit)
    retail_path, decomp_path = _object_paths_for_unit(project, unit)
    if retail_path is None:
        print(f"ERROR: no retail object for unit {unit.name}", file=sys.stderr)
        return 1
    if decomp_path is None:
        print(f"ERROR: no decomp (base) object for unit {unit.name}", file=sys.stderr)
        return 1

    # Build decomp if needed
    if not args.no_build:
        rel_path = str(decomp_path.relative_to(project.root))
        print(f"building {rel_path} ...", file=sys.stderr, flush=True)
        # Build with output redirected to stderr so --json stdout is clean
        build_result = subprocess.run(
            [project.ninja_bin(), rel_path],
            cwd=project.root, check=False, capture_output=True, text=True,
        )
        if build_result.returncode != 0:
            print(build_result.stdout, file=sys.stderr)
            print(build_result.stderr, file=sys.stderr)
            print(f"ERROR: build failed for {rel_path}", file=sys.stderr)
            return 2
        print(build_result.stdout, file=sys.stderr)
        _postprocess_mtrand_object(project, decomp_path)
        if not decomp_path.is_file():
            print(f"ERROR: build failed for {rel_path}", file=sys.stderr)
            return 2

    # Extract function bytes
    try:
        retail_fn = list_text_functions(retail_path)
        decomp_fn = list_text_functions(decomp_path)
    except Exception as exc:
        print(f"ERROR reading objects: {exc}", file=sys.stderr)
        return 3

    # Resolve symbol
    retail_match = _resolve_candidates(retail_fn, args.symbol)
    decomp_match = _resolve_candidates(decomp_fn, args.symbol)

    if not retail_match:
        print(f"ERROR: symbol {args.symbol!r} not found in {retail_path}", file=sys.stderr)
        print(f"  available: {', '.join(f.name for f in retail_fn[:20])}", file=sys.stderr)
        return 4
    if not decomp_match:
        print(f"ERROR: symbol {args.symbol!r} not found in {decomp_path}", file=sys.stderr)
        print(f"  available: {', '.join(f.name for f in decomp_fn[:20])}", file=sys.stderr)
        return 4

    retail = retail_match[0]
    decomp = decomp_match[0]

    # Build relocation offset sets
    retail_reloc_offsets = frozenset(r.offset for r in retail.relocations)
    decomp_reloc_offsets = frozenset(r.offset for r in decomp.relocations)

    if args.json:
        return _output_json(args, retail, decomp, retail_reloc_offsets, decomp_reloc_offsets)
    return _output_terminal(args, retail, decomp, retail_reloc_offsets, decomp_reloc_offsets)


def _resolve_candidates(functions: list[FunctionBytes], symbol: str) -> list[FunctionBytes]:
    exact = [f for f in functions if f.name == symbol]
    if exact:
        return exact
    lowered = symbol.lower()
    ci = [f for f in functions if f.name.lower() == lowered]
    if ci:
        return ci
    partial = [f for f in functions if lowered in f.name.lower()]
    return partial


def _object_paths_for_unit(project: Project, unit) -> tuple[Path | None, Path | None]:
    """Return (retail_path, decomp_path) for an objdiff unit."""
    retail = unit.target_path
    decomp = unit.base_path
    return retail, decomp


def _postprocess_mtrand_object(project: Project, obj: Path | None) -> None:
    """Run reloc-name post-processing (PLAN.md §17.6)."""
    if obj is None:
        return
    script = project.root / "tools" / "postprocess_reloc_names.py"
    if not script.is_file():
        return
    subprocess.run(
        [sys.executable, str(script), str(obj)],
        cwd=project.root, check=False,
        capture_output=True,
    )


def _output_terminal(
    args: argparse.Namespace,
    retail: FunctionBytes,
    decomp: FunctionBytes,
    retail_relocs: frozenset,
    decomp_relocs: frozenset,
) -> int:
    print(f"function: {retail.name}")
    print(f"  retail:  {retail.path}  (0x{retail.size:x} bytes)")
    print(f"  decomp:  {decomp.path}  (0x{decomp.size:x} bytes)")
    print()

    if retail.size != decomp.size:
        print(
            f"{_RED}WARNING: size mismatch — retail=0x{retail.size:x} "
            f"decomp=0x{decomp.size:x}{_RESET}\n"
        )

    # Print relocation info if requested
    if args.relocs:
        if retail.relocations:
            print(f"{_CYAN}Retail relocations:{_RESET}")
            for r in retail.relocations:
                print(f"  +0x{r.offset:04x} type={r.relocation_type} "
                      f"symbol={r.symbol} addend={r.addend}")
        if decomp.relocations:
            print(f"{_CYAN}Decomp relocations:{_RESET}")
            for r in decomp.relocations:
                print(f"  +0x{r.offset:04x} type={r.relocation_type} "
                      f"symbol={r.symbol} addend={r.addend}")
        print()

    # Side-by-side hex + disasm diff
    byte_mismatches = 0
    reloc_count = 0
    total = max(retail.size, decomp.size)
    retail_base = retail.base
    decomp_base = decomp.base

    for offset in range(0, total, 4):
        r_word = int.from_bytes(retail.code[offset:offset + 4], "big") if offset + 4 <= retail.size else None
        d_word = int.from_bytes(decomp.code[offset:offset + 4], "big") if offset + 4 <= decomp.size else None

        d_has_reloc = offset in decomp_relocs

        if r_word is None:
            r_hex = "         "
            r_asm = ""
        else:
            r_hex = f"0x{r_word:08X}"
            r_asm = disasm_one(r_word, retail_base + offset)

        if d_word is None:
            d_hex = "         "
            d_asm = ""
        else:
            d_hex = f"0x{d_word:08X}"
            d_asm = disasm_one(d_word, decomp_base + offset)

        colour = _colour_diff(r_word, d_word, d_has_reloc)
        match_char = "=" if r_word == d_word else "≠"

        if r_word != d_word or d_has_reloc:
            if r_word != d_word:
                byte_mismatches += 1
            if d_has_reloc:
                reloc_count += 1

        print(
            f"  {_DIM}+0x{offset:04x}{_RESET}  "
            f"{colour}{r_hex}{_RESET}  {r_asm:<40s}  "
            f"{colour}{d_hex}{_RESET}  {d_asm}"
        )

    print()
    if byte_mismatches == 0 and reloc_count == 0:
        print(f"{_GREEN}✓ 100% match — {total // 4} instructions identical{_RESET}")
    else:
        parts = []
        if byte_mismatches:
            pct = 100.0 * (total // 4 - byte_mismatches) / (total // 4)
            parts.append(f"{_RED}{byte_mismatches} byte mismatch(es) ({pct:.1f}% match){_RESET}")
        if reloc_count:
            parts.append(f"{_YELLOW}{reloc_count} unresolved relocation(s){_RESET}")
        print(f"  " + ", ".join(parts))

    return 0 if byte_mismatches == 0 else 5


def _output_json(
    args: argparse.Namespace,
    retail: FunctionBytes,
    decomp: FunctionBytes,
    retail_relocs: frozenset,
    decomp_relocs: frozenset,
) -> int:
    import json

    diffs = []
    total = max(retail.size, decomp.size)
    for offset in range(0, total, 4):
        r_word = int.from_bytes(retail.code[offset:offset + 4], "big") if offset + 4 <= retail.size else None
        d_word = int.from_bytes(decomp.code[offset:offset + 4], "big") if offset + 4 <= decomp.size else None
        diffs.append({
            "offset": offset,
            "retail_hex": f"0x{r_word:08X}" if r_word is not None else None,
            "decomp_hex": f"0x{d_word:08X}" if d_word is not None else None,
            "match": r_word == d_word,
            "has_decomp_reloc": offset in decomp_relocs,
        })

    output = {
        "symbol": retail.name,
        "retail_path": str(retail.path),
        "decomp_path": str(decomp.path),
        "retail_size": retail.size,
        "decomp_size": decomp.size,
        "total_instructions": total // 4,
        "mismatch_count": sum(1 for d in diffs if not d["match"]),
        "instructions": diffs,
        "retail_relocations": [
            {"offset": r.offset, "type": r.relocation_type, "symbol": r.symbol, "addend": r.addend}
            for r in retail.relocations
        ],
        "decomp_relocations": [
            {"offset": r.offset, "type": r.relocation_type, "symbol": r.symbol, "addend": r.addend}
            for r in decomp.relocations
        ],
    }
    print(json.dumps(output, indent=2))
    return 0 if all(d["match"] for d in diffs) else 5


if __name__ == "__main__":
    raise SystemExit(run())
