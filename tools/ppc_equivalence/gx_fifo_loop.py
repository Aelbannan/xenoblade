"""GX FIFO CTR loop recognition and closed-form emission summary.

Domain: ``mmio-loop-emission-v1`` (see ``GX_FIFO_TIER_A.md``). Deliberately
narrow, natural single-header CTR loop shape::

    li / addi / addis+ori  rT, _, N
    mtctr                  rT
  header:
    stb / sth / stw  rS, disp(rB)     # concrete GX pipe address
    addi             rS, rS, K        # optional — affine value only
    bdnz             header

The store's *base* register must resolve to a concrete address inside
``[gx_base, gx_base + gx_span)`` via bounded straight-line materialization
(``ctr_materialization.recover_gpr_constant``); the loop body must never
write the base register (the FIFO address stays fixed every iteration —
this is a write-gather pipe, not a memory stream). The store *source*
register is either untouched in the body (``value_kind="invariant"``) or
incremented by a constant immediately after the store
(``value_kind="affine"``). Multi-store bodies, indexed stores, ``stwu``,
loads, calls, and any other MMIO are rejected (prefer false negatives).

Recognition never authorizes equivalence. ``apply_gx_fifo_loop_summary``
performs the closed-form GPR/CTR update and returns a ``RepeatedEmission``
descriptor for the caller to append to a ``SymbolicEventTrace`` — it does not
discharge the per-side refinement obligations required by Tier A.
"""

from __future__ import annotations

from collections.abc import Mapping, Sequence
from dataclasses import dataclass, replace
from typing import Any

from tools.ppc_equivalence.ctr_materialization import recover_gpr_constant
from tools.ppc_equivalence.ir import Instruction, Opcode
from tools.ppc_equivalence.memory_loop import (
    MemoryLoopPlan,
    instruction_semantic_identity,
    memory_loop_plan_may_touch_regions,
)
from tools.ppc_equivalence.provenance import canonical_json_sha256
from tools.ppc_equivalence.trip_expression import canonical_dict, recognize_trip_expr

_CTR_SPR = 9
_BDNZ_BO = 16  # decrement CTR; branch if CTR != 0 after decrement

# GX write-gather pipe (SDK ``WGPIPE``); see GX_FIFO_TIER_A.md.
DEFAULT_GX_BASE = 0xCC00_8000
DEFAULT_GX_SPAN = 0x80
DEFAULT_DEVICE_ID = "gx-fifo"
DEFAULT_MAX_EVENTS = 256

_STORE_WIDTH: dict[Opcode, int] = {
    Opcode.STB: 1,
    Opcode.STH: 2,
    Opcode.STW: 4,
}

PROOF_KIND = "gx-fifo-loop-emission-v1"


@dataclass(frozen=True)
class GxFifoLoopCandidate:
    """Recognized CTR counted loop whose body emits one GX FIFO write."""

    header_pc: int
    latch_pc: int
    exit_pc: int
    mtctr_pc: int
    device_id: str
    address: int
    width: int
    writes_per_iteration: int
    base_reg: int
    source_reg: int
    value_kind: str  # "invariant" | "affine"
    value_step: int | None
    trip_count: int | None
    trip_count_reg: int | None
    trip_expr: dict[str, Any] | None
    trip_upper_bound: int | None
    zero_guard: str | None
    max_events: int
    confidence: str
    notes: tuple[str, ...]


@dataclass(frozen=True)
class GxFifoLoopSummary:
    """Closed-form summary for a recognized GX FIFO write-emission loop."""

    header_pc: int
    latch_pc: int
    exit_pc: int
    device_id: str
    address: int
    width: int
    writes_per_iteration: int
    value_kind: str  # "invariant" | "affine"
    value_expr: dict[str, Any]
    trip_count: int
    trip_expr: dict[str, Any] | None
    trip_upper_bound: int | None
    zero_guard: str | None
    final_ctr: int
    max_events: int
    footprint: dict[str, Any]
    proof_kind: str = PROOF_KIND


