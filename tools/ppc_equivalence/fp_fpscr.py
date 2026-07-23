"""Centralized Broadway FPSCR transitions (SCALAR_FP_V2 Phase 4).

``apply_fpscr_transition`` is the single audited path for scalar FP FPSCR
updates. Production ``semantics._fpscr_raise`` remains authoritative until
Phase 12; this module mirrors that behavior for exact-oracle / test integration
behind ``SCALAR_FP_EXACT_V2``.
"""

from __future__ import annotations

from dataclasses import dataclass
from typing import Any

from .fp_exact_outcome import FiFrPolicy, ScalarFPOutcome
from .fp_oracle import mask32, mask64

# --- Broadway FPSCR bit layout (Gekko / Broadway, Python LSB indexing) ---

FPSCR_FX = 1 << 31
FPSCR_FEX = 1 << 30
FPSCR_VX = 1 << 29
FPSCR_OX = 1 << 28
FPSCR_UX = 1 << 27
FPSCR_ZX = 1 << 26
FPSCR_XX = 1 << 25
FPSCR_VXSNAN = 1 << 24
FPSCR_VXISI = 1 << 23
FPSCR_VXIDI = 1 << 22
FPSCR_VXZDZ = 1 << 21
FPSCR_VXIMZ = 1 << 20
FPSCR_VXVC = 1 << 19
FPSCR_FR = 1 << 18
FPSCR_FI = 1 << 17
FPSCR_FPRF_MASK = 0x1F << 12
FPSCR_VXSOFT = 1 << 10
FPSCR_VXSQRT = 1 << 9
FPSCR_VXCVI = 1 << 8
FPSCR_VE = 1 << 7
FPSCR_OE = 1 << 6
FPSCR_UE = 1 << 5
FPSCR_ZE = 1 << 4
FPSCR_XE = 1 << 3
FPSCR_NI = 1 << 2

FPSCR_VX_ANY = (
    FPSCR_VXSNAN
    | FPSCR_VXISI
    | FPSCR_VXIDI
    | FPSCR_VXZDZ
    | FPSCR_VXIMZ
    | FPSCR_VXVC
    | FPSCR_VXSOFT
    | FPSCR_VXSQRT
    | FPSCR_VXCVI
)
FPSCR_ANY_ENABLE = FPSCR_VE | FPSCR_OE | FPSCR_UE | FPSCR_ZE | FPSCR_XE
FPSCR_HW_MASK = 0xFFFFF7FF

FPSCR_EXCEPTION_BITS = (
    FPSCR_FX
    | FPSCR_OX
    | FPSCR_UX
    | FPSCR_ZX
    | FPSCR_XX
    | FPSCR_VX_ANY
)

FPSCR_CONTROL_OPS = frozenset({
    "mffs",
    "mtfsf",
    "mtfsfi",
    "mtfsb0",
    "mtfsb1",
    "mcrfs",
})

# Bitwise scalar ops preserve FI/FR; everything else defaults to CLEAR/SET via outcome.
FPSCR_FI_FR_PRESERVE_OPS = frozenset({
    "fmr",
    "fabs",
    "fneg",
    "fnabs",
    "fres",
    "frsqrte",
    "fsel",
})


@dataclass(frozen=True)
class FPSCRControlWrite:
    """Side-band operands for FPSCR control instructions."""

    mtfsf_fm: int | None = None
    mtfsf_source: int | None = None
    mtfsfi_bf: int | None = None
    mtfsfi_imm4: int | None = None
    mtfsb_bt: int | None = None
    mtfsb_set: bool | None = None
    mcrfs_bfa: int | None = None


@dataclass(frozen=True)
class MCRFSResult:
    """``mcrfs`` transfers a CR nibble and clears selected exception stickies."""

    post_fpscr: int
    cr_field: int


def cr1_from_fpscr(fpscr: int) -> int:
    """CR1 nibble copied from FPSCR for record-form FP ops (FPCC shadow)."""
    return (int(fpscr) >> 28) & 0xF


def _field_mask(field: int) -> int:
    return 0xF << ((7 - field) * 4)


