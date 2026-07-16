from __future__ import annotations

import json
import subprocess
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace

from tools.llm_harness.core import Harness, parse_candidate
from tools.llm_harness.providers import parse_opencode_output
from tools.llm_harness.types import Candidate, ProviderResult, SourcePatch
from tools.llm_harness.workspace import GitWorktreeManager
from tools.llm_harness.xenoblade_project import (
    XenobladeAdapter,
    _begin_marker,
    _end_marker,
    _apply_tu_patches,
    _find_function_region,
    _find_tu_slots,
    _function_size_comparison,
    _knowledge_queries,
    _knowledge_record,
    _insert_marker_slot,
    _insert_empty_tu_slot,
    _replace_function_source,
    _residual_section_names,
    _require_tu_completion_ready,
    _wrap_tu_slot,
)


class CandidateTests(unittest.TestCase):
    def test_parses_fenced_typed_candidate(self) -> None:
        candidate = parse_candidate(
            '```json\n{"source":"int f() { return 1; }","notes":"one","confidence":0.8}\n```'
        )
        self.assertEqual(candidate.notes, ["one"])
        self.assertEqual(candidate.confidence, 0.8)

    def test_rejects_missing_source(self) -> None:
        with self.assertRaises(ValueError):
            parse_candidate('{"hypothesis":"x"}')

    def test_parses_targeted_tu_patches(self) -> None:
        candidate = parse_candidate(
            '{"patches":[{"slot_id":"vtable","source":"static int x = 1;"}],'
            '"hypothesis":"layout"}',
            workflow="tu-complete",
        )
        self.assertEqual(candidate.source, "")
        self.assertEqual(candidate.patches[0].slot_id, "vtable")

    def test_rejects_mixed_full_source_and_tu_patches(self) -> None:
        with self.assertRaisesRegex(ValueError, "either 'source' or 'patches'"):
            parse_candidate(
                '{"source":"x","patches":[{"slot_id":"vtable","source":"y"}]}',
                workflow="tu-complete",
            )

    def test_requires_tu_output_contract_selected_by_prompt_mode(self) -> None:
        with self.assertRaisesRegex(ValueError, "bounded 'patches'"):
            parse_candidate(
                '{"source":"int complete_tu;"}', workflow="tu-complete"
            )
        with self.assertRaisesRegex(ValueError, "complete 'source'"):
            parse_candidate(
                '{"patches":[{"slot_id":"data","source":"int x;"}]}',
                workflow="tu-complete",
                full_context=True,
            )
        candidate = parse_candidate(
            '{"source":"int complete_tu;"}',
            workflow="tu-complete",
            full_context=True,
        )
        self.assertEqual(candidate.source, "int complete_tu;")


class OpenCodeOutputTests(unittest.TestCase):
    def test_parses_text_events_and_usage(self) -> None:
        output = "\n".join([
            json.dumps({"type": "text", "part": {"type": "text", "text": "{\"sou"}}),
            json.dumps({
                "type": "text",
                "part": {"type": "text", "text": "rce\":\"x\"}"},
                "usage": {"input_tokens": 12, "output_tokens": 4},
            }),
        ])
        text, events, usage = parse_opencode_output(output)
        self.assertEqual(text, '{"source":"x"}')
        self.assertEqual(len(events), 2)
        self.assertEqual(usage["input"], 12)
        self.assertEqual(usage["output"], 4)

    def test_sums_opencode_step_costs(self) -> None:
        output = "\n".join([
            json.dumps({
                "type": "step_finish",
                "part": {"type": "step-finish", "cost": 0.03, "tokens": {"input": 10, "output": 2}},
            }),
            json.dumps({
                "type": "step_finish",
                "part": {"type": "step-finish", "cost": 0.04, "tokens": {"input": 4, "output": 3}},
            }),
            json.dumps({"type": "text", "part": {"type": "text", "text": '{"source":"x"}'}}),
        ])
        _, _, usage = parse_opencode_output(output)
        self.assertEqual(usage["input"], 14)
        self.assertEqual(usage["output"], 5)
        self.assertAlmostEqual(usage["cost"], 0.07)


