from __future__ import annotations

import json
import re
from dataclasses import dataclass, field
from datetime import datetime, timezone
from pathlib import Path
from typing import Any, Dict, Iterable, List, Optional

from tools.coop.lib.config import CoopConfig, _load_yaml
from tools.symbolrecover.lib.mwcc import demangle_symbol
from tools.symbolrecover.lib.parser import SymbolEntry, load_symbols


MATCH_STATUSES = {
    "NOT_STARTED",
    "COMPILES",
    "STRUCTURAL",
    "HIGH_MATCH",
    "CODE_MATCH",
    "EQUIVALENT_MATCH",
    "FULL_MATCH",
    "BEHAVIOR_VERIFIED",
}

WORKFLOW_STATUSES = {
    "BACKLOG",
    "DISCOVERY",
    "QUEUED",
    "CLAIMED",
    "ACTIVE",
    "ACCEPTED",
    "BLOCKED",
    "NOT_REQUIRED",
}


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
    region: Optional[str] = None
    kind: str = "function"
    workflow_status: str = "BACKLOG"
    notes: str = ""
    status: str = "NOT_STARTED"
    extra: Dict[str, Any] = field(default_factory=dict)

    @property
    def buildable(self) -> bool:
        return self.source is not None and self.unit is not None and self.kind == "function"


def targets_path(config: CoopConfig) -> Path:
    return config.resolve(config.targets_file)


def load_targets_document(config: CoopConfig) -> Dict[str, Any]:
    path = targets_path(config)
    if not path.is_file():
        raise FileNotFoundError(f"Targets file not found: {path}")
    if path.suffix in {".yaml", ".yml"}:
        return _load_yaml(path)
    with path.open(encoding="utf-8") as f:
        return json.load(f)


def write_targets_document(config: CoopConfig, data: Dict[str, Any]) -> Path:
    path = targets_path(config)
    if path.suffix in {".yaml", ".yml"}:
        raise ValueError("Symbol import currently writes JSON target registries only")
    path.write_text(json.dumps(data, indent=2, ensure_ascii=False) + "\n", encoding="utf-8")
    return path


def update_target_result(
    config: CoopConfig,
    target_id: str,
    *,
    status: str,
    instruction_match: Optional[float],
    equivalence_status: Optional[str] = None,
) -> Path:
    """Persist the latest result so the registry remains current state."""
    data = load_targets_document(config)
    for row in data.get("targets", []):
        if row.get("id") != target_id:
            continue
        row["status"] = status
        if instruction_match is not None:
            row["instruction_match"] = round(float(instruction_match), 3)
        if equivalence_status:
            row["equivalence_status"] = equivalence_status
        if status in {"FULL_MATCH", "EQUIVALENT_MATCH"}:
            row["workflow_status"] = "ACCEPTED"
        elif row.get("workflow_status") in {None, "BACKLOG", "QUEUED", "CLAIMED"}:
            row["workflow_status"] = "ACTIVE"
        return write_targets_document(config, data)
    raise KeyError(f"Unknown target id: {target_id}")


def sync_results_from_attempts(config: CoopConfig, attempts: Iterable[Dict[str, Any]]) -> int:
    """Migrate latest append-only attempt results into the canonical registry."""
    latest: Dict[str, Dict[str, Any]] = {}
    for attempt in attempts:
        target_id = attempt.get("target_id")
        if target_id:
            latest[str(target_id)] = attempt
    data = load_targets_document(config)
    changed = 0
    for row in data.get("targets", []):
        attempt = latest.get(str(row.get("id")))
        if not attempt:
            continue
        before = (
            row.get("status"), row.get("workflow_status"), row.get("instruction_match"),
            row.get("equivalence_status"),
        )
        status = str(attempt.get("status", row.get("status", "NOT_STARTED")))
        row["status"] = status
        if attempt.get("instruction_match") is not None:
            row["instruction_match"] = round(float(attempt["instruction_match"]), 3)
        if attempt.get("equivalence_status"):
            row["equivalence_status"] = attempt["equivalence_status"]
        if status in {"FULL_MATCH", "EQUIVALENT_MATCH"}:
            row["workflow_status"] = "ACCEPTED"
        elif row.get("workflow_status") in {None, "BACKLOG", "QUEUED", "CLAIMED"}:
            row["workflow_status"] = "ACTIVE"
        after = (
            row.get("status"), row.get("workflow_status"), row.get("instruction_match"),
            row.get("equivalence_status"),
        )
        changed += before != after
    if changed:
        write_targets_document(config, data)
    return changed


