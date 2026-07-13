#!/usr/bin/env python3
"""
Symbol recovery tooling for the Xenoblade decompilation fork.

Examples:
  python tools/symrecover.py list --region us
  python tools/symrecover.py show UnkClass_8045F564
  python tools/symrecover.py xref 8043C59C
  python tools/symrecover.py demangle createRegion__17UnkClass_8045F564FiiPCci
  python tools/symrecover.py rtti cf
  python tools/symrecover.py rename-plan UnkClass_8045F564 CLibLayoutRegion
  python tools/symrecover.py rename-apply UnkClass_8045F564 CLibLayoutRegion --dry-run
  python tools/symrecover.py rename-all UnkClass_8043C59C CViewRectDataCore --dry-run
  python tools/symrecover.py compare-regions UnkClass_8043C59C
"""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from tools.symbolrecover.lib.catalog import (  # noqa: E402
    find_type,
    list_rtti_symbols,
    load_catalog,
)
from tools.symbolrecover.lib.mwcc import (  # noqa: E402
    demangle_symbol,
    rename_mangling_compatible,
    suggest_same_length_names,
)
from tools.symbolrecover.lib.parser import load_symbols  # noqa: E402
from tools.symbolrecover.lib.rename import apply_rename, plan_rename  # noqa: E402
from tools.symbolrecover.lib.rename_source import apply_full_rename, plan_full_rename  # noqa: E402
from tools.symbolrecover.lib.xref import build_xref, compare_regions  # noqa: E402

REGIONS = ("us", "eu", "jp")


def _symbols_path(project_root: Path, region: str) -> Path:
    return project_root / "config" / region / "symbols.txt"


def cmd_list(args: argparse.Namespace) -> int:
    catalog = load_catalog(_symbols_path(args.project_root, args.region))
    rows = sorted(catalog.values(), key=lambda item: item.name)
    if args.kind:
        rows = [r for r in rows if r.kind == args.kind]
    if args.json:
        payload = [
            {
                "name": row.name,
                "address": row.address,
                "kind": row.kind,
                "symbol_count": row.symbol_count,
                "namespaces": sorted(row.namespaces),
                "methods": sorted(row.methods),
            }
            for row in rows
        ]
        print(json.dumps(payload, indent=2))
        return 0
    print(f"{'Type':<28} {'Addr':<10} {'Syms':>5}  Namespaces / methods")
    print("-" * 72)
    for row in rows:
        ns = ", ".join(sorted(row.namespaces)) or "-"
        methods = ", ".join(sorted(row.methods)[:4])
        if len(row.methods) > 4:
            methods += ", ..."
        print(f"{row.name:<28} {row.address:<10} {row.symbol_count:>5}  {ns}")
        if methods:
            print(f"{'':28} {'':10} {'':>5}  {methods}")
    print(f"\n{len(rows)} placeholder types")
    return 0


def cmd_show(args: argparse.Namespace) -> int:
    symbols_path = _symbols_path(args.project_root, args.region)
    catalog = load_catalog(symbols_path)
    item = find_type(catalog, args.query)
    if item is None:
        print(f"ERROR: no placeholder type matching {args.query!r}", file=sys.stderr)
        return 1
    print(f"type:     {item.name}")
    print(f"address:  0x{item.address}")
    print(f"kind:     {item.kind}")
    print(f"symbols:  {item.symbol_count}")
    if item.namespaces:
        print(f"namespace: {', '.join(sorted(item.namespaces))}")
    if item.methods:
        print(f"methods:  {', '.join(sorted(item.methods))}")
    print()
    for entry in sorted(item.symbols, key=lambda e: e.address):
        info = demangle_symbol(entry.name)
        label = info.function
        if info.args:
            label += info.args
        print(f"{entry.address_hex}  {entry.size or 0:>5}  {label}")
        print(f"           {entry.name}")
    return 0


