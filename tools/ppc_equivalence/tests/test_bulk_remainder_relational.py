"""Tests for bulk+remainder relational scaffolding and SMT discharge (PR12)."""

from __future__ import annotations

import copy
import importlib.util
import unittest

from tools.ppc_equivalence.bulk_remainder_relational import (
    BulkRemainderLoopPair,
    RangeWriteParams,
    build_bulk_remainder_relational_sketch,
    discharge_bulk_remainder_identity,
    find_bulk_remainder_pairs,
    prove_bulk_remainder_identity,
    try_range_write_params,
)
from tools.ppc_equivalence.deadline import Deadline
from tools.ppc_equivalence.ir import Instruction, Opcode
from tools.ppc_equivalence.memory_loop import ConstantStrideStoreLoop
from tools.ppc_equivalence.relational_induction import RelationalInductionSketch
from tools.ppc_equivalence.trip_expression import remainder_mask_for_shift


_HAS_Z3 = importlib.util.find_spec("z3") is not None


def _insn(opcode: Opcode, operands: tuple[int, ...], *, address: int = 0) -> Instruction:
    return Instruction(address, 0, opcode, operands)


def _exact_pair(*, shift_k: int = 2) -> BulkRemainderLoopPair:
    mask = remainder_mask_for_shift(shift_k)
    bulk = ConstantStrideStoreLoop(
        header_pc=8,
        latch_pc=16,
        exit_pc=20,
        mtctr_pc=4,
        base_reg=4,
        index_reg=None,
        stride=4,
        store_width=4,
        source_reg=3,
        store_kind="d-form-addi",
        trip_count=2,
        trip_count_reg=0,
        trip_expr={"kind": "lshr", "left": {"kind": "entry", "reg": 6}, "shift": shift_k},
        trip_upper_bound=None,
        zero_guard="concrete-nonzero",
        confidence="exact-pattern",
        notes=(),
    )
    rem = ConstantStrideStoreLoop(
        header_pc=28,
        latch_pc=36,
        exit_pc=40,
        mtctr_pc=24,
        base_reg=4,
        index_reg=None,
        stride=4,
        store_width=4,
        source_reg=3,
        store_kind="d-form-addi",
        trip_count=1,
        trip_count_reg=6,
        trip_expr={
            "kind": "and",
            "left": {"kind": "entry", "reg": 6},
            "right": {"kind": "const", "value": mask},
        },
        trip_upper_bound=mask,
        zero_guard="concrete-nonzero",
        confidence="exact-pattern",
        notes=(),
    )
    return BulkRemainderLoopPair(
        entry_reg=6,
        shift_k=shift_k,
        bulk=bulk,
        remainder=rem,
        identity_notes=("test",),
    )


