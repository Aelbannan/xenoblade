#!/usr/bin/env python3
"""Run a command while holding the repo-wide build lock.

Acquires the same advisory flock used by tools/coop/hexdiff.py
(`build/<region>/.hexdiff.lock`) so harness-driven builds (configure.py,
ninja, size checks in the TU-final phase) serialise against hexdiff builds
triggered by other agents.

Lock protocol:
  - The lock file is NEVER deleted/unlinked.  flock() is per-inode, so
    unlinking creates a new inode and breaks mutual exclusion.
  - The holder writes JSON metadata {pid, ts} AFTER acquiring the flock.
  - Waiters check if the holder PID is alive.  If the holder is dead, the
    kernel already released the flock, so a plain flock(LOCK_EX|LOCK_NB)
    succeeds immediately — no steal/unlink needed.
  - For stuck-but-alive holders, SIGKILL is sent after STALE_TIMEOUT.

Usage (from repo root):
    python3 tools/pi_harness/build_lock.py <region> -- <cmd> [args...]
"""
from __future__ import annotations

import atexit
import fcntl
import json
import os
import signal
import subprocess
import sys
import time

# If the holder PID is alive but the lock is older than this (seconds),
# send SIGKILL to the holder to unblock ourselves.
#
# Must EXCEED the longest legitimate hold: batch-cycle.py holds the lock up
# to its own --timeout (1800s in the harness), and a TU-final configure/ninja
# can hold it for the full build. A smaller stale threshold would SIGKILL a
# live, legitimately-working holder mid-cycle (adversarial review H5).
# Override with --stale-timeout N.
STALE_TIMEOUT = 3600

# Polling interval when waiting for the lock.
POLL_INTERVAL = 2

# Maximum time to wait before giving up (0 = no limit).
MAX_WAIT = 0


def _pid_alive(pid: int) -> bool:
    """Return True if *pid* is a running process."""
    if pid <= 0:
        return False
    try:
        os.kill(pid, 0)
        return True
    except OSError:
        return False


def _read_lock_meta(lock_path: str) -> dict | None:
    """Read PID/timestamp metadata from the lock file (JSON)."""
    try:
        with open(lock_path) as f:
            data = json.load(f)
        if isinstance(data, dict) and isinstance(data.get("pid"), int):
            return data
    except (OSError, json.JSONDecodeError, ValueError):
        pass
    return None


def _write_lock_meta(fd: int) -> None:
    """Write current PID and timestamp into the lock file (truncates)."""
    meta = json.dumps({"pid": os.getpid(), "ts": time.monotonic()})
    os.ftruncate(fd, 0)
    os.lseek(fd, 0, os.SEEK_SET)
    os.write(fd, meta.encode())


def _try_kill_holder(lock_path: str, stale_timeout: int = STALE_TIMEOUT) -> bool:
    """If the holder is dead or stale, return True (caller should retry flock).
    If the holder is alive and not stale, return False."""
    meta = _read_lock_meta(lock_path)
    if meta is None:
        # No valid metadata — assume stale.
        return True

    pid = meta.get("pid", 0)
    ts = meta.get("ts", 0)
    age = time.monotonic() - ts

    if not _pid_alive(pid):
        # Holder is dead — kernel already released the flock.
        return True

    if age > stale_timeout:
        # Holder is alive but stuck — kill it.
        print(
            f"build_lock: holder PID {pid} alive for {age:.0f}s, sending SIGKILL...",
            file=sys.stderr,
            flush=True,
        )
        try:
            os.kill(pid, signal.SIGKILL)
        except OSError:
            pass
        # Give the kernel a moment to clean up.
        time.sleep(0.5)
        return True

    return False


def main() -> int:
    args = sys.argv[1:]
    timeout = 1800
    stale_timeout = STALE_TIMEOUT
    while args and args[0] in ("--timeout", "--stale-timeout"):
        flag = args.pop(0)
        try:
            val = int(args.pop(0))
        except (ValueError, IndexError):
            print(f"build_lock: {flag} must be an integer", file=sys.stderr)
            return 2
        if flag == "--timeout":
            timeout = val
        else:
            stale_timeout = val
    if len(args) < 2 or "--" not in args:
        print(__doc__, file=sys.stderr)
        return 2
    region = args[0]
    if not region or any(
        c not in "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-"
        for c in region
    ):
        print("build_lock: invalid region", file=sys.stderr)
        return 2
    sep = args.index("--")
    cmd = args[sep + 1 :]
    if not cmd:
        print("build_lock: no command after --", file=sys.stderr)
        return 2

    lock_path = os.path.join("build", region, ".hexdiff.lock")
    os.makedirs(os.path.dirname(lock_path), exist_ok=True)

    # --- Acquire phase ---
    # Open (or create) the lock file.  We NEVER delete/unlink it.
    lock_fd = os.open(lock_path, os.O_CREAT | os.O_RDWR)

    # Ensure clean release on exit / SIGTERM.
    def _release() -> None:
        try:
            fcntl.flock(lock_fd, fcntl.LOCK_UN)
        except OSError:
            pass
        try:
            os.close(lock_fd)
        except OSError:
            pass

    atexit.register(_release)

    def _sigterm_handler(signum: int, frame: object) -> None:
        _release()
        signal.signal(signum, signal.SIG_DFL)
        os.kill(os.getpid(), signum)

    signal.signal(signal.SIGTERM, _sigterm_handler)

    wait_start = time.monotonic()
    warned = False

    while True:
        try:
            fcntl.flock(lock_fd, fcntl.LOCK_EX | fcntl.LOCK_NB)
            # Got the lock — write our metadata.
            _write_lock_meta(lock_fd)
            break
        except OSError:
            pass

        # Couldn't get the lock — check if we should kill the holder.
        if _try_kill_holder(lock_path, stale_timeout):
            # Retry immediately after killing / detecting dead holder.
            try:
                fcntl.flock(lock_fd, fcntl.LOCK_EX | fcntl.LOCK_NB)
                _write_lock_meta(lock_fd)
                break
            except OSError:
                pass

        elapsed = time.monotonic() - wait_start

        if MAX_WAIT > 0 and elapsed > MAX_WAIT:
            print(
                f"build_lock: gave up after {elapsed:.0f}s waiting for {lock_path}",
                file=sys.stderr,
            )
            _release()
            return 2

        if not warned:
            print(
                f"waiting for build lock ({lock_path})...",
                file=sys.stderr,
                flush=True,
            )
            warned = True

        time.sleep(POLL_INTERVAL)

    # --- Hold phase ---
    try:
        result = subprocess.run(cmd, timeout=timeout)
        return result.returncode
    except subprocess.TimeoutExpired:
        print(
            f"build_lock: command timed out after {timeout}s: {cmd}",
            file=sys.stderr,
        )
        return 2


if __name__ == "__main__":
    sys.exit(main())
