from __future__ import annotations

import unittest

from tools.ppc_equivalence.fp_oracle import (
    ORACLE_SUPPORTED_OPS,
    OracleUnimplementedError,
    classify_binary64,
    dispatch_oracle,
    fadd_binary64_rne,
    fadds_fpr_rne,
    fmul_binary64_rne,
    fmuls_fpr_rne,
    fprf_from_binary64,
    mask64,
)
from tools.ppc_equivalence.result import (
    FP_FLAG_ASSUMED,
    FP_FLAG_MODELED,
    FP_FLAG_UNSUPPORTED,
    FP_RN_MODES_MODELED,
    FloatingPointDomain,
)


# Scalar corpus constants (fixtures/corpus.py).
_F15 = 0x3FF8000000000000
_F1 = 0x3FF0000000000000
_F2 = 0x4000000000000000
_F4 = 0x4010000000000000


class FpOracleBitHelperTests(unittest.TestCase):
    def test_classify_binary64_normals(self):
        self.assertEqual(classify_binary64(_F1).value, "normal")
        self.assertEqual(classify_binary64(0).value, "zero")

    def test_fprf_matches_semantics_table(self):
        self.assertEqual(fprf_from_binary64(_F1), 0x04)
        self.assertEqual(fprf_from_binary64(0x7FF0000000000000), 0x05)

    def test_mask64_truncates(self):
        self.assertEqual(mask64(1 << 64), 0)


class FpOracleArithmeticTests(unittest.TestCase):
    def test_fadd_binary64_corpus_inputs(self):
        result = fadd_binary64_rne(_F15, _F2)
        self.assertEqual(result.bits64, 0x400C000000000000)
        self.assertFalse(result.flags.inexact)

    def test_fmul_binary64_corpus_inputs(self):
        result = fmul_binary64_rne(_F15, _F4)
        self.assertEqual(result.bits64, 0x4018000000000000)

    def test_fadds_fpr_corpus_inputs(self):
        inputs = {"f1": _F15, "f2": _F2, "f3": _F4}
        result = fadds_fpr_rne(inputs["f1"], inputs["f2"])
        self.assertEqual(result.bits64, 0x400C000000000000)

    def test_fmuls_fpr_corpus_inputs(self):
        inputs = {"f1": _F15, "f3": _F4}
        result = fmuls_fpr_rne(inputs["f1"], inputs["f3"])
        self.assertEqual(result.bits64, 0x4018000000000000)

    def test_dispatch_supported_ops(self):
        self.assertEqual(
            dispatch_oracle("fadd", _F15, _F2).bits64,
            0x400C000000000000,
        )
        self.assertEqual(
            dispatch_oracle("fmuls", _F15, _F4).bits64,
            0x4018000000000000,
        )

    def test_dispatch_rejects_unknown_op(self):
        with self.assertRaises(OracleUnimplementedError):
            dispatch_oracle("fdiv", _F2, _F1)

    def test_nan_operand_fail_closed(self):
        with self.assertRaises(OracleUnimplementedError):
            fadd_binary64_rne(0x7FF8000012345678, _F2)

    def test_supported_ops_registry(self):
        self.assertIn("fadds", ORACLE_SUPPORTED_OPS)
        self.assertNotIn("fdiv", ORACLE_SUPPORTED_OPS)


class FloatingPointDomainFlagMetadataTests(unittest.TestCase):
    def test_to_dict_includes_fpscr_flag_modeling(self):
        payload = FloatingPointDomain().to_dict()
        self.assertIn("fpscr_flags", payload)
        flags = payload["fpscr_flags"]
        self.assertEqual(flags["modeled"], list(FP_FLAG_MODELED))
        self.assertEqual(flags["assumed"], list(FP_FLAG_ASSUMED))
        self.assertEqual(flags["unsupported"], list(FP_FLAG_UNSUPPORTED))

    def test_to_dict_includes_rounding_mode_modeling(self):
        payload = FloatingPointDomain().to_dict()
        rn = payload["rounding_mode_modeling"]
        self.assertEqual(rn["modeled"], list(FP_RN_MODES_MODELED))
        self.assertIn("toward-zero", rn["unsupported"])

    def test_fpscr_flags_roundtrip_via_from_dict(self):
        original = FloatingPointDomain()
        restored = FloatingPointDomain.from_dict(original.to_dict())
        self.assertEqual(
            restored.fpscr_flag_modeling(),
            original.fpscr_flag_modeling(),
        )

    def test_oracle_result_serializes_without_float(self):
        result = fadds_fpr_rne(_F15, _F2)
        payload = result.to_dict()
        self.assertTrue(str(payload["bits64"]).startswith("0x"))
        self.assertIn("flags", payload)


if __name__ == "__main__":
    unittest.main()
