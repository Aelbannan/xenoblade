"""Jump-table obligation binding in proof-request and cache identity."""

from __future__ import annotations

import copy
import unittest

from tools.coop.lib.equivalence_check import _cache_key
from tools.coop.lib.targets import equivalence_certificate_hash
from tools.ppc_equivalence.jump_table_obligations import (
    JumpTableWords,
    build_indirect_targets_obligation,
    build_readonly_image_obligation,
)
from tools.ppc_equivalence.proof_features import validate_proof_features
from tools.ppc_equivalence.provenance import (
    canonical_obligation_dict,
    proof_request_hash,
    proof_request_identity,
)
from tools.ppc_equivalence.result import ARCHITECTURE_MODEL, RESULT_FORMAT


def _sample_obligations() -> tuple[list[str], dict, dict]:
    table = JumpTableWords(
        base=0x80010000,
        words=(0x80020000, 0x80020010),
        source="linked-elf",
        artifact_path="fixture.elf",
    )
    address_space = build_readonly_image_obligation(table, no_write_status="unsat")
    indirect_targets = build_indirect_targets_obligation(
        branch_pc=0x80001234,
        targets=(("case0", 0x80020000), ("case1", 0x80020010)),
        source="linked-elf",
        artifact_hashes=(address_space["image_sha256"],),
        coverage="unsat-remainder",
    )
    proof_features = ["indirect-target-closure", "readonly-image"]
    return proof_features, address_space, indirect_targets


class JumpTableIdentityTests(unittest.TestCase):
    def test_legacy_identity_unchanged_without_features(self) -> None:
        legacy = proof_request_hash(
            original_hex="aa",
            candidate_hex="bb",
            contract="ppc-eabi",
        )
        explicit_none = proof_request_hash(
            original_hex="aa",
            candidate_hex="bb",
            contract="ppc-eabi",
            proof_features=None,
            address_space=None,
            indirect_targets=None,
        )
        self.assertEqual(legacy, explicit_none)
        payload = proof_request_identity(
            original_hex="aa",
            candidate_hex="bb",
            contract="ppc-eabi",
        )
        self.assertNotIn("proof_features", payload)
        self.assertNotIn("address_space", payload)
        self.assertNotIn("indirect_targets", payload)

    def test_image_sha256_affects_proof_request_hash(self) -> None:
        proof_features, address_space, indirect_targets = _sample_obligations()
        base = dict(
            original_hex="aa",
            candidate_hex="bb",
            contract="ppc-eabi",
            proof_features=proof_features,
            address_space=address_space,
            indirect_targets=indirect_targets,
        )
        first = proof_request_hash(**base)
        mutated = copy.deepcopy(address_space)
        mutated["image_sha256"] = "b" * 64
        second_base = {**base, "address_space": mutated}
        second = proof_request_hash(**second_base)
        self.assertNotEqual(first, second)

    def test_target_set_affects_proof_request_hash(self) -> None:
        proof_features, address_space, indirect_targets = _sample_obligations()
        base = dict(
            original_hex="aa",
            candidate_hex="bb",
            contract="ppc-eabi",
            proof_features=proof_features,
            address_space=address_space,
            indirect_targets=indirect_targets,
        )
        first = proof_request_hash(**base)
        mutated = build_indirect_targets_obligation(
            branch_pc=indirect_targets["branch_pc"],
            targets=(
                ("case0", 0x80020000),
                ("case1", 0x80020010),
                ("case2", 0x80020020),
            ),
            source=indirect_targets["source"],
            artifact_hashes=indirect_targets["artifact_hashes"],
            coverage=indirect_targets["coverage"],
        )
        second_base = {**base, "indirect_targets": mutated}
        second = proof_request_hash(**second_base)
        self.assertNotEqual(first, second)

    def test_obligation_targets_canonicalized_by_identity(self) -> None:
        proof_features, address_space, indirect_targets = _sample_obligations()
        reordered = build_indirect_targets_obligation(
            branch_pc=indirect_targets["branch_pc"],
            targets=(("case1", 0x80020010), ("case0", 0x80020000)),
            source=indirect_targets["source"],
            artifact_hashes=indirect_targets["artifact_hashes"],
            coverage=indirect_targets["coverage"],
        )
        first = proof_request_hash(
            original_hex="aa",
            candidate_hex="bb",
            contract="ppc-eabi",
            proof_features=proof_features,
            address_space=address_space,
            indirect_targets=indirect_targets,
        )
        second = proof_request_hash(
            original_hex="aa",
            candidate_hex="bb",
            contract="ppc-eabi",
            proof_features=list(reversed(proof_features)),
            address_space=address_space,
            indirect_targets=reordered,
        )
        self.assertEqual(first, second)
        self.assertEqual(
            canonical_obligation_dict(reordered)["targets"],
            canonical_obligation_dict(indirect_targets)["targets"],
        )

    def test_cache_key_changes_with_obligations(self) -> None:
        proof_features, address_space, indirect_targets = _sample_obligations()
        common = dict(
            contract_name="ppc-eabi",
            observables=("r3",),
            original_hex="aa",
            candidate_hex="bb",
            original_base=0x80000000,
            candidate_base=0x80000000,
            engine_hash="e" * 64,
            certifier_hash="c" * 64,
        )
        legacy = _cache_key(**common)
        bound = _cache_key(
            **common,
            proof_features=proof_features,
            address_space=address_space,
            indirect_targets=indirect_targets,
        )
        self.assertNotEqual(legacy, bound)
        mutated = copy.deepcopy(address_space)
        mutated["image_sha256"] = "d" * 64
        changed = _cache_key(
            **common,
            proof_features=proof_features,
            address_space=mutated,
            indirect_targets=indirect_targets,
        )
        self.assertNotEqual(bound, changed)

    def test_certificate_round_trip_includes_obligations(self) -> None:
        proof_features, address_space, indirect_targets = _sample_obligations()
        certificate = {
            "version": 2,
            "status": "SEMANTIC_CERTIFIED",
            "architecture": ARCHITECTURE_MODEL,
            "result_format": RESULT_FORMAT,
            "target_id": "us-test",
            "evidence": "symbolic-equivalence",
            "retail_sha256": "a" * 64,
            "candidate_sha256": "b" * 64,
            "summary": {
                "reads": ["memory"],
                "writes": ["memory"],
                "invalid_reasons": [],
                "return_behavior": "normal",
            },
            "callees": [],
            "helpers": [],
            "engine_hash": "e" * 64,
            "certifier_hash": "c" * 64,
            "proof_features": proof_features,
            "address_space": address_space,
            "indirect_targets": indirect_targets,
        }
        certificate["certificate_sha256"] = equivalence_certificate_hash(certificate)
        self.assertIsNone(validate_proof_features(certificate))
        self.assertEqual(certificate["proof_features"], proof_features)
        self.assertEqual(certificate["address_space"]["image_sha256"], address_space["image_sha256"])
        self.assertEqual(len(certificate["indirect_targets"]["targets"]), 2)

        tampered = copy.deepcopy(certificate)
        tampered["address_space"]["image_sha256"] = "f" * 64
        self.assertNotEqual(
            certificate["certificate_sha256"],
            equivalence_certificate_hash(tampered),
        )


if __name__ == "__main__":
    unittest.main()
