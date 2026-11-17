"""Linker weak-dedup view (tools/coop/lib/weak_dedup.py) on hand-built ELFs."""

from __future__ import annotations

import struct
import tempfile
import unittest
from pathlib import Path

from tools.coop.lib.data_match import check_data_sections
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
          symbols: list[tuple], sdata_relocs: list[tuple[int, str]]) -> Path:
    """symbols: (name, section, value, size, bind, type)."""
    names = [".text", ".rodata", ".sdata", ".rela.sdata", ".symtab", ".strtab", ".shstrtab"]
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
    blobs = {".text": text, ".rodata": rodata, ".sdata": sdata, ".rela.sdata": rela,
             ".symtab": b"".join(syms), ".strtab": strtab, ".shstrtab": shstr}
    body, offs = b"", {}
    for n in names:
        body += b"\0" * ((-(0x34 + len(body))) % 4)
        offs[n] = 0x34 + len(body)
        body += blobs[n]
    body += b"\0" * ((-(0x34 + len(body))) % 4)
    shdrs = [b"\0" * 40]
    for n in names:
        typ = {".symtab": 2, ".strtab": 3, ".shstrtab": 3, ".rela.sdata": 4}.get(n, 1)
        link = idx[".symtab"] if n == ".rela.sdata" else (idx[".strtab"] if n == ".symtab" else 0)
        info = idx[".sdata"] if n == ".rela.sdata" else 0
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

    def view(self, obj: Path, addrs: dict[str, int]):
        return linked_view_for(obj, RANGES, addrs, "this/Unit.cpp")

    def test_weak_copy_owned_by_other_unit_is_excluded(self):
        obj = unit_object(self.d / "u.o")
        v = self.view(obj, {"__dt__4BaseFv": 0x80000040})
        self.assertEqual(v.removed.get(".text"), 0x10)
        self.assertTrue(any("__dt__4BaseFv" in n for n in v.excluded))

    def test_weak_symbol_retail_places_in_this_unit_is_kept(self):
        obj = unit_object(self.d / "u.o")
        v = self.view(obj, {"__dt__4BaseFv": 0x80001010})
        self.assertEqual(v.excluded, [])
        self.assertEqual(v.path, obj)

    def test_global_symbol_with_same_name_is_kept(self):
        obj = unit_object(self.d / "u.o", weak_dtor_bind=GLOBAL)
        v = self.view(obj, {"__dt__4BaseFv": 0x80000040})
        self.assertEqual(v.excluded, [])

    def test_weak_symbol_unknown_to_symbols_txt_is_kept(self):
        obj = unit_object(self.d / "u.o")
        self.assertEqual(self.view(obj, {}).excluded, [])

    def test_string_only_an_excluded_symbol_references_is_stripped(self):
        obj = unit_object(self.d / "u.o")
        v = self.view(obj, {"__RTTI__4Base": 0x80600004})
        self.assertEqual(v.removed.get(".sdata"), 4)
        self.assertEqual(v.removed.get(".rodata"), 8)  # "Base" + its padding
        # This unit's own string (referenced by a kept global) stays.
        retail = build(self.d / "r.o", text=b"\x60\0\0\0" * 4 + b"\x4e\x80\x00\x20" * 4,
                       rodata=b"Unit\0\0\0\0", sdata=b"\0" * 4,
                       symbols=[("f", ".text", 0, 0x10, GLOBAL, FUNC),
                                ("__dt__4BaseFv", ".text", 0x10, 0x10, WEAK, FUNC),
                                ("lbl_1", ".rodata", 0, 5, GLOBAL, OBJECT),
                                ("__RTTI__4Unit", ".sdata", 0, 4, GLOBAL, OBJECT)],
                       sdata_relocs=[(0, "lbl_1")])
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
