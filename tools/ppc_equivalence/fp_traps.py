"""PR18 floating-point exception trap delivery (Wave 5 Track D / Phase 8).

Pipeline for arithmetic ops that already produce :class:`FPOutcome` / SoftFloat
sticky updates:

```text
outcome / sticky raise → update sticky/cause → inspect enable bits
  → if enabled exception: suppress destination (scalar precise) + program-exception
    OR defer to pending FP state (imprecise / reserved FE0/FE1 under exact-v2)
  → else: continue
```

Reuses the existing exception exit vocabulary (``program-exception``, vector
``0x700``, ``_exception_entry`` SRR0/SRR1/MSR). Equivalence compares trap
versus continue via terminal ``exit_kind`` / ``exit_target``; imprecise delivery
also records ``MachineState.fp_pending_*`` for observability.

Broadway MSR FE0/FE1 trap-delivery modes (Book I; FE0=MSR[25], FE1=MSR[24],
Python LSB bit indices 6/7):

+------------------+------------------+------------------------------------------+
| FE0 | FE1        | Mode             | Modeled behavior (``SCALAR_FP_EXACT_V2``)|
+-----+------------+------------------+------------------------------------------+
|  0  |  0         | imprecise nonrec | Pending FP exception; writeback allowed; |
|     |            |                  | ``recoverability=False``                 |
|  0  |  1         | imprecise recov  | Pending FP exception; writeback allowed; |
|     |            |                  | ``recoverability=True``                  |
|  1  |  0         | precise          | Immediate program interrupt; scalar dest |
|     |            |                  | suppressed when enable matches           |
|  1  |  1         | reserved         | Pinned: deferred pending record (same as |
|     |            |                  | imprecise for delivery/writeback;        |
|     |            |                  | ``delivery_class=DEFERRED``)             |
+------------------+------------------+------------------------------------------+

Production (``SCALAR_FP_EXACT_V2=0``): ``traps_enabled`` keeps the legacy
precise-delivery assumption; ledger ``fe0_fe1`` stays false (fail-closed).

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
- Production ``SCALAR_FP_EXACT_V2=0``: MSR FE0/FE1 not consulted; precise
  delivery assumed when ``traps_enabled`` (ledger ``fe0_fe1: false``).

Still **Tier C** only; does not enable automatic promotion.
"""

from __future__ import annotations

from contextvars import ContextVar
from dataclasses import dataclass, replace
from enum import Enum
from typing import Any

from .fp_capabilities import scalar_fp_exact_v2_enabled
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

# Delivery-class tags stored on ``MachineState.fp_pending_delivery``.
FP_PENDING_NONE = 0
FP_PENDING_IMPRECIS = 1
FP_PENDING_DEFERRED = 2

# Broadway MSR FE0/FE1 (Book I: FE0=MSR[25], FE1=MSR[24], LSB bit indices 6/7).
MSR_FE0 = 1 << 6
MSR_FE1 = 1 << 7


class FE0Fe1Mode(str, Enum):
    """All four MSR FE0/FE1 trap-delivery modes (Phase 8 foundation)."""

    IMPRECISE_NONRECOVERABLE = "imprecise-nonrecoverable"  # FE0=0 FE1=0
    IMPRECISE_RECOVERABLE = "imprecise-recoverable"        # FE0=0 FE1=1
    PRECISE = "precise"                                     # FE0=1 FE1=0
    RESERVED = "reserved"                                   # FE0=1 FE1=1


class FPDeliveryClass(str, Enum):
    """Whether an enabled FP exception is delivered or deferred."""

    DELIVERED = "delivered"
    PENDING = "pending"
    DEFERRED = "deferred"


@dataclass(frozen=True, slots=True)
class PendingFPException:
    """Deferred FP program-interrupt state (Phase 8)."""

    cause_mask: int
    fault_pc: int
    recoverability: bool
    delivery_class: FPDeliveryClass


@dataclass(frozen=True, slots=True)
class FPTrapDeliveryPlan:
    """Concrete vs imprecise trap delivery under live FPSCR + MSR."""

    trap: bool
    writeback: bool
    enabled_exception: bool
    pending: PendingFPException | None = None
    incomplete_reason: str | None = None

    @property
    def supported(self) -> bool:
        return self.incomplete_reason is None

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


