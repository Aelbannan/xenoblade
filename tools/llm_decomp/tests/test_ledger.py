"""Tests for tools.llm_decomp.ledger.
"""
from __future__ import annotations

import json
import os
import re
import tempfile
import threading
import unittest
from pathlib import Path

from tools.llm_decomp.ledger import (
    STAGES,
    STATUS_COMPLETE,
    STATUS_IN_PROGRESS,
    STATUS_OVER_BUDGET,
    Ledger,
    LedgerError,
    UnitRecord,
)


class TestUnitRecord(unittest.TestCase):
    def test_defaults(self) -> None:
        r = UnitRecord(unit="kyoshin/CExchangeWin")
        self.assertEqual(r.unit, "kyoshin/CExchangeWin")
        self.assertEqual(r.status, STATUS_IN_PROGRESS)
        self.assertEqual(r.stages, {})
        self.assertEqual(r.sessions, [])
        self.assertEqual(r.notes, "")

    def test_from_dict(self) -> None:
        r = UnitRecord(
            unit="u",
            status=STATUS_COMPLETE,
            stages={"match": "2026-01-01T00:00:00Z"},
            sessions=["s1", "s2"],
            notes="done",
        )
        self.assertEqual(r.status, STATUS_COMPLETE)
        self.assertIn("match", r.stages)


class TestLedgerEmptyFile(unittest.TestCase):
    """Behaviour when the ledger file does not exist."""

    def setUp(self) -> None:
        self._tmp = tempfile.TemporaryDirectory()
        self.path = Path(self._tmp.name) / "tu_ledger.json"

    def tearDown(self) -> None:
        self._tmp.cleanup()

    def test_record_returns_none_for_missing_unit(self) -> None:
        ledger = Ledger(self.path)
        self.assertIsNone(ledger.record("kyoshin/CExchangeWin"))

    def test_is_complete_false_for_missing_unit(self) -> None:
        ledger = Ledger(self.path)
        self.assertFalse(ledger.is_complete("kyoshin/CExchangeWin"))

    def test_complete_units_empty(self) -> None:
        ledger = Ledger(self.path)
        self.assertEqual(ledger.complete_units(), [])


class TestMarkStage(unittest.TestCase):
    def setUp(self) -> None:
        self._tmp = tempfile.TemporaryDirectory()
        self.path = Path(self._tmp.name) / "tu_ledger.json"
        self.ledger = Ledger(self.path)

    def tearDown(self) -> None:
        self._tmp.cleanup()

    def test_mark_stage_stamps_iso_timestamp(self) -> None:
        self.ledger.mark_stage("kyoshin/CExchangeWin", "match", "sess-001")
        rec = self.ledger.record("kyoshin/CExchangeWin")
        assert rec is not None
        ts = rec.stages.get("match")
        self.assertIsNotNone(ts)
        # ISO-8601 with trailing Z
        self.assertRegex(ts, r"^\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}Z$")

    def test_mark_stage_appends_session_id(self) -> None:
        self.ledger.mark_stage("kyoshin/CExchangeWin", "match", "sess-001")
        rec = self.ledger.record("kyoshin/CExchangeWin")
        assert rec is not None
        self.assertIn("sess-001", rec.sessions)

    def test_mark_stage_no_duplicate_session(self) -> None:
        self.ledger.mark_stage("kyoshin/CExchangeWin", "match", "sess-001")
        self.ledger.mark_stage("kyoshin/CExchangeWin", "rename", "sess-001")
        rec = self.ledger.record("kyoshin/CExchangeWin")
        assert rec is not None
        self.assertEqual(rec.sessions, ["sess-001"])

    def test_mark_stage_multiple_sessions(self) -> None:
        self.ledger.mark_stage("kyoshin/CExchangeWin", "match", "sess-001")
        self.ledger.mark_stage("kyoshin/CExchangeWin", "rename", "sess-002")
        rec = self.ledger.record("kyoshin/CExchangeWin")
        assert rec is not None
        self.assertEqual(rec.sessions, ["sess-001", "sess-002"])

    def test_mark_stage_sets_status_in_progress(self) -> None:
        # First mark it complete, then mark a stage — should revert to in_progress
        self.ledger.set_status("kyoshin/CExchangeWin", STATUS_COMPLETE)
        self.ledger.mark_stage("kyoshin/CExchangeWin", "match", "sess-001")
        rec = self.ledger.record("kyoshin/CExchangeWin")
        assert rec is not None
        self.assertEqual(rec.status, STATUS_IN_PROGRESS)

    def test_mark_stage_invalid_stage_raises(self) -> None:
        with self.assertRaises(ValueError):
            self.ledger.mark_stage("kyoshin/CExchangeWin", "bogus", "sess-001")


