"""Unified exact floating-point outcome scaffold (Track C).

``FPOutcome`` is the shared container for SoftFloat / ConcreteOps /
SymbolicOps result bits, exception flags, FPRF, writeback/trap policy, and
supportedness. Wave 3 Track B adds paired-single lane producers and
``combine_paired_outcomes`` for ConcreteOps oracle paths; SymbolicOps still
uses host-float / Z3 lane execution. Nothing here promotes FP proofs out of
Tier C.

**SoftFloat already covers (exact integer-significand RNE bits):** scalar
``fadd``/``fadds``/``fmul``/``fmuls``/``fsub``/``fsubs``/``fdiv``/``fdivs`` and
fused ``fmadd``/``fmadds``/``fmsub``/``fmsubs``/``fnmadd``/``fnmadds``/
``fnmsub``/``fnmsubs`` for finite normals/zeros, subnormals, ±Inf, quiet /
signaling NaN propagation, division by zero, and overflow (see ``fp_oracle``).

**Paired-single (Wave 3 Track B, ConcreteOps):** ``ps_add``/``ps_sub``,
``ps_mul``, and ``ps_madd``/``ps_msub``/``ps_nmadd``/``ps_nmsub`` run two scalar
single-lane oracle outcomes and merge via ``combine_paired_outcomes`` (PS0
FPRF, accumulated VX subcauses, unconditional lane writeback). Other paired
families remain on the legacy semantics path or fail closed with explicit
reasons.

**FPSCR.NI (Wave 4 Track B / PR17):** supported opcodes apply Broadway
flush-to-zero on denormal operands and results when ``FPSCR.NI=1`` (see
``NI_SUPPORTED_OPS``). Unsupported NI-affected opcodes fail closed when NI
may be set. Still **Tier C** only.

**Deferred (Fraction oracle / FMA residual Track C follow-ups):**
- Fraction / rational exact cross-check oracle for finite values
- Full single-round FMA residual modeling (Broadway midpoint-tie with
  nonzero addend; near-cancellation sticky residues)
- SymbolicOps native ``FPOutcome`` paired lane producers
- SymbolicOps OX/UX/XX sticky / OE/UE/XE trap predicates
- NI for estimates (``fres``/``frsqrte``/``ps_res``/``ps_rsqrte``), ``frsp``,
  converts, compares, stores, and non-oracle paired families
- Trap delivery for estimates, compares, conversions; MSR FE0/FE1 modes

**PR18 trap delivery:** ``fp_traps.resolve_fp_trap_policy`` / CFG forking under
``FloatingPointDomain.traps_enabled`` deliver VE/ZE/OE/UE/XE program exceptions
for the scalar SoftFloat set and Wave-3 paired-oracle ops; incomplete opcodes
fail closed. See ``fp_traps.py``.
"""

from __future__ import annotations

from dataclasses import dataclass
from typing import Any

from .fp_oracle import (
    FpOracleFlags,
    FpOracleResult,
    NI_SCALAR_SUPPORTED_OPS,
    fprf_from_binary64,
    mask64,
)

# Broadway FPSCR VX subcause bits (Gekko layout; mirror semantics.py).
FPSCR_VXSNAN = 1 << 24
FPSCR_VXISI = 1 << 23
FPSCR_VXIMZ = 1 << 20
FPSCR_FI = 1 << 17
FPSCR_FR = 1 << 18
FPSCR_FPRF_MASK = 0x1F << 12

# Wave 3 Track B: initial paired-single oracle family (Tier C only).
PAIRED_ORACLE_BASIC_OPS: frozenset[str] = frozenset({
    "ps_add",
    "ps_sub",
    "ps_mul",
})
PAIRED_ORACLE_FUSED_OPS: frozenset[str] = frozenset({
    "ps_madd",
    "ps_msub",
    "ps_nmadd",
    "ps_nmsub",
})
PAIRED_ORACLE_OPS: frozenset[str] = PAIRED_ORACLE_BASIC_OPS | PAIRED_ORACLE_FUSED_OPS

# Identity string for certificates / cache when paired-single oracle ops appear.
# Bump when lane combine / SoftFloat paired semantics change (PR13).
FP_ORACLE_VERSION = "broadway-softfloat-paired-single-v1"

# Wave 4 Track B: opcodes with concrete/symbolic FPSCR.NI flush modeling.
NI_SUPPORTED_OPS: frozenset[str] = NI_SCALAR_SUPPORTED_OPS | PAIRED_ORACLE_OPS


