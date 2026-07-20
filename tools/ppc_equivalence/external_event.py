"""External nondeterminism scaffolding for MMIO reads (Wave 3).

Timer / status / input registers must not be treated as fixed constants.
Observable device reads consume an explicit event stream:

    value = ReadOracle(device_id, event_index, device_state)

Sharing rule (see SOUNDNESS.md):
- The same physical external event may be shared only when both sides perform
  the same logical read at the same event index under equivalent device state.
- Additional or reordered reads advance ``event_index`` and consume distinct
  oracle answers — original and candidate must not silently receive the same
  value after different read histories.
"""

from __future__ import annotations

from abc import ABC, abstractmethod
from dataclasses import dataclass, field
from typing import Any, Mapping


@dataclass
class ExternalEventCursor:
    """Per-device monotonic event index for observable reads."""

    counters: dict[str, int] = field(default_factory=dict)

    def next_index(self, device_id: str) -> int:
        index = self.counters.get(device_id, 0)
        self.counters[device_id] = index + 1
        return index

    def peek(self, device_id: str) -> int:
        return self.counters.get(device_id, 0)

    def clone(self) -> ExternalEventCursor:
        return ExternalEventCursor(counters=dict(self.counters))


class ReadOracle(ABC):
    """Oracle for externally nondeterministic MMIO read results."""

    @abstractmethod
    def read(
        self,
        device_id: str,
        event_index: int,
        device_state: Mapping[str, Any] | None = None,
    ) -> Any:
        """Return the value observed for ``(device_id, event_index)``.

        ``device_state`` is the device's visible state *before* the read's
        side effects (read-clear / W1C apply after the oracle answer).
        """


@dataclass
class DeterministicReadOracle(ReadOracle):
    """Test oracle: fixed map of ``(device_id, event_index) -> value``."""

    values: dict[tuple[str, int], int] = field(default_factory=dict)
    default: int | None = None

    def read(
        self,
        device_id: str,
        event_index: int,
        device_state: Mapping[str, Any] | None = None,
    ) -> int:
        del device_state  # unused — state is for future symbolic oracles
        key = (device_id, event_index)
        if key in self.values:
            return self.values[key]
        if self.default is not None:
            return self.default
        raise KeyError(f"no external event for {device_id!r} index {event_index}")


@dataclass
class SymbolicReadOracle(ReadOracle):
    """Fresh symbolic bitvector per ``(device_id, event_index)`` (test scaffold)."""

    bit_width: int = 32
    _cache: dict[tuple[str, int], Any] = field(default_factory=dict, repr=False)

    def read(
        self,
        device_id: str,
        event_index: int,
        device_state: Mapping[str, Any] | None = None,
    ) -> Any:
        del device_state
        import z3

        key = (device_id, event_index)
        if key not in self._cache:
            name = f"ext_{device_id}_{event_index}"
            self._cache[key] = z3.BitVec(name, self.bit_width)
        return self._cache[key]


def shared_external_read_ok(
    *,
    original_device_id: str,
    candidate_device_id: str,
    original_event_index: int,
    candidate_event_index: int,
    original_device_state: Mapping[str, Any] | None,
    candidate_device_state: Mapping[str, Any] | None,
) -> bool:
    """True when both sides may share one oracle answer for this read.

    Requires identical device id, event index, and visible device state.
    Divergent histories must not share values.
    """
    if original_device_id != candidate_device_id:
        return False
    if original_event_index != candidate_event_index:
        return False
    return dict(original_device_state or {}) == dict(candidate_device_state or {})
