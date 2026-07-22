"""Bounded GX FIFO symbolic event trace (PR 15 / Track C, v2).

Records a bounded sequence of MMIO events (concrete reads/writes and summarized
``RepeatedEmission`` segments). Reads remain unsupported at the bus layer.
Rejects unbounded symbolic-loop FIFO emission when the trip count is not
provably within the bound.

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

ALGORITHM_V1 = "gxfifo-stream-trace-v1"
ALGORITHM_V2 = "gxfifo-stream-trace-v2"
ALGORITHM = ALGORITHM_V2
THEORY = "gxfifo-stream"
DEFAULT_MAX_FIFO_EVENTS = 256

TraceSegment = "SymbolicFifoEvent | RepeatedEmission"

__all__ = [
    "ALGORITHM",
    "ALGORITHM_V1",
    "ALGORITHM_V2",
    "THEORY",
    "DEFAULT_MAX_FIFO_EVENTS",
    "RepeatedEmission",
    "SymbolicFifoEvent",
    "SymbolicFifoWrite",
    "SymbolicEventTrace",
    "SymbolicTraceCompareResult",
    "UnboundedEmissionStatus",
    "compare_repeated_extensional",
    "initial_symbolic_event_trace",
    "symbolic_trace_from_device_spec",
    "reject_unbounded_symbolic_loop_fifo_emission",
    "build_gxfifo_trace_obligation",
]


@dataclass(frozen=True, slots=True)
class SymbolicFifoEvent:
    """One bounded FIFO read or write event."""

    kind: str  # "read" | "write"
    addr: Any
    width: int
    value: Any
    event_index: int | None = None


@dataclass(frozen=True, slots=True)
class SymbolicFifoWrite:
    """One bounded FIFO write event (legacy write-only view)."""

    addr: Any
    width: int
    value: Any

    @classmethod
    def from_event(cls, event: SymbolicFifoEvent) -> SymbolicFifoWrite:
        if event.kind != "write":
            raise ValueError("SymbolicFifoWrite requires kind=write")
        return cls(addr=event.addr, width=event.width, value=event.value)


@dataclass(frozen=True, slots=True)
class RepeatedEmission:
    """Summarized CTR-loop FIFO write emission (one iteration template group)."""

    count: Any
    writes_per_iteration: int
    writes: tuple[SymbolicFifoEvent, ...]


@dataclass(frozen=True, slots=True)
class SymbolicEventTrace:
    """Bounded event-stream trace for one ``gxfifo-stream`` device."""

    device_id: str
    base: int
    span: int
    max_events: int
    events: tuple[SymbolicFifoEvent | RepeatedEmission, ...] = ()

    @property
    def writes(self) -> tuple[SymbolicFifoWrite, ...]:
        """Flatten explicit write events only (not repeated segments)."""
        out: list[SymbolicFifoWrite] = []
        for segment in self.events:
            if isinstance(segment, SymbolicFifoEvent) and segment.kind == "write":
                out.append(SymbolicFifoWrite.from_event(segment))
        return tuple(out)

    def event_count(self) -> int | None:
        """Total expanded event count when every segment count is concrete."""
        total = 0
        for segment in self.events:
            if isinstance(segment, SymbolicFifoEvent):
                total += 1
                continue
            concrete = _concrete_int_or_none(segment.count)
            if concrete is None:
                return None
            total += concrete * segment.writes_per_iteration
        return total

    def append_write(
        self,
        addr: Any,
        width: int,
        value: Any,
        *,
        event_index: int | None = None,
    ) -> tuple[SymbolicEventTrace | None, str | None]:
        """Append one write; return ``(next_trace, rejection_reason)``."""
        if width not in (1, 2, 4):
            return None, "unsupported-width"
        current = self.event_count()
        if current is not None and current >= self.max_events:
            return None, "bounded-event-limit"
        event = SymbolicFifoEvent(
            kind="write",
            addr=addr,
            width=width,
            value=value,
            event_index=event_index,
        )
        return (
            SymbolicEventTrace(
                device_id=self.device_id,
                base=self.base,
                span=self.span,
                max_events=self.max_events,
                events=self.events + (event,),
            ),
            None,
        )

    def append_read(
        self,
        addr: Any,
        width: int,
        value: Any,
        *,
        event_index: int | None = None,
    ) -> tuple[SymbolicEventTrace | None, str | None]:
        """Reads are unsupported; do not append."""
        del addr, width, value, event_index
        return None, "gxfifo-read-unsupported"

    def append_repeated_emission(
        self,
        segment: RepeatedEmission,
    ) -> tuple[SymbolicEventTrace | None, str | None]:
        """Append a summarized loop segment after bound check."""
        if segment.writes_per_iteration < 1:
            return None, "writes-per-iteration-invalid"
        if len(segment.writes) != segment.writes_per_iteration:
            return None, "writes-per-iteration-mismatch"
        for write in segment.writes:
            if write.kind != "write":
                return None, "repeated-emission-write-only"
            if write.width not in (1, 2, 4):
                return None, "unsupported-width"
        concrete_count = _concrete_int_or_none(segment.count)
        if concrete_count is None:
            return None, "symbolic-loop-fifo-emission"
        current = self.event_count() or 0
        projected = current + concrete_count * segment.writes_per_iteration
        if projected > self.max_events:
            return None, "bounded-event-limit"
        return (
            SymbolicEventTrace(
                device_id=self.device_id,
                base=self.base,
                span=self.span,
                max_events=self.max_events,
                events=self.events + (segment,),
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
        """Extensional equality on segment structure and per-event fields."""
        if self.device_id != other.device_id:
            return SymbolicTraceCompareResult(
                equal=z3.BoolVal(False),
                supported=False,
                reason="device-id-mismatch",
            )
        if len(self.events) != len(other.events):
            return SymbolicTraceCompareResult(
                equal=z3.BoolVal(False),
                supported=True,
                reason=None,
            )
        if not self.events:
            return SymbolicTraceCompareResult(
                equal=z3.BoolVal(True),
                supported=True,
                reason=None,
            )
        terms: list[Any] = []
        for left, right in zip(self.events, other.events):
            if isinstance(left, SymbolicFifoEvent) and isinstance(right, SymbolicFifoEvent):
                terms.extend(_compare_concrete_event_terms(left, right, z3))
            elif isinstance(left, RepeatedEmission) and isinstance(right, RepeatedEmission):
                repeated = compare_repeated_extensional(
                    left,
                    right,
                    z3,
                    device_id=self.device_id,
                )
                if repeated is None:
                    return SymbolicTraceCompareResult(
                        equal=z3.BoolVal(False),
                        supported=True,
                        reason=None,
                    )
                terms.append(repeated)
            else:
                return SymbolicTraceCompareResult(
                    equal=z3.BoolVal(False),
                    supported=True,
                    reason=None,
                )
        return SymbolicTraceCompareResult(
            equal=z3.And(*terms) if terms else z3.BoolVal(True),
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


def _concrete_int_or_none(value: Any) -> int | None:
    if isinstance(value, bool):
        return int(value)
    if isinstance(value, int):
        return value
    if hasattr(value, "as_long"):
        try:
            return int(value.as_long())
        except Exception:
            return None
    return None


def _compare_concrete_event_terms(
    left: SymbolicFifoEvent,
    right: SymbolicFifoEvent,
    z3: Any,
) -> list[Any]:
    if left.kind != right.kind:
        return [z3.BoolVal(False)]
    if left.width != right.width:
        return [z3.BoolVal(False)]
    terms: list[Any] = [
        left.addr == right.addr,
        left.value == right.value,
    ]
    if left.event_index is not None or right.event_index is not None:
        if left.event_index != right.event_index:
            return [z3.BoolVal(False)]
    return terms


def compare_repeated_extensional(
    left: RepeatedEmission,
    right: RepeatedEmission,
    z3: Any,
    *,
    device_id: str,
) -> Any | None:
    """Compare two repeated segments with invariant per-iteration templates.

    For v2, templates do not depend on iteration index ``k``. Equality is
    ``count ∧ writes_per_iteration ∧ template fields``. When counts are
    symbolic, ``count`` terms are emitted as Z3 equalities.

    Returns ``None`` when structure is statically incompatible (mismatched
    ``writes_per_iteration`` or template arity).
    """
    if left.writes_per_iteration != right.writes_per_iteration:
        return None
    if len(left.writes) != len(right.writes):
        return None
    terms: list[Any] = [left.count == right.count]
    for left_write, right_write in zip(left.writes, right.writes):
        if left_write.width != right_write.width:
            return None
        if left_write.kind != "write" or right_write.kind != "write":
            return None
        terms.append(left_write.addr == right_write.addr)
        terms.append(left_write.value == right_write.value)
    # Mismatch-index k is reserved for future k-dependent templates; document
    # that callers may see a free ``k`` in ``equal`` once affine values land.
    _ = z3.BitVec(f"fifo_mismatch_{device_id}", 32)
    return z3.And(*terms)


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
    """Build the GX FIFO trace obligation block (v2 schema)."""
    return {
        "schema_version": 2,
        "algorithm": ALGORITHM,
        "status": status,
        "bus_spec_sha256": bus_spec_sha256,
        "devices": devices,
        "reads": {"policy": "unsupported", "reason": "gxfifo-read-unsupported"},
    }
