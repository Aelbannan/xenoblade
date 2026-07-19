"""Tests for jump-table obligation helpers and engine fail-closed gate."""

from __future__ import annotations

import importlib.util
import unittest

from tools.ppc_equivalence.contract import make_contract
from tools.ppc_equivalence.decoder import decode_block
from tools.ppc_equivalence.engine import check_equivalence
from tools.ppc_equivalence.jump_table_obligations import (
    JumpTableWords,
    build_indirect_targets_obligation,
    build_readonly_image_obligation,
    jump_table_gate_reason,
    rom_image_byte_constraints,
    rom_image_no_write_constraints,
    validate_indirect_targets_obligation,
    validate_readonly_image_obligation,
)
from tools.ppc_equivalence.result import ProofStatus
from tools.ppc_equivalence.semantics import SymbolicOps


_HAS_Z3 = importlib.util.find_spec("z3") is not None

# cmplwi r0,1; slwi r0,r0,2; lwzx r3,r4,r0; mtctr r3; bctr
_JUMP_TABLE_HEX = "28000001 5400103a 7c63202e 7c6903a6 4e800420"
# addi r3,r3,4; blr
_LEAF_HEX = "38630004 4e800020"


class JumpTableObligationTests(unittest.TestCase):
    def test_builds_and_validates_readonly_obligation(self) -> None:
        table = JumpTableWords(
            base=0x80010000,
            words=(0x80020000, 0x80020010),
            source="test",
            artifact_path="fixture.elf",
        )
        obligation = build_readonly_image_obligation(table, no_write_status="unsat")
        self.assertIsNone(validate_readonly_image_obligation(obligation))
        self.assertEqual(obligation["byte_count"], 8)
        self.assertEqual(obligation["image_sha256"], table.image_sha256)

    def test_builds_and_validates_indirect_targets(self) -> None:
        obligation = build_indirect_targets_obligation(
            branch_pc=0x80001234,
            targets=(("case0", 0x80020000), ("case1", 0x80020010)),
            source="linked-elf",
            artifact_hashes=("a" * 64,),
            coverage="unsat-remainder",
        )
        self.assertIsNone(validate_indirect_targets_obligation(obligation))

    def test_rejects_incomplete_obligations(self) -> None:
        self.assertIsNotNone(validate_readonly_image_obligation({"kind": "rom-image"}))
        self.assertIsNotNone(validate_indirect_targets_obligation({"branch_pc": 1}))

    def test_gate_reason_detects_jump_table(self) -> None:
        insns = decode_block(bytes.fromhex(_JUMP_TABLE_HEX), 0x80000000, validate_with_capstone=False)
        leaf = decode_block(bytes.fromhex(_LEAF_HEX), 0x80000000, validate_with_capstone=False)
        self.assertIsNotNone(jump_table_gate_reason(insns, insns))
        self.assertIsNone(jump_table_gate_reason(leaf, leaf))

    @unittest.skipUnless(_HAS_Z3, "z3-solver is not installed")
    def test_rom_byte_constraints_are_satisfiable(self) -> None:
        ops = SymbolicOps()
        z3 = ops.z3
        initial = z3.Array("mem", z3.BitVecSort(32), z3.BitVecSort(8))
        table = JumpTableWords(base=0x1000, words=(0xAABBCCDD,), source="test")
        solver = z3.Solver()
        solver.add(*rom_image_byte_constraints(initial, table, ops))
        self.assertEqual(solver.check(), z3.sat)
        model = solver.model()
        self.assertEqual(
            model.eval(z3.Select(initial, z3.BitVecVal(0x1000, 32))).as_long(),
            0xAA,
        )

    @unittest.skipUnless(_HAS_Z3, "z3-solver is not installed")
    def test_no_write_constraints_reject_store_into_rom(self) -> None:
        ops = SymbolicOps()
        z3 = ops.z3
        initial = z3.Array("mem", z3.BitVecSort(32), z3.BitVecSort(8))
        table = JumpTableWords(base=0x1000, words=(0x11223344,), source="test")
        written = ops.store_byte(initial, ops.const(0x1000), ops.const(0xFF))

        class _Terminal:
            def __init__(self, condition, memory):
                self.condition = condition
                self.state = type("S", (), {"memory": memory})()

        terminals = [_Terminal(z3.BoolVal(True), written)]
        solver = z3.Solver()
        solver.add(*rom_image_byte_constraints(initial, table, ops))
        solver.add(*rom_image_no_write_constraints(terminals, initial, table, ops))
        self.assertEqual(solver.check(), z3.unsat)


@unittest.skipUnless(_HAS_Z3, "z3-solver is not installed")
class JumpTableEngineGateTests(unittest.TestCase):
    def test_identical_jump_tables_do_not_claim_equivalent(self) -> None:
        code = bytes.fromhex(_JUMP_TABLE_HEX)
        insns = decode_block(code, 0x80000000, validate_with_capstone=False)
        contract = make_contract(preset=None, observe=["r3"], timeout_ms=10_000)
        result = check_equivalence(
            insns,
            insns,
            contract,
            original_hex=code.hex(),
            candidate_hex=code.hex(),
        )
        self.assertEqual(result.status, ProofStatus.INCONCLUSIVE_UNSUPPORTED)
        self.assertTrue(
            any("jump-table" in item for item in result.unsupported),
            result.unsupported,
        )


if __name__ == "__main__":
    unittest.main()
