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
    ExecutionInconclusive,
    SymbolicOps,
    _path_condition_feasible,
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

# Opcodes whose RA field (bits 16-20) is the literal-zero position: r0 in RA
# encodes the constant zero and reads NO register (doc 32 A2, rev 3, per
# G3/G4/F8 — verified against the engine's ``ra == 0`` guards:
# ``_dform_address`` semantics.py:1854, LMW/STMW :4498, INDEXED_MEMORY
# :3531/:3559, ADDI/ADDIS :3296; DCBZ :3261 is reject-listed; PSQ D-forms
# :3874 are A1-scoped).  RA is a NON-register field for these: bit-equal,
# excluded from rho.  Everywhere else (cmpwi, mulli/addic/subfic, X-form
# arithmetic, M-form, RD/RB/RS, X-form load/store RB) r0 is an ordinary
# register.
_RA_LITERAL_OPCODES = frozenset(
    {
        Opcode.ADDI, Opcode.ADDIS,
        # D-form integer load/store + update forms.
        Opcode.LWZ, Opcode.LWZU, Opcode.LBZ, Opcode.LBZU,
        Opcode.LHZ, Opcode.LHZU, Opcode.LHA, Opcode.LHAU,
        Opcode.STW, Opcode.STWU, Opcode.STB, Opcode.STBU,
        Opcode.STH, Opcode.STHU, Opcode.LMW, Opcode.STMW,
        # D-form FP load/store + update forms.
        Opcode.LFS, Opcode.LFSU, Opcode.LFD, Opcode.LFDU,
        Opcode.STFS, Opcode.STFSU, Opcode.STFD, Opcode.STFDU,
        # X-form indexed integer load/store (RA is the base; RB is real).
        Opcode.LWZX, Opcode.LWZUX, Opcode.LBZX, Opcode.LBZUX,
        Opcode.LHZX, Opcode.LHZUX, Opcode.LHAX, Opcode.LHAUX,
        Opcode.STWX, Opcode.STWUX, Opcode.STBX, Opcode.STBUX,
        Opcode.STHX, Opcode.STHUX, Opcode.LWBRX, Opcode.STWBRX,
        Opcode.LHBRX, Opcode.STHBRX,
        # X-form indexed FP load/store (RA is the base; RB is real).
        Opcode.LFSX, Opcode.LFSUX, Opcode.LFDX, Opcode.LFDUX,
        Opcode.STFSX, Opcode.STFSUX, Opcode.STFDX, Opcode.STFDUX,
        Opcode.STFIWX,
        # PSQ D-forms (semantics.py:3874: ``ra == 0`` literal in the address
        # computation).  Their fS/fD and rA are non-register fields; the
        # rA entry here only drives the liveness RA-use guard.
        Opcode.PSQ_L, Opcode.PSQ_LU, Opcode.PSQ_ST, Opcode.PSQ_STU,
    }
)


def _register_fields(opcode: Opcode) -> tuple[tuple[int, str], ...]:
    """Return the ``(start_bit, kind)`` 5-bit register fields for ``opcode``.

    For ``_RA_LITERAL_OPCODES`` the RA field (bits 16-20) is dropped: r0 in
    RA is the literal zero on those forms (doc 32 A2), so it is a non-register,
    bit-equal field.  Unknown opcodes return ``()`` — the caller then treats
    every bit as non-register, which only makes the witness *stricter*
    (byte-equality of all non-register bits plus a trivially-empty rho), never
    looser.
    """
    fields = _register_fields_impl(opcode)
    if opcode in _RA_LITERAL_OPCODES:
        fields = tuple(f for f in fields if f[0] != 16)
    return fields


def _register_fields_impl(opcode: Opcode) -> tuple[tuple[int, str], ...]:
    """Pre-A2 role table (frozen for hexdiff's classifier copy)."""
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
        # TWI decodes (to, ra, simm): operand 0 is the 5-bit trap-condition
        # immediate, NOT a renameable register — only RA (operand 1) enters
        # rho (impl-review Finding 4).  RLWIMI's operand 0 (rA) is a genuine
        # GPR (read-modify-write) and stays in the (RD, RA) group.
        if opcode == Opcode.TWI:
            return (_RA,)
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
        Opcode.FMADD, Opcode.FNMSUB, Opcode.FNMADD,
    ):
        return (_FD, _FA, _FB, _FC)
    if opcode in (
        Opcode.FRSP, Opcode.FCTIW, Opcode.FCTIWZ, Opcode.FNEG,
        Opcode.FMR, Opcode.FNABS, Opcode.FABS,
    ):
        # 2-operand FP ops (impl-review MINOR): the decoder emits
        # (fd, fa=0, fb, fc=0); bits 6-10 are XO, not a renameable fC.
        # Modelling them with (_FD, _FA, _FB, _FC) made the XO bits look
        # like a register field and produced spurious rho conflicts
        # (e.g. fmr f1,f0 vs fmr f2,f0 collided on fC=2).  Real source is
        # FB (pos 2), matching _use_def.
        return (_FD, _FB)
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
        # PSQ X-forms stay rejected (doc 32 A1 rev 3, R2-5); the four D-forms
        # below leave REJECT_OPCODES and are handled by the byte-identical
        # exemption in _stream_validation_failure.
        Opcode.PSQ_LUX, Opcode.PSQ_LX,
        Opcode.PSQ_STUX, Opcode.PSQ_STX,
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

