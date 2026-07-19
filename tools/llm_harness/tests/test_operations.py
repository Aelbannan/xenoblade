from __future__ import annotations

import json
import sys
import tempfile
import unittest
from pathlib import Path
from typing import Any, Dict, List, Optional

ROOT = Path(__file__).resolve().parents[3]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from tools.llm_harness.pipeline import PipelineRunner
from tools.llm_harness.promotion import (
    InvalidPatch, TargetMetadata, validate_patch, validate_candidate_patches,
)
from tools.llm_harness.types import (
    Candidate, Evaluation, ExperimentState, FeaturesConfig, PipelineConfig,
    PipelineEvent, PipelineStage, ProjectAdapter, PromotionPolicy,
    ProviderResult, SourcePatch,
)


class TestFeaturesConfig(unittest.TestCase):
    """§23.1 — Feature flags"""

    def test_defaults(self):
        f = FeaturesConfig()
        self.assertTrue(f.transactional_promotion)
        self.assertTrue(f.typed_dossier)
        self.assertFalse(f.staged_pipeline)
        self.assertFalse(f.structural_routing)
        self.assertFalse(f.retrieved_knowledge)

    def test_from_dict(self):
        config = PipelineConfig.from_dict({
            "features": {"staged_pipeline": True, "structural_routing": True},
        })
        self.assertTrue(config.features.staged_pipeline)
        self.assertTrue(config.features.structural_routing)
        self.assertTrue(config.features.transactional_promotion)

    def test_legacy_default_false(self):
        """staged_pipeline defaults to False so old behavior is preserved."""
        config = PipelineConfig()
        self.assertFalse(config.features.staged_pipeline)


class TestPipelineEvent(unittest.TestCase):
    """§24.1 — Structured event log"""

    def test_event_creation(self):
        ev = PipelineEvent(
            event="baseline_captured",
            experiment_id="exp-1",
            target_id="test_func",
            stage="baseline",
        )
        self.assertEqual(ev.event, "baseline_captured")
        self.assertEqual(ev.target_id, "test_func")

    def test_event_to_dict(self):
        ev = PipelineEvent(event="candidate_evaluated", match_percent=75.0)
        d = ev.to_dict()
        self.assertEqual(d["event"], "candidate_evaluated")
        self.assertAlmostEqual(d["match_percent"], 75.0)

    def test_event_default_timestamp(self):
        ev = PipelineEvent(event="test")
        self.assertEqual(ev.timestamp, "")

    def test_event_default_match_percent(self):
        ev = PipelineEvent(event="test")
        self.assertAlmostEqual(ev.match_percent, 0.0)


class TestStagedPipelineFeatureGate(unittest.TestCase):
    """§23.1 — staged_pipeline flag gates full pipeline stages"""

    def test_legacy_mode_skips_stages(self):
        """When staged_pipeline=False, pipeline completes after reconstruction."""
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            adapter = _make_adapter()
            models = [_make_model("m1")]
            providers = {"fake": _make_provider()}
            pm = _make_pm(root)
            config = PipelineConfig()
            config.features.staged_pipeline = False

            runner = PipelineRunner(
                adapter=adapter, models=models, providers=providers,
                promotion_manager=pm, config=config,
                output_dir=root, max_parallel=1,
            )
            experiment = runner.run("test_func")
            self.assertEqual(experiment.status, PipelineStage.COMPLETE)

    def test_staged_mode_runs_full_pipeline(self):
        """When staged_pipeline=True, pipeline runs all stages."""
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            # Stub coop cycle
            coop_dir = root / "tools" / "coop"
            coop_dir.mkdir(parents=True, exist_ok=True)
            (coop_dir / "__init__.py").write_text("")
            (coop_dir / "run.py").write_text("import sys; sys.exit(0)\n")
            adapter = _make_adapter()
            models = [_make_model("m1")]
            providers = {"fake": _make_provider()}
            pm = _make_pm(root)
            config = PipelineConfig()
            config.features.staged_pipeline = True
            config.features.structural_routing = True

            runner = PipelineRunner(
                adapter=adapter, models=models, providers=providers,
                promotion_manager=pm, config=config,
                output_dir=root, max_parallel=1,
            )
            experiment = runner.run("test_func")
            self.assertEqual(experiment.status, PipelineStage.COMPLETE)


