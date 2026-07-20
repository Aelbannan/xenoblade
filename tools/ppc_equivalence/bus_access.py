"""Access-family coverage instrumentation for opt-in ``memory_bus=`` execution.

Track B exports a stable API so Track A discharged obligations can attest that
every memory-touching opcode family either routed through the bus / SymbolicBus
path or produced an explicit unsupported / ``ExecutionInconclusive`` reject.

**Per-side coverage:** each ``execute_cfg(..., memory_bus=, bus_access_side=)``
run stores its dynamic snapshot under ``original`` or ``candidate``. Concrete
sampling overwrites only the last-run snapshot, not the per-side store.

**Static coverage:** ``static_opcode_families`` / ``static_access_coverage``
decode instruction lists without executing, so both sides' families appear in
attestations even when destinations are unobserved.
"""

from __future__ import annotations

import hashlib
from contextvars import ContextVar
from typing import Iterable, Literal, Sequence

from tools.ppc_equivalence.ir import Instruction, Opcode

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

BusAccessSide = Literal["original", "candidate"]
BUS_ACCESS_SIDES: tuple[BusAccessSide, ...] = ("original", "candidate")

# Well-known inconclusive / rejection reason prefixes (grep-friendly).
REASON_UNSUPPORTED_FAMILY_PREFIX = "memory-bus-unsupported-access-family:"
REASON_UNSUPPORTED_WIDTH_64 = "memory-bus-unsupported-mmio-width-64"
REASON_PSQ_MMIO = f"{REASON_UNSUPPORTED_FAMILY_PREFIX}psq-mmio"
REASON_DCBZ_MMIO_SPAN = f"{REASON_UNSUPPORTED_FAMILY_PREFIX}dcbz-mmio"
REASON_EABI_HELPER_WIDTH_64_ATOMIC = (
    f"{REASON_UNSUPPORTED_FAMILY_PREFIX}eabi-helper-atomic-64"
)

_INTEGER_OPCODES: frozenset[Opcode] = frozenset({
    Opcode.LBZ, Opcode.LBZU, Opcode.LHZ, Opcode.LHZU, Opcode.LHA, Opcode.LHAU,
    Opcode.LWZ, Opcode.LWZU,
    Opcode.LBZX, Opcode.LBZUX, Opcode.LHZX, Opcode.LHZUX, Opcode.LHAX, Opcode.LHAUX,
    Opcode.LWZX, Opcode.LWZUX, Opcode.LHBRX, Opcode.LWBRX,
    Opcode.STB, Opcode.STBU, Opcode.STH, Opcode.STHU, Opcode.STW, Opcode.STWU,
    Opcode.STBX, Opcode.STBUX, Opcode.STHX, Opcode.STHUX, Opcode.STWX, Opcode.STWUX,
    Opcode.STHBRX, Opcode.STWBRX,
})
_SCALAR_FP_OPCODES: frozenset[Opcode] = frozenset({
    Opcode.LFS, Opcode.LFSU, Opcode.LFD, Opcode.LFDU,
    Opcode.LFSX, Opcode.LFSUX, Opcode.LFDX, Opcode.LFDUX,
    Opcode.STFS, Opcode.STFSU, Opcode.STFD, Opcode.STFDU,
    Opcode.STFSX, Opcode.STFSUX, Opcode.STFDX, Opcode.STFDUX, Opcode.STFIWX,
})
_PSQ_OPCODES: frozenset[Opcode] = frozenset({
    Opcode.PSQ_L, Opcode.PSQ_LU, Opcode.PSQ_ST, Opcode.PSQ_STU,
    Opcode.PSQ_LX, Opcode.PSQ_LUX, Opcode.PSQ_STX, Opcode.PSQ_STUX,
})
_LMW_OPCODES: frozenset[Opcode] = frozenset({Opcode.LMW, Opcode.STMW})
_DCBZ_OPCODES: frozenset[Opcode] = frozenset({Opcode.DCBZ, Opcode.DCBZ_L})

_OPCODE_TO_FAMILY: dict[Opcode, str] = {
    **{op: BUS_ACCESS_FAMILY_INTEGER for op in _INTEGER_OPCODES},
    **{op: BUS_ACCESS_FAMILY_SCALAR_FP for op in _SCALAR_FP_OPCODES},
    **{op: BUS_ACCESS_FAMILY_PSQ for op in _PSQ_OPCODES},
    **{op: BUS_ACCESS_FAMILY_LMW for op in _LMW_OPCODES},
    **{op: BUS_ACCESS_FAMILY_DCBZ for op in _DCBZ_OPCODES},
}

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
_SIDE_BUS_ACCESS_COVERAGE: ContextVar[dict[str, dict[str, object]] | None] = ContextVar(
    "ppc_equivalence_side_bus_access_coverage",
    default=None,
)


