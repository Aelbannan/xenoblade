from __future__ import annotations

import unittest

from tools.ppc_equivalence.fp_oracle import (
    fadd_binary64_rne,
    fadds_fpr_rne,
    fdiv_binary64_rne,
    fdivs_fpr_rne,
    fmadd_binary64_rne,
    fmadds_fpr_rne,
    fmsub_binary64_rne,
    fmsubs_fpr_rne,
    fnmadd_binary64_rne,
    fnmadds_fpr_rne,
    fnmsub_binary64_rne,
    fnmsubs_fpr_rne,
    fmul_binary64_rne,
    fmuls_fpr_rne,
    fsub_binary64_rne,
    fsubs_fpr_rne,
)
from tools.ppc_equivalence.ir import ExecutionInconclusive, Instruction, Opcode
from tools.ppc_equivalence.model import concrete_state
from tools.ppc_equivalence.semantics import ConcreteOps, execute_instruction

_F15 = 0x3FF8000000000000
_F1 = 0x3FF0000000000000
_F2 = 0x4000000000000000
_F4 = 0x4010000000000000
_QNAN = 0x7FF8000012345678


class ConcreteOpsFpOracleMethodTests(unittest.TestCase):
    def setUp(self) -> None:
        self.ops = ConcreteOps()

    def test_fadd_bits_match_oracle(self) -> None:
        expected = fadd_binary64_rne(_F15, _F2).bits64
        self.assertEqual(self.ops.fp_fadd_rne_bits(_F15, _F2), expected)

    def test_fmul_bits_match_oracle(self) -> None:
        expected = fmul_binary64_rne(_F15, _F4).bits64
        self.assertEqual(self.ops.fp_fmul_rne_bits(_F15, _F4), expected)

    def test_fadds_fpr_bits_match_oracle(self) -> None:
        expected = fadds_fpr_rne(_F15, _F2).bits64
        self.assertEqual(self.ops.fp_fadds_fpr_bits("rne", _F15, _F2), expected)

    def test_fmuls_fpr_bits_match_oracle(self) -> None:
        expected = fmuls_fpr_rne(_F15, _F4).bits64
        self.assertEqual(self.ops.fp_fmuls_fpr_bits("rne", _F15, _F4), expected)

    def test_fsub_bits_match_oracle(self) -> None:
        expected = fsub_binary64_rne(_F15, _F2).bits64
        self.assertEqual(self.ops.fp_fsub_rne_bits(_F15, _F2), expected)

    def test_fdiv_bits_match_oracle(self) -> None:
        expected = fdiv_binary64_rne(_F15, _F2).bits64
        self.assertEqual(self.ops.fp_fdiv_rne_bits(_F15, _F2), expected)

    def test_fsubs_fpr_bits_match_oracle(self) -> None:
        expected = fsubs_fpr_rne(_F15, _F2).bits64
        self.assertEqual(self.ops.fp_fsubs_fpr_bits("rne", _F15, _F2), expected)

    def test_fdivs_fpr_bits_match_oracle(self) -> None:
        expected = fdivs_fpr_rne(_F15, _F2).bits64
        self.assertEqual(self.ops.fp_fdivs_fpr_bits("rne", _F15, _F2), expected)


    def test_fmadd_bits_match_oracle(self) -> None:
        expected = fmadd_binary64_rne(_F15, _F4, _F2).bits64
        self.assertEqual(self.ops.fp_fmadd_rne_bits(_F15, _F4, _F2), expected)

    def test_fmsub_bits_match_oracle(self) -> None:
        expected = fmsub_binary64_rne(_F15, _F4, _F2).bits64
        self.assertEqual(self.ops.fp_fmsub_rne_bits(_F15, _F4, _F2), expected)

    def test_fnmadd_bits_match_oracle(self) -> None:
        expected = fnmadd_binary64_rne(_F15, _F4, _F2).bits64
        self.assertEqual(self.ops.fp_fnmadd_rne_bits(_F15, _F4, _F2), expected)

    def test_fnmsub_bits_match_oracle(self) -> None:
        expected = fnmsub_binary64_rne(_F15, _F4, _F2).bits64
        self.assertEqual(self.ops.fp_fnmsub_rne_bits(_F15, _F4, _F2), expected)

    def test_fmadds_fpr_bits_match_oracle(self) -> None:
        expected = fmadds_fpr_rne(_F15, _F2, _F4).bits64
        self.assertEqual(self.ops.fp_fmadds_fpr_bits("rne", _F15, _F2, _F4), expected)

    def test_fmsubs_fpr_bits_match_oracle(self) -> None:
        expected = fmsubs_fpr_rne(_F15, _F2, _F4).bits64
        self.assertEqual(self.ops.fp_fmsubs_fpr_bits("rne", _F15, _F2, _F4), expected)

    def test_fnmadds_fpr_bits_match_oracle(self) -> None:
        expected = fnmadds_fpr_rne(_F15, _F2, _F4).bits64
        self.assertEqual(self.ops.fp_fnmadds_fpr_bits("rne", _F15, _F2, _F4), expected)

    def test_fnmsubs_fpr_bits_match_oracle(self) -> None:
        expected = fnmsubs_fpr_rne(_F15, _F2, _F4).bits64
        self.assertEqual(self.ops.fp_fnmsubs_fpr_bits("rne", _F15, _F2, _F4), expected)

    def test_non_rne_rounding_fail_closed(self) -> None:
        with self.assertRaises(ExecutionInconclusive):
            self.ops.fp_fadds_fpr_bits("rtz", _F15, _F2)

    def test_nan_operand_fail_closed(self) -> None:
        with self.assertRaises(ExecutionInconclusive):
            self.ops.fp_fadd_rne_bits(_QNAN, _F2)

    def test_fnmadd_nan_operand_fail_closed(self) -> None:
        with self.assertRaises(ExecutionInconclusive):
            self.ops.fp_fnmadd_rne_bits(_QNAN, _F4, _F2)

    def test_divide_by_zero_fail_closed(self) -> None:
        with self.assertRaises(ExecutionInconclusive):
            self.ops.fp_fdiv_rne_bits(_F2, 0)


