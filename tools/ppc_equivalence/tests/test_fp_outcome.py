"""Unit tests for the unified FPOutcome scaffold and SoftFloat adapters."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.fp_oracle import (
    FpOracleFlags,
    FpOracleResult,
    fadd_binary64_rne,
    fprf_from_binary64,
)
from tools.ppc_equivalence.fp_outcome import (
    FPExceptionFlags,
    FPOutcome,
    clear_exception_flags,
    combine_paired_outcomes,
    exception_flags,
    flags_from_oracle,
    flags_to_oracle,
    oracle_from_outcome,
    outcome_from_oracle,
    outcome_from_result_bits,
    outcome_primary_bits,
    outcome_with_computed_fprf,
    unsupported_outcome,
)

_F15 = 0x3FF8000000000000
_F2 = 0x4000000000000000


class FPOutcomeModelTests(unittest.TestCase):
    def test_exception_flags_defaults(self) -> None:
        flags = clear_exception_flags()
        self.assertFalse(flags.invalid)
        self.assertFalse(flags.overflow)
        self.assertFalse(flags.underflow)
        self.assertFalse(flags.divide_by_zero)
        self.assertFalse(flags.inexact)

    def test_exception_flags_partial(self) -> None:
        flags = exception_flags(inexact=True, overflow=True)
        self.assertTrue(flags.inexact)
        self.assertTrue(flags.overflow)
        self.assertFalse(flags.invalid)

    def test_outcome_primary_bits(self) -> None:
        outcome = outcome_from_result_bits(0x3FF0000000000000, fprf=0x04)
        self.assertEqual(outcome.primary_bits(), 0x3FF0000000000000)
        self.assertEqual(outcome_primary_bits(outcome), 0x3FF0000000000000)

    def test_outcome_empty_bits_rejected(self) -> None:
        with self.assertRaises(ValueError):
            outcome_from_result_bits()

    def test_unsupported_outcome(self) -> None:
        outcome = unsupported_outcome("scaffold deferral")
        self.assertFalse(outcome.supported)
        self.assertEqual(outcome.unsupported_reason, "scaffold deferral")
        self.assertFalse(outcome.writeback)

    def test_to_dict_shape(self) -> None:
        outcome = outcome_from_result_bits(
            1,
            flags=exception_flags(inexact=True),
            fprf=0x02,
        )
        payload = outcome.to_dict()
        self.assertEqual(payload["result_bits"], [1])
        self.assertTrue(payload["flags"]["inexact"])
        self.assertEqual(payload["fprf"], 0x02)
        self.assertTrue(payload["supported"])

    def test_frozen(self) -> None:
        outcome = outcome_from_result_bits(0)
        with self.assertRaises(Exception):
            outcome.fprf = 1  # type: ignore[misc]
        flags = clear_exception_flags()
        with self.assertRaises(Exception):
            flags.inexact = True  # type: ignore[misc]


class SoftFloatAdapterRoundTripTests(unittest.TestCase):
    def test_flags_round_trip(self) -> None:
        oracle_flags = FpOracleFlags(invalid=True, inexact=True, underflow=True)
        unified = flags_from_oracle(oracle_flags)
        self.assertIsInstance(unified, FPExceptionFlags)
        back = flags_to_oracle(unified)
        self.assertEqual(back, oracle_flags)

    def test_oracle_result_round_trip(self) -> None:
        result = fadd_binary64_rne(_F15, _F2)
        outcome = outcome_from_oracle(result)
        self.assertIsInstance(outcome, FPOutcome)
        self.assertEqual(outcome.result_bits, (result.bits64,))
        self.assertEqual(outcome.fprf, result.fprf)
        self.assertEqual(outcome.flags.inexact, result.flags.inexact)
        self.assertTrue(outcome.supported)
        self.assertTrue(outcome.writeback)
        self.assertFalse(outcome.trap)

        restored = oracle_from_outcome(outcome)
        self.assertEqual(restored.bits64, result.bits64)
        self.assertEqual(restored.flags, result.flags)
        self.assertEqual(restored.fprf, result.fprf)

    def test_fp_oracle_result_to_outcome_method(self) -> None:
        result = fadd_binary64_rne(_F15, _F2)
        outcome = result.to_outcome()
        self.assertEqual(outcome.primary_bits(), result.bits64)
        self.assertEqual(oracle_from_outcome(outcome), result)

    def test_bits_api_adapter_preserves_pattern(self) -> None:
        result = fadd_binary64_rne(_F15, _F2)
        # Simulate ConcreteOps bits-only consumers wrapping afterward.
        bits_only = result.bits64
        outcome = outcome_from_result_bits(
            bits_only,
            flags=flags_from_oracle(result.flags),
            fprf=result.fprf,
        )
        self.assertEqual(outcome_primary_bits(outcome), bits_only)
        self.assertEqual(oracle_from_outcome(outcome).bits64, bits_only)

    def test_outcome_with_computed_fprf(self) -> None:
        bits = 0x3FF0000000000000
        outcome = outcome_with_computed_fprf(bits)
        self.assertEqual(outcome.fprf, fprf_from_binary64(bits))
        self.assertEqual(outcome.primary_bits(), bits)

    def test_oracle_from_unsupported_rejected(self) -> None:
        with self.assertRaises(ValueError):
            oracle_from_outcome(unsupported_outcome("nope"))

    def test_oracle_from_paired_lanes_rejected(self) -> None:
        outcome = outcome_from_result_bits(1, 2, fprf=0)
        with self.assertRaises(ValueError):
            oracle_from_outcome(outcome)

    def test_combine_paired_outcomes_two_lanes(self) -> None:
        from tools.ppc_equivalence.fp_oracle import fadds_fpr_rne

        combined = combine_paired_outcomes(
            outcome_from_oracle(fadds_fpr_rne(_F15, _F2)),
            outcome_from_oracle(fadds_fpr_rne(_F15, _F2)),
        )
        self.assertEqual(len(combined.result_bits), 2)
        self.assertTrue(combined.writeback)
        with self.assertRaises(ValueError):
            oracle_from_outcome(combined)

    def test_oracle_from_symbolic_bits_rejected(self) -> None:
        outcome = FPOutcome(
            result_bits=("sym",),
            flags=clear_exception_flags(),
            invalid_cause=0,
            fprf=0,
            writeback=True,
            trap=False,
            supported=True,
        )
        with self.assertRaises(TypeError):
            oracle_from_outcome(outcome)

    def test_manual_oracle_result_round_trip(self) -> None:
        original = FpOracleResult(
            bits64=0xBFF0000000000000,
            flags=FpOracleFlags(inexact=True),
            fprf=0x08,
        )
        self.assertEqual(oracle_from_outcome(outcome_from_oracle(original)), original)


if __name__ == "__main__":
    unittest.main()
