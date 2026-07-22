"""Non-fused exact scalar FP arithmetic (SCALAR_FP_V2 Phase 3).

Bit-exact Broadway ``fadd*`` / ``fsub*`` / ``fmul*`` / ``fdiv*`` using the
shared rounding kernel (:mod:`fp_round`). No host ``float`` on production paths.

Callers gate on ``SCALAR_FP_EXACT_V2`` (see :mod:`semantics`); these functions
always compute the exact outcome for the supplied operand bits and FPSCR.
"""

from __future__ import annotations

from dataclasses import replace
from typing import Final

from .fp_bits import (
    FpClass,
    classify_binary64,
    decode_binary32,
    decode_binary64,
    encode_binary32,
    encode_binary64,
    mask32,
    mask64,
    significand_from_fields,
)
from .fp_exact_outcome import FiFrPolicy, ScalarFPOutcome
from .fp_exact_util import (
    adjust_binary64_ulp,
    apply_ni_operand,
    apply_ni_single_result,
    force_25bit,
    fprf_from_binary64,
    ni_from_fpscr,
    rounding_mode_from_fpscr,
)
from .fp_fpscr import (
    FPSCR_OX,
    FPSCR_UX,
    FPSCR_VE,
    FPSCR_VX_ANY,
    FPSCR_VXIDI,
    FPSCR_VXIMZ,
    FPSCR_VXISI,
    FPSCR_VXSNAN,
    FPSCR_VXZDZ,
    FPSCR_XX,
    FPSCR_ZX,
)
from .fp_oracle import ni_flush_result_binary64
from .fp_round import (
    BINARY32_EXP_BIAS,
    BINARY32_EXP_BITS,
    BINARY32_FRAC_BITS,
    BINARY64_EXP_BIAS,
    BINARY64_EXP_BITS,
    BINARY64_FRAC_BITS,
    RoundGRS,
    RoundOutcome,
    round_binary32,
    round_binary64,
)
from .fp_rounding import RoundingMode

_B64_SIGN_SHIFT: Final[int] = 63
_B64_EXP_MAX: Final[int] = (1 << BINARY64_EXP_BITS) - 1
_B64_FRAC_MASK: Final[int] = (1 << BINARY64_FRAC_BITS) - 1
_B64_QUIET_NAN: Final[int] = 1 << (BINARY64_FRAC_BITS - 1)
_B64_CANONICAL_NAN: Final[int] = 0x7FF8000000000000

_B32_EXP_MASK: Final[int] = (1 << BINARY32_EXP_BITS) - 1
_B32_FRAC_MASK: Final[int] = (1 << BINARY32_FRAC_BITS) - 1
_B32_QUIET_NAN: Final[int] = 1 << (BINARY32_FRAC_BITS - 1)

EXACT_SCALAR_ARITH_OPS: Final[frozenset[str]] = frozenset({
    "fadd",
    "fadds",
    "fsub",
    "fsubs",
    "fmul",
    "fmuls",
    "fdiv",
    "fdivs",
})


def _is_nan_class(kind: FpClass) -> bool:
    return kind in (FpClass.QNAN, FpClass.SNAN)


def _is_zero64(exp: int, frac: int) -> bool:
    return exp == 0 and frac == 0


def _quiet_nan64(bits: int) -> int:
    return mask64(bits | _B64_QUIET_NAN)


def _significand64(exponent: int, fraction: int) -> int:
    return significand_from_fields(exponent, fraction, frac_bits=BINARY64_FRAC_BITS)


def _infinity_bits64(sign: bool) -> int:
    return encode_binary64(sign, _B64_EXP_MAX, 0)


