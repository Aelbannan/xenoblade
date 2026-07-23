"""Shared stack-pointer escape marking for store and terminal effects.

When a store publishes an ``r1``-derived pointer (the entry stack pointer, or a
value computed from it) **outside** the private frame, the per-implementation
private-stack masking must be disabled: the frame becomes observable to
callers or aliasing code, so the two implementations can no longer be masked
independently.

Spills of r1-derived values back onto r1-relative addresses stay private.
That covers the standard MWCC ``stwu r1,-N(r1)`` back-chain save and
``stw r1,disp(r1)`` frame slots: the published word remains inside
``[stack_low, entry_sp)`` and is masked like any other private-frame byte.
Publishing the same value to a non-r1-relative address still escapes.

Load-derived values are a second escape class: a ``Select`` from memory may
hold the entry stack pointer (or an r1-derived pointer previously spilled).
Walking the ``Store``/``Select`` memory spine on every store is super-linear, so
escape detection never descends into array arguments. Instead, any store of a
load-derived word to an address that is **not** itself r1-relative is treated
as a potential SP publish (fail closed). Spills back to the private frame
(r1-relative addresses) keep masking.

A third escape class is **compared-register publication** at a terminal: if any
contract-compared GPR other than ``r1`` syntactically depends on
``input.gpr.r1`` (for example ``addi r3,r1,8`` left live at ``blr``), the
caller can dereference that pointer into the private frame after return.
Masking must then be cleared even though no public *store* of the pointer
occurred. Temporary frame-pointer registers that are not compared at the
exit (for example volatile ``r11``) do not trigger this gate.

Store-path logic is shared between ordinary ``execute_instruction`` stores
(``semantics._mark_stack_pointer_escape``) and summarized closed-form
memory-loop stores (``memory_semantics.apply_store_effect``). The register
publish gate is applied by the equivalence engine after CFG exploration,
once the exit-kind observable set is known.
"""

from __future__ import annotations

from dataclasses import replace
from typing import Any


_ARRAY_KINDS: frozenset[int] | None = None
_SELECT_KIND: int | None = None


def _array_kinds(z3: Any) -> frozenset[int]:
    global _ARRAY_KINDS
    if _ARRAY_KINDS is None:
        kinds = [
            getattr(z3, name, None)
            for name in (
                "Z3_OP_STORE",
                "Z3_OP_SELECT",
                "Z3_OP_CONST_ARRAY",
                "Z3_OP_ARRAY_MAP",
                "Z3_OP_ARRAY_DEFAULT",
                "Z3_OP_AS_ARRAY",
            )
        ]
        _ARRAY_KINDS = frozenset(kind for kind in kinds if isinstance(kind, int))
    return _ARRAY_KINDS


def _select_kind(z3: Any) -> int | None:
    global _SELECT_KIND
    if _SELECT_KIND is None:
        kind = getattr(z3, "Z3_OP_SELECT", None)
        _SELECT_KIND = kind if isinstance(kind, int) else -1
    return None if _SELECT_KIND < 0 else _SELECT_KIND


def _walk_bv_cone(stored_value: Any, z3: Any, *, stop_on_select: bool) -> bool:
    """Walk the BV cone of ``stored_value``, skipping array ``Store``/``Select`` spines.

    When ``stop_on_select`` is true, encountering a ``Select`` returns True
    immediately (load-derived value) without descending into the memory array.
    When false, ``Select`` nodes are skipped like other array ops and the walk
    looks only for the ``input.gpr.r1`` free variable.
    """
    array_kinds = _array_kinds(z3)
    select_kind = _select_kind(z3)
    seen: set[int] = set()
    stack: list[Any] = [stored_value]
    while stack:
        current = stack.pop()
        # Prefer Z3's stable AST id — Python ``id()`` can be reused after GC and
        # spuriously skip subtrees (flaky missed escapes in suite runs).
        try:
            marker = int(current.get_id())
        except Exception:
            marker = id(current)
        if marker in seen:
            continue
        seen.add(marker)
        try:
            decl = current.decl()
        except AttributeError:
            continue
        try:
            kind = decl.kind()
        except Exception:
            kind = None
        if stop_on_select and select_kind is not None and kind == select_kind:
            return True
        if kind in array_kinds:
            continue
        try:
            name = decl.name()
        except Exception:
            name = None
        if not stop_on_select and name == "input.gpr.r1":
            return True
        try:
            nargs = current.num_args()
        except Exception:
            continue
        for index in range(nargs):
            stack.append(current.arg(index))
    return False


