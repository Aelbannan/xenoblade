"""Constant-stride store loop recognition and closed-form discharge.

Typical MWCC counted-loop shapes::

    li / addi / addis+ori  rT, _, N
    mtctr                  rT
  header:
    stw / stwu / stb / sth  rS, disp(rB)   # D-form store
    addi       rB, rB, K                     # optional when not stwu
    bdnz       header

``stwu`` advances the base register by ``disp``; otherwise a single
``addi rB, rB, K`` on the store base register must match the store width
and equal the memory stride. Indexed stores and multi-store bodies are
rejected (prefer false negatives).

When ``execute_cfg`` is given a plan map (or the engine auto-builds one),
matching headers with a positive concrete trip count are applied by
recording typed ``StoreEffect`` writes (not memory alone) and advancing
base/CTR — but only under an explicit entry-CTR guard. Authorization of
``EQUIVALENT`` requires a schema-v2 side-specific obligation whose every
used plan discharges ``instructions ≡ summary`` refinement (see
``memory_loop_discharge``). Recognition, ``coverage=applied``, or v1
self-referential transition-equivalence never authorizes. Bounded-remainder
expansions stay ``applied`` until relational discharge lands.
"""

from __future__ import annotations

from collections.abc import Sequence
from dataclasses import dataclass, replace
from typing import Any

from tools.ppc_equivalence.bounded_remainder_loop import (
    BoundedRemainderTrip,
    apply_bounded_remainder_memory_loop,
    recover_bounded_remainder_trip,
)
from tools.ppc_equivalence.ctr_materialization import collect_lwz_readonly_addresses, recover_gpr_constant
from tools.ppc_equivalence.trip_expression import canonical_dict, recognize_trip_expr
from tools.ppc_equivalence.ir import Instruction, Opcode
from tools.ppc_equivalence.memory_semantics import (
    apply_memory_loop_transition,
    build_memory_loop_transition,
    footprint_ok_for_summary,
    footprint_wraps_u32,
)
from tools.ppc_equivalence.provenance import canonical_json_sha256

_CTR_SPR = 9
_BDNZ_BO = 16  # decrement CTR; branch if CTR != 0 after decrement

# Fail closed above this trip count: closed-form applies N explicit stores.
MAX_MEMORY_LOOP_TRIPS = 4096

# Per-side summary identity algorithm (embedded in summary_sha256).
_MEMORY_LOOP_SIDE_ALGORITHM = "constant-stride-store-v2"
# Top-level side-set obligation algorithm (schema v2).
_MEMORY_LOOP_SET_ALGORITHM = "constant-stride-store-set-v3"
_MEMORY_LOOP_SCHEMA_VERSION = 2

_STORE_WIDTH: dict[Opcode, int] = {
    Opcode.STB: 1,
    Opcode.STH: 2,
    Opcode.STW: 4,
    Opcode.STWU: 4,
}


@dataclass(frozen=True)
class ConstantStrideStoreLoop:
    """Recognized CTR counted loop whose body performs constant-stride stores."""

    header_pc: int
    latch_pc: int
    exit_pc: int
    mtctr_pc: int
    base_reg: int
    index_reg: int | None
    stride: int
    store_width: int
    source_reg: int
    store_kind: str  # "stwu" | "d-form-addi"
    trip_count: int | None
    trip_count_reg: int | None
    trip_expr: dict[str, Any] | None
    trip_upper_bound: int | None
    zero_guard: str | None
    confidence: str
    notes: tuple[str, ...]


@dataclass(frozen=True)
class MemoryLoopSummary:
    """Closed-form summary for a constant-stride CTR store loop."""

    header_pc: int
    latch_pc: int
    exit_pc: int
    trip_count: int
    base_reg: int
    source_reg: int
    stride: int
    store_width: int
    store_kind: str
    final_ctr: int
    ranking: str
    proof_kind: str
    invariant_notes: tuple[str, ...]
    trip_expr: dict[str, Any] | None = None
    trip_upper_bound: int | None = None
    zero_guard: str | None = None
    expansion: str = "closed-form"


@dataclass(frozen=True)
class MemoryLoopWitness:
    """Exact recognized instructions for a constant-stride store loop."""

    body: tuple[Instruction, ...]
    latch: Instruction
    mtctr: Instruction
    header_pc: int
    latch_pc: int
    exit_pc: int
    code_sha256: str
    recognized_trip_count: int


@dataclass(frozen=True)
class MemoryLoopPlan:
    """Paired summary + witness so refinement cannot drift from recognition."""

    summary: MemoryLoopSummary
    witness: MemoryLoopWitness

    @property
    def header_pc(self) -> int:
        return self.summary.header_pc

    @property
    def latch_pc(self) -> int:
        return self.summary.latch_pc

    @property
    def exit_pc(self) -> int:
        return self.summary.exit_pc

    @property
    def expansion(self) -> str:
        return self.summary.expansion

    @property
    def trip_count(self) -> int:
        return self.summary.trip_count


