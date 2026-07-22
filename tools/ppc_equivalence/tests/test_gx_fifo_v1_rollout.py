"""Wave 5 GX FIFO Tier-A v1 rollout plumbing tests (GX_FIFO_TIER_A.md)."""

from __future__ import annotations

import json
import unittest
from pathlib import Path

from tools.ppc_equivalence.gx_fifo_v1_rollout import (
    DEFAULT_POLICY_MANIFESTS,
    GX_FIFO_HARDWARE_PROFILE,
    GX_FIFO_MODEL_VERSIONS,
    GX_FIFO_V1_ALLOWLIST_ORDER,
    GX_FIFO_V1_STAGES,
    GX_FIFO_V1_TEST_MODULES,
    READY_LEDGER_STATUSES,
    ReadinessCheck,
    ReadinessReport,
    RolloutStage,
    apply_canary_stage,
    build_stage_manifest,
    check_all_stage_manifests,
    check_default_manifests_still_empty,
    check_gx_fifo_test_modules_importable,
    check_hardware_profile_reviewed,
    check_validation_ledger_ready,
    format_readiness_report,
    load_canary_manifest,
    ordered_stage_names,
    readiness_report,
    stage_by_name,
    validate_gx_fifo_manifest,
    validate_stage_manifest,
)
from tools.ppc_equivalence.result import ARCHITECTURE_MODEL

_COOP_DIR = Path(__file__).resolve().parents[2] / "coop"


class StageOrderingTests(unittest.TestCase):
    def test_stage_order_matches_tier_a_doc(self) -> None:
        self.assertEqual(
            ordered_stage_names(),
            (
                "land",
                "gx_fifo_write_trace_v2",
                "gx_fifo_read_v1",
                "gx_fifo_loop_emission_v1",
            ),
        )

    def test_allowlist_order_matches_tier_a_doc(self) -> None:
        self.assertEqual(
            GX_FIFO_V1_ALLOWLIST_ORDER,
            ("gx-fifo-write-trace", "gx-fifo-read", "mmio-loop-emission"),
        )

    def test_model_versions_match_mmio_capability_obligations(self) -> None:
        self.assertEqual(GX_FIFO_MODEL_VERSIONS["gx-fifo-write-trace"], "gx-fifo-trace-v2")
        self.assertEqual(GX_FIFO_MODEL_VERSIONS["gx-fifo-read"], "gx-fifo-read-v1")
        self.assertEqual(GX_FIFO_MODEL_VERSIONS["mmio-loop-emission"], "mmio-loop-emission-v1")

    def test_stages_accumulate_monotonically(self) -> None:
        seen: set[str] = set()
        for stage in GX_FIFO_V1_STAGES:
            self.assertTrue(seen <= set(stage.cumulative_capabilities))
            seen = set(stage.cumulative_capabilities)
        self.assertEqual(seen, set(GX_FIFO_V1_ALLOWLIST_ORDER))

    def test_land_stage_has_no_capability(self) -> None:
        land = stage_by_name("land")
        self.assertIsNone(land.capability)
        self.assertIsNone(land.model_version)
        self.assertEqual(land.cumulative_capabilities, ())

    def test_stage_by_name_unknown_raises(self) -> None:
        with self.assertRaises(KeyError):
            stage_by_name("does-not-exist")

    def test_automatic_promotion_always_false_by_construction(self) -> None:
        # RolloutStage carries no automatic_promotion field at all — the
        # generated/checked-in manifests are the only place it can appear,
        # and every manifest validator rejects anything but False.
        for stage in GX_FIFO_V1_STAGES:
            self.assertFalse(hasattr(stage, "automatic_promotion"))


