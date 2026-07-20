"""Unit tests for extensional symbolic register-bank scaffolding (PR 14 track)."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.address_space import AddressSpace, mmio_region
from tools.ppc_equivalence.bus_spec import DeviceSpecification, lift_symbolic_register_banks
from tools.ppc_equivalence.deadline import Deadline
from tools.ppc_equivalence.device_model import RegisterSpec
from tools.ppc_equivalence.memory_bus import build_memory_bus
from tools.ppc_equivalence.memory_bus_obligations import (
    enrich_memory_bus_obligation_with_symbolic_mmio,
    symbolic_mmio_still_fail_closed,
)
from tools.ppc_equivalence.proof_features import UNSUPPORTED_FOR_EQUIVALENT
from tools.ppc_equivalence.symbolic_bus import (
    ALGORITHM,
    THEORY,
    UnsupportedAccessStatus,
    access_supported,
    apply_register_write,
    build_register_bank_extensional_obligation,
    collect_mmio_touches_from_terminals,
    discharge_unsupported_access,
    initial_symbolic_register_bank,
    normal_write_next,
    query_unsupported_access,
    read_clear_next,
    read_returned_value,
    register_masks,
    route_symbolic_mmio_access,
    symbolic_bank_from_device_spec,
    symbolic_read,
    symbolic_write,
    w1c_write_next,
    width_mask,
)


def _z3():
    import z3

    return z3


class RegisterMaskFormulaTests(unittest.TestCase):
    def test_width_mask(self) -> None:
        self.assertEqual(width_mask(1), 0xFF)
        self.assertEqual(width_mask(2), 0xFFFF)
        self.assertEqual(width_mask(4), 0xFFFFFFFF)

    def test_normal_write_masks_from_spec(self) -> None:
        masks = register_masks(RegisterSpec(offset=0), 4)
        self.assertEqual(masks.writable, 0xFFFFFFFF)
        self.assertEqual(masks.w1c, 0)
        self.assertEqual(masks.read_clear, 0)

    def test_w1c_and_read_clear_masks(self) -> None:
        w1c = register_masks(RegisterSpec(offset=0, w1c=True), 4)
        self.assertEqual(w1c.writable, 0)
        self.assertEqual(w1c.w1c, 0xFFFFFFFF)
        rc = register_masks(RegisterSpec(offset=0, read_clear=True), 4)
        self.assertEqual(rc.read_clear, 0xFFFFFFFF)
        self.assertEqual(rc.writable, 0xFFFFFFFF)

    def test_normal_write_next_formula(self) -> None:
        z3 = _z3()
        old = z3.BitVecVal(0xFFFF_0000, 32)
        value = z3.BitVecVal(0x0000_00AB, 32)
        nxt = normal_write_next(old, value, 0xFFFFFFFF, z3, bits=32)
        self.assertEqual(z3.simplify(nxt).as_long(), 0x0000_00AB)

    def test_w1c_write_next_formula(self) -> None:
        z3 = _z3()
        old = z3.BitVecVal(0x0000_00FF, 32)
        value = z3.BitVecVal(0x0000_000F, 32)
        nxt = w1c_write_next(old, value, 0xFFFFFFFF, z3, bits=32)
        self.assertEqual(z3.simplify(nxt).as_long(), 0x0000_00F0)

    def test_read_clear_formulas(self) -> None:
        z3 = _z3()
        old = z3.BitVecVal(0xDEAD_BEEF, 32)
        returned = read_returned_value(old, 0xFFFFFFFF, z3, bits=32)
        nxt = read_clear_next(old, 0xFFFFFFFF, z3, bits=32)
        self.assertEqual(z3.simplify(returned).as_long(), 0xDEAD_BEEF)
        self.assertEqual(z3.simplify(nxt).as_long(), 0)

    def test_apply_register_write_matches_concrete_semantics(self) -> None:
        z3 = _z3()
        old = z3.BitVecVal(0x1111_1111, 32)
        value = z3.BitVecVal(0x0000_00FF, 32)
        plain = apply_register_write(
            old, value, RegisterSpec(offset=0), 4, z3,
        )
        w1c = apply_register_write(
            old, value, RegisterSpec(offset=0, w1c=True), 4, z3,
        )
        self.assertEqual(z3.simplify(plain).as_long(), 0x0000_00FF)
        self.assertEqual(z3.simplify(w1c).as_long(), 0x1111_1100)


class SymbolicRoutingTests(unittest.TestCase):
    def setUp(self) -> None:
        self.z3 = _z3()
        self.registers = (
            RegisterSpec(offset=0x00, initial=0x11),
            RegisterSpec(offset=0x04, initial=0x22, read_clear=True),
            RegisterSpec(offset=0x08, initial=0x33, w1c=True),
        )
        self.bank = initial_symbolic_register_bank(
            base=0xCC008000,
            registers=self.registers,
            reg_width=4,
            z3=self.z3,
            prefix="test.bank",
            device_id="test-bank",
        )

    def test_bank_from_device_spec(self) -> None:
        spec = DeviceSpecification(
            device_id="test-bank",
            theory="register-bank",
            base=0xCC008000,
            registers=self.registers,
            reg_width=4,
        )
        bank = symbolic_bank_from_device_spec(spec, self.z3)
        self.assertEqual(bank.base, 0xCC008000)
        self.assertEqual(set(bank.values), {0x00, 0x04, 0x08})

    def test_lift_hook_from_bus_specification(self) -> None:
        from tools.ppc_equivalence.address_space import AddressSpace
        from tools.ppc_equivalence.bus_spec import BusSpecification

        device = DeviceSpecification(
            device_id="test-bank",
            theory="register-bank",
            base=0xCC008000,
            registers=self.registers,
            reg_width=4,
        )
        fifo = DeviceSpecification(
            device_id="fifo",
            theory="gxfifo-stream",
            base=0xCC008100,
            span=0x100,
        )
        spec = BusSpecification(address_space=AddressSpace(()), devices=(device, fifo))
        banks = lift_symbolic_register_banks(spec, self.z3)
        self.assertEqual(set(banks), {"test-bank"})
        self.assertEqual(banks["test-bank"].reg_width, 4)

    def test_supported_when_addr_hits_register(self) -> None:
        addr = self.z3.BitVecVal(0xCC008004, 32)
        supported = access_supported(addr, self.bank, self.z3, width=4)
        self.assertEqual(self.z3.simplify(supported), self.z3.BoolVal(True))

    def test_unsupported_wrong_width(self) -> None:
        addr = self.z3.BitVecVal(0xCC008000, 32)
        supported = access_supported(addr, self.bank, self.z3, width=2)
        self.assertEqual(self.z3.simplify(supported), self.z3.BoolVal(False))

    def test_symbolic_read_routes_and_read_clears(self) -> None:
        # Constrain symbolic regs to known initials, then read offset 0x04.
        addr = self.z3.BitVecVal(0xCC008004, 32)
        solver = self.z3.Solver()
        for spec in self.registers:
            solver.add(self.bank.value_at(spec.offset) == self.z3.BitVecVal(spec.initial, 32))
        result = symbolic_read(addr, 4, self.bank, self.z3)
        solver.add(result.supported)
        solver.push()
        solver.add(result.value == self.z3.BitVecVal(0x22, 32))
        self.assertEqual(solver.check(), self.z3.sat)
        solver.pop()
        # Read-clear zeros that register in next_state.
        solver.add(result.next_state.value_at(0x04) == self.z3.BitVecVal(0, 32))
        self.assertEqual(solver.check(), self.z3.sat)

    def test_symbolic_write_w1c(self) -> None:
        addr = self.z3.BitVecVal(0xCC008008, 32)
        value = self.z3.BitVecVal(0x0000_000F, 32)
        bank = self.bank.with_values(
            {
                0x00: self.z3.BitVecVal(0, 32),
                0x04: self.z3.BitVecVal(0, 32),
                0x08: self.z3.BitVecVal(0x0000_00FF, 32),
            }
        )
        result = symbolic_write(addr, 4, value, bank, self.z3)
        solver = self.z3.Solver()
        solver.add(result.supported)
        solver.add(result.next_state.value_at(0x08) == self.z3.BitVecVal(0x0000_00F0, 32))
        self.assertEqual(solver.check(), self.z3.sat)


class UnsupportedAccessObligationTests(unittest.TestCase):
    def setUp(self) -> None:
        self.z3 = _z3()
        self.bank = initial_symbolic_register_bank(
            base=0xCC008000,
            registers=(RegisterSpec(offset=0x00), RegisterSpec(offset=0x04)),
            reg_width=4,
            z3=self.z3,
            prefix="unsup.bank",
        )

    def test_unsupported_access_sat_is_inconclusive(self) -> None:
        addr = self.z3.BitVec("mmio.addr", 32)
        path = self.z3.BoolVal(True)
        supported = access_supported(addr, self.bank, self.z3, width=4)
        query = query_unsupported_access(path, supported, self.z3)
        self.assertEqual(query.status, UnsupportedAccessStatus.SAT)
        self.assertTrue(query.inconclusive)

    def test_unsupported_access_unsat_when_addr_forced(self) -> None:
        addr = self.z3.BitVec("mmio.addr", 32)
        path = addr == self.z3.BitVecVal(0xCC008000, 32)
        supported = access_supported(addr, self.bank, self.z3, width=4)
        query = query_unsupported_access(path, supported, self.z3)
        self.assertEqual(query.status, UnsupportedAccessStatus.UNSAT)
        self.assertFalse(query.inconclusive)

    def test_does_not_assume_supported_into_equivalence_shape(self) -> None:
        """Equivalence query must not receive ``supported`` as a premise.

        The helper only returns a separate obligation result; callers must
        keep ``supported`` out of the main equivalence solver.
        """
        addr = self.z3.BitVec("mmio.addr", 32)
        supported = access_supported(addr, self.bank, self.z3, width=4)
        query = query_unsupported_access(self.z3.BoolVal(True), supported, self.z3)
        # Separate query found a bad access; equivalence must stay inconclusive.
        self.assertTrue(query.inconclusive)
        # A naive "assume supported" would make unsupported UNSAT — that is
        # forbidden. Check that Not(supported) is still SAT under True path.
        solver = self.z3.Solver()
        solver.add(self.z3.Not(supported))
        self.assertEqual(solver.check(), self.z3.sat)

    def test_obligation_block_shape(self) -> None:
        obligation = build_register_bank_extensional_obligation(
            bus_spec_sha256="abc",
            devices=[
                {
                    "device_id": "example",
                    "theory": THEORY,
                    "region": {"start": 0xCC008000, "end": 0xCC0080FF},
                }
            ],
            unsupported_access={
                "original": {"result": "sat", "query_sha256": "q0"},
                "candidate": {"result": "unsat", "query_sha256": "q1"},
            },
            status="scaffolded",
        )
        self.assertEqual(obligation["algorithm"], ALGORITHM)
        self.assertEqual(obligation["status"], "scaffolded")
        self.assertEqual(obligation["schema_version"], 1)
        self.assertEqual(obligation["devices"][0]["theory"], THEORY)
        self.assertEqual(obligation["unsupported_access"]["original"]["result"], "sat")


    def test_discharge_unsupported_access_matches_query(self) -> None:
        addr = self.z3.BitVec("mmio.addr", 32)
        path = addr == self.z3.BitVecVal(0xCC008000, 32)
        supported = access_supported(addr, self.bank, self.z3, width=4)
        query = discharge_unsupported_access(
            path_condition=path,
            supported=supported,
            deadline=Deadline.after_ms(5_000),
            z3=self.z3,
        )
        self.assertEqual(query.status, UnsupportedAccessStatus.UNSAT)
        self.assertFalse(query.inconclusive)

    def test_route_symbolic_mmio_access_write(self) -> None:
        addr = self.z3.BitVecVal(0xCC008000, 32)
        value = self.z3.BitVecVal(0x0000_00AB, 32)
        result = route_symbolic_mmio_access(
            addr=addr,
            width=4,
            bank=self.bank,
            z3=self.z3,
            value=value,
            is_write=True,
        )
        self.assertEqual(self.z3.simplify(result.supported), self.z3.BoolVal(True))


class MmioObservabilityTests(unittest.TestCase):
    def test_enrich_obligation_attaches_scaffold_blocks(self) -> None:
        from tools.ppc_equivalence.device_model import RegisterBankDevice

        device = RegisterBankDevice(
            base=0xCC008000,
            reg_width=4,
            registers=(RegisterSpec(offset=0x00, initial=0xAB),),
        )
        mmio = mmio_region(0xCC008000, 0xCC008FFF, device_id="test-bank")
        bus = build_memory_bus(AddressSpace((mmio,)), devices={"test-bank": device})
        base_obligation = {"algorithm": "memory-bus-v1", "mmio": "fail-closed"}
        enriched = enrich_memory_bus_obligation_with_symbolic_mmio(
            base_obligation,
            bus,
        )
        self.assertEqual(enriched["symbolic_mmio"], "scaffolded")
        self.assertIn("register_bank_extensional", enriched)
        self.assertEqual(
            enriched["register_bank_extensional"]["algorithm"],
            ALGORITHM,
        )
        self.assertIn("observability", enriched)


class FreezeGuardTests(unittest.TestCase):
    def test_memory_bus_still_unsupported_for_equivalent(self) -> None:
        self.assertIn("memory-bus", UNSUPPORTED_FOR_EQUIVALENT)

    def test_symbolic_mmio_still_fail_closed_hook(self) -> None:
        self.assertTrue(symbolic_mmio_still_fail_closed())


if __name__ == "__main__":
    unittest.main()
