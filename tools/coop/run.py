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
  python3 tools/coop/run.py queue --tier P1
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
import hashlib
import json
import subprocess
import sys
from pathlib import Path
from typing import Optional

ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from tools.coop.lib.attempts import AttemptRecord, append_attempt, append_contribution, count_attempts, read_attempts
from tools.coop.lib.config import CoopConfig, load_config
from tools.coop.lib.objdiff_report import (
    diff_function_json,
    evaluate_unit_match,
    find_function_match,
    meets_required_level,
    report_unit,
)
from tools.coop.lib.object_size import ObjectSizeCheck, check_object_size, format_size_check
from tools.coop.lib.project import ObjdiffUnit, Project

from tools.coop.lib.targets import (
    audit_promotion_registry,
    claim_target,
    equivalence_certificate_error,
    get_target,
    harness_targets,
    import_symbols,
    load_targets,
    load_targets_document,
    pending_targets,
    plan_recertify_bottom_up,
    recertify_ready_wave,
    release_target,
    sync_results_from_attempts,
    sync_called_functions,
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


def cmd_build(project: Project, hint: str) -> int:
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
    _postprocess_reloc_object(project, obj)


def _object_paths_for_unit(project: Project, unit) -> tuple[Path | None, Path | None]:
  # objdiff: target_path = retail split object, base_path = decompiled object
    retail = unit.target_path
    decomp = unit.base_path
    return retail, decomp


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
        project, unit, symbol, linked=linked, target_id=target_id,
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
        detail = f" ({evaluation.equivalence_detail})" if evaluation.equivalence_detail else ""
        print(f"equivalence: {evaluation.equivalence.value}{detail}")

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
    if not size_check.ok:
        return 1
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
) -> int:
    targets = load_targets(config)
    target = get_target(targets, target_id)
    if not target.buildable:
        print(f"ERROR: target '{target_id}' is not buildable yet ({target.notes})", file=sys.stderr)
        return 1

    assert target.source is not None
    assert target.unit is not None

    ctx_out = target.source.with_suffix(".ctx.c")
    cmd_ctx(project, target.source.relative_to(project.root), ctx_out)
    cmd_build(project, target.unit)

    unit = project.resolve_unit(target.unit)
    if unit.base_path:
        _postprocess_mtrand_object(project, unit.base_path)

    evaluation = evaluate_unit_match(
        project, unit, target.symbol, linked=linked, target_id=target.id,
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
        detail = f" ({evaluation.equivalence_detail})" if evaluation.equivalence_detail else ""
        print(f"equivalence: {evaluation.equivalence.value}{detail}")
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
    if not size_check.ok:
        return 1

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
    print(f"PASS: meets required level {target.required_level}")
    return 0


def cmd_queue(
    project: Project,
    config: CoopConfig,
    tier: Optional[str],
    *,
    dry_run: bool,
    selection: str = "pending",
    include_catalog: bool = False,
    limit: Optional[int] = None,
) -> int:
    all_targets = load_targets(config)
    if selection == "pending" and not include_catalog:
        targets = pending_targets(all_targets, tier)
    else:
        targets = harness_targets(
            all_targets,
            selection=selection,
            tier=tier,
            include_catalog=include_catalog,
        )
    if limit is not None:
        targets = targets[:limit]
    if not targets:
        print("no pending buildable targets")
        return 0
    print(f"queue: {len(targets)} target(s)")
    failures = 0
    for target in targets:
        print(f"\n==> {target.id} ({target.tier})")
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
    tier: Optional[str],
    milestone: Optional[str],
    workflow_status: Optional[str],
    match_status: Optional[str],
    kind: Optional[str],
    include_catalog: bool,
) -> int:
    targets = load_targets(config)
    if not include_catalog:
        targets = [target for target in targets if target.extra.get("origin") != "symbols.txt"]
    if tier:
        targets = [t for t in targets if t.tier == tier]
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
            f"{target.id:28} {target.tier:3} {target.milestone:16} "
            f"flow={target.workflow_status:12} match={target.status:16} "
            f"buildable={buildable:3} {target.function}"
        )
    return 0


