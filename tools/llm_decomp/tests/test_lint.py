"""Tests for tools/llm_decomp/lint.py."""
from __future__ import annotations

import unittest

from tools.llm_decomp.lint import lint_delta

STUB = 'extern "C" u8 func_8022D08C(void* self) { return ((u8*)self)[0x25]; }\n'


def rules(old: str | None, new: str) -> set[str]:
    return {v.rule for v in lint_delta("src/a.cpp", old, new)}


class LintRuleTest(unittest.TestCase):
    def test_no_asm_positive(self) -> None:
        self.assertIn("no_asm", rules("", 'asm void f() { }'))
        self.assertIn("no_asm", rules("", 'x = __asm("nop");'))

    def test_no_asm_negative(self) -> None:
        self.assertNotIn("no_asm", rules("", "int chasm_count = 0;"))
        self.assertNotIn("no_asm", rules("", "// asm would go here"))

    def test_s_include(self) -> None:
        self.assertIn("no_asm", rules("", '#include "foo.s"'))

    def test_register_keyword(self) -> None:
        self.assertIn("no_register_keyword", rules("", "register int x;"))
        self.assertNotIn("no_register_keyword", rules("", "int registry;"))

    def test_register_names(self) -> None:
        self.assertIn("no_register_names", rules("", "u32 r30 = 0;"))
        self.assertNotIn("no_register_names",
                         rules("", "u32 r300 = 0;"))  # not r0-r31
        self.assertNotIn("no_register_names",
                         rules("", 'f("r30 is mentioned in a string");')
                         - {"no_asm"})

    def test_extern_c(self) -> None:
        self.assertIn("no_extern_c",
                      rules("", 'extern "C" void func_8022D08C(void);'))
        self.assertNotIn(
            "no_extern_c",
            rules("", 'extern "C" u8 lbl_8025F1A0[4];'))

    def test_void_ptr(self) -> None:
        self.assertIn("no_void_ptr", rules("", "void f(void* self);"))
        self.assertIn("no_void_ptr", rules("", "x = ((void*)p);"))
        self.assertNotIn("no_void_ptr", rules("", "void f();"))
        self.assertNotIn("no_void_ptr", rules("", "// takes void* self"))

    def test_volatile_fake_stack(self) -> None:
        self.assertIn("no_volatile_fake_stack",
                      rules("", "volatile u8 buf[0x20];"))
        self.assertIn("no_volatile_fake_stack",
                      rules("", "u8 sp[0x1C];"))
        self.assertNotIn("no_volatile_fake_stack",
                         rules("", "u8 sprite_count[4];"))

    def test_offset_arithmetic(self) -> None:
        self.assertIn("no_offset_arithmetic",
                      rules("", "v = *(u32*)((u8*)self + 0x25);"))
        self.assertNotIn("no_offset_arithmetic",
                         rules("", "v = self->field_0x25;"))

    def test_codegen_macros(self) -> None:
        self.assertIn("no_codegen_macros",
                      rules("", "x = DECOMP_PPC_RLWINM(v, 1, 0, 30);"))
        self.assertIn("no_codegen_macros",
                      rules("", "DECOMP_FORCEACTIVE(mod, sym);"))

    def test_pragmas(self) -> None:
        self.assertIn("no_pragmas", rules("", "#pragma peephole off"))
        self.assertNotIn("no_pragmas", rules("", "int pragma_like;"))

    def test_if0(self) -> None:
        self.assertIn("no_if0", rules("", "#if 0"))
        self.assertNotIn("no_if0", rules("", "#if 1"))

    def test_section_attr(self) -> None:
        self.assertIn("no_section_attr",
                      rules("", '__declspec(section ".init") void f();'))

    def test_angle_include(self) -> None:
        self.assertIn("no_angle_include",
                      rules("", "#include <boost/x.hpp>"))
        self.assertNotIn("no_angle_include",
                         rules("", "#include <string.h>"))
        self.assertNotIn("no_angle_include",
                         rules("", '#include "kyoshin/cf/CfPadTask.hpp"'))

    def test_delta_only_old_stub_not_flagged(self) -> None:
        # The stub violates several rules, but it is unchanged context.
        new = STUB + "int added() { return 1; }\n"
        self.assertEqual(rules(STUB, new), set())

    def test_created_header_guards_clean(self) -> None:
        header = ("#ifndef A_HPP\n#define A_HPP\n\n"
                  "struct Foo { u32 field_0x00; };\n\n#endif\n")
        self.assertEqual(rules(None, header), set())

    def test_line_numbers_point_into_new_text(self) -> None:
        old = "int a;\nint b;\n"
        new = "int a;\nregister int r;\nint b;\n"
        vs = lint_delta("src/a.cpp", old, new)
        reg = [v for v in vs if v.rule == "no_register_keyword"]
        self.assertTrue(reg)
        self.assertEqual(reg[0].line, 2)


if __name__ == "__main__":
    unittest.main()