def msr_fe0_set(msr: int) -> bool:
    return bool(int(msr) & MSR_FE0)


def msr_fe1_set(msr: int) -> bool:
    return bool(int(msr) & MSR_FE1)


def classify_fe0_fe1_mode(msr: int) -> FE0Fe1Mode:
    """Classify MSR FE0/FE1 into one of the four Broadway delivery modes."""
    fe0 = msr_fe0_set(msr)
    fe1 = msr_fe1_set(msr)
    if fe0 and fe1:
        return FE0Fe1Mode.RESERVED
    if fe0:
        return FE0Fe1Mode.PRECISE
    if fe1:
        return FE0Fe1Mode.IMPRECISE_RECOVERABLE
    return FE0Fe1Mode.IMPRECISE_NONRECOVERABLE


def fpscr_trap_enables_set(fpscr: int) -> bool:
    """True when any VE/ZE/OE/UE/XE enable is set in ``fpscr``."""
    return bool(int(fpscr) & FPSCR_ANY_ENABLE)


def effective_traps_enabled(
    *,
    domain_traps_enabled: bool,
    fpscr: int,
    msr: int = 0,
) -> bool:
    """Derive whether trap delivery is active for this instruction.

    Production (``SCALAR_FP_EXACT_V2=0``): honor the external
    ``FloatingPointDomain.traps_enabled`` approximation unchanged.

    Exact-v2: additionally require live FPSCR enables; MSR FE0/FE1 selects
    precise delivery vs pending-event stubs via :func:`plan_fp_trap_delivery`.
    """
    if not domain_traps_enabled:
        return False
    if not scalar_fp_exact_v2_enabled():
        return True
    del msr  # mode consulted in plan_fp_trap_delivery
    return fpscr_trap_enables_set(fpscr)


def pending_fp_exception_from_causes(
    cause_mask: int,
    fault_pc: int,
    *,
    mode: FE0Fe1Mode,
) -> PendingFPException:
    """Build a pending FP exception record for imprecise / reserved modes."""
    recoverable = mode is FE0Fe1Mode.IMPRECISE_RECOVERABLE
    delivery = (
        FPDeliveryClass.DEFERRED
        if mode is FE0Fe1Mode.RESERVED
        else FPDeliveryClass.PENDING
    )
    return PendingFPException(
        cause_mask=int(cause_mask),
        fault_pc=int(fault_pc),
        recoverability=recoverable,
        delivery_class=delivery,
    )


def plan_fp_trap_delivery(
    outcome: FPOutcome,
    fpscr: int,
    *,
    msr: int = 0,
    fault_pc: int = 0,
    domain_traps_enabled: bool = False,
    paired: bool = False,
) -> FPTrapDeliveryPlan:
    """Resolve trap delivery using FPSCR enables and MSR FE0/FE1 when exact-v2.

    When ``SCALAR_FP_EXACT_V2=0``, this reduces to
    :func:`resolve_fp_trap_policy` (precise per-instruction delivery assumed).
    """
    if not effective_traps_enabled(
        domain_traps_enabled=domain_traps_enabled,
        fpscr=fpscr,
        msr=msr,
    ):
        decision = resolve_fp_trap_policy(outcome, fpscr, paired=paired)
        return FPTrapDeliveryPlan(
            trap=False,
            writeback=decision.writeback,
            enabled_exception=decision.enabled_exception,
            incomplete_reason=decision.incomplete_reason,
        )

    decision = resolve_fp_trap_policy(outcome, fpscr, paired=paired)
    if decision.incomplete_reason is not None:
        return FPTrapDeliveryPlan(
            trap=decision.trap,
            writeback=decision.writeback,
            enabled_exception=decision.enabled_exception,
            incomplete_reason=decision.incomplete_reason,
        )

    if not decision.enabled_exception:
        return FPTrapDeliveryPlan(
            trap=False,
            writeback=decision.writeback,
            enabled_exception=False,
        )

    if not scalar_fp_exact_v2_enabled():
        return FPTrapDeliveryPlan(
            trap=decision.trap,
            writeback=decision.writeback,
            enabled_exception=True,
        )

    mode = classify_fe0_fe1_mode(msr)
    cause_mask = sticky_mask_from_outcome(outcome)
    if mode is FE0Fe1Mode.PRECISE:
        return FPTrapDeliveryPlan(
            trap=True,
            writeback=decision.writeback,
            enabled_exception=True,
        )

    pending = pending_fp_exception_from_causes(
        cause_mask,
        fault_pc,
        mode=mode,
    )
    return FPTrapDeliveryPlan(
        trap=False,
        writeback=decision.writeback if paired else True,
        enabled_exception=True,
        pending=pending,
    )


