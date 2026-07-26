"""Verification pipeline for the LLM decompilation harness.

Design reference: docs/llm_decomp_design.md §6.

This module provides the verification gate: baseline capture, one-shot
object-vs-object sweeps, regression detection, size checks, equivalence
probes, and verdict construction.

Reuse strategy for hexdiff:
  We import tools.coop.hexdiff directly (no import errors). For per-symbol
  byte comparison we use tools.ppc_equivalence.elf_symbols.list_text_functions
  and hash/compare instruction bytes directly — never per-symbol subprocess
  calls. For path resolution we reuse hexdiff's _object_paths_for_unit.

Thread-safety:
  BuildLock wraps both an in-process threading.Lock and an interprocess
  fcntl.flock on the build lock file.
"""
from __future__ import annotations

import hashlib
import os
import shutil
import subprocess
import sys
import threading
from pathlib import Path

from tools.coop.hexdiff import _object_paths_for_unit, _postprocess_mtrand_object
from tools.coop.lib.config import load_config
from tools.coop.lib.object_size import ObjectSizeCheck, check_object_size
from tools.coop.lib.project import Project
from tools.llm_decomp.contracts import (
    Baseline,
    Regression,
    SymbolBaseline,
    Verdict,
    VerdictKind,
)
from tools.llm_decomp.paths import baseline_object_path, build_lock_path
from tools.llm_decomp.compile_diagnostic import clean_mwcc_diagnostics
from tools.ppc_equivalence.elf_symbols import list_text_functions

# ── re-export for testability ──────────────────────────────────────────────

__all__ = [
    "BuildLock",
    "build_unit",
    "capture_baseline",
    "sweep",
    "added_symbols",
    "check_size",
    "estimate_fuzzy",
    "run_equivalence",
    "verdict_for_submit",
]

# ── shared helpers ─────────────────────────────────────────────────────────


def _resolve_decomp_path(repo_root: Path, unit_hint: str) -> Path:
    """Return the decomp object path for *unit_hint* using the project manifest."""
    config = load_config(None, repo_root)
    project = Project(config)
    unit = project.resolve_unit(unit_hint)
    _retail, decomp = _object_paths_for_unit(project, unit)
    if decomp is None:
        raise FileNotFoundError(f"no decomp object path for unit {unit_hint!r}")
    return decomp


def current_object_path(repo_root: Path, unit_hint: str) -> Path:
    """Public: the decomp object path for *unit_hint* (post-build)."""
    return _resolve_decomp_path(repo_root, unit_hint)


def _resolve_retail_path(repo_root: Path, unit_hint: str) -> Path:
    """Return the retail object path for *unit_hint* using the project manifest."""
    config = load_config(None, repo_root)
    project = Project(config)
    unit = project.resolve_unit(unit_hint)
    retail, _decomp = _object_paths_for_unit(project, unit)
    if retail is None:
        raise FileNotFoundError(f"no retail object path for unit {unit_hint!r}")
    return retail


def _bytes_sha256(code: bytes) -> str:
    """Return hex sha256 of *code*."""
    return hashlib.sha256(code).hexdigest()


def _count_mismatches(retail_code: bytes, decomp_code: bytes) -> int:
    """Count non-equal 4-byte words between retail and decomp instruction blobs.

    The shorter of the two determines the comparison length; remaining words
    in the longer blob are counted as mismatches.
    """
    count = 0
    step = 4
    min_len = min(len(retail_code), len(decomp_code))
    for offset in range(0, min_len, step):
        r_word = int.from_bytes(retail_code[offset : offset + step], "big")
        d_word = int.from_bytes(decomp_code[offset : offset + step], "big")
        if r_word != d_word:
            count += 1
    # Extra words in the longer blob count as mismatches
    count += (max(len(retail_code), len(decomp_code)) - min_len) // step
    return count


# ── 1. BuildLock ───────────────────────────────────────────────────────────


