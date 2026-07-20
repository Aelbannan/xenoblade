"""PR18 floating-point exception trap delivery (Wave 5 Track D).

Pipeline for arithmetic ops that already produce :class:`FPOutcome` / SoftFloat
sticky updates:

```text
outcome / sticky raise → update sticky/cause → inspect enable bits
  → if enabled exception: suppress destination (scalar) + program-exception
  → else: continue
```

Reuses the existing exception exit vocabulary (``program-exception``, vector
``0x700``, ``_exception_entry`` SRR0/SRR1/MSR). Equivalence already compares
trap versus continue via terminal ``exit_kind`` / ``exit_target``.

**Supported under ``traps_enabled`` (Tier C):**

- VE / ZE / OE / UE / XE for the scalar SoftFloat arith + fused family
  (``TRAP_DELIVERY_SUPPORTED_OPS``), with OX/UX/XX sticky latch from SoftFloat
  outcomes on ConcreteOps.
- Wave-3 paired-oracle ops (``TRAP_DELIVERY_PAIRED_OPS``): unconditional lane
  writeback + trap when any accumulated lane sticky matches an enable.
- **FEX already-set re-trap (Broadway precise policy):** each instruction that
  raises an enabled exception delivers a program interrupt, even when FEX was
  already 1. Detection uses newly-latched stickies and/or a ConcreteOps
  re-raise note from ``_fpscr_raise_if`` (same sticky already set). SMT proofs
  still typically start with FEX clear; re-raise is covered on ConcreteOps.
- **NI×trap:** flush-to-zero (PR17) composes with trap delivery for the
  intersection of NI-supported and trap-supported opcodes. NI-unsupported
  opcodes with NI possibly set remain inconclusive (no IEEE widening).

**Fail closed / deferred:**

- Estimates, compares, conversions (``frsp`` / ``fctiw*``), non-oracle paired.
- SymbolicOps: OE/UE/XE must stay clear (OX/UX/XX not SMT-modeled).
- MSR FE0/FE1 precise vs imprecise modes: deferred; ``traps_enabled`` assumes
  precise delivery when an enabled exception occurs on the instruction.
  Wave 4 ``fp-traps`` capability attestation stays incomplete /
  never promotion-grade until FE0/FE1 is modeled (``fe0_fe1: false`` in
  the validation ledger).

Still **Tier C** only; does not enable automatic promotion.
"""

from __future__ import annotations

from contextvars import ContextVar
from dataclasses import dataclass, replace
from typing import Any

from .fp_outcome import FPExceptionFlags, FPOutcome
from .ir import ExecutionInconclusive, Opcode

# Classic PowerPC program interrupt vector and SRR1 cause bit
# (MSB-numbered bit 11 = floating-point enabled exception).
PROGRAM_EXCEPTION_VECTOR = 0x700
SRR1_FP_ENABLED_EXCEPTION = 0x00100000

# Mirror semantics.py FPSCR masks (Gekko layout) without importing semantics
# (avoids circular imports from CFG wiring).
FPSCR_FEX = 1 << 30
FPSCR_VX = 1 << 29
FPSCR_OX = 1 << 28
FPSCR_UX = 1 << 27
FPSCR_ZX = 1 << 26
FPSCR_XX = 1 << 25
FPSCR_VE = 1 << 7
FPSCR_OE = 1 << 6
FPSCR_UE = 1 << 5
FPSCR_ZE = 1 << 4
FPSCR_XE = 1 << 3
FPSCR_ANY_ENABLE = FPSCR_VE | FPSCR_OE | FPSCR_UE | FPSCR_ZE | FPSCR_XE
FPSCR_OX_UX_XX_ENABLES = FPSCR_OE | FPSCR_UE | FPSCR_XE
FPSCR_VE_ZE_ENABLES = FPSCR_VE | FPSCR_ZE
# VX* subcauses occupy bits 8..24 (VXCVI .. VXSNAN) in the Gekko FPSCR layout.
FPSCR_VX_SUBCAUSES = (
    (1 << 24) | (1 << 23) | (1 << 22) | (1 << 21) | (1 << 20)
    | (1 << 19) | (1 << 10) | (1 << 9) | (1 << 8)
)
FPSCR_STICKY_TRAP_MASK = (
    FPSCR_OX | FPSCR_UX | FPSCR_ZX | FPSCR_XX | FPSCR_VX_SUBCAUSES
)

