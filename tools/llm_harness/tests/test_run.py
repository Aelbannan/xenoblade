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
        harness.select_targets.assert_called_once_with(
            "improve",
            2,
            randomize=True,
            certified_funcs=False,
            tu=None,
            selection="pending",
        )
        harness.run_batch.assert_called_once_with(
            "improve", ["one", "two"], runs=None, dry_run=True,
            model_parallel=None, full_context=False,
        )

    def test_solve_number_defaults_to_ready_selection(self) -> None:
        harness = Mock()
        harness.select_targets.return_value = ["one", "two"]
        harness.run_batch.return_value = Path("batch-output")
        harness.adapter.describe_frontier.return_value = [
            {"id": "one", "kind": "leaf"},
            {"id": "two", "kind": "callees-accepted"},
        ]

        with patch("tools.llm_harness.run.Harness", return_value=harness):
            with redirect_stdout(io.StringIO()) as out:
                result = main(["solve", "--number", "2", "--dry-run"])

        self.assertEqual(result, 0)
        harness.select_targets.assert_called_once_with(
            "solve", 2, certified_funcs=False, tu=None, selection="ready"
        )
        harness.select_new_targets.assert_not_called()
        printed = out.getvalue()
        self.assertIn("selected frontier selection=ready count=2", printed)
        self.assertIn("one (leaf)", printed)
        self.assertIn("two (callees-accepted)", printed)
        harness.run_batch.assert_called_once_with(
            "solve",
            ["one", "two"],
            dry_run=True,
            model_parallel=None,
        )
        harness.solve.assert_not_called()

    def test_solve_selection_leaf(self) -> None:
        harness = Mock()
        harness.select_targets.return_value = ["leaf-one"]
        harness.run_batch.return_value = Path("batch-output")
        harness.adapter.describe_frontier.return_value = [
            {"id": "leaf-one", "kind": "leaf"},
        ]

        with patch("tools.llm_harness.run.Harness", return_value=harness):
            with redirect_stdout(io.StringIO()) as out:
                result = main(["solve", "--number", "1", "--selection", "leaf", "--dry-run"])

        self.assertEqual(result, 0)
        harness.select_targets.assert_called_once_with(
            "solve", 1, certified_funcs=False, tu=None, selection="leaf"
        )
        self.assertIn("selected frontier selection=leaf count=1", out.getvalue())
        harness.run_batch.assert_called_once()

    def test_batch_solve_uses_run_batch(self) -> None:
        harness = Mock()
        harness.run_batch.return_value = Path("batch-output")

        with patch("tools.llm_harness.run.Harness", return_value=harness):
            with redirect_stdout(io.StringIO()):
                result = main(["batch", "solve", "t1", "t2", "--dry-run"])

        self.assertEqual(result, 0)
        harness.run_batch.assert_called_once_with(
            "solve",
            ["t1", "t2"],
            runs=None,
            dry_run=True,
            max_target_parallel=None,
            model_parallel=None,
            full_context=False,
        )
        harness.solve.assert_not_called()

    def test_improve_selection_ready(self) -> None:
        harness = Mock()
        harness.select_targets.return_value = ["one"]
        harness.run_batch.return_value = Path("batch-output")

        with patch("tools.llm_harness.run.Harness", return_value=harness):
            with redirect_stdout(io.StringIO()):
                result = main([
                    "improve", "--number", "1", "--selection", "ready", "--dry-run",
                ])

        self.assertEqual(result, 0)
        harness.select_targets.assert_called_once_with(
            "improve",
            1,
            randomize=False,
            certified_funcs=False,
            tu=None,
            selection="ready",
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