def instruction_semantic_identity(insn: Instruction) -> dict[str, Any]:
    """Canonical semantic identity for one decoded instruction."""
    payload: dict[str, Any] = {
        "address": int(insn.address) & 0xFFFFFFFF,
        "raw": int(insn.raw) & 0xFFFFFFFF,
        "opcode": insn.opcode.value,
        "operands": [int(v) for v in insn.operands],
        "record": bool(insn.record),
        "overflow": bool(insn.overflow),
        "link": bool(insn.link),
    }
    relocation = insn.relocation
    if relocation is not None:
        payload["relocation"] = {
            "offset": int(relocation.offset),
            "type": int(relocation.relocation_type),
            "symbol": relocation.symbol,
            "canonical_symbol": relocation.canonical_symbol,
            "addend": int(relocation.addend),
        }
    return payload


def compute_witness_code_sha256(
    body: Sequence[Instruction],
    latch: Instruction,
    mtctr: Instruction,
) -> str:
    """SHA-256 over the exact semantic instruction identity of a witness."""
    return canonical_json_sha256(
        {
            "mtctr": instruction_semantic_identity(mtctr),
            "body": [instruction_semantic_identity(insn) for insn in body],
            "latch": instruction_semantic_identity(latch),
        },
    )


def compute_summary_identity_sha256(summary: MemoryLoopSummary) -> str:
    """SHA-256 over the closed-form summary identity (not the witness)."""
    payload: dict[str, Any] = {
        "proof_kind": summary.proof_kind,
        "header_pc": summary.header_pc,
        "latch_pc": summary.latch_pc,
        "exit_pc": summary.exit_pc,
        "trip_count": summary.trip_count,
        "base_reg": summary.base_reg,
        "source_reg": summary.source_reg,
        "stride": summary.stride,
        "store_width": summary.store_width,
        "store_kind": summary.store_kind,
        "final_ctr": summary.final_ctr,
        "ranking": summary.ranking,
        "algorithm": _MEMORY_LOOP_SIDE_ALGORITHM,
        "effects": "typed-store",
        "footprint": "ok",
        "expansion": summary.expansion,
    }
    if summary.trip_expr is not None:
        payload["trip_expr"] = dict(summary.trip_expr)
    if summary.trip_upper_bound is not None:
        payload["trip_upper_bound"] = int(summary.trip_upper_bound)
    if summary.zero_guard is not None:
        payload["zero_guard"] = summary.zero_guard
    return canonical_json_sha256(payload)


def plan_identity_key(plan: MemoryLoopPlan) -> tuple[int, int, str, str]:
    """Canonical unique key for a used plan on one CFG side."""
    return (
        int(plan.summary.header_pc) & 0xFFFFFFFF,
        int(plan.summary.latch_pc) & 0xFFFFFFFF,
        compute_summary_identity_sha256(plan.summary),
        plan.witness.code_sha256,
    )


def find_constant_stride_store_loops(
    instructions: Sequence[Instruction],
    *,
    readonly_words: dict[int, int] | None = None,
) -> list[ConstantStrideStoreLoop]:
    """Scan for ``mtctr`` / store body / ``bdnz`` constant-stride store loops."""
    if not instructions:
        return []

    by_address = {insn.address: index for index, insn in enumerate(instructions)}
    loops: list[ConstantStrideStoreLoop] = []

    for index, insn in enumerate(instructions):
        if not _is_bdnz(insn):
            continue
        target = int(insn.operands[2]) & 0xFFFFFFFC
        header_index = by_address.get(target)
        if header_index is None or header_index >= index:
            continue

        body = list(instructions[header_index:index])
        parsed, body_notes = _parse_constant_stride_store_body(body)
        if parsed is None:
            continue

        mtctr_index = header_index - 1
        if mtctr_index < 0:
            continue
        mtctr = instructions[mtctr_index]
        if not _is_mtctr(mtctr):
            continue
        trip_reg = int(mtctr.operands[0])
        trip_count, trip_notes = _concrete_trip_count(
            instructions,
            mtctr_index,
            trip_reg,
            readonly_words=readonly_words,
        )
        trip_expr_dict: dict[str, Any] | None = None
        trip_upper_bound: int | None = None
        zero_guard: str | None = None
        bounded_trip: BoundedRemainderTrip | None = None
        expr, expr_notes = recognize_trip_expr(
            instructions,
            mtctr_index,
            trip_reg,
            readonly_words=readonly_words,
        )
        if expr is not None:
            trip_expr_dict = canonical_dict(expr)
            from tools.ppc_equivalence.trip_expression import remainder_upper_bound

            bound = remainder_upper_bound(expr)
            if bound is not None:
                trip_upper_bound = bound
            bounded_trip = recover_bounded_remainder_trip(
                instructions,
                mtctr_index,
                trip_reg,
                header_pc=instructions[header_index].address,
                readonly_words=readonly_words,
            )
            if bounded_trip is not None:
                trip_upper_bound = bounded_trip.upper_bound
                zero_guard = bounded_trip.zero_guard.kind
                if trip_count is None and bounded_trip.concrete_trip is not None:
                    trip_count = bounded_trip.concrete_trip

        notes = list(body_notes)
        notes.extend(trip_notes)
        notes.extend(expr_notes)
        if bounded_trip is not None:
            notes.extend(bounded_trip.notes)
            notes.extend(bounded_trip.zero_guard.notes)
        if trip_count == 0:
            # mtctr 0 + bdnz wraps to 0xffffffff — never summarize as a store loop.
            notes.append("CTR load of 0 wraps under bdnz (unsupported without skip guard)")
            confidence = "unsupported"
        elif trip_count is not None and trip_count >= 1:
            confidence = "exact-pattern"
        elif bounded_trip is not None and zero_guard in ("concrete-nonzero", "skip-branch"):
            confidence = "bounded-remainder"
        else:
            confidence = "partial"

        loops.append(
            ConstantStrideStoreLoop(
                header_pc=instructions[header_index].address,
                latch_pc=insn.address,
                exit_pc=insn.address + 4,
                mtctr_pc=mtctr.address,
                base_reg=parsed.base_reg,
                index_reg=parsed.index_reg,
                stride=parsed.stride,
                store_width=parsed.store_width,
                source_reg=parsed.source_reg,
                store_kind=parsed.store_kind,
                trip_count=trip_count,
                trip_count_reg=trip_reg,
                trip_expr=trip_expr_dict,
                trip_upper_bound=trip_upper_bound,
                zero_guard=zero_guard,
                confidence=confidence,
                notes=tuple(notes),
            ),
        )
    return loops


