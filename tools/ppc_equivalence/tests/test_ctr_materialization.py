"""Tests for bounded GPR constant recovery used by memory-loop CTR discharge."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.ctr_materialization import recover_gpr_constant
from tools.ppc_equivalence.ir import Instruction, Opcode


def _insn(
    opcode: Opcode,
    operands: tuple[int, ...],
    *,
    address: int = 0,
) -> Instruction:
    return Instruction(address, 0, opcode, operands)


class RecoverGprConstantTests(unittest.TestCase):
    def test_li_form_addi_immediately_before_use(self) -> None:
        program = [
            _insn(Opcode.ADDI, (3, 0, 12), address=0),
            _insn(Opcode.MTSPR, (3, 9), address=4),
        ]
        value, notes = recover_gpr_constant(program, 1, 3)
        self.assertEqual(value, 12)
        self.assertEqual(notes, [])

    def test_addi_self_increment_after_zero(self) -> None:
        program = [
            _insn(Opcode.ADDI, (5, 0, 10), address=0),
            _insn(Opcode.ADDI, (5, 5, 2), address=4),
            _insn(Opcode.MTSPR, (5, 9), address=8),
        ]
        value, _ = recover_gpr_constant(program, 2, 5)
        self.assertEqual(value, 12)

    def test_addis_addi_li32(self) -> None:
        program = [
            _insn(Opcode.ADDIS, (4, 0, 0x1000), address=0),
            _insn(Opcode.ADDI, (4, 4, 0x20), address=4),
            _insn(Opcode.MTSPR, (4, 9), address=8),
        ]
        value, _ = recover_gpr_constant(program, 2, 4)
        self.assertEqual(value, 0x10000020)

    def test_addis_ori_li32(self) -> None:
        program = [
            _insn(Opcode.ADDIS, (6, 0, 0x8020), address=0),
            _insn(Opcode.ORI, (6, 6, 0x1234), address=4),
            _insn(Opcode.MTSPR, (6, 9), address=8),
        ]
        value, _ = recover_gpr_constant(program, 2, 6)
        self.assertEqual(value, 0x80201234)

    def test_or_register_copy(self) -> None:
        program = [
            _insn(Opcode.ADDI, (7, 0, 16), address=0),
            _insn(Opcode.OR, (8, 7, 7), address=4),
            _insn(Opcode.MTSPR, (8, 9), address=8),
        ]
        value, _ = recover_gpr_constant(program, 2, 8)
        self.assertEqual(value, 16)

    def test_andi_dot_remainder_mask(self) -> None:
        program = [
            _insn(Opcode.ADDI, (6, 0, 0x2B), address=0),
            _insn(Opcode.ANDI_DOT, (6, 6, 7), address=4),
            _insn(Opcode.MTSPR, (6, 9), address=8),
        ]
        value, notes = recover_gpr_constant(program, 2, 6)
        self.assertEqual(value, 0x2B & 7)
        self.assertEqual(notes, [])

    def test_rlwinm_srwi_by_3(self) -> None:
        # srwi r0, r6, 3  ==  rlwinm r0, r6, 29, 3, 31
        program = [
            _insn(Opcode.ADDI, (6, 0, 0x28), address=0),
            _insn(Opcode.RLWINM, (0, 6, 29, 3, 31), address=4),
            _insn(Opcode.MTSPR, (0, 9), address=8),
        ]
        value, notes = recover_gpr_constant(program, 2, 0)
        self.assertEqual(value, 0x28 >> 3)
        self.assertEqual(notes, [])

    def test_rlwinm_srwi_by_1_and_by_31(self) -> None:
        # srwi r5, r4, 1
        program_n1 = [
            _insn(Opcode.ADDI, (4, 0, 0xABC), address=0),
            _insn(Opcode.RLWINM, (5, 4, 31, 1, 31), address=4),
            _insn(Opcode.MTSPR, (5, 9), address=8),
        ]
        value, _ = recover_gpr_constant(program_n1, 2, 5)
        self.assertEqual(value, 0xABC >> 1)

        # lis/ori → 0x80000001; srwi r5, r4, 31
        program_n31 = [
            _insn(Opcode.ADDIS, (4, 0, 0x8000), address=0),
            _insn(Opcode.ORI, (4, 4, 1), address=4),
            _insn(Opcode.RLWINM, (5, 4, 1, 31, 31), address=8),
            _insn(Opcode.MTSPR, (5, 9), address=12),
        ]
        value, _ = recover_gpr_constant(program_n31, 3, 5)
        self.assertEqual(value, 0x80000001 >> 31)

    def test_rejects_rlwinm_non_srwi_mask(self) -> None:
        # Left-shift form: rlwinm rA, rS, 2, 0, 29  (== slwi by 2)
        program = [
            _insn(Opcode.ADDI, (6, 0, 0x10), address=0),
            _insn(Opcode.RLWINM, (0, 6, 2, 0, 29), address=4),
            _insn(Opcode.MTSPR, (0, 9), address=8),
        ]
        value, notes = recover_gpr_constant(program, 2, 0)
        self.assertIsNone(value)
        self.assertTrue(any("srwi" in note for note in notes))

    def test_rejects_rlwinm_truncated_me(self) -> None:
        # SH/MB look like >>3 but ME clears the LSB — not pure srwi.
        program = [
            _insn(Opcode.ADDI, (6, 0, 0x28), address=0),
            _insn(Opcode.RLWINM, (0, 6, 29, 3, 30), address=4),
            _insn(Opcode.MTSPR, (0, 9), address=8),
        ]
        value, notes = recover_gpr_constant(program, 2, 0)
        self.assertIsNone(value)
        self.assertTrue(notes)

    def test_rejects_rlwinm_with_unknown_source(self) -> None:
        program = [
            _insn(Opcode.RLWINM, (0, 31, 29, 3, 31), address=0),
            _insn(Opcode.MTSPR, (0, 9), address=4),
        ]
        value, notes = recover_gpr_constant(program, 1, 0)
        self.assertIsNone(value)
        self.assertTrue(notes)

    def test_andis_dot_high_mask(self) -> None:
        program = [
            _insn(Opcode.ADDIS, (4, 0, 0x8020), address=0),
            _insn(Opcode.ORI, (4, 4, 0x1234), address=4),
            _insn(Opcode.ANDIS_DOT, (5, 4, 0xFFFF), address=8),
            _insn(Opcode.MTSPR, (5, 9), address=12),
        ]
        value, _ = recover_gpr_constant(program, 3, 5)
        self.assertEqual(value, 0x80201234 & 0xFFFF0000)

    def test_rejects_andi_dot_with_unknown_source(self) -> None:
        program = [
            _insn(Opcode.ANDI_DOT, (6, 31, 7), address=0),
            _insn(Opcode.MTSPR, (6, 9), address=4),
        ]
        value, notes = recover_gpr_constant(program, 1, 6)
        self.assertIsNone(value)
        self.assertTrue(notes)

    def test_addi_from_nonzero_base(self) -> None:
        program = [
            _insn(Opcode.ADDI, (0, 0, 0), address=0),
            _insn(Opcode.ADDI, (3, 0, 9), address=4),
            _insn(Opcode.MTSPR, (3, 9), address=8),
        ]
        value, _ = recover_gpr_constant(program, 2, 3)
        self.assertEqual(value, 9)

    def test_lwz_from_readonly_image(self) -> None:
        table_addr = 0x80201000
        program = [
            _insn(Opcode.ADDIS, (5, 0, 0x8020), address=0),
            _insn(Opcode.ORI, (5, 5, 0x1000), address=4),
            _insn(Opcode.LWZ, (3, 5, 0), address=8),
            _insn(Opcode.MTSPR, (3, 9), address=12),
        ]
        value, _ = recover_gpr_constant(
            program,
            3,
            3,
            readonly_words={table_addr: 24},
        )
        self.assertEqual(value, 24)

    def test_rejects_lwz_without_readonly_image(self) -> None:
        program = [
            _insn(Opcode.ADDI, (5, 0, 0x1000), address=0),
            _insn(Opcode.LWZ, (3, 5, 0), address=4),
            _insn(Opcode.MTSPR, (3, 9), address=8),
        ]
        value, notes = recover_gpr_constant(program, 2, 3)
        self.assertIsNone(value)
        self.assertTrue(any("readonly" in note for note in notes))

    def test_rejects_unknown_base_register(self) -> None:
        program = [
            _insn(Opcode.ADDI, (3, 31, 4), address=0),
            _insn(Opcode.MTSPR, (3, 9), address=4),
        ]
        value, notes = recover_gpr_constant(program, 1, 3)
        self.assertIsNone(value)
        self.assertTrue(notes)

    def test_rejects_lookback_beyond_window(self) -> None:
        program = [
            _insn(Opcode.ADDI, (3, 0, 5), address=0),
        ]
        for index in range(13):
            program.append(_insn(Opcode.ADDI, (0, 0, 0), address=4 + index * 4))
        program.append(_insn(Opcode.MTSPR, (3, 9), address=4 + 13 * 4))
        value, notes = recover_gpr_constant(program, len(program) - 1, 3, max_lookback=12)
        self.assertIsNone(value)
        self.assertTrue(any("lookback" in note for note in notes))


if __name__ == "__main__":
    unittest.main()
