"""Tests for FUNCTION-symbol renames (tools/coop/symbol_fn_rename.py).

Covers: MWCC mangling against known-good retail symbols, target-spec
parsing, plan generation, apply (on temporary fixtures only), and safe
refusals / no-op cases.
"""

from __future__ import annotations

import unittest
from pathlib import Path
from tempfile import TemporaryDirectory

from tools.coop.symbol_fn_rename import (
    FnRenameRefusal,
    apply_function_rename,
    encode_class,
    encode_type,
    looks_like_function_rename,
    mangle_member_function,
    parse_function_target,
    plan_function_rename,
)
from tools.symbolrecover.lib.mwcc import demangle_symbol

# (class, method, params) -> known-good retail symbol from config/us/symbols.txt
KNOWN_GOOD: list[tuple[str, str, list[str], str]] = [
    ("CMMTex", "OnFileEvent", ["CEventFile*"], "OnFileEvent__6CMMTexFP10CEventFile"),
    ("CCur07", "initLayout", [], "initLayout__6CCur07Fv"),
    ("CGame", "setViewRect", ["CView*", "short", "short", "short", "short"],
     "setViewRect__5CGameFP5CViewssss"),
    ("CSLCur", "createLayout", [], "createLayout__6CSLCurFv"),
    ("CToken", "clear", [], "clear__6CTokenFv"),
    ("CToken", "findTokenName", ["char*", "int", "const char*"],
     "findTokenName__6CTokenFPciPCc"),
    ("CTaskGame", "OnFileEvent", ["CEventFile*"], "OnFileEvent__9CTaskGameFP10CEventFile"),
    ("COccCulling", "isOccluded", ["const ml::CVec3&", "float"],
     "isOccluded__11COccCullingFRCQ22ml5CVec3f"),
    # NOTE: the retail method name literally carries a CfObject_ prefix.
    ("cf::CfObject", "CfObject_setMoveHeadAngle", ["float"],
     "CfObject_setMoveHeadAngle__Q22cf8CfObjectFf"),
    ("IWorkEvent", "WorkEvent1", ["void*", "const char*"],
     "WorkEvent1__10IWorkEventFPvPCc"),
]