def summarize_constant_stride_store_loop(
    loop: ConstantStrideStoreLoop,
    *,
    bounded_trip: BoundedRemainderTrip | None = None,
) -> MemoryLoopSummary | None:
    """Build a closed-form or bounded-remainder summary when premises hold.

    Bounded remainder summaries are allowed when the zero-trip guard and a
    small upper bound are proven, even without a concrete trip count. The
    obligation ``trip_count`` then records the expansion upper bound; apply
    uses ``concrete_trip=None`` under ``skip-branch`` guards.
    """
    del bounded_trip  # Recovered during recognition; loop fields are authoritative.
    if loop.confidence not in ("exact-pattern", "bounded-remainder"):
        return None

    expansion = "closed-form"
    if loop.confidence == "bounded-remainder":
        if loop.trip_expr is None or loop.trip_upper_bound is None:
            return None
        if loop.zero_guard not in ("concrete-nonzero", "skip-branch"):
            return None
        expansion = "bounded-remainder"
        # Footprint / schema use concrete trip when known, else the proven bound.
        trip_for_schema = (
            int(loop.trip_count)
            if loop.trip_count is not None and loop.trip_count >= 1
            else int(loop.trip_upper_bound)
        )
    else:
        if loop.trip_count is None or loop.trip_count < 1:
            return None
        trip_for_schema = int(loop.trip_count)

    if trip_for_schema > MAX_MEMORY_LOOP_TRIPS:
        return None
    if loop.store_kind not in ("stwu", "d-form-addi"):
        return None
    if not footprint_ok_for_summary(
        trip_count=trip_for_schema,
        stride=int(loop.stride),
        store_width=int(loop.store_width),
        store_kind=loop.store_kind,
    ):
        return None

    return MemoryLoopSummary(
        header_pc=loop.header_pc,
        latch_pc=loop.latch_pc,
        exit_pc=loop.exit_pc,
        trip_count=trip_for_schema,
        base_reg=loop.base_reg,
        source_reg=loop.source_reg,
        stride=loop.stride,
        store_width=loop.store_width,
        store_kind=loop.store_kind,
        final_ctr=0,
        ranking="ctr-descending",
        proof_kind="constant-stride-store",
        invariant_notes=tuple(loop.notes),
        trip_expr=loop.trip_expr,
        trip_upper_bound=loop.trip_upper_bound,
        zero_guard=loop.zero_guard,
        expansion=expansion,
    )


def build_memory_loop_summary_map(
    instructions: Sequence[Instruction],
    *,
    readonly_words: dict[int, int] | None = None,
) -> dict[int, MemoryLoopSummary]:
    """Map loop header PC → closed-form memory-loop summary.

    Descriptive callers may use this map; the engine consumes
    ``build_memory_loop_plan_map`` so the witness cannot drift from the summary.
    """
    return {
        header: plan.summary
        for header, plan in build_memory_loop_plan_map(
            instructions,
            readonly_words=readonly_words,
        ).items()
    }


