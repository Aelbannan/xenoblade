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
5. ABI-boundary fixedness — rho must fix r0, r1, r2, r13, LR/CTR (inherently
   fixed: SPR indices are non-register fields), all argument/return registers
   (r3–r10, f1–f8), every register live-in at entry (any lane — not just the
   EABI argument ranges: a live-in r11/r12/r14–r31/f0/f9–f31 is an input the
   caller placed in a physical lane), and every volatile register live across
   a call (opaque-EABI clobber set).  Round-3 review BLOCKER fix: at EVERY
   call/tail-call site, rho must fix every lane the callee READS — precise
   ``contract.reads`` when a contract exists, otherwise the EABI argument
   window r3–r10 / f1–f8 (+ ps1 sub-lanes) — because a callee observes its
   arguments in physical lanes and the F3 token canonicalization would hide a
   rename of an observed lane from the structural comparison.  Nonvolatile
   permutations across calls (e.g. r20<->r25, both preserved by EABI) are
   SOUND and are NOT pre-rejected
   — that is the Chaitin-cycle class this feature exists for — but the pair
   must additionally PASS the per-terminal nonvolatile preservation check
   (F1, adversarial review 2026-08): each side must restore every nonvolatile
   lane it touches (terminal value ≡ entry binding after ``z3.simplify``).
6. Reject-list — ``ps_*``, ``psq_*``, ``mtfsf``/``mffs``/``mcrfs``/
   ``mtfsb*``/``mtfsfi``, ``mtspr``/``mfspr`` to GQRs (912–919) or any
   non-{LR,CTR,XER} SPR, ``dcbz``/``icbi``/``tlb*``, privileged/system
   opcodes, and indirect dispatch (``bcctr``/``blrl`` — F2, adversarial
   review 2026-08: the global path now rejects these exactly like the region
   path) fall straight to SMT and are never certified via renaming.

Additional checks (adversarial review 2026-08, all fail-closed):
- F3: the callee token is canonicalized through the rho (retail lane order)
  so opaque contracts certify the across-call Chaitin class; sound because a
  genuine EABI callee observes only fixed/shared lanes — and round-3 review
  now ENFORCES that: every lane a callee reads (``contract.reads`` for
  precise contracts; ALL 96 lanes for opaque/unknown callees — R3) is
  rho-fixed at each call site by gate 5, so the canonicalization only ever
  reorders lanes the callee does not observe.
- S1: the terminal memory comparison is location-aware (stored ``pc+4``
  constants compare relative to the function base instead of over-rejecting).

Known limitations (round-3 review, documented trust boundaries):
- Opaque/unknown callees are handled conservatively: ``_call_observed_lanes``
  fixes ALL 96 lanes (GPR/FPR/PS1) at every call/tail-call site when the
  callee is opaque (``reads="*"``) or has no contract (round-3 review R3),
  because EABI constrains preservation, not reads — a callee may read any
  volatile lane or any nonvolatile it preserves.  Precise contracts
  (certified callees) narrow the fixed set to ``contract.reads``, and the
  soundness of that narrowing is ``callee_inference``'s responsibility: a
  precise ``reads`` that under-approximates the callee's dataflow-affecting
  inputs is the residual false-certificate class (round-8 review, GLM F1).
- Precise ``contract.reads`` must be a sound over-approximation of the
  callee's dataflow-affecting inputs — ``callee_inference``'s responsibility
  for certified callees, and the live-in read scan in
  ``_full_match_callee_body_fits_narrow`` (equivalence_check.py) for
  FULL_MATCH narrow callees (round-2 review).
- A non-link ``bclr`` used as an INDIRECT TAIL CALL (``mtlr rN; bclr`` with
  N a callee address) is not a call site here and certifies like a return.
  Not MWCC-reachable (computed jumps use ``mtctr;bcctr``, rejected by
  ``_has_indirect_dispatch``); LR is reserved for return addresses.
