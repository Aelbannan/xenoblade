"""Pure-logic tests for tools/coop/faithful.py and section_owners.py."""
import io
import json
import sys
import tempfile
import unittest
from contextlib import redirect_stdout
from pathlib import Path
from unittest import mock

sys.path.insert(0, str(Path(__file__).resolve().parents[3]))

from tools.coop import faithful, section_owners  # noqa: E402


class SmellCounts(unittest.TestCase):
    def test_comments_do_not_count(self):
        hard, _ = faithful.count_smells("// lbl_eu_80666974 lives here\n/* (u8*)p + 0x10 */\nint x;\n")
        self.assertEqual(sum(hard.values()), 0)

    def test_hard_smells(self):
        src = (
            'extern "C" int initEnemyBattleState__Q22cf11CfObjectEneFv(cf::CfObjectEne* self);\n'
            "f32 a = lbl_eu_8066696C;\n"
            "u8* region = (u8*)self + 0x44A8;\n"
            "((cf::CfObjectAt3E9C*)self)->setPointEnabled(1);\n"
            '__declspec(section ".sdata2") const float f = 1.0f;\n'
        )
        hard, _ = faithful.count_smells(src)
        self.assertEqual(hard, {"label": 1, "mangled_c": 1, "section": 1, "offset_cast": 1, "view_struct": 1,
                               "noinline": 0, "decl_shield": 0, "self_alias": 0, "volatile": 0})

    def test_view_struct_ignores_real_retail_classes(self):
        src = "CView* v = getView();\nActParamCallView* a = (ActParamCallView*)p;\n"
        hard, _ = faithful.count_smells(src)
        self.assertIn("CView", faithful.real_classes())
        self.assertEqual(hard["view_struct"], 2)

    def test_noinline_is_hard_unless_justified(self):
        src = (
            "__declspec(noinline) void a();\n"
            "#pragma dont_inline on\n"
            "#pragma auto_inline off\n"
            "__declspec(noinline) void b(); // noinline-ok: split, decl order and -inline flags tried\n"
            "__declspec(noinline) void c(); // noinline-ok:\n"
            "__attribute__((never_inline)) void d();\n"
            "DECOMP_DONT_INLINE void e();\n"
            "DECOMP_DONT_INLINE void f(); // noinline-ok: TU boundary and -ipa off tried\n"
        )
        hard, soft = faithful.count_smells(src)
        self.assertEqual(hard["noinline"], 6)
        self.assertNotIn("noinline", soft)

    def test_volatile_reread_is_hard_unless_justified(self):
        src = (
            "s16 a = *(volatile s16*)&self->unk335A;\n"
            "*(volatile u32*)&WGPIPE = 1;\n"
            "*(volatile u8*)0xCC008000 = 0x61;\n"
            "u32 b = *(volatile u32*)&mFront; // volatile-ok: written by the DVD callback\n"
        )
        hard, _ = faithful.count_smells(src)
        self.assertEqual(hard["volatile"], 1)

    def test_self_alias_is_hard(self):
        src = (
            "    u32 savedRetStore;\n"
            "    u32& savedRet = savedRetStore;\n"
            "    int resultStore;\n"
            "    int& result = resultStore;\n"
            "    void* ptrStore;\n"
            "    void*& ptr = ptrStore;\n"
            "    u32 macroStore; \\\n"
            "    u32& macroRef = macroStore; \\\n"
        )
        hard, soft = faithful.count_smells(src)
        self.assertEqual(hard["self_alias"], 4)
        self.assertNotIn("self_alias", soft)

    def test_self_alias_ignores_real_aliases(self):
        src = (
            "    u32 a;\n"
            "    u32& b = other;\n"                  # different initializer
            "    u32 c;\n"
            "    u32 d = c;\n"                       # value copy, not a ref
            "    volatile u32 e;\n"
            "    u32& f = e;\n"                      # genuinely volatile object
            "    static u32 g;\n"
            "    u32& h = g;\n"                      # not a plain local
            "    u32 i;\n"
            "\n"
            "    u32& j = i;\n"                      # not the immediately prior line
            "    u32 k;\n"
            "        u32& l = k;\n"                   # different scope/indent
            "    u32 m;\n"
            "    const u32& n = m;\n"                 # const ref to a plain local
        )
        hard, _ = faithful.count_smells(src)
        self.assertEqual(hard["self_alias"], 0)

    def test_decl_shield(self):
        src = (
            "#define Scn_FindCamItem Scn_FindCamItem_hidden\n"
            "#define CItem_slotCnt CItem_slotCnt_citem_dup\n"
            "#define gxHeapSize gxHeapSize__9CDeviceGX\n"

            "#define MAX_SLOTS 8\n"
        )
        hard, _ = faithful.count_smells(src)
        self.assertEqual(hard["decl_shield"], 3)

    def test_plain_extern_c_is_not_mangled(self):
        hard, _ = faithful.count_smells('extern "C" void OSReport(const char* fmt, ...);\n')
        self.assertEqual(hard["mangled_c"], 0)

    def test_soft_smells_do_not_gate(self):
        hard, soft = faithful.count_smells("self->field_0x10 = func_80123456();\n")
        self.assertEqual(sum(hard.values()), 0)
        self.assertEqual(soft["field_0x"], 1)
        self.assertEqual(soft["addr_func"], 1)


