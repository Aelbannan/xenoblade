"""Phase 2 unit tests for the shared exact rounding kernel."""

from __future__ import annotations

import ast
import struct
import unittest
from pathlib import Path

from tools.ppc_equivalence.fp_backend import DEFAULT_INT_BACKEND, IntBackend
from tools.ppc_equivalence.fp_bits import (
    classify_binary32,
    classify_binary64,
    decode_binary32,
    decode_binary64,
    encode_binary32,
    encode_binary64,
    shift_right_sticky,
    significand_from_fields,
)
from tools.ppc_equivalence.fp_round import (
    FORMAT_BINARY32,
    FORMAT_BINARY64,
    FpFormat,
    RoundOutcome,
    round_binary32,
    round_binary64,
    round_to_format,
)
from tools.ppc_equivalence.fp_rounding import RoundingMode

_PKG = Path(__file__).resolve().parent.parent
_PRODUCTION_MODULES = (_PKG / "fp_bits.py", _PKG / "fp_round.py")

_ALL_MODES = (
    RoundingMode.NEAREST_EVEN,
    RoundingMode.TOWARD_ZERO,
    RoundingMode.TOWARD_PLUS_INFINITY,
    RoundingMode.TOWARD_MINUS_INFINITY,
)

_MINI_FMT = FpFormat(frac_bits=4, exp_bits=4, exp_bias=8, width=32)


def _should_increment_ref(
    sign: bool,
    mode: RoundingMode,
    *,
    guard: bool,
    round_bit: bool,
    sticky: bool,
    lsb: bool,
) -> bool:
    has_discarded = guard or round_bit or sticky
    if not has_discarded:
        return False
    if mode is RoundingMode.NEAREST_EVEN:
        return guard and (round_bit or sticky or lsb)
    if mode is RoundingMode.TOWARD_ZERO:
        return False
    if mode is RoundingMode.TOWARD_PLUS_INFINITY:
        return (not sign) and has_discarded
    if mode is RoundingMode.TOWARD_MINUS_INFINITY:
        return sign and has_discarded
    raise AssertionError(mode)


def _round_sig_ref(sign: bool, sig: int, shift: int, mode: RoundingMode) -> tuple[int, bool]:
    if shift <= 0:
        return sig << (-shift), False
    shifted, guard, round_bit, sticky, discarded = shift_right_sticky(sig, shift)
    if _should_increment_ref(
        sign,
        mode,
        guard=guard,
        round_bit=round_bit,
        sticky=sticky,
        lsb=bool(shifted & 1),
    ):
        shifted += 1
    return shifted, discarded


def _encode_ref(sign: bool, exp_unbiased: int, sig: int, fmt: FpFormat) -> int:
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
    return encode_binary64(sign, encoded_exp, encoded_frac)


def _signed_zero_ref(sign: bool, fmt: FpFormat) -> int:
    return _encode_ref(sign, -fmt.exp_bias, 0, fmt)


def _max_finite_ref(sign: bool, fmt: FpFormat) -> int:
    return _encode_ref(
        sign,
        fmt.exp_max_unbiased,
        (1 << fmt.frac_bits) | fmt.max_finite_fraction,
        fmt,
    )


def _infinity_ref(sign: bool, fmt: FpFormat) -> int:
    exp_encoded = (1 << fmt.exp_bits) - 1
    if fmt.width == 32:
        return encode_binary32(sign, exp_encoded, 0)
    return encode_binary64(sign, exp_encoded, 0)