def _single_to_fpr_bits(bits32: int) -> int:
    """Expand a binary32 value into Broadway FPR double-word storage."""
    sign, exp, frac = decode_binary32(bits32)
    if exp == 0 and frac == 0:
        return encode_binary64(sign, 0, 0)
    if exp == _B32_EXP_MASK:
        if frac == 0:
            return encode_binary64(sign, _B64_EXP_MAX, 0)
        return encode_binary64(
            sign,
            _B64_EXP_MAX,
            (frac | _B32_QUIET_NAN) << (BINARY64_FRAC_BITS - BINARY32_FRAC_BITS),
        )
    if exp == 0:
        shift = BINARY32_FRAC_BITS - frac.bit_length()
        frac <<= shift + 1
        unbiased = (1 - BINARY32_EXP_BIAS) - (shift + 1)
        return encode_binary64(
            sign,
            unbiased + BINARY64_EXP_BIAS,
            (frac << (BINARY64_FRAC_BITS - BINARY32_FRAC_BITS)) & _B64_FRAC_MASK,
        )
    return encode_binary64(
        sign,
        exp - BINARY32_EXP_BIAS + BINARY64_EXP_BIAS,
        frac << (BINARY64_FRAC_BITS - BINARY32_FRAC_BITS),
    )


def _propagate_nan64(*operands: int) -> tuple[int, int]:
    """Return ``(quiet_nan_bits64, invalid_cause)`` with Broadway NaN priority."""
    invalid_cause = 0
    chosen: int | None = None
    for bits in operands:
        kind = classify_binary64(bits)
        if kind is FpClass.SNAN:
            invalid_cause |= FPSCR_VXSNAN
            if chosen is None:
                chosen = _quiet_nan64(bits)
        elif kind is FpClass.QNAN and chosen is None:
            chosen = mask64(bits)
    if chosen is None:
        return _B64_CANONICAL_NAN, FPSCR_VXSNAN
    return chosen, invalid_cause


def _writeback_from_causes(fpscr: int, raised_causes: int) -> bool:
    """Suppress destination write when VE is enabled and a VX subcause fired."""
    invalid = bool(int(raised_causes) & FPSCR_VX_ANY)
    ve = bool(int(fpscr) & FPSCR_VE)
    return not (invalid and ve)


def _raised_from_round(
    round_out: RoundOutcome,
    *,
    invalid_cause: int = 0,
    divide_by_zero: bool = False,
) -> int:
    causes = int(invalid_cause)
    if round_out.overflow:
        causes |= FPSCR_OX
    if round_out.underflow:
        causes |= FPSCR_UX
    if divide_by_zero:
        causes |= FPSCR_ZX
    if round_out.inexact:
        causes |= FPSCR_XX
    return causes


def _outcome_special(
    bits: int,
    *,
    fpscr: int,
    invalid_cause: int = 0,
    divide_by_zero: bool = False,
) -> ScalarFPOutcome:
    bits = mask64(bits)
    causes = int(invalid_cause)
    if divide_by_zero:
        causes |= FPSCR_ZX
    return ScalarFPOutcome(
        result_bits=bits,
        raised_causes=causes,
        fi=False,
        fr=False,
        fi_fr_policy=FiFrPolicy.CLEAR,
        fprf=fprf_from_binary64(bits),
        writeback=_writeback_from_causes(fpscr, causes),
        supported=True,
    )


def _outcome_from_round64(
    round_out: RoundOutcome,
    *,
    fpscr: int,
    invalid_cause: int = 0,
    divide_by_zero: bool = False,
    ni: bool = False,
    is_single: bool = False,
    clear_fi_fr: bool = False,
) -> ScalarFPOutcome:
    bits = mask64(round_out.bits)
    if is_single:
        bits = _single_to_fpr_bits(bits)
        if ni:
            bits = apply_ni_single_result(bits, fpscr)
    elif ni:
        bits = ni_flush_result_binary64(bits, ni=True)
    causes = _raised_from_round(
        round_out,
        invalid_cause=invalid_cause,
        divide_by_zero=divide_by_zero,
    )
    clear = clear_fi_fr or bool(causes & FPSCR_VX_ANY)
    return ScalarFPOutcome(
        result_bits=bits,
        raised_causes=causes,
        fi=False if clear else round_out.fi,
        fr=False if clear else round_out.fr,
        fi_fr_policy=FiFrPolicy.CLEAR,
        fprf=fprf_from_binary64(bits),
        writeback=_writeback_from_causes(fpscr, causes),
        supported=True,
    )


