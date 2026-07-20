"""Exact Broadway FP convert ops (Phase 6 — ``frsp``, ``fctiw``, ``fctiwz``).

All four RN modes for ``frsp`` / ``fctiw``; ``fctiwz`` uses RTZ. FI/FR/XX,
NaN/Inf/overflow, VXCVI, and VE writeback suppression hooks — no host
``float``.
"""

from __future__ import annotations

from fractions import Fraction

from .fp_bits import FpClass, classify_binary64, decode_binary64, mask64
from .fp_exact_outcome import FiFrPolicy, ScalarFPOutcome
from .fp_exact_util import (
    _NEGATIVE_INT_LIMIT,
    _POSITIVE_INT_LIMIT,
    apply_ni_single_result,
    binary64_to_fraction,
    exact_v2_enabled,
    fctiw_integer_result_bits,
    fprf_from_binary64,
    is_snan_binary64,
    quiet_nan_binary64,
    rounding_mode_from_fpscr,
    unsupported_exact_outcome,
)
from .fp_fpscr import FPSCR_OX, FPSCR_UX, FPSCR_VE, FPSCR_VXCVI, FPSCR_VXSNAN, FPSCR_XX
from .fp_oracle import _single_to_fpr_bits
from .fp_round import (
    BINARY32_FRAC_BITS,
    BINARY64_FRAC_BITS,
    FORMAT_BINARY32,
    round_to_format,
)
from .fp_rounding import RoundingMode