def reference_round(
    sign: bool,
    exp_unbiased: int,
    significand: int,
    fmt: FpFormat,
    mode: RoundingMode,
) -> int:
    """Independent integer-only reference implementation (tests only)."""
    if significand == 0:
        return _signed_zero_ref(sign, fmt)

    sig = significand
    exp = exp_unbiased
    inexact = False

    top_bit = sig.bit_length() - 1
    if top_bit > fmt.frac_bits:
        shift = top_bit - fmt.frac_bits
        sig, discarded = _round_sig_ref(sign, sig, shift, mode)
        inexact = inexact or discarded
        exp += shift
    elif top_bit < fmt.frac_bits:
        sig <<= fmt.frac_bits - top_bit
        exp -= fmt.frac_bits - top_bit

    if sig >= (1 << (fmt.frac_bits + 1)):
        sig, discarded = _round_sig_ref(sign, sig, 1, mode)
        inexact = inexact or discarded
        exp += 1

    exp_max = fmt.exp_max_unbiased
    exp_min = fmt.exp_min_unbiased

    if exp > exp_max:
        pos_overflow = not sign
        if mode is RoundingMode.TOWARD_ZERO:
            return _max_finite_ref(sign, fmt)
        if mode is RoundingMode.TOWARD_PLUS_INFINITY:
            return _infinity_ref(sign, fmt) if pos_overflow else _max_finite_ref(sign, fmt)
        if mode is RoundingMode.TOWARD_MINUS_INFINITY:
            return _infinity_ref(sign, fmt) if not pos_overflow else _max_finite_ref(sign, fmt)
        return _infinity_ref(sign, fmt)

    if exp < exp_min:
        shift = exp_min - exp
        sig, discarded = _round_sig_ref(sign, sig, shift, mode)
        inexact = True
        if sig == 0:
            return _signed_zero_ref(sign, fmt)
        if sig >= (1 << fmt.frac_bits):
            return _encode_ref(sign, fmt.exp_min_unbiased, 1 << fmt.frac_bits, fmt)
        return _encode_ref(sign, exp_min, sig, fmt)

    _ = inexact
    return _encode_ref(sign, exp, sig, fmt)


class FpBitsTests(unittest.TestCase):
    def test_classify_and_pack_round_trip_binary64(self) -> None:
        bits = encode_binary64(False, 1023, 0)
        self.assertEqual(classify_binary64(bits).value, "normal")
        sign, exp, frac = decode_binary64(bits)
        self.assertEqual(encode_binary64(sign, exp, frac), bits)

    def test_shift_right_sticky_guard_round(self) -> None:
        sig = 0b1110110
        shifted, guard, round_bit, sticky, discarded = shift_right_sticky(sig, 3)
        self.assertEqual(shifted, 0b1110)
        self.assertTrue(guard)
        self.assertTrue(round_bit)
        self.assertFalse(sticky)
        self.assertTrue(discarded)

    def test_significand_from_fields(self) -> None:
        self.assertEqual(significand_from_fields(0, 5, frac_bits=4), 5)
        self.assertEqual(significand_from_fields(10, 3, frac_bits=4), (1 << 4) | 3)


class NoHostFloatProductionTests(unittest.TestCase):
    def test_production_modules_avoid_host_float(self) -> None:
        banned_names = {"float", "Float"}
        banned_attrs = {"pack", "unpack"}
        for path in _PRODUCTION_MODULES:
            tree = ast.parse(path.read_text(encoding="utf-8"))
            for node in ast.walk(tree):
                if isinstance(node, ast.Call):
                    func = node.func
                    if isinstance(func, ast.Name) and func.id in banned_names:
                        self.fail(f"{path.name} uses host float() at line {node.lineno}")
                    if isinstance(func, ast.Attribute) and func.attr in banned_attrs:
                        self.fail(
                            f"{path.name} uses struct.{func.attr} at line {node.lineno}",
                        )
                if isinstance(node, ast.ImportFrom) and node.module == "struct":
                    self.fail(f"{path.name} imports struct at line {node.lineno}")


class FpBackendTests(unittest.TestCase):
    def test_int_backend_is_default(self) -> None:
        backend = IntBackend()
        self.assertIsInstance(backend, IntBackend)
        self.assertEqual(backend.add(3, 4), 7)
        self.assertTrue(backend.land(True, False) is False)
        self.assertEqual(DEFAULT_INT_BACKEND.ite(True, 1, 2), 1)


