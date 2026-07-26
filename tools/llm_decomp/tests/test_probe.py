"""Tests for tools/llm_decomp/probe.py."""
from __future__ import annotations

import json
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest import mock

from tools.llm_decomp.probe import eligible_targets, probe_targets

T1 = SimpleNamespace(id="us-1", unit="kyoshin/A", symbol="symA",
                     buildable=True, status="NOT_STARTED")
T2 = SimpleNamespace(id="us-2", unit="kyoshin/B", symbol="symB",
                     buildable=True, status="ACTIVE")


def _run_probe(fuzzy: float, equivalence: str = "EQUIVALENT",
               build_ok: bool = True, write: bool = False,
               targets=(T1,), out_dir=None):
    if out_dir is None:
        out_dir = Path(tempfile.mkdtemp())
    with mock.patch("tools.llm_decomp.probe.verify.build_unit",
                    return_value=(build_ok, "diag")), \
         mock.patch("tools.llm_decomp.probe._fuzzy_for_target",
                    return_value=(fuzzy, 3)) as fz, \
         mock.patch("tools.llm_decomp.probe.verify.run_equivalence",
                    return_value=equivalence) as eq, \
         mock.patch("tools.llm_decomp.probe._persist") as persist:
        rows = probe_targets(Path("/fake"), list(targets), write=write,
                             out_dir=out_dir)
    return rows, fz, eq, persist


class ProbeTest(unittest.TestCase):
    def test_compile_error(self) -> None:
        rows, _fz, eq, persist = _run_probe(0.0, build_ok=False)
        self.assertEqual(rows[0].status, "COMPILE_ERROR")
        self.assertFalse(rows[0].accepted)
        eq.assert_not_called()
        persist.assert_not_called()

    def test_full_match_skips_equivalence(self) -> None:
        rows, _fz, eq, _p = _run_probe(100.0)
        self.assertEqual(rows[0].status, "FULL_MATCH")
        self.assertTrue(rows[0].accepted)
        eq.assert_not_called()

    def test_equivalent_match(self) -> None:
        rows, _fz, _eq, _p = _run_probe(62.0, "EQUIVALENT")
        self.assertEqual(rows[0].status, "EQUIVALENT_MATCH")
        self.assertTrue(rows[0].accepted)

    def test_code_match_when_not_equivalent(self) -> None:
        rows, _fz, _eq, persist = _run_probe(62.0, "NOT_EQUIVALENT")
        self.assertEqual(rows[0].status, "CODE_MATCH")
        self.assertFalse(rows[0].accepted)
        persist.assert_not_called()

    def test_below_min_fuzzy_skips_equivalence(self) -> None:
        rows, _fz, eq, _p = _run_probe(30.0)
        self.assertEqual(rows[0].status, "CODE_MATCH")
        eq.assert_not_called()

    def test_write_persists_accepted_only(self) -> None:
        rows, _fz, _eq, persist = _run_probe(62.0, "EQUIVALENT", write=True)
        self.assertTrue(rows[0].written)
        persist.assert_called_once()
        rows2, _fz2, _eq2, persist2 = _run_probe(62.0, "EQUIVALENT",
                                                 write=False)
        self.assertFalse(rows2[0].written)
        persist2.assert_not_called()

    def test_exception_row_and_continue(self) -> None:
        with mock.patch("tools.llm_decomp.probe.verify.build_unit",
                        side_effect=[RuntimeError("boom"), (True, "")]), \
             mock.patch("tools.llm_decomp.probe._fuzzy_for_target",
                        return_value=(100.0, 0)):
            rows = probe_targets(Path("/fake"), [T1, T2],
                                 out_dir=Path(tempfile.mkdtemp()))
        self.assertEqual(rows[0].status, "PROBE_ERROR")
        self.assertEqual(rows[1].status, "FULL_MATCH")

    def test_results_files(self) -> None:
        with tempfile.TemporaryDirectory() as td:
            rows, _fz, _eq, _p = _run_probe(100.0, out_dir=Path(td))
            lines = (Path(td) / "results.jsonl").read_text().splitlines()
            self.assertEqual(len(lines), 1)
            self.assertEqual(json.loads(lines[0])["status"], "FULL_MATCH")
            summary = json.loads((Path(td) / "summary.json").read_text())
            self.assertEqual(summary["accepted"], 1)
            self.assertEqual(summary["by_status"], {"FULL_MATCH": 1})

    def test_eligible_targets_filters(self) -> None:
        targets = [
            SimpleNamespace(id="a", unit="u/A", symbol="s", buildable=True,
                            status="NOT_STARTED"),
            SimpleNamespace(id="b", unit="u/A", symbol="s2", buildable=True,
                            status="FULL_MATCH"),
            SimpleNamespace(id="c", unit="u/B", symbol="s3", buildable=False,
                            status="NOT_STARTED"),
        ]
        with mock.patch("tools.coop.lib.targets.load_targets",
                        return_value=targets), \
             mock.patch("tools.coop.lib.config.load_config",
                        return_value=mock.Mock()):
            out = eligible_targets(Path("/fake"))
            self.assertEqual([t.id for t in out], ["a"])
            out = eligible_targets(Path("/fake"), tu="u/B")
            self.assertEqual(out, [])


if __name__ == "__main__":
    unittest.main()
