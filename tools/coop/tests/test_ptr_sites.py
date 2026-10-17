"""Tests for the curated pointer-arithmetic site attribution (ptr_sites.py).

Companion to the line-based smell metrics: ptr_sites counts individual
expressions and splits each TU by whether the enclosing function is accepted.
These tests pin the (heuristic) definition-span parser and the accepted/unmatched
classification, which is what the "actionable pointer arithmetic" report section
and the ptr-acc metric depend on.
"""

from __future__ import annotations

import json
import pathlib
import sys
import tempfile
import unittest

ROOT = pathlib.Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT))

from tools.coop import ptr_sites  # noqa: E402


SPAN_SRC = [
    "// leading comment",
    '#include "x.hpp"',
    "",
    "extern \"C\" void c_func(u32* p) {",
    "    *(u32*)((u8*)p + 0x10) = 1;",
    "}",
    "",
    "void Klass::Method(int a,",
    "                  int b) {",
    "    *(u32*)((u8*)p + 0x20) = 2;",
    "}",
    "",
    "static void tail(void) {",
    "}",
]


class TestFunctionSpans(unittest.TestCase):
    def test_spans_and_enclosing(self):
        spans = ptr_sites.function_spans(SPAN_SRC)
        names = [n for _, _, n in spans]
        self.assertEqual(names, ["c_func", "Klass::Method", "tail"])
        self.assertEqual(ptr_sites.enclosing_name(spans, 5), "c_func")
        self.assertEqual(ptr_sites.enclosing_name(spans, 10), "Klass::Method")
        self.assertEqual(ptr_sites.enclosing_name(spans, 14), "tail")
        # a site before the first definition (e.g. in a global initializer) is unattributed
        self.assertIsNone(ptr_sites.enclosing_name(spans, 2))

    def test_declarations_are_not_spans(self):
        spans = ptr_sites.function_spans(["void proto(u32* p);", "void real(void) {", "}"])
        self.assertEqual([n for _, _, n in spans], ["real"])


class TestAcceptedKeys(unittest.TestCase):
    def test_keys_and_filtering(self):
        data = {
            "targets": [
                {"status": "FULL_MATCH", "source": "src/a.cpp", "function": "A::Init", "symbol": "Init__1AFv"},
                {"status": "EQUIVALENT_MATCH", "source": "src/b.cpp", "function": "func_8000", "symbol": "func_8000"},
                {"status": "HIGH_MATCH", "source": "src/c.cpp", "function": "C::Nope", "symbol": "Nope__1CFv"},
            ]
        }
        with tempfile.TemporaryDirectory() as td:
            p = pathlib.Path(td) / "targets.json"
            p.write_text(json.dumps(data))
            keys = ptr_sites.accepted_keys(p)
        self.assertIn(("src/a.cpp", "A::Init"), keys)  # qualified function
        self.assertIn(("src/b.cpp", "func_8000"), keys)
        self.assertNotIn(("src/c.cpp", "C::Nope"), keys)  # not accepted

    def test_member_name_collision_is_class_aware(self):
        """A TU-local method whose name merely collides with another class's
        accepted method must not count as accepted (the old bare-last-name
        lookup did: `CUICfManager::Move` vs `CTTask<12CUICfManager>::Move`)."""
        data = {
            "targets": [
                {"status": "FULL_MATCH", "source": "x.cpp",
                 "function": "CTTask<12CUICfManager>::Move()",
                 "symbol": "Move__22CTTask<12CUICfManager>Fv"},
                {"status": "HIGH_MATCH", "source": "x.cpp",
                 "function": "CUICfManager::Move()",
                 "symbol": "Move__12CUICfManagerFv"},
                {"status": "FULL_MATCH", "source": "x.cpp",
                 "function": "CUICfManager::Init()",
                 "symbol": "Init__12CUICfManagerFv"},
            ]
        }
        with tempfile.TemporaryDirectory() as td:
            p = pathlib.Path(td) / "targets.json"
            p.write_text(json.dumps(data))
            idx = ptr_sites.accepted_index(p)
        self.assertTrue(idx.matches("x.cpp", "CTTask<12CUICfManager>::Move"))
        self.assertFalse(idx.matches("x.cpp", "CUICfManager::Move"))
        self.assertTrue(idx.matches("x.cpp", "CUICfManager::Init"))
        # unqualified def (free function) still matches by last component
        self.assertTrue(idx.matches("x.cpp", "Init"))


class TestScanFileSites(unittest.TestCase):
    def _write(self, td: str, body: str) -> pathlib.Path:
        p = pathlib.Path(td) / "x.cpp"
        p.write_text(body)
        return p

    def test_accepted_split(self):
        body = (
            "void f(u32* p) {\n"
            "    *(u32*)((u8*)p + 0x10) = 1;\n"
            "}\n"
            "\n"
            "void g(u32* p) {\n"
            "    *(u32*)((u8*)p + 0x20) = 2;\n"
            "}\n"
        )
        with tempfile.TemporaryDirectory() as td:
            p = self._write(td, body)
            st = ptr_sites.scan_file_sites("x.cpp", p, accepted={("x.cpp", "f")})
            self.assertIsNotNone(st)
            self.assertGreaterEqual(st.total, 2)
            self.assertEqual(st.accepted, 1)
            self.assertEqual(sum(st.by_category_accepted.values()), 1)

            st_none = ptr_sites.scan_file_sites("x.cpp", p, accepted=set())
            self.assertEqual(st_none.accepted, 0)
            self.assertGreaterEqual(st_none.total, 2)

    def test_non_cpp_skipped(self):
        with tempfile.TemporaryDirectory() as td:
            p = self._write(td, "void f(void){ *(u32*)((u8*)0 + 4) = 1; }\n")
            self.assertIsNone(ptr_sites.scan_file_sites("x.hpp", p, accepted=set()))

    def test_clean_file_is_none(self):
        with tempfile.TemporaryDirectory() as td:
            p = self._write(td, "int f(void) { return 0; }\n")
            self.assertIsNone(ptr_sites.scan_file_sites("x.cpp", p, accepted=set()))


if __name__ == "__main__":
    unittest.main()