class CanaryManifestContentTests(unittest.TestCase):
    def test_every_stage_manifest_file_exists_and_validates(self) -> None:
        ok, failures = check_all_stage_manifests()
        self.assertTrue(ok, msg=str(failures))

    def test_land_manifest_allowlists_empty_and_shadow_mode_true(self) -> None:
        land = stage_by_name("land")
        manifest = load_canary_manifest(land.manifest_path)
        self.assertTrue(manifest.get("shadow_mode"))
        self.assertIs(manifest.get("automatic_promotion"), False)
        allowlists = manifest.get("allowed_tier_a_capabilities") or {}
        for capability in GX_FIFO_V1_ALLOWLIST_ORDER:
            self.assertEqual(allowlists.get(capability), [])

    def test_write_trace_stage_allowlists_only_write_trace(self) -> None:
        stage = stage_by_name("gx_fifo_write_trace_v2")
        manifest = load_canary_manifest(stage.manifest_path)
        allowlists = manifest.get("allowed_tier_a_capabilities") or {}
        self.assertEqual(allowlists.get("gx-fifo-write-trace"), ["gx-fifo-trace-v2"])
        self.assertEqual(allowlists.get("gx-fifo-read"), [])
        self.assertEqual(allowlists.get("mmio-loop-emission"), [])

    def test_read_stage_allowlists_write_trace_and_read(self) -> None:
        stage = stage_by_name("gx_fifo_read_v1")
        manifest = load_canary_manifest(stage.manifest_path)
        allowlists = manifest.get("allowed_tier_a_capabilities") or {}
        self.assertEqual(allowlists.get("gx-fifo-write-trace"), ["gx-fifo-trace-v2"])
        self.assertEqual(allowlists.get("gx-fifo-read"), ["gx-fifo-read-v1"])
        self.assertEqual(allowlists.get("mmio-loop-emission"), [])

    def test_loop_emission_stage_allowlists_all_three(self) -> None:
        stage = stage_by_name("gx_fifo_loop_emission_v1")
        manifest = load_canary_manifest(stage.manifest_path)
        allowlists = manifest.get("allowed_tier_a_capabilities") or {}
        self.assertEqual(allowlists.get("gx-fifo-write-trace"), ["gx-fifo-trace-v2"])
        self.assertEqual(allowlists.get("gx-fifo-read"), ["gx-fifo-read-v1"])
        self.assertEqual(allowlists.get("mmio-loop-emission"), ["mmio-loop-emission-v1"])

    def test_non_land_stages_are_not_shadow_mode(self) -> None:
        for stage in GX_FIFO_V1_STAGES:
            if stage.name == "land":
                continue
            manifest = load_canary_manifest(stage.manifest_path)
            self.assertFalse(manifest.get("shadow_mode"))
            self.assertTrue(manifest.get("require_capability_assurance"))

    def test_every_stage_manifest_automatic_promotion_false(self) -> None:
        for stage in GX_FIFO_V1_STAGES:
            manifest = load_canary_manifest(stage.manifest_path)
            self.assertIs(manifest.get("automatic_promotion"), False)

    def test_stage_manifest_metadata_matches_stage_name(self) -> None:
        for stage in GX_FIFO_V1_STAGES:
            manifest = load_canary_manifest(stage.manifest_path)
            meta = manifest.get("gx_fifo_exact_v1") or {}
            self.assertEqual(meta.get("stage"), stage.name)
            self.assertEqual(meta.get("hardware_profile"), GX_FIFO_HARDWARE_PROFILE)


class ManifestValidatorTests(unittest.TestCase):
    def test_validate_gx_fifo_manifest_rejects_automatic_promotion_true(self) -> None:
        manifest = load_canary_manifest(stage_by_name("land").manifest_path)
        mutated = dict(manifest)
        mutated["automatic_promotion"] = True
        ok, reasons = validate_gx_fifo_manifest(mutated)
        self.assertFalse(ok)
        self.assertTrue(any("automatic_promotion" in r for r in reasons))

    def test_validate_gx_fifo_manifest_rejects_missing_metadata(self) -> None:
        manifest = load_canary_manifest(stage_by_name("land").manifest_path)
        mutated = dict(manifest)
        mutated.pop("gx_fifo_exact_v1", None)
        ok, reasons = validate_gx_fifo_manifest(mutated)
        self.assertFalse(ok)
        self.assertTrue(any("gx_fifo_exact_v1" in r for r in reasons))

    def test_validate_stage_manifest_detects_wrong_allowlist(self) -> None:
        stage = stage_by_name("gx_fifo_write_trace_v2")
        manifest = load_canary_manifest(stage.manifest_path)
        mutated = json.loads(json.dumps(manifest))
        mutated["allowed_tier_a_capabilities"]["gx-fifo-read"] = ["gx-fifo-read-v1"]
        ok, reasons = validate_stage_manifest(stage, mutated)
        self.assertFalse(ok)
        self.assertTrue(any("gx-fifo-read" in r for r in reasons))

    def test_build_stage_manifest_matches_checked_in_file(self) -> None:
        for stage in GX_FIFO_V1_STAGES:
            if stage.name == "land":
                continue
            built = build_stage_manifest(stage)
            on_disk = load_canary_manifest(stage.manifest_path)
            self.assertEqual(
                built.get("allowed_tier_a_capabilities"),
                on_disk.get("allowed_tier_a_capabilities"),
            )
            self.assertEqual(built.get("automatic_promotion"), False)
            self.assertEqual(built.get("shadow_mode"), on_disk.get("shadow_mode"))

    def test_apply_canary_refuses_default_manifests(self) -> None:
        from tools.ppc_equivalence.gx_fifo_v1_rollout import PROTECTED_POLICY_MANIFESTS

        for path in PROTECTED_POLICY_MANIFESTS:
            with self.assertRaises(ValueError):
                apply_canary_stage("gx_fifo_write_trace_v2", out_path=path)