def recompute_vx_summary(fpscr: int) -> int:
    """Set or clear FPSCR.VX from VX subcause stickies."""
    fpscr = mask64(fpscr)
    if fpscr & FPSCR_VX_ANY:
        return fpscr | FPSCR_VX
    return fpscr & ~FPSCR_VX


def recompute_fex(fpscr: int) -> int:
    """Recompute FPSCR.FEX from latched causes × enables."""
    fpscr = mask64(fpscr)
    enabled = ((fpscr >> 22) & fpscr & FPSCR_ANY_ENABLE)
    if enabled:
        return fpscr | FPSCR_FEX
    return fpscr & ~FPSCR_FEX


def recompute_summaries(fpscr: int) -> int:
    """Recompute derived VX and FEX summary bits."""
    return recompute_fex(recompute_vx_summary(fpscr))


def normalize_fpscr(value: int, *, preserve_fx: bool = True) -> int:
    """Apply Broadway writable mask and recompute VX/FEX summaries.

    ``preserve_fx`` keeps an already-set FX sticky when normalization does not
    latch new exception bits (``mtfs*`` / ``mcrfs`` paths).
    """
    pre_fx = int(value) & FPSCR_FX
    value = mask64(value) & FPSCR_HW_MASK
    value = recompute_summaries(value)
    if preserve_fx and pre_fx:
        value |= FPSCR_FX
    return value


def raise_sticky(pre_fpscr: int, exception_mask: int) -> int:
    """Latch sticky exception bits and apply FX / VX / FEX recompute rules."""
    pre_fpscr = mask64(pre_fpscr)
    mask = int(exception_mask) & FPSCR_EXCEPTION_BITS
    if mask == 0:
        return pre_fpscr
    already_set = (pre_fpscr & mask) == mask
    value = pre_fpscr | mask
    if not already_set:
        value |= FPSCR_FX
    return recompute_summaries(value)


def apply_fi_fr(
    fpscr: int,
    *,
    policy: FiFrPolicy,
    fi: bool = False,
    fr: bool = False,
) -> int:
    """Update FPSCR.FI/FR under Broadway preserve / clear / set rules."""
    fpscr = mask64(fpscr)
    if policy is FiFrPolicy.PRESERVE:
        return fpscr
    cleared = fpscr & ~(FPSCR_FI | FPSCR_FR)
    if policy is FiFrPolicy.CLEAR:
        return cleared
    value = cleared
    if fi:
        value |= FPSCR_FI
    if fr:
        value |= FPSCR_FR
    return value


def apply_fprf(fpscr: int, fprf: int) -> int:
    """Replace the five FPRF bits (FPSCR[12:16])."""
    fpscr = mask64(fpscr)
    fprf &= 0x1F
    return (fpscr & ~FPSCR_FPRF_MASK) | (fprf << 12)


def apply_raised_causes(pre_fpscr: int, raised_causes: int) -> int:
    """Latch OX/UX/ZX/XX and VX subcauses from an outcome cause mask."""
    causes = mask_scalar_raised_by_enables(pre_fpscr, raised_causes) & (
        FPSCR_OX | FPSCR_UX | FPSCR_ZX | FPSCR_XX | FPSCR_VX_ANY
    )
    value = mask64(pre_fpscr)
    bit = 1
    while causes:
        if causes & bit:
            value = raise_sticky(value, bit)
            causes &= ~bit
        bit <<= 1
    if int(raised_causes) & FPSCR_VX_ANY:
        value = recompute_vx_summary(value)
        value = recompute_fex(value)
    return value


def mask_scalar_raised_by_enables(fpscr: int, raised_causes: int) -> int:
    """Gate OX/UX/ZX/XX stickies on FPSCR enable bits (Broadway/Dolphin default paths).

    VX subcauses always pass through. When OE/UE/XE/ZE are clear, hardware still
    updates FPRF/FI/FR but does not latch the corresponding OX/UX/XX/ZX stickies.
    """
    fpscr = mask32(fpscr)
    raised = int(raised_causes)
    masked = raised & FPSCR_VX_ANY
    if raised & FPSCR_ZX:
        masked |= FPSCR_ZX
    if raised & FPSCR_OX and (fpscr & FPSCR_OE):
        masked |= FPSCR_OX
    if raised & FPSCR_UX and (fpscr & FPSCR_UE):
        masked |= FPSCR_UX
    if raised & FPSCR_XX and (fpscr & FPSCR_XE):
        masked |= FPSCR_XX
    return masked


