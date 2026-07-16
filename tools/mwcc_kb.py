#!/usr/bin/env python3
"""Searchable local knowledge base for MWCC matching work."""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from tools.coop.lib.mwcc_knowledge import (
    TAG_PATTERNS,
    build_database,
    connect,
    database_is_fresh,
    get_entry,
    search,
    stats,
)


DEFAULT_DATABASE = ROOT / "build" / "mwcc_knowledge.sqlite"
DEFAULT_REFERENCE = ROOT / "docs" / "MWCC_REFERENCE.md"
DEFAULT_ATTEMPTS = ROOT / "docs" / "evidence" / "decomp" / "attempts.jsonl"


def _ensure_database(args: argparse.Namespace) -> None:
    sources = [args.reference, args.attempts]
    if args.rebuild or not database_is_fresh(args.database, sources):
        count = build_database(
            args.database,
            args.reference,
            args.attempts,
            root=ROOT,
        )
        print(
            f"indexed {count} entries -> {args.database.relative_to(ROOT)}",
            file=sys.stderr,
        )


def _print_result(row: object) -> None:
    percent = row["match_percent"]
    evidence = " ".join(
        item for item in (row["source_kind"], row["status"], f"{percent:g}%" if percent is not None else "") if item
    )
    print(f"{row['id']}  [{evidence}]")
    print(f"  {row['title']}")
    print(f"  {row['source_path']}:{row['line_start']}  tags: {row['tags'] or '-'}")
    snippet = " ".join((row["snippet"] or "").split())
    if snippet:
        print(f"  {snippet}")


def _search_result_json(row: object) -> dict[str, object]:
    return {
        "id": row["id"],
        "source_kind": row["source_kind"],
        "source_path": row["source_path"],
        "line_start": row["line_start"],
        "line_end": row["line_end"],
        "section": row["section"],
        "title": row["title"],
        "status": row["status"] or None,
        "match_percent": row["match_percent"],
        "target_id": row["target_id"] or None,
        "symbol": row["symbol"] or None,
        "unit": row["unit"] or None,
        "tags": row["tags"].split(),
        "rank": row["rank"],
        "snippet": " ".join((row["snippet"] or "").split()),
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--database", type=Path, default=DEFAULT_DATABASE)
    parser.add_argument("--reference", type=Path, default=DEFAULT_REFERENCE)
    parser.add_argument("--attempts", type=Path, default=DEFAULT_ATTEMPTS)
    parser.add_argument("--rebuild", action="store_true", help="Rebuild even when the index is fresh")
    sub = parser.add_subparsers(dest="command", required=True)

    sub.add_parser("build", help="Rebuild the generated SQLite index")
    search_parser = sub.add_parser("search", help="Full-text search reference patterns and attempts")
    search_parser.add_argument("query")
    search_parser.add_argument("--kind", choices=["reference", "attempt"], default="")
    search_parser.add_argument("--status", default="")
    search_parser.add_argument("--tag", choices=sorted(TAG_PATTERNS), default="")
    search_parser.add_argument(
        "--mode",
        choices=["all", "any"],
        default="all",
        help="Require every query term (default) or match any term for broader recall",
    )
    search_parser.add_argument("--limit", type=int, default=10)
    search_parser.add_argument("--json", action="store_true", help="Emit machine-readable results")
    show_parser = sub.add_parser("show", help="Show one result by its stable entry id")
    show_parser.add_argument("entry_id")
    show_parser.add_argument("--json", action="store_true", help="Emit one machine-readable record")
    sub.add_parser("stats", help="Show indexed entry and taxonomy counts")
    sub.add_parser("tags", help="List the inferred taxonomy")

    args = parser.parse_args()
    if args.command == "tags":
        print("\n".join(sorted(TAG_PATTERNS)))
        return 0
    _ensure_database(args)
    if args.command == "build":
        return 0

    with connect(args.database) as connection:
        if args.command == "search":
            if not args.query.strip():
                parser.error("search query cannot be empty")
            rows = search(
                connection,
                args.query,
                source_kind=args.kind,
                status=args.status,
                tag=args.tag,
                mode=args.mode,
                limit=args.limit,
            )
            if args.json:
                print(
                    json.dumps(
                        {
                            "query": args.query,
                            "mode": args.mode,
                            "count": len(rows),
                            "results": [_search_result_json(row) for row in rows],
                        },
                        indent=2,
                        ensure_ascii=False,
                    )
                )
            else:
                for index, row in enumerate(rows):
                    if index:
                        print()
                    _print_result(row)
            return 0 if rows else 1
        if args.command == "show":
            row = get_entry(connection, args.entry_id)
            if row is None:
                print(f"unknown entry: {args.entry_id}")
                return 1
            record = {key: row[key] for key in row.keys()}
            record["tags"] = record["tags"].split()
            if args.json:
                print(json.dumps(record, indent=2, ensure_ascii=False))
            else:
                body = record.pop("body")
                print(json.dumps(record, indent=2, ensure_ascii=False))
                print("\n" + body)
            return 0
        if args.command == "stats":
            print(json.dumps(stats(connection), indent=2, sort_keys=True))
            return 0
    return 2


if __name__ == "__main__":
    raise SystemExit(main())
