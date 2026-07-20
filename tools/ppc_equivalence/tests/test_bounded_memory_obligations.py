"""Wave 2 bounded-memory address-coverage / wraparound obligations."""

from __future__ import annotations

import copy
import unittest
from types import SimpleNamespace
from typing import Any

import z3

from tools.ppc_equivalence.bounded_memory_obligations import (
    BOUNDED_MEMORY_ATTESTATION_ALGORITHM,
    BOUNDED_MEMORY_MODEL_VERSION,
    PLATFORM_PROFILE_PROOF_REQUEST_FIELD,
    MemoryAccessObligation,
    SOURCE_AD_HOC,
    SOURCE_INCOMPLETE,
    SOURCE_PLATFORM_PROFILE,
    access_outside_all_ranges,
    access_wraps_32bit,
    build_bounded_memory_attestation,
    build_bounded_memory_obligation,
    build_bounded_memory_obligation_from_terminals,
    load_platform_profile,
    obligation_is_promotion_grade,
    recompute_bounded_memory_attestation_status,
    validate_bounded_memory_obligation,
)
from tools.ppc_equivalence.capability_assurance import (
    STATUS_INCOMPLETE,
    STATUS_PROMOTION_GRADE,
    STATUS_SCOPED_ASSUMPTION,
    CapabilityAssurance,
    CapabilityManifest,
    evaluate_capability_assurance,
)
from tools.ppc_equivalence.deadline import Deadline
from tools.ppc_equivalence.memory_profile import MemoryEnvironment, MemoryProfile
from tools.ppc_equivalence.result import (
    ARCHITECTURE_MODEL,
    MASKING_SEMANTICS,
    RESULT_FORMAT,
    MemoryScope,
    PrivateStackInfo,
    ProofResult,
    ProofStatus,
)

MEM1 = (0x80000000, 0x817FFFFF)


def _deadline() -> Deadline:
    return Deadline.after_ms(5_000)


def _access(
    addr: Any,
    width: int,
    *,
    path: Any | None = None,
    is_write: bool = False,
    label: str | None = None,
) -> MemoryAccessObligation:
    return MemoryAccessObligation(
        addr=addr,
        width=width,
        path_condition=z3.BoolVal(True) if path is None else path,
        is_write=is_write,
        label=label,
    )


class PredicateHelpers(unittest.TestCase):
    def test_outside_symbolic_inside_mem1(self) -> None:
        addr = z3.BitVec("addr", 32)
        outside = access_outside_all_ranges(addr, 1, [MEM1])
        solver = z3.Solver()
        solver.add(addr == 0x80400000, outside)
        self.assertEqual(solver.check(), z3.unsat)

    def test_outside_symbolic_outside_mem1(self) -> None:
        addr = z3.BitVec("addr", 32)
        outside = access_outside_all_ranges(addr, 1, [MEM1])
        solver = z3.Solver()
        solver.add(addr == 0x70000000, outside)
        self.assertEqual(solver.check(), z3.sat)

    def test_last_byte_boundary_width_2(self) -> None:
        addr = z3.BitVec("addr", 32)
        # Last valid start for width-2 is end-1 = 0x817ffffe.
        outside = access_outside_all_ranges(addr, 2, [MEM1])
        ok = z3.Solver()
        ok.add(addr == 0x817FFFFE, outside)
        self.assertEqual(ok.check(), z3.unsat)
        bad = z3.Solver()
        bad.add(addr == 0x817FFFFF, outside)
        self.assertEqual(bad.check(), z3.sat)

    def test_last_byte_boundary_width_4(self) -> None:
        addr = z3.BitVec("addr", 32)
        outside = access_outside_all_ranges(addr, 4, [MEM1])
        ok = z3.Solver()
        ok.add(addr == 0x817FFFFC, outside)
        self.assertEqual(ok.check(), z3.unsat)
        bad = z3.Solver()
        bad.add(addr == 0x817FFFFD, outside)
        self.assertEqual(bad.check(), z3.sat)

    def test_wrap_at_ffffffff(self) -> None:
        addr = z3.BitVec("addr", 32)
        wraps = access_wraps_32bit(addr, 2)
        solver = z3.Solver()
        solver.add(addr == 0xFFFFFFFF, wraps)
        self.assertEqual(solver.check(), z3.sat)
        no_wrap = z3.Solver()
        no_wrap.add(addr == 0xFFFFFFFE, wraps)
        self.assertEqual(no_wrap.check(), z3.unsat)


