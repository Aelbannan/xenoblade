"""Payload-accurate symbolic binary64 scalar FP (SCALAR_FP_V2 Phase 7).

Builds Z3 bitvector formulas for finite and special-case scalar add/mul/div and
binary64 fused multiply-add without native Z3 FloatingPoint or host float.
Rounding follows the integer kernel in :mod:`fp_round` for all four Broadway RN
modes selected from live FPSCR[0:1].
"""

from __future__ import annotations

from typing import Any

from .fp_bits import encode_binary64
from .fp_round import BINARY64_EXP_BIAS, BINARY64_FRAC_BITS

_B64_SIGN_SHIFT = 63
_B64_EXP_MAX = 0x7FF
_B64_FRAC_MASK = (1 << BINARY64_FRAC_BITS) - 1
_B64_QUIET_NAN = 1 << (BINARY64_FRAC_BITS - 1)
_B64_CANONICAL_NAN = 0x7FF8000000000000
_B64_EXP_BIAS = BINARY64_EXP_BIAS
_EXP_MIN_UNBIASED = 1 - _B64_EXP_BIAS
_EXP_MAX_UNBIASED = _B64_EXP_MAX - 1 - _B64_EXP_BIAS
_SIG_WIDTH = 128
_IDX_WIDTH = 8
_DIV_EXTRA_SHIFT = BINARY64_FRAC_BITS + 3
_FMA_FAR_SHIFT = BINARY64_FRAC_BITS * 3 + 8


def _fp_const64(ops: Any, value: int) -> Any:
    return ops.fp_const64(int(value) & 0xFFFFFFFFFFFFFFFF)


def _const32(ops: Any, value: int) -> Any:
    return ops.const(int(value) & 0xFFFFFFFF)


def _bv(ops: Any, value: int, width: int) -> Any:
    return ops.z3.BitVecVal(int(value) & ((1 << width) - 1), width)


def _decode_binary64_bv(ops: Any, bits: Any) -> tuple[Any, Any, Any]:
    z3 = ops.z3
    return z3.Extract(63, 63, bits), z3.Extract(62, 52, bits), z3.Extract(51, 0, bits)


def _encode_binary64_bv(ops: Any, sign: Any, exp: Any, frac: Any) -> Any:
    z3 = ops.z3
    return z3.Concat(sign, z3.Extract(10, 0, exp), z3.Extract(51, 0, frac))


def _exp_is_zero(ops: Any, exp: Any) -> Any:
    return ops.eq(exp, _bv(ops, 0, 11))


def _exp_is_max(ops: Any, exp: Any) -> Any:
    return ops.eq(exp, _bv(ops, _B64_EXP_MAX, 11))


def _frac_is_zero(ops: Any, frac: Any) -> Any:
    return ops.eq(frac, _bv(ops, 0, 52))


def _is_nan_bv(ops: Any, bits: Any) -> Any:
    _sign, exp, frac = _decode_binary64_bv(ops, bits)
    return ops.land(_exp_is_max(ops, exp), ops.lnot(_frac_is_zero(ops, frac)))


def _is_snan_bv(ops: Any, bits: Any) -> Any:
    z3 = ops.z3
    _sign, exp, frac = _decode_binary64_bv(ops, bits)
    quiet = z3.Extract(51, 51, frac) == _bv(ops, 1, 1)
    return ops.land(
        _exp_is_max(ops, exp),
        ops.land(ops.lnot(_frac_is_zero(ops, frac)), ops.lnot(quiet)),
    )


def _is_inf_bv(ops: Any, bits: Any) -> Any:
    _sign, exp, frac = _decode_binary64_bv(ops, bits)
    return ops.land(_exp_is_max(ops, exp), _frac_is_zero(ops, frac))


def _is_zero_bv(ops: Any, bits: Any) -> Any:
    _sign, exp, frac = _decode_binary64_bv(ops, bits)
    return ops.land(_exp_is_zero(ops, exp), _frac_is_zero(ops, frac))


def _quiet_nan_bv(ops: Any, bits: Any) -> Any:
    return ops.bor(bits, _fp_const64(ops, _B64_QUIET_NAN))


def _infinity_bits_bv(ops: Any, sign: Any) -> Any:
    z3 = ops.z3
    return z3.Concat(sign, _bv(ops, _B64_EXP_MAX, 11), _bv(ops, 0, 52))


def _signed_zero_bv(ops: Any, sign: Any) -> Any:
    z3 = ops.z3
    return z3.Concat(sign, _bv(ops, 0, 63))


def _significand64_bv(ops: Any, exp: Any, frac: Any) -> Any:
    z3 = ops.z3
    implicit = _bv(ops, 1 << BINARY64_FRAC_BITS, 64)
    frac64 = z3.ZeroExt(12, frac)
    return ops.ite(_exp_is_zero(ops, exp), frac64, ops.bor(implicit, frac64))