"""

from __future__ import annotations

from dataclasses import dataclass, field
from typing import Any

from tools.ppc_equivalence.deadline import Deadline, ProofDeadlineExceeded
from tools.ppc_equivalence.ir import (
    R_PPC_ADDR16_HA,
    R_PPC_ADDR16_HI,
    R_PPC_ADDR16_LO,
    R_PPC_EMB_SDA21,
    R_PPC_REL14,
    R_PPC_REL24,
    Instruction,
    Opcode,
)
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

# X-form ops whose RA/RB operands commute: `add rA,rB,rC` == `add rA,rC,rB`.
# The rho builder treats operand positions positionally, so a pure operand-
# order swap (retail `add r0,r0,r4` vs decomp `add r0,r4,r0`) produced a
# many-to-one rho conflict and the witness rejected byte-identical-equivalent
# pairs (us-8025658c / us-8025650c stuck at 99.4-99.6% on exactly this —
# hexdiff said "2 pure reg-swaps" but the witness refused).
_COMMUTATIVE_RA_RB = frozenset({
    Opcode.ADD, Opcode.ADDC, Opcode.ADDE,
    Opcode.AND, Opcode.ANDC, Opcode.EQV, Opcode.NAND, Opcode.NOR,
    Opcode.OR, Opcode.ORC, Opcode.XOR,
    Opcode.MULLW, Opcode.MULHW, Opcode.MULHWU,
    Opcode.SUBF, Opcode.SUBFC, Opcode.SUBFE,
})

# Symmetric-compare extension (vf9, us-8017e6b8): CR-logic opcodes whose
# BA/BB source fields conservatively count as whole-field reads when
# deciding whether a compare's LT/GT/UN bits are observable.


_CR_WRITE_OPS = None


def _writes_cr_field(ins, crf):
    """True when ins writes (all of) CR field crf."""
    global _CR_WRITE_OPS
    if _CR_WRITE_OPS is None:
        _CR_WRITE_OPS = frozenset({
            Opcode.CMPW, Opcode.CMPLW, Opcode.CMPWI, Opcode.CMPLWI,
            Opcode.FCMPU, Opcode.FCMPO,
        })
    raw = getattr(ins, "raw", None)
    if raw is None:
        return False
    op = ins.opcode
    if op in _CR_WRITE_OPS:
        return ((raw >> 6) & 0x7) == crf
    mcrf = getattr(Opcode, "MCRF", None)
    if mcrf is not None and op == mcrf:
        return ((raw >> 6) & 0x7) == crf
    mtcrf = getattr(Opcode, "MTCRF", None)
    if mtcrf is not None and op == mtcrf:
        fxm = (raw >> 12) & 0xFF
        lo = 2 * (7 - crf)
        return ((fxm >> lo) & 0x3) == 0x3
    return False


def _reads_cr_non_eq(ins, crf):
    """True when ins consumes a non-EQ bit of CR field crf."""
    raw = getattr(ins, "raw", None)
    if raw is None:
        return False
    op = ins.opcode
    if op in (Opcode.BC, Opcode.BCLR, Opcode.BCCTR):
        bo = (raw >> 21) & 0x1F
        if bo & 0x10:
            return False  # BO says branch unconditionally - no CR test
        bi = (raw >> 16) & 0x1F  # BI lives at bits 16-20 on B-form branches
        return ((bi >> 2) & 0x7) == crf and (bi & 0x3) != 2
    mcrf = getattr(Opcode, "MCRF", None)
    if mcrf is not None and op == mcrf:
        return (((raw >> 11) & 0x7) >> 2) == crf
    if op in _CR_LOGIC_OPCODES:
        ba = (raw >> 11) & 0x7
        bb = (raw >> 16) & 0x7
        return crf in (ba >> 2, bb >> 2)
    return False


def _swapped_compare_order_safe(instructions, pos, crf):
    """Kill-analysis: a non-EQ read of crf after pos blocks the swap only
    when no intervening write to crf kills the swapped values first."""
    first_post_write = None
    for idx in range(pos + 1, len(instructions)):
        if _writes_cr_field(instructions[idx], crf):
            first_post_write = idx
            break
    for idx in range(pos + 1, len(instructions)):
        if _reads_cr_non_eq(instructions[idx], crf):
            blocked = any(
                _writes_cr_field(instructions[k], crf)
                for k in range(pos + 1, idx + 1)
            )
            if not blocked:
                return False
    return True


_CR_LOGIC_OPCODES = frozenset({
    Opcode.CRAND, Opcode.CRANDC, Opcode.CREQV, Opcode.CRNAND,
    Opcode.CRNOR,
    Opcode.CRORC, Opcode.CRXOR,
})


def _cr_field_non_eq_reads(instructions: list) -> set:
    """Return crfD indexes whose LT/GT/UN bits are consumed by any branch.

    Scans BC/BCLR/BCCTR words: BI (bits 11-15) encodes ``crfD*4 + bit`` with
    bit 0=LT, 1=GT, 2=EQ, 3=UN.  A branch testing any bit other than EQ is a
    non-EQ consumption.  MCRF and CR-logic ops read whole fields and are
    treated conservatively as non-EQ reads of their source fields.
    Used by the symmetric-compare operand-swap tolerance (see §7j-2 spec in
    MWCC_PATTERNS.md): an FCMPU/FCMPO pair whose compared field's non-EQ bits
    are never read may have its operands swapped without semantic change.
    """
    reads: set = set()
    mcrf = getattr(Opcode, "MCRF", None)
    for insn in instructions:
        op = insn.opcode
        raw = getattr(insn, "raw", None)
        if raw is None:
            continue
        if op in (Opcode.BC, Opcode.BCLR, Opcode.BCCTR):
            bi = (raw >> 11) & 0x1F
            crf = (bi >> 2) & 0x7
            bit = bi & 0x3
            if bit != 2:
                reads.add(crf)
        elif mcrf is not None and op == mcrf:
            src = (raw >> 11) & 0x7
            reads.add(src)
        elif op in _CR_LOGIC_OPCODES:
            # Conservative: BA/BB may reference any field.
            reads.add((raw >> 11) & 0x7)
            reads.add((raw >> 16) & 0x7)
    return reads


def _cr_order_insensitive_mask(
    instructions_a: list, instructions_b: list,
) -> int:
    """CR bit-mask clearing LT/GT/UN bits of order-insensitive fields.

    A field is order-insensitive when it is call-clobbered (EABI volatile:
    cr0/cr5-cr7 - exit remnants unobservable by conforming callers) and its
    LT/GT/UN bits are never consumed by any branch or CR-logic op in either
    function.  Divergences confined to cleared bits are unobservable and may
    be ignored by the terminal comparison (symmetric-compare extension,
    vf9/us-8017e6b8).  Soundness: any consumer would be caught by the
    non-EQ-read scan; volatile fields carry no exit obligation.
    """
    ignore = 0
    non_eq = (_cr_field_non_eq_reads(instructions_a)
              | _cr_field_non_eq_reads(instructions_b))
    for f in _volatile_cr_fields():
        if f in non_eq:
            continue
        base = 28 - 4 * f  # nibble low bit position (UN)
        # clear LT (base+3), GT (base+2), UN (base); keep EQ (base+1)
        ignore |= (1 << (base + 3)) | (1 << (base + 2)) | (1 << base)
    return ignore


def _volatile_cr_fields() -> set:
    """CR fields that are call-clobbered (EABI): exit values unobservable."""
    return {0, 5, 6, 7}
# Round-9 R9-1/R9-4: load opcodes that can read a spill slot ``(r1 + c)``.
# C1/C2 in ``_live_in_spill_only`` scan these to detect slot reads; ``lmw``
# (loads rD..r31, multi-lane) is handled separately (reject — cannot confine).
# X-form loads (register-indexed) are tracked separately: with rA == r1 OR
# rB == r1 (round-9 R9-4 finding N5 — the X-form effective address is
# ``gpr[rA] + gpr[rB]``, so EITHER base register may carry r1), the register
# index could alias any spill displacement, so they are treated conservatively
# as potential slot reads.
#
# R9-4 (second adversarial round): the set is scanned across BOTH register
# kinds (a GPR spill slot read by an FPR load, or vice versa, is an equally
# observable escape — findings N3/N4), includes the byte-reversed X-form
# loads ``lwbrx``/``lhbrx`` (GLM finding 1 / Kimi N6/N7) and the paired-single
# D-form loads ``psq_l``/``psq_lu`` (Kimi N11).
_SPILL_SLOT_LOAD_GPR = frozenset(
    {
        Opcode.LWZ, Opcode.LWZU, Opcode.LWZX, Opcode.LWZUX,
        Opcode.LBZ, Opcode.LBZU, Opcode.LBZX, Opcode.LBZUX,
        Opcode.LHZ, Opcode.LHZU, Opcode.LHZX, Opcode.LHZUX,
        Opcode.LHA, Opcode.LHAU, Opcode.LHAX, Opcode.LHAUX,
        Opcode.LWBRX, Opcode.LHBRX,
        Opcode.LMW,
    }
)
_SPILL_SLOT_LOAD_FPR = frozenset(
    {
        Opcode.LFS, Opcode.LFSU, Opcode.LFSX, Opcode.LFSUX,
        Opcode.LFD, Opcode.LFDU, Opcode.LFDX, Opcode.LFDUX,
        Opcode.PSQ_L, Opcode.PSQ_LU, Opcode.PSQ_LX, Opcode.PSQ_LUX,
    }
)
_SPILL_SLOT_XFORM_LOADS = frozenset(
    {
        Opcode.LWZX, Opcode.LWZUX, Opcode.LBZX, Opcode.LBZUX,
        Opcode.LHZX, Opcode.LHZUX, Opcode.LHAX, Opcode.LHAUX,
        Opcode.LWBRX, Opcode.LHBRX,
        Opcode.LFSX, Opcode.LFSUX, Opcode.LFDX, Opcode.LFDUX,
        Opcode.PSQ_LX, Opcode.PSQ_LUX,
    }
)
# Union of every load opcode that can read a spill slot, regardless of the
# spilled lane's register kind (R9-4: cross-kind slot reads escape).
_SPILL_SLOT_LOADS_ALL = _SPILL_SLOT_LOAD_GPR | _SPILL_SLOT_LOAD_FPR
# D-form load access widths (bytes) for the byte-range overlap test (R9-4,
# Kimi N1: ``lhz`` at disp+2 reads bytes 10-11 of a 4-byte slot at 8).
_SPILL_SLOT_LOAD_WIDTH = {
    Opcode.LWZ: 4, Opcode.LWZU: 4, Opcode.LBZ: 1, Opcode.LBZU: 1,
    Opcode.LHZ: 2, Opcode.LHZU: 2, Opcode.LHA: 2, Opcode.LHAU: 2,
    Opcode.LFS: 4, Opcode.LFSU: 4, Opcode.LFD: 8, Opcode.LFDU: 8,
    Opcode.PSQ_L: 4, Opcode.PSQ_LU: 4,
}
# Spill-slot width per register kind: GPR ``stw`` stores 4 bytes, FPR
# ``stfd`` stores 8 bytes.  Used for byte-range overlap and for the
# cross-kind access-width check.
_SPILL_SLOT_WIDTH = {GPR: 4, FPR: 8}


def _spill_slot_load_width(op: Opcode, insn: Instruction) -> int:
    """Access width (bytes) of a D-form spill-slot load candidate.

    R9-4 H9 fix: PSQ loads are operand-width-dependent — the engine models
    ``psq_span = 4 if w else 8`` (semantics.py:3927), and a W=0 psq_l reads
    TWO 4-byte words (``addr`` and ``addr+4``; the second lands in ps1).  The
    C1 byte-range overlap test must use the REAL width or a W=0 psq_l at
    disp-4 of a slot reads [disp, disp+8) ⊇ slot while the width-4 test sees
    [disp, disp+4) disjoint — an escape (GLM F1 / Kimi H9).  The W bit is
    operand index 3 of the decoded D-form (fd, ra, d, w, t).
    """
    if op in (Opcode.PSQ_L, Opcode.PSQ_LU):
        return 8 if (len(insn.operands) > 3 and insn.operands[3] == 0) else 4
    return _SPILL_SLOT_LOAD_WIDTH.get(op, 4)


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

def _ra_field_is_register(
    r_insn: Instruction,
    d_insn: Instruction,
) -> bool:
    """Value-dependent RA rule (doc 32 A2 rev 5): the RA field of an
    RA-literal opcode is the literal zero ONLY when its value is 0.  A nonzero
    RA is a real register read (semantics: ``ra == 0 if ... else
    state.gpr[ra]``) and is a renameable register pair.  The field is a
    literal (bit-equal, excluded from rho) iff EITHER side is 0 — a literal
    can never rename with a register.
    """
    if r_insn.opcode not in _RA_LITERAL_OPCODES:
        return False
    r_ra = (r_insn.raw >> 16) & 0x1F
    d_ra = (d_insn.raw >> 16) & 0x1F
    return r_ra != 0 and d_ra != 0


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
    if opcode in (Opcode.FMULS, Opcode.FMUL):
        # A-form with a RESERVED FB field: fmuls/fmul frD,frA,frC encode
        # frC at bits 21-25 MSB (LSB 10-6) and bits 16-20 MSB (LSB 15-11)
        # are reserved-zero (the decoder rejects FMULS with fb!=0; FMUL's
        # fb is reserved the same way).  Treating the always-zero FB as a
        # renameable register poisoned the rho with an f0=0 mapping and
        # broke otherwise-consistent swaps (func_800B06A4 / us-800b0f70).
        return (_FD, _FA, _FC)
    if opcode in (
        Opcode.FDIVS, Opcode.FSUBS, Opcode.FADDS, Opcode.FRES,
        Opcode.FMSUBS, Opcode.FMADDS, Opcode.FNMSUBS,
        Opcode.FNMADDS, Opcode.FDIV, Opcode.FSUB, Opcode.FADD,
        Opcode.FSEL, Opcode.FRSQRTE, Opcode.FMSUB,
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
    if opcode in (Opcode.FMULS, Opcode.FMUL):
        # fmuls/fmul read fa (LSB 20-16) and fc (LSB 10-6) only; the
        # reserved FB field (LSB 15-11) is never an input — semantics
        # compute fa*fc and the decoder rejects FMULS with fb!=0.
        return ((1, FPR), (3, FPR)), ((0, FPR),)
    if opcode in (
        Opcode.FDIVS, Opcode.FSUBS, Opcode.FADDS, Opcode.FRES,
        Opcode.FMSUBS, Opcode.FMADDS, Opcode.FNMSUBS,
        Opcode.FNMADDS, Opcode.FDIV, Opcode.FSUB, Opcode.FADD,
        Opcode.FSEL, Opcode.FRSQRTE, Opcode.FMSUB,
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


def _call_target(insn: Instruction) -> int | str | None:
    """The callee key for a call/tail-call slot, or None for indirect calls
    (bctrl/blrl) where the target is unknown — those fall back to the opaque
    EABI envelope."""
    if insn.relocation is not None:
        return insn.relocation.canonical_symbol
    if insn.opcode == Opcode.B:
        return insn.operands[0]
    if insn.opcode == Opcode.BC and len(insn.operands) > 2:
        return insn.operands[2]
    return None


def _is_tail_call(insn: Instruction, by_index: dict[int, int], end_pc: int) -> bool:
    """True when a non-link branch is a tail call: the branch leaves the
    function to a callee the witness cannot see.  Non-link branches WITH a
    relocation (the normal ``b <sym>`` tail-call form) are always tail calls;
    relocation-less branches are tail calls only when the static target is
    out-of-function (absolute form).  In-function gotos and branches to the
    function end (fallthrough exit) are control flow, not tail calls."""
    if insn.link or insn.opcode not in (Opcode.B, Opcode.BC):
        return False
    if insn.relocation is not None:
        return True
    if insn.opcode == Opcode.B:
        target = insn.operands[0]
    elif len(insn.operands) > 2:
        target = insn.operands[2]
    else:
        return False
    return target not in by_index and target != end_pc


def _call_observed_lanes(
    instructions: list[Instruction],
    callee_contracts: dict[int | str, Any],
) -> frozenset[int]:
    """Combined-numbering lanes a callee may READ at any call site (round-3
    review BLOCKER: outgoing-argument false certificates).

    A call (link ``b``/``bc``/``bctrl``/``blrl``) or a tail call (non-link
    ``b``/``bc`` to an out-of-function target) hands EABI arguments to a
    callee the proof cannot see.  A rho that renames a lane the callee READS
    is observable — the physical callee reads the physical lane — and the F3
    token canonicalization (semantics.py:4606) would hide the divergence
    from the structural comparison.  Precise contracts declare the reads;
    opaque ``*`` contracts and unknown callees conservatively fix EVERY lane
    (round-3 review GLM B1: EABI constrains preservation, not reads — a
    callee may read volatile f0/f9–f13 or any nonvolatile it preserves).
    r1/r2/r13 are unconditionally fixed anyway and memory is shared verbatim.
    """
    observed: set[int] = set()
    by_index = {insn.address: i for i, insn in enumerate(instructions)}
    end_pc = instructions[-1].address + 4 if instructions else 0
    for insn in instructions:
        if not (_is_call(insn) or _is_tail_call(insn, by_index, end_pc)):
            continue
        target = _call_target(insn)
        contract = (
            (callee_contracts or {}).get(target)
            if target is not None else None
        )
        reads = getattr(contract, "reads", None)
        if contract is None or reads is None or "*" in reads:
            # GLM-5.2 round-3 BLOCKER (B1): an opaque/unknown callee may READ
            # ANY lane — EABI constrains preservation, not reads (a callee may
            # read volatile f0/f9–f13 or a nonvolatile it preserves).  The F3
            # token canonicalization (semantics.py:4606) would hide a permuted
            # lane from the structural comparison, so every lane must be
            # rho-fixed at a call to an opaque callee.  The earlier arg-window
            # model (r3–r10/f1–f8) plus the r11/r12 patch (502e50099) was
            # GPR-incomplete; the sound rule is the original F3 design: for
            # ``*`` in reads, fix every lane.
            observed.update(range(_PS1_OFFSET + 32))
            continue
        for name in reads:
            base = name[:-4] if name.endswith(".ps1") else name
            if base.startswith("r") and base[1:].isdigit():
                observed.add(int(base[1:]))
            elif base.startswith("f") and base[1:].isdigit():
                n = int(base[1:])
                observed.add(32 + n)
                if name.endswith(".ps1"):
                    observed.add(_PS1_OFFSET + n)
    return frozenset(observed)


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


def _has_indirect_dispatch(instructions: list[Instruction]) -> bool:
    """True when the stream contains an indirect dispatch (``bcctr``) or a
    non-return indirect call (``bclr`` with ``link=True``).  Jump-table
    switches compile to ``bcctr``.  The region-sliced witness rejects these
    (doc 33 Item 2): the executor records an ``indirect-branch`` terminal with
    a symbolic CTR that would self-agree under the shared-state binding, so
    certifying through them would need jump-table target modeling — keeping
    the reject is a soundness-preserving scope choice, not a fail-closed
    fallback (dispatch modeling is doc-28/30 work).
    """
    for insn in instructions:
        op = insn.opcode
        if op == Opcode.BCCTR:
            return True  # indirect branch / jump-table dispatch
        if op == Opcode.BCLR and insn.link:
            return True  # blrl — non-return indirect call
    return False


def _has_unmodeled_absolute_branch(instructions: list[Instruction]) -> bool:
    """True when a non-link branch targets an out-of-function address without
    a relocation (round-3 review BLOCKER: the absolute tail-call form).  The
    executor records a ``direct-branch`` terminal for these instead of
    failing closed, so the witness would compare states at the unmodeled
    callee boundary — with ZERO callee contracts required — and could certify
    a permuted-argument pair.  In-function gotos and branches to the function
    end (fallthrough exit) are control flow, not tail calls."""
    if not instructions:
        return False
    by_index = {insn.address: i for i, insn in enumerate(instructions)}
    end_pc = instructions[-1].address + 4
    for insn in instructions:
        if insn.link or insn.opcode not in (Opcode.B, Opcode.BC):
            continue
        if insn.relocation is not None:
            continue
        if insn.opcode == Opcode.B:
            target = insn.operands[0]
        elif len(insn.operands) > 2:
            target = insn.operands[2]
        else:
            continue
        if target not in by_index and target != end_pc:
            return True
    return False


def _has_direct_backward_branch(
    instructions: list[Instruction],
    *,
    local_symbol: str | None = None,
) -> bool:
    """True when the stream contains a direct backward branch (loop) or
    direct self-recursion (tail call to the local function).  Return-position
    ``bclr`` (link=False) is a terminal, not a loop marker.  A relocated
    branch is a call unless its symbol is the local function (then it is a
    real back-edge).
    """
    by_address = {insn.address: i for i, insn in enumerate(instructions)}
    for i, insn in enumerate(instructions):
        op = insn.opcode
        if op == Opcode.B:
            target = insn.operands[0]
            if insn.relocation is not None:
                # Relocated branch: a call unless it is direct recursion.
                # Non-link relocated self-branch (tail recursion) is ALSO a
                # real back-edge (impl-review MINOR 5).
                if insn.relocation.canonical_symbol == local_symbol:
                    return True
                continue
            if target <= insn.address and target in by_address:
                return True
        elif op == Opcode.BC:
            if insn.link:
                continue  # conditional call
            if insn.relocation is not None:
                continue
            target = insn.operands[2]
            if target <= insn.address and target in by_address:
                return True
    return False


def _loop_spans(
    instructions: list[Instruction],
    *,
    local_symbol: str | None = None,
) -> list[tuple[int, int]]:
    """Return ``(target_addr, branch_addr)`` spans of backward direct
    branches / direct self-recursion (doc 33 Item 2 loop-boundary guard).

    Only **in-function** targets count — a backward ``b`` to an out-of-function
    address is a terminal (``_cfg_successors`` returns ``()``), not a loop.
    A direct self-recursion (relocated non-link branch to the local symbol)
    spans the whole function ``[start, insn]``: a boundary anywhere inside it
    would resume the recursion under a different region rho.
    """
    by_address = {insn.address: i for i, insn in enumerate(instructions)}
    func_start = instructions[0].address if instructions else 0
    spans: list[tuple[int, int]] = []
    for insn in instructions:
        op = insn.opcode
        if op == Opcode.B:
            if insn.relocation is not None:
                if insn.relocation.canonical_symbol == local_symbol:
                    spans.append((func_start, insn.address))
                continue
            target = insn.operands[0]
            if target <= insn.address and target in by_address:
                spans.append((target, insn.address))
        elif op == Opcode.BC and not insn.link and insn.relocation is None:
            target = insn.operands[2]
            if target <= insn.address and target in by_address:
                spans.append((target, insn.address))
    return spans


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

    Doc 33 Item 2 splits this into ``_has_indirect_dispatch`` +
    ``_has_direct_backward_branch`` (the region path now executes direct
    backward loops bounded and rejects only indirect dispatch + boundaries
    inside loop spans).  This combined predicate is kept for the pinned
    first-cut tests and callers that want the union.  Note: since Item 2,
    an out-of-function backward ``b`` is NOT flagged (it is a terminal per
    ``_cfg_successors``, not a loop — only in-function targets count).
    """
    return _has_indirect_dispatch(instructions) or _has_direct_backward_branch(
        instructions, local_symbol=local_symbol,
    )


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
    # R9-4 (option B): True when the certified pair relied on the F1 spill
    # carve-out (a live-in lane was spilled and the rho permutes it).  The
    # certificate must then declare the no-stack-slot-aliasing scoped
    # assumption (H5: computed-pointer reads of the save slot are excluded).
    spill_carveout_used: bool = False
    details: dict[str, Any] = field(default_factory=dict)


