"""Bit-level floating-point oracle scaffold (SoftFloat-style prep).

This module provides pure-Python IEEE-754 bit manipulations for a small set of
scalar ops. It deliberately avoids host ``float`` / ``struct.pack`` on
proof-critical paths so a future migration can treat results as independently
checkable bit patterns.

**Not production-complete:** exception propagation into FPSCR sticky latch,
Force25 (applied by semantics before the oracle), paired-single lanes, and
Broadway single-FMA midpoint residuals remain partial or unimplemented.
Unhandled cases fail closed via :class:`OracleUnimplementedError`.

**Partially wired:** ``ConcreteOps`` routes ``fadd``/``fadds``/``fmul``/``fmuls``/
``fsub``/``fsubs``/``fdiv``/``fdivs``/``fmadd``/``fmadds``/``fmsub``/``fmsubs``/
``fnmadd``/``fnmadds``/``fnmsub``/``fnmsubs`` through this oracle, and Wave 3
Track B routes ``ps_add``/``ps_sub``/``ps_mul``/``ps_m*`` paired lanes through
``ps_lane_outcome`` + ``combine_paired_outcomes`` (ConcreteOps only).
significand path models finite normals/zeros, subnormals, ±Inf, quiet/signaling
NaN propagation, division by zero (±Inf + ZX), and overflow (±Inf + OX). Force25
for single fused forms is applied by the semantics layer before the oracle sees
``frC``. Negative fused forms negate finite results only (NaN payloads are never
sign-flipped). Single fused forms still fail closed on Dolphin/Broadway
midpoint-tie residues with a nonzero addend, and on near-cancellation sticky
residues. SymbolicOps, paired-single lanes, and other FP paths still use host
float or Z3. Nothing here promotes FP proofs out of Tier C.

**Unified outcome (Track C scaffold):** :class:`FpOracleResult` remains the
SoftFloat return type. Prefer :mod:`tools.ppc_equivalence.fp_outcome` adapters
(``outcome_from_oracle`` / ``oracle_from_outcome``) when lifting into the
shared ``FPOutcome`` container — existing ``.bits64`` call sites stay valid.
"""

from __future__ import annotations

from dataclasses import dataclass
from enum import Enum
from typing import Final

MASK32: Final[int] = 0xFFFFFFFF
MASK64: Final[int] = 0xFFFFFFFFFFFFFFFF

# Binary64 field masks (IEEE-754).
_B64_SIGN_SHIFT = 63
_B64_EXP_SHIFT = 52
_B64_EXP_BITS = 11
_B64_FRAC_BITS = 52
_B64_EXP_MASK = (1 << _B64_EXP_BITS) - 1
_B64_FRAC_MASK = (1 << _B64_FRAC_BITS) - 1
_B64_QUIET_NAN = 1 << (_B64_FRAC_BITS - 1)
_B64_EXP_BIAS = 1023
_B64_EXP_MAX = _B64_EXP_MASK

# Binary32 field masks.
_B32_EXP_SHIFT = 23
_B32_EXP_BITS = 8
_B32_FRAC_BITS = 23
_B32_EXP_MASK = (1 << _B32_EXP_BITS) - 1
_B32_FRAC_MASK = (1 << _B32_FRAC_BITS) - 1
_B32_QUIET_NAN = 1 << (_B32_FRAC_BITS - 1)
_B32_EXP_BIAS = 127


class OracleUnimplementedError(RuntimeError):
    """Raised when the oracle scaffold cannot model an operand or op safely."""


class FpClass(str, Enum):
    ZERO = "zero"
    SUBNORMAL = "subnormal"
    NORMAL = "normal"
    INFINITY = "infinity"
    QNAN = "qnan"
    SNAN = "snan"


ORACLE_SUPPORTED_OPS: frozenset[str] = frozenset({
    "fadd",
    "fadds",
    "fmul",
    "fmuls",
    "fsub",
    "fsubs",
    "fdiv",
    "fdivs",
    "fmadd",
    "fmadds",
    "fmsub",
    "fmsubs",
    "fnmadd",
    "fnmadds",
    "fnmsub",
    "fnmsubs",
})


@dataclass(frozen=True, slots=True)
class FpOracleFlags:
    """Sticky exception indicators the oracle may eventually model."""

    invalid: bool = False
    divide_by_zero: bool = False
    inexact: bool = False
    overflow: bool = False
    underflow: bool = False

    def to_dict(self) -> dict[str, bool]:
        return {
            "invalid": self.invalid,
            "divide_by_zero": self.divide_by_zero,
            "inexact": self.inexact,
            "overflow": self.overflow,
            "underflow": self.underflow,
        }


@dataclass(frozen=True, slots=True)
class FpOracleResult:
    """Bit-exact oracle output for one scalar FP operation."""

    bits64: int
    flags: FpOracleFlags
    fprf: int

    def to_dict(self) -> dict[str, object]:
        return {
            "bits64": f"0x{self.bits64:016x}",
            "flags": self.flags.to_dict(),
            "fprf": self.fprf,
        }

    def to_outcome(self):
        """Lift into the unified :class:`~.fp_outcome.FPOutcome` scaffold."""
        from .fp_outcome import outcome_from_oracle

        return outcome_from_oracle(self)


def mask32(value: int) -> int:
    return value & MASK32


def mask64(value: int) -> int:
    return value & MASK64


def classify_binary64(bits: int) -> FpClass:
    bits = mask64(bits)
    exponent = (bits >> _B64_EXP_SHIFT) & _B64_EXP_MASK
    fraction = bits & _B64_FRAC_MASK
    if exponent == _B64_EXP_MAX:
        if fraction == 0:
            return FpClass.INFINITY
        if fraction & _B64_QUIET_NAN:
            return FpClass.QNAN
        return FpClass.SNAN
    if exponent == 0:
        return FpClass.ZERO if fraction == 0 else FpClass.SUBNORMAL
    return FpClass.NORMAL


