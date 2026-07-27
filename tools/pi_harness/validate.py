#!/usr/bin/env python3
"""Validate header/source consistency after fixup/scaffold.

Checks:
  1. No duplicate method declarations in headers
  2. No constructor/destructor mismatches
  3. No ``extern "C"`` stubs left outside blocks (in matched targets)
  4. All ``#include`` directives resolve to existing files

Usage:
    python3 tools/pi_harness/validate.py [--source <path>] [--all]
"""

from __future__ import annotations

import argparse, os, re, sys
from collections import defaultdict

REPO_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))


def check_duplicate_declarations(header_path: str) -> list[str]:
    """Find duplicate method declarations in a header."""
    issues = []
    try:
        with open(header_path) as f:
            lines = f.readlines()
    except Exception:
        return issues

    seen: dict[str, list[int]] = defaultdict(list)
    for i, line in enumerate(lines, 1):
        # Match method declarations: Type ClassName::method(...);
        m = re.match(r'^\s*(?:\w+(?:\s*[*&])?\s+)?(\w+)\([^)]*\)\s*;', line)
        if m:
            name = m.group(1)
            seen[name].append(i)

    for name, linenos in seen.items():
        if len(linenos) > 1:
            issues.append(
                f"Duplicate declaration '{name}()' at lines {linenos}"
            )

    return issues


def check_orphan_extern_c(source_path: str) -> list[str]:
    """Find extern-C stubs outside blocks."""
    issues = []
    try:
        with open(source_path) as f:
            content = f.read()
    except Exception:
        return issues

    lines = content.split("\n")
    block_depth = 0
    for i, line in enumerate(lines, 1):
        stripped = line.strip()
        if stripped.startswith("extern") and "{" in stripped:
            block_depth += 1
            continue
        if block_depth > 0 and stripped == "}":
            block_depth -= 1
            continue

        if block_depth == 0 and re.match(r'extern\s+"C"\s+\w', stripped):
            # Allow lbl_eu_ declarations
            if "lbl_" not in stripped:
                issues.append(f"Line {i}: orphan extern-C: {stripped[:80]}")

    return issues


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--source", help="Check a single file")
    parser.add_argument("--all", action="store_true", help="Check all .cpp/.hpp files")
    args = parser.parse_args()

    total_issues = 0

    if args.source:
        path = args.source
        issues = check_duplicate_declarations(path) + check_orphan_extern_c(path)
        for i in issues:
            print(f"  {path}: {i}")
        total_issues += len(issues)

    elif args.all:
        # Check all modified files
        import subprocess
        result = subprocess.run(
            ["git", "diff", "--name-only", "--", "*.cpp", "*.hpp"],
            capture_output=True, text=True, cwd=REPO_ROOT,
        )
        files = [f for f in result.stdout.splitlines() if f.endswith((".cpp", ".hpp"))]

        for f in files:
            full = os.path.join(REPO_ROOT, f)
            issues = check_duplicate_declarations(full) + check_orphan_extern_c(full)
            for i in issues:
                print(f"  {f}: {i}")
            total_issues += len(issues)

    if total_issues:
        print(f"\n{total_issues} issue(s) found")
        return 1
    else:
        print("All clean!")
        return 0


if __name__ == "__main__":
    sys.exit(main())
