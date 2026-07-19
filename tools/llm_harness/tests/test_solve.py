"""Integration tests for solve loop, lineage, acceptance, and KB absence."""
from __future__ import annotations

import io
import json
import tempfile
import unittest
from contextlib import redirect_stdout
from dataclasses import asdict, dataclass, field
from pathlib import Path
from typing import Any, Dict, List, Optional
from unittest.mock import Mock, patch

from tools.llm_harness.core import Harness, parse_candidate
from tools.llm_harness.promotion import evaluation_to_candidate
from tools.llm_harness.run import main
from tools.llm_harness.types import (
    Candidate,
    CandidateEvaluation,
    CandidateStatus,
    CompileReport,
    Evaluation,
    ProviderResult,
)


@dataclass
class FakeProvider:
    responses: List[str]
    calls: List[str] = field(default_factory=list)

    def invoke(self, prompt: str, model: Any, context_dir: Path) -> ProviderResult:
        self.calls.append(prompt)
        if not self.responses:
            raise RuntimeError("no scripted responses left")
        text = self.responses.pop(0)
        return ProviderResult(text=text, duration_seconds=0.01)


class FakeAdapter:
    def __init__(self, root: Path) -> None:
        self.root = root
        self.evaluate_calls = 0
        self.baseline_accepted = False
        self._scores = [40.0, 55.0, 70.0, 100.0]
        self.prompts: List[Dict[str, Any]] = []

    def finalize(self) -> None:
        return None

    def build_prompt(
        self,
        workflow: str,
        target_id: str,
        history: List[Dict[str, Any]],
        options: Optional[Dict[str, Any]] = None,
    ) -> str:
        options = options or {}
        self.prompts.append({"workflow": workflow, "options": options})
        repair = options.get("repair_context") or {}
        body = repair.get("source") or "void target() {}"
        feedback = repair.get("binary_feedback")
        return (
            f"workflow={workflow}\n"
            f"target={target_id}\n"
            f"CURRENT:\n{body}\n"
            f"FEEDBACK:{json.dumps(feedback)}\n"
            f"FROZEN_KB must never appear\n"
        )

    def evaluate(self, workflow: str, target_id: str, candidate: Candidate) -> Evaluation:
        self.evaluate_calls += 1
        if "broken" in candidate.source:
            return Evaluation(
                status="COMPILE_ERROR",
                match_percent=0.0,
                accepted=False,
                detail="error: undeclared identifier",
                symbol_accepted=False,
                promising=False,
            )
        score = self._scores.pop(0) if self._scores else 80.0
        symbol_accepted = score >= 100.0
        status = "FULL_MATCH" if symbol_accepted else "HIGH_MATCH"
        return Evaluation(
            status=status,
            match_percent=score,
            accepted=symbol_accepted,
            symbol_accepted=symbol_accepted,
            promising=not symbol_accepted and score > 0,
            metrics={
                "symbol_accepted": symbol_accepted,
                "mismatch_fingerprint": f"fp-{int(score)}",
                "binary_feedback": {
                    "first_difference_offset": 20,
                    "differences": [
                        {
                            "offset": 20,
                            "retail": "cmplwi",
                            "candidate": "cmpwi",
                            "retail_raw": "0x1",
                            "candidate_raw": "0x2",
                        }
                    ],
                    "instruction_windows": [],
                    "relocation_differences": {},
                    "function_size": {"retail": 64, "candidate": 68},
                    "stack_frame": {"retail": 16, "candidate": 16},
                },
                "function_size_delta": 4,
            },
        )

    def read_target_source(self, target_id: str) -> str:
        return "void target() { /* baseline */ }"

    def evaluate_canon(self, target_id: str, artifact_dir: Path) -> CandidateEvaluation:
        if self.baseline_accepted:
            return CandidateEvaluation(
                status=CandidateStatus.FULL_MATCH,
                compile_report=CompileReport(succeeded=True),
                match_percent=100.0,
                symbol_accepted=True,
            )
        return CandidateEvaluation(
            status=CandidateStatus.COMPILES,
            compile_report=CompileReport(succeeded=True),
            match_percent=10.0,
            symbol_accepted=False,
            promising=True,
        )

    def rank_candidate(self, workflow: str, evaluation: Dict[str, Any]) -> tuple:
        metrics = evaluation.get("metrics") or {}
        return (
            1 if evaluation.get("symbol_accepted", metrics.get("symbol_accepted")) else 0,
            float(evaluation.get("match_percent") or 0.0),
        )


def _write_harness(root: Path, responses: List[str]) -> tuple[Harness, FakeProvider, FakeAdapter]:
    adapter_path = root / "adapter.py"
    adapter_path.write_text(
        "def create_adapter(root, settings):\n"
        "    raise RuntimeError('patched')\n",
        encoding="utf-8",
    )
    config = {
        "project_adapter": "adapter.py",
        "output_dir": "out",
        "models": {
            "initial": [{"id": "init", "provider": "fake", "model": "init-model", "runs": 1}],
            "repair": [{"id": "repair", "provider": "fake", "model": "repair-model", "runs": 1}],
        },
        "providers": {"fake": {}},
        "execution": {"isolation": {"mode": "none"}, "max_retries": 0, "auto_promote": False},
        "solve": {
            "initial_candidates": 2,
            "compile_repairs": 1,
            "match_repairs": 3,
            "max_repeated_fingerprint": 2,
            "stop_on_full_match": True,
            "stop_on_equivalent_match": True,
        },
    }
    (root / "config.json").write_text(json.dumps(config), encoding="utf-8")
    with patch("tools.llm_harness.core.Harness._load_adapter") as load:
        adapter = FakeAdapter(root)
        load.return_value = adapter
        harness = Harness(root / "config.json")
    provider = FakeProvider(responses=list(responses))
    harness.providers = {"fake": provider}
    harness.adapter = adapter
    return harness, provider, adapter