def build_memory_loop_plan_map(
    instructions: Sequence[Instruction],
    *,
    readonly_words: dict[int, int] | None = None,
) -> dict[int, MemoryLoopPlan]:
    """Map loop header PC → paired summary + exact instruction witness."""
    if not instructions:
        return {}

    by_address = {insn.address: index for index, insn in enumerate(instructions)}
    mapping: dict[int, MemoryLoopPlan] = {}

    for loop in find_constant_stride_store_loops(
        instructions,
        readonly_words=readonly_words,
    ):
        summary = summarize_constant_stride_store_loop(loop)
        if summary is None:
            continue
        if summary.header_pc in mapping:
            del mapping[summary.header_pc]
            continue

        header_index = by_address.get(summary.header_pc)
        latch_index = by_address.get(summary.latch_pc)
        mtctr_index = by_address.get(loop.mtctr_pc)
        if header_index is None or latch_index is None or mtctr_index is None:
            continue
        if header_index >= latch_index or mtctr_index != header_index - 1:
            continue

        body = tuple(instructions[header_index:latch_index])
        latch = instructions[latch_index]
        mtctr = instructions[mtctr_index]
        if not _is_bdnz(latch) or not _is_mtctr(mtctr):
            continue
        parsed, _notes = _parse_constant_stride_store_body(body)
        if parsed is None:
            continue

        witness = MemoryLoopWitness(
            body=body,
            latch=latch,
            mtctr=mtctr,
            header_pc=summary.header_pc,
            latch_pc=summary.latch_pc,
            exit_pc=summary.exit_pc,
            code_sha256=compute_witness_code_sha256(body, latch, mtctr),
            recognized_trip_count=int(summary.trip_count),
        )
        mapping[summary.header_pc] = MemoryLoopPlan(summary=summary, witness=witness)
    return mapping


def memory_loop_plan_address_span(
    plan: MemoryLoopPlan,
    instructions: Sequence[Instruction],
    *,
    readonly_words: dict[int, int] | None = None,
) -> tuple[int, int] | None:
    """Resolve the concrete ``[start, end)`` byte span a recognized
    constant-stride store loop touches, when its base register is a
    concrete straight-line constant at loop entry (bounded materialization,
    same technique ``gx_fifo_loop.py`` uses to resolve the FIFO address).

    Returns ``None`` — "not provably resolvable" — when the base is not a
    concrete constant or the footprint would wrap 32-bit address space.
    Callers (Track C loop×FIFO routing) must treat ``None`` as "may touch
    every MMIO region" and fail closed; this function never claims
    disjointness it cannot prove.
    """
    summary = plan.summary
    by_address = {insn.address: index for index, insn in enumerate(instructions)}
    header_index = by_address.get(summary.header_pc)
    if header_index is None:
        return None
    base_value, _notes = recover_gpr_constant(
        instructions,
        header_index,
        summary.base_reg,
        readonly_words=readonly_words,
    )
    if base_value is None:
        return None
    trip_count = int(summary.trip_count)
    stride = int(summary.stride)
    if footprint_wraps_u32(int(base_value), trip_count, stride, summary.store_kind):
        return None
    if summary.store_kind == "stwu":
        start = (int(base_value) + stride) & 0xFFFFFFFF
    else:
        start = int(base_value) & 0xFFFFFFFF
    end = start + trip_count * stride
    return start, end


def memory_loop_plan_may_touch_regions(
    plan: MemoryLoopPlan,
    instructions: Sequence[Instruction],
    regions: Sequence[tuple[int, int]],
    *,
    readonly_words: dict[int, int] | None = None,
) -> bool:
    """True when ``plan``'s footprint cannot be proven disjoint from ``regions``.

    Fail-closed / conservative: an unresolvable base address (or a footprint
    that would wrap the address space) counts as "may touch" every region in
    ``regions`` rather than "provably does not touch".
    """
    if not regions:
        return False
    span = memory_loop_plan_address_span(
        plan, instructions, readonly_words=readonly_words,
    )
    if span is None:
        return True
    start, end = span
    return any(start < r_end and r_start < end for r_start, r_end in regions)


def collect_memory_loop_ctr_lwz_addresses(
    instructions: Sequence[Instruction],
) -> frozenset[int]:
    """Collect linked VAs of readonly words referenced by CTR ``lwz`` before ``mtctr``."""
    if not instructions:
        return frozenset()

    by_address = {insn.address: index for index, insn in enumerate(instructions)}
    addresses: set[int] = set()

    for index, insn in enumerate(instructions):
        if not _is_bdnz(insn):
            continue
        target = int(insn.operands[2]) & 0xFFFFFFFC
        header_index = by_address.get(target)
        if header_index is None or header_index >= index:
            continue

        mtctr_index = header_index - 1
        if mtctr_index < 0:
            continue
        mtctr = instructions[mtctr_index]
        if not _is_mtctr(mtctr):
            continue
        trip_reg = int(mtctr.operands[0])
        addresses.update(
            collect_lwz_readonly_addresses(instructions, mtctr_index, trip_reg),
        )

    return frozenset(addresses)


