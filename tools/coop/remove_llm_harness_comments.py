#!/usr/bin/env python3
"""
Remove all 'llm-harness' comment lines from source files.

Targets the pattern:
    // Mangled extern stubs for llm-harness / coop selection.

and its variant:
    // Mangled extern stubs for n / coop selection.

Also targets:
    // LLM-HARNESS-BEGIN: <target-id>
    // LLM-HARNESS-END: <target-id>

across all .c, .h, .cpp, .hpp, and .ctx.c files in src/ and libs/.
"""

import os
import re
import sys

REPO_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
GLOBS = ("src", "libs")

PATTERNS = [
    re.compile(r"//\s*Mangled extern stubs for.*coop selection\.\s*$"),
    re.compile(r"//\s*LLM-HARNESS-BEGIN:\s*"),
    re.compile(r"//\s*LLM-HARNESS-END:\s*"),
    re.compile(r"//\s*---\s*LLM-HARNESS stubs"),
]


def find_source_files(root):
    for top in GLOBS:
        dirpath = os.path.join(root, top)
        if not os.path.isdir(dirpath):
            continue
        for dirname, _, filenames in os.walk(dirpath):
            for fn in filenames:
                if any(fn.endswith(ext) for ext in (".c", ".h", ".cpp", ".hpp", ".ctx.c")):
                    yield os.path.join(dirname, fn)


def file_has_pattern(path):
    try:
        with open(path, "r") as f:
            for line in f:
                for pat in PATTERNS:
                    if pat.search(line):
                        return True
        return False
    except Exception:
        return False


def remove_pattern_lines(path, dry_run=False):
    """Remove lines matching any of the patterns. Returns (changed, removed_count)."""
    with open(path, "r") as f:
        lines = f.readlines()

    filtered = []
    removed = 0
    for line in lines:
        if any(pat.search(line) for pat in PATTERNS):
            removed += 1
        else:
            filtered.append(line)

    if removed == 0:
        return False, 0

    if not dry_run:
        with open(path, "w") as f:
            f.writelines(filtered)

    return True, removed


def main():
    dry_run = "--dry-run" in sys.argv
    stats = {"files_modified": 0, "lines_removed": 0, "files_skipped": 0}

    for fpath in find_source_files(REPO_ROOT):
        if not file_has_pattern(fpath):
            continue
        changed, count = remove_pattern_lines(fpath, dry_run=dry_run)
        if changed:
            rel = os.path.relpath(fpath, REPO_ROOT)
            print(f"{'[DRY-RUN]' if dry_run else '[REMOVED]'} {rel}  ({count} line{'s' if count != 1 else ''})")
            stats["files_modified"] += 1
            stats["lines_removed"] += count
        else:
            stats["files_skipped"] += 1

    label = "Would remove" if dry_run else "Removed"
    print(f"\n{label} {stats['lines_removed']} line(s) across {stats['files_modified']} file(s). "
          f"({stats['files_skipped']} files matched pattern but unchanged)")


if __name__ == "__main__":
    main()