def _cand(source: str, hypothesis: str = "h") -> str:
    return json.dumps({
        "source": source,
        "hypothesis": hypothesis,
        "notes": [],
        "next_change": "n",
        "change": "c",
    })


class TestSolveLoop(unittest.TestCase):
    def test_solve_full_match_stops(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            harness, provider, adapter = _write_harness(
                root,
                [
                    _cand("void target() { /* a */ }"),
                    _cand("void target() { /* full */ }"),
                ],
            )
            adapter._scores = [40.0, 100.0]
            experiment = harness.solve("demo-target")
            state = json.loads((experiment / "state.json").read_text(encoding="utf-8"))
            self.assertEqual(state["status"], "complete")
            self.assertEqual(state["workflow"], "solve")
            self.assertGreaterEqual(len(state["branches"]), 1)
            self.assertTrue((experiment / "best.json").is_file())
            # Ordinary solve must not mutate adapter root source files.
            self.assertFalse(any(root.rglob("*.cpp")))
            self.assertLessEqual(len(provider.calls), 2)

    def test_solve_compile_then_match_repair(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            harness, provider, adapter = _write_harness(
                root,
                [
                    _cand("void target() { broken; }"),
                    _cand("void target() { /* repaired compile */ }"),
                    _cand("void target() { /* matched */ }"),
                ],
            )
            adapter._scores = [55.0, 100.0]
            experiment = harness.solve("demo-target")
            state = json.loads((experiment / "state.json").read_text(encoding="utf-8"))
            phases = [b.get("phase") for b in state["branches"]]
            self.assertIn("initial", phases)
            self.assertTrue(
                any(p in {"compile_repair", "match_repair"} for p in phases),
                phases,
            )
            self.assertTrue(any(b.get("parent_id") for b in state["branches"]))

    def test_baseline_already_accepted_skips_models(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            harness, provider, adapter = _write_harness(root, [_cand("void target() {}")])
            adapter.baseline_accepted = True
            experiment = harness.solve("demo-target")
            state = json.loads((experiment / "state.json").read_text(encoding="utf-8"))
            self.assertEqual(state["reason"], "baseline_already_accepted")
            self.assertEqual(state["model_calls"], 0)
            self.assertEqual(provider.calls, [])

    def test_improve_prompt_receives_repair_context(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            harness, provider, adapter = _write_harness(
                root,
                [
                    _cand("void target() { /* parent */ }"),
                    _cand("void target() { /* child */ }"),
                    _cand("void target() { /* done */ }"),
                ],
            )
            adapter._scores = [40.0, 60.0, 100.0]
            harness.solve("demo-target")
            repair_prompts = [
                p for p in adapter.prompts if (p["options"] or {}).get("repair_context")
            ]
            self.assertTrue(repair_prompts)
            ctx = repair_prompts[0]["options"]["repair_context"]
            # Best initial candidate was the 60% "child" body, not the weaker parent.
            self.assertIn("child", ctx["source"])
            self.assertIsNotNone(ctx.get("binary_feedback"))
            self.assertTrue(any("FEEDBACK:" in call and "cmplwi" in call for call in provider.calls))

    def test_prompts_never_contain_frozen_kb_marker_as_content(self) -> None:
        # The fake prompt mentions FROZEN_KB only to assert absence of corpus injection.
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            harness, provider, adapter = _write_harness(
                root, [_cand("void target() { /* full */ }")]
            )
            adapter._scores = [100.0]
            harness.solve("demo-target")
            joined = "\n".join(provider.calls)
            self.assertNotIn("MWCC_REFERENCE", joined)
            self.assertNotIn("{{FROZEN_KB}}", joined)


class TestAcceptanceSemantics(unittest.TestCase):
    def test_code_match_not_symbol_accepted(self) -> None:
        ev = evaluation_to_candidate({
            "status": "CODE_MATCH",
            "match_percent": 95.0,
            "accepted": False,
            "equivalence": "INCONCLUSIVE_UNVALIDATED_CALLEE",
            "metrics": {},
        })
        self.assertFalse(ev.symbol_accepted)
        self.assertEqual(ev.blocked_reason, "unvalidated_callee")

    def test_full_match_symbol_accepted(self) -> None:
        ev = evaluation_to_candidate({
            "status": "FULL_MATCH",
            "match_percent": 100.0,
            "accepted": True,
            "symbol_accepted": True,
        })
        self.assertTrue(ev.symbol_accepted)


class TestSolveCli(unittest.TestCase):
    def test_solve_help(self) -> None:
        with redirect_stdout(io.StringIO()) as out:
            with self.assertRaises(SystemExit) as cm:
                main(["solve", "--help"])
        self.assertEqual(cm.exception.code, 0)
        self.assertIn("solve", out.getvalue())

    def test_sample_command_registered(self) -> None:
        with redirect_stdout(io.StringIO()) as out:
            with self.assertRaises(SystemExit) as cm:
                main(["sample", "--help"])
        self.assertEqual(cm.exception.code, 0)
        self.assertIn("sample", out.getvalue())


class TestParseCandidateChange(unittest.TestCase):
    def test_change_field_optional(self) -> None:
        candidate = parse_candidate(
            json.dumps({
                "source": "void f() {}",
                "hypothesis": "h",
                "notes": [],
                "next_change": "n",
            }),
            workflow="new",
        )
        self.assertEqual(candidate.source, "void f() {}")


if __name__ == "__main__":
    unittest.main()
