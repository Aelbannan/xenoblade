#!/usr/bin/env python3
"""Mark targets whose source already reflects decomp progress.

Two modes:

1. Default — bump NOT_STARTED rows with non-placeholder bodies to COMPILES.
2. ``--byte-identical`` — bump stub/placeholder rows whose built decomp object
   already byte-matches retail to FULL_MATCH (empty ``Draw() {}`` vs ``blr``, etc.).

Usage:
  python3 tools/coop/mark_implemented_bodies.py              # dry-run
  python3 tools/coop/mark_implemented_bodies.py --write
  python3 tools/coop/mark_implemented_bodies.py --byte-identical --write
  python3 tools/coop/mark_implemented_bodies.py --write --status STRUCTURAL
  python3 tools/coop/mark_implemented_bodies.py --tu kyoshin/CGame
"""
from __future__ import annotations

import argparse
import sys
from pathlib import Path
from typing import Any, Dict, List, Optional, Tuple

ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from tools.coop.lib.config import load_config
from tools.coop.lib.project import Project
from tools.coop.lib.targets import (
    ACCEPTED_MATCH_STATUSES,
    MATCH_STATUSES,
    load_targets_document,
    write_targets_document,
)
from tools.llm_harness.promotion import PlaceholderDetector
from tools.llm_harness.source_regions import find_function_region
from tools.ppc_equivalence.elf_symbols import extract_function


class _RowTarget:
    """Minimal target-like object for find_function_region."""

    def __init__(self, row: Dict[str, Any]) -> None:
        self.id = str(row.get("id", ""))
        self.function = str(row.get("function") or row.get("demangled") or "")
        self.symbol = row.get("symbol")
        self.unit = row.get("unit")


def _unit_matches(unit: Optional[str], needle: str) -> bool:
    if not unit:
        return False
    u = unit.removeprefix("main/")
    n = needle.removeprefix("main/")
    return u == n or u.endswith("/" + n) or n.endswith("/" + u)


def classify_body(body: str, detector: PlaceholderDetector) -> str:
    stripped = body.strip()
    if not stripped:
        return "empty"
    if detector.is_placeholder(stripped):
        return "placeholder"
    return "implemented"


def collect_updates(
    data: Dict[str, Any],
    *,
    from_status: str,
    to_status: str,
    tu: Optional[str],
    root: Path,
) -> Tuple[List[Dict[str, Any]], Dict[str, int]]:
    detector = PlaceholderDetector()
    source_cache: Dict[Path, str] = {}
    updates: List[Dict[str, Any]] = []
    counts = {
        "scanned": 0,
        "missing_source": 0,
        "unreadable": 0,
        "no_region": 0,
        "empty": 0,
        "placeholder": 0,
        "implemented": 0,
        "already_target_status": 0,
    }

    for row in data.get("targets", []):
        if row.get("status", "NOT_STARTED") != from_status:
            continue
        if tu is not None and not _unit_matches(row.get("unit"), tu):
            continue
        counts["scanned"] += 1

        source_rel = row.get("source")
        if not source_rel:
            counts["missing_source"] += 1
            continue
        source_path = root / source_rel if not Path(source_rel).is_absolute() else Path(source_rel)
        if not source_path.is_file():
            counts["missing_source"] += 1
            continue

        text = source_cache.get(source_path)
        if text is None:
            try:
                text = source_path.read_text(encoding="utf-8")
            except OSError:
                counts["unreadable"] += 1
                continue
            source_cache[source_path] = text

        try:
            region = find_function_region(text, _RowTarget(row))
        except (OSError, ValueError):
            counts["no_region"] += 1
            continue

        body = text[region.content_start : region.content_end]
        kind = classify_body(body, detector)
        counts[kind] = counts.get(kind, 0) + 1
        if kind != "implemented":
            continue
        if to_status == from_status:
            counts["already_target_status"] += 1
            continue

        updates.append(
            {
                "id": row.get("id"),
                "function": row.get("function"),
                "unit": row.get("unit"),
                "source": source_rel,
                "from_status": from_status,
                "to_status": to_status,
                "body_preview": " ".join(body.strip().split())[:80],
                "instruction_match": None,
            }
        )

    return updates, counts


