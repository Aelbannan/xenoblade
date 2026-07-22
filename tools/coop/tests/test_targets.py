from __future__ import annotations

import json
import tempfile
import unittest
from pathlib import Path

from tools.coop.lib.config import CoopConfig
from tools.coop.lib.targets import (
    EQUIVALENCE_CERTIFICATE_VERSION,
    Target,
    claim_target,
    equivalence_certificate_error,
    harness_targets,
    import_symbols,
    load_split_ranges,
    parse_asm_calls,
    plan_recertify_bottom_up,
    recertify_ready_wave,
    release_target,
    validate_targets,
    equivalence_certificate_hash,
)
from tools.ppc_equivalence.provenance import hash_certifier_tree, hash_engine_tree
from tools.ppc_equivalence.result import ARCHITECTURE_MODEL, RESULT_FORMAT

_REPO_ROOT = Path(__file__).resolve().parents[3]


def _live_engine_hash() -> str:
    return hash_engine_tree(_REPO_ROOT)


def _live_certifier_hash() -> str:
    return hash_certifier_tree(_REPO_ROOT)


def _attestation_model_version(capability: str) -> str:
    from tools.ppc_equivalence.fp_capabilities import model_version_for_capability

    return model_version_for_capability(capability) or f"{capability}-v0"


def _attestation_algorithm(capability: str) -> str:
    from tools.ppc_equivalence.capability_attachment import (
        FP_COMPARE_ALGORITHM,
        FP_CONVERT_ALGORITHM,
        FP_LOAD_STORE_ALGORITHM,
    )
    from tools.ppc_equivalence.certified_calls_obligations import (
        CERTIFIED_CALLS_ALGORITHM,
    )

    return {
        "integer-core": "opcode-ledger-v2",
        "provenance": "provenance-binding-v1",
        "certified-calls": CERTIFIED_CALLS_ALGORITHM,
        "fp-load-store": FP_LOAD_STORE_ALGORITHM,
        "fp-compare": FP_COMPARE_ALGORITHM,
        "fp-convert": FP_CONVERT_ALGORITHM,
    }.get(capability, f"{capability}-incomplete-v0")


def _certificate(target_id: str, callees: list[dict[str, str]] | None = None) -> dict:
    certificate = {
        "version": EQUIVALENCE_CERTIFICATE_VERSION,
        "status": "SEMANTIC_CERTIFIED",
        "architecture": ARCHITECTURE_MODEL,
        "result_format": RESULT_FORMAT,
        "target_id": target_id,
        "evidence": "symbolic-equivalence",
        "retail_sha256": "1" * 64,
        "candidate_sha256": "2" * 64,
        "summary": {"reads": ["r3"], "writes": ["r3"], "return_behavior": "normal"},
        "callees": callees or [],
        "helpers": [],
        "engine_hash": _live_engine_hash(),
        "certifier_hash": _live_certifier_hash(),
        "opcodes_used": ["addi", "blr"],
    }
    from tools.coop.lib.equivalence_policy import proof_result_from_certificate
    from tools.ppc_equivalence.capability_assurance import (
        CapabilityAssurance,
        build_attestation,
    )
    from tools.ppc_equivalence.capability_requirements import (
        derive_capability_requirements,
    )
    from tools.ppc_equivalence.result import ProofStatus

    proof = proof_result_from_certificate(ProofStatus.EQUIVALENT, certificate)
    requirements = derive_capability_requirements(proof)
    by_cap = requirements.by_capability()
    attestations = []
    for capability, requirement in by_cap.items():
        attestations.append(
            build_attestation(
                capability=capability,
                model_version=_attestation_model_version(capability),
                algorithm=_attestation_algorithm(capability),
                evidence={
                    "opcodes": list(requirement.required_opcodes),
                    "requirement_sha256": requirement.requirement_sha256,
                    "requirements_sha256": requirements.requirements_sha256,
                },
            )
        )
    certificate["capability_requirements"] = requirements.to_dict()
    certificate["capability_assurance"] = CapabilityAssurance(
        capabilities=tuple(attestations),
    ).to_dict()
    certificate["certificate_sha256"] = equivalence_certificate_hash(certificate)
    return certificate


