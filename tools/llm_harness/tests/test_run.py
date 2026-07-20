from __future__ import annotations

import io
import unittest
from contextlib import redirect_stdout
from pathlib import Path
from unittest.mock import Mock, patch

from tools.llm_harness.run import main


class AutomaticNewTargetTests(unittest.TestCase):
    def test_number_selects_targets_and_uses_batch_workflow(self) -> None:
        harness = Mock()
        harness.select_new_targets.return_value = ["one", "two"]
        harness.run_batch.return_value = Path("batch-output")

        with patch("tools.llm_harness.run.Harness", return_value=harness):
            with redirect_stdout(io.StringIO()):
                result = main([
                    "new", "--number", "2", "--ignore-called-functions",
                    "--dry-run", "--runs", "3",
                ])

        self.assertEqual(result, 0)
        harness.select_new_targets.assert_called_once_with(
            2, ignore_called_functions=True, certified_funcs=False, tu=None
        )
        harness.run_batch.assert_called_once_with(
            "new",
            ["one", "two"],
            runs=3,
            dry_run=True,
            model_parallel=None,
            full_context=False,
        )

    def test_explicit_target_still_uses_single_target_workflow(self) -> None:
        harness = Mock()
        harness.run.return_value = Path("experiment-output")

        with patch("tools.llm_harness.run.Harness", return_value=harness):
            with redirect_stdout(io.StringIO()):
                result = main(["new", "one", "--dry-run"])

        self.assertEqual(result, 0)
        harness.select_new_targets.assert_not_called()
        harness.run.assert_called_once()

    def test_improve_number_selects_randomized_batch(self) -> None:
        harness = Mock()
        harness.select_targets.return_value = ["one", "two"]
        harness.run_batch.return_value = Path("batch-output")

        with patch("tools.llm_harness.run.Harness", return_value=harness):
            with redirect_stdout(io.StringIO()):
                result = main(["improve", "--number", "2", "--random", "--dry-run"])

        self.assertEqual(result, 0)
        harness.select_targets.assert_called_once_with("improve", 2, randomize=True, certified_funcs=False, tu=None)
        harness.run_batch.assert_called_once_with(
            "improve", ["one", "two"], runs=None, dry_run=True,
            model_parallel=None, full_context=False,
        )

    def test_solve_number_selects_non_accepted_targets(self) -> None:
        harness = Mock()
        harness.select_targets.return_value = ["one", "two"]
        harness.solve.side_effect = [Path("s1"), Path("s2")]

        with patch("tools.llm_harness.run.Harness", return_value=harness):
            with redirect_stdout(io.StringIO()):
                result = main(["solve", "--number", "2", "--dry-run"])

        self.assertEqual(result, 0)
        harness.select_targets.assert_called_once_with(
            "solve", 2, certified_funcs=False, tu=None
        )
        harness.select_new_targets.assert_not_called()
        self.assertEqual(harness.solve.call_count, 2)
        harness.solve.assert_any_call(
            "one", dry_run=True, resume=None, max_parallel=None
        )
        harness.solve.assert_any_call(
            "two", dry_run=True, resume=None, max_parallel=None
        )

    def test_tu_number_preserves_full_context_option(self) -> None:
        harness = Mock()
        harness.select_targets.return_value = ["unit-a", "unit-b"]
        harness.run_batch.return_value = Path("batch-output")

        with patch("tools.llm_harness.run.Harness", return_value=harness):
            with redirect_stdout(io.StringIO()):
                result = main([
                    "tu-complete", "--number", "2", "--random",
                    "--full-context", "--dry-run",
                ])

        self.assertEqual(result, 0)
        harness.select_targets.assert_called_once_with(
            "tu-complete", 2, randomize=True, certified_funcs=False, tu=None
        )
        harness.run_batch.assert_called_once_with(
            "tu-complete", ["unit-a", "unit-b"], runs=None, dry_run=True,
            model_parallel=None, full_context=True,
        )


if __name__ == "__main__":
    unittest.main()
