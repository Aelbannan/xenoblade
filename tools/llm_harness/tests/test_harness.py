from __future__ import annotations

import json
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace

from tools.llm_harness.core import Harness, parse_candidate
from tools.llm_harness.providers import parse_opencode_output
from tools.llm_harness.xenoblade_project import _knowledge_queries, _knowledge_record


class CandidateTests(unittest.TestCase):
    def test_parses_fenced_typed_candidate(self) -> None:
        candidate = parse_candidate(
            '```json\n{"full_source":"int f() { return 1; }","notes":"one","confidence":0.8}\n```'
        )
        self.assertEqual(candidate.notes, ["one"])
        self.assertEqual(candidate.confidence, 0.8)

    def test_rejects_missing_source(self) -> None:
        with self.assertRaises(ValueError):
            parse_candidate('{"hypothesis":"x"}')


class OpenCodeOutputTests(unittest.TestCase):
    def test_parses_text_events_and_usage(self) -> None:
        output = "\n".join([
            json.dumps({"type": "text", "part": {"type": "text", "text": "{\"full_"}}),
            json.dumps({
                "type": "text",
                "part": {"type": "text", "text": "source\":\"x\"}"},
                "usage": {"input_tokens": 12, "output_tokens": 4},
            }),
        ])
        text, events, usage = parse_opencode_output(output)
        self.assertEqual(text, '{"full_source":"x"}')
        self.assertEqual(len(events), 2)
        self.assertEqual(usage["input"], 12)
        self.assertEqual(usage["output"], 4)


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


if __name__ == "__main__":
    unittest.main()
