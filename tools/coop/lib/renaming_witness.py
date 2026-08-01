"""Register-renaming witness: pre-SMT equivalence certificates.

Implements ``docs/ppc_equiv_work/31-reg-swap-witness.md`` (the corrected
design after two adversarial reviews; supersedes the unsound "reg-swap-only
fast path" plan).

Certifies equivalence for position-aligned, same-mnemonic function pairs
whose instruction streams differ only in register colors (MWCC
register-allocation soft-caps) WITHOUT Z3 CFG exploration.

Soundness formulation (mandatory): *symbolic-variable substitution on the
initial state*, not operand rewriting.  Each retail register ``r_i`` and the
decomp register ``rho(i)`` share one symbolic variable.  Both sides execute
through the audited ``SymbolicOps``/``execute_cfg`` (semantics.py) — the same
semantics that already handle rA=0, shift counts, CR fields, SPR indices,
relocations, and calls.  The terminal states are then compared with
structural ``z3.eq`` (AST identity).  Identical trees imply identical values
under every assignment, so a structurally-equal terminal pair can never
diverge; any structural divergence makes the witness fail and the caller falls
through to the normal SMT probe.  Incompleteness degrades to SMT — never a
false certificate.

Gates (in order):

1. Size equality — same instruction count.
2. Per-slot reloc equality — offset/type/symbol/addend must all match
   (relocs are addresses, not colors).
3. Non-register field equality — every bit that is not a renameable 5-bit
   GPR/FPR register field must be bit-equal between the raw words.  This
   covers opcode/XO bits, Rc/OE, immediates, branch displacements, BO/BI/BH,
   CR bits/fields, SPR indices, FXM masks, and LK/AA in one comparison.
4. rho bijection — single-valued in both directions, consistent across all
   mnemonics/positions.  (hexdiff's ``reg_map`` is keyed by ``(mnem, pos)``
   and allows many-to-one; this rho is built fresh from the role table.)
5. ABI-boundary fixedness — rho must fix r0 (zero-register encoding), r1, r2,
   r13, LR/CTR (inherently fixed: SPR indices are non-register fields), all
   argument/return registers (r3–r10, f1–f8), and every volatile register
   live across a call.  Nonvolatile permutations across calls (e.g.
   r20<->r25, both preserved by EABI) are SOUND and are NOT pre-rejected —
   that is the Chaitin-cycle class this feature exists for.
6. Reject-list — ``ps_*``, ``psq_*``, ``mtfsf``/``mffs``/``mcrfs``/
   ``mtfsb*``/``mtfsfi``, ``mtspr``/``mfspr`` to GQRs (912–919) or any
   non-{LR,CTR,XER} SPR, ``dcbz``/``icbi``/``tlb*``, and privileged/system
   opcodes fall straight to SMT and are never certified via renaming.
"""

from __future__ import annotations

from dataclasses import dataclass, field
from typing import Any

from tools.ppc_equivalence.deadline import Deadline, ProofDeadlineExceeded
from tools.ppc_equivalence.ir import Instruction, Opcode
from tools.ppc_equivalence.model import MachineState, XerState
from tools.ppc_equivalence.semantics import (
    DEFAULT_MAX_LOOP_ITERATIONS,
    SymbolicOps,
    execute_cfg,
)
from tools.ppc_equivalence.spr import AUX_SPR_OBSERVABLES

# ── operand-role classification ────────────────────────────────────────────
#
# Each 5-bit field of an instruction word is one of:
#   gpr           — general-purpose register (enters rho_gpr)
#   fpr           — floating-point register (enters rho_fpr)
#   cr_bit        — CR bit index (BT/BA/BB/BI) — must be bit-equal
#   cr_field_bf   — CR field index (BF/BFA) — must be bit-equal
#   spr_index     — SPR index — must be bit-equal
#   fxm_mask      — mtcrf field mask — must be bit-equal
#   immediate     — everything else — must be bit-equal
#
# Only ``gpr``/``fpr`` fields enter rho; every other role is required to be
# bit-equal between the two sides (gate 3).  The classification is implemented
# as per-opcode *register field bit ranges*: the bits covered by gpr/fpr
# fields are extracted for rho, and the complement (which is every
# cr_bit/cr_field_bf/spr_index/fxm_mask/immediate bit plus the opcode, XO,
# Rc, OE, LK, and AA bits) is compared for bit-equality.

GPR = "gpr"
FPR = "fpr"
CR_BIT = "cr_bit"
CR_FIELD_BF = "cr_field_bf"
SPR_INDEX = "spr_index"
FXM_MASK = "fxm_mask"
IMMEDIATE = "immediate"

# Bit ranges of the register fields, keyed by ``(start, kind)`` for the
# 5-bit fields only.  Everything outside these ranges is non-register.
_RD = (21, GPR)
_RA = (16, GPR)
_RB = (11, GPR)
_FD = (21, FPR)
_FA = (16, FPR)
_FB = (11, FPR)
_FC = (6, FPR)

# Opcodes whose entire register-field set is empty (all bits non-register).
_NO_REG_FIELDS = frozenset(
    {
        Opcode.B, Opcode.BC, Opcode.BCLR, Opcode.BCCTR,
        Opcode.CRAND, Opcode.CRANDC, Opcode.CREQV, Opcode.CRNAND,
        Opcode.CRNOR, Opcode.CROR, Opcode.CRORC, Opcode.CRXOR,
        Opcode.SYNC, Opcode.ISYNC, Opcode.SC, Opcode.RFI,
    }
)


