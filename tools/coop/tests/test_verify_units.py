"""Tests for the registry-drift verification gate (verify_units.py).

All comparison logic is exercised with stubbed `hexdiff --all --json` docs —
no real builds run in these tests.
"""

from __future__ import annotations

import pathlib
import sys
import unittest

ROOT = pathlib.Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT))

from tools.coop import verify_units  # noqa: E402


def _row(symbol: str, *, present: bool = True, match: bool = True,
         mismatch: int = 0, structural: int = 0, reg_swap: int = 0,
         retail_size: int = 40, decomp_size: int = 40) -> dict:
    row = {"symbol": symbol, "present": present}
    if present:
        row.update({"match": match, "mismatch": mismatch,
                    "structural": structural, "reg_swap": reg_swap,
                    "pure_reg_swap": 0, "retail_size": retail_size,
                    "decomp_size": decomp_size})
    return row


def _target(symbol: str, status: str, instruction_match: float = 100.0,
            unit: str = "kyoshin/U") -> dict:
    return {"function": "f", "symbol": symbol, "unit": unit,
            "source": "src/kyoshin/U.cpp", "status": status,
            "instruction_match": instruction_match}


def _doc(*rows: dict) -> dict:
    return {"unit": "main/kyoshin/U", "functions": list(rows),
            "matched": 0, "total": len(rows)}


class TestPercentFromRow(unittest.TestCase):
    def test_full_match_is_100(self):
        self.assertEqual(verify_units.percent_from_row(_row("s")), 100.0)

    def test_partial(self):
        # 10 instructions, 3 mismatched -> 70%.
        row = _row("s", match=False, mismatch=3, retail_size=40, decomp_size=40)
        self.assertAlmostEqual(verify_units.percent_from_row(row), 70.0)

    def test_size_difference_counts_against(self):
        # retail 8 instr vs decomp 5 -> total 8, 5 missed -> 37.5%
        # (mirrors the human --all column, e.g. notifySubA08).
        row = _row("s", match=False, mismatch=5, retail_size=32, decomp_size=20)
        self.assertAlmostEqual(verify_units.percent_from_row(row), 37.5)

    def test_absent_row_is_none(self):
        self.assertIsNone(verify_units.percent_from_row(_row("s", present=False)))


class TestExtractJsonBlob(unittest.TestCase):
    def test_skips_build_log(self):
        blob = '{"unit": "u", "functions": [], "matched": 0, "total": 0}'
        text = "[1/2] Building CXX object...\n" + blob + "\n"
        self.assertEqual(verify_units.extract_json_blob(text)["unit"], "u")

    def test_combined_stdout_stderr(self):
        blob = '{"unit": "main/u", "functions": [{"symbol": "s"}], "matched": 0, "total": 1}'
        text = "ninja: Entering directory\n" + blob + "\nwarning: something\n"
        doc = verify_units.extract_json_blob(text)
        self.assertEqual(len(doc["functions"]), 1)

    def test_missing_blob_raises(self):
        with self.assertRaises(ValueError):
            verify_units.extract_json_blob("build ok, no json here\n")


