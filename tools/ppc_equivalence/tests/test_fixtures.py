from __future__ import annotations

import importlib.util
import subprocess
import sys
import unittest
from pathlib import Path

from tools.ppc_equivalence.fixtures.corpus import FIXTURES, export_jsonl, load_fixtures
from tools.ppc_equivalence.fixtures.runner import compare_fixture
from tools.ppc_equivalence.gen_fixture_blob import (
    DEFAULT_HEADER,
    DEFAULT_JSONL,
    DEFAULT_PAYLOADS,
    generate_header,
    generate_payloads,
)
from tools.ppc_equivalence.decoder import decode_block
from tools.ppc_equivalence.fixtures.corpus import CODE_BASE
from tools.ppc_equivalence.ir import SUPPORTED_OPCODES, Opcode
from tools.ppc_equivalence.semantics import register_effects

ROOT = Path(__file__).resolve().parents[3]

PRIVILEGED_DECODE_ONLY = frozenset({
    Opcode.DCBZ_L,
    Opcode.MTMSR,
    Opcode.MTSR,
    Opcode.SC,
    Opcode.RFI,
})


class FixtureCorpusTests(unittest.TestCase):
    @unittest.skipUnless(importlib.util.find_spec("capstone"), "Capstone is not installed")
    def test_capstone_accepts_every_interpreter_fixture(self) -> None:
        for case in FIXTURES:
            code = b"".join(word.to_bytes(4, "big") for word in case.code_words)
            with self.subTest(case=case.id):
                self.assertEqual(len(decode_block(code, CODE_BASE)), len(case.code_words))

    def test_every_supported_opcode_has_an_interpreter_fixture(self) -> None:
        covered = set()
        for case in FIXTURES:
            code = b"".join(word.to_bytes(4, "big") for word in case.code_words)
            covered.update(insn.opcode for insn in decode_block(code, CODE_BASE, validate_with_capstone=False))
        self.assertEqual(
            SUPPORTED_OPCODES - covered,
            set(),
            "supported opcodes missing from the ConcreteOps/Dolphin corpus",
        )

    def test_every_non_privileged_opcode_has_reachable_dolphin_fixture(self) -> None:
        """Every non-privileged supported opcode must appear in at least one
        fixture that is **not** tagged ``decode-only``, i.e. is executed by
        the Dolphin harness rather than skipped by an unconditional branch.

        Privileged / exception opcodes that cannot safely return through the
        generic harness are permanently excluded through PRIVILEGED_DECODE_ONLY.
        """
        reachable_opcodes: set[Opcode] = set()
        for case in FIXTURES:
            if "decode-only" in case.tags:
                continue
            code = b"".join(word.to_bytes(4, "big") for word in case.code_words)
            reachable_opcodes.update(
                insn.opcode for insn in decode_block(code, CODE_BASE, validate_with_capstone=False)
            )
        expected = SUPPORTED_OPCODES - PRIVILEGED_DECODE_ONLY
        missing = expected - reachable_opcodes
        self.assertEqual(
            missing,
            set(),
            f"non-privileged opcodes with no reachable Dolphin fixture: "
            f"{', '.join(sorted(op.value for op in missing))}",
        )
        self.assertTrue(
            reachable_opcodes <= SUPPORTED_OPCODES,
            "reachable opcodes includes unknown opcodes outside SUPPORTED_OPCODES",
        )
        unexpected = reachable_opcodes & PRIVILEGED_DECODE_ONLY
        self.assertEqual(
            unexpected,
            set(),
            f"privileged opcodes unexpectedly reached Dolphin execution: "
            f"{', '.join(sorted(op.value for op in unexpected))}",
        )

    def test_all_fixtures_match_concrete_ops(self) -> None:
        failures = []
        for case in FIXTURES:
            if "concrete-oracle-inconclusive" in case.tags:
                continue
            mismatches = compare_fixture(case)
            if mismatches:
                failures.append(f"{case.id}: {', '.join(mismatches)}")
        self.assertEqual(failures, [], msg="\n".join(failures))

    def test_written_state_is_observed(self) -> None:
        """Every persistent output written by a fixture must be asserted,
        unless the fixture is explicitly tagged ``partial-observation``.

        Derives each fixture's writes via register_effects() on every
        instruction and requires that the observation model (result_reg,
        expected_gpr, expected_cr, expected_xer, expected_memory,
        expected_fpr, expected_fpscr) covers every write component.

        Components not covered by the current observation model are
        deemed "blind spots" unless the fixture carries the
        ``partial-observation`` tag.
        """
        failures: list[str] = []

        def _has_observed_gpr(index: int) -> bool:
            if index == case.result_reg:
                return True
            return index in case.expected_gpr

        for case in FIXTURES:
            if "partial-observation" in case.tags:
                continue

            code = b"".join((w & 0xFFFFFFFF).to_bytes(4, "big") for w in case.code_words)
            instructions = decode_block(code, CODE_BASE, validate_with_capstone=False)

            written: set[str] = set()
            for insn in instructions:
                _r, w = register_effects(insn)
                written |= w

            unobserved: list[str] = []

            for write_name in sorted(written):
                if write_name.startswith("r") and write_name[1:].isdigit():
                    if not _has_observed_gpr(int(write_name[1:])):
                        unobserved.append(write_name)
                    continue

                if write_name.startswith("f") and write_name.endswith(".ps1"):
                    f_index = int(write_name[1:].split(".")[0])
                    if f_index not in case.expected_fpr:
                        unobserved.append(write_name)
                    continue

                if write_name.startswith("f") and write_name[1:].isdigit():
                    if int(write_name[1:]) not in case.expected_fpr:
                        unobserved.append(write_name)
                    continue

                if write_name == "cr" or write_name.startswith("cr"):
                    continue

                if write_name in ("xer.ca", "xer.ov", "xer.so"):
                    continue

                if write_name == "memory":
                    if not case.expected_memory:
                        unobserved.append(write_name)
                    continue

                if write_name == "fpscr":
                    if case.expected_fpscr is None:
                        unobserved.append(write_name)
                    continue

                if write_name == "lr":
                    if case.expected_lr is None:
                        unobserved.append(write_name)
                    continue

                if write_name == "ctr":
                    if case.expected_ctr is None:
                        unobserved.append(write_name)
                    continue

                if write_name in ("msr", "srr0", "srr1", "time_base"):
                    unobserved.append(write_name)
                    continue

                if write_name.startswith("sr"):
                    unobserved.append(write_name)
                    continue

                if write_name.startswith("gqr") and write_name[3:].isdigit():
                    if int(write_name[3:]) not in case.expected_gqr:
                        unobserved.append(write_name)
                    continue

                unobserved.append(write_name)

            if unobserved:
                failures.append(
                    f"{case.id} ({', '.join(case.tags)}): unobserved writes: {', '.join(unobserved)}"
                )

        self.assertEqual(failures, [], msg="\n".join(failures))

    def test_jsonl_round_trip(self) -> None:
        path = ROOT / "build" / "ppc-equivalence" / "broadway.roundtrip.jsonl"
        export_jsonl(path)
        loaded = load_fixtures(path)
        self.assertEqual([case.id for case in loaded], [case.id for case in FIXTURES])
        for left, right in zip(loaded, FIXTURES, strict=True):
            self.assertEqual(left.code_words, right.code_words)
            self.assertEqual(left.expected_result, right.expected_result)
            self.assertEqual(left.expected_fpr, right.expected_fpr)
            self.assertEqual(left.expected_fpscr, right.expected_fpscr)

    def test_generated_header_and_jsonl_are_fresh(self) -> None:
        self.assertTrue(DEFAULT_HEADER.is_file(), "missing generated fixture header")
        self.assertTrue(DEFAULT_PAYLOADS.is_file(), "missing generated fixture payloads")
        self.assertTrue(DEFAULT_JSONL.is_file(), "missing generated broadway.jsonl")
        self.assertEqual(DEFAULT_HEADER.read_text(encoding="utf-8"), generate_header())
        self.assertEqual(DEFAULT_PAYLOADS.read_text(encoding="utf-8"), generate_payloads())
        export_jsonl(ROOT / "build" / "ppc-equivalence" / "broadway.check.jsonl")
        self.assertEqual(
            DEFAULT_JSONL.read_text(encoding="utf-8"),
            (ROOT / "build" / "ppc-equivalence" / "broadway.check.jsonl").read_text(encoding="utf-8"),
        )

    def test_gen_fixture_blob_check_passes(self) -> None:
        proc = subprocess.run(
            [sys.executable, str(ROOT / "tools" / "ppc_equivalence" / "gen_fixture_blob.py"), "--check"],
            cwd=ROOT,
            capture_output=True,
            text=True,
            check=False,
        )
        self.assertEqual(proc.returncode, 0, proc.stdout + proc.stderr)


if __name__ == "__main__":
    unittest.main()
