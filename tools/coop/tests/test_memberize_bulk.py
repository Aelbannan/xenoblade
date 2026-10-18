"""Tests for the supervised BULK member-ization driver (memberize_bulk)."""
from __future__ import annotations

import argparse
import json
import unittest
from pathlib import Path
from tempfile import TemporaryDirectory

from tools.coop import memberize_bulk as mb
from tools.coop.memberize_bulk import (
    compare_snapshots,
    discover_class,
    filter_true_memberizable,
    load_scan_candidates,
    plan_all,
    propose_method_name,
    unit_for_file,
)
from tools.coop.symbol_fn_rename import FnRenameRefusal


def _write_basic(root: Path, *, old="SchedList_DropItem", cls="SLList",
                 method_extra="CSchedule* x", with_addr_ref=False,
                 with_bare=False) -> None:
    (root / "config" / "us").mkdir(parents=True, exist_ok=True)
    (root / "src" / "unit").mkdir(parents=True, exist_ok=True)
    (root / "src" / "other").mkdir(parents=True, exist_ok=True)
    (root / "config" / "us" / "symbols.txt").write_text(
        f"{old} = .text:0x80001000; // type:function size:0x40\n", encoding="utf-8")
    (root / "config" / "eu").mkdir(parents=True, exist_ok=True)
    (root / "config" / "eu" / "symbols.txt").write_text(
        f"{old} = .text:0x80001000;\n", encoding="utf-8")
    (root / "src" / "unit" / "Owner.hpp").write_text(
        "#pragma once\nclass SLList {\npublic:\n    void Existing();\n};\n",
        encoding="utf-8")
    (root / "src" / "unit" / "Def.cpp").write_text(
        '#include "Owner.hpp"\n'
        f'extern "C" void {old}({cls}* self, {method_extra});\n'
        f'void {old}({cls}* self, {method_extra}) {{\n'
        '    (void)self;\n'
        '}\n', encoding="utf-8")
    caller = f'#include "Owner.hpp"\nvoid run() {{\n    {old}(a, b);\n}}\n'
    if with_addr_ref:
        caller += f'void* p = (void*)&{old};\n'
    if with_bare:
        caller += f'int bare = {old};\n'
    (root / "src" / "other" / "Caller.cpp").write_text(caller, encoding="utf-8")
    (root / "configure.py").write_text("// stub\n", encoding="utf-8")


class FilterTest(unittest.TestCase):
    def test_true_set(self):
        cands = [
            {"name": "Foo", "self_style": True},
            {"name": "Bar__Q", "self_style": True},
            {"name": "Baz", "self_style": False},
            {"name": "Qux__X", "self_style": False},
        ]
        out = filter_true_memberizable(cands)
        self.assertEqual([c["name"] for c in out], ["Foo"])

    def test_load_scan_candidates_shape(self):
        with TemporaryDirectory() as td:
            p = Path(td) / "scan.json"
            p.write_text(json.dumps({"member_candidates": [
                ["a.cpp", 1, "Foo", "decl", True],
                ["b.cpp", 2, "Bar__Q", "decl", True],
            ]}), encoding="utf-8")
            rows = load_scan_candidates(p)
            self.assertEqual(len(rows), 2)
            self.assertTrue(rows[0]["self_style"])


class DiscoverTest(unittest.TestCase):
    def setUp(self):
        self.tmp = TemporaryDirectory()
        self.root = Path(self.tmp.name)
        _write_basic(self.root)

    def tearDown(self):
        self.tmp.cleanup()

    def test_discovers_sl_list(self):
        self.assertEqual(discover_class(self.root, "SchedList_DropItem"), "SLList")

    def test_missing_definition_blocked(self):
        with self.assertRaises(FnRenameRefusal):
            discover_class(self.root, "Nope_Nothing")


class NamingPolicyTest(unittest.TestCase):
    def test_table(self):
        cases = [
            ("SchedList_DropItem", "SLList", "DropItem"),
            ("ColiBuildAxisFrameMixed", "CColiObject", "BuildAxisFrameMixed"),
            ("Coli_XformNotifyA_6C48", "CColiObject", "XformNotifyA"),
            ("BgTex_Tick_3D54", "BgTex", "Tick"),
            ("EffFxInitNodeHead", "EffectInit", "EffFxInitNodeHead"),
            ("Widget_Paint", "WdgState", "Widget_Paint"),
            ("func_804A7834", "CColiObject", "func_804A7834"),
        ]
        for old, cls, want in cases:
            with self.subTest(old=old):
                self.assertEqual(propose_method_name(old, cls), want)

    def test_trailing_hex_stripped(self):
        self.assertEqual(propose_method_name("Foo_791C", "Bar"), "Foo")
        self.assertEqual(propose_method_name("Foo_791c", "Bar"), "Foo")

    def test_refuses_empty(self):
        self.assertIsNone(propose_method_name("", "SLList"))

    def test_refuses_leading_digit(self):
        self.assertIsNone(propose_method_name("123bad", "Foo"))

    def test_refuses_non_identifier(self):
        self.assertIsNone(propose_method_name("Foo-Bar", "Foo"))


