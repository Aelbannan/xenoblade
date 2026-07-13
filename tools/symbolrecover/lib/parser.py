from __future__ import annotations

import re
from dataclasses import dataclass
from pathlib import Path
from typing import Iterator, Optional

SYMBOL_LINE_RE = re.compile(
    r"^(?P<name>[^=]+?)\s*=\s*(?P<section>[^:]+):(?P<address>0x[0-9A-Fa-f]+)"
    r"(?:\s*;\s*//\s*type:(?P<type>\S+)(?:\s+size:(?P<size>0x[0-9A-Fa-f]+))?"
    r"(?:\s+scope:(?P<scope>\S+))?(?:\s+align:(?P<align>\d+))?)?"
)

PLACEHOLDER_RE = re.compile(
    r"^(?P<kind>UnkClass|UnkStruct|Class)_(?P<addr>[0-9A-Fa-f]{8})$"
)


@dataclass(frozen=True)
class SymbolEntry:
    name: str
    section: str
    address: int
    sym_type: Optional[str] = None
    size: Optional[int] = None
    scope: Optional[str] = None
    align: Optional[int] = None
    line_no: int = 0
    raw_line: str = ""

    @property
    def address_hex(self) -> str:
        return f"0x{self.address:08X}"


def parse_symbol_line(line: str, line_no: int = 0) -> Optional[SymbolEntry]:
    stripped = line.strip()
    if not stripped or stripped.startswith("#"):
        return None
    match = SYMBOL_LINE_RE.match(stripped)
    if not match:
        return None
    groups = match.groupdict()
    size = int(groups["size"], 16) if groups.get("size") else None
    align = int(groups["align"]) if groups.get("align") else None
    return SymbolEntry(
        name=groups["name"].strip(),
        section=groups["section"],
        address=int(groups["address"], 16),
        sym_type=groups.get("type"),
        size=size,
        scope=groups.get("scope"),
        align=align,
        line_no=line_no,
        raw_line=line.rstrip("\n"),
    )


def load_symbols(path: Path) -> list[SymbolEntry]:
    entries: list[SymbolEntry] = []
    with path.open(encoding="utf-8") as f:
        for line_no, line in enumerate(f, start=1):
            entry = parse_symbol_line(line, line_no=line_no)
            if entry is not None:
                entries.append(entry)
    return entries


def iter_placeholder_types(name: str) -> Iterator[str]:
    """Yield placeholder type tokens embedded in a mangled symbol name."""
    for kind in ("UnkClass", "UnkStruct", "Class"):
        prefix = kind + "_"
        start = 0
        while True:
            idx = name.find(prefix, start)
            if idx < 0:
                break
            end = idx + len(prefix) + 8
            if end <= len(name):
                candidate = name[idx:end]
                if PLACEHOLDER_RE.match(candidate):
                    yield candidate
            start = idx + len(prefix)


def is_placeholder_type(type_name: str) -> bool:
    return PLACEHOLDER_RE.match(type_name) is not None


def placeholder_address(type_name: str) -> Optional[str]:
    match = PLACEHOLDER_RE.match(type_name)
    if match is None:
        return None
    return match.group("addr")
