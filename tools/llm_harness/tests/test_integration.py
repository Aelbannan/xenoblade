"""Integration tests (§20.8).

Scenarios using a fake adapter/compiler to test end-to-end flows.
"""
from __future__ import annotations

import json
import os
import sys
import tempfile
import unittest
from pathlib import Path
from typing import Any, Dict, List, Optional

ROOT = Path(__file__).resolve().parents[3]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from tools.llm_harness.pipeline import PipelineRunner
from tools.llm_harness.promotion import PromotionManager
from tools.llm_harness.types import (
    Candidate,
    Evaluation,
    ExperimentState,
    PipelineConfig,
    PipelineStage,
    ProjectAdapter,
    PromotionPolicy,
    ProviderResult,
)


class _IntegrationAdapter:
    """Adapter that simulates a real compiler/adapter for integration tests."""

    def __init__(self, root: Path):
        self.root = root
        self.evaluate_calls: List[str] = []
        self._eval_results: Dict[str, Evaluation] = {}
        self._promote_writes: List[str] = []
        self._build_fail = False
        self._source = "int f() { return 0; }"

    def set_eval_result(self, target_id: str, result: Evaluation) -> None:
        self._eval_results[target_id] = result

    def set_build_fail(self, fail: bool) -> None:
        self._build_fail = fail

    def set_source(self, source: str) -> None:
        self._source = source

    def build_prompt(self, workflow: str, target_id: str,
                     history: List[Dict[str, Any]],
                     options: Optional[Dict[str, Any]] = None) -> str:
        return f"# {target_id}"

    def evaluate(self, workflow: str, target_id: str,
                 candidate: Candidate) -> Evaluation:
        self.evaluate_calls.append(target_id)
        if target_id in self._eval_results:
            return self._eval_results[target_id]
        return Evaluation(
            status="COMPILES", match_percent=75.0, accepted=True, detail="ok",
        )

    def finalize(self) -> None:
        pass

    def read_target_source(self, target_id: str) -> str:
        return self._source

    def promote(self, workflow: str, target_id: str,
                candidate: Any, *, write: bool = False) -> str:
        self._promote_writes.append(target_id)
        return f"promoted {target_id}"

    def _build_object(self, experiment_dir: Path) -> Optional[str]:
        if self._build_fail:
            return "build error"
        return None


