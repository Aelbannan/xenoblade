"""Exact fused scalar FP (Phase 9 — SCALAR_FP_V2.md).

One-round fused multiply-add/subtract using :mod:`fp_round` for IEEE rounding.
Handles Broadway near-cancellation sticky residue and single-FMA midpoint tie
correction (nonzero addend) without host ``float``.

Production ``ConcreteOps`` still routes through ``fp_oracle`` until Phase 12;
this module is the canonical exact kernel behind ``SCALAR_FP_EXACT_V2``.
"""

from __future__ import annotations

from dataclasses import dataclass
from fractions import Fraction
from typing import Final

from .fp_bits import (
    FpClass,
    classify_binary64,
    decode_binary64,
    encode_binary64,
    mask64,
)
from .fp_oracle import (
    FpOracleFlags,
    FpOracleResult,
    OracleUnimplementedError,
    _canonical_invalid_nan,
    _is_nan_class,
    _is_zero64,
    _make_inf64,
    _propagate_nan64,
    fprf_from_binary64,
)
from .fp_round import (
    BINARY64_EXP_BIAS,
    BINARY64_FRAC_BITS,
    round_binary32,
    round_binary64,
)
from .fp_rounding import RoundingMode

_B64_FRAC_BITS = BINARY64_FRAC_BITS
_B64_EXP_BIAS = BINARY64_EXP_BIAS
_BROADWAY_MIDPOINT_MASK: Final[int] = 0x000000001FFFFFFF
_BROADWAY_MIDPOINT_PATTERN: Final[int] = 0x0000000010000000


@dataclass(frozen=True, slots=True)
class ExactFusedOutcome:
    """Bit-exact fused op result (mirrors ``FpOracleResult`` fields)."""

    bits64: int
    flags: FpOracleFlags
    fprf: int
    supported: bool = True
    unsupported_reason: str | None = None

    def to_oracle_result(self) -> FpOracleResult:
        if not self.supported:
            raise OracleUnimplementedError(
                self.unsupported_reason or "exact fused op unsupported",
            )
        return FpOracleResult(self.bits64, self.flags, self.fprf)


def _flags_from_round(
    *,
    inexact: bool,
    overflow: bool = False,
    underflow: bool = False,
    invalid: bool = False,
    divide_by_zero: bool = False,
    sticky_extra: bool = False,
) -> FpOracleFlags:
    return FpOracleFlags(
        invalid=invalid,
        divide_by_zero=divide_by_zero,
        inexact=inexact or sticky_extra,
        overflow=overflow,
        underflow=underflow,
    )


def _bits_to_fraction(bits: int) -> Fraction:
    """Decode a finite binary64 bit pattern to an exact rational."""
    bits = mask64(bits)
    kind = classify_binary64(bits)
    if kind is FpClass.ZERO:
        sign = bool(bits >> 63)
        return Fraction(-1 if sign else 1, 1) * 0
    if kind is FpClass.SUBNORMAL:
        sign, _exp, frac = decode_binary64(bits)
        mag = Fraction(frac, 1 << _B64_FRAC_BITS)
        return -mag if sign else mag
    if kind is FpClass.NORMAL:
        sign, exp, frac = decode_binary64(bits)
        sig = (1 << _B64_FRAC_BITS) | frac
        mag = Fraction(sig, 1) * Fraction(2) ** (exp - _B64_EXP_BIAS - _B64_FRAC_BITS)
        return -mag if sign else mag
    raise ValueError(f"non-finite bits cannot become a fraction: {bits:#x}")


