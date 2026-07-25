"""Equivalence proofs with declared-return ABI shape narrowing (§4 P1–N4).

Test that an attached ``declared_return`` narrowing correctly drops/keeps
``r4`` / ``f1`` on return/fallthrough exits, and that indirect-branch exits
are never narrowed.
"""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.abi_infer import (
    abi_shape_from_declared_return,
    combine_abi_shapes,
    infer_abi_shape,
)
from tools.ppc_equivalence.abi_shape import AbiShape
from tools.ppc_equivalence.contract import make_contract, with_abi_shape
from tools.ppc_equivalence.decoder import decode_block, parse_hex
from tools.ppc_equivalence.engine import check_equivalence
from tools.ppc_equivalence.result import ProofStatus
from tools.ppc_equivalence.semantics import automatic_live_out


# ---------------------------------------------------------------------------
# P1 — __prep_buffer shaped pair (Chaitin register rotation)
# Both load three values from memory, store them to different offsets of r3,
# then return with r3=0.  The loads/stores use a Chaitin-rotated register
# assignment so the memory is equivalent; only r4 differs at exit.
# ---------------------------------------------------------------------------
# Retail: r4=position, r6=alignment, r5=size
#   lwz r4,0(r3); lwz r6,4(r3); lwz r5,8(r3)
#   stw r4,24(r3); stw r6,36(r3); stw r5,40(r3); stw r6,52(r3)
#   li r3,0; blr
_PREP_RETAIL = (
    "80830000 80C30004 80A30008 "
    "90830018 90C30024 90A30028 90C30034 "
    "38600000 4E800020"
)
# Candidate: r5=position, r4=alignment, r6=size
#   lwz r5,0(r3); lwz r4,4(r3); lwz r6,8(r3)
#   stw r5,24(r3); stw r4,36(r3); stw r6,40(r3); stw r4,52(r3)
#   li r3,0; blr
_PREP_CANDIDATE = (
    "80A30000 80830004 80C30008 "
    "90A30018 90830024 90C30028 90830034 "
    "38600000 4E800020"
)

# ---------------------------------------------------------------------------
# P2 — float-return pair.
# ---------------------------------------------------------------------------
# Retail: li r3,0; blr (f1 unchanged from entry)
_RET_F1_SAME = "38600000 4E800020"
# Candidate: li r3,0; fsubs f1,f1,f1; blr  (f1 = 0)
_RET_F1_ZEROED = "38600000 EC210828 4E800020"
# Candidate with r4 clobber: li r4,1; li r3,0; blr
_RET_R4_CLOBBER = "38800001 38600000 4E800020"

# ---------------------------------------------------------------------------
# N1 — memory divergence under declared void.
# ---------------------------------------------------------------------------
_N1_RETAIL = "3880002A 9081FFF8 38600000 4E800020"   # li r4,42; stw r4,-8(r1); li r3,0; blr
_N1_CANDIDATE = "38A00063 90A1FFF8 38600000 4E800020"  # li r5,99; stw r5,-8(r1); li r3,0; blr

# ---------------------------------------------------------------------------
# N2 — r3 divergence under declared void.
# ---------------------------------------------------------------------------
_N2_RETAIL = "3860002A 4E800020"      # li r3,42; blr
_N2_CANDIDATE = "38600063 4E800020"   # li r3,99; blr

# ---------------------------------------------------------------------------
# N3 — indirect-branch thunk (borrowed from test_adv_r4_thunk_leak).
# ---------------------------------------------------------------------------
_N3_CORRECT = "81830000 818C0020 7D8903A6 4E800420"      # lwz r12,…; mtctr r12; bctr
_N3_R4_CLOBBER = "38800000 81830000 818C0020 7D8903A6 4E800420"  # li r4,0; …

