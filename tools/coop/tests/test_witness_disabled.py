"""witness_enabled=False regression tests (adversarial-review fixes).

The witness opt-out contract, end-to-end through certify_unit_symbol /
evaluate_unit_match:

- A byte-identical pair STILL certifies as FULL_MATCH (full-instruction-match
  synthesis; no witness, no Z3) — including when objdiff reports 0%/None
  (fn_match is None, e.g. CBattleManager-class null retail symbols), which the
  byte-identity direct-read branch must reach even with the witness disabled.
- A non-byte-identical pair reports INCONCLUSIVE_SMT_DISABLED and can NEVER
  reach EQUIVALENT / EQUIVALENT_MATCH — including when a reviewed
  hardware_profile configures the memory-bus SMT block: the nested
  prove_unit_symbol must be passed smt=False + witness_enabled=False so
  neither the register-renaming witness nor Z3 ever runs (the pre-fix code
  re-enabled both and could mint an EQUIVALENT probe that FULL_MATCH-only
  acceptance then trusted).

Run: .venv/bin/python3 -m unittest tools.coop.tests.test_witness_disabled
"""

from __future__ import annotations

import json
import os
import tempfile
import unittest
from pathlib import Path
from unittest import mock

from tools.coop.lib.config import CoopConfig
from tools.coop.lib.equivalence_check import EquivalenceProbe, certify_unit_symbol
from tools.coop.lib.project import ObjdiffUnit, Project
from tools.ppc_equivalence.result import ProofStatus
from tools.ppc_equivalence.tests.test_elf_symbols import _EQ_LEFT, _NEQ, build_reloc_elf

_TARGET_ID = "us-witness-disabled"
_SYMBOL = "func__3LeafFv"
_UNIT_NAME = "demo/WitnessDisabled"
# Real reviewed profile (read-only load from the repo) — makes
# memory_bus_from_config return a real MemoryBus so the HIGH-1 memory-bus
# block actually executes in the end-to-end test.
_HARDWARE_PROFILE = "wii-broadway-xenoblade-us-v2"
# Pure volatile r5<->r6 color swap: same shape, different registers — the
# register-renaming witness WOULD certify this pair when enabled.
_RETAIL_REGSWAP = bytes.fromhex("7c631b78 38a30008 4e800020")
_DECOMP_REGSWAP = bytes.fromhex("7c631b78 38c30008 4e800020")


