"""Regression tests for adversarial-review soundness remediations."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.contract import (
    make_contract,
    refine_auto_contract_with_writes,
)
from tools.ppc_equivalence.decoder import decode_block
from tools.ppc_equivalence.engine import check_equivalence
from tools.ppc_equivalence.jump_table_obligations import (
    jump_table_gate_reason,
    unresolved_indirect_exit_gate_reason,
)
from tools.ppc_equivalence.nan_freedom import (
    NAN_FREEDOM_ASSUMPTION,
    NAN_FREEDOM_UNSUPPORTED,
    enforce_nan_freedom,
)
from tools.ppc_equivalence.result import FloatingPointDomain, ProofResult, ProofStatus
from tools.ppc_equivalence.semantics import ConcreteOps, SymbolicOps, automatic_live_out, execute_cfg


def _hex_to_insns(hex_bytes: str, base: int = 0x80001000):
    return decode_block(bytes.fromhex(hex_bytes.replace(" ", "")), base)


def _auto_prove(original_hex: str, candidate_hex: str, **kwargs):
    original = _hex_to_insns(original_hex)
    candidate = _hex_to_insns(candidate_hex, base=0x80002000)
    contract = make_contract(
        preset="auto",
        observe=None,
        timeout_ms=10_000,
        original_live_out=automatic_live_out(original),
        candidate_live_out=automatic_live_out(candidate),
    )
    return check_equivalence(
        original,
        candidate,
        contract,
        original_hex=original_hex.replace(" ", ""),
        candidate_hex=candidate_hex.replace(" ", ""),
        **kwargs,
    )


class AutomaticLiveOutFrspTests(unittest.TestCase):
    def test_frsp_and_fctiw_mark_fpscr(self) -> None:
        # frsp f1,f1
        self.assertIn("fpscr", automatic_live_out(_hex_to_insns("fc200018")))
        # fctiwz f1,f1
        self.assertIn("fpscr", automatic_live_out(_hex_to_insns("fc20001e")))


class AutoRefineTests(unittest.TestCase):
    def test_refine_adds_symbolic_persistent_write(self) -> None:
        contract = make_contract(
            preset="auto",
            observe=None,
            timeout_ms=5_000,
            original_live_out=("r3",),
            candidate_live_out=("r3",),
        )
        self.assertNotIn("fpscr", {item.name for item in contract.observables})
        refined = refine_auto_contract_with_writes(
            contract, symbolic_writes={"fpscr"},
        )
        self.assertIn("fpscr", refined.auto_added)
        self.assertIn("fpscr", {item.name for item in refined.observables})


class EmptyTerminalGuardTests(unittest.TestCase):
    def test_unresolved_indirect_gate_fires_without_pattern(self) -> None:
        # mtctr r3 ; bctr  — no lwzx adjacency, so jump_table_gate may miss,
        # but unresolved_indirect_exit_gate must still fire after CFG.
        original = _hex_to_insns("7c6903a6 4e800420")  # mtctr r3; bctr
        candidate = original
        self.assertIsNone(jump_table_gate_reason(original, candidate))
        ops = SymbolicOps()
        from tools.ppc_equivalence.engine import _symbolic_initial

        initial = _symbolic_initial(ops)
        exits = execute_cfg(initial, original, ops, max_instructions=64, max_paths=16)
        reason = unresolved_indirect_exit_gate_reason(exits, exits)
        self.assertIsNotNone(reason)
        self.assertIn("indirect", reason or "")

    def test_bctr_without_context_is_inconclusive(self) -> None:
        result = _auto_prove("7c6903a6 4e800420", "7c6903a6 4e800420")
        self.assertEqual(result.status, ProofStatus.INCONCLUSIVE_UNSUPPORTED)
        self.assertTrue(
            any("indirect" in item for item in result.unsupported + result.warnings)
        )


class NanFreedomTests(unittest.TestCase):
    def test_allow_nan_false_skips_payload_gate(self) -> None:
        domain = FloatingPointDomain(allow_nan=False)
        result = ProofResult(status=ProofStatus.EQUIVALENT, observables=["f1"])
        ops = SymbolicOps()
        gated = enforce_nan_freedom(
            result,
            original_exits=[],
            candidate_exits=[],
            contract=make_contract(
                preset=None, observe=["f1"], timeout_ms=1_000,
            ),
            ops=ops,
            domain=domain,
            layout_constraints=[],
            used_fp=True,
        )
        self.assertEqual(gated.status, ProofStatus.EQUIVALENT)
        self.assertTrue(any("allow_nan=false" in item for item in gated.assumptions))

    def test_bitwise_fmr_is_exempt_from_nan_demotion(self) -> None:
        # fmr is bit-preserving: payload bits are compared exactly, so the
        # refined H2 gate correctly leaves identical fmr blocks EQUIVALENT.
        result = _auto_prove(
            "fc200090",
            "fc200090",
            floating_point_domain=FloatingPointDomain(allow_nan=True),
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT)
        self.assertFalse(any("nan-freedom:" in item for item in result.unsupported))

    def test_fp_theory_diverging_nan_exprs_demote(self) -> None:
        # Two terminals with syntactically different FP-theory NaN results —
        # collapse would make the divergence query miss a hardware difference.
        from tools.ppc_equivalence.engine import _symbolic_initial
        from tools.ppc_equivalence.semantics import Terminal

        ops = SymbolicOps()
        z3m = ops.z3
        initial = _symbolic_initial(ops)
        nan_a = z3m.fpToIEEEBV(z3m.fpNaN(z3m.Float64()))
        nan_b = z3m.fpToIEEEBV(z3m.fpAbs(z3m.fpNaN(z3m.Float64())))
        self.assertFalse(z3m.eq(nan_a, nan_b))

        left = Terminal(
            condition=z3m.BoolVal(True),
            state=initial.with_fpr(1, nan_a),
            exit_kind="return",
            exit_target=None,
        )
        right = Terminal(
            condition=z3m.BoolVal(True),
            state=initial.with_fpr(1, nan_b),
            exit_kind="return",
            exit_target=None,
        )
        contract = make_contract(preset=None, observe=["f1"], timeout_ms=2_000)
        result = ProofResult(status=ProofStatus.EQUIVALENT, observables=["f1"])
        gated = enforce_nan_freedom(
            result,
            original_exits=[left],
            candidate_exits=[right],
            contract=contract,
            ops=ops,
            domain=FloatingPointDomain(allow_nan=True),
            layout_constraints=[],
            used_fp=True,
        )
        self.assertEqual(gated.status, ProofStatus.INCONCLUSIVE_UNSUPPORTED)
        self.assertTrue(any("nan-freedom" in item for item in gated.unsupported))


class FrspAutoObservesFpscrTests(unittest.TestCase):
    def test_frsp_auto_includes_fpscr_observable(self) -> None:
        original = _hex_to_insns("fc200018")  # frsp f1,f1
        contract = make_contract(
            preset="auto",
            observe=None,
            timeout_ms=5_000,
            original_live_out=automatic_live_out(original),
            candidate_live_out=automatic_live_out(original),
        )
        self.assertIn("fpscr", {item.name for item in contract.observables})


class FpscrStickyStatusTests(unittest.TestCase):
    def test_symbolic_stickies_reported_incomplete(self) -> None:
        from tools.ppc_equivalence.fp_fpscr import fpscr_sticky_modeling_status

        status = fpscr_sticky_modeling_status()
        self.assertFalse(status["ox"])
        self.assertFalse(status["ux"])
        self.assertFalse(status["xx"])


class CalleeTierCTests(unittest.TestCase):
    def test_assumed_callees_force_tier_c(self) -> None:
        from tools.coop.lib.equivalence_policy import _compute_confidence_tier_legacy
        from tools.ppc_equivalence.result import ProofResult, ProofStatus

        result = ProofResult(
            status=ProofStatus.EQUIVALENT,
            assumed_callees=[0x80001234],
            observables=["r3", "r1", "memory"],
            engine_hash="a" * 64,
            certifier_hash="b" * 64,
            source_hash="c" * 64,
            git_commit="d" * 40,
            git_dirty=False,
        )
        # Incomplete provenance/ledger may also force C; presence of callees alone
        # must not land in B.
        tier = _compute_confidence_tier_legacy(result, ledger=None)
        self.assertEqual(tier, "C")


if __name__ == "__main__":
    unittest.main()
