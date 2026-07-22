"""Regression: indirect-branch must compare live outgoing r4 at tail calls.

At a ``this``-adjustment tail thunk, r4 is a live outgoing argument to the
virtual method. A correct thunk preserves it; a broken thunk that zeroes r4
before the identical ``bctr`` must be NOT_EQUIVALENT.
"""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.contract import make_contract
from tools.ppc_equivalence.decoder import decode_block, parse_hex
from tools.ppc_equivalence.engine import check_equivalence
from tools.ppc_equivalence.result import ProofStatus
from tools.ppc_equivalence.semantics import automatic_live_out


def _decode(hex_words: str):
    return decode_block(parse_hex(hex_words), validate_with_capstone=False)


def _prove(original_hex: str, candidate_hex: str):
    original = _decode(original_hex)
    candidate = _decode(candidate_hex)
    contract = make_contract(
        preset="auto",
        observe=None,
        timeout_ms=10_000,
        original_live_out=automatic_live_out(original),
        candidate_live_out=automatic_live_out(candidate),
    )
    return check_equivalence(
        original,
        candidate,
        contract,
        original_hex=original_hex.replace(" ", ""),
        candidate_hex=candidate_hex.replace(" ", ""),
    )


class R4ThunkLeakTests(unittest.TestCase):
    # Correct virtual thunk: preserves r4 (the callee's first real argument).
    #   lwz  r12, 0(r3)
    #   lwz  r12, 0x20(r12)
    #   mtctr r12
    #   bctr
    _CORRECT_THUNK = "81830000 818c0020 7d8903a6 4e800420"

    # Broken thunk: clobbers r4 to 0, then the SAME tail branch.
    #   li   r4, 0
    #   lwz  r12, 0(r3)
    #   lwz  r12, 0x20(r12)
    #   mtctr r12
    #   bctr
    _BROKEN_THUNK_R4 = "38800000 81830000 818c0020 7d8903a6 4e800420"

    # Control: also clobber r3 (the `this` pointer, which IS still compared).
    #   li   r3, 0
    _CLOBBER_R3 = "38600000 81830000 818c0020 7d8903a6 4e800420"

    def test_r4_clobber_is_not_equivalent(self) -> None:
        result = _prove(self._CORRECT_THUNK, self._BROKEN_THUNK_R4)
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        self.assertEqual((result.mismatch or {}).get("name"), "r4")
        self.assertIn("r4", result.observables or [])

    def test_r3_clobber_is_correctly_not_equivalent(self) -> None:
        result = _prove(self._CORRECT_THUNK, self._CLOBBER_R3)
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        self.assertEqual((result.mismatch or {}).get("name"), "exit.target")


if __name__ == "__main__":
    unittest.main()
