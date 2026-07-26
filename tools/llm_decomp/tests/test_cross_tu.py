"""Tests for tools.llm_decomp.cross_tu — tiered dependent-sweep module.

Heavily mocks tools.llm_decomp.verify since we have no real toolchain in CI.
"""
from __future__ import annotations

import unittest
from pathlib import Path
from unittest.mock import patch

from tools.llm_decomp.config import Config


class MockFn:
    """Minimal stand-in for an ELF symbol (name + code bytes)."""

    def __init__(self, name: str, code: bytes) -> None:
        self.name = name
        self.code = code


class TestTierFor(unittest.TestCase):
    """tier_for maps dependent counts to the correct Tier."""

    def setUp(self) -> None:
        self.cfg = Config()
        self.cfg.cross_tu_full_sweep_max = 10
        self.cfg.cross_tu_refuse_over = 40

    def test_zero_dependents(self) -> None:
        from tools.llm_decomp.cross_tu import Tier, tier_for
        self.assertIs(tier_for(0, self.cfg), Tier.TU_ONLY)

    def test_at_full_sweep_max(self) -> None:
        from tools.llm_decomp.cross_tu import Tier, tier_for
        self.assertIs(tier_for(10, self.cfg), Tier.FULL_SWEEP)

    def test_just_above_full_sweep_max(self) -> None:
        from tools.llm_decomp.cross_tu import Tier, tier_for
        self.assertIs(tier_for(11, self.cfg), Tier.ACCEPTED_ONLY)

    def test_at_refuse_over(self) -> None:
        from tools.llm_decomp.cross_tu import Tier, tier_for
        self.assertIs(tier_for(40, self.cfg), Tier.ACCEPTED_ONLY)

    def test_above_refuse_over(self) -> None:
        from tools.llm_decomp.cross_tu import Tier, tier_for
        self.assertIs(tier_for(41, self.cfg), Tier.REFUSE)

    def test_custom_thresholds(self) -> None:
        from tools.llm_decomp.cross_tu import Tier, tier_for
        cfg2 = Config()
        cfg2.cross_tu_full_sweep_max = 5
        cfg2.cross_tu_refuse_over = 20
        self.assertIs(tier_for(0, cfg2), Tier.TU_ONLY)
        self.assertIs(tier_for(5, cfg2), Tier.FULL_SWEEP)
        self.assertIs(tier_for(6, cfg2), Tier.ACCEPTED_ONLY)
        self.assertIs(tier_for(20, cfg2), Tier.ACCEPTED_ONLY)
        self.assertIs(tier_for(21, cfg2), Tier.REFUSE)


class TestDependentUnits(unittest.TestCase):
    """dependent_units maps source paths to sorted, deduplicated unit hints."""

    def test_src_path(self) -> None:
        from tools.llm_decomp.cross_tu import dependent_units
        result = dependent_units({"src/kyoshin/cf/X.cpp"})
        self.assertEqual(result, ["kyoshin/cf/X"])

    def test_libs_path(self) -> None:
        from tools.llm_decomp.cross_tu import dependent_units
        result = dependent_units({"libs/monolib/src/scn/Y.cpp"})
        self.assertEqual(result, ["monolib/src/scn/Y"])

    def test_unknown_prefix(self) -> None:
        from tools.llm_decomp.cross_tu import dependent_units
        result = dependent_units({"other/path/Z.cpp"})
        self.assertEqual(result, ["other/path/Z"])

    def test_deduplication(self) -> None:
        from tools.llm_decomp.cross_tu import dependent_units
        deps = {"src/kyoshin/cf/A.cpp", "src/kyoshin/cf/A.cpp", "libs/monolib/src/scn/B.cpp"}
        result = dependent_units(deps)
        self.assertEqual(result, ["kyoshin/cf/A", "monolib/src/scn/B"])

    def test_sorting(self) -> None:
        from tools.llm_decomp.cross_tu import dependent_units
        deps = {"src/zebra/Z.cpp", "src/alpha/A.cpp", "src/beta/B.cpp"}
        result = dependent_units(deps)
        self.assertEqual(result, ["alpha/A", "beta/B", "zebra/Z"])

    def test_multiple_extensions(self) -> None:
        from tools.llm_decomp.cross_tu import dependent_units
        result = dependent_units({"src/kyoshin/cf/Foo.cxx"})
        self.assertEqual(result, ["kyoshin/cf/Foo"])

    def test_no_extension(self) -> None:
        from tools.llm_decomp.cross_tu import dependent_units
        result = dependent_units({"src/kyoshin/cf/Baz"})
        self.assertEqual(result, ["kyoshin/cf/Baz"])

    def test_empty_set(self) -> None:
        from tools.llm_decomp.cross_tu import dependent_units
        result = dependent_units(set())
        self.assertEqual(result, [])