def _unbiased_exp64_bv(ops: Any, exp: Any) -> Any:
    z3 = ops.z3
    exp32 = z3.ZeroExt(21, exp)
    subnormal = _bv(ops, 1 - _B64_EXP_BIAS, 32)
    normal = exp32 - _bv(ops, _B64_EXP_BIAS, 32)
    return ops.ite(_exp_is_zero(ops, exp), subnormal, normal)


def _msb_index_bv(ops: Any, sig: Any, width: int) -> Any:
    """Index of highest set bit, or 0 when ``sig == 0`` (8-bit BV)."""
    z3 = ops.z3
    idx = _bv(ops, 0, _IDX_WIDTH)
    for bit_index in range(width - 1, -1, -1):
        bit = z3.Extract(bit_index, bit_index, sig) == _bv(ops, 1, 1)
        idx = z3.If(
            ops.land(bit, idx == _bv(ops, 0, _IDX_WIDTH)),
            _bv(ops, bit_index + 1, _IDX_WIDTH),
            idx,
        )
    return idx


def _rounding_modes_bv(ops: Any, fpscr: Any) -> tuple[Any, Any, Any, Any]:
    rn = ops.band(fpscr, _const32(ops, 3))
    return (
        rn == _const32(ops, 0),
        rn == _const32(ops, 1),
        rn == _const32(ops, 2),
        rn == _const32(ops, 3),
    )


def _should_increment_bv(
    ops: Any,
    sign: Any,
    *,
    guard: Any,
    round_bit: Any,
    sticky: Any,
    lsb: Any,
    is_rne: Any,
    is_rtz: Any,
    is_rp: Any,
    is_rm: Any,
) -> Any:
    z3 = ops.z3
    has_discarded = ops.lor(guard, ops.lor(round_bit, sticky))
    rne = ops.land(guard, ops.lor(round_bit, ops.lor(sticky, lsb)))
    rp = ops.land(z3.Not(sign == _bv(ops, 1, 1)), has_discarded)
    rm = ops.land(sign == _bv(ops, 1, 1), has_discarded)
    return ops.ite(
        is_rne,
        rne,
        ops.ite(is_rtz, ops.bool(False), ops.ite(is_rp, rp, rm)),
    )


def _shift_right_sticky_int(
    ops: Any,
    sig: Any,
    shift: int,
    width: int,
) -> tuple[Any, Any, Any, Any]:
    z3 = ops.z3
    if shift <= 0:
        return sig, ops.bool(False), ops.bool(False), ops.bool(False)
    if shift >= width:
        nonzero = sig != _bv(ops, 0, width)
        return _bv(ops, 0, width), nonzero, ops.bool(False), nonzero
    lost_mask = (1 << shift) - 1
    lost = sig & _bv(ops, lost_mask, width)
    shifted = z3.LShR(sig, shift)
    if shift == 1:
        guard = z3.Extract(0, 0, lost) == _bv(ops, 1, 1)
        return shifted, guard, ops.bool(False), ops.bool(False)
    guard = z3.Extract(shift - 1, shift - 1, sig) == _bv(ops, 1, 1)
    round_bit = (
        z3.Extract(shift - 2, shift - 2, sig) == _bv(ops, 1, 1)
        if shift >= 2
        else ops.bool(False)
    )
    sticky_mask = (1 << max(0, shift - 2)) - 1
    sticky = (lost & _bv(ops, sticky_mask, width)) != _bv(ops, 0, width)
    return shifted, guard, round_bit, sticky


def _round_significand_int(
    ops: Any,
    sign: Any,
    sig: Any,
    shift: int,
    width: int,
    *,
    is_rne: Any,
    is_rtz: Any,
    is_rp: Any,
    is_rm: Any,
) -> Any:
    z3 = ops.z3
    if shift <= 0:
        return sig
    shifted, guard, round_bit, sticky = _shift_right_sticky_int(ops, sig, shift, width)
    increment = _should_increment_bv(
        ops,
        sign,
        guard=guard,
        round_bit=round_bit,
        sticky=sticky,
        lsb=z3.Extract(0, 0, shifted) == _bv(ops, 1, 1),
        is_rne=is_rne,
        is_rtz=is_rtz,
        is_rp=is_rp,
        is_rm=is_rm,
    )
    return shifted + z3.If(increment, _bv(ops, 1, width), _bv(ops, 0, width))


