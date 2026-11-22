"""Banned-construct counters that faithful's HARD categories do not cover.

Counted per file after masking comments and string literals.  The land gate
ratchets these per unit (no rise), so pre-existing legitimate uses (e.g. SDK
asm that retail really has) are tolerated while new ones are rejected.
"""
from __future__ import annotations

import re

_COMMENT = re.compile(r"//[^\n]*|/\*.*?\*/", re.S)
_STRING = re.compile(r'"(?:\\.|[^"\\\n])*"|\'(?:\\.|[^\'\\\n])*\'')

BANS: dict[str, re.Pattern[str]] = {
    # inline / function-level assembly
    "asm": re.compile(r"\b(?:asm|__asm__|__asm)\b\s*(?:[({]|volatile\b|[\w \t*:]+\()"),
    # any pragma other than include guards / plain push-pop bookkeeping
    "pragma": re.compile(r"^[ \t]*#[ \t]*pragma[ \t]+(?!once\b)\w+", re.M),
    "line_directive": re.compile(r"^[ \t]*#[ \t]*line\b", re.M),
    "uintptr": re.compile(r"\buintptr_t\b"),
    "intrinsic": re.compile(
        r"\b__(?:lwbrx|lhbrx|stwbrx|sthbrx|sync|isync|eieio|mftb|cntlzw|rlwinm|rlwimi|"
        r"fnabs|frsqrte|fres|fsel|dcbz|dcbf|dcbst|dcbi|dcbt|icbi|mfspr|mtspr|mfmsr|mtmsr)\s*\("),
    # hand-written vtable / RTTI object references
    "hand_vtable": re.compile(r"\b__vt__\w+|\b__RTTI__\w+|\w+_typestr\b|\w+_hierarchy\b"),
    # any pre-mangled C++ name used as an identifier (calls, externs, addresses)
    "premangled": re.compile(r"\b[A-Za-z_]\w*?__(?:Q\d|\d+[A-Za-z_])\w*?F[A-Za-z0-9_]+\b"),
    # byte-array blobs with brace initialisers
    "byte_blob": re.compile(r"\b(?:u8|s8|char|unsigned\s+char)\s+\w+\s*\[[^\]]*\]\s*=\s*\{"),
    # local pointer bound to the address of one scalar member/local, used to
    # re-read the same object through a "view" (forces a reload / changes
    # register allocation): `const u32* pFlags = &self->flags;`.  Indexed
    # element addresses (`&a[i]`) and struct-typed members are not counted.
    "scalar_ptr_alias": re.compile(
        r"(?m)^[ \t]*(?:const\s+)?(?:u8|s8|u16|s16|u32|s32|u64|s64|int|unsigned(?:\s+int)?|"
        r"short|long|f32|f64|float|double|bool|BOOL)\s*(?:const\s+)?\*\s*\w+\s*=\s*"
        r"&\s*[A-Za-z_]\w*(?:\s*(?:->|\.)\s*[A-Za-z_]\w*)*\s*;"),
    # local pointer bound to a *cast* address of a plain object:
    # `Vec* pMag = (Vec*)&magIn;` (the same object seen through another type).
    "cast_addr_alias": re.compile(
        r"(?m)^[ \t]*(?:const\s+)?[A-Za-z_][\w:]*\s*\*\s*\w+\s*=\s*"
        r"\(\s*(?:const\s+)?[A-Za-z_][\w:]*\s*\*\s*\)\s*&\s*[A-Za-z_]\w*\s*;"),
    # extern data declared under an address-suffixed name instead of its real
    # name (`extern const GXColorS10 DefaultBlackColor_8066B550;`).
    "addr_extern": re.compile(
        r"\bextern\b[^;(){}\n]*?\b(?!lbl_)[A-Za-z_]\w*_[89][0-9A-Fa-f]{7}\s*(?:\[[^\]]*\]\s*)*;"),
}

# file-local `static inline` helper with exactly one call site: the typical
# shape of an invented helper whose only purpose is to perturb inlining /
# register allocation (or to relocate a banned construct out of sight).
_STATIC_INLINE = re.compile(
    r"\bstatic\s+(?:inline|__inline)\b[^;{}()]*?\b([A-Za-z_]\w*)\s*\([^;{}]*\)\s*(?:const\s*)?\{")


def _single_use_static_inline(code: str) -> int:
    hits = 0
    for name in set(_STATIC_INLINE.findall(code)):
        if len(re.findall(rf"\b{re.escape(name)}\s*\(", code)) == 2:  # definition + one call
            hits += 1
    return hits


def mask(text: str) -> str:
    """Blank comments and string/char literals, preserving offsets and newlines."""
    def blank(m: re.Match[str]) -> str:
        return "".join("\n" if c == "\n" else " " for c in m.group(0))
    return _STRING.sub(blank, _COMMENT.sub(blank, text))


def count(text: str) -> dict[str, int]:
    code = mask(text)
    out = {name: len(p.findall(code)) for name, p in BANS.items()}
    out["single_use_inline"] = _single_use_static_inline(code)
    return out


# Snapshot stamp: a baseline measured with a different counter set must be
# re-measured, otherwise every pre-existing hit of a new counter reads as a rise.
KEYS = tuple(sorted([*BANS, "single_use_inline"]))