def collect_byte_identical(
    data: Dict[str, Any],
    *,
    from_statuses: set[str],
    tu: Optional[str],
    root: Path,
    project: Project,
) -> Tuple[List[Dict[str, Any]], Dict[str, int]]:
    """Placeholder/empty bodies whose built decomp already matches retail bytes."""
    detector = PlaceholderDetector()
    source_cache: Dict[Path, str] = {}
    updates: List[Dict[str, Any]] = []
    counts = {
        "scanned": 0,
        "missing_source": 0,
        "unreadable": 0,
        "no_region": 0,
        "empty": 0,
        "placeholder": 0,
        "implemented": 0,
        "missing_objects": 0,
        "extract_error": 0,
        "mismatch": 0,
        "identical": 0,
    }

    for row in data.get("targets", []):
        status = row.get("status", "NOT_STARTED")
        if status not in from_statuses:
            continue
        if status in ACCEPTED_MATCH_STATUSES:
            continue
        if tu is not None and not _unit_matches(row.get("unit"), tu):
            continue
        symbol = row.get("symbol")
        unit_name = row.get("unit")
        if not symbol or not unit_name:
            continue
        counts["scanned"] += 1

        source_rel = row.get("source")
        if not source_rel:
            counts["missing_source"] += 1
            continue
        source_path = root / source_rel if not Path(source_rel).is_absolute() else Path(source_rel)
        if not source_path.is_file():
            counts["missing_source"] += 1
            continue

        text = source_cache.get(source_path)
        if text is None:
            try:
                text = source_path.read_text(encoding="utf-8")
            except OSError:
                counts["unreadable"] += 1
                continue
            source_cache[source_path] = text

        try:
            region = find_function_region(text, _RowTarget(row))
        except (OSError, ValueError):
            counts["no_region"] += 1
            continue

        body = text[region.content_start : region.content_end]
        kind = classify_body(body, detector)
        counts[kind] = counts.get(kind, 0) + 1

        try:
            unit = project.resolve_unit(str(unit_name))
        except (OSError, ValueError):
            counts["missing_objects"] += 1
            continue
        if (
            unit.target_path is None
            or not unit.target_path.is_file()
            or unit.base_path is None
            or not unit.base_path.is_file()
        ):
            counts["missing_objects"] += 1
            continue

        try:
            retail = extract_function(unit.target_path, str(symbol))
            decomp = extract_function(unit.base_path, str(symbol))
        except (OSError, ValueError):
            counts["extract_error"] += 1
            continue

        if retail.code != decomp.code:
            counts["mismatch"] += 1
            continue

        counts["identical"] += 1
        updates.append(
            {
                "id": row.get("id"),
                "function": row.get("function"),
                "unit": row.get("unit"),
                "source": source_rel,
                "from_status": status,
                "to_status": "FULL_MATCH",
                "body_preview": " ".join(body.strip().split())[:80],
                "instruction_match": 100.0,
            }
        )

    return updates, counts


def apply_updates(
    data: Dict[str, Any],
    updates: List[Dict[str, Any]],
    *,
    to_status: str,
    note: str,
) -> int:
    by_id = {u["id"]: u for u in updates}
    changed = 0
    for row in data.get("targets", []):
        tid = row.get("id")
        info = by_id.get(tid)
        if info is None:
            continue
        status = info.get("to_status", to_status)
        row["status"] = status
        if info.get("instruction_match") is not None:
            row["instruction_match"] = float(info["instruction_match"])
        if status in ACCEPTED_MATCH_STATUSES:
            row["workflow_status"] = "ACCEPTED"
        else:
            workflow = row.get("workflow_status", row.get("tracking", "BACKLOG"))
            if workflow in {None, "BACKLOG", "DISCOVERY", "QUEUED", "CLAIMED"}:
                row["workflow_status"] = "ACTIVE"
        if note:
            existing = (row.get("notes") or "").strip()
            if note not in existing:
                row["notes"] = f"{existing}; {note}".strip("; ").strip()
        changed += 1
    return changed


