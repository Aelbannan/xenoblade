"""Tests for the non-LLM ppc_equivalence probe workflow."""
from __future__ import annotations

import json
import tempfile
import unittest
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any, Dict, List, Optional
from unittest.mock import patch

from tools.llm_harness.core import Harness
from tools.llm_harness.run import main
from tools.llm_harness.xenoblade_project import PROBE_COMPILE_STATUSES


@dataclass
class FakeProbeAdapter:
    root: Path
    probe_rows: Dict[str, Dict[str, Any]] = field(default_factory=dict)
    selected: List[str] = field(default_factory=list)
    probe_calls: List[Dict[str, Any]] = field(default_factory=list)

    def finalize(self) -> None:
        return None

    def select_targets(
        self,
        workflow: str,
        number: int,
        *,
        randomize: bool = False,
        certified_funcs: bool = False,
        tu: Optional[str] = None,
        selection: Optional[str] = None,
        min_fuzzy: Optional[float] = None,
    ) -> List[str]:
        assert workflow == "probe"
        ids = self.selected[:number]
        if len(ids) < number:
            raise ValueError(f"Only {len(ids)} eligible probe targets")
        return ids

    def describe_frontier(self, target_ids: List[str]) -> List[Dict[str, str]]:
        return [{"id": tid, "kind": "leaf"} for tid in target_ids]

    def probe_target(
        self,
        target_id: str,
        *,
        write: bool = False,
        linked: bool = False,
        rebuild: bool = True,
    ) -> Dict[str, Any]:
        self.probe_calls.append(
            {
                "target_id": target_id,
                "write": write,
                "linked": linked,
                "rebuild": rebuild,
            }
        )
        row = dict(self.probe_rows.get(target_id) or {})
        row.setdefault("target_id", target_id)
        row.setdefault("status", "CODE_MATCH")
        row.setdefault("match_percent", 80.0)
        row.setdefault("accepted", False)
        row.setdefault("equivalence", "NOT_EQUIVALENT")
        row.setdefault("detail", "")
        row["written"] = bool(write and row.get("accepted"))
        return row


def _write_harness(
    root: Path, adapter: FakeProbeAdapter
) -> Harness:
    (root / "adapter.py").write_text(
        "def create_adapter(root, settings):\n"
        "    raise RuntimeError('patched')\n",
        encoding="utf-8",
    )
    config = {
        "project_adapter": "adapter.py",
        "output_dir": "out",
        "models": {
            "default": [{"id": "m", "provider": "fake", "model": "x", "runs": 1}],
        },
        "providers": {"fake": {}},
        "execution": {"isolation": {"mode": "none"}, "auto_promote": False},
        "solve": {},
        "prompt": {},
    }
    path = root / "config.json"
    path.write_text(json.dumps(config), encoding="utf-8")
    with patch("tools.llm_harness.core.Harness._load_adapter", return_value=adapter):
        harness = Harness(path)
    harness.adapter = adapter
    harness.output_dir = root / "out"
    harness.output_dir.mkdir(parents=True, exist_ok=True)
    return harness


