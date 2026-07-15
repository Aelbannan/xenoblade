#!/usr/bin/env python3
"""
Co-op decompilation runner for the Xenoblade downstream fork.

Usage (from repository root):
  python tools/coop/run.py status
  python tools/coop/run.py baseline
  python tools/coop/run.py configure
  python tools/coop/run.py ctx src/kyoshin/cf/CfPadTask.cpp
  python tools/coop/run.py build kyoshin/cf/CfPadTask
  python tools/coop/run.py diff kyoshin/cf/CfPadTask --symbol copyInputFlag__Q22cf9CfPadTaskFP4CPadUlUl
  python tools/coop/run.py size monolib/src/core/CViewRectDataCore
  python tools/coop/run.py size --all
  python tools/coop/run.py cycle pad-copy-input-flag
  python tools/coop/run.py queue --tier P1
  python tools/coop/run.py targets list
  python tools/coop/run.py log --tail 20
  python tools/coop/run.py symbols list
  python tools/coop/run.py behaviour audit
  python tools/coop/run.py behaviour compare --all
  python tools/coop/run.py equivalence check-hex --original ... --candidate ... --observe r3
  python tools/coop/run.py equivalence check-unit kyoshin/CGame --symbol OnPauseTrigger__5CGameFv
  python tools/coop/run.py opcodes
  python tools/coop/run.py symbols show UnkClass_8045F564
"""

from __future__ import annotations

import argparse
import hashlib
import subprocess
import sys
from pathlib import Path
from typing import Optional

ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from tools.coop.lib.attempts import AttemptRecord, append_attempt, count_attempts, read_attempts
from tools.coop.lib.config import CoopConfig, load_config
from tools.coop.lib.objdiff_report import (
    diff_function_json,
    evaluate_unit_match,
    find_function_match,
    meets_required_level,
    report_unit,
)
from tools.coop.lib.object_size import ObjectSizeCheck, check_object_size, format_size_check
from tools.coop.lib.project import Project
from tools.coop.lib.targets import get_target, load_targets, pending_targets


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
) -> int:
    unit = project.resolve_unit(hint)
    if unit.base_path:
        project.ninja_build(str(unit.base_path.relative_to(project.root)))
        _postprocess_mtrand_object(project, unit.base_path)

    evaluation = evaluate_unit_match(project, unit, symbol)
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

    evaluation = evaluate_unit_match(project, unit, target.symbol)
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
    )
    log_path = append_attempt(config.resolve(config.attempt_log), record)
    print(f"attempt #{attempt_num} logged to {log_path}")

    fn_percent = fn_match.match_percent if fn_match else None
    if not meets_required_level(
        target.required_level,
        evaluation.status,
        function_match=fn_percent,
        unit=unit_report,
        symbol=target.symbol,
        equivalence=evaluation.equivalence,
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
) -> int:
    targets = pending_targets(load_targets(config), tier)
    if not targets:
        print("no pending buildable targets")
        return 0
    print(f"queue: {len(targets)} target(s)")
    failures = 0
    for target in targets:
        print(f"\n==> {target.id} ({target.tier})")
        if dry_run:
            continue
        rc = cmd_cycle(project, config, target.id, hypothesis="", next_change="", runtime_test="")
        if rc != 0:
            failures += 1
    return 1 if failures else 0


def cmd_targets_list(config: CoopConfig, tier: Optional[str]) -> int:
    targets = load_targets(config)
    if tier:
        targets = [t for t in targets if t.tier == tier]
    for target in targets:
        buildable = "yes" if target.buildable else "no"
        print(
            f"{target.id:28} {target.tier:3} {target.milestone:8} "
            f"buildable={buildable:3} req={target.required_level:12} {target.function}"
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
    print(f"status:   {target.status}")
    if target.notes:
        print(f"notes:    {target.notes}")
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
        args[1:1] = ["--contract", "ppc-eabi"]
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


def cmd_equivalence(project: Project, config: CoopConfig, equivalence_args: list[str]) -> int:
    if equivalence_args and equivalence_args[0] == "--":
        equivalence_args = equivalence_args[1:]
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
    sub.add_parser("configure", help="Run python configure.py for the selected region")

    p_ctx = sub.add_parser("ctx", help="Generate decomp.me context for a source file")
    p_ctx.add_argument("source", type=Path)
    p_ctx.add_argument("-o", "--output", type=Path)

    p_build = sub.add_parser("build", help="Build one translation unit")
    p_build.add_argument("unit", help="objdiff unit hint or source path")

    p_diff = sub.add_parser("diff", help="Build and diff one translation unit")
    p_diff.add_argument("unit", help="objdiff unit hint or source path")
    p_diff.add_argument("--symbol", help="Function symbol for per-function stats")

    p_size = sub.add_parser("size", help="Check decomp .text size against split budget")
    p_size.add_argument("unit", nargs="?", help="objdiff unit hint or source path")
    p_size.add_argument("--all", action="store_true", help="Check every buildable objdiff unit")

    p_cycle = sub.add_parser("cycle", help="ctx + build + diff + JSONL log for one target id")
    p_cycle.add_argument("target_id")
    p_cycle.add_argument("--hypothesis", default="")
    p_cycle.add_argument("--next-change", default="")
    p_cycle.add_argument("--runtime-test", default="")

    p_queue = sub.add_parser("queue", help="Run cycle on pending targets")
    p_queue.add_argument("--tier", choices=["P0", "P1", "P2", "P3"])
    p_queue.add_argument("--dry-run", action="store_true")

    p_targets = sub.add_parser("targets", help="Inspect curated targets")
    p_targets_sub = p_targets.add_subparsers(dest="targets_cmd", required=True)
    p_targets_list = p_targets_sub.add_parser("list")
    p_targets_list.add_argument("--tier", choices=["P0", "P1", "P2", "P3"])
    p_targets_show = p_targets_sub.add_parser("show")
    p_targets_show.add_argument("target_id")

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

    args = parser.parse_args()
    config = load_config(args.config, ROOT)
    project = Project(config)

    if args.command == "status":
        return cmd_status(project, config)
    if args.command == "baseline":
        return cmd_baseline(project, config)
    if args.command == "configure":
        return cmd_configure(project, config)
    if args.command == "ctx":
        return cmd_ctx(project, args.source, args.output)
    if args.command == "build":
        return cmd_build(project, args.unit)
    if args.command == "diff":
        return cmd_diff(project, config, args.unit, args.symbol, write_function_diff=False)
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
        )
    if args.command == "queue":
        return cmd_queue(project, config, args.tier, dry_run=args.dry_run)
    if args.command == "targets" and args.targets_cmd == "list":
        return cmd_targets_list(config, args.tier)
    if args.command == "targets" and args.targets_cmd == "show":
        return cmd_targets_show(config, args.target_id)
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