class ManglingTest(unittest.TestCase):
    def test_known_good_symbols(self) -> None:
        for cls, method, params, expected in KNOWN_GOOD:
            with self.subTest(symbol=expected):
                self.assertEqual(mangle_member_function(cls, method, params), expected)

    def test_round_trip_demangle(self) -> None:
        for cls, method, params, expected in KNOWN_GOOD:
            with self.subTest(symbol=expected):
                mangled = mangle_member_function(cls, method, params)
                info = demangle_symbol(mangled)
                self.assertEqual(info.function, method)
                # Class may carry a namespace (cf::CfObject) or not.
                leaf = cls.split("::")[-1]
                self.assertEqual(info.class_name, leaf)

    def test_brief_example_u32(self) -> None:
        # Task brief: void SLList::DropItem(u32) -> DropItem__6SLListFUi.
        self.assertEqual(mangle_member_function("SLList", "DropItem", ["u32"]),
                         "DropItem__6SLListFUi")
        self.assertEqual(mangle_member_function("SLList", "DropItem", ["unsigned int"]),
                         "DropItem__6SLListFUi")

    def test_demo_symbol(self) -> None:
        self.assertEqual(mangle_member_function("SLList", "DropItem", ["CSchedule*"]),
                         "DropItem__6SLListFP9CSchedule")

    def test_dtor(self) -> None:
        self.assertEqual(mangle_member_function("CToken", "~CToken", []), "__dt__6CTokenFv")

    def test_encode_primitives_and_typedefs(self) -> None:
        cases = {
            "void*": "Pv", "bool": "b", "char": "c", "u8": "Uc",
            "short": "s", "u16": "Us", "int": "i", "u32": "Ui",
            "long": "l", "unsigned long": "Ul", "long long": "x",
            "unsigned long long": "Ux", "float": "f", "f32": "f",
            "double": "d", "s16": "s", "s32": "i", "s64": "x", "u64": "Ux",
            "unsigned": "Ui", "unsigned int": "Ui", "unsigned short": "Us",
            "char*": "Pc", "const char*": "PCc", "char const*": "PCc",
            "CSchedule*": "P9CSchedule", "CSchedule**": "PP9CSchedule",
            "const CVec3&": "RC5CVec3", "CVec3&": "R5CVec3",
        }
        for spelling, code in cases.items():
            with self.subTest(spelling=spelling):
                self.assertEqual(encode_type(spelling), code)

    def test_encode_class(self) -> None:
        self.assertEqual(encode_class("SLList"), "6SLList")
        self.assertEqual(encode_class("cf::CfObject"), "Q22cf8CfObject")
        self.assertEqual(encode_class("ml::FixStr<32>"), "Q22ml10FixStr<32>")
        self.assertEqual(encode_class("nw4r::lyt::Layout"), "Q34nw4r3lyt6Layout")

    def test_encode_refusals(self) -> None:
        for bad in ["...", "volatile int", "int[8]", "void (*)()", "", "unsigned foo_t",
                    "int&*"]:
            with self.subTest(bad=bad):
                with self.assertRaises(FnRenameRefusal):
                    encode_type(bad)

    def test_parse_target(self) -> None:
        t = parse_function_target("SLList::DropItem(CSchedule*)")
        self.assertEqual((t.class_name, t.method, t.params), ("SLList", "DropItem", ["CSchedule*"]))
        t = parse_function_target("void SLList::DropItem(CSchedule* x)")
        self.assertEqual((t.class_name, t.method), ("SLList", "DropItem"))
        t = parse_function_target("SLList::DropItem")
        self.assertIsNone(t.params)
        t = parse_function_target("SLList::Run()")
        self.assertEqual(t.params, [])
        t = parse_function_target("SLList::Run(void)")
        self.assertEqual(t.params, [])
        with self.assertRaises(FnRenameRefusal):
            parse_function_target("DropItem")
        with self.assertRaises(FnRenameRefusal):
            parse_function_target("SLList::")


def _write_fixture_tree(root: Path, *, configure_hit: bool = False) -> None:
    (root / "config" / "us").mkdir(parents=True, exist_ok=True)
    (root / "config" / "eu").mkdir(parents=True, exist_ok=True)
    (root / "src" / "unit").mkdir(parents=True, exist_ok=True)
    (root / "src" / "other").mkdir(parents=True, exist_ok=True)
    symbols_us = (
        "# fixture\n"
        "Widget_Paint = .text:0x80001000; // type:function size:0x40 scope:global align:4\n"
        "Other = .text:0x80002000; // type:function size:0x10 scope:global align:4\n"
        "Widget_Painter = .text:0x80003000; // type:function size:0x10 scope:global align:4\n"
    )
    symbols_eu = (
        "Widget_Paint = .text:0x80001000; // type:function size:0x40\n"
    )
    (root / "config" / "us" / "symbols.txt").write_text(symbols_us, encoding="utf-8")
    (root / "config" / "eu" / "symbols.txt").write_text(symbols_eu, encoding="utf-8")
    (root / "src" / "unit" / "Widget.hpp").write_text(
        '#pragma once\nextern "C" void Widget_Paint(WdgState* self, int x, int y);\n',
        encoding="utf-8",
    )
    (root / "src" / "unit" / "Widget.cpp").write_text(
        '#include "Widget.hpp"\n'
        'extern "C" void Widget_Paint(WdgState* self, int x, int y);\n'
        "extern \"C\" unsigned int Widget_vtable[2] = {\n"
        "    (u32)&Widget_Paint, 0,\n"
        "};\n"
        "// Widget_Paint renders the widget.\n"
        "void Widget_Paint(WdgState* self, int x, int y) {\n"
        "    self->pixels[x] = y;\n"
        "}\n",
        encoding="utf-8",
    )
    (root / "src" / "other" / "Caller.cpp").write_text(
        '#include \"Widget.hpp\"\n'
        "void run(WdgState* st) {\n"
        "    Widget_Paint(st, 1, 2);\n"
        "    Widget_Paint(getState(), 3, 4);\n"
        "}\n",
        encoding="utf-8",
    )
    (root / "src" / "unit" / "Widget.ctx.c").write_text(
        "void Widget_Paint(WdgState* self, int x, int y) {}\n", encoding="utf-8"
    )
    cfg = "// configure stub\n"
    if configure_hit:
        cfg += "USE_WIDGET_PAINT = 1  # Widget_Paint\n"
    (root / "configure.py").write_text(cfg, encoding="utf-8")
    (root / "config" / "us" / "splits.txt").write_text("src/unit/Widget.cpp:\n", encoding="utf-8")


