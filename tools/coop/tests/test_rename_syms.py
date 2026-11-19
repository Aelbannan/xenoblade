"""Regression: tools/coop/faithful/rename_syms.py must keep targets.json valid JSON
when a new name contains a backslash (e.g. __sinit_\\CTagProcessor_cpp)."""

from __future__ import annotations

import importlib.util
import json
import unittest
from pathlib import Path

_spec = importlib.util.spec_from_file_location(
    "rename_syms", Path(__file__).resolve().parents[1] / "faithful" / "rename_syms.py")
rename_syms = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(rename_syms)


class RenameSymsTest(unittest.TestCase):
    def test_backslash_name_roundtrips(self):
        data = {"targets": [{"symbol": "sinit_8012BA00"}, {"symbol": "other"}]}
        n = rename_syms.rename_targets(data, [("sinit_8012BA00", "__sinit_\\CTagProcessor_cpp")])
        self.assertEqual(n, 1)
        text = json.dumps(data, indent=2, ensure_ascii=False)
        self.assertEqual(json.loads(text)["targets"][0]["symbol"], "__sinit_\\CTagProcessor_cpp")

    def test_exact_match_only(self):
        data = {"targets": [{"symbol": "foo_bar"}, {"name": "foo"}]}
        self.assertEqual(rename_syms.rename_targets(data, [("foo", "baz")]), 1)
        self.assertEqual(data["targets"][0]["symbol"], "foo_bar")
        self.assertEqual(data["targets"][1]["name"], "baz")


if __name__ == "__main__":
    unittest.main()