class ClassificationTest(unittest.TestCase):
    def setUp(self):
        self.tmp = TemporaryDirectory()
        self.root = Path(self.tmp.name)

    def tearDown(self):
        self.tmp.cleanup()

    def test_clean_with_demote(self):
        _write_basic(self.root)
        from tools.coop.memberize_bulk import classify_candidate
        plan, used_nhi = classify_candidate(
            self.root, "SchedList_DropItem", "SLList", "DropItem")
        self.assertEqual(plan.reviews, [])
        self.assertEqual(plan.addr_refs, [])
        self.assertFalse(used_nhi)

    def test_needs_review_on_addr_ref(self):
        _write_basic(self.root, with_addr_ref=True)
        from tools.coop.memberize_bulk import classify_candidate
        plan, _ = classify_candidate(
            self.root, "SchedList_DropItem", "SLList", "DropItem")
        self.assertGreater(len(plan.addr_refs), 0)

    def test_pointer_arithmetic_call_stays_clean(self):
        # Option A regression: a call through raw pointer arithmetic (the
        # live CMiniMap shape MiniMapFillMarkerEntry((u8*)self + 0x17C)) is
        # rewritten with an explicit cast, so the candidate stays CLEAN and
        # the bulk driver attempts it instead of blocking the unit.
        _write_basic(self.root)
        (self.root / "src" / "other" / "Caller.cpp").write_text(
            '#include "Owner.hpp"\n'
            "void run() {\n"
            "    SchedList_DropItem((u8*)self + 0x17C, b);\n"
            "}\n",
            encoding="utf-8",
        )
        from tools.coop.memberize_bulk import classify_candidate
        plan, used_nhi = classify_candidate(
            self.root, "SchedList_DropItem", "SLList", "DropItem")
        self.assertEqual(plan.reviews, [])
        self.assertEqual(plan.addr_refs, [])
        self.assertEqual(len(plan.calls), 1)
        self.assertEqual(
            plan.calls[0].new_span,
            "((SLList*)((u8*)self + 0x17C))->DropItem(b)",
        )
        self.assertFalse(used_nhi)

    def test_plan_all_cast_candidate_clean(self):
        # Plan-level: the pointer-arithmetic candidate above classifies
        # CLEAN end to end (not NEEDS-REVIEW, not blocked).
        _write_basic(self.root)
        (self.root / "src" / "other" / "Caller.cpp").write_text(
            '#include "Owner.hpp"\n'
            "void run() {\n"
            "    SchedList_DropItem(arr[i], b);\n"
            "}\n",
            encoding="utf-8",
        )
        scan = [{"file": "src/unit/Def.cpp", "line": 2,
                 "name": "SchedList_DropItem", "kind": "decl", "self_style": True}]
        data = plan_all(self.root, scan)
        self.assertEqual(data["summary"]["clean"], 1)
        row = data["candidates"][0]
        self.assertEqual(row["classification"], "CLEAN")
        self.assertEqual(row["class"], "SLList")

    def test_plan_all_clean_row(self):
        _write_basic(self.root)
        scan = [{"file": "src/unit/Def.cpp", "line": 2,
                 "name": "SchedList_DropItem", "kind": "decl", "self_style": True}]
        data = plan_all(self.root, scan)
        self.assertEqual(data["summary"]["clean"], 1)
        row = data["candidates"][0]
        self.assertEqual(row["classification"], "CLEAN")
        self.assertEqual(row["class"], "SLList")
        self.assertEqual(row["proposed_method"], "DropItem")
        self.assertTrue(row["mangled"].startswith("DropItem__"))

    def test_plan_all_blocked_unknown(self):
        _write_basic(self.root)
        scan = [{"file": "src/unit/Def.cpp", "line": 1,
                 "name": "Nope_Nothing", "kind": "decl", "self_style": True}]
        data = plan_all(self.root, scan)
        self.assertTrue(data["candidates"][0]["classification"].startswith("blocked:"))


class CompareSnapshotsTest(unittest.TestCase):
    def _rec(self, sym, size=0x40, mis=0):
        return {"symbol": sym, "retail_size": size,
                "decomp_size": size, "mismatch": mis}

    def test_size_change_detected(self):
        before = {0x1000: self._rec("Old")}
        after = {0x1000: self._rec("Old", size=0x44)}
        ok, diffs = compare_snapshots(before, after, {})
        self.assertFalse(ok)
        self.assertTrue(any("size" in d for d in diffs))

    def test_match_change_detected(self):
        before = {0x1000: self._rec("Old", mis=0)}
        after = {0x1000: self._rec("Old", mis=4)}
        ok, diffs = compare_snapshots(before, after, {})
        self.assertFalse(ok)
        self.assertTrue(diffs)

    def test_name_only_passes(self):
        before = {0x1000: self._rec("Old")}
        after = {0x1000: self._rec("New")}
        ok, diffs = compare_snapshots(before, after, {"Old": "New"})
        self.assertTrue(ok)
        self.assertEqual(diffs, [])

    def test_unexpected_rename_fails(self):
        before = {0x1000: self._rec("Old")}
        after = {0x1000: self._rec("Other")}
        ok, _ = compare_snapshots(before, after, {})
        self.assertFalse(ok)

    def test_extra_address_fails(self):
        before = {0x1000: self._rec("Old")}
        after = {0x1000: self._rec("Old"), 0x2000: self._rec("Extra")}
        ok, _ = compare_snapshots(before, after, {})
        self.assertFalse(ok)


