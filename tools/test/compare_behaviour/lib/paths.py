from __future__ import annotations

from pathlib import Path

ROOT = Path(__file__).resolve().parents[4]
TOOL_DIR = Path(__file__).resolve().parents[1]
HOST_DIR = TOOL_DIR / "host"
BUILD_TEST_DIR = ROOT / "build" / "us" / "test" / "compare_behaviour"


def repo_root() -> Path:
    return ROOT


def retail_object(region: str, unit: str) -> Path:
    return ROOT / "build" / region / "obj" / f"{unit}.o"


def decomp_object(region: str, unit: str) -> Path:
    return ROOT / "build" / region / "src" / f"{unit}.o"
