"""Regression tests for postprocess_reloc_names.define_absent_symbol.

Guards the L5 label-wave mechanism for the ABSENT class (a retail data label
with no symtab entry at all: no definition and no UNDEF slot, so
add_local_symbol has nothing to patch):

  * the new entry appends at the END of .symtab as GLOBAL OBJECT (sh_info
    untouched, no .rela r_info renumbering, locals-first invariant kept,
    which is the shape mwldeppc requires -- appended LOCALs past sh_info
    abort it: ELF_linker.c:11164),
  * the name appends LAST in .strtab (4-byte padded, like
    retarget_reloc_to_symbol), so existing st_name offsets never shift and
    repeated calls compose,
  * existing names (defined or UNDEF) and out-of-section ranges refuse
    without touching the file (the patch path belongs to add_local_symbol;
    a second definition would be a hard multiply-defined link error).
"""

from __future__ import annotations

import struct
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2]))

from tools.postprocess_reloc_names import (  # noqa: E402
    UnitRules,
    define_absent_symbol,
    postprocess_object,
)

SHT_PROGBITS = 1
SHT_SYMTAB = 2
SHT_STRTAB = 3
SHT_RELA = 4


def _build_elf(sections: list[dict]) -> bytes:
    """Assemble a big-endian ELF32 relocatable with link/info/entsize support."""
    shstr = b"\x00"
    soff: dict[str, int] = {}
    for s in sections:
        soff[s["name"]] = len(shstr)
        shstr += s["name"].encode() + b"\x00"
    soff[".shstrtab"] = len(shstr)
    shstr += b".shstrtab\x00"
    all_secs = list(sections) + [
        {"name": ".shstrtab", "type": SHT_STRTAB, "align": 1, "data": shstr}
    ]

    off = 0x40
    offs: dict[str, int] = {}
    bodies: dict[str, bytes] = {}
    for s in all_secs:
        if s.get("align", 1) > 1:
            off += (-off) % s["align"]
        body = s.get("data", b"")
        offs[s["name"]] = off
        bodies[s["name"]] = body
        off += len(body)

    shoff = off
    shentsize = 40
    out = bytearray(shoff + shentsize * (len(all_secs) + 1))
    out[0:4] = b"\x7fELF"
    out[4] = 1
    out[5] = 2
    out[6] = 1
    struct.pack_into(">HHI", out, 16, 1, 20, 1)
    struct.pack_into(">III", out, 24, 0, 0, shoff)
    struct.pack_into(">I", out, 36, 0)
    struct.pack_into(
        ">HHHHHH", out, 40, 52, 0, 0, shentsize, len(all_secs) + 1, len(all_secs)
    )
    for i, s in enumerate(all_secs, start=1):
        h = shoff + i * shentsize
        struct.pack_into(">I", out, h, soff[s["name"]])
        struct.pack_into(
            ">IIIIIIIII",
            out,
            h + 4,
            s["type"],
            s.get("flags", 3),
            0,
            offs[s["name"]],
            len(bodies[s["name"]]),
            s.get("link", 0),
            s.get("info", 0),
            s.get("align", 1),
            s.get("entsize", 0),
        )
    for s in all_secs:
        o = offs[s["name"]]
        out[o : o + len(bodies[s["name"]])] = bodies[s["name"]]
    return bytes(out)


def _sym(name_off: int, value: int, size: int, info: int, shndx: int) -> bytes:
    return struct.pack(">IIIBBH", name_off, value, size, info, 0, shndx)


