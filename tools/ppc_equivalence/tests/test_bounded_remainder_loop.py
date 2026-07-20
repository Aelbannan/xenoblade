"""Tests for bounded symbolic remainder loops (PR11)."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.bounded_remainder_loop import (
    apply_bounded_remainder_memory_loop,
    recover_bounded_remainder_trip,
)
from tools.ppc_equivalence.ir import Instruction, Opcode
from tools.ppc_equivalence.memory_loop import (
    find_constant_stride_store_loops,
    summarize_constant_stride_store_loop,
)
from tools.ppc_equivalence.model import concrete_state
from tools.ppc_equivalence.semantics import ConcreteOps
from tools.ppc_equivalence.trip_expression import TripAnd, TripConstant, TripEntryReg


def _insn(opcode: Opcode, operands: tuple[int, ...], *, address: int = 0) -> Instruction:
    return Instruction(address, 0, opcode, operands)


def _remainder_store_loop(*, count_insn: list[Instruction]) -> list[Instruction]:
    return [
        *count_insn,
        _insn(Opcode.MTSPR, (6, 9), address=8),
        _insn(Opcode.STW, (3, 4, 0), address=12),
        _insn(Opcode.ADDI, (4, 4, 4), address=16),
        _insn(Opcode.BC, (16, 0, 12, 0), address=20),
        _insn(Opcode.BCLR, (20, 0, 0), address=24),
    ]


class BoundedRemainderTripTests(unittest.TestCase):
    def test_concrete_andi_remainder_recognized(self) -> None:
        program = _remainder_store_loop(
            count_insn=[
                _insn(Opcode.ADDI, (6, 0, 0x2B), address=0),
                _insn(Opcode.ANDI_DOT, (6, 6, 7), address=4),
            ],
        )
        trip = recover_bounded_remainder_trip(program, 2, 6, header_pc=12)
        self.assertIsNotNone(trip)
        assert trip is not None
        self.assertEqual(trip.concrete_trip, 0x2B & 7)
        self.assertEqual(trip.upper_bound, 0x2B & 7)
        self.assertEqual(trip.zero_guard.kind, "concrete-nonzero")

    def test_symbolic_remainder_without_skip_guard_unsupported(self) -> None:
        program = _remainder_store_loop(
            count_insn=[_insn(Opcode.ANDI_DOT, (6, 6, 7), address=0)],
        )
        trip = recover_bounded_remainder_trip(program, 1, 6, header_pc=12)
        self.assertIsNone(trip)

    def test_symbolic_remainder_with_skip_guard(self) -> None:
        program = [
            _insn(Opcode.ANDI_DOT, (6, 6, 7), address=0),
            _insn(Opcode.BC, (12, 0, 24, 0), address=4),
            _insn(Opcode.MTSPR, (6, 9), address=8),
            _insn(Opcode.STW, (3, 4, 0), address=12),
            _insn(Opcode.ADDI, (4, 4, 4), address=16),
            _insn(Opcode.BC, (16, 0, 12, 0), address=20),
            _insn(Opcode.BCLR, (20, 0, 0), address=24),
        ]
        trip = recover_bounded_remainder_trip(program, 2, 6, header_pc=12)
        self.assertIsNotNone(trip)
        assert trip is not None
        self.assertEqual(trip.zero_guard.kind, "skip-branch")
        self.assertIsNone(trip.concrete_trip)

    def test_zero_trip_without_guard_unsupported(self) -> None:
        program = _remainder_store_loop(
            count_insn=[_insn(Opcode.ADDI, (6, 0, 0), address=0)],
        )
        trip = recover_bounded_remainder_trip(program, 1, 6, header_pc=12)
        self.assertIsNone(trip)

    def test_bounded_expansion_matches_closed_form(self) -> None:
        from tools.ppc_equivalence.bounded_remainder_loop import BoundedRemainderTrip, ZeroTripGuard
        from tools.ppc_equivalence.memory_semantics import (
            apply_memory_loop_transition,
            build_memory_loop_transition,
        )

        trip = BoundedRemainderTrip(
            expr=TripAnd(TripEntryReg(6), TripConstant(7)),
            expr_canonical={"kind": "const", "value": 3},
            upper_bound=7,
            concrete_trip=3,
            zero_guard=ZeroTripGuard("concrete-nonzero"),
            notes=(),
        )
        initial = concrete_state({"gpr": {"r4": 0x1000, "r3": 0xDEAD}})
        ops = ConcreteOps()
        closed = apply_memory_loop_transition(
            initial,
            build_memory_loop_transition(
                initial,
                trip_count=3,
                base_reg=4,
                source_reg=3,
                stride=4,
                store_width=4,
                store_kind="d-form-addi",
                final_ctr=0,
                ops=ops,
            ),
            ops,
        )
        expanded = apply_bounded_remainder_memory_loop(
            initial,
            trip=trip,
            base_reg=4,
            source_reg=3,
            stride=4,
            store_width=4,
            store_kind="d-form-addi",
            final_ctr=0,
            ops=ops,
        )
        self.assertEqual(expanded.gpr[4], closed.gpr[4])
        self.assertEqual(expanded.memory, closed.memory)
        self.assertEqual(len(expanded.memory_writes), len(closed.memory_writes))

    def test_memory_loop_recognizer_tags_bounded_remainder(self) -> None:
        program = _remainder_store_loop(
            count_insn=[
                _insn(Opcode.ADDI, (6, 0, 0x2B), address=0),
                _insn(Opcode.ANDI_DOT, (6, 6, 7), address=4),
            ],
        )
        loops = find_constant_stride_store_loops(program)
        self.assertEqual(len(loops), 1)
        loop = loops[0]
        self.assertEqual(loop.confidence, "exact-pattern")
        self.assertIsNotNone(loop.trip_expr)
        summary = summarize_constant_stride_store_loop(loop)
        self.assertIsNotNone(summary)
        assert summary is not None
        self.assertEqual(summary.trip_count, 0x2B & 7)
        self.assertEqual(summary.trip_upper_bound, 0x2B & 7)

    def test_summarize_bounded_remainder_with_skip_guard(self) -> None:
        program = [
            _insn(Opcode.ANDI_DOT, (6, 6, 7), address=0),
            _insn(Opcode.BC, (12, 0, 24, 0), address=4),
            _insn(Opcode.MTSPR, (6, 9), address=8),
            _insn(Opcode.STW, (3, 4, 0), address=12),
            _insn(Opcode.ADDI, (4, 4, 4), address=16),
            _insn(Opcode.BC, (16, 0, 12, 0), address=20),
            _insn(Opcode.BCLR, (20, 0, 0), address=24),
        ]
        loops = find_constant_stride_store_loops(program)
        self.assertEqual(len(loops), 1)
        loop = loops[0]
        self.assertEqual(loop.confidence, "bounded-remainder")
        self.assertEqual(loop.zero_guard, "skip-branch")
        summary = summarize_constant_stride_store_loop(loop)
        self.assertIsNotNone(summary)
        assert summary is not None
        self.assertEqual(summary.expansion, "bounded-remainder")
        self.assertEqual(summary.trip_upper_bound, 7)
        self.assertEqual(summary.trip_count, 7)
        self.assertEqual(summary.zero_guard, "skip-branch")


if __name__ == "__main__":
    unittest.main()
