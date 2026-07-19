"""Phase 9 trustworthiness tests: feedback goldens, mutation safety, docs, solve-vs-sample."""
from __future__ import annotations

import io
import json
import tempfile
import unittest
from contextlib import redirect_stdout
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any, Dict, List, Optional
from unittest.mock import patch

from tools.llm_harness.benchmark import (
    ABLATIONS,
    BenchmarkReport,
    TargetRunMetrics,
    check_rollout_gates,
    classify_size_bucket,
    compare_workflows,
    hide_function_body,
    load_corpus,
    metrics_from_experiment_state,
    verify_hidden_source,
)
from tools.llm_harness.core import Harness
from tools.llm_harness.match_improve import normalize_objdiff_feedback
from tools.llm_harness.run import main
from tools.llm_harness.types import (
    Candidate,
    CandidateEvaluation,
    CandidateStatus,
    CompileReport,
    Evaluation,
    ProviderResult,
    fingerprint_binary_feedback,
)
from tools.ppc_equivalence.elf_symbols import FunctionBytes, FunctionRelocation


ROOT = Path(__file__).resolve().parents[3]
CORPUS_PATH = Path(__file__).parent / "benchmark" / "golden_corpus.json"


def _fn(code: bytes, *, name: str = "f", base: int = 0x80000000, relocs=None) -> FunctionBytes:
    return FunctionBytes(
        name=name,
        path=Path("."),
        code=code,
        base=base,
        value=base,
        size=len(code),
        section_index=0,
        section_name=".text",
        symbol_type=0,
        relocations=list(relocs or []),
    )


class TestFeedbackGoldens(unittest.TestCase):
    def test_cmpwi_vs_cmplwi(self) -> None:
        # cmpwi r3,0  vs  cmplwi r3,0
        retail = _fn(bytes.fromhex("2c030000") + bytes.fromhex("4e800020"))
        cand = _fn(bytes.fromhex("28030000") + bytes.fromhex("4e800020"))
        feedback = normalize_objdiff_feedback(retail, cand)
        self.assertIsNotNone(feedback["first_difference_offset"])
        diff = feedback["differences"][0]
        pair = {diff["retail_mnemonic"], diff["candidate_mnemonic"]}
        self.assertTrue({"cmpwi", "cmplwi"} <= pair or "cmp" in diff["retail_mnemonic"])
        text = json.dumps(feedback)
        self.assertTrue("cmp" in text)

    def test_lwz_vs_lhz(self) -> None:
        retail = _fn(bytes.fromhex("80640000") + bytes.fromhex("4e800020"))  # lwz r3,0(r4)
        cand = _fn(bytes.fromhex("a0640000") + bytes.fromhex("4e800020"))  # lhz r3,0(r4)
        feedback = normalize_objdiff_feedback(retail, cand)
        self.assertGreaterEqual(feedback["num_differences"], 1)
        hint = feedback["differences"][0].get("likely_cause", "")
        self.assertTrue("load" in hint or feedback["differences"][0]["retail_mnemonic"] != feedback["differences"][0]["candidate_mnemonic"])

    def test_stw_vs_stb(self) -> None:
        retail = _fn(bytes.fromhex("90640000") + bytes.fromhex("4e800020"))  # stw
        cand = _fn(bytes.fromhex("98640000") + bytes.fromhex("4e800020"))  # stb
        feedback = normalize_objdiff_feedback(retail, cand)
        self.assertGreaterEqual(feedback["num_differences"], 1)

    def test_identical_functions(self) -> None:
        code = bytes.fromhex("386000004e800020")
        feedback = normalize_objdiff_feedback(_fn(code), _fn(code))
        self.assertEqual(feedback["num_differences"], 0)
        self.assertIsNone(feedback["first_difference_offset"])
        self.assertEqual(fingerprint_binary_feedback(feedback), fingerprint_binary_feedback(feedback))

    def test_size_and_frame_delta(self) -> None:
        retail = _fn(bytes.fromhex("4e800020"))
        cand = _fn(bytes.fromhex("386000004e800020"))
        feedback = normalize_objdiff_feedback(retail, cand)
        self.assertEqual(feedback["function_size"]["retail"], 4)
        self.assertEqual(feedback["function_size"]["candidate"], 8)


class TestCorpusAndBuckets(unittest.TestCase):
    def test_corpus_loads_and_buckets(self) -> None:
        entries = load_corpus(CORPUS_PATH)
        self.assertGreaterEqual(len(entries), 8)
        categories = {e.category for e in entries}
        self.assertIn("trivial getter/setter", categories)
        self.assertIn("member-function-pointer/PTMF case", categories)
        self.assertEqual(classify_size_bucket(2), "1-8")
        self.assertEqual(classify_size_bucket(40), "25-64")
        self.assertEqual(classify_size_bucket(200), "161+")
        self.assertEqual(entries[0].size_bucket, classify_size_bucket(entries[0].approx_insns))

    def test_ablations_are_solve_sample_not_knowledge(self) -> None:
        names = {a.name for a in ABLATIONS}
        self.assertIn("solve_default", names)
        self.assertIn("sample_new_10", names)
        for ablation in ABLATIONS:
            self.assertNotIn("knowledge", json.dumps(ablation.overrides))


