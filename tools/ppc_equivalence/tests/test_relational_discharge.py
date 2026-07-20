"""PR7 relational-induction SMT discharge tests."""

from __future__ import annotations

import copy
import importlib.util
import unittest

from tools.ppc_equivalence.deadline import Deadline
from tools.ppc_equivalence.ir import Instruction, Opcode
from tools.ppc_equivalence.loop_summary import find_ctr_affine_loop_candidates
from tools.ppc_equivalence.proof_features import (
    UNSUPPORTED_FOR_EQUIVALENT,
    enforce_equivalent_proof_features,
    validate_proof_features,
)
from tools.ppc_equivalence.proof_request import ProofRequest, cache_key
from tools.ppc_equivalence.relational_induction import (
    NARROW_INVARIANT_NAMES,
    RelationalInductionSketch,
    build_relational_induction_sketch,
    try_discharge_ctr_affine_relational,
    try_discharge_relational,
    validate_relational_induction_obligation,
)
from tools.ppc_equivalence.result import ProofResult, ProofStatus


_HAS_Z3 = importlib.util.find_spec("z3") is not None


def _insn(
    opcode: Opcode,
    operands: tuple[int, ...],
    *,
    address: int = 0,
) -> Instruction:
    return Instruction(address, 0, opcode, operands, link=False)


def _ctr_counted_loop(
    *,
    count: int,
    body_reg: int = 3,
    addend: int = 1,
    base_address: int = 0,
) -> list[Instruction]:
    return [
        _insn(Opcode.ADDI, (0, 0, count), address=base_address),
        _insn(Opcode.MTSPR, (0, 9), address=base_address + 4),
        _insn(Opcode.ADDI, (body_reg, body_reg, addend), address=base_address + 8),
        _insn(Opcode.BC, (16, 0, base_address + 8, 0), address=base_address + 12),
        _insn(Opcode.BCLR, (20, 0, 0), address=base_address + 16),
    ]


def _sample_request(**overrides: object) -> ProofRequest:
    base = dict(
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
        proof_features=(),
        obligations={},
    )
    base.update(overrides)
    return ProofRequest(**base)  # type: ignore[arg-type]


