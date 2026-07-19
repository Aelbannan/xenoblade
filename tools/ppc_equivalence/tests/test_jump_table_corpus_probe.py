"""Regression tests for retail jump-table auto-context productization."""

from __future__ import annotations

import tempfile
import unittest
from pathlib import Path
from struct import pack

from tools.ppc_equivalence.decoder import decode_block
from tools.ppc_equivalence.jump_table_auto import (
    expand_jump_table_instructions,
    resolve_table_base_va,
    try_auto_jump_table_context,
)
from tools.ppc_equivalence.jump_table import find_jump_table_candidates
from tools.ppc_equivalence.jump_table_corpus_probe import (
    DOCUMENTED_EXACT_BRANCH_PCS,
    probe_branch_pc,
    probe_retail_exact_patterns,
    summarize_probe_results,
)
from tools.ppc_equivalence.tests.fixtures.jump_table_corpus_snippets import (
    CQST_LOG_INFO_BOUND_IMM,
    CQST_LOG_INFO_BRANCH_PC,
    CQST_LOG_INFO_SWITCH_BASE,
    CQST_LOG_INFO_SWITCH_HEX,
    CQST_LOG_INFO_TABLE_BASE,
)
from tools.ppc_equivalence.tests.fixtures.jump_table_elf import (
    build_linked_jump_table_elf,
    sequential_case_targets,
)
from tools.ppc_equivalence.tests.fixtures.jump_table_retail import DEFAULT_RETAIL_DOL


class ExpandJumpTableInstructionsTests(unittest.TestCase):
    def test_embedded_corpus_snippet_recovers_table_base_without_dol(self) -> None:
        code = bytes.fromhex(CQST_LOG_INFO_SWITCH_HEX)
        insns = decode_block(code, CQST_LOG_INFO_SWITCH_BASE, validate_with_capstone=False)
        candidates = [
            item for item in find_jump_table_candidates(insns)
            if item.branch_pc == CQST_LOG_INFO_BRANCH_PC
        ]
        self.assertEqual(len(candidates), 1)
        self.assertEqual(
            resolve_table_base_va(insns, candidates[0]),
            CQST_LOG_INFO_TABLE_BASE,
        )
        self.assertEqual(candidates[0].bound_imm, CQST_LOG_INFO_BOUND_IMM)

    def test_expand_uses_linked_dol_window(self) -> None:
        if not DEFAULT_RETAIL_DOL.is_file():
            self.skipTest(f"retail DOL missing: {DEFAULT_RETAIL_DOL}")
        short_start = CQST_LOG_INFO_BRANCH_PC - 48
        short = decode_block(
            __import__(
                "tools.ppc_equivalence.dol_symbols", fromlist=["extract_by_address"]
            ).extract_by_address(
                DEFAULT_RETAIL_DOL,
                short_start,
                CQST_LOG_INFO_BRANCH_PC + 4 - short_start,
            ).code,
            short_start,
            validate_with_capstone=False,
        )
        expanded = expand_jump_table_instructions(short, dol_path=DEFAULT_RETAIL_DOL)
        candidates = [
            item for item in find_jump_table_candidates(expanded)
            if item.branch_pc == CQST_LOG_INFO_BRANCH_PC
        ]
        self.assertEqual(len(candidates), 1)
        self.assertEqual(
            resolve_table_base_va(expanded, candidates[0]),
            CQST_LOG_INFO_TABLE_BASE,
        )


class JumpTableCorpusProbeTests(unittest.TestCase):
    def test_offline_snippet_builds_auto_context_with_temp_elf(self) -> None:
        code = bytes.fromhex(CQST_LOG_INFO_SWITCH_HEX)
        insns = decode_block(code, CQST_LOG_INFO_SWITCH_BASE, validate_with_capstone=False)
        targets = sequential_case_targets(entry_count=CQST_LOG_INFO_BOUND_IMM + 1)
        elf = build_linked_jump_table_elf(
            table_targets=targets,
            switch_body=code[-20:],
            table_base=CQST_LOG_INFO_TABLE_BASE,
        )
        with tempfile.TemporaryDirectory() as tmp:
            elf_path = Path(tmp) / "linked.elf"
            elf_path.write_bytes(elf)
            context = try_auto_jump_table_context(
                insns,
                insns,
                elf_path=elf_path,
                candidate_elf_path=elf_path,
            )
        self.assertIsNotNone(context)
        assert context is not None
        self.assertEqual(context.table.base, CQST_LOG_INFO_TABLE_BASE)
        self.assertEqual(len(context.table.words), CQST_LOG_INFO_BOUND_IMM + 1)

    def test_documented_examples_when_dol_present(self) -> None:
        if not DEFAULT_RETAIL_DOL.is_file():
            self.skipTest(f"retail DOL missing: {DEFAULT_RETAIL_DOL}")
        results = [probe_branch_pc(pc, DEFAULT_RETAIL_DOL) for pc in DOCUMENTED_EXACT_BRANCH_PCS]
        recognized = [item for item in results if item.confidence == "exact-pattern"]
        self.assertGreaterEqual(len(recognized), 6)
        self.assertTrue(all(item.auto_context for item in recognized))

    def test_full_retail_scan_reports_exact_count(self) -> None:
        if not DEFAULT_RETAIL_DOL.is_file():
            self.skipTest(f"retail DOL missing: {DEFAULT_RETAIL_DOL}")
        results = probe_retail_exact_patterns(DEFAULT_RETAIL_DOL)
        summary = summarize_probe_results(results)
        self.assertGreaterEqual(summary["exact_pattern"], 20)
        self.assertGreaterEqual(
            summary["auto_context_ok"],
            int(summary["exact_pattern"] * 0.8),
        )


if __name__ == "__main__":
    unittest.main()
