"""Tests for tools.llm_decomp.verify — verification pipeline.

No real toolchain in CI; we mock subprocess/ninja, object parsing, and
every external dependency.
"""
from __future__ import annotations

import os
import subprocess
import threading
import time
import unittest
from pathlib import Path
from unittest.mock import MagicMock, patch

from tools.coop.lib.object_size import ObjectSizeCheck
from tools.llm_decomp.contracts import (
    Baseline,
    Regression,
    SymbolBaseline,
    VerdictKind,
)
from tools.llm_decomp.paths import build_lock_path


class TestBuildLock(unittest.TestCase):
    """BuildLock serializes both in-process and interprocess."""

    def setUp(self) -> None:
        self.tmp = Path(self._mkdtemp())

    @staticmethod
    def _mkdtemp() -> str:
        import tempfile

        return tempfile.mkdtemp()

    def test_serialization_two_threads(self) -> None:
        """Two threads incrementing a shared counter under BuildLock reach 200."""
        from tools.llm_decomp.verify import BuildLock

        counter = [0]
        errors: list[Exception] = []

        def worker() -> None:
            for _ in range(50):
                try:
                    with BuildLock(self.tmp):
                        val = counter[0]
                        time.sleep(0.002)  # force yield
                        counter[0] = val + 1
                except Exception as exc:
                    errors.append(exc)

        t1 = threading.Thread(target=worker)
        t2 = threading.Thread(target=worker)
        t1.start()
        t2.start()
        t1.join()
        t2.join()

        if errors:
            self.fail(f"thread raised {errors[0]}")
        self.assertEqual(counter[0], 100, "counter must reach 100 (no lost updates)")

    def test_lock_file_created(self) -> None:
        from tools.llm_decomp.verify import BuildLock

        lock_path = build_lock_path(self.tmp)
        self.assertFalse(lock_path.exists())
        with BuildLock(self.tmp):
            self.assertTrue(lock_path.exists())
        self.assertTrue(lock_path.exists())  # file stays after release

    def test_reentrant_acquire_release(self) -> None:
        """Acquire and release twice without deadlock."""
        from tools.llm_decomp.verify import BuildLock

        with BuildLock(self.tmp):
            pass
        with BuildLock(self.tmp):
            pass

    def tearDown(self) -> None:
        import shutil

        shutil.rmtree(self.tmp, ignore_errors=True)


class TestEstimateFuzzy(unittest.TestCase):
    """estimate_fuzzy produces correct percentages."""

    def test_exact_match(self) -> None:
        from tools.llm_decomp.verify import estimate_fuzzy

        self.assertAlmostEqual(estimate_fuzzy(0, 100), 100.0)

    def test_half_match(self) -> None:
        from tools.llm_decomp.verify import estimate_fuzzy

        self.assertAlmostEqual(estimate_fuzzy(50, 100), 50.0)

    def test_no_instructions(self) -> None:
        from tools.llm_decomp.verify import estimate_fuzzy

        self.assertEqual(estimate_fuzzy(0, 0), 0.0)
        self.assertEqual(estimate_fuzzy(5, 0), 0.0)

    def test_bounds_clamped(self) -> None:
        from tools.llm_decomp.verify import estimate_fuzzy

        self.assertAlmostEqual(estimate_fuzzy(150, 100), 0.0)


class TestAddedSymbols(unittest.TestCase):
    """added_symbols detects symbols in current_obj not in baseline."""

    def test_no_added(self) -> None:
        from tools.llm_decomp.verify import added_symbols

        baseline = Baseline(
            unit="test",
            symbols={"func1": SymbolBaseline(byte_hash="a" * 64, mismatch_count=0)},
            text_size=100,
            text_budget=120,
            object_path="/tmp/fake.o",
        )
        fake_obj = MagicMock()

        def _make_fn(name: str) -> MagicMock:
            fn = MagicMock()
            fn.name = name
            fn.code = b"\x00\x00\x00\x00"
            return fn

        with patch(
            "tools.llm_decomp.verify.list_text_functions"
        ) as mock_list:
            mock_list.return_value = [
                _make_fn("func1"),
            ]

            result = added_symbols(baseline, fake_obj)
            self.assertEqual(result, [])

    def test_with_added(self) -> None:
        from tools.llm_decomp.verify import added_symbols

        baseline = Baseline(
            unit="test",
            symbols={"func1": SymbolBaseline(byte_hash="a" * 64, mismatch_count=0)},
            text_size=100,
            text_budget=120,
            object_path="/tmp/fake.o",
        )
        fake_obj = MagicMock()

        def _make_fn(name: str) -> MagicMock:
            fn = MagicMock()
            fn.name = name
            fn.code = b"\x00\x00\x00\x00"
            return fn

        with patch(
            "tools.llm_decomp.verify.list_text_functions"
        ) as mock_list:
            mock_list.return_value = [
                _make_fn("func1"),
                _make_fn("func2"),
                _make_fn("func3"),
            ]

            result = added_symbols(baseline, fake_obj)
            self.assertEqual(result, ["func2", "func3"])


