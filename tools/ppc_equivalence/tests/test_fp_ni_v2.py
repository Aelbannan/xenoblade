"""Phase 5 SCALAR_FP_V2 NI foundations (live FPSCR.NI + exact-v2 op set)."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.fp_capabilities import (
    set_scalar_fp_exact_v2_module_flag,
)
from tools.ppc_equivalence.fp_fpscr import FPSCR_NI
from tools.ppc_equivalence.fp_ni import (
    NI_EXACT_V2_EXTRA_OPS,
    NI_EXACT_V2_OPS,
    compare_ordering_after_ni_operand_flush,
    effective_ni_supported_ops,
    fpscr_ni_enabled,
    frsp_exact_v2_stub,
    ni_documentation,
    ni_operand_flush_from_fpscr,
)
from tools.ppc_equivalence.fp_oracle import OracleUnimplementedError
from tools.ppc_equivalence.fp_outcome import NI_SUPPORTED_OPS
from tools.ppc_equivalence.ir import ExecutionInconclusive, Opcode
from tools.ppc_equivalence.model import concrete_state
from tools.ppc_equivalence.result import FloatingPointDomain
from tools.ppc_equivalence.semantics import ConcreteOps, execute_instruction

# Smallest positive binary64 subnormal.
_SUB_D = 0x0000000000000001
_NEG_SUB_D = _SUB_D | (1 << 63)
# Smallest positive binary32 subnormal expanded in FPR storage.
_SUB_S_FPR = 0x36A0000000000000


class LiveFpscrNiTests(unittest.TestCase):
    def setUp(self) -> None:
        set_scalar_fp_exact_v2_module_flag(None)

    def tearDown(self) -> None:
        set_scalar_fp_exact_v2_module_flag(None)

    def test_fpscr_ni_reads_live_bit(self) -> None:
        self.assertFalse(fpscr_ni_enabled(0))
        self.assertTrue(fpscr_ni_enabled(FPSCR_NI))
        self.assertFalse(fpscr_ni_enabled(FPSCR_NI ^ FPSCR_NI))

    def test_operand_flush_uses_live_fpscr_not_external_flag(self) -> None:
        self.assertEqual(ni_operand_flush_from_fpscr(_SUB_D, 0), _SUB_D)
        self.assertEqual(ni_operand_flush_from_fpscr(_SUB_D, FPSCR_NI), 0)
        self.assertEqual(
            ni_operand_flush_from_fpscr(_NEG_SUB_D, FPSCR_NI),
            1 << 63,
        )


class NiExactV2OpSetTests(unittest.TestCase):
    def setUp(self) -> None:
        set_scalar_fp_exact_v2_module_flag(None)

    def tearDown(self) -> None:
        set_scalar_fp_exact_v2_module_flag(None)

    def test_production_set_unchanged_when_flag_off(self) -> None:
        self.assertEqual(effective_ni_supported_ops(), NI_SUPPORTED_OPS)
        self.assertNotIn("frsp", effective_ni_supported_ops())
        self.assertNotIn("fres", effective_ni_supported_ops())

    def test_exact_v2_expands_supported_set_when_flag_on(self) -> None:
        set_scalar_fp_exact_v2_module_flag(True)
        expanded = effective_ni_supported_ops()
        self.assertTrue(NI_SUPPORTED_OPS <= expanded)
        self.assertEqual(expanded, NI_EXACT_V2_OPS)
        for name in (
            "frsp",
            "fres",
            "frsqrte",
            "fcmpu",
            "fcmpo",
            "stfs",
            "fctiw",
            "fctiwz",
        ):
            self.assertIn(name, expanded)
            self.assertIn(name, NI_EXACT_V2_EXTRA_OPS)

    def test_documentation_lists_planned_ops(self) -> None:
        doc = ni_documentation()
        self.assertIn("exact_v2_planned", doc)
        self.assertIn("stfiwx", doc["exact_v2_planned"])


class FrspExactV2StubTests(unittest.TestCase):
    def tearDown(self) -> None:
        set_scalar_fp_exact_v2_module_flag(None)

    def test_stub_requires_exact_v2_flag(self) -> None:
        set_scalar_fp_exact_v2_module_flag(False)
        with self.assertRaises(OracleUnimplementedError):
            frsp_exact_v2_stub(_SUB_S_FPR, FPSCR_NI)

    def test_frsp_subnormal_result_flushed_under_ni(self) -> None:
        set_scalar_fp_exact_v2_module_flag(True)
        ieee = frsp_exact_v2_stub(_SUB_S_FPR, 0)
        ni = frsp_exact_v2_stub(_SUB_S_FPR, FPSCR_NI)
        self.assertNotEqual(ieee, 0)
        self.assertEqual(ieee, _SUB_S_FPR)
        self.assertEqual(ni, 0)

    def test_frsp_signed_zero_preserved_under_ni(self) -> None:
        set_scalar_fp_exact_v2_module_flag(True)
        neg_zero = 1 << 63
        self.assertEqual(frsp_exact_v2_stub(neg_zero, FPSCR_NI), neg_zero)


class NiCompareOrderingTests(unittest.TestCase):
    def test_subnormal_ordering_changes_after_ni_operand_flush(self) -> None:
        # Two distinct positive subnormals order before flush; both become +0 after.
        tiny_a = 0x0000000000000001
        tiny_b = 0x0000000000000002
        _left, _right, ieee_order = compare_ordering_after_ni_operand_flush(
            tiny_a,
            tiny_b,
            0,
        )
        _, _, ni_order = compare_ordering_after_ni_operand_flush(
            tiny_a,
            tiny_b,
            FPSCR_NI,
        )
        self.assertEqual(ieee_order, "less")
        self.assertEqual(ni_order, "equal")


class NiExactV2GateTests(unittest.TestCase):
    def tearDown(self) -> None:
        set_scalar_fp_exact_v2_module_flag(None)

    def test_frsp_fail_closed_when_flag_off_and_ni_set(self) -> None:
        state = concrete_state({
            "fpr": {"f1": f"0x{_SUB_S_FPR:016x}"},
            "fpscr": FPSCR_NI,
        })
        with self.assertRaises(ExecutionInconclusive):
            execute_instruction(
                state,
                __import__(
                    "tools.ppc_equivalence.ir",
                    fromlist=["Instruction"],
                ).Instruction(0, 0, Opcode.FRSP, (1, 0, 1)),
                ConcreteOps(),
                floating_point_domain=FloatingPointDomain(require_ni_zero=False),
            )

    def test_semantics_gate_includes_frsp_when_exact_v2_on(self) -> None:
        from tools.ppc_equivalence.semantics import _fp_ni_supported_for_gate

        set_scalar_fp_exact_v2_module_flag(True)
        self.assertIn(Opcode.FRSP, _fp_ni_supported_for_gate())
        set_scalar_fp_exact_v2_module_flag(False)
        self.assertNotIn(Opcode.FRSP, _fp_ni_supported_for_gate())


if __name__ == "__main__":
    unittest.main()
