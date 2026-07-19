"""First-divergence diagnostics and solver-query audit helpers.

This module reproduces a SAT witness concretely with ``ConcreteOps`` and
reports the earliest instruction step where the two implementations' observed
architectural state first differ.  It is used by P2-03 to populate the
``first_divergence`` field of a counterexample bundle, and by P2-04 for
lightweight symbol / query introspection.
"""

from __future__ import annotations

import copy
from typing import Any

from .contract import Observable
from .model import MachineState, concrete_state
from .semantics import ConcreteOps, execute_instruction


def _snapshot(state: MachineState) -> dict[str, Any]:
    return {
        "gpr": {f"r{i}": int(state.gpr[i]) & 0xFFFFFFFF for i in range(32)},
        "fpr": {
            f"f{i}": int(state.fpr[i]) & 0xFFFFFFFFFFFFFFFF for i in range(32)
        },
        "cr": int(state.cr) & 0xFFFFFFFF,
        "xer": {
            "ca": int(bool(state.xer.ca)),
            "ov": int(bool(state.xer.ov)),
            "so": int(bool(state.xer.so)),
        },
        "lr": int(state.lr) & 0xFFFFFFFF,
        "ctr": int(state.ctr) & 0xFFFFFFFF,
        "memory": {
            f"0x{addr:08x}": byte & 0xFF
            for addr, byte in sorted(state.memory.bytes)
        },
    }


def _compact_step(step: int, insn: Any, state: MachineState) -> dict[str, Any]:
    snap = _snapshot(state)
    return {
        "step": step,
        "opcode": getattr(getattr(insn, "opcode", None), "value", str(insn)),
        "operands": list(getattr(insn, "operands", ())),
        "gpr": snap["gpr"],
        "cr": f"0x{snap['cr']:08x}",
        "xer": snap["xer"],
        "lr": f"0x{snap['lr']:08x}",
        "ctr": f"0x{snap['ctr']:08x}",
        "memory": snap["memory"],
    }


def _states_first_divergence(
    original: MachineState, candidate: MachineState
) -> dict[str, Any] | None:
    """Return the first architectural component that differs between two states."""
    for index in range(32):
        o = int(original.gpr[index]) & 0xFFFFFFFF
        c = int(candidate.gpr[index]) & 0xFFFFFFFF
        if o != c:
            return {
                "component": f"r{index}",
                "original": f"0x{o:08x}",
                "candidate": f"0x{c:08x}",
            }
    for index in range(32):
        o = int(original.fpr[index]) & 0xFFFFFFFFFFFFFFFF
        c = int(candidate.fpr[index]) & 0xFFFFFFFFFFFFFFFF
        if o != c:
            return {
                "component": f"f{index}",
                "original": f"0x{o:016x}",
                "candidate": f"0x{c:016x}",
            }
    o_cr = int(original.cr) & 0xFFFFFFFF
    c_cr = int(candidate.cr) & 0xFFFFFFFF
    if o_cr != c_cr:
        return {
            "component": "cr",
            "original": f"0x{o_cr:08x}",
            "candidate": f"0x{c_cr:08x}",
        }
    for flag in ("ca", "ov", "so"):
        o = int(bool(getattr(original.xer, flag)))
        c = int(bool(getattr(candidate.xer, flag)))
        if o != c:
            return {"component": f"xer.{flag}", "original": o, "candidate": c}
    o_lr = int(original.lr) & 0xFFFFFFFF
    c_lr = int(candidate.lr) & 0xFFFFFFFF
    if o_lr != c_lr:
        return {
            "component": "lr",
            "original": f"0x{o_lr:08x}",
            "candidate": f"0x{c_lr:08x}",
        }
    o_ctr = int(original.ctr) & 0xFFFFFFFF
    c_ctr = int(candidate.ctr) & 0xFFFFFFFF
    if o_ctr != c_ctr:
        return {
            "component": "ctr",
            "original": f"0x{o_ctr:08x}",
            "candidate": f"0x{c_ctr:08x}",
        }
    if sorted(original.memory.bytes) != sorted(candidate.memory.bytes):
        return {
            "component": "memory",
            "original": "different",
            "candidate": "different",
        }
    return None


