from __future__ import annotations

import json
from dataclasses import asdict, dataclass
from datetime import datetime, timezone
from pathlib import Path
from typing import Any, Dict, List, Optional


@dataclass
class AttemptRecord:
    target_id: str
    function: str
    region: str
    unit: str
    symbol: Optional[str]
    status: str
    instruction_match: Optional[float]
    relocation_match: Optional[float]
    code_match_percent: Optional[float]
    data_match_percent: Optional[float]
    hypothesis: str = ""
    next_change: str = ""
    runtime_test: str = ""
    git_commit: Optional[str] = None
    timestamp: str = ""

    def to_json(self) -> Dict[str, Any]:
        data = asdict(self)
        if not data["timestamp"]:
            data["timestamp"] = datetime.now(timezone.utc).isoformat()
        return data


def append_attempt(log_path: Path, record: AttemptRecord) -> Path:
    log_path.parent.mkdir(parents=True, exist_ok=True)
    if not record.timestamp:
        record.timestamp = datetime.now(timezone.utc).isoformat()
    with log_path.open("a", encoding="utf-8") as f:
        f.write(json.dumps(record.to_json(), separators=(",", ":")) + "\n")
    return log_path


def read_attempts(log_path: Path, *, tail: Optional[int] = None) -> List[Dict[str, Any]]:
    if not log_path.is_file():
        return []
    lines = log_path.read_text(encoding="utf-8").splitlines()
    if tail is not None:
        lines = lines[-tail:]
    return [json.loads(line) for line in lines if line.strip()]


def count_attempts(log_path: Path, target_id: str) -> int:
    return sum(1 for row in read_attempts(log_path) if row.get("target_id") == target_id)