class TestHasHardFailure(unittest.TestCase):
    """has_hard_failure detects build failures and protected-symbol regressions."""

    def test_no_failure_empty(self) -> None:
        from tools.llm_decomp.cross_tu import has_hard_failure
        self.assertFalse(has_hard_failure([]))

    def test_build_failure_is_hard(self) -> None:
        from tools.llm_decomp.cross_tu import has_hard_failure
        results = [{"unit": "kyoshin/cf/X", "ok": False, "error": "build", "detail": "compile error"}]
        self.assertTrue(has_hard_failure(results))

    def test_diff_failure_is_hard(self) -> None:
        from tools.llm_decomp.cross_tu import has_hard_failure
        results = [{"unit": "kyoshin/cf/X", "ok": False, "error": "diff", "detail": "object not found"}]
        self.assertTrue(has_hard_failure(results))

    def test_clean_results_no_baseline(self) -> None:
        from tools.llm_decomp.cross_tu import has_hard_failure
        results = [
            {"unit": "kyoshin/cf/X", "ok": True, "diffs": []},
            {"unit": "kyoshin/cf/Y", "ok": True, "diffs": [{"symbol": "func_a", "mismatches": 3}]},
        ]
        self.assertFalse(has_hard_failure(results))

    def test_unprotected_diff_not_hard(self) -> None:
        from tools.llm_decomp.cross_tu import has_hard_failure
        results = [{"unit": "kyoshin/cf/X", "ok": True, "diffs": [{"symbol": "func_a", "mismatches": 3}]}]
        baseline = {"kyoshin/cf/X:func_b": 0}
        self.assertFalse(has_hard_failure(results, baseline))

    def test_protected_symbol_diff_is_hard(self) -> None:
        from tools.llm_decomp.cross_tu import has_hard_failure
        results = [{"unit": "kyoshin/cf/X", "ok": True, "diffs": [{"symbol": "func_a", "mismatches": 3}]}]
        baseline = {"kyoshin/cf/X:func_a": 0}
        self.assertTrue(has_hard_failure(results, baseline))

    def test_protected_with_nonzero_baseline_not_hard(self) -> None:
        from tools.llm_decomp.cross_tu import has_hard_failure
        results = [{"unit": "kyoshin/cf/X", "ok": True, "diffs": [{"symbol": "func_a", "mismatches": 3}]}]
        baseline = {"kyoshin/cf/X:func_a": 5}
        self.assertFalse(has_hard_failure(results, baseline))


class TestSweepDependentsRefuse(unittest.TestCase):
    """REFUSE tier returns [] immediately."""

    def test_refuse_returns_empty(self) -> None:
        from tools.llm_decomp.cross_tu import Tier, sweep_dependents
        result = sweep_dependents(Path("/fake/repo"), ["kyoshin/cf/X"], Tier.REFUSE)
        self.assertEqual(result, [])


class TestSweepDependentsBuildFailure(unittest.TestCase):
    """Build failure in any dependent -> ok=False record."""

    def setUp(self) -> None:
        self.repo_root = Path("/fake/repo")
        self.patches = [patch("tools.llm_decomp.verify.build_unit", return_value=(False, "some error"))]
        for p in self.patches:
            p.start()
            self.addCleanup(p.stop)

    def test_build_failure_record(self) -> None:
        from tools.llm_decomp.cross_tu import Tier, has_hard_failure, sweep_dependents
        results = sweep_dependents(self.repo_root, ["kyoshin/cf/X"], Tier.FULL_SWEEP)
        self.assertEqual(len(results), 1)
        r = results[0]
        self.assertEqual(r["unit"], "kyoshin/cf/X")
        self.assertFalse(r["ok"])
        self.assertEqual(r["error"], "build")
        self.assertTrue(has_hard_failure(results))

    def test_build_failure_truncates_detail(self) -> None:
        from tools.llm_decomp.cross_tu import Tier, sweep_dependents
        long_diag = "x" * 1000
        with patch("tools.llm_decomp.verify.build_unit", return_value=(False, long_diag)):
            results = sweep_dependents(self.repo_root, ["kyoshin/cf/X"], Tier.FULL_SWEEP)
        self.assertFalse(results[0]["ok"])
        self.assertEqual(len(results[0]["detail"]), 500)


