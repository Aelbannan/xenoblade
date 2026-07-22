"""Symbolic exact scalar-FP helpers and proof obligations (Phase 7).

Evaluates concrete bit patterns through :class:`Z3BitVecBackend` and provides
the ``feasible_path ∧ scalar_fp_unsupported`` discharge wrapper.

When ``SCALAR_FP_EXACT_V2`` is enabled, :mod:`semantics` routes SymbolicOps
scalar arithmetic/fused through :func:`try_dispatch_exact_scalar_v2` instead of
native Z3 FP.
"""

from __future__ import annotations

import hashlib
import time
from dataclasses import dataclass
from typing import Any, Callable, Literal, Sequence

from .discharge import _map_z3_answer, _query_sha256
from .fp_backend import IntBackend, Z3BitVecBackend
from .fp_capabilities import set_scalar_fp_exact_v2_module_flag
from .fp_exact import EXACT_SCALAR_ARITH_OPS, exact_fadd, exact_fmul, exact_scalar_arith
from .fp_exact_outcome import ScalarFPOutcome
from .fp_fpscr import apply_fpscr_transition

ScalarFPQueryStatus = Literal["unsat", "sat", "unknown", "timeout", "vacuous"]

_MASK64 = 0xFFFFFFFFFFFFFFFF
_MASK32 = 0xFFFFFFFF

_CLASS_REPS: dict[str, int] = {
    "snan": 0x7FF0000000000001,
    "qnan": 0x7FF8000000000000,
    "inf": 0x7FF0000000000000,
    "zero": 0x0000000000000000,
    "subnormal": 0x0000000000000001,
    "normal": 0x3FF0000000000000,
}


@dataclass(frozen=True)
class ScalarFPUnsupportedQuery:
    status: ScalarFPQueryStatus
    query_sha256: str
    elapsed_ms: float = 0.0
    algorithm: str = "scalar-fp-unsupported-v1"

    def to_dict(self) -> dict[str, Any]:
        return {
            "status": self.status,
            "query_sha256": self.query_sha256,
            "elapsed_ms": self.elapsed_ms,
            "algorithm": self.algorithm,
        }


@dataclass(frozen=True)
class ExactDispatchResult:
    outcome: ScalarFPOutcome
    result_bits: Any
    writeback: Any
    post_fpscr: Any
    symbolic: bool


def try_concrete_bv64(value: Any) -> int | None:
    """Return a concrete 64-bit value when ``value`` is a closed BitVec constant."""
    if value is None:
        return None
    if isinstance(value, int):
        return value & _MASK64
    as_long = getattr(value, "as_long", None)
    if callable(as_long):
        try:
            return int(as_long()) & _MASK64
        except Exception:
            pass
    try:
        import z3

        simplified = z3.simplify(value)
        if z3.is_bv_value(simplified):
            return int(simplified.as_long()) & _MASK64
    except Exception:
        return None
    return None


def try_concrete_bv32(value: Any) -> int | None:
    """Return a concrete 32-bit value when ``value`` is a closed BitVec constant."""
    if value is None:
        return None
    if isinstance(value, int):
        return value & _MASK32
    as_long = getattr(value, "as_long", None)
    if callable(as_long):
        try:
            return int(as_long()) & _MASK32
        except Exception:
            pass
    try:
        import z3

        simplified = z3.simplify(value)
        if z3.is_bv_value(simplified):
            return int(simplified.as_long()) & _MASK32
    except Exception:
        return None
    return None


def _bv64(ops: Any, value: int) -> Any:
    return ops.fp_const64(int(value) & _MASK64)


def _bool(ops: Any, value: bool) -> Any:
    return ops.bool(value)


def apply_fpscr_transition_expr(
    pre_fpscr: Any,
    opcode: str,
    outcome: ScalarFPOutcome,
    ops: Any,
) -> Any:
    pre_i = try_concrete_bv32(pre_fpscr)
    if pre_i is not None:
        return ops.const(apply_fpscr_transition(pre_i, opcode, outcome) & _MASK32)
    value = pre_fpscr
    causes = int(outcome.raised_causes) & 0xFFFFFFFF
    for bit in (
        1 << 28, 1 << 27, 1 << 26, 1 << 25,
        1 << 24, 1 << 23, 1 << 22, 1 << 21, 1 << 20, 1 << 19,
    ):
        if causes & bit:
            mask = ops.const(bit)
            already = ops.eq(ops.band(value, mask), mask)
            value = ops.bor(value, mask)
            value = ops.ite(already, value, ops.bor(value, ops.const(1 << 31)))
    if outcome.update_fprf:
        fprf = ops.shl(ops.const(int(outcome.fprf) & 0x1F), ops.const(12))
        value = ops.bor(ops.band(value, ops.bnot(ops.const(0x1F000))), fprf)
    if outcome.fi_fr_policy.value == "clear":
        cleared = ops.band(value, ops.bnot(ops.const((1 << 18) | (1 << 17))))
        if outcome.fi:
            cleared = ops.bor(cleared, ops.const(1 << 17))
        if outcome.fr:
            cleared = ops.bor(cleared, ops.const(1 << 18))
        value = cleared
    return value