def classify_binary32(bits: int) -> FpClass:
    bits = mask32(bits)
    exponent = (bits >> _B32_EXP_SHIFT) & _B32_EXP_MASK
    fraction = bits & _B32_FRAC_MASK
    if exponent == _B32_EXP_MASK:
        if fraction == 0:
            return FpClass.INFINITY
        if fraction & _B32_QUIET_NAN:
            return FpClass.QNAN
        return FpClass.SNAN
    if exponent == 0:
        return FpClass.ZERO if fraction == 0 else FpClass.SUBNORMAL
    return FpClass.NORMAL


def decode_binary64(bits: int) -> tuple[bool, int, int]:
    bits = mask64(bits)
    sign = bool(bits >> _B64_SIGN_SHIFT)
    exponent = (bits >> _B64_EXP_SHIFT) & _B64_EXP_MASK
    fraction = bits & _B64_FRAC_MASK
    return sign, exponent, fraction


def encode_binary64(sign: bool, exponent: int, fraction: int) -> int:
    value = ((int(sign) << _B64_SIGN_SHIFT)
               | ((exponent & _B64_EXP_MASK) << _B64_EXP_SHIFT)
               | (fraction & _B64_FRAC_MASK))
    return mask64(value)


def decode_binary32(bits: int) -> tuple[bool, int, int]:
    bits = mask32(bits)
    sign = bool(bits >> 31)
    exponent = (bits >> _B32_EXP_SHIFT) & _B32_EXP_MASK
    fraction = bits & _B32_FRAC_MASK
    return sign, exponent, fraction


def encode_binary32(sign: bool, exponent: int, fraction: int) -> int:
    value = ((int(sign) << 31)
             | ((exponent & _B32_EXP_MASK) << _B32_EXP_SHIFT)
             | (fraction & _B32_FRAC_MASK))
    return mask32(value)


def fprf_from_binary64(bits: int) -> int:
    """Return the 5-bit FPRF class nibble for ``bits`` (Gekko layout)."""
    bits = mask64(bits)
    sign = bool(bits >> _B64_SIGN_SHIFT)
    exponent = (bits >> _B64_EXP_SHIFT) & _B64_EXP_MASK
    fraction = bits & _B64_FRAC_MASK
    if exponent == _B64_EXP_MAX:
        if fraction:
            return 0x11
        return 0x09 if sign else 0x05
    if exponent == 0:
        if fraction == 0:
            return 0x12 if sign else 0x02
        return 0x18 if sign else 0x14
    return 0x08 if sign else 0x04


def _fail_unimplemented(reason: str) -> OracleUnimplementedError:
    return OracleUnimplementedError(f"fp oracle scaffold: {reason}")


_B64_CANONICAL_NAN: Final[int] = 0x7FF8000000000000


def _oracle_result(bits: int, flags: FpOracleFlags | None = None) -> FpOracleResult:
    bits = mask64(bits)
    return FpOracleResult(bits, flags or FpOracleFlags(), fprf_from_binary64(bits))


def _make_inf64(sign: bool, *, overflow: bool = False, divide_by_zero: bool = False) -> FpOracleResult:
    flags = FpOracleFlags(
        overflow=overflow,
        inexact=overflow,
        divide_by_zero=divide_by_zero,
    )
    return _oracle_result(encode_binary64(sign, _B64_EXP_MAX, 0), flags)


def _quiet_nan64(bits: int) -> int:
    return mask64(bits | _B64_QUIET_NAN)


def _is_nan_class(kind: FpClass) -> bool:
    return kind in (FpClass.QNAN, FpClass.SNAN)


def _is_zero64(exp: int, frac: int) -> bool:
    return exp == 0 and frac == 0


def _propagate_nan64(*operands: int) -> FpOracleResult:
    """Quiet the first NaN operand; set invalid when any operand is sNaN."""
    invalid = False
    chosen: int | None = None
    for bits in operands:
        kind = classify_binary64(bits)
        if kind is FpClass.SNAN:
            invalid = True
            if chosen is None:
                chosen = _quiet_nan64(bits)
        elif kind is FpClass.QNAN and chosen is None:
            chosen = mask64(bits)
    if chosen is None:
        chosen = _B64_CANONICAL_NAN
        invalid = True
    return _oracle_result(chosen, FpOracleFlags(invalid=invalid))


def _canonical_invalid_nan() -> FpOracleResult:
    return _oracle_result(_B64_CANONICAL_NAN, FpOracleFlags(invalid=True))


def _significand64(exponent: int, fraction: int) -> int:
    if exponent == 0:
        return fraction
    return (1 << _B64_FRAC_BITS) | fraction


def _rne_round_bits(sig: int, shift: int) -> tuple[int, bool]:
    """Right-shift ``sig`` by ``shift`` with round-nearest-even; return (sig, inexact)."""
    if shift <= 0:
        return sig << (-shift), False
    lost = sig & ((1 << shift) - 1)
    sig >>= shift
    if not lost:
        return sig, False
    half = 1 << (shift - 1)
    if lost > half or (lost == half and (sig & 1)):
        sig += 1
    return sig, True


