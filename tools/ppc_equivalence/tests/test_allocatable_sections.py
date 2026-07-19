"""Tests for allocatable data-section extraction and ROM image binding."""

from __future__ import annotations

import struct
import tempfile
import unittest
from pathlib import Path

from tools.ppc_equivalence.address_space import RegionKind
from tools.ppc_equivalence.elf_symbols import (
    ElfSymbolError,
    SHF_ALLOC,
    extract_allocatable_section,
    list_allocatable_sections,
    rom_image_from_allocatable_section,
)
from tools.ppc_equivalence.ir import R_PPC_ADDR32
from tools.ppc_equivalence.tests.test_elf_symbols import build_reloc_elf, _EQ_LEFT


def build_elf_with_rodata(
    *,
    text: bytes = _EQ_LEFT,
    rodata: bytes,
    text_symbol: str = "f",
    rodata_relocations: tuple[tuple[int, str, int, int], ...] = (),
    text_addr: int = 0,
    rodata_addr: int = 0,
    e_type: int = 1,
) -> bytes:
    """Minimal ET_REL with .text + .rodata and optional .rela.rodata (ADDR32)."""
    shstr = (
        b"\x00.text\x00.rodata\x00.strtab\x00.shstrtab\x00.symtab\x00"
        b".rela.rodata\x00"
    )
    strtab = bytearray(b"\x00")
    text_name_off = len(strtab)
    strtab.extend(text_symbol.encode("ascii") + b"\x00")
    case_offsets: dict[str, int] = {}
    for _offset, symbol, _rtype, _addend in rodata_relocations:
        if symbol in case_offsets or symbol == text_symbol:
            continue
        case_offsets[symbol] = len(strtab)
        strtab.extend(symbol.encode("ascii") + b"\x00")

    # Section indexes: 0 null, 1 .text, 2 .rodata, 3 .strtab, 4 .shstrtab, 5 .symtab,
    # 6 .rela.rodata (optional)
    symtab = bytearray(b"\x00" * 16)
    # text function
    symtab.extend(
        struct.pack(
            ">IIIBBH",
            text_name_off,
            0,
            len(text),
            (1 << 4) | 2,  # GLOBAL FUNC
            0,
            1,
        )
    )
    symbol_indexes: dict[str, int] = {text_symbol: 1}
    next_index = 2
    for symbol, name_off in case_offsets.items():
        # Absolute/section symbols for jump targets — NOTYPE in .text
        symbol_indexes[symbol] = next_index
        next_index += 1
        symtab.extend(
            struct.pack(
                ">IIIBBH",
                name_off,
                0,
                4,
                (1 << 4) | 0,
                0,
                1,
            )
        )

    rela = bytearray()
    for offset, symbol, relocation_type, addend in rodata_relocations:
        rela.extend(
            struct.pack(
                ">IIi",
                offset,
                (symbol_indexes[symbol] << 8) | relocation_type,
                addend,
            )
        )

    has_rela = bool(rodata_relocations)
    e_shnum = 7 if has_rela else 6
    e_shentsize = 40
    e_shoff = 52
    payload_off = e_shoff + e_shnum * e_shentsize

    text_off = payload_off
    rodata_off = text_off + len(text)
    strtab_off = rodata_off + len(rodata)
    shstr_off = strtab_off + len(strtab)
    symtab_off = shstr_off + len(shstr)
    rela_off = symtab_off + len(symtab)

    def shdr(
        name_off: int,
        sh_type: int,
        flags: int,
        addr: int,
        offset: int,
        size: int,
        link: int = 0,
        info: int = 0,
        align: int = 4,
        entsize: int = 0,
    ) -> bytes:
        return struct.pack(
            ">IIIIIIIIII",
            name_off,
            sh_type,
            flags,
            addr,
            offset,
            size,
            link,
            info,
            align,
            entsize,
        )

    text_name = shstr.index(b".text\x00")
    rodata_name = shstr.index(b".rodata\x00")
    strtab_name = shstr.index(b".strtab\x00")
    shstr_name = shstr.index(b".shstrtab\x00")
    symtab_name = shstr.index(b".symtab\x00")
    rela_name = shstr.index(b".rela.rodata\x00")

    headers = (
        shdr(0, 0, 0, 0, 0, 0)
        + shdr(text_name, 1, 6, text_addr, text_off, len(text), align=4)
        + shdr(rodata_name, 1, SHF_ALLOC, rodata_addr, rodata_off, len(rodata), align=4)
        + shdr(strtab_name, 3, 0, 0, strtab_off, len(strtab), align=1)
        + shdr(shstr_name, 3, 0, 0, shstr_off, len(shstr), align=1)
        + shdr(symtab_name, 2, 0, 0, symtab_off, len(symtab), link=3, info=1, align=4, entsize=16)
    )
    if has_rela:
        # info = .rodata section index = 2; link = .symtab = 5
        headers += shdr(rela_name, 4, 0, 0, rela_off, len(rela), link=5, info=2, align=4, entsize=12)

    ident = bytearray(16)
    ident[0:4] = b"\x7fELF"
    ident[4] = 1
    ident[5] = 2
    ident[6] = 1
    header = bytes(ident) + struct.pack(
        ">HHIIIIIHHHHHH",
        e_type,
        20,
        1,
        0,
        0,
        e_shoff,
        0,
        52,
        0,
        0,
        e_shentsize,
        e_shnum,
        4,
    )
    assert len(header) == 52
    return (
        header
        + headers
        + text
        + rodata
        + bytes(strtab)
        + shstr
        + bytes(symtab)
        + bytes(rela)
    )


