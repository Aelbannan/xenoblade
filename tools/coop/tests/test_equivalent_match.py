"""Status ladder + EQUIVALENT_MATCH acceptance policy tests."""

from __future__ import annotations

import unittest

from tools.coop.lib.equivalence_check import EQUIVALENT_MATCH_MIN_PERCENT, should_probe_equivalence
from tools.coop.lib.objdiff_report import UnitReport, classify_status, meets_required_level
from tools.ppc_equivalence.result import ProofStatus


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
        self.assertEqual(
            classify_status(
                EQUIVALENT_MATCH_MIN_PERCENT - 0.1,
                _unit(),
                symbol="f",
                equivalence=ProofStatus.EQUIVALENT,
            ),
            "HIGH_MATCH",
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
        self.assertFalse(should_probe_equivalence(79.9))
        self.assertTrue(should_probe_equivalence(80.0))
        self.assertTrue(should_probe_equivalence(99.9))
        self.assertFalse(should_probe_equivalence(100.0))


if __name__ == "__main__":
    unittest.main()