class ObligationDischargeTests(unittest.TestCase):
    def test_platform_profile_in_range_is_promotion_grade(self) -> None:
        profile = load_platform_profile("xenoblade-us-retail-v1")
        addr = z3.BitVec("p_addr", 32)
        path = z3.And(z3.UGE(addr, MEM1[0]), z3.ULE(addr, MEM1[1]))
        access = _access(addr, 1, path=path, label="in-mem1")
        obligation = build_bounded_memory_obligation(
            original_accesses=[access],
            candidate_accesses=[access],
            platform_profile=profile,
            deadline=_deadline(),
            z3=z3,
        )
        self.assertIsNone(validate_bounded_memory_obligation(obligation))
        self.assertEqual(obligation["source"], SOURCE_PLATFORM_PROFILE)
        self.assertEqual(obligation["original"]["address_coverage"]["result"], "unsat")
        self.assertEqual(obligation["original"]["wraparound"]["result"], "unsat")
        self.assertTrue(obligation_is_promotion_grade(obligation))

    def test_symbolic_outside_path_is_sat(self) -> None:
        profile = load_platform_profile("xenoblade-us-retail-v1")
        addr = z3.BitVec("out_addr", 32)
        # Unconstrained path: solver can pick an outside address.
        access = _access(addr, 1, path=z3.BoolVal(True), label="free")
        obligation = build_bounded_memory_obligation(
            original_accesses=[access],
            candidate_accesses=[access],
            platform_profile=profile,
            deadline=_deadline(),
            z3=z3,
        )
        self.assertEqual(obligation["original"]["address_coverage"]["result"], "sat")
        self.assertFalse(obligation_is_promotion_grade(obligation))
        self.assertEqual(obligation["status"], "incomplete")

    def test_wrap_query_detects_ffffffff(self) -> None:
        profile = load_platform_profile("xenoblade-us-retail-v1")
        addr = z3.BitVecVal(0xFFFFFFFF, 32)
        access = _access(addr, 4, path=z3.BoolVal(True), label="wrap")
        obligation = build_bounded_memory_obligation(
            original_accesses=[access],
            candidate_accesses=[access],
            platform_profile=profile,
            deadline=_deadline(),
            z3=z3,
        )
        self.assertEqual(obligation["original"]["wraparound"]["result"], "sat")
        self.assertFalse(obligation_is_promotion_grade(obligation))

    def test_rom_write_rejection(self) -> None:
        regions = [
            {"kind": "ram", "start": "0x80000000", "end": "0x817fffff"},
            {"kind": "rom", "start": "0x82000000", "end": "0x8200ffff"},
        ]
        # Build a one-off reviewed-shaped profile that is NOT in the reviewed set
        # so we only assert rom_write discharge, not promotion-grade.
        addr = z3.BitVecVal(0x82000100, 32)
        access = _access(addr, 4, path=z3.BoolVal(True), is_write=True, label="rom-store")
        obligation = build_bounded_memory_obligation(
            original_accesses=[access],
            candidate_accesses=[access],
            ranges=[MEM1, (0x82000000, 0x8200FFFF)],
            regions=regions,
            deadline=_deadline(),
            z3=z3,
        )
        self.assertEqual(obligation["source"], SOURCE_AD_HOC)
        self.assertIn("rom_write", obligation["original"])
        self.assertEqual(obligation["original"]["rom_write"]["result"], "sat")

    def test_rom_write_unsat_when_store_in_ram(self) -> None:
        regions = [
            {"kind": "ram", "start": "0x80000000", "end": "0x817fffff"},
            {"kind": "rom", "start": "0x82000000", "end": "0x8200ffff"},
        ]
        addr = z3.BitVecVal(0x80400000, 32)
        access = _access(addr, 4, path=z3.BoolVal(True), is_write=True, label="ram-store")
        obligation = build_bounded_memory_obligation(
            original_accesses=[access],
            candidate_accesses=[access],
            ranges=[MEM1],
            regions=regions,
            deadline=_deadline(),
            z3=z3,
        )
        self.assertEqual(obligation["original"]["rom_write"]["result"], "unsat")

    def test_mutation_removing_coverage_terminal_fails(self) -> None:
        profile = load_platform_profile("xenoblade-us-retail-v1")
        a = z3.BitVec("a0", 32)
        b = z3.BitVec("a1", 32)
        path_a = z3.And(z3.UGE(a, MEM1[0]), z3.ULE(a, MEM1[1]))
        path_b = z3.And(z3.UGE(b, MEM1[0]), z3.ULE(b, MEM1[1]))
        accesses = [
            _access(a, 1, path=path_a, label="t0"),
            _access(b, 1, path=path_b, label="t1"),
        ]
        obligation = build_bounded_memory_obligation(
            original_accesses=accesses,
            candidate_accesses=accesses,
            platform_profile=profile,
            deadline=_deadline(),
            z3=z3,
        )
        self.assertIsNone(validate_bounded_memory_obligation(obligation))
        mutated = copy.deepcopy(obligation)
        terminals = mutated["original"]["address_coverage"]["terminals"]
        self.assertGreaterEqual(len(terminals), 2)
        terminals.pop()
        reason = validate_bounded_memory_obligation(mutated)
        self.assertIsNotNone(reason)
        self.assertIn("query_sha256", reason or "")

    def test_ad_hoc_ranges_not_promotion_grade(self) -> None:
        addr = z3.BitVec("adhoc", 32)
        path = z3.And(z3.UGE(addr, MEM1[0]), z3.ULE(addr, MEM1[1]))
        access = _access(addr, 1, path=path)
        env = MemoryEnvironment(
            profile=MemoryProfile.BOUNDED_ORDINARY_RAM,
            ranges=[MEM1],
        )
        obligation = build_bounded_memory_obligation(
            original_accesses=[access],
            candidate_accesses=[access],
            environment=env,
            deadline=_deadline(),
            z3=z3,
        )
        self.assertEqual(obligation["source"], SOURCE_AD_HOC)
        self.assertEqual(obligation["original"]["address_coverage"]["result"], "unsat")
        self.assertFalse(obligation_is_promotion_grade(obligation))
        self.assertNotEqual(obligation["status"], "promotion-grade")
        status = recompute_bounded_memory_attestation_status(
            {"bounded_memory": obligation},
            model_version=BOUNDED_MEMORY_MODEL_VERSION,
            allowed_versions=(BOUNDED_MEMORY_MODEL_VERSION,),
        )
        self.assertEqual(status, STATUS_SCOPED_ASSUMPTION)


