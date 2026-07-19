"""Tests for the deterministic seeded PPC32 random generator (P2-01)."""
import contextlib
import copy
import io
import json
import unittest

from tools.ppc_equivalence.fixtures.encode import dform, words_to_hex
from tools.ppc_equivalence.generators import (
    differential_check,
    generate_program,
    shrink_program,
)
from tools.ppc_equivalence.generators.random_gen import (
    ENVIRONMENT_PROFILE,
    GENERATOR_NAME,
    RESULT_REG,
    SANDBOX_BASE,
    SUPPORTED_FAMILIES,
    metamorphic_add_zero,
    metamorphic_encode_decode,
    metamorphic_rotate_zero,
    metamorphic_store_load,
)
from tools.ppc_equivalence.cli import main
from tools.ppc_equivalence.result import ARCHITECTURE_MODEL


_SEED_PAYLOAD_KEYS = {
    "generator",
    "seed",
    "architecture_model",
    "program_hex",
    "initial_state",
    "environment_profile",
    "concrete",
    "symbolic",
}


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
        self.assertIn("environment_profile", program)
        self.assertEqual(program["environment_profile"]["name"], ENVIRONMENT_PROFILE["name"])
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
        result = differential_check(1, shrink_on_failure=False)
        self.assertTrue(result["match"], result.get("mismatches"))
        self.assertEqual(result["dolphin"]["status"], "skipped")
        self.assertEqual(result["generator"], GENERATOR_NAME)
        self.assertEqual(result["architecture_model"], ARCHITECTURE_MODEL)

    def test_differential_check_batch_concrete_symbolic(self):
        failures = []
        for seed in range(1, 51):
            result = differential_check(seed, shrink_on_failure=False)
            if not result["match"]:
                failures.append((seed, result.get("mismatches")))
            self.assertEqual(result["dolphin"]["status"], "skipped")
            self.assertEqual(
                result["concrete"]["gpr"][RESULT_REG],
                result["symbolic"]["gpr"][RESULT_REG],
            )
        self.assertFalse(failures, f"concrete↔symbolic mismatches at {failures}")

    def test_seed_payload_completeness(self):
        result = differential_check(9, shrink_on_failure=False)
        missing = _SEED_PAYLOAD_KEYS - set(result)
        self.assertFalse(missing, f"missing seed payload keys: {sorted(missing)}")
        self.assertEqual(result["generator"], GENERATOR_NAME)
        self.assertEqual(result["seed"], 9)
        self.assertEqual(result["architecture_model"], ARCHITECTURE_MODEL)
        self.assertTrue(result["program_hex"])
        self.assertIn("gpr", result["initial_state"])
        self.assertIn("memory_bytes", result["initial_state"])
        self.assertEqual(result["environment_profile"]["sandbox_base"], f"0x{SANDBOX_BASE:08x}")
        self.assertIn("gpr", result["concrete"])
        self.assertIn("gpr", result["symbolic"])
        self.assertIn("memory_bytes", result["concrete"])
        self.assertIn("memory_bytes", result["symbolic"])

    def test_shrinker_reduces_injected_mutant(self):
        # Long program of harmless addi; inject a poison word that the
        # failure predicate requires. Shrinker must drop fillers while
        # preserving the poison instruction.
        poison = dform(14, 7, 0, 0x1234) & 0xFFFFFFFF
        fillers = [dform(14, 3, 0, imm) & 0xFFFFFFFF for imm in range(1, 9)]
        words = fillers[:4] + [poison] + fillers[4:]
        program = generate_program(42, max_instructions=1)
        program = copy.deepcopy(program)
        program["program_hex"] = words_to_hex(words)
        poison_hex = f"{poison:08x}"

        def still_fails(candidate: dict) -> bool:
            return poison_hex in candidate["program_hex"].split()

        result = shrink_program(program, still_fails)
        self.assertTrue(result["reduced"])
        self.assertLess(
            result["shrunk_instruction_count"],
            result["original_instruction_count"],
        )
        self.assertEqual(result["shrunk_instruction_count"], 1)
        self.assertEqual(result["shrunk"]["program_hex"].split(), [poison_hex])
        # Original reproduction preserved.
        self.assertEqual(result["original"]["program_hex"], words_to_hex(words))

    def test_metamorphic_store_load(self):
        for seed in range(50):
            self.assertTrue(metamorphic_store_load(seed)["holds"])

    def test_metamorphic_rotate_zero(self):
        for seed in range(50):
            self.assertTrue(metamorphic_rotate_zero(seed)["holds"])

    def test_metamorphic_add_zero(self):
        for seed in range(50):
            self.assertTrue(metamorphic_add_zero(seed)["holds"])

    def test_metamorphic_encode_decode(self):
        for seed in range(40):
            self.assertTrue(
                metamorphic_encode_decode(seed)["holds"],
                f"encode/decode round-trip failed for seed {seed}",
            )


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
        for program in payload["programs"]:
            self.assertIn("environment_profile", program)


if __name__ == "__main__":
    unittest.main()
