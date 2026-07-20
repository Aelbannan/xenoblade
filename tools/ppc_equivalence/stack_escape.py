"""Shared stack-pointer escape marking for store effects.

When a store publishes an ``r1``-derived pointer (the entry stack pointer, or a
value computed from it) into memory, the per-implementation private-stack
masking must be disabled: the frame becomes observable to callers or aliasing
code, so the two implementations can no longer be masked independently.

This logic is shared between ordinary ``execute_instruction`` stores
(``semantics._mark_stack_pointer_escape``) and summarized closed-form
memory-loop stores (``memory_semantics.apply_store_effect``) so both paths
enforce the identical escape rule.

Performance note: do **not** call ``z3util.get_vars`` on arbitrary store
values. Load-derived values embed the full ``Store``/``Select`` memory spine;
walking that spine on every store is super-linear and hangs ordinary
load-modify-store functions during certificate validation. Escape is a GPR
cone property: once an r1-derived pointer has been stored, ``stack_private``
is already cleared, so later loads of that pointer need not re-detect it.
"""

from __future__ import annotations

from dataclasses import replace
from typing import Any


_ARRAY_KINDS: frozenset[int] | None = None


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


def bv_depends_on_entry_r1(stored_value: Any, z3: Any) -> bool:
    """Return True when ``input.gpr.r1`` appears in the BV cone of ``stored_value``.

    Skips Array ``Store``/``Select`` spines so load-modify-store sequences stay
    linear. Sufficient because the first store of an r1-derived pointer already
    clears ``stack_private``.
    """
    array_kinds = _array_kinds(z3)
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
        if kind in array_kinds:
            continue
        try:
            name = decl.name()
        except Exception:
            name = None
        if name == "input.gpr.r1":
            return True
        try:
            nargs = current.num_args()
        except Exception:
            continue
        for index in range(nargs):
            stack.append(current.arg(index))
    return False


def mark_stack_pointer_escape(state: Any, stored_value: Any, ops: Any) -> Any:
    """Disable private-stack masking when ``stored_value`` derives from ``r1``.

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
    if not bv_depends_on_entry_r1(stored_value, z3):
        return state
    return replace(state, stack_private=ops.bool(False))
