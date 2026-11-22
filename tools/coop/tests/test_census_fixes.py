from __future__ import annotations

import unittest

from tools.coop import census_fixes as cfx


class ArityTests(unittest.TestCase):
    def test_arity(self):
        self.assertEqual(cfx._arity(""), 0)
        self.assertEqual(cfx._arity("void"), 0)
        self.assertEqual(cfx._arity("int a, Foo<int, char>* b, void (*cb)(int, int)"), 3)
        self.assertIsNone(cfx._arity("const char* fmt, ..."))

    def test_decl_regex_finds_free_function_decls(self):
        text = "void scnLgtRemoveLgtItem(CScnEnvLgtCtrlResList* pData);\nreturn foo(a);\n"
        names = [m.group("name") for m in cfx._DECL.finditer(text) if m.group("ret") not in cfx._KEYWORDS]
        self.assertEqual(names, ["scnLgtRemoveLgtItem"])


if __name__ == "__main__":
    unittest.main()
