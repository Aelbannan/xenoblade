"""Tests for the deterministic seeded PPC32 random generator (P2-01)."""
import contextlib
import io
import json
import unittest

from tools.ppc_equivalence.generators import (
    differential_check,
    generate_program,
)
from tools.ppc_equivalence.generators.random_gen import (
    SUPPORTED_FAMILIES,
    metamorphic_add_zero,
    metamorphic_rotate_zero,
    metamorphic_store_load,
)
from tools.ppc_equivalence.cli import main


class RandomGeneratorTests(unittest.TestCase):
    def test_determinism(self):
        a = generate_program(123)
        b = generate_program(123)
        self.assertEqual(a, b)
        c = generate_program(124)
        self.assertNotEqual(a, c)

    def test_program_has_expected_oracle(self):
        program = generate_program(7)
        self.assertIn("program_hex", program)
        self.assertIn("expected", program)
        self.assertIsInstance(program["family"], str)
        expected = program["expected"]
        self.assertEqual(len(expected["gpr"]), 32)
        self.assertEqual(len(expected["fpr_bits"]), 32)
        self.assertIn("memory_bytes", expected)

    def test_family_coverage_over_seeds(self):
        seen = {generate_program(seed)["family"] for seed in range(300)}
        missing = set(SUPPORTED_FAMILIES) - seen
        self.assertFalse(missing, f"uncovered families: {sorted(missing)}")

    def test_differential_check_matches_single_seed(self):
        result = differential_check(1)
        self.assertTrue(result["match"])
        self.assertIn(result["symbolic_status"], ("equivalent", "inconclusive"))

    def test_differential_check_batch(self):
        failures = [seed for seed in range(1, 51) if not differential_check(seed)["match"]]
        self.assertFalse(failures, f"differential mismatches at seeds {failures}")

    def test_metamorphic_store_load(self):
        for seed in range(50):
            self.assertTrue(metamorphic_store_load(seed)["holds"])

    def test_metamorphic_rotate_zero(self):
        for seed in range(50):
            self.assertTrue(metamorphic_rotate_zero(seed)["holds"])

    def test_metamorphic_add_zero(self):
        for seed in range(50):
            self.assertTrue(metamorphic_add_zero(seed)["holds"])


class GenerateCliTests(unittest.TestCase):
    def test_cli_generate_text(self):
        out = io.StringIO()
        with contextlib.redirect_stdout(out):
            rc = main(["generate", "--seed", "1", "--count", "3"])
        self.assertEqual(rc, 0)
        # three program blocks, one per seed
        self.assertEqual(out.getvalue().count("# seed"), 3)

    def test_cli_generate_json_with_differential(self):
        out = io.StringIO()
        with contextlib.redirect_stdout(out):
            rc = main(
                ["generate", "--seed", "1", "--count", "4", "--differential", "--json"]
            )
        self.assertEqual(rc, 0)
        payload = json.loads(out.getvalue())
        self.assertEqual(payload["count"], 4)
        self.assertEqual(payload["differential_failed"], 0)
        self.assertEqual(len(payload["programs"]), 4)


if __name__ == "__main__":
    unittest.main()
