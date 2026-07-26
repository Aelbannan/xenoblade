"""Tests for tools/llm_decomp/patcher.py."""
from __future__ import annotations

import tempfile
import unittest
from pathlib import Path

from tools.llm_decomp.patcher import (FilePatch, PatchOutcome, SearchReplace,
                                      apply_patch)


class PatcherTest(unittest.TestCase):
    def setUp(self) -> None:
        self._tmp = tempfile.TemporaryDirectory()
        self.root = Path(self._tmp.name)
        self.writable = {"src/a.cpp", "include/a.hpp"}
        (self.root / "src").mkdir()
        (self.root / "src" / "a.cpp").write_text(
            "int one() { return 1; }\nint two() { return 2; }\n",
            encoding="utf-8")

    def tearDown(self) -> None:
        self._tmp.cleanup()

    def _src(self) -> str:
        return (self.root / "src" / "a.cpp").read_text(encoding="utf-8")

    def test_simple_replace(self) -> None:
        out = apply_patch(self.root, [FilePatch(
            path="src/a.cpp",
            blocks=[SearchReplace("return 1;", "return 42;")])],
            self.writable)
        self.assertTrue(out.ok, out.error)
        self.assertIn("return 42;", self._src())
        self.assertEqual(out.changed_ranges["src/a.cpp"], [(1, 1)])

    def test_path_outside_writable_rejected(self) -> None:
        out = apply_patch(self.root, [FilePatch(
            path="src/other.cpp",
            blocks=[SearchReplace("x", "y")])], self.writable)
        self.assertFalse(out.ok)
        self.assertIn("writable", out.error)

    def test_dotdot_and_absolute_rejected(self) -> None:
        for bad in ("src/../a.cpp", "/etc/passwd"):
            out = apply_patch(self.root, [FilePatch(
                path=bad, blocks=[SearchReplace("x", "y")])],
                self.writable | {bad})
            self.assertFalse(out.ok, bad)

    def test_search_must_match_exactly_once_zero(self) -> None:
        out = apply_patch(self.root, [FilePatch(
            path="src/a.cpp",
            blocks=[SearchReplace("return 99;", "x")])], self.writable)
        self.assertFalse(out.ok)
        self.assertIn("0 times", out.error)
        self.assertEqual(out.failing, ("src/a.cpp", 0))
        self.assertIn("return 99;", out.error)  # anchor hint
        # disk untouched
        self.assertIn("return 1;", self._src())

    def test_search_must_match_exactly_once_multiple(self) -> None:
        (self.root / "src" / "a.cpp").write_text("int x;\nint x;\n",
                                                 encoding="utf-8")
        out = apply_patch(self.root, [FilePatch(
            path="src/a.cpp", blocks=[SearchReplace("int x;", "int y;")])],
            self.writable)
        self.assertFalse(out.ok)
        self.assertIn("2 times", out.error)

    def test_blocks_apply_in_order(self) -> None:
        out = apply_patch(self.root, [FilePatch(
            path="src/a.cpp",
            blocks=[SearchReplace("return 1;", "return 7;"),
                    SearchReplace("return 7;", "return 8;")])],
            self.writable)
        self.assertTrue(out.ok, out.error)
        self.assertIn("return 8;", self._src())

    def test_all_or_nothing_disk_untouched(self) -> None:
        out = apply_patch(self.root, [FilePatch(
            path="src/a.cpp",
            blocks=[SearchReplace("return 1;", "return 42;"),
                    SearchReplace("no such text", "x")])], self.writable)
        self.assertFalse(out.ok)
        self.assertIn("return 1;", self._src())
        self.assertNotIn("return 42", self._src())

    def test_create_new_file(self) -> None:
        out = apply_patch(self.root, [FilePatch(
            path="include/a.hpp", create=True,
            content="#ifndef A_HPP\n#define A_HPP\n#endif\n")], self.writable)
        self.assertTrue(out.ok, out.error)
        self.assertTrue((self.root / "include" / "a.hpp").exists())
        self.assertEqual(out.changed_ranges["include/a.hpp"], [(1, 3)])

    def test_create_fails_if_exists(self) -> None:
        out = apply_patch(self.root, [FilePatch(
            path="src/a.cpp", create=True, content="x")], self.writable)
        self.assertFalse(out.ok)
        self.assertIn("already exists", out.error)

    def test_create_with_blocks_rejected(self) -> None:
        out = apply_patch(self.root, [FilePatch(
            path="include/a.hpp", create=True, content="x",
            blocks=[SearchReplace("a", "b")])], self.writable)
        self.assertFalse(out.ok)

    def test_missing_file_without_create(self) -> None:
        out = apply_patch(self.root, [FilePatch(
            path="include/a.hpp",
            blocks=[SearchReplace("a", "b")])], self.writable)
        self.assertFalse(out.ok)
        self.assertIn("does not exist", out.error)

    def test_no_blocks_rejected(self) -> None:
        out = apply_patch(self.root, [FilePatch(path="src/a.cpp")],
                          self.writable)
        self.assertFalse(out.ok)

    def test_changed_ranges_multiline_and_merge(self) -> None:
        out = apply_patch(self.root, [FilePatch(
            path="src/a.cpp",
            blocks=[SearchReplace(
                "int one() { return 1; }\nint two() { return 2; }",
                "int one() {\n    return 1;\n}\nint two() { return 2; }")])],
            self.writable)
        self.assertTrue(out.ok, out.error)
        self.assertEqual(out.changed_ranges["src/a.cpp"], [(1, 4)])


if __name__ == "__main__":
    unittest.main()
