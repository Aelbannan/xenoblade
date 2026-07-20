"""PR18 floating-point exception trap delivery scaffold (Wave 4 Track D).

Pipeline for arithmetic ops that already produce :class:`FPOutcome`:

```text
FPOutcome → update sticky/cause → inspect enable bits
  → if enabled exception: suppress destination + program-exception terminal
  → else: continue
```

Reuses the existing exception exit vocabulary (``program-exception``, vector
``0x700``, ``_exception_entry`` SRR0/SRR1/MSR). Equivalence already compares
trap versus continue via terminal ``exit_kind`` / ``exit_target``.

**Scaffold limits (fail closed):**

- VE (invalid) and ZE (divide-by-zero) only; OE/UE/XE must be clear.
- Scalar exception-arith + fused opcodes that already model VE/ZE sticky and
  destination suppression.
- Paired-single, estimates, compares, conversions, and OX/UX/XX traps remain
  incomplete → ``ExecutionInconclusive`` when ``traps_enabled``.
- Full NI-aware trap interaction may depend on PR17.

Still **Tier C** only; does not enable automatic promotion.
"""

from __future__ import annotations

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
FPSCR_OX_UX_XX_ENABLES = FPSCR_OE | FPSCR_UE | FPSCR_XE
FPSCR_VE_ZE_ENABLES = FPSCR_VE | FPSCR_ZE

# Scalar ops with modeled VE/ZE sticky + destination suppression (FPOutcome /
# SoftFloat oracle family on ConcreteOps; host/Z3 path shares the same
# exception-raise logic in semantics).
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

# FP arithmetic / exception-raising ops that must not silently continue under
# traps_enabled until their trap policy is complete.
TRAP_DELIVERY_INCOMPLETE_OPS: frozenset[Opcode] = frozenset({
    Opcode.FRES, Opcode.FRSQRTE, Opcode.FRSP,
    Opcode.FCTIW, Opcode.FCTIWZ,
    Opcode.FCMPU, Opcode.FCMPO,
    Opcode.PS_ADD, Opcode.PS_SUB, Opcode.PS_MUL,
    Opcode.PS_MULS0, Opcode.PS_MULS1,
    Opcode.PS_DIV,
    Opcode.PS_MADD, Opcode.PS_MSUB, Opcode.PS_NMADD, Opcode.PS_NMSUB,
    Opcode.PS_MADDS0, Opcode.PS_MADDS1,
    Opcode.PS_SUM0, Opcode.PS_SUM1,
    Opcode.PS_RES, Opcode.PS_RSQRTE,
    Opcode.PS_CMPU0, Opcode.PS_CMPU1, Opcode.PS_CMPO0, Opcode.PS_CMPO1,
})


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
    return opcode in TRAP_DELIVERY_SUPPORTED_OPS


def trap_delivery_incomplete(opcode: Opcode) -> bool:
    return opcode in TRAP_DELIVERY_INCOMPLETE_OPS


def is_fp_trap_cfg_opcode(opcode: Opcode) -> bool:
    """Opcodes that participate in traps_enabled CFG forking / fail-closed."""
    return trap_delivery_supported(opcode) or trap_delivery_incomplete(opcode)


def ox_ux_xx_enables_set(fpscr: int) -> bool:
    return bool(int(fpscr) & FPSCR_OX_UX_XX_ENABLES)


def ensure_fp_trap_delivery_supported(opcode: Opcode, fpscr: Any = 0) -> None:
    """Fail closed when trap semantics are incomplete for this opcode / enables.

    ``fpscr`` may be concrete ``int``. Symbolic FPSCR OE/UE/XE bits are forced
    clear by the domain constraint path when ``traps_enabled``.
    """
    if trap_delivery_incomplete(opcode):
        raise ExecutionInconclusive(
            f"FP trap delivery incomplete for opcode {opcode.value}",
        )
    if not trap_delivery_supported(opcode):
        raise ExecutionInconclusive(
            f"FP trap delivery not modeled for opcode {opcode.value}",
        )
    try:
        concrete = int(fpscr)
    except TypeError:
        return
    if ox_ux_xx_enables_set(concrete):
        raise ExecutionInconclusive(
            "FP OX/UX/XX trap delivery is not modeled; OE/UE/XE must be clear",
        )


def exception_flags_match_enables(
    flags: FPExceptionFlags,
    fpscr: int,
) -> bool:
    """True when outcome sticky indicators intersect VE/ZE enables."""
    fpscr = int(fpscr)
    if bool(flags.invalid) and (fpscr & FPSCR_VE):
        return True
    if bool(flags.divide_by_zero) and (fpscr & FPSCR_ZE):
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
    Paired: writeback stays unconditional; trap delivery itself is incomplete.
    """
    if not outcome.supported:
        reason = outcome.unsupported_reason or "unsupported FPOutcome"
        return FPTrapDecision(
            trap=False,
            writeback=False,
            enabled_exception=False,
            incomplete_reason=reason,
        )
    if paired:
        return FPTrapDecision(
            trap=False,
            writeback=True,
            enabled_exception=False,
            incomplete_reason="paired-single FP trap delivery is not modeled",
        )
    if ox_ux_xx_enables_set(fpscr):
        return FPTrapDecision(
            trap=False,
            writeback=True,
            enabled_exception=False,
            incomplete_reason=(
                "FP OX/UX/XX trap delivery is not modeled; OE/UE/XE must be clear"
            ),
        )
    # Overflow / underflow / inexact enables are out of scope; flags may be set
    # on the SoftFloat outcome but sticky latch for OX/UX/XX is incomplete.
    if (
        (bool(outcome.flags.overflow) and (int(fpscr) & FPSCR_OE))
        or (bool(outcome.flags.underflow) and (int(fpscr) & FPSCR_UE))
        or (bool(outcome.flags.inexact) and (int(fpscr) & FPSCR_XE))
    ):
        return FPTrapDecision(
            trap=False,
            writeback=True,
            enabled_exception=False,
            incomplete_reason="FP OX/UX/XX trap delivery is not modeled",
        )

    enabled = exception_flags_match_enables(outcome.flags, fpscr)
    if int(outcome.invalid_cause or 0) and (int(fpscr) & FPSCR_VE):
        enabled = True
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


def fp_trap_pending_from_fex_transition(
    pre_fpscr: Any,
    post_fpscr: Any,
    ops: Any,
) -> Any:
    """Scaffold trap condition: FEX transitions 0 → 1 on this instruction.

    Matches VE/ZE delivery when proofs start with FEX clear (domain default).
    Re-trapping when FEX was already set is a documented full-PR18 blocker.
    """
    fex = ops.const(FPSCR_FEX)
    was_set = ops.eq(ops.band(pre_fpscr, fex), fex)
    now_set = ops.eq(ops.band(post_fpscr, fex), fex)
    return ops.land(ops.lnot(was_set), now_set)


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


def incomplete_opcode_names() -> tuple[str, ...]:
    return tuple(sorted(op.value for op in TRAP_DELIVERY_INCOMPLETE_OPS))