class BuildLock:
    """Context manager serializing builds.

    Combines an in-process ``threading.Lock`` with an interprocess lock via
    ``fcntl.flock`` on the file at ``paths.build_lock_path(repo_root)``.

    Usage:
        with BuildLock(repo_root):
            ...
    """

    _global_lock = threading.Lock()

    def __init__(self, repo_root: Path) -> None:
        self._repo_root = repo_root
        self._lock_path = build_lock_path(repo_root)
        self._fd: int | None = None

    def __enter__(self) -> BuildLock:
        self._lock_path.parent.mkdir(parents=True, exist_ok=True)
        self._lock_path.touch(exist_ok=True)
        self._global_lock.acquire()
        try:
            fd = os.open(str(self._lock_path), os.O_RDWR)
            import fcntl

            fcntl.flock(fd, fcntl.LOCK_EX)
            self._fd = fd
        except Exception:
            self._global_lock.release()
            raise
        return self

    def __exit__(self, *exc_args: object) -> None:
        if self._fd is not None:
            import fcntl

            fcntl.flock(self._fd, fcntl.LOCK_UN)
            os.close(self._fd)
            self._fd = None
        self._global_lock.release()


# ── 2. build_unit ──────────────────────────────────────────────────────────


def build_unit(repo_root: Path, unit_hint: str) -> tuple[bool, str]:
    """Build the decomp object for *unit_hint* under ``BuildLock``.

    Returns ``(success, cleaned_diagnostics)``.
    Success is True when ninja exits with code 0 *and* the object file exists.
    Diagnostics are cleaned via ``clean_mwcc_diagnostics``.
    """
    config = load_config(None, repo_root)
    project = Project(config)
    resolved = project.resolve_unit(unit_hint)
    _retail, decomp = _object_paths_for_unit(project, resolved)

    if decomp is None:
        return False, "no decomp object path for unit"

    rel_path = str(decomp.relative_to(repo_root))

    with BuildLock(repo_root):
        result = subprocess.run(
            [project.ninja_bin(), rel_path],
            cwd=repo_root,
            check=False,
            capture_output=True,
            text=True,
        )
        _postprocess_mtrand_object(project, decomp)

    combined_output = (result.stdout or "") + "\n" + (result.stderr or "")
    diagnostics = clean_mwcc_diagnostics(combined_output)

    success = result.returncode == 0 and decomp.is_file()
    return success, diagnostics


# ── 3. capture_baseline ────────────────────────────────────────────────────


def capture_baseline(repo_root: Path, unit_hint: str, sdir: Path) -> Baseline:
    """Build *unit_hint*, save its decomp ``.o`` into *sdir*, and return a
    ``Baseline`` with a ``SymbolBaseline`` for every text symbol in the
    decomp object.

    Each ``SymbolBaseline`` stores:
      - ``byte_hash``:   sha256 of the symbol's instruction bytes (hex)
      - ``mismatch_count``: instruction mismatches vs retail (computed using
                            hexdiff-style word comparison)
      - ``fingerprint``: '' (not used at capture time)
    """
    success, diagnostics = build_unit(repo_root, unit_hint)
    if not success:
        raise RuntimeError(
            f"build failed for {unit_hint!r} during baseline capture:\n{diagnostics}"
        )

    decomp_path = _resolve_decomp_path(repo_root, unit_hint)
    saved_path = baseline_object_path(sdir)
    saved_path.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(str(decomp_path), str(saved_path))

    # Compute text size and budget
    size_check = check_size(repo_root, unit_hint)

    # Extract symbols from the decomp object
    functions = list_text_functions(saved_path)

    retail_path = _resolve_retail_path(repo_root, unit_hint)
    retail_functions = {fn.name: fn for fn in list_text_functions(retail_path)}

    symbols: dict[str, SymbolBaseline] = {}
    for fn in functions:
        retail_fn = retail_functions.get(fn.name)
        if retail_fn is not None:
            mismatch_count = _count_mismatches(retail_fn.code, fn.code)
        else:
            mismatch_count = -1  # symbol not in retail

        symbols[fn.name] = SymbolBaseline(
            byte_hash=_bytes_sha256(fn.code),
            mismatch_count=mismatch_count,
            fingerprint="",
        )

    return Baseline(
        unit=unit_hint,
        symbols=symbols,
        text_size=size_check.decomp_text or 0,
        text_budget=size_check.budget,
        object_path=str(saved_path),
    )


# ── 4. sweep ───────────────────────────────────────────────────────────────


