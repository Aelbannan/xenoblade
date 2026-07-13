from __future__ import annotations

import json
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any, Dict, List, Optional

from tools.coop.lib.config import CoopConfig, _load_yaml


@dataclass
class Target:
    id: str
    tier: str
    milestone: str
    function: str
    symbol: Optional[str]
    address: Optional[str]
    source: Optional[Path]
    unit: Optional[str]
    required_level: str
    notes: str = ""
    status: str = "NOT_STARTED"
    extra: Dict[str, Any] = field(default_factory=dict)

    @property
    def buildable(self) -> bool:
        return self.source is not None and self.unit is not None


def load_targets(config: CoopConfig) -> List[Target]:
    path = config.resolve(config.targets_file)
    if not path.is_file():
        raise FileNotFoundError(f"Targets file not found: {path}")

    if path.suffix in {".yaml", ".yml"}:
        data = _load_yaml(path)
    else:
        with path.open(encoding="utf-8") as f:
            data = json.load(f)

    targets: List[Target] = []
    for item in data.get("targets", []):
        source = item.get("source")
        targets.append(
            Target(
                id=item["id"],
                tier=item.get("tier", "P9"),
                milestone=item.get("milestone", "unknown"),
                function=item["function"],
                symbol=item.get("symbol"),
                address=item.get("address"),
                source=config.resolve(source) if source else None,
                unit=item.get("unit"),
                required_level=item.get(
                    "required_level",
                    data.get("default_required_level", config.default_required_level),
                ),
                notes=item.get("notes", ""),
                status=item.get("status", "NOT_STARTED"),
            )
        )
    return targets


def get_target(targets: List[Target], target_id: str) -> Target:
    for target in targets:
        if target.id == target_id:
            return target
    raise KeyError(f"Unknown target id: {target_id}")


def pending_targets(targets: List[Target], tier: Optional[str] = None) -> List[Target]:
    result = [t for t in targets if t.buildable and t.status != "FULL_MATCH"]
    if tier:
        result = [t for t in result if t.tier == tier]
    tier_order = {"P0": 0, "P1": 1, "P2": 2, "P3": 3}
    result.sort(key=lambda t: (tier_order.get(t.tier, 99), t.id))
    return result
