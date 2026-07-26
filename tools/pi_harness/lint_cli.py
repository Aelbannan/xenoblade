#!/usr/bin/env python3
"""CLI shim so the TS harness can reuse tools/llm_decomp/lint.py.

Usage (from repo root):
    python3 tools/pi_harness/lint_cli.py --pair <old_snapshot_file> <new_abs_path> [--pair ...]

Prints one JSON object: {"ok": bool, "violations": [{path, rule, line, detail}]}.
"""
from __future__ import annotations

import argparse
import json
import os
import sys

REPO_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
sys.path.insert(0, REPO_ROOT)

from tools.llm_decomp.lint import lint_delta  # noqa: E402


def read_text(path: str) -> str | None:
    if not os.path.exists(path):
        return None
    with open(path, "r", encoding="utf-8", errors="replace") as f:
        return f.read()


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--pair", nargs=2, action="append", default=[],
                        metavar=("OLD", "NEW"))
    args = parser.parse_args()

    violations = []
    for old_path, new_path in args.pair:
        old_text = read_text(old_path)
        new_text = read_text(new_path)
        if new_text is None:
            continue
        for v in lint_delta(new_path, old_text, new_text):
            violations.append({
                "path": new_path,
                "rule": v.rule,
                "line": v.line,
                "detail": v.detail,
            })

    print(json.dumps({"ok": not violations, "violations": violations}, indent=2))
    return 0


if __name__ == "__main__":
    sys.exit(main())
