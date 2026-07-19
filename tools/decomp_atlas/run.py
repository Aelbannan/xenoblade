#!/usr/bin/env python3
"""CLI for Xenoblade Decomp Atlas."""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))


def cmd_status(args: argparse.Namespace) -> int:
    from tools.decomp_atlas.lib.config import db_path, index_state_path
    from tools.decomp_atlas.lib.project_adapter import AtlasProjectAdapter

    adapter = AtlasProjectAdapter(args.root, config_path=args.config)
    region = args.region or adapter.region
    state_file = index_state_path(args.root, region)
    database = db_path(args.root, region)
    payload = {
        "region": region,
        "db_exists": database.is_file(),
        "db_path": str(database),
        "index_state_exists": state_file.is_file(),
        "index_state_path": str(state_file),
        "targets_path": str(adapter.targets_path()),
        "objdiff_path": str(adapter.objdiff_path()),
    }
    if state_file.is_file():
        try:
            payload["index_state"] = json.loads(state_file.read_text(encoding="utf-8"))
        except json.JSONDecodeError as exc:
            payload["index_state_error"] = str(exc)
    print(json.dumps(payload, indent=2))
    return 0


def cmd_index(args: argparse.Namespace) -> int:
    from tools.decomp_atlas.lib.indexer import run_index

    result = run_index(
        args.root,
        full=bool(args.full),
        vectors=bool(args.vectors),
        region=args.region,
        config_path=args.config,
    )
    print(json.dumps(result, indent=2))
    return 0 if result.get("ok") else 1


def cmd_serve(args: argparse.Namespace) -> int:
    from tools.decomp_atlas.lib.server import serve

    serve(
        args.root,
        host=args.host,
        port=args.port,
        enable_jobs=bool(args.enable_jobs),
        region=args.region,
        config_path=args.config,
    )
    return 0


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="Xenoblade Decomp Atlas")
    common = argparse.ArgumentParser(add_help=False)
    common.add_argument(
        "--root",
        type=Path,
        default=ROOT,
        help="project root (default: repository root)",
    )
    common.add_argument("--config", type=Path, default=None, help="coop.json path")
    common.add_argument("--region", default=None, help="override region (default from coop config)")

    sub = parser.add_subparsers(dest="command", required=True)

    status = sub.add_parser("status", parents=[common], help="show index status")
    status.set_defaults(func=cmd_status)

    index = sub.add_parser("index", parents=[common], help="build or refresh the Atlas SQLite index")
    index.add_argument("--full", action="store_true", help="extract per-function artifacts")
    index.add_argument("--vectors", action="store_true", help="compute structural vectors/neighbors/projection")
    index.set_defaults(func=cmd_index)

    serve_p = sub.add_parser("serve", parents=[common], help="serve local API (+ optional UI)")
    serve_p.add_argument("--host", default="127.0.0.1")
    serve_p.add_argument("--port", type=int, default=8765)
    serve_p.add_argument("--enable-jobs", action="store_true", help="allow new/improve harness jobs")
    serve_p.set_defaults(func=cmd_serve)

    return parser


def main(argv: list[str] | None = None) -> int:
    parser = build_parser()
    args = parser.parse_args(argv)
    args.root = args.root.resolve()
    return int(args.func(args))


if __name__ == "__main__":
    raise SystemExit(main())