@dataclass(frozen=True)
class GxFifoLoopWitness:
    """Exact recognized instructions for a GX FIFO loop (drift-proof pairing)."""

    body: tuple[Instruction, ...]
    latch: Instruction
    mtctr: Instruction
    header_pc: int
    latch_pc: int
    exit_pc: int
    code_sha256: str
    recognized_trip_count: int


@dataclass(frozen=True)
class GxFifoLoopPlan:
    """Paired summary + witness so refinement cannot drift from recognition."""

    summary: GxFifoLoopSummary
    witness: GxFifoLoopWitness

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
    def trip_count(self) -> int:
        return self.summary.trip_count


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


def compute_summary_identity_sha256(summary: GxFifoLoopSummary) -> str:
    """SHA-256 over the closed-form summary identity (not the witness)."""
    payload: dict[str, Any] = {
        "proof_kind": summary.proof_kind,
        "header_pc": summary.header_pc,
        "latch_pc": summary.latch_pc,
        "exit_pc": summary.exit_pc,
        "device_id": summary.device_id,
        "address": summary.address,
        "width": summary.width,
        "writes_per_iteration": summary.writes_per_iteration,
        "value_kind": summary.value_kind,
        "value_expr": dict(summary.value_expr),
        "trip_count": summary.trip_count,
        "final_ctr": summary.final_ctr,
        "max_events": summary.max_events,
        "footprint": dict(summary.footprint),
    }
    if summary.trip_expr is not None:
        payload["trip_expr"] = dict(summary.trip_expr)
    if summary.trip_upper_bound is not None:
        payload["trip_upper_bound"] = int(summary.trip_upper_bound)
    if summary.zero_guard is not None:
        payload["zero_guard"] = summary.zero_guard
    return canonical_json_sha256(payload)


def recognize_gx_fifo_loops(
    instructions: Sequence[Instruction],
    *,
    gx_base: int = DEFAULT_GX_BASE,
    gx_span: int = DEFAULT_GX_SPAN,
    device_id: str = DEFAULT_DEVICE_ID,
    max_events: int = DEFAULT_MAX_EVENTS,
    readonly_words: Mapping[int, int] | None = None,
) -> list[GxFifoLoopCandidate]:
    """Scan for ``mtctr`` / single-FIFO-store body / ``bdnz`` loops.

    Prefers false negatives: any body shape other than exactly one narrow
    store (``value_kind="invariant"``) or one store followed by a matching
    self-``addi`` on the store's source register (``value_kind="affine"``)
    is rejected, as is any store whose base register does not resolve to a
    concrete address inside ``[gx_base, gx_base + gx_span)``.
    """
    if not instructions:
        return []

    by_address = {insn.address: index for index, insn in enumerate(instructions)}
    candidates: list[GxFifoLoopCandidate] = []

    for index, insn in enumerate(instructions):
        if not _is_bdnz(insn):
            continue
        target = int(insn.operands[2]) & 0xFFFFFFFC
        header_index = by_address.get(target)
        if header_index is None or header_index >= index:
            continue

        body = list(instructions[header_index:index])
        parsed, body_notes = _parse_gx_fifo_body(
            body,
            header_index=header_index,
            instructions=instructions,
            gx_base=gx_base,
            gx_span=gx_span,
            readonly_words=readonly_words,
        )
        if parsed is None:
            continue

        mtctr_index = header_index - 1
        if mtctr_index < 0:
            continue
        mtctr = instructions[mtctr_index]
        if not _is_mtctr(mtctr):
            continue
        trip_reg = int(mtctr.operands[0])
        trip_count, trip_notes = recover_gpr_constant(
            instructions,
            mtctr_index,
            trip_reg,
            readonly_words=readonly_words,
        )
        trip_expr_dict: dict[str, Any] | None = None
        trip_upper_bound: int | None = None
        expr, expr_notes = recognize_trip_expr(
            instructions,
            mtctr_index,
            trip_reg,
            readonly_words=readonly_words,
        )
        if expr is not None:
            trip_expr_dict = canonical_dict(expr)
            from tools.ppc_equivalence.trip_expression import remainder_upper_bound

            trip_upper_bound = remainder_upper_bound(expr)

        notes = list(body_notes)
        notes.extend(trip_notes)
        notes.extend(expr_notes)

        if trip_count == 0:
            # mtctr 0 + bdnz wraps to 0xffffffff — never summarize.
            notes.append("CTR load of 0 wraps under bdnz (unsupported without skip guard)")
            confidence = "unsupported"
        elif trip_count is not None and trip_count >= 1:
            confidence = "exact-pattern"
        else:
            confidence = "partial"

        candidates.append(
            GxFifoLoopCandidate(
                header_pc=instructions[header_index].address,
                latch_pc=insn.address,
                exit_pc=insn.address + 4,
                mtctr_pc=mtctr.address,
                device_id=device_id,
                address=parsed.address,
                width=parsed.width,
                writes_per_iteration=1,
                base_reg=parsed.base_reg,
                source_reg=parsed.source_reg,
                value_kind=parsed.value_kind,
                value_step=parsed.value_step,
                trip_count=trip_count,
                trip_count_reg=trip_reg,
                trip_expr=trip_expr_dict,
                trip_upper_bound=trip_upper_bound,
                zero_guard=None,
                max_events=max_events,
                confidence=confidence,
                notes=tuple(notes),
            ),
        )
    return candidates


