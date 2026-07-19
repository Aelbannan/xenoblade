"""Tests for logical jump-table case pairing and linked ELF fixtures."""

from __future__ import annotations

import tempfile
import unittest
from pathlib import Path

from tools.ppc_equivalence.jump_table_image import hydrate_jump_table
from tools.ppc_equivalence.jump_table_obligations import validate_indirect_targets_obligation
from tools.ppc_equivalence.jump_table_pairing import (
    JumpTablePairingError,
    indirect_targets_obligation_for_side,
    indirect_targets_obligations_for_pairing,
    pair_jump_table_cases,
)
from tools.ppc_equivalence.tests.fixtures.jump_table_elf import (
    DEFAULT_TABLE_BASE,
    DEFAULT_TEXT_ADDR,
    cmplwi_bound_hex,
    differing_case_fixture,
    identical_table_fixture,
    mismatched_entry_count_fixtures,
    sequential_case_targets,
)


_BRANCH_PC = DEFAULT_TEXT_ADDR + 0x10


class JumpTablePairingTests(unittest.TestCase):
    def test_pairs_by_index_not_address(self) -> None:
        original = (0x80101000, 0x80101004, 0x80101008)
        candidate = (0x80201000, 0x80201004, 0x80201008)
        pairing = pair_jump_table_cases(
            original_words=original,
            candidate_words=candidate,
            table_base_original=0x80200000,
            table_base_candidate=0x80300000,
        )
        self.assertEqual(pairing.entry_count, 3)
        self.assertEqual(pairing.cases[0].identity, "case-0")
        self.assertEqual(pairing.cases[1].original_pc, 0x80101004)
        self.assertEqual(pairing.cases[1].candidate_pc, 0x80201004)

    def test_identical_words_pair_with_matching_pcs(self) -> None:
        words = sequential_case_targets(entry_count=4)
        pairing = pair_jump_table_cases(
            original_words=words,
            candidate_words=words,
            table_base_original=DEFAULT_TABLE_BASE,
            table_base_candidate=DEFAULT_TABLE_BASE,
        )
        for case in pairing.cases:
            self.assertEqual(case.original_pc, case.candidate_pc)

    def test_entry_count_mismatch_raises(self) -> None:
        with self.assertRaises(JumpTablePairingError) as ctx:
            pair_jump_table_cases(
                original_words=(0x80101000, 0x80101004),
                candidate_words=(0x80101000,),
            )
        self.assertIn("entry count mismatch", str(ctx.exception))

    def test_empty_words_raises(self) -> None:
        with self.assertRaises(JumpTablePairingError):
            pair_jump_table_cases(original_words=(), candidate_words=())

    def test_indirect_targets_obligation_for_side(self) -> None:
        pairing = pair_jump_table_cases(
            original_words=(0x80101000, 0x80101004),
            candidate_words=(0x80201000, 0x80201004),
        )
        original_ob = indirect_targets_obligation_for_side(
            pairing,
            branch_pc=_BRANCH_PC,
            side="original",
            source="linked-elf",
            artifact_hashes=("dead" * 8,),
        )
        candidate_ob = indirect_targets_obligation_for_side(
            pairing,
            branch_pc=_BRANCH_PC + 0x100,
            side="candidate",
            source="linked-elf",
            artifact_hashes=("beef" * 8,),
        )
        self.assertIsNone(validate_indirect_targets_obligation(original_ob))
        self.assertIsNone(validate_indirect_targets_obligation(candidate_ob))
        self.assertEqual(original_ob["targets"][0]["pc"], 0x80101000)
        self.assertEqual(candidate_ob["targets"][1]["pc"], 0x80201004)
        self.assertEqual(original_ob["targets"][0]["identity"], "case-0")

    def test_indirect_targets_obligations_for_pairing(self) -> None:
        pairing = pair_jump_table_cases(
            original_words=(0x80101000,),
            candidate_words=(0x80201000,),
        )
        left, right = indirect_targets_obligations_for_pairing(
            pairing,
            branch_pc_original=_BRANCH_PC,
            branch_pc_candidate=_BRANCH_PC + 4,
            source_original="retail",
            source_candidate="decomp",
            artifact_hashes=("aa" * 32,),
        )
        self.assertIsNone(validate_indirect_targets_obligation(left))
        self.assertIsNone(validate_indirect_targets_obligation(right))
        self.assertEqual(left["source"], "retail")
        self.assertEqual(right["source"], "decomp")


class JumpTableFixturePairingTests(unittest.TestCase):
    def test_hydrate_identical_fixture_and_pair(self) -> None:
        elf, targets, table_base = identical_table_fixture(entry_count=4)
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "identical.elf"
            path.write_bytes(elf)
            image = hydrate_jump_table(
                base=table_base,
                entry_count=len(targets),
                elf_path=path,
            )
        pairing = pair_jump_table_cases(
            original_words=image.words,
            candidate_words=image.words,
            table_base_original=table_base,
            table_base_candidate=table_base,
        )
        self.assertEqual(pairing.entry_count, 4)
        self.assertEqual(
            [case.original_pc for case in pairing.cases],
            list(targets),
        )

    def test_hydrate_differing_case_fixture(self) -> None:
        (
            original_elf,
            candidate_elf,
            original_targets,
            candidate_targets,
            table_base,
        ) = differing_case_fixture(entry_count=4, differing_index=1)
        with tempfile.TemporaryDirectory() as tmp:
            orig_path = Path(tmp) / "original.elf"
            cand_path = Path(tmp) / "candidate.elf"
            orig_path.write_bytes(original_elf)
            cand_path.write_bytes(candidate_elf)
            orig_image = hydrate_jump_table(
                base=table_base,
                entry_count=len(original_targets),
                elf_path=orig_path,
            )
            cand_image = hydrate_jump_table(
                base=table_base,
                entry_count=len(candidate_targets),
                elf_path=cand_path,
            )
        pairing = pair_jump_table_cases(
            original_words=orig_image.words,
            candidate_words=cand_image.words,
            table_base_original=table_base,
            table_base_candidate=table_base,
        )
        self.assertEqual(pairing.cases[0].original_pc, pairing.cases[0].candidate_pc)
        self.assertNotEqual(pairing.cases[1].original_pc, pairing.cases[1].candidate_pc)
        self.assertEqual(pairing.cases[1].identity, "case-1")

    def test_mismatched_entry_count_fixture_raises_on_pair(self) -> None:
        four_elf, three_elf, four_count, three_count = mismatched_entry_count_fixtures()
        with tempfile.TemporaryDirectory() as tmp:
            four_path = Path(tmp) / "four.elf"
            three_path = Path(tmp) / "three.elf"
            four_path.write_bytes(four_elf)
            three_path.write_bytes(three_elf)
            four_image = hydrate_jump_table(
                base=DEFAULT_TABLE_BASE,
                entry_count=four_count,
                elf_path=four_path,
            )
            three_image = hydrate_jump_table(
                base=DEFAULT_TABLE_BASE,
                entry_count=three_count,
                elf_path=three_path,
            )
        with self.assertRaises(JumpTablePairingError):
            pair_jump_table_cases(
                original_words=four_image.words,
                candidate_words=three_image.words,
            )

    def test_cmplwi_bound_hex_tracks_entry_count(self) -> None:
        self.assertEqual(
            cmplwi_bound_hex(4),
            "28000003 5400103a 7c63002e 7c6903a6 4e800420",
        )
        self.assertTrue(cmplwi_bound_hex(9).startswith("28000008"))


if __name__ == "__main__":
    unittest.main()