class _Unit:
    def __init__(self, root: Path) -> None:
        self.name = "main/foo/Bar"
        self.source_path = root / "src/foo/Bar.cpp"


class _Project:
    def __init__(self, root: Path) -> None:
        self.root = root

    def load_objdiff_units(self):
        return [_Unit(self.root)]


class TargetRegistryTests(unittest.TestCase):
    def setUp(self) -> None:
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name)
        (self.root / "tools/coop").mkdir(parents=True)
        (self.root / "config/us").mkdir(parents=True)
        (self.root / "src/foo").mkdir(parents=True)
        (self.root / "src/foo/Bar.cpp").touch()
        self.config = CoopConfig(project_root=self.root, region="us")
        (self.root / "tools/coop/targets.json").write_text(
            json.dumps(
                {
                    "schema_version": 2,
                    "default_required_level": "EQUIVALENT_MATCH",
                    "targets": [],
                }
            ),
            encoding="utf-8",
        )

    def tearDown(self) -> None:
        self.tmp.cleanup()

    def test_split_parser_and_function_import_are_idempotent(self) -> None:
        (self.root / "config/us/splits.txt").write_text(
            "foo/Bar.cpp:\n\t.text start:0x80001000 end:0x80001100\n",
            encoding="utf-8",
        )
        (self.root / "config/us/symbols.txt").write_text(
            "func__3BarFv = .text:0x80001020; // type:function size:0x20 scope:global\n"
            "global = .data:0x80500000; // type:object size:0x4 scope:global\n",
            encoding="utf-8",
        )
        ranges = load_split_ranges(self.root / "config/us/splits.txt")
        self.assertEqual(ranges[0].unit, "foo/Bar.cpp")

        data, added, skipped = import_symbols(_Project(self.root), self.config)
        self.assertEqual((added, skipped), (1, 0))
        row = data["targets"][0]
        self.assertEqual(row["id"], "us-80001020")
        self.assertEqual(row["unit"], "foo/Bar")
        self.assertEqual(row["source"], "src/foo/Bar.cpp")

        (self.root / "tools/coop/targets.json").write_text(
            json.dumps(data), encoding="utf-8"
        )
        _data, added, skipped = import_symbols(_Project(self.root), self.config)
        self.assertEqual((added, skipped), (0, 1))

    def test_all_import_includes_data_symbols(self) -> None:
        (self.root / "config/us/splits.txt").write_text("", encoding="utf-8")
        (self.root / "config/us/symbols.txt").write_text(
            "f = .text:0x80001000; // type:function size:0x4 scope:global\n"
            "g = .data:0x80500000; // type:object size:0x4 scope:global\n",
            encoding="utf-8",
        )
        data, added, _skipped = import_symbols(
            _Project(self.root), self.config, kind="all"
        )
        self.assertEqual(added, 2)
        self.assertEqual({row["kind"] for row in data["targets"]}, {"function", "object"})

    def test_aliases_at_same_address_are_all_imported(self) -> None:
        (self.root / "config/us/splits.txt").write_text("", encoding="utf-8")
        (self.root / "config/us/symbols.txt").write_text(
            "first = .text:0x80001000; // type:function size:0x4 scope:global\n"
            "alias = .text:0x80001000; // type:function size:0x4 scope:global\n",
            encoding="utf-8",
        )
        data, added, _skipped = import_symbols(_Project(self.root), self.config)
        self.assertEqual(added, 2)
        self.assertEqual({row["id"] for row in data["targets"]}, {"us-80001000", "us-80001000-2"})

    def test_validation_rejects_duplicate_symbol_identity(self) -> None:
        rows = [
            {"id": "a", "symbol": "f", "address": "0x1"},
            {"id": "b", "symbol": "f", "address": "0x1"},
        ]
        (self.root / "tools/coop/targets.json").write_text(
            json.dumps({"schema_version": 2, "targets": rows}), encoding="utf-8"
        )
        errors = validate_targets(self.config)
        self.assertTrue(any("duplicate symbol identity" in error for error in errors))

    def test_claim_requires_release_before_owner_changes(self) -> None:
        path = self.root / "tools/coop/targets.json"
        path.write_text(
            json.dumps(
                {
                    "schema_version": 2,
                    "targets": [
                        {"id": "a", "symbol": "f", "address": "0x1", "status": "NOT_STARTED"}
                    ],
                }
            ),
            encoding="utf-8",
        )
        claim_target(self.config, "a", owner="one", allowed_paths=["src/a.cpp"])
        with self.assertRaisesRegex(ValueError, "already claimed"):
            claim_target(self.config, "a", owner="two", allowed_paths=[])
        release_target(self.config, "a", owner="one")
        claim_target(self.config, "a", owner="two", allowed_paths=[])

    def test_parse_calls_resolves_direct_destination_and_marks_indirect(self) -> None:
        asm = self.root / "calls.s"
        asm.write_text(
            ".fn caller, global\n"
            "/* 80001000 00000000  48 00 00 09 */\tbl callee\n"
            "/* 80001004 00000004  4E 80 04 21 */\tbctrl\n"
            ".endfn caller\n",
            encoding="utf-8",
        )
        parsed = parse_asm_calls(asm)
        self.assertEqual(parsed[0].address, 0x80001000)
        self.assertEqual(parsed[0].direct, [("callee", 0x80001008)])
        self.assertTrue(parsed[0].has_indirect)

    def test_harness_bottom_up_selections(self) -> None:
        def target(target_id: str, status: str, **extra) -> Target:
            return Target(
                id=target_id,
                tier="P9",
                milestone="unassigned",
                function=target_id,
                symbol=target_id,
                address="0x1",
                source=self.root / "src/foo/Bar.cpp",
                unit="foo/Bar",
                required_level="EQUIVALENT_MATCH",
                status=status,
                extra=extra,
            )

        accepted = target(
            "accepted", "EQUIVALENT_MATCH", callgraph_status="complete",
            equivalence_certificate=_certificate("accepted"),
        )
        leaf = target("leaf", "NOT_STARTED", callgraph_status="complete", called_functions=[])
        ready = target(
            "ready",
            "NOT_STARTED",
            callgraph_status="complete",
            called_functions=["accepted"],
        )
        blocked = target(
            "blocked",
            "NOT_STARTED",
            callgraph_status="complete",
            called_functions=["leaf"],
        )
        indirect = target(
            "indirect",
            "NOT_STARTED",
            callgraph_status="complete",
            called_functions=[],
            has_indirect_calls=True,
        )
        unresolved = target(
            "unresolved",
            "NOT_STARTED",
            callgraph_status="complete",
            called_functions=[],
            unresolved_called_functions=["unknown"],
        )
        rows = [accepted, leaf, ready, blocked, indirect, unresolved]
        self.assertEqual(
            [item.id for item in harness_targets(rows, selection="leaf")], ["leaf"]
        )
        self.assertEqual(
            [item.id for item in harness_targets(rows, selection="callees-accepted")],
            ["ready"],
        )
        self.assertEqual(
            [item.id for item in harness_targets(rows, selection="ready")],
            ["leaf", "ready"],
        )

    def test_validation_rejects_stale_certificate_dependency(self) -> None:
        callee = {
            "id": "callee", "symbol": "callee", "address": "0x1",
            "status": "EQUIVALENT_MATCH",
            "equivalence_certificate": _certificate("callee"),
        }
        caller_certificate = _certificate("caller", [{
            "target_id": "callee",
            "certificate_sha256": "f" * 64,
        }])
        rows = [
            callee,
            {
                "id": "caller", "symbol": "caller", "address": "0x2",
                "status": "EQUIVALENT_MATCH",
                "called_functions": ["callee"],
                "equivalence_certificate": caller_certificate,
            },
        ]
        (self.root / "tools/coop/targets.json").write_text(
            json.dumps({"schema_version": 2, "targets": rows}), encoding="utf-8",
        )
        errors = validate_targets(self.config)
        self.assertTrue(any("callee certificate changed" in error for error in errors))

    def _make_row(self, target_id: str, certificate: dict | None) -> dict:
        return {
            "id": target_id,
            "symbol": target_id,
            "address": "0x1",
            "equivalence_certificate": certificate,
        }

    def _rows_by_id(self, rows: list[dict]) -> dict[str, dict]:
        return {row["id"]: row for row in rows}

    def test_old_architecture_model_certificate_rejected(self) -> None:
        cert = _certificate("x")
        cert["architecture"] = "broadway-ppc32-be-v18"
        cert["certificate_sha256"] = equivalence_certificate_hash(cert)
        row = self._make_row("x", cert)
        err = equivalence_certificate_error(row, self._rows_by_id([row]))
        self.assertIsNotNone(err)
        self.assertIn("architecture", err or "")

    def test_old_result_format_certificate_rejected(self) -> None:
        cert = _certificate("x")
        cert["result_format"] = 7
        cert["certificate_sha256"] = equivalence_certificate_hash(cert)
        row = self._make_row("x", cert)
        err = equivalence_certificate_error(row, self._rows_by_id([row]))
        self.assertIsNotNone(err)
        self.assertIn("result_format", err or "")

    def test_wrong_certificate_version_rejected(self) -> None:
        cert = _certificate("x")
        cert["version"] = 1
        cert["certificate_sha256"] = equivalence_certificate_hash(cert)
        row = self._make_row("x", cert)
        err = equivalence_certificate_error(row, self._rows_by_id([row]))
        self.assertIsNotNone(err)
        self.assertIn("version", err or "")

    def test_missing_engine_hash_rejected(self) -> None:
        cert = _certificate("x")
        del cert["engine_hash"]
        cert["certificate_sha256"] = equivalence_certificate_hash(cert)
        row = self._make_row("x", cert)
        err = equivalence_certificate_error(row, self._rows_by_id([row]))
        self.assertIsNotNone(err)
        self.assertIn("engine_hash", err or "")

    def test_wrong_engine_hash_rejected(self) -> None:
        cert = _certificate("x")
        cert["engine_hash"] = "a" * 64
        cert["certificate_sha256"] = equivalence_certificate_hash(cert)
        row = self._make_row("x", cert)
        err = equivalence_certificate_error(row, self._rows_by_id([row]))
        self.assertIsNotNone(err)
        self.assertIn("engine_hash", err or "")

    def test_missing_certifier_hash_rejected(self) -> None:
        cert = _certificate("x")
        del cert["certifier_hash"]
        cert["certificate_sha256"] = equivalence_certificate_hash(cert)
        row = self._make_row("x", cert)
        err = equivalence_certificate_error(row, self._rows_by_id([row]))
        self.assertIsNotNone(err)
        self.assertIn("certifier_hash", err or "")

    def test_wrong_certifier_hash_rejected(self) -> None:
        cert = _certificate("x")
        cert["certifier_hash"] = "b" * 64
        cert["certificate_sha256"] = equivalence_certificate_hash(cert)
        row = self._make_row("x", cert)
        err = equivalence_certificate_error(row, self._rows_by_id([row]))
        self.assertIsNotNone(err)
        self.assertIn("certifier_hash", err or "")

    def test_transitive_callee_old_model_rejected(self) -> None:
        callee_cert = _certificate("callee")
        callee_cert["architecture"] = "broadway-ppc32-be-v18"
        callee_cert["certificate_sha256"] = equivalence_certificate_hash(callee_cert)
        caller_cert = _certificate("caller", [{
            "target_id": "callee",
            "certificate_sha256": callee_cert["certificate_sha256"],
        }])
        rows = [
            self._make_row("callee", callee_cert),
            self._make_row("caller", caller_cert),
        ]
        err = equivalence_certificate_error(rows[1], self._rows_by_id(rows))
        self.assertIsNotNone(err)
        self.assertIn("callee", err or "")
        self.assertIn("architecture", err or "")

    def test_full_match_not_invalidated_by_stale_certificate(self) -> None:
        stale = _certificate("full")
        stale["architecture"] = "broadway-ppc32-be-v18"
        stale["certificate_sha256"] = equivalence_certificate_hash(stale)
        rows = [
            {
                "id": "full", "symbol": "f", "address": "0x1",
                "status": "FULL_MATCH",
                "equivalence_certificate": stale,
            },
        ]
        (self.root / "tools/coop/targets.json").write_text(
            json.dumps({"schema_version": 2, "targets": rows}), encoding="utf-8",
        )
        errors = validate_targets(self.config)
        full_match_errors = [e for e in errors if "full" in e]
        self.assertEqual(
            len(full_match_errors), 0,
            f"FULL_MATCH should not be flagged: {full_match_errors}",
        )

    def test_recertify_bottom_up_orders_leaves_before_callers(self) -> None:
        def target(
            target_id: str,
            status: str,
            *,
            workflow_status: str = "ACCEPTED",
            **extra,
        ) -> Target:
            return Target(
                id=target_id,
                tier="P9",
                milestone="unassigned",
                function=target_id,
                symbol=target_id,
                address="0x1",
                source=self.root / "src/foo/Bar.cpp",
                unit="foo/Bar",
                required_level="EQUIVALENT_MATCH",
                status=status,
                workflow_status=workflow_status,
                extra=extra,
            )

        leaf = target(
            "leaf",
            "FULL_MATCH",
            callgraph_status="complete",
            called_functions=[],
        )
        mid = target(
            "mid",
            "FULL_MATCH",
            callgraph_status="complete",
            called_functions=["leaf"],
        )
        root = target(
            "root",
            "EQUIVALENT_MATCH",
            callgraph_status="complete",
            called_functions=["mid"],
            equivalence_certificate=_certificate("root", [{
                "target_id": "mid",
                "certificate_sha256": "f" * 64,
            }]),
        )
        already = target(
            "already",
            "FULL_MATCH",
            callgraph_status="complete",
            called_functions=[],
            equivalence_certificate=_certificate("already"),
        )
        blocked = target(
            "blocked",
            "FULL_MATCH",
            callgraph_status="complete",
            called_functions=["missing"],
        )
        indirect = target(
            "indirect",
            "FULL_MATCH",
            callgraph_status="complete",
            called_functions=[],
            has_indirect_calls=True,
        )
        pending = target(
            "pending",
            "NOT_STARTED",
            workflow_status="QUEUED",
            callgraph_status="complete",
            called_functions=[],
        )
        rows = [root, mid, leaf, already, blocked, indirect, pending]
        plan = plan_recertify_bottom_up(rows)
        self.assertEqual([item.id for item in plan.ordered], ["leaf", "mid", "root"])
        self.assertEqual(plan.reasons["leaf"], "missing equivalence_certificate")
        self.assertIn("callee certificate changed", plan.reasons["root"])
        self.assertEqual(
            {item.id for item in plan.blocked},
            {"blocked", "indirect"},
        )
        self.assertIn("uncertified callees", plan.block_reasons["blocked"])
        self.assertIn("indirect", plan.block_reasons["indirect"])

        wave = recertify_ready_wave(rows)
        self.assertEqual([item.id for item in wave], ["leaf"])

        leaf_certified = target(
            "leaf",
            "FULL_MATCH",
            callgraph_status="complete",
            called_functions=[],
            equivalence_certificate=_certificate("leaf"),
        )
        rows_after_leaf = [root, mid, leaf_certified, already, blocked, indirect, pending]
        self.assertEqual(
            [item.id for item in recertify_ready_wave(rows_after_leaf)],
            ["mid"],
        )

        caller = target(
            "caller",
            "NOT_STARTED",
            workflow_status="QUEUED",
            callgraph_status="complete",
            called_functions=["leaf"],
        )
        frontier = harness_targets(
            [leaf_certified, caller],
            selection="callees-accepted",
            include_catalog=True,
        )
        self.assertEqual([item.id for item in frontier], ["caller"])


if __name__ == "__main__":
    unittest.main()