def _round_binary64_bv(
    ops: Any,
    sign: Any,
    exp_unbiased: Any,
    sig: Any,
    fpscr: Any,
) -> Any:
    z3 = ops.z3
    is_rne, is_rtz, is_rp, is_rm = _rounding_modes_bv(ops, fpscr)
    sig128 = z3.ZeroExt(_SIG_WIDTH - 64, sig) if sig.size() == 64 else sig
    sig_is_zero = sig128 == _bv(ops, 0, _SIG_WIDTH)
    msb = _msb_index_bv(ops, sig128, _SIG_WIDTH)
    bit_len = msb
    top_bit = msb - _bv(ops, 1, _IDX_WIDTH)

    frac_bits = BINARY64_FRAC_BITS
    exp_min = _bv(ops, _EXP_MIN_UNBIASED, 32)
    exp_max = _bv(ops, _EXP_MAX_UNBIASED, 32)

    shift_down = top_bit - _bv(ops, frac_bits, _IDX_WIDTH)
    shift_up = _bv(ops, frac_bits, _IDX_WIDTH) - top_bit
    needs_down = ops.land(
        ops.lnot(sig_is_zero),
        bit_len > _bv(ops, frac_bits, _IDX_WIDTH),
    )
    needs_up = ops.land(
        ops.land(ops.lnot(sig_is_zero), ops.lnot(needs_down)),
        bit_len < _bv(ops, frac_bits, _IDX_WIDTH),
    )

    rounded_down = _round_significand_int(
        ops,
        sign,
        sig128,
        0,
        _SIG_WIDTH,
        is_rne=is_rne,
        is_rtz=is_rtz,
        is_rp=is_rp,
        is_rm=is_rm,
    )
    for shift_amt in range(1, _SIG_WIDTH):
        rounded_down = ops.ite(
            shift_down == _bv(ops, shift_amt, _IDX_WIDTH),
            _round_significand_int(
                ops,
                sign,
                sig128,
                shift_amt,
                _SIG_WIDTH,
                is_rne=is_rne,
                is_rtz=is_rtz,
                is_rp=is_rp,
                is_rm=is_rm,
            ),
            rounded_down,
        )

    norm_sig = ops.ite(
        sig_is_zero,
        _bv(ops, 0, _SIG_WIDTH),
        ops.ite(
            needs_down,
            rounded_down,
            ops.ite(
                needs_up,
                sig128 << z3.ZeroExt(_SIG_WIDTH - _IDX_WIDTH, shift_up),
                sig128,
            ),
        ),
    )
    norm_exp = ops.ite(
        sig_is_zero,
        exp_min,
        ops.ite(
            needs_down,
            exp_unbiased + z3.ZeroExt(24, shift_down),
            ops.ite(
                needs_up,
                exp_unbiased - z3.ZeroExt(24, shift_up),
                exp_unbiased,
            ),
        ),
    )

    carry = ops.land(
        ops.lnot(sig_is_zero),
        norm_sig >= _bv(ops, 1 << (frac_bits + 1), _SIG_WIDTH),
    )
    norm_sig = ops.ite(
        carry,
        _round_significand_int(
            ops,
            sign,
            norm_sig,
            1,
            _SIG_WIDTH,
            is_rne=is_rne,
            is_rtz=is_rtz,
            is_rp=is_rp,
            is_rm=is_rm,
        ),
        norm_sig,
    )
    norm_exp = ops.ite(carry, norm_exp + _bv(ops, 1, 32), norm_exp)

    overflow = norm_exp > exp_max
    underflow = norm_exp < exp_min

    encoded_frac = z3.Extract(frac_bits - 1, 0, norm_sig)
    encoded_exp = norm_exp + _bv(ops, _B64_EXP_BIAS, 32)
    normal_bits = _encode_binary64_bv(
        ops,
        sign,
        z3.Extract(10, 0, encoded_exp),
        encoded_frac,
    )

    max_finite = _fp_const64(
        ops,
        encode_binary64(False, _B64_EXP_MAX - 1, _B64_FRAC_MASK),
    )
    pos_inf = _fp_const64(ops, encode_binary64(False, _B64_EXP_MAX, 0))
    neg_inf = _fp_const64(ops, encode_binary64(True, _B64_EXP_MAX, 0))
    pos_overflow = ops.land(
        overflow,
        ops.lor(
            is_rne,
            ops.lor(is_rp, ops.land(is_rm, sign == _bv(ops, 1, 1))),
        ),
    )
    overflow_bits = ops.ite(
        sign == _bv(ops, 1, 1),
        ops.ite(pos_overflow, neg_inf, max_finite),
        ops.ite(pos_overflow, pos_inf, max_finite),
    )

    tiny_shift = exp_min - norm_exp
    tiny_sig = norm_sig
    for shift_amt in range(1, 64):
        tiny_sig = ops.ite(
            tiny_shift == _bv(ops, shift_amt, 32),
            _round_significand_int(
                ops,
                sign,
                norm_sig,
                shift_amt,
                _SIG_WIDTH,
                is_rne=is_rne,
                is_rtz=is_rtz,
                is_rp=is_rp,
                is_rm=is_rm,
            ),
            tiny_sig,
        )

    denormal_bits = _encode_binary64_bv(
        ops,
        sign,
        _bv(ops, 0, 11),
        z3.Extract(frac_bits - 1, 0, tiny_sig),
    )
    underflow_bits = ops.ite(
        tiny_sig == _bv(ops, 0, _SIG_WIDTH),
        _signed_zero_bv(ops, sign),
        denormal_bits,
    )

    rounded = ops.ite(overflow, overflow_bits, ops.ite(underflow, underflow_bits, normal_bits))
    return ops.ite(sig_is_zero, _signed_zero_bv(ops, sign), rounded)


