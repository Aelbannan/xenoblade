#!/usr/bin/env python3
"""CLI shim so the TS harness can run the anti-cheat lint gate.

Usage (from repo root):
    python3 tools/pi_harness/lint_cli.py --pair <old_snapshot_file> <new_abs_path> [--pair ...]

Prints one JSON object: {"ok": bool, "violations": [{path, rule, line, detail}]}.
"""
from __future__ import annotations

import argparse
import json
import os
import subprocess
import sys

REPO_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
sys.path.insert(0, REPO_ROOT)

from tools.pi_harness.lint import lint_delta  # noqa: E402


def read_text(path: str) -> str | None:
    if not os.path.exists(path):
        return None
    with open(path, "r", encoding="utf-8", errors="replace") as f:
        return f.read()


def git_head_text(relpath: str) -> str | None:
    """Return the blob at HEAD for a repo-relative path (the --file delta
    baseline), or None when the path is untracked / outside the repo / git
    is unavailable. Callers fall back to whole-file lint in that case —
    strictness is the safe default for files HEAD doesn't know."""
    if relpath.startswith("..") or os.path.isabs(relpath):
        return None
    try:
        out = subprocess.run(
            ["git", "show", f"HEAD:{relpath}"],
            capture_output=True, text=True, timeout=10, cwd=REPO_ROOT,
        )
        if out.returncode == 0:
            return out.stdout
    except Exception:
        pass
    return None


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--pair", nargs=2, action="append", default=[],
                        metavar=("OLD", "NEW"))
    # Full-file lint mode (snapshots disabled). The default baseline is the
    # last COMMIT, not None: linting every line as "added" flags legitimate
    # pre-existing code (`#pragma once` line 1 of every src/kyoshin/*.hpp,
    # `extern "C"` blocks for func_* symbols) and makes TU-final unpassable
    # (Kimi H1). Only NEW (since HEAD) violations gate.
    parser.add_argument("--file", action="append", default=[],
                        metavar="PATH", help="lint file against HEAD baseline (delta lint)")
    args = parser.parse_args()

    violations = []
    for new_path in args.file:
        new_text = read_text(new_path)
        if new_text is None:
            continue
        relpath = os.path.relpath(new_path, REPO_ROOT)
        old_text = git_head_text(relpath)
        for v in lint_delta(new_path, old_text, new_text):
            violations.append({
                "path": new_path,
                "rule": v.rule,
                "line": v.line,
                "detail": v.detail,
            })
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
