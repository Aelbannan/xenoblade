"""Cross-check tests for Phase 3 exact scalar arithmetic (``fp_exact``)."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.fp_capabilities import set_scalar_fp_exact_v2_module_flag
from tools.ppc_equivalence.fp_exact import (
    EXACT_SCALAR_ARITH_OPS,
    exact_fadd,
    exact_fadds,
    exact_fdiv,
    exact_fdivs,
    exact_fmul,
    exact_fmuls,
    exact_fsub,
    exact_fsubs,
    exact_scalar_arith,
)
from tools.ppc_equivalence.fp_exact_util import rounding_mode_from_fpscr
from tools.ppc_equivalence.fp_exact_outcome import ScalarFPOutcome
from tools.ppc_equivalence.fp_fpscr import (
    FPSCR_OX,
    FPSCR_VXIDI,
    FPSCR_VXIMZ,
    FPSCR_VXISI,
    FPSCR_VXSNAN,
    FPSCR_VXZDZ,
    FPSCR_ZX,
)
from tools.ppc_equivalence.fp_oracle import (
    fadd_binary64_rne,
    fadds_fpr_rne,
    fdiv_binary64_rne,
    fdivs_fpr_rne,
    fmul_binary64_rne,
    fmuls_fpr_rne,
    fsub_binary64_rne,
    fsubs_fpr_rne,
)
from tools.ppc_equivalence.fp_rounding import RoundingMode

# RNE FPSCR: RN=0, NI=0.
_RNE_FPSCR = 0


def _oracle_bits(fn, *args: int) -> int:
    return fn(*args).bits64


class ExactScalarArithDispatcherTests(unittest.TestCase):
    def setUp(self) -> None:
        set_scalar_fp_exact_v2_module_flag(None)

    def test_supported_opcodes(self) -> None:
        self.assertEqual(
            EXACT_SCALAR_ARITH_OPS,
            frozenset({
                "fadd", "fadds", "fsub", "fsubs",
                "fmul", "fmuls", "fdiv", "fdivs",
            }),
        )

    def test_unknown_opcode_returns_none(self) -> None:
        self.assertIsNone(
            exact_scalar_arith("fmadd", 0, 0, 0, fpscr=_RNE_FPSCR),
        )

    def test_rounding_mode_from_fpscr(self) -> None:
        self.assertEqual(rounding_mode_from_fpscr(0), RoundingMode.NEAREST_EVEN)
        self.assertEqual(rounding_mode_from_fpscr(1), RoundingMode.TOWARD_ZERO)
        self.assertEqual(rounding_mode_from_fpscr(2), RoundingMode.TOWARD_PLUS_INFINITY)
        self.assertEqual(rounding_mode_from_fpscr(3), RoundingMode.TOWARD_MINUS_INFINITY)


class ExactVsOracleRneTests(unittest.TestCase):
    """Finite and special-case RNE agreement with the SoftFloat oracle."""

    FINITE_PAIRS = (
        (0x3FF0000000000000, 0x4000000000000000),  # 1.0 + 2.0
        (0x3FF0000000000000, 0xBFF0000000000000),  # 1.0 + -1.0
        (0x0000000000000000, 0x8000000000000000),  # +0 + -0
        (0x000FFFFFFFFFFFFF, 0x3FF0000000000000),  # max subnormal + 1.0
        (0x3FEFFFFFFFFFFFFF, 0x3FF0000000000000),  # 0.999... + 1.0
        (0x405EC00000000000, 0x4049000000000000),  # large finite add
        (0x3FF0000000000000, 0x3FD0000000000000),  # mul/div operands
        (0x400921FB54442D18, 0x4000000000000000),  # pi-ish * 2
    )

    def _assert_rne_match(
        self,
        exact_fn,
        oracle_fn,
        a: int,
        b: int,
        *,
        c: int | None = None,
    ) -> None:
        if c is None:
            exact = exact_fn(a, b, fpscr=_RNE_FPSCR)
            oracle = oracle_fn(a, b)
        else:
            exact = exact_fn(a, c, fpscr=_RNE_FPSCR)
            oracle = oracle_fn(a, c)
        self.assertIsInstance(exact, ScalarFPOutcome)
        self.assertTrue(exact.supported)
        self.assertEqual(exact.result_bits, oracle.bits64)
        self.assertEqual(exact.fprf, oracle.fprf)
        self.assertEqual(
            bool(exact.raised_causes & FPSCR_ZX),
            oracle.flags.divide_by_zero,
        )

    def test_fadd_rne_finite(self) -> None:
        for a, b in self.FINITE_PAIRS:
            with self.subTest(a=hex(a), b=hex(b)):
                self._assert_rne_match(exact_fadd, fadd_binary64_rne, a, b)

    def test_fsub_rne_finite(self) -> None:
        for a, b in self.FINITE_PAIRS:
            with self.subTest(a=hex(a), b=hex(b)):
                self._assert_rne_match(exact_fsub, fsub_binary64_rne, a, b)

    def test_fmul_rne_finite(self) -> None:
        for a, b in self.FINITE_PAIRS:
            with self.subTest(a=hex(a), b=hex(b)):
                self._assert_rne_match(exact_fmul, fmul_binary64_rne, a, b)

    def test_fdiv_rne_finite(self) -> None:
        for a, b in self.FINITE_PAIRS:
            if b & 0x7FFFFFFFFFFFFFFF == 0:
                continue
            with self.subTest(a=hex(a), b=hex(b)):
                self._assert_rne_match(exact_fdiv, fdiv_binary64_rne, a, b)

    def test_fadds_rne_finite(self) -> None:
        for a, b in self.FINITE_PAIRS:
            with self.subTest(a=hex(a), b=hex(b)):
                self._assert_rne_match(exact_fadds, fadds_fpr_rne, a, b)

    def test_fsubs_rne_finite(self) -> None:
        for a, b in self.FINITE_PAIRS:
            with self.subTest(a=hex(a), b=hex(b)):
                self._assert_rne_match(exact_fsubs, fsubs_fpr_rne, a, b)

    def test_fmuls_rne_finite(self) -> None:
        for a, b in self.FINITE_PAIRS:
            with self.subTest(a=hex(a), b=hex(b)):
                self._assert_rne_match(exact_fmuls, fmuls_fpr_rne, a, b)

    def test_fdivs_rne_finite(self) -> None:
        for a, b in self.FINITE_PAIRS:
            if b & 0x7FFFFFFFFFFFFFFF == 0:
                continue
            with self.subTest(a=hex(a), b=hex(b)):
                self._assert_rne_match(exact_fdivs, fdivs_fpr_rne, a, b)

    def test_dispatcher_matches_direct_rne(self) -> None:
        a, b = 0x3FF0000000000000, 0x4000000000000000
        direct = exact_fadd(a, b, fpscr=_RNE_FPSCR)
        dispatched = exact_scalar_arith("fadd", a, b, fpscr=_RNE_FPSCR)
        assert dispatched is not None
        self.assertEqual(dispatched.result_bits, direct.result_bits)


class ExactSpecialCaseTests(unittest.TestCase):
    def test_inf_plus_neg_inf_is_invalid(self) -> None:
        pos_inf = 0x7FF0000000000000
        neg_inf = 0xFFF0000000000000
        out = exact_fadd(pos_inf, neg_inf, fpscr=_RNE_FPSCR)
        self.assertEqual(out.raised_causes & FPSCR_VXISI, FPSCR_VXISI)
        self.assertEqual(out.result_bits, 0x7FF8000000000000)

    def test_inf_times_zero_is_invalid(self) -> None:
        out = exact_fmul(0x7FF0000000000000, 0x0000000000000000, fpscr=_RNE_FPSCR)
        self.assertEqual(out.raised_causes & FPSCR_VXIMZ, FPSCR_VXIMZ)

    def test_zero_div_zero_is_invalid(self) -> None:
        out = exact_fdiv(0x0000000000000000, 0x0000000000000000, fpscr=_RNE_FPSCR)
        self.assertEqual(out.raised_causes & FPSCR_VXZDZ, FPSCR_VXZDZ)

    def test_inf_div_inf_is_invalid(self) -> None:
        out = exact_fdiv(0x7FF0000000000000, 0x7FF0000000000000, fpscr=_RNE_FPSCR)
        self.assertEqual(out.raised_causes & FPSCR_VXIDI, FPSCR_VXIDI)

    def test_finite_div_zero_raises_zx(self) -> None:
        out = exact_fdiv(0x3FF0000000000000, 0x0000000000000000, fpscr=_RNE_FPSCR)
        self.assertEqual(out.raised_causes & FPSCR_ZX, FPSCR_ZX)
        self.assertEqual(out.result_bits, 0x7FF0000000000000)

    def test_snan_quieted_and_vxsnan(self) -> None:
        snan = 0x7FF4000000000000
        one = 0x3FF0000000000000
        out = exact_fadd(snan, one, fpscr=_RNE_FPSCR)
        self.assertEqual(out.raised_causes & FPSCR_VXSNAN, FPSCR_VXSNAN)
        self.assertEqual(out.result_bits & 0x0008000000000000, 0x0008000000000000)


class ExactNonRneModeTests(unittest.TestCase):
    def test_rtz_overflow_to_max_finite(self) -> None:
        # Same-binade add that overflows: RTZ saturates to max finite; RNE goes to +Inf.
        a = 0x7FEFFFFFFFFFFFFF
        b = 0x7FE0000000000000
        out = exact_fadd(a, b, fpscr=1)  # RTZ
        self.assertEqual(out.result_bits, 0x7FEFFFFFFFFFFFFF)
        self.assertTrue(out.raised_causes & FPSCR_OX)

    def test_rtp_underflow_positive_subnormal(self) -> None:
        # Smallest positive normal - half ULP → positive subnormal under RTP.
        a = 0x0010000000000000
        b = 0x0008000000000000
        out = exact_fsub(a, b, fpscr=2)  # RTP
        self.assertGreater(out.result_bits & 0x7FFFFFFFFFFFFFFF, 0)


if __name__ == "__main__":
    unittest.main()