def claim_target(
    config: CoopConfig,
    target_id: str,
    *,
    owner: str,
    allowed_paths: List[str],
    note: str = "",
) -> Path:
    data = load_targets_document(config)
    for row in data.get("targets", []):
        if row.get("id") != target_id:
            continue
        claim = row.get("claim")
        if isinstance(claim, dict) and claim.get("owner") and claim.get("owner") != owner:
            raise ValueError(
                f"target {target_id!r} is already claimed by {claim['owner']!r}; release it first"
            )
        row["claim"] = {
            "owner": owner,
            "claimed_at": datetime.now(timezone.utc).isoformat(),
            "allowed_paths": allowed_paths,
            "note": note,
        }
        if row.get("workflow_status") not in {"ACCEPTED", "NOT_REQUIRED"}:
            row["workflow_status"] = "CLAIMED"
        return write_targets_document(config, data)
    raise KeyError(f"Unknown target id: {target_id}")


def release_target(config: CoopConfig, target_id: str, *, owner: Optional[str]) -> Path:
    data = load_targets_document(config)
    for row in data.get("targets", []):
        if row.get("id") != target_id:
            continue
        claim = row.get("claim")
        if not isinstance(claim, dict):
            raise ValueError(f"target {target_id!r} is not claimed")
        current_owner = claim.get("owner")
        if owner and current_owner != owner:
            raise ValueError(
                f"target {target_id!r} is claimed by {current_owner!r}, not {owner!r}"
            )
        row.pop("claim", None)
        if row.get("workflow_status") == "CLAIMED":
            row["workflow_status"] = (
                "ACCEPTED"
                if row.get("status") in {"FULL_MATCH", "EQUIVALENT_MATCH"}
                else "BACKLOG"
            )
        return write_targets_document(config, data)
    raise KeyError(f"Unknown target id: {target_id}")


def load_targets(config: CoopConfig) -> List[Target]:
    data = load_targets_document(config)
    targets: List[Target] = []
    known = {
        "id", "tier", "priority", "milestone", "function", "symbol", "address",
        "source", "unit", "required_level", "region", "kind", "workflow_status",
        "tracking", "notes", "status",
    }
    for item in data.get("targets", []):
        source = item.get("source")
        targets.append(
            Target(
                id=item["id"],
                tier=item.get("tier", item.get("priority", "P9")),
                milestone=item.get("milestone", "unassigned"),
                function=item.get("function", item.get("symbol", item["id"])),
                symbol=item.get("symbol"),
                address=item.get("address"),
                source=config.resolve(source) if source else None,
                unit=item.get("unit"),
                required_level=item.get(
                    "required_level",
                    data.get("default_required_level", config.default_required_level),
                ),
                region=item.get("region", config.region),
                kind=item.get("kind", "function"),
                workflow_status=item.get("workflow_status", item.get("tracking", "BACKLOG")),
                notes=item.get("notes", ""),
                status=item.get("status", "NOT_STARTED"),
                extra={key: value for key, value in item.items() if key not in known},
            )
        )
    return targets


def get_target(targets: List[Target], target_id: str) -> Target:
    for target in targets:
        if target.id == target_id:
            return target
    raise KeyError(f"Unknown target id: {target_id}")


