"""End-to-end certify_unit_symbol integration tests."""

from __future__ import annotations

import importlib.util
import json
import tempfile
import unittest
from pathlib import Path

from tools.coop.lib.config import CoopConfig
from tools.coop.lib.equivalence_check import certify_unit_symbol, prove_unit_symbol
from tools.coop.lib.project import ObjdiffUnit, Project
from tools.coop.lib.targets import equivalence_certificate_error
from tools.ppc_equivalence.provenance import hash_certifier_tree, hash_engine_tree
from tools.ppc_equivalence.result import ProofStatus
from tools.ppc_equivalence.tests.test_elf_symbols import _EQ_LEFT, _NEQ, build_reloc_elf
from unittest import mock

_HAS_Z3 = importlib.util.find_spec("z3") is not None
_REPO_ROOT = Path(__file__).resolve().parents[3]
_TARGET_ID = "us-certify-leaf"
_SYMBOL = "func__3LeafFv"
_UNIT_NAME = "demo/CertifyLeaf"


class CertifyUnitSymbolTests(unittest.TestCase):
    def setUp(self) -> None:
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name)
        (self.root / "tools/coop").mkdir(parents=True)
        (self.root / "build/us").mkdir(parents=True)
        self.config = CoopConfig(project_root=self.root, region="us")

        retail = self.root / "build/us/retail.o"
        decomp = self.root / "build/us/decomp.o"
        elf_bytes = build_reloc_elf({_SYMBOL: _EQ_LEFT})
        retail.write_bytes(elf_bytes)
        decomp.write_bytes(elf_bytes)

        (self.root / "build/us/objdiff.json").write_text(
            json.dumps(
                {
                    "units": [
                        {
                            "name": _UNIT_NAME,
                            "target_path": "build/us/retail.o",
                            "base_path": "build/us/decomp.o",
                        }
                    ]
                }
            ),
            encoding="utf-8",
        )
        (self.root / "tools/coop/targets.json").write_text(
            json.dumps(
                {
                    "schema_version": 2,
                    "default_required_level": "EQUIVALENT_MATCH",
                    "targets": [
                        {
                            "id": _TARGET_ID,
                            "symbol": _SYMBOL,
                            "address": "0x80001000",
                            "function": _SYMBOL,
                            "unit": _UNIT_NAME,
                            "region": "us",
                            "status": "FULL_MATCH",
                            "called_functions": [],
                            "abi_helper_calls": [],
                        }
                    ],
                }
            ),
            encoding="utf-8",
        )

        self.project = Project(self.config)
        self.unit = ObjdiffUnit(
            name=_UNIT_NAME,
            target_path=retail,
            base_path=decomp,
            source_path=None,
        )

    def tearDown(self) -> None:
        self.tmp.cleanup()


