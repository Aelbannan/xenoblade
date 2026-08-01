"""Symbolic-trip CTR-affine closed forms (doc 30 Phase A).

Covers A1 (trip-expr recognition incl. big-const ``lis+ori`` folding),
A2 (entry premise for affine applications), and A3 (parametric relational
discharge against symbolic ``N``).
"""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.contract import EquivalenceContract, parse_observables
from tools.ppc_equivalence.engine import check_equivalence
from tools.ppc_equivalence.ir import Instruction, Opcode
from tools.ppc_equivalence.loop_summary import (
    build_affine_summary_map,
    find_ctr_affine_loop_candidates,
    summarize_ctr_affine_loop,
)
from tools.ppc_equivalence.model import concrete_state
from tools.ppc_equivalence.result import ProofStatus
from tools.ppc_equivalence.semantics import ConcreteOps, execute_cfg


def _insn(
    opcode: Opcode,
    operands: tuple[int, ...],
    *,
    address: int = 0,
    record: bool = False,
) -> Instruction:
    return Instruction(address, 0, opcode, operands, record=record)


def _guarded_symbolic_bulk_loop(*, shift: int = 3, header: int = 12) -> list[Instruction]:
    """[srwi. r0,r6,k][mtctr r0][beq exit][header: addi r3,r3,1][bdnz][blr] (contiguous)."""
    return [
        _insn(Opcode.RLWINM, (0, 6, 32 - shift, shift, 31), address=0, record=True),
        _insn(Opcode.MTSPR, (0, 9), address=4),
        _insn(Opcode.BC, (12, 2, 20, 0), address=8),  # beq exit(20)
        _insn(Opcode.ADDI, (3, 3, 1), address=header),
        _insn(Opcode.BC, (16, 0, header, 0), address=header + 4),
        _insn(Opcode.BCLR, (20, 0, 0), address=20),
    ]


class SymbolicTripRecognitionTests(unittest.TestCase):
    def test_srwi_trip_is_symbolic_trip_confidence(self) -> None:
        program = _guarded_symbolic_bulk_loop()
        candidates = find_ctr_affine_loop_candidates(program)
        self.assertEqual(len(candidates), 1)
        candidate = candidates[0]
        self.assertEqual(candidate.confidence, "symbolic-trip")
        self.assertIsNone(candidate.trip_count)
        self.assertIsNotNone(candidate.trip_expr)
        self.assertIsNotNone(candidate.skip_guard)
        self.assertEqual(candidate.trip_expr["kind"], "lshr")

    def test_plain_entry_reg_trip_is_symbolic(self) -> None:
        # mr r6,r5; mtctr r6 — parameter trip with a copy materialization.
        program = [
            _insn(Opcode.OR, (6, 5, 5), address=0),  # mr r6, r5
            _insn(Opcode.MTSPR, (6, 9), address=4),
            _insn(Opcode.ADDI, (3, 3, 1), address=8),
            _insn(Opcode.BC, (16, 0, 8, 0), address=12),
        ]
        candidates = find_ctr_affine_loop_candidates(program)
        self.assertEqual(len(candidates), 1)
        self.assertEqual(candidates[0].confidence, "symbolic-trip")
        self.assertIsNotNone(candidates[0].trip_expr)
        self.assertEqual(candidates[0].trip_expr["kind"], "entry")

    def test_big_const_lis_ori_folds_to_exact_pattern(self) -> None:
        # F-7 / A1: constants > 32767 materialize as lis+ori and fold to
        # TripConstant — previously rejected by the single-addi-only counter.
        hi = (50000 >> 16) & 0xFFFF
        lo = 50000 & 0xFFFF
        program = [
            _insn(Opcode.ADDIS, (0, 0, hi), address=0),
            _insn(Opcode.ORI, (0, 0, lo), address=4),
            _insn(Opcode.MTSPR, (0, 9), address=8),
            _insn(Opcode.ADDI, (3, 3, 1), address=12),
            _insn(Opcode.BC, (16, 0, 12, 0), address=16),
        ]
        candidates = find_ctr_affine_loop_candidates(program)
        self.assertEqual(len(candidates), 1)
        candidate = candidates[0]
        self.assertEqual(candidate.confidence, "exact-pattern")
        self.assertEqual(candidate.trip_count, 50000)
        self.assertEqual(candidate.trip_expr["kind"], "const")

    def test_symbolic_summary_requires_guard(self) -> None:
        unguarded = [
            _insn(Opcode.RLWINM, (0, 6, 29, 3, 31), address=0, record=True),
            _insn(Opcode.MTSPR, (0, 9), address=4),
            _insn(Opcode.ADDI, (3, 3, 1), address=8),
            _insn(Opcode.BC, (16, 0, 8, 0), address=12),
        ]
        candidate = find_ctr_affine_loop_candidates(unguarded)[0]
        self.assertIsNone(candidate.skip_guard)
        self.assertIsNone(summarize_ctr_affine_loop(candidate))

        guarded = _guarded_symbolic_bulk_loop()
        candidate = find_ctr_affine_loop_candidates(guarded)[0]
        summary = summarize_ctr_affine_loop(candidate)
        self.assertIsNotNone(summary)
        assert summary is not None
        self.assertIsNone(summary.trip_count)
        self.assertIsNotNone(summary.trip_expr)
        self.assertEqual(summary.zero_guard, "skip-branch")
        self.assertEqual(summary.proof_kind, "affine-closed-form")

    def test_parametric_summary_matches_unrolling_concretely(self) -> None:
        # A1: summarized vs unrolled must agree bit-for-bit for N in 0..8.
        program = _guarded_symbolic_bulk_loop()
        summaries = build_affine_summary_map(program)
        self.assertEqual(len(summaries), 1)
        summary = next(iter(summaries.values()))
        for n in range(0, 9):
            entry = concrete_state(
                {"gpr": {"r6": n, "r3": 10}, "lr": 0x80001000},
            )
            summarized = execute_cfg(
                entry, program, ConcreteOps(), affine_loop_summaries=summaries,
            )
            unrolled = execute_cfg(
                concrete_state({"gpr": {"r6": n, "r3": 10}, "lr": 0x80001000}),
                program,
                ConcreteOps(),
            )
            sum_terms = [t for t in summarized if t.condition]
            unr_terms = [t for t in unrolled if t.condition]
            self.assertEqual(len(sum_terms), 1, f"N={n}")
            self.assertEqual(len(unr_terms), 1, f"N={n}")
            self.assertEqual(sum_terms[0].exit_kind, "return", f"N={n}")
            self.assertEqual(
                sum_terms[0].state.gpr[3], unr_terms[0].state.gpr[3], f"N={n}",
            )
            self.assertEqual(
                sum_terms[0].state.ctr, unr_terms[0].state.ctr, f"N={n}",
            )
            # Guarded skip on N >> 3 == 0: no iterations.
            expected = 10 + (n >> 3)
            self.assertEqual(sum_terms[0].state.gpr[3], expected, f"N={n}")