class TestCheckTarget(unittest.TestCase):
    def test_full_ok(self):
        f = verify_units.check_target(_target("s", "FULL_MATCH"), {"s": _row("s")})
        self.assertEqual(f["verdict"], "ok")

    def test_full_drift(self):
        row = _row("s", match=False, mismatch=5, structural=5, retail_size=32, decomp_size=20)
        f = verify_units.check_target(_target("s", "FULL_MATCH"), {"s": row})
        self.assertEqual(f["verdict"], "drift-full")
        self.assertAlmostEqual(f["current"], 37.5)

    def test_equiv_ok_when_above_recorded(self):
        row = _row("s", match=False, mismatch=1, retail_size=40, decomp_size=40)
        f = verify_units.check_target(_target("s", "EQUIVALENT_MATCH", 90.0), {"s": row})
        self.assertEqual(f["verdict"], "ok")

    def test_equiv_tolerates_rounding(self):
        # Recorded 91.0, current 90.95 -> within the default 0.1 tolerance.
        row = _row("s", match=False, mismatch=0, retail_size=4000, decomp_size=4000)
        f = verify_units.check_target(
            _target("s", "EQUIVALENT_MATCH", 91.0),
            {"s": {**row, "mismatch": 36, "match": False,
                   "retail_size": 4000, "decomp_size": 4000}})
        # 1000 instr, 36 missed -> 96.4%, well above; sanity of path only.
        self.assertEqual(f["verdict"], "ok")

    def test_equiv_drift(self):
        row = _row("s", match=False, mismatch=5, retail_size=40, decomp_size=40)
        f = verify_units.check_target(_target("s", "EQUIVALENT_MATCH", 90.0), {"s": row})
        self.assertEqual(f["verdict"], "drift-equiv")  # 50% < 90%

    def test_missing_symbol_is_not_in_object(self):
        f = verify_units.check_target(_target("s", "FULL_MATCH"), {})
        self.assertEqual(f["verdict"], "not-in-object")
        self.assertIsNone(f["current"])

    def test_absent_row_is_not_in_object(self):
        f = verify_units.check_target(_target("s", "EQUIVALENT_MATCH", 80.0),
                                      {"s": _row("s", present=False)})
        self.assertEqual(f["verdict"], "not-in-object")


class TestEvaluateUnit(unittest.TestCase):
    def test_mixed_unit(self):
        accepted = [_target("ok_full", "FULL_MATCH"),
                    _target("bad_full", "FULL_MATCH"),
                    _target("ok_equiv", "EQUIVALENT_MATCH", 50.0),
                    _target("gone", "FULL_MATCH")]
        doc = _doc(_row("ok_full"),
                   _row("bad_full", match=False, mismatch=2, structural=2),
                   _row("ok_equiv", match=False, mismatch=1, retail_size=40, decomp_size=40),
                   _row("stranger", match=False, mismatch=1))
        res = verify_units.evaluate_unit("kyoshin/U", doc, accepted)
        self.assertEqual(res["stats"]["accepted"], 4)
        self.assertEqual(res["stats"]["ok"], 2)
        self.assertEqual(len(res["drifts"]), 1)
        self.assertEqual(res["drifts"][0]["symbol"], "bad_full")
        self.assertEqual(len(res["not_in_object"]), 1)
        self.assertEqual(res["not_in_object"][0]["symbol"], "gone")
        # Unregistered mismatched row is informational, never drift.
        self.assertEqual(len(res["unregistered"]), 1)
        self.assertEqual(res["unregistered"][0]["symbol"], "stranger")

    def test_non_accepted_targets_never_indexed(self):
        by_unit = verify_units.accepted_targets([
            {"status": "HIGH_MATCH", "unit": "kyoshin/U", "source": "src/kyoshin/U.cpp",
             "symbol": "s", "function": "f"},
            {"status": "FULL_MATCH", "unit": "kyoshin/U", "source": "src/ctx/foo.ctx.cpp",
             "symbol": "c", "function": "g"},
        ])
        self.assertEqual(by_unit, {})

    def test_roots_filter(self):
        self.assertTrue(verify_units.unit_matches_roots(
            "kyoshin/U", ["src/kyoshin/U.cpp"], ["src/kyoshin"]))
        self.assertTrue(verify_units.unit_matches_roots(
            "main/kyoshin/U", ["src/kyoshin/U.cpp"], ["kyoshin"]))
        self.assertFalse(verify_units.unit_matches_roots(
            "kyoshin/U", ["src/kyoshin/U.cpp"], ["libs/monolib/src"]))
        self.assertTrue(verify_units.unit_matches_roots("kyoshin/U", [], []))


if __name__ == "__main__":
    unittest.main()