class SmtOptInTests(unittest.TestCase):
    """prove_unit_symbol smt=False runs only the reg-renaming witness.

    The retail/decomp pair differs in an immediate (addi r3,r3,4 vs addi
    r3,r3,5), so the witness gates (non-register field bit-equality) reject
    it and the pair falls through to the solver only when ``smt=True``.
    """

    def setUp(self) -> None:
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name)
        (self.root / "tools/coop").mkdir(parents=True)
        (self.root / "build/us").mkdir(parents=True)
        self.config = CoopConfig(project_root=self.root, region="us")

        retail = self.root / "build/us/retail.o"
        decomp = self.root / "build/us/decomp.o"
        retail.write_bytes(build_reloc_elf({_SYMBOL: _EQ_LEFT}))
        decomp.write_bytes(build_reloc_elf({_SYMBOL: _NEQ}))

        (self.root / "tools/coop/targets.json").write_text(
            json.dumps(
                {
                    "schema_version": 2,
                    "default_required_level": "EQUIVALENT_MATCH",
                    "targets": [
                        {
                            "id": _TARGET_ID,
                            "symbol": _SYMBOL,
                            "address": "0x80001000",
                            "function": _SYMBOL,
                            "unit": _UNIT_NAME,
                            "region": "us",
                            "status": "FULL_MATCH",
                            "called_functions": [],
                            "abi_helper_calls": [],
                        }
                    ],
                }
            ),
            encoding="utf-8",
        )

        self.project = Project(self.config)
        self.unit = ObjdiffUnit(
            name=_UNIT_NAME,
            target_path=retail,
            base_path=decomp,
            source_path=None,
        )

    def tearDown(self) -> None:
        self.tmp.cleanup()

    def test_smt_disabled_skips_solver_and_reports_disabled(self) -> None:
        """smt=False never touches the solver: the witness cannot certify the
        immediate-differing pair, so the probe reports INCONCLUSIVE_SMT_DISABLED
        (no Z3 dependency)."""
        probe = prove_unit_symbol(
            self.project, self.unit, _SYMBOL, target_id=_TARGET_ID, smt=False,
        )
        self.assertEqual(probe.status, ProofStatus.INCONCLUSIVE_SMT_DISABLED)
        self.assertIsNone(probe.certificate)

    @unittest.skipUnless(_HAS_Z3, "z3-solver is not installed")
    def test_smt_enabled_falls_through_to_solver(self) -> None:
        """smt=True (the default) resolves the same pair via Z3, never
        reporting the disabled status."""
        probe = prove_unit_symbol(
            self.project, self.unit, _SYMBOL, target_id=_TARGET_ID, smt=True,
        )
        self.assertNotEqual(probe.status, ProofStatus.INCONCLUSIVE_SMT_DISABLED)
        self.assertIn(probe.status, (ProofStatus.EQUIVALENT, ProofStatus.NOT_EQUIVALENT))