class DefineAbsentBase(unittest.TestCase):
    def _mk(self, tmp: Path, extra_syms: tuple[bytes, ...] = ()) -> Path:
        """Synthetic object: .sdata(12B) + .rela.sdata + .symtab + .strtab.

        symtab: [null, LOCAL @100 @ .sdata+8/4, GLOBAL sdata_owner @
        .sdata+0/12, ...extra], sh_info=2. The rela entry targets the
        GLOBAL (index 2) so index stability is observable.
        """
        strtab = b"\x00@100\x00sdata_owner\x00"
        n_at = 1
        n_owner = 1 + len(b"@100\x00")
        syms = b"".join(
            [
                _sym(0, 0, 0, 0x00, 0),
                _sym(n_at, 8, 4, 0x01, 1),
                _sym(n_owner, 0, 12, 0x11, 1),
                *extra_syms,
            ]
        )
        sections = [
            {"name": ".sdata", "type": SHT_PROGBITS, "align": 8,
             "data": bytes.fromhex("3f8000008050862800000000")},
            {"name": ".rela.sdata", "type": SHT_RELA, "align": 4,
             "data": struct.pack(">IIi", 4, (2 << 8) | 1, 0),
             "link": 3, "info": 1},
            {"name": ".symtab", "type": SHT_SYMTAB, "align": 4,
             "data": syms, "link": 4, "info": 2, "entsize": 16},
            {"name": ".strtab", "type": SHT_STRTAB, "align": 1, "data": strtab},
        ]
        p = tmp / "synth.o"
        p.write_bytes(_build_elf(sections))
        return p

    def _read_symtab(self, data: bytes):
        e_shoff, = struct.unpack_from(">I", data, 32)
        entsize, = struct.unpack_from(">H", data, 46)
        num, = struct.unpack_from(">H", data, 48)
        strndx, = struct.unpack_from(">H", data, 50)
        idx = {self._sec_name(data, e_shoff, entsize, num, strndx, i): i
               for i in range(num)}
        sym = idx[".symtab"]
        strt = idx[".strtab"]
        sdata = idx[".sdata"]
        sym_off, = struct.unpack_from(">I", data, e_shoff + sym * entsize + 16)
        sym_size, = struct.unpack_from(">I", data, e_shoff + sym * entsize + 20)
        sym_info, = struct.unpack_from(">I", data, e_shoff + sym * entsize + 28)
        str_off, = struct.unpack_from(">I", data, e_shoff + strt * entsize + 16)
        entries = []
        for j in range(0, sym_size, 16):
            st_name, st_value, st_size, st_info, _, st_shndx = struct.unpack_from(
                ">IIIBBH", data, sym_off + j)
            end = data.index(0, str_off + st_name)
            entries.append({
                "name": data[str_off + st_name: end].decode(),
                "value": st_value, "size": st_size,
                "info": st_info, "shndx": st_shndx,
            })
        return entries, sym_info, sdata

    @staticmethod
    def _sec_name(data, e_shoff, entsize, num, strndx, i):
        hstr, = struct.unpack_from(">I", data, e_shoff + strndx * entsize + 16)
        nm, = struct.unpack_from(">I", data, e_shoff + i * entsize)
        end = data.index(0, hstr + nm)
        return data[hstr + nm: end].decode()


