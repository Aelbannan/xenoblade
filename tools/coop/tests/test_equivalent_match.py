"""Status ladder + EQUIVALENT_MATCH acceptance policy tests."""

from __future__ import annotations

import hashlib
import json
import unittest
import tempfile
from pathlib import Path

from tools.coop.lib.config import CoopConfig
from tools.coop.lib.equivalence_check import (
    CertifiedCalleeContext,
    EQUIVALENT_MATCH_MIN_PERCENT,
    EquivalenceProbe,
    _cache_get,
    _nv_spill_false_positive,
    _prove_bytes,
    should_probe_equivalence,
)
from tools.coop.lib.objdiff_report import UnitReport, classify_status, meets_required_level
from tools.coop.lib.project import Project
from tools.ppc_equivalence.elf_symbols import FunctionRelocation
from tools.ppc_equivalence.ir import R_PPC_REL24
from tools.ppc_equivalence.result import ARCHITECTURE_MODEL, RESULT_FORMAT, ProofStatus


def _unit(**kwargs) -> UnitReport:
    defaults = dict(
        unit_name="u",
        code_match_percent=0.0,
        data_match_percent=0.0,
        fuzzy_match_percent=0.0,
        total_functions=1,
        matched_functions=0,
        functions=[],
    )
    defaults.update(kwargs)
    return UnitReport(**defaults)


class NvSpillCertificateTests(unittest.TestCase):
    def test_nv_spill_false_positive_only_callee_saves(self) -> None:
        from types import SimpleNamespace

        ok = SimpleNamespace(
            reason=None,
            missing_reads=frozenset(),
            missing_writes=frozenset({"r28", "r29", "r30", "r31"}),
            missing_invalid_reasons=frozenset(),
        )
        self.assertTrue(_nv_spill_false_positive(ok))

        volatile = SimpleNamespace(
            reason=None,
            missing_reads=frozenset(),
            missing_writes=frozenset({"r3"}),
            missing_invalid_reasons=frozenset(),
        )
        self.assertFalse(_nv_spill_false_positive(volatile))

        with_reason = SimpleNamespace(
            reason="callee does not have only normal returns: fallthrough",
            missing_reads=frozenset(),
            missing_writes=frozenset({"r31"}),
            missing_invalid_reasons=frozenset(),
        )
        self.assertFalse(_nv_spill_false_positive(with_reason))


class ClassifyStatusTests(unittest.TestCase):
    def test_full_match_unchanged(self) -> None:
        self.assertEqual(
            classify_status(100.0, _unit(), symbol="f"),
            "FULL_MATCH",
        )

    def test_equivalent_match_requires_proof_and_floor(self) -> None:
        self.assertEqual(
            classify_status(
                85.0,
                _unit(),
                symbol="f",
                equivalence=ProofStatus.EQUIVALENT,
            ),
            "EQUIVALENT_MATCH",
        )
        self.assertEqual(
            classify_status(
                85.0,
                _unit(),
                symbol="f",
                equivalence=ProofStatus.NOT_EQUIVALENT,
            ),
            "HIGH_MATCH",
        )
        self.assertEqual(
            classify_status(
                85.0,
                _unit(),
                symbol="f",
                equivalence=None,
            ),
            "HIGH_MATCH",
        )

    def test_equivalent_outranks_code_match_band(self) -> None:
        self.assertEqual(
            classify_status(
                96.0,
                _unit(),
                symbol="f",
                equivalence=ProofStatus.EQUIVALENT,
            ),
            "EQUIVALENT_MATCH",
        )
        self.assertEqual(
            classify_status(
                96.0,
                _unit(),
                symbol="f",
                equivalence=ProofStatus.INCONCLUSIVE_UNSUPPORTED,
            ),
            "CODE_MATCH",
        )

    def test_below_floor_never_equivalent_match(self) -> None:
        self.assertNotEqual(
            classify_status(
                EQUIVALENT_MATCH_MIN_PERCENT - 0.1,
                _unit(),
                symbol="f",
                equivalence=ProofStatus.EQUIVALENT,
            ),
            "EQUIVALENT_MATCH",
        )


