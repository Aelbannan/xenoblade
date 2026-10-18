"""Regression tests for detect_pointer_arithmetic regex coverage.

These pin the two bugs fixed in the 2026-10-12 metric rework, both of which made
the curated detector blind to the *dominant* raw-offset form:

1. a statement beginning with `*` (i.e. every `*(T*)(base + N) = ...`) was
   skipped as if it were a block-comment continuation line;
2. `self->f + N` matched with `-` from the arrow consumed as the arithmetic
   operator, yielding base `self` / offset `>f + N`.

They also pin that the byte-cast base may be a call/field chain and that a
comma must not let one cast expression swallow a later `+`.
"""

from __future__ import annotations

import pathlib
import sys
import tempfile
import unittest

ROOT = pathlib.Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT))

from tools.coop.detect_pointer_arithmetic import CATEGORIES, scan_file  # noqa: E402


def _scan(source: str):
    with tempfile.TemporaryDirectory() as td:
        p = pathlib.Path(td) / "t.cpp"
        p.write_text(source)
        return scan_file("t.cpp", str(p), {c: True for c in CATEGORIES})


def _cats(source: str):
    return [f["category"] for f in _scan(source)]


class TestCoverage(unittest.TestCase):
    def test_deref_statement_not_treated_as_comment(self):
        fs = _scan("void f(u32* p) {\n    *(u32*)((u8*)p + 0x10) = 1;\n}\n")
        self.assertEqual([f["category"] for f in fs], ["cast_byte_offset_deref"])
        self.assertEqual(fs[0]["base_expr"], "p")
        self.assertEqual(fs[0]["offset"], "0x10")

    def test_block_comment_continuation_still_skipped(self):
        self.assertEqual(_cats("/*\n * *(u32*)(p + 0x10)\n */\n"), [])

    def test_arrow_is_not_the_operator(self):
        fs = _scan("void g(void) {\n    *(u32*)(self->f + 4) = 1;\n}\n")
        self.assertEqual([f["category"] for f in fs], ["ptr_offset_deref"])
        self.assertEqual(fs[0]["variable"], "self->f")
        self.assertEqual(fs[0]["offset"], "4")

    def test_reinterpret_arrow_is_not_arithmetic(self):
        # `->` after a reinterpret_cast<u8*>(...) is member access, not a
        # subtraction: it must not be counted as reinterpret byte arithmetic.
        self.assertEqual(
            _cats("void g(void) {\n    reinterpret_cast<u8*>(&m)->field = 1;\n}\n"),
            [],
        )
        self.assertEqual(
            _cats("void g(void) {\n    u32* q = reinterpret_cast<u32*>(&m)->field;\n}\n"),
            [],
        )
        # ...while a real reinterpret addition is still detected.
        self.assertEqual(
            _cats("void g(void) {\n    u32* q = reinterpret_cast<u32*>(reinterpret_cast<u8*>(p) + 0x10);\n}\n"),
            ["reinterpret_arith"],
        )

    def test_byte_pointer_arith_with_call_base(self):
        fs = _scan("void g(void) {\n    u8* r = (u8*)GetCharEntry(self->f) + 0x3534;\n}\n")
        self.assertEqual([f["category"] for f in fs], ["cast_byte_ptr_arith"])
        self.assertEqual(fs[0]["base_expr"], "GetCharEntry(self->f)")

    def test_comma_does_not_swallow_later_plus(self):
        self.assertEqual(_cats("void g(void) {\n    foo((u8*)x), y + 1;\n}\n"), [])

    def test_plain_parenthesized_and_unparenthesized_forms(self):
        self.assertEqual(
            _cats("void g(void) {\n    *(u32*)(p + 0x10) = 1;\n    *(u32*)(((u8*)q) + 0x20) = 2;\n}\n"),
            ["ptr_offset_deref", "cast_byte_ptr_arith"],
        )


if __name__ == "__main__":
    unittest.main()