def _register_fields(opcode: Opcode) -> tuple[tuple[int, str], ...]:
    """Return the ``(start_bit, kind)`` 5-bit register fields for ``opcode``.

    Unknown opcodes return ``()`` — the caller then treats every bit as
    non-register, which only makes the witness *stricter* (byte-equality of
    all non-register bits plus a trivially-empty rho), never looser.
    """
    if opcode in _NO_REG_FIELDS:
        return ()
    if opcode in (
        Opcode.TWI, Opcode.MULLI, Opcode.SUBFIC, Opcode.ADDIC,
        Opcode.ADDIC_DOT, Opcode.ADDI, Opcode.ADDIS,
        Opcode.ORI, Opcode.ORIS, Opcode.XORI, Opcode.XORIS,
        Opcode.ANDI_DOT, Opcode.ANDIS_DOT,
        Opcode.RLWIMI, Opcode.RLWINM,
        Opcode.LWZ, Opcode.LWZU, Opcode.LBZ, Opcode.LBZU,
        Opcode.STW, Opcode.STWU, Opcode.STB, Opcode.STBU,
        Opcode.LHZ, Opcode.LHZU, Opcode.LHA, Opcode.LHAU,
        Opcode.STH, Opcode.STHU, Opcode.LMW, Opcode.STMW,
        Opcode.MFCR,
        Opcode.CNTLZW, Opcode.EXTSH, Opcode.EXTSB,
        Opcode.SRAWI,
        Opcode.NEG, Opcode.ADDME, Opcode.ADDZE, Opcode.SUBFME,
        Opcode.SUBFZE,
    ):
        return (_RD, _RA)
    if opcode in (
        Opcode.MFSPR, Opcode.MTSPR, Opcode.MTCRF,
        Opcode.MFMSR, Opcode.MTMSR, Opcode.MFSR, Opcode.MTSR,
        Opcode.MFTB,
    ):
        # Only the destination/source GPR (bits 21-25) is a register field;
        # bits 11-20 carry the SPR index / SR index / TBR / FXM / reserved
        # fields, all non-register and required to be bit-equal.
        return (_RD,)
    if opcode in (Opcode.CMPWI, Opcode.CMPLWI):
        # BF (23-25) is a CR field immediate; only RA (16-20) is a register.
        return (_RA,)
    if opcode == Opcode.RLWNM:
        return (_RD, _RA, _RB)
    if opcode in (
        Opcode.CMPW, Opcode.CMPLW, Opcode.DCBF, Opcode.DCBI,
        Opcode.DCBST, Opcode.DCBT, Opcode.DCBZ, Opcode.DCBZ_L,
        Opcode.ICBI,
    ):
        # RA (16-20) and RB (11-15); BF (23-25) is a CR field immediate.
        return (_RA, _RB)
    if opcode in (
        Opcode.AND, Opcode.ANDC, Opcode.EQV, Opcode.NAND, Opcode.NOR,
        Opcode.OR, Opcode.ORC, Opcode.XOR, Opcode.SLW, Opcode.SRW,
        Opcode.SRAW, Opcode.SUBFC, Opcode.ADDC, Opcode.MULHWU,
        Opcode.SUBF, Opcode.MULHW, Opcode.SUBFE, Opcode.ADDE,
        Opcode.MULLW, Opcode.ADD, Opcode.DIVWU, Opcode.DIVW,
        Opcode.LWZX, Opcode.LWZUX, Opcode.LBZX, Opcode.LBZUX,
        Opcode.STWX, Opcode.STWUX, Opcode.STBX, Opcode.STBUX,
        Opcode.LHZX, Opcode.LHZUX, Opcode.LHAX, Opcode.LHAUX,
        Opcode.STHX, Opcode.STHUX, Opcode.LWBRX, Opcode.STWBRX,
        Opcode.LHBRX, Opcode.STHBRX,
    ):
        return (_RD, _RA, _RB)
    if opcode in (
        Opcode.LFS, Opcode.LFSU, Opcode.LFD, Opcode.LFDU,
        Opcode.STFS, Opcode.STFSU, Opcode.STFD, Opcode.STFDU,
    ):
        return (_FD, _RA)
    if opcode in (
        Opcode.LFSX, Opcode.LFSUX, Opcode.LFDX, Opcode.LFDUX,
        Opcode.STFSX, Opcode.STFSUX, Opcode.STFDX, Opcode.STFDUX,
        Opcode.STFIWX,
    ):
        return (_FD, _RA, _RB)
    if opcode in (
        Opcode.FDIVS, Opcode.FSUBS, Opcode.FADDS, Opcode.FRES,
        Opcode.FMULS, Opcode.FMSUBS, Opcode.FMADDS, Opcode.FNMSUBS,
        Opcode.FNMADDS, Opcode.FDIV, Opcode.FSUB, Opcode.FADD,
        Opcode.FSEL, Opcode.FMUL, Opcode.FRSQRTE, Opcode.FMSUB,
        Opcode.FMADD, Opcode.FNMSUB, Opcode.FNMADD, Opcode.FRSP,
        Opcode.FCTIW, Opcode.FCTIWZ, Opcode.FNEG, Opcode.FMR,
        Opcode.FNABS, Opcode.FABS,
    ):
        return (_FD, _FA, _FB, _FC)
    if opcode in (Opcode.FCMPU, Opcode.FCMPO):
        return (_FA, _FB)
    return ()


def _gpr_fpr_masks(opcode: Opcode) -> tuple[int, int]:
    """Return ``(gpr_mask, fpr_mask)`` bit masks for ``opcode``'s 5-bit fields."""
    gpr_mask = 0
    fpr_mask = 0
    for start, kind in _register_fields(opcode):
        mask = 0x1F << start
        if kind == GPR:
            gpr_mask |= mask
        else:
            fpr_mask |= mask
    return gpr_mask, fpr_mask


