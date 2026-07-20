"""Opt-in MemoryBus routing through execute_cfg (ConcreteOps, Tier C)."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.address_space import (
    AddressSpace,
    Region,
    RegionKind,
    mmio_region,
    rom_image_region,
)
from tools.ppc_equivalence.device_model import (
    GxFifoStreamDevice,
    RegisterBankDevice,
    RegisterSpec,
)
from tools.ppc_equivalence.ir import ExecutionInconclusive, Instruction, Opcode
from tools.ppc_equivalence.memory_bus import build_memory_bus
from tools.ppc_equivalence.model import concrete_state
from tools.ppc_equivalence.semantics import ConcreteOps, execute_cfg, execute_instruction


def _insn(opcode: Opcode, operands: tuple[int, ...], *, address: int = 0) -> Instruction:
    return Instruction(address, 0, opcode, operands)


class ExecuteCfgMemoryBusTests(unittest.TestCase):
    def setUp(self) -> None:
        self.image = b"\x12\x34\x56\x78"
        self.rom = rom_image_region(0x80200000, self.image)
        self.ram = Region(0x80000000, 0x801FFFFF, RegionKind.RAM)
        self.bus = build_memory_bus(AddressSpace((self.ram, self.rom)))

    def test_rom_read_via_execute_cfg(self) -> None:
        program = [
            _insn(Opcode.ADDI, (3, 0, 0x80200000), address=0),
            _insn(Opcode.LWZ, (4, 3, 0), address=4),
            _insn(Opcode.BCLR, (20, 0, 0), address=8),
        ]
        terminals = execute_cfg(
            concrete_state({"lr": 12}),
            program,
            ConcreteOps(),
            memory_bus=self.bus,
        )
        self.assertEqual(len(terminals), 1)
        self.assertEqual(terminals[0].state.gpr[4], 0x12345678)

    def test_rom_write_fail_closed(self) -> None:
        program = [
            _insn(Opcode.ADDI, (3, 0, 0x80200000), address=0),
            _insn(Opcode.ADDI, (4, 0, 0xDEADBEEF), address=4),
            _insn(Opcode.STW, (4, 3, 0), address=8),
            _insn(Opcode.BCLR, (20, 0, 0), address=12),
        ]
        with self.assertRaisesRegex(ExecutionInconclusive, "memory bus store: read-only"):
            execute_cfg(
                concrete_state({"lr": 16}),
                program,
                ConcreteOps(),
                memory_bus=self.bus,
            )

    def test_unmapped_load_fail_closed(self) -> None:
        program = [
            _insn(Opcode.ADDI, (3, 0, 0x90000000), address=0),
            _insn(Opcode.LWZ, (4, 3, 0), address=4),
            _insn(Opcode.BCLR, (20, 0, 0), address=8),
        ]
        with self.assertRaisesRegex(ExecutionInconclusive, "memory bus load: unmapped"):
            execute_cfg(
                concrete_state({"lr": 12}),
                program,
                ConcreteOps(),
                memory_bus=self.bus,
            )

    def test_default_path_without_bus_unchanged(self) -> None:
        state = concrete_state({
            "memory": {
                0x80200000: 0x12,
                0x80200001: 0x34,
                0x80200002: 0x56,
                0x80200003: 0x78,
            },
            "gpr": {"r3": 0x80200000},
            "lr": 8,
        })
        without_bus = execute_instruction(
            state,
            _insn(Opcode.LWZ, (4, 3, 0)),
            ConcreteOps(),
        )
        with_bus = execute_cfg(
            state,
            [
                _insn(Opcode.LWZ, (4, 3, 0), address=0),
                _insn(Opcode.BCLR, (20, 0, 0), address=4),
            ],
            ConcreteOps(),
        )
        self.assertEqual(without_bus.gpr[4], with_bus[0].state.gpr[4])

    def test_ram_store_round_trip_via_bus(self) -> None:
        program = [
            _insn(Opcode.ADDI, (3, 0, 0x80001000), address=0),
            _insn(Opcode.ADDI, (4, 0, 0xAABBCCDD), address=4),
            _insn(Opcode.STW, (4, 3, 0), address=8),
            _insn(Opcode.LWZ, (5, 3, 0), address=12),
            _insn(Opcode.BCLR, (20, 0, 0), address=16),
        ]
        terminals = execute_cfg(
            concrete_state({"lr": 20}),
            program,
            ConcreteOps(),
            memory_bus=self.bus,
        )
        self.assertEqual(terminals[0].state.gpr[5], 0xAABBCCDD)


class ExecuteCfgMmioDeviceTests(unittest.TestCase):
    """Live RegisterBank / GX FIFO devices through execute_cfg + memory_bus."""

    def setUp(self) -> None:
        self.bank = RegisterBankDevice(
            base=0xCC008000,
            reg_width=4,
            registers=(RegisterSpec(offset=0x00, initial=0x0000_00AB),),
        )
        self.fifo = GxFifoStreamDevice(base=0xCC008100, span=0x20)
        mmio_bank = mmio_region(0xCC008000, 0xCC0080FF, device_id="test-bank")
        mmio_fifo = mmio_region(0xCC008100, 0xCC00811F, device_id="gx-fifo")
        self.bus = build_memory_bus(
            AddressSpace((mmio_bank, mmio_fifo)),
            devices={"test-bank": self.bank, "gx-fifo": self.fifo},
        )

    def test_register_bank_read_via_execute_cfg(self) -> None:
        program = [
            _insn(Opcode.ADDI, (3, 0, 0xCC008000), address=0),
            _insn(Opcode.LWZ, (4, 3, 0), address=4),
            _insn(Opcode.BCLR, (20, 0, 0), address=8),
        ]
        terminals = execute_cfg(
            concrete_state({"lr": 12}),
            program,
            ConcreteOps(),
            memory_bus=self.bus,
        )
        self.assertEqual(terminals[0].state.gpr[4], 0x0000_00AB)

    def test_register_bank_write_read_round_trip(self) -> None:
        program = [
            _insn(Opcode.ADDI, (3, 0, 0xCC008000), address=0),
            _insn(Opcode.ADDI, (4, 0, 0x0000_00CD), address=4),
            _insn(Opcode.STW, (4, 3, 0), address=8),
            _insn(Opcode.LWZ, (5, 3, 0), address=12),
            _insn(Opcode.BCLR, (20, 0, 0), address=16),
        ]
        terminals = execute_cfg(
            concrete_state({"lr": 20}),
            program,
            ConcreteOps(),
            memory_bus=self.bus,
        )
        self.assertEqual(terminals[0].state.gpr[5], 0x0000_00CD)
        self.assertEqual(self.bank.read(0xCC008000, 4).value, 0x0000_00CD)

    def test_register_bank_unsupported_width_fail_closed(self) -> None:
        # Halfword load against a 4-byte register bank → UNSUPPORTED, not RAM.
        program = [
            _insn(Opcode.ADDI, (3, 0, 0xCC008000), address=0),
            _insn(Opcode.LHZ, (4, 3, 0), address=4),
            _insn(Opcode.BCLR, (20, 0, 0), address=8),
        ]
        with self.assertRaisesRegex(ExecutionInconclusive, "memory bus load: unsupported"):
            execute_cfg(
                concrete_state({"lr": 12}),
                program,
                ConcreteOps(),
                memory_bus=self.bus,
            )

    def test_gx_fifo_write_records_event(self) -> None:
        program = [
            _insn(Opcode.ADDI, (3, 0, 0xCC008100), address=0),
            _insn(Opcode.ADDI, (4, 0, 0x12345678), address=4),
            _insn(Opcode.STW, (4, 3, 0), address=8),
            _insn(Opcode.BCLR, (20, 0, 0), address=12),
        ]
        execute_cfg(
            concrete_state({"lr": 16}),
            program,
            ConcreteOps(),
            memory_bus=self.bus,
        )
        self.assertEqual(len(self.fifo.write_events), 1)
        self.assertEqual(self.fifo.write_events[0]["value"], hex(0x12345678))

    def test_gx_fifo_read_fail_closed(self) -> None:
        program = [
            _insn(Opcode.ADDI, (3, 0, 0xCC008100), address=0),
            _insn(Opcode.LWZ, (4, 3, 0), address=4),
            _insn(Opcode.BCLR, (20, 0, 0), address=8),
        ]
        with self.assertRaisesRegex(ExecutionInconclusive, "memory bus load: unsupported"):
            execute_cfg(
                concrete_state({"lr": 12}),
                program,
                ConcreteOps(),
                memory_bus=self.bus,
            )

    def test_missing_device_fail_closed_no_ram_fallback(self) -> None:
        orphan = mmio_region(0xCD000000, 0xCD000FFF, device_id="missing")
        bus = build_memory_bus(AddressSpace((orphan,)), devices={})
        program = [
            _insn(Opcode.ADDI, (3, 0, 0xCD000000), address=0),
            _insn(Opcode.LWZ, (4, 3, 0), address=4),
            _insn(Opcode.BCLR, (20, 0, 0), address=8),
        ]
        with self.assertRaisesRegex(ExecutionInconclusive, "memory bus load: unsupported"):
            execute_cfg(
                concrete_state({"lr": 12}),
                program,
                ConcreteOps(),
                memory_bus=bus,
            )


if __name__ == "__main__":
    unittest.main()
