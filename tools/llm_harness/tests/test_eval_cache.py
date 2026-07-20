"""Tests for worktree SPLIT stamp helper."""

from __future__ import annotations

import json
import os
import tempfile
import time
import unittest
from pathlib import Path

from tools.llm_harness.eval_cache import stamp_split_config


class StampSplitConfigTests(unittest.TestCase):
    def test_replaces_symlink_and_bumps_mtime(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            main_cfg = root / "main" / "config.json"
            main_cfg.parent.mkdir(parents=True)
            main_cfg.write_text(json.dumps({"version": "1.0.0"}), encoding="utf-8")
            workspace = root / "worktree"
            region_build = workspace / "build" / "us"
            region_build.mkdir(parents=True)
            link = region_build / "config.json"
            link.symlink_to(main_cfg)
            yml = workspace / "config" / "us" / "config.yml"
            yml.parent.mkdir(parents=True)
            yml.write_text("name: us\n", encoding="utf-8")
            # Make yaml appear newer than the shared config so a naive symlink would rebuild.
            future = time.time() + 3600
            os.utime(yml, (future, future))
            os.utime(main_cfg, (future - 7200, future - 7200))

            self.assertTrue(stamp_split_config(workspace, "us"))
            stamped = region_build / "config.json"
            self.assertTrue(stamped.is_file())
            self.assertFalse(stamped.is_symlink())
            self.assertGreater(stamped.stat().st_mtime, yml.stat().st_mtime)
            # Shared main tree untouched as a symlink target rewrite.
            self.assertTrue(main_cfg.is_file())
            self.assertEqual(
                json.loads(stamped.read_text(encoding="utf-8"))["version"],
                "1.0.0",
            )

    def test_missing_config_returns_false(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            workspace = Path(tmp)
            self.assertFalse(stamp_split_config(workspace, "us"))


if __name__ == "__main__":
    unittest.main()