def operand_roles(opcode: Opcode, word: int) -> list[tuple[str, int]]:
    """Classify every 5-bit field of ``word`` into ``(role, value)``.

    Fields within a register range are ``gpr``/``fpr`` (values extracted);
    every other 5-bit field is classified as ``cr_bit``, ``cr_field_bf``,
    ``spr_index``, ``fxm_mask``, or ``immediate`` using the opcode layout.
    Used for documentation / audit output; gates use the raw-bit masks.
    """
    roles: list[tuple[str, int]] = []
    gpr_mask, fpr_mask = _gpr_fpr_masks(opcode)
    for start in range(0, 26, 5):
        value = (word >> start) & 0x1F
        bit = 1 << start
        if bit & gpr_mask:
            roles.append((GPR, value))
        elif bit & fpr_mask:
            roles.append((FPR, value))
        elif opcode in (
            Opcode.CMPWI, Opcode.CMPLWI, Opcode.CMPW, Opcode.CMPLW,
            Opcode.FCMPU, Opcode.FCMPO,
        ) and start == 21:
            roles.append((CR_FIELD_BF, value))
        elif opcode in (Opcode.MFSPR, Opcode.MTSPR) and start == 11:
            roles.append((SPR_INDEX, value))
        elif opcode == Opcode.MTCRF and start == 12:
            roles.append((FXM_MASK, value))
        elif opcode in (
            Opcode.CRAND, Opcode.CRANDC, Opcode.CREQV, Opcode.CRNAND,
            Opcode.CRNOR, Opcode.CROR, Opcode.CRORC, Opcode.CRXOR,
            Opcode.BC, Opcode.BCLR, Opcode.BCCTR,
        ) and start in (11, 16, 21):
            roles.append((CR_BIT, value))
        else:
            roles.append((IMMEDIATE, value))
    return roles


# ── reject-list (gate 6) ───────────────────────────────────────────────────

# Opcodes that must never be certified through the renaming witness; the pair
# falls straight to SMT.  Paired-single / PSQ (fp-capability stubs, custom
# semantics), FPSCR transfers (sticky semantics), cache-block/system ops, and
# privileged/supervisor ops.
REJECT_OPCODES = frozenset(
    {
        # Paired-single arithmetic + PSQ memory forms.
        Opcode.PS_ABS, Opcode.PS_ADD, Opcode.PS_CMPO0, Opcode.PS_CMPO1,
        Opcode.PS_CMPU0, Opcode.PS_CMPU1, Opcode.PS_DIV, Opcode.PS_MADD,
        Opcode.PS_MADDS0, Opcode.PS_MADDS1, Opcode.PS_MERGE00,
        Opcode.PS_MERGE01, Opcode.PS_MERGE10, Opcode.PS_MERGE11,
        Opcode.PS_MR, Opcode.PS_MSUB, Opcode.PS_MUL, Opcode.PS_MULS0,
        Opcode.PS_MULS1, Opcode.PS_NABS, Opcode.PS_NEG, Opcode.PS_NMADD,
        Opcode.PS_NMSUB, Opcode.PS_RES, Opcode.PS_RSQRTE, Opcode.PS_SEL,
        Opcode.PS_SUB, Opcode.PS_SUM0, Opcode.PS_SUM1,
        Opcode.PSQ_L, Opcode.PSQ_LU, Opcode.PSQ_LUX, Opcode.PSQ_LX,
        Opcode.PSQ_ST, Opcode.PSQ_STU, Opcode.PSQ_STUX, Opcode.PSQ_STX,
        # FPSCR transfer / control (sticky bits).
        Opcode.MTFSF, Opcode.MFFS, Opcode.MCRFS,
        Opcode.MTFSB0, Opcode.MTFSB1, Opcode.MTFSFI,
        # Cache-block / system operations.
        Opcode.DCBZ, Opcode.DCBZ_L, Opcode.DCBI, Opcode.ICBI,
        # Privileged / supervisor.
        Opcode.MFMSR, Opcode.MTMSR, Opcode.MFSR, Opcode.MTSR,
        Opcode.SC, Opcode.RFI,
    }
)

# SPR indices the witness may certify across: XER, LR, CTR.  Any other SPR
# (GQRs 912-919, aux SPRs, TB, SRR0/SRR1, ...) fails closed to SMT.
_WITNESS_ALLOWED_SPRS = frozenset({1, 8, 9})

# r0..r2, r13, and the return registers r3/r4 are unconditionally fixed by
# rho (zero-register encoding r0, stack pointer r1, small-data bases r2/r13,
# integer return r3/r4).  LR/CTR are inherently fixed: their SPR indices are
# non-register fields required to be bit-equal.  Registers that are actually
# read before being written (live-in at entry) in the EABI argument range
# (r3..r10, f1..f8) are the function's input signature and are also fixed.
# Volatile r11/r12 / f0/f9..f13 join the fixed set only when live across a
# call (liveness gate).
_UNCONDITIONALLY_FIXED_GPRS = frozenset({0, 1, 2, 3, 4, 13})
# f1 is the FP return register (and first FP argument).
_UNCONDITIONALLY_FIXED_FPRS = frozenset({1})
# EABI outgoing-argument ranges: registers that may carry live-in inputs.
_EABI_ARG_GPRS = frozenset(range(3, 11))
_EABI_ARG_FPRS = frozenset(range(1, 9))
_VOLATILE_GPRS = frozenset(range(3, 13))
_VOLATILE_FPRS = frozenset(range(0, 14))

# ── per-opcode use/def positions (decoded operand indices) for liveness ────

# (uses, defs) over decoded operand positions with their register-file kind
# (GPR/FPR), used by the gate-5 liveness check ("every volatile register live
# across a call must be fixed by rho").  CR/XER/LR/CTR/SPR components are
# never renamed so they are not tracked here.  Unknown opcodes default to
# no uses/defs, which only under-approximates *uses*; the structural terminal
# comparison remains the soundness backstop.


