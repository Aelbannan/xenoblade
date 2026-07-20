"""Bit-level IEEE-754 helpers for the scalar-FP v2 exact core (Phase 2).

Pure integer classification, pack/unpack, leading-zero count, and
shift-with-sticky. Production APIs in this module must not use host ``float``,
``struct.pack('f'...)``, or other native FP conversions.
"""

from __future__ import annotations

from enum import Enum
from typing import Final

MASK32: Final[int] = 0xFFFFFFFF
MASK64: Final[int] = 0xFFFFFFFFFFFFFFFF

# Binary64 field layout.
_B64_SIGN_SHIFT = 63
_B64_EXP_SHIFT = 52
_B64_EXP_BITS = 11
_B64_FRAC_BITS = 52
_B64_EXP_MASK = (1 << _B64_EXP_BITS) - 1
_B64_FRAC_MASK = (1 << _B64_FRAC_BITS) - 1
_B64_QUIET_NAN = 1 << (_B64_FRAC_BITS - 1)
_B64_EXP_BIAS = 1023
_B64_EXP_MAX = _B64_EXP_MASK

# Binary32 field layout.
_B32_SIGN_SHIFT = 31
_B32_EXP_SHIFT = 23
_B32_EXP_BITS = 8
_B32_FRAC_BITS = 23
_B32_EXP_MASK = (1 << _B32_EXP_BITS) - 1
_B32_FRAC_MASK = (1 << _B32_FRAC_BITS) - 1
_B32_QUIET_NAN = 1 << (_B32_FRAC_BITS - 1)
_B32_EXP_BIAS = 127


class FpClass(str, Enum):
    """IEEE-754 value class for one encoded operand."""

    ZERO = "zero"
    SUBNORMAL = "subnormal"
    NORMAL = "normal"
    INFINITY = "infinity"
    QNAN = "qnan"
    SNAN = "snan"


def mask32(value: int) -> int:
    """Mask to 32 unsigned bits."""
    return value & MASK32


def mask64(value: int) -> int:
    """Mask to 64 unsigned bits."""
    return value & MASK64


def classify_binary64(bits: int) -> FpClass:
    """Classify a binary64 bit pattern."""
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
    """Classify a binary32 bit pattern."""
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
    """Return ``(sign, exponent, fraction)`` for binary64."""
    bits = mask64(bits)
    sign = bool(bits >> _B64_SIGN_SHIFT)
    exponent = (bits >> _B64_EXP_SHIFT) & _B64_EXP_MASK
    fraction = bits & _B64_FRAC_MASK
    return sign, exponent, fraction


def encode_binary64(sign: bool, exponent: int, fraction: int) -> int:
    """Pack ``(sign, exponent, fraction)`` into binary64."""
    value = (
        (int(sign) << _B64_SIGN_SHIFT)
        | ((exponent & _B64_EXP_MASK) << _B64_EXP_SHIFT)
        | (fraction & _B64_FRAC_MASK)
    )
    return mask64(value)


def decode_binary32(bits: int) -> tuple[bool, int, int]:
    """Return ``(sign, exponent, fraction)`` for binary32."""
    bits = mask32(bits)
    sign = bool(bits >> _B32_SIGN_SHIFT)
    exponent = (bits >> _B32_EXP_SHIFT) & _B32_EXP_MASK
    fraction = bits & _B32_FRAC_MASK
    return sign, exponent, fraction


def encode_binary32(sign: bool, exponent: int, fraction: int) -> int:
    """Pack ``(sign, exponent, fraction)`` into binary32."""
    value = (
        (int(sign) << _B32_SIGN_SHIFT)
        | ((exponent & _B32_EXP_MASK) << _B32_EXP_SHIFT)
        | (fraction & _B32_FRAC_MASK)
    )
    return mask32(value)


def significand_from_fields(exponent: int, fraction: int, *, frac_bits: int) -> int:
    """Build an integer significand with implicit bit at ``frac_bits``."""
    if exponent == 0:
        return fraction
    return (1 << frac_bits) | fraction


def leading_zero_count(value: int, *, width: int | None = None) -> int:
    """Count leading zero bits in ``value`` up to ``width`` (default: bit length)."""
    if value <= 0:
        return width if width is not None else 0
    msb = value.bit_length() - 1
    if width is None:
        return 0
    return max(0, width - 1 - msb)


def flush_denormal_binary64(bits: int) -> int:
    """Flush a binary64 denormal (zero exponent field) to signed zero."""
    bits = mask64(bits)
    if (bits >> _B64_EXP_SHIFT) & _B64_EXP_MASK == 0:
        return bits & (1 << _B64_SIGN_SHIFT)
    return bits


def flush_denormal_binary32(bits: int) -> int:
    """Flush a binary32 denormal (zero exponent field) to signed zero."""
    bits = mask32(bits)
    if (bits >> _B32_EXP_SHIFT) & _B32_EXP_MASK == 0:
        return bits & (1 << _B32_SIGN_SHIFT)
    return bits


def signed_zero_binary64(sign: bool) -> int:
    """Return ±0.0 as a binary64 bit pattern."""
    return mask64(int(sign) << _B64_SIGN_SHIFT)


def shift_right_sticky(
    significand: int,
    shift: int,
) -> tuple[int, bool, bool, bool, bool]:
    """Right-shift ``significand`` by ``shift`` with IEEE guard/round/sticky.

    Returns ``(shifted, guard, round_bit, sticky, discarded_nonzero)``.

    When ``shift <= 0`` the significand is left-shifted by ``-shift`` and all
    discarded-bit indicators are false.
    """
    if shift <= 0:
        return significand << (-shift), False, False, False, False

    lost_mask = (1 << shift) - 1
    lost = significand & lost_mask
    shifted = significand >> shift
    discarded_nonzero = lost != 0
    if shift == 1:
        guard = bool(lost & 1)
        return shifted, guard, False, False, discarded_nonzero
    guard = bool((significand >> (shift - 1)) & 1)
    round_bit = bool((significand >> (shift - 2)) & 1) if shift >= 2 else False
    sticky_mask = (1 << max(0, shift - 2)) - 1
    sticky = bool(lost & sticky_mask) if shift >= 2 else False
    return shifted, guard, round_bit, sticky, discarded_nonzero


__all__ = [
    "FpClass",
    "MASK32",
    "MASK64",
    "classify_binary32",
    "classify_binary64",
    "decode_binary32",
    "decode_binary64",
    "encode_binary32",
    "encode_binary64",
    "flush_denormal_binary32",
    "flush_denormal_binary64",
    "leading_zero_count",
    "mask32",
    "mask64",
    "shift_right_sticky",
    "significand_from_fields",
    "signed_zero_binary64",
]
