"""PR18 FP trap delivery tests (Wave 5 Track D)."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.contract import make_contract
from tools.ppc_equivalence.decoder import decode_block, parse_hex
from tools.ppc_equivalence.engine import check_equivalence
from tools.ppc_equivalence.fp_oracle import fadd_binary64_rne, fadds_fpr_rne, fdiv_binary64_rne
from tools.ppc_equivalence.fp_outcome import outcome_from_oracle
from tools.ppc_equivalence.fp_traps import (
    PROGRAM_EXCEPTION_VECTOR,
    SRR1_FP_ENABLED_EXCEPTION,
    TRAP_DELIVERY_PAIRED_OPS,
    TRAP_DELIVERY_SUPPORTED_OPS,
    ensure_fp_trap_delivery_supported,
    outcome_with_trap_policy,
    resolve_fp_trap_policy,
    supported_opcode_names,
)
from tools.ppc_equivalence.ir import ExecutionInconclusive, Instruction, Opcode
from tools.ppc_equivalence.model import concrete_state
from tools.ppc_equivalence.result import (
    FP_COVERAGE_TRAP_SCAFFOLD,
    FloatingPointDomain,
    ProofStatus,
)
from tools.ppc_equivalence.semantics import (
    ConcreteOps,
    FPSCR_FEX,
    FPSCR_FX,
    FPSCR_NI,
    FPSCR_OE,
    FPSCR_OX,
    FPSCR_UE,
    FPSCR_UX,
    FPSCR_VE,
    FPSCR_VX,
    FPSCR_VXISI,
    FPSCR_VXSNAN,
    FPSCR_XE,
    FPSCR_XX,
    FPSCR_ZE,
    FPSCR_ZX,
    _exception_entry,
    execute_cfg,
    execute_instruction,
)

# Signaling NaN and ±Inf bit patterns for exception edges.
_SNAN = 0x7FF0000012345678
_PINF = 0x7FF0000000000000
_NINF = 0xFFF0000000000000
_ONE = 0x3FF0000000000000
_ZERO = 0x0000000000000000
_HUGE_S = 0x47EFFFFFE0000000  # ~1.7e38 expanded binary32


def _fadd_insns() -> list[Instruction]:
    # fadd f3, f1, f2
    return decode_block(parse_hex("fc61102a"), validate_with_capstone=False)


def _fdiv_insns() -> list[Instruction]:
    # fdiv f3, f1, f2
    return decode_block(parse_hex("fc611024"), validate_with_capstone=False)


def _fadds_insns() -> list[Instruction]:
    # fadds f3, f1, f2
    return decode_block(parse_hex("ec61102a"), validate_with_capstone=False)


def _fres_insns() -> list[Instruction]:
    # fres f7, f2
    return decode_block(parse_hex("ece01030"), validate_with_capstone=False)


def _ps_add_insns() -> list[Instruction]:
    # ps_add f3, f1, f2
    return decode_block(parse_hex("1061102a"), validate_with_capstone=False)


class FPTrapPolicyUnitTests(unittest.TestCase):
    def test_supported_opcode_set_is_scalar_outcome_family(self) -> None:
        names = set(supported_opcode_names())
        self.assertIn("fadd", names)
        self.assertIn("fdivs", names)
        self.assertIn("fmadd", names)
        self.assertNotIn("fres", names)
        self.assertNotIn("ps_add", names)
        self.assertEqual(len(TRAP_DELIVERY_SUPPORTED_OPS), 16)
        self.assertIn(Opcode.PS_ADD, TRAP_DELIVERY_PAIRED_OPS)

    def test_outcome_policy_enabled_vs_disabled(self) -> None:
        snan_add = outcome_from_oracle(fadd_binary64_rne(_SNAN, _ONE))
        self.assertTrue(snan_add.flags.invalid)

        disabled = resolve_fp_trap_policy(snan_add, 0)
        self.assertFalse(disabled.trap)
        self.assertTrue(disabled.writeback)

        enabled = resolve_fp_trap_policy(snan_add, FPSCR_VE)
        self.assertTrue(enabled.trap)
        self.assertFalse(enabled.writeback)

        with_fields = outcome_with_trap_policy(snan_add, FPSCR_VE)
        self.assertTrue(with_fields.trap)
        self.assertFalse(with_fields.writeback)

    def test_divzero_ze_policy(self) -> None:
        div0 = outcome_from_oracle(fdiv_binary64_rne(_ONE, _ZERO))
        self.assertTrue(div0.flags.divide_by_zero)
        self.assertTrue(resolve_fp_trap_policy(div0, FPSCR_ZE).trap)
        self.assertFalse(resolve_fp_trap_policy(div0, 0).trap)

    def test_oe_overflow_policy(self) -> None:
        overflow = outcome_from_oracle(fadds_fpr_rne(_HUGE_S, _HUGE_S))
        self.assertTrue(overflow.flags.overflow)
        decision = resolve_fp_trap_policy(overflow, FPSCR_OE)
        self.assertTrue(decision.trap)
        self.assertFalse(decision.writeback)
        self.assertIsNone(decision.incomplete_reason)

    def test_xe_inexact_policy(self) -> None:
        # 1.0 / 3.0 is inexact under SoftFloat RNE.
        inexact = outcome_from_oracle(
            fdiv_binary64_rne(_ONE, 0x4008000000000000),
        )
        self.assertTrue(inexact.flags.inexact)
        self.assertTrue(resolve_fp_trap_policy(inexact, FPSCR_XE).trap)
        self.assertFalse(resolve_fp_trap_policy(inexact, 0).trap)

    def test_paired_policy_traps_with_unconditional_writeback(self) -> None:
        lane = outcome_from_oracle(fadd_binary64_rne(_SNAN, _ONE))
        decision = resolve_fp_trap_policy(lane, FPSCR_VE, paired=True)
        self.assertTrue(decision.trap)
        self.assertTrue(decision.writeback)
        self.assertIsNone(decision.incomplete_reason)

    def test_ensure_incomplete_opcode_fail_closed(self) -> None:
        with self.assertRaises(ExecutionInconclusive) as ctx:
            ensure_fp_trap_delivery_supported(Opcode.FRES, 0)
        self.assertIn("incomplete", str(ctx.exception).lower())

        # OE is modeled for scalar SoftFloat family.
        ensure_fp_trap_delivery_supported(Opcode.FADD, FPSCR_OE)


class FPTrapCfgConcreteTests(unittest.TestCase):
    def test_enabled_ve_traps_disabled_continues(self) -> None:
        state = concrete_state({
            "fpr": {"f1": f"0x{_PINF:016x}", "f2": f"0x{_NINF:016x}", "f3": _ONE},
            "fpscr": FPSCR_VE,
            "msr": 0x00010001,
        })
        insns = _fadd_insns()

        disabled = [
            t for t in execute_cfg(
                state, insns, ConcreteOps(),
                floating_point_domain=FloatingPointDomain(traps_enabled=False),
            )
            if t.condition
        ]
        self.assertEqual(len(disabled), 1)
        self.assertEqual(disabled[0].exit_kind, "fallthrough")
        # VE suppresses destination writeback even without trap delivery.
        self.assertEqual(disabled[0].state.fpr[3], _ONE)
        self.assertEqual(
            disabled[0].state.fpscr & (FPSCR_FEX | FPSCR_VX | FPSCR_VXISI | FPSCR_VE),
            FPSCR_FEX | FPSCR_VX | FPSCR_VXISI | FPSCR_VE,
        )

        enabled = [
            t for t in execute_cfg(
                state, insns, ConcreteOps(),
                floating_point_domain=FloatingPointDomain(traps_enabled=True),
            )
            if t.condition
        ]
        traps = [t for t in enabled if t.exit_kind == "program-exception"]
        continues = [t for t in enabled if t.exit_kind == "fallthrough"]
        self.assertEqual(len(traps), 1)
        self.assertEqual(len(continues), 0)
        trap = traps[0]
        self.assertEqual(trap.exit_target, PROGRAM_EXCEPTION_VECTOR)
        self.assertEqual(trap.state.srr0, 0)
        expected_srr1 = (state.msr & 0x87C0FFFF) | SRR1_FP_ENABLED_EXCEPTION
        self.assertEqual(trap.state.srr1, expected_srr1)
        expected_msr = (((state.msr & ~1) | ((state.msr >> 16) & 1)) & ~0x0004EF36) & 0xFFFFFFFF
        self.assertEqual(trap.state.msr, expected_msr)
        # Destination remains suppressed on the trap path.
        self.assertEqual(trap.state.fpr[3], _ONE)

    def test_disabled_ze_divzero_continues_with_sticky(self) -> None:
        state = concrete_state({
            "fpr": {"f1": f"0x{_ONE:016x}", "f2": f"0x{_ZERO:016x}"},
            "fpscr": FPSCR_ZE,
        })
        terms = [
            t for t in execute_cfg(
                state, _fdiv_insns(), ConcreteOps(),
                floating_point_domain=FloatingPointDomain(traps_enabled=False),
            )
            if t.condition
        ]
        self.assertEqual(terms[0].exit_kind, "fallthrough")
        self.assertTrue(terms[0].state.fpscr & FPSCR_ZX)

    def test_enabled_ze_divzero_traps(self) -> None:
        state = concrete_state({
            "fpr": {"f1": f"0x{_ONE:016x}", "f2": f"0x{_ZERO:016x}", "f3": _ONE},
            "fpscr": FPSCR_ZE,
            "msr": 0x00010001,
        })
        terms = [
            t for t in execute_cfg(
                state, _fdiv_insns(), ConcreteOps(),
                floating_point_domain=FloatingPointDomain(traps_enabled=True),
            )
            if t.condition
        ]
        traps = [t for t in terms if t.exit_kind == "program-exception"]
        self.assertEqual(len(traps), 1)
        self.assertEqual(traps[0].exit_target, PROGRAM_EXCEPTION_VECTOR)
        self.assertEqual(traps[0].state.fpr[3], _ONE)

    def test_enabled_oe_overflow_traps_and_suppresses(self) -> None:
        state = concrete_state({
            "fpr": {
                "f1": f"0x{_HUGE_S:016x}",
                "f2": f"0x{_HUGE_S:016x}",
                "f3": _ONE,
            },
            "fpscr": FPSCR_OE,
            "msr": 0x00010001,
        })
        domain = FloatingPointDomain(
            traps_enabled=True,
            exclude_finite_overflow=False,
        )
        terms = [
            t for t in execute_cfg(
                state, _fadds_insns(), ConcreteOps(),
                floating_point_domain=domain,
            )
            if t.condition
        ]
        traps = [t for t in terms if t.exit_kind == "program-exception"]
        self.assertEqual(len(traps), 1)
        self.assertEqual(traps[0].state.fpr[3], _ONE)
        self.assertTrue(traps[0].state.fpscr & FPSCR_OX)
        self.assertTrue(traps[0].state.fpscr & FPSCR_FEX)

    def test_oe_overflow_continues_with_sticky_when_traps_disabled(self) -> None:
        state = concrete_state({
            "fpr": {
                "f1": f"0x{_HUGE_S:016x}",
                "f2": f"0x{_HUGE_S:016x}",
                "f3": _ONE,
            },
            "fpscr": FPSCR_OE,
        })
        domain = FloatingPointDomain(
            traps_enabled=False,
            exclude_finite_overflow=False,
        )
        terms = [
            t for t in execute_cfg(
                state, _fadds_insns(), ConcreteOps(),
                floating_point_domain=domain,
            )
            if t.condition
        ]
        self.assertEqual(terms[0].exit_kind, "fallthrough")
        self.assertTrue(terms[0].state.fpscr & FPSCR_OX)
        self.assertTrue(terms[0].state.fpscr & FPSCR_FEX)
        # OE suppresses destination even without trap delivery.
        self.assertEqual(terms[0].state.fpr[3], _ONE)

    def test_fex_already_set_retraps_on_new_enabled_exception(self) -> None:
        # FEX already set from prior OX|OE; another VE invalid must re-trap.
        state = concrete_state({
            "fpr": {
                "f1": f"0x{_PINF:016x}",
                "f2": f"0x{_NINF:016x}",
                "f3": _ONE,
            },
            "fpscr": FPSCR_OE | FPSCR_OX | FPSCR_FX | FPSCR_FEX | FPSCR_VE,
            "msr": 0x00010001,
        })
        terms = [
            t for t in execute_cfg(
                state, _fadd_insns(), ConcreteOps(),
                floating_point_domain=FloatingPointDomain(traps_enabled=True),
            )
            if t.condition
        ]
        traps = [t for t in terms if t.exit_kind == "program-exception"]
        self.assertEqual(len(traps), 1)
        self.assertEqual(traps[0].exit_target, PROGRAM_EXCEPTION_VECTOR)

    def test_fex_already_set_no_trap_without_new_enabled_exception(self) -> None:
        state = concrete_state({
            "fpr": {
                "f1": f"0x{_ONE:016x}",
                "f2": f"0x{_ONE:016x}",
                "f3": 0,
            },
            "fpscr": FPSCR_OE | FPSCR_OX | FPSCR_FX | FPSCR_FEX,
            "msr": 0x00010001,
        })
        terms = [
            t for t in execute_cfg(
                state, _fadd_insns(), ConcreteOps(),
                floating_point_domain=FloatingPointDomain(traps_enabled=True),
            )
            if t.condition
        ]
        self.assertEqual(len(terms), 1)
        self.assertEqual(terms[0].exit_kind, "fallthrough")
        self.assertEqual(terms[0].state.fpr[3], 0x4000000000000000)

    def test_paired_ve_traps_with_unconditional_writeback(self) -> None:
        state = concrete_state({
            "fpr": {
                "f1": f"0x{_SNAN:016x}",
                "f2": f"0x{_ONE:016x}",
                "f3": 0,
            },
            "ps1": {
                "f1": f"0x{_ONE:016x}",
                "f2": f"0x{_ONE:016x}",
                "f3": 0,
            },
            "fpscr": FPSCR_VE,
            "msr": 0x00010001,
        })
        terms = [
            t for t in execute_cfg(
                state, _ps_add_insns(), ConcreteOps(),
                floating_point_domain=FloatingPointDomain(traps_enabled=True),
            )
            if t.condition
        ]
        traps = [t for t in terms if t.exit_kind == "program-exception"]
        self.assertEqual(len(traps), 1)
        # Paired writeback is unconditional under VE.
        self.assertNotEqual(traps[0].state.fpr[3], 0)

    def test_incomplete_opcode_fail_closed_under_traps_enabled(self) -> None:
        state = concrete_state({
            "fpr": {"f2": 0.0},
            "fpscr": FPSCR_ZE,
        })
        with self.assertRaises(ExecutionInconclusive) as ctx:
            execute_cfg(
                state, _fres_insns(), ConcreteOps(),
                floating_point_domain=FloatingPointDomain(traps_enabled=True),
            )
        self.assertIn("incomplete", str(ctx.exception).lower())

    def test_exception_entry_vocabulary_matches_twi(self) -> None:
        state = concrete_state({"msr": 0x00010001, "fpscr": FPSCR_VE | FPSCR_FEX})
        entered = _exception_entry(state, 0x20, SRR1_FP_ENABLED_EXCEPTION, ConcreteOps())
        self.assertEqual(entered.srr0, 0x20)
        self.assertEqual(entered.srr1, (state.msr & 0x87C0FFFF) | SRR1_FP_ENABLED_EXCEPTION)


class FPTrapNiInteractionTests(unittest.TestCase):
    def test_ni_flush_then_ve_trap_on_supported_op(self) -> None:
        # Subnormal + Inf under NI: operand flush yields 0+Inf (no invalid);
        # use SNaN so VE still traps after NI flush (SNaN is not a denormal).
        state = concrete_state({
            "fpr": {
                "f1": f"0x{_SNAN:016x}",
                "f2": f"0x{_ONE:016x}",
                "f3": _ONE,
            },
            "fpscr": FPSCR_NI | FPSCR_VE,
            "msr": 0x00010001,
        })
        terms = [
            t for t in execute_cfg(
                state, _fadd_insns(), ConcreteOps(),
                floating_point_domain=FloatingPointDomain(
                    traps_enabled=True,
                    require_ni_zero=False,
                ),
            )
            if t.condition
        ]
        traps = [t for t in terms if t.exit_kind == "program-exception"]
        self.assertEqual(len(traps), 1)
        self.assertEqual(traps[0].state.fpr[3], _ONE)

    def test_ni_unsupported_with_traps_stays_inconclusive(self) -> None:
        state = concrete_state({
            "fpr": {"f2": f"0x{_ONE:016x}"},
            "fpscr": FPSCR_NI | FPSCR_ZE,
        })
        with self.assertRaises(ExecutionInconclusive) as ctx:
            execute_cfg(
                state, _fres_insns(), ConcreteOps(),
                floating_point_domain=FloatingPointDomain(
                    traps_enabled=True,
                    require_ni_zero=False,
                ),
            )
        message = str(ctx.exception).lower()
        self.assertTrue("ni" in message or "incomplete" in message)


class FPTrapDomainAndEquivalenceTests(unittest.TestCase):
    def test_traps_enabled_domain_validate_and_coverage(self) -> None:
        domain = FloatingPointDomain(traps_enabled=True)
        domain.validate()  # must not raise
        coverage = domain.coverage_dict()
        self.assertNotIn("traps-disabled", coverage["assumed"])
        self.assertIn(FP_COVERAGE_TRAP_SCAFFOLD[0], coverage["assumed"])
        self.assertIn(
            "fp-exception-trap-delivery-estimates-compares-converts",
            coverage["unsupported"],
        )
        self.assertEqual(coverage["status"], "assumed")
        names = domain.trap_delivery_supported_opcodes()
        self.assertIn("fadd", names)
        self.assertIn("ps_add", names)

    def test_enabled_vs_disabled_exit_kinds_differ_observably(self) -> None:
        """Control behavior differs: trap terminal vs fallthrough."""
        state = concrete_state({
            "fpr": {"f1": f"0x{_SNAN:016x}", "f2": f"0x{_ONE:016x}"},
            "fpscr": FPSCR_VE,
            "msr": 0x00010001,
        })
        insns = _fadd_insns()
        enabled_kinds = {
            t.exit_kind
            for t in execute_cfg(
                state, insns, ConcreteOps(),
                floating_point_domain=FloatingPointDomain(traps_enabled=True),
            )
            if t.condition
        }
        disabled_kinds = {
            t.exit_kind
            for t in execute_cfg(
                state, insns, ConcreteOps(),
                floating_point_domain=FloatingPointDomain(traps_enabled=False),
            )
            if t.condition
        }
        self.assertEqual(enabled_kinds, {"program-exception"})
        self.assertEqual(disabled_kinds, {"fallthrough"})

    def test_equivalence_same_code_with_traps_enabled(self) -> None:
        code = parse_hex("fc61102a")  # fadd f3, f1, f2
        insns = decode_block(code, 0x80000000)
        contract = make_contract(preset=None, observe=["f3", "fpscr"], timeout_ms=10_000)
        result = check_equivalence(
            insns, insns, contract,
            original_hex=code.hex(),
            candidate_hex=code.hex(),
            floating_point_domain=FloatingPointDomain(traps_enabled=True),
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT, result.unsupported)
        assert result.floating_point_domain is not None
        self.assertTrue(result.floating_point_domain.traps_enabled)

    def test_softfloat_latches_ox_on_overflow(self) -> None:
        state = execute_instruction(
            concrete_state({
                "fpr": {
                    "f1": f"0x{_HUGE_S:016x}",
                    "f2": f"0x{_HUGE_S:016x}",
                },
            }),
            Instruction(0, 0, Opcode.FADDS, (1, 1, 2)),
            ConcreteOps(),
            floating_point_domain=FloatingPointDomain(exclude_finite_overflow=False),
        )
        self.assertTrue(state.fpscr & FPSCR_OX)
        self.assertTrue(state.fpscr & FPSCR_XX)


if __name__ == "__main__":
    unittest.main()
