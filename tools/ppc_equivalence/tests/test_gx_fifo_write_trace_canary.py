"""Authoritative GX FIFO Tier-A ``gx-fifo-write-trace`` canary.

Canary target: ``us-8031bfc0`` / ``GXInvalidateVtxCache`` (leaf FULL_MATCH,
single ``stb`` to WGPIPE at ``0xCC008000`` via
``lis r3,0xcc01; li r0,0x48; stb r0,-0x8000(r3); blr``). Real retail bytes:
``3C60CC01 38000048 98038000 4E800020``.

Proves the pre-allowlist wiring end to end under the reviewed
``wii-broadway-xenoblade-us-v2`` hardware profile:

* ``build_memory_bus_from_hardware_profile`` materializes a non-ad-hoc
  ``MemoryBus`` whose identity (``hardware_profile_name`` /
  ``hardware_profile_sha256`` / ``device_models_sha256``) survives into
  ``check_equivalence(..., memory_bus=)``'s ``result.memory_bus`` obligation.
* Self-vs-self is ``EQUIVALENT`` with the memory-bus discharged and a
  ``gxfifo_trace`` block present.
* A ``gx-fifo-write-trace`` obligation built from that reviewed bus is
  ``promotion-grade`` only once ``gx-fifo-trace-v2`` is in the allowlist
  (``recompute_mmio_attestation_status``) — never with an empty allowlist.
* A byte-value mismatch between original/candidate is ``NOT_EQUIVALENT``.
* The same shape built from an ad-hoc bus (no reviewed profile) is never
  promotion-grade, even with complete UNSAT sides and an allowlisted version.

This file does **not** mutate ``capability_manifest.json`` (shadow default).
The authoritative manifest may graduate GX allowlists after the canary
(see ``gx_fifo_v1_rollout.check_authoritative_gx_graduation``).
"""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.address_space import AddressSpace, Region, RegionKind, mmio_region
from tools.ppc_equivalence.capability_assurance import STATUS_INCOMPLETE, STATUS_PROMOTION_GRADE
from tools.ppc_equivalence.contract import EquivalenceContract, parse_observables
from tools.ppc_equivalence.decoder import decode_block
from tools.ppc_equivalence.device_model import GxFifoStreamDevice
from tools.ppc_equivalence.engine import check_equivalence
from tools.ppc_equivalence.hardware_profile import SOURCE_AD_HOC_BUS, SOURCE_HARDWARE_PROFILE
from tools.ppc_equivalence.memory_bus import build_memory_bus, build_memory_bus_from_hardware_profile
from tools.ppc_equivalence.mmio_capability_obligations import (
    GX_FIFO_TRACE_ALGORITHM,
    GX_FIFO_TRACE_MODEL_VERSION,
    GX_FIFO_WRITE_TRACE_CAPABILITY,
    build_mmio_capability_obligation,
    obligation_is_promotion_grade,
    recompute_mmio_attestation_status,
)
from tools.ppc_equivalence.result import ProofStatus

_HARDWARE_PROFILE = "wii-broadway-xenoblade-us-v2"

# Real retail bytes for GXInvalidateVtxCache (us-8031bfc0):
#   3C60CC01  addis r3, r0, 0xCC01     ; lis r3, 0xCC01
#   38000048  addi  r0, r0, 0x0048     ; li r0, 0x48 (FIFO write-trace command byte)
#   98038000  stb   r0, -0x8000(r3)    ; WGPIPE == 0xCC01_0000 - 0x8000 == 0xCC00_8000
#   4E800020  blr
_ORIGINAL_HEX = "3C60CC0138000048980380004E800020"
# Same shape, differing FIFO command byte (0x48 -> 0x49): a value mismatch a
# reviewed-profile GX FIFO write-trace proof must still catch.
_MISMATCH_HEX = "3C60CC0138000049980380004E800020"
_TARGET_ADDRESS = 0x8031BFC0

_QUERY = "a" * 64
_ADHOC_BUS_SPEC = "b" * 64


def _decode(hex_bytes: str):
    return decode_block(bytes.fromhex(hex_bytes), _TARGET_ADDRESS, validate_with_capstone=False)