class TestSweep(unittest.TestCase):
    """sweep produces correct Regressions."""

    def setUp(self) -> None:
        self.zero_code = b"\x00\x00\x00\x00"
        self.retail_code = b"\x00\x00\x00\x01"
        self.current_code = b"\x00\x00\x00\x02"

        import hashlib
        self.zero_hash = hashlib.sha256(self.zero_code).hexdigest()
        self.retail_hash = hashlib.sha256(self.retail_code).hexdigest()
        self.diff_hash = hashlib.sha256(self.current_code).hexdigest()

        self.baseline = Baseline(
            unit="test",
            symbols={
                "matched_func": SymbolBaseline(
                    byte_hash=self.zero_hash,
                    mismatch_count=0,
                ),
                "unmatched_func": SymbolBaseline(
                    byte_hash=self.zero_hash,
                    mismatch_count=5,
                ),
                "missing_func": SymbolBaseline(
                    byte_hash=self.zero_hash,
                    mismatch_count=2,
                ),
            },
            text_size=200,
            text_budget=250,
            object_path="/tmp/baseline.o",
        )
        self.repo_root = Path("/fake/repo")
        self.unit_hint = "test/unit"

    def _make_current_fn(self, name: str, code: bytes = b"\x00\x00\x00\x00") -> MagicMock:
        fn = MagicMock(name=name)
        fn.name = name
        fn.code = code
        return fn

    @patch("tools.llm_decomp.verify._resolve_retail_path")
    @patch("tools.llm_decomp.verify.list_text_functions")
    def test_missing_symbol_is_regression(
        self, mock_list_text: MagicMock, mock_retail: MagicMock
    ) -> None:
        from tools.llm_decomp.verify import sweep

        mock_retail.return_value = Path("/fake/retail.o")
        code = self.zero_code
        mock_list_text.side_effect = [
            # retail functions
            [self._make_current_fn("matched_func", code=code), self._make_current_fn("unmatched_func", code=code)],
            # current functions (missing_func is missing)
            [self._make_current_fn("matched_func", code=code), self._make_current_fn("unmatched_func", code=code)],
        ]

        current_obj = MagicMock()
        regressions = sweep(self.repo_root, self.unit_hint, self.baseline, current_obj)

        self.assertEqual(len(regressions), 1)
        self.assertEqual(regressions[0].symbol, "missing_func")
        self.assertEqual(regressions[0].current_mismatches, -1)
        self.assertFalse(regressions[0].was_matched)

    @patch("tools.llm_decomp.verify._resolve_retail_path")
    @patch("tools.llm_decomp.verify.list_text_functions")
    def test_byte_hash_diff(
        self, mock_list_text: MagicMock, mock_retail: MagicMock
    ) -> None:
        from tools.llm_decomp.verify import sweep

        mock_retail.return_value = Path("/fake/retail.o")
        mock_list_text.side_effect = [
            # retail functions (all three baseline symbols)
            [
                self._make_current_fn("matched_func", code=self.retail_code),
                self._make_current_fn("unmatched_func", code=self.zero_code),
                self._make_current_fn("missing_func", code=self.zero_code),
            ],
            # current functions (all three, but matched_func changed)
            [
                self._make_current_fn("matched_func", code=self.current_code),
                self._make_current_fn("unmatched_func", code=self.zero_code),
                self._make_current_fn("missing_func", code=self.zero_code),
            ],
        ]

        current_obj = MagicMock()
        regressions = sweep(self.repo_root, self.unit_hint, self.baseline, current_obj)

        # Only matched_func should have changed (hash diff); unmatched_func and
        # missing_func have matching hashes (all baseline hashes = zero_hash),
        # so they're clean.
        self.assertEqual(len(regressions), 1)
        self.assertEqual(regressions[0].symbol, "matched_func")
        self.assertTrue(regressions[0].was_matched)

    @patch("tools.llm_decomp.verify._resolve_retail_path")
    @patch("tools.llm_decomp.verify.list_text_functions")
    def test_no_change_no_regression(
        self, mock_list_text: MagicMock, mock_retail: MagicMock
    ) -> None:
        from tools.llm_decomp.verify import sweep

        mock_retail.return_value = Path("/fake/retail.o")
        code = b"\x00\x00\x00\x00"
        mock_list_text.side_effect = [
            # retail functions
            [self._make_current_fn("matched_func", code=code)],
            # current functions (identical)
            [self._make_current_fn("matched_func", code=code)],
        ]
        current_obj = MagicMock()
        regressions = sweep(self.repo_root, self.unit_hint, self.baseline, current_obj)

        # matched_func has matching hash → no regression
        # unmatched_func and missing_func are missing from current → 2 regressions
        self.assertEqual(len(regressions), 2)
        for r in regressions:
            self.assertIn(r.symbol, ("unmatched_func", "missing_func"))


