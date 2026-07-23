#!/usr/bin/env python3
"""Create missing translation units for catalog-only (not-buildable) targets.

The ~13k ``origin=symbols.txt`` rows already have ``unit`` from splits.txt and
are listed in ``configure.py``, but have no ``.cpp``/``.c`` on disk and no
``source`` in ``targets.json``. This tool creates those files with harness-marked
mangled ``extern "C"`` stubs plus a shared catalog include
(``kyoshin/harness_catalog.hpp`` or ``<harness_catalog.h>``) and wires ``source``.

Usage:
  python3 tools/coop/scaffold_catalog_tus.py
  python3 tools/coop/scaffold_catalog_tus.py --prefix kyoshin --limit-units 3
  python3 tools/coop/scaffold_catalog_tus.py --prefix kyoshin --limit-units 3 --write
  python3 tools/coop/scaffold_catalog_tus.py --tu kyoshin/plugin/ocBuiltin --write --verify
"""
from __future__ import annotations

import argparse
import re
import subprocess
import sys
from collections import defaultdict
from dataclasses import dataclass
from pathlib import Path
from typing import Dict, Iterable, List, Optional, Sequence, Tuple

ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from tools.coop.lib.config import load_config
from tools.coop.lib.targets import (
    Target,
    load_targets,
    load_targets_document,
    write_targets_document,
)
from tools.coop.scaffold_missing_slots import (
    _format_stub,
    _infer_return_type,
    _is_thunk_symbol,
    _symbol_has_invalid_identifier_chars,
    begin_marker,
)

_OBJECT_PATH_RE = re.compile(r'Object\([^,]+,\s*"(?P<path>[^"]+)"')


@dataclass(frozen=True)
class CatalogStub:
    target_id: str
    symbol: str
    function: str
    unit: str
    stub_text: str
    return_type: str


@dataclass(frozen=True)
class UnitPlan:
    unit: str
    object_path: str
    source_path: Path
    stubs: Tuple[CatalogStub, ...]
    skip_reasons: Tuple[Tuple[str, str], ...]  # (target_id, reason)


def _unit_matches(unit: Optional[str], needle: str) -> bool:
    if not unit:
        return False
    u = unit.removeprefix("main/")
    n = needle.removeprefix("main/")
    return u == n or u.endswith("/" + n) or n.endswith("/" + u)


def _parse_configure_object_paths(configure_path: Path) -> Dict[str, str]:
    """Map unit stem -> configure Object relative path (with extension)."""
    text = configure_path.read_text(encoding="utf-8")
    mapping: Dict[str, str] = {}
    for match in _OBJECT_PATH_RE.finditer(text):
        object_path = match.group("path")
        unit = str(Path(object_path).with_suffix(""))
        mapping[unit] = object_path
    return mapping


def resolve_source_path(project_root: Path, object_path: str) -> Path:
    """Map a configure Object path to its on-disk location.

    Game code (``kyoshin/``, ``us/``) lives under ``src/``; library objects use
    ``libs/<object_path>`` (configure ``src_dir: libs``).
    """
    if object_path.startswith("kyoshin/") or object_path.startswith("us/"):
        return project_root / "src" / object_path
    return project_root / "libs" / object_path


def _parse_retail_size(target: Target) -> Optional[int]:
    raw = target.extra.get("size")
    if raw is None and hasattr(target, "size"):
        raw = getattr(target, "size", None)
    if raw is None:
        return None
    text = str(raw).strip()
    try:
        return int(text, 0)
    except ValueError:
        return None


def _should_skip_symbol(symbol: str, *, keep_sinit: bool) -> Optional[str]:
    if _is_thunk_symbol(symbol) or _symbol_has_invalid_identifier_chars(symbol):
        return "invalid_symbol"
    if not keep_sinit and (
        symbol.startswith("__sinit_") or symbol.startswith("sinit_")
    ):
        return "sinit"
    return None


def collect_catalog_targets(
    targets: Sequence[Target],
    *,
    status: str,
    tu: Optional[str],
    prefix: Optional[str],
) -> List[Target]:
    selected: List[Target] = []
    for target in targets:
        if target.kind != "function" or target.status != status:
            continue
        if not target.symbol or not target.unit:
            continue
        if target.source is not None and target.source.is_file():
            continue
        if tu is not None and not _unit_matches(target.unit, tu):
            continue
        unit = target.unit.removeprefix("main/")
        if prefix is not None and not (
            unit == prefix or unit.startswith(prefix.rstrip("/") + "/")
        ):
            continue
        selected.append(target)
    return selected