def _is_nan_bv(ops: Any, bits: Any) -> Any:
    z3 = ops.z3
    exp = z3.Extract(62, 52, bits)
    frac = z3.Extract(51, 0, bits)
    return z3.And(exp == z3.BitVecVal(0x7FF, 11), frac != z3.BitVecVal(0, 52))


def _bv_class_is(ops: Any, bits: Any, rep_name: str) -> Any:
    from .fp_bits import FpClass, classify_binary64

    rep = _CLASS_REPS[rep_name]
    if try_concrete_bv64(bits) is not None:
        return _bool(ops, classify_binary64(try_concrete_bv64(bits) or 0) is {
            "snan": FpClass.SNAN,
            "qnan": FpClass.QNAN,
            "inf": FpClass.INFINITY,
            "zero": FpClass.ZERO,
            "subnormal": FpClass.SUBNORMAL,
            "normal": FpClass.NORMAL,
        }[rep_name])
    z3 = ops.z3
    exp = z3.Extract(62, 52, bits)
    frac = z3.Extract(51, 0, bits)
    exp_max = z3.BitVecVal(0x7FF, 11)
    exp_zero = exp == z3.BitVecVal(0, 11)
    frac_zero = frac == z3.BitVecVal(0, 52)
    mapping = {
        "inf": z3.And(exp == exp_max, frac_zero),
        "qnan": z3.And(exp == exp_max, z3.Extract(51, 51, frac) == z3.BitVecVal(1, 1)),
        "snan": z3.And(
            exp == exp_max,
            z3.And(z3.Not(frac_zero), z3.Not(z3.Extract(51, 51, frac) == z3.BitVecVal(1, 1))),
        ),
        "zero": z3.And(exp_zero, frac_zero),
        "subnormal": z3.And(exp_zero, z3.Not(frac_zero)),
        "normal": z3.And(z3.Not(exp_zero), z3.Not(exp == exp_max)),
    }
    return mapping[rep_name]


def _exact_arith_result_bits_bv(
    opcode: str,
    a_bits: Any,
    b_bits: Any,
    c_bits: Any,
    fpscr: Any,
    ops: Any,
) -> Any:
    """Bit-exact scalar arithmetic result as BitVec without native Z3 FP."""
    a_i = try_concrete_bv64(a_bits)
    b_i = try_concrete_bv64(b_bits)
    c_i = try_concrete_bv64(c_bits)
    fpscr_i = try_concrete_bv32(fpscr)
    if (
        a_i is not None
        and b_i is not None
        and c_i is not None
        and fpscr_i is not None
        and str(opcode) in EXACT_SCALAR_ARITH_OPS
    ):
        outcome = exact_scalar_arith(opcode, a_i, b_i, c_bits=c_i, fpscr=fpscr_i)
        if outcome is not None and outcome.supported:
            return _bv64(ops, outcome.result_bits)

    name = str(opcode)
    if name in ("fadd", "fadds", "fsub", "fsubs", "fmul", "fmuls", "fdiv", "fdivs"):
        from .fp_exact_symbolic_arith import exact_arith_result_bits_bv

        payload = exact_arith_result_bits_bv(name, a_bits, b_bits, c_bits, fpscr, ops)
        if payload is not None:
            return payload

    if name in ("fmul", "fmuls"):
        left, right = a_bits, c_bits
    else:
        left, right = a_bits, b_bits

    nan_l = _is_nan_bv(ops, left)
    nan_r = _is_nan_bv(ops, right)
    any_nan = ops.lor(nan_l, nan_r)
    nan_result = ops.ite(
        nan_l,
        ops.fp_quiet_nan_bits(left),
        ops.ite(nan_r, ops.fp_quiet_nan_bits(right), ops.fp_const64(0x7FF8000000000000)),
    )
    finite = _exact_arith_class_macro(name, a_bits, b_bits, c_bits, fpscr, ops)
    return ops.ite(any_nan, nan_result, finite)


