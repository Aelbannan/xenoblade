"""Cross-TU dependent-sweep module for the LLM decompilation harness.

Design reference: docs/llm_decomp_design.md §6 'Cross-TU (shared-header) path'
and §14.4 tiered policy.

Tiered policy summary
---------------------
By include-graph dependent count of the edited header:

  * 0        → TU_ONLY (no dependents beyond the owning TU)
  * 1–N      → FULL_SWEEP (every dependent built + every symbol diffed vs retail)
  * N+1–M    → ACCEPTED_ONLY (build all dependents; diff only protected symbols)
  * >M       → REFUSE (edit refused outright)

  N = cross_tu_full_sweep_max (default 10)
  M = cross_tu_refuse_over    (default 40)
"""
from __future__ import annotations

import enum
from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path

from tools.llm_decomp import verify
from tools.llm_decomp.config import Config

__all__ = [
    "Tier",
    "tier_for",
    "dependent_units",
    "sweep_dependents",
    "has_hard_failure",
]


class Tier(enum.Enum):
    """Tier assigned to a shared-header edit based on dependent count.

    Values are ordered so comparison operators work::

        tier_for(count, cfg) >= Tier.REFUSE  # → refuse the edit
    """

    TU_ONLY = 0  # header has no dependents beyond the owning TU
    FULL_SWEEP = 1  # <= cross_tu_full_sweep_max dependents
    ACCEPTED_ONLY = 2  # <= cross_tu_refuse_over dependents
    REFUSE = 3  # > cross_tu_refuse_over dependents


def tier_for(dependent_count: int, cfg: Config) -> Tier:
    """Map a dependent count to the appropriate :class:`Tier`.

    Parameters
    ----------
    dependent_count:
        Number of TUs that include the edited header (from the include graph).
    cfg:
        Harness configuration providing ``cross_tu_full_sweep_max`` and
        ``cross_tu_refuse_over`` thresholds.

    Returns
    -------
    Tier
        The applicable tier.
    """
    if dependent_count == 0:
        return Tier.TU_ONLY
    if dependent_count <= cfg.cross_tu_full_sweep_max:
        return Tier.FULL_SWEEP
    if dependent_count <= cfg.cross_tu_refuse_over:
        return Tier.ACCEPTED_ONLY
    return Tier.REFUSE


def dependent_units(dependents: set[str]) -> list[str]:
    """Convert repo-relative source paths to objdiff unit hints.

    Transformations
    ---------------
    * ``src/kyoshin/cf/X.cpp`` → ``kyoshin/cf/X``
    * ``libs/monolib/src/scn/Y.cpp`` → ``monolib/src/scn/Y``
    * Any other prefix is left as-is (just strips the suffix).

    Deduplicates and sorts the result alphabetically.

    Parameters
    ----------
    dependents:
        Set of repo-relative source paths (e.g. ``{"src/kyoshin/cf/X.cpp"}``).

    Returns
    -------
    list[str]
        Sorted, deduplicated unit hints.
    """
    result: set[str] = set()
    for d in dependents:
        # Strip well-known prefix
        if d.startswith("src/"):
            unit = d[4:]
        elif d.startswith("libs/"):
            unit = d[5:]
        else:
            unit = d
        # Strip file extension (last .ext)
        idx = unit.rfind(".")
        if idx > 0:
            unit = unit[:idx]
        result.add(unit)
    return sorted(result)


# ── internal helpers ───────────────────────────────────────────────────────


def _diff_unit(
    repo_root: Path,
    unit: str,
) -> tuple[str, bool, list[dict]]:
    """Diff every defined symbol in *unit* against its retail object.

    Returns ``(unit, ok, diffs)`` where:

    * ``ok`` is ``True`` when both the decomp and retail objects could be
      read.
    * ``diffs`` is a list of ``{"symbol": <mangled>, "mismatches": <int>}``
      for every symbol whose instruction bytes differ from retail.
      ``mismatches`` is ``-1`` when the symbol exists only in the decomp
      object (not in retail).
    """
    try:
        retail_path = verify._resolve_retail_path(repo_root, unit)
        decomp_path = verify._resolve_decomp_path(repo_root, unit)
    except FileNotFoundError as exc:
        return unit, False, [{"error": str(exc)}]

    retail_fns = {fn.name: fn for fn in verify.list_text_functions(retail_path)}
    current_fns = {fn.name: fn for fn in verify.list_text_functions(decomp_path)}

    diffs: list[dict] = []
    for name, cur_fn in current_fns.items():
        retail_fn = retail_fns.get(name)
        if retail_fn is not None:
            mismatches = verify._count_mismatches(retail_fn.code, cur_fn.code)
        else:
            mismatches = -1  # symbol exists only in decomp (added)

        if mismatches != 0:
            diffs.append({"symbol": name, "mismatches": mismatches})

    return unit, True, diffs


# ── public sweep ────────────────────────────────────────────────────────────