@dataclass(frozen=True)
class FPExceptionFlags:
    """IEEE-754 sticky exception indicators (concrete bool or symbolic)."""

    invalid: Any
    overflow: Any
    underflow: Any
    divide_by_zero: Any
    inexact: Any

    def to_dict(self) -> dict[str, Any]:
        return {
            "invalid": self.invalid,
            "overflow": self.overflow,
            "underflow": self.underflow,
            "divide_by_zero": self.divide_by_zero,
            "inexact": self.inexact,
        }


@dataclass(frozen=True)
class FPOutcome:
    """Unified FP operation outcome for exact / symbolic backends.

    ``result_bits`` is a tuple so paired-single lanes can share the type
    later (``(ps0, ps1)``); SoftFloat scalar adapters use a 1-tuple.
    Fields use ``Any`` so ConcreteOps (ints/bools) and SymbolicOps (Z3
    expressions) can share the container without a second hierarchy.
    """

    result_bits: tuple[Any, ...]
    flags: FPExceptionFlags
    invalid_cause: Any
    fprf: Any
    writeback: Any
    trap: Any
    supported: Any
    unsupported_reason: str | None = None

    def primary_bits(self) -> Any:
        """Return the first result-bits lane (scalar SoftFloat / FPR path)."""
        if not self.result_bits:
            raise ValueError("FPOutcome.result_bits is empty")
        return self.result_bits[0]

    def to_dict(self) -> dict[str, Any]:
        return {
            "result_bits": list(self.result_bits),
            "flags": self.flags.to_dict(),
            "invalid_cause": self.invalid_cause,
            "fprf": self.fprf,
            "writeback": self.writeback,
            "trap": self.trap,
            "supported": self.supported,
            "unsupported_reason": self.unsupported_reason,
        }


def clear_exception_flags() -> FPExceptionFlags:
    """Concrete all-clear sticky flags."""
    return FPExceptionFlags(
        invalid=False,
        overflow=False,
        underflow=False,
        divide_by_zero=False,
        inexact=False,
    )


def exception_flags(
    *,
    invalid: Any = False,
    overflow: Any = False,
    underflow: Any = False,
    divide_by_zero: Any = False,
    inexact: Any = False,
) -> FPExceptionFlags:
    """Build flags with optional concrete or symbolic sticky bits."""
    return FPExceptionFlags(
        invalid=invalid,
        overflow=overflow,
        underflow=underflow,
        divide_by_zero=divide_by_zero,
        inexact=inexact,
    )


def outcome_from_result_bits(
    *bits: Any,
    flags: FPExceptionFlags | None = None,
    invalid_cause: Any = 0,
    fprf: Any = 0,
    writeback: Any = True,
    trap: Any = False,
    supported: Any = True,
    unsupported_reason: str | None = None,
) -> FPOutcome:
    """Wrap a bits-only API result (ConcreteOps / SoftFloat ``.bits64`` path).

    Existing call sites that only need the bit pattern can keep returning
    ints; adapters here build an ``FPOutcome`` when a full container is
    required without replacing those APIs.
    """
    if not bits:
        raise ValueError("outcome_from_result_bits requires at least one lane")
    return FPOutcome(
        result_bits=tuple(bits),
        flags=flags if flags is not None else clear_exception_flags(),
        invalid_cause=invalid_cause,
        fprf=fprf,
        writeback=writeback,
        trap=trap,
        supported=supported,
        unsupported_reason=unsupported_reason,
    )


def unsupported_outcome(
    reason: str,
    *,
    result_bits: tuple[Any, ...] = (0,),
) -> FPOutcome:
    """Fail-closed placeholder when a backend cannot model an op/domain."""
    return FPOutcome(
        result_bits=result_bits,
        flags=clear_exception_flags(),
        invalid_cause=0,
        fprf=0,
        writeback=False,
        trap=False,
        supported=False,
        unsupported_reason=reason,
    )


def flags_from_oracle(flags: FpOracleFlags) -> FPExceptionFlags:
    """Map SoftFloat ``FpOracleFlags`` into the unified flag container."""
    return FPExceptionFlags(
        invalid=flags.invalid,
        overflow=flags.overflow,
        underflow=flags.underflow,
        divide_by_zero=flags.divide_by_zero,
        inexact=flags.inexact,
    )


def flags_to_oracle(flags: FPExceptionFlags) -> FpOracleFlags:
    """Map unified flags back to SoftFloat ``FpOracleFlags`` (concrete bools)."""
    return FpOracleFlags(
        invalid=bool(flags.invalid),
        overflow=bool(flags.overflow),
        underflow=bool(flags.underflow),
        divide_by_zero=bool(flags.divide_by_zero),
        inexact=bool(flags.inexact),
    )