def _exact_arith_class_macro(
    opcode: str,
    a_bits: Any,
    b_bits: Any,
    c_bits: Any,
    fpscr: Any,
    ops: Any,
) -> Any:
    """If-ladder over IEEE classes using the integer exact kernel at leaves."""
    fpscr_i = try_concrete_bv32(fpscr) or 0
    class_names = ("snan", "qnan", "inf", "zero", "subnormal", "normal")
    expr: Any = _bv64(ops, 0)
    for a_name in class_names:
        a_pred = _bv_class_is(ops, a_bits, a_name)
        a_rep = _CLASS_REPS[a_name]
        for b_name in class_names:
            b_pred = _bv_class_is(ops, b_bits, b_name)
            b_rep = _CLASS_REPS[b_name]
            for c_name in class_names:
                c_pred = _bv_class_is(ops, c_bits, c_name)
                c_rep = _CLASS_REPS[c_name]
                outcome = exact_scalar_arith(opcode, a_rep, b_rep, c_bits=c_rep, fpscr=fpscr_i)
                if outcome is None or not outcome.supported:
                    continue
                guard = ops.land(a_pred, ops.land(b_pred, c_pred))
                expr = ops.ite(guard, _bv64(ops, outcome.result_bits), expr)
    return expr


def _dispatch_exact_fused(
    opcode: str,
    a_bits: Any,
    b_bits: Any,
    c_bits: Any,
    fpscr: Any,
    msr: Any,
    ops: Any,
    *,
    scalar_outcome_from_fused: Callable[..., ScalarFPOutcome],
) -> ExactDispatchResult | None:
    a_i = try_concrete_bv64(a_bits)
    b_i = try_concrete_bv64(b_bits)
    c_i = try_concrete_bv64(c_bits)
    fpscr_i = try_concrete_bv32(fpscr)
    msr_i = try_concrete_bv32(msr)
    name = str(opcode)
    symbolic = (
        a_i is None
        or b_i is None
        or c_i is None
        or fpscr_i is None
    )

    if symbolic and name in {"fmadd", "fmsub", "fnmadd", "fnmsub"}:
        from .fp_exact_symbolic_arith import exact_fused_result_bits_bv

        result_bits = exact_fused_result_bits_bv(name, a_bits, c_bits, b_bits, fpscr, ops)
        if result_bits is None:
            return None
        from .fp_exact_fused import dispatch_exact_fused

        rep = dispatch_exact_fused(name, 0x3FF0000000000000, 0x3FF0000000000000, 0x3FF0000000000000)
        if not rep.supported:
            return None
        outcome = scalar_outcome_from_fused(
            rep,
            fpscr=0,
            msr=0,
            a_bits=0x3FF0000000000000,
            c_bits=0x3FF0000000000000,
            b_bits=0x3FF0000000000000,
        )
        if not outcome.supported:
            return None
        return ExactDispatchResult(
            outcome=outcome,
            result_bits=result_bits,
            writeback=_bool(ops, bool(outcome.writeback)),
            post_fpscr=apply_fpscr_transition_expr(fpscr, name, outcome, ops),
            symbolic=True,
        )

    if symbolic and name in {"fmadds", "fmsubs", "fnmadds", "fnmsubs"}:
        from .fp_exact_symbolic_arith import exact_fused_single_result_bits_bv

        result_bits = exact_fused_single_result_bits_bv(
            name, a_bits, b_bits, c_bits, fpscr, ops,
        )
        if result_bits is None:
            return None
        from .fp_exact_fused import dispatch_exact_fused

        rep = dispatch_exact_fused(name, 0x3FF0000000000000, 0x3FF0000000000000, 0x3FF0000000000000)
        if not rep.supported:
            return None
        outcome = scalar_outcome_from_fused(
            rep,
            fpscr=0,
            msr=0,
            a_bits=0x3FF0000000000000,
            c_bits=0x3FF0000000000000,
            b_bits=0x3FF0000000000000,
        )
        if not outcome.supported:
            return None
        return ExactDispatchResult(
            outcome=outcome,
            result_bits=result_bits,
            writeback=_bool(ops, bool(outcome.writeback)),
            post_fpscr=apply_fpscr_transition_expr(fpscr, name, outcome, ops),
            symbolic=True,
        )

    if a_i is None or b_i is None or c_i is None or fpscr_i is None:
        return None
    from .fp_exact_fused import dispatch_exact_fused

    if name.endswith("s"):
        fused = dispatch_exact_fused(name, a_i, b_i, c_i)
    else:
        fused = dispatch_exact_fused(name, a_i, c_i, b_i)
    if not fused.supported:
        return None
    outcome = scalar_outcome_from_fused(
        fused,
        fpscr=fpscr_i,
        msr=msr_i if msr_i is not None else 0,
        a_bits=a_i,
        c_bits=c_i,
        b_bits=b_i,
    )
    if not outcome.supported:
        return None
    return ExactDispatchResult(
        outcome=outcome,
        result_bits=_bv64(ops, outcome.result_bits),
        writeback=_bool(ops, bool(outcome.writeback)),
        post_fpscr=apply_fpscr_transition_expr(fpscr, name, outcome, ops),
        symbolic=False,
    )


