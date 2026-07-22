"""Per-side refinement discharge tests for recognized GX FIFO loops."""

from __future__ import annotations

import copy
import unittest
from dataclasses import replace

from tools.ppc_equivalence.gx_fifo_loop import build_gx_fifo_loop_plans
from tools.ppc_equivalence.gx_fifo_loop_discharge import (
    KNOWN_REFINEMENT_ALGORITHMS,
    REFINEMENT_ALGORITHM,
    REQUIRED_REFINEMENT_BLOCKS,
    discharge_gx_fifo_loop_plan,
    refinement_for_plan,
    validate_gx_fifo_loop_refinement_v2,
)
from tools.ppc_equivalence.ir import Instruction, Opcode


def _insn(
    opcode: Opcode,
    operands: tuple[int, ...],
    *,
    address: int = 0,
    link: bool = False,
) -> Instruction:
    return Instruction(address, 0, opcode, operands, link=link)


def _materialize_gx_base(reg: int, *, address: int = 0) -> list[Instruction]:
    """``addis reg,0,0xCC00; ori reg,reg,0x8000`` -> ``reg = 0xCC00_8000``."""
    return [
        _insn(Opcode.ADDIS, (reg, 0, 0xCC00), address=address),
        _insn(Opcode.ORI, (reg, reg, 0x8000), address=address + 4),
    ]


def _gx_fifo_program(
    *,
    count: int,
    store: tuple[Opcode, int],  # (opcode, source_reg); base reg is fixed at 5
    affine_step: int | None = None,
    base_reg: int = 5,
) -> list[Instruction]:
    """base-materialize; li count; mtctr; header:[store][affine addi]; bdnz; blr."""
    prologue = _materialize_gx_base(base_reg)
    pc = len(prologue) * 4
    prologue.append(_insn(Opcode.ADDI, (0, 0, count), address=pc))
    pc += 4
    prologue.append(_insn(Opcode.MTSPR, (0, 9), address=pc))
    pc += 4

    header_pc = pc
    opcode, source_reg = store
    body = [_insn(opcode, (source_reg, base_reg, 0), address=pc)]
    pc += 4
    if affine_step is not None:
        body.append(_insn(Opcode.ADDI, (source_reg, source_reg, affine_step), address=pc))
        pc += 4

    latch_pc = pc
    return [
        *prologue,
        *body,
        _insn(Opcode.BC, (16, 0, header_pc, 0), address=latch_pc),
        _insn(Opcode.BCLR, (20, 0, 0), address=latch_pc + 4),
    ]


def _build_plan(**kwargs):
    program = _gx_fifo_program(**kwargs)
    plans = build_gx_fifo_loop_plans(program)
    self_check = next(iter(plans.values()))
    return self_check


class DischargeGxFifoLoopPlanTests(unittest.TestCase):
    def test_empty_entry_violations_refuse_discharge(self) -> None:
        """Engine default: missing CFG violation conditions must not discharge."""
        plan = _build_plan(count=4, store=(Opcode.STW, 3))
        refused = discharge_gx_fifo_loop_plan(plan)
        self.assertFalse(refused.all_unsat())
        self.assertEqual(refused.status, "applied")
        self.assertIn("entry violation", refused.reason or "")

    def test_discharges_simple_invariant_stw_loop(self) -> None:
        plan = _build_plan(count=4, store=(Opcode.STW, 3))
        result = discharge_gx_fifo_loop_plan(plan, require_entry_violations=False)
        self.assertTrue(result.all_unsat(), result.reason)
        self.assertEqual(result.status, "discharged")
        self.assertEqual(result.entry_guard["result"], "unsat")
        self.assertEqual(result.refinement["algorithm"], REFINEMENT_ALGORITHM)
        self.assertEqual(result.refinement["status"], "discharged")
        for name in REQUIRED_REFINEMENT_BLOCKS:
            block = result.refinement[name]
            self.assertEqual(block["result"], "unsat", name)
            self.assertEqual(len(block["query_sha256"]), 64)
            self.assertIn(block["algorithm"], KNOWN_REFINEMENT_ALGORITHMS)

    def test_discharges_stb_and_sth_widths(self) -> None:
        for opcode in (Opcode.STB, Opcode.STH):
            with self.subTest(opcode=opcode.value):
                plan = _build_plan(count=3, store=(opcode, 4))
                result = discharge_gx_fifo_loop_plan(plan, require_entry_violations=False)
                self.assertTrue(result.all_unsat(), result.reason)

    def test_discharges_affine_value_loop(self) -> None:
        plan = _build_plan(count=5, store=(Opcode.STW, 3), affine_step=4)
        result = discharge_gx_fifo_loop_plan(plan, require_entry_violations=False)
        self.assertTrue(result.all_unsat(), result.reason)

    def test_refinement_for_plan_returns_attachable_payload(self) -> None:
        plan = _build_plan(count=2, store=(Opcode.STW, 3))
        payload = refinement_for_plan(plan, require_entry_violations=False)
        self.assertIsNotNone(payload)
        assert payload is not None
        self.assertIn("entry_guard", payload)
        self.assertIn("refinement", payload)
        self.assertIsNone(
            validate_gx_fifo_loop_refinement_v2(payload["refinement"]),
        )

    def test_refinement_for_plan_none_without_entry_violations(self) -> None:
        plan = _build_plan(count=2, store=(Opcode.STW, 3))
        self.assertIsNone(refinement_for_plan(plan))

    def test_event_identity_bound_and_footprint_notes(self) -> None:
        plan = _build_plan(count=4, store=(Opcode.STW, 3))
        result = discharge_gx_fifo_loop_plan(plan, require_entry_violations=False)
        self.assertTrue(result.all_unsat(), result.reason)
        event_identity = result.refinement["event_identity"]
        self.assertEqual(event_identity["device_id"], "gx-fifo")
        bound = result.refinement["bound"]
        self.assertEqual(bound["trip_count"], 4)
        self.assertEqual(bound["max_events"], plan.summary.max_events)

    def test_bound_exceeded_fails_before_smt(self) -> None:
        # max_events smaller than count means build_gx_fifo_loop_plan already
        # refuses recognition; simulate a caller-mutated plan sneaking past
        # recognition by lowering the summary's own max_events after the fact.
        plan = _build_plan(count=10, store=(Opcode.STW, 3))
        mutated_summary = replace(plan.summary, max_events=4)
        mutated_plan = replace(plan, summary=mutated_summary)
        result = discharge_gx_fifo_loop_plan(mutated_plan, require_entry_violations=False)
        self.assertFalse(result.all_unsat())
        self.assertEqual(result.status, "failed")
        self.assertIn("bound", result.reason or "")


