"""Exact Broadway FP estimate/select ops (Phase 6 — ``fres``, ``frsqrte``, ``fsel``).

``fres`` / ``frsqrte`` use the Broadway estimate tables from ``semantics.py``.
``fsel`` is bit-preserving (no arithmetic conversion). No host ``float``.
"""

from __future__ import annotations

from .fp_bits import mask32, mask64
from .fp_exact_outcome import FiFrPolicy, ScalarFPOutcome
from .fp_exact_util import (
    exact_v2_enabled,
    fprf_from_binary64,
    is_nan_binary64,
    quiet_nan_binary64,
    unsupported_exact_outcome,
)
from .fp_fpscr import FPSCR_VXSQRT
from .semantics import FRES_TABLE, FRSQRTE_TABLE


def exact_fres(source_bits: int) -> ScalarFPOutcome:
    """Reciprocal estimate ``fres`` (table-driven)."""
    if not exact_v2_enabled():
        return unsupported_exact_outcome("SCALAR_FP_EXACT_V2 disabled")

    bits = mask64(source_bits)
    sign = bits & (1 << 63)
    exponent = (bits >> 52) & 0x7FF
    mantissa = bits & ((1 << 52) - 1)

    if exponent == 0 and mantissa == 0:
        result = sign | 0x7FF0000000000000
    elif exponent == 0x7FF:
        result = sign if mantissa == 0 else quiet_nan_binary64(bits)
    elif exponent < 895:
        result = sign | 0x47EFFFFFE0000000
    elif exponent >= 1149:
        result = sign
    else:
        out_exponent = 0x7FD - exponent
        index = mantissa >> 37
        base, decrement = FRES_TABLE[index // 1024]
        estimate = base - (decrement * (index % 1024) + 1) // 2
        result = sign | (out_exponent << 52) | (estimate << 29)

    result = mask64(result)
    return ScalarFPOutcome(
        result_bits=result,
        fi_fr_policy=FiFrPolicy.PRESERVE,
        fprf=fprf_from_binary64(result),
        writeback=True,
        supported=True,
    )


def exact_frsqrte(source_bits: int) -> ScalarFPOutcome:
    """Reciprocal square-root estimate ``frsqrte`` (table-driven)."""
    if not exact_v2_enabled():
        return unsupported_exact_outcome("SCALAR_FP_EXACT_V2 disabled")

    bits = mask64(source_bits)
    sign = bits & (1 << 63)
    exponent = (bits >> 52) & 0x7FF
    mantissa = bits & ((1 << 52) - 1)
    raised = 0

    if exponent == 0 and mantissa == 0:
        result = sign | 0x7FF0000000000000
    elif exponent == 0x7FF:
        if mantissa == 0:
            result = 0x7FF8000000000000 if sign else 0
        else:
            result = quiet_nan_binary64(bits)
    elif sign:
        raised |= FPSCR_VXSQRT
        result = 0x7FF8000000000000
    else:
        if exponent == 0:
            while not (mantissa & (1 << 52)):
                exponent -= 1
                mantissa <<= 1
            mantissa &= (1 << 52) - 1
            exponent += 1
        exponent_lsb = exponent & 1
        difference = exponent - 0x3FE
        half = (difference + 1) // 2
        out_exponent = (0x3FF - half) & 0x7FF
        index = ((exponent_lsb << 52) | mantissa) >> 37
        base, decrement = FRSQRTE_TABLE[index // 2048]
        estimate = base + decrement * (index % 2048)
        result = (out_exponent << 52) | (estimate << 26)

    result = mask64(result)
    return ScalarFPOutcome(
        result_bits=result,
        raised_causes=raised,
        fi_fr_policy=FiFrPolicy.PRESERVE,
        fprf=fprf_from_binary64(result),
        writeback=True,
        supported=True,
    )


def exact_fsel(a_bits: int, c_bits: int, b_bits: int) -> ScalarFPOutcome:
    """Floating select ``fsel`` — bit-preserving, no arithmetic conversion."""
    if not exact_v2_enabled():
        return unsupported_exact_outcome("SCALAR_FP_EXACT_V2 disabled")

    a = mask64(a_bits)
    c = mask64(c_bits)
    b = mask64(b_bits)
    # ``frA >= 0.0`` is false for NaN; otherwise sign bit determines polarity.
    if is_nan_binary64(a):
        select_c = False
    else:
        select_c = (a >> 63) == 0
    result = c if select_c else b
    return ScalarFPOutcome(
        result_bits=result,
        fi_fr_policy=FiFrPolicy.PRESERVE,
        fprf=fprf_from_binary64(result),
        writeback=True,
        supported=True,
    )


__all__ = [
    "FRES_TABLE",
    "FRSQRTE_TABLE",
    "exact_fres",
    "exact_frsqrte",
    "exact_fsel",
]
