"""Certified-calls-v2 refinement obligation tests (Wave 2)."""

from __future__ import annotations

import importlib.util
import unittest

from tools.ppc_equivalence.capability_assurance import (
    STATUS_INCOMPLETE,
    STATUS_PROMOTION_GRADE,
    CapabilityManifest,
    evaluate_capability_assurance,
)
from tools.ppc_equivalence.certified_calls_obligations import (
    REJECTION_CIRCULAR_SCC,
    REJECTION_MISSING_CALLEE_INPUT,
    REJECTION_MISSING_DEFINEDNESS,
    REJECTION_OPAQUE_EABI,
    REJECTION_STALE_CHILD,
    REJECTION_SYMBOL_TOKEN,
    REJECTION_UNRESOLVED_DIRECT,
    REJECTION_ZERO_HASH_INPUT,
    CalleeObligationInput,
    CertifiedCallsContext,
    build_certified_calls_attestation,
    build_certified_calls_obligation,
    collect_rejection_reasons,
    compute_body_sha256,
    compute_closure_sha256,
    compute_summary_sha256,
    discharge_trivial_leaf_refinement,
    draft_certified_calls_assurance,
    evaluate_certified_calls_status,
    maybe_attach_certified_calls_draft,
    obligation_promotion_eligible,
    select_used_callee_inputs,
    validate_certified_calls_obligation,
    verify_certificate_sha256_binding,
)
from tools.ppc_equivalence.model import InvalidReason
from tools.ppc_equivalence.result import (
    ARCHITECTURE_MODEL,
    MASKING_SEMANTICS,
    RESULT_FORMAT,
    MemoryScope,
    PrivateStackInfo,
    ProofResult,
    ProofStatus,
)

_HAS_Z3 = importlib.util.find_spec("z3") is not None


def _sha(seed: str) -> str:
    import hashlib

    return hashlib.sha256(seed.encode("utf-8")).hexdigest()


def _summary(**kwargs):
    base = {
        "reads": ["r3", "valid"],
        "writes": ["r3", "valid"],
        "invalid_reasons": [],
        "return_behavior": "normal",
    }
    base.update(kwargs)
    return base


def _leaf(
    target_id: str = "leaf",
    *,
    symbol: str | None = None,
    summary: dict | None = None,
    certificate_sha256: str | None = None,
    contract_source: str = "certified:deadbeef",
    required_invalid_reasons: frozenset[int] = frozenset(),
    engine_hash: str = "",
    expected_engine_hash: str = "",
    refinement: dict | None = None,
) -> CalleeObligationInput:
    summary = summary if summary is not None else _summary()
    return CalleeObligationInput(
        target_id=target_id,
        symbol=symbol or target_id,
        certificate_sha256=certificate_sha256 or _sha(f"cert:{target_id}"),
        retail_sha256=_sha(f"retail:{target_id}"),
        candidate_sha256=_sha(f"cand:{target_id}"),
        summary=summary,
        contract_source=contract_source,
        required_invalid_reasons=required_invalid_reasons,
        engine_hash=engine_hash,
        expected_engine_hash=expected_engine_hash,
        refinement=refinement,
    )


def _equivalent(**kwargs) -> ProofResult:
    defaults = dict(
        status=ProofStatus.EQUIVALENT,
        architecture_model=ARCHITECTURE_MODEL,
        format=RESULT_FORMAT,
        observables=["r3"],
        engine_hash="a" * 64,
        source_hash="b" * 64,
        git_commit="c" * 40,
        opcodes_used=["addi", "blr"],
        memory_scope=MemoryScope(
            masking_semantics=MASKING_SEMANTICS,
            original=PrivateStackInfo(enabled_on_all_terminal_paths=True),
            candidate=PrivateStackInfo(enabled_on_all_terminal_paths=True),
        ),
    )
    defaults.update(kwargs)
    return ProofResult(**defaults)