def _propagate_nan_bv(ops: Any, *operands: Any) -> Any:
    result = _fp_const64(ops, _B64_CANONICAL_NAN)
    for bits in operands:
        is_snan = _is_snan_bv(ops, bits)
        is_qnan = ops.land(_is_nan_bv(ops, bits), ops.lnot(is_snan))
        result = ops.ite(is_snan, _quiet_nan_bv(ops, bits), result)
        result = ops.ite(
            ops.land(is_qnan, result == _fp_const64(ops, _B64_CANONICAL_NAN)),
            bits,
            result,
        )
    return result


def _finite_pair_lt_bv(
    ops: Any,
    exp_a: Any,
    sig_a: Any,
    exp_b: Any,
    sig_b: Any,
) -> Any:
    exp_lt = exp_a < exp_b
    exp_eq = exp_a == exp_b
    sig_lt = sig_a < sig_b
    return ops.lor(exp_lt, ops.land(exp_eq, sig_lt))


def _exact_finite_add_bv(
    ops: Any,
    a_bits: Any,
    b_bits: Any,
    fpscr: Any,
) -> Any:
    z3 = ops.z3
    a_sign, a_exp, a_frac = _decode_binary64_bv(ops, a_bits)
    b_sign, b_exp, b_frac = _decode_binary64_bv(ops, b_bits)

    a_sig = _significand64_bv(ops, a_exp, a_frac)
    b_sig = _significand64_bv(ops, b_exp, b_frac)
    a_unbiased = _unbiased_exp64_bv(ops, a_exp)
    b_unbiased = _unbiased_exp64_bv(ops, b_exp)

    swap = _finite_pair_lt_bv(ops, a_unbiased, a_sig, b_unbiased, b_sig)
    big_sign = ops.ite(swap, b_sign, a_sign)
    big_sig = ops.ite(swap, b_sig, a_sig)
    big_unbiased = ops.ite(swap, b_unbiased, a_unbiased)
    big_bits = ops.ite(swap, b_bits, a_bits)
    small_sign = ops.ite(swap, a_sign, b_sign)
    small_sig = ops.ite(swap, a_sig, b_sig)
    small_unbiased = ops.ite(swap, a_unbiased, b_unbiased)

    shift = z3.Extract(7, 0, big_unbiased - small_unbiased)
    far_apart = shift >= _bv(ops, BINARY64_FRAC_BITS + 3, _IDX_WIDTH)

    small_aligned = small_sig
    for shift_amt in range(64):
        shifted, _g, _r, _s = _shift_right_sticky_int(ops, small_sig, shift_amt, 64)
        small_aligned = ops.ite(shift == _bv(ops, shift_amt, _IDX_WIDTH), shifted, small_aligned)

    same_sign = big_sign == small_sign
    sum_sig = ops.ite(same_sign, big_sig + small_aligned, big_sig - small_aligned)
    diff_zero = ops.land(ops.lnot(same_sign), sum_sig == _bv(ops, 0, 64))

    rounded = _round_binary64_bv(ops, big_sign, big_unbiased, sum_sig, fpscr)
    return ops.ite(
        far_apart,
        big_bits,
        ops.ite(diff_zero, _signed_zero_bv(ops, _bv(ops, 0, 1)), rounded),
    )


def _exact_finite_mul_bv(ops: Any, a_bits: Any, b_bits: Any, fpscr: Any) -> Any:
    z3 = ops.z3
    a_sign, a_exp, a_frac = _decode_binary64_bv(ops, a_bits)
    b_sign, b_exp, b_frac = _decode_binary64_bv(ops, b_bits)
    sign = ops.bxor(a_sign, b_sign)
    a_sig = _significand64_bv(ops, a_exp, a_frac)
    b_sig = _significand64_bv(ops, b_exp, b_frac)
    a_unbiased = _unbiased_exp64_bv(ops, a_exp)
    b_unbiased = _unbiased_exp64_bv(ops, b_exp)
    product = z3.ZeroExt(64, a_sig) * z3.ZeroExt(64, b_sig)
    exp_sum = a_unbiased + b_unbiased - _bv(ops, BINARY64_FRAC_BITS, 32)
    return _round_binary64_bv(ops, sign, exp_sum, product, fpscr)


