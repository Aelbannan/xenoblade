"""Unit tests for tools/coop/layout_audit.py (struct layout-offset audit)."""
from __future__ import annotations

import tempfile
import unittest
from pathlib import Path

from tools.coop import layout_audit as la

REPO = Path(__file__).resolve().parent.parent.parent.parent


def scan_snippet(files: dict[str, str]):
    """Scan in-memory {relative_path: content} via a temp dir."""
    with tempfile.TemporaryDirectory() as td:
        for rel, text in files.items():
            p = Path(td) / rel
            p.parent.mkdir(parents=True, exist_ok=True)
            p.write_text(text)
        la._resolve_cache.clear()
        reg, findings, _ = la.scan_files([td], [])
        return reg, findings


def verdicts(findings, cls=None):
    if cls is not None:
        findings = [f for f in findings if f["class"] == cls]
    return [(f["verdict"], f["member"]) for f in findings]


OCUNIT_BAD = """\
#pragma once
#include <types.h>
namespace cf {
    class CObjectState {
    public:
        virtual void f();
        u32 unk4;          // 0x04
        u32 unk8;          // 0x08
        u32 unkC;          // 0x0C
    };
    //min size: 0x38
    class CObjectParam : public CObjectState {
    public:
        void* mPtr10;          // 0x10-0x13 (pointer stored at offset 0x10)
        u8 unk14[12];          // 0x14-0x1F (actually 0x1C long: BUG)
        u32 field_30;          // 0x30
        u8  unk34[4];          // 0x34..0x37
    };
}
"""


class UnderSizedTest(unittest.TestCase):
    def test_ocunit_style_hole_flagged(self):
        _reg, findings = scan_snippet({"t.hpp": OCUNIT_BAD})
        hits = [f for f in findings if f["verdict"] == "UNDER_SIZED"
                and f["member"] == "unk14"]
        self.assertEqual(len(hits), 1)
        self.assertIn("0x10", hits[0]["detail"])  # the 0x10-byte hole

    def test_fixed_layout_clean(self):
        fixed = OCUNIT_BAD.replace("u8 unk14[12];", "u8 unk14[0x30 - 0x14];")
        _reg, findings = scan_snippet({"t.hpp": fixed})
        bad = [f for f in findings if f["verdict"] in ("UNDER_SIZED", "OVERLAP")]
        self.assertEqual(bad, [])


class OverlapTest(unittest.TestCase):
    def test_overlap_flagged(self):
        _reg, findings = scan_snippet({"t.hpp": """\
struct S {
    u32 a; // 0x0
    u16 b; // 0x2
};
"""})
        # a needs 4 but b starts 2 later
        self.assertIn(("OVERLAP", "a"), verdicts(findings, "S"))

    def test_abutting_ok(self):
        _reg, findings = scan_snippet({"t.hpp": """\
struct S {
    u32 a; // 0x0
    u32 b; // 0x4
};
"""})
        self.assertEqual([f for f in findings if f["verdict"] in ("OVERLAP", "UNDER_SIZED")], [])


class MonotonicTest(unittest.TestCase):
    def test_out_of_order_flagged(self):
        _reg, findings = scan_snippet({"t.hpp": """\
struct S {
    u32 high; // 0x10
    u32 low;  // 0x04
};
"""})
        self.assertIn(("NON_MONOTONIC", "high"), verdicts(findings, "S"))

    def test_overlay_same_offset_ok(self):
        _reg, findings = scan_snippet({"t.hpp": """\
struct RView {
    u32 o1; // 0x1C4
    u32 r1; // 0x1C4
};
"""})
        self.assertEqual([f for f in findings if f["verdict"] == "NON_MONOTONIC"], [])


class ArraySizingTest(unittest.TestCase):
    def test_expr_extent(self):
        _reg, findings = scan_snippet({"t.hpp": """\
struct S {
    u8 pad[0x30 - 0x14]; // 0x14-0x2F
    u32 tail;            // 0x30
};
"""})
        bad = [f for f in findings if f["verdict"] in ("UNDER_SIZED", "OVERLAP", "EXTENT_MISMATCH")]
        self.assertEqual(bad, [])

    def test_multidim(self):
        reg, _findings = scan_snippet({"t.hpp": "struct M { f32 m[4][4]; };\n"})
        rec = reg.canonical("M")
        self.assertEqual(rec.members[0].size, 64)

    def test_extent_mismatch(self):
        _reg, findings = scan_snippet({"t.hpp": """\
struct S {
    u8 gap[0x1D4C]; // 0x100..0x143
    u32 tail;       // 0x144
};
"""})
        self.assertIn(("EXTENT_MISMATCH", "gap"), verdicts(findings, "S"))


class UnknownTypeTest(unittest.TestCase):
    def test_unknown_skipped_not_flagged(self):
        _reg, findings = scan_snippet({"t.hpp": """\
struct Opaque;
struct S {
    Opaque* p;   // 0x0
    Opaque big;  // 0x4
    u32 tail;    // 0x8
};
"""})
        # 'big' is unknown-size: no span verdicts may name it
        for f in findings:
            if f["verdict"] in ("UNDER_SIZED", "OVERLAP"):
                self.assertNotIn(f["member"], ("big", "p"))
        # 'big' is still listed with size? in the class table
        rec = _reg.canonical("S")
        big = next(m for m in rec.members if m.name == "big")
        self.assertIsNone(big.size)


class WeakNameTest(unittest.TestCase):
    def test_counter_names_ignored(self):
        _reg, findings = scan_snippet({"t.hpp": """\
struct S {
    BigType unk1;
    BigType unk2;
};
"""})
        # single-digit counter names must not make the struct "annotated"
        self.assertEqual(findings, [])


class CObjectParamCleanTest(unittest.TestCase):
    def test_now_fixed_header_clean(self):
        la._resolve_cache.clear()
        reg, findings, _files = la.scan_files(
            [str(REPO / "src" / "kyoshin" / "cf" / "object")], [])
        mine = [f for f in findings
                if f["class"] == "cf::CObjectParam"
                and "CObjectParam.hpp" in f["header"]
                and f["verdict"] not in ("MISSING_OFFSET", "ALIGN_PAD")]
        self.assertEqual(mine, [])
        # ...and the layout really is what the header claims
        rec = None
        for r in reg.structs.get("cf::CObjectParam", []):
            if "CObjectParam.hpp" in str(r.path):
                rec = r
        self.assertIsNotNone(rec)
        by_name = {m.name: m for m in rec.members}
        self.assertEqual(by_name["unk14"].size, 0x1C)
        self.assertEqual(by_name["field_30"].declared, 0x30)


if __name__ == "__main__":
    unittest.main()
