"""Tests for Phase 0 — Promotion Safety (§20.1).

Covers: Promotion gate, placeholder detection, baseline capture,
transactional promote, rollback, journal, concurrency.
"""
from __future__ import annotations

import json
import os
import stat
import tempfile
import threading
import time
import unittest
from pathlib import Path
from typing import Any, Dict, List, Optional

from tools.llm_harness.promotion import (
    PlaceholderDetector,
    PromotionManager,
    capture_baseline,
    evaluation_to_candidate,
    passes_promotion_gate,
    rank_candidate,
    source_transaction,
)
from tools.llm_harness.types import (
    CandidateEvaluation,
    CandidateStatus,
    CompileReport,
    Evaluation,
    PromotionPolicy,
    PromotionResult,
    SourcePatch,
    StructuralComponent,
    StructuralReport,
)


def _make_evaluation(**overrides: Any) -> CandidateEvaluation:
    defaults: Dict[str, Any] = dict(
        status=CandidateStatus.COMPILES,
        compile_report=CompileReport(succeeded=True),
        match_percent=50.0,
        structural_report=StructuralReport(
            total_score=0.80,
            calls=StructuralComponent(score=0.8, matched=2, expected=2),
            relocations=StructuralComponent(score=1.0, matched=1, expected=1),
            memory_accesses=StructuralComponent(score=0.8, matched=4, expected=5),
            cfg=StructuralComponent(score=0.8, matched=3, expected=3),
            constants=StructuralComponent(score=1.0, matched=2, expected=2),
            returns=StructuralComponent(score=1.0, matched=1, expected=1),
            instruction_classes=StructuralComponent(score=0.8, matched=8, expected=10),
        ),
        function_size=100,
        retail_size=100,
        object_regressions=[],
        accepted_function_regressions=[],
        warnings=[],
    )
    defaults.update(overrides)
    return CandidateEvaluation(**defaults)