def pending_targets(targets: List[Target], tier: Optional[str] = None) -> List[Target]:
    done = {"FULL_MATCH", "EQUIVALENT_MATCH"}
    closed = {"ACCEPTED", "NOT_REQUIRED"}
    result = [
        target for target in targets
        if target.buildable
        and target.extra.get("origin") != "symbols.txt"
        and target.status not in done
        and target.workflow_status not in closed
    ]
    if tier:
        result = [target for target in result if target.tier == tier]
    tier_order = {"P0": 0, "P1": 1, "P2": 2, "P3": 3}
    result.sort(key=lambda target: (tier_order.get(target.tier, 99), target.id))
    return result


def validate_targets(config: CoopConfig) -> List[str]:
    data = load_targets_document(config)
    errors: List[str] = []
    if not isinstance(data.get("schema_version"), int):
        errors.append("top-level schema_version must be an integer")
    rows = data.get("targets")
    if not isinstance(rows, list):
        return [*errors, "top-level targets must be an array"]

    ids: Dict[str, int] = {}
    identities: Dict[tuple[str, str, Optional[str]], int] = {}
    for index, row in enumerate(rows, start=1):
        label = f"targets[{index - 1}]"
        if not isinstance(row, dict):
            errors.append(f"{label} must be an object")
            continue
        for field_name in ("id", "symbol", "address"):
            if field_name not in row:
                errors.append(f"{label} is missing {field_name}")
        target_id = row.get("id")
        if isinstance(target_id, str):
            if target_id in ids:
                errors.append(f"duplicate id {target_id!r} at rows {ids[target_id]} and {index}")
            ids[target_id] = index
        region = str(row.get("region", config.region))
        symbol = row.get("symbol")
        if isinstance(symbol, str):
            identity = (region, symbol, row.get("address"))
            if identity in identities:
                errors.append(
                    f"duplicate symbol identity {region}:{symbol}@{row.get('address')} "
                    f"at rows {identities[identity]} and {index}"
                )
            identities[identity] = index
        status = row.get("status", "NOT_STARTED")
        if status not in MATCH_STATUSES:
            errors.append(f"{label} has unknown match status {status!r}")
        workflow = row.get("workflow_status", row.get("tracking", "BACKLOG"))
        if workflow not in WORKFLOW_STATUSES:
            errors.append(f"{label} has unknown workflow_status {workflow!r}")
    return errors


@dataclass(frozen=True)
class SplitRange:
    unit: str
    section: str
    start: int
    end: int


_SPLIT_UNIT_RE = re.compile(r"^(?P<unit>[^\s].*):$")
_SPLIT_RANGE_RE = re.compile(
    r"^\s*(?P<section>\S+)\s+start:0x(?P<start>[0-9A-Fa-f]+)\s+end:0x(?P<end>[0-9A-Fa-f]+)"
)


def load_split_ranges(path: Path) -> List[SplitRange]:
    ranges: List[SplitRange] = []
    current_unit: Optional[str] = None
    for line in path.read_text(encoding="utf-8").splitlines():
        unit_match = _SPLIT_UNIT_RE.match(line)
        if unit_match and not line.startswith((" ", "\t")) and line != "Sections:":
            current_unit = unit_match.group("unit")
            continue
        range_match = _SPLIT_RANGE_RE.match(line)
        if current_unit and range_match:
            ranges.append(
                SplitRange(
                    unit=current_unit,
                    section=range_match.group("section"),
                    start=int(range_match.group("start"), 16),
                    end=int(range_match.group("end"), 16),
                )
            )
    return ranges


def _unit_for_symbol(symbol: SymbolEntry, ranges: Iterable[SplitRange]) -> Optional[str]:
    for item in ranges:
        if item.section == symbol.section and item.start <= symbol.address < item.end:
            unit = str(Path(item.unit).with_suffix(""))
            return unit
    return None