def _exact_add_special_bv(
    ops: Any,
    a_bits: Any,
    b_bits: Any,
    fpscr: Any,
    *,
    subtract: bool = False,
) -> Any:
    if subtract:
        b_bits = b_bits ^ _fp_const64(ops, 1 << _B64_SIGN_SHIFT)

    any_nan = ops.lor(_is_nan_bv(ops, a_bits), _is_nan_bv(ops, b_bits))
    nan_result = _propagate_nan_bv(ops, a_bits, b_bits)

    a_inf = _is_inf_bv(ops, a_bits)
    b_inf = _is_inf_bv(ops, b_bits)
    both_inf = ops.land(a_inf, b_inf)
    a_sign, _, _ = _decode_binary64_bv(ops, a_bits)
    b_sign, _, _ = _decode_binary64_bv(ops, b_bits)
    inf_invalid = ops.land(both_inf, ops.lnot(a_sign == b_sign))
    inf_same = ops.land(both_inf, a_sign == b_sign)
    either_inf = ops.lor(a_inf, b_inf)

    a_zero = _is_zero_bv(ops, a_bits)
    b_zero = _is_zero_bv(ops, b_bits)
    both_zero = ops.land(a_zero, b_zero)
    either_zero = ops.lor(a_zero, b_zero)

    finite = _exact_finite_add_bv(ops, a_bits, b_bits, fpscr)

    result = finite
    result = ops.ite(
        either_zero,
        ops.ite(
            both_zero,
            _signed_zero_bv(ops, ops.band(a_sign, b_sign)),
            ops.ite(a_zero, b_bits, a_bits),
        ),
        result,
    )
    result = ops.ite(
        either_inf,
        ops.ite(
            inf_invalid,
            _fp_const64(ops, _B64_CANONICAL_NAN),
            ops.ite(inf_same, a_bits, ops.ite(a_inf, a_bits, b_bits)),
        ),
        result,
    )
    return ops.ite(any_nan, nan_result, result)


def _exact_mul_special_bv(ops: Any, a_bits: Any, b_bits: Any, fpscr: Any) -> Any:
    any_nan = ops.lor(_is_nan_bv(ops, a_bits), _is_nan_bv(ops, b_bits))
    nan_result = _propagate_nan_bv(ops, a_bits, b_bits)

    a_inf = _is_inf_bv(ops, a_bits)
    b_inf = _is_inf_bv(ops, b_bits)
    a_zero = _is_zero_bv(ops, a_bits)
    b_zero = _is_zero_bv(ops, b_bits)
    invalid = ops.lor(ops.land(a_inf, b_zero), ops.land(b_inf, a_zero))
    either_inf = ops.lor(a_inf, b_inf)
    either_zero = ops.lor(a_zero, b_zero)

    a_sign, _, _ = _decode_binary64_bv(ops, a_bits)
    b_sign, _, _ = _decode_binary64_bv(ops, b_bits)
    sign = ops.bxor(a_sign, b_sign)

    finite = _exact_finite_mul_bv(ops, a_bits, b_bits, fpscr)

    result = finite
    result = ops.ite(
        either_zero,
        ops.ite(invalid, _fp_const64(ops, _B64_CANONICAL_NAN), _signed_zero_bv(ops, sign)),
        result,
    )
    result = ops.ite(
        either_inf,
        ops.ite(invalid, _fp_const64(ops, _B64_CANONICAL_NAN), _infinity_bits_bv(ops, sign)),
        result,
    )
    return ops.ite(any_nan, nan_result, result)


def _negate_finite_bits_bv(ops: Any, bits: Any) -> Any:
    non_finite = ops.lor(_is_nan_bv(ops, bits), _is_inf_bv(ops, bits))
    return ops.ite(
        non_finite,
        bits,
        ops.bxor(bits, _fp_const64(ops, 1 << _B64_SIGN_SHIFT)),
    )


def _exact_finite_div_bv(
    ops: Any,
    a_bits: Any,
    b_bits: Any,
    fpscr: Any,
) -> Any:
    z3 = ops.z3
    a_sign, a_exp, a_frac = _decode_binary64_bv(ops, a_bits)
    b_sign, b_exp, b_frac = _decode_binary64_bv(ops, b_bits)
    sign = ops.bxor(a_sign, b_sign)
    a_sig = _significand64_bv(ops, a_exp, a_frac)
    b_sig = _significand64_bv(ops, b_exp, b_frac)
    a_unbiased = _unbiased_exp64_bv(ops, a_exp)
    b_unbiased = _unbiased_exp64_bv(ops, b_exp)
    dividend = z3.ZeroExt(64, a_sig) << _DIV_EXTRA_SHIFT
    divisor = z3.ZeroExt(64, b_sig)
    quot = z3.UDiv(dividend, divisor)
    exp_unbiased = (
        a_unbiased
        - b_unbiased
        - _bv(ops, _DIV_EXTRA_SHIFT, 32)
        + _bv(ops, BINARY64_FRAC_BITS, 32)
    )
    return _round_binary64_bv(ops, sign, exp_unbiased, quot, fpscr)