# Scalar ops with modeled sticky + destination suppression (SoftFloat /
# FPOutcome family on ConcreteOps; host/Z3 path shares VE/ZE raise logic).
TRAP_DELIVERY_SUPPORTED_OPS: frozenset[Opcode] = frozenset({
    Opcode.FADD, Opcode.FADDS,
    Opcode.FSUB, Opcode.FSUBS,
    Opcode.FMUL, Opcode.FMULS,
    Opcode.FDIV, Opcode.FDIVS,
    Opcode.FMADD, Opcode.FMADDS,
    Opcode.FMSUB, Opcode.FMSUBS,
    Opcode.FNMADD, Opcode.FNMADDS,
    Opcode.FNMSUB, Opcode.FNMSUBS,
})

# Wave-3 paired SoftFloat oracle ops: trap with unconditional writeback.
TRAP_DELIVERY_PAIRED_OPS: frozenset[Opcode] = frozenset({
    Opcode.PS_ADD, Opcode.PS_SUB, Opcode.PS_MUL,
    Opcode.PS_MADD, Opcode.PS_MSUB, Opcode.PS_NMADD, Opcode.PS_NMSUB,
})

# FP arithmetic / exception-raising ops that must not silently continue under
# traps_enabled until their trap policy is complete.
TRAP_DELIVERY_INCOMPLETE_OPS: frozenset[Opcode] = frozenset({
    Opcode.FRES, Opcode.FRSQRTE, Opcode.FRSP,
    Opcode.FCTIW, Opcode.FCTIWZ,
    Opcode.FCMPU, Opcode.FCMPO,
    Opcode.PS_MULS0, Opcode.PS_MULS1,
    Opcode.PS_DIV,
    Opcode.PS_MADDS0, Opcode.PS_MADDS1,
    Opcode.PS_SUM0, Opcode.PS_SUM1,
    Opcode.PS_RES, Opcode.PS_RSQRTE,
    Opcode.PS_CMPU0, Opcode.PS_CMPU1, Opcode.PS_CMPO0, Opcode.PS_CMPO1,
})

# ConcreteOps note: enabled exception was raised on this instruction even when
# the corresponding sticky was already set (FPSCR may be unchanged → no edge).
_fp_enabled_exception_reraise: ContextVar[bool] = ContextVar(
    "fp_enabled_exception_reraise", default=False,
)


@dataclass(frozen=True, slots=True)
class FPTrapDecision:
    """Concrete trap / writeback policy derived from an ``FPOutcome``."""

    trap: bool
    writeback: bool
    enabled_exception: bool
    incomplete_reason: str | None = None

    @property
    def supported(self) -> bool:
        return self.incomplete_reason is None


def trap_delivery_supported(opcode: Opcode) -> bool:
    return opcode in TRAP_DELIVERY_SUPPORTED_OPS or opcode in TRAP_DELIVERY_PAIRED_OPS


def trap_delivery_paired(opcode: Opcode) -> bool:
    return opcode in TRAP_DELIVERY_PAIRED_OPS


def trap_delivery_incomplete(opcode: Opcode) -> bool:
    return opcode in TRAP_DELIVERY_INCOMPLETE_OPS


def is_fp_trap_cfg_opcode(opcode: Opcode) -> bool:
    """Opcodes that participate in traps_enabled CFG forking / fail-closed."""
    return trap_delivery_supported(opcode) or trap_delivery_incomplete(opcode)


def ox_ux_xx_enables_set(fpscr: int) -> bool:
    return bool(int(fpscr) & FPSCR_OX_UX_XX_ENABLES)


def clear_fp_enabled_exception_reraise() -> None:
    """Reset the per-instruction ConcreteOps re-raise note (CFG step entry)."""
    _fp_enabled_exception_reraise.set(False)


def note_fp_enabled_exception_reraise() -> None:
    """Record that this instruction raised an enabled exception (ConcreteOps)."""
    _fp_enabled_exception_reraise.set(True)


def consume_fp_enabled_exception_reraise() -> bool:
    """Read and clear the ConcreteOps re-raise note."""
    flagged = _fp_enabled_exception_reraise.get()
    _fp_enabled_exception_reraise.set(False)
    return bool(flagged)


