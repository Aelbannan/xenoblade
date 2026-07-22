"""Exact Broadway FP compare ops (Phase 6 — ``fcmpu`` / ``fcmpo``).

Bit-exact classification, FPCC/CR updates, VXSNAN/VXVC, and optional NI operand
flush. No host ``float``.
"""

from __future__ import annotations

from dataclasses import dataclass

from .fp_exact_outcome import FiFrPolicy, ScalarFPOutcome
from .fp_exact_util import (
    apply_ni_operand,
    compare_binary64_magnitude,
    exact_v2_enabled,
    fpcc_nibble,
    is_nan_binary64,
    is_snan_binary64,
    unsupported_exact_outcome,
)
from .fp_fpscr import FPSCR_VE, FPSCR_VXSNAN, FPSCR_VXVC


@dataclass(frozen=True, slots=True)
class CompareOutcome:
    """Compare side effects beyond ``ScalarFPOutcome``."""

    scalar: ScalarFPOutcome
    cr_field: int
    fpcc: int
    cr_bf: int = 0


def _compare_relation(left: int, right: int) -> tuple[int, int]:
    """Return ``(fpcc_nibble, raised_causes)`` for two binary64 operands."""
    left_nan = is_nan_binary64(left)
    right_nan = is_nan_binary64(right)
    if left_nan or right_nan:
        return fpcc_nibble(lt=False, gt=False, eq=False, unordered=True), 0

    relation = compare_binary64_magnitude(left, right)
    lt = relation < 0
    gt = relation > 0
    eq = relation == 0
    return fpcc_nibble(lt=lt, gt=gt, eq=eq, unordered=False), 0


def exact_fcmp(
    left_bits: int,
    right_bits: int,
    *,
    ordered: bool,
    fpscr: int = 0,
    cr_bf: int = 0,
    ni_flush_operands: bool = False,
) -> CompareOutcome:
    """Execute ``fcmpu`` (``ordered=False``) or ``fcmpo`` (``ordered=True``).

    Parameters
    ----------
    left_bits, right_bits:
        Original binary64 FPR operand bits (SNAN tests use unflushed values).
    fpscr:
        Pre-instruction FPSCR (RN/NI/VE enables).
    cr_bf:
        Target CR field index (0–7) for the compare nibble write.
    ni_flush_operands:
        When True and FPSCR.NI=1, flush denormal operands before relation.
        Broadway ``fcmpu`` / ``fcmpo`` do not apply NI operand flush (Phase 5 policy).
    """
    if not exact_v2_enabled():
        scalar = unsupported_exact_outcome("SCALAR_FP_EXACT_V2 disabled")
        return CompareOutcome(scalar=scalar, cr_field=0, fpcc=0, cr_bf=cr_bf)

    left = int(left_bits) & 0xFFFFFFFFFFFFFFFF
    right = int(right_bits) & 0xFFFFFFFFFFFFFFFF
    raised = 0

    left_snan = is_snan_binary64(left)
    right_snan = is_snan_binary64(right)
    if left_snan or right_snan:
        raised |= FPSCR_VXSNAN

    compare_left = apply_ni_operand(left, fpscr) if ni_flush_operands else left
    compare_right = apply_ni_operand(right, fpscr) if ni_flush_operands else right
    fpcc, _ = _compare_relation(compare_left, compare_right)

    left_nan = is_nan_binary64(left)
    right_nan = is_nan_binary64(right)
    is_nan = left_nan or right_nan
    if ordered and is_nan:
        ve_enabled = bool(int(fpscr) & FPSCR_VE)
        snan = left_snan or right_snan
        if not snan or not ve_enabled:
            raised |= FPSCR_VXVC

    scalar = ScalarFPOutcome(
        result_bits=0,
        raised_causes=raised,
        fi_fr_policy=FiFrPolicy.PRESERVE,
        fprf=fpcc,
        update_fprf=True,
        writeback=False,
        supported=True,
    )
    return CompareOutcome(
        scalar=scalar,
        cr_field=fpcc,
        fpcc=fpcc,
        cr_bf=int(cr_bf) & 7,
    )


def exact_fcmpu(
    left_bits: int,
    right_bits: int,
    *,
    fpscr: int = 0,
    cr_bf: int = 0,
    ni_flush_operands: bool = False,
) -> CompareOutcome:
    """Unordered compare ``fcmpu``."""
    return exact_fcmp(
        left_bits,
        right_bits,
        ordered=False,
        fpscr=fpscr,
        cr_bf=cr_bf,
        ni_flush_operands=ni_flush_operands,
    )


def exact_fcmpo(
    left_bits: int,
    right_bits: int,
    *,
    fpscr: int = 0,
    cr_bf: int = 0,
    ni_flush_operands: bool = False,
) -> CompareOutcome:
    """Ordered compare ``fcmpo``."""
    return exact_fcmp(
        left_bits,
        right_bits,
        ordered=True,
        fpscr=fpscr,
        cr_bf=cr_bf,
        ni_flush_operands=ni_flush_operands,
    )


__all__ = [
    "CompareOutcome",
    "exact_fcmp",
    "exact_fcmpo",
    "exact_fcmpu",
]
