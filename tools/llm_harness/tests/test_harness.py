from __future__ import annotations

import json
import subprocess
import tempfile
import threading
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
    _assembly_function_block,
    _binary_mismatch_summary,
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

    def test_parses_cache_usage(self) -> None:
        output = json.dumps({
            "type": "step_finish",
            "part": {
                "type": "step-finish",
                "tokens": {"input": 10, "output": 2, "cache": {"read": 8, "write": 3}},
            },
        })
        _, _, usage = parse_opencode_output(output)
        self.assertEqual(usage["cache_read"], 8)
        self.assertEqual(usage["cache_write"], 3)

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

    def test_extracts_only_named_retail_assembly_block(self) -> None:
        source = ".fn first, global\na\n.endfn first\n.fn second, global\nb\n.endfn second\n"
        self.assertEqual(
            _assembly_function_block(source, "second"),
            ".fn second, global\nb\n.endfn second\n",
        )


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
    def test_uses_one_self_contained_prompt_without_context_files(self) -> None:
        adapter = object.__new__(XenobladeAdapter)
        self.assertEqual(adapter.model_context_mode("tu-complete"), "inline")
        self.assertEqual(
            adapter.build_context_files("tu-complete", "unit", [], "prompt"), {}
        )

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

    def test_binary_summary_bounds_instruction_and_relocation_deltas(self) -> None:
        relocation = lambda offset, symbol: SimpleNamespace(
            offset=offset, relocation_type=10, symbol=symbol, addend=0
        )
        retail = SimpleNamespace(
            code=bytes.fromhex("60000000 38600001 4e800020"),
            relocations=(relocation(4, "retail_symbol"),),
        )
        candidate = SimpleNamespace(
            code=bytes.fromhex("60000000 38600002 60000000 4e800020"),
            relocations=(relocation(4, "candidate_symbol"),),
        )

        summary = _binary_mismatch_summary(retail, candidate)

        self.assertIn("binary mismatch summary", summary)
        self.assertIn("retail +0x4", summary)
        self.assertIn("candidate-only", summary)
        self.assertIn("retail_symbol", summary)
        self.assertLessEqual(len(summary), 3000)


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
                " def evaluate(self, workflow, target, candidate): return Evaluation('HIGH_MATCH',80,False)\n"
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
                    self.last_cwd = None

                def invoke(self, prompt, model, cwd):
                    self.calls += 1
                    self.last_cwd = cwd
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
            self.assertEqual(fake.last_cwd.name, "context")
            self.assertTrue((fake.last_cwd / "TASK.md").is_file())

    def test_full_match_stops_remaining_agents(self) -> None:
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
                "execution": {"max_retries": 0},
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
                    return ProviderResult(
                        text='{"source":"int f() { return 1; }","hypothesis":"x"}',
                        duration_seconds=1,
                    )

            harness = Harness(root / "config.json")
            fake = FakeProvider()
            harness.providers["fake"] = fake
            experiment = harness.run("new", "target")
            state = json.loads((experiment / "state.json").read_text())
            self.assertEqual(state["status"], "complete")
            self.assertEqual(len(state["records"]), 1)
            self.assertEqual(fake.calls, 1)
            self.assertEqual(state["records"][0]["evaluation"]["status"], "FULL_MATCH")

    def test_parallel_full_match_logs_already_running_non_winners(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "adapter.py").write_text(
                "from tools.llm_harness.types import Evaluation\n"
                "class A:\n"
                " def __init__(self, root): self.root=root\n"
                " def build_prompt(self, workflow, target, history): return 'prompt'\n"
                " def evaluate(self, workflow, target, candidate):\n"
                "  return Evaluation('FULL_MATCH',100,True) if 'return 1' in candidate.source else Evaluation('HIGH_MATCH',80,False)\n"
                " def finalize(self): pass\n"
                "def create_adapter(root, settings): return A(root)\n",
                encoding="utf-8",
            )
            (root / "config.json").write_text(json.dumps({
                "project_adapter": "adapter.py",
                "output_dir": "out",
                "execution": {"max_retries": 0, "max_parallel": 2},
                "models": [
                    {"id": "a", "provider": "fake", "model": "p/a"},
                    {"id": "b", "provider": "fake", "model": "p/b"},
                ],
            }), encoding="utf-8")

            class FakeProvider:
                def __init__(self) -> None:
                    self.barrier = threading.Barrier(2)

                def invoke(self, prompt, model, cwd):
                    self.barrier.wait(timeout=2)
                    value = 1 if model.id == "a" else 2
                    return ProviderResult(
                        text=(
                            '{"source":"int f() { return '
                            f'{value}; }}","hypothesis":"x"}}'
                        ),
                        duration_seconds=1,
                    )

            harness = Harness(root / "config.json")
            harness.providers["fake"] = FakeProvider()
            experiment = harness.run("new", "target", max_parallel=2)
            state = json.loads((experiment / "state.json").read_text())
            rows = [
                json.loads(line)
                for line in (root / "out" / "experiments.jsonl").read_text().splitlines()
            ]
            self.assertEqual(len(state["records"]), 2)
            self.assertEqual(len(rows), 2)
            self.assertEqual(sum(bool(row["winner"]) for row in rows), 1)
            self.assertEqual(
                {row["evaluation"]["status"] for row in rows},
                {"FULL_MATCH", "HIGH_MATCH"},
            )