def cmd_xref(args: argparse.Namespace) -> int:
    symbols_path = _symbols_path(args.project_root, args.region)
    symbols = load_symbols(symbols_path)
    catalog = load_catalog(symbols_path)
    item = find_type(catalog, args.query)
    placeholder = item.name if item else args.query
    xref = build_xref(args.project_root, args.region, placeholder, symbols)
    if args.json:
        print(
            json.dumps(
                {
                    "placeholder": xref.placeholder,
                    "symbols": len(xref.symbols),
                    "split_units": [u.path for u in xref.split_units],
                    "source_files": [str(p.relative_to(args.project_root)) for p in xref.source_files],
                    "configure_refs": xref.configure_refs,
                },
                indent=2,
            )
        )
        return 0
    print(f"placeholder: {xref.placeholder}")
    print(f"symbols:     {len(xref.symbols)}")
    print("\nSplit units:")
    if xref.split_units:
        for unit in xref.split_units:
            span = ""
            if unit.text_start is not None and unit.text_end is not None:
                span = f"  (.text 0x{unit.text_start:08X}-0x{unit.text_end:08X})"
            print(f"  {unit.path}{span}")
    else:
        print("  (none)")
    print("\nSource files:")
    if xref.source_files:
        for path in xref.source_files:
            print(f"  {path.relative_to(args.project_root)}")
    else:
        print("  (none — type may only exist in symbols/splits)")
    print("\nconfigure.py:")
    if xref.configure_refs:
        for line in xref.configure_refs:
            print(f"  {line}")
    else:
        print("  (none)")
    region_counts = compare_regions(args.project_root, placeholder, REGIONS)
    print("\nRegion symbol hits:")
    for region, count in region_counts.items():
        print(f"  {region}: {count if count >= 0 else 'missing'}")
    return 0


def cmd_demangle(args: argparse.Namespace) -> int:
    for symbol in args.symbols:
        info = demangle_symbol(symbol)
        print(f"raw:       {info.raw}")
        if info.namespace:
            print(f"namespace: {info.namespace}")
        if info.class_name:
            print(f"class:     {info.class_name}")
        print(f"function:  {info.function}{info.args or ''}")
        print()
    return 0


def cmd_rtti(args: argparse.Namespace) -> int:
    entries = list_rtti_symbols(load_symbols(_symbols_path(args.project_root, args.region)), args.query or "")
    for entry in entries:
        print(f"{entry.address_hex}  {entry.name}")
    print(f"\n{len(entries)} RTTI entries")
    return 0


def cmd_rename_plan(args: argparse.Namespace) -> int:
    plan = plan_rename(args.project_root, args.old_type, args.new_type, list(REGIONS))
    compatible = rename_mangling_compatible(args.old_type, args.new_type)
    print(f"old:  {plan.old_type} ({len(plan.old_type)} chars)")
    print(f"new:  {plan.new_type} ({len(plan.new_type)} chars)")
    print(f"mangling-compatible: {'yes' if compatible else 'NO — re-match all functions after rename'}")
    if not compatible:
        suggestions = suggest_same_length_names(args.old_type, args.new_type)
        if suggestions:
            print(f"same-length alternative: {suggestions[0]!r}")
    print(f"symbol lines to change: {plan.change_count}")
    if args.verbose:
        print()
        for change in plan.changes[: args.limit]:
            print(f"[{change.region}:{change.line_no}]")
            print(f"  - {change.old_name}")
            print(f"  + {change.new_name}")
        if len(plan.changes) > args.limit:
            print(f"... {len(plan.changes) - args.limit} more")
    xref = build_xref(
        args.project_root,
        args.region,
        args.old_type,
        load_symbols(_symbols_path(args.project_root, args.region)),
    )
    if xref.source_files:
        print("\nSource files (use rename-all to apply):")
        for path in xref.source_files:
            print(f"  {path.relative_to(args.project_root)}")
    return 0