class ReportOnlyGaps(unittest.TestCase):
    """New smell kinds size debt without touching the hard score/baseline."""

    def test_volatile_declarations_and_pointer_spellings(self):
        src = (
            "volatile int savedRet;\n"                       # object declaration
            "volatile u32* fp = &self->unk68;\n"               # pointer declaration
            "u32 v = *(s16 volatile*)&self->unk335A;\n"        # suffix cast
            "cf::Thing* volatile* q = &p;\n"                   # volatile pointer
            "*(volatile u32*)&WGPIPE = 1;\n"                   # hardware FIFO
            "*(volatile u8*)0xCC008000 = 0x61;\n"              # hardware address
            "volatile u32 keep = 0; // volatile-ok: DVD callback writes it\n"
            "// volatile f32 only mentioned in a comment\n"
        )
        hard, soft = faithful.count_smells(src)
        self.assertEqual(hard["volatile"], 0)
        self.assertEqual(soft["volatile"], 4)
        self.assertEqual(sorted(faithful.HARD), sorted([
            "label", "mangled_c", "section", "offset_cast", "view_struct",
            "noinline", "decl_shield", "self_alias", "volatile"]))

    def test_hard_volatile_still_sees_cast_rereads(self):
        hard, soft = faithful.count_smells(
            "u32 g = *(volatile u32*)&gmGameFlags;\nvolatile u32 gmGameFlags;\n")
        self.assertEqual(hard["volatile"], 1)
        self.assertEqual(soft["volatile"], 2)

    def test_named_blob_offset_reads(self):
        src = (
            "char* a = lbl_eu_8050B00C + 0x58;\n"
            "char* b = &sMenuEnemyNamePool[0x24];\n"
            "char* c = &pool[0x10];\n"
            "int n = count + 0x10;\n"                 # not a named blob
            "// lbl_eu_8050B00C + 0x58 in a comment\n"
        )
        hard, soft = faithful.count_smells(src)
        self.assertEqual(hard["offset_cast"], 0)
        self.assertEqual(soft["blob_offset"], 3)

    def test_named_sdata2_extern_reads(self):
        src = (
            "extern const float lbl_eu_80668800;\n"
            'extern "C" const double lbl_eu_80668818;\n'
            'extern "C" { extern const float lbl_eu_80668804; }\n'
            "extern \"C\" {\n"
            "float lbl_eu_8066880C;\n"
            "}\n"
            "extern const f32 sPool[];\n"            # array pool, not a scalar
            "extern void func();\n"
            "float local = 1.0f;\n"                 # not extern
        )
        hard, soft = faithful.count_smells(src)
        self.assertEqual(hard["label"], 4)  # already a hard smell; unchanged
        self.assertEqual(hard["mangled_c"], 0)
        self.assertEqual(soft["sdata2_extern"], 4)


