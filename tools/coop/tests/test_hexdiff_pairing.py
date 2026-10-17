"""Regression tests for hexdiff symbol pairing.

`--all` used to pair retail functions to decomp ones by exact name, then plain
case-insensitive substring, then **by address offset**. Retail registers ctors
under short names (`__ct__CNumSelect`) while MWCC emits Itanium-length manglings
(`__ct__10CNumSelectFUc`); the length-prefix digits break the plain substring, so
`--all` fell through to the address fallback and paired unrelated functions,
reporting a real 100% ctor as 0%. `_resolve_candidates` (the `--symbol` path)
already had the digit-strip fallback; `--all` now reuses it.
"""

from __future__ import annotations

import pathlib
import sys
import unittest

ROOT = pathlib.Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT))

from tools.coop.hexdiff import _resolve_candidates  # noqa: E402
from tools.ppc_equivalence.elf_symbols import FunctionBytes  # noqa: E402


def _fn(name: str, value: int = 0) -> FunctionBytes:
    return FunctionBytes(
        name=name, path=pathlib.Path("x.o"), code=b"", base=value, value=value,
        size=0, section_index=0, section_name=".text", symbol_type=0,
    )


class TestResolveCandidates(unittest.TestCase):
    def test_exact_wins(self):
        fns = [_fn("Foo"), _fn("FooBar")]
        self.assertEqual([f.name for f in _resolve_candidates(fns, "Foo")], ["Foo"])

    def test_plain_substring_before_digit_strip(self):
        fns = [_fn("__ct__11CMCEffUpPrmFPQ34nw4r3lyt6Layout")]
        got = _resolve_candidates(fns, "__ct__CMCEffUpPrm")
        self.assertEqual(got[0].name, "__ct__11CMCEffUpPrmFPQ34nw4r3lyt6Layout")

    def test_digit_strip_fallback_for_short_retail_name(self):
        # The ctor-cluster case: length prefix `10`/`14` breaks plain substring.
        fns = [_fn("__ct__10CNumSelectFUc"), _fn("__ct__8CFooBaseFv")]
        got = _resolve_candidates(fns, "__ct__CNumSelect")
        self.assertEqual(len(got), 1)
        self.assertEqual(got[0].name, "__ct__10CNumSelectFUc")

    def test_no_match_returns_empty(self):
        fns = [_fn("__ct__10CNumSelectFUc")]
        self.assertEqual(_resolve_candidates(fns, "__ct__SomethingElse"), [])

    def test_case_insensitive(self):
        fns = [_fn("__ct__10CNumSelectFUc")]
        self.assertEqual(_resolve_candidates(fns, "__CT__10cnumselectfuc")[0].name,
                         "__ct__10CNumSelectFUc")


if __name__ == "__main__":
    unittest.main()
