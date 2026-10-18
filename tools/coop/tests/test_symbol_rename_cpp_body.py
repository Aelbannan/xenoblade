"""Tests for .cpp class-body search in tools/coop/symbol_fn_rename.py.

Covers the dominant member-ization blocker: target classes forward-declared
in a header (.hpp/.h) but fully defined in a TU's .cpp (e.g. CColiObject,
CGimmickGlobal).  _find_class_bodies must search .cpp files, ignore forward
declarations, prefer same-file > header > unique .cpp, and refuse loudly
when ambiguous.
"""

from __future__ import annotations

import unittest
from pathlib import Path
from tempfile import TemporaryDirectory

from tools.coop.symbol_fn_rename import (
    FnRenameRefusal,
    _find_class_bodies,
    apply_function_rename,
    plan_function_rename,
)

OLD = "SchedList_DropItem"
CLS = "SLList"
METHOD = "DropItem"
SELF_TYPE = "SLList"
EXTRA = "CSchedule* x"


def _write_tree(
    root: Path,
    *,
    header_text: str,
    def_text: str,
    caller_text: str | None = None,
    extra_files: dict[str, str] | None = None,
    old: str = OLD,
) -> None:
    (root / "config" / "us").mkdir(parents=True, exist_ok=True)
    (root / "src" / "unit").mkdir(parents=True, exist_ok=True)
    (root / "src" / "other").mkdir(parents=True, exist_ok=True)
    (root / "config" / "us" / "symbols.txt").write_text(
        f"{old} = .text:0x80001000; // type:function size:0x40\n", encoding="utf-8"
    )
    (root / "src" / "unit" / "Owner.hpp").write_text(header_text, encoding="utf-8")
    (root / "src" / "unit" / "Def.cpp").write_text(def_text, encoding="utf-8")
    if caller_text is None:
        caller_text = (
            '#include "Owner.hpp"\n'
            'void run() {\n'
            f'    {old}(effectArray[i]->mSchedule, lbl);\n'
            '}\n'
        )
    (root / "src" / "other" / "Caller.cpp").write_text(caller_text, encoding="utf-8")
    (root / "configure.py").write_text("// stub\n", encoding="utf-8")
    for rel, text in (extra_files or {}).items():
        p = root / rel
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_text(text, encoding="utf-8")


def _def_with_body_in_same_file(body: str) -> str:
    return (
        '#include "Owner.hpp"\n'
        + body
        + f'extern "C" void {OLD}({SELF_TYPE}* self, {EXTRA});\n'
        f'void {OLD}({SELF_TYPE}* self, {EXTRA}) {{\n'
        '    (void)self; (void)*x;\n'
        '}\n'
    )


SLLIST_BODY = (
    "class SLList {\n"
    "public:\n"
    "    void Existing();\n"
    "};\n"
)


