"""Tests for linked jump-table word hydration."""

from __future__ import annotations

import hashlib
import struct
import tempfile
import unittest
from pathlib import Path

from tools.ppc_equivalence.address_space import rom_image_region
from tools.ppc_equivalence.ir import R_PPC_ADDR32
from tools.ppc_equivalence.jump_table_image import (
    JumpTableImageError,
    hydrate_jump_table,
    readonly_image_obligation,
)
from tools.ppc_equivalence.tests.test_allocatable_sections import build_elf_with_rodata
from tools.ppc_equivalence.tests.test_dol_symbols import _build_dol


_TABLE_BASE = 0x80200000
_TARGETS = (0x80001000, 0x80001004, 0x80001008, 0x8000100C)


def _table_bytes(targets: tuple[int, ...] = _TARGETS) -> bytes:
    return b"".join(struct.pack(">I", target) for target in targets)


class JumpTableImageTests(unittest.TestCase):
    def test_hydrates_linked_elf_rodata(self) -> None:
        rodata = _table_bytes()
        elf = build_elf_with_rodata(
            rodata=rodata,
            rodata_addr=_TABLE_BASE,
            e_type=2,
        )
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "linked.elf"
            path.write_bytes(elf)
            image = hydrate_jump_table(
                base=_TABLE_BASE,
                entry_count=len(_TARGETS),
                elf_path=path,
            )
        self.assertEqual(image.source, "linked-elf")
        self.assertEqual(image.base, _TABLE_BASE)
        self.assertEqual(image.entry_count, 4)
        self.assertEqual(image.entry_size, 4)
        self.assertEqual(image.words, _TARGETS)
        self.assertEqual(image.image_sha256, hashlib.sha256(rodata).hexdigest())
        self.assertEqual(image.artifact_path, str(path.resolve()))

    def test_hydrates_linked_dol_data(self) -> None:
        rodata = _table_bytes()
        text = b"\x00" * 8
        blob = _build_dol(
            text_sections=[(0x80004000, 0x100, text)],
            data_sections=[(_TABLE_BASE, 0x100 + len(text), rodata)],
        )
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "main.dol"
            path.write_bytes(blob)
            image = hydrate_jump_table(
                base=_TABLE_BASE,
                entry_count=len(_TARGETS),
                dol_path=path,
            )
        self.assertEqual(image.source, "linked-dol")
        self.assertEqual(image.words, _TARGETS)

    def test_prefers_elf_over_dol(self) -> None:
        elf_rodata = _table_bytes((0x80002000, 0x80002004))
        dol_rodata = _table_bytes((0x80003000, 0x80003004))
        elf = build_elf_with_rodata(
            rodata=elf_rodata,
            rodata_addr=_TABLE_BASE,
            e_type=2,
        )
        dol = _build_dol(
            text_sections=[(0x80004000, 0x100, b"\x00" * 8)],
            data_sections=[(_TABLE_BASE, 0x108, dol_rodata)],
        )
        with tempfile.TemporaryDirectory() as tmp:
            elf_path = Path(tmp) / "linked.elf"
            dol_path = Path(tmp) / "main.dol"
            elf_path.write_bytes(elf)
            dol_path.write_bytes(dol)
            image = hydrate_jump_table(
                base=_TABLE_BASE,
                entry_count=2,
                elf_path=elf_path,
                dol_path=dol_path,
            )
        self.assertEqual(image.source, "linked-elf")
        self.assertEqual(image.words, (0x80002000, 0x80002004))

    def test_hydrates_object_rodata_without_relocs(self) -> None:
        rodata = _table_bytes()
        elf = build_elf_with_rodata(
            rodata=rodata,
            rodata_addr=_TABLE_BASE,
            e_type=1,
        )
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "table.o"
            path.write_bytes(elf)
            image = hydrate_jump_table(
                base=_TABLE_BASE,
                entry_count=len(_TARGETS),
                elf_path=path,
            )
        self.assertEqual(image.source, "object-rodata")
        self.assertEqual(image.words, _TARGETS)

    def test_rejects_object_rodata_with_unresolved_addr32(self) -> None:
        elf = build_elf_with_rodata(
            rodata=bytes(16),
            rodata_addr=_TABLE_BASE,
            rodata_relocations=(
                (0, "case0", R_PPC_ADDR32, 0),
                (4, "case1", R_PPC_ADDR32, 0),
            ),
        )
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "reloc.o"
            path.write_bytes(elf)
            with self.assertRaises(JumpTableImageError) as ctx:
                hydrate_jump_table(
                    base=_TABLE_BASE,
                    entry_count=2,
                    elf_path=path,
                )
        self.assertIn("unresolved ADDR32", str(ctx.exception))

    def test_rejects_unaligned_targets(self) -> None:
        rodata = struct.pack(">I", 0x80001001)
        elf = build_elf_with_rodata(
            rodata=rodata,
            rodata_addr=_TABLE_BASE,
            e_type=2,
        )
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "bad.elf"
            path.write_bytes(elf)
            with self.assertRaises(JumpTableImageError) as ctx:
                hydrate_jump_table(base=_TABLE_BASE, entry_count=1, elf_path=path)
        self.assertIn("word-aligned", str(ctx.exception))

    def test_rejects_missing_image(self) -> None:
        with self.assertRaises(JumpTableImageError) as ctx:
            hydrate_jump_table(base=_TABLE_BASE, entry_count=2)
        self.assertIn("no linked image supplied", str(ctx.exception))

    def test_readonly_image_obligation(self) -> None:
        rodata = _table_bytes()
        elf = build_elf_with_rodata(
            rodata=rodata,
            rodata_addr=_TABLE_BASE,
            e_type=2,
        )
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "linked.elf"
            path.write_bytes(elf)
            image = hydrate_jump_table(
                base=_TABLE_BASE,
                entry_count=len(_TARGETS),
                elf_path=path,
            )
            obligation = readonly_image_obligation(image)
        self.assertEqual(obligation["base"], _TABLE_BASE)
        self.assertEqual(obligation["end"], _TABLE_BASE + 15)
        self.assertEqual(obligation["sha256"], image.image_sha256)
        self.assertEqual(obligation["entry_count"], 4)
        self.assertEqual(obligation["source"], "linked-elf")
        self.assertEqual(obligation["artifact_path"], str(path.resolve()))
        self.assertEqual(obligation["words"], list(_TARGETS))

    def test_obligation_bytes_match_rom_image_region(self) -> None:
        rodata = _table_bytes()
        region = rom_image_region(_TABLE_BASE, rodata, label="jump-table")
        elf = build_elf_with_rodata(
            rodata=rodata,
            rodata_addr=_TABLE_BASE,
            e_type=2,
        )
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "linked.elf"
            path.write_bytes(elf)
            image = hydrate_jump_table(
                base=_TABLE_BASE,
                entry_count=len(_TARGETS),
                elf_path=path,
            )
        self.assertEqual(image.image_sha256, region.image_sha256)


if __name__ == "__main__":
    unittest.main()
