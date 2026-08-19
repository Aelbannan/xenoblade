"""Conservative AbiShape inference from decoded bodies and optional symbol hints.

v2 policy (fail-closed):
- Narrow ``outgoing_gpr_args`` / ``outgoing_fpr_args`` only when **both** sides
  are simple vtable dispatches that never touch outgoing-arg GPRs ``r4``–``r10``
  or ``f1`` (CTR scratch must stay in non-arg volatiles such as ``r11``/``r12``).
- A mangled ``…Fv`` (void / no extra int/float args) hint may only *annotate*
  an already-structural narrow; it must **never** override a body that touches
  ``r4``–``r10`` / ``f1``. Xenoblade/MWCC often still passes hidden register
  args under shortened ``…Fv`` names (see ``docs/MWCC_PATTERNS.md``).
- ``endswith("Fv")`` alone is rejected: ``…FPFv`` / ``…FPCFv`` are
  function-pointer parameter lists, not void-no-args.
- Set ``returns_i64=False`` only when both sides return and neither writes ``r4``.
"""

from __future__ import annotations

import re

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

# Fixed MWCC Q-namespace prefixes (same set as tools/symbolrecover/lib/mwcc.py).
_KNOWN_Q_PREFIXES = (
    "Q26mpfsys",
    "Q23LOD",
    "Q22ml",
    "Q22cf",
)

# No ^ anchor: Pattern.match(blob, pos) must match at ``pos``, and Python's
# ``^`` does not rematch after a non-zero start offset.
_LEN_PREFIXED_IDENT = re.compile(r"(\d{1,3})([A-Za-z_][A-Za-z0-9_:]*)")


