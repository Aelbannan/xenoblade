"""Wave 3 MMIO capability-assurance foundations."""

from __future__ import annotations

import copy
import importlib.util
import unittest

from tools.ppc_equivalence.capability_assurance import (
    CAPABILITY_ASSURANCE_POLICY,
    CAPABILITY_ASSURANCE_SCHEMA_VERSION,
    STATUS_INCOMPLETE,
    STATUS_PROMOTION_GRADE,
    STATUS_UNMODELED,
    CapabilityAssurance,
    CapabilityManifest,
    attestation_digest,
    build_attestation,
    compute_confidence_tier_from_assurance,
    evaluate_capability_assurance,
    infer_used_capabilities,
)
from tools.ppc_equivalence.external_event import (
    DeterministicReadOracle,
    ExternalEventCursor,
    SymbolicReadOracle,
    shared_external_read_ok,
)
from tools.ppc_equivalence.hardware_profile import (
    SOURCE_AD_HOC_BUS,
    SOURCE_HARDWARE_PROFILE,
    compute_hardware_profile_sha256,
    is_reviewed_hardware_profile,
    load_hardware_profile,
)
from tools.ppc_equivalence.mmio_capability_obligations import (
    GX_FIFO_READ_ALGORITHM,
    GX_FIFO_READ_CAPABILITY,
    GX_FIFO_READ_MODEL_VERSION,
    GX_FIFO_WRITE_TRACE_CAPABILITY,
    MMIO_REGISTER_BANK_ALGORITHM,
    MMIO_REGISTER_BANK_CAPABILITY,
    MMIO_REGISTER_BANK_MODEL_VERSION,
    build_mmio_attestation,
    build_mmio_capability_obligation,
    infer_mmio_capabilities_from_memory_bus,
    obligation_is_promotion_grade,
    recompute_mmio_attestation_status,
    validate_mmio_capability_obligation,
)
from tools.ppc_equivalence.result import (
    ARCHITECTURE_MODEL,
    MASKING_SEMANTICS,
    RESULT_FORMAT,
    MemoryScope,
    PrivateStackInfo,
    ProofResult,
    ProofStatus,
)

_QUERY = "a" * 64
_BUS = "b" * 64
_HAS_Z3 = importlib.util.find_spec("z3") is not None


def _ledger(*opcodes: str, capabilities: dict | None = None):
    from tools.coop.lib.equivalence_policy import ValidationLedger

    return ValidationLedger(
        frozenset(opcodes),
        intentionally_loaded=True,
        capabilities=dict(capabilities or {}),
    )


def _equivalent(**kwargs) -> ProofResult:
    defaults = dict(
        status=ProofStatus.EQUIVALENT,
        architecture_model=ARCHITECTURE_MODEL,
        format=RESULT_FORMAT,
        observables=["r3"],
        engine_hash="a" * 64,
        certifier_hash="d" * 64,
        source_hash="b" * 64,
        git_commit="c" * 40,
        opcodes_used=["lwz", "blr"],
        memory_scope=MemoryScope(
            masking_semantics=MASKING_SEMANTICS,
            original=PrivateStackInfo(enabled_on_all_terminal_paths=True),
            candidate=PrivateStackInfo(enabled_on_all_terminal_paths=True),
        ),
    )
    defaults.update(kwargs)
    return ProofResult(**defaults)


def _manifest(**kwargs) -> CapabilityManifest:
    defaults = dict(
        allowed_tier_a_capabilities={
            "integer-core": ("integer-core-v1",),
            "provenance": ("provenance-v1",),
            # Empty until canary — Wave 3 default.
            "mmio-register-bank": (),
            "gx-fifo-write-trace": (),
            "gx-fifo-read": (),
        },
        shadow_mode=True,
    )
    defaults.update(kwargs)
    return CapabilityManifest(**defaults)


def _provenance_attestation():
    return build_attestation(
        capability="provenance",
        model_version="provenance-v1",
        algorithm="provenance-binding-v1",
        status=STATUS_INCOMPLETE,
        evidence={
            "engine_hash": "a" * 64,
            "certifier_hash": "d" * 64,
            "source_hash": "b" * 64,
            "git_commit": "c" * 40,
            "git_dirty": False,
        },
    )


def _integer_attestation(*opcodes: str):
    return build_attestation(
        capability="integer-core",
        model_version="integer-core-v1",
        algorithm="opcode-ledger-v2",
        status=STATUS_INCOMPLETE,
        evidence={"opcodes": list(opcodes), "ledger_sha256": "e" * 64},
    )


