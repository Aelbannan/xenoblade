"""Code-only unit kind (PLAN.md §17.7) — shared helpers.

``configure.py`` marks a unit ``Object(MatchingCodeOnly, "<name>")`` when its
code matches and is linked but its data is still owned by another TU / shared
data range.  This module is the single parser for that label, used by
``readme_status.py`` and ``code_only_check.py`` so neither needs to execute
``configure.py``.
"""

from __future__ import annotations

import re
from pathlib import Path

#: ``Object(MatchingCodeOnly, "kyoshin/Foo.cpp"`` (kwargs may follow).
_CODE_ONLY_RE = re.compile(r"""Object\(\s*MatchingCodeOnly\s*,\s*["']([^"']+)["']""")
#: Every ``Object(<kind>, "<name>"`` with a literal kind token.
_OBJECT_RE = re.compile(r"""Object\(\s*(\w+)\s*,\s*["']([^"']+)["']""")


def code_only_units(configure_text: str) -> list[str]:
    """Object names labelled ``MatchingCodeOnly``, in file order, comments ignored."""
    out: list[str] = []
    for line in configure_text.splitlines():
        code = line.split("#", 1)[0]
        out.extend(_CODE_ONLY_RE.findall(code))
    return out


def unit_kind_counts(configure_text: str) -> dict[str, int]:
    """Count ``Object(<kind>, ...)`` occurrences per literal kind token."""
    counts: dict[str, int] = {}
    for line in configure_text.splitlines():
        code = line.split("#", 1)[0]
        for kind, _name in _OBJECT_RE.findall(code):
            counts[kind] = counts.get(kind, 0) + 1
    return counts


def load_code_only_units(root: Path) -> list[str]:
    return code_only_units((root / "configure.py").read_text(encoding="utf-8"))


def object_stem(name: str) -> str:
    """``kyoshin/Foo.cpp`` -> ``kyoshin/Foo`` (the build/<region>/{src,obj} stem)."""
    return str(Path(name).with_suffix("")).replace("\\", "/")
