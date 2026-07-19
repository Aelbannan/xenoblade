"""Tests for tools/ppc_equivalence/dol_symbols.py — DOL text-section extraction."""

from __future__ import annotations

import struct
import tempfile
import unittest
from pathlib import Path

from tools.ppc_equivalence.dol_symbols import (
    DolSlice,
    DolSymbolError,
    DolTextSection,
    data_sections,
    extract_by_address,
    extract_data_by_address,
    text_sections,
)


def _build_dol(
    text_sections: list[tuple[int, int, bytes]],
    *,
    data_sections: list[tuple[int, int, bytes]] | None = None,
) -> bytes:
    """Build a minimal broadway DOL with the given text section tuples.

    Each tuple is ``(virtual_address, file_offset, code_bytes)``. ``data_sections``
    has the same shape for the 11 data slots (unused by these tests but kept for
    parity with the real header layout).
    """
    text_off = [0] * 7
    text_addr = [0] * 7
    text_size = [0] * 7
    for index, (addr, off, code) in enumerate(text_sections):
        text_off[index] = off
        text_addr[index] = addr
        text_size[index] = len(code)

    data_off = [0] * 11
    data_addr = [0] * 11
    data_size = [0] * 11
    for index, (addr, off, code) in enumerate(data_sections or ()):
        data_off[index] = off
        data_addr[index] = addr
        data_size[index] = len(code)

    header = bytearray(0x100)
    struct.pack_into(">7I", header, 0x00, *text_off)
    struct.pack_into(">11I", header, 0x1C, *data_off)
    struct.pack_into(">7I", header, 0x48, *text_addr)
    struct.pack_into(">11I", header, 0x64, *data_addr)
    struct.pack_into(">7I", header, 0x90, *text_size)
    struct.pack_into(">11I", header, 0xAC, *data_size)
    struct.pack_into(">I", header, 0xE0, 0x80004000)  # entry point

    payload = bytearray()
    for _, _, code in text_sections:
        payload.extend(code)
    for _, _, code in data_sections or ():
        payload.extend(code)

    return bytes(header) + bytes(payload)


# Two text sections, matching retail main.dol layout (.init at 0x80004000, .text at 0x800395a0)
_INIT = bytes.fromhex("38600001 4e800020")  # addi r3,r0,1 ; blr (8 bytes)
_TEXT = bytes([
    0x38, 0x63, 0x00, 0x04,  # addi r3,r3,4
    0x4e, 0x80, 0x00, 0x20,  # blr
] * 4)  # 16 bytes