class ExhaustiveMiniFormatTests(unittest.TestCase):
    def test_exhaustive_mini_format_all_modes(self) -> None:
        fmt = _MINI_FMT
        cases = 0
        for sign in (False, True):
            for exp in range(fmt.exp_min_unbiased - 3, fmt.exp_max_unbiased + 3):
                for sig in range(1, 1 << (fmt.frac_bits + 2)):
                    for mode in _ALL_MODES:
                        got = round_to_format(sign, exp, sig, fmt, mode)
                        ref_bits = reference_round(sign, exp, sig, fmt, mode)
                        self.assertEqual(
                            got.bits,
                            ref_bits,
                            msg=f"sign={sign} exp={exp} sig={sig} mode={mode}",
                        )
                        if got.inexact:
                            self.assertTrue(got.fi)
                            if got.magnitude_increased:
                                self.assertTrue(got.fr)
                        cases += 1
        self.assertGreater(cases, 5000)


class RoundScenarioTests(unittest.TestCase):
    def test_exact_no_rounding(self) -> None:
        outcome = round_binary64(False, 0, 1 << 52, RoundingMode.NEAREST_EVEN)
        self.assertEqual(outcome.bits, 0x3FF0000000000000)
        self.assertTrue(outcome.exact)
        self.assertFalse(outcome.fi)

    def test_halfway_even_rounds_to_even(self) -> None:
        sig = (1 << 53) + 1
        outcome = round_binary64(False, -1, sig, RoundingMode.NEAREST_EVEN)
        self.assertEqual(outcome.bits, 0x3FF0000000000000)
        self.assertTrue(outcome.inexact)
        self.assertFalse(outcome.magnitude_increased)
        self.assertFalse(outcome.fr)

    def test_halfway_rounds_up_on_odd(self) -> None:
        sig = (1 << 53) + 3
        outcome = round_binary64(False, -1, sig, RoundingMode.NEAREST_EVEN)
        self.assertEqual(outcome.bits, 0x3FF0000000000002)
        self.assertTrue(outcome.magnitude_increased)
        self.assertTrue(outcome.fr)

    def test_sticky_bit_causes_increment_rne(self) -> None:
        sig = (1 << 53) + 5
        outcome = round_binary64(False, -1, sig, RoundingMode.NEAREST_EVEN)
        self.assertEqual(outcome.bits, 0x3FF0000000000002)
        self.assertTrue(outcome.grs.sticky or outcome.grs.discarded_nonzero)

    def test_carry_into_next_binade(self) -> None:
        sig = (1 << 53) | 1
        outcome = round_binary64(False, 0, sig, RoundingMode.NEAREST_EVEN)
        self.assertEqual(outcome.bits, 0x4000000000000000)
        self.assertEqual(outcome.exp_unbiased, 1)

    def test_subnormal_to_smallest_normal(self) -> None:
        fmt = FORMAT_BINARY32
        sig = (1 << (fmt.frac_bits + 1)) - 1
        outcome = round_to_format(
            False,
            fmt.exp_min_unbiased - 1,
            sig,
            fmt,
            RoundingMode.NEAREST_EVEN,
        )
        self.assertEqual(outcome.bits, 0x00800000)
        self.assertEqual(classify_binary32(outcome.bits).value, "normal")
        self.assertTrue(outcome.pre_round_tiny)
        self.assertFalse(outcome.post_round_tiny)

    def test_smallest_subnormal_to_zero_rtz(self) -> None:
        fmt = FORMAT_BINARY32
        outcome = round_to_format(
            False,
            fmt.exp_min_unbiased - 1,
            1,
            fmt,
            RoundingMode.TOWARD_ZERO,
        )
        self.assertEqual(outcome.bits, 0)
        self.assertTrue(outcome.inexact)
        self.assertTrue(outcome.underflow)

    def test_signed_zero_preserves_sign(self) -> None:
        pos = round_binary64(False, -100, 0, RoundingMode.NEAREST_EVEN)
        neg = round_binary64(True, -100, 0, RoundingMode.TOWARD_MINUS_INFINITY)
        self.assertEqual(pos.bits, 0)
        self.assertEqual(neg.bits, 1 << 63)

    def test_directed_overflow_binary32(self) -> None:
        huge_sig = (1 << 24) | ((1 << 23) - 1)
        exp = FORMAT_BINARY32.exp_max_unbiased + 1
        rtz = round_binary32(False, exp, huge_sig, RoundingMode.TOWARD_ZERO)
        rip = round_binary32(False, exp, huge_sig, RoundingMode.TOWARD_PLUS_INFINITY)
        rim = round_binary32(False, exp, huge_sig, RoundingMode.TOWARD_MINUS_INFINITY)
        self.assertFalse(rtz.overflow_to_infinity)
        self.assertTrue(rip.overflow_to_infinity)
        self.assertFalse(rim.overflow_to_infinity)
        self.assertEqual(classify_binary32(rtz.bits).value, "normal")
        self.assertEqual(classify_binary32(rip.bits).value, "infinity")

    def test_directed_overflow_negative_binary32(self) -> None:
        huge_sig = (1 << 24) | ((1 << 23) - 1)
        exp = FORMAT_BINARY32.exp_max_unbiased + 1
        rtz = round_binary32(True, exp, huge_sig, RoundingMode.TOWARD_ZERO)
        rip = round_binary32(True, exp, huge_sig, RoundingMode.TOWARD_PLUS_INFINITY)
        rim = round_binary32(True, exp, huge_sig, RoundingMode.TOWARD_MINUS_INFINITY)
        self.assertFalse(rtz.overflow_to_infinity)
        self.assertFalse(rip.overflow_to_infinity)
        self.assertTrue(rim.overflow_to_infinity)