class TestSetStatus(unittest.TestCase):
    def setUp(self) -> None:
        self._tmp = tempfile.TemporaryDirectory()
        self.path = Path(self._tmp.name) / "tu_ledger.json"
        self.ledger = Ledger(self.path)

    def tearDown(self) -> None:
        self._tmp.cleanup()

    def test_set_status_complete(self) -> None:
        self.ledger.set_status("kyoshin/CExchangeWin", STATUS_COMPLETE)
        self.assertTrue(self.ledger.is_complete("kyoshin/CExchangeWin"))

    def test_set_status_over_budget(self) -> None:
        self.ledger.set_status("kyoshin/CExchangeWin", STATUS_OVER_BUDGET)
        rec = self.ledger.record("kyoshin/CExchangeWin")
        assert rec is not None
        self.assertEqual(rec.status, STATUS_OVER_BUDGET)
        self.assertFalse(self.ledger.is_complete("kyoshin/CExchangeWin"))

    def test_set_status_in_progress(self) -> None:
        self.ledger.set_status("kyoshin/CExchangeWin", STATUS_IN_PROGRESS)
        rec = self.ledger.record("kyoshin/CExchangeWin")
        assert rec is not None
        self.assertEqual(rec.status, STATUS_IN_PROGRESS)

    def test_set_status_invalid_raises(self) -> None:
        with self.assertRaises(ValueError):
            self.ledger.set_status("kyoshin/CExchangeWin", "finished")
        with self.assertRaises(ValueError):
            self.ledger.set_status("kyoshin/CExchangeWin", "")
        with self.assertRaises(ValueError):
            self.ledger.set_status("kyoshin/CExchangeWin", "COMPLETE")

    def test_set_status_with_notes(self) -> None:
        self.ledger.set_status("kyoshin/CExchangeWin", STATUS_OVER_BUDGET, notes="Size trim exhausted")
        rec = self.ledger.record("kyoshin/CExchangeWin")
        assert rec is not None
        self.assertEqual(rec.notes, "Size trim exhausted")

    def test_set_status_empty_notes_does_not_overwrite(self) -> None:
        self.ledger.set_status("kyoshin/CExchangeWin", STATUS_OVER_BUDGET, notes="too big")
        self.ledger.set_status("kyoshin/CExchangeWin", STATUS_IN_PROGRESS)  # no notes arg
        rec = self.ledger.record("kyoshin/CExchangeWin")
        assert rec is not None
        self.assertEqual(rec.notes, "too big")  # unchanged


class TestCompleteUnits(unittest.TestCase):
    def setUp(self) -> None:
        self._tmp = tempfile.TemporaryDirectory()
        self.path = Path(self._tmp.name) / "tu_ledger.json"
        self.ledger = Ledger(self.path)

    def tearDown(self) -> None:
        self._tmp.cleanup()

    def test_complete_units_after_setting_multiple(self) -> None:
        self.ledger.set_status("u/a", STATUS_COMPLETE)
        self.ledger.set_status("u/b", STATUS_IN_PROGRESS)
        self.ledger.set_status("u/c", STATUS_COMPLETE)
        self.ledger.set_status("u/d", STATUS_OVER_BUDGET)
        self.assertCountEqual(self.ledger.complete_units(), ["u/a", "u/c"])