class PlanTest(unittest.TestCase):
    def setUp(self) -> None:
        self.tmp = TemporaryDirectory()
        self.root = Path(self.tmp.name)
        _write_fixture_tree(self.root)

    def tearDown(self) -> None:
        self.tmp.cleanup()

    def test_plan_contents(self) -> None:
        plan = plan_function_rename(self.root, "Widget_Paint", "WdgState::Paint(int, int)")
        self.assertEqual(plan.new_mangled, "Paint__8WdgStateFii")
        self.assertEqual(len(plan.symbol_edits), 2)
        regions = sorted(e.region for e in plan.symbol_edits)
        self.assertEqual(regions, ["eu", "us"])
        # Only the name part changes; address/size suffix preserved.
        for edit in plan.symbol_edits:
            self.assertTrue(edit.new_line.startswith("Paint__8WdgStateFii = "),
                            msg=edit.new_line)
            self.assertIn(edit.old_line.split("=", 1)[1], edit.new_line)
        # Exact-name matching: Widget_Painter untouched.
        self.assertNotIn("Widget_Painter", "".join(e.new_line for e in plan.symbol_edits))
        self.assertEqual(len(plan.defns), 1)
        self.assertEqual(len(plan.decls), 2)
        self.assertEqual(len(plan.calls), 2)
        self.assertEqual(len(plan.addr_refs), 1)
        self.assertEqual(plan.reviews, [])
        self.assertEqual(plan.body_self_count, 1)
        self.assertEqual(plan.configure_hits, [])
        self.assertEqual(plan.splits_hits, [])
        self.assertEqual(plan.ctx_files, ["src/unit/Widget.ctx.c"])

    def test_plan_rewrite_spans(self) -> None:
        plan = plan_function_rename(self.root, "Widget_Paint", "WdgState::Paint(int, int)")
        defn = plan.defns[0]
        self.assertEqual(defn.old_span, "void Widget_Paint(WdgState* self, int x, int y)")
        self.assertEqual(defn.new_span, "void WdgState::Paint(int x, int y)")
        decl_srcs = sorted(d.old_span for d in plan.decls)
        self.assertIn('extern "C" void Widget_Paint(WdgState* self, int x, int y)', decl_srcs)
        for d in plan.decls:
            self.assertNotIn('extern "C"', d.new_span)
            self.assertIn("WdgState::Paint(int x, int y)", d.new_span)
        call_news = sorted(c.new_span for c in plan.calls)
        self.assertEqual(call_news, ["(getState())->Paint(3, 4)", "st->Paint(1, 2)"])
        self.assertEqual(plan.addr_refs[0].new_span, "&WdgState::Paint")

    def test_plan_derives_params(self) -> None:
        plan = plan_function_rename(self.root, "Widget_Paint", "WdgState::Paint")
        self.assertTrue(plan.derived_params)
        self.assertEqual(plan.new_mangled, "Paint__8WdgStateFii")
        self.assertEqual(plan.target.params, ["int", "int"])

    def test_plan_param_mismatch_refuses(self) -> None:
        with self.assertRaises(FnRenameRefusal):
            plan_function_rename(self.root, "Widget_Paint", "WdgState::Paint(int)")

    def test_plan_self_mismatch_refuses(self) -> None:
        with self.assertRaises(FnRenameRefusal):
            plan_function_rename(self.root, "Widget_Paint", "Other::Paint(int, int)")

    def test_plan_unknown_symbol_refuses(self) -> None:
        with self.assertRaises(FnRenameRefusal):
            plan_function_rename(self.root, "Nope_Nothing", "WdgState::Paint(int, int)")

    def test_plan_placeholder_refuses(self) -> None:
        with self.assertRaises(FnRenameRefusal):
            plan_function_rename(self.root, "UnkClass_8045F564", "WdgState::Paint(int, int)")

    def test_plan_noop_refuses(self) -> None:
        with self.assertRaises(FnRenameRefusal):
            plan_function_rename(self.root, "Widget_Paint", "Widget_Paint")

    def test_free_rename(self) -> None:
        plan = plan_function_rename(self.root, "Widget_Paint", "Widget_Draw")
        self.assertTrue(plan.free_rename)
        self.assertEqual(plan.new_mangled, "Widget_Draw")
        # No signature change: decl keeps extern "C" and self.
        for edit in plan.symbol_edits:
            self.assertTrue(edit.new_line.startswith("Widget_Draw = "))

    def test_looks_like_function_rename(self) -> None:
        self.assertFalse(looks_like_function_rename(self.root, "UnkClass_8045F564", "Foo"))
        self.assertTrue(looks_like_function_rename(self.root, "Widget_Paint", "WdgState::Paint(int)"))
        self.assertTrue(looks_like_function_rename(self.root, "Widget_Paint", "Widget_Draw"))
        self.assertFalse(looks_like_function_rename(self.root, "Totally_Unknown", "Other"))


