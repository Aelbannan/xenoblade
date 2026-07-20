"""Semantics integration tests for SCALAR_FP_EXACT_V2 fused / estimate paths."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.fp_capabilities import set_scalar_fp_exact_v2_module_flag
from tools.ppc_equivalence.fp_exact_fused import fmadd_binary64_rne, fmadds_fpr_rne
from tools.ppc_equivalence.fp_oracle import fmadd_binary64_rne as oracle_fmadd_binary64_rne
from tools.ppc_equivalence.ir import Instruction, Opcode
from tools.ppc_equivalence.model import concrete_state
from tools.ppc_equivalence.semantics import ConcreteOps, execute_instruction

_F15 = 0x3FF8000000000000
_F2 = 0x4000000000000000
_F4 = 0x4010000000000000
_ONE = 0x3FF0000000000000


class ExactSemanticsV2Tests(unittest.TestCase):
    def setUp(self) -> None:
        set_scalar_fp_exact_v2_module_flag(True)

    def tearDown(self) -> None:
        set_scalar_fp_exact_v2_module_flag(None)

    def test_fmadd_finite_matches_oracle_when_flag_on(self) -> None:
        state = concrete_state({
            "fpr": {"f1": _F15, "f2": _F2, "f3": _F4, "f7": 0},
            "fpscr": 0,
        })
        final = execute_instruction(
            state,
            Instruction(0, 0, Opcode.FMADD, (7, 1, 2, 3)),
            ConcreteOps(),
        )
        expected = oracle_fmadd_binary64_rne(_F15, _F4, _F2).bits64
        self.assertEqual(final.fpr[7], expected)
        self.assertEqual(final.fpr[7], fmadd_binary64_rne(_F15, _F4, _F2).bits64)

    def test_fmadds_midpoint_dolphin_fixture_when_flag_on(self) -> None:
        fa = 0x4049000000000000
        fb = 0x3B638E5400000000
        fc = 0xBF91198700000000
        state = concrete_state({
            "fpr": {"f1": fa, "f2": fb, "f3": fc, "f7": 0},
            "fpscr": 0,
        })
        final = execute_instruction(
            state,
            Instruction(0, 0, Opcode.FMADDS, (7, 1, 2, 3)),
            ConcreteOps(),
        )
        expected = fmadds_fpr_rne(fa, fb, fc).bits64
        self.assertEqual(final.fpr[7], expected)
        self.assertEqual(final.fpr[7], 0xBFEAB7E2E0000000)

    def test_fsel_bit_preserving(self) -> None:
        a = 0xBFF0000000000000  # -1.0
        b = 0x7FF0000012345678  # sNaN payload preserved
        c = 0x400921FB54442D18  # unused when a < 0
        state = concrete_state({
            "fpr": {"f1": a, "f2": b, "f3": c, "f7": 0},
            "fpscr": 0,
        })
        final = execute_instruction(
            state,
            Instruction(0, 0, Opcode.FSEL, (7, 1, 2, 3)),
            ConcreteOps(),
        )
        self.assertEqual(final.fpr[7], b)

    def test_fres_finite_smoke(self) -> None:
        state = concrete_state({
            "fpr": {"f2": _ONE, "f7": 0},
            "fpscr": 0,
        })
        final = execute_instruction(
            state,
            Instruction(0, 0, Opcode.FRES, (7, 0, 2, 0)),
            ConcreteOps(),
        )
        self.assertEqual(final.fpr[7], 0x3FEFFF0000000000)


class ExactSemanticsV2FlagOffTests(unittest.TestCase):
    def tearDown(self) -> None:
        set_scalar_fp_exact_v2_module_flag(None)

    def test_fmadd_still_uses_oracle_when_flag_off(self) -> None:
        set_scalar_fp_exact_v2_module_flag(False)
        state = concrete_state({
            "fpr": {"f1": _F15, "f2": _F2, "f3": _F4, "f7": 0},
            "fpscr": 0,
        })
        final = execute_instruction(
            state,
            Instruction(0, 0, Opcode.FMADD, (7, 1, 2, 3)),
            ConcreteOps(),
        )
        expected = oracle_fmadd_binary64_rne(_F15, _F4, _F2).bits64
        self.assertEqual(final.fpr[7], expected)


if __name__ == "__main__":
    unittest.main()