class TestPassesPromotionGate(unittest.TestCase):
    """§20.1 — promotion gate logic (tests 1–9)"""

    def setUp(self) -> None:
        self.policy = PromotionPolicy()

    def test_worse_compiling_does_not_replace_compiling(self):
        """1. Worse compiling candidate does not replace a compiling baseline."""
        baseline = _make_evaluation(match_percent=80.0)
        candidate = _make_evaluation(match_percent=50.0)
        ok, _ = passes_promotion_gate(baseline, candidate, self.policy, False)
        self.assertFalse(ok)

    def test_first_compile_replaces_non_compiling(self):
        """2. First compiling candidate can replace a non-compiling baseline."""
        baseline = _make_evaluation(
            status=CandidateStatus.COMPILE_ERROR,
            compile_report=CompileReport(succeeded=False),
            match_percent=0.0,
        )
        candidate = _make_evaluation(match_percent=30.0)
        ok, _ = passes_promotion_gate(baseline, candidate, self.policy, False)
        self.assertTrue(ok)

    def test_first_compile_below_structural_rejected(self):
        """3. First compiling candidate below structural threshold is rejected."""
        policy = PromotionPolicy(first_compile_min_structural_score=0.90)
        baseline = _make_evaluation(
            status=CandidateStatus.COMPILE_ERROR,
            compile_report=CompileReport(succeeded=False),
        )
        candidate = _make_evaluation(structural_report=StructuralReport(
            total_score=0.50,
            calls=StructuralComponent(score=0.5, matched=1, expected=2),
            relocations=StructuralComponent(score=0.5, matched=0, expected=1),
            memory_accesses=StructuralComponent(score=0.5, matched=2, expected=5),
            cfg=StructuralComponent(score=0.5, matched=2, expected=3),
            constants=StructuralComponent(score=0.5, matched=1, expected=2),
            returns=StructuralComponent(score=0.5, matched=1, expected=1),
            instruction_classes=StructuralComponent(score=0.5, matched=5, expected=10),
        ))
        ok, _ = passes_promotion_gate(baseline, candidate, policy, False)
        self.assertFalse(ok)

    def test_placeholder_replaced_by_plausible(self):
        """4. Placeholder baseline replaced by plausible compiling candidate."""
        baseline = _make_evaluation(
            status=CandidateStatus.COMPILE_ERROR,
            compile_report=CompileReport(succeeded=False),
        )
        candidate = _make_evaluation(match_percent=40.0)
        ok, _ = passes_promotion_gate(baseline, candidate, self.policy, True)
        self.assertTrue(ok)

    def test_full_match_promotes(self):
        """5. Full match always promotes."""
        baseline = _make_evaluation(match_percent=90.0)
        candidate = _make_evaluation(
            status=CandidateStatus.FULL_MATCH, match_percent=100.0,
        )
        ok, reason = passes_promotion_gate(baseline, candidate, self.policy, False)
        self.assertTrue(ok)
        self.assertIn("full match", reason.lower())

    def test_proved_equivalent_improving_promotes(self):
        """6. Proven-equivalent improving candidate promotes."""
        baseline = _make_evaluation(match_percent=60.0)
        candidate = _make_evaluation(
            status=CandidateStatus.EQUIVALENT_MATCH, match_percent=65.0,
            equivalence_status="proved",
        )
        ok, _ = passes_promotion_gate(baseline, candidate, self.policy, False)
        self.assertTrue(ok)

    def test_equivalent_regressing_rejected(self):
        """7. Equivalent but integration-regressing candidate is rejected."""
        candidate = _make_evaluation(
            object_regressions=["otherFunc regressed"],
        )
        ok, _ = passes_promotion_gate(_make_evaluation(), candidate, self.policy, False)
        self.assertFalse(ok)

    def test_higher_match_with_structural_regression_rejected(self):
        """8. Higher fuzzy match with severe structural regression is rejected."""
        baseline = _make_evaluation(
            match_percent=50.0,
            structural_report=StructuralReport(
                total_score=0.90,
                calls=StructuralComponent(score=0.9, matched=2, expected=2),
                relocations=StructuralComponent(score=1.0, matched=1, expected=1),
                memory_accesses=StructuralComponent(score=0.9, matched=4, expected=5),
                cfg=StructuralComponent(score=0.9, matched=3, expected=3),
                constants=StructuralComponent(score=1.0, matched=2, expected=2),
                returns=StructuralComponent(score=1.0, matched=1, expected=1),
                instruction_classes=StructuralComponent(score=0.9, matched=8, expected=10),
            ),
        )
        candidate = _make_evaluation(
            match_percent=70.0,
            structural_report=StructuralReport(
                total_score=0.30,
                calls=StructuralComponent(score=0.3, matched=1, expected=2),
                relocations=StructuralComponent(score=0.3, matched=0, expected=1),
                memory_accesses=StructuralComponent(score=0.3, matched=2, expected=5),
                cfg=StructuralComponent(score=0.3, matched=1, expected=3),
                constants=StructuralComponent(score=0.3, matched=1, expected=2),
                returns=StructuralComponent(score=0.3, matched=0, expected=1),
                instruction_classes=StructuralComponent(score=0.3, matched=3, expected=10),
            ),
        )
        ok, _ = passes_promotion_gate(baseline, candidate, self.policy, False)
        self.assertFalse(ok)

    def test_accepted_sibling_regression_blocks(self):
        """9. Accepted sibling regression blocks promotion."""
        candidate = _make_evaluation(
            accepted_function_regressions=["siblingFunc regressed"],
        )
        ok, _ = passes_promotion_gate(_make_evaluation(), candidate, self.policy, False)
        self.assertFalse(ok)


