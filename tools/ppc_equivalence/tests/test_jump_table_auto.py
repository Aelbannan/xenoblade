"""Tests for jump-table table-base recovery and auto proof-context building."""

from __future__ import annotations

import tempfile
import unittest
from pathlib import Path
from struct import pack

from tools.ppc_equivalence.decoder import decode_block
from tools.ppc_equivalence.ir import Instruction, Opcode, RelocationRef, R_PPC_EMB_SDA21
from tools.ppc_equivalence.jump_table import find_jump_table_candidates
from tools.ppc_equivalence.jump_table_auto import (
    resolve_table_base_va,
    table_base_reg,
    try_auto_jump_table_context,
)
from tools.ppc_equivalence.jump_table_obligations import (
    JumpTableProofContext,
    build_jump_table_obligations,
)
from tools.ppc_equivalence.tests.fixtures.jump_table_elf import (
    DEFAULT_TABLE_BASE,
    build_linked_jump_table_elf,
    dual_base_table_fixture,
    identical_table_fixture,
    sda_addi_switch_body,
    sequential_case_targets,
)
from tools.ppc_equivalence.tests.fixtures.jump_table_retail import (
    DEFAULT_RETAIL_DOL,
    RETAIL_BRANCH_PC,
    RETAIL_TABLE_BASE,
)


class ResolveTableBaseTests(unittest.TestCase):
    def test_resolves_lis_addi_materialization(self) -> None:
        code = bytes.fromhex(
            "3c608020 38630000 28000003 5400103a 7c63002e 7c6903a6 4e800420"
        )
        insns = decode_block(code, 0x80100000, validate_with_capstone=False)
        candidates = find_jump_table_candidates(insns)
        self.assertEqual(len(candidates), 1)
        self.assertEqual(resolve_table_base_va(insns, candidates[0]), 0x80200000)
        self.assertEqual(table_base_reg(candidates[0], insns), 3)

    def test_resolves_sda_addi_from_r13(self) -> None:
        switch = sda_addi_switch_body(entry_count=4, sda_reg=13, displacement=0x1BC)
        insns = decode_block(switch, 0x80100000, validate_with_capstone=False)
        candidates = find_jump_table_candidates(insns)
        self.assertEqual(len(candidates), 1)
        self.assertIsNone(resolve_table_base_va(insns, candidates[0]))
        self.assertEqual(
            resolve_table_base_va(
                insns,
                candidates[0],
                sda_bases={13: 0x8052B000},
            ),
            0x8052B1BC,
        )

    def test_resolves_sda21_symbol_on_addi(self) -> None:
        switch = sda_addi_switch_body(entry_count=4, sda_reg=13, displacement=0)
        insns = decode_block(switch, 0x80100000, validate_with_capstone=False)
        addi = insns[0]
        insns = [
            Instruction(
                address=addi.address,
                opcode=Opcode.ADDI,
                operands=addi.operands,
                raw=addi.raw,
                relocation=RelocationRef(
                    offset=0,
                    relocation_type=R_PPC_EMB_SDA21,
                    symbol="jump_table",
                    canonical_symbol="jump_table",
                    addend=0,
                ),
            ),
            *insns[1:],
        ]
        candidates = find_jump_table_candidates(insns)
        self.assertEqual(
            resolve_table_base_va(
                insns,
                candidates[0],
                symbol_addresses={"jump_table": 0x8052B1BC},
            ),
            0x8052B1BC,
        )

    def test_lwz_sda_without_symbol_is_fail_closed(self) -> None:
        # lwz r3, 0x20(r13); cmplwi ... lwzx ...
        code = b"".join(
            [
                pack(">I", (32 << 26) | (3 << 21) | (13 << 16) | 0x20),
                bytes.fromhex("28000003 5400103a 7c63002e 7c6903a6 4e800420"),
            ]
        )
        insns = decode_block(code, 0x80100000, validate_with_capstone=False)
        candidates = find_jump_table_candidates(insns)
        self.assertEqual(len(candidates), 1)
        self.assertIsNone(
            resolve_table_base_va(
                insns,
                candidates[0],
                sda_bases={13: 0x80400000},
            )
        )

    def test_resolves_retail_fixture_with_lis_in_window(self) -> None:
        if not DEFAULT_RETAIL_DOL.is_file():
            self.skipTest(f"retail DOL missing: {DEFAULT_RETAIL_DOL}")
        from tools.ppc_equivalence.dol_symbols import extract_by_address

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