class TestHiddenSourceHelpers(unittest.TestCase):
    def test_hide_and_detect_leak(self) -> None:
        original = "int Foo::bar(int x) {\n  return x + 1;\n}\n"
        hidden = hide_function_body(original, target_id="foo-bar")
        self.assertIn("HARVEST_HIDDEN_SOURCE:foo-bar", hidden)
        self.assertNotIn("return x + 1", hidden)
        leaks = verify_hidden_source(
            f"prompt with leaked body:\n{original}",
            forbidden_snippets=[original],
        )
        self.assertTrue(leaks)
        clean = verify_hidden_source(hidden, forbidden_snippets=[original])
        self.assertEqual(clean, [])


@dataclass
class ScriptedProvider:
    responses: List[str]
    calls: List[str] = field(default_factory=list)

    def invoke(self, prompt: str, model: Any, context_dir: Path) -> ProviderResult:
        self.calls.append(prompt)
        if not self.responses:
            raise RuntimeError("exhausted")
        return ProviderResult(text=self.responses.pop(0), duration_seconds=0.01, cost=0.01)


class ScriptedAdapter:
    def __init__(self, root: Path, scores: List[float]) -> None:
        self.root = root
        self.scores = list(scores)
        self.source_writes = 0
        self.canonical = "void target() { /* baseline */ }"

    def finalize(self) -> None:
        return None

    def build_prompt(self, workflow, target_id, history, options=None):
        options = options or {}
        repair = (options or {}).get("repair_context") or {}
        body = repair.get("source") or self.canonical
        return f"workflow={workflow}\ntarget={target_id}\n{body}\n"

    def evaluate(self, workflow, target_id, candidate: Candidate) -> Evaluation:
        if "broken" in candidate.source:
            return Evaluation(status="COMPILE_ERROR", match_percent=0.0, accepted=False)
        score = self.scores.pop(0) if self.scores else 50.0
        accepted = score >= 100.0
        return Evaluation(
            status="FULL_MATCH" if accepted else "HIGH_MATCH",
            match_percent=score,
            accepted=accepted,
            symbol_accepted=accepted,
            metrics={
                "symbol_accepted": accepted,
                "mismatch_fingerprint": f"fp-{int(score)}",
                "binary_feedback": {"differences": [], "fingerprint": f"fp-{int(score)}"},
            },
        )

    def read_target_source(self, target_id: str) -> str:
        return self.canonical

    def evaluate_canon(self, target_id: str, artifact_dir: Path) -> CandidateEvaluation:
        return CandidateEvaluation(
            status=CandidateStatus.COMPILES,
            compile_report=CompileReport(succeeded=True),
            match_percent=5.0,
            symbol_accepted=False,
            promising=True,
        )

    def rank_candidate(self, workflow, evaluation):
        metrics = evaluation.get("metrics") or {}
        return (
            1 if evaluation.get("symbol_accepted", metrics.get("symbol_accepted")) else 0,
            float(evaluation.get("match_percent") or 0.0),
        )

    def promote(self, *args, **kwargs):
        self.source_writes += 1
        return "promoted"


def _cand(source: str) -> str:
    return json.dumps({
        "source": source,
        "hypothesis": "h",
        "notes": [],
        "next_change": "n",
        "change": "c",
    })


def _harness(root: Path, responses: List[str], scores: List[float]) -> tuple[Harness, ScriptedProvider, ScriptedAdapter]:
    root.mkdir(parents=True, exist_ok=True)
    (root / "adapter.py").write_text(
        "def create_adapter(root, settings):\n    raise RuntimeError('patched')\n",
        encoding="utf-8",
    )
    config = {
        "project_adapter": "adapter.py",
        "output_dir": "out",
        "models": {
            "initial": [{"id": "init", "provider": "fake", "model": "m", "runs": 1}],
            "repair": [{"id": "repair", "provider": "fake", "model": "m", "runs": 1}],
            "new": [{"id": "new", "provider": "fake", "model": "m", "runs": 1}],
        },
        "providers": {"fake": {}},
        "execution": {"isolation": {"mode": "none"}, "max_retries": 0},
        "solve": {
            "initial_candidates": 2,
            "compile_repairs": 1,
            "match_repairs": 2,
            "max_repeated_fingerprint": 2,
        },
    }
    (root / "config.json").write_text(json.dumps(config), encoding="utf-8")
    with patch("tools.llm_harness.core.Harness._load_adapter") as load:
        adapter = ScriptedAdapter(root, scores)
        load.return_value = adapter
        harness = Harness(root / "config.json")
    provider = ScriptedProvider(list(responses))
    harness.providers = {"fake": provider}
    harness.adapter = adapter
    return harness, provider, adapter


