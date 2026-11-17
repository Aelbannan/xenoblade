"""extab/extabindex comparison (data_match) and the shrink-only extab gate.

Fixtures are tiny hand-built ELF32 big-endian relocatable objects; no build.
"""

from __future__ import annotations

import struct
import tempfile
import unittest
from pathlib import Path

from tools.coop import extab_gate as eg
from tools.coop.lib.data_match import (
    check_data_sections,
    check_extab_sections,
    extab_function_diffs,
)

R_PPC_ADDR32 = 1


def build_elf(path: Path, *, text: bytes, extab: bytes, index: bytes,
              symbols: list[tuple[str, str | None, int, bool]],
              extab_relocs: list[tuple[int, str, int]],
              index_relocs: list[tuple[int, str, int]]) -> Path:
    """Write an ET_REL object.

    *symbols*: (name, section or None for UNDEF, value, is_global).
    Relocs: (offset, symbol name, addend), all R_PPC_ADDR32.
    """
    sec_names = [".text", "extab", "extabindex", ".relaextab", ".relaextabindex",
                 ".symtab", ".strtab", ".shstrtab"]
    idx = {n: i + 1 for i, n in enumerate(sec_names)}
    shstr = b"\0" + b"".join(n.encode() + b"\0" for n in sec_names)
    shname = {}
    off = 1
    for n in sec_names:
        shname[n] = off
        off += len(n) + 1
    strtab = b"\0"
    syms = [b"\0" * 16]
    order = sorted(symbols, key=lambda s: s[3])  # locals first
    sym_index = {}
    for name, sec, value, glob in order:
        st_name = len(strtab)
        strtab += name.encode() + b"\0"
        info = (1 if glob else 0) << 4 | (2 if sec == ".text" else 0)
        shndx = idx[sec] if sec else 0
        sym_index[name] = len(syms)
        syms.append(struct.pack(">IIIBBH", st_name, value, 0, info, 0, shndx))
    n_local = 1 + sum(1 for s in order if not s[3])
    symtab = b"".join(syms)

    def rela(rs):
        return b"".join(struct.pack(">IIi", o, sym_index[n] << 8 | R_PPC_ADDR32, a)
                        for o, n, a in rs)

    blobs = {".text": text, "extab": extab, "extabindex": index,
             ".relaextab": rela(extab_relocs), ".relaextabindex": rela(index_relocs),
             ".symtab": symtab, ".strtab": strtab, ".shstrtab": shstr}
    body = b""
    offsets = {}
    pos = 0x34
    for n in sec_names:
        while (pos + len(body)) % 4:
            body += b"\0"
        offsets[n] = pos + len(body)
        body += blobs[n]
    while (pos + len(body)) % 4:
        body += b"\0"
    shoff = pos + len(body)
    shdrs = [b"\0" * 40]
    for n in sec_names:
        typ = {".symtab": 2, ".strtab": 3, ".shstrtab": 3}.get(n, 4 if n.startswith(".rela") else 1)
        link = idx[".symtab"] if n.startswith(".rela") else (idx[".strtab"] if n == ".symtab" else 0)
        info = idx[n[5:]] if n.startswith(".rela") else (n_local if n == ".symtab" else 0)
        entsize = 12 if n.startswith(".rela") else (16 if n == ".symtab" else 0)
        shdrs.append(struct.pack(">IIIIIIIIII", shname[n], typ, 0, 0, offsets[n],
                                 len(blobs[n]), link, info, 4, entsize))
    ehdr = (b"\x7fELF\x01\x02\x01" + b"\0" * 9
            + struct.pack(">HHIIIIIHHHHHH", 1, 20, 1, 0, 0, shoff, 0, 0x34, 0, 0, 40,
                          len(shdrs), idx[".shstrtab"]))
    path.write_bytes(ehdr + body + b"".join(shdrs))
    return path


# Two functions; f1 has a cleanup action that calls an out-of-unit dtor.
REC_F1 = bytes.fromhex("1008000000000020" "00000010" "00000000" "8680001e" "00000000" "00000000")
REC_F0 = bytes.fromhex("0808000000000000")


