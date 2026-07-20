"""Tests for memory-loop readonly image hydration."""

from __future__ import annotations

import struct
import tempfile
import unittest
from pathlib import Path

from tools.ppc_equivalence.ir import Instruction, Opcode
from tools.ppc_equivalence.memory_loop import (
    collect_memory_loop_ctr_lwz_addresses,
    find_constant_stride_store_loops,
)
from tools.ppc_equivalence.memory_loop_image import (
    MemoryLoopImageError,
    hydrate_readonly_words,
    try_build_memory_loop_readonly_words,
)
from tools.ppc_equivalence.tests.test_allocatable_sections import build_elf_with_rodata
from tools.ppc_equivalence.tests.test_dol_symbols import _build_dol

_TABLE_ADDR = 0x80201000


def _insn(
    opcode: Opcode,
    operands: tuple[int, ...],
    *,
    address: int = 0,
) -> Instruction:
    return Instruction(address, 0, opcode, operands)


class MemoryLoopImageTests(unittest.TestCase):
    def test_collects_lwz_address_from_store_loop(self) -> None:
        table_addr = 0x80201000
        program = [
            _insn(Opcode.ADDIS, (5, 0, 0x8020), address=0),
            _insn(Opcode.ORI, (5, 5, 0x1000), address=4),
            _insn(Opcode.LWZ, (0, 5, 0), address=8),
            _insn(Opcode.MTSPR, (0, 9), address=12),
            _insn(Opcode.STW, (3, 4, 0), address=16),
            _insn(Opcode.ADDI, (4, 4, 4), address=20),
            _insn(Opcode.BC, (16, 0, 16, 0), address=24),
        ]
        self.assertEqual(
            collect_memory_loop_ctr_lwz_addresses(program),
            frozenset({table_addr}),
        )

    def test_hydrates_word_from_dol_data(self) -> None:
        text = b"\x00" * 8
        blob = _build_dol(
            text_sections=[(0x80004000, 0x100, text)],
            data_sections=[(_TABLE_ADDR, 0x100 + len(text), struct.pack(">I", 7) + b"\x00" * 252)],
        )
        with tempfile.TemporaryDirectory() as tmp:
            dol_path = Path(tmp) / "main.dol"
            dol_path.write_bytes(blob)
            words = hydrate_readonly_words({_TABLE_ADDR}, dol_path=dol_path)
        self.assertEqual(words[_TABLE_ADDR], 7)

    def test_try_build_enables_exact_pattern(self) -> None:
        table_addr = 0x80201000
        program = [
            _insn(Opcode.ADDIS, (5, 0, 0x8020), address=0),
            _insn(Opcode.ORI, (5, 5, 0x1000), address=4),
            _insn(Opcode.LWZ, (0, 5, 0), address=8),
            _insn(Opcode.MTSPR, (0, 9), address=12),
            _insn(Opcode.STW, (3, 4, 0), address=16),
            _insn(Opcode.ADDI, (4, 4, 4), address=20),
            _insn(Opcode.BC, (16, 0, 16, 0), address=24),
        ]
        with tempfile.TemporaryDirectory() as tmp:
            dol_path = Path(tmp) / "main.dol"
            text = b"\x00" * 8
            blob = _build_dol(
                text_sections=[(0x80004000, 0x100, text)],
                data_sections=[(_TABLE_ADDR, 0x100 + len(text), struct.pack(">I", 3) + b"\x00" * 252)],
            )
            dol_path.write_bytes(blob)
            readonly = try_build_memory_loop_readonly_words(program, dol_path=dol_path)
        self.assertIsNotNone(readonly)
        assert readonly is not None
        loops = find_constant_stride_store_loops(program, readonly_words=readonly)
        self.assertEqual(loops[0].confidence, "exact-pattern")
        self.assertEqual(loops[0].trip_count, 3)

    def test_hydrates_word_from_linked_elf(self) -> None:
        elf = build_elf_with_rodata(
            rodata=struct.pack(">I", 11),
            rodata_addr=_TABLE_ADDR,
            e_type=2,
        )
        with tempfile.TemporaryDirectory() as tmp:
            elf_path = Path(tmp) / "main.elf"
            elf_path.write_bytes(elf)
            words = hydrate_readonly_words({_TABLE_ADDR}, elf_path=elf_path)
        self.assertEqual(words[_TABLE_ADDR], 11)

    def test_fail_closed_without_image(self) -> None:
        with self.assertRaises(MemoryLoopImageError):
            hydrate_readonly_words({0x80201000})


if __name__ == "__main__":
    unittest.main()
