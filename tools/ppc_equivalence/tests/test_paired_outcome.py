"""Wave 3 Track B: paired-single FPOutcome combine and oracle lane tests."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.fp_oracle import (
    ps_lane_invalid_cause_add,
    ps_lane_invalid_cause_fused,
    ps_lane_invalid_cause_mul,
    ps_lane_outcome,
)
from tools.ppc_equivalence.fp_outcome import (
    FPSCR_VXIMZ,
    FPSCR_VXISI,
    FPSCR_VXSNAN,
    PAIRED_ORACLE_OPS,
    combine_paired_outcomes,
    cr1_from_fpscr,
    merge_exception_flags,
    merge_invalid_cause,
    outcome_from_oracle,
    unsupported_outcome,
)
from tools.ppc_equivalence.fp_oracle import fadds_fpr_rne
from tools.ppc_equivalence.ir import Instruction, Opcode
from tools.ppc_equivalence.model import concrete_state
from tools.ppc_equivalence.semantics import (
    FPSCR_FX,
    FPSCR_VX,
    FPSCR_VXIMZ as SEM_VXIMZ,
    FPSCR_VXISI as SEM_VXISI,
    FPSCR_VXSNAN as SEM_VXSNAN,
    ConcreteOps,
    execute_instruction,
)


class CombinePairedOutcomesTests(unittest.TestCase):
    def test_merge_invalid_cause_or(self) -> None:
        self.assertEqual(
            merge_invalid_cause(FPSCR_VXSNAN, FPSCR_VXISI),
            FPSCR_VXSNAN | FPSCR_VXISI,
        )

    def test_merge_exception_flags(self) -> None:
        from tools.ppc_equivalence.fp_outcome import clear_exception_flags, exception_flags

        merged = merge_exception_flags(
            exception_flags(invalid=True),
            exception_flags(inexact=True),
        )
        self.assertTrue(merged.invalid)
        self.assertTrue(merged.inexact)
        self.assertFalse(merged.overflow)
        self.assertEqual(merged, merge_exception_flags(merged, clear_exception_flags()))

    def test_combine_ps0_fprf_and_writeback(self) -> None:
        lane0 = outcome_from_oracle(fadds_fpr_rne(0x3FF0000000000000, 0x4000000000000000))
        lane1 = outcome_from_oracle(fadds_fpr_rne(0xBFF0000000000000, 0x4000000000000000))
        combined = combine_paired_outcomes(lane0, lane1)
        self.assertEqual(len(combined.result_bits), 2)
        self.assertEqual(combined.fprf, lane0.fprf)
        self.assertTrue(combined.writeback)
        self.assertTrue(combined.supported)

    def test_combine_rejects_unsupported_lane(self) -> None:
        bad = unsupported_outcome("deferred ps_div")
        good = outcome_from_oracle(fadds_fpr_rne(0x3FF0000000000000, 0x4000000000000000))
        combined = combine_paired_outcomes(bad, good)
        self.assertFalse(combined.supported)
        self.assertIn("deferred", combined.unsupported_reason or "")

    def test_cr1_from_fpscr(self) -> None:
        self.assertEqual(cr1_from_fpscr(0xA0000000), 0xA)


class PsLaneOracleTests(unittest.TestCase):
    def test_supported_opcode_set(self) -> None:
        self.assertEqual(
            PAIRED_ORACLE_OPS,
            frozenset({
                "ps_add", "ps_sub", "ps_mul",
                "ps_madd", "ps_msub", "ps_nmadd", "ps_nmsub",
            }),
        )

    def test_lane_invalid_cause_add_inf_opposite(self) -> None:
        cause = ps_lane_invalid_cause_add(
            0x7FF0000000000000,
            0xFFF0000000000000,
        )
        self.assertEqual(cause, FPSCR_VXISI)

    def test_lane_invalid_cause_mul_imz(self) -> None:
        cause = ps_lane_invalid_cause_mul(0x7FF0000000000000, 0x0000000000000000)
        self.assertEqual(cause, FPSCR_VXIMZ)

    def test_lane_invalid_cause_fused_snan(self) -> None:
        cause = ps_lane_invalid_cause_fused(
            0xFFF0000012345678,
            0x4000000000000000,
            0x4000000000000000,
            subtract=False,
        )
        self.assertEqual(cause, FPSCR_VXSNAN)

    def test_lane_outcome_deferred_op(self) -> None:
        outcome = ps_lane_outcome("ps_div", 0x3FF0000000000000, 0x4000000000000000)
        self.assertFalse(outcome.supported)


class PairedOracleSemanticsTests(unittest.TestCase):
    def test_ps_add_matches_corpus_lane_bits(self) -> None:
        state = concrete_state({
            "fpr": {"f1": 1.5, "f2": 2.0, "f3": 4.0},
            "ps1": {"f1": -2.0, "f2": 4.0, "f3": -0.5},
            "fpscr": 0x10000000,
        })
        final = execute_instruction(
            state,
            Instruction(0, 0, Opcode.PS_ADD, (7, 1, 2), record=True),
            ConcreteOps(),
        )
        self.assertEqual(final.fpr[7], 0x400C000000000000)
        self.assertEqual(final.ps1[7], 0x4000000000000000)
        self.assertEqual(final.fpscr & 0x1F000, 0x4000)
        self.assertEqual(final.cr & 0x0F000000, 0x01000000)

    def test_ps_add_aggregates_lane_exceptions(self) -> None:
        final = execute_instruction(concrete_state({
            "fpr": {"f1": "0x7ff0000000000000", "f2": "0xfff0000000000000"},
            "ps1": {"f1": "0x7ff0000012345678", "f2": 2.0},
        }), Instruction(0, 0, Opcode.PS_ADD, (7, 1, 2), record=True), ConcreteOps())
        self.assertEqual(final.fpr[7], 0x7FF8000000000000)
        self.assertEqual(final.ps1[7], 0x7FF8000000000000)
        self.assertEqual(
            final.fpscr,
            FPSCR_FX | FPSCR_VX | SEM_VXISI | SEM_VXSNAN | 0x11000,
        )

    def test_ps_mul_vximz_ps1(self) -> None:
        final = execute_instruction(concrete_state({
            "fpr": {"f1": 1.0, "f3": 2.0},
            "ps1": {"f1": 0.0, "f3": "0x7ff0000000000000"},
        }), Instruction(0, 0, Opcode.PS_MUL, (7, 1, 3)), ConcreteOps())
        self.assertEqual(final.ps1[7], 0x7FF8000000000000)
        self.assertEqual(final.fpscr, FPSCR_FX | FPSCR_VX | SEM_VXIMZ | 0x4000)

    def test_ps_nmadd_nan_priority_and_ve_writeback(self) -> None:
        snan_a = 0xFFF0000012345678
        final = execute_instruction(concrete_state({
            "fpr": {"f1": snan_a, "f2": "0x7ff80000abcdef01", "f3": 2.0, "f7": 1.0},
            "ps1": {
                "f1": 0.0,
                "f2": "0xfff0000000000000",
                "f3": "0x7ff0000000000000",
                "f7": 1.0,
            },
            "fpscr": 0x80,
        }), Instruction(0, 0, Opcode.PS_NMADD, (7, 1, 2, 3)), ConcreteOps())
        self.assertEqual(final.fpr[7], 0xFFF8000000000000)
        self.assertEqual(final.ps1[7], 0x7FF8000000000000)
        self.assertTrue(final.fpr[7] != 1.0)
        self.assertEqual(
            final.fpscr & (FPSCR_FX | FPSCR_VX | SEM_VXSNAN | SEM_VXIMZ),
            FPSCR_FX | FPSCR_VX | SEM_VXSNAN | SEM_VXIMZ,
        )


if __name__ == "__main__":
    unittest.main()
