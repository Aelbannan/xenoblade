#!/usr/bin/env python3
"""
Co-op decompilation runner for the Xenoblade downstream fork.

Usage (from repository root):
  python3 tools/coop/run.py status
  python3 tools/coop/run.py baseline
  python3 tools/coop/run.py configure
  python3 tools/coop/run.py progress
  python3 tools/coop/run.py ctx src/kyoshin/cf/CfPadTask.cpp
  python3 tools/coop/run.py build kyoshin/cf/CfPadTask
  python3 tools/coop/run.py diff kyoshin/cf/CfPadTask --symbol copyInputFlag__Q22cf9CfPadTaskFP4CPadUlUl
  python3 tools/coop/run.py size monolib/src/core/CViewRectDataCore
  python3 tools/coop/run.py size --all
  python3 tools/coop/run.py cycle pad-copy-input-flag
  python3 tools/coop/run.py queue
  python3 tools/coop/run.py targets list
  python3 tools/coop/run.py targets recertify --bottom-up --dry-run
  python3 tools/coop/run.py log --tail 20
  python3 tools/coop/run.py symbols list
  python3 tools/coop/run.py behaviour audit
  python3 tools/coop/run.py behaviour compare --all
  python3 tools/coop/run.py equivalence check-hex --original ... --candidate ... --observe r3
  python3 tools/coop/run.py equivalence check-unit kyoshin/CGame --symbol OnPauseTrigger__5CGameFv
  python3 tools/coop/run.py opcodes
  python3 tools/coop/run.py symbols show UnkClass_8045F564
"""

from __future__ import annotations

import argparse
import fcntl
import hashlib
import json
import os
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path
from typing import Optional

ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from tools.coop.lib.attempts import AttemptRecord, append_attempt, append_contribution, count_attempts, read_attempts
from tools.coop.lib.config import CoopConfig, load_config
from tools.coop.lib.equivalence_check import rebind_certificate_provenance
from tools.coop.lib.objdiff_report import (
    diff_function_json,
    evaluate_unit_match,
    find_function_match,
    meets_required_level,
    report_unit,
)
from tools.coop.lib.object_size import ObjectSizeCheck, check_object_size, format_size_check
from tools.coop.lib.project import ObjdiffUnit, Project
from tools.coop.lib.source_scan import (
    apply_to_registry,
    scan_project,
    summarize,
    symbol_status_map,
    write_scan_report,
)

from tools.coop.lib.targets import (
    _write_targets_document_unlocked,
    audit_promotion_registry,
    claim_target,
    equivalence_certificate_error,
    equivalence_certificate_migration_report,
    exclusive_targets_lock,
    get_target,
    harness_targets,
    import_symbols,
    load_targets,
    load_targets_document,
    locked_targets_document,
    pending_targets,
    plan_recertify_bottom_up,
    recertify_ready_wave,
    release_target,
    sync_results_from_attempts,
    sync_called_functions,
    sync_symbol_names,
    dedupe_registry,
    targets_path,
    update_target_result,
    validate_targets,
    write_targets_document,
)
from tools.ppc_equivalence.result import ProofStatus


def _git_head(project: Project) -> Optional[str]:
    try:
        result = subprocess.run(
            ["git", "rev-parse", "HEAD"],
            cwd=project.root,
            check=True,
            capture_output=True,
            text=True,
        )
        return result.stdout.strip()
    except (subprocess.CalledProcessError, FileNotFoundError):
        return None