class TestVerdictForSubmit(unittest.TestCase):
    """verdict_for_submit produces correct Verdict for each scenario."""

    def setUp(self) -> None:
        self.repo_root = Path("/fake/repo")
        self.unit_hint = "test/unit"
        self.target_symbol = "target_func"
        self.baseline = Baseline(
            unit="test/unit",
            symbols={
                "target_func": SymbolBaseline(
                    byte_hash="aaaa", mismatch_count=5
                ),
                "sibling_a": SymbolBaseline(
                    byte_hash="bbbb", mismatch_count=0
                ),
            },
            text_size=200,
            text_budget=250,
            object_path="/tmp/baseline.o",
        )

    def _make_verdict_helper(
        self,
        build_result: tuple[bool, str] = (True, ""),
        regressions: list[Regression] | None = None,
        added: list[str] | None = None,
        size_check: ObjectSizeCheck | None = None,
        target_in_baseline: bool = True,
        target_mismatches: int = 0,
        target_fuzzy: float = 100.0,
        eq_result: str = "NOT_EQUIVALENT",
    ):
        from tools.llm_decomp.verify import verdict_for_submit

        if regressions is None:
            regressions = []
        if added is None:
            added = []
        if size_check is None:
            size_check = ObjectSizeCheck(
                unit_hint=self.unit_hint,
                split_path="test/unit.cpp",
                budget=250,
                retail_text=200,
                decomp_text=200,
                ok=True,
                notes="ok",
            )

        # Build baseline symbol with appropriate mismatch
        syms = dict(self.baseline.symbols)
        if target_in_baseline:
            # Ensure target exists in baseline with the right mismatch count
            pass  # already in setUp
        else:
            syms.pop(self.target_symbol, None)

        baseline = Baseline(
            unit=self.baseline.unit,
            symbols=syms,
            text_size=self.baseline.text_size,
            text_budget=self.baseline.text_budget,
            object_path=self.baseline.object_path,
        )

        patchers = [
            patch("tools.llm_decomp.verify.build_unit", return_value=build_result),
            patch("tools.llm_decomp.verify._resolve_decomp_path", return_value=Path("/tmp/fake_decomp.o")),
            patch("tools.llm_decomp.verify.sweep", return_value=regressions),
            patch("tools.llm_decomp.verify.added_symbols", return_value=added),
            patch("tools.llm_decomp.verify.check_size", return_value=size_check),
            patch("tools.llm_decomp.verify._resolve_retail_path", return_value=Path("/tmp/retail.o")),
            patch("tools.llm_decomp.verify._count_mismatches", return_value=target_mismatches),
            patch("tools.llm_decomp.verify.estimate_fuzzy", return_value=target_fuzzy),
            patch("tools.llm_decomp.verify.run_equivalence", return_value=eq_result),
        ]

        for p in patchers:
            p.start()
            self.addCleanup(p.stop)

        # Also patch list_text_functions to return valid mock data
        # This is needed by added_symbols and the target status computation
        def make_fn(name: str, code: bytes = b"\x00\x00\x00\x01") -> MagicMock:
            fn = MagicMock(name=name)
            fn.name = name
            fn.code = code
            return fn

        # For list_text_functions, we need to return proper symbols.
        # added_symbols calls it, and sweep calls it via _resolve_retail_path.
        # Build a list of symbol names from the baseline plus any "added" ones.
        current_names = set(syms.keys()) | set(added)
        current_symbols = [make_fn(n) for n in current_names]

        list_text_patcher = patch(
            "tools.llm_decomp.verify.list_text_functions",
            return_value=current_symbols,
        )
        list_text_patcher.start()
        self.addCleanup(list_text_patcher.stop)

        return verdict_for_submit(
            self.repo_root,
            self.unit_hint,
            self.target_symbol,
            baseline,
            equivalence_min_fuzzy=50.0,
        )

    # ── Test cases ──────────────────────────────────────────────────────

    def test_compile_error(self) -> None:
        verdict = self._make_verdict_helper(
            build_result=(False, "some error"),
        )
        self.assertEqual(verdict.kind, VerdictKind.COMPILE_ERROR)
        self.assertFalse(verdict.accepted)
        self.assertEqual(verdict.rule, "compile_error")

    def test_sibling_regression_hard_reject(self) -> None:
        verdict = self._make_verdict_helper(
            regressions=[
                Regression(
                    symbol="sibling_a",
                    baseline_mismatches=0,
                    current_mismatches=3,
                    was_matched=True,
                )
            ],
        )
        self.assertEqual(verdict.kind, VerdictKind.HARD_REJECT)
        self.assertFalse(verdict.accepted)
        self.assertEqual(verdict.rule, "sibling_regression")

    def test_added_symbol_hard_reject(self) -> None:
        verdict = self._make_verdict_helper(
            added=["new_func"],
        )
        self.assertEqual(verdict.kind, VerdictKind.HARD_REJECT)
        self.assertFalse(verdict.accepted)
        self.assertEqual(verdict.rule, "added_symbol")

    def test_sibling_worse_soft_reject(self) -> None:
        verdict = self._make_verdict_helper(
            regressions=[
                Regression(
                    symbol="sibling_a",
                    baseline_mismatches=2,
                    current_mismatches=5,
                    was_matched=False,
                )
            ],
        )
        self.assertEqual(verdict.kind, VerdictKind.SOFT_REJECT)
        self.assertFalse(verdict.accepted)
        self.assertEqual(verdict.rule, "sibling_worse")

    def test_full_match_accepted(self) -> None:
        verdict = self._make_verdict_helper(
            target_mismatches=0,
            target_fuzzy=100.0,
        )
        self.assertEqual(verdict.kind, VerdictKind.ACCEPTED)
        self.assertTrue(verdict.accepted)
        self.assertEqual(verdict.rule, "full_match")

    def test_fuzzy_equivalent_match(self) -> None:
        verdict = self._make_verdict_helper(
            target_mismatches=38,
            target_fuzzy=62.0,
            eq_result="EQUIVALENT",
        )
        self.assertEqual(verdict.kind, VerdictKind.ACCEPTED)
        self.assertTrue(verdict.accepted)
        self.assertEqual(verdict.rule, "equivalent_match")

    def test_fuzzy_not_equivalent(self) -> None:
        verdict = self._make_verdict_helper(
            target_mismatches=38,
            target_fuzzy=62.0,
            eq_result="NOT_EQUIVALENT",
        )
        self.assertEqual(verdict.kind, VerdictKind.SOFT_REJECT)
        self.assertFalse(verdict.accepted)
        self.assertEqual(verdict.rule, "not_matched")

    def test_size_overflow_overrides_full_match(self) -> None:
        verdict = self._make_verdict_helper(
            target_mismatches=0,
            target_fuzzy=100.0,
            size_check=ObjectSizeCheck(
                unit_hint=self.unit_hint,
                split_path="test/unit.cpp",
                budget=200,
                retail_text=180,
                decomp_text=250,
                ok=False,
                notes="decomp .text exceeds budget",
            ),
        )
        self.assertEqual(verdict.kind, VerdictKind.SOFT_REJECT)
        self.assertFalse(verdict.accepted)
        self.assertEqual(verdict.rule, "size_overflow")

    def test_target_missing(self) -> None:
        verdict = self._make_verdict_helper(
            target_in_baseline=False,
        )
        self.assertEqual(verdict.kind, VerdictKind.SOFT_REJECT)
        self.assertFalse(verdict.accepted)
        self.assertEqual(verdict.rule, "target_missing")

    def test_not_matched_default(self) -> None:
        verdict = self._make_verdict_helper(
            target_mismatches=10,
            target_fuzzy=85.0,
            eq_result="NOT_EQUIVALENT",
        )
        self.assertEqual(verdict.kind, VerdictKind.SOFT_REJECT)
        self.assertFalse(verdict.accepted)
        self.assertEqual(verdict.rule, "not_matched")


