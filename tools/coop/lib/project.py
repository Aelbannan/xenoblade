from __future__ import annotations

import json
import os
import shutil
import subprocess
from dataclasses import dataclass
from functools import lru_cache
from pathlib import Path
from typing import Any, Dict, List, Optional, Tuple

from tools.coop.lib.config import CoopConfig
from tools.symbolrecover.lib.parser import SymbolEntry, load_symbols


@dataclass
class ObjdiffUnit:
    name: str
    target_path: Optional[Path]
    base_path: Optional[Path]
    source_path: Optional[Path]


class Project:
    def __init__(self, config: CoopConfig) -> None:
        self.config = config
        self.root = config.project_root

    def ninja_bin(self) -> str:
        if self.config.ninja:
            return str(self.config.ninja)
        return shutil.which("ninja") or "ninja"

    def objdiff_bin(self) -> str:
        if self.config.objdiff_cli and self.config.objdiff_cli.is_file():
            return str(self.config.objdiff_cli.resolve())
        downloaded = self.root / "build" / "tools" / "objdiff-cli"
        if downloaded.is_file():
            return str(downloaded)
        on_path = shutil.which("objdiff-cli")
        if on_path:
            return on_path
        raise FileNotFoundError(
            "objdiff-cli not found. Run configure + ninja once, or set objdiff_cli in coop.yaml"
        )

    def run(
        self,
        args: List[str],
        *,
        check: bool = True,
        capture: bool = False,
    ) -> subprocess.CompletedProcess[str]:
        return subprocess.run(
            args,
            cwd=self.root,
            check=check,
            text=True,
            capture_output=capture,
            env=os.environ.copy(),
        )

    def configure(self) -> None:
        cmd = ["python3", "configure.py", "--version", self.config.region]
        cmd.extend(self.config.configure_args)
        if self.config.non_matching:
            cmd.append("--non-matching")
        self.run(cmd)

    def ninja_build(self, target: str) -> None:
        self.run([self.ninja_bin(), target])

    def build_object_for_source(self, source: Path) -> Path:
        rel = source.relative_to(self.root)
        if rel.suffix not in {".c", ".cpp", ".cc", ".cxx"}:
            raise ValueError(f"Not a C/C++ source file: {source}")
        obj_rel = Path("build") / self.config.region / "src" / rel.with_suffix(".o")
        self.ninja_build(str(obj_rel))
        return self.root / obj_rel

    def load_objdiff_units(self) -> List[ObjdiffUnit]:
        path = self.config.objdiff_json
        if not path.is_file():
            raise FileNotFoundError(
                f"{path} missing. Run: python tools/coop/run.py configure"
            )
        with path.open(encoding="utf-8") as f:
            data = json.load(f)
        units: List[ObjdiffUnit] = []
        for unit in data.get("units", []):
            units.append(
                ObjdiffUnit(
                    name=unit["name"],
                    target_path=_path_or_none(self.root, unit.get("target_path")),
                    base_path=_path_or_none(self.root, unit.get("base_path")),
                    source_path=_path_or_none(
                        self.root, (unit.get("metadata") or {}).get("source_path")
                    ),
                )
            )
        return units

    def resolve_unit(self, hint: str) -> ObjdiffUnit:
        units = self.load_objdiff_units()
        hint_path = hint.replace("\\", "/")

        for unit in units:
            if unit.name == hint_path or unit.name.endswith("/" + hint_path):
                return unit
            if unit.source_path and str(unit.source_path.relative_to(self.root)) == hint_path:
                return unit
            if unit.source_path and unit.source_path.name == Path(hint_path).name:
                if hint_path in str(unit.source_path):
                    return unit

        stem = Path(hint_path).stem
        matches = [u for u in units if u.name.endswith("/" + stem) or u.name.endswith(stem)]
        if len(matches) == 1:
            return matches[0]
        if len(matches) > 1:
            names = ", ".join(u.name for u in matches[:5])
            raise ValueError(f"Ambiguous unit '{hint}'. Matches include: {names}")

        raise ValueError(f"No objdiff unit found for '{hint}'")

    @property
    def linked_elf_path(self) -> Path:
        """Path to the ninja-produced linked ELF (consumed by ``elf2dol``).

        Gitignored per ``.gitignore``; produced by ``ninja build/<region>/main.elf``.
        Used as a relocation-free candidate source for SMT linked-byes fallback.
        """
        return self.config.build_dir / "main.elf"

    @lru_cache(maxsize=4)
    def _symbol_table(self) -> Dict[str, SymbolEntry]:
        path = self.root / "config" / self.config.region / "symbols.txt"
        if not path.is_file():
            return {}
        entries: Dict[str, SymbolEntry] = {}
        for entry in load_symbols(path):
            entries[entry.name] = entry
        return entries

    def symbol_entry(self, name: str) -> Optional[SymbolEntry]:
        """Look up a mangled symbol in ``config/<region>/symbols.txt`` by name."""
        table = self._symbol_table()
        if name in table:
            return table[name]
        # Case-insensitive fallback (objdiff demangled tokens may differ in case).
        lowered = name.lower()
        for key, value in table.items():
            if key.lower() == lowered:
                return value
        return None

    def symbol_address(self, name: str) -> Optional[Tuple[int, int]]:
        """Return ``(virtual_address, size_bytes)`` for a named function symbol.

        Used to extract linked bytes from ``main.dol`` when the unlinked ``.o``
        pair carries unresolved relocations.
        """
        entry = self.symbol_entry(name)
        if entry is None or entry.size is None or entry.size <= 0:
            return None
        return entry.address, entry.size


def _path_or_none(root: Path, value: Any) -> Optional[Path]:
    if not value:
        return None
    path = Path(value)
    if not path.is_absolute():
        path = root / path
    return path
