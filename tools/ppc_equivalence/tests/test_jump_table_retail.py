"""End-to-end retail jump-table fixture: pattern, hydration, pairing."""

from __future__ import annotations

import unittest
from pathlib import Path

from tools.ppc_equivalence.jump_table_image import hydrate_jump_table
from tools.ppc_equivalence.jump_table_pairing import pair_jump_table_cases
from tools.ppc_equivalence.tests.fixtures.jump_table_retail import (
    DEFAULT_RETAIL_DOL,
    RETAIL_BOUND_IMM,
    RETAIL_BRANCH_PC,
    RETAIL_ENTRY_COUNT,
    RETAIL_TABLE_BASE,
    RETAIL_TABLE_WORDS,
    find_retail_jump_table_candidates,
    load_documented_fixture,
)

_HAS_RETAIL_DOL = DEFAULT_RETAIL_DOL.is_file()


@unittest.skipUnless(_HAS_RETAIL_DOL, "retail main.dol not present at orig/us/sys/main.dol")
class RetailJumpTableFixtureTests(unittest.TestCase):
    def test_scan_finds_documented_candidate(self) -> None:
        candidates = find_retail_jump_table_candidates(DEFAULT_RETAIL_DOL)
        branch_pcs = {candidate.branch_pc for candidate in candidates}
        self.assertIn(RETAIL_BRANCH_PC, branch_pcs)
        documented = [
            candidate
            for candidate in candidates
            if candidate.branch_pc == RETAIL_BRANCH_PC
        ]
        self.assertEqual(len(documented), 1)
        self.assertEqual(documented[0].confidence, "exact-pattern")
        self.assertEqual(documented[0].bound_imm, RETAIL_BOUND_IMM)

    def test_documented_fixture_pattern_recognition(self) -> None:
        fixture = load_documented_fixture(DEFAULT_RETAIL_DOL)
        self.assertEqual(fixture.branch_pc, RETAIL_BRANCH_PC)
        self.assertEqual(fixture.entry_count, RETAIL_ENTRY_COUNT)
        self.assertEqual(fixture.candidate.confidence, "exact-pattern")
        self.assertEqual(fixture.candidate.bound_imm, RETAIL_BOUND_IMM)
        self.assertEqual(fixture.candidate.table_base_expr, "r3")

    def test_hydrate_jump_table_from_dol(self) -> None:
        fixture = load_documented_fixture(DEFAULT_RETAIL_DOL)
        image = hydrate_jump_table(
            base=RETAIL_TABLE_BASE,
            entry_count=fixture.entry_count,
            dol_path=DEFAULT_RETAIL_DOL,
        )
        self.assertEqual(image.source, "linked-dol")
        self.assertEqual(image.base, RETAIL_TABLE_BASE)
        self.assertEqual(image.entry_count, RETAIL_ENTRY_COUNT)
        self.assertEqual(image.entry_size, 4)
        self.assertEqual(image.words, RETAIL_TABLE_WORDS)
        for index, word in enumerate(image.words):
            self.assertEqual(word & 3, 0, f"entry {index} target 0x{word:x} not aligned")

    def test_entry_count_matches_bound_plus_one(self) -> None:
        fixture = load_documented_fixture(DEFAULT_RETAIL_DOL)
        self.assertEqual(fixture.entry_count, fixture.bound_imm + 1)
        image = hydrate_jump_table(
            base=RETAIL_TABLE_BASE,
            entry_count=fixture.entry_count,
            dol_path=DEFAULT_RETAIL_DOL,
        )
        self.assertEqual(len(image.words), fixture.bound_imm + 1)

    def test_pair_jump_table_cases_retail_vs_retail(self) -> None:
        image = hydrate_jump_table(
            base=RETAIL_TABLE_BASE,
            entry_count=RETAIL_ENTRY_COUNT,
            dol_path=DEFAULT_RETAIL_DOL,
        )
        pairing = pair_jump_table_cases(
            original_words=image.words,
            candidate_words=image.words,
            table_base_original=RETAIL_TABLE_BASE,
            table_base_candidate=RETAIL_TABLE_BASE,
        )
        self.assertEqual(pairing.entry_count, RETAIL_ENTRY_COUNT)
        for case in pairing.cases:
            self.assertEqual(case.original_pc, case.candidate_pc)
            self.assertEqual(case.original_pc, RETAIL_TABLE_WORDS[case.index])


class RetailJumpTableGoldenTests(unittest.TestCase):
    """Golden vectors always run (no DOL required)."""

    def test_golden_words_are_word_aligned(self) -> None:
        for index, word in enumerate(RETAIL_TABLE_WORDS):
            self.assertEqual(word & 3, 0, f"golden entry {index} misaligned")

    def test_golden_entry_count(self) -> None:
        self.assertEqual(len(RETAIL_TABLE_WORDS), RETAIL_ENTRY_COUNT)
        self.assertEqual(RETAIL_ENTRY_COUNT, RETAIL_BOUND_IMM + 1)


if __name__ == "__main__":
    unittest.main()