def _complete_sides() -> tuple[dict, dict]:
    side = {
        "address_coverage": {"result": "unsat", "query_sha256": _QUERY},
        "unsupported_access": {"result": "unsat", "query_sha256": _QUERY},
        "event_trace_complete": {"result": "unsat", "query_sha256": _QUERY},
    }
    return copy.deepcopy(side), copy.deepcopy(side)


class HardwareProfileTests(unittest.TestCase):
    def test_reviewed_profile_loads_and_hashes(self) -> None:
        profile = load_hardware_profile("wii-broadway-xenoblade-us-v1")
        self.assertEqual(profile["profile"], "wii-broadway-xenoblade-us-v1")
        self.assertTrue(is_reviewed_hardware_profile(profile))
        self.assertEqual(
            profile["profile_sha256"],
            compute_hardware_profile_sha256(profile),
        )
        self.assertTrue(any(r["kind"] == "ram" for r in profile["regions"]))
        self.assertTrue(any(r["kind"] == "mmio" for r in profile["regions"]))
        self.assertTrue(profile["devices"])
        self.assertTrue(profile["register_specs"])

    def test_profile_hash_mismatch_fail_closed(self) -> None:
        profile = load_hardware_profile("wii-broadway-xenoblade-us-v1")
        mutated = dict(profile)
        mutated["profile_sha256"] = "0" * 64
        self.assertFalse(is_reviewed_hardware_profile(mutated))
        expected = compute_hardware_profile_sha256(profile)
        self.assertNotEqual(mutated["profile_sha256"], expected)


class ObligationSchemaTests(unittest.TestCase):
    def test_ad_hoc_bus_map_not_promotion_grade(self) -> None:
        original, candidate = _complete_sides()
        obligation = build_mmio_capability_obligation(
            ad_hoc=True,
            bus_spec_sha256=_BUS,
            original=original,
            candidate=candidate,
            status="promotion-grade",
        )
        self.assertEqual(obligation["source"], SOURCE_AD_HOC_BUS)
        self.assertIsNotNone(
            validate_mmio_capability_obligation(
                obligation, require_promotion_shape=True
            )
        )
        self.assertFalse(obligation_is_promotion_grade(obligation))
        status = recompute_mmio_attestation_status(
            {"mmio": obligation},
            capability=MMIO_REGISTER_BANK_CAPABILITY,
            algorithm=MMIO_REGISTER_BANK_ALGORITHM,
            model_version=MMIO_REGISTER_BANK_MODEL_VERSION,
            allowed_versions=(MMIO_REGISTER_BANK_MODEL_VERSION,),
        )
        self.assertEqual(status, STATUS_INCOMPLETE)

    def test_reviewed_profile_hash_mismatch_fail_closed(self) -> None:
        profile = load_hardware_profile("wii-broadway-xenoblade-us-v1")
        original, candidate = _complete_sides()
        obligation = build_mmio_capability_obligation(
            hardware_profile=profile,
            bus_spec_sha256=_BUS,
            original=original,
            candidate=candidate,
            status="promotion-grade",
        )
        obligation["hardware_profile_sha256"] = "f" * 64
        reason = validate_mmio_capability_obligation(
            obligation, require_promotion_shape=True
        )
        self.assertIsNotNone(reason)
        self.assertIn("hardware_profile_sha256", reason or "")

    def test_malformed_obligation_fail_closed(self) -> None:
        reason = validate_mmio_capability_obligation({"capability": "mmio-register-bank"})
        self.assertIsNotNone(reason)
        status = recompute_mmio_attestation_status(
            {"mmio": {"capability": "mmio-register-bank"}},
            capability=MMIO_REGISTER_BANK_CAPABILITY,
            algorithm=MMIO_REGISTER_BANK_ALGORITHM,
            model_version=MMIO_REGISTER_BANK_MODEL_VERSION,
            allowed_versions=(MMIO_REGISTER_BANK_MODEL_VERSION,),
        )
        self.assertEqual(status, STATUS_INCOMPLETE)

    def test_hardware_profile_sha256_mutation_not_promotion_grade(self) -> None:
        profile = load_hardware_profile("wii-broadway-xenoblade-us-v1")
        original, candidate = _complete_sides()
        obligation = build_mmio_capability_obligation(
            hardware_profile=profile,
            bus_spec_sha256=_BUS,
            original=original,
            candidate=candidate,
            status="promotion-grade",
        )
        # Even with allowlist, mutated digest fails closed.
        obligation["hardware_profile_sha256"] = "1" * 64
        self.assertFalse(obligation_is_promotion_grade(obligation))
        status = recompute_mmio_attestation_status(
            {"mmio": obligation},
            capability=MMIO_REGISTER_BANK_CAPABILITY,
            algorithm=MMIO_REGISTER_BANK_ALGORITHM,
            model_version=MMIO_REGISTER_BANK_MODEL_VERSION,
            allowed_versions=(MMIO_REGISTER_BANK_MODEL_VERSION,),
        )
        self.assertEqual(status, STATUS_INCOMPLETE)

    def test_empty_allowlist_keeps_complete_obligation_incomplete(self) -> None:
        profile = load_hardware_profile("wii-broadway-xenoblade-us-v1")
        original, candidate = _complete_sides()
        obligation = build_mmio_capability_obligation(
            hardware_profile=profile,
            bus_spec_sha256=_BUS,
            original=original,
            candidate=candidate,
            status="promotion-grade",
        )
        self.assertEqual(obligation["source"], SOURCE_HARDWARE_PROFILE)
        self.assertTrue(obligation_is_promotion_grade(obligation))
        # Default empty allowlist → incomplete grade.
        status = recompute_mmio_attestation_status(
            {"mmio": obligation},
            capability=MMIO_REGISTER_BANK_CAPABILITY,
            algorithm=MMIO_REGISTER_BANK_ALGORITHM,
            model_version=MMIO_REGISTER_BANK_MODEL_VERSION,
            allowed_versions=(),
        )
        self.assertEqual(status, STATUS_INCOMPLETE)


