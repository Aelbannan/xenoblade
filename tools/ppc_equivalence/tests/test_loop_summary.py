"""Affine / CTR closed-form loop summary tests (recognition only)."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.ir import Instruction, Opcode
from tools.ppc_equivalence.loop_summary import (
    closed_form_gpr_value,
    find_compare_affine_loop_candidates,
    find_ctr_affine_loop_candidates,
    summarize_compare_affine_loop,
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


def _compare_counted_loop(*, count: int, addend: int = 1) -> list[Instruction]:
    """li r4,count; loop: addi r3,r3,addend; addi r4,r4,-1; cmpwi r4,0; bne loop; blr"""
    return [
        _insn(Opcode.ADDI, (4, 0, count), address=0),
        _insn(Opcode.ADDI, (3, 3, addend), address=4),
        _insn(Opcode.ADDI, (4, 4, -1), address=8),
        _insn(Opcode.CMPWI, (0, 4, 0), address=12),
        _insn(Opcode.BC, (4, 2, 4, 0), address=16),
        _insn(Opcode.BCLR, (20, 0, 0), address=20),
    ]


class CompareAffineRecognitionTests(unittest.TestCase):
    def test_recognizes_compare_affine_loop(self) -> None:
        program = _compare_counted_loop(count=5, addend=2)
        candidates = find_compare_affine_loop_candidates(program)
        self.assertEqual(len(candidates), 1)
        candidate = candidates[0]
        self.assertEqual(candidate.confidence, "exact-pattern")
        self.assertEqual(candidate.trip_count, 5)
        self.assertEqual(candidate.trip_count_reg, 4)
        self.assertEqual(candidate.body_updates[0].reg, 3)
        self.assertEqual(candidate.body_updates[0].addend, 2)

    def test_summary_matches_concrete_execution(self) -> None:
        program = _compare_counted_loop(count=4, addend=3)
        candidate = find_compare_affine_loop_candidates(program)[0]
        summary = summarize_compare_affine_loop(candidate)
        self.assertIsNotNone(summary)
        assert summary is not None
        self.assertEqual(summary.proof_kind, "compare-affine-closed-form")
        entry = 10
        predicted = closed_form_gpr_value(entry, 3, 4)
        terminals = [
            t
            for t in execute_cfg(
                concrete_state({"gpr": {"r3": entry, "r4": 0}, "lr": 0x80001000, "ctr": 7}),
                program,
                ConcreteOps(),
                affine_loop_summaries={summary.header_pc: summary},
                max_loop_iterations=2,
            )
            if t.condition
        ]
        self.assertEqual(len(terminals), 1)
        self.assertEqual(terminals[0].state.gpr[3], predicted)
        self.assertEqual(terminals[0].state.gpr[4], 0)
        self.assertEqual(terminals[0].state.ctr, 7)
        # Final cmpwi r4,0 after countdown leaves CR0.EQ set (and SO from XER).
        self.assertEqual((terminals[0].state.cr >> 28) & 0xF, 0x2)


class AffineFeatureGateTests(unittest.TestCase):
    def test_affine_feature_is_unfrozen_with_discharge_gate(self) -> None:
        from tools.ppc_equivalence.proof_features import (
            KNOWN_PROOF_FEATURES,
            UNSUPPORTED_FOR_EQUIVALENT,
        )

        self.assertIn("affine-loop-summary", KNOWN_PROOF_FEATURES)
        self.assertNotIn("affine-loop-summary", UNSUPPORTED_FOR_EQUIVALENT)
        self.assertIn("memory-bus", UNSUPPORTED_FOR_EQUIVALENT)

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
        self.assertEqual(result.loop_summary["status"], "discharged")
        self.assertEqual(result.relational_induction["status"], "discharged")
        self.assertEqual(len(result.loop_summary["summary_sha256"]), 64)

    def test_compare_affine_proves_under_tight_iteration_bound(self) -> None:
        from tools.ppc_equivalence.contract import EquivalenceContract, parse_observables
        from tools.ppc_equivalence.engine import check_equivalence
        from tools.ppc_equivalence.result import ProofStatus

        program = _compare_counted_loop(count=20, addend=3)
        contract = EquivalenceContract(parse_observables(["r3", "r4"]), timeout_ms=15_000)
        result = check_equivalence(
            program, program, contract,
            original_hex="00", candidate_hex="00",
            max_loop_iterations=2,
        )
        # Compare-affine now gets relational SMT discharge (counter-descending).
        # Affine-loop-summary remains in the result; with Track B unfreeze it may
        # authorize only when both obligations are discharged — under a tight
        # iteration bound the CFG still needed the closed form, so status is
        # EQUIVALENT when features validate, else INCONCLUSIVE from other gates.
        self.assertIn("affine-loop-summary", result.proof_features)
        self.assertIn("relational-induction", result.proof_features)
        self.assertEqual(result.loop_summary["proof_kind"], "compare-affine-closed-form")
        self.assertEqual(result.loop_summary["status"], "discharged")
        self.assertEqual(result.relational_induction["status"], "discharged")
        self.assertEqual(result.relational_induction["termination"]["witness"], "counter-descending")
        # Frozen memory-bus is irrelevant; with affine+relational discharged the
        # self-equivalence under a tight bound should be EQUIVALENT (Track B).
        self.assertEqual(result.status, ProofStatus.EQUIVALENT, result.unsupported)

    def test_applied_only_affine_cannot_authorize_equivalent(self) -> None:
        """Negative: coverage=applied without discharged status must demote."""
        from tools.ppc_equivalence.loop_summary import (
            build_loop_summary_obligation,
            find_ctr_affine_loop_candidates,
            summarize_ctr_affine_loop,
        )
        from tools.ppc_equivalence.proof_features import (
            enforce_equivalent_proof_features,
            validate_proof_features,
        )
        from tools.ppc_equivalence.result import ProofResult, ProofStatus

        summary = summarize_ctr_affine_loop(
            find_ctr_affine_loop_candidates(_ctr_counted_loop(count=2))[0],
        )
        assert summary is not None
        obligation = build_loop_summary_obligation(
            summary, coverage="applied", status="applied",
        )
        reason = validate_proof_features(
            {
                "proof_features": ["affine-loop-summary"],
                "loop_summary": obligation,
            },
            require_equivalent_ready=True,
        )
        self.assertIsNotNone(reason)
        assert reason is not None
        self.assertIn("discharged", reason)

        result = ProofResult(
            status=ProofStatus.EQUIVALENT,
            proof_features=["affine-loop-summary"],
            loop_summary=obligation,
        )
        gated = enforce_equivalent_proof_features(result)
        self.assertEqual(gated.status, ProofStatus.INCONCLUSIVE_UNSUPPORTED)

    def test_summary_digest_mutation_rejected(self) -> None:
        import copy

        from tools.ppc_equivalence.loop_summary import (
            build_loop_summary_obligation,
            find_ctr_affine_loop_candidates,
            summarize_ctr_affine_loop,
            validate_loop_summary_obligation,
        )
        from tools.ppc_equivalence.proof_request import ProofRequest, cache_key

        summary = summarize_ctr_affine_loop(
            find_ctr_affine_loop_candidates(_ctr_counted_loop(count=2))[0],
        )
        assert summary is not None
        obligation = build_loop_summary_obligation(
            summary,
            coverage="applied",
            status="discharged",
            relational_companion="discharged",
        )
        self.assertIsNone(validate_loop_summary_obligation(obligation))
        mutated = copy.deepcopy(obligation)
        mutated["summary_sha256"] = "a" * 64
        self.assertIsNotNone(validate_loop_summary_obligation(mutated))

        def _request(obl: dict) -> ProofRequest:
            return ProofRequest(
                original_hex="48000000",
                candidate_hex="48000000",
                original_base=0x80000000,
                candidate_base=0x80000000,
                contract="default",
                observables=("r3",),
                limits={"timeout_ms": 1000},
                memory_environment={},
                floating_point_domain=None,
                assumed_callees=(),
                callee_contracts={},
                relocations={},
                proof_features=("affine-loop-summary",),
                obligations={"loop_summary": obl},
            )

        self.assertNotEqual(
            cache_key(_request(obligation), "e" * 64, "c" * 64),
            cache_key(_request(mutated), "e" * 64, "c" * 64),
        )

    def test_compare_affine_vs_straight_line_cr_never_equivalent(self) -> None:
        """False-eq regression: loop exits with CR0.EQ=1; candidate leaves entry CR.

        After FinalCompare repair, observing cr0 must yield NOT_EQUIVALENT.
        """
        from tools.ppc_equivalence.contract import EquivalenceContract, parse_observables
        from tools.ppc_equivalence.engine import check_equivalence
        from tools.ppc_equivalence.result import ProofStatus

        loop = _compare_counted_loop(count=1, addend=0)
        straight = [
            _insn(Opcode.ADDI, (4, 0, 0), address=0),
            _insn(Opcode.BCLR, (20, 0, 0), address=4),
        ]
        contract = EquivalenceContract(
            parse_observables(["r3", "r4", "cr0"]),
            timeout_ms=15_000,
        )
        result = check_equivalence(
            loop,
            straight,
            contract,
            original_hex="00",
            candidate_hex="00",
            max_loop_iterations=2,
        )
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT, result.unsupported)


if __name__ == "__main__":
    unittest.main()