def enable_bit_for_exception_mask(exception_mask: int) -> int:
    """Map a sticky / VX-subcause mask to its FPSCR enable bit."""
    mask = int(exception_mask)
    if mask & FPSCR_ZX:
        return FPSCR_ZE
    if mask & FPSCR_OX:
        return FPSCR_OE
    if mask & FPSCR_UX:
        return FPSCR_UE
    if mask & FPSCR_XX:
        return FPSCR_XE
    # VX summary or any VX* subcause → VE
    if mask & (FPSCR_VX | FPSCR_VX_SUBCAUSES):
        return FPSCR_VE
    return 0


def ensure_fp_trap_delivery_supported(opcode: Opcode, fpscr: Any = 0) -> None:
    """Fail closed when trap semantics are incomplete for this opcode.

    ``fpscr`` is accepted for API compatibility; OE/UE/XE are modeled for the
    scalar SoftFloat family on ConcreteOps. SymbolicOps keeps OE/UE/XE clear
    via the domain constraint path in ``semantics``.
    """
    del fpscr  # reserved for future per-enable gating
    if trap_delivery_incomplete(opcode):
        raise ExecutionInconclusive(
            f"FP trap delivery incomplete for opcode {opcode.value}",
        )
    if not trap_delivery_supported(opcode):
        raise ExecutionInconclusive(
            f"FP trap delivery not modeled for opcode {opcode.value}",
        )


def exception_flags_match_enables(
    flags: FPExceptionFlags,
    fpscr: int,
) -> bool:
    """True when outcome sticky indicators intersect VE/ZE/OE/UE/XE enables."""
    fpscr = int(fpscr)
    if bool(flags.invalid) and (fpscr & FPSCR_VE):
        return True
    if bool(flags.divide_by_zero) and (fpscr & FPSCR_ZE):
        return True
    if bool(flags.overflow) and (fpscr & FPSCR_OE):
        return True
    if bool(flags.underflow) and (fpscr & FPSCR_UE):
        return True
    if bool(flags.inexact) and (fpscr & FPSCR_XE):
        return True
    return False


def resolve_fp_trap_policy(
    outcome: FPOutcome,
    fpscr: int,
    *,
    paired: bool = False,
) -> FPTrapDecision:
    """FPOutcome → sticky already implied → inspect enables → trap decision.

    Scalar: enabled exception suppresses destination writeback (Broadway).
    Paired: writeback stays unconditional; trap when enables match lane flags.
    """
    if not outcome.supported:
        reason = outcome.unsupported_reason or "unsupported FPOutcome"
        return FPTrapDecision(
            trap=False,
            writeback=False,
            enabled_exception=False,
            incomplete_reason=reason,
        )

    enabled = exception_flags_match_enables(outcome.flags, fpscr)
    if int(outcome.invalid_cause or 0) and (int(fpscr) & FPSCR_VE):
        enabled = True

    if paired:
        return FPTrapDecision(
            trap=enabled,
            writeback=True,
            enabled_exception=enabled,
            incomplete_reason=None,
        )

    writeback = not enabled
    return FPTrapDecision(
        trap=enabled,
        writeback=writeback,
        enabled_exception=enabled,
        incomplete_reason=None,
    )


def outcome_with_trap_policy(
    outcome: FPOutcome,
    fpscr: int,
    *,
    paired: bool = False,
) -> FPOutcome:
    """Return a copy of ``outcome`` with ``trap`` / ``writeback`` filled in."""
    decision = resolve_fp_trap_policy(outcome, fpscr, paired=paired)
    if decision.incomplete_reason is not None and not outcome.supported:
        return outcome
    if decision.incomplete_reason is not None:
        from .fp_outcome import unsupported_outcome

        return unsupported_outcome(
            decision.incomplete_reason,
            result_bits=outcome.result_bits,
        )
    return replace(
        outcome,
        trap=decision.trap,
        writeback=decision.writeback,
    )


def sticky_mask_from_outcome(outcome: FPOutcome) -> int:
    """Broadway sticky / cause bits implied by a concrete ``FPOutcome``."""
    mask = int(outcome.invalid_cause or 0)
    flags = outcome.flags
    if bool(flags.invalid) and not mask:
        # Generic invalid without a VX subcause — VX summary alone is not a
        # sticky latch source; callers should supply invalid_cause.
        pass
    if bool(flags.divide_by_zero):
        mask |= FPSCR_ZX
    if bool(flags.overflow):
        mask |= FPSCR_OX
    if bool(flags.underflow):
        mask |= FPSCR_UX
    if bool(flags.inexact):
        mask |= FPSCR_XX
    return mask


