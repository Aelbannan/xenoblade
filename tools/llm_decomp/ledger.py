"""TU completion tracking ledger for the llm_decomp pipeline.

Checked-in JSON file recording pipeline stage completion per translation
unit.  All mutations are atomic and interprocess-safe via fcntl flock on a
sidecar ``<path>.lock`` file.

Design reference: docs/llm_decomp_design.md §12 "TU completion tracking".
"""
from __future__ import annotations

import fcntl
import json
import os
import time
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any


# ---------------------------------------------------------------------------
# Public constants
# ---------------------------------------------------------------------------

STAGES = ("type-recovery", "match", "rename", "tu-cleanup", "size-trim")

STATUS_IN_PROGRESS = "in_progress"
STATUS_COMPLETE = "complete"
STATUS_OVER_BUDGET = "over-budget"

_VALID_STATUSES = frozenset({STATUS_IN_PROGRESS, STATUS_COMPLETE, STATUS_OVER_BUDGET})


# ---------------------------------------------------------------------------
# Exceptions
# ---------------------------------------------------------------------------

class LedgerError(Exception):
    """Corrupt or unreadable ledger JSON."""


# ---------------------------------------------------------------------------
# Data model
# ---------------------------------------------------------------------------

@dataclass
class UnitRecord:
    unit: str
    status: str = STATUS_IN_PROGRESS
    stages: dict[str, str] = field(default_factory=dict)  # stage name → ISO-8601 timestamp
    sessions: list[str] = field(default_factory=list)     # session ids, append-only
    notes: str = ""


# ---------------------------------------------------------------------------
# Internal helpers
# ---------------------------------------------------------------------------

def _now_iso() -> str:
    """UTC ISO-8601 timestamp with seconds precision."""
    return time.strftime("%Y-%m-%dT%H:%M:%SZ", time.gmtime())


def _lock_path(path: Path) -> Path:
    return path.with_suffix(path.suffix + ".lock")


def _read_ledger_json(path: Path) -> dict[str, Any]:
    """Read the ledger JSON file, returning the parsed dict.

    Returns the empty-ledger shape if the file does not exist.
    Raises ``LedgerError`` on corrupt content.
    """
    if not path.is_file():
        return {"version": 1, "units": {}}

    try:
        data = json.loads(path.read_text("utf-8"))
    except (json.JSONDecodeError, OSError) as exc:
        raise LedgerError(f"Corrupt ledger file: {path}") from exc

    if not isinstance(data, dict) or "version" not in data or "units" not in data:
        raise LedgerError(f"Corrupt ledger file (missing version/units keys): {path}")
    return data


def _write_ledger_json(path: Path, data: dict[str, Any]) -> None:
    """Atomically write *data* to *path* via a temporary sibling file."""
    tmp = path.with_name(f"{path.name}.tmp.{os.getpid()}")
    tmp.write_text(json.dumps(data, indent=2, sort_keys=True) + "\n", "utf-8")
    tmp.replace(path)


# ---------------------------------------------------------------------------
# Ledger (public API)
# ---------------------------------------------------------------------------

class Ledger:
    """TU completion ledger backed by a JSON file.

    All methods that mutate the ledger acquire an ``fcntl.flock`` on
    ``<path>.lock``, re-read the file under the lock, and write atomically
    so that concurrent processes or threads never lose updates.
    """

    def __init__(self, path: Path) -> None:
        self._path = path.resolve()

    # -- queries (read-only, no lock needed) --------------------------------

    def record(self, unit: str) -> UnitRecord | None:
        """Return the current record for *unit*, or ``None`` if absent."""
        data = _read_ledger_json(self._path)
        raw = data["units"].get(unit)
        if raw is None:
            return None
        return UnitRecord(unit=unit, **raw)

    def is_complete(self, unit: str) -> bool:
        """Return ``True`` if *unit* has status ``STATUS_COMPLETE``."""
        rec = self.record(unit)
        return rec is not None and rec.status == STATUS_COMPLETE

    def complete_units(self) -> list[str]:
        """Return all units whose status is ``STATUS_COMPLETE``."""
        data = _read_ledger_json(self._path)
        return [u for u, r in data["units"].items() if r.get("status") == STATUS_COMPLETE]

    # -- mutations (locked, atomic) ----------------------------------------

    def mark_stage(self, unit: str, stage: str, session_id: str) -> None:
        """Mark *stage* as completed for *unit*.

        Validates *stage* ∈ ``STAGES`` (raises ``ValueError``).  Appends
        *session_id* (no duplicates), stamps a UTC ISO-8601 timestamp, and
        sets status to ``STATUS_IN_PROGRESS``.
        """
        if stage not in STAGES:
            raise ValueError(
                f"Unknown stage {stage!r}; expected one of {STAGES}"
            )

        def _mutate(data: dict[str, Any]) -> None:
            units = data["units"]
            if unit not in units:
                units[unit] = {"status": STATUS_IN_PROGRESS, "stages": {}, "sessions": [], "notes": ""}
            rec = units[unit]
            rec["stages"][stage] = _now_iso()
            if session_id not in rec["sessions"]:
                rec["sessions"].append(session_id)
            rec["status"] = STATUS_IN_PROGRESS

        self._locked_mutate(_mutate)

    def set_status(self, unit: str, status: str, notes: str = "") -> None:
        """Set the overall status of *unit*.

        *status* must be one of the three ``STATUS_*`` constants or
        ``ValueError`` is raised.
        """
        if status not in _VALID_STATUSES:
            raise ValueError(
                f"Unknown status {status!r}; expected one of "
                f"{sorted(_VALID_STATUSES)}"
            )

        def _mutate(data: dict[str, Any]) -> None:
            units = data["units"]
            if unit not in units:
                units[unit] = {"status": STATUS_IN_PROGRESS, "stages": {}, "sessions": [], "notes": ""}
            units[unit]["status"] = status
            if notes:
                units[unit]["notes"] = notes

        self._locked_mutate(_mutate)

    # -- internal ----------------------------------------------------------

    def _locked_mutate(self, mutator) -> None:
        """Acquire the lock, re-read, apply *mutator*, write atomically."""
        lock = _lock_path(self._path)
        # Ensure the lock file exists before opening.
        lock.touch()
        fd = os.open(str(lock), os.O_RDONLY)
        try:
            fcntl.flock(fd, fcntl.LOCK_EX)
            data = _read_ledger_json(self._path)
            mutator(data)
            _write_ledger_json(self._path, data)
        finally:
            os.close(fd)