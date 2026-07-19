"""SQLite schema and helpers for Decomp Atlas."""

from __future__ import annotations

import hashlib
import json
import os
import sqlite3
from pathlib import Path
from typing import Any, Iterable, Optional

from tools.decomp_atlas.lib.config import ATLAS_SCHEMA_VERSION, INDEXER_VERSION

SCHEMA_SQL = """
CREATE TABLE IF NOT EXISTS meta (
    key TEXT PRIMARY KEY,
    value TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS functions (
    target_id TEXT PRIMARY KEY,
    region TEXT,
    kind TEXT,
    display_name TEXT,
    symbol TEXT,
    address TEXT,
    size INTEGER,
    source TEXT,
    unit TEXT,
    target_object TEXT,
    base_object TEXT,
    tier TEXT,
    milestone TEXT,
    capabilities TEXT,
    required_level TEXT,
    workflow_status TEXT,
    status TEXT,
    instruction_percent REAL,
    buildable INTEGER NOT NULL DEFAULT 0,
    owner TEXT,
    claimed_at TEXT,
    origin TEXT,
    notes TEXT,
    direct_call_count INTEGER NOT NULL DEFAULT 0,
    unresolved_call_count INTEGER NOT NULL DEFAULT 0,
    has_indirect_calls INTEGER NOT NULL DEFAULT 0,
    instruction_count INTEGER,
    branch_count INTEGER,
    relocation_count INTEGER,
    stack_frame INTEGER,
    readiness TEXT,
    difficulty REAL,
    artifact_fresh INTEGER NOT NULL DEFAULT 0,
    callgraph_status TEXT,
    vector_json TEXT
);

CREATE INDEX IF NOT EXISTS idx_functions_status ON functions(status);
CREATE INDEX IF NOT EXISTS idx_functions_workflow ON functions(workflow_status);
CREATE INDEX IF NOT EXISTS idx_functions_tier ON functions(tier);
CREATE INDEX IF NOT EXISTS idx_functions_milestone ON functions(milestone);
CREATE INDEX IF NOT EXISTS idx_functions_unit ON functions(unit);
CREATE INDEX IF NOT EXISTS idx_functions_source ON functions(source);
CREATE INDEX IF NOT EXISTS idx_functions_readiness ON functions(readiness);
CREATE INDEX IF NOT EXISTS idx_functions_buildable ON functions(buildable);
CREATE INDEX IF NOT EXISTS idx_functions_owner ON functions(owner);
CREATE INDEX IF NOT EXISTS idx_functions_region ON functions(region);
CREATE INDEX IF NOT EXISTS idx_functions_symbol ON functions(symbol);
CREATE INDEX IF NOT EXISTS idx_functions_display ON functions(display_name);

CREATE TABLE IF NOT EXISTS edges (
    caller_id TEXT NOT NULL,
    callee_id TEXT NOT NULL,
    kind TEXT NOT NULL,
    PRIMARY KEY (caller_id, callee_id, kind)
);
CREATE INDEX IF NOT EXISTS idx_edges_callee ON edges(callee_id);
CREATE INDEX IF NOT EXISTS idx_edges_kind ON edges(kind);

CREATE TABLE IF NOT EXISTS attempts (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    target_id TEXT NOT NULL,
    timestamp TEXT,
    status TEXT,
    instruction_match REAL,
    hypothesis TEXT,
    next_change TEXT,
    runtime_test TEXT,
    policy_exception INTEGER NOT NULL DEFAULT 0,
    equivalence_status TEXT,
    raw_json TEXT
);
CREATE INDEX IF NOT EXISTS idx_attempts_target ON attempts(target_id);
CREATE INDEX IF NOT EXISTS idx_attempts_timestamp ON attempts(timestamp);

CREATE TABLE IF NOT EXISTS experiments (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    experiment_id TEXT,
    target_id TEXT NOT NULL,
    workflow TEXT,
    model TEXT,
    model_id TEXT,
    run_index INTEGER,
    timestamp TEXT,
    duration_seconds REAL,
    input_tokens INTEGER,
    output_tokens INTEGER,
    cost REAL,
    result_status TEXT,
    match_percent REAL,
    equivalence_result TEXT,
    size_ok INTEGER,
    accepted INTEGER,
    winner INTEGER,
    error TEXT,
    artifact TEXT,
    raw_json TEXT
);
CREATE INDEX IF NOT EXISTS idx_experiments_target ON experiments(target_id);
CREATE INDEX IF NOT EXISTS idx_experiments_id ON experiments(experiment_id);

CREATE TABLE IF NOT EXISTS artifacts (
    target_id TEXT PRIMARY KEY,
    retail_object_hash TEXT,
    candidate_object_hash TEXT,
    source_hash TEXT,
    cpp_source TEXT,
    retail_asm TEXT,
    candidate_asm TEXT,
    relocations_json TEXT,
    decoded_json TEXT,
    warnings_json TEXT,
    updated_at TEXT
);

CREATE TABLE IF NOT EXISTS neighbors (
    target_id TEXT NOT NULL,
    neighbor_id TEXT NOT NULL,
    score REAL NOT NULL,
    method TEXT NOT NULL,
    rank INTEGER NOT NULL,
    PRIMARY KEY (target_id, neighbor_id, method)
);
CREATE INDEX IF NOT EXISTS idx_neighbors_target ON neighbors(target_id);

CREATE TABLE IF NOT EXISTS projection (
    target_id TEXT PRIMARY KEY,
    x REAL NOT NULL,
    y REAL NOT NULL,
    version TEXT NOT NULL
);
"""


