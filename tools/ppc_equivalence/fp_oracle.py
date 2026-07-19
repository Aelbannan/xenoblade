"""Bit-level floating-point oracle scaffold (SoftFloat-style prep).

This module provides pure-Python IEEE-754 bit manipulations for a small set of
scalar ops. It deliberately avoids host ``float`` / ``struct.pack`` on
proof-critical paths so a future migration can treat results as independently
checkable bit patterns.

**Not production-complete:** exception propagation, Force25, paired-single lanes,
and most FPSCR sticky-flag updates remain partial or unimplemented. Unhandled
cases fail closed via :class:`OracleUnimplementedError`.

**Partially wired:** ``ConcreteOps`` routes ``fadd``/``fadds``/``fmul``/``fmuls``
through this oracle (fail-closed elsewhere). SymbolicOps and other FP paths still
use host float or Z3. Nothing here promotes FP proofs out of Tier C.
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


def _require_finite_normal_or_zero(*values: int) -> None:
    for bits in values:
        kind = classify_binary64(bits)
        if kind in (FpClass.SNAN, FpClass.QNAN, FpClass.INFINITY):
            raise _fail_unimplemented(f"non-finite operand class {kind.value}")
        if kind is FpClass.SUBNORMAL:
            raise _fail_unimplemented("subnormal operands are not modeled yet")


def _significand64(exponent: int, fraction: int) -> int:
    if exponent == 0:
        return fraction
    return (1 << _B64_FRAC_BITS) | fraction


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
    if top_bit > _B64_FRAC_BITS + 1:
        shift = top_bit - (_B64_FRAC_BITS + 1)
        lost = sig & ((1 << shift) - 1)
        sig >>= shift
        exp_unbiased += shift
        if lost:
            flags = FpOracleFlags(inexact=True)
            if lost > (1 << (shift - 1)) or (
                lost == (1 << (shift - 1)) and (sig & 1)
            ):
                sig += 1
                flags = FpOracleFlags(inexact=True)
    elif top_bit < _B64_FRAC_BITS:
        shift = _B64_FRAC_BITS - top_bit
        exp_unbiased -= shift
        sig <<= shift

    exp_max_unbiased = (1 << exp_bits) - 2 - exp_bias
    exp_min_unbiased = 1 - exp_bias
    if exp_unbiased > exp_max_unbiased:
        raise _fail_unimplemented("finite overflow is excluded by default domain")
    if exp_unbiased < exp_min_unbiased:
        raise _fail_unimplemented("subnormal/underflow result is not modeled yet")

    while sig >= (1 << (_B64_FRAC_BITS + 1)):
        sig >>= 1
        exp_unbiased += 1

    encoded_frac = sig & _B64_FRAC_MASK
    encoded_exp = exp_unbiased + exp_bias
    return encode_binary64(sign, encoded_exp, encoded_frac), flags


def fadd_binary64_rne(a: int, b: int) -> FpOracleResult:
    """Add two binary64 operands with round-nearest-even."""
    _require_finite_normal_or_zero(a, b)
    a_sign, a_exp, a_frac = decode_binary64(a)
    b_sign, b_exp, b_frac = decode_binary64(b)
    if a_exp == 0 and a_frac == 0:
        bits = mask64(b)
        return FpOracleResult(bits, FpOracleFlags(), fprf_from_binary64(bits))
    if b_exp == 0 and b_frac == 0:
        bits = mask64(a)
        return FpOracleResult(bits, FpOracleFlags(), fprf_from_binary64(bits))

    a_sig = _significand64(a_exp, a_frac)
    b_sig = _significand64(b_exp, b_frac)
    a_unbiased = (a_exp - _B64_EXP_BIAS) if a_exp else (1 - _B64_EXP_BIAS)
    b_unbiased = (b_exp - _B64_EXP_BIAS) if b_exp else (1 - _B64_EXP_BIAS)

    if a_unbiased < b_unbiased or (a_unbiased == b_unbiased and b_sig > a_sig):
        a_sign, b_sign = b_sign, a_sign
        a_sig, b_sig = b_sig, a_sig
        a_unbiased, b_unbiased = b_unbiased, a_unbiased

    shift = a_unbiased - b_unbiased
    if shift >= _B64_FRAC_BITS + 2:
        bits = encode_binary64(a_sign, a_exp, a_frac)
        return FpOracleResult(bits, FpOracleFlags(), fprf_from_binary64(bits))

    b_sig >>= shift
    if a_sign == b_sign:
        sig = a_sig + b_sig
        sign = a_sign
        exp_unbiased = a_unbiased
    else:
        sig = a_sig - b_sig
        sign = a_sign
        exp_unbiased = a_unbiased

    bits, flags = _round_rne(
        sign,
        exp_unbiased,
        sig,
        exp_bits=_B64_EXP_BITS,
        exp_bias=_B64_EXP_BIAS,
    )
    return FpOracleResult(bits, flags, fprf_from_binary64(bits))


def fmul_binary64_rne(a: int, b: int) -> FpOracleResult:
    """Multiply two binary64 operands with round-nearest-even."""
    _require_finite_normal_or_zero(a, b)
    a_sign, a_exp, a_frac = decode_binary64(a)
    b_sign, b_exp, b_frac = decode_binary64(b)
    if (a_exp == 0 and a_frac == 0) or (b_exp == 0 and b_frac == 0):
        sign = a_sign ^ b_sign
        bits = encode_binary64(sign, 0, 0)
        return FpOracleResult(bits, FpOracleFlags(), fprf_from_binary64(bits))

    a_sig = _significand64(a_exp, a_frac)
    b_sig = _significand64(b_exp, b_frac)
    sign = a_sign ^ b_sign
    a_unbiased = (a_exp - _B64_EXP_BIAS) if a_exp else (1 - _B64_EXP_BIAS)
    b_unbiased = (b_exp - _B64_EXP_BIAS) if b_exp else (1 - _B64_EXP_BIAS)
    sig = (a_sig * b_sig) >> _B64_FRAC_BITS
    exp_unbiased = a_unbiased + b_unbiased
    bits, flags = _round_rne(
        sign,
        exp_unbiased,
        sig,
        exp_bits=_B64_EXP_BITS,
        exp_bias=_B64_EXP_BIAS,
    )
    return FpOracleResult(bits, flags, fprf_from_binary64(bits))


def _round_binary64_to_binary32_rne(bits64: int) -> tuple[int, FpOracleFlags]:
    sign, exp, frac = decode_binary64(bits64)
    if exp == _B64_EXP_MAX:
        raise _fail_unimplemented("non-finite to-single conversion")
    if exp == 0 and frac == 0:
        return encode_binary32(sign, 0, 0), FpOracleFlags()

    sig = _significand64(exp, frac)
    unbiased = (exp - _B64_EXP_BIAS) if exp else (1 - _B64_EXP_BIAS)
    flags = FpOracleFlags()

    shift = _B64_FRAC_BITS - _B32_FRAC_BITS
    lost = sig & ((1 << shift) - 1)
    sig >>= shift
    if lost:
        flags = FpOracleFlags(inexact=True)
        if lost > (1 << (shift - 1)) or (
            lost == (1 << (shift - 1)) and (sig & 1)
        ):
            sig += 1
            flags = FpOracleFlags(inexact=True)

    while sig >= (1 << (_B32_FRAC_BITS + 1)):
        sig >>= 1
        unbiased += 1

    if unbiased > (1 << _B32_EXP_BITS) - 2 - _B32_EXP_BIAS:
        raise _fail_unimplemented("single-precision overflow is not modeled yet")
    if unbiased < 1 - _B32_EXP_BIAS:
        raise _fail_unimplemented("single-precision subnormal result is not modeled yet")

    encoded_frac = sig & _B32_FRAC_MASK
    encoded_exp = unbiased + _B32_EXP_BIAS
    return encode_binary32(sign, encoded_exp, encoded_frac), flags


def _single_to_fpr_bits(bits32: int) -> int:
    """Expand a binary32 value into Broadway FPR double-word storage."""
    sign, exp, frac = decode_binary32(bits32)
    if exp == 0 and frac == 0:
        return encode_binary64(sign, 0, 0)
    if exp == _B32_EXP_MASK:
        raise _fail_unimplemented("non-finite single expansion")
    return encode_binary64(sign, exp - _B32_EXP_BIAS + _B64_EXP_BIAS, frac << (_B64_FRAC_BITS - _B32_FRAC_BITS))


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
    raise _fail_unimplemented(f"unsupported op {op!r}")
