from __future__ import annotations

import time
from dataclasses import dataclass, field


class ProofDeadlineExceeded(Exception):
    def __init__(self, phase: str) -> None:
        self.phase = phase
        super().__init__(f"proof deadline exceeded during {phase}")


@dataclass(frozen=True)
class SolverPhase:
    name: str
    result: str
    elapsed_ms: float

    def to_dict(self) -> dict[str, str | float]:
        return {
            "name": self.name,
            "result": self.result,
            "elapsed_ms": self.elapsed_ms,
        }


@dataclass(frozen=True)
class Deadline:
    end_ns: int

    @classmethod
    def after_ms(cls, timeout_ms: int) -> Deadline:
        if timeout_ms <= 0:
            raise ValueError("timeout_ms must be positive")
        return cls(time.monotonic_ns() + timeout_ms * 1_000_000)

    def remaining_ms(self) -> int:
        remaining_ns = self.end_ns - time.monotonic_ns()
        if remaining_ns <= 0:
            return 0
        return max(1, (remaining_ns + 999_999) // 1_000_000)

    def require_time(self, phase: str) -> int:
        remaining = self.remaining_ms()
        if remaining <= 0:
            raise ProofDeadlineExceeded(phase)
        return remaining

    def expired(self) -> bool:
        return time.monotonic_ns() >= self.end_ns