def _round_rne(
    sign: bool,
    exp_unbiased: int,
    sig: int,
    *,
    exp_bits: int,
    exp_bias: int,
) -> tuple[int, FpOracleFlags]:
    """Normalize ``sig`` (with implicit bit) and round to binary64."""
    flags = FpOracleFlags()
    if sig == 0:
        return encode_binary64(sign, 0, 0), flags

    top_bit = sig.bit_length() - 1
    if top_bit > _B64_FRAC_BITS:
        shift = top_bit - _B64_FRAC_BITS
        sig, inexact = _rne_round_bits(sig, shift)
        exp_unbiased += shift
        if inexact:
            flags = FpOracleFlags(inexact=True)
    elif top_bit < _B64_FRAC_BITS:
        shift = _B64_FRAC_BITS - top_bit
        exp_unbiased -= shift
        sig <<= shift

    # Carry from rounding can bump the significand into the next binade.
    if sig >= (1 << (_B64_FRAC_BITS + 1)):
        sig >>= 1
        exp_unbiased += 1

    exp_max_unbiased = (1 << exp_bits) - 2 - exp_bias
    exp_min_unbiased = 1 - exp_bias
    if exp_unbiased > exp_max_unbiased:
        return encode_binary64(sign, _B64_EXP_MAX, 0), FpOracleFlags(
            overflow=True, inexact=True,
        )

    if exp_unbiased < exp_min_unbiased:
        # Shift into the subnormal window (or flush to zero).
        shift = exp_min_unbiased - exp_unbiased
        # ``sig`` still has the implicit bit at position FRAC_BITS.
        sig, inexact = _rne_round_bits(sig, shift)
        if inexact:
            flags = FpOracleFlags(inexact=True, underflow=True)
        if sig == 0:
            return encode_binary64(sign, 0, 0), flags if flags.inexact else FpOracleFlags(
                underflow=True, inexact=True,
            )
        if sig >= (1 << _B64_FRAC_BITS):
            # Rounded up to the smallest normal.
            return encode_binary64(sign, 1, 0), flags
        return encode_binary64(sign, 0, sig & _B64_FRAC_MASK), (
            flags if flags.inexact else FpOracleFlags(underflow=True, inexact=bool(sig))
        )

    encoded_frac = sig & _B64_FRAC_MASK
    encoded_exp = exp_unbiased + exp_bias
    return encode_binary64(sign, encoded_exp, encoded_frac), flags


def fadd_binary64_rne(a: int, b: int) -> FpOracleResult:
    """Add two binary64 operands with round-nearest-even."""
    a_kind = classify_binary64(a)
    b_kind = classify_binary64(b)
    if _is_nan_class(a_kind) or _is_nan_class(b_kind):
        return _propagate_nan64(a, b)

    a_sign, a_exp, a_frac = decode_binary64(a)
    b_sign, b_exp, b_frac = decode_binary64(b)
    a_inf = a_kind is FpClass.INFINITY
    b_inf = b_kind is FpClass.INFINITY
    if a_inf and b_inf:
        if a_sign != b_sign:
            return _canonical_invalid_nan()
        return _make_inf64(a_sign)
    if a_inf:
        return _make_inf64(a_sign)
    if b_inf:
        return _make_inf64(b_sign)

    if _is_zero64(a_exp, a_frac):
        if _is_zero64(b_exp, b_frac):
            # (+0)+(+0)=+0; (-0)+(-0)=-0; mixed → +0 under RNE.
            bits = encode_binary64(a_sign and b_sign, 0, 0)
            return _oracle_result(bits)
        return _oracle_result(b)
    if _is_zero64(b_exp, b_frac):
        return _oracle_result(a)

    a_sig = _significand64(a_exp, a_frac)
    b_sig = _significand64(b_exp, b_frac)
    a_unbiased = (a_exp - _B64_EXP_BIAS) if a_exp else (1 - _B64_EXP_BIAS)
    b_unbiased = (b_exp - _B64_EXP_BIAS) if b_exp else (1 - _B64_EXP_BIAS)

    if a_unbiased < b_unbiased or (a_unbiased == b_unbiased and b_sig > a_sig):
        a_sign, b_sign = b_sign, a_sign
        a_sig, b_sig = b_sig, a_sig
        a_unbiased, b_unbiased = b_unbiased, a_unbiased
        a_exp, a_frac = b_exp, b_frac

    shift = a_unbiased - b_unbiased
    if shift >= _B64_FRAC_BITS + 3:
        return _oracle_result(encode_binary64(a_sign, a_exp, a_frac))

    sticky = 0
    if shift > 0:
        sticky = 1 if (b_sig & ((1 << shift) - 1)) else 0
        b_sig >>= shift
    if a_sign == b_sign:
        sig = a_sig + b_sig
        sign = a_sign
        exp_unbiased = a_unbiased
    else:
        sig = a_sig - b_sig
        sign = a_sign
        exp_unbiased = a_unbiased
        if sig == 0:
            if sticky:
                # Tiny residue after cancellation — treat as +0 under RNE.
                return _oracle_result(encode_binary64(False, 0, 0))
            return _oracle_result(encode_binary64(False, 0, 0))

    bits, flags = _round_rne(
        sign,
        exp_unbiased,
        sig,
        exp_bits=_B64_EXP_BITS,
        exp_bias=_B64_EXP_BIAS,
    )
    if sticky and not flags.inexact:
        flags = FpOracleFlags(
            invalid=flags.invalid,
            divide_by_zero=flags.divide_by_zero,
            inexact=True,
            overflow=flags.overflow,
            underflow=flags.underflow,
        )
    return _oracle_result(bits, flags)


def fmul_binary64_rne(a: int, b: int) -> FpOracleResult:
    """Multiply two binary64 operands with round-nearest-even."""
    a_kind = classify_binary64(a)
    b_kind = classify_binary64(b)
    if _is_nan_class(a_kind) or _is_nan_class(b_kind):
        return _propagate_nan64(a, b)

    a_sign, a_exp, a_frac = decode_binary64(a)
    b_sign, b_exp, b_frac = decode_binary64(b)
    sign = a_sign ^ b_sign
    a_inf = a_kind is FpClass.INFINITY
    b_inf = b_kind is FpClass.INFINITY
    a_zero = _is_zero64(a_exp, a_frac)
    b_zero = _is_zero64(b_exp, b_frac)
    if (a_inf and b_zero) or (b_inf and a_zero):
        return _canonical_invalid_nan()
    if a_inf or b_inf:
        return _make_inf64(sign)
    if a_zero or b_zero:
        return _oracle_result(encode_binary64(sign, 0, 0))

    a_sig = _significand64(a_exp, a_frac)
    b_sig = _significand64(b_exp, b_frac)
    a_unbiased = (a_exp - _B64_EXP_BIAS) if a_exp else (1 - _B64_EXP_BIAS)
    b_unbiased = (b_exp - _B64_EXP_BIAS) if b_exp else (1 - _B64_EXP_BIAS)
    product = a_sig * b_sig
    bits, flags = _round_rne(
        sign,
        a_unbiased + b_unbiased - _B64_FRAC_BITS,
        product,
        exp_bits=_B64_EXP_BITS,
        exp_bias=_B64_EXP_BIAS,
    )
    return _oracle_result(bits, flags)