class TestRunEquivalence(unittest.TestCase):
    """run_equivalence normalizes SMT output correctly."""

    def setUp(self) -> None:
        self.repo_root = Path("/fake/repo")

    @patch("tools.llm_decomp.verify.subprocess.run")
    def test_equivalent(self, mock_run: MagicMock) -> None:
        from tools.llm_decomp.verify import run_equivalence

        result = MagicMock()
        result.returncode = 0
        result.stdout = "EQUIVALENT\n"
        result.stderr = ""
        mock_run.return_value = result

        self.assertEqual(
            run_equivalence(self.repo_root, "test/unit", "target_func"),
            "EQUIVALENT",
        )

    @patch("tools.llm_decomp.verify.subprocess.run")
    def test_not_equivalent(self, mock_run: MagicMock) -> None:
        from tools.llm_decomp.verify import run_equivalence

        result = MagicMock()
        result.returncode = 0
        result.stdout = "NOT_EQUIVALENT\n"
        result.stderr = ""
        mock_run.return_value = result

        self.assertEqual(
            run_equivalence(self.repo_root, "test/unit", "target_func"),
            "NOT_EQUIVALENT",
        )

    @patch("tools.llm_decomp.verify.subprocess.run")
    def test_stdout_no_match_stderr_match(self, mock_run: MagicMock) -> None:
        from tools.llm_decomp.verify import run_equivalence

        result = MagicMock()
        result.returncode = 0
        result.stdout = "some other output\n"
        result.stderr = "Result: EQUIVALENT\n"
        mock_run.return_value = result

        self.assertEqual(
            run_equivalence(self.repo_root, "test/unit", "target_func"),
            "EQUIVALENT",
        )

    @patch("tools.llm_decomp.verify.subprocess.run")
    def test_timeout(self, mock_run: MagicMock) -> None:
        from tools.llm_decomp.verify import run_equivalence

        mock_run.side_effect = subprocess.TimeoutExpired(cmd="test", timeout=300)

        self.assertEqual(
            run_equivalence(self.repo_root, "test/unit", "target_func"),
            "INCONCLUSIVE",
        )


