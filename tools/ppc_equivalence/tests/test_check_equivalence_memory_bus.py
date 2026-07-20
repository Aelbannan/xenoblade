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
from tools.ppc_equivalence.device_model import RegisterBankDevice, RegisterSpec
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
        self.assertEqual(result.status, ProofStatus.INCONCLUSIVE_UNSUPPORTED, result.unsupported)
        self.assertIn("memory-bus", result.proof_features)
        self.assertIsNotNone(result.memory_bus)

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

    def test_mmio_access_inconclusive_symbolic_fail_closed(self) -> None:
        """MMIO devices are live in concrete CFG; SMT still excludes MMIO."""
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
        self.assertEqual(result.status, ProofStatus.INCONCLUSIVE_LAYOUT)
        self.assertIn("memory-bus", result.proof_features)
        assert result.memory_bus is not None
        self.assertEqual(result.memory_bus.get("mmio"), "fail-closed")


if __name__ == "__main__":
    unittest.main()
