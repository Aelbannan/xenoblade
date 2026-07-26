"""Session-loop tests with a scripted fake provider (no toolchain, no LLM).

verify.* and the hexdiff subprocess are monkeypatched; the tests exercise
the state machine: budgets, patch/lint feedback, submit checkpoints,
acceptance -> cleanup -> DONE, and failure restore.
"""
from __future__ import annotations

import tempfile
import unittest
from pathlib import Path
from unittest import mock

from tools.llm_decomp import paths, state
from tools.llm_decomp.config import Config, SessionTypeConfig
from tools.llm_decomp.contracts import (SessionMeta, SessionStatus, ToolCall,
                                        Verdict, VerdictKind)
from tools.llm_decomp.policies import get_policy
from tools.llm_decomp.providers.base import ProviderReply
from tools.llm_decomp.session import Session

SRC = "src/kyoshin/X.cpp"
HDR = "include/kyoshin/X.hpp"
SRC_TEXT = '// stub\nextern "C" u8 func_80000000(void* s);\n'


class FakeProvider:
    """Plays back a script: list of ProviderReply, one per turn."""

    def __init__(self, script: list[ProviderReply]):
        self.script = list(script)
        self.calls = 0

    def send(self, messages, tools, model) -> ProviderReply:
        self.calls += 1
        if not self.script:
            return ProviderReply(text="(no script)", tool_calls=[],
                                 usage={"input_tokens": 1},
                                 finish_reason="stop")
        return self.script.pop(0)


def reply(*calls: ToolCall, text: str = "") -> ProviderReply:
    return ProviderReply(text=text, tool_calls=list(calls),
                         usage={"input_tokens": 10, "output_tokens": 5},
                         finish_reason="tool_calls" if calls else "stop")


def verdict(kind: VerdictKind, accepted: bool, rule: str,
            mismatches: int | None = 3) -> Verdict:
    return Verdict(kind=kind, accepted=accepted, target_symbol="sym",
                   target_mismatches=mismatches, rule=rule, text_size=100,
                   text_budget=200)


class SessionTestBase(unittest.TestCase):
    def setUp(self) -> None:
        self._tmp = tempfile.TemporaryDirectory()
        self.root = Path(self._tmp.name)
        (self.root / SRC).parent.mkdir(parents=True)
        (self.root / SRC).write_text(SRC_TEXT, encoding="utf-8")
        self.cfg = Config()
        self.policy = get_policy("match")
        self.meta = SessionMeta(
            target_id="us-80000000", session_id="test00000001",
            session_type="match", unit="kyoshin/X", symbol="sym",
            status=SessionStatus.INIT.value, writable=[SRC, HDR],
            owner="test", created_at="now")
        self.sdir = paths.session_dir(self.root, self.meta.target_id,
                                      self.meta.session_id)
        paths.init_snapshot_dir(self.sdir).mkdir(parents=True)
        (paths.init_snapshot_dir(self.sdir) / SRC).parent.mkdir(
            parents=True, exist_ok=True)
        (paths.init_snapshot_dir(self.sdir) / SRC).write_text(
            SRC_TEXT, encoding="utf-8")
        state.save_meta(self.sdir, self.meta)

    def tearDown(self) -> None:
        self._tmp.cleanup()

    def set_budgets(self, **overrides) -> None:
        budgets = dict(self.cfg.budgets_for("match"))
        budgets.update(overrides)
        self.cfg.session_types["match"] = SessionTypeConfig(
            model=self.cfg.default_model, budgets=budgets)

    def make_session(self, provider: FakeProvider) -> Session:
        return Session(self.root, self.cfg, self.policy, self.meta,
                       self.sdir, provider,
                       demangled="func_80000000", signature="u8 f(void*)",
                       retail_asm="lwz r3, 0(r3)\nblr",
                       target_id=self.meta.target_id)

    def patched(self, baseline=True, **verify_kwargs):
        """Context manager patching verify + hexdiff subprocess."""
        mocks = {
            "capture_baseline": mock.DEFAULT if baseline else None,
            "verdict_for_submit": mock.DEFAULT,
            "build_unit": mock.DEFAULT,
        }
        return mock.patch.multiple(
            "tools.llm_decomp.session.verify",
            capture_baseline=mock.Mock(return_value=None),
            **verify_kwargs)


