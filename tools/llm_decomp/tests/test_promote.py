"""Tests for tools/llm_decomp/promote.py."""
from __future__ import annotations

import subprocess
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest import mock

from tools.llm_decomp.config import Config
from tools.llm_decomp.contracts import SessionMeta, Verdict, VerdictKind
from tools.llm_decomp.promote import (PromotionError, PromotionQueue,
                                      _LOCK)

META = SessionMeta(target_id="us-1", session_id="s1", session_type="match",
                   unit="kyoshin/X", symbol="sym", status="DONE",
                   writable=["src/kyoshin/X.cpp"], owner="t",
                   created_at="now")


def _verdict(rule: str) -> Verdict:
    return Verdict(kind=VerdictKind.ACCEPTED, accepted=True,
                   target_symbol="sym", rule=rule, target_fuzzy=100.0)


def _cp(rc: int, out: str = "") -> subprocess.CompletedProcess:
    return subprocess.CompletedProcess([], rc, out, "")


class CheckpointTest(unittest.TestCase):
    def test_disabled(self) -> None:
        pq = PromotionQueue(Path("."), Config(checkpoint_commits=False))
        with mock.patch("tools.llm_decomp.promote._git") as g:
            self.assertFalse(pq.checkpoint("msg", ["x"]))
            g.assert_not_called()

    def test_success(self) -> None:
        pq = PromotionQueue(Path("."), Config(checkpoint_commits=True))
        with mock.patch("tools.llm_decomp.promote._git",
                        side_effect=[_cp(0), _cp(0)]) as g:
            self.assertTrue(pq.checkpoint("msg", ["a", "b"]))
            self.assertEqual(g.call_args_list[0].args[1:],
                             ("add", "--", "a", "b"))

    def test_nothing_to_commit(self) -> None:
        pq = PromotionQueue(Path("."), Config(checkpoint_commits=True))
        with mock.patch("tools.llm_decomp.promote._git",
                        side_effect=[_cp(0), _cp(1, "nothing to commit")]):
            self.assertFalse(pq.checkpoint("msg", ["a"]))

    def test_commit_error_raises(self) -> None:
        pq = PromotionQueue(Path("."), Config(checkpoint_commits=True))
        with mock.patch("tools.llm_decomp.promote._git",
                        side_effect=[_cp(0), _cp(1, "fatal: boom")]):
            self.assertRaises(PromotionError, pq.checkpoint, "msg", ["a"])


class PromoteAcceptedTest(unittest.TestCase):
    def _run(self, rule: str) -> dict:
        pq = PromotionQueue(Path("."), Config(checkpoint_commits=True))
        with mock.patch("tools.coop.lib.targets.update_target_result") as u, \
             mock.patch("tools.coop.lib.config.load_config",
                        return_value=SimpleNamespace(region="us")), \
             mock.patch("tools.llm_decomp.promote.Ledger"), \
             mock.patch.object(PromotionQueue, "checkpoint",
                               return_value=True):
            out = pq.promote_accepted(META, _verdict(rule))
        return out, u.call_args.kwargs["status"]

    def test_full_match_tier(self) -> None:
        out, status = self._run("full_match")
        self.assertEqual(status, "FULL_MATCH")
        self.assertEqual(out["tier"], "FULL_MATCH")
        self.assertTrue(out["committed"])

    def test_equivalent_match_tier(self) -> None:
        out, status = self._run("equivalent_match")
        self.assertEqual(status, "EQUIVALENT_MATCH")
        self.assertEqual(out["tier"], "EQUIVALENT_MATCH")


class MarkUnitMatchingTest(unittest.TestCase):
    def test_flip(self) -> None:
        with tempfile.TemporaryDirectory() as td:
            root = Path(td)
            (root / "configure.py").write_text(
                'Object(NonMatching, "kyoshin/cf/CfPadTask.cpp"),\n',
                encoding="utf-8")
            pq = PromotionQueue(root, Config())
            with mock.patch("tools.llm_decomp.promote.subprocess.run",
                            return_value=_cp(0)), \
                 mock.patch("tools.llm_decomp.promote.BuildLock"), \
                 mock.patch("tools.coop.lib.config.load_config",
                            return_value=SimpleNamespace(region="us")):
                self.assertTrue(pq.mark_unit_matching("kyoshin/cf/CfPadTask"))
            self.assertIn("Object(Matching,",
                          (root / "configure.py").read_text())

    def test_absent(self) -> None:
        with tempfile.TemporaryDirectory() as td:
            root = Path(td)
            (root / "configure.py").write_text("# empty\n",
                                               encoding="utf-8")
            pq = PromotionQueue(root, Config())
            self.assertFalse(pq.mark_unit_matching("kyoshin/cf/CfPadTask"))


if __name__ == "__main__":
    unittest.main()
