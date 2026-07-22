"""Integration tests: GX FIFO loop plans wired into ``execute_cfg``.

Covers the targeted loop×FIFO routing that replaced the old blanket
"any loop summary + any FIFO device" rejection (see ``semantics.py``
``execute_cfg`` and ``gx_fifo_loop.unresolved_fifo_touching_memory_loop_headers``):

1. A ``MemoryLoopPlan`` whose footprint is provably disjoint from every GX
   FIFO region no longer raises just because a FIFO device is declared.
2. A ``MemoryLoopPlan`` whose footprint cannot be proven disjoint (moving
   base resolves into the GX region) and has no matching recognized
   ``GxFifoLoopPlan`` at the same header still hard-rejects.
3. A recognized ``GxFifoLoopPlan`` applies its closed form and appends a
   ``RepeatedEmission`` to the bounded symbolic trace, but recognition alone
   never authorizes equivalence — the entry-guard premise terminal (a real
   proof obligation) is still present in the returned terminal list.
"""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.address_space import AddressSpace, Region, RegionKind, mmio_region
from tools.ppc_equivalence.device_model import GxFifoStreamDevice
from tools.ppc_equivalence.engine import _symbolic_initial
from tools.ppc_equivalence.gx_fifo_loop import (
    DEFAULT_DEVICE_ID,
    DEFAULT_GX_BASE,
    DEFAULT_GX_SPAN,
    build_gx_fifo_loop_plans,
)
from tools.ppc_equivalence.ir import ExecutionInconclusive, Instruction, Opcode
from tools.ppc_equivalence.memory_bus import build_memory_bus
from tools.ppc_equivalence.memory_loop import build_memory_loop_plan_map
from tools.ppc_equivalence.semantics import SymbolicOps, execute_cfg
from tools.ppc_equivalence.symbolic_event_trace import RepeatedEmission
from tools.ppc_equivalence.tests.test_gx_fifo_loop import _gx_fifo_program


def _insn(opcode: Opcode, operands: tuple[int, ...], *, address: int = 0) -> Instruction:
    return Instruction(address, 0, opcode, operands)


def _gx_device_bus(*, extra_ram: bool = False):
    device = GxFifoStreamDevice(base=DEFAULT_GX_BASE, span=DEFAULT_GX_SPAN)
    mmio = mmio_region(
        DEFAULT_GX_BASE, DEFAULT_GX_BASE + DEFAULT_GX_SPAN - 1, device_id=DEFAULT_DEVICE_ID,
    )
    regions: tuple = (mmio,)
    if extra_ram:
        regions = (Region(0x80000000, 0x801FFFFF, RegionKind.RAM), mmio)
    return build_memory_bus(AddressSpace(regions), devices={DEFAULT_DEVICE_ID: device})


class NonFifoMemoryLoopCoexistsWithFifoDeviceTests(unittest.TestCase):
    """Requirement 1: a non-FIFO-touching MemoryLoopPlan is no longer rejected
    solely because a GX FIFO device is declared on the same bus."""

    def _ram_stride_store_program(self) -> list[Instruction]:
        # base=0x80001000 (RAM); trip=4; header: stw r3,0(r5); addi r5,r5,4; bdnz.
        return [
            _insn(Opcode.ADDIS, (5, 0, 0x8000), address=0),
            _insn(Opcode.ORI, (5, 5, 0x1000), address=4),
            _insn(Opcode.ADDI, (0, 0, 4), address=8),
            _insn(Opcode.MTSPR, (0, 9), address=12),
            _insn(Opcode.STW, (3, 5, 0), address=16),
            _insn(Opcode.ADDI, (5, 5, 4), address=20),
            _insn(Opcode.BC, (16, 0, 16, 0), address=24),
            _insn(Opcode.BCLR, (20, 0, 0), address=28),
        ]

    def test_non_fifo_memory_loop_plan_does_not_raise(self) -> None:
        program = self._ram_stride_store_program()
        bus = _gx_device_bus(extra_ram=True)
        memory_plans = build_memory_loop_plan_map(program)
        self.assertTrue(memory_plans, "expected a recognized constant-stride store loop")

        ops = SymbolicOps()
        memory_plans_used: list = []
        terminals = execute_cfg(
            _symbolic_initial(ops),
            program,
            ops,
            memory_bus=bus,
            memory_loop_plans=memory_plans,
            memory_plans_used=memory_plans_used,
        )
        self.assertTrue(terminals)
        self.assertTrue(memory_plans_used, "expected the closed form to actually apply")