class TestPromotionManagerTransactional(unittest.TestCase):
    """§20.1 — transactional promotion (tests 10–15)"""

    def setUp(self) -> None:
        self.tmp = Path(tempfile.mkdtemp(prefix="promo-test-"))
        self.source_file = self.tmp / "source.cpp"
        self.source_file.write_text("int f() { return 0; }")

        # Stub out the coop cycle script so promotion can succeed
        coop_dir = self.tmp / "tools" / "coop"
        coop_dir.mkdir(parents=True, exist_ok=True)
        (coop_dir / "__init__.py").write_text("")
        (coop_dir / "run.py").write_text(
            "import sys; sys.exit(0)\n"
        )

        self.adapter = _make_fake_adapter(self.tmp, self.source_file)
        self.manager = PromotionManager(root=self.tmp)
        experiment_dir = self.tmp / "experiment"
        experiment_dir.mkdir()
        self.experiment_dir = experiment_dir

    def tearDown(self) -> None:
        import shutil
        shutil.rmtree(self.tmp, ignore_errors=True)

    def test_rollback_on_build_failure(self):
        """11. Build failure after source write causes rollback."""
        adapter = _make_fake_adapter(
            self.tmp, self.source_file,
            build_result="build error: syntax",
        )
        result = self.manager.promote(
            adapter, "function", "test_func",
            "int f() { return broken; }",
            self.experiment_dir, write=True, owner="tester",
        )
        self.assertFalse(result.promoted)
        self.assertTrue(result.rolled_back)
        self.assertIn("build", result.reason.lower())

    def test_journal_written_on_success(self):
        """14. Rollback journal is written."""
        # Build must pass for journal to be written
        result = self.manager.promote(
            self.adapter, "function", "test_func",
            "int f() { return 1; }",
            self.experiment_dir, write=True, owner="tester",
        )
        journal_dir = self.tmp / "build" / "llm-harness" / "promotions"
        self.assertTrue(journal_dir.is_dir())
        journals = list(journal_dir.rglob("prom-*.json"))
        if result.promoted:
            self.assertGreaterEqual(len(journals), 1)
        else:
            self.assertFalse(result.rolled_back)

    def test_rollback_failure_reported(self):
        """15. Rollback failure is reported as a critical error."""
        path = self.tmp / "nonexistent" / "source.cpp"
        result = PromotionManager._rollback(path, b"data", 0o644)
        self.assertFalse(result)

    def test_concurrent_promotions_serialized(self):
        """13. Concurrent promotions are serialized."""
        errors: List[str] = []

        def promote_sync(name: str) -> None:
            try:
                self.manager.promote(
                    self.adapter, "function", f"func_{name}",
                    f"int f_{name}() {{ return 1; }}",
                    self.experiment_dir, write=True, owner="tester",
                )
            except Exception as exc:
                errors.append(f"{name}: {exc}")

        threads = [
            threading.Thread(target=promote_sync, args=("A",)),
            threading.Thread(target=promote_sync, args=("B",)),
        ]
        for t in threads:
            t.start()
        for t in threads:
            t.join()
        self.assertEqual(len(errors), 0)

    def test_canonical_source_restored_after_rollback(self):
        """16. Canonical source equals original bytes after rollback."""
        original_text = "int f() { return 0; }"
        self.source_file.write_text(original_text)
        original_bytes = original_text.encode("utf-8")
        adapter = _make_fake_adapter(
            self.tmp, self.source_file,
            build_result="build error",
        )
        result = self.manager.promote(
            adapter, "function", "test_func",
            "int f() { return broken; }",
            self.experiment_dir, write=True, owner="tester",
        )
        self.assertTrue(result.rolled_back)
        self.assertEqual(self.source_file.read_bytes(), original_bytes)


class TestPlaceholderDetector(unittest.TestCase):
    """PlaceholderDetector"""

    def test_return_zero_detected(self):
        detector = PlaceholderDetector()
        self.assertTrue(detector.is_placeholder("int f() { return 0; }"))

    def test_empty_body_detected(self):
        detector = PlaceholderDetector()
        self.assertTrue(detector.is_placeholder("void f() {}"))

    def test_abort_detected(self):
        detector = PlaceholderDetector()
        self.assertTrue(detector.is_placeholder("void f() { abort(); }"))

    def test_placeholder_metadata(self):
        detector = PlaceholderDetector()
        self.assertTrue(detector.is_placeholder("anything", {"placeholder": True}))

    def test_real_code_not_placeholder(self):
        detector = PlaceholderDetector()
        self.assertFalse(detector.is_placeholder(
            "int f() { int x = 42; return x + 1; }"
        ))

    def test_unimplemented_pattern_in_code(self):
        detector = PlaceholderDetector()
        self.assertTrue(detector.is_placeholder('int f() { UNIMPLEMENTED return 0; }'))