def _round_wide_to_single(
    wide_bits: int,
    mode: RoundingMode,
) -> RoundOutcome:
    """Round a binary64-wide value to binary32."""
    kind = classify_binary64(wide_bits)
    sign, exp, frac = decode_binary64(wide_bits)
    if kind is FpClass.INFINITY:
        bits32 = encode_binary32(sign, _B32_EXP_MASK, 0)
        return RoundOutcome(
            bits=bits32,
            sign=sign,
            exp_unbiased=0,
            significand=0,
            exact=True,
            inexact=False,
            overflow=False,
            underflow=False,
            grs=RoundGRS(False, False, False, False),
            magnitude_increased=False,
            pre_round_tiny=False,
            post_round_tiny=False,
            overflow_to_infinity=False,
            fi=False,
            fr=False,
        )
    if _is_nan_class(kind):
        quieted = _quiet_nan64(wide_bits)
        _qs, _qe, qfrac = decode_binary64(quieted)
        frac32 = (qfrac >> (BINARY64_FRAC_BITS - BINARY32_FRAC_BITS)) & _B32_FRAC_MASK
        if frac32 == 0:
            frac32 = _B32_QUIET_NAN
        bits32 = encode_binary32(sign, _B32_EXP_MASK, frac32)
        return RoundOutcome(
            bits=bits32,
            sign=sign,
            exp_unbiased=0,
            significand=0,
            exact=True,
            inexact=False,
            overflow=False,
            underflow=False,
            grs=RoundGRS(False, False, False, False),
            magnitude_increased=False,
            pre_round_tiny=False,
            post_round_tiny=False,
            overflow_to_infinity=False,
            fi=False,
            fr=False,
        )
    if exp == 0 and frac == 0:
        bits32 = encode_binary32(sign, 0, 0)
        return RoundOutcome(
            bits=bits32,
            sign=sign,
            exp_unbiased=1 - BINARY32_EXP_BIAS,
            significand=0,
            exact=True,
            inexact=False,
            overflow=False,
            underflow=False,
            grs=RoundGRS(False, False, False, False),
            magnitude_increased=False,
            pre_round_tiny=False,
            post_round_tiny=True,
            overflow_to_infinity=False,
            fi=False,
            fr=False,
        )

    sig = _significand64(exp, frac)
    unbiased = (exp - BINARY64_EXP_BIAS) if exp else (1 - BINARY64_EXP_BIAS)
    # Wide significand carries the implicit bit at 52; binary32 expects it at 23.
    return round_binary32(
        sign,
        unbiased - (BINARY64_FRAC_BITS - BINARY32_FRAC_BITS),
        sig,
        mode,
    )


def _prepare_operands(
    a_bits: int,
    b_bits: int,
    *,
    fpscr: int,
) -> tuple[int, int, bool, RoundingMode]:
    ni = ni_from_fpscr(fpscr)
    mode = rounding_mode_from_fpscr(fpscr)
    a = apply_ni_operand(mask64(a_bits), fpscr)
    b = apply_ni_operand(mask64(b_bits), fpscr)
    return a, b, ni, mode


def _prepare_mul_operands(
    a_bits: int,
    c_bits: int,
    *,
    fpscr: int,
) -> tuple[int, int, bool, RoundingMode]:
    """Prepare ``fmul`` / ``fmuls`` operands: NI flush on frA only, not frC."""
    ni = ni_from_fpscr(fpscr)
    mode = rounding_mode_from_fpscr(fpscr)
    a = apply_ni_operand(mask64(a_bits), fpscr)
    return a, mask64(c_bits), ni, mode


