"""Backend protocol for the scalar-FP v2 exact core (Phase 2).

The rounding kernel is parameterized over a minimal integer/bitvector algebra so
the same code can run under:

* :class:`IntBackend` — concrete Python ``int`` (Phase 2, fully working)
* a future Z3 ``BitVec`` backend (Phase 7) with identical operation names

Native host ``float`` must not appear in backend implementations used on
proof-critical paths.
"""

from __future__ import annotations

from typing import Any, Protocol, runtime_checkable


@runtime_checkable
class FPBackend(Protocol):
    """Minimal algebra shared by concrete and symbolic FP backends."""

    # Literal constructors -------------------------------------------------
    def const(self, value: int) -> int:
        """Return a backend value for integer literal ``value``."""
        ...

    # Bitwise / word ops ---------------------------------------------------
    def band(self, a: int, b: int) -> int:
        """Bitwise AND."""
        ...

    def bor(self, a: int, b: int) -> int:
        """Bitwise OR."""
        ...

    def bxor(self, a: int, b: int) -> int:
        """Bitwise XOR."""
        ...

    def bnot(self, a: int) -> int:
        """Bitwise NOT (within backend word width when applicable)."""
        ...

    def shl(self, value: int, amount: int) -> int:
        """Left shift."""
        ...

    def shr(self, value: int, amount: int) -> int:
        """Logical right shift."""
        ...

    # Integer arithmetic ---------------------------------------------------
    def add(self, a: int, b: int) -> int:
        """Add."""
        ...

    def sub(self, a: int, b: int) -> int:
        """Subtract."""
        ...

    def mul(self, a: int, b: int) -> int:
        """Multiply."""
        ...

    # Comparisons (concrete bool today; symbolic Bool in Phase 7) ----------
    def eq(self, a: int, b: int) -> bool:
        """Equality."""
        ...

    def lt(self, a: int, b: int) -> bool:
        """Signed or unsigned less-than — concrete ints use native ordering."""
        ...

    def le(self, a: int, b: int) -> bool:
        """Less-than or equal."""
        ...

    def gt(self, a: int, b: int) -> bool:
        """Greater-than."""
        ...

    # Boolean algebra (concrete bool today) --------------------------------
    def land(self, a: bool, b: bool) -> bool:
        """Logical AND."""
        ...

    def lor(self, a: bool, b: bool) -> bool:
        """Logical OR."""
        ...

    def lnot(self, a: bool) -> bool:
        """Logical NOT."""
        ...

    def ite(self, cond: bool, when_true: int, when_false: int) -> int:
        """If-then-else on integer values."""
        ...


class IntBackend:
    """Concrete Python-integer backend (Phase 2 default)."""

    __slots__ = ()

    def const(self, value: int) -> int:
        return int(value)

    def band(self, a: int, b: int) -> int:
        return a & b

    def bor(self, a: int, b: int) -> int:
        return a | b

    def bxor(self, a: int, b: int) -> int:
        return a ^ b

    def bnot(self, a: int) -> int:
        return ~a

    def shl(self, value: int, amount: int) -> int:
        return value << amount

    def shr(self, value: int, amount: int) -> int:
        return value >> amount

    def add(self, a: int, b: int) -> int:
        return a + b

    def sub(self, a: int, b: int) -> int:
        return a - b

    def mul(self, a: int, b: int) -> int:
        return a * b

    def eq(self, a: int, b: int) -> bool:
        return a == b

    def lt(self, a: int, b: int) -> bool:
        return a < b

    def le(self, a: int, b: int) -> bool:
        return a <= b

    def gt(self, a: int, b: int) -> bool:
        return a > b

    def land(self, a: bool, b: bool) -> bool:
        return a and b

    def lor(self, a: bool, b: bool) -> bool:
        return a or b

    def lnot(self, a: bool) -> bool:
        return not a

    def ite(self, cond: bool, when_true: int, when_false: int) -> int:
        return when_true if cond else when_false


class Z3BitVecBackend:
    """Z3 BitVec backend (Phase 7) — mirrors :class:`IntBackend` without host float.

    Comparisons and ``ite`` return Z3 ``BoolRef`` / ``BitVecRef`` AST nodes. Public
    methods must not branch on symbolic predicates in Python.
    """

    __slots__ = ("width", "z3")

    def __init__(self, width: int = 64) -> None:
        import z3

        if width <= 0:
            raise ValueError(f"BitVec width must be positive, got {width}")
        self.width = int(width)
        self.z3 = z3

    def _mask(self, value: int) -> int:
        if self.width >= 64:
            return value & 0xFFFFFFFFFFFFFFFF
        return value & ((1 << self.width) - 1)

    def const(self, value: int) -> Any:
        return self.z3.BitVecVal(self._mask(value), self.width)

    def band(self, a: Any, b: Any) -> Any:
        return a & b

    def bor(self, a: Any, b: Any) -> Any:
        return a | b

    def bxor(self, a: Any, b: Any) -> Any:
        return a ^ b

    def bnot(self, a: Any) -> Any:
        return ~a

    def shl(self, value: Any, amount: int) -> Any:
        return value << int(amount)

    def shr(self, value: Any, amount: int) -> Any:
        return self.z3.LShR(value, int(amount))

    def add(self, a: Any, b: Any) -> Any:
        return a + b

    def sub(self, a: Any, b: Any) -> Any:
        return a - b

    def mul(self, a: Any, b: Any) -> Any:
        return a * b

    def eq(self, a: Any, b: Any) -> Any:
        return a == b

    def lt(self, a: Any, b: Any) -> Any:
        return a < b

    def le(self, a: Any, b: Any) -> Any:
        return a <= b

    def gt(self, a: Any, b: Any) -> Any:
        return a > b

    def land(self, a: Any, b: Any) -> Any:
        return self.z3.And(a, b)

    def lor(self, a: Any, b: Any) -> Any:
        return self.z3.Or(a, b)

    def lnot(self, a: Any) -> Any:
        return self.z3.Not(a)

    def ite(self, cond: Any, when_true: Any, when_false: Any) -> Any:
        return self.z3.If(cond, when_true, when_false)


DEFAULT_INT_BACKEND = IntBackend()

__all__ = [
    "DEFAULT_INT_BACKEND",
    "FPBackend",
    "IntBackend",
    "Z3BitVecBackend",
]