def _exact_div_special_bv(
    ops: Any,
    a_bits: Any,
    b_bits: Any,
    fpscr: Any,
) -> Any:
    any_nan = ops.lor(_is_nan_bv(ops, a_bits), _is_nan_bv(ops, b_bits))
    nan_result = _propagate_nan_bv(ops, a_bits, b_bits)

    a_inf = _is_inf_bv(ops, a_bits)
    b_inf = _is_inf_bv(ops, b_bits)
    both_inf = ops.land(a_inf, b_inf)
    a_zero = _is_zero_bv(ops, a_bits)
    b_zero = _is_zero_bv(ops, b_bits)
    both_zero = ops.land(a_zero, b_zero)

    a_sign, _, _ = _decode_binary64_bv(ops, a_bits)
    b_sign, _, _ = _decode_binary64_bv(ops, b_bits)
    sign = ops.bxor(a_sign, b_sign)

    finite = _exact_finite_div_bv(ops, a_bits, b_bits, fpscr)

    result = finite
    result = ops.ite(
        b_zero,
        ops.ite(
            a_zero,
            _fp_const64(ops, _B64_CANONICAL_NAN),
            _infinity_bits_bv(ops, sign),
        ),
        result,
    )
    result = ops.ite(a_zero, _signed_zero_bv(ops, sign), result)
    result = ops.ite(
        b_inf,
        _signed_zero_bv(ops, sign),
        result,
    )
    result = ops.ite(
        a_inf,
        _infinity_bits_bv(ops, sign),
        result,
    )
    result = ops.ite(
        both_inf,
        _fp_const64(ops, _B64_CANONICAL_NAN),
        result,
    )
    result = ops.ite(
        both_zero,
        _fp_const64(ops, _B64_CANONICAL_NAN),
        result,
    )
    return ops.ite(any_nan, nan_result, result)


def _bv_uge(ops: Any, left: Any, right: Any) -> Any:
    return ops.z3.UGE(left, right)


def _bv_ugt(ops: Any, left: Any, right: Any) -> Any:
    return ops.z3.UGT(left, right)


def _shift_right_sticky_bv(
    ops: Any,
    sig: Any,
    shift: Any,
    width: int,
) -> tuple[Any, Any]:
    """Right-shift ``sig`` by symbolic ``shift`` (8-bit), returning (shifted, sticky)."""
    shifted = sig
    sticky = ops.bool(False)
    for shift_amt in range(min(width, _FMA_FAR_SHIFT + 1)):
        next_shifted, guard, round_bit, tail_sticky = _shift_right_sticky_int(
            ops,
            sig,
            shift_amt,
            width,
        )
        match = shift == _bv(ops, shift_amt, _IDX_WIDTH)
        shifted = ops.ite(match, next_shifted, shifted)
        lost = ops.lor(guard, ops.lor(round_bit, tail_sticky))
        sticky = ops.ite(match, lost, sticky)
    return shifted, sticky


def _exact_finite_fmadd_bv(
    ops: Any,
    a_bits: Any,
    c_bits: Any,
    b_bits: Any,
    fpscr: Any,
    *,
    subtract_b: bool = False,
) -> Any:
    """Finite ``a * c +/- b`` with one binary64 round (mirrors ``fp_oracle.fmadd``)."""
    if subtract_b:
        b_bits = ops.bxor(b_bits, _fp_const64(ops, 1 << _B64_SIGN_SHIFT))

    z3 = ops.z3
    a_sign, a_exp, a_frac = _decode_binary64_bv(ops, a_bits)
    c_sign, c_exp, c_frac = _decode_binary64_bv(ops, c_bits)
    b_sign, b_exp, b_frac = _decode_binary64_bv(ops, b_bits)

    product_sign = ops.bxor(a_sign, c_sign)
    a_sig = _significand64_bv(ops, a_exp, a_frac)
    c_sig = _significand64_bv(ops, c_exp, c_frac)
    a_unbiased = _unbiased_exp64_bv(ops, a_exp)
    c_unbiased = _unbiased_exp64_bv(ops, c_exp)
    product = z3.ZeroExt(64, a_sig) * z3.ZeroExt(64, c_sig)
    product_scale = a_unbiased + c_unbiased

    b_zero = _is_zero_bv(ops, b_bits)
    b_sig = _significand64_bv(ops, b_exp, b_frac)
    b_unbiased = _unbiased_exp64_bv(ops, b_exp)
    b_ext = z3.ZeroExt(64, b_sig) << BINARY64_FRAC_BITS

    product_round_exp = product_scale - _bv(ops, BINARY64_FRAC_BITS, 32)
    product_only = _round_binary64_bv(ops, product_sign, product_round_exp, product, fpscr)

    prod_ge_b = product_scale >= b_unbiased
    shift_pb = z3.Extract(7, 0, product_scale - b_unbiased)
    shift_bp = z3.Extract(7, 0, b_unbiased - product_scale)
    far_pb = _bv_uge(ops, shift_pb, _bv(ops, _FMA_FAR_SHIFT, _IDX_WIDTH))
    far_bp = _bv_uge(ops, shift_bp, _bv(ops, _FMA_FAR_SHIFT, _IDX_WIDTH))

    b_aligned, sticky_pb = _shift_right_sticky_bv(ops, b_ext, shift_pb, _SIG_WIDTH)
    product_aligned, sticky_bp = _shift_right_sticky_bv(ops, product, shift_bp, _SIG_WIDTH)

    left_sign = product_sign
    left = ops.ite(prod_ge_b, product, product_aligned)
    right_sign = b_sign
    right = ops.ite(prod_ge_b, b_aligned, b_ext)
    common = ops.ite(prod_ge_b, product_scale, b_unbiased)
    sticky = ops.ite(prod_ge_b, sticky_pb, sticky_bp)

    same_sign = left_sign == right_sign
    left_gt_right = _bv_ugt(ops, left, right)
    sum_sig = ops.ite(
        same_sign,
        left + right,
        ops.ite(left_gt_right, left - right, right - left),
    )
    result_sign = ops.ite(
        same_sign,
        left_sign,
        ops.ite(left_gt_right, left_sign, right_sign),
    )
    cancel_zero = ops.land(ops.lnot(same_sign), sum_sig == _bv(ops, 0, _SIG_WIDTH))
    rounded = _round_binary64_bv(
        ops,
        result_sign,
        common - _bv(ops, BINARY64_FRAC_BITS, 32),
        sum_sig,
        fpscr,
    )

    fused = ops.ite(cancel_zero, _signed_zero_bv(ops, _bv(ops, 0, 1)), rounded)
    fused = ops.ite(ops.land(prod_ge_b, far_pb), product_only, fused)
    fused = ops.ite(ops.land(ops.lnot(prod_ge_b), far_bp), b_bits, fused)
    fused = ops.ite(b_zero, product_only, fused)
    _ = sticky  # sticky forces inexact in concrete kernel; result bits unchanged here
    return fused