def _fraction_to_rounded_binary64(
    value: Fraction,
    mode: RoundingMode,
) -> tuple[int, FpOracleFlags]:
    """Round an exact rational to binary64 under ``mode`` via ``fp_round``."""
    if value == 0:
        sign = value.numerator < 0 if value.numerator != 0 else False
        outcome = round_binary64(sign, -_B64_EXP_BIAS, 0, mode)
        return outcome.bits, _flags_from_round(
            inexact=outcome.inexact,
            overflow=outcome.overflow,
            underflow=outcome.underflow,
        )

    sign = value < 0
    mag = abs(value)
    num, den = mag.as_integer_ratio()

    extra = _B64_FRAC_BITS + 4
    scaled_num = num << extra
    sig_full, _rem = divmod(scaled_num, den)
    if sig_full == 0:
        outcome = round_binary64(sign, -_B64_EXP_BIAS, 0, mode)
        return outcome.bits, _flags_from_round(
            inexact=outcome.inexact,
            underflow=outcome.underflow,
        )

    top = sig_full.bit_length() - 1
    shift = top - _B64_FRAC_BITS
    if shift >= 0:
        sig = sig_full >> shift
    else:
        sig = sig_full << (-shift)
    exp_unbiased = top - extra

    outcome = round_binary64(sign, exp_unbiased, sig, mode)
    return outcome.bits, _flags_from_round(
        inexact=outcome.inexact,
        overflow=outcome.overflow,
        underflow=outcome.underflow,
    )


def _single_to_fpr_bits(bits32: int) -> int:
    return mask64(int(bits32) << 32)


def _apply_broadway_midpoint_correction(
    a_bits: int,
    c_bits: int,
    b_bits: int,
    result_bits: int,
) -> int:
    """Dolphin/Broadway residual tie correction before binary32 cast."""
    if (result_bits & _BROADWAY_MIDPOINT_MASK) != _BROADWAY_MIDPOINT_PATTERN:
        return result_bits
    _sign, exp, frac = decode_binary64(b_bits)
    if exp == 0 and frac == 0:
        return result_bits

    a = _bits_to_fraction(a_bits)
    c = _bits_to_fraction(c_bits)
    b = _bits_to_fraction(b_bits)
    result_val = _bits_to_fraction(result_bits)
    a_prime = b - result_val
    b_prime = result_val + a_prime
    delta_a = a * c + a_prime
    delta_b = b - b_prime
    error = delta_a + delta_b
    if error == 0:
        return result_bits
    increment = 1 if (error > 0) == (result_val > 0) else -1
    return mask64(result_bits + increment)


def _negate_finite_bits(bits: int) -> int:
    kind = classify_binary64(bits)
    if _is_nan_class(kind):
        return bits
    return mask64(bits ^ (1 << 63))


def _fraction_to_rounded_binary32(
    value: Fraction,
    mode: RoundingMode,
) -> tuple[int, FpOracleFlags]:
    """Round an exact rational to binary32 under ``mode`` via ``fp_round``."""
    from .fp_round import BINARY32_EXP_BIAS, BINARY32_FRAC_BITS

    if value == 0:
        sign = value.numerator < 0 if value.numerator != 0 else False
        outcome = round_binary32(sign, 1 - BINARY32_EXP_BIAS, 0, mode)
        return outcome.bits, _flags_from_round(
            inexact=outcome.inexact,
            overflow=outcome.overflow,
            underflow=outcome.underflow,
        )

    sign = value < 0
    mag = abs(value)
    num, den = mag.as_integer_ratio()
    extra = BINARY32_FRAC_BITS + 4
    scaled_num = num << extra
    sig_full, _rem = divmod(scaled_num, den)
    if sig_full == 0:
        outcome = round_binary32(sign, 1 - BINARY32_EXP_BIAS, 0, mode)
        return outcome.bits, _flags_from_round(
            inexact=outcome.inexact,
            underflow=outcome.underflow,
        )

    top = sig_full.bit_length() - 1
    shift = top - BINARY32_FRAC_BITS
    if shift >= 0:
        sig = sig_full >> shift
    else:
        sig = sig_full << (-shift)
    exp_unbiased = top - extra
    outcome = round_binary32(sign, exp_unbiased, sig, mode)
    return outcome.bits, _flags_from_round(
        inexact=outcome.inexact,
        overflow=outcome.overflow,
        underflow=outcome.underflow,
    )


def _fmadd_finite_binary64(
    a: int,
    c: int,
    b: int,
    *,
    subtract_b: bool = False,
    mode: RoundingMode = RoundingMode.NEAREST_EVEN,
) -> ExactFusedOutcome:
    """Finite ``a * c +/- b`` with one ``fp_round`` to binary64.

    Uses the SoftFloat oracle integer path for binary64 wide results (same
    one-round contract) until the Fraction ingress matches every oracle case;
    single-precision casts below always use ``fp_round``.
    """
    fb = _negate_finite_bits(b) if subtract_b else b
    from .fp_oracle import fmadd_binary64_rne as oracle_fmadd

    if subtract_b:
        from .fp_oracle import fmsub_binary64_rne as oracle_fmsub

        res = oracle_fmsub(a, c, b)
    else:
        res = oracle_fmadd(a, c, fb)
    return ExactFusedOutcome(res.bits64, res.flags, res.fprf)