class MeetsRequiredLevelTests(unittest.TestCase):
    def test_equivalent_match_accepts_full_and_equivalent(self) -> None:
        unit = _unit(code_match_percent=50.0, data_match_percent=50.0)
        self.assertTrue(
            meets_required_level(
                "EQUIVALENT_MATCH",
                "EQUIVALENT_MATCH",
                function_match=85.0,
                unit=unit,
                symbol="f",
                equivalence=ProofStatus.EQUIVALENT,
            )
        )
        self.assertTrue(
            meets_required_level(
                "EQUIVALENT_MATCH",
                "FULL_MATCH",
                function_match=100.0,
                unit=unit,
                symbol="f",
            )
        )
        self.assertFalse(
            meets_required_level(
                "EQUIVALENT_MATCH",
                "CODE_MATCH",
                function_match=96.0,
                unit=unit,
                symbol="f",
                equivalence=ProofStatus.INCONCLUSIVE_UNSUPPORTED,
            )
        )

    def test_full_match_bar_still_requires_100(self) -> None:
        unit = _unit()
        self.assertFalse(
            meets_required_level(
                "FULL_MATCH",
                "EQUIVALENT_MATCH",
                function_match=90.0,
                unit=unit,
                symbol="f",
                equivalence=ProofStatus.EQUIVALENT,
            )
        )


