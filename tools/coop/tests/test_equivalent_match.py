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
    _cache_get,
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
    ) -> Path:
        from tools.coop.lib.equivalence_check import _current_engine_hash

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
            )
            self.assertIsNone(_cache_get(key, cache_dir))

    def test_cache_returns_current_entry(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            cache_dir = Path(tmp)
            key = hashlib.sha256(b"current").hexdigest()
            self._make_cache_entry(
                cache_dir, key,
                architecture=ARCHITECTURE_MODEL,
                result_format=RESULT_FORMAT,
            )
            result = _cache_get(key, cache_dir)
            self.assertIsNotNone(result)
            self.assertEqual(result.status, ProofStatus.EQUIVALENT)


if __name__ == "__main__":
    unittest.main()
