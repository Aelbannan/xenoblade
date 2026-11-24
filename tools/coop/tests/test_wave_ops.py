import json
import subprocess
import tempfile
import unittest
from pathlib import Path
from unittest import mock

from tools.coop.land import wave, wave_ops

COLS = wave_ops.COLS


def _row(i, model="hy4"):
    return [f"W9-{i:02d}", model, f"kyoshin/u{i}", f"fn{i}__Fv", "90.0", "2", "64", f"src/u{i}.cpp"]


class _Env(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        t = Path(self.tmp.name)
        self.patches = [mock.patch.object(wave, "WAVES", t / "waves"), mock.patch.object(wave, "PLANS", t / "plans"),
                        mock.patch.object(wave, "HISTORY", t / "hist.jsonl"), mock.patch.object(wave_ops, "ORCH", t / "orch")]
        for p in self.patches:
            p.start()
        self.d = wave.WAVES / "W9"
        self.d.mkdir(parents=True)
        wave.PLANS.mkdir()

    def tearDown(self):
        for p in self.patches:
            p.stop()
        self.tmp.cleanup()

    def write_targets(self, rows, sel=None):
        body = lambda rs: "".join("\t".join(r) + "\n" for r in rs)
        (self.d / "targets.tsv").write_text(body(rows))
        (self.d / "targets.sel.tsv").write_text(body(sel if sel is not None else rows))


class CheckPlanTests(_Env):
    def test_ok(self):
        self.write_targets([_row(i) for i in range(12)])
        (self.d / "plan.md").write_text("dropped: none\n")
        self.assertEqual(wave_ops.check_plan("W9"), [])

    def test_errors(self):
        sel = [_row(i) for i in range(12)]
        rows = [_row(i, "ds") for i in range(8)] + [_row(20)]
        rows[0][3] = "other__Fv"
        rows.append(_row(9, "opus"))
        self.write_targets(rows, sel)
        (self.d / "hints").mkdir()
        (self.d / "hints" / "W9-55.md").write_text("x\n")
        errs = "\n".join(wave_ops.check_plan("W9"))
        for frag in ("W9-20: not in the selection", "W9-00: unit/symbol/src changed", "model 'opus'",
                     "more than 6 ds rows", "hint W9-55.md has no target row", "plan.md missing"):
            self.assertIn(frag, errs)


class ReviewDecideTests(_Env):
    def setUp(self):
        super().setUp()
        self.write_targets([_row(i) for i in range(4)])
        for i in range(4):
            (wave.PLANS / f"W9-{i:02d}.patch").write_text("--- a\n")

    def test_check_review_errors(self):
        (self.d / "review.tsv").write_text("W9-00\tland\tok\nW9-01\tmaybe\tx\nW9-02\tretry-ds\tneeds sig\n")
        errs = "\n".join(wave_ops.check_review("W9"))
        self.assertIn("decision 'maybe'", errs)
        self.assertIn("retry-ds without hints/W9-02.md", errs)
        self.assertIn("W9-03: no review row", errs)

    def test_decide(self):
        (self.d / "hints").mkdir()
        (self.d / "hints" / "W9-02.md").write_text("callee takes 2 args\n")
        (self.d / "review.tsv").write_text(
            "W9-00\tland\tclean\nW9-01\thold\tsingle-use inline\nW9-02\tretry-ds\tsig\nW9-03\tdrop\tnone\n")
        r = wave_ops.decide("W9")
        self.assertEqual(r["lands"], ["W9-00"])
        self.assertEqual((wave_ops.ORCH / "W9.multi").read_text(), "W9-00\t\t\t\t\n")
        self.assertTrue((wave.PLANS / "held" / "W9-01.patch").is_file())
        self.assertFalse((wave.PLANS / "W9-02.patch").exists())
        self.assertEqual(r["retry_wave"], "W9r")
        rt = (wave.WAVES / "W9r" / "targets.tsv").read_text().split("\t")
        self.assertEqual(rt[:2], ["W9-02", "ds"])
        self.assertTrue((wave.WAVES / "W9r" / "hints" / "W9-02.md").is_file())
        decisions = [json.loads(l)["decision"] for l in wave.HISTORY.read_text().splitlines()]
        self.assertEqual(decisions, ["land", "hold", "hold", "drop"])

    def test_retry_wave_may_not_retry(self):
        (wave.WAVES / "W9r").mkdir()
        (wave.WAVES / "W9r" / "targets.tsv").write_text("\t".join(_row(0, "ds")) + "\n")
        (wave.WAVES / "W9r" / "review.tsv").write_text("W9-00\tretry-ds\tagain\n")
        self.assertTrue(any("decision 'retry-ds'" in e for e in wave_ops.check_review("W9r")))


class PermissionTests(unittest.TestCase):
    R = str(wave.ROOT)

    def c(self, cmd):
        return wave_ops.classify_permission({"agentId": "a1", "id": "r1", "request": {"input": {"command": cmd}}})[0]

    def test_allow_in_repo(self):
        self.assertEqual(self.c(f"cd {self.R} && .venv/bin/python3 tools/coop/hexdiff.py u --symbol f 2>/dev/null"), "allow")
        self.assertEqual(self.c("cat /tmp/xb/packs/W2-01.md"), "allow")

    def test_deny(self):
        for cmd in ("git status", f"cd {self.R}; git diff", "cat /etc/passwd", "ls ~/Documents",
                    f"cat {self.R}/../secret", "rm -rf /tmp/xb/W2-01", "XB_LAND_OWNER=1 python -m tools.coop.land apply q",
                    f"vim {self.R}/src/a.ctx.c"):
            self.assertEqual(self.c(cmd), "deny", cmd)
        self.assertEqual(wave_ops.classify_permission({"agentId": "a", "id": "r"})[0], "deny")

    def test_handle_only_our_agents(self):
        tmp = tempfile.TemporaryDirectory()
        self.addCleanup(tmp.cleanup)
        with mock.patch.object(wave, "WAVES", Path(tmp.name)):
            (Path(tmp.name) / "W9").mkdir()
            (Path(tmp.name) / "W9" / "agents.json").write_text(json.dumps({"W9-00": {"id": "abc123", "model": "hy4", "started": 0}}))
            pending = [{"agentId": "abc123", "id": "r1", "request": {"command": "git log"}},
                       {"agentId": "zzz999", "id": "r2", "request": {"command": "cat /etc/hosts"}}]
            calls = []

            def runner(*args):
                calls.append(args)
                out = json.dumps(pending) if args[:2] == ("permit", "ls") else ""
                return subprocess.CompletedProcess(args, 0, out, "")
            log = wave_ops.handle_permissions("W9", runner)
        self.assertEqual(calls[1], ("permit", "deny", "abc123", "r1"))
        self.assertEqual(len(calls), 2)  # the foreign agent's request is left alone
        self.assertIn("git", log[0])


class LaunchTests(_Env):
    def test_throttles_ds_and_is_idempotent(self):
        self.write_targets([_row(0), _row(1, "ds"), _row(2, "ds"), _row(3, "ds"), _row(4)])
        for i in range(5):
            (self.d / f"W9-{i:02d}.prompt").write_text("go\n")
        n = iter(range(100))
        runner = lambda *a: subprocess.CompletedProcess(a, 0, f"id{next(n)}\n", "")
        first = wave_ops.launch("W9", ds_max=2, runner=runner)
        self.assertEqual([l.split("\t")[0] for l in first], ["W9-00", "W9-01", "W9-02", "W9-04"])
        self.assertEqual(wave_ops.launch("W9", ds_max=2, runner=runner), [])
        (self.d / "W9-01.result").write_text("DONE\n")
        self.assertEqual([l.split("\t")[0] for l in wave_ops.launch("W9", ds_max=2, runner=runner)], ["W9-03"])


if __name__ == "__main__":
    unittest.main()
