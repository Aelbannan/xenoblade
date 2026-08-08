from __future__ import annotations

import tempfile
import unittest
from pathlib import Path

from tools.coop import member_check as mc


def _mk_asm(body: str) -> mc.AsmIndex:
    """Build an AsmIndex from a fake dtk asm file."""
    with tempfile.TemporaryDirectory() as td:
        d = Path(td) / "asm"
        d.mkdir()
        (d / "test.s").write_text(body)
        return mc.AsmIndex(d)


F = "/* 80070000 00000000  00 00 00 00 */\t"


def fn(name: str, insns: str) -> str:
    return f".fn {name}, global\n{insns}.endfn {name}\n"


class ProvenanceTest(unittest.TestCase):
    def test_constant_li(self):
        idx = _mk_asm(
            fn("caller", f"{F}li r3, 0xe\n{F}li r4, 0x1\n{F}bl target_free\n")
            + fn("target_free", f"{F}blr\n")
        )
        kind, detail = mc.r3_provenance(idx.files["test.s"], 2)
        self.assertEqual(kind, "CONSTANT")

    def test_constant_lis(self):
        idx = _mk_asm(
            fn("caller", f"{F}lis r3, 0x4000\n{F}bl target_free\n")
            + fn("target_free", f"{F}blr\n")
        )
        kind, _ = mc.r3_provenance(idx.files["test.s"], 1)
        self.assertEqual(kind, "CONSTANT")

    def test_stack_addr(self):
        idx = _mk_asm(
            fn("caller", f"{F}addi r3, r1, 0x18\n{F}bl target\n")
            + fn("target", f"{F}blr\n")
        )
        kind, _ = mc.r3_provenance(idx.files["test.s"], 1)
        self.assertEqual(kind, "ADDR_STACK")

    def test_label_addr(self):
        idx = _mk_asm(
            fn("caller", f"{F}lis r5, lbl_eu_8052AC98@ha\n{F}addi r3, r5, lbl_eu_8052AC98@l\n{F}bl target\n")
            + fn("target", f"{F}blr\n")
        )
        kind, _ = mc.r3_provenance(idx.files["test.s"], 2)
        self.assertEqual(kind, "ADDR_LABEL")

    def test_copy_chase_to_constant(self):
        idx = _mk_asm(
            fn("caller", f"{F}li r31, 0x1000\n{F}mr r3, r31\n{F}bl target\n")
            + fn("target", f"{F}blr\n")
        )
        kind, _ = mc.r3_provenance(idx.files["test.s"], 2)
        self.assertEqual(kind, "CONSTANT")


class CalleeUsageTest(unittest.TestCase):
    def test_deref_and_max_offset(self):
        idx = _mk_asm(
            fn("f", f"{F}lwz r4, 0x888(r3)\n{F}blr\n")
        )
        u = mc.callee_r3_usage(idx.fn_range["f"][0] and idx.files["test.s"][idx.fn_range["f"][1]:idx.fn_range["f"][2]])
        self.assertTrue(u["deref"])
        self.assertEqual(u["deref_max_offset"], 0x888)

    def test_integer_only(self):
        idx = _mk_asm(
            fn("f", f"{F}slwi r3, r3, 27\n{F}or r3, r3, r0\n{F}blr\n")
        )
        u = mc.callee_r3_usage(idx.files["test.s"][idx.fn_range["f"][1]:idx.fn_range["f"][2]])
        self.assertTrue(u["integer_only"])

    def test_vtable_dispatch(self):
        idx = _mk_asm(
            fn("f", f"{F}lwz r12, 0x0(r3)\n{F}lwz r12, 0x14(r12)\n{F}mtctr r12\n{F}bctrl\n")
        )
        u = mc.callee_r3_usage(idx.files["test.s"][idx.fn_range["f"][1]:idx.fn_range["f"][2]])
        self.assertTrue(u["vtable_dispatch"])


class VerdictTest(unittest.TestCase):
    def test_n1_constant(self):
        idx = _mk_asm(
            fn("caller", f"{F}li r3, 0xe\n{F}bl target\n")
            + fn("target", f"{F}blr\n")
        )
        r = mc.classify_symbol("target", idx)
        self.assertIn("NOT non-static member", r["verdict"])

    def test_n3_layout(self):
        idx = _mk_asm(
            fn("caller", f"{F}mr r3, r31\n{F}bl target\n")
            + fn("target", f"{F}lwz r4, 0x888(r3)\n{F}blr\n")
        )
        r = mc.classify_symbol("target", idx, class_size=0xB8)
        self.assertIn("N3", r["verdict"])

    def test_instance_anchored_possible_member(self):
        idx = _mk_asm(
            fn("caller", f"{F}lis r5, lbl_eu_80571758@ha\n{F}addi r3, r5, lbl_eu_80571758@l\n{F}bl target\n")
            + fn("target", f"{F}stw r0, 0x28(r3)\n{F}blr\n")
        )
        r = mc.classify_symbol("target", idx)
        self.assertIn("possible member", r["verdict"])

    def test_undecidable_tier(self):
        idx = _mk_asm(
            fn("caller", f"{F}mr r3, r31\n{F}bl target\n")
            + fn("target", f"{F}lwz r4, 0x4(r3)\n{F}blr\n")
        )
        r = mc.classify_symbol("target", idx, class_size=0xB8)
        self.assertIn("Tier B", r["verdict"])


