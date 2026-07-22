"""Phase 12 scalar FP v2 rollout plumbing tests."""

from __future__ import annotations

import os
import unittest
from pathlib import Path

from tools.ppc_equivalence.fp_capabilities import (
    SCALAR_FP_EXACT_V2_ENV,
    set_scalar_fp_exact_v2_module_flag,
    set_scalar_fp_exact_v2_production_module_flag,
)
from tools.ppc_equivalence.scalar_fp_v2_rollout import (
    PRODUCTION_SWITCH_BLOCKERS,
    SCALAR_FP_V2_ALLOWLIST_ORDER,
    SCALAR_FP_V2_CANARY_TARGETS,
    SCALAR_FP_V2_PHASE_TEST_MODULES,
    ScalarFPExactV2ProductionError,
    ReadinessCheck,
    ReadinessReport,
    build_production_switch_checklist,
    check_experimental_models_defined,
    check_phase_test_suite_importable,
    enable_scalar_fp_exact_v2_production,
    format_readiness_report,
    list_recommended_canary_targets,
    load_scalar_fp_v2_canary_manifest,
    readiness_report,
    recommended_canary_target_ids,
    scalar_fp_v2_production_preconditions,
    validate_canary_manifest_targets,
    validate_shadow_manifest,
)

from tools.ppc_equivalence.result import ARCHITECTURE_MODEL

_MANIFEST_EXAMPLE = (
    Path(__file__).resolve().parents[2]
    / "coop"
    / "capability_manifest.scalar_fp_v2_canary.json.example"
)
_MANIFEST_CANARY = (
    Path(__file__).resolve().parents[2]
    / "coop"
    / "capability_manifest.scalar_fp_v2_canary.json"
)


