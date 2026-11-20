"""Linker weak-dedup view (tools/coop/lib/weak_dedup.py) on hand-built ELFs."""

from __future__ import annotations

import struct
import tempfile
import unittest
from pathlib import Path

from tools.coop.lib.data_match import check_data_sections
from tools.coop.lib import weak_dedup as W
from tools.coop.lib.weak_dedup import linked_view_for

R_PPC_ADDR32 = 1
LOCAL, GLOBAL, WEAK = 0, 1, 2
FUNC, OBJECT = 2, 1

# This unit owns 0x80001000.. ; another unit owns 0x80000000.. (where retail
# keeps the surviving copies).
RANGES = [("other/Base.cpp", ".text", 0x80000000, 0x80000100),
          ("other/Base.cpp", ".sdata", 0x80600000, 0x80600010),
          ("this/Unit.cpp", ".text", 0x80001000, 0x80001100),
          ("this/Unit.cpp", ".sdata", 0x80601000, 0x80601010)]


def build(path: Path, *, text: bytes, rodata: bytes, sdata: bytes,
          symbols: list[tuple], sdata_relocs: list[tuple[int, str]],
          text_relocs: list[tuple[int, str]] = (), force_active: tuple[str, ...] = ()) -> Path:
    """symbols: (name, section, value, size, bind, type)."""
    names = [".text", ".rodata", ".sdata", ".rela.sdata"]
    names += [".rela.text"] if text_relocs else []
    names += [".comment"] if force_active else []
    names += [".symtab", ".strtab", ".shstrtab"]
    idx = {n: i + 1 for i, n in enumerate(names)}
    shstr = b"\0" + b"".join(n.encode() + b"\0" for n in names)
    shname, off = {}, 1
    for n in names:
        shname[n] = off
        off += len(n) + 1
    strtab, syms, sym_index = b"\0", [b"\0" * 16], {}
    order = sorted(symbols, key=lambda s: s[4] != LOCAL)
    for name, sec, value, size, bind, typ in order:
        sym_index[name] = len(syms)
        syms.append(struct.pack(">IIIBBH", len(strtab), value, size, bind << 4 | typ, 0, idx[sec]))
        strtab += name.encode() + b"\0"
    rela = b"".join(struct.pack(">IIi", o, sym_index[n] << 8 | R_PPC_ADDR32, 0) for o, n in sdata_relocs)
    rela_text = b"".join(struct.pack(">IIi", o, sym_index[n] << 8 | R_PPC_ADDR32, 0) for o, n in text_relocs)
    # MWCC .comment: 0x2C header, then 8 bytes per symbol (byte 5 & 0x08 = force-active).
    comment = b"CodeWarrior".ljust(0x2C, b"\0") + b"".join(
        bytes([0, 0, 0, 4, 0, 0x08 if n in force_active else 0, 0, 0])
        for n in [None] + [name for name, *_ in order])
    blobs = {".text": text, ".rodata": rodata, ".sdata": sdata, ".rela.sdata": rela,
             ".rela.text": rela_text, ".comment": comment,
             ".symtab": b"".join(syms), ".strtab": strtab, ".shstrtab": shstr}
    body, offs = b"", {}
    for n in names:
        body += b"\0" * ((-(0x34 + len(body))) % 4)
        offs[n] = 0x34 + len(body)
        body += blobs[n]
    body += b"\0" * ((-(0x34 + len(body))) % 4)
    shdrs = [b"\0" * 40]
    for n in names:
        typ = {".symtab": 2, ".strtab": 3, ".shstrtab": 3, ".rela.sdata": 4, ".rela.text": 4}.get(n, 1)
        link = idx[".symtab"] if n.startswith(".rela") else (idx[".strtab"] if n == ".symtab" else 0)
        info = idx[n[5:]] if n.startswith(".rela") else 0
        shdrs.append(struct.pack(">IIIIIIIIII", shname[n], typ, 0, 0, offs[n], len(blobs[n]),
                                 link, info, 4, 0))
    ehdr = (b"\x7fELF\x01\x02\x01" + b"\0" * 9
            + struct.pack(">HHIIIIIHHHHHH", 1, 20, 1, 0, 0, 0x34 + len(body), 0, 0x34, 0, 0, 40,
                          len(shdrs), idx[".shstrtab"]))
    path.write_bytes(ehdr + body + b"".join(shdrs))
    return path