class QualifierTest(unittest.TestCase):
    def test_parse_class(self):
        self.assertEqual(
            mc.parse_qualifier_class("func_8007EEE0__Q22cf13CfGameManagerFv"),
            "CfGameManager",
        )
        self.assertEqual(
            mc.parse_qualifier_class("func_8007DCA8__FUiUi"),
            None,
        )
        self.assertEqual(
            mc.parse_qualifier_class("getPlayer__Q22cf13CfGameManagerFi"),
            "CfGameManager",
        )


class ParamExtractionTest(unittest.TestCase):
    F = "/* 80070000 00000000  00 00 00 00 */\t"

    def _fn(self, name: str, insns: str) -> mc.AsmIndex:
        return _mk_asm(f".fn {name}, global\n{insns}.endfn {name}\n")

    def _params(self, idx: mc.AsmIndex, name: str) -> dict:
        rel, fs, fe = idx.fn_range[name]
        return mc.callee_params(idx.files[rel][fs:fe])

    def test_bitpack_two_ints(self):
        idx = self._fn("f", f"{self.F}slwi r3, r3, 27\n{self.F}slwi r0, r4, 20\n{self.F}or r3, r3, r0\n{self.F}blr\n")
        p = self._params(idx, "f")
        self.assertEqual(p["gprs"], [("r3", "int"), ("r4", "int")])

    def test_ptr_and_index(self):
        idx = self._fn("f", f"{self.F}clrlwi r0, r4, 16\n{self.F}add r3, r3, r0\n{self.F}lhz r3, 0x2(r3)\n{self.F}blr\n")
        p = self._params(idx, "f")
        self.assertEqual(p["gprs"], [("r3", "ptr"), ("r4", "int")])

    def test_bool_flag(self):
        idx = self._fn("f", f"{self.F}cmpwi r3, 0x0\n{self.F}bne .L1\n{self.F}.L1: blr\n")
        p = self._params(idx, "f")
        self.assertEqual(p["gprs"], [("r3", "bool")])

    def test_two_ptrs(self):
        idx = self._fn("f", f"{self.F}lwz r5, 0x0(r3)\n{self.F}lwz r0, 0x0(r4)\n{self.F}subf r3, r5, r0\n{self.F}blr\n")
        p = self._params(idx, "f")
        self.assertEqual(p["gprs"], [("r3", "ptr"), ("r4", "ptr")])

    def test_no_params(self):
        idx = self._fn("f", f"{self.F}lis r3, lbl_eu_80571758@ha\n{self.F}addi r3, r3, lbl_eu_80571758@l\n{self.F}blr\n")
        p = self._params(idx, "f")
        self.assertEqual(p["gprs"], [])

    def test_first_write_is_not_param(self):
        idx = self._fn("f", f"{self.F}li r3, 0x0\n{self.F}blr\n")
        p = self._params(idx, "f")
        self.assertEqual(p["gprs"], [])


class FakeMemberTest(unittest.TestCase):
    def test_detects_null_this_trick(self):
        with tempfile.TemporaryDirectory() as td:
            src = Path(td) / "src"
            src.mkdir()
            (src / "CFake.hpp").write_text("class CFake {};")
            (src / "CFake.cpp").write_text(
                "#include \"CFake.hpp\"\n"
                "void CFake::func_8007C140() {\n"
                "    if (this == nullptr) { lbl_eu_80663E24 |= 8; }\n"
                "}\n"
            )
            old_src = mc.ROOT / "src"
            # point find_class_tu at the temp tree via ROOT override
            saved = mc.ROOT
            mc.ROOT = Path(td)
            try:
                hits = mc.fake_members("CFake")
            finally:
                mc.ROOT = saved
            self.assertEqual(len(hits), 1)
            self.assertIn("this == nullptr", hits[0][2])


class HeaderDriftTest(unittest.TestCase):
    def test_drift_detection(self):
        # header says no-arg non-static member; binary proves bool param + no this
        idx = _mk_asm(
            fn("caller", f"{F}li r3, 0x1\n{F}bl func_8007C140__Q22cf13CfGameManagerFv\n")
            + fn("func_8007C140__Q22cf13CfGameManagerFv", f"{F}cmpwi r3, 0x0\n{F}blr\n")
        )
        with tempfile.TemporaryDirectory() as td:
            inc = Path(td) / "include"
            inc.mkdir(parents=True)
            (inc / "CfGameManager.hpp").write_text(
                "class CfGameManager {\npublic:\n    void func_8007C140();\n};\n"
            )
            cfg = Path(td) / "config" / "us"
            cfg.mkdir(parents=True)
            (cfg / "symbols.txt").write_text(
                "func_8007C140__Q22cf13CfGameManagerFv = .text:0x8007CADC; // type:function size:0x48 scope:global align:4\n"
            )
            saved = mc.ROOT
            mc.ROOT = Path(td)
            try:
                drifts = mc.header_drift("CfGameManager", idx)
            finally:
                mc.ROOT = saved
            self.assertEqual(len(drifts), 1)
            self.assertTrue(any("proves no-this" in d for d in drifts[0]["drift"]))
            self.assertTrue(any("binary 1" in d for d in drifts[0]["drift"]))


if __name__ == "__main__":
    unittest.main()
