#!/usr/bin/env python3
"""Run a command while holding the repo-wide build lock.

Acquires the same advisory flock used by tools/coop/hexdiff.py
(`build/<region>/.hexdiff.lock`) so harness-driven builds (configure.py,
ninja, size checks in the TU-final phase) serialise against hexdiff builds
triggered by other agents.

Usage (from repo root):
    python3 tools/pi_harness/build_lock.py <region> -- <cmd> [args...]
"""
from __future__ import annotations

import fcntl
import os
import subprocess
import sys


def main() -> int:
    args = sys.argv[1:]
    timeout = 1800
    if len(args) >= 2 and args[0] == "--timeout":
        try:
            timeout = int(args[1])
        except ValueError:
            print("build_lock: --timeout must be an integer", file=sys.stderr)
            return 2
        args = args[2:]
    if len(args) < 2 or "--" not in args:
        print(__doc__, file=sys.stderr)
        return 2
    region = args[0]
    if not region or any(c not in "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-" for c in region):
        print("build_lock: invalid region", file=sys.stderr)
        return 2
    sep = args.index("--")
    cmd = args[sep + 1 :]
    if not cmd:
        print("build_lock: no command after --", file=sys.stderr)
        return 2

    lock_path = os.path.join("build", region, ".hexdiff.lock")
    os.makedirs(os.path.dirname(lock_path), exist_ok=True)

    with open(lock_path, "w") as lock:
        try:
            fcntl.flock(lock, fcntl.LOCK_EX | fcntl.LOCK_NB)
        except OSError:
            print(f"waiting for build lock ({lock_path})...", file=sys.stderr)
            fcntl.flock(lock, fcntl.LOCK_EX)
        try:
            return subprocess.run(cmd, timeout=timeout).returncode
        except subprocess.TimeoutExpired:
            print(f"build_lock: command timed out after {timeout}s: {cmd}",
                  file=sys.stderr)
            return 2
        finally:
            fcntl.flock(lock, fcntl.LOCK_UN)


if __name__ == "__main__":
    sys.exit(main())