class ConcreteOpsFpOracleInstructionTests(unittest.TestCase):
    def test_fadds_instruction_uses_oracle_bits(self) -> None:
        state = concrete_state({
            "fpr": {"f1": _F15, "f2": _F2, "f3": 0},
            "fpscr": 0,
        })
        final = execute_instruction(
            state,
            Instruction(0, 0, Opcode.FADDS, (3, 1, 2)),
            ConcreteOps(),
        )
        expected = fadds_fpr_rne(_F15, _F2).bits64
        self.assertEqual(final.fpr[3], expected)

    def test_fmuls_instruction_uses_oracle_bits(self) -> None:
        state = concrete_state({
            "fpr": {"f1": _F15, "f3": _F4, "f7": 0},
            "fpscr": 0,
        })
        final = execute_instruction(
            state,
            Instruction(0, 0, Opcode.FMULS, (7, 1, 0, 3)),
            ConcreteOps(),
        )
        expected = fmuls_fpr_rne(_F15, _F4).bits64
        self.assertEqual(final.fpr[7], expected)

    def test_fadd_instruction_uses_oracle_bits(self) -> None:
        state = concrete_state({
            "fpr": {"f1": _F15, "f2": _F2, "f3": 0},
            "fpscr": 0,
        })
        final = execute_instruction(
            state,
            Instruction(0, 0, Opcode.FADD, (3, 1, 2)),
            ConcreteOps(),
        )
        expected = fadd_binary64_rne(_F15, _F2).bits64
        self.assertEqual(final.fpr[3], expected)

    def test_fsub_instruction_uses_oracle_bits(self) -> None:
        state = concrete_state({
            "fpr": {"f1": _F15, "f2": _F2, "f7": 0},
            "fpscr": 0,
        })
        final = execute_instruction(
            state,
            Instruction(0, 0, Opcode.FSUB, (7, 1, 2)),
            ConcreteOps(),
        )
        expected = fsub_binary64_rne(_F15, _F2).bits64
        self.assertEqual(final.fpr[7], expected)

    def test_fdivs_instruction_uses_oracle_bits(self) -> None:
        state = concrete_state({
            "fpr": {"f1": _F15, "f2": _F2, "f7": 0},
            "fpscr": 0,
        })
        final = execute_instruction(
            state,
            Instruction(0, 0, Opcode.FDIVS, (7, 1, 2)),
            ConcreteOps(),
        )
        expected = fdivs_fpr_rne(_F15, _F2).bits64
        self.assertEqual(final.fpr[7], expected)


    def test_fmadd_instruction_uses_oracle_bits(self) -> None:
        state = concrete_state({
            "fpr": {"f1": _F15, "f2": _F2, "f3": _F4, "f7": 0},
            "fpscr": 0,
        })
        final = execute_instruction(
            state,
            Instruction(0, 0, Opcode.FMADD, (7, 1, 2, 3)),
            ConcreteOps(),
        )
        expected = fmadd_binary64_rne(_F15, _F4, _F2).bits64
        self.assertEqual(final.fpr[7], expected)

    def test_fmadds_instruction_uses_oracle_bits(self) -> None:
        state = concrete_state({
            "fpr": {"f1": _F15, "f2": _F2, "f3": _F4, "f7": 0},
            "fpscr": 0,
        })
        final = execute_instruction(
            state,
            Instruction(0, 0, Opcode.FMADDS, (7, 1, 2, 3)),
            ConcreteOps(),
        )
        expected = fmadds_fpr_rne(_F15, _F2, _F4).bits64
        self.assertEqual(final.fpr[7], expected)

    def test_fmsub_instruction_uses_oracle_bits(self) -> None:
        state = concrete_state({
            "fpr": {"f1": _F15, "f2": _F2, "f3": _F4, "f7": 0},
            "fpscr": 0,
        })
        final = execute_instruction(
            state,
            Instruction(0, 0, Opcode.FMSUB, (7, 1, 2, 3)),
            ConcreteOps(),
        )
        expected = fmsub_binary64_rne(_F15, _F4, _F2).bits64
        self.assertEqual(final.fpr[7], expected)

    def test_fnmadd_instruction_uses_oracle_bits(self) -> None:
        state = concrete_state({
            "fpr": {"f1": _F15, "f2": _F2, "f3": _F4, "f7": 0},
            "fpscr": 0,
        })
        final = execute_instruction(
            state,
            Instruction(0, 0, Opcode.FNMADD, (7, 1, 2, 3)),
            ConcreteOps(),
        )
        expected = fnmadd_binary64_rne(_F15, _F4, _F2).bits64
        self.assertEqual(final.fpr[7], expected)

    def test_fnmsub_instruction_uses_oracle_bits(self) -> None:
        state = concrete_state({
            "fpr": {"f1": _F15, "f2": _F2, "f3": _F4, "f7": 0},
            "fpscr": 0,
        })
        final = execute_instruction(
            state,
            Instruction(0, 0, Opcode.FNMSUB, (7, 1, 2, 3)),
            ConcreteOps(),
        )
        expected = fnmsub_binary64_rne(_F15, _F4, _F2).bits64
        self.assertEqual(final.fpr[7], expected)

    def test_fnmadds_instruction_uses_oracle_bits(self) -> None:
        state = concrete_state({
            "fpr": {"f1": _F15, "f2": _F2, "f3": _F4, "f7": 0},
            "fpscr": 0,
        })
        final = execute_instruction(
            state,
            Instruction(0, 0, Opcode.FNMADDS, (7, 1, 2, 3)),
            ConcreteOps(),
        )
        expected = fnmadds_fpr_rne(_F15, _F2, _F4).bits64
        self.assertEqual(final.fpr[7], expected)

    def test_fnmsubs_instruction_uses_oracle_bits(self) -> None:
        state = concrete_state({
            "fpr": {"f1": _F15, "f2": _F2, "f3": _F4, "f7": 0},
            "fpscr": 0,
        })
        final = execute_instruction(
            state,
            Instruction(0, 0, Opcode.FNMSUBS, (7, 1, 2, 3)),
            ConcreteOps(),
        )
        expected = fnmsubs_fpr_rne(_F15, _F2, _F4).bits64
        self.assertEqual(final.fpr[7], expected)


if __name__ == "__main__":
    unittest.main()