def fsub_binary64_rne(a: int, b: int) -> FpOracleResult:
    """Subtract two binary64 operands with round-nearest-even."""
    b_neg = mask64(b ^ (1 << _B64_SIGN_SHIFT))
    return fadd_binary64_rne(a, b_neg)


def fdiv_binary64_rne(a: int, b: int) -> FpOracleResult:
    """Divide two binary64 operands with round-nearest-even."""
    a_kind = classify_binary64(a)
    b_kind = classify_binary64(b)
    if _is_nan_class(a_kind) or _is_nan_class(b_kind):
        return _propagate_nan64(a, b)

    a_sign, a_exp, a_frac = decode_binary64(a)
    b_sign, b_exp, b_frac = decode_binary64(b)
    sign = a_sign ^ b_sign
    a_inf = a_kind is FpClass.INFINITY
    b_inf = b_kind is FpClass.INFINITY
    a_zero = _is_zero64(a_exp, a_frac)
    b_zero = _is_zero64(b_exp, b_frac)
    if a_inf and b_inf:
        return _canonical_invalid_nan()
    if a_zero and b_zero:
        return _canonical_invalid_nan()
    if a_inf:
        return _make_inf64(sign)
    if b_inf:
        return _oracle_result(encode_binary64(sign, 0, 0))
    if b_zero:
        if a_zero:
            return _canonical_invalid_nan()
        return _make_inf64(sign, divide_by_zero=True)
    if a_zero:
        return _oracle_result(encode_binary64(sign, 0, 0))

    a_sig = _significand64(a_exp, a_frac)
    b_sig = _significand64(b_exp, b_frac)
    a_unbiased = (a_exp - _B64_EXP_BIAS) if a_exp else (1 - _B64_EXP_BIAS)
    b_unbiased = (b_exp - _B64_EXP_BIAS) if b_exp else (1 - _B64_EXP_BIAS)
    # Quotient significand with extra guard bits for RNE.
    shift = _B64_FRAC_BITS + 3
    quot, rem = divmod(a_sig << shift, b_sig)
    bits, flags = _round_rne(
        sign,
        a_unbiased - b_unbiased - shift + _B64_FRAC_BITS,
        quot,
        exp_bits=_B64_EXP_BITS,
        exp_bias=_B64_EXP_BIAS,
    )
    if rem and not flags.inexact:
        flags = FpOracleFlags(
            invalid=flags.invalid,
            divide_by_zero=flags.divide_by_zero,
            inexact=True,
            overflow=flags.overflow,
            underflow=flags.underflow,
        )
    return _oracle_result(bits, flags)


def _round_binary64_to_binary32_rne(bits64: int) -> tuple[int, FpOracleFlags]:
    sign, exp, frac = decode_binary64(bits64)
    kind = classify_binary64(bits64)
    if kind is FpClass.INFINITY:
        return encode_binary32(sign, _B32_EXP_MASK, 0), FpOracleFlags()
    if _is_nan_class(kind):
        # Truncate the quieted payload into the binary32 NaN fraction.
        quieted = _quiet_nan64(bits64)
        _qs, _qe, qfrac = decode_binary64(quieted)
        frac32 = (qfrac >> (_B64_FRAC_BITS - _B32_FRAC_BITS)) & _B32_FRAC_MASK
        if frac32 == 0:
            frac32 = _B32_QUIET_NAN
        return encode_binary32(sign, _B32_EXP_MASK, frac32), FpOracleFlags(
            invalid=(kind is FpClass.SNAN),
        )
    if exp == 0 and frac == 0:
        return encode_binary32(sign, 0, 0), FpOracleFlags()

    sig = _significand64(exp, frac)
    unbiased = (exp - _B64_EXP_BIAS) if exp else (1 - _B64_EXP_BIAS)
    flags = FpOracleFlags()

    shift = _B64_FRAC_BITS - _B32_FRAC_BITS
    sig, inexact = _rne_round_bits(sig, shift)
    if inexact:
        flags = FpOracleFlags(inexact=True)

    if sig >= (1 << (_B32_FRAC_BITS + 1)):
        sig >>= 1
        unbiased += 1

    exp_max_unbiased = (1 << _B32_EXP_BITS) - 2 - _B32_EXP_BIAS
    exp_min_unbiased = 1 - _B32_EXP_BIAS
    if unbiased > exp_max_unbiased:
        return encode_binary32(sign, _B32_EXP_MASK, 0), FpOracleFlags(
            overflow=True, inexact=True,
        )
    if unbiased < exp_min_unbiased:
        sub_shift = exp_min_unbiased - unbiased
        sig, sub_inexact = _rne_round_bits(sig, sub_shift)
        if sub_inexact:
            flags = FpOracleFlags(inexact=True, underflow=True)
        if sig == 0:
            return encode_binary32(sign, 0, 0), flags if flags.inexact else FpOracleFlags(
                underflow=True, inexact=True,
            )
        if sig >= (1 << _B32_FRAC_BITS):
            return encode_binary32(sign, 1, 0), flags
        return encode_binary32(sign, 0, sig & _B32_FRAC_MASK), (
            flags if flags.inexact else FpOracleFlags(underflow=True, inexact=True)
        )

    encoded_frac = sig & _B32_FRAC_MASK
    encoded_exp = unbiased + _B32_EXP_BIAS
    return encode_binary32(sign, encoded_exp, encoded_frac), flags


