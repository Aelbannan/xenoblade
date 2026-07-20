"""Precision-independent IEEE-754 rounding kernel (Phase 2).

Normalizes an exact unrounded ``(sign, unbiased_exponent, significand)`` tuple
and rounds to a target format under any of the four Broadway RN modes. Reports
guard/round/sticky, discarded nonzero, magnitude increase, pre/post tininess,
overflow direction, and FI/FR indicators for FPSCR updates.

Significands carry the implicit bit at bit index ``frac_bits`` (matching
``fp_oracle`` / SoftFloat conventions). No host ``float`` on production paths.
"""

from __future__ import annotations

from dataclasses import dataclass
from fractions import Fraction
from typing import Final

from .fp_backend import DEFAULT_INT_BACKEND, FPBackend
from .fp_bits import encode_binary32, encode_binary64, shift_right_sticky
from .fp_rounding import RoundingMode

BINARY32_FRAC_BITS: Final[int] = 23
BINARY32_EXP_BITS: Final[int] = 8
BINARY32_EXP_BIAS: Final[int] = 127

BINARY64_FRAC_BITS: Final[int] = 52
BINARY64_EXP_BITS: Final[int] = 11
BINARY64_EXP_BIAS: Final[int] = 1023


@dataclass(frozen=True, slots=True)
class FpFormat:
    """Target IEEE-754 encoding parameters."""

    frac_bits: int
    exp_bits: int
    exp_bias: int
    width: int

    @property
    def exp_max_unbiased(self) -> int:
        return (1 << self.exp_bits) - 2 - self.exp_bias

    @property
    def exp_min_unbiased(self) -> int:
        return 1 - self.exp_bias

    @property
    def max_finite_fraction(self) -> int:
        return (1 << self.frac_bits) - 1


FORMAT_BINARY32 = FpFormat(
    frac_bits=BINARY32_FRAC_BITS,
    exp_bits=BINARY32_EXP_BITS,
    exp_bias=BINARY32_EXP_BIAS,
    width=32,
)
FORMAT_BINARY64 = FpFormat(
    frac_bits=BINARY64_FRAC_BITS,
    exp_bits=BINARY64_EXP_BITS,
    exp_bias=BINARY64_EXP_BIAS,
    width=64,
)


@dataclass(frozen=True, slots=True)
class RoundGRS:
    """Guard, round, and sticky bits from the last rounding step."""

    guard: bool
    round_bit: bool
    sticky: bool
    discarded_nonzero: bool


@dataclass(frozen=True, slots=True)
class RoundOutcome:
    """Result of one format-directed rounding step."""

    bits: int
    sign: bool
    exp_unbiased: int
    significand: int
    exact: bool
    inexact: bool
    overflow: bool
    underflow: bool
    grs: RoundGRS
    magnitude_increased: bool
    pre_round_tiny: bool
    post_round_tiny: bool
    overflow_to_infinity: bool
    fi: bool
    fr: bool


def _encode(sign: bool, exp_unbiased: int, sig: int, fmt: FpFormat) -> int:
    frac_mask = (1 << fmt.frac_bits) - 1
    encoded_frac = sig & frac_mask
    if sig == 0:
        encoded_exp = 0
    elif sig >= (1 << fmt.frac_bits):
        encoded_exp = exp_unbiased + fmt.exp_bias
    else:
        encoded_exp = 0
    if fmt.width == 32:
        return encode_binary32(sign, encoded_exp, encoded_frac)
    if fmt.width == 64:
        return encode_binary64(sign, encoded_exp, encoded_frac)
    raise ValueError(f"unsupported format width {fmt.width}")


def _max_finite_bits(sign: bool, fmt: FpFormat) -> int:
    return _encode(
        sign,
        fmt.exp_max_unbiased,
        (1 << fmt.frac_bits) | fmt.max_finite_fraction,
        fmt,
    )


def _infinity_bits(sign: bool, fmt: FpFormat) -> int:
    exp_encoded = (1 << fmt.exp_bits) - 1
    if fmt.width == 32:
        return encode_binary32(sign, exp_encoded, 0)
    return encode_binary64(sign, exp_encoded, 0)


def _signed_zero(sign: bool, fmt: FpFormat) -> int:
    return _encode(sign, -fmt.exp_bias, 0, fmt)


def _should_increment(
    sign: bool,
    mode: RoundingMode,
    *,
    guard: bool,
    round_bit: bool,
    sticky: bool,
    lsb: bool,
) -> bool:
    """Return whether the rounded significand should increment by one ULP."""
    has_discarded = guard or round_bit or sticky
    if not has_discarded:
        return False
    if mode is RoundingMode.NEAREST_EVEN:
        if not guard:
            return False
        return round_bit or sticky or lsb
    if mode is RoundingMode.TOWARD_ZERO:
        return False
    if mode is RoundingMode.TOWARD_PLUS_INFINITY:
        return (not sign) and has_discarded
    if mode is RoundingMode.TOWARD_MINUS_INFINITY:
        return sign and has_discarded
    raise ValueError(f"unknown rounding mode {mode!r}")


