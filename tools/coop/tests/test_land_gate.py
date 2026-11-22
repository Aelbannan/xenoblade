"""Unit tests for the land gate's pure layers (compare, bans, ledger, queue)."""
from __future__ import annotations

import tempfile
import unittest
from pathlib import Path

from tools.coop.land import bans, compare as cmp, ledger, runner
from tools.coop.land.snapshot import section_identical


def unit(fns=None, data=None, extab=True, hard=None):
    return {"fns": fns or {}, "data": data or {}, "extab": extab, "hard": hard or {}}


def snap(units, headers=None, build_ok=True, errors=None):
    return {"units": units, "headers": headers or {}, "build_ok": build_ok, "errors": errors or {}}


class CompareTests(unittest.TestCase):
    def test_identical_passes(self):
        s = snap({"main/a": unit({"f": "✓"}, {".data": 10}, hard={"label": 2})})
        self.assertTrue(cmp.compare(s, s).ok)

    def test_check_to_diamond_is_lost(self):
        # the CMenuEnemyState case: bytes still 100% but the mark falls to ◇
        b = snap({"main/a": unit({"f": "✓", "g": "✓"})})
        a = snap({"main/a": unit({"f": "◇", "g": "✓"})})
        v = cmp.compare(b, a)
        self.assertEqual([x.kind for x in v.violations], ["lost"])

    def test_rename_is_followed(self):
        b = snap({"main/a": unit({"old": "✓"})})
        a = snap({"main/a": unit({"new": "✓"})})
        self.assertFalse(cmp.compare(b, a).ok)
        self.assertTrue(cmp.compare(b, a, {"old": "new"}).ok)

    def test_data_move_between_units_is_conserved(self):
        # the MPFDrawBillLayTex case: 40 bytes move to CScnItemCameraNw4r by a split change
        b = snap({"main/x": unit(data={".rodata": 152}), "main/y": unit(data={})})
        a = snap({"main/x": unit(data={".rodata": 112}), "main/y": unit(data={".rodata": 40, ".sdata": 16})})
        v = cmp.compare(b, a)
        self.assertTrue(v.ok, cmp.format_verdict(v))
        self.assertTrue(any(".sdata: +16" in g for g in v.gains))

    def test_data_drop_anywhere_rejects(self):
        b = snap({"main/x": unit(data={".rodata": 152}), "main/y": unit(data={".rodata": 8})})
        a = snap({"main/x": unit(data={".rodata": 112}), "main/y": unit(data={".rodata": 8})})
        v = cmp.compare(b, a)
        self.assertEqual([x.kind for x in v.violations], ["data"])
        self.assertIn("x -40", v.violations[0].detail)

    def test_extab_regression(self):
        b = snap({"main/a": unit(extab=True)})
        a = snap({"main/a": unit(extab=False)})
        self.assertEqual([x.kind for x in cmp.compare(b, a).violations], ["extab"])
        # pre-existing mismatch staying a mismatch is not a regression
        self.assertTrue(cmp.compare(a, a).ok)

    def test_hard_rise_rejected_unless_allowed(self):
        b = snap({"main/a": unit(hard={"label": 2})})
        a = snap({"main/a": unit(hard={"label": 5})})
        v = cmp.compare(b, a)
        self.assertEqual([(x.kind, x.detail) for x in v.violations], [("hard", "label +3")])
        al = cmp.Allowance.parse("main/a hard:label +3 -- regression revert (ledger ref)")
        v = cmp.compare(b, a, allowances=[al])
        self.assertTrue(v.ok)
        self.assertEqual(v.used_allowances, [al])
        too_small = cmp.Allowance.parse("a hard:label +2 -- x")
        self.assertFalse(cmp.compare(b, a, allowances=[too_small]).ok)

    def test_allowance_needs_reason(self):
        with self.assertRaises(ValueError):
            cmp.Allowance.parse("a hard:label +1")

    def test_header_ratchet(self):
        b = snap({}, headers={"hdr/include/x.hpp": {"hard": {"ban:asm": 0}}})
        a = snap({}, headers={"hdr/include/x.hpp": {"hard": {"ban:asm": 1}}})
        self.assertEqual([x.kind for x in cmp.compare(b, a).violations], ["hard"])

    def test_build_failure_and_new_error(self):
        b = snap({"main/a": unit()})
        self.assertFalse(cmp.compare(b, snap({"main/a": unit()}, build_ok=False)).ok)
        a = snap({"main/a": unit()}, errors={"main/a": "hexdiff timeout"})
        self.assertEqual([x.kind for x in cmp.compare(b, a).violations], ["error"])
        self.assertEqual([x.kind for x in cmp.compare(b, snap({})).violations], ["missing"])

    def test_link_new_missing_rejected(self):
        lk = lambda miss: {"missing": miss, "multiply_defined_names": [], "counts": {}}
        b = snap({}); b["link"] = lk(["x"])
        a = snap({}); a["link"] = lk(["x", "lbl_eu_805231BC"])
        v = cmp.compare(b, a)
        self.assertEqual([(x.kind, x.detail) for x in v.violations], [("link", "new missing: lbl_eu_805231BC")])
        self.assertTrue(cmp.compare(a, b).ok)          # retiring a missing name is a gain
        self.assertTrue(cmp.compare(snap({}), a).ok)  # no link on one side -> skipped

    def test_unit_merge_allowance(self):
        b = snap({"main/a": unit({"f": "✓"}, hard={"label": 1}), "main/b": unit({"g": "✓"}, hard={"label": 2})})
        a = snap({"main/b": unit({"f": "✓", "g": "✓"}, hard={"label": 3})})
        self.assertEqual(sorted(x.kind for x in cmp.compare(b, a).violations), ["hard", "missing"])
        al = cmp.Allowance.parse("main/a merge:main/b 0 -- same TU: shared @stringBase0")
        v = cmp.compare(b, a, allowances=[al])
        self.assertTrue(v.ok, cmp.format_verdict(v))
        lost = snap({"main/b": unit({"f": "◇", "g": "✓"}, hard={"label": 3})})
        self.assertEqual([x.kind for x in cmp.compare(b, lost, allowances=[al]).violations], ["lost"])

    def test_gains_reported(self):
        b = snap({"main/a": unit({"f": "✗"})})
        a = snap({"main/a": unit({"f": "✓"})})
        v = cmp.compare(b, a)
        self.assertTrue(v.ok)
        self.assertEqual(v.gains, ["a: f -> ✓"])


