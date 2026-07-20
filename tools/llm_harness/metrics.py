"""Phase timing helpers for LLM harness experiments.

Phases tracked (when instrumented):

- ``llm`` — provider invoke / model generation
- ``configure`` — worktree ``configure.py``
- ``ninja`` — object compile
- ``objdiff`` — symbol-local objdiff scoring
- ``smt`` — PPC equivalence / SMT probe
- ``worktree_create`` — git worktree + dirty overlay

Most phases are recorded from adapter/evaluation hooks. When those hooks are
absent, ``Harness._finalize_solve`` falls back to aggregating each record's
``duration_seconds`` under ``llm`` (provider wall time dominates that field
today). Adapter hooks (``ninja`` / ``objdiff`` / ``smt``) fire when the harness
binds ``adapter.timings`` before ``evaluate()``.
"""

from __future__ import annotations

import threading
import time
from contextlib import contextmanager
from typing import Any, Dict, Iterator, List, Mapping, Optional

PHASES = (
    "llm",
    "configure",
    "ninja",
    "objdiff",
    "smt",
    "worktree_create",
)


class TimingRecorder:
    """Thread-safe accumulator for named phase durations."""

    def __init__(self, phases: tuple[str, ...] = PHASES) -> None:
        self.phases = phases
        self._lock = threading.Lock()
        self._totals: Dict[str, float] = {name: 0.0 for name in phases}
        self._counts: Dict[str, int] = {name: 0 for name in phases}

    def add(self, phase: str, seconds: float) -> None:
        if seconds < 0:
            seconds = 0.0
        with self._lock:
            if phase not in self._totals:
                self._totals[phase] = 0.0
                self._counts[phase] = 0
            self._totals[phase] += float(seconds)
            self._counts[phase] += 1

    @contextmanager
    def measure(self, phase: str) -> Iterator[None]:
        started = time.monotonic()
        try:
            yield
        finally:
            self.add(phase, time.monotonic() - started)

    def summary(self) -> Dict[str, Any]:
        with self._lock:
            totals = dict(self._totals)
            counts = dict(self._counts)
        return {
            "totals_seconds": totals,
            "counts": counts,
            "total_seconds": sum(totals.values()),
        }

    def merge_into(self, destination: Dict[str, Any]) -> Dict[str, Any]:
        """Write ``summary()`` under ``destination['timings']`` and return it."""
        payload = self.summary()
        destination["timings"] = payload
        return payload


def timings_from_records(
    records: List[Mapping[str, Any]],
    *,
    llm_key: str = "duration_seconds",
) -> Dict[str, Any]:
    """Fallback summary when per-phase hooks were not recorded.

    Maps each experiment record's wall duration into the ``llm`` bucket.
    Other phases remain zero so dashboards can still render a stable shape.
    """
    recorder = TimingRecorder()
    for row in records:
        seconds = float(row.get(llm_key) or 0.0)
        if seconds > 0:
            recorder.add("llm", seconds)
    return recorder.summary()


def merge_timing_dicts(*parts: Optional[Mapping[str, Any]]) -> Dict[str, Any]:
    """Combine several ``summary()``-shaped dicts by summing totals/counts."""
    recorder = TimingRecorder()
    for part in parts:
        if not part:
            continue
        totals = part.get("totals_seconds") or {}
        counts = part.get("counts") or {}
        for phase, seconds in totals.items():
            seconds_f = float(seconds or 0.0)
            count = int(counts.get(phase) or 0)
            if seconds_f <= 0 and count <= 0:
                continue
            if count <= 0:
                recorder.add(str(phase), seconds_f)
                continue
            per = seconds_f / count
            for _ in range(count):
                recorder.add(str(phase), per)
    return recorder.summary()
