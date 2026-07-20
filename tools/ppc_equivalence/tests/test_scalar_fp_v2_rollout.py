"""Phase 12 scalar FP v2 rollout plumbing tests."""

from __future__ import annotations

import json
import os
import unittest
from pathlib import Path

from tools.ppc_equivalence.fp_capabilities import (
    SCALAR_FP_EXACT_V2_ENV,
    set_scalar_fp_exact_v2_module_flag,
)
from tools.ppc_equivalence.scalar_fp_v2_rollout import (
    SCALAR_FP_V2_ALLOWLIST_ORDER,
    SCALAR_FP_V2_CANARY_TARGETS,
    ScalarFPExactV2ProductionError,
    enable_scalar_fp_exact_v2_production,
    list_recommended_canary_targets,
    load_scalar_fp_v2_canary_manifest,
    recommended_canary_target_ids,
    scalar_fp_v2_production_preconditions,
)

_MANIFEST = (
    Path(__file__).resolve().parents[2]
    / "coop"
    / "capability_manifest.scalar_fp_v2_canary.json.example"
)


class ScalarFPV2RolloutTests(unittest.TestCase):
    def setUp(self) -> None:
        set_scalar_fp_exact_v2_module_flag(None)

    def tearDown(self) -> None:
        set_scalar_fp_exact_v2_module_flag(None)

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

    def test_shadow_manifest_is_default_off(self) -> None:
        manifest = load_scalar_fp_v2_canary_manifest(_MANIFEST)
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

    def test_production_preconditions_fail_by_default(self) -> None:
        env_backup = os.environ.pop(SCALAR_FP_EXACT_V2_ENV, None)
        prod_backup = os.environ.pop("SCALAR_FP_EXACT_V2_PRODUCTION", None)
        try:
            ok, reasons = scalar_fp_v2_production_preconditions(
                manifest=load_scalar_fp_v2_canary_manifest(_MANIFEST),
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
            enable_scalar_fp_exact_v2_production(manifest_path=_MANIFEST)

    def test_enable_production_not_implemented_even_when_gates_met(self) -> None:
        set_scalar_fp_exact_v2_module_flag(True)
        os.environ["SCALAR_FP_EXACT_V2_PRODUCTION"] = "1"
        manifest = json.loads(_MANIFEST.read_text(encoding="utf-8"))
        manifest["allowed_tier_a_capabilities"]["fp-fused-arithmetic"] = [
            "broadway-fp-fused-v2"
        ]
        ok, reasons = scalar_fp_v2_production_preconditions(manifest=manifest)
        self.assertTrue(ok, msg=str(reasons))
        tmp = Path(__file__).resolve().parent / "_tmp_canary_manifest.json"
        tmp.write_text(json.dumps(manifest), encoding="utf-8")
        try:
            with self.assertRaises(NotImplementedError):
                enable_scalar_fp_exact_v2_production(manifest_path=tmp)
        finally:
            tmp.unlink(missing_ok=True)
            os.environ.pop("SCALAR_FP_EXACT_V2_PRODUCTION", None)


if __name__ == "__main__":
    unittest.main()
