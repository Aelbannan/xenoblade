"""Two-phase object-base-mem1 prove: justification, tier caps, provenance.

Regression coverage for the unsound-blanket-range bug: entry r3 was ranged to
MEM1 for every proof, which masked a real inequivalence on us-80137c30
(``func_801372B4(int)`` — the range assumed away the buggy input range).
"""

from __future__ import annotations

import unittest

from tools.coop.lib.equivalence_check import (
    _OBJECT_BASE_RETRY_STATUSES,
    _object_base_r3_justification,
)
from tools.coop.lib.equivalence_policy import (
    _cap_tier,
    _object_base_mem1_tier_cap,
    compute_confidence_tier_from_certificate,
)
from tools.ppc_equivalence.provenance import proof_request_hash
from tools.ppc_equivalence.result import ProofStatus

_MEM1 = "object-base-mem1:r3:[0x80000000,0x817fffff]"


class JustificationTests(unittest.TestCase):
    def test_cpp_method_is_justified_this(self) -> None:
        self.assertEqual(
            _object_base_r3_justification("CfObject_UnkVirtualFunc22__Q22cf8CfObjectFv"),
            "cpp-method-this",
        )
        self.assertEqual(
            _object_base_r3_justification("setCurrent__5CViewFv"),
            "cpp-method-this",
        )

    def test_free_function_pointer_first_param(self) -> None:
        self.assertEqual(
            _object_base_r3_justification("__prep_buffer__FP4FILE"),
            "mangled-first-param-pointer",
        )

    def test_free_function_int_first_param_not_justified(self) -> None:
        self.assertIsNone(_object_base_r3_justification("func_801372B4__Fi"))

    def test_unmangled_and_placeholder_names_not_justified(self) -> None:
        self.assertIsNone(_object_base_r3_justification("func_801372B4"))
        self.assertIsNone(_object_base_r3_justification("BTM_AclRegisterForChanges"))
        self.assertIsNone(_object_base_r3_justification(""))


class TierCapTests(unittest.TestCase):
    def test_no_range_no_cap(self) -> None:
        self.assertIsNone(_object_base_mem1_tier_cap([]))
        self.assertIsNone(_object_base_mem1_tier_cap(None))
        self.assertIsNone(_object_base_mem1_tier_cap(["domain-exception:1"]))

    def test_unjustified_range_caps_at_c(self) -> None:
        self.assertEqual(_object_base_mem1_tier_cap([_MEM1]), "C")
        self.assertEqual(
            _object_base_mem1_tier_cap([_MEM1, "object-base-mem1-unjustified"]), "C"
        )

    def test_justified_range_caps_at_b(self) -> None:
        self.assertEqual(
            _object_base_mem1_tier_cap([_MEM1, "object-base-mem1-justified:cpp-method-this"]),
            "B",
        )

    def test_cap_tier_demotes_only(self) -> None:
        self.assertEqual(_cap_tier("A", "C"), "C")
        self.assertEqual(_cap_tier("B", "C"), "C")
        self.assertEqual(_cap_tier("C", "B"), "C")
        self.assertEqual(_cap_tier("A", "B"), "B")
        self.assertEqual(_cap_tier("B", "B"), "B")
        self.assertEqual(_cap_tier("A", None), "A")
        self.assertIsNone(_cap_tier(None, "C"))

    def test_certificate_tier_reads_assumptions(self) -> None:
        cert = {
            "status": "SEMANTIC_CERTIFIED",
            "summary": {"reads": [], "writes": ["r3"], "invalid_reasons": []},
            "callees": [],
            "assumptions": [_MEM1],
        }
        # Register-only cert would be Tier A without the range; the unjustified
        # mem1 assumption must cap it at Tier C.
        self.assertEqual(compute_confidence_tier_from_certificate(cert), "C")
        cert["assumptions"] = [_MEM1, "object-base-mem1-justified:cpp-method-this"]
        self.assertEqual(compute_confidence_tier_from_certificate(cert), "B")
        cert["assumptions"] = []
        self.assertEqual(compute_confidence_tier_from_certificate(cert), "A")


class ProvenanceBindingTests(unittest.TestCase):
    def test_gpr_ranges_change_proof_request_hash(self) -> None:
        base = proof_request_hash(original_hex="4e800020", candidate_hex="4e800020", contract="auto")
        ranged = proof_request_hash(
            original_hex="4e800020",
            candidate_hex="4e800020",
            contract="auto",
            initial_gpr_ranges={"3": [0x80000000, 0x817FFFFF]},
        )
        other = proof_request_hash(
            original_hex="4e800020",
            candidate_hex="4e800020",
            contract="auto",
            initial_gpr_ranges={"4": [0x80000000, 0x817FFFFF]},
        )
        self.assertNotEqual(base, ranged)
        self.assertNotEqual(ranged, other)


class RetryStatusTests(unittest.TestCase):
    def test_retry_statuses_are_resource_limits_only(self) -> None:
        self.assertIn(ProofStatus.INCONCLUSIVE_TIMEOUT, _OBJECT_BASE_RETRY_STATUSES)
        self.assertIn(ProofStatus.INCONCLUSIVE_UNKNOWN, _OBJECT_BASE_RETRY_STATUSES)
        self.assertNotIn(ProofStatus.NOT_EQUIVALENT, _OBJECT_BASE_RETRY_STATUSES)
        self.assertNotIn(ProofStatus.INCONCLUSIVE_UNSUPPORTED, _OBJECT_BASE_RETRY_STATUSES)
        self.assertNotIn(
            ProofStatus.INCONCLUSIVE_UNVALIDATED_CALLEE, _OBJECT_BASE_RETRY_STATUSES
        )
        self.assertNotIn(ProofStatus.EQUIVALENT, _OBJECT_BASE_RETRY_STATUSES)


if __name__ == "__main__":
    unittest.main()
