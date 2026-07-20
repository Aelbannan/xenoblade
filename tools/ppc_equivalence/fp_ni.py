"""FPSCR.NI modeling foundations for SCALAR_FP_V2 Phase 5.

NI is read from the live FPSCR.NI bit (``FPSCR[2]``). Production paths keep
Wave-4 ``NI_SUPPORTED_OPS`` (sixteen scalar + paired oracle ops) until
``SCALAR_FP_EXACT_V2=1``, which expands coverage via ``NI_EXACT_V2_OPS`` and
enables opcode stubs such as ``frsp_exact_v2_stub``.
"""

from __future__ import annotations

from dataclasses import dataclass
from enum import Enum
from typing import Final

from .fp_bits import mask64
from .fp_capabilities import scalar_fp_exact_v2_enabled
from .fp_fpscr import FPSCR_NI
from .fp_oracle import (
    NI_SCALAR_SUPPORTED_OPS,
    OracleUnimplementedError,
    _round_binary64_to_binary32_rne,
    _single_to_fpr_bits,
    ni_flush_operand_binary64,
    ni_flush_result_binary64,
    ni_force_single_result_bits,
)
from .fp_outcome import NI_SUPPORTED_OPS, PAIRED_ORACLE_OPS
from .fp_rounding import RoundingMode

# Smallest positive binary32 normal as a binary64 magnitude (Broadway ForceSingle).
_SMALLEST_NORMAL_SINGLE_MAG: Final[int] = 0x3810000000000000

# Phase 5: NI-sensitive scalar opcodes beyond Wave-4 ``NI_SUPPORTED_OPS``.
NI_EXACT_V2_EXTRA_OPS: frozenset[str] = frozenset({
    "frsp",
    "fres",
    "frsqrte",
    "fcmpu",
    "fcmpo",
    "stfs",
    "stfsu",
    "stfsx",
    "stfsux",
    "fctiw",
    "fctiwz",
})

# Documented NI-sensitive families landing in Phase 6+ (contract rows present).
NI_EXACT_V2_PLANNED_OPS: frozenset[str] = frozenset({
    "lfs",
    "lfsu",
    "lfsx",
    "lfsux",
    "stfd",
    "stfdu",
    "stfdx",
    "stfdux",
    "stfiwx",
    "ps_res",
    "ps_rsqrte",
    "ps_muls0",
    "ps_muls1",
    "ps_div",
    "ps_sum0",
    "ps_sum1",
})

NI_EXACT_V2_OPS: frozenset[str] = NI_SUPPORTED_OPS | NI_EXACT_V2_EXTRA_OPS


class NiFlushKind(str, Enum):
    """Which NI flush stages apply to an opcode (contract v2 summary)."""

    NONE = "none"
    OPERAND = "operand"
    RESULT = "result"
    OPERAND_AND_RESULT = "operand-and-result"
    FORCE_SINGLE_RESULT = "force-single-result"
    STORE_OPERAND = "store-operand"


@dataclass(frozen=True, slots=True)
class NiOpcodePolicy:
    """Per-opcode NI operand/result flush policy."""

    operand: NiFlushKind = NiFlushKind.NONE
    result: NiFlushKind = NiFlushKind.NONE


# Contract-derived NI policies for exact-v2 opcodes (Phase 5 ledger).
NI_EXACT_V2_POLICIES: dict[str, NiOpcodePolicy] = {
    "frsp": NiOpcodePolicy(result=NiFlushKind.FORCE_SINGLE_RESULT),
    "fres": NiOpcodePolicy(
        operand=NiFlushKind.OPERAND,
        result=NiFlushKind.FORCE_SINGLE_RESULT,
    ),
    "frsqrte": NiOpcodePolicy(
        operand=NiFlushKind.OPERAND,
        result=NiFlushKind.RESULT,
    ),
    "fcmpu": NiOpcodePolicy(),
    "fcmpo": NiOpcodePolicy(),
    "stfs": NiOpcodePolicy(operand=NiFlushKind.STORE_OPERAND),
    "stfsu": NiOpcodePolicy(operand=NiFlushKind.STORE_OPERAND),
    "stfsx": NiOpcodePolicy(operand=NiFlushKind.STORE_OPERAND),
    "stfsux": NiOpcodePolicy(operand=NiFlushKind.STORE_OPERAND),
    "fctiw": NiOpcodePolicy(),
    "fctiwz": NiOpcodePolicy(),
}


