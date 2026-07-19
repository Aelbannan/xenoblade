"""Paths and version constants for Decomp Atlas."""

from __future__ import annotations

from pathlib import Path

ATLAS_SCHEMA_VERSION = 1
INDEXER_VERSION = "1.0.0"
VECTOR_METHOD = "structural-v1"
PROJECTION_SEED = 0xA71A5
PROJECTION_VERSION = "pca2-v1"


def atlas_root(project_root: Path | str, region: str) -> Path:
    """Return ``project_root/build/decomp-atlas/<region>``."""
    return Path(project_root) / "build" / "decomp-atlas" / region


def db_path(project_root: Path | str, region: str) -> Path:
    root = atlas_root(project_root, region)
    sqlite_path = root / "atlas.sqlite"
    legacy = root / "atlas.db"
    if sqlite_path.is_file() or not legacy.is_file():
        return sqlite_path
    return legacy


def index_state_path(project_root: Path | str, region: str) -> Path:
    return atlas_root(project_root, region) / "index-state.json"


def jobs_dir(project_root: Path | str, region: str) -> Path:
    return atlas_root(project_root, region) / "jobs"


def ui_dir(project_root: Path | str, region: str) -> Path:
    """Prefer staged UI under build/, else package Vite dist."""
    staged = atlas_root(project_root, region) / "ui"
    if staged.is_dir() and (staged / "index.html").is_file():
        return staged
    package = Path(__file__).resolve().parents[1]
    for candidate in (package / "dist", package / "web" / "dist", package / "web"):
        if candidate.is_dir() and (
            (candidate / "index.html").is_file() or candidate.name == "web"
        ):
            return candidate
    return package / "web"
