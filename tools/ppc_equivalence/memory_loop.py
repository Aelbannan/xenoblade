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

When ``execute_cfg`` is given a summary map (or the engine auto-builds one),
matching headers with a positive concrete trip count are applied by
recording typed ``StoreEffect`` writes (not memory alone) and advancing
base/CTR. ``memory-loop-summary`` may authorize ``EQUIVALENT`` only when the
obligation carries ``status=discharged`` with a matching ``summary_sha256``,
typed-effect + footprint evidence, and a concrete closed-form expansion.
Recognition or ``coverage=applied`` alone never authorizes. Bounded-remainder
expansions stay ``applied`` until paired relational discharge lands.
"""

from __future__ import annotations

from collections.abc import Sequence
from dataclasses import dataclass
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
)
from tools.ppc_equivalence.provenance import canonical_json_sha256

_CTR_SPR = 9
_BDNZ_BO = 16  # decrement CTR; branch if CTR != 0 after decrement

# Fail closed above this trip count: closed-form applies N explicit stores.
MAX_MEMORY_LOOP_TRIPS = 4096

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
    """Map loop header PC → closed-form memory-loop summary."""
    mapping: dict[int, MemoryLoopSummary] = {}
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
        mapping[summary.header_pc] = summary
    return mapping


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


REQUIRED_MEMORY_LOOP_KEYS = frozenset({
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
    "status",
    "algorithm",
    "summary_sha256",
    "effects",
    "footprint",
})

_MEMORY_LOOP_ALGORITHM = "constant-stride-store-v1"

_MEMORY_LOOP_IDENTITY_KEYS = (
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
    "algorithm",
    "effects",
    "footprint",
    "trip_expr",
    "trip_upper_bound",
    "zero_guard",
    "expansion",
    "readonly_words_sha256",
)


def memory_loop_identity_payload(obligation: dict[str, Any]) -> dict[str, Any]:
    """Canonical fields hashed into ``summary_sha256`` (excludes status/coverage)."""
    payload: dict[str, Any] = {}
    for key in _MEMORY_LOOP_IDENTITY_KEYS:
        if key not in obligation:
            continue
        value = obligation[key]
        if value is None:
            continue
        payload[key] = value
    return payload


def compute_memory_loop_sha256(obligation: dict[str, Any]) -> str:
    """SHA-256 over the canonical memory-loop summary identity payload."""
    return canonical_json_sha256(memory_loop_identity_payload(obligation))


def build_memory_loop_obligation(
    summary: MemoryLoopSummary,
    *,
    coverage: str = "pending",
    status: str = "pending",
    readonly_words_sha256: str | None = None,
) -> dict[str, Any]:
    """Obligation block for ``proof_features: [\"memory-loop-summary\"]``.

    ``status=discharged`` requires a concrete closed-form expansion with typed
    store effects and a footprint gate. Bounded-remainder / recognition-only
    paths must remain ``pending`` / ``applied``.
    """
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
        "coverage": coverage,
        "status": status,
        "algorithm": _MEMORY_LOOP_ALGORITHM,
        "effects": "typed-store",
        "footprint": "ok",
    }
    if summary.trip_expr is not None:
        payload["trip_expr"] = dict(summary.trip_expr)
    if summary.trip_upper_bound is not None:
        payload["trip_upper_bound"] = int(summary.trip_upper_bound)
    if summary.zero_guard is not None:
        payload["zero_guard"] = summary.zero_guard
    if summary.expansion != "closed-form":
        payload["expansion"] = summary.expansion
    else:
        payload["expansion"] = "closed-form"
    if readonly_words_sha256 is not None:
        payload["readonly_words_sha256"] = readonly_words_sha256
    payload["summary_sha256"] = compute_memory_loop_sha256(payload)
    return payload


def validate_memory_loop_obligation(obligation: dict[str, Any]) -> str | None:
    """Return None when a memory-loop obligation is structurally well-formed."""
    missing = sorted(REQUIRED_MEMORY_LOOP_KEYS - obligation.keys())
    if missing:
        return "memory_loop missing " + ", ".join(missing)

    proof_kind = obligation.get("proof_kind")
    if not isinstance(proof_kind, str) or not proof_kind:
        return "memory_loop.proof_kind must be a nonempty string"

    for key in ("header_pc", "latch_pc", "exit_pc", "final_ctr"):
        value = obligation.get(key)
        if not isinstance(value, int) or value < 0 or value > 0xFFFFFFFF:
            return f"memory_loop.{key} must be a u32 int"

    trip_count = obligation.get("trip_count")
    if not isinstance(trip_count, int) or trip_count < 1 or trip_count > 0xFFFFFFFF:
        return "memory_loop.trip_count must be a positive u32 int"

    for key in ("base_reg", "source_reg"):
        value = obligation.get(key)
        if not isinstance(value, int) or value < 0 or value > 31:
            return f"memory_loop.{key} must be a GPR index 0..31"

    stride = obligation.get("stride")
    if not isinstance(stride, int) or stride < 1 or stride > 0xFFFFFFFF:
        return "memory_loop.stride must be a positive u32 int"

    store_width = obligation.get("store_width")
    if store_width not in (1, 2, 4):
        return "memory_loop.store_width must be 1, 2, or 4"

    store_kind = obligation.get("store_kind")
    if store_kind not in ("stwu", "d-form-addi"):
        return "memory_loop.store_kind must be 'stwu' or 'd-form-addi'"

    ranking = obligation.get("ranking")
    if not isinstance(ranking, str) or not ranking:
        return "memory_loop.ranking must be a nonempty string"

    status = obligation.get("status")
    if not isinstance(status, str) or status not in (
        "pending",
        "applied",
        "discharged",
        "failed",
    ):
        return "memory_loop.status must be pending|applied|discharged|failed"

    if obligation.get("algorithm") != _MEMORY_LOOP_ALGORITHM:
        return f"memory_loop.algorithm must be {_MEMORY_LOOP_ALGORITHM!r}"

    if obligation.get("effects") != "typed-store":
        return "memory_loop.effects must be 'typed-store'"

    if obligation.get("footprint") != "ok":
        return "memory_loop.footprint must be 'ok'"

    digest = obligation.get("summary_sha256")
    if not isinstance(digest, str) or len(digest) != 64:
        return "memory_loop.summary_sha256 must be a 64-hex digest"
    try:
        int(digest, 16)
    except ValueError:
        return "memory_loop.summary_sha256 must be a 64-hex digest"
    expected = compute_memory_loop_sha256(obligation)
    if digest != expected:
        return "memory_loop.summary_sha256 does not match obligation identity"

    readonly_digest = obligation.get("readonly_words_sha256")
    if readonly_digest is not None:
        if not isinstance(readonly_digest, str) or len(readonly_digest) != 64:
            return "memory_loop.readonly_words_sha256 must be a 64-hex digest"
        try:
            int(readonly_digest, 16)
        except ValueError:
            return "memory_loop.readonly_words_sha256 must be a 64-hex digest"

    expansion = obligation.get("expansion", "closed-form")
    if not isinstance(expansion, str) or not expansion:
        return "memory_loop.expansion must be a nonempty string"

    if status == "discharged":
        if expansion != "closed-form":
            return (
                "memory_loop.status=discharged requires expansion=closed-form "
                "(bounded-remainder stays applied until relational discharge)"
            )
        coverage = obligation.get("coverage")
        if coverage not in ("applied", "discharged"):
            return "memory_loop.status=discharged requires coverage applied|discharged"
        if not footprint_ok_for_summary(
            trip_count=int(trip_count),
            stride=int(stride),
            store_width=int(store_width),
            store_kind=str(store_kind),
        ):
            return "memory_loop.status=discharged requires a sound footprint"

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
