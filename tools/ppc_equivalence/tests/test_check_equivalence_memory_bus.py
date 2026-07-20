"""Opt-in MemoryBus integration through ``check_equivalence`` (Tier C)."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.address_space import (
    AddressSpace,
    Region,
    RegionKind,
    mmio_region,
    rom_image_region,
)
from tools.ppc_equivalence.contract import EquivalenceContract, parse_observables
from tools.ppc_equivalence.device_model import (
    GxFifoStreamDevice,
    RegisterBankDevice,
    RegisterSpec,
)
from tools.ppc_equivalence.engine import check_equivalence
from tools.ppc_equivalence.ir import Instruction, Opcode
from tools.ppc_equivalence.memory_bus import build_memory_bus
from tools.ppc_equivalence.result import ProofStatus


def _insn(opcode: Opcode, operands: tuple[int, ...], *, address: int = 0) -> Instruction:
    return Instruction(address, 0, opcode, operands)


def _rom_bus() -> tuple[object, object]:
    image = b"\x12\x34\x56\x78"
    rom = rom_image_region(0x80200000, image)
    ram = Region(0x80000000, 0x801FFFFF, RegionKind.RAM)
    bus = build_memory_bus(AddressSpace((ram, rom)))
    return bus, rom


def _mmio_bank_bus() -> object:
    device = RegisterBankDevice(
        base=0xCC008000,
        reg_width=4,
        registers=(RegisterSpec(offset=0x00, initial=0x0000_00AB),),
    )
    mmio = mmio_region(0xCC008000, 0xCC008FFF, device_id="test-bank")
    return build_memory_bus(AddressSpace((mmio,)), devices={"test-bank": device})


class CheckEquivalenceMemoryBusTests(unittest.TestCase):
    def test_rom_read_proves_with_memory_bus(self) -> None:
        bus, _ = _rom_bus()
        program = [
            _insn(Opcode.ADDI, (3, 0, 0x80200000), address=0),
            _insn(Opcode.LWZ, (4, 3, 0), address=4),
            _insn(Opcode.BCLR, (20, 0, 0), address=8),
        ]
        contract = EquivalenceContract(parse_observables(["r4"]), timeout_ms=15_000)
        result = check_equivalence(
            program,
            program,
            contract,
            original_hex="00",
            candidate_hex="00",
            memory_bus=bus,
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT, result.unsupported)
        self.assertIn("memory-bus", result.proof_features)
        self.assertIsNotNone(result.memory_bus)
        assert result.memory_bus is not None
        self.assertEqual(result.memory_bus.get("status"), "discharged")
        self.assertEqual(result.memory_bus.get("mmio"), "cfg-routed")
        from tools.ppc_equivalence.memory_bus_obligations import (
            validate_memory_bus_obligation,
        )

        self.assertIsNone(validate_memory_bus_obligation(result.memory_bus))

    def test_rom_write_inconclusive_with_memory_bus(self) -> None:
        bus, _ = _rom_bus()
        program = [
            _insn(Opcode.ADDI, (3, 0, 0x80200000), address=0),
            _insn(Opcode.ADDI, (4, 0, 0xDEADBEEF), address=4),
            _insn(Opcode.STW, (4, 3, 0), address=8),
            _insn(Opcode.BCLR, (20, 0, 0), address=12),
        ]
        contract = EquivalenceContract(parse_observables(["r4"]), timeout_ms=15_000)
        result = check_equivalence(
            program,
            program,
            contract,
            original_hex="00",
            candidate_hex="00",
            memory_bus=bus,
        )
        self.assertEqual(result.status, ProofStatus.INCONCLUSIVE_LAYOUT)
        self.assertIn("memory-bus", result.proof_features)

    def test_unmapped_load_inconclusive_with_memory_bus(self) -> None:
        bus, _ = _rom_bus()
        program = [
            _insn(Opcode.ADDI, (3, 0, 0x90000000), address=0),
            _insn(Opcode.LWZ, (4, 3, 0), address=4),
            _insn(Opcode.BCLR, (20, 0, 0), address=8),
        ]
        contract = EquivalenceContract(parse_observables(["r4"]), timeout_ms=15_000)
        result = check_equivalence(
            program,
            program,
            contract,
            original_hex="00",
            candidate_hex="00",
            memory_bus=bus,
        )
        self.assertEqual(result.status, ProofStatus.INCONCLUSIVE_LAYOUT)
        self.assertIn("memory-bus", result.proof_features)

    def test_default_path_without_memory_bus_unchanged(self) -> None:
        program = [
            _insn(Opcode.ADDI, (3, 0, 0x80200000), address=0),
            _insn(Opcode.LWZ, (4, 3, 0), address=4),
            _insn(Opcode.BCLR, (20, 0, 0), address=8),
        ]
        contract = EquivalenceContract(parse_observables(["r4"]), timeout_ms=15_000)
        result = check_equivalence(
            program,
            program,
            contract,
            original_hex="00",
            candidate_hex="00",
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT, result.unsupported)
        self.assertNotIn("memory-bus", result.proof_features)
        self.assertIsNone(result.memory_bus)

    def test_concrete_sampling_uses_memory_bus(self) -> None:
        bus, _ = _rom_bus()
        program = [
            _insn(Opcode.ADDI, (3, 0, 0x80200000), address=0),
            _insn(Opcode.LWZ, (4, 3, 0), address=4),
            _insn(Opcode.BCLR, (20, 0, 0), address=8),
        ]
        contract = EquivalenceContract(parse_observables(["r4"]), timeout_ms=15_000)
        result = check_equivalence(
            program,
            program,
            contract,
            original_hex="00",
            candidate_hex="00",
            memory_bus=bus,
            concrete_samples=2,
        )
        self.assertIsNotNone(result.concrete_sampling)
        assert result.concrete_sampling is not None
        self.assertEqual(result.concrete_sampling["samples_requested"], 2)
        self.assertFalse(result.concrete_sampling["mismatch_found"])

    def test_mmio_access_equivalent_when_discharged(self) -> None:
        """Track D: pure-MMIO CFG with vacuous unsupported-access → EQUIVALENT."""
        bus = _mmio_bank_bus()
        program = [
            _insn(Opcode.ADDI, (3, 0, 0xCC008000), address=0),
            _insn(Opcode.LWZ, (4, 3, 0), address=4),
            _insn(Opcode.BCLR, (20, 0, 0), address=8),
        ]
        contract = EquivalenceContract(parse_observables(["r4"]), timeout_ms=15_000)
        result = check_equivalence(
            program,
            program,
            contract,
            original_hex="00",
            candidate_hex="00",
            memory_bus=bus,
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT, result.unsupported)
        self.assertIn("memory-bus", result.proof_features)
        assert result.memory_bus is not None
        self.assertEqual(result.memory_bus.get("mmio"), "cfg-routed")
        self.assertEqual(result.memory_bus.get("symbolic_mmio"), "cfg-routed")
        self.assertIn("register_bank_extensional", result.memory_bus)
        self.assertIn("bus_spec_sha256", result.memory_bus)
        unsupported = result.memory_bus.get("unsupported_access") or {}
        # Supported concrete register: no unsupported predicates ⇒ vacuous, not forged UNSAT.
        for side in ("original", "candidate"):
            side_block = unsupported.get(side) or {}
            self.assertEqual(
                side_block.get("status"),
                "vacuously-discharged",
                side_block,
            )
            self.assertEqual(side_block.get("reason"), "no-unsupported-predicates")
            self.assertIsInstance(side_block.get("cfg_trace_sha256"), str)
            self.assertEqual(len(side_block["cfg_trace_sha256"]), 64)
            self.assertIsInstance(side_block.get("access_coverage_sha256"), str)
            self.assertEqual(len(side_block["access_coverage_sha256"]), 64)
        self.assertEqual(result.memory_bus.get("status"), "discharged")
        self.assertEqual(result.memory_bus.get("schema_version"), 2)
        self.assertIn("access_coverage", result.memory_bus)
        self.assertTrue(result.memory_bus["access_coverage"].get("attested"))
        from tools.ppc_equivalence.memory_bus_obligations import (
            validate_memory_bus_obligation,
            symbolic_mmio_still_fail_closed,
        )
        from tools.ppc_equivalence.proof_features import UNSUPPORTED_FOR_EQUIVALENT

        self.assertIsNone(validate_memory_bus_obligation(result.memory_bus))
        self.assertNotIn("memory-bus", UNSUPPORTED_FOR_EQUIVALENT)
        self.assertFalse(symbolic_mmio_still_fail_closed())

    def test_forged_weak_memory_bus_obligation_demotes(self) -> None:
        """Negative: caller-forged regions-only discharged JSON cannot authorize."""
        from tools.ppc_equivalence.proof_features import enforce_equivalent_proof_features
        from tools.ppc_equivalence.result import ProofResult

        forged = ProofResult(
            status=ProofStatus.EQUIVALENT,
            proof_features=["memory-bus"],
            memory_bus={
                "algorithm": "memory-bus-v1",
                "status": "discharged",
                "regions": [{"kind": "ram", "start": 0, "end": 0xFF}],
            },
        )
        gated = enforce_equivalent_proof_features(forged)
        self.assertEqual(gated.status, ProofStatus.INCONCLUSIVE_UNSUPPORTED)
        self.assertTrue(
            any("memory_bus" in w or "discharged" in w for w in gated.warnings)
        )

    def test_unsupported_mmio_gap_inconclusive(self) -> None:
        """Reachable access to undeclared MMIO offset ⇒ unsupported-access SAT."""
        bus = _mmio_bank_bus()
        program = [
            _insn(Opcode.ADDI, (3, 0, 0xCC008010), address=0),
            _insn(Opcode.LWZ, (4, 3, 0), address=4),
            _insn(Opcode.BCLR, (20, 0, 0), address=8),
        ]
        contract = EquivalenceContract(parse_observables(["r4"]), timeout_ms=15_000)
        result = check_equivalence(
            program,
            program,
            contract,
            original_hex="00",
            candidate_hex="00",
            memory_bus=bus,
        )
        self.assertEqual(result.status, ProofStatus.INCONCLUSIVE_UNSUPPORTED)
        assert result.memory_bus is not None
        unsupported = result.memory_bus.get("unsupported_access") or {}
        self.assertEqual(unsupported.get("original", {}).get("result"), "sat")
        self.assertTrue(
            any("unsupported-access" in item for item in result.unsupported)
            or any("unsupported-access" in item for item in result.abstractions)
        )

    def test_register_bank_write_mismatch_not_equivalent(self) -> None:
        """End-to-end negative: differing MMIO register-bank stores."""
        bus = _mmio_bank_bus()
        original = [
            _insn(Opcode.ADDI, (3, 0, 0xCC008000), address=0),
            _insn(Opcode.ADDI, (4, 0, 0x1111), address=4),
            _insn(Opcode.STW, (4, 3, 0), address=8),
            _insn(Opcode.BCLR, (20, 0, 0), address=12),
        ]
        candidate = [
            _insn(Opcode.ADDI, (3, 0, 0xCC008000), address=0),
            _insn(Opcode.ADDI, (4, 0, 0x2222), address=4),
            _insn(Opcode.STW, (4, 3, 0), address=8),
            _insn(Opcode.BCLR, (20, 0, 0), address=12),
        ]
        contract = EquivalenceContract(parse_observables(["r4"]), timeout_ms=15_000)
        result = check_equivalence(
            original,
            candidate,
            contract,
            original_hex="00",
            candidate_hex="01",
            memory_bus=bus,
        )
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT, result.unsupported)
        self.assertIn("memory-bus", result.proof_features)

    def test_fifo_write_mismatch_not_equivalent(self) -> None:
        """End-to-end negative: differing GX FIFO write streams."""
        device = GxFifoStreamDevice(base=0xCC008100, span=0x20)
        mmio = mmio_region(0xCC008100, 0xCC00811F, device_id="gx-fifo")
        bus = build_memory_bus(AddressSpace((mmio,)), devices={"gx-fifo": device})
        original = [
            _insn(Opcode.ADDI, (3, 0, 0xCC008100), address=0),
            _insn(Opcode.ADDI, (4, 0, 0xAAAA), address=4),
            _insn(Opcode.STW, (4, 3, 0), address=8),
            _insn(Opcode.BCLR, (20, 0, 0), address=12),
        ]
        candidate = [
            _insn(Opcode.ADDI, (3, 0, 0xCC008100), address=0),
            _insn(Opcode.ADDI, (4, 0, 0xBBBB), address=4),
            _insn(Opcode.STW, (4, 3, 0), address=8),
            _insn(Opcode.BCLR, (20, 0, 0), address=12),
        ]
        contract = EquivalenceContract(parse_observables(["r4"]), timeout_ms=15_000)
        result = check_equivalence(
            original,
            candidate,
            contract,
            original_hex="00",
            candidate_hex="01",
            memory_bus=bus,
        )
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT, result.unsupported)
        self.assertIn("memory-bus", result.proof_features)


if __name__ == "__main__":
    unittest.main()
