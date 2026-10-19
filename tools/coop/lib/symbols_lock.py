"""Advisory exclusive lock serialising ``config/<region>/symbols.txt`` edits.

Why this exists
---------------
``symbols.txt`` is a single shared file that several tools (and, historically,
several agents at once) read-modify-write.  Concurrent writers are the suspected
cause of a truncated ``config/jp/symbols.txt`` (the file was found cut off
mid-line).  Pairing this lock with the atomic temp-file+``os.replace`` write in
``tools/coop/lib/atomic_write.py`` means an interrupted or racing edit can no
longer destroy the file.

Design (mirrors ``tools/pi_harness/build_lock.py``)
---------------------------------------------------
- ``flock(LOCK_EX)`` on ``build/.symbols_edit.lock`` -- an advisory lock, so it
  only serialises processes that opt in by calling :func:`symbols_edit_lock`.
- The lock file is **never deleted**: ``flock`` is per-inode, so unlinking it
  would let two processes lock different inodes and both proceed.
- Waits (with a timeout) rather than failing fast, so a queued editor simply
  runs after the current one instead of erroring out.
- **Re-entrant within a thread.**  ``flock`` is per *open file description*, so a
  second ``os.open`` + ``flock`` in the same process would block against itself
  and deadlock.  Nested callers (e.g. the legacy full-rename, which calls
  ``apply_rename`` inside its own lock) are handled by a thread-local depth
  counter: the inner acquisition is a no-op.
- ``build/`` is gitignored build output, so this adds no repo-root clutter.
"""
from __future__ import annotations

import fcntl
import os
import threading
import time
from contextlib import contextmanager
from pathlib import Path
from typing import Iterator

LOCK_REL = Path("build") / ".symbols_edit.lock"

#: How long to wait for another editor before giving up.  Generous: a single
#: apply does a plan + a few file writes, and batch runs queue behind each other.
DEFAULT_TIMEOUT_S = 900.0
_POLL_S = 0.2

#: Per-thread re-entrancy depth (see the module docstring).
_state = threading.local()


@contextmanager
def symbols_edit_lock(
    project_root: Path, timeout_s: float = DEFAULT_TIMEOUT_S
) -> Iterator[None]:
    """Hold the repo-wide symbols-edit lock for the duration of the block.

    Re-entrant per thread.  Raises ``TimeoutError`` if the lock cannot be
    acquired within *timeout_s*.
    """
    depth = getattr(_state, "depth", 0)
    if depth:
        _state.depth = depth + 1
        try:
            yield
        finally:
            _state.depth -= 1
        return

    path = Path(project_root) / LOCK_REL
    path.parent.mkdir(parents=True, exist_ok=True)
    fd = os.open(str(path), os.O_CREAT | os.O_RDWR, 0o644)
    acquired = False
    try:
        deadline = time.monotonic() + timeout_s
        while True:
            try:
                fcntl.flock(fd, fcntl.LOCK_EX | fcntl.LOCK_NB)
                acquired = True
                break
            except OSError:
                if time.monotonic() >= deadline:
                    raise TimeoutError(
                        f"timed out after {timeout_s:.0f}s waiting for the "
                        f"symbols edit lock at {path}"
                    )
                time.sleep(_POLL_S)
        # Metadata for diagnostics only; the flock is the real lock.
        try:
            os.ftruncate(fd, 0)
            os.write(fd, f'{{"pid": {os.getpid()}, "ts": {time.time():.3f}}}\n'.encode())
        except OSError:
            pass
        _state.depth = 1
        yield
    finally:
        _state.depth = 0
        if acquired:
            try:
                fcntl.flock(fd, fcntl.LOCK_UN)
            except OSError:
                pass
        os.close(fd)