def build_unit_plans(
    targets: Sequence[Target],
    *,
    project_root: Path,
    object_by_unit: Dict[str, str],
    keep_sinit: bool,
) -> Tuple[List[UnitPlan], List[str]]:
    by_unit: Dict[str, List[Target]] = defaultdict(list)
    for target in targets:
        by_unit[target.unit.removeprefix("main/")].append(target)

    plans: List[UnitPlan] = []
    errors: List[str] = []
    for unit in sorted(by_unit):
        object_path = object_by_unit.get(unit)
        if object_path is None:
            errors.append(f"{unit}: not found in configure.py Object() list")
            continue
        source_path = resolve_source_path(project_root, object_path)
        c_source = Path(object_path).suffix.lower() == ".c"
        stubs: List[CatalogStub] = []
        skips: List[Tuple[str, str]] = []
        # Stable order by address / id.
        ordered = sorted(
            by_unit[unit],
            key=lambda t: (str(t.extra.get("address") or t.address or ""), t.id),
        )
        for target in ordered:
            reason = _should_skip_symbol(target.symbol, keep_sinit=keep_sinit)
            if reason:
                skips.append((target.id, reason))
                continue
            retail_size = _parse_retail_size(target)
            return_type = _infer_return_type(target.symbol, retail_size) or "void"
            stub = _format_stub(
                target_id=target.id,
                qualified_name=target.symbol,
                args="()",
                return_type=return_type,
                is_c_linkage=True,
                symbol=target.symbol,
                force_mangled_symbol=True,
                c_source=c_source,
            )
            stubs.append(
                CatalogStub(
                    target_id=target.id,
                    symbol=target.symbol,
                    function=target.function,
                    unit=unit,
                    stub_text=stub,
                    return_type=return_type,
                )
            )
        plans.append(
            UnitPlan(
                unit=unit,
                object_path=object_path,
                source_path=source_path,
                stubs=tuple(stubs),
                skip_reasons=tuple(skips),
            )
        )
    return plans, errors


def _scaffold_include_line(plan: UnitPlan) -> str:
    """Prefer a real unit header when present; else the kyoshin/libs umbrella."""
    unit = plan.unit.removeprefix("main/")
    object_path = plan.object_path.replace("\\", "/")
    unit_header = plan.source_path.with_suffix(".hpp")
    if unit_header.is_file():
        try:
            rel_src = unit_header.relative_to(ROOT / "src").as_posix()
            return f'#include "{rel_src}"\n'
        except ValueError:
            pass
    if unit.startswith("kyoshin/") or object_path.startswith("kyoshin/"):
        return '#include "kyoshin/harness_catalog.hpp"\n'
    return "#include <harness_catalog.h>\n"


def _render_unit_source(plan: UnitPlan) -> str:
    header = (
        f"// Auto-scaffolded catalog TU for {plan.unit}\n"
        f"// Mangled extern stubs for llm-harness / coop selection.\n"
        f"// Replace stubs with high-level C/C++ during decomp.\n\n"
        f"{_scaffold_include_line(plan)}\n"
    )
    body = "\n".join(stub.stub_text.rstrip() + "\n" for stub in plan.stubs)
    return header + body


def apply_unit_plans(
    plans: Sequence[UnitPlan],
    *,
    project_root: Path,
    config,
    overwrite: bool,
) -> Tuple[int, int, List[str]]:
    """Create source files and update targets.json. Returns (files, stubs, errors)."""
    errors: List[str] = []
    files_written = 0
    stubs_written = 0
    document = load_targets_document(config)
    rows_by_id = {
        str(row["id"]): row
        for row in document.get("targets", [])
        if isinstance(row, dict) and isinstance(row.get("id"), str)
    }
    relative_sources: Dict[str, str] = {}

    for plan in plans:
        if not plan.stubs:
            continue
        if plan.source_path.is_file() and not overwrite:
            errors.append(
                f"{plan.unit}: source already exists at "
                f"{plan.source_path.relative_to(project_root)}; pass --overwrite"
            )
            continue
        text = _render_unit_source(plan)
        # Validate markers round-trip before write.
        for stub in plan.stubs:
            if begin_marker(stub.target_id) not in text:
                errors.append(f"{stub.target_id}: marker missing in generated source")
                break
        else:
            plan.source_path.parent.mkdir(parents=True, exist_ok=True)
            plan.source_path.write_text(text, encoding="utf-8")
            files_written += 1
            stubs_written += len(plan.stubs)
            rel = plan.source_path.relative_to(project_root).as_posix()
            for stub in plan.stubs:
                relative_sources[stub.target_id] = rel

    if relative_sources:
        changed = 0
        for target_id, source in relative_sources.items():
            row = rows_by_id.get(target_id)
            if row is None:
                errors.append(f"{target_id}: missing from targets.json")
                continue
            row["source"] = source
            if row.get("workflow_status") in {None, "DISCOVERY", "BACKLOG"}:
                row["workflow_status"] = "BACKLOG"
            changed += 1
        if changed:
            write_targets_document(config, document)

    return files_written, stubs_written, errors


