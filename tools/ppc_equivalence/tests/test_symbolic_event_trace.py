"""Unit tests for bounded GX FIFO symbolic event trace scaffold (PR 15)."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.bus_spec import DeviceSpecification, lift_symbolic_fifo_traces
from tools.ppc_equivalence.symbolic_event_trace import (
    ALGORITHM,
    DEFAULT_MAX_FIFO_EVENTS,
    UnboundedEmissionStatus,
    initial_symbolic_event_trace,
    reject_unbounded_symbolic_loop_fifo_emission,
    symbolic_trace_from_device_spec,
)


def _z3():
    import z3

    return z3


class SymbolicEventTraceTests(unittest.TestCase):
    def test_append_write_and_compare_equal(self) -> None:
        z3 = _z3()
        left = initial_symbolic_event_trace(
            device_id="gx-fifo",
            base=0xCC008100,
            span=0x100,
            max_events=4,
        )
        right = initial_symbolic_event_trace(
            device_id="gx-fifo",
            base=0xCC008100,
            span=0x100,
            max_events=4,
        )
        addr = z3.BitVecVal(0xCC008100, 32)
        value = z3.BitVecVal(0x12345678, 32)
        left_next, err = left.append_write(addr, 4, value)
        assert left_next is not None and err is None
        right_next, err = right.append_write(addr, 4, value)
        assert right_next is not None and err is None
        result = left_next.compare_equal(right_next, z3)
        self.assertTrue(result.supported)
        solver = z3.Solver()
        solver.add(result.equal)
        self.assertEqual(solver.check(), z3.sat)

    def test_compare_detects_value_mismatch(self) -> None:
        z3 = _z3()
        trace = initial_symbolic_event_trace(
            device_id="gx-fifo",
            base=0xCC008100,
            span=0x100,
            max_events=2,
        )
        addr = z3.BitVecVal(0xCC008100, 32)
        left, _ = trace.append_write(addr, 4, z3.BitVecVal(1, 32))
        right, _ = trace.append_write(addr, 4, z3.BitVecVal(2, 32))
        assert left is not None and right is not None
        result = left.compare_equal(right, z3)
        solver = z3.Solver()
        solver.add(result.equal)
        self.assertEqual(solver.check(), z3.unsat)

    def test_bounded_append_rejects_overflow(self) -> None:
        trace = initial_symbolic_event_trace(
            device_id="gx-fifo",
            base=0xCC008100,
            span=0x100,
            max_events=1,
        )
        next_trace, err = trace.append_write(0xCC008100, 4, 0x11)
        self.assertIsNotNone(next_trace)
        overflow, err2 = next_trace.append_write(0xCC008104, 4, 0x22)  # type: ignore[union-attr]
        self.assertIsNone(overflow)
        self.assertEqual(err2, "bounded-event-limit")

    def test_reads_remain_unsupported(self) -> None:
        self.assertFalse(initial_symbolic_event_trace(
            device_id="gx-fifo",
            base=0xCC008100,
            span=0x100,
        ).read_supported())

    def test_reject_unbounded_symbolic_loop_emission(self) -> None:
        z3 = _z3()
        trip = z3.BitVec("trip", 32)
        status, detail = reject_unbounded_symbolic_loop_fifo_emission(
            trip_count=trip,
            writes_per_trip=1,
            max_events=DEFAULT_MAX_FIFO_EVENTS,
            z3=z3,
        )
        self.assertEqual(status, UnboundedEmissionStatus.REJECTED_SYMBOLIC_TRIP)
        self.assertEqual(detail, "symbolic-loop-fifo-emission")

    def test_allow_concrete_loop_within_bound(self) -> None:
        z3 = _z3()
        status, detail = reject_unbounded_symbolic_loop_fifo_emission(
            trip_count=z3.BitVecVal(8, 32),
            writes_per_trip=2,
            max_events=32,
            z3=z3,
        )
        self.assertEqual(status, UnboundedEmissionStatus.ALLOWED)
        self.assertIsNone(detail)

    def test_reject_concrete_loop_exceeding_bound(self) -> None:
        z3 = _z3()
        status, detail = reject_unbounded_symbolic_loop_fifo_emission(
            trip_count=z3.BitVecVal(100, 32),
            writes_per_trip=4,
            max_events=32,
            z3=z3,
        )
        self.assertEqual(status, UnboundedEmissionStatus.REJECTED_BOUND_EXCEEDED)
        self.assertIn("projected-writes", detail or "")

    def test_lift_hook_from_bus_specification(self) -> None:
        from tools.ppc_equivalence.address_space import AddressSpace

        device = DeviceSpecification(
            device_id="gx-fifo",
            theory="gxfifo-stream",
            base=0xCC008100,
            span=0x100,
        )
        from tools.ppc_equivalence.bus_spec import BusSpecification

        spec = BusSpecification(address_space=AddressSpace(()), devices=(device,))
        traces = lift_symbolic_fifo_traces(spec)
        self.assertEqual(set(traces), {"gx-fifo"})
        self.assertEqual(traces["gx-fifo"].max_events, DEFAULT_MAX_FIFO_EVENTS)

    def test_obligation_block_algorithm(self) -> None:
        device = DeviceSpecification(
            device_id="gx-fifo",
            theory="gxfifo-stream",
            base=0xCC008100,
            span=0x100,
        )
        trace = symbolic_trace_from_device_spec(device, max_events=8)
        self.assertEqual(trace.device_id, "gx-fifo")
        self.assertEqual(trace.span, 0x100)
        self.assertEqual(ALGORITHM, "gxfifo-stream-trace-v1")

    def test_enrich_attaches_gxfifo_trace_block(self) -> None:
        from tools.ppc_equivalence.address_space import AddressSpace, mmio_region
        from tools.ppc_equivalence.device_model import GxFifoStreamDevice
        from tools.ppc_equivalence.memory_bus import build_memory_bus
        from tools.ppc_equivalence.memory_bus_obligations import (
            LOOP_FIFO_EMISSION,
            LOOP_FIFO_POLICY,
            enrich_memory_bus_obligation_with_symbolic_mmio,
        )

        device = GxFifoStreamDevice(base=0xCC008100, span=0x100)
        mmio = mmio_region(0xCC008100, 0xCC0081FF, device_id="gx-fifo")
        bus = build_memory_bus(AddressSpace((mmio,)), devices={"gx-fifo": device})
        enriched = enrich_memory_bus_obligation_with_symbolic_mmio(
            {"algorithm": "memory-bus-v1"},
            bus,
        )
        self.assertIn("gxfifo_trace", enriched)
        self.assertEqual(enriched["gxfifo_trace"]["algorithm"], ALGORITHM)
        self.assertEqual(enriched["gxfifo_trace"]["reads"], "unsupported")
        self.assertEqual(enriched["gxfifo_trace"]["status"], "cfg-routed")
        self.assertEqual(enriched["loop_fifo_policy"], LOOP_FIFO_POLICY)
        self.assertEqual(enriched["loop_fifo_emission"], LOOP_FIFO_EMISSION)
        self.assertEqual(
            enriched["gxfifo_trace"]["bounded_summarized_emission"],
            LOOP_FIFO_EMISSION,
        )

    def test_loop_summary_with_fifo_attests_hard_reject(self) -> None:
        from tools.ppc_equivalence.address_space import AddressSpace, mmio_region
        from tools.ppc_equivalence.device_model import GxFifoStreamDevice
        from tools.ppc_equivalence.memory_bus import build_memory_bus
        from tools.ppc_equivalence.memory_bus_obligations import (
            LOOP_FIFO_EMISSION,
            LOOP_FIFO_POLICY,
            LOOP_FIFO_REJECTION_REASON,
            build_memory_bus_obligation,
            enrich_memory_bus_obligation_with_symbolic_mmio,
            validate_memory_bus_obligation,
        )

        device = GxFifoStreamDevice(base=0xCC008100, span=0x100)
        mmio = mmio_region(0xCC008100, 0xCC0081FF, device_id="gx-fifo")
        bus = build_memory_bus(AddressSpace((mmio,)), devices={"gx-fifo": device})
        enriched = enrich_memory_bus_obligation_with_symbolic_mmio(
            build_memory_bus_obligation(bus),
            bus,
            loop_summaries_active=True,
        )
        self.assertEqual(enriched["symbolic_mmio"], "cfg-routed-rejected")
        self.assertIn(LOOP_FIFO_REJECTION_REASON, enriched["cfg_rejections"])
        self.assertIn(LOOP_FIFO_REJECTION_REASON, enriched["cfg_rejection_reasons"])
        self.assertEqual(enriched["loop_fifo_policy"], LOOP_FIFO_POLICY)
        self.assertEqual(enriched["loop_fifo_emission"], LOOP_FIFO_EMISSION)
        self.assertIn(LOOP_FIFO_REJECTION_REASON, enriched["loop_fifo_reject_markers"])
        self.assertEqual(enriched["status"], "cfg-routed-rejected")
        self.assertEqual(
            enriched["loop_fifo_rejection"]["reason"],
            LOOP_FIFO_REJECTION_REASON,
        )
        self.assertEqual(enriched["coverage"]["loop_fifo"], "hard-rejected")
        self.assertIsNone(validate_memory_bus_obligation(enriched))

        forged = dict(enriched)
        forged["status"] = "discharged"
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
            "FIFO" in (reason or "")
            or "loop" in (reason or "")
            or "cfg_rejection" in (reason or "")
            or "loop_fifo" in (reason or "")
            or "×" in (reason or ""),
            reason,
        )


if __name__ == "__main__":
    unittest.main()
