from __future__ import annotations

import shutil
import tempfile
import unittest
from pathlib import Path
from unittest import mock

from tools.decomp_atlas.lib.jobs import JobController, MAX_CONCURRENT
from tools.decomp_atlas.tests import PROJECT


class _FakeProc:
    def __init__(self) -> None:
        self.pid = 4242
        self._code = None
        self.returncode = None

    def poll(self):
        return self._code

    def terminate(self) -> None:
        self._code = -15
        self.returncode = -15

    def kill(self) -> None:
        self._code = -9
        self.returncode = -9

    def wait(self, timeout=None):
        return self.returncode


class JobsTests(unittest.TestCase):
    def setUp(self) -> None:
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name) / "project"
        shutil.copytree(PROJECT, self.root)
        self.controller = JobController(self.root, "us")

    def tearDown(self) -> None:
        self.tmp.cleanup()

    def test_start_cancel_list(self) -> None:
        fake = _FakeProc()

        def fake_popen(argv, **kwargs):
            self.assertTrue(str(argv[1]).endswith("tools/llm_harness/run.py"))
            self.assertEqual(argv[2], "new")
            self.assertEqual(argv[3], "demo-leaf")
            self.assertFalse(kwargs.get("shell"))
            return fake

        with mock.patch("subprocess.Popen", side_effect=fake_popen):
            meta = self.controller.start_job("demo-leaf", "new")
        self.assertEqual(meta["status"], "running")
        self.assertEqual(meta["workflow"], "new")
        jobs = self.controller.list_jobs()
        self.assertEqual(len(jobs), 1)
        cancelled = self.controller.cancel_job(meta["job_id"])
        self.assertEqual(cancelled["status"], "cancelled")

    def test_rejects_bad_workflow_and_concurrency(self) -> None:
        with self.assertRaises(ValueError):
            self.controller.start_job("demo-leaf", "promote")
        fakes = [_FakeProc() for _ in range(MAX_CONCURRENT)]
        with mock.patch("subprocess.Popen", side_effect=fakes):
            for i in range(MAX_CONCURRENT):
                self.controller.start_job(f"demo-leaf", "improve")
            with self.assertRaises(RuntimeError):
                self.controller.start_job("demo-leaf", "new")


if __name__ == "__main__":
    unittest.main()
