# P0-03 — Enforce a real end-to-end proof deadline

**Priority:** P0  
**Primary owner area:** `tools/ppc_equivalence/engine.py`  
**Dependencies:** none

---

## Problem statement

The default solver receives `timeout_ms`, but the fallback `simplify -> bit-blast -> sat` tactic path is separately constructed without a corresponding timeout. The tactic path is also attempted even when the default solver has already returned a definitive result.

Consequences:

- `timeout_ms` is not a reliable wall-clock limit;
- proofs can perform duplicate work after a conclusive answer;
- automation can hang or consume excessive resources;
- result timing does not express where the budget was spent.

## Required semantic rule

`timeout_ms` must mean:

> Maximum elapsed wall-clock budget for the solver portfolio and layout-feasibility check. Counterexample extraction (Z3 model evaluation on a solved model) and query construction are intentionally excluded — they are effectively instant and guarding them would add complexity with no real benefit.

If query construction is intentionally excluded, rename the option to `solver_timeout_ms` and add a separate `proof_deadline_ms`. The preferred API is a single end-to-end deadline plus optional internal portfolio shares.

## Deadline utility

```python
from dataclasses import dataclass
import time


@dataclass(frozen=True)
class Deadline:
    end_ns: int

    @classmethod
    def after_ms(cls, timeout_ms: int) -> "Deadline":
        if timeout_ms <= 0:
            raise ValueError("timeout_ms must be positive")
        return cls(time.monotonic_ns() + timeout_ms * 1_000_000)

    def remaining_ms(self) -> int:
        remaining_ns = self.end_ns - time.monotonic_ns()
        if remaining_ns <= 0:
            return 0
        return max(1, (remaining_ns + 999_999) // 1_000_000)

    def require_time(self, phase: str) -> int:
        remaining = self.remaining_ms()
        if remaining <= 0:
            raise ProofDeadlineExceeded(phase)
        return remaining
```

## Solver portfolio behavior

Preferred initial behavior:

1. Run the default solver first.
2. Return immediately on `sat` or `unsat`.
3. Run fallback tactics only when the default solver returns `unknown`, and only with remaining budget.
4. Record each phase's result and elapsed time.

```python
def check_with_portfolio(
    z3_module: Any, build_solver: Callable[[], z3.Solver],
    deadline: Deadline,
) -> tuple[Any, Any, str, list[SolverPhase]]:
    phases: list[SolverPhase] = []

    default_solver = build_solver()
    default_solver.set(timeout=deadline.require_time("default-solver"))
    started = time.monotonic_ns()
    default_answer = default_solver.check()
    default_elapsed = round((time.monotonic_ns() - started) / 1_000_000, 3)
    phases.append(SolverPhase("default", str(default_answer), default_elapsed))

    if default_answer in (z3_module.sat, z3_module.unsat):
        return default_solver, default_answer, "default", phases

    try:
        remaining = deadline.require_time("bitblast-fallback")
        fallback_solver = build_solver()
        t = z3_module.Then("simplify", "bit-blast", "sat")
        fallback_solver.set(timeout=remaining)
        bit_solver = t(fallback_solver)
        t_start = time.monotonic_ns()
        bit_answer = bit_solver.check()
        bit_elapsed = round((time.monotonic_ns() - t_start) / 1_000_000, 3)
        phases.append(SolverPhase("bitblast", str(bit_answer), bit_elapsed))
        if bit_answer in (z3_module.sat, z3_module.unsat):
            return bit_solver, bit_answer, "bitblast", phases
    except ProofDeadlineExceeded:
        phases.append(SolverPhase("bitblast", "deadline-exceeded", 0))
        return default_solver, z3_module.unknown, "default", phases
    except Exception:
        phases.append(SolverPhase("bitblast", "error", 0))

    return default_solver, default_answer, "default", phases
```

Do not assume every tactic solver honors timeout perfectly. For a strict hard wall-clock guarantee, run the proof worker in a subprocess and terminate it when the parent deadline expires.

## Hard-limit subprocess design (future enhancement)

Not implemented in this P0. The current approach relies on Z3's internal timeout (`solver.set(timeout=...)`), which is honored by all tactic solvers used. For a strict hard wall-clock guarantee in automation, a future subprocess architecture should follow:

```text
parent process
  -> serializes proof request to worker
  -> starts worker subprocess in its own process group
  -> waits until deadline
  -> receives structured result, or terminates process group
  -> returns INCONCLUSIVE_TIMEOUT with phase diagnostics
```

Requirements:

- no `fork` reuse of an active Z3 context unless tested safe;
- bounded stdout/stderr;
- temporary files cleaned after forced termination;
- platform-specific termination handled explicitly;
- proof cache written only after a complete, validated result;
- no partial certificate emitted after timeout.

## Resource metadata

Phases are recorded inside the `solver` dict alongside other solver metadata:

```json
{
  "solver": {
    "name": "z3",
    "version": "4.13.0",
    "result": "unsat",
    "elapsed_ms": 8724,
    "timeout_ms": 120000,
    "tactic": "default",
    "original_paths": 1,
    "candidate_paths": 1,
    "phases": [
      {
        "name": "default-solver",
        "elapsed_ms": 8720,
        "result": "unsat"
      }
    ]
  }
}
```

`layout-feasibility` is not recorded as a phase — it runs with the same remaining deadline but its result is used only to gate `ProofStatus` and is not part of solver diagnostics.

Peak RSS may be populated by the worker/parent in the future on supported platforms. Do not make cross-platform availability mandatory for proof validity.

## Tests

### Implemented (test_deadline.py)

| Test | Mechanism |
|------|-----------|
| Definitive default `unsat` does not invoke fallback | `_prove("4e800020", "4e800020")` → 1 phase in result |
| Definitive default `sat` does not invoke fallback | `_prove("38630004…", "38630005…")` → 1 phase |
| Default `unknown` invokes fallback with budget ≤ original | Mock `Solver.check` → `unknown` then `unsat`; verify 2 phases and fallback time < total budget |
| Expired deadline before fallback returns `unknown` with `deadline-exceeded` phase | Mock `Deadline.require_time` raises on second call; verify phase and answer |
| `timeout_ms=0` rejected | `Deadline.after_ms(0)` / `(-1)` → `ValueError` |
| Phase dict structure | `SolverPhase.to_dict()` contains `name`, `result`, `elapsed_ms` |

### Not tested / future

| Gap | Reason |
|-----|--------|
| Layout solver uses the same deadline | Covered by code path (hard to force deterministically); deferred to soundness regression suite (P1-01) |
| Counterexample extraction cannot exceed deadline | Model eval is effectively instant; not worth a test |
| Cache/certificate writes after timeout | No cache layer exists yet; deferred to proof-provenance (P1-08) |
| Subprocess hard timeout | Architecture deferred beyond this P0 |

## Acceptance criteria

- No proof phase can receive the original full timeout after earlier phases consumed time.
- The fallback is not run after a conclusive default result.
- Timeout results remain inconclusive and never generate acceptance certificates.
- Phase timing appears in the solver result dict for diagnostics.
- `Deadline.after_ms` rejects non-positive timeout values.
- An expired `Deadline.require_time` raises `ProofDeadlineExceeded` carrying the phase name.