class FullMatchCertWithoutSmtTests(unittest.TestCase):
    """FULL_MATCH (100% static) targets get certificates without SMT.

    The FULL_MATCH certification branch in evaluate_unit_match fires on
    pct >= 100 independent of ``run_smt``, and certify_unit_symbol synthesizes
    the certificate from the byte-identical body (no solver). This keeps the
    callees-accepted frontier populated when mass-cycling without --smt.
    """

    def setUp(self) -> None:
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name)
        (self.root / "tools/coop").mkdir(parents=True)
        (self.root / "build/us").mkdir(parents=True)
        self.config = CoopConfig(project_root=self.root, region="us")

        retail = self.root / "build/us/retail.o"
        decomp = self.root / "build/us/decomp.o"
        elf_bytes = build_reloc_elf({_SYMBOL: _EQ_LEFT})
        retail.write_bytes(elf_bytes)
        decomp.write_bytes(elf_bytes)

        (self.root / "tools/coop/targets.json").write_text(
            json.dumps(
                {
                    "schema_version": 2,
                    "default_required_level": "EQUIVALENT_MATCH",
                    "targets": [
                        {
                            "id": _TARGET_ID,
                            "symbol": _SYMBOL,
                            "address": "0x80001000",
                            "function": _SYMBOL,
                            "unit": _UNIT_NAME,
                            "region": "us",
                            "status": "FULL_MATCH",
                            "called_functions": [],
                            "abi_helper_calls": [],
                        }
                    ],
                }
            ),
            encoding="utf-8",
        )

        self.project = Project(self.config)
        self.unit = ObjdiffUnit(
            name=_UNIT_NAME,
            target_path=retail,
            base_path=decomp,
            source_path=None,
        )

    def tearDown(self) -> None:
        self.tmp.cleanup()

    def test_full_match_certificate_issues_without_smt(self) -> None:
        from tools.coop.lib import objdiff_report as _or

        fn_match = _or.FunctionMatch(
            name=_SYMBOL, demangled_name=None, match_percent=100.0, size=8,
        )
        unit_report = _or.UnitReport(
            unit_name=_UNIT_NAME,
            code_match_percent=100.0,
            data_match_percent=100.0,
            fuzzy_match_percent=100.0,
            total_functions=1,
            matched_functions=1,
            functions=[fn_match],
        )
        with mock.patch.object(_or, "report_unit", return_value=unit_report), mock.patch.object(
            _or, "find_function_match", return_value=fn_match
        ):
            ev = _or.evaluate_unit_match(
                self.project, self.unit, _SYMBOL, target_id=_TARGET_ID, run_smt=False,
            )
        self.assertEqual(ev.status, "FULL_MATCH")
        self.assertIsNotNone(ev.equivalence_certificate)
        self.assertEqual(
            ev.equivalence_certificate.get("evidence"), "full-instruction-match"
        )

    @unittest.skipUnless(_HAS_Z3, "z3-solver is not installed")
    def test_certify_unit_symbol_issues_live_engine_hash_certificate(self) -> None:
        probe = certify_unit_symbol(
            self.project,
            self.unit,
            _SYMBOL,
            _TARGET_ID,
        )
        self.assertEqual(probe.status, ProofStatus.EQUIVALENT, probe.detail)
        self.assertIsNotNone(probe.certificate)
        certificate = probe.certificate
        assert certificate is not None

        engine_hash = certificate.get("engine_hash")
        self.assertIsInstance(engine_hash, str)
        assert isinstance(engine_hash, str)
        self.assertRegex(engine_hash, r"^[0-9a-f]{64}$")
        self.assertEqual(engine_hash, hash_engine_tree(_REPO_ROOT))

        certifier_hash = certificate.get("certifier_hash")
        self.assertIsInstance(certifier_hash, str)
        assert isinstance(certifier_hash, str)
        self.assertRegex(certifier_hash, r"^[0-9a-f]{64}$")
        self.assertEqual(certifier_hash, hash_certifier_tree(_REPO_ROOT))

        row = {
            "id": _TARGET_ID,
            "equivalence_certificate": certificate,
        }
        rows_by_id = {_TARGET_ID: row}
        self.assertIsNone(equivalence_certificate_error(row, rows_by_id))

        cert_hash = certificate.get("certificate_sha256")
        self.assertIsInstance(cert_hash, str)
        assert isinstance(cert_hash, str)
        self.assertRegex(cert_hash, r"^[0-9a-f]{64}$")
        self.assertEqual(certificate.get("target_id"), _TARGET_ID)
        self.assertEqual(certificate.get("evidence"), "full-instruction-match")
        self.assertEqual(certificate.get("status"), "SEMANTIC_CERTIFIED")


    def test_rebind_certificate_provenance_refreshes_stale_engine_hash(self) -> None:
        from tools.coop.lib.equivalence_check import rebind_certificate_provenance
        from tools.coop.lib.targets import equivalence_certificate_hash
        from tools.ppc_equivalence.result import ARCHITECTURE_MODEL, RESULT_FORMAT

        stale = {
            "version": 17,
            "status": "SEMANTIC_CERTIFIED",
            "architecture": ARCHITECTURE_MODEL,
            "result_format": RESULT_FORMAT,
            "target_id": _TARGET_ID,
            "evidence": "full-instruction-match",
            "retail_sha256": "a" * 64,
            "candidate_sha256": "b" * 64,
            "summary": {"reads": [], "writes": [], "invalid_reasons": [], "return_behavior": "normal"},
            "callees": [],
            "helpers": [],
            "engine_hash": "a" * 64,
            "certifier_hash": "b" * 64,
        }
        stale["certificate_sha256"] = equivalence_certificate_hash(stale)
        refreshed = rebind_certificate_provenance(stale)
        self.assertEqual(refreshed["engine_hash"], hash_engine_tree(_REPO_ROOT))
        self.assertEqual(refreshed["certifier_hash"], hash_certifier_tree(_REPO_ROOT))
        self.assertNotEqual(refreshed["certificate_sha256"], stale["certificate_sha256"])
if __name__ == "__main__":
    unittest.main()