@unittest.skipUnless(_HAS_Z3, "z3-solver is not installed")
class RelationalSmtDischargeTests(unittest.TestCase):
    def test_positive_paired_ctr_loop_discharges_all_five(self) -> None:
        program = _ctr_counted_loop(count=3)
        sketch = try_discharge_ctr_affine_relational(program, program)
        self.assertIsNotNone(sketch)
        assert sketch is not None
        self.assertEqual(sketch.status, "discharged")
        obligation = sketch.to_obligation_dict()
        self.assertEqual(obligation["status"], "discharged")
        for block_name in (
            "initiation",
            "preservation",
            "exit_agreement",
            "postcondition",
            "termination",
        ):
            block = obligation[block_name]
            self.assertEqual(block["status"], "discharged", block_name)
            self.assertEqual(block["result"], "unsat", block_name)
            self.assertEqual(len(block["query_sha256"]), 64, block_name)
            self.assertEqual(block["solver"]["name"], "z3", block_name)
            self.assertTrue(block["algorithm"], block_name)
        self.assertTrue(set(obligation["templates"]) <= NARROW_INVARIANT_NAMES)
        self.assertIsNone(validate_relational_induction_obligation(obligation))
        self.assertIsNone(
            validate_proof_features(
                {
                    "proof_features": ["relational-induction"],
                    "relational_induction": obligation,
                },
                require_equivalent_ready=True,
            ),
        )

    def test_sat_or_unknown_block_is_not_discharged(self) -> None:
        """Negative: mutating a digests to SAT must fail equivalent-ready validation."""
        program = _ctr_counted_loop(count=2)
        sketch = try_discharge_ctr_affine_relational(program, program)
        self.assertIsNotNone(sketch)
        assert sketch is not None
        obligation = sketch.to_obligation_dict()
        bad = copy.deepcopy(obligation)
        bad["initiation"]["result"] = "sat"
        bad["initiation"]["status"] = "failed"
        # Top-level still claims discharged — validator must reject.
        self.assertIsNotNone(validate_relational_induction_obligation(bad))
        reason = validate_proof_features(
            {
                "proof_features": ["relational-induction"],
                "relational_induction": bad,
            },
            require_equivalent_ready=True,
        )
        self.assertIsNotNone(reason)

        pending = copy.deepcopy(obligation)
        pending["status"] = "pending"
        for block_name in (
            "initiation",
            "preservation",
            "exit_agreement",
            "postcondition",
            "termination",
        ):
            pending[block_name]["status"] = "pending"
            pending[block_name].pop("result", None)
        self.assertIsNone(validate_relational_induction_obligation(pending))
        ready = validate_proof_features(
            {
                "proof_features": ["relational-induction"],
                "relational_induction": pending,
            },
            require_equivalent_ready=True,
        )
        self.assertIsNotNone(ready)
        assert ready is not None
        self.assertIn("discharged", ready)

    def test_mismatched_bodies_do_not_discharge(self) -> None:
        left = _ctr_counted_loop(count=4, addend=1)
        right = _ctr_counted_loop(count=4, addend=2)
        self.assertIsNone(try_discharge_ctr_affine_relational(left, right))
        self.assertIsNone(try_discharge_relational(left, right))

    def test_obligation_block_mutation_rejected(self) -> None:
        program = _ctr_counted_loop(count=2)
        sketch = try_discharge_ctr_affine_relational(program, program)
        assert sketch is not None
        obligation = sketch.to_obligation_dict()
        mutated = copy.deepcopy(obligation)
        mutated["preservation"]["query_sha256"] = "0" * 64
        # Structural discharge fields still present, but digest no longer matches
        # solver evidence identity used by cache — validation of shape still
        # passes; equivalent gating uses the mutated digest in cache keys.
        self.assertIsNone(validate_relational_induction_obligation(mutated))
        # Drop required solver metadata → rejected.
        del mutated["preservation"]["solver"]
        self.assertIsNotNone(validate_relational_induction_obligation(mutated))

        missing_algo = copy.deepcopy(obligation)
        missing_algo["exit_agreement"]["algorithm"] = ""
        self.assertIsNotNone(validate_relational_induction_obligation(missing_algo))

    def test_cache_key_sensitive_to_relational_obligation(self) -> None:
        program = _ctr_counted_loop(count=2)
        sketch = try_discharge_ctr_affine_relational(program, program)
        assert sketch is not None
        obligation = sketch.to_obligation_dict()
        baseline = _sample_request(
            proof_features=("relational-induction",),
            obligations={"relational_induction": obligation},
        )
        mutated_obl = copy.deepcopy(obligation)
        mutated_obl["initiation"]["query_sha256"] = "a" * 64
        mutated = _sample_request(
            proof_features=("relational-induction",),
            obligations={"relational_induction": mutated_obl},
        )
        engine = "e" * 64
        certifier = "c" * 64
        self.assertNotEqual(
            cache_key(baseline, engine, certifier),
            cache_key(mutated, engine, certifier),
        )

    def test_discharged_sketch_can_authorize_equivalent(self) -> None:
        program = _ctr_counted_loop(count=2)
        sketch = try_discharge_ctr_affine_relational(program, program)
        assert sketch is not None
        result = ProofResult(
            status=ProofStatus.EQUIVALENT,
            proof_features=["relational-induction"],
            relational_induction=sketch.to_obligation_dict(),
        )
        gated = enforce_equivalent_proof_features(result)
        self.assertEqual(gated.status, ProofStatus.EQUIVALENT)

    def test_relational_feature_unfrozen(self) -> None:
        self.assertNotIn("relational-induction", UNSUPPORTED_FOR_EQUIVALENT)
        self.assertIn("affine-loop-summary", UNSUPPORTED_FOR_EQUIVALENT)

    def test_failed_smt_bundle_does_not_claim_discharged(self) -> None:
        """Force a SAT initiation query and ensure status stays failed."""
        import z3

        from tools.ppc_equivalence.discharge import discharge_bad_conditions
        from tools.ppc_equivalence.relational_discharge import discharge_block_payload

        x = z3.BitVec("x", 32)
        sat_discharge = discharge_bad_conditions(
            premises=[x == 1],
            bad_conditions=[x == 1],
            deadline=Deadline.after_ms(5_000),
            algorithm="test-sat-initiation",
            z3_module=z3,
        )
        self.assertEqual(sat_discharge.status, "sat")
        payload = discharge_block_payload(
            sat_discharge,
            invariants=[{"name": "equal-ctr", "params": {}}],
            z3_module=z3,
        )
        self.assertEqual(payload["status"], "failed")
        self.assertEqual(payload["result"], "sat")

    def test_sketch_builder_still_pending_without_smt(self) -> None:
        original = find_ctr_affine_loop_candidates(
            _ctr_counted_loop(count=2, body_reg=3, addend=1),
        )[0]
        candidate = find_ctr_affine_loop_candidates(
            _ctr_counted_loop(count=2, body_reg=3, addend=1, base_address=0x40),
        )[0]
        sketch = build_relational_induction_sketch(original, candidate)
        assert isinstance(sketch, RelationalInductionSketch)
        self.assertEqual(sketch.status, "pending")


if __name__ == "__main__":
    unittest.main()