# PSQ D-forms exempted from the reject list by the byte-identical rule
# (doc 32 A1 rev 3).  X-forms stay rejected.
_PSQ_D_FORMS = frozenset(
    {Opcode.PSQ_L, Opcode.PSQ_LU, Opcode.PSQ_ST, Opcode.PSQ_STU}
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
# r0 dropped from the unconditional fixed set (doc 32 A2): r0 is now only
# special in the D/DS/X-indexed load-store and ADDI/ADDIS RA position, which is
# a bit-equal non-register field (gate 3), so rho may rename r0 elsewhere.
# r4/f1 are CONDITIONAL (doc 32 A3 rev 3): default FIXED; unfixed only when
# trusted registry metadata says the caller cannot observe them AND the body
# never writes them on a path to a return and there are no tail-call exits.
_UNCONDITIONALLY_FIXED_GPRS = frozenset({1, 2, 3, 13})
# f1 is the FP return register (and first FP argument) — conditional like r4.
_UNCONDITIONALLY_FIXED_FPRS = frozenset()

# r4 (integer return pair / 64-bit high word) and f1 (FP return) — the A3
# conditional set.  Metadata values that prove the caller does NOT observe the
# register (non-64-bit / non-aggregate returns; doc 32 A3 rev 3, I13 — the raw
# string, never the narrowed AbiShape).
_CONDITIONALLY_FIXED_GPRS = frozenset({4})
_CONDITIONALLY_FIXED_FPRS = frozenset({1})
_TRUSTED_NON_64BIT_RETURNS = frozenset(
    {"void", "i32", "u32", "f32", "f64", "bool", "ptr"}
)
# EABI outgoing-argument ranges: registers that may carry live-in inputs.
_EABI_ARG_GPRS = frozenset(range(3, 11))
_EABI_ARG_FPRS = frozenset(range(1, 9))
# EABI volatile GPRs (r0, r3-r12); r1/r2 are unconditionally fixed anyway.
_VOLATILE_GPRS = frozenset(range(13)) - frozenset({1, 2})
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
        # TWI decodes (to, ra, simm): operand 0 is the 5-bit trap-condition
        # immediate, NOT a register — the real use is ra at operand 1
        # (engine register_effects: semantics.py:5698-5700).  Treating `to`
        # as a GPR use under-approximated ra and polluted rho (impl-review
        # Finding 4).
        if opcode == Opcode.TWI:
            return ((1, g),), ()
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
        Opcode.MFCR, Opcode.MFMSR, Opcode.MFSR,
    ):
        return ((1, g),), ((0, g),)
    if opcode in (Opcode.MFSPR, Opcode.MFTB):
        # mfspr/mftb read an SPR/TBR index (operand 1), never a GPR — treat
        # as a pure def of rt (impl-review R2-N2: the old ((1,g),) shape
        # spuriously marked the SPR index as a GPR use).  MTSPR (below) reads
        # rt and writes the SPR — correct as ((0,g),), ().
        return (), ((0, g),)
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
        Opcode.ANDI_DOT, Opcode.ANDIS_DOT, Opcode.RLWINM,
        Opcode.SRAWI, Opcode.CNTLZW, Opcode.EXTSH, Opcode.EXTSB,
    ):
        return ((1, g),), ((0, g),)
    if opcode == Opcode.RLWIMI:
        # rlwimi rA, rS, sh, mb, me is a read-modify-write: operand 0 (rA)
        # is BOTH read (accumulator) and written (engine semantics:
        # semantics.py:3334 ``result = bor(result, band(gpr[ra], bnot(mask)))``)
        # and operand 1 (rS) is read.  Missing the rA read under-
        # approximated a use → liveness reported rA dead at the boundary →
        # unsound rebind (impl-review BLOCKER 1).
        return ((0, g), (1, g)), ((0, g),)
    if opcode in (Opcode.CMPWI, Opcode.CMPLWI):
        return ((1, g),), ()
    if opcode in (Opcode.CMPW, Opcode.CMPLW, Opcode.FCMPU, Opcode.FCMPO):
        kind = FPR if opcode in (Opcode.FCMPU, Opcode.FCMPO) else g
        return ((1, kind), (2, kind)), ()
    if opcode in (
        Opcode.PSQ_ST, Opcode.PSQ_STU, Opcode.PSQ_L, Opcode.PSQ_LU,
    ):
        # PSQ D-form operands are (fr, ra, disp, w, i) — semantics.py:3874.
        # Store reads fS (pos 0) + rA (pos 1); load defs fD (pos 0) + reads
        # rA (pos 1); update forms also def rA.  The ps1 lane side effects
        # (psq_st reads ps1[fS], psq_l defs ps1[fD]) are added in
        # _use_def_numbered (doc 32 A1 rev 3, F1/R2-6).
        defs = ((1, g),) if opcode in (Opcode.PSQ_STU, Opcode.PSQ_LU) else ()
        if opcode in (Opcode.PSQ_L, Opcode.PSQ_LU):
            return ((1, g),), ((0, FPR),) + defs
        return ((0, FPR), (1, g)), defs
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


# Scalar single-precision FP arithmetic that defines ``ps1[fd]`` as a side
# effect (semantics.py:4487-4488 ``if is_single: state.with_ps1(fd, d_bits)``
# over ``_FP_SINGLE_ARITH``, semantics.py:2116-2118).  These are NOT on the
# witness reject list, so the witness certifies functions that use them and
# the liveness fixpoint must model the ps1 def (doc: witness_expansion_plan
# §2.2, rev-5 correction).
_PS1_DEF_ARITH = frozenset(
    {
        Opcode.FDIVS, Opcode.FSUBS, Opcode.FADDS, Opcode.FRES,
        Opcode.FMULS, Opcode.FMSUBS, Opcode.FMADDS, Opcode.FNMSUBS,
        Opcode.FNMADDS, Opcode.FRSP,
    }
)

# Single-precision FP loads that define ``ps1[rt]`` (semantics.py:3613
# ``if width == 4: state.with_ps1(rt, result)``; modeled by the engine's own
# use-def table at semantics.py:5687).
_PS1_DEF_LOADS = frozenset(
    {Opcode.LFS, Opcode.LFSU, Opcode.LFSX, Opcode.LFSUX}
)

# PS1 lane numbering offset: GPR r -> r, FPR f -> 32 + f, PS1 f -> 64 + f.
_PS1_OFFSET = 64


def _numbered_lane(kind: str, value: int) -> int:
    """Map a (kind, register) pair into the combined liveness numbering."""
    if kind == GPR:
        return value
    if kind == FPR:
        return 32 + value
    return _PS1_OFFSET + value  # PS1


def _use_def_numbered(
    insn: Instruction,
) -> tuple[frozenset[int], frozenset[int]]:
    """Return ``(uses, defs)`` for one instruction in the combined numbering
    space (GPR r -> r, FPR f -> 32 + f, PS1 f -> 64 + f).

    PS1 defs: scalar-s FP arithmetic defines ``ps1[fd]`` (operand 0) and
    single-precision FP loads define ``ps1[rt]`` (operand 0) — both per
    witness_expansion_plan §2.2 (rev-5).  Unknown opcodes over-approximate
    BOTH directions (use+def of every GPR/FPR/PS1 lane) so liveness can never
    under-approximate a use when it is load-bearing (rev-4 finding 7: the
    unknown-opcode default must include PS1).
    """
    uses_raw, defs_raw = _use_def(insn.opcode)
    uses: set[int] = set()
    defs: set[int] = set()
    for pos, kind in uses_raw:
        if pos < len(insn.operands):
            # RA-literal guard (doc 32 A2 rev 3, G6/F5): on load/store and
            # ADDI/ADDIS forms the engine reads gpr[ra] only when ra != 0
            # (semantics.py:3296/3531/3559); RA=0 is the literal zero and must
            # not count as a use, or region-boundary liveness would spuriously
            # keep r0 live and over-reject rebinds.
            if (
                pos == 1
                and insn.opcode in _RA_LITERAL_OPCODES
                and insn.operands[1] == 0
            ):
                continue
            uses.add(_numbered_lane(kind, insn.operands[pos]))
    for pos, kind in defs_raw:
        if pos < len(insn.operands):
            defs.add(_numbered_lane(kind, insn.operands[pos]))
    op = insn.opcode
    # stmw/lmw operate over a register RANGE starting at operand 0 (rD),
    # which position-based tables cannot express: stmw stores rD..r31 (all
    # reads), lmw defines rD..r31 (engine table: semantics.py:5838).
    # Under-approximating the stmw reads was the unsound direction for
    # liveness (impl-review MAJOR 3).
    if op == Opcode.STMW and insn.operands:
        start = insn.operands[0]
        uses.update(_numbered_lane(GPR, r) for r in range(start, 32))
    elif op == Opcode.LMW and insn.operands:
        start = insn.operands[0]
        defs.update(_numbered_lane(GPR, r) for r in range(start, 32))
    if op in _PS1_DEF_ARITH or op in _PS1_DEF_LOADS:
        # Destination is operand 0 (fd for arith, rt for loads); both write
        # ps1 as a side effect.
        if insn.operands:
            defs.add(_PS1_OFFSET + insn.operands[0])
    if op in (Opcode.PSQ_ST, Opcode.PSQ_STU):
        # psq_st reads ps1[fS] in addition to fpr[fS] (semantics.py:3884-3888
        # ``source1 = ops.fp_bits_to_double(state.ps1[rs])``).  Missing this
        # use lets region-boundary liveness see ps1[fS] dead and rebind it to
        # a fresh shared variable — the F1 false-certificate hole.
        if insn.operands:
            uses.add(_PS1_OFFSET + insn.operands[0])
    elif op in (Opcode.PSQ_L, Opcode.PSQ_LU):
        # psq_l writes ps1[fD] alongside fpr[fD] (semantics.py:3889).
        if insn.operands:
            defs.add(_PS1_OFFSET + insn.operands[0])
    if op in _NO_REG_FIELDS or op in REJECT_OPCODES:
        # Branch/CR/system ops that carry no GPR/FPR register fields and
        # reject-listed ops: leave uses/defs as decoded (no ps1 side effect).
        pass
    elif not uses_raw and not defs_raw:
        # Unknown opcode: over-approximate BOTH directions over all lanes.
        uses = set(range(96))
        defs = set(range(96))
    return frozenset(uses), frozenset(defs)


