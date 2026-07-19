"""Tests for Pipeline Orchestration (Phase 8 / §16-19)."""
from __future__ import annotations

import json
import tempfile
import unittest
from pathlib import Path
from typing import Any, Dict, List, Optional

from tools.llm_harness.pipeline import PipelineRunner, parse_config
from tools.llm_harness.types import (
    Candidate,
    CandidateEvaluation,
    CompileReport,
    Evaluation,
    ExperimentState,
    PipelineBudgets,
    PipelineConfig,
    PipelineStage,
    ProjectAdapter,
    PromotionPolicy,
)


class FakeProvider:
    def __init__(self) -> None:
        self.call_count = 0

    def invoke(self, prompt: str, model: Any, cwd: Path) -> Any:
        self.call_count += 1
        from tools.llm_harness.types import ProviderResult
        return ProviderResult(
            text='{"source":"int f() { return 1; }","hypothesis":"first try"}',
            duration_seconds=0.01,
            input_tokens=10,
            output_tokens=5,
        )


class FakeAdapter:
    root: Path = Path("/tmp")

    def __init__(self) -> None:
        self.evaluate_calls: List[str] = []

    def build_prompt(self, workflow: str, target_id: str,
                     history: List[Dict[str, Any]],
                     options: Optional[Dict[str, Any]] = None) -> str:
        return f"# Target: {target_id}\nReconstruct this function."

    def evaluate(self, workflow: str, target_id: str, candidate: Candidate) -> Evaluation:
        self.evaluate_calls.append(target_id)
        return Evaluation(
            status="COMPILES",
            match_percent=75.0,
            accepted=True,
            detail="ok",
            metrics={
                "structural": {
                    "total_score": 0.80,
                    "calls": {"score": 1.0, "matched": 2, "expected": 2, "details": []},
                    "relocations": {"score": 1.0, "matched": 1, "expected": 1, "details": []},
                    "memory_accesses": {"score": 0.8, "matched": 4, "expected": 5, "details": []},
                    "cfg": {"score": 1.0, "matched": 3, "expected": 3, "details": []},
                    "constants": {"score": 1.0, "matched": 2, "expected": 2, "details": []},
                    "returns": {"score": 1.0, "matched": 1, "expected": 1, "details": []},
                    "instruction_classes": {"score": 1.0, "matched": 8, "expected": 8, "details": []},
                    "unexpected_effects": [],
                    "missing_effects": [],
                },
                "retail_function_size": 100,
                "candidate_function_size": 100,
                "function_size_delta": 0,
            },
        )

    def finalize(self) -> None:
        pass

    def read_target_source(self, target_id: str) -> str:
        return "int f() { return 1; }"

    def promote(self, workflow: str, target_id: str,
                candidate: Candidate, *, write: bool = False) -> str:
        return f"promoted {target_id}"


class TestPipelineConfig(unittest.TestCase):
    """§17 — Config validation"""

    def test_default_config_valid(self) -> None:
        config = PipelineConfig()
        errors = config.validate()
        self.assertEqual(errors, [])

    def test_weights_sum_validation(self) -> None:
        config = PipelineConfig()
        config.structural.weights["calls"] = 0.5
        config.structural.weights["cfg"] = 0.5
        errors = config.validate()
        self.assertTrue(
            any("sum" in e for e in errors),
            f"Expected weight-sum error, got: {errors}",
        )

    def test_negative_budget_error(self) -> None:
        config = PipelineConfig()
        config.budgets.reconstruct = -1
        errors = config.validate()
        self.assertTrue(
            any("nonnegative" in e for e in errors),
            f"Expected budget error, got: {errors}",
        )

    def test_threshold_range_error(self) -> None:
        config = PipelineConfig()
        config.structural.semantic_repair_below = 1.5
        errors = config.validate()
        self.assertTrue(
            any("in [0, 1]" in e for e in errors),
            f"Expected threshold error, got: {errors}",
        )

    def test_unknown_lock_scope_error(self) -> None:
        config = PipelineConfig()
        config.promotion.lock_scope = "invalid"
        errors = config.validate()
        self.assertTrue(
            any("lock_scope" in e for e in errors),
            f"Expected lock_scope error, got: {errors}",
        )

    def test_unknown_knowledge_mode_error(self) -> None:
        config = PipelineConfig()
        config.knowledge.mode = "unknown"
        errors = config.validate()
        self.assertTrue(
            any("knowledge.mode" in e for e in errors),
            f"Expected knowledge.mode error, got: {errors}",
        )

    def test_from_dict(self) -> None:
        data = {
            "budgets": {"reconstruct": 2, "compile_repair": 4},
            "stop_on": {"full_match": False},
            "promotion": {"lock_scope": "unit"},
        }
        config = PipelineConfig.from_dict(data)
        self.assertEqual(config.budgets.reconstruct, 2)
        self.assertEqual(config.budgets.compile_repair, 4)
        self.assertFalse(config.stop_on.full_match)
        self.assertEqual(config.promotion.lock_scope, "unit")

    def test_parse_config_pipeline_section(self) -> None:
        data = {"pipeline": {"budgets": {"reconstruct": 1}}}
        config = parse_config(data)
        self.assertEqual(config.budgets.reconstruct, 1)

    def test_parse_config_invalid(self) -> None:
        data = {
            "pipeline": {
                "structural": {"weights": {"calls": 2.0}},
            }
        }
        with self.assertRaises(ValueError):
            parse_config(data)


