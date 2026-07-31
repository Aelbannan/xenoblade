"""Jump-table pattern recognition tests."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.decoder import decode_block, parse_hex
from tools.ppc_equivalence.ir import Instruction, Opcode
from tools.ppc_equivalence.jump_table import find_jump_table_candidates


CODE_BASE = 0x80103F60


def _decode(text: str) -> list[Instruction]:
    return decode_block(parse_hex(text), base=CODE_BASE, validate_with_capstone=False)


def _insn(
    opcode: Opcode,
    operands: tuple[int, ...],
    *,
    address: int = 0,
    link: bool = False,
) -> Instruction:
    return Instruction(address, 0, opcode, operands, link=link)


class JumpTablePatternTests(unittest.TestCase):
    def test_happy_path_real_encoding(self) -> None:
        # cmplwi r0,8; slwi r0,r0,2; lwzx r3,r3,r0; mtctr r3; bctr
        instructions = _decode(
            "28000008 5400103a 7c63002e 7c6903a6 4e800420",
        )
        candidates = find_jump_table_candidates(instructions)
        self.assertEqual(len(candidates), 1)
        candidate = candidates[0]
        self.assertEqual(candidate.confidence, "exact-pattern")
        self.assertEqual(candidate.index_reg, 0)
        self.assertEqual(candidate.bound_imm, 8)
        self.assertEqual(candidate.table_base_expr, "r3")
        self.assertEqual(candidate.branch_pc, CODE_BASE + 16)
        self.assertEqual(candidate.load_pc, CODE_BASE + 8)
        self.assertEqual(candidate.mtctr_pc, CODE_BASE + 12)
        self.assertEqual(candidate.instruction_indexes, (0, 1, 2, 3, 4))
        self.assertEqual(candidate.notes, ())

    def test_rejects_missing_bctr(self) -> None:
        instructions = _decode("28000008 5400103a 7c63002e 7c6903a6")
        self.assertEqual(find_jump_table_candidates(instructions), [])

    def test_rejects_unrelated_code(self) -> None:
        instructions = _decode("38600001 38600002 4e800020")
        self.assertEqual(find_jump_table_candidates(instructions), [])

    def test_partial_without_cmplwi(self) -> None:
        instructions = _decode("5400103a 7c63002e 7c6903a6 4e800420")
        candidates = find_jump_table_candidates(instructions)
        self.assertEqual(len(candidates), 1)
        self.assertEqual(candidates[0].confidence, "partial")
        self.assertIsNone(candidates[0].bound_imm)
        self.assertIn("missing cmplwi bounds check", candidates[0].notes)

    def test_shift_between_lis_and_addi_real_encoding(self) -> None:
        # cmplwi r3,8; bgt; lis r4,..@ha; slwi r0,r3,2; addi r4,r4,..@l;
        # lwzx r4,r4,r0; mtctr r4; bctr
        instructions = _decode(
            "28030008 41810014 3c800000 5460103a 38840000 "
            "7c84002e 7c8903a6 4e800420"
        )
        candidates = find_jump_table_candidates(instructions)
        self.assertEqual(len(candidates), 1)
        candidate = candidates[0]
        self.assertEqual(candidate.confidence, "exact-pattern")
        self.assertEqual(candidate.index_reg, 3)
        self.assertEqual(candidate.bound_imm, 8)
        self.assertEqual(candidate.table_base_expr, "r4")
        self.assertEqual(candidate.load_pc, CODE_BASE + 20)
        self.assertEqual(candidate.mtctr_pc, CODE_BASE + 24)
        self.assertEqual(candidate.instruction_indexes, (0, 3, 5, 6, 7))
        self.assertEqual(candidate.notes, ())

    def test_rejects_shift_with_non_base_insn_between(self) -> None:
        # slwi r0,r3,2; stw r5,0(r4); lwzx r4,r4,r0; mtctr r4; bctr
        # (a memory op between the shift and the load is not a table dispatch)
        instructions = _decode(
            "5460103a 90a40000 7c84002e 7c8903a6 4e800420"
        )
        candidates = find_jump_table_candidates(instructions)
        self.assertEqual(len(candidates), 1)
        self.assertIsNone(candidates[0].bound_imm)
        self.assertIn("missing index scale shift", candidates[0].notes)

    def test_rejects_bctrl_not_bctr(self) -> None:
        instructions = _decode("5400103a 7c63002e 7c6903a6 4e800421")
        self.assertEqual(find_jump_table_candidates(instructions), [])

    def test_rejects_lwzx_mtctr_register_mismatch(self) -> None:
        program = [
            _insn(Opcode.RLWINM, (0, 0, 2, 0, 29), address=0),
            _insn(Opcode.LWZX, (3, 3, 0), address=4),
            _insn(Opcode.MTSPR, (4, 9), address=8),
            _insn(Opcode.BCCTR, (20, 0, 0), address=12),
        ]
        self.assertEqual(find_jump_table_candidates(program), [])


if __name__ == "__main__":
    unittest.main()
