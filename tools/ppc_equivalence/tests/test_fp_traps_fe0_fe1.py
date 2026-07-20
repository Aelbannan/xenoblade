"""Phase 8 SCALAR_FP_V2 FE0/FE1 trap-delivery."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.decoder import decode_block, parse_hex
from tools.ppc_equivalence.fp_capabilities import set_scalar_fp_exact_v2_module_flag
from tools.ppc_equivalence.fp_oracle import fadd_binary64_rne
from tools.ppc_equivalence.fp_outcome import outcome_from_oracle
from tools.ppc_equivalence.fp_traps import (
    FE0Fe1Mode,
    FPDeliveryClass,
    FP_PENDING_DEFERRED,
    FP_PENDING_IMPRECIS,
    FPSCR_OE,
    FPSCR_OX,
    FPSCR_VE,
    MSR_FE0,
    MSR_FE1,
    PROGRAM_EXCEPTION_VECTOR,
    cfg_fp_trap_branches,
    classify_fe0_fe1_mode,
    effective_traps_enabled,
    fe0_fe1_modeling_status,
    fpscr_trap_enables_set,
    msr_fe0_set,
    msr_fe1_set,
    pending_fp_exception_from_causes,
    plan_fp_trap_delivery,
    resolve_fp_trap_policy,
    traps_v2_ledger_dimensions,
)
from tools.ppc_equivalence.model import concrete_state
from tools.ppc_equivalence.result import FloatingPointDomain
from tools.ppc_equivalence.semantics import ConcreteOps, FPSCR_VXSNAN, execute_cfg

_SNAN = 0x7FF0000012345678
_ONE = 0x3FF0000000000000


def _fadd_insns():
    return decode_block(parse_hex("fc61102a"), validate_with_capstone=False)


class FE0Fe1ModeClassificationTests(unittest.TestCase):
    def test_all_four_modes(self) -> None:
        self.assertEqual(classify_fe0_fe1_mode(0), FE0Fe1Mode.IMPRECISE_NONRECOVERABLE)
        self.assertEqual(
            classify_fe0_fe1_mode(MSR_FE1),
            FE0Fe1Mode.IMPRECISE_RECOVERABLE,
        )
        self.assertEqual(
            classify_fe0_fe1_mode(MSR_FE0),
            FE0Fe1Mode.PRECISE,
        )
        self.assertEqual(
            classify_fe0_fe1_mode(MSR_FE0 | MSR_FE1),
            FE0Fe1Mode.RESERVED,
        )

    def test_msr_bit_helpers(self) -> None:
        self.assertFalse(msr_fe0_set(0))
        self.assertFalse(msr_fe1_set(0))
        self.assertTrue(msr_fe0_set(MSR_FE0))
        self.assertTrue(msr_fe1_set(MSR_FE1))


class EffectiveTrapsEnabledTests(unittest.TestCase):
    def tearDown(self) -> None:
        set_scalar_fp_exact_v2_module_flag(None)

    def test_legacy_domain_flag_only_when_v2_off(self) -> None:
        set_scalar_fp_exact_v2_module_flag(False)
        self.assertTrue(
            effective_traps_enabled(
                domain_traps_enabled=True,
                fpscr=0,
                msr=0,
            ),
        )
        self.assertFalse(
            effective_traps_enabled(
                domain_traps_enabled=False,
                fpscr=FPSCR_VE,
                msr=MSR_FE0,
            ),
        )

    def test_exact_v2_requires_live_enables(self) -> None:
        set_scalar_fp_exact_v2_module_flag(True)
        self.assertFalse(
            effective_traps_enabled(
                domain_traps_enabled=True,
                fpscr=0,
                msr=MSR_FE0,
            ),
        )
        self.assertTrue(
            effective_traps_enabled(
                domain_traps_enabled=True,
                fpscr=FPSCR_VE,
                msr=MSR_FE0,
            ),
        )
        self.assertTrue(fpscr_trap_enables_set(FPSCR_VE | FPSCR_OE))


class TrapDeliveryPlanTests(unittest.TestCase):
    def tearDown(self) -> None:
        set_scalar_fp_exact_v2_module_flag(None)

    def _snan_outcome(self):
        return outcome_from_oracle(fadd_binary64_rne(_SNAN, _ONE))

    def test_legacy_precise_delivery_when_v2_off(self) -> None:
        set_scalar_fp_exact_v2_module_flag(False)
        outcome = self._snan_outcome()
        legacy = resolve_fp_trap_policy(outcome, FPSCR_VE)
        plan = plan_fp_trap_delivery(
            outcome,
            FPSCR_VE,
            msr=0,
            domain_traps_enabled=True,
        )
        self.assertTrue(plan.trap)
        self.assertFalse(plan.writeback)
        self.assertIsNone(plan.pending)
        self.assertEqual(plan.trap, legacy.trap)

    def test_precise_mode_delivers_immediately_when_v2_on(self) -> None:
        set_scalar_fp_exact_v2_module_flag(True)
        outcome = self._snan_outcome()
        plan = plan_fp_trap_delivery(
            outcome,
            FPSCR_VE,
            msr=MSR_FE0,
            fault_pc=0x100,
            domain_traps_enabled=True,
        )
        self.assertTrue(plan.trap)
        self.assertFalse(plan.writeback)
        self.assertIsNone(plan.pending)

    def test_imprecise_recoverable_defers_to_pending_when_v2_on(self) -> None:
        set_scalar_fp_exact_v2_module_flag(True)
        outcome = self._snan_outcome()
        plan = plan_fp_trap_delivery(
            outcome,
            FPSCR_VE,
            msr=MSR_FE1,
            fault_pc=0x200,
            domain_traps_enabled=True,
        )
        self.assertFalse(plan.trap)
        self.assertTrue(plan.writeback)
        self.assertIsNotNone(plan.pending)
        assert plan.pending is not None
        self.assertEqual(plan.pending.fault_pc, 0x200)
        self.assertTrue(plan.pending.recoverability)
        self.assertEqual(plan.pending.delivery_class, FPDeliveryClass.PENDING)

    def test_imprecise_nonrecoverable_pending(self) -> None:
        set_scalar_fp_exact_v2_module_flag(True)
        outcome = self._snan_outcome()
        plan = plan_fp_trap_delivery(
            outcome,
            FPSCR_VE,
            msr=0,
            fault_pc=0x300,
            domain_traps_enabled=True,
        )
        self.assertFalse(plan.trap)
        assert plan.pending is not None
        self.assertFalse(plan.pending.recoverability)
        self.assertEqual(plan.pending.delivery_class, FPDeliveryClass.PENDING)

    def test_reserved_mode_deferred_pending(self) -> None:
        set_scalar_fp_exact_v2_module_flag(True)
        outcome = self._snan_outcome()
        plan = plan_fp_trap_delivery(
            outcome,
            FPSCR_VE,
            msr=MSR_FE0 | MSR_FE1,
            fault_pc=0x350,
            domain_traps_enabled=True,
        )
        self.assertFalse(plan.trap)
        self.assertTrue(plan.writeback)
        assert plan.pending is not None
        self.assertEqual(plan.pending.delivery_class, FPDeliveryClass.DEFERRED)

    def test_overflow_pending_carries_cause_mask(self) -> None:
        set_scalar_fp_exact_v2_module_flag(True)
        from tools.ppc_equivalence.fp_outcome import exception_flags, outcome_from_result_bits

        outcome = outcome_from_result_bits(
            0x7FF0000000000000,
            flags=exception_flags(overflow=True, inexact=True),
        )
        plan = plan_fp_trap_delivery(
            outcome,
            FPSCR_OE,
            msr=MSR_FE1,
            fault_pc=0x400,
            domain_traps_enabled=True,
        )
        assert plan.pending is not None
        self.assertTrue(plan.pending.cause_mask & FPSCR_OX)


class PendingFPExceptionTests(unittest.TestCase):
    def test_pending_record_fields(self) -> None:
        pending = pending_fp_exception_from_causes(
            FPSCR_OX,
            0x500,
            mode=FE0Fe1Mode.RESERVED,
        )
        self.assertEqual(pending.cause_mask, FPSCR_OX)
        self.assertEqual(pending.fault_pc, 0x500)
        self.assertFalse(pending.recoverability)
        self.assertEqual(pending.delivery_class, FPDeliveryClass.DEFERRED)


class FE0Fe1ModelingStatusTests(unittest.TestCase):
    def tearDown(self) -> None:
        set_scalar_fp_exact_v2_module_flag(None)

    def test_legacy_status_assumes_precise_delivery(self) -> None:
        set_scalar_fp_exact_v2_module_flag(False)
        status = fe0_fe1_modeling_status()
        self.assertTrue(status["precise_delivery_assumed_under_traps_enabled"])
        self.assertFalse(status["imprecise_modes_modeled"])
        self.assertFalse(status["fe0"])
        self.assertFalse(status["fe1"])
        dims = traps_v2_ledger_dimensions()
        self.assertFalse(dims["fe0_fe1"])
        self.assertFalse(dims["traps"])

    def test_exact_v2_status_reports_wired_delivery(self) -> None:
        set_scalar_fp_exact_v2_module_flag(True)
        status = fe0_fe1_modeling_status()
        self.assertTrue(status["modes_documented"])
        self.assertTrue(status["fe0"])
        self.assertTrue(status["fe1"])
        self.assertTrue(status["imprecise_modes_modeled"])
        self.assertFalse(status["precise_delivery_assumed_under_traps_enabled"])
        self.assertTrue(status["live_fpscr_enables_when_exact_v2"])
        self.assertEqual(status["delivery_class"], "fe0-fe1-v2")
        dims = traps_v2_ledger_dimensions()
        self.assertTrue(dims["fe0_fe1"])
        self.assertTrue(dims["traps"])


class FE0Fe1SemanticsIntegrationTests(unittest.TestCase):
    def tearDown(self) -> None:
        set_scalar_fp_exact_v2_module_flag(None)

    def _run_fadd(self, *, msr: int, traps_enabled: bool) -> list:
        state = concrete_state({
            "fpr": {
                "f1": f"0x{_SNAN:016x}",
                "f2": f"0x{_ONE:016x}",
                "f3": f"0x{_ONE:016x}",
            },
            "fpscr": FPSCR_VE,
            "msr": msr,
        })
        domain = FloatingPointDomain(traps_enabled=traps_enabled)
        return [
            t for t in execute_cfg(
                state,
                _fadd_insns(),
                ConcreteOps(),
                floating_point_domain=domain,
            )
            if t.condition
        ]

    def test_legacy_v2_off_precise_trap_unchanged(self) -> None:
        set_scalar_fp_exact_v2_module_flag(False)
        terms = self._run_fadd(msr=0, traps_enabled=True)
        traps = [t for t in terms if t.exit_kind == "program-exception"]
        self.assertEqual(len(traps), 1)
        self.assertEqual(traps[0].exit_target, PROGRAM_EXCEPTION_VECTOR)
        self.assertEqual(traps[0].state.fpr[3], _ONE)

    def test_v2_precise_trap_with_suppressed_writeback(self) -> None:
        set_scalar_fp_exact_v2_module_flag(True)
        terms = self._run_fadd(msr=MSR_FE0, traps_enabled=True)
        traps = [t for t in terms if t.exit_kind == "program-exception"]
        continues = [t for t in terms if t.exit_kind == "fallthrough"]
        self.assertEqual(len(traps), 1)
        self.assertEqual(len(continues), 0)
        self.assertEqual(traps[0].state.fpr[3], _ONE)

    def test_v2_imprecise_nonrecoverable_pending_continue(self) -> None:
        set_scalar_fp_exact_v2_module_flag(True)
        terms = self._run_fadd(msr=0, traps_enabled=True)
        traps = [t for t in terms if t.exit_kind == "program-exception"]
        continues = [t for t in terms if t.exit_kind == "fallthrough"]
        self.assertEqual(len(traps), 0)
        self.assertEqual(len(continues), 1)
        cont = continues[0]
        self.assertNotEqual(cont.state.fpr[3], _ONE)
        self.assertTrue(int(cont.state.fp_pending_cause) & FPSCR_VXSNAN)
        self.assertEqual(int(cont.state.fp_pending_fault_pc), 0)
        self.assertFalse(bool(cont.state.fp_pending_recoverable))
        self.assertEqual(int(cont.state.fp_pending_delivery), FP_PENDING_IMPRECIS)

    def test_v2_imprecise_recoverable_pending(self) -> None:
        set_scalar_fp_exact_v2_module_flag(True)
        terms = self._run_fadd(msr=MSR_FE1, traps_enabled=True)
        cont = [t for t in terms if t.exit_kind == "fallthrough"][0]
        self.assertTrue(bool(cont.state.fp_pending_recoverable))
        self.assertEqual(int(cont.state.fp_pending_delivery), FP_PENDING_IMPRECIS)

    def test_v2_reserved_deferred_pending(self) -> None:
        set_scalar_fp_exact_v2_module_flag(True)
        terms = self._run_fadd(msr=MSR_FE0 | MSR_FE1, traps_enabled=True)
        cont = [t for t in terms if t.exit_kind == "fallthrough"][0]
        self.assertFalse(bool(cont.state.fp_pending_recoverable))
        self.assertEqual(int(cont.state.fp_pending_delivery), FP_PENDING_DEFERRED)

    def test_v2_flag_off_domain_traps_false_no_program_exception(self) -> None:
        set_scalar_fp_exact_v2_module_flag(True)
        terms = self._run_fadd(msr=MSR_FE0, traps_enabled=False)
        traps = [t for t in terms if t.exit_kind == "program-exception"]
        continues = [t for t in terms if t.exit_kind == "fallthrough"]
        self.assertEqual(len(traps), 0)
        self.assertEqual(len(continues), 1)
        self.assertEqual(continues[0].state.fpr[3], _ONE)


class CfgFpTrapBranchTests(unittest.TestCase):
    def tearDown(self) -> None:
        set_scalar_fp_exact_v2_module_flag(None)

    def test_cfg_branches_legacy_matches_enabled_edge(self) -> None:
        set_scalar_fp_exact_v2_module_flag(False)
        ops = ConcreteOps()
        state = concrete_state({"fpscr": FPSCR_VE, "msr": MSR_FE0})
        pre = FPSCR_VE
        post = FPSCR_VE | FPSCR_VXSNAN | (1 << 30)  # FEX
        _, trap, pending = cfg_fp_trap_branches(
            state,
            pre,
            post,
            0x100,
            ops,
            domain_traps_enabled=True,
            msr=state.msr,
        )
        self.assertTrue(bool(trap))
        self.assertFalse(bool(pending))

    def test_cfg_branches_v2_imprecise_pending_only(self) -> None:
        set_scalar_fp_exact_v2_module_flag(True)
        ops = ConcreteOps()
        state = concrete_state({"fpscr": FPSCR_VE, "msr": MSR_FE1})
        pre = FPSCR_VE
        post = FPSCR_VE | FPSCR_VXSNAN | (1 << 30)
        _, trap, pending = cfg_fp_trap_branches(
            state,
            pre,
            post,
            0x200,
            ops,
            domain_traps_enabled=True,
            msr=state.msr,
        )
        self.assertFalse(bool(trap))
        self.assertTrue(bool(pending))


if __name__ == "__main__":
    unittest.main()