def _observable_concrete_value(state: MachineState, observable: Observable) -> Any:
    kind = observable.kind
    if kind == "gpr":
        assert observable.index is not None
        return int(state.gpr[observable.index]) & 0xFFFFFFFF
    if kind == "fpr":
        assert observable.index is not None
        return int(state.fpr[observable.index]) & 0xFFFFFFFFFFFFFFFF
    if kind == "ps1":
        assert observable.index is not None
        return int(state.ps1[observable.index]) & 0xFFFFFFFFFFFFFFFF
    if kind == "gqr":
        assert observable.index is not None
        return int(state.gqr[observable.index]) & 0xFFFFFFFF
    if kind == "sr":
        assert observable.index is not None
        return int(state.sr[observable.index]) & 0xFFFFFFFF
    if kind == "spr":
        assert observable.index is not None
        return int(state.spr[observable.index]) & 0xFFFFFFFF
    if kind == "cr":
        return int(state.cr) & 0xFFFFFFFF
    if kind == "cr_field":
        assert observable.index is not None
        shift = 4 * (7 - observable.index)
        return (int(state.cr) >> shift) & 0xF
    if kind == "xer":
        # Names are xer.ca / xer.ov / xer.so (index may be None).
        flag = observable.name.split(".", 1)[-1]
        if flag in ("ca", "ov", "so"):
            return int(bool(getattr(state.xer, flag)))
        return None
    if kind == "lr":
        return int(state.lr) & 0xFFFFFFFF
    if kind == "ctr":
        return int(state.ctr) & 0xFFFFFFFF
    if kind == "msr":
        return int(state.msr) & 0xFFFFFFFF
    if kind == "fpscr":
        return int(state.fpscr) & 0xFFFFFFFF
    if kind == "time_base":
        return int(state.time_base) & 0xFFFFFFFFFFFFFFFF
    if kind == "srr0":
        return int(state.srr0) & 0xFFFFFFFF
    if kind == "srr1":
        return int(state.srr1) & 0xFFFFFFFF
    if kind == "memory":
        return tuple(sorted(state.memory.bytes))
    return None


def _contract_mismatch_reproduced(
    original: MachineState, candidate: MachineState, contract: Any
) -> bool | None:
    """Return True/False when observables can be checked, else None (inconclusive)."""
    observables = getattr(contract, "observables", ()) or ()
    if not observables:
        return _states_first_divergence(original, candidate) is not None
    saw_comparable = False
    for observable in observables:
        left = _observable_concrete_value(original, observable)
        right = _observable_concrete_value(candidate, observable)
        if left is None or right is None:
            continue
        saw_comparable = True
        if left != right:
            return True
    if not saw_comparable:
        return None
    return False


def replay_counterexample(
    original_insns: list[Any],
    candidate_insns: list[Any],
    initial_state_dict: dict[str, Any],
    contract: Any,
) -> dict[str, Any]:
    """Concretely replay both sides; return traces, divergence, and reproduce status.

    On success returns::

        {
          "reproduced": True,
          "original_trace": [...],
          "candidate_trace": [...],
          "first_divergence": {"step": int, "component": str, ...},
        }

    On failure returns ``reproduced=False`` with an ``error`` string suitable for
    ``first_divergence`` / ``INTERNAL_ERROR`` classification.
    """
    try:
        initial = concrete_state(initial_state_dict)
    except Exception as exc:
        return {
            "reproduced": False,
            "error": f"concrete-state-construction-failed: {exc}",
            "original_trace": None,
            "candidate_trace": None,
            "first_divergence": {
                "error": "symbolic-relocations-prevent-concrete-replay",
            },
        }

    ops = ConcreteOps()
    original_state = initial
    candidate_state = initial
    original_trace: list[dict[str, Any]] = []
    candidate_trace: list[dict[str, Any]] = []
    first_divergence: dict[str, Any] | None = None
    min_len = min(len(original_insns), len(candidate_insns))

    for step in range(min_len):
        try:
            original_state = execute_instruction(
                original_state, original_insns[step], ops
            )
            candidate_state = execute_instruction(
                candidate_state, candidate_insns[step], ops
            )
        except Exception as exc:
            return {
                "reproduced": False,
                "error": f"concrete-execution-failed: {exc}",
                "original_trace": original_trace or None,
                "candidate_trace": candidate_trace or None,
                "first_divergence": {
                    "error": "concrete-replay-execution-failed",
                    "step": step,
                    "detail": str(exc),
                },
            }

        original_trace.append(
            _compact_step(step, original_insns[step], original_state)
        )
        candidate_trace.append(
            _compact_step(step, candidate_insns[step], candidate_state)
        )

        if first_divergence is None:
            div = _states_first_divergence(original_state, candidate_state)
            if div is not None:
                first_divergence = {"step": step, **div}

    # Finish remaining instructions on the longer side so final states are complete.
    for step in range(min_len, len(original_insns)):
        try:
            original_state = execute_instruction(
                original_state, original_insns[step], ops
            )
        except Exception as exc:
            return {
                "reproduced": False,
                "error": f"concrete-execution-failed: {exc}",
                "original_trace": original_trace or None,
                "candidate_trace": candidate_trace or None,
                "first_divergence": {
                    "error": "concrete-replay-execution-failed",
                    "step": step,
                    "detail": str(exc),
                },
            }
        original_trace.append(
            _compact_step(step, original_insns[step], original_state)
        )
    for step in range(min_len, len(candidate_insns)):
        try:
            candidate_state = execute_instruction(
                candidate_state, candidate_insns[step], ops
            )
        except Exception as exc:
            return {
                "reproduced": False,
                "error": f"concrete-execution-failed: {exc}",
                "original_trace": original_trace or None,
                "candidate_trace": candidate_trace or None,
                "first_divergence": {
                    "error": "concrete-replay-execution-failed",
                    "step": step,
                    "detail": str(exc),
                },
            }
        candidate_trace.append(
            _compact_step(step, candidate_insns[step], candidate_state)
        )

    if first_divergence is None:
        first_divergence = {"step": -1, "component": None}

    architectural = first_divergence.get("component") is not None
    contract_repro = _contract_mismatch_reproduced(
        original_state, candidate_state, contract
    )
    # Prefer architectural divergence or a confirmed contract mismatch.
    # Only escalate when ConcreteOps fully ran and neither signal is present.
    if architectural or contract_repro is True:
        return {
            "reproduced": True,
            "original_trace": original_trace,
            "candidate_trace": candidate_trace,
            "first_divergence": first_divergence,
        }
    if contract_repro is None:
        # Observables could not be concretely evaluated (keep traces; no escalate).
        return {
            "reproduced": True,
            "original_trace": original_trace,
            "candidate_trace": candidate_trace,
            "first_divergence": first_divergence,
        }

    return {
        "reproduced": False,
        "error": "sat-witness-not-reproduced-under-concrete-ops",
        "original_trace": original_trace,
        "candidate_trace": candidate_trace,
        "first_divergence": {
            "error": "sat-witness-not-reproduced-under-concrete-ops",
            "step": first_divergence.get("step", -1),
            "component": first_divergence.get("component"),
        },
    }


