"""Phase 9 exact fused kernel tests vs SoftFloat oracle (RNE)."""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.fp_exact_fused import (
    fmadd_binary64_rne,
    fmadds_fpr_rne,
    fmsub_binary64_rne,
    fmsubs_fpr_rne,
    fnmadd_binary64_rne,
    fnmadds_fpr_rne,
    fnmsub_binary64_rne,
    fnmsubs_fpr_rne,
)
from tools.ppc_equivalence.fp_fused_obligations import (
    FUSED_V2_COMPLETE_DIMENSIONS,
    build_fp_fused_obligation_v2,
    evaluate_fp_fused_obligation_status,
    recompute_fp_fused_attestation_status,
)
from tools.ppc_equivalence.fp_oracle import (
    OracleUnimplementedError,
    fmadd_binary64_rne as oracle_fmadd_binary64_rne,
    fmadds_fpr_rne as oracle_fmadds_fpr_rne,
    fmsub_binary64_rne as oracle_fmsub_binary64_rne,
    fmsubs_fpr_rne as oracle_fmsubs_fpr_rne,
    fnmadd_binary64_rne as oracle_fnmadd_binary64_rne,
    fnmadds_fpr_rne as oracle_fnmadds_fpr_rne,
    fnmsub_binary64_rne as oracle_fnmsub_binary64_rne,
    fnmsubs_fpr_rne as oracle_fnmsubs_fpr_rne,
)
from tools.ppc_equivalence.capability_assurance import (
    STATUS_INCOMPLETE,
    STATUS_PROMOTION_GRADE,
)
from tools.ppc_equivalence.fp_advanced_obligations import (
    FP_FUSED_ALGORITHM,
    build_fp_advanced_obligation,
    evaluate_fp_advanced_obligation_status,
)

_F15 = 0x3FF8000000000000
_F2 = 0x4000000000000000
_F4 = 0x4010000000000000


class ExactFusedKernelTests(unittest.TestCase):
    def test_fmadd_matches_oracle_for_finite_corpus(self) -> None:
        exact = fmadd_binary64_rne(_F15, _F4, _F2)
        oracle = oracle_fmadd_binary64_rne(_F15, _F4, _F2)
        self.assertEqual(exact.bits64, oracle.bits64)

    def test_fused_double_family_matches_oracle(self) -> None:
        pairs = (
            (fmadd_binary64_rne, oracle_fmadd_binary64_rne, (_F15, _F4, _F2)),
            (fmsub_binary64_rne, oracle_fmsub_binary64_rne, (_F15, _F4, _F2)),
            (fnmadd_binary64_rne, oracle_fnmadd_binary64_rne, (_F15, _F4, _F2)),
            (fnmsub_binary64_rne, oracle_fnmsub_binary64_rne, (_F15, _F4, _F2)),
        )
        for exact_fn, oracle_fn, args in pairs:
            with self.subTest(fn=exact_fn.__name__):
                exact = exact_fn(*args)
                oracle = oracle_fn(*args)
                self.assertEqual(exact.bits64, oracle.bits64)

    def test_fused_single_family_matches_oracle_except_midpoint(self) -> None:
        for exact_fn, oracle_fn, args in (
            (fmadds_fpr_rne, oracle_fmadds_fpr_rne, (_F15, _F2, _F4)),
            (fmsubs_fpr_rne, oracle_fmsubs_fpr_rne, (_F15, _F2, _F4)),
            (fnmadds_fpr_rne, oracle_fnmadds_fpr_rne, (_F15, _F2, _F4)),
            (fnmsubs_fpr_rne, oracle_fnmsubs_fpr_rne, (_F15, _F2, _F4)),
        ):
            with self.subTest(fn=exact_fn.__name__):
                exact = exact_fn(*args)
                oracle = oracle_fn(*args)
                self.assertEqual(exact.bits64, oracle.bits64)

    def test_fmadds_midpoint_nonzero_addend_matches_dolphin_fixture(self) -> None:
        exact = fmadds_fpr_rne(
            0x4049000000000000,
            0x3B638E5400000000,
            0xBF91198700000000,
        )
        self.assertEqual(exact.bits64, 0xBFEAB7E2E0000000)
        with self.assertRaises(OracleUnimplementedError):
            oracle_fmadds_fpr_rne(
                0x4049000000000000,
                0x3B638E5400000000,
                0xBF91198700000000,
            )

    def test_legacy_fused_grader_stays_incomplete(self) -> None:
        legacy = build_fp_advanced_obligation("fp-fused-arithmetic", opcodes=["fmadd"])
        self.assertEqual(
            evaluate_fp_advanced_obligation_status(legacy),
            STATUS_INCOMPLETE,
        )

    def test_v2_fused_grader_can_promote_when_complete(self) -> None:
        obl = build_fp_fused_obligation_v2(
            opcodes=["fmadd"],
            dimensions=FUSED_V2_COMPLETE_DIMENSIONS,
            unsupported_remainder_result="unsat",
            unsupported_remainder_query_sha256="a" * 64,
            corpus_sha256="b" * 64,
            validation_ledger_hash="c" * 64,
        )
        self.assertEqual(
            evaluate_fp_fused_obligation_status(
                obl,
                live_corpus_sha256="b" * 64,
                live_validation_ledger_hash="c" * 64,
                allowlist_contains_model=True,
            ),
            STATUS_PROMOTION_GRADE,
        )

    def test_v2_fused_grader_fails_closed_on_forged_digest(self) -> None:
        obl = build_fp_fused_obligation_v2(
            opcodes=["fmadd"],
            dimensions=FUSED_V2_COMPLETE_DIMENSIONS,
            unsupported_remainder_result="unsat",
            unsupported_remainder_query_sha256="a" * 64,
            corpus_sha256="b" * 64,
            validation_ledger_hash="c" * 64,
        )
        self.assertEqual(
            evaluate_fp_fused_obligation_status(
                obl,
                live_corpus_sha256="d" * 64,
                live_validation_ledger_hash="c" * 64,
                allowlist_contains_model=True,
            ),
            STATUS_INCOMPLETE,
        )

    def test_legacy_algorithm_still_routes_to_incomplete_v0(self) -> None:
        legacy = build_fp_advanced_obligation("fp-fused-arithmetic", opcodes=["fmadd"])
        self.assertEqual(
            evaluate_fp_fused_obligation_status(legacy),
            STATUS_INCOMPLETE,
        )
        self.assertEqual(
            recompute_fp_fused_attestation_status(
                {"obligation": legacy, "host_float": False},
                capability="fp-fused-arithmetic",
                algorithm=FP_FUSED_ALGORITHM,
                model_version="broadway-fp-fused-v1",
            ),
            STATUS_INCOMPLETE,
        )


if __name__ == "__main__":
    unittest.main()
