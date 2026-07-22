"""Phase 6/12 fp-load-store-exact-v2 obligation tests."""

from __future__ import annotations

import copy
import unittest

from tools.ppc_equivalence.capability_assurance import (
    STATUS_INCOMPLETE,
    STATUS_PROMOTION_GRADE,
)
from tools.ppc_equivalence.capability_attachment import (
    FP_LOAD_STORE_ALGORITHM,
    draft_fp_capability_attestations,
)
from tools.ppc_equivalence.fp_capabilities import (
    FP_LOAD_STORE_MODEL_VERSION,
    set_scalar_fp_exact_v2_module_flag,
    set_scalar_fp_exact_v2_production_module_flag,
)
from tools.ppc_equivalence.fp_load_store_obligations import (
    FP_LOAD_STORE_BIT_TRANSFORM_OPS,
    LOAD_STORE_BIT_V2_DIMENSIONS,
    build_fp_load_store_obligation_v2,
    evaluate_fp_load_store_obligation_status,
    recompute_fp_load_store_attestation_status,
)
from tools.ppc_equivalence.fp_scalar_obligations_v2 import (
    FP_LOAD_STORE_EXACT_V2_ALGORITHM,
    validate_fp_scalar_obligation_v2,
)
from tools.ppc_equivalence.result import (
    ARCHITECTURE_MODEL,
    RESULT_FORMAT,
    FloatingPointDomain,
    ProofResult,
    ProofStatus,
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


def _equivalent(**kwargs) -> ProofResult:
    defaults = dict(
        status=ProofStatus.EQUIVALENT,
        architecture_model=ARCHITECTURE_MODEL,
        format=RESULT_FORMAT,
        engine_hash="a" * 64,
        certifier_hash="d" * 64,
        source_hash="b" * 64,
        proof_request_hash="b" * 64,
        validation_ledger_hash="c" * 64,
        git_commit="c" * 40,
        git_dirty=False,
        opcodes_used=["blr"],
        observables=["r3"],
    )
    defaults.update(kwargs)
    return ProofResult(**defaults)


class FPLoadStoreObligationsTests(unittest.TestCase):
    def _complete_obligation(self, *, opcodes: list[str] | None = None) -> dict:
        return build_fp_load_store_obligation_v2(
            opcodes=opcodes or ["stfs"],
            dimensions=LOAD_STORE_BIT_V2_DIMENSIONS,
            unsupported_remainder_result="unsat",
            unsupported_remainder_query_sha256="a" * 64,
            corpus_sha256="b" * 64,
            validation_ledger_hash="c" * 64,
        )

    def test_algorithm_identity(self) -> None:
        obl = build_fp_load_store_obligation_v2(opcodes=["lfs"])
        self.assertEqual(obl["algorithm"], FP_LOAD_STORE_EXACT_V2_ALGORITHM)

    def test_validate_accepts_well_formed_v2(self) -> None:
        self.assertIsNone(validate_fp_scalar_obligation_v2(self._complete_obligation()))

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
                    forged[field] = "broadway-fp-load-store-v1"
                elif field == "algorithm":
                    forged[field] = FP_LOAD_STORE_ALGORITHM
                elif field == "domain":
                    forged[field] = {"no_host_float": False}
                elif field == "opcodes":
                    forged[field] = ["fadd"]
                elif field == "modes":
                    forged[field]["rn"] = ["bogus-mode"]
                elif field == "dimensions":
                    forged[field]["stfs_rounding"] = False
                elif field == "coverage":
                    forged[field]["unsupported_remainder"]["result"] = "sat"
                if field in ("dimensions", "coverage"):
                    self.assertEqual(
                        evaluate_fp_load_store_obligation_status(
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

    def test_promotion_with_allowlist_unsat_and_digests(self) -> None:
        obl = self._complete_obligation(opcodes=["stfs"])
        self.assertEqual(
            evaluate_fp_load_store_obligation_status(
                obl,
                live_corpus_sha256="b" * 64,
                live_validation_ledger_hash="c" * 64,
                allowlist_contains_model=True,
            ),
            STATUS_PROMOTION_GRADE,
        )

    def test_stfs_only_vacuous_remainder_promotes(self) -> None:
        obl = build_fp_load_store_obligation_v2(
            opcodes=["stfs"],
            dimensions=LOAD_STORE_BIT_V2_DIMENSIONS,
            unsupported_remainder_result="vacuous",
            unsupported_remainder_query_sha256="a" * 64,
            corpus_sha256="b" * 64,
            validation_ledger_hash="c" * 64,
        )
        self.assertEqual(
            evaluate_fp_load_store_obligation_status(
                obl,
                live_corpus_sha256="b" * 64,
                live_validation_ledger_hash="c" * 64,
                allowlist_contains_model=True,
            ),
            STATUS_PROMOTION_GRADE,
        )

    def test_vacuous_remainder_rejected_for_non_stfs_only(self) -> None:
        obl = build_fp_load_store_obligation_v2(
            opcodes=["lfs", "stfs"],
            dimensions=LOAD_STORE_BIT_V2_DIMENSIONS,
            unsupported_remainder_result="vacuous",
            unsupported_remainder_query_sha256="a" * 64,
            corpus_sha256="b" * 64,
            validation_ledger_hash="c" * 64,
        )
        self.assertEqual(
            evaluate_fp_load_store_obligation_status(
                obl,
                live_corpus_sha256="b" * 64,
                live_validation_ledger_hash="c" * 64,
                allowlist_contains_model=True,
            ),
            STATUS_INCOMPLETE,
        )

    def test_indexed_opcode_blocks_promotion(self) -> None:
        obl = build_fp_load_store_obligation_v2(
            opcodes=["lfsu"],
            dimensions=LOAD_STORE_BIT_V2_DIMENSIONS,
            unsupported_remainder_result="unsat",
            unsupported_remainder_query_sha256="a" * 64,
            corpus_sha256="b" * 64,
            validation_ledger_hash="c" * 64,
        )
        self.assertEqual(
            evaluate_fp_load_store_obligation_status(
                obl,
                live_corpus_sha256="b" * 64,
                live_validation_ledger_hash="c" * 64,
                allowlist_contains_model=True,
            ),
            STATUS_INCOMPLETE,
        )

    def test_bounded_memory_false_does_not_block_bit_path(self) -> None:
        dims = dict(LOAD_STORE_BIT_V2_DIMENSIONS)
        dims["bounded_memory"] = False
        obl = build_fp_load_store_obligation_v2(
            opcodes=["stfs"],
            dimensions=dims,
            unsupported_remainder_result="unsat",
            unsupported_remainder_query_sha256="a" * 64,
            corpus_sha256="b" * 64,
            validation_ledger_hash="c" * 64,
        )
        self.assertEqual(
            evaluate_fp_load_store_obligation_status(
                obl,
                live_corpus_sha256="b" * 64,
                live_validation_ledger_hash="c" * 64,
                allowlist_contains_model=True,
            ),
            STATUS_PROMOTION_GRADE,
        )

    def test_incomplete_v0_never_promotes(self) -> None:
        self.assertEqual(
            recompute_fp_load_store_attestation_status(
                {
                    "host_float": False,
                    "obligation": {
                        "algorithm": FP_LOAD_STORE_ALGORITHM,
                        "status": STATUS_INCOMPLETE,
                    },
                },
                capability="fp-load-store",
                algorithm=FP_LOAD_STORE_ALGORITHM,
                model_version=FP_LOAD_STORE_MODEL_VERSION,
                allowed_versions=[FP_LOAD_STORE_MODEL_VERSION],
            ),
            STATUS_INCOMPLETE,
        )

    def test_host_float_forces_incomplete(self) -> None:
        obl = self._complete_obligation()
        self.assertEqual(
            evaluate_fp_load_store_obligation_status(obl, host_float=True),
            STATUS_INCOMPLETE,
        )

    def test_attachment_uses_incomplete_when_flag_off(self) -> None:
        set_scalar_fp_exact_v2_module_flag(False)
        set_scalar_fp_exact_v2_production_module_flag(False)
        try:
            proof = _equivalent(
                floating_point_domain=FloatingPointDomain(),
                opcodes_used=["lfs", "blr"],
                observables=["f1"],
            )
            att = draft_fp_capability_attestations(proof)
            by_name = {item.capability: item for item in att}
            self.assertEqual(by_name["fp-load-store"].algorithm, FP_LOAD_STORE_ALGORITHM)
        finally:
            set_scalar_fp_exact_v2_module_flag(None)
            set_scalar_fp_exact_v2_production_module_flag(None)

    def test_attachment_uses_exact_v2_when_flag_on(self) -> None:
        set_scalar_fp_exact_v2_module_flag(True)
        set_scalar_fp_exact_v2_production_module_flag(False)
        proof = _equivalent(
            floating_point_domain=FloatingPointDomain(),
            opcodes_used=["stfs", "blr"],
            observables=["f1"],
        )
        try:
            att = draft_fp_capability_attestations(proof)
            by_name = {item.capability: item for item in att}
            load_store = by_name["fp-load-store"]
            self.assertEqual(load_store.algorithm, FP_LOAD_STORE_EXACT_V2_ALGORITHM)
            self.assertFalse(load_store.evidence.get("host_float"))
            obligation = load_store.evidence.get("obligation") or {}
            remainder = (obligation.get("coverage") or {}).get(
                "unsupported_remainder"
            ) or {}
            self.assertEqual(remainder.get("result"), "vacuous")
            corpus = (obligation.get("coverage") or {}).get("corpus_sha256") or ""
            self.assertEqual(
                recompute_fp_load_store_attestation_status(
                    load_store.evidence,
                    capability="fp-load-store",
                    algorithm=FP_LOAD_STORE_EXACT_V2_ALGORITHM,
                    model_version=FP_LOAD_STORE_MODEL_VERSION,
                    allowed_versions=[FP_LOAD_STORE_MODEL_VERSION],
                    live_corpus_sha256=corpus,
                ),
                STATUS_PROMOTION_GRADE,
            )
        finally:
            set_scalar_fp_exact_v2_module_flag(None)
            set_scalar_fp_exact_v2_production_module_flag(None)

    def test_recompute_attestation_status_promotes_with_allowlist(self) -> None:
        obl = self._complete_obligation(opcodes=["stfs"])
        status = recompute_fp_load_store_attestation_status(
            {
                "obligation": obl,
                "opcodes": ["stfs"],
                "host_float": False,
            },
            capability="fp-load-store",
            algorithm=FP_LOAD_STORE_EXACT_V2_ALGORITHM,
            model_version=FP_LOAD_STORE_MODEL_VERSION,
            allowed_versions=[FP_LOAD_STORE_MODEL_VERSION],
            live_corpus_sha256="b" * 64,
            live_validation_ledger_hash="c" * 64,
        )
        self.assertEqual(status, STATUS_PROMOTION_GRADE)

    def test_bit_transform_opcode_set_matches_kernel(self) -> None:
        self.assertEqual(
            FP_LOAD_STORE_BIT_TRANSFORM_OPS,
            frozenset({"lfs", "lfd", "stfs", "stfd", "stfiwx"}),
        )


if __name__ == "__main__":
    unittest.main()
