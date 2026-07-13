from __future__ import annotations

import json
from dataclasses import dataclass
from pathlib import Path
from typing import List, Optional

from tools.test.compare_behaviour.lib.paths import TOOL_DIR


@dataclass
class BehaviourTest:
    id: str
    description: str
    unit: str
    symbol: str
    host_binary: Optional[str]
    ppc_source: Optional[str] = None
    ppc_stubs: Optional[list[str]] = None
    region: str = "us"


def load_manifest(path: Optional[Path] = None) -> tuple[str, List[BehaviourTest]]:
    manifest_path = path or (TOOL_DIR / "manifest.json")
    with manifest_path.open(encoding="utf-8") as f:
        data = json.load(f)

    region = str(data.get("region", "us"))
    tests: List[BehaviourTest] = []
    for item in data.get("tests", []):
        tests.append(
            BehaviourTest(
                id=item["id"],
                description=item.get("description", ""),
                unit=item["unit"],
                symbol=item["symbol"],
                host_binary=item.get("host_binary"),
                ppc_source=item.get("ppc_source"),
                ppc_stubs=item.get("ppc_stubs"),
                region=region,
            )
        )
    return region, tests


def get_test(tests: List[BehaviourTest], test_id: str) -> BehaviourTest:
    for test in tests:
        if test.id == test_id:
            return test
    known = ", ".join(t.id for t in tests)
    raise KeyError(f"Unknown test id '{test_id}'. Known: {known}")
