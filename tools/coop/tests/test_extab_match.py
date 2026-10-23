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


if __name__ == "__main__":
    unittest.main()
