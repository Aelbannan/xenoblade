"""Phase 6 unit tests for exact compare / convert / estimate / load-store."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.fp_capabilities import set_scalar_fp_exact_v2_module_flag
from tools.ppc_equivalence.fp_exact_compare import exact_fcmpo, exact_fcmpu
from tools.ppc_equivalence.fp_exact_convert import exact_fctiw, exact_fctiwz, exact_frsp
from tools.ppc_equivalence.fp_exact_estimate import exact_fres, exact_frsqrte, exact_fsel
from tools.ppc_equivalence.fp_exact_loadstore import (
    exact_lfs_bits,
    exact_stfiwx_bits,
    expand_binary32_to_fpr,
)
from tools.ppc_equivalence.fp_fpscr import FPSCR_VXSNAN


class ExactCompareTests(unittest.TestCase):
    def setUp(self) -> None:
        set_scalar_fp_exact_v2_module_flag(True)

    def tearDown(self) -> None:
        set_scalar_fp_exact_v2_module_flag(None)

    def test_fcmpu_lt_sets_fpcc(self) -> None:
        left = 0x3FF8000000000000  # 1.5
        right = 0x4000000000000000  # 2.0
        outcome = exact_fcmpu(left, right, cr_bf=0)
        self.assertTrue(outcome.scalar.supported)
        self.assertEqual(outcome.fpcc, 0x8)  # LT
        self.assertEqual(outcome.cr_field, 0x8)

    def test_fcmpu_snan_raises_vxsnan(self) -> None:
        snan = 0x7FF0000012345678
        normal = 0x4000000000000000
        outcome = exact_fcmpu(snan, normal)
        self.assertEqual(outcome.scalar.raised_causes & FPSCR_VXSNAN, FPSCR_VXSNAN)
        self.assertEqual(outcome.fpcc, 0x1)  # UN

    def test_fcmpo_qnan_raises_vxvc(self) -> None:
        qnan = 0x7FF8000012345678
        normal = 0x4000000000000000
        outcome = exact_fcmpo(qnan, normal, fpscr=0)
        from tools.ppc_equivalence.fp_fpscr import FPSCR_VXVC

        self.assertEqual(outcome.scalar.raised_causes & FPSCR_VXVC, FPSCR_VXVC)


class ExactConvertTests(unittest.TestCase):
    def setUp(self) -> None:
        set_scalar_fp_exact_v2_module_flag(True)

    def tearDown(self) -> None:
        set_scalar_fp_exact_v2_module_flag(None)

    def test_frsp_exact_double(self) -> None:
        src = 0x3FF8000000000000
        outcome = exact_frsp(src, fpscr=0)
        self.assertEqual(outcome.result_bits, 0x3FF8000000000000)

    def test_fctiwz_negative_zero(self) -> None:
        src = 0xBFD999999999999A
        outcome = exact_fctiwz(src, fpscr=0x3)
        self.assertEqual(outcome.result_bits, 0xFFF8000100000000)

    def test_fctiw_exact_two(self) -> None:
        src = 0x4008000000000000  # 3.0? actually 40080000 = 3.0 in hex float
        outcome = exact_fctiw(0x4004000000000000, fpscr=0)  # 2.5 -> 2 RNE
        self.assertEqual(outcome.result_bits & 0xFFFFFFFF, 2)


class ExactEstimateTests(unittest.TestCase):
    def setUp(self) -> None:
        set_scalar_fp_exact_v2_module_flag(True)

    def tearDown(self) -> None:
        set_scalar_fp_exact_v2_module_flag(None)

    def test_fres_one(self) -> None:
        one = 0x3FF0000000000000
        outcome = exact_fres(one)
        self.assertEqual(outcome.result_bits, 0x3FEFFF0000000000)

    def test_fsel_positive_selects_c(self) -> None:
        a = 0x3FF8000000000000
        b = 0x4000000000000000
        c = 0x4010000000000000
        outcome = exact_fsel(a, c, b)
        self.assertEqual(outcome.result_bits, c)

    def test_frsqrte_negative_invalid(self) -> None:
        neg_one = 0xBFF0000000000000
        outcome = exact_frsqrte(neg_one)
        from tools.ppc_equivalence.fp_fpscr import FPSCR_VXSQRT

        self.assertEqual(outcome.raised_causes & FPSCR_VXSQRT, FPSCR_VXSQRT)


class ExactLoadStoreTests(unittest.TestCase):
    def setUp(self) -> None:
        set_scalar_fp_exact_v2_module_flag(True)

    def tearDown(self) -> None:
        set_scalar_fp_exact_v2_module_flag(None)

    def test_lfs_expansion(self) -> None:
        word = 0x3F800000
        fpr = expand_binary32_to_fpr(word)
        self.assertEqual(fpr, 0x3FF0000000000000)
        outcome = exact_lfs_bits(word)
        self.assertEqual(outcome.scalar.result_bits, fpr)

    def test_stfiwx_low_word(self) -> None:
        fpr = 0xFFF8000000000002
        outcome = exact_stfiwx_bits(fpr)
        self.assertEqual(outcome.memory_word, 2)


if __name__ == "__main__":
    unittest.main()
