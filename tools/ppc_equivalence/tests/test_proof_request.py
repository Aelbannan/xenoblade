"""Unit tests for Canonical ProofRequest (PR1)."""

from __future__ import annotations

import copy
import unittest

from tools.ppc_equivalence.proof_features import (
    PROOF_OBLIGATION_FIELDS,
    ParsedProofFeatures,
    apply_proof_obligations,
    parse_proof_features,
    proof_features_from_dict,
    proof_obligations_from_dict,
    proof_obligations_from_result,
)
from tools.ppc_equivalence.proof_request import (
    ProofRequest,
    cache_key,
    canonical_request_dict,
    proof_request_hash,
)
from tools.ppc_equivalence.result import ProofResult, ProofStatus


def _sample_request(**overrides: object) -> ProofRequest:
    base = dict(
        original_hex="7c0802a6",
        candidate_hex="7c0802a6",
        original_base=0x80001234,
        candidate_base=0x80005678,
        contract="auto",
        observables=("gpr", "cr"),
        limits={"max_paths": 64, "max_instructions": 256},
        memory_environment={"profile": "assumed-mapped"},
        floating_point_domain=None,
        assumed_callees=(0x80001000, "foo"),
        callee_contracts={"foo": {"source": "opaque", "reads": [], "writes": []}},
        relocations={
            "original": [{"offset": 4, "type": "R_PPC_REL24", "symbol": "bar"}],
            "candidate": [{"offset": 4, "type": "R_PPC_REL24", "symbol": "bar"}],
        },
        proof_features=(),
        obligations={},
    )
    base.update(overrides)
    return ProofRequest(**base)  # type: ignore[arg-type]


class ProofRequestCanonicalTests(unittest.TestCase):
    def test_canonical_dict_and_hash_stable_under_key_reorder(self) -> None:
        first = _sample_request(
            observables=("cr", "gpr"),
            limits={"max_instructions": 256, "max_paths": 64},
            proof_features=("indirect-target-closure", "readonly-image"),
            obligations={
                "indirect_targets": {"targets": [{"identity": "b"}, {"identity": "a"}]},
                "address_space": {"image_sha256": "a" * 64, "kind": "rom-image"},
            },
        )
        second = _sample_request(
            observables=("gpr", "cr"),
            limits={"max_paths": 64, "max_instructions": 256},
            proof_features=("readonly-image", "indirect-target-closure"),
            obligations={
                "address_space": {"kind": "rom-image", "image_sha256": "a" * 64},
                "indirect_targets": {"targets": [{"identity": "a"}, {"identity": "b"}]},
            },
        )
        self.assertEqual(canonical_request_dict(first), canonical_request_dict(second))
        self.assertEqual(proof_request_hash(first), proof_request_hash(second))

    def test_cache_key_changes_when_candidate_hex_mutates(self) -> None:
        baseline = _sample_request()
        mutated = _sample_request(candidate_hex="48000000")
        engine = "e" * 64
        certifier = "c" * 64
        self.assertNotEqual(
            cache_key(baseline, engine, certifier),
            cache_key(mutated, engine, certifier),
        )

    def test_cache_key_changes_when_obligation_digest_mutates(self) -> None:
        baseline = _sample_request(
            proof_features=("readonly-image",),
            obligations={"address_space": {"image_sha256": "a" * 64}},
        )
        mutated = _sample_request(
            proof_features=("readonly-image",),
            obligations={"address_space": {"image_sha256": "b" * 64}},
        )
        engine = "e" * 64
        certifier = "c" * 64
        self.assertNotEqual(
            cache_key(baseline, engine, certifier),
            cache_key(mutated, engine, certifier),
        )

    def test_cache_key_changes_when_feature_set_mutates(self) -> None:
        baseline = _sample_request(proof_features=("readonly-image",))
        mutated = _sample_request(
            proof_features=("readonly-image", "indirect-target-closure"),
        )
        engine = "e" * 64
        certifier = "c" * 64
        self.assertNotEqual(
            cache_key(baseline, engine, certifier),
            cache_key(mutated, engine, certifier),
        )

    def test_cache_key_changes_when_engine_or_certifier_hash_mutates(self) -> None:
        request = _sample_request()
        self.assertNotEqual(
            cache_key(request, "e" * 64, "c" * 64),
            cache_key(request, "f" * 64, "c" * 64),
        )
        self.assertNotEqual(
            cache_key(request, "e" * 64, "c" * 64),
            cache_key(request, "e" * 64, "d" * 64),
        )


