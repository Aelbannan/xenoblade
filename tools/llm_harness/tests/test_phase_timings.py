"""Adapter / evaluate_unit_match phase timing hooks (ninja, objdiff, smt)."""

from __future__ import annotations

import unittest
from contextlib import contextmanager
from typing import Iterator, List
from unittest.mock import MagicMock, patch

from tools.coop.lib.objdiff_report import FunctionMatch, MatchEvaluation, UnitReport, evaluate_unit_match
from tools.llm_harness.metrics import TimingRecorder
from tools.llm_harness.xenoblade_project import XenobladeAdapter
from tools.ppc_equivalence.result import ProofStatus


class _PhaseSpy:
    def __init__(self) -> None:
        self.phases: List[str] = []

    @contextmanager
    def measure(self, phase: str) -> Iterator[None]:
        self.phases.append(phase)
        yield


class EvaluateUnitMatchPhaseTimerTests(unittest.TestCase):
    def test_phase_timer_records_objdiff_and_smt(self) -> None:
        spy = _PhaseSpy()
        unit_report = UnitReport(
            unit_name="demo",
            code_match_percent=80.0,
            data_match_percent=100.0,
            fuzzy_match_percent=80.0,
            total_functions=1,
            matched_functions=0,
            functions=[],
        )
        fn = FunctionMatch(
            name="fn",
            demangled_name=None,
            match_percent=80.0,
            size=16,
        )
        probe = MagicMock()
        probe.status = ProofStatus.EQUIVALENT
        probe.detail = "ok"
        probe.certificate = None
        probe.proof = None

        with patch(
            "tools.coop.lib.objdiff_report.report_unit", return_value=unit_report
        ), patch(
            "tools.coop.lib.objdiff_report.find_function_match", return_value=fn
        ), patch(
            "tools.coop.lib.objdiff_report.should_probe_equivalence", return_value=True
        ), patch(
            "tools.coop.lib.objdiff_report.prove_unit_symbol", return_value=probe
        ), patch(
            "tools.coop.lib.objdiff_report.classify_status", return_value="EQUIVALENT_MATCH"
        ), patch(
            "tools.coop.lib.objdiff_report.classify_for_promotion_legacy",
            return_value=MagicMock(confidence_tier="high"),
        ):
            result = evaluate_unit_match(
                MagicMock(),
                MagicMock(),
                "fn",
                target_id="us-demo",
                phase_timer=spy.measure,
            )

        self.assertIsInstance(result, MatchEvaluation)
        self.assertEqual(spy.phases, ["objdiff", "smt"])

    def test_phase_timer_objdiff_only_when_no_smt(self) -> None:
        spy = _PhaseSpy()
        unit_report = UnitReport(
            unit_name="demo",
            code_match_percent=40.0,
            data_match_percent=100.0,
            fuzzy_match_percent=40.0,
            total_functions=1,
            matched_functions=0,
            functions=[],
        )
        fn = FunctionMatch(
            name="fn",
            demangled_name=None,
            match_percent=40.0,
            size=16,
        )
        with patch(
            "tools.coop.lib.objdiff_report.report_unit", return_value=unit_report
        ), patch(
            "tools.coop.lib.objdiff_report.find_function_match", return_value=fn
        ), patch(
            "tools.coop.lib.objdiff_report.should_probe_equivalence", return_value=False
        ), patch(
            "tools.coop.lib.objdiff_report.classify_status", return_value="LOW_MATCH"
        ):
            evaluate_unit_match(
                MagicMock(),
                MagicMock(),
                "fn",
                run_equivalence=True,
                phase_timer=spy.measure,
            )
        self.assertEqual(spy.phases, ["objdiff"])


class AdapterNinjaPhaseTests(unittest.TestCase):
    def test_build_object_records_ninja(self) -> None:
        adapter = XenobladeAdapter.__new__(XenobladeAdapter)
        adapter.root = MagicMock()
        adapter.timings = TimingRecorder()
        adapter.project = MagicMock()
        adapter.project.ninja_bin.return_value = "ninja"
        object_path = MagicMock()
        object_path.relative_to.return_value = "build/us/src/demo.o"

        completed = MagicMock(returncode=0, stdout="", stderr="")
        with patch("tools.llm_harness.xenoblade_project.subprocess.run", return_value=completed):
            err = adapter._build_object(object_path)

        self.assertEqual(err, "")
        summary = adapter.timings.summary()
        self.assertEqual(summary["counts"]["ninja"], 1)
        self.assertGreaterEqual(summary["totals_seconds"]["ninja"], 0.0)

    def test_phase_noop_without_timings(self) -> None:
        adapter = XenobladeAdapter.__new__(XenobladeAdapter)
        adapter.timings = None
        with adapter._phase("ninja"):
            pass


if __name__ == "__main__":
    unittest.main()