def _print_rename_edits(project_root: Path, edits, symbol_touched: list[Path], dry_run: bool) -> None:
    mode = "would update" if dry_run else "updated"
    if symbol_touched:
        print(f"{mode} symbols in {len(symbol_touched)} file(s):")
        for path in symbol_touched:
            print(f"  {path.relative_to(project_root)}")
    text_edits = [e for e in edits if e.replacements]
    if text_edits:
        print(f"\n{mode} {sum(e.replacements for e in text_edits)} line(s) in {len(text_edits)} text file(s):")
        for edit in text_edits:
            print(f"  {edit.path.relative_to(project_root)} ({edit.replacements})")
    rename_edits = [e for e in edits if e.renamed_to is not None]
    if rename_edits:
        label = "would rename" if dry_run else "renamed"
        print(f"\n{label} {len(rename_edits)} file(s):")
        for edit in rename_edits:
            print(f"  {edit.path.relative_to(project_root)} -> {edit.renamed_to.relative_to(project_root)}")


def cmd_rename_apply(args: argparse.Namespace) -> int:
    regions = [args.region] if args.region else list(REGIONS)
    if args.all:
        active_region = args.region or "us"
        edits, symbol_touched = apply_full_rename(
            args.project_root,
            args.old_type,
            args.new_type,
            regions,
            active_region,
            dry_run=args.dry_run,
            rename_files=not args.no_rename_files,
            include_symbols=True,
        )
        _print_rename_edits(args.project_root, edits, symbol_touched, args.dry_run)
        if args.dry_run:
            print("\nRe-run without --dry-run to write changes.")
        else:
            print("\nNext: python configure.py && ninja")
            print("Then: python tools/coop/run.py diff <unit> --symbol <mangled-symbol>")
        return 0

    count, touched = apply_rename(
        args.project_root,
        args.old_type,
        args.new_type,
        regions,
        dry_run=args.dry_run,
    )
    mode = "would update" if args.dry_run else "updated"
    print(f"{mode} {count} symbol lines in {len(touched)} file(s)")
    for path in touched:
        print(f"  {path.relative_to(args.project_root)}")
    if args.dry_run:
        print("\nRe-run without --dry-run to write changes.")
        print("Or use: rename-all (or rename-apply --all) to update source/configure/splits too.")
    else:
        print("\nNext: rename in source/headers/configure.py, or use rename-all next time.")
    return 0


def cmd_rename_all(args: argparse.Namespace) -> int:
    active_region = args.region or "us"
    regions = [args.region] if args.region else list(REGIONS)
    plan = plan_full_rename(args.project_root, active_region, args.old_type, args.new_type)
    compatible = rename_mangling_compatible(args.old_type, args.new_type)
    print(f"old:  {args.old_type} ({len(args.old_type)} chars)")
    print(f"new:  {args.new_type} ({len(args.new_type)} chars)")
    print(f"mangling-compatible: {'yes' if compatible else 'NO — re-match required after rename'}")
    print(f"symbol files: {len(plan.symbol_files)}")
    print(f"text files:   {len(plan.text_files)}")
    print(f"file renames: {len(plan.file_renames)}")
    if args.verbose:
        print("\nText files:")
        for path in plan.text_files:
            print(f"  {path.relative_to(args.project_root)}")
        if plan.file_renames:
            print("\nFile renames:")
            for old_path, new_path in plan.file_renames:
                print(f"  {old_path.relative_to(args.project_root)} -> {new_path.relative_to(args.project_root)}")
    if not compatible and not args.force:
        print("\nERROR: names differ in length; use --force or pick a same-length name.", file=sys.stderr)
        return 1
    edits, symbol_touched = apply_full_rename(
        args.project_root,
        args.old_type,
        args.new_type,
        regions,
        active_region,
        dry_run=args.dry_run,
        rename_files=not args.no_rename_files,
        include_symbols=True,
    )
    _print_rename_edits(args.project_root, edits, symbol_touched, args.dry_run)
    if args.dry_run:
        print("\nRe-run without --dry-run to apply.")
    else:
        print("\nNext: python configure.py && ninja")
    return 0


