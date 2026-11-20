"""Structural pad-overlay / view-union detection in tools/coop/faithful.py.

The regex ``view_struct`` counter only sees a use of a ``...View*`` type.  These
tests pin the whole-file pass that counts the *definitions* the regex misses,
including the finish-line/gate wiring (new soft category stays out of score;
``// faithful-ok:`` cannot exempt an overlay).
"""
import io
import json
import sys
import tempfile
import unittest
from contextlib import redirect_stdout
from pathlib import Path
from unittest import mock

sys.path.insert(0, str(Path(__file__).resolve().parents[3]))

from tools.coop import faithful as f  # noqa: E402


class PadStructs(unittest.TestCase):
    def test_leading_pads_at_large_offsets(self):
        # The CtrlObjectParam.hpp / CPartsChange.hpp shapes that used to be
        # invisible to the regex counter.
        src = """
        namespace cf {
        struct ByteE4 { u8 pad_00[0xE4]; u8 field_E4; u8 getByteE4(); };
        struct ArtsOwner { u8 pad_0000[0x436C]; CActorParam mParam; };
        struct Tail {
            u8  pad_00[0x3380];
            u32 field_34FC;
            u8  pad_3500[0x3DA0 - 0x3500];
            u32 field_3DA0;
        };
        struct PcExt {
            u8 pad_00[0xA0];
            u16 mFieldA0;
            u8 pad_A2[0x3F00 - 0xA2];
            u32 mField3F00;
        };
        }
        """
        hits = f.detect_pad_overlays(src)
        self.assertEqual([h.kind for h in hits], ["pad_struct"] * 4)
        self.assertEqual([h.name for h in hits],
                         ["ByteE4", "ArtsOwner", "Tail", "PcExt"])
        hard, soft = f.count_smells(src)
        self.assertEqual(hard["view_struct"], 4)
        self.assertEqual(soft["type_pun_union"], 0)
        self.assertEqual(f.count_pad_overlays(src), (4, 0))

    def test_pad_fraction_and_unk_byte_arrays(self):
        src = """
        struct Accessors {
            u8 pad_00[0x1E];  u16 mFlags1E;
            u8 pad_20[0x10];  void* mField30;
            u8 pad_34[0x40];  void* mField74;
            u8 pad_78[0x14];  f32 mField8C;
            u8 pad_90[4];     u32 mField94;
            u8 pad_98[6];     u16 mField9E;
        };
        struct Sub { u8 unk_00[0x10]; s16 shortArr[8]; };
        struct WordPad { u32 pad; u32 field_04; };
        """
        self.assertEqual([h.name for h in f.detect_pad_overlays(src)],
                         ["Accessors", "Sub"])

    def test_function_pointer_members_are_positioned_fields(self):
        src = """
        struct Hooks {
            u8 _pad[0x118];
            void (Thing::*hook118)();
            void (Thing::*hook124)();
        };
        """
        self.assertEqual([h.name for h in f.detect_pad_overlays(src)], ["Hooks"])

    def test_view_wording_is_a_signal_only_with_real_padding(self):
        src = """
        // Pane view onto the outer object.
        struct PaneWindow {
            u8 gap[0x40];
            u32 a; u32 b; u32 c; u32 d; u32 e; u32 f;
        };
        struct OverlayBits { u8 gap[0x20]; u32 flags; };
        struct NoPadding { u32 a; u32 b; };
        """
        names = [h.name for h in f.detect_pad_overlays(src)]
        self.assertEqual(names, ["PaneWindow", "OverlayBits"])

    def test_trailing_blob_is_storage_not_positioning(self):
        src = """
        struct Init { s16 a, b, c, d, e, f; u8 blob[192]; };
        struct Opaque { u8 pad_00[0x100]; };
        """
        self.assertEqual(f.detect_pad_overlays(src), [])
        self.assertEqual(f.count_pad_overlays(src), (0, 0))

    def test_real_classes_with_unk_gaps_are_not_flagged(self):
        src = """
        class UnkClass_8004041C {
            u8 unk0; volatile float unk4; int unk8; u32 unkC;
            u8 unk10; volatile u32 unk14; u32 unk18;
        };
        class Recovered {
            u16 field_00[7]; s16 field_0E[6];
            u8 unk_1A[2]; s16 shortArr[6];
            u8 unk_E8[0x92]; CActorParam mParam;
            u32 field_34FC; u32 field_3500;
            u8 unk_350D[0x1F]; u32 field_352C; u32 field_3530;
            u8 unk_3DA0[0x1C]; int costs[5]; u32 field_3DD0;
        };
        struct BigTable { u8 pad_00[0x20]; u32 w[0x400]; u32 count; };
        class FooView { public: virtual ~FooView(); };
        FooView* real = getView();
        """
        self.assertEqual(f.count_pad_overlays(src), (0, 0))
        hard, _ = f.count_smells(src)
        self.assertEqual(hard["view_struct"], 1)  # only the old regex hit

    def test_forward_declarations_and_macros_are_not_definitions(self):
        src = """
        struct Forward;
        union ForwardUnion;
        #define MAKE_VIEW struct Bogus { u8 pad[0x100]; u32 x; }
        int value = 1;
        """
        self.assertEqual(f.detect_pad_overlays(src), [])

    def test_array_bound_arithmetic(self):
        self.assertEqual(f._eval_int("0x3F00 - 0xA2"), 0x3F00 - 0xA2)
        self.assertEqual(f._eval_int("0x17C - 0x02"), 0x17A)
        self.assertIsNone(f._eval_int("COUNT * 4"))


