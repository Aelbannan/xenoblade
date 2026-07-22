"""Fake-adapter ingestion tests for web-batch promote / debug overwrite."""
from __future__ import annotations

import json
import tempfile
import threading
import unittest
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any, Dict, List, Optional

from tools.llm_harness.types import Candidate, Evaluation, PromotionResult
from tools.llm_harness.web_batches import (
    EXIT_INFRA,
    EXIT_OK,
    FENCE_POLICY,
    WebBatch,
    WebFunctionResponse,
    create_web_batches,
    ingest_web_batch_output,
    render_batch_request,
    render_web_response,
    web_target_from_row,
)


@dataclass
class FakeAdapter:
    eval_by_id: Dict[str, Evaluation]
    promote_fail: set[str] = field(default_factory=set)
    promoted: List[str] = field(default_factory=list)
    sources: Dict[str, str] = field(default_factory=dict)
    prompts: List[Dict[str, Any]] = field(default_factory=list)

    def web_batch_candidates(self, **kwargs: Any) -> List[Dict[str, Any]]:
        return []

    def build_prompt(
        self,
        workflow: str,
        target_id: str,
        history: list,
        options: Optional[dict] = None,
    ) -> str:
        self.prompts.append(
            {"workflow": workflow, "target_id": target_id, "options": options}
        )
        return f"PROMPT for {target_id} ({workflow})\n"

    def evaluate(self, workflow: str, target_id: str, candidate: Candidate) -> Evaluation:
        return self.eval_by_id[target_id]

    def promote(self, *args: Any, **kwargs: Any) -> str:
        raise AssertionError("direct promote should go through harness wrapper")

    def ensure_auto_promote_claim(self, target_id: str, owner: str) -> None:
        return None

    def read_target_source(self, target_id: str) -> str:
        return self.sources.get(target_id, "original\n")

    def target_source_path(self, target_id: str) -> Path:
        return Path("/tmp/unused")


def _eval(
    status: str,
    match: float,
    *,
    accepted: bool = False,
    symbol_accepted: bool | None = None,
    detail: str = "",
    binary_feedback: dict | None = None,
) -> Evaluation:
    sa = accepted if symbol_accepted is None else symbol_accepted
    metrics = {}
    if binary_feedback is not None:
        metrics["binary_feedback"] = binary_feedback
        metrics["mismatch_fingerprint"] = "deadbeef"
    return Evaluation(
        status=status,
        match_percent=match,
        accepted=accepted,
        symbol_accepted=sa,
        detail=detail,
        metrics=metrics,
    )


class FakeHarness:
    def __init__(self, adapter: FakeAdapter, root: Path) -> None:
        self.adapter = adapter
        self.root = root
        self.config_path = root / "llm-harness.json"
        self.output_dir = root / "build" / "llm-harness"
        self.output_dir.mkdir(parents=True, exist_ok=True)
        self._adapter_lock = threading.RLock()
        self._promote_lock = threading.Lock()
        self.workspace_manager = None

    def records(self, target_id: str | None = None) -> list:
        return []

    def build_external_prompt(
        self,
        workflow: str,
        target_id: str,
        *,
        repair_context: dict | None = None,
    ) -> str:
        options = {"repair_context": repair_context} if repair_context else None
        return self.adapter.build_prompt(workflow, target_id, [], options)

    def evaluate_external_candidate(
        self,
        target_id: str,
        source: str,
        *,
        hypothesis: str = "",
        workspace_root: Path | None = None,
    ) -> Evaluation:
        return self.adapter.evaluate("improve", target_id, Candidate(source=source))

    def promote_external_accepted(
        self,
        target_id: str,
        source: str,
        evaluation: Any,
        *,
        owner: str,
        batch_id: str = "",
        batch_round: int = 0,
        dry_run: bool = False,
    ) -> PromotionResult:
        if target_id in self.adapter.promote_fail:
            # Simulate guarded promote failure with rollback
            return PromotionResult(
                promoted=False,
                rolled_back=True,
                reason="canonical re-eval HIGH_MATCH",
                target_id=target_id,
            )
        self.adapter.promoted.append(target_id)
        self.adapter.sources[target_id] = source
        status = (
            evaluation.get("status")
            if isinstance(evaluation, dict)
            else getattr(evaluation, "status", "")
        )
        return PromotionResult(
            promoted=True,
            rolled_back=False,
            reason=f"promoted status={status}",
            target_id=target_id,
        )


