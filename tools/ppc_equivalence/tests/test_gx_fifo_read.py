"""``gx-fifo-read`` (outcome 3) fail-closed policy tests.

Per ``GX_FIFO_TIER_A.md``: "In-tree SDK evidence documents a FIFO
write/gather pipe. There is no authoritative measurement of CPU load
results from 0xCC008000." Outcome 3 is a *policy* capability — concrete and
symbolic loads targeting the GX FIFO region fail closed
(``AccessOutcome.UNSUPPORTED``) — never a value model. These tests prove:

1. Reads are unsupported for every declared width and address.
2. The event cursor never advances on a read (mutation-killed below: a
   device subclass that *does* advance the cursor on read must be caught).
3. Reordered-read sharing is **not applicable** under outcome 3 — there is
   no read value to share between two "sides" of an equivalence check, so
   there is nothing to reorder. This is documented, not asserted
   numerically (see ``corpora/gx_fifo_v1/read_policy.jsonl`` row notes).
4. Read side-effect drop is **not applicable** — ``read_side_effects`` is
   statically ``False`` on the reviewed device
   (``GX_FIFO_TIER_A.md`` "Dependencies": "Because reads are unsupported (no
   external input, no read side effects), ``gx-fifo-read-v1`` does not
   demand ``mmio-external-input`` or ``mmio-read-side-effects``.").
"""

from __future__ import annotations

import unittest
from dataclasses import dataclass, field
from typing import Any

from tools.ppc_equivalence.device_model import (
    AccessOutcome,
    DeviceReadResult,
    GxFifoStreamDevice,
)
from tools.ppc_equivalence.gx_fifo_corpus import (
    CORPUS_DIR,
    load_corpus_file,
    parse_hex_literal,
    replay_row,
)

_BASE = 0xCC008000
_SPAN = 128


class ReadUnsupportedForEveryWidthAndAddressTests(unittest.TestCase):
    """Requirement 1: every declared width/address in the read_policy corpus
    is fail-closed, and directly probing the device confirms it independent
    of the corpus loader."""

    def setUp(self) -> None:
        self.device = GxFifoStreamDevice(base=_BASE, span=_SPAN)

    def test_supported_widths_all_unsupported_for_reads(self) -> None:
        for width in (1, 2, 4):
            with self.subTest(width=width):
                result = self.device.read(_BASE, width)
                self.assertEqual(result.outcome, AccessOutcome.UNSUPPORTED)
                self.assertIsNone(result.value)

    def test_unsupported_width_also_unsupported(self) -> None:
        result = self.device.read(_BASE, 8)
        self.assertEqual(result.outcome, AccessOutcome.UNSUPPORTED)

    def test_misaligned_read_is_misaligned_not_unsupported(self) -> None:
        result = self.device.read(_BASE + 1, 2)
        self.assertEqual(result.outcome, AccessOutcome.MISALIGNED)

    def test_neighboring_addresses_still_fail_closed(self) -> None:
        for addr in (_BASE - 4, _BASE, _BASE + _SPAN - 4, _BASE + _SPAN):
            with self.subTest(addr=hex(addr)):
                result = self.device.read(addr, 4)
                self.assertEqual(result.outcome, AccessOutcome.UNSUPPORTED)

    def test_read_policy_corpus_rows_replay_against_the_real_device(self) -> None:
        _, rows = load_corpus_file(CORPUS_DIR / "read_policy.jsonl")
        self.assertTrue(rows)
        for row in rows:
            with self.subTest(row_id=row["id"]):
                result = replay_row(row, corpus_file="read_policy.jsonl")
                self.assertTrue(result.passed, result.reason)
                for op in row["ops"]:
                    if op["kind"] == "read":
                        self.assertIn(
                            op["expected_outcome"], ("unsupported", "misaligned"),
                        )