class ProbeWorkflowTests(unittest.TestCase):
    def test_probe_compile_statuses(self) -> None:
        self.assertIn("COMPILES", PROBE_COMPILE_STATUSES)
        self.assertIn("CODE_MATCH", PROBE_COMPILE_STATUSES)
        self.assertNotIn("FULL_MATCH", PROBE_COMPILE_STATUSES)
        self.assertNotIn("NOT_STARTED", PROBE_COMPILE_STATUSES)

    def test_run_probe_dry_run(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            adapter = FakeProbeAdapter(
                root=root,
                selected=["a", "b"],
                probe_rows={
                    "a": {
                        "status": "EQUIVALENT_MATCH",
                        "match_percent": 80.0,
                        "accepted": True,
                        "equivalence": "EQUIVALENT",
                    },
                    "b": {
                        "status": "CODE_MATCH",
                        "match_percent": 55.0,
                        "accepted": False,
                        "equivalence": "NOT_EQUIVALENT",
                    },
                },
            )
            harness = _write_harness(root, adapter)
            # dry_run still probes, but never persists even if write=True.
            batch = harness.run_probe(["a", "b"], dry_run=True, write=True)
            summary = json.loads((batch / "summary.json").read_text(encoding="utf-8"))
            self.assertTrue(summary["dry_run"])
            self.assertFalse(summary["write"])
            self.assertEqual(summary["count"], 2)
            self.assertEqual(summary["accepted"], 1)
            self.assertEqual(len(adapter.probe_calls), 2)
            self.assertFalse(adapter.probe_calls[0]["write"])
            self.assertFalse(adapter.probe_calls[1]["write"])
            results_path = batch / "results.jsonl"
            self.assertTrue(results_path.is_file())
            lines = results_path.read_text(encoding="utf-8").strip().splitlines()
            self.assertEqual(len(lines), 2)
            first = json.loads(lines[0])
            self.assertEqual(first["status"], "EQUIVALENT_MATCH")
            self.assertEqual(first["target_id"], "a")
            self.assertFalse(first["written"])

    def test_run_probe_writes_results(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            adapter = FakeProbeAdapter(
                root=root,
                probe_rows={
                    "win": {
                        "status": "EQUIVALENT_MATCH",
                        "match_percent": 72.0,
                        "accepted": True,
                        "equivalence": "EQUIVALENT",
                    },
                    "lose": {
                        "status": "CODE_MATCH",
                        "match_percent": 60.0,
                        "accepted": False,
                        "equivalence": "NOT_EQUIVALENT",
                    },
                },
            )
            harness = _write_harness(root, adapter)
            batch = harness.run_probe(["win", "lose"], write=True, rebuild=False)
            summary = json.loads((batch / "summary.json").read_text(encoding="utf-8"))
            self.assertEqual(summary["accepted"], 1)
            self.assertEqual(summary["errors"], 0)
            self.assertTrue(summary["write"])
            lines = (batch / "results.jsonl").read_text(encoding="utf-8").strip().splitlines()
            self.assertEqual(len(lines), 2)
            self.assertEqual(adapter.probe_calls[0]["write"], True)
            self.assertEqual(adapter.probe_calls[0]["rebuild"], False)

    def test_cli_probe_number_dry_run(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            adapter = FakeProbeAdapter(
                root=root,
                selected=["t1", "t2", "t3"],
                probe_rows={
                    "t1": {"status": "CODE_MATCH", "match_percent": 60.0},
                    "t2": {"status": "CODE_MATCH", "match_percent": 70.0},
                },
            )
            (root / "adapter.py").write_text(
                "def create_adapter(root, settings):\n"
                "    raise RuntimeError('patched')\n",
                encoding="utf-8",
            )
            config = {
                "project_adapter": "adapter.py",
                "output_dir": "out",
                "models": {
                    "default": [{"id": "m", "provider": "fake", "model": "x", "runs": 1}],
                },
                "providers": {"fake": {}},
                "execution": {"isolation": {"mode": "none"}, "auto_promote": False},
                "solve": {},
                "prompt": {},
            }
            cfg = root / "config.json"
            cfg.write_text(json.dumps(config), encoding="utf-8")

            def _factory(path: Path) -> Harness:
                with patch(
                    "tools.llm_harness.core.Harness._load_adapter",
                    return_value=adapter,
                ):
                    harness = Harness(path)
                harness.adapter = adapter
                harness.output_dir = root / "out"
                harness.output_dir.mkdir(parents=True, exist_ok=True)
                return harness

            with patch("tools.llm_harness.run.Harness", side_effect=_factory):
                code = main(
                    [
                        "--config",
                        str(cfg),
                        "probe",
                        "--number",
                        "2",
                        "--dry-run",
                    ]
                )
            self.assertEqual(code, 0)
            self.assertEqual(len(adapter.probe_calls), 2)
            self.assertFalse(adapter.probe_calls[0]["write"])


if __name__ == "__main__":
    unittest.main()
