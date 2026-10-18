"""Region control: renames only touch explicitly requested regions.

Default is us only (the US pipeline is what matters; jp is corrupt).  A
rename must never rewrite a region the caller did not ask for.  Tests assert
with byte hashes that unlisted regions stay identical.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import unittest
from pathlib import Path
from tempfile import TemporaryDirectory

from tools.coop import memberize_bulk as mb
from tools.coop.symbol_fn_rename import (
    FnRenameRefusal,
    apply_function_rename,
    plan_function_rename,
)


def _sha(p: Path) -> str:
    return hashlib.sha256(p.read_bytes()).hexdigest()


def _write_three_region_fixture(root: Path, *, old: str = "Widget_Paint") -> None:
    for reg in ("us", "eu", "jp"):
        (root / "config" / reg).mkdir(parents=True, exist_ok=True)
        (root / "config" / reg / "symbols.txt").write_text(
            f"{old} = .text:0x80001000; // type:function size:0x40\n",
            encoding="utf-8")
    (root / "src" / "unit").mkdir(parents=True, exist_ok=True)
    (root / "src" / "other").mkdir(parents=True, exist_ok=True)
    (root / "src" / "unit" / "Widget.hpp").write_text(
        "#pragma once\nclass WdgState {\npublic:\n    void Existing();\n};\n",
        encoding="utf-8")
    (root / "src" / "unit" / "Widget.cpp").write_text(
        '#include "Widget.hpp"\n'
        f'extern "C" void {old}(WdgState* self, int x, int y);\n'
        f'void {old}(WdgState* self, int x, int y) {{\n'
        '    self->value = x;\n'
        '}\n',
        encoding="utf-8")
    (root / "src" / "other" / "Caller.cpp").write_text(
        f'#include "Widget.hpp"\nvoid run() {{ {old}(a, 1, 2); }}\n',
        encoding="utf-8")
    (root / "configure.py").write_text("// stub\n", encoding="utf-8")


def _write_memberize_fixture(root: Path) -> None:
    # Two-region memberize fixture (us + eu + jp) with one CLEAN candidate.
    for reg in ("us", "eu", "jp"):
        (root / "config" / reg).mkdir(parents=True, exist_ok=True)
    (root / "src" / "unit").mkdir(parents=True, exist_ok=True)
    (root / "src" / "other").mkdir(parents=True, exist_ok=True)
    old, cls = "SchedList_DropItem", "SLList"
    for reg in ("us", "eu", "jp"):
        (root / "config" / reg / "symbols.txt").write_text(
            f"{old} = .text:0x80001000; // type:function size:0x40\n",
            encoding="utf-8")
    (root / "src" / "unit" / "Owner.hpp").write_text(
        "#pragma once\nclass SLList {\npublic:\n    void Existing();\n};\n",
        encoding="utf-8")
    (root / "src" / "unit" / "Def.cpp").write_text(
        '#include "Owner.hpp"\n'
        f'extern "C" void {old}({cls}* self, int x);\n'
        f'void {old}({cls}* self, int x) {{ (void)self; }}\n',
        encoding="utf-8")
    (root / "src" / "other" / "Caller.cpp").write_text(
        f'#include "Owner.hpp"\nvoid run() {{ {old}(a, b); }}\n',
        encoding="utf-8")
    (root / "configure.py").write_text("// stub\n", encoding="utf-8")


class ApplyRegionListTest(unittest.TestCase):
    def test_us_only_leaves_eu_jp_byte_identical(self):
        with TemporaryDirectory() as td:
            root = Path(td)
            _write_three_region_fixture(root)
            eu_before = _sha(root / "config" / "eu" / "symbols.txt")
            jp_before = _sha(root / "config" / "jp" / "symbols.txt")
            us_before = _sha(root / "config" / "us" / "symbols.txt")
            plan, _ = apply_function_rename(
                root, "Widget_Paint", "WdgState::Paint(int, int)",
                regions=["us"], dry_run=False, demote_import_band=True)
            self.assertEqual(_sha(root / "config" / "eu" / "symbols.txt"), eu_before)
            self.assertEqual(_sha(root / "config" / "jp" / "symbols.txt"), jp_before)
            self.assertNotEqual(_sha(root / "config" / "us" / "symbols.txt"), us_before)
            self.assertIn("Paint__8WdgStateFii",
                          (root / "config" / "us" / "symbols.txt").read_text())
            self.assertEqual(len(plan.symbol_edits), 1)
            self.assertEqual(plan.symbol_edits[0].region, "us")

    def test_explicit_us_eu_updates_both_leaves_jp(self):
        with TemporaryDirectory() as td:
            root = Path(td)
            _write_three_region_fixture(root)
            jp_before = _sha(root / "config" / "jp" / "symbols.txt")
            apply_function_rename(
                root, "Widget_Paint", "WdgState::Paint(int, int)",
                regions=["us", "eu"], dry_run=False, demote_import_band=True)
            for reg in ("us", "eu"):
                self.assertIn("Paint__8WdgStateFii",
                              (root / "config" / reg / "symbols.txt").read_text())
            self.assertEqual(_sha(root / "config" / "jp" / "symbols.txt"), jp_before)

    def test_plan_regions_propagate(self):
        with TemporaryDirectory() as td:
            root = Path(td)
            _write_three_region_fixture(root)
            plan = plan_function_rename(
                root, "Widget_Paint", "WdgState::Paint(int, int)",
                regions=["us"])
            self.assertEqual([e.region for e in plan.symbol_edits], ["us"])

    def test_unknown_region_refuses(self):
        with TemporaryDirectory() as td:
            root = Path(td)
            _write_three_region_fixture(root)
            with self.assertRaises(FnRenameRefusal):
                plan_function_rename(
                    root, "Widget_Paint", "WdgState::Paint(int, int)",
                    regions=["xx"])


class MemberizeRegionTest(unittest.TestCase):
    def _plan_json_for(self, root: Path) -> Path:
        scan = [{"file": "src/unit/Def.cpp", "line": 2,
                 "name": "SchedList_DropItem", "kind": "decl", "self_style": True}]
        data = mb.plan_all(root, scan)
        self.assertEqual(data["summary"]["clean"], 1)
        pj = root / "plan.json"
        pj.write_text(json.dumps(data), encoding="utf-8")
        return pj

    def _passing_snapshotter(self):
        # Name-only gate: every rename verifies byte-identical.
        def snap(_root: Path, _unit: str, aliases=None):
            aliases = aliases or {}
            if not aliases:
                return {0x1000: {"symbol": "SchedList_DropItem",
                                 "decomp_symbol": "SchedList_DropItem",
                                 "present": True, "retail_size": 0x40,
                                 "decomp_size": 0x40, "mismatch": 0}}
            # after: retail unchanged, decomp NEW, sizes identical.
            out = {}
            for old, new in aliases.items():
                out[0x1000] = {"symbol": old, "decomp_symbol": new,
                               "present": True, "retail_size": 0x40,
                               "decomp_size": 0x40, "mismatch": 0}
            return out
        # Simpler: before always Old, after always New (same sizes).
        calls: list[int] = []

        def counting(_root: Path, _unit: str, aliases=None):
            calls.append(1)
            if len(calls) % 2 == 1:
                return {0x1000: {"symbol": "SchedList_DropItem",
                                 "decomp_symbol": "SchedList_DropItem",
                                 "present": True, "retail_size": 0x40,
                                 "decomp_size": 0x40, "mismatch": 0}}
            new = next(iter((aliases or {}).values()),
                       "DropItem__6SLListFi")
            return {0x1000: {"symbol": "SchedList_DropItem",
                             "decomp_symbol": new,
                             "present": True, "retail_size": 0x40,
                             "decomp_size": 0x40, "mismatch": 0}}
        return counting

    def test_apply_region_us_leaves_eu_jp_identical(self):
        with TemporaryDirectory() as td:
            root = Path(td)
            _write_memberize_fixture(root)
            pj = self._plan_json_for(root)
            eu_before = _sha(root / "config" / "eu" / "symbols.txt")
            jp_before = _sha(root / "config" / "jp" / "symbols.txt")
            ns = argparse.Namespace(
                plan_json=str(pj), candidate=[], unit=None, limit=None,
                execute=True, dry_run=False, json=None, report=None,
                region="us",
                _snapshotter=self._passing_snapshotter(), _applier=None,
                _no_post=True)
            rc = mb.cmd_apply(ns, root)
            self.assertEqual(rc, 0)
            self.assertEqual(_sha(root / "config" / "eu" / "symbols.txt"), eu_before)
            self.assertEqual(_sha(root / "config" / "jp" / "symbols.txt"), jp_before)
            self.assertIn("DropItem__",
                          (root / "config" / "us" / "symbols.txt").read_text())

    def test_apply_default_is_us_only(self):
        # No --region passed (legacy Namespace without the attr, or None):
        # must still default to us only and leave eu/jp identical.
        with TemporaryDirectory() as td:
            root = Path(td)
            _write_memberize_fixture(root)
            pj = self._plan_json_for(root)
            eu_before = _sha(root / "config" / "eu" / "symbols.txt")
            jp_before = _sha(root / "config" / "jp" / "symbols.txt")
            ns = argparse.Namespace(
                plan_json=str(pj), candidate=[], unit=None, limit=None,
                execute=True, dry_run=False, json=None, report=None,
                _snapshotter=self._passing_snapshotter(), _applier=None,
                _no_post=True)
            # No 'region' attr at all -> getattr default None -> us only.
            self.assertFalse(hasattr(ns, "region"))
            rc = mb.cmd_apply(ns, root)
            self.assertEqual(rc, 0)
            self.assertEqual(_sha(root / "config" / "eu" / "symbols.txt"), eu_before)
            self.assertEqual(_sha(root / "config" / "jp" / "symbols.txt"), jp_before)

    def test_apply_comma_list_us_eu(self):
        with TemporaryDirectory() as td:
            root = Path(td)
            _write_memberize_fixture(root)
            pj = self._plan_json_for(root)
            jp_before = _sha(root / "config" / "jp" / "symbols.txt")
            ns = argparse.Namespace(
                plan_json=str(pj), candidate=[], unit=None, limit=None,
                execute=True, dry_run=False, json=None, report=None,
                region="us,eu",
                _snapshotter=self._passing_snapshotter(), _applier=None,
                _no_post=True)
            rc = mb.cmd_apply(ns, root)
            self.assertEqual(rc, 0)
            for reg in ("us", "eu"):
                self.assertIn("DropItem__",
                              (root / "config" / reg / "symbols.txt").read_text())
            self.assertEqual(_sha(root / "config" / "jp" / "symbols.txt"), jp_before)

    def test_parse_region_list(self):
        self.assertEqual(mb.parse_region_list(None), ["us"])
        self.assertEqual(mb.parse_region_list("us"), ["us"])
        self.assertEqual(mb.parse_region_list("us,eu"), ["us", "eu"])
        self.assertEqual(mb.parse_region_list(" eu , us "), ["eu", "us"])
        with self.assertRaises(ValueError):
            mb.parse_region_list("xx")


if __name__ == "__main__":
    unittest.main()
