#!/usr/bin/env python3
"""Deduplicate registry targets that share a (unit, address).

A symbol-import pass created duplicate records keyed by address instead of
updating existing rows: for every duplicated address there is an original id
(`us-XXXXXXXX`) and a re-imported `us-XXXXXXXX-2` with the same address/size.
The `-2` copy usually carries the richer mangled symbol name but zero work
(``NOT_STARTED`` / ``DISCOVERY``), while the original holds the accepted state
(``FULL_MATCH`` / ``ACCEPTED``). The harness grinds the phantom `-2`` rows as
if they were new targets, wasting waves on byte-identical functions.

For each duplicate group this script:
  * skips groups with a live claim (the running harness owns them — re-run
    after the run stops to finish those),
  * keeps the record with the most work (status + workflow priority),
  * adopts the mangled symbol name if the kept record only has a ``func_``
    placeholder,
  * migrates a sidecar certificate from a dropped id to the kept id when the
    kept id has none,
  * drops the remaining records from targets.json and the cert sidecar.

Never embeds sidecar certs into targets.json (load_targets_document merges
them into rows; the r6 sweep regression ballooned the file 6x that way).
"""

from __future__ import annotations

import argparse
import gzip
import json
import re
import sys
from collections import defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2]))

from tools.coop.lib.config import load_config  # noqa: E402
from tools.coop.lib.targets import (  # noqa: E402
    certs_sidecar_path,
    exclusive_targets_lock,
    load_targets_document,
    targets_path,
)

# Status rank: higher = more work done. Used to pick the record to keep.
_STATUS_RANK = {
    "FULL_MATCH": 9,
    "EQUIVALENT_MATCH": 8,
    "CODE_MATCH": 6,
    "COMPILES": 4,
    "STRUCTURAL": 3,
    "NOT_STARTED": 1,
    "UNMATCHED": 1,
}
_WORKFLOW_RANK = {
    "ACCEPTED": 5,
    "MATCHING": 4,
    "BACKLOG": 3,
    "ACTIVE": 2,
    "CLAIMED": 2,
    "DISCOVERY": 1,
}
_FUNC_PLACEHOLDER = re.compile(r"^func_[0-9A-Fa-f]{8}$")


def _is_mangled(name: str) -> bool:
    return bool(name) and "__" in name and not _FUNC_PLACEHOLDER.match(name)


def _keep_score(row: dict) -> tuple[int, int, int]:
    """Higher = better record to keep. (status, workflow, id-suffix)."""
    status = _STATUS_RANK.get(row.get("status", ""), 0)
    workflow = _WORKFLOW_RANK.get(row.get("workflow_status", ""), 0)
    # Prefer the original id over a `-N` re-import when equally worked.
    suffix = 0 if re.search(r"-\d+$", str(row.get("id", ""))) else 1
    return (status, workflow, suffix)


def _load_sidecar(path: Path) -> dict[str, list]:
    """id -> [cert, ...] from the sidecar (list-valued: tolerate dupes)."""
    out: dict[str, list] = defaultdict(list)
    if not path.is_file():
        return out
    with gzip.open(path, "rt", encoding="utf-8") as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            try:
                pair = json.loads(line)
            except ValueError:
                continue
            if isinstance(pair, list) and len(pair) == 2 and isinstance(pair[0], str):
                out[pair[0]].append(pair[1])
    return out


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--dry-run", action="store_true", help="print the merge list, change nothing")
    ap.add_argument("--limit", type=int, default=0, help="max groups to process (0 = all)")
    args = ap.parse_args()

    config = load_config(None, Path.cwd().resolve())
    document = load_targets_document(config)
    rows = [r for r in document.get("targets", []) if isinstance(r, dict)]

    by_addr: dict[tuple[str, str], list[dict]] = defaultdict(list)
    for row in rows:
        unit = row.get("unit")
        addr = row.get("address")
        if unit and addr:
            by_addr[(unit, addr)].append(row)

    groups = {k: v for k, v in by_addr.items() if len(v) > 1}
    print(f"targets: {len(rows)}")
    print(f"duplicate groups: {len(groups)}  (extra records: {sum(len(v) - 1 for v in groups.values())})")

    if args.limit:
        groups = dict(list(groups.items())[: args.limit])

    skipped_claimed = 0
    plan: list[tuple[dict, list[dict]]] = []  # (keep, drop)
    for (unit, addr), recs in sorted(groups.items()):
        if any(r.get("claim") for r in recs):
            skipped_claimed += 1
            continue
        best = max(recs, key=_keep_score)
        drop = [r for r in recs if r is not best]
        plan.append((best, drop))

    print(f"skipped (live claim): {skipped_claimed}")
    print(f"would merge: {len(plan)} groups, dropping {sum(len(d) for _, d in plan)} records")

    if args.dry_run:
        for keep, drop in plan[:60]:
            print(f"  keep {keep.get('id')} [{keep.get('status')}/{keep.get('workflow_status')}] "
                  f"<- drop {[d.get('id') for d in drop]}")
        if len(plan) > 60:
            print(f"  ... and {len(plan) - 60} more")
        return 0

    with exclusive_targets_lock(config):
        document = load_targets_document(config)
        # Strip merged sidecar certs before writing (never embed sidecar data).
        for row in document.get("targets", []):
            if isinstance(row, dict):
                row.pop("equivalence_certificate", None)
        by_id = {str(r.get("id")): r for r in document.get("targets", []) if isinstance(r, dict)}

        dropped_ids: set[str] = set()
        adopted_symbols = 0
        for keep, drop in plan:
            kid = str(keep.get("id"))
            row = by_id.get(kid)
            if row is None:
                continue
            # Adopt a mangled symbol name if the kept record only has a placeholder.
            if _FUNC_PLACEHOLDER.match(str(row.get("symbol", ""))):
                for d in drop:
                    if _is_mangled(str(d.get("symbol"))):
                        row["symbol"] = d.get("symbol")
                        adopted_symbols += 1
                        break
            for d in drop:
                dropped_ids.add(str(d.get("id")))

        # Remove dropped records.
        kept_rows = [r for r in document.get("targets", []) if isinstance(r, dict) and str(r.get("id")) not in dropped_ids]
        document["targets"] = kept_rows

        with targets_path(config).open("w", encoding="utf-8") as f:
            json.dump(document, f, indent=2, ensure_ascii=False)
            f.write("\n")

        # Sidecar: migrate certs from dropped ids to their kept id when the kept
        # id has none; drop the rest of the dropped ids' entries.
        sidecar_path = certs_sidecar_path(targets_path(config))
        if sidecar_path.is_file():
            sidecar = _load_sidecar(sidecar_path)
            keep_id_of: dict[str, str] = {}
            for keep, drop in plan:
                for d in drop:
                    keep_id_of[str(d.get("id"))] = str(keep.get("id"))
            migrated = 0
            for did in dropped_ids:
                kid = keep_id_of.get(did)
                if kid is None:
                    continue
                for cert in sidecar.get(did, []):
                    if kid not in sidecar or not sidecar[kid]:
                        sidecar[kid].append(cert)
                        migrated += 1
                sidecar.pop(did, None)
            with gzip.open(sidecar_path, "wt", encoding="utf-8") as f:
                for tid, certs in sorted(sidecar.items()):
                    for cert in certs:
                        f.write(json.dumps([tid, cert]) + "\n")
            print(f"sidecar: migrated {migrated} cert(s), dropped {len(dropped_ids)} ids")

        print(f"merged {len(plan)} groups, dropped {len(dropped_ids)} records, adopted {adopted_symbols} mangled symbol(s)")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