class WitnessDisabledCertifyTests(unittest.TestCase):
    """certify_unit_symbol with witness_enabled=False: byte-identical pairs
    still certify as FULL_MATCH; non-byte-identical pairs are
    INCONCLUSIVE_SMT_DISABLED and never EQUIVALENT."""

    def setUp(self) -> None:
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name)
        (self.root / "tools/coop").mkdir(parents=True)
        (self.root / "build/us").mkdir(parents=True)
        self.config = CoopConfig(project_root=self.root, region="us")
        (self.root / "build/us/objdiff.json").write_text(
            json.dumps(
                {
                    "units": [
                        {
                            "name": _UNIT_NAME,
                            "target_path": "build/us/retail.o",
                            "base_path": "build/us/decomp.o",
                        }
                    ]
                }
            ),
            encoding="utf-8",
        )
        (self.root / "tools/coop/targets.json").write_text(
            json.dumps(
                {
                    "schema_version": 2,
                    "default_required_level": "EQUIVALENT_MATCH",
                    "targets": [
                        {
                            "id": _TARGET_ID,
                            "symbol": _SYMBOL,
                            "address": "0x80001000",
                            "function": _SYMBOL,
                            "unit": _UNIT_NAME,
                            "region": "us",
                            "status": "FULL_MATCH",
                            "called_functions": [],
                            "abi_helper_calls": [],
                        }
                    ],
                }
            ),
            encoding="utf-8",
        )
        self.project = Project(self.config)
        self.retail = self.root / "build/us/retail.o"
        self.decomp = self.root / "build/us/decomp.o"

    def tearDown(self) -> None:
        self.tmp.cleanup()

    def _unit(self) -> ObjdiffUnit:
        return ObjdiffUnit(
            name=_UNIT_NAME,
            target_path=self.retail,
            base_path=self.decomp,
            source_path=None,
        )

    def _pin_past_mtimes(self) -> None:
        """Pin fixture object mtimes to the past so prove_unit_symbol's
        reloc-map freshness check never re-mines (which would WRITE the repo's
        retail_reloc_map.json)."""
        past = 946684800.0  # 2000-01-01
        for p in (self.retail, self.decomp):
            if p.is_file():
                os.utime(p, (past, past))

    def _write_pair(self, retail_body: bytes, decomp_body: bytes) -> None:
        self.retail.write_bytes(build_reloc_elf({_SYMBOL: retail_body}))
        self.decomp.write_bytes(build_reloc_elf({_SYMBOL: decomp_body}))
        self._pin_past_mtimes()

    def test_byte_identical_pair_still_certifies_full_match(self) -> None:
        """witness disabled must NOT block the byte-identity FULL_MATCH path:
        the pair certifies via the full-instruction-match synthesis (no
        witness probe, no Z3)."""
        self._write_pair(_EQ_LEFT, _EQ_LEFT)
        unit = self._unit()

        probe = certify_unit_symbol(
            self.project, unit, _SYMBOL, _TARGET_ID, witness_enabled=False,
        )
        self.assertEqual(probe.status, ProofStatus.EQUIVALENT, probe.detail)
        self.assertIsNotNone(probe.certificate)
        self.assertEqual(
            probe.certificate.get("evidence"), "full-instruction-match",
        )

        # And through the full evaluate_unit_match pipeline the status is
        # FULL_MATCH (never EQUIVALENT_MATCH — no reg-swap witness involved).
        from tools.coop.lib import objdiff_report as _or

        fn_match = _or.FunctionMatch(
            name=_SYMBOL, demangled_name=None, match_percent=100.0, size=8,
        )
        unit_report = _or.UnitReport(
            unit_name=_UNIT_NAME,
            code_match_percent=100.0,
            data_match_percent=100.0,
            fuzzy_match_percent=100.0,
            total_functions=1,
            matched_functions=1,
            functions=[fn_match],
        )
        with mock.patch.object(_or, "report_unit", return_value=unit_report), mock.patch.object(
            _or, "find_function_match", return_value=fn_match
        ):
            ev = _or.evaluate_unit_match(
                self.project, unit, _SYMBOL, target_id=_TARGET_ID,
                run_smt=False, witness_enabled=False,
            )
        self.assertEqual(ev.status, "FULL_MATCH")
        self.assertIsNotNone(ev.equivalence_certificate)
        self.assertEqual(
            ev.equivalence_certificate.get("evidence"), "full-instruction-match",
        )

    def test_byte_identical_with_null_objdiff_match_still_certifies(self) -> None:
        """HIGH-3 (GLM F2 / DeepSeek F1): units whose objdiff report returns
        None for byte-identical functions (null retail symbols, e.g.
        CBattleManager) must still be FULL_MATCH-acceptable with the witness
        disabled — the byte-identity direct read must not be gated on
        witness_enabled (the 2nd/3rd branches require fn_match)."""
        self._write_pair(_EQ_LEFT, _EQ_LEFT)
        unit = self._unit()

        from tools.coop.lib import objdiff_report as _or

        unit_report = _or.UnitReport(
            unit_name=_UNIT_NAME,
            code_match_percent=0.0,
            data_match_percent=0.0,
            fuzzy_match_percent=0.0,
            total_functions=1,
            matched_functions=0,
            functions=[],
        )
        with mock.patch.object(_or, "report_unit", return_value=unit_report), mock.patch.object(
            _or, "find_function_match", return_value=None
        ):
            ev = _or.evaluate_unit_match(
                self.project, unit, _SYMBOL, target_id=_TARGET_ID,
                run_smt=False, witness_enabled=False,
            )
        self.assertEqual(ev.status, "FULL_MATCH", ev.equivalence_detail)
        self.assertIsNotNone(ev.equivalence_certificate)

    def test_non_byte_identical_pair_is_inconclusive_never_equivalent(self) -> None:
        """A non-byte-identical pair (constant differs, so the witness would
        reject it anyway) reports INCONCLUSIVE_SMT_DISABLED with no
        certificate — never EQUIVALENT, never a full-instruction-match cert."""
        self._write_pair(_NEQ, _EQ_LEFT)
        unit = self._unit()

        probe = certify_unit_symbol(
            self.project, unit, _SYMBOL, _TARGET_ID, witness_enabled=False,
        )
        self.assertEqual(probe.status, ProofStatus.INCONCLUSIVE_SMT_DISABLED)
        self.assertIsNone(probe.certificate)

        from tools.coop.lib import objdiff_report as _or

        fn_match = _or.FunctionMatch(
            name=_SYMBOL, demangled_name=None, match_percent=0.0, size=8,
        )
        unit_report = _or.UnitReport(
            unit_name=_UNIT_NAME,
            code_match_percent=0.0,
            data_match_percent=100.0,
            fuzzy_match_percent=0.0,
            total_functions=1,
            matched_functions=0,
            functions=[fn_match],
        )
        with mock.patch.object(_or, "report_unit", return_value=unit_report), mock.patch.object(
            _or, "find_function_match", return_value=fn_match
        ):
            ev = _or.evaluate_unit_match(
                self.project, unit, _SYMBOL, target_id=_TARGET_ID,
                run_smt=False, witness_enabled=False,
            )
        self.assertEqual(ev.equivalence, ProofStatus.INCONCLUSIVE_SMT_DISABLED)
        self.assertNotEqual(ev.status, "EQUIVALENT_MATCH")
        self.assertNotEqual(ev.status, "FULL_MATCH")

    def test_memory_bus_block_forwards_smt_false_and_witness_disabled(self) -> None:
        """HIGH-1 (DeepSeek F2): with a hardware_profile configured, the
        memory-bus block inside certify_unit_symbol must call the nested
        prove_unit_symbol with smt=False AND witness_enabled=False — the
        pre-fix call used both defaults (True), re-enabling the witness and
        full Z3 for a non-byte-identical fuzzy target so an EQUIVALENT probe
        could be produced and accepted by the FULL_MATCH-only pipeline."""
        self.config = CoopConfig(
            project_root=self.root, region="us",
            hardware_profile=_HARDWARE_PROFILE,
        )
        self.project = Project(self.config)
        self._write_pair(_RETAIL_REGSWAP, _DECOMP_REGSWAP)
        unit = self._unit()

        from tools.coop.lib import equivalence_check as eq

        calls: dict = {}

        def fake_prove(project, unit, symbol, **kw):
            calls["kwargs"] = kw
            return EquivalenceProbe(
                ProofStatus.INCONCLUSIVE_SMT_DISABLED, "no SMT (witness disabled)",
            )

        with mock.patch.object(eq, "prove_unit_symbol", side_effect=fake_prove):
            probe = certify_unit_symbol(
                self.project, unit, _SYMBOL, _TARGET_ID, witness_enabled=False,
            )

        # The nested prove was actually reached (memory bus configured).
        self.assertIn("kwargs", calls, "memory-bus block must call prove_unit_symbol")
        kwargs = calls["kwargs"]
        self.assertFalse(
            kwargs.get("smt", True),
            "nested prove_unit_symbol must be passed smt=False (Z3 must never run)",
        )
        self.assertFalse(
            kwargs.get("witness_enabled", True),
            "nested prove_unit_symbol must inherit witness_enabled=False",
        )
        self.assertEqual(probe.status, ProofStatus.INCONCLUSIVE_SMT_DISABLED)
        self.assertIsNone(probe.certificate)

    def test_regswap_pair_with_hardware_profile_stays_inconclusive_end_to_end(self) -> None:
        """End-to-end (no mocks): a pure reg-swap pair that the witness WOULD
        certify is still INCONCLUSIVE_SMT_DISABLED when the witness is
        disabled — even with a real MemoryBus configured, because the nested
        prove_unit_symbol runs with smt=False + witness_enabled=False (witness
        probe skipped, Z3 never invoked)."""
        self.config = CoopConfig(
            project_root=self.root, region="us",
            hardware_profile=_HARDWARE_PROFILE,
        )
        self.project = Project(self.config)
        self._write_pair(_RETAIL_REGSWAP, _DECOMP_REGSWAP)
        unit = self._unit()

        probe = certify_unit_symbol(
            self.project, unit, _SYMBOL, _TARGET_ID, witness_enabled=False,
        )
        self.assertEqual(probe.status, ProofStatus.INCONCLUSIVE_SMT_DISABLED)
        self.assertIsNone(probe.certificate)


if __name__ == "__main__":
    unittest.main()