def _seed_batch(
    root: Path,
    *,
    targets: list[WebTarget],
    prompts: dict[str, str] | None = None,
) -> Path:
    out = root / "web_batches"
    out.mkdir(parents=True, exist_ok=True)
    (out / "history").mkdir(exist_ok=True)
    batch = WebBatch(batch_id="batch_001", index=1, budget_limit=50, targets=targets)
    prompt_map = prompts or {t.target_id: f"prompt {t.target_id}" for t in targets}
    request = render_batch_request(batch, prompt_map, round_number=0, mode="initial")
    (out / "batch_001.txt").write_text(request, encoding="utf-8")
    manifest = {
        "schema_version": 1,
        "created_at": "2026-07-22T00:00:00Z",
        "config_path": "llm-harness.json",
        "budget_default": 50,
        "selection": "ready",
        "certified_funcs": True,
        "tu": None,
        "randomized": False,
        "random_seed": None,
        "fence_policy": FENCE_POLICY,
        "acceptance_policy": "FULL_MATCH_or_certified_EQUIVALENT_MATCH",
        "batches": [
            {
                "batch_id": "batch_001",
                "active_file": "web_batches/batch_001.txt",
                "response_filename": "batch_001.response.txt",
                "round": 0,
                "status": "OPEN",
                "budget_limit": 50,
                "budget_used": batch.used_budget,
                "original_target_ids": [t.target_id for t in targets],
                "remaining_target_ids": [t.target_id for t in targets],
                "promoted_target_ids": [],
                "promoted_full_match_ids": [],
                "promoted_equivalent_match_ids": [],
                "best_by_target": {},
                "last_response_sha256": None,
                "last_ingest_at": None,
                "target_meta": {
                    t.target_id: {
                        "target_id": t.target_id,
                        "unit": t.unit,
                        "symbol": t.symbol,
                        "source_path": t.source_path,
                        "status": t.status,
                        "instruction_match": t.match_percent,
                        "frontier_kind": t.frontier_kind,
                        "retail_text_size": t.retail_text_size,
                        "retail_instruction_count": t.instruction_count,
                        "direct_call_count": t.direct_call_count,
                        "unresolved_call_count": t.unresolved_call_count,
                        "has_indirect_calls": t.has_indirect_calls,
                        "tier": t.tier_sort,
                        "workflow": t.workflow,
                    }
                    for t in targets
                },
            }
        ],
        "skipped": [],
        "not_packed": [],
    }
    (out / "manifest.json").write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")
    return out


def _row_target(target_id: str, unit: str = "main/u") -> WebTarget:
    return web_target_from_row(
        {
            "target_id": target_id,
            "unit": unit,
            "symbol": target_id,
            "source_path": f"src/{target_id}.cpp",
            "status": "STUB",
            "frontier_kind": "leaf",
            "retail_text_size": 40,
            "retail_instruction_count": 10,
            "direct_call_count": 0,
            "unresolved_call_count": 0,
            "has_indirect_calls": False,
            "tier": 1,
            "workflow": "new",
        }
    )