def bv_depends_on_entry_r1(stored_value: Any, z3: Any) -> bool:
    """Return True when ``input.gpr.r1`` appears in the BV cone of ``stored_value``.

    Skips Array ``Store``/``Select`` spines so load-modify-store sequences stay
    linear. Direct and arithmetic r1-derived pointers are still detected.
    """
    return _walk_bv_cone(stored_value, z3, stop_on_select=False)


def bv_is_load_derived(stored_value: Any, z3: Any) -> bool:
    """Return True when ``stored_value`` contains a memory ``Select`` in its BV cone.

    Does not walk into the memory-array argument of ``Select``/``Store`` (avoids
    super-linear spine walks). A loaded word may equal the entry stack pointer.
    """
    return _walk_bv_cone(stored_value, z3, stop_on_select=True)


def mark_stack_pointer_escape(
    state: Any,
    stored_value: Any,
    ops: Any,
    *,
    store_address: Any | None = None,
) -> Any:
    """Disable private-stack masking when ``stored_value`` may publish the SP.

    Escapes when:
    1. ``stored_value`` syntactically depends on ``input.gpr.r1`` **and** the
       store address is missing or not r1-relative (public SP publish), or
    2. ``stored_value`` is load-derived (contains ``Select``) **and** the store
       address is missing or not r1-relative — publishing a loaded word outside
       the private frame may reveal the stack pointer when ``mem[addr] == r1``.

    r1-derived spills to r1-relative addresses (``stwu`` back-chain,
    ``stw r1,disp(r1)``) keep masking — the word stays in the private frame.

    Only meaningful under symbolic execution: ``SymbolicOps`` exposes ``.z3`` and
    binds the entry stack pointer to the Z3 variable ``input.gpr.r1``. Concrete
    execution has no symbolic ``input.gpr.r1`` to trace and returns ``state``
    unchanged.
    """
    z3 = getattr(ops, "z3", None)
    if z3 is None:
        return state
    # Sticky: once cleared, further stores cannot re-enable masking.
    if state.stack_private is False:
        return state
    frame_relative = (
        store_address is not None and bv_depends_on_entry_r1(store_address, z3)
    )
    if bv_depends_on_entry_r1(stored_value, z3):
        # Back-chain / SP spill into the private frame stays masked.
        if frame_relative:
            return state
        return replace(state, stack_private=ops.bool(False))
    if bv_is_load_derived(stored_value, z3):
        # Spill of a loaded word back onto an r1-relative address stays private.
        if frame_relative:
            return state
        return replace(state, stack_private=ops.bool(False))
    return state


def compared_registers_publish_entry_r1(
    state: Any,
    gpr_indices: tuple[int, ...] | list[int],
    ops: Any,
) -> bool:
    """Return True when a compared GPR (other than r1) depends on entry ``r1``.

    ``r1`` itself always depends on ``input.gpr.r1`` after a normal prologue and
    must not be treated as a publish. Only other compared GPRs count.
    """
    z3 = getattr(ops, "z3", None)
    if z3 is None:
        return False
    for index in gpr_indices:
        reg = int(index)
        if reg == 1 or not 0 <= reg < 32:
            continue
        if bv_depends_on_entry_r1(state.gpr[reg], z3):
            return True
    return False


def apply_compared_register_publish_escape(
    state: Any,
    gpr_indices: tuple[int, ...] | list[int],
    ops: Any,
) -> Any:
    """Clear ``stack_private`` when a compared GPR publishes an r1-derived value.

    Fail-closed and sticky: once cleared, masking cannot be re-enabled. No-op
    under ConcreteOps (no symbolic ``input.gpr.r1`` cone) and when masking is
    already disabled.
    """
    z3 = getattr(ops, "z3", None)
    if z3 is None:
        return state
    if state.stack_private is False:
        return state
    if isinstance(state.stack_private, z3.BoolRef) and z3.is_false(state.stack_private):
        return state
    if not compared_registers_publish_entry_r1(state, gpr_indices, ops):
        return state
    return replace(state, stack_private=ops.bool(False))