def build_gx_fifo_loop_plan(
    candidate: GxFifoLoopCandidate,
    instructions: Sequence[Instruction],
    *,
    readonly_words: Mapping[int, int] | None = None,
) -> GxFifoLoopPlan | None:
    """Build a paired summary + witness for one recognized candidate.

    Returns ``None`` when the trip count is not a positive concrete value,
    the write-count bound is exceeded, or the witness cannot be re-sliced
    from ``instructions`` exactly as recognized (drift guard).
    """
    if candidate.confidence != "exact-pattern":
        return None
    if candidate.trip_count is None or candidate.trip_count < 1:
        return None
    trip_count = int(candidate.trip_count)

    total_events = trip_count * candidate.writes_per_iteration
    if total_events > candidate.max_events:
        return None

    by_address = {insn.address: index for index, insn in enumerate(instructions)}
    header_index = by_address.get(candidate.header_pc)
    latch_index = by_address.get(candidate.latch_pc)
    mtctr_index = by_address.get(candidate.mtctr_pc)
    if header_index is None or latch_index is None or mtctr_index is None:
        return None
    if header_index >= latch_index or mtctr_index != header_index - 1:
        return None

    body = tuple(instructions[header_index:latch_index])
    latch = instructions[latch_index]
    mtctr = instructions[mtctr_index]
    if not _is_bdnz(latch) or not _is_mtctr(mtctr):
        return None

    footprint = {
        "prohibited_effects_observed": [],
        "checked": [
            "no-calls",
            "no-indirect-branches",
            "no-early-exits",
            "no-fifo-reads",
            "no-other-mmio",
            "single-store-body" if candidate.value_kind == "invariant" else "store-then-affine-addi-body",
        ],
    }

    value_expr = _value_expr_for_candidate(candidate, instructions, header_index, readonly_words)

    summary = GxFifoLoopSummary(
        header_pc=candidate.header_pc,
        latch_pc=candidate.latch_pc,
        exit_pc=candidate.exit_pc,
        device_id=candidate.device_id,
        address=candidate.address,
        width=candidate.width,
        writes_per_iteration=candidate.writes_per_iteration,
        value_kind=candidate.value_kind,
        value_expr=value_expr,
        trip_count=trip_count,
        trip_expr=candidate.trip_expr,
        trip_upper_bound=candidate.trip_upper_bound,
        zero_guard=candidate.zero_guard,
        final_ctr=0,
        max_events=candidate.max_events,
        footprint=footprint,
    )

    witness = GxFifoLoopWitness(
        body=body,
        latch=latch,
        mtctr=mtctr,
        header_pc=candidate.header_pc,
        latch_pc=candidate.latch_pc,
        exit_pc=candidate.exit_pc,
        code_sha256=compute_witness_code_sha256(body, latch, mtctr),
        recognized_trip_count=trip_count,
    )
    return GxFifoLoopPlan(summary=summary, witness=witness)