class WebIngestFakeAdapterTests(unittest.TestCase):
    def test_full_partial_compile_and_idempotent(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "llm-harness.json").write_text("{}", encoding="utf-8")
            targets = [_row_target("A"), _row_target("B"), _row_target("C")]
            out = _seed_batch(root, targets=targets)
            adapter = FakeAdapter(
                eval_by_id={
                    "A": _eval("FULL_MATCH", 100.0, accepted=True, symbol_accepted=True),
                    "B": _eval(
                        "CODE_MATCH",
                        93.0,
                        detail="mismatch",
                        binary_feedback={
                            "first_difference_offset": 4,
                            "instruction_windows": [
                                {
                                    "start_offset": 4,
                                    "retail": ["add r3,r3,r4"],
                                    "candidate": ["addi r3,r3,1"],
                                }
                            ],
                            "differences": [
                                {
                                    "offset": 4,
                                    "retail": "7c832214",
                                    "candidate": "38630001",
                                    "retail_mnemonic": "add",
                                    "candidate_mnemonic": "addi",
                                    "likely_cause": "arith",
                                }
                            ],
                            "relocation_differences": {},
                            "function_size": {"retail": 16, "candidate": 16},
                            "stack_frame": {"retail": 0, "candidate": 0},
                            "structural_summary": {"total_score": 0.8},
                            "num_differences": 1,
                        },
                    ),
                    "C": _eval(
                        "COMPILE_ERROR",
                        0.0,
                        detail="src/C.cpp:1: illegal implicit conversion from 'void *' to 'Actor *'",
                    ),
                }
            )
            harness = FakeHarness(adapter, root)
            response = render_web_response(
                batch_id="batch_001",
                round_number=0,
                entries=[
                    WebFunctionResponse("A", "int A(){return 1;}\n", "full", ""),
                    WebFunctionResponse("B", "int B(){return 2;}\n", "partial", ""),
                    WebFunctionResponse("C", "int C(){return 3;}\n", "compile", ""),
                ],
            )
            response_path = out / "batch_001.response.txt"
            response_path.write_text(response, encoding="utf-8")

            result = ingest_web_batch_output(harness, response_path, owner="chatgpt-web")
            self.assertEqual(result.exit_code, EXIT_OK)
            self.assertEqual(adapter.promoted, ["A"])
            active = (out / "batch_001.txt").read_text(encoding="utf-8")
            self.assertNotIn('id="A"', active)
            self.assertIn('id="B"', active)
            self.assertIn('id="C"', active)
            self.assertIn("Objdiff feedback", active)
            self.assertIn("illegal implicit conversion", active)
            self.assertIn("<<<OBJDIFF_FEEDBACK>>>", active)
            self.assertIn("<<<COMPILER_FEEDBACK>>>", active)
            manifest = json.loads((out / "manifest.json").read_text(encoding="utf-8"))
            self.assertEqual(manifest["batches"][0]["round"], 1)
            self.assertEqual(manifest["batches"][0]["status"], "DEBUG")
            self.assertTrue((result.history_dir / "summary.json").is_file())
            self.assertEqual(result.fence_policy, FENCE_POLICY)

            # Idempotent re-ingest
            again = ingest_web_batch_output(harness, response_path, owner="chatgpt-web")
            self.assertTrue(again.idempotent)
            self.assertEqual(adapter.promoted, ["A"])

    def test_promotion_failure_keeps_unresolved(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "llm-harness.json").write_text("{}", encoding="utf-8")
            targets = [_row_target("A")]
            out = _seed_batch(root, targets=targets)
            original = "ORIGINAL_SOURCE_BYTES\n"
            adapter = FakeAdapter(
                eval_by_id={
                    "A": _eval("FULL_MATCH", 100.0, accepted=True, symbol_accepted=True),
                },
                promote_fail={"A"},
                sources={"A": original},
            )
            harness = FakeHarness(adapter, root)
            response = render_web_response(
                batch_id="batch_001",
                round_number=0,
                entries=[
                    WebFunctionResponse("A", "int A(){return 1;}\n", "looks full", ""),
                ],
            )
            path = out / "batch_001.response.txt"
            path.write_text(response, encoding="utf-8")
            result = ingest_web_batch_output(harness, path)
            self.assertEqual(result.exit_code, EXIT_INFRA)
            self.assertEqual(result.results[0].outcome, "PROMOTION_FAILED")
            self.assertEqual(adapter.sources["A"], original)
            active = (out / "batch_001.txt").read_text(encoding="utf-8")
            self.assertIn('id="A"', active)

    def test_missing_response_keeps_function(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "llm-harness.json").write_text("{}", encoding="utf-8")
            targets = [_row_target("A"), _row_target("B"), _row_target("C")]
            out = _seed_batch(root, targets=targets)
            adapter = FakeAdapter(
                eval_by_id={
                    "A": _eval("CODE_MATCH", 50.0),
                    "B": _eval("CODE_MATCH", 60.0),
                }
            )
            harness = FakeHarness(adapter, root)
            response = render_web_response(
                batch_id="batch_001",
                round_number=0,
                entries=[
                    WebFunctionResponse("A", "int A(){return 1;}\n", "", ""),
                    WebFunctionResponse("B", "int B(){return 2;}\n", "", ""),
                ],
            )
            path = out / "batch_001.response.txt"
            path.write_text(response, encoding="utf-8")
            result = ingest_web_batch_output(harness, path)
            self.assertEqual(result.exit_code, EXIT_OK)
            outcomes = {r.target_id: r.outcome for r in result.results}
            self.assertEqual(outcomes["C"], "MISSING_RESPONSE")
            active = (out / "batch_001.txt").read_text(encoding="utf-8")
            self.assertIn('id="C"', active)

    def test_equivalent_match_promoted(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "llm-harness.json").write_text("{}", encoding="utf-8")
            targets = [_row_target("E")]
            out = _seed_batch(root, targets=targets)
            adapter = FakeAdapter(
                eval_by_id={
                    "E": _eval(
                        "EQUIVALENT_MATCH",
                        72.0,
                        accepted=True,
                        symbol_accepted=True,
                    ),
                }
            )
            # Attach equivalence field
            adapter.eval_by_id["E"].equivalence = "EQUIVALENT"
            harness = FakeHarness(adapter, root)
            response = render_web_response(
                batch_id="batch_001",
                round_number=0,
                entries=[
                    WebFunctionResponse("E", "int E(){return 1;}\n", "proved", ""),
                ],
            )
            path = out / "batch_001.response.txt"
            path.write_text(response, encoding="utf-8")
            result = ingest_web_batch_output(harness, path)
            self.assertEqual(result.results[0].outcome, "EQUIVALENT_MATCH_PROMOTED")
            active = (out / "batch_001.txt").read_text(encoding="utf-8")
            self.assertIn("WEB_DECOMP_BATCH_COMPLETE_V1", active)
            self.assertIn(f"fence_policy: {FENCE_POLICY}", active)


class WebExportDryRunTests(unittest.TestCase):
    def test_create_dry_run_packing(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "llm-harness.json").write_text("{}", encoding="utf-8")

            class Adapter:
                def web_batch_candidates(self, **kwargs):
                    return [
                        {
                            "target_id": f"f{i}",
                            "unit": f"u{i % 2}",
                            "symbol": f"f{i}",
                            "source_path": f"src/f{i}.cpp",
                            "status": "NOT_STARTED",
                            "frontier_kind": "leaf",
                            "retail_text_size": 40,
                            "retail_instruction_count": 10,
                            "direct_call_count": 0,
                            "unresolved_call_count": 0,
                            "has_indirect_calls": False,
                            "tier": 1,
                            "workflow": "new",
                        }
                        for i in range(6)
                    ]

            harness = FakeHarness(Adapter(), root)  # type: ignore[arg-type]
            result = create_web_batches(
                harness, batch_count=3, budget=20, dry_run=True
            )
            self.assertTrue(result.dry_run)
            self.assertEqual(len(result.batches), 3)
            self.assertFalse((root / "web_batches" / "manifest.json").exists())


if __name__ == "__main__":
    unittest.main()
