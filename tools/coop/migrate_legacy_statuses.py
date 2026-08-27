#!/usr/bin/env python3
"""One-shot migration: normalize freeform match statuses in the registry.

The registry accumulated ad-hoc ``status`` values (``OPEN_ITEM``,
``NEAR-MISS 74.3%``, ``cycle_fail_diagnostic``, ...) that violate the
``MATCH_STATUSES`` vocabulary and break ``targets validate`` / byte metrics.

Each offending row is re-mapped to the canonical status its recorded
``instruction_match`` already implies (the same ladder ``classify_status``
uses, capped at ``CODE_MATCH`` so the migration can never mint a new
accepted row), and the original string is preserved in ``legacy_status``.
``workflow_status`` is untouched.

Idempotent: rows with a canonical status are skipped.

    python3 tools/coop/migrate_legacy_statuses.py --dry-run
    python3 tools/coop/migrate_legacy_statuses.py
"""

from __future__ import annotations

import argparse
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2]))

from tools.coop.lib.config import load_config  # noqa: E402
from tools.coop.lib.targets import (  # noqa: E402
    MATCH_STATUSES,
    locked_targets_document,
    validate_targets,
)


def canonical_for_pct(pct) -> str:
    """Canonical status implied by a recorded instruction_match.

    Capped at CODE_MATCH: a row whose status is garbage cannot be promoted
    into the accepted tier (FULL/EQUIVALENT) by a schema migration.
    """
    if not isinstance(pct, (int, float)):
        return "NOT_STARTED"
    if pct >= 95.0:
        return "CODE_MATCH"
    if pct >= 70.0:
        return "HIGH_MATCH"
    if pct > 0.0:
        return "STRUCTURAL"
    return "COMPILES"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--dry-run", action="store_true", help="report only, change nothing")
    args = parser.parse_args()

    config = load_config(None, Path.cwd().resolve())

    plan: list[tuple[str, str, str]] = []
    with locked_targets_document(config) as (data, write):
        for row in data.get("targets", []):
            if not isinstance(row, dict):
                continue
            status = row.get("status", "NOT_STARTED")
            if status in MATCH_STATUSES:
                continue
            new_status = canonical_for_pct(row.get("instruction_match"))
            plan.append((row.get("id", "?"), status, new_status))
        if args.dry_run:
            for target_id, old, new in plan:
                print(f"would migrate {target_id}: {old!r} -> {new!r}")
            print(f"dry-run: {len(plan)} row(s) would migrate")
            return 0
        for row in data.get("targets", []):
            if not isinstance(row, dict):
                continue
            status = row.get("status", "NOT_STARTED")
            if status in MATCH_STATUSES:
                continue
            row["legacy_status"] = status
            row["status"] = canonical_for_pct(row.get("instruction_match"))
        path = write()
    for target_id, old, new in plan:
        print(f"migrated {target_id}: {old!r} -> {new!r} (preserved in legacy_status)")
    print(f"migrated {len(plan)} row(s); updated {path}")
    errors = validate_targets(config, check_certificates=False)
    status_errors = [error for error in errors if "unknown" in error]
    if status_errors:
        print(f"WARNING: {len(status_errors)} status error(s) remain", file=sys.stderr)
        for error in status_errors:
            print(f"  {error}", file=sys.stderr)
        return 1
    print("status vocabulary clean")
    return 0


if __name__ == "__main__":
    sys.exit(main())