class DolTextSectionsTests(unittest.TestCase):
    def test_text_sections_skips_empty_slots(self) -> None:
        # Two populated text slots (0 and 1), rest zero — matches retail main.dol
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "main.dol"
            blob = _build_dol(
                [
                    (0x80004000, 0x100, _INIT),  # slot 0
                    (0x800395A0, 0x100 + len(_INIT), _TEXT),  # slot 1
                ]
            )
            path.write_bytes(blob)
            sections = text_sections(path)
        self.assertEqual(len(sections), 2)
        self.assertIsInstance(sections[0], DolTextSection)
        self.assertEqual(sections[0].index, 0)
        self.assertEqual(sections[0].address, 0x80004000)
        self.assertEqual(sections[0].size, len(_INIT))
        self.assertEqual(sections[1].index, 1)
        self.assertEqual(sections[1].address, 0x800395A0)
        self.assertEqual(sections[1].size, len(_TEXT))
        self.assertEqual(sections[1].end, 0x800395A0 + len(_TEXT))

    def test_text_sections_rejects_truncated_file(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "main.dol"
            path.write_bytes(b"\x00" * 16)  # < 0x100 header
            with self.assertRaises(DolSymbolError):
                text_sections(path)

    def test_text_sections_rejects_section_past_eof(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "main.dol"
            blob = _build_dol([(0x80004000, 0x100, _INIT)])[:-8]  # trim end
            path.write_bytes(blob)
            with self.assertRaises(DolSymbolError):
                text_sections(path)


class ExtractByAddressTests(unittest.TestCase):
    def test_extract_within_init_section(self) -> None:
        # _INIT is 8 bytes: 2 instructions at 0x80004000 and 0x80004004
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "main.dol"
            blob = _build_dol([(0x80004000, 0x100, _INIT)])
            path.write_bytes(blob)
            slice_ = extract_by_address(path, 0x80004000, 4)
        self.assertIsInstance(slice_, DolSlice)
        self.assertEqual(slice_.base, 0x80004000)
        self.assertEqual(slice_.code, _INIT[:4])

    def test_extract_within_second_text_section(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "main.dol"
            blob = _build_dol(
                [
                    (0x80004000, 0x100, _INIT),
                    (0x800395A0, 0x100 + len(_INIT), _TEXT),
                ]
            )
            path.write_bytes(blob)
            # Slice 4 instructions (16 bytes) starting at the .text base.
            slice_ = extract_by_address(path, 0x800395A0, 16)
        self.assertEqual(slice_.base, 0x800395A0)
        self.assertEqual(slice_.code, _TEXT[:16])

    def test_extract_offset_within_section(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "main.dol"
            blob = _build_dol([(0x800395A0, 0x100, _TEXT)])
            path.write_bytes(blob)
            # Start 8 bytes into _TEXT, pick the next 4 bytes.
            slice_ = extract_by_address(path, 0x800395A0 + 8, 4)
        self.assertEqual(slice_.base, 0x800395A0 + 8)
        self.assertEqual(slice_.code, _TEXT[8:12])

    def test_extract_spanning_end_of_section_errors(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "main.dol"
            blob = _build_dol([(0x80004000, 0x100, _INIT)])
            path.write_bytes(blob)
            with self.assertRaises(DolSymbolError) as ctx:
                extract_by_address(path, 0x80004000, 16)
        self.assertIn("overruns text section", str(ctx.exception))

    def test_extract_unknown_address_errors(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "main.dol"
            blob = _build_dol([(0x80004000, 0x100, _INIT)])
            path.write_bytes(blob)
            with self.assertRaises(DolSymbolError) as ctx:
                extract_by_address(path, 0x80100000, 4)
        self.assertIn("not in any text section", str(ctx.exception))

    def test_extract_rejects_non_word_size(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "main.dol"
            blob = _build_dol([(0x80004000, 0x100, _INIT)])
            path.write_bytes(blob)
            with self.assertRaises(DolSymbolError):
                extract_by_address(path, 0x80004000, 6)
            with self.assertRaises(DolSymbolError):
                extract_by_address(path, 0x80004000, 0)

    def test_extract_rejects_missing_file(self) -> None:
        with self.assertRaises((FileNotFoundError, DolSymbolError)):
            extract_by_address(Path("/nonexistent/main.dol"), 0x80004000, 4)

    def test_data_sections_skips_zero_file_offset_slots(self) -> None:
        # Retail DOL headers may leave garbage size/addr with file offset zero.
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "main.dol"
            data_off = 0x100 + len(_INIT)
            blob = _build_dol(
                [(0x80004000, 0x100, _INIT)],
                data_sections=[(0x804FA1E0, data_off, b"\x00" * 16)],
            )
            # Poison slot 8: off=0 but nonzero size (must not be listed).
            header = bytearray(blob[:0x100])
            struct.pack_into(">I", header, 0x1C + 8 * 4, 0)
            struct.pack_into(">I", header, 0x6C + 8 * 4, 0x80500000)
            struct.pack_into(">I", header, 0xB8 + 8 * 4, 0x1000)
            path.write_bytes(bytes(header) + blob[0x100:])
            sections = data_sections(path)
            self.assertEqual(len(sections), 1)
            self.assertEqual(sections[0].address, 0x804FA1E0)

    def test_extract_data_by_address_reads_data_section(self) -> None:
        payload = b"\x80\x0d\x30\x84\x80\x0d\x30\xa4"
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "main.dol"
            data_off = 0x100 + len(_INIT)
            blob = _build_dol(
                [(0x80004000, 0x100, _INIT)],
                data_sections=[(0x8052B1BC, data_off, payload)],
            )
            path.write_bytes(blob)
            slice_ = extract_data_by_address(path, 0x8052B1BC, 8)
        self.assertEqual(slice_.base, 0x8052B1BC)
        self.assertEqual(slice_.code, payload)


if __name__ == "__main__":
    unittest.main()
