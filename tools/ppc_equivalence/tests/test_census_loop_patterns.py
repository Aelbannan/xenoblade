"""Tests for retail loop / jump-table pattern census."""

from __future__ import annotations

import json
import tempfile
import unittest
from pathlib import Path

from tools.ppc_equivalence.census_loop_patterns import (
    decode_function,
    parse_asm_functions,
    run_census,
)


COMPARE_AFFINE_FIXTURE = """
.text
.fn counted__Fv, global
/* 00000000 00000000  38 80 00 04 */	addi r4, r0, 0x4
.L_00000004:
/* 00000004 00000004  38 63 00 02 */	addi r3, r3, 0x2
/* 00000008 00000008  38 84 FF FF */	addi r4, r4, -0x1
/* 0000000C 0000000C  2C 04 00 00 */	cmpwi r4, 0x0
/* 00000010 00000010  40 82 FF F4 */	bne .L_00000004
/* 00000014 00000014  4E 80 00 20 */	blr
.endfn counted__Fv
"""

MEMORY_LOOP_FIXTURE = """
.text
.fn memset_words__Fv, global
/* 00000000 00000000  38 00 00 08 */	addi r0, r0, 0x8
/* 00000004 00000004  7C 09 03 A6 */	mtctr r0
.L_00000008:
/* 00000008 00000008  90 64 00 00 */	stw r3, 0x0(r4)
/* 0000000C 0000000C  38 84 00 04 */	addi r4, r4, 0x4
/* 00000010 00000010  42 00 FF F8 */	bdnz .L_00000008
/* 00000014 00000014  4E 80 00 20 */	blr
.endfn memset_words__Fv
"""

JUMP_TABLE_FIXTURE = """
.text
.fn dispatch__Fv, global
/* 00000000 00000000  28 00 00 02 */	cmplwi r0, 0x2
/* 00000004 00000004  54 00 10 3A */	slwi r0, r0, 2
/* 00000008 00000008  7C 63 00 2E */	lwzx r3, r3, r0
/* 0000000C 0000000C  7C 69 03 A6 */	mtctr r3
/* 00000010 00000010  4E 80 04 20 */	bctr
.endfn dispatch__Fv
"""


class CensusLoopPatternsTests(unittest.TestCase):
    def test_parse_and_scan_inline_fixtures(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            fixture = root / "fixture.s"
            fixture.write_text(
                COMPARE_AFFINE_FIXTURE + MEMORY_LOOP_FIXTURE + JUMP_TABLE_FIXTURE,
                encoding="utf-8",
            )
            acc = run_census(root)
            payload = acc.to_payload()
            self.assertEqual(payload["files_scanned"], 1)
            self.assertGreaterEqual(payload["totals"].get("compare-affine", 0), 1)
            self.assertGreaterEqual(payload["totals"].get("memory-loop", 0), 1)
            self.assertGreaterEqual(payload["totals"].get("jump-table", 0), 1)
            self.assertGreaterEqual(payload["text_store_loop_loops"], 1)

    def test_decode_function_roundtrip(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "one.s"
            path.write_text(MEMORY_LOOP_FIXTURE, encoding="utf-8")
            function = parse_asm_functions(path, root=Path(tmp))[0]
            instructions = decode_function(function)
            self.assertIsNotNone(instructions)
            assert instructions is not None
            self.assertEqual(len(instructions), 6)
            self.assertEqual(instructions[1].address, 0x4)

    def test_cli_json_roundtrip(self) -> None:
        from tools.ppc_equivalence.census_loop_patterns import main

        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "mini.s").write_text(JUMP_TABLE_FIXTURE, encoding="utf-8")
            out = root / "census.json"
            code = main([str(root), "--json", str(out)])
            payload = json.loads(out.read_text(encoding="utf-8"))
        self.assertEqual(code, 0)
        self.assertEqual(payload["files_scanned"], 1)
        self.assertGreaterEqual(payload["totals"].get("jump-table", 0), 1)

    def test_missing_root_exits_gracefully(self) -> None:
        from tools.ppc_equivalence.census_loop_patterns import main

        with tempfile.TemporaryDirectory() as tmp:
            missing = Path(tmp) / "missing"
            self.assertEqual(main([str(missing)]), 2)


if __name__ == "__main__":
    unittest.main()
