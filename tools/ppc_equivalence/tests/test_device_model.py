from __future__ import annotations

import unittest

from tools.ppc_equivalence.device_model import (
    AccessOutcome,
    GxFifoStreamDevice,
    RegisterBankDevice,
    RegisterSpec,
)


class TestRegisterBankDevice(unittest.TestCase):
    def setUp(self) -> None:
        self.device = RegisterBankDevice(
            base=0xCC008000,
            reg_width=4,
            registers=(
                RegisterSpec(offset=0x00, initial=0x0000_0001),
                RegisterSpec(offset=0x04, initial=0x0000_00FF, w1c=True),
                RegisterSpec(offset=0x08, initial=0x0000_000A, read_clear=True),
            ),
        )

    def test_read_write_round_trip(self) -> None:
        addr = 0xCC008000
        self.assertEqual(self.device.read(addr, 4).value, 0x0000_0001)
        result = self.device.write(addr, 4, 0x0000_00AB)
        self.assertEqual(result.outcome, AccessOutcome.OK)
        self.assertEqual(self.device.read(addr, 4).value, 0x0000_00AB)

    def test_w1c_clears_set_bits(self) -> None:
        addr = 0xCC008004
        self.assertEqual(self.device.read(addr, 4).value, 0x0000_00FF)
        self.device.write(addr, 4, 0x0000_0033)
        self.assertEqual(self.device.read(addr, 4).value, 0x0000_00CC)

    def test_read_clear_zeros_register(self) -> None:
        addr = 0xCC008008
        self.assertEqual(self.device.read(addr, 4).value, 0x0000_000A)
        self.assertEqual(self.device.read(addr, 4).value, 0)

    def test_unsupported_width_fail_closed(self) -> None:
        addr = 0xCC008000
        self.assertEqual(
            self.device.validate_access(addr, 8, is_write=False),
            AccessOutcome.UNSUPPORTED,
        )
        self.assertEqual(
            self.device.read(addr, 8).outcome,
            AccessOutcome.UNSUPPORTED,
        )
        self.assertEqual(
            self.device.write(addr, 8, 0).outcome,
            AccessOutcome.UNSUPPORTED,
        )

    def test_misaligned_address(self) -> None:
        self.assertEqual(
            self.device.validate_access(0xCC008001, 4, is_write=False),
            AccessOutcome.MISALIGNED,
        )

    def test_out_of_range_offset(self) -> None:
        self.assertEqual(
            self.device.validate_access(0xCC008010, 4, is_write=False),
            AccessOutcome.OUT_OF_RANGE,
        )

    def test_visible_state_snapshot(self) -> None:
        state = self.device.visible_state()
        self.assertEqual(state["kind"], "register-bank")
        self.assertIn("0x0", state["registers"])


class TestGxFifoStreamDevice(unittest.TestCase):
    def test_records_write_events(self) -> None:
        device = GxFifoStreamDevice(base=0xCC008000, span=0x20)
        result = device.write(0xCC008000, 4, 0x12345678)
        self.assertEqual(result.outcome, AccessOutcome.OK)
        self.assertEqual(len(device.write_events), 1)
        self.assertEqual(device.write_events[0]["value"], hex(0x12345678))

    def test_read_fail_closed(self) -> None:
        device = GxFifoStreamDevice(base=0xCC008000)
        self.assertEqual(device.read(0xCC008000, 4).outcome, AccessOutcome.UNSUPPORTED)

    def test_out_of_span_write(self) -> None:
        device = GxFifoStreamDevice(base=0xCC008000, span=0x10)
        self.assertEqual(
            device.write(0xCC008020, 4, 0).outcome,
            AccessOutcome.OUT_OF_RANGE,
        )


if __name__ == "__main__":
    unittest.main()
