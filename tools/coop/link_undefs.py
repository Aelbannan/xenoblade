#!/usr/bin/env python3
"""Link-graph undefined symbols: the *uncapped* main.elf frontier.

mwldeppc stops after a fixed number of undefined-symbol errors, so
``ninja build/us/main.elf`` only ever shows the first ~100 error blocks.  This
tool answers the real question offline: for the current response file, which
symbols are referenced (strong ``U``) by at least one linked object and defined
by none, and which retail split owns each of them.

Usage:
  python3 tools/coop/link_undefs.py                 # summary + top owner units
  python3 tools/coop/link_undefs.py --list          # every missing symbol
  python3 tools/coop/link_undefs.py --json out.json # machine-readable
  python3 tools/coop/link_undefs.py --top 40

Stdlib only (plus the shared linker-script name parser).  Reads
``build/<region>/main.elf.rsp`` (regenerate it with one
``ninja build/<region>/main.elf`` run first), ``build/<region>/ldscript.lcf``,
and ``config/<region>/{symbols,splits}.txt``.
"""
from __future__ import annotations

import argparse
import collections
import json
import pathlib
import re
import subprocess
import sys

ROOT = pathlib.Path(__file__).resolve().parents[2]
NM = ROOT / "build" / "binutils" / "powerpc-eabi-nm"

if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from tools.coop.lib.reloc_canon import ldscript_assigned_names  # noqa: E402

#: Absolutes mwldeppc synthesizes itself (EABI small-data bases), never
#: defined by any object: `-lcf` changes their *value* (the script drives the
#: layout they derive from) but they resolve either way, so they are
#: link-provided rather than missing. See the with/without-`-lcf` micro-link
#: comparison in docs/evidence/decomp/unit_rules_category_e_pool_labels.md.
EABI_LINKER_SYMBOLS = frozenset({"_SDA_BASE_", "_SDA2_BASE_"})


def ldscript_provided(region: str) -> tuple[set[str], str]:
    """(linker-provided names, lcf path) for one region.

    dtk writes the retail-accurate ``build/<region>/ldscript.lcf``; the build
    tree's copy always wins when it exists (same policy as reloc_canon).
    """
    lcf = ROOT / "build" / region / "ldscript.lcf"
    if not lcf.is_file():
        return set(EABI_LINKER_SYMBOLS), "<eabi built-ins only>"
    return set(ldscript_assigned_names(lcf)) | EABI_LINKER_SYMBOLS, str(lcf)


def split_missing(
    undef: set[str], defined: set[str], provided: set[str]
) -> tuple[list[str], list[str]]:
    """(object-level missing, linker-provided references) - the frontier split.

    A reference counts as object-level work only when no object defines it and
    no linker script/EABI built-in provides it.
    """
    linker = sorted(n for n in undef if n in provided and n not in defined)
    missing = sorted(set(undef) - defined - set(linker))
    return missing, linker


def object_symbols(path: pathlib.Path) -> tuple[set[str], dict[str, set[str]]]:
    """(defined names, undefined name -> referring objects) for one object."""
    defined: set[str] = set()
    undef: dict[str, set[str]] = {}
    for flag, is_defined in (("--defined-only", True), ("-u", False)):
        try:
            out = subprocess.run(
                [str(NM), "-A", flag, str(path)],
                capture_output=True, text=True, timeout=120,
            ).stdout
        except Exception:
            continue
        for line in out.split("\n"):
            if ":" not in line:
                continue
            fields = line.rsplit(":", 1)[1].split()
            if not fields:
                continue
            if is_defined:
                if fields[0] != "U":
                    defined.add(fields[-1])
            elif fields[0] == "U" and len(fields) >= 2:
                undef.setdefault(fields[-1], set()).add(str(path))
    return defined, undef


def _retail_symbols(region: str) -> dict[str, tuple[str, int]]:
    ent: dict[str, tuple[str, int]] = {}
    path = ROOT / "config" / region / "symbols.txt"
    if not path.exists():
        return ent
    for line in path.read_text(errors="replace").split("\n"):
        if " = " not in line or ";" not in line:
            continue
        name, rest = line.split(" = ", 1)
        try:
            sec, addr = rest.split(";")[0].split(":")
            ent[name.strip()] = (sec.strip(), int(addr, 16))
        except ValueError:
            continue
    return ent