def make(path: Path, *, rec_f1: bytes = REC_F1, dtor: str = "__dt__4BaseFv",
         f1_size: int = 0x20, local_label: str = "@12", swap: bool = False,
         with_action_reloc: bool = True) -> Path:
    extab = REC_F0 + rec_f1
    index = struct.pack(">III", 0, 0x10, 0) + struct.pack(">III", 0, f1_size, 0)
    syms = [("f0", ".text", 0, True), ("f1", ".text", 0x10, True),
            ("@10", "extab", 0, False), (local_label, "extab", 8, False),
            (dtor, None, 0, True)]
    irel = [(0, "f0", 0), (8, "@10", 0), (12, "f1", 0), (20, local_label, 0)]
    if swap:  # same records, reversed index order
        index = index[12:] + index[:12]
        irel = [(0, "f1", 0), (8, local_label, 0), (12, "f0", 0), (20, "@10", 0)]
    erel = [(8 + 0x18, dtor, 0)] if with_action_reloc else []
    return build_elf(path, text=b"\x60\0\0\0" * 12, extab=extab, index=index,
                     symbols=syms, extab_relocs=erel, index_relocs=irel)


class ExtabCompareTest(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.d = Path(self.tmp.name)
        self.retail = make(self.d / "retail.o")

    def tearDown(self):
        self.tmp.cleanup()

    def test_identical_with_renumbered_local_labels_matches(self):
        decomp = make(self.d / "decomp.o", local_label="@9999")
        res = check_extab_sections(self.retail, decomp)
        self.assertTrue(res.ok, [s.detail for s in res.sections])
        self.assertEqual(extab_function_diffs(self.retail, decomp), [])

    def test_missing_cleanup_action_fails(self):
        decomp = make(self.d / "decomp.o", rec_f1=REC_F0 + bytes(16), with_action_reloc=False)
        self.assertFalse(check_extab_sections(self.retail, decomp).ok)
        diffs = extab_function_diffs(self.retail, decomp)
        self.assertEqual(len(diffs), 1)
        self.assertIn("record retail", diffs[0])

    def test_changed_dtor_target_fails_on_reloc_only(self):
        # Bytes identical; only the action's dtor relocation differs.
        decomp = make(self.d / "decomp.o", dtor="__dt__5OtherFv")
        res = check_extab_sections(self.retail, decomp)
        self.assertFalse(res.ok)
        extab = next(s for s in res.sections if s.name == "extab")
        self.assertIn("reloc drift", extab.detail)

    def test_function_size_change_fails(self):
        decomp = make(self.d / "decomp.o", f1_size=0x24)
        self.assertFalse(check_extab_sections(self.retail, decomp).ok)
        self.assertIn("function size", extab_function_diffs(self.retail, decomp)[0])

    def test_order_only_difference_is_strict_fail_but_no_function_diff(self):
        decomp = make(self.d / "decomp.o", swap=True)
        self.assertFalse(check_extab_sections(self.retail, decomp).ok)
        self.assertEqual(extab_function_diffs(self.retail, decomp), [])

    def test_data_gate_unaffected_by_extab(self):
        decomp = make(self.d / "decomp.o", dtor="__dt__5OtherFv")
        self.assertTrue(check_data_sections(self.retail, decomp).ok)


class ExtabGateTest(unittest.TestCase):
    BASE = {"a": 2, "b": 0}

    def test_identical_passes(self):
        self.assertEqual(eg.compare(self.BASE, dict(self.BASE)), ([], []))

    def test_new_unit_fails(self):
        reg, _ = eg.compare(self.BASE, {**self.BASE, "c": 0})
        self.assertEqual(reg, ["new extab mismatch: c (0 function diff(s))"])

    def test_count_rise_fails(self):
        reg, _ = eg.compare(self.BASE, {"a": 3, "b": 0})
        self.assertEqual(reg, ["extab diffs rose in a: 2 -> 3"])

    def test_improvements_and_shrink_only_ratchet(self):
        cur = {"a": 1}
        reg, imp = eg.compare(self.BASE, cur)
        self.assertEqual(reg, [])
        self.assertEqual(len(imp), 2)
        self.assertEqual(eg.ratchet(self.BASE, cur), {"a": 1})
        # Ratchet never adds units or raises counts.
        self.assertEqual(eg.ratchet(self.BASE, {"a": 5, "z": 1}), {"a": 2})

    def test_baseline_roundtrip(self):
        with tempfile.TemporaryDirectory() as td:
            p = Path(td) / "b.json"
            eg.write_baseline({"u": 1}, p)
            self.assertEqual(eg.load_baseline(p), {"u": 1})
            self.assertTrue(eg.baseline_has("u", p))
            self.assertFalse(eg.baseline_has("v", p))


class _FakeLayout:
    """Minimal LinkerLayout: ``symbols.txt`` names -> (section, address)."""

    def __init__(self, symbols: dict[str, tuple[str, int]]):
        self.symbols = symbols

    def resolve_name(self, name):
        return self.symbols.get(name, (None, None))

    def canon_reloc(self, name, addend):
        sec, addr = self.symbols.get(name, (None, None))
        return type("T", (), {"addr": None if addr is None else addr + addend})()


class ExtabPairingAfterReorderTest(unittest.TestCase):
    """Regression (CfScript, 2026-10-24): MWCC emitted the weak
    ``format__Q22ml11FixStr<128>FPCce`` at the front of the decomp ``.text``
    while retail has it at the end of the unit.  The old single ``.text``
    base came from whichever named symbol was seen first (``format``), so
    every function reached through a name ``symbols.txt`` lacks was keyed at
    a shifted address: records showed up as retail-only / decomp-only pairs
    and the gate went 4 -> 9.  extabindex entries must pair with functions
    through their relocations, placed by the nearest anchor."""

    LAYOUT = _FakeLayout({
        "f0": (".text", 0x80001000),
        "f1": (".text", 0x80001010),
        "weak_tmpl": (".text", 0x80001020),
        # A local name that symbols.txt also places in another unit.
        "end": (".text", 0x80123450),
    })

    def _obj(self, path, *, layout_front: bool, f1_name: str, f1_global: bool,
             f1_size: int = 0x10):
        # (function, section offset); retail order: f0, f1, weak_tmpl.
        order = [("weak_tmpl", 0), ("f0", 0x10), (f1_name, 0x20)] if layout_front \
            else [("f0", 0), (f1_name, 0x10), ("weak_tmpl", 0x20)]
        off = dict(order)
        extab = REC_F0 + REC_F0 + REC_F0
        index = b"".join(struct.pack(">III", 0, sz, 0)
                         for sz in (0x10, f1_size, 0x10))
        syms = [("f0", ".text", off["f0"], True),
                (f1_name, ".text", off[f1_name], f1_global),
                ("weak_tmpl", ".text", off["weak_tmpl"], True),
                ("@1", "extab", 0, False), ("@2", "extab", 8, False),
                ("@3", "extab", 16, False)]
        if layout_front:  # MWCC: weak instance first in the symbol table too
            syms = [syms[2]] + syms[:2] + syms[3:]
        irel = [(0, "f0", 0), (8, "@1", 0), (12, f1_name, 0), (20, "@2", 0),
                (24, "weak_tmpl", 0), (32, "@3", 0)]
        return build_elf(path, text=b"\x60\0\0\0" * 12, extab=extab, index=index,
                         symbols=syms, extab_relocs=[], index_relocs=irel)

    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.d = Path(self.tmp.name)
        self.retail = self._obj(self.d / "retail.o", layout_front=False,
                                f1_name="f1", f1_global=True)

    def tearDown(self):
        self.tmp.cleanup()

    def test_weak_instance_moved_to_front_still_pairs(self):
        decomp = self._obj(self.d / "decomp.o", layout_front=True,
                           f1_name="f1__Q22cf3FooFv", f1_global=True)
        self.assertEqual(extab_function_diffs(self.retail, decomp, layout=self.LAYOUT), [])

    def test_real_size_diff_is_reported_against_the_right_function(self):
        decomp = self._obj(self.d / "decomp.o", layout_front=True,
                           f1_name="f1__Q22cf3FooFv", f1_global=True, f1_size=0x14)
        diffs = extab_function_diffs(self.retail, decomp, layout=self.LAYOUT)
        self.assertEqual(diffs, ["f1: function size 0x10 != 0x14"])

    def test_colliding_local_name_is_not_an_anchor(self):
        # Retail splitter label ``end`` is local; symbols.txt's ``end`` is elsewhere.
        retail = self._obj(self.d / "r2.o", layout_front=False, f1_name="end",
                           f1_global=False)
        decomp = self._obj(self.d / "d2.o", layout_front=False,
                           f1_name="end_80001010", f1_global=True)
        self.assertTrue(check_extab_sections(retail, decomp, layout=self.LAYOUT).ok)
        self.assertEqual(extab_function_diffs(retail, decomp, layout=self.LAYOUT), [])


if __name__ == "__main__":
    unittest.main()
