"""Phase 10 schema-v2 FP obligation tests."""

from __future__ import annotations

import copy
import unittest

from tools.ppc_equivalence.capability_assurance import (
    STATUS_INCOMPLETE,
    STATUS_PROMOTION_GRADE,
)
from tools.ppc_equivalence.fp_fused_obligations import build_fp_fused_obligation_v2
from tools.ppc_equivalence.fp_scalar_obligations_v2 import (
    FP_FUSED_EXACT_V2_ALGORITHM,
    FUSED_V2_DIMENSIONS,
    build_fp_scalar_obligation_v2,
    evaluate_fp_scalar_obligation_v2_status,
    live_scalar_fp_v2_corpus_sha256,
    obligation_v2_sha256,
    recompute_obligation_digests,
    validate_fp_scalar_obligation_v2,
    validation_ledger_scalar_fp_v2_hook,
)

_CRITICAL_FIELDS = (
    "schema_version",
    "capability",
    "model_version",
    "algorithm",
    "domain",
    "opcodes",
    "modes",
    "dimensions",
    "coverage",
)


class FPScalarObligationsV2Tests(unittest.TestCase):
    def _complete_obligation(self) -> dict:
        return build_fp_fused_obligation_v2(
            opcodes=["fmadd", "fmadds"],
            dimensions={key: True for key in FUSED_V2_DIMENSIONS},
            unsupported_remainder_result="unsat",
            unsupported_remainder_query_sha256="a" * 64,
            corpus_sha256="b" * 64,
            validation_ledger_hash="c" * 64,
        )

    def test_validate_accepts_well_formed_v2(self) -> None:
        obl = self._complete_obligation()
        self.assertIsNone(validate_fp_scalar_obligation_v2(obl))

    def test_forgery_on_each_critical_field_fails_closed(self) -> None:
        base = self._complete_obligation()
        for field in _CRITICAL_FIELDS:
            with self.subTest(field=field):
                forged = copy.deepcopy(base)
                if field == "schema_version":
                    forged[field] = 1
                elif field == "capability":
                    forged[field] = "fp-bitwise"
                elif field == "model_version":
                    forged[field] = "broadway-fp-fused-v1"
                elif field == "algorithm":
                    forged[field] = "fp-fused-incomplete-v0"
                elif field == "domain":
                    forged[field] = {"no_host_float": False}
                elif field == "opcodes":
                    forged[field] = ["fadd"]
                elif field == "modes":
                    forged[field]["rn"] = ["bogus-mode"]
                elif field == "dimensions":
                    forged[field]["midpoint_residual"] = False
                elif field == "coverage":
                    forged[field]["unsupported_remainder"]["result"] = "sat"
                if field in ("dimensions", "coverage"):
                    self.assertEqual(
                        evaluate_fp_scalar_obligation_v2_status(
                            forged,
                            live_corpus_sha256="b" * 64,
                            live_validation_ledger_hash="c" * 64,
                            allowlist_contains_model=True,
                        ),
                        STATUS_INCOMPLETE,
                    )
                    continue
                err = validate_fp_scalar_obligation_v2(forged)
                self.assertIsNotNone(err, msg=f"{field}: {err}")

    def test_promotion_requires_unsat_dimensions_and_allowlist(self) -> None:
        obl = self._complete_obligation()
        self.assertEqual(
            evaluate_fp_scalar_obligation_v2_status(
                obl,
                live_corpus_sha256="b" * 64,
                live_validation_ledger_hash="c" * 64,
                allowlist_contains_model=True,
            ),
            STATUS_PROMOTION_GRADE,
        )
        self.assertEqual(
            evaluate_fp_scalar_obligation_v2_status(
                obl,
                live_corpus_sha256="b" * 64,
                live_validation_ledger_hash="c" * 64,
                allowlist_contains_model=False,
            ),
            STATUS_INCOMPLETE,
        )

    def test_host_float_forces_incomplete(self) -> None:
        obl = self._complete_obligation()
        self.assertEqual(
            evaluate_fp_scalar_obligation_v2_status(obl, host_float=True),
            STATUS_INCOMPLETE,
        )

    def test_obligation_digest_is_stable(self) -> None:
        obl = build_fp_scalar_obligation_v2("fp-fused-arithmetic", opcodes=["fmadd"])
        self.assertEqual(len(obligation_v2_sha256(obl)), 64)

    def test_recompute_obligation_digests(self) -> None:
        obl = self._complete_obligation()
        live = live_scalar_fp_v2_corpus_sha256()
        ok, reasons = recompute_obligation_digests(
            obl,
            live_corpus_sha256=live,
            live_validation_ledger_hash="c" * 64,
        )
        self.assertFalse(ok)
        self.assertIn("corpus_sha256 mismatch", reasons)
        ok_match, reasons_match = recompute_obligation_digests(
            obl,
            live_corpus_sha256="b" * 64,
            live_validation_ledger_hash="c" * 64,
        )
        self.assertTrue(ok_match)
        self.assertEqual(reasons_match, [])
        ok_bad, reasons_bad = recompute_obligation_digests(
            obl,
            live_corpus_sha256="x" * 64,
            live_validation_ledger_hash="c" * 64,
        )
        self.assertFalse(ok_bad)
        self.assertIn("corpus_sha256 mismatch", reasons_bad)

    def test_validation_ledger_hook_lists_corpus_roots(self) -> None:
        hook = validation_ledger_scalar_fp_v2_hook()
        self.assertIn("scalar_fp_v2_corpus_roots", hook)
        self.assertTrue(
            any("fused_residual.jsonl" in path for path in hook["scalar_fp_v2_corpus_roots"])
        )

    def test_algorithm_identity_is_v2_exact(self) -> None:
        obl = build_fp_scalar_obligation_v2("fp-fused-arithmetic")
        self.assertEqual(obl["algorithm"], FP_FUSED_EXACT_V2_ALGORITHM)


if __name__ == "__main__":
    unittest.main()