class UnrecognizedFifoTouchingMemoryLoopStillRejectsTests(unittest.TestCase):
    """Requirement 2: a moving-base MemoryLoopPlan whose footprint resolves
    into the declared GX FIFO region — and has no matching GxFifoLoopPlan at
    the same header (the addi increments the *base*, not the GX fifo shape's
    fixed-base/affine-source shape) — must still hard-reject."""

    def _gx_region_stride_store_program(self) -> list[Instruction]:
        # base starts at the GX device's own base and moves by the store
        # width every iteration — this is the memory_loop.py "d-form-addi"
        # shape (addi increments the base register), not the gx_fifo_loop.py
        # shape (fixed base, affine addi on the *source* register).
        return [
            _insn(Opcode.ADDIS, (5, 0, (DEFAULT_GX_BASE >> 16) & 0xFFFF), address=0),
            _insn(Opcode.ORI, (5, 5, DEFAULT_GX_BASE & 0xFFFF), address=4),
            _insn(Opcode.ADDI, (0, 0, 4), address=8),
            _insn(Opcode.MTSPR, (0, 9), address=12),
            _insn(Opcode.STW, (3, 5, 0), address=16),
            _insn(Opcode.ADDI, (5, 5, 4), address=20),
            _insn(Opcode.BC, (16, 0, 16, 0), address=24),
            _insn(Opcode.BCLR, (20, 0, 0), address=28),
        ]

    def test_unrecognized_fifo_touching_loop_raises(self) -> None:
        program = self._gx_region_stride_store_program()
        bus = _gx_device_bus()
        memory_plans = build_memory_loop_plan_map(program)
        self.assertTrue(memory_plans, "expected a recognized constant-stride store loop")
        # Confirm the GX-shaped recognizer does *not* also claim this header
        # (mutually exclusive body shapes — addi targets base, not source).
        gx_plans = build_gx_fifo_loop_plans(program)
        self.assertEqual(gx_plans, {})

        ops = SymbolicOps()
        with self.assertRaisesRegex(ExecutionInconclusive, "symbolic-loop-fifo-emission"):
            execute_cfg(
                _symbolic_initial(ops),
                program,
                ops,
                memory_bus=bus,
                memory_loop_plans=memory_plans,
            )


class RecognizedGxFifoLoopPlanAppliesWithoutAuthorizingTests(unittest.TestCase):
    """Requirement 3: a recognized GxFifoLoopPlan applies its closed form
    (RepeatedEmission appended to the bounded symbolic trace) but recognition
    alone never authorizes equivalence — the entry-guard premise terminal
    (a real, still-open proof obligation) is present in the result."""

    def test_recognized_plan_applies_and_leaves_entry_guard_obligation_open(self) -> None:
        program = _gx_fifo_program(count=4, store=(Opcode.STW, 3))
        bus = _gx_device_bus()
        plans = build_gx_fifo_loop_plans(program)
        self.assertEqual(len(plans), 1)

        ops = SymbolicOps()
        used: list = []
        terminals = execute_cfg(
            _symbolic_initial(ops),
            program,
            ops,
            memory_bus=bus,
            gx_fifo_loop_plans=plans,
            gx_fifo_plans_used=used,
        )
        # The plan was actually discharged mechanically (not skipped)...
        self.assertEqual(len(used), 1)

        # ...but execute_cfg only returns Terminals, never a proof verdict —
        # recognition/application is not itself an equivalence certificate.
        # The negated entry-guard premise is kept as its own terminal exactly
        # like a memory-loop summary, i.e. a real obligation the caller's
        # solver must still discharge (see GX_FIFO_TIER_A.md "Authorization").
        premise_terminals = [t for t in terminals if t.exit_kind == "gx-fifo-loop-entry-premise"]
        self.assertEqual(len(premise_terminals), 1)

        # The successful path's final state carries the summarized emission
        # in the bounded symbolic FIFO trace (not silently dropped).
        applied = [t for t in terminals if t.exit_kind != "gx-fifo-loop-entry-premise"]
        self.assertEqual(len(applied), 1)
        bus_state = applied[0].state.symbolic_bus
        assert bus_state is not None
        trace = bus_state.fifo_traces[DEFAULT_DEVICE_ID]
        self.assertEqual(len(trace.events), 1)
        segment = trace.events[0]
        self.assertIsInstance(segment, RepeatedEmission)
        self.assertEqual(segment.count, 4)
        self.assertEqual(segment.writes_per_iteration, 1)


if __name__ == "__main__":
    unittest.main()