def _use_def(opcode: Opcode) -> tuple[tuple[tuple[int, str], ...], tuple[tuple[int, str], ...]]:
    g = GPR
    if opcode in (
        Opcode.TWI, Opcode.MTCRF, Opcode.MTSPR, Opcode.MTMSR, Opcode.MTSR,
    ):
        return ((0, g),), ()
    if opcode in (
        Opcode.STW, Opcode.STWU, Opcode.STB, Opcode.STBU, Opcode.STH,
        Opcode.STHU, Opcode.STMW,
    ):
        defs = ((1, g),) if opcode in (Opcode.STWU, Opcode.STBU, Opcode.STHU) else ()
        return ((0, g), (1, g)), defs
    if opcode in (Opcode.STFS, Opcode.STFSU, Opcode.STFD, Opcode.STFDU):
        defs = ((1, g),) if opcode in (Opcode.STFSU, Opcode.STFDU) else ()
        return ((0, FPR), (1, g)), defs
    if opcode in (
        Opcode.LWZ, Opcode.LBZ, Opcode.LHZ, Opcode.LHA, Opcode.LMW,
        Opcode.MFCR, Opcode.MFSPR, Opcode.MFMSR, Opcode.MFSR, Opcode.MFTB,
    ):
        return ((1, g),), ((0, g),)
    if opcode in (Opcode.LFS, Opcode.LFD):
        return ((1, g),), ((0, FPR),)
    if opcode in (Opcode.LWZU, Opcode.LBZU, Opcode.LHZU, Opcode.LHAU):
        return ((1, g),), ((0, g), (1, g))
    if opcode in (Opcode.LFSU, Opcode.LFDU):
        return ((1, g),), ((0, FPR), (1, g))
    if opcode in (
        Opcode.STWX, Opcode.STBX, Opcode.STHX, Opcode.STWBRX, Opcode.STHBRX,
        Opcode.DCBF, Opcode.DCBI, Opcode.DCBST, Opcode.DCBT,
        Opcode.DCBZ, Opcode.DCBZ_L, Opcode.ICBI,
    ):
        return ((0, g), (1, g), (2, g)), ()
    if opcode in (Opcode.STWUX, Opcode.STBUX, Opcode.STHUX):
        return ((0, g), (1, g), (2, g)), ((1, g),)
    if opcode in (Opcode.STFIWX, Opcode.STFSX, Opcode.STFDX):
        return ((0, FPR), (1, g), (2, g)), ()
    if opcode in (Opcode.STFSUX, Opcode.STFDUX):
        return ((0, FPR), (1, g), (2, g)), ((1, g),)
    if opcode in (
        Opcode.LWZX, Opcode.LBZX, Opcode.LHZX, Opcode.LHAX,
        Opcode.LWBRX, Opcode.LHBRX,
    ):
        return ((1, g), (2, g)), ((0, g),)
    if opcode in (Opcode.LFSX, Opcode.LFDX):
        return ((1, g), (2, g)), ((0, FPR),)
    if opcode in (Opcode.LWZUX, Opcode.LBZUX, Opcode.LHZUX, Opcode.LHAUX):
        return ((1, g), (2, g)), ((0, g), (1, g))
    if opcode in (Opcode.LFSUX, Opcode.LFDUX):
        return ((1, g), (2, g)), ((0, FPR), (1, g))
    if opcode in (
        Opcode.ADDI, Opcode.ADDIS, Opcode.ADDIC, Opcode.ADDIC_DOT,
        Opcode.SUBFIC, Opcode.MULLI,
    ):
        return ((1, g),), ((0, g),)
    if opcode in (
        Opcode.ORI, Opcode.ORIS, Opcode.XORI, Opcode.XORIS,
        Opcode.ANDI_DOT, Opcode.ANDIS_DOT, Opcode.RLWIMI, Opcode.RLWINM,
        Opcode.SRAWI, Opcode.CNTLZW, Opcode.EXTSH, Opcode.EXTSB,
    ):
        return ((1, g),), ((0, g),)
    if opcode in (Opcode.CMPWI, Opcode.CMPLWI):
        return ((1, g),), ()
    if opcode in (Opcode.CMPW, Opcode.CMPLW, Opcode.FCMPU, Opcode.FCMPO):
        kind = FPR if opcode in (Opcode.FCMPU, Opcode.FCMPO) else g
        return ((1, kind), (2, kind)), ()
    if opcode == Opcode.RLWNM:
        return ((1, g), (2, g)), ((0, g),)
    if opcode in (
        Opcode.AND, Opcode.ANDC, Opcode.EQV, Opcode.NAND, Opcode.NOR,
        Opcode.OR, Opcode.ORC, Opcode.XOR, Opcode.SLW, Opcode.SRW,
        Opcode.SRAW, Opcode.SUBFC, Opcode.ADDC, Opcode.MULHWU,
        Opcode.SUBF, Opcode.MULHW, Opcode.SUBFE, Opcode.ADDE,
        Opcode.MULLW, Opcode.ADD, Opcode.DIVWU, Opcode.DIVW,
    ):
        return ((1, g), (2, g)), ((0, g),)
    if opcode in (
        Opcode.NEG, Opcode.ADDME, Opcode.ADDZE, Opcode.SUBFME,
        Opcode.SUBFZE,
    ):
        return ((1, g),), ((0, g),)
    if opcode in (
        Opcode.FDIVS, Opcode.FSUBS, Opcode.FADDS, Opcode.FRES,
        Opcode.FMULS, Opcode.FMSUBS, Opcode.FMADDS, Opcode.FNMSUBS,
        Opcode.FNMADDS, Opcode.FDIV, Opcode.FSUB, Opcode.FADD,
        Opcode.FSEL, Opcode.FMUL, Opcode.FRSQRTE, Opcode.FMSUB,
        Opcode.FMADD, Opcode.FNMSUB, Opcode.FNMADD,
    ):
        return ((1, FPR), (2, FPR), (3, FPR)), ((0, FPR),)
    if opcode in (Opcode.FRSP, Opcode.FCTIW, Opcode.FCTIWZ,
                  Opcode.FNEG, Opcode.FMR, Opcode.FNABS, Opcode.FABS):
        # Decoder emits (fd, fa=0, fb, fc=0); the real source is FB (pos 2).
        return ((2, FPR),), ((0, FPR),)
    return (), ()