class BanTests(unittest.TestCase):
    def test_counts_ignore_comments_and_strings(self):
        src = ('// asm(" nop") in a comment\n'
               'const char* s = "#pragma once __vt__Foo";\n'
               '#pragma once\n'
               '#pragma optimization_level 0\n'
               'asm void f() {}\n'
               'extern "C" void bar__3FooFv();\n'
               'u8 blob[4] = {1, 2, 3, 4};\n'
               '#line 12\n'
               'uintptr_t p;\n'
               'x = __lwbrx(p, 0);\n'
               'extern u8 __vt__3Foo[];\n')
        c = bans.count(src)
        self.assertEqual(c["asm"], 1)
        self.assertEqual(c["pragma"], 1)
        self.assertEqual(c["line_directive"], 1)
        self.assertEqual(c["uintptr"], 1)
        self.assertEqual(c["intrinsic"], 1)
        self.assertEqual(c["hand_vtable"], 1)
        self.assertEqual(c["premangled"], 1)
        self.assertEqual(c["byte_blob"], 1)

    def test_section_identical_parse(self):
        self.assertEqual(section_identical("100 bytes identical (align 8 vs 8) + 0x4 retail link tail padding"), 104)
        self.assertEqual(section_identical("retail size 0x48 != decomp 0x10"), 0)


class SteerBanTests(unittest.TestCase):
    """Shapes held from wave 1 that the original counters missed."""

    def hits(self, text):
        return {k: v for k, v in bans.count(text).items() if v}

    def test_scalar_pointer_alias(self):
        self.assertEqual(self.hits("void f(S* self) {\n    const u32* pFlags = &self->flags;\n}"),
                         {"scalar_ptr_alias": 1})

    def test_cast_address_alias(self):
        self.assertEqual(self.hits("void f() {\n    Vec* pMag = (Vec*)&magIn;\n}"), {"cast_addr_alias": 1})

    def test_address_named_extern(self):
        self.assertEqual(self.hits("extern const GXColorS10 DefaultBlackColor_8066B550;"), {"addr_extern": 1})
        self.assertEqual(self.hits("extern u8 lbl_eu_80661234;"), {})  # counted by faithful label

    def test_single_use_static_inline(self):
        one = "static inline bool p(E* e) { return e->f; }\nbool g() { return p(a); }"
        two = "static inline int h(int x) { return x; }\nint a() { return h(1); }\nint b() { return h(2); }"
        self.assertEqual(self.hits(one), {"single_use_inline": 1})
        self.assertEqual(self.hits(two), {})

    def test_legit_shapes_not_counted(self):
        ok = ("void f() {\n    u32* slot =\n        &obj->tbl[obj->tbl[0] * i + 1];\n"
              "    CList* list = &self->mList;\n    PSVECMag((Vec*)&diff);\n}")
        self.assertEqual(self.hits(ok), {})

    def test_unmeasured_ban_key_skipped(self):
        b = snap({"main/a": unit(hard={"ban:asm": 0})})
        b["ban_keys"] = ["asm"]
        a = snap({"main/a": unit(hard={"ban:asm": 0, "ban:addr_extern": 9})})
        self.assertTrue(cmp.compare(b, a).ok)
        b["ban_keys"] = ["asm", "addr_extern"]
        self.assertFalse(cmp.compare(b, a).ok)


