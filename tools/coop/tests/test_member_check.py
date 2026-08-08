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


if __name__ == "__main__":
    unittest.main()
