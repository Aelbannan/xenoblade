"""Exact scalar FP outcome type for SCALAR_FP_V2 (Phase 4+).

``ScalarFPOutcome`` is the bit-exact state-transition result described in
``SCALAR_FP_V2.md``. Production ``ConcreteOps`` / SoftFloat paths still use
``fp_outcome.FPOutcome`` and the ContextVar side channel until Phase 12; this
module is the canonical shape for centralized FPSCR transitions and future exact
oracle integration behind ``SCALAR_FP_EXACT_V2``.
"""

from __future__ import annotations

from dataclasses import dataclass
from enum import Enum
from typing import Any

from .fp_oracle import FpOracleFlags, FpOracleResult, mask64
from .fp_outcome import FPExceptionFlags, FPOutcome


class FiFrPolicy(str, Enum):
    """How an instruction updates FPSCR.FI/FR relative to the pre-FPSCR value."""

    PRESERVE = "preserve"
    CLEAR = "clear"
    SET = "set"


@dataclass(frozen=True)
class FPExceptionEvent:
    """Optional trap / program-exception delivery (Phase 8)."""

    kind: str
    enabled_mask: int = 0


@dataclass(frozen=True)
class ScalarFPOutcome:
    """Shared exact scalar-FP operation outcome (architecture v2)."""

    result_bits: int
    raised_causes: int = 0
    fi: bool = False
    fr: bool = False
    fi_fr_policy: FiFrPolicy = FiFrPolicy.PRESERVE
    fprf: int = 0
    update_fprf: bool = True
    writeback: bool = True
    exception: FPExceptionEvent | None = None
    supported: bool = True
    unsupported_reason: str | None = None

    def primary_bits(self) -> int:
        return mask64(self.result_bits)


def raised_causes_from_flags(
    flags: FPExceptionFlags | FpOracleFlags,
    *,
    invalid_cause: int = 0,
) -> int:
    """Map IEEE sticky flags + Broadway VX subcauses to FPSCR cause bits."""
    from .fp_fpscr import (
        FPSCR_OX,
        FPSCR_UX,
        FPSCR_XX,
        FPSCR_ZX,
    )

    causes = int(invalid_cause or 0)
    if bool(flags.invalid):
        # VX summary is derived later; subcauses live in invalid_cause.
        pass
    if bool(flags.overflow):
        causes |= FPSCR_OX
    if bool(flags.underflow):
        causes |= FPSCR_UX
    if bool(flags.divide_by_zero):
        causes |= FPSCR_ZX
    if bool(flags.inexact):
        causes |= FPSCR_XX
    return causes


def scalar_outcome_from_oracle(
    result: FpOracleResult,
    *,
    invalid_cause: int = 0,
    fi_fr_policy: FiFrPolicy = FiFrPolicy.CLEAR,
    fi: bool = False,
    fr: bool = False,
    writeback: bool = True,
) -> ScalarFPOutcome:
    """Lift a SoftFloat oracle result into ``ScalarFPOutcome``."""
    return ScalarFPOutcome(
        result_bits=mask64(result.bits64),
        raised_causes=raised_causes_from_flags(result.flags, invalid_cause=invalid_cause),
        fi=fi or bool(result.flags.inexact),
        fr=fr,
        fi_fr_policy=fi_fr_policy,
        fprf=int(result.fprf),
        writeback=writeback,
        supported=True,
    )


def scalar_outcome_from_fp_outcome(
    outcome: FPOutcome,
    *,
    fi_fr_policy: FiFrPolicy = FiFrPolicy.PRESERVE,
    fi: bool | None = None,
    fr: bool | None = None,
) -> ScalarFPOutcome:
    """Project the unified ``FPOutcome`` scaffold into ``ScalarFPOutcome``."""
    if not outcome.result_bits:
        raise ValueError("FPOutcome.result_bits is empty")
    bits = outcome.result_bits[0]
    if not isinstance(bits, int):
        raise TypeError(
            "scalar_outcome_from_fp_outcome requires concrete int result_bits "
            f"(got {type(bits).__name__})",
        )
    fprf = outcome.fprf
    if not isinstance(fprf, int):
        raise TypeError(
            "scalar_outcome_from_fp_outcome requires concrete int fprf "
            f"(got {type(fprf).__name__})",
        )
    resolved_fi = bool(fi) if fi is not None else bool(outcome.flags.inexact)
    resolved_fr = bool(fr) if fr is not None else False
    return ScalarFPOutcome(
        result_bits=mask64(bits),
        raised_causes=raised_causes_from_flags(
            outcome.flags,
            invalid_cause=int(outcome.invalid_cause or 0),
        ),
        fi=resolved_fi,
        fr=resolved_fr,
        fi_fr_policy=fi_fr_policy,
        fprf=fprf,
        writeback=bool(outcome.writeback),
        supported=bool(outcome.supported),
        unsupported_reason=outcome.unsupported_reason,
    )


def fp_outcome_from_scalar_outcome(scalar: ScalarFPOutcome) -> FPOutcome:
    """Down-convert ``ScalarFPOutcome`` for legacy ``FPOutcome`` consumers."""
    from .fp_outcome import clear_exception_flags, exception_flags, outcome_from_result_bits

    flags = clear_exception_flags()
    causes = int(scalar.raised_causes)
    from .fp_fpscr import FPSCR_OX, FPSCR_UX, FPSCR_XX, FPSCR_VX_ANY, FPSCR_ZX

    if causes & FPSCR_VX_ANY:
        flags = exception_flags(
            invalid=True,
            overflow=bool(causes & FPSCR_OX),
            underflow=bool(causes & FPSCR_UX),
            divide_by_zero=bool(causes & FPSCR_ZX),
            inexact=bool(causes & FPSCR_XX),
        )
    else:
        flags = exception_flags(
            overflow=bool(causes & FPSCR_OX),
            underflow=bool(causes & FPSCR_UX),
            divide_by_zero=bool(causes & FPSCR_ZX),
            inexact=bool(causes & FPSCR_XX),
        )
    return outcome_from_result_bits(
        scalar.primary_bits(),
        flags=flags,
        invalid_cause=causes & FPSCR_VX_ANY,
        fprf=scalar.fprf,
        writeback=scalar.writeback,
        supported=scalar.supported,
        unsupported_reason=scalar.unsupported_reason,
    )
