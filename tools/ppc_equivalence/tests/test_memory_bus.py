from __future__ import annotations

import unittest

from tools.ppc_equivalence.address_space import (
    AddressSpace,
    Region,
    RegionKind,
    mmio_region,
    rom_image_region,
)
from tools.ppc_equivalence.device_model import RegisterBankDevice, RegisterSpec
from tools.ppc_equivalence.memory_bus import (
    BusOutcome,
    MemoryBus,
    build_memory_bus,
)
from tools.ppc_equivalence.model import ConcreteMemory


class TestMemoryBusRom(unittest.TestCase):
    def setUp(self) -> None:
        self.image = b"\x12\x34\x56\x78\x9A\xBC"
        self.rom = rom_image_region(0x80200000, self.image)
        self.ram = Region(0x80000000, 0x801FFFFF, RegionKind.RAM)
        self.bus = build_memory_bus(AddressSpace((self.ram, self.rom)))

    def test_rom_read_word(self) -> None:
        result = self.bus.load(0x80200000, 4)
        self.assertEqual(result.outcome, BusOutcome.OK)
        self.assertEqual(result.value, 0x12345678)

    def test_rom_read_halfword(self) -> None:
        result = self.bus.load(0x80200004, 2)
        self.assertEqual(result.outcome, BusOutcome.OK)
        self.assertEqual(result.value, 0x9ABC)

    def test_rom_write_rejected(self) -> None:
        result = self.bus.store(0x80200000, 4, 0xDEADBEEF)
        self.assertEqual(result.outcome, BusOutcome.READ_ONLY)
        reread = self.bus.load(0x80200000, 4)
        self.assertEqual(reread.value, 0x12345678)


class TestMemoryBusMmio(unittest.TestCase):
    def setUp(self) -> None:
        self.device = RegisterBankDevice(
            base=0xCC008000,
            reg_width=4,
            registers=(RegisterSpec(offset=0x00, initial=0x0000_00AB),),
        )
        mmio = mmio_region(0xCC008000, 0xCC008FFF, device_id="test-bank")
        self.bus = build_memory_bus(
            AddressSpace((mmio,)),
            devices={"test-bank": self.device},
        )

    def test_mmio_register_read_via_bus(self) -> None:
        result = self.bus.load(0xCC008000, 4)
        self.assertEqual(result.outcome, BusOutcome.OK)
        self.assertEqual(result.value, 0x0000_00AB)

    def test_mmio_register_write_via_bus(self) -> None:
        write = self.bus.store(0xCC008000, 4, 0x0000_00CD)
        self.assertEqual(write.outcome, BusOutcome.OK)
        read = self.bus.load(0xCC008000, 4)
        self.assertEqual(read.value, 0x0000_00CD)

    def test_mmio_missing_device_fail_closed(self) -> None:
        mmio = mmio_region(0xCD000000, 0xCD000FFF, device_id="missing")
        bus = build_memory_bus(AddressSpace((mmio,)), devices={})
        self.assertEqual(bus.load(0xCD000000, 4).outcome, BusOutcome.UNSUPPORTED)
        self.assertEqual(bus.store(0xCD000000, 4, 0).outcome, BusOutcome.UNSUPPORTED)


class TestMemoryBusUnmapped(unittest.TestCase):
    def test_unmapped_load_and_store_rejected(self) -> None:
        bus = build_memory_bus(AddressSpace(()))
        self.assertEqual(bus.load(0x90000000, 4).outcome, BusOutcome.UNMAPPED)
        self.assertEqual(bus.store(0x90000000, 4, 0).outcome, BusOutcome.UNMAPPED)

    def test_explicit_unmapped_region(self) -> None:
        hole = Region(0x90000000, 0x90000FFF, RegionKind.UNMAPPED)
        bus = build_memory_bus(AddressSpace((hole,)))
        self.assertEqual(bus.load(0x90000000, 4).outcome, BusOutcome.UNMAPPED)


class TestMemoryBusRam(unittest.TestCase):
    def test_ram_round_trip(self) -> None:
        ram_region = Region(0x80000000, 0x8000FFFF, RegionKind.RAM)
        ram = ConcreteMemory(default=0, bytes=((0x80000000, 0x11),))
        bus = build_memory_bus(AddressSpace((ram_region,)), ram=ram)
        self.assertEqual(bus.load(0x80000000, 1).value, 0x11)
        self.assertEqual(bus.store(0x80000000, 4, 0xAABBCCDD).outcome, BusOutcome.OK)
        self.assertEqual(bus.load(0x80000000, 4).value, 0xAABBCCDD)


class TestMemoryBusFailClosed(unittest.TestCase):
    def test_span_multiple_regions(self) -> None:
        ram = Region(0x80000000, 0x801FFFFF, RegionKind.RAM)
        rom = rom_image_region(0x80200000, b"\x00" * 8)
        bus = build_memory_bus(AddressSpace((ram, rom)))
        self.assertEqual(
            bus.load(0x801FFFFE, 4).outcome,
            BusOutcome.SPAN_MULTIPLE_REGIONS,
        )
        self.assertEqual(
            bus.store(0x801FFFFE, 4, 0).outcome,
            BusOutcome.SPAN_MULTIPLE_REGIONS,
        )

    def test_unsupported_width(self) -> None:
        ram = Region(0x80000000, 0x8000FFFF, RegionKind.RAM)
        bus = build_memory_bus(AddressSpace((ram,)))
        self.assertEqual(bus.load(0x80000000, 8).outcome, BusOutcome.UNSUPPORTED)
        self.assertEqual(bus.store(0x80000000, 8, 0).outcome, BusOutcome.UNSUPPORTED)

    def test_misaligned_access(self) -> None:
        ram = Region(0x80000000, 0x8000FFFF, RegionKind.RAM)
        bus = build_memory_bus(AddressSpace((ram,)))
        self.assertEqual(bus.load(0x80000001, 4).outcome, BusOutcome.MISALIGNED)
        self.assertEqual(bus.store(0x80000001, 2, 0).outcome, BusOutcome.MISALIGNED)


if __name__ == "__main__":
    unittest.main()
