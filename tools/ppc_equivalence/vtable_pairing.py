"""Logical slot pairing for MWCC virtual-call indirect-target closure.

Pairs vtable slot targets by **logical callee identity** (symbol or stable
``slot-<offset>`` label), not by absolute linked address equality.
"""

from __future__ import annotations

from dataclasses import dataclass
from typing import Any, Literal, Sequence

from tools.ppc_equivalence.jump_table_obligations import build_indirect_targets_obligation


class VirtualCallPairingError(ValueError):
    """Fail-closed rejection when virtual-call targets cannot be paired."""


@dataclass(frozen=True)
class VirtualCalleeCase:
    identity: str
    slot_offset: int
    original_pc: int
    candidate_pc: int
    original_symbol: str | None = None
    candidate_symbol: str | None = None


@dataclass(frozen=True)
class VirtualCallPairing:
    cases: tuple[VirtualCalleeCase, ...]
    slot_offset: int


def logical_callee_identity(
    *,
    slot_offset: int,
    symbol: str | None = None,
) -> str:
    """Stable logical identity for one vtable slot target."""
    if symbol:
        return symbol
    return f"slot-{slot_offset:#x}"


def pair_virtual_call_targets(
    *,
    slot_offset: int,
    original_pc: int,
    candidate_pc: int,
    original_symbol: str | None = None,
    candidate_symbol: str | None = None,
) -> VirtualCallPairing:
    """Pair one virtual-call slot by logical callee identity.

    When both sides supply symbols they must agree; otherwise pairing fails
    closed with ``VirtualCallPairingError``.
    """
    if original_symbol and candidate_symbol and original_symbol != candidate_symbol:
        raise VirtualCallPairingError(
            f"logical callee identity mismatch: original={original_symbol!r} "
            f"candidate={candidate_symbol!r}"
        )
    identity = logical_callee_identity(
        slot_offset=slot_offset,
        symbol=original_symbol or candidate_symbol,
    )
    case = VirtualCalleeCase(
        identity=identity,
        slot_offset=slot_offset,
        original_pc=original_pc & 0xFFFFFFFC,
        candidate_pc=candidate_pc & 0xFFFFFFFC,
        original_symbol=original_symbol,
        candidate_symbol=candidate_symbol,
    )
    return VirtualCallPairing(cases=(case,), slot_offset=slot_offset)


def pair_virtual_call_cases(
    *,
    slot_offset: int,
    original_targets: Sequence[tuple[str, int, str | None]],
    candidate_targets: Sequence[tuple[str, int, str | None]],
) -> VirtualCallPairing:
    """Pair multiple slot targets sharing one logical identity key."""
    original_by_id = {
        item[0]: (item[1], item[2]) for item in original_targets
    }
    candidate_by_id = {
        item[0]: (item[1], item[2]) for item in candidate_targets
    }
    if set(original_by_id) != set(candidate_by_id):
        raise VirtualCallPairingError(
            "virtual-call logical identity set mismatch: "
            f"original={sorted(original_by_id)} "
            f"candidate={sorted(candidate_by_id)}"
        )
    cases = tuple(
        VirtualCalleeCase(
            identity=identity,
            slot_offset=slot_offset,
            original_pc=original_by_id[identity][0] & 0xFFFFFFFC,
            candidate_pc=candidate_by_id[identity][0] & 0xFFFFFFFC,
            original_symbol=original_by_id[identity][1],
            candidate_symbol=candidate_by_id[identity][1],
        )
        for identity in sorted(original_by_id)
    )
    return VirtualCallPairing(cases=cases, slot_offset=slot_offset)


def indirect_targets_obligation_for_side(
    pairing: VirtualCallPairing,
    *,
    branch_pc: int,
    side: Literal["original", "candidate"],
    source: str,
    artifact_hashes: Sequence[str],
    coverage: str | dict[str, Any] = "pending",
) -> dict[str, Any]:
    """Build ``indirect-target-closure`` payload for one side of a pairing."""
    if side == "original":
        targets = tuple((case.identity, case.original_pc) for case in pairing.cases)
    elif side == "candidate":
        targets = tuple((case.identity, case.candidate_pc) for case in pairing.cases)
    else:
        raise ValueError(f"unsupported side {side!r}")
    return build_indirect_targets_obligation(
        branch_pc=branch_pc,
        targets=targets,
        source=source,
        artifact_hashes=artifact_hashes,
        coverage=coverage,
        algorithm="virtual-call-enumerated-v1",
    )


def indirect_targets_obligations_for_pairing(
    pairing: VirtualCallPairing,
    *,
    branch_pc_original: int,
    branch_pc_candidate: int,
    source_original: str,
    source_candidate: str,
    artifact_hashes: Sequence[str],
    coverage: str | dict[str, Any] = "pending",
) -> tuple[dict[str, Any], dict[str, Any]]:
    """Build original and candidate indirect-target obligations from one pairing."""
    return (
        indirect_targets_obligation_for_side(
            pairing,
            branch_pc=branch_pc_original,
            side="original",
            source=source_original,
            artifact_hashes=artifact_hashes,
            coverage=coverage,
        ),
        indirect_targets_obligation_for_side(
            pairing,
            branch_pc=branch_pc_candidate,
            side="candidate",
            source=source_candidate,
            artifact_hashes=artifact_hashes,
            coverage=coverage,
        ),
    )