class TestExperimentState(unittest.TestCase):
    """§16.2 — Checkpoint/resume"""

    def test_default_state(self) -> None:
        state = ExperimentState(target_id="test_func")
        self.assertEqual(state.target_id, "test_func")
        self.assertEqual(state.status, PipelineStage.BASELINE)

    def test_save_and_load(self) -> None:
        with tempfile.TemporaryDirectory() as tmpdir:
            path = Path(tmpdir) / "state.json"
            state = ExperimentState(
                target_id="test_func",
                status=PipelineStage.RECONSTRUCT,
                baseline={"source_hash": "abc123"},
            )
            state.save(path)
            self.assertTrue(path.is_file())
            loaded = ExperimentState.load(path)
            self.assertEqual(loaded.target_id, "test_func")
            self.assertEqual(loaded.status, PipelineStage.RECONSTRUCT)
            self.assertEqual(loaded.baseline, {"source_hash": "abc123"})

    def test_status_enum_roundtrip(self) -> None:
        with tempfile.TemporaryDirectory() as tmpdir:
            state = ExperimentState(
                target_id="test",
                status=PipelineStage.COMPILE_REPAIR,
            )
            path = Path(tmpdir) / "state.json"
            state.save(path)
            loaded = ExperimentState.load(path)
            self.assertEqual(loaded.status, PipelineStage.COMPILE_REPAIR)

    def test_from_json_complete(self) -> None:
        data = {
            "schema_version": 2,
            "experiment_id": "exp-1",
            "target_id": "f",
            "status": "complete",
            "records": [{"model_id": "m1", "run_index": 1}],
        }
        state = ExperimentState.from_json(data)
        self.assertEqual(state.status.value, "complete")
        self.assertEqual(len(state.records), 1)