def _cfg_successors(
    instructions: list[Instruction],
    index: int,
    by_index: dict[int, int],
    end_pc: int,
) -> tuple[int, ...]:
    """Successor instruction indices for ``instructions[index]`` (positions).

    Branches resolve through the static operand target; an out-of-function
    target or an indirect exit has no in-function successor (the value is
    live-out of the function — the fixpoint treats it as a terminal).  Calls
    (link=True) fall through to pc+4: the callee clobbers per contract, and
    gate-5's live-across-call set is computed separately (see
    ``_live_across_calls``).  Return ``()`` for a terminal.
    """
    insn = instructions[index]
    op = insn.opcode
    if op == Opcode.B:
        if insn.link:
            return (index + 1,) if index + 1 < len(instructions) else ()
        target = insn.operands[0]
        ti = by_index.get(target)
        return (ti,) if ti is not None else ()
    if op == Opcode.BC:
        if insn.link:
            return (index + 1,) if index + 1 < len(instructions) else ()
        succ: list[int] = []
        if index + 1 < len(instructions):
            succ.append(index + 1)
        target = insn.operands[2]
        ti = by_index.get(target)
        if ti is not None:
            succ.append(ti)
        return tuple(succ)
    if op in (Opcode.BCLR, Opcode.BCCTR):
        # Link: call — falls through.  Non-link: the executor yields a
        # *symbolic* taken predicate for BO=4/12/etc. (only BO=20 ``blr`` is
        # constant-true) and keeps a live fallthrough edge to pc+4
        # (semantics.py:5647) — verified: ``beqlr`` continues past the
        # branch.  Liveness must model the fallthrough for non-link BCLR/
        # BCCTR too (over-live is the safe direction; dropping it
        # under-approximates live and made the region deadness assertion
        # unsound — impl-review Finding 2).  An out-of-range fallthrough
        # (``blr`` at function end) returns () as a terminal.
        if insn.link:
            return (index + 1,) if index + 1 < len(instructions) else ()
        # Non-link BCLR/BCCTR: taken edge is a terminal (return / indirect
        # exit); the fallthrough continues when the branch is not taken.
        fall = (index + 1,) if index + 1 < len(instructions) else ()
        if insn.opcode == Opcode.BCLR and (insn.operands[0] & 0x14) == 0x14:
            # BO=20 (canonical ``blr``): always-taken return — no fallthrough.
            return ()
        return fall
    return (index + 1,) if index + 1 < len(instructions) else ()


def _cfg_liveness(
    instructions: list[Instruction],
) -> tuple[list[frozenset[int]], frozenset[int], frozenset[int]]:
    """Backwards dataflow fixpoint liveness over the real CFG.

    Returns ``(per_slot_live_out, entry_live_in, live_across_calls)`` where
    ``per_slot_live_out[i]`` is the live-out set of ``instructions[i]`` and
    both are in the combined numbering space (GPR r -> r, FPR f -> 32 + f,
    PS1 f -> 64 + f).  This is the load-bearing liveness for the region-
    sliced witness: a lane dead at a region boundary may be rebound (§2.1),
    a lane dead at an exit may be masked (§2.3) — both require fixpoint-
    accurate liveness, NOT the straight-line approximation it replaces.

    Branches are resolved through the CFG (backward edges included, so the
    fixpoint converges for loops); unknown opcodes over-approximate both
    directions (see ``_use_def_numbered``) so liveness can never
    under-approximate a use.
    """
    n = len(instructions)
    by_index = {insn.address: i for i, insn in enumerate(instructions)}
    end_pc = instructions[-1].address + 4
    succ: list[tuple[int, ...]] = []
    gen: list[frozenset[int]] = []
    kill: list[frozenset[int]] = []
    for i, insn in enumerate(instructions):
        succ.append(_cfg_successors(instructions, i, by_index, end_pc))
        uses, defs = _use_def_numbered(insn)
        gen.append(uses)
        kill.append(defs)
    # Standard backward dataflow fixpoint.
    live_in: list[set[int]] = [set() for _ in range(n)]
    live_out: list[set[int]] = [set() for _ in range(n)]
    changed = True
    while changed:
        changed = False
        for i in range(n - 1, -1, -1):
            new_out: set[int] = set()
            for s in succ[i]:
                new_out |= live_in[s]
            new_in = (new_out - kill[i]) | gen[i]
            if new_in != live_in[i] or new_out != live_out[i]:
                live_in[i] = new_in
                live_out[i] = new_out
                changed = True
    # Live-across-call: registers live after a call (the call's live-out).
    volatiles = set(_VOLATILE_GPRS) | {32 + r for r in _VOLATILE_FPRS}
    live_across: set[int] = set()
    for i, insn in enumerate(instructions):
        if _is_call(insn):
            live_across |= live_out[i] & volatiles
    return [frozenset(s) for s in live_out], frozenset(live_in[0]), frozenset(live_across)


def _liveness_sets(
    instructions: list[Instruction],
) -> tuple[frozenset[int], frozenset[int]]:
    """Return ``(live_in, live_across_calls)`` in one combined numbering
    space (GPR r -> r, FPR f -> 32 + f, PS1 f -> 64 + f).

    Computed via the CFG fixpoint (``_cfg_liveness``); branches and backward
    edges are modeled, so the entry live-in and live-across-call sets are
    fixpoint-accurate — the straight-line approximation this replaces
    silently missed loop-carried values, which would have made the region-
    sliced witness's deadness assertions unsound (review F1/B1).
    """
    _, entry_live_in, live_across = _cfg_liveness(instructions)
    return entry_live_in, live_across


