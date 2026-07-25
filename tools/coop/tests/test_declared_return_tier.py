"""C7: declared-return ABI-shape narrowing caps confidence at Tier C.

Tests §2.9 of docs/ppc_equiv_work/29-declared-return-abi-shapes.md:
_compute_confidence_tier_legacy must cap the tier at "C" when
result.contract_resolution["abi_shape"]["declared_return"] is truthy.
"""

from __future__ import annotations

import unittest

from tools.coop.lib.equivalence_policy import _compute_confidence_tier_legacy
from tools.ppc_equivalence.result import (
    ARCHITECTURE_MODEL,
    MASKING_SEMANTICS,
    RESULT_FORMAT,
    MemoryScope,
    PrivateStackInfo,
    ProofResult,
    ProofStatus,
)


def _tier_a_proof(**overrides) -> ProofResult:
    """A minimal register-only proof that would otherwise classify as Tier A.

    No FP, no memory access, no callees, no domain exceptions, complete
    provenance, and a valid memory scope with standard masking semantics.
    """
    defaults = dict(
        status=ProofStatus.EQUIVALENT,
        observables=["r3"],
        architecture_model=ARCHITECTURE_MODEL,
        format=RESULT_FORMAT,
        engine_hash="a" * 64,
        certifier_hash="b" * 64,
        source_hash="c" * 64,
        proof_request_hash="d" * 64,
        validation_ledger_hash="e" * 64,
        git_commit="f" * 40,
        git_dirty=False,
        memory_scope=MemoryScope(
            masking_semantics=MASKING_SEMANTICS,
            original=PrivateStackInfo(enabled_on_all_terminal_paths=True),
            candidate=PrivateStackInfo(enabled_on_all_terminal_paths=True),
        ),
    )
    defaults.update(overrides)
    return ProofResult(**defaults)


def _tier_b_proof(**overrides) -> ProofResult:
    """A minimal memory-access proof that would otherwise classify as Tier B.

    Adds "memory" to observables (losing Tier A) without triggering any
    Tier-C condition (environment is None, so assumed-RAM is false).
    """
    return _tier_a_proof(
        observables=["r3", "memory"],
        **overrides,
    )


class DeclaredReturnTierCapTest(unittest.TestCase):
    """C7: declared-return narrowing must cap tier at C."""

    # -- Tier A base cases -----------------------------------------------

    def test_tier_a_without_declared_return(self) -> None:
        """Register-only proof with no contract_resolution → Tier A."""
        result = _tier_a_proof()
        tier = _compute_confidence_tier_legacy(result)
        self.assertEqual(tier, "A")

    def test_tier_a_without_abi_shape_key(self) -> None:
        """contract_resolution with no abi_shape key → no cap, stays A."""
        result = _tier_a_proof(contract_resolution={"some_other": "data"})
        tier = _compute_confidence_tier_legacy(result)
        self.assertEqual(tier, "A")

    def test_tier_a_with_none_declared_return(self) -> None:
        """declared_return=None is falsy → no cap, stays A."""
        result = _tier_a_proof(
            contract_resolution={"abi_shape": {"declared_return": None}}
        )
        tier = _compute_confidence_tier_legacy(result)
        self.assertEqual(tier, "A")

    def test_tier_a_with_empty_declared_return(self) -> None:
        """declared_return="" is falsy → no cap, stays A."""
        result = _tier_a_proof(
            contract_resolution={"abi_shape": {"declared_return": ""}}
        )
        tier = _compute_confidence_tier_legacy(result)
        self.assertEqual(tier, "A")

    def test_tier_a_with_void_capped_to_c(self) -> None:
        """declared_return="void" on an A-qualifying proof → Tier C."""
        result = _tier_a_proof(
            contract_resolution={"abi_shape": {"declared_return": "void"}}
        )
        tier = _compute_confidence_tier_legacy(result)
        self.assertEqual(tier, "C")

    def test_tier_a_with_i32_capped_to_c(self) -> None:
        """declared_return="i32" on an A-qualifying proof → Tier C."""
        result = _tier_a_proof(
            contract_resolution={"abi_shape": {"declared_return": "i32"}}
        )
        tier = _compute_confidence_tier_legacy(result)
        self.assertEqual(tier, "C")

    # -- Tier B base cases -----------------------------------------------

    def test_tier_b_without_declared_return(self) -> None:
        """Memory proof with no contract_resolution → Tier B."""
        result = _tier_b_proof()
        tier = _compute_confidence_tier_legacy(result)
        self.assertEqual(tier, "B")

    def test_tier_b_with_void_capped_to_c(self) -> None:
        """declared_return="void" on a B-qualifying proof → Tier C."""
        result = _tier_b_proof(
            contract_resolution={"abi_shape": {"declared_return": "void"}}
        )
        tier = _compute_confidence_tier_legacy(result)
        self.assertEqual(tier, "C")

    def test_tier_b_with_null_abi_shape_no_cap(self) -> None:
        """Missing abi_shape key → no cap, stays B."""
        result = _tier_b_proof(contract_resolution={})
        tier = _compute_confidence_tier_legacy(result)
        self.assertEqual(tier, "B")

    # -- Not-equivalent proofs stay None despite declared_return ----------

    def test_not_equivalent_ignores_declared_return(self) -> None:
        """Non-EQUIVALENT proof returns None regardless of declared_return."""
        result = _tier_a_proof(
            status=ProofStatus.NOT_EQUIVALENT,
            contract_resolution={"abi_shape": {"declared_return": "void"}},
        )
        tier = _compute_confidence_tier_legacy(result)
        self.assertIsNone(tier)

    # -- Already C stays C -----------------------------------------------

    def test_already_tier_c_stays_c(self) -> None:
        """A proof already at Tier C stays C with declared_return present."""
        result = _tier_a_proof(
            # Trigger Tier C via incomplete provenance
            engine_hash="",
            certifier_hash="",
            source_hash="",
            git_commit="",
            contract_resolution={"abi_shape": {"declared_return": "void"}},
        )
        tier = _compute_confidence_tier_legacy(result)
        self.assertEqual(tier, "C")

    def test_null_contract_resolution_no_cap(self) -> None:
        """contract_resolution=None → no cap, stays A."""
        result = _tier_a_proof(contract_resolution=None)
        tier = _compute_confidence_tier_legacy(result)
        self.assertEqual(tier, "A")


if __name__ == "__main__":
    unittest.main()
