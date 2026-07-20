"""PR18 FP trap delivery scaffold tests (Wave 4 Track D)."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.contract import make_contract
from tools.ppc_equivalence.decoder import decode_block, parse_hex
from tools.ppc_equivalence.engine import check_equivalence
from tools.ppc_equivalence.fp_oracle import fadd_binary64_rne, fdiv_binary64_rne
from tools.ppc_equivalence.fp_outcome import outcome_from_oracle
from tools.ppc_equivalence.fp_traps import (
    PROGRAM_EXCEPTION_VECTOR,
    SRR1_FP_ENABLED_EXCEPTION,
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
    FPSCR_OE,
    FPSCR_VE,
    FPSCR_VX,
    FPSCR_VXISI,
    FPSCR_VXSNAN,
    FPSCR_ZE,
    FPSCR_ZX,
    _exception_entry,
    execute_cfg,
)

# Signaling NaN and ±Inf bit patterns for exception edges.
_SNAN = 0x7FF0000012345678
_PINF = 0x7FF0000000000000
_NINF = 0xFFF0000000000000
_ONE = 0x3FF0000000000000
_ZERO = 0x0000000000000000


def _fadd_insns() -> list[Instruction]:
    # fadd f3, f1, f2
    return decode_block(parse_hex("fc61102a"), validate_with_capstone=False)


def _fdiv_insns() -> list[Instruction]:
    # fdiv f3, f1, f2
    return decode_block(parse_hex("fc611024"), validate_with_capstone=False)


def _fres_insns() -> list[Instruction]:
    # fres f7, f2
    return decode_block(parse_hex("ece01030"), validate_with_capstone=False)


class FPTrapPolicyUnitTests(unittest.TestCase):
    def test_supported_opcode_set_is_scalar_outcome_family(self) -> None:
        names = set(supported_opcode_names())
        self.assertIn("fadd", names)
        self.assertIn("fdivs", names)
        self.assertIn("fmadd", names)
        self.assertNotIn("fres", names)
        self.assertNotIn("ps_add", names)
        self.assertEqual(len(TRAP_DELIVERY_SUPPORTED_OPS), 16)

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

    def test_oe_enable_is_incomplete(self) -> None:
        normal = outcome_from_oracle(fadd_binary64_rne(_ONE, _ONE))
        decision = resolve_fp_trap_policy(normal, FPSCR_OE)
        self.assertIsNotNone(decision.incomplete_reason)
        self.assertIn("OX/UX/XX", decision.incomplete_reason or "")

    def test_paired_policy_incomplete(self) -> None:
        lane = outcome_from_oracle(fadd_binary64_rne(_ONE, _ONE))
        decision = resolve_fp_trap_policy(lane, FPSCR_VE, paired=True)
        self.assertIsNotNone(decision.incomplete_reason)

    def test_ensure_incomplete_opcode_fail_closed(self) -> None:
        with self.assertRaises(ExecutionInconclusive) as ctx:
            ensure_fp_trap_delivery_supported(Opcode.FRES, 0)
        self.assertIn("incomplete", str(ctx.exception).lower())

        with self.assertRaises(ExecutionInconclusive) as ctx:
            ensure_fp_trap_delivery_supported(Opcode.FADD, FPSCR_OE)
        self.assertIn("OE/UE/XE", str(ctx.exception))


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

    def test_oe_set_fail_closed_under_traps_enabled(self) -> None:
        state = concrete_state({
            "fpr": {"f1": f"0x{_ONE:016x}", "f2": f"0x{_ONE:016x}"},
            "fpscr": FPSCR_OE,
        })
        with self.assertRaises(ExecutionInconclusive) as ctx:
            execute_cfg(
                state, _fadd_insns(), ConcreteOps(),
                floating_point_domain=FloatingPointDomain(traps_enabled=True),
            )
        self.assertIn("OE/UE/XE", str(ctx.exception))

    def test_exception_entry_vocabulary_matches_twi(self) -> None:
        state = concrete_state({"msr": 0x00010001, "fpscr": FPSCR_VE | FPSCR_FEX})
        entered = _exception_entry(state, 0x20, SRR1_FP_ENABLED_EXCEPTION, ConcreteOps())
        self.assertEqual(entered.srr0, 0x20)
        self.assertEqual(entered.srr1, (state.msr & 0x87C0FFFF) | SRR1_FP_ENABLED_EXCEPTION)


class FPTrapDomainAndEquivalenceTests(unittest.TestCase):
    def test_traps_enabled_domain_validate_and_coverage(self) -> None:
        domain = FloatingPointDomain(traps_enabled=True)
        domain.validate()  # must not raise
        coverage = domain.coverage_dict()
        self.assertNotIn("traps-disabled", coverage["assumed"])
        self.assertIn(FP_COVERAGE_TRAP_SCAFFOLD[0], coverage["assumed"])
        self.assertIn("fp-exception-trap-delivery", coverage["unsupported"])
        self.assertEqual(coverage["status"], "assumed")

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


if __name__ == "__main__":
    unittest.main()