def precise_trap_delivery_for_msr(msr: Any, ops: Any) -> Any:
    """Symbolic/concrete condition: FE0=1 FE1=0 → immediate trap delivery."""
    fe0 = ops.lnot(ops.eq(ops.band(msr, ops.const(MSR_FE0)), ops.const(0)))
    fe1 = ops.lnot(ops.eq(ops.band(msr, ops.const(MSR_FE1)), ops.const(0)))
    return ops.land(fe0, ops.lnot(fe1))


def imprecise_writeback_allowed(msr: Any, ops: Any) -> Any:
    """True when scalar destination writeback is allowed despite enables (exact-v2)."""
    return ops.lnot(precise_trap_delivery_for_msr(msr, ops))


def sticky_cause_mask_from_fpscr_transition(pre_fpscr: Any, post_fpscr: Any, ops: Any) -> Any:
    """Newly latched sticky / VX subcause bits from an instruction FPSCR update."""
    sticky = ops.const(FPSCR_STICKY_TRAP_MASK)
    pre_s = ops.band(pre_fpscr, sticky)
    post_s = ops.band(post_fpscr, sticky)
    return ops.band(post_s, ops.bnot(pre_s))


def fp_outcome_from_fpscr_transition(
    pre_fpscr: int,
    post_fpscr: int,
    *,
    paired: bool = False,
) -> FPOutcome:
    """Build a minimal ``FPOutcome`` from a concrete FPSCR edge (CFG replay)."""
    from .fp_outcome import exception_flags, outcome_from_result_bits

    newly = int(post_fpscr) & FPSCR_STICKY_TRAP_MASK
    newly &= ~int(pre_fpscr) & FPSCR_STICKY_TRAP_MASK
    flags = exception_flags(
        invalid=bool(newly & FPSCR_VX_SUBCAUSES),
        divide_by_zero=bool(newly & FPSCR_ZX),
        overflow=bool(newly & FPSCR_OX),
        underflow=bool(newly & FPSCR_UX),
        inexact=bool(newly & FPSCR_XX),
    )
    outcome = outcome_from_result_bits(
        0,
        flags=flags,
        invalid_cause=newly & FPSCR_VX_SUBCAUSES,
    )
    decision = resolve_fp_trap_policy(outcome, int(post_fpscr), paired=paired)
    return replace(outcome, trap=decision.trap, writeback=decision.writeback)


def pending_fp_exception_for_mode(
    cause_mask: Any,
    fault_pc: Any,
    msr: Any,
    ops: Any,
) -> tuple[Any, Any, Any, Any]:
    """Return ``(cause, fault_pc, recoverable, delivery)`` for ``MachineState``."""
    if isinstance(cause_mask, int) and isinstance(msr, int):
        pending = pending_fp_exception_from_causes(
            int(cause_mask),
            int(fault_pc),
            mode=classify_fe0_fe1_mode(msr),
        )
        delivery_tag = (
            FP_PENDING_DEFERRED
            if pending.delivery_class is FPDeliveryClass.DEFERRED
            else FP_PENDING_IMPRECIS
        )
        return (
            ops.const(pending.cause_mask),
            ops.const(pending.fault_pc),
            ops.bool(pending.recoverability),
            ops.const(delivery_tag),
        )

    fe0 = ops.lnot(ops.eq(ops.band(msr, ops.const(MSR_FE0)), ops.const(0)))
    fe1 = ops.lnot(ops.eq(ops.band(msr, ops.const(MSR_FE1)), ops.const(0)))
    recoverable = ops.land(ops.lnot(fe0), fe1)
    reserved = ops.land(fe0, fe1)
    delivery = ops.ite(
        reserved,
        ops.const(FP_PENDING_DEFERRED),
        ops.const(FP_PENDING_IMPRECIS),
    )
    return cause_mask, fault_pc, recoverable, delivery