def _has_loop_or_non_return_indirect(
    instructions: list[Instruction],
    *,
    local_symbol: str | None = None,
) -> bool:
    """First-cut loop predicate (witness_expansion_plan §2.2): True when the
    full-function CFG contains a backward direct branch, or a non-return
    indirect branch (``bctr``/``bcctr``, or ``bclr`` with ``link=True``).

    Return-position ``bclr`` (``link=False``, incl. predicated ``beqlr``/
    ``bnelr``) is a terminal per the executor (semantics.py:5516-5518) and
    is NOT a loop/indirect marker.  A relocated direct branch whose symbol is
    the local function (direct recursion) is a real back-edge and is flagged;
    other relocated branches (calls) are excluded from the static-target
    test.  This predicate is pinned as committed code + test fixture (rev-4
    finding 8): two reviewers disagreed 21-vs-19 on the same targets by
    eyeball — the code must be the authority.
    """
    by_address = {insn.address: i for i, insn in enumerate(instructions)}
    for i, insn in enumerate(instructions):
        op = insn.opcode
        if op == Opcode.BCCTR:
            return True  # indirect branch / jump-table dispatch — first cut
        if op == Opcode.BCLR:
            if insn.link:
                return True  # blrl — non-return indirect call
            continue  # return terminal — not a loop marker
        if op == Opcode.B:
            target = insn.operands[0]
            if insn.relocation is not None:
                # Relocated branch: a call unless it is direct recursion.
                # Non-link relocated self-branch (tail recursion) is ALSO a
                # real back-edge and must be flagged (impl-review MINOR 5:
                # the `insn.link` guard skipped tail-recursive self-calls).
                if insn.relocation.canonical_symbol == local_symbol:
                    return True  # direct self-recursion — real back-edge
                continue
            if target <= insn.address:
                return True
        elif op == Opcode.BC:
            if insn.link:
                continue  # conditional call
            if insn.relocation is not None:
                continue
            target = insn.operands[2]
            if target <= insn.address:
                return True
    return False


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

    Identity is preferred for every unused register present on both sides
    (free_domain AND free_range), then the remaining domains/ranges are
    matched in ascending order.  Without the identity-first pass, dropping an
    identity entry from rho (doc 32 A2: RA fields of load/store and
    ADDI/ADDIS are now non-register, so a register used only as an address
    base no longer contributes ``r -> r``) would let the ascending zip rotate
    unrelated live registers — e.g. rho {0:6} extending to 1->0, 2->1, ...,
    scrambling the r3 return lane and rejecting equivalent pairs.  The
    identity-first pass yields the natural 2-cycle closure (rho {0:6} extends
    to {0<->6}), which is canonical and a pure identity for byte-identical
    pairs.
    """
    perm = {k: v for k, v in partial.items()}
    used_domain = set(perm)
    used_range = set(perm.values())
    free_domain = set(range(32)) - used_domain
    free_range = set(range(32)) - used_range
    # Identity first: registers free on both sides map to themselves.
    for r in sorted(free_domain & free_range):
        perm[r] = r
    # Then the remainder (genuinely re-colored lanes) ascending.
    for key, value in zip(
        sorted(free_domain - free_range), sorted(free_range - free_domain),
    ):
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


def _stream_validation_failure(
    original: list[Instruction],
    candidate: list[Instruction],
) -> WitnessFailure | None:
    """Run the per-slot stream gates (reject-list, mnemonic, reloc, SPR
    policy, non-register bits) over ALL slots, returning the first failure.

    Extracted from ``check_gates`` so the region-sliced path can validate the
    FULL stream even when a rho conflict occurs mid-function (impl-review
    Finding 3: gates 2/3/6 after the first rho conflict were never checked,
    letting e.g. a reject-list ``mffs`` after a conflict be certified).
    """
    if len(original) != len(candidate):
        return WitnessFailure(
            "size", f"{len(original)} vs {len(candidate)} instructions",
        )
    for index, (r_insn, d_insn) in enumerate(zip(original, candidate)):
        # Gate 6 (reject-list) and SPR policy are checked per slot.
        if r_insn.opcode in REJECT_OPCODES or d_insn.opcode in REJECT_OPCODES:
            op = r_insn.opcode if r_insn.opcode in REJECT_OPCODES else d_insn.opcode
            return WitnessFailure("reject-list", f"slot {index}: {op.value}")
        # PSQ D-form exemption (doc 32 A1 rev 3): the four D-forms are allowed
        # iff the slot is byte-identical (their fS/fD/rA fields are
        # non-register — gate 3 enforces bit-equality anyway).  A
        # non-byte-identical PSQ pair falls back to SMT via reject-list.
        psq_r = r_insn.opcode in _PSQ_D_FORMS
        psq_d = d_insn.opcode in _PSQ_D_FORMS
        if psq_r or psq_d:
            if r_insn.opcode != d_insn.opcode or r_insn.raw != d_insn.raw:
                return WitnessFailure(
                    "reject-list",
                    f"slot {index}: non-byte-identical PSQ "
                    f"({r_insn.opcode.value} vs {d_insn.opcode.value})",
                )
        if r_insn.opcode != d_insn.opcode:
            return WitnessFailure(
                "mnemonic", f"slot {index}: {r_insn.opcode.value} vs {d_insn.opcode.value}",
            )
        if r_insn.opcode in (Opcode.MFSPR, Opcode.MTSPR):
            for insn in (r_insn, d_insn):
                if insn.operands[1] not in _WITNESS_ALLOWED_SPRS:
                    return WitnessFailure(
                        "reject-list",
                        f"slot {index}: {insn.opcode.value} to SPR {insn.operands[1]}",
                    )
        # Gate 2: per-slot relocation equality (offset/type/symbol/addend).
        r_reloc = r_insn.relocation
        d_reloc = d_insn.relocation
        if (r_reloc is None) != (d_reloc is None):
            return WitnessFailure(
                "reloc", f"slot {index}: reloc presence differs",
            )
        if r_reloc is not None:
            if (
                r_reloc.offset != d_reloc.offset
                or r_reloc.relocation_type != d_reloc.relocation_type
                or r_reloc.canonical_symbol != d_reloc.canonical_symbol
                or r_reloc.addend != d_reloc.addend
            ):
                return WitnessFailure(
                    "reloc",
                    f"slot {index}: {r_reloc.canonical_symbol}@{r_reloc.addend} "
                    f"vs {d_reloc.canonical_symbol}@{d_reloc.addend}",
                )
        # Gate 3: non-register field equality (raw-bit comparison).  Slots
        # with a matched relocation are exempt: the relocated bits are
        # placeholders that resolve to the same canonical symbol on both
        # sides (gate 2 binds identity).
        gpr_mask, fpr_mask = _gpr_fpr_masks(r_insn.opcode)
        register_mask = gpr_mask | fpr_mask
        if r_reloc is None:
            non_register_diff = (r_insn.raw ^ d_insn.raw) & ~register_mask
            if non_register_diff:
                return WitnessFailure(
                    "fields",
                    f"slot {index}: non-register bits differ "
                    f"(0x{non_register_diff:08x})",
                )
    return None


def check_gates(
    original: list[Instruction],
    candidate: list[Instruction],
    declared_return: str | None = None,
    callee_contracts: dict[int | str, Any] | None = None,
) -> WitnessOutcome:
    """Run gates 1-6; return the rho on success."""
    stream_failure = _stream_validation_failure(original, candidate)
    if stream_failure is not None:
        return WitnessOutcome(False, failure=stream_failure)

    rho_gpr: dict[int, int] = {}
    rho_fpr: dict[int, int] = {}

    for index, (r_insn, d_insn) in enumerate(zip(original, candidate)):
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
    failure = _check_abi_fixedness(original, candidate, rho, declared_return,
                              callee_contracts)
    if failure is not None:
        return WitnessOutcome(False, rho=rho, failure=failure)
    # A1 post-rho belt-and-suspenders (global path).
    failure = _psq_operands_rho_fixed(original, rho)
    if failure is not None:
        return WitnessOutcome(False, rho=rho, failure=failure)

    return WitnessOutcome(True, rho=rho)


def _tail_call_reads_lane(
    instructions: list[Instruction],
    lane_name: str,
    callee_contracts: dict[int | str, Any],
) -> bool:
    """True when any non-link ``b`` tail-call exits with a callee whose
    contract reads ``lane_name`` (doc 32 A3 rev 3, F7).

    A tail call passes EABI arguments to an out-of-function callee the proof
    cannot see; if the callee reads the lane, a rename there is observable.
    Unknown callees (no contract) and opaque ``*`` contracts conservatively
    read everything.  The ``_cfg_liveness`` fixpoint models no exit live-out,
    so this contract check is the only protection for tail-call exits; the
    default-FIXED rule covers them when the metadata is absent (F2/F7).
    """
    by_index = {insn.address: i for i, insn in enumerate(instructions)}
    for insn in instructions:
        if insn.opcode == Opcode.BCCTR and not insn.link:
            # Indirect tail call / computed jump (impl-review BLOCKER 2): the
            # target is unknown, so any callee may read the lane.  Conservative
            # (fix the lane); the G7 closure is incomplete without this.
            return True
        if insn.opcode == Opcode.B and not insn.link:
            if insn.relocation is not None:
                target: int | str = insn.relocation.canonical_symbol
            elif insn.operands and insn.operands[0] not in by_index:
                # Out-of-function absolute tail call (Kimi F2).  In-function
                # `b` jumps (gotos) are control flow, NOT tail calls — the
                # callee never sees the lane (impl-review r2 MINOR).
                target = insn.operands[0]
            else:
                continue
            contract = (callee_contracts or {}).get(target)
            if contract is None:
                return True  # unknown callee: conservative (fix the lane)
            reads = getattr(contract, "reads", None)
            if reads is None or "*" in reads or lane_name in reads:
                return True
    return False


def _written_before_return(
    instructions: list[Instruction],
    lane: int,
) -> bool:
    """True when ``lane`` is written on any forward path reaching a
    non-link ``bclr``/``bcctr`` return (doc 32 A3 rev 3, R2-4).

    Forward DFS/BFS over the existing ``_cfg_successors`` from each write
    site; a visited node that is itself a non-link BCLR/BCCTR is a potential
    return terminal (predicated forms included — the taken edge returns).
    Per-function, NOT per-region: a write in region 0 reaching a return in
    region 2 still fixes the register.  Coarse over-approximation is sound
    (only over-fixes, never unfixes).
    """
    by_index = {insn.address: i for i, insn in enumerate(instructions)}
    end_pc = instructions[-1].address + 4
    write_sites = [
        i for i, insn in enumerate(instructions)
        if lane in _use_def_numbered(insn)[1]
    ]
    if not write_sites:
        return False
    returns = {
        i for i, insn in enumerate(instructions)
        if insn.opcode in (Opcode.BCLR, Opcode.BCCTR) and not insn.link
    }
    if not returns:
        return False
    for start in write_sites:
        seen: set[int] = set()
        frontier = [start]
        while frontier:
            node = frontier.pop()
            if node in seen:
                continue
            seen.add(node)
            if node in returns:
                return True
            frontier.extend(_cfg_successors(instructions, node, by_index, end_pc))
    return False


def _check_abi_fixedness(
    original: list[Instruction],
    candidate: list[Instruction],
    rho: Rho,
    declared_return: str | None = None,
    callee_contracts: dict[int | str, Any] | None = None,
) -> WitnessFailure | None:
    # LR/CTR are inherently fixed (SPR indices are non-register bit-equal
    # fields); only GPR/FPR entries are checked here.
    fixed_gpr = set(_UNCONDITIONALLY_FIXED_GPRS)
    fixed_fpr = set(_UNCONDITIONALLY_FIXED_FPRS)
    # A3 conditional r4/f1 (doc 32 A3 rev 3): default FIXED.  Unfixed ONLY
    # when trusted metadata proves a non-64-bit/non-aggregate return AND the
    # body never writes the register on a forward path to a return AND there
    # are no tail-call exits (F7).  The structural check can only ever fix,
    # never unfix (F2/F3).
    for register, fixed_set, kind, lane_name in (
        (4, fixed_gpr, GPR, "r4"), (1, fixed_fpr, FPR, "f1"),
    ):
        lane = _numbered_lane(kind, register)
        if declared_return not in _TRUSTED_NON_64BIT_RETURNS:
            fixed_set.add(register)
        elif (
            _tail_call_reads_lane(original, lane_name, callee_contracts)
            or _tail_call_reads_lane(candidate, lane_name, callee_contracts)
        ):
            fixed_set.add(register)
        elif _written_before_return(original, lane) or _written_before_return(
            candidate, lane,
        ):
            fixed_set.add(register)
    # Registers read before being written (live-in at entry) in the EABI
    # argument ranges are the function's input signature and must be fixed.
    # The two sides are position-aligned with identical control flow, so a
    # union of both sides' live-in sets is the conservative choice.
    for instructions in (original, candidate):
        live_in, live_across = _liveness_sets(instructions)
        fixed_gpr.update(live_in & _EABI_ARG_GPRS)
        fixed_fpr.update(
            {n - 32 for n in live_in if 32 <= n < _PS1_OFFSET} & _EABI_ARG_FPRS
        )
        fixed_gpr.update(n for n in live_across if n < 32)
        fixed_fpr.update(n - 32 for n in live_across if 32 <= n < _PS1_OFFSET)
    # Validate the FULL permutation, not the partial rho (impl-review BLOCKER):
    # execution and the terminal comparison use ``gpr_perm()``/``fpr_perm()``,
    # whose canonical extension can map a fixed register non-identically when
    # another mapping steals its image (e.g. partial {5:3} forces the extension
    # to send r3 elsewhere).  Checking only the partial dict let ``li r5,1; blr``
    # vs ``li r3,1; blr`` certify with perm[3] = 5 while retail returns its r3
    # input and decomp returns 1 — a false certificate.
    gpr_perm = rho.gpr_perm()
    fpr_perm = rho.fpr_perm()
    for register in sorted(fixed_gpr):
        if gpr_perm[register] != register:
            return WitnessFailure(
                "abi-boundary",
                f"rho perm maps gpr r{register} -> r{gpr_perm[register]}; "
                f"ABI registers must be fixed",
            )
    for register in sorted(fixed_fpr):
        if fpr_perm[register] != register:
            return WitnessFailure(
                "abi-boundary",
                f"rho perm maps fpr f{register} -> f{fpr_perm[register]}; "
                f"ABI registers must be fixed",
            )
    return None


def _psq_operands_rho_fixed(
    instructions: list[Instruction],
    rho: Rho,
    start: int = 0,
    end: int | None = None,
) -> WitnessFailure | None:
    """Belt-and-suspenders PSQ check (doc 32 A1 rev 3, G2/F6): a
    byte-identical PSQ D-form slot's operand registers (fS/fD at pos 0, rA at
    pos 1) must be rho-fixed, or the slot would store/load different shared
    variables on the two sides (retail X_reg vs decomp X_{rho^{-1}(reg)}).
    The structural terminal comparison catches this anyway; this check is
    fail-closed diagnostics and is NOT load-bearing (the byte-identity gate
    and the ps1-liveness fix are).  rA=0 is the literal-zero position and
    reads nothing (semantics.py:3874), so it is skipped.
    """
    end = len(instructions) if end is None else end
    for index in range(start, end):
        insn = instructions[index]
        if insn.opcode not in _PSQ_D_FORMS:
            continue
        if len(insn.operands) >= 1:
            fd = insn.operands[0]
            if rho.fpr.get(fd, fd) != fd:
                return WitnessFailure(
                    "reject-list",
                    f"slot {index}: PSQ f{fd} renamed by rho "
                    f"(f{fd} -> f{rho.fpr.get(fd)})",
                )
        if len(insn.operands) >= 2 and insn.operands[1] != 0:
            ra = insn.operands[1]
            if rho.gpr.get(ra, ra) != ra:
                return WitnessFailure(
                    "reject-list",
                    f"slot {index}: PSQ r{ra} renamed by rho "
                    f"(r{ra} -> r{rho.gpr.get(ra)})",
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
    if pairs_checked == 0:
        # All terminal pairs disjoint (or empty exits): the witness compared
        # NOTHING — a vacuous True would be a false certificate.  Reject and
        # let the caller fall through to SMT (witness_expansion_plan §2.3).
        return WitnessOutcome(
            False,
            rho=rho,
            failure=WitnessFailure(
                "structural",
                "all path conditions disjoint (no comparable terminal pairs)",
            ),
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
    local_symbol: str | None = None,
    candidate_local_symbol: str | None = None,
    declared_return: str | None = None,
) -> WitnessOutcome:
    """Full pipeline: gates 1-6, then the structural witness execution.

    When the global bijection fails on the rho gate (a *local*
    register-allocation difference — no single global bijection exists),
    falls through to the region-sliced witness (expansion B) before giving
    up.  Region slicing rebinds changed lanes at boundaries gated on
    four-lane deadness; any deadness / executor / structural failure
    degrades to SMT (``certified=False``), never a false certificate.
    """
    outcome = check_gates(original, candidate, declared_return, callee_contracts)
    if not outcome.certified:
        if getattr(outcome.failure, "gate", None) == "rho":
            # Local rho conflict: try the region-sliced witness.
            return run_region_sliced_witness(
                original, candidate,
                max_instructions=max_instructions,
                max_paths=max_paths,
                max_loop_iterations=max_loop_iterations,
                assumed_callees=assumed_callees,
                callee_contracts=callee_contracts,
                deadline=Deadline.after_ms(deadline_ms),
                local_symbol=local_symbol,
                candidate_local_symbol=candidate_local_symbol,
                declared_return=declared_return,
            )
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


# ── region-sliced witness (expansion B) ────────────────────────────────────
#
# The global-bijection witness (above) rejects any pair whose register
# correspondence is not a single consistent bijection across the whole
# function.  MWCC routinely produces *local* register-allocation differences
# (a temp lives in r5 in one region and r4 in another); the pair is
# equivalent but no single rho exists.  Expansion B splits the stream into
# regions with a consistent bijection each, rebinds the shared symbolic
# variables at region boundaries (sound iff every changed binding is dead at
# the boundary on both sides — see witness_expansion_plan §2.1), and compares
# each terminal under its exit region's rho (§3.1).
#
# Soundness backstop: the full-state structural terminal comparison remains
# (all lanes + memory), the ``pairs_checked == 0`` guard rejects vacuous
# certifications (§2.3), and liveness is a CFG fixpoint (§2.2) — never the
# straight-line approximation, whose branch-ignoring would make the boundary
# deadness assertion unsound on loop-carried values.


def _rho_region_boundaries(
    original: list[Instruction],
    candidate: list[Instruction],
) -> list[int]:
    """Split-point instruction indices for the region-sliced witness.

    A split occurs at (a) any slot where the accumulated bijection changes
    (a Gate-4 conflict — the local temp re-allocation case) and (b) every
    call site (link-set B/BC/BCLR/BCCTR — volatiles may rebind across calls
    under precise contracts).  Splitting is conservative: extra boundaries
    only add deadness requirements (fail-closed), never loosen anything.  A
    boundary slot is the FIRST slot of the next region: its register fields
    seed the new region's bijection.
    """
    boundaries: list[int] = []
    rho_gpr: dict[int, int] = {}
    rho_fpr: dict[int, int] = {}

    def _conflicts(start: int, kind: str, rv: int, dv: int) -> bool:
        table = rho_gpr if kind == GPR else rho_fpr
        if rv in table:
            return table[rv] != dv
        return dv in table.values()

    for index, (r_insn, d_insn) in enumerate(zip(original, candidate)):
        if _is_call(r_insn) or _is_call(d_insn):
            if index > 0 and index not in boundaries:
                boundaries.append(index)
            rho_gpr = {}
            rho_fpr = {}
        # Accumulate this slot's register fields into the current region's
        # bijection; on conflict, start a new region at this slot.
        split_here = False
        for start_bit, kind in _register_fields(r_insn.opcode):
            rv = (r_insn.raw >> start_bit) & 0x1F
            dv = (d_insn.raw >> start_bit) & 0x1F
            if _conflicts(index, kind, rv, dv):
                split_here = True
                break
        if split_here:
            if index > 0 and index not in boundaries:
                boundaries.append(index)
            rho_gpr = {}
            rho_fpr = {}
        # Seed the (possibly fresh) region with this slot's fields.
        for start_bit, kind in _register_fields(r_insn.opcode):
            rv = (r_insn.raw >> start_bit) & 0x1F
            dv = (d_insn.raw >> start_bit) & 0x1F
            table = rho_gpr if kind == GPR else rho_fpr
            if rv not in table and dv not in table.values():
                table[rv] = dv
    return boundaries


def _region_rho(
    original: list[Instruction],
    candidate: list[Instruction],
    start: int,
    end: int,
) -> Rho | None:
    """Consistent bijection over [start, end); None when one does not exist
    (a conflict inside a region — a bug in boundary computation)."""
    rho_gpr: dict[int, int] = {}
    rho_fpr: dict[int, int] = {}
    for index in range(start, end):
        r_insn, d_insn = original[index], candidate[index]
        for start_bit, kind in _register_fields(r_insn.opcode):
            rv = (r_insn.raw >> start_bit) & 0x1F
            dv = (d_insn.raw >> start_bit) & 0x1F
            table = rho_gpr if kind == GPR else rho_fpr
            if rv in table:
                if table[rv] != dv:
                    return None
            elif dv in table.values():
                return None
            else:
                table[rv] = dv
    return Rho(gpr=rho_gpr, fpr=rho_fpr)


def _boundary_deadness_ok(
    original: list[Instruction],
    candidate: list[Instruction],
    boundary: int,
    old_rho: Rho,
    new_rho: Rho,
) -> bool:
    """Four-lane deadness at a region boundary (plan §2.1).

    The effective correspondence is the FULL permutation (``Rho.gpr_perm()``,
    which extends the partial bijection canonically): a lane whose partial
    mapping is absent can still change across a boundary via the extension
    (round-2 reviews' stale-lane ``m = rho_k(i')`` case).  For every retail
    lane ``r`` whose mapping changes, the rebind replaces the value in retail
    lane ``r`` and old decomp lane ``rho_k(r)``, and reuses new decomp lane
    ``rho_{k+1}(r)`` (whose old value came from retail
    ``rho_k^{-1}(rho_{k+1}(r))``).  Soundness requires ALL FOUR lanes dead at
    the boundary:

    - retail lane ``r`` dead on the retail side;
    - old decomp lane ``rho_k(r)`` dead on the decomp side (retail direction);
    - new decomp lane ``rho_{k+1}(r)`` dead on the decomp side (its old
      value, retail ``rho_k^{-1}(rho_{k+1}(r))``, is discarded);
    - the retail lane whose value the new decomp lane previously carried,
      ``rho_k^{-1}(rho_{k+1}(r))``, dead on the retail side (decomp direction).

    Liveness is the CFG fixpoint (``_cfg_liveness``); a lane is dead at the
    boundary iff it is not live-in to the boundary slot (live-out of the
    previous slot).
    """
    live_out_r, _, _ = _cfg_liveness(original)
    live_out_c, _, _ = _cfg_liveness(candidate)
    # live_r/live_c = lanes LIVE at the boundary (live-in to the boundary
    # slot = live-out of the previous slot).  A changed lane must NOT be in
    # these sets (deadness is required); naming them "dead_*" inverted the
    # meaning and invited review errors (impl-review NIT 7).
    live_r = live_out_r[boundary - 1] if boundary > 0 else frozenset()
    live_c = live_out_c[boundary - 1] if boundary > 0 else frozenset()

    def _num(kind: str, lane: int) -> int:
        if kind == GPR:
            return lane
        if kind == FPR:
            return 32 + lane
        return _PS1_OFFSET + lane

    for kind, old_perm, new_perm in (
        (GPR, old_rho.gpr_perm(), new_rho.gpr_perm()),
        (FPR, old_rho.fpr_perm(), new_rho.fpr_perm()),
    ):
        old_preimage: dict[int, int] = {}
        for r, d in enumerate(old_perm):
            old_preimage[d] = r
        for r in range(32):
            old_d = old_perm[r]
            new_d = new_perm[r]
            if old_d == new_d:
                continue  # mapping unchanged — no rebind needed
            # Four-lane deadness (round-2 reviews).
            if _num(kind, r) in live_r:
                return False  # retail lane r live at boundary
            if _num(kind, old_d) in live_c:
                return False  # old decomp lane live at boundary
            if _num(kind, new_d) in live_c:
                return False  # new decomp lane's old value live at boundary
            old_retail_of_new = old_preimage[new_d]
            if _num(kind, old_retail_of_new) in live_r:
                return False  # decomp direction: retail rho_k^{-1}(new_d) live
            if kind == FPR:
                # PS1 sub-lane deadness (doc 32 A1 rev 3, impl-review BLOCKER 1):
                # an FPR rebind replaces BOTH fpr[r] and ps1[r] with fresh
                # variables, so the four-lane deadness must hold for the PS1
                # sub-lanes too.  A byte-identical psq_st reads ps1[fS]; if a
                # double-precision op killed only fpr[fS], the FPR lane looks
                # dead while ps1[fS] stays live — rebinding then stores a fresh
                # variable on both sides and the memory comparison self-agrees,
                # a false certificate (the F1 ps1 use is necessary but not
                # sufficient; the CHECK must consult the sub-lane).
                for ps1_lane in (r, old_d, new_d, old_retail_of_new):
                    n = _PS1_OFFSET + ps1_lane
                    if n in live_r or n in live_c:
                        return False
    return True


def run_region_sliced_witness(
    original: list[Instruction],
    candidate: list[Instruction],
    *,
    max_instructions: int = 2048,
    max_paths: int = 256,
    max_loop_iterations: int = DEFAULT_MAX_LOOP_ITERATIONS,
    assumed_callees: frozenset[int | str] = frozenset(),
    callee_contracts: dict[int | str, Any] | None = None,
    deadline: Deadline | None = None,
    local_symbol: str | None = None,
    candidate_local_symbol: str | None = None,
    declared_return: str | None = None,
) -> WitnessOutcome:
    """Region-sliced structural witness (plan §3.1, §3.2).

    Splits the stream at bijection conflicts / call sites, executes each
    region through the executor with ``stop_at_pcs`` + ``initial_seed``,
    rebinds changed lanes at boundaries (fresh shared variables, deadness-
    asserted per §2.1), and compares each terminal pair under its exit
    region's rho.  Falls back (``certified=False``) on any deadness failure,
    executor inconclusiveness, or structural divergence — never a false
    certificate; the caller degrades to the SMT probe.
    """
    from dataclasses import replace as _replace

    ops = SymbolicOps()
    z3 = ops.z3
    # Full-stream validation (gates 1/2/3/6) BEFORE region slicing: the
    # global path returns at the first rho conflict, so slots after it were
    # never checked for reject-list / reloc / non-register-bit equality
    # (impl-review Finding 3 — an ``mffs`` after a conflict certified).
    stream_failure = _stream_validation_failure(original, candidate)
    if stream_failure is not None:
        return WitnessOutcome(False, failure=stream_failure)
    boundaries = _rho_region_boundaries(original, candidate)
    boundaries_set = frozenset(original[b].address for b in boundaries)
    # Region rho per boundary interval: [0, b0), [b0, b1), ..., [bn, end).
    region_starts = [0] + boundaries
    regions: list[tuple[int, int, Rho]] = []
    for i, start in enumerate(region_starts):
        end = region_starts[i + 1] if i + 1 < len(region_starts) else len(original)
        rho = _region_rho(original, candidate, start, end)
        if rho is None:
            return WitnessOutcome(False, failure=WitnessFailure(
                "rho", f"no consistent bijection in region [{start}, {end})",
            ))
        # Gate 5 PER REGION (impl-review BLOCKER 1): the global path never
        # runs gate 5 after a rho conflict, and the region path must enforce
        # ABI-boundary fixedness (r0/r1/r2/r13/args/returns, live-across-call
        # volatiles) in EVERY region's rho — otherwise a non-identity mapping
        # on a fixed register (e.g. the return register r3) self-consistently
        # passes the structural comparison under the region perm.
        abi_failure = _check_abi_fixedness(original, candidate, rho, declared_return,
                                  callee_contracts)
        if abi_failure is not None:
            return WitnessOutcome(False, rho=rho, failure=abi_failure)
        # A1 post-rho belt-and-suspenders (per region, doc 32 A1 rev 3, I12).
        psq_failure = _psq_operands_rho_fixed(original, rho, start, end)
        if psq_failure is not None:
            return WitnessOutcome(False, rho=rho, failure=psq_failure)
        regions.append((start, end, rho))
    # First-cut loop policy: region slicing assumes a loop-free target (the
    # fixpoint handles loops for liveness, but the rebinding driver is only
    # sound for the loop-free first cut; loop targets fall to SMT).
    if _has_loop_or_non_return_indirect(original, local_symbol=local_symbol) or \
            _has_loop_or_non_return_indirect(
                candidate, local_symbol=candidate_local_symbol or local_symbol,
            ):
        return WitnessOutcome(False, failure=WitnessFailure(
            "loop", "target contains a backward branch / non-return indirect; first cut",
        ))

    def _run_region(
        start_state: MachineState,
        start_pc: int,
        condition: Any,
        visits: dict[int, int],
        steps: int,
        stop: frozenset[int] | None,
        side: list[Instruction],
    ) -> tuple[list[Terminal], list[tuple[int, MachineState, Any, dict[int, int], int]]]:
        paused: list[tuple[int, MachineState, Any, dict[int, int], int]] = []
        if start_state is None:
            raise AssertionError("region driver: start_state required")
        # Seed-time feasibility pre-check (plan §3.2, impl-review Finding 7):
        # the seed bypasses enqueue's _path_condition_feasible, so an
        # infeasible resumed condition would otherwise execute until its
        # first prune.  Reject the region run outright instead.
        if not _path_condition_feasible(condition, ops):
            raise ExecutionInconclusive(
                "region driver: infeasible resumed path condition",
            )
        try:
            terms = execute_cfg(
                start_state, side, ops,
                max_instructions=max_instructions,
                max_paths=max_paths,
                max_loop_iterations=max_loop_iterations,
                assumed_callees=assumed_callees,
                callee_contracts=callee_contracts or {},
                deadline=deadline,
                stop_at_pcs=stop,
                initial_seed=(start_pc, start_state, condition, visits, steps),
                paused_out=paused,
            )
        except (ProofDeadlineExceeded, Exception) as exc:
            if isinstance(exc, ProofDeadlineExceeded):
                raise
            raise ExecutionInconclusive(f"region executor: {type(exc).__name__}: {exc}") from exc
        return terms, paused

    try:
        # Region 0: initial shared-variable pair under region 0's rho.
        rho0 = regions[0][2]
        retail_initial, decomp_initial = _symbolic_initial_pair(
            ops, rho0.gpr_perm(), rho0.fpr_perm(),
        )
        # Frontier entries per side: (pc, state, condition, visits, steps).
        retail_frontier = [(original[0].address, retail_initial, ops.bool(True), {}, 0)]
        decomp_frontier = [(candidate[0].address, decomp_initial, ops.bool(True), {}, 0)]
        all_retail_terms: list[tuple[Terminal, int]] = []  # (terminal, region idx)
        all_decomp_terms: list[tuple[Terminal, int]] = []
        # Driver-side cross-region max_paths budget (plan §3.2, impl-review
        # MAJOR 4): enqueue's len(work)+len(terminals) bound resets per
        # execute_cfg call, so without driver accounting the total frontier
        # could reach num_regions * max_paths.  Snapshot the cumulative
        # frontier+terminal count after each region and fall to SMT
        # (ExecutionInconclusive) past the budget.  (R2-N1: a += accumulator
        # re-counted prior regions' terminals — replaced by a snapshot check.)
        for region_idx, (start, end, rho) in enumerate(regions):
            retail_stop = (
                frozenset({original[regions[region_idx + 1][0]].address})
                if region_idx + 1 < len(regions) else None
            )
            decomp_stop = (
                frozenset({candidate[regions[region_idx + 1][0]].address})
                if region_idx + 1 < len(regions) else None
            )
            new_retail_frontier: list[tuple[int, MachineState, Any, dict[int, int], int]] = []
            new_decomp_frontier: list[tuple[int, MachineState, Any, dict[int, int], int]] = []
            for entry in retail_frontier:
                terms, paused = _run_region(
                    entry[1], entry[0], entry[2], entry[3], entry[4],
                    retail_stop, original,
                )
                for t in terms:
                    all_retail_terms.append((t, region_idx))
                new_retail_frontier.extend(paused)
            for entry in decomp_frontier:
                terms, paused = _run_region(
                    entry[1], entry[0], entry[2], entry[3], entry[4],
                    decomp_stop, candidate,
                )
                for t in terms:
                    all_decomp_terms.append((t, region_idx))
                new_decomp_frontier.extend(paused)
            # Snapshot budget check: cumulative frontier + terminal count
            # across BOTH sides and ALL regions so far (R2-N1 fix — no
            # double-count; over-budget falls to SMT).
            cumulative = (
                len(new_retail_frontier) + len(new_decomp_frontier)
                + len(all_retail_terms) + len(all_decomp_terms)
            )
            if cumulative > max_paths:
                raise ExecutionInconclusive(
                    f"region driver: cumulative path budget exceeded ({max_paths})",
                )
            if region_idx + 1 < len(regions):
                # Rebind at the boundary: changed lanes get fresh shared
                # variables on both sides, gated on two-direction deadness.
                old_rho, new_rho = rho, regions[region_idx + 1][2]
                boundary = regions[region_idx + 1][0]
                if not _boundary_deadness_ok(original, candidate, boundary, old_rho, new_rho):
                    return WitnessOutcome(False, failure=WitnessFailure(
                        "abi-boundary",
                        f"region boundary {boundary}: a changed lane is live "
                        "across the boundary (two-direction deadness)",
                    ))
                # Fresh shared variables for changed lanes, applied to every
                # paused frontier entry on both sides (path conditions are
                # carried per entry).  Retail lane r and decomp lane
                # new_perm(r) receive the SAME fresh variable so both sides
                # share it (old values dead — asserted above).
                new_gpr_perm = new_rho.gpr_perm()
                new_fpr_perm = new_rho.fpr_perm()
                old_gpr_perm = rho.gpr_perm()
                old_fpr_perm = rho.fpr_perm()
                fresh_gpr = [z3.BitVec(f"witness.r{i}.r{region_idx + 1}", 32) for i in range(32)]
                fresh_fpr = [z3.BitVec(f"witness.f{i}.r{region_idx + 1}", 64) for i in range(32)]
                fresh_ps1 = [z3.BitVec(f"witness.f{i}.ps1.r{region_idx + 1}", 64) for i in range(32)]

                def _changed(retail_lane: int, kind: str, new_perm: list[int], old_perm: list[int]) -> bool:
                    return old_perm[retail_lane] != new_perm[retail_lane]

                def _rebind_retail(state: MachineState) -> MachineState:
                    gpr = list(state.gpr)
                    fpr = list(state.fpr)
                    ps1 = list(state.ps1)
                    for r in range(32):
                        if _changed(r, GPR, new_gpr_perm, old_gpr_perm):
                            gpr[r] = fresh_gpr[r]
                    for r in range(32):
                        if _changed(r, FPR, new_fpr_perm, old_fpr_perm):
                            fpr[r] = fresh_fpr[r]
                            ps1[r] = fresh_ps1[r]
                    return _replace(state, gpr=tuple(gpr), fpr=tuple(fpr), ps1=tuple(ps1))

                def _rebind_decomp(state: MachineState) -> MachineState:
                    # Decomp lane j plays retail lane new_perm^{-1}(j); give it
                    # the fresh variable of its retail partner.
                    gpr = list(state.gpr)
                    fpr = list(state.fpr)
                    ps1 = list(state.ps1)
                    for r in range(32):
                        if _changed(r, GPR, new_gpr_perm, old_gpr_perm):
                            gpr[new_gpr_perm[r]] = fresh_gpr[r]
                    for r in range(32):
                        if _changed(r, FPR, new_fpr_perm, old_fpr_perm):
                            fpr[new_fpr_perm[r]] = fresh_fpr[r]
                            ps1[new_fpr_perm[r]] = fresh_ps1[r]
                    return _replace(state, gpr=tuple(gpr), fpr=tuple(fpr), ps1=tuple(ps1))

                boundary_pc_r = original[boundary].address
                boundary_pc_c = candidate[boundary].address
                retail_frontier = [
                    (boundary_pc_r, _rebind_retail(e[1]), e[2], e[3], e[4])
                    for e in new_retail_frontier
                ]
                decomp_frontier = [
                    (boundary_pc_c, _rebind_decomp(e[1]), e[2], e[3], e[4])
                    for e in new_decomp_frontier
                ]
            else:
                retail_frontier = new_retail_frontier
                decomp_frontier = new_decomp_frontier
        if retail_frontier or decomp_frontier:
            return WitnessOutcome(False, failure=WitnessFailure(
                "execute", "frontier not exhausted after last region",
            ))
        # Terminal comparison under each exit region's rho.
        pairs_checked = 0
        left_base = original[0].address
        right_base = candidate[0].address
        for left, lregion in all_retail_terms:
            for right, rregion in all_decomp_terms:
                combined = z3.simplify(z3.And(left.condition, right.condition))
                if z3.is_false(combined):
                    continue
                pairs_checked += 1
                if lregion != rregion:
                    # Exits in different regions: the two sides' exit PCs are
                    # at different region-relative positions, which cannot be
                    # equivalent under position-aligned streams — reject.
                    return WitnessOutcome(
                        False,
                        failure=WitnessFailure(
                            "structural",
                            f"terminal pair regions differ ({lregion} vs {rregion})",
                        ),
                        terminal_pairs_checked=pairs_checked,
                    )
                lrho = regions[lregion][2]
                gpr_perm = lrho.gpr_perm()
                fpr_perm = lrho.fpr_perm()
                if not _terminals_agree(
                    left, right, gpr_perm, fpr_perm, z3,
                    left_base=left_base, right_base=right_base,
                ):
                    return WitnessOutcome(
                        False,
                        failure=WitnessFailure(
                            "structural",
                            f"terminal pair ({left.exit_kind}, {right.exit_kind}) "
                            "diverges structurally under region rho",
                        ),
                        terminal_pairs_checked=pairs_checked,
                    )
        if pairs_checked == 0:
            return WitnessOutcome(False, failure=WitnessFailure(
                "structural",
                "all path conditions disjoint (no comparable terminal pairs)",
            ))
        return WitnessOutcome(
            True,
            rho=regions[0][2],
            structural_eq=True,
            terminal_pairs_checked=pairs_checked,
            details={
                "rho_mode": "region-sliced",
                "regions": [
                    {"start": s, "end": e, "rho": r.to_dict()}
                    for s, e, r in regions
                ],
            },
        )
    except ExecutionInconclusive as exc:
        return WitnessOutcome(False, failure=WitnessFailure(
            "execute", f"ExecutionInconclusive: {exc}",
        ))
    except ProofDeadlineExceeded as exc:
        return WitnessOutcome(False, failure=WitnessFailure("deadline", exc.phase))
    except Exception as exc:  # pragma: no cover - defensive
        return WitnessOutcome(False, failure=WitnessFailure(
            "execute", f"{type(exc).__name__}: {exc}",
        ))
