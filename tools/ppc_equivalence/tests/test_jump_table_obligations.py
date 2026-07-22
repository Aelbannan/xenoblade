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
    write_hits_table_conditions,
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
        self.assertEqual(obligation["schema_version"], 2)
        self.assertEqual(obligation["original"]["byte_count"], 8)
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
        self.assertEqual(obligation["schema_version"], 2)
        self.assertEqual(obligation["original"]["coverage"], "unsat-remainder")

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
            def __init__(self, condition, memory, writes=()):
                self.condition = condition
                self.state = type("S", (), {"memory": memory, "memory_writes": writes})()

        terminals = [_Terminal(z3.BoolVal(True), written, writes=(ops.const(0x1000),))]
        solver = z3.Solver()
        solver.add(*rom_image_byte_constraints(initial, table, ops))
        solver.add(*rom_image_no_write_constraints(terminals, initial, table, ops))
        self.assertEqual(solver.check(), z3.unsat)

    @unittest.skipUnless(_HAS_Z3, "z3-solver is not installed")
    def test_write_hits_detects_store_same_value(self) -> None:
        """Final memory matches ROM, but memory_writes still hits the table."""
        ops = SymbolicOps()
        z3 = ops.z3
        table = JumpTableWords(base=0x1000, words=(0x11223344,), source="test")
        # Same value as image byte 0 (0x11).
        write_addr = ops.const(0x1000)

        class _Terminal:
            def __init__(self):
                self.condition = z3.BoolVal(True)
                self.state = type(
                    "S",
                    (),
                    {
                        "memory_writes": (write_addr,),
                        "memory": None,
                    },
                )()

        bad = write_hits_table_conditions([_Terminal()], table, ops)
        self.assertEqual(len(bad), 1)
        solver = z3.Solver()
        solver.add(z3.Or(*bad))
        self.assertEqual(solver.check(), z3.sat)

    @unittest.skipUnless(_HAS_Z3, "z3-solver is not installed")
    def test_write_hits_detects_store_restore(self) -> None:
        ops = SymbolicOps()
        z3 = ops.z3
        table = JumpTableWords(base=0x1000, words=(0x11223344,), source="test")
        writes = (ops.const(0x1000), ops.const(0x1000))

        class _Terminal:
            def __init__(self):
                self.condition = z3.BoolVal(True)
                self.state = type("S", (), {"memory_writes": writes, "memory": None})()

        bad = write_hits_table_conditions([_Terminal()], table, ops)
        self.assertEqual(len(bad), 2)
        solver = z3.Solver()
        solver.add(z3.Or(*bad))
        self.assertEqual(solver.check(), z3.sat)


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

    def test_proven_jump_table_context_attaches_discharged_obligations(self) -> None:
        # cmplwi r0,1; slwi r0,r0,2; lwzx r3,r3,r0; mtctr r3; bctr with lis/addi.
        # Without a bound branch, coverage may stay open; never claim soft EQUIVALENT
        # via invisible index premises. Obligations must still be schema v2.
        code = bytes.fromhex(
            "3c608001 38630000 28000001 5400103a 7c63002e 7c6903a6 4e800420"
        )
        base = 0x80001000
        insns = decode_block(code, base, validate_with_capstone=False)
        table = JumpTableWords(
            base=0x80010000,
            words=(0x80020000, 0x80020010),
            source="test-fixture",
        )
        context = JumpTableProofContext(
            table=table,
            branch_pc=base + 24,
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
        self.assertNotEqual(result.status, ProofStatus.EQUIVALENT)
        self.assertEqual(
            result.proof_features,
            ["readonly-image", "indirect-target-closure"],
        )
        self.assertIsNotNone(result.address_space)
        self.assertIsNotNone(result.indirect_targets)
        self.assertEqual(result.address_space["schema_version"], 2)
        self.assertEqual(result.indirect_targets["schema_version"], 2)

    def test_jump_table_context_bypasses_unresolved_indirect_gate(self) -> None:
        # With a JumpTableProofContext attached, the broadened M1 indirect-exit
        # gate must NOT fire: closure is handled by the discharge path, and a
        # retained indirect-branch remainder terminal is expected. Exact status
        # may still be non-EQUIVALENT (coverage open), but never via the M1 gate.
        code = bytes.fromhex(
            "3c608001 38630000 28000001 5400103a 7c63002e 7c6903a6 4e800420"
        )
        base = 0x80001000
        insns = decode_block(code, base, validate_with_capstone=False)
        table = JumpTableWords(
            base=0x80010000,
            words=(0x80020000, 0x80020010),
            source="test-fixture",
        )
        context = JumpTableProofContext(
            table=table,
            branch_pc=base + 24,
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
        self.assertFalse(
            any(
                "unresolved indirect-branch/call-indirect" in item
                for item in result.unsupported
            ),
            result.unsupported,
        )
        self.assertNotIn("indirect-exit-unproven", result.abstractions)

    def test_closed_set_excluding_true_targets_never_equivalent(self) -> None:
        """False-eq regression: original→A, candidate→B, closed set {C,D}.

        Remainder retention + coverage discharge must refuse EQUIVALENT.
        Exact status is inconclusive (coverage sat) or not-equivalent.
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
        self.assertNotEqual(result.status, ProofStatus.EQUIVALENT, result.unsupported)
        self.assertIn(
            result.status,
            {
                ProofStatus.NOT_EQUIVALENT,
                ProofStatus.INCONCLUSIVE_UNSUPPORTED,
            },
            result.unsupported or result.warnings,
        )
        # Coverage discharge must report a non-unsat remainder.
        coverage = result.indirect_targets["original"]["coverage"]
        if isinstance(coverage, dict):
            self.assertNotEqual(coverage["result"], "unsat")
        self.assertTrue(
            any("coverage" in item or "discharge" in item for item in result.unsupported)
            or result.status is ProofStatus.NOT_EQUIVALENT,
            result.unsupported,
        )

    def test_store_same_value_into_rom_fails_no_write_discharge(self) -> None:
        """stb of the existing ROM byte must still fail no-write discharge."""
        from tools.ppc_equivalence.ir import Instruction, Opcode

        table_base = 0x80010000
        # Image first byte is 0x80 (from 0x80020000 BE).
        words = (0x80020000, 0x80020010)
        table = JumpTableWords(base=table_base, words=words, source="rom-store")

        def _program() -> list[Instruction]:
            # li r4, 0x80; lis/addi r3,table_base; stb r4,0(r3); blr
            return [
                Instruction(0, 0, Opcode.ADDI, (4, 0, 0x80)),
                Instruction(4, 0, Opcode.ADDIS, (3, 0, (table_base >> 16) & 0xFFFF)),
                Instruction(8, 0, Opcode.ORI, (3, 3, table_base & 0xFFFF)),
                Instruction(12, 0, Opcode.STB, (4, 3, 0)),
                Instruction(16, 0, Opcode.BCLR, (20, 0, 0)),
            ]

        context = JumpTableProofContext(
            table=table,
            branch_pc=0xFFFFFFFC,  # unused; no bctr expansion
            table_base_reg=3,
            index_reg=0,
        )
        contract = make_contract(preset=None, observe=["r3"], timeout_ms=15_000)
        result = check_equivalence(
            _program(),
            _program(),
            contract,
            original_hex="00",
            candidate_hex="00",
            jump_table=context,
            max_paths=64,
        )
        self.assertNotEqual(result.status, ProofStatus.EQUIVALENT)
        no_write = result.address_space["original"]["no_write"]
        self.assertIsInstance(no_write, dict)
        self.assertEqual(no_write["result"], "sat")
        self.assertTrue(
            any("no-write" in item for item in result.unsupported),
            result.unsupported,
        )

    def test_store_restore_into_rom_fails_no_write_discharge(self) -> None:
        from tools.ppc_equivalence.ir import Instruction, Opcode

        table_base = 0x80010000
        words = (0x80020000, 0x80020010)
        table = JumpTableWords(base=table_base, words=words, source="rom-restore")

        def _program() -> list[Instruction]:
            # Write 0xFF then restore 0x80 into table[0].
            return [
                Instruction(0, 0, Opcode.ADDIS, (3, 0, (table_base >> 16) & 0xFFFF)),
                Instruction(4, 0, Opcode.ORI, (3, 3, table_base & 0xFFFF)),
                Instruction(8, 0, Opcode.ADDI, (4, 0, 0xFF)),
                Instruction(12, 0, Opcode.STB, (4, 3, 0)),
                Instruction(16, 0, Opcode.ADDI, (4, 0, 0x80)),
                Instruction(20, 0, Opcode.STB, (4, 3, 0)),
                Instruction(24, 0, Opcode.BCLR, (20, 0, 0)),
            ]

        context = JumpTableProofContext(
            table=table,
            branch_pc=0xFFFFFFFC,
            table_base_reg=3,
            index_reg=0,
        )
        contract = make_contract(preset=None, observe=["r3"], timeout_ms=15_000)
        result = check_equivalence(
            _program(),
            _program(),
            contract,
            original_hex="00",
            candidate_hex="00",
            jump_table=context,
            max_paths=64,
        )
        self.assertNotEqual(result.status, ProofStatus.EQUIVALENT)
        no_write = result.address_space["original"]["no_write"]
        self.assertIsInstance(no_write, dict)
        self.assertEqual(no_write["result"], "sat")


class JumpTableStrictValidationTests(unittest.TestCase):
    """Forged jump-table obligations must never authorize EQUIVALENT."""

    def _discharged_readonly(self) -> dict:
        import hashlib

        from tools.ppc_equivalence.jump_table_obligations import (
            build_dual_readonly_image_obligation,
        )

        table = JumpTableWords(base=0x80010000, words=(0x80020000,), source="test")
        return build_dual_readonly_image_obligation(
            table,
            no_write_status={
                "result": "unsat",
                "query_sha256": hashlib.sha256(b"nw").hexdigest(),
                "algorithm": "rom-image-no-write-v2",
            },
            status="discharged",
        )

    def _discharged_targets(self) -> dict:
        import hashlib

        obligation = build_indirect_targets_obligation(
            branch_pc=0x80001000,
            targets=(("case-0", 0x80020000),),
            source="test",
            artifact_hashes=(
                JumpTableWords(
                    base=0x80010000, words=(0x80020000,), source="test",
                ).image_sha256,
            ),
            coverage={
                "result": "unsat",
                "query_sha256": hashlib.sha256(b"cov").hexdigest(),
                "algorithm": "indirect-target-closure-v2",
            },
            status="discharged",
        )
        obligation["candidate"] = obligation["original"]
        return obligation

    def test_discharged_obligations_validate_strict(self) -> None:
        from tools.ppc_equivalence.jump_table_obligations import (
            validate_indirect_targets_obligation_strict,
            validate_readonly_image_obligation_strict,
        )

        self.assertIsNone(
            validate_readonly_image_obligation_strict(
                self._discharged_readonly(), require_discharged=True,
            ),
        )
        self.assertIsNone(
            validate_indirect_targets_obligation_strict(
                self._discharged_targets(), require_discharged=True,
            ),
        )

    def test_forged_no_write_sat_rejected(self) -> None:
        from tools.ppc_equivalence.jump_table_obligations import (
            validate_readonly_image_obligation_strict,
        )

        forged = self._discharged_readonly()
        forged["original"]["no_write"]["result"] = "sat"
        forged["candidate"] = forged["original"]
        self.assertIsNotNone(
            validate_readonly_image_obligation_strict(forged, require_discharged=True),
        )

    def test_forged_pending_status_rejected(self) -> None:
        from tools.ppc_equivalence.jump_table_obligations import (
            validate_readonly_image_obligation_strict,
        )

        forged = self._discharged_readonly()
        forged["status"] = "pending"
        self.assertIsNotNone(
            validate_readonly_image_obligation_strict(forged, require_discharged=True),
        )

    def test_forged_legacy_schema_rejected(self) -> None:
        from tools.ppc_equivalence.jump_table_obligations import (
            validate_readonly_image_obligation_strict,
        )

        forged = self._discharged_readonly()
        forged["schema_version"] = 1
        self.assertIsNotNone(
            validate_readonly_image_obligation_strict(forged, require_discharged=True),
        )

    def test_forged_uppercase_digest_rejected(self) -> None:
        from tools.ppc_equivalence.jump_table_obligations import (
            validate_indirect_targets_obligation_strict,
        )

        forged = self._discharged_targets()
        forged["original"]["coverage"]["query_sha256"] = (
            forged["original"]["coverage"]["query_sha256"].upper()
        )
        forged["candidate"] = forged["original"]
        self.assertIsNotNone(
            validate_indirect_targets_obligation_strict(
                forged, require_discharged=True,
            ),
        )

    def test_forged_missing_candidate_rejected(self) -> None:
        from tools.ppc_equivalence.jump_table_obligations import (
            validate_readonly_image_obligation_strict,
        )

        forged = self._discharged_readonly()
        forged.pop("candidate", None)
        self.assertIsNotNone(
            validate_readonly_image_obligation_strict(forged, require_discharged=True),
        )

    def test_forged_coverage_pending_demotes_equivalent(self) -> None:
        from tools.ppc_equivalence.proof_features import (
            enforce_equivalent_proof_features,
            validate_proof_features,
        )
        from tools.ppc_equivalence.result import ProofResult

        targets = self._discharged_targets()
        # Forge the coverage back to an undischarged string status.
        targets["original"]["coverage"] = "pending"
        targets["candidate"] = targets["original"]
        payload = {
            "proof_features": ["readonly-image", "indirect-target-closure"],
            "address_space": self._discharged_readonly(),
            "indirect_targets": targets,
        }
        reason = validate_proof_features(payload, require_equivalent_ready=True)
        self.assertIsNotNone(reason)

        result = ProofResult(
            status=ProofStatus.EQUIVALENT,
            proof_features=["readonly-image", "indirect-target-closure"],
            address_space=payload["address_space"],
            indirect_targets=targets,
        )
        gated = enforce_equivalent_proof_features(result)
        self.assertEqual(gated.status, ProofStatus.INCONCLUSIVE_UNSUPPORTED)

    def test_lenient_path_still_accepts_string_status(self) -> None:
        # Without require_equivalent_ready the lenient validators accept
        # pending/string obligations (they simply do not authorize EQUIVALENT).
        from tools.ppc_equivalence.proof_features import validate_proof_features

        payload = {
            "proof_features": ["readonly-image", "indirect-target-closure"],
            "address_space": build_readonly_image_obligation(
                JumpTableWords(base=0x80010000, words=(0x80020000,), source="t"),
                no_write_status="pending",
            ),
            "indirect_targets": build_indirect_targets_obligation(
                branch_pc=0x80001000,
                targets=(("case-0", 0x80020000),),
                source="t",
                artifact_hashes=("a" * 64,),
                coverage="pending",
            ),
        }
        self.assertIsNone(validate_proof_features(payload))


if __name__ == "__main__":
    unittest.main()