def family_for_opcode(opcode: Opcode | str) -> str | None:
    """Return the bus-access family for a memory-touching opcode, else None."""
    if isinstance(opcode, str):
        try:
            opcode = Opcode(opcode)
        except ValueError:
            return None
    return _OPCODE_TO_FAMILY.get(opcode)


def static_opcode_families(instructions: Sequence[Instruction] | None) -> list[str]:
    """Sorted unique access families present in a decoded instruction list."""
    if not instructions:
        return []
    families: set[str] = set()
    for insn in instructions:
        family = family_for_opcode(insn.opcode)
        if family is not None:
            families.add(family)
    return sorted(families)


def static_access_coverage(
    instructions: Sequence[Instruction] | None,
) -> dict[str, object]:
    """Static decoded-opcode coverage snapshot (no execution required)."""
    families = static_opcode_families(instructions)
    return {
        "families": families,
        "opcode_families": families,
        "rejections": [],
        "source": "static",
        "access_coverage_sha256": access_coverage_digest(families, ()),
    }


def begin_bus_access_coverage() -> None:
    """Start recording families/rejects for a ``memory_bus=`` CFG run."""
    _BUS_ACCESS_FAMILIES.set(set())
    _BUS_ACCESS_REJECTIONS.set([])


def end_bus_access_coverage(
    *,
    side: BusAccessSide | None = None,
) -> dict[str, object]:
    """Stop recording and return a coverage snapshot for obligation builders.

    When ``side`` is ``original`` or ``candidate``, also store the snapshot in
    the per-side map (does not clear the other side).
    """
    families = frozenset(_BUS_ACCESS_FAMILIES.get() or ())
    rejections = tuple(_BUS_ACCESS_REJECTIONS.get() or ())
    _BUS_ACCESS_FAMILIES.set(None)
    _BUS_ACCESS_REJECTIONS.set(None)
    snapshot: dict[str, object] = {
        "families": sorted(families),
        "opcode_families": sorted(families),
        "rejections": list(rejections),
        "source": "dynamic",
        "access_coverage_sha256": access_coverage_digest(families, rejections),
    }
    _LAST_BUS_ACCESS_COVERAGE.set(snapshot)
    if side is not None:
        store_side_bus_access_coverage(side, snapshot)
    return snapshot


def last_bus_access_coverage() -> dict[str, object] | None:
    """Most recent ``end_bus_access_coverage`` snapshot (single-run / tests)."""
    return _LAST_BUS_ACCESS_COVERAGE.get()


def clear_side_bus_access_coverage() -> None:
    """Reset per-side dynamic snapshots (call at proof start)."""
    _SIDE_BUS_ACCESS_COVERAGE.set({})


def store_side_bus_access_coverage(
    side: BusAccessSide,
    snapshot: dict[str, object] | None = None,
) -> dict[str, object]:
    """Persist a dynamic coverage snapshot for ``side``.

    When ``snapshot`` is omitted, copies ``last_bus_access_coverage()``.
    Sampling that only runs ``execute_cfg`` without ``bus_access_side`` must
    not call this — otherwise it would erase the symbolic CFG sides.
    """
    if side not in BUS_ACCESS_SIDES:
        raise ValueError(f"unknown bus access side {side!r}")
    snap = snapshot if snapshot is not None else last_bus_access_coverage()
    if snap is None:
        snap = {
            "families": [],
            "opcode_families": [],
            "rejections": [],
            "source": "dynamic",
            "access_coverage_sha256": access_coverage_digest((), ()),
        }
    bucket = _SIDE_BUS_ACCESS_COVERAGE.get()
    if bucket is None:
        bucket = {}
        _SIDE_BUS_ACCESS_COVERAGE.set(bucket)
    stored = dict(snap)
    stored.setdefault("opcode_families", list(stored.get("families") or []))
    bucket[side] = stored
    return stored


def side_bus_access_coverage(side: BusAccessSide) -> dict[str, object] | None:
    """Dynamic coverage snapshot captured for ``side``, if any."""
    bucket = _SIDE_BUS_ACCESS_COVERAGE.get()
    if not bucket:
        return None
    snap = bucket.get(side)
    return dict(snap) if snap is not None else None


def per_side_bus_access_coverage() -> dict[str, dict[str, object] | None]:
    """Return ``{original: snap|None, candidate: snap|None}``."""
    return {
        "original": side_bus_access_coverage("original"),
        "candidate": side_bus_access_coverage("candidate"),
    }


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
