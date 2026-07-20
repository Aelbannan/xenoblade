"""Broadway FP rounding-mode and precondition-closure foundations (Wave 3).

Tier A for ``fp-scalar-arithmetic`` requires either:

* all four Broadway RN modes modeled; **or**
* a ``precondition-closure-v1`` attestation whose violation query is UNSAT.

The configured default (nearest-even only) is **not** proof that other modes
are unreachable. NI=0/1 modeling hooks live here for status reporting;
opcodes outside ``NI_SUPPORTED_OPS`` remain non-promotable when NI may be set.
"""

from __future__ import annotations

from enum import Enum
from typing import Any, Iterable, Mapping

from tools.ppc_equivalence.capability_assurance import (
    STATUS_INCOMPLETE,
    STATUS_PROMOTION_GRADE,
    STATUS_SCOPED_ASSUMPTION,
)

PRECONDITION_CLOSURE_CAPABILITY = "precondition-closure"
PRECONDITION_CLOSURE_ALGORITHM = "precondition-closure-v1"
PRECONDITION_CLOSURE_MODEL_VERSION = "precondition-closure-v1"

# Broadway FPSCR[0:1] encodings.
class RoundingMode(str, Enum):
    """Broadway floating-point rounding modes (FPSCR RN field)."""

    NEAREST_EVEN = "nearest-even"
    TOWARD_ZERO = "toward-zero"
    TOWARD_PLUS_INFINITY = "toward-plus-infinity"
    TOWARD_MINUS_INFINITY = "toward-minus-infinity"


# Canonical Tier-A RN set. Names match FloatingPointDomain / result.py.
BROADWAY_RN_MODES: tuple[str, ...] = (
    RoundingMode.NEAREST_EVEN.value,
    RoundingMode.TOWARD_ZERO.value,
    RoundingMode.TOWARD_PLUS_INFINITY.value,
    RoundingMode.TOWARD_MINUS_INFINITY.value,
)

# Currently modeled in SoftFloat / ConcreteOps (Wave 3 foundations).
RN_MODES_MODELED: frozenset[str] = frozenset({RoundingMode.NEAREST_EVEN.value})
RN_MODES_UNMODELED: frozenset[str] = frozenset(BROADWAY_RN_MODES) - RN_MODES_MODELED

# Obligation / attestation mode labels (also accepted in fp-scalar obligations).
OBLIGATION_RN_ALIASES: dict[str, str] = {
    "nearest-even": RoundingMode.NEAREST_EVEN.value,
    "rne": RoundingMode.NEAREST_EVEN.value,
    "zero": RoundingMode.TOWARD_ZERO.value,
    "toward-zero": RoundingMode.TOWARD_ZERO.value,
    "rtz": RoundingMode.TOWARD_ZERO.value,
    "+inf": RoundingMode.TOWARD_PLUS_INFINITY.value,
    "toward-plus-infinity": RoundingMode.TOWARD_PLUS_INFINITY.value,
    "-inf": RoundingMode.TOWARD_MINUS_INFINITY.value,
    "toward-minus-infinity": RoundingMode.TOWARD_MINUS_INFINITY.value,
}


def normalize_rn_mode(name: str) -> str | None:
    """Map an obligation / domain RN label to the canonical mode name."""
    key = str(name).strip().lower()
    return OBLIGATION_RN_ALIASES.get(key)


def all_broadway_rn_modes_modeled(modeled: Iterable[str] | None = None) -> bool:
    """True when the modeled set covers all four Broadway RN modes."""
    active = frozenset(
        str(m) for m in (modeled if modeled is not None else RN_MODES_MODELED)
    )
    return frozenset(BROADWAY_RN_MODES) <= active


def tier_a_rounding_satisfied(
    *,
    modeled_rn: Iterable[str] | None = None,
    precondition_closure_status: str | None = None,
) -> bool:
    """Tier A RN gate: all four modes modeled, or precondition-closure UNSAT.

    Default config alone (nearest-even) is never sufficient.
    """
    if all_broadway_rn_modes_modeled(modeled_rn):
        return True
    return precondition_closure_status == STATUS_PROMOTION_GRADE


def evaluate_precondition_closure_status(
    evidence: Mapping[str, Any] | None,
    *,
    assumptions: tuple[str, ...] | list[str] = (),
    unsupported: tuple[str, ...] | list[str] = (),
) -> str:
    """Grade ``precondition-closure-v1``; promotion-grade only with UNSAT.

    Expected evidence keys (foundations stub):

    * ``violation_query_result``: ``\"unsat\"`` required for promotion-grade
    * ``violation_query_sha256``: 64-hex digest binding the query
    * ``closed_preconditions``: list of precondition labels (optional)
    """
    if unsupported:
        return STATUS_INCOMPLETE
    if evidence is None or not isinstance(evidence, Mapping):
        return STATUS_INCOMPLETE
    result = evidence.get("violation_query_result")
    digest = evidence.get("violation_query_sha256")
    if result != "unsat":
        # Missing / sat / unknown / incomplete → never promotion-grade.
        return STATUS_INCOMPLETE
    if not isinstance(digest, str) or len(digest) != 64:
        return STATUS_INCOMPLETE
    if not all(ch in "0123456789abcdef" for ch in digest):
        return STATUS_INCOMPLETE
    if assumptions:
        return STATUS_SCOPED_ASSUMPTION
    return STATUS_PROMOTION_GRADE


def ni_modeling_status(*, opcode: str, ni_may_be_set: bool) -> str:
    """Report NI modeling completeness for an opcode.

    When NI may be set and the opcode is outside ``NI_SUPPORTED_OPS``, the
    capability is incomplete / non-promotable.
    """
    from tools.ppc_equivalence.fp_outcome import NI_SUPPORTED_OPS

    if not ni_may_be_set:
        return STATUS_PROMOTION_GRADE  # NI fixed to 0 — no NI modeling needed
    if str(opcode) in NI_SUPPORTED_OPS:
        # SoftFloat NI flush is modeled, but sticky / trap coupling incomplete.
        return STATUS_INCOMPLETE
    return STATUS_INCOMPLETE


def fpscr_sticky_modeling_status() -> dict[str, bool]:
    """Declare which FPSCR sticky / summary bits SoftFloat currently latches.

    SoftFloat oracle paths set IEEE sticky indicators (OX/UX/ZX/XX/VX via
    ``FpOracleFlags`` / ``invalid_cause``). FX/FEX sticky re-raise and full
    VX subcause latching remain incomplete — do not claim promotion-grade.
    """
    return {
        "ox": True,  # SoftFloat overflow sticky latched into FPOutcome.flags
        "ux": True,
        "zx": True,
        "xx": True,
        "vx": True,  # coarse invalid; subcauses via invalid_cause when set
        "vx_subcauses_complete": False,
        "fx": False,
        "fex": False,
        "fi": True,  # FI mirrored from SoftFloat inexact where applied
        "fr": False,  # FR not fully modeled for Tier A
        "fprf": True,
        "enable_ve_ze_oe_ue_xe": False,  # trap enables: see fp_traps (incomplete)
        "sticky_preservation_complete": False,
    }