def _is_call(insn: Instruction) -> bool:
    return bool(insn.link) and insn.opcode in (
        Opcode.B, Opcode.BC, Opcode.BCLR, Opcode.BCCTR,
    )


def _liveness_sets(
    instructions: list[Instruction],
) -> tuple[frozenset[int], frozenset[int]]:
    """Return ``(live_in, live_across_calls)`` in one combined numbering
    space (GPR r -> r, FPR f -> 32 + f).

    Straight-line backward liveness over the position grid; branches are
    ignored so both sets are safe over-approximations for the gate-5
    defense-in-depth check.
    """
    # One numbering space: GPR r -> r, FPR f -> 32 + f.
    live: set[int] = set()
    live_across: set[int] = set()
    for i in range(len(instructions) - 1, -1, -1):
        insn = instructions[i]
        uses, defs = _use_def(insn.opcode)
        for pos, _kind in defs:
            if pos < len(insn.operands):
                live.discard(insn.operands[pos])
        if _is_call(insn):
            live_across |= live
            # Registers read at the call (arguments) are also conservatively
            # considered live across it.
            for pos, kind in uses:
                if pos < len(insn.operands):
                    value = insn.operands[pos]
                    live_across.add(value if kind == GPR else 32 + value)
        for pos, kind in uses:
            if pos < len(insn.operands):
                value = insn.operands[pos]
                live.add(value if kind == GPR else 32 + value)
    volatiles = set(_VOLATILE_GPRS) | {32 + r for r in _VOLATILE_FPRS}
    return frozenset(live), frozenset(live_across & volatiles)


# ── gates ──────────────────────────────────────────────────────────────────


@dataclass(frozen=True)
class Rho:
    """Partial bijection over GPR and FPR register files.

    ``rho_gpr[i] == j`` means decomp's register j plays retail's register i.
    """

    gpr: dict[int, int] = field(default_factory=dict)
    fpr: dict[int, int] = field(default_factory=dict)

    def gpr_perm(self) -> list[int]:
        """Full permutation over 0..31 extending ``gpr`` by a canonical
        sorted match of the unused registers (identity when the used sets
        coincide)."""
        return _extend_permutation(self.gpr)

    def fpr_perm(self) -> list[int]:
        return _extend_permutation(self.fpr)

    def to_dict(self) -> dict[str, dict[str, int]]:
        return {
            "gpr": {str(k): v for k, v in sorted(self.gpr.items())},
            "fpr": {str(k): v for k, v in sorted(self.fpr.items())},
        }


def _extend_permutation(partial: dict[int, int]) -> list[int]:
    """Extend a partial bijection to a full permutation of 0..31.

    Unused domain/range registers are matched in ascending order, so the
    extension is canonical and a pure identity for byte-identical pairs.
    """
    perm = {k: v for k, v in partial.items()}
    used_domain = set(perm)
    used_range = set(perm.values())
    free_domain = sorted(set(range(32)) - used_domain)
    free_range = sorted(set(range(32)) - used_range)
    for key, value in zip(free_domain, free_range):
        perm[key] = value
    assert sorted(perm.values()) == list(range(32)), "rho extension is not a permutation"
    return [perm[k] for k in range(32)]


@dataclass(frozen=True)
class WitnessFailure:
    gate: str
    reason: str


@dataclass
class WitnessOutcome:
    """Result of the gate pipeline + structural witness execution."""

    certified: bool
    rho: Rho | None = None
    failure: WitnessFailure | None = None
    structural_eq: bool = False
    terminal_pairs_checked: int = 0
    details: dict[str, Any] = field(default_factory=dict)


