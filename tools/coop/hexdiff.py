#!/usr/bin/env python3
"""Headless instruction-level diff for a single function symbol.

Usage:
  python3 tools/coop/hexdiff.py <unit-or-source> --symbol <mangled-name>
  python3 tools/coop/hexdiff.py kyoshin/COccCulling --symbol func_801A1188__11COccCullingFP12CCullFrustum
  python3 tools/coop/hexdiff.py src/kyoshin/COccCulling.cpp -s cbRenderBefore

Outputs a side-by-side hex diff with highlighting.
Green = match, Red = mismatch (retail vs decomp), Yellow = reloc placeholder.

If --build is passed, runs `ninja` on the decomp object first.

Extra modes:
  python3 tools/coop/hexdiff.py <unit> --all          # one build, per-function table
  python3 tools/coop/hexdiff.py <unit> --list [SUBSTR]  # retail function symbols (no build)
  python3 tools/coop/hexdiff.py <unit> -s <sym> --brief  # one-line verdict + mismatches only
  python3 tools/coop/hexdiff.py <unit> -s <sym> --asm    # full clean disassembly, both sides
"""

from __future__ import annotations

import argparse
import json
import os
import re
import subprocess
import sys
from pathlib import Path
from typing import Optional

# Ensure the repo root is on sys.path.
_REPO = Path(__file__).resolve().parent.parent.parent
sys.path.insert(0, str(_REPO))

from tools.coop.lib.config import CoopConfig, load_config
from tools.coop.lib.object_size import check_object_size, format_size_check
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