def infer_abi_shape(
    original: list[Instruction],
    candidate: list[Instruction],
    *,
    symbol: str | None = None,
    enabled: bool = True,
) -> AbiShape:
    """Infer a conservative AbiShape, or the fail-closed default.

    Symbol-based ``Fv`` annotation is opt-in via ``enabled`` (default True when
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

    # Structural narrow requires CTR scratch outside outgoing-arg GPRs r4–r10
    # (r11/r12 only). Using r4/r5 as the vtable load chain clobbers a live arg
    # — never narrow in that case, even when the mangled name ends in Fv.
    structural = (
        _is_simple_vtable_dispatch(original)
        and _is_simple_vtable_dispatch(candidate)
        and not _touches_outgoing_arg_gprs(original)
        and not _touches_outgoing_arg_gprs(candidate)
        and not _touches_fpr(original, 1)
        and not _touches_fpr(candidate, 1)
    )

    if structural:
        outgoing_gpr_args = 1
        outgoing_fpr_args = 0
        reasons.append("simple-vtable-dispatch")
        if _symbol_suggests_void_no_extra_args(symbol):
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
    """True only when the MWCC parameter blob is exactly ``Fv``.

    Rejects nested function-pointer encodings such as ``FPFv`` / ``FPCFv`` that
    merely *end* with the characters ``Fv``. Unparseable mangling fails closed
    (returns False — no void hint).
    """
    if not symbol:
        return False
    return _mwcc_params_blob(symbol) == "Fv"


def _mwcc_params_blob(symbol: str) -> str | None:
    """Return the trailing MWCC ``F…`` parameter encoding, or None."""
    if symbol.startswith("__ct__") or symbol.startswith("__dt__"):
        tail = symbol[6:]
    elif "__" in symbol:
        tail = symbol.split("__", 1)[1]
    else:
        return None
    if not tail:
        return None

    # Member: <len><ClassName>F…
    if tail[0].isdigit():
        consumed = _skip_length_prefixed(tail, 0)
        if consumed is not None and consumed < len(tail) and tail[consumed] == "F":
            return tail[consumed:]
        return None

    # Qualified: Q22cf5CHelpF… / generic Qn + n idents + F…
    if tail.startswith("Q"):
        consumed = _skip_q_encoding(tail)
        if consumed is not None and consumed < len(tail) and tail[consumed] == "F":
            return tail[consumed:]
        return None

    # Free function: entire tail is the param list (Fv, Fi, FPFv, …).
    if tail.startswith("F"):
        return tail
    return None


def _skip_length_prefixed(blob: str, pos: int) -> int | None:
    match = _LEN_PREFIXED_IDENT.match(blob, pos)
    if match is None:
        return None
    length = int(match.group(1))
    name = match.group(2)
    if len(name) < length:
        return None
    # Length prefix counts only the declared identifier span.
    return pos + len(match.group(1)) + length


def _skip_q_encoding(tail: str) -> int | None:
    for prefix in _KNOWN_Q_PREFIXES:
        if tail.startswith(prefix):
            return _skip_length_prefixed(tail, len(prefix))

    # Generic: Q<count><ident>×count (CodeWarrior nested-name encoding).
    if len(tail) < 2 or tail[0] != "Q" or not tail[1].isdigit():
        return None
    count = int(tail[1])
    if count <= 0:
        return None
    pos = 2
    for _ in range(count):
        nxt = _skip_length_prefixed(tail, pos)
        if nxt is None:
            return None
        pos = nxt
    return pos


# Mapping per §2.1: declared return type class to narrowing AbiShape.
# ``aggregate``, ``f128``, and unknown/absent types produce no narrowing.
DECLARED_RETURN_SHAPES: dict[str, AbiShape] = {
    "void": AbiShape(
        returns_i64=False, returns_float=False,
        source="declared-return:void", declared_return="void",
    ),
    "i32": AbiShape(
        returns_i64=False, returns_float=False,
        source="declared-return:i32", declared_return="i32",
    ),
    "u32": AbiShape(
        returns_i64=False, returns_float=False,
        source="declared-return:u32", declared_return="u32",
    ),
    "bool": AbiShape(
        returns_i64=False, returns_float=False,
        source="declared-return:bool", declared_return="bool",
    ),
    "ptr": AbiShape(
        returns_i64=False, returns_float=False,
        source="declared-return:ptr", declared_return="ptr",
    ),
    "f32": AbiShape(
        returns_i64=False, returns_float=True,
        source="declared-return:f32", declared_return="f32",
    ),
    "f64": AbiShape(
        returns_i64=False, returns_float=True,
        source="declared-return:f64", declared_return="f64",
    ),
    "i64": AbiShape(
        returns_i64=True, returns_float=False,
        source="declared-return:i64", declared_return="i64",
    ),
    "u64": AbiShape(
        returns_i64=True, returns_float=False,
        source="declared-return:u64", declared_return="u64",
    ),
}


def abi_shape_from_declared_return(declared: str | None) -> AbiShape | None:
    """Narrowing shape for a trusted return-type declaration.

    Returns None for None/unknown/``"aggregate"``/``"f128"`` (no narrowing).
    Returns a narrowing :class:`AbiShape` for all other §2.1 type classes.

    Shape source is ``"declared-return:<declared>"``.
    """
    if declared is None:
        return None
    return DECLARED_RETURN_SHAPES.get(declared)


def combine_abi_shapes(
    inferred: AbiShape,
    declared: AbiShape | None,
) -> AbiShape:
    """Fail-closed conjunction of inferred and declared ABI shapes (§2.3).

    ``declared=None`` returns ``inferred`` unchanged.
    When both shapes are present, the combined shape's ``returns_i64`` is
    ``inferred.returns_i64 AND declared.returns_i64``; same for
    ``returns_float``. ``outgoing_gpr_args`` / ``outgoing_fpr_args`` come from
    the inferred shape. ``declared_return`` is preserved from the declared
    shape, falling back to the inferred shape's value when the declared
    shape's is None (or from inferred if declared itself is None).

    Source strings are joined with ``"+"`` in order (inferred first, then
    declared), skipping ``"default-conservative"`` and duplicates.
    """
    if declared is None:
        return inferred

    sources: list[str] = []
    for src in (inferred.source, declared.source):
        if src != "default-conservative" and src not in sources:
            sources.append(src)
    source = "+".join(sources) if sources else "default-conservative"

    return AbiShape(
        returns_i64=inferred.returns_i64 and declared.returns_i64,
        returns_float=inferred.returns_float and declared.returns_float,
        outgoing_gpr_args=inferred.outgoing_gpr_args,
        outgoing_fpr_args=inferred.outgoing_fpr_args,
        source=source,
        declared_return=(
            declared.declared_return
            if declared.declared_return is not None
            else inferred.declared_return
        ),
    )
