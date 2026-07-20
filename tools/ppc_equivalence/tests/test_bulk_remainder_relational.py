"""Tests for bulk+remainder relational scaffolding (PR12)."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.bulk_remainder_relational import (
    build_bulk_remainder_relational_sketch,
    find_bulk_remainder_pairs,
    prove_bulk_remainder_identity,
)
from tools.ppc_equivalence.ir import Instruction, Opcode
from tools.ppc_equivalence.relational_induction import RelationalInductionSketch
from tools.ppc_equivalence.trip_expression import remainder_mask_for_shift


def _insn(opcode: Opcode, operands: tuple[int, ...], *, address: int = 0) -> Instruction:
    return Instruction(address, 0, opcode, operands)


def _store_loop(
    *,
    trip_materialization: list[Instruction],
    base_address: int,
) -> list[Instruction]:
    header = base_address + len(trip_materialization) * 4 + 4
    return [
        *[
            _insn(insn.opcode, insn.operands, address=base_address + index * 4)
            for index, insn in enumerate(trip_materialization)
        ],
        _insn(Opcode.MTSPR, (0, 9), address=base_address + len(trip_materialization) * 4),
        _insn(Opcode.STW, (3, 4, 0), address=header),
        _insn(Opcode.ADDI, (4, 4, 4), address=header + 4),
        _insn(Opcode.BC, (16, 0, header, 0), address=header + 8),
        _insn(Opcode.BCLR, (20, 0, 0), address=header + 12),
    ]


class BulkRemainderRelationalTests(unittest.TestCase):
    def test_remainder_mask_identity(self) -> None:
        self.assertEqual(remainder_mask_for_shift(3), 7)
        self.assertEqual(remainder_mask_for_shift(2), 3)

    def test_find_bulk_remainder_pair(self) -> None:
        bulk = _store_loop(
            trip_materialization=[
                _insn(Opcode.RLWINM, (0, 6, 29, 3, 31), address=0),
            ],
            base_address=0,
        )
        rem = _store_loop(
            trip_materialization=[
                _insn(Opcode.ANDI_DOT, (6, 6, 7), address=0),
            ],
            base_address=0x40,
        )
        program = bulk[:-1] + rem
        # Rebuild as one contiguous program with adjusted addresses.
        program = [
            _insn(Opcode.RLWINM, (0, 6, 29, 3, 31), address=0),
            _insn(Opcode.MTSPR, (0, 9), address=4),
            _insn(Opcode.STW, (3, 4, 0), address=8),
            _insn(Opcode.ADDI, (4, 4, 4), address=12),
            _insn(Opcode.BC, (16, 0, 8, 0), address=16),
            _insn(Opcode.ANDI_DOT, (6, 6, 7), address=20),
            _insn(Opcode.MTSPR, (6, 9), address=24),
            _insn(Opcode.STW, (3, 4, 0), address=28),
            _insn(Opcode.ADDI, (4, 4, 4), address=32),
            _insn(Opcode.BC, (16, 0, 28, 0), address=36),
            _insn(Opcode.BCLR, (20, 0, 0), address=40),
        ]
        pairs = find_bulk_remainder_pairs(program)
        # Without concrete trip counts the loops stay partial; pairing is shape-only.
        self.assertIsInstance(pairs, list)

    def test_unsupported_mismatched_shift(self) -> None:
        from tools.ppc_equivalence.memory_loop import ConstantStrideStoreLoop

        bulk = ConstantStrideStoreLoop(
            header_pc=8,
            latch_pc=16,
            exit_pc=20,
            mtctr_pc=4,
            base_reg=4,
            index_reg=None,
            stride=4,
            store_width=4,
            source_reg=3,
            store_kind="d-form-addi",
            trip_count=2,
            trip_count_reg=0,
            trip_expr={"kind": "lshr", "left": {"kind": "entry", "reg": 6}, "shift": 3},
            trip_upper_bound=None,
            zero_guard="concrete-nonzero",
            confidence="exact-pattern",
            notes=(),
        )
        rem = ConstantStrideStoreLoop(
            header_pc=28,
            latch_pc=36,
            exit_pc=40,
            mtctr_pc=24,
            base_reg=4,
            index_reg=None,
            stride=4,
            store_width=4,
            source_reg=3,
            store_kind="d-form-addi",
            trip_count=1,
            trip_count_reg=6,
            trip_expr={
                "kind": "and",
                "left": {"kind": "entry", "reg": 6},
                "right": {"kind": "const", "value": 3},
            },
            trip_upper_bound=3,
            zero_guard="concrete-nonzero",
            confidence="exact-pattern",
            notes=(),
        )
        pair = __import__(
            "tools.ppc_equivalence.bulk_remainder_relational",
            fromlist=["BulkRemainderLoopPair"],
        ).BulkRemainderLoopPair(
            entry_reg=6,
            shift_k=3,
            bulk=bulk,
            remainder=rem,
            identity_notes=("test",),
        )
        self.assertFalse(prove_bulk_remainder_identity(pair))

    def test_build_relational_sketch_pending(self) -> None:
        from tools.ppc_equivalence.memory_loop import ConstantStrideStoreLoop
        from tools.ppc_equivalence.bulk_remainder_relational import BulkRemainderLoopPair

        mask = remainder_mask_for_shift(2)
        bulk = ConstantStrideStoreLoop(
            header_pc=8,
            latch_pc=16,
            exit_pc=20,
            mtctr_pc=4,
            base_reg=4,
            index_reg=None,
            stride=4,
            store_width=4,
            source_reg=3,
            store_kind="d-form-addi",
            trip_count=2,
            trip_count_reg=0,
            trip_expr={"kind": "lshr", "left": {"kind": "entry", "reg": 6}, "shift": 2},
            trip_upper_bound=None,
            zero_guard="concrete-nonzero",
            confidence="exact-pattern",
            notes=(),
        )
        rem = ConstantStrideStoreLoop(
            header_pc=28,
            latch_pc=36,
            exit_pc=40,
            mtctr_pc=24,
            base_reg=4,
            index_reg=None,
            stride=4,
            store_width=4,
            source_reg=3,
            store_kind="d-form-addi",
            trip_count=1,
            trip_count_reg=6,
            trip_expr={
                "kind": "and",
                "left": {"kind": "entry", "reg": 6},
                "right": {"kind": "const", "value": mask},
            },
            trip_upper_bound=mask,
            zero_guard="concrete-nonzero",
            confidence="exact-pattern",
            notes=(),
        )
        pair = BulkRemainderLoopPair(
            entry_reg=6,
            shift_k=2,
            bulk=bulk,
            remainder=rem,
            identity_notes=("test",),
        )
        result = build_bulk_remainder_relational_sketch(pair, pair)
        self.assertIsInstance(result, RelationalInductionSketch)
        assert isinstance(result, RelationalInductionSketch)
        self.assertEqual(result.status, "pending")
        self.assertIn("bulk+remainder", " ".join(result.notes))


if __name__ == "__main__":
    unittest.main()