class KnowledgeDossierTests(unittest.TestCase):
    def test_queries_identity_and_prior_mismatch_tags(self) -> None:
        target = SimpleNamespace(function="CThing::Move", symbol="Move__6CThingFv")
        history = [{"candidate_summary": {"notes": ["SDA relocation and stack frame spill"]}}]
        queries = _knowledge_queries(target, history)
        self.assertEqual(queries[0]["query"], "CThing::Move")
        self.assertEqual(queries[1]["query"], "Move__6CThingFv")
        self.assertIn("relocation", {row.get("tag") for row in queries})
        self.assertIn("stack_frame", {row.get("tag") for row in queries})

    def test_full_record_is_capped_and_keeps_stable_id(self) -> None:
        row = {
            "id": "ref:abc",
            "source_kind": "reference",
            "title": "Pattern",
            "body": "abcdefghij",
            "status": "FULL_MATCH",
            "match_percent": 100.0,
            "target_id": "",
            "symbol": "",
            "tags": "relocation abi",
            "source_path": "docs/MWCC_REFERENCE.md",
            "line_start": 10,
        }
        record = _knowledge_record(row, 5)
        self.assertEqual(record["id"], "ref:abc")
        self.assertTrue(record["truncated"])
        self.assertEqual(record["tags"], ["relocation", "abi"])


class FunctionRegionTests(unittest.TestCase):
    def setUp(self) -> None:
        self.target = SimpleNamespace(
            id="thing-move", function="CThing::Move", source=Path("Thing.cpp")
        )

    def test_finds_only_qualified_definition_with_nested_braces(self) -> None:
        source = """void call() { thing.Move(); }

int CThing::Move(int value) {
    const char* brace = "}";
    if (value) { return 1; }
    return 0;
}

void after() {}
"""
        region = _find_function_region(source, self.target)
        function = source[region.content_start : region.content_end]
        self.assertTrue(function.startswith("int CThing::Move"))
        self.assertTrue(function.rstrip().endswith("}"))
        self.assertNotIn("void after", function)

    def test_prefers_stable_markers(self) -> None:
        source = (
            _begin_marker(self.target.id)
            + "\nint CThing::Move() { return 1; }\n"
            + _end_marker(self.target.id)
            + "\n"
        )
        region = _find_function_region(source, self.target)
        self.assertTrue(region.marked)
        self.assertEqual(
            source[region.content_start : region.content_end].strip(),
            "int CThing::Move() { return 1; }",
        )
        replaced = _replace_function_source(source, region, "int CThing::Move() { return 2; }")
        self.assertIn("int CThing::Move() { return 2; }\n" + _end_marker(self.target.id), replaced)

    def test_catalog_signature_does_not_duplicate_parenthesis(self) -> None:
        target = SimpleNamespace(
            id="thing-move", function="ns::CThing::Move(int, float)", source=Path("Thing.cpp")
        )
        source = "int CThing::Move(int value, float scale) { return value; }\n"
        region = _find_function_region(source, target)
        self.assertEqual(source[region.content_start : region.content_end].strip(), source.strip())