class BinaryFormatBoundaryTests(unittest.TestCase):
    def test_binary64_limits_rne(self) -> None:
        cases = [
            ((1 << 53) + 1, -1, 0x3FF0000000000000),
            ((1 << 53) + 3, -1, 0x3FF0000000000002),
            ((1 << 53) | 1, 0, 0x4000000000000000),
        ]
        for sig, exp_u, expected in cases:
            outcome = round_binary64(False, exp_u, sig, RoundingMode.NEAREST_EVEN)
            self.assertEqual(outcome.bits, expected, msg=f"sig={sig} exp={exp_u}")

    def test_binary32_limits_all_modes(self) -> None:
        max_finite = 0x7F7FFFFF
        sig = (1 << 24) | ((1 << 23) - 1)
        exp = FORMAT_BINARY32.exp_max_unbiased + 1
        for mode, expect_inf in (
            (RoundingMode.NEAREST_EVEN, True),
            (RoundingMode.TOWARD_ZERO, False),
            (RoundingMode.TOWARD_PLUS_INFINITY, True),
            (RoundingMode.TOWARD_MINUS_INFINITY, False),
        ):
            outcome = round_binary32(False, exp, sig, mode)
            if expect_inf:
                self.assertEqual(classify_binary32(outcome.bits).value, "infinity")
                self.assertTrue(outcome.overflow_to_infinity)
            else:
                self.assertEqual(outcome.bits & 0x7FFFFFFF, max_finite)
                self.assertFalse(outcome.overflow_to_infinity)

    def test_binary64_smallest_subnormal_rtz(self) -> None:
        fmt = FORMAT_BINARY64
        rtz = round_binary64(False, fmt.exp_min_unbiased - 1, 1, RoundingMode.TOWARD_ZERO)
        rim = round_binary64(False, fmt.exp_min_unbiased - 1, 1, RoundingMode.TOWARD_MINUS_INFINITY)
        self.assertEqual(rtz.bits, 0)
        self.assertEqual(rim.bits, 0)
        self.assertFalse(rtz.fr)
        self.assertFalse(rim.fr)

    def test_binary64_boundary_matches_struct_rne(self) -> None:
        """Spot-check against host struct only in tests (not production)."""
        value = 1.0 + 2.0 ** -53
        packed = struct.pack(">d", value)
        bits = int.from_bytes(packed, "big")
        sign, exp, frac = decode_binary64(bits)
        sig = significand_from_fields(exp, frac, frac_bits=52)
        exp_unbiased = (exp - 1023) if exp else (1 - 1023)
        outcome = round_binary64(sign, exp_unbiased, sig, RoundingMode.NEAREST_EVEN)
        self.assertEqual(outcome.bits, bits)

    def test_round_outcome_type(self) -> None:
        outcome = round_binary64(False, 0, 1 << 52, RoundingMode.TOWARD_ZERO)
        self.assertIsInstance(outcome, RoundOutcome)


if __name__ == "__main__":
    unittest.main()