def compute_first_divergence(
    original_insns: list[Any],
    candidate_insns: list[Any],
    initial_state_dict: dict[str, Any],
    contract: Any,
) -> dict[str, Any]:
    """Concretely replay both sides and find the earliest diverging step.

    Compatibility wrapper around :func:`replay_counterexample`.
    """
    result = replay_counterexample(
        original_insns, candidate_insns, initial_state_dict, contract
    )
    return result["first_divergence"]


def _model_still_reproduces(
    original_insns: list[Any],
    candidate_insns: list[Any],
    model_values: dict[str, Any],
    contract: Any,
) -> bool:
    replay = replay_counterexample(
        original_insns, candidate_insns, model_values, contract
    )
    return bool(replay.get("reproduced"))


def _gpr_is_zero(value: Any) -> bool:
    try:
        return (int(value, 0) if isinstance(value, str) else int(value)) & 0xFFFFFFFF == 0
    except (TypeError, ValueError):
        return False


def _memory_bytes_map(model_values: dict[str, Any]) -> dict[str, Any] | None:
    memory = model_values.get("memory")
    if not isinstance(memory, dict):
        return None
    if "bytes" in memory or "default" in memory:
        bytes_map = memory.get("bytes", {})
        return bytes_map if isinstance(bytes_map, dict) else None
    return memory


def minimize_counterexample_model(
    original_insns: list[Any],
    candidate_insns: list[Any],
    model_values: dict[str, Any],
    contract: Any,
) -> dict[str, Any]:
    """Shrink a reproduced ConcreteOps witness without changing the proof domain.

    Safe greedy minimization (P2-03):

    1. zero each non-zero GPR independently when the divergence still reproduces;
    2. drop each memory byte independently when the divergence still reproduces.

    Returns a deep copy.  Does not mutate ``model_values``.  If the input model
    does not reproduce under ConcreteOps, returns an unmodified deep copy.
    """
    minimized = copy.deepcopy(model_values)
    if not _model_still_reproduces(
        original_insns, candidate_insns, minimized, contract
    ):
        return minimized

    gpr = minimized.get("gpr")
    if isinstance(gpr, dict):
        for name in sorted(gpr.keys()):
            if _gpr_is_zero(gpr.get(name)):
                continue
            candidate = copy.deepcopy(minimized)
            candidate_gpr = candidate.get("gpr")
            assert isinstance(candidate_gpr, dict)
            candidate_gpr[name] = "0x00000000"
            if _model_still_reproduces(
                original_insns, candidate_insns, candidate, contract
            ):
                minimized = candidate

    bytes_map = _memory_bytes_map(minimized)
    if bytes_map:
        # Snapshot keys so removals during iteration stay deterministic.
        for address in sorted(bytes_map.keys()):
            candidate = copy.deepcopy(minimized)
            candidate_bytes = _memory_bytes_map(candidate)
            if candidate_bytes is None or address not in candidate_bytes:
                continue
            del candidate_bytes[address]
            if _model_still_reproduces(
                original_insns, candidate_insns, candidate, contract
            ):
                minimized = candidate

    return minimized