def apply_memory_loop_summary(state: Any, summary: MemoryLoopSummary, ops: Any) -> Any:
    """Return a post-loop state under the closed-form store summary.

    Stores go through ``memory_semantics.apply_store_effect`` so
    ``memory_writes`` / ``memory_touches`` match ordinary store execution.
    """
    if summary.expansion == "bounded-remainder" and summary.trip_expr is not None:
        from tools.ppc_equivalence.trip_expression import (
            TripAnd,
            TripConstant,
            TripEntryReg,
            TripLshr,
            TripAdd,
        )

        def _expr_from_dict(data: dict[str, Any]) -> Any:
            kind = data.get("kind")
            if kind == "const":
                return TripConstant(int(data["value"]))
            if kind == "entry":
                return TripEntryReg(int(data["reg"]))
            if kind == "and":
                return TripAnd(_expr_from_dict(data["left"]), _expr_from_dict(data["right"]))
            if kind == "lshr":
                return TripLshr(_expr_from_dict(data["left"]), int(data["shift"]))
            if kind == "add":
                return TripAdd(_expr_from_dict(data["left"]), _expr_from_dict(data["right"]))
            raise ValueError(f"unknown trip_expr kind {kind!r}")

        from tools.ppc_equivalence.bounded_remainder_loop import ZeroTripGuard

        # Under skip-branch, trip_count may be the expansion upper bound only.
        concrete_trip: int | None
        if summary.zero_guard == "concrete-nonzero":
            concrete_trip = int(summary.trip_count)
        elif (
            summary.zero_guard == "skip-branch"
            and summary.trip_upper_bound is not None
            and int(summary.trip_count) == int(summary.trip_upper_bound)
        ):
            concrete_trip = None
        elif summary.trip_count is not None and int(summary.trip_count) >= 1:
            concrete_trip = int(summary.trip_count)
        else:
            concrete_trip = None
        bounded = BoundedRemainderTrip(
            expr=_expr_from_dict(summary.trip_expr),
            expr_canonical=summary.trip_expr,
            upper_bound=int(summary.trip_upper_bound or summary.trip_count),
            concrete_trip=concrete_trip,
            zero_guard=ZeroTripGuard(summary.zero_guard or "concrete-nonzero"),
            notes=summary.invariant_notes,
        )
        return apply_bounded_remainder_memory_loop(
            state,
            trip=bounded,
            base_reg=summary.base_reg,
            source_reg=summary.source_reg,
            stride=int(summary.stride),
            store_width=int(summary.store_width),
            store_kind=summary.store_kind,
            final_ctr=int(summary.final_ctr),
            ops=ops,
        )

    if summary.trip_count < 1:
        raise ValueError("memory-loop summary requires a positive concrete trip count")
    transition = build_memory_loop_transition(
        state,
        trip_count=int(summary.trip_count),
        base_reg=summary.base_reg,
        source_reg=summary.source_reg,
        stride=int(summary.stride),
        store_width=int(summary.store_width),
        store_kind=summary.store_kind,
        final_ctr=int(summary.final_ctr),
        ops=ops,
    )
    return apply_memory_loop_transition(state, transition, ops)


def apply_memory_loop_iteration_summary(
    state: Any,
    summary: MemoryLoopSummary,
    ops: Any,
) -> Any:
    """Apply exactly one summarized store iteration without collapsing CTR to zero.

    Advances the base by one stride, performs one typed store, and sets
    ``CTR = entry.ctr - 1``. Used by body-step refinement discharge.
    """
    if summary.store_kind not in ("stwu", "d-form-addi"):
        raise ValueError(f"unsupported store_kind {summary.store_kind!r}")
    entry_ctr = state.ctr
    stepped = apply_memory_loop_transition(
        state,
        build_memory_loop_transition(
            state,
            trip_count=1,
            base_reg=summary.base_reg,
            source_reg=summary.source_reg,
            stride=int(summary.stride),
            store_width=int(summary.store_width),
            store_kind=summary.store_kind,
            final_ctr=0,  # overwritten below
            ops=ops,
        ),
        ops,
    )
    return replace(stepped, ctr=ops.sub(entry_ctr, ops.const(1)))


REQUIRED_MEMORY_LOOP_KEYS = frozenset({
    "schema_version",
    "algorithm",
    "status",
    "original",
    "candidate",
})

_LEGACY_FLAT_MIRROR_KEYS = frozenset({
    "proof_kind",
    "header_pc",
    "latch_pc",
    "exit_pc",
    "trip_count",
    "base_reg",
    "source_reg",
    "stride",
    "store_width",
    "store_kind",
    "final_ctr",
    "ranking",
    "summary_sha256",
    "effects",
    "footprint",
    "transition_equivalence",
    "coverage",
})


def memory_loop_identity_payload(obligation: dict[str, Any]) -> dict[str, Any]:
    """Canonical fields hashed into set-level digests (excludes status)."""
    payload: dict[str, Any] = {
        "schema_version": obligation.get("schema_version"),
        "algorithm": obligation.get("algorithm"),
    }
    for side in ("original", "candidate"):
        entries = obligation.get(side)
        if isinstance(entries, list):
            payload[side] = [
                {
                    "header_pc": item.get("header_pc"),
                    "latch_pc": item.get("latch_pc"),
                    "summary_sha256": item.get("summary_sha256"),
                    "code_sha256": item.get("code_sha256"),
                }
                for item in entries
                if isinstance(item, dict)
            ]
    if "readonly_words_sha256" in obligation:
        payload["readonly_words_sha256"] = obligation["readonly_words_sha256"]
    return payload


def compute_memory_loop_sha256(obligation: dict[str, Any]) -> str:
    """SHA-256 over the canonical memory-loop set identity payload."""
    return canonical_json_sha256(memory_loop_identity_payload(obligation))


