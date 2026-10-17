#!/usr/bin/env python3
"""Curated per-expression pointer-arithmetic sites + accepted/unmatched split.

``smell_scan``'s ``ptr_arith``/``deref_arith`` metrics are line-based proxies:
a line carrying a cast plus an offset increments ``ptr_arith``, and the same
line's ``*(T*)(...)`` deref independently increments ``deref_arith`` — so one
expression can move two counters.  This module counts individual expressions
with the curated detector (``tools/coop/detect_pointer_arithmetic.py``) and
splits each TU's total by whether the *enclosing function* is already accepted
(``targets.json``), because only sites inside matched code are actionable:
unmatched bodies get rewritten while matching.

The split is a heuristic: function spans are derived from column-0 definition
lines and the acceptance lookup keys on the definition's last name component
(``CMenuSkipTimer::Init`` -> ``Init``).  It is good enough to rank burn-down
work and to report an actionable total; it is not a C++ parser.
"""

from __future__ import annotations

import json
import re
from pathlib import Path

from tools.coop.detect_pointer_arithmetic import CATEGORIES, find_source_files, scan_file

ROOT = Path(__file__).resolve().parents[2]
TARGETS = ROOT / "tools" / "coop" / "targets.json"
ACCEPTED_STATUSES = ("FULL_MATCH", "EQUIVALENT_MATCH")

# Column-0 definition start: optional `extern "C"` / storage qualifiers, then
# the last identifier before the argument list.  Definitions whose signature
# continues on the next line are still matched because the line has no `;`.
DEF_RE = re.compile(
    r'^(?!\s)(?:extern\s+"C"\s+)?(?:static\s+|inline\s+|virtual\s+)?'
    r'[A-Za-z_][\w:<>,*\s&]*?([A-Za-z_~]\w*)\s*\([^;]*$'
)
# The qualified name that precedes the argument list (`Klass::Method`,
# `cf::Foo::Bar`, `CTTask<9CTaskGame>::Move`).  Used instead of DEF_RE's last
# identifier so the accepted/unmatched split cannot confuse two classes that
# share a method name (`CUICfManager::Move` vs `CTTask<12CUICfManager>::Move`).
QUAL_RE = re.compile(
    r'([A-Za-z_~]\w*(?:\s*<[^;(){}]*>)?'
    r'(?:::[A-Za-z_~]\w*(?:\s*<[^;(){}]*>)?)*)\s*\([^;]*$'
)


def _read_lines(path) -> list[str]:
    try:
        return Path(path).read_text(errors="replace").splitlines()
    except OSError:
        return []


def function_spans(lines: list[str]) -> list[tuple[int, int, str]]:
    """Return ``(start_line, end_line, name)`` spans for column-0 definitions.

    A definition ends where the next one starts (minus one).  Blank lines and
    inter-function comments are therefore attributed to the preceding
    definition, which is harmless: the detector already skips comment-only
    lines.
    """
    starts: list[tuple[int, str]] = []
    n = len(lines)
    for i, raw in enumerate(lines):
        line = raw.rstrip("\n")
        if not line or line[0] in " \t/*#":
            continue
        m = DEF_RE.match(line)
        if not m:
            continue
        q = QUAL_RE.search(line)
        name = q.group(1).strip() if q else m.group(1)
        for j in range(i, min(i + 5, n)):
            if "{" in lines[j]:
                starts.append((i + 1, name))
                break
            if ";" in lines[j]:
                break
    spans: list[tuple[int, int, str]] = []
    for k, (ln, name) in enumerate(starts):
        end = starts[k + 1][0] - 1 if k + 1 < len(starts) else n
        spans.append((ln, end, name))
    return spans


def enclosing_name(spans: list[tuple[int, int, str]], line: int) -> str | None:
    lo, hi = 0, len(spans) - 1
    while lo <= hi:
        mid = (lo + hi) // 2
        start, end, name = spans[mid]
        if line < start:
            hi = mid - 1
        elif line > end:
            lo = mid + 1
        else:
            return name
    return None


def _qualified(fn: str) -> str:
    """``"cf::CActorParam::Foo()"`` -> ``"cf::CActorParam::Foo"`` (drop args)."""
    fn = (fn or "").strip()
    if "(" in fn:
        fn = fn.split("(", 1)[0]
    return fn.strip()


