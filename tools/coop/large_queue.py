#!/usr/bin/env python3
"""
Large-function queue — rank NOT_STARTED catalog functions >= 512 bytes by
readiness, splitting into tier A (ready to solve) and tier B (blocked).

Usage:
  python3 tools/coop/large_queue.py list [--top N]
  python3 tools/coop/large_queue.py json --out file.json
  python3 tools/coop/large_queue.py callees <target-id>
  python3 tools/coop/large_queue.py summary
"""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path
from typing import Any, Dict, List, Tuple

ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from tools.coop.lib.config import load_config
from tools.coop.lib.targets import Target, load_targets


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

def _parse_size(raw: Any) -> int:
    """Convert a size value (hex string or int) to an integer byte count."""
    if isinstance(raw, str) and raw.startswith("0x"):
        return int(raw, 16)
    if isinstance(raw, (int, float)):
        return int(raw)
    return 0


def _compute_readiness(target: Target, targets_by_id: Dict[str, Target]) -> float:
    """Fraction of called_functions whose target status is FULL_MATCH or EQUIVALENT_MATCH."""
    called = target.extra.get("called_functions") or []
    if not called:
        return 1.0
    matched = 0
    for cid in called:
        ct = targets_by_id.get(cid)
        if ct is not None and ct.status in ("FULL_MATCH", "EQUIVALENT_MATCH"):
            matched += 1
    return matched / len(called)


# ---------------------------------------------------------------------------
# Queue construction
# ---------------------------------------------------------------------------

QueueItem = Dict[str, Any]


def build_queue(targets: List[Target]) -> Tuple[List[QueueItem], List[QueueItem], List[QueueItem]]:
    """
    Build the ranked queue.

    Returns (all_ranked, tier_a, tier_b) where each item has keys:
      id, function, size (int), size_hex (str), readiness, tier,
      has_indirect_calls, num_unresolved, num_called, unit.
    """
    targets_by_id = {t.id: t for t in targets}

    # Filter: us- prefix, NOT_STARTED, size >= 512
    candidates: List[Tuple[Target, int]] = []
    for t in targets:
        if not t.id.startswith("us-"):
            continue
        if t.status != "NOT_STARTED":
            continue
        size = _parse_size(t.extra.get("size"))
        if size < 512:
            continue
        candidates.append((t, size))

    # Build result items
    results: List[QueueItem] = []
    for t, size in candidates:
        readiness = _compute_readiness(t, targets_by_id)
        unresolved = t.extra.get("unresolved_called_functions") or []
        indirect = bool(t.extra.get("has_indirect_calls", False))

        tier = "A"
        if readiness < 1.0 or unresolved or indirect:
            tier = "B"

        results.append({
            "id": t.id,
            "function": t.function,
            "size": size,
            "size_hex": t.extra.get("size", "0x0"),
            "readiness": readiness,
            "tier": tier,
            "has_indirect_calls": indirect,
            "num_unresolved": len(unresolved),
            "num_called": len(t.extra.get("called_functions") or []),
            "unit": t.unit or "",
        })

    # Sort: tier A by size descending; tier B by readiness desc, then size desc
    tier_a = [r for r in results if r["tier"] == "A"]
    tier_b = [r for r in results if r["tier"] == "B"]

    tier_a.sort(key=lambda r: (-r["size"], r["id"]))
    tier_b.sort(key=lambda r: (-r["readiness"], -r["size"], r["id"]))

    all_ranked = tier_a + tier_b
    return all_ranked, tier_a, tier_b


# ---------------------------------------------------------------------------
# Sub-commands
# ---------------------------------------------------------------------------

def cmd_list(all_ranked: List[QueueItem], top: int) -> None:
    """Print head of the queue as a formatted table."""
    if not all_ranked:
        print("(empty queue — no NOT_STARTED functions >= 512 bytes)")
        return
    header = f"{'Rank':>5}  {'Tier':>4}  {'Size':>8}  {'Readiness':>9}  {'Target ID':<20}  Function"
    sep = "-" * len(header)
    print(header)
    print(sep)
    for i, item in enumerate(all_ranked[:top]):
        readiness_str = f"{item['readiness']:.0%}" if item["readiness"] == 1.0 or item["readiness"] == 0.0 else f"{item['readiness']:.1%}"
        print(
            f"{i + 1:>5}  "
            f"{item['tier']:>4}  "
            f"{item['size']:>8}  "
            f"{readiness_str:>9}  "
            f"{item['id']:<20}  "
            f"{item['function']}"
        )
    print()
    print(f"Showing top {min(top, len(all_ranked))} of {len(all_ranked)} queued functions.")


def cmd_json(all_ranked: List[QueueItem], tier_a: List[QueueItem], tier_b: List[QueueItem], out_path: Path) -> None:
    """Write queue as JSON consumable by batch-cycle.py / scripting."""
    def _brief(item: QueueItem) -> Dict[str, Any]:
        return {
            "id": item["id"],
            "function": item["function"],
            "size": item["size"],
            "size_hex": item["size_hex"],
            "readiness": item["readiness"],
            "tier": item["tier"],
            "unit": item["unit"],
        }

    payload = {
        "meta": {
            "total_queued": len(all_ranked),
            "tier_a_count": len(tier_a),
            "tier_b_count": len(tier_b),
            "tier_a_bytes": sum(item["size"] for item in tier_a),
            "tier_b_bytes": sum(item["size"] for item in tier_b),
        },
        "tier_a": [_brief(item) for item in tier_a],
        "tier_b": [_brief(item) for item in tier_b],
        "all_ranked": [_brief(item) for item in all_ranked],
    }
    out_path.write_text(json.dumps(payload, indent=2) + "\n", encoding="utf-8")
    print(f"Wrote {len(all_ranked)} queue entries to {out_path}")


