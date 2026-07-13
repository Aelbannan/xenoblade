from __future__ import annotations

from dataclasses import dataclass, field
from pathlib import Path

from .mwcc import mangle_length_prefix, rename_mangling_compatible, replace_type_in_symbol
from .parser import load_symbols


@dataclass(frozen=True)
class RenameChange:
    region: str
    line_no: int
    old_name: str
    new_name: str


@dataclass
class RenamePlan:
    old_type: str
    new_type: str
    mangling_compatible: bool
    changes: list[RenameChange] = field(default_factory=list)

    @property
    def change_count(self) -> int:
        return len(self.changes)


def plan_rename(
    project_root: Path,
    old_type: str,
    new_type: str,
    regions: list[str],
) -> RenamePlan:
    changes: list[RenameChange] = []
    for region in regions:
        symbols_path = project_root / "config" / region / "symbols.txt"
        if not symbols_path.is_file():
            continue
        with symbols_path.open(encoding="utf-8") as f:
            for line_no, line in enumerate(f, start=1):
                if old_type not in line:
                    continue
                name_part = line.split("=", 1)[0].strip()
                new_name = replace_type_in_symbol(name_part, old_type, new_type)
                if new_name == name_part:
                    continue
                changes.append(
                    RenameChange(
                        region=region,
                        line_no=line_no,
                        old_name=name_part,
                        new_name=new_name,
                    )
                )
    return RenamePlan(
        old_type=old_type,
        new_type=new_type,
        mangling_compatible=rename_mangling_compatible(old_type, new_type),
        changes=changes,
    )


def apply_rename(
    project_root: Path,
    old_type: str,
    new_type: str,
    regions: list[str],
    dry_run: bool,
) -> tuple[int, list[Path]]:
    """Update config/<region>/symbols.txt. Returns (lines_changed, files_touched)."""
    touched: list[Path] = []
    total = 0
    old_prefixed = mangle_length_prefix(old_type)
    new_prefixed = mangle_length_prefix(new_type)
    for region in regions:
        symbols_path = project_root / "config" / region / "symbols.txt"
        if not symbols_path.is_file():
            continue
        original = symbols_path.read_text(encoding="utf-8")
        if old_type not in original and old_prefixed not in original:
            continue
        updated = original.replace(old_prefixed, new_prefixed).replace(old_type, new_type)
        if updated == original:
            continue
        count = sum(
            1
            for before, after in zip(original.splitlines(), updated.splitlines())
            if before != after
        )
        total += count
        touched.append(symbols_path)
        if not dry_run:
            symbols_path.write_text(updated, encoding="utf-8")
    return total, touched
