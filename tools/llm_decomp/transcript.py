"""Append-per-turn JSONL transcript (fsync per append; survives crashes).

Design reference: docs/llm_decomp_design.md §8.
"""
from __future__ import annotations

import json
import os
import time
from pathlib import Path


class Transcript:
    def __init__(self, path: Path):
        path.parent.mkdir(parents=True, exist_ok=True)
        self._path = path
        self._fh = open(path, "a", encoding="utf-8")

    def log(self, record: dict) -> None:
        record.setdefault("ts", round(time.time(), 3))
        self._fh.write(json.dumps(record, ensure_ascii=False) + "\n")
        self._fh.flush()
        os.fsync(self._fh.fileno())

    def close(self) -> None:
        self._fh.close()

    def __enter__(self) -> "Transcript":
        return self

    def __exit__(self, *exc) -> None:
        self.close()