class DefaultPolicyUnchangedTests(unittest.TestCase):
    def test_shadow_default_manifest_keeps_gx_allowlists_empty(self) -> None:
        ok, reasons = check_default_manifests_still_empty()
        self.assertTrue(ok, msg=str(reasons))

    def test_authoritative_manifest_is_gx_graduated(self) -> None:
        from tools.ppc_equivalence.gx_fifo_v1_rollout import (
            check_authoritative_gx_graduation,
        )

        ok, reasons = check_authoritative_gx_graduation()
        self.assertTrue(ok, msg=str(reasons))

    def test_default_manifest_paths_are_the_real_files(self) -> None:
        names = {path.name for path in DEFAULT_POLICY_MANIFESTS}
        self.assertEqual(names, {"capability_manifest.json"})
        for path in DEFAULT_POLICY_MANIFESTS:
            self.assertTrue(path.is_file())
            self.assertEqual(path.parent, _COOP_DIR)

    def test_apply_canary_refuses_protected_manifests(self) -> None:
        from tools.ppc_equivalence.gx_fifo_v1_rollout import PROTECTED_POLICY_MANIFESTS

        for path in PROTECTED_POLICY_MANIFESTS:
            with self.assertRaises(ValueError):
                apply_canary_stage("gx_fifo_write_trace_v2", out_path=path)


class ReadinessGateTests(unittest.TestCase):
    def test_validation_ledger_ready_for_all_three_capabilities(self) -> None:
        ok, statuses, reasons = check_validation_ledger_ready()
        self.assertTrue(ok, msg=str(reasons))
        for capability in GX_FIFO_V1_ALLOWLIST_ORDER:
            self.assertIn(statuses[capability], READY_LEDGER_STATUSES)

    def test_hardware_profile_is_reviewed(self) -> None:
        ok, reasons = check_hardware_profile_reviewed()
        self.assertTrue(ok, msg=str(reasons))

    def test_gx_fifo_test_modules_importable(self) -> None:
        ok, failures = check_gx_fifo_test_modules_importable()
        self.assertTrue(ok, msg=str(failures))
        self.assertGreaterEqual(len(GX_FIFO_V1_TEST_MODULES), 4)

    def test_readiness_report_structure(self) -> None:
        report = readiness_report()
        self.assertIsInstance(report, ReadinessReport)
        self.assertEqual(report.schema_version, 1)
        self.assertTrue(report.infrastructure_ready)
        check_names = {item.name for item in report.checks}
        self.assertTrue(
            {
                "gx_fifo_test_modules",
                "validation_ledger",
                "hardware_profile",
                "stage_manifests",
                "default_manifests_unchanged",
                "authoritative_gx_graduated",
            }
            <= check_names,
        )
        for item in report.checks:
            self.assertIsInstance(item, ReadinessCheck)
            self.assertIsInstance(item.ok, bool)
        payload = report.to_dict()
        self.assertIn("checks", payload)
        self.assertIn("blockers", payload)
        self.assertTrue(payload["infrastructure_ready"])

    def test_readiness_blockers_always_mention_automatic_promotion(self) -> None:
        report = readiness_report()
        text = " ".join(report.blockers)
        self.assertIn("automatic_promotion", text)

    def test_format_readiness_report_includes_summary(self) -> None:
        report = readiness_report()
        text = format_readiness_report(report)
        self.assertIn("infrastructure_ready=True", text)
        self.assertIn("blockers", text)


class ArchitectureIdentityTests(unittest.TestCase):
    def test_architecture_model_is_v43_or_later(self) -> None:
        self.assertTrue(ARCHITECTURE_MODEL.startswith("broadway-ppc32-be-v"))
        version = int(ARCHITECTURE_MODEL.rsplit("v", 1)[1])
        self.assertGreaterEqual(version, 43)


if __name__ == "__main__":
    unittest.main()