class TestDefineAbsentSymbol(DefineAbsentBase):
    def test_append_defines_absent(self):
        with tempfile.TemporaryDirectory() as td:
            p = self._mk(Path(td))
            before = p.read_bytes()
            rela_before = before  # compared after via section parse
            self.assertTrue(
                define_absent_symbol(p, "lbl_eu_80662784", ".sdata", 0, 4))
            data = p.read_bytes()
            entries, sh_info, sdata_idx = self._read_symtab(data)
            # sh_info untouched: still 2 locals-then-globals
            self.assertEqual(sh_info, 2)
            self.assertEqual(len(entries), 4)
            # pre-existing entries byte-identical (names/values/sizes kept)
            old, _, _ = self._read_symtab(before)
            self.assertEqual(entries[:3], old)
            # the newcomer: trailing GLOBAL OBJECT at (.sdata, 0, 4)
            new = entries[3]
            self.assertEqual(new["name"], "lbl_eu_80662784")
            self.assertEqual(
                (new["value"], new["size"], new["info"], new["shndx"]),
                (0, 4, 0x11, sdata_idx))
            # every entry before sh_info is still LOCAL (linker invariant)
            for e in entries[:sh_info]:
                self.assertEqual(e["info"] >> 4, 0)
            self.assertEqual(entries[3]["info"] >> 4, 1)
            # section bytes untouched: .sdata and .rela.sdata identical
            for sec, want in ((b".sdata", bytes.fromhex("3f8000008050862800000000")),
                              (b".rela.sdata", struct.pack(">IIi", 4, (2 << 8) | 1, 0))):
                name = sec.decode()
                e_shoff, = struct.unpack_from(">I", data, 32)
                entsize, = struct.unpack_from(">H", data, 46)
                num, = struct.unpack_from(">H", data, 48)
                strndx, = struct.unpack_from(">H", data, 50)
                i = [k for k in range(num)
                     if self._sec_name(data, e_shoff, entsize, num, strndx, k) == name][0]
                o, s = struct.unpack_from(">II", data, e_shoff + i * entsize + 16)
                self.assertEqual(data[o: o + s], want, name)
            # headers still parse: table inside the file
            e_shoff, = struct.unpack_from(">I", data, 32)
            num, = struct.unpack_from(">H", data, 48)
            self.assertLessEqual(e_shoff + num * 40, len(data))
            _ = rela_before

    def test_refuses_existing_defined(self):
        with tempfile.TemporaryDirectory() as td:
            p = self._mk(Path(td))
            before = p.read_bytes()
            self.assertFalse(
                define_absent_symbol(p, "sdata_owner", ".sdata", 0, 4))
            self.assertEqual(p.read_bytes(), before)

    def test_refuses_existing_undef(self):
        with tempfile.TemporaryDirectory() as td:
            tmp = Path(td)
            strtab = b"\x00@100\x00sdata_owner\x00lbl_x\x00"
            n_x = 1 + len(b"@100\x00sdata_owner\x00")
            syms = b"".join([
                _sym(0, 0, 0, 0x00, 0),
                _sym(1, 8, 4, 0x01, 1),
                _sym(1 + len(b"@100\x00"), 0, 12, 0x11, 1),
                _sym(n_x, 0, 0, 0x10, 0),  # GLOBAL UNDEF lbl_x
            ])
            sections = [
                {"name": ".sdata", "type": SHT_PROGBITS, "align": 8, "data": b"\x00" * 12},
                {"name": ".symtab", "type": SHT_SYMTAB, "align": 4,
                 "data": syms, "link": 3, "info": 3, "entsize": 16},
                {"name": ".strtab", "type": SHT_STRTAB, "align": 1, "data": strtab},
            ]
            p = tmp / "undef.o"
            p.write_bytes(_build_elf(sections))
            before = p.read_bytes()
            # UNDEF twin: add_local_symbol's patch path owns it, not the append.
            self.assertFalse(define_absent_symbol(p, "lbl_x", ".sdata", 0, 4))
            self.assertEqual(p.read_bytes(), before)

    def test_bounds_and_missing_section_refuse(self):
        with tempfile.TemporaryDirectory() as td:
            p = self._mk(Path(td))
            before = p.read_bytes()
            self.assertFalse(define_absent_symbol(p, "lbl_a", ".sdata", 9, 4))
            self.assertFalse(define_absent_symbol(p, "lbl_b", ".nope", 0, 4))
            self.assertFalse(define_absent_symbol(p, "lbl_c", ".sdata", -4, 4))
            self.assertEqual(p.read_bytes(), before)

    def test_double_append_composes(self):
        """The old failure mode: a second .strtab append corrupting the
        first appended st_name. Both names must resolve afterwards."""
        with tempfile.TemporaryDirectory() as td:
            p = self._mk(Path(td))
            self.assertTrue(define_absent_symbol(p, "lbl_one", ".sdata", 0, 4))
            self.assertTrue(define_absent_symbol(p, "lbl_two", ".sdata", 4, 4))
            entries, sh_info, _ = self._read_symtab(p.read_bytes())
            self.assertEqual(sh_info, 2)
            self.assertEqual([e["name"] for e in entries[3:]], ["lbl_one", "lbl_two"])
            self.assertEqual((entries[3]["value"], entries[3]["size"]), (0, 4))
            self.assertEqual((entries[4]["value"], entries[4]["size"]), (4, 4))

    def test_postprocess_wiring(self):
        with tempfile.TemporaryDirectory() as td:
            p = self._mk(Path(td))
            before = p.read_bytes()
            # default: no-op, byte-identical
            self.assertFalse(postprocess_object(p, UnitRules()))
            self.assertEqual(p.read_bytes(), before)
            # opt-in field defines the label
            self.assertTrue(postprocess_object(
                p, UnitRules(define_symbols=(("lbl_eu_80662784", ".sdata", 0, 4),))))
            entries, _, _ = self._read_symtab(p.read_bytes())
            self.assertIn("lbl_eu_80662784", [e["name"] for e in entries])


if __name__ == "__main__":
    unittest.main()
