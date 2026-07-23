"""Typed store effects for closed-form memory-loop discharge.

``apply_memory_loop_summary`` must not mutate ``state.memory`` alone: every
closed-form store must also record ``memory_writes`` / ``memory_touches`` so
observational memory comparison sees the written footprint (PR2 typed tracking).
"""

from __future__ import annotations

from dataclasses import dataclass, replace
from typing import Any

from tools.ppc_equivalence.model import InvalidReason
from tools.ppc_equivalence.stack_escape import mark_stack_pointer_escape


@dataclass(frozen=True)
class StoreEffect:
    """One big-endian store of ``width`` bytes at ``address`` from ``value``."""

    address: Any
    value: Any
    width: int


@dataclass(frozen=True)
class MemoryLoopTransition:
    """Closed-form post-loop register/memory update for a store loop."""

    stores: tuple[StoreEffect, ...]
    base_reg: int
    base_delta: int
    final_ctr: int


def apply_store_effect(state: Any, effect: StoreEffect, ops: Any) -> Any:
    """Apply one store and append its byte addresses to write/touch tracking.

    Also constrains ``state.valid`` for width>1 alignment (same rule as ordinary
    D-form stores). Region / bus routing remains outside this helper.
    """
    if effect.width not in (1, 2, 4):
        raise ValueError(f"unsupported store width {effect.width}")
    current = state
    if effect.width > 1:
        aligned = ops.eq(
            ops.band(effect.address, ops.const(effect.width - 1)),
            ops.const(0),
        )
        new_valid = ops.land(current.valid, aligned)
        # Mirror semantics._constrain_valid without importing WordOps.
        if hasattr(ops, "z3"):
            z3 = ops.z3
            new_reason = z3.If(
                z3.And(current.valid, z3.Not(aligned)),
                z3.BitVecVal(InvalidReason.UNALIGNED_ACCESS.value, 8),
                current.invalid_reason,
            )
        else:
            if current.valid and not aligned:
                new_reason = InvalidReason.UNALIGNED_ACCESS.value
            else:
                new_reason = current.invalid_reason
        current = replace(current, valid=new_valid, invalid_reason=new_reason)

    memory = current.memory
    addrs: list[Any] = []
    for offset in range(effect.width):
        shift = (effect.width - 1 - offset) * 8
        byte = ops.band(ops.lshr(effect.value, ops.const(shift)), ops.const(0xFF))
        addr = ops.add(effect.address, ops.const(offset))
        memory = ops.store_byte(memory, addr, byte)
        addrs.append(addr)
    addrs_t = tuple(addrs)
    current = replace(
        current,
        memory=memory,
        memory_writes=current.memory_writes + addrs_t,
        memory_touches=current.memory_touches + addrs_t,
    )
    # Publishing an r1-derived / load-derived pointer through a summarized store
    # escapes the private stack exactly as an ordinary D-form store would.
    return mark_stack_pointer_escape(
        current, effect.value, ops, store_address=effect.address,
    )


def apply_memory_loop_transition(state: Any, transition: MemoryLoopTransition, ops: Any) -> Any:
    """Apply every store effect, then advance base GPR and CTR."""
    current = state
    for effect in transition.stores:
        current = apply_store_effect(current, effect, ops)
    gprs = list(current.gpr)
    base = state.gpr[transition.base_reg]
    gprs[transition.base_reg] = ops.add(base, ops.const(transition.base_delta & 0xFFFFFFFF))
    return replace(
        current,
        gpr=tuple(gprs),
        ctr=ops.const(int(transition.final_ctr) & 0xFFFFFFFF),
    )



def build_memory_loop_transition(
    state: Any,
    *,
    trip_count: int,
    base_reg: int,
    source_reg: int,
    stride: int,
    store_width: int,
    store_kind: str,
    final_ctr: int,
    ops: Any,
) -> MemoryLoopTransition:
    """Materialize N width-matched stores for a recognized constant-stride loop."""
    if trip_count < 1:
        raise ValueError("memory-loop transition requires a positive concrete trip count")
    if store_kind not in ("stwu", "d-form-addi"):
        raise ValueError(f"unsupported store_kind {store_kind!r}")
    if stride != store_width:
        raise ValueError("stride must equal store width for closed-form stores")

    base = state.gpr[base_reg]
    value = state.gpr[source_reg]
    stores: list[StoreEffect] = []
    for index in range(trip_count):
        if store_kind == "stwu":
            offset = (index + 1) * stride
        else:
            offset = index * stride
        address = ops.add(base, ops.const(offset & 0xFFFFFFFF))
        stores.append(StoreEffect(address=address, value=value, width=store_width))

    base_delta = (int(trip_count) * int(stride)) & 0xFFFFFFFF
    return MemoryLoopTransition(
        stores=tuple(stores),
        base_reg=base_reg,
        base_delta=base_delta,
        final_ctr=final_ctr,
    )


def footprint_wraps_u32(base: int, trip_count: int, stride: int, store_kind: str) -> bool:
    """True when the closed-form store span wraps the 32-bit address space.

    Uses unsigned arithmetic on the entry base (concrete checks only). Symbolic
    bases are gated separately via alignment constraints at apply time.
    """
    if trip_count < 1 or stride < 1:
        return True
    span = int(trip_count) * int(stride)
    if span > 0xFFFFFFFF:
        return True
    base_u = int(base) & 0xFFFFFFFF
    # Exit base = entry + N*stride; if that addition wraps, the footprint wrapped.
    if (base_u + span) > 0xFFFFFFFF:
        return True
    if store_kind == "stwu":
        first = (base_u + stride) & 0xFFFFFFFF
        last_byte = (base_u + span + stride - 1) & 0xFFFFFFFF
    else:
        first = base_u
        last_byte = (base_u + span - 1) & 0xFFFFFFFF
    # Contiguous ascending footprint must not wrap past 0.
    return last_byte < first


def footprint_ok_for_summary(
    *,
    trip_count: int,
    stride: int,
    store_width: int,
    store_kind: str,
) -> bool:
    """Static footprint gate used before summarizing a memory loop."""
    if trip_count < 1 or stride < 1:
        return False
    if stride != store_width:
        return False
    if store_kind not in ("stwu", "d-form-addi"):
        return False
    span = int(trip_count) * int(stride)
    if span > 0xFFFFFFFF:
        return False
    return True