class BackupRestoreTest(unittest.TestCase):
    def test_roundtrip_byte_identical(self):
        with TemporaryDirectory() as td:
            root = Path(td)
            f = root / "a" / "b.txt"
            f.parent.mkdir(parents=True)
            f.write_bytes(b"hello\x00world")
            stamp = "t1"
            bdir = mb.backup_files(root, [f], stamp)
            f.write_bytes(b"corrupted")
            mb.restore_files(bdir, root, [f])
            self.assertEqual(f.read_bytes(), b"hello\x00world")


class DryRunTest(unittest.TestCase):
    def test_plan_writes_nothing_without_flags(self):
        with TemporaryDirectory() as td:
            root = Path(td)
            _write_basic(root)
            before = {p: p.read_bytes() for p in root.rglob("*") if p.is_file()}
            scan = [{"file": "src/unit/Def.cpp", "line": 2,
                     "name": "SchedList_DropItem", "kind": "decl", "self_style": True}]
            data = plan_all(root, scan)
            self.assertEqual(data["summary"]["clean"], 1)
            after = {p: p.read_bytes() for p in root.rglob("*") if p.is_file()}
            self.assertEqual(before, after)

    def test_apply_dry_run_writes_nothing(self):
        with TemporaryDirectory() as td:
            root = Path(td)
            _write_basic(root)
            plan_data = plan_all(root, [{"file": "src/unit/Def.cpp", "line": 2,
                                         "name": "SchedList_DropItem", "kind": "decl",
                                         "self_style": True}])
            pj = root / "plan.json"
            pj.write_text(json.dumps(plan_data), encoding="utf-8")
            before = {p: p.read_bytes() for p in root.rglob("*") if p.is_file()}
            ns = argparse.Namespace(plan_json=str(pj), candidate=[], unit=None,
                                    limit=None, execute=False, dry_run=True,
                                    json=None, report=None,
                                    _snapshotter=None, _applier=None)
            rc = mb.cmd_apply(ns, root)
            self.assertEqual(rc, 0)
            after = {p: p.read_bytes() for p in root.rglob("*") if p.is_file()}
            # plan.json itself was written before; cmd_apply dry-run must not
            # touch anything else (no new files besides possibly --json).
            self.assertEqual(before, after)


class BatchingAndRevertTest(unittest.TestCase):
    def _two_unit_fixture(self, root: Path):
        for unit_dir, old in (("unitA", "Alpha_Do"), ("unitB", "Beta_Do")):
            cls = "Alpha" if unit_dir == "unitA" else "Beta"
            (root / "config" / "us").mkdir(parents=True, exist_ok=True)
            (root / "src" / unit_dir).mkdir(parents=True, exist_ok=True)
            (root / "config" / "us" / "symbols.txt").write_text(
                "Alpha_Do = .text:0x80001000;\nBeta_Do = .text:0x80002000;\n",
                encoding="utf-8")
            (root / "config" / "eu").mkdir(parents=True, exist_ok=True)
            (root / "config" / "eu" / "symbols.txt").write_text(
                "Alpha_Do = .text:0x80001000;\nBeta_Do = .text:0x80002000;\n",
                encoding="utf-8")
            (root / "src" / unit_dir / "Owner.hpp").write_text(
                f"#pragma once\nclass {cls} {{\npublic:\n    void Existing();\n}};\n",
                encoding="utf-8")
            (root / "src" / unit_dir / "Def.cpp").write_text(
                f'#include "Owner.hpp"\nextern "C" void {old}({cls}* self, int x);\n'
                f'void {old}({cls}* self, int x) {{ (void)self; }}\n',
                encoding="utf-8")
        (root / "configure.py").write_text("// stub\n", encoding="utf-8")

    def test_per_unit_isolation_and_revert(self):
        with TemporaryDirectory() as td:
            root = Path(td)
            self._two_unit_fixture(root)
            scan = [
                {"file": "src/unitA/Def.cpp", "line": 2, "name": "Alpha_Do",
                 "kind": "decl", "self_style": True},
                {"file": "src/unitB/Def.cpp", "line": 2, "name": "Beta_Do",
                 "kind": "decl", "self_style": True},
            ]
            plan_data = plan_all(root, scan)
            self.assertEqual(plan_data["summary"]["clean"], 2)
            pj = root / "plan.json"
            pj.write_text(json.dumps(plan_data), encoding="utf-8")
            units = sorted(r["unit"] for r in plan_data["candidates"])
            # units fall back to parent dirs (no objdiff.json): src/unitA, src/unitB.
            self.assertEqual(len(set(units)), 2)
            # Snapshotter: PASS for unitA (name-only), FAIL for unitB (size).
            states: dict = {}
            def snap_fn(proot: Path, unit: str):
                if unit not in states:
                    # baseline
                    if "unitA" in unit:
                        states[unit] = {0x1000: {"symbol": "Alpha_Do",
                                                 "retail_size": 0x40, "decomp_size": 0x40,
                                                 "mismatch": 0}}
                    else:
                        states[unit] = {0x2000: {"symbol": "Beta_Do",
                                                 "retail_size": 0x40, "decomp_size": 0x40,
                                                 "mismatch": 0}}
                    return dict(states[unit])
                # post-apply
                if "unitA" in unit:
                    return {0x1000: {"symbol": "Do__5AlphaFv",
                                     "retail_size": 0x40, "decomp_size": 0x40,
                                     "mismatch": 0}}
                return {0x2000: {"symbol": "Beta_Do",
                                 "retail_size": 0x48, "decomp_size": 0x48,
                                 "mismatch": 0}}
            # Applier: no-op (do not touch files); gate alone decides.
            ns = argparse.Namespace(
                plan_json=str(pj), candidate=[], unit=None, limit=None,
                execute=True, dry_run=False, json=None, report=None,
                _snapshotter=snap_fn, _applier=lambda *a: None, _no_post=True)
            # Need two calls per unit (before+after): track call counts.
            calls: dict[str, int] = {}
            orig_snap = snap_fn
            def counting_snap(proot: Path, unit: str, aliases=None):
                calls[unit] = calls.get(unit, 0) + 1
                if calls[unit] == 1:
                    if "unitA" in unit:
                        return {0x1000: {"symbol": "Alpha_Do", "retail_size": 0x40,
                                         "decomp_size": 0x40, "mismatch": 0}}
                    return {0x2000: {"symbol": "Beta_Do", "retail_size": 0x40,
                                     "decomp_size": 0x40, "mismatch": 0}}
                if "unitA" in unit:
                    return {0x1000: {"symbol": "Do__5AlphaFv", "retail_size": 0x40,
                                     "decomp_size": 0x40, "mismatch": 0}}
                return {0x2000: {"symbol": "Beta_Do", "retail_size": 0x48,
                                 "decomp_size": 0x48, "mismatch": 0}}
            ns._snapshotter = counting_snap
            # Fix renames to match our fake post symbols.
            for r in plan_data["candidates"]:
                if r["old"] == "Alpha_Do":
                    r["mangled"] = "Do__5AlphaFv"
            pj.write_text(json.dumps(plan_data), encoding="utf-8")
            before_bytes = {(p): p.read_bytes() for p in root.rglob("*") if p.is_file()}
            rc = mb.cmd_apply(ns, root)
            self.assertEqual(rc, 1)  # one FAIL -> nonzero
            # unitA PASS, unitB FAIL w/ revert (files byte-identical).
            after_bytes = {(p): p.read_bytes() for p in root.rglob("*") if p.is_file()}
            self.assertEqual(before_bytes, after_bytes)