def _chain_ok(def_name: str, target_name: str) -> bool:
    """True when the class/namespace chains are consistent (one is a suffix of
    the other), so ``cf::CBattleManager::Foo`` matches ``CBattleManager::Foo``
    but ``CUICfManager::Move`` does not match ``CTTask<12CUICfManager>::Move``.
    """
    d = def_name.split("::")[:-1]
    t = target_name.split("::")[:-1]
    if not d or not t:
        return False
    n = min(len(d), len(t))
    return d[-n:] == t[-n:]


class AcceptedIndex:
    """Per-source accepted function names, matched **class-aware**.

    The original lookup keyed on the bare last name component, so
    ``CUICfManager::Move`` (HIGH_MATCH) matched the accepted
    ``CTTask<12CUICfManager>::Move``.  This index matches qualified definition
    names only: exact qualified match, chain-suffix match, or (for targets the
    symbol table records unqualified) last-component match.
    """

    def __init__(self) -> None:
        self.full: set[tuple[str, str]] = set()
        self.by_short: dict[tuple[str, str], set[str]] = {}

    def add(self, src: str, name: str) -> None:
        if not name:
            return
        self.full.add((src, name))
        self.by_short.setdefault((src, name.split("::")[-1]), set()).add(name)

    def matches(self, src: str, def_name: str) -> bool:
        if (src, def_name) in self.full:
            return True
        short = def_name.split("::")[-1]
        qualified_def = "::" in def_name
        for target in self.by_short.get((src, short), ()):
            if not qualified_def:
                return True  # unqualified def (free function): last-component match
            # A qualified def only matches a qualified target whose class chain
            # is consistent.  Targets the symbol table records unqualified
            # (`Move()`) cannot disambiguate classes, so they are ignored here.
            if "::" in target and _chain_ok(def_name, target):
                return True
        return False


def accepted_index(targets_path=None) -> AcceptedIndex:
    path = Path(targets_path) if targets_path else TARGETS
    data = json.loads(path.read_text())
    idx = AcceptedIndex()
    for t in data.get("targets", []):
        if t.get("status") not in ACCEPTED_STATUSES:
            continue
        src = t.get("source") or ""
        fn = _qualified(t.get("function") or "")
        if fn:
            idx.add(src, fn)
            continue
        sym = t.get("symbol") or ""
        base = sym.split("__")[0] if "__" in sym else sym
        if base:
            idx.add(src, base)
    return idx


def accepted_keys(targets_path=None) -> set[tuple[str, str]]:
    """``{(source.cpp, qualified_name)}`` for every accepted target.

    Plain set for callers/tests that only need membership; the class-aware
    matching used by :func:`scan_roots` is :class:`AcceptedIndex`.
    """
    return set(accepted_index(targets_path).full)


class SiteStats:
    __slots__ = ("total", "accepted", "by_category", "by_category_accepted")

    def __init__(self):
        self.total = 0
        self.accepted = 0
        self.by_category: dict[str, int] = {}
        self.by_category_accepted: dict[str, int] = {}


def scan_file_sites(rel: str, abs_path, accepted, suffixes=(".cpp",)) -> SiteStats | None:
    if suffixes and not rel.endswith(suffixes):
        return None
    enabled = {c: True for c in CATEGORIES}
    findings = scan_file(rel, abs_path, enabled)
    if not findings:
        return None
    spans = function_spans(_read_lines(abs_path))
    st = SiteStats()
    for f in findings:
        st.total += 1
        cat = f["category"]
        st.by_category[cat] = st.by_category.get(cat, 0) + 1
        name = enclosing_name(spans, f["line"])
        if name is None:
            continue
        if isinstance(accepted, AcceptedIndex):
            ok = accepted.matches(rel, name)
        else:
            ok = (rel, name) in accepted
        if ok:
            st.accepted += 1
            st.by_category_accepted[cat] = st.by_category_accepted.get(cat, 0) + 1
    return st


def scan_roots(roots, accepted=None, suffixes=(".cpp",)) -> dict[str, SiteStats]:
    """Map ``relpath -> SiteStats`` for every file under ``roots`` with sites."""
    if accepted is None:
        accepted = accepted_index()
    per: dict[str, SiteStats] = {}
    for root in roots:
        for rel, abs_path in find_source_files([root]):
            st = scan_file_sites(rel, abs_path, accepted, suffixes)
            if st is not None:
                per[rel] = st
    return per