class FinishLine(unittest.TestCase):
    """PLAN_TO_DONE §1 census: wider name counters + reviewed markers."""

    def test_address_named_function_declarators(self):
        src = (
            'extern "C" void func_80123456(void* self);\n'
            "void __ct__801C18F4(void* self) {}\n"
            "void create_8045F858();\n"          # *_80xxxxxx-style name
            "void caller() { func_80123456(0); }\n"  # a use, not a name debt
            "extern const float lbl_eu_80668800;\n"  # data label
        )
        totals, _exs, _g, _v = faithful.count_finish_line_file(src, "src/T.cpp")
        self.assertEqual(totals["soft/addr_func"], 3)
        self.assertFalse(faithful.is_addr_func_name("lbl_eu_80668800"))
        self.assertFalse(faithful.is_addr_func_name("code_800B06A4"))
        self.assertFalse(faithful.is_addr_func_name("__vt__801C18F4"))
        self.assertFalse(faithful.is_addr_func_name("UnkClass_8045F564"))
        self.assertTrue(faithful.is_addr_func_name("create_8045F858"))

    def test_name_counters_and_markers(self):
        src = (
            "UnkClass_800821F8* u = 0;\n"
            "UnkVirtualFunc1 v;\n"
            "unk_3E4 n;\n"
            "u32 a = self->field_0x10;\n"
            "u32 b = self->field_0x10; // name-guess: only written by setAnchor()\n"
            "extern const float lbl_eu_80668804; // faithful-ok: SDK pool owner\n"
            "// UnkClass_800821F8 in a comment\n"
        )
        totals, exs, guesses, volatile_exempt = faithful.count_finish_line_file(
            src, "src/T.hpp")
        self.assertEqual(totals["soft/unk_name"], 3)
        self.assertEqual(totals["soft/field_0x"], 1)
        self.assertEqual(totals["hard/label"], 0)  # exempted faithful-ok line

        self.assertEqual(guesses, 1)
        self.assertEqual(volatile_exempt, 0)
        self.assertEqual(exs, [("src/T.hpp", 6, "faithful-ok", "SDK pool owner")])

    def test_volatile_exemptions_still_apply(self):
        totals, _exs, _g, vok = faithful.count_finish_line_file(
            "volatile int shared; // volatile-ok: DVD callback writes it\n"
            "volatile int other;\n"
            "u32 x = *(volatile u32*)&obj;\n", "src/T.cpp")
        self.assertEqual(totals["hard/volatile"], 1)
        self.assertEqual(totals["soft/volatile"], 2)
        self.assertEqual(vok, 1)

    def test_marker_census(self):
        src = ("// faithful-ok: pool owned by another TU\n"
               "u32 x; // name-guess: written only by setAnchor()\n"
               "volatile u32 y; // volatile-ok: callback writes it\n")
        exs, guesses, vok = faithful.finish_line_markers(src, "include/T.hpp")
        self.assertEqual(exs, [("include/T.hpp", 1, "faithful-ok",
                                "pool owned by another TU")])
        self.assertEqual((guesses, vok), (1, 1))

    def test_report_lists_every_section_1_category(self):
        totals = faithful.collections.Counter({"hard/label": 7})
        lines = faithful.finish_line_report(
            "us", totals, [("src/T.cpp", 12, "faithful-ok", "split boundary")], 2, 3)
        report = "\n".join(lines)
        for label, _kind, _key in faithful.FINISH_CATEGORIES:
            self.assertIn(label, report)
        self.assertIn("current", report)
        self.assertIn("target", report)
        self.assertIn("status", report)
        self.assertIn("OPEN", report)
        self.assertIn("DONE", report)
        self.assertIn("src/T.cpp:12: split boundary", report)
        self.assertIn("guessed names (// name-guess:): 2", report)
        self.assertIn("volatile definitions (// volatile-ok:): 3", report)

    def test_finish_line_flag_never_writes_baseline(self):
        ap = faithful.build_parser()
        self.assertTrue(ap.parse_args(["--finish-line"]).finish_line)
        self.assertFalse(ap.parse_args([]).finish_line)
        for bad in ("--ratchet", "--init"):
            with mock.patch.object(sys, "argv", ["faithful.py", "--finish-line", bad]):
                with self.assertRaises(SystemExit):
                    faithful.main()

    def test_sdata2_extern_line_counts_preserve_linkage_blocks(self):
        code = ('extern "C" {\nfloat a;\n}\n'
                'extern "C" { float b; }\nextern float c;\n')
        per_line = faithful.sdata2_extern_line_counts(code)
        self.assertEqual(per_line, [0, 1, 0, 1, 1])
        self.assertEqual(faithful.count_sdata2_externs(code), sum(per_line))