class ProbeGateTests(unittest.TestCase):
    def test_probe_window(self) -> None:
        self.assertFalse(should_probe_equivalence(None))
        self.assertFalse(should_probe_equivalence(49.9))
        self.assertTrue(should_probe_equivalence(50.0))
        self.assertTrue(should_probe_equivalence(99.9))
        self.assertFalse(should_probe_equivalence(100.0))

    def test_registry_proof_fails_closed_without_current_callee_certificate(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            project = Project(CoopConfig(project_root=Path(tmp), region="us"))
            probe = _prove_bytes(
                project,
                "caller",
                bytes.fromhex("48000001 4e800020"),
                0,
                bytes.fromhex("48000001 4e800020"),
                0,
                contract="auto",
                timeout_ms=1000,
                max_instructions=32,
                max_paths=8,
                original_relocations=(FunctionRelocation(0, R_PPC_REL24, "leaf", 0),),
                candidate_relocations=(FunctionRelocation(0, R_PPC_REL24, "leaf", 0),),
                certificate_target_id="caller-id",
                certified_context=CertifiedCalleeContext({}, ()),
            )
        self.assertEqual(probe.status, ProofStatus.INCONCLUSIVE_UNVALIDATED_CALLEE)
        self.assertIn("leaf", probe.detail)


class CacheInvalidationTests(unittest.TestCase):
    """P0-04: proof cache entries with stale version are not returned."""

    def _make_cache_entry(
        self,
        cache_dir: Path,
        key: str,
        *,
        architecture: str,
        result_format: int,
        engine_hash: str | None = ...,
        certifier_hash: str | None = ...,
    ) -> Path:
        from tools.coop.lib.equivalence_check import (
            _current_certifier_hash,
            _current_engine_hash,
        )

        payload: dict = {
            "architecture": architecture,
            "result_format": result_format,
            "status": "equivalent",
            "detail": "",
        }
        if engine_hash is ...:
            payload["engine_hash"] = _current_engine_hash()
        elif engine_hash is not None:
            payload["engine_hash"] = engine_hash
        if certifier_hash is ...:
            payload["certifier_hash"] = _current_certifier_hash()
        elif certifier_hash is not None:
            payload["certifier_hash"] = certifier_hash
        entry = cache_dir / f"{key}.json"
        entry.write_text(json.dumps(payload))
        return entry

    def test_cache_rejects_old_architecture_model(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            cache_dir = Path(tmp)
            key = hashlib.sha256(b"stale-arch").hexdigest()
            self._make_cache_entry(
                cache_dir, key,
                architecture="broadway-ppc32-be-v18",
                result_format=RESULT_FORMAT,
                engine_hash="a" * 64,
                certifier_hash="b" * 64,
            )
            self.assertIsNone(_cache_get(key, cache_dir))

    def test_cache_rejects_old_result_format(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            cache_dir = Path(tmp)
            key = hashlib.sha256(b"stale-format").hexdigest()
            self._make_cache_entry(
                cache_dir, key,
                architecture=ARCHITECTURE_MODEL,
                result_format=7,
                engine_hash="a" * 64,
                certifier_hash="b" * 64,
            )
            self.assertIsNone(_cache_get(key, cache_dir))

    def test_cache_rejects_missing_engine_hash(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            cache_dir = Path(tmp)
            key = hashlib.sha256(b"missing-engine").hexdigest()
            self._make_cache_entry(
                cache_dir, key,
                architecture=ARCHITECTURE_MODEL,
                result_format=RESULT_FORMAT,
                engine_hash=None,
                certifier_hash=None,
            )
            self.assertIsNone(_cache_get(key, cache_dir))

    def test_cache_rejects_stale_engine_hash(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            cache_dir = Path(tmp)
            key = hashlib.sha256(b"stale-engine").hexdigest()
            self._make_cache_entry(
                cache_dir, key,
                architecture=ARCHITECTURE_MODEL,
                result_format=RESULT_FORMAT,
                engine_hash="a" * 64,
                certifier_hash="b" * 64,
            )
            self.assertIsNone(_cache_get(key, cache_dir))

    def test_cache_rejects_missing_certifier_hash(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            cache_dir = Path(tmp)
            key = hashlib.sha256(b"missing-certifier").hexdigest()
            self._make_cache_entry(
                cache_dir, key,
                architecture=ARCHITECTURE_MODEL,
                result_format=RESULT_FORMAT,
                certifier_hash=None,
            )
            self.assertIsNone(_cache_get(key, cache_dir))

    def test_cache_rejects_stale_certifier_hash(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            cache_dir = Path(tmp)
            key = hashlib.sha256(b"stale-certifier").hexdigest()
            self._make_cache_entry(
                cache_dir, key,
                architecture=ARCHITECTURE_MODEL,
                result_format=RESULT_FORMAT,
                certifier_hash="b" * 64,
            )
            self.assertIsNone(_cache_get(key, cache_dir))

    def test_cache_returns_current_entry(self) -> None:
        from tools.coop.lib.equivalence_check import (
            _cache_put,
            _current_certifier_hash,
            _current_engine_hash,
        )
        from tools.ppc_equivalence.result import ProofResult

        with tempfile.TemporaryDirectory() as tmp:
            cache_dir = Path(tmp)
            key = hashlib.sha256(b"current").hexdigest()
            engine = _current_engine_hash()
            proof = ProofResult(
                status=ProofStatus.EQUIVALENT,
                engine_hash=engine,
                source_hash="e" * 64,
                git_commit="f" * 40,
            )
            certificate = {
                "architecture": ARCHITECTURE_MODEL,
                "result_format": RESULT_FORMAT,
                "engine_hash": engine,
                "source_hash": proof.source_hash,
                "git_commit": proof.git_commit,
            }
            _cache_put(
                key,
                EquivalenceProbe(
                    ProofStatus.EQUIVALENT,
                    "",
                    certificate=certificate,
                    proof=proof,
                ),
                cache_dir,
                engine_hash=engine,
                certifier_hash=_current_certifier_hash(),
            )
            result = _cache_get(key, cache_dir)
            self.assertIsNotNone(result)
            assert result is not None
            self.assertEqual(result.status, ProofStatus.EQUIVALENT)
            self.assertIsInstance(result.proof, ProofResult)

    def test_cache_misses_evidence_free_equivalent(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            cache_dir = Path(tmp)
            key = hashlib.sha256(b"evidence-free").hexdigest()
            self._make_cache_entry(
                cache_dir, key,
                architecture=ARCHITECTURE_MODEL,
                result_format=RESULT_FORMAT,
            )
            self.assertIsNone(_cache_get(key, cache_dir))

    def test_cache_restores_proofresult_not_dict(self) -> None:
        from tools.coop.lib.equivalence_check import (
            _cache_put,
            _current_certifier_hash,
            _current_engine_hash,
        )
        from tools.ppc_equivalence.result import ProofResult

        with tempfile.TemporaryDirectory() as tmp:
            cache_dir = Path(tmp)
            key = hashlib.sha256(b"restore-proof").hexdigest()
            proof = ProofResult(
                status=ProofStatus.EQUIVALENT,
                engine_hash=_current_engine_hash(),
                source_hash="a" * 64,
                proof_features=["memory-bus"],
                memory_bus={
                    "schema_version": 1,
                    "algorithm": "memory-bus-v1",
                    "status": "discharged",
                },
            )
            probe = EquivalenceProbe(
                ProofStatus.EQUIVALENT,
                "ok",
                certificate={
                    "architecture": ARCHITECTURE_MODEL,
                    "result_format": RESULT_FORMAT,
                    "engine_hash": _current_engine_hash(),
                    "proof_features": ["memory-bus"],
                    "memory_bus": proof.memory_bus,
                },
                proof=proof,
            )
            _cache_put(
                key,
                probe,
                cache_dir,
                engine_hash=_current_engine_hash(),
                certifier_hash=_current_certifier_hash(),
            )
            restored = _cache_get(key, cache_dir)
            self.assertIsNotNone(restored)
            assert restored is not None
            self.assertIsInstance(restored.proof, ProofResult)
            assert isinstance(restored.proof, ProofResult)
            # Weak/forged memory-bus (missing digests) and/or temporary
            # UNSUPPORTED_FOR_EQUIVALENT freeze demote on revalidation.
            # Outer probe status must match the re-gated ProofResult.
            self.assertEqual(
                restored.proof.status,
                ProofStatus.INCONCLUSIVE_UNSUPPORTED,
            )
            self.assertEqual(restored.status, restored.proof.status)
            self.assertIsNone(restored.certificate)
            self.assertIn("memory-bus", restored.proof.proof_features)
            self.assertIsNotNone(restored.proof.memory_bus)
            # Attribute access must not crash (legacy dict bug).
            _ = restored.proof.engine_hash

    def test_cache_sound_evidence_round_trip_stays_equivalent(self) -> None:
        from tools.coop.lib.equivalence_check import (
            _cache_put,
            _current_certifier_hash,
            _current_engine_hash,
        )
        from tools.ppc_equivalence.result import ProofResult

        with tempfile.TemporaryDirectory() as tmp:
            cache_dir = Path(tmp)
            key = hashlib.sha256(b"sound-round-trip").hexdigest()
            engine = _current_engine_hash()
            proof = ProofResult(
                status=ProofStatus.EQUIVALENT,
                engine_hash=engine,
                source_hash="b" * 64,
                git_commit="c" * 40,
            )
            certificate = {
                "architecture": ARCHITECTURE_MODEL,
                "result_format": RESULT_FORMAT,
                "engine_hash": engine,
                "source_hash": proof.source_hash,
                "git_commit": proof.git_commit,
            }
            probe = EquivalenceProbe(
                ProofStatus.EQUIVALENT,
                "ok",
                certificate=certificate,
                proof=proof,
            )
            _cache_put(
                key,
                probe,
                cache_dir,
                engine_hash=engine,
                certifier_hash=_current_certifier_hash(),
            )
            restored = _cache_get(key, cache_dir)
            self.assertIsNotNone(restored)
            assert restored is not None
            self.assertIsInstance(restored.proof, ProofResult)
            assert isinstance(restored.proof, ProofResult)
            self.assertEqual(restored.status, ProofStatus.EQUIVALENT)
            self.assertEqual(restored.proof.status, ProofStatus.EQUIVALENT)
            self.assertIsNotNone(restored.certificate)

    def test_cache_demoted_memory_bus_never_reports_equivalent(self) -> None:
        from tools.coop.lib.equivalence_check import (
            _cache_put,
            _current_certifier_hash,
            _current_engine_hash,
        )
        from tools.ppc_equivalence.result import ProofResult

        with tempfile.TemporaryDirectory() as tmp:
            cache_dir = Path(tmp)
            key = hashlib.sha256(b"forged-memory-bus").hexdigest()
            # Intentionally forged/weak discharged claim (no schema v2 digests).
            forged_bus = {
                "schema_version": 1,
                "algorithm": "memory-bus-v1",
                "status": "discharged",
                "regions": [{"kind": "mmio", "start": 0xCC000000, "end": 0xCC001000}],
            }
            proof = ProofResult(
                status=ProofStatus.EQUIVALENT,
                engine_hash=_current_engine_hash(),
                source_hash="d" * 64,
                proof_features=["memory-bus"],
                memory_bus=forged_bus,
            )
            probe = EquivalenceProbe(
                ProofStatus.EQUIVALENT,
                "cached-as-equivalent",
                certificate={
                    "architecture": ARCHITECTURE_MODEL,
                    "result_format": RESULT_FORMAT,
                    "engine_hash": _current_engine_hash(),
                    "proof_features": ["memory-bus"],
                    "memory_bus": forged_bus,
                },
                proof=proof,
            )
            _cache_put(
                key,
                probe,
                cache_dir,
                engine_hash=_current_engine_hash(),
                certifier_hash=_current_certifier_hash(),
            )
            restored = _cache_get(key, cache_dir)
            self.assertIsNotNone(restored)
            assert restored is not None
            assert isinstance(restored.proof, ProofResult)
            self.assertNotEqual(restored.status, ProofStatus.EQUIVALENT)
            self.assertEqual(restored.status, restored.proof.status)
            self.assertEqual(
                restored.proof.status,
                ProofStatus.INCONCLUSIVE_UNSUPPORTED,
            )
            self.assertIsNone(restored.certificate)

    def test_cache_key_changes_when_memory_bus_obligation_mutates(self) -> None:
        from tools.coop.lib.equivalence_check import _cache_key

        common = dict(
            contract_name="auto",
            observables=("r3",),
            original_hex="38600001",
            candidate_hex="38600001",
            original_base=0x80000000,
            candidate_base=0x80000000,
            proof_features=["memory-bus"],
        )
        left = _cache_key(
            **common,
            memory_bus={"schema_version": 1, "algorithm": "a", "status": "discharged"},
        )
        right = _cache_key(
            **common,
            memory_bus={"schema_version": 1, "algorithm": "b", "status": "discharged"},
        )
        self.assertNotEqual(left, right)

    def test_cache_key_binds_certified_callee_digests(self) -> None:
        """H3: a change to any callee certificate/summary digest busts the
        caller's cache identity; absence keeps legacy (callee-free) identity."""
        from tools.coop.lib.equivalence_check import _cache_key

        common = dict(
            contract_name="auto",
            observables=("r3",),
            original_hex="48000001",
            candidate_hex="48000001",
            original_base=0x80000000,
            candidate_base=0x80000000,
        )
        base = _cache_key(**common)
        bound = _cache_key(
            **common,
            certified_callee_digests={
                "leaf": {
                    "certificate_sha256": "a" * 64,
                    "summary_sha256": "b" * 64,
                    "body_sha256": "c" * 64,
                },
            },
        )
        changed_cert = _cache_key(
            **common,
            certified_callee_digests={
                "leaf": {
                    "certificate_sha256": "d" * 64,
                    "summary_sha256": "b" * 64,
                    "body_sha256": "c" * 64,
                },
            },
        )
        changed_summary = _cache_key(
            **common,
            certified_callee_digests={
                "leaf": {
                    "certificate_sha256": "a" * 64,
                    "summary_sha256": "e" * 64,
                    "body_sha256": "c" * 64,
                },
            },
        )
        self.assertNotEqual(base, bound)
        self.assertNotEqual(bound, changed_cert)
        self.assertNotEqual(bound, changed_summary)
        # None / empty leaves existing (standalone) identity unchanged.
        self.assertEqual(base, _cache_key(**common, certified_callee_digests=None))
        self.assertEqual(base, _cache_key(**common, certified_callee_digests={}))


if __name__ == "__main__":
    unittest.main()