def unit_object(path: Path, *, weak_dtor_bind: int = WEAK, rtti_bind: int = WEAK) -> Path:
    """f (this unit) + dtor copy (0x10) in .text; RTTI {name} in .sdata whose
    name string lives in .rodata after this unit's own string."""
    return build(
        path,
        text=b"\x60\0\0\0" * 4 + b"\x4e\x80\x00\x20" * 4,
        rodata=b"Unit\0\0\0\0" + b"Base\0\0\0\0",
        sdata=b"\0" * 4 + b"\0" * 4,
        symbols=[("f", ".text", 0, 0x10, GLOBAL, FUNC),
                 ("__dt__4BaseFv", ".text", 0x10, 0x10, weak_dtor_bind, FUNC),
                 ("@1", ".rodata", 0, 5, LOCAL, OBJECT),
                 ("@2", ".rodata", 8, 5, LOCAL, OBJECT),
                 ("__RTTI__4Unit", ".sdata", 0, 4, GLOBAL, OBJECT),
                 ("__RTTI__4Base", ".sdata", 4, 4, rtti_bind, OBJECT)],
        sdata_relocs=[(0, "@1"), (4, "@2")])


class WeakDedupTest(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.d = Path(self.tmp.name)

    def tearDown(self):
        self.tmp.cleanup()

    # Retail names of what this unit really owns (so they are never link-dead).
    OWN = {"f": 0x80001000, "__RTTI__4Unit": 0x80601000, "keep": 0x80601000}

    def view(self, obj: Path, addrs: dict[str, int]):
        return linked_view_for(obj, RANGES, {**self.OWN, **addrs}, "this/Unit.cpp")

    def test_weak_copy_owned_by_other_unit_is_excluded(self):
        obj = unit_object(self.d / "u.o")
        v = self.view(obj, {"__dt__4BaseFv": 0x80000040})
        self.assertEqual(v.removed.get(".text"), 0x10)
        self.assertTrue(any("__dt__4BaseFv" in n for n in v.excluded))

    def test_weak_copy_retail_keeps_in_unsplit_object_is_excluded(self):
        # Retail address outside every split range (an unassigned auto_* object):
        # still not this unit's copy, so mwld discarded ours.
        obj = unit_object(self.d / "u.o")
        v = self.view(obj, {"__dt__4BaseFv": 0x8FFF0000})
        self.assertEqual(v.removed.get(".text"), 0x10)
        self.assertTrue(any("__dt__4BaseFv" in n and "unsplit" in n for n in v.excluded))

    def test_weak_symbol_retail_places_in_this_unit_is_kept(self):
        obj = unit_object(self.d / "u.o")
        v = self.view(obj, {"__dt__4BaseFv": 0x80001010, "__RTTI__4Base": 0x80601004})
        self.assertEqual(v.excluded, [])
        self.assertEqual(v.path, obj)

    def test_global_symbol_with_same_name_is_kept(self):
        obj = unit_object(self.d / "u.o", weak_dtor_bind=GLOBAL)
        v = self.view(obj, {"__dt__4BaseFv": 0x80000040, "__RTTI__4Base": 0x80601004})
        self.assertEqual(v.excluded, [])

    def test_unreferenced_symbol_with_no_retail_copy_is_dead_stripped(self):
        # Nothing references the dtor copy or the Base RTTI and retail has
        # neither anywhere: mwld garbage-collected them (any binding).
        for bind in (WEAK, GLOBAL, LOCAL):
            obj = unit_object(self.d / f"u{bind}.o", weak_dtor_bind=bind)
            v = self.view(obj, {})
            self.assertEqual(v.removed.get(".text"), 0x10, bind)
            self.assertTrue(any("__dt__4BaseFv" in n and "dead-strip" in n for n in v.excluded))
            self.assertFalse(any(n.startswith(".text f ") for n in v.excluded))
            # Unreferenced data with an unknown name is left alone.
            self.assertNotIn(".sdata", v.removed)
            self.assertNotIn(".rodata", v.removed)

    def test_symbol_referenced_from_retained_data_is_not_dead_stripped(self):
        obj = build(self.d / "u.o", text=b"\x60\0\0\0" * 8, rodata=b"", sdata=b"\0" * 4,
                    symbols=[("f", ".text", 0, 0x10, GLOBAL, FUNC),
                             ("helper", ".text", 0x10, 0x10, GLOBAL, FUNC),
                             ("keep", ".sdata", 0, 4, GLOBAL, OBJECT)],
                    sdata_relocs=[(0, "helper")])
        v = self.view(obj, {})
        self.assertEqual(v.excluded, [])
        self.assertEqual(v.path, obj)

    def test_unit_with_an_unspelled_retail_function_is_not_dead_stripped(self):
        # symbols.txt places "mystery" in this unit's .text but the object
        # defines no such name: an unknown decomp name may be that function
        # under another spelling, so nothing is treated as link-dead.
        obj = unit_object(self.d / "u.o")
        v = self.view(obj, {"mystery": 0x80001010})
        self.assertEqual(v.excluded, [])
        self.assertEqual(v.path, obj)

    def test_flat_spelling_of_a_retail_name_is_not_dead_stripped(self):
        obj = build(self.d / "u.o", text=b"\x60\0\0\0" * 8, rodata=b"", sdata=b"",
                    symbols=[("f", ".text", 0, 0x10, GLOBAL, FUNC),
                             ("g__3BarFv__FP3Bar", ".text", 0x10, 0x10, GLOBAL, FUNC)],
                    sdata_relocs=[])
        self.assertEqual(self.view(obj, {"g__3BarFv": 0x80001010}).excluded, [])

    def class_object(self, name: str, **kw) -> Path:
        """f (retail) + ctor/dtor of a base retail never instantiates: the ctor
        installs vtable ``vt`` (global, .sdata+4), whose slot names the dtor;
        the ctor also reads a local literal."""
        return build(self.d / name, text=b"\x60\0\0\0" * 12, rodata=b"lit\0", sdata=b"\0" * 8,
                     symbols=[("f", ".text", 0, 0x10, GLOBAL, FUNC),
                              ("ctor", ".text", 0x10, 0x10, GLOBAL, FUNC),
                              ("dtor", ".text", 0x20, 0x10, GLOBAL, FUNC),
                              ("@1", ".rodata", 0, 4, LOCAL, OBJECT),
                              ("keep", ".sdata", 0, 4, GLOBAL, OBJECT),
                              ("vt", ".sdata", 4, 4, GLOBAL, OBJECT)],
                     sdata_relocs=[(4, "dtor")], text_relocs=[(0x10, "vt"), (0x14, "@1")], **kw)

    # symbols.txt for class_object(): retail has f and keep, nothing else.
    CLASS = {"f": 0x80001000, "keep": 0x80601000}

    def class_view(self, obj: Path, addrs: dict[str, int]):
        return linked_view_for(obj, RANGES, {**self.CLASS, **addrs}, "this/Unit.cpp")

    def test_vtable_only_a_dead_ctor_installs_goes_with_its_dtor(self):
        v = self.class_view(self.class_object("u.o"), {})
        self.assertEqual(v.removed.get(".text"), 0x20)
        self.assertEqual(v.removed.get(".sdata"), 4)
        self.assertTrue(any(".sdata vt " in n for n in v.excluded))
        # Retail keeps the literals of a function it stripped.
        self.assertNotIn(".rodata", v.removed)

    def orphan_vtable_object(self, vt_name: str) -> Path:
        """f and g (both retail) + a global vtable naming g that nothing installs."""
        return build(self.d / "o.o", text=b"\x60\0\0\0" * 8, rodata=b"", sdata=b"\0" * 8,
                     symbols=[("f", ".text", 0, 0x10, GLOBAL, FUNC),
                              ("g", ".text", 0x10, 0x10, GLOBAL, FUNC),
                              ("keep", ".sdata", 0, 4, GLOBAL, OBJECT),
                              (vt_name, ".sdata", 4, 4, GLOBAL, OBJECT)],
                     sdata_relocs=[(4, "g")])

    def test_unreferenced_vtable_with_no_retail_copy_is_dropped(self):
        v = self.class_view(self.orphan_vtable_object("__vt__4Base"), {"g": 0x80001010})
        self.assertEqual(v.removed.get(".sdata"), 4)
        self.assertNotIn(".text", v.removed)

    def test_unreferenced_vtable_retail_names_is_kept(self):
        v = self.class_view(self.orphan_vtable_object("__vt__4Base"),
                            {"g": 0x80001010, "__vt__4Base": 0x80601004})
        self.assertEqual(v.excluded, [])

    def test_unreferenced_vtable_retail_may_name_differently_is_kept(self):
        v = self.class_view(self.orphan_vtable_object("__vt__4Base"),
                            {"g": 0x80001010, "lbl_80601004": 0x80601004})
        self.assertEqual(v.excluded, [])

    def test_retail_switch_table_placeholder_does_not_block_the_drop(self):
        # A jumptable_<region>_<addr> retail name stands for a compiler-anonymous @N local.
        v = self.class_view(self.orphan_vtable_object("__vt__4Base"),
                            {"g": 0x80001010, "jumptable_eu_80601004": 0x80601004})
        self.assertEqual(v.removed.get(".sdata"), 4)

    def test_unreferenced_plain_data_with_no_retail_copy_is_kept(self):
        v = self.class_view(self.orphan_vtable_object("table"), {"g": 0x80001010})
        self.assertEqual(v.excluded, [])

    def test_data_retail_may_name_differently_is_not_stripped(self):
        # symbols.txt has an unknown label in this unit's .sdata slice: "vt"
        # may be that object, so it stays and keeps its dtor alive.
        v = self.class_view(self.class_object("u.o"), {"lbl_80601004": 0x80601004})
        self.assertEqual(v.removed.get(".text"), 0x10)
        self.assertTrue(any(".text ctor " in n for n in v.excluded))
        self.assertNotIn(".sdata", v.removed)

    def test_force_active_function_is_a_root(self):
        v = self.class_view(self.class_object("u.o", force_active=("ctor",)), {})
        self.assertEqual(v.excluded, [])

    def test_unreferenced_local_data_is_left_alone(self):
        obj = build(self.d / "u.o", text=b"\x60\0\0\0" * 4, rodata=b"Unit\0\0\0\0", sdata=b"",
                    symbols=[("f", ".text", 0, 0x10, GLOBAL, FUNC),
                             ("@1", ".rodata", 0, 5, LOCAL, OBJECT)],
                    sdata_relocs=[])
        self.assertEqual(self.view(obj, {}).excluded, [])

    def test_no_retail_evidence_keeps_locals_referenced_only_by_weak_copy(self):
        obj = unit_object(self.d / "u.o")
        v = self.view(obj, {"__RTTI__4Base": 0x80600004})
        # The weak locator's own bytes go; the name string stays until retail
        # evidence says mwld did not keep it.
        self.assertEqual(v.removed.get(".sdata"), 4)
        self.assertNotIn(".rodata", v.removed)

    def test_local_rtti_name_stays_when_retail_carries_its_bytes(self):
        obj = unit_object(self.d / "u.o")
        retail = build(self.d / "r.o",
                       text=b"\x60\0\0\0" * 4 + b"\x4e\x80\x00\x20" * 4,
                       rodata=b"Unit\0\0\0\0Base\0\0\0\0", sdata=b"\0" * 4,
                       symbols=[("f", ".text", 0, 0x10, GLOBAL, FUNC),
                                ("__dt__4BaseFv", ".text", 0x10, 0x10, WEAK, FUNC),
                                ("@1", ".rodata", 0, 5, LOCAL, OBJECT),
                                ("@2", ".rodata", 8, 5, LOCAL, OBJECT),
                                ("__RTTI__4Unit", ".sdata", 0, 4, GLOBAL, OBJECT)],
                       sdata_relocs=[(0, "@1")])
        v = linked_view_for(obj, RANGES, {**self.OWN, "__RTTI__4Base": 0x80600004},
                            "this/Unit.cpp", retail_sections=W._retail_sections(retail))
        self.assertEqual(v.removed.get(".sdata"), 4)
        self.assertNotIn(".rodata", v.removed)
        self.assertTrue(check_data_sections(retail, v.path).ok)

    def test_local_data_dropped_only_where_retail_lacks_its_bytes(self):
        # Two deduplicated RTTIs name two adjacent locals; retail kept the
        # first name and the pool after the second one, but not the second
        # name.  Only the second local may be cascaded away.
        obj = build(self.d / "u.o", text=b"\x60\0\0\0" * 4,
                    rodata=b"First\0\0\0Second\0\0POOL", sdata=b"\0" * 12,
                    symbols=[("f", ".text", 0, 0x10, GLOBAL, FUNC),
                             ("keep", ".sdata", 0, 4, GLOBAL, OBJECT),
                             ("@1", ".rodata", 0, 8, LOCAL, OBJECT),
                             ("@2", ".rodata", 8, 8, LOCAL, OBJECT),
                             ("@pool", ".rodata", 0x10, 4, LOCAL, OBJECT),
                             ("__RTTI__4First", ".sdata", 4, 4, WEAK, OBJECT),
                             ("__RTTI__4Second", ".sdata", 8, 4, WEAK, OBJECT)],
                    sdata_relocs=[(4, "@1"), (8, "@2")])
        retail = build(self.d / "r.o", text=b"\x60\0\0\0" * 4,
                       rodata=b"First\0\0\0POOL", sdata=b"\0" * 4,
                       symbols=[("f", ".text", 0, 0x10, GLOBAL, FUNC),
                                ("keep", ".sdata", 0, 4, GLOBAL, OBJECT),
                                ("@1", ".rodata", 0, 8, LOCAL, OBJECT),
                                ("@pool", ".rodata", 8, 4, LOCAL, OBJECT)],
                       sdata_relocs=[])
        v = linked_view_for(obj, RANGES,
                            {**self.OWN,
                             "__RTTI__4First": 0x80600000,
                             "__RTTI__4Second": 0x80600004},
                            "this/Unit.cpp", retail_sections=W._retail_sections(retail))
        self.assertEqual(v.removed.get(".sdata"), 8)
        self.assertEqual(v.removed.get(".rodata"), 8)
        self.assertFalse(any("@1" in n for n in v.excluded))
        self.assertTrue(any("@2" in n and "retail lacks those bytes at +0x8" in n
                            for n in v.excluded))
        self.assertTrue(check_data_sections(retail, v.path).ok)

    def test_string_only_an_excluded_symbol_references_is_stripped(self):
        obj = unit_object(self.d / "u.o")
        # This unit's own string (referenced by a kept global) stays.
        retail = build(self.d / "r.o", text=b"\x60\0\0\0" * 4 + b"\x4e\x80\x00\x20" * 4,
                       rodata=b"Unit\0\0\0\0", sdata=b"\0" * 4,
                       symbols=[("f", ".text", 0, 0x10, GLOBAL, FUNC),
                                ("__dt__4BaseFv", ".text", 0x10, 0x10, WEAK, FUNC),
                                ("lbl_1", ".rodata", 0, 5, GLOBAL, OBJECT),
                                ("__RTTI__4Unit", ".sdata", 0, 4, GLOBAL, OBJECT)],
                       sdata_relocs=[(0, "lbl_1")])
        v = linked_view_for(obj, RANGES, {**self.OWN, "__RTTI__4Base": 0x80600004},
                            "this/Unit.cpp", retail_sections=W._retail_sections(retail))
        self.assertEqual(v.removed.get(".sdata"), 4)
        self.assertEqual(v.removed.get(".rodata"), 8)  # "Base" + its padding
        self.assertFalse(check_data_sections(retail, obj).ok)
        self.assertTrue(check_data_sections(retail, v.path).ok)

    def test_string_still_used_by_kept_data_is_not_stripped(self):
        obj = build(self.d / "u.o", text=b"\x60\0\0\0" * 4, rodata=b"Base\0\0\0\0",
                    sdata=b"\0" * 8,
                    symbols=[("f", ".text", 0, 0x10, GLOBAL, FUNC),
                             ("@2", ".rodata", 0, 5, LOCAL, OBJECT),
                             ("keep", ".sdata", 0, 4, GLOBAL, OBJECT),
                             ("__RTTI__4Base", ".sdata", 4, 4, WEAK, OBJECT)],
                    sdata_relocs=[(0, "@2"), (4, "@2")])
        v = self.view(obj, {"__RTTI__4Base": 0x80600004})
        self.assertEqual(v.removed.get(".sdata"), 4)
        self.assertNotIn(".rodata", v.removed)


    def test_retail_link_tail_padding(self):
        def obj(name, rodata):
            return build(self.d / name, text=b"\x60\0\0\0", rodata=rodata, sdata=b"",
                         symbols=[("f", ".text", 0, 4, GLOBAL, FUNC)], sdata_relocs=[])
        retail = obj("r.o", b"Unit\0\0\0\0")
        self.assertTrue(check_data_sections(retail, obj("d.o", b"Unit\0")).ok)
        # Non-zero tail bytes are data, not padding.
        self.assertFalse(check_data_sections(obj("r2.o", b"Unit\0\0\0\1"), obj("d2.o", b"Unit\0")).ok)


if __name__ == "__main__":
    unittest.main()


class KeepAlignmentTest(unittest.TestCase):
    """Dropping an object must leave the next one at its own alignment."""

    def fake(self, aligns):
        import struct
        import types
        # symbol tuples: (name, value, size, info, other, shndx); section 1 = .rodata
        syms = [(0, 0, 0, 0, 0, 0),
                (0, 0x0, 0xB, 1, 0, 1),    # kept name string
                (0, 0xC, 0xB, 1, 0, 1),    # dropped
                (0, 0x18, 0x10, 1, 0, 1)]  # string table
        comment = bytearray(b"CodeWarrior".ljust(0x2C, b"\0"))
        comment += bytes(8)
        for a in aligns:
            comment += struct.pack(">I", a) + bytes(4)
        return types.SimpleNamespace(names=["", ".rodata", ".comment"], syms=syms, shnum=3,
                                     contents=[b"", b"", bytes(comment)])

    def test_tail_of_dropped_range_becomes_padding(self):
        ranges, zero = W._keep_alignment(self.fake([4, 4, 8]), {1: [(0xC, 0x18)]})
        # 0x18 would land on 0xC; an 8-aligned object needs 0x10, so 4 bytes stay.
        self.assertEqual(ranges[1], [(0xC, 0x14)])
        self.assertEqual(zero[1], [(0x14, 0x18)])

    def test_already_aligned_object_is_untouched(self):
        ranges, zero = W._keep_alignment(self.fake([4, 4, 4]), {1: [(0xC, 0x18)]})
        self.assertEqual(ranges[1], [(0xC, 0x18)])
        self.assertEqual(zero, {})