class TestSweepDependentsFullSweep(unittest.TestCase):
    """FULL_SWEEP builds and diffs every symbol."""

    def setUp(self) -> None:
        self.repo_root = Path("/fake/repo")
        self.build_patch = patch("tools.llm_decomp.verify.build_unit", return_value=(True, ""))
        self.build_patch.start()
        self.addCleanup(self.build_patch.stop)
        self.obj_patch = patch("tools.llm_decomp.verify.current_object_path", return_value=Path("/fake/decomp.o"))
        self.obj_patch.start()
        self.addCleanup(self.obj_patch.stop)
        self.retail_patch = patch("tools.llm_decomp.verify._resolve_retail_path", return_value=Path("/fake/retail.o"))
        self.retail_patch.start()
        self.addCleanup(self.retail_patch.stop)
        self.decomp_patch = patch("tools.llm_decomp.verify._resolve_decomp_path", return_value=Path("/fake/decomp.o"))
        self.decomp_patch.start()
        self.addCleanup(self.decomp_patch.stop)
        self.list_patch = patch("tools.llm_decomp.verify.list_text_functions")
        self.mock_list = self.list_patch.start()
        self.addCleanup(self.list_patch.stop)
        self.count_patch = patch("tools.llm_decomp.verify._count_mismatches")
        self.mock_count = self.count_patch.start()
        self.addCleanup(self.count_patch.stop)

    def test_full_sweep_no_diffs(self) -> None:
        from tools.llm_decomp.cross_tu import Tier, has_hard_failure, sweep_dependents
        code = b"\x00\x01\x02\x03"
        self.mock_list.side_effect = [
            [MockFn("func_a", code), MockFn("func_b", code)],
            [MockFn("func_a", code), MockFn("func_b", code)],
        ]
        self.mock_count.return_value = 0
        results = sweep_dependents(self.repo_root, ["kyoshin/cf/X"], Tier.FULL_SWEEP)
        self.assertEqual(len(results), 1)
        r = results[0]
        self.assertTrue(r["ok"])
        self.assertEqual(r["diffs"], [])
        self.assertFalse(has_hard_failure(results))

    def test_full_sweep_with_diffs(self) -> None:
        from tools.llm_decomp.cross_tu import Tier, has_hard_failure, sweep_dependents
        match_code = b"\x00\x01\x02\x03"
        diff_code = b"\xff\xff\xff\xff"
        self.mock_list.side_effect = [
            [MockFn("func_a", match_code), MockFn("func_b", match_code)],
            [MockFn("func_a", diff_code), MockFn("func_b", match_code)],
        ]
        self.mock_count.side_effect = [3, 0]
        results = sweep_dependents(self.repo_root, ["kyoshin/cf/X"], Tier.FULL_SWEEP)
        self.assertEqual(len(results), 1)
        r = results[0]
        self.assertTrue(r["ok"])
        self.assertEqual(len(r["diffs"]), 1)
        self.assertEqual(r["diffs"][0]["symbol"], "func_a")
        self.assertEqual(r["diffs"][0]["mismatches"], 3)
        self.assertFalse(has_hard_failure(results))

    def test_full_sweep_multiple_units(self) -> None:
        from tools.llm_decomp.cross_tu import Tier, sweep_dependents
        match_code = b"\x00\x01\x02\x03"
        diff_code = b"\xff\xff\xff\xff"
        self.mock_list.side_effect = [
            [MockFn("func_a", match_code)],
            [MockFn("func_a", diff_code)],
            [MockFn("func_b", match_code)],
            [MockFn("func_b", match_code)],
        ]
        self.mock_count.side_effect = [3, 0]
        results = sweep_dependents(self.repo_root, ["kyoshin/cf/X", "kyoshin/cf/Y"], Tier.FULL_SWEEP)
        self.assertEqual(len(results), 2)
        results.sort(key=lambda r: r["unit"])
        self.assertEqual(results[0]["unit"], "kyoshin/cf/X")
        self.assertEqual(len(results[0]["diffs"]), 1)
        self.assertEqual(results[1]["unit"], "kyoshin/cf/Y")
        self.assertEqual(results[1]["diffs"], [])