def _contract() -> EquivalenceContract:
    return EquivalenceContract(parse_observables(["r0", "r3"]), timeout_ms=15_000)


def _complete_sides() -> tuple[dict, dict]:
    side = {
        "address_coverage": {"result": "unsat", "query_sha256": _QUERY},
        "unsupported_access": {"result": "unsat", "query_sha256": _QUERY},
        "event_trace_complete": {"result": "unsat", "query_sha256": _QUERY},
    }
    return dict(side), dict(side)


def _ad_hoc_bus():
    """A GX FIFO bus with no reviewed hardware-profile identity attached."""
    device = GxFifoStreamDevice(base=0xCC008000, span=0x80, device_id="gx-fifo")
    mmio = mmio_region(0xCC008000, 0xCC00807F, device_id="gx-fifo")
    ram = Region(0x80000000, 0x817FFFFF, RegionKind.RAM)
    return build_memory_bus(AddressSpace((ram, mmio)), devices={"gx-fifo": device})


class ReviewedProfileBusTests(unittest.TestCase):
    def test_bus_from_hardware_profile_is_not_ad_hoc(self) -> None:
        bus = build_memory_bus_from_hardware_profile(_HARDWARE_PROFILE)
        self.assertEqual(bus.hardware_profile_name, _HARDWARE_PROFILE)
        self.assertIsNotNone(bus.hardware_profile_sha256)
        self.assertIsNotNone(bus.device_models_sha256)

    def test_self_vs_self_equivalent_with_memory_bus_discharged(self) -> None:
        bus = build_memory_bus_from_hardware_profile(_HARDWARE_PROFILE)
        program = _decode(_ORIGINAL_HEX)
        result = check_equivalence(
            program,
            program,
            _contract(),
            original_hex=_ORIGINAL_HEX,
            candidate_hex=_ORIGINAL_HEX,
            memory_bus=bus,
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT, result.unsupported)
        self.assertIn("memory-bus", result.proof_features)
        mb = result.memory_bus
        self.assertIsInstance(mb, dict)
        self.assertEqual(mb.get("status"), "discharged")
        self.assertEqual(mb.get("hardware_profile"), _HARDWARE_PROFILE)
        self.assertIsNotNone(mb.get("hardware_profile_sha256"))
        self.assertIsNotNone(mb.get("device_models_sha256"))
        # GX FIFO write-trace evidence is present for the write-trace
        # obligation to be built from.
        self.assertIn("gxfifo_trace", mb)
        self.assertIsNotNone(mb.get("bus_spec_sha256"))

    def test_value_mismatch_is_not_equivalent(self) -> None:
        bus = build_memory_bus_from_hardware_profile(_HARDWARE_PROFILE)
        original = _decode(_ORIGINAL_HEX)
        candidate = _decode(_MISMATCH_HEX)
        result = check_equivalence(
            original,
            candidate,
            _contract(),
            original_hex=_ORIGINAL_HEX,
            candidate_hex=_MISMATCH_HEX,
            memory_bus=bus,
        )
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT, result.unsupported)