# ---------------------------------------------------------------------------
# N4 — nonvolatile (r31) divergence under declared void.
# ---------------------------------------------------------------------------
_N4_RETAIL = "3BE0002A 4E800020"      # li r31,42; blr
_N4_CANDIDATE = "3BE00063 4E800020"   # li r31,99; blr


def _decode(hex_words: str):
    return decode_block(parse_hex(hex_words), validate_with_capstone=False)


def _prove(
    original_hex: str,
    candidate_hex: str,
    *,
    abi_shape: AbiShape | None = None,
    timeout_ms: int = 15_000,
):
    """Run a proof with optional ABI shape attached to an auto contract."""
    original = _decode(original_hex)
    candidate = _decode(candidate_hex)
    contract = make_contract(
        preset="auto",
        observe=None,
        timeout_ms=timeout_ms,
        original_live_out=automatic_live_out(original),
        candidate_live_out=automatic_live_out(candidate),
        abi_shape=abi_shape,
    )
    return check_equivalence(
        original,
        candidate,
        contract,
        original_hex=original_hex.replace(" ", ""),
        candidate_hex=candidate_hex.replace(" ", ""),
    )


def _prove_with_declared_return(
    original_hex: str,
    candidate_hex: str,
    declared: str | None,
    *,
    timeout_ms: int = 15_000,
):
    """Run a proof after combining inferred shape with a declared return."""
    original = _decode(original_hex)
    candidate = _decode(candidate_hex)
    inferred = infer_abi_shape(original, candidate, enabled=True)
    declared_shape = abi_shape_from_declared_return(declared)
    combined = combine_abi_shapes(inferred, declared_shape)
    contract = make_contract(
        preset="auto",
        observe=None,
        timeout_ms=timeout_ms,
        original_live_out=automatic_live_out(original),
        candidate_live_out=automatic_live_out(candidate),
        abi_shape=combined,
    )
    return check_equivalence(
        original,
        candidate,
        contract,
        original_hex=original_hex.replace(" ", ""),
        candidate_hex=candidate_hex.replace(" ", ""),
    )


# ===================================================================
# P1 — prep_buffer-shaped reg-swapped pair
# ===================================================================
class PrepBufferDeclaredReturnTests(unittest.TestCase):
    """P1: Chaitin-rotated pair with declared return narrowing."""

    def test_auto_no_declared_not_equivalent_r4(self) -> None:
        """Auto contract without narrowing → NOT_EQUIVALENT on r4."""
        result = _prove(_PREP_RETAIL, _PREP_CANDIDATE)
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        self.assertEqual((result.mismatch or {}).get("name"), "r4")

    def test_auto_declared_void_equivalent(self) -> None:
        """Auto + declared void → r4 dropped → EQUIVALENT."""
        result = _prove_with_declared_return(
            _PREP_RETAIL, _PREP_CANDIDATE, "void",
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT, result.mismatch)

    def test_auto_declared_i64_not_equivalent(self) -> None:
        """Auto + declared i64 → r4 kept → NOT_EQUIVALENT."""
        result = _prove_with_declared_return(
            _PREP_RETAIL, _PREP_CANDIDATE, "i64",
        )
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        # Under i64 return, r4 is a live return half — mismatch should be r4.
        self.assertEqual((result.mismatch or {}).get("name"), "r4")