class UnitForFileTest(unittest.TestCase):
    def test_fallback_parent(self):
        with TemporaryDirectory() as td:
            root = Path(td)
            self.assertEqual(unit_for_file(root, "src/unitA/Def.cpp"), "src/unitA")


# ---------------------------------------------------------------------------
# DEFECT 1: address-keyed, rename-aware gate (first live wave: 9 units
# wrongly reverted when a successful rename read as "old symbol vanished").
# ---------------------------------------------------------------------------


def _gate_rec(symbol, decomp_symbol=None, retail=0x40, decomp=None,
              mis=0, present=True):
    return {
        "symbol": symbol,
        "decomp_symbol": decomp_symbol if decomp_symbol is not None else symbol,
        "present": present,
        "retail_size": retail,
        "decomp_size": decomp if decomp is not None else retail,
        "mismatch": mis,
    }


class AddressKeyedGateTest(unittest.TestCase):
    def test_rename_pairing_passes(self):
        """Same address, Old -> planned New, identical sizes: PASS."""
        from tools.coop.memberize_bulk import compare_snapshots
        before = {0xA1EC: _gate_rec("ShowCellIcon")}
        after = {0xA1EC: _gate_rec("ShowCellIcon",
                                   decomp_symbol="Icon__8CellMapFv")}
        ok, diffs = compare_snapshots(
            before, after, {"ShowCellIcon": "Icon__8CellMapFv"})
        self.assertTrue(ok, diffs)
        self.assertEqual(diffs, [])

    def test_rename_with_size_change_still_fails(self):
        """ColiProc_InitQuad-style: real codegen delta under a rename FAILS."""
        from tools.coop.memberize_bulk import compare_snapshots
        before = {0x1000: _gate_rec("ColiProc_InitQuad", retail=24, mis=0)}
        after = {0x1000: _gate_rec("ColiProc_InitQuad",
                                   decomp_symbol="InitQuad__10CColiProcFv",
                                   retail=24, decomp=40, mis=10)}
        ok, diffs = compare_snapshots(
            before, after,
            {"ColiProc_InitQuad": "InitQuad__10CColiProcFv"})
        self.assertFalse(ok)
        self.assertTrue(any("decomp_size" in d for d in diffs), diffs)
        self.assertTrue(any("mismatch" in d for d in diffs), diffs)

    def test_planned_rename_not_vanished(self):
        """Old name absent post-apply but planned New at same address: PASS."""
        from tools.coop.memberize_bulk import compare_snapshots
        before = {0x3E5C: {"symbol": "EvtSeqProbeEvtSlot",
                           "retail_size": 908, "decomp_size": 908,
                           "mismatch": 0}}
        after = {0x3E5C: {"symbol": "ProbeEvtSlot__7EvtSeqFv",
                          "retail_size": 908, "decomp_size": 908,
                          "mismatch": 0}}
        ok, diffs = compare_snapshots(
            before, after,
            {"EvtSeqProbeEvtSlot": "ProbeEvtSlot__7EvtSeqFv"})
        self.assertTrue(ok, diffs)

    def test_unexplained_extra_address_fails(self):
        from tools.coop.memberize_bulk import compare_snapshots
        before = {0x1000: _gate_rec("Old")}
        after = {0x1000: _gate_rec("Old"),
                 0x2000: _gate_rec("Mystery")}
        ok, diffs = compare_snapshots(before, after, {})
        self.assertFalse(ok)
        self.assertTrue(any("extra" in d for d in diffs), diffs)

    def test_unexplained_missing_address_fails(self):
        from tools.coop.memberize_bulk import compare_snapshots
        before = {0x1000: _gate_rec("Old"),
                  0x2000: _gate_rec("Gone")}
        after = {0x1000: _gate_rec("Old")}
        ok, diffs = compare_snapshots(before, after, {})
        self.assertFalse(ok)
        self.assertTrue(any("missing" in d for d in diffs), diffs)

    def test_unobserved_planned_rename_fails(self):
        """A rename that took no effect anywhere must not silently PASS."""
        from tools.coop.memberize_bulk import compare_snapshots
        before = {0x1000: _gate_rec("Old")}
        after = {0x1000: _gate_rec("Old")}
        ok, diffs = compare_snapshots(
            before, after, {"Old": "New__5ClsFv"})
        self.assertFalse(ok)
        self.assertTrue(any("not observed" in d for d in diffs), diffs)


