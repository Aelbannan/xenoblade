"""Regression tests for the duplicate-definition invariant.

One object defining a name twice makes `mwldeppc` abort the link with
"multiply-defined: <name> in <obj>". A UNIT_RULES entry must therefore never
mint a second definition of a name that is already defined by an earlier entry
(the 2026-10 `g3d_resmat` `lbl_eu_80669A78` bug: a `pool_patterns` rename
created a second definition and `add_local_symbol` then globalized it).
"""

from __future__ import annotations

import struct
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2]))

from tools.coop.tests.test_postprocess_pad import (  # noqa: E402
    SHT_PROGBITS,
    SHT_STRTAB,
    SHT_SYMTAB,
    _build_elf,
    _strtab,
)
from tools.postprocess_reloc_names import duplicate_definitions  # noqa: E402

STB_GLOBAL = 1
STB_WEAK = 2
STB_LOCAL = 0
STT_OBJECT = 1
SHN_UNDEF = 0


def _symtab(entries: list[tuple[str, int, int, int, int]]) -> bytes:
    """entries: (name, value, size, st_info, st_shndx) - 16-byte big-endian each."""
    names = [e[0] for e in entries]
    _, offs = _strtab(*names)
    out = bytearray()
    for name, value, size, info, shndx in entries:
        out += struct.pack(">IIIBBH", offs[name], value, size, info, 0, shndx)
    return bytes(out)


def _object(tmp: Path, label: str, entries: list[tuple[str, int, int, int]]) -> Path:
    """Build a minimal object whose .symtab holds *entries* (name, value, size, st_info), all in .data."""
    spec = [(name, value, size, info, 1) for name, value, size, info in entries]
    names = [e[0] for e in spec]
    strtab, _offs = _strtab(*names)
    symtab = _symtab(spec)
    data = _build_elf(
        [
            {"name": ".data", "type": SHT_PROGBITS, "align": 4, "data": b"\x00" * 16},
            {"name": ".strtab", "type": SHT_STRTAB, "align": 1, "data": strtab},
            {"name": ".symtab", "type": SHT_SYMTAB, "align": 4, "data": symtab},
        ]
    )
    path = tmp / label
    path.write_bytes(data)
    return path


class DuplicateDefinitions(unittest.TestCase):
    def test_two_strong_definitions_of_one_name_are_flagged(self) -> None:
        with tempfile.TemporaryDirectory() as td:
            obj = _object(
                Path(td),
                "dup.o",
                [
                    ("lbl_eu_80669A78", 0x10, 8, (STB_GLOBAL << 4) | STT_OBJECT),
                    ("lbl_eu_80669A78", 0x10, 8, (STB_GLOBAL << 4) | STT_OBJECT),
                ],
            )
            dups = duplicate_definitions(obj)
        self.assertEqual(list(dups), ["lbl_eu_80669A78"])
        self.assertEqual(len(dups["lbl_eu_80669A78"]), 2)

    def test_weak_twin_beside_strong_is_legal(self) -> None:
        """ELF permits one strong definition plus weak/local copies (linkers
        pick the strong): e.g. CDeviceGX.reloc.o keeps the weak
        __RTTI__9CDeviceGX copy beside the strong lbl_eu_80663758."""
        with tempfile.TemporaryDirectory() as td:
            path = Path(td) / "weak.o"
            strtab, _ = _strtab("lbl_eu_80663758")
            symtab = _symtab(
                [
                    ("lbl_eu_80663758", 0x8, 8, (STB_GLOBAL << 4) | STT_OBJECT, 1),
                    ("lbl_eu_80663758", 0x10, 8, (STB_WEAK << 4) | STT_OBJECT, 1),
                ]
            )
            data = _build_elf(
                [
                    {"name": ".data", "type": SHT_PROGBITS, "align": 4, "data": b"\x00" * 16},
                    {"name": ".strtab", "type": SHT_STRTAB, "align": 1, "data": strtab},
                    {"name": ".symtab", "type": SHT_SYMTAB, "align": 4, "data": symtab},
                ]
            )
            path.write_bytes(data)
            self.assertEqual(duplicate_definitions(path), {})

    def test_undefined_twin_is_legal(self) -> None:
        """Defined + UNDEF with the same name resolves by name; not a duplicate."""
        with tempfile.TemporaryDirectory() as td:
            path = Path(td) / "ok.o"
            strtab, offs = _strtab("lbl_eu_80669A78")
            symtab = _symtab(
                [
                    ("lbl_eu_80669A78", 0x10, 8, (STB_LOCAL << 4) | STT_OBJECT, 1),
                    ("lbl_eu_80669A78", 0, 0, (STB_GLOBAL << 4) | STT_OBJECT, SHN_UNDEF),
                ]
            )
            data = _build_elf(
                [
                    {"name": ".data", "type": SHT_PROGBITS, "align": 4, "data": b"\x00" * 16},
                    {"name": ".strtab", "type": SHT_STRTAB, "align": 1, "data": strtab},
                    {"name": ".symtab", "type": SHT_SYMTAB, "align": 4, "data": symtab},
                ]
            )
            path.write_bytes(data)
            self.assertEqual(duplicate_definitions(path), {})

    def test_distinct_names_are_clean(self) -> None:
        with tempfile.TemporaryDirectory() as td:
            obj = _object(
                Path(td),
                "clean.o",
                [
                    ("lbl_a", 0x0, 4, (STB_LOCAL << 4) | STT_OBJECT),
                    ("lbl_b", 0x4, 4, (STB_LOCAL << 4) | STT_OBJECT),
                ],
            )
            self.assertEqual(duplicate_definitions(obj), {})

    def test_real_build_object_is_clean(self) -> None:
        """The postprocessed g3d_resmat object was the bug's home: assert clean."""
        obj = (
            Path(__file__).resolve().parents[2]
            / "build"
            / "us"
            / "src"
            / "nw4r"
            / "src"
            / "g3d"
            / "res"
            / "g3d_resmat.reloc.o"
        )
        if not obj.is_file():
            self.skipTest("g3d_resmat.reloc.o not built")
        self.assertEqual(duplicate_definitions(obj), {})


if __name__ == "__main__":
    unittest.main()