def check_gates(
    original: list[Instruction],
    candidate: list[Instruction],
) -> WitnessOutcome:
    """Run gates 1-6; return the rho on success."""
    # Gate 1: size equality.
    if len(original) != len(candidate):
        return WitnessOutcome(
            False, failure=WitnessFailure(
                "size", f"{len(original)} vs {len(candidate)} instructions",
            ),
        )

    rho_gpr: dict[int, int] = {}
    rho_fpr: dict[int, int] = {}

    for index, (r_insn, d_insn) in enumerate(zip(original, candidate)):
        # Gate 6 (reject-list) and SPR policy are checked per slot.
        if r_insn.opcode in REJECT_OPCODES or d_insn.opcode in REJECT_OPCODES:
            op = r_insn.opcode if r_insn.opcode in REJECT_OPCODES else d_insn.opcode
            return WitnessOutcome(
                False, failure=WitnessFailure(
                    "reject-list", f"slot {index}: {op.value}",
                ),
            )
        if r_insn.opcode != d_insn.opcode:
            return WitnessOutcome(
                False, failure=WitnessFailure(
                    "mnemonic", f"slot {index}: {r_insn.opcode.value} vs {d_insn.opcode.value}",
                ),
            )
        if r_insn.opcode in (Opcode.MFSPR, Opcode.MTSPR):
            for insn in (r_insn, d_insn):
                if insn.operands[1] not in _WITNESS_ALLOWED_SPRS:
                    return WitnessOutcome(
                        False, failure=WitnessFailure(
                            "reject-list",
                            f"slot {index}: {insn.opcode.value} to SPR {insn.operands[1]}",
                        ),
                    )

        # Gate 2: per-slot relocation equality (offset/type/symbol/addend).
        r_reloc = r_insn.relocation
        d_reloc = d_insn.relocation
        if (r_reloc is None) != (d_reloc is None):
            return WitnessOutcome(
                False, failure=WitnessFailure(
                    "reloc", f"slot {index}: reloc presence differs",
                ),
            )
        if r_reloc is not None:
            if (
                r_reloc.offset != d_reloc.offset
                or r_reloc.relocation_type != d_reloc.relocation_type
                or r_reloc.canonical_symbol != d_reloc.canonical_symbol
                or r_reloc.addend != d_reloc.addend
            ):
                return WitnessOutcome(
                    False, failure=WitnessFailure(
                        "reloc",
                        f"slot {index}: {r_reloc.canonical_symbol}@{r_reloc.addend} "
                        f"vs {d_reloc.canonical_symbol}@{d_reloc.addend}",
                    ),
                )

        # Gate 3: non-register field equality (raw-bit comparison).  Slots
        # with a matched relocation are exempt: the relocated bits are
        # placeholders that resolve to the same canonical symbol on both
        # sides (gate 2 binds identity), and execute_cfg substitutes the
        # shared symbolic value.
        gpr_mask, fpr_mask = _gpr_fpr_masks(r_insn.opcode)
        register_mask = gpr_mask | fpr_mask
        if r_reloc is None:
            non_register_diff = (r_insn.raw ^ d_insn.raw) & ~register_mask
            if non_register_diff:
                return WitnessOutcome(
                    False, failure=WitnessFailure(
                        "fields",
                        f"slot {index}: non-register bits differ "
                        f"(0x{non_register_diff:08x})",
                    ),
                )

        # Gate 4: rho accumulation — single-valued and injective, consistent
        # across all mnemonics/positions.
        for start, kind in _register_fields(r_insn.opcode):
            rv = (r_insn.raw >> start) & 0x1F
            dv = (d_insn.raw >> start) & 0x1F
            table = rho_gpr if kind == GPR else rho_fpr
            if rv in table:
                if table[rv] != dv:
                    return WitnessOutcome(
                        False, failure=WitnessFailure(
                            "rho",
                            f"slot {index}: {kind} r{rv} maps to both "
                            f"r{table[rv]} and r{dv}",
                        ),
                    )
            elif dv in table.values():
                return WitnessOutcome(
                    False, failure=WitnessFailure(
                        "rho",
                        f"slot {index}: {kind} r{rv} and r{next(k for k, v in table.items() if v == dv)} "
                        f"both map to r{dv}",
                    ),
                )
            else:
                table[rv] = dv

    rho = Rho(gpr=rho_gpr, fpr=rho_fpr)

    # Gate 5: ABI-boundary fixedness.
    failure = _check_abi_fixedness(original, candidate, rho)
    if failure is not None:
        return WitnessOutcome(False, rho=rho, failure=failure)

    return WitnessOutcome(True, rho=rho)


def _check_abi_fixedness(
    original: list[Instruction],
    candidate: list[Instruction],
    rho: Rho,
) -> WitnessFailure | None:
    # LR/CTR are inherently fixed (SPR indices are non-register bit-equal
    # fields); only GPR/FPR entries are checked here.
    fixed_gpr = set(_UNCONDITIONALLY_FIXED_GPRS)
    fixed_fpr = set(_UNCONDITIONALLY_FIXED_FPRS)
    # Registers read before being written (live-in at entry) in the EABI
    # argument ranges are the function's input signature and must be fixed.
    # The two sides are position-aligned with identical control flow, so a
    # union of both sides' live-in sets is the conservative choice.
    for instructions in (original, candidate):
        live_in, live_across = _liveness_sets(instructions)
        fixed_gpr.update(live_in & _EABI_ARG_GPRS)
        fixed_fpr.update({n - 32 for n in live_in if n >= 32} & _EABI_ARG_FPRS)
        fixed_gpr.update(n for n in live_across if n < 32)
        fixed_fpr.update(n - 32 for n in live_across if n >= 32)
    for register in sorted(fixed_gpr):
        if rho.gpr.get(register, register) != register:
            return WitnessFailure(
                "abi-boundary",
                f"rho maps gpr r{register} -> r{rho.gpr.get(register)}; "
                f"ABI registers must be fixed",
            )
    for register in sorted(fixed_fpr):
        if rho.fpr.get(register, register) != register:
            return WitnessFailure(
                "abi-boundary",
                f"rho maps fpr f{register} -> f{rho.fpr.get(register)}; "
                f"ABI registers must be fixed",
            )
    return None


# ── witness execution ──────────────────────────────────────────────────────


