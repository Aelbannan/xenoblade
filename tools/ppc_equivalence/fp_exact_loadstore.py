"""Exact Broadway FP load/store bit helpers (Phase 6).

Bit expansion/conversion for ``lfs``/``lfd``/``stfs*``/``stfiwx``. Full memory
bus integration remains in ``semantics.py``; this module exposes exact bit
transforms only. No host ``float``.
"""

from __future__ import annotations

from dataclasses import dataclass

from .fp_bits import mask32, mask64
from .fp_exact_outcome import ScalarFPOutcome
from .fp_exact_util import (
    exact_v2_enabled,
    rounding_mode_from_fpscr,
    unsupported_exact_outcome,
)
from .fp_oracle import _single_to_fpr_bits
from .fp_round import (
    BINARY32_FRAC_BITS,
    BINARY64_FRAC_BITS,
    FORMAT_BINARY32,
    round_to_format,
)


@dataclass(frozen=True, slots=True)
class LoadStoreBitOutcome:
    """Bit-level load/store transform with optional memory metadata."""

    scalar: ScalarFPOutcome
    memory_word: int | None = None
    memory_supported: bool = True
    memory_unsupported_reason: str | None = None


def expand_binary32_to_fpr(word32: int) -> int:
    """``lfs*``: memory binary32 → binary64 FPR storage."""
    return mask64(_single_to_fpr_bits(mask32(word32)))


def preserve_binary64_to_fpr(word64: int) -> int:
    """``lfd*``: preserve loaded 64-bit pattern in FPR."""
    return mask64(word64)


def round_fpr_to_binary32(fpr_bits: int, *, fpscr: int = 0) -> int:
    """``stfs*``: round FPR binary64 to memory binary32 under live RN."""
    from .fp_bits import FpClass, classify_binary64, decode_binary64

    bits = mask64(fpr_bits)
    mode = rounding_mode_from_fpscr(fpscr)
    kind = classify_binary64(bits)
    sign, exp, frac = decode_binary64(bits)
    if kind is FpClass.ZERO:
        return mask32(int(sign) << 31)
    if kind is FpClass.INFINITY:
        return mask32((int(sign) << 31) | (0xFF << 23))
    if kind in (FpClass.QNAN, FpClass.SNAN):
        payload = (frac >> 29) & ((1 << 23) - 1)
        if kind is FpClass.SNAN:
            payload |= 1 << 22
        return mask32((int(sign) << 31) | (0xFF << 23) | payload)
    sig = frac if exp == 0 else (1 << BINARY64_FRAC_BITS) | frac
    unbiased = (exp - 1023) if exp else (1 - 1023)
    adjusted_unbiased = unbiased - (BINARY64_FRAC_BITS - BINARY32_FRAC_BITS)
    rounded = round_to_format(sign, adjusted_unbiased, sig, FORMAT_BINARY32, mode)
    return mask32(rounded.bits)


def stfiwx_word_from_fpr(fpr_bits: int) -> int:
    """``stfiwx``: low 32 bits of FPR without FP conversion."""
    return mask32(fpr_bits)


def exact_lfs_bits(memory_word: int) -> LoadStoreBitOutcome:
    """Bit-exact ``lfs`` FPR write from a loaded word."""
    if not exact_v2_enabled():
        scalar = unsupported_exact_outcome("SCALAR_FP_EXACT_V2 disabled")
        return LoadStoreBitOutcome(scalar=scalar, memory_supported=False)
    result = expand_binary32_to_fpr(memory_word)
    scalar = ScalarFPOutcome(result_bits=result, writeback=True, supported=True)
    return LoadStoreBitOutcome(
        scalar=scalar,
        memory_word=mask32(memory_word),
        memory_supported=True,
    )


def exact_lfd_bits(memory_doubleword: int) -> LoadStoreBitOutcome:
    """Bit-exact ``lfd`` FPR write from a loaded doubleword."""
    if not exact_v2_enabled():
        scalar = unsupported_exact_outcome("SCALAR_FP_EXACT_V2 disabled")
        return LoadStoreBitOutcome(scalar=scalar, memory_supported=False)
    result = preserve_binary64_to_fpr(memory_doubleword)
    scalar = ScalarFPOutcome(result_bits=result, writeback=True, supported=True)
    return LoadStoreBitOutcome(
        scalar=scalar,
        memory_word=mask64(memory_doubleword),
        memory_supported=True,
    )


def exact_stfs_bits(fpr_bits: int, *, fpscr: int = 0) -> LoadStoreBitOutcome:
    """Bit-exact ``stfs`` memory word from FPR (RN from ``fpscr``)."""
    if not exact_v2_enabled():
        scalar = unsupported_exact_outcome("SCALAR_FP_EXACT_V2 disabled")
        return LoadStoreBitOutcome(scalar=scalar, memory_supported=False)
    word = round_fpr_to_binary32(fpr_bits, fpscr=fpscr)
    scalar = ScalarFPOutcome(result_bits=mask64(fpr_bits), writeback=False, supported=True)
    return LoadStoreBitOutcome(
        scalar=scalar,
        memory_word=word,
        memory_supported=True,
    )


def exact_stfd_bits(fpr_bits: int) -> LoadStoreBitOutcome:
    """Bit-exact ``stfd`` memory doubleword from FPR (preserve bits)."""
    if not exact_v2_enabled():
        scalar = unsupported_exact_outcome("SCALAR_FP_EXACT_V2 disabled")
        return LoadStoreBitOutcome(scalar=scalar, memory_supported=False)
    bits = preserve_binary64_to_fpr(fpr_bits)
    scalar = ScalarFPOutcome(result_bits=bits, writeback=False, supported=True)
    return LoadStoreBitOutcome(
        scalar=scalar,
        memory_word=bits,
        memory_supported=True,
    )


def exact_stfiwx_bits(fpr_bits: int) -> LoadStoreBitOutcome:
    """Bit-exact ``stfiwx`` memory word (FPR low 32, no conversion)."""
    if not exact_v2_enabled():
        scalar = unsupported_exact_outcome("SCALAR_FP_EXACT_V2 disabled")
        return LoadStoreBitOutcome(scalar=scalar, memory_supported=False)
    word = stfiwx_word_from_fpr(fpr_bits)
    scalar = ScalarFPOutcome(result_bits=mask64(fpr_bits), writeback=False, supported=True)
    return LoadStoreBitOutcome(
        scalar=scalar,
        memory_word=word,
        memory_supported=True,
    )


__all__ = [
    "LoadStoreBitOutcome",
    "expand_binary32_to_fpr",
    "exact_lfd_bits",
    "exact_lfs_bits",
    "exact_stfd_bits",
    "exact_stfs_bits",
    "exact_stfiwx_bits",
    "preserve_binary64_to_fpr",
    "round_fpr_to_binary32",
    "stfiwx_word_from_fpr",
]