def fpscr_ni_enabled(fpscr: int) -> bool:
    """Return True when FPSCR.NI (bit 2) is set."""
    return bool(mask64(int(fpscr)) & FPSCR_NI)


def fpscr_rn_mode(fpscr: int) -> RoundingMode:
    """Map FPSCR[0:1] to the Broadway rounding-mode enum."""
    rn = int(fpscr) & 3
    return (
        RoundingMode.NEAREST_EVEN,
        RoundingMode.TOWARD_ZERO,
        RoundingMode.TOWARD_PLUS_INFINITY,
        RoundingMode.TOWARD_MINUS_INFINITY,
    )[rn]


def effective_ni_supported_ops() -> frozenset[str]:
    """Opcode names with NI modeling for the active configuration."""
    if scalar_fp_exact_v2_enabled():
        return NI_EXACT_V2_OPS
    return NI_SUPPORTED_OPS


def ni_policy_for_opcode(opcode: str) -> NiOpcodePolicy:
    """Return the Phase-5 NI policy for ``opcode`` (exact-v2 extras only)."""
    if opcode in NI_SCALAR_SUPPORTED_OPS or opcode in PAIRED_ORACLE_OPS:
        return NiOpcodePolicy(
            operand=NiFlushKind.OPERAND,
            result=NiFlushKind.FORCE_SINGLE_RESULT
            if opcode.endswith("s") or opcode.startswith("ps_")
            else NiFlushKind.RESULT,
        )
    return NI_EXACT_V2_POLICIES.get(str(opcode), NiOpcodePolicy())


def ni_operand_flush_from_fpscr(bits: int, fpscr: int) -> int:
    """Apply Broadway Table 2-24 operand flush using live FPSCR.NI."""
    return ni_flush_operand_binary64(bits, ni=fpscr_ni_enabled(fpscr))


def ni_result_flush_from_fpscr(
    bits: int,
    fpscr: int,
    *,
    is_single: bool = False,
) -> int:
    """Apply Broadway result flush using live FPSCR.NI."""
    ni = fpscr_ni_enabled(fpscr)
    if is_single:
        return ni_force_single_result_bits(bits, ni=ni)
    return ni_flush_result_binary64(bits, ni=ni)


def ni_apply_policy_operand(bits: int, fpscr: int, opcode: str) -> int:
    """Flush operands per opcode policy before evaluation."""
    policy = ni_policy_for_opcode(opcode)
    if policy.operand is NiFlushKind.NONE:
        return mask64(bits)
    if policy.operand is NiFlushKind.STORE_OPERAND:
        return ni_operand_flush_from_fpscr(bits, fpscr)
    if policy.operand in (NiFlushKind.OPERAND, NiFlushKind.OPERAND_AND_RESULT):
        return ni_operand_flush_from_fpscr(bits, fpscr)
    return mask64(bits)


def ni_apply_policy_result(
    bits: int,
    fpscr: int,
    opcode: str,
    *,
    is_single: bool | None = None,
) -> int:
    """Flush results per opcode policy after evaluation."""
    policy = ni_policy_for_opcode(opcode)
    if policy.result is NiFlushKind.NONE:
        return mask64(bits)
    single = (
        bool(is_single)
        if is_single is not None
        else policy.result is NiFlushKind.FORCE_SINGLE_RESULT
        or opcode.endswith("s")
        or opcode in {"fres", "frsp"}
    )
    if policy.result is NiFlushKind.FORCE_SINGLE_RESULT:
        return ni_result_flush_from_fpscr(bits, fpscr, is_single=True)
    if policy.result in (NiFlushKind.RESULT, NiFlushKind.OPERAND_AND_RESULT):
        return ni_result_flush_from_fpscr(bits, fpscr, is_single=single)
    return mask64(bits)


