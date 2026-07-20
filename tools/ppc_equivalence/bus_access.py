"""Access-family coverage instrumentation for opt-in ``memory_bus=`` execution.

Track B exports a stable API so Track A discharged obligations can attest that
every memory-touching opcode family either routed through the bus / SymbolicBus
path or produced an explicit unsupported / ``ExecutionInconclusive`` reject.

Families are recorded into a context var while ``execute_cfg(..., memory_bus=)``
runs. Vacuous unsupported-access discharge must not treat a CFG with missing
family coverage as complete.
"""

from __future__ import annotations

import hashlib
from contextvars import ContextVar
from typing import Iterable

# Stable family ids (obligation ``access_coverage.families`` / digests).
BUS_ACCESS_FAMILY_INTEGER = "integer-load-store"
BUS_ACCESS_FAMILY_SCALAR_FP = "scalar-fp-load-store"
BUS_ACCESS_FAMILY_PSQ = "psq-load-store"
BUS_ACCESS_FAMILY_LMW = "lmw-stmw"
BUS_ACCESS_FAMILY_DCBZ = "dcbz"
BUS_ACCESS_FAMILY_EABI_HELPER = "fixed-eabi-save-restore"

BUS_ACCESS_FAMILIES: frozenset[str] = frozenset({
    BUS_ACCESS_FAMILY_INTEGER,
    BUS_ACCESS_FAMILY_SCALAR_FP,
    BUS_ACCESS_FAMILY_PSQ,
    BUS_ACCESS_FAMILY_LMW,
    BUS_ACCESS_FAMILY_DCBZ,
    BUS_ACCESS_FAMILY_EABI_HELPER,
})

# Well-known inconclusive / rejection reason prefixes (grep-friendly).
REASON_UNSUPPORTED_FAMILY_PREFIX = "memory-bus-unsupported-access-family:"
REASON_UNSUPPORTED_WIDTH_64 = "memory-bus-unsupported-mmio-width-64"
REASON_PSQ_MMIO = f"{REASON_UNSUPPORTED_FAMILY_PREFIX}psq-mmio"
REASON_DCBZ_MMIO_SPAN = f"{REASON_UNSUPPORTED_FAMILY_PREFIX}dcbz-mmio"
REASON_EABI_HELPER_WIDTH_64_ATOMIC = (
    f"{REASON_UNSUPPORTED_FAMILY_PREFIX}eabi-helper-atomic-64"
)

_BUS_ACCESS_FAMILIES: ContextVar[set[str] | None] = ContextVar(
    "ppc_equivalence_bus_access_families",
    default=None,
)
_BUS_ACCESS_REJECTIONS: ContextVar[list[str] | None] = ContextVar(
    "ppc_equivalence_bus_access_rejections",
    default=None,
)
_LAST_BUS_ACCESS_COVERAGE: ContextVar[dict[str, object] | None] = ContextVar(
    "ppc_equivalence_last_bus_access_coverage",
    default=None,
)


def begin_bus_access_coverage() -> None:
    """Start recording families/rejects for a ``memory_bus=`` CFG run."""
    _BUS_ACCESS_FAMILIES.set(set())
    _BUS_ACCESS_REJECTIONS.set([])


def end_bus_access_coverage() -> dict[str, object]:
    """Stop recording and return a coverage snapshot for obligation builders."""
    families = frozenset(_BUS_ACCESS_FAMILIES.get() or ())
    rejections = tuple(_BUS_ACCESS_REJECTIONS.get() or ())
    _BUS_ACCESS_FAMILIES.set(None)
    _BUS_ACCESS_REJECTIONS.set(None)
    snapshot: dict[str, object] = {
        "families": sorted(families),
        "rejections": list(rejections),
        "access_coverage_sha256": access_coverage_digest(families, rejections),
    }
    _LAST_BUS_ACCESS_COVERAGE.set(snapshot)
    return snapshot


def last_bus_access_coverage() -> dict[str, object] | None:
    """Most recent ``end_bus_access_coverage`` snapshot (Track A / engine)."""
    return _LAST_BUS_ACCESS_COVERAGE.get()



def record_bus_access_family(family: str) -> None:
    """Record that ``family`` executed under an active memory bus.

    No-op when coverage is not armed (no ``memory_bus=`` / default proofs).
    """
    bucket = _BUS_ACCESS_FAMILIES.get()
    if bucket is None:
        return
    if family not in BUS_ACCESS_FAMILIES:
        raise ValueError(f"unknown bus access family {family!r}")
    bucket.add(family)


def record_bus_access_rejection(reason: str) -> None:
    """Record an explicit MMIO / family reject reason for coverage attestation."""
    bucket = _BUS_ACCESS_REJECTIONS.get()
    if bucket is None:
        return
    if reason not in bucket:
        bucket.append(reason)


def observed_bus_access_families() -> frozenset[str]:
    """Families observed so far in the current armed coverage window."""
    return frozenset(_BUS_ACCESS_FAMILIES.get() or ())


def observed_bus_access_rejections() -> tuple[str, ...]:
    return tuple(_BUS_ACCESS_REJECTIONS.get() or ())


def access_coverage_digest(
    families: Iterable[str] | None = None,
    rejections: Iterable[str] | None = None,
) -> str:
    """SHA-256 over sorted families + rejection reasons (Track A vacuous attest)."""
    fam = sorted(families if families is not None else observed_bus_access_families())
    rej = list(rejections if rejections is not None else observed_bus_access_rejections())
    payload = "families=" + ",".join(fam) + ";rejections=" + ",".join(rej)
    return hashlib.sha256(payload.encode("utf-8")).hexdigest()


def unsupported_family_reason(family_suffix: str) -> str:
    """Build a well-known ``ExecutionInconclusive`` reason for a family reject."""
    return f"{REASON_UNSUPPORTED_FAMILY_PREFIX}{family_suffix}"
