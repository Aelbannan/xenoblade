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
    automatic_promotion: bool = False
    reject_architecture_models: tuple[str, ...] = (
        "broadway-ppc32-be-v18",
        "broadway-ppc32-be-v19",
        "broadway-ppc32-be-v20",
        "broadway-ppc32-be-v21",
        "broadway-ppc32-be-v22",
        "broadway-ppc32-be-v23",
        "broadway-ppc32-be-v24",
        "broadway-ppc32-be-v25",
        "broadway-ppc32-be-v26",
        "broadway-ppc32-be-v27",
        "broadway-ppc32-be-v28",
        "broadway-ppc32-be-v29",
        "broadway-ppc32-be-v30",
        "broadway-ppc32-be-v31",
        "broadway-ppc32-be-v32",
        "broadway-ppc32-be-v33",
        "broadway-ppc32-be-v34",
        "broadway-ppc32-be-v35",
        "broadway-ppc32-be-v36",
        "broadway-ppc32-be-v37",
    )
    allowed_confidence_tiers: frozenset[str] = frozenset({"A", "B"})
    # Required for promotion when capability assurance is authoritative
    # (shadow_mode=false / require_capability_assurance=true). Optional in
    # Wave 1 while shadow_mode remains the default.
    allowed_engine_sha256: str | None = None
    require_bounded_ram: bool = False
    memory_profile: str | None = None
    memory_ranges: list[str] = field(default_factory=list)
    floating_point_domain: dict[str, Any] | None = None
    capability_manifest_path: Path = Path("tools/coop/capability_manifest.json")
    allowed_tier_a_capabilities: dict[str, tuple[str, ...]] = field(default_factory=dict)
    capability_assurance_shadow_mode: bool = True
    require_capability_assurance: bool = False

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

    raw_reject = data.get("reject_architecture_models")
    if isinstance(raw_reject, list):
        reject_models = tuple(str(m) for m in raw_reject)
    else:
        reject_models = (
            "broadway-ppc32-be-v18",
            "broadway-ppc32-be-v19",
            "broadway-ppc32-be-v20",
            "broadway-ppc32-be-v21",
            "broadway-ppc32-be-v22",
            "broadway-ppc32-be-v23",
            "broadway-ppc32-be-v24",
            "broadway-ppc32-be-v25",
            "broadway-ppc32-be-v26",
            "broadway-ppc32-be-v27",
            "broadway-ppc32-be-v28",
            "broadway-ppc32-be-v29",
            "broadway-ppc32-be-v30",
            "broadway-ppc32-be-v31",
            "broadway-ppc32-be-v32",
            "broadway-ppc32-be-v33",
            "broadway-ppc32-be-v34",
            "broadway-ppc32-be-v35",
            "broadway-ppc32-be-v36",
            "broadway-ppc32-be-v37",
        )

    raw_tiers = data.get("allowed_confidence_tiers")
    if isinstance(raw_tiers, list):
        allowed_tiers = frozenset(str(t) for t in raw_tiers)
    else:
        allowed_tiers = frozenset({"A", "B"})

    capability_manifest_path = Path(
        data.get("capability_manifest_path", "tools/coop/capability_manifest.json")
    )
    allowed_tier_a: dict[str, tuple[str, ...]] = {}
    raw_allowed = data.get("allowed_tier_a_capabilities")
    if isinstance(raw_allowed, dict):
        for key, values in raw_allowed.items():
            if isinstance(values, list):
                allowed_tier_a[str(key)] = tuple(str(item) for item in values)

    # Prefer explicit coop.json knobs; otherwise load from the capability manifest.
    shadow_mode = data.get("capability_assurance_shadow_mode")
    require_assurance = data.get("require_capability_assurance")
    if not allowed_tier_a or shadow_mode is None or require_assurance is None:
        try:
            from tools.ppc_equivalence.capability_assurance import (
                load_capability_manifest,
            )

            manifest = load_capability_manifest(root / capability_manifest_path)
            if not allowed_tier_a:
                allowed_tier_a = dict(manifest.allowed_tier_a_capabilities)
            if shadow_mode is None:
                shadow_mode = manifest.shadow_mode
            if require_assurance is None:
                require_assurance = manifest.require_capability_assurance
        except (OSError, ValueError, TypeError, json.JSONDecodeError):
            if shadow_mode is None:
                shadow_mode = True
            if require_assurance is None:
                require_assurance = False

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
        automatic_promotion=bool(data.get("automatic_promotion", False)),
        reject_architecture_models=reject_models,
        allowed_confidence_tiers=allowed_tiers,
        allowed_engine_sha256=data.get("allowed_engine_sha256"),
        require_bounded_ram=bool(data.get("require_bounded_ram", False)),
        memory_profile=(
            str(data["memory_profile"])
            if data.get("memory_profile") is not None
            else None
        ),
        memory_ranges=[str(item) for item in data.get("memory_ranges", []) or []],
        floating_point_domain=(
            dict(data["floating_point_domain"])
            if isinstance(data.get("floating_point_domain"), dict)
            else None
        ),
        capability_manifest_path=capability_manifest_path,
        allowed_tier_a_capabilities=allowed_tier_a,
        capability_assurance_shadow_mode=bool(shadow_mode),
        require_capability_assurance=bool(require_assurance),
    )


def memory_environment_from_config(config: CoopConfig) -> dict[str, Any] | None:
    """Build a ``MemoryEnvironment.to_dict()`` payload from coop config knobs.

    Returns ``None`` when no profile is configured (engine default:
    ``assumed-ordinary-ram``). Bounded / hardware profiles with empty ranges
    are still returned so the engine can fail closed rather than silently
    degrading.
    """
    if config.memory_profile is None:
        return None
    from tools.ppc_equivalence.memory_profile import (
        MemoryEnvironment,
        MemoryProfile,
        parse_ranges,
    )

    ranges = parse_ranges(list(config.memory_ranges)) if config.memory_ranges else []
    env = MemoryEnvironment(
        profile=MemoryProfile(config.memory_profile),
        ranges=ranges,
    )
    return env.to_dict()


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