def build_gx_fifo_loop_plans(
    instructions: Sequence[Instruction],
    *,
    gx_base: int = DEFAULT_GX_BASE,
    gx_span: int = DEFAULT_GX_SPAN,
    device_id: str = DEFAULT_DEVICE_ID,
    max_events: int = DEFAULT_MAX_EVENTS,
    readonly_words: Mapping[int, int] | None = None,
) -> dict[int, GxFifoLoopPlan]:
    """Map loop header PC → paired summary + exact instruction witness."""
    if not instructions:
        return {}

    mapping: dict[int, GxFifoLoopPlan] = {}
    for candidate in recognize_gx_fifo_loops(
        instructions,
        gx_base=gx_base,
        gx_span=gx_span,
        device_id=device_id,
        max_events=max_events,
        readonly_words=readonly_words,
    ):
        plan = build_gx_fifo_loop_plan(candidate, instructions, readonly_words=readonly_words)
        if plan is None:
            continue
        if plan.summary.header_pc in mapping:
            # Ambiguous re-detection at the same header: fail closed.
            del mapping[plan.summary.header_pc]
            continue
        mapping[plan.summary.header_pc] = plan
    return mapping


def build_gx_fifo_loop_plans_for_devices(
    instructions: Sequence[Instruction],
    devices: Sequence[Any],
    *,
    max_events: int = DEFAULT_MAX_EVENTS,
    readonly_words: Mapping[int, int] | None = None,
) -> dict[int, GxFifoLoopPlan]:
    """Build the recognized-loop plan map across every ``gxfifo-stream`` device.

    ``devices`` is the bus specification's ``DeviceSpecification`` sequence
    (or anything exposing ``.theory`` / ``.device_id`` / ``.base`` / ``.span``);
    non-FIFO theories are skipped. A header recognized against more than one
    device is ambiguous and dropped (fail closed), mirroring
    ``build_gx_fifo_loop_plans``'s own same-device ambiguity handling.
    """
    merged: dict[int, GxFifoLoopPlan] = {}
    seen_ambiguous: set[int] = set()
    for device in devices:
        if getattr(device, "theory", None) != "gxfifo-stream":
            continue
        per_device = build_gx_fifo_loop_plans(
            instructions,
            gx_base=int(device.base),
            gx_span=int(device.span),
            device_id=str(device.device_id),
            max_events=max_events,
            readonly_words=readonly_words,
        )
        for header, plan in per_device.items():
            if header in seen_ambiguous:
                continue
            if header in merged:
                del merged[header]
                seen_ambiguous.add(header)
                continue
            merged[header] = plan
    return merged


def unresolved_fifo_touching_memory_loop_headers(
    instructions: Sequence[Instruction],
    *,
    memory_loop_plans: Mapping[int, MemoryLoopPlan] | None,
    gx_fifo_loop_plans: Mapping[int, GxFifoLoopPlan] | None,
    fifo_regions: Sequence[tuple[int, int]],
    readonly_words: Mapping[int, int] | None = None,
) -> frozenset[int]:
    """Headers whose ``MemoryLoopPlan`` may write into a GX FIFO region and
    are *not* covered by a recognized ``mmio-loop-emission-v1`` plan at the
    same header.

    This is the narrow, targeted replacement for the old blanket
    "any loop summary + any FIFO device" rejection: ``affine_loop_summaries``
    never perform stores (pure GPR closed forms) so they can never touch
    FIFO and are not considered here at all. Constant-stride store loops
    (``MemoryLoopPlan``) *do* store, but apply their closed form straight to
    the RAM array (see ``memory_semantics.apply_store_effect``) — bypassing
    MMIO/FIFO device routing entirely. A ``MemoryLoopPlan`` whose footprint
    cannot be proven disjoint from every declared FIFO region must therefore
    stay hard-rejected; recognized ``GxFifoLoopPlan`` headers are the only
    proven-sound way to summarize a FIFO-touching counted loop.
    """
    if not fifo_regions or not memory_loop_plans:
        return frozenset()
    gx_headers = frozenset((gx_fifo_loop_plans or {}).keys())
    risky = {
        header
        for header, plan in memory_loop_plans.items()
        if header not in gx_headers
        and memory_loop_plan_may_touch_regions(
            plan, instructions, fifo_regions, readonly_words=readonly_words,
        )
    }
    return frozenset(risky)