class ViewUnions(unittest.TestCase):
    def test_pad_arm_with_raw_array_is_hard(self):
        src = """
        union {
            s16 arr[6];
            struct { u8 pad_1C[0x0A]; s16 field_26; };
        };
        """
        hits = f.detect_pad_overlays(src)
        self.assertEqual([h.kind for h in hits], ["union_view"])
        hard, soft = f.count_smells(src)
        self.assertEqual(hard["view_struct"], 1)
        self.assertEqual(soft["type_pun_union"], 0)

    def test_multiple_pad_arms_count_once(self):
        src = """
        union Wide {
            struct { u8 pad_00[0x40]; u32 field_40; };
            struct { u8 pad_00[0x80]; f32 field_80; };
            u8 raw[0x100];
        };
        """
        hits = f.detect_pad_overlays(src)
        self.assertEqual([(h.kind, h.name) for h in hits], [("union_view", "Wide")])

    def test_plain_type_punning_union_is_soft(self):
        src = "union CtrlObjectParamColName { u32 words[2]; u8 bytes[8]; };\n"
        hits = f.detect_pad_overlays(src)
        self.assertEqual([(h.kind, h.name) for h in hits],
                         [("type_pun_union", "CtrlObjectParamColName")])
        hard, soft = f.count_smells(src)
        self.assertEqual(hard["view_struct"], 0)
        self.assertEqual(soft["type_pun_union"], 1)

    def test_union_of_plain_structs_is_soft_not_hard(self):
        src = """
        union Pair { struct { u32 a; u16 b; } rec; u64 bits; };
        """
        hits = f.detect_pad_overlays(src)
        self.assertEqual([(h.kind, h.name) for h in hits], [("type_pun_union", "Pair")])
        hard, soft = f.count_smells(src)
        self.assertEqual(hard["view_struct"], 0)
        self.assertEqual(soft["type_pun_union"], 1)