def sweep(
    repo_root: Path,
    unit_hint: str,
    baseline: Baseline,
    current_obj: Path,
) -> list[Regression]:
    """One-shot object-vs-object comparison (review #13).

    For every symbol in *baseline.symbols*:
      - If the symbol exists in *current_obj*, hash its instruction bytes.
        When the hash differs from ``baseline.symbols[name].byte_hash``,
        recompute the mismatch count vs retail and emit a ``Regression``.
      - If found and bytes match, skip (no regression).
      - If **missing** from *current_obj*, emit a Regression with
        ``current_mismatches=-1``.

    Returns the list of regressions (empty when nothing changed).
    """
    retail_path = _resolve_retail_path(repo_root, unit_hint)
    retail_functions = {fn.name: fn for fn in list_text_functions(retail_path)}

    current_functions = {fn.name: fn for fn in list_text_functions(current_obj)}

    regressions: list[Regression] = []

    for sym_name, sym_baseline in baseline.symbols.items():
        current_fn = current_functions.get(sym_name)
        if current_fn is None:
            # Symbol missing from current object → regression
            regressions.append(
                Regression(
                    symbol=sym_name,
                    baseline_mismatches=sym_baseline.mismatch_count,
                    current_mismatches=-1,
                    was_matched=sym_baseline.mismatch_count == 0,
                )
            )
            continue

        # Check byte hash
        current_hash = _bytes_sha256(current_fn.code)
        if current_hash == sym_baseline.byte_hash:
            continue  # identical, no regression

        # Hash differs → recompute mismatch count vs retail
        retail_fn = retail_functions.get(sym_name)
        if retail_fn is not None:
            current_mismatches = _count_mismatches(retail_fn.code, current_fn.code)
        else:
            current_mismatches = -1

        regressions.append(
            Regression(
                symbol=sym_name,
                baseline_mismatches=sym_baseline.mismatch_count,
                current_mismatches=current_mismatches,
                was_matched=sym_baseline.mismatch_count == 0,
            )
        )

    return regressions


# ── 5. added_symbols ───────────────────────────────────────────────────────


def added_symbols(baseline: Baseline, current_obj: Path) -> list[str]:
    """Return exported (text) symbols in *current_obj* not present in *baseline*.

    This is the "added" direction of the bidirectional symbol-table diff
    (review #10) used to detect newly exported functions that would be a
    hard-reject signal.
    """
    current_functions = {fn.name for fn in list_text_functions(current_obj)}
    baseline_symbols = set(baseline.symbols.keys())
    added = sorted(current_functions - baseline_symbols)
    return added


# ── 6. check_size ──────────────────────────────────────────────────────────


def check_size(repo_root: Path, unit_hint: str) -> ObjectSizeCheck:
    """Return the result of a split-``.text`` budget check for *unit_hint*.

    Uses ``tools.coop.lib.object_size.check_object_size``, which reads the
    split budget from ``config/<region>/splits.txt`` and measures the
    decompiled ``.text`` section.
    """
    config = load_config(None, repo_root)
    project = Project(config)
    resolved = project.resolve_unit(unit_hint)
    retail_path, decomp_path = _object_paths_for_unit(project, resolved)

    return check_object_size(
        project_root=repo_root,
        region=config.region,
        unit_hint=unit_hint,
        retail_object=retail_path,
        decomp_object=decomp_path,
    )


# ── 7. estimate_fuzzy ──────────────────────────────────────────────────────


def estimate_fuzzy(
    mismatch_count: int,
    retail_instruction_count: int,
) -> float:
    """Estimate fuzzy match percentage for a symbol.

    ``mismatch_count`` is the number of 4-byte words that differ between
    retail and decomp.  ``retail_instruction_count`` is the total number of
    4-byte instructions in the retail function (``retail_size / 4``).
    """
    if retail_instruction_count <= 0:
        return 0.0
    match_fraction = 1.0 - (mismatch_count / retail_instruction_count)
    fuzzy = 100.0 * max(0.0, match_fraction)
    return fuzzy


# ── 8. run_equivalence ─────────────────────────────────────────────────────


_EQUIVALENCE_TIMEOUT_S = 300


