"""Session state persistence and crash reconciliation.

Design reference: docs/llm_decomp_design.md §2, §6 (review findings #1, #2, #6).

Rules implemented here:
- INIT snapshots writable files to the on-disk session dir (fsync) — never
  in-memory only.
- Acceptance snapshots the accepted file state to disk before CLEANUP;
  promotion reads from that directory, not the live tree.
- The best non-accepted file state is checkpointed for the carryover chain.
- reconcile() at startup restores files and marks orphaned sessions ABORTED.
"""
from __future__ import annotations

import json
import os
import shutil
import uuid
from dataclasses import asdict
from datetime import datetime, timezone
from pathlib import Path

from . import paths
from .contracts import SessionMeta, SessionStatus


def _fsync_tree(root: Path) -> None:
    for dirpath, _dirnames, filenames in os.walk(root):
        for name in filenames:
            fd = os.open(Path(dirpath) / name, os.O_RDONLY)
            try:
                os.fsync(fd)
            finally:
                os.close(fd)


def _write_json_fsync(path: Path, data: dict) -> None:
    tmp = path.with_suffix(path.suffix + ".tmp")
    with open(tmp, "w", encoding="utf-8") as fh:
        json.dump(data, fh, indent=2)
        fh.flush()
        os.fsync(fh.fileno())
    os.replace(tmp, path)


def _snapshot_files(repo_root: Path, dest: Path, writable: list[str]) -> None:
    for rel in writable:
        src = repo_root / rel
        if not src.exists():
            continue
        out = dest / rel
        out.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(src, out)


def save_meta(sdir: Path, meta: SessionMeta) -> None:
    _write_json_fsync(paths.session_json_path(sdir), asdict(meta))


def load_meta(sdir: Path) -> SessionMeta:
    data = json.loads(paths.session_json_path(sdir).read_text(encoding="utf-8"))
    return SessionMeta(**data)


def init_session(repo_root: Path, target_id: str, session_type: str,
                 unit: str, symbol: str, writable: list[str],
                 owner: str) -> tuple[SessionMeta, Path]:
    """Create the session dir, snapshot writable files, persist the claim."""
    session_id = uuid.uuid4().hex[:12]
    sdir = paths.session_dir(repo_root, target_id, session_id)
    paths.init_snapshot_dir(sdir).mkdir(parents=True, exist_ok=True)
    meta = SessionMeta(
        target_id=target_id,
        session_id=session_id,
        session_type=session_type,
        unit=unit,
        symbol=symbol,
        status=SessionStatus.INIT.value,
        writable=list(writable),
        owner=owner,
        created_at=datetime.now(timezone.utc).isoformat(),
    )
    _snapshot_files(repo_root, paths.init_snapshot_dir(sdir), writable)
    save_meta(sdir, meta)
    _fsync_tree(sdir)
    return meta, sdir


def set_status(sdir: Path, meta: SessionMeta, status: SessionStatus) -> None:
    meta.status = status.value
    save_meta(sdir, meta)


def record_accepted(repo_root: Path, sdir: Path, meta: SessionMeta) -> Path:
    """Snapshot accepted file state to disk + fsync (deliverable)."""
    dest = paths.accepted_snapshot_dir(sdir)
    dest.mkdir(parents=True, exist_ok=True)
    _snapshot_files(repo_root, dest, meta.writable)
    _fsync_tree(dest)
    return dest


def record_best(repo_root: Path, sdir: Path, meta: SessionMeta) -> Path:
    """Checkpoint best non-accepted state for the carryover chain."""
    dest = paths.best_state_dir(sdir)
    dest.mkdir(parents=True, exist_ok=True)
    _snapshot_files(repo_root, dest, meta.writable)
    return dest


def reconcile(repo_root: Path) -> list[dict]:
    """Restore files and abort sessions whose process died mid-flight.

    A session is orphaned when its session.json exists and its status is
    not DONE/ABORTED. Its writable files are restored from init_snapshot.
    """
    report: list[dict] = []
    root = paths.sessions_root(repo_root)
    if not root.exists():
        return report
    for sdir in root.glob("*/*/"):
        sjson = paths.session_json_path(sdir)
        if not sjson.exists():
            continue
        try:
            meta = load_meta(sdir)
        except (json.JSONDecodeError, TypeError):
            continue
        if meta.status in (SessionStatus.DONE.value,
                           SessionStatus.ABORTED.value):
            continue
        restored: list[str] = []
        for rel in meta.writable:
            snap = paths.init_snapshot_dir(sdir) / rel
            if snap.exists():
                dest = repo_root / rel
                dest.parent.mkdir(parents=True, exist_ok=True)
                shutil.copy2(snap, dest)
                restored.append(rel)
        meta.status = SessionStatus.ABORTED.value
        save_meta(sdir, meta)
        report.append({"session_id": meta.session_id,
                       "target_id": meta.target_id,
                       "restored": restored})
    return report