def _fail_rho(
    table: dict[int, int],
    rv: int,
    dv: int,
    index: int,
    kind: str,
) -> WitnessFailure | None:
    """Accumulate one (retail rv -> decomp dv) mapping into a partial rho
    table; return the failure when it breaks single-valuedness/injectivity.
    Shared by gate 4 and the value-dependent RA rule (doc 32 A2 rev 5).
    """
    if rv in table:
        if table[rv] != dv:
            return WitnessFailure(
                "rho",
                f"slot {index}: {kind} r{rv} maps to both "
                f"r{table[rv]} and r{dv}",
            )
        return None
    if dv in table.values():
        other = next(k for k, v in table.items() if v == dv)
        return WitnessFailure(
            "rho",
            f"slot {index}: {kind} r{rv} and r{other} "
            f"both map to r{dv}",
        )
    table[rv] = dv
    return None


def _first_rho_failure(
    r_insn: Instruction,
    d_insn: Instruction,
    rho_gpr: dict[int, int],
    rho_fpr: dict[int, int],
    index: int,
) -> WitnessFailure:
    """Report the first positional rho conflict for an instruction pair
    (used after both the normal and commutative-swapped orders fail)."""
    for start, kind in _register_fields(r_insn.opcode):
        rv = (r_insn.raw >> start) & 0x1F
        dv = (d_insn.raw >> start) & 0x1F
        table = rho_gpr if kind == GPR else rho_fpr
        if rv in table and table[rv] != dv:
            return WitnessFailure(
                "rho",
                f"slot {index}: {kind} r{rv} maps to both "
                f"r{table[rv]} and r{dv}",
            )
        if dv in table.values():
            other = next(k for k, v in table.items() if v == dv)
            return WitnessFailure(
                "rho",
                f"slot {index}: {kind} r{rv} and r{other} "
                f"both map to r{dv}",
            )
    return WitnessFailure("rho", f"slot {index}: no consistent bijection")
    return None


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
        # with a matched relocation exempt ONLY the relocated address-field
        # bits (the placeholder that resolves to the same canonical symbol on
        # both sides; gate 2 binds identity).  Every OTHER non-register bit —
        # LK/AA on B/BC, BO/BI on BC, opcode/XO/Rc, SPR indices, immediates —
        # must still be bit-equal (round-8 review R8-2: the previous blanket
        # exemption skipped the whole comparison for relocated slots, leaving
        # e.g. the LK bit of a relocated ``b`` vs ``bl`` pair — same
        # ``Opcode.B`` — uncompared and relying solely on the downstream
        # terminal comparison to catch it).
        gpr_mask, fpr_mask = _gpr_fpr_masks(r_insn.opcode)
        register_mask = gpr_mask | fpr_mask
        # Value-dependent RA (doc 32 A2 rev 5): a both-nonzero RA pair on an
        # RA-literal opcode is a real register rename — exclude its bits from
        # the non-register bit-equality; gate 4 accumulates it into rho.
        ra_rename_mask = (
            0x1F << 16 if _ra_field_is_register(r_insn, d_insn) else 0
        )
        # Address-field masks per relocation type (mirrors the decoder's
        # fixup masks, decoder.py:589-629): REL24 fixes bits 6-29 (LI), REL14
        # bits 16-29 (BD), ADDR16_* the low half-word bits 16-31, EMB_SDA21
        # bits 11-31.  Unknown types default to 0 (full comparison — stricter).
        reloc_field_mask = {
            R_PPC_REL24: 0x03FFFFFC,
            R_PPC_REL14: 0x0000FFFC,
            R_PPC_ADDR16_LO: 0x0000FFFF,
            R_PPC_ADDR16_HI: 0x0000FFFF,
            R_PPC_ADDR16_HA: 0x0000FFFF,
            R_PPC_EMB_SDA21: 0x001FFFFF,
        }.get(r_reloc.relocation_type, 0) if r_reloc is not None else 0
        non_register_diff = (
            (r_insn.raw ^ d_insn.raw)
            & ~(register_mask | ra_rename_mask | reloc_field_mask)
        )
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

    # Symmetric-compare extension (vf9, us-8017e6b8): CR fields whose
    # LT/GT/UN bits are never consumed may have their FCMPU/FCMPO operands
    # swapped without semantic change (EQ bit is symmetric; volatile fields
    # are call-clobbered so exit remnants are unobservable).
    non_eq_cr = (_cr_field_non_eq_reads(original)
                 | _cr_field_non_eq_reads(candidate))
    import os as _os
    _dbg = _os.environ.get('WITNESS_DEBUG')
    if _dbg:
        print(f"[witness-debug] non_eq_cr={sorted(non_eq_cr)}")
    import os as _os
    if _os.environ.get('WITNESS_DEBUG'):
        print(f"[witness-debug] non_eq_cr={sorted(non_eq_cr)}")
        for i, (ri, di) in enumerate(zip(original, candidate)):
            if ri.opcode in (Opcode.FCMPU, Opcode.FCMPO):
                crf = (ri.raw >> 6) & 0x7
                print(f"[witness-debug] slot {i}: fcmpu crfD={crf} "
                      f"order-insensitive={crf not in non_eq_cr}")

    for index, (r_insn, d_insn) in enumerate(zip(original, candidate)):
        # Gate 4: rho accumulation — single-valued and injective, consistent
        # across all mnemonics/positions. For commutative X-form ops, a
        # positional conflict (retail `add rA,rB,rC` vs decomp `add rA,rC,rB`)
        # is retried with the decomp RA/RB fields swapped — the computed value
        # is identical (us-8025658c / us-8025650c were stuck at 99.4-99.6%
        # on exactly this, hexdiff called it "2 pure reg-swaps" but the
        # witness rejected the pair at the rho gate).
        def _with(swap_ra_rb: bool):
            trial_r, trial_f = dict(rho_gpr), dict(rho_fpr)
            d_raw = d_insn.raw
            if swap_ra_rb:
                ra = (d_raw >> 16) & 0x1F  # old RA (bits 16-20)
                rb = (d_raw >> 11) & 0x1F  # old RB (bits 11-15)
                d_raw = (d_raw & ~(0x1F << 16)) | (rb << 16)
                d_raw = (d_raw & ~(0x1F << 11)) | (ra << 11)
            for start, kind in _register_fields(r_insn.opcode):
                rv = (r_insn.raw >> start) & 0x1F
                dv = (d_raw >> start) & 0x1F
                failure = _fail_rho(
                    trial_r if kind == GPR else trial_f, rv, dv, index, kind,
                )
                if failure is not None:
                    return None
            return trial_r, trial_f

        applied = _with(False)
        if (
            r_insn.opcode in (Opcode.FCMPU, Opcode.FCMPO)
            and d_insn.opcode == r_insn.opcode
        ):
            crf_d = (r_insn.raw >> 6) & 0x7
            safe = _swapped_compare_order_safe(original, index, crf_d)
            safe_c = _swapped_compare_order_safe(candidate, index, crf_d)
            if _dbg and not (safe and safe_c):
                for k in range(index + 1, min(index + 40, len(original))):
                    w = _writes_cr_field(original[k], crf_d)
                    r = _reads_cr_non_eq(original[k], crf_d)
                    if w or r:
                        print(f"[witness-debug]   +{k}: W={w} Rne={r} "
                              f"raw={original[k].raw:08x}")
                print(f"[witness-debug]   kill-safe(orig)={safe}")
        _symmetric_cmp = (
            r_insn.opcode in (Opcode.FCMPU, Opcode.FCMPO)
            and d_insn.opcode == r_insn.opcode
            and (((r_insn.raw >> 6) & 0x7) not in non_eq_cr
                 or _swapped_compare_order_safe(
                     original, index, (r_insn.raw >> 6) & 0x7,
                 ))
        )
        if applied is None and (
            r_insn.opcode in _COMMUTATIVE_RA_RB or _symmetric_cmp
        ):
            # Commutative operand-order swap (retail `add rA,rB,rC` vs decomp
            # `add rA,rC,rB`): retry with the decomp RA (16-20) <-> RB (11-15)
            # exchanged — the computed value is identical.
            applied = _with(True)
        if applied is None:
            # Both orders conflicted — report the first positional failure.
            return WitnessOutcome(False, failure=_first_rho_failure(
                r_insn, d_insn, rho_gpr, rho_fpr, index,
            ))
        rho_gpr, rho_fpr = applied
        # Value-dependent RA (doc 32 A2 rev 5): both-nonzero RA pair on an
        # RA-literal opcode is a real register rename.
        if _ra_field_is_register(r_insn, d_insn):
            rv = (r_insn.raw >> 16) & 0x1F
            dv = (d_insn.raw >> 16) & 0x1F
            failure = _fail_rho(rho_gpr, rv, dv, index, GPR)
            if failure is not None:
                return WitnessOutcome(False, failure=failure)

    rho = Rho(gpr=rho_gpr, fpr=rho_fpr)

    # Gate 5: ABI-boundary fixedness.
    carveout_used: list[bool] = []
    failure = _check_abi_fixedness(original, candidate, rho, declared_return,
                              callee_contracts, carveout_out=carveout_used)
    if failure is not None:
        return WitnessOutcome(False, rho=rho, failure=failure)
    # A1 post-rho belt-and-suspenders (global path).
    failure = _psq_operands_rho_fixed(original, rho)
    if failure is not None:
        return WitnessOutcome(False, rho=rho, failure=failure)

    return WitnessOutcome(
        True, rho=rho,
        spill_carveout_used=bool(carveout_used and carveout_used[0]),
    )


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
    carveout_out: list[bool] | None = None,
) -> WitnessFailure | None:
    # LR/CTR are inherently fixed (SPR indices are non-register bit-equal
    # fields); only GPR/FPR entries are checked here.
    fixed_gpr = set(_UNCONDITIONALLY_FIXED_GPRS)
    fixed_fpr = set(_UNCONDITIONALLY_FIXED_FPRS)
    # R9-4 (option B): track whether the F1 spill carve-out was actually
    # LOAD-BEARING — a live-in lane exempted as spill-only that the final rho
    # permutes.  When true, the certificate must declare the
    # no-stack-slot-aliasing scoped assumption (the H5 computed-pointer
    # boundary).  Identity-mapped spill lanes do not engage the fiction and
    # need no assumption.
    spill_exempted: set[int] = set()  # combined-numbering lanes exempted
    gpr_perm = rho.gpr_perm()
    fpr_perm = rho.fpr_perm()
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
    # Registers read before being written (live-in at entry) are the
    # function's input signature and must be fixed.  F1 (adversarial review
    # 2026-08): this is now EVERY live-in lane, not just the EABI argument
    # ranges — a live-in r11/r12/r14–r31/f0/f9–f31/r0-in-a-genuine-operand
    # is an input the byte-identical caller placed in a physical lane, and
    # permuting it assumes the caller renamed its registers (false for
    # live-in values; F1a/F1b/H1/H2/H5).  Carve-out: a SPILL-ONLY live-in
    # lane (entry value consumed exclusively by a prologue stack save
    # ``stw rN, c(r1)`` / ``stfd fN, c(r1)`` before its first def) is not an
    # input dependency — both sides spill the same shared variable to the
    # same slot and restore it, so the perm stays sound and the Chaitin
    # prologue-save class survives.  The two sides are position-aligned with
    # identical control flow, so a union of both sides' live-in sets is the
    # conservative choice.
    for instructions in (original, candidate):
        live_in, live_across = _liveness_sets(instructions)
        # Combined numbering (GPR r -> r, FPR f -> 32 + f) so r20 and f20 do
        # not collide in the exemption set.
        spill_exempted.update(
            n for n in live_in if n < 32
            and _live_in_spill_only(instructions, n, GPR, callee_contracts)
        )
        spill_exempted.update(
            n for n in live_in
            if 32 <= n < _PS1_OFFSET
            and _live_in_spill_only(instructions, n - 32, FPR, callee_contracts)
        )
        fixed_gpr.update(
            n for n in live_in
            if n < 32 and n not in spill_exempted
        )
        fixed_fpr.update(
            n - 32 for n in live_in
            if 32 <= n < _PS1_OFFSET and n not in spill_exempted
        )
        # PS1 sub-lanes are bound together with their owning FPR; a live-in
        # ps1 lane fixes the FPR too (no spill carve-out: ps1 saves are not
        # modeled).
        fixed_fpr.update(n - _PS1_OFFSET for n in live_in if n >= _PS1_OFFSET)
        fixed_gpr.update(n for n in live_across if n < 32)
        fixed_fpr.update(n - 32 for n in live_across if 32 <= n < _PS1_OFFSET)
    # Round-3 review BLOCKER (outgoing-argument false certificates, GLM-5.2
    # CX-A / Kimi escapes 1-5): a callee READS its argument lanes at every
    # call/tail-call site.  A rho that renames a lane the callee observes is
    # observable — the physical callee reads the physical lane — and the F3
    # token canonicalization (semantics.py:4606) would hide the divergence
    # from the structural comparison.  Precise contracts declare the reads;
    # opaque ``*`` contracts and unknown callees fix every lane (GLM B1).
    # This subsumes the
    # r4/f1 tail-call cases in the A3 block above (the gate can only ever
    # fix, never unfix) and enforces the doc-31 §2.5 call model for every
    # call form.  Per-function, NOT per-region: a lane observed at any call
    # site is fixed in every region's rho (over-fixing is sound).
    for instructions in (original, candidate):
        for lane in _call_observed_lanes(instructions, callee_contracts):
            if lane < 32:
                fixed_gpr.add(lane)
            elif lane < _PS1_OFFSET:
                fixed_fpr.add(lane - 32)
            else:
                fixed_fpr.add(lane - _PS1_OFFSET)
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
    # R9-4b F4 (fourth review round): the carve-out flag write must NOT be
    # nested inside the fixed_fpr loop — when fixed_fpr is empty (a trusted
    # declared_return skips the conditional f1 fix, and there are no live-in /
    # live-across / call-observed FPRs), the write never executed and the
    # no-stack-slot-aliasing declaration was silently dropped from the
    # certificate for a load-bearing GPR carve-out.  Now at function end.
    if carveout_out is not None:
        # R9-4 (option B): the carve-out is LOAD-BEARING iff a spill-exempted
        # live-in lane is actually permuted by the final rho (identity-mapped
        # spill lanes don't engage the caller-renaming fiction).  The caller
        # passes a fresh list per call site; the REGION path calls this per
        # region, so the out-param is a per-call snapshot (the caller ORs
        # across regions).
        carveout_out[:] = [
            any(
                lane < 32
                and lane in spill_exempted and gpr_perm[lane] != lane
                or lane >= 32 and lane < _PS1_OFFSET
                and lane in spill_exempted and fpr_perm[lane - 32] != lane - 32
                for lane in spill_exempted
            )
        ]
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