def _exact_fmadd_special_bv(
    ops: Any,
    a_bits: Any,
    c_bits: Any,
    b_bits: Any,
    fpscr: Any,
    *,
    subtract_b: bool = False,
) -> Any:
    if subtract_b:
        b_bits = ops.bxor(b_bits, _fp_const64(ops, 1 << _B64_SIGN_SHIFT))

    any_nan = ops.lor(
        _is_nan_bv(ops, a_bits),
        ops.lor(_is_nan_bv(ops, c_bits), _is_nan_bv(ops, b_bits)),
    )
    nan_result = _propagate_nan_bv(ops, a_bits, c_bits, b_bits)

    a_sign, a_exp, a_frac = _decode_binary64_bv(ops, a_bits)
    c_sign, c_exp, c_frac = _decode_binary64_bv(ops, c_bits)
    b_sign, b_exp, b_frac = _decode_binary64_bv(ops, b_bits)

    a_inf = _is_inf_bv(ops, a_bits)
    c_inf = _is_inf_bv(ops, c_bits)
    b_inf = _is_inf_bv(ops, b_bits)
    a_zero = _is_zero_bv(ops, a_bits)
    c_zero = _is_zero_bv(ops, c_bits)
    b_zero = _is_zero_bv(ops, b_bits)

    product_sign = ops.bxor(a_sign, c_sign)
    invalid_imz = ops.lor(ops.land(a_inf, c_zero), ops.land(c_inf, a_zero))
    product_inf = ops.lor(
        ops.land(a_inf, ops.lnot(c_zero)),
        ops.land(c_inf, ops.lnot(a_zero)),
    )
    inf_conflict = ops.land(
        ops.land(product_inf, b_inf),
        ops.lnot(product_sign == b_sign),
    )

    finite = _exact_finite_fmadd_bv(
        ops,
        a_bits,
        c_bits,
        b_bits,
        fpscr,
        subtract_b=False,
    )

    result = finite
    result = ops.ite(
        ops.lor(a_zero, c_zero),
        ops.ite(
            b_zero,
            _signed_zero_bv(ops, ops.band(product_sign, b_sign)),
            b_bits,
        ),
        result,
    )
    result = ops.ite(b_inf, _infinity_bits_bv(ops, b_sign), result)
    result = ops.ite(product_inf, _infinity_bits_bv(ops, product_sign), result)
    result = ops.ite(inf_conflict, _fp_const64(ops, _B64_CANONICAL_NAN), result)
    result = ops.ite(invalid_imz, _fp_const64(ops, _B64_CANONICAL_NAN), result)
    return ops.ite(any_nan, nan_result, result)


def exact_fused_result_bits_bv(
    opcode: str,
    a_bits: Any,
    c_bits: Any,
    b_bits: Any,
    fpscr: Any,
    ops: Any,
) -> Any | None:
    """Return payload-accurate binary64 fused result bits when supported."""
    name = str(opcode)
    if name == "fmadd":
        return _exact_fmadd_special_bv(ops, a_bits, c_bits, b_bits, fpscr)
    if name == "fmsub":
        return _exact_fmadd_special_bv(
            ops,
            a_bits,
            c_bits,
            b_bits,
            fpscr,
            subtract_b=True,
        )
    if name == "fnmadd":
        inner = _exact_fmadd_special_bv(ops, a_bits, c_bits, b_bits, fpscr)
        return _negate_finite_bits_bv(ops, inner)
    if name == "fnmsub":
        inner = _exact_fmadd_special_bv(
            ops,
            a_bits,
            c_bits,
            b_bits,
            fpscr,
            subtract_b=True,
        )
        return _negate_finite_bits_bv(ops, inner)
    return None