def _single_to_fpr_bits(bits32: int) -> int:
    """Expand a binary32 value into Broadway FPR double-word storage."""
    sign, exp, frac = decode_binary32(bits32)
    if exp == 0 and frac == 0:
        return encode_binary64(sign, 0, 0)
    if exp == _B32_EXP_MASK:
        if frac == 0:
            return encode_binary64(sign, _B64_EXP_MAX, 0)
        # Quiet NaN payload truncated to binary32 then re-expanded.
        return encode_binary64(
            sign,
            _B64_EXP_MAX,
            (frac | _B32_QUIET_NAN) << (_B64_FRAC_BITS - _B32_FRAC_BITS),
        )
    if exp == 0:
        # Normalize binary32 subnormal into binary64.
        shift = _B32_FRAC_BITS - frac.bit_length()
        frac <<= shift + 1
        unbiased = (1 - _B32_EXP_BIAS) - (shift + 1)
        return encode_binary64(
            sign,
            unbiased + _B64_EXP_BIAS,
            (frac << (_B64_FRAC_BITS - _B32_FRAC_BITS)) & _B64_FRAC_MASK,
        )
    return encode_binary64(
        sign,
        exp - _B32_EXP_BIAS + _B64_EXP_BIAS,
        frac << (_B64_FRAC_BITS - _B32_FRAC_BITS),
    )


def fadds_fpr_rne(a_fpr: int, b_fpr: int) -> FpOracleResult:
    """``fadds`` on FPR-encoded operands (single-precision result in FPR)."""
    wide = fadd_binary64_rne(a_fpr, b_fpr)
    bits32, flags = _round_binary64_to_binary32_rne(wide.bits64)
    merged_flags = FpOracleFlags(
        invalid=wide.flags.invalid,
        divide_by_zero=wide.flags.divide_by_zero,
        inexact=wide.flags.inexact or flags.inexact,
        overflow=wide.flags.overflow or flags.overflow,
        underflow=wide.flags.underflow or flags.underflow,
    )
    bits64 = _single_to_fpr_bits(bits32)
    return FpOracleResult(bits64, merged_flags, fprf_from_binary64(bits64))


def fmuls_fpr_rne(a_fpr: int, c_fpr: int) -> FpOracleResult:
    """``fmuls`` on FPR-encoded operands (single-precision result in FPR)."""
    wide = fmul_binary64_rne(a_fpr, c_fpr)
    bits32, flags = _round_binary64_to_binary32_rne(wide.bits64)
    merged_flags = FpOracleFlags(
        invalid=wide.flags.invalid,
        divide_by_zero=wide.flags.divide_by_zero,
        inexact=wide.flags.inexact or flags.inexact,
        overflow=wide.flags.overflow or flags.overflow,
        underflow=wide.flags.underflow or flags.underflow,
    )
    bits64 = _single_to_fpr_bits(bits32)
    return FpOracleResult(bits64, merged_flags, fprf_from_binary64(bits64))


def fsubs_fpr_rne(a_fpr: int, b_fpr: int) -> FpOracleResult:
    """``fsubs`` on FPR-encoded operands (single-precision result in FPR)."""
    wide = fsub_binary64_rne(a_fpr, b_fpr)
    bits32, flags = _round_binary64_to_binary32_rne(wide.bits64)
    merged_flags = FpOracleFlags(
        invalid=wide.flags.invalid,
        divide_by_zero=wide.flags.divide_by_zero,
        inexact=wide.flags.inexact or flags.inexact,
        overflow=wide.flags.overflow or flags.overflow,
        underflow=wide.flags.underflow or flags.underflow,
    )
    bits64 = _single_to_fpr_bits(bits32)
    return FpOracleResult(bits64, merged_flags, fprf_from_binary64(bits64))


def fdivs_fpr_rne(a_fpr: int, b_fpr: int) -> FpOracleResult:
    """``fdivs`` on FPR-encoded operands (single-precision result in FPR)."""
    wide = fdiv_binary64_rne(a_fpr, b_fpr)
    bits32, flags = _round_binary64_to_binary32_rne(wide.bits64)
    merged_flags = FpOracleFlags(
        invalid=wide.flags.invalid,
        divide_by_zero=wide.flags.divide_by_zero,
        inexact=wide.flags.inexact or flags.inexact,
        overflow=wide.flags.overflow or flags.overflow,
        underflow=wide.flags.underflow or flags.underflow,
    )
    bits64 = _single_to_fpr_bits(bits32)
    return FpOracleResult(bits64, merged_flags, fprf_from_binary64(bits64))



