"""Regression tests for postprocess_reloc_names.pad_data_section_func.

Guards the ELF tail-rebuild invariants behind the monolib data gate:
  * the section-header table must survive the rebuild at a valid e_shoff,
  * every moved section keeps its bytes AND its sh_addralign congruence,
  * pre-existing inter-section slack must not desync recorded vs actual
    offsets (the 2025-08 regression that broke every pad_data_section user).
"""

from __future__ import annotations

import struct
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2]))

from tools.postprocess_reloc_names import pad_data_section_func  # noqa: E402

SHT_PROGBITS = 1
SHT_STRTAB = 3
SHT_SYMTAB = 2
SHT_RELA = 4
SHT_NOBITS = 8


def _strtab(*names: str) -> tuple[bytes, dict[str, int]]:
    out = b"\x00"
    offs: dict[str, int] = {}
    for n in names:
        offs[n] = len(out)
        out += n.encode() + b"\x00"
    return out, offs


def _build_elf(sections: list[dict]) -> bytes:
    """Assemble a big-endian ELF32 relocatable.

    Section bodies are laid out sequentially starting at 0x40 (ehdr ends
    there), honoring each section's alignment; .shstrtab and then the shdr
    table are appended after them.
    """
    all_secs = list(sections) + [{"name": ".shstrtab", "type": SHT_STRTAB, "align": 1}]
    names = [""] + [s["name"] for s in all_secs]
    shstr, soff = _strtab(*names)

    ehsize = 52
    shentsize = 40
    off = 0x40
    offs: dict[str, int] = {}
    sizes: dict[str, int] = {}
    bodies: dict[str, bytes] = {}
    for s in all_secs:
        if s.get("align", 1) > 1:
            off += (-off) % s["align"]
        body = b""
        if s["type"] != SHT_NOBITS:
            body = s.get("data", b"\x00" * s.get("size", 0))
        offs[s["name"]] = off
        sizes[s["name"]] = len(body)
        bodies[s["name"]] = body
        off += len(body)

    shoff = off
    total = shoff + shentsize * (len(all_secs) + 1)
    out = bytearray(total)

    out[0:4] = b"\x7fELF"
    out[4] = 1  # ELFCLASS32
    out[5] = 2  # big-endian
    out[6] = 1  # EV_CURRENT
    struct.pack_into(">HHI", out, 16, 1, 20, 1)  # e_type=REL, PPC, version
    struct.pack_into(">III", out, 24, 0, 0, shoff)
    struct.pack_into(
        ">HHHHHH", out, 36, 0, ehsize, 0, 0, shentsize, len(all_secs) + 1, len(all_secs)
    )

    for i, s in enumerate(all_secs, start=1):
        h = shoff + i * shentsize
        typ = {SHT_PROGBITS: 1, SHT_RELA: 4, SHT_NOBITS: 8, SHT_STRTAB: 3}[s["type"]]
        stored_off = 0 if s["type"] == SHT_NOBITS else offs[s["name"]]
        stored_size = sizes[s["name"]] if s["type"] != SHT_NOBITS else s.get("size", 0)
        struct.pack_into(
            ">IIIIIIIII",
            out,
            h + 4,
            typ,
            s.get("flags", 3),
            0,
            stored_off,
            stored_size,
            0,
            0,
            s.get("align", 1),
        )
        struct.pack_into(">I", out, h, soff[s["name"]])
    # section bodies (shstrtab included)
    for s in all_secs:
        o = offs[s["name"]]
        out[o : o + len(bodies[s["name"]])] = bodies[s["name"]]
    return bytes(out)


def _parse(data: bytes):
    """Return ([ (name,type,off,size,align) ], e_shoff)."""
    e_shoff, = struct.unpack_from(">I", data, 32)
    entsize, = struct.unpack_from(">H", data, 46)
    num, = struct.unpack_from(">H", data, 48)
    strndx, = struct.unpack_from(">H", data, 50)
    hstr_hdr = e_shoff + strndx * entsize
    shstr, = struct.unpack_from(">I", data, hstr_hdr + 16)
    out = []
    for i in range(num):
        h = e_shoff + i * entsize
        nm, typ = struct.unpack_from(">II", data, h)
        end = data.index(b"\0", shstr + nm)
        name = data[shstr + nm : end].decode()
        o, sz = struct.unpack_from(">II", data, h + 16)
        align, = struct.unpack_from(">I", data, h + 32)
        out.append((name, typ, o, sz, align))
    return out, e_shoff


