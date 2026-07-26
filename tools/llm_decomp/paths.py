"""On-disk layout for llm_decomp session state.

Everything ephemeral lives under build/llm-decomp/ (gitignored with build/).
Design reference: docs/llm_decomp_design.md §6 (persistence), §8 (transcript).
"""
from __future__ import annotations

from pathlib import Path

SESSIONS_DIRNAME = "sessions"


def sessions_root(repo_root: Path) -> Path:
    return repo_root / "build" / "llm-decomp" / SESSIONS_DIRNAME


def session_dir(repo_root: Path, target_id: str, session_id: str) -> Path:
    return sessions_root(repo_root) / target_id / session_id


def init_snapshot_dir(sdir: Path) -> Path:
    """Pre-session copies of writable files; repo-relative paths preserved."""
    return sdir / "init_snapshot"


def accepted_snapshot_dir(sdir: Path) -> Path:
    """The accepted file state — the session's deliverable (fsynced)."""
    return sdir / "accepted_snapshot"


def best_state_dir(sdir: Path) -> Path:
    """Best non-accepted file state (fewest target mismatches) for carryover."""
    return sdir / "best_state"


def baseline_object_path(sdir: Path) -> Path:
    return sdir / "baseline_object.o"


def session_json_path(sdir: Path) -> Path:
    return sdir / "session.json"


def transcript_path(sdir: Path) -> Path:
    return sdir / "conversation.jsonl"


def build_lock_path(repo_root: Path) -> Path:
    return repo_root / "build" / ".llm_decomp_build.lock"


def include_graph_path(repo_root: Path) -> Path:
    return repo_root / "build" / "llm-decomp" / "include_graph.json"


def pipeline_state_path(repo_root: Path, unit: str) -> Path:
    safe = unit.replace("/", "__")
    return repo_root / "build" / "llm-decomp" / "pipeline" / safe / "state.json"