class ProofObligationHelperTests(unittest.TestCase):
    def test_obligation_fields_cover_known_keys(self) -> None:
        self.assertEqual(
            PROOF_OBLIGATION_FIELDS,
            (
                "address_space",
                "indirect_targets",
                "loop_summary",
                "relational_induction",
                "memory_loop",
                "memory_bus",
            ),
        )

    def test_obligation_helpers_round_trip_result(self) -> None:
        source = ProofResult(
            status=ProofStatus.INCONCLUSIVE_UNSUPPORTED,
            proof_features=["readonly-image", "memory-bus"],
            address_space={"image_sha256": "a" * 64},
            memory_bus={"algorithm": "memory-bus-v1", "regions": [{"kind": "ram"}]},
            loop_summary={"proof_kind": "affine-closed-form"},
        )
        obligations = proof_obligations_from_result(source)
        self.assertEqual(
            set(obligations),
            {"address_space", "memory_bus", "loop_summary"},
        )

        target = ProofResult(status=ProofStatus.EQUIVALENT)
        apply_proof_obligations(target, obligations)
        self.assertEqual(target.address_space, source.address_space)
        self.assertEqual(target.memory_bus, source.memory_bus)
        self.assertEqual(target.loop_summary, source.loop_summary)
        self.assertIsNone(target.indirect_targets)

    def test_obligation_helpers_round_trip_dict(self) -> None:
        payload = {
            "proof_features": ["relational-induction", "memory-loop-summary"],
            "relational_induction": {"status": "applied", "proof_kind": "relational-induction"},
            "memory_loop": {"proof_kind": "constant-stride-store", "trip_count": 4},
            "indirect_targets": "not-a-dict",
        }
        obligations = proof_obligations_from_dict(payload)
        self.assertEqual(
            set(obligations),
            {"relational_induction", "memory_loop"},
        )

        parsed = parse_proof_features(payload)
        self.assertIsInstance(parsed, ParsedProofFeatures)
        self.assertEqual(
            parsed.features,
            ("relational-induction", "memory-loop-summary"),
        )
        self.assertEqual(parsed.obligations, obligations)

        # Back-compat wrapper still returns only jump-table pair fields.
        features, address_space, indirect_targets = proof_features_from_dict(payload)
        self.assertEqual(features, list(parsed.features))
        self.assertIsNone(address_space)
        self.assertIsNone(indirect_targets)

    def test_apply_proof_obligations_ignores_unknown_keys(self) -> None:
        result = ProofResult(status=ProofStatus.EQUIVALENT)
        apply_proof_obligations(
            result,
            {
                "address_space": {"regions": []},
                "not_a_field": {"x": 1},  # type: ignore[dict-item]
            },
        )
        self.assertEqual(result.address_space, {"regions": []})
        self.assertFalse(hasattr(result, "not_a_field"))

    def test_deep_copy_safe_for_canonical_obligations(self) -> None:
        request = _sample_request(
            obligations={"address_space": {"image_sha256": "a" * 64, "extra": [1, 2]}},
        )
        canonical = canonical_request_dict(request)
        mutated = copy.deepcopy(canonical)
        mutated["obligations"]["address_space"]["image_sha256"] = "b" * 64
        self.assertEqual(
            request.obligations["address_space"]["image_sha256"],
            "a" * 64,
        )
        self.assertNotEqual(
            proof_request_hash(request),
            proof_request_hash(
                _sample_request(
                    obligations={"address_space": {"image_sha256": "b" * 64, "extra": [1, 2]}},
                )
            ),
        )


if __name__ == "__main__":
    unittest.main()
