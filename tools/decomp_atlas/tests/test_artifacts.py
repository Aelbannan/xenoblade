from __future__ import annotations

import unittest
from pathlib import Path

from tools.decomp_atlas.lib.artifacts import extract_artifacts
from tools.llm_harness.asm_listings import assembly_function_block


class AsmListingTests(unittest.TestCase):
    def test_assembly_function_block(self) -> None:
        text = (
            '.fn foo, global\n'
            "/* 80000000 */\tli r3, 1\n"
            ".endfn foo\n"
            ".fn bar, global\n"
            "/* 80000004 */\tblr\n"
            ".endfn bar\n"
        )
        block = assembly_function_block(text, "foo")
        self.assertIn(".fn foo", block)
        self.assertIn("li r3, 1", block)
        self.assertNotIn(".fn bar", block)

    def test_extract_falls_back_to_asm_listing(self) -> None:
        root = Path(__file__).resolve().parents[3]
        asm = root / "build" / "us" / "asm" / "PowerPC_EABI_Support" / "src" / "Runtime" / "__mem.s"
        if not asm.is_file():
            self.skipTest("retail asm listing not present")
        artifact = extract_artifacts(
            root=root,
            record={
                "target_id": "us-80004000",
                "display_name": "memcpy",
                "symbol": "memcpy",
                "region": "us",
                "unit": "PowerPC_EABI_Support/src/Runtime/__mem",
                "source": "libs/PowerPC_EABI_Support/src/Runtime/__mem.c",
                "target_object": "build/us/obj/missing/memcpy.o",
                "base_object": "build/us/src/missing/memcpy.o",
            },
        )
        self.assertIn(".fn memcpy", artifact["retail_asm"])
        self.assertGreater(len(artifact["retail_asm"]), 100)


if __name__ == "__main__":
    unittest.main()