def main() -> int:
    parser = argparse.ArgumentParser(description="Symbol recovery helpers for Xenoblade decomp")
    parser.add_argument(
        "--project-root",
        type=Path,
        default=ROOT,
        help="Repository root (default: auto-detected)",
    )
    parser.add_argument(
        "--region",
        choices=REGIONS,
        default="us",
        help="Active region for reads (default: us)",
    )
    sub = parser.add_subparsers(dest="command", required=True)

    p_list = sub.add_parser("list", help="List UnkClass_/UnkStruct_/Class_* placeholder types")
    p_list.add_argument("--kind", choices=["UnkClass", "UnkStruct", "Class"])
    p_list.add_argument("--json", action="store_true")

    p_show = sub.add_parser("show", help="Show symbols for one placeholder type")
    p_show.add_argument("query", help="Type name or address suffix (e.g. 8045F564)")

    p_xref = sub.add_parser("xref", help="Cross-reference splits, source, and configure.py")
    p_xref.add_argument("query", help="Type name or address suffix")
    p_xref.add_argument("--json", action="store_true")

    p_demangle = sub.add_parser("demangle", help="Demangle one or more MWCC symbol names")
    p_demangle.add_argument("symbols", nargs="+")

    p_rtti = sub.add_parser("rtti", help="List __RTTI__ entries (optional filter)")
    p_rtti.add_argument("query", nargs="?", default="")

    p_plan = sub.add_parser("rename-plan", help="Preview a placeholder type rename in symbols.txt")
    p_plan.add_argument("old_type", help="e.g. UnkClass_8045F564")
    p_plan.add_argument("new_type", help="e.g. CLibLayoutRegion")
    p_plan.add_argument("--verbose", action="store_true")
    p_plan.add_argument("--limit", type=int, default=20)

    p_apply = sub.add_parser("rename-apply", help="Apply rename to config/<region>/symbols.txt")
    p_apply.add_argument("old_type")
    p_apply.add_argument("new_type")
    p_apply.add_argument("--dry-run", action="store_true", help="Preview without writing")
    p_apply.add_argument("--region", choices=REGIONS, help="Single region (default: all)")
    p_apply.add_argument(
        "--all",
        action="store_true",
        help="Also update source, configure.py, splits, ownership, and rename matching .cpp/.hpp files",
    )
    p_apply.add_argument(
        "--no-rename-files",
        action="store_true",
        help="With --all: replace text only, do not rename UnkClass_*.cpp/.hpp files",
    )

    p_all = sub.add_parser(
        "rename-all",
        help="Apply full rename: symbols + source + configure + splits + file renames",
    )
    p_all.add_argument("old_type")
    p_all.add_argument("new_type")
    p_all.add_argument("--dry-run", action="store_true", help="Preview without writing")
    p_all.add_argument("--region", choices=REGIONS, help="Single region for symbol maps (default: all)")
    p_all.add_argument(
        "--no-rename-files",
        action="store_true",
        help="Replace text only; keep UnkClass_*.cpp/.hpp filenames",
    )
    p_all.add_argument("--verbose", action="store_true", help="List every file in the plan")
    p_all.add_argument(
        "--force",
        action="store_true",
        help="Proceed even when old/new names differ in length (requires re-match)",
    )

    p_cmp = sub.add_parser("compare-regions", help="Count symbol hits per region")
    p_cmp.add_argument("query")

    args = parser.parse_args()
    if args.command == "list":
        return cmd_list(args)
    if args.command == "show":
        return cmd_show(args)
    if args.command == "xref":
        return cmd_xref(args)
    if args.command == "demangle":
        return cmd_demangle(args)
    if args.command == "rtti":
        return cmd_rtti(args)
    if args.command == "rename-plan":
        return cmd_rename_plan(args)
    if args.command == "rename-apply":
        return cmd_rename_apply(args)
    if args.command == "rename-all":
        return cmd_rename_all(args)
    if args.command == "compare-regions":
        placeholder = args.query
        counts = compare_regions(args.project_root, placeholder, REGIONS)
        for region, count in counts.items():
            print(f"{region}: {count if count >= 0 else 'missing'}")
        return 0
    parser.error(f"unknown command: {args.command}")
    return 2


if __name__ == "__main__":
    raise SystemExit(main())