def apply_fp_pending_to_state(
    state: Any,
    *,
    cause_mask: Any,
    fault_pc: Any,
    msr: Any,
    ops: Any,
    active: Any,
) -> Any:
    """Set or clear ``MachineState`` pending FP fields when ``active`` holds."""
    cause, pc, recoverable, delivery = pending_fp_exception_for_mode(
        cause_mask, fault_pc, msr, ops,
    )
    return replace(
        state,
        fp_pending_cause=ops.ite(active, cause, ops.const(0)),
        fp_pending_fault_pc=ops.ite(active, pc, ops.const(0)),
        fp_pending_recoverable=ops.ite(active, recoverable, ops.bool(False)),
        fp_pending_delivery=ops.ite(active, delivery, ops.const(FP_PENDING_NONE)),
    )


def cfg_fp_trap_branches(
    state: Any,
    pre_fpscr: Any,
    post_fpscr: Any,
    fault_pc: int,
    ops: Any,
    *,
    domain_traps_enabled: bool,
    msr: Any,
    paired: bool = False,
) -> tuple[Any, Any, Any]:
    """CFG trap path: ``(enabled, trap_now, pending_active)`` conditions."""
    enabled = fp_trap_pending_from_fex_transition(pre_fpscr, post_fpscr, ops)
    if not scalar_fp_exact_v2_enabled():
        return enabled, enabled, ops.bool(False)
    if not effective_traps_enabled(
        domain_traps_enabled=domain_traps_enabled,
        fpscr=post_fpscr if isinstance(post_fpscr, int) else 0,
        msr=msr if isinstance(msr, int) else 0,
    ):
        return enabled, ops.bool(False), ops.bool(False)

    precise = precise_trap_delivery_for_msr(msr, ops)
    trap_now = ops.land(enabled, precise)
    pending_active = ops.land(enabled, ops.lnot(precise))
    return enabled, trap_now, pending_active


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

    MSR FE0/FE1 imprecise/reserved modes defer delivery; precise mode traps
    immediately. Legacy path (``SCALAR_FP_EXACT_V2=0``) treats every enabled
    exception as precise.
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


def fe0_fe1_modeling_status() -> dict[str, bool | str]:
    """Document FE0/FE1 progress for capability-assurance ledgers.

    Production (``SCALAR_FP_EXACT_V2`` off) remains fail-closed: ``fe0`` /
    ``fe1`` stay false and precise delivery is still assumed under
    ``traps_enabled``. Experimental v2 wires live FPSCR enables, all four
    MSR modes, CFG trap/pending branching, and imprecise writeback when the
    module flag / env is enabled.
    """
    v2 = scalar_fp_exact_v2_enabled()
    imprecise_wired = v2
    return {
        # Ledger completeness bits: true only on the experimental exact-v2 path.
        "fe0": v2,
        "fe1": v2,
        "modes_documented": True,
        "imprecise_modes_modeled": imprecise_wired,
        "precise_mode_delivered": v2,
        "imprecise_nonrecoverable_pending_stub": False,
        "imprecise_recoverable_pending_stub": False,
        "reserved_mode_pending_stub": False,
        "precise_delivery_assumed_under_traps_enabled": not v2,
        "live_fpscr_enables_when_exact_v2": v2,
        "delivery_class": "fe0-fe1-v2" if v2 else "legacy-precise-assumption",
    }


def traps_v2_ledger_dimensions() -> dict[str, bool]:
    """Honest ``fp-traps`` ledger dimensions for the active modeling tier."""
    status = fe0_fe1_modeling_status()
    v2 = bool(status["fe0"])
    return {
        "ve_ze_oe_ue_xe": False,
        "destination_suppression": False,
        "srr0_srr1": False,
        "fex_reraise": False,
        "fe0_fe1": v2 and bool(status["imprecise_modes_modeled"]),
        "traps": v2 and bool(status["precise_mode_delivered"]),
    }