class LedgerTests(unittest.TestCase):
    def test_blob_sha_matches_git(self):
        # `printf 'hello\n' | git hash-object --stdin`
        self.assertEqual(ledger.blob_sha(b"hello\n"), "ce013625030ba8dba906f756967f9e9ca394464a")

    def test_gated_paths(self):
        self.assertTrue(ledger.is_gated("src/kyoshin/a.cpp"))
        self.assertTrue(ledger.is_gated("config/us/symbols.txt"))
        self.assertTrue(ledger.is_gated("configure.py"))
        self.assertFalse(ledger.is_gated("src/kyoshin/a.ctx.c"))
        self.assertFalse(ledger.is_gated("tools/coop/hexdiff.py"))
        self.assertFalse(ledger.is_gated("docs/x.md"))

    def test_record_and_verify(self):
        with tempfile.TemporaryDirectory() as d:
            lp = Path(d) / "l.jsonl"
            self.assertEqual(ledger.record({"src/a.cpp": "aa"}, "t1", ledger=lp), 1)
            self.assertEqual(ledger.record({"src/a.cpp": "aa"}, "t2", ledger=lp), 0)
            bad = ledger.verify([("src/a.cpp", "aa"), ("src/b.cpp", "bb"), ("src/c.cpp", None),
                                 ("docs/x.md", "zz")], ledger=lp)
            self.assertEqual(bad, ["src/b.cpp"])

    def test_unledgered_lists_only_unpassed_gated(self):
        with tempfile.TemporaryDirectory() as d:
            lp = Path(d) / "l.jsonl"
            ledger.record({"src/a.cpp": "aa"}, "t", ledger=lp)
            got = ledger.unledgered({"src/a.cpp": "aa", "src/b.cpp": "bb", "docs/x.md": "cc"}, ledger=lp)
            self.assertEqual(got, ["src/b.cpp"])

    def test_tree_id_order_independent(self):
        self.assertEqual(ledger.tree_id({"a": "1", "b": "2"}), ledger.tree_id({"b": "2", "a": "1"}))


class QueueTests(unittest.TestCase):
    def test_parse_and_group(self):
        q = ("T1\told=new\t@1@0x80000010|scope:local\t\tmain/a hard:label +1 -- revert\n"
             "# comment\n"
             "T2\t\t\t\n")
        plans = runner.parse_queue(q, "/p")
        self.assertEqual([p.task for p in plans], ["T1", "T2"])
        self.assertEqual(plans[0].patch, "/p/T1.patch")
        self.assertEqual(plans[0].rename_map(), {"old": "new"})
        self.assertEqual(plans[0].allow[0].key, "label")
        runner.resolve(plans[0], "+++ src/a.cpp\n")
        runner.resolve(plans[1], "+++ src/a.cpp\n")
        self.assertIn("0x80000010", plans[0].syms)
        # same file -> different groups
        self.assertEqual(len(runner.group_plans(plans, 8)), 2)

    def test_malformed_flags(self):
        with self.assertRaises(SystemExit):
            runner.parse_queue("T\t\tnoflagbar\n", "/p")


class PackTests(unittest.TestCase):
    def test_function_source_finds_definition_not_decl(self):
        from tools.coop.land.pack import function_source
        src = ("int foo(int a);\n"
               "// doc line\n"
               "int foo(int a) {\n    if (a) { return 1; }\n    return 0;\n}\n"
               "int bar() { return 2; }\n")
        body = function_source(src, "foo__Fi")
        self.assertTrue(body.startswith("// doc line\nint foo(int a) {"))
        self.assertTrue(body.endswith("return 0;\n}"))
        self.assertNotIn("bar", body)
        self.assertEqual(function_source(src, "missing__Fv"), "")


if __name__ == "__main__":
    unittest.main()
