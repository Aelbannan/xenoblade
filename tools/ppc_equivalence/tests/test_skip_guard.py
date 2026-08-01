"""Skip-guard recognition and SMT discharge (doc 30 Phase B)."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.deadline import Deadline
from tools.ppc_equivalence.ir import ExecutionInconclusive, Instruction, Opcode
from tools.ppc_equivalence.loop_summary import find_ctr_affine_loop_candidates
from tools.ppc_equivalence.memory_loop import (
    build_memory_loop_plan_map,
    find_constant_stride_store_loops,
    summarize_constant_stride_store_loop,
)
from tools.ppc_equivalence.model import concrete_state
from tools.ppc_equivalence.semantics import ConcreteOps, SymbolicOps, execute_cfg
from tools.ppc_equivalence.skip_guard import (
    SkipGuardInfo,
    discharge_skip_guard,
    find_mtctr_with_guard,
    find_skip_guard,
    guard_family,
)
from tools.ppc_equivalence.trip_expression import (
    TripAnd,
    TripEntryReg,
    TripLshr,
    recognize_trip_expr,
)


def _insn(
    opcode: Opcode,
    operands: tuple[int, ...],
    *,
    address: int = 0,
    record: bool = False,
) -> Instruction:
    return Instruction(address, 0, opcode, operands, record=record)


def _shape1_bulk_loop(*, header: int = 12) -> list[Instruction]:
    """[srwi. r0,r6,3][mtctr r0][beq exit][header: stw][addi][bdnz] (guard between)."""
    return [
        _insn(Opcode.RLWINM, (0, 6, 29, 3, 31), address=0, record=True),  # srwi. r0,r6,3
        _insn(Opcode.MTSPR, (0, 9), address=4),
        _insn(Opcode.BC, (12, 2, 32, 0), address=8),  # beq exit(32)
        _insn(Opcode.STW, (3, 4, 0), address=header),
        _insn(Opcode.ADDI, (4, 4, 4), address=header + 4),
        _insn(Opcode.BC, (16, 0, header, 0), address=header + 8),
        _insn(Opcode.BCLR, (20, 0, 0), address=32),
    ]


def _shape2_remainder_loop(*, header: int = 16) -> list[Instruction]:
    """[andi. r6,r6,7][beq exit][mtctr r6][nop][header: stw][addi][bdnz][blr] (guard before mtctr)."""
    return [
        _insn(Opcode.ANDI_DOT, (6, 6, 7), address=0, record=True),
        _insn(Opcode.BC, (12, 2, 28, 0), address=4),  # beq exit(28)
        _insn(Opcode.MTSPR, (6, 9), address=8),
        _insn(Opcode.ORI, (0, 0, 0), address=12),  # nop
        _insn(Opcode.STW, (3, 4, 0), address=header),
        _insn(Opcode.ADDI, (4, 4, 4), address=header + 4),
        _insn(Opcode.BC, (16, 0, header, 0), address=header + 8),
        _insn(Opcode.BCLR, (20, 0, 0), address=28),
    ]


class GuardRecognitionTests(unittest.TestCase):
    def test_shape1_guard_between_mtctr_and_header(self) -> None:
        program = _shape1_bulk_loop()
        header = next(
            i for i, insn in enumerate(program) if insn.opcode == Opcode.STW
        )
        mtctr_index, guard = find_mtctr_with_guard(program, header)
        self.assertEqual(mtctr_index, header - 2)
        self.assertIsNotNone(guard)
        assert guard is not None
        self.assertEqual(guard.family, "beq")
        self.assertEqual(guard.trip_def_index, header - 3)
        self.assertEqual(guard.target_pc, 32)

    def test_shape2_guard_before_mtctr_with_nop(self) -> None:
        program = _shape2_remainder_loop()
        header = next(
            i for i, insn in enumerate(program) if insn.opcode == Opcode.STW
        )
        mtctr_index, guard = find_mtctr_with_guard(program, header)
        self.assertEqual(mtctr_index, header - 2)  # nop between mtctr and header
        self.assertIsNotNone(guard)
        assert guard is not None
        self.assertEqual(guard.family, "beq")
        self.assertEqual(guard.guard_index, header - 3)

    def test_adjacent_mtctr_no_guard_still_recognized(self) -> None:
        program = [
            _insn(Opcode.ADDI, (6, 0, 5), address=0),
            _insn(Opcode.MTSPR, (6, 9), address=4),
            _insn(Opcode.STW, (3, 4, 0), address=8),
            _insn(Opcode.ADDI, (4, 4, 4), address=12),
            _insn(Opcode.BC, (16, 0, 8, 0), address=16),
        ]
        mtctr_index, guard = find_mtctr_with_guard(program, 2)
        self.assertEqual(mtctr_index, 1)
        self.assertIsNone(guard)

    def test_pure_padding_adjacency_relaxation(self) -> None:
        # mtctr two instructions back with a nop between (no guard).
        program = [
            _insn(Opcode.ADDI, (6, 0, 5), address=0),
            _insn(Opcode.MTSPR, (6, 9), address=4),
            _insn(Opcode.ORI, (0, 0, 0), address=8),  # nop
            _insn(Opcode.STW, (3, 4, 0), address=12),
            _insn(Opcode.ADDI, (4, 4, 4), address=16),
            _insn(Opcode.BC, (16, 0, 12, 0), address=20),
        ]
        mtctr_index, guard = find_mtctr_with_guard(program, 3)
        self.assertEqual(mtctr_index, 1)
        self.assertIsNone(guard)

    def test_rejects_non_padding_between_mtctr_and_header(self) -> None:
        program = [
            _insn(Opcode.ADDI, (6, 0, 5), address=0),
            _insn(Opcode.MTSPR, (6, 9), address=4),
            _insn(Opcode.ADDI, (7, 7, 1), address=8),  # not padding
            _insn(Opcode.STW, (3, 4, 0), address=12),
            _insn(Opcode.BC, (16, 0, 12, 0), address=16),
        ]
        mtctr_index, _guard = find_mtctr_with_guard(program, 3)
        self.assertIsNone(mtctr_index)

    def test_guard_jumping_into_loop_body_rejected(self) -> None:
        # Guard target = the latch (20, the bdnz), which re-enters the header
        # via the back-edge — not a skip guard.
        program = [
            _insn(Opcode.ANDI_DOT, (6, 6, 7), address=0, record=True),
            _insn(Opcode.BC, (12, 2, 20, 0), address=4),  # beq latch
            _insn(Opcode.MTSPR, (6, 9), address=8),
            _insn(Opcode.STW, (3, 4, 0), address=12),
            _insn(Opcode.ADDI, (4, 4, 4), address=16),
            _insn(Opcode.BC, (16, 0, 12, 0), address=20),  # bdnz header
            _insn(Opcode.BCLR, (20, 0, 0), address=24),
        ]
        mtctr_index, guard = find_mtctr_with_guard(program, 3)
        self.assertEqual(mtctr_index, 2)
        self.assertIsNone(guard)

    def test_trip_def_must_be_dot_form(self) -> None:
        # li does not set CR0; the guard's condition is not the trip value.
        program = [
            _insn(Opcode.ADDI, (6, 0, 7), address=0),  # li r6,7 (no dot)
            _insn(Opcode.BC, (12, 2, 28, 0), address=4),  # beq exit
            _insn(Opcode.MTSPR, (6, 9), address=8),
            _insn(Opcode.STW, (3, 4, 0), address=12),
            _insn(Opcode.ADDI, (4, 4, 4), address=16),
            _insn(Opcode.BC, (16, 0, 12, 0), address=20),
            _insn(Opcode.BCLR, (20, 0, 0), address=28),
        ]
        mtctr_index, guard = find_mtctr_with_guard(program, 3)
        self.assertEqual(mtctr_index, 2)
        self.assertIsNone(guard)

    def test_cr0_clobber_between_dot_form_and_guard_rejected(self) -> None:
        program = [
            _insn(Opcode.ANDI_DOT, (6, 6, 7), address=0, record=True),
            _insn(Opcode.CROR, (2, 2, 2), address=4),  # clobbers CR0.EQ
            _insn(Opcode.BC, (12, 2, 32, 0), address=8),  # beq exit
            _insn(Opcode.MTSPR, (6, 9), address=12),
            _insn(Opcode.STW, (3, 4, 0), address=16),
            _insn(Opcode.ADDI, (4, 4, 4), address=20),
            _insn(Opcode.BC, (16, 0, 16, 0), address=24),
            _insn(Opcode.BCLR, (20, 0, 0), address=32),
        ]
        mtctr_index, guard = find_mtctr_with_guard(program, 4)
        self.assertEqual(mtctr_index, 3)
        self.assertIsNone(guard)

    def test_guard_family_decode_includes_prediction_bits(self) -> None:
        self.assertEqual(guard_family(12, 2), "beq")
        self.assertEqual(guard_family(13, 2), "beq")  # beq+ hint
        self.assertEqual(guard_family(15, 2), "beq")  # beq- hint
        self.assertEqual(guard_family(4, 2), "bne")
        self.assertEqual(guard_family(5, 2), "bne")  # bne+ hint
        self.assertEqual(guard_family(7, 2), "bne")  # bne- hint
        self.assertEqual(guard_family(12, 0), "blt")
        self.assertEqual(guard_family(12, 1), "bgt")
        self.assertEqual(guard_family(4, 1), "ble")
        self.assertEqual(guard_family(4, 0), "bge")
        self.assertIsNone(guard_family(16, 0))  # bdnz is not a CR-test
        self.assertIsNone(guard_family(12, 5))  # CR1 field


class GuardDischargeTests(unittest.TestCase):
    def _discharge(self, guard: SkipGuardInfo, program: list[Instruction]) -> bool:
        expr, _notes = recognize_trip_expr(program, guard.mtctr_index, guard.trip_reg)
        self.assertIsNotNone(expr)
        ops = SymbolicOps()
        entry_gpr = {reg: ops.const(reg) for reg in range(32)}
        result = discharge_skip_guard(guard, expr, entry_gpr, ops)
        self.assertIsNotNone(result)
        assert result is not None
        return result.all_unsat()

    def test_beq_srwi_guard_discharges(self) -> None:
        program = _shape1_bulk_loop()
        mtctr_index, guard = find_mtctr_with_guard(program, 3)
        self.assertIsNotNone(guard)
        self.assertTrue(self._discharge(guard, program))

    def test_beq_andi_guard_discharges(self) -> None:
        program = _shape2_remainder_loop()
        mtctr_index, guard = find_mtctr_with_guard(program, 4)
        self.assertIsNotNone(guard)
        self.assertTrue(self._discharge(guard, program))

    def test_ble_andi_guard_discharges(self) -> None:
        # ble (branch if value <= 0 signed); andi. result is non-negative so
        # the guard skips exactly when value == 0.
        program = [
            _insn(Opcode.ANDI_DOT, (6, 6, 7), address=0, record=True),
            _insn(Opcode.BC, (4, 1, 36, 0), address=4),  # ble exit
            _insn(Opcode.MTSPR, (6, 9), address=8),
            _insn(Opcode.ORI, (0, 0, 0), address=12),
            _insn(Opcode.STW, (3, 4, 0), address=16),
            _insn(Opcode.ADDI, (4, 4, 4), address=20),
            _insn(Opcode.BC, (16, 0, 16, 0), address=24),
            _insn(Opcode.BCLR, (20, 0, 0), address=36),
        ]
        mtctr_index, guard = find_mtctr_with_guard(program, 4)
        self.assertIsNotNone(guard)
        self.assertEqual(guard.family, "ble")
        self.assertTrue(self._discharge(guard, program))

    def test_bgt_srwi_guard_fails_closed(self) -> None:
        # bgt (branch if value > 0): for a non-negative srwi result the guard
        # is taken even when the trip is large — not a zero-trip skip guard.
        program = [
            _insn(Opcode.RLWINM, (0, 6, 29, 3, 31), address=0, record=True),
            _insn(Opcode.BC, (12, 1, 32, 0), address=4),  # bgt exit
            _insn(Opcode.MTSPR, (0, 9), address=8),
            _insn(Opcode.STW, (3, 4, 0), address=12),
            _insn(Opcode.ADDI, (4, 4, 4), address=16),
            _insn(Opcode.BC, (16, 0, 12, 0), address=20),
            _insn(Opcode.BCLR, (20, 0, 0), address=32),
        ]
        mtctr_index, guard = find_mtctr_with_guard(program, 3)
        self.assertIsNotNone(guard)
        self.assertFalse(self._discharge(guard, program))

    def test_bne_andi_guard_fails_closed(self) -> None:
        # bne (branch if value != 0) skips exactly when the trip is *nonzero*,
        # the opposite of a zero-trip guard.
        program = [
            _insn(Opcode.ANDI_DOT, (6, 6, 7), address=0, record=True),
            _insn(Opcode.BC, (4, 2, 36, 0), address=4),  # bne exit
            _insn(Opcode.MTSPR, (6, 9), address=8),
            _insn(Opcode.ORI, (0, 0, 0), address=12),
            _insn(Opcode.STW, (3, 4, 0), address=16),
            _insn(Opcode.ADDI, (4, 4, 4), address=20),
            _insn(Opcode.BC, (16, 0, 16, 0), address=24),
            _insn(Opcode.BCLR, (20, 0, 0), address=36),
        ]
        mtctr_index, guard = find_mtctr_with_guard(program, 4)
        self.assertIsNotNone(guard)
        self.assertFalse(self._discharge(guard, program))


class GuardApplyTimeTests(unittest.TestCase):
    def test_guarded_symbolic_remainder_applies_summary(self) -> None:
        # Self-execution of the AXVPB-style remainder loop: with the skip
        # guard discharged at apply time, the bounded-remainder summary is
        # applied and execution terminates (no iteration-limit failure).
        program = _shape2_remainder_loop()
        plans = build_memory_loop_plan_map(program)
        self.assertEqual(len(plans), 1)
        plan = next(iter(plans.values()))
        self.assertEqual(plan.summary.zero_guard, "skip-branch")
        self.assertIsNotNone(plan.summary.skip_guard)
        terminals = execute_cfg(
            concrete_state({"gpr": {"r6": 0x2B, "r4": 0x1000, "r3": 0xAA}, "lr": 0x80001000}),
            program,
            ConcreteOps(),
            memory_loop_plans=plans,
        )
        live = [t for t in terminals if t.condition]
        self.assertEqual(len(live), 1)
        self.assertEqual(live[0].exit_kind, "return")
        # r6 & 7 == 3 iterations: r4 advances 3*4, ctr hits 0.
        self.assertEqual(live[0].state.gpr[4], 0x1000 + 12)
        self.assertEqual(live[0].state.ctr, 0)

    def test_zero_trip_guarded_loop_skips(self) -> None:
        program = _shape2_remainder_loop()
        plans = build_memory_loop_plan_map(program)
        terminals = execute_cfg(
            concrete_state({"gpr": {"r6": 0x8, "r4": 0x1000, "r3": 0xAA}, "lr": 0x80001000}),
            program,
            ConcreteOps(),
            memory_loop_plans=plans,
        )
        live = [t for t in terminals if t.condition]
        # r6 & 7 == 0: the guard skips the loop entirely.
        self.assertEqual(len(live), 1)
        self.assertEqual(live[0].state.gpr[4], 0x1000)
        self.assertEqual(live[0].state.ctr, 0)

    def test_unguarded_symbolic_remainder_fails_at_iteration_limit(self) -> None:
        # An unbounded symbolic trip with no guard and no concrete value has
        # no summary (partial) and fails closed at the iteration limit under
        # symbolic execution (a masked 3-bit trip would instead be proven
        # terminating by the solver, so use an entry-register trip).
        program = [
            _insn(Opcode.MTSPR, (6, 9), address=0),
            _insn(Opcode.STW, (3, 4, 0), address=4),
            _insn(Opcode.ADDI, (4, 4, 4), address=8),
            _insn(Opcode.BC, (16, 0, 4, 0), address=12),
        ]
        plans = build_memory_loop_plan_map(program)
        self.assertEqual(len(plans), 0)  # no guard, no concrete trip -> partial
        from tools.ppc_equivalence.engine import _symbolic_initial

        initial = _symbolic_initial(SymbolicOps())
        with self.assertRaises(ExecutionInconclusive):
            execute_cfg(
                initial,
                program,
                SymbolicOps(),
                max_loop_iterations=16,
            )

    def test_guard_bearing_ctr_affine_candidate_recorded(self) -> None:
        # The ctr-affine finder relaxes adjacency and records the guard; the
        # symbolic trip carries ``symbolic-trip`` confidence with a parametric
        # summary (doc 30 Phase A).
        program = [
            _insn(Opcode.RLWINM, (0, 6, 29, 3, 31), address=0, record=True),  # srwi. r0,r6,3
            _insn(Opcode.MTSPR, (0, 9), address=4),
            _insn(Opcode.BC, (12, 2, 32, 0), address=8),  # beq exit
            _insn(Opcode.ADDI, (3, 3, 1), address=12),
            _insn(Opcode.BC, (16, 0, 12, 0), address=16),
            _insn(Opcode.BCLR, (20, 0, 0), address=32),
        ]
        candidates = find_ctr_affine_loop_candidates(program)
        self.assertEqual(len(candidates), 1)
        candidate = candidates[0]
        self.assertEqual(candidate.confidence, "symbolic-trip")
        self.assertIsNone(candidate.trip_count)
        self.assertIsNotNone(candidate.skip_guard)
        self.assertIsNotNone(candidate.trip_expr)

    def test_adjacent_mtctr_relaxation_in_ctr_affine_finder(self) -> None:
        # mtctr at header_index - 2 with a nop between (no guard) is accepted.
        program = [
            _insn(Opcode.ADDI, (6, 0, 4), address=0),
            _insn(Opcode.MTSPR, (6, 9), address=4),
            _insn(Opcode.ORI, (0, 0, 0), address=8),  # nop
            _insn(Opcode.ADDI, (3, 3, 1), address=12),
            _insn(Opcode.BC, (16, 0, 12, 0), address=16),
        ]
        candidates = find_ctr_affine_loop_candidates(program)
        self.assertEqual(len(candidates), 1)
        self.assertEqual(candidates[0].trip_count, 4)
        self.assertIsNone(candidates[0].skip_guard)

    def test_engine_elevates_guarded_remainder_past_iteration_limit(self) -> None:
        # End-to-end: self-equivalence of the AXVPB-style guarded remainder
        # loop must apply the bounded-remainder summary (memory-loop-summary
        # feature present, no iteration-limit failure). EQUIVALENT stays gated
        # on the refinement discharge (``applied``), exactly as designed.
        from tools.ppc_equivalence.contract import EquivalenceContract, parse_observables
        from tools.ppc_equivalence.engine import check_equivalence
        from tools.ppc_equivalence.result import ProofStatus

        program = _shape2_remainder_loop()
        contract = EquivalenceContract(parse_observables(["r3", "r4", "r6"]), timeout_ms=20_000)
        result = check_equivalence(
            program, program, contract, original_hex="00", candidate_hex="00",
        )
        self.assertEqual(result.status, ProofStatus.INCONCLUSIVE_UNSUPPORTED)
        self.assertFalse(
            any("loop iteration limit" in item for item in result.unsupported),
        )
        self.assertIn("memory-loop-summary", result.proof_features)
        self.assertEqual(result.memory_loop.get("status"), "applied")


if __name__ == "__main__":
    unittest.main()
