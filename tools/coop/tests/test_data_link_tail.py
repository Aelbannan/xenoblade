"""Retail split tail padding in ``run.py data diff``.

A retail split's data section ends at the *next* unit's start.  The padding
bytes therefore align the next section (4- or 8-aligned), not necessarily the
current section's ``sh_addralign``.  These fixtures reproduce the
`CWorkThreadSystem` case: an 0x11-byte string in an 0x14-byte, 8-aligned split
(the next TU's string starts at a 4-aligned address).
"""

from __future__ import annotations

import struct
import tempfile
import unittest
from pathlib import Path

from tools.coop.lib.data_match import check_data_sections


def _make_data_object(path: Path, data: bytes, *, align: int,
                      section: str = ".rodata") -> Path:
    """Minimal ET_REL with one data section and no symbols/relocations."""
    names = [section, ".shstrtab"]
    shstr = b"\0"
    shname = {}
    for name in names:
        shname[name] = len(shstr)
        shstr += name.encode() + b"\0"

    body = bytearray()
    while (0x34 + len(body)) % 4:
        body.append(0)
    off_data = 0x34 + len(body)
    body.extend(data)
    while (0x34 + len(body)) % 4:
        body.append(0)
    off_shstr = 0x34 + len(body)
    body.extend(shstr)
    while (0x34 + len(body)) % 4:
        body.append(0)
    shoff = 0x34 + len(body)

    shdrs = [b"\0" * 40]
    shdrs.append(struct.pack(">IIIIIIIIII", shname[section], 1, 0, 0,
                             off_data, len(data), 0, 0, align, 0))
    shdrs.append(struct.pack(">IIIIIIIIII", shname[".shstrtab"], 3, 0, 0,
                             off_shstr, len(shstr), 0, 0, 1, 0))
    ehdr = (b"\x7fELF\x01\x02\x01" + b"\0" * 9
            + struct.pack(">HHIIIIIHHHHHH", 1, 20, 1, 0, 0, shoff, 0,
                          0x34, 0, 0, 40, len(shdrs), 2))
    path.write_bytes(ehdr + bytes(body) + b"".join(shdrs))
    return path


class LinkerTailPaddingTest(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.d = Path(self.tmp.name)

    def tearDown(self):
        self.tmp.cleanup()

    def _compare(self, *, retail: bytes, decomp: bytes, align: int = 8,
                 section: str = ".rodata"):
        r = _make_data_object(self.d / "retail.o", retail, align=align,
                              section=section)
        d = _make_data_object(self.d / "decomp.o", decomp, align=align,
                              section=section)
        return check_data_sections(r, d)

    def test_four_aligned_tail_in_eight_aligned_split_is_accepted(self):
        literal = b"WorkThreadSystem\0"
        self.assertEqual(len(literal), 0x11)
        result = self._compare(retail=literal + bytes(3), decomp=literal)
        self.assertTrue(result.ok, [s.detail for s in result.sections])
        rodata = next(s for s in result.sections if s.name == ".rodata")
        self.assertIn("+ 0x3 retail link tail padding", rodata.detail)

    def test_eight_aligned_tail_is_still_accepted(self):
        literal = b"WorkThreadSystem\0"
        result = self._compare(retail=literal + bytes(7), decomp=literal)
        self.assertTrue(result.ok, [s.detail for s in result.sections])

    def test_one_byte_sdata2_tail_is_accepted(self):
        # ocThread: retail .sdata2:0x80665BC8 is "thread\0\0" (8 bytes);
        # the plain literal emits 7 bytes and the last byte is link padding.
        literal = b"thread\0"
        result = self._compare(retail=literal + b"\0", decomp=literal,
                               section=".sdata2")
        self.assertTrue(result.ok, [s.detail for s in result.sections])
        sdata2 = next(s for s in result.sections if s.name == ".sdata2")
        self.assertIn("+ 0x1 retail link tail padding", sdata2.detail)

    def test_nonzero_tail_is_data_not_padding(self):
        literal = b"WorkThreadSystem\0"
        result = self._compare(retail=literal + b"\0\x01\0", decomp=literal)
        self.assertFalse(result.ok)

    def test_tail_at_or_above_section_alignment_is_not_padding(self):
        literal = b"WorkThreadSystem\0"
        result = self._compare(retail=literal + bytes(8), decomp=literal)
        self.assertFalse(result.ok)

    def test_unaligned_retail_end_is_not_padding(self):
        literal = b"abc\0"
        result = self._compare(retail=literal + bytes(3), decomp=literal)
        self.assertFalse(result.ok)


if __name__ == "__main__":
    unittest.main()