def run_equivalence(repo_root: Path, unit_hint: str, symbol: str) -> str:
    """Run SMT equivalence check on *symbol* in *unit_hint*.

    Calls ``tools/coop/run.py equivalence check-unit`` as a subprocess and
    returns a short normalized verdict: ``'EQUIVALENT'``, ``'NOT_EQUIVALENT'``,
    or ``'INCONCLUSIVE'``.

    Timeout: 300 seconds.
    """
    cmd = [
        sys.executable,
        "tools/coop/run.py",
        "equivalence",
        "check-unit",
        unit_hint,
        "--symbol",
        symbol,
    ]
    try:
        result = subprocess.run(
            cmd,
            cwd=repo_root,
            check=False,
            capture_output=True,
            text=True,
            timeout=_EQUIVALENCE_TIMEOUT_S,
        )
    except subprocess.TimeoutExpired:
        return "INCONCLUSIVE"

    stdout = (result.stdout or "").strip()
    stderr = (result.stderr or "").strip()

    # Parse the verdict from stdout — check NOT_EQUIVALENT first since it
    # contains "EQUIVALENT" as a substring.
    for line in stdout.splitlines():
        stripped = line.strip().upper()
        if "NOT EQUIVALENT" in stripped or "NOT_EQUIVALENT" in stripped:
            return "NOT_EQUIVALENT"
        if "EQUIVALENT" in stripped:
            return "EQUIVALENT"

    # Check stderr as fallback
    for line in stderr.splitlines():
        stripped = line.strip().upper()
        if "NOT EQUIVALENT" in stripped or "NOT_EQUIVALENT" in stripped:
            return "NOT_EQUIVALENT"
        if "EQUIVALENT" in stripped:
            return "EQUIVALENT"

    return "INCONCLUSIVE"


# ── 9. verdict_for_submit ──────────────────────────────────────────────────

# Internal helpers used by verdict_for_submit


def _verdict_compile_error(
    target_symbol: str,
    diagnostics: str,
) -> Verdict:
    return Verdict(
        kind=VerdictKind.COMPILE_ERROR,
        accepted=False,
        target_symbol=target_symbol,
        diagnostics=diagnostics,
        rule="compile_error",
    )


def _verdict_hard_reject(
    target_symbol: str,
    rule: str,
    regressions: list[Regression] | None = None,
    text_size: int = 0,
    text_budget: int | None = None,
) -> Verdict:
    return Verdict(
        kind=VerdictKind.HARD_REJECT,
        accepted=False,
        target_symbol=target_symbol,
        regressions=regressions or [],
        text_size=text_size,
        text_budget=text_budget,
        rule=rule,
    )


def _verdict_soft_reject(
    target_symbol: str,
    rule: str,
    target_mismatches: int | None = None,
    target_fuzzy: float | None = None,
    regressions: list[Regression] | None = None,
    text_size: int = 0,
    text_budget: int | None = None,
) -> Verdict:
    return Verdict(
        kind=VerdictKind.SOFT_REJECT,
        accepted=False,
        target_symbol=target_symbol,
        target_mismatches=target_mismatches,
        target_fuzzy=target_fuzzy,
        regressions=regressions or [],
        text_size=text_size,
        text_budget=text_budget,
        rule=rule,
    )


def _verdict_accepted(
    target_symbol: str,
    rule: str,
    target_mismatches: int | None = None,
    target_fuzzy: float | None = None,
    regressions: list[Regression] | None = None,
    text_size: int = 0,
    text_budget: int | None = None,
) -> Verdict:
    return Verdict(
        kind=VerdictKind.ACCEPTED,
        accepted=True,
        target_symbol=target_symbol,
        target_mismatches=target_mismatches,
        target_fuzzy=target_fuzzy,
        regressions=regressions or [],
        text_size=text_size,
        text_budget=text_budget,
        rule=rule,
    )