def build_memory_loop_side_entry(
    plan: MemoryLoopPlan,
    *,
    entry_guard: dict[str, Any] | None = None,
    refinement: dict[str, Any] | None = None,
) -> dict[str, Any]:
    """One per-side discharge entry for the schema-v2 obligation set."""
    return {
        "header_pc": int(plan.summary.header_pc) & 0xFFFFFFFF,
        "latch_pc": int(plan.summary.latch_pc) & 0xFFFFFFFF,
        "summary_sha256": compute_summary_identity_sha256(plan.summary),
        "code_sha256": plan.witness.code_sha256,
        "entry_guard": entry_guard or {},
        "refinement": refinement or {},
    }


def build_memory_loop_obligation(
    *,
    original: Sequence[dict[str, Any]] = (),
    candidate: Sequence[dict[str, Any]] = (),
    status: str = "pending",
    readonly_words_sha256: str | None = None,
) -> dict[str, Any]:
    """Schema-v2 obligation block for ``proof_features: [\"memory-loop-summary\"]``.

    ``status=discharged`` requires every side entry to carry a discharged
    refinement block and an UNSAT entry-guard discharge. Side arrays may differ
    in length; structural summary equality across sides is not required.
    """
    payload: dict[str, Any] = {
        "schema_version": _MEMORY_LOOP_SCHEMA_VERSION,
        "algorithm": _MEMORY_LOOP_SET_ALGORITHM,
        "status": status,
        "original": [dict(item) for item in original],
        "candidate": [dict(item) for item in candidate],
    }
    if readonly_words_sha256 is not None:
        payload["readonly_words_sha256"] = readonly_words_sha256
    return payload


# Back-compat alias used by older call sites that still pass a single summary.
def build_memory_loop_obligation_from_summary(
    summary: MemoryLoopSummary,
    *,
    coverage: str = "pending",
    status: str = "pending",
    readonly_words_sha256: str | None = None,
    transition_equivalence: dict[str, Any] | None = None,
) -> dict[str, Any]:
    """Deprecated flat builder retained only for transitional tests.

    Always emits schema v2 with a single original entry and empty candidate.
    Flat v1 transition-equivalence payloads are rejected by validation.
    """
    del coverage, transition_equivalence
    side = {
        "header_pc": summary.header_pc,
        "latch_pc": summary.latch_pc,
        "summary_sha256": compute_summary_identity_sha256(summary),
        "code_sha256": "0" * 64,
        "entry_guard": {},
        "refinement": {},
    }
    return build_memory_loop_obligation(
        original=[side],
        candidate=[],
        status=status if status != "discharged" else "applied",
        readonly_words_sha256=readonly_words_sha256,
    )


def validate_memory_loop_obligation(obligation: dict[str, Any]) -> str | None:
    """Return None when a memory-loop obligation is structurally well-formed."""
    if not isinstance(obligation, dict):
        return "memory_loop must be an object"

    legacy = sorted(_LEGACY_FLAT_MIRROR_KEYS & obligation.keys())
    if legacy:
        return (
            "memory_loop must not carry legacy flat mirrors: "
            + ", ".join(legacy)
        )

    missing = sorted(REQUIRED_MEMORY_LOOP_KEYS - obligation.keys())
    if missing:
        return "memory_loop missing " + ", ".join(missing)

    if obligation.get("schema_version") != _MEMORY_LOOP_SCHEMA_VERSION:
        return (
            f"memory_loop.schema_version must be {_MEMORY_LOOP_SCHEMA_VERSION}"
        )

    if obligation.get("algorithm") != _MEMORY_LOOP_SET_ALGORITHM:
        return f"memory_loop.algorithm must be {_MEMORY_LOOP_SET_ALGORITHM!r}"

    status = obligation.get("status")
    if not isinstance(status, str) or status not in (
        "pending",
        "applied",
        "discharged",
        "failed",
    ):
        return "memory_loop.status must be pending|applied|discharged|failed"

    readonly_digest = obligation.get("readonly_words_sha256")
    if readonly_digest is not None:
        if not isinstance(readonly_digest, str) or len(readonly_digest) != 64:
            return "memory_loop.readonly_words_sha256 must be a 64-hex digest"
        try:
            int(readonly_digest, 16)
        except ValueError:
            return "memory_loop.readonly_words_sha256 must be a 64-hex digest"

    for side in ("original", "candidate"):
        entries = obligation.get(side)
        if not isinstance(entries, list):
            return f"memory_loop.{side} must be an array"
        reason = _validate_side_entries(side, entries, require_discharged=(status == "discharged"))
        if reason is not None:
            return reason

    if status == "discharged":
        for side in ("original", "candidate"):
            for entry in obligation[side]:
                if not isinstance(entry, dict):
                    continue
                refinement = entry.get("refinement")
                if not isinstance(refinement, dict) or refinement.get("status") != "discharged":
                    return (
                        f"memory_loop.status=discharged requires every "
                        f"{side} entry refinement.status=discharged"
                    )
                guard = entry.get("entry_guard")
                if not isinstance(guard, dict) or guard.get("result") != "unsat":
                    return (
                        f"memory_loop.status=discharged requires every "
                        f"{side} entry entry_guard.result=unsat"
                    )

    return None