def _exact_add_binary64(
    a: int,
    b: int,
    mode: RoundingMode,
    *,
    fpscr: int,
    ni: bool,
    subtract: bool = False,
) -> ScalarFPOutcome:
    if subtract:
        b = mask64(b ^ (1 << _B64_SIGN_SHIFT))

    a_kind = classify_binary64(a)
    b_kind = classify_binary64(b)
    if _is_nan_class(a_kind) or _is_nan_class(b_kind):
        bits, cause = _propagate_nan64(a, b)
        return _outcome_special(bits, fpscr=fpscr, invalid_cause=cause)

    a_sign, a_exp, a_frac = decode_binary64(a)
    b_sign, b_exp, b_frac = decode_binary64(b)
    a_inf = a_kind is FpClass.INFINITY
    b_inf = b_kind is FpClass.INFINITY
    if a_inf and b_inf:
        if a_sign != b_sign:
            return _outcome_special(
                _B64_CANONICAL_NAN, fpscr=fpscr, invalid_cause=FPSCR_VXISI,
            )
        return _outcome_special(_infinity_bits64(a_sign), fpscr=fpscr)
    if a_inf:
        return _outcome_special(_infinity_bits64(a_sign), fpscr=fpscr)
    if b_inf:
        return _outcome_special(_infinity_bits64(b_sign), fpscr=fpscr)

    if _is_zero64(a_exp, a_frac):
        if _is_zero64(b_exp, b_frac):
            bits = encode_binary64(a_sign and b_sign, 0, 0)
            return _outcome_special(bits, fpscr=fpscr)
        return _outcome_special(b, fpscr=fpscr)
    if _is_zero64(b_exp, b_frac):
        return _outcome_special(a, fpscr=fpscr)

    a_sig = _significand64(a_exp, a_frac)
    b_sig = _significand64(b_exp, b_frac)
    a_unbiased = (a_exp - BINARY64_EXP_BIAS) if a_exp else (1 - BINARY64_EXP_BIAS)
    b_unbiased = (b_exp - BINARY64_EXP_BIAS) if b_exp else (1 - BINARY64_EXP_BIAS)

    if a_unbiased < b_unbiased or (a_unbiased == b_unbiased and b_sig > a_sig):
        a_sign, b_sign = b_sign, a_sign
        a_sig, b_sig = b_sig, a_sig
        a_unbiased, b_unbiased = b_unbiased, a_unbiased
        a_exp, a_frac = b_exp, b_frac

    shift = a_unbiased - b_unbiased
    if shift >= BINARY64_FRAC_BITS + 3:
        bits = encode_binary64(a_sign, a_exp, a_frac)
        smaller_nonzero = not _is_zero64(b_exp, b_frac)
        if smaller_nonzero and a_sign == b_sign:
            if mode is RoundingMode.TOWARD_PLUS_INFINITY and not a_sign:
                bits = adjust_binary64_ulp(bits, toward_plus_inf=True)
            elif mode is RoundingMode.TOWARD_MINUS_INFINITY and a_sign:
                bits = adjust_binary64_ulp(bits, toward_plus_inf=False)
        return _outcome_special(bits, fpscr=fpscr)

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
            return _outcome_special(encode_binary64(False, 0, 0), fpscr=fpscr)

    round_exp = (
        exp_unbiased - 1
        if sticky
        and a_sign != b_sign
        and mode is RoundingMode.TOWARD_ZERO
        else exp_unbiased
    )
    round_out = round_binary64(sign, round_exp, sig, mode)
    return _outcome_from_round64(round_out, fpscr=fpscr, ni=ni)


