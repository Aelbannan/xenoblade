from __future__ import annotations

import unittest

from tools.decomp_atlas.lib.attempts import attempt_stats_by_target, normalize_attempts
from tools.decomp_atlas.lib.project_adapter import AtlasProjectAdapter
from tools.decomp_atlas.tests import PROJECT


class AttemptsTests(unittest.TestCase):
    def test_normalize_fixture_attempts(self) -> None:
        adapter = AtlasProjectAdapter(PROJECT, config_path=PROJECT / "coop.json")
        rows = normalize_attempts(adapter.load_attempts())
        self.assertEqual(len(rows), 3)
        self.assertEqual(rows[0]["target_id"], "demo-leaf")
        self.assertEqual(rows[0]["instruction_match"], 0.0)
        stats = attempt_stats_by_target(rows)
        self.assertEqual(stats["demo-callees-ready"]["attempt_count"], 2)
        self.assertGreaterEqual(stats["demo-callees-ready"]["non_improving_attempts"], 1)


if __name__ == "__main__":
    unittest.main()