class MarkersAndCensus(unittest.TestCase):
    def test_faithful_ok_cannot_exempt_a_structural_hit(self):
        src = """
        // faithful-ok: pretend this is fine
        struct ByteE4 { u8 pad_00[0xE4]; u8 field_E4; };
        union ColName { u32 w[2]; u8 b[8]; }; // faithful-ok: nope
        """
        totals, exceptions, _, _ = f.count_finish_line_file(src, "src/T.hpp")
        self.assertEqual(totals["hard/view_struct"], 1)
        self.assertEqual(totals["soft/type_pun_union"], 1)
        self.assertEqual(len(exceptions), 2)
        hard, soft = f.count_smells(src)
        self.assertEqual(hard["view_struct"], 1)
        self.assertEqual(soft["type_pun_union"], 1)

    def test_finish_line_counts_a_definition_once(self):
        src = """
        struct ByteE4 { u8 pad_00[0xE4]; u8 field_E4; };
        union ColName { u32 w[2]; u8 b[8]; };
        void f() { ((ByteE4View*)p)->field_E4 = 1; }
        """
        totals, _, _, _ = f.count_finish_line_file(src, "src/T.cpp")
        self.assertEqual(totals["hard/view_struct"], 2)  # regex use + definition
        self.assertEqual(totals["soft/type_pun_union"], 1)

    def test_soft_category_does_not_change_the_score(self):
        src = "union ColName { u32 w[2]; u8 b[8]; };\n"
        hard, soft = f.count_smells(src)
        self.assertEqual(sum(hard.values()), 0)
        self.assertEqual(soft["type_pun_union"], 1)
        self.assertIn(("type_pun_union", "soft", "soft/type_pun_union"),
                      f.FINISH_CATEGORIES)


class Attribution(unittest.TestCase):
    """Definitions are charged to the file that declares them."""

    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        # measure() under a temporary ROOT would otherwise poison the cached
        # real_classes() set for tests that run afterwards.
        f.real_classes.cache_clear()
        self.addCleanup(f.real_classes.cache_clear)
        self.root = Path(self.tmp.name)
        files = {
            "objdiff.json": json.dumps({"units": [{
                "name": "A", "base_path": "A",
                "metadata": {"source_path": "src/A.cpp", "complete": True},
            }]}),
            "src/A.cpp": """
                struct AOwn { u8 pad_00[0x20]; u32 field_20; };
                union APun { u32 w[2]; u8 b[8]; };
            """,
            "src/A.hpp": "struct AHead { u8 pad_00[0x40]; u32 field_40; };\n",
            "src/Side.hpp": "struct Side { u8 pad_00[0x30]; u32 field_30; };\n",
        }
        for rel, text in files.items():
            path = self.root / rel
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(text)

    def measure(self):
        from tools.coop.lib import weak_dedup as W
        import tools.postprocess_reloc_names as P
        with mock.patch.object(f, "ROOT", self.root), \
             mock.patch.object(f, "foreign_data_counts", return_value={}), \
             mock.patch.object(f, "rtti_off_units", return_value=set()), \
             mock.patch.object(W, "_layout", return_value=({}, None)), \
             mock.patch.object(W, "resolve_unit", return_value=None), \
             mock.patch.object(P, "unit_has_rules", return_value=False):
            return f.measure("us")

    def test_source_and_paired_header_counts(self):
        res = self.measure()
        self.assertEqual(set(res), {"A"})
        row = res["A"]
        self.assertEqual(row["header"], "src/A.hpp")
        self.assertEqual(row["hard"]["view_struct"], 2)
        self.assertEqual(row["soft"]["type_pun_union"], 1)
        self.assertEqual(row["score"], sum(row["hard"].values()))

    def test_unpaired_header_pseudo_unit_count(self):
        with mock.patch.object(f, "ROOT", self.root):
            headers = f.measure_unpaired_headers(self.measure())
        self.assertEqual(headers["hdr/src/Side.hpp"]["hard"]["view_struct"], 1)
        self.assertEqual(headers["hdr/src/Side.hpp"]["score"], 1)

    def test_gate_score_includes_new_hard_but_not_soft(self):
        res = self.measure()
        score = res["A"]["score"]
        baseline = self.root / "baseline.json"
        baseline.write_text(json.dumps({"region": "us", "units": {"A": score}}))
        with mock.patch.object(f, "ROOT", self.root), \
             mock.patch.object(f, "BASELINE", baseline), \
             mock.patch.object(f, "measure", return_value=res):
            out = io.StringIO()
            with redirect_stdout(out):
                rc = f.gate("us", False, False, False)
        self.assertEqual(rc, 0)
        self.assertNotIn("FAIL:", out.getvalue())
        self.assertIn("type_pun_union 1", out.getvalue())


if __name__ == "__main__":
    unittest.main()