class SchemaTests(unittest.TestCase):
    def test_schema_round_trip(self) -> None:
        leaf = _leaf()
        obligation = build_certified_calls_obligation([leaf])
        self.assertIsNone(validate_certified_calls_obligation(obligation))
        restored = build_certified_calls_obligation(obligation["callees"])
        self.assertEqual(restored, obligation)
        attestation = build_certified_calls_attestation(obligation)
        self.assertEqual(attestation.capability, "certified-calls")
        self.assertEqual(attestation.model_version, "certified-calls-v2")
        self.assertEqual(
            attestation.evidence["obligation"]["closure_sha256"],
            obligation["closure_sha256"],
        )

    def test_unknown_field_fail_closed(self) -> None:
        obligation = build_certified_calls_obligation([_leaf()])
        bad = dict(obligation)
        bad["extra"] = True
        self.assertIn("unknown fields", validate_certified_calls_obligation(bad) or "")

    def test_unknown_refinement_field_fail_closed(self) -> None:
        obligation = build_certified_calls_obligation([_leaf()])
        bad = dict(obligation)
        callees = [dict(obligation["callees"][0])]
        callees[0]["refinement"] = {
            "result": "incomplete",
            "query_sha256": "0" * 64,
            "algorithm": "forged",
        }
        bad["callees"] = callees
        bad["closure_sha256"] = compute_closure_sha256(callees)
        self.assertIn("unknown fields", validate_certified_calls_obligation(bad) or "")

    def test_closure_mismatch_fail_closed(self) -> None:
        obligation = build_certified_calls_obligation([_leaf()])
        bad = dict(obligation)
        bad["closure_sha256"] = "0" * 64
        self.assertIn(
            "closure_sha256",
            validate_certified_calls_obligation(bad) or "",
        )

    def test_certificate_sha256_mutation_fail_closed(self) -> None:
        obligation = build_certified_calls_obligation([_leaf()])
        reason = verify_certificate_sha256_binding(obligation)
        self.assertIsNotNone(reason)
        self.assertIn("closure_sha256", reason or "")


class RejectionTests(unittest.TestCase):
    def test_stale_child_certificate_not_promotion_grade(self) -> None:
        leaf = _leaf(certificate_sha256=_sha("old"))
        live = {
            "leaf": {
                "certificate_sha256": _sha("new"),
                "summary": leaf.summary,
                "retail_sha256": leaf.retail_sha256,
                "candidate_sha256": leaf.candidate_sha256,
            }
        }
        reasons = collect_rejection_reasons([leaf], live_certificates=live)
        self.assertIn(REJECTION_STALE_CHILD, reasons)
        obligation = build_certified_calls_obligation(
            [leaf],
            refinements={
                "leaf": {"result": "unsat", "query_sha256": _sha("q")},
            },
        )
        self.assertFalse(
            obligation_promotion_eligible(obligation, rejection_reasons=reasons)
        )
        self.assertEqual(
            evaluate_certified_calls_status(
                obligation, rejection_reasons=reasons
            ),
            STATUS_INCOMPLETE,
        )

    def test_wrong_callee_symbol_sharing_token_fails(self) -> None:
        summary = _summary()
        digest_foo = compute_summary_sha256(
            target_id="leaf", symbol="foo", summary=summary
        )
        digest_bar = compute_summary_sha256(
            target_id="leaf", symbol="bar", summary=summary
        )
        self.assertNotEqual(digest_foo, digest_bar)
        # Claiming foo's sharing token while presenting as bar fails closed.
        leaf = _leaf(symbol="bar", summary=summary)
        reasons = collect_rejection_reasons(
            [leaf],
            live_certificates={
                "leaf": {
                    "certificate_sha256": leaf.certificate_sha256,
                    "symbol": "foo",
                    "summary": summary,
                    "summary_sha256": digest_foo,
                    "retail_sha256": leaf.retail_sha256,
                    "candidate_sha256": leaf.candidate_sha256,
                }
            },
        )
        self.assertIn(REJECTION_SYMBOL_TOKEN, reasons)
        obligation = build_certified_calls_obligation(
            [leaf],
            refinements={
                "leaf": {"result": "unsat", "query_sha256": _sha("q")},
            },
        )
        self.assertFalse(
            obligation_promotion_eligible(obligation, rejection_reasons=reasons)
        )

    def test_missing_definedness_effect_fails(self) -> None:
        leaf = _leaf(
            summary=_summary(invalid_reasons=[]),
            required_invalid_reasons=frozenset(
                {InvalidReason.DIVIDE_UNDEFINED.value}
            ),
        )
        reasons = collect_rejection_reasons([leaf])
        self.assertIn(REJECTION_MISSING_DEFINEDNESS, reasons)
        obligation = build_certified_calls_obligation([leaf])
        self.assertEqual(
            evaluate_certified_calls_status(
                obligation, rejection_reasons=reasons
            ),
            STATUS_INCOMPLETE,
        )

    def test_nested_opaque_callee_not_promotion_grade(self) -> None:
        leaf = _leaf(contract_source="nested-call-opaque-eabi")
        reasons = collect_rejection_reasons([leaf])
        self.assertIn(REJECTION_OPAQUE_EABI, reasons)
        obligation = build_certified_calls_obligation(
            [leaf],
            refinements={
                "leaf": {"result": "unsat", "query_sha256": _sha("q")},
            },
        )
        self.assertFalse(
            obligation_promotion_eligible(obligation, rejection_reasons=reasons)
        )
        attestation = build_certified_calls_attestation(
            obligation, rejection_reasons=reasons, status=STATUS_PROMOTION_GRADE
        )
        # Caller-forged promotion-grade is ignored by recompute.
        proof = _equivalent(
            assumed_callees=["leaf"],
            capability_assurance={
                "schema_version": 1,
                "policy": "capability-assurance-v1",
                "capabilities": [attestation.to_dict()],
            },
        )
        result = evaluate_capability_assurance(
            proof,
            manifest=CapabilityManifest(
                allowed_tier_a_capabilities={
                    "certified-calls": ("certified-calls-v2",),
                    "integer-core": ("integer-core-v1",),
                },
                shadow_mode=True,
            ),
        )
        self.assertEqual(
            result.recomputed_statuses.get("certified-calls"),
            STATUS_INCOMPLETE,
        )

    def test_unresolved_and_circular_scc(self) -> None:
        leaf = _leaf()
        reasons = collect_rejection_reasons(
            [leaf],
            context=CertifiedCallsContext(
                unresolved_direct_calls=True,
                call_graph={"root": frozenset({"leaf"}), "leaf": frozenset({"root"})},
                root_target_id="root",
            ),
        )
        self.assertIn(REJECTION_UNRESOLVED_DIRECT, reasons)
        self.assertIn(REJECTION_CIRCULAR_SCC, reasons)


