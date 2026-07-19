"""Greedy failure shrinker for randomized PPC32 programs (P2-01).

Preserves the original reproduction payload, then greedily reduces instruction
count and simplifies immediates while a caller-supplied failure predicate still
holds.
"""

from __future__ import annotations

import copy
from typing import Any, Callable

from ..fixtures.encode import words_to_hex

FailPredicate = Callable[[dict[str, Any]], bool]


def _words(program: dict[str, Any]) -> list[int]:
    return [int(w, 16) & 0xFFFFFFFF for w in program["program_hex"].split()]


def _with_words(program: dict[str, Any], words: list[int]) -> dict[str, Any]:
    out = copy.deepcopy(program)
    out["program_hex"] = words_to_hex(words) if words else ""
    return out


def _try_immediate_simplify(word: int) -> list[int]:
    """Yield simpler encodings that keep the primary opcode nibble intact.

    D-form immediates (low 16 bits) and M-form SH/MB/ME fields are nudged toward
    zero when that produces a distinct word. X-form encodings are left alone.
    """
    primary = (word >> 26) & 0x3F
    candidates: list[int] = []
    # D-form family: primary in the common integer/memory set.
    if primary in {
        7, 8, 10, 11, 12, 13, 14, 15, 24, 25, 26, 27, 28, 29,
        32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45,
        56, 57, 60, 61,
    }:
        imm = word & 0xFFFF
        if imm != 0:
            candidates.append((word & ~0xFFFF) | 0)
        if imm not in (0, 1) and imm != 0xFFFF:
            candidates.append((word & ~0xFFFF) | 1)
        if imm != 0xFFFF and (imm & 0x8000):
            candidates.append((word & ~0xFFFF) | 0xFFFF)
    # M-form rotate: primary 20/21/23 — zero SH when nonzero.
    if primary in (20, 21, 23):
        sh = (word >> 11) & 0x1F
        if sh != 0:
            candidates.append((word & ~(0x1F << 11)) | (0 << 11))
    # Deduplicate while preserving order.
    seen: set[int] = set()
    out: list[int] = []
    for cand in candidates:
        cand &= 0xFFFFFFFF
        if cand != word and cand not in seen:
            seen.add(cand)
            out.append(cand)
    return out


def shrink_program(
    program: dict[str, Any],
    still_fails: FailPredicate,
    *,
    max_rounds: int = 256,
) -> dict[str, Any]:
    """Greedily shrink ``program`` while ``still_fails`` remains true.

    Returns a dict with ``original``, ``shrunk``, instruction counts, and whether
    any reduction succeeded. Empty programs are never accepted: at least one
    instruction must remain.
    """
    if not still_fails(program):
        raise ValueError("shrink_program requires the original program to fail")

    original = copy.deepcopy(program)
    current = copy.deepcopy(program)
    words = _words(current)
    rounds = 0
    changed = True

    while changed and rounds < max_rounds and len(words) > 1:
        changed = False
        rounds += 1
        # Prefer deleting later instructions first (preserves early setup).
        for index in range(len(words) - 1, -1, -1):
            if len(words) <= 1:
                break
            candidate_words = words[:index] + words[index + 1 :]
            if not candidate_words:
                continue
            candidate = _with_words(current, candidate_words)
            try:
                if still_fails(candidate):
                    current = candidate
                    words = candidate_words
                    changed = True
                    break
            except Exception:  # noqa: BLE001 — keep exploring other deletions
                continue

    # Immediate simplification pass (no length change).
    imm_changed = True
    while imm_changed and rounds < max_rounds:
        imm_changed = False
        rounds += 1
        for index, word in enumerate(words):
            for simpler in _try_immediate_simplify(word):
                candidate_words = list(words)
                candidate_words[index] = simpler
                candidate = _with_words(current, candidate_words)
                try:
                    if still_fails(candidate):
                        current = candidate
                        words = candidate_words
                        imm_changed = True
                        changed = True
                        break
                except Exception:  # noqa: BLE001
                    continue
            if imm_changed:
                break

    original_count = len(_words(original))
    shrunk_count = len(words)
    return {
        "original": original,
        "shrunk": current,
        "original_instruction_count": original_count,
        "shrunk_instruction_count": shrunk_count,
        "reduced": shrunk_count < original_count or current["program_hex"] != original["program_hex"],
        "rounds": rounds,
    }