class ShowCellIconFixtureDemoTest(unittest.TestCase):
    """Repro of the exact first-wave false negative, now passing.

    Live reason was::

        0xa1ec ShowCellIcon: retail_size 1392 -> 0; ...
        0xa1ec ShowCellIcon: decomp_size 1392 -> 0; ...

    -- i.e. post-rename, hexdiff's name-based pairing lost the decomp
    function and the snapshot zero-filled the sizes.  The fixed snapshot
    keeps ``None`` (unpaired, never zero) and pairs the planned rename via
    ``--pair-alias``; the gate then sees a name-only change with identical
    sizes and PASSES.
    """
    ADDR = 0xA1EC
    OLD = "ShowCellIcon"
    NEW = "Icon__8CellMapFv"
    SIZE = 1392

    def _payload(self, paired):
        fn = {"symbol": self.OLD, "present": paired}
        if paired:
            fn.update({"decomp_symbol": self.NEW, "match": True,
                       "mismatch": 0, "retail_size": self.SIZE,
                       "decomp_size": self.SIZE})
        return {"unit": "u", "functions": [fn],
                "matched": 1 if paired else 0, "total": 1}

    def _list_out(self):
        return f"  0x{self.ADDR:08x}  0x{self.SIZE:x}  {self.OLD}\n"

    def _run_snapshot(self, paired):
        from unittest import mock
        seen_argv: list = []

        def fake_run(argv, **kwargs):
            if "--all" in argv:
                seen_argv.append(list(argv))
                m = mock.Mock()
                m.returncode = 0
                m.stdout = json.dumps(self._payload(paired))
                m.stderr = ""
                return m
            m = mock.Mock()
            m.returncode = 0
            m.stdout = self._list_out()
            m.stderr = ""
            return m

        with TemporaryDirectory() as td, \
                mock.patch.object(mb, "subprocess") as msub:
            msub.run.side_effect = fake_run
            snap = mb.snapshot_from_hexdiff(
                Path(td), "someUnit", {self.OLD: self.NEW})
        return snap, seen_argv

    def test_alias_flag_forwarded(self):
        _, seen = self._run_snapshot(paired=True)
        self.assertTrue(seen)
        self.assertIn(f"{self.OLD}={self.NEW}",
                      " ".join(seen[0]), seen)

    def test_unpaired_post_snapshot_is_not_zero_filled(self):
        """Without pairing (the old failure mode) sizes stay None."""
        snap, _ = self._run_snapshot(paired=False)
        rec = snap[self.ADDR]
        self.assertFalse(rec["present"])
        self.assertIsNone(rec["retail_size"])
        self.assertIsNone(rec["decomp_size"])

    def test_showcellicon_now_passes(self):
        from tools.coop.memberize_bulk import compare_snapshots
        snap_after, _ = self._run_snapshot(paired=True)
        before = {self.ADDR: _gate_rec(
            self.OLD, decomp_symbol=self.OLD,
            retail=self.SIZE, mis=0)}
        ok, diffs = compare_snapshots(
            before, snap_after, {self.OLD: self.NEW})
        self.assertTrue(ok, diffs)
        self.assertEqual(diffs, [])


# ---------------------------------------------------------------------------
# DEFECT 2: primitive self types are BLOCKED, never CLEAN.
# ---------------------------------------------------------------------------


def _write_char_self_fixture(root: Path, *, old="getEntryPtrGrid",
                             self_type="char") -> None:
    (root / "config" / "us").mkdir(parents=True, exist_ok=True)
    (root / "config" / "eu").mkdir(parents=True, exist_ok=True)
    (root / "src" / "unit").mkdir(parents=True, exist_ok=True)
    (root / "src" / "other").mkdir(parents=True, exist_ok=True)
    (root / "config" / "us" / "symbols.txt").write_text(
        f"{old} = .text:0x80001000;\n", encoding="utf-8")
    (root / "config" / "eu" / "symbols.txt").write_text(
        f"{old} = .text:0x80001000;\n", encoding="utf-8")
    (root / "src" / "unit" / "Def.cpp").write_text(
        f'extern "C" void {old}({self_type}* self, int x);\n'
        f'void {old}({self_type}* self, int x) {{ (void)self; }}\n',
        encoding="utf-8")
    (root / "src" / "other" / "Caller.cpp").write_text(
        f"void run() {{ {old}(a, b); }}\n", encoding="utf-8")
    (root / "configure.py").write_text("// stub\n", encoding="utf-8")


