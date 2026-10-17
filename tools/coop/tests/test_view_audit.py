"""Tests for the TU-local view-struct audit (view_audit.py)."""

from __future__ import annotations

import pathlib
import sys
import unittest

ROOT = pathlib.Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT))

from tools.coop import view_audit as va  # noqa: E402


class TestParseViews(unittest.TestCase):
    def test_leading_pad_sets_base_and_offsets(self):
        text = (
            "struct FooView {\n"
            "    u8 _00[0x10];\n"
            "    u32 field_10; // 0x10\n"
            "    u16 field_14; // 0x14\n"
            "};\n"
        )
        views = va.parse_views(text, "src/x.cpp")
        self.assertEqual(len(views), 1)
        v = views[0]
        self.assertEqual((v.name, v.base), ("FooView", 0x10))
        self.assertEqual([o for o, _, _ in v.fields], [0x10, 0x14])
        self.assertEqual([n for _, _, n in v.fields], ["field_10", "field_14"])

    def test_offset_comment_overrides_cursor(self):
        text = "struct BView {\n    u8 a;\n    u8 pad[3];\n    u32 x; /* 0x8 */\n};\n"
        v = va.parse_views(text, "src/x.cpp")[0]
        self.assertEqual(v.fields[-1][0], 0x8)

    def test_pad_members_are_marked(self):
        v = va.parse_views("struct CView {\n    u8 _00[4];\n    u8 pad_04[4];\n    u32 v;\n};\n", "src/x.cpp")[0]
        self.assertEqual([n for _, _, n in v.fields], ["_pad", "v"])


class TestResolveWrapped(unittest.TestCase):
    def test_self_receiver_resolves_enclosing_class(self):
        text = "void CFoo::Bar(u32 v) {\n    ((FooView*)self)->field = v;\n}\n"
        v = va.View(name="FooView", file="src/x.cpp", line=2, base=0)
        # `self` with no self-param signature -> falls back to enclosing method class
        self.assertEqual(va.resolve_wrapped(text, v, {}), "CFoo")

    def test_local_pointer_decl_resolves(self):
        text = "void f(void) {\n    CFoo* p = get();\n    ((FooView*)p)->x = 1;\n}\n"
        v = va.View(name="FooView", file="src/x.cpp", line=3, base=0)
        self.assertEqual(va.resolve_wrapped(text, v, {"CFoo": "src/CFoo.hpp"}), "CFoo")

    def test_opaque_cast_is_none(self):
        text = "void f(void) {\n    ((FooView*)gRawBuf)->x = 1;\n}\n"
        v = va.View(name="FooView", file="src/x.cpp", line=2, base=0)
        self.assertIsNone(va.resolve_wrapped(text, v, {}))


class TestKlassMembers(unittest.TestCase):
    def test_offset_comment_members(self):
        hdr = (
            "class CFoo {\n"
            "public:\n"
            "    /* 0x00 */ u32 mVtbl;\n"
            "    /* 0x10 */ u32 mExisting;\n"
            "    u8 pad_14[4];\n"
            "};\n"
        )
        mem = va.klass_members(hdr, "CFoo")
        self.assertEqual(mem[0x0], ("u32", "mVtbl"))
        self.assertEqual(mem[0x10], ("u32", "mExisting"))


class TestScanSmoke(unittest.TestCase):
    def test_scan_returns_classified_views(self):
        views = va.scan_roots(("src/kyoshin",))
        self.assertTrue(views)
        allowed = {"opaque", "vendored", "debt", "duplicate", "unconfirmed", "unresolved", "reverted"}
        self.assertTrue(all(v.klass in allowed for v in views))


if __name__ == "__main__":
    unittest.main()