def _round_significand(
    sign: bool,
    sig: int,
    shift: int,
    mode: RoundingMode,
) -> tuple[int, RoundGRS, bool]:
    """Round ``sig`` right by ``shift`` under ``mode``."""
    if shift <= 0:
        return sig << (-shift), RoundGRS(False, False, False, False), False

    shifted, guard, round_bit, sticky, discarded = shift_right_sticky(sig, shift)
    increment = _should_increment(
        sign,
        mode,
        guard=guard,
        round_bit=round_bit,
        sticky=sticky,
        lsb=bool(shifted & 1),
    )
    if increment:
        shifted += 1
    grs = RoundGRS(guard, round_bit, sticky, discarded)
    return shifted, grs, increment


def _merge_grs(existing: RoundGRS | None, new: RoundGRS) -> RoundGRS:
    if existing is None:
        return new
    return RoundGRS(
        guard=new.guard,
        round_bit=new.round_bit,
        sticky=existing.sticky or existing.discarded_nonzero or new.sticky,
        discarded_nonzero=existing.discarded_nonzero or new.discarded_nonzero,
    )


def _real_magnitude(exp_unbiased: int, sig: int, frac_bits: int) -> Fraction:
    """Return |value| as an exact rational (no host float)."""
    if sig == 0:
        return Fraction(0)
    return Fraction(sig, 1 << frac_bits) * (Fraction(2) ** exp_unbiased)


def _magnitude_increased(
    pre_exp: int,
    pre_sig: int,
    post_exp: int,
    post_sig: int,
    *,
    frac_bits: int,
) -> bool:
    """True when |rounded| > |exact| (FR helper)."""
    if post_sig == 0:
        return False
    if pre_sig == 0:
        return True
    return _real_magnitude(post_exp, post_sig, frac_bits) > _real_magnitude(
        pre_exp,
        pre_sig,
        frac_bits,
    )