# ── Deterministic simplify budget (B1 node-cap + B2 memoization; timeout backstop) ─
# The terminal comparison and the preservation check re-run ``z3.simplify`` over
# shared-AST lanes up to |retail_exits| x |decomp_exits| x 96 times.  simplify is
# a pure function of the AST (and z3 ASTs are hash-consed: equal expressions
# share an ``ast_id``), so identical ASTs may share ONE cached result (B2) with
# zero soundness cost and it collapses the repeated work on branchy/multi-exit
# functions.  The deterministic primary bound is ``node_cap`` (B1): any AST over
# the cap is left UNSIMPLIFIED and compared by raw ``z3.eq`` (fail-closed — raw
# eq is already tried first, so an over-cap lane that needed canonicalization
# degrades to SMT, never a false certificate).  A wall-clock timeout is only a
# secondary lateness backstop (non-deterministic); the callers clamp it to
# ``>=1`` whenever a deadline is active so it is never silently disabled by
# ``deadline.remaining_ms()`` returning 0 at exhaustion.
_SIMPLIFY_NODE_CAP = 4096
_SIMPLIFY_CACHE_LIMIT = 20_000

# Bounded disjointness SAT fallback (2026-08): the cheap simplify is a local
# rewrite and can leave a structural contradiction unreduced — e.g. the cross
# pairs of a branchy function build ``And(X, Not(X))`` from per-side byte-read
# memory terms whose equalities are not structurally identical, so
# ``is_false(simplify(And(c0, c1)))`` returns False even though the two path
# conditions can never co-occur.  A bounded QF_BV satisfiability check is
# complete for this structure: ``unsat`` proves the pair is genuinely
# disjoint, so skipping it is correct.  Fail-closed on sat/unknown/timeout
# (the pair is compared as before).
_DISJOINT_SAT_TIMEOUT_MS = 10_000


@dataclass(frozen=False)
class _SimplifyBudget:
    """Per-witness-run simplify cache + deterministic node cap + timeout.

    ``cache`` maps ``ast_id -> simplified expr``; ``calls`` / ``node_skips``
    are diagnostics (a test asserts memoization fires).  ``timeout_ms<=0``
    means no timeout; callers set it via ``_simplify_timeout`` which clamps to
    ``>=1`` whenever a deadline is active.
    """
    node_cap: int = _SIMPLIFY_NODE_CAP
    timeout_ms: int = 0
    cache: dict[int, Any] = field(default_factory=dict)
    calls: int = 0
    node_skips: int = 0


def _simplify_timeout(deadline: Deadline | None) -> int:
    """Return the simplify timeout for a run.

    ``0`` (none) only when there is no deadline; ``>=1`` ms when a deadline is
    active — even at ``remaining_ms()==0`` — so the timeout is never silently
    disabled (a 0 would mean "no timeout", re-opening the run30 spin exactly
    when the budget is exhausted).
    """
    if deadline is None:
        return 0
    return max(1, deadline.remaining_ms())


def _path_conditions_disjoint_sat(
    combined: Any, z3: Any, deadline: Deadline | None,
) -> bool:
    """Bounded QF_BV satisfiability fallback for the terminal-pair
    disjointness check.

    ``True`` iff ``combined`` (the conjunction of the retail and decomp path
    conditions) is unsatisfiable — i.e. the two paths can never co-occur, so
    skipping the terminal pair is correct.  Sound: a solver ``unsat`` result
    for the bitvector formula is a proof of infeasibility, so skipping is
    never a false certificate.  Fail-closed: ``sat``/``unknown``/timeout/
    exception return ``False`` and the pair is compared as before (which can
    only reject, never accept).  Bounded by a fixed wall-clock timeout so a
    pathological condition cannot spin the witness.
    """
    if deadline is not None and deadline.expired():
        return False
    try:
        solver = z3.Solver()
        solver.set(timeout=_DISJOINT_SAT_TIMEOUT_MS)
        solver.add(combined)
        return solver.check() == z3.unsat
    except Exception:
        return False


def _ast_node_count(node: Any, z3: Any, cap: int) -> int:
    """Bounded DFS over the AST node count; stops as soon as ``cap`` is
    exceeded and returns ``cap+1`` for any larger AST (deterministic)."""
    stack = [node]
    count = 0
    while stack and count <= cap:
        top = stack.pop()
        if top is None:
            continue
        count += 1
        try:
            for i in range(top.num_args()):
                stack.append(top.arg(i))
        except Exception:
            pass
    return count


def _z3_simplify(expr: Any, z3: Any, budget: _SimplifyBudget) -> Any:
    """Deterministic-bounded, memoized, optionally-timed simplify.

    Soundness contract: the ONLY consumer of the result that can ACCEPT a pair
    is structural ``z3.eq`` (AST identity), and every return value here is a
    semantics-preserving rewrite of ``expr`` (the unsimplified original on a
    node-cap skip, or z3.simplify's output).  An accept is therefore always
    sound; every other return is fail-closed (the witness falls through to
    SMT).  Requires a non-None ``budget``; callers thread one on hot paths and
    fall back to plain ``z3.simplify`` otherwise.
    """
    try:
        key = expr.get_id() if hasattr(expr, "get_id") else expr.ast_id()
    except Exception:
        return expr
    hit = budget.cache.get(key)
    if hit is not None:
        return hit[1]
    budget.calls += 1
    if (
        budget.node_cap > 0
        and _ast_node_count(expr, z3, budget.node_cap) > budget.node_cap
    ):
        budget.node_skips += 1
        # Pin the INPUT expr in the cache entry so the Z3 ast_id cannot be
        # recycled (z3 ids are "unique up to reference counting" — a freed
        # input's id is reused by a later expression, and a stale cached
        # result under that id would be a fail-open).  See E1 (adversarial
        # review 2026-08): transient inputs like the per-pair `And` or the
        # `prev - node.arg(1)` word-group delta are dropped every iteration.
        budget.cache[key] = (expr, expr)
        return expr
    try:
        if budget.timeout_ms > 0:
            out = z3.simplify(expr, timeout=budget.timeout_ms)
        else:
            out = z3.simplify(expr)
    except Exception:
        out = expr
    if len(budget.cache) >= _SIMPLIFY_CACHE_LIMIT and budget.cache:
        budget.cache.pop(next(iter(budget.cache)))
    # Pin the input alongside the output (see above): keeps the id valid for
    # the cache lifetime and makes stale-id reuse impossible.
    budget.cache[key] = (expr, out)
    return out


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


def _structurally_equal_simplified(
    left: Any, right: Any, z3: Any, *, timeout_ms: int = 0,
    budget: _SimplifyBudget | None = None,
) -> bool:
    """Structural equality with commutative canonicalization.

    Raw ``z3.eq(a+b, b+a)`` is False — z3 normalizes commutativity only under
    ``simplify``.  A pure commutative operand-order swap (retail `add rA,rB,rC`
    vs decomp `add rA,rC,rB`) is value-identical, and the rho fix already lets
    it pass the gate; the terminal compare must not then re-reject it
    (us-8025658c / us-8025650c, r8 WS-1).  ``z3.simplify`` is semantics-
    preserving, so this is strictly more permissive on provably-equal
    expressions and never accepts an unequal pair.

    ``timeout_ms`` (0 = none) is passed to ``z3.simplify`` as a Z3 param —
    the rewriter honors it internally, so even a single stuck ``simplify`` on
    a pathological AST is interrupted instead of spinning (run30 incident:
    one lane held the build lock ~30 min at 99.7% CPU in th_rewriter).  When a
    ``budget`` is provided it carries the timeout, the deterministic node cap
    and a memoized cache, superseding ``timeout_ms``."""
    if _structurally_equal(left, right, z3):
        return True
    try:
        if budget is not None:
            return bool(z3.eq(_z3_simplify(left, z3, budget),
                              _z3_simplify(right, z3, budget)))
        if timeout_ms > 0:
            return bool(z3.eq(z3.simplify(left, timeout=timeout_ms),
                              z3.simplify(right, timeout=timeout_ms)))
        return bool(z3.eq(z3.simplify(left), z3.simplify(right)))
    except Exception:
        return False


