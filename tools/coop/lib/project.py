from __future__ import annotations

import json
import os
import shutil
import subprocess
from dataclasses import dataclass
from pathlib import Path
from typing import Any, Dict, List, Optional

from tools.coop.lib.config import CoopConfig


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


def _path_or_none(root: Path, value: Any) -> Optional[Path]:
    if not value:
        return None
    path = Path(value)
    if not path.is_absolute():
        path = root / path
    return path