def cmd_targets_show(config: CoopConfig, target_id: str) -> int:
    target = get_target(load_targets(config), target_id)
    print(f"id:       {target.id}")
    print(f"tier:     {target.tier}")
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
                "tier": target.tier,
                "milestone": target.milestone,
                "workflow_status": workflow,
                "match_status": match_status,
                "instruction_match": instruction_match,
                "buildable": target.buildable,
                "unit": target.unit,
                "catalog": target.extra.get("origin") == "symbols.txt",
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
    tier: Optional[str],
    milestone: Optional[str],
    kind: Optional[str],
    include_catalog: bool,
) -> list[dict]:
    if not include_catalog:
        rows = [row for row in rows if not row["catalog"]]
    if tier:
        rows = [row for row in rows if row["tier"] == tier]
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
                "| Target | Function | Tier | Workflow | Match | Percent | Owner | Buildable |",
                "|---|---|---|---|---|---:|---|---|",
            ]
        )
        for row in sorted(by_milestone[milestone], key=lambda item: (item["tier"], item["id"])):
            percent = row["instruction_match"]
            percent_text = f"{percent:.1f}%" if isinstance(percent, (int, float)) else "—"
            function = str(row["function"]).replace("|", "\\|")
            lines.append(
                f"| `{row['id']}` | {function} | {row['tier']} | "
                f"{row['workflow_status']} | {row['match_status']} | {percent_text} | "
                f"{row['owner'] or '—'} | "
                f"{'yes' if row['buildable'] else 'no'} |"
            )
    return "\n".join(lines) + "\n"


def cmd_targets_status(
    config: CoopConfig,
    *,
    tier: Optional[str],
    milestone: Optional[str],
    kind: Optional[str],
    output: Optional[Path],
    output_format: str,
    include_catalog: bool,
) -> int:
    rows = _filter_target_rows(
        _resolved_target_rows(config),
        tier=tier,
        milestone=milestone,
        kind=kind,
        include_catalog=include_catalog,
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
    data, added, skipped = import_symbols(project, config, kind=kind)
    label = "all symbols" if kind == "all" else f"{kind} symbols"
    print(f"import {label}: {added} add, {skipped} already present")
    if dry_run:
        print("dry-run: registry not changed")
        return 0
    path = write_targets_document(config, data)
    print(f"updated: {path}")
    return cmd_targets_validate(config)


def cmd_targets_sync_attempts(config: CoopConfig) -> int:
    attempts = read_attempts(config.resolve(config.attempt_log))
    changed = sync_results_from_attempts(config, attempts)
    print(f"synced latest attempts into target registry: {changed} target(s) changed")
    return cmd_targets_validate(config)


def cmd_targets_sync_calls(project: Project, config: CoopConfig, *, dry_run: bool) -> int:
    data, scanned, resolved, unresolved = sync_called_functions(project, config)
    print(
        f"call graph: {scanned} function record(s), "
        f"{resolved} resolved direct edge(s), {unresolved} unresolved direct edge(s)"
    )
    if dry_run:
        print("dry-run: registry not changed")
        return 0
    path = write_targets_document(config, data)
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


def cmd_targets_release(config: CoopConfig, target_id: str, *, owner: Optional[str]) -> int:
    path = release_target(config, target_id, owner=owner)
    print(f"released {target_id}: {path}")
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
        f"- Priority/milestone: `{target.tier}` / `{target.milestone}`",
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
    include_catalog: bool,
    linked: bool = False,
) -> int:
    """Issue/refresh semantic certificates for accepted targets, leaves first."""
    if not bottom_up:
        print("ERROR: targets recertify requires --bottom-up", file=sys.stderr)
        return 2

    targets = load_targets(config)
    plan = plan_recertify_bottom_up(targets, include_catalog=include_catalog)
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
        wave = recertify_ready_wave(
            live_targets,
            include_catalog=include_catalog,
            skip_ids=failed_ids,
        )
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

            update_target_result(
                config,
                target.id,
                status=target.status,
                instruction_match=(
                    evaluation.fn_match.match_percent if evaluation.fn_match else None
                ),
                equivalence_status=evaluation.equivalence.value,
                equivalence_certificate=certificate,
                certificate_checked=True,
                equivalence_confidence=evaluation.equivalence_confidence,
                equivalence_policy=evaluation.equivalence_policy,
            )
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
        return _run_check_unit_linked(project, config, unit, symbol, parsed.candidate_symbol, rest)

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