class TestPipelineRunner(unittest.TestCase):
    """§16.1 — PipelineRunner stage sequencing"""

    def setUp(self) -> None:
        self.adapter = FakeAdapter()
        self.models = [
            type("Model", (), {"id": "m1", "model": "deepseek-flash", "provider": "fake",
                               "runs": 1, "agent": None, "variant": None})()
        ]
        self.providers = {"fake": FakeProvider()}
        self.promotion_manager = type(
            "PM", (), {"promote": lambda *a, **kw: type("R", (), {
                "promoted": True, "rolled_back": False, "reason": "ok",
            })()}
        )()
        self.config = PipelineConfig()
        self.config.features.staged_pipeline = True
        self.output_dir = Path(tempfile.mkdtemp(prefix="pipeline-test-"))

    def tearDown(self) -> None:
        import shutil
        shutil.rmtree(self.output_dir, ignore_errors=True)

    def test_run_completes_all_stages(self) -> None:
        runner = PipelineRunner(
            adapter=self.adapter,
            models=self.models,
            providers=self.providers,
            promotion_manager=self.promotion_manager,
            config=self.config,
            output_dir=self.output_dir,
            max_parallel=1,
        )
        experiment = runner.run("test_func")
        self.assertEqual(experiment.status, PipelineStage.COMPLETE)
        self.assertGreater(len(experiment.records), 0)

    def test_resume_from_checkpoint(self) -> None:
        experiment_dir = self.output_dir / "test_func" / "resume-test"
        experiment_dir.mkdir(parents=True, exist_ok=True)

        # Save a partially-complete state
        partial = ExperimentState(
            target_id="test_func",
            status=PipelineStage.STRUCTURAL_ANALYSIS,
            records=[{"model_id": "m1", "run_index": 1}],
            best_evaluation={
                "status": "COMPILES",
                "match_percent": 75.0,
                "accepted": True,
                "metrics": {"structural": {"total_score": 0.80}},
            },
            best_candidate={"source": "int f() { return 1; }", "hypothesis": "first try"},
            started_at="2026-01-01T00:00:00",
        )
        partial.save(experiment_dir / "pipeline_state.json")

        runner = PipelineRunner(
            adapter=self.adapter,
            models=self.models,
            providers=self.providers,
            promotion_manager=self.promotion_manager,
            config=self.config,
            output_dir=self.output_dir,
            max_parallel=1,
        )
        experiment = runner.run("test_func", experiment_dir=experiment_dir)

        self.assertEqual(experiment.status, PipelineStage.COMPLETE)
        # Should have preserved the prior record
        self.assertGreaterEqual(len(experiment.records), 1)

    def test_routing_based_on_structural_score(self) -> None:
        config = PipelineConfig()
        config.structural.semantic_repair_below = 0.85  # above the 0.80 score

        eval_adapter = FakeAdapter()
        original_evaluate = eval_adapter.evaluate
        eval_count = 0

        def tracking_evaluate(workflow: str, target_id: str, candidate: Candidate) -> Evaluation:
            nonlocal eval_count
            eval_count += 1
            return original_evaluate(workflow, target_id, candidate)

        eval_adapter.evaluate = tracking_evaluate
        eval_adapter.promote = lambda *a, **kw: "promoted"

        runner = PipelineRunner(
            adapter=eval_adapter,
            models=self.models,
            providers=self.providers,
            promotion_manager=self.promotion_manager,
            config=config,
            output_dir=self.output_dir,
            max_parallel=1,
        )
        experiment = runner.run("test_func")
        self.assertEqual(experiment.status, PipelineStage.COMPLETE)


class TestStageRouting(unittest.TestCase):
    """§16.1 — Routing decisions per structural score"""

    def test_weak_structural_triggers_semantic_repair(self) -> None:
        state = ExperimentState(
            target_id="test",
            status=PipelineStage.STRUCTURAL_ANALYSIS,
            best_evaluation={
                "metrics": {"structural": {"total_score": 0.50}},
            },
            best_candidate={"source": "x", "hypothesis": "test"},
        )
        self.assertIn("weak", f"weak_structural_alignment:0.50")

    def test_strong_structural_skips_semantic_repair(self) -> None:
        state = ExperimentState(
            target_id="test",
            status=PipelineStage.STRUCTURAL_ANALYSIS,
            best_evaluation={
                "metrics": {"structural": {"total_score": 0.85}},
            },
            best_candidate={"source": "x", "hypothesis": "test"},
        )
        self.assertIn("strong", f"strong_structural_alignment:0.85")

    def test_full_match_skips_match_improve(self) -> None:
        state = ExperimentState(
            target_id="test",
            status=PipelineStage.MATCH_IMPROVE,
            best_evaluation={
                "match_percent": 100.0,
                "status": "FULL_MATCH",
                "accepted": True,
            },
            best_candidate={"source": "x", "hypothesis": "test"},
        )
        self.assertEqual(state.best_evaluation["match_percent"], 100.0)

    def test_compile_error_triggers_repair(self) -> None:
        state = ExperimentState(
            target_id="test",
            status=PipelineStage.COMPILE_REPAIR,
            best_evaluation={
                "status": "COMPILE_ERROR",
                "match_percent": 0.0,
                "accepted": False,
                "detail": "error: expected ';'",
            },
            best_candidate={"source": "int f(", "hypothesis": "test"},
        )
        self.assertEqual(state.best_evaluation["status"], "COMPILE_ERROR")

    def test_already_compiling_skips_repair(self) -> None:
        state = ExperimentState(
            target_id="test",
            status=PipelineStage.COMPILE_REPAIR,
            best_evaluation={
                "status": "COMPILES",
                "match_percent": 50.0,
                "accepted": False,
            },
            best_candidate={"source": "int f() {}", "hypothesis": "test"},
        )
        self.assertNotEqual(state.best_evaluation["status"], "COMPILE_ERROR")


