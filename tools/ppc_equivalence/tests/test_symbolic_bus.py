"""Unit tests for extensional symbolic register-bank CFG routing (PR 14 track)."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.address_space import AddressSpace, mmio_region
from tools.ppc_equivalence.bus_spec import DeviceSpecification, lift_symbolic_register_banks
from tools.ppc_equivalence.deadline import Deadline
from tools.ppc_equivalence.device_model import RegisterSpec
from tools.ppc_equivalence.ir import Instruction, Opcode
from tools.ppc_equivalence.memory_bus import build_memory_bus
from tools.ppc_equivalence.memory_bus_obligations import (
    build_memory_bus_obligation,
    enrich_memory_bus_obligation_with_symbolic_mmio,
    symbolic_mmio_still_fail_closed,
)
from tools.ppc_equivalence.proof_features import UNSUPPORTED_FOR_EQUIVALENT
from tools.ppc_equivalence.semantics import SymbolicOps, execute_cfg
from tools.ppc_equivalence.symbolic_bus import (
    ALGORITHM,
    THEORY,
    UnsupportedAccessStatus,
    access_supported,
    apply_register_write,
    apply_symbolic_bus_access,
    build_register_bank_extensional_obligation,
    discharge_unsupported_access,
    initial_symbolic_bus_state,
    initial_symbolic_register_bank,
    normal_write_next,
    query_unsupported_access,
    read_clear_next,
    read_returned_value,
    register_masks,
    route_symbolic_mmio_access,
    symbolic_bank_from_device_spec,
    symbolic_bus_difference,
    symbolic_read,
    symbolic_write,
    w1c_write_next,
    width_mask,
)


def _z3():
    import z3

    return z3


def _insn(opcode: Opcode, operands: tuple[int, ...], *, address: int = 0) -> Instruction:
    return Instruction(address, 0, opcode, operands)


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
        addr = self.z3.BitVec("mmio.addr", 32)
        supported = access_supported(addr, self.bank, self.z3, width=4)
        query = query_unsupported_access(self.z3.BoolVal(True), supported, self.z3)
        self.assertTrue(query.inconclusive)
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


class CfgRoutingTests(unittest.TestCase):
    def test_apply_symbolic_bus_access_concrete_register(self) -> None:
        from tools.ppc_equivalence.device_model import RegisterBankDevice

        z3 = _z3()
        device = RegisterBankDevice(
            base=0xCC008000,
            reg_width=4,
            registers=(RegisterSpec(offset=0x00, initial=0xAB),),
        )
        mmio = mmio_region(0xCC008000, 0xCC008FFF, device_id="test-bank")
        bus = build_memory_bus(AddressSpace((mmio,)), devices={"test-bank": device})
        state = initial_symbolic_bus_state(bus, z3)
        assert state is not None
        outcome = apply_symbolic_bus_access(
            state,
            address_space=bus.address_space,
            addr=z3.BitVecVal(0xCC008000, 32),
            width=4,
            z3=z3,
            is_write=False,
        )
        self.assertTrue(outcome.handled)
        self.assertIn("test-bank", outcome.next_state.touched_devices)
        self.assertFalse(outcome.next_state.unsupported_predicates)

    def test_unsupported_gap_in_bank_is_inconclusive_predicate(self) -> None:
        from tools.ppc_equivalence.device_model import RegisterBankDevice

        z3 = _z3()
        device = RegisterBankDevice(
            base=0xCC008000,
            reg_width=4,
            registers=(RegisterSpec(offset=0x00, initial=0xAB),),
        )
        mmio = mmio_region(0xCC008000, 0xCC008FFF, device_id="test-bank")
        bus = build_memory_bus(AddressSpace((mmio,)), devices={"test-bank": device})
        state = initial_symbolic_bus_state(bus, z3)
        assert state is not None
        outcome = apply_symbolic_bus_access(
            state,
            address_space=bus.address_space,
            addr=z3.BitVecVal(0xCC008010, 32),
            width=4,
            z3=z3,
            is_write=False,
        )
        self.assertTrue(outcome.handled)
        self.assertTrue(outcome.next_state.unsupported_predicates)
        self.assertTrue(
            z3.is_true(z3.simplify(outcome.next_state.unsupported_predicates[0]))
        )

    def test_symbolic_ops_cfg_routes_mmio_load(self) -> None:
        from tools.ppc_equivalence.device_model import RegisterBankDevice
        from tools.ppc_equivalence.engine import _symbolic_initial

        device = RegisterBankDevice(
            base=0xCC008000,
            reg_width=4,
            registers=(RegisterSpec(offset=0x00, initial=0xAB),),
        )
        mmio = mmio_region(0xCC008000, 0xCC008FFF, device_id="test-bank")
        bus = build_memory_bus(AddressSpace((mmio,)), devices={"test-bank": device})
        ops = SymbolicOps()
        program = [
            _insn(Opcode.ADDI, (3, 0, 0xCC008000), address=0),
            _insn(Opcode.LWZ, (4, 3, 0), address=4),
            _insn(Opcode.BCLR, (20, 0, 0), address=8),
        ]
        terminals = execute_cfg(
            _symbolic_initial(ops),
            program,
            ops,
            memory_bus=bus,
        )
        self.assertEqual(len(terminals), 1)
        bus_state = terminals[0].state.symbolic_bus
        self.assertIsNotNone(bus_state)
        assert bus_state is not None
        self.assertIn("test-bank", bus_state.touched_devices)

    def test_fifo_cfg_write_emits_trace(self) -> None:
        from tools.ppc_equivalence.device_model import GxFifoStreamDevice
        from tools.ppc_equivalence.engine import _symbolic_initial

        device = GxFifoStreamDevice(base=0xCC008100, span=0x100)
        mmio = mmio_region(0xCC008100, 0xCC0081FF, device_id="gx-fifo")
        bus = build_memory_bus(AddressSpace((mmio,)), devices={"gx-fifo": device})
        ops = SymbolicOps()
        program = [
            _insn(Opcode.ADDI, (3, 0, 0xCC008100), address=0),
            _insn(Opcode.ADDI, (4, 0, 0x1234), address=4),
            _insn(Opcode.STW, (4, 3, 0), address=8),
            _insn(Opcode.BCLR, (20, 0, 0), address=12),
        ]
        terminals = execute_cfg(
            _symbolic_initial(ops),
            program,
            ops,
            memory_bus=bus,
        )
        self.assertEqual(len(terminals), 1)
        bus_state = terminals[0].state.symbolic_bus
        assert bus_state is not None
        trace = bus_state.fifo_traces["gx-fifo"]
        self.assertEqual(len(trace.writes), 1)
        self.assertEqual(trace.writes[0].width, 4)

    def test_fifo_read_rejected(self) -> None:
        from tools.ppc_equivalence.device_model import GxFifoStreamDevice
        from tools.ppc_equivalence.engine import _symbolic_initial

        device = GxFifoStreamDevice(base=0xCC008100, span=0x100)
        mmio = mmio_region(0xCC008100, 0xCC0081FF, device_id="gx-fifo")
        bus = build_memory_bus(AddressSpace((mmio,)), devices={"gx-fifo": device})
        ops = SymbolicOps()
        program = [
            _insn(Opcode.ADDI, (3, 0, 0xCC008100), address=0),
            _insn(Opcode.LWZ, (4, 3, 0), address=4),
            _insn(Opcode.BCLR, (20, 0, 0), address=8),
        ]
        terminals = execute_cfg(
            _symbolic_initial(ops),
            program,
            ops,
            memory_bus=bus,
        )
        bus_state = terminals[0].state.symbolic_bus
        assert bus_state is not None
        self.assertIn("gxfifo-read-unsupported", bus_state.rejections)

    def test_device_compare_detects_bank_mismatch(self) -> None:
        z3 = _z3()
        left_bank = initial_symbolic_register_bank(
            base=0xCC008000,
            registers=(RegisterSpec(offset=0),),
            reg_width=4,
            z3=z3,
            prefix="left",
            device_id="bank",
        )
        right_bank = left_bank.with_values({0: z3.BitVecVal(1, 32)})
        from tools.ppc_equivalence.symbolic_bus import SymbolicBusState

        left = SymbolicBusState(
            banks={"bank": left_bank},
            fifo_traces={},
            touched_devices=frozenset({"bank"}),
        )
        right = SymbolicBusState(
            banks={"bank": right_bank},
            fifo_traces={},
            touched_devices=frozenset({"bank"}),
        )
        diff = symbolic_bus_difference(left, right, z3)
        solver = z3.Solver()
        solver.add(diff)
        self.assertEqual(solver.check(), z3.sat)


class MmioObservabilityTests(unittest.TestCase):
    def test_enrich_obligation_attaches_cfg_routed_blocks(self) -> None:
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
        self.assertEqual(enriched["symbolic_mmio"], "cfg-routed")
        self.assertIn("register_bank_extensional", enriched)
        self.assertEqual(
            enriched["register_bank_extensional"]["algorithm"],
            ALGORITHM,
        )
        self.assertIn("bus_spec_sha256", enriched)
        self.assertIn("observability", enriched)

    def test_enrich_runs_unsupported_access_when_ops_provided(self) -> None:
        from tools.ppc_equivalence.device_model import RegisterBankDevice

        device = RegisterBankDevice(
            base=0xCC008000,
            reg_width=4,
            registers=(RegisterSpec(offset=0x00, initial=0xAB),),
        )
        mmio = mmio_region(0xCC008000, 0xCC008FFF, device_id="test-bank")
        bus = build_memory_bus(AddressSpace((mmio,)), devices={"test-bank": device})
        base_obligation = {"algorithm": "memory-bus-v1", "mmio": "fail-closed"}
        ops = SymbolicOps()
        enriched = enrich_memory_bus_obligation_with_symbolic_mmio(
            base_obligation,
            bus,
            ops=ops,
        )
        self.assertIn("unsupported_access", enriched)
        self.assertEqual(
            enriched["unsupported_access"]["original"]["result"],
            "not-queried",
        )


class MemoryBusObligationSchemaTests(unittest.TestCase):
    """Track A: discharged schema, vacuous path, forge fail-closed."""

    def _mmio_bus(self):
        from tools.ppc_equivalence.device_model import RegisterBankDevice

        device = RegisterBankDevice(
            base=0xCC008000,
            reg_width=4,
            registers=(RegisterSpec(offset=0x00, initial=0xAB),),
        )
        mmio = mmio_region(0xCC008000, 0xCC008FFF, device_id="test-bank")
        return build_memory_bus(AddressSpace((mmio,)), devices={"test-bank": device})

    def _vacuous_side(self, coverage_sha: str, cfg_sha: str) -> dict:
        return {
            "status": "vacuously-discharged",
            "reason": "no-unsupported-predicates",
            "cfg_trace_sha256": cfg_sha,
            "access_coverage_sha256": coverage_sha,
        }

    def _unsat_side(self, query_sha: str) -> dict:
        return {
            "result": "unsat",
            "query_sha256": query_sha,
            "solver": {"name": "z3", "version": "4.12.0", "elapsed_ms": 1.0},
            "terminals": [],
        }

    def test_positive_discharged_vacuous_shape(self) -> None:
        from tools.ppc_equivalence.memory_bus_obligations import (
            MEMORY_BUS_OBLIGATION_SCHEMA_VERSION,
            build_access_coverage_attestation,
            build_device_state_in_compare_attestation,
            build_memory_bus_obligation,
            validate_memory_bus_obligation,
        )
        from tools.ppc_equivalence.provenance import canonical_json_sha256

        bus = self._mmio_bus()
        coverage = build_access_coverage_attestation(
            attested=True,
            status="observed",
            opcode_families=["integer-load-store"],
        )
        cfg_sha = canonical_json_sha256({"kind": "cfg-trace", "terminals": []})
        observability = {
            "register_banks": {"test-bank": {"0x0": "0xab"}},
            "fifo_traces": {},
            "touches": {"original": [], "candidate": []},
            "symbolic": {
                "original": {"register_banks": {}, "fifo_traces": {}},
                "candidate": {"register_banks": {}, "fifo_traces": {}},
            },
        }
        obligation = build_memory_bus_obligation(bus)
        obligation.update(
            {
                "status": "discharged",
                "schema_version": MEMORY_BUS_OBLIGATION_SCHEMA_VERSION,
                "access_coverage": coverage,
                "unsupported_access": {
                    "original": self._vacuous_side(
                        coverage["original"]["sha256"], cfg_sha,
                    ),
                    "candidate": self._vacuous_side(
                        coverage["candidate"]["sha256"], cfg_sha,
                    ),
                },
                "register_bank_theory": {
                    "status": "present",
                    "devices": [{"device_id": "test-bank", "theory": "register-bank"}],
                },
                "fifo_theory": {"status": "none", "devices": []},
                "observability": observability,
                "device_state_in_compare": build_device_state_in_compare_attestation(
                    included=True,
                    observability=observability,
                ),
                "cfg_rejection_reasons": [],
                "loop_fifo_reject_markers": [],
            }
        )
        self.assertIsNone(validate_memory_bus_obligation(obligation))

    def test_positive_discharged_unsat_shape(self) -> None:
        from tools.ppc_equivalence.address_space import AddressSpace, Region, RegionKind
        from tools.ppc_equivalence.memory_bus import build_memory_bus
        from tools.ppc_equivalence.memory_bus_obligations import (
            MEMORY_BUS_OBLIGATION_SCHEMA_VERSION,
            build_access_coverage_attestation,
            build_memory_bus_obligation,
            validate_memory_bus_obligation,
        )
        from tools.ppc_equivalence.symbolic_bus import (
            aggregate_unsupported_access_query_sha256,
        )

        # RAM-only: no MMIO theory / device-compare required.
        ram = Region(0x80000000, 0x801FFFFF, RegionKind.RAM)
        bus = build_memory_bus(AddressSpace((ram,)))
        coverage = build_access_coverage_attestation(
            attested=True,
            status="observed",
            opcode_families=["integer-load-store"],
        )
        term_a = "a" * 64
        term_b = "b" * 64
        aggregate = aggregate_unsupported_access_query_sha256([term_a, term_b])
        unsat = {
            "result": "unsat",
            "query_sha256": aggregate,
            "solver": {"name": "z3", "version": "4.12.0", "elapsed_ms": 1.0},
            "terminals": [
                {"query_sha256": term_a, "result": "unsat"},
                {"query_sha256": term_b, "result": "unsat"},
            ],
        }
        obligation = build_memory_bus_obligation(bus)
        obligation.update(
            {
                "status": "discharged",
                "schema_version": MEMORY_BUS_OBLIGATION_SCHEMA_VERSION,
                "access_coverage": coverage,
                "unsupported_access": {
                    "original": unsat,
                    "candidate": unsat,
                },
                "register_bank_theory": {"status": "none", "devices": []},
                "fifo_theory": {"status": "none", "devices": []},
                "device_state_in_compare": {"included": False, "digest_sha256": None},
                "cfg_rejection_reasons": [],
                "loop_fifo_reject_markers": [],
            }
        )
        self.assertIsNone(validate_memory_bus_obligation(obligation))

    def test_forged_discharged_without_digests_fails(self) -> None:
        from tools.ppc_equivalence.memory_bus_obligations import (
            build_memory_bus_obligation,
            validate_memory_bus_obligation,
        )

        bus = self._mmio_bus()
        forged = build_memory_bus_obligation(bus)
        forged["status"] = "discharged"
        # Weak JSON: algorithm/regions only — no attestations.
        reason = validate_memory_bus_obligation(forged)
        self.assertIsNotNone(reason)
        self.assertTrue(
            "bus_spec" in (reason or "")
            or "unsupported_access" in (reason or "")
            or "access_coverage" in (reason or "")
            or "schema_version" in (reason or ""),
            reason,
        )

    def test_unknown_schema_fail_closed(self) -> None:
        from tools.ppc_equivalence.memory_bus_obligations import (
            build_access_coverage_attestation,
            build_memory_bus_obligation,
            validate_memory_bus_obligation,
        )
        from tools.ppc_equivalence.provenance import canonical_json_sha256

        bus = self._mmio_bus()
        coverage = build_access_coverage_attestation(
            attested=True, status="observed",
        )
        cfg_sha = canonical_json_sha256({"k": 1})
        obligation = build_memory_bus_obligation(bus)
        obligation.update(
            {
                "status": "discharged",
                "schema_version": 99,
                "access_coverage": coverage,
                "unsupported_access": {
                    "original": self._vacuous_side(
                        coverage["original"]["sha256"], cfg_sha,
                    ),
                    "candidate": self._vacuous_side(
                        coverage["candidate"]["sha256"], cfg_sha,
                    ),
                },
                "register_bank_theory": {"status": "none", "devices": []},
                "fifo_theory": {"status": "none", "devices": []},
                "device_state_in_compare": {"included": False, "digest_sha256": None},
                "cfg_rejection_reasons": [],
                "loop_fifo_reject_markers": [],
            }
        )
        reason = validate_memory_bus_obligation(obligation)
        self.assertIsNotNone(reason)
        self.assertIn("schema_version", reason or "")

    def test_vacuous_without_coverage_attestation_fails(self) -> None:
        from tools.ppc_equivalence.memory_bus_obligations import (
            MEMORY_BUS_OBLIGATION_SCHEMA_VERSION,
            build_access_coverage_attestation,
            build_memory_bus_obligation,
            validate_memory_bus_obligation,
        )
        from tools.ppc_equivalence.provenance import canonical_json_sha256

        bus = self._mmio_bus()
        coverage = build_access_coverage_attestation(
            attested=False, status="observed",
        )
        cfg_sha = canonical_json_sha256({"k": 1})
        obligation = build_memory_bus_obligation(bus)
        obligation.update(
            {
                "status": "discharged",
                "schema_version": MEMORY_BUS_OBLIGATION_SCHEMA_VERSION,
                "access_coverage": coverage,
                "unsupported_access": {
                    "original": self._vacuous_side(
                        coverage["original"]["sha256"], cfg_sha,
                    ),
                    "candidate": self._vacuous_side(
                        coverage["candidate"]["sha256"], cfg_sha,
                    ),
                },
                "register_bank_theory": {
                    "status": "present",
                    "devices": [{"device_id": "test-bank", "theory": "register-bank"}],
                },
                "fifo_theory": {"status": "none", "devices": []},
                "device_state_in_compare": {"included": True, "digest_sha256": None},
                "cfg_rejection_reasons": [],
                "loop_fifo_reject_markers": [],
            }
        )
        reason = validate_memory_bus_obligation(obligation)
        self.assertIsNotNone(reason)
        self.assertIn("attested", reason or "")

    def test_cfg_vacuous_path_via_discharge(self) -> None:
        from tools.ppc_equivalence.deadline import Deadline
        from tools.ppc_equivalence.memory_bus_obligations import (
            enrich_memory_bus_obligation_with_symbolic_mmio,
        )
        from tools.ppc_equivalence.symbolic_bus import (
            apply_symbolic_bus_access,
            initial_symbolic_bus_state,
        )

        z3 = _z3()
        bus = self._mmio_bus()
        state = initial_symbolic_bus_state(bus, z3)
        assert state is not None
        outcome = apply_symbolic_bus_access(
            state,
            address_space=bus.address_space,
            addr=z3.BitVecVal(0xCC008000, 32),
            width=4,
            z3=z3,
            is_write=False,
        )
        self.assertFalse(outcome.next_state.unsupported_predicates)

        class _Term:
            def __init__(self) -> None:
                self.state = type(
                    "S",
                    (),
                    {
                        "symbolic_bus": outcome.next_state,
                        "memory_touches": (),
                    },
                )()
                self.condition = z3.BoolVal(True)

        enriched = enrich_memory_bus_obligation_with_symbolic_mmio(
            {"algorithm": "memory-bus-v1", "regions": [
                {"kind": "mmio", "start": 0xCC008000, "end": 0xCC008FFF,
                 "device_id": "test-bank"},
            ]},
            bus,
            original_terminals=[_Term()],
            candidate_terminals=[_Term()],
            ops=SymbolicOps(),
            deadline=Deadline.after_ms(5_000),
        )
        side = enriched["unsupported_access"]["original"]
        self.assertEqual(side["status"], "vacuously-discharged")
        self.assertEqual(side["reason"], "no-unsupported-predicates")
        self.assertEqual(
            side["access_coverage_sha256"],
            enriched["access_coverage"]["original"]["sha256"],
        )
        self.assertEqual(enriched["status"], "discharged")
        from tools.ppc_equivalence.memory_bus_obligations import (
            validate_memory_bus_obligation,
        )

        self.assertIsNone(validate_memory_bus_obligation(enriched, bus=bus))


class StrictMemoryBusValidatorTests(unittest.TestCase):
    """Finding 3: recompute digests + cross-check discharged obligations."""

    def _mmio_bus(self):
        from tools.ppc_equivalence.device_model import RegisterBankDevice

        device = RegisterBankDevice(
            base=0xCC008000,
            reg_width=4,
            registers=(RegisterSpec(offset=0x00, initial=0xAB),),
        )
        mmio = mmio_region(0xCC008000, 0xCC008FFF, device_id="test-bank")
        return build_memory_bus(AddressSpace((mmio,)), devices={"test-bank": device})

    def _base_discharged(self, bus):
        from tools.ppc_equivalence.memory_bus_obligations import (
            MEMORY_BUS_OBLIGATION_SCHEMA_VERSION,
            build_access_coverage_attestation,
            build_device_state_in_compare_attestation,
            build_memory_bus_obligation,
        )
        from tools.ppc_equivalence.provenance import canonical_json_sha256

        coverage = build_access_coverage_attestation(
            attested=True,
            status="observed",
            opcode_families=["integer-load-store"],
        )
        cfg_sha = canonical_json_sha256({"kind": "cfg-trace", "terminals": []})
        observability = {
            "register_banks": {"test-bank": {"0x0": "0xab"}},
            "fifo_traces": {},
            "touches": {"original": [], "candidate": []},
            "symbolic": {
                "original": {"register_banks": {}, "fifo_traces": {}},
                "candidate": {"register_banks": {}, "fifo_traces": {}},
            },
        }
        vacuous_original = {
            "status": "vacuously-discharged",
            "reason": "no-unsupported-predicates",
            "cfg_trace_sha256": cfg_sha,
            "access_coverage_sha256": coverage["original"]["sha256"],
        }
        vacuous_candidate = {
            **vacuous_original,
            "access_coverage_sha256": coverage["candidate"]["sha256"],
        }
        obligation = build_memory_bus_obligation(bus)
        obligation.update(
            {
                "status": "discharged",
                "schema_version": MEMORY_BUS_OBLIGATION_SCHEMA_VERSION,
                "access_coverage": coverage,
                "unsupported_access": {
                    "original": vacuous_original,
                    "candidate": vacuous_candidate,
                },
                "register_bank_theory": {
                    "status": "present",
                    "devices": [{"device_id": "test-bank", "theory": "register-bank"}],
                },
                "fifo_theory": {"status": "none", "devices": []},
                "observability": observability,
                "device_state_in_compare": build_device_state_in_compare_attestation(
                    included=True,
                    observability=observability,
                ),
                "cfg_rejection_reasons": [],
                "loop_fifo_reject_markers": [],
            }
        )
        return obligation

    def test_fabricated_access_coverage_digest_rejected(self) -> None:
        from tools.ppc_equivalence.memory_bus_obligations import (
            validate_memory_bus_obligation,
        )

        bus = self._mmio_bus()
        obligation = self._base_discharged(bus)
        obligation["access_coverage"] = dict(obligation["access_coverage"])
        obligation["access_coverage"]["sha256"] = "0" * 64
        reason = validate_memory_bus_obligation(obligation)
        self.assertIsNotNone(reason)
        self.assertIn("access_coverage.sha256", reason or "")

    def test_stubbed_coverage_rejected_when_discharged(self) -> None:
        from tools.ppc_equivalence.memory_bus_obligations import (
            build_access_coverage_attestation,
            validate_memory_bus_obligation,
        )

        bus = self._mmio_bus()
        obligation = self._base_discharged(bus)
        stubbed = build_access_coverage_attestation(attested=True, status="stubbed")
        obligation["access_coverage"] = stubbed
        for side in ("original", "candidate"):
            obligation["unsupported_access"][side] = dict(
                obligation["unsupported_access"][side]
            )
            obligation["unsupported_access"][side]["access_coverage_sha256"] = (
                stubbed[side]["sha256"]
            )
        reason = validate_memory_bus_obligation(obligation)
        self.assertIsNotNone(reason)
        self.assertIn("stubbed", reason or "")

    def test_theory_none_on_mmio_register_bank_rejected(self) -> None:
        from tools.ppc_equivalence.memory_bus_obligations import (
            validate_memory_bus_obligation,
        )

        bus = self._mmio_bus()
        obligation = self._base_discharged(bus)
        obligation["register_bank_theory"] = {"status": "none", "devices": []}
        reason = validate_memory_bus_obligation(obligation)
        self.assertIsNotNone(reason)
        self.assertTrue(
            "register_bank_theory" in (reason or "")
            or "MMIO" in (reason or ""),
            reason,
        )

    def test_theory_device_id_not_in_regions_rejected(self) -> None:
        from tools.ppc_equivalence.memory_bus_obligations import (
            validate_memory_bus_obligation,
        )

        bus = self._mmio_bus()
        obligation = self._base_discharged(bus)
        obligation["register_bank_theory"] = {
            "status": "present",
            "devices": [{"device_id": "other-bank", "theory": "register-bank"}],
        }
        reason = validate_memory_bus_obligation(obligation)
        self.assertIsNotNone(reason)
        self.assertIn("device_id", reason or "")

    def test_mmio_included_false_rejected(self) -> None:
        from tools.ppc_equivalence.memory_bus_obligations import (
            validate_memory_bus_obligation,
        )

        bus = self._mmio_bus()
        obligation = self._base_discharged(bus)
        obligation["device_state_in_compare"] = {
            "included": False,
            "digest_sha256": None,
        }
        reason = validate_memory_bus_obligation(obligation)
        self.assertIsNotNone(reason)
        self.assertIn("device_state_in_compare.included", reason or "")

    def test_aggregate_query_hash_mutation_rejected(self) -> None:
        from tools.ppc_equivalence.address_space import Region, RegionKind
        from tools.ppc_equivalence.memory_bus_obligations import (
            MEMORY_BUS_OBLIGATION_SCHEMA_VERSION,
            build_access_coverage_attestation,
            build_memory_bus_obligation,
            validate_memory_bus_obligation,
        )
        from tools.ppc_equivalence.symbolic_bus import (
            aggregate_unsupported_access_query_sha256,
        )

        ram = Region(0x80000000, 0x801FFFFF, RegionKind.RAM)
        bus = build_memory_bus(AddressSpace((ram,)))
        coverage = build_access_coverage_attestation(
            attested=True, status="observed",
        )
        digests = ["1" * 64, "2" * 64]
        aggregate = aggregate_unsupported_access_query_sha256(digests)
        side = {
            "result": "unsat",
            "query_sha256": aggregate,
            "solver": {"name": "z3", "version": "4.12.0", "elapsed_ms": 1.0},
            "terminals": [
                {"query_sha256": digests[0], "result": "unsat"},
                {"query_sha256": digests[1], "result": "unsat"},
            ],
        }
        obligation = build_memory_bus_obligation(bus)
        obligation.update(
            {
                "status": "discharged",
                "schema_version": MEMORY_BUS_OBLIGATION_SCHEMA_VERSION,
                "access_coverage": coverage,
                "unsupported_access": {"original": side, "candidate": side},
                "register_bank_theory": {"status": "none", "devices": []},
                "fifo_theory": {"status": "none", "devices": []},
                "device_state_in_compare": {"included": False, "digest_sha256": None},
                "cfg_rejection_reasons": [],
                "loop_fifo_reject_markers": [],
            }
        )
        self.assertIsNone(validate_memory_bus_obligation(obligation))

        mutated = dict(obligation)
        mutated_side = dict(side)
        # Forged: top-level hash is only the last terminal (pre-fix bug).
        mutated_side["query_sha256"] = digests[-1]
        mutated["unsupported_access"] = {
            "original": mutated_side,
            "candidate": mutated_side,
        }
        reason = validate_memory_bus_obligation(mutated)
        self.assertIsNotNone(reason)
        self.assertIn("aggregate", reason or "")

    def test_engine_built_valid_discharged_still_passes(self) -> None:
        from tools.ppc_equivalence.deadline import Deadline
        from tools.ppc_equivalence.memory_bus_obligations import (
            enrich_memory_bus_obligation_with_symbolic_mmio,
            validate_memory_bus_obligation,
            validate_memory_bus_obligation_strict,
        )
        from tools.ppc_equivalence.symbolic_bus import (
            apply_symbolic_bus_access,
            initial_symbolic_bus_state,
        )

        z3 = _z3()
        bus = self._mmio_bus()
        state = initial_symbolic_bus_state(bus, z3)
        assert state is not None
        outcome = apply_symbolic_bus_access(
            state,
            address_space=bus.address_space,
            addr=z3.BitVecVal(0xCC008000, 32),
            width=4,
            z3=z3,
            is_write=False,
        )

        class _Term:
            def __init__(self) -> None:
                self.state = type(
                    "S",
                    (),
                    {
                        "symbolic_bus": outcome.next_state,
                        "memory_touches": (),
                    },
                )()
                self.condition = z3.BoolVal(True)

        enriched = enrich_memory_bus_obligation_with_symbolic_mmio(
            build_memory_bus_obligation(bus),
            bus,
            original_terminals=[_Term()],
            candidate_terminals=[_Term()],
            ops=SymbolicOps(),
            deadline=Deadline.after_ms(5_000),
        )
        self.assertEqual(enriched["status"], "discharged")
        self.assertEqual(enriched["access_coverage"]["status"], "observed")
        self.assertIsNone(validate_memory_bus_obligation(enriched, bus=bus))
        self.assertIsNone(
            validate_memory_bus_obligation_strict(
                enriched,
                bus=bus,
                original_terminals=[_Term()],
                candidate_terminals=[_Term()],
            )
        )


class FreezeGuardTests(unittest.TestCase):
    def test_memory_bus_unfrozen_for_equivalent(self) -> None:
        self.assertNotIn("memory-bus", UNSUPPORTED_FOR_EQUIVALENT)

    def test_symbolic_mmio_promotion_authorized_when_unfrozen(self) -> None:
        self.assertFalse(symbolic_mmio_still_fail_closed())


class LoopFifoAndMixedSpaceCoverageTests(unittest.TestCase):
    """Track C: loop×FIFO / mixed-space rejects must be obligation-attested."""

    def test_mixed_space_symbolic_attests_rejection(self) -> None:
        from tools.ppc_equivalence.address_space import Region, RegionKind
        from tools.ppc_equivalence.device_model import RegisterBankDevice
        from tools.ppc_equivalence.memory_bus_obligations import (
            MIXED_SPACE_REJECTION_REASON,
            build_memory_bus_obligation,
            validate_memory_bus_obligation,
        )
        from tools.ppc_equivalence.symbolic_bus import (
            apply_symbolic_bus_access,
            initial_symbolic_bus_state,
        )

        z3 = _z3()
        device = RegisterBankDevice(
            base=0xCC008000,
            reg_width=4,
            registers=(RegisterSpec(offset=0x00, initial=0xAB),),
        )
        ram = Region(0x80000000, 0x801FFFFF, RegionKind.RAM)
        mmio = mmio_region(0xCC008000, 0xCC008FFF, device_id="test-bank")
        bus = build_memory_bus(
            AddressSpace((ram, mmio)), devices={"test-bank": device},
        )
        state = initial_symbolic_bus_state(bus, z3)
        assert state is not None
        outcome = apply_symbolic_bus_access(
            state,
            address_space=bus.address_space,
            addr=z3.BitVec("mixed.addr", 32),
            width=4,
            z3=z3,
            is_write=False,
        )
        self.assertTrue(outcome.handled)
        self.assertIn(
            MIXED_SPACE_REJECTION_REASON,
            outcome.next_state.rejections,
        )

        # Minimal terminal: include memory_touches for observability collectors.
        class _State:
            def __init__(self) -> None:
                self.symbolic_bus = outcome.next_state
                self.memory_touches = ()

        class _Term:
            def __init__(self) -> None:
                self.state = _State()

        enriched = enrich_memory_bus_obligation_with_symbolic_mmio(
            build_memory_bus_obligation(bus),
            bus,
            original_terminals=[_Term()],
        )
        self.assertEqual(
            enriched["mixed_space_rejection"],
            MIXED_SPACE_REJECTION_REASON,
        )
        self.assertIn(
            MIXED_SPACE_REJECTION_REASON,
            enriched["cfg_rejection_reasons"],
        )
        self.assertEqual(enriched["mixed_space_symbolic_mmio"], "fail-closed")
        self.assertEqual(enriched["coverage"]["mixed_space"], "rejected")
        self.assertEqual(enriched["status"], "cfg-routed-rejected")
        self.assertIsNone(validate_memory_bus_obligation(enriched))

        forged = dict(enriched)
        forged["status"] = "discharged"
        # Satisfy Track A digest shape so the Track C reject gate is what fails.
        from tools.ppc_equivalence.memory_bus_obligations import (
            build_access_coverage_attestation,
            build_device_state_in_compare_attestation,
        )

        coverage = build_access_coverage_attestation(
            attested=True,
            status="observed",
            opcode_families=["integer-load-store"],
        )
        forged["access_coverage"] = coverage
        observability = forged.get("observability") or {
            "register_banks": {},
            "fifo_traces": {},
            "touches": {"original": [], "candidate": []},
            "symbolic": {
                "original": {"register_banks": {}, "fifo_traces": {}},
                "candidate": {"register_banks": {}, "fifo_traces": {}},
            },
        }
        forged["device_state_in_compare"] = build_device_state_in_compare_attestation(
            included=True,
            observability=observability,
        )
        vacuous_original = {
            "status": "vacuously-discharged",
            "reason": "no-unsupported-predicates",
            "cfg_trace_sha256": "a" * 64,
            "access_coverage_sha256": coverage["original"]["sha256"],
        }
        vacuous_candidate = {
            **vacuous_original,
            "access_coverage_sha256": coverage["candidate"]["sha256"],
        }
        forged["unsupported_access"] = {
            "original": vacuous_original,
            "candidate": vacuous_candidate,
        }
        reason = validate_memory_bus_obligation(forged)
        self.assertIsNotNone(reason)
        self.assertTrue(
            "mixed-space" in (reason or "")
            or "cfg_rejection" in (reason or ""),
            reason,
        )

    def test_pure_mmio_symbolic_still_routes(self) -> None:
        from tools.ppc_equivalence.device_model import RegisterBankDevice
        from tools.ppc_equivalence.symbolic_bus import (
            apply_symbolic_bus_access,
            initial_symbolic_bus_state,
        )

        z3 = _z3()
        device = RegisterBankDevice(
            base=0xCC008000,
            reg_width=4,
            registers=(RegisterSpec(offset=0x00, initial=0xAB),),
        )
        mmio = mmio_region(0xCC008000, 0xCC008FFF, device_id="test-bank")
        bus = build_memory_bus(AddressSpace((mmio,)), devices={"test-bank": device})
        state = initial_symbolic_bus_state(bus, z3)
        assert state is not None
        addr = z3.BitVec("pure.mmio.addr", 32)
        outcome = apply_symbolic_bus_access(
            state,
            address_space=bus.address_space,
            addr=addr,
            width=4,
            z3=z3,
            is_write=False,
        )
        self.assertTrue(outcome.handled)
        self.assertNotIn(
            "symbolic-mmio-mixed-address-space",
            outcome.next_state.rejections,
        )
        # Supported when addr equals the declared register.
        solver = z3.Solver()
        solver.add(addr == z3.BitVecVal(0xCC008000, 32))
        solver.add(z3.Not(outcome.next_state.unsupported_predicates[0]))
        self.assertEqual(solver.check(), z3.sat)

    def test_cfg_loop_summary_plus_fifo_inconclusive_with_obligation(self) -> None:
        from tools.ppc_equivalence.contract import EquivalenceContract, parse_observables
        from tools.ppc_equivalence.device_model import GxFifoStreamDevice
        from tools.ppc_equivalence.engine import _symbolic_initial, check_equivalence
        from tools.ppc_equivalence.ir import ExecutionInconclusive, Instruction, Opcode
        from tools.ppc_equivalence.loop_summary import build_affine_summary_map
        from tools.ppc_equivalence.memory_bus_obligations import (
            LOOP_FIFO_POLICY,
            LOOP_FIFO_REJECTION_REASON,
            validate_memory_bus_obligation,
        )
        from tools.ppc_equivalence.result import ProofStatus

        def _insn(
            opcode: Opcode, operands: tuple[int, ...], *, address: int = 0,
        ) -> Instruction:
            return Instruction(address, 0, opcode, operands)

        # CTR-affine counted loop (same pattern as test_loop_summary).
        program = [
            _insn(Opcode.ADDI, (0, 0, 4), address=0),
            _insn(Opcode.MTSPR, (0, 9), address=4),
            _insn(Opcode.ADDI, (3, 3, 1), address=8),
            _insn(Opcode.BC, (16, 0, 8, 0), address=12),
            _insn(Opcode.BCLR, (20, 0, 0), address=16),
        ]
        summaries = build_affine_summary_map(program)
        self.assertTrue(summaries, "expected affine summary map for counted loop")

        device = GxFifoStreamDevice(base=0xCC008100, span=0x100)
        mmio = mmio_region(0xCC008100, 0xCC0081FF, device_id="gx-fifo")
        bus = build_memory_bus(AddressSpace((mmio,)), devices={"gx-fifo": device})
        ops = SymbolicOps()
        with self.assertRaisesRegex(ExecutionInconclusive, "symbolic-loop-fifo-emission"):
            execute_cfg(
                _symbolic_initial(ops),
                program,
                ops,
                memory_bus=bus,
                affine_loop_summaries=summaries,
            )

        contract = EquivalenceContract(parse_observables(["r3"]), timeout_ms=5_000)
        result = check_equivalence(
            program,
            program,
            contract,
            original_hex="00",
            candidate_hex="00",
            memory_bus=bus,
            max_loop_iterations=2,
        )
        self.assertEqual(result.status, ProofStatus.INCONCLUSIVE_UNSUPPORTED)
        self.assertIn("memory-bus", result.proof_features)
        self.assertIsNotNone(result.memory_bus)
        assert result.memory_bus is not None
        self.assertEqual(result.memory_bus["loop_fifo_policy"], LOOP_FIFO_POLICY)
        self.assertIn(
            LOOP_FIFO_REJECTION_REASON,
            result.memory_bus.get("cfg_rejection_reasons")
            or result.memory_bus.get("cfg_rejections")
            or [],
        )
        self.assertIn(
            LOOP_FIFO_REJECTION_REASON,
            result.memory_bus.get("loop_fifo_reject_markers") or [],
        )
        self.assertEqual(result.memory_bus["status"], "cfg-routed-rejected")
        self.assertIsNone(validate_memory_bus_obligation(result.memory_bus))
        forged = dict(result.memory_bus)
        forged["status"] = "discharged"
        self.assertIsNotNone(validate_memory_bus_obligation(forged))


if __name__ == "__main__":
    unittest.main()
