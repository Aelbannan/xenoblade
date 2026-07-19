from __future__ import annotations

import unittest

from tools.decomp_atlas.lib.experiments import normalize_experiments
from tools.decomp_atlas.lib.project_adapter import AtlasProjectAdapter
from tools.decomp_atlas.tests import PROJECT


class ExperimentsTests(unittest.TestCase):
    def test_normalize_fixture_experiments(self) -> None:
        adapter = AtlasProjectAdapter(PROJECT, config_path=PROJECT / "coop.json")
        rows = normalize_experiments(adapter.load_experiments())
        self.assertEqual(len(rows), 2)
        winners = [r for r in rows if r["winner"]]
        self.assertEqual(len(winners), 1)
        self.assertEqual(winners[0]["result_status"], "FULL_MATCH")
        self.assertEqual(winners[0]["match_percent"], 100.0)
        self.assertEqual(winners[0]["accepted"], 1)


if __name__ == "__main__":
    unittest.main()