def _newly_enabled_sticky_trap(pre_fpscr: Any, post_fpscr: Any, ops: Any) -> Any:
    """True when this instruction newly latched a sticky whose enable is set."""
    sticky = ops.const(FPSCR_STICKY_TRAP_MASK)
    pre_s = ops.band(pre_fpscr, sticky)
    post_s = ops.band(post_fpscr, sticky)
    newly = ops.band(post_s, ops.bnot(pre_s))

    # OX↔OE, UX↔UE, ZX↔ZE, XX↔XE (enables live in the low byte).
    ox = ops.land(
        ops.lnot(ops.eq(ops.band(newly, ops.const(FPSCR_OX)), ops.const(0))),
        ops.lnot(ops.eq(ops.band(post_fpscr, ops.const(FPSCR_OE)), ops.const(0))),
    )
    ux = ops.land(
        ops.lnot(ops.eq(ops.band(newly, ops.const(FPSCR_UX)), ops.const(0))),
        ops.lnot(ops.eq(ops.band(post_fpscr, ops.const(FPSCR_UE)), ops.const(0))),
    )
    zx = ops.land(
        ops.lnot(ops.eq(ops.band(newly, ops.const(FPSCR_ZX)), ops.const(0))),
        ops.lnot(ops.eq(ops.band(post_fpscr, ops.const(FPSCR_ZE)), ops.const(0))),
    )
    xx = ops.land(
        ops.lnot(ops.eq(ops.band(newly, ops.const(FPSCR_XX)), ops.const(0))),
        ops.lnot(ops.eq(ops.band(post_fpscr, ops.const(FPSCR_XE)), ops.const(0))),
    )
    # Any newly set VX* subcause with VE.
    vx = ops.land(
        ops.lnot(ops.eq(ops.band(newly, ops.const(FPSCR_VX_SUBCAUSES)), ops.const(0))),
        ops.lnot(ops.eq(ops.band(post_fpscr, ops.const(FPSCR_VE)), ops.const(0))),
    )
    return ops.lor(ox, ops.lor(ux, ops.lor(zx, ops.lor(xx, vx))))


def fp_trap_pending_from_fex_transition(
    pre_fpscr: Any,
    post_fpscr: Any,
    ops: Any,
) -> Any:
    """Trap when this instruction caused an enabled FP exception (Broadway).

    Combines:

    1. Newly latched sticky bits whose enable is set (covers FEX 0→1 and
       additional stickies while FEX was already set).
    2. ConcreteOps re-raise note when the same sticky was already set (FPSCR
       edge may be empty) — see ``note_fp_enabled_exception_reraise``.

    MSR FE0/FE1 imprecise modes remain deferred; ``traps_enabled`` assumes
    precise per-instruction delivery.
    """
    newly = _newly_enabled_sticky_trap(pre_fpscr, post_fpscr, ops)
    reraise = ops.bool(consume_fp_enabled_exception_reraise())
    return ops.lor(newly, reraise)


def deliver_fp_program_exception(
    state: Any,
    fault_pc: int,
    ops: Any,
    *,
    exception_entry,
) -> Any:
    """Apply program-exception entry state for an FP enabled exception.

    ``exception_entry`` is :func:`semantics._exception_entry` (injected to
    avoid an import cycle at module load).
    """
    return exception_entry(state, fault_pc, SRR1_FP_ENABLED_EXCEPTION, ops)


def supported_opcode_names() -> tuple[str, ...]:
    return tuple(sorted(op.value for op in TRAP_DELIVERY_SUPPORTED_OPS))


def paired_opcode_names() -> tuple[str, ...]:
    return tuple(sorted(op.value for op in TRAP_DELIVERY_PAIRED_OPS))


def incomplete_opcode_names() -> tuple[str, ...]:
    return tuple(sorted(op.value for op in TRAP_DELIVERY_INCOMPLETE_OPS))


def capability_tags_for_trap_domain(*, traps_enabled: bool = False) -> frozenset[str]:
    """Wave 4: demand ``fp-traps`` when the proof domain enables traps.

    Does not claim FE0/FE1 completeness — attestation remains incomplete.
    """
    if traps_enabled:
        return frozenset({"fp-traps"})
    return frozenset()


def fe0_fe1_modeling_status() -> dict[str, bool]:
    """Document FE0/FE1 incompleteness for capability-assurance ledgers."""
    return {
        "fe0": False,
        "fe1": False,
        "precise_delivery_assumed_under_traps_enabled": True,
        "imprecise_modes_modeled": False,
    }