def apply_mtfsf(pre_fpscr: int, fm: int, source32: int) -> int:
    """``mtfsf`` field-masked merge + normalization."""
    mask = 0
    for field in range(8):
        if fm & (1 << (7 - field)):
            mask |= _field_mask(field)
    source32 = int(source32) & 0xFFFFFFFF
    merged = (pre_fpscr & ~mask) | (source32 & mask)
    return normalize_fpscr(merged)


def apply_mtfsfi(pre_fpscr: int, bf: int, imm4: int) -> int:
    """``mtfsfi`` immediate field write + normalization."""
    shift = (7 - int(bf)) * 4
    mask = 0xF << shift
    merged = (pre_fpscr & ~mask) | ((int(imm4) & 0xF) << shift)
    return normalize_fpscr(merged)


def apply_mtfsb(pre_fpscr: int, bt: int, *, set_bit: bool) -> int:
    """``mtfsb0`` / ``mtfsb1`` bit clear or exception-aware set."""
    bit = 1 << (31 - int(bt))
    if set_bit and bit & FPSCR_EXCEPTION_BITS:
        return raise_sticky(pre_fpscr, bit)
    if set_bit:
        return normalize_fpscr(pre_fpscr | bit)
    return normalize_fpscr(pre_fpscr & ~bit)


def apply_mcrfs(pre_fpscr: int, bfa: int) -> MCRFSResult:
    """``mcrfs`` CR transfer + selective exception-bit clearing in source field."""
    shift = (7 - int(bfa)) * 4
    cr_field = (pre_fpscr >> shift) & 0xF
    clear_mask = _field_mask(int(bfa)) & (
        FPSCR_FX | FPSCR_OX | FPSCR_UX | FPSCR_ZX | FPSCR_XX | FPSCR_VX_ANY
    )
    post = normalize_fpscr(pre_fpscr & ~clear_mask)
    return MCRFSResult(post_fpscr=post, cr_field=cr_field)


def default_fi_fr_policy(opcode: str) -> FiFrPolicy:
    """Opcode-default FI/FR policy when the outcome does not override."""
    name = str(opcode)
    if name in FPSCR_FI_FR_PRESERVE_OPS:
        return FiFrPolicy.PRESERVE
    if name in FPSCR_CONTROL_OPS or name in {"mffs"}:
        return FiFrPolicy.PRESERVE
    return FiFrPolicy.CLEAR


def apply_fpscr_transition(
    pre_fpscr: int,
    opcode: str,
    outcome: ScalarFPOutcome | None = None,
    control_write: FPSCRControlWrite | None = None,
) -> int:
    """Apply one scalar FP FPSCR transition (exact Broadway rules).

    Parameters
    ----------
    pre_fpscr:
        FPSCR value before the instruction.
    opcode:
        Scalar FP opcode name (e.g. ``"fadd"``, ``"mtfsf"``).
    outcome:
        Exact operation outcome for arithmetic/compare/convert paths. May be
        ``None`` for pure control ops when ``control_write`` is supplied.
    control_write:
        Operands for ``mtfsf`` / ``mtfsfi`` / ``mtfsb*`` / ``mcrfs``.
    """
    pre = mask64(pre_fpscr)
    name = str(opcode)

    if control_write is not None:
        if control_write.mtfsf_fm is not None:
            source = control_write.mtfsf_source if control_write.mtfsf_source is not None else 0
            return apply_mtfsf(pre, int(control_write.mtfsf_fm), int(source))
        if control_write.mtfsfi_bf is not None:
            imm = control_write.mtfsfi_imm4 if control_write.mtfsfi_imm4 is not None else 0
            return apply_mtfsfi(pre, int(control_write.mtfsfi_bf), int(imm))
        if control_write.mtfsb_bt is not None:
            set_bit = bool(control_write.mtfsb_set)
            return apply_mtfsb(pre, int(control_write.mtfsb_bt), set_bit=set_bit)
        if control_write.mcrfs_bfa is not None:
            return apply_mcrfs(pre, int(control_write.mcrfs_bfa)).post_fpscr

    if name == "mffs":
        return pre

    if outcome is None:
        raise ValueError(
            f"apply_fpscr_transition requires outcome or control_write for opcode {name!r}",
        )

    value = apply_raised_causes(pre, outcome.raised_causes)
    policy = outcome.fi_fr_policy
    if policy is FiFrPolicy.PRESERVE and name not in FPSCR_FI_FR_PRESERVE_OPS:
        policy = default_fi_fr_policy(name)
    value = apply_fi_fr(value, policy=policy, fi=outcome.fi, fr=outcome.fr)
    if outcome.update_fprf:
        value = apply_fprf(value, outcome.fprf)
    return value