def _display_name(symbol: str) -> str:
    info = demangle_symbol(symbol)
    function = info.function
    if info.is_ctor and info.class_name:
        function = info.class_name
    elif info.is_dtor and info.class_name:
        function = f"~{info.class_name}"
    parts = [part for part in (info.namespace, info.class_name, function) if part]
    if not parts or (len(parts) == 1 and parts[0] == symbol):
        return symbol
    return "::".join(parts) + (info.args or "")


def _source_maps(project: Any) -> tuple[Dict[str, str], Dict[str, str]]:
    source_by_unit: Dict[str, str] = {}
    objdiff_by_unit: Dict[str, str] = {}
    try:
        units = project.load_objdiff_units()
    except FileNotFoundError:
        return source_by_unit, objdiff_by_unit
    for unit in units:
        canonical = unit.name.removeprefix("main/")
        objdiff_by_unit[canonical] = canonical
        if unit.source_path:
            source_by_unit[canonical] = str(unit.source_path.relative_to(project.root))
    return source_by_unit, objdiff_by_unit


def import_symbols(
    project: Any,
    config: CoopConfig,
    *,
    kind: str = "function",
) -> tuple[Dict[str, Any], int, int]:
    """Return an updated registry plus (added, skipped) counts.

    Curated records are never replaced. Imported IDs are address-based so a later
    semantic rename does not create a second target.
    """
    data = load_targets_document(config)
    data.setdefault("schema_version", 2)
    rows = data.setdefault("targets", [])
    for row in rows:
        if row.get("origin") == "symbols.txt" and row.get("workflow_status") == "BACKLOG":
            row["workflow_status"] = "DISCOVERY"
    symbols_path = project.root / "config" / config.region / "symbols.txt"
    splits_path = project.root / "config" / config.region / "splits.txt"
    symbols = load_symbols(symbols_path)
    ranges = load_split_ranges(splits_path) if splits_path.is_file() else []
    source_by_unit, _ = _source_maps(project)

    existing_symbols: Dict[tuple[str, Any], set[Optional[str]]] = {}
    for row in rows:
        key = (str(row.get("region", config.region)), row.get("symbol"))
        existing_symbols.setdefault(key, set()).add(row.get("address"))
    existing_ids = {row["id"] for row in rows if isinstance(row, dict) and "id" in row}
    added = 0
    skipped = 0
    for entry in symbols:
        symbol_kind = entry.sym_type or "unknown"
        if kind != "all" and symbol_kind != kind:
            continue
        identity = (config.region, entry.name)
        known_addresses = existing_symbols.get(identity, set())
        if entry.address_hex in known_addresses or None in known_addresses:
            skipped += 1
            continue
        base_id = f"{config.region}-{entry.address:08x}"
        target_id = base_id
        suffix = 2
        while target_id in existing_ids:
            target_id = f"{base_id}-{suffix}"
            suffix += 1
        unit = _unit_for_symbol(entry, ranges)
        row: Dict[str, Any] = {
            "id": target_id,
            "region": config.region,
            "kind": symbol_kind,
            "tier": "P9",
            "milestone": "unassigned",
            "function": _display_name(entry.name),
            "symbol": entry.name,
            "address": entry.address_hex,
            "size": f"0x{entry.size:X}" if entry.size is not None else None,
            "unit": unit,
            "required_level": data.get("default_required_level", config.default_required_level),
            "workflow_status": "DISCOVERY" if symbol_kind == "function" else "NOT_REQUIRED",
            "status": "NOT_STARTED",
            "origin": "symbols.txt",
        }
        if unit and unit in source_by_unit:
            row["source"] = source_by_unit[unit]
        rows.append(row)
        existing_symbols.setdefault(identity, set()).add(entry.address_hex)
        existing_ids.add(target_id)
        added += 1
    rows.sort(key=lambda row: (str(row.get("region", config.region)), str(row.get("address") or ""), row["id"]))
    return data, added, skipped