def _run_check_unit_linked(
    project: Project,
    config: CoopConfig,
    unit: ObjdiffUnit,
    symbol: str,
    candidate_symbol: Optional[str],
    rest: list[str],
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

    probe = prove_unit_symbol(
        project, unit, symbol,
        contract=contract,
        candidate_symbol=candidate_symbol,
        linked=True,
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


def cmd_atlas(atlas_args: list[str]) -> int:
    if atlas_args and atlas_args[0] == "--":
        atlas_args = atlas_args[1:]
    script = ROOT / "tools" / "decomp_atlas" / "run.py"
    cmd = [sys.executable, str(script), *atlas_args]
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

    p_size = sub.add_parser("size", help="Check decomp .text size against split budget")
    p_size.add_argument("unit", nargs="?", help="objdiff unit hint or source path")
    p_size.add_argument("--all", action="store_true", help="Check every buildable objdiff unit")

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

    def add_harness_args(command_parser: argparse.ArgumentParser) -> None:
        command_parser.add_argument("--tier")
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
        command_parser.add_argument(
            "--include-catalog",
            action="store_true",
            help="Include buildable P9 functions imported from symbols.txt",
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
    p_targets_list.add_argument("--tier")
    p_targets_list.add_argument("--milestone")
    p_targets_list.add_argument("--workflow-status")
    p_targets_list.add_argument("--match-status")
    p_targets_list.add_argument("--kind")
    p_targets_list.add_argument(
        "--include-catalog",
        action="store_true",
        help="Include P9 records imported directly from symbols.txt",
    )
    p_targets_show = p_targets_sub.add_parser("show")
    p_targets_show.add_argument("target_id")
    p_targets_status = p_targets_sub.add_parser(
        "status", help="Render a generated human-readable or JSON status view"
    )
    p_targets_status.add_argument("--tier")
    p_targets_status.add_argument("--milestone")
    p_targets_status.add_argument("--kind", default="function")
    p_targets_status.add_argument("--format", choices=["markdown", "json"], default="markdown")
    p_targets_status.add_argument("--output", type=Path)
    p_targets_status.add_argument(
        "--include-catalog",
        action="store_true",
        help="Include unassigned records imported directly from symbols.txt",
    )
    p_targets_sub.add_parser("validate", help="Validate registry identities and status vocabularies")
    p_targets_sub.add_parser(
        "sync-attempts",
        help="Migrate each target's latest attempts.jsonl result into current registry state",
    )
    p_targets_calls = p_targets_sub.add_parser(
        "sync-calls", help="Populate called_functions from generated retail assembly"
    )
    p_targets_calls.add_argument("--dry-run", action="store_true")
    p_targets_claim = p_targets_sub.add_parser(
        "claim", help="Record the current owner and exclusive edit scope in the registry"
    )
    p_targets_claim.add_argument("target_id")
    p_targets_claim.add_argument("--owner", required=True)
    p_targets_claim.add_argument("--allowed-path", action="append", default=[])
    p_targets_claim.add_argument("--note", default="")
    p_targets_release = p_targets_sub.add_parser("release", help="Release a current claim")
    p_targets_release.add_argument("target_id")
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
        "--include-catalog",
        action="store_true",
        help="Include buildable P9 functions imported from symbols.txt",
    )
    p_targets_recertify.add_argument(
        "--linked",
        action="store_true",
        help=(
            "Allow SMT prove fallback to linked DOL/ELF bytes when unlinked "
            "objects are inconclusive due to relocations"
        ),
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

    p_opcodes = sub.add_parser(
        "opcodes",
        help="List PowerPC opcodes used in main.dol (wraps tools.dol_opcodes)",
    )
    p_opcodes.add_argument(
        "opcodes_args",
        nargs=argparse.REMAINDER,
        help="optional DOL path and flags (e.g. --sort count, --names-only, --json)",
    )

    p_atlas = sub.add_parser(
        "atlas",
        help="Decomp Atlas index/serve (wraps tools/decomp_atlas/run.py)",
    )
    p_atlas.add_argument(
        "atlas_args",
        nargs=argparse.REMAINDER,
        help="atlas subcommand (status, index [--full] [--vectors], serve [--enable-jobs])",
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
            write_function_diff=False, linked=args.linked,
        )
    if args.command == "size":
        return cmd_size(project, config, args.unit, check_all=args.all)
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
        )
    if args.command == "queue":
        return cmd_queue(
            project,
            config,
            args.tier,
            dry_run=args.dry_run,
            selection=args.selection,
            include_catalog=args.include_catalog,
            limit=args.limit,
        )
    if args.command == "harness":
        return cmd_queue(
            project,
            config,
            args.tier,
            dry_run=args.dry_run,
            selection=args.selection,
            include_catalog=args.include_catalog,
            limit=args.limit,
        )
    if args.command == "targets" and args.targets_cmd == "list":
        return cmd_targets_list(
            config,
            args.tier,
            args.milestone,
            args.workflow_status,
            args.match_status,
            args.kind,
            args.include_catalog,
        )
    if args.command == "targets" and args.targets_cmd == "show":
        return cmd_targets_show(config, args.target_id)
    if args.command == "targets" and args.targets_cmd == "status":
        return cmd_targets_status(
            config,
            tier=args.tier,
            milestone=args.milestone,
            kind=args.kind,
            output=args.output,
            output_format=args.format,
            include_catalog=args.include_catalog,
        )
    if args.command == "targets" and args.targets_cmd == "validate":
        return cmd_targets_validate(config)
    if args.command == "targets" and args.targets_cmd == "sync-attempts":
        return cmd_targets_sync_attempts(config)
    if args.command == "targets" and args.targets_cmd == "sync-calls":
        return cmd_targets_sync_calls(project, config, dry_run=args.dry_run)
    if args.command == "targets" and args.targets_cmd == "claim":
        return cmd_targets_claim(
            config,
            args.target_id,
            owner=args.owner,
            allowed_paths=args.allowed_path,
            note=args.note,
        )
    if args.command == "targets" and args.targets_cmd == "release":
        return cmd_targets_release(config, args.target_id, owner=args.owner)
    if args.command == "targets" and args.targets_cmd == "audit-promotion":
        return cmd_targets_audit_promotion(
            config,
            apply=bool(args.apply) and not args.dry_run,
            write_report=args.write_report,
        )
    if args.command == "targets" and args.targets_cmd == "recertify":
        return cmd_targets_recertify(
            project,
            config,
            bottom_up=bool(args.bottom_up),
            dry_run=bool(args.dry_run),
            limit=args.limit,
            include_catalog=bool(args.include_catalog),
            linked=bool(args.linked),
        )
    if args.command == "targets" and args.targets_cmd == "brief":
        return cmd_targets_brief(config, args.target_id, output=args.output)
    if args.command == "targets" and args.targets_cmd == "import-symbols":
        return cmd_targets_import_symbols(
            project, config, kind=args.kind, dry_run=args.dry_run
        )
    if args.command == "log":
        return cmd_log(config, args.tail, args.target)
    if args.command == "symbols":
        return cmd_symbols(args.symrecover_args)
    if args.command == "behaviour":
        return cmd_behaviour(args.behaviour_args)
    if args.command == "equivalence":
        return cmd_equivalence(project, config, args.equivalence_args)
    if args.command == "opcodes":
        return cmd_opcodes(args.opcodes_args, config)
    if args.command == "atlas":
        return cmd_atlas(args.atlas_args)

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
