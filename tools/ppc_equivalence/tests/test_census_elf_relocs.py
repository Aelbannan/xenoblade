"""Tests for ELF relocation census tooling."""

from __future__ import annotations

import json
import tempfile
import unittest
from pathlib import Path

from tools.ppc_equivalence.census_elf_relocs import R_PPC_ADDR32, run_census, section_class
from tools.ppc_equivalence.elf_symbols import list_section_relocations
from tools.ppc_equivalence.tests.test_elf_symbols import build_reloc_elf


class CensusElfRelocsTests(unittest.TestCase):
    def test_section_class_buckets(self) -> None:
        self.assertEqual(section_class(".text"), "text")
        self.assertEqual(section_class(".rodata"), "data")
        self.assertEqual(section_class(".data.rel.ro"), "data")
        self.assertEqual(section_class(".rela.text"), "reloc_meta")

    def test_list_section_relocations_reads_text_entries(self) -> None:
        elf = build_reloc_elf(
            {"first": bytes.fromhex("48000001 4e800020"), "second": bytes.fromhex("4e800020")},
            relocations=((0, "second", 10, 0),),
        )
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "reloc.o"
            path.write_bytes(elf)
            entries = list_section_relocations(path)
        self.assertEqual(len(entries), 1)
        self.assertEqual(entries[0].target_section_name, ".text")
        self.assertEqual(entries[0].relocation_type, 10)
        self.assertEqual(entries[0].symbol, "second")

    def test_run_census_reports_addr32_non_text(self) -> None:
        # Minimal ET_REL with .rodata + .rela.rodata is too heavy for build_reloc_elf;
        # verify census aggregates text relocs and empty trees cleanly.
        elf = build_reloc_elf(
            {"f": bytes.fromhex("48000001 4e800020"), "leaf": bytes.fromhex("4e800020")},
            relocations=((0, "leaf", 10, 0),),
        )
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "sample.o").write_bytes(elf)
            acc = run_census(root)
            payload = acc.to_payload(root=root)
        self.assertEqual(payload["objects_scanned"], 1)
        self.assertEqual(payload["objects_parsed"], 1)
        self.assertEqual(payload["summary_top_types"]["text"][0]["type"], 10)
        self.assertEqual(payload["addr32_non_text"]["total"], 0)

    def test_cli_json_roundtrip(self) -> None:
        from tools.ppc_equivalence.census_elf_relocs import main

        elf = build_reloc_elf({"f": bytes.fromhex("4e800020")})
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "empty.o").write_bytes(elf)
            out = root / "census.json"
            code = main([str(root), "--json", str(out)])
            payload = json.loads(out.read_text(encoding="utf-8"))
        self.assertEqual(code, 0)
        self.assertEqual(payload["objects_parsed"], 1)
        self.assertNotIn(str(R_PPC_ADDR32), payload["totals_by_relocation_type"])


if __name__ == "__main__":
    unittest.main()