def verify_units(units: Iterable[str], *, project_root: Path) -> List[str]:
    """Reconfigure once, then size-check each unit. Returns failure lines."""
    failures: List[str] = []
    configure = subprocess.run(
        [sys.executable, "configure.py"],
        cwd=str(project_root),
        capture_output=True,
        text=True,
    )
    if configure.returncode != 0:
        tail = (configure.stdout + configure.stderr)[-2000:]
        return [f"configure.py failed:\n{tail}"]

    for unit in units:
        result = subprocess.run(
            [sys.executable, "tools/coop/run.py", "size", unit],
            cwd=str(project_root),
            capture_output=True,
            text=True,
        )
        out = result.stdout + result.stderr
        size_line = next((ln for ln in out.splitlines() if ln.startswith("size:")), "")
        compile_fail = (
            result.returncode != 0
            and ("ninja: build stopped" in out or "ERROR: command failed" in out)
        )
        if compile_fail:
            err = next(
                (ln for ln in out.splitlines() if "Error" in ln or re.search(r"#\s+\d+:", ln)),
                out[-500:],
            )
            failures.append(f"{unit}: COMPILE {err.strip()}")
        elif result.returncode != 0:
            failures.append(f"{unit}: size check failed {size_line or out[-300:]}")
        else:
            print(f"verify ok {unit} {size_line}".rstrip(), flush=True)
    return failures


def main(argv: Optional[List[str]] = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--config", type=Path, default=None)
    parser.add_argument(
        "--tu",
        default=None,
        help="Restrict to one translation unit (e.g. kyoshin/plugin/ocBuiltin)",
    )
    parser.add_argument(
        "--prefix",
        default=None,
        help="Restrict to units under this prefix (e.g. kyoshin)",
    )
    parser.add_argument(
        "--limit-units",
        type=int,
        default=0,
        help="Only plan/write the first N units after filtering (0 = all)",
    )
    parser.add_argument(
        "--status",
        default="NOT_STARTED",
        help="Only consider targets with this match status (default: NOT_STARTED)",
    )
    parser.add_argument(
        "--keep-sinit",
        action="store_true",
        help="Include __sinit_* static initializer symbols (skipped by default)",
    )
    parser.add_argument(
        "--write",
        action="store_true",
        help="Create source files and update targets.json (otherwise dry-run)",
    )
    parser.add_argument(
        "--overwrite",
        action="store_true",
        help="Replace an existing scaffolded source file",
    )
    parser.add_argument(
        "--verify",
        action="store_true",
        help="After --write, run configure.py and coop size for each written unit",
    )
    args = parser.parse_args(argv)

    if args.verify and not args.write:
        parser.error("--verify requires --write")

    config = load_config(args.config, ROOT)
    project_root = config.project_root
    object_by_unit = _parse_configure_object_paths(project_root / "configure.py")
    selected = collect_catalog_targets(
        load_targets(config),
        status=args.status,
        tu=args.tu,
        prefix=args.prefix,
    )
    plans, plan_errors = build_unit_plans(
        selected,
        project_root=project_root,
        object_by_unit=object_by_unit,
        keep_sinit=args.keep_sinit,
    )
    if args.limit_units > 0:
        plans = plans[: args.limit_units]

    stub_count = sum(len(p.stubs) for p in plans)
    skip_count = sum(len(p.skip_reasons) for p in plans)
    skip_reasons: Dict[str, int] = defaultdict(int)
    for plan in plans:
        for _tid, reason in plan.skip_reasons:
            skip_reasons[reason] += 1

    existing = sum(1 for p in plans if p.source_path.is_file())
    print(
        f"catalog_targets={len(selected)} units={len(plans)} stubs={stub_count} "
        f"skipped_symbols={skip_count} existing_files={existing}"
    )
    if skip_reasons:
        print(
            "symbol_skips: "
            + " ".join(f"{k}={v}" for k, v in sorted(skip_reasons.items()))
        )
    for err in plan_errors[:20]:
        print(f"error: {err}", file=sys.stderr)
    for plan in plans[:15]:
        rel = plan.source_path.relative_to(project_root)
        print(
            f"  {plan.unit}: stubs={len(plan.stubs)} skips={len(plan.skip_reasons)} "
            f"-> {rel}"
        )
    if len(plans) > 15:
        print(f"  ... and {len(plans) - 15} more units")

    if not args.write:
        print("dry-run only; pass --write to create files and update targets.json")
        return 1 if plan_errors else 0

    files, stubs, write_errors = apply_unit_plans(
        plans,
        project_root=project_root,
        config=config,
        overwrite=args.overwrite,
    )
    print(f"written_files={files} written_stubs={stubs}")
    for err in write_errors[:20]:
        print(f"error: {err}", file=sys.stderr)

    verify_failures: List[str] = []
    if args.verify and files:
        written_units = [
            p.unit
            for p in plans
            if p.stubs and p.source_path.is_file()
        ]
        verify_failures = verify_units(written_units, project_root=project_root)
        for line in verify_failures[:20]:
            print(f"verify: {line}", file=sys.stderr)

    if plan_errors or write_errors or verify_failures:
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
