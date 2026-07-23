from __future__ import annotations

import io
import tempfile
import unittest
from contextlib import redirect_stdout
from pathlib import Path
from types import SimpleNamespace
from typing import Optional
from unittest.mock import Mock, patch

from tools.coop.lib.targets import Target
from tools.llm_harness.run import main
from tools.llm_harness.source_regions import begin_marker, end_marker
from tools.llm_harness.xenoblade_project import XenobladeAdapter


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
            2, ignore_called_functions=True, certified_funcs=False,
            high_match_callees=False, tu=None
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
            high_match_callees=False,
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
            "solve",
            2,
            randomize=False,
            certified_funcs=False,
            high_match_callees=False,
            tu=None,
            selection="ready",
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
            local=False,
        )
        harness.solve.assert_not_called()

    def test_solve_number_selects_randomized_batch(self) -> None:
        harness = Mock()
        harness.select_targets.return_value = ["two", "one"]
        harness.run_batch.return_value = Path("batch-output")

        with patch("tools.llm_harness.run.Harness", return_value=harness):
            with redirect_stdout(io.StringIO()):
                result = main(["solve", "--number", "2", "--random", "--dry-run"])

        self.assertEqual(result, 0)
        harness.select_targets.assert_called_once_with(
            "solve",
            2,
            randomize=True,
            certified_funcs=False,
            high_match_callees=False,
            tu=None,
            selection="ready",
        )
        harness.run_batch.assert_called_once_with(
            "solve",
            ["two", "one"],
            dry_run=True,
            model_parallel=None,
            local=False,
        )

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
            "solve",
            1,
            randomize=False,
            certified_funcs=False,
            high_match_callees=False,
            tu=None,
            selection="leaf",
        )
        self.assertIn("selected frontier selection=leaf count=1", out.getvalue())
        harness.run_batch.assert_called_once()

    def test_solve_high_match_callees_flag(self) -> None:
        harness = Mock()
        harness.select_targets.return_value = ["callee"]
        harness.run_batch.return_value = Path("batch-output")
        harness.adapter.describe_frontier.return_value = [
            {"id": "callee", "kind": "leaf"},
        ]

        with patch("tools.llm_harness.run.Harness", return_value=harness):
            with redirect_stdout(io.StringIO()):
                result = main([
                    "solve", "--number", "1", "--high-match-callees", "--dry-run",
                ])

        self.assertEqual(result, 0)
        harness.select_targets.assert_called_once_with(
            "solve",
            1,
            randomize=False,
            certified_funcs=False,
            high_match_callees=True,
            tu=None,
            selection="ready",
        )

    def test_new_high_match_callees_flag(self) -> None:
        harness = Mock()
        harness.select_new_targets.return_value = ["callee"]
        harness.run_batch.return_value = Path("batch-output")

        with patch("tools.llm_harness.run.Harness", return_value=harness):
            with redirect_stdout(io.StringIO()):
                result = main([
                    "new", "--number", "1", "--high-match-callees",
                    "--ignore-called-functions", "--dry-run",
                ])

        self.assertEqual(result, 0)
        harness.select_new_targets.assert_called_once_with(
            1,
            ignore_called_functions=True,
            certified_funcs=False,
            high_match_callees=True,
            tu=None,
        )

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
            high_match_callees=False,
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
            "tu-complete", 2, randomize=True, certified_funcs=False,
            high_match_callees=False, tu=None
        )
        harness.run_batch.assert_called_once_with(
            "tu-complete", ["unit-a", "unit-b"], runs=None, dry_run=True,
            model_parallel=None, full_context=True,
        )

    def test_strip_redundant_externs_dry_run(self) -> None:
        harness = Mock()
        harness.adapter.strip_accepted_redundant_externs.return_value = [
            {
                "target_id": "us-80373470",
                "source": "libs/RVL_SDK/src/revolution/wpad/WPAD.c",
                "status": "FULL_MATCH",
                "action": "would_strip",
                "removed_externs": ["extern void (*_wpadUsedCallback)(void);"],
            }
        ]

        with patch("tools.llm_harness.run.Harness", return_value=harness):
            with redirect_stdout(io.StringIO()) as out:
                result = main([
                    "strip-redundant-externs", "--tu", "WPAD", "--dry-run",
                ])

        self.assertEqual(result, 0)
        harness.adapter.strip_accepted_redundant_externs.assert_called_once_with(
            dry_run=True, tu="WPAD", target_id=""
        )
        payload = out.getvalue()
        self.assertIn('"dry_run": true', payload)
        self.assertIn('"changed": 1', payload)
        self.assertIn("would_strip", payload)

    def test_strip_redundant_externs_rejects_bad_target(self) -> None:
        harness = Mock()
        harness.adapter.strip_accepted_redundant_externs.side_effect = ValueError(
            "Target 'nope' is not an accepted FULL/EQUIVALENT_MATCH "
            "buildable function with source"
        )

        with patch("tools.llm_harness.run.Harness", return_value=harness):
            with self.assertRaises(SystemExit):
                main(["strip-redundant-externs", "nope", "--dry-run"])