class TestRoundTripPersistence(unittest.TestCase):
    """Two ledger instances over the same file must see the same state."""

    def setUp(self) -> None:
        self._tmp = tempfile.TemporaryDirectory()
        self.path = Path(self._tmp.name) / "tu_ledger.json"

    def tearDown(self) -> None:
        self._tmp.cleanup()

    def test_write_then_read(self) -> None:
        w = Ledger(self.path)
        w.mark_stage("kyoshin/CExchangeWin", "match", "sess-001")
        w.set_status("kyoshin/CExchangeWin", STATUS_COMPLETE)

        r = Ledger(self.path)
        self.assertTrue(r.is_complete("kyoshin/CExchangeWin"))
        rec = r.record("kyoshin/CExchangeWin")
        assert rec is not None
        self.assertIn("match", rec.stages)
        self.assertIn("sess-001", rec.sessions)


class TestConcurrentThreads(unittest.TestCase):
    """8 threads calling mark_stage on the same file — no sessions lost."""

    def setUp(self) -> None:
        self._tmp = tempfile.TemporaryDirectory()
        self.path = Path(self._tmp.name) / "tu_ledger.json"
        self.ledger = Ledger(self.path)

    def tearDown(self) -> None:
        self._tmp.cleanup()

    def test_eight_threads_no_session_loss(self) -> None:
        n_threads = 8
        unit = "kyoshin/CExchangeWin"

        def _mark(i: int) -> None:
            l2 = Ledger(self.path)
            # Each thread gets its own stage name to avoid stage-key collisions
            # (stage names must be unique within a unit for timestamps, but
            # we want to verify that all session ids survive).
            l2.mark_stage(unit, STAGES[i % len(STAGES)], f"sess-{i:04d}")

        threads = [threading.Thread(target=_mark, args=(i,)) for i in range(n_threads)]
        for t in threads:
            t.start()
        for t in threads:
            t.join()

        rec = self.ledger.record(unit)
        assert rec is not None
        # All 8 session ids should be present
        expected = {f"sess-{i:04d}" for i in range(n_threads)}
        self.assertEqual(set(rec.sessions), expected)
        self.assertEqual(len(rec.sessions), n_threads)


class TestCorruptFile(unittest.TestCase):
    def setUp(self) -> None:
        self._tmp = tempfile.TemporaryDirectory()
        self.path = Path(self._tmp.name) / "tu_ledger.json"

    def tearDown(self) -> None:
        self._tmp.cleanup()

    def test_corrupt_json_raises_ledger_error(self) -> None:
        self.path.write_text("not valid json\n", "utf-8")
        ledger = Ledger(self.path)
        with self.assertRaises(LedgerError) as cm:
            ledger.record("kyoshin/CExchangeWin")
        self.assertIn(str(self.path), str(cm.exception))

    def test_invalid_structure_raises_ledger_error(self) -> None:
        self.path.write_text(json.dumps({"oops": True}), "utf-8")
        ledger = Ledger(self.path)
        with self.assertRaises(LedgerError) as cm:
            ledger.record("kyoshin/CExchangeWin")
        self.assertIn(str(self.path), str(cm.exception))

    def test_corrupt_on_mutation_raises_ledger_error(self) -> None:
        self.path.write_text("{{{bad json\n", "utf-8")
        ledger = Ledger(self.path)
        with self.assertRaises(LedgerError) as cm:
            ledger.mark_stage("kyoshin/CExchangeWin", "match", "sess-001")
        self.assertIn(str(self.path), str(cm.exception))


class TestAtomicWrite(unittest.TestCase):
    """Verify that the temp file sidecar is used correctly."""

    def setUp(self) -> None:
        self._tmp = tempfile.TemporaryDirectory()
        self.path = Path(self._tmp.name) / "tu_ledger.json"
        self.ledger = Ledger(self.path)

    def tearDown(self) -> None:
        self._tmp.cleanup()

    def test_tmp_file_cleaned_up_after_write(self) -> None:
        self.ledger.mark_stage("kyoshin/CExchangeWin", "match", "sess-001")
        # No .tmp.* file should remain
        tmp_files = list(self.path.parent.glob(f"{self.path.name}.tmp.*"))
        self.assertEqual(tmp_files, [])

    def test_lock_file_created(self) -> None:
        self.ledger.mark_stage("kyoshin/CExchangeWin", "match", "sess-001")
        lock = self.path.with_suffix(self.path.suffix + ".lock")
        self.assertTrue(lock.is_file())


if __name__ == "__main__":
    unittest.main()