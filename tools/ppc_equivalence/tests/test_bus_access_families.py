"""Track B: memory-bus access families must route or fail closed on MMIO."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.address_space import AddressSpace, Region, RegionKind, mmio_region
from tools.ppc_equivalence.bus_access import (
    BUS_ACCESS_FAMILY_DCBZ,
    BUS_ACCESS_FAMILY_INTEGER,
    BUS_ACCESS_FAMILY_LMW,
    BUS_ACCESS_FAMILY_PSQ,
    BUS_ACCESS_FAMILY_SCALAR_FP,
    REASON_PSQ_MMIO,
    last_bus_access_coverage,
)
from tools.ppc_equivalence.device_model import RegisterBankDevice, RegisterSpec
from tools.ppc_equivalence.ir import ExecutionInconclusive, Instruction, Opcode
from tools.ppc_equivalence.memory_bus import build_memory_bus
from tools.ppc_equivalence.model import concrete_state
from tools.ppc_equivalence.semantics import ConcreteOps, execute_cfg


def _insn(opcode: Opcode, operands: tuple[int, ...], *, address: int = 0) -> Instruction:
    return Instruction(address, 0, opcode, operands)


def _mmio_bank_bus() -> tuple[object, RegisterBankDevice]:
    bank = RegisterBankDevice(
        base=0xCC008000,
        reg_width=4,
        registers=(
            RegisterSpec(offset=0x00, initial=0x1111_2222),
            RegisterSpec(offset=0x04, initial=0x3333_4444),
        ),
    )
    mmio = mmio_region(0xCC008000, 0xCC0080FF, device_id="test-bank")
    ram = Region(0x80000000, 0x801FFFFF, RegionKind.RAM)
    bus = build_memory_bus(AddressSpace((ram, mmio)), devices={"test-bank": bank})
    return bus, bank


class AccessFamilyMmioTests(unittest.TestCase):
    """Each family either routes through the bus or fail-closes on MMIO."""

    def test_scalar_fp_lfs_routes_through_mmio_bank(self) -> None:
        bus, bank = _mmio_bank_bus()
        # lfs f1, 0(r3) — 4-byte FP load from MMIO register.
        program = [
            _insn(Opcode.ADDI, (3, 0, 0xCC008000), address=0),
            _insn(Opcode.LFS, (1, 3, 0), address=4),
            _insn(Opcode.BCLR, (20, 0, 0), address=8),
        ]
        terminals = execute_cfg(
            concrete_state({"lr": 12}),
            program,
            ConcreteOps(),
            memory_bus=bus,
        )
        self.assertEqual(len(terminals), 1)
        # Device was read (no silent RAM).
        self.assertEqual(bank.read(0xCC008000, 4).value, 0x1111_2222)
        coverage = last_bus_access_coverage()
        assert coverage is not None
        self.assertIn(BUS_ACCESS_FAMILY_SCALAR_FP, coverage["families"])

    def test_scalar_fp_lfd_two_word_mmio_route(self) -> None:
        bus, bank = _mmio_bank_bus()
        program = [
            _insn(Opcode.ADDI, (3, 0, 0xCC008000), address=0),
            _insn(Opcode.LFD, (1, 3, 0), address=4),
            _insn(Opcode.BCLR, (20, 0, 0), address=8),
        ]
        terminals = execute_cfg(
            concrete_state({"lr": 12}),
            program,
            ConcreteOps(),
            memory_bus=bus,
        )
        self.assertEqual(len(terminals), 1)
        # Two ordered 32-bit MMIO reads (not silent RAM / not atomic-64).
        self.assertEqual(bank.read(0xCC008000, 4).value, 0x1111_2222)
        self.assertEqual(bank.read(0xCC008004, 4).value, 0x3333_4444)
        coverage = last_bus_access_coverage()
        assert coverage is not None
        self.assertIn(BUS_ACCESS_FAMILY_SCALAR_FP, coverage["families"])

    def test_stfiwx_routes_through_mmio_bank(self) -> None:
        bus, bank = _mmio_bank_bus()
        # f1 low word written via stfiwx to MMIO.
        program = [
            _insn(Opcode.STFIWX, (1, 3, 0), address=0),
            _insn(Opcode.BCLR, (20, 0, 0), address=4),
        ]
        state = concrete_state({
            "lr": 8,
            "fpr": {"f1": 0x0000_0000_ABCD_EF01},
            "gpr": {"r3": 0xCC008000, "r0": 0},
        })
        execute_cfg(state, program, ConcreteOps(), memory_bus=bus)
        self.assertEqual(bank.read(0xCC008000, 4).value, 0xABCD_EF01)
        coverage = last_bus_access_coverage()
        assert coverage is not None
        self.assertIn(BUS_ACCESS_FAMILY_SCALAR_FP, coverage["families"])

    def test_psq_mmio_fail_closed(self) -> None:
        bus, _bank = _mmio_bank_bus()
        # psq_l frs,0(ra),w,i — operands (rs, ra, disp, w, i)
        program = [
            _insn(Opcode.ADDI, (3, 0, 0xCC008000), address=0),
            _insn(Opcode.PSQ_L, (1, 3, 0, 0, 0), address=4),
            _insn(Opcode.BCLR, (20, 0, 0), address=8),
        ]
        with self.assertRaisesRegex(ExecutionInconclusive, REASON_PSQ_MMIO):
            execute_cfg(
                concrete_state({"lr": 12, "gqr": {"gqr0": 0}}),
                program,
                ConcreteOps(),
                memory_bus=bus,
            )
        coverage = last_bus_access_coverage()
        assert coverage is not None
        self.assertIn(BUS_ACCESS_FAMILY_PSQ, coverage["families"])
        self.assertIn(REASON_PSQ_MMIO, coverage["rejections"])

    def test_lmw_routes_through_mmio_bank(self) -> None:
        bus, bank = _mmio_bank_bus()
        # lmw r31, 0(r3) — single word from MMIO into r31.
        program = [
            _insn(Opcode.ADDI, (3, 0, 0xCC008000), address=0),
            _insn(Opcode.LMW, (31, 3, 0), address=4),
            _insn(Opcode.BCLR, (20, 0, 0), address=8),
        ]
        terminals = execute_cfg(
            concrete_state({"lr": 12}),
            program,
            ConcreteOps(),
            memory_bus=bus,
        )
        self.assertEqual(terminals[0].state.gpr[31], 0x1111_2222)
        coverage = last_bus_access_coverage()
        assert coverage is not None
        self.assertIn(BUS_ACCESS_FAMILY_LMW, coverage["families"])

    def test_stmw_routes_through_mmio_bank(self) -> None:
        bus, bank = _mmio_bank_bus()
        program = [
            _insn(Opcode.STMW, (31, 3, 0), address=0),
            _insn(Opcode.BCLR, (20, 0, 0), address=4),
        ]
        execute_cfg(
            concrete_state({"lr": 8, "gpr": {"r3": 0xCC008000, "r31": 0xDEAD_BEEF}}),
            program,
            ConcreteOps(),
            memory_bus=bus,
        )
        self.assertEqual(bank.read(0xCC008000, 4).value, 0xDEAD_BEEF)
        coverage = last_bus_access_coverage()
        assert coverage is not None
        self.assertIn(BUS_ACCESS_FAMILY_LMW, coverage["families"])

    def test_dcbz_mmio_fail_closed(self) -> None:
        bus, _bank = _mmio_bank_bus()
        # dcbz r0, r3 with HID0.DCE set; block covers MMIO base.
        program = [
            _insn(Opcode.DCBZ, (0, 3), address=0),
            _insn(Opcode.BCLR, (20, 0, 0), address=4),
        ]
        with self.assertRaisesRegex(
            ExecutionInconclusive,
            r"memory-bus-unsupported-access-family:dcbz-mmio",
        ):
            execute_cfg(
                concrete_state({
                    "lr": 8,
                    "gpr": {"r3": 0xCC008000},
                    "spr": {"hid0": 0x4000},
                }),
                program,
                ConcreteOps(),
                memory_bus=bus,
            )
        coverage = last_bus_access_coverage()
        assert coverage is not None
        self.assertIn(BUS_ACCESS_FAMILY_DCBZ, coverage["families"])

    def test_dcbz_ram_routes_via_bus(self) -> None:
        bus, _bank = _mmio_bank_bus()
        program = [
            _insn(Opcode.DCBZ, (0, 3), address=0),
            _insn(Opcode.BCLR, (20, 0, 0), address=4),
        ]
        terminals = execute_cfg(
            concrete_state({
                "lr": 8,
                "gpr": {"r3": 0x80001000},
                "spr": {"hid0": 0x4000},
                "memory": {0x80001000: 0xFF, 0x8000101F: 0xEE},
            }),
            program,
            ConcreteOps(),
            memory_bus=bus,
        )
        mem = terminals[0].state.memory
        self.assertEqual(mem.read(0x80001000), 0)
        self.assertEqual(mem.read(0x8000101F), 0)
        coverage = last_bus_access_coverage()
        assert coverage is not None
        self.assertIn(BUS_ACCESS_FAMILY_DCBZ, coverage["families"])

    def test_integer_family_still_recorded(self) -> None:
        bus, bank = _mmio_bank_bus()
        program = [
            _insn(Opcode.ADDI, (3, 0, 0xCC008000), address=0),
            _insn(Opcode.LWZ, (4, 3, 0), address=4),
            _insn(Opcode.BCLR, (20, 0, 0), address=8),
        ]
        terminals = execute_cfg(
            concrete_state({"lr": 12}),
            program,
            ConcreteOps(),
            memory_bus=bus,
        )
        self.assertEqual(terminals[0].state.gpr[4], 0x1111_2222)
        coverage = last_bus_access_coverage()
        assert coverage is not None
        self.assertIn(BUS_ACCESS_FAMILY_INTEGER, coverage["families"])

    def test_psq_ram_still_runs_with_bus(self) -> None:
        """PSQ on ordinary RAM under memory_bus= must not false-reject."""
        bus, _bank = _mmio_bank_bus()
        program = [
            _insn(Opcode.PSQ_L, (1, 3, 0, 1, 0), address=0),
            _insn(Opcode.BCLR, (20, 0, 0), address=4),
        ]
        # Float type (gqr type 0), w=1 single element; RAM at 0x80001000.
        terminals = execute_cfg(
            concrete_state({
                "lr": 8,
                "gpr": {"r3": 0x80001000},
                "gqr": {"gqr0": 0},
                "memory": {
                    0x80001000: 0x3F,
                    0x80001001: 0x80,
                    0x80001002: 0x00,
                    0x80001003: 0x00,
                },
            }),
            program,
            ConcreteOps(),
            memory_bus=bus,
        )
        self.assertEqual(len(terminals), 1)
        coverage = last_bus_access_coverage()
        assert coverage is not None
        self.assertIn(BUS_ACCESS_FAMILY_PSQ, coverage["families"])
        self.assertNotIn(REASON_PSQ_MMIO, coverage["rejections"])


class PerSideAccessCoverageTests(unittest.TestCase):
    """Finding 2: coverage must retain both original and candidate families."""

    def test_static_opcode_families_lwz_vs_lfs(self) -> None:
        from tools.ppc_equivalence.bus_access import static_opcode_families

        original = [
            _insn(Opcode.ADDI, (3, 0, 0xCC008000), address=0),
            _insn(Opcode.LWZ, (4, 3, 0), address=4),
            _insn(Opcode.BCLR, (20, 0, 0), address=8),
        ]
        candidate = [
            _insn(Opcode.ADDI, (3, 0, 0xCC008000), address=0),
            _insn(Opcode.LFS, (1, 3, 0), address=4),
            _insn(Opcode.BCLR, (20, 0, 0), address=8),
        ]
        self.assertEqual(
            static_opcode_families(original),
            [BUS_ACCESS_FAMILY_INTEGER],
        )
        self.assertEqual(
            static_opcode_families(candidate),
            [BUS_ACCESS_FAMILY_SCALAR_FP],
        )

    def test_per_side_dynamic_survives_second_execute_cfg(self) -> None:
        from tools.ppc_equivalence.bus_access import (
            clear_side_bus_access_coverage,
            per_side_bus_access_coverage,
            side_bus_access_coverage,
        )

        bus, _bank = _mmio_bank_bus()
        clear_side_bus_access_coverage()
        original = [
            _insn(Opcode.ADDI, (3, 0, 0xCC008000), address=0),
            _insn(Opcode.LWZ, (4, 3, 0), address=4),
            _insn(Opcode.BCLR, (20, 0, 0), address=8),
        ]
        candidate = [
            _insn(Opcode.ADDI, (3, 0, 0xCC008000), address=0),
            _insn(Opcode.LFS, (1, 3, 0), address=4),
            _insn(Opcode.BCLR, (20, 0, 0), address=8),
        ]
        execute_cfg(
            concrete_state({"lr": 12}),
            original,
            ConcreteOps(),
            memory_bus=bus,
            bus_access_side="original",
        )
        execute_cfg(
            concrete_state({"lr": 12}),
            candidate,
            ConcreteOps(),
            memory_bus=bus,
            bus_access_side="candidate",
        )
        # Sampling-style third run overwrites last, not per-side store.
        execute_cfg(
            concrete_state({"lr": 12}),
            candidate,
            ConcreteOps(),
            memory_bus=bus,
        )
        sides = per_side_bus_access_coverage()
        orig = sides["original"]
        cand = sides["candidate"]
        assert orig is not None and cand is not None
        self.assertIn(BUS_ACCESS_FAMILY_INTEGER, orig["families"])
        self.assertIn(BUS_ACCESS_FAMILY_SCALAR_FP, cand["families"])
        stored_orig = side_bus_access_coverage("original")
        assert stored_orig is not None
        self.assertIn(BUS_ACCESS_FAMILY_INTEGER, stored_orig["families"])
        # last coverage is candidate-only after the third run
        last = last_bus_access_coverage()
        assert last is not None
        self.assertIn(BUS_ACCESS_FAMILY_SCALAR_FP, last["families"])
        self.assertNotIn(BUS_ACCESS_FAMILY_INTEGER, last["families"])

    def test_attestation_includes_both_families_without_observing_dests(self) -> None:
        from tools.ppc_equivalence.bus_access import clear_side_bus_access_coverage
        from tools.ppc_equivalence.memory_bus_obligations import (
            build_access_coverage_attestation,
        )

        clear_side_bus_access_coverage()
        original = [
            _insn(Opcode.ADDI, (3, 0, 0xCC008000), address=0),
            _insn(Opcode.LWZ, (4, 3, 0), address=4),
            _insn(Opcode.BCLR, (20, 0, 0), address=8),
        ]
        candidate = [
            _insn(Opcode.ADDI, (3, 0, 0xCC008000), address=0),
            _insn(Opcode.LFS, (1, 3, 0), address=4),
            _insn(Opcode.BCLR, (20, 0, 0), address=8),
        ]
        coverage = build_access_coverage_attestation(
            attested=True,
            original_instructions=original,
            candidate_instructions=candidate,
        )
        self.assertIn(BUS_ACCESS_FAMILY_INTEGER, coverage["original"]["opcode_families"])
        self.assertIn(
            BUS_ACCESS_FAMILY_SCALAR_FP,
            coverage["candidate"]["opcode_families"],
        )
        self.assertEqual(
            coverage["sides"]["original"]["sha256"],
            coverage["original"]["sha256"],
        )
        self.assertNotEqual(
            coverage["original"]["sha256"],
            coverage["candidate"]["sha256"],
        )

    def test_engine_lwz_vs_lfs_coverage_both_sides(self) -> None:
        """Review reproduction: neither dest observed; both families attested."""
        from tools.ppc_equivalence.contract import EquivalenceContract, parse_observables
        from tools.ppc_equivalence.engine import check_equivalence
        from tools.ppc_equivalence.result import ProofStatus

        bus, _bank = _mmio_bank_bus()
        original = [
            _insn(Opcode.ADDI, (3, 0, 0xCC008000), address=0),
            _insn(Opcode.LWZ, (4, 3, 0), address=4),
            _insn(Opcode.BCLR, (20, 0, 0), address=8),
        ]
        candidate = [
            _insn(Opcode.ADDI, (3, 0, 0xCC008000), address=0),
            _insn(Opcode.LFS, (1, 3, 0), address=4),
            _insn(Opcode.BCLR, (20, 0, 0), address=8),
        ]
        # Observe neither r4 nor f1 — previously only candidate family survived.
        # Contract still needs ≥1 observable; use lr (untouched by either load).
        contract = EquivalenceContract(parse_observables(["lr"]), timeout_ms=15_000)
        result = check_equivalence(
            original,
            candidate,
            contract,
            original_hex="00",
            candidate_hex="00",
            memory_bus=bus,
            concrete_samples=2,
        )
        self.assertIn("memory-bus", result.proof_features)
        assert result.memory_bus is not None
        coverage = result.memory_bus["access_coverage"]
        self.assertIn(BUS_ACCESS_FAMILY_INTEGER, coverage["original"]["opcode_families"])
        self.assertIn(
            BUS_ACCESS_FAMILY_SCALAR_FP,
            coverage["candidate"]["opcode_families"],
        )
        # Freeze may demote EQUIVALENT; coverage shape is the Finding 2 gate.
        self.assertIn(
            result.status,
            (ProofStatus.EQUIVALENT, ProofStatus.INCONCLUSIVE_UNSUPPORTED),
        )


class EabiHelperBusTests(unittest.TestCase):
    def test_savegpr_helper_mmio_routes_via_symbolic_bus(self) -> None:
        """Fixed ``_savegpr_31`` summary stores through SymbolicBus on MMIO."""
        try:
            import z3  # noqa: F401
        except ImportError:
            self.skipTest("z3 required")

        from dataclasses import replace

        from tools.ppc_equivalence.bus_access import (
            BUS_ACCESS_FAMILY_EABI_HELPER,
            begin_bus_access_coverage,
            end_bus_access_coverage,
        )
        from tools.ppc_equivalence.engine import _symbolic_initial
        from tools.ppc_equivalence.semantics import (
            CalleeContract,
            SymbolicOps,
            _MEMORY_BUS,
            _apply_call_summary,
        )
        from tools.ppc_equivalence.symbolic_bus import initial_symbolic_bus_state

        bus, _bank = _mmio_bank_bus()
        ops = SymbolicOps()
        z3 = ops.z3
        token = _MEMORY_BUS.set(bus)
        begin_bus_access_coverage()
        try:
            state = _symbolic_initial(ops)
            lifted = initial_symbolic_bus_state(bus, z3)
            assert lifted is not None
            # Point r11 so _savegpr_31 stores at r11-4 = MMIO base.
            gpr = list(state.gpr)
            gpr[11] = z3.BitVecVal(0xCC008004, 32)
            gpr[31] = z3.BitVecVal(0xCAFEBABE, 32)
            state = replace(state, gpr=tuple(gpr), symbolic_bus=lifted)
            contract = CalleeContract(
                frozenset({"r11", "r31", "memory", "valid"}),
                frozenset({"memory", "valid"}),
                "fixed-eabi-runtime-helper:_savegpr_31",
            )
            result = _apply_call_summary(state, ops, "_savegpr_31", contract)
            self.assertIsNotNone(result.symbolic_bus)
            assert result.symbolic_bus is not None
            self.assertIn("test-bank", result.symbolic_bus.touched_devices)
            coverage = end_bus_access_coverage()
            self.assertIn(BUS_ACCESS_FAMILY_EABI_HELPER, coverage["families"])
        finally:
            _MEMORY_BUS.reset(token)



if __name__ == "__main__":
    unittest.main()
