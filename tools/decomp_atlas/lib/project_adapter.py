"""Read-only adapter over coop config, targets, and objdiff units."""

from __future__ import annotations

import json
from pathlib import Path
from typing import Any, Dict, List, Optional

from tools.coop.lib.attempts import read_attempts
from tools.coop.lib.config import CoopConfig, load_config
from tools.coop.lib.project import ObjdiffUnit, Project
from tools.coop.lib.targets import (
    ACCEPTED_MATCH_STATUSES,
    Target,
    equivalence_certificate_error,
    load_targets,
)


def _rel(root: Path, path: Optional[Path]) -> Optional[str]:
    if path is None:
        return None
    try:
        return path.resolve().relative_to(root.resolve()).as_posix()
    except ValueError:
        return path.as_posix()


def parse_size_bytes(value: Any) -> Optional[int]:
    if value is None or value == "":
        return None
    if isinstance(value, bool):
        return None
    if isinstance(value, int):
        return value
    if isinstance(value, float):
        return int(value)
    text = str(value).strip()
    try:
        if text.lower().startswith("0x"):
            return int(text, 16)
        return int(text, 10)
    except ValueError:
        return None


class AtlasProjectAdapter:
    """Read-only view of a Xenoblade co-op project for Atlas indexing."""

    def __init__(self, root: Path | str, config_path: Optional[Path | str] = None) -> None:
        self.root = Path(root).resolve()
        cfg = Path(config_path) if config_path else None
        self.config: CoopConfig = load_config(cfg, self.root)
        self.project = Project(self.config)
        self._targets: Optional[List[Target]] = None
        self._unit_map: Optional[Dict[str, ObjdiffUnit]] = None
        self._rows_by_id: Optional[Dict[str, Dict[str, Any]]] = None

    @property
    def region(self) -> str:
        return self.config.region

    def load_targets(self) -> List[Target]:
        if self._targets is None:
            self._targets = load_targets(self.config)
        return self._targets

    def get_unit_map(self) -> Dict[str, ObjdiffUnit]:
        """Map unit hint / name / source path -> ObjdiffUnit (best effort)."""
        if self._unit_map is not None:
            return self._unit_map
        mapping: Dict[str, ObjdiffUnit] = {}
        try:
            units = self.project.load_objdiff_units()
        except FileNotFoundError:
            self._unit_map = mapping
            return mapping
        for unit in units:
            mapping[unit.name] = unit
            mapping[unit.name.removeprefix("main/")] = unit
            if unit.source_path is not None:
                rel = _rel(self.root, unit.source_path)
                if rel:
                    mapping[rel] = unit
                    mapping[Path(rel).stem] = unit
        self._unit_map = mapping
        return mapping

    def resolve_unit(self, target: Target) -> Optional[ObjdiffUnit]:
        if not target.unit:
            return None
        units = self.get_unit_map()
        hint = target.unit.replace("\\", "/")
        if hint in units:
            return units[hint]
        for key, unit in units.items():
            if key.endswith("/" + hint) or key.endswith(hint):
                return unit
        try:
            return self.project.resolve_unit(hint)
        except (ValueError, FileNotFoundError):
            return None

    def _raw_rows_by_id(self) -> Dict[str, Dict[str, Any]]:
        if self._rows_by_id is not None:
            return self._rows_by_id
        from tools.coop.lib.targets import load_targets_document

        data = load_targets_document(self.config)
        rows: Dict[str, Dict[str, Any]] = {}
        for row in data.get("targets", []):
            if isinstance(row, dict) and row.get("id"):
                rows[str(row["id"])] = row
        self._rows_by_id = rows
        return rows

    def certified_ids(self) -> set[str]:
        rows = self._raw_rows_by_id()
        certified: set[str] = set()
        for target in self.load_targets():
            if target.status not in ACCEPTED_MATCH_STATUSES:
                continue
            row = rows.get(target.id) or {"id": target.id, **target.extra}
            if equivalence_certificate_error(row, rows) is None:
                certified.add(target.id)
        return certified

    def list_function_records(self) -> List[Dict[str, Any]]:
        """Return one Atlas function record dict per target (never mutates targets)."""
        records: List[Dict[str, Any]] = []
        for target in self.load_targets():
            unit = self.resolve_unit(target)
            claim = target.extra.get("claim")
            owner = None
            claimed_at = None
            if isinstance(claim, dict):
                owner = claim.get("owner")
                claimed_at = claim.get("claimed_at")
            called = target.extra.get("called_functions") or []
            unresolved = target.extra.get("unresolved_called_functions") or []
            abi_helpers = target.extra.get("abi_helper_calls") or []
            size = parse_size_bytes(target.extra.get("size"))
            if size is None and target.symbol:
                try:
                    entry = self.project.symbol_entry(target.symbol)
                except Exception:
                    entry = None
                if entry is not None and getattr(entry, "size", None):
                    size = int(entry.size)
            capabilities = target.extra.get("capabilities")
            if isinstance(capabilities, list):
                capabilities = json.dumps(capabilities)
            elif capabilities is not None:
                capabilities = str(capabilities)
            records.append(
                {
                    "target_id": target.id,
                    "region": target.region or self.config.region,
                    "kind": target.kind,
                    "display_name": target.function,
                    "symbol": target.symbol,
                    "address": target.address,
                    "size": size,
                    "source": _rel(self.root, target.source),
                    "unit": target.unit,
                    "target_object": _rel(self.root, unit.target_path) if unit else None,
                    "base_object": _rel(self.root, unit.base_path) if unit else None,
                    "tier": target.tier,
                    "milestone": target.milestone,
                    "capabilities": capabilities,
                    "required_level": target.required_level,
                    "workflow_status": target.workflow_status,
                    "status": target.status,
                    "instruction_percent": target.extra.get("instruction_match"),
                    "buildable": bool(target.buildable),
                    "owner": owner,
                    "claimed_at": claimed_at,
                    "origin": target.extra.get("origin"),
                    "notes": target.notes,
                    "called_functions": list(called) if isinstance(called, list) else [],
                    "unresolved_called_functions": (
                        list(unresolved) if isinstance(unresolved, list) else []
                    ),
                    "abi_helper_calls": list(abi_helpers) if isinstance(abi_helpers, list) else [],
                    "has_indirect_calls": bool(target.extra.get("has_indirect_calls", False)),
                    "callgraph_status": target.extra.get("callgraph_status"),
                    "claim": claim if isinstance(claim, dict) else None,
                    "equivalence_certificate": target.extra.get("equivalence_certificate"),
                }
            )
        return records

    def load_attempts(self) -> List[Dict[str, Any]]:
        return read_attempts(self.config.resolve(self.config.attempt_log))

    def load_experiments(self, path: Optional[Path | str] = None) -> List[Dict[str, Any]]:
        experiments_path = (
            Path(path)
            if path is not None
            else self.root / "build" / "llm-harness" / "experiments.jsonl"
        )
        if not experiments_path.is_file():
            return []
        rows: List[Dict[str, Any]] = []
        for line in experiments_path.read_text(encoding="utf-8").splitlines():
            line = line.strip()
            if not line:
                continue
            try:
                rows.append(json.loads(line))
            except json.JSONDecodeError:
                continue
        return rows

    def targets_path(self) -> Path:
        return self.config.resolve(self.config.targets_file)

    def objdiff_path(self) -> Path:
        return self.config.objdiff_json

    def attempts_path(self) -> Path:
        return self.config.resolve(self.config.attempt_log)

    def experiments_path(self) -> Path:
        return self.root / "build" / "llm-harness" / "experiments.jsonl"
