from __future__ import annotations

import unittest

from tools.ppc_equivalence.result import (
    FP_COVERAGE_ASSUMED,
    FP_COVERAGE_PROVEN,
    FP_COVERAGE_UNSUPPORTED,
    FloatingPointDomain,
)


class FloatingPointDomainCoverageTests(unittest.TestCase):
    def test_to_dict_includes_coverage(self):
        d = FloatingPointDomain().to_dict()
        self.assertTrue(d["used"])
        self.assertIn("coverage", d)
        self.assertEqual(d["coverage"]["proven"], list(FP_COVERAGE_PROVEN))
        self.assertEqual(d["coverage"]["assumed"], list(FP_COVERAGE_ASSUMED))
        self.assertEqual(d["coverage"]["unsupported"], list(FP_COVERAGE_UNSUPPORTED))

    def test_from_dict_roundtrip_core_fields(self):
        original = FloatingPointDomain(allow_subnormal=True, traps_enabled=True)
        restored = FloatingPointDomain.from_dict(original.to_dict())
        self.assertEqual(restored.rounding_modes, original.rounding_modes)
        self.assertTrue(restored.allow_subnormal)
        self.assertTrue(restored.traps_enabled)


if __name__ == "__main__":
    unittest.main()