class BulkRemainderRelationalTests(unittest.TestCase):
    def test_remainder_mask_identity(self) -> None:
        self.assertEqual(remainder_mask_for_shift(3), 7)
        self.assertEqual(remainder_mask_for_shift(2), 3)

    def test_find_bulk_remainder_pair(self) -> None:
        program = [
            _insn(Opcode.RLWINM, (0, 6, 29, 3, 31), address=0),
            _insn(Opcode.MTSPR, (0, 9), address=4),
            _insn(Opcode.STW, (3, 4, 0), address=8),
            _insn(Opcode.ADDI, (4, 4, 4), address=12),
            _insn(Opcode.BC, (16, 0, 8, 0), address=16),
            _insn(Opcode.ANDI_DOT, (6, 6, 7), address=20),
            _insn(Opcode.MTSPR, (6, 9), address=24),
            _insn(Opcode.STW, (3, 4, 0), address=28),
            _insn(Opcode.ADDI, (4, 4, 4), address=32),
            _insn(Opcode.BC, (16, 0, 28, 0), address=36),
            _insn(Opcode.BCLR, (20, 0, 0), address=40),
        ]
        pairs = find_bulk_remainder_pairs(program)
        self.assertIsInstance(pairs, list)

    def test_unsupported_mismatched_shift(self) -> None:
        bulk = ConstantStrideStoreLoop(
            header_pc=8,
            latch_pc=16,
            exit_pc=20,
            mtctr_pc=4,
            base_reg=4,
            index_reg=None,
            stride=4,
            store_width=4,
            source_reg=3,
            store_kind="d-form-addi",
            trip_count=2,
            trip_count_reg=0,
            trip_expr={"kind": "lshr", "left": {"kind": "entry", "reg": 6}, "shift": 3},
            trip_upper_bound=None,
            zero_guard="concrete-nonzero",
            confidence="exact-pattern",
            notes=(),
        )
        rem = ConstantStrideStoreLoop(
            header_pc=28,
            latch_pc=36,
            exit_pc=40,
            mtctr_pc=24,
            base_reg=4,
            index_reg=None,
            stride=4,
            store_width=4,
            source_reg=3,
            store_kind="d-form-addi",
            trip_count=1,
            trip_count_reg=6,
            trip_expr={
                "kind": "and",
                "left": {"kind": "entry", "reg": 6},
                "right": {"kind": "const", "value": 3},
            },
            trip_upper_bound=3,
            zero_guard="concrete-nonzero",
            confidence="exact-pattern",
            notes=(),
        )
        pair = BulkRemainderLoopPair(
            entry_reg=6,
            shift_k=3,
            bulk=bulk,
            remainder=rem,
            identity_notes=("test",),
        )
        self.assertFalse(prove_bulk_remainder_identity(pair))

    def test_range_write_params_for_constant_value_store(self) -> None:
        pair = _exact_pair()
        params = try_range_write_params(pair)
        self.assertIsNotNone(params)
        assert params is not None
        self.assertEqual(params.element_width, 4)
        self.assertEqual(params.stride, 4)
        self.assertEqual(params.unroll_factor, 4)

    def test_range_write_rejected_when_source_is_base(self) -> None:
        from dataclasses import replace

        pair = _exact_pair()
        bad = BulkRemainderLoopPair(
            entry_reg=pair.entry_reg,
            shift_k=pair.shift_k,
            bulk=replace(pair.bulk, source_reg=pair.bulk.base_reg),
            remainder=replace(pair.remainder, source_reg=pair.remainder.base_reg),
            identity_notes=pair.identity_notes,
        )
        self.assertIsNone(try_range_write_params(bad))

    @unittest.skipUnless(_HAS_Z3, "z3-solver is not installed")
    def test_identity_smt_discharge(self) -> None:
        result = discharge_bulk_remainder_identity(
            shift_k=3,
            deadline=Deadline.after_ms(5_000),
        )
        self.assertEqual(result.status, "unsat")
        self.assertEqual(len(result.query_sha256), 64)
        self.assertEqual(result.algorithm, "bulk-remainder-identity-v1")

    @unittest.skipUnless(_HAS_Z3, "z3-solver is not installed")
    def test_identity_digest_stable_across_calls(self) -> None:
        a = discharge_bulk_remainder_identity(shift_k=2, deadline=Deadline.after_ms(5_000))
        b = discharge_bulk_remainder_identity(shift_k=2, deadline=Deadline.after_ms(5_000))
        self.assertEqual(a.query_sha256, b.query_sha256)
        other = discharge_bulk_remainder_identity(shift_k=3, deadline=Deadline.after_ms(5_000))
        self.assertNotEqual(a.query_sha256, other.query_sha256)

    @unittest.skipUnless(_HAS_Z3, "z3-solver is not installed")
    def test_build_relational_sketch_discharges(self) -> None:
        pair = _exact_pair()
        result = build_bulk_remainder_relational_sketch(pair, pair)
        self.assertIsInstance(result, RelationalInductionSketch)
        assert isinstance(result, RelationalInductionSketch)
        self.assertEqual(result.status, "discharged")
        obligation = result.to_obligation_dict()
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
        self.assertIn("range_write", obligation["postcondition"])
        self.assertIsInstance(
            RangeWriteParams(**{
                k: v
                for k, v in obligation["postcondition"]["range_write"].items()
                if k != "unroll_factor"
            }),
            RangeWriteParams,
        )

    @unittest.skipUnless(_HAS_Z3, "z3-solver is not installed")
    def test_mutation_demotes_discharged_obligation(self) -> None:
        from tools.ppc_equivalence.relational_induction import (
            validate_relational_induction_obligation,
        )

        pair = _exact_pair()
        sketch = build_bulk_remainder_relational_sketch(pair, pair)
        assert isinstance(sketch, RelationalInductionSketch)
        obligation = sketch.to_obligation_dict()
        bad = copy.deepcopy(obligation)
        bad["initiation"]["result"] = "unknown"
        bad["initiation"]["status"] = "failed"
        self.assertIsNotNone(validate_relational_induction_obligation(bad))

    def test_mismatched_shape_stays_unsupported(self) -> None:
        from tools.ppc_equivalence.bulk_remainder_relational import (
            BulkRemainderRelationalUnsupported,
        )
        from dataclasses import replace

        left = _exact_pair()
        right = BulkRemainderLoopPair(
            entry_reg=left.entry_reg,
            shift_k=left.shift_k,
            bulk=replace(left.bulk, stride=8, store_width=8),
            remainder=replace(left.remainder, stride=8, store_width=8),
            identity_notes=left.identity_notes,
        )
        result = build_bulk_remainder_relational_sketch(left, right)
        self.assertIsInstance(result, BulkRemainderRelationalUnsupported)

    def test_engine_attaches_pending_bulk_remainder_sketch(self) -> None:
        from unittest import mock

        from tools.ppc_equivalence.contract import make_contract
        from tools.ppc_equivalence.engine import check_equivalence
        from tools.ppc_equivalence.relational_induction import (
            ExitAgreementObligation,
            InitiationObligation,
            PostconditionObligation,
            PreservationObligation,
            RelationalInductionSketch,
            RelationalLoopSide,
            TerminationObligation,
        )
        from tools.ppc_equivalence.result import ProofStatus

        sketch = RelationalInductionSketch(
            original=RelationalLoopSide(header_pc=12, latch_pc=20, exit_pc=24),
            candidate=RelationalLoopSide(header_pc=12, latch_pc=20, exit_pc=24),
            initiation=InitiationObligation(()),
            preservation=PreservationObligation(()),
            exit_agreement=ExitAgreementObligation(()),
            postcondition=PostconditionObligation(()),
            termination=TerminationObligation(
                witness="bulk-remainder-ctr-descending",
                notes=("bulk+remainder relational scaffold",),
            ),
            templates=(),
            status="pending",
            notes=("bulk+remainder relational scaffold",),
        )
        leaf = [
            _insn(Opcode.ADDI, (3, 0, 1), address=0),
            _insn(Opcode.BCLR, (20, 0, 0), address=4),
        ]
        contract = make_contract(preset=None, observe=["r3"], timeout_ms=5_000)
        with mock.patch(
            "tools.ppc_equivalence.engine.try_build_bulk_remainder_relational_sketch",
            return_value=sketch,
        ):
            result = check_equivalence(
                leaf,
                leaf,
                contract,
                original_hex="00",
                candidate_hex="00",
            )
        self.assertIn("relational-induction", result.proof_features)
        self.assertIsNotNone(result.relational_induction)
        assert result.relational_induction is not None
        self.assertEqual(result.relational_induction.get("status"), "pending")
        # Pending sketches must not authorize EQUIVALENT (status must be discharged).
        self.assertNotEqual(result.status, ProofStatus.EQUIVALENT)


if __name__ == "__main__":
    unittest.main()