def fmadd_binary64_rne(a: int, c: int, b: int) -> FpOracleResult:
    """Fused multiply-add ``a * c + b`` with one binary64 round-nearest-even.

    Operand order matches PowerPC ``fmadd`` (``frA * frC + frB``).
    """
    a_kind = classify_binary64(a)
    c_kind = classify_binary64(c)
    b_kind = classify_binary64(b)
    if _is_nan_class(a_kind) or _is_nan_class(c_kind) or _is_nan_class(b_kind):
        return _propagate_nan64(a, c, b)

    a_sign, a_exp, a_frac = decode_binary64(a)
    c_sign, c_exp, c_frac = decode_binary64(c)
    b_sign, b_exp, b_frac = decode_binary64(b)
    product_sign = a_sign ^ c_sign
    a_zero = _is_zero64(a_exp, a_frac)
    c_zero = _is_zero64(c_exp, c_frac)
    b_zero = _is_zero64(b_exp, b_frac)
    a_inf = a_kind is FpClass.INFINITY
    c_inf = c_kind is FpClass.INFINITY
    b_inf = b_kind is FpClass.INFINITY

    # Invalid: Inf * 0 (either way).
    if (a_inf and c_zero) or (c_inf and a_zero):
        return _canonical_invalid_nan()

    product_inf = (a_inf and not c_zero) or (c_inf and not a_zero)
    if product_inf and b_inf and product_sign != b_sign:
        return _canonical_invalid_nan()
    if product_inf:
        return _make_inf64(product_sign)
    if b_inf:
        return _make_inf64(b_sign)

    if a_zero or c_zero:
        if b_zero:
            # IEEE-754 RNE: exact zero from 0*x+0 is -0 only when both are -0.
            bits = encode_binary64(product_sign and b_sign, 0, 0)
            return _oracle_result(bits)
        return _oracle_result(b)

    a_sig = _significand64(a_exp, a_frac)
    c_sig = _significand64(c_exp, c_frac)
    a_unbiased = (a_exp - _B64_EXP_BIAS) if a_exp else (1 - _B64_EXP_BIAS)
    c_unbiased = (c_exp - _B64_EXP_BIAS) if c_exp else (1 - _B64_EXP_BIAS)
    product = a_sig * c_sig
    # product * 2^(a_u+c_u-104); _round_rne sees exp = a_u+c_u-52.
    product_scale = a_unbiased + c_unbiased

    if b_zero:
        bits, flags = _round_rne(
            product_sign,
            product_scale - _B64_FRAC_BITS,
            product,
            exp_bits=_B64_EXP_BITS,
            exp_bias=_B64_EXP_BIAS,
        )
        return _oracle_result(bits, flags)

    b_sig = _significand64(b_exp, b_frac)
    b_unbiased = (b_exp - _B64_EXP_BIAS) if b_exp else (1 - _B64_EXP_BIAS)
    # Align both operands to a 104-bit fraction scale.
    b_ext = b_sig << _B64_FRAC_BITS
    sticky = 0
    if product_scale >= b_unbiased:
        shift = product_scale - b_unbiased
        if shift >= (_B64_FRAC_BITS * 3 + 8):
            bits, flags = _round_rne(
                product_sign,
                product_scale - _B64_FRAC_BITS,
                product,
                exp_bits=_B64_EXP_BITS,
                exp_bias=_B64_EXP_BIAS,
            )
            return _oracle_result(bits, flags)
        if shift > 0:
            sticky = 1 if (b_ext & ((1 << shift) - 1)) else 0
            b_aligned = b_ext >> shift
        else:
            b_aligned = b_ext
        left_sign, left = product_sign, product
        right_sign, right = b_sign, b_aligned
        common = product_scale
    else:
        shift = b_unbiased - product_scale
        if shift >= (_B64_FRAC_BITS * 3 + 8):
            return _oracle_result(b)
        if shift > 0:
            sticky = 1 if (product & ((1 << shift) - 1)) else 0
            product_aligned = product >> shift
        else:
            product_aligned = product
        left_sign, left = product_sign, product_aligned
        right_sign, right = b_sign, b_ext
        common = b_unbiased

    if left_sign == right_sign:
        total = left + right
        sign = left_sign
    elif left > right:
        total = left - right
        sign = left_sign
    elif right > left:
        total = right - left
        sign = right_sign
    else:
        if sticky == 0:
            return _oracle_result(encode_binary64(False, 0, 0))
        raise _fail_unimplemented(
            "fused near-cancellation with sticky residue is not modeled yet",
        )

    bits, flags = _round_rne(
        sign,
        common - _B64_FRAC_BITS,
        total,
        exp_bits=_B64_EXP_BITS,
        exp_bias=_B64_EXP_BIAS,
    )
    if sticky and not flags.inexact:
        flags = FpOracleFlags(
            invalid=flags.invalid,
            divide_by_zero=flags.divide_by_zero,
            inexact=True,
            overflow=flags.overflow,
            underflow=flags.underflow,
        )
    return _oracle_result(bits, flags)


def fmsub_binary64_rne(a: int, c: int, b: int) -> FpOracleResult:
    """Fused multiply-sub ``a * c - b`` with one binary64 round-nearest-even."""
    b_neg = mask64(b ^ (1 << _B64_SIGN_SHIFT))
    return fmadd_binary64_rne(a, c, b_neg)


def _broadway_single_fma_midpoint_guard(precise_bits: int, addend_bits: int) -> None:
    """Fail closed on Dolphin/Broadway single-FMA midpoint ties with nonzero addend.

    When the exact binary64 FMA lands on the ``…10000000`` midpoint pattern,
    Dolphin applies a residual tie correction before the binary32 cast. The
    scaffold models the common ``addend == 0`` case (no correction) and rejects
    the nonzero-addend residue path rather than inventing host-float residuals.
    """
    if (precise_bits & 0x000000001FFFFFFF) != 0x0000000010000000:
        return
    _sign, exp, frac = decode_binary64(addend_bits)
    if exp == 0 and frac == 0:
        return
    raise _fail_unimplemented(
        "single fused midpoint tie with nonzero addend is not modeled yet",
    )


def fmadds_fpr_rne(a_fpr: int, b_fpr: int, c_fpr: int) -> FpOracleResult:
    """``fmadds`` on FPR-encoded operands (``frA * frC + frB`` → single in FPR).

    Caller must apply Broadway Force25 to ``c_fpr`` when matching hardware.
    """
    wide = fmadd_binary64_rne(a_fpr, c_fpr, b_fpr)
    _broadway_single_fma_midpoint_guard(wide.bits64, b_fpr)
    bits32, flags = _round_binary64_to_binary32_rne(wide.bits64)
    merged_flags = FpOracleFlags(
        invalid=wide.flags.invalid,
        divide_by_zero=wide.flags.divide_by_zero,
        inexact=wide.flags.inexact or flags.inexact,
        overflow=wide.flags.overflow or flags.overflow,
        underflow=wide.flags.underflow or flags.underflow,
    )
    bits64 = _single_to_fpr_bits(bits32)
    return FpOracleResult(bits64, merged_flags, fprf_from_binary64(bits64))