class PrimitiveSelfTypeTest(unittest.TestCase):
    def test_propose_refuses_primitives(self):
        cases = ["char", "int", "u8", "u16", "u32", "u64",
                 "s8", "s16", "s32", "s64", "f32", "f64",
                 "void", "bool", "float", "double",
                 "short", "long", "unsigned", "signed",
                 "unsigned int", "const char*", "u16"]
        for cls in cases:
            with self.subTest(cls=cls):
                self.assertIsNone(
                    propose_method_name("getEntryPtrGrid", cls),
                    f"primitive self type {cls!r} must be refused")
        # Real classes still propose.
        self.assertEqual(
            propose_method_name("CfRes_getEntryPtrAt", "CfRes"),
            "getEntryPtrAt")

    def test_plan_all_blocks_primitive_char(self):
        with TemporaryDirectory() as td:
            root = Path(td)
            _write_char_self_fixture(root, old="getEntryPtrGrid",
                                     self_type="char")
            scan = [{"file": "src/unit/Def.cpp", "line": 2,
                     "name": "getEntryPtrGrid", "kind": "decl",
                     "self_style": True}]
            data = plan_all(root, scan)
            self.assertEqual(data["summary"]["clean"], 0)
            row = data["candidates"][0]
            self.assertTrue(
                row["classification"].startswith("blocked:"),
                row["classification"])
            self.assertIn("primitive", row["classification"])
            self.assertIsNone(row["proposed_method"])
            self.assertIsNone(row["mangled"])

    def test_plan_all_blocks_unsigned_int_typedef(self):
        # Multi-word self types never reach the primitive guard (the
        # planner's single-token self regex refuses them first), but they
        # must still be BLOCKED, never CLEAN.
        with TemporaryDirectory() as td:
            root = Path(td)
            _write_char_self_fixture(root, old="CfRes_getEntryPtrAt",
                                     self_type="unsigned int")
            scan = [{"file": "src/unit/Def.cpp", "line": 2,
                     "name": "CfRes_getEntryPtrAt", "kind": "decl",
                     "self_style": True}]
            data = plan_all(root, scan)
            self.assertEqual(data["summary"]["clean"], 0)
            row = data["candidates"][0]
            self.assertTrue(row["classification"].startswith("blocked:"),
                            row["classification"])
            # And the guard itself fires on the multi-word spelling.
            self.assertIsNone(
                propose_method_name("CfRes_getEntryPtrAt", "unsigned int"))


# ---------------------------------------------------------------------------
# Single-unit retry: --unit dry-run touches only that unit.
# ---------------------------------------------------------------------------


class UnitRetryDryRunTest(unittest.TestCase):
    def _two_unit_plan(self, root: Path) -> Path:
        for unit_dir, old, cls in (("unitA", "Alpha_Do", "Alpha"),
                                  ("unitB", "Beta_Do", "Beta")):
            (root / "config" / "us").mkdir(parents=True, exist_ok=True)
            (root / "config" / "eu").mkdir(parents=True, exist_ok=True)
            (root / "src" / unit_dir).mkdir(parents=True, exist_ok=True)
            (root / "src" / unit_dir / "Owner.hpp").write_text(
                f"#pragma once\nclass {cls} {{\npublic:\n"
                f"    void Existing();\n}};\n", encoding="utf-8")
            (root / "src" / unit_dir / "Def.cpp").write_text(
                f'#include "Owner.hpp"\nextern "C" void {old}({cls}* self, int x);\n'
                f'void {old}({cls}* self, int x) {{ (void)self; }}\n',
                encoding="utf-8")
        (root / "config" / "us" / "symbols.txt").write_text(
            "Alpha_Do = .text:0x80001000;\nBeta_Do = .text:0x80002000;\n",
            encoding="utf-8")
        (root / "config" / "eu" / "symbols.txt").write_text(
            "Alpha_Do = .text:0x80001000;\nBeta_Do = .text:0x80002000;\n",
            encoding="utf-8")
        (root / "configure.py").write_text("// stub\n", encoding="utf-8")
        scan = [
            {"file": "src/unitA/Def.cpp", "line": 2, "name": "Alpha_Do",
             "kind": "decl", "self_style": True},
            {"file": "src/unitB/Def.cpp", "line": 2, "name": "Beta_Do",
             "kind": "decl", "self_style": True},
        ]
        plan_data = plan_all(root, scan)
        self.assertEqual(plan_data["summary"]["clean"], 2)
        pj = root / "plan.json"
        pj.write_text(json.dumps(plan_data), encoding="utf-8")
        return pj

    def test_unit_retry_dry_run_only_touches_that_unit(self):
        import argparse
        with TemporaryDirectory() as td:
            root = Path(td)
            pj = self._two_unit_plan(root)
            before = {p: p.read_bytes() for p in root.rglob("*") if p.is_file()}
            out_json = root / "dry.json"
            ns = argparse.Namespace(plan_json=str(pj), candidate=[],
                                    unit="src/unitA", limit=None,
                                    execute=False, dry_run=True,
                                    json=str(out_json), report=None,
                                    _snapshotter=None, _applier=None)
            rc = mb.cmd_apply(ns, root)
            self.assertEqual(rc, 0)
            report = json.loads(out_json.read_text(encoding="utf-8"))
            self.assertTrue(report["dry_run"])
            self.assertEqual(report["units"], ["src/unitA"])
            after = {p: p.read_bytes() for p in root.rglob("*") if p.is_file()
                     if p.name != "dry.json"}
            self.assertEqual(before, after)