class UnpairedHeaders(unittest.TestCase):
    """Headers with no paired .cpp are pseudo-units, not invisible debt."""

    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name)
        self.addCleanup(mock.patch.object(faithful, "ROOT", self.root).stop)
        mock.patch.object(faithful, "ROOT", self.root).start()
        files = {
            "src/A.cpp": "int a;\n",
            "src/A.hpp": "int a;\n",
            "src/SideImports.hpp": "extern float lbl_eu_80666974;\n"
                                   "int n = self->field_0x10;\n",
            "include/Other.hpp": "int other;\n",
            "libs/monolib/M.hpp": "int m;\n",
            "libs/RVL_SDK/include/vendor.h": "extern float lbl_eu_80666974;\n",
        }
        for rel, text in files.items():
            path = self.root / rel
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(text)
        self.res = {
            "unitA": {
                "source": "src/A.cpp",
                "header": "src/A.hpp",
                "complete": True,
                "hard": dict.fromkeys(faithful.HARD, 0) | {
                    "rule": 0, "rtti_off": 0, "foreign_data": 0},
                "soft": dict.fromkeys(faithful.SOFT, 0),
                "score": 0,
            },
        }

    def test_pairing_detection_skips_vendor_headers(self):
        self.assertEqual(faithful.paired_header_paths(self.res), {"src/A.hpp"})
        rels = [p.relative_to(self.root).as_posix()
                for p in faithful.unpaired_header_paths(self.res)]
        self.assertEqual(rels, ["src/SideImports.hpp", "include/Other.hpp",
                                "libs/monolib/M.hpp"])
        self.assertNotIn("libs/RVL_SDK/include/vendor.h", rels)

    def test_pseudo_unit_counting(self):
        headers = faithful.measure_unpaired_headers(self.res)
        self.assertEqual(set(headers), {
            "hdr/src/SideImports.hpp", "hdr/include/Other.hpp",
            "hdr/libs/monolib/M.hpp"})
        row = headers["hdr/src/SideImports.hpp"]
        self.assertEqual(row["source"], "src/SideImports.hpp")
        self.assertEqual(row["hard"]["label"], 1)
        self.assertEqual(row["soft"]["field_0x"], 1)
        self.assertEqual(row["score"], 1)
        self.assertNotIn("hdr/src/A.hpp", headers)

    def test_finish_line_totals_include_unpaired_headers(self):
        with mock.patch.object(faithful, "measure", return_value=self.res):
            out = io.StringIO()
            with redirect_stdout(out):
                rc = faithful.finish_line("us")
        self.assertEqual(rc, 0)
        lines = out.getvalue().splitlines()
        for cat in ("label", "field_0x"):
            for i, line in enumerate(lines):
                if line.startswith(f"{cat:26} "):
                    self.assertEqual(lines[i + 1].strip(),
                                     "of which unpaired headers: 1")
                    break
            else:
                self.fail(f"no finish-line row for {cat}")

    def test_unbaselined_header_warns_but_does_not_fail(self):
        baseline = self.root / "baseline.json"
        baseline.write_text(json.dumps({"region": "us", "units": {}}))
        with mock.patch.object(faithful, "BASELINE", baseline),              mock.patch.object(faithful, "measure", return_value=self.res):
            out = io.StringIO()
            with redirect_stdout(out):
                rc = faithful.gate("us", False, False, False)
        text = out.getvalue()
        self.assertEqual(rc, 0)
        self.assertIn("WARN: unbaselined header hdr/src/SideImports.hpp", text)
        self.assertNotIn("FAIL:", text)
        self.assertIn("label 1", text)  # the pseudo-unit is in the totals

    def test_baselined_header_rise_fails(self):
        baseline = self.root / "baseline.json"
        baseline.write_text(json.dumps({
            "region": "us",
            "units": {"hdr/src/SideImports.hpp": 0},
        }))
        with mock.patch.object(faithful, "BASELINE", baseline), \
             mock.patch.object(faithful, "measure", return_value=self.res):
            out = io.StringIO()
            with redirect_stdout(out):
                rc = faithful.gate("us", False, False, False)
        self.assertEqual(rc, 1)
        self.assertIn("FAIL: faithfulness score rose in "
                      "hdr/src/SideImports.hpp: 0 -> 1", out.getvalue())

    def test_deleted_header_baseline_entry_disappears(self):
        baseline = self.root / "baseline.json"
        baseline.write_text(json.dumps({
            "region": "us",
            "units": {"hdr/src/Deleted.hpp": 5},
        }))
        with mock.patch.object(faithful, "BASELINE", baseline), \
             mock.patch.object(faithful, "measure", return_value=self.res):
            out = io.StringIO()
            with redirect_stdout(out):
                rc = faithful.gate("us", False, False, False)
        text = out.getvalue()
        self.assertEqual(rc, 0)
        self.assertNotIn("Deleted.hpp", text)
        self.assertNotIn("improved", text)

    def test_seed_headers_only_adds_missing_hdr_keys(self):
        baseline = self.root / "baseline.json"
        original = {
            "region": "us",
            "units": {
                "unitA": 7,
                "hdr/src/SideImports.hpp": 99,  # existing entry: must not change
                "hdr/src/Deleted.hpp": 3,       # deleted header: must not change
            },
        }
        baseline.write_text(json.dumps(original, indent=1))
        with mock.patch.object(faithful, "BASELINE", baseline),              mock.patch.object(faithful, "measure", return_value=self.res):
            with redirect_stdout(io.StringIO()):
                rc = faithful.seed_headers("us")
        self.assertEqual(rc, 0)
        units = json.loads(baseline.read_text())["units"]
        self.assertEqual(units["unitA"], 7)
        self.assertEqual(units["hdr/src/SideImports.hpp"], 99)
        self.assertEqual(units["hdr/src/Deleted.hpp"], 3)
        self.assertEqual(units["hdr/include/Other.hpp"], 0)
        self.assertEqual(units["hdr/libs/monolib/M.hpp"], 0)
        self.assertNotIn("hdr/src/A.hpp", units)
        self.assertTrue(faithful.build_parser().parse_args(
            ["--seed-headers"]).seed_headers)