class TestSummaryReport(unittest.TestCase):
    """§24.2 — summary.md generation"""

    def test_summary_written(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            adapter = _make_adapter()
            models = []
            providers = {}
            pm = _make_pm(root)
            config = PipelineConfig()

            runner = PipelineRunner(
                adapter=adapter, models=models, providers=providers,
                promotion_manager=pm, config=config,
                output_dir=root, max_parallel=1,
            )
            experiment = runner.run("test_func")
            # Find the summary file
            summaries = list(root.rglob("summary.md"))
            self.assertGreaterEqual(len(summaries), 1)
            content = summaries[0].read_text()
            self.assertIn("Experiment summary", content)
            self.assertIn(experiment.target_id, content)

    def test_summary_includes_records(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            exp_dir = root / "test_func" / "exp1"
            exp_dir.mkdir(parents=True)
            state = ExperimentState(
                target_id="test_func",
                status=PipelineStage.COMPLETE,
                records=[{"model_id": "m1", "input_tokens": 10, "output_tokens": 5, "cost": 0.001}],
                best_evaluation={
                    "status": "COMPILES", "match_percent": 75.0,
                    "accepted": True,
                    "metrics": {"structural": {"total_score": 0.80}},
                },
            )
            state.save(exp_dir / "pipeline_state.json")

            adapter = _make_adapter()
            pm = _make_pm(root)

            runner = PipelineRunner(
                adapter=adapter, models=[], providers={},
                promotion_manager=pm, config=PipelineConfig(),
                output_dir=root, max_parallel=1,
            )
            runner.run("test_func", experiment_dir=exp_dir)
            summary = exp_dir / "summary.md"
            self.assertTrue(summary.is_file())
            content = summary.read_text()
            self.assertIn("COMPILES", content)
            self.assertIn("75.0%", content)


class TestPatchValidation(unittest.TestCase):
    """§25 — Security: patch validation"""

    def test_valid_patch_passes(self):
        patch = SourcePatch(slot_id="source", source="int f() { return 1; }")
        target = TargetMetadata(target_id="test", source_file="test.cpp",
                                signature="int f()")
        # Should not raise
        validate_patch(patch, target)

    def test_path_traversal_rejected(self):
        patch = SourcePatch(slot_id="../other.cpp", source="x")
        target = TargetMetadata(target_id="test", source_file="test.cpp")
        with self.assertRaises(InvalidPatch):
            validate_patch(patch, target)

    def test_preprocessor_rejected(self):
        patch = SourcePatch(slot_id="source", source="#include <evil.h>")
        target = TargetMetadata(target_id="test", source_file="test.cpp")
        with self.assertRaises(InvalidPatch):
            validate_patch(patch, target)

    def test_shell_command_rejected(self):
        patch = SourcePatch(slot_id="source", source="int x = `rm -rf /`;")
        target = TargetMetadata(target_id="test", source_file="test.cpp")
        with self.assertRaises(InvalidPatch):
            validate_patch(patch, target)

    def test_signature_change_rejected(self):
        patch = SourcePatch(slot_id="source", source="float f() { return 0.0; }")
        target = TargetMetadata(target_id="test", source_file="test.cpp",
                                signature="int f()")
        with self.assertRaises(InvalidPatch):
            validate_patch(patch, target)

    def test_all_patches_validated(self):
        patches = [
            SourcePatch(slot_id="source", source="int f() { return 1; }"),
            SourcePatch(slot_id="../escape", source="x"),
        ]
        target = TargetMetadata(target_id="test", source_file="test.cpp",
                                signature="int f()")
        with self.assertRaises(InvalidPatch):
            validate_candidate_patches(patches, target)


def _make_adapter() -> Any:
    """Create a minimal adapter stub."""
    class Adapter:
        root: Path = Path("/tmp")
        evaluate_calls: List[str] = []

        def build_prompt(self, workflow, target_id, history, options=None):
            return f"# {target_id}"

        def evaluate(self, workflow, target_id, candidate):
            self.evaluate_calls.append(target_id)
            return Evaluation(status="COMPILES", match_percent=75.0,
                              accepted=True, detail="ok")

        def finalize(self):
            pass

        def read_target_source(self, target_id):
            return "int f() { return 0; }"

        def promote(self, workflow, target_id, candidate, *, write=False):
            return f"promoted {target_id}"

    return Adapter()


def _make_model(model_id: str) -> Any:
    return type("M", (), {
        "id": model_id, "model": "deepseek-flash", "provider": "fake",
        "runs": 1, "agent": None, "variant": None,
    })()


def _make_provider() -> Any:
    return type("FP", (), {
        "invoke": lambda s, p, m, cwd: ProviderResult(
            text='{"source":"int f(){return 1;}","hypothesis":"try"}',
            duration_seconds=0.01, input_tokens=10, output_tokens=5,
        )
    })()


def _make_pm(root: Path) -> Any:
    from tools.llm_harness.promotion import PromotionManager
    return PromotionManager(root=root)


if __name__ == "__main__":
    raise SystemExit(unittest.main())
