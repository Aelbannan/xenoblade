"""Immutable BusSpecification identity and BusState isolation (PR 9 / Track D)."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.address_space import AddressSpace, Region, RegionKind, mmio_region
from tools.ppc_equivalence.bus_spec import (
    BusSpecification,
    BusState,
    DeviceSpecification,
    build_bus_specification,
)
from tools.ppc_equivalence.contract import EquivalenceContract, parse_observables
from tools.ppc_equivalence.device_model import RegisterBankDevice, RegisterSpec
from tools.ppc_equivalence.engine import run_concrete_sampling
from tools.ppc_equivalence.ir import Instruction, Opcode
from tools.ppc_equivalence.memory_bus import MemoryBus, build_memory_bus
from tools.ppc_equivalence.model import ConcreteMemory, concrete_state
from tools.ppc_equivalence.semantics import ConcreteOps, execute_cfg


def _insn(opcode: Opcode, operands: tuple[int, ...], *, address: int = 0) -> Instruction:
    return Instruction(address, 0, opcode, operands)


def _mmio_bank_bus(
    *,
    initial: int = 0x0000_00AB,
    w1c: bool = False,
    read_clear: bool = False,
) -> MemoryBus:
    device = RegisterBankDevice(
        base=0xCC008000,
        reg_width=4,
        registers=(
            RegisterSpec(
                offset=0x00,
                initial=initial,
                w1c=w1c,
                read_clear=read_clear,
            ),
        ),
    )
    mmio = mmio_region(0xCC008000, 0xCC008FFF, device_id="test-bank")
    return build_memory_bus(AddressSpace((mmio,)), devices={"test-bank": device})


class BusSpecificationIdentityTests(unittest.TestCase):
    def test_sha256_stable_for_identical_specs(self) -> None:
        bus_a = _mmio_bank_bus(initial=0x11, w1c=True, read_clear=False)
        bus_b = _mmio_bank_bus(initial=0x11, w1c=True, read_clear=False)
        assert bus_a.specification is not None
        assert bus_b.specification is not None
        self.assertEqual(bus_a.specification.sha256(), bus_b.specification.sha256())
        self.assertEqual(
            bus_a.specification.canonical_dict(),
            bus_b.specification.canonical_dict(),
        )

    def test_sha256_includes_register_flags(self) -> None:
        plain = _mmio_bank_bus(initial=0x11, w1c=False, read_clear=False)
        w1c = _mmio_bank_bus(initial=0x11, w1c=True, read_clear=False)
        read_clear = _mmio_bank_bus(initial=0x11, w1c=False, read_clear=True)
        assert plain.specification is not None
        assert w1c.specification is not None
        assert read_clear.specification is not None
        self.assertNotEqual(plain.specification.sha256(), w1c.specification.sha256())
        self.assertNotEqual(plain.specification.sha256(), read_clear.specification.sha256())
        self.assertNotEqual(w1c.specification.sha256(), read_clear.specification.sha256())

        # Flags appear explicitly in the canonical payload.
        w1c_regs = w1c.specification.canonical_dict()["devices"][0]["registers"]
        self.assertTrue(w1c_regs[0]["w1c"])
        self.assertFalse(w1c_regs[0]["read_clear"])


class BusStateIsolationTests(unittest.TestCase):
    def test_clone_does_not_share_device_values(self) -> None:
        state = BusState(
            ram=ConcreteMemory(default=0, bytes=((0x80000000, 0x11),)),
            device_values={"bank": {0: 0xAB}},
            event_logs={"fifo": [{"addr": "0x1", "width": 4, "value": "0x2"}]},
        )
        cloned = state.clone()
        state.device_values["bank"][0] = 0xFF
        state.event_logs["fifo"].append({"addr": "0x3"})
        state.ram = ConcreteMemory(default=1)
        self.assertEqual(cloned.device_values["bank"][0], 0xAB)
        self.assertEqual(len(cloned.event_logs["fifo"]), 1)
        self.assertEqual(cloned.ram.default, 0)

    def test_with_state_isolates_devices_from_source_bus(self) -> None:
        bus = _mmio_bank_bus(initial=0x0000_00AB)
        assert isinstance(bus.devices["test-bank"], RegisterBankDevice)
        snapshot = bus.snapshot_state().clone()
        isolated = bus.with_state(snapshot)

        # Mutate the source device; isolated clone must keep initial values.
        bus.store(0xCC008000, 4, 0xDEADBEEF)
        self.assertEqual(bus.load(0xCC008000, 4).value, 0xDEADBEEF)
        self.assertEqual(isolated.load(0xCC008000, 4).value, 0x0000_00AB)

        # Mutate the isolated device; source must stay at the mutated value.
        isolated.store(0xCC008000, 4, 0x0000_0011)
        self.assertEqual(isolated.load(0xCC008000, 4).value, 0x0000_0011)
        self.assertEqual(bus.load(0xCC008000, 4).value, 0xDEADBEEF)


class ConcreteSamplingBusIsolationTests(unittest.TestCase):
    def test_original_mmio_mutation_does_not_affect_candidate_initial(self) -> None:
        """Original CFG writes MMIO; candidate must still see pristine initial."""
        device = RegisterBankDevice(
            base=0xCC008000,
            reg_width=4,
            registers=(RegisterSpec(offset=0x00, initial=0x0000_00AB),),
        )
        mmio = mmio_region(0xCC008000, 0xCC008FFF, device_id="test-bank")
        ram = Region(0x80000000, 0x8000FFFF, RegionKind.RAM)
        bus = build_memory_bus(
            AddressSpace((ram, mmio)),
            devices={"test-bank": device},
        )

        # Original: write 0x11 to MMIO, then load into r4.
        # Candidate: load MMIO into r4 without writing first.
        # Shared-bus bug would make candidate see 0x11.
        original = [
            _insn(Opcode.ADDI, (3, 0, 0xCC008000), address=0),
            _insn(Opcode.ADDI, (5, 0, 0x11), address=4),
            _insn(Opcode.STW, (5, 3, 0), address=8),
            _insn(Opcode.LWZ, (4, 3, 0), address=12),
            _insn(Opcode.BCLR, (20, 0, 0), address=16),
        ]
        candidate = [
            _insn(Opcode.ADDI, (3, 0, 0xCC008000), address=0),
            _insn(Opcode.LWZ, (4, 3, 0), address=4),
            _insn(Opcode.BCLR, (20, 0, 0), address=8),
        ]

        initial_state = bus.snapshot_state().clone()
        original_bus = bus.with_state(initial_state.clone())
        candidate_bus = bus.with_state(initial_state.clone())

        machine = concrete_state({"lr": 20})
        ops = ConcreteOps()
        original_exits = execute_cfg(
            machine, original, ops, memory_bus=original_bus,
        )
        candidate_exits = execute_cfg(
            machine, candidate, ops, memory_bus=candidate_bus,
        )
        self.assertEqual(len(original_exits), 1)
        self.assertEqual(len(candidate_exits), 1)
        # Candidate must observe the pristine initial register value.
        self.assertEqual(candidate_exits[0].state.gpr[4] & 0xFFFFFFFF, 0x0000_00AB)
        # Original observed its own store.
        self.assertEqual(original_exits[0].state.gpr[4] & 0xFFFFFFFF, 0x0000_0011)
        # Caller's bus devices must remain at the pre-sampling initial.
        self.assertEqual(bus.load(0xCC008000, 4).value, 0x0000_00AB)

    def test_run_concrete_sampling_clones_bus_per_side(self) -> None:
        device = RegisterBankDevice(
            base=0xCC008000,
            reg_width=4,
            registers=(RegisterSpec(offset=0x00, initial=0x0000_00AB, read_clear=True),),
        )
        mmio = mmio_region(0xCC008000, 0xCC008FFF, device_id="test-bank")
        bus = build_memory_bus(AddressSpace((mmio,)), devices={"test-bank": device})

        # Fallthrough block (no blr): sampling initials use lr=0, which would
        # otherwise re-enter the block. Both sides load MMIO into r4. With
        # read-clear, a shared bus would zero the register for the candidate.
        program = [
            _insn(Opcode.ADDI, (3, 0, 0xCC008000), address=0),
            _insn(Opcode.LWZ, (4, 3, 0), address=4),
        ]
        contract = EquivalenceContract(parse_observables(["r4"]), timeout_ms=5_000)
        report = run_concrete_sampling(
            program,
            program,
            contract,
            sample_count=1,
            seed=0,
            memory_bus=bus,
        )
        self.assertFalse(report["mismatch_found"], report)
        self.assertGreaterEqual(report["samples_run"], 1)
        # Parent bus must still hold the initial value (not cleared by sampling).
        self.assertEqual(bus.load(0xCC008000, 4).value, 0x0000_00AB)


class DeviceSpecificationHelpersTests(unittest.TestCase):
    def test_build_bus_specification_orders_devices(self) -> None:
        bank = RegisterBankDevice(
            base=0xCC008000,
            registers=(RegisterSpec(offset=0, initial=1, w1c=True),),
        )
        space = AddressSpace((mmio_region(0xCC008000, 0xCC008FFF, device_id="z-bank"),))
        spec = build_bus_specification(space, {"z-bank": bank})
        self.assertIsInstance(spec, BusSpecification)
        self.assertEqual(len(spec.devices), 1)
        self.assertIsInstance(spec.devices[0], DeviceSpecification)
        self.assertEqual(spec.devices[0].theory, "register-bank")
        self.assertTrue(spec.devices[0].registers[0].w1c)


if __name__ == "__main__":
    unittest.main()