def apply_gx_fifo_loop_summary(
    state: Any,
    plan: GxFifoLoopPlan,
    ops: Any,
    *,
    symbolic_bus_append_fn: Any = None,
) -> tuple[Any, Any]:
    """Apply the closed-form GPR/CTR update and build an emission descriptor.

    Returns ``(next_state, descriptor)``. ``next_state`` advances the source
    register (affine) and sets ``CTR = final_ctr``; the base register and
    every other GPR are left untouched (the GX pipe address never moves).

    For ``value_kind="invariant"`` the descriptor is a genuine
    ``symbolic_event_trace.RepeatedEmission`` (its per-iteration template is
    already iteration-invariant, matching that dataclass's contract). For
    ``value_kind="affine"`` the per-iteration value is *not* representable by
    ``RepeatedEmission`` yet (its template has no ``k``-dependence — see
    ``symbolic_event_trace.compare_repeated_extensional``), so a structured
    dict is returned instead; callers must not pass it to
    ``SymbolicEventTrace.append_repeated_emission`` until k-dependent
    templates land. This function never authorizes equivalence; it only
    performs the mechanical post-loop state update recognition already
    proved sound structurally.
    """
    summary = plan.summary
    if summary.trip_count < 1:
        raise ValueError("gx-fifo-loop summary requires a positive concrete trip count")

    trip_count = int(summary.trip_count)
    source_reg = _value_expr_reg(summary.value_expr)
    entry_value = state.gpr[source_reg] if source_reg is not None else None

    gprs = list(state.gpr)
    descriptor: Any

    if summary.value_kind == "invariant":
        # Unchanged: never written inside the recognized body.
        from tools.ppc_equivalence.symbolic_event_trace import (
            RepeatedEmission,
            SymbolicFifoEvent,
        )

        descriptor = RepeatedEmission(
            count=trip_count,
            writes_per_iteration=summary.writes_per_iteration,
            writes=(
                SymbolicFifoEvent(
                    kind="write",
                    addr=ops.const(summary.address),
                    width=summary.width,
                    value=entry_value if entry_value is not None else ops.const(
                        int(summary.value_expr.get("value", 0)),
                    ),
                ),
            ),
        )
    elif summary.value_kind == "affine":
        step = int(summary.value_expr.get("step", 0))
        if source_reg is not None and entry_value is not None:
            final_value = ops.add(entry_value, ops.const((step * trip_count) & 0xFFFFFFFF))
            gprs[source_reg] = final_value
        descriptor = {
            "kind": "affine-unsupported-template",
            "device_id": summary.device_id,
            "count": trip_count,
            "writes_per_iteration": summary.writes_per_iteration,
            "address": summary.address,
            "width": summary.width,
            "initial_value": entry_value,
            "step": step,
            "note": (
                "affine per-iteration FIFO values are not yet representable by "
                "RepeatedEmission's iteration-invariant template; deferred until "
                "k-dependent templates land (see symbolic_event_trace.py)"
            ),
        }
    else:  # pragma: no cover - guarded by recognition
        raise ValueError(f"unsupported value_kind {summary.value_kind!r}")

    next_state = replace(
        state,
        gpr=tuple(gprs),
        ctr=ops.const(int(summary.final_ctr) & 0xFFFFFFFF),
    )

    if symbolic_bus_append_fn is not None:
        symbolic_bus_append_fn(descriptor)

    return next_state, descriptor


@dataclass(frozen=True)
class _ParsedGxFifoBody:
    address: int
    width: int
    base_reg: int
    source_reg: int
    value_kind: str  # "invariant" | "affine"
    value_step: int | None


