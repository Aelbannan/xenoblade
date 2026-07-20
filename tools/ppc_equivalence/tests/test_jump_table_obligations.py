"""Tests for jump-table obligation helpers and engine fail-closed gate."""

from __future__ import annotations

import importlib.util
import unittest

from tools.ppc_equivalence.contract import make_contract
from tools.ppc_equivalence.decoder import decode_block
from tools.ppc_equivalence.engine import check_equivalence
from tools.ppc_equivalence.jump_table_obligations import (
    JumpTableProofContext,
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

    def test_proven_jump_table_context_can_be_equivalent(self) -> None:
        # cmplwi r0,1; slwi r0,r0,2; lwzx r3,r3,r0; mtctr r3; bctr
        # PR0 freeze: jump-table features demote EQUIVALENT → INCONCLUSIVE_UNSUPPORTED.
        code = bytes.fromhex("28000001 5400103a 7c63002e 7c6903a6 4e800420")
        base = 0x80001000
        insns = decode_block(code, base, validate_with_capstone=False)
        table = JumpTableWords(
            base=0x80010000,
            words=(0x80020000, 0x80020010),
            source="test-fixture",
        )
        context = JumpTableProofContext(
            table=table,
            branch_pc=base + 16,
            table_base_reg=3,
            index_reg=0,
        )
        contract = make_contract(preset=None, observe=["r3"], timeout_ms=15_000)
        result = check_equivalence(
            insns,
            insns,
            contract,
            original_hex=code.hex(),
            candidate_hex=code.hex(),
            jump_table=context,
            max_paths=64,
        )
        self.assertEqual(
            result.status,
            ProofStatus.INCONCLUSIVE_UNSUPPORTED,
            result.unsupported or result.warnings,
        )
        self.assertEqual(
            result.proof_features,
            ["readonly-image", "indirect-target-closure"],
        )
        self.assertIsNotNone(result.address_space)
        self.assertIsNotNone(result.indirect_targets)

    def test_closed_set_excluding_true_targets_never_equivalent(self) -> None:
        """False-eq regression: original→A, candidate→B, closed set {C,D}.

        Soft for PR0: must not be promotion-eligible / EQUIVALENT. Without the
        freeze the engine currently returns EQUIVALENT by expanding only C/D.
        """
        from tools.ppc_equivalence.ir import Instruction, Opcode

        target_a = 0x80020000
        target_b = 0x80020040
        target_c = 0x80020080
        target_d = 0x800200C0

        def _insn(
            opcode: Opcode,
            operands: tuple[int, ...],
            *,
            address: int = 0,
        ) -> Instruction:
            return Instruction(address, 0, opcode, operands)

        def _stub(pc: int, imm: int) -> list[Instruction]:
            return [
                _insn(Opcode.ADDI, (3, 0, imm), address=pc),
                _insn(Opcode.BCLR, (20, 0, 0), address=pc + 4),
            ]

        def _materialize_bctr(target: int) -> list[Instruction]:
            hi = (target >> 16) & 0xFFFF
            lo = target & 0xFFFF
            return [
                _insn(Opcode.ADDIS, (3, 0, hi), address=0),
                _insn(Opcode.ORI, (3, 3, lo), address=4),
                _insn(Opcode.MTSPR, (3, 9), address=8),
                _insn(Opcode.BCCTR, (20, 0, 0), address=12),
            ]

        handlers = {
            target_a: _stub(target_a, 1),
            target_b: _stub(target_b, 2),
            target_c: _stub(target_c, 3),
            target_d: _stub(target_d, 4),
        }

        def _assemble(target: int) -> list[Instruction]:
            out = _materialize_bctr(target)
            for pc in sorted(handlers):
                out.extend(handlers[pc])
            return out

        # Closed set excludes true targets A and B.
        context = JumpTableProofContext(
            table=JumpTableWords(
                base=0x80010000,
                words=(target_c, target_d),
                source="incomplete-closure",
            ),
            branch_pc=12,
            table_base_reg=4,  # do not pin r3 (materialized target)
            index_reg=0,
        )
        contract = make_contract(preset=None, observe=["r3"], timeout_ms=15_000)
        result = check_equivalence(
            _assemble(target_a),
            _assemble(target_b),
            contract,
            original_hex="00",
            candidate_hex="00",
            jump_table=context,
            max_paths=64,
        )
        # Soft: never EQUIVALENT (strengthen to inconclusive/not-equivalent later).
        self.assertNotEqual(result.status, ProofStatus.EQUIVALENT, result.unsupported)


if __name__ == "__main__":
    unittest.main()
