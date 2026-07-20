"""Per-side readonly word evidence for memory-loop CTR ``lwz`` hydration.

Never merge sides with ``dict.update``: conflicting values at the same VA must
reject, and each side's evidence is bound into ``Select(initial_memory, …)``
premises independently.
"""

from __future__ import annotations

from collections.abc import Mapping, Sequence
from dataclasses import dataclass
from typing import Any


@dataclass(frozen=True)
class ReadonlyWordEvidence:
    """One big-endian u32 word proven from a linked image or explicit map."""

    address: int
    value: int
    source: str = "explicit"

    def __post_init__(self) -> None:
        object.__setattr__(self, "address", int(self.address) & 0xFFFFFFFF)
        object.__setattr__(self, "value", int(self.value) & 0xFFFFFFFF)

    def as_map_entry(self) -> tuple[int, int]:
        return self.address, self.value

    def image_bytes(self) -> bytes:
        return int(self.value).to_bytes(4, "big")


@dataclass(frozen=True)
class MemoryLoopReadonlyContext:
    """Side-isolated readonly evidence for original/candidate memory loops."""

    original: tuple[ReadonlyWordEvidence, ...] = ()
    candidate: tuple[ReadonlyWordEvidence, ...] = ()

    def words_for(self, side: str) -> dict[int, int]:
        if side == "original":
            evidence = self.original
        elif side == "candidate":
            evidence = self.candidate
        else:
            raise ValueError(f"unknown side {side!r}")
        return {item.address: item.value for item in evidence}

    def all_evidence(self) -> tuple[ReadonlyWordEvidence, ...]:
        return self.original + self.candidate

    def conflict_reason(self) -> str | None:
        """Return a reject reason when the same VA has disagreeing values."""
        seen: dict[int, int] = {}
        for item in self.all_evidence():
            prior = seen.get(item.address)
            if prior is None:
                seen[item.address] = item.value
                continue
            if prior != item.value:
                return (
                    f"readonly word conflict at 0x{item.address:08X}: "
                    f"0x{prior:08X} vs 0x{item.value:08X}"
                )
        return None


def evidence_from_words(
    words: Mapping[int, int] | None,
    *,
    source: str = "explicit",
) -> tuple[ReadonlyWordEvidence, ...]:
    if not words:
        return ()
    return tuple(
        ReadonlyWordEvidence(
            address=int(address),
            value=int(value),
            source=source,
        )
        for address, value in sorted(words.items())
    )


def build_memory_loop_readonly_context(
    *,
    original_words: Mapping[int, int] | None = None,
    candidate_words: Mapping[int, int] | None = None,
    shared_words: Mapping[int, int] | None = None,
    original_source: str = "explicit",
    candidate_source: str = "explicit",
) -> MemoryLoopReadonlyContext | None:
    """Build a dual-side context; ``shared_words`` clones onto both sides."""
    if shared_words is not None:
        shared = evidence_from_words(shared_words, source="explicit")
        original = (
            evidence_from_words(original_words, source=original_source)
            if original_words is not None
            else shared
        )
        candidate = (
            evidence_from_words(candidate_words, source=candidate_source)
            if candidate_words is not None
            else shared
        )
    else:
        original = evidence_from_words(original_words, source=original_source)
        candidate = evidence_from_words(candidate_words, source=candidate_source)
    if not original and not candidate:
        return None
    return MemoryLoopReadonlyContext(original=original, candidate=candidate)


def merge_side_readonly_words(
    *maps: Mapping[int, int] | None,
) -> dict[int, int] | None:
    """Merge maps for a *single* side; reject conflicting values at one VA."""
    merged: dict[int, int] = {}
    for mapping in maps:
        if not mapping:
            continue
        for key, value in mapping.items():
            address = int(key) & 0xFFFFFFFF
            word = int(value) & 0xFFFFFFFF
            prior = merged.get(address)
            if prior is not None and prior != word:
                raise ValueError(
                    f"readonly word conflict at 0x{address:08X}: "
                    f"0x{prior:08X} vs 0x{word:08X}"
                )
            merged[address] = word
    return merged or None


def readonly_word_byte_constraints(
    initial_memory: Any,
    evidence: Sequence[ReadonlyWordEvidence],
    ops: Any,
) -> list[Any]:
    """Bind ``Select(initial_memory, addr+i) == byte`` for each evidence word."""
    constraints: list[Any] = []
    seen: set[int] = set()
    for item in evidence:
        if item.address in seen:
            continue
        seen.add(item.address)
        for offset, byte in enumerate(item.image_bytes()):
            address = ops.const((item.address + offset) & 0xFFFFFFFF)
            constraints.append(
                ops.eq(ops.load_byte(initial_memory, address), ops.const(byte)),
            )
    return constraints
