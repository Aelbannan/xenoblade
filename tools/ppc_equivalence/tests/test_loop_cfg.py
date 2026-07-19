"""Unit tests for natural-loop CFG recognition."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.ir import Instruction, Opcode
from tools.ppc_equivalence.loop_cfg import analyze_loop_cfg


def _insn(
    opcode: Opcode,
    operands: tuple[int, ...],
    *,
    address: int = 0,
    link: bool = False,
) -> Instruction:
    return Instruction(address, 0, opcode, operands, link=link)


def _ctr_counted_loop(*, count: int, addend: int = 1) -> list[Instruction]:
    """li r0,count; mtctr r0; loop: addi r3,r3,addend; bdnz loop; blr"""
    return [
        _insn(Opcode.ADDI, (0, 0, count), address=0),
        _insn(Opcode.MTSPR, (0, 9), address=4),
        _insn(Opcode.ADDI, (3, 3, addend), address=8),
        _insn(Opcode.BC, (16, 0, 8, 0), address=12),
        _insn(Opcode.BCLR, (20, 0, 0), address=16),
    ]


def _irreducible_diamond_loop() -> list[Instruction]:
    """Classic multi-entry SCC: entry -> {A,B} -> C -> {A,B}."""
    return [
        _insn(Opcode.BC, (12, 0, 4, 0), address=0),
        _insn(Opcode.B, (16,), address=4),
        _insn(Opcode.B, (16,), address=8),
        _insn(Opcode.BC, (12, 0, 4, 0), address=16),
    ]


class LoopCfgAnalysisTests(unittest.TestCase):
    def test_empty_program(self) -> None:
        analysis = analyze_loop_cfg([])
        self.assertEqual(analysis.blocks, ())
        self.assertEqual(analysis.natural_loops, ())
        self.assertIsNone(analysis.unsupported_reason)

    def test_straight_line_has_no_loops(self) -> None:
        program = [
            _insn(Opcode.ADDI, (3, 3, 1), address=0),
            _insn(Opcode.ADDI, (3, 3, 2), address=4),
            _insn(Opcode.BCLR, (20, 0, 0), address=8),
        ]
        analysis = analyze_loop_cfg(program)
        self.assertIsNone(analysis.unsupported_reason)
        self.assertEqual(analysis.natural_loops, ())
        self.assertEqual(len(analysis.blocks), 1)
        self.assertEqual(analysis.blocks[0].start_pc, 0)
        self.assertEqual(analysis.blocks[0].end_pc, 8)

    def test_ctr_bdnz_loop(self) -> None:
        program = _ctr_counted_loop(count=3)
        analysis = analyze_loop_cfg(program)
        self.assertIsNone(analysis.unsupported_reason)
        self.assertEqual(len(analysis.natural_loops), 1)
        loop = analysis.natural_loops[0]
        self.assertEqual(loop.header_pc, 8)
        self.assertEqual(loop.latch_pc, 12)
        self.assertEqual(loop.back_edge, (12, 8))
        self.assertEqual(loop.body_pcs, frozenset({8, 12}))

    def test_infinite_self_branch(self) -> None:
        program = [_insn(Opcode.B, (0, 0), address=0)]
        analysis = analyze_loop_cfg(program)
        self.assertIsNone(analysis.unsupported_reason)
        self.assertEqual(len(analysis.natural_loops), 1)
        loop = analysis.natural_loops[0]
        self.assertEqual(loop.header_pc, 0)
        self.assertEqual(loop.latch_pc, 0)
        self.assertEqual(loop.back_edge, (0, 0))
        self.assertEqual(loop.body_pcs, frozenset({0}))

    def test_irreducible_scc_is_unsupported(self) -> None:
        program = _irreducible_diamond_loop()
        analysis = analyze_loop_cfg(program)
        self.assertEqual(analysis.natural_loops, ())
        self.assertIsNotNone(analysis.unsupported_reason)
        assert analysis.unsupported_reason is not None
        self.assertIn("irreducible", analysis.unsupported_reason.lower())

    def test_cfg_edges_for_conditional_loop(self) -> None:
        program = _ctr_counted_loop(count=2)
        analysis = analyze_loop_cfg(program)
        self.assertEqual(analysis.successors[8], frozenset({8, 16}))
        self.assertEqual(analysis.predecessors[8], frozenset({0, 8}))


if __name__ == "__main__":
    unittest.main()