def sweep_dependents(
    repo_root: Path,
    units: list[str],
    tier: Tier,
    accepted_baseline: dict[str, int] | None = None,
) -> list[dict]:
    """Build and diff all dependent units for a shared-header edit.

    Parameters
    ----------
    repo_root:
        Repository root path.
    units:
        List of objdiff unit hints (e.g. ``["kyoshin/cf/OtherUnit"]``).
    tier:
        The tier assigned by :func:`tier_for`.  When ``REFUSE`` the function
        returns ``[]`` immediately.
    accepted_baseline:
        Optional mapping of ``"<unit>:<mangled_symbol>"`` to baseline mismatch
        count.  Used by the ``ACCEPTED_ONLY`` tier to restrict diffs to
        previously-matched (value == 0) symbols.  ``None`` means *all* diffs
        are included (conservative).

    Returns
    -------
    list[dict]
        Per-unit result dicts:

        * Build failure: ``{"unit": …, "ok": False, "error": "build",
          "detail": <truncated diagnostics>}``
        * Success: ``{"unit": …, "ok": True, "diffs": [{"symbol": …,
          "mismatches": <int>}, …]}``

    Build ordering & parallelism
    ----------------------------
    1. All units are **built sequentially** (``verify.build_unit`` holds the
       global build lock).
    2. Per-unit diff work (after each build) runs inside a
       ``ThreadPoolExecutor(max_workers=4)`` — builds never execute inside
       the pool.

    Hard failure
    ------------
    A broken build in **any** dependent is a hard failure overall.
    Use :func:`has_hard_failure` to check.
    """
    if tier == Tier.REFUSE:
        return []

    # ── Phase 1: build all units sequentially ──────────────────────────
    build_results: list[tuple[str, bool, str, Path | None]] = []
    for unit in units:
        success, diagnostics = verify.build_unit(repo_root, unit)
        if not success:
            # Hard failure — record and continue (caller decides to stop)
            detail = diagnostics[-500:] if len(diagnostics) > 500 else diagnostics
            build_results.append((unit, False, detail, None))
        else:
            try:
                obj_path = verify.current_object_path(repo_root, unit)
                build_results.append((unit, True, "", obj_path))
            except FileNotFoundError as exc:
                build_results.append((unit, False, f"object not found: {exc}", None))

    # ── Phase 2: diff in thread pool ──────────────────────────────────
    results: list[dict] = []

    # Separate failed and successful builds
    succeeded_units: list[tuple[str, Path]] = []
    for unit, ok, detail_or_empty, obj_path in build_results:
        if not ok:
            results.append(
                {
                    "unit": unit,
                    "ok": False,
                    "error": "build",
                    "detail": detail_or_empty,
                }
            )
        else:
            succeeded_units.append((unit, obj_path))

    # If no units succeeded, we're done
    if not succeeded_units:
        return results

    if tier in (Tier.TU_ONLY, Tier.FULL_SWEEP):
        # Diff every symbol for every unit
        with ThreadPoolExecutor(max_workers=4) as pool:
            fut_to_unit = {
                pool.submit(_diff_unit, repo_root, unit): unit
                for unit, _ in succeeded_units
            }
            for fut in as_completed(fut_to_unit):
                _unit, ok, diffs = fut.result()
                if ok:
                    results.append({"unit": _unit, "ok": True, "diffs": diffs})
                else:
                    results.append(
                        {
                            "unit": _unit,
                            "ok": False,
                            "error": "diff",
                            "detail": diffs[0].get("error", "unknown diff error"),
                        }
                    )

    elif tier == Tier.ACCEPTED_ONLY:
        # Build all but only diff protected symbols
        with ThreadPoolExecutor(max_workers=4) as pool:
            fut_to_unit = {
                pool.submit(_diff_unit, repo_root, unit): unit
                for unit, _ in succeeded_units
            }
            for fut in as_completed(fut_to_unit):
                _unit, ok, diffs = fut.result()
                if not ok:
                    results.append(
                        {
                            "unit": _unit,
                            "ok": False,
                            "error": "diff",
                            "detail": diffs[0].get("error", "unknown diff error"),
                        }
                    )
                    continue

                if accepted_baseline is not None:
                    # Only include diffs for protected symbols
                    filtered = []
                    for d in diffs:
                        key = f"{_unit}:{d['symbol']}"
                        bval = accepted_baseline.get(key)
                        if bval is not None and bval == 0:
                            filtered.append(d)
                    results.append(
                        {"unit": _unit, "ok": True, "diffs": filtered}
                    )
                else:
                    # Conservative: include everything
                    results.append({"unit": _unit, "ok": True, "diffs": diffs})

    # Sort results by unit name for deterministic output
    results.sort(key=lambda r: r["unit"])
    return results


def has_hard_failure(
    results: list[dict],
    accepted_baseline: dict[str, int] | None = None,
) -> bool:
    """Check whether the sweep results contain a hard failure.

    A hard failure is:

    * Any entry with ``ok == False`` (build or diff failure), **or**
    * Any diff on a *protected* symbol — defined as an entry in
      ``accepted_baseline`` with value ``0`` (previously matched).

    Parameters
    ----------
    results:
        Output of :func:`sweep_dependents`.
    accepted_baseline:
        Optional mapping ``"<unit>:<symbol>"`` → baseline mismatch count.
        When provided, only symbols whose baseline mismatch is ``0``
        (previously matched) are considered protected.

    Returns
    -------
    bool
    """
    for r in results:
        if not r["ok"]:
            return True
        if accepted_baseline:
            for d in r.get("diffs", []):
                key = f"{r['unit']}:{d['symbol']}"
                bval = accepted_baseline.get(key)
                if bval is not None and bval == 0:
                    return True
    return False