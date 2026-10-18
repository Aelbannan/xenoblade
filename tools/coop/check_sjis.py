#!/usr/bin/env python3
"""Shift-JIS build-landmine guard.

Background: source files reach MWCC through ``sjiswrap``, which reads each
translation unit as UTF-8 and re-encodes it to Shift JIS on the fly
(``encoding_rs::SHIFT_JIS``). Any character with no Shift-JIS encoding —
e.g. U+2014 EM DASH, U+2013 EN DASH, U+2248 ALMOST EQUAL TO — makes sjiswrap
emit ``contains Shift JIS encoding errors`` for that TU. The breakage is
latent: a unit with an up-to-date ``.o`` keeps working until its source is
touched, so one stray punctuation byte silently blocks any later edit of the
file.

This checker enforces the repo-wide invariant: every ``src/`` / ``libs/``
source file must decode as UTF-8 *and* every character must round-trip
through Shift JIS. The check is file-level on purpose — sjiswrap encodes the
whole file without parsing C++, so a bad byte breaks the build whether it
sits in a comment or a string literal. (Fixes, in contrast, must stay
comment-only: changing a literal changes compiled bytes. See the module
docstring history: 63 TUs were fixed in one batch by replacing comment-only
punctuation with ASCII equivalents.)

Python's ``shift_jis`` codec is used as the oracle. It agrees with sjiswrap
on the repo corpus (verified: files holding only arrows/box-drawing/ellipsis/
Japanese build clean through sjiswrap; every em-dash file errored), and where
the two tables could theoretically differ the checker is deliberately
conservative — a false positive only asks for ASCII, while a miss re-breaks
the build.

Files that intentionally bypass sjiswrap (compiled with
``shift_jis=False, extra_cflags=["-enc UTF8"]`` in configure.py) are exempt
via ALLOWLIST below. Generated ``*.ctx.c`` files are never scanned.

Usage:
    python3 tools/coop/check_sjis.py [paths ...] [--json]
Exit code 0 = clean, 1 = violations found. Default paths: src/ and libs/.
"""
from __future__ import annotations

import argparse
import json
import pathlib

ROOT = pathlib.Path(__file__).resolve().parents[2]

# sjiswrap encodes these extensions (see encounter/sjiswrap is_text_file).
TEXT_EXTS = {
    ".c", ".cc", ".cp", ".cpp", ".cxx", ".c++",
    ".h", ".hh", ".hp", ".hpp", ".hxx", ".h++",
    ".pch", ".pch++", ".inc",
}

# Exempt by design: compiled with shift_jis=False + -enc UTF8, so sjiswrap
# never sees them and non-SJIS-safe bytes are harmless. Keep this list
# minimal — every other source file must be sjiswrap-clean.
ALLOWLIST = {
    # Japanese encoding-conversion tables; full of JIS-external kanji/ideoforms.
    "libs/RVL_SDK/src/revolution/enc/encjapanese.c",
    # Japanese error-message data compiled as UTF-8.
    "src/kyoshin/ErrMesData.cpp",
}


def _is_allowlisted(path: pathlib.Path) -> bool:
    try:
        rel = path.resolve().relative_to(ROOT.resolve()).as_posix()
    except ValueError:
        return False
    return rel in ALLOWLIST


def scan_text(text: str, name: str = "<text>") -> list[dict]:
    """Return one hit per offending line: chars with no Shift-JIS encoding."""
    bad_lines: dict[int, set[str]] = {}
    for lineno, line in enumerate(text.split("\n"), 1):
        for ch in line:
            if ord(ch) <= 0x7F:
                continue
            try:
                ch.encode("shift_jis")
            except UnicodeEncodeError:
                bad_lines.setdefault(lineno, set()).add(ch)
    return [
        {
            "line": lineno,
            "chars": "".join(sorted(chars)),
            "codepoints": [f"U+{ord(c):04X}" for c in sorted(chars)],
        }
        for lineno, chars in sorted(bad_lines.items())
    ]


def scan_bytes(data: bytes, name: str = "<bytes>") -> list[dict]:
    """Byte-level entry point: UTF-8 decode first, then scan_text."""
    try:
        text = data.decode("utf-8")
    except UnicodeDecodeError as e:
        return [{"line": None, "chars": "", "codepoints": [],
                 "error": f"not valid UTF-8: {e}"}]
    hits = scan_text(text, name)
    return hits


def scan_file(path: pathlib.Path) -> list[dict]:
    """Scan one file; allowlisted paths return []."""
    if _is_allowlisted(path):
        return []
    try:
        data = pathlib.Path(path).read_bytes()
    except OSError as e:
        return [{"line": None, "chars": "", "codepoints": [],
                 "error": f"unreadable: {e}"}]
    hits = scan_bytes(data, str(path))
    for h in hits:
        h["file"] = str(path)
    return hits


def iter_sources(roots: list[pathlib.Path]):
    """Yield sjiswrap-relevant source files, skipping generated *.ctx.c."""
    for root in roots:
        root = pathlib.Path(root)
        if root.is_file():
            if root.suffix in TEXT_EXTS and not root.name.endswith(".ctx.c"):
                yield root
            continue
        for p in sorted(root.rglob("*")):
            if not p.is_file():
                continue
            if p.suffix not in TEXT_EXTS:
                continue
            if p.name.endswith(".ctx.c"):
                continue
            yield p


def check_tree(roots: list[pathlib.Path] | None = None) -> list[dict]:
    """Scan the tree; returns all hits outside the allowlist."""
    if roots is None:
        roots = [ROOT / "src", ROOT / "libs"]
    hits: list[dict] = []
    for path in iter_sources(roots):
        hits += scan_file(path)
    return hits


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("paths", nargs="*", default=["src", "libs"],
                    help="files/dirs to scan (default: src libs)")
    ap.add_argument("--json", action="store_true", help="emit hits as JSON")
    args = ap.parse_args()

    hits: list[dict] = []
    for raw in args.paths:
        p = pathlib.Path(raw)
        if not p.is_absolute():
            p = ROOT / p
        if p.is_file():
            hits += scan_file(p)
        else:
            hits += check_tree([p])
    if args.json:
        print(json.dumps(hits, indent=1, ensure_ascii=False))
    else:
        for h in hits:
            loc = f"{h.get('file')}:{h.get('line')}" if h.get("line") else h.get("file")
            detail = h.get("error") or f"{' '.join(h['codepoints'])} ({h['chars']})"
            print(f"{loc}: {detail}")
        if hits:
            print(f"\n{len(hits)} Shift-JIS violation(s) — "
                  f"replace with ASCII or add to ALLOWLIST in tools/coop/check_sjis.py")
    return 1 if hits else 0


if __name__ == "__main__":
    raise SystemExit(main())