class GxFifoWriteTraceObligationTests(unittest.TestCase):
    """Manually-built ``gx-fifo-write-trace`` obligations (mirrors
    ``test_mmio_capability_assurance``'s ``_complete_sides`` pattern) prove
    the promotion-grade gate is allowlist-driven, not automatic."""

    def test_reviewed_profile_obligation_is_promotion_grade_shape(self) -> None:
        bus = build_memory_bus_from_hardware_profile(_HARDWARE_PROFILE)
        program = _decode(_ORIGINAL_HEX)
        result = check_equivalence(
            program,
            program,
            _contract(),
            original_hex=_ORIGINAL_HEX,
            candidate_hex=_ORIGINAL_HEX,
            memory_bus=bus,
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT)
        bus_spec_sha256 = result.memory_bus.get("bus_spec_sha256")
        self.assertIsInstance(bus_spec_sha256, str)

        original_side, candidate_side = _complete_sides()
        obligation = build_mmio_capability_obligation(
            capability=GX_FIFO_WRITE_TRACE_CAPABILITY,
            model_version=GX_FIFO_TRACE_MODEL_VERSION,
            algorithm=GX_FIFO_TRACE_ALGORITHM,
            hardware_profile=_HARDWARE_PROFILE,
            bus_spec_sha256=bus_spec_sha256,
            original=original_side,
            candidate=candidate_side,
            status="promotion-grade",
        )
        self.assertEqual(obligation["source"], SOURCE_HARDWARE_PROFILE)
        self.assertTrue(obligation_is_promotion_grade(obligation))

    def test_promotion_grade_status_requires_allowlisted_model_version(self) -> None:
        original_side, candidate_side = _complete_sides()
        obligation = build_mmio_capability_obligation(
            capability=GX_FIFO_WRITE_TRACE_CAPABILITY,
            model_version=GX_FIFO_TRACE_MODEL_VERSION,
            algorithm=GX_FIFO_TRACE_ALGORITHM,
            hardware_profile=_HARDWARE_PROFILE,
            bus_spec_sha256=_QUERY,
            original=original_side,
            candidate=candidate_side,
            status="promotion-grade",
        )
        self.assertTrue(obligation_is_promotion_grade(obligation))

        allowed_status = recompute_mmio_attestation_status(
            {"mmio": obligation},
            capability=GX_FIFO_WRITE_TRACE_CAPABILITY,
            algorithm=GX_FIFO_TRACE_ALGORITHM,
            model_version=GX_FIFO_TRACE_MODEL_VERSION,
            allowed_versions=(GX_FIFO_TRACE_MODEL_VERSION,),
        )
        self.assertEqual(allowed_status, STATUS_PROMOTION_GRADE)

        # Empty allowlist (shadow-default capability_manifest.json):
        # structurally promotable obligation still reports incomplete.
        empty_allowlist_status = recompute_mmio_attestation_status(
            {"mmio": obligation},
            capability=GX_FIFO_WRITE_TRACE_CAPABILITY,
            algorithm=GX_FIFO_TRACE_ALGORITHM,
            model_version=GX_FIFO_TRACE_MODEL_VERSION,
            allowed_versions=(),
        )
        self.assertEqual(empty_allowlist_status, STATUS_INCOMPLETE)

    def test_ad_hoc_bus_never_promotion_grade_even_when_allowlisted(self) -> None:
        # Real ad-hoc GX FIFO bus (no reviewed profile) still self-vs-self
        # proves EQUIVALENT, but its memory_bus obligation carries no
        # hardware-profile identity.
        bus = _ad_hoc_bus()
        program = _decode(_ORIGINAL_HEX)
        result = check_equivalence(
            program,
            program,
            _contract(),
            original_hex=_ORIGINAL_HEX,
            candidate_hex=_ORIGINAL_HEX,
            memory_bus=bus,
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT)
        self.assertIsNone(result.memory_bus.get("hardware_profile"))
        self.assertIsNone(result.memory_bus.get("hardware_profile_sha256"))

        original_side, candidate_side = _complete_sides()
        obligation = build_mmio_capability_obligation(
            capability=GX_FIFO_WRITE_TRACE_CAPABILITY,
            model_version=GX_FIFO_TRACE_MODEL_VERSION,
            algorithm=GX_FIFO_TRACE_ALGORITHM,
            ad_hoc=True,
            bus_spec_sha256=_ADHOC_BUS_SPEC,
            original=original_side,
            candidate=candidate_side,
            status="promotion-grade",
        )
        self.assertEqual(obligation["source"], SOURCE_AD_HOC_BUS)
        self.assertFalse(obligation_is_promotion_grade(obligation))

        status = recompute_mmio_attestation_status(
            {"mmio": obligation},
            capability=GX_FIFO_WRITE_TRACE_CAPABILITY,
            algorithm=GX_FIFO_TRACE_ALGORITHM,
            model_version=GX_FIFO_TRACE_MODEL_VERSION,
            allowed_versions=(GX_FIFO_TRACE_MODEL_VERSION,),
        )
        self.assertEqual(status, STATUS_INCOMPLETE)


if __name__ == "__main__":
    unittest.main()