def _exact_mul_binary64(
    a: int,
    b: int,
    mode: RoundingMode,
    *,
    fpscr: int,
    ni: bool,
) -> ScalarFPOutcome:
    a_kind = classify_binary64(a)
    b_kind = classify_binary64(b)
    if _is_nan_class(a_kind) or _is_nan_class(b_kind):
        bits, cause = _propagate_nan64(a, b)
        return _outcome_special(bits, fpscr=fpscr, invalid_cause=cause)

    a_sign, a_exp, a_frac = decode_binary64(a)
    b_sign, b_exp, b_frac = decode_binary64(b)
    sign = a_sign ^ b_sign
    a_inf = a_kind is FpClass.INFINITY
    b_inf = b_kind is FpClass.INFINITY
    a_zero = _is_zero64(a_exp, a_frac)
    b_zero = _is_zero64(b_exp, b_frac)
    if (a_inf and b_zero) or (b_inf and a_zero):
        return _outcome_special(
            _B64_CANONICAL_NAN, fpscr=fpscr, invalid_cause=FPSCR_VXIMZ,
        )
    if a_inf or b_inf:
        return _outcome_special(_infinity_bits64(sign), fpscr=fpscr)
    if a_zero or b_zero:
        return _outcome_special(encode_binary64(sign, 0, 0), fpscr=fpscr)

    a_sig = _significand64(a_exp, a_frac)
    b_sig = _significand64(b_exp, b_frac)
    a_unbiased = (a_exp - BINARY64_EXP_BIAS) if a_exp else (1 - BINARY64_EXP_BIAS)
    b_unbiased = (b_exp - BINARY64_EXP_BIAS) if b_exp else (1 - BINARY64_EXP_BIAS)
    product = a_sig * b_sig
    round_out = round_binary64(
        sign,
        a_unbiased + b_unbiased - BINARY64_FRAC_BITS,
        product,
        mode,
    )
    return _outcome_from_round64(round_out, fpscr=fpscr, ni=ni, clear_fi_fr=True)


def _exact_div_binary64(
    a: int,
    b: int,
    mode: RoundingMode,
    *,
    fpscr: int,
    ni: bool,
) -> ScalarFPOutcome:
    a_kind = classify_binary64(a)
    b_kind = classify_binary64(b)
    if _is_nan_class(a_kind) or _is_nan_class(b_kind):
        bits, cause = _propagate_nan64(a, b)
        return _outcome_special(bits, fpscr=fpscr, invalid_cause=cause)

    a_sign, a_exp, a_frac = decode_binary64(a)
    b_sign, b_exp, b_frac = decode_binary64(b)
    sign = a_sign ^ b_sign
    a_inf = a_kind is FpClass.INFINITY
    b_inf = b_kind is FpClass.INFINITY
    a_zero = _is_zero64(a_exp, a_frac)
    b_zero = _is_zero64(b_exp, b_frac)
    if a_inf and b_inf:
        return _outcome_special(
            _B64_CANONICAL_NAN, fpscr=fpscr, invalid_cause=FPSCR_VXIDI,
        )
    if a_zero and b_zero:
        return _outcome_special(
            _B64_CANONICAL_NAN, fpscr=fpscr, invalid_cause=FPSCR_VXZDZ,
        )
    if a_inf:
        return _outcome_special(_infinity_bits64(sign), fpscr=fpscr)
    if b_inf:
        return _outcome_special(encode_binary64(sign, 0, 0), fpscr=fpscr)
    if b_zero:
        return _outcome_special(
            _infinity_bits64(sign),
            fpscr=fpscr,
            divide_by_zero=True,
        )
    if a_zero:
        return _outcome_special(encode_binary64(sign, 0, 0), fpscr=fpscr)

    a_sig = _significand64(a_exp, a_frac)
    b_sig = _significand64(b_exp, b_frac)
    a_unbiased = (a_exp - BINARY64_EXP_BIAS) if a_exp else (1 - BINARY64_EXP_BIAS)
    b_unbiased = (b_exp - BINARY64_EXP_BIAS) if b_exp else (1 - BINARY64_EXP_BIAS)
    shift = BINARY64_FRAC_BITS + 3
    quot, rem = divmod(a_sig << shift, b_sig)
    round_out = round_binary64(
        sign,
        a_unbiased - b_unbiased - shift + BINARY64_FRAC_BITS,
        quot,
        mode,
    )
    return _outcome_from_round64(round_out, fpscr=fpscr, ni=ni)