class ParametricDischargeTests(unittest.TestCase):
    def test_guarded_symbolic_loop_self_equivalent_parametric(self) -> None:
        # A3: parametric relational discharge (symbolic N) + parametric
        # summary authorize EQUIVALENT for a skip-guarded symbolic-trip loop.
        program = _guarded_symbolic_bulk_loop()
        contract = EquivalenceContract(parse_observables(["r3"]), timeout_ms=20_000)
        result = check_equivalence(
            program, program, contract,
            original_hex="00", candidate_hex="00",
            max_loop_iterations=2,
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT, result.unsupported)
        self.assertIn("affine-loop-summary", result.proof_features)
        self.assertIn("relational-induction", result.proof_features)
        self.assertIsNotNone(result.loop_summary)
        self.assertEqual(result.loop_summary["status"], "discharged")
        self.assertEqual(
            result.loop_summary["algorithm"], "affine-closed-form-v2",
        )
        self.assertEqual(result.loop_summary["trip_count"], None)
        self.assertIn("trip_expr", result.loop_summary)
        self.assertIn("zero_guard", result.loop_summary)
        self.assertEqual(result.relational_induction["status"], "discharged")

    def test_big_const_loop_proves_under_tight_iteration_bound(self) -> None:
        # A1/F-7: lis+ori materialized trip > 256 proves under a tight bound
        # (the 256-iteration unrolling fallback could never reach it).
        hi = (300 >> 16) & 0xFFFF
        lo = 300 & 0xFFFF
        program = [
            _insn(Opcode.ADDIS, (0, 0, hi), address=0),
            _insn(Opcode.ORI, (0, 0, lo), address=4),
            _insn(Opcode.MTSPR, (0, 9), address=8),
            _insn(Opcode.ADDI, (3, 3, 1), address=12),
            _insn(Opcode.BC, (16, 0, 12, 0), address=16),
            _insn(Opcode.BCLR, (20, 0, 0), address=20),
        ]
        contract = EquivalenceContract(parse_observables(["r3"]), timeout_ms=20_000)
        result = check_equivalence(
            program, program, contract,
            original_hex="00", candidate_hex="00",
            max_loop_iterations=2,
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT, result.unsupported)
        self.assertEqual(result.loop_summary["trip_count"], 300)
        self.assertEqual(result.loop_summary["algorithm"], "affine-closed-form-v1")

    def test_parametric_discharge_rejects_mismatched_trips(self) -> None:
        # A3 negative: symbolic vs concrete mismatched trips must fail closed.
        from tools.ppc_equivalence.relational_induction import (
            RelationalInductionUnsupported,
            _affine_bodies_match_for_discharge,
        )

        symbolic = find_ctr_affine_loop_candidates(
            _guarded_symbolic_bulk_loop(),
        )[0]
        concrete_program = [
            _insn(Opcode.ADDI, (0, 0, 4), address=0),
            _insn(Opcode.MTSPR, (0, 9), address=4),
            _insn(Opcode.ADDI, (3, 3, 1), address=8),
            _insn(Opcode.BC, (16, 0, 8, 0), address=12),
        ]
        concrete = find_ctr_affine_loop_candidates(concrete_program)[0]
        # Different bodies (srwi-trip vs li-trip) but the *trip exprs* differ;
        # the concrete side cannot fold to an equal TripConstant.
        self.assertFalse(_affine_bodies_match_for_discharge(symbolic, concrete))

    def test_parametric_termination_premise_n_nonzero(self) -> None:
        # A3: the termination discharge must hold for the guarded symbolic
        # shape (N != 0 premise) and the whole bundle must be UNSAT.
        from tools.ppc_equivalence.deadline import Deadline
        from tools.ppc_equivalence.relational_discharge import (
            try_smt_discharge_ctr_affine,
        )
        from tools.ppc_equivalence.relational_induction import RelationalInductionSketch

        program = _guarded_symbolic_bulk_loop()
        candidate = find_ctr_affine_loop_candidates(program)[0]
        result = try_smt_discharge_ctr_affine(
            candidate, candidate, deadline=Deadline.after_ms(20_000),
        )
        self.assertIsInstance(result, RelationalInductionSketch)
        assert isinstance(result, RelationalInductionSketch)
        self.assertEqual(result.status, "discharged")
        self.assertIn("parametric symbolic trip (v2)", result.notes)


if __name__ == "__main__":
    unittest.main()