class EmissionTests(unittest.TestCase):
    def test_emit_attestation_when_assumed_callees_present(self) -> None:
        proof = _equivalent(
            assumed_callees=["leaf"],
            callee_contracts={
                "leaf": {
                    "source": "opaque-eabi",
                    "reads": ["*"],
                    "writes": ["r3"],
                    "invalid_reasons": [],
                }
            },
        )
        maybe_attach_certified_calls_draft(
            proof, allow_zero_hash_placeholders=True,
        )
        assurance = proof.capability_assurance
        self.assertIsInstance(assurance, dict)
        caps = assurance["capabilities"]
        names = {item["capability"] for item in caps}
        self.assertIn("certified-calls", names)
        certified = next(item for item in caps if item["capability"] == "certified-calls")
        self.assertEqual(certified["model_version"], "certified-calls-v2")
        self.assertEqual(certified["algorithm"], "certified-calls-refinement-v1")
        obligation = certified["evidence"]["obligation"]
        self.assertIsNone(validate_certified_calls_obligation(obligation))
        self.assertIn(
            REJECTION_OPAQUE_EABI,
            certified["evidence"]["rejection_reasons"],
        )
        self.assertIn(
            REJECTION_ZERO_HASH_INPUT,
            certified["evidence"]["rejection_reasons"],
        )

    def test_draft_none_without_callees(self) -> None:
        proof = _equivalent(assumed_callees=[])
        self.assertIsNone(draft_certified_calls_assurance(proof))


