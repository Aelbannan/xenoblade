"""Tests for benchmark infrastructure (§21)."""
from __future__ import annotations

import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from tools.llm_harness.benchmark import (
    AblationConfig,
    ABLATIONS,
    AcceptanceTargets,
    BenchmarkDataset,
    BenchmarkEntry,
    BenchmarkMetrics,
    BenchmarkRun,
    HiddenSourceConfig,
    SurvivalObservation,
    check_acceptance,
    compute_promotion_survival,
)


class TestBenchmarkDataset(unittest.TestCase):
    """§21.1 — Dataset"""

    def setUp(self):
        self.dataset = BenchmarkDataset(entries=[
            BenchmarkEntry(
                target_id="Foo__bar", name="Foo::bar", size_bytes=100,
                num_blocks=3, num_calls=2, has_loops=False,
                member_access_count=4, global_access=True, uses_fp=False,
                existing_match_status="none", class_name="Foo",
            ),
            BenchmarkEntry(
                target_id="Baz__qux", name="Baz::qux", size_bytes=200,
                num_blocks=5, num_calls=1, has_loops=True,
                member_access_count=2, global_access=False, uses_fp=True,
                existing_match_status="full_match", class_name="Baz",
            ),
        ])

    def test_add_entry(self):
        ds = BenchmarkDataset()
        ds.add(self.dataset.entries[0])
        self.assertEqual(len(ds.entries), 1)

    def test_filter(self):
        filtered = self.dataset.filter(lambda e: e.size_bytes > 150)
        self.assertEqual(len(filtered.entries), 1)
        self.assertEqual(filtered.entries[0].target_id, "Baz__qux")

    def test_stratified_sample(self):
        sampled = self.dataset.stratified_sample(target_count=10)
        self.assertGreaterEqual(len(sampled.entries), 1)

    def test_summary(self):
        s = self.dataset.summary()
        self.assertEqual(s["total"], 2)
        self.assertEqual(s["with_fp"], 1)


class TestHiddenSourceConfig(unittest.TestCase):
    """§21.2 — Hidden-source setup"""

    def test_default_config(self):
        cfg = HiddenSourceConfig()
        self.assertTrue(cfg.preserve_headers)
        self.assertTrue(cfg.remove_attempt_history)
        self.assertEqual(cfg.isolate_branch, "benchmark-hidden")

    def test_verify_no_leak(self):
        cfg = HiddenSourceConfig()
        issues = cfg.verify_no_leak("some source code", "test_func")
        self.assertEqual(len(issues), 0)

    def test_verify_detects_leak(self):
        cfg = HiddenSourceConfig()
        issues = cfg.verify_no_leak("function test_func() {}", "test_func")
        self.assertGreater(len(issues), 0)


class TestAblations(unittest.TestCase):
    """§21.3 — Ablation configurations"""

    def test_ablations_defined(self):
        self.assertGreaterEqual(len(ABLATIONS), 8)

    def test_each_ablation_has_name(self):
        for a in ABLATIONS:
            self.assertTrue(len(a.name) > 0)

    def test_each_ablation_has_description(self):
        for a in ABLATIONS:
            self.assertTrue(len(a.description) > 0)

    def test_raw_hex_ablation_disables_knowledge(self):
        raw = [a for a in ABLATIONS if a.name == "raw_hex_independent"]
        self.assertEqual(len(raw), 1)
        self.assertEqual(raw[0].overrides.get("knowledge.mode"), "none")


class TestBenchmarkMetrics(unittest.TestCase):
    """§21.4 — Metrics"""

    def test_metrics_defaults(self):
        m = BenchmarkMetrics(target_id="test", ablation_name="baseline")
        self.assertEqual(m.target_id, "test")
        self.assertFalse(m.compile_within_budget)

    def test_metrics_to_dict(self):
        m = BenchmarkMetrics(target_id="t", ablation_name="a",
                             best_match_percent=85.0, full_match=True)
        d = m.to_dict()
        self.assertEqual(d["target_id"], "t")
        self.assertEqual(d["best_match_percent"], 85.0)
        self.assertTrue(d["full_match"])


class TestBenchmarkRun(unittest.TestCase):
    """BenchmarkRun"""

    def test_empty_report(self):
        ds = BenchmarkDataset()
        run = BenchmarkRun(ds, ABLATIONS[0])
        report = run.report()
        self.assertEqual(report, {})

    def test_record_and_report(self):
        ds = BenchmarkDataset(entries=[
            BenchmarkEntry(target_id="t1", name="t1", size_bytes=100,
                           num_blocks=2, num_calls=1, has_loops=False,
                           member_access_count=0, global_access=False,
                           uses_fp=False, existing_match_status="none"),
        ])
        run = BenchmarkRun(ds, ABLATIONS[0])
        run.record(BenchmarkMetrics(
            target_id="t1", ablation_name="baseline",
            compile_within_budget=True, nonzero_match=True,
            best_match_percent=90.0, structural_alignment_score=0.85,
        ))
        report = run.report()
        self.assertAlmostEqual(report["compile_rate"], 1.0)
        self.assertAlmostEqual(report["nonzero_match_rate"], 1.0)

    def test_elapsed(self):
        run = BenchmarkRun(BenchmarkDataset(), ABLATIONS[0])
        self.assertGreaterEqual(run.elapsed(), 0)


class TestPromotionSurvival(unittest.TestCase):
    """§21.5 — Promotion survival rate"""

    def test_all_survived(self):
        obs = [
            SurvivalObservation("t1", "2026-01-01", True, True, "next_promotion"),
            SurvivalObservation("t2", "2026-01-02", True, False, "tu_closure"),
        ]
        rate = compute_promotion_survival(obs)
        self.assertAlmostEqual(rate, 1.0)

    def test_partial_survival(self):
        obs = [
            SurvivalObservation("t1", "2026-01-01", True, True, "next_promotion"),
            SurvivalObservation("t2", "2026-01-02", False, False, "full_build"),
        ]
        rate = compute_promotion_survival(obs)
        self.assertAlmostEqual(rate, 0.5)

    def test_empty(self):
        self.assertAlmostEqual(compute_promotion_survival([]), 0.0)


class TestAcceptanceTargets(unittest.TestCase):
    """§21.6 — Acceptance targets"""

    def test_check_acceptance_passes(self):
        baseline = {"compile_rate": 0.3, "repeated_diagnostic_rate": 0.2,
                     "nonzero_match_rate": 0.25, "full_match_rate": 0.1}
        candidate = {"compile_rate": 0.7, "repeated_diagnostic_rate": 0.05,
                      "nonzero_match_rate": 0.6, "full_match_rate": 0.15,
                      "promotion_survival_rate": 0.95}
        results = check_acceptance(baseline, candidate)
        passes = [r for r in results if r.startswith("PASS")]
        self.assertGreaterEqual(len(passes), 3)

    def test_check_acceptance_fails(self):
        baseline = {"compile_rate": 0.5, "repeated_diagnostic_rate": 0.1,
                     "nonzero_match_rate": 0.5, "full_match_rate": 0.2}
        candidate = {"compile_rate": 0.3, "repeated_diagnostic_rate": 0.3,
                      "nonzero_match_rate": 0.2, "full_match_rate": 0.1,
                      "promotion_survival_rate": 0.5}
        results = check_acceptance(baseline, candidate)
        fails = [r for r in results if r.startswith("FAIL")]
        self.assertGreaterEqual(len(fails), 3)


if __name__ == "__main__":
    raise SystemExit(unittest.main())
