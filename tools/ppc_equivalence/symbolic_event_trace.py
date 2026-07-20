"""Bounded GX FIFO symbolic event trace (PR 15 / Track C).

Records a bounded sequence of MMIO writes (count / addr / width / value).
Reads remain unsupported. Rejects unbounded symbolic-loop FIFO emission when
the trip count is not provably within the bound.

CFG writes are emitted via ``apply_symbolic_bus_access`` (concrete FIFO
addresses). Terminal compare uses ``SymbolicEventTrace.compare_equal``.
Concrete ``GxFifoStreamDevice`` routing is unchanged. ``memory-bus`` may
authorize ``EQUIVALENT`` only under engine-generated ``status=discharged``
obligations (same gate as ``bus_spec`` / ``memory_bus_obligations``).
"""

from __future__ import annotations

from dataclasses import dataclass
from enum import Enum
from typing import Any

from tools.ppc_equivalence.bus_spec import DeviceSpecification
from tools.ppc_equivalence.provenance import canonical_json_sha256

ALGORITHM = "gxfifo-stream-trace-v1"
THEORY = "gxfifo-stream"
DEFAULT_MAX_FIFO_EVENTS = 256

__all__ = [
    "ALGORITHM",
    "THEORY",
    "DEFAULT_MAX_FIFO_EVENTS",
    "SymbolicFifoWrite",
    "SymbolicEventTrace",
    "SymbolicTraceCompareResult",
    "UnboundedEmissionStatus",
    "initial_symbolic_event_trace",
    "symbolic_trace_from_device_spec",
    "reject_unbounded_symbolic_loop_fifo_emission",
    "build_gxfifo_trace_obligation",
]


@dataclass(frozen=True, slots=True)
class SymbolicFifoWrite:
    """One bounded FIFO write event."""

    addr: Any
    width: int
    value: Any


@dataclass(frozen=True, slots=True)
class SymbolicEventTrace:
    """Bounded write-stream trace for one ``gxfifo-stream`` device."""

    device_id: str
    base: int
    span: int
    max_events: int
    writes: tuple[SymbolicFifoWrite, ...] = ()

    def append_write(
        self,
        addr: Any,
        width: int,
        value: Any,
    ) -> tuple[SymbolicEventTrace | None, str | None]:
        """Append one write; return ``(next_trace, rejection_reason)``."""
        if width not in (1, 2, 4):
            return None, "unsupported-width"
        if len(self.writes) >= self.max_events:
            return None, "bounded-event-limit"
        event = SymbolicFifoWrite(addr=addr, width=width, value=value)
        return (
            SymbolicEventTrace(
                device_id=self.device_id,
                base=self.base,
                span=self.span,
                max_events=self.max_events,
                writes=self.writes + (event,),
            ),
            None,
        )

    @staticmethod
    def read_supported() -> bool:
        """GX FIFO reads are intentionally unsupported in this scaffold."""
        return False

    def compare_equal(
        self,
        other: SymbolicEventTrace,
        z3: Any,
    ) -> SymbolicTraceCompareResult:
        """Extensional equality on write count and per-event addr/width/value."""
        if self.device_id != other.device_id:
            return SymbolicTraceCompareResult(
                equal=z3.BoolVal(False),
                supported=False,
                reason="device-id-mismatch",
            )
        if len(self.writes) != len(other.writes):
            return SymbolicTraceCompareResult(
                equal=z3.BoolVal(False),
                supported=True,
                reason=None,
            )
        if not self.writes:
            return SymbolicTraceCompareResult(
                equal=z3.BoolVal(True),
                supported=True,
                reason=None,
            )
        terms: list[Any] = []
        for left, right in zip(self.writes, other.writes):
            if left.width != right.width:
                return SymbolicTraceCompareResult(
                    equal=z3.BoolVal(False),
                    supported=True,
                    reason=None,
                )
            terms.append(left.addr == right.addr)
            terms.append(left.value == right.value)
        return SymbolicTraceCompareResult(
            equal=z3.And(*terms),
            supported=True,
            reason=None,
        )


@dataclass(frozen=True, slots=True)
class SymbolicTraceCompareResult:
    """Outcome of comparing two bounded symbolic FIFO traces."""

    equal: Any
    supported: bool
    reason: str | None = None


class UnboundedEmissionStatus(str, Enum):
    ALLOWED = "allowed"
    REJECTED_SYMBOLIC_TRIP = "rejected-symbolic-trip"
    REJECTED_BOUND_EXCEEDED = "rejected-bound-exceeded"


def initial_symbolic_event_trace(
    *,
    device_id: str,
    base: int,
    span: int,
    max_events: int = DEFAULT_MAX_FIFO_EVENTS,
) -> SymbolicEventTrace:
    if max_events < 1:
        raise ValueError("max_events must be >= 1")
    if span <= 0:
        raise ValueError("span must be positive")
    return SymbolicEventTrace(
        device_id=device_id,
        base=base,
        span=span,
        max_events=max_events,
    )


def symbolic_trace_from_device_spec(
    device: DeviceSpecification,
    *,
    max_events: int = DEFAULT_MAX_FIFO_EVENTS,
) -> SymbolicEventTrace:
    if device.theory != THEORY:
        raise TypeError(
            f"symbolic FIFO trace requires theory {THEORY!r}, got {device.theory!r}"
        )
    return initial_symbolic_event_trace(
        device_id=device.device_id,
        base=device.base,
        span=device.span,
        max_events=max_events,
    )


def _trip_count_as_int(trip_count: Any, z3: Any) -> int | None:
    simplified = z3.simplify(trip_count)
    if hasattr(simplified, "as_long"):
        try:
            return int(simplified.as_long())
        except Exception:
            return None
    return None


def reject_unbounded_symbolic_loop_fifo_emission(
    *,
    trip_count: Any,
    writes_per_trip: int,
    max_events: int,
    z3: Any,
) -> tuple[UnboundedEmissionStatus, str | None]:
    """Reject FIFO emission when loop trip count is symbolic or exceeds the bound."""
    if writes_per_trip < 1:
        raise ValueError("writes_per_trip must be >= 1")
    if max_events < 1:
        raise ValueError("max_events must be >= 1")

    concrete = _trip_count_as_int(trip_count, z3)
    if concrete is None:
        return UnboundedEmissionStatus.REJECTED_SYMBOLIC_TRIP, "symbolic-loop-fifo-emission"
    projected = concrete * writes_per_trip
    if projected > max_events:
        return (
            UnboundedEmissionStatus.REJECTED_BOUND_EXCEEDED,
            f"projected-writes={projected}>max={max_events}",
        )
    return UnboundedEmissionStatus.ALLOWED, None


def build_gxfifo_trace_obligation(
    *,
    bus_spec_sha256: str,
    devices: list[dict[str, Any]],
    status: str = "scaffolded",
) -> dict[str, Any]:
    """Build the PR-15 obligation block shape (not yet promotion-discharged)."""
    return {
        "schema_version": 1,
        "algorithm": ALGORITHM,
        "status": status,
        "bus_spec_sha256": bus_spec_sha256,
        "devices": devices,
        "reads": "unsupported",
    }
