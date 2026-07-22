"""Object-base MEM1 constraints and RAM-only bus projection."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.address_space import AddressSpace, Region, RegionKind, mmio_region
from tools.ppc_equivalence.contract import make_contract
from tools.ppc_equivalence.decoder import decode_block, parse_hex
from tools.ppc_equivalence.device_model import RegisterBankDevice, RegisterSpec
from tools.ppc_equivalence.engine import check_equivalence
from tools.ppc_equivalence.ir import Opcode
from tools.ppc_equivalence.memory_bus import build_memory_bus
from tools.ppc_equivalence.object_base import (
    MEM1_RANGE,
    instruction_may_form_mmio_address,
    mem1_gpr_ranges,
    mmio_ranges_from_bus_or_profile,
    ram_only_memory_bus,
    ram_ranges_from_platform_profile,
    should_use_ram_only_bus,
)
from tools.ppc_equivalence.result import ProofStatus


class Mem1GprRangesTests(unittest.TestCase):
    def test_default_r3_mem1(self) -> None:
        ranges = mem1_gpr_ranges()
        self.assertEqual(ranges, {3: MEM1_RANGE})
        self.assertEqual(ranges[3], (0x80000000, 0x817FFFFF))

    def test_profile_prefers_mem1_start(self) -> None:
        profile = {
            "regions": [
                {"kind": "ram", "start": "0x90000000", "end": "0x93ffffff"},
                {"kind": "ram", "start": "0x80000000", "end": "0x817fffff"},
            ]
        }
        ranges = mem1_gpr_ranges(registers=(3, 4), profile=profile)
        self.assertEqual(ranges[3], (0x80000000, 0x817FFFFF))
        self.assertEqual(ranges[4], (0x80000000, 0x817FFFFF))

    def test_fallback_without_profile(self) -> None:
        self.assertEqual(ram_ranges_from_platform_profile(None), [MEM1_RANGE])


class InitialGprRangesEngineTests(unittest.TestCase):
    def test_blr_with_mem1_ranges_equivalent(self) -> None:
        code = decode_block(parse_hex("4e800020"), validate_with_capstone=False)
        contract = make_contract(preset="ppc-eabi", observe=None, timeout_ms=5_000)
        ranges = mem1_gpr_ranges()
        result = check_equivalence(
            code,
            code,
            contract,
            original_hex="4e800020",
            candidate_hex="4e800020",
            initial_gpr_ranges=ranges,
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT)
        self.assertTrue(
            any(item.startswith("object-base-mem1") for item in result.assumptions),
            result.assumptions[-5:],
        )
        self.assertIn(
            "object-base-mem1:r3:[0x80000000,0x817fffff]",
            result.assumptions,
        )


class MmioLiteralHeuristicTests(unittest.TestCase):
    def test_ordinary_addi_lwz_false(self) -> None:
        # addi r3, r3, 0x10 ; lwz r4, 0(r3)
        ordinary = decode_block(
            parse_hex("38630010 80830000"),
            validate_with_capstone=False,
        )
        mmio = [(0xCC003000, 0xCC00301F), (0xCC008000, 0xCC00807F)]
        self.assertFalse(instruction_may_form_mmio_address(ordinary, mmio))
        self.assertFalse(instruction_may_form_mmio_address([], mmio))

    def test_lis_cc00_true(self) -> None:
        # lis r3, 0xCC00  ==  addis r3, r0, 0xCC00
        lis = decode_block(parse_hex("3c60cc00"), validate_with_capstone=False)
        self.assertEqual(lis[0].opcode, Opcode.ADDIS)
        self.assertTrue(instruction_may_form_mmio_address(lis, []))
        self.assertTrue(
            instruction_may_form_mmio_address(
                lis, [(0xCC003000, 0xCC00301F)],
            )
        )


class RamOnlyBusTests(unittest.TestCase):
    def _ram_mmio_bus(self):
        ram = Region(start=0x80000000, end=0x817FFFFF, kind=RegionKind.RAM, label="MEM1")
        mmio = mmio_region(0xCC003000, 0xCC00301F, device_id="pi-stub", label="PI")
        device = RegisterBankDevice(
            base=0xCC003000,
            reg_width=4,
            registers=(RegisterSpec(offset=0, initial=0),),
        )
        return build_memory_bus(AddressSpace((ram, mmio)), devices={"pi-stub": device})

    def test_ram_only_strips_mmio(self) -> None:
        bus = self._ram_mmio_bus()
        self.assertTrue(any(r.kind is RegionKind.MMIO for r in bus.address_space.regions))
        projected = ram_only_memory_bus(bus)
        self.assertTrue(all(r.kind is not RegionKind.MMIO for r in projected.address_space.regions))
        self.assertEqual(projected.devices, {})
        self.assertTrue(any(r.kind is RegionKind.RAM for r in projected.address_space.regions))
        self.assertEqual(
            mmio_ranges_from_bus_or_profile(memory_bus=projected),
            [],
        )

    def test_should_use_ram_only_when_no_mmio_literals(self) -> None:
        bus = self._ram_mmio_bus()
        ordinary = decode_block(
            parse_hex("80630000 4e800020"),  # lwz r3, 0(r3); blr
            validate_with_capstone=False,
        )
        self.assertTrue(should_use_ram_only_bus(ordinary, ordinary, bus))

        lis = decode_block(parse_hex("3c60cc00 4e800020"), validate_with_capstone=False)
        self.assertFalse(should_use_ram_only_bus(lis, ordinary, bus))
        self.assertFalse(should_use_ram_only_bus(ordinary, ordinary, None))


if __name__ == "__main__":
    unittest.main()