class JumpTableProofContextTests(unittest.TestCase):
    def test_expansion_map_uses_per_side_targets(self) -> None:
        from tools.ppc_equivalence.jump_table_obligations import JumpTableWords
        from tools.ppc_equivalence.jump_table_pairing import pair_jump_table_cases

        original = JumpTableWords(
            base=0x80200000,
            words=(0x80101000, 0x80101004),
            source="linked-elf",
        )
        candidate = JumpTableWords(
            base=0x80300000,
            words=(0x80111000, 0x80111004),
            source="linked-elf",
        )
        pairing = pair_jump_table_cases(
            original_words=original.words,
            candidate_words=candidate.words,
            table_base_original=original.base,
            table_base_candidate=candidate.base,
        )
        context = JumpTableProofContext(
            table=original,
            candidate_table=candidate,
            pairing=pairing,
            branch_pc=0x80100010,
            candidate_branch_pc=0x80200010,
        )
        mapping = context.expansion_map()
        self.assertEqual(mapping[0x80100010], (0x80101000, 0x80101004))
        self.assertEqual(mapping[0x80200010], (0x80111000, 0x80111004))

    def test_build_obligations_includes_candidate_companion(self) -> None:
        from tools.ppc_equivalence.jump_table_obligations import JumpTableWords
        from tools.ppc_equivalence.jump_table_pairing import pair_jump_table_cases

        original = JumpTableWords(
            base=0x80200000,
            words=(0x80101000,),
            source="linked-elf",
        )
        candidate = JumpTableWords(
            base=0x80300000,
            words=(0x80111000,),
            source="linked-elf",
        )
        pairing = pair_jump_table_cases(
            original_words=original.words,
            candidate_words=candidate.words,
            table_base_original=original.base,
            table_base_candidate=candidate.base,
        )
        context = JumpTableProofContext(
            table=original,
            candidate_table=candidate,
            pairing=pairing,
            branch_pc=0x80100010,
            candidate_branch_pc=0x80200010,
        )
        address_space, indirect_targets = build_jump_table_obligations(context)
        self.assertIn("candidate", address_space)
        self.assertIn("candidate", indirect_targets)
        self.assertEqual(address_space["candidate"]["base"], 0x80300000)


class AutoJumpTableContextTests(unittest.TestCase):
    def test_builds_context_from_linked_elf(self) -> None:
        elf_bytes, _targets, table_base = identical_table_fixture(entry_count=4)
        ha = (table_base + 0x8000) >> 16
        lo = table_base & 0xFFFF
        if lo >= 0x8000:
            lo -= 0x10000
        switch = b"".join(
            [
                pack(">I", (15 << 26) | (3 << 21) | (0 << 16) | (ha & 0xFFFF)),
                pack(">I", (14 << 26) | (3 << 21) | (3 << 16) | (lo & 0xFFFF)),
                bytes.fromhex("28000003 5400103a 7c63002e 7c6903a6 4e800420"),
            ]
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
        self.assertIsNone(context.candidate_table)
        self.assertEqual(context.table_base_reg, 3)
        self.assertEqual(context.index_reg, 0)

    def test_dual_base_hydrates_and_pairs(self) -> None:
        (
            original_elf,
            candidate_elf,
            original_targets,
            candidate_targets,
            original_base,
            candidate_base,
        ) = dual_base_table_fixture(entry_count=4)
        from tools.ppc_equivalence.tests.fixtures.jump_table_elf import _switch_with_table_base

        orig_switch = _switch_with_table_base(original_base, entry_count=4)
        cand_switch = _switch_with_table_base(candidate_base, entry_count=4)
        orig_insns = decode_block(orig_switch, 0x80100000, validate_with_capstone=False)
        cand_insns = decode_block(cand_switch, 0x80100000, validate_with_capstone=False)
        with tempfile.TemporaryDirectory() as tmp:
            orig_path = Path(tmp) / "original.elf"
            cand_path = Path(tmp) / "candidate.elf"
            orig_path.write_bytes(original_elf)
            cand_path.write_bytes(candidate_elf)
            context = try_auto_jump_table_context(
                orig_insns,
                cand_insns,
                original_elf_path=orig_path,
                candidate_elf_path=cand_path,
            )
        self.assertIsNotNone(context)
        assert context is not None
        self.assertEqual(context.table.base, original_base)
        self.assertIsNotNone(context.candidate_table)
        assert context.candidate_table is not None
        self.assertEqual(context.candidate_table.base, candidate_base)
        self.assertIsNotNone(context.pairing)
        assert context.pairing is not None
        self.assertEqual(context.pairing.cases[0].original_pc, original_targets[0])
        self.assertEqual(context.pairing.cases[0].candidate_pc, candidate_targets[0])
        mapping = context.expansion_map()
        self.assertEqual(
            context.original_expansion_map()[context.branch_pc],
            tuple(target & 0xFFFFFFFC for target in original_targets),
        )
        cand_branch = context.candidate_branch_pc or context.branch_pc
        self.assertEqual(
            context.candidate_expansion_map()[cand_branch],
            tuple(target & 0xFFFFFFFC for target in candidate_targets),
        )
        if context.branch_pc != cand_branch:
            self.assertIn(context.branch_pc, mapping)
            self.assertIn(cand_branch, mapping)

    def test_sda_addi_auto_context(self) -> None:
        table_base = 0x8052B1BC
        targets = sequential_case_targets(entry_count=4)
        switch = sda_addi_switch_body(entry_count=4, sda_reg=13, displacement=0x1BC)
        elf = build_linked_jump_table_elf(
            table_targets=targets,
            switch_body=switch,
            table_base=table_base,
        )
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "linked.elf"
            path.write_bytes(elf)
            insns = decode_block(switch, 0x80100000, validate_with_capstone=False)
            context = try_auto_jump_table_context(
                insns,
                insns,
                elf_path=path,
                sda_bases={13: 0x8052B000},
            )
        self.assertIsNotNone(context)
        assert context is not None
        self.assertEqual(context.table.base, table_base)

    def test_rejects_mismatched_table_bases_without_hydration(self) -> None:
        from tools.ppc_equivalence.tests.fixtures.jump_table_elf import _switch_with_table_base

        orig_switch = _switch_with_table_base(0x80200000, entry_count=4)
        cand_switch = _switch_with_table_base(0x80300000, entry_count=4)
        orig_insns = decode_block(orig_switch, 0x80100000, validate_with_capstone=False)
        cand_insns = decode_block(cand_switch, 0x80100000, validate_with_capstone=False)
        self.assertIsNone(
            try_auto_jump_table_context(orig_insns, cand_insns),
        )

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