class TuCompletionTests(unittest.TestCase):
    def test_residual_sections_exclude_text_handled_by_function_evidence(self) -> None:
        entry = {
            "sections": [
                {"name": ".text", "fuzzy_match_percent": 90},
                {"name": ".data", "fuzzy_match_percent": 80},
                {"name": ".rodata", "fuzzy_match_percent": 100},
            ]
        }
        self.assertEqual(_residual_section_names(entry), [".data"])

    def test_requires_nonzero_match_for_every_function(self) -> None:
        report = SimpleNamespace(
            functions=[
                SimpleNamespace(name="matched", match_percent=1.0),
                SimpleNamespace(name="untouched", match_percent=0.0),
            ]
        )
        with self.assertRaisesRegex(ValueError, "untouched"):
            _require_tu_completion_ready(report)

    def test_accepts_tu_after_every_function_has_first_match(self) -> None:
        report = SimpleNamespace(
            functions=[SimpleNamespace(name="candidate", match_percent=0.1)]
        )
        _require_tu_completion_ready(report)

    def test_tu_rank_prioritizes_guard_before_percentages(self) -> None:
        adapter = XenobladeAdapter.__new__(XenobladeAdapter)
        guarded = {
            "accepted": False,
            "match_percent": 80,
            "metrics": {"guard_ok": True, "accepted_function_count": 2, "data_percent": 80, "code_percent": 80},
        }
        regressed = {
            "accepted": False,
            "match_percent": 99,
            "metrics": {"guard_ok": False, "accepted_function_count": 5, "data_percent": 99, "code_percent": 99},
        }
        self.assertGreater(
            adapter.rank_candidate("tu-complete", guarded),
            adapter.rank_candidate("tu-complete", regressed),
        )

    def test_applies_only_named_tu_slot_contents(self) -> None:
        source = (
            "before\n// LLM-HARNESS-TU-BEGIN: data\nstatic int old;\n"
            "// LLM-HARNESS-TU-END: data\nafter\n"
        )
        slots = _find_tu_slots(source)
        updated = _apply_tu_patches(
            source, slots, [SourcePatch(slot_id="data", source="static int replacement;")]
        )
        self.assertIn("static int replacement;", updated)
        self.assertNotIn("static int old;", updated)
        self.assertTrue(updated.startswith("before\n"))
        self.assertTrue(updated.endswith("after\n"))

    def test_adapter_rejects_unknown_tu_slot(self) -> None:
        adapter = XenobladeAdapter.__new__(XenobladeAdapter)
        adapter.max_source_chars = 1000
        source = (
            "// LLM-HARNESS-TU-BEGIN: known\nstatic int old;\n"
            "// LLM-HARNESS-TU-END: known\n"
        )
        candidate = Candidate(
            source="",
            hypothesis="x",
            patches=[SourcePatch(slot_id="unknown", source="static int x;")],
        )
        with self.assertRaisesRegex(ValueError, "unknown slot"):
            adapter._tu_candidate_source(
                source, candidate, SimpleNamespace(name="unit", source_path=Path("unit.cpp"))
            )

    def test_inserts_and_wraps_exact_tu_slots(self) -> None:
        inserted = _insert_empty_tu_slot("a\nanchor\nb\n", "data", before="anchor")
        self.assertIn("LLM-HARNESS-TU-BEGIN: data", inserted)
        wrapped = _wrap_tu_slot(
            "a\nstatic int x;\nstatic int y;\nb\n",
            "statics",
            start="static int x;",
            end="static int y;",
        )
        slots = _find_tu_slots(wrapped)
        self.assertIn("static int x;", wrapped[slots["statics"].content_start:slots["statics"].content_end])


class FunctionSizeTests(unittest.TestCase):
    def test_equal_and_smaller_functions_fit(self) -> None:
        self.assertTrue(_function_size_comparison(0x40, 0x40)[0])
        self.assertTrue(_function_size_comparison(0x40, 0x30)[0])

    def test_larger_function_does_not_fit(self) -> None:
        fits, detail = _function_size_comparison(0x40, 0x44)
        self.assertFalse(fits)
        self.assertIn("exceeds retail", detail)
        self.assertIn("4 bytes", detail)


class MarkerSlotTests(unittest.TestCase):
    def test_inserts_only_at_unique_exact_anchor(self) -> None:
        target = SimpleNamespace(id="new-fn", function="CThing::newFn")
        source = "namespace x {\n// anchor\n}\n"
        updated = _insert_marker_slot(source, target, before="// anchor")
        self.assertIn("LLM-HARNESS-BEGIN: new-fn", updated)
        self.assertLess(updated.index("LLM-HARNESS-BEGIN"), updated.index("// anchor"))

    def test_rejects_ambiguous_anchor(self) -> None:
        target = SimpleNamespace(id="new-fn", function="CThing::newFn")
        with self.assertRaisesRegex(ValueError, "found 2"):
            _insert_marker_slot("x x", target, before="x")