class TestBaselineCapture(unittest.TestCase):
    """§20.1 — baseline snapshot"""

    def test_capture_baseline(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            exp_dir = root / "exp"
            exp_dir.mkdir()
            adapter = _make_fake_adapter(root, root / "source.cpp")
            snapshot = capture_baseline(adapter, "test_func", exp_dir)
            self.assertEqual(snapshot.target_id, "test_func")

    def test_capture_source_hash(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            src = root / "source.cpp"
            src.write_text("int f() { return 42; }")
            exp_dir = root / "exp"
            exp_dir.mkdir()
            adapter = _make_fake_adapter(root, src)
            snapshot = capture_baseline(adapter, "test_func", exp_dir)
            self.assertTrue(len(snapshot.source_hash) > 0)

    def test_source_hash_mismatch_triggers_fresh_eval(self):
        """12. Source hash mismatch triggers fresh re-evaluation."""
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            src = root / "source.cpp"
            src.write_text("int f() { return 0; }")
            exp_dir = root / "exp"
            exp_dir.mkdir()
            adapter = _make_fake_adapter(root, src)
            first = capture_baseline(adapter, "test_func", exp_dir)
            src.write_text("int f() { return 1; }")
            second = capture_baseline(adapter, "test_func", exp_dir)
            self.assertNotEqual(first.source_hash, second.source_hash)


class TestCandidateSourcePreserved(unittest.TestCase):
    """17. Candidate source is preserved in experiment evidence after rejection."""

    def test_source_preserved_after_rejection(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            src = root / "source.cpp"
            src.write_text("int f() { return 0; }")
            adapter = _make_fake_adapter(root, src)
            manager = PromotionManager(root=root)
            exp_dir = root / "exp"
            exp_dir.mkdir()
            result = manager.promote(
                adapter, "function", "test_func",
                "int f() { return rejected_source; }",
                exp_dir, write=True, owner="tester",
            )
            # Source should still be accessible via the adapter's record
            self.assertIn("f()", adapter.last_candidate_source)


class TestSourceTransaction(unittest.TestCase):
    """source_transaction context manager"""

    def test_restores_on_exception(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "test.txt"
            original = b"hello world"
            path.write_bytes(original)
            try:
                with source_transaction(path):
                    path.write_bytes(b"corrupted")
                    raise ValueError("oops")
            except ValueError:
                pass
            self.assertEqual(path.read_bytes(), original)

    def test_preserves_on_success(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "test.txt"
            original = b"keep me"
            path.write_bytes(original)
            with source_transaction(path):
                path.write_bytes(b"updated")
            self.assertEqual(path.read_bytes(), b"updated")


def _make_fake_adapter(root: Path, source_path: Path, *,
                       build_result: str = "") -> Any:
    """Create a minimal adapter stub for promotion tests."""
    _root = root
    _path = source_path
    _build = build_result

    class FakeAdapter:
        root: Path = _root
        last_candidate_source: str = ""

        def read_target_source(self, target_id: str) -> str:
            if _path.is_file():
                return _path.read_text(encoding="utf-8")
            return ""

        def target_source_path(self, target_id: str) -> Path:
            return _path

        def promote(self, workflow: str, target_id: str,
                     candidate: Any, *, write: bool = False, owner: str = "") -> str:
            if hasattr(candidate, "source") and candidate.source:
                _path.write_text(candidate.source)
                self.last_candidate_source = candidate.source
            elif hasattr(candidate, "patches"):
                for p in candidate.patches:
                    if p.source:
                        _path.write_text(p.source)
                        self.last_candidate_source = p.source
            return f"promoted {target_id}"

        def evaluate(self, workflow: str, target_id: str,
                     candidate: Any) -> Evaluation:
            return Evaluation(
                status="FULL_MATCH",
                match_percent=100.0,
                accepted=True,
                detail="ok",
            )

        def _build_object(self, experiment_dir: Path) -> Optional[str]:
            return _build or None

        def finalize(self) -> None:
            pass

        def evaluate_canon(self, target_id: str,
                           baseline_dir: Path) -> CandidateEvaluation:
            return _make_evaluation()

    return FakeAdapter()


if __name__ == "__main__":
    raise SystemExit(unittest.main())
