"""Process-isolated Z3 entry points for thread-safe parallel proofs.

Z3 contexts are not safe to share across threads. The llm-harness (and any
other ``ThreadPoolExecutor`` caller) can race the default global context and
segfault inside ``libz3``. This module runs the Z3-heavy entry points in a
``spawn`` process pool so each worker has its own interpreter + Z3 state.

Configuration (environment):

- ``PPC_EQUIV_PROCESS_POOL`` — ``auto`` (default), ``always`` / ``1``, or
  ``off`` / ``0``.  ``auto`` isolates when the caller is not the main thread.
- ``PPC_EQUIV_PROCESS_POOL_WORKERS`` — pool size (default ``min(cpu, 8)``).

Known limitations:

- Spawn cold-start is amortized by a persistent pool, but first use is slower.
- Each worker holds its own Z3 RSS; large ``workers`` can OOM.
- Pickling large instruction lists / jump-table contexts has a cost.
- A stuck worker past the soft wait timeout forces a full pool restart
  (ProcessPoolExecutor cannot kill a single worker).
- Hard wall-clock kill of an in-flight Z3 ``check()`` is best-effort; prefer
  contract ``timeout_ms`` + engine ``Deadline``.
"""

from __future__ import annotations

import atexit
import multiprocessing
import os
import threading
from concurrent.futures import ProcessPoolExecutor, TimeoutError as FuturesTimeoutError
from typing import Any, Optional

from tools.ppc_equivalence.contract import EquivalenceContract
from tools.ppc_equivalence.ir import Instruction
from tools.ppc_equivalence.result import ProofResult, ProofStatus
from tools.ppc_equivalence.semantics import (
    DEFAULT_MAX_LOOP_ITERATIONS,
    CalleeContract,
)

_IN_WORKER = False
_POOL_LOCK = threading.Lock()
_POOL: ProcessPoolExecutor | None = None
_POOL_WORKERS: int | None = None
_MP_CONTEXT = multiprocessing.get_context("spawn")


def _env_mode() -> str:
    raw = os.environ.get("PPC_EQUIV_PROCESS_POOL", "auto").strip().lower()
    if raw in ("0", "false", "off", "no", "never"):
        return "off"
    if raw in ("1", "true", "on", "yes", "always"):
        return "always"
    return "auto"


def _worker_count() -> int:
    raw = os.environ.get("PPC_EQUIV_PROCESS_POOL_WORKERS", "").strip()
    if raw:
        return max(1, int(raw))
    cpus = os.cpu_count() or 4
    return max(1, min(cpus, 8))


def should_isolate() -> bool:
    """Return True when this call should run in a worker process."""
    if _IN_WORKER:
        return False
    mode = _env_mode()
    if mode == "off":
        return False
    if mode == "always":
        return True
    return threading.current_thread() is not threading.main_thread()


def _pool_initializer() -> None:
    global _IN_WORKER
    _IN_WORKER = True


def _get_pool() -> ProcessPoolExecutor:
    global _POOL, _POOL_WORKERS
    with _POOL_LOCK:
        workers = _worker_count()
        if _POOL is not None and _POOL_WORKERS == workers:
            return _POOL
        if _POOL is not None:
            _POOL.shutdown(wait=False, cancel_futures=True)
        _POOL = ProcessPoolExecutor(
            max_workers=workers,
            mp_context=_MP_CONTEXT,
            initializer=_pool_initializer,
        )
        _POOL_WORKERS = workers
        return _POOL


def shutdown_pool(*, wait: bool = True) -> None:
    """Tear down the shared pool (tests / process exit)."""
    global _POOL, _POOL_WORKERS
    with _POOL_LOCK:
        if _POOL is None:
            return
        _POOL.shutdown(wait=wait, cancel_futures=not wait)
        _POOL = None
        _POOL_WORKERS = None


def _restart_pool() -> None:
    shutdown_pool(wait=False)
    _get_pool()


atexit.register(lambda: shutdown_pool(wait=False))


def _worker_check_equivalence(payload: dict[str, Any]) -> dict[str, Any]:
    from tools.ppc_equivalence.engine import _check_equivalence_impl

    callees_used: set[int | str] = set()
    kwargs = dict(payload["kwargs"])
    kwargs["assumed_callees_used"] = callees_used
    result = _check_equivalence_impl(
        payload["original"],
        payload["candidate"],
        payload["contract"],
        original_hex=payload["original_hex"],
        candidate_hex=payload["candidate_hex"],
        **kwargs,
    )
    return {
        "result": result,
        "assumed_callees_used": callees_used,
    }