def _parse_gx_fifo_body(
    body: Sequence[Instruction],
    *,
    header_index: int,
    instructions: Sequence[Instruction],
    gx_base: int,
    gx_span: int,
    readonly_words: Mapping[int, int] | None,
) -> tuple[_ParsedGxFifoBody | None, list[str]]:
    """Accept only a single narrow GX store, optionally with a trailing affine addi."""
    if not body:
        return None, ["empty loop body"]
    if len(body) not in (1, 2):
        return None, ["body must be a single GX store or store-then-affine-addi"]

    store_insn = body[0]
    if store_insn.opcode not in _STORE_WIDTH:
        return None, [f"first body insn must be stb/sth/stw, got {store_insn.opcode.value}"]

    width = _STORE_WIDTH[store_insn.opcode]
    source_reg, base_reg, disp = (int(v) for v in store_insn.operands)
    if source_reg == base_reg:
        return None, ["store source equals base register"]

    address = _resolve_store_address(
        instructions,
        header_index,
        base_reg,
        disp,
        readonly_words=readonly_words,
    )
    if address is None:
        return None, [f"store base r{base_reg} does not resolve to a concrete address"]
    if address % width != 0:
        return None, [f"address 0x{address:08X} misaligned for width {width}"]
    if not (gx_base <= address and address + width <= gx_base + gx_span):
        return None, [f"address 0x{address:08X} outside GX region"]

    if len(body) == 1:
        return (
            _ParsedGxFifoBody(
                address=address,
                width=width,
                base_reg=base_reg,
                source_reg=source_reg,
                value_kind="invariant",
                value_step=None,
            ),
            [],
        )

    addi_insn = body[1]
    if addi_insn.opcode != Opcode.ADDI:
        return None, ["second body insn must be a value-affine addi"]
    rt, ra, imm = (int(v) for v in addi_insn.operands)
    if rt != source_reg or ra != source_reg:
        return None, [f"addi r{rt}, r{ra}, {imm} does not self-increment store source r{source_reg}"]
    if rt == base_reg:
        return None, ["affine addi must not target the GX base register"]

    return (
        _ParsedGxFifoBody(
            address=address,
            width=width,
            base_reg=base_reg,
            source_reg=source_reg,
            value_kind="affine",
            value_step=_sign_extend_16(imm),
        ),
        [],
    )


def _resolve_store_address(
    instructions: Sequence[Instruction],
    header_index: int,
    base_reg: int,
    disp: int,
    *,
    readonly_words: Mapping[int, int] | None,
) -> int | None:
    if base_reg == 0:
        return _sign_extend_16(disp) & 0xFFFFFFFF
    base_value, _notes = recover_gpr_constant(
        instructions,
        header_index,
        base_reg,
        readonly_words=readonly_words,
    )
    if base_value is None:
        return None
    return (int(base_value) + _sign_extend_16(disp)) & 0xFFFFFFFF


def _value_expr_for_candidate(
    candidate: GxFifoLoopCandidate,
    instructions: Sequence[Instruction],
    header_index: int,
    readonly_words: Mapping[int, int] | None,
) -> dict[str, Any]:
    initial, _notes = recover_gpr_constant(
        instructions,
        header_index,
        candidate.source_reg,
        readonly_words=readonly_words,
    )
    if candidate.value_kind == "invariant":
        if initial is not None:
            return {"kind": "invariant", "reg": candidate.source_reg, "value": int(initial)}
        return {"kind": "invariant", "reg": candidate.source_reg}
    # affine
    step = int(candidate.value_step or 0)
    if initial is not None:
        return {
            "kind": "affine",
            "reg": candidate.source_reg,
            "initial": int(initial),
            "step": step,
        }
    return {"kind": "affine", "reg": candidate.source_reg, "step": step}


def _value_expr_reg(value_expr: Mapping[str, Any]) -> int | None:
    reg = value_expr.get("reg")
    return int(reg) if reg is not None else None


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


def _sign_extend_16(value: int) -> int:
    word = int(value) & 0xFFFF
    if word >= 0x8000:
        word -= 0x10000
    return word & 0xFFFFFFFF