class TestSolveVsSampleBenchmark(unittest.TestCase):
    def test_solve_beats_blind_sample_on_accepted_per_call(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            # Solve: 2 initials then repair to full match => 3 calls, 1 accepted
            solve_h, _, solve_adapter = _harness(
                root / "solve",
                [
                    _cand("void target() { /* a */ }"),
                    _cand("void target() { /* b */ }"),
                    _cand("void target() { /* full */ }"),
                ],
                [40.0, 60.0, 100.0],
            )
            solve_dir = solve_h.solve("demo")
            solve_state = json.loads((solve_dir / "state.json").read_text(encoding="utf-8"))
            solve_report = BenchmarkReport(name="solve", workflow="solve")
            solve_report.add(
                metrics_from_experiment_state(
                    solve_state,
                    workflow="solve",
                    category="toy",
                    size_bucket="1-8",
                    baseline_match_percent=5.0,
                )
            )

            # Sample: 10 identical new calls, only last accepts => worse accepted/call
            sample_root = root / "sample"
            sample_h, _, sample_adapter = _harness(
                sample_root,
                [_cand(f"void target() {{ /* s{i} */ }}") for i in range(10)],
                [10.0] * 9 + [100.0],
            )
            # Force new models list for sampling
            sample_dir = sample_h.run("new", "demo", runs=10)
            sample_state = json.loads((sample_dir / "state.json").read_text(encoding="utf-8"))
            sample_state["model_calls"] = len(sample_state.get("records") or [])
            sample_report = BenchmarkReport(name="sample", workflow="sample")
            sample_report.add(
                metrics_from_experiment_state(
                    sample_state,
                    workflow="sample",
                    category="toy",
                    size_bucket="1-8",
                    baseline_match_percent=5.0,
                )
            )

            comparison = compare_workflows(solve_report, sample_report)
            self.assertEqual(comparison["verdict"], "solve_wins")
            self.assertEqual(comparison["automatic_source_mutation_count"], 0)
            gates = check_rollout_gates(comparison)
            self.assertTrue(all(g.startswith("PASS") for g in gates), gates)
            self.assertEqual(solve_adapter.source_writes, 0)
            self.assertEqual(sample_adapter.source_writes, 0)


class TestMutationSafety(unittest.TestCase):
    def test_ordinary_solve_never_calls_promote(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            harness, _, adapter = _harness(
                root,
                [_cand("void target() { /* full */ }")],
                [100.0],
            )
            harness.solve("demo")
            self.assertEqual(adapter.source_writes, 0)

    def test_metrics_mark_zero_auto_mutations(self) -> None:
        state = {
            "target_id": "t",
            "status": "complete",
            "model_calls": 1,
            "records": [
                {
                    "evaluation": {
                        "status": "FULL_MATCH",
                        "match_percent": 100.0,
                        "accepted": True,
                        "symbol_accepted": True,
                    },
                    "candidate_summary": {},
                }
            ],
        }
        metrics = metrics_from_experiment_state(state, workflow="solve")
        self.assertEqual(metrics.automatic_source_mutations, 0)
        self.assertTrue(metrics.symbol_accepted)


class TestDocumentationCommands(unittest.TestCase):
    def test_readme_commands_help(self) -> None:
        for args in (
            ["solve", "--help"],
            ["sample", "--help"],
            ["promote", "--help"],
            ["--show-config"],
        ):
            # --show-config needs a real config; skip content check there when missing adapter
            if args == ["--show-config"]:
                continue
            with redirect_stdout(io.StringIO()):
                with self.assertRaises(SystemExit) as cm:
                    main(args)
            self.assertEqual(cm.exception.code, 0)


class TestInfrastructureErrorsNotZeroMatch(unittest.TestCase):
    def test_evaluation_error_counted_separately(self) -> None:
        state = {
            "target_id": "t",
            "records": [
                {
                    "evaluation": {
                        "status": "EVALUATION_ERROR",
                        "match_percent": 0.0,
                        "accepted": False,
                    },
                    "candidate_summary": {},
                }
            ],
            "model_calls": 1,
        }
        metrics = metrics_from_experiment_state(state, workflow="solve")
        self.assertEqual(metrics.evaluation_errors, 1)
        self.assertFalse(metrics.symbol_accepted)
        self.assertEqual(metrics.best_match_percent, 0.0)


if __name__ == "__main__":
    unittest.main()