# ---------------------------------------------------------------------------
# LIVE second-wave regression: address-keyed join + nm-verified NEW.
#
# First-wave mocks used bare source names on both sides (Old -> New) and
# therefore never exercised the two shapes that failed live 0/26:
#   (a) ShowCellIcon: rename unpaired post-apply (alias key/NEW mismatch),
#   (b) EvtSeqProbeEvtSlot__FP... -> SeqProbe... with
#       ``planned: EvtSeqProbeEvtSlot__FP... -> ?`` (source-keyed gate miss).
# These tests feed the EXACT live shapes through the REAL
# ``build_address_keyed_maps`` + ``compare_snapshots`` path (no hexdiff
# mock for the join/gate logic) and assert PASS -- plus an end-to-end
# ``cmd_apply`` run with live-shaped snapshots.  A mock-only test is what
# let this bug through; these shapes are the live contract.
# ---------------------------------------------------------------------------


def _write_live_evt_fixture(root: Path) -> None:
    """Fixture whose class/body matches the live EvtSeqProbeEvtSlot shape."""
    (root / "config" / "us").mkdir(parents=True, exist_ok=True)
    (root / "config" / "eu").mkdir(parents=True, exist_ok=True)
    (root / "src" / "unit").mkdir(parents=True, exist_ok=True)
    (root / "src" / "other").mkdir(parents=True, exist_ok=True)
    old = "EvtSeqProbeEvtSlot"
    (root / "config" / "us" / "symbols.txt").write_text(
        f"{old} = .text:0x80001000;\n", encoding="utf-8")
    (root / "config" / "eu" / "symbols.txt").write_text(
        f"{old} = .text:0x80001000;\n", encoding="utf-8")
    (root / "src" / "unit" / "Owner.hpp").write_text(
        "#pragma once\nclass EvtBdabModel {\npublic:\n"
        "    void Existing();\n};\n",
        encoding="utf-8")
    (root / "src" / "unit" / "Def.cpp").write_text(
        '#include "Owner.hpp"\n'
        f'extern "C" void {old}(EvtBdabModel* self, unsigned int* out);\n'
        f'void {old}(EvtBdabModel* self, unsigned int* out) {{\n'
        '    (void)self; (void)out;\n'
        '}\n', encoding="utf-8")
    (root / "src" / "other" / "Caller.cpp").write_text(
        f'#include "Owner.hpp"\nvoid run() {{ {old}(a, b); }}\n',
        encoding="utf-8")
    (root / "configure.py").write_text("// stub\n", encoding="utf-8")


def _live_rec(symbol, decomp_symbol=None, retail=0x40, decomp=None,
              mis=0, present=True):
    return {
        "symbol": symbol,
        "decomp_symbol": decomp_symbol if decomp_symbol is not None else symbol,
        "present": present,
        "retail_size": retail,
        "decomp_size": decomp if decomp is not None else retail,
        "mismatch": mis,
    }