class TestSessionLoop(SessionTestBase):
    def test_patch_then_submit_accepted(self) -> None:
        provider = FakeProvider([
            reply(ToolCall("patch", {"files": [{
                "path": SRC,
                "blocks": [{"search": '// stub', "replace": "// matched"}],
            }]}, id="c1")),
            reply(ToolCall("submit", {"note": "done"}, id="c2")),
            reply(),  # cleanup turn: no tools
        ])
        with self.patched(verdict_for_submit=mock.Mock(
                return_value=verdict(VerdictKind.ACCEPTED, True,
                                     "full_match", 0))):
            outcome = self.make_session(provider).run()
        self.assertTrue(outcome.accepted)
        self.assertIn("// matched",
                      (self.root / SRC).read_text(encoding="utf-8"))
        # accepted snapshot recorded
        self.assertTrue((paths.accepted_snapshot_dir(self.sdir) / SRC)
                        .exists())

    def test_failed_submit_continues_then_budget(self) -> None:
        provider = FakeProvider([
            reply(ToolCall("submit", {}, id="c1")),
        ])
        soft = verdict(VerdictKind.SOFT_REJECT, False, "not_matched", 7)
        self.set_budgets(max_turns=3)
        with self.patched(verdict_for_submit=mock.Mock(return_value=soft)):
            outcome = self.make_session(provider).run()
        self.assertFalse(outcome.accepted)
        # scripted replies run out -> consecutive no-tool turns end the loop
        self.assertEqual(outcome.reason, "no_progress")
        # failed session restores init state
        self.assertEqual((self.root / SRC).read_text(encoding="utf-8"),
                         SRC_TEXT)

    def test_lint_violation_reverts_patch(self) -> None:
        provider = FakeProvider([
            reply(ToolCall("patch", {"files": [{
                "path": SRC,
                "blocks": [{"search": "// stub",
                            "replace": 'asm void hack() { }'}],
            }]}, id="c1")),
        ])
        self.set_budgets(max_turns=2)
        with self.patched(verdict_for_submit=mock.Mock()):
            outcome = self.make_session(provider).run()
        self.assertFalse(outcome.accepted)
        self.assertEqual((self.root / SRC).read_text(encoding="utf-8"),
                         SRC_TEXT)
        # lint rejects are not anchoring failures
        self.assertEqual(outcome.reason, "budget_exhausted")

    def test_bad_patch_counts_toward_failure_streak(self) -> None:
        bad = reply(ToolCall("patch", {"files": [{
            "path": SRC,
            "blocks": [{"search": "no such text", "replace": "x"}],
        }]}, id="c1"))
        provider = FakeProvider([bad] * 5)
        with self.patched(verdict_for_submit=mock.Mock()):
            outcome = self.make_session(provider).run()
        self.assertEqual(outcome.reason, "patch_failure_streak")

    def test_cleanup_break_reverts_to_accepted(self) -> None:
        accepted = verdict(VerdictKind.ACCEPTED, True, "full_match", 0)
        broken = Verdict(kind=VerdictKind.HARD_REJECT, accepted=False,
                         target_symbol="sym", rule="sibling_regression",
                         text_size=100, text_budget=200)
        provider = FakeProvider([
            reply(ToolCall("submit", {}, id="c1")),          # -> accepted
            reply(ToolCall("patch", {"files": [{
                "path": SRC,
                "blocks": [{"search": "// stub", "replace": "// broken"}],
            }]}, id="c2")),                                   # cleanup edit
        ])
        verdicts = mock.Mock(side_effect=[accepted, broken])
        with self.patched(verdict_for_submit=verdicts):
            outcome = self.make_session(provider).run()
        # After the cleanup break, tree must hold the ACCEPTED state.
        self.assertTrue(outcome.accepted)
        self.assertEqual((self.root / SRC).read_text(encoding="utf-8"),
                         SRC_TEXT)