class TestSweepDependentsAcceptedOnly(unittest.TestCase):
    """ACCEPTED_ONLY filters diffs to protected symbols."""

    def setUp(self) -> None:
        self.repo_root = Path("/fake/repo")
        self.build_patch = patch("tools.llm_decomp.verify.build_unit", return_value=(True, ""))
        self.build_patch.start()
        self.addCleanup(self.build_patch.stop)
        self.obj_patch = patch("tools.llm_decomp.verify.current_object_path", return_value=Path("/fake/decomp.o"))
        self.obj_patch.start()
        self.addCleanup(self.obj_patch.stop)
        self.retail_patch = patch("tools.llm_decomp.verify._resolve_retail_path", return_value=Path("/fake/retail.o"))
        self.retail_patch.start()
        self.addCleanup(self.retail_patch.stop)
        self.decomp_patch = patch("tools.llm_decomp.verify._resolve_decomp_path", return_value=Path("/fake/decomp.o"))
        self.decomp_patch.start()
        self.addCleanup(self.decomp_patch.stop)
        self.list_patch = patch("tools.llm_decomp.verify.list_text_functions")
        self.mock_list = self.list_patch.start()
        self.addCleanup(self.list_patch.stop)
        self.count_patch = patch("tools.llm_decomp.verify._count_mismatches")
        self.mock_count = self.count_patch.start()
        self.addCleanup(self.count_patch.stop)

    def test_accepted_only_filters_to_protected(self) -> None:
        from tools.llm_decomp.cross_tu import Tier, has_hard_failure, sweep_dependents
        match_code = b"\x00\x01\x02\x03"
        diff_code = b"\xff\xff\xff\xff"

        # Both func_a and func_b differ from retail in decomp
        # func_a is protected (baseline 0), func_b is unprotected (not in baseline)
        self.mock_list.side_effect = [
            [MockFn("func_a", match_code), MockFn("func_b", match_code)],  # retail
            [MockFn("func_a", diff_code), MockFn("func_b", diff_code)],  # decomp
        ]
        self.mock_count.side_effect = [3, 5]

        baseline = {"kyoshin/cf/X:func_a": 0}
        results = sweep_dependents(
            self.repo_root,
            ["kyoshin/cf/X"],
            Tier.ACCEPTED_ONLY,
            accepted_baseline=baseline,
        )
        self.assertEqual(len(results), 1)
        r = results[0]
        self.assertTrue(r["ok"])
        # Only func_a (protected) should be in diffs
        self.assertEqual(len(r["diffs"]), 1)
        self.assertEqual(r["diffs"][0]["symbol"], "func_a")
        self.assertEqual(r["diffs"][0]["mismatches"], 3)
        # Protected symbol regression -> hard failure
        self.assertTrue(has_hard_failure(results, baseline))

    def test_accepted_only_without_baseline_includes_all(self) -> None:
        from tools.llm_decomp.cross_tu import Tier, sweep_dependents
        match_code = b"\x00\x01\x02\x03"
        diff_code = b"\xff\xff\xff\xff"

        self.mock_list.side_effect = [
            [MockFn("func_a", match_code), MockFn("func_b", match_code)],
            [MockFn("func_a", diff_code), MockFn("func_b", diff_code)],
        ]
        self.mock_count.side_effect = [3, 5]

        results = sweep_dependents(
            self.repo_root,
            ["kyoshin/cf/X"],
            Tier.ACCEPTED_ONLY,
            accepted_baseline=None,
        )
        self.assertEqual(len(results), 1)
        r = results[0]
        self.assertTrue(r["ok"])
        self.assertEqual(len(r["diffs"]), 2)  # Both symbols included (conservative)

    def test_accepted_only_multiple_units(self) -> None:
        from tools.llm_decomp.cross_tu import Tier, sweep_dependents
        match_code = b"\x00\x01\x02\x03"
        diff_code = b"\xff\xff\xff\xff"

        # retail/decomp for X, retail/decomp for Y
        self.mock_list.side_effect = [
            [MockFn("func_a", match_code)],
            [MockFn("func_a", diff_code)],
            [MockFn("func_b", match_code)],
            [MockFn("func_b", diff_code)],
        ]
        self.mock_count.side_effect = [3, 5]

        baseline = {"kyoshin/cf/X:func_a": 0}
        results = sweep_dependents(
            self.repo_root,
            ["kyoshin/cf/X", "kyoshin/cf/Y"],
            Tier.ACCEPTED_ONLY,
            accepted_baseline=baseline,
        )
        self.assertEqual(len(results), 2)
        results.sort(key=lambda r: r["unit"])
        # X has protected symbol func_a with diff
        self.assertEqual(len(results[0]["diffs"]), 1)
        # Y has func_b which is not protected -> filtered out
        self.assertEqual(len(results[1]["diffs"]), 0)

    def test_tu_only_no_baseline_needed(self) -> None:
        from tools.llm_decomp.cross_tu import Tier, sweep_dependents
        match_code = b"\x00\x01\x02\x03"
        diff_code = b"\xff\xff\xff\xff"

        self.mock_list.side_effect = [
            [MockFn("func_a", match_code)],
            [MockFn("func_a", diff_code)],
        ]
        self.mock_count.return_value = 2

        results = sweep_dependents(
            self.repo_root,
            ["kyoshin/cf/X"],
            Tier.TU_ONLY,
        )
        self.assertEqual(len(results), 1)
        self.assertTrue(results[0]["ok"])
        self.assertEqual(len(results[0]["diffs"]), 1)


if __name__ == "__main__":
    unittest.main()