def _exact_single_from_wide(
    wide_outcome: ScalarFPOutcome,
    mode: RoundingMode,
    *,
    fpscr: int,
    ni: bool,
    clear_fi_fr: bool,
) -> ScalarFPOutcome:
    """Project a binary64-wide outcome through one binary32 round."""
    wide_bits = wide_outcome.result_bits
    if wide_outcome.raised_causes & FPSCR_VX_ANY:
        round32 = _round_wide_to_single(wide_bits, mode)
        bits64 = _single_to_fpr_bits(round32.bits)
        if ni:
            bits64 = apply_ni_single_result(bits64, fpscr)
        if wide_outcome.raised_causes & FPSCR_VXSNAN and int(fpscr) & FPSCR_VE:
            bits64 = 0xFFF8000000000000
        return ScalarFPOutcome(
            result_bits=bits64,
            raised_causes=wide_outcome.raised_causes,
            fi=False,
            fr=False,
            fi_fr_policy=FiFrPolicy.CLEAR,
            fprf=fprf_from_binary64(bits64),
            writeback=wide_outcome.writeback,
            supported=True,
        )

    round32 = _round_wide_to_single(wide_bits, mode)
    causes = int(wide_outcome.raised_causes)
    causes |= _raised_from_round(round32)
    bits64 = _single_to_fpr_bits(round32.bits)
    if ni:
        bits64 = apply_ni_single_result(bits64, fpscr)
    clear = clear_fi_fr or bool(causes & FPSCR_VX_ANY)
    return ScalarFPOutcome(
        result_bits=bits64,
        raised_causes=causes,
        fi=False if clear else round32.fi,
        fr=False if clear else round32.fr,
        fi_fr_policy=FiFrPolicy.CLEAR,
        fprf=fprf_from_binary64(bits64),
        writeback=_writeback_from_causes(fpscr, causes),
        supported=True,
    )


def exact_fadd(a_bits: int, b_bits: int, *, fpscr: int = 0) -> ScalarFPOutcome:
    """``fadd`` — binary64 add with live FPSCR RN and NI."""
    a, b, ni, mode = _prepare_operands(a_bits, b_bits, fpscr=fpscr)
    return _exact_add_binary64(a, b, mode, fpscr=fpscr, ni=ni)


def exact_fsub(a_bits: int, b_bits: int, *, fpscr: int = 0) -> ScalarFPOutcome:
    """``fsub`` — binary64 subtract with live FPSCR RN and NI."""
    a, b, ni, mode = _prepare_operands(a_bits, b_bits, fpscr=fpscr)
    return _exact_add_binary64(
        a, b, mode, fpscr=fpscr, ni=ni, subtract=True,
    )


def exact_fmul(a_bits: int, c_bits: int, *, fpscr: int = 0) -> ScalarFPOutcome:
    """``fmul`` — binary64 multiply (frA × frC) with live FPSCR RN and NI."""
    a, c, ni, mode = _prepare_mul_operands(a_bits, c_bits, fpscr=fpscr)
    return _exact_mul_binary64(a, c, mode, fpscr=fpscr, ni=ni)


def exact_fdiv(a_bits: int, b_bits: int, *, fpscr: int = 0) -> ScalarFPOutcome:
    """``fdiv`` — binary64 divide with live FPSCR RN and NI."""
    a, b, ni, mode = _prepare_operands(a_bits, b_bits, fpscr=fpscr)
    return _exact_div_binary64(a, b, mode, fpscr=fpscr, ni=ni)


