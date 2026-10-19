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
    _split_top_level,
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
        # u32 is unsigned long under MWCC -> Ul (retail: FUl for u32
        # params); unsigned int stays Ui.  Distinct spellings, distinct
        # encodings.
        self.assertEqual(mangle_member_function("SLList", "DropItem", ["u32"]),
                         "DropItem__6SLListFUl")
        self.assertEqual(mangle_member_function("SLList", "DropItem", ["unsigned int"]),
                         "DropItem__6SLListFUi")
        self.assertEqual(mangle_member_function("SLList", "DropItem", ["s32"]),
                         "DropItem__6SLListFl")

    def test_demo_symbol(self) -> None:
        self.assertEqual(mangle_member_function("SLList", "DropItem", ["CSchedule*"]),
                         "DropItem__6SLListFP9CSchedule")

    def test_dtor(self) -> None:
        self.assertEqual(mangle_member_function("CToken", "~CToken", []), "__dt__6CTokenFv")

    def test_encode_primitives_and_typedefs(self) -> None:
        cases = {
            "void*": "Pv", "bool": "b", "char": "c", "u8": "Uc",
            "short": "s", "u16": "Us", "int": "i", "u32": "Ul",
            "long": "l", "unsigned long": "Ul", "long long": "x",
            "unsigned long long": "Ux", "float": "f", "f32": "f",
            "double": "d", "s16": "s", "s32": "l", "s64": "x", "u64": "Ux",
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
        '#pragma once\n'
        'class WdgState {\n'
        'public:\n'
        '    void Existing();\n'
        '    int value;\n'
        '};\n'
        'extern "C" void Widget_Paint(WdgState* self, int x, int y);\n',
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
        # DEFAULT IS US-ONLY: a rename must never rewrite a region the caller
        # did not ask for (multi-region rewrites caused the skew/truncation bug).
        self.assertEqual(len(plan.symbol_edits), 1)
        self.assertEqual([e.region for e in plan.symbol_edits], ["us"])
        # Explicit multi-region is still supported when genuinely intended.
        plan_multi = plan_function_rename(
            self.root, "Widget_Paint", "WdgState::Paint(int, int)",
            regions=["us", "eu"],
        )
        self.assertEqual(
            sorted(e.region for e in plan_multi.symbol_edits), ["eu", "us"]
        )
        # Only the name part changes; address/size suffix preserved.
        for edit in plan.symbol_edits:
            self.assertTrue(edit.new_line.startswith("Paint__8WdgStateFii = "),
                            msg=edit.new_line)
            self.assertIn(edit.old_line.split("=", 1)[1], edit.new_line)
        # Exact-name matching: Widget_Painter untouched.
        self.assertNotIn("Widget_Painter", "".join(e.new_line for e in plan.symbol_edits))
        self.assertEqual(len(plan.defns), 1)
        # MWCC 10127: file-scope 'Ret Class::method(...);' declarations are
        # illegal, so the two 'extern "C"' import-band decls are NOT
        # rewritten into qualified form -- they become NEEDS-REVIEW items and
        # the legal declaration is the in-class insert below.
        self.assertEqual(len(plan.decls), 0)
        self.assertEqual(len(plan.calls), 2)
        self.assertEqual(len(plan.addr_refs), 1)
        self.assertEqual(len(plan.reviews), 2)
        for rev in plan.reviews:
            self.assertIn("10127", rev.note)
        self.assertEqual(len(plan.header_edits), 1)
        he = plan.header_edits[0]
        self.assertEqual(he.class_name, "WdgState")
        self.assertEqual(he.method, "Paint")
        self.assertEqual(he.decl_line, "void Paint(int x, int y);")
        self.assertEqual(he.access, "public")
        self.assertFalse(he.already_present)
        self.assertTrue(str(he.path).endswith("Widget.hpp"))
        self.assertEqual(plan.body_self_count, 1)
        self.assertEqual(plan.configure_hits, [])
        self.assertEqual(plan.splits_hits, [])
        self.assertEqual(plan.ctx_files, ["src/unit/Widget.ctx.c"])

    def test_plan_rewrite_spans(self) -> None:
        plan = plan_function_rename(self.root, "Widget_Paint", "WdgState::Paint(int, int)")
        defn = plan.defns[0]
        self.assertEqual(defn.old_span, "void Widget_Paint(WdgState* self, int x, int y)")
        self.assertEqual(defn.new_span, "void WdgState::Paint(int x, int y)")
        # No file-scope qualified declarations are produced (MWCC 10127).
        self.assertEqual(plan.decls, [])
        for hit in plan.source_hits:
            if hit.kind == "decl":
                self.fail("member-ization must not emit file-scope qualified decls")
        # The two import-band decls are NEEDS-REVIEW with a 10127 note.
        self.assertEqual(len(plan.reviews), 2)
        for rev in plan.reviews:
            self.assertIn("10127", rev.note)
            self.assertIn("WdgState", rev.note)
        # The legal declaration is the single in-class insert.
        self.assertEqual(len(plan.header_edits), 1)
        self.assertEqual(plan.header_edits[0].decl_line, "void Paint(int x, int y);")
        call_news = sorted(c.new_span for c in plan.calls)
        # Option A: the call-result receiver is not provably WdgState*, so
        # it is wrapped; the bare identifier is left alone.
        self.assertEqual(call_news, ["((WdgState*)(getState()))->Paint(3, 4)", "st->Paint(1, 2)"])
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
        # Definition is a legal file-scope qualified definition.
        self.assertIn("void WdgState::Paint(int x, int y) {", widget)
        self.assertIn("(u32)&WdgState::Paint, 0,", widget)
        self.assertIn("this->pixels[x] = y;", widget)
        # Old name survives only in the untouched import-band forward decl
        # (flagged for manual demote) and the comment; no call/definition.
        scrubbed = widget.replace('extern "C" void Widget_Paint', '').replace(
            '// Widget_Paint renders the widget.', '')
        self.assertNotIn("Widget_Paint(", scrubbed.replace("Widget_Painter", ""))
        # MWCC 10127: no bare qualified declaration is created at file scope.
        self.assertNotIn("void WdgState::Paint(int x, int y);", widget)
        self.assertNotIn('extern "C" void WdgState::Paint', widget)
        # Import-band forward decl is left untouched (NEEDS-REVIEW) -- never
        # rewritten into qualified form.
        self.assertIn('extern "C" void Widget_Paint(WdgState* self, int x, int y);', widget)
        # Comment lines are left alone.
        self.assertIn("// Widget_Paint renders the widget.", widget)
        header = (self.root / "src" / "unit" / "Widget.hpp").read_text(encoding="utf-8")
        # The legal declaration lives in-class, exactly once.
        self.assertIn("void Paint(int x, int y);", header)
        self.assertEqual(header.count("void Paint(int x, int y);"), 1)
        self.assertNotIn("void WdgState::Paint(int x, int y);", header)
        self.assertIn("void Existing();", header)  # surrounding member kept
        caller = (self.root / "src" / "other" / "Caller.cpp").read_text(encoding="utf-8")
        self.assertIn("st->Paint(1, 2);", caller)
        self.assertIn("((WdgState*)(getState()))->Paint(3, 4);", caller)
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


def _write_minimal_member_fixture(
    root: Path,
    *,
    old: str = "SchedList_DropItem",
    class_name: str = "SLList",
    method: str = "DropItem",
    self_type: str = "SLList",
    extra_param: str = "CSchedule* x",
    caller_first: str = "effectArray[i]->mSchedule",
    caller_rest: str = "lbl_eu_80667CF0",
    header_body: str | None = None,
) -> None:
    """Minimal fixture for call-rewrite / header-insert tests."""
    (root / "config" / "us").mkdir(parents=True, exist_ok=True)
    (root / "src" / "unit").mkdir(parents=True, exist_ok=True)
    (root / "src" / "other").mkdir(parents=True, exist_ok=True)
    (root / "config" / "us" / "symbols.txt").write_text(
        f"{old} = .text:0x80001000; // type:function size:0x40\n", encoding="utf-8"
    )
    if header_body is None:
        header_body = (
            f"class {class_name} {{\n"
            "public:\n"
            "    void Existing();\n"
            "};\n"
        )
    (root / "src" / "unit" / "Owner.hpp").write_text("#pragma once\n" + header_body, encoding="utf-8")
    (root / "src" / "unit" / "Def.cpp").write_text(
        '#include "Owner.hpp"\n'
        f'extern "C" void {old}({self_type}* self, {extra_param});\n'
        f'void {old}({self_type}* self, {extra_param}) {{\n'
        '    (void)self;\n'
        '}\n',
        encoding="utf-8",
    )
    (root / "src" / "other" / "Caller.cpp").write_text(
        '#include "Owner.hpp"\n'
        f'void run() {{\n    {old}({caller_first}, {caller_rest});\n}}\n',
        encoding="utf-8",
    )
    (root / "configure.py").write_text("// stub\n", encoding="utf-8")


class SplitTopLevelTest(unittest.TestCase):
    """BUG 1: _split_top_level must ignore '<'/'>' (C++ '->', comparisons)."""

    def test_exact_repro(self) -> None:
        self.assertEqual(
            _split_top_level('effectArray[i]->mSchedule, lbl_eu_80667CF0'),
            ['effectArray[i]->mSchedule', ' lbl_eu_80667CF0'],
        )

    def test_arrow_receiver(self) -> None:
        self.assertEqual(_split_top_level('ptr->member, arg'), ['ptr->member', ' arg'])

    def test_arrow_call_nested_parens(self) -> None:
        self.assertEqual(_split_top_level('a->b(x, y), c'), ['a->b(x, y)', ' c'])

    def test_nested_calls(self) -> None:
        self.assertEqual(_split_top_level('f(g(a,b), c), d'), ['f(g(a,b), c)', ' d'])

    def test_bracket_index(self) -> None:
        self.assertEqual(_split_top_level('arr[i], x'), ['arr[i]', ' x'])
        self.assertEqual(_split_top_level('arr[i](a,b), c'), ['arr[i](a,b)', ' c'])

    def test_template_args_in_call(self) -> None:
        # Comma inside '<>' but within call parens stays intact.
        self.assertEqual(
            _split_top_level('f(std::pair<int,int>), x'),
            ['f(std::pair<int,int>)', ' x'],
        )

    def test_cast_arg(self) -> None:
        self.assertEqual(_split_top_level('(CSchedule*)x, y'), ['(CSchedule*)x', ' y'])

    def test_comparison_does_not_suppress_split(self) -> None:
        self.assertEqual(_split_top_level('a > b, c'), ['a > b', ' c'])
        self.assertEqual(_split_top_level('a < b, c'), ['a < b', ' c'])


class CallRewriteTest(unittest.TestCase):
    """BUG 1 whole-path: Old(recv, args) -> recv->Method(args), no drops."""

    def setUp(self) -> None:
        self.tmp = TemporaryDirectory()
        self.root = Path(self.tmp.name)

    def tearDown(self) -> None:
        self.tmp.cleanup()

    def test_arrow_receiver_no_dropped_args(self) -> None:
        _write_minimal_member_fixture(self.root)
        plan = plan_function_rename(self.root, "SchedList_DropItem", "SLList::DropItem(CSchedule*)")
        self.assertEqual(len(plan.calls), 1)
        call = plan.calls[0]
        # Option A: ptr->member is not provably SLList*, so it is wrapped
        # in an explicit cast (identical pointer value, valid C++).
        self.assertEqual(
            call.new_span,
            "((SLList*)(effectArray[i]->mSchedule))->DropItem(lbl_eu_80667CF0)",
        )
        # Both arguments survived: receiver + one rest arg (no '(recv, arg)->' shape).
        self.assertNotIn(", lbl_eu_80667CF0)->DropItem()", call.new_span)
        self.assertIn("lbl_eu_80667CF0", call.new_span)

    def test_nested_call_first_arg(self) -> None:
        _write_minimal_member_fixture(
            self.root, caller_first="f(g(a,b), c)", caller_rest="d",
            extra_param="int* p", class_name="Wdg", self_type="Wdg",
            old="Wdg_Do", method="Do",
        )
        plan = plan_function_rename(self.root, "Wdg_Do", "Wdg::Do(int*)")
        self.assertEqual(len(plan.calls), 1)
        # Option A: a call-result receiver is wrapped in an explicit cast.
        self.assertEqual(plan.calls[0].new_span, "((Wdg*)(f(g(a,b), c)))->Do(d)")

    def test_cast_receiver(self) -> None:
        _write_minimal_member_fixture(
            self.root, caller_first="(CSchedule*)node->mItem", caller_rest="dt",
            extra_param="float dt", class_name="CSchedule", self_type="CSchedule",
            old="schedUpdate", method="Update",
        )
        plan = plan_function_rename(self.root, "schedUpdate", "CSchedule::Update(float)")
        self.assertEqual(len(plan.calls), 1)
        self.assertEqual(plan.calls[0].new_span, "((CSchedule*)node->mItem)->Update(dt)")


class CallReceiverCastTest(unittest.TestCase):
    """Option A: non-provable receivers get ``((Class*)...)``; provable ones do not.

    Regression for the bulk wave-2 blocker: member-izing
    ``MiniMapFillMarkerEntry`` rewrote ``((u8*)self + 0x17C)`` (a ``u8*``) into
    ``((u8*)self + 0x17C)->FillMarkerEntry()``, which MWCC rejects with
    ``Error: (10141) expression syntax error``.  The fix wraps such receivers
    in an explicit cast to the target class (identical pointer value)."""

    def setUp(self) -> None:
        self.tmp = TemporaryDirectory()
        self.root = Path(self.tmp.name)

    def tearDown(self) -> None:
        self.tmp.cleanup()

    def test_pointer_arithmetic_receiver_gets_cast(self) -> None:
        _write_minimal_member_fixture(
            self.root, caller_first="((u8*)self + 0x17C)", caller_rest="row",
            extra_param="u32 row", class_name="GimmickView", self_type="GimmickView",
            old="FillMarkerEntry", method="FillMarkerEntry",
        )
        plan = plan_function_rename(
            self.root, "FillMarkerEntry", "GimmickView::FillMarkerEntry(u32)")
        # The only review is the expected TU-local import-band decl (10127).
        self.assertTrue(all("10127" in r.note for r in plan.reviews))
        self.assertEqual(len(plan.calls), 1)
        self.assertEqual(
            plan.calls[0].new_span,
            "((GimmickView*)((u8*)self + 0x17C))->FillMarkerEntry(row)",
        )

    def test_exact_live_minimap_shape(self) -> None:
        # Live source: src/kyoshin/CMiniMap.cpp:1127
        #     gimmickView = MiniMapFillMarkerEntry((u8*)self + 0x17C);
        # where MiniMapFillMarkerEntry(CMiniMapGimmickView* self) lives at
        # minimap+0x17C.  The member-ized call must be valid C++.
        _write_minimal_member_fixture(
            self.root, old="MiniMapFillMarkerEntry",
            class_name="CMiniMapGimmickView", method="FillMarkerEntry",
            self_type="CMiniMapGimmickView", extra_param="u32 row",
            caller_first="((u8*)self + 0x17C)", caller_rest="row",
            header_body=(
                "struct CMiniMapGimmickView {\n"
                "public:\n"
                "    void Existing();\n"
                "};\n"
            ),
        )
        (self.root / "src" / "other" / "Caller.cpp").write_text(
            '#include "Owner.hpp"\n'
            "void run() {\n"
            "    gimmickView = MiniMapFillMarkerEntry((u8*)self + 0x17C);\n"
            "}\n",
            encoding="utf-8",
        )
        (self.root / "src" / "unit" / "Def.cpp").write_text(
            '#include "Owner.hpp"\n'
            "u32 MiniMapFillMarkerEntry(CMiniMapGimmickView* self) {\n"
            "    (void)self;\n"
            "    return 0;\n"
            "}\n",
            encoding="utf-8",
        )
        plan = plan_function_rename(
            self.root, "MiniMapFillMarkerEntry", "CMiniMapGimmickView::FillMarkerEntry")
        self.assertEqual(plan.reviews, [])
        self.assertEqual(len(plan.calls), 1)
        self.assertEqual(
            plan.calls[0].new_span,
            "((CMiniMapGimmickView*)((u8*)self + 0x17C))->FillMarkerEntry()",
        )

    def test_fully_parenthesized_cast_not_double_wrapped(self) -> None:
        _write_minimal_member_fixture(
            self.root, caller_first="((CSchedule*)x)", caller_rest="dt",
            extra_param="float dt", class_name="CSchedule", self_type="CSchedule",
            old="schedUpdate", method="Update",
        )
        plan = plan_function_rename(self.root, "schedUpdate", "CSchedule::Update(float)")
        self.assertEqual(len(plan.calls), 1)
        self.assertEqual(plan.calls[0].new_span, "((CSchedule*)x)->Update(dt)")
        self.assertNotIn("((CSchedule*)((CSchedule*)x))", plan.calls[0].new_span)

    def test_cast_to_other_type_is_wrapped(self) -> None:
        # A cast to u8* does NOT prove CSchedule*: wrap it.  Only a cast to
        # the TARGET class suppresses the wrap (see previous test).
        _write_minimal_member_fixture(
            self.root, caller_first="((u8*)p)", caller_rest="dt",
            extra_param="float dt", class_name="CSchedule", self_type="CSchedule",
            old="schedUpdate", method="Update",
        )
        plan = plan_function_rename(self.root, "schedUpdate", "CSchedule::Update(float)")
        self.assertEqual(len(plan.calls), 1)
        self.assertEqual(
            plan.calls[0].new_span,
            "((CSchedule*)((u8*)p))->Update(dt)",
        )

    def test_index_receiver_gets_cast(self) -> None:
        _write_minimal_member_fixture(
            self.root, caller_first="arr[i]", caller_rest="x",
            extra_param="int x", class_name="Wdg", self_type="Wdg",
            old="Wdg_Do", method="Do",
        )
        plan = plan_function_rename(self.root, "Wdg_Do", "Wdg::Do(int)")
        self.assertTrue(all("10127" in r.note for r in plan.reviews))
        self.assertEqual(len(plan.calls), 1)
        self.assertEqual(plan.calls[0].new_span, "((Wdg*)(arr[i]))->Do(x)")

    def test_ptr_member_receiver_gets_cast(self) -> None:
        _write_minimal_member_fixture(
            self.root, caller_first="ptr->member", caller_rest="x",
            extra_param="int x", class_name="Wdg", self_type="Wdg",
            old="Wdg_Do", method="Do",
        )
        plan = plan_function_rename(self.root, "Wdg_Do", "Wdg::Do(int)")
        self.assertTrue(all("10127" in r.note for r in plan.reviews))
        self.assertEqual(len(plan.calls), 1)
        self.assertEqual(plan.calls[0].new_span, "((Wdg*)(ptr->member))->Do(x)")

    def test_this_field_receiver_gets_cast(self) -> None:
        _write_minimal_member_fixture(
            self.root, caller_first="this->field", caller_rest="x",
            extra_param="int x", class_name="Wdg", self_type="Wdg",
            old="Wdg_Do", method="Do",
        )
        plan = plan_function_rename(self.root, "Wdg_Do", "Wdg::Do(int)")
        self.assertTrue(all("10127" in r.note for r in plan.reviews))
        self.assertEqual(len(plan.calls), 1)
        self.assertEqual(plan.calls[0].new_span, "((Wdg*)(this->field))->Do(x)")

    def test_plain_identifier_gets_no_cast(self) -> None:
        _write_minimal_member_fixture(
            self.root, caller_first="st", caller_rest="1",
            extra_param="int x", class_name="Wdg", self_type="Wdg",
            old="Wdg_Do", method="Do",
        )
        plan = plan_function_rename(self.root, "Wdg_Do", "Wdg::Do(int)")
        self.assertEqual(len(plan.calls), 1)
        self.assertEqual(plan.calls[0].new_span, "st->Do(1)")
        self.assertNotIn("((Wdg*)", plan.calls[0].new_span)

    def test_this_gets_no_cast(self) -> None:
        _write_minimal_member_fixture(
            self.root, caller_first="this", caller_rest="1",
            extra_param="int x", class_name="Wdg", self_type="Wdg",
            old="Wdg_Do", method="Do",
        )
        plan = plan_function_rename(self.root, "Wdg_Do", "Wdg::Do(int)")
        self.assertEqual(len(plan.calls), 1)
        self.assertEqual(plan.calls[0].new_span, "this->Do(1)")

    def test_call_result_receiver_gets_cast(self) -> None:
        _write_minimal_member_fixture(
            self.root, caller_first="getView()", caller_rest="1, 2",
            extra_param="int x, int y", class_name="Wdg", self_type="Wdg",
            old="Wdg_Do", method="Do",
        )
        plan = plan_function_rename(self.root, "Wdg_Do", "Wdg::Do(int, int)")
        self.assertTrue(all("10127" in r.note for r in plan.reviews))
        self.assertEqual(len(plan.calls), 1)
        self.assertEqual(plan.calls[0].new_span, "((Wdg*)(getView()))->Do(1, 2)")

    def test_missing_receiver_stays_review(self) -> None:
        # No receiver expression at all: the class cannot be determined, so
        # the safe path is a NEEDS-REVIEW item, never a bare ``->``.
        _write_minimal_member_fixture(self.root)
        (self.root / "src" / "other" / "Caller.cpp").write_text(
            '#include "Owner.hpp"\n'
            "void run() {\n"
            "    SchedList_DropItem();\n"
            "}\n",
            encoding="utf-8",
        )
        plan = plan_function_rename(
            self.root, "SchedList_DropItem", "SLList::DropItem(CSchedule*)")
        self.assertEqual(plan.calls, [])
        notes = [r.note for r in plan.reviews]
        # The argless call plus the expected import-band decl (10127).
        self.assertEqual(len(notes), 2)
        self.assertTrue(any("no arguments" in n for n in notes))
        self.assertTrue(any("10127" in n for n in notes))


class HeaderInsertTest(unittest.TestCase):
    """GAP 2: in-class declaration insert + MWCC 10127 guard."""

    def setUp(self) -> None:
        self.tmp = TemporaryDirectory()
        self.root = Path(self.tmp.name)

    def tearDown(self) -> None:
        self.tmp.cleanup()

    def test_insert_into_public_section(self) -> None:
        _write_fixture_tree(self.root)
        plan = plan_function_rename(self.root, "Widget_Paint", "WdgState::Paint(int, int)")
        self.assertEqual(len(plan.header_edits), 1)
        he = plan.header_edits[0]
        self.assertEqual(he.access, "public")
        self.assertEqual(he.decl_line, "void Paint(int x, int y);")
        # Apply emits it exactly once.
        _plan, _log = apply_function_rename(
            self.root, "Widget_Paint", "WdgState::Paint(int, int)",
            regions=["us", "eu"], dry_run=False, allow_review=True,
        )
        header = (self.root / "src" / "unit" / "Widget.hpp").read_text(encoding="utf-8")
        self.assertEqual(header.count("void Paint(int x, int y);"), 1)
        self.assertIn("void Existing();", header)
        # In-class, not file-scope: between 'class WdgState' and the closing '};'.
        self.assertLess(header.index("class WdgState"), header.index("void Paint(int x, int y);"))
        self.assertLess(header.index("void Paint(int x, int y);"), header.rindex("};"))
        self.assertNotIn("}\n    void Paint", header)  # must not dangle outside the body

    def test_insert_prefers_method_section(self) -> None:
        _write_minimal_member_fixture(
            self.root,
            header_body=(
                "class SLList {\n"
                "public:\n"
                "    int value;\n"
                "private:\n"
                "    void Helper();\n"
                "    void Other();\n"
                "};\n"
            ),
        )
        plan = plan_function_rename(self.root, "SchedList_DropItem", "SLList::DropItem(CSchedule*)")
        self.assertEqual(plan.header_edits[0].access, "private")

    def test_idempotent_rerun(self) -> None:
        _write_fixture_tree(self.root)
        apply_function_rename(
            self.root, "Widget_Paint", "WdgState::Paint(int, int)",
            regions=["us", "eu"], dry_run=False, allow_review=True,
        )
        header1 = (self.root / "src" / "unit" / "Widget.hpp").read_text(encoding="utf-8")
        self.assertEqual(header1.count("void Paint(int x, int y);"), 1)
        # Re-running after success must not duplicate: Old is gone, so plan refuses.
        with self.assertRaises(FnRenameRefusal):
            plan_function_rename(self.root, "Widget_Paint", "WdgState::Paint(int, int)")
        self.assertEqual(header1.count("void Paint(int x, int y);"), 1)
        # already_present path: fresh fixture whose header already has decl
        # must report already_present and apply must not duplicate.
        # Simulate re-run header check: planning a fresh fixture that already
        # contains the decl must report already_present.
        _write_fixture_tree(self.root)
        hpp = self.root / "src" / "unit" / "Widget.hpp"
        text = hpp.read_text(encoding="utf-8")
        text = text.replace("    void Existing();\n", "    void Existing();\n    void Paint(int x, int y);\n")
        hpp.write_text(text, encoding="utf-8")
        plan3 = plan_function_rename(self.root, "Widget_Paint", "WdgState::Paint(int, int)")
        self.assertTrue(plan3.header_edits[0].already_present)
        _p, _l = apply_function_rename(
            self.root, "Widget_Paint", "WdgState::Paint(int, int)",
            regions=["us", "eu"], dry_run=False, allow_review=True,
        )
        header3 = hpp.read_text(encoding="utf-8")
        self.assertEqual(header3.count("void Paint(int x, int y);"), 1)

    def test_no_header_insert_flag(self) -> None:
        _write_minimal_member_fixture(self.root)
        (self.root / "src" / "unit" / "Owner.hpp").write_text("#pragma once\n// no class\n", encoding="utf-8")
        # Without the flag this refuses (no unambiguous class body).
        with self.assertRaises(FnRenameRefusal):
            plan_function_rename(self.root, "SchedList_DropItem", "SLList::DropItem(CSchedule*)")
        # With the flag it plans (header insert skipped).
        plan = plan_function_rename(
            self.root, "SchedList_DropItem", "SLList::DropItem(CSchedule*)",
            no_header_insert=True,
        )
        self.assertEqual(plan.header_edits, [])
        self.assertTrue(any("--no-header-insert" in n for n in plan.header_notes))

    def test_missing_class_refuses(self) -> None:
        _write_minimal_member_fixture(self.root)
        (self.root / "src" / "unit" / "Owner.hpp").write_text("#pragma once\nstruct Other {};\n", encoding="utf-8")
        with self.assertRaises(FnRenameRefusal) as ctx:
            plan_function_rename(self.root, "SchedList_DropItem", "SLList::DropItem(CSchedule*)")
        self.assertIn("SLList", str(ctx.exception))
        self.assertIn("no", str(ctx.exception).lower())

    def test_ambiguous_class_refuses(self) -> None:
        _write_minimal_member_fixture(self.root)
        (self.root / "src" / "unit" / "Second.hpp").write_text(
            "#pragma once\nclass SLList {\npublic:\n    void Foo();\n};\n", encoding="utf-8"
        )
        with self.assertRaises(FnRenameRefusal) as ctx:
            plan_function_rename(self.root, "SchedList_DropItem", "SLList::DropItem(CSchedule*)")
        self.assertIn("ambiguous", str(ctx.exception).lower())

    def test_10127_import_band_not_rewritten(self) -> None:
        _write_fixture_tree(self.root)
        plan = plan_function_rename(self.root, "Widget_Paint", "WdgState::Paint(int, int)")
        # No file-scope qualified declaration is ever emitted.
        for hit in plan.source_hits:
            self.assertNotEqual(hit.kind, "decl", msg=f"unexpected decl hit: {hit}")
        for rev in plan.reviews:
            if "Widget_Paint" in (rev.note or "") or rev.old_span == "Widget_Paint":
                pass
        decl_notes = [r.note for r in plan.reviews]
        self.assertTrue(any("10127" in n for n in decl_notes))
        # The import-band lines themselves are untouched by the plan.
        hpp = (self.root / "src" / "unit" / "Widget.hpp").read_text(encoding="utf-8")
        self.assertIn('extern "C" void Widget_Paint', hpp)
        self.assertNotIn("WdgState::Paint(int x, int y);", hpp)

    def test_10127_extern_c_block_form(self) -> None:
        _write_minimal_member_fixture(self.root)
        deff = self.root / "src" / "unit" / "Def.cpp"
        deff.write_text(
            '#include "Owner.hpp"\n'
            'extern "C" {\n'
            'void SchedList_DropItem(SLList* self, CSchedule* x);\n'
            '}\n'
            'void SchedList_DropItem(SLList* self, CSchedule* x) {\n'
            '    (void)self; (void)x;\n'
            '}\n',
            encoding="utf-8",
        )
        plan = plan_function_rename(self.root, "SchedList_DropItem", "SLList::DropItem(CSchedule*)")
        self.assertEqual(plan.decls, [])
        self.assertTrue(any("10127" in r.note for r in plan.reviews))
        for hit in plan.source_hits:
            self.assertNotEqual(hit.kind, "decl")


class DemoteImportBandTest(unittest.TestCase):
    """--demote-import-band: auto-resolve the single dominant blocker.

    The TU-local `extern "C"` import-band declaration of the renamed free
    function is the only review in ~55% of member-izable candidates.  When
    (and only when) every review is such a declaration and a header insert
    supplies the in-class declaration, the tool rewrites each import-band
    line into a line-neutral `// (memberized: ...)` comment and records it
    as a resolved edit (kind 'demote'), so the plan becomes clean.
    """

    def setUp(self) -> None:
        self.tmp = TemporaryDirectory()
        self.root = Path(self.tmp.name)

    def tearDown(self) -> None:
        self.tmp.cleanup()

    def test_plan_becomes_clean(self) -> None:
        _write_minimal_member_fixture(self.root)
        plain = plan_function_rename(
            self.root, "SchedList_DropItem", "SLList::DropItem(CSchedule*)"
        )
        self.assertGreater(len(plain.reviews), 0)
        self.assertEqual(plain.demotions, [])
        self.assertTrue(all("10127" in r.note for r in plain.reviews))
        plan = plan_function_rename(
            self.root, "SchedList_DropItem", "SLList::DropItem(CSchedule*)",
            demote_import_band=True,
        )
        self.assertEqual(plan.reviews, [])
        self.assertEqual(len(plan.demotions), len(plain.reviews))
        self.assertEqual(len(plan.demotions), 1)
        self.assertTrue(plan.demote_import_band)
        for d in plan.demotions:
            self.assertEqual(d.kind, "demote")
            self.assertIn(
                "// (memberized: SchedList_DropItem -> SLList::DropItem;",
                d.new_span,
            )
            self.assertIn("declaration moved into class SLList", d.new_span)
            self.assertEqual(d.old_span.count("\n"), d.new_span.count("\n"))
        # Header insert still supplies the legal declaration.
        self.assertEqual(len(plan.header_edits), 1)

    def test_exact_line_count_preserved(self) -> None:
        _write_minimal_member_fixture(self.root)
        deff = self.root / "src" / "unit" / "Def.cpp"
        before = deff.read_text(encoding="utf-8")
        before_lines = len(before.splitlines())
        plan, _log = apply_function_rename(
            self.root, "SchedList_DropItem", "SLList::DropItem(CSchedule*)",
            regions=["us"], dry_run=False, demote_import_band=True,
        )
        self.assertEqual(plan.reviews, [])
        after = deff.read_text(encoding="utf-8")
        # Def.cpp is not the header-insert target, so its line count is
        # exactly preserved by the line-neutral demotion.
        self.assertEqual(len(after.splitlines()), before_lines)
        self.assertNotIn('extern "C" void SchedList_DropItem', after)
        self.assertIn("// (memberized: SchedList_DropItem -> SLList::DropItem;", after)
        # No file-scope qualified declaration is ever created (MWCC 10127).
        self.assertNotIn("SLList::DropItem(CSchedule* x);", after)

    def test_multiline_import_band_stays_line_neutral(self) -> None:
        _write_minimal_member_fixture(self.root)
        deff = self.root / "src" / "unit" / "Def.cpp"
        deff.write_text(
            '#include "Owner.hpp"\n'
            'extern "C" void SchedList_DropItem(\n'
            '    SLList* self,\n'
            '    CSchedule* x);\n'
            'void SchedList_DropItem(SLList* self, CSchedule* x) {\n'
            '    (void)self; (void)x;\n'
            '}\n',
            encoding="utf-8",
        )
        before_lines = len(deff.read_text(encoding="utf-8").splitlines())
        plan = plan_function_rename(
            self.root, "SchedList_DropItem", "SLList::DropItem(CSchedule*)",
            demote_import_band=True,
        )
        self.assertEqual(plan.reviews, [])
        self.assertEqual(len(plan.demotions), 1)
        dem = plan.demotions[0]
        self.assertEqual(dem.start_line, 2)
        self.assertEqual(dem.end_line, 4)
        self.assertEqual(dem.old_span.count("\n"), 2)
        self.assertEqual(dem.new_span.count("\n"), 2)
        _plan, _log = apply_function_rename(
            self.root, "SchedList_DropItem", "SLList::DropItem(CSchedule*)",
            regions=["us"], dry_run=False, demote_import_band=True,
        )
        after_lines = len(deff.read_text(encoding="utf-8").splitlines())
        self.assertEqual(after_lines, before_lines)

    def test_idempotent_rerun_no_stack(self) -> None:
        _write_minimal_member_fixture(self.root)
        deff = self.root / "src" / "unit" / "Def.cpp"
        hpp = self.root / "src" / "unit" / "Owner.hpp"
        _plan, _log = apply_function_rename(
            self.root, "SchedList_DropItem", "SLList::DropItem(CSchedule*)",
            regions=["us"], dry_run=False, demote_import_band=True,
        )
        first_cpp = deff.read_text(encoding="utf-8")
        first_hpp = hpp.read_text(encoding="utf-8")
        self.assertEqual(first_cpp.count("(memberized: SchedList_DropItem"), 1)
        self.assertNotIn("// //", first_cpp)
        self.assertNotIn("// //", first_hpp)
        self.assertEqual(first_hpp.count("void DropItem(CSchedule* x);"), 1)
        # Old symbol is gone: re-planning the same rename refuses instead of
        # stacking another comment.
        with self.assertRaises(FnRenameRefusal):
            plan_function_rename(
                self.root, "SchedList_DropItem", "SLList::DropItem(CSchedule*)",
                demote_import_band=True,
            )
        self.assertEqual(deff.read_text(encoding="utf-8"), first_cpp)
        self.assertEqual(hpp.read_text(encoding="utf-8"), first_hpp)
        # A demotion marker is comment-only: rescans skip it, so even if the
        # old symbol were still present elsewhere the marker never re-emits.
        self.assertNotIn('extern "C" void SchedList_DropItem', first_cpp)

    def test_refuse_when_no_header_insert(self) -> None:
        _write_minimal_member_fixture(self.root)
        with self.assertRaises(FnRenameRefusal) as ctx:
            plan_function_rename(
                self.root, "SchedList_DropItem", "SLList::DropItem(CSchedule*)",
                no_header_insert=True, demote_import_band=True,
            )
        self.assertIn("--demote-import-band", str(ctx.exception))
        self.assertIn("in-class", str(ctx.exception))
        # Without demote the same shape stays a review (no loud refusal).
        plain = plan_function_rename(
            self.root, "SchedList_DropItem", "SLList::DropItem(CSchedule*)",
            no_header_insert=True,
        )
        self.assertGreater(len(plain.reviews), 0)

    def test_refuse_multi_declaration_line(self) -> None:
        _write_minimal_member_fixture(self.root)
        deff = self.root / "src" / "unit" / "Def.cpp"
        deff.write_text(
            '#include "Owner.hpp"\n'
            'extern "C" void SchedList_DropItem(SLList* self, CSchedule* x); '
            'void Other(SLList* self, int y);\n'
            'void SchedList_DropItem(SLList* self, CSchedule* x) {\n'
            '    (void)self; (void)x;\n'
            '}\n',
            encoding="utf-8",
        )
        # Default plan surfaces the import-band review without refusing.
        plain = plan_function_rename(
            self.root, "SchedList_DropItem", "SLList::DropItem(CSchedule*)"
        )
        self.assertTrue(any("10127" in r.note for r in plain.reviews))
        # Opt-in demotion refuses loudly: the line mentions other symbols too.
        with self.assertRaises(FnRenameRefusal) as ctx:
            plan_function_rename(
                self.root, "SchedList_DropItem", "SLList::DropItem(CSchedule*)",
                demote_import_band=True,
            )
        self.assertIn("other", str(ctx.exception).lower())

    def test_additional_unrelated_review_stays_review(self) -> None:
        _write_minimal_member_fixture(self.root)
        caller = self.root / "src" / "other" / "Caller.cpp"
        caller.write_text(
            '#include "Owner.hpp"\n'
            'void run() {\n'
            '    SchedList_DropItem(effectArray[i]->mSchedule, lbl);\n'
            '}\n'
            'int bare = SchedList_DropItem;\n',
            encoding="utf-8",
        )
        plan = plan_function_rename(
            self.root, "SchedList_DropItem", "SLList::DropItem(CSchedule*)",
            demote_import_band=True,
        )
        # Not ONLY import-band reviews (there is a bare occurrence too), so
        # nothing is demoted and the plan stays NEEDS-REVIEW.
        self.assertGreater(len(plan.reviews), 1)
        self.assertEqual(plan.demotions, [])
        self.assertTrue(any("10127" in r.note for r in plan.reviews))
        self.assertTrue(any("bare occurrence" in r.note for r in plan.reviews))

    def test_comment_lines_untouched_and_do_not_block(self) -> None:
        _write_minimal_member_fixture(self.root)
        deff = self.root / "src" / "unit" / "Def.cpp"
        text = deff.read_text(encoding="utf-8")
        deff.write_text(
            '// SchedList_DropItem renders the widget.\n' + text,
            encoding="utf-8",
        )
        before_lines = len(deff.read_text(encoding="utf-8").splitlines())
        plan = plan_function_rename(
            self.root, "SchedList_DropItem", "SLList::DropItem(CSchedule*)",
            demote_import_band=True,
        )
        # Pure comment mentions are skipped by the scanner: demotion proceeds.
        self.assertEqual(plan.reviews, [])
        self.assertEqual(len(plan.demotions), 1)
        _plan, _log = apply_function_rename(
            self.root, "SchedList_DropItem", "SLList::DropItem(CSchedule*)",
            regions=["us"], dry_run=False, demote_import_band=True,
        )
        after = deff.read_text(encoding="utf-8")
        self.assertEqual(len(after.splitlines()), before_lines)
        self.assertIn("// SchedList_DropItem renders the widget.", after)

    def test_string_literal_blocks_demote(self) -> None:
        _write_minimal_member_fixture(self.root)
        caller = self.root / "src" / "other" / "Caller.cpp"
        caller.write_text(
            '#include "Owner.hpp"\n'
            'const char* name = "SchedList_DropItem";\n'
            'void run() {\n'
            '    SchedList_DropItem(effectArray[i]->mSchedule, lbl);\n'
            '}\n',
            encoding="utf-8",
        )
        plan = plan_function_rename(
            self.root, "SchedList_DropItem", "SLList::DropItem(CSchedule*)",
            demote_import_band=True,
        )
        # The string literal is a bare-occurrence review, so the plan must
        # NOT auto-resolve: string contents are never rewritten.
        self.assertGreater(len(plan.reviews), 0)
        self.assertEqual(plan.demotions, [])
        self.assertTrue(any("bare occurrence" in r.note for r in plan.reviews))

    def test_block_form_demotes_inner_line_only(self) -> None:
        _write_minimal_member_fixture(self.root)
        deff = self.root / "src" / "unit" / "Def.cpp"
        deff.write_text(
            '#include "Owner.hpp"\n'
            'extern "C" {\n'
            'void SchedList_DropItem(SLList* self, CSchedule* x);\n'
            '}\n'
            'void SchedList_DropItem(SLList* self, CSchedule* x) {\n'
            '    (void)self; (void)x;\n'
            '}\n',
            encoding="utf-8",
        )
        before = deff.read_text(encoding="utf-8")
        before_lines = len(before.splitlines())
        plan = plan_function_rename(
            self.root, "SchedList_DropItem", "SLList::DropItem(CSchedule*)",
            demote_import_band=True,
        )
        self.assertEqual(plan.reviews, [])
        self.assertEqual(len(plan.demotions), 1)
        dem = plan.demotions[0]
        self.assertNotIn('extern "C"', dem.old_span)
        self.assertIn("SchedList_DropItem", dem.old_span)
        _plan, _log = apply_function_rename(
            self.root, "SchedList_DropItem", "SLList::DropItem(CSchedule*)",
            regions=["us"], dry_run=False, demote_import_band=True,
        )
        after = deff.read_text(encoding="utf-8")
        self.assertEqual(len(after.splitlines()), before_lines)
        self.assertIn('extern "C" {', after)
        self.assertIn("}", after)
        self.assertIn("(memberized: SchedList_DropItem", after)

    def test_apply_clean_without_allow_review(self) -> None:
        _write_minimal_member_fixture(self.root)
        # Without the flag the import-band reviews need --allow-review.
        with self.assertRaises(FnRenameRefusal):
            apply_function_rename(
                self.root, "SchedList_DropItem", "SLList::DropItem(CSchedule*)",
                regions=["us"], dry_run=False, allow_review=False,
            )
        # With the flag the same plan is clean: no --allow-review needed.
        plan, log = apply_function_rename(
            self.root, "SchedList_DropItem", "SLList::DropItem(CSchedule*)",
            regions=["us"], dry_run=False, allow_review=False,
            demote_import_band=True,
        )
        self.assertEqual(plan.reviews, [])
        self.assertEqual(len(plan.demotions), 1)
        deff = (self.root / "src" / "unit" / "Def.cpp").read_text(encoding="utf-8")
        self.assertIn("(memberized: SchedList_DropItem", deff)


if __name__ == "__main__":
    unittest.main()
