"""Tests for the US-only default and the repo-wide symbols-edit lock.

Two properties are pinned here:

1. ``symbol_fn_rename`` edits ``config/us/symbols.txt`` by DEFAULT.  The other
   regions are only touched when the caller explicitly asks for them -- rewriting
   all three regions on every rename is what produced a multi-region skew (and
   a truncated ``symbols.txt``).
2. ``tools/coop/lib/symbols_lock.py`` serialises concurrent editors, so two
   read-modify-write cycles can never interleave.
"""
from __future__ import annotations

import threading
import time
import unittest
from pathlib import Path
from tempfile import TemporaryDirectory

from tools.coop.lib.symbols_lock import symbols_edit_lock
from tools.coop.symbol_fn_rename import (
    DEFAULT_REGIONS,
    REGIONS,
    apply_function_rename,
    plan_function_rename,
)
from tools.coop.tests.test_symbol_rename_function import _write_fixture_tree


class RegionDefaultTest(unittest.TestCase):
    def setUp(self) -> None:
        self.tmp = TemporaryDirectory()
        self.root = Path(self.tmp.name)
        _write_fixture_tree(self.root)

    def tearDown(self) -> None:
        self.tmp.cleanup()

    def test_default_regions_is_us_only(self) -> None:
        self.assertEqual(DEFAULT_REGIONS, ("us",))
        # us must remain a known region; the known set is unchanged.
        self.assertIn("us", REGIONS)

    def test_plan_touches_only_us_by_default(self) -> None:
        plan = plan_function_rename(self.root, "Widget_Paint", "WdgState::Paint(int, int)")
        self.assertEqual([e.region for e in plan.symbol_edits], ["us"])

    def test_apply_leaves_other_regions_byte_identical(self) -> None:
        eu = self.root / "config" / "eu" / "symbols.txt"
        jp = self.root / "config" / "jp" / "symbols.txt"
        before_eu = eu.read_bytes() if eu.is_file() else None
        before_jp = jp.read_bytes() if jp.is_file() else None
        apply_function_rename(
            self.root, "Widget_Paint", "WdgState::Paint(int, int)",
            dry_run=False, allow_review=True,
        )
        after_eu = eu.read_bytes() if eu.is_file() else None
        after_jp = jp.read_bytes() if jp.is_file() else None
        self.assertEqual(before_eu, after_eu)
        self.assertEqual(before_jp, after_jp)
        us = (self.root / "config" / "us" / "symbols.txt").read_text(encoding="utf-8")
        self.assertIn("Paint__8WdgStateFii", us)

    def test_explicit_regions_still_work(self) -> None:
        plan = plan_function_rename(
            self.root, "Widget_Paint", "WdgState::Paint(int, int)",
            regions=["us", "eu"],
        )
        self.assertEqual(sorted(e.region for e in plan.symbol_edits), ["eu", "us"])

    def test_unknown_region_refused(self) -> None:
        with self.assertRaises(Exception):
            plan_function_rename(
                self.root, "Widget_Paint", "WdgState::Paint(int, int)",
                regions=["us", "nope"],
            )


