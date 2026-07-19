"""Relational loop induction scaffold tests (descriptors only)."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.ir import Instruction, Opcode
from tools.ppc_equivalence.loop_cfg import NaturalLoop, analyze_loop_cfg
from tools.ppc_equivalence.loop_summary import find_ctr_affine_loop_candidates
from tools.ppc_equivalence.proof_features import (
    KNOWN_PROOF_FEATURES,
    UNSUPPORTED_FOR_EQUIVALENT,
    validate_proof_features,
)
from tools.ppc_equivalence.relational_induction import (
    HOUDINI_TEMPLATE_NAMES,
    HoudiniTemplateName,
    RelationalInductionSketch,
    RelationalInductionUnsupported,
    RelationalLoopSide,
    build_relational_induction_sketch,
)


def _insn(
    opcode: Opcode,
    operands: tuple[int, ...],
    *,
    address: int = 0,
    link: bool = False,
) -> Instruction:
    return Instruction(address, 0, opcode, operands, link=link)


def _ctr_counted_loop(
    *,
    count: int,
    body_reg: int = 3,
    addend: int = 1,
    base_address: int = 0,
) -> list[Instruction]:
    """li r0,count; mtctr r0; loop: addi rD,rD,addend; bdnz loop; blr"""
    return [
        _insn(Opcode.ADDI, (0, 0, count), address=base_address),
        _insn(Opcode.MTSPR, (0, 9), address=base_address + 4),
        _insn(Opcode.ADDI, (body_reg, body_reg, addend), address=base_address + 8),
        _insn(Opcode.BC, (16, 0, base_address + 8, 0), address=base_address + 12),
        _insn(Opcode.BCLR, (20, 0, 0), address=base_address + 16),
    ]


class RelationalInductionSketchTests(unittest.TestCase):
    def test_houdini_template_registry(self) -> None:
        self.assertEqual(
            HOUDINI_TEMPLATE_NAMES,
            frozenset(
                {
                    "register-equality",
                    "entry-plus-k-stride",
                    "interval",
                    "ctr-remaining",
                    "unmodified-component",
                    "constant-pointer-diff",
                },
            ),
        )
        self.assertEqual(
            HoudiniTemplateName.REGISTER_EQUALITY.value,
            "register-equality",
        )

    def test_sketch_differently_shaped_ctr_loops(self) -> None:
        original = find_ctr_affine_loop_candidates(
            _ctr_counted_loop(count=4, body_reg=3, addend=2),
        )[0]
        candidate = find_ctr_affine_loop_candidates(
            _ctr_counted_loop(count=4, body_reg=4, addend=3, base_address=0x100),
        )[0]
        result = build_relational_induction_sketch(original, candidate)
        self.assertIsInstance(result, RelationalInductionSketch)
        sketch = result
        self.assertEqual(sketch.status, "pending")
        self.assertEqual(sketch.original.affine, original)
        self.assertEqual(sketch.candidate.affine, candidate)
        self.assertIn("ctr-remaining", sketch.templates)
        self.assertEqual(sketch.termination.witness, "ctr-descending")
        obligation = sketch.to_obligation_dict()
        self.assertEqual(obligation["proof_kind"], "relational-induction")
        self.assertEqual(obligation["initiation"]["status"], "pending")
        self.assertTrue(obligation["preservation"]["invariants"])

    def test_shared_register_same_stride_uses_register_equality(self) -> None:
        original = find_ctr_affine_loop_candidates(
            _ctr_counted_loop(count=2, body_reg=3, addend=5),
        )[0]
        candidate = find_ctr_affine_loop_candidates(
            _ctr_counted_loop(count=2, body_reg=3, addend=5, base_address=0x40),
        )[0]
        sketch = build_relational_induction_sketch(original, candidate)
        assert isinstance(sketch, RelationalInductionSketch)
        self.assertIn("register-equality", sketch.templates)
        names = [item.name for item in sketch.initiation.invariants]
        self.assertIn("register-equality", names)

    def test_mismatched_trip_counts_unsupported(self) -> None:
        original = find_ctr_affine_loop_candidates(_ctr_counted_loop(count=4))[0]
        candidate = find_ctr_affine_loop_candidates(_ctr_counted_loop(count=3))[0]
        result = build_relational_induction_sketch(original, candidate)
        self.assertIsInstance(result, RelationalInductionUnsupported)
        assert isinstance(result, RelationalInductionUnsupported)
        self.assertIn("trip counts", result.reason)

    def test_mismatched_header_kinds_unsupported(self) -> None:
        original = find_ctr_affine_loop_candidates(_ctr_counted_loop(count=2))[0]
        analysis = analyze_loop_cfg(
            [
                _insn(Opcode.B, (8, 0), address=0),
                _insn(Opcode.B, (0, 0), address=8),
            ],
        )
        self.assertEqual(len(analysis.natural_loops), 1)
        candidate = analysis.natural_loops[0]
        result = build_relational_induction_sketch(original, candidate)
        self.assertIsInstance(result, RelationalInductionUnsupported)
        assert isinstance(result, RelationalInductionUnsupported)
        self.assertIn("mismatched loop header kinds", result.reason)

    def test_natural_loop_pair_builds_shape_only_sketch(self) -> None:
        original_cfg = analyze_loop_cfg(
            [
                _insn(Opcode.B, (8, 0), address=0),
                _insn(Opcode.B, (0, 0), address=8),
            ],
        )
        candidate_cfg = analyze_loop_cfg(
            [
                _insn(Opcode.B, (0x108, 0), address=0x100),
                _insn(Opcode.B, (0x100, 0), address=0x108),
            ],
        )
        original = RelationalLoopSide.from_natural(original_cfg.natural_loops[0])
        candidate = RelationalLoopSide.from_natural(candidate_cfg.natural_loops[0])
        sketch = build_relational_induction_sketch(original, candidate)
        assert isinstance(sketch, RelationalInductionSketch)
        self.assertIn("interval", sketch.templates)
        self.assertEqual(sketch.termination.witness, "natural-loop-ranking")

    def test_natural_loop_wrapper(self) -> None:
        analysis = analyze_loop_cfg(_ctr_counted_loop(count=1))
        loop: NaturalLoop = analysis.natural_loops[0]
        side = RelationalLoopSide.from_natural(loop)
        self.assertEqual(side.header_pc, loop.header_pc)
        self.assertEqual(side.natural, loop)

    def test_natural_loop_discharges_when_affine_backed(self) -> None:
        from tools.ppc_equivalence.relational_induction import (
            try_discharge_natural_relational,
        )

        program = _ctr_counted_loop(count=3)
        sketch = try_discharge_natural_relational(program, program)
        self.assertIsNotNone(sketch)
        assert sketch is not None
        self.assertEqual(sketch.status, "applied")
        self.assertIn("natural-loop", " ".join(sketch.notes))


class RelationalInductionFeatureGateTests(unittest.TestCase):
    def test_feature_is_supported(self) -> None:
        self.assertIn("relational-induction", KNOWN_PROOF_FEATURES)
        self.assertNotIn("relational-induction", UNSUPPORTED_FOR_EQUIVALENT)

    def test_incomplete_obligation_fails_validation(self) -> None:
        reason = validate_proof_features(
            {
                "proof_features": ["relational-induction"],
                "relational_induction": {"proof_kind": "relational-induction"},
            },
            require_equivalent_ready=True,
        )
        self.assertIsNotNone(reason)
        assert reason is not None
        self.assertIn("missing", reason)

    def test_discharged_sketch_stays_equivalent(self) -> None:
        from tools.ppc_equivalence.proof_features import enforce_equivalent_proof_features
        from tools.ppc_equivalence.relational_induction import (
            try_discharge_ctr_affine_relational,
        )
        from tools.ppc_equivalence.result import ProofResult, ProofStatus

        program = _ctr_counted_loop(count=4)
        sketch = try_discharge_ctr_affine_relational(program, program)
        self.assertIsNotNone(sketch)
        assert sketch is not None
        result = ProofResult(
            status=ProofStatus.EQUIVALENT,
            proof_features=["relational-induction"],
            relational_induction=sketch.to_obligation_dict(),
        )
        gated = enforce_equivalent_proof_features(result)
        self.assertEqual(gated.status, ProofStatus.EQUIVALENT)

    def test_obligation_validates_structurally(self) -> None:
        from tools.ppc_equivalence.relational_induction import (
            discharge_ctr_affine_relational_sketch,
        )

        original = find_ctr_affine_loop_candidates(_ctr_counted_loop(count=1))[0]
        candidate = find_ctr_affine_loop_candidates(
            _ctr_counted_loop(count=1, base_address=0x80),
        )[0]
        sketch = build_relational_induction_sketch(original, candidate)
        assert isinstance(sketch, RelationalInductionSketch)
        discharged = discharge_ctr_affine_relational_sketch(sketch)
        assert isinstance(discharged, RelationalInductionSketch)
        payload = {
            "proof_features": ["relational-induction"],
            "relational_induction": discharged.to_obligation_dict(),
        }
        self.assertIsNone(validate_proof_features(payload, require_equivalent_ready=True))



if __name__ == "__main__":
    unittest.main()