class FakeGraph:
    def __init__(self, deps: set[str]):
        self._deps = deps

    def dependents(self, path: str) -> set:
        return set(self._deps)

    def snapshot_id(self) -> str:
        return "fake0000"


SHARED_HDR = "include/shared/Hdr.hpp"


class TestSharedHeaders(SessionTestBase):
    def _graph_patch(self, deps: set[str]):
        return mock.patch(
            "tools.llm_decomp.include_graph.IncludeGraph.load_or_build",
            return_value=FakeGraph(deps))

    def test_shared_header_allowed_zero_dependents(self) -> None:
        provider = FakeProvider([
            reply(ToolCall("patch", {"files": [{
                "path": SHARED_HDR, "create": True,
                "content": "#ifndef HDR_HPP\n#define HDR_HPP\n#endif\n"}],
            }, id="c1")),
            reply(ToolCall("submit", {}, id="c2")),
        ])
        with self._graph_patch(set()), self.patched(
                verdict_for_submit=mock.Mock(return_value=verdict(
                    VerdictKind.ACCEPTED, True, "full_match", 0))):
            outcome = self.make_session(provider).run()
        self.assertTrue(outcome.accepted)
        self.assertTrue((self.root / SHARED_HDR).exists())

    def test_shared_header_refuse_tier_rejected(self) -> None:
        deps = {f"src/unit{i}/X.cpp" for i in range(41)}
        provider = FakeProvider([
            reply(ToolCall("patch", {"files": [{
                "path": SHARED_HDR, "create": True,
                "content": "#ifndef HDR_HPP\n#define HDR_HPP\n#endif\n"}],
            }, id="c1")),
        ])
        self.set_budgets(max_turns=2)
        with self._graph_patch(deps), self.patched(
                verdict_for_submit=mock.Mock()):
            outcome = self.make_session(provider).run()
        self.assertFalse(outcome.accepted)
        self.assertFalse((self.root / SHARED_HDR).exists())

    def test_cross_tu_failure_overrides_acceptance(self) -> None:
        from tools.llm_decomp.contracts import Regression  # noqa: F401
        sweep = mock.Mock(return_value=[{"unit": "other/Y", "ok": False,
                                         "detail": "boom"}])
        provider = FakeProvider([
            reply(ToolCall("patch", {"files": [{
                "path": SHARED_HDR, "create": True,
                "content": "#ifndef HDR_HPP\n#define HDR_HPP\n#endif\n"}],
            }, id="c1")),
            reply(ToolCall("submit", {}, id="c2")),
        ])
        accepted = verdict(VerdictKind.ACCEPTED, True, "full_match", 0)
        with self._graph_patch({"src/other/Y.cpp"}), self.patched(
                verdict_for_submit=mock.Mock(return_value=accepted)), \
                mock.patch("tools.llm_decomp.cross_tu.sweep_dependents",
                           sweep), \
                mock.patch("tools.coop.lib.targets.load_targets",
                           return_value=[]), \
                mock.patch("tools.coop.lib.config.load_config",
                           return_value=mock.Mock()):
            self.set_budgets(max_turns=4)
            outcome = self.make_session(provider).run()
        self.assertFalse(outcome.accepted)
        sweep.assert_called_once()

    def test_no_shared_headers_no_sweep(self) -> None:
        provider = FakeProvider([
            reply(ToolCall("submit", {}, id="c1")),
        ])
        with self._graph_patch(set()), self.patched(
                verdict_for_submit=mock.Mock(return_value=verdict(
                    VerdictKind.ACCEPTED, True, "full_match", 0))), \
                mock.patch("tools.llm_decomp.cross_tu.sweep_dependents") \
                as sweep:
            outcome = self.make_session(provider).run()
        self.assertTrue(outcome.accepted)
        sweep.assert_not_called()


if __name__ == "__main__":
    unittest.main()
