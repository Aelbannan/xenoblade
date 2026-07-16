from __future__ import annotations

import json
import tempfile
import unittest
from pathlib import Path

from tools.coop.lib.config import CoopConfig
from tools.coop.lib.targets import (
    claim_target,
    import_symbols,
    load_split_ranges,
    release_target,
    validate_targets,
)


class _Unit:
    def __init__(self, root: Path) -> None:
        self.name = "main/foo/Bar"
        self.source_path = root / "src/foo/Bar.cpp"


class _Project:
    def __init__(self, root: Path) -> None:
        self.root = root

    def load_objdiff_units(self):
        return [_Unit(self.root)]


class TargetRegistryTests(unittest.TestCase):
    def setUp(self) -> None:
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name)
        (self.root / "tools/coop").mkdir(parents=True)
        (self.root / "config/us").mkdir(parents=True)
        (self.root / "src/foo").mkdir(parents=True)
        (self.root / "src/foo/Bar.cpp").touch()
        self.config = CoopConfig(project_root=self.root, region="us")
        (self.root / "tools/coop/targets.json").write_text(
            json.dumps(
                {
                    "schema_version": 2,
                    "default_required_level": "EQUIVALENT_MATCH",
                    "targets": [],
                }
            ),
            encoding="utf-8",
        )

    def tearDown(self) -> None:
        self.tmp.cleanup()

    def test_split_parser_and_function_import_are_idempotent(self) -> None:
        (self.root / "config/us/splits.txt").write_text(
            "foo/Bar.cpp:\n\t.text start:0x80001000 end:0x80001100\n",
            encoding="utf-8",
        )
        (self.root / "config/us/symbols.txt").write_text(
            "func__3BarFv = .text:0x80001020; // type:function size:0x20 scope:global\n"
            "global = .data:0x80500000; // type:object size:0x4 scope:global\n",
            encoding="utf-8",
        )
        ranges = load_split_ranges(self.root / "config/us/splits.txt")
        self.assertEqual(ranges[0].unit, "foo/Bar.cpp")

        data, added, skipped = import_symbols(_Project(self.root), self.config)
        self.assertEqual((added, skipped), (1, 0))
        row = data["targets"][0]
        self.assertEqual(row["id"], "us-80001020")
        self.assertEqual(row["unit"], "foo/Bar")
        self.assertEqual(row["source"], "src/foo/Bar.cpp")

        (self.root / "tools/coop/targets.json").write_text(
            json.dumps(data), encoding="utf-8"
        )
        _data, added, skipped = import_symbols(_Project(self.root), self.config)
        self.assertEqual((added, skipped), (0, 1))

    def test_all_import_includes_data_symbols(self) -> None:
        (self.root / "config/us/splits.txt").write_text("", encoding="utf-8")
        (self.root / "config/us/symbols.txt").write_text(
            "f = .text:0x80001000; // type:function size:0x4 scope:global\n"
            "g = .data:0x80500000; // type:object size:0x4 scope:global\n",
            encoding="utf-8",
        )
        data, added, _skipped = import_symbols(
            _Project(self.root), self.config, kind="all"
        )
        self.assertEqual(added, 2)
        self.assertEqual({row["kind"] for row in data["targets"]}, {"function", "object"})

    def test_aliases_at_same_address_are_all_imported(self) -> None:
        (self.root / "config/us/splits.txt").write_text("", encoding="utf-8")
        (self.root / "config/us/symbols.txt").write_text(
            "first = .text:0x80001000; // type:function size:0x4 scope:global\n"
            "alias = .text:0x80001000; // type:function size:0x4 scope:global\n",
            encoding="utf-8",
        )
        data, added, _skipped = import_symbols(_Project(self.root), self.config)
        self.assertEqual(added, 2)
        self.assertEqual({row["id"] for row in data["targets"]}, {"us-80001000", "us-80001000-2"})

    def test_validation_rejects_duplicate_symbol_identity(self) -> None:
        rows = [
            {"id": "a", "symbol": "f", "address": "0x1"},
            {"id": "b", "symbol": "f", "address": "0x1"},
        ]
        (self.root / "tools/coop/targets.json").write_text(
            json.dumps({"schema_version": 2, "targets": rows}), encoding="utf-8"
        )
        errors = validate_targets(self.config)
        self.assertTrue(any("duplicate symbol identity" in error for error in errors))

    def test_claim_requires_release_before_owner_changes(self) -> None:
        path = self.root / "tools/coop/targets.json"
        path.write_text(
            json.dumps(
                {
                    "schema_version": 2,
                    "targets": [
                        {"id": "a", "symbol": "f", "address": "0x1", "status": "NOT_STARTED"}
                    ],
                }
            ),
            encoding="utf-8",
        )
        claim_target(self.config, "a", owner="one", allowed_paths=["src/a.cpp"])
        with self.assertRaisesRegex(ValueError, "already claimed"):
            claim_target(self.config, "a", owner="two", allowed_paths=[])
        release_target(self.config, "a", owner="one")
        claim_target(self.config, "a", owner="two", allowed_paths=[])


if __name__ == "__main__":
    unittest.main()
