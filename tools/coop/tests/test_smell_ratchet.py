"""Tests for the per-TU smell ratchet (smell_report --ratchet/--check) and the
derived clean tier (clean_tier.py)."""

from __future__ import annotations

import json
import pathlib
import sys
import tempfile
import unittest
from unittest import mock

ROOT = pathlib.Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT))

from tools.coop import clean_tier, smell_report  # noqa: E402
from tools.coop.smell_scan import Stats  # noqa: E402


def _stats(**kw) -> Stats:
    s = Stats()
    for k, v in kw.items():
        setattr(s, k, v)
    return s


class TestRatchetCompare(unittest.TestCase):
    def test_increase_is_regression(self):
        reg, new, loose = smell_report.ratchet_compare({"a.cpp": {"goto_count": 1}}, {"a.cpp": {"goto_count": 2}})
        self.assertEqual(len(reg), 1)
        self.assertIn("goto 1 → 2", reg[0])
        self.assertEqual((new, loose), ([], 0))

    def test_new_metric_on_known_tu_is_regression(self):
        reg, _, _ = smell_report.ratchet_compare({"a.cpp": {}}, {"a.cpp": {"void_ptr": 1}})
        self.assertEqual(len(reg), 1)

    def test_decrease_is_loosened_not_regression(self):
        reg, new, loose = smell_report.ratchet_compare({"a.cpp": {"void_ptr": 3}}, {"a.cpp": {"void_ptr": 1}})
        self.assertEqual((reg, new, loose), ([], [], 1))

    def test_new_tus(self):
        _, new, _ = smell_report.ratchet_compare({}, {"dirty.cpp": {"goto_count": 1}, "clean.cpp": {}})
        self.assertEqual(new, ["dirty.cpp"])


class TestRatchetCommand(unittest.TestCase):
    def setUp(self):
        self.td = tempfile.TemporaryDirectory()
        self.path = pathlib.Path(self.td.name) / "smell_baseline.json"
        self.patch = mock.patch.object(smell_report, "RATCHET", self.path)
        self.patch.start()
        self.patch_root = mock.patch.object(smell_report, "ROOT", pathlib.Path(self.td.name))
        self.patch_root.start()

    def tearDown(self):
        self.patch_root.stop()
        self.patch.stop()
        self.td.cleanup()

    def test_lowers_never_raises(self):
        smell_report.cmd_ratchet({"a.cpp": _stats(void_ptr=3, goto_count=1)}, init=True)
        # cleanup lowers the ceiling and adds a new TU
        rc = smell_report.cmd_ratchet({"a.cpp": _stats(void_ptr=1, goto_count=1), "b.cpp": _stats(goto_count=2)})
        self.assertEqual(rc, 0)
        tus = smell_report.load_ratchet(self.path)
        self.assertEqual(tus["a.cpp"], {"goto_count": 1, "void_ptr": 1})
        self.assertEqual(tus["b.cpp"], {"goto_count": 2})
        # a regression is refused and leaves the file untouched
        before = self.path.read_text()
        with mock.patch("sys.stderr"):
            rc = smell_report.cmd_ratchet({"a.cpp": _stats(void_ptr=2, goto_count=1), "b.cpp": _stats(goto_count=2)})
        self.assertEqual(rc, 1)
        self.assertEqual(self.path.read_text(), before)

    def test_check_flags_regression_and_unregistered(self):
        smell_report.cmd_ratchet({"a.cpp": _stats(void_ptr=1)}, init=True)
        self.assertEqual(smell_report._check_ratchet({"a.cpp": _stats(void_ptr=1)}), [])
        probs = smell_report._check_ratchet({"a.cpp": _stats(void_ptr=2), "n.cpp": _stats(goto_count=1)})
        self.assertEqual(len(probs), 2)

    def test_missing_baseline_fails(self):
        self.assertEqual(len(smell_report._check_ratchet({})), 1)


class TestCleanTier(unittest.TestCase):
    SRC = (
        "void A::Clean(int x) {\n"
        "    mX = x;\n"
        "}\n"
        "\n"
        "extern \"C\" void dirty_fn(void* self) {\n"
        "    goto out;\n"
        "out:;\n"
        "}\n"
        "\n"
        "void A::Unmatched() {\n"
        "}\n"
    )

    def test_metrics_mirror_report(self):
        self.assertEqual(clean_tier.CLEAN_METRICS, tuple(k for k, _ in smell_report.METRICS))

    def test_classify(self):
        targets = [
            {"id": "c", "source": "src/x.cpp", "function": "A::Clean(int)", "symbol": "Clean__1AFi",
             "status": "FULL_MATCH", "size": "0x10"},
            {"id": "d", "source": "src/x.cpp", "function": "dirty_fn", "symbol": "dirty_fn",
             "status": "EQUIVALENT_MATCH", "size": 32},
            {"id": "u", "source": "src/x.cpp", "function": "A::Unmatched()", "symbol": "Unmatched__1AFv",
             "status": "HIGH_MATCH", "size": 8},
            {"id": "m", "source": "src/x.cpp", "function": "A::Missing()", "symbol": "Missing__1AFv",
             "status": "FULL_MATCH", "size": 4},
            {"id": "o", "source": "other/y.cpp", "function": "f", "symbol": "f",
             "status": "FULL_MATCH", "size": 4},
        ]
        with tempfile.TemporaryDirectory() as td:
            root = pathlib.Path(td)
            (root / "src").mkdir()
            (root / "src" / "x.cpp").write_text(self.SRC)
            res = {r.id: r for r in clean_tier.classify(targets, ("src",), root)}
        self.assertNotIn("o", res)  # outside roots
        self.assertTrue(res["c"].clean)
        self.assertFalse(res["d"].clean)
        self.assertIn("goto_count", res["d"].smells)
        self.assertIn("extern_c_nonlbl_def", res["d"].smells)
        self.assertFalse(res["u"].clean)  # not accepted
        self.assertFalse(res["m"].located)
        s = clean_tier.summarize(list(res.values()))
        self.assertEqual((s.accepted, s.clean, s.dirty, s.unlocated), (3, 1, 1, 1))
        self.assertEqual((s.clean_bytes, s.dirty_bytes, s.unlocated_bytes), (16, 32, 4))
        self.assertEqual((s.tus, s.clean_tus), (1, 0))

    def test_unqualified_registry_name_matches_member(self):
        self.assertTrue(clean_tier._span_matches("CTitleAHelp::updateHelp", "updateHelp"))
        self.assertFalse(clean_tier._span_matches("updateHelp", "CTitleAHelp::updateHelp"))
        self.assertFalse(clean_tier._span_matches("B::Move", "A::Move"))
        self.assertTrue(clean_tier._span_matches("Foo__1AFv", "A::Foo", "Foo__1AFv"))


if __name__ == "__main__":
    unittest.main()