class DischargeInternalErrorTests(unittest.TestCase):
    def test_mismatched_affine_step_is_internal_error(self) -> None:
        """A summary whose affine step disagrees with the real addi must SAT (never NOT_EQUIVALENT)."""
        plan = _build_plan(count=3, store=(Opcode.STW, 3), affine_step=1)
        mutated_summary = replace(
            plan.summary,
            value_expr={**plan.summary.value_expr, "step": 2},
        )
        mutated_plan = replace(plan, summary=mutated_summary)
        result = discharge_gx_fifo_loop_plan(mutated_plan, require_entry_violations=False)
        self.assertFalse(result.all_unsat())
        self.assertEqual(result.status, "internal_error")
        self.assertEqual(result.proof_status_hint, "INTERNAL_ERROR")
        self.assertEqual(result.refinement["body_step"]["result"], "sat")

    def test_mismatched_final_ctr_is_internal_error(self) -> None:
        """A summary claiming the wrong post-loop CTR must SAT postcondition."""
        plan = _build_plan(count=3, store=(Opcode.STW, 3))
        mutated_summary = replace(plan.summary, final_ctr=5)
        mutated_plan = replace(plan, summary=mutated_summary)
        result = discharge_gx_fifo_loop_plan(mutated_plan, require_entry_violations=False)
        self.assertFalse(result.all_unsat())
        self.assertEqual(result.status, "internal_error")
        self.assertEqual(result.refinement["postcondition"]["result"], "sat")


class ValidateGxFifoLoopRefinementTests(unittest.TestCase):
    def _discharged_refinement(self) -> dict:
        plan = _build_plan(count=4, store=(Opcode.STW, 3))
        result = discharge_gx_fifo_loop_plan(plan, require_entry_violations=False)
        self.assertTrue(result.all_unsat(), result.reason)
        return result.refinement

    def test_valid_discharged_refinement_accepted(self) -> None:
        refinement = self._discharged_refinement()
        self.assertIsNone(validate_gx_fifo_loop_refinement_v2(refinement))

    def test_rejects_placeholder_digest(self) -> None:
        refinement = copy.deepcopy(self._discharged_refinement())
        refinement["body_step"]["query_sha256"] = "0" * 64
        reason = validate_gx_fifo_loop_refinement_v2(refinement)
        self.assertIsNotNone(reason)
        assert reason is not None
        self.assertIn("placeholder", reason)

    def test_rejects_caller_injected_unsat_without_valid_digest(self) -> None:
        refinement = copy.deepcopy(self._discharged_refinement())
        refinement["postcondition"]["query_sha256"] = "not-a-digest"
        reason = validate_gx_fifo_loop_refinement_v2(refinement)
        self.assertIsNotNone(reason)

    def test_rejects_unknown_block_algorithm(self) -> None:
        refinement = copy.deepcopy(self._discharged_refinement())
        refinement["termination"]["algorithm"] = "totally-invented-algorithm"
        reason = validate_gx_fifo_loop_refinement_v2(refinement)
        self.assertIsNotNone(reason)

    def test_v1_top_level_algorithm_never_promotion_grade(self) -> None:
        refinement = copy.deepcopy(self._discharged_refinement())
        refinement["algorithm"] = "gx-fifo-loop-refinement-v1"
        reason = validate_gx_fifo_loop_refinement_v2(refinement)
        self.assertIsNotNone(reason)
        assert reason is not None
        self.assertIn("stale", reason)
        self.assertIn("never promotion-grade", reason)

    def test_v1_block_algorithm_never_promotion_grade(self) -> None:
        refinement = copy.deepcopy(self._discharged_refinement())
        refinement["body_step"]["algorithm"] = "gx-fifo-loop-body-step-v1"
        reason = validate_gx_fifo_loop_refinement_v2(refinement)
        self.assertIsNotNone(reason)
        assert reason is not None
        self.assertIn("v1", reason)

    def test_missing_block_rejected_when_discharged(self) -> None:
        refinement = copy.deepcopy(self._discharged_refinement())
        del refinement["footprint"]
        reason = validate_gx_fifo_loop_refinement_v2(refinement)
        self.assertIsNotNone(reason)
        assert reason is not None
        self.assertIn("footprint", reason)

    def test_non_discharged_status_tolerates_absent_blocks(self) -> None:
        self.assertIsNone(
            validate_gx_fifo_loop_refinement_v2(
                {"algorithm": REFINEMENT_ALGORITHM, "status": "applied"},
            ),
        )

    def test_rejects_non_object(self) -> None:
        self.assertIsNotNone(validate_gx_fifo_loop_refinement_v2([]))  # type: ignore[arg-type]


if __name__ == "__main__":
    unittest.main()