class CapabilityAttestationTests(unittest.TestCase):
    def _proof(self, **kwargs: Any) -> ProofResult:
        defaults = dict(
            status=ProofStatus.EQUIVALENT,
            architecture_model=ARCHITECTURE_MODEL,
            format=RESULT_FORMAT,
            observables=["memory"],
            engine_hash="a" * 64,
            source_hash="b" * 64,
            git_commit="c" * 40,
            opcodes_used=["lwz", "blr"],
            memory_scope=MemoryScope(
                masking_semantics=MASKING_SEMANTICS,
                original=PrivateStackInfo(enabled_on_all_terminal_paths=True),
                candidate=PrivateStackInfo(enabled_on_all_terminal_paths=True),
            ),
            environment=MemoryEnvironment(
                profile=MemoryProfile.BOUNDED_ORDINARY_RAM,
                ranges=[MEM1],
            ),
        )
        defaults.update(kwargs)
        return ProofResult(**defaults)

    def test_platform_attestation_promotion_grade_under_shadow(self) -> None:
        profile = load_platform_profile("xenoblade-us-retail-v1")
        addr = z3.BitVec("cap_addr", 32)
        path = z3.And(z3.UGE(addr, MEM1[0]), z3.ULE(addr, MEM1[1]))
        access = _access(addr, 1, path=path)
        obligation = build_bounded_memory_obligation(
            original_accesses=[access],
            candidate_accesses=[access],
            platform_profile=profile,
            deadline=_deadline(),
            z3=z3,
        )
        attestation = build_bounded_memory_attestation(obligation)
        self.assertEqual(attestation.algorithm, BOUNDED_MEMORY_ATTESTATION_ALGORITHM)
        assurance = CapabilityAssurance(capabilities=(attestation,))
        proof = self._proof(capability_assurance=assurance.to_dict())
        result = evaluate_capability_assurance(
            proof,
            ledger=None,
            manifest=CapabilityManifest(
                allowed_tier_a_capabilities={
                    "bounded-memory": (BOUNDED_MEMORY_MODEL_VERSION,),
                    "integer-core": ("integer-core-v1",),
                },
                shadow_mode=True,
            ),
        )
        self.assertEqual(
            result.recomputed_statuses["bounded-memory"],
            STATUS_PROMOTION_GRADE,
        )

    def test_assumed_ordinary_ram_never_promotion_grade(self) -> None:
        attestation = build_bounded_memory_attestation(
            {
                "schema_version": 2,
                "capability": "bounded-memory",
                "model_version": BOUNDED_MEMORY_MODEL_VERSION,
                "algorithm": "bounded-memory-v2",
                "status": "promotion-grade",
                "source": "assumed-ordinary-ram",
                "address_space_sha256": "d" * 64,
                "original": {
                    "address_coverage": {"result": "unsat", "query_sha256": "e" * 64},
                    "wraparound": {"result": "unsat", "query_sha256": "f" * 64},
                },
                "candidate": {
                    "address_coverage": {"result": "unsat", "query_sha256": "e" * 64},
                    "wraparound": {"result": "unsat", "query_sha256": "f" * 64},
                },
            }
        )
        # Forged assumed-RAM obligation must not validate as promotion-grade.
        status = recompute_bounded_memory_attestation_status(
            attestation.evidence,
            assumptions=("assumed-ordinary-ram",),
            model_version=BOUNDED_MEMORY_MODEL_VERSION,
            allowed_versions=(BOUNDED_MEMORY_MODEL_VERSION,),
        )
        self.assertNotEqual(status, STATUS_PROMOTION_GRADE)

    def test_forged_promotion_without_platform_rejected(self) -> None:
        forged = {
            "schema_version": 2,
            "capability": "bounded-memory",
            "model_version": BOUNDED_MEMORY_MODEL_VERSION,
            "algorithm": "bounded-memory-v2",
            "status": "promotion-grade",
            "source": SOURCE_AD_HOC,
            "address_space_sha256": "a" * 64,
            "original": {
                "address_coverage": {
                    "result": "unsat",
                    "query_sha256": "b" * 64,
                    "status": "vacuously-discharged",
                    "terminals": [],
                },
                "wraparound": {
                    "result": "unsat",
                    "query_sha256": "c" * 64,
                    "status": "vacuously-discharged",
                    "terminals": [],
                },
            },
            "candidate": {
                "address_coverage": {
                    "result": "unsat",
                    "query_sha256": "b" * 64,
                    "status": "vacuously-discharged",
                    "terminals": [],
                },
                "wraparound": {
                    "result": "unsat",
                    "query_sha256": "c" * 64,
                    "status": "vacuously-discharged",
                    "terminals": [],
                },
            },
        }
        self.assertIsNotNone(validate_bounded_memory_obligation(forged))
        status = recompute_bounded_memory_attestation_status(
            {"bounded_memory": forged},
            model_version=BOUNDED_MEMORY_MODEL_VERSION,
            allowed_versions=(BOUNDED_MEMORY_MODEL_VERSION,),
        )
        self.assertEqual(status, STATUS_INCOMPLETE)