def _sha1(path: Path) -> str:
    digest = hashlib.sha1()
    with path.open("rb") as f:
        for chunk in iter(lambda: f.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def _equivalence_label(evaluation) -> str:
    """Format the equivalence value for display. Byte-identical targets
    certify as full-instruction-match; the probe status enum has no
    FULL_MATCH value, so surface 'full_match' from the certificate evidence
    instead of the misleading 'equivalent' (the registry status is
    FULL_MATCH via classify_status)."""
    if evaluation.equivalence is None:
        return ""
    detail = f" ({evaluation.equivalence_detail})" if evaluation.equivalence_detail else ""
    ev = evaluation.equivalence.value
    if (
        ev == "equivalent"
        and evaluation.equivalence_certificate
        and evaluation.equivalence_certificate.get("evidence") == "full-instruction-match"
    ):
        ev = "full_match"
    return f"{ev}{detail}"


def cmd_status(project: Project, config: CoopConfig) -> int:
    print(f"project_root: {project.root}")
    print(f"region:       {config.region}")
    print(f"match_policy: {config.match_policy} (default bar: {config.default_required_level})")
    print(f"build_dir:    {config.build_dir} ({'exists' if config.build_dir.is_dir() else 'missing'})")
    print(f"objdiff.json: {config.objdiff_json} ({'exists' if config.objdiff_json.is_file() else 'missing'})")
    print(f"main.dol:     {config.main_dol} ({'exists' if config.main_dol.is_file() else 'missing'})")
    try:
        print(f"objdiff-cli:  {project.objdiff_bin()}")
    except FileNotFoundError as exc:
        print(f"objdiff-cli:  {exc}")
    targets = load_targets(config)
    buildable = sum(1 for t in targets if t.buildable)
    print(f"targets:      {len(targets)} total, {buildable} buildable")
    return 0


def cmd_baseline(project: Project, config: CoopConfig) -> int:
    if not config.main_dol.is_file():
        print(f"ERROR: missing game binary at {config.main_dol}", file=sys.stderr)
        return 1
    digest = _sha1(config.main_dol)
    print(f"main.dol sha1: {digest}")
    head = _git_head(project)
    if head:
        print(f"git HEAD:      {head}")
    cmd_configure(project, config)
    project.ninja_build("")
    print("baseline build complete")
    return 0


def cmd_configure(project: Project, config: CoopConfig) -> int:
    project.configure()
    print("configure complete")
    return 0


def cmd_progress(project: Project, config: CoopConfig, fancy: Optional[bool]) -> int:
    """Build the report.json via ninja (no linking) and print progress."""
    report_path = config.build_dir / "report.json"
    if not report_path.is_file():
        project.ninja_build(f"build/{config.region}/report.json")
    if not report_path.is_file():
        print(f"ERROR: report.json still missing after build attempt", file=sys.stderr)
        return 1
    cfg_py = ROOT / "configure.py"
    cmd = [sys.executable, str(cfg_py), "--version", config.region, "progress"]
    if fancy is not None:
        print(f"(use --fancy/--no-fancy by editing configure.py:196-201)")
    result = subprocess.run(cmd, cwd=ROOT, check=False)
    return result.returncode


def cmd_ctx(project: Project, source: Path, output: Optional[Path]) -> int:
    source_path = source if source.is_absolute() else project.root / source
    if not source_path.is_file():
        print(f"ERROR: source not found: {source_path}", file=sys.stderr)
        return 1
    out = output or source_path.with_suffix(".ctx.c")
    project.run(
        ["python3", "tools/decompctx.py", str(source_path), "-o", str(out)],
    )
    print(out)
    return 0


def _with_build_lock(region: str, fn) -> int:
    """Run *fn* while holding the repo-wide build lock.

    Same advisory flock as tools/coop/hexdiff.py (`build/<region>/.hexdiff.lock`)
    so ninja invocations here serialize against hexdiff builds and
    configure.py regeneration. The lock is held ONLY around the build — the
    witness / equivalence evaluation that follows in `cycle` must NOT run
    under it (a slow z3 simplify would freeze every other agent's builds,
    observed as a ~30 min lock hold on one acceptance, run30 incident).
    """
    lock_path = Path("build") / region / ".hexdiff.lock"
    lock_path.parent.mkdir(parents=True, exist_ok=True)
    fd = os.open(str(lock_path), os.O_CREAT | os.O_RDWR)
    try:
        try:
            fcntl.flock(fd, fcntl.LOCK_EX)
        except OSError:
            # Lock not supported on this filesystem (NFS etc.) — build anyway;
            # ninja's own lock still applies.
            os.close(fd)
            fd = -1
        return fn()
    finally:
        if fd >= 0:
            try:
                fcntl.flock(fd, fcntl.LOCK_UN)
            except OSError:
                pass
            os.close(fd)


def _build_unlocked(project: Project, hint: str) -> int:
    """Build one object WITHOUT acquiring the repo build lock.

    Callers MUST hold the lock (via _with_build_lock) themselves. Used by
    cmd_build (which adds the lock) and by cmd_cycle's lock-scoped build+read
    path, where the evaluation's ELF reads must see a freshly-built, stable
    .o (another session's ninja rewriting it mid-read was the lost-certify
    race — run 3/4).
    """
    hint_path = Path(hint)
    if hint_path.suffix in {".c", ".cpp", ".cc", ".cxx"}:
        obj = project.build_object_for_source(
            hint_path if hint_path.is_absolute() else project.root / hint_path
        )
        _postprocess_mtrand_object(project, obj)
    else:
        unit = project.resolve_unit(hint)
        if not unit.base_path:
            print(f"ERROR: unit has no compiled base path: {unit.name}", file=sys.stderr)
            return 1
        project.ninja_build(str(unit.base_path.relative_to(project.root)))
        obj = unit.base_path
        _postprocess_mtrand_object(project, obj)
    print(obj)
    return 0


def cmd_build(project: Project, hint: str) -> int:
    return _with_build_lock(project.config.region, lambda: _build_unlocked(project, hint))


def _postprocess_reloc_object(project: Project, obj: Path | None) -> None:
    """PLAN.md §17.6 reloc name drift — see tools/postprocess_reloc_names.py."""
    if obj is None:
        return
    script = project.root / "tools" / "postprocess_reloc_names.py"
    if not script.is_file():
        return
    # Script no-ops when the basename has no rules.
    subprocess.run([sys.executable, str(script), str(obj)], cwd=project.root, check=False)


def _postprocess_mtrand_object(project: Project, obj: Path | None) -> None:
    # .note.split FIRST, then reloc-name post-processing: objcopy's --add-section
    # rewrite collapses ABS symbols at st_value 0 (the trim/drop-created pool
    # labels) into the null symbol, clobbering their .text relocs. Running the
    # reloc postprocess last creates those ABS symbols after the last objcopy
    # pass.
    _postprocess_notesplit_object(project, obj)
    _postprocess_reloc_object(project, obj)


def _postprocess_notesplit_object(project: Project, obj: Path | None) -> None:
    """Copy .note.split from retail object to decomp object."""
    if obj is None:
        return
    script = project.root / "tools" / "postprocess_notesplit.py"
    if not script.is_file():
        return
    # Script is a no-op when the decomp already has .note.split
    # or when the retail counterpart is missing.
    subprocess.run([sys.executable, str(script), str(obj)], cwd=project.root, check=False)


def _object_paths_for_unit(project: Project, unit) -> tuple[Path | None, Path | None]:
  # objdiff: target_path = retail split object, base_path = decompiled object
    retail = unit.target_path
    decomp = unit.base_path
    return retail, decomp


def _data_object_paths(project: Project, unit) -> tuple[Path | None, Path | None]:
    """Retail/decompiled object paths for data-section comparison.

    Data-only units may not have a configured base_path yet (no source wired
    into objdiff.json); derive the conventional build/us/src/<unit>.o path
    from the unit name so `data diff` works before/after the configure swap.
    """
    retail = unit.target_path
    decomp = unit.base_path
    if decomp is None and retail is not None:
        name = unit.name
        stem = Path(name).stem
        decomp = Path("build") / project.config.region / "src" / f"{stem}.o"
    return retail, decomp


def _postprocess_data_copy(project: Project, decomp: Path) -> Path | None:
    """Return a postprocessed COPY of the decompiled object for the data gate.

    The rest of the pipeline (``run.py diff`` / hexdiff) applies the
    PLAN.md §17.6 reloc-name postprocess (tools/postprocess_reloc_names.py)
    to the decompiled object before comparing; ``data diff`` applies the same
    rules to a temp copy so the gate sees the postprocessed object while the
    build .o is never mutated. Units without UNIT_RULES pass through
    unchanged (None). Use --no-postprocess to compare the raw object.
    """
    script = project.root / "tools" / "postprocess_reloc_names.py"
    if not script.is_file():
        return None
    try:
        from tools.postprocess_reloc_names import UNIT_RULES  # type: ignore[import-not-found]
    except Exception:
        return None
    if Path(decomp).name not in UNIT_RULES:
        return None
    scratch = project.root / ".scratch"
    scratch.mkdir(parents=True, exist_ok=True)
    tmpdir = Path(tempfile.mkdtemp(prefix="data-diff-", dir=scratch))
    tmp = tmpdir / Path(decomp).name
    shutil.copy2(decomp, tmp)
    subprocess.run(
        [sys.executable, str(script), str(tmp)],
        cwd=project.root,
        check=False,
        capture_output=True,  # never pollute the gate output
    )
    return tmp


def _cmd_data_diff(project: Project, config: CoopConfig, hint: str | None, *, check_all: bool, postprocess: bool = True) -> int:
    from tools.coop.lib.data_match import (
        DataMatchResult,
        check_data_sections,
        format_data_result,
        has_data_sections,
    )

    def _compare(retail: Path, decomp: Path) -> tuple[DataMatchResult, str]:
        """Compare retail vs decomp for the data gate.

        Default policy: compare the raw object first; if it already matches,
        report MATCH untouched (a stale §17.6 rule must never regress a
        matched unit). Only when the raw comparison FAILS is the §17.6
        reloc-name postprocess applied to a temp copy (the same rules
        hexdiff / ``run.py diff`` apply) and the comparison repeated.
        ``--no-postprocess`` forces the raw comparison.
        Returns (result, note) where note explains which object was compared.
        """
        raw_result = check_data_sections(retail, decomp)
        if raw_result.ok:
            note = "raw object already data-matched (postprocess skipped)" if postprocess else ""
            return raw_result, note
        if not postprocess:
            return raw_result, ""
        tmp = _postprocess_data_copy(project, decomp)
        if tmp is None:
            return raw_result, ""
        try:
            result = check_data_sections(retail, tmp)
        finally:
            shutil.rmtree(tmp.parent, ignore_errors=True)
        return result, "§17.6 reloc-name rules applied to a temp copy"

    failures = 0
    units = project.load_objdiff_units()
    if check_all:
        for unit in units:
            retail, decomp = _data_object_paths(project, unit)
            if retail is None or decomp is None or not decomp.is_file():
                continue
            if not has_data_sections(decomp):
                # extern-only TU: data still ships from the retail side; nothing
                # to verify yet (the migration target is to define it in source).
                continue
            result, _note = _compare(retail, decomp)
            print(f"unit: {unit.name}  [{result.per_section_status()}]")
            if not result.ok:
                failures += 1
        print(f"\n{len(units)} units scanned; {failures} data-mismatch failure(s)")
        return 1 if failures else 0

    if hint is None:
        print("ERROR: unit hint required (or use --all)", file=sys.stderr)
        return 2
    try:
        unit = project.resolve_unit(hint)
    except ValueError as exc:
        print(f"ERROR: {exc}", file=sys.stderr)
        return 2
    retail, decomp = _data_object_paths(project, unit)
    if retail is None or not retail.is_file():
        print(f"ERROR: retail object missing: {retail}", file=sys.stderr)
        return 2
    if decomp is None or not decomp.is_file():
        print(
            f"ERROR: decompiled object missing: {decomp}\n"
            "       (data-only units: build the source first, or wire it into configure.py)",
            file=sys.stderr,
        )
        return 2
    result, note = _compare(retail, decomp)
    print(f"unit: {unit.name}")
    if note:
        print(f"  (postprocess: {note})")
    print(format_data_result(result))
    return 0 if result.ok else 1


def _print_object_size(project: Project, config: CoopConfig, unit_hint: str, unit) -> ObjectSizeCheck:
    retail, decomp = _object_paths_for_unit(project, unit)
    check = check_object_size(
        project_root=project.root,
        region=config.region,
        unit_hint=unit_hint,
        retail_object=retail,
        decomp_object=decomp,
    )
    print(f"size:    {format_size_check(check)}")
    if check.split_path:
        print(f"          split: {check.split_path}")
    return check


def cmd_diff(
    project: Project,
    config: CoopConfig,
    hint: str,
    symbol: Optional[str],
    *,
    write_function_diff: bool,
    linked: bool = False,
    smt: bool = True,
    witness_enabled: bool = True,
) -> int:
    unit = project.resolve_unit(hint)
    if unit.base_path:
        project.ninja_build(str(unit.base_path.relative_to(project.root)))
        _postprocess_mtrand_object(project, unit.base_path)

    target_id = None
    if symbol:
        registry_matches = [
            target for target in load_targets(config)
            if target.symbol == symbol and target.unit
            and (target.unit == unit.name or unit.name.endswith("/" + target.unit))
        ]
        if len(registry_matches) == 1:
            target_id = registry_matches[0].id
    evaluation = evaluate_unit_match(
        project, unit, symbol, linked=linked, target_id=target_id, run_smt=smt,
        witness_enabled=witness_enabled,
    )
    unit_report = evaluation.unit_report
    fn_match = evaluation.fn_match

    print(f"unit: {unit.name}")
    print(
        f"code: {unit_report.code_match_percent:.1f}%  "
        f"data: {unit_report.data_match_percent:.1f}%  "
        f"fuzzy: {unit_report.fuzzy_match_percent:.1f}%  "
        f"functions: {unit_report.matched_functions}/{unit_report.total_functions}"
    )
    if fn_match:
        print(
            f"symbol: {fn_match.name}  "
            f"match: {fn_match.match_percent:.1f}%  "
            f"size: 0x{fn_match.size:X}"
        )
    elif symbol:
        print(f"WARNING: symbol not found in report: {symbol}", file=sys.stderr)

    if evaluation.equivalence is not None:
        print(f"equivalence: {_equivalence_label(evaluation)}")
    # r8 WS-1: witness rejection telemetry — the model needs the actionable
    # failure (reloc drift / rho / callee / structural + diverging component)
    # instead of a bare "NOT certifiable" to fix near-match targets.
    if evaluation.witness_gate:
        print(f"witness-gate: {evaluation.witness_gate} | {evaluation.witness_reason or ''}")

    if write_function_diff and symbol and fn_match:
        out = config.resolve(Path("build/coop-function-diff.json"))
        try:
            diff_function_json(project, unit, symbol, out)
            print(f"function diff: {out}")
        except subprocess.CalledProcessError as exc:
            # Newer objdiff-cli dropped `diff -o/--format`; match % already from report.
            print(
                f"WARNING: function-diff JSON skipped ({exc.returncode}): {' '.join(exc.cmd)}",
                file=sys.stderr,
            )

    print(f"status: {evaluation.status}")
    size_check = _print_object_size(project, config, hint, unit)
    # Function acceptance is per-function (user policy 2026-08): the unit
    # split size is informational here (NOTE only) — it gates unit promotion
    # (TU-final configure.py NonMatching->Matching), not the function verdict.
    if not size_check.ok:
        print(
            f"NOTE: unit split size exceeds budget (decomp .text over by "
            f"{size_check.over_by} bytes) — per-function verdict above "
            f"stands; the unit cannot be promoted to Matching until the "
            f"size is fixed",
            file=sys.stderr,
        )
    return 0


def cmd_size(
    project: Project,
    config: CoopConfig,
    hint: str | None,
    *,
    check_all: bool,
) -> int:
    failures = 0
    if check_all:
        units = project.load_objdiff_units()
        for unit in units:
            if unit.base_path is None:
                continue
            print(f"unit: {unit.name}")
            check = _print_object_size(project, config, unit.name, unit)
            if not check.ok:
                failures += 1
            print()
        return 1 if failures else 0

    if hint is None:
        print("ERROR: unit hint required (or use --all)", file=sys.stderr)
        return 2

    unit = project.resolve_unit(hint)
    if unit.base_path:
        project.ninja_build(str(unit.base_path.relative_to(project.root)))
        _postprocess_mtrand_object(project, unit.base_path)
    check = _print_object_size(project, config, hint, unit)
    return 0 if check.ok else 1


def _callee_readiness_reasons(targets, target) -> list[str]:
    """Cheap registry-only preflight: would the SMT probe fail closed on callees?

    Mirrors the certified-callee gate in
    ``equivalence_check._load_certified_callees`` (indirect calls / unresolved
    direct callees / unaccepted callee tree) so ``cycle --smt`` can warn BEFORE
    the ctx+build work instead of after the probe reports it.
    """
    from tools.coop.lib.targets import ACCEPTED_MATCH_STATUSES

    reasons: list[str] = []
    extra = target.extra or {}
    if extra.get("has_indirect_calls"):
        reasons.append("has_indirect_calls")
    unresolved = extra.get("unresolved_called_functions")
    if unresolved:
        if isinstance(unresolved, list):
            reasons.append(f"unresolved direct callees ({len(unresolved)})")
        else:
            reasons.append("unresolved direct callees")
    called = extra.get("called_functions") or []
    if isinstance(called, list):
        by_id = {t.id: t for t in targets}
        not_ready = [
            str(cid) for cid in called
            if str(cid) not in by_id or by_id[str(cid)].status not in ACCEPTED_MATCH_STATUSES
        ]
        if not_ready:
            reasons.append("unaccepted callees: " + ", ".join(sorted(not_ready)[:6]))
    return reasons


def cmd_cycle(
    project: Project,
    config: CoopConfig,
    target_id: str,
    *,
    hypothesis: str,
    next_change: str,
    runtime_test: str,
    linked: bool = False,
    add_to_kb: str = "",
    contract: str = "auto",
    smt: bool = False,
    witness_timeout_ms: int = 0,
    witness_enabled: bool = True,
) -> int:
    targets = load_targets(config)
    target = get_target(targets, target_id)
    if not target.buildable:
        print(f"ERROR: target '{target_id}' is not buildable yet ({target.notes})", file=sys.stderr)
        return 1

    if smt:
        reasons = _callee_readiness_reasons(targets, target)
        if reasons:
            print(
                "NOTE: --smt probe will be inconclusive_unvalidated_callee "
                f"(callee tree not ready): {'; '.join(reasons)}. "
                "FULL_MATCH / witness certification still runs.",
                file=sys.stderr,
            )

    assert target.source is not None
    assert target.unit is not None

    ctx_out = target.source.with_suffix(".ctx.c")
    cmd_ctx(project, target.source.relative_to(project.root), ctx_out)

    # §2.7.6: honor a declared_return recorded on the target's stored
    # certificate (manual review), so cycle re-proofs replay it.
    _declared_return = None
    try:
        _raw = load_targets_document(config)
        for _row in _raw.get("targets", []):
            if _row.get("id") == target_id:
                _abi = (_row.get("equivalence_certificate") or {}).get("abi_shape")
                if isinstance(_abi, dict):
                    _declared_return = _abi.get("declared_return")
                if _declared_return is None and isinstance(
                    _row.get("declared_return"), str
                ):
                    _declared_return = _row["declared_return"]
                break
    except (OSError, ValueError):
        pass

    unit = project.resolve_unit(target.unit)

    # Lock-scoped build + evaluation (no-z3 configs only). With the witness
    # disabled and --smt off, the whole evaluation is ELF reads + a byte
    # comparison (report_unit + certify_unit_symbol byte-identity path) —
    # milliseconds, no z3. Holding the repo build lock across the build AND
    # those reads guarantees the .o seen by evaluate_unit_match was just
    # built from the current source and no other session's ninja can rewrite
    # it mid-read (the torn-read race that lost byte-identical certifies,
    # runs 3-4: us-8044bae8 FULL_MATCH evaluated NOT_STARTED; us-801652b0
    # COMPILES). The lock is deliberately NOT held when the witness is
    # enabled or --smt runs — those invoke z3, and a z3 simplify under the
    # lock froze every agent for ~30 min (run30 incident).
    if not witness_enabled and not smt:
        def _locked_cycle():
            _build_unlocked(project, target.unit)
            if unit.base_path:
                _postprocess_mtrand_object(project, unit.base_path)
            return evaluate_unit_match(
                project, unit, target.symbol, linked=linked, target_id=target.id,
                declared_return=_declared_return, contract=contract, run_smt=smt,
                witness_timeout_ms=witness_timeout_ms,
                witness_enabled=witness_enabled,
            )

        evaluation = _with_build_lock(project.config.region, _locked_cycle)
    else:
        cmd_build(project, target.unit)
        if unit.base_path:
            _postprocess_mtrand_object(project, unit.base_path)
        evaluation = evaluate_unit_match(
            project, unit, target.symbol, linked=linked, target_id=target.id,
            declared_return=_declared_return, contract=contract, run_smt=smt,
            witness_timeout_ms=witness_timeout_ms,
            witness_enabled=witness_enabled,
        )
    unit_report = evaluation.unit_report
    fn_match = evaluation.fn_match

    print(f"unit: {unit.name}")
    print(
        f"code: {unit_report.code_match_percent:.1f}%  "
        f"data: {unit_report.data_match_percent:.1f}%  "
        f"fuzzy: {unit_report.fuzzy_match_percent:.1f}%  "
        f"functions: {unit_report.matched_functions}/{unit_report.total_functions}"
    )
    if fn_match:
        print(
            f"symbol: {fn_match.name}  "
            f"match: {fn_match.match_percent:.1f}%  "
            f"size: 0x{fn_match.size:X}"
        )
    if evaluation.equivalence is not None:
        print(f"equivalence: {_equivalence_label(evaluation)}")
    # r8 WS-1: witness rejection telemetry (see cmd_diff).
    if evaluation.witness_gate:
        print(f"witness-gate: {evaluation.witness_gate} | {evaluation.witness_reason or ''}")
    if evaluation.equivalence_certificate:
        print(
            "certificate: semantic-certified "
            + evaluation.equivalence_certificate["certificate_sha256"]
        )
    elif evaluation.certificate_checked:
        print("certificate: unavailable (target cannot yet serve as a trusted callee)")
    print(f"status: {evaluation.status}")

    if target.symbol and fn_match:
        out = config.resolve(Path("build/coop-function-diff.json"))
        try:
            diff_function_json(project, unit, target.symbol, out)
            print(f"function diff: {out}")
        except subprocess.CalledProcessError as exc:
            print(
                f"WARNING: function-diff JSON skipped ({exc.returncode}): {' '.join(exc.cmd)}",
                file=sys.stderr,
            )

    size_check = _print_object_size(project, config, target.unit, unit)

    # Always log the attempt and update the registry, even if the unit-level
    # split size is over budget — so the target's match progress is recorded
    # and claim-smallest will not re-pick it.
    # §2.7.6: extract declared_return from the certificate's abi_shape.
    declared_return_value: str | None = None
    if evaluation.equivalence_certificate is not None:
        cert_abi = evaluation.equivalence_certificate.get("abi_shape")
        if isinstance(cert_abi, dict):
            declared_return_value = cert_abi.get("declared_return")

    attempt_num = count_attempts(config.resolve(config.attempt_log), target.id) + 1
    record = AttemptRecord(
        target_id=target.id,
        function=target.function,
        region=config.region,
        unit=unit.name,
        symbol=target.symbol,
        status=evaluation.status,
        instruction_match=fn_match.match_percent if fn_match else unit_report.fuzzy_match_percent,
        relocation_match=None,
        code_match_percent=unit_report.code_match_percent,
        data_match_percent=unit_report.data_match_percent,
        hypothesis=hypothesis,
        next_change=next_change,
        runtime_test=runtime_test,
        git_commit=_git_head(project),
        equivalence_status=evaluation.equivalence.value if evaluation.equivalence else None,
        equivalence_detail=evaluation.equivalence_detail,
        equivalence_confidence=evaluation.equivalence_confidence,
        equivalence_policy=evaluation.equivalence_policy,
        add_to_kb=add_to_kb,
        declared_return=declared_return_value,
    )
    log_path = append_attempt(config.resolve(config.attempt_log), record)
    print(f"attempt #{attempt_num} logged to {log_path}")

    if add_to_kb.strip():
        kb_path = append_contribution(project.root, add_to_kb)
        if kb_path:
            print(f"kb contribution logged to {kb_path}")
    update_target_result(
        config,
        target.id,
        status=evaluation.status,
        instruction_match=record.instruction_match,
        equivalence_status=record.equivalence_status,
        equivalence_certificate=evaluation.equivalence_certificate,
        certificate_checked=evaluation.certificate_checked,
        equivalence_confidence=evaluation.equivalence_confidence,
        equivalence_policy=evaluation.equivalence_policy,
    )
    print(f"target registry updated: {target.id}")

    fn_percent = fn_match.match_percent if fn_match else None
    if not meets_required_level(
        target.required_level,
        evaluation.status,
        function_match=fn_percent,
        unit=unit_report,
        symbol=target.symbol,
        equivalence=evaluation.equivalence,
        policy=config,
    ):
        print(
            f"FAIL: required {target.required_level}, got {evaluation.status} "
            f"(function={fn_percent}, code={unit_report.code_match_percent:.1f}%, "
            f"data={unit_report.data_match_percent:.1f}%, "
            f"equivalence={evaluation.equivalence.value if evaluation.equivalence else 'n/a'})",
            file=sys.stderr,
        )
        return 1

    # Function acceptance is gated ONLY on the function's own match — NOT the
    # unit split size. A byte-identical function is proven correct regardless
    # of whether the unit's object exceeds its retail split budget (the
    # overrun comes from OTHER functions in the unit). The unit size gate
    # lives in TU-final, where it gates the configure.py NonMatching->Matching
    # promotion — not per-function acceptance (user policy 2026-08).
    if not size_check.ok:
        print(
            f"NOTE: unit split size exceeds budget (decomp .text over by "
            f"{size_check.over_by} bytes) — function accepted, but the unit "
            f"cannot be promoted to Matching until the size is fixed",
            file=sys.stderr,
        )
    print(f"PASS: meets required level {target.required_level}")
    return 0


def cmd_queue(
    project: Project,
    config: CoopConfig,
    *,
    dry_run: bool,
    selection: str = "pending",
    limit: Optional[int] = None,
) -> int:
    all_targets = load_targets(config)
    if selection == "pending":
        targets = pending_targets(all_targets)
    else:
        targets = harness_targets(all_targets, selection=selection)
    if limit is not None:
        targets = targets[:limit]
    if not targets:
        print("no pending buildable targets")
        return 0
    print(f"queue: {len(targets)} target(s)")
    failures = 0
    for target in targets:
        print(f"\n==> {target.id}")
        if dry_run:
            continue
        rc = cmd_cycle(
            project,
            config,
            target.id,
            hypothesis=f"harness selection: {selection}",
            next_change="inspect remaining static/equivalence mismatch",
            runtime_test="",
        )
        if rc != 0:
            failures += 1
    return 1 if failures else 0


def cmd_targets_list(
    config: CoopConfig,
    milestone: Optional[str],
    workflow_status: Optional[str],
    match_status: Optional[str],
    kind: Optional[str],
) -> int:
    targets = load_targets(config)
    if milestone:
        targets = [t for t in targets if t.milestone == milestone]
    if workflow_status:
        targets = [t for t in targets if t.workflow_status == workflow_status]
    if match_status:
        targets = [t for t in targets if t.status == match_status]
    if kind:
        targets = [t for t in targets if t.kind == kind]
    for target in targets:
        buildable = "yes" if target.buildable else "no"
        print(
            f"{target.id:28} {target.milestone:16} "
            f"flow={target.workflow_status:12} match={target.status:16} "
            f"buildable={buildable:3} {target.function}"
        )
    return 0


def cmd_targets_show(config: CoopConfig, target_id: str) -> int:
    target = get_target(load_targets(config), target_id)
    print(f"id:       {target.id}")
    print(f"milestone:{target.milestone}")
    print(f"function: {target.function}")
    print(f"symbol:   {target.symbol}")
    print(f"address:  {target.address}")
    print(f"source:   {target.source}")
    print(f"unit:     {target.unit}")
    print(f"required: {target.required_level}")
    print(f"workflow: {target.workflow_status}")
    print(f"match:    {target.status}")
    print(f"kind:     {target.kind}")
    source_present = target.extra.get("source_present")
    if isinstance(source_present, bool):
        print(f"source:   {'present' if source_present else 'absent'} (from scan-source)")
    if target.notes:
        print(f"notes:    {target.notes}")
    claim = target.extra.get("claim")
    if isinstance(claim, dict):
        print(f"owner:    {claim.get('owner')}")
        print(f"scope:    {', '.join(claim.get('allowed_paths', []))}")
    if target.extra.get("callgraph_status"):
        print(f"calls:    {len(target.extra.get('called_functions', []))} resolved")
        for called_id in target.extra.get("called_functions", []):
            print(f"          - {called_id}")
        for unresolved in target.extra.get("unresolved_called_functions", []):
            print(f"          - unresolved: {unresolved}")
        if target.extra.get("abi_helper_calls"):
            print(
                "          - ABI helpers: "
                + ", ".join(target.extra["abi_helper_calls"])
            )
        if target.extra.get("has_indirect_calls"):
            print("          - indirect call present")
    certificate = target.extra.get("equivalence_certificate")
    if isinstance(certificate, dict):
        print(f"certificate: {certificate.get('status', 'invalid')}")
        print(f"          {certificate.get('certificate_sha256', 'missing hash')}")
    return 0


def _resolved_target_rows(config: CoopConfig) -> list[dict]:
    rows: list[dict] = []
    for target in load_targets(config):
        match_status = target.status
        instruction_match = target.extra.get("instruction_match")
        workflow = target.workflow_status
        if match_status in {"FULL_MATCH", "EQUIVALENT_MATCH"}:
            workflow = "ACCEPTED"
        rows.append(
            {
                "id": target.id,
                "function": target.function,
                "kind": target.kind,
                "milestone": target.milestone,
                "workflow_status": workflow,
                "match_status": match_status,
                "instruction_match": instruction_match,
                "buildable": target.buildable,
                "unit": target.unit,
                "catalog": target.extra.get("origin") == "symbols.txt",
                "source_present": target.extra.get("source_present"),
                "owner": (
                    target.extra.get("claim", {}).get("owner")
                    if isinstance(target.extra.get("claim"), dict)
                    else None
                ),
            }
        )
    return rows


def _filter_target_rows(
    rows: list[dict],
    *,
    milestone: Optional[str],
    kind: Optional[str],
) -> list[dict]:
    if milestone:
        rows = [row for row in rows if row["milestone"] == milestone]
    if kind:
        rows = [row for row in rows if row["kind"] == kind]
    return rows


def _render_target_status_markdown(rows: list[dict], region: str) -> str:
    workflow_counts: dict[str, int] = {}
    for row in rows:
        workflow = row["workflow_status"]
        workflow_counts[workflow] = workflow_counts.get(workflow, 0) + 1
    lines = [
        "<!-- GENERATED by tools/coop/run.py targets status. Do not edit. -->",
        "",
        "# Target status",
        "",
        f"Region: `{region}`",
        "",
        "## Summary",
        "",
        f"- Total: {len(rows)}",
    ]
    for name in sorted(workflow_counts):
        lines.append(f"- {name}: {workflow_counts[name]}")
    by_milestone: dict[str, list[dict]] = {}
    for row in rows:
        by_milestone.setdefault(row["milestone"], []).append(row)
    for milestone in sorted(by_milestone):
        lines.extend(
            [
                "",
                f"## {milestone}",
                "",
                "| Target | Function | Workflow | Match | Percent | Src | Owner | Buildable |",
                "|---|---|---|---|---|---:|---|---|---|",
            ]
        )
        for row in sorted(by_milestone[milestone], key=lambda item: item["id"]):
            percent = row["instruction_match"]
            percent_text = f"{percent:.1f}%" if isinstance(percent, (int, float)) else "—"
            function = str(row["function"]).replace("|", "\\|")
            src = row.get("source_present")
            src_text = "yes" if src is True else ("no" if src is False else "—")
            lines.append(
                f"| `{row['id']}` | {function} | "
                f"{row['workflow_status']} | {row['match_status']} | {percent_text} | "
                f"{src_text} | "
                f"{row['owner'] or '—'} | "
                f"{'yes' if row['buildable'] else 'no'} |"
            )
    return "\n".join(lines) + "\n"


def cmd_targets_status(
    config: CoopConfig,
    *,
    milestone: Optional[str],
    kind: Optional[str],
    output: Optional[Path],
    output_format: str,
) -> int:
    rows = _filter_target_rows(
        _resolved_target_rows(config),
        milestone=milestone,
        kind=kind,
    )
    if output_format == "json":
        rendered = json.dumps({"region": config.region, "targets": rows}, indent=2) + "\n"
    else:
        rendered = _render_target_status_markdown(rows, config.region)
    if output:
        path = output if output.is_absolute() else config.project_root / output
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(rendered, encoding="utf-8")
        print(path)
    else:
        print(rendered, end="")
    return 0


def cmd_targets_validate(config: CoopConfig) -> int:
    errors = validate_targets(config)
    if errors:
        for error in errors:
            print(f"ERROR: {error}", file=sys.stderr)
        print(f"target registry invalid: {len(errors)} error(s)", file=sys.stderr)
        return 1
    print(f"target registry valid: {len(load_targets(config))} target(s)")
    return 0


def cmd_targets_import_symbols(
    project: Project,
    config: CoopConfig,
    *,
    kind: str,
    dry_run: bool,
) -> int:
    label = "all symbols" if kind == "all" else f"{kind} symbols"
    with locked_targets_document(config) as (data, write):
        data, added, skipped = import_symbols(project, config, kind=kind, _data=data)
        print(f"import {label}: {added} add, {skipped} already present")
        if dry_run:
            print("dry-run: registry not changed")
            return 0
        path = write()
        print(f"updated: {path}")
    return cmd_targets_validate(config)


def cmd_targets_sync_attempts(config: CoopConfig) -> int:
    attempts = read_attempts(config.resolve(config.attempt_log))
    changed = sync_results_from_attempts(config, attempts)
    print(f"synced latest attempts into target registry: {changed} target(s) changed")
    return cmd_targets_validate(config)


def cmd_targets_sync_calls(project: Project, config: CoopConfig, *, dry_run: bool) -> int:
    with locked_targets_document(config) as (data, write):
        data, scanned, resolved, unresolved = sync_called_functions(
            project, config, _data=data
        )
        print(
            f"call graph: {scanned} function record(s), "
            f"{resolved} resolved direct edge(s), {unresolved} unresolved direct edge(s)"
        )
        if dry_run:
            print("dry-run: registry not changed")
            return 0
        path = write()
        print(f"updated: {path}")
    return cmd_targets_validate(config)


def cmd_targets_sync_symbols(project: Project, config: CoopConfig, *, dry_run: bool) -> int:
    """Re-sync imported registry symbol names from the current symbols.txt.

    Fixes rows whose address-based entries drifted: symbol recovery renamed
    the function in symbols.txt after the row was imported, so the row still
    carries the old placeholder name (e.g. ``func_80058714`` vs the current
    ``getSubField78``).  Curated rows are never touched.
    """
    with locked_targets_document(config) as (data, write):
        data, renamed, size_only, unchanged = sync_symbol_names(
            project, config, _data=data
        )
        print(
            f"symbol-name sync: {renamed} renamed, "
            f"{size_only} size-only, {unchanged} unchanged"
        )
        if dry_run:
            print("dry-run: registry not changed")
            return 0
        path = write()
        print(f"updated: {path}")
    return cmd_targets_validate(config)


def cmd_targets_dedupe(config: CoopConfig, *, dry_run: bool) -> int:
    """Drop duplicate address rows (e.g. ``us-80058d7c-2``) keeping the
    canonical base row, re-pointing callgraph references to the kept id."""
    with locked_targets_document(config) as (data, write):
        data, removed, repointed = dedupe_registry(config, _data=data)
        print(f"dedupe: {removed} duplicate row(s) removed, {repointed} reference(s) re-pointed")
        if dry_run:
            print("dry-run: registry not changed")
            return 0
        path = write()
        print(f"updated: {path}")
    return cmd_targets_validate(config)


def cmd_targets_claim(
    config: CoopConfig,
    target_id: str,
    *,
    owner: str,
    allowed_paths: list[str],
    note: str,
) -> int:
    target = get_target(load_targets(config), target_id)
    if not allowed_paths and target.source:
        allowed_paths = [str(target.source.relative_to(config.project_root))]
    path = claim_target(
        config,
        target_id,
        owner=owner,
        allowed_paths=allowed_paths,
        note=note,
    )
    print(f"claimed {target_id} for {owner}: {path}")
    return 0


def cmd_targets_claim_batch(
    config: CoopConfig,
    target_ids: list[str],
    *,
    owner: str,
    allowed_paths: list[str],
    note: str,
) -> int:
    """Claim N targets in ONE lock hold + ONE registry write (run32: the
    per-target claim rewrote the full 17MB registry under the exclusive lock
    each time — 30 sessions × 10 targets ≈ 300 serialized rewrites blew the
    120s lock timeout). Batching removes the contention."""
    from tools.coop.lib.targets import claim_targets_batch

    # Resolve allowed_paths once (per source file), shared across the batch.
    if not allowed_paths:
        srcs: set[str] = set()
        for t in load_targets(config):
            if t.id in target_ids and t.source:
                srcs.add(str(t.source.relative_to(config.project_root)))
        allowed_paths = sorted(srcs)
    paths = claim_targets_batch(
        config,
        target_ids,
        owner=owner,
        allowed_paths=allowed_paths,
        note=note,
    )
    for tid, p in paths.items():
        print(f"claimed {tid} for {owner}: {p}")
    return 0


def cmd_targets_release_batch(config: CoopConfig, target_ids: list[str], *, owner: Optional[str]) -> int:
    """Release N claims in ONE lock hold + ONE registry write."""
    from tools.coop.lib.targets import release_targets_batch

    paths = release_targets_batch(config, target_ids, owner=owner)
    for tid, p in paths.items():
        print(f"released {tid}: {p}")
    return 0


def cmd_targets_release(config: CoopConfig, target_id: str, *, owner: Optional[str]) -> int:
    path = release_target(config, target_id, owner=owner)
    print(f"released {target_id}: {path}")
    return 0


def cmd_targets_claim_smallest(
    config: CoopConfig,
    *,
    num: int = 1,
    owner: str = "",
    no_claim: bool = False,
) -> int:
    """Claim and print the smallest NOT_STARTED function(s) by binary size."""
    targets = load_targets(config)

    # Filter to NOT_STARTED, buildable functions
    candidates = [
        t for t in targets
        if t.status == "NOT_STARTED" and t.kind == "function" and t.buildable
    ]

    if not candidates:
        print("no NOT_STARTED buildable functions found")
        return 1

    # Sort by size ascending (hex string like "0x29C")
    def _size_key(t):
        s = getattr(t, "extra", None) or {}
        raw = s.get("size") or ""
        if isinstance(raw, str) and raw.startswith("0x"):
            return int(raw, 16)
        return 0

    # Also check on the Target directly
    def _size_val(t):
        # t.extra is a dict that holds leftover fields including "size"
        raw = t.extra.get("size", "0x0")
        if isinstance(raw, str) and raw.startswith("0x"):
            return int(raw, 16)
        if isinstance(raw, (int, float)):
            return int(raw)
        return 0

    candidates.sort(key=_size_val)

    selected = candidates[:num]
    count = 0

    for target in selected:
        source = str(target.source.relative_to(config.project_root)) if target.source else "unresolved"
        size_hex = target.extra.get("size", "?")
        size_str = str(size_hex)

        do_claim = owner and not no_claim
        if do_claim:
            allowed_paths = [source] if target.source else []
            try:
                claim_target(
                    config,
                    target.id,
                    owner=owner,
                    allowed_paths=allowed_paths,
                    note=f"claimed via claim-smallest (size={size_str})",
                )
            except ValueError as exc:
                print(f"WARNING: {exc}", file=sys.stderr)
                continue

        count += 1
        claimed_mark = " [CLAIMED]" if do_claim else ""
        parts = [
            f"id:       {target.id}",
            f"function: {target.function}",
            f"source:   {source}",
            f"size:     {size_str}",
        ]
        print(f"--- smallest NOT_STARTED #{count}{claimed_mark} ---")
        for line in parts:
            print(line)

    return 0 if count == len(selected) else 1


def cmd_targets_scan_source(
    project: Project,
    config: CoopConfig,
    *,
    update: bool,
    dry_run: bool,
) -> int:
    """Scan the whole binary for which retail functions exist in source.

    Parses the existing retail/decompiled object pairs (no build) and, for
    every retail function, records whether a decompiled implementation exists
    in the same unit (matched by mangled symbol).  Writes the catalog-wide
    scan to ``build/<region>/coop-source-scan.json`` and, with ``--update``,
    stamps ``source_present`` onto every matching registry row so status
    reflects source existence rather than just match state.
    """
    report_path = (
        config.project_root
        / "build"
        / config.region
        / "coop-source-scan.json"
    )
    print(f"scanning {config.region} retail vs decompiled objects (no build)...")
    by_unit = scan_project(project)
    summary = summarize(by_unit)
    write_scan_report(project, by_unit, report_path)
    print(f"report: {report_path}")
    print(
        f"units scanned {summary['scanned_units']}/{summary['units']} "
        f"({summary['unscanned_units']} unscanned)"
    )
    print(
        f"retail functions {summary['retail_functions']}: "
        f"{summary['in_source']} in source, "
        f"{summary['absent_from_source']} absent, "
        f"{summary['byte_identical']} byte-identical (reloc-free), "
        f"{summary['reloc_compatible']} reloc-compatible (masked), "
        f"{summary['unbuilt_source']} unbuilt-source, "
        f"{summary['stale_objects']} stale objects"
    )
    if summary["decompiled_only"]:
        print(
            f"note: {summary['decompiled_only']} decompiled-only symbols "
            "(extras / renamed / emitted elsewhere) not counted as retail functions"
        )

    by_symbol = symbol_status_map(by_unit)
    targets = load_targets(config)
    curated = [
        t for t in targets
        if t.extra.get("origin") != "symbols.txt" and t.symbol in by_symbol
    ]
    print(
        f"\ncurated targets with a scanned symbol: {len(curated)}"
    )
    not_started = [t for t in curated if t.status == "NOT_STARTED"]
    if not_started:
        print("NOT_STARTED curated targets:")
        for t in sorted(not_started, key=lambda t: t.id):
            st = by_symbol[t.symbol]
            mark = "src" if st.in_source else "no-src"
            print(f"  {t.id:28} {mark:7} {t.function}")

    if update:
        changed = apply_to_registry(config, by_symbol, dry_run=dry_run)
        if dry_run:
            print(f"\ndry-run: {changed} registry row(s) would get source_present updated")
        else:
            print(f"\nupdated source_present on {changed} registry row(s)")
    else:
        print(
            "\nregistry untouched (pass --update to stamp source_present on "
            "matching rows; --dry-run to preview)"
        )
    return 0


def _render_target_brief(config: CoopConfig, target_id: str) -> str:
    target = get_target(load_targets(config), target_id)
    source = str(target.source.relative_to(config.project_root)) if target.source else "unresolved"
    claim = target.extra.get("claim") if isinstance(target.extra.get("claim"), dict) else {}
    allowed_paths = claim.get("allowed_paths") or ([source] if source != "unresolved" else [])
    dependencies = target.extra.get("depends_on", [])
    called_functions = target.extra.get("called_functions", [])
    questions = target.extra.get("questions", [])
    capabilities = target.extra.get("capabilities", [])
    lines = [
        "<!-- GENERATED by tools/coop/run.py targets brief. Do not edit. -->",
        "",
        f"# Worker brief: {target.id}",
        "",
        "## Objective",
        "",
        f"Reach `{target.required_level}` (or a stronger accepted result) for "
        f"`{target.function}` under the current runner policy.",
        "",
        "## Ground truth",
        "",
        f"- Region: `{target.region or config.region}`",
        f"- Symbol: `{target.symbol}`",
        f"- Address: `{target.address}`",
        f"- Unit: `{target.unit}`",
        f"- Source: `{source}`",
        f"- Milestone: `{target.milestone}`",
        f"- Current workflow/match: `{target.workflow_status}` / `{target.status}`",
        f"- Owner: `{claim.get('owner', 'unclaimed')}`",
    ]
    if capabilities:
        lines.append(f"- Capabilities: {', '.join(f'`{value}`' for value in capabilities)}")
    if dependencies:
        lines.append(f"- Dependencies: {', '.join(f'`{value}`' for value in dependencies)}")
    if target.extra.get("callgraph_status") == "complete":
        lines.append(
            "- Direct callees: "
            + (", ".join(f"`{value}`" for value in called_functions) or "none (leaf)")
        )
        if target.extra.get("has_indirect_calls"):
            lines.append("- Indirect calls: present; target is not leaf-safe")
        if target.extra.get("unresolved_called_functions"):
            lines.append(
                "- Unresolved direct calls: "
                + ", ".join(
                    f"`{value}`" for value in target.extra["unresolved_called_functions"]
                )
            )
        if target.extra.get("abi_helper_calls"):
            lines.append(
                "- Ignored MWCC ABI helpers: "
                + ", ".join(f"`{value}`" for value in target.extra["abi_helper_calls"])
            )
    if questions:
        lines.extend(["", "## Questions", ""] + [f"- {value}" for value in questions])
    lines.extend(["", "## Edit scope", ""])
    if allowed_paths:
        lines.extend([f"- Allowed: `{value}`" for value in allowed_paths])
    else:
        lines.append("- Resolve and claim an explicit source path before editing.")
    lines.extend(
        [
            "- Do not edit sibling functions or unrelated shared headers.",
            "- High-level C/C++ only; retail assembly is read-only reference.",
            "",
            "## Loop",
            "",
            "1. State one mismatch hypothesis.",
            "2. Make one bounded source change.",
            f"3. Run `python3 tools/coop/run.py cycle {target.id} --hypothesis \"...\" --next-change \"...\"`.",
            "4. Preserve the best candidate and record regressions explicitly.",
            "5. After three non-improving attempts, emit the stall packet required by the skill.",
            "",
            "## Acceptance",
            "",
            "- `cycle` exits zero at the registry's resolved required level.",
            "- Split `.text` size passes.",
            "- Equivalence result and contract are reported when that route is used.",
            "- Changes remain inside the claimed scope.",
            "- The claim is released after handoff.",
            "",
            "## Final response",
            "",
            "Report match status/percent, equivalence contract/result, size, changed files, "
            "reusable MWCC insight, claim release, and remaining risk.",
        ]
    )
    if target.notes:
        lines.extend(["", "## Existing notes", "", target.notes])
    return "\n".join(lines) + "\n"


def cmd_targets_recertify(
    project: Project,
    config: CoopConfig,
    *,
    bottom_up: bool,
    dry_run: bool,
    limit: Optional[int],
    linked: bool = False,
) -> int:
    """Issue/refresh semantic certificates for accepted targets, leaves first."""
    if not bottom_up:
        print("ERROR: targets recertify requires --bottom-up", file=sys.stderr)
        return 2

    targets = load_targets(config)
    plan = plan_recertify_bottom_up(targets)
    queue = plan.ordered if limit is None else plan.ordered[:limit]
    mode = "dry-run" if dry_run else "apply"
    print(
        f"Recertify ({mode}, bottom-up): {len(queue)} queued"
        + (f" (of {len(plan.ordered)} ordered)" if limit is not None else "")
        + f", {len(plan.blocked)} blocked"
    )
    for target in queue:
        print(f"  [queue] {target.id}: {plan.reasons.get(target.id, 'needs certificate')}")
    blocked_cap = 20 if limit is None else limit
    blocked_view = plan.blocked[:blocked_cap]
    for target in blocked_view:
        print(
            f"  [blocked] {target.id}: {plan.block_reasons.get(target.id, 'blocked')}"
            f" ({plan.reasons.get(target.id, 'needs certificate')})"
        )
    if len(plan.blocked) > len(blocked_view):
        print(f"  ... {len(plan.blocked) - len(blocked_view)} more blocked")
    if dry_run:
        if queue:
            print("Dry-run only. Re-run without --dry-run to issue/refresh certificates.")
        return 0

    succeeded = 0
    failed = 0
    attempted = 0
    failed_ids: set[str] = set()
    while limit is None or attempted < limit:
        live_targets = load_targets(config)
        wave = recertify_ready_wave(live_targets, skip_ids=failed_ids)
        if not wave:
            break
        remaining_budget = None if limit is None else max(0, limit - attempted)
        if remaining_budget is not None:
            wave = wave[:remaining_budget]
        for target in wave:
            assert target.unit is not None
            assert target.symbol is not None
            attempted += 1
            print(f"recertify: {target.id} ({target.status})")
            try:
                unit = project.resolve_unit(target.unit)
                if unit.base_path:
                    project.ninja_build(str(unit.base_path.relative_to(project.root)))
                    _postprocess_mtrand_object(project, unit.base_path)
                evaluation = evaluate_unit_match(
                    project,
                    unit,
                    target.symbol,
                    linked=linked,
                    target_id=target.id,
                )
            except (FileNotFoundError, ValueError, subprocess.CalledProcessError) as exc:
                print(f"  FAIL: {exc}", file=sys.stderr)
                failed_ids.add(target.id)
                failed += 1
                continue

            certificate = evaluation.equivalence_certificate
            if evaluation.equivalence != ProofStatus.EQUIVALENT or not certificate:
                detail = evaluation.equivalence_detail or "no certificate issued"
                status = (
                    evaluation.equivalence.value
                    if evaluation.equivalence is not None
                    else "n/a"
                )
                print(f"  FAIL: {status} ({detail})", file=sys.stderr)
                append_attempt(
                    config.resolve(config.attempt_log),
                    AttemptRecord(
                        target_id=target.id,
                        function=target.function,
                        region=config.region,
                        unit=target.unit,
                        symbol=target.symbol,
                        status=target.status,
                        instruction_match=(
                            evaluation.fn_match.match_percent
                            if evaluation.fn_match
                            else None
                        ),
                        relocation_match=None,
                        code_match_percent=evaluation.unit_report.code_match_percent,
                        data_match_percent=evaluation.unit_report.data_match_percent,
                        hypothesis="recertify: bottom-up certificate refresh failed",
                        next_change=(
                            "Inspect inconclusive certification blockers, then retry"
                        ),
                        equivalence_status=status if status != "n/a" else None,
                        equivalence_detail=detail,
                        git_commit=_git_head(project),
                    ),
                )
                failed_ids.add(target.id)
                failed += 1
                continue

            certificate = rebind_certificate_provenance(certificate)

            with exclusive_targets_lock(config):
                document = load_targets_document(config)
                rows_by_id = {
                    str(row["id"]): row
                    for row in document.get("targets", [])
                    if isinstance(row, dict) and isinstance(row.get("id"), str)
                }
                trial = dict(rows_by_id.get(target.id, {"id": target.id}))
                trial["status"] = target.status
                trial["equivalence_certificate"] = certificate
                rows_by_id[target.id] = trial
                cert_error = equivalence_certificate_error(trial, rows_by_id)
                if cert_error:
                    print(f"  FAIL: certificate rejected ({cert_error})", file=sys.stderr)
                    failed_ids.add(target.id)
                    failed += 1
                    continue

                for row in document.get("targets", []):
                    if row.get("id") != target.id:
                        continue
                    row["status"] = target.status
                    if evaluation.fn_match and evaluation.fn_match.match_percent is not None:
                        row["instruction_match"] = round(float(evaluation.fn_match.match_percent), 3)
                    if evaluation.equivalence.value:
                        row["equivalence_status"] = evaluation.equivalence.value
                    row["equivalence_certificate"] = certificate
                    if evaluation.equivalence_confidence is not None:
                        row["equivalence_confidence"] = evaluation.equivalence_confidence
                    if evaluation.equivalence_policy is not None:
                        row["equivalence_policy"] = evaluation.equivalence_policy
                    if target.status in {"FULL_MATCH", "EQUIVALENT_MATCH"}:
                        row["workflow_status"] = "ACCEPTED"
                    elif row.get("workflow_status") in {
                        None, "BACKLOG", "QUEUED", "CLAIMED", "ACCEPTED",
                    }:
                        row["workflow_status"] = "ACTIVE"
                    break
                _write_targets_document_unlocked(config, document)
            append_attempt(
                config.resolve(config.attempt_log),
                AttemptRecord(
                    target_id=target.id,
                    function=target.function,
                    region=config.region,
                    unit=target.unit,
                    symbol=target.symbol,
                    status=target.status,
                    instruction_match=(
                        evaluation.fn_match.match_percent if evaluation.fn_match else None
                    ),
                    relocation_match=None,
                    code_match_percent=evaluation.unit_report.code_match_percent,
                    data_match_percent=evaluation.unit_report.data_match_percent,
                    hypothesis="recertify: bottom-up certificate refresh",
                    next_change="",
                    equivalence_status=evaluation.equivalence.value,
                    equivalence_detail="certificate refreshed",
                    equivalence_confidence=evaluation.equivalence_confidence,
                    equivalence_policy=evaluation.equivalence_policy,
                    git_commit=_git_head(project),
                ),
            )
            print(f"  OK: certificate {certificate['certificate_sha256']}")
            succeeded += 1

    print(f"Recertify complete: {succeeded} certified, {failed} failed")
    return 1 if failed else 0


def cmd_targets_migrate_report(
    config: CoopConfig,
    *,
    write_report: Optional[Path],
    json_stdout: bool,
) -> int:
    report = equivalence_certificate_migration_report(config)
    if json_stdout:
        print(json.dumps(report, indent=2))
    else:
        print(
            "Migration report: "
            f"{report['valid_count']} valid, "
            f"{report['stale_count']} stale, "
            f"{report['no_certificate_count']} no-certificate, "
            f"{report['skipped_full_match_count']} FULL_MATCH skipped"
        )
        if report["skipped_full_match_stale_cert_count"]:
            print(
                f"  ({report['skipped_full_match_stale_cert_count']} FULL_MATCH "
                "retain stale historical certificates)"
            )
        for entry in report["no_certificate"]:
            print(f"  [no-certificate] {entry['id']}")
        for entry in report["stale"]:
            print(f"  [stale] {entry['id']}: {entry['certificate_error']}")

    if write_report:
        path = write_report if write_report.is_absolute() else config.project_root / write_report
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
        if not json_stdout:
            print(f"Report written to {path}")

    return 1 if report["affected_count"] else 0


def cmd_targets_audit_promotion(
    config: CoopConfig,
    *,
    apply: bool,
    write_report: Optional[Path],
) -> int:
    report = audit_promotion_registry(config, apply=apply)
    mode = "apply" if apply else "dry-run"
    print(
        f"Audit complete ({mode}): {report['valid_count']} valid, "
        f"{report['affected_count']} affected, "
        f"{report['skipped_full_match']} FULL_MATCH skipped"
    )
    for entry in report["affected"]:
        print(f"  [{entry['action']}] {entry['id']}: {entry['certificate_error']}")
    if apply and report["mutations"]:
        print(f"Applied {report['applied_count']} revalidation marking(s)")
        for mutation in report["mutations"]:
            print(
                f"  {mutation['id']}: {mutation['from_status']}/{mutation['from_workflow']} "
                f"-> {mutation['to_status']}/{mutation['to_workflow']}"
            )
    elif not apply and report["affected_count"]:
        print("Dry-run only. Re-run with --apply to mark targets for revalidation.")

    if write_report:
        path = write_report if write_report.is_absolute() else config.project_root / write_report
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(json.dumps(report, indent=2), encoding="utf-8")
        print(f"Report written to {path}")

    return 1 if report["affected_count"] else 0


def cmd_targets_brief(
    config: CoopConfig, target_id: str, *, output: Optional[Path]
) -> int:
    rendered = _render_target_brief(config, target_id)
    if output:
        path = output if output.is_absolute() else config.project_root / output
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(rendered, encoding="utf-8")
        print(path)
    else:
        print(rendered, end="")
    return 0


def cmd_log(config: CoopConfig, tail: Optional[int], target_id: Optional[str]) -> int:
    rows = read_attempts(config.resolve(config.attempt_log), tail=tail)
    if target_id:
        rows = [r for r in rows if r.get("target_id") == target_id]
    for row in rows:
        print(row)
    return 0


def cmd_symbols(symrecover_args: list[str]) -> int:
    if symrecover_args and symrecover_args[0] == "--":
        symrecover_args = symrecover_args[1:]
    if not symrecover_args:
        symrecover_args = ["--help"]
    old_argv = sys.argv
    try:
        sys.argv = ["symrecover", *symrecover_args]
        from tools.symrecover import main as symrecover_main

        return symrecover_main()
    finally:
        sys.argv = old_argv


def cmd_behaviour(behaviour_args: list[str]) -> int:
    if behaviour_args and behaviour_args[0] == "--":
        behaviour_args = behaviour_args[1:]
    script = ROOT / "tools" / "test" / "compare_behaviour" / "run.py"
    cmd = [sys.executable, str(script), *behaviour_args]
    return subprocess.run(cmd, cwd=ROOT, check=False).returncode


def _equivalence_args_with_default_contract(equivalence_args: list[str]) -> list[str]:
    args = list(equivalence_args)
    if not args or args[0] not in {"check", "check-hex", "check-objects"}:
        return args
    has_contract = any(
        arg in {"--contract", "--observe"}
        or arg.startswith("--contract=")
        or arg.startswith("--observe=")
        for arg in args[1:]
    )
    if not has_contract:
        default_contract = "auto" if args[0] == "check-objects" else "ppc-eabi"
        args[1:1] = ["--contract", default_contract]
    return args


def _cmd_equivalence_check_unit(project: Project, config: CoopConfig, unit_args: list[str]) -> int:
    """Resolve an objdiff unit pair, extract one symbol, and run check-objects."""
    parser = argparse.ArgumentParser(
        prog="equivalence check-unit",
        description="SMT-check one function from an objdiff retail/decomp object pair",
    )
    parser.add_argument("unit", help="objdiff unit hint or source path")
    parser.add_argument(
        "--symbol",
        required=True,
        help="function symbol (mangled or unique substring / demangled token)",
    )
    parser.add_argument(
        "--candidate-symbol",
        help="candidate symbol when names differ (default: resolved --symbol)",
    )
    parser.add_argument(
        "--no-build",
        action="store_true",
        help="do not ninja-build / post-process the decomp object first",
    )
    parser.add_argument(
        "--linked",
        action="store_true",
        help=(
            "When the unlinked .o pair has unresolved relocations, retry the "
            "proof with linked bytes from main.dol (retail) and main.elf "
            "(candidate). Requires `ninja build/<region>/main.elf` for the "
            "candidate side."
        ),
    )
    parser.add_argument(
        "--declared-return",
        choices=["void","i32","u32","bool","ptr","f32","f64","i64","u64","aggregate","f128"],
        help="Source-level return-type class for narrowing AbiShape",
    )
    parser.add_argument(
        "--force-declared-return",
        action="store_true",
        help="Override the §2.8 caller-corroboration gate and force narrowing",
    )
    parsed, rest = parser.parse_known_args(unit_args)

    unit = project.resolve_unit(parsed.unit)
    retail, decomp = _object_paths_for_unit(project, unit)
    if retail is None or not retail.is_file():
        print(f"ERROR: retail object missing for unit {unit.name}: {retail}", file=sys.stderr)
        return 3
    if decomp is None:
        print(f"ERROR: unit has no compiled base path: {unit.name}", file=sys.stderr)
        return 3

    if not parsed.no_build:
        project.ninja_build(str(decomp.relative_to(project.root)))
        _postprocess_mtrand_object(project, decomp)
    if not decomp.is_file():
        print(f"ERROR: decomp object missing for unit {unit.name}: {decomp}", file=sys.stderr)
        return 3

    symbol = parsed.symbol
    # Prefer the mangled name from the objdiff report when the user passed a demangled hint.
    try:
        fn_match = find_function_match(report_unit(project, unit), parsed.symbol)
        if fn_match is not None:
            symbol = fn_match.name
            print(
                f"unit: {unit.name}  symbol: {fn_match.name}  "
                f"objdiff fuzzy: {fn_match.match_percent:.1f}%",
                flush=True,
            )
        else:
            print(f"unit: {unit.name}  symbol: {symbol}", flush=True)
    except (FileNotFoundError, ValueError, subprocess.CalledProcessError) as exc:
        print(f"unit: {unit.name}  symbol: {symbol}", flush=True)
        print(f"warning: objdiff report unavailable ({exc})", file=sys.stderr)

    if parsed.linked:
        return _run_check_unit_linked(
            project, config, unit, symbol, parsed.candidate_symbol, rest,
            declared_return=parsed.declared_return,
            force_declared_return=parsed.force_declared_return,
        )

    # When --declared-return or --force-declared-return is set, bypass the
    # upstream CLI (which doesn't support these flags) and call prove_unit_symbol directly.
    if parsed.declared_return is not None or parsed.force_declared_return:
        return _run_check_unit_direct(
            project, unit, symbol, parsed.candidate_symbol, rest,
            declared_return=parsed.declared_return,
            force_declared_return=parsed.force_declared_return,
        )

    forwarded = [
        "check-objects",
        "--original",
        str(retail),
        "--candidate",
        str(decomp),
        "--symbol",
        symbol,
    ]
    if parsed.candidate_symbol:
        forwarded.extend(["--candidate-symbol", parsed.candidate_symbol])
    forwarded.extend(rest)
    return cmd_equivalence(project, config, forwarded)


def _run_check_unit_direct(
    project: Project,
    unit: ObjdiffUnit,
    symbol: str,
    candidate_symbol: str | None,
    rest: list[str],
    *,
    declared_return: str | None = None,
    force_declared_return: bool = False,
) -> int:
    """Run prove_unit_symbol directly (bypasses ppc_equivalence CLI).

    Used when --declared-return or --force-declared-return is set, since
    the upstream CLI does not support these flags.

    Resolves the registry target by symbol so the §2.8 caller-corroboration
    gate runs. If --declared-return is given but no registry target exists
    for the symbol, errors unless --force-declared-return is also given.
    """
    from tools.coop.lib.equivalence_check import prove_unit_symbol
    from tools.ppc_equivalence.result import ProofStatus

    contract = "auto"
    rest_iter = iter(rest)
    for arg in rest_iter:
        if arg == "--contract":
            contract = next(rest_iter, contract)
        elif arg.startswith("--contract="):
            contract = arg.split("=", 1)[1]
        elif arg == "--observe":
            print(
                "ERROR: --observe is not supported in direct mode "
                "(prove_unit_symbol uses contract-based observables)",
                file=sys.stderr,
            )
            return 3
        elif arg.startswith("--observe="):
            print(
                "ERROR: --observe is not supported in direct mode "
                "(prove_unit_symbol uses contract-based observables)",
                file=sys.stderr,
            )
            return 3
        elif arg.startswith("--"):
            print(f"ERROR: unsupported flag {arg!r} in direct mode", file=sys.stderr)
            return 3

    # Resolve registry target by symbol so the §2.8 gate can run.
    target_id: str | None = None
    if declared_return is not None or force_declared_return:
        try:
            from tools.coop.lib.targets import load_targets
            from tools.coop.lib.config import load_config

            config = load_config(None, project.root)
            all_targets = load_targets(config)
            matching = [t for t in all_targets if t.symbol == symbol]
            if len(matching) == 1:
                target_id = matching[0].id
            elif len(matching) > 1:
                print(
                    f"ERROR: --declared-return given but {len(matching)} registry targets "
                    f"match symbol {symbol!r}; refusing ambiguous resolution",
                    file=sys.stderr,
                )
                return 3
            else:
                # No registry target for this symbol.
                if declared_return is not None and not force_declared_return:
                    print(
                        f"ERROR: --declared-return given but no registry target exists for "
                        f"symbol {symbol!r}; use --force-declared-return to proceed without "
                        f"registry corroboration",
                        file=sys.stderr,
                    )
                    return 3
                # With --force-declared-return alone, proceed without a target_id.
        except Exception as exc:
            print(
                f"WARNING: could not load registry for target resolution: {exc}",
                file=sys.stderr,
            )

    probe = prove_unit_symbol(
        project, unit, symbol,
        contract=contract,
        candidate_symbol=candidate_symbol,
        target_id=target_id,
        declared_return=declared_return,
        force_declared_return=force_declared_return,
    )
    status = probe.status
    label = {
        ProofStatus.EQUIVALENT: "EQUIVALENT",
        ProofStatus.NOT_EQUIVALENT: "NOT_EQUIVALENT",
        ProofStatus.INCONCLUSIVE_UNSUPPORTED: "INCONCLUSIVE_UNSUPPORTED",
        ProofStatus.INVALID_INPUT: "INVALID_INPUT",
        ProofStatus.INTERNAL_ERROR: "INTERNAL_ERROR",
    }.get(status, status.value)
    print(f"check-unit: {label}")
    if probe.detail:
        print(f"  detail: {probe.detail}")

    if status == ProofStatus.EQUIVALENT:
        return 0
    if status == ProofStatus.NOT_EQUIVALENT:
        return 1
    if status == ProofStatus.INVALID_INPUT:
        return 3
    return 2


def _run_check_unit_linked(
    project: Project,
    config: CoopConfig,
    unit: ObjdiffUnit,
    symbol: str,
    candidate_symbol: Optional[str],
    rest: list[str],
    *,
    declared_return: Optional[str] = None,
    force_declared_return: bool = False,
) -> int:
    """`check-unit --linked`: invoke prove_unit_symbol directly and print the result.

    The library fallback path can retry the proof with linked bytes from
    main.dol + main.elf when the unlinked `.o` pair carries unresolved
    relocations. Passthrough ``rest`` args (`--contract`, `--observe`, ...) are
    honored for `--contract`; others are ignored in linked mode.
    """
    from tools.coop.lib.equivalence_check import prove_unit_symbol
    from tools.ppc_equivalence.result import ProofStatus

    contract = "auto"
    rest_iter = iter(rest)
    for arg in rest_iter:
        if arg == "--contract":
            contract = next(rest_iter, contract)
        elif arg.startswith("--contract="):
            contract = arg.split("=", 1)[1]
        elif arg.startswith("--observe"):
            print(
                "ERROR: --observe is not supported in linked mode "
                "(prove_unit_symbol uses contract-based observables)",
                file=sys.stderr,
            )
            return 3
        elif arg.startswith("--"):
            print(f"ERROR: unsupported flag {arg!r} in linked mode", file=sys.stderr)
            return 3

    probe = prove_unit_symbol(
        project, unit, symbol,
        contract=contract,
        candidate_symbol=candidate_symbol,
        linked=True,
        declared_return=declared_return,
        force_declared_return=force_declared_return,
    )
    status = probe.status
    label = {
        ProofStatus.EQUIVALENT: "EQUIVALENT",
        ProofStatus.NOT_EQUIVALENT: "NOT_EQUIVALENT",
        ProofStatus.INCONCLUSIVE_UNSUPPORTED: "INCONCLUSIVE_UNSUPPORTED",
        ProofStatus.INVALID_INPUT: "INVALID_INPUT",
        ProofStatus.INTERNAL_ERROR: "INTERNAL_ERROR",
    }.get(status, status.value)
    print(f"check-unit --linked: {label}")
    if probe.detail:
        print(f"  detail: {probe.detail}")

    if status == ProofStatus.EQUIVALENT:
        return 0
    if status == ProofStatus.NOT_EQUIVALENT:
        return 1
    if status == ProofStatus.INVALID_INPUT:
        return 3
    return 2


def cmd_equivalence(project: Project, config: CoopConfig, equivalence_args: list[str]) -> int:
    if equivalence_args and equivalence_args[0] == "--":
        equivalence_args = equivalence_args[1:]
    if equivalence_args and equivalence_args[0] == "audit-registry":
        parser = argparse.ArgumentParser(prog="equivalence audit-registry")
        parser.add_argument(
            "--apply",
            action="store_true",
            help="Mark affected EQUIVALENT_MATCH rows for revalidation",
        )
        parser.add_argument(
            "--dry-run",
            action="store_true",
            help="Report only (default when --apply is omitted)",
        )
        parser.add_argument("--write-report", type=Path)
        parsed = parser.parse_args(equivalence_args[1:])
        return cmd_targets_audit_promotion(
            config,
            apply=bool(parsed.apply) and not parsed.dry_run,
            write_report=parsed.write_report,
        )
    if equivalence_args and equivalence_args[0] == "check-unit":
        return _cmd_equivalence_check_unit(project, config, equivalence_args[1:])
    equivalence_args = _equivalence_args_with_default_contract(equivalence_args)
    script = ROOT / "tools" / "ppc_equivalence" / "run.py"
    cmd = [sys.executable, str(script), *equivalence_args]
    return subprocess.run(cmd, cwd=ROOT, check=False).returncode


def cmd_opcodes(opcodes_args: list[str], config: CoopConfig) -> int:
    if opcodes_args and opcodes_args[0] == "--":
        opcodes_args = opcodes_args[1:]
    # Default to this region's main.dol when the caller did not pass a path.
    if not opcodes_args or opcodes_args[0].startswith("-"):
        opcodes_args = [str(config.main_dol), *opcodes_args]
    cmd = [sys.executable, "-m", "tools.dol_opcodes", *opcodes_args]
    return subprocess.run(cmd, cwd=ROOT, check=False).returncode


def main() -> int:
    parser = argparse.ArgumentParser(description="Xenoblade co-op decompilation runner")
    parser.add_argument(
        "--config",
        type=Path,
        help="Path to coop.json or coop.yaml (default: ./coop.json)",
    )
    sub = parser.add_subparsers(dest="command", required=True)

    sub.add_parser("status", help="Show runner and project health")
    sub.add_parser("baseline", help="Verify main.dol, configure, and full ninja build")
    sub.add_parser("configure", help="Run python3 configure.py for the selected region")

    p_progress = sub.add_parser(
        "progress", help="Build report and print decompilation progress (no linking required)",
    )
    p_progress.add_argument(
        "--fancy", action="store_true", default=None,
        help="Force fancy progress output (achievements/collectopaedias)",
    )
    p_progress.add_argument(
        "--no-fancy", action="store_false", dest="fancy", default=None,
        help="Suppress fancy progress output",
    )

    p_ctx = sub.add_parser("ctx", help="Generate decomp.me context for a source file")
    p_ctx.add_argument("source", type=Path)
    p_ctx.add_argument("-o", "--output", type=Path)

    p_build = sub.add_parser("build", help="Build one translation unit")
    p_build.add_argument("unit", help="objdiff unit hint or source path")

    p_diff = sub.add_parser("diff", help="Build and diff one translation unit")
    p_diff.add_argument("unit", help="objdiff unit hint or source path")
    p_diff.add_argument("--symbol", help="Function symbol for per-function stats")
    p_diff.add_argument(
        "--linked",
        action="store_true",
        help=(
            "When the SMT equivalence probe would otherwise be inconclusive due "
            "to unresolved relocations, retry with linked bytes from main.dol + "
            "main.elf (build the latter with `ninja build/<region>/main.elf`)."
        ),
    )
    p_diff.add_argument(
        "--no-smt",
        action="store_true",
        help=(
            "Skip the SMT equivalence probe (register-renaming witness still "
            "runs). Use to avoid Z3 cost while iterating; pass without the flag "
            "for the full probe when stuck or near acceptance."
        ),
    )
    p_diff.add_argument(
        "--no-witness",
        action="store_true",
        help=(
            "Disable the register-renaming witness probe entirely (witness "
            "opt-out): only byte-identical FULL_MATCH can be accepted; a "
            "non-byte-identical function reports inconclusive_smt_disabled. "
            "Default keeps the witness enabled (human CLI unchanged)."
        ),
    )

    p_size = sub.add_parser("size", help="Check decomp .text size against split budget")
    p_size.add_argument("unit", nargs="?", help="objdiff unit hint or source path")
    p_size.add_argument("--all", action="store_true", help="Check every buildable objdiff unit")

    p_data = sub.add_parser("data", help="Data-section matching for data-only / mixed TUs")
    p_data_sub = p_data.add_subparsers(dest="data_cmd", required=True)
    p_data_diff = p_data_sub.add_parser(
        "diff",
        help="Compare decompiled vs retail data sections (raw first; on failure, applies the §17.6 reloc-name postprocess to a temp copy when the unit has rules)",
    )
    p_data_diff.add_argument("unit", nargs="?", help="objdiff unit hint or source path")
    p_data_diff.add_argument("--all", action="store_true", help="Check every objdiff unit with data sections")
    p_data_diff.add_argument(
        "--no-postprocess",
        action="store_true",
        help="compare the raw decompiled object only (default: raw first, then §17.6 reloc-name rules on a temp copy when the raw comparison fails)",
    )

    p_reloc = sub.add_parser(
        "reloc-map",
        help="Reloc name-drift detection + map miner (tools/coop/reloc_map.py, MWCC_REFERENCE §1)",
    )
    p_reloc_sub = p_reloc.add_subparsers(dest="reloc_cmd", required=True)
    p_reloc_diff = p_reloc_sub.add_parser("diff", help="Per-function reloc drift + approved source fixes")
    p_reloc_diff.add_argument("unit", help="objdiff unit hint or source path")
    p_reloc_diff.add_argument("-s", "--symbol", required=True, help="function symbol (mangled name or unique substring)")
    p_reloc_diff.add_argument("--no-build", action="store_true", help="do not build the decomp object first")
    p_reloc_diff.add_argument("--json", action="store_true")
    p_reloc_mine = p_reloc_sub.add_parser("mine", help="Batch-mine the named-symbol map across all objdiff units")
    p_reloc_mine.add_argument("--out", default=None, help="output map path (default: tools/coop/retail_reloc_map.json)")
    p_reloc_mine.add_argument("--all-kinds", action="store_true", help="include call/branch reloc kinds (default: data only)")
    p_reloc_mine.add_argument("--dry-run", action="store_true")
    p_reloc_mine.add_argument("--json", action="store_true")
    p_reloc_show = p_reloc_sub.add_parser("show", help="Pretty-print the mined reloc map")
    p_reloc_show.add_argument("--symbol", default=None, help="filter by symbol substring")
    p_reloc_show.add_argument("--limit", type=int, default=None)
    p_reloc_show.add_argument("--verbose", action="store_true")
    p_reloc_show.add_argument("--unit-scoped", action="store_true", default=None, help="only TU-local entries")
    p_reloc_show.add_argument("--global-only", dest="unit_scoped", action="store_false", help="only named (global) entries")
    p_reloc_fresh = p_reloc_sub.add_parser("ensure-fresh", help="Re-mine the reloc map iff stale (doc 33 Item 0.5)")
    p_reloc_fresh.add_argument("--out", default=None, help="map path (default: tools/coop/retail_reloc_map.json)")
    p_reloc_fresh.add_argument("--force", action="store_true", help="re-mine unconditionally")

    p_cycle = sub.add_parser("cycle", help="ctx + build + diff + JSONL log for one target id")
    p_cycle.add_argument("target_id")
    p_cycle.add_argument("--hypothesis", default="")
    p_cycle.add_argument("--next-change", default="")
    p_cycle.add_argument("--runtime-test", default="")
    p_cycle.add_argument("--add-to-kb", default="",
                         help="JSON payload for a reusable kb contribution to append to docs/mwcc/contributions.jsonl")
    p_cycle.add_argument(
        "--linked",
        action="store_true",
        help=(
            "Allow the SMT equivalence probe to fall back to linked DOL/ELF bytes "
            "when the unlinked .o pair has unresolved relocations."
        ),
    )
    p_cycle.add_argument(
        "--smt",
        action="store_true",
        help=(
            "Run the full SMT equivalence probe on this cycle. Opt-in: the "
            "default runs only the cheap register-renaming witness. Required to "
            "reach EQUIVALENT_MATCH acceptance for functions the witness cannot "
            "certify."
        ),
    )
    p_cycle.add_argument(
        "--contract",
        default="auto",
        choices=["auto", "ppc-eabi", "ppc-eabi-fp", "strict", "live-out", "memory"],
        help=(
            "SMT equivalence contract preset (default: auto). "
            "Use 'memory' to check only memory writes (void functions with register allocation differences). "
            "Use live-out to check only live registers, or ppc-eabi for full ABI."
        ),
    )
    p_cycle.add_argument(
        "--witness-timeout",
        type=int,
        default=0,
        metavar="MS",
        help=(
            "Hard timeout (milliseconds) for the register-renaming witness's z3 "
            "rewriter calls (0 = z3 default, no timeout). The harness passes a "
            "bounded value so a pathological z3.simplify on a huge terminal AST "
            "is interrupted instead of spinning (run30 incident: one lane held "
            "the build lock ~30 min at 99.7% CPU)."
        ),
    )
    p_cycle.add_argument(
        "--no-witness",
        action="store_true",
        help=(
            "Disable the register-renaming witness probe entirely: only "
            "byte-identical FULL_MATCH (with reloc-site equality) can be "
            "accepted. A non-byte-identical function reports "
            "inconclusive_smt_disabled and never reaches EQUIVALENT_MATCH."
        ),
    )

    def add_harness_args(command_parser: argparse.ArgumentParser) -> None:
        command_parser.add_argument(
            "--selection",
            choices=["pending", "leaf", "callees-accepted", "ready"],
            default="pending",
            help=(
                "pending=normal queue; leaf=no direct/indirect/unresolved calls; "
                "callees-accepted=non-leaf with every known callee accepted and "
                "semantically certified; ready=union of leaf and callees-accepted"
            ),
        )
        command_parser.add_argument("--limit", type=int)
        command_parser.add_argument("--dry-run", action="store_true")

    p_queue = sub.add_parser("queue", help="Run cycle on a selected target frontier")
    add_harness_args(p_queue)
    p_harness = sub.add_parser(
        "harness", help="Run bottom-up cycle harness selections from the call graph"
    )
    add_harness_args(p_harness)

    p_targets = sub.add_parser("targets", help="Inspect and maintain the canonical target registry")
    p_targets_sub = p_targets.add_subparsers(dest="targets_cmd", required=True)
    p_targets_list = p_targets_sub.add_parser("list")
    p_targets_list.add_argument("--milestone")
    p_targets_list.add_argument("--workflow-status")
    p_targets_list.add_argument("--match-status")
    p_targets_list.add_argument("--kind")
    p_targets_show = p_targets_sub.add_parser("show")
    p_targets_show.add_argument("target_id")
    p_targets_status = p_targets_sub.add_parser(
        "status", help="Render a generated human-readable or JSON status view"
    )
    p_targets_status.add_argument("--milestone")
    p_targets_status.add_argument("--kind", default="function")
    p_targets_status.add_argument("--format", choices=["markdown", "json"], default="markdown")
    p_targets_status.add_argument("--output", type=Path)
    p_targets_sub.add_parser("validate", help="Validate registry identities and status vocabularies")
    p_targets_sub.add_parser(
        "sync-attempts",
        help="Migrate each target's latest attempts.jsonl result into current registry state",
    )
    p_targets_calls = p_targets_sub.add_parser(
        "sync-calls", help="Populate called_functions from generated retail assembly"
    )
    p_targets_calls.add_argument("--dry-run", action="store_true")
    p_targets_syms = p_targets_sub.add_parser(
        "sync-symbols",
        help="Re-sync imported registry symbol names from the current symbols.txt",
    )
    p_targets_syms.add_argument("--dry-run", action="store_true")
    p_targets_dedupe = p_targets_sub.add_parser(
        "dedupe",
        help="Drop duplicate address rows, keeping the canonical base row",
    )
    p_targets_dedupe.add_argument("--dry-run", action="store_true")
    p_targets_claim = p_targets_sub.add_parser(
        "claim", help="Record the current owner and exclusive edit scope in the registry"
    )
    p_targets_claim.add_argument("target_id", nargs="+", help="one or more target ids (batched in one lock hold + one registry write)")
    p_targets_claim.add_argument("--owner", required=True)
    p_targets_claim.add_argument("--allowed-path", action="append", default=[])
    p_targets_claim.add_argument("--note", default="")
    p_targets_release = p_targets_sub.add_parser("release", help="Release a current claim")
    p_targets_release.add_argument("target_id", nargs="+", help="one or more target ids (batched)")
    p_targets_release.add_argument("--owner")
    p_targets_audit = p_targets_sub.add_parser(
        "audit-promotion",
        help="Audit EQUIVALENT_MATCH targets for promotion eligibility",
    )
    p_targets_audit.add_argument(
        "--apply",
        action="store_true",
        help="Mark affected rows as CODE_MATCH / REVALIDATION_REQUIRED",
    )
    p_targets_audit.add_argument(
        "--dry-run",
        action="store_true",
        help="Report only (default when --apply is omitted)",
    )
    p_targets_audit.add_argument(
        "--write-report", type=Path,
        help="Write audit JSON report to path",
    )
    p_targets_migrate = p_targets_sub.add_parser(
        "migrate-report",
        help=(
            "Enumerate EQUIVALENT_MATCH rows whose equivalence_certificate is "
            "stale relative to live architecture/result-format/certificate version "
            "or rejected architecture models (read-only; does not mutate registry)"
        ),
    )
    p_targets_migrate.add_argument(
        "--write-report",
        type=Path,
        help="Write migration JSON report to path",
    )
    p_targets_migrate.add_argument(
        "--json",
        action="store_true",
        help="Print machine-readable JSON summary to stdout",
    )
    p_targets_recertify = p_targets_sub.add_parser(
        "recertify",
        help=(
            "Issue or refresh semantic certificates for accepted targets "
            "in bottom-up call-graph order"
        ),
    )
    p_targets_recertify.add_argument(
        "--bottom-up",
        action="store_true",
        help="Process leaves and certified-callee frontiers before dependents (required)",
    )
    p_targets_recertify.add_argument(
        "--dry-run",
        action="store_true",
        help="Print the bottom-up queue without building or certifying",
    )
    p_targets_recertify.add_argument(
        "--limit",
        type=int,
        help="Maximum number of targets to certify (apply) or list (dry-run)",
    )
    p_targets_recertify.add_argument(
        "--linked",
        action="store_true",
        help=(
            "Allow SMT prove fallback to linked DOL/ELF bytes when unlinked "
            "objects are inconclusive due to relocations"
        ),
    )
    p_targets_claim_smallest = p_targets_sub.add_parser(
        "claim-smallest",
        help="Claim and print the smallest NOT_STARTED function(s)",
    )
    p_targets_claim_smallest.add_argument(
        "--num", type=int, default=1,
        help="Number of smallest targets to claim (default: 1)",
    )
    p_targets_claim_smallest.add_argument("--owner", default="",
        help="Owner name for claiming (skips claim if empty)",
    )
    p_targets_claim_smallest.add_argument("--no-claim", action="store_true",
        help="Just list the smallest targets without claiming",
    )

    p_targets_brief = p_targets_sub.add_parser(
        "brief", help="Generate a synchronized worker prompt for one target"
    )
    p_targets_brief.add_argument("target_id")
    p_targets_brief.add_argument("--output", type=Path)
    p_targets_import = p_targets_sub.add_parser(
        "import-symbols",
        help="Idempotently add symbols from config/<region>/symbols.txt",
    )
    p_targets_import.add_argument(
        "--kind",
        choices=["function", "object", "label", "all"],
        default="function",
        help="Import every function by default; use 'all' for every game symbol",
    )
    p_targets_import.add_argument("--dry-run", action="store_true")
    p_targets_scan = p_targets_sub.add_parser(
        "scan-source",
        help=(
            "Scan the whole binary for which retail functions have a decompiled "
            "implementation in source (no build; parses existing objects)"
        ),
    )
    p_targets_scan.add_argument(
        "--update",
        action="store_true",
        help="Stamp source_present onto every matching registry row",
    )
    p_targets_scan.add_argument(
        "--dry-run",
        action="store_true",
        help="With --update: report what would change without writing",
    )

    p_log = sub.add_parser("log", help="Show JSONL attempt log")
    p_log.add_argument("--tail", type=int)
    p_log.add_argument("--target")

    p_symbols = sub.add_parser(
        "symbols",
        help="Symbol recovery helpers (wraps tools/symrecover.py)",
    )
    p_symbols.add_argument(
        "symrecover_args",
        nargs=argparse.REMAINDER,
        help="symrecover subcommand and flags (e.g. list, show 8043C59C)",
    )

    p_behaviour = sub.add_parser(
        "behaviour",
        help="Retail vs decomp behaviour tests (tools/test/compare_behaviour)",
    )
    p_behaviour.add_argument(
        "behaviour_args",
        nargs=argparse.REMAINDER,
        help="compare_behaviour subcommand (audit, compare --all, compare <id>, …)",
    )

    p_equivalence = sub.add_parser(
        "equivalence",
        help="SMT equivalence check for supported Broadway PPC32 blocks",
    )
    p_equivalence.add_argument(
        "equivalence_args",
        nargs=argparse.REMAINDER,
        help="equivalence subcommand (decode, check-hex, check, check-objects, check-unit, extract, replay, differential)",
    )

    p_extc = sub.add_parser(
        "extc",
        help="extern \"C\" declaration classification + member-conversion planning",
    )
    p_extc.add_argument(
        "extc_args",
        nargs=argparse.REMAINDER,
        help="extc subcommand and flags (scan, plan <class>, --json PATH)",
    )

    p_opcodes = sub.add_parser(
        "opcodes",
        help="List PowerPC opcodes used in main.dol (wraps tools.dol_opcodes)",
    )
    p_opcodes.add_argument(
        "opcodes_args",
        nargs=argparse.REMAINDER,
        help="optional DOL path and flags (e.g. --sort count, --names-only, --json)",
    )

    args = parser.parse_args()
    config = load_config(args.config, ROOT)
    project = Project(config)

    if args.command == "status":
        return cmd_status(project, config)
    if args.command == "baseline":
        return cmd_baseline(project, config)
    if args.command == "configure":
        return cmd_configure(project, config)
    if args.command == "progress":
        return cmd_progress(project, config, args.fancy)
    if args.command == "ctx":
        return cmd_ctx(project, args.source, args.output)
    if args.command == "build":
        return cmd_build(project, args.unit)
    if args.command == "diff":
        return cmd_diff(
            project, config, args.unit, args.symbol,
            write_function_diff=False, linked=args.linked, smt=not args.no_smt,
            witness_enabled=not args.no_witness,
        )
    if args.command == "size":
        return cmd_size(project, config, args.unit, check_all=args.all)
    if args.command == "data" and args.data_cmd == "diff":
        return _cmd_data_diff(project, config, args.unit, check_all=args.all, postprocess=not args.no_postprocess)
    if args.command == "reloc-map":
        from tools.coop.reloc_map import main as reloc_map_main

        # Forward the subcommand (diff/mine/show) and its args as argv.
        argv = [args.reloc_cmd]
        if args.reloc_cmd == "diff":
            argv += [args.unit, "--symbol", args.symbol]
            if args.no_build:
                argv.append("--no-build")
            if args.json:
                argv.append("--json")
        elif args.reloc_cmd == "mine":
            if args.out:
                argv += ["--out", args.out]
            if args.all_kinds:
                argv.append("--all-kinds")
            if args.dry_run:
                argv.append("--dry-run")
            if args.json:
                argv.append("--json")
        elif args.reloc_cmd == "show":
            if args.symbol:
                argv += ["--symbol", args.symbol]
            if args.limit:
                argv += ["--limit", str(args.limit)]
            if args.verbose:
                argv.append("--verbose")
            if args.unit_scoped is True:
                argv.append("--unit-scoped")
            if args.unit_scoped is False:
                argv.append("--global-only")
        elif args.reloc_cmd == "ensure-fresh":
            if args.out:
                argv += ["--out", args.out]
            if args.force:
                argv.append("--force")
        return reloc_map_main(argv)
    if args.command == "cycle":
        return cmd_cycle(
            project,
            config,
            args.target_id,
            hypothesis=args.hypothesis,
            next_change=args.next_change,
            runtime_test=args.runtime_test,
            linked=args.linked,
            add_to_kb=args.add_to_kb,
            contract=args.contract,
            smt=args.smt,
            witness_timeout_ms=args.witness_timeout,
            witness_enabled=not args.no_witness,
        )
    if args.command == "queue":
        return cmd_queue(
            project,
            config,
            dry_run=args.dry_run,
            selection=args.selection,
            limit=args.limit,
        )
    if args.command == "harness":
        return cmd_queue(
            project,
            config,
            dry_run=args.dry_run,
            selection=args.selection,
            limit=args.limit,
        )
    if args.command == "targets" and args.targets_cmd == "list":
        return cmd_targets_list(
            config,
            args.milestone,
            args.workflow_status,
            args.match_status,
            args.kind,
        )
    if args.command == "targets" and args.targets_cmd == "show":
        return cmd_targets_show(config, args.target_id)
    if args.command == "targets" and args.targets_cmd == "status":
        return cmd_targets_status(
            config,
            milestone=args.milestone,
            kind=args.kind,
            output=args.output,
            output_format=args.format,
        )
    if args.command == "targets" and args.targets_cmd == "validate":
        return cmd_targets_validate(config)
    if args.command == "targets" and args.targets_cmd == "sync-attempts":
        return cmd_targets_sync_attempts(config)
    if args.command == "targets" and args.targets_cmd == "sync-calls":
        return cmd_targets_sync_calls(project, config, dry_run=args.dry_run)
    if args.command == "targets" and args.targets_cmd == "sync-symbols":
        return cmd_targets_sync_symbols(project, config, dry_run=args.dry_run)
    if args.command == "targets" and args.targets_cmd == "dedupe":
        return cmd_targets_dedupe(config, dry_run=args.dry_run)
    if args.command == "targets" and args.targets_cmd == "claim":
        if len(args.target_id) == 1:
            return cmd_targets_claim(
                config,
                args.target_id[0],
                owner=args.owner,
                allowed_paths=args.allowed_path,
                note=args.note,
            )
        return cmd_targets_claim_batch(
            config,
            args.target_id,
            owner=args.owner,
            allowed_paths=args.allowed_path,
            note=args.note,
        )
    if args.command == "targets" and args.targets_cmd == "release":
        if len(args.target_id) == 1:
            return cmd_targets_release(config, args.target_id[0], owner=args.owner)
        return cmd_targets_release_batch(config, args.target_id, owner=args.owner)
    if args.command == "targets" and args.targets_cmd == "audit-promotion":
        return cmd_targets_audit_promotion(
            config,
            apply=bool(args.apply) and not args.dry_run,
            write_report=args.write_report,
        )
    if args.command == "targets" and args.targets_cmd == "migrate-report":
        return cmd_targets_migrate_report(
            config,
            write_report=args.write_report,
            json_stdout=bool(args.json),
        )
    if args.command == "targets" and args.targets_cmd == "recertify":
        return cmd_targets_recertify(
            project,
            config,
            bottom_up=bool(args.bottom_up),
            dry_run=bool(args.dry_run),
            limit=args.limit,
            linked=bool(args.linked),
        )
    if args.command == "targets" and args.targets_cmd == "claim-smallest":
        return cmd_targets_claim_smallest(
            config,
            num=args.num,
            owner=args.owner,
            no_claim=args.no_claim,
        )
    if args.command == "targets" and args.targets_cmd == "brief":
        return cmd_targets_brief(config, args.target_id, output=args.output)
    if args.command == "targets" and args.targets_cmd == "import-symbols":
        return cmd_targets_import_symbols(
            project, config, kind=args.kind, dry_run=args.dry_run
        )
    if args.command == "targets" and args.targets_cmd == "scan-source":
        return cmd_targets_scan_source(
            project,
            config,
            update=bool(args.update),
            dry_run=bool(args.dry_run),
        )
    if args.command == "log":
        return cmd_log(config, args.tail, args.target)
    if args.command == "symbols":
        return cmd_symbols(args.symrecover_args)
    if args.command == "extc":
        from tools.coop import extc

        return extc.main(args.extc_args)
    if args.command == "behaviour":
        return cmd_behaviour(args.behaviour_args)
    if args.command == "equivalence":
        return cmd_equivalence(project, config, args.equivalence_args)
    if args.command == "opcodes":
        return cmd_opcodes(args.opcodes_args, config)

    parser.error(f"unknown command: {args.command}")
    return 2


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except FileNotFoundError as exc:
        print(f"ERROR: {exc}", file=sys.stderr)
        raise SystemExit(1)
    except subprocess.CalledProcessError as exc:
        print(f"ERROR: command failed ({exc.returncode}): {' '.join(exc.cmd)}", file=sys.stderr)
        raise SystemExit(exc.returncode)