def main(argv: Optional[List[str]] = None) -> int:
    parser = argparse.ArgumentParser(
        description=(
            "Bump NOT_STARTED targets with non-placeholder bodies to COMPILES, "
            "or mark byte-identical stubs as FULL_MATCH."
        )
    )
    parser.add_argument(
        "--config",
        type=Path,
        default=None,
        help="Path to coop.json (default: discover from repo root)",
    )
    parser.add_argument(
        "--from-status",
        default="NOT_STARTED",
        choices=sorted(MATCH_STATUSES),
        help="Only rewrite rows currently at this status (default: NOT_STARTED)",
    )
    parser.add_argument(
        "--status",
        default="COMPILES",
        choices=sorted(
            MATCH_STATUSES
            - {"NOT_STARTED", "FULL_MATCH", "EQUIVALENT_MATCH", "BEHAVIOR_VERIFIED"}
        ),
        help="New match status for implemented bodies (default: COMPILES)",
    )
    parser.add_argument(
        "--byte-identical",
        action="store_true",
        help=(
            "Mark any non-accepted target whose built decomp already byte-matches "
            "retail as FULL_MATCH"
        ),
    )
    parser.add_argument(
        "--tu",
        default=None,
        help="Restrict to one translation unit (e.g. kyoshin/CGame)",
    )
    parser.add_argument(
        "--write",
        action="store_true",
        help="Persist changes to tools/coop/targets.json (otherwise dry-run)",
    )
    parser.add_argument(
        "--limit",
        type=int,
        default=0,
        help="Only report/update the first N matches (0 = all)",
    )
    parser.add_argument(
        "--note",
        default="",
        help="Note appended to updated rows (empty = mode default)",
    )
    args = parser.parse_args(argv)

    if (
        not args.byte_identical
        and args.status in {"FULL_MATCH", "EQUIVALENT_MATCH", "BEHAVIOR_VERIFIED"}
    ):
        print(
            "Refusing acceptance-tier statuses without objdiff/equivalence evidence. "
            "Use COMPILES / STRUCTURAL / HIGH_MATCH / CODE_MATCH, or --byte-identical.",
            file=sys.stderr,
        )
        return 2

    config = load_config(args.config, ROOT)
    data = load_targets_document(config)

    if args.byte_identical:
        project = Project(config)
        from_statuses = {args.from_status}
        # Also catch stubs still sitting at COMPILES after a prior body pass.
        if args.from_status == "NOT_STARTED":
            from_statuses.add("COMPILES")
        updates, counts = collect_byte_identical(
            data,
            from_statuses=from_statuses,
            tu=args.tu,
            root=config.project_root,
            project=project,
        )
        to_status = "FULL_MATCH"
        note = args.note or (
            "decomp object byte-identical to retail; marked via "
            "mark_implemented_bodies.py --byte-identical"
        )
        print(
            f"scanned={counts['scanned']} identical={counts['identical']} "
            f"mismatch={counts['mismatch']} extract_error={counts['extract_error']} "
            f"missing_objects={counts['missing_objects']} "
            f"implemented={counts['implemented']} placeholder={counts['placeholder']} "
            f"empty={counts['empty']} no_region={counts['no_region']} "
            f"missing_source={counts['missing_source']}"
        )
    else:
        updates, counts = collect_updates(
            data,
            from_status=args.from_status,
            to_status=args.status,
            tu=args.tu,
            root=config.project_root,
        )
        to_status = args.status
        note = args.note or (
            "non-placeholder body present; marked via mark_implemented_bodies.py"
        )
        print(
            f"scanned={counts['scanned']} "
            f"implemented={counts['implemented']} "
            f"placeholder={counts['placeholder']} "
            f"empty={counts['empty']} "
            f"missing_source={counts['missing_source']} "
            f"no_region={counts['no_region']} "
            f"unreadable={counts['unreadable']}"
        )

    if args.limit > 0:
        updates = updates[: args.limit]

    print(f"would_update={len(updates)} -> {to_status}")
    for item in updates[:20]:
        print(
            f"  {item['id']}: {item['function']} "
            f"[{item['unit']}] {item['from_status']} {item['body_preview']!r}"
        )
    if len(updates) > 20:
        print(f"  ... and {len(updates) - 20} more")

    if not args.write:
        print("dry-run only; pass --write to persist")
        return 0

    changed = apply_updates(data, updates, to_status=to_status, note=note)
    path = write_targets_document(config, data)
    print(f"wrote {changed} updates to {path}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
