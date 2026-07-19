"""P0-02 / P0-04 registry audit-promotion migration tests."""

from __future__ import annotations

import json
import tempfile
import unittest
from pathlib import Path

from tools.coop.lib.attempts import read_attempts
from tools.coop.lib.config import CoopConfig
from tools.coop.lib.targets import (
    EQUIVALENCE_CERTIFICATE_VERSION,
    EQUIVALENCE_PROMOTION_POLICY,
    audit_promotion_registry,
    equivalence_certificate_hash,
    load_targets_document,
    update_target_result,
    validate_targets,
)
from tools.ppc_equivalence.result import ARCHITECTURE_MODEL, RESULT_FORMAT


def _certificate(
    target_id: str,
    *,
    architecture: str = ARCHITECTURE_MODEL,
    result_format: int = RESULT_FORMAT,
    version: int = EQUIVALENCE_CERTIFICATE_VERSION,
    engine_hash: str = "a" * 64,
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
        "engine_hash": engine_hash,
        "source_hash": "b" * 64,
        "git_commit": "c" * 40,
        "memory_scope": {
            "comparison": "touched-byte-extensional",
            "masking_semantics": "per-implementation-independent-v1",
        },
    }
    certificate["certificate_sha256"] = equivalence_certificate_hash(certificate)
    return certificate


class AuditPromotionTests(unittest.TestCase):
    def setUp(self) -> None:
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name)
        (self.root / "tools/coop").mkdir(parents=True)
        (self.root / "docs/evidence/decomp").mkdir(parents=True)
        self.config = CoopConfig(
            project_root=self.root,
            region="us",
            automatic_promotion=True,
            reject_architecture_models=("broadway-ppc32-be-v18",),
            allowed_confidence_tiers=frozenset({"A", "B"}),
        )
        self._write_targets([])

    def tearDown(self) -> None:
        self.tmp.cleanup()

    def _write_targets(self, rows: list[dict]) -> None:
        (self.root / "tools/coop/targets.json").write_text(
            json.dumps(
                {
                    "schema_version": 2,
                    "default_required_level": "EQUIVALENT_MATCH",
                    "targets": rows,
                },
                indent=2,
            )
            + "\n",
            encoding="utf-8",
        )

    def _row(
        self,
        target_id: str,
        *,
        status: str,
        certificate: dict | None = None,
        workflow_status: str = "ACCEPTED",
    ) -> dict:
        row = {
            "id": target_id,
            "symbol": target_id,
            "address": "0x80001000",
            "function": target_id,
            "unit": "demo/Demo",
            "region": "us",
            "status": status,
            "workflow_status": workflow_status,
            "instruction_match": 72.5,
            "equivalence_status": "equivalent",
        }
        if certificate is not None:
            row["equivalence_certificate"] = certificate
        return row

    def test_stale_model_equivalent_match_reported_and_applied(self) -> None:
        stale = _certificate("stale", architecture="broadway-ppc32-be-v18")
        self._write_targets(
            [self._row("stale", status="EQUIVALENT_MATCH", certificate=stale)]
        )

        dry = audit_promotion_registry(self.config, apply=False)
        self.assertEqual(dry["affected_count"], 1)
        self.assertEqual(dry["valid_count"], 0)
        self.assertEqual(dry["applied_count"], 0)
        self.assertIn("architecture", dry["affected"][0]["certificate_error"])

        after_dry = load_targets_document(self.config)["targets"][0]
        self.assertEqual(after_dry["status"], "EQUIVALENT_MATCH")
        self.assertEqual(after_dry["workflow_status"], "ACCEPTED")

        applied = audit_promotion_registry(self.config, apply=True)
        self.assertEqual(applied["affected_count"], 1)
        self.assertEqual(applied["applied_count"], 1)

        row = load_targets_document(self.config)["targets"][0]
        self.assertEqual(row["status"], "CODE_MATCH")
        self.assertEqual(row["workflow_status"], "REVALIDATION_REQUIRED")
        self.assertIn("equivalence_certificate", row)

        attempts = read_attempts(self.config.resolve(self.config.attempt_log))
        self.assertEqual(len(attempts), 1)
        self.assertEqual(attempts[0]["status"], "CODE_MATCH")
        self.assertIn("revalidation required", attempts[0]["hypothesis"].lower())
        self.assertIn(stale["certificate_sha256"], attempts[0]["equivalence_detail"])

    def test_full_match_not_affected_by_stale_cert(self) -> None:
        stale = _certificate("full", architecture="broadway-ppc32-be-v18")
        valid = _certificate("ok")
        self._write_targets(
            [
                self._row("full", status="FULL_MATCH", certificate=stale),
                self._row("ok", status="EQUIVALENT_MATCH", certificate=valid),
            ]
        )

        report = audit_promotion_registry(self.config, apply=True)
        self.assertEqual(report["skipped_full_match"], 1)
        self.assertEqual(report["affected_count"], 0)
        self.assertEqual(report["valid_count"], 1)

        rows = {row["id"]: row for row in load_targets_document(self.config)["targets"]}
        self.assertEqual(rows["full"]["status"], "FULL_MATCH")
        self.assertEqual(rows["full"]["workflow_status"], "ACCEPTED")
        self.assertEqual(rows["ok"]["status"], "EQUIVALENT_MATCH")

        errors = validate_targets(self.config)
        self.assertEqual(errors, [])

    def test_dry_run_does_not_mutate(self) -> None:
        stale = _certificate("stale", architecture="broadway-ppc32-be-v18")
        self._write_targets(
            [self._row("stale", status="EQUIVALENT_MATCH", certificate=stale)]
        )
        before = (self.root / "tools/coop/targets.json").read_text(encoding="utf-8")
        report = audit_promotion_registry(self.config, apply=False)
        self.assertEqual(report["affected_count"], 1)
        after = (self.root / "tools/coop/targets.json").read_text(encoding="utf-8")
        self.assertEqual(before, after)
        self.assertEqual(read_attempts(self.config.resolve(self.config.attempt_log)), [])

    def test_idempotent_second_apply(self) -> None:
        stale = _certificate("stale", architecture="broadway-ppc32-be-v18")
        self._write_targets(
            [self._row("stale", status="EQUIVALENT_MATCH", certificate=stale)]
        )
        first = audit_promotion_registry(self.config, apply=True)
        self.assertEqual(first["applied_count"], 1)
        second = audit_promotion_registry(self.config, apply=True)
        self.assertEqual(second["affected_count"], 0)
        self.assertEqual(second["applied_count"], 0)
        self.assertEqual(len(read_attempts(self.config.resolve(self.config.attempt_log))), 1)

    def test_update_target_result_stores_confidence_and_policy(self) -> None:
        cert = _certificate("promo")
        self._write_targets(
            [self._row("promo", status="CODE_MATCH", certificate=None, workflow_status="ACTIVE")]
        )
        update_target_result(
            self.config,
            "promo",
            status="EQUIVALENT_MATCH",
            instruction_match=81.0,
            equivalence_status="equivalent",
            equivalence_certificate=cert,
            certificate_checked=True,
            equivalence_confidence="B",
            equivalence_policy=EQUIVALENCE_PROMOTION_POLICY,
        )
        row = load_targets_document(self.config)["targets"][0]
        self.assertEqual(row["status"], "EQUIVALENT_MATCH")
        self.assertEqual(row["equivalence_confidence"], "B")
        self.assertEqual(row["equivalence_policy"], EQUIVALENCE_PROMOTION_POLICY)
        self.assertEqual(row["workflow_status"], "ACCEPTED")


if __name__ == "__main__":
    unittest.main()