def fmsubs_fpr_rne(a_fpr: int, b_fpr: int, c_fpr: int) -> FpOracleResult:
    """``fmsubs`` on FPR-encoded operands (``frA * frC - frB`` → single in FPR)."""
    wide = fmsub_binary64_rne(a_fpr, c_fpr, b_fpr)
    _broadway_single_fma_midpoint_guard(wide.bits64, b_fpr)
    bits32, flags = _round_binary64_to_binary32_rne(wide.bits64)
    merged_flags = FpOracleFlags(
        invalid=wide.flags.invalid,
        divide_by_zero=wide.flags.divide_by_zero,
        inexact=wide.flags.inexact or flags.inexact,
        overflow=wide.flags.overflow or flags.overflow,
        underflow=wide.flags.underflow or flags.underflow,
    )
    bits64 = _single_to_fpr_bits(bits32)
    return FpOracleResult(bits64, merged_flags, fprf_from_binary64(bits64))


def _negate_finite_oracle_result(result: FpOracleResult) -> FpOracleResult:
    """Flip the sign bit of a finite fused result (``fn*`` forms).

    NaN payloads must never be sign-flipped; Inf/zero/finite results negate.
    """
    kind = classify_binary64(result.bits64)
    if _is_nan_class(kind):
        return result
    bits = mask64(result.bits64 ^ (1 << _B64_SIGN_SHIFT))
    return _oracle_result(bits, result.flags)


def fnmadd_binary64_rne(a: int, c: int, b: int) -> FpOracleResult:
    """Negative fused multiply-add ``-(a * c + b)`` (``fnmadd``)."""
    return _negate_finite_oracle_result(fmadd_binary64_rne(a, c, b))


def fnmsub_binary64_rne(a: int, c: int, b: int) -> FpOracleResult:
    """Negative fused multiply-sub ``-(a * c - b)`` (``fnmsub``)."""
    return _negate_finite_oracle_result(fmsub_binary64_rne(a, c, b))


def fnmadds_fpr_rne(a_fpr: int, b_fpr: int, c_fpr: int) -> FpOracleResult:
    """``fnmadds`` on FPR-encoded operands (``-(frA * frC + frB)`` → single)."""
    return _negate_finite_oracle_result(fmadds_fpr_rne(a_fpr, b_fpr, c_fpr))


def fnmsubs_fpr_rne(a_fpr: int, b_fpr: int, c_fpr: int) -> FpOracleResult:
    """``fnmsubs`` on FPR-encoded operands (``-(frA * frC - frB)`` → single)."""
    return _negate_finite_oracle_result(fmsubs_fpr_rne(a_fpr, b_fpr, c_fpr))


def dispatch_oracle(op: str, *operands: int) -> FpOracleResult:
    """Fail-closed dispatch table for supported scalar ops."""
    if op not in ORACLE_SUPPORTED_OPS:
        raise _fail_unimplemented(f"unsupported op {op!r}")
    if op == "fadd":
        if len(operands) != 2:
            raise ValueError("fadd expects two operands")
        return fadd_binary64_rne(operands[0], operands[1])
    if op == "fmul":
        if len(operands) != 2:
            raise ValueError("fmul expects two operands")
        return fmul_binary64_rne(operands[0], operands[1])
    if op == "fadds":
        if len(operands) != 2:
            raise ValueError("fadds expects two operands")
        return fadds_fpr_rne(operands[0], operands[1])
    if op == "fmuls":
        if len(operands) != 2:
            raise ValueError("fmuls expects two operands")
        return fmuls_fpr_rne(operands[0], operands[1])
    if op == "fsub":
        if len(operands) != 2:
            raise ValueError("fsub expects two operands")
        return fsub_binary64_rne(operands[0], operands[1])
    if op == "fdiv":
        if len(operands) != 2:
            raise ValueError("fdiv expects two operands")
        return fdiv_binary64_rne(operands[0], operands[1])
    if op == "fsubs":
        if len(operands) != 2:
            raise ValueError("fsubs expects two operands")
        return fsubs_fpr_rne(operands[0], operands[1])
    if op == "fdivs":
        if len(operands) != 2:
            raise ValueError("fdivs expects two operands")
        return fdivs_fpr_rne(operands[0], operands[1])
    if op == "fmadd":
        if len(operands) != 3:
            raise ValueError("fmadd expects three operands (a, c, b)")
        return fmadd_binary64_rne(operands[0], operands[1], operands[2])
    if op == "fmsub":
        if len(operands) != 3:
            raise ValueError("fmsub expects three operands (a, c, b)")
        return fmsub_binary64_rne(operands[0], operands[1], operands[2])
    if op == "fmadds":
        if len(operands) != 3:
            raise ValueError("fmadds expects three operands (a, b, c)")
        return fmadds_fpr_rne(operands[0], operands[1], operands[2])
    if op == "fmsubs":
        if len(operands) != 3:
            raise ValueError("fmsubs expects three operands (a, b, c)")
        return fmsubs_fpr_rne(operands[0], operands[1], operands[2])
    if op == "fnmadd":
        if len(operands) != 3:
            raise ValueError("fnmadd expects three operands (a, c, b)")
        return fnmadd_binary64_rne(operands[0], operands[1], operands[2])
    if op == "fnmsub":
        if len(operands) != 3:
            raise ValueError("fnmsub expects three operands (a, c, b)")
        return fnmsub_binary64_rne(operands[0], operands[1], operands[2])
    if op == "fnmadds":
        if len(operands) != 3:
            raise ValueError("fnmadds expects three operands (a, b, c)")
        return fnmadds_fpr_rne(operands[0], operands[1], operands[2])
    if op == "fnmsubs":
        if len(operands) != 3:
            raise ValueError("fnmsubs expects three operands (a, b, c)")
        return fnmsubs_fpr_rne(operands[0], operands[1], operands[2])
    raise _fail_unimplemented(f"unsupported op {op!r}")


def _signs_equal_bits(a: int, b: int) -> bool:
    return (((a ^ b) >> 63) & 1) == 0


def _is_zero_bits(bits: int) -> bool:
    _sign, exp, frac = decode_binary64(bits)
    return exp == 0 and frac == 0