class TestBuildUnit(unittest.TestCase):
    """build_unit calls ninja with the correct path."""

    def setUp(self) -> None:
        self.repo_root = Path("/fake/repo")

    @patch("tools.llm_decomp.verify._resolve_decomp_path")
    @patch("tools.llm_decomp.verify.load_config")
    @patch("tools.llm_decomp.verify.BuildLock")
    @patch("tools.llm_decomp.verify.subprocess.run")
    @patch("tools.llm_decomp.verify.clean_mwcc_diagnostics")
    def test_build_success(
        self,
        mock_clean: MagicMock,
        mock_run: MagicMock,
        mock_lock: MagicMock,
        mock_load_config: MagicMock,
        mock_decomp: MagicMock,
    ) -> None:
        from tools.llm_decomp.verify import build_unit

        mock_clean.return_value = "cleaned"
        mock_decomp.return_value = Path("/fake/repo/build/us/src/test/unit.o")
        mock_run.return_value.returncode = 0
        mock_run.return_value.stdout = "build log"
        mock_run.return_value.stderr = ""

        # Mock Project and unit resolution
        mock_config = MagicMock()
        mock_load_config.return_value = mock_config
        mock_project = MagicMock()
        mock_project.ninja_bin.return_value = "ninja"

        with patch("tools.llm_decomp.verify.Project", return_value=mock_project):
            with patch(
                "tools.llm_decomp.verify._object_paths_for_unit",
                return_value=(MagicMock(), MagicMock()),
            ):
                resolved = MagicMock()
                mock_project.resolve_unit.return_value = resolved

                with patch.object(Path, "is_file", return_value=True):
                    success, diagnostics = build_unit(self.repo_root, "test/unit")

        self.assertTrue(success)
        self.assertEqual(diagnostics, "cleaned")


if __name__ == "__main__":
    unittest.main()