def apply_fpscr_transition_with_mcrfs(
    pre_fpscr: int,
    bfa: int,
) -> MCRFSResult:
    """Convenience wrapper for ``mcrfs`` CR-field side effect."""
    return apply_mcrfs(pre_fpscr, bfa)


def fpscr_sticky_modeling_status() -> dict[str, bool]:
    """Capability surface for promotion / status reporting.

    SymbolicOps does not fully SMT-model OX/UX/XX latching (SOUNDNESS.md).
    ConcreteOps / SoftFloat and the opt-in exact-v2 path cover more bits, but
    EQUIVALENT authorization uses SymbolicOps — report incompleteness honestly.
    """
    return {
        "ox": False,
        "ux": False,
        "zx": True,
        "xx": False,
        "vx": True,
        "vx_subcauses_complete": False,
        "fx": False,
        "fex": False,
        "fi": False,
        "fr": False,
        "fprf": False,
        "enable_ve_ze_oe_ue_xe": True,
        "sticky_preservation_complete": False,
        "mtfs_normalization": True,
        "mcrfs_selective_clear": True,
    }


# Assumption tag recorded on EQUIVALENT proofs that compare ``fpscr`` while the
# SymbolicOps path leaves OX/UX/XX stickies unmodeled (see SOUNDNESS.md).
FPSCR_SYMBOLIC_STICKIES_ASSUMPTION = "fpscr-symbolic-stickies-incomplete"


def symbolic_fpscr_stickies_incomplete() -> bool:
    """True when the default SymbolicOps path leaves OX/UX/XX stickies unmodeled."""
    status = fpscr_sticky_modeling_status()
    return not (status["ox"] and status["ux"] and status["xx"])


FPSCR_STICKIES_UNSUPPORTED = (
    "fpscr compared after FP arithmetic but SymbolicOps leaves OX/UX/XX "
    f"stickies unmodeled ({FPSCR_SYMBOLIC_STICKIES_ASSUMPTION}); "
    "fail closed — cannot authorize EQUIVALENT"
)

# Opcodes that publish FPSCR (or a sticky-bearing projection) into another
# compared observable — demotion must not require the literal ``fpscr`` name.
_FPSCR_PROJECTION_OPS = frozenset({"mffs", "mcrfs"})


def _observes_cr_field(observables: set[str], field: int) -> bool:
    """True when ``cr`` or ``cr{field}`` is among compared observables."""
    return "cr" in observables or f"cr{field}" in observables


def _compares_fpscr_or_sticky_projection(result: Any) -> bool:
    """True when compared observables can see incomplete FPSCR stickies.

    Sticky visibility paths:
    * literal ``fpscr``
    * ``mffs`` into a compared FPR / ``mcrfs`` into a compared CR field
    * record-form FP Rc copying FPSCR[FX,FEX,VX,OX] into CR1 (observing
      ``cr1`` or whole ``cr``) — even when ``mffs``/``mcrfs`` are absent
    """
    observables = {str(item) for item in (getattr(result, "observables", None) or [])}
    if "fpscr" in observables:
        return True
    # Record-form scalar/PS Rc writes CR1 from the FPSCR high nibble (includes
    # OX). Observing that field after FP arithmetic is a sticky projection.
    if _observes_cr_field(observables, 1):
        return True
    opcodes = {str(item) for item in (getattr(result, "opcodes_used", None) or [])}
    if not opcodes & _FPSCR_PROJECTION_OPS:
        return False
    # mffs writes an FPR; mcrfs writes a CR field. Either projection carries XX.
    if any(
        name.startswith("f") and (name[1:].isdigit() or name.endswith(".ps1"))
        for name in observables
    ):
        return True
    if "cr" in observables or any(
        name.startswith("cr") and name[2:].isdigit() for name in observables
    ):
        return True
    return False