class CursorNeverAdvancesOnReadTests(unittest.TestCase):
    """Requirement 2: cursor invariance across reads, with a mutation-killer
    proving the check actually detects a broken implementation."""

    def test_cursor_unchanged_after_many_reads(self) -> None:
        device = GxFifoStreamDevice(base=_BASE, span=_SPAN)
        before = device.event_cursor
        for _ in range(5):
            device.read(_BASE, 4)
        self.assertEqual(device.event_cursor, before)
        self.assertEqual(device.read_events, [])

    def test_cursor_unaffected_by_reads_interleaved_with_writes(self) -> None:
        device = GxFifoStreamDevice(base=_BASE, span=_SPAN)
        device.write(_BASE, 4, 0x1)
        cursor_after_write = device.event_cursor
        for _ in range(3):
            device.read(_BASE, 4)
        self.assertEqual(device.event_cursor, cursor_after_write)

    def test_mutation_broken_device_that_advances_cursor_on_read_is_caught(self) -> None:
        """A hypothetical buggy device that advances event_cursor on read
        must be distinguishable from the real (correct) device by this
        invariant — i.e. the invariant check itself is not vacuous."""

        @dataclass
        class _CursorLeakingGxFifoDevice(GxFifoStreamDevice):
            def read(self, addr: int, width: int) -> DeviceReadResult:  # type: ignore[override]
                outcome = self.validate_access(addr, width, is_write=False)
                if outcome is AccessOutcome.UNSUPPORTED:
                    # Bug: silently leaks a cursor advance on an
                    # "unsupported" read instead of leaving state untouched.
                    self.event_cursor += 1
                return DeviceReadResult(outcome=outcome)

        def _assert_cursor_never_advances_on_read(device: GxFifoStreamDevice) -> None:
            before = device.event_cursor
            device.read(_BASE, 4)
            if device.event_cursor != before:
                raise AssertionError(
                    "gx-fifo-read cursor invariant violated: cursor advanced on read",
                )

        good = GxFifoStreamDevice(base=_BASE, span=_SPAN)
        _assert_cursor_never_advances_on_read(good)  # must not raise

        bad = _CursorLeakingGxFifoDevice(base=_BASE, span=_SPAN)
        with self.assertRaises(AssertionError):
            _assert_cursor_never_advances_on_read(bad)


class NotApplicableUnderOutcome3Tests(unittest.TestCase):
    """Requirements 3 and 4: documented as not-applicable rather than
    asserted numerically, per GX_FIFO_TIER_A.md outcome 3."""

    def test_reordered_read_sharing_is_not_applicable(self) -> None:
        """There is no read value to share between two equivalence "sides"
        under outcome 3 (see GX_FIFO_TIER_A.md: "An unconstrained value
        chosen independently per side would produce false negatives;
        blindly sharing one unconstrained value would be unsound."). Two
        independent devices reading in different orders both simply fail
        closed — there is nothing to reorder or share."""
        device_a = GxFifoStreamDevice(base=_BASE, span=_SPAN)
        device_b = GxFifoStreamDevice(base=_BASE, span=_SPAN)

        # "Side A" reads low-to-high; "side B" reads high-to-low. Both must
        # independently fail closed regardless of order — no shared value.
        outcomes_a = [device_a.read(_BASE + off, 4).outcome for off in (0, 4, 8)]
        outcomes_b = [device_b.read(_BASE + off, 4).outcome for off in (8, 4, 0)]
        self.assertTrue(all(o is AccessOutcome.UNSUPPORTED for o in outcomes_a))
        self.assertTrue(all(o is AccessOutcome.UNSUPPORTED for o in outcomes_b))

    def test_read_side_effects_flag_is_statically_false(self) -> None:
        """No read side effects to drop: read_side_effects is a static
        False on the reviewed device, so "side-effect drop" mutation-testing
        (as done for write-event dropping) has no analogue here."""
        device = GxFifoStreamDevice(base=_BASE, span=_SPAN)
        self.assertFalse(device.read_side_effects)
        self.assertFalse(device.external_input)
        self.assertEqual(device.read_model_version, "gx-fifo-read-v1")


if __name__ == "__main__":
    unittest.main()