class ApplyTest(unittest.TestCase):
    def setUp(self) -> None:
        self.tmp = TemporaryDirectory()
        self.root = Path(self.tmp.name)
        _write_fixture_tree(self.root)

    def tearDown(self) -> None:
        self.tmp.cleanup()

    def test_dry_run_writes_nothing(self) -> None:
        before = {p: p.read_bytes() for p in self.root.rglob("*") if p.is_file()}
        plan, _log = apply_function_rename(
            self.root, "Widget_Paint", "WdgState::Paint(int, int)",
            regions=["us", "eu"], dry_run=True, allow_review=True,
        )
        after = {p: p.read_bytes() for p in self.root.rglob("*") if p.is_file()}
        self.assertEqual(before, after)
        self.assertEqual(plan.new_mangled, "Paint__8WdgStateFii")

    def test_apply_needs_allow_review(self) -> None:
        with self.assertRaises(FnRenameRefusal):
            apply_function_rename(
                self.root, "Widget_Paint", "WdgState::Paint(int, int)",
                regions=["us"], dry_run=False, allow_review=False,
            )
        # Refusal left the tree untouched.
        self.assertIn("Widget_Paint = .text",
                      (self.root / "config" / "us" / "symbols.txt").read_text(encoding="utf-8"))

    def test_apply_rewrites_everything(self) -> None:
        plan, log = apply_function_rename(
            self.root, "Widget_Paint", "WdgState::Paint(int, int)",
            regions=["us", "eu"], dry_run=False, allow_review=True,
        )
        self.assertEqual(plan.new_mangled, "Paint__8WdgStateFii")
        widget = (self.root / "src" / "unit" / "Widget.cpp").read_text(encoding="utf-8")
        self.assertIn("void WdgState::Paint(int x, int y);", widget)
        self.assertIn("void WdgState::Paint(int x, int y) {", widget)
        self.assertIn("(u32)&WdgState::Paint, 0,", widget)
        self.assertIn("this->pixels[x] = y;", widget)
        self.assertNotIn("Widget_Paint(", widget.replace("Widget_Painter", ""))
        self.assertNotIn('extern "C" void WdgState::Paint', widget)
        # Comment lines are left alone.
        self.assertIn("// Widget_Paint renders the widget.", widget)
        header = (self.root / "src" / "unit" / "Widget.hpp").read_text(encoding="utf-8")
        self.assertIn("void WdgState::Paint(int x, int y);", header)
        caller = (self.root / "src" / "other" / "Caller.cpp").read_text(encoding="utf-8")
        self.assertIn("st->Paint(1, 2);", caller)
        self.assertIn("(getState())->Paint(3, 4);", caller)
        for region in ("us", "eu"):
            symbols = (self.root / "config" / region / "symbols.txt").read_text(encoding="utf-8")
            self.assertIn("Paint__8WdgStateFii = .text:0x80001000;", symbols)
            self.assertNotIn("Widget_Paint =", symbols.replace("Paint__8WdgStateFii", ""))
        us_symbols = (self.root / "config" / "us" / "symbols.txt").read_text(encoding="utf-8")
        self.assertIn("Widget_Painter = .text:0x80003000;", us_symbols)  # untouched
        # .ctx.c regenerable dump untouched.
        ctx = (self.root / "src" / "unit" / "Widget.ctx.c").read_text(encoding="utf-8")
        self.assertIn("Widget_Paint", ctx)
        # Backups exist under /tmp.
        self.assertTrue(any("symbol_fn_rename" in line for line in log))

    def test_apply_blocked_by_configure_hit(self) -> None:
        _write_fixture_tree(self.root, configure_hit=True)
        # Dry-run still works (shows the plan + a note).
        _plan, log = apply_function_rename(
            self.root, "Widget_Paint", "WdgState::Paint(int, int)",
            regions=["us"], dry_run=True, allow_review=True,
        )
        self.assertTrue(any("configure" in line for line in log))
        with self.assertRaises(FnRenameRefusal):
            apply_function_rename(
                self.root, "Widget_Paint", "WdgState::Paint(int, int)",
                regions=["us"], dry_run=False, allow_review=True,
            )

    def test_body_shadow_refuses(self) -> None:
        (self.root / "src" / "unit" / "Widget.cpp").write_text(
            '#include "Widget.hpp"\n'
            'extern "C" void Widget_Paint(WdgState* self, int x, int y);\n'
            "void Widget_Paint(WdgState* self, int x, int y) {\n"
            "    int self = x;\n"
            "    (void)self;\n"
            "}\n",
            encoding="utf-8",
        )
        (self.root / "src" / "unit" / "Widget.hpp").write_text("// empty\n", encoding="utf-8")
        (self.root / "src" / "other" / "Caller.cpp").write_text("// empty\n", encoding="utf-8")
        with self.assertRaises(FnRenameRefusal):
            plan_function_rename(self.root, "Widget_Paint", "WdgState::Paint(int, int)")

    def test_apply_missing_definition_refuses(self) -> None:
        (self.root / "src" / "unit" / "Widget.cpp").write_text(
            "// no definition here\nvoid something_else() {}\n", encoding="utf-8")
        (self.root / "src" / "unit" / "Widget.hpp").write_text("// empty\n", encoding="utf-8")
        (self.root / "src" / "other" / "Caller.cpp").write_text("// empty\n", encoding="utf-8")
        with self.assertRaises(FnRenameRefusal):
            apply_function_rename(
                self.root, "Widget_Paint", "WdgState::Paint(int, int)",
                regions=["us"], dry_run=False, allow_review=True,
            )


if __name__ == "__main__":
    unittest.main()