class GxFifoReadTests(unittest.TestCase):
    def test_gx_fifo_read_remains_incomplete(self) -> None:
        original, candidate = _complete_sides()
        obligation = build_mmio_capability_obligation(
            capability=GX_FIFO_READ_CAPABILITY,
            model_version=GX_FIFO_READ_MODEL_VERSION,
            algorithm=GX_FIFO_READ_ALGORITHM,
            hardware_profile="wii-broadway-xenoblade-us-v1",
            bus_spec_sha256=_BUS,
            original=original,
            candidate=candidate,
            status="promotion-grade",
        )
        status = recompute_mmio_attestation_status(
            {"mmio": obligation},
            capability=GX_FIFO_READ_CAPABILITY,
            algorithm=GX_FIFO_READ_ALGORITHM,
            model_version=GX_FIFO_READ_MODEL_VERSION,
            allowed_versions=(GX_FIFO_READ_MODEL_VERSION,),
        )
        self.assertEqual(status, STATUS_INCOMPLETE)

        caps = infer_mmio_capabilities_from_memory_bus(
            {
                "fifo_theory": {"status": "present"},
                "gx_fifo_read": True,
                "bus_spec_canonical": {
                    "devices": [{"theory": "gxfifo-stream", "device_id": "gx"}]
                },
            },
            proof_features=["memory-bus"],
        )
        self.assertIn(GX_FIFO_READ_CAPABILITY, caps)
        self.assertIn(GX_FIFO_WRITE_TRACE_CAPABILITY, caps)


