"""Tests for TripExpr recognition and evaluation (PR10)."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.ir import Instruction, Opcode
from tools.ppc_equivalence.semantics import ConcreteOps
from tools.ppc_equivalence.trip_expression import (
    TripAnd,
    TripConstant,
    TripEntryReg,
    TripLshr,
    canonical_dict,
    evaluate_concrete,
    evaluate_symbolic,
    normalize_trip_expr,
    recognize_trip_expr,
    remainder_upper_bound,
    trip_exprs_equal,
)


def _insn(opcode: Opcode, operands: tuple[int, ...], *, address: int = 0) -> Instruction:
    return Instruction(address, 0, opcode, operands)


class TripExpressionRecognitionTests(unittest.TestCase):
    def test_li_constant(self) -> None:
        program = [
            _insn(Opcode.ADDI, (3, 0, 12), address=0),
            _insn(Opcode.MTSPR, (3, 9), address=4),
        ]
        expr, notes = recognize_trip_expr(program, 1, 3)
        self.assertEqual(notes, [])
        assert expr is not None
        self.assertEqual(canonical_dict(expr), {"kind": "const", "value": 12})

    def test_andi_remainder_entry_reg(self) -> None:
        program = [
            _insn(Opcode.ANDI_DOT, (6, 6, 7), address=0),
            _insn(Opcode.MTSPR, (6, 9), address=4),
        ]
        expr, _ = recognize_trip_expr(program, 1, 6)
        assert expr is not None
        self.assertEqual(
            canonical_dict(expr),
            {
                "kind": "and",
                "left": {"kind": "entry", "reg": 6},
                "right": {"kind": "const", "value": 7},
            },
        )
        self.assertEqual(remainder_upper_bound(expr), 7)

    def test_li_andi_folds_to_constant(self) -> None:
        program = [
            _insn(Opcode.ADDI, (6, 0, 0x2B), address=0),
            _insn(Opcode.ANDI_DOT, (6, 6, 7), address=4),
            _insn(Opcode.MTSPR, (6, 9), address=8),
        ]
        expr, _ = recognize_trip_expr(program, 2, 6)
        assert expr is not None
        normalized = normalize_trip_expr(expr)
        self.assertIsInstance(normalized, TripConstant)
        assert isinstance(normalized, TripConstant)
        self.assertEqual(normalized.value, 0x2B & 7)

    def test_rlwinm_srwi(self) -> None:
        program = [
            _insn(Opcode.ADDI, (6, 0, 0x28), address=0),
            _insn(Opcode.RLWINM, (0, 6, 29, 3, 31), address=4),
            _insn(Opcode.MTSPR, (0, 9), address=8),
        ]
        expr, _ = recognize_trip_expr(program, 2, 0)
        assert expr is not None
        normalized = normalize_trip_expr(expr)
        self.assertIsInstance(normalized, TripConstant)
        assert isinstance(normalized, TripConstant)
        self.assertEqual(normalized.value, 0x28 >> 3)

    def test_entry_reg_srwi_symbolic(self) -> None:
        program = [
            _insn(Opcode.RLWINM, (0, 6, 29, 3, 31), address=0),
            _insn(Opcode.MTSPR, (0, 9), address=4),
        ]
        expr, _ = recognize_trip_expr(program, 1, 0)
        assert expr is not None
        self.assertEqual(
            canonical_dict(expr),
            {
                "kind": "lshr",
                "left": {"kind": "entry", "reg": 6},
                "shift": 3,
            },
        )

    def test_canonical_compare(self) -> None:
        left = TripAnd(TripEntryReg(6), TripConstant(7))
        right = TripAnd(TripConstant(7), TripEntryReg(6))
        self.assertFalse(trip_exprs_equal(left, right))
        self.assertTrue(
            trip_exprs_equal(
                TripAnd(TripEntryReg(6), TripConstant(7)),
                TripAnd(TripEntryReg(6), TripConstant(7)),
            ),
        )

    def test_evaluate_symbolic_entry(self) -> None:
        expr = TripAnd(TripEntryReg(6), TripConstant(7))
        ops = ConcreteOps()
        value = evaluate_symbolic(expr, {6: 0x2B}, ops)
        self.assertEqual(value, 0x2B & 7)

    def test_evaluate_concrete_missing_entry(self) -> None:
        expr = TripLshr(TripEntryReg(4), 2)
        self.assertIsNone(evaluate_concrete(expr, {}))

    def test_unsupported_non_srwi_rlwinm(self) -> None:
        program = [
            _insn(Opcode.ADDI, (6, 0, 0x10), address=0),
            _insn(Opcode.RLWINM, (0, 6, 2, 0, 29), address=4),
            _insn(Opcode.MTSPR, (0, 9), address=8),
        ]
        expr, notes = recognize_trip_expr(program, 2, 0)
        self.assertIsNone(expr)
        self.assertTrue(notes)


if __name__ == "__main__":
    unittest.main()