# ===================================================================
# P2 — float-return pair
# ===================================================================
class FloatReturnDeclaredReturnTests(unittest.TestCase):
    """P2: declared f32 keeps f1 but may drop r4."""

    def test_f1_divergence_declared_f32_not_equivalent(self) -> None:
        """f1 differs at exit with declared f32 → NOT_EQUIVALENT (f1 kept)."""
        result = _prove_with_declared_return(
            _RET_F1_SAME, _RET_F1_ZEROED, "f32",
        )
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        # The mismatch should be on a floating-point lane.
        name = (result.mismatch or {}).get("name", "")
        self.assertTrue(
            name in ("f1", "f1.ps1"),
            f"expected f1 or f1.ps1 mismatch, got {name!r}",
        )

    def test_r4_divergence_declared_f32_equivalent(self) -> None:
        """r4 differs but f1 same with declared f32 → EQUIVALENT (r4 dropped)."""
        result = _prove_with_declared_return(
            _RET_F1_SAME, _RET_R4_CLOBBER, "f32",
        )
        self.assertEqual(result.status, ProofStatus.EQUIVALENT, result.mismatch)

    def test_f1ps1_divergence_variant(self) -> None:
        """f1.ps1-only divergence: skip if ps1 instructions unsupported."""
        # The engine models f1.ps1 as an observable (parse_observables handles
        # .ps1 suffixes), but paired-single instructions that affect ps1
        # independently are not available in the supported opcode set.
        # Check by attempting to decode a ps_mr; if UnsupportedInstruction is
        # raised, skip with a note.
        try:
            decode_block(
                parse_hex("10000030"),  # ps_mr f0,f0 — should raise if unsupported
                validate_with_capstone=False,
            )
        except Exception as exc:
            self.skipTest(
                f"paired-single instructions not supported by decoder ({exc})"
            )
        # If we get here, ps instructions are decodable — add a real test.
        # Build a pair where f1=entry in both but f1.ps1 differs.
        # ps_mr f1,f2 would copy f2's entire 64b into f1, setting both .ps0 and .ps1
        # to match f2's lanes.  For a f1.ps1-only divergence we need a PS op that
        # writes only the high (ps1) lane of f1 — that requires ps_merge or
        # ps_sel with specific operands.  Leave as a placeholder for now.
        self.skipTest("f1.ps1-only divergence needs PS instruction support")


# ===================================================================
# N1 — memory divergence
# ===================================================================
class MemoryDivergenceDeclaredVoidTests(unittest.TestCase):
    """N1: memory divergence persists under declared void."""

    def test_memory_differs_still_not_equivalent(self) -> None:
        """Declared void → memory still compared → NOT_EQUIVALENT."""
        result = _prove_with_declared_return(
            _N1_RETAIL, _N1_CANDIDATE, "void",
        )
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)


# ===================================================================
# N2 — r3 divergence
# ===================================================================
class R3DivergenceDeclaredVoidTests(unittest.TestCase):
    """N2: r3 divergence persists under declared void."""

    def test_r3_differs_still_not_equivalent(self) -> None:
        """Declared void → r3 always kept → NOT_EQUIVALENT."""
        result = _prove_with_declared_return(
            _N2_RETAIL, _N2_CANDIDATE, "void",
        )
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        self.assertEqual((result.mismatch or {}).get("name"), "r3")


# ===================================================================
# N3 — indirect-branch thunk
# ===================================================================
class IndirectBranchDeclaredVoidTests(unittest.TestCase):
    """N3: indirect-branch exits are never narrowed, even with declared void."""

    def test_indirect_exit_with_r4_clobber_not_equivalent(self) -> None:
        """R4 clobber before bctr → NOT_EQUIVALENT (indirect exits not narrowed)."""
        result = _prove_with_declared_return(
            _N3_CORRECT, _N3_R4_CLOBBER, "void",
        )
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        # The mismatch for an indirect-branch exit should be r4.
        self.assertEqual((result.mismatch or {}).get("name"), "r4")


# ===================================================================
# N4 — nonvolatile (r31) divergence
# ===================================================================
class NonvolatileDeclaredVoidTests(unittest.TestCase):
    """N4: nonvolatile divergence persists under declared void."""

    def test_r31_differs_still_not_equivalent(self) -> None:
        """Declared void → nonvolatile r31 still compared → NOT_EQUIVALENT."""
        result = _prove_with_declared_return(
            _N4_RETAIL, _N4_CANDIDATE, "void",
        )
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        self.assertEqual((result.mismatch or {}).get("name"), "r31")


if __name__ == "__main__":
    unittest.main()