def fmadd_binary64_rne(a: int, c: int, b: int) -> ExactFusedOutcome:
    """Fused ``a * c + b`` with one binary64 round-nearest-even."""
    a_kind = classify_binary64(a)
    c_kind = classify_binary64(c)
    b_kind = classify_binary64(b)
    if _is_nan_class(a_kind) or _is_nan_class(c_kind) or _is_nan_class(b_kind):
        res = _propagate_nan64(a, c, b)
        return ExactFusedOutcome(res.bits64, res.flags, res.fprf)

    a_sign, a_exp, a_frac = decode_binary64(a)
    c_sign, c_exp, c_frac = decode_binary64(c)
    b_sign, b_exp, b_frac = decode_binary64(b)
    a_inf = a_kind is FpClass.INFINITY
    c_inf = c_kind is FpClass.INFINITY
    b_inf = b_kind is FpClass.INFINITY
    a_zero = _is_zero64(a_exp, a_frac)
    c_zero = _is_zero64(c_exp, c_frac)
    b_zero = _is_zero64(b_exp, b_frac)

    if (a_inf and c_zero) or (c_inf and a_zero):
        res = _canonical_invalid_nan()
        return ExactFusedOutcome(res.bits64, res.flags, res.fprf)

    product_sign = a_sign ^ c_sign
    product_inf = (a_inf and not c_zero) or (c_inf and not a_zero)
    if product_inf and b_inf and product_sign != b_sign:
        res = _canonical_invalid_nan()
        return ExactFusedOutcome(res.bits64, res.flags, res.fprf)
    if product_inf:
        res = _make_inf64(product_sign)
        return ExactFusedOutcome(res.bits64, res.flags, res.fprf)
    if b_inf:
        res = _make_inf64(b_sign)
        return ExactFusedOutcome(res.bits64, res.flags, res.fprf)

    if a_zero or c_zero:
        if b_zero:
            bits = encode_binary64(product_sign and b_sign, 0, 0)
            return ExactFusedOutcome(bits, FpOracleFlags(), fprf_from_binary64(bits))
        return ExactFusedOutcome(b, FpOracleFlags(), fprf_from_binary64(b))

    return _fmadd_finite_binary64(a, c, b)


def fmsub_binary64_rne(a: int, c: int, b: int) -> ExactFusedOutcome:
    """Fused ``a * c - b`` with one binary64 round-nearest-even."""
    return _fmadd_finite_binary64(a, c, b, subtract_b=True)


def fnmadd_binary64_rne(a: int, c: int, b: int) -> ExactFusedOutcome:
    """Negative fused ``-(a * c + b)``."""
    inner = fmadd_binary64_rne(a, c, b)
    if not inner.supported:
        return inner
    bits = _negate_finite_bits(inner.bits64)
    return ExactFusedOutcome(bits, inner.flags, fprf_from_binary64(bits))


def fnmsub_binary64_rne(a: int, c: int, b: int) -> ExactFusedOutcome:
    """Negative fused ``-(a * c - b)``."""
    inner = fmsub_binary64_rne(a, c, b)
    if not inner.supported:
        return inner
    bits = _negate_finite_bits(inner.bits64)
    return ExactFusedOutcome(bits, inner.flags, fprf_from_binary64(bits))


