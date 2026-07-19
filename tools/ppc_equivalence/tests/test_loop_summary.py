"""Affine / CTR closed-form loop summary tests (recognition only)."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.ir import Instruction, Opcode
from tools.ppc_equivalence.loop_summary import (
    closed_form_gpr_value,
    find_ctr_affine_loop_candidates,
    summarize_ctr_affine_loop,
)
from tools.ppc_equivalence.model import concrete_state
from tools.ppc_equivalence.semantics import ConcreteOps, execute_cfg


def _insn(
    opcode: Opcode,
    operands: tuple[int, ...],
    *,
    address: int = 0,
    link: bool = False,
) -> Instruction:
    return Instruction(address, 0, opcode, operands, link=link)


def _ctr_counted_loop(*, count: int, addend: int = 1) -> list[Instruction]:
    """li r0,count; mtctr r0; loop: addi r3,r3,addend; bdnz loop; blr"""
    return [
        _insn(Opcode.ADDI, (0, 0, count), address=0),
        _insn(Opcode.MTSPR, (0, 9), address=4),
        _insn(Opcode.ADDI, (3, 3, addend), address=8),
        _insn(Opcode.BC, (16, 0, 8, 0), address=12),
        _insn(Opcode.BCLR, (20, 0, 0), address=16),
    ]


class CtrAffineRecognitionTests(unittest.TestCase):
    def test_recognizes_exact_ctr_affine_loop(self) -> None:
        program = _ctr_counted_loop(count=4, addend=3)
        candidates = find_ctr_affine_loop_candidates(program)
        self.assertEqual(len(candidates), 1)
        candidate = candidates[0]
        self.assertEqual(candidate.confidence, "exact-pattern")
        self.assertEqual(candidate.trip_count, 4)
        self.assertEqual(candidate.header_pc, 8)
        self.assertEqual(candidate.latch_pc, 12)
        self.assertEqual(candidate.body_updates[0].reg, 3)
        self.assertEqual(candidate.body_updates[0].addend, 3)

    def test_summary_matches_concrete_execution(self) -> None:
        for count, addend, entry in ((1, 1, 0), (3, 2, 10), (5, -1, 100)):
            with self.subTest(count=count, addend=addend, entry=entry):
                program = _ctr_counted_loop(count=count, addend=addend)
                candidate = find_ctr_affine_loop_candidates(program)[0]
                summary = summarize_ctr_affine_loop(candidate)
                self.assertIsNotNone(summary)
                assert summary is not None
                self.assertEqual(summary.proof_kind, "affine-closed-form")
                self.assertEqual(summary.final_ctr, 0)
                self.assertEqual(summary.trip_count, count)
                stride = summary.final_gpr[3][1]
                predicted = closed_form_gpr_value(entry, stride, count)

                terminals = [
                    t
                    for t in execute_cfg(
                        concrete_state({"gpr": {"r3": entry}, "lr": 0x80001000}),
                        program,
                        ConcreteOps(),
                    )
                    if t.condition
                ]
                self.assertEqual(len(terminals), 1)
                self.assertEqual(terminals[0].state.gpr[3], predicted)
                self.assertEqual(terminals[0].state.ctr, 0)

    def test_ctr_zero_is_partial_not_summarized(self) -> None:
        program = _ctr_counted_loop(count=0, addend=1)
        candidates = find_ctr_affine_loop_candidates(program)
        self.assertEqual(len(candidates), 1)
        self.assertEqual(candidates[0].confidence, "partial")
        self.assertIsNone(summarize_ctr_affine_loop(candidates[0]))
        self.assertTrue(
            any("wrap" in note for note in candidates[0].notes),
            candidates[0].notes,
        )

    def test_rejects_non_affine_body(self) -> None:
        program = [
            _insn(Opcode.ADDI, (0, 0, 3), address=0),
            _insn(Opcode.MTSPR, (0, 9), address=4),
            _insn(Opcode.ADD, (3, 3, 4), address=8),  # not addi rt,rt,imm
            _insn(Opcode.BC, (16, 0, 8, 0), address=12),
        ]
        self.assertEqual(find_ctr_affine_loop_candidates(program), [])

    def test_rejects_forward_bdnz(self) -> None:
        program = [
            _insn(Opcode.ADDI, (0, 0, 2), address=0),
            _insn(Opcode.MTSPR, (0, 9), address=4),
            _insn(Opcode.ADDI, (3, 3, 1), address=8),
            _insn(Opcode.BC, (16, 0, 0x20, 0), address=12),  # forward
        ]
        self.assertEqual(find_ctr_affine_loop_candidates(program), [])


class AffineFeatureGateTests(unittest.TestCase):
    def test_affine_feature_is_supported(self) -> None:
        from tools.ppc_equivalence.proof_features import (
            KNOWN_PROOF_FEATURES,
            UNSUPPORTED_FOR_EQUIVALENT,
        )

        self.assertIn("affine-loop-summary", KNOWN_PROOF_FEATURES)
        self.assertNotIn("affine-loop-summary", UNSUPPORTED_FOR_EQUIVALENT)

    def test_summary_proves_under_tight_iteration_bound(self) -> None:
        from tools.ppc_equivalence.contract import EquivalenceContract, parse_observables
        from tools.ppc_equivalence.engine import check_equivalence
        from tools.ppc_equivalence.result import ProofStatus

        program = _ctr_counted_loop(count=20, addend=3)
        contract = EquivalenceContract(parse_observables(["r3"]), timeout_ms=15_000)
        result = check_equivalence(
            program, program, contract,
            original_hex="00", candidate_hex="00",
            max_loop_iterations=2,
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT, result.unsupported)
        self.assertIn("affine-loop-summary", result.proof_features)
        self.assertIn("relational-induction", result.proof_features)
        self.assertIsNotNone(result.loop_summary)
        self.assertIsNotNone(result.relational_induction)
        self.assertEqual(result.loop_summary["trip_count"], 20)
        self.assertEqual(result.relational_induction["status"], "applied")


if __name__ == "__main__":
    unittest.main()