def round_to_format(
    sign: bool,
    exp_unbiased: int,
    significand: int,
    fmt: FpFormat,
    mode: RoundingMode,
    *,
    backend: FPBackend | None = None,
) -> RoundOutcome:
    """Round an exact unnormalized finite value to ``fmt`` under ``mode``.

    ``significand`` includes the implicit bit at index ``fmt.frac_bits``.
    ``backend`` is reserved for Phase 7 symbolic instantiation; Phase 2 uses
    integer ops directly and ignores the backend parameter.
    """
    _ = backend or DEFAULT_INT_BACKEND

    pre_round_tiny = exp_unbiased < fmt.exp_min_unbiased

    if significand == 0:
        bits = _signed_zero(sign, fmt)
        grs = RoundGRS(False, False, False, False)
        return RoundOutcome(
            bits=bits,
            sign=sign,
            exp_unbiased=fmt.exp_min_unbiased,
            significand=0,
            exact=True,
            inexact=False,
            overflow=False,
            underflow=pre_round_tiny,
            grs=grs,
            magnitude_increased=False,
            pre_round_tiny=pre_round_tiny,
            post_round_tiny=True,
            overflow_to_infinity=False,
            fi=False,
            fr=False,
        )

    sig = significand
    exp = exp_unbiased
    grs: RoundGRS | None = None
    inexact = False

    top_bit = sig.bit_length() - 1
    if top_bit > fmt.frac_bits:
        shift = top_bit - fmt.frac_bits
        sig, step_grs, _inc = _round_significand(sign, sig, shift, mode)
        grs = _merge_grs(grs, step_grs)
        if step_grs.discarded_nonzero:
            inexact = True
        exp += shift
    elif top_bit < fmt.frac_bits:
        sig <<= fmt.frac_bits - top_bit
        exp -= fmt.frac_bits - top_bit

    if sig >= (1 << (fmt.frac_bits + 1)):
        sig, step_grs, _inc = _round_significand(sign, sig, 1, mode)
        grs = _merge_grs(grs, step_grs)
        if step_grs.discarded_nonzero:
            inexact = True
        exp += 1

    exp_max = fmt.exp_max_unbiased
    exp_min = fmt.exp_min_unbiased

    if exp > exp_max:
        inexact = True
        pos_overflow = not sign
        if mode is RoundingMode.TOWARD_ZERO:
            bits = _max_finite_bits(sign, fmt)
            overflow_to_inf = False
        elif mode is RoundingMode.TOWARD_PLUS_INFINITY:
            overflow_to_inf = pos_overflow
            bits = (
                _infinity_bits(sign, fmt)
                if overflow_to_inf
                else _max_finite_bits(sign, fmt)
            )
        elif mode is RoundingMode.TOWARD_MINUS_INFINITY:
            overflow_to_inf = not pos_overflow
            bits = (
                _infinity_bits(sign, fmt)
                if overflow_to_inf
                else _max_finite_bits(sign, fmt)
            )
        else:
            overflow_to_inf = True
            bits = _infinity_bits(sign, fmt)
        if overflow_to_inf:
            mag_inc = True
        else:
            mag_inc = _magnitude_increased(
                exp_unbiased,
                significand,
                fmt.exp_max_unbiased,
                (1 << fmt.frac_bits) | fmt.max_finite_fraction,
                frac_bits=fmt.frac_bits,
            )
        return RoundOutcome(
            bits=bits,
            sign=sign,
            exp_unbiased=exp,
            significand=sig,
            exact=False,
            inexact=True,
            overflow=True,
            underflow=False,
            grs=grs or RoundGRS(False, False, False, True),
            magnitude_increased=mag_inc,
            pre_round_tiny=pre_round_tiny,
            post_round_tiny=False,
            overflow_to_infinity=overflow_to_inf,
            fi=True,
            fr=mag_inc,
        )

    if exp < exp_min:
        shift = exp_min - exp
        sig, step_grs, _inc = _round_significand(sign, sig, shift, mode)
        grs = _merge_grs(grs, step_grs)
        if step_grs.discarded_nonzero:
            inexact = True
        exp = exp_min

        if sig == 0:
            mag_inc = _magnitude_increased(
                exp_unbiased, significand, exp_min, 0, frac_bits=fmt.frac_bits,
            )
            return RoundOutcome(
                bits=_signed_zero(sign, fmt),
                sign=sign,
                exp_unbiased=exp_min,
                significand=0,
                exact=False,
                inexact=True,
                overflow=False,
                underflow=True,
                grs=grs,
                magnitude_increased=mag_inc,
                pre_round_tiny=pre_round_tiny,
                post_round_tiny=True,
                overflow_to_infinity=False,
                fi=True,
                fr=mag_inc,
            )

        if sig >= (1 << fmt.frac_bits):
            normal_exp = fmt.exp_min_unbiased
            normal_sig = 1 << fmt.frac_bits
            mag_inc = _magnitude_increased(
                exp_unbiased,
                significand,
                normal_exp,
                normal_sig,
                frac_bits=fmt.frac_bits,
            )
            return RoundOutcome(
                bits=_encode(sign, normal_exp, normal_sig, fmt),
                sign=sign,
                exp_unbiased=normal_exp,
                significand=normal_sig,
                exact=not inexact,
                inexact=inexact,
                overflow=False,
                underflow=pre_round_tiny,
                grs=grs,
                magnitude_increased=mag_inc,
                pre_round_tiny=pre_round_tiny,
                post_round_tiny=False,
                overflow_to_infinity=False,
                fi=inexact,
                fr=inexact and mag_inc,
            )

        mag_inc = _magnitude_increased(
            exp_unbiased, significand, exp_min, sig, frac_bits=fmt.frac_bits,
        )
        if not inexact and sig != 0:
            inexact = True
        return RoundOutcome(
            bits=_encode(sign, exp_min, sig, fmt),
            sign=sign,
            exp_unbiased=exp_min,
            significand=sig,
            exact=not inexact,
            inexact=inexact,
            overflow=False,
            underflow=True,
            grs=grs or RoundGRS(False, False, False, False),
            magnitude_increased=mag_inc,
            pre_round_tiny=pre_round_tiny,
            post_round_tiny=True,
            overflow_to_infinity=False,
            fi=inexact,
            fr=inexact and mag_inc,
        )

    bits = _encode(sign, exp, sig, fmt)
    post_round_tiny = exp == exp_min and (sig >> fmt.frac_bits) == 0
    mag_inc = _magnitude_increased(
        exp_unbiased, significand, exp, sig, frac_bits=fmt.frac_bits,
    )
    return RoundOutcome(
        bits=bits,
        sign=sign,
        exp_unbiased=exp,
        significand=sig,
        exact=not inexact,
        inexact=inexact,
        overflow=False,
        underflow=pre_round_tiny and (post_round_tiny or inexact),
        grs=grs or RoundGRS(False, False, False, False),
        magnitude_increased=mag_inc,
        pre_round_tiny=pre_round_tiny,
        post_round_tiny=post_round_tiny,
        overflow_to_infinity=False,
        fi=inexact,
        fr=inexact and mag_inc,
    )


def round_binary64(
    sign: bool,
    exp_unbiased: int,
    significand: int,
    mode: RoundingMode,
    *,
    backend: FPBackend | None = None,
) -> RoundOutcome:
    """Round to binary64."""
    return round_to_format(
        sign,
        exp_unbiased,
        significand,
        FORMAT_BINARY64,
        mode,
        backend=backend,
    )


def round_binary32(
    sign: bool,
    exp_unbiased: int,
    significand: int,
    mode: RoundingMode,
    *,
    backend: FPBackend | None = None,
) -> RoundOutcome:
    """Round to binary32."""
    return round_to_format(
        sign,
        exp_unbiased,
        significand,
        FORMAT_BINARY32,
        mode,
        backend=backend,
    )


__all__ = [
    "BINARY32_EXP_BIAS",
    "BINARY32_EXP_BITS",
    "BINARY32_FRAC_BITS",
    "BINARY64_EXP_BIAS",
    "BINARY64_EXP_BITS",
    "BINARY64_FRAC_BITS",
    "FORMAT_BINARY32",
    "FORMAT_BINARY64",
    "FpFormat",
    "RoundGRS",
    "RoundOutcome",
    "round_binary32",
    "round_binary64",
    "round_to_format",
]
