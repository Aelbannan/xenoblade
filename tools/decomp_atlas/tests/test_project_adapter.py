from __future__ import annotations

import unittest

from tools.decomp_atlas.lib.project_adapter import AtlasProjectAdapter
from tools.decomp_atlas.tests import PROJECT


class ProjectAdapterTests(unittest.TestCase):
    def test_list_function_records_relative_paths(self) -> None:
        adapter = AtlasProjectAdapter(PROJECT, config_path=PROJECT / "coop.json")
        records = adapter.list_function_records()
        self.assertGreaterEqual(len(records), 7)
        by_id = {r["target_id"]: r for r in records}
        leaf = by_id["demo-leaf"]
        self.assertEqual(leaf["source"], "src/demo/Demo.cpp")
        self.assertEqual(leaf["unit"], "demo/Demo")
        self.assertTrue(leaf["buildable"])
        self.assertEqual(leaf["target_object"], "build/us/asm/demo/Demo.o")
        placeholder = by_id["demo-func-placeholder"]
        self.assertFalse(placeholder["buildable"])
        self.assertEqual(placeholder["origin"], "symbols.txt")

    def test_never_mutates_targets(self) -> None:
        targets_path = PROJECT / "tools" / "coop" / "targets.json"
        before = targets_path.read_text(encoding="utf-8")
        adapter = AtlasProjectAdapter(PROJECT, config_path=PROJECT / "coop.json")
        adapter.list_function_records()
        adapter.load_attempts()
        adapter.load_experiments()
        after = targets_path.read_text(encoding="utf-8")
        self.assertEqual(before, after)

    def test_certified_ids(self) -> None:
        adapter = AtlasProjectAdapter(PROJECT, config_path=PROJECT / "coop.json")
        certified = adapter.certified_ids()
        self.assertIn("demo-accepted", certified)


if __name__ == "__main__":
    unittest.main()