def exact_arith_result_bits_bv(
    opcode: str,
    a_bits: Any,
    b_bits: Any,
    c_bits: Any,
    fpscr: Any,
    ops: Any,
) -> Any | None:
    """Return payload-accurate result bits for supported symbolic scalar ops."""
    name = str(opcode)
    if name in ("fmul", "fmuls"):
        left, right = a_bits, c_bits
    else:
        left, right = a_bits, b_bits

    if name in ("fadd", "fadds"):
        return _exact_add_special_bv(ops, left, right, fpscr, subtract=False)
    if name in ("fsub", "fsubs"):
        return _exact_add_special_bv(ops, left, right, fpscr, subtract=True)
    if name in ("fmul", "fmuls"):
        return _exact_mul_special_bv(ops, left, right, fpscr)
    if name in ("fdiv",):
        return _exact_div_special_bv(ops, left, right, fpscr)
    return None


def verify_exact_arith_bv_concrete(
    opcode: str,
    a: int,
    b: int,
    *,
    c: int = 0,
    fpscr: int = 0,
) -> bool:
    """Evaluate a symbolic formula at concrete inputs and compare to :mod:`fp_exact`."""
    from .fp_exact import exact_scalar_arith
    from .semantics import SymbolicOps

    ops = SymbolicOps()
    z3 = ops.z3
    a_bv = z3.BitVec("a", 64)
    b_bv = z3.BitVec("b", 64)
    c_bv = z3.BitVec("c", 64)
    fpscr_bv = z3.BitVec("fpscr", 32)
    expr = exact_arith_result_bits_bv(opcode, a_bv, b_bv, c_bv, fpscr_bv, ops)
    if expr is None:
        return False
    expected = exact_scalar_arith(opcode, a, b, c_bits=c, fpscr=fpscr)
    if expected is None or not expected.supported:
        return False
    solver = z3.Solver()
    solver.set("timeout", 30000)
    out = z3.BitVec("out", 64)
    solver.add(a_bv == z3.BitVecVal(a & 0xFFFFFFFFFFFFFFFF, 64))
    solver.add(b_bv == z3.BitVecVal(b & 0xFFFFFFFFFFFFFFFF, 64))
    solver.add(c_bv == z3.BitVecVal(c & 0xFFFFFFFFFFFFFFFF, 64))
    solver.add(fpscr_bv == z3.BitVecVal(fpscr & 0xFFFFFFFF, 32))
    solver.add(out == expr)
    if solver.check() != z3.sat:
        return False
    got = int(solver.model()[out].as_long()) & 0xFFFFFFFFFFFFFFFF
    return got == (expected.result_bits & 0xFFFFFFFFFFFFFFFF)


def verify_exact_fused_bv_concrete(
    opcode: str,
    a: int,
    c: int,
    b: int,
    *,
    fpscr: int = 0,
) -> bool:
    """Evaluate a symbolic fused formula at concrete inputs and compare to exact kernel."""
    from .fp_exact_fused import dispatch_exact_fused
    from .semantics import SymbolicOps

    ops = SymbolicOps()
    z3 = ops.z3
    a_bv = z3.BitVec("a", 64)
    c_bv = z3.BitVec("c", 64)
    b_bv = z3.BitVec("b", 64)
    fpscr_bv = z3.BitVec("fpscr", 32)
    expr = exact_fused_result_bits_bv(opcode, a_bv, c_bv, b_bv, fpscr_bv, ops)
    if expr is None:
        return False
    expected = dispatch_exact_fused(opcode, a, c, b)
    if not expected.supported:
        return False
    solver = z3.Solver()
    solver.set("timeout", 30000)
    out = z3.BitVec("out", 64)
    solver.add(a_bv == z3.BitVecVal(a & 0xFFFFFFFFFFFFFFFF, 64))
    solver.add(c_bv == z3.BitVecVal(c & 0xFFFFFFFFFFFFFFFF, 64))
    solver.add(b_bv == z3.BitVecVal(b & 0xFFFFFFFFFFFFFFFF, 64))
    solver.add(fpscr_bv == z3.BitVecVal(fpscr & 0xFFFFFFFF, 32))
    solver.add(out == expr)
    if solver.check() != z3.sat:
        return False
    got = int(solver.model()[out].as_long()) & 0xFFFFFFFFFFFFFFFF
    return got == (expected.bits64 & 0xFFFFFFFFFFFFFFFF)


__all__ = [
    "exact_arith_result_bits_bv",
    "exact_fused_result_bits_bv",
    "verify_exact_arith_bv_concrete",
    "verify_exact_fused_bv_concrete",
]
