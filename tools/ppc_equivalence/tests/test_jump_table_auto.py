"""Tests for jump-table table-base recovery and auto proof-context building."""

from __future__ import annotations

import tempfile
import unittest
from pathlib import Path

from tools.ppc_equivalence.decoder import decode_block
from tools.ppc_equivalence.jump_table import find_jump_table_candidates
from tools.ppc_equivalence.jump_table_auto import (
    resolve_table_base_va,
    table_base_reg,
    try_auto_jump_table_context,
)
from tools.ppc_equivalence.tests.fixtures.jump_table_elf import (
    DEFAULT_TABLE_BASE,
    identical_table_fixture,
)
from tools.ppc_equivalence.tests.fixtures.jump_table_retail import (
    DEFAULT_RETAIL_DOL,
    RETAIL_BRANCH_PC,
    RETAIL_TABLE_BASE,
)


class ResolveTableBaseTests(unittest.TestCase):
    def test_resolves_lis_addi_materialization(self) -> None:
        # lis r3, 0x8020; addi r3, r3, 0; slwi; lwzx; mtctr; bctr
        # 3c608020 38630000 5400103a 7c63002e 7c6903a6 4e800420
        code = bytes.fromhex(
            "3c608020 38630000 28000003 5400103a 7c63002e 7c6903a6 4e800420"
        )
        insns = decode_block(code, 0x80100000, validate_with_capstone=False)
        candidates = find_jump_table_candidates(insns)
        self.assertEqual(len(candidates), 1)
        self.assertEqual(resolve_table_base_va(insns, candidates[0]), 0x80200000)
        self.assertEqual(table_base_reg(candidates[0], insns), 3)

    def test_resolves_retail_fixture_with_lis_in_window(self) -> None:
        if not DEFAULT_RETAIL_DOL.is_file():
            self.skipTest(f"retail DOL missing: {DEFAULT_RETAIL_DOL}")
        from tools.ppc_equivalence.dol_symbols import extract_by_address

        # Include addis at 0x800e0d34 through bctr at 0x800e0d8c.
        start = 0x800E0D34
        end = RETAIL_BRANCH_PC + 4
        slice_ = extract_by_address(DEFAULT_RETAIL_DOL, start, end - start)
        insns = decode_block(slice_.code, start, validate_with_capstone=False)
        candidates = [
            item for item in find_jump_table_candidates(insns)
            if item.branch_pc == RETAIL_BRANCH_PC
        ]
        self.assertEqual(len(candidates), 1)
        self.assertEqual(resolve_table_base_va(insns, candidates[0]), RETAIL_TABLE_BASE)


class AutoJumpTableContextTests(unittest.TestCase):
    def test_builds_context_from_linked_elf(self) -> None:
        elf_bytes, _targets, table_base = identical_table_fixture(entry_count=4)
        # lis r3, ha(table); addi r3,r3,lo; cmplwi; slwi; lwzx; mtctr; bctr
        ha = (table_base + 0x8000) >> 16
        lo = table_base & 0xFFFF
        if lo >= 0x8000:
            lo -= 0x10000
        from struct import pack

        switch = b"".join(
            [
                pack(">I", (15 << 26) | (3 << 21) | (0 << 16) | (ha & 0xFFFF)),  # addis
                pack(">I", (14 << 26) | (3 << 21) | (3 << 16) | (lo & 0xFFFF)),  # addi
                bytes.fromhex("28000003 5400103a 7c63002e 7c6903a6 4e800420"),
            ]
        )
        from tools.ppc_equivalence.tests.fixtures.jump_table_elf import (
            build_linked_jump_table_elf,
            sequential_case_targets,
        )

        elf = build_linked_jump_table_elf(
            table_targets=sequential_case_targets(entry_count=4),
            switch_body=switch,
            table_base=table_base,
        )
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "linked.elf"
            path.write_bytes(elf)
            insns = decode_block(switch, 0x80100000, validate_with_capstone=False)
            context = try_auto_jump_table_context(
                insns, insns, elf_path=path,
            )
        self.assertIsNotNone(context)
        assert context is not None
        self.assertEqual(context.table.base, DEFAULT_TABLE_BASE)
        self.assertEqual(len(context.table.words), 4)
        self.assertEqual(context.table_base_reg, 3)
        self.assertEqual(context.index_reg, 0)

    def test_retail_identical_sides_build_context(self) -> None:
        if not DEFAULT_RETAIL_DOL.is_file():
            self.skipTest(f"retail DOL missing: {DEFAULT_RETAIL_DOL}")
        from tools.ppc_equivalence.dol_symbols import extract_by_address

        start = 0x800E0D34
        end = RETAIL_BRANCH_PC + 4
        slice_ = extract_by_address(DEFAULT_RETAIL_DOL, start, end - start)
        insns = decode_block(slice_.code, start, validate_with_capstone=False)
        context = try_auto_jump_table_context(
            insns, insns, dol_path=DEFAULT_RETAIL_DOL,
        )
        self.assertIsNotNone(context)
        assert context is not None
        self.assertEqual(context.table.base, RETAIL_TABLE_BASE)
        self.assertEqual(len(context.table.words), 14)
        self.assertEqual(context.branch_pc, RETAIL_BRANCH_PC)


if __name__ == "__main__":
    unittest.main()
