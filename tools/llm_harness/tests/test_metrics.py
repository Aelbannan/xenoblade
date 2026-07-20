"""Unit tests for TimingRecorder and record fallback aggregation."""

from __future__ import annotations

import time
import unittest

from tools.llm_harness.metrics import (
    TimingRecorder,
    merge_timing_dicts,
    timings_from_records,
)


class TimingRecorderTests(unittest.TestCase):
    def test_measure_and_summary(self) -> None:
        recorder = TimingRecorder()
        with recorder.measure("llm"):
            time.sleep(0.01)
        recorder.add("ninja", 1.5)
        recorder.add("ninja", 0.5)
        summary = recorder.summary()
        self.assertGreaterEqual(summary["totals_seconds"]["llm"], 0.01)
        self.assertEqual(summary["counts"]["llm"], 1)
        self.assertAlmostEqual(summary["totals_seconds"]["ninja"], 2.0)
        self.assertEqual(summary["counts"]["ninja"], 2)
        self.assertEqual(summary["counts"]["smt"], 0)
        self.assertGreaterEqual(summary["total_seconds"], 2.0)

    def test_merge_into_writes_timings_key(self) -> None:
        recorder = TimingRecorder()
        recorder.add("objdiff", 0.25)
        state: dict = {}
        payload = recorder.merge_into(state)
        self.assertIn("timings", state)
        self.assertEqual(state["timings"], payload)
        self.assertAlmostEqual(payload["totals_seconds"]["objdiff"], 0.25)

    def test_timings_from_records_fallback(self) -> None:
        summary = timings_from_records(
            [
                {"duration_seconds": 1.0},
                {"duration_seconds": 2.5},
                {"error": "x"},
            ]
        )
        self.assertAlmostEqual(summary["totals_seconds"]["llm"], 3.5)
        self.assertEqual(summary["counts"]["llm"], 2)
        self.assertEqual(summary["totals_seconds"]["configure"], 0.0)

    def test_merge_timing_dicts(self) -> None:
        left = TimingRecorder()
        left.add("llm", 1.0)
        right = TimingRecorder()
        right.add("llm", 2.0)
        right.add("worktree_create", 0.5)
        merged = merge_timing_dicts(left.summary(), right.summary())
        self.assertAlmostEqual(merged["totals_seconds"]["llm"], 3.0)
        self.assertEqual(merged["counts"]["llm"], 2)
        self.assertAlmostEqual(merged["totals_seconds"]["worktree_create"], 0.5)


if __name__ == "__main__":
    unittest.main()
