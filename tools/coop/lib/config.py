from __future__ import annotations

import json
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any, List, Optional


@dataclass
class CoopConfig:
    project_root: Path
    region: str = "us"
    match_policy: str = "full"
    default_required_level: str = "EQUIVALENT_MATCH"
    objdiff_cli: Optional[Path] = None
    ninja: Optional[Path] = None
    configure_args: List[str] = field(default_factory=lambda: ["--map"])
    non_matching: bool = False
    targets_file: Path = Path("tools/coop/targets.json")
    attempt_log: Path = Path("docs/evidence/decomp/attempts.jsonl")
    report_cache: Path = Path("build/coop-last-report.json")
    objdiff_report_args: List[str] = field(default_factory=list)

    @property
    def build_dir(self) -> Path:
        return self.project_root / "build" / self.region

    @property
    def objdiff_json(self) -> Path:
        return self.project_root / "objdiff.json"

    @property
    def main_dol(self) -> Path:
        return self.project_root / "orig" / self.region / "sys" / "main.dol"

    def resolve(self, path: Path | str) -> Path:
        p = Path(path)
        if p.is_absolute():
            return p
        return self.project_root / p


def _load_json(path: Path) -> dict[str, Any]:
    with path.open(encoding="utf-8") as f:
        return json.load(f)


def load_config(config_path: Optional[Path], project_root: Path) -> CoopConfig:
    candidates: list[Path] = []
    if config_path is not None:
        candidates.append(config_path)
    candidates.extend(
        [
            project_root / "coop.json",
            project_root / "coop.yaml",
            project_root / "tools/coop/coop.json",
            project_root / "tools/coop/coop.yaml",
            project_root / "tools/coop/coop.example.json",
            project_root / "tools/coop/coop.example.yaml",
        ]
    )

    data: dict[str, Any] = {}
    chosen: Optional[Path] = None
    for candidate in candidates:
        if candidate.is_file():
            if candidate.suffix in {".yaml", ".yml"}:
                data = _load_yaml(candidate)
            else:
                data = _load_json(candidate)
            chosen = candidate
            break

    if chosen is None:
        raise FileNotFoundError(
            "No coop config found. Copy tools/coop/coop.example.json to coop.json"
        )

    root_value = str(data.get("project_root", "."))
    if root_value in (".", "./"):
        root = project_root.resolve()
    else:
        root = Path(root_value).expanduser().resolve()

    return CoopConfig(
        project_root=root,
        region=str(data.get("region", "us")),
        match_policy=str(data.get("match_policy", "full")),
        default_required_level=str(data.get("default_required_level", "EQUIVALENT_MATCH")),
        objdiff_cli=_optional_path(data.get("objdiff_cli")),
        ninja=_optional_path(data.get("ninja")),
        configure_args=list(data.get("configure_args", ["--map"])),
        non_matching=bool(data.get("non_matching", False)),
        targets_file=Path(data.get("targets_file", "tools/coop/targets.json")),
        attempt_log=Path(data.get("attempt_log", "docs/evidence/decomp/attempts.jsonl")),
        report_cache=Path(data.get("report_cache", "build/coop-last-report.json")),
        objdiff_report_args=list(data.get("objdiff_report_args", [])),
    )


def _optional_path(value: Any) -> Optional[Path]:
    if not value:
        return None
    return Path(value)


def _load_yaml(path: Path) -> dict[str, Any]:
    try:
        import yaml
    except ImportError as exc:
        raise ImportError(
            f"PyYAML is required to read {path}. Install with: pip install pyyaml\n"
            "Or use coop.json / targets.json instead."
        ) from exc
    with path.open(encoding="utf-8") as f:
        return yaml.safe_load(f) or {}