def ni_modeling_status_for_opcode(opcode: str, *, ni_may_be_set: bool) -> str:
    """Report NI completeness for capability ledgers."""
    from tools.ppc_equivalence.capability_assurance import (
        STATUS_INCOMPLETE,
        STATUS_PROMOTION_GRADE,
    )

    if not ni_may_be_set:
        return STATUS_PROMOTION_GRADE
    if str(opcode) not in effective_ni_supported_ops():
        return STATUS_INCOMPLETE
    if scalar_fp_exact_v2_enabled() and str(opcode) in NI_EXACT_V2_EXTRA_OPS:
        # Declared in the exact-v2 set; full semantics still Phase 6+ for most.
        return STATUS_INCOMPLETE
    return STATUS_INCOMPLETE


def compare_ordering_after_ni_operand_flush(
    left: int,
    right: int,
    fpscr: int,
) -> tuple[int, int, str]:
    """Return flushed operands and a three-way ordering label.

    Used by Phase-5 tests to show NI operand flush can change compare ordering
    (e.g. opposite-sign subnormals both become signed zero and compare equal).
    """
    left_f = ni_operand_flush_from_fpscr(left, fpscr)
    right_f = ni_operand_flush_from_fpscr(right, fpscr)
    if left_f == right_f:
        ordering = "equal"
    elif left_f < right_f:
        ordering = "less"
    else:
        ordering = "greater"
    return left_f, right_f, ordering


def frsp_exact_v2_stub(operand_bits64: int, fpscr: int) -> int:
    """Phase-5 ``frsp`` stub: RN round double→single + NI result flush.

    Requires ``SCALAR_FP_EXACT_V2=1``. Operand NI flush is not applied (contract
    ``ni_operand: none``). Rounding uses RNE today (ConcreteOps domain still
    requires RN=nearest-even); RN selection reads live FPSCR for composition with
    the Phase-2 kernel once all four modes are enabled on the convert path.
    """
    if not scalar_fp_exact_v2_enabled():
        raise OracleUnimplementedError(
            "frsp_exact_v2_stub requires SCALAR_FP_EXACT_V2=1",
        )
    bits64 = mask64(operand_bits64)
    _ = fpscr_rn_mode(fpscr)  # reserved for multi-RN convert integration
    bits32, _flags = _round_binary64_to_binary32_rne(bits64)
    expanded = _single_to_fpr_bits(bits32)
    return ni_apply_policy_result(expanded, fpscr, "frsp", is_single=True)


def ni_documentation() -> dict[str, object]:
    """Machine-readable NI coverage summary for status reporting."""
    return {
        "production_supported": sorted(NI_SUPPORTED_OPS),
        "exact_v2_extra": sorted(NI_EXACT_V2_EXTRA_OPS),
        "exact_v2_planned": sorted(NI_EXACT_V2_PLANNED_OPS),
        "effective_supported": sorted(effective_ni_supported_ops()),
        "live_fpscr_ni": True,
        "exact_v2_enabled": scalar_fp_exact_v2_enabled(),
    }


__all__ = [
    "NI_EXACT_V2_EXTRA_OPS",
    "NI_EXACT_V2_OPS",
    "NI_EXACT_V2_PLANNED_OPS",
    "NI_EXACT_V2_POLICIES",
    "NiFlushKind",
    "NiOpcodePolicy",
    "compare_ordering_after_ni_operand_flush",
    "effective_ni_supported_ops",
    "fpscr_ni_enabled",
    "fpscr_rn_mode",
    "frsp_exact_v2_stub",
    "ni_apply_policy_operand",
    "ni_apply_policy_result",
    "ni_documentation",
    "ni_modeling_status_for_opcode",
    "ni_operand_flush_from_fpscr",
    "ni_policy_for_opcode",
    "ni_result_flush_from_fpscr",
]
