from __future__ import annotations

import unittest

from tools.llm_harness.candidate_sanitize import (
    extract_signature_prefix,
    normalize_source_for_compare,
    sanitize_candidate_source,
    scaffold_from_body,
)
from tools.llm_harness.compile_diagnostic import (
    clean_mwcc_diagnostics,
    normalize_compile_output,
    select_root_diagnostic,
)
from tools.llm_harness.core import parse_candidate


class CandidateSanitizeTests(unittest.TestCase):
    def test_rewrites_this_param_and_flat_mangled_call(self) -> None:
        src = (
            "void Foo(CView* this) {\n"
            "    func_8049B3FC__Fv();\n"
            "}\n"
        )
        out = sanitize_candidate_source(src)
        self.assertIn("this_", out)
        self.assertNotIn("CView* this)", out)
        self.assertIn("func_8049B3FC();", out)
        self.assertNotIn("func_8049B3FC__Fv", out)

    def test_scaffolds_body_only(self) -> None:
        scaffold = "extern \"C\" void pluginUnitRegist() {\n    return;\n}\n"
        body = "{\n    vmPluginRegist(a, b);\n}"
        out = sanitize_candidate_source(body, scaffold_from=scaffold)
        self.assertTrue(out.startswith('extern "C" void pluginUnitRegist()'))
        self.assertIn("vmPluginRegist", out)

    def test_local_mode_strips_leading_proto(self) -> None:
        src = (
            "extern \"C\" void helper(void);\n"
            "extern \"C\" void target(void) {\n"
            "    helper();\n"
            "}\n"
        )
        out = sanitize_candidate_source(src, local_mode=True)
        self.assertNotIn("helper(void);", out)
        self.assertIn("void target(void)", out)

    def test_extract_signature_and_scaffold(self) -> None:
        defn = "int Foo::bar(int x) const {\n  return x;\n}\n"
        sig = extract_signature_prefix(defn)
        self.assertEqual(sig, "int Foo::bar(int x) const")
        wrapped = scaffold_from_body(sig or "", "{ return x + 1; }")
        self.assertIn("return x + 1", wrapped)

    def test_normalize_compare(self) -> None:
        a = "void f() {\n  return;\n}"
        b = "void f() { return; }"
        self.assertEqual(normalize_source_for_compare(a), normalize_source_for_compare(b))

    def test_parse_accepts_body_only(self) -> None:
        cand = parse_candidate(
            '{"source":"{ return 1; }","hypothesis":"h","notes":[],'
            '"next_change":"","change":""}'
        )
        self.assertTrue(cand.source.strip().startswith("{"))


class MwccDiagnosticTests(unittest.TestCase):
    def test_parses_mwcc_undefined_identifier(self) -> None:
        raw = """
[1/1] MWCC build/us/src/foo.o
### mwcceppc.exe Compiler:
#    File: libs\\monolib\\src\\scn\\CScn.cpp
# --------------------------------------
#     259:     func_8049B3FC(); 
#   Error:                   ^
#   (10140) undefined identifier 'func_8049B3FC'
#   Too many errors printed, aborting program
"""
        diags = normalize_compile_output(raw)
        self.assertTrue(diags)
        root = select_root_diagnostic(diags)
        assert root is not None
        self.assertEqual(root.category, "unknown_identifier")
        self.assertEqual(root.symbol, "func_8049B3FC")
        cleaned = clean_mwcc_diagnostics(raw)
        self.assertIn("undefined identifier", cleaned)
        self.assertNotIn("ninja: build stopped", cleaned)


if __name__ == "__main__":
    unittest.main()