class AllocatableSectionTests(unittest.TestCase):
    def test_lists_text_from_minimal_elf(self) -> None:
        elf = build_reloc_elf({"f": _EQ_LEFT})
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "t.o"
            path.write_bytes(elf)
            sections = list_allocatable_sections(path)
        names = [item.name for item in sections]
        self.assertIn(".text", names)
        text = next(item for item in sections if item.name == ".text")
        self.assertTrue(text.executable)
        self.assertEqual(text.data, _EQ_LEFT)

    def test_extracts_rodata_and_addr32_relocs(self) -> None:
        # Four ADDR32 slots (placeholder zeros) targeting cases in .text.
        rodata = bytes(16)
        elf = build_elf_with_rodata(
            rodata=rodata,
            rodata_relocations=(
                (0, "case0", R_PPC_ADDR32, 0),
                (4, "case1", R_PPC_ADDR32, 0),
                (8, "case2", R_PPC_ADDR32, 0),
                (12, "case3", R_PPC_ADDR32, 0),
            ),
        )
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "jt.o"
            path.write_bytes(elf)
            section = extract_allocatable_section(path, ".rodata")
        self.assertEqual(section.data, rodata)
        self.assertFalse(section.writable)
        self.assertFalse(section.executable)
        self.assertEqual(len(section.relocations), 4)
        self.assertEqual(
            [item.relocation_type for item in section.relocations],
            [R_PPC_ADDR32] * 4,
        )
        self.assertEqual(
            [item.symbol for item in section.relocations],
            ["case0", "case1", "case2", "case3"],
        )
        self.assertEqual(section.relocations[0].target_section, ".rodata")

    def test_rom_image_from_rodata(self) -> None:
        rodata = bytes.fromhex("1122334455667788")
        elf = build_elf_with_rodata(rodata=rodata, rodata_addr=0x80010000)
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "ro.o"
            path.write_bytes(elf)
            section = extract_allocatable_section(path, ".rodata")
            region = rom_image_from_allocatable_section(section)
        self.assertEqual(region.kind, RegionKind.ROM_IMAGE)
        self.assertEqual(region.start, 0x80010000)
        self.assertEqual(region.image_bytes, rodata)
        self.assertEqual(region.label, ".rodata")

    def test_rom_image_rejects_text(self) -> None:
        elf = build_reloc_elf({"f": _EQ_LEFT})
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "t.o"
            path.write_bytes(elf)
            text = extract_allocatable_section(path, ".text")
            with self.assertRaises(ElfSymbolError):
                rom_image_from_allocatable_section(text)

    def test_missing_section_errors(self) -> None:
        elf = build_reloc_elf({"f": _EQ_LEFT})
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "t.o"
            path.write_bytes(elf)
            with self.assertRaises(ElfSymbolError):
                extract_allocatable_section(path, ".rodata")


if __name__ == "__main__":
    unittest.main()