def _round_fraction_to_integer(value: Fraction, mode: RoundingMode) -> Fraction:
    """Round an exact rational to an integer under ``mode`` (no host float)."""
    if value == 0:
        return Fraction(0)
    sign = value < 0
    magnitude = abs(value)
    floor_val = Fraction(magnitude.numerator // magnitude.denominator)

    frac_part = magnitude - floor_val
    if frac_part == 0:
        return -floor_val if sign else floor_val

    if mode is RoundingMode.TOWARD_ZERO:
        rounded = floor_val
    elif mode is RoundingMode.TOWARD_PLUS_INFINITY:
        rounded = floor_val if sign else floor_val + 1
    elif mode is RoundingMode.TOWARD_MINUS_INFINITY:
        rounded = floor_val + 1 if sign else floor_val
    else:
        twice = frac_part * 2
        if twice > 1:
            increment = 1
        elif twice < 1:
            increment = 0
        else:
            increment = int(floor_val) & 1
        rounded = floor_val + increment
    return -rounded if sign else rounded


def _fraction_out_of_positive_range(rounded: Fraction) -> bool:
    return rounded >= Fraction(2**31, 1)


def _fraction_out_of_negative_range(rounded: Fraction) -> bool:
    limit = binary64_to_fraction(_NEGATIVE_INT_LIMIT)
    return limit is not None and rounded < limit


def exact_frsp(
    source_bits: int,
    *,
    fpscr: int = 0,
    ve_trap_suppresses: bool | None = None,
) -> ScalarFPOutcome:
    """Round binary64 ``frB`` to binary32 in FPR (``frsp``)."""
    if not exact_v2_enabled():
        return unsupported_exact_outcome("SCALAR_FP_EXACT_V2 disabled")

    source = mask64(source_bits)
    mode = rounding_mode_from_fpscr(fpscr)
    raised = 0
    invalid = False
    fi = False
    fr = False
    kind = classify_binary64(source)

    if is_snan_binary64(source):
        raised |= FPSCR_VXSNAN
        invalid = True
        result = quiet_nan_binary64(source)
    elif kind is FpClass.QNAN:
        result = quiet_nan_binary64(source)
    elif kind is FpClass.INFINITY:
        sign, exp, frac = decode_binary64(source)
        result = mask64(_single_to_fpr_bits(((sign << 31) | (0xFF << 23))))
    elif kind is FpClass.ZERO or kind is FpClass.SUBNORMAL or kind is FpClass.NORMAL:
        sign, exp, frac = decode_binary64(source)
        sig = frac if exp == 0 else (1 << BINARY64_FRAC_BITS) | frac
        unbiased = (exp - 1023) if exp else (1 - 1023)
        adjusted_unbiased = unbiased - (BINARY64_FRAC_BITS - BINARY32_FRAC_BITS)
        rounded = round_to_format(
            sign, adjusted_unbiased, sig, FORMAT_BINARY32, mode,
        )
        result = mask64(_single_to_fpr_bits(rounded.bits))
        fi = rounded.fi
        fr = rounded.fr
        if rounded.overflow:
            raised |= FPSCR_OX
        if rounded.underflow:
            raised |= FPSCR_UX
        if rounded.inexact:
            raised |= FPSCR_XX
    else:
        result = quiet_nan_binary64(source)

    result = apply_ni_single_result(result, fpscr)
    fprf = fprf_from_binary64(result)
    ve = bool(int(fpscr) & FPSCR_VE)
    suppress = bool(ve_trap_suppresses) if ve_trap_suppresses is not None else (invalid and ve)
    return ScalarFPOutcome(
        result_bits=result,
        raised_causes=raised,
        fi=fi,
        fr=fr,
        fi_fr_policy=FiFrPolicy.CLEAR,
        fprf=fprf,
        writeback=not suppress,
        supported=True,
    )


def _exact_fctiw_core(
    source_bits: int,
    mode: RoundingMode,
    *,
    fpscr: int = 0,
    ve_trap_suppresses: bool | None = None,
) -> ScalarFPOutcome:
    source = mask64(source_bits)
    raised = 0
    invalid = False

    if is_snan_binary64(source):
        raised |= FPSCR_VXSNAN
        invalid = True

    value = binary64_to_fraction(source)
    if value is None:
        invalid = True
        int_value = 0x80000000
        negative_zero = False
        inexact = False
        fi = False
        fr = False
    else:
        rounded = _round_fraction_to_integer(value, mode)
        positive_invalid = _fraction_out_of_positive_range(rounded)
        negative_invalid = _fraction_out_of_negative_range(rounded)
        range_invalid = positive_invalid or negative_invalid
        invalid = invalid or range_invalid

        if range_invalid:
            raised |= FPSCR_VXCVI

        if positive_invalid:
            int_value = 0x7FFFFFFF
        elif negative_invalid:
            int_value = 0x80000000
        else:
            int_value = int(rounded) & 0xFFFFFFFF

        inexact = (not range_invalid) and (value != rounded)
        if inexact:
            raised |= FPSCR_XX
        rounded_away = abs(rounded) > abs(value)
        fi = inexact
        fr = inexact and rounded_away
        negative_zero = int_value == 0 and value < 0 and rounded == Fraction(0)

    result = fctiw_integer_result_bits(int_value, negative_zero=negative_zero)
    ve = bool(int(fpscr) & FPSCR_VE)
    suppress = bool(ve_trap_suppresses) if ve_trap_suppresses is not None else (invalid and ve)
    return ScalarFPOutcome(
        result_bits=result,
        raised_causes=raised,
        fi=fi,
        fr=fr,
        fi_fr_policy=FiFrPolicy.CLEAR,
        fprf=0,
        update_fprf=False,
        writeback=not suppress,
        supported=True,
    )


def exact_fctiw(
    source_bits: int,
    *,
    fpscr: int = 0,
    ve_trap_suppresses: bool | None = None,
) -> ScalarFPOutcome:
    """Convert binary64 to signed 32-bit integer in FPR (``fctiw``)."""
    if not exact_v2_enabled():
        return unsupported_exact_outcome("SCALAR_FP_EXACT_V2 disabled")
    mode = rounding_mode_from_fpscr(fpscr)
    return _exact_fctiw_core(
        source_bits, mode, fpscr=fpscr, ve_trap_suppresses=ve_trap_suppresses,
    )


def exact_fctiwz(
    source_bits: int,
    *,
    fpscr: int = 0,
    ve_trap_suppresses: bool | None = None,
) -> ScalarFPOutcome:
    """Convert binary64 to signed 32-bit integer with RTZ (``fctiwz``)."""
    if not exact_v2_enabled():
        return unsupported_exact_outcome("SCALAR_FP_EXACT_V2 disabled")
    return _exact_fctiw_core(
        source_bits,
        RoundingMode.TOWARD_ZERO,
        fpscr=fpscr,
        ve_trap_suppresses=ve_trap_suppresses,
    )


__all__ = [
    "exact_fctiw",
    "exact_fctiwz",
    "exact_frsp",
]
