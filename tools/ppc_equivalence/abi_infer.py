"""Conservative AbiShape inference from decoded bodies and optional symbol hints.

v1 policy (fail-closed):
- Narrow ``outgoing_gpr_args`` / ``outgoing_fpr_args`` only when both sides are
  simple vtable dispatches that never touch outgoing-arg GPRs ``r4``–``r10`` or
  ``f1`` (CTR scratch must stay in non-arg volatiles such as ``r11``/``r12``),
  OR when a symbol mangling hint says ``Fv`` (void / no extra int/float args).
- If either side uses ``r4``–``r10`` as scratch, do **not** auto-narrow unless
  the symbol ``Fv`` hint is present (caller may still pass an explicit AbiShape).
- Set ``returns_i64=False`` only when both sides return and neither writes ``r4``.
"""

from __future__ import annotations

from .abi_shape import AbiShape
from .ir import Instruction, Opcode
from .semantics import register_effects

_CTR_SPR = 9

# Ops allowed inside a short virtual thunk (this-adjust + vtable load + bctr).
_VTABLE_DISPATCH_OPS = frozenset({
    Opcode.LWZ,
    Opcode.LWZU,
    Opcode.ADDI,
    Opcode.ADDIS,
    Opcode.ADD,
    Opcode.MTSPR,
    Opcode.BCCTR,
})


def infer_abi_shape(
    original: list[Instruction],
    candidate: list[Instruction],
    *,
    symbol: str | None = None,
    enabled: bool = True,
) -> AbiShape:
    """Infer a conservative AbiShape, or the fail-closed default.

    Symbol-based ``Fv`` narrowing is opt-in via ``enabled`` (default True when
    callers choose to invoke inference). Pass ``enabled=False`` to always get
    :meth:`AbiShape.conservative`.
    """
    if not enabled:
        return AbiShape.conservative()

    reasons: list[str] = []
    returns_i64 = True
    returns_float = True
    outgoing_gpr_args = 8
    outgoing_fpr_args = 8

    if (
        _contains_return(original)
        and _contains_return(candidate)
        and not _writes_gpr(original, 4)
        and not _writes_gpr(candidate, 4)
    ):
        returns_i64 = False
        reasons.append("no-r4-write-return")

    symbol_void = _symbol_suggests_void_no_extra_args(symbol)
    # Structural narrow requires CTR scratch outside outgoing-arg GPRs r4–r10
    # (r11/r12 only). Using r5 as the vtable load chain clobbers a live arg.
    structural = (
        _is_simple_vtable_dispatch(original)
        and _is_simple_vtable_dispatch(candidate)
        and not _touches_outgoing_arg_gprs(original)
        and not _touches_outgoing_arg_gprs(candidate)
        and not _touches_fpr(original, 1)
        and not _touches_fpr(candidate, 1)
    )

    if structural or symbol_void:
        outgoing_gpr_args = 1
        outgoing_fpr_args = 0
        if structural:
            reasons.append("simple-vtable-dispatch")
        if symbol_void:
            reasons.append("symbol:Fv")

    if (
        returns_i64
        and returns_float
        and outgoing_gpr_args == 8
        and outgoing_fpr_args == 8
    ):
        return AbiShape.conservative()

    source = "+".join(reasons) if reasons else "inferred"
    return AbiShape(
        returns_i64=returns_i64,
        returns_float=returns_float,
        outgoing_gpr_args=outgoing_gpr_args,
        outgoing_fpr_args=outgoing_fpr_args,
        source=source,
    )


def _contains_return(insns: list[Instruction]) -> bool:
    return any(insn.opcode == Opcode.BCLR and not insn.link for insn in insns)


def _is_simple_vtable_dispatch(insns: list[Instruction]) -> bool:
    """Short virtual wrapper ending in ``mtctr`` + unlinked ``bctr``."""
    if not insns or len(insns) > 8 or len(insns) < 2:
        return False
    for insn in insns:
        if insn.link:
            return False
        if insn.opcode in (Opcode.B, Opcode.BC, Opcode.BCLR):
            return False
        if insn.opcode not in _VTABLE_DISPATCH_OPS:
            return False
    mtctr, bctr = insns[-2], insns[-1]
    if mtctr.opcode != Opcode.MTSPR or int(mtctr.operands[1]) != _CTR_SPR:
        return False
    if bctr.opcode != Opcode.BCCTR or bctr.link:
        return False
    return True


def _writes_gpr(insns: list[Instruction], reg: int) -> bool:
    name = f"r{reg}"
    for insn in insns:
        _, writes = register_effects(insn)
        if name in writes:
            return True
    return False


def _touches_gpr(insns: list[Instruction], reg: int) -> bool:
    name = f"r{reg}"
    for insn in insns:
        reads, writes = register_effects(insn)
        if name in reads or name in writes:
            return True
    return False


def _touches_outgoing_arg_gprs(insns: list[Instruction]) -> bool:
    """True when any of r4–r10 (outgoing args beyond ``this``) is read or written."""
    return any(_touches_gpr(insns, reg) for reg in range(4, 11))


def _touches_fpr(insns: list[Instruction], reg: int) -> bool:
    names = {f"f{reg}", f"f{reg}.ps1"}
    for insn in insns:
        reads, writes = register_effects(insn)
        if names & reads or names & writes:
            return True
    return False


def _symbol_suggests_void_no_extra_args(symbol: str | None) -> bool:
    """MWCC Itanium-ish void / no-extra-int-arg mangling hints (``Fv``)."""
    if not symbol:
        return False
    if symbol.endswith("Fv") or "__Fv" in symbol or "Fv)" in symbol:
        return True
    return False