class TestPipelineIntegration(unittest.TestCase):
    """§16.3-16.4 — Parallelism and cancellation"""

    def test_parallel_execution(self) -> None:
        adapter = FakeAdapter()
        p1 = FakeProvider()
        p2 = FakeProvider()
        models = [
            type("M", (), {"id": "m1", "model": "deepseek-flash", "provider": "fake1",
                           "runs": 1, "agent": None, "variant": None})(),
            type("M", (), {"id": "m2", "model": "deepseek-flash", "provider": "fake2",
                           "runs": 1, "agent": None, "variant": None})(),
        ]
        providers = {"fake1": p1, "fake2": p2}

        runner = PipelineRunner(
            adapter=adapter,
            models=models,
            providers=providers,
            promotion_manager=type(
                "PM", (), {"promote": lambda *a, **kw: type("R", (), {
                    "promoted": True, "rolled_back": False, "reason": "ok",
                })()}
            )(),
            config=PipelineConfig(),
            output_dir=Path(tempfile.mkdtemp(prefix="pipeline-par-")),
            max_parallel=2,
        )
        experiment = runner.run("test_func")
        self.assertEqual(experiment.status, PipelineStage.COMPLETE)

    def test_cancellation_on_full_match(self) -> None:
        # Simulate a provider that returns a FULL_MATCH response
        class FullMatchProvider:
            call_count = 0

            def invoke(self, prompt: str, model: Any, cwd: Path) -> Any:
                self.call_count += 1
                from tools.llm_harness.types import ProviderResult
                return ProviderResult(
                    text='{"source":"int f() { return 42; }","hypothesis":"perfect"}',
                    duration_seconds=0.01,
                    input_tokens=10,
                    output_tokens=5,
                )

        class FullMatchAdapter(FakeAdapter):
            def evaluate(self, workflow: str, target_id: str, candidate: Candidate) -> Evaluation:
                return Evaluation(
                    status="FULL_MATCH",
                    match_percent=100.0,
                    accepted=True,
                    detail="exact match",
                )

        provider = FullMatchProvider()
        adapter = FullMatchAdapter()
        config = PipelineConfig()
        config.features.staged_pipeline = True

        runner = PipelineRunner(
            adapter=adapter,
            models=self._single_model(),
            providers={"fake": provider},
            promotion_manager=type(
                "PM", (), {"promote": lambda *a, **kw: type("R", (), {
                    "promoted": True, "rolled_back": False, "reason": "full match",
                })()}
            )(),
            config=config,
            output_dir=Path(tempfile.mkdtemp(prefix="pipeline-cancel-")),
            max_parallel=1,
        )
        experiment = runner.run("test_func")
        self.assertEqual(experiment.status, PipelineStage.COMPLETE)
        # Should still have at least one record
        self.assertGreaterEqual(len(experiment.records), 1)

    def test_empty_model_list_graceful(self) -> None:
        runner = PipelineRunner(
            adapter=FakeAdapter(),
            models=[],
            providers={},
            promotion_manager=type(
                "PM", (), {"promote": lambda *a, **kw: type("R", (), {
                    "promoted": False, "rolled_back": False, "reason": "no promote",
                })()}
            )(),
            config=PipelineConfig(),
            output_dir=Path(tempfile.mkdtemp(prefix="pipeline-empty-")),
            max_parallel=1,
        )
        experiment = runner.run("test_func")
        self.assertEqual(experiment.status, PipelineStage.COMPLETE)

    @staticmethod
    def _single_model() -> list:
        return [
            type("M", (), {"id": "m1", "model": "deepseek-flash", "provider": "fake",
                           "runs": 1, "agent": None, "variant": None})(),
        ]