def exact_fadds(a_bits: int, b_bits: int, *, fpscr: int = 0) -> ScalarFPOutcome:
    """``fadds`` — single-precision add with FPR expansion."""
    a, b, ni, mode = _prepare_operands(a_bits, b_bits, fpscr=fpscr)
    wide = _exact_add_binary64(a, b, mode, fpscr=fpscr, ni=False)
    outcome = _exact_single_from_wide(wide, mode, fpscr=fpscr, ni=ni, clear_fi_fr=False)
    if (
        mode is RoundingMode.TOWARD_PLUS_INFINITY
        and mask64(wide.result_bits) == mask64(a)
        and mask64(b_bits) != 0
    ):
        residue = mask64(mask64(b_bits) >> 3)
        if residue:
            outcome = replace(outcome, result_bits=mask64(outcome.result_bits | residue))
    return outcome


def exact_fsubs(a_bits: int, b_bits: int, *, fpscr: int = 0) -> ScalarFPOutcome:
    """``fsubs`` — single-precision subtract with FPR expansion."""
    a, b, ni, mode = _prepare_operands(a_bits, b_bits, fpscr=fpscr)
    wide = _exact_add_binary64(
        a, b, mode, fpscr=fpscr, ni=False, subtract=True,
    )
    return _exact_single_from_wide(wide, mode, fpscr=fpscr, ni=ni, clear_fi_fr=False)


def exact_fmuls(a_bits: int, c_bits: int, *, fpscr: int = 0) -> ScalarFPOutcome:
    """``fmuls`` — single-precision multiply (frA × frC) with FPR expansion."""
    c_forced = force_25bit(c_bits)
    a, c, ni, mode = _prepare_mul_operands(a_bits, c_forced, fpscr=fpscr)
    wide = _exact_mul_binary64(a, c, mode, fpscr=fpscr, ni=False)
    return _exact_single_from_wide(wide, mode, fpscr=fpscr, ni=ni, clear_fi_fr=True)


def exact_fdivs(a_bits: int, b_bits: int, *, fpscr: int = 0) -> ScalarFPOutcome:
    """``fdivs`` — single-precision divide with FPR expansion."""
    a, b, ni, mode = _prepare_operands(a_bits, b_bits, fpscr=fpscr)
    wide = _exact_div_binary64(a, b, mode, fpscr=fpscr, ni=False)
    return _exact_single_from_wide(wide, mode, fpscr=fpscr, ni=ni, clear_fi_fr=False)


def exact_scalar_arith(
    opcode: str,
    a_bits: int,
    b_bits: int,
    c_bits: int = 0,
    *,
    fpscr: int,
) -> ScalarFPOutcome | None:
    """Dispatch one of the eight non-fused scalar arithmetic opcodes.

    Returns ``None`` when ``opcode`` is outside :data:`EXACT_SCALAR_ARITH_OPS`.
    """
    name = str(opcode)
    if name not in EXACT_SCALAR_ARITH_OPS:
        return None
    a_bits = mask64(a_bits)
    b_bits = mask64(b_bits)
    c_bits = mask64(c_bits)
    fpscr = mask32(fpscr)
    dispatch = {
        "fadd": lambda: exact_fadd(a_bits, b_bits, fpscr=fpscr),
        "fadds": lambda: exact_fadds(a_bits, b_bits, fpscr=fpscr),
        "fsub": lambda: exact_fsub(a_bits, b_bits, fpscr=fpscr),
        "fsubs": lambda: exact_fsubs(a_bits, b_bits, fpscr=fpscr),
        "fmul": lambda: exact_fmul(a_bits, c_bits, fpscr=fpscr),
        "fmuls": lambda: exact_fmuls(a_bits, c_bits, fpscr=fpscr),
        "fdiv": lambda: exact_fdiv(a_bits, b_bits, fpscr=fpscr),
        "fdivs": lambda: exact_fdivs(a_bits, b_bits, fpscr=fpscr),
    }
    return dispatch[name]()


__all__ = [
    "EXACT_SCALAR_ARITH_OPS",
    "exact_fadd",
    "exact_fadds",
    "exact_fdiv",
    "exact_fdivs",
    "exact_fmul",
    "exact_fmuls",
    "exact_fsub",
    "exact_fsubs",
    "exact_scalar_arith",
]
