from __future__ import annotations

import unittest
from types import SimpleNamespace

from tools.llm_harness.source_regions import (
    SourceRegion,
    apply_strip_redundant_externs_to_regions,
    begin_marker,
    end_marker,
    find_function_region,
    replace_function_source,
    should_keep_extern_c_on_definition,
    strip_redundant_extern_decls,
)


class StripRedundantExternTests(unittest.TestCase):
    def _region(self, source: str, target_id: str, function: str) -> SourceRegion:
        target = SimpleNamespace(id=target_id, function=function)
        return find_function_region(source, target)

    def test_strips_conflicting_extern_when_symbol_already_in_tu(self) -> None:
        """WPAD-style: second slot invents a different type for the same global."""
        source = (
            begin_marker("us-80373460")
            + "\n"
            + "extern void *_wpadUsedCallback;\n"
            + "\n"
            + "void* WPADIsUsedCallbackByKPAD() {\n"
            + "    return _wpadUsedCallback;\n"
            + "}\n"
            + end_marker("us-80373460")
            + "\n"
            + begin_marker("us-80373470")
            + "\n"
            + "void WPADSetCallbackByKPAD(void (*callback)(void)) {}\n"
            + end_marker("us-80373470")
            + "\n"
        )
        region = self._region(source, "us-80373470", "WPADSetCallbackByKPAD")
        candidate = (
            "extern void (*_wpadUsedCallback)(void);\n"
            "\n"
            "void WPADSetCallbackByKPAD(void (*callback)(void))\n"
            "{\n"
            "    _wpadUsedCallback = callback;\n"
            "}\n"
        )
        cleaned = strip_redundant_extern_decls(source, region, candidate)
        self.assertNotIn("extern", cleaned)
        self.assertIn("WPADSetCallbackByKPAD", cleaned)
        self.assertIn("_wpadUsedCallback = callback", cleaned)

        updated = replace_function_source(source, region, candidate)
        self.assertEqual(updated.count("extern void *_wpadUsedCallback"), 1)
        self.assertNotIn("extern void (*_wpadUsedCallback)", updated)

    def test_keeps_extern_when_symbol_absent_elsewhere(self) -> None:
        source = (
            begin_marker("first")
            + "\n"
            + "void* WPADIsUsedCallbackByKPAD() { return 0; }\n"
            + end_marker("first")
            + "\n"
        )
        region = self._region(source, "first", "WPADIsUsedCallbackByKPAD")
        candidate = (
            "extern void *_wpadUsedCallback;\n"
            "\n"
            "void* WPADIsUsedCallbackByKPAD() {\n"
            "    return _wpadUsedCallback;\n"
            "}\n"
        )
        cleaned = strip_redundant_extern_decls(source, region, candidate)
        self.assertIn("extern void *_wpadUsedCallback;", cleaned)

    def test_keeps_extern_c_on_flat_cpp_definition(self) -> None:
        source = (
            begin_marker("fn")
            + "\n"
            + "void func_800407C8(float *out) {}\n"
            + end_marker("fn")
            + "\n"
        )
        region = self._region(source, "fn", "func_800407C8")
        candidate = (
            'extern "C" void func_800407C8(float *out) {\n'
            "    out[0] = 1.0f;\n"
            "}\n"
        )
        cleaned = strip_redundant_extern_decls(
            source,
            region,
            candidate,
            target_function="func_800407C8",
            target_symbol="func_800407C8",
            source_path="src/foo.cpp",
        )
        self.assertIn('extern "C" void func_800407C8', cleaned)

    def test_strips_extern_c_on_mangled_cpp_definition(self) -> None:
        source = (
            begin_marker("fn")
            + "\n"
            + "void CGame::wkRender() {}\n"
            + end_marker("fn")
            + "\n"
        )
        region = self._region(source, "fn", "CGame::wkRender")
        candidate = (
            'extern "C" void CGame::wkRender() {\n'
            "    return;\n"
            "}\n"
        )
        cleaned = strip_redundant_extern_decls(
            source,
            region,
            candidate,
            target_function="CGame::wkRender",
            target_symbol="wkRender__5CGameFv",
            source_path="src/CGame.cpp",
        )
        self.assertNotIn('extern "C"', cleaned)
        self.assertIn("void CGame::wkRender()", cleaned)

    def test_strips_extern_c_on_c_file_definition(self) -> None:
        source = (
            begin_marker("fn")
            + "\n"
            + "void WPADInit(void) {}\n"
            + end_marker("fn")
            + "\n"
        )
        region = self._region(source, "fn", "WPADInit")
        candidate = 'extern "C" void WPADInit(void) {\n}\n'
        cleaned = strip_redundant_extern_decls(
            source,
            region,
            candidate,
            target_function="WPADInit",
            target_symbol="WPADInit",
            source_path="libs/RVL_SDK/src/revolution/wpad/WPAD.c",
        )
        self.assertNotIn('extern "C"', cleaned)
        self.assertIn("void WPADInit(void)", cleaned)

    def test_should_keep_extern_c_policy(self) -> None:
        self.assertTrue(
            should_keep_extern_c_on_definition("func_800407C8", "a.cpp")
        )
        self.assertFalse(
            should_keep_extern_c_on_definition("wkRender__5CGameFv", "a.cpp")
        )
        self.assertFalse(should_keep_extern_c_on_definition("WPADInit", "a.c"))
        self.assertTrue(should_keep_extern_c_on_definition("func_800407C8", None))

    def test_strips_extern_c_data_when_label_already_declared(self) -> None:
        source = (
            'extern "C" const float lbl_eu_80665548[];\n'
            + begin_marker("fn")
            + "\n"
            + "float use_label() { return 0; }\n"
            + end_marker("fn")
            + "\n"
        )
        region = self._region(source, "fn", "use_label")
        candidate = (
            'extern "C" const float lbl_eu_80665548[];\n'
            "float use_label() { return lbl_eu_80665548[0]; }\n"
        )
        cleaned = strip_redundant_extern_decls(source, region, candidate)
        self.assertNotIn("extern", cleaned)
        self.assertIn("lbl_eu_80665548[0]", cleaned)

    def test_ignores_decl_only_inside_replaced_region(self) -> None:
        """Replacing the slot that owns the only decl must keep a fresh extern."""
        source = (
            begin_marker("owner")
            + "\n"
            + "extern void *_wpadUsedCallback;\n"
            + "void* WPADIsUsedCallbackByKPAD() { return _wpadUsedCallback; }\n"
            + end_marker("owner")
            + "\n"
        )
        region = self._region(source, "owner", "WPADIsUsedCallbackByKPAD")
        candidate = (
            "extern void *_wpadUsedCallback;\n"
            "void* WPADIsUsedCallbackByKPAD() { return _wpadUsedCallback; }\n"
        )
        cleaned = strip_redundant_extern_decls(source, region, candidate)
        self.assertIn("extern void *_wpadUsedCallback;", cleaned)

    def test_one_liner_definition_not_eaten_as_prototype(self) -> None:
        """One-liner bodies contain ``;`` — must not match as extern prototypes."""
        source = (
            begin_marker("fn")
            + "\n"
            + 'extern "C" u32 func_8006B6A8(const void* self) { return 1; }\n'
            + end_marker("fn")
            + "\n"
        )
        region = self._region(source, "fn", "func_8006B6A8")
        body = source[region.content_start : region.content_end]
        # Flat C++: keep extern "C", keep the whole definition.
        cleaned = strip_redundant_extern_decls(
            source,
            region,
            body,
            earlier_only=True,
            target_function="func_8006B6A8",
            target_symbol="func_8006B6A8",
            source_path="src/kyoshin/cf/CfCam.cpp",
        )
        self.assertEqual(cleaned.strip(), body.strip())

        # Mangled C++: strip only the extern "C" prefix.
        mangled_body = (
            'extern "C" float GetUpdateRate__Q34nw4r3g3d9AnmScnResCFv(const void* this_) '
            "{ return 1.0f; }\n"
        )
        mangled_source = (
            begin_marker("m")
            + "\n"
            + mangled_body
            + end_marker("m")
            + "\n"
        )
        mangled_region = self._region(
            mangled_source, "m", "GetUpdateRate__Q34nw4r3g3d9AnmScnResCFv"
        )
        cleaned_m = strip_redundant_extern_decls(
            mangled_source,
            mangled_region,
            mangled_body,
            earlier_only=True,
            target_function="GetUpdateRate__Q34nw4r3g3d9AnmScnResCFv",
            target_symbol="GetUpdateRate__Q34nw4r3g3d9AnmScnResCFv",
            source_path="libs/nw4r/src/g3d/g3d_anmscn.cpp",
        )
        self.assertNotIn('extern "C"', cleaned_m)
        self.assertIn("float GetUpdateRate__Q34nw4r3g3d9AnmScnResCFv", cleaned_m)
        self.assertIn("return 1.0f", cleaned_m)

    def test_same_slot_drops_prototype_before_definition(self) -> None:
        source = (
            begin_marker("fn")
            + "\n"
            + "void Foo(int x) {}\n"
            + end_marker("fn")
            + "\n"
        )
        region = self._region(source, "fn", "Foo")
        candidate = (
            "extern void Foo(int x);\n"
            "void Foo(int x) {\n"
            "    return;\n"
            "}\n"
        )
        cleaned = strip_redundant_extern_decls(
            source,
            region,
            candidate,
            target_function="Foo",
            target_symbol="Foo",
            source_path="x.c",
        )
        self.assertNotIn("extern void Foo", cleaned)
        self.assertIn("void Foo(int x)", cleaned)

    def test_cross_slot_drops_helper_prototype_when_defined_earlier(self) -> None:
        source = (
            begin_marker("a")
            + "\n"
            + "void Helper(void) {}\n"
            + end_marker("a")
            + "\n"
            + begin_marker("b")
            + "\n"
            + "void Target(void) {}\n"
            + end_marker("b")
            + "\n"
        )
        region = self._region(source, "b", "Target")
        candidate = (
            "extern void Helper(void);\n"
            "void Target(void) { Helper(); }\n"
        )
        cleaned = strip_redundant_extern_decls(
            source,
            region,
            candidate,
            earlier_only=True,
            target_function="Target",
            target_symbol="Target",
            source_path="x.c",
        )
        self.assertNotIn("extern void Helper", cleaned)
        self.assertIn("Helper();", cleaned)

    def test_earlier_only_cleanup_keeps_first_slot(self) -> None:
        source = (
            begin_marker("us-80373460")
            + "\n"
            + "extern void *_wpadUsedCallback;\n"
            + "void* WPADIsUsedCallbackByKPAD() { return _wpadUsedCallback; }\n"
            + end_marker("us-80373460")
            + "\n"
            + begin_marker("us-80373470")
            + "\n"
            + "extern void (*_wpadUsedCallback)(void);\n"
            + "void WPADSetCallbackByKPAD(void (*callback)(void)) {\n"
            + "    _wpadUsedCallback = callback;\n"
            + "}\n"
            + end_marker("us-80373470")
            + "\n"
        )
        regions = [
            (
                "us-80373460",
                self._region(source, "us-80373460", "WPADIsUsedCallbackByKPAD"),
                "WPADIsUsedCallbackByKPAD",
                "WPADIsUsedCallbackByKPAD",
            ),
            (
                "us-80373470",
                self._region(source, "us-80373470", "WPADSetCallbackByKPAD"),
                "WPADSetCallbackByKPAD",
                "WPADSetCallbackByKPAD",
            ),
        ]
        updated, rows = apply_strip_redundant_externs_to_regions(
            source, regions, earlier_only=True, source_path="WPAD.c"
        )
        by_id = {row["target_id"]: row for row in rows}
        self.assertEqual(by_id["us-80373460"]["action"], "unchanged")
        self.assertEqual(by_id["us-80373470"]["action"], "stripped")
        self.assertEqual(
            by_id["us-80373470"]["removed_externs"],
            ["extern void (*_wpadUsedCallback)(void);"],
        )
        self.assertEqual(updated.count("extern void *_wpadUsedCallback;"), 1)
        self.assertNotIn("extern void (*_wpadUsedCallback)", updated)
        self.assertIn("WPADSetCallbackByKPAD", updated)
        self.assertIn("_wpadUsedCallback = callback", updated)

    def test_any_outside_would_strip_both_duplicate_slots(self) -> None:
        """Apply-time policy: either outside decl is enough to drop a candidate extern."""
        source = (
            begin_marker("a")
            + "\n"
            + "extern void *_foo;\n"
            + "void* get_foo() { return _foo; }\n"
            + end_marker("a")
            + "\n"
            + begin_marker("b")
            + "\n"
            + "extern void (*_foo)(void);\n"
            + "void set_foo(void (*cb)(void)) { _foo = cb; }\n"
            + end_marker("b")
            + "\n"
        )
        region_a = self._region(source, "a", "get_foo")
        body_a = source[region_a.content_start : region_a.content_end]
        cleaned_a = strip_redundant_extern_decls(source, region_a, body_a)
        self.assertNotIn("extern", cleaned_a)


if __name__ == "__main__":
    unittest.main()