def try_dispatch_exact_scalar_v2(
    *,
    opcode: str,
    a_bits: Any,
    b_bits: Any,
    c_bits: Any,
    fpscr: Any,
    msr: Any,
    ops: Any,
    scalar_outcome_from_fused: Callable[..., ScalarFPOutcome],
) -> ExactDispatchResult | None:
    from .fp_capabilities import scalar_fp_exact_v2_enabled

    if not scalar_fp_exact_v2_enabled():
        return None

    name = str(opcode)
    a_i = try_concrete_bv64(a_bits)
    b_i = try_concrete_bv64(b_bits)
    c_i = try_concrete_bv64(c_bits)
    fpscr_i = try_concrete_bv32(fpscr)
    symbolic = not (
        a_i is not None
        and b_i is not None
        and c_i is not None
        and fpscr_i is not None
    )

    if name in EXACT_SCALAR_ARITH_OPS:
        if not symbolic:
            outcome = exact_scalar_arith(name, a_i, b_i, c_bits=c_i, fpscr=fpscr_i)  # type: ignore[arg-type]
            if outcome is None or not outcome.supported:
                return None
            result_bits = _bv64(ops, outcome.result_bits)
        else:
            result_bits = _exact_arith_result_bits_bv(name, a_bits, b_bits, c_bits, fpscr, ops)
            rep = exact_scalar_arith(name, 0x3FF0000000000000, 0x3FF0000000000000, fpscr=0)
            if rep is None:
                return None
            outcome = ScalarFPOutcome(
                result_bits=0,
                raised_causes=rep.raised_causes,
                fi=rep.fi,
                fr=rep.fr,
                fi_fr_policy=rep.fi_fr_policy,
                fprf=rep.fprf,
                update_fprf=rep.update_fprf,
                writeback=rep.writeback,
                supported=True,
            )
        return ExactDispatchResult(
            outcome=outcome,
            result_bits=result_bits,
            writeback=_bool(ops, bool(outcome.writeback)),
            post_fpscr=apply_fpscr_transition_expr(fpscr, name, outcome, ops),
            symbolic=symbolic,
        )

    if name in {
        "fmadd", "fmadds", "fmsub", "fmsubs",
        "fnmadd", "fnmadds", "fnmsub", "fnmsubs",
    }:
        return _dispatch_exact_fused(
            name, a_bits, b_bits, c_bits, fpscr, msr, ops,
            scalar_outcome_from_fused=scalar_outcome_from_fused,
        )

    if name == "fsel":
        if a_i is None or b_i is None or c_i is None:
            return None
        from .fp_exact_estimate import exact_fsel

        outcome = exact_fsel(a_i, c_i, b_i)
        if not outcome.supported:
            return None
        return ExactDispatchResult(
            outcome=outcome,
            result_bits=_bv64(ops, outcome.result_bits),
            writeback=_bool(ops, bool(outcome.writeback)),
            post_fpscr=apply_fpscr_transition_expr(fpscr, name, outcome, ops),
            symbolic=False,
        )

    return None


def scalar_fp_unsupported_predicate(*, supported: Any) -> Any:
    import z3

    if isinstance(supported, bool):
        return z3.BoolVal(not supported)
    return z3.Not(supported)


def _evaluate_bv64(model: Any, expr: Any) -> int:
    return int(model.eval(expr, model_completion=True).as_long()) & _MASK64


def evaluate_backend_add(backend: IntBackend | Z3BitVecBackend, a: int, b: int) -> int | Any:
    return backend.add(backend.const(a), backend.const(b))


def evaluate_backend_mul(backend: IntBackend | Z3BitVecBackend, a: int, b: int) -> int | Any:
    return backend.mul(backend.const(a), backend.const(b))