class BatchTests(unittest.TestCase):
    def test_dry_run_creates_one_context_per_target(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "adapter.py").write_text(
                "class A:\n"
                " def __init__(self, root): self.root=root\n"
                " def build_prompt(self, workflow, target, history): return 'prompt '+target\n"
                " def evaluate(self, workflow, target, candidate): raise AssertionError()\n"
                " def finalize(self): pass\n"
                "def create_adapter(root, settings): return A(root)\n",
                encoding="utf-8",
            )
            (root / "config.json").write_text(json.dumps({
                "project_adapter": "adapter.py",
                "output_dir": "out",
                "execution": {"max_target_parallel": 2},
                "models": [{"id": "a", "provider": "opencode", "model": "p/a"}],
            }), encoding="utf-8")
            harness = Harness(root / "config.json")
            batch = harness.run_batch("new", ["one", "two"], dry_run=True)
            manifest = json.loads((batch / "batch.json").read_text())
            self.assertEqual(manifest["status"], "complete")
            self.assertEqual(set(manifest["targets"]), {"one", "two"})
            for row in manifest["targets"].values():
                context = root / row["experiment"] / "context"
                self.assertTrue((context / "TASK.md").is_file())

    def test_inline_context_omits_duplicate_task(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / "adapter.py").write_text(
                "class A:\n"
                " def __init__(self, root): self.root=root\n"
                " def build_prompt(self, workflow, target, history): return 'prompt '+target\n"
                " def model_context_mode(self, workflow): return 'inline'\n"
                " def evaluate(self, workflow, target, candidate): raise AssertionError()\n"
                " def finalize(self): pass\n"
                "def create_adapter(root, settings): return A(root)\n",
                encoding="utf-8",
            )
            (root / "config.json").write_text(json.dumps({
                "project_adapter": "adapter.py",
                "output_dir": "out",
                "models": [{"id": "a", "provider": "opencode", "model": "p/a"}],
            }), encoding="utf-8")
            harness = Harness(root / "config.json")
            experiment = harness.run("new", "one", dry_run=True)
            self.assertEqual(list((experiment / "context").iterdir()), [])
            state = json.loads((experiment / "state.json").read_text())
            self.assertEqual(state["context_mode"], "inline")


