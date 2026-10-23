#!/usr/bin/env python3
"""Declarations-only checker for code-only units (PLAN.md §17.7).

A unit labelled ``Object(MatchingCodeOnly, ...)`` in ``configure.py`` links its
matched code while its data is owned elsewhere (another TU or a shared data
range).  The source invariant: **foreign data is only ``extern``-declared; a TU
that defines data owns it.**  Concretely, for every data section whose retail
split slice is empty (the unit owns nothing there), the RAW source-built object
must not define any bytes in that section.

Per unit, comparing ``build/<region>/obj/<stem>.o`` (retail split) with the raw
``build/<region>/src/<stem>.o`` (never the ``*.reloc.o`` link copy):

* VIOLATION  raw object defines data in a section the retail split leaves empty,
             and no UNIT_RULES ``extern_data_sections`` entry covers it -> exit 1;
* TRANSITIONAL same, but a (frozen, shrink-only) EDS entry still strips it at
             link — reported, allowed; retiring the EDS entry makes it a VIOLATION;
* PROMOTE    raw ``run.py data diff`` would already MATCH — the label is stale,
             relabel the unit ``Matching`` (``--strict`` turns this into a failure).

Missing objects fail unless ``--allow-missing`` (CI builds ``all_source`` first).

Usage:
    python3 tools/coop/code_only_check.py [--region us] [--strict] [--allow-missing]
"""

from __future__ import annotations

import argparse
import sys
from dataclasses import dataclass, field
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from tools.coop.lib.code_only import load_code_only_units, object_stem  # noqa: E402
from tools.coop.lib.data_match import (  # noqa: E402
    DATA_SECTIONS,
    ParsedObject,
    _parse,
    check_data_sections,
)


@dataclass
class UnitVerdict:
    unit: str
    violations: list[str] = field(default_factory=list)
    transitional: list[str] = field(default_factory=list)
    promote: bool = False
    missing: str = ""


def _size(parsed: ParsedObject, sec: str) -> int:
    return parsed.sections.get(sec, {}).get("size", 0)


def defined_in(parsed: ParsedObject, sec: str) -> list[str]:
    idx = {i for i, n in parsed.section_names.items() if n == sec}
    return sorted(n for n, (shndx, _v) in parsed.symbols.items() if shndx in idx)


def foreign_definitions(
    retail: ParsedObject, decomp: ParsedObject, eds_sections: tuple[str, ...] = ()
) -> tuple[list[str], list[str]]:
    """(violations, transitional) for one unit; pure, used by the tests."""
    violations: list[str] = []
    transitional: list[str] = []
    for sec in DATA_SECTIONS:
        dsz = _size(decomp, sec)
        if dsz == 0 or _size(retail, sec) != 0:
            continue
        syms = defined_in(decomp, sec)
        shown = ", ".join(syms[:6]) + (f", … (+{len(syms) - 6})" if len(syms) > 6 else "")
        msg = f"{sec}: defines 0x{dsz:X} bytes the retail split does not own [{shown}]"
        (transitional if sec in eds_sections else violations).append(msg)
    return violations, transitional


def _eds_sections(decomp_obj: Path) -> tuple[str, ...]:
    from tools.postprocess_reloc_names import select_unit_rules

    rules = select_unit_rules(decomp_obj)
    eds = getattr(rules, "extern_data_sections", ()) if rules else ()
    return tuple(eds) if isinstance(eds, tuple) else ()


def check_unit(unit: str, region: str, root: Path = ROOT) -> UnitVerdict:
    stem = object_stem(unit)
    retail_obj = root / "build" / region / "obj" / f"{stem}.o"
    decomp_obj = root / "build" / region / "src" / f"{stem}.o"
    verdict = UnitVerdict(unit)
    for p in (retail_obj, decomp_obj):
        if not p.is_file():
            verdict.missing = str(p.relative_to(root))
            return verdict
    retail, decomp = _parse(retail_obj), _parse(decomp_obj)
    verdict.violations, verdict.transitional = foreign_definitions(
        retail, decomp, _eds_sections(decomp_obj)
    )
    verdict.promote = check_data_sections(retail_obj, decomp_obj).ok
    return verdict


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--region", default="us")
    parser.add_argument("--strict", action="store_true", help="fail on stale code-only labels too")
    parser.add_argument("--allow-missing", action="store_true", help="skip units whose objects are not built")
    args = parser.parse_args(argv)

    units = load_code_only_units(ROOT)
    failed = 0
    for unit in units:
        v = check_unit(unit, args.region)
        if v.missing:
            print(f"{'SKIP' if args.allow_missing else 'FAIL'} {unit}: missing {v.missing}")
            failed += 0 if args.allow_missing else 1
            continue
        for msg in v.violations:
            print(f"FAIL {unit}: {msg} — declare it extern, the owning TU defines it")
        for msg in v.transitional:
            print(f"TRANSITIONAL {unit}: {msg} (EDS still strips it at link)")
        if v.promote:
            print(f"{'FAIL' if args.strict else 'NOTE'} {unit}: raw data diff MATCHes — relabel Matching")
        failed += bool(v.violations) or (args.strict and v.promote)
    print(f"code-only check: {len(units)} unit(s), {failed} failing")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