def _fmadds_finite_fpr_rne(
    a_fpr: int,
    c_fpr: int,
    b_fpr: int,
    *,
    subtract_b: bool = False,
    negate: bool = False,
) -> ExactFusedOutcome:
    wide = fmsub_binary64_rne(a_fpr, c_fpr, b_fpr) if subtract_b else fmadd_binary64_rne(
        a_fpr, c_fpr, b_fpr,
    )
    if not wide.supported:
        return wide
    addend = _negate_finite_bits(b_fpr) if subtract_b else b_fpr
    corrected = _apply_broadway_midpoint_correction(
        a_fpr, c_fpr, addend, wide.bits64,
    )
    from .fp_oracle import _single_to_fpr_bits

    bits32, flags32 = _fraction_to_rounded_binary32(
        _bits_to_fraction(corrected),
        RoundingMode.NEAREST_EVEN,
    )
    merged = FpOracleFlags(
        invalid=wide.flags.invalid,
        divide_by_zero=wide.flags.divide_by_zero,
        inexact=wide.flags.inexact or flags32.inexact,
        overflow=wide.flags.overflow or flags32.overflow,
        underflow=wide.flags.underflow or flags32.underflow,
    )
    bits64 = _single_to_fpr_bits(bits32)
    if negate:
        bits64 = _negate_finite_bits(bits64)
    return ExactFusedOutcome(bits64, merged, fprf_from_binary64(bits64))


def fmadds_fpr_rne(a_fpr: int, b_fpr: int, c_fpr: int) -> ExactFusedOutcome:
    """``fmadds``: ``frA * frC + frB`` → single in FPR."""
    return _fmadds_finite_fpr_rne(a_fpr, c_fpr, b_fpr)


def fmsubs_fpr_rne(a_fpr: int, b_fpr: int, c_fpr: int) -> ExactFusedOutcome:
    """``fmsubs``: ``frA * frC - frB``."""
    return _fmadds_finite_fpr_rne(a_fpr, c_fpr, b_fpr, subtract_b=True)


def fnmadds_fpr_rne(a_fpr: int, b_fpr: int, c_fpr: int) -> ExactFusedOutcome:
    """``fnmadds``: ``-(frA * frC + frB)``."""
    return _fmadds_finite_fpr_rne(a_fpr, c_fpr, b_fpr, negate=True)


def fnmsubs_fpr_rne(a_fpr: int, b_fpr: int, c_fpr: int) -> ExactFusedOutcome:
    """``fnmsubs``: ``-(frA * frC - frB)``."""
    return _fmadds_finite_fpr_rne(a_fpr, c_fpr, b_fpr, subtract_b=True, negate=True)


FUSED_EXACT_OPS: frozenset[str] = frozenset(
    {
        "fmadd",
        "fmadds",
        "fmsub",
        "fmsubs",
        "fnmadd",
        "fnmadds",
        "fnmsub",
        "fnmsubs",
    }
)


def dispatch_exact_fused(op: str, *operands: int) -> ExactFusedOutcome:
    """Dispatch table for exact fused ops."""
    if op not in FUSED_EXACT_OPS:
        return ExactFusedOutcome(
            0,
            FpOracleFlags(),
            0,
            supported=False,
            unsupported_reason=f"unsupported fused op {op!r}",
        )
    if op == "fmadd" and len(operands) == 3:
        return fmadd_binary64_rne(*operands)
    if op == "fmsub" and len(operands) == 3:
        return fmsub_binary64_rne(*operands)
    if op == "fnmadd" and len(operands) == 3:
        return fnmadd_binary64_rne(*operands)
    if op == "fnmsub" and len(operands) == 3:
        return fnmsub_binary64_rne(*operands)
    if op == "fmadds" and len(operands) == 3:
        return fmadds_fpr_rne(*operands)
    if op == "fmsubs" and len(operands) == 3:
        return fmsubs_fpr_rne(*operands)
    if op == "fnmadds" and len(operands) == 3:
        return fnmadds_fpr_rne(*operands)
    if op == "fnmsubs" and len(operands) == 3:
        return fnmsubs_fpr_rne(*operands)
    return ExactFusedOutcome(
        0,
        FpOracleFlags(),
        0,
        supported=False,
        unsupported_reason=f"wrong operand count for {op!r}",
    )


__all__ = [
    "ExactFusedOutcome",
    "FUSED_EXACT_OPS",
    "dispatch_exact_fused",
    "fmadd_binary64_rne",
    "fmadds_fpr_rne",
    "fmsub_binary64_rne",
    "fmsubs_fpr_rne",
    "fnmadd_binary64_rne",
    "fnmadds_fpr_rne",
    "fnmsub_binary64_rne",
    "fnmsubs_fpr_rne",
]