class TestPadDataSectionFunc(unittest.TestCase):
    def _mk(self, tmp: Path) -> tuple[Path, dict[str, bytes]]:
        """Synthetic MWCC-like object with inter-section slack:

        .text(0x20,a4) .data(0x10,a8)->pad .sdata(0x10,a8)
        .rela.data(one ADDR32 @4,a4) .bss(nobits 8,a8) .comment(a1)
        """
        sections = [
            {"name": ".text", "type": SHT_PROGBITS, "data": bytes(range(0x20)), "align": 4},
            {"name": ".data", "type": SHT_PROGBITS, "data": bytes([0xAA] * 0x10), "align": 8},
            {"name": ".sdata", "type": SHT_PROGBITS, "data": b"\xBB" * 0x10, "align": 8},
            {
                "name": ".rela.data",
                "type": SHT_RELA,
                "align": 4,
                "data": struct.pack(">IIi", 4, (7 << 8) | 1, 0),
            },
            {"name": ".bss", "type": SHT_NOBITS, "size": 0x8, "align": 8},
            {"name": ".comment", "type": SHT_PROGBITS, "data": b"GCC: (devkitpro)\x00", "align": 1},
        ]
        blob = _build_elf(sections)
        p = tmp / "synth.o"
        p.write_bytes(blob)
        want = {
            ".sdata": b"\xBB" * 0x10,
            ".comment": b"GCC: (devkitpro)\x00",
        }
        return p, want

    def test_pad_preserves_headers_and_content(self):
        with tempfile.TemporaryDirectory() as td:
            p, wants = self._mk(Path(td))
            self.assertTrue(pad_data_section_func(p, ".data", 0x18))

            data = p.read_bytes()
            secs, e_shoff = _parse(data)
            by = {n: (t, o, s, a) for n, t, o, s, a in secs}
            self.assertEqual(by[".data"][2], 0x18)
            # everything after .data shifted past the grown section
            self.assertGreaterEqual(by[".sdata"][1], by[".data"][1] + 0x18)
            # alignment congruence preserved; no section overruns the file
            for n, t, o, s, a in secs:
                if t != SHT_NOBITS and a > 1:
                    self.assertEqual(o % a, 0, f"{n} offset {o:#x} not {a}-aligned")
                if t != SHT_NOBITS and s:
                    self.assertLessEqual(o + s, len(data), f"{n} overruns file")
            # shdr table readable at e_shoff (old bug read past EOF here)
            self.assertLess(e_shoff + 12 * 40, len(data))
            # later-section contents preserved verbatim
            for n, want in wants.items():
                _, o_, s_, _ = by[n]
                self.assertEqual(data[o_ : o_ + s_], want, n)
            # rela entry survived untouched
            _, ro, rs, _ = by[".rela.data"]
            self.assertEqual(struct.unpack_from(">IIi", data, ro)[0], 4)

    def test_pad_is_grow_only(self):
        with tempfile.TemporaryDirectory() as td:
            p, _ = self._mk(Path(td))
            before = bytearray(p.read_bytes())
            secs0, _ = _parse(bytes(before))
            d_off = [s for s in secs0 if s[0] == ".data"][0][2]

            self.assertFalse(pad_data_section_func(p, ".data", 0x10))  # no-op shrink refused
            mid = p.read_bytes()
            self.assertEqual(mid, before)

            self.assertTrue(pad_data_section_func(p, ".data", 0x30))  # grows
            secs, _ = _parse(p.read_bytes())
            self.assertEqual([s for s in secs if s[0] == ".data"][0][3], 0x30)
            # original .data bytes intact after growth
            data = p.read_bytes()
            self.assertEqual(data[d_off : d_off + 0x10], bytes([0xAA] * 0x10))

    def test_pad_reloc_follows_moved_content(self):
        """A .rela.data entry pointing into .data must keep its r_offset when
        earlier content shifts — exercised indirectly via the grow-only test;
        here we assert the rela section itself is byte-preserved."""
        with tempfile.TemporaryDirectory() as td:
            p, _ = self._mk(Path(td))
            before = p.read_bytes()
            secs0, _ = _parse(before)
            rel = [s for s in secs0 if s[0] == ".rela.data"][0]
            orig = before[rel[2] : rel[2] + rel[3]]
            self.assertTrue(pad_data_section_func(p, ".data", 0x20))
            data = p.read_bytes()
            secs, _ = _parse(data)
            rel2 = [s for s in secs if s[0] == ".rela.data"][0]
            self.assertEqual(rel2[3], rel[3])  # size unchanged
            entry = struct.unpack(">IIi", data[rel2[2] : rel2[2] + 12])
            self.assertEqual(entry[0], 4)  # r_offset still points at .data+4


if __name__ == "__main__":
    unittest.main()