def annotate_fpscr_sticky_incompleteness(
    result: Any,
    *,
    identical_implementations: bool = False,
) -> None:
    """Fail closed when FPSCR stickies are compared after FP arithmetic under SymbolicOps.

    SymbolicOps does not SMT-model OX/UX/XX sticky latching. An ``unsat``
    divergence query over ``fpscr`` — or over an FPR/CR field that received a
    sticky projection via ``mffs`` / ``mcrfs`` / record-form Rc→CR1 — can
    therefore report EQUIVALENT while hardware stickies diverge (for example,
    clearing XX after an inexact ``fdivs``, then ``mffs`` into a compared FPR;
    or overflow ``fadds.`` vs clear-OX + ``fmr.`` when ``cr1`` is observed).
    When such an observable is compared and a non-bitwise FP opcode was used on
    *differing* implementations, demote EQUIVALENT to ``INCONCLUSIVE_ABSTRACTION``.
    Byte-identical implementations keep EQUIVALENT (same code ⇒ same hardware
    stickies) but still record the incompleteness assumption for triage.
    FP-bitwise-only proofs (``fmr``/``fabs``/``fneg``/``fnabs``) never touch
    stickies and are skipped.

    Idempotent. Safe to call on any result; no-ops unless the proof is still
    EQUIVALENT and actually compared fpscr (or a sticky projection) after FP
    arithmetic. Independent of ``allow_nan``: NaN-freedom and sticky modeling
    are separate fail-closed gates.
    """
    if getattr(result, "status", None) is None:
        return
    # Duck-typed EQUIVALENT check (avoid importing ProofStatus at module load).
    status = result.status
    status_value = getattr(status, "value", status)
    if status_value != "equivalent":
        return
    if not _compares_fpscr_or_sticky_projection(result):
        return
    if not symbolic_fpscr_stickies_incomplete():
        return

    from tools.ppc_equivalence.fp_bitwise import FP_BITWISE_OPS, fp_opcodes_among
    from tools.ppc_equivalence.result import ProofStatus

    fp_ops = fp_opcodes_among(getattr(result, "opcodes_used", None) or [])
    # Only arithmetic-ish FP touches OX/UX/XX; pure bitwise sign ops do not.
    # Projection ops alone (mffs of an untouched fpscr) do not require demotion.
    if not (fp_ops - FP_BITWISE_OPS - _FPSCR_PROJECTION_OPS):
        return

    assumptions = list(getattr(result, "assumptions", None) or [])
    if FPSCR_SYMBOLIC_STICKIES_ASSUMPTION not in assumptions:
        assumptions.append(FPSCR_SYMBOLIC_STICKIES_ASSUMPTION)
        result.assumptions = assumptions

    if identical_implementations:
        # Same bytes cannot diverge on unmodeled stickies; keep EQUIVALENT.
        warning = (
            "fpscr compared after FP arithmetic; SymbolicOps leaves OX/UX/XX "
            f"stickies unmodeled ({FPSCR_SYMBOLIC_STICKIES_ASSUMPTION}), but "
            "implementations are byte-identical so EQUIVALENT remains sound"
        )
        warnings = list(getattr(result, "warnings", None) or [])
        if warning not in warnings:
            warnings.append(warning)
            result.warnings = warnings
        return

    warning = FPSCR_STICKIES_UNSUPPORTED
    warnings = list(getattr(result, "warnings", None) or [])
    if warning not in warnings:
        warnings.append(warning)
        result.warnings = warnings

    unsupported = list(getattr(result, "unsupported", None) or [])
    if FPSCR_STICKIES_UNSUPPORTED not in unsupported:
        unsupported.append(FPSCR_STICKIES_UNSUPPORTED)
        result.unsupported = unsupported

    abstractions = list(getattr(result, "abstractions", None) or [])
    if "fpscr-symbolic-stickies-unmodeled" not in abstractions:
        abstractions.append("fpscr-symbolic-stickies-unmodeled")
        result.abstractions = abstractions

    result.status = ProofStatus.INCONCLUSIVE_ABSTRACTION
