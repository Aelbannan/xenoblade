#!/usr/bin/env python3
"""Unit tests for the paseo-harness 3-way merge drivers."""
import json
import os
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

HARNESS = Path(__file__).resolve().parent.parent
VENV = Path(HARNESS).parent.parent / ".venv" / "bin" / "python3"
if not VENV.exists():
    VENV = Path(sys.executable)


def run_driver(name, base, ours, theirs):
    """Write base/ours/theirs as files and run a merge driver; return merged text."""
    script = HARNESS / f"merge_{name}.py"
    with tempfile.TemporaryDirectory() as td:
        b, o, t = (Path(td) / "b", Path(td) / "o", Path(td) / "t")
        if isinstance(base, str):
            b.write_text(base)
            o.write_text(ours)
            t.write_text(theirs)
            ours, theirs = str(o), str(t)
            base = str(b)
        else:
            b.write_text(json.dumps(base))
            o.write_text(json.dumps(ours))
            t.write_text(json.dumps(theirs))
        r = subprocess.run([str(VENV), str(script), str(b), str(o), str(t)],
                           capture_output=True, text=True)
        assert r.returncode == 0, r.stderr
        return o.read_text()


class TestMergeTargets(unittest.TestCase):
    def test_3way_per_target_union(self):
        base = {"targets": [{"id": "A", "status": "ACTIVE", "v": 1}]}
        ours = {"targets": [{"id": "A", "status": "ACTIVE", "v": 2, "ours": 1},
                            {"id": "B", "status": "ACTIVE", "v": 9}]}
        theirs = {"targets": [{"id": "A", "status": "FULL_MATCH", "v": 3, "theirs": 1},
                              {"id": "C", "status": "ACCEPTED"}]}
        out = json.loads(run_driver("targets", base, ours, theirs))
        ids = [t["id"] for t in out["targets"]]
        self.assertEqual(ids, ["A", "B", "C"])
        a = [t for t in out["targets"] if t["id"] == "A"][0]
        self.assertEqual(a["status"], "FULL_MATCH")   # theirs won (changed vs base)
        self.assertIn("theirs", a)
        self.assertNotIn("ours", a)

    def test_ours_wins_when_theirs_unchanged(self):
        base = {"targets": [{"id": "A", "status": "ACTIVE"}]}
        ours = {"targets": [{"id": "A", "status": "FULL_MATCH", "ours": 1}]}
        theirs = {"targets": [{"id": "A", "status": "ACTIVE"}]}
        out = json.loads(run_driver("targets", base, ours, theirs))
        a = out["targets"][0]
        self.assertEqual(a["status"], "FULL_MATCH")
        self.assertIn("ours", a)


class TestMergeAttempts(unittest.TestCase):
    def test_union_dedup(self):
        out = run_driver("attempts", "l1\nl2\n", "l1\nl2\nlo\n", "l1\nl2\nlt\nl2\n")
        self.assertEqual(out.splitlines(), ["l1", "l2", "lo", "lt"])


class TestMergeSymbols(unittest.TestCase):
    def test_keyed_3way(self):
        base = "alpha = .text:0x100\n"
        ours = "alpha = .text:0x100\nbeta = .text:0x200\n"
        theirs = "alpha = .text:0x999\nbeta = .text:0x200\ngamma = .text:0x300\n"
        out = run_driver("symbols", base, ours, theirs)
        self.assertIn("alpha = .text:0x999", out)   # theirs changed vs base
        self.assertIn("beta = .text:0x200", out)    # unchanged, kept
        self.assertIn("gamma = .text:0x300", out)   # new in theirs


if __name__ == "__main__":
    unittest.main()