def verdict_for_submit(
    repo_root: Path,
    unit_hint: str,
    target_symbol: str,
    baseline: Baseline,
    equivalence_min_fuzzy: float = 50.0,
) -> Verdict:
    """Orchestrate full verification and produce a ``Verdict``.

    Steps:
      1. Build the unit.
      2. Compute regressions (sweep) and added symbols.
      3. Check split ``.text`` budget.
      4. Evaluate target status (fuzzy match, equivalence probe).
      5. Combine into a single ``Verdict``.

    Returns:

      - ``COMPILE_ERROR`` when the build fails.
      - ``HARD_REJECT`` when an already-matched sibling regressed or an
        exported symbol was added.
      - ``SOFT_REJECT`` when an unmatched sibling got worse, the target
        is not matched, or size exceeds budget.
      - ``ACCEPTED`` when the target is a full match or equivalent match
        *and* no regressions / size issues exist.
    """
    # Step 1: Build
    success, diagnostics = build_unit(repo_root, unit_hint)
    if not success:
        return _verdict_compile_error(
            target_symbol=target_symbol,
            diagnostics=diagnostics,
        )

    # Locate the freshly built object
    decomp_path = _resolve_decomp_path(repo_root, unit_hint)

    # Step 2: Sweep + added symbols
    regressions = sweep(repo_root, unit_hint, baseline, decomp_path)
    added = added_symbols(baseline, decomp_path)

    # Step 3: Size check
    size_check = check_size(repo_root, unit_hint)

    # Step 4: Target status
    target_is_in_baseline = target_symbol in baseline.symbols
    target_mismatches: int | None = None
    target_fuzzy: float | None = None

    if target_is_in_baseline:
        # Compute current mismatches for the target symbol vs retail
        retail_path = _resolve_retail_path(repo_root, unit_hint)
        retail_fns = {fn.name: fn for fn in list_text_functions(retail_path)}
        current_fns = {fn.name: fn for fn in list_text_functions(decomp_path)}

        retail_fn = retail_fns.get(target_symbol)
        current_fn = current_fns.get(target_symbol)

        if current_fn is not None and retail_fn is not None:
            target_mismatches = _count_mismatches(retail_fn.code, current_fn.code)
            target_fuzzy = estimate_fuzzy(
                target_mismatches,
                len(retail_fn.code) // 4,
            )
        elif current_fn is None:
            target_mismatches = -1  # missing from current build
        elif retail_fn is None:
            # Target not in retail — shouldn't happen for a real target
            target_mismatches = 0
            target_fuzzy = 100.0
    else:
        # Target symbol not found in baseline — soft reject
        return _verdict_soft_reject(
            target_symbol=target_symbol,
            rule="target_missing",
            text_size=size_check.decomp_text or 0,
            text_budget=size_check.budget,
        )

    # Step 5: Check size overflow first (overrides acceptance)
    size_overflow = (
        size_check.decomp_text is not None
        and size_check.budget is not None
        and size_check.decomp_text > size_check.budget
    )

    # Check for hard-reject conditions
    if any(r.was_matched for r in regressions):
        return _verdict_hard_reject(
            target_symbol=target_symbol,
            rule="sibling_regression",
            regressions=regressions,
            text_size=size_check.decomp_text or 0,
            text_budget=size_check.budget,
        )

    if added:
        return _verdict_hard_reject(
            target_symbol=target_symbol,
            rule="added_symbol",
            regressions=regressions,
            text_size=size_check.decomp_text or 0,
            text_budget=size_check.budget,
        )

    # Check for soft-reject conditions
    for r in regressions:
        if not r.was_matched and r.current_mismatches > r.baseline_mismatches:
            return _verdict_soft_reject(
                target_symbol=target_symbol,
                rule="sibling_worse",
                target_mismatches=target_mismatches,
                target_fuzzy=target_fuzzy,
                regressions=regressions,
                text_size=size_check.decomp_text or 0,
                text_budget=size_check.budget,
            )

    if size_overflow:
        return _verdict_soft_reject(
            target_symbol=target_symbol,
            rule="size_overflow",
            target_mismatches=target_mismatches,
            target_fuzzy=target_fuzzy,
            regressions=regressions,
            text_size=size_check.decomp_text or 0,
            text_budget=size_check.budget,
        )

    # Check target status for acceptance
    if target_is_in_baseline:
        if target_mismatches == 0:
            return _verdict_accepted(
                target_symbol=target_symbol,
                rule="full_match",
                target_mismatches=target_mismatches,
                target_fuzzy=100.0,
                regressions=regressions,
                text_size=size_check.decomp_text or 0,
                text_budget=size_check.budget,
            )

        if target_fuzzy is not None and target_fuzzy >= equivalence_min_fuzzy:
            eq_result = run_equivalence(repo_root, unit_hint, target_symbol)
            if eq_result == "EQUIVALENT":
                return _verdict_accepted(
                    target_symbol=target_symbol,
                    rule="equivalent_match",
                    target_mismatches=target_mismatches,
                    target_fuzzy=target_fuzzy,
                    regressions=regressions,
                    text_size=size_check.decomp_text or 0,
                    text_budget=size_check.budget,
                )

    # Default: target not matched
    return _verdict_soft_reject(
        target_symbol=target_symbol,
        rule="not_matched",
        target_mismatches=target_mismatches,
        target_fuzzy=target_fuzzy,
        regressions=regressions,
        text_size=size_check.decomp_text or 0,
        text_budget=size_check.budget,
    )