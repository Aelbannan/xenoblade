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
    if len(args) < 3 or "--" not in args:
        print(__doc__, file=sys.stderr)
        return 2
    region = args[0]
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
            return subprocess.run(cmd).returncode
        finally:
            fcntl.flock(lock, fcntl.LOCK_UN)


if __name__ == "__main__":
    sys.exit(main())