class Stage3BProductionIntegrationTests(unittest.TestCase):
    """Certified-calls production path: real child cert inputs, no zero hashes."""

    def test_used_callee_omitted_from_attestation_incomplete(self) -> None:
        leaf = _leaf(target_id="kept", symbol="kept")
        proof = _equivalent(assumed_callees=["kept", "omitted"])
        draft = draft_certified_calls_assurance(
            proof,
            callees=[leaf],
            missing_callees=["omitted"],
            allow_zero_hash_placeholders=False,
        )
        self.assertIsNotNone(draft)
        att = draft.capabilities[0]
        self.assertEqual(att.status, STATUS_INCOMPLETE)
        reasons = att.evidence["rejection_reasons"]
        self.assertIn(REJECTION_MISSING_CALLEE_INPUT, reasons)
        self.assertEqual(att.evidence.get("missing_callees"), ["omitted"])
        # Still attaches an obligation for the attested subset.
        obligation = att.evidence.get("obligation")
        self.assertIsNotNone(obligation)
        self.assertEqual(
            [entry["target_id"] for entry in obligation["callees"]],
            ["kept"],
        )
        self.assertEqual(
            evaluate_certified_calls_status(
                obligation, rejection_reasons=reasons
            ),
            STATUS_INCOMPLETE,
        )

    def test_stale_child_certificate_not_promotion_grade(self) -> None:
        summary = {
            "reads": [],
            "writes": [],
            "invalid_reasons": [],
            "return_behavior": "normal",
        }
        leaf = _leaf(
            target_id="leaf",
            symbol="leaf",
            summary=summary,
            certificate_sha256=_sha("old"),
            refinement={"result": "unsat", "query_sha256": _sha("q")},
        )
        live = {
            "leaf": {
                "certificate_sha256": _sha("new"),
                "symbol": "leaf",
                "summary": summary,
                "summary_sha256": compute_summary_sha256(
                    target_id="leaf", symbol="leaf", summary=summary
                ),
                "retail_sha256": leaf.retail_sha256,
                "candidate_sha256": leaf.candidate_sha256,
            }
        }
        proof = _equivalent(assumed_callees=["leaf"])
        draft = draft_certified_calls_assurance(
            proof,
            callees=[leaf],
            live_certificates=live,
            allow_zero_hash_placeholders=False,
        )
        self.assertIsNotNone(draft)
        att = draft.capabilities[0]
        reasons = att.evidence["rejection_reasons"]
        self.assertIn(REJECTION_STALE_CHILD, reasons)
        self.assertNotEqual(att.status, STATUS_PROMOTION_GRADE)
        self.assertEqual(
            evaluate_certified_calls_status(
                att.evidence["obligation"], rejection_reasons=reasons
            ),
            STATUS_INCOMPLETE,
        )

    def test_trivial_blr_leaf_can_discharge_promotion_grade(self) -> None:
        summary = {
            "reads": [],
            "writes": [],
            "invalid_reasons": [],
            "return_behavior": "normal",
        }
        # Prefer live SMT discharge when z3 is present; otherwise use a
        # discharged identity-leaf refinement shape (same gate as production).
        if _HAS_Z3:
            refinement = discharge_trivial_leaf_refinement(
                target_id="nop",
                symbol="nop",
                summary=summary,
                retail_hex="4e800020",
                candidate_hex="4e800020",
            )
            self.assertEqual(refinement["result"], "unsat")
        else:
            refinement = {"result": "unsat", "query_sha256": _sha("blr-leaf")}
        leaf = CalleeObligationInput(
            target_id="nop",
            symbol="nop",
            certificate_sha256=_sha("cert:nop"),
            retail_sha256=_sha("retail:nop"),
            candidate_sha256=_sha("cand:nop"),
            summary=summary,
            contract_source=f"certified:{_sha('cert:nop')}",
            refinement=refinement,
        )
        live = {
            "nop": {
                "certificate_sha256": leaf.certificate_sha256,
                "symbol": "nop",
                "summary": summary,
                "summary_sha256": compute_summary_sha256(
                    target_id="nop", symbol="nop", summary=summary
                ),
                "retail_sha256": leaf.retail_sha256,
                "candidate_sha256": leaf.candidate_sha256,
            }
        }
        proof = _equivalent(assumed_callees=["nop"])
        draft = draft_certified_calls_assurance(
            proof,
            callees=[leaf],
            live_certificates=live,
            allow_zero_hash_placeholders=False,
        )
        self.assertIsNotNone(draft)
        att = draft.capabilities[0]
        reasons = att.evidence["rejection_reasons"]
        self.assertEqual(reasons, [])
        obligation = att.evidence["obligation"]
        self.assertTrue(
            obligation_promotion_eligible(obligation, rejection_reasons=reasons)
        )
        self.assertEqual(
            evaluate_certified_calls_status(obligation, rejection_reasons=reasons),
            STATUS_PROMOTION_GRADE,
        )

        # Nontrivial summaries without refinement stay incomplete.
        nontrivial = _leaf(
            target_id="heavy",
            symbol="heavy",
            summary=_summary(writes=["r3", "memory"]),
        )
        heavy_draft = draft_certified_calls_assurance(
            _equivalent(assumed_callees=["heavy"]),
            callees=[nontrivial],
            allow_zero_hash_placeholders=False,
        )
        self.assertIsNotNone(heavy_draft)
        heavy_att = heavy_draft.capabilities[0]
        self.assertEqual(
            evaluate_certified_calls_status(
                heavy_att.evidence["obligation"],
                rejection_reasons=heavy_att.evidence["rejection_reasons"],
            ),
            STATUS_INCOMPLETE,
        )

    def test_no_zero_hash_promotion_grade(self) -> None:
        # Production path refuses placeholders entirely.
        proof = _equivalent(assumed_callees=["leaf"])
        draft = draft_certified_calls_assurance(
            proof, allow_zero_hash_placeholders=False,
        )
        self.assertIsNotNone(draft)
        att = draft.capabilities[0]
        self.assertIsNone(att.evidence.get("obligation"))
        self.assertIn(
            REJECTION_MISSING_CALLEE_INPUT,
            att.evidence["rejection_reasons"],
        )
        self.assertEqual(
            evaluate_certified_calls_status(
                None, rejection_reasons=att.evidence["rejection_reasons"]
            ),
            STATUS_INCOMPLETE,
        )

        # Even a forged UNSAT over zero-hash digests stays non-promotion-grade.
        zero = "0" * 64
        forged = CalleeObligationInput(
            target_id="leaf",
            symbol="leaf",
            certificate_sha256=zero,
            retail_sha256=zero,
            candidate_sha256=zero,
            summary={
                "reads": [],
                "writes": [],
                "invalid_reasons": [],
                "return_behavior": "normal",
            },
            refinement={"result": "unsat", "query_sha256": _sha("forged")},
        )
        forged_draft = draft_certified_calls_assurance(
            proof,
            callees=[forged],
            allow_zero_hash_placeholders=False,
        )
        self.assertIsNotNone(forged_draft)
        forged_att = forged_draft.capabilities[0]
        self.assertIn(
            REJECTION_ZERO_HASH_INPUT,
            forged_att.evidence["rejection_reasons"],
        )
        self.assertFalse(
            obligation_promotion_eligible(
                forged_att.evidence["obligation"],
                rejection_reasons=forged_att.evidence["rejection_reasons"],
            )
        )

    def test_select_used_filters_registry_extras(self) -> None:
        kept = _leaf(target_id="a", symbol="sym_a")
        extra = _leaf(target_id="b", symbol="sym_b")
        selected, missing = select_used_callee_inputs(
            [kept, extra], ["sym_a"]
        )
        self.assertEqual([item.target_id for item in selected], ["a"])
        self.assertEqual(missing, [])
        selected, missing = select_used_callee_inputs(
            [kept], ["sym_a", "sym_missing"]
        )
        self.assertEqual([item.target_id for item in selected], ["a"])
        self.assertEqual(missing, ["sym_missing"])