def ps_lane_invalid_cause_add(a: int, b: int) -> int:
    """VX subcause mask for one ``ps_add`` lane."""
    from .fp_outcome import FPSCR_VXSNAN, FPSCR_VXISI

    cause = 0
    a_kind = classify_binary64(a)
    b_kind = classify_binary64(b)
    if a_kind is FpClass.SNAN or b_kind is FpClass.SNAN:
        cause |= FPSCR_VXSNAN
    if not _is_nan_class(a_kind) and not _is_nan_class(b_kind):
        if a_kind is FpClass.INFINITY and b_kind is FpClass.INFINITY:
            if not _signs_equal_bits(a, b):
                cause |= FPSCR_VXISI
    return cause


def ps_lane_invalid_cause_sub(a: int, b: int) -> int:
    """VX subcause mask for one ``ps_sub`` lane."""
    from .fp_outcome import FPSCR_VXSNAN, FPSCR_VXISI

    cause = 0
    a_kind = classify_binary64(a)
    b_kind = classify_binary64(b)
    if a_kind is FpClass.SNAN or b_kind is FpClass.SNAN:
        cause |= FPSCR_VXSNAN
    if not _is_nan_class(a_kind) and not _is_nan_class(b_kind):
        if a_kind is FpClass.INFINITY and b_kind is FpClass.INFINITY:
            if _signs_equal_bits(a, b):
                cause |= FPSCR_VXISI
    return cause


def ps_lane_invalid_cause_mul(a: int, b: int) -> int:
    """VX subcause mask for one ``ps_mul`` lane."""
    from .fp_outcome import FPSCR_VXIMZ, FPSCR_VXSNAN

    cause = 0
    a_kind = classify_binary64(a)
    b_kind = classify_binary64(b)
    if a_kind is FpClass.SNAN or b_kind is FpClass.SNAN:
        cause |= FPSCR_VXSNAN
    if not _is_nan_class(a_kind) and not _is_nan_class(b_kind):
        a_inf = a_kind is FpClass.INFINITY
        b_inf = b_kind is FpClass.INFINITY
        a_zero = _is_zero_bits(a)
        b_zero = _is_zero_bits(b)
        if (a_inf and b_zero) or (a_zero and b_inf):
            cause |= FPSCR_VXIMZ
    return cause


def ps_lane_invalid_cause_fused(
    a: int,
    b: int,
    c: int,
    *,
    subtract: bool,
) -> int:
    """VX subcause mask for one paired fused lane (``ps_m*`` family)."""
    from .fp_outcome import FPSCR_VXIMZ, FPSCR_VXISI, FPSCR_VXSNAN

    cause = 0
    a_kind = classify_binary64(a)
    b_kind = classify_binary64(b)
    c_kind = classify_binary64(c)
    if (
        a_kind is FpClass.SNAN
        or b_kind is FpClass.SNAN
        or c_kind is FpClass.SNAN
    ):
        cause |= FPSCR_VXSNAN
    if _is_nan_class(a_kind) or _is_nan_class(b_kind) or _is_nan_class(c_kind):
        return cause

    a_inf = a_kind is FpClass.INFINITY
    b_inf = b_kind is FpClass.INFINITY
    c_inf = c_kind is FpClass.INFINITY
    a_zero = _is_zero_bits(a)
    c_zero = _is_zero_bits(c)
    vximz = (a_inf and c_zero) or (a_zero and c_inf)
    if vximz:
        cause |= FPSCR_VXIMZ
        return cause

    product_infinite = (a_inf and not c_zero) or (c_inf and not a_zero)
    product_positive = _signs_equal_bits(a, c)
    b_positive = _signs_equal_bits(b, 0)
    product_matches_b = product_positive == b_positive
    invalid_signs = product_matches_b if subtract else not product_matches_b
    if product_infinite and b_inf and invalid_signs:
        cause |= FPSCR_VXISI
    return cause


def ps_lane_outcome(
    op: str,
    a_fpr: int,
    b_fpr: int,
    c_fpr: int | None = None,
) -> "FPOutcome":
    """Run one paired-single lane through the scalar single oracle.

    ``invalid_cause`` carries Broadway VX subcause bits; result bits follow
    NaN priority ``frA`` then ``frB`` then ``frC`` (fused) via the oracle.
    """
    from .fp_outcome import (
        PAIRED_ORACLE_OPS,
        outcome_from_oracle,
        unsupported_outcome,
    )

    a_fpr = mask64(a_fpr)
    b_fpr = mask64(b_fpr)
    if op not in PAIRED_ORACLE_OPS:
        return unsupported_outcome(
            f"paired-single oracle scaffold does not model {op!r}",
        )
    try:
        if op == "ps_add":
            result = fadds_fpr_rne(a_fpr, b_fpr)
            invalid_cause = ps_lane_invalid_cause_add(a_fpr, b_fpr)
        elif op == "ps_sub":
            result = fsubs_fpr_rne(a_fpr, b_fpr)
            invalid_cause = ps_lane_invalid_cause_sub(a_fpr, b_fpr)
        elif op == "ps_mul":
            result = fmuls_fpr_rne(a_fpr, b_fpr)
            invalid_cause = ps_lane_invalid_cause_mul(a_fpr, b_fpr)
        else:
            if c_fpr is None:
                raise ValueError(f"{op} requires three operands")
            c_fpr = mask64(c_fpr)
            subtract = op in ("ps_msub", "ps_nmsub")
            if op == "ps_madd":
                result = fmadds_fpr_rne(a_fpr, b_fpr, c_fpr)
            elif op == "ps_msub":
                result = fmsubs_fpr_rne(a_fpr, b_fpr, c_fpr)
            elif op == "ps_nmadd":
                result = fnmadds_fpr_rne(a_fpr, b_fpr, c_fpr)
            else:
                result = fnmsubs_fpr_rne(a_fpr, b_fpr, c_fpr)
            invalid_cause = ps_lane_invalid_cause_fused(
                a_fpr, b_fpr, c_fpr, subtract=subtract,
            )
    except OracleUnimplementedError as exc:
        return unsupported_outcome(str(exc))
    return outcome_from_oracle(result, invalid_cause=invalid_cause)
