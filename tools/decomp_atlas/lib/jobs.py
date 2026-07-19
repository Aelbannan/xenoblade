"""Allow-listed llm-harness job controller (no claims, no promotions)."""

from __future__ import annotations

import json
import os
import signal
import subprocess
import sys
import threading
import uuid
from dataclasses import dataclass
from datetime import datetime, timezone
from pathlib import Path
from typing import Any, Dict, List, Optional

from tools.decomp_atlas.lib.config import jobs_dir
from tools.decomp_atlas.lib.database import write_json_atomic

ALLOWED_WORKFLOWS = frozenset({"new", "improve"})
MAX_CONCURRENT = 2


def _utc_now() -> str:
    return datetime.now(timezone.utc).isoformat()


def _relpath(root: Path, path: Path) -> str:
    try:
        return path.resolve().relative_to(root.resolve()).as_posix()
    except ValueError:
        return str(path)


@dataclass
class _LiveJob:
    job_id: str
    process: subprocess.Popen[str]
    log_path: Path
    meta_path: Path


class JobController:
    """Manage at most ``MAX_CONCURRENT`` harness subprocesses."""

    def __init__(self, root: Path | str, region: str) -> None:
        self.root = Path(root).resolve()
        self.region = region
        self.dir = jobs_dir(self.root, region)
        self.dir.mkdir(parents=True, exist_ok=True)
        self._lock = threading.Lock()
        self._live: Dict[str, _LiveJob] = {}

    def _job_dir(self, job_id: str) -> Path:
        return self.dir / job_id

    def _write_meta(self, job_id: str, payload: Dict[str, Any]) -> Path:
        path = self._job_dir(job_id) / "job.json"
        write_json_atomic(path, payload)
        return path

    def _read_meta(self, job_id: str) -> Optional[Dict[str, Any]]:
        path = self._job_dir(job_id) / "job.json"
        if not path.is_file():
            return None
        try:
            return json.loads(path.read_text(encoding="utf-8"))
        except json.JSONDecodeError:
            return None

    def _refresh_live(self) -> None:
        finished: List[str] = []
        for job_id, live in list(self._live.items()):
            code = live.process.poll()
            if code is None:
                continue
            meta = self._read_meta(job_id) or {}
            meta["status"] = "succeeded" if code == 0 else "failed"
            meta["exit_code"] = code
            meta["finished_at"] = _utc_now()
            self._write_meta(job_id, meta)
            finished.append(job_id)
        for job_id in finished:
            self._live.pop(job_id, None)

    def start_job(self, target_id: str, workflow: str) -> Dict[str, Any]:
        if workflow not in ALLOWED_WORKFLOWS:
            raise ValueError(f"workflow must be one of {sorted(ALLOWED_WORKFLOWS)}")
        if not target_id or "/" in target_id or "\\" in target_id or ".." in target_id:
            raise ValueError("invalid target_id")

        with self._lock:
            self._refresh_live()
            running = sum(
                1
                for meta in (self._read_meta(j) for j in self._live)
                if meta and meta.get("status") == "running"
            )
            # Also count live processes directly.
            running = len(self._live)
            if running >= MAX_CONCURRENT:
                raise RuntimeError(f"max concurrent jobs ({MAX_CONCURRENT}) reached")

            job_id = datetime.now(timezone.utc).strftime("%Y%m%dT%H%M%SZ") + "-" + uuid.uuid4().hex[:8]
            job_path = self._job_dir(job_id)
            job_path.mkdir(parents=True, exist_ok=True)
            log_path = job_path / "log.txt"
            argv = [
                sys.executable,
                str(self.root / "tools" / "llm_harness" / "run.py"),
                workflow,
                target_id,
            ]
            meta: Dict[str, Any] = {
                "job_id": job_id,
                "target_id": target_id,
                "workflow": workflow,
                "status": "running",
                "argv": argv,
                "pid": None,
                "started_at": _utc_now(),
                "finished_at": None,
                "exit_code": None,
                "log": _relpath(self.root, log_path),
            }
            log_handle = log_path.open("w", encoding="utf-8")
            try:
                process = subprocess.Popen(
                    argv,
                    cwd=str(self.root),
                    stdout=log_handle,
                    stderr=subprocess.STDOUT,
                    text=True,
                    shell=False,
                )
            except Exception:
                log_handle.close()
                raise
            log_handle.close()

            meta["pid"] = process.pid
            meta_path = self._write_meta(job_id, meta)
            self._live[job_id] = _LiveJob(
                job_id=job_id,
                process=process,
                log_path=log_path,
                meta_path=meta_path,
            )
            return dict(meta)

    def cancel_job(self, job_id: str) -> Dict[str, Any]:
        with self._lock:
            self._refresh_live()
            live = self._live.get(job_id)
            meta = self._read_meta(job_id)
            if meta is None:
                raise KeyError(job_id)
            if live is not None and live.process.poll() is None:
                live.process.terminate()
                try:
                    live.process.wait(timeout=5)
                except subprocess.TimeoutExpired:
                    live.process.kill()
                meta["status"] = "cancelled"
                meta["exit_code"] = live.process.returncode
                meta["finished_at"] = _utc_now()
                self._write_meta(job_id, meta)
                self._live.pop(job_id, None)
            elif meta.get("status") == "running":
                pid = meta.get("pid")
                if isinstance(pid, int) and pid > 0:
                    try:
                        os.kill(pid, signal.SIGTERM)
                    except OSError:
                        pass
                meta["status"] = "cancelled"
                meta["finished_at"] = _utc_now()
                self._write_meta(job_id, meta)
            return dict(meta)

    def get_job(self, job_id: str) -> Optional[Dict[str, Any]]:
        with self._lock:
            self._refresh_live()
        return self._read_meta(job_id)

    def list_jobs(self) -> List[Dict[str, Any]]:
        with self._lock:
            self._refresh_live()
        jobs: List[Dict[str, Any]] = []
        if not self.dir.is_dir():
            return jobs
        for child in sorted(self.dir.iterdir(), reverse=True):
            if not child.is_dir():
                continue
            meta = self._read_meta(child.name)
            if meta:
                jobs.append(meta)
        return jobs

    def job_events(self, job_id: str, *, offset: int = 0, limit: int = 65536) -> Dict[str, Any]:
        meta = self.get_job(job_id)
        if meta is None:
            raise KeyError(job_id)
        log_path = self._job_dir(job_id) / "log.txt"
        text = ""
        if log_path.is_file():
            data = log_path.read_bytes()
            chunk = data[offset : offset + limit]
            text = chunk.decode("utf-8", errors="replace")
            next_offset = offset + len(chunk)
        else:
            next_offset = offset
        return {
            "job_id": job_id,
            "status": meta.get("status"),
            "offset": offset,
            "next_offset": next_offset,
            "text": text,
            "eof": meta.get("status") not in {"running", "starting"},
        }

    def cleanup(self, *, keep: int = 50) -> int:
        """Remove oldest finished job dirs beyond ``keep``. Returns removed count."""
        jobs = self.list_jobs()
        finished = [j for j in jobs if j.get("status") not in {"running"}]
        removed = 0
        for meta in finished[keep:]:
            job_id = meta.get("job_id")
            if not job_id:
                continue
            path = self._job_dir(str(job_id))
            if path.is_dir():
                for child in path.iterdir():
                    child.unlink(missing_ok=True)
                path.rmdir()
                removed += 1
        return removed