def _symbolic_initial_pair(
    ops: SymbolicOps,
    gpr_perm: list[int],
    fpr_perm: list[int],
) -> tuple[MachineState, MachineState]:
    """Build (retail, decomp) initial states sharing one variable per
    register-role: ``retail.gpr[i] == decomp.gpr[gpr_perm[i]] == X_i``.

    All non-register state (CR, XER, LR, CTR, GQR, SR, MSR, time base, SRR*,
    aux SPRs, FPSCR, memory, validity) is shared verbatim between the sides.
    The permutations are full permutations of 0..31, so each decomp register
    receives exactly one shared variable.
    """
    z3 = ops.z3
    gpr_vars = [z3.BitVec(f"witness.r{i}", 32) for i in range(32)]
    fpr_vars = [z3.BitVec(f"witness.f{i}", 64) for i in range(32)]
    ps1_vars = [z3.BitVec(f"witness.f{i}.ps1", 64) for i in range(32)]
    shared = dict(
        gqr=[z3.BitVec(f"witness.gqr{i}", 32) for i in range(8)],
        cr=z3.BitVec("witness.cr", 32),
        xer=XerState(
            z3.Bool("witness.xer.ca"),
            z3.Bool("witness.xer.ov"),
            z3.Bool("witness.xer.so"),
        ),
        fpscr=z3.BitVec("witness.fpscr", 32),
        lr=z3.BitVec("witness.lr", 32),
        ctr=z3.BitVec("witness.ctr", 32),
        msr=z3.BitVec("witness.msr", 32),
        sr=[z3.BitVec(f"witness.sr{i}", 32) for i in range(16)],
        time_base=z3.BitVec("witness.time_base", 64),
        srr0=z3.BitVec("witness.srr0", 32),
        srr1=z3.BitVec("witness.srr1", 32),
        spr=[z3.BitVec(f"witness.spr.{name}", 32) for name in AUX_SPR_OBSERVABLES],
        memory=z3.Array("witness.memory", z3.BitVecSort(32), z3.BitVecSort(8)),
    )

    # Decomp register j plays retail's perm^{-1}(j).
    inverse_gpr = [0] * 32
    for i, j in enumerate(gpr_perm):
        inverse_gpr[j] = i
    inverse_fpr = [0] * 32
    for i, j in enumerate(fpr_perm):
        inverse_fpr[j] = i

    def _state(
        gpr_inverse: list[int],
        fpr_inverse: list[int],
    ) -> MachineState:
        state = MachineState(
            tuple(gpr_vars[i] for i in gpr_inverse),
            tuple(fpr_vars[i] for i in fpr_inverse),
            tuple(ps1_vars[i] for i in fpr_inverse),
            tuple(shared["gqr"]),
            shared["cr"],
            shared["xer"],
            shared["fpscr"],
            shared["lr"],
            shared["ctr"],
            shared["msr"],
            tuple(shared["sr"]),
            shared["time_base"],
            shared["srr0"],
            shared["srr1"],
            tuple(shared["spr"]),
            shared["memory"],
            z3.BoolVal(True),
            z3.BitVecVal(0, 8),
        )
        from dataclasses import replace

        return replace(
            state,
            stack_low=state.gpr[1],
            stack_layout_valid=z3.BoolVal(True),
            stack_private=z3.BoolVal(True),
        )

    retail_state = _state(list(range(32)), list(range(32)))
    decomp_state = _state(inverse_gpr, inverse_fpr)
    return retail_state, decomp_state


def _structurally_equal(left: Any, right: Any, z3: Any) -> bool:
    """Structural AST identity (``z3.eq``); ints/strs compare directly."""
    if left is right:
        return True
    if isinstance(left, (int, str)) or isinstance(right, (int, str)):
        return left == right
    try:
        return bool(z3.eq(left, right))
    except Exception:
        return False


def _constant_int(expr: Any, z3: Any) -> int | None:
    """Return the integer value of a constant expression, else ``None``.

    Accepts python ints and (simplified) Z3 constant bitvectors, so
    ``lr = pc + 4`` after a call (a constant ``bvand``/``bvadd`` tree over
    ``BitVecVal``s) is recognised as a constant for the location-independent
    relative comparison.
    """
    if isinstance(expr, int):
        return expr
    try:
        if z3.is_bv_value(expr):
            return expr.as_long()
    except Exception:
        pass
    try:
        simplified = z3.simplify(expr)
        if z3.is_bv_value(simplified):
            return simplified.as_long()
    except Exception:
        pass
    return None


def _value_equal(
    left: Any, right: Any, z3: Any, *, left_base: int, right_base: int
) -> bool:
    """Structural equality with the documented location-independence carve-out.

    ``Matched callees are assumed to be location-independent EABI functions:
    the absolute link-register return address is not a semantic input``
    (SOUNDNESS.md).  A ``bl`` writes ``lr = pc + 4`` (an absolute constant),
    so after a call the two sides' LR values (and ``return`` exit targets)
    differ by exactly the base offset even for identical code.  Constant
    values are therefore compared relative to the function base; symbolic
    expressions (which share variables across the sides) are compared
    structurally.
    """
    left_const = _constant_int(left, z3)
    right_const = _constant_int(right, z3)
    if left_const is not None and right_const is not None:
        if left_const == right_const:
            return True
        return (left_const - left_base) == (right_const - right_base)
    return _structurally_equal(left, right, z3)


def _terminals_agree(
    left: Any,
    right: Any,
    gpr_perm: list[int],
    fpr_perm: list[int],
    z3: Any,
    *,
    left_base: int,
    right_base: int,
) -> bool:
    """Structural agreement of two terminals under the renaming permutation.

    Compares every state component; GPR/FPR/PS1 lanes are indexed by the
    permutation (``retail.r_i`` vs ``decomp.r_perm[i]``), everything else
    directly.  Path feasibility is handled by the caller (disjoint pairs are
    skipped before this is called).  Constant values (e.g. ``lr = pc + 4``
    after a call) are compared relative to the function base per the
    location-independence assumption; symbolic values are compared
    structurally.
    """
    if left.exit_kind != right.exit_kind:
        return False
    if not _value_equal(
        left.exit_target, right.exit_target, z3,
        left_base=left_base, right_base=right_base,
    ):
        return False
    ls, rs = left.state, right.state
    if any(
        not _structurally_equal(ls.gpr[i], rs.gpr[gpr_perm[i]], z3)
        for i in range(32)
    ):
        return False
    if any(
        not _structurally_equal(ls.fpr[i], rs.fpr[fpr_perm[i]], z3)
        for i in range(32)
    ):
        return False
    if any(
        not _structurally_equal(ls.ps1[i], rs.ps1[fpr_perm[i]], z3)
        for i in range(32)
    ):
        return False
    direct_pairs = [
        (ls.cr, rs.cr),
        (ls.xer.ca, rs.xer.ca),
        (ls.xer.ov, rs.xer.ov),
        (ls.xer.so, rs.xer.so),
        (ls.fpscr, rs.fpscr),
        (ls.ctr, rs.ctr),
        (ls.msr, rs.msr),
        (ls.time_base, rs.time_base),
        (ls.srr0, rs.srr0),
        (ls.srr1, rs.srr1),
        (ls.valid, rs.valid),
        (ls.invalid_reason, rs.invalid_reason),
        (ls.stack_low, rs.stack_low),
        (ls.stack_layout_valid, rs.stack_layout_valid),
        (ls.stack_private, rs.stack_private),
        (ls.fp_pending_cause, rs.fp_pending_cause),
        (ls.fp_pending_fault_pc, rs.fp_pending_fault_pc),
        (ls.fp_pending_recoverable, rs.fp_pending_recoverable),
        (ls.fp_pending_delivery, rs.fp_pending_delivery),
    ]
    for left_v, right_v in direct_pairs:
        if not _structurally_equal(left_v, right_v, z3):
            return False
    # LR is the one component the summary writes as an absolute constant
    # (``pc + 4`` at the call site); compare it with the location-independent
    # carve-out.
    if not _value_equal(ls.lr, rs.lr, z3, left_base=left_base, right_base=right_base):
        return False
    # Register-file tuples (gqr, sr, aux spr) compare elementwise.
    for left_t, right_t in (
        (ls.gqr, rs.gqr),
        (ls.sr, rs.sr),
        (ls.spr, rs.spr),
    ):
        if len(left_t) != len(right_t) or any(
            not _structurally_equal(a, b, z3) for a, b in zip(left_t, right_t)
        ):
            return False
    if getattr(ls, "symbolic_bus", None) is not None or getattr(rs, "symbolic_bus", None) is not None:
        return False
    if not _structurally_equal(ls.memory, rs.memory, z3):
        return False
    return True


