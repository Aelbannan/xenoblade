"""Audit-trail integrity: TCB module refs, exit kinds, certificate provenance."""

from __future__ import annotations

import re
import unittest
from pathlib import Path

from tools.coop.lib.equivalence_check import (
    _build_equivalence_certificate,
    _proof_audit_dict,
)
from tools.coop.lib.equivalence_policy import proof_result_from_certificate
from tools.coop.lib.targets import EQUIVALENCE_CERTIFICATE_VERSION
from tools.ppc_equivalence.decoder import decode_block
from tools.ppc_equivalence.memory_profile import MemoryEnvironment, MemoryProfile
from tools.ppc_equivalence.provenance import (
    proof_request_hash,
    proof_request_identity,
)
from tools.ppc_equivalence.result import (
    ARCHITECTURE_MODEL,
    RESULT_FORMAT,
    FloatingPointDomain,
    MemoryScope,
    PrivateStackInfo,
    ProofResult,
    ProofStatus,
)


ROOT = Path(__file__).resolve().parents[3]
PPC = ROOT / "tools" / "ppc_equivalence"
TCB_PATH = PPC / "TRUSTED_COMPUTING_BASE.md"
SOUNDNESS_PATH = PPC / "SOUNDNESS.md"

# Exit kinds emitted by semantics.execute_cfg (keep in sync with SOUNDNESS.md).
DOCUMENTED_EXIT_KINDS = frozenset({
    "return",
    "direct-branch",
    "indirect-branch",
    "fallthrough",
    "call",
    "call-indirect",
    "program-exception",
    "system-call",
    "return-from-interrupt",
})


def _tcb_module_refs(text: str) -> set[str]:
    """Collect backtick-quoted ``*.py`` module names from the TCB doc."""
    return set(re.findall(r"`([A-Za-z0-9_./-]+\.py)`", text))


def _resolve_tcb_module(name: str) -> Path:
    if name.startswith("tools/"):
        return ROOT / name
    if "/" in name:
        return PPC / name
    direct = PPC / name
    if direct.is_file():
        return direct
    return PPC / "fixtures" / name


class TrustedComputingBaseDocsTests(unittest.TestCase):
    def test_tcb_referenced_modules_exist(self) -> None:
        text = TCB_PATH.read_text(encoding="utf-8")
        refs = _tcb_module_refs(text)
        self.assertTrue(refs, "TCB should reference at least one .py module")
        missing = [
            name for name in sorted(refs)
            if not _resolve_tcb_module(name).is_file()
        ]
        self.assertEqual(
            missing, [],
            f"TCB references missing modules: {missing}",
        )

    def test_tcb_does_not_mention_stale_module_names(self) -> None:
        text = TCB_PATH.read_text(encoding="utf-8")
        stale = (
            "ops.py",
            "concrete_ops.py",
            "memory.py",
            "cfg.py",
            "contracts.py",
            "relocations.py",
            "callee.py",
            "solver_portfolio.py",
        )
        for name in stale:
            self.assertNotIn(
                f"`{name}`",
                text,
                f"stale TCB module name still present: {name}",
            )


class SoundnessExitKindDocsTests(unittest.TestCase):
    def test_soundness_exit_kinds_match_documented_set(self) -> None:
        text = SOUNDNESS_PATH.read_text(encoding="utf-8")
        table_kinds = set(
            re.findall(
                r"^\| `(return|direct-branch|indirect-branch|fallthrough|"
                r"call|call-indirect|program-exception|system-call|"
                r"return-from-interrupt|branch|trap|sc|rfi)` \|",
                text,
                flags=re.MULTILINE,
            )
        )
        self.assertEqual(table_kinds, DOCUMENTED_EXIT_KINDS)

    def test_soundness_uses_floating_point_domain_field(self) -> None:
        text = SOUNDNESS_PATH.read_text(encoding="utf-8")
        self.assertIn("`floating_point_domain`", text)
        self.assertNotIn("`floating_point_scope`", text)

    def test_soundness_result_fields_cover_provenance(self) -> None:
        text = SOUNDNESS_PATH.read_text(encoding="utf-8")
        for field in (
            "assumptions",
            "callee_contracts",
            "source_hash",
            "engine_hash",
            "limits",
            "repair_hint",
            "floating_point_domain",
            "environment",
        ):
            self.assertIn(f"`{field}`", text)