def cmd_callees(target_id: str, targets: List[Target]) -> None:
    """List NOT_STARTED callees of a given target (decompose-first list)."""
    targets_by_id = {t.id: t for t in targets}
    parent = targets_by_id.get(target_id)
    if parent is None:
        print(f"Error: target '{target_id}' not found in registry.", file=sys.stderr)
        sys.exit(1)

    called = parent.extra.get("called_functions") or []
    if not called:
        print(f"Target {target_id} ({parent.function}) has no called_functions.")
        return

    not_started_callees: List[QueueItem] = []
    for cid in called:
        ct = targets_by_id.get(cid)
        if ct is None:
            continue
        if ct.status != "NOT_STARTED":
            continue
        sz = _parse_size(ct.extra.get("size"))
        not_started_callees.append({
            "id": ct.id,
            "function": ct.function,
            "size": sz,
            "unit": ct.unit or "",
            "status": ct.status,
        })

    if not not_started_callees:
        print(f"All {len(called)} callees of {target_id} ({parent.function}) are already matched.")
        return

    not_started_callees.sort(key=lambda r: (-r["size"], r["id"]))

    header = f"{'Target ID':<22}  {'Size':>8}  {'Status':<16}  Unit / Function"
    print(f"NOT_STARTED callees of {target_id} ({parent.function}):")
    print(header)
    print("-" * len(header))
    for item in not_started_callees:
        print(
            f"{item['id']:<22}  "
            f"{item['size']:>8}  "
            f"{item['status']:<16}  "
            f"{item['unit']} / {item['function']}"
        )
    print(f"\n({len(not_started_callees)} of {len(called)} callees are NOT_STARTED)")


def cmd_summary(tier_a: List[QueueItem], tier_b: List[QueueItem]) -> None:
    """Print tier A/B counts and byte totals."""
    total = len(tier_a) + len(tier_b)
    a_bytes = sum(item["size"] for item in tier_a)
    b_bytes = sum(item["size"] for item in tier_b)
    total_bytes = a_bytes + b_bytes

    print("Large-function queue summary")
    print("=" * 40)
    print(f"{'':20s}  {'Count':>8}  {'Bytes':>12}")
    print("-" * 42)
    print(f"{'Tier A (ready)':20s}  {len(tier_a):>8}  {a_bytes:>12,}")
    print(f"{'Tier B (blocked)':20s}  {len(tier_b):>8}  {b_bytes:>12,}")
    print(f"{'Total':20s}  {total:>8}  {total_bytes:>12,}")
    print()
    if tier_a:
        a_pct = a_bytes / total_bytes * 100 if total_bytes else 0
        print(f"Tier A is {a_pct:.1f}% of total large bytes ({a_bytes:,} / {total_bytes:,}).")
    if tier_b:
        top_blockers = sorted(
            tier_b,
            key=lambda r: (-r["readiness"], -r["size"])
        )[:5]
        print()
        print("Top-5 tier-B items by readiness then size:")
        for item in top_blockers:
            reason = []
            if item["readiness"] < 1.0:
                reason.append(f"readiness={item['readiness']:.0%}")
            if item["num_unresolved"]:
                reason.append(f"{item['num_unresolved']} unresolved")
            if item["has_indirect_calls"]:
                reason.append("indirect_calls")
            print(f"  {item['id']:<20s}  {item['size']:>8}  {'; '.join(reason)}")


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------

def main(argv: List[str] | None = None) -> int:
    parser = argparse.ArgumentParser(
        description="Large-function queue — rank NOT_STARTED catalog functions >= 512 bytes by readiness.",
    )
    sub = parser.add_subparsers(dest="command", required=True)

    # --- list ---
    list_p = sub.add_parser("list", help="Print the top-N queue (default 50).")
    list_p.add_argument("--top", type=int, default=50, help="Number of entries to show (default: 50)")

    # --- json ---
    json_p = sub.add_parser("json", help="Emit queue as JSON.")
    json_p.add_argument("--out", required=True, type=Path, help="Output JSON file path")

    # --- callees ---
    callees_p = sub.add_parser("callees", help="List NOT_STARTED callees of a target (decompose-first list).")
    callees_p.add_argument("target_id", help="Target ID of the parent function")

    # --- summary ---
    sub.add_parser("summary", help="Tier A/B counts and byte totals.")

    args = parser.parse_args(argv)

    # Load targets
    config = load_config(Path("coop.json"), ROOT)
    targets = load_targets(config)
    all_ranked, tier_a, tier_b = build_queue(targets)

    # Dispatch
    if args.command == "list":
        cmd_list(all_ranked, args.top)
    elif args.command == "json":
        cmd_json(all_ranked, tier_a, tier_b, args.out)
    elif args.command == "callees":
        cmd_callees(args.target_id, targets)
    elif args.command == "summary":
        cmd_summary(tier_a, tier_b)
    else:
        parser.print_help()
        return 1

    return 0


if __name__ == "__main__":
    sys.exit(main())
