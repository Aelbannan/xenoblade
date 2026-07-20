"""Shared helpers for the SCALAR_FP_V2 exact opcode path (Phase 6+).

Pure integer / bitvector utilities — no host ``float`` on production paths.
"""

from __future__ import annotations

from fractions import Fraction

from .fp_bits import (
    FpClass,
    classify_binary32,
    classify_binary64,
    decode_binary32,
    decode_binary64,
    encode_binary64,
    mask32,
    mask64,
)
from .fp_capabilities import scalar_fp_exact_v2_enabled
from .fp_exact_outcome import FiFrPolicy, ScalarFPOutcome
from .fp_fpscr import FPSCR_NI, FPSCR_VXCVI, FPSCR_VXSNAN, FPSCR_VXVC, FPSCR_XX
from .fp_oracle import (
    fprf_from_binary64,
    ni_force_single_result_bits,
    ni_flush_operand_binary64,
)
from .fp_rounding import RoundingMode

_POSITIVE_INT_LIMIT = 0x41E0000000000000
_NEGATIVE_INT_LIMIT = 0xC1E0000000000000
_FCTIW_RESULT_PREFIX = 0xFFF8000000000000


def exact_v2_enabled() -> bool:
    """True when the experimental exact scalar-FP v2 path is active."""
    return scalar_fp_exact_v2_enabled()


def unsupported_exact_outcome(reason: str) -> ScalarFPOutcome:
    """Fail-closed ``ScalarFPOutcome`` when v2 is disabled or incomplete."""
    return ScalarFPOutcome(
        result_bits=0,
        writeback=False,
        supported=False,
        unsupported_reason=reason,
    )


def rounding_mode_from_fpscr(fpscr: int) -> RoundingMode:
    """Decode FPSCR[0:1] into a Broadway rounding mode."""
    rn = int(fpscr) & 3
    return {
        0: RoundingMode.NEAREST_EVEN,
        1: RoundingMode.TOWARD_ZERO,
        2: RoundingMode.TOWARD_PLUS_INFINITY,
        3: RoundingMode.TOWARD_MINUS_INFINITY,
    }[rn]


def ni_from_fpscr(fpscr: int) -> bool:
    """Return True when FPSCR.NI is set."""
    return bool(int(fpscr) & FPSCR_NI)


def is_snan_binary64(bits: int) -> bool:
    """True when ``bits`` encodes a binary64 signaling NaN."""
    return classify_binary64(bits) is FpClass.SNAN


def is_nan_binary64(bits: int) -> bool:
    """True when ``bits`` encodes any binary64 NaN."""
    kind = classify_binary64(bits)
    return kind in (FpClass.QNAN, FpClass.SNAN)


def quiet_nan_binary64(bits: int) -> int:
    """Quiet a binary64 NaN payload (Broadway compare / convert paths)."""
    bits = mask64(bits)
    return bits | 0x0008000000000000


def binary64_to_fraction(bits: int) -> Fraction | None:
    """Exact rational value for finite binary64; ``None`` for NaN/Inf."""
    bits = mask64(bits)
    kind = classify_binary64(bits)
    if kind in (FpClass.QNAN, FpClass.SNAN, FpClass.INFINITY):
        return None
    sign, exp, frac = decode_binary64(bits)
    if exp == 0:
        if frac == 0:
            magnitude = Fraction(0)
        else:
            magnitude = Fraction(frac, 1 << 52) * Fraction(2) ** (1 - 1023)
    else:
        sig = (1 << 52) | frac
        magnitude = Fraction(sig, 1 << 52) * Fraction(2) ** (exp - 1023)
    return -magnitude if sign else magnitude


def compare_binary64_magnitude(a: int, b: int) -> int:
    """Return ``-1``, ``0``, or ``1`` for ordered finite magnitudes (+0 == -0)."""
    a = mask64(a)
    b = mask64(b)
    if a == b:
        return 0
    a_sign, a_exp, a_frac = decode_binary64(a)
    b_sign, b_exp, b_frac = decode_binary64(b)
    a_zero = a_exp == 0 and a_frac == 0
    b_zero = b_exp == 0 and b_frac == 0
    if a_zero and b_zero:
        return 0
    if a_sign != b_sign:
        return -1 if a_sign else 1
    a_mag = (a_exp, a_frac) if a_exp else (0, a_frac)
    b_mag = (b_exp, b_frac) if b_exp else (0, b_frac)
    if a_mag == b_mag:
        return 0
    less = a_mag < b_mag
    if a_sign:
        return 1 if less else -1
    return -1 if less else 1


def fpcc_nibble(*, lt: bool, gt: bool, eq: bool, unordered: bool) -> int:
    """Assemble a four-bit FPCC / CR compare field."""
    value = 0
    if lt:
        value |= 8
    if gt:
        value |= 4
    if eq:
        value |= 2
    if unordered:
        value |= 1
    return value & 0xF


def fctiw_integer_result_bits(value32: int, *, negative_zero: bool) -> int:
    """Pack a signed 32-bit integer into Broadway ``fctiw`` FPR format."""
    marker = 1 << 32 if negative_zero else 0
    return mask64(_FCTIW_RESULT_PREFIX | marker | (mask32(value32)))


def apply_ni_operand(bits: int, fpscr: int) -> int:
    """NI operand flush when FPSCR.NI=1 (Table 2-24)."""
    return ni_flush_operand_binary64(bits, ni=ni_from_fpscr(fpscr))


def apply_ni_single_result(bits: int, fpscr: int) -> int:
    """NI single-result flush (``frsp`` / ForceSingle quirk)."""
    return ni_force_single_result_bits(bits, ni=ni_from_fpscr(fpscr))


__all__ = [
    "FiFrPolicy",
    "FPSCR_VXCVI",
    "FPSCR_VXSNAN",
    "FPSCR_VXVC",
    "FPSCR_XX",
    "ScalarFPOutcome",
    "_NEGATIVE_INT_LIMIT",
    "_POSITIVE_INT_LIMIT",
    "apply_ni_operand",
    "apply_ni_single_result",
    "binary64_to_fraction",
    "compare_binary64_magnitude",
    "exact_v2_enabled",
    "fctiw_integer_result_bits",
    "fpcc_nibble",
    "fprf_from_binary64",
    "is_nan_binary64",
    "is_snan_binary64",
    "ni_from_fpscr",
    "quiet_nan_binary64",
    "rounding_mode_from_fpscr",
    "unsupported_exact_outcome",
]