class WorkflowModelTests(unittest.TestCase):
    @staticmethod
    def _write_adapter(root: Path) -> None:
        (root / "adapter.py").write_text(
            "class A:\n"
            " def __init__(self, root): self.root=root\n"
            " def finalize(self): pass\n"
            "def create_adapter(root, settings): return A(root)\n",
            encoding="utf-8",
        )

    def test_selects_exact_pipeline_then_default(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            self._write_adapter(root)
            (root / "config.json").write_text(json.dumps({
                "project_adapter": "adapter.py",
                "models": {
                    "default": [
                        {"id": "general", "provider": "opencode", "model": "p/general"}
                    ],
                    "new": [
                        {"id": "fast", "provider": "opencode", "model": "p/fast"}
                    ],
                },
            }), encoding="utf-8")
            harness = Harness(root / "config.json")
            self.assertEqual(
                [model.id for model in harness.models_for_workflow("new")], ["fast"]
            )
            self.assertEqual(
                [model.id for model in harness.models_for_workflow("improve")],
                ["general"],
            )

    def test_pipeline_without_exact_or_default_is_rejected_on_use(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            self._write_adapter(root)
            (root / "config.json").write_text(json.dumps({
                "project_adapter": "adapter.py",
                "models": {
                    "new": [
                        {"id": "fast", "provider": "opencode", "model": "p/fast"}
                    ]
                },
            }), encoding="utf-8")
            harness = Harness(root / "config.json")
            with self.assertRaisesRegex(ValueError, "models.improve or models.default"):
                harness.models_for_workflow("improve")

    def test_legacy_flat_model_list_remains_supported(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            self._write_adapter(root)
            (root / "config.json").write_text(json.dumps({
                "project_adapter": "adapter.py",
                "models": [
                    {"id": "legacy", "provider": "opencode", "model": "p/legacy"}
                ],
            }), encoding="utf-8")
            harness = Harness(root / "config.json")
            self.assertEqual(
                [model.id for model in harness.models_for_workflow("tu-complete")],
                ["legacy"],
            )


class WorkflowExecutionTests(unittest.TestCase):
    @staticmethod
    def _write_config(root: Path, execution: dict) -> Harness:
        (root / "adapter.py").write_text(
            "class A:\n"
            " def __init__(self, root): self.root=root\n"
            " def finalize(self): pass\n"
            "def create_adapter(root, settings): return A(root)\n",
            encoding="utf-8",
        )
        (root / "config.json").write_text(json.dumps({
            "project_adapter": "adapter.py",
            "execution": execution,
            "models": [
                {"id": "model", "provider": "opencode", "model": "p/model"}
            ],
        }), encoding="utf-8")
        return Harness(root / "config.json")

    def test_pipeline_parallelism_overrides_global_values(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            harness = self._write_config(Path(tmp), {
                "max_parallel": 2,
                "max_target_parallel": 3,
                "batch_model_parallel": 4,
                "pipelines": {
                    "new": {"max_parallel": 5, "max_target_parallel": 6},
                    "improve": {"batch_model_parallel": 1},
                },
            })
            self.assertEqual(harness.parallelism_for_workflow("new", "max_parallel"), 5)
            self.assertEqual(
                harness.parallelism_for_workflow("new", "max_target_parallel"), 6
            )
            self.assertEqual(
                harness.parallelism_for_workflow("new", "batch_model_parallel"), 4
            )
            self.assertEqual(
                harness.parallelism_for_workflow("improve", "max_parallel"), 2
            )
            self.assertEqual(
                harness.parallelism_for_workflow("improve", "batch_model_parallel"), 1
            )

    def test_rejects_unknown_pipeline_execution_option(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            with self.assertRaisesRegex(ValueError, "Unknown execution option"):
                self._write_config(Path(tmp), {
                    "pipelines": {"new": {"concurrent": 2}}
                })

    def test_rejects_nonpositive_pipeline_parallelism(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            with self.assertRaisesRegex(ValueError, "values must be positive"):
                self._write_config(Path(tmp), {
                    "pipelines": {"tu-complete": {"max_parallel": 0}}
                })


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
                 "cache_read_tokens": 8, "cache_write_tokens": 3,
                 "cost": 0.1, "winner": True, "evaluation": {"accepted": True, "match_percent": 100}},
                {"model_id": "a", "duration_seconds": 4, "input_tokens": 20, "output_tokens": 6,
                 "cost": 0.2, "winner": False, "evaluation": {"accepted": False, "match_percent": 80}},
                {"model_id": "a", "duration_seconds": 180, "input_tokens": 0, "output_tokens": 0,
                 "cost": None, "winner": False, "timed_out": True,
                 "error": "TimeoutExpired: model exceeded 180 seconds", "evaluation": {}},
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
            self.assertEqual(stats["timeouts"], 1)
            self.assertEqual(stats["errors"], 1)
            self.assertAlmostEqual(stats["total_cost"], 0.3)
            self.assertAlmostEqual(stats["average_cost"], 0.15)
            self.assertEqual(stats["cache_read_tokens"], 8)
            self.assertEqual(stats["cache_write_tokens"], 3)


if __name__ == "__main__":
    unittest.main()