def _constant_int(
    expr: Any, z3: Any, *, budget: _SimplifyBudget | None = None,
) -> int | None:
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
        simplified = _z3_simplify(expr, z3, budget) if budget is not None else z3.simplify(expr)
        if z3.is_bv_value(simplified):
            return simplified.as_long()
    except Exception:
        pass
    return None


def _value_equal(
    left: Any, right: Any, z3: Any, *,
    left_base: int, right_base: int,
    budget: _SimplifyBudget | None = None,
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
    left_const = _constant_int(left, z3, budget=budget)
    right_const = _constant_int(right, z3, budget=budget)
    if left_const is not None and right_const is not None:
        if left_const == right_const:
            return True
        return (left_const - left_base) == (right_const - right_base)
    return _structurally_equal(left, right, z3)


def _live_in_spill_only(
    instructions: list[Instruction],
    lane: int,
    kind: str,
    callee_contracts: dict[int | str, Any] | None = None,
) -> bool:
    """True when ``lane``'s live-in value is used ONLY by a prologue stack
    save (F1 carve-out, adversarial review 2026-08; fixed for N1 in round 2
    2026-08-05; round-9 R9-1 escape confinement).

    ``stw rN, c(r1)`` / ``stfd fN, c(r1)`` with a constant offset and the
    frame pointer r1 is the EABI prologue-save pattern: the entry value flows
    only into the spill slot (position-aligned on both sides — the same
    shared slot holds the same shared variable), so permuting the lane is
    sound and the Chaitin prologue-save class survives.  ANY other use of the
    live-in value (data computation, call argument, non-r1 store) makes it a
    real input dependency → not spill-only → the lane is fixed.

    The entry value's reachability is computed over the REAL CFG (forward
    dataflow: ``entry_reaches[i]`` = can the entry value reach instruction
    ``i`` without an intervening def), NOT a stream-order first-def scan — a
    live-in read reachable via a forward branch that skips a stream-order
    earlier def is a genuine input dependency (N1, round-2 review: a
    ``beq``-skipped ``li r20,7`` left the taken path reading the caller's
    r20 as data while the carve-out called it spill-only).  A def that also
    USES the lane (e.g. ``rlwimi``) observes the entry value at that slot and
    is therefore an input dependency unless it is itself a spill store.

    Round-9 review (R9-1, Kimi K3 probe A; independently reproduced): the
    pre-round-9 carve-out checked only what flows INTO the lane's entry value
    and never what flows OUT of the spill slot.  Under rho ``{20:25}`` both
    sides store the SAME shared variable X_20 to the same slot, so the slot
    self-agrees structurally — but the PHYSICAL slot content differs (caller
    r20 vs caller r25).  Any later consumer of that slot content (reload into
    a different lane, data use of a reloaded value, store to a global, a
    callee that reads memory while the slot holds the entry value) exposes
    the divergence while the structural comparison still self-agrees — a
    false certificate.  ``stw r20,8(r1); lwz r5,8(r1); mr r3,r5; blr`` vs
    ``stw r25,8(r1); lwz r5,8(r1); mr r3,r5; blr`` CERTIFIED before the fix
    (retail returns the caller's r20, decomp the caller's r25).

    The carve-out is therefore granted only when the spilled entry value is
    CONFINED:

    - C1 (slot read confinement): every load from the save slot ``(r1+c)``
      must write the SAME lane ``lane`` — a reload into any other lane
      (identity or otherwise) makes the physical divergence caller-visible.
    - C2 (terminal restore): after a reload of the slot into ``lane``, the
      lane must not be READ again before its next def or the function exit —
      the restored value may not flow into computation, a global store, a
      call argument, or the return.
    - C3 (memory-observing callee): no call/tail-call whose callee contract
      READS MEMORY (opaque ``*``, unknown, or precise ``reads`` containing
      ``"memory"``) may execute while the slot holds the un-restored entry
      value — such a callee could observe the divergent physical slot
      content in place.

    All three are fail-closed: any violation makes the lane FIXED (rho must
    map it to itself) and the pair falls to SMT — never a false certificate.
    The accepted Chaitin controls (spill; use lane as scratch; restore to the
    SAME lane; restore is the lane's last touch; precise callee contracts
    that do not read memory) still certify.
    """
    n = len(instructions)
    if n == 0:
        return False
    by_index = {insn.address: i for i, insn in enumerate(instructions)}
    end_pc = instructions[-1].address + 4
    # ``_use_def_numbered`` uses the COMBINED numbering (GPR r -> r, FPR f ->
    # 32 + f, PS1 -> 64 + f); the caller passes the raw register number.
    nlane = lane if kind == GPR else _PS1_OFFSET - 32 + lane
    defs = [_use_def_numbered(insn)[1] for insn in instructions]
    uses = [_use_def_numbered(insn)[0] for insn in instructions]
    reaches = [False] * n
    reaches[0] = True
    changed = True
    while changed:
        changed = False
        for i in range(n):
            out = reaches[i] and nlane not in defs[i]
            if not out:
                continue
            for s in _cfg_successors(instructions, i, by_index, end_pc):
                if not reaches[s]:
                    reaches[s] = True
                    changed = True
    # Spill-slot displacements used by the lane's prologue save(s); used by
    # C1/C3 to identify slot reads and the un-restored-value hazard window.
    spill_disp: set[int] = set()
    for i in range(n):
        if reaches[i] and nlane in uses[i]:
            insn = instructions[i]
            op = insn.opcode
            if kind == GPR:
                # stw rN, c(r1) — the prologue save.  RA must be r1 and the
                # displacement a plain immediate (no relocation / symbolic
                # base).  Update forms (stwu) are REJECTED as saves (R9-4
                # N9/N9b): the r1 update moves the slot, so its post-update
                # address never matches spill_disp and C1/C2 would be blind.
                if op != Opcode.STW:
                    return False
                if len(insn.operands) < 2 or insn.operands[0] != lane:
                    return False
                if insn.operands[1] != 1 or insn.relocation is not None:
                    return False
            else:
                # stfd fN, c(r1).  Update forms (stfdu) rejected (R9-4 N9).
                if op != Opcode.STFD:
                    return False
                if len(insn.operands) < 2 or insn.operands[0] != lane:
                    return False
                if insn.operands[1] != 1 or insn.relocation is not None:
                    return False
            if len(insn.operands) >= 3:
                spill_disp.add(insn.operands[2])

    # ── C1/C2/C3: confine the spilled entry value (round-9 R9-1) ──────────
    # Forward CFG reachability from the spill stores: instructions that can
    # execute while the slot holds the entry value.  Everything that reads
    # the slot or runs while the slot holds the divergent content is inside
    # this window.
    after_spill = [False] * n
    for i in range(n):
        if reaches[i] and nlane in uses[i]:
            after_spill[i] = True
    changed = True
    while changed:
        changed = False
        for i in range(n):
            if not after_spill[i]:
                continue
            for s in _cfg_successors(instructions, i, by_index, end_pc):
                if not after_spill[s]:
                    after_spill[s] = True
                    changed = True

    # C0 (R9-4 H8 + R9-4b, r1-stationarity): the slot-read detection is
    # syntactic on ``(r1 + c)``, which assumes r1 is constant between the
    # save and every read.  If ANY instruction in the after_spill window
    # DEFINES r1 (ALU move, update-form load/store with rA==r1, the
    # frame-alloc stwu r1 in save-before-alloc order), the recorded
    # displacement may no longer match the physical slot — a compensating
    # ``lwz r5,72(r1)`` after ``addi r1,r1,-64`` reads the slot at 8 while C1
    # sees 72 disjoint (Kimi H8: certified caller-r20 vs caller-r25).
    # Reject the carve-out when an r1-relative memory access is reachable
    # from a prior r1 def.
    #
    # R9-4b fix (Kimi H-a, fifth round): the reachability is seeded at the
    # SUCCESSORS of each r1 def, NOT at the def site, and the def-site skip
    # is removed.  An UPDATE-FORM LOAD (lwzu/lbzu/lhzu/lhau/lfsu/lfdu/
    # psq_lu with rA==r1) is BOTH an r1 def AND a memory read whose EA uses
    # the PRE-update r1 — after a prior move, that EA hits the slot, so the
    # def site itself must be checked as the access (the old def-site skip
    # made it invisible).  A pure ALU ``addi r1,r1,-64`` is not a memory
    # access and is skipped by the is_mem test; the epilogue
    # ``addi r1,r1,N`` / ``lwz r1,0(r1)`` has no SUBSEQUENT r1-relative
    # access (successor-seeding reaches nothing) — the standard
    # prologue/restore/epilogue Chaitin shape survives.  (Kimi P6: a
    # blanket "no r1-def in window" would kill the whole class.)
    r1_defs = [i for i in range(n) if after_spill[i] and 1 in defs[i]]
    if r1_defs:
        r1_def_reaches = [False] * n
        for d in r1_defs:
            # Seed at SUCCESSORS: the def itself is not "after the move"
            # (its own EA uses the pre-update r1), but everything reachable
            # from it executes with the moved r1.
            for s in _cfg_successors(instructions, d, by_index, end_pc):
                r1_def_reaches[s] = True
        changed = True
        while changed:
            changed = False
            for i in range(n):
                if not r1_def_reaches[i]:
                    continue
                for s in _cfg_successors(instructions, i, by_index, end_pc):
                    if not r1_def_reaches[s]:
                        r1_def_reaches[s] = True
                        changed = True
        for i in range(n):
            if not r1_def_reaches[i]:
                continue
            insn = instructions[i]
            op = insn.opcode
            # An r1-relative memory access reachable from an r1 def: the slot
            # address may have moved.  The def site of an update-form LOAD is
            # itself such an access (its EA uses the pre-update r1, which a
            # prior move has desynchronized) — it must NOT be skipped.
            is_mem = (
                op in _SPILL_SLOT_LOADS_ALL
                or op in (Opcode.STW, Opcode.STFD)
                or op in (Opcode.STWU, Opcode.STFDU, Opcode.STWX, Opcode.STFDX)
                or op in (Opcode.PSQ_ST, Opcode.PSQ_STU)
                or op in (Opcode.STB, Opcode.STBU, Opcode.STH, Opcode.STHU)
            )
            if not is_mem:
                continue
            if len(insn.operands) >= 2 and insn.operands[1] == 1:
                return False

    # C1: any load from the save slot must write the SAME lane (same register
    # KIND and number).  R9-4 closes the syntactic gaps the first fix left:
    #   - byte-reversed X-form loads (lwbrx/lhbrx) and psq_l* are in the sets;
    #   - cross-kind reads (GPR slot via FPR load, or vice versa) are caught
    #     because the scan uses the UNION of both load sets and the lane
    #     comparison includes the register kind;
    #   - D-form displacement uses a BYTE-RANGE overlap test (a 2-byte lhz at
    #     disp+2 overlaps a 4-byte slot at disp; an 8-byte lfd at disp
    #     overlaps a 4-byte GPR slot and vice versa);
    #   - X-form loads test BOTH base registers (the effective address is
    #     gpr[rA] + gpr[rB], so rB == r1 with a constant rA also addresses
    #     the slot);
    #   - lmw (multi-lane) remains an outright reject.
    # The residual H5 class (a COMPUTED pointer into the slot, e.g.
    # ``addi r4,r1,8; lwz r5,0(r4)``) is not syntactically provable; it is
    # excluded by the DECLARED no-stack-slot-aliasing scoped assumption the
    # certificate records when the carve-out is used (option B).
    for i in range(n):
        insn = instructions[i]
        op = insn.opcode
        if op not in _SPILL_SLOT_LOADS_ALL:
            continue
        # Only loads reachable after a spill store can read the spilled value
        # (a load before the prologue save reads the caller's stack — not the
        # divergent slot content).
        if not after_spill[i]:
            continue
        if op == Opcode.LMW:
            # lmw rD, c(r1) loads rD..r31 — the slot value would flow into
            # MANY lanes; cannot confine.
            return False
        # D-form loads: (rD, rA, disp).  X-form: (rD, rA, rB) — either base
        # register may carry r1 (R9-4 N5).
        if len(insn.operands) < 2:
            continue
        d_form = op not in _SPILL_SLOT_XFORM_LOADS
        if d_form:
            # D-form: rA must be r1 and the displacement must OVERLAP the
            # slot byte-range [c, c+slot_width) for some spill displacement c.
            if insn.operands[1] != 1:
                continue
            if len(insn.operands) < 3:
                continue
            d = insn.operands[2]
            width = _spill_slot_load_width(op, insn)
            slot_width = _SPILL_SLOT_WIDTH[kind]
            if not any(
                d < c + slot_width and d + width > c for c in spill_disp
            ):
                continue
        else:
            # X-form: address = gpr[rA] + gpr[rB]; a slot read if EITHER base
            # is r1 (the other could carry the displacement).
            if insn.operands[1] != 1 and (
                len(insn.operands) < 3 or insn.operands[2] != 1
            ):
                continue
        # The load's destination must be the SAME lane (same kind + number).
        # Determine the destination register kind from the opcode family.
        if op in _SPILL_SLOT_LOAD_GPR:
            dst_kind = GPR
        else:
            dst_kind = FPR
        if dst_kind != kind or insn.operands[0] != lane:
            return False

    # C2: after a slot→lane reload, the lane must not be READ before its
    # next def or function exit (the restore is terminal).  A reloaded value
    # used as data (add r3,r3,r20), stored to a global, passed to a callee,
    # or returned is the escape (V1/V3/B1 class).  Track whether the lane's
    # entry value is live from a restore-load: forward reachability from each
    # reload site, killed by any def of the lane.
    restored_live = [False] * n
    # Seed at the SUCCESSORS of each reload site: the reload itself defs the
    # lane (the restored value), so its live range starts after it.  The
    # def-kill in the fixpoint below must NOT kill at the reload site itself.
    changed = True
    while changed:
        changed = False
        for i in range(n):
            insn = instructions[i]
            if not after_spill[i]:
                continue
            op = insn.opcode
            if op not in _SPILL_SLOT_LOADS_ALL or op == Opcode.LMW:
                continue
            if len(insn.operands) < 2:
                continue
            d_form = op not in _SPILL_SLOT_XFORM_LOADS
            if d_form:
                if insn.operands[1] != 1 or len(insn.operands) < 3:
                    continue
                d = insn.operands[2]
                width = _spill_slot_load_width(op, insn)
                slot_width = _SPILL_SLOT_WIDTH[kind]
                if not any(
                    d < c + slot_width and d + width > c for c in spill_disp
                ):
                    continue
            else:
                if insn.operands[1] != 1 and (
                    len(insn.operands) < 3 or insn.operands[2] != 1
                ):
                    continue
            if op in _SPILL_SLOT_LOAD_GPR:
                dst_kind = GPR
            else:
                dst_kind = FPR
            # A reload of the slot into lane: the entry value is live again.
            if dst_kind == kind and insn.operands[0] == lane:
                for s in _cfg_successors(instructions, i, by_index, end_pc):
                    if not restored_live[s]:
                        restored_live[s] = True
                        changed = True
    # Propagate the restored value's liveness forward; a def of the lane
    # (other than the reload site itself, which is upstream) kills it.
    seeded = True
    while seeded:
        seeded = False
        for i in range(n):
            if not restored_live[i]:
                continue
            if nlane in defs[i]:
                # A def of the lane kills the restored value — the live
                # range ends here.
                continue
            for s in _cfg_successors(instructions, i, by_index, end_pc):
                if not restored_live[s]:
                    restored_live[s] = True
                    seeded = True
        for i in range(n):
            if not restored_live[i]:
                continue
            if nlane not in uses[i]:
                continue
            insn = instructions[i]
            # A spill STORE of the restored value back to the same slot is
            # harmless (the value stays confined in the slot); any OTHER use
            # of the restored value escapes.  (Update-form stores are not
            # recognized as saves — R9-4 N9: stwu/stfdu move r1, so the slot
            # address after the update never matches spill_disp.)
            if kind == GPR:
                is_spill = insn.opcode == Opcode.STW
            else:
                is_spill = insn.opcode == Opcode.STFD
            if is_spill and len(insn.operands) >= 3 and insn.operands[1] == 1 \
                    and insn.operands[2] in spill_disp:
                continue
            return False

    # C3: no memory-observing callee while the slot holds the un-restored
    # entry value.  The slot's PHYSICAL content is divergent (caller r20 vs
    # caller r25); a callee whose contract reads memory could observe it in
    # place (round-9 probe A3).  Precise contracts that read only registers
    # (e.g. ``reads={r3}``) cannot see the slot — the accepted across-call
    # Chaitin control keeps certifying.
    for i in range(n):
        if not after_spill[i]:
            continue
        insn = instructions[i]
        if not (_is_call(insn) or _is_tail_call(insn, by_index, end_pc)):
            continue
        target = _call_target(insn)
        contract = (
            (callee_contracts or {}).get(target)
            if target is not None else None
        )
        reads = getattr(contract, "reads", None)
        if contract is None or reads is None or "*" in reads or "memory" in reads:
            return False
    return True


def _nonvolatile_preservation_failure(
    exits: list[Any],
    initial: MachineState,
    z3: Any,
    side: str,
    gpr_perm: list[int],
    fpr_perm: list[int],
    *,
    budget: _SimplifyBudget | None = None,
) -> WitnessFailure | None:
    """F1 (adversarial review 2026-08): EABI nonvolatile preservation.

    The input binding (``decomp.gpr[j] = X_{inverse[j]}``) assumes the CALLER
    renamed its registers; that fiction is sound only for lanes whose
    caller-visible values coincide on both sides.  EABI makes every nonvolatile
    (r14–r31, f14–f31) caller-visible at the return (the preservation
    obligation), so a PERMUTED nonvolatile lane (``perm[r] != r``) must be
    preserved on EACH side: the terminal value must equal the lane's entry
    binding.  ``z3.simplify``-then-``z3.eq`` proves the simple save/restore
    pattern (``Select(Store(m, slot, X_r), slot)`` reduces to ``X_r``); an
    aliased restore that simplify cannot resolve falls through to SMT — never
    a certificate.  Catches the clobber-without-restore false certificates
    (F1c: ``li r20,7`` vs ``li r25,7``; the suite's r20/r25 load/store tests).
    Identity-mapped lanes need no check here: their terminal comparison is
    already physical (``ls.gpr[j]`` vs ``rs.gpr[j]`` directly), and a
    byte-identical save/restore on an identity lane (e.g. the A1 psq
    prologue/epilogue) certifies via the shared-slot restore.

    Round-8 review (R8-1): the check covers the PS1 shadow half of a permuted
    nonvolatile FPR as well.  Scalar-single FP arithmetic (``fadds``/``fmuls``/
    ``fdivs``/``frsp``/…, semantics.py:4500 ``if is_single:
    state.with_ps1(fd, d_bits)``) and single-precision loads (``lfs``,
    semantics.py:3626) write ``ps1[fd]`` as a side effect, and a prologue that
    saves only the double half (``stfd``/``lfd``) does NOT restore it.  A pair
    that restores ``fpr[lane]`` but leaves ``ps1[lane]`` clobbered previously
    certified: the terminal comparison indexes ps1 by the same perm, so the
    clobber self-agrees while the physical caller-visible ps1 lane diverges
    (retail clobbers the caller's ps1[f20], decomp leaves it untouched) — the
    F1c class for the ps1 half.  ps1 is bound to its owning FPR everywhere
    else in the witness (gate-5 live-in, region-boundary deadness), so the
    preservation obligation must cover it too.
    """
    def _sim(x: Any) -> Any:
        return _z3_simplify(x, z3, budget) if budget is not None else z3.simplify(x)

    for lane in range(14, 32):
        if gpr_perm[lane] == lane:
            continue
        for insn_exit in exits:
            if not z3.eq(_sim(insn_exit.state.gpr[lane]), initial.gpr[lane]):
                return WitnessFailure(
                    "abi-boundary",
                    f"{side} nonvolatile r{lane} not preserved at exit "
                    "(clobbered without restore)",
                )
    for lane in range(14, 32):
        if fpr_perm[lane] == lane:
            continue
        for insn_exit in exits:
            if not z3.eq(_sim(insn_exit.state.fpr[lane]), initial.fpr[lane]):
                return WitnessFailure(
                    "abi-boundary",
                    f"{side} nonvolatile f{lane} not preserved at exit "
                    "(clobbered without restore)",
                )
            # R8-1: the paired-single shadow half must be preserved too — the
            # same simplify-then-eq save/restore test as the double half.
            if not z3.eq(_sim(insn_exit.state.ps1[lane]), initial.ps1[lane]):
                return WitnessFailure(
                    "abi-boundary",
                    f"{side} nonvolatile f{lane}.ps1 not preserved at exit "
                    "(clobbered without restore)",
                )
    return None


def _terminals_agree(
    left: Any,
    right: Any,
    gpr_perm: list[int],
    fpr_perm: list[int],
    z3: Any,
    *,
    left_base: int,
    right_base: int,
    first_divergence: list[str] | None = None,
    simplify_timeout_ms: int = 0,
    budget: _SimplifyBudget | None = None,
    cr_ignore_mask: int = 0,
) -> bool:
    """Structural agreement of two terminals under the renaming permutation.

    Compares every state component; GPR/FPR/PS1 lanes are indexed by the
    permutation (``retail.r_i`` vs ``decomp.r_perm[i]``), everything else
    directly.  Path feasibility is handled by the caller (disjoint pairs are
    skipped before this is called).  Constant values (e.g. ``lr = pc + 4``
    after a call) are compared relative to the function base per the
    location-independence assumption; symbolic values are compared
    structurally.

    ``first_divergence`` (optional out-param): on failure, receives a short
    human label of the first diverging component (r8 WS-1 — lets the caller
    tell the model "memory diverges" vs "gpr r20 diverges" instead of a
    bare structural rejection).
    """
    def _fail(component: str) -> bool:
        if first_divergence is not None:
            first_divergence[:] = [component]
        return False
    if left.exit_kind != right.exit_kind:
        return _fail("exit-kind")
    if not _value_equal(
        left.exit_target, right.exit_target, z3,
        left_base=left_base, right_base=right_base, budget=budget,
    ):
        return _fail("exit-target")
    ls, rs = left.state, right.state
    for i in range(32):
        if not _structurally_equal_simplified(ls.gpr[i], rs.gpr[gpr_perm[i]], z3, timeout_ms=simplify_timeout_ms, budget=budget):
            return _fail(f"gpr r{i}")
    for i in range(32):
        if not _structurally_equal_simplified(ls.fpr[i], rs.fpr[fpr_perm[i]], z3, timeout_ms=simplify_timeout_ms, budget=budget):
            return _fail(f"fpr f{i}")
    for i in range(32):
        if not _structurally_equal_simplified(ls.ps1[i], rs.ps1[fpr_perm[i]], z3, timeout_ms=simplify_timeout_ms, budget=budget):
            return _fail(f"ps1 f{i}")
    direct_pairs = [
        ((ls.cr & ~cr_ignore_mask) if cr_ignore_mask else ls.cr,
         (rs.cr & ~cr_ignore_mask) if cr_ignore_mask else rs.cr, "cr"),
        (ls.xer.ca, rs.xer.ca, "xer.ca"),
        (ls.xer.ov, rs.xer.ov, "xer.ov"),
        (ls.xer.so, rs.xer.so, "xer.so"),
        (ls.fpscr, rs.fpscr, "fpscr"),
        (ls.ctr, rs.ctr, "ctr"),
        (ls.msr, rs.msr, "msr"),
        (ls.time_base, rs.time_base, "time_base"),
        (ls.srr0, rs.srr0, "srr0"),
        (ls.srr1, rs.srr1, "srr1"),
        (ls.valid, rs.valid, "valid"),
        (ls.invalid_reason, rs.invalid_reason, "invalid_reason"),
        (ls.stack_low, rs.stack_low, "stack_low"),
        (ls.stack_layout_valid, rs.stack_layout_valid, "stack_layout_valid"),
        (ls.stack_private, rs.stack_private, "stack_private"),
        (ls.fp_pending_cause, rs.fp_pending_cause, "fp_pending_cause"),
        (ls.fp_pending_fault_pc, rs.fp_pending_fault_pc, "fp_pending_fault_pc"),
        (ls.fp_pending_recoverable, rs.fp_pending_recoverable, "fp_pending_recoverable"),
        (ls.fp_pending_delivery, rs.fp_pending_delivery, "fp_pending_delivery"),
    ]
    for left_v, right_v, label in direct_pairs:
        if not _structurally_equal(left_v, right_v, z3):
            return _fail(label)
    # LR is the one component the summary writes as an absolute constant
    # (``pc + 4`` at the call site); compare it with the location-independent
    # carve-out.
    if not _value_equal(ls.lr, rs.lr, z3, left_base=left_base, right_base=right_base, budget=budget):
        return _fail("lr")
    # Register-file tuples (gqr, sr, aux spr) compare elementwise.
    for name, left_t, right_t in (
        ("gqr", ls.gqr, rs.gqr),
        ("sr", ls.sr, rs.sr),
        ("spr", ls.spr, rs.spr),
    ):
        if len(left_t) != len(right_t) or any(
            not _structurally_equal(a, b, z3) for a, b in zip(left_t, right_t)
        ):
            return _fail(name)
    if getattr(ls, "symbolic_bus", None) is not None or getattr(rs, "symbolic_bus", None) is not None:
        return _fail("symbolic_bus")
    if not _memory_arrays_agree(
        ls.memory, rs.memory, z3,
        left_base=left_base, right_base=right_base, budget=budget,
    ):
        return _fail("memory")
    return True


def _memory_arrays_agree(
    left: Any,
    right: Any,
    z3: Any,
    *,
    left_base: int,
    right_base: int,
    budget: _SimplifyBudget | None = None,
) -> bool:
    """S1 (adversarial review 2026-08): location-aware memory comparison.

    Position-aligned sides produce Store chains from the same shared root with
    the same addresses in the same order; compare the stored VALUES with the
    base-relative ``_value_equal`` — a ``mflr; stw`` after a call stores
    ``pc + 4``, an absolute constant that differs by the function base, so the
    raw structural comparison over-rejected every post-call LR save.  The
    store model is byte-level (``_bus_mem_store`` splits words into four
    per-byte Extracts), so consecutive byte-stores are recombined into the
    stored 32-bit word before the base-relative comparison — the individual
    bytes of a base-offset constant do not differ by the base, but the word
    does.  Any structural mismatch (aliasing, reordering, uninterpreted
    terms) rejects — fail-closed, the pair falls to SMT.
    """
    def _sim(x: Any) -> Any:
        return _z3_simplify(x, z3, budget) if budget is not None else z3.simplify(x)

    def _word_group(
        store: Any,
    ) -> tuple[Any, Any, Any] | None:
        """Recombine 4 consecutive byte-stores (big-endian word) into the
        stored 32-bit value.  The Store chain is innermost-first (byte 0, the
        MSB, at the bottom): walking down from the top visits bytes 3, 2, 1,
        0 at addresses a, a-1, a-2, a-3.  Returns (top_addr, word, tail)."""
        top_addr = store.arg(1)
        parts = [store.arg(2)]  # byte 3 (top), then 2, 1, 0
        node = store.arg(0)
        prev = top_addr
        for _ in range(3):
            if not z3.is_store(node):
                return None
            delta = _sim(prev - node.arg(1))
            if not z3.eq(delta, z3.BitVecVal(1, delta.sort().size())):
                return None
            parts.append(node.arg(2))
            prev = node.arg(1)
            node = node.arg(0)
        # parts = [b3, b2, b1, b0]; word = b0<<24 | b1<<16 | b2<<8 | b3.
        word = parts[-1]
        for part in reversed(parts[:-1]):
            word = z3.Concat(word, part)
        return top_addr, word, node

    def _walk(l: Any, r: Any) -> bool:
        if _structurally_equal(l, r, z3):
            return True
        if z3.is_store(l) and z3.is_store(r):
            gl = _word_group(l)
            gr = _word_group(r)
            if gl is not None and gr is not None:
                base_l, word_l, tail_l = gl
                base_r, word_r, tail_r = gr
                if not _structurally_equal(base_l, base_r, z3):
                    return False
                if not _value_equal(
                    word_l, word_r, z3,
                    left_base=left_base, right_base=right_base, budget=budget,
                ):
                    return False
                return _walk(tail_l, tail_r)
            if not _structurally_equal(l.arg(1), r.arg(1), z3):
                return False
            if not _value_equal(
                l.arg(2), r.arg(2), z3,
                left_base=left_base, right_base=right_base, budget=budget,
            ):
                return False
            return _walk(l.arg(0), r.arg(0))
        return False
    try:
        if _structurally_equal(_sim(left), _sim(right), z3):
            return True
        return _walk(left, right)
    except Exception:
        return False


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
    spill_carveout_used: bool = False,
) -> WitnessOutcome:
    """Execute both sides through SymbolicOps and compare terminals.

    ``retail_r_i`` and ``decomp_r_rho(i)`` share one symbolic variable; both
    sides run through the audited ``execute_cfg``; terminal pairs whose path
    conditions are not disjoint must be structurally identical (``z3.eq``).
    Any divergence or execution inconclusiveness makes the witness fail and
    the caller falls through to the SMT probe.
    """
    # F2 (adversarial review 2026-08): the region path rejects indirect
    # dispatch (bcctr/blrl) because the executor records an indirect-branch
    # terminal with a symbolic CTR that self-agrees under the shared-state
    # binding, masking unmodeled dispatch targets (doc 31 §5 / doc 33 Item 2).
    # The global path had no such guard and certified ``mtctr; bcctr`` pairs;
    # enforce the same soundness-preserving reject here.
    if _has_indirect_dispatch(original) or _has_indirect_dispatch(candidate):
        return WitnessOutcome(False, failure=WitnessFailure(
            "loop",
            "target contains an indirect branch (bcctr/blrl); "
            "dispatch modeling deferred",
        ))
    # Round-3 review BLOCKER (CX-B): a non-link branch to an out-of-function
    # address without a relocation (absolute tail-call form) is not fail-
    # closed by the executor — it records a ``direct-branch`` terminal — so
    # the witness must reject it before execution.  Gate 5's call-observed
    # lane rule would reject the permuted-argument shape anyway; this reject
    # closes the unmodeled-callee boundary for every shape.
    if (
        _has_unmodeled_absolute_branch(original)
        or _has_unmodeled_absolute_branch(candidate)
    ):
        return WitnessOutcome(False, failure=WitnessFailure(
            "loop",
            "target contains an unmodeled absolute tail branch "
            "(out-of-function non-link branch without a relocation); "
            "dispatch modeling deferred",
        ))
    # Direct backward branches (loops) are NOT rejected here: the global path
    # executes them through the bounded-iteration executor (max_instructions /
    # max_paths / max_loop_iterations caps + the deadline), which is fail-closed
    # (overflow or an infeasible exit -> ExecutionInconclusive -> SMT, never a
    # false certificate).  The pre-A1 ~90 s wall on symbolic-trip loops was the
    # superlinear per-fork feasibility simplify, removed by A2 (memoized
    # _path_condition_feasible) + B1 (simplify node cap) in commit 902964154;
    # measured wall on symbolic-trip loops is ~0.6-1.4 s to the caps.  Removing
    # the A1 gate also restores certification of concrete-trip loops with a call
    # inside the loop body (adversarial-review 2026 Finding 1), which A1's
    # region-path routing had over-rejected.  (Rho-conflict loop targets still
    # reach the region path via certify_renaming_witness's gate=="rho" fallback.)
    ops = SymbolicOps()
    z3 = ops.z3
    budget = _SimplifyBudget(timeout_ms=_simplify_timeout(deadline))
    gpr_perm = rho.gpr_perm()
    fpr_perm = rho.fpr_perm()
    retail_initial, decomp_initial = _symbolic_initial_pair(
        ops, gpr_perm, fpr_perm,
    )
    # Symmetric-compare extension: CR bits proven unobservable (volatile
    # field, EQ-only consumption) are excluded from the terminal comparison.
    cr_ignore_mask = _cr_order_insensitive_mask(original, candidate)
    try:
        retail_exits = execute_cfg(
            retail_initial, original, ops,
            max_instructions=max_instructions,
            max_paths=max_paths,
            max_loop_iterations=max_loop_iterations,
            assumed_callees=assumed_callees,
            callee_contracts=callee_contracts or {},
            deadline=deadline,
            path_feasibility_cache={},
            track_access_log=False,
        )
        decomp_exits = execute_cfg(
            decomp_initial, candidate, ops,
            max_instructions=max_instructions,
            max_paths=max_paths,
            max_loop_iterations=max_loop_iterations,
            assumed_callees=assumed_callees,
            callee_contracts=callee_contracts or {},
            deadline=deadline,
            # F3 (adversarial review 2026-08): canonicalize the decomp lanes
            # for the callee token so opaque contracts certify the across-call
            # Chaitin class.
            witness_register_perm=(gpr_perm, fpr_perm),
            path_feasibility_cache={},
            track_access_log=False,
        )
    except (ProofDeadlineExceeded, Exception) as exc:
        if isinstance(exc, ProofDeadlineExceeded):
            return WitnessOutcome(False, rho=rho, failure=WitnessFailure(
                "deadline", exc.phase,
            ))
        return WitnessOutcome(False, rho=rho, failure=WitnessFailure(
            "execute", f"{type(exc).__name__}: {exc}",
        ))

    # F1 (adversarial review 2026-08): EABI nonvolatile preservation on each
    # side — a certified pair must restore every PERMUTED nonvolatile it
    # touches (identity-mapped lanes are compared physically by the terminal
    # comparison itself).
    for exits, initial, side in (
        (retail_exits, retail_initial, "retail"),
        (decomp_exits, decomp_initial, "decomp"),
    ):
        if deadline is not None and deadline.expired():
            return WitnessOutcome(
                False, rho=rho, terminal_pairs_checked=0,
                failure=WitnessFailure("deadline", "nonvolatile-preservation"),
            )
        budget.timeout_ms = _simplify_timeout(deadline)
        failure = _nonvolatile_preservation_failure(
            exits, initial, z3, side, gpr_perm, fpr_perm, budget=budget,
        )
        if failure is not None:
            return WitnessOutcome(False, rho=rho, failure=failure)

    pairs_checked = 0
    left_base = original[0].address
    right_base = candidate[0].address
    for left in retail_exits:
        for right in decomp_exits:
            if deadline is not None and deadline.expired():
                return WitnessOutcome(
                    False, rho=rho, terminal_pairs_checked=pairs_checked,
                    failure=WitnessFailure("deadline", "terminal-compare"),
                )
            # Skip terminal pairs whose path conditions are disjoint: a
            # cheap propositional simplification (no solver).  Pairs that
            # can co-occur must be structurally identical.
            budget.timeout_ms = _simplify_timeout(deadline)
            combined = _z3_simplify(
                z3.And(left.condition, right.condition), z3, budget,
            )
            if z3.is_false(combined):
                continue
            # Bounded SAT fallback (2026-08): the simplify is a local rewrite
            # and can leave a genuine contradiction (e.g. And(X, Not(X)) over
            # per-side byte-read memory terms) unreduced, so cross-path pairs
            # of branchy functions are compared and spuriously reject.  An
            # ``unsat`` QF_BV check proves the paths cannot co-occur — skip.
            # Fail-closed: sat/unknown/timeout compare the pair as before.
            if not z3.is_true(combined) and _path_conditions_disjoint_sat(
                combined, z3, deadline,
            ):
                continue
            pairs_checked += 1
            divergence: list[str] = []
            if not _terminals_agree(
                left, right, gpr_perm, fpr_perm, z3,
                left_base=left_base, right_base=right_base,
                first_divergence=divergence,
                simplify_timeout_ms=deadline.remaining_ms() if deadline else 0,
                budget=budget,
                cr_ignore_mask=cr_ignore_mask,
            ):
                reason = (
                    f"terminal pair ({left.exit_kind}, {right.exit_kind}) "
                    f"diverges structurally"
                )
                if divergence:
                    reason += f" — first divergence: {divergence[0]}"
                return WitnessOutcome(
                    False,
                    rho=rho,
                    failure=WitnessFailure("structural", reason),
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
        # R9-4: propagate the gate-5 carve-out flag so the certificate can
        # declare the no-stack-slot-aliasing scoped assumption.
        spill_carveout_used=spill_carveout_used,
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
    result = run_structural_witness(
        original, candidate, outcome.rho,
        max_instructions=max_instructions,
        max_paths=max_paths,
        max_loop_iterations=max_loop_iterations,
        assumed_callees=assumed_callees,
        callee_contracts=callee_contracts,
        deadline=deadline,
        spill_carveout_used=outcome.spill_carveout_used,
    )
    return result


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
        # bijection; on conflict, start a new region at this slot. For
        # commutative X-form ops, a positional conflict (retail add rA,rB,rC
        # vs decomp add rA,rC,rB) is first retried with the decomp RA/RB
        # swapped — only a real conflict then splits.
        split_here = False
        conflicted = None
        for start_bit, kind in _register_fields(r_insn.opcode):
            rv = (r_insn.raw >> start_bit) & 0x1F
            dv = (d_insn.raw >> start_bit) & 0x1F
            if _conflicts(index, kind, rv, dv):
                conflicted = (start_bit, kind, rv, dv)
                break
        if conflicted is not None and r_insn.opcode in _COMMUTATIVE_RA_RB:
            # Commutative retry with decomp RA <-> RB exchanged.
            d_swapped = d_insn.raw
            ra = (d_swapped >> 16) & 0x1F  # old RA (bits 16-20)
            rb = (d_swapped >> 11) & 0x1F  # old RB (bits 11-15)
            d_swapped = (d_swapped & ~(0x1F << 16)) | (rb << 16)
            d_swapped = (d_swapped & ~(0x1F << 11)) | (ra << 11)
            recheck = False
            for start_bit, kind in _register_fields(r_insn.opcode):
                rv = (r_insn.raw >> start_bit) & 0x1F
                dv = (d_swapped >> start_bit) & 0x1F
                if _conflicts(index, kind, rv, dv):
                    recheck = True
                    break
            if recheck:
                split_here = True
        elif conflicted is not None:
            split_here = True
        # Value-dependent RA (doc 32 A2 rev 5): both-nonzero RA pair is a
        # real register rename participating in the bijection.
        if not split_here and _ra_field_is_register(r_insn, d_insn):
            rv = (r_insn.raw >> 16) & 0x1F
            dv = (d_insn.raw >> 16) & 0x1F
            if _conflicts(index, GPR, rv, dv):
                split_here = True
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
        if _ra_field_is_register(r_insn, d_insn):
            rv = (r_insn.raw >> 16) & 0x1F
            dv = (d_insn.raw >> 16) & 0x1F
            table = rho_gpr
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
        # Snapshot before this instruction so a failed positional attempt can
        # roll back cleanly and retry with swapped commutative operands.
        snap_gpr, snap_fpr = dict(rho_gpr), dict(rho_fpr)
        if not _apply_rho_fields(
            r_insn, d_insn, rho_gpr, rho_fpr,
        ):
            if r_insn.opcode in _COMMUTATIVE_RA_RB:
                # Commutative operand-order swap: retail `add rA,rB,rC` vs
                # decomp `add rA,rC,rB`. Retry with the decomp RA/RB fields
                # swapped, from the pre-instruction snapshot.
                rho_gpr, rho_fpr = snap_gpr, snap_fpr
                if not _apply_rho_fields(
                    r_insn, d_insn, rho_gpr, rho_fpr, swap_ra_rb=True,
                ):
                    return None
            else:
                return None
        # Value-dependent RA (doc 32 A2 rev 5).
        if _ra_field_is_register(r_insn, d_insn):
            rv = (r_insn.raw >> 16) & 0x1F
            dv = (d_insn.raw >> 16) & 0x1F
            if rv in rho_gpr:
                if rho_gpr[rv] != dv:
                    return None
            elif dv in rho_gpr.values():
                return None
            else:
                rho_gpr[rv] = dv
    return Rho(gpr=rho_gpr, fpr=rho_fpr)


def _apply_rho_fields(
    r_insn: Instruction,
    d_insn: Instruction,
    rho_gpr: dict[int, int],
    rho_fpr: dict[int, int],
    *,
    swap_ra_rb: bool = False,
) -> bool:
    """Apply one instruction pair's register fields to the rho tables.
    Returns True on success (no conflict). With *swap_ra_rb*, the decomp
    RA/RB register bits are exchanged before comparison (for commutative
    operand-order swaps)."""
    fields = _register_fields(r_insn.opcode)
    d_raw = d_insn.raw
    if swap_ra_rb:
        ra = (d_raw >> 16) & 0x1F
        rb = (d_raw >> 11) & 0x1F
        d_raw = (d_raw & ~(0x1F << 16)) | (rb << 16)
        d_raw = (d_raw & ~(0x1F << 11)) | (ra << 11)
    for start_bit, kind in fields:
        rv = (r_insn.raw >> start_bit) & 0x1F
        dv = (d_raw >> start_bit) & 0x1F
        table = rho_gpr if kind == GPR else rho_fpr
        if rv in table:
            if table[rv] != dv:
                return False
        elif dv in table.values():
            return False
        else:
            table[rv] = dv
    return True


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
    budget = _SimplifyBudget(timeout_ms=_simplify_timeout(deadline))
    # Symmetric-compare CR extension (same as the whole-function driver):
    # CR bits proven unobservable (volatile field, EQ-only consumption) are
    # excluded from terminal comparison. Without this binding the terminal
    # comparison below raises NameError, surfacing as a spurious "execute"
    # gate failure for every region-sliced certification.
    cr_ignore_mask = _cr_order_insensitive_mask(original, candidate)
    # Full-stream validation (gates 1/2/3/6) BEFORE region slicing: the
    # global path returns at the first rho conflict, so slots after it were
    # never checked for reject-list / reloc / non-register-bit equality
    # (impl-review Finding 3 — an ``mffs`` after a conflict certified).
    stream_failure = _stream_validation_failure(original, candidate)
    if stream_failure is not None:
        return WitnessOutcome(False, failure=stream_failure)
    boundaries = _rho_region_boundaries(original, candidate)
    # Region rho per boundary interval: [0, b0), [b0, b1), ..., [bn, end).
    region_starts = [0] + boundaries
    regions: list[tuple[int, int, Rho]] = []
    # R9-4 F3 fix: OR-accumulate the carve-out flag across ALL regions (each
    # _check_abi_fixedness call overwrites the out-list with ITS region's
    # snapshot; the union is what matters for the certificate declaration).
    carveout_used: list[bool] = [False]
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
        region_carveout: list[bool] = [False]
        abi_failure = _check_abi_fixedness(original, candidate, rho, declared_return,
                                  callee_contracts, carveout_out=region_carveout)
        if abi_failure is not None:
            return WitnessOutcome(False, rho=rho, failure=abi_failure)
        if region_carveout and region_carveout[0]:
            carveout_used[0] = True  # OR-accumulate across regions (R9-4 F3)
        # F1 (adversarial review 2026-08): the region path does not run the
        # per-terminal nonvolatile preservation check, so a non-identity
        # mapping on an EABI nonvolatile lane (r14–r31 / f14–f31) in ANY
        # region is rejected outright — a locally re-allocated nonvolatile
        # temp is the clobber-without-restore false-cert shape (e.g. retail
        # r5 vs decomp r20: decomp clobbers r20 the caller expects preserved
        # while retail leaves it untouched).  Fail-closed: falls to SMT.
        gpr_perm_region = rho.gpr_perm()
        fpr_perm_region = rho.fpr_perm()
        if any(gpr_perm_region[r] != r for r in range(14, 32)) or any(
            fpr_perm_region[f] != f for f in range(14, 32)
        ):
            return WitnessOutcome(False, rho=rho, failure=WitnessFailure(
                "abi-boundary",
                f"region [{start}, {end}) permutes an EABI nonvolatile lane; "
                "falls to SMT",
            ))
        # A1 post-rho belt-and-suspenders (per region, doc 32 A1 rev 3, I12).
        psq_failure = _psq_operands_rho_fixed(original, rho, start, end)
        if psq_failure is not None:
            return WitnessOutcome(False, rho=rho, failure=psq_failure)
        regions.append((start, end, rho))
    # Loop policy (doc 33 Item 2, loop second-cut): region slicing now
    # executes DIRECT backward-branch loops through the bounded-iteration
    # executor (``max_loop_iterations``; overflow raises ExecutionInconclusive
    # before any terminal is recorded => degrade to SMT, never a partial-
    # unroll certificate).  Two guards remain:
    #  (a) indirect dispatch (bcctr/blrl) stays rejected — scope choice,
    #      jump-table dispatch modeling is doc-28/30 work; and
    #  (b) a region boundary may not fall INSIDE a loop span: the rebinding
    #      driver is only sound for loop-free spans (four-lane deadness
    #      covers loop-carried lanes at the loop's edges, not mid-body
    #      rebinds that would re-execute a body under a different rho).
    if _has_indirect_dispatch(original) or _has_indirect_dispatch(candidate):
        return WitnessOutcome(False, failure=WitnessFailure(
            "loop",
            "target contains an indirect branch (bcctr/blrl); "
            "dispatch modeling deferred",
        ))
    # Round-3 review BLOCKER (CX-B): unmodeled absolute tail branches must be
    # rejected here too (the executor records a ``direct-branch`` terminal
    # for them instead of failing closed).
    if (
        _has_unmodeled_absolute_branch(original)
        or _has_unmodeled_absolute_branch(candidate)
    ):
        return WitnessOutcome(False, failure=WitnessFailure(
            "loop",
            "target contains an unmodeled absolute tail branch "
            "(out-of-function non-link branch without a relocation); "
            "dispatch modeling deferred",
        ))
    spans_r = _loop_spans(original, local_symbol=local_symbol)
    spans_d = _loop_spans(candidate, local_symbol=candidate_local_symbol or local_symbol)
    if spans_r or spans_d:
        # Compare each side's spans against THAT side's boundary addresses
        # (round-3 finding: candidate spans must not be checked against
        # original-space boundary addresses — position-aligned gates mask the
        # mismatch today, but the guard must be self-consistent).
        boundary_addrs_r = [original[b].address for b in boundaries]
        boundary_addrs_d = [candidate[b].address for b in boundaries]
        for spans, addrs in ((spans_r, boundary_addrs_r), (spans_d, boundary_addrs_d)):
            for tgt, br in spans:
                for addr in addrs:
                    if tgt < addr < br:
                        return WitnessOutcome(False, failure=WitnessFailure(
                            "loop",
                            f"region boundary {addr:#x} falls inside loop span "
                            f"[{tgt:#x}, {br:#x}]; rebinding mid-iteration is unsound",
                        ))

    def _run_region(
        start_state: MachineState,
        start_pc: int,
        condition: Any,
        visits: dict[int, int],
        steps: int,
        stop: frozenset[int] | None,
        side: list[Instruction],
        register_perm: tuple[list[int], list[int]] | None = None,
    ) -> tuple[list[Terminal], list[tuple[int, MachineState, Any, dict[int, int], int]]]:
        paused: list[tuple[int, MachineState, Any, dict[int, int], int]] = []
        if start_state is None:
            raise AssertionError("region driver: start_state required")
        # Seed-time feasibility pre-check (plan §3.2, impl-review Finding 7):
        # the seed bypasses enqueue's _path_condition_feasible, so an
        # infeasible resumed condition would otherwise execute until its
        # first prune.  Reject the region run outright instead.  (Adversarial
        # review 2026 Finding 3: this was the one unbounded simplify call site
        # — thread the SAME run-scoped cache + deadline used by the inner
        # execute_cfg so a large carried frontier condition cannot spin the
        # th_rewriter.  Fail-open on timeout: an infeasible seed that slips
        # through is pruned downstream or produces no co-feasible terminal
        # pairs, both fail-closed.)
        seed_cache: dict[int, tuple[Any, bool]] = {}
        if not _path_condition_feasible(
            condition, ops, cache=seed_cache, deadline=deadline,
        ):
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
                path_feasibility_cache=seed_cache,
                track_access_log=False,
                stop_at_pcs=stop,
                initial_seed=(start_pc, start_state, condition, visits, steps),
                paused_out=paused,
                # F3 (adversarial review 2026-08): the region's rho is the
                # binding the resumed state carries; canonicalize for the
                # callee token.
                witness_register_perm=register_perm,
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
                    register_perm=(rho.gpr_perm(), rho.fpr_perm()),
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
                if deadline is not None and deadline.expired():
                    return WitnessOutcome(
                        False, failure=WitnessFailure(
                            "deadline", "terminal-compare",
                        ),
                        terminal_pairs_checked=pairs_checked,
                    )
                budget.timeout_ms = _simplify_timeout(deadline)
                combined = _z3_simplify(
                    z3.And(left.condition, right.condition), z3, budget,
                )
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
                divergence: list[str] = []
                if not _terminals_agree(
                    left, right, gpr_perm, fpr_perm, z3,
                    left_base=left_base, right_base=right_base,
                    first_divergence=divergence,
                    simplify_timeout_ms=deadline.remaining_ms() if deadline else 0,
                    budget=budget,
                    cr_ignore_mask=cr_ignore_mask,
                ):
                    reason = (
                        f"terminal pair ({left.exit_kind}, {right.exit_kind}) "
                        "diverges structurally under region rho"
                    )
                    if divergence:
                        reason += f" — first divergence: {divergence[0]}"
                    return WitnessOutcome(
                        False,
                        failure=WitnessFailure("structural", reason),
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
            spill_carveout_used=bool(carveout_used and carveout_used[0]),
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
