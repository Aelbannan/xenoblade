"""Pure-logic tests for tools/coop/faithful.py and section_owners.py."""
import sys
import unittest
from pathlib import Path

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
                               "noinline": 0})

    def test_noinline_is_hard_unless_justified(self):
        src = (
            "__declspec(noinline) void a();\n"
            "#pragma dont_inline on\n"
            "#pragma auto_inline off\n"
            "__declspec(noinline) void b(); // noinline-ok: split, decl order and -inline flags tried\n"
            "__declspec(noinline) void c(); // noinline-ok:\n"
        )
        hard, soft = faithful.count_smells(src)
        self.assertEqual(hard["noinline"], 4)
        self.assertNotIn("noinline", soft)

    def test_plain_extern_c_is_not_mangled(self):
        hard, _ = faithful.count_smells('extern "C" void OSReport(const char* fmt, ...);\n')
        self.assertEqual(hard["mangled_c"], 0)

    def test_soft_smells_do_not_gate(self):
        hard, soft = faithful.count_smells("self->field_0x10 = func_80123456();\n")
        self.assertEqual(sum(hard.values()), 0)
        self.assertEqual(soft["field_0x"], 1)
        self.assertEqual(soft["addr_func"], 1)


class Gate(unittest.TestCase):
    def test_compare(self):
        fails, better = faithful.compare({"a": 3, "b": 2}, {"a": 4, "c": 1})
        self.assertEqual(fails, ["a: 3 -> 4", "c: 0 -> 1"])
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