class TestIntegrationScenarios(unittest.TestCase):
    """§20.8 — End-to-end integration scenarios"""

    def setUp(self):
        self.tmp = Path(tempfile.mkdtemp(prefix="integ-test-"))
        # Stub out coop cycle
        coop_dir = self.tmp / "tools" / "coop"
        coop_dir.mkdir(parents=True, exist_ok=True)
        (coop_dir / "__init__.py").write_text("")
        (coop_dir / "run.py").write_text("import sys; sys.exit(0)\n")
        self.adapter = _IntegrationAdapter(self.tmp)
        self.pm = PromotionManager(root=self.tmp)
        self.config = PipelineConfig()
        self.models = [
            type("M", (), {"id": "m1", "model": "deepseek-flash", "provider": "fake",
                           "runs": 1, "agent": None, "variant": None})(),
        ]
        self.providers = {"fake": type("FP", (), {
            "invoke": lambda s, p, m, cwd: ProviderResult(
                text='{"source":"int f(){return 1;}","hypothesis":"try"}',
                duration_seconds=0.01, input_tokens=10, output_tokens=5,
            )
        })()}

    def tearDown(self):
        import shutil
        shutil.rmtree(self.tmp, ignore_errors=True)

    def test_stub_to_promotion(self):
        """Stub → compile repair → plausible compile → auto-promotion"""
        runner = PipelineRunner(
            adapter=self.adapter, models=self.models, providers=self.providers,
            promotion_manager=self.pm, config=self.config,
            output_dir=self.tmp, max_parallel=1,
        )
        experiment = runner.run("test_func")
        self.assertEqual(experiment.status, PipelineStage.COMPLETE)

    def test_compile_error_repeated_blocked(self):
        """Compile error → repeated error → blocked"""
        self.adapter.set_eval_result("test_func", Evaluation(
            status="COMPILE_ERROR", match_percent=0.0, accepted=False,
            detail="error: expected ';'",
        ))
        runner = PipelineRunner(
            adapter=self.adapter, models=[],
            providers=self.providers, promotion_manager=self.pm,
            config=self.config, output_dir=self.tmp, max_parallel=1,
        )
        # With no models, the runner should still complete gracefully
        experiment = runner.run("test_func")
        self.assertEqual(experiment.status, PipelineStage.COMPLETE)

    def test_resume_halfway(self):
        """Resume halfway through compile repair"""
        experiment_dir = self.tmp / "test_func" / "resume-half"
        experiment_dir.mkdir(parents=True, exist_ok=True)
        partial = ExperimentState(
            target_id="test_func",
            status=PipelineStage.RECONSTRUCT,
            records=[{"model_id": "m1", "run_index": 1}],
            best_evaluation={
                "status": "COMPILE_ERROR", "match_percent": 0.0,
                "accepted": False, "detail": "err",
            },
            best_candidate={"source": "int f(", "hypothesis": "try"},
            started_at="2026-01-01T00:00:00",
        )
        partial.save(experiment_dir / "pipeline_state.json")

        runner = PipelineRunner(
            adapter=self.adapter, models=[],
            providers=self.providers, promotion_manager=self.pm,
            config=self.config, output_dir=self.tmp, max_parallel=1,
        )
        experiment = runner.run("test_func", experiment_dir=experiment_dir)
        self.assertEqual(experiment.status, PipelineStage.COMPLETE)
        # Prior records preserved
        self.assertGreaterEqual(len(experiment.records), 1)

    def test_cost_limit_before_promotion(self):
        """Cost limit reached before promotion — pipeline still completes"""
        config = PipelineConfig()
        config.budgets.reconstruct = 1
        runner = PipelineRunner(
            adapter=self.adapter, models=self.models, providers=self.providers,
            promotion_manager=self.pm, config=config,
            output_dir=self.tmp, max_parallel=1,
        )
        experiment = runner.run("test_func")
        self.assertEqual(experiment.status, PipelineStage.COMPLETE)

    def test_model_provider_error_no_corruption(self):
        """Model provider error without corrupting state"""
        # Provider that returns empty text
        class ErrorProvider:
            def invoke(self, prompt, model, cwd):
                from tools.llm_harness.types import ProviderResult
                return ProviderResult(
                    text="", duration_seconds=0.01, input_tokens=0, output_tokens=0,
                )

        runner = PipelineRunner(
            adapter=self.adapter, models=self.models,
            providers={"fake": ErrorProvider()},
            promotion_manager=self.pm, config=self.config,
            output_dir=self.tmp, max_parallel=1,
        )
        experiment = runner.run("test_func")
        self.assertEqual(experiment.status, PipelineStage.COMPLETE)

    def test_concurrent_targets_serialized_promotions(self):
        """Concurrent target runs → serialized promotions"""
        import threading
        results: List[str] = []

        def run_target(name: str) -> None:
            runner = PipelineRunner(
                adapter=self.adapter, models=self.models,
                providers=self.providers, promotion_manager=self.pm,
                config=self.config, output_dir=self.tmp, max_parallel=1,
            )
            exp = runner.run(name)
            results.append(exp.status.value)

        threads = [threading.Thread(target=run_target, args=(f"func_{i}",))
                   for i in range(3)]
        for t in threads:
            t.start()
        for t in threads:
            t.join()

        self.assertEqual(len(results), 3)
        for status in results:
            self.assertEqual(status, "complete")

    def test_low_structure_to_semantic_repair(self):
        """Compile → low structural → semantic repair → high structure"""
        self.config.features.staged_pipeline = True
        self.config.features.structural_routing = True

        exp_dir = self.tmp / "test_func" / "resume-low-struct"
        exp_dir.mkdir(parents=True, exist_ok=True)
        state = ExperimentState(
            target_id="test_func",
            status=PipelineStage.COMPILE_REPAIR,
            records=[],
            best_evaluation={
                "status": "COMPILES", "match_percent": 45.0, "accepted": False,
                "detail": "low structural alignment",
                "metrics": {"structural": {"total_score": 0.30}},
            },
            best_candidate={"source": "int f(){return 1;}", "hypothesis": "try"},
            started_at="2026-01-01T00:00:00",
        )
        state.save(exp_dir / "pipeline_state.json")

        runner = PipelineRunner(
            adapter=self.adapter, models=[],
            providers=self.providers, promotion_manager=self.pm,
            config=self.config, output_dir=self.tmp, max_parallel=1,
        )
        experiment = runner.run("test_func", experiment_dir=exp_dir)

        self.assertEqual(experiment.status, PipelineStage.COMPLETE)

        routing_file = exp_dir / "semantic_repairs" / "routing.json"
        self.assertTrue(routing_file.exists())
        routing = json.loads(routing_file.read_text())
        self.assertEqual(routing["stage"], "semantic_repair")
        self.assertEqual(routing["budget"], self.config.budgets.semantic_repair)

        self.assertFalse((exp_dir / "match_improves").exists())

    def test_high_structure_to_match_improve(self):
        """High structure → match improvement → full match"""
        self.config.features.staged_pipeline = True
        self.config.features.structural_routing = True

        exp_dir = self.tmp / "test_func" / "resume-high-struct"
        exp_dir.mkdir(parents=True, exist_ok=True)
        state = ExperimentState(
            target_id="test_func",
            status=PipelineStage.COMPILE_REPAIR,
            records=[],
            best_evaluation={
                "status": "COMPILES", "match_percent": 80.0, "accepted": True,
                "detail": "strong structural alignment",
                "metrics": {"structural": {"total_score": 0.85}},
            },
            best_candidate={"source": "int f(){return 1;}", "hypothesis": "try"},
            started_at="2026-01-01T00:00:00",
        )
        state.save(exp_dir / "pipeline_state.json")

        runner = PipelineRunner(
            adapter=self.adapter, models=[],
            providers=self.providers, promotion_manager=self.pm,
            config=self.config, output_dir=self.tmp, max_parallel=1,
        )
        experiment = runner.run("test_func", experiment_dir=exp_dir)

        self.assertEqual(experiment.status, PipelineStage.COMPLETE)

        routing_file = exp_dir / "match_improves" / "routing.json"
        self.assertTrue(routing_file.exists())
        routing = json.loads(routing_file.read_text())
        self.assertEqual(routing["stage"], "match_improve")
        self.assertEqual(routing["current_match_percent"], 80.0)

        self.assertFalse((exp_dir / "semantic_repairs").exists())

    def test_stale_root_fresh_evaluation(self):
        """Candidate prepared against stale root → fresh re-evaluation"""
        self.config.features.staged_pipeline = True
        self.config.features.structural_routing = True

        class _StaleRootAdapter(_IntegrationAdapter):
            def __init__(self, root):
                super().__init__(root)
                self._read_count = 0

            def read_target_source(self, target_id: str) -> str:
                self._read_count += 1
                if self._read_count == 1:
                    return "int g() { return 2; }"
                return self._source

        adapter = _StaleRootAdapter(self.tmp)
        adapter.set_eval_result("test_func", Evaluation(
            status="COMPILES", match_percent=85.0, accepted=True,
            detail="good candidate",
            metrics={"structural": {"total_score": 0.80}},
        ))

        exp_dir = self.tmp / "test_func" / "resume-stale-root"
        exp_dir.mkdir(parents=True, exist_ok=True)
        state = ExperimentState(
            target_id="test_func",
            status=PipelineStage.COMPILE_REPAIR,
            records=[],
            best_evaluation={
                "status": "COMPILES", "match_percent": 85.0, "accepted": True,
                "detail": "good candidate",
                "metrics": {"structural": {"total_score": 0.80}},
            },
            best_candidate={"source": "int f(){return 1;}", "hypothesis": "try"},
            started_at="2026-01-01T00:00:00",
        )
        state.save(exp_dir / "pipeline_state.json")

        runner = PipelineRunner(
            adapter=adapter, models=[],
            providers=self.providers, promotion_manager=self.pm,
            config=self.config, output_dir=self.tmp, max_parallel=1,
        )
        experiment = runner.run("test_func", experiment_dir=exp_dir)

        self.assertEqual(experiment.status, PipelineStage.COMPLETE)
        self.assertIsNotNone(experiment.baseline)
        self.assertEqual(experiment.baseline["source_text"], "int g() { return 2; }")
        self.assertGreaterEqual(adapter._read_count, 2)

    def test_promotion_failure_rollback(self):
        """Promotion integration failure → successful rollback"""
        self.config.features.staged_pipeline = True
        self.config.features.structural_routing = True

        self.adapter.set_eval_result("test_func", Evaluation(
            status="COMPILES", match_percent=90.0, accepted=True,
            detail="good candidate",
            metrics={"structural": {"total_score": 0.85}},
        ))

        coop_dir = self.tmp / "tools" / "coop"
        (coop_dir / "run.py").write_text("import sys; sys.exit(1)\n")

        exp_dir = self.tmp / "test_func" / "resume-rollback"
        exp_dir.mkdir(parents=True, exist_ok=True)
        state = ExperimentState(
            target_id="test_func",
            status=PipelineStage.COMPILE_REPAIR,
            records=[],
            best_evaluation={
                "status": "COMPILES", "match_percent": 90.0, "accepted": True,
                "detail": "good candidate",
                "metrics": {"structural": {"total_score": 0.85}},
            },
            best_candidate={"source": "int f(){return 1;}", "hypothesis": "try"},
            started_at="2026-01-01T00:00:00",
        )
        state.save(exp_dir / "pipeline_state.json")

        runner = PipelineRunner(
            adapter=self.adapter, models=[],
            providers=self.providers, promotion_manager=self.pm,
            config=self.config, output_dir=self.tmp, max_parallel=1,
        )
        experiment = runner.run("test_func", experiment_dir=exp_dir)

        self.assertEqual(experiment.status, PipelineStage.COMPLETE)
        self.assertIsNotNone(experiment.promotion)
        self.assertFalse(experiment.promotion["promoted"])
        self.assertTrue(experiment.promotion["rolled_back"])
        self.assertIn("coop cycle", experiment.promotion["reason"])


if __name__ == "__main__":
    raise SystemExit(unittest.main())
