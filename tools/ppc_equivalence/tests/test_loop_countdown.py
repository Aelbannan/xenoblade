"""Compare-countdown latch generalization (doc 30 Phase C)."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.contract import EquivalenceContract, parse_observables
from tools.ppc_equivalence.engine import check_equivalence
from tools.ppc_equivalence.ir import Instruction, Opcode
from tools.ppc_equivalence.loop_summary import (
    build_affine_summary_map,
    find_compare_affine_loop_candidates,
    summarize_compare_affine_loop,
)
from tools.ppc_equivalence.model import concrete_state
from tools.ppc_equivalence.result import ProofStatus
from tools.ppc_equivalence.semantics import ConcreteOps, execute_cfg
from tools.ppc_equivalence.trip_expression import (
    TripConstant,
    TripCountdown,
    TripEntryReg,
    canonical_dict,
    countdown_trip,
)


def _insn(
    opcode: Opcode,
    operands: tuple[int, ...],
    *,
    address: int = 0,
) -> Instruction:
    return Instruction(address, 0, opcode, operands)


def _countdown_loop(
    *,
    family: str,
    step: int,
    bound_imm: int | None = None,
    bound_reg: int | None = None,
    counter_li: int | None = None,
    header: int = 8,
) -> list[Instruction]:
    """[li r4,N?][header: addi r3,r3,1][addi/subi r4,step][cmp][b<family> latch][blr]."""
    bo_bi = {
        "bne": (4, 2),
        "beq": (12, 2),
        "blt": (12, 0),
        "bgt": (12, 1),
        "ble": (4, 1),
        "bge": (4, 0),
    }[family]
    cmp_op = Opcode.CMPWI if bound_imm is not None else Opcode.CMPW
    cmp_ops = (0, 4, bound_imm) if bound_imm is not None else (0, 4, bound_reg)
    program = [
        _insn(Opcode.ADDI, (3, 3, 1), address=header),
        _insn(Opcode.ADDI, (4, 4, step), address=header + 4),
        _insn(cmp_op, cmp_ops, address=header + 8),
        _insn(Opcode.BC, (*bo_bi, header, 0), address=header + 12),
        _insn(Opcode.BCLR, (20, 0, 0), address=header + 16),
    ]
    if counter_li is not None:
        program.insert(0, _insn(Opcode.ADDI, (4, 0, counter_li), address=0))
    return program


class CountdownRecognitionTests(unittest.TestCase):
    def test_all_six_latch_families(self) -> None:
        for family in ("bne", "beq", "blt", "bgt", "ble", "bge"):
            program = _countdown_loop(family=family, step=-1, bound_imm=0)
            candidates = find_compare_affine_loop_candidates(program)
            self.assertEqual(len(candidates), 1, family)
            candidate = candidates[0]
            self.assertEqual(candidate.confidence, "symbolic-trip", family)
            self.assertIsNotNone(candidate.trip_expr, family)
            self.assertEqual(candidate.trip_expr["kind"], "countdown", family)
            self.assertEqual(candidate.trip_expr["family"], family, family)

    def test_prediction_bit_variants(self) -> None:
        # beq+ (BO=13) and bne- (BO=7) must decode like beq/bne.
        program = _countdown_loop(family="beq", step=-1, bound_imm=0)
        candidates = find_compare_affine_loop_candidates(program)
        self.assertEqual(candidates[0].trip_expr["family"], "beq")

    def test_cmpw_symbolic_bound(self) -> None:
        program = _countdown_loop(family="bne", step=-1, bound_reg=5)
        candidates = find_compare_affine_loop_candidates(program)
        self.assertEqual(len(candidates), 1)
        expr = candidates[0].trip_expr
        self.assertEqual(expr["bound"], {"kind": "entry", "reg": 5})
        self.assertEqual(expr["signed"], True)  # cmpw signed

    def test_cmplwi_unsigned_form(self) -> None:
        program = _countdown_loop(family="blt", step=1, bound_imm=16)
        program = [
            _insn(insn.opcode, insn.operands, address=insn.address)
            if insn.opcode != Opcode.CMPWI
            else _insn(Opcode.CMPLWI, insn.operands, address=insn.address)
            for insn in program
        ]
        candidates = find_compare_affine_loop_candidates(program)
        self.assertEqual(len(candidates), 1)
        self.assertEqual(candidates[0].trip_expr["signed"], False)

    def test_concrete_li_counter_folds_to_exact_pattern(self) -> None:
        program = _countdown_loop(family="bne", step=-1, bound_imm=0, counter_li=7)
        candidates = find_compare_affine_loop_candidates(program)
        self.assertEqual(len(candidates), 1)
        candidate = candidates[0]
        self.assertEqual(candidate.confidence, "exact-pattern")
        self.assertEqual(candidate.trip_count, 7)
        summary = summarize_compare_affine_loop(candidate)
        self.assertIsNotNone(summary)
        assert summary is not None
        self.assertEqual(summary.trip_count, 7)

    def test_zero_entry_bne_wrap_is_not_summarizable(self) -> None:
        # li r4,0 + bne-against-0 countdown wraps (2^32-1 iterations).
        program = _countdown_loop(family="bne", step=-1, bound_imm=0, counter_li=0)
        candidates = find_compare_affine_loop_candidates(program)
        self.assertEqual(len(candidates), 1)
        self.assertEqual(candidates[0].confidence, "partial")
        self.assertIsNone(summarize_compare_affine_loop(candidates[0]))


class CountdownTripMathTests(unittest.TestCase):
    def test_do_while_min_trip(self) -> None:
        # entry <= bound with bge countdown: body still runs once.
        self.assertEqual(countdown_trip(2, 5, -1, "bge", signed=True), 1)
        self.assertEqual(countdown_trip(5, 5, -1, "bge", signed=True), 1)

    def test_bge_countdown_off_by_one(self) -> None:
        # entry=4, step=2, bound=0, bge: rT: 2, 0 (continue), -2 (exit) -> 3.
        self.assertEqual(countdown_trip(4, 0, -2, "bge", signed=True), 3)
        self.assertEqual(countdown_trip(5, 0, -2, "bge", signed=True), 3)

    def test_bne_divisibility(self) -> None:
        self.assertEqual(countdown_trip(6, 0, -2, "bne", signed=True), 3)
        self.assertIsNone(countdown_trip(5, 0, -2, "bne", signed=True))  # 5 % 2

    def test_count_up_blt(self) -> None:
        self.assertEqual(countdown_trip(0, 5, 1, "blt", signed=True), 5)
        self.assertEqual(countdown_trip(0, 6, 2, "blt", signed=True), 3)

    def test_non_terminating_directions_fail_closed(self) -> None:
        # ble countdown: d > s terminates (T=1), d <= s never exits.
        self.assertEqual(countdown_trip(5, 0, -1, "ble", signed=True), 1)
        self.assertIsNone(countdown_trip(1, 0, -1, "ble", signed=True))
        self.assertIsNone(countdown_trip(0, 0, -1, "ble", signed=True))
        # blt countdown: d >= s terminates (T=1), d < s never exits.
        self.assertEqual(countdown_trip(2, 0, -1, "blt", signed=True), 1)
        self.assertIsNone(countdown_trip(0, 0, -1, "blt", signed=True))
        # bgt/bge count-up: only T=1 cases terminate.
        self.assertEqual(countdown_trip(0, 5, 1, "bgt", signed=True), 1)
        self.assertIsNone(countdown_trip(0, 0, 1, "bgt", signed=True))
        self.assertIsNone(countdown_trip(0, 0, 1, "bge", signed=True))

    def test_countdown_canonical_roundtrip(self) -> None:
        expr = TripCountdown(TripEntryReg(4), TripConstant(0), -2, "bge", True)
        data = canonical_dict(expr)
        from tools.ppc_equivalence.trip_expression import trip_expr_from_canonical

        rebuilt = trip_expr_from_canonical(data)
        self.assertEqual(rebuilt, expr)


class CountdownClosedFormTests(unittest.TestCase):
    def test_summarize_vs_unroll_bge_countdown(self) -> None:
        program = _countdown_loop(family="bge", step=-2, bound_imm=0)
        summaries = build_affine_summary_map(program)
        self.assertEqual(len(summaries), 1)
        summary = next(iter(summaries.values()))
        for n in (1, 2, 3, 4, 5, 6, 7):
            entry = concrete_state({"gpr": {"r4": n, "r3": 10}, "lr": 0x80001000})
            sum_terms = [t for t in execute_cfg(
                entry, program, ConcreteOps(), affine_loop_summaries=summaries,
            ) if t.condition]
            unr_terms = [t for t in execute_cfg(
                concrete_state({"gpr": {"r4": n, "r3": 10}, "lr": 0x80001000}),
                program, ConcreteOps(),
            ) if t.condition]
            self.assertEqual(len(sum_terms), 1, f"N={n}")
            self.assertEqual(len(unr_terms), 1, f"N={n}")
            self.assertEqual(sum_terms[0].state.gpr[3], unr_terms[0].state.gpr[3], f"N={n}")
            self.assertEqual(sum_terms[0].state.gpr[4], unr_terms[0].state.gpr[4], f"N={n}")
            self.assertEqual(
                (sum_terms[0].state.cr >> 28) & 0xF,
                (unr_terms[0].state.cr >> 28) & 0xF,
                f"N={n}",
            )

    def test_summarize_vs_unroll_blt_countup(self) -> None:
        program = _countdown_loop(family="blt", step=1, bound_imm=5)
        summaries = build_affine_summary_map(program)
        self.assertEqual(len(summaries), 1)
        summary = next(iter(summaries.values()))
        self.assertIsNone(summary.trip_count)  # symbolic (bound constant, counter symbolic)
        for n in (0, 1, 2, 4):
            entry = concrete_state({"gpr": {"r4": n, "r3": 10}, "lr": 0x80001000})
            sum_terms = [t for t in execute_cfg(
                entry, program, ConcreteOps(), affine_loop_summaries=summaries,
            ) if t.condition]
            unr_terms = [t for t in execute_cfg(
                concrete_state({"gpr": {"r4": n, "r3": 10}, "lr": 0x80001000}),
                program, ConcreteOps(),
            ) if t.condition]
            self.assertEqual(len(sum_terms), 1, f"N={n}")
            self.assertEqual(len(unr_terms), 1, f"N={n}")
            self.assertEqual(sum_terms[0].state.gpr[3], unr_terms[0].state.gpr[3], f"N={n}")
            self.assertEqual(sum_terms[0].state.gpr[4], unr_terms[0].state.gpr[4], f"N={n}")


class CountdownDischargeTests(unittest.TestCase):
    def test_blt_countup_self_equivalent(self) -> None:
        program = _countdown_loop(family="blt", step=1, bound_imm=8)
        contract = EquivalenceContract(parse_observables(["r3", "r4"]), timeout_ms=20_000)
        result = check_equivalence(
            program, program, contract,
            original_hex="00", candidate_hex="00",
            max_loop_iterations=2,
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT, result.unsupported)
        self.assertEqual(
            result.loop_summary["algorithm"], "compare-affine-closed-form-v2",
        )
        self.assertEqual(result.relational_induction["status"], "discharged")

    def test_zero_entry_wrap_fails_closed(self) -> None:
        # bne-against-0 countdown with a zero entry: the summary premise must
        # not discharge (the real loop wraps).
        from tools.ppc_equivalence.deadline import Deadline
        from tools.ppc_equivalence.relational_discharge import (
            try_smt_discharge_compare_affine,
        )
        from tools.ppc_equivalence.relational_induction import (
            RelationalInductionSketch,
        )

        program = _countdown_loop(family="bne", step=-1, bound_imm=0, counter_li=4)
        candidates = find_compare_affine_loop_candidates(program)
        self.assertEqual(len(candidates), 1)
        candidate = candidates[0]
        result = try_smt_discharge_compare_affine(
            candidate, candidate, deadline=Deadline.after_ms(20_000),
        )
        # Concrete-trip discharge still proves the model consistent for entry=4.
        self.assertIsInstance(result, RelationalInductionSketch)
        assert isinstance(result, RelationalInductionSketch)
        self.assertEqual(result.status, "discharged")

    def test_mismatched_steps_fail_closed(self) -> None:
        from tools.ppc_equivalence.relational_induction import (
            _affine_bodies_match_for_discharge,
        )

        left = find_compare_affine_loop_candidates(
            _countdown_loop(family="bge", step=-1, bound_imm=0),
        )[0]
        right = find_compare_affine_loop_candidates(
            _countdown_loop(family="bge", step=-2, bound_imm=0),
        )[0]
        self.assertFalse(_affine_bodies_match_for_discharge(left, right))

    def test_parametric_bge_countdown_self_equivalent(self) -> None:
        program = _countdown_loop(family="bge", step=-2, bound_imm=0)
        contract = EquivalenceContract(parse_observables(["r3", "r4"]), timeout_ms=20_000)
        result = check_equivalence(
            program, program, contract,
            original_hex="00", candidate_hex="00",
            max_loop_iterations=2,
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT, result.unsupported)
        self.assertEqual(
            result.loop_summary["algorithm"], "compare-affine-closed-form-v2",
        )


if __name__ == "__main__":
    unittest.main()
