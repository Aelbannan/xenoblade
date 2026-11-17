"""Shrink-only .text function-order gate (tools/coop/text_order_gate.py).

Object fixtures reuse the hand-built ELF writer from test_extab_match; no build.
"""

from __future__ import annotations

import tempfile
import unittest
from pathlib import Path

from tools.coop import text_order_gate as tg
from tools.coop.tests.test_extab_match import build_elf


def make(path: Path, funcs: list[tuple[str, int]]) -> Path:
    syms = [(name, ".text", addr, True) for name, addr in funcs]
    syms.append(("@1", "extab", 0, False))  # non-.text symbol: ignored
    return build_elf(path, text=b"\x60\0\0\0" * 16, extab=b"\0" * 8, index=b"",
                     symbols=syms, extab_relocs=[], index_relocs=[])


class OrderDiffTest(unittest.TestCase):
    def test_same_order(self):
        self.assertEqual(tg.order_diffs(["a", "b", "c"], ["a", "b", "c"]), 0)

    def test_one_moved(self):
        # ctor/dtor moved from the front to the back: two functions out of place.
        self.assertEqual(tg.order_diffs(["ct", "dt", "f", "g"], ["f", "g", "ct", "dt"]), 2)
        self.assertEqual(tg.order_diffs(["a", "b", "c", "d"], ["a", "c", "b", "d"]), 1)

    def test_only_common_names_count(self):
        # Renamed / side-only symbols are a naming problem, not an order problem.
        self.assertEqual(tg.order_diffs(["a", "x", "b"], ["a", "b", "y"]), 0)

    def test_text_functions_sorted_by_address(self):
        with tempfile.TemporaryDirectory() as d:
            p = make(Path(d) / "o.o", [("late", 0x20), ("early", 0), ("mid", 0x10)])
            self.assertEqual(tg.text_functions(p), ["early", "mid", "late"])

    def test_objects_end_to_end(self):
        with tempfile.TemporaryDirectory() as d:
            r = make(Path(d) / "r.o", [("ct", 0), ("f", 0x10), ("g", 0x20)])
            o = make(Path(d) / "d.o", [("f", 0), ("g", 0x10), ("ct", 0x20)])
            self.assertEqual(tg.order_diffs(tg.text_functions(r), tg.text_functions(r)), 0)
            self.assertEqual(tg.order_diffs(tg.text_functions(r), tg.text_functions(o)), 1)


class OrderGateTest(unittest.TestCase):
    def test_new_unit_regresses(self):
        regress, improve = tg.compare({}, {"u": 1})
        self.assertEqual(len(regress), 1)
        self.assertEqual(improve, [])

    def test_rise_regresses_fall_improves(self):
        self.assertEqual(len(tg.compare({"u": 1}, {"u": 2})[0]), 1)
        regress, improve = tg.compare({"u": 3, "v": 1}, {"u": 2})
        self.assertEqual(regress, [])
        self.assertEqual(len(improve), 2)

    def test_ratchet_shrink_only(self):
        self.assertEqual(tg.ratchet({"u": 3, "v": 1}, {"u": 2, "w": 5}), {"u": 2})
        self.assertEqual(tg.ratchet({"u": 1}, {"u": 4}), {"u": 1})


if __name__ == "__main__":
    unittest.main()
