"""Unit tests for centralized FPSCR transitions (SCALAR_FP_V2 Phase 4)."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.fp_exact_outcome import FiFrPolicy, ScalarFPOutcome
from tools.ppc_equivalence.fp_fpscr import (
    FPSCR_FEX,
    FPSCR_FI,
    FPSCR_FR,
    FPSCR_FX,
    FPSCR_OE,
    FPSCR_OX,
    FPSCR_VE,
    FPSCR_VX,
    FPSCR_VXIMZ,
    FPSCR_VXSNAN,
    FPSCR_XE,
    FPSCR_XX,
    FPSCR_ZE,
    FPSCR_ZX,
    FPSCRControlWrite,
    apply_fpscr_transition,
    apply_mcrfs,
    apply_mtfsf,
    apply_mtfsfi,
    apply_mtfsb,
    cr1_from_fpscr,
    raise_sticky,
    recompute_fex,
)


class FpscrStickyReRaiseTests(unittest.TestCase):
    def test_new_exception_sets_fx(self) -> None:
        post = raise_sticky(0, FPSCR_VXIMZ)
        self.assertEqual(post, FPSCR_FX | FPSCR_VX | FPSCR_VXIMZ)

    def test_already_sticky_does_not_re_raise_fx(self) -> None:
        pre = FPSCR_VX | FPSCR_VXIMZ
        post = raise_sticky(pre, FPSCR_VXIMZ)
        self.assertEqual(post, pre)
        self.assertFalse(post & FPSCR_FX)

    def test_pre_existing_fx_preserved_when_reraising_same_sticky(self) -> None:
        pre = FPSCR_FX | FPSCR_VX | FPSCR_VXIMZ
        post = raise_sticky(pre, FPSCR_VXIMZ)
        self.assertEqual(post, pre)


class FpscrFexTests(unittest.TestCase):
    def test_vx_summary_with_ve_sets_fex(self) -> None:
        pre = FPSCR_VE
        post = raise_sticky(pre, FPSCR_VXIMZ)
        self.assertEqual(
            post,
            FPSCR_FX | FPSCR_FEX | FPSCR_VX | FPSCR_VXIMZ | FPSCR_VE,
        )

    def test_ox_with_oe_sets_fex(self) -> None:
        pre = FPSCR_OE
        post = raise_sticky(pre, FPSCR_OX)
        self.assertEqual(post, FPSCR_FX | FPSCR_FEX | FPSCR_OX | FPSCR_OE)

    def test_zx_with_ze_sets_fex(self) -> None:
        pre = FPSCR_ZE
        post = raise_sticky(pre, FPSCR_ZX)
        self.assertEqual(post, FPSCR_FX | FPSCR_FEX | FPSCR_ZX | FPSCR_ZE)

    def test_xx_with_xe_sets_fex(self) -> None:
        pre = FPSCR_XX >> 22  # XE enable bit position
        from tools.ppc_equivalence.fp_fpscr import FPSCR_XE

        pre = FPSCR_XE
        post = raise_sticky(pre, FPSCR_XX)
        self.assertEqual(post, FPSCR_FX | FPSCR_FEX | FPSCR_XX | FPSCR_XE)

    def test_enabled_exception_without_matching_sticky_clears_fex(self) -> None:
        pre = FPSCR_VE | FPSCR_FEX
        post = recompute_fex(pre)
        self.assertEqual(post, FPSCR_VE)
        self.assertFalse(post & FPSCR_FEX)


class FpscrFiFrPolicyTests(unittest.TestCase):
    def test_bitwise_preserves_pre_existing_fi_fr(self) -> None:
        pre = FPSCR_FI | FPSCR_FR | 0x4000
        outcome = ScalarFPOutcome(
            result_bits=0x3FF0000000000000,
            fprf=0x04,
            fi_fr_policy=FiFrPolicy.PRESERVE,
        )
        post = apply_fpscr_transition(pre, "fmr", outcome)
        self.assertEqual(post & (FPSCR_FI | FPSCR_FR), FPSCR_FI | FPSCR_FR)

    def test_arithmetic_clears_then_sets_fi_fr(self) -> None:
        pre = FPSCR_FI | FPSCR_FR | FPSCR_XE | 0x4000
        outcome = ScalarFPOutcome(
            result_bits=0x4008000000000000,
            raised_causes=FPSCR_XX,
            fi=True,
            fr=False,
            fi_fr_policy=FiFrPolicy.SET,
            fprf=0x04,
        )
        post = apply_fpscr_transition(pre, "fadd", outcome)
        self.assertEqual(post & FPSCR_FI, FPSCR_FI)
        self.assertFalse(post & FPSCR_FR)
        self.assertTrue(post & FPSCR_XX)

    def test_xx_sticky_gated_without_xe(self) -> None:
        pre = FPSCR_FI | FPSCR_FR | 0x4000
        outcome = ScalarFPOutcome(
            result_bits=0x4008000000000000,
            raised_causes=FPSCR_XX,
            fi=True,
            fr=False,
            fi_fr_policy=FiFrPolicy.SET,
            fprf=0x04,
        )
        post = apply_fpscr_transition(pre, "fadd", outcome)
        self.assertFalse(post & FPSCR_XX)

    def test_arithmetic_default_clears_fi_fr_without_new_values(self) -> None:
        pre = FPSCR_FI | FPSCR_FR
        outcome = ScalarFPOutcome(
            result_bits=0x3FF0000000000000,
            fprf=0x04,
            fi_fr_policy=FiFrPolicy.CLEAR,
        )
        post = apply_fpscr_transition(pre, "fmul", outcome)
        self.assertFalse(post & (FPSCR_FI | FPSCR_FR))


class FpscrControlOpTests(unittest.TestCase):
    def test_mtfsf_applies_field_mask_and_normalizes(self) -> None:
        # Corpus: copy VXIMZ, VE, reserved bit from FPR low word 0x100880.
        pre = 0
        source = 0x100880
        post = apply_mtfsf(pre, 0xFF, source)
        self.assertEqual(post, 0x60100080)
        self.assertEqual(cr1_from_fpscr(post), 0x6)

    def test_mtfsf_preserves_rn_ni_field_when_unmasked(self) -> None:
        pre = 0x5
        source = 0x00000003
        post = apply_mtfsf(pre, 0xFE, source)
        self.assertEqual(post & 0xF, 0x5)
        post_all = apply_mtfsf(pre, 0xFF, source)
        self.assertEqual(post_all & 0xF, 0x3)

    def test_mtfsfi_writes_immediate_field(self) -> None:
        post = apply_mtfsfi(0, 7, 3)
        self.assertEqual(post, 3)

    def test_mtfsfi_reserved_field_forced_zero(self) -> None:
        post = apply_mtfsfi(0, 5, 8)
        self.assertEqual(post, 0)

    def test_mtfsb1_raises_exception_sticky(self) -> None:
        pre = FPSCR_VE
        post = apply_mtfsb(pre, 11, set_bit=True)  # VXIMZ
        self.assertEqual(
            post,
            FPSCR_FX | FPSCR_FEX | FPSCR_VX | FPSCR_VXIMZ | FPSCR_VE,
        )

    def test_mtfsb0_clears_summary_sticky_preserves_fx(self) -> None:
        pre = FPSCR_FX | FPSCR_FEX | FPSCR_VX | FPSCR_VXIMZ | FPSCR_VE
        post = apply_mtfsb(pre, 11, set_bit=False)
        self.assertEqual(post, FPSCR_FX | FPSCR_VE)
        self.assertFalse(post & FPSCR_FEX)

    def test_mcrfs_transfers_cr_and_clears_exception_field(self) -> None:
        pre = FPSCR_FX | FPSCR_VX | FPSCR_VXIMZ
        result = apply_mcrfs(pre, 2)
        self.assertEqual(result.cr_field, 0x1)
        self.assertEqual(result.post_fpscr, FPSCR_FX)

    def test_mcrfs_corpus_case(self) -> None:
        pre = 0xA0100000
        result = apply_mcrfs(pre, 2)
        self.assertEqual(result.cr_field, 0x1)
        self.assertEqual(result.post_fpscr, 0x80000000)

    def test_apply_fpscr_transition_control_write_paths(self) -> None:
        mtfsf = apply_fpscr_transition(
            0,
            "mtfsf",
            control_write=FPSCRControlWrite(mtfsf_fm=0xFF, mtfsf_source=0x100880),
        )
        self.assertEqual(mtfsf, 0x60100080)

        mtfsfi = apply_fpscr_transition(
            0,
            "mtfsfi",
            control_write=FPSCRControlWrite(mtfsfi_bf=7, mtfsfi_imm4=3),
        )
        self.assertEqual(mtfsfi, 3)

        mtfsb1 = apply_fpscr_transition(
            FPSCR_VE,
            "mtfsb1",
            control_write=FPSCRControlWrite(mtfsb_bt=11, mtfsb_set=True),
        )
        self.assertEqual(
            mtfsb1,
            FPSCR_FX | FPSCR_FEX | FPSCR_VX | FPSCR_VXIMZ | FPSCR_VE,
        )

        mcrfs = apply_fpscr_transition(
            FPSCR_FX | FPSCR_VX | FPSCR_VXIMZ,
            "mcrfs",
            control_write=FPSCRControlWrite(mcrfs_bfa=2),
        )
        self.assertEqual(mcrfs, FPSCR_FX)


class FpscrArithmeticTransitionTests(unittest.TestCase):
    def test_invalid_arithmetic_with_pre_existing_fpscr(self) -> None:
        pre = 0x5000 | FPSCR_VE | FPSCR_ZE
        outcome = ScalarFPOutcome(
            result_bits=0x7FF8000000000000,
            raised_causes=FPSCR_VXSNAN | FPSCR_ZX,
            fprf=0x11,
            fi_fr_policy=FiFrPolicy.CLEAR,
        )
        post = apply_fpscr_transition(pre, "fdiv", outcome)
        self.assertEqual(
            post,
            FPSCR_FX
            | FPSCR_FEX
            | FPSCR_VX
            | FPSCR_VXSNAN
            | FPSCR_ZX
            | FPSCR_VE
            | FPSCR_ZE
            | 0x11000,
        )

    def test_record_form_cr1_shadow(self) -> None:
        from tools.ppc_equivalence.fp_fpscr import FPSCR_VXSQRT

        pre = FPSCR_FI | FPSCR_FR
        outcome = ScalarFPOutcome(
            result_bits=0x7FF8000000000000,
            raised_causes=FPSCR_VXSQRT,
            fprf=0x11,
            fi_fr_policy=FiFrPolicy.PRESERVE,
        )
        post = apply_fpscr_transition(pre, "frsqrte", outcome)
        self.assertEqual(
            post,
            FPSCR_FX | FPSCR_VX | FPSCR_VXSQRT | FPSCR_FI | FPSCR_FR | 0x11000,
        )
        self.assertEqual(cr1_from_fpscr(post), 0xA)


if __name__ == "__main__":
    unittest.main()
