from __future__ import annotations

import shutil
from dataclasses import dataclass, field
from pathlib import Path

from .mwcc import mangle_length_prefix
from .parser import load_symbols
from .xref import build_xref, scan_source_tree


TEXT_SCAN_ROOTS = ("src", "libs", "configure.py", "config", "docs")
TEXT_SCAN_SUFFIXES = {".cpp", ".hpp", ".h", ".c", ".inc", ".py", ".md", ".csv", ".jsonl", ".txt"}
SKIP_DIR_NAMES = {".git", "build", "orig", "node_modules", "__pycache__"}


@dataclass(frozen=True)
class FileEdit:
    path: Path
    replacements: int
    renamed_to: Path | None = None


@dataclass
class FullRenamePlan:
    old_type: str
    new_type: str
    symbol_files: list[Path] = field(default_factory=list)
    text_files: list[Path] = field(default_factory=list)
    file_renames: list[tuple[Path, Path]] = field(default_factory=list)

    @property
    def total_files(self) -> int:
        return len(self.symbol_files) + len(self.text_files) + len(self.file_renames)


def _should_scan(path: Path) -> bool:
    if any(part in SKIP_DIR_NAMES for part in path.parts):
        return False
    if path.is_dir():
        return False
    if path.name == "symbols.txt":
        return False
    return path.suffix in TEXT_SCAN_SUFFIXES or path.name == "configure.py"


def collect_text_files(project_root: Path, old_type: str, region: str) -> list[Path]:
    symbols = load_symbols(project_root / "config" / region / "symbols.txt")
    xref = build_xref(project_root, region, old_type, symbols)
    hits: set[Path] = set(xref.source_files)

    for root_name in TEXT_SCAN_ROOTS:
        root = project_root / root_name if root_name != "configure.py" else project_root / "configure.py"
        if root.is_file():
            if old_type in root.read_text(encoding="utf-8", errors="ignore"):
                hits.add(root)
            continue
        if not root.is_dir():
            continue
        for path in root.rglob("*"):
            if not _should_scan(path):
                continue
            try:
                text = path.read_text(encoding="utf-8")
            except (OSError, UnicodeDecodeError):
                continue
            if old_type in text or mangle_length_prefix(old_type) in text:
                hits.add(path)

    for scan_root in (project_root / "src", project_root / "libs"):
        if scan_root.is_dir():
            hits.update(scan_source_tree(scan_root, old_type))

    return sorted(hits)


def collect_file_renames(project_root: Path, old_type: str, new_type: str) -> list[tuple[Path, Path]]:
    renames: list[tuple[Path, Path]] = []
    for root in (project_root / "src", project_root / "libs"):
        if not root.is_dir():
            continue
        for suffix in (".cpp", ".hpp", ".h"):
            old_path = None
            for path in root.rglob(f"{old_type}{suffix}"):
                new_path = path.with_name(f"{new_type}{suffix}")
                if path != new_path:
                    renames.append((path, new_path))
    return sorted(renames, key=lambda pair: str(pair[0]))


def replace_type_text(text: str, old_type: str, new_type: str) -> tuple[str, int]:
    old_prefixed = mangle_length_prefix(old_type)
    new_prefixed = mangle_length_prefix(new_type)
    updated = text.replace(old_prefixed, new_prefixed).replace(old_type, new_type)
    if updated == text:
        return text, 0
    count = sum(
        1
        for before, after in zip(text.splitlines(), updated.splitlines())
        if before != after
    )
    if count == 0 and updated != text:
        count = 1
    return updated, count


def plan_full_rename(project_root: Path, region: str, old_type: str, new_type: str) -> FullRenamePlan:
    symbol_files = [
        project_root / "config" / r / "symbols.txt"
        for r in ("us", "eu", "jp")
        if (project_root / "config" / r / "symbols.txt").is_file()
    ]
    text_files = collect_text_files(project_root, old_type, region)
    file_renames = collect_file_renames(project_root, old_type, new_type)
    return FullRenamePlan(
        old_type=old_type,
        new_type=new_type,
        symbol_files=symbol_files,
        text_files=text_files,
        file_renames=file_renames,
    )


def apply_text_file(path: Path, old_type: str, new_type: str, dry_run: bool) -> FileEdit:
    original = path.read_text(encoding="utf-8")
    updated, count = replace_type_text(original, old_type, new_type)
    if count and not dry_run:
        path.write_text(updated, encoding="utf-8")
    return FileEdit(path=path, replacements=count)


def apply_file_renames(renames: list[tuple[Path, Path]], dry_run: bool) -> list[FileEdit]:
    edits: list[FileEdit] = []
    for old_path, new_path in renames:
        if dry_run:
            edits.append(FileEdit(path=old_path, replacements=0, renamed_to=new_path))
            continue
        new_path.parent.mkdir(parents=True, exist_ok=True)
        if new_path.exists():
            raise FileExistsError(f"rename target already exists: {new_path}")
        shutil.move(str(old_path), str(new_path))
        edits.append(FileEdit(path=old_path, replacements=0, renamed_to=new_path))
    return edits


def apply_full_rename(
    project_root: Path,
    old_type: str,
    new_type: str,
    regions: list[str],
    region: str,
    dry_run: bool,
    rename_files: bool,
    include_symbols: bool,
) -> tuple[list[FileEdit], list[Path]]:
    from .rename import apply_rename

    edits: list[FileEdit] = []
    symbol_touched: list[Path] = []

    if include_symbols:
        _, symbol_touched = apply_rename(project_root, old_type, new_type, regions, dry_run)

    plan = plan_full_rename(project_root, region, old_type, new_type)
    for path in plan.text_files:
        if include_symbols and path in symbol_touched:
            continue
        if not path.is_file():
            continue
        edits.append(apply_text_file(path, old_type, new_type, dry_run))

    if rename_files:
        rename_edits = apply_file_renames(plan.file_renames, dry_run)
        edits.extend(rename_edits)

    return edits, symbol_touched