def outcome_from_oracle(
    result: FpOracleResult,
    *,
    writeback: Any = True,
    trap: Any = False,
    invalid_cause: Any = 0,
) -> FPOutcome:
    """Lift a SoftFloat ``FpOracleResult`` into ``FPOutcome`` (scalar 1-tuple)."""
    return FPOutcome(
        result_bits=(mask64(result.bits64),),
        flags=flags_from_oracle(result.flags),
        invalid_cause=invalid_cause,
        fprf=result.fprf,
        writeback=writeback,
        trap=trap,
        supported=True,
        unsupported_reason=None,
    )


def oracle_from_outcome(outcome: FPOutcome) -> FpOracleResult:
    """Project a concrete scalar ``FPOutcome`` back to SoftFloat result.

    Requires a supported outcome with a single integer result lane. Symbolic
    fields are not accepted — round-trip SoftFloat adapters only.
    """
    if not outcome.supported:
        reason = outcome.unsupported_reason or "unsupported FPOutcome"
        raise ValueError(f"cannot project unsupported FPOutcome to oracle: {reason}")
    if len(outcome.result_bits) != 1:
        raise ValueError(
            "oracle_from_outcome expects a single scalar lane, "
            f"got {len(outcome.result_bits)}",
        )
    bits = outcome.result_bits[0]
    if not isinstance(bits, int):
        raise TypeError(
            "oracle_from_outcome requires concrete int result_bits "
            f"(got {type(bits).__name__})",
        )
    fprf = outcome.fprf
    if not isinstance(fprf, int):
        raise TypeError(
            "oracle_from_outcome requires concrete int fprf "
            f"(got {type(fprf).__name__})",
        )
    return FpOracleResult(
        bits64=mask64(bits),
        flags=flags_to_oracle(outcome.flags),
        fprf=fprf,
    )


def outcome_primary_bits(outcome: FPOutcome) -> Any:
    """Adapter for bits-only consumers (``ConcreteOps.fp_*_bits`` style)."""
    return outcome.primary_bits()


def outcome_with_computed_fprf(bits64: int, **kwargs: Any) -> FPOutcome:
    """Build a concrete scalar outcome and fill FPRF from SoftFloat tables."""
    bits = mask64(bits64)
    return outcome_from_result_bits(
        bits,
        fprf=fprf_from_binary64(bits),
        **kwargs,
    )


def merge_exception_flags(
    left: FPExceptionFlags,
    right: FPExceptionFlags,
) -> FPExceptionFlags:
    """Bitwise OR of sticky flags across paired lanes (concrete bools)."""
    return FPExceptionFlags(
        invalid=bool(left.invalid) or bool(right.invalid),
        overflow=bool(left.overflow) or bool(right.overflow),
        underflow=bool(left.underflow) or bool(right.underflow),
        divide_by_zero=bool(left.divide_by_zero) or bool(right.divide_by_zero),
        inexact=bool(left.inexact) or bool(right.inexact),
    )


def merge_invalid_cause(left: Any, right: Any) -> int:
    """OR Broadway VX subcause masks from two lanes."""
    return int(left or 0) | int(right or 0)


def cr1_from_fpscr(fpscr: int) -> int:
    """CR1 nibble copied from FPSCR for record-form FP ops."""
    return (fpscr >> 28) & 0xF


def combine_paired_outcomes(
    lane0: FPOutcome,
    lane1: FPOutcome,
    *,
    fprf_lane: int = 0,
) -> FPOutcome:
    """Merge two scalar lane outcomes into one paired-single container.

    - ``result_bits`` becomes ``(ps0, ps1)`` destination lane writeback values.
    - Exception flags and ``invalid_cause`` VX submasks accumulate across lanes.
    - ``fprf`` is taken from ``fprf_lane`` (Broadway uses PS0 / FPR high lane).
    - Paired arithmetic always sets ``writeback=True`` (Broadway unconditional
      destination update under enabled exceptions).
    """
    if not lane0.supported:
        reason = lane0.unsupported_reason or "unsupported paired lane0"
        return unsupported_outcome(reason, result_bits=(0, 0))
    if not lane1.supported:
        reason = lane1.unsupported_reason or "unsupported paired lane1"
        return unsupported_outcome(reason, result_bits=(0, 0))
    if len(lane0.result_bits) != 1 or len(lane1.result_bits) != 1:
        return unsupported_outcome(
            "combine_paired_outcomes expects single-lane scalar outcomes",
            result_bits=(0, 0),
        )
    fprf = lane0.fprf if fprf_lane == 0 else lane1.fprf
    return FPOutcome(
        result_bits=(lane0.result_bits[0], lane1.result_bits[0]),
        flags=merge_exception_flags(lane0.flags, lane1.flags),
        invalid_cause=merge_invalid_cause(lane0.invalid_cause, lane1.invalid_cause),
        fprf=fprf,
        writeback=True,
        trap=False,
        supported=True,
        unsupported_reason=None,
    )
