"""P0-04 equivalence certificate migration report tests."""

from __future__ import annotations

import json
import tempfile
import unittest
from pathlib import Path

from tools.coop.lib.config import CoopConfig
from tools.coop.lib.targets import (
    EQUIVALENCE_CERTIFICATE_VERSION,
    equivalence_certificate_hash,
    equivalence_certificate_migration_report,
)
from tools.ppc_equivalence.provenance import hash_certifier_tree, hash_engine_tree
from tools.ppc_equivalence.result import ARCHITECTURE_MODEL, RESULT_FORMAT

_REPO_ROOT = Path(__file__).resolve().parents[3]


def _certificate(
    target_id: str,
    *,
    architecture: str = ARCHITECTURE_MODEL,
    result_format: int = RESULT_FORMAT,
    version: int = EQUIVALENCE_CERTIFICATE_VERSION,
    callees: list[dict[str, str]] | None = None,
) -> dict:
    certificate = {
        "version": version,
        "status": "SEMANTIC_CERTIFIED",
        "architecture": architecture,
        "result_format": result_format,
        "target_id": target_id,
        "evidence": "symbolic-equivalence",
        "retail_sha256": "1" * 64,
        "candidate_sha256": "2" * 64,
        "summary": {"reads": ["r3"], "writes": ["r3"], "return_behavior": "normal"},
        "callees": callees or [],
        "helpers": [],
        "engine_hash": hash_engine_tree(_REPO_ROOT),
        "certifier_hash": hash_certifier_tree(_REPO_ROOT),
        "source_hash": "b" * 64,
        "git_commit": "c" * 40,
        "memory_scope": {
            "comparison": "touched-byte-extensional",
            "masking_semantics": "per-implementation-independent-v1",
        },
    }
    certificate["certificate_sha256"] = equivalence_certificate_hash(certificate)
    return certificate


class MigrateReportTests(unittest.TestCase):
    def setUp(self) -> None:
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name)
        (self.root / "tools/coop").mkdir(parents=True)
        self.config = CoopConfig(project_root=self.root, region="us")
        self._write_targets([])

    def tearDown(self) -> None:
        self.tmp.cleanup()

    def _write_targets(self, rows: list[dict]) -> None:
        (self.root / "tools/coop/targets.json").write_text(
            json.dumps({"schema_version": 2, "targets": rows}, indent=2) + "\n",
            encoding="utf-8",
        )

    def _row(
        self,
        target_id: str,
        *,
        status: str,
        certificate: dict | None = None,
    ) -> dict:
        row = {
            "id": target_id,
            "symbol": target_id,
            "address": "0x80001000",
            "status": status,
            "workflow_status": "ACCEPTED",
        }
        if certificate is not None:
            row["equivalence_certificate"] = certificate
        return row

    def test_current_model_certificate_not_stale(self) -> None:
        valid = _certificate("ok")
        self._write_targets([self._row("ok", status="EQUIVALENT_MATCH", certificate=valid)])

        report = equivalence_certificate_migration_report(self.config)
        self.assertEqual(report["valid_count"], 1)
        self.assertEqual(report["stale_count"], 0)
        self.assertEqual(report["no_certificate_count"], 0)
        self.assertEqual(report["affected_count"], 0)
        self.assertEqual(report["valid"][0]["id"], "ok")
        self.assertEqual(report["stale"], [])
        self.assertEqual(report["no_certificate"], [])

    def test_v23_architecture_listed_as_stale(self) -> None:
        stale = _certificate("old", architecture="broadway-ppc32-be-v23")
        self._write_targets(
            [self._row("old", status="EQUIVALENT_MATCH", certificate=stale)]
        )

        report = equivalence_certificate_migration_report(self.config)
        self.assertEqual(report["valid_count"], 0)
        self.assertEqual(report["stale_count"], 1)
        self.assertIn("architecture", report["stale"][0]["certificate_error"])
        self.assertEqual(report["stale"][0]["architecture"], "broadway-ppc32-be-v23")

    def test_old_result_format_listed_as_stale(self) -> None:
        stale = _certificate("fmt", result_format=7)
        self._write_targets(
            [self._row("fmt", status="EQUIVALENT_MATCH", certificate=stale)]
        )

        report = equivalence_certificate_migration_report(self.config)
        self.assertEqual(report["stale_count"], 1)
        self.assertIn("result_format", report["stale"][0]["certificate_error"])
        self.assertEqual(report["stale"][0]["result_format"], 7)

    def test_old_certificate_version_listed_as_stale(self) -> None:
        stale = _certificate("ver", version=1)
        self._write_targets(
            [self._row("ver", status="EQUIVALENT_MATCH", certificate=stale)]
        )

        report = equivalence_certificate_migration_report(self.config)
        self.assertEqual(report["stale_count"], 1)
        self.assertIn("version", report["stale"][0]["certificate_error"])
        self.assertEqual(report["stale"][0]["certificate_version"], 1)

    def test_full_match_stale_cert_not_counted_as_affected(self) -> None:
        stale = _certificate("full", architecture="broadway-ppc32-be-v23")
        current = _certificate("ok")
        self._write_targets(
            [
                self._row("full", status="FULL_MATCH", certificate=stale),
                self._row("ok", status="EQUIVALENT_MATCH", certificate=current),
            ]
        )

        report = equivalence_certificate_migration_report(self.config)
        self.assertEqual(report["skipped_full_match_count"], 1)
        self.assertEqual(report["skipped_full_match_stale_cert_count"], 1)
        self.assertEqual(report["valid_count"], 1)
        self.assertEqual(report["stale_count"], 0)
        self.assertEqual(report["affected_count"], 0)
        self.assertEqual([entry["id"] for entry in report["stale"]], [])

    def test_missing_certificate_listed_separately(self) -> None:
        self._write_targets([self._row("bare", status="EQUIVALENT_MATCH")])

        report = equivalence_certificate_migration_report(self.config)
        self.assertEqual(report["no_certificate_count"], 1)
        self.assertEqual(report["stale_count"], 0)
        self.assertEqual(report["affected_count"], 1)
        self.assertEqual(report["no_certificate"][0]["id"], "bare")

    def test_report_includes_live_constants(self) -> None:
        report = equivalence_certificate_migration_report(self.config)
        self.assertEqual(report["architecture_model"], ARCHITECTURE_MODEL)
        self.assertEqual(report["result_format"], RESULT_FORMAT)
        self.assertEqual(
            report["equivalence_certificate_version"], EQUIVALENCE_CERTIFICATE_VERSION
        )
        self.assertIn("broadway-ppc32-be-v23", report["reject_architecture_models"])


if __name__ == "__main__":
    unittest.main()