class ProofRequestHashTests(unittest.TestCase):
    def test_callee_sources_affect_source_hash(self) -> None:
        base = dict(
            original_hex="4e800020",
            candidate_hex="4e800020",
            contract="ppc-eabi",
            timeout_ms=1000,
            max_instructions=64,
            max_paths=16,
            max_loop_iterations=8,
            assumed_callees=["leaf"],
        )
        opaque = proof_request_hash(
            **base, callee_contract_sources={"leaf": "opaque-eabi"},
        )
        inferred = proof_request_hash(
            **base, callee_contract_sources={"leaf": "inferred:leaf"},
        )
        self.assertNotEqual(opaque, inferred)

    def test_identity_omits_none_fields(self) -> None:
        payload = proof_request_identity(
            original_hex="aa",
            candidate_hex="bb",
            contract="manual",
            certificate_target_id=None,
        )
        self.assertNotIn("certificate_target_id", payload)
        self.assertEqual(payload["contract"], "manual")

    def test_memory_profile_and_ranges_affect_source_hash(self) -> None:
        base = dict(
            original_hex="4e800020",
            candidate_hex="4e800020",
            contract="ppc-eabi",
            timeout_ms=1000,
        )
        assumed = proof_request_hash(
            **base,
            memory_profile="assumed-ordinary-ram",
            memory_ranges=[],
        )
        bounded = proof_request_hash(
            **base,
            memory_profile="bounded-ordinary-ram",
            memory_ranges=["0x80000000,0x817fffff"],
        )
        bounded_env = proof_request_hash(
            **base,
            memory_environment={
                "memory_profile": "bounded-ordinary-ram",
                "ranges": [{"start": "0x80000000", "end": "0x817fffff"}],
            },
        )
        self.assertNotEqual(assumed, bounded)
        self.assertNotEqual(assumed, bounded_env)
        # Range order must not change the hash.
        reordered = proof_request_hash(
            **base,
            memory_profile="bounded-ordinary-ram",
            memory_ranges=["0x81000000,0x81100000", "0x80000000,0x80100000"],
        )
        ordered = proof_request_hash(
            **base,
            memory_profile="bounded-ordinary-ram",
            memory_ranges=["0x80000000,0x80100000", "0x81000000,0x81100000"],
        )
        self.assertEqual(reordered, ordered)


class _FakeFunction:
    def __init__(self, name: str = "f") -> None:
        self.name = name
        self.base = 0x80000000
        self.code = bytes.fromhex("4e800020")
        self.relocations = ()


class CertificateAuditTrailTests(unittest.TestCase):
    def test_certificate_records_assumptions_and_callee_sources(self) -> None:
        blr = decode_block(bytes.fromhex("4e800020"), 0, validate_with_capstone=False)
        proof = ProofResult(
            status=ProofStatus.EQUIVALENT,
            contract="ppc-eabi",
            observables=["r3", "memory"],
            assumptions=["test-assumption-a", "test-assumption-b"],
            assumed_callees=["leaf"],
            callee_contracts={
                "leaf": {
                    "source": "certified:deadbeef",
                    "reads": ["r3"],
                    "writes": ["r3"],
                }
            },
            limits={
                "max_instructions": 128,
                "max_paths": 32,
                "max_loop_iterations": 8,
            },
            opcodes_used=["blr"],
            engine_hash="a" * 64,
            source_hash="b" * 64,
            git_commit="c" * 40,
            environment=MemoryEnvironment(
                profile=MemoryProfile.BOUNDED_ORDINARY_RAM,
                ranges=[(0x80000000, 0x817FFFFF)],
            ),
            floating_point_domain=FloatingPointDomain(),
            memory_scope=MemoryScope(
                original=PrivateStackInfo(enabled_on_all_terminal_paths=True),
                candidate=PrivateStackInfo(enabled_on_all_terminal_paths=True),
            ),
        )
        certificate, error = _build_equivalence_certificate(
            "us-test",
            _FakeFunction("retail"),
            _FakeFunction("candidate"),
            original=blr,
            candidate=blr,
            call_targets=frozenset(),
            callee_contracts={},
            dependencies=(),
            helpers=(),
            evidence="symbolic-equivalence",
            max_instructions=128,
            max_paths=32,
            max_loop_iterations=8,
            memory_scope=proof.memory_scope.to_dict() if proof.memory_scope else None,
            proof=proof,
        )
        self.assertEqual(error, "", error)
        assert certificate is not None
        self.assertEqual(certificate["architecture"], ARCHITECTURE_MODEL)
        self.assertEqual(certificate["result_format"], RESULT_FORMAT)
        self.assertEqual(certificate["version"], EQUIVALENCE_CERTIFICATE_VERSION)
        self.assertEqual(certificate["assumptions"], proof.assumptions)
        self.assertEqual(
            certificate["callee_contracts"]["leaf"]["source"],
            "certified:deadbeef",
        )
        self.assertEqual(certificate["limits"]["max_loop_iterations"], 8)
        self.assertEqual(certificate["source_hash"], "b" * 64)
        self.assertEqual(certificate["contract"], "ppc-eabi")
        self.assertIn("memory_profile", certificate["environment"])

        restored = proof_result_from_certificate(ProofStatus.EQUIVALENT, certificate)
        self.assertEqual(restored.assumptions, proof.assumptions)
        self.assertEqual(
            restored.callee_contracts["leaf"]["source"], "certified:deadbeef",
        )
        self.assertEqual(restored.limits["max_loop_iterations"], 8)
        self.assertEqual(restored.source_hash, "b" * 64)
        self.assertEqual(restored.contract, "ppc-eabi")
        self.assertIsNotNone(restored.floating_point_domain)
        self.assertIsNotNone(restored.environment)

    def test_proof_audit_dict_includes_repair_hint(self) -> None:
        proof = ProofResult(
            status=ProofStatus.NOT_EQUIVALENT,
            repair_hint={
                "instruction_index": 1,
                "diverged_register": "r3",
                "original_value": "0x1",
                "candidate_value": "0x2",
            },
            source_hash="d" * 64,
            engine_hash="e" * 64,
        )
        audit = _proof_audit_dict(proof)
        assert audit is not None
        self.assertEqual(audit["repair_hint"]["diverged_register"], "r3")
        self.assertEqual(audit["source_hash"], "d" * 64)


if __name__ == "__main__":
    unittest.main()