def run_structural_witness(
    original: list[Instruction],
    candidate: list[Instruction],
    rho: Rho,
    *,
    max_instructions: int = 2048,
    max_paths: int = 256,
    max_loop_iterations: int = DEFAULT_MAX_LOOP_ITERATIONS,
    assumed_callees: frozenset[int | str] = frozenset(),
    callee_contracts: dict[int | str, Any] | None = None,
    deadline: Deadline | None = None,
) -> WitnessOutcome:
    """Execute both sides through SymbolicOps and compare terminals.

    ``retail_r_i`` and ``decomp_r_rho(i)`` share one symbolic variable; both
    sides run through the audited ``execute_cfg``; terminal pairs whose path
    conditions are not disjoint must be structurally identical (``z3.eq``).
    Any divergence or execution inconclusiveness makes the witness fail and
    the caller falls through to the SMT probe.
    """
    ops = SymbolicOps()
    z3 = ops.z3
    gpr_perm = rho.gpr_perm()
    fpr_perm = rho.fpr_perm()
    retail_initial, decomp_initial = _symbolic_initial_pair(
        ops, gpr_perm, fpr_perm,
    )
    try:
        retail_exits = execute_cfg(
            retail_initial, original, ops,
            max_instructions=max_instructions,
            max_paths=max_paths,
            max_loop_iterations=max_loop_iterations,
            assumed_callees=assumed_callees,
            callee_contracts=callee_contracts or {},
            deadline=deadline,
        )
        decomp_exits = execute_cfg(
            decomp_initial, candidate, ops,
            max_instructions=max_instructions,
            max_paths=max_paths,
            max_loop_iterations=max_loop_iterations,
            assumed_callees=assumed_callees,
            callee_contracts=callee_contracts or {},
            deadline=deadline,
        )
    except (ProofDeadlineExceeded, Exception) as exc:
        if isinstance(exc, ProofDeadlineExceeded):
            return WitnessOutcome(False, rho=rho, failure=WitnessFailure(
                "deadline", exc.phase,
            ))
        return WitnessOutcome(False, rho=rho, failure=WitnessFailure(
            "execute", f"{type(exc).__name__}: {exc}",
        ))

    pairs_checked = 0
    left_base = original[0].address
    right_base = candidate[0].address
    for left in retail_exits:
        for right in decomp_exits:
            # Skip terminal pairs whose path conditions are disjoint: a
            # cheap propositional simplification (no solver).  Pairs that
            # can co-occur must be structurally identical.
            combined = z3.simplify(z3.And(left.condition, right.condition))
            if z3.is_false(combined):
                continue
            pairs_checked += 1
            if not _terminals_agree(
                left, right, gpr_perm, fpr_perm, z3,
                left_base=left_base, right_base=right_base,
            ):
                return WitnessOutcome(
                    False,
                    rho=rho,
                    failure=WitnessFailure(
                        "structural",
                        f"terminal pair ({left.exit_kind}, {right.exit_kind}) "
                        "diverges structurally",
                    ),
                    terminal_pairs_checked=pairs_checked,
                )
    return WitnessOutcome(
        True,
        rho=rho,
        structural_eq=True,
        terminal_pairs_checked=pairs_checked,
    )


def certify_renaming_witness(
    original: list[Instruction],
    candidate: list[Instruction],
    *,
    max_instructions: int = 2048,
    max_paths: int = 256,
    max_loop_iterations: int = DEFAULT_MAX_LOOP_ITERATIONS,
    assumed_callees: frozenset[int | str] = frozenset(),
    callee_contracts: dict[int | str, Any] | None = None,
    deadline_ms: int = 30_000,
) -> WitnessOutcome:
    """Full pipeline: gates 1-6, then the structural witness execution."""
    outcome = check_gates(original, candidate)
    if not outcome.certified:
        return outcome
    deadline = Deadline.after_ms(deadline_ms)
    return run_structural_witness(
        original, candidate, outcome.rho,
        max_instructions=max_instructions,
        max_paths=max_paths,
        max_loop_iterations=max_loop_iterations,
        assumed_callees=assumed_callees,
        callee_contracts=callee_contracts,
        deadline=deadline,
    )