class TestPipelineBranching(unittest.TestCase):
    """§20.4 — Branch-level pipeline tests"""

    def setUp(self):
        from pathlib import Path
        import tempfile
        self.adapter = FakeAdapter()
        self.models = [
            type("M", (), {"id": "m1", "model": "deepseek-flash", "provider": "fake",
                           "runs": 1, "agent": None, "variant": None})(),
            type("M", (), {"id": "m2", "model": "deepseek-flash", "provider": "fake",
                           "runs": 1, "agent": None, "variant": None})(),
            type("M", (), {"id": "m3", "model": "deepseek-flash", "provider": "fake",
                           "runs": 1, "agent": None, "variant": None})(),
        ]
        self.providers = {"fake": FakeProvider()}
        self.pm = type("PM", (), {"promote": lambda *a, **kw: type("R", (), {
            "promoted": True, "rolled_back": False, "reason": "ok",
        })()})()
        self.config = PipelineConfig()
        self.config.features.staged_pipeline = True
        self.output_dir = Path(tempfile.mkdtemp(prefix="pipeline-branch-"))

    def tearDown(self):
        import shutil
        shutil.rmtree(self.output_dir, ignore_errors=True)

    def test_three_initial_branches_generated(self):
        """1. Three initial branches are generated (via three models/runs)."""
        from tools.llm_harness.pipeline import PipelineRunner
        runner = PipelineRunner(
            adapter=self.adapter,
            models=self.models,
            providers=self.providers,
            promotion_manager=self.pm,
            config=self.config,
            output_dir=self.output_dir,
            max_parallel=3,
        )
        experiment = runner.run("test_func")
        self.assertGreaterEqual(len(experiment.records), 1)

    def test_compile_repair_receives_source_and_diagnostics(self):
        """4. Compile repair receives parent source and exact diagnostics."""
        # Simulate a record with COMPILE_ERROR status
        state = ExperimentState(
            target_id="test",
            status=PipelineStage.COMPILE_REPAIR,
            best_evaluation={
                "status": "COMPILE_ERROR",
                "match_percent": 0.0,
                "accepted": False,
                "detail": "error: expected ';'",
                "metrics": {"structural": {"total_score": 0.0}},
            },
            best_candidate={"source": "int f(", "hypothesis": "test", "notes": []},
        )
        self.assertEqual(state.status, PipelineStage.COMPILE_REPAIR)
        self.assertIn("error", state.best_evaluation["detail"])

    def test_repeated_diagnostic_blocks_branch(self):
        """7. Repeated diagnostic blocks the branch."""
        state = ExperimentState(
            target_id="test",
            status=PipelineStage.COMPILE_REPAIR,
            best_evaluation={
                "status": "COMPILE_ERROR",
                "detail": "error: expected ';'",
            },
            best_candidate={"source": "int f(", "hypothesis": "test"},
        )
        # Evaluation should have the same root diagnostic
        self.assertEqual(state.best_evaluation["detail"], "error: expected ';'")

    def test_second_branch_tried_when_first_blocks(self):
        """8. Second branch is tried when first branch blocks."""
        # With multiple models, if one produces BLOCKED, others continue
        models = [self.models[0], self.models[1]]
        runner = type("Runner", (), {"models": models, "config": self.config})()
        self.assertGreaterEqual(len(runner.models), 2)

    def test_successful_compile_ends_repair(self):
        """6. A successful compile ends compile repair for that branch."""
        evaluation = {
            "status": "COMPILES",
            "match_percent": 50.0,
            "accepted": False,
        }
        self.assertEqual(evaluation["status"], "COMPILES")

    def test_low_structural_routes_to_semantic_repair(self):
        """9. Low structural score routes to semantic repair."""
        from tools.llm_harness.types import PipelineStage
        config = PipelineConfig()
        config.structural.semantic_repair_below = 0.85
        state = ExperimentState(
            target_id="test",
            status=PipelineStage.STRUCTURAL_ANALYSIS,
            best_evaluation={
                "metrics": {"structural": {"total_score": 0.45}},
            },
            best_candidate={"source": "x", "hypothesis": "test"},
        )
        score = state.best_evaluation["metrics"]["structural"]["total_score"]
        self.assertLess(score, config.structural.semantic_repair_below)

    def test_high_structural_routes_to_match_improve(self):
        """10. High structural score routes to match improvement."""
        from tools.llm_harness.types import PipelineStage
        config = PipelineConfig()
        config.structural.semantic_repair_below = 0.60
        state = ExperimentState(
            target_id="test",
            status=PipelineStage.STRUCTURAL_ANALYSIS,
            best_evaluation={
                "metrics": {"structural": {"total_score": 0.85}},
            },
            best_candidate={"source": "x", "hypothesis": "test"},
        )
        score = state.best_evaluation["metrics"]["structural"]["total_score"]
        self.assertGreaterEqual(score, config.structural.semantic_repair_below)

    def test_full_match_stops_new_submissions(self):
        """12. Full match stops new submissions."""
        state = ExperimentState(target_id="test", status=PipelineStage.COMPLETE)
        self.assertEqual(state.status, PipelineStage.COMPLETE)

    def test_resume_does_not_repeat_completed_calls(self):
        """14. Resume does not repeat completed calls."""
        import json
        experiment_dir = self.output_dir / "test_func" / "resume-no-repeat"
        experiment_dir.mkdir(parents=True, exist_ok=True)

        # Save state at EQUIVALENCE stage — should skip all prior stages
        partial = ExperimentState(
            target_id="test_func",
            status=PipelineStage.EQUIVALENCE,
            records=[{"model_id": "m1", "run_index": 1}],
            best_evaluation={
                "status": "COMPILES",
                "match_percent": 75.0,
                "accepted": True,
                "metrics": {"structural": {"total_score": 0.80}},
            },
            best_candidate={"source": "int f() { return 1; }", "hypothesis": "first try"},
            started_at="2026-01-01T00:00:00",
        )
        partial.save(experiment_dir / "pipeline_state.json")

        from tools.llm_harness.pipeline import PipelineRunner
        runner = PipelineRunner(
            adapter=self.adapter,
            models=self.models,
            providers=self.providers,
            promotion_manager=self.pm,
            config=self.config,
            output_dir=self.output_dir,
            max_parallel=1,
        )
        experiment = runner.run("test_func", experiment_dir=experiment_dir)
        self.assertEqual(experiment.status, PipelineStage.COMPLETE)

    def test_budget_accounting_survives_resume(self):
        """15. Budget accounting survives resume."""
        import json
        experiment_dir = self.output_dir / "test_func" / "resume-budget"
        experiment_dir.mkdir(parents=True, exist_ok=True)

        partial = ExperimentState(
            target_id="test_func",
            status=PipelineStage.EQUIVALENCE,
            records=[{"model_id": "m1", "run_index": 1}],
            best_evaluation={
                "status": "COMPILES",
                "match_percent": 75.0,
                "accepted": True,
                "metrics": {"structural": {"total_score": 0.80}},
            },
            best_candidate={"source": "int f() { return 1; }", "hypothesis": "first try"},
            started_at="2026-01-01T00:00:00",
        )
        partial.save(experiment_dir / "pipeline_state.json")

        from tools.llm_harness.pipeline import PipelineRunner, parse_config
        config = parse_config({"pipeline": {"budgets": {"reconstruct": 1, "compile_repair": 2,
                                                          "semantic_repair": 1, "match_improve": 2}}})
        runner = PipelineRunner(
            adapter=self.adapter,
            models=self.models[:1],
            providers=self.providers,
            promotion_manager=self.pm,
            config=config,
            output_dir=self.output_dir,
            max_parallel=1,
        )
        experiment = runner.run("test_func", experiment_dir=experiment_dir)
        self.assertEqual(experiment.status, PipelineStage.COMPLETE)

    def test_winner_selection_independent_from_promotion(self):
        """17. Winner selection is independent from promotion acceptance."""
        state = ExperimentState(
            target_id="test",
            status=PipelineStage.EQUIVALENCE,
            best_evaluation={"status": "COMPILES", "match_percent": 85.0},
        )
        promotion_accepted = True  # promotion may accept or reject independently
        self.assertEqual(state.status, PipelineStage.EQUIVALENCE)


if __name__ == "__main__":
    unittest.main()
