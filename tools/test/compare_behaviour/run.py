#!/usr/bin/env python3
"""
Compare retail vs decompiled object behaviour.

  python tools/test/compare_behaviour/run.py list
  python tools/test/compare_behaviour/run.py audit
  python tools/test/compare_behaviour/run.py compare game-set-view-rect
  python tools/test/compare_behaviour/run.py compare --all
  python tools/test/compare_behaviour/run.py static game-set-view-rect
  python tools/test/compare_behaviour/run.py ppc view-rect-data-clamp
"""

from __future__ import annotations

import argparse
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from tools.coop.lib.object_size import format_size_check
from tools.test.compare_behaviour.lib.audit import audit_all
from tools.test.compare_behaviour.lib.manifest import get_test, load_manifest
from tools.test.compare_behaviour.lib.ppc_runner import run_ppc_compare
from tools.test.compare_behaviour.lib.static_compare import run_static_compare


def cmd_list() -> int:
    _, tests = load_manifest()
    for test in tests:
        ppc = test.ppc_source or "-"
        print(f"{test.id:28} unit={test.unit} ppc={ppc}")
        print(f"{'':28} {test.description}")
    return 0


def cmd_audit() -> int:
    rows = audit_all()
    ok = True
    for row in rows:
        pct = "n/a" if row.match_percent is None else f"{row.match_percent:.1f}%"
        status = "PASS" if row.ok else "FAIL"
        if not row.ok:
            ok = False
        ppc = row.ppc_source or "-"
        print(f"{status} {row.test_id:28} match={pct:>7} ppc={ppc:28}  {row.notes}")
    return 0 if ok else 1


def _print_static(test_id: str) -> tuple[bool, float | None]:
    _, tests = load_manifest()
    test = get_test(tests, test_id)
    result = run_static_compare(test)
    pct = "n/a" if result.match_percent is None else f"{result.match_percent:.1f}%"
    print(f"[static]  {test.id}")
    print(f"          retail: {result.retail_path}")
    print(f"          decomp: {result.decomp_path}")
    print(f"          symbol: {test.symbol}")
    print(f"          match:  {pct} ({result.status})")
    size_ok = True
    if result.size_check is not None:
        print(f"          {format_size_check(result.size_check)}")
        size_ok = result.size_check.ok
    return size_ok, result.match_percent


def _print_ppc(test_id: str) -> bool:
    _, tests = load_manifest()
    test = get_test(tests, test_id)
    if not test.ppc_source:
        print(f"[ppc]     {test.id}: skipped (no ppc_source in manifest)")
        return True

    result = run_ppc_compare(test)
    print(f"[ppc]     {test.id}")
    print(f"          dol:    {result.dol}")
    if result.dolphin:
        print(f"          runner: {result.dolphin} (--batch headless)")
    if result.skipped:
        print(f"          result: SKIP ({result.output})")
        return True
    if result.passed is not None:
        print(f"          passed: {result.passed} failed: {result.failed}")
    if result.failed_scenarios:
        print("          failed scenarios:")
        for name in result.failed_scenarios:
            print(f"            - {name}")
    if result.failure_details:
        print("          field diffs (retail/decomp per unk):")
        for detail in result.failure_details:
            print(f"            {detail}")
    elif result.last_fail:
        print(f"          failed scenarios: {result.last_fail}")
    if result.output and not result.failed_scenarios:
        for line in result.output.splitlines()[-4:]:
            if line.strip():
                print(f"          {line}")
    print(f"          result: {'PASS' if result.ok else 'FAIL'}")
    return result.ok


def cmd_static(test_ids: list[str]) -> int:
    ok = True
    for test_id in test_ids:
        _, tests = load_manifest()
        test = get_test(tests, test_id)
        static_ok, _ = _print_static(test_id)
        result = run_static_compare(test)
        ok = static_ok and (result.match_percent is not None and result.match_percent >= 100.0) and ok
    return 0 if ok else 1


def cmd_ppc(test_ids: list[str]) -> int:
    ok = True
    for test_id in test_ids:
        ok = _print_ppc(test_id) and ok
    return 0 if ok else 1


def cmd_compare(test_ids: list[str], *, static_only: bool, ppc_only: bool) -> int:
    ok = True
    for test_id in test_ids:
        print(f"=== {test_id} ===")
        if ppc_only:
            ok = _print_ppc(test_id) and ok
            print()
            continue

        static_ok, _ = _print_static(test_id)
        ok = static_ok and ok

        if not static_only:
            ok = _print_ppc(test_id) and ok
        print()
    return 0 if ok else 1


def main() -> int:
    parser = argparse.ArgumentParser(description="Retail vs decomp behaviour comparison")
    sub = parser.add_subparsers(dest="command", required=True)

    sub.add_parser("list", help="List registered behaviour tests")
    sub.add_parser("audit", help="Audit split-size budget for registered tests")

    p_static = sub.add_parser("static", help="objdiff retail .o vs decomp .o")
    p_static.add_argument("test_id", nargs="+")

    p_ppc = sub.add_parser("ppc", help="Headless Dolphin PPC dual-call tests")
    p_ppc.add_argument("test_id", nargs="*", help="Test id(s); omit with --all")
    p_ppc.add_argument("--all", action="store_true", help="Run every test with ppc_source")

    p_compare = sub.add_parser("compare", help="Run static + ppc checks")
    p_compare.add_argument("test_id", nargs="*", help="Test id(s); omit with --all")
    p_compare.add_argument("--all", action="store_true", help="Run every test in manifest")
    p_compare.add_argument("--static-only", action="store_true")
    p_compare.add_argument("--ppc-only", action="store_true")

    args = parser.parse_args()

    if args.command == "list":
        return cmd_list()

    if args.command == "audit":
        return cmd_audit()

    if args.command == "static":
        return cmd_static(args.test_id)

    if args.command == "ppc":
        if args.all:
            _, tests = load_manifest()
            test_ids = [t.id for t in tests if t.ppc_source]
        elif args.test_id:
            test_ids = args.test_id
        else:
            parser.error("ppc requires test_id(s) or --all")
        return cmd_ppc(test_ids)

    if args.command == "compare":
        if args.all:
            _, tests = load_manifest()
            test_ids = [t.id for t in tests]
        elif args.test_id:
            test_ids = args.test_id
        else:
            parser.error("compare requires test_id(s) or --all")
        return cmd_compare(
            test_ids,
            static_only=args.static_only,
            ppc_only=args.ppc_only,
        )

    return 1


if __name__ == "__main__":
    raise SystemExit(main())