class TrivialLeafRefinementTests(unittest.TestCase):
    @unittest.skipUnless(_HAS_Z3, "z3-solver is not installed")
    def test_blr_identity_leaf_discharges_unsat(self) -> None:
        summary = {
            "reads": [],
            "writes": [],
            "invalid_reasons": [],
            "return_behavior": "normal",
        }
        refinement = discharge_trivial_leaf_refinement(
            target_id="nop",
            symbol="nop",
            summary=summary,
            retail_hex="4e800020",
            candidate_hex="4e800020",
        )
        self.assertEqual(refinement["result"], "unsat")
        self.assertEqual(len(refinement["query_sha256"]), 64)

    def test_opaque_never_trivial_unsat(self) -> None:
        summary = {
            "reads": ["*"],
            "writes": ["r3", "memory"],
            "invalid_reasons": [],
            "return_behavior": "normal",
        }
        refinement = discharge_trivial_leaf_refinement(
            target_id="opaque",
            symbol="opaque",
            summary=summary,
            retail_hex="4e800020",
            candidate_hex="4e800020",
        )
        self.assertEqual(refinement["result"], "incomplete")

    def test_allowlisted_without_unsat_stays_incomplete(self) -> None:
        leaf = _leaf(
            summary={
                "reads": [],
                "writes": [],
                "invalid_reasons": [],
                "return_behavior": "normal",
            },
            refinement={"result": "incomplete", "query_sha256": _sha("inc")},
        )
        obligation = build_certified_calls_obligation([leaf])
        self.assertEqual(
            evaluate_certified_calls_status(obligation),
            STATUS_INCOMPLETE,
        )
        attestation = build_certified_calls_attestation(obligation)
        proof = _equivalent(
            assumed_callees=["leaf"],
            capability_assurance={
                "schema_version": 1,
                "policy": "capability-assurance-v1",
                "capabilities": [attestation.to_dict()],
            },
        )
        result = evaluate_capability_assurance(
            proof,
            manifest=CapabilityManifest(
                allowed_tier_a_capabilities={
                    "certified-calls": ("certified-calls-v2",),
                },
                shadow_mode=True,
            ),
        )
        self.assertEqual(
            result.recomputed_statuses["certified-calls"],
            STATUS_INCOMPLETE,
        )


class DigestTests(unittest.TestCase):
    def test_body_and_closure_stable(self) -> None:
        body = compute_body_sha256(
            retail_sha256=_sha("r"), candidate_sha256=_sha("c")
        )
        self.assertEqual(len(body), 64)
        leaf = _leaf()
        obligation = build_certified_calls_obligation([leaf])
        self.assertEqual(
            obligation["closure_sha256"],
            compute_closure_sha256(obligation["callees"]),
        )


if __name__ == "__main__":
    unittest.main()