class TerminalLiftTests(unittest.TestCase):
    def test_accesses_from_terminal_memory_touches(self) -> None:
        from tools.ppc_equivalence.bounded_memory_obligations import (
            accesses_from_terminals,
        )

        addr = z3.BitVec("touch", 32)
        terminal = SimpleNamespace(
            condition=z3.BoolVal(True),
            state=SimpleNamespace(
                memory_touches=(addr,),
                memory_writes=(),
            ),
        )
        accesses = accesses_from_terminals([terminal])
        self.assertEqual(len(accesses), 1)
        self.assertEqual(accesses[0].width, 1)
        self.assertFalse(accesses[0].is_write)


class Stage3AProductionIntegrationTests(unittest.TestCase):
    """Stage 3A: from_terminals + reviewed profile + fail-closed grades."""

    def _lwz_terminal(self, *, in_range: bool = True) -> Any:
        # Model lwz as a 4-byte load; path constrains base into MEM1 when in_range.
        addr = z3.BitVec("lwz_addr", 32)
        if in_range:
            # Width-4 last valid start is MEM1[1]-3.
            path = z3.And(
                z3.UGE(addr, MEM1[0]),
                z3.ULE(addr, MEM1[1] - 3),
            )
        else:
            path = z3.BoolVal(True)
        # Per-byte touches for the word load.
        touches = tuple(addr + z3.BitVecVal(i, 32) for i in range(4))
        return SimpleNamespace(
            condition=path,
            state=SimpleNamespace(memory_touches=touches, memory_writes=()),
        )

    def test_lwz_under_reviewed_profile_address_coverage_unsat(self) -> None:
        profile = load_platform_profile("xenoblade-us-retail-v1")
        terminal = self._lwz_terminal(in_range=True)
        obligation = build_bounded_memory_obligation_from_terminals(
            [terminal],
            [terminal],
            platform_profile=profile,
            deadline=_deadline(),
            z3=z3,
        )
        self.assertEqual(obligation["source"], SOURCE_PLATFORM_PROFILE)
        self.assertEqual(obligation["original"]["address_coverage"]["result"], "unsat")
        self.assertEqual(obligation["candidate"]["address_coverage"]["result"], "unsat")
        self.assertEqual(obligation["original"]["wraparound"]["result"], "unsat")
        self.assertTrue(obligation_is_promotion_grade(obligation))
        self.assertIsNone(validate_bounded_memory_obligation(obligation))

    def test_missing_profile_is_incomplete(self) -> None:
        terminal = self._lwz_terminal(in_range=True)
        obligation = build_bounded_memory_obligation_from_terminals(
            [terminal],
            [terminal],
            platform_profile=None,
            deadline=_deadline(),
            z3=z3,
        )
        self.assertEqual(obligation["source"], SOURCE_INCOMPLETE)
        self.assertEqual(obligation["status"], "incomplete")
        self.assertFalse(obligation_is_promotion_grade(obligation))

        missing = build_bounded_memory_obligation_from_terminals(
            [terminal],
            [terminal],
            platform_profile="does-not-exist-profile-v0",
            deadline=_deadline(),
            z3=z3,
        )
        self.assertEqual(missing["status"], "incomplete")
        self.assertFalse(obligation_is_promotion_grade(missing))

    def test_mutation_dropping_coverage_not_promotion_grade(self) -> None:
        profile = load_platform_profile("xenoblade-us-retail-v1")
        terminal = self._lwz_terminal(in_range=True)
        obligation = build_bounded_memory_obligation_from_terminals(
            [terminal],
            [terminal],
            platform_profile=profile,
            deadline=_deadline(),
            z3=z3,
        )
        self.assertTrue(obligation_is_promotion_grade(obligation))
        mutated = copy.deepcopy(obligation)
        terminals = mutated["original"]["address_coverage"]["terminals"]
        self.assertGreaterEqual(len(terminals), 1)
        terminals.pop()
        self.assertIsNotNone(validate_bounded_memory_obligation(mutated))
        self.assertFalse(obligation_is_promotion_grade(mutated))

    def test_ad_hoc_ranges_not_promotion_grade(self) -> None:
        terminal = self._lwz_terminal(in_range=True)
        env = MemoryEnvironment(
            profile=MemoryProfile.BOUNDED_ORDINARY_RAM,
            ranges=[MEM1],
        )
        obligation = build_bounded_memory_obligation_from_terminals(
            [terminal],
            [terminal],
            platform_profile=None,
            environment=env,
            deadline=_deadline(),
            z3=z3,
        )
        self.assertEqual(obligation["source"], SOURCE_AD_HOC)
        self.assertNotEqual(obligation["status"], "promotion-grade")
        self.assertFalse(obligation_is_promotion_grade(obligation))
        status = recompute_bounded_memory_attestation_status(
            {"bounded_memory": obligation},
            model_version=BOUNDED_MEMORY_MODEL_VERSION,
            allowed_versions=(BOUNDED_MEMORY_MODEL_VERSION,),
        )
        self.assertIn(status, (STATUS_SCOPED_ASSUMPTION, STATUS_INCOMPLETE))

    def test_include_memory_constraints_rejected(self) -> None:
        with self.assertRaises(ValueError):
            build_bounded_memory_obligation_from_terminals(
                [],
                [],
                platform_profile="xenoblade-us-retail-v1",
                include_equivalence_memory_constraints=True,
            )

    def test_draft_consumes_terminals_and_requirements(self) -> None:
        from tools.ppc_equivalence.capability_assurance import (
            draft_bounded_memory_assurance,
        )

        profile = load_platform_profile("xenoblade-us-retail-v1")
        terminal = self._lwz_terminal(in_range=True)
        proof = ProofResult(
            status=ProofStatus.EQUIVALENT,
            architecture_model=ARCHITECTURE_MODEL,
            format=RESULT_FORMAT,
            observables=["memory"],
            engine_hash="a" * 64,
            source_hash="b" * 64,
            git_commit="c" * 40,
            opcodes_used=["lwz", "blr"],
            memory_scope=MemoryScope(
                masking_semantics=MASKING_SEMANTICS,
                original=PrivateStackInfo(enabled_on_all_terminal_paths=True),
                candidate=PrivateStackInfo(enabled_on_all_terminal_paths=True),
            ),
            environment=MemoryEnvironment(
                profile=MemoryProfile.BOUNDED_ORDINARY_RAM,
                ranges=[MEM1],
            ),
        )
        requirements = {
            "schema_version": 1,
            "requirements_sha256": "d" * 64,
            "requirements": [
                {
                    "capability": "bounded-memory",
                    "requirement_sha256": "e" * 64,
                    "required_subjects": ["xenoblade-us-retail-v1"],
                }
            ],
        }
        assurance = draft_bounded_memory_assurance(
            proof,
            requirements=requirements,
            original_terminals=[terminal],
            candidate_terminals=[terminal],
            platform_profile=profile,
            deadline=_deadline(),
            z3=z3,
        )
        self.assertIsNotNone(assurance)
        assert assurance is not None
        names = {item.capability for item in assurance.capabilities}
        self.assertIn("bounded-memory", names)
        bm = next(c for c in assurance.capabilities if c.capability == "bounded-memory")
        self.assertEqual(bm.evidence.get("requirement_sha256"), "e" * 64)
        self.assertEqual(bm.evidence.get("requirements_sha256"), "d" * 64)

    def test_proof_request_field_name(self) -> None:
        self.assertEqual(
            PLATFORM_PROFILE_PROOF_REQUEST_FIELD,
            "platform_profile_sha256",
        )
        from tools.ppc_equivalence.provenance import proof_request_hash

        base = dict(
            original_hex="00",
            candidate_hex="00",
            contract="ppc-eabi",
        )
        without = proof_request_hash(**base)
        with_digest = proof_request_hash(
            **base, platform_profile_sha256="a" * 64
        )
        self.assertNotEqual(without, with_digest)


if __name__ == "__main__":
    unittest.main()