class WorktreeTests(unittest.TestCase):
    def test_overlays_dirty_snapshot_and_removes_worktree(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp) / "repo"
            root.mkdir()
            subprocess.run(["git", "init", "-q"], cwd=root, check=True)
            subprocess.run(["git", "config", "user.email", "test@example.invalid"], cwd=root, check=True)
            subprocess.run(["git", "config", "user.name", "Test"], cwd=root, check=True)
            (root / "tracked.txt").write_text("base\n", encoding="utf-8")
            subprocess.run(["git", "add", "tracked.txt"], cwd=root, check=True)
            subprocess.run(["git", "commit", "-qm", "base"], cwd=root, check=True)
            (root / "tracked.txt").write_text("dirty\n", encoding="utf-8")
            (root / "new.txt").write_text("new\n", encoding="utf-8")
            manager = GitWorktreeManager(root)
            workspace = manager.create("test")
            try:
                self.assertEqual((workspace / "tracked.txt").read_text(), "dirty\n")
                self.assertEqual((workspace / "new.txt").read_text(), "new\n")
            finally:
                manager.remove(workspace)
            self.assertFalse(workspace.exists())


class ResumeBudgetTests(unittest.TestCase):
    def test_budget_stop_retry_and_resume(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "adapter.py").write_text(
                "from tools.llm_harness.types import Evaluation\n"
                "class A:\n"
                " def __init__(self, root): self.root=root\n"
                " def build_prompt(self, workflow, target, history): return 'prompt'\n"
                " def evaluate(self, workflow, target, candidate): return Evaluation('FULL_MATCH',100,True)\n"
                " def finalize(self): pass\n"
                "def create_adapter(root, settings): return A(root)\n",
                encoding="utf-8",
            )
            (root / "config.json").write_text(json.dumps({
                "project_adapter": "adapter.py",
                "output_dir": "out",
                "execution": {"max_retries": 1},
                "models": [
                    {"id": "a", "provider": "fake", "model": "p/a"},
                    {"id": "b", "provider": "fake", "model": "p/b"},
                ],
            }), encoding="utf-8")

            class FakeProvider:
                def __init__(self) -> None:
                    self.calls = 0

                def invoke(self, prompt, model, cwd):
                    self.calls += 1
                    if self.calls == 1:
                        raise RuntimeError("retry me")
                    return ProviderResult(
                        text='{"source":"int f() { return 1; }","hypothesis":"x"}',
                        duration_seconds=1,
                        input_tokens=10,
                        output_tokens=5,
                        cost=1.0,
                    )

            harness = Harness(root / "config.json")
            fake = FakeProvider()
            harness.providers["fake"] = fake
            experiment = harness.run("new", "target", max_cost=0.5)
            state = json.loads((experiment / "state.json").read_text())
            self.assertEqual(state["status"], "budget_exhausted")
            self.assertEqual(len(state["records"]), 1)
            harness.run("new", "target", resume=experiment, max_cost=3.0)
            state = json.loads((experiment / "state.json").read_text())
            self.assertEqual(state["status"], "complete")
            self.assertEqual(len(state["records"]), 2)
            self.assertEqual(fake.calls, 3)


class StatsTests(unittest.TestCase):
    def test_aggregates_model_stats(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "adapter.py").write_text(
                "class A:\n root='.'\n def finalize(self): pass\n"
                "def create_adapter(root, settings): return A()\n",
                encoding="utf-8",
            )
            (root / "config.json").write_text(json.dumps({
                "project_adapter": "adapter.py",
                "output_dir": "out",
                "models": [{"id": "a", "provider": "opencode", "model": "p/a"}],
            }), encoding="utf-8")
            out = root / "out"
            out.mkdir()
            rows = [
                {"model_id": "a", "duration_seconds": 2, "input_tokens": 10, "output_tokens": 4,
                 "cost": 0.1, "winner": True, "evaluation": {"accepted": True, "match_percent": 100}},
                {"model_id": "a", "duration_seconds": 4, "input_tokens": 20, "output_tokens": 6,
                 "cost": 0.2, "winner": False, "evaluation": {"accepted": False, "match_percent": 80}},
            ]
            (out / "experiments.jsonl").write_text(
                "".join(json.dumps(row) + "\n" for row in rows), encoding="utf-8"
            )
            stats = Harness(root / "config.json").stats()[0]
            self.assertEqual(stats["wins"], 1)
            self.assertEqual(stats["accepted_wins"], 1)
            self.assertEqual(stats["accepted"], 1)
            self.assertEqual(stats["average_match_percent"], 90)
            self.assertEqual(stats["average_seconds"], 3)
            self.assertAlmostEqual(stats["total_cost"], 0.3)
            self.assertAlmostEqual(stats["average_cost"], 0.15)


if __name__ == "__main__":
    unittest.main()
