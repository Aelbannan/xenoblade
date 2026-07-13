from __future__ import annotations

import re
from dataclasses import dataclass, field
from pathlib import Path
from typing import Iterable, Optional

from .parser import SymbolEntry


SPLIT_UNIT_RE = re.compile(r"^([^\s:#]+):\s*$")
TEXT_RANGE_RE = re.compile(
    r"^\s+\.text\s+start:(?P<start>0x[0-9A-Fa-f]+)\s+end:(?P<end>0x[0-9A-Fa-f]+)"
)


@dataclass
class SplitUnit:
    path: str
    text_start: Optional[int] = None
    text_end: Optional[int] = None


@dataclass
class SourceXref:
    placeholder: str
    symbols: list[SymbolEntry] = field(default_factory=list)
    split_units: list[SplitUnit] = field(default_factory=list)
    source_files: list[Path] = field(default_factory=list)
    configure_refs: list[str] = field(default_factory=list)


def load_splits(path: Path) -> list[SplitUnit]:
    units: list[SplitUnit] = []
    current: SplitUnit | None = None
    with path.open(encoding="utf-8") as f:
        for line in f:
            unit_match = SPLIT_UNIT_RE.match(line)
            if unit_match:
                if current is not None:
                    units.append(current)
                current = SplitUnit(path=unit_match.group(1))
                continue
            if current is None:
                continue
            range_match = TEXT_RANGE_RE.match(line)
            if range_match:
                current.text_start = int(range_match.group("start"), 16)
                current.text_end = int(range_match.group("end"), 16)
        if current is not None:
            units.append(current)
    return units


def find_split_units_for_address(units: list[SplitUnit], address: int) -> list[SplitUnit]:
    hits: list[SplitUnit] = []
    for unit in units:
        if unit.text_start is None or unit.text_end is None:
            continue
        if unit.text_start <= address < unit.text_end:
            hits.append(unit)
    return hits


def find_split_units_for_placeholder(units: list[SplitUnit], placeholder: str) -> list[SplitUnit]:
    addr = placeholder.rsplit("_", 1)[-1]
    try:
        target = int(addr, 16)
    except ValueError:
        return []
    exact = [u for u in units if u.path.endswith(placeholder + ".cpp")]
    if exact:
        return exact
    return find_split_units_for_address(units, target)


def scan_source_tree(root: Path, placeholder: str) -> list[Path]:
    hits: list[Path] = []
    for path in root.rglob("*"):
        if not path.is_file():
            continue
        if path.suffix not in {".cpp", ".hpp", ".h", ".c", ".inc"}:
            continue
        try:
            text = path.read_text(encoding="utf-8")
        except (OSError, UnicodeDecodeError):
            continue
        if placeholder in text:
            hits.append(path)
    return sorted(hits)


def scan_configure(configure_path: Path, placeholder: str) -> list[str]:
    lines: list[str] = []
    with configure_path.open(encoding="utf-8") as f:
        for line in f:
            if placeholder in line:
                lines.append(line.strip())
    return lines


def build_xref(
    project_root: Path,
    region: str,
    placeholder: str,
    symbols: list[SymbolEntry],
) -> SourceXref:
    splits_path = project_root / "config" / region / "splits.txt"
    units = load_splits(splits_path) if splits_path.is_file() else []
    symbol_hits = [s for s in symbols if placeholder in s.name]
    split_hits = find_split_units_for_placeholder(units, placeholder)
    source_hits: list[Path] = []
    for scan_root in (project_root / "src", project_root / "libs"):
        if scan_root.is_dir():
            source_hits.extend(scan_source_tree(scan_root, placeholder))
    source_hits = sorted(set(source_hits))
    configure_path = project_root / "configure.py"
    configure_hits = scan_configure(configure_path, placeholder) if configure_path.is_file() else []
    return SourceXref(
        placeholder=placeholder,
        symbols=symbol_hits,
        split_units=split_hits,
        source_files=source_hits,
        configure_refs=configure_hits,
    )


def compare_regions(project_root: Path, placeholder: str, regions: Iterable[str]) -> dict[str, int]:
    counts: dict[str, int] = {}
    for region in regions:
        symbols_path = project_root / "config" / region / "symbols.txt"
        if not symbols_path.is_file():
            counts[region] = -1
            continue
        with symbols_path.open(encoding="utf-8") as f:
            counts[region] = sum(1 for line in f if placeholder in line)
    return counts