def _split_ranges(region: str) -> dict[str, list[tuple[str, int]]]:
    ranges: dict[str, list[tuple[str, int]]] = {}
    unit = None
    path = ROOT / "config" / region / "splits.txt"
    if not path.exists():
        return ranges
    for line in path.read_text(errors="replace").split("\n"):
        if not line.startswith(("\t", " ")) and line.rstrip().endswith(":"):
            unit = line.strip()[:-1]
            continue
        fields = line.split()
        if len(fields) >= 2 and fields[1].startswith("start:") and unit:
            try:
                start = int(fields[1].split(":")[1], 16)
            except ValueError:
                continue
            ranges.setdefault(unit, []).append((fields[0], start))
    return ranges


def owner_of(name: str, symbols: dict, ranges: dict) -> str | None:
    if name not in symbols:
        return None
    sec, addr = symbols[name]
    best: tuple[int, str] | None = None
    for unit, entries in ranges.items():
        for s, start in entries:
            if s == sec and start <= addr and (best is None or start > best[0]):
                best = (start, unit)
    return best[1] if best else None


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--region", default="us")
    ap.add_argument("--rsp", default=None, help="response file (default build/<region>/main.elf.rsp)")
    ap.add_argument("--top", type=int, default=25)
    ap.add_argument("--list", action="store_true", help="print every missing symbol")
    ap.add_argument("--json", default=None, help="write the full result as JSON")
    args = ap.parse_args()

    rsp = pathlib.Path(args.rsp) if args.rsp else ROOT / "build" / args.region / "main.elf.rsp"
    if not rsp.exists():
        print(f"ERROR: {rsp} not found - run `ninja build/{args.region}/main.elf` once", file=sys.stderr)
        return 2
    provided, lcf_source = ldscript_provided(args.region)
    objects = [l for l in rsp.read_text().split("\n") if l.strip()]

    defined: set[str] = set()
    undef: dict[str, set[str]] = {}
    for obj in objects:
        d, u = object_symbols(ROOT / obj if not pathlib.Path(obj).is_absolute() else pathlib.Path(obj))
        defined |= d
        for name, objs in u.items():
            undef.setdefault(name, set()).update(objs)

    missing, linker = split_missing(set(undef), defined, provided)
    symbols = _retail_symbols(args.region)
    ranges = _split_ranges(args.region)
    groups: collections.Counter = collections.Counter()
    rows = []
    for name in missing:
        owner = owner_of(name, symbols, ranges)
        if owner:
            groups[owner] += 1
        rows.append({"symbol": name, "owner": owner, "referrers": sorted(undef[name])})

    print(f"objects: {len(objects)}  defined names: {len(defined)}  undefined names: {len(undef)}")
    print(f"MISSING (referenced, defined nowhere in any object): {len(missing)}")
    if linker:
        print(
            f"linker-provided (not object work): {len(linker)} -> "
            f"{', '.join(linker)}  [{lcf_source}]"
        )
    print(f"\nmissing symbols by retail owner unit (top {args.top}):")
    for unit, count in groups.most_common(args.top):
        print(f"  {count:4d}  {unit}")
    unattributed = len(missing) - sum(groups.values())
    if unattributed:
        print(f"\nunattributed (no retail symbol entry): {unattributed}")
    if args.list:
        print()
        for row in rows:
            print(f"{row['symbol']}\t{row['owner'] or '?'}")
        if linker:
            print(f"\n-- provided by the linker script (not object work): {lcf_source}")
            for name in linker:
                print(f"{name}\tlinker")
    if args.json:
        pathlib.Path(args.json).write_text(
            json.dumps(
                {
                    "missing": rows,
                    "linker_provided": linker,
                    "lcf_source": lcf_source,
                },
                indent=1,
            )
            + "\n"
        )
        print(f"\nwrote {args.json}")
    return 0 if not missing else 1


if __name__ == "__main__":
    raise SystemExit(main())