def open_db(path: Path | str, *, readonly: bool = False) -> sqlite3.Connection:
    """Open (and optionally create) an Atlas SQLite database."""
    db_path = Path(path)
    if readonly:
        if not db_path.is_file():
            raise FileNotFoundError(f"Atlas database not found: {db_path}")
        uri = db_path.resolve().as_uri() + "?mode=ro"
        conn = sqlite3.connect(uri, uri=True)
    else:
        db_path.parent.mkdir(parents=True, exist_ok=True)
        conn = sqlite3.connect(str(db_path))
    conn.row_factory = sqlite3.Row
    conn.execute("PRAGMA foreign_keys = ON")
    if not readonly:
        conn.executescript(SCHEMA_SQL)
    return conn


def init_schema(conn: sqlite3.Connection) -> None:
    conn.executescript(SCHEMA_SQL)
    set_meta(conn, "schema_version", str(ATLAS_SCHEMA_VERSION))
    set_meta(conn, "indexer_version", INDEXER_VERSION)


def set_meta(conn: sqlite3.Connection, key: str, value: Any) -> None:
    conn.execute(
        "INSERT INTO meta(key, value) VALUES(?, ?) "
        "ON CONFLICT(key) DO UPDATE SET value = excluded.value",
        (key, "" if value is None else str(value)),
    )


def get_meta(conn: sqlite3.Connection, key: str, default: Optional[str] = None) -> Optional[str]:
    row = conn.execute("SELECT value FROM meta WHERE key = ?", (key,)).fetchone()
    if row is None:
        return default
    return str(row["value"])


def get_all_meta(conn: sqlite3.Connection) -> dict[str, str]:
    rows = conn.execute("SELECT key, value FROM meta").fetchall()
    return {str(r["key"]): str(r["value"]) for r in rows}


def file_sha256(path: Path | str) -> Optional[str]:
    """Return lowercase hex SHA-256 of a file, or None if missing."""
    p = Path(path)
    if not p.is_file():
        return None
    digest = hashlib.sha256()
    with p.open("rb") as handle:
        while True:
            chunk = handle.read(1 << 20)
            if not chunk:
                break
            digest.update(chunk)
    return digest.hexdigest()


def text_sha256(text: str) -> str:
    return hashlib.sha256(text.encode("utf-8")).hexdigest()


def atomic_replace(src: Path | str, dest: Path | str) -> None:
    """Atomically replace ``dest`` with ``src`` (same-filesystem rename)."""
    src_path = Path(src)
    dest_path = Path(dest)
    dest_path.parent.mkdir(parents=True, exist_ok=True)
    os.replace(src_path, dest_path)


def write_json_atomic(path: Path | str, payload: Any) -> None:
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    tmp = path.with_suffix(path.suffix + ".tmp")
    tmp.write_text(json.dumps(payload, indent=2, sort_keys=True) + "\n", encoding="utf-8")
    atomic_replace(tmp, path)


def row_to_dict(row: sqlite3.Row | None) -> Optional[dict[str, Any]]:
    if row is None:
        return None
    return {key: row[key] for key in row.keys()}


def rows_to_dicts(rows: Iterable[sqlite3.Row]) -> list[dict[str, Any]]:
    return [{key: row[key] for key in row.keys()} for row in rows]
