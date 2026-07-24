#!/usr/bin/env python3
"""
Prepare a batch of decompilation target metadata for the xenoblade_decomp_fanout workflow.

Light mode only — outputs just metadata (id, function, symbol, source, unit, called_functions).
Agents read assembly and source files directly.

Usage:
  python3 tools/prepare_decomp_batch.py [--limit N] [--status DISCOVERY] [--output work_batch.json]
  python3 tools/prepare_decomp_batch.py --summary  # quick count
"""
import argparse
import json
import os
import sys

PROJECT_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
TARGETS_FILE = os.path.join(PROJECT_ROOT, "tools", "coop", "targets.json")


def load_targets():
    try:
        with open(TARGETS_FILE, "r") as f:
            return json.load(f)["targets"]
    except (FileNotFoundError, KeyError, json.JSONDecodeError) as e:
        print(f"Error loading targets: {e}", file=sys.stderr)
        sys.exit(1)


def filter_targets(targets, statuses, limit):
    filtered = [t for t in targets if t.get("workflow_status") in statuses]
    filtered.sort(key=lambda t: (
        0 if t.get("workflow_status") == "BACKLOG" else 1,
        t.get("id", ""),
    ))
    return filtered[:limit] if limit else filtered


def prepare_item(target):
    return {
        "id": target["id"],
        "function": target.get("function", ""),
        "symbol": target.get("symbol", ""),
        "address": target.get("address", ""),
        "size": target.get("size", ""),
        "source": target.get("source", ""),
        "unit": target.get("unit", ""),
        "workflow_status": target.get("workflow_status", ""),
        "status": target.get("status", ""),
        "called_functions": target.get("called_functions", []),
    }


def main():
    parser = argparse.ArgumentParser(description="Prepare decompilation batch metadata")
    parser.add_argument(
        "--status", nargs="+",
        default=["DISCOVERY", "BACKLOG"],
        choices=["DISCOVERY", "BACKLOG"],
    )
    parser.add_argument("--limit", type=int, default=None)
    parser.add_argument("--output", "-o", default="work_batch.json")
    parser.add_argument("--summary", action="store_true", help="Print count summary only")

    args = parser.parse_args()

    targets = load_targets()
    filtered = filter_targets(targets, args.status, args.limit)

    items = [prepare_item(t) for t in filtered]

    if args.summary:
        print(json.dumps({"total": len(items), "statuses": args.status, "limit": args.limit}, indent=2))
        return

    try:
        with open(args.output, "w") as f:
            json.dump(items, f, indent=2)
    except OSError as e:
        print(f"Error writing {args.output}: {e}", file=sys.stderr)
        sys.exit(1)

    size_kb = len(json.dumps(items)) / 1024
    print(f"Written {len(items)} items to {args.output} ({size_kb:.1f} KB)", file=sys.stderr)


if __name__ == "__main__":
    main()
