"""Regression: FPSCR sticky incompleteness must not authorize EQUIVALENT.

SymbolicOps never latches OX/UX/XX for scalar FP arithmetic. Two blocks that
differ only in hardware FPSCR.XX after an inexact ``fdivs`` must not be
reported EQUIVALENT when ``fpscr`` is a compared observable — the engine fails
closed with ``INCONCLUSIVE_ABSTRACTION``.
"""

from __future__ import annotations

import unittest

from tools.ppc_equivalence.contract import make_contract
from tools.ppc_equivalence.decoder import decode_block, parse_hex
from tools.ppc_equivalence.engine import check_equivalence
from tools.ppc_equivalence.fp_fpscr import (
    FPSCR_STICKIES_UNSUPPORTED,
    FPSCR_SYMBOLIC_STICKIES_ASSUMPTION,
    symbolic_fpscr_stickies_incomplete,
)
from tools.ppc_equivalence.fp_oracle import fdivs_fpr_rne, fmuls_fpr_rne
from tools.ppc_equivalence.result import ProofStatus
from tools.ppc_equivalence.semantics import automatic_live_out


def _w(v: int) -> str:
    return f"{v:08X}"


def _fdivs(fd: int, fa: int, fb: int) -> str:
    return _w((59 << 26) | (fd << 21) | (fa << 16) | (fb << 11) | (18 << 1))


def _fmuls(fd: int, fa: int, fc: int) -> str:
    return _w((59 << 26) | (fd << 21) | (fa << 16) | (0 << 11) | (fc << 6) | (25 << 1))


def _fres(fd: int, fb: int) -> str:
    return _w((59 << 26) | (fd << 21) | (fb << 11) | (24 << 1))


# mtfsb0/mtfsb1 take an MSB-numbered bit index; FPSCR_XX == 1<<25 -> MSB bit 6.
_MTFSB0_XX = _w((63 << 26) | (6 << 21) | (70 << 1))
_MTFSB1_XX = _w((63 << 26) | (6 << 21) | (38 << 1))


def _decode(hex_words: str):
    return decode_block(parse_hex(hex_words), validate_with_capstone=False)


def _prove(original_hex: str, candidate_hex: str):
    original = _decode(original_hex)
    candidate = _decode(candidate_hex)
    contract = make_contract(
        preset="auto",
        observe=None,
        timeout_ms=20_000,
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


class FpscrXxFalseEquivalentTests(unittest.TestCase):
    def test_symbolic_stickies_declared_incomplete(self) -> None:
        self.assertTrue(symbolic_fpscr_stickies_incomplete())

    def test_hardware_ground_truth_inexact_sets_xx(self) -> None:
        one, three = 0x3FF0000000000000, 0x4008000000000000
        v11 = 0x3FF199999999999A  # 1.1
        self.assertTrue(fdivs_fpr_rne(one, three).flags.inexact)
        self.assertTrue(fmuls_fpr_rne(v11, v11).flags.inexact)

    def test_fdivs_xx_clear_is_inconclusive(self) -> None:
        """Re-clearing XX after inexact divide must not authorize EQUIVALENT."""
        original = _MTFSB0_XX + " " + _fdivs(1, 2, 3)
        candidate = _MTFSB0_XX + " " + _fdivs(1, 2, 3) + " " + _MTFSB0_XX
        result = _prove(original, candidate)

        self.assertIn("fpscr", result.observables or [])
        self.assertEqual(result.status, ProofStatus.INCONCLUSIVE_ABSTRACTION)
        self.assertIn(
            FPSCR_SYMBOLIC_STICKIES_ASSUMPTION, result.assumptions or [],
        )
        self.assertIn(FPSCR_STICKIES_UNSUPPORTED, result.unsupported or [])

    def test_fmuls_xx_clear_is_inconclusive(self) -> None:
        original = _MTFSB0_XX + " " + _fmuls(1, 2, 3)
        candidate = _MTFSB0_XX + " " + _fmuls(1, 2, 3) + " " + _MTFSB0_XX
        result = _prove(original, candidate)
        self.assertEqual(result.status, ProofStatus.INCONCLUSIVE_ABSTRACTION)

    def test_control_explicit_xx_set_is_caught(self) -> None:
        """Explicit mtfsb1 XX is modeled and yields NOT_EQUIVALENT."""
        original = _MTFSB0_XX + " " + _fdivs(1, 2, 3)
        candidate = _MTFSB0_XX + " " + _fdivs(1, 2, 3) + " " + _MTFSB1_XX
        result = _prove(original, candidate)
        # Sticky demotion runs only on EQUIVALENT; NE is reported first.
        self.assertEqual(result.status, ProofStatus.NOT_EQUIVALENT)
        self.assertEqual((result.mismatch or {}).get("kind"), "fpscr")

    def test_third_angle_fres_vs_fdivs_not_equivalent(self) -> None:
        result = _prove(_fres(1, 2), _fdivs(1, 3, 2))
        self.assertNotEqual(result.status, ProofStatus.EQUIVALENT)


if __name__ == "__main__":
    unittest.main()
