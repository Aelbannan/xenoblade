"""Independent UNSAT discharge queries for proof obligations.

These solvers intentionally exclude the main equivalence mismatch formula.
A bad behavior is only ruled out when a dedicated query returns UNSAT.
"""

from __future__ import annotations

import hashlib
import time
from dataclasses import dataclass
from typing import Any, Literal, Sequence

from tools.ppc_equivalence.deadline import Deadline, ProofDeadlineExceeded, SolverPhase


@dataclass(frozen=True)
class UnsatDischarge:
    status: Literal["unsat", "sat", "unknown", "timeout"]
    algorithm: str
    query_sha256: str
    elapsed_ms: float
    solver_phases: tuple[dict[str, Any], ...]

    def to_obligation_digest(self) -> dict[str, Any]:
        return {
            "result": self.status,
            "query_sha256": self.query_sha256,
            "algorithm": self.algorithm,
            "elapsed_ms": self.elapsed_ms,
            "solver_phases": list(self.solver_phases),
        }


def _query_sha256(
    algorithm: str,
    premises: Sequence[Any],
    bad_conditions: Sequence[Any],
) -> str:
    digest = hashlib.sha256()
    digest.update(algorithm.encode("utf-8"))
    digest.update(b"\0")
    for premise in premises:
        digest.update(str(premise).encode("utf-8"))
        digest.update(b"\0")
    digest.update(b"|")
    for condition in bad_conditions:
        digest.update(str(condition).encode("utf-8"))
        digest.update(b"\0")
    return digest.hexdigest()


def _map_z3_answer(answer: Any, z3_module: Any) -> Literal["unsat", "sat", "unknown"]:
    if answer == z3_module.unsat:
        return "unsat"
    if answer == z3_module.sat:
        return "sat"
    return "unknown"


def discharge_bad_conditions(
    *,
    premises: Sequence[Any],
    bad_conditions: Sequence[Any],
    deadline: Deadline,
    algorithm: str,
    z3_module: Any | None = None,
) -> UnsatDischarge:
    """Prove that OR(bad_conditions) is unreachable under ``premises``.

    Does not include any equivalence mismatch formula. Empty ``bad_conditions``
    is vacuously discharged (``unsat``).
    """
    query_digest = _query_sha256(algorithm, premises, bad_conditions)
    if not bad_conditions:
        return UnsatDischarge(
            status="unsat",
            algorithm=algorithm,
            query_sha256=query_digest,
            elapsed_ms=0.0,
            solver_phases=({"name": "vacuous", "result": "unsat", "elapsed_ms": 0.0},),
        )

    if z3_module is None:
        import z3 as z3_module  # type: ignore[no-redef]

    phases: list[SolverPhase] = []
    started = time.monotonic_ns()

    def _build_solver() -> Any:
        solver = z3_module.Solver()
        for premise in premises:
            solver.add(premise)
        if len(bad_conditions) == 1:
            solver.add(bad_conditions[0])
        else:
            solver.add(z3_module.Or(*list(bad_conditions)))
        return solver

    try:
        remaining = deadline.require_time(f"discharge:{algorithm}")
    except ProofDeadlineExceeded:
        return UnsatDischarge(
            status="timeout",
            algorithm=algorithm,
            query_sha256=query_digest,
            elapsed_ms=0.0,
            solver_phases=(
                {"name": "default", "result": "deadline-exceeded", "elapsed_ms": 0.0},
            ),
        )

    try:
        solver = _build_solver()
        solver.set(timeout=remaining)
        phase_started = time.monotonic_ns()
        answer = solver.check()
        phase_elapsed = round((time.monotonic_ns() - phase_started) / 1_000_000, 3)
        phases.append(SolverPhase("default", str(answer), phase_elapsed))
    except ProofDeadlineExceeded:
        return UnsatDischarge(
            status="timeout",
            algorithm=algorithm,
            query_sha256=query_digest,
            elapsed_ms=round((time.monotonic_ns() - started) / 1_000_000, 3),
            solver_phases=(
                {"name": "default", "result": "deadline-exceeded", "elapsed_ms": 0.0},
            ),
        )

    status = _map_z3_answer(answer, z3_module)
    if status == "unknown":
        try:
            remaining = deadline.require_time(f"discharge-bitblast:{algorithm}")
            fallback = _build_solver()
            tactic = z3_module.Then("simplify", "bit-blast", "sat")
            bit_solver = tactic.solver()
            bit_solver.set(timeout=remaining)
            for assertion in fallback.assertions():
                bit_solver.add(assertion)
            phase_started = time.monotonic_ns()
            bit_answer = bit_solver.check()
            phase_elapsed = round((time.monotonic_ns() - phase_started) / 1_000_000, 3)
            phases.append(SolverPhase("bitblast", str(bit_answer), phase_elapsed))
            status = _map_z3_answer(bit_answer, z3_module)
        except ProofDeadlineExceeded:
            phases.append(SolverPhase("bitblast", "deadline-exceeded", 0.0))
            status = "timeout"
        except Exception:
            phases.append(SolverPhase("bitblast", "error", 0.0))

    if status == "unknown" and any(
        "timeout" in phase.result.lower() for phase in phases
    ):
        status = "timeout"

    return UnsatDischarge(
        status=status,
        algorithm=algorithm,
        query_sha256=query_digest,
        elapsed_ms=round((time.monotonic_ns() - started) / 1_000_000, 3),
        solver_phases=tuple(phase.to_dict() for phase in phases),
    )