def _validate_side_entries(
    side: str,
    entries: list[Any],
    *,
    require_discharged: bool,
) -> str | None:
    seen: set[tuple[Any, ...]] = set()
    for index, entry in enumerate(entries):
        if not isinstance(entry, dict):
            return f"memory_loop.{side}[{index}] must be an object"
        for key in ("header_pc", "latch_pc", "summary_sha256", "code_sha256"):
            if key not in entry:
                return f"memory_loop.{side}[{index}] missing {key}"
        for key in ("header_pc", "latch_pc"):
            value = entry.get(key)
            if not isinstance(value, int) or value < 0 or value > 0xFFFFFFFF:
                return f"memory_loop.{side}[{index}].{key} must be a u32 int"
        for key in ("summary_sha256", "code_sha256"):
            digest = entry.get(key)
            if not isinstance(digest, str) or len(digest) != 64:
                return f"memory_loop.{side}[{index}].{key} must be a 64-hex digest"
            if digest != digest.lower():
                return (
                    f"memory_loop.{side}[{index}].{key} must be a lowercase "
                    "64-hex digest"
                )
            try:
                int(digest, 16)
            except ValueError:
                return f"memory_loop.{side}[{index}].{key} must be a 64-hex digest"

        identity = (
            entry.get("header_pc"),
            entry.get("latch_pc"),
            entry.get("summary_sha256"),
            entry.get("code_sha256"),
        )
        if identity in seen:
            return f"memory_loop.{side} has duplicate plan identity"
        seen.add(identity)

        guard = entry.get("entry_guard")
        if guard is None:
            guard = {}
        if not isinstance(guard, dict):
            return f"memory_loop.{side}[{index}].entry_guard must be an object"
        reason = _validate_entry_guard(side, index, guard, require_discharged=require_discharged)
        if reason is not None:
            return reason

        refinement = entry.get("refinement")
        if refinement is None:
            refinement = {}
        if not isinstance(refinement, dict):
            return f"memory_loop.{side}[{index}].refinement must be an object"
        reason = _validate_refinement(side, index, refinement, require_discharged=require_discharged)
        if reason is not None:
            return reason
    return None


def _validate_query_digest(label: str, digest: Any) -> str | None:
    if not isinstance(digest, str) or len(digest) != 64 or digest != digest.lower():
        return f"{label} must be a lowercase 64-hex digest"
    try:
        int(digest, 16)
    except ValueError:
        return f"{label} must be a lowercase 64-hex digest"
    return None


def _validate_entry_guard(
    side: str,
    index: int,
    guard: dict[str, Any],
    *,
    require_discharged: bool,
) -> str | None:
    # Lazy import avoids a memory_loop <-> memory_loop_discharge import cycle.
    from tools.ppc_equivalence.memory_loop_discharge import (
        ENTRY_GUARD_ALGORITHM,
        KNOWN_REFINEMENT_ALGORITHMS,
    )

    if not guard:
        if require_discharged:
            return (
                f"memory_loop.{side}[{index}].entry_guard is required when "
                "status=discharged"
            )
        return None
    algorithm = guard.get("algorithm")
    if algorithm not in {ENTRY_GUARD_ALGORITHM} | KNOWN_REFINEMENT_ALGORITHMS:
        # entry guard has its own algorithm; reject unknown / v1 names
        if algorithm != ENTRY_GUARD_ALGORITHM:
            return (
                f"memory_loop.{side}[{index}].entry_guard.algorithm "
                f"{algorithm!r} is not a known entry-guard algorithm"
            )
    if require_discharged and guard.get("result") != "unsat":
        return f"memory_loop.{side}[{index}].entry_guard.result must be 'unsat'"
    if "query_sha256" in guard or require_discharged:
        reason = _validate_query_digest(
            f"memory_loop.{side}[{index}].entry_guard.query_sha256",
            guard.get("query_sha256"),
        )
        if reason is not None:
            return reason
    return None


def _validate_refinement(
    side: str,
    index: int,
    refinement: dict[str, Any],
    *,
    require_discharged: bool,
) -> str | None:
    from tools.ppc_equivalence.memory_loop_discharge import (
        KNOWN_REFINEMENT_ALGORITHMS,
        REFINEMENT_ALGORITHM,
        REQUIRED_REFINEMENT_BLOCKS,
    )

    if not refinement:
        if require_discharged:
            return (
                f"memory_loop.{side}[{index}].refinement is required when "
                "status=discharged"
            )
        return None

    algorithm = refinement.get("algorithm")
    if algorithm != REFINEMENT_ALGORITHM:
        return (
            f"memory_loop.{side}[{index}].refinement.algorithm must be "
            f"{REFINEMENT_ALGORITHM!r}"
        )
    # Reject any legacy v1 transition-equivalence block names nested here.
    if "transition_equivalence" in refinement:
        return (
            f"memory_loop.{side}[{index}].refinement must not carry legacy "
            "transition_equivalence"
        )

    status = refinement.get("status")
    if status not in ("discharged", "applied", "failed", "unsupported"):
        return (
            f"memory_loop.{side}[{index}].refinement.status must be "
            "discharged|applied|failed|unsupported"
        )
    if require_discharged and status != "discharged":
        return (
            f"memory_loop.{side}[{index}].refinement.status must be "
            "'discharged'"
        )

    for name in REQUIRED_REFINEMENT_BLOCKS:
        block = refinement.get(name)
        if not isinstance(block, dict):
            if require_discharged:
                return (
                    f"memory_loop.{side}[{index}].refinement.{name} block "
                    "is required"
                )
            continue
        if require_discharged and block.get("result") != "unsat":
            return (
                f"memory_loop.{side}[{index}].refinement.{name}.result "
                "must be 'unsat'"
            )
        if "query_sha256" in block or require_discharged:
            reason = _validate_query_digest(
                f"memory_loop.{side}[{index}].refinement.{name}.query_sha256",
                block.get("query_sha256"),
            )
            if reason is not None:
                return reason
        block_algorithm = block.get("algorithm")
        if block_algorithm not in KNOWN_REFINEMENT_ALGORITHMS:
            return (
                f"memory_loop.{side}[{index}].refinement.{name}.algorithm "
                f"{block_algorithm!r} is not a known refinement algorithm"
            )
        if isinstance(block_algorithm, str) and block_algorithm.endswith("-v1"):
            return (
                f"memory_loop.{side}[{index}].refinement.{name}.algorithm "
                "must not be a legacy v1 transition algorithm"
            )
    return None