def count_z3_symbols(expr_or_assertions: Any) -> dict[str, int]:
    """Count distinct BV / array / uninterpreted-function symbols in Z3 exprs."""
    try:
        import z3
    except Exception:
        return {"bv_symbols": 0, "array_symbols": 0, "func_symbols": 0}

    if not isinstance(expr_or_assertions, (list, tuple)):
        assertions = [expr_or_assertions]
    else:
        assertions = list(expr_or_assertions)

    bv: set[str] = set()
    arrays: set[str] = set()
    funcs: set[str] = set()
    seen: set[int] = set()

    def walk(expr: Any) -> None:
        try:
            key = expr.get_id()
        except Exception:
            key = id(expr)
        if key in seen:
            return
        seen.add(key)
        try:
            if z3.is_const(expr) and expr.decl().kind() == z3.Z3_OP_UNINTERPRETED:
                sort = expr.sort()
                name = expr.decl().name()
                if z3.is_bv_sort(sort):
                    bv.add(name)
                elif sort.kind() == z3.Z3_ARRAY_SORT:
                    arrays.add(name)
                else:
                    funcs.add(name)
            elif (
                z3.is_app(expr)
                and expr.decl().kind() == z3.Z3_OP_UNINTERPRETED
                and expr.num_args() > 0
            ):
                funcs.add(expr.decl().name())
        except Exception:
            pass
        try:
            for child in expr.children():
                walk(child)
        except Exception:
            pass

    for assertion in assertions:
        walk(assertion)

    return {
        "bv_symbols": len(bv),
        "array_symbols": len(arrays),
        "func_symbols": len(funcs),
    }


def categorize_assertions(
    *,
    layout_constraint_count: int,
    memory_constraint_count: int,
    relocation_binding_count: int,
    path_pair_count: int,
) -> dict[str, int]:
    """Normalize assertion counts into reviewer-facing categories."""
    categories = {
        "layout": max(0, layout_constraint_count - memory_constraint_count),
        "memory": max(0, memory_constraint_count),
        "relocation_bindings": max(0, relocation_binding_count),
        # Path-pair differences are OR'd into a single top-level assertion.
        "inequivalence_disjunction": 1 if path_pair_count > 0 else 0,
        "path_pairs_covered": max(0, path_pair_count),
    }
    return categories


def differential_query_check(
    original_insns: list[Any],
    candidate_insns: list[Any],
    *,
    observe_gpr: int = 3,
    bit_width: int = 4,
) -> dict[str, Any]:
    """Exhaustive ConcreteOps cross-check for tiny single-instruction cases.

    Enumerates a reduced input domain for ``r{observe_gpr}`` and reports whether
    the two encodings always agree on that register.  Intended as a construction
    sanity check for small generated cases, not a replacement for the SMT query.
    """
    if len(original_insns) != 1 or len(candidate_insns) != 1:
        return {
            "applicable": False,
            "reason": "only-single-instruction-blocks-supported",
        }
    if bit_width < 1 or bit_width > 8:
        return {"applicable": False, "reason": "bit_width-out-of-range"}

    ops = ConcreteOps()
    domain = 1 << bit_width
    disagreements: list[dict[str, Any]] = []
    for value in range(domain):
        initial = concrete_state({"gpr": {f"r{observe_gpr}": value}})
        try:
            left = execute_instruction(initial, original_insns[0], ops)
            right = execute_instruction(initial, candidate_insns[0], ops)
        except Exception as exc:
            return {
                "applicable": True,
                "agreed": False,
                "error": str(exc),
                "domain_size": domain,
            }
        left_v = int(left.gpr[observe_gpr]) & 0xFFFFFFFF
        right_v = int(right.gpr[observe_gpr]) & 0xFFFFFFFF
        if left_v != right_v:
            disagreements.append(
                {
                    "input": f"0x{value:x}",
                    "original": f"0x{left_v:08x}",
                    "candidate": f"0x{right_v:08x}",
                }
            )
    return {
        "applicable": True,
        "agreed": len(disagreements) == 0,
        "domain_size": domain,
        "disagreement_count": len(disagreements),
        "sample_disagreements": disagreements[:8],
    }
