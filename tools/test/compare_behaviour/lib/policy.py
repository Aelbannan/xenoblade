from __future__ import annotations

import re
from pathlib import Path

# Minimum host scenarios required when static objdiff match is below 100%.
# Non-FULL_MATCH objects MUST have a host_binary and meet these counts before
# an agent may claim BEHAVIOR_VERIFIED or stop iterating on semantics.
SCENARIO_MINIMUMS: tuple[tuple[float, int], ...] = (
    (95.0, 8),
    (90.0, 12),
    (80.0, 20),
    (0.0, 30),
)


def min_scenarios_for_match(match_percent: float | None) -> int:
    if match_percent is None:
        return SCENARIO_MINIMUMS[-1][1]
    if match_percent >= 100.0:
        return 0
    for threshold, minimum in SCENARIO_MINIMUMS:
        if match_percent >= threshold:
            return minimum
    return SCENARIO_MINIMUMS[-1][1]


def count_host_scenarios(host_source: Path) -> int:
    if not host_source.is_file():
        return 0
    text = host_source.read_text(encoding="utf-8")
    # Count call sites only (not `static void run_scenario(...)` definitions).
    return len(re.findall(r"^\s+run_scenario(?:_\w+)?\s*\(", text, re.MULTILINE))


def host_source_for_binary(host_dir: Path, host_binary: str) -> Path:
    return host_dir / f"{host_binary}.cpp"
