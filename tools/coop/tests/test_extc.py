from __future__ import annotations

import tempfile
import unittest
from pathlib import Path

from tools.coop import extc


class ExtcScannerTest(unittest.TestCase):
    """extern "C" declaration classification + member-plan heuristics."""

    def _scan_lines(self, lines: list[str]) -> list[tuple[str, str, int, str, str]]:
        return list(extc.extract_entries(lines))

    def test_extracts_single_line_decl(self):
        out = self._scan_lines(['extern "C" void func_80123456(void*);'])
        self.assertEqual(len(out), 1)
        name, kind, lineno, raw, _ = out[0]
        self.assertEqual(name, "func_80123456")
        self.assertEqual(kind, "decl")
        self.assertEqual(lineno, 1)

    def test_extracts_data_decl_with_template_args(self):
        # template sizeof(...) must not be picked as the symbol name
        out = self._scan_lines(
            [
                'extern "C" const nw4r::g3d::G3dObj::ResNameDataT'
                '<sizeof("G3dObj")> lbl_eu_8051D640;'
            ]
        )
        self.assertEqual(len(out), 1)
        self.assertEqual(out[0][0], "lbl_eu_8051D640")

    def test_block_decl(self):
        lines = [
            'extern "C" {',
            "void HBMCreate(const void* p);",
            "}",
        ]
        out = self._scan_lines(lines)
        self.assertEqual(len(out), 1)
        self.assertEqual(out[0][0], "HBMCreate")

    def test_block_wrapping_definition_is_not_a_decl(self):
        # `extern "C" { void f() { ... } }` — the inner lines are not decls
        lines = [
            'extern "C" {',
            "void f(void* p) {",
            "    int x;",
            "}",
            "}",
        ]
        out = self._scan_lines(lines)
        self.assertEqual(out, [])

    def test_one_line_definition_is_skipped(self):
        out = self._scan_lines(
            ['extern "C" void f(void* p) { (void)p; }']
        )
        self.assertEqual(out, [])

    def test_attribute_macro_stripped(self):
        out = self._scan_lines(
            ['extern "C" void func_804CD0A4(void* self) DECOMP_DONT_INLINE;']
        )
        self.assertEqual(out[0][0], "func_804CD0A4")

    def test_gnu_attribute_name(self):
        out = self._scan_lines(
            [
                'extern "C" __attribute__((noinline)) void func_8022D1F8'
                "(CExchangeWin* self);"
            ]
        )
        self.assertEqual(out[0][0], "func_8022D1F8")

    def test_member_mangling(self):
        self.assertEqual(
            extc.member_mangled("CExchangeWin", "getField25"),
            "getField25__12CExchangeWinFv",
        )

    def test_classify_missing_name(self):
        names = {"func_800F4A98": "us"}
        cat, resolved = extc.classify("func_80123456", names, {})
        self.assertEqual(cat, "invented")
        self.assertIsNone(resolved)

    def test_classify_prefix_drift(self):
        names = {"func_8008235C__Q22cf13CfGameManagerFv": "us"}
        cat, resolved = extc.classify("func_8008235C", names, {})
        self.assertEqual(cat, "drift")
        self.assertEqual(resolved, "func_8008235C__Q22cf13CfGameManagerFv")

    def test_extern_c_def_bodies_capture_cast(self):
        lines = [
            'extern "C" u8 func_8022D08C(void* self) {',
            "    return ((CExchangeWin*)self)->field_25;",
            "}",
        ]
        defs = list(extc.extern_c_defs_with_bodies(lines))
        self.assertEqual(len(defs), 1)
        name, lineno, header, body = defs[0]
        self.assertEqual(name, "func_8022D08C")
        m = extc.RE_CLASS_CAST.search(body)
        self.assertIsNotNone(m)
        self.assertEqual(m.group(1), "CExchangeWin")


if __name__ == "__main__":
    unittest.main()