class Gate(unittest.TestCase):
    def test_compare(self):
        fails, better = faithful.compare({"a": 3, "b": 2}, {"a": 4, "c": 1})

        self.assertEqual(better, ["b: 2 -> 0"])


class RttiOff(unittest.TestCase):
    def test_pattern_reads_configure_lines(self):
        line = 'Object(Matching, "monolib/src/core/CProc.cpp", extra_cflags=["-RTTI off"]),  # note\n'
        other = 'Object(Matching, "monolib/src/core/CProcRoot.cpp"),\n'
        self.assertEqual(faithful.RTTI_OFF.findall(line + other), ["monolib/src/core/CProc.cpp"])


class Tiers(unittest.TestCase):
    META = {0x10: ("lbl_eu_00000010", "type:object size:0x4 data:float"),
            0x20: ("huge__2ml", "type:object size:0x4 data:float"),
            0x30: ("lbl_eu_00000030", "type:object size:0x8"),
            0x40: ("lbl_eu_00000040", "type:object size:0x5 data:string")}

    def test_named_symbols_are_never_evidence(self):
        row = {"addr": 0x20, "kind": "text"}
        self.assertEqual(section_owners.tier(".sdata2", row, self.META), "C")

    def test_anonymous_literals_and_strings_are_tier_a(self):
        self.assertEqual(section_owners.tier(".sdata2", {"addr": 0x10, "kind": "text"}, self.META), "A")
        self.assertEqual(section_owners.tier(".rodata", {"addr": 0x40, "kind": "text"}, self.META), "A")

    def test_other_anonymous_data_is_tier_b(self):
        self.assertEqual(section_owners.tier(".data", {"addr": 0x30, "kind": "text"}, self.META), "B")

    def test_data_only_votes_have_no_tier(self):
        self.assertIsNone(section_owners.tier(".data", {"addr": 0x30, "kind": "data"}, self.META))


class LinkOrder(unittest.TestCase):
    def test_consistent_drops_out_of_order_vote(self):
        # unit indices by address; the lone 9 between 2s contradicts link order
        keep = section_owners.consistent([(0, 1), (1, 2), (2, 9), (3, 2), (4, 3)])
        self.assertEqual(keep, [True, True, False, True, True])

    def test_consistent_empty(self):
        self.assertEqual(section_owners.consistent([]), [])


if __name__ == "__main__":
    unittest.main()