class SymbolsLockTest(unittest.TestCase):
    def setUp(self) -> None:
        self.tmp = TemporaryDirectory()
        self.root = Path(self.tmp.name)

    def tearDown(self) -> None:
        self.tmp.cleanup()

    def test_creates_lock_file_in_build_dir(self) -> None:
        with symbols_edit_lock(self.root, timeout_s=5):
            self.assertTrue((self.root / "build" / ".symbols_edit.lock").is_file())

    def test_serialises_two_holders(self) -> None:
        """Two holders must never overlap."""
        order: list[str] = []
        overlap = {"seen": False}
        inside = {"n": 0}

        def worker(tag: str) -> None:
            with symbols_edit_lock(self.root, timeout_s=10):
                inside["n"] += 1
                if inside["n"] > 1:
                    overlap["seen"] = True
                order.append(f"enter-{tag}")
                time.sleep(0.15)
                order.append(f"exit-{tag}")
                inside["n"] -= 1

        threads = [threading.Thread(target=worker, args=(t,)) for t in ("a", "b")]
        for t in threads:
            t.start()
        for t in threads:
            t.join(timeout=20)
        self.assertFalse(overlap["seen"], f"holders overlapped: {order}")
        # Strict enter/exit pairing for both workers.
        self.assertEqual(len(order), 4)
        self.assertEqual(order[0][:5], "enter")
        self.assertEqual(order[1][:4], "exit")
        self.assertEqual(order[2][:5], "enter")
        self.assertEqual(order[3][:4], "exit")

    def test_timeout_when_held(self) -> None:
        hold = threading.Event()
        release = threading.Event()

        def holder() -> None:
            with symbols_edit_lock(self.root, timeout_s=10):
                hold.set()
                release.wait(timeout=10)

        t = threading.Thread(target=holder)
        t.start()
        try:
            self.assertTrue(hold.wait(timeout=10))
            with self.assertRaises(TimeoutError):
                with symbols_edit_lock(self.root, timeout_s=0.2):
                    pass
        finally:
            release.set()
            t.join(timeout=10)

    def test_reacquire_after_release(self) -> None:
        with symbols_edit_lock(self.root, timeout_s=5):
            pass
        with symbols_edit_lock(self.root, timeout_s=5):
            pass


class LegacyTypeRenameTest(unittest.TestCase):
    """The LEGACY type-rename path (tools/symbolrecover) gets the same two
    guarantees as the function path: US-only by default, and atomic + locked
    writes.  Also pins the re-entrancy that stops a nested lock self-deadlocking."""

    def setUp(self) -> None:
        self.tmp = TemporaryDirectory()
        self.root = Path(self.tmp.name)
        for region in ("us", "eu", "jp"):
            d = self.root / "config" / region
            d.mkdir(parents=True, exist_ok=True)
            (d / "symbols.txt").write_text(
                "OldType = .text:0x80000000; // type:function size:0x10\n",
                encoding="utf-8",
            )
        (self.root / "src").mkdir(exist_ok=True)

    def tearDown(self) -> None:
        self.tmp.cleanup()

    def test_plan_full_rename_defaults_to_us(self) -> None:
        from tools.symbolrecover.lib.rename_source import plan_full_rename

        plan = plan_full_rename(self.root, "us", "OldType", "NewType")
        self.assertEqual([p.parent.name for p in plan.symbol_files], ["us"])

    def test_apply_rename_touches_only_listed_regions(self) -> None:
        from tools.symbolrecover.lib.rename import apply_rename

        eu_before = (self.root / "config" / "eu" / "symbols.txt").read_bytes()
        jp_before = (self.root / "config" / "jp" / "symbols.txt").read_bytes()
        _n, touched = apply_rename(self.root, "OldType", "NewType", ["us"], dry_run=False)
        self.assertEqual([p.parent.name for p in touched], ["us"])
        self.assertEqual((self.root / "config" / "eu" / "symbols.txt").read_bytes(), eu_before)
        self.assertEqual((self.root / "config" / "jp" / "symbols.txt").read_bytes(), jp_before)
        self.assertIn("NewType", (self.root / "config" / "us" / "symbols.txt").read_text())

    def test_dry_run_creates_no_lock_file(self) -> None:
        from tools.symbolrecover.lib.rename import apply_rename

        apply_rename(self.root, "OldType", "NewType", ["us"], dry_run=True)
        self.assertFalse((self.root / "build" / ".symbols_edit.lock").is_file())

    def test_real_apply_creates_lock_file(self) -> None:
        from tools.symbolrecover.lib.rename import apply_rename

        apply_rename(self.root, "OldType", "NewType", ["us"], dry_run=False)
        self.assertTrue((self.root / "build" / ".symbols_edit.lock").is_file())

    def test_nested_lock_does_not_deadlock(self) -> None:
        """apply_full_rename holds the lock and calls apply_rename inside it."""
        from tools.symbolrecover.lib.rename import apply_rename

        with symbols_edit_lock(self.root, timeout_s=3):
            _n, touched = apply_rename(
                self.root, "OldType", "NewType", ["us"], dry_run=False
            )
        self.assertEqual([p.parent.name for p in touched], ["us"])


if __name__ == "__main__":
    unittest.main()
