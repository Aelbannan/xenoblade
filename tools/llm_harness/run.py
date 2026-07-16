#!/usr/bin/env python3
from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from tools.llm_harness.core import Harness


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description="Generic project-adapted LLM experiment harness")
    parser.add_argument("--config", type=Path, default=Path("llm-harness.json"))
    sub = parser.add_subparsers(dest="command", required=True)
    for name in ("new", "improve"):
        command = sub.add_parser(name)
        command.add_argument("target_id")
        command.add_argument("--runs", type=int)
        command.add_argument("--dry-run", action="store_true")
    sub.add_parser("stats")
    args = parser.parse_args(argv)
    harness = Harness(args.config)
    if args.command == "stats":
        print(json.dumps(harness.stats(), indent=2))
        return 0
    output = harness.run(args.command, args.target_id, runs=args.runs, dry_run=args.dry_run)
    print(output)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