def evaluate_z3_concrete(backend: Z3BitVecBackend, expr: Any) -> int:
    solver = backend.z3.Solver()
    result = backend.z3.BitVec("result", backend.width)
    solver.add(result == expr)
    if solver.check() != backend.z3.sat:
        raise RuntimeError("failed to evaluate concrete Z3 bitvector expression")
    return _evaluate_bv64(solver.model(), result)


def prove_backend_agrees(op: str, a: int, b: int, *, width: int = 64) -> bool:
    int_backend = IntBackend()
    z3_backend = Z3BitVecBackend(width)
    if op == "add":
        int_result = int_backend.add(int_backend.const(a), int_backend.const(b))
        z3_result = evaluate_z3_concrete(z3_backend, evaluate_backend_add(z3_backend, a, b))
        return (int_result & ((1 << width) - 1)) == z3_result
    if op == "mul":
        int_result = int_backend.mul(int_backend.const(a), int_backend.const(b))
        mask = (1 << width) - 1
        z3_result = evaluate_z3_concrete(z3_backend, evaluate_backend_mul(z3_backend, a, b))
        return (int_result & mask) == (z3_result & mask)
    raise ValueError(f"unsupported backend op {op!r}")


def prove_exact_arithmetic_corpus(
    pairs: Sequence[tuple[int, int]],
    *,
    op: str = "fadd",
) -> list[tuple[int, int, bool]]:
    set_scalar_fp_exact_v2_module_flag(True)
    results: list[tuple[int, int, bool]] = []
    try:
        for a, b in pairs:
            if op == "fadd":
                outcome = exact_fadd(a, b, fpscr=0)
                again = exact_fadd(a, b, fpscr=0)
            elif op == "fmul":
                outcome = exact_fmul(a, b, fpscr=0)
                again = exact_fmul(a, b, fpscr=0)
            else:
                raise ValueError(op)
            ok = outcome.supported and again.supported and outcome.result_bits == again.result_bits
            results.append((a, b, ok))
    finally:
        set_scalar_fp_exact_v2_module_flag(None)
    return results


def scalar_fp_unsupported_query(
    feasible_path: Sequence[Any],
    unsupported_pred: Any | None,
    *,
    algorithm: str = "scalar-fp-unsupported-v1",
    timeout_ms: int | None = None,
    z3_module: Any | None = None,
) -> ScalarFPUnsupportedQuery:
    import z3 as z3_default

    z3 = z3_module or z3_default
    premises = list(feasible_path)
    if unsupported_pred is None:
        return ScalarFPUnsupportedQuery(status="vacuous", query_sha256=_query_sha256(algorithm, premises, ()))

    solver = z3.Solver()
    if timeout_ms is not None:
        solver.set("timeout", int(timeout_ms))
    for premise in premises:
        solver.add(premise)
    solver.add(unsupported_pred)

    start = time.perf_counter()
    answer = solver.check()
    elapsed_ms = (time.perf_counter() - start) * 1000.0
    mapped = _map_z3_answer(answer, z3)
    status: ScalarFPQueryStatus
    if mapped == "unknown" and timeout_ms is not None and elapsed_ms >= timeout_ms:
        status = "timeout"
    else:
        status = mapped  # type: ignore[assignment]
    return ScalarFPUnsupportedQuery(
        status=status,
        query_sha256=_query_sha256(algorithm, premises, [unsupported_pred]),
        elapsed_ms=elapsed_ms,
        algorithm=algorithm,
    )


def scalar_fp_unsupported_query_sha256(
    feasible_path: Sequence[Any],
    unsupported_pred: Any,
) -> str:
    return hashlib.sha256(
        (_query_sha256("scalar-fp-unsupported-v1", feasible_path, (unsupported_pred,))).encode(),
    ).hexdigest()


__all__ = [
    "ExactDispatchResult",
    "ScalarFPQueryStatus",
    "ScalarFPUnsupportedQuery",
    "evaluate_backend_add",
    "evaluate_backend_mul",
    "evaluate_z3_concrete",
    "prove_backend_agrees",
    "prove_exact_arithmetic_corpus",
    "scalar_fp_unsupported_predicate",
    "scalar_fp_unsupported_query",
    "scalar_fp_unsupported_query_sha256",
    "try_concrete_bv64",
    "try_concrete_bv32",
    "try_dispatch_exact_scalar_v2",
    "apply_fpscr_transition_expr",
]