class LiveShapeRegressionTest(unittest.TestCase):
    """Exact live failure shapes must PASS after the address-keyed fix."""

    def test_shape_a_showcellicon_pairs(self):
        """(a) 0xa1ec ShowCellIcon rename unpaired -- now pairs via alias."""
        from tools.coop.memberize_bulk import (
            build_address_keyed_maps, compare_snapshots,
        )
        OLD = "ShowCellIcon"
        NEW = "ShowCellIcon__16CItemBoxGridFullFUiPvUsUi"
        ADDR = 0xA1EC
        SIZE = 1392
        before = {ADDR: _live_rec(OLD, OLD, retail=SIZE, mis=0)}
        # Post-apply: retail spelling unchanged, decomp paired via alias.
        after = {ADDR: _live_rec(OLD, NEW, retail=SIZE, mis=0)}
        rows = [{"old": OLD, "mangled": NEW}]
        aliases, gate, addr_map, unresolved = build_address_keyed_maps(
            before, rows)
        self.assertEqual(unresolved, [])
        # Alias key MUST be the retail/--list spelling hexdiff expects.
        self.assertEqual(aliases, {OLD: NEW})
        self.assertIn(ADDR, addr_map)
        ok, diffs = compare_snapshots(before, after, gate)
        self.assertTrue(ok, diffs)
        self.assertEqual(diffs, [])

    def test_shape_b_evtseq_mangled_old_pairs(self):
        """(b) 0x3e5c unexpected rename (planned: ... -> ?) -- now keyed."""
        from tools.coop.memberize_bulk import (
            build_address_keyed_maps, compare_snapshots,
        )
        SRC_OLD = "EvtSeqProbeEvtSlot"
        MANGLED_OLD = "EvtSeqProbeEvtSlot__FP12EvtBdabModelPUl"
        NEW = "SeqProbeEvtSlot__12EvtBdabModelFPUi"
        ADDR = 0x3E5C
        SIZE = 908
        # Live before: retail source spelling, decomp MWCC-mangled old.
        before = {ADDR: _live_rec(SRC_OLD, MANGLED_OLD, retail=SIZE, mis=0)}
        # Live after: same retail, decomp NEW (paired via alias).
        after = {ADDR: _live_rec(SRC_OLD, NEW, retail=SIZE, mis=0)}
        rows = [{"old": SRC_OLD, "mangled": NEW}]
        aliases, gate, addr_map, unresolved = build_address_keyed_maps(
            before, rows)
        self.assertEqual(unresolved, [])
        # Alias key is the RETAIL spelling (source here); gate key is the
        # BEFORE-IDENT (decomp-preferred mangled old) -- the old code keyed
        # both by source and missed (``planned: ... -> ?``).
        self.assertEqual(aliases, {SRC_OLD: NEW})
        self.assertEqual(gate, {MANGLED_OLD: NEW})
        # The OLD (buggy) raw renames dict must FAIL on these shapes --
        # this is the exact live message; it proves the test is real.
        ok_raw, diffs_raw = compare_snapshots(
            before, after, {SRC_OLD: NEW})
        self.assertFalse(ok_raw)
        self.assertTrue(
            any("unexpected rename" in d and "-> ?" in d for d in diffs_raw),
            diffs_raw)
        # The fixed address-keyed gate must PASS.
        ok, diffs = compare_snapshots(before, after, gate)
        self.assertTrue(ok, diffs)
        self.assertEqual(diffs, [])

    def test_shape_b_hikari_prefix_strip_pairs(self):
        """hikariSetEnableBit0 (source) vs mangled old decomp pairs."""
        from tools.coop.memberize_bulk import (
            build_address_keyed_maps, compare_snapshots,
        )
        SRC = "hikariSetEnableBit0"
        MANGLED_OLD = "hikariSetEnableBit0__FP18CfHikariItemRecordUl"
        NEW = "SetEnableBit0__18CfHikariItemRecordFUi"
        ADDR = 0x484
        before = {ADDR: _live_rec(SRC, MANGLED_OLD, retail=0x28, mis=0)}
        after = {ADDR: _live_rec(SRC, NEW, retail=0x28, mis=0)}
        rows = [{"old": SRC, "mangled": NEW}]
        aliases, gate, _, unresolved = build_address_keyed_maps(before, rows)
        self.assertEqual(unresolved, [])
        self.assertEqual(aliases, {SRC: NEW})
        self.assertEqual(gate, {MANGLED_OLD: NEW})
        ok, diffs = compare_snapshots(before, after, gate)
        self.assertTrue(ok, diffs)

    def test_real_delta_still_fails(self):
        """ColiProc_InitQuad decomp_size 24 -> 40 must STILL FAIL."""
        from tools.coop.memberize_bulk import (
            build_address_keyed_maps, compare_snapshots,
        )
        OLD, NEW, ADDR = "ColiProc_InitQuad", "InitQuad__9CColiProcFv", 0x2C
        before = {ADDR: _live_rec(OLD, OLD, retail=24, mis=0)}
        after = {ADDR: _live_rec(OLD, NEW, retail=24, decomp=40, mis=10)}
        rows = [{"old": OLD, "mangled": NEW}]
        _, gate, _, _ = build_address_keyed_maps(before, rows)
        ok, diffs = compare_snapshots(before, after, gate)
        self.assertFalse(ok)
        self.assertTrue(any("decomp_size" in d for d in diffs), diffs)
        self.assertTrue(any("mismatch" in d for d in diffs), diffs)

    def test_cmd_apply_end_to_end_live_shapes(self):
        """cmd_apply with live-shaped snapshots must PASS the gate."""
        import argparse
        with TemporaryDirectory() as td:
            root = Path(td)
            _write_live_evt_fixture(root)
            # Plan row uses the live source old + planned NEW.
            NEW = "SeqProbeEvtSlot__12EvtBdabModelFPUi"
            MANGLED_OLD = "EvtSeqProbeEvtSlot__FP12EvtBdabModelPUl"
            plan_data = {
                "candidates": [{
                    "old": "EvtSeqProbeEvtSlot", "file": "src/unit/Def.cpp",
                    "line": 2, "class": "EvtBdabModel",
                    "proposed_method": "SeqProbeEvtSlot", "mangled": NEW,
                    "classification": "CLEAN", "unit": "src/unit",
                    "no_header_insert": False,
                }],
                "summary": {"total": 1, "clean": 1,
                              "needs_review": 0, "blocked": 0, "by_unit": {}},
            }
            pj = root / "plan.json"
            pj.write_text(json.dumps(plan_data), encoding="utf-8")
            ADDR = 0x3E5C
            SIZE = 908
            calls: list[str] = []

            def live_snap(_root: Path, _unit: str, aliases=None):
                calls.append(_unit)
                if len(calls) == 1:
                    # Baseline: retail source, decomp mangled old.
                    return {ADDR: _live_rec("EvtSeqProbeEvtSlot",
                                            MANGLED_OLD, retail=SIZE, mis=0)}
                # Post: retail source, decomp NEW (alias paired).
                # The fixed driver passes retail-keyed aliases; assert it.
                self.assertIn("EvtSeqProbeEvtSlot", (aliases or {}))
                return {ADDR: _live_rec("EvtSeqProbeEvtSlot", NEW,
                                         retail=SIZE, mis=0)}

            ns = argparse.Namespace(
                plan_json=str(pj), candidate=[], unit=None, limit=None,
                execute=True, dry_run=False, json=None, report=None,
                _snapshotter=live_snap, _applier=lambda *a: None,
                _no_post=True)
            rc = mb.cmd_apply(ns, root)
            self.assertEqual(rc, 0)
            self.assertEqual(len(calls), 2)


if __name__ == "__main__":
    unittest.main()