class CertificateRoundTripTests(unittest.TestCase):
    def test_mmio_attestation_certificate_round_trip(self) -> None:
        profile = load_hardware_profile("wii-broadway-xenoblade-us-v1")
        original, candidate = _complete_sides()
        obligation = build_mmio_capability_obligation(
            hardware_profile=profile,
            bus_spec_sha256=_BUS,
            original=original,
            candidate=candidate,
            status="incomplete",
        )
        attestation = build_mmio_attestation(obligation)
        assurance = CapabilityAssurance(
            schema_version=CAPABILITY_ASSURANCE_SCHEMA_VERSION,
            policy=CAPABILITY_ASSURANCE_POLICY,
            capabilities=(
                _integer_attestation("lwz", "blr"),
                attestation,
                _provenance_attestation(),
            ),
        )
        restored = CapabilityAssurance.from_dict(assurance.to_dict())
        restored.validate_structure()
        self.assertEqual(restored.to_dict(), assurance.to_dict())
        mmio = next(
            item for item in restored.capabilities if item.capability == "mmio-register-bank"
        )
        self.assertEqual(mmio.attestation_sha256, attestation_digest(mmio))
        self.assertEqual(mmio.algorithm, MMIO_REGISTER_BANK_ALGORITHM)
        self.assertEqual(
            mmio.evidence["hardware_profile_sha256"],
            profile["profile_sha256"],
        )

    def test_assurance_eval_mmio_incomplete_under_empty_allowlist(self) -> None:
        profile = load_hardware_profile("wii-broadway-xenoblade-us-v1")
        original, candidate = _complete_sides()
        obligation = build_mmio_capability_obligation(
            hardware_profile=profile,
            bus_spec_sha256=_BUS,
            original=original,
            candidate=candidate,
            status="promotion-grade",
        )
        attestation = build_mmio_attestation(obligation)
        # Forge caller status; evaluator must recompute incomplete.
        forged = build_attestation(
            capability=attestation.capability,
            model_version=attestation.model_version,
            algorithm=attestation.algorithm,
            status=STATUS_PROMOTION_GRADE,
            evidence=dict(attestation.evidence),
        )
        proof = _equivalent(
            proof_features=["memory-bus"],
            memory_bus={"schema_version": 2, "algorithm": "memory-bus-v1"},
            capability_assurance=CapabilityAssurance(
                capabilities=(
                    _integer_attestation("lwz", "blr"),
                    forged,
                    _provenance_attestation(),
                ),
            ).to_dict(),
        )
        used = infer_used_capabilities(proof)
        self.assertIn("mmio-register-bank", used)
        result = evaluate_capability_assurance(proof, _ledger("lwz", "blr"), _manifest())
        self.assertEqual(
            result.recomputed_statuses.get("mmio-register-bank"),
            STATUS_INCOMPLETE,
        )
        self.assertEqual(compute_confidence_tier_from_assurance(result), "C")

    def test_fifo_write_inference_split(self) -> None:
        proof = _equivalent(
            proof_features=["memory-bus"],
            memory_bus={
                "schema_version": 2,
                "fifo_theory": {"status": "present"},
                "bus_spec_canonical": {
                    "devices": [
                        {"device_id": "gx-fifo", "theory": "gxfifo-stream"},
                    ]
                },
            },
            capability_assurance=CapabilityAssurance(
                capabilities=(
                    _integer_attestation("stw"),
                    _provenance_attestation(),
                ),
            ).to_dict(),
            opcodes_used=["stw"],
        )
        used = infer_used_capabilities(proof)
        self.assertIn(GX_FIFO_WRITE_TRACE_CAPABILITY, used)
        result = evaluate_capability_assurance(proof, _ledger("stw"), _manifest())
        self.assertEqual(
            result.recomputed_statuses.get(GX_FIFO_WRITE_TRACE_CAPABILITY),
            STATUS_UNMODELED,
        )
        self.assertEqual(compute_confidence_tier_from_assurance(result), "C")


class ReadOracleTests(unittest.TestCase):
    def test_deterministic_oracle_and_sharing_rule(self) -> None:
        oracle = DeterministicReadOracle(values={("pi-stub", 0): 0x11, ("pi-stub", 1): 0x22})
        cursor = ExternalEventCursor()
        idx0 = cursor.next_index("pi-stub")
        self.assertEqual(oracle.read("pi-stub", idx0, {"regs": {}}), 0x11)
        idx1 = cursor.next_index("pi-stub")
        self.assertEqual(oracle.read("pi-stub", idx1), 0x22)
        self.assertTrue(
            shared_external_read_ok(
                original_device_id="pi-stub",
                candidate_device_id="pi-stub",
                original_event_index=0,
                candidate_event_index=0,
                original_device_state={"regs": {}},
                candidate_device_state={"regs": {}},
            )
        )
        self.assertFalse(
            shared_external_read_ok(
                original_device_id="pi-stub",
                candidate_device_id="pi-stub",
                original_event_index=0,
                candidate_event_index=1,
                original_device_state={},
                candidate_device_state={},
            )
        )

    @unittest.skipUnless(_HAS_Z3, "z3-solver is not installed")
    def test_symbolic_oracle_distinct_events(self) -> None:
        import z3

        oracle = SymbolicReadOracle()
        a = oracle.read("pi-stub", 0)
        b = oracle.read("pi-stub", 1)
        self.assertIsInstance(a, z3.BitVecRef)
        self.assertFalse(z3.eq(a, b))
        self.assertTrue(z3.eq(a, oracle.read("pi-stub", 0)))


class ArchitectureFreezeTests(unittest.TestCase):
    def test_architecture_tracks_live_model(self) -> None:
        # Live architecture tracks parent bumps (currently v40 / cert15).
        self.assertEqual(ARCHITECTURE_MODEL, "broadway-ppc32-be-v41")
        self.assertTrue(ARCHITECTURE_MODEL.startswith("broadway-ppc32-be-v"))


if __name__ == "__main__":
    unittest.main()
