"""SDA base / symbol-address extraction for harness ELF proofs."""

from __future__ import annotations

import struct
import tempfile
import unittest
from pathlib import Path

from tools.ppc_equivalence.sda_layout import (
    extract_sda_bases,
    extract_symbol_addresses,
)


def _elf32_be_exec_with_symbols(symbols: dict[str, int]) -> bytes:
    """Minimal big-endian ELF32 ET_EXEC with an absolute symtab."""
    # Layout: ehdr(52) + shdr*4 + shstrtab + strtab + symtab
    # Sections: [0]=NULL, [1]=.shstrtab, [2]=.strtab, [3]=.symtab
    shstr = b"\x00.shstrtab\x00.strtab\x00.symtab\x00"
    strtab = b"\x00" + b"".join(name.encode() + b"\x00" for name in symbols)
    name_offsets: dict[str, int] = {}
    cursor = 1
    for name in symbols:
        name_offsets[name] = cursor
        cursor += len(name) + 1

    symtab = bytearray(16)  # index 0 null symbol
    for name, value in symbols.items():
        symtab += struct.pack(
            ">IIIBBH",
            name_offsets[name],
            value & 0xFFFFFFFF,
            0,
            (1 << 4) | 1,  # STB_GLOBAL | STT_OBJECT
            0,
            0xFFF1,  # SHN_ABS
        )

    e_shoff = 52
    shentsize = 40
    shnum = 4
    shstr_off = e_shoff + shnum * shentsize
    str_off = shstr_off + len(shstr)
    sym_off = str_off + len(strtab)

    def shdr(
        name_off: int,
        sh_type: int,
        flags: int,
        addr: int,
        offset: int,
        size: int,
        link: int,
        info: int,
        entsize: int,
    ) -> bytes:
        return struct.pack(
            ">IIIIIIIIII",
            name_off, sh_type, flags, addr, offset, size, link, info, 0, entsize,
        )

    shdrs = (
        shdr(0, 0, 0, 0, 0, 0, 0, 0, 0)
        + shdr(1, 3, 0, 0, shstr_off, len(shstr), 0, 0, 0)  # .shstrtab
        + shdr(11, 3, 0, 0, str_off, len(strtab), 0, 0, 0)  # .strtab
        + shdr(19, 2, 0, 0, sym_off, len(symtab), 2, 0, 16)  # .symtab -> .strtab
    )

    ehdr = bytearray(52)
    ehdr[0:4] = b"\x7fELF"
    ehdr[4] = 1
    ehdr[5] = 2
    ehdr[6] = 1
    struct.pack_into(">HHI", ehdr, 0x10, 2, 20, 1)  # ET_EXEC, EM_PPC, EV_CURRENT
    struct.pack_into(">I", ehdr, 0x14, 1)
    struct.pack_into(">I", ehdr, 0x20, e_shoff)
    struct.pack_into(">HHH", ehdr, 0x2E, shentsize, shnum, 1)

    return bytes(ehdr) + shdrs + shstr + strtab + bytes(symtab)


class InitialGprBindingTests(unittest.TestCase):
    def test_initial_gpr_bindings_constrain_r13(self) -> None:
        from tools.ppc_equivalence.contract import make_contract
        from tools.ppc_equivalence.decoder import decode_block, parse_hex
        from tools.ppc_equivalence.engine import check_equivalence
        from tools.ppc_equivalence.result import ProofStatus

        # lwz r3, 0(r13); blr  — with r13 pinned, both sides load the same cell
        code = decode_block(parse_hex("806d0000 4e800020"), validate_with_capstone=False)
        contract = make_contract(preset="ppc-eabi", observe=None, timeout_ms=5_000)
        result = check_equivalence(
            code, code, contract,
            original_hex="806d00004e800020",
            candidate_hex="806d00004e800020",
            initial_gpr_bindings={13: 0x80500000},
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT)


class SdaLayoutTests(unittest.TestCase):
    def test_extract_sda_bases_from_linker_symbols(self) -> None:
        blob = _elf32_be_exec_with_symbols({
            "_SDA_BASE_": 0x80500000,
            "_SDA2_BASE_": 0x80520000,
        })
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "main.elf"
            path.write_bytes(blob)
            bases = extract_sda_bases(path)
            self.assertEqual(bases, {13: 0x80500000, 2: 0x80520000})
            addrs = extract_symbol_addresses(
                path, names={"_SDA_BASE_", "_SDA2_BASE_"},
            )
            self.assertEqual(addrs["_SDA_BASE_"], 0x80500000)

    def test_missing_file_returns_empty(self) -> None:
        self.assertEqual(extract_sda_bases("/no/such/file.elf"), {})
        self.assertEqual(extract_symbol_addresses("/no/such/file.elf"), {})


if __name__ == "__main__":
    unittest.main()