@dataclass(frozen=True)
class _ParsedStoreBody:
    base_reg: int
    index_reg: int | None
    stride: int
    store_width: int
    source_reg: int
    store_kind: str


def _parse_constant_stride_store_body(
    body: Sequence[Instruction],
) -> tuple[_ParsedStoreBody | None, list[str]]:
    """Accept only exact ``stwu`` or ``store; addi`` bodies (order-sensitive)."""
    if not body:
        return None, ["empty loop body"]

    notes: list[str] = []

    # stwu alone: single instruction, no trailing addi.
    if len(body) == 1 and body[0].opcode == Opcode.STWU:
        store_insn = body[0]
        width = _STORE_WIDTH[store_insn.opcode]
        source_reg, base_reg, disp = (int(v) for v in store_insn.operands)
        if base_reg == 0:
            return None, ["store uses r0 as base"]
        if source_reg == base_reg:
            return None, ["store source equals base register"]
        stride = int(disp)
        if stride <= 0:
            return None, [f"stwu disp {stride} not positive"]
        if stride != width:
            return None, [f"stwu disp {stride} != store width {width}"]
        return (
            _ParsedStoreBody(
                base_reg=base_reg,
                index_reg=None,
                stride=stride,
                store_width=width,
                source_reg=source_reg,
                store_kind="stwu",
            ),
            notes,
        )

    # Exact store-then-addi (reject reversed order, multi-store, calls, etc.).
    if len(body) != 2:
        return None, ["body must be stwu or exact store-then-addi"]

    store_insn, addi_insn = body[0], body[1]
    if store_insn.opcode not in _STORE_WIDTH or store_insn.opcode == Opcode.STWU:
        return None, ["first body insn must be stb/sth/stw (not stwu)"]
    if addi_insn.opcode != Opcode.ADDI:
        return None, ["second body insn must be pointer addi"]

    width = _STORE_WIDTH[store_insn.opcode]
    source_reg, base_reg, disp = (int(v) for v in store_insn.operands)
    if base_reg == 0:
        return None, ["store uses r0 as base"]
    if source_reg == base_reg:
        return None, ["store source equals base register"]
    if disp != 0:
        return None, [f"store disp {disp} != 0 without indexed addressing"]

    rt, ra, imm = (int(v) for v in addi_insn.operands)
    if rt != ra:
        return None, [f"non-pointer addi r{rt}, r{ra}, {imm}"]
    if rt != base_reg:
        return None, [f"addi r{rt} does not match store base r{base_reg}"]

    stride = int(imm)
    if stride <= 0:
        return None, [f"stride {stride} not positive"]
    if stride != width:
        return None, [f"stride {stride} != store width {width}"]

    return (
        _ParsedStoreBody(
            base_reg=base_reg,
            index_reg=None,
            stride=stride,
            store_width=width,
            source_reg=source_reg,
            store_kind="d-form-addi",
        ),
        notes,
    )


def _is_bdnz(insn: Instruction) -> bool:
    if insn.opcode != Opcode.BC or insn.link:
        return False
    bo, _bi, _target, _aa = insn.operands
    return int(bo) == _BDNZ_BO


def _is_mtctr(insn: Instruction) -> bool:
    return (
        insn.opcode == Opcode.MTSPR
        and len(insn.operands) == 2
        and int(insn.operands[1]) == _CTR_SPR
    )


def _concrete_trip_count(
    instructions: Sequence[Instruction],
    mtctr_index: int,
    trip_reg: int,
    *,
    readonly_words: dict[int, int] | None = None,
) -> tuple[int | None, list[str]]:
    """Recover ``N`` from bounded straight-line materialization before ``mtctr``."""
    value, notes = recover_gpr_constant(
        instructions,
        mtctr_index,
        trip_reg,
        readonly_words=readonly_words,
    )
    if value is None:
        if not notes:
            notes = ["CTR source is not a concrete constant"]
        return None, notes
    return value, notes