class CppBodySearchTest(unittest.TestCase):
    def setUp(self) -> None:
        self.tmp = TemporaryDirectory()
        self.root = Path(self.tmp.name)

    def tearDown(self) -> None:
        self.tmp.cleanup()

    def test_body_only_in_cpp_same_file(self) -> None:
        """Body lives only in the .cpp that also holds the definition."""
        _write_tree(
            self.root,
            header_text="#pragma once\nstruct Other {};\n",
            def_text=_def_with_body_in_same_file(SLLIST_BODY),
        )
        bodies = _find_class_bodies(self.root, CLS)
        self.assertEqual(len(bodies), 1)
        self.assertEqual(bodies[0][0].suffix, ".cpp")
        plan = plan_function_rename(self.root, OLD, f"{CLS}::{METHOD}(CSchedule*)")
        self.assertEqual(len(plan.header_edits), 1)
        he = plan.header_edits[0]
        self.assertTrue(str(he.path).endswith("Def.cpp"))
        self.assertEqual(he.decl_line, "void DropItem(CSchedule* x);")
        self.assertFalse(he.already_present)

    def test_forward_decl_header_plus_body_cpp_picks_cpp(self) -> None:
        """Forward decl in header is ignored; the .cpp body is selected."""
        _write_tree(
            self.root,
            header_text="#pragma once\nclass SLList;\n",
            def_text=_def_with_body_in_same_file(SLLIST_BODY),
        )
        bodies = _find_class_bodies(self.root, CLS)
        # Forward declaration must not count.
        self.assertEqual(len(bodies), 1)
        self.assertEqual(bodies[0][0].suffix, ".cpp")
        plan = plan_function_rename(self.root, OLD, f"{CLS}::{METHOD}(CSchedule*)")
        self.assertEqual(len(plan.header_edits), 1)
        self.assertTrue(str(plan.header_edits[0].path).endswith("Def.cpp"))

    def test_struct_forward_decl_ignored(self) -> None:
        """'struct X;' alone is not a body -> refuse with 'found no'."""
        _write_tree(
            self.root,
            header_text="#pragma once\nstruct SLList;\n",
            def_text=(
                '#include "Owner.hpp"\n'
                f'extern "C" void {OLD}({SELF_TYPE}* self, {EXTRA});\n'
                f'void {OLD}({SELF_TYPE}* self, {EXTRA}) {{\n'
                '    (void)self; (void)*x;\n'
                '}\n'
            ),
        )
        bodies = _find_class_bodies(self.root, CLS)
        self.assertEqual(bodies, [])
        with self.assertRaises(FnRenameRefusal) as ctx:
            plan_function_rename(self.root, OLD, f"{CLS}::{METHOD}(CSchedule*)")
        msg = str(ctx.exception).lower()
        self.assertIn("found no", msg)
        self.assertIn("sllist", msg)

    def test_two_real_bodies_refuse(self) -> None:
        """Two .cpp bodies in different TUs (no header body, no same-file)."""
        _write_tree(
            self.root,
            header_text="#pragma once\nstruct Other {};\n",
            def_text=(
                '#include "Owner.hpp"\n'
                f'extern "C" void {OLD}({SELF_TYPE}* self, {EXTRA});\n'
                f'void {OLD}({SELF_TYPE}* self, {EXTRA}) {{\n'
                '    (void)self; (void)*x;\n'
                '}\n'
            ),
            extra_files={
                "src/unit/Extra1.cpp": SLLIST_BODY,
                "src/unit/Extra2.cpp": SLLIST_BODY,
            },
        )
        bodies = _find_class_bodies(self.root, CLS)
        self.assertEqual(len(bodies), 2)
        with self.assertRaises(FnRenameRefusal) as ctx:
            plan_function_rename(self.root, OLD, f"{CLS}::{METHOD}(CSchedule*)")
        self.assertIn("ambiguous", str(ctx.exception).lower())

    def test_two_headers_refuse(self) -> None:
        """Two header bodies remain ambiguous even with .cpp search."""
        _write_tree(
            self.root,
            header_text="#pragma once\n" + SLLIST_BODY,
            def_text=(
                '#include "Owner.hpp"\n'
                f'extern "C" void {OLD}({SELF_TYPE}* self, {EXTRA});\n'
                f'void {OLD}({SELF_TYPE}* self, {EXTRA}) {{\n'
                '    (void)self; (void)*x;\n'
                '}\n'
            ),
            extra_files={"src/unit/Second.hpp": "#pragma once\n" + SLLIST_BODY},
        )
        with self.assertRaises(FnRenameRefusal) as ctx:
            plan_function_rename(self.root, OLD, f"{CLS}::{METHOD}(CSchedule*)")
        self.assertIn("ambiguous", str(ctx.exception).lower())

    def test_same_file_preference_over_header(self) -> None:
        """Same-file .cpp body beats a header body elsewhere."""
        _write_tree(
            self.root,
            header_text="#pragma once\n" + SLLIST_BODY,
            def_text=_def_with_body_in_same_file(SLLIST_BODY),
        )
        bodies = _find_class_bodies(self.root, CLS)
        self.assertEqual(len(bodies), 2)
        plan = plan_function_rename(self.root, OLD, f"{CLS}::{METHOD}(CSchedule*)")
        self.assertEqual(len(plan.header_edits), 1)
        # Same-file (Def.cpp) wins over the header.
        self.assertTrue(str(plan.header_edits[0].path).endswith("Def.cpp"))

    def test_header_preferred_over_different_tu_cpp(self) -> None:
        """Single header body beats a .cpp body in an unrelated TU."""
        _write_tree(
            self.root,
            header_text="#pragma once\n" + SLLIST_BODY,
            def_text=(
                '#include "Owner.hpp"\n'
                f'extern "C" void {OLD}({SELF_TYPE}* self, {EXTRA});\n'
                f'void {OLD}({SELF_TYPE}* self, {EXTRA}) {{\n'
                '    (void)self; (void)*x;\n'
                '}\n'
            ),
            extra_files={"src/other/Other.cpp": SLLIST_BODY},
        )
        bodies = _find_class_bodies(self.root, CLS)
        self.assertEqual(len(bodies), 2)
        plan = plan_function_rename(self.root, OLD, f"{CLS}::{METHOD}(CSchedule*)")
        self.assertEqual(len(plan.header_edits), 1)
        self.assertTrue(str(plan.header_edits[0].path).endswith("Owner.hpp"))

    def test_different_tu_cpp_used_when_unique(self) -> None:
        """A .cpp body in a different TU is used iff it is unambiguous."""
        _write_tree(
            self.root,
            header_text="#pragma once\nstruct Other {};\n",
            def_text=(
                '#include "Owner.hpp"\n'
                f'extern "C" void {OLD}({SELF_TYPE}* self, {EXTRA});\n'
                f'void {OLD}({SELF_TYPE}* self, {EXTRA}) {{\n'
                '    (void)self; (void)*x;\n'
                '}\n'
            ),
            extra_files={"src/other/Other.cpp": SLLIST_BODY},
        )
        bodies = _find_class_bodies(self.root, CLS)
        self.assertEqual(len(bodies), 1)
        self.assertTrue(str(bodies[0][0]).endswith("Other.cpp"))
        plan = plan_function_rename(self.root, OLD, f"{CLS}::{METHOD}(CSchedule*)")
        self.assertEqual(len(plan.header_edits), 1)
        self.assertTrue(str(plan.header_edits[0].path).endswith("Other.cpp"))
        # And with the demote flag the plan is fully clean.
        clean = plan_function_rename(
            self.root, OLD, f"{CLS}::{METHOD}(CSchedule*)", demote_import_band=True
        )
        self.assertEqual(clean.reviews, [])
        self.assertGreater(len(clean.demotions), 0)

    def test_cpp_body_apply_inserts_and_idempotent(self) -> None:
        """Apply inserts into the .cpp body exactly once; re-run is clean."""
        _write_tree(
            self.root,
            header_text="#pragma once\nclass SLList;\n",
            def_text=_def_with_body_in_same_file(SLLIST_BODY),
        )
        deff = self.root / "src" / "unit" / "Def.cpp"
        before_lines = len(deff.read_text(encoding="utf-8").splitlines())
        plan, _log = apply_function_rename(
            self.root, OLD, f"{CLS}::{METHOD}(CSchedule*)",
            regions=["us"], dry_run=False, allow_review=True,
        )
        self.assertEqual(len(plan.header_edits), 1)
        after = deff.read_text(encoding="utf-8")
        # Header insert adds exactly one line to the owning .cpp.
        self.assertEqual(len(after.splitlines()), before_lines + 1)
        self.assertEqual(after.count("void DropItem(CSchedule* x);"), 1)
        self.assertIn("void SLList::DropItem(CSchedule* x) {", after)
        # Old symbol is gone: re-planning refuses instead of duplicating.
        with self.assertRaises(FnRenameRefusal):
            plan_function_rename(self.root, OLD, f"{CLS}::{METHOD}(CSchedule*)")
        self.assertEqual(deff.read_text(encoding="utf-8").count("void DropItem(CSchedule* x);"), 1)
        # Fresh fixture whose .cpp already declares the method reports
        # already_present and apply does not duplicate.
        _write_tree(
            self.root,
            header_text="#pragma once\nclass SLList;\n",
            def_text=(
                '#include "Owner.hpp"\n'
                "class SLList {\n"
                "public:\n"
                "    void Existing();\n"
                "    void DropItem(CSchedule* x);\n"
                "};\n"
                f'extern "C" void {OLD}({SELF_TYPE}* self, {EXTRA});\n'
                f'void {OLD}({SELF_TYPE}* self, {EXTRA}) {{\n'
                '    (void)self; (void)*x;\n'
                '}\n'
            ),
        )
        plan3 = plan_function_rename(self.root, OLD, f"{CLS}::{METHOD}(CSchedule*)")
        self.assertTrue(plan3.header_edits[0].already_present)
        apply_function_rename(
            self.root, OLD, f"{CLS}::{METHOD}(CSchedule*)",
            regions=["us"], dry_run=False, allow_review=True,
        )
        final = (self.root / "src" / "unit" / "Def.cpp").read_text(encoding="utf-8")
        self.assertEqual(final.count("void DropItem(CSchedule* x);"), 1)

    def test_comment_class_word_is_not_a_body(self) -> None:
        """A '// ... class ...' comment plus 'Leaf* self' is not a body."""
        _write_tree(
            self.root,
            header_text="#pragma once\nstruct Other {};\n",
            def_text=(
                '#include "Owner.hpp"\n'
                "class SLList {\n"
                "public:\n"
                "    void Existing();\n"
                "};\n"
                "// 4-insn rotation below is the closest reachable shape (wall class #11).\n"
                f'extern "C" void {OLD}({SELF_TYPE}* self, {EXTRA});\n'
                f'void {OLD}({SELF_TYPE}* self, {EXTRA}) {{\n'
                '    (void)self; (void)*x;\n'
                '}\n'
            ),
        )
        bodies = _find_class_bodies(self.root, CLS)
        self.assertEqual(len(bodies), 1)
        plan = plan_function_rename(self.root, OLD, f"{CLS}::{METHOD}(CSchedule*)")
        self.assertEqual(len(plan.header_edits), 1)
        self.assertTrue(str(plan.header_edits[0].path).endswith("Def.cpp"))

    def test_no_header_insert_unchanged_with_cpp_body(self) -> None:
        """--no-header-insert still skips the insert when the body is in .cpp."""
        _write_tree(
            self.root,
            header_text="#pragma once\nstruct Other {};\n",
            def_text=_def_with_body_in_same_file(SLLIST_BODY),
        )
        plan = plan_function_rename(
            self.root, OLD, f"{CLS}::{METHOD}(CSchedule*)", no_header_insert=True
        )
        self.assertEqual(plan.header_edits, [])
        self.assertTrue(any("--no-header-insert" in n for n in plan.header_notes))
        # The same shape without the flag finds the .cpp body.
        plan2 = plan_function_rename(self.root, OLD, f"{CLS}::{METHOD}(CSchedule*)")
        self.assertEqual(len(plan2.header_edits), 1)


if __name__ == "__main__":
    unittest.main()