def _pure_reg_swap(r_word: int, d_word: int, r_mnem: str, d_mnem: str) -> bool:
    """Role-table refinement of the reg-swap classifier (doc 31, additive).

    True when the two words differ ONLY in 5-bit GPR/FPR register fields per
    the operand-role table in ``tools/coop/lib/renaming_witness.py``: every
    non-register bit (opcode/XO, Rc/OE, immediates, branch displacements,
    BO/BI/BH, CR bits/fields, SPR indices, FXM masks, LK/AA) must be
    bit-equal.  A same-mnemonic mismatch with a differing immediate (e.g.
    ``addi r3,r4,1`` vs ``addi r3,r4,2``) is structural, not a pure reg-swap.

    Unknown opcodes fall back to the historical heuristic (same mnemonic
    implies registers-only) so the classifier never regresses on opcodes the
    mini-disassembler handles but the role table does not.
    """
    if not r_mnem or r_mnem != d_mnem:
        return False
    try:
        from tools.coop.lib.renaming_witness import _gpr_fpr_masks
        from tools.ppc_equivalence.decoder import _decode_word

        opcode = _decode_word(r_word, 0, allow_broadway_lmw_overlap=True).opcode
    except Exception:
        return True
    gpr_mask, fpr_mask = _gpr_fpr_masks(opcode)
    non_register = (r_word ^ d_word) & ~(gpr_mask | fpr_mask)
    return non_register == 0


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
        required=False,
        help="function symbol (mangled name, e.g. func_801A1188__11COccCullingFP12CCullFrustum)",
    )
    parser.add_argument(
        "--all",
        action="store_true",
        help="one build, then a per-function match table for the whole unit (no --symbol needed)",
    )
    parser.add_argument(
        "--list",
        nargs="?",
        const="",
        default=None,
        metavar="SUBSTR",
        help="list retail function symbols (address | size | name), optionally filtered by SUBSTR; no build",
    )
    parser.add_argument(
        "--brief",
        action="store_true",
        help="one-line verdict first, then only mismatched instructions (no full diff dump)",
    )
    parser.add_argument(
        "--asm",
        action="store_true",
        help="full clean disassembly of both sides instead of the diff (replaces objdump)",
    )
    parser.add_argument(
        "--no-build",
        action="store_true",
        help="do not build the decomp object before diffing",
    )
    parser.add_argument(
        "--build-timeout",
        type=int,
        default=600,
        metavar="SECONDS",
        help="build timeout in seconds (default: 600)",
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

    modes = sum(1 for x in (args.symbol, args.all, args.list is not None) if x)
    if modes != 1:
        print(
            "ERROR: exactly one of --symbol / --all / --list is required",
            file=sys.stderr,
        )
        return 2

    config = load_config(None, _REPO)
    project = Project(config)

    # Resolve unit → retail + decomp .o paths
    unit = project.resolve_unit(args.unit)
    retail_path, decomp_path = _object_paths_for_unit(project, unit)
    if retail_path is None:
        print(f"ERROR: no retail object for unit {unit.name}", file=sys.stderr)
        return 1

    if args.list is not None:
        return _cmd_list(args, retail_path)

    if decomp_path is None:
        print(f"ERROR: no decomp (base) object for unit {unit.name}", file=sys.stderr)
        return 1

    # Build decomp if needed. Ninja's own .ninja_lock serialises concurrent
    # builds in the shared build directory; no outer lock is needed.
    if not args.no_build:
        rel_path = str(decomp_path.relative_to(project.root))

        def _run_build() -> subprocess.CompletedProcess:
            # Output redirected to stderr so --json stdout stays clean
            return subprocess.run(
                [project.ninja_bin(), rel_path],
                cwd=project.root, check=False, capture_output=True, text=True,
                timeout=args.build_timeout,
            )

        try:
            build_result = _run_build()
        except subprocess.TimeoutExpired:
            print(f"ERROR: build timed out after {args.build_timeout}s for {rel_path}",
                  file=sys.stderr)
            return 2

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

    if args.all:
        return _cmd_all(args, project, unit, retail_path, decomp_path, config)

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
    if not decomp_match and retail_match:
        # If the symbol resolved in the retail object but not in the decomp
        # object (name mismatch between retail and decompiled symbols), try
        # matching by offset (value) so the diff still works.
        retail_val = retail_match[0].value
        decomp_match = [f for f in decomp_fn if f.value == retail_val]
        if not decomp_match:
            print(f"ERROR: symbol {args.symbol!r} not found in {decomp_path}", file=sys.stderr)
            print(f"  available: {', '.join(f.name for f in decomp_fn[:20])}", file=sys.stderr)
            return 4
    elif not decomp_match:
        print(f"ERROR: symbol {args.symbol!r} not found in {decomp_path}", file=sys.stderr)
        print(f"  available: {', '.join(f.name for f in decomp_fn[:20])}", file=sys.stderr)
        return 4

    retail = retail_match[0]
    decomp = decomp_match[0]
    compiler_cfg = _unit_compiler_config(project, unit.name)

    if args.asm:
        return _cmd_asm(args, retail, decomp, compiler_cfg)

    # Build relocation offset sets
    retail_reloc_offsets = frozenset(r.offset for r in retail.relocations)
    decomp_reloc_offsets = frozenset(r.offset for r in decomp.relocations)

    # Split-budget check (blocker #9): compare the whole unit's decomp .text
    # against the retail split budget, so overflow is caught in the hexdiff
    # loop instead of failing at `cycle` time. Runs on the freshly built object.
    size_check = check_object_size(
        project_root=project.root,
        region=config.region,
        unit_hint=unit.name,
        retail_object=retail_path,
        decomp_object=decomp_path,
    )

    # Reloc name-drift analysis (MWCC_REFERENCE §1): classify per-offset reloc
    # differences (name drift vs addend drift vs structural) and look up fixes
    # in the mined map (tools/coop/retail_reloc_map.json, built by
    # tools/coop/reloc_map.py mine). Failures degrade to "no drift info".
    reloc_drifts = []
    reloc_suggestions = {}
    try:
        from tools.coop.reloc_map import DEFAULT_MAP, analyze_function_pair, load_map, suggestions

        reloc_map = load_map(DEFAULT_MAP) if DEFAULT_MAP.is_file() else {}
        reloc_drifts = analyze_function_pair(retail, decomp)
        decomp_obj = decomp_path.stem
        reloc_suggestions = {
            f"0x{d.offset:04x}": suggestions(d, unit.name, decomp_obj, reloc_map)
            for d in reloc_drifts
        }
    except Exception:
        reloc_drifts = []
        reloc_suggestions = {}

    if args.json:
        return _output_json(args, retail, decomp, retail_reloc_offsets, decomp_reloc_offsets, size_check, reloc_drifts, reloc_suggestions)
    return _output_terminal(args, retail, decomp, retail_reloc_offsets, decomp_reloc_offsets, size_check, reloc_drifts, reloc_suggestions, compiler_cfg=compiler_cfg, unit_name=unit.name)


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
    # Also copy .note.split from retail to decomp
    notesplit_script = project.root / "tools" / "postprocess_notesplit.py"
    if notesplit_script.is_file():
        subprocess.run(
            [sys.executable, str(notesplit_script), str(obj)],
            cwd=project.root, check=False,
        )


# ── new modes: --all / --list / --brief / --asm / compiler line / KB hints ──

def _counts_for(retail: FunctionBytes, decomp: FunctionBytes) -> dict:
    """Classify every 4-byte slot; return counts only (no rendering).

    Mirrors the classification loop used by the terminal/JSON outputs so
    --all / --brief share the same rules (pure reg-swap vs structural).
    """
    total = max(retail.size, decomp.size)
    decomp_relocs = frozenset(r.offset for r in decomp.relocations)
    mismatch = reg_swap = pure_reg_swap = structural = reloc = 0
    for offset in range(0, total, 4):
        r_word = int.from_bytes(retail.code[offset:offset + 4], "big") if offset + 4 <= retail.size else None
        d_word = int.from_bytes(decomp.code[offset:offset + 4], "big") if offset + 4 <= decomp.size else None
        d_has_reloc = offset in decomp_relocs
        if d_has_reloc:
            reloc += 1
        if r_word != d_word:
            mismatch += 1
            if r_word is not None and d_word is not None:
                r_asm = disasm_one(r_word, 0) or ""
                d_asm = disasm_one(d_word, 0) or ""
                r_mnem = r_asm.split()[0] if r_asm else ""
                d_mnem = d_asm.split()[0] if d_asm else ""
                if r_mnem and r_mnem == d_mnem:
                    reg_swap += 1
                    if _pure_reg_swap(r_word, d_word, r_mnem, d_mnem):
                        pure_reg_swap += 1
                else:
                    structural += 1
            else:
                structural += 1
    return {
        "total": total // 4,
        "mismatch": mismatch,
        "reg_swap": reg_swap,
        "pure_reg_swap": pure_reg_swap,
        "structural": structural,
        "reloc": reloc,
    }


def _summary_line(retail: FunctionBytes, decomp: FunctionBytes, c: dict, size_check=None) -> str:
    """One-line triage verdict: pct | structural | reg_swap | size."""
    total = c["total"] or 1
    pct = 100.0 * (total - c["mismatch"]) / total
    color = _GREEN if c["mismatch"] == 0 and c["reloc"] == 0 else _RED
    size = f"0x{retail.size:x}/0x{decomp.size:x}"
    if size_check is not None and size_check.budget is not None:
        size += " PASS" if size_check.ok else f" OVER({size_check.over_by})"
    return (
        f"{retail.name}: {color}{pct:.1f}%{_RESET} | "
        f"{c['structural']} structural | {c['reg_swap']} reg_swap | {size}"
    )


def _unit_compiler_config(project: Project, unit_name: str) -> str:
    """Return the unit's configured compiler/flags from configure.py, or "".

    Extracts mw_version / extra_cflags from the Object(...) entry matching the
    unit, falling back to the lib-level mw_version default. Kills the grep
    configure.py archaeology loop.
    """
    cfg = project.root / "configure.py"
    try:
        text = cfg.read_text(encoding="utf-8", errors="replace")
    except OSError:
        return ""
    for line in text.splitlines():
        if f'"{unit_name}' in line and "Object(" in line:
            mw = re.search(r'mw_version="([^"]+)"', line)
            flags = re.search(r'extra_cflags=\[([^\]]*)\]', line)
            parts = []
            if mw:
                parts.append(mw.group(1))
            if flags:
                fl = re.findall(r'"([^"]+)"', flags.group(1))
                if fl:
                    parts.append(" ".join(fl))
            if parts:
                return " ".join(parts)
            # Object(...) with no explicit flags → falls through to lib default.
    # Unit names may carry a plan/workspace prefix (e.g. "main/RVL_SDK/...");
    # configure.py keys on the path from the lib root ("RVL_SDK/...").
    idx = unit_name.find("/RVL_SDK/")
    if idx >= 0:
        stripped = unit_name[idx + 1:]
        for line in text.splitlines():
            if f'"{stripped}' in line and "Object(" in line:
                mw = re.search(r'mw_version="([^"]+)"', line)
                flags = re.search(r'extra_cflags=\[([^\]]*)\]', line)
                parts = []
                if mw:
                    parts.append(mw.group(1))
                if flags:
                    fl = re.findall(r'"([^"]+)"', flags.group(1))
                    if fl:
                        parts.append(" ".join(fl))
                if parts:
                    return " ".join(parts)
    for line in text.splitlines():
        if any(k in line for k in ("DolphinLib(", "criwareLib(", "nw4rLib(")) and "RVL_SDK" in line:
            mw = re.search(r'mw_version="([^"]+)"', line)
            if mw:
                return mw.group(1) + " (lib default)"
    return ""


def _kb_hints(unit_name: str, c: dict, retail: FunctionBytes, decomp: FunctionBytes, compiler_cfg: str = "") -> list[str]:
    """Detect known MWCC_REFERENCE stall signatures and point at the section."""
    hints: list[str] = []

    def _words(fn: FunctionBytes) -> list[int]:
        return [int.from_bytes(fn.code[i:i + 4], "big") for i in range(0, len(fn.code), 4)]

    def _align_nop_after_mtctr(words: list[int]) -> bool:
        # ori r0,r0,0 = 0x60000000; mtctr rS = 0x7C000000 | rS<<21 | 0x3A6
        for i, w in enumerate(words):
            if w == 0x60000000:
                for j in range(max(0, i - 3), min(len(words), i + 4)):
                    if (words[j] & 0xFC0007FF) == 0x7C0003A6:
                        return True
        return False

    r_nop = _align_nop_after_mtctr(_words(retail))
    d_nop = _align_nop_after_mtctr(_words(decomp))
    if r_nop != d_nop and c["structural"] > 0:
        side = "decomp" if d_nop else "retail"
        hints.append(
            f"alignment nop (`ori r0,r0,0`) near `mtctr` on the {side} side only — classic "
            "-func_align / -ipa scheduling artifact. See docs/MWCC_REFERENCE.md "
            "(btm_inq / gki notes): add `-func_align 4` (and `-ipa off` if it persists) "
            "via extra_cflags on the unit's Object(...)."
        )
    if "/bte/" in unit_name and c["structural"] > 0 and "func_align 4" not in compiler_cfg:
        hints.append(
            f"bte-family unit is compiled with {compiler_cfg or 'defaults'} — retail bte uses "
            "-func_align 4 (GC/3.0a5.2). If you see scheduling nops / extra padding, set "
            'mw_version="GC/3.0a5.2" + extra_cflags=["-func_align 4"] on the Object(...) '
            "(docs/MWCC_REFERENCE.md)."
        )
    return hints


def _cmd_list(args: argparse.Namespace, retail_path: Path) -> int:
    """--list: print retail function symbols (address | size | name). No build."""
    try:
        retail_fn = list_text_functions(retail_path)
    except Exception as exc:
        print(f"ERROR reading {retail_path}: {exc}", file=sys.stderr)
        return 3
    sub = (args.list or "").lower()
    rows = sorted(retail_fn, key=lambda f: f.value)
    if sub:
        rows = [f for f in rows if sub in f.name.lower()]
    for f in rows:
        print(f"  0x{f.value:08x}  0x{f.size:x}  {f.name}")
    print(f"-- {len(rows)} function(s) in {retail_path}", file=sys.stderr)
    return 0


def _cmd_all(args: argparse.Namespace, project: Project, unit, retail_path: Path, decomp_path: Path, config) -> int:
    """--all: one build, per-function table for the whole unit."""
    try:
        retail_fn = list_text_functions(retail_path)
        decomp_fn = list_text_functions(decomp_path)
    except Exception as exc:
        print(f"ERROR reading objects: {exc}", file=sys.stderr)
        return 3

    compiler_cfg = _unit_compiler_config(project, unit.name)
    print(f"unit: {unit.name}  —  {len(retail_fn)} function(s)"
          + (f"  |  compiler: {compiler_cfg}" if compiler_cfg else ""))

    try:
        size_check = check_object_size(
            project_root=project.root, region=config.region, unit_hint=unit.name,
            retail_object=retail_path, decomp_object=decomp_path,
        )
        if size_check is not None and size_check.budget is not None:
            color = _GREEN if size_check.ok else _RED
            print(f"split: {color}{format_size_check(size_check)}{_RESET}")
    except Exception:
        size_check = None

    decomp_by_name = {f.name: f for f in decomp_fn}
    decomp_by_value = {f.value: f for f in decomp_fn}
    rows = []
    for rf in sorted(retail_fn, key=lambda f: f.value):
        dm = decomp_by_name.get(rf.name) or decomp_by_value.get(rf.value)
        if dm is None:
            rows.append((rf, None, None))
            continue
        rows.append((rf, dm, _counts_for(rf, dm)))

    n_match = 0
    print(f"\n  {'':4s} {'MATCH':>7s} {'STRUC':>5s} {'REGSW':>5s} {'SIZE':>12s}  SYMBOL")
    for rf, dm, c in rows:
        if dm is None:
            print(f"  {'-':4s} {'n/a':>7s} {'-':>5s} {'-':>5s} {f'0x{rf.size:x}':>12s}  {rf.name}  (not written yet)")
            continue
        ok = c["mismatch"] == 0
        if ok:
            n_match += 1
        mark = "✓" if ok else "✗"
        pct = 100.0 * (c["total"] - c["mismatch"]) / (c["total"] or 1)
        print(
            f"  {mark:4s} {pct:6.1f}% {c['structural']:5d} {c['reg_swap']:5d} "
            f"{f'0x{rf.size:x}/0x{dm.size:x}':>12s}  {rf.name}"
        )
    print(f"\n{n_match}/{len(retail_fn)} functions fully matched")
    return 0 if n_match == len(retail_fn) else 5


def _cmd_asm(args: argparse.Namespace, retail: FunctionBytes, decomp: FunctionBytes, compiler_cfg: str = "") -> int:
    """--asm: full clean disassembly of both sides, no diff annotations."""
    if compiler_cfg:
        print(f"compiler: {compiler_cfg}")
    for label, fn, base in (("retail", retail, retail.base), ("decomp", decomp, decomp.base)):
        print(f"\n=== {label}: {fn.name} (0x{fn.size:x} bytes) ===")
        for i in range(0, len(fn.code), 4):
            word = int.from_bytes(fn.code[i:i + 4], "big")
            print(f"  0x{base + i:08x}  {word:08X}  {disasm_one(word, base + i)}")
    return 0


def _output_terminal(
    args: argparse.Namespace,
    retail: FunctionBytes,
    decomp: FunctionBytes,
    retail_relocs: frozenset,
    decomp_relocs: frozenset,
    size_check=None,
    reloc_drifts=None,
    reloc_suggestions=None,
    compiler_cfg: str = "",
    unit_name: str = "",
) -> int:
    reloc_drifts = reloc_drifts or []
    reloc_suggestions = reloc_suggestions or {}

    # One-line triage verdict first (kills the grep-the-JSON loop).
    counts = _counts_for(retail, decomp)
    print(_summary_line(retail, decomp, counts, size_check))
    if compiler_cfg:
        print(f"{_DIM}compiler: {compiler_cfg}{_RESET}")

    if not args.brief:
        print(f"function: {retail.name}")
        print(f"  retail:  {retail.path}  (0x{retail.size:x} bytes)")
        print(f"  decomp:  {decomp.path}  (0x{decomp.size:x} bytes)")
        if size_check is not None and size_check.budget is not None:
            color = _GREEN if size_check.ok else _RED
            print(f"  split:   {color}{format_size_check(size_check)}{_RESET}")
            if size_check.split_path:
                print(f"           split: {size_check.split_path}")
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
    reg_swap_count = 0
    pure_reg_swap_count = 0
    structural_count = 0
    total = max(retail.size, decomp.size)
    retail_base = retail.base
    decomp_base = decomp.base

    # Reg-map: (retail_reg, instruction_short) → set of decomp_regs
    reg_map: dict[str, set[int]] = {}

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

        # Detect pure register swap: same mnemonic base, different registers
        is_reg_swap = False
        pure_reg_swap = False
        if r_word is not None and d_word is not None and r_asm and d_asm and r_word != d_word:
            r_mnem_base = r_asm.split()[0] if r_asm else ""
            d_mnem_base = d_asm.split()[0] if d_asm else ""
            if r_mnem_base and r_mnem_base == d_mnem_base:
                is_reg_swap = True
                # Doc-31 operand-role refinement (additive): a pure reg-swap
                # must differ only in gpr/fpr register fields.
                pure_reg_swap = _pure_reg_swap(r_word, d_word, r_mnem_base, d_mnem_base)
                # Build register mapping: extract GPR/FPR operands from both sides
                for fn, word in [("retail", r_word), ("decomp", d_word)]:
                    pass
                r_regs = set()
                d_regs = set()
                for reg_list, w in [(_GPR, r_word), (_GPR, d_word)]:
                    pass
                r_rd, r_ra, r_rb = _rd(r_word), _ra(r_word), _rb(r_word)
                d_rd, d_ra, d_rb = _rd(d_word), _ra(d_word), _rb(d_word)
                # For each operand position that differs, record the mapping
                op_positions = [("rd", r_rd, d_rd), ("ra", r_ra, d_ra), ("rb", r_rb, d_rb)]
                for pos, r_r, d_r in op_positions:
                    if r_r != d_r:
                        key = f"{r_mnem_base}*{pos}"
                        reg_map.setdefault(key, set()).add((r_r, d_r))

        colour = _colour_diff(r_word, d_word, d_has_reloc)
        match_char = "=" if r_word == d_word else "≠"

        if r_word != d_word or d_has_reloc:
            if r_word != d_word:
                byte_mismatches += 1
                if is_reg_swap:
                    reg_swap_count += 1
                    if pure_reg_swap:
                        pure_reg_swap_count += 1
                else:
                    structural_count += 1
            if d_has_reloc:
                reloc_count += 1

        if args.brief and r_word == d_word and not d_has_reloc:
            continue

        print(
            f"  {_DIM}+0x{offset:04x}{_RESET}  "
            f"{colour}{r_hex}{_RESET}  {r_asm:<40s}  "
            f"{colour}{d_hex}{_RESET}  {d_asm}"
        )

    print()
    if byte_mismatches == 0 and reloc_count == 0:
        print(f"{_GREEN}✓ 100% match — {total // 4} instructions identical{_RESET}")
    elif not args.brief:
        parts = []
        if byte_mismatches:
            pct = 100.0 * (total // 4 - byte_mismatches) / (total // 4)
            parts.append(f"{_RED}{byte_mismatches} mismatch(es) ({pct:.1f}% match){_RESET}")
            if reg_swap_count:
                suffix = "" if reg_swap_count == 1 else "s"
                parts.append(f"{_CYAN}{reg_swap_count} pure reg-swap{suffix} ({100*reg_swap_count//byte_mismatches}%){_RESET}")
                if pure_reg_swap_count:
                    suffix = "" if pure_reg_swap_count == 1 else "s"
                    parts.append(
                        f"{_DIM}{pure_reg_swap_count} role-clean swap{suffix} "
                        f"(doc-31 role table){_RESET}"
                    )
        if reloc_count:
            parts.append(f"{_YELLOW}{reloc_count} unresolved relocation(s){_RESET}")
        print(f"  " + ", ".join(parts))

    # Print register mapping table if any reg-swaps were detected
    if reg_map:
        print(f"\n{_CYAN}Register mapping (retail → decomp):{_RESET}")
        for key in sorted(reg_map):
            pairs = reg_map[key]
            # Group by same retail reg
            by_r: dict[int, list[int]] = {}
            for r_r, d_r in pairs:
                by_r.setdefault(r_r, []).append(d_r)
            parts_list = []
            for r_r in sorted(by_r):
                d_regs = sorted(set(by_r[r_r]))
                if len(d_regs) == 1:
                    parts_list.append(f"r{r_r}→{_GPR[d_regs[0]]}")
                else:
                    parts_list.append(f"r{r_r}→{','.join(_GPR[dr] for dr in d_regs)}")
            instr_part = key.split("*")[0]
            joined = ", ".join(parts_list)
            print(f"  {instr_part:<12s}  {joined}")

    # Reloc name-drift summary (MWCC_REFERENCE §1) — the #1 cause of
    # 99.3-99.9% near-misses: instructions byte-identical, reloc names differ.
    if reloc_drifts:
        print(f"\n{_YELLOW}Reloc name drift ({len(reloc_drifts)}):{_RESET}")
        for d in reloc_drifts:
            delta = f" (addend delta {d.addend_delta:+d})" if d.addend_delta else ""
            print(
                f"  +0x{d.offset:04x} {d.type_name:20s} {d.kind:10s} "
                f"{d.retail_symbol} → {d.decomp_symbol}{delta}"
            )
            for line in reloc_suggestions.get(f"0x{d.offset:04x}", []):
                print(f"      {line}")
        if not reloc_suggestions:
            print(
                f"      (no map — run `python3 tools/coop/reloc_map.py mine` "
                f"to build tools/coop/retail_reloc_map.json)"
            )

    # KB hints: known MWCC_REFERENCE stall signatures (compiler/flags).
    hints = _kb_hints(unit_name, counts, retail, decomp, compiler_cfg)
    if hints:
        print(f"\n{_CYAN}KB hints:{_RESET}")
        for h in hints:
            print(f"  - {h}")

    return 0 if byte_mismatches == 0 else 5


def _output_json(
    args: argparse.Namespace,
    retail: FunctionBytes,
    decomp: FunctionBytes,
    retail_relocs: frozenset,
    decomp_relocs: frozenset,
    size_check=None,
    reloc_drifts=None,
    reloc_suggestions=None,
) -> int:
    import json

    import collections

    reloc_drifts = reloc_drifts or []
    reloc_suggestions = reloc_suggestions or {}
    diffs = []
    total = max(retail.size, decomp.size)
    retail_base = retail.base
    decomp_base = decomp.base

    reg_swap_count = 0
    pure_reg_swap_count = 0
    structural_count = 0
    reg_map: dict[str, set[tuple[int, int]]] = collections.defaultdict(set)

    for offset in range(0, total, 4):
        r_word = int.from_bytes(retail.code[offset:offset + 4], "big") if offset + 4 <= retail.size else None
        d_word = int.from_bytes(decomp.code[offset:offset + 4], "big") if offset + 4 <= decomp.size else None

        r_asm = disasm_one(r_word, retail_base + offset) if r_word is not None else None
        d_asm = disasm_one(d_word, decomp_base + offset) if d_word is not None else None

        # Detect reg-swap mismatch
        is_reg_swap = False
        pure_reg_swap = False
        if r_word is not None and d_word is not None and r_word != d_word and r_asm and d_asm:
            r_mnem = r_asm.split()[0] if r_asm else ""
            d_mnem = d_asm.split()[0] if d_asm else ""
            if r_mnem and r_mnem == d_mnem:
                is_reg_swap = True
                pure_reg_swap = _pure_reg_swap(r_word, d_word, r_mnem, d_mnem)
                r_rd, r_ra, r_rb = _rd(r_word), _ra(r_word), _rb(r_word)
                d_rd, d_ra, d_rb = _rd(d_word), _ra(d_word), _rb(d_word)
                for pos, r_r, d_r in [("rd", r_rd, d_rd), ("ra", r_ra, d_ra), ("rb", r_rb, d_rb)]:
                    if r_r != d_r:
                        reg_map[f"{r_mnem}*{pos}"].add((r_r, d_r))

        byte_match = r_word == d_word
        diffs.append({
            "offset": offset,
            "retail_hex": f"0x{r_word:08X}" if r_word is not None else None,
            "decomp_hex": f"0x{d_word:08X}" if d_word is not None else None,
            "retail_asm": r_asm,
            "decomp_asm": d_asm,
            "match": byte_match,
            "has_decomp_reloc": offset in decomp_relocs,
            "reg_swap": is_reg_swap,
            "pure_reg_swap": pure_reg_swap,
            "structural": (r_word != d_word) and not is_reg_swap,
        })

        if r_word != d_word:
            if is_reg_swap:
                reg_swap_count += 1
                if pure_reg_swap:
                    pure_reg_swap_count += 1
            else:
                structural_count += 1

    # Build register mapping summary (retail → decomp per operand position)
    reg_mapping = {}
    for key in sorted(reg_map):
        pairs = reg_map[key]
        by_r: dict[int, list[int]] = {}
        for r_r, d_r in pairs:
            by_r.setdefault(r_r, []).append(d_r)
        entries = []
        for r_r in sorted(by_r):
            d_regs = sorted(set(by_r[r_r]))
            if len(d_regs) == 1:
                entries.append({"retail_reg": r_r, "decomp_regs": d_regs})
            else:
                entries.append({"retail_reg": r_r, "decomp_regs": d_regs})
        reg_mapping[key] = entries

    output = {
        "symbol": retail.name,
        "retail_path": str(retail.path),
        "decomp_path": str(decomp.path),
        "retail_size": retail.size,
        "decomp_size": decomp.size,
        "total_instructions": total // 4,
        "mismatch_count": sum(1 for d in diffs if not d["match"]),
        "reg_swap_count": reg_swap_count,
        "pure_reg_swap_count": pure_reg_swap_count,
        "structural_count": structural_count,
        "reg_mapping": reg_mapping,
        "instructions": diffs,
        "size_check": (
            {
                "budget": size_check.budget,
                "retail_text": size_check.retail_text,
                "decomp_text": size_check.decomp_text,
                "ok": size_check.ok,
                "over_by": size_check.over_by,
                "split_path": size_check.split_path,
                "notes": size_check.notes,
            }
            if size_check is not None
            else None
        ),
        "retail_relocations": [
            {"offset": r.offset, "type": r.relocation_type, "symbol": r.symbol, "addend": r.addend}
            for r in retail.relocations
        ],
        "decomp_relocations": [
            {"offset": r.offset, "type": r.relocation_type, "symbol": r.symbol, "addend": r.addend}
            for r in decomp.relocations
        ],
        "reloc_drift": [d.to_dict() for d in reloc_drifts],
        "reloc_suggestions": reloc_suggestions,
    }
    print(json.dumps(output, indent=2))
    return 0 if all(d["match"] for d in diffs) else 5


if __name__ == "__main__":
    raise SystemExit(run())