class ScalarFPV2RolloutTests(unittest.TestCase):
    def setUp(self) -> None:
        set_scalar_fp_exact_v2_module_flag(None)
        set_scalar_fp_exact_v2_production_module_flag(None)

    def tearDown(self) -> None:
        set_scalar_fp_exact_v2_module_flag(None)
        set_scalar_fp_exact_v2_production_module_flag(None)

    def test_canary_targets_match_census_notes(self) -> None:
        self.assertEqual(
            recommended_canary_target_ids()["fp-load-store"],
            "us-80040998",
        )
        self.assertEqual(
            recommended_canary_target_ids()["fp-bitwise"],
            "us-803e9714",
        )
        self.assertEqual(
            recommended_canary_target_ids()["fp-convert"],
            "us-802c9b90",
        )
        self.assertEqual(
            recommended_canary_target_ids()["fp-fpscr-control"],
            "us-802dd650",
        )

    def test_allowlist_order_matches_scalar_fp_v2_doc(self) -> None:
        self.assertEqual(
            SCALAR_FP_V2_ALLOWLIST_ORDER[:4],
            (
                "fp-load-store",
                "fp-compare",
                "fp-convert",
                "fp-fpscr-control",
            ),
        )
        self.assertEqual(SCALAR_FP_V2_ALLOWLIST_ORDER[-1], "fp-fused-arithmetic")

    def test_list_recommended_canary_targets_includes_all_census(self) -> None:
        rows = list_recommended_canary_targets()
        ids = {row["target_id"] for row in rows}
        self.assertTrue(set(SCALAR_FP_V2_CANARY_TARGETS.values()) <= ids)

    def test_shadow_manifest_example_is_default_off(self) -> None:
        manifest = load_scalar_fp_v2_canary_manifest(_MANIFEST_EXAMPLE)
        self.assertFalse(manifest.get("automatic_promotion"))
        self.assertTrue(manifest.get("shadow_mode"))
        allowlists = manifest.get("allowed_tier_a_capabilities") or {}
        fp_caps = (
            "fp-load-store",
            "fp-fused-arithmetic",
            "fp-scalar-arithmetic",
        )
        for cap in fp_caps:
            self.assertEqual(allowlists.get(cap), [])

    def test_canary_manifest_allowlists_fp_load_store_only(self) -> None:
        manifest = load_scalar_fp_v2_canary_manifest(_MANIFEST_CANARY)
        allowlists = manifest.get("allowed_tier_a_capabilities") or {}
        self.assertEqual(
            allowlists.get("fp-load-store"),
            ["broadway-fp-load-store-v2"],
        )
        for cap in ("fp-compare", "fp-convert", "fp-fused-arithmetic"):
            self.assertEqual(allowlists.get(cap), [])

    def test_production_preconditions_fail_by_default(self) -> None:
        env_backup = os.environ.pop(SCALAR_FP_EXACT_V2_ENV, None)
        prod_backup = os.environ.pop("SCALAR_FP_EXACT_V2_PRODUCTION", None)
        try:
            ok, reasons = scalar_fp_v2_production_preconditions(
                manifest=load_scalar_fp_v2_canary_manifest(_MANIFEST_EXAMPLE),
            )
            self.assertFalse(ok)
            self.assertTrue(any(SCALAR_FP_EXACT_V2_ENV in r for r in reasons))
        finally:
            if env_backup is not None:
                os.environ[SCALAR_FP_EXACT_V2_ENV] = env_backup
            if prod_backup is not None:
                os.environ["SCALAR_FP_EXACT_V2_PRODUCTION"] = prod_backup

    def test_enable_production_raises_when_gates_unmet(self) -> None:
        with self.assertRaises(ScalarFPExactV2ProductionError):
            enable_scalar_fp_exact_v2_production(manifest_path=_MANIFEST_EXAMPLE)

    def test_enable_production_succeeds_when_gates_met(self) -> None:
        set_scalar_fp_exact_v2_module_flag(True)
        os.environ["SCALAR_FP_EXACT_V2_PRODUCTION"] = "1"
        try:
            result = enable_scalar_fp_exact_v2_production(
                manifest_path=_MANIFEST_CANARY,
                run_corpus_check=False,
            )
            self.assertTrue(result["enabled"])
            self.assertIn("fp-load-store", result["enabled_capabilities"])
            self.assertIn("checklist", result)
            self.assertIn("operator_steps", result)
        finally:
            os.environ.pop("SCALAR_FP_EXACT_V2_PRODUCTION", None)
            set_scalar_fp_exact_v2_production_module_flag(None)

    def test_build_production_switch_checklist_structure(self) -> None:
        checklist = build_production_switch_checklist(run_corpus_check=False)
        self.assertFalse(checklist.production_switch_ready)
        self.assertFalse(checklist.all_gates_pass)
        gate_names = {item.name for item in checklist.gates}
        self.assertIn("infrastructure_readiness", gate_names)
        self.assertIn("architecture_model_bump", gate_names)
        payload = checklist.to_dict()
        self.assertEqual(payload["planned"]["architecture_model"], "broadway-ppc32-be-v43")
        self.assertEqual(payload["live"]["architecture_model"], ARCHITECTURE_MODEL)

    def test_readiness_report_structure(self) -> None:
        report = readiness_report(run_corpus_check=False)
        self.assertIsInstance(report, ReadinessReport)
        self.assertEqual(report.schema_version, 1)
        self.assertFalse(report.production_switch_ready)
        self.assertFalse(report.production_enabled)
        self.assertGreaterEqual(len(report.checks), 8)
        check_names = {item.name for item in report.checks}
        self.assertTrue(
            {
                "corpus_check",
                "phase_test_suite",
                "experimental_models",
                "canary_targets",
                "shadow_manifest",
                "canary_manifest_targets",
                "fe0_fe1_status",
                "unsupported_query_helper",
            }
            <= check_names,
        )
        for item in report.checks:
            self.assertIsInstance(item, ReadinessCheck)
            self.assertIsInstance(item.ok, bool)
            self.assertIsInstance(item.summary, str)
        self.assertTrue(report.blockers)
        payload = report.to_dict()
        self.assertIn("checks", payload)
        self.assertIn("blockers", payload)
        self.assertFalse(payload["production_switch_ready"])

    def test_readiness_report_blockers_are_honest(self) -> None:
        text = " ".join(PRODUCTION_SWITCH_BLOCKERS)
        self.assertIn("recertification", text)
        self.assertNotIn("NotImplemented", text)
        self.assertNotIn("ARCHITECTURE_MODEL", text)

    def test_canary_manifest_targets_dry_run(self) -> None:
        manifest = load_scalar_fp_v2_canary_manifest(_MANIFEST_CANARY)
        ok, reasons = validate_canary_manifest_targets(manifest)
        self.assertTrue(ok, msg=str(reasons))

    def test_phase_test_modules_importable(self) -> None:
        ok, failures = check_phase_test_suite_importable()
        self.assertTrue(ok, msg=str(failures))
        self.assertEqual(len(SCALAR_FP_V2_PHASE_TEST_MODULES), 13)

    def test_experimental_models_cover_allowlist_order(self) -> None:
        ok, missing = check_experimental_models_defined()
        self.assertTrue(ok, msg=str(missing))
        for capability in SCALAR_FP_V2_ALLOWLIST_ORDER:
            self.assertNotIn(capability, missing)

    def test_shadow_manifest_validates(self) -> None:
        manifest = load_scalar_fp_v2_canary_manifest(_MANIFEST_CANARY)
        ok, reasons = validate_shadow_manifest(manifest)
        self.assertTrue(ok, msg=str(reasons))

    def test_format_readiness_report_includes_blockers(self) -> None:
        report = readiness_report(run_corpus_check=False)
        text = format_readiness_report(report)
        self.assertIn("production_switch_ready=False", text)
        self.assertIn("blockers", text)


if __name__ == "__main__":
    unittest.main()