class HighMatchCalleesFilterTests(unittest.TestCase):
    def _target(
        self,
        tid: str,
        status: str,
        *,
        called: Optional[list[str]] = None,
    ) -> Target:
        return Target(
            id=tid,
            tier="P1",
            milestone="test",
            function=tid,
            symbol=tid,
            address=None,
            source=None,
            unit=None,
            required_level="EQUIVALENT_MATCH",
            status=status,
            extra={"called_functions": list(called or [])},
        )

    def test_collects_callees_of_high_match_and_above_excluding_full(self) -> None:
        targets = [
            self._target("caller-high", "HIGH_MATCH", called=["a", "b"]),
            self._target("caller-code", "CODE_MATCH", called=["b", "c"]),
            self._target("caller-equiv", "EQUIVALENT_MATCH", called=["d"]),
            self._target("caller-full", "FULL_MATCH", called=["e"]),
            self._target("caller-struct", "STRUCTURAL", called=["f"]),
            self._target("a", "NOT_STARTED"),
            self._target("b", "NOT_STARTED"),
            self._target("c", "NOT_STARTED"),
            self._target("d", "NOT_STARTED"),
            self._target("e", "NOT_STARTED"),
            self._target("f", "NOT_STARTED"),
        ]
        allowed = XenobladeAdapter._high_match_caller_callee_ids(targets)
        self.assertEqual(allowed, {"a", "b", "c", "d"})

    def test_filter_keeps_only_callees_of_near_matched_callers(self) -> None:
        all_targets = [
            self._target("caller", "HIGH_MATCH", called=["keep"]),
            self._target("full", "FULL_MATCH", called=["drop-full"]),
            self._target("keep", "NOT_STARTED"),
            self._target("drop-full", "NOT_STARTED"),
            self._target("unrelated", "NOT_STARTED"),
        ]
        candidates = [
            all_targets[2],
            all_targets[3],
            all_targets[4],
        ]
        filtered = XenobladeAdapter._filter_high_match_callees(all_targets, candidates)
        self.assertEqual([t.id for t in filtered], ["keep"])


class StripAcceptedRedundantExternAdapterTests(unittest.TestCase):
    def test_dry_run_then_write_keeps_earliest_extern(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            src = root / "WPAD.c"
            original = (
                begin_marker("us-80373460")
                + "\n"
                + "extern void *_wpadUsedCallback;\n"
                + "void* WPADIsUsedCallbackByKPAD() { return _wpadUsedCallback; }\n"
                + end_marker("us-80373460")
                + "\n"
                + begin_marker("us-80373470")
                + "\n"
                + "extern void (*_wpadUsedCallback)(void);\n"
                + "void WPADSetCallbackByKPAD(void (*callback)(void)) {\n"
                + "    _wpadUsedCallback = callback;\n"
                + "}\n"
                + end_marker("us-80373470")
                + "\n"
            )
            src.write_text(original, encoding="utf-8")

            def make_target(tid: str, fn: str, status: str) -> SimpleNamespace:
                return SimpleNamespace(
                    id=tid,
                    function=fn,
                    status=status,
                    buildable=True,
                    source=src,
                    symbol=fn,
                    unit="WPAD",
                )

            targets = [
                make_target("us-80373460", "WPADIsUsedCallbackByKPAD", "FULL_MATCH"),
                make_target("us-80373470", "WPADSetCallbackByKPAD", "EQUIVALENT_MATCH"),
                make_target("ignored", "Ignored", "HIGH_MATCH"),
            ]
            adapter = object.__new__(XenobladeAdapter)
            adapter.root = root
            adapter.config = object()

            with patch(
                "tools.llm_harness.xenoblade_project.load_targets",
                return_value=targets,
            ):
                dry = adapter.strip_accepted_redundant_externs(dry_run=True)
                self.assertEqual(src.read_text(encoding="utf-8"), original)
                self.assertEqual(
                    {row["target_id"]: row["action"] for row in dry},
                    {
                        "us-80373460": "unchanged",
                        "us-80373470": "would_strip",
                    },
                )

                written = adapter.strip_accepted_redundant_externs(dry_run=False)
                self.assertEqual(
                    {row["target_id"]: row["action"] for row in written},
                    {
                        "us-80373460": "unchanged",
                        "us-80373470": "stripped",
                    },
                )
                updated = src.read_text(encoding="utf-8")
                self.assertIn("extern void *_wpadUsedCallback;", updated)
                self.assertNotIn("extern void (*_wpadUsedCallback)", updated)
                self.assertIn("_wpadUsedCallback = callback", updated)


if __name__ == "__main__":
    unittest.main()