def _worker_validate_callee_contract(payload: dict[str, Any]) -> Any:
    from tools.ppc_equivalence.engine import _validate_callee_contract_impl

    return _validate_callee_contract_impl(
        payload["instructions"],
        payload["contract"],
        **payload["kwargs"],
    )


def _submit(fn: Any, payload: dict[str, Any], *, wait_s: float) -> Any:
    pool = _get_pool()
    future = pool.submit(fn, payload)
    try:
        return future.result(timeout=wait_s)
    except FuturesTimeoutError:
        _restart_pool()
        raise
    except Exception:
        # BrokenProcessPool / pickling errors: recycle workers for the next call.
        try:
            _restart_pool()
        except Exception:
            shutdown_pool(wait=False)
        raise


def run_check_equivalence(
    original: list[Instruction],
    candidate: list[Instruction],
    contract: EquivalenceContract,
    *,
    original_hex: str,
    candidate_hex: str,
    assumed_callees_used: set[int | str] | None = None,
    **kwargs: Any,
) -> ProofResult:
    """Run ``_check_equivalence_impl`` in a worker; merge callee side effects."""
    timeout_ms = int(getattr(contract, "timeout_ms", 10_000) or 10_000)
    wait_s = max(5.0, timeout_ms / 1000.0 + 15.0)
    payload = {
        "original": original,
        "candidate": candidate,
        "contract": contract,
        "original_hex": original_hex,
        "candidate_hex": candidate_hex,
        "kwargs": kwargs,
    }
    try:
        reply = _submit(_worker_check_equivalence, payload, wait_s=wait_s)
    except FuturesTimeoutError:
        return ProofResult(
            status=ProofStatus.INCONCLUSIVE_TIMEOUT,
            contract=contract.name,
            contract_resolution=contract.resolution_dict(),
            observables=[item.name for item in contract.observables],
            warnings=["process-pool wait exceeded; worker pool restarted"],
            limits={
                "max_instructions": int(kwargs.get("max_instructions", 2048)),
                "max_paths": int(kwargs.get("max_paths", 256)),
                "max_loop_iterations": int(
                    kwargs.get("max_loop_iterations", DEFAULT_MAX_LOOP_ITERATIONS)
                ),
            },
            source_hash=str(kwargs.get("source_hash", "") or ""),
            solver={
                "name": "z3",
                "result": "unknown",
                "elapsed_ms": 0,
                "timeout_ms": timeout_ms,
                "tactic": "process-pool-wait",
                "phases": [],
            },
        )
    except Exception as exc:
        return ProofResult(
            status=ProofStatus.INTERNAL_ERROR,
            contract=contract.name,
            contract_resolution=contract.resolution_dict(),
            observables=[item.name for item in contract.observables],
            unsupported=[f"process-pool failure: {exc}"],
            source_hash=str(kwargs.get("source_hash", "") or ""),
        )

    result: ProofResult = reply["result"]
    used = reply.get("assumed_callees_used") or set()
    if assumed_callees_used is not None:
        assumed_callees_used.update(used)
    return result


def run_validate_callee_contract(
    instructions: list[Instruction],
    contract: CalleeContract,
    **kwargs: Any,
) -> Any:
    """Run ``_validate_callee_contract_impl`` in a worker process."""
    # Validation is usually cheaper than full proofs; still bound the wait.
    wait_s = float(os.environ.get("PPC_EQUIV_PROCESS_POOL_VALIDATE_WAIT_S", "120"))
    payload = {
        "instructions": instructions,
        "contract": contract,
        "kwargs": kwargs,
    }
    return _submit(_worker_validate_callee_contract, payload, wait_s=wait_s)


def configure_for_tests(
    *,
    enabled: Optional[bool] = None,
    workers: Optional[int] = None,
) -> None:
    """Test helper: adjust env + reset pool. Pass ``enabled=False`` to force off."""
    if enabled is False:
        os.environ["PPC_EQUIV_PROCESS_POOL"] = "off"
    elif enabled is True:
        os.environ["PPC_EQUIV_PROCESS_POOL"] = "always"
    if workers is not None:
        os.environ["PPC_EQUIV_PROCESS_POOL_WORKERS"] = str(workers)
    shutdown_pool(wait=True)
