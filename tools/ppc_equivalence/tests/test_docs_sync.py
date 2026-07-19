from __future__ import annotations

import tempfile
import unittest
from pathlib import Path

from tools.ppc_equivalence.result import ARCHITECTURE_MODEL, ProofStatus
from tools.ppc_equivalence.docs_sync import (
    TABLE_BEGIN,
    TABLE_END,
    VERSION_BEGIN,
    VERSION_END,
    _run,
    generate_readme,
    generate_status_table,
    generate_version_block,
    replace_section,
)


class TestDocsSync(unittest.TestCase):
    def test_version_block_contains_expected_values(self) -> None:
        block = generate_version_block()
        self.assertIn(VERSION_BEGIN, block)
        self.assertIn(VERSION_END, block)
        self.assertIn(ARCHITECTURE_MODEL, block)
        self.assertIn("`8`", block)
        self.assertIn("`2`", block)

    def test_status_table_contains_all_statuses(self) -> None:
        block = generate_status_table()
        self.assertIn(TABLE_BEGIN, block)
        self.assertIn(TABLE_END, block)
        for member in ProofStatus:
            self.assertIn(member.name, block)
            self.assertIn(member.value, block)

    def test_write_updates_content_and_check_passes(self) -> None:
        with tempfile.NamedTemporaryFile(mode="w", suffix=".md", delete=False) as f:
            f.write("# Test\n\n## Section\n\ncontent\n")
            tmp = Path(f.name)
        try:
            self.assertEqual(_run(True, False, tmp), 0)
            written = tmp.read_text(encoding="utf-8")
            self.assertIn(VERSION_BEGIN, written)
            self.assertIn(VERSION_END, written)
            self.assertIn(TABLE_BEGIN, written)
            self.assertIn(TABLE_END, written)
            self.assertIn(ARCHITECTURE_MODEL, written)
            self.assertIn("EQUIVALENT", written)
            self.assertEqual(_run(False, True, tmp), 0)
            tmp.write_text("# Stale\n\n## Section\n\nstale\n", encoding="utf-8")
            self.assertNotEqual(_run(False, True, tmp), 0)
        finally:
            tmp.unlink()

    def test_write_with_existing_markers(self) -> None:
        content = (
            "# Test\n\n"
            f"{VERSION_BEGIN}\n{VERSION_END}\n\n"
            "## Section\n\n"
            f"{TABLE_BEGIN}\n{TABLE_END}\n"
        )
        with tempfile.NamedTemporaryFile(mode="w", suffix=".md", delete=False) as f:
            f.write(content)
            tmp = Path(f.name)
        try:
            self.assertEqual(_run(True, False, tmp), 0)
            self.assertEqual(_run(False, True, tmp), 0)
        finally:
            tmp.unlink()

    def test_appends_when_markers_missing(self) -> None:
        with tempfile.NamedTemporaryFile(mode="w", suffix=".md", delete=False) as f:
            f.write("# Title\n\npreamble\n\n## Feature One\n\ndetails\n\n## Feature Two\n\nmore\n")
            tmp = Path(f.name)
        try:
            self.assertEqual(_run(True, False, tmp), 0)
            written = tmp.read_text(encoding="utf-8")
            version_pos = written.find(VERSION_BEGIN)
            table_pos = written.find(TABLE_BEGIN)
            section_pos = written.find("## Feature One")
            self.assertGreaterEqual(version_pos, 0)
            self.assertGreaterEqual(table_pos, 0)
            self.assertLess(version_pos, section_pos)
            self.assertLess(table_pos, section_pos)
            self.assertEqual(_run(False, True, tmp), 0)
        finally:
            tmp.unlink()

    def test_replace_section_known_markers(self) -> None:
        text = "before\nBEGIN\nold\nEND\nafter\n"
        result = replace_section(text, "BEGIN", "END", "BEGIN\nnew\nEND")
        self.assertIn("before\n", result)
        self.assertIn("BEGIN\nnew\nEND\n", result)
        self.assertIn("after\n", result)
        self.assertNotIn("old", result)

    def test_replace_section_missing_markers_appends_before_heading(self) -> None:
        text = "intro\n\n## Heading\n\nbody\n"
        result = replace_section(text, "BEGIN", "END", "BEGIN\ncontent\nEND")
        self.assertIn("BEGIN\ncontent\nEND\n## Heading", result)
