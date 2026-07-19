"""Logical case-index pairing for MWCC jump-table indirect-target closure.

Pairs table entries by **index** (``case-0``, ``case-1``, …), not by absolute
target address equality. Two implementations may relocate case handlers while
still sharing the same dispatch shape.
"""

from __future__ import annotations

from dataclasses import dataclass
from typing import Any, Literal, Sequence

from tools.ppc_equivalence.jump_table_obligations import build_indirect_targets_obligation


class JumpTablePairingError(ValueError):
    """Fail-closed rejection when tables cannot be paired logically."""


@dataclass(frozen=True)
class JumpTableCase:
    identity: str
    index: int
    original_pc: int
    candidate_pc: int


@dataclass(frozen=True)
class JumpTablePairing:
    cases: tuple[JumpTableCase, ...]
    table_base_original: int
    table_base_candidate: int
    entry_count: int


def pair_jump_table_cases(
    *,
    original_words: Sequence[int],
    candidate_words: Sequence[int],
    table_base_original: int = 0,
    table_base_candidate: int = 0,
) -> JumpTablePairing:
    """Pair jump-table entries by table index.

    Requires equal entry counts; raises ``JumpTablePairingError`` on mismatch.
    """
    original = tuple(int(word) & 0xFFFFFFFF for word in original_words)
    candidate = tuple(int(word) & 0xFFFFFFFF for word in candidate_words)
    if len(original) != len(candidate):
        raise JumpTablePairingError(
            f"jump-table entry count mismatch: original={len(original)} "
            f"candidate={len(candidate)}"
        )
    if not original:
        raise JumpTablePairingError("jump-table words must be nonempty")

    cases = tuple(
        JumpTableCase(
            identity=f"case-{index}",
            index=index,
            original_pc=original[index],
            candidate_pc=candidate[index],
        )
        for index in range(len(original))
    )
    return JumpTablePairing(
        cases=cases,
        table_base_original=table_base_original,
        table_base_candidate=table_base_candidate,
        entry_count=len(original),
    )


def indirect_targets_obligation_for_side(
    pairing: JumpTablePairing,
    *,
    branch_pc: int,
    side: Literal["original", "candidate"],
    source: str,
    artifact_hashes: Sequence[str],
    coverage: str = "pending",
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
    )


def indirect_targets_obligations_for_pairing(
    pairing: JumpTablePairing,
    *,
    branch_pc_original: int,
    branch_pc_candidate: int,
    source_original: str,
    source_candidate: str,
    artifact_hashes: Sequence[str],
    coverage: str = "pending",
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
