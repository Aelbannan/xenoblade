#!/usr/bin/env python3
"""Infer which unit owns each retail data symbol from the retail relocations.

A translation unit's data sections (.rodata/.data/.bss/.sdata/.sbss/.sdata2/
.sbss2) are laid out in link order, the same order as its .text.  A data symbol
whose only referrers are one unit's code is evidence that the symbol lives in
that unit's slice.  This tool collects that evidence from the dtk-split retail
objects, keeps the largest set of votes that is consistent with link order,
and reports where config/<region>/splits.txt disagrees.

    section_owners.py                 summary per section
    section_owners.py --unit <name>   evidence for one unit
    section_owners.py --json <path>   full machine-readable result
    section_owners.py --plan [--tier A,B] [--apply]
                                      split range edits implied by trusted votes
    section_owners.py --shared        literals shared by neighbouring units

Votes are evidence, not proof - a global defined at the end of unit A and used
only by unit A+1 is indistinguishable from a static of unit A+1.  Votes are
therefore tiered (see tier()): only anonymous symbols move a boundary, and
named symbols never do.  Only --plan --apply edits splits.txt.
"""
from __future__ import annotations

import argparse
import bisect
import collections
import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))

from elftools.elf.elffile import ELFFile  # noqa: E402

from tools.coop.lib import weak_dedup as W  # noqa: E402

DATA_SECTIONS = (".rodata", ".data", ".bss", ".sdata", ".sbss", ".sdata2", ".sbss2")
SYM_RE = re.compile(r"^(\S+) = (\S+?):0x([0-9A-Fa-f]+); // (.*)$")
SIZE_RE = re.compile(r"size:0x([0-9A-Fa-f]+)")


def load_symbols(region: str):
    """Return {section: sorted [(addr, size, name)]} and {name: addr}."""
    by_sec: dict[str, list] = collections.defaultdict(list)
    addr_of: dict[str, int] = {}
    for line in (ROOT / "config" / region / "symbols.txt").read_text().splitlines():
        m = SYM_RE.match(line)
        if not m:
            continue
        name, sec, addr = m.group(1), m.group(2), int(m.group(3), 16)
        sm = SIZE_RE.search(m.group(4))
        by_sec[sec].append((addr, int(sm.group(1), 16) if sm else 0, name))
        addr_of[name] = addr
    for v in by_sec.values():
        v.sort()
    return by_sec, addr_of


def collect(region: str):
    """Scan every retail object; return (ranges, units, refs).

    refs: {target_addr: [(src_unit, src_section, src_addr)]}
    """
    ranges, _ = W._layout(ROOT, region)
    by_sec, addr_of = load_symbols(region)
    start_of = {(u, s): a for u, s, a, _ in ranges}
    refs: dict[int, list] = collections.defaultdict(list)
    units = []
    for u in json.loads((ROOT / "objdiff.json").read_text())["units"]:
        tp = u.get("target_path")
        if not tp or not (ROOT / tp).is_file():
            continue
        split = W.resolve_unit(ranges, u["name"])
        if not split:
            continue
        units.append((split, u))
        with open(ROOT / tp, "rb") as fh:
            elf = ELFFile(fh)
            secs = list(elf.iter_sections())
            symtab = elf.get_section_by_name(".symtab")
            if symtab is None:
                continue
            syms = list(symtab.iter_symbols())
            for rs in secs:
                if rs["sh_type"] != "SHT_RELA":
                    continue
                src_sec = secs[rs["sh_info"]].name
                src_base = start_of.get((split, src_sec))
                for r in rs.iter_relocations():
                    sym = syms[r["r_info_sym"]]
                    shndx = sym["st_shndx"]
                    if shndx == "SHN_UNDEF":
                        tgt = addr_of.get(sym.name)
                    elif isinstance(shndx, int):
                        base = start_of.get((split, secs[shndx].name))
                        if base is None:
                            continue
                        tgt = base + sym["st_value"]
                        if sym["st_info"]["type"] == "STT_SECTION":
                            tgt += r["r_addend"]
                    else:
                        continue
                    if tgt is None:
                        continue
                    refs[tgt].append((split, src_sec, (src_base or 0) + r["r_offset"]))
    return ranges, units, refs, by_sec


def text_order(ranges) -> dict[str, int]:
    order = sorted((a, u) for u, s, a, _ in ranges if s == ".text")
    return {u: i for i, (_, u) in enumerate(order)}


def owner_table(ranges, section: str):
    rs = sorted((a, b, u) for u, s, a, b in ranges if s == section)
    starts = [a for a, _, _ in rs]

    def owner(addr: int):
        i = bisect.bisect_right(starts, addr) - 1
        if i >= 0 and rs[i][0] <= addr < rs[i][1]:
            return rs[i][2]
        return None

    return owner


def consistent(votes: list[tuple[int, int]]) -> list[bool]:
    """Longest non-decreasing subsequence over (addr-ordered) unit indices."""
    tails: list[int] = []  # tails[k] = smallest last value of a run of length k+1
    tail_idx: list[int] = []
    prev = [-1] * len(votes)
    for i, (_, v) in enumerate(votes):
        k = bisect.bisect_right(tails, v)
        if k == len(tails):
            tails.append(v)
            tail_idx.append(i)
        else:
            tails[k] = v
            tail_idx[k] = i
        prev[i] = tail_idx[k - 1] if k else -1
    keep = [False] * len(votes)
    i = tail_idx[-1] if tail_idx else -1
    while i >= 0:
        keep[i] = True
        i = prev[i]
    return keep


def infer(region: str = "us"):
    ranges, units, refs, by_sec = collect(region)
    order = text_order(ranges)
    result = {}
    inferred_owner: dict[int, str] = {}
    for pass_no in (1, 2):
        for sec in DATA_SECTIONS:
            cur_owner = owner_table(ranges, sec)
            rows = []
            for addr, size, name in by_sec.get(sec, []):
                rr = refs.get(addr, [])
                text_units = {u for u, s, _ in rr if s == ".text"}
                vote, kind = None, None
                if len(text_units) == 1:
                    vote, kind = next(iter(text_units)), "text"
                elif not text_units and pass_no == 2:
                    # Referenced only from data (vtable -> RTTI, pointer tables):
                    # the referrer's inferred owner carries over.
                    owners = {inferred_owner.get(a) for _, s, a in rr if s in DATA_SECTIONS}
                    owners.discard(None)
                    if len(owners) == 1:
                        vote, kind = next(iter(owners)), "data"
                rows.append({"addr": addr, "size": size, "name": name, "vote": vote, "kind": kind,
                             "cur": cur_owner(addr), "shared": len(text_units) > 1})
            voted = [(i, order[r["vote"]]) for i, r in enumerate(rows)
                     if r["vote"] is not None and r["vote"] in order]
            keep = consistent([(i, v) for i, v in voted])
            for (i, _), k in zip(voted, keep):
                rows[i]["ok"] = k
            result[sec] = rows
            for r in rows:
                if r.get("ok"):
                    inferred_owner[r["addr"]] = r["vote"]
                elif r["cur"] and r["addr"] not in inferred_owner:
                    inferred_owner.setdefault(r["addr"], None)
    # Data-only referrers resolve against symbol start addresses; map reloc
    # source addresses to their containing symbol for the second pass.
    return ranges, units, result


ANON_RE = re.compile(r"(lbl_|@)")


def symbol_meta(region: str) -> dict[int, tuple[str, str]]:
    out = {}
    for line in (ROOT / "config" / region / "symbols.txt").read_text().splitlines():
        m = SYM_RE.match(line)
        if m:
            out[int(m.group(3), 16)] = (m.group(1), m.group(4))
    return out


def tier(sec: str, row: dict, meta: dict[int, tuple[str, str]]) -> str | None:
    """How far a vote can be trusted.

    A  anonymous small-data literals, jump tables, anonymous strings: the
       compiler emits these next to the code that uses them, so a single text
       referrer is the owner.
    B  other anonymous symbols (statics, tables).  Usually right, but the
       owning source must then define them, so each move needs follow-up.
    C  named symbols.  Never evidence: a global is routinely defined in one TU
       and used only by another (ml::huge, __EXIFreq, bd_addr_null).
    """
    if row.get("kind") != "text":
        return None
    name, info = meta[row["addr"]]
    if not ANON_RE.match(name) and not name.startswith("jumptable_"):
        return "C"
    if sec in (".sdata2", ".sbss2") or name.startswith("jumptable_"):
        return "A"
    if "data:string" in info:
        return "A"
    return "B"


def disagreements(region: str = "us", tiers=("A", "B")):
    """Yield (section, row, tier) for trusted votes that splits.txt contradicts."""
    _, _, result = infer(region)
    meta = symbol_meta(region)
    for sec, rows in result.items():
        for r in rows:
            if r.get("ok") and r["cur"] != r["vote"]:
                t = tier(sec, r, meta)
                if t in tiers:
                    yield sec, r, t


def _runs(rows, key) -> set:
    """Units that own more than one separate run of rows."""
    seen, bad, last = set(), set(), None
    for r in rows:
        f = r[key]
        if f != last:
            if f is not None and f in seen:
                bad.add(f)
            if f is not None:
                seen.add(f)
            last = f
    return bad


def plan(region: str = "us", tiers=("A",)):
    """Turn trusted votes into new split ranges.

    Returns (edits, conflicts, stats): edits maps (unit, section) to the new
    (start, end) or None (range removed).  A unit whose slice would stop being
    contiguous is dropped from the plan and listed in conflicts.
    """
    ranges, _, result = infer(region)
    meta = symbol_meta(region)
    cur_rng = {(u, s): (a, b) for u, s, a, b in ranges}
    edits: dict = {}
    conflicts = []
    stats: collections.Counter = collections.Counter()
    for sec, rows in result.items():
        for r in rows:
            r["t"] = tier(sec, r, meta) if r.get("ok") and r["vote"] != r["cur"] else None
        banned: set = set()
        pre = _runs(rows, "cur")
        while True:
            for r in rows:
                r["final"] = r["cur"]
                r.pop("moved", None)
                if (r["t"] in tiers and r["vote"] not in banned and r["cur"] not in banned):
                    r["final"], r["moved"] = r["vote"], True
            # Unreferenced symbols between two symbols of one owner follow it.
            idx = [i for i, r in enumerate(rows) if r.get("ok") and r["vote"] == r["final"]]
            for a, b in zip(idx, idx[1:]):
                owner = rows[a]["final"]
                if rows[b]["final"] == owner and owner not in banned:
                    for k in range(a + 1, b):
                        if rows[k]["final"] != owner and rows[k]["cur"] not in banned:
                            rows[k]["final"], rows[k]["moved"] = owner, True
                            rows[k]["t"] = rows[k]["t"] or "fill"
            bad = _runs(rows, "final") - pre
            if not bad - banned:
                break
            banned |= bad
        if banned:
            conflicts.append((sec, sorted(banned)))
        for r in rows:
            if r.get("moved"):
                stats[(sec, r["t"])] += 1
        changed = {r["final"] for r in rows if r.get("moved")}
        changed |= {r["cur"] for r in rows if r.get("moved") and r["cur"]}
        for u in changed - banned - {None}:
            mine = [i for i, r in enumerate(rows) if r["final"] == u]
            if not mine:
                edits[(u, sec)] = None
                continue
            first, last = rows[mine[0]], rows[mine[-1]]
            start, end = first["addr"], last["addr"] + last["size"]
            nxt = rows[mine[-1] + 1]["addr"] if mine[-1] + 1 < len(rows) else None
            old = cur_rng.get((u, sec))
            if nxt is not None and nxt - end < 8:
                end = nxt
            elif old and old[1] >= end and (nxt is None or old[1] <= nxt):
                end = old[1]
            else:
                end = (end + 3) & ~3
            prev_end = rows[mine[0] - 1]["addr"] + rows[mine[0] - 1]["size"] if mine[0] else 0
            if old and old[0] < start and prev_end <= old[0]:
                start = old[0]  # only leading padding differs
            if old != (start, end):
                edits[(u, sec)] = (start, end)
    return edits, conflicts, stats


SPLIT_LINE = re.compile(r"(\s+)(\S+)(\s+)start:0x([0-9A-Fa-f]+) end:0x([0-9A-Fa-f]+)(.*)$")


def apply_plan(region: str, edits: dict) -> int:
    """Rewrite config/<region>/splits.txt.  Returns the number of ranges changed."""
    ranges, _ = W._layout(ROOT, region)
    existing = {(u, s) for u, s, _, _ in ranges}
    path = ROOT / "config" / region / "splits.txt"
    out: list[str] = []
    done: set = set()
    cur = None

    def flush():
        if cur is None:
            return
        for (u, s), v in edits.items():
            if u == cur and v and (u, s) not in done and (u, s) not in existing:
                j = len(out)
                while j > 0 and not out[j - 1].strip():
                    j -= 1
                out.insert(j, "\t%s        start:0x%08X end:0x%08X" % (s, v[0], v[1]))
                done.add((u, s))

    for line in path.read_text().split("\n"):
        if line and not line[0].isspace() and line.rstrip().endswith(":"):
            flush()
            cur = line.rstrip()[:-1]
        m = SPLIT_LINE.match(line)
        if m and cur and (cur, m.group(2)) in edits:
            v = edits[(cur, m.group(2))]
            done.add((cur, m.group(2)))
            if v is None:
                continue
            line = "%s%s%sstart:0x%08X end:0x%08X%s" % (m.group(1), m.group(2), m.group(3), v[0], v[1], m.group(6))
        out.append(line)
    flush()
    path.write_text("\n".join(out))
    return len(done)


def shared_literals(region: str = "us"):
    """Signs that the text split itself is wrong.

    An anonymous small-data literal belongs to exactly one translation unit, so
    one referenced from several units means either a named constant that lost
    its name or one source file split into several units.  Returns
    {frozenset(units): [addresses]} for adjacent-in-link-order groups, plus the
    units whose .sdata2 slice does not start 8-aligned.
    """
    ranges, _, refs, by_sec = collect(region)
    meta = symbol_meta(region)
    order = text_order(ranges)
    groups: dict = collections.defaultdict(list)
    for sec in (".sdata2", ".sbss2"):
        for addr, _size, name in by_sec.get(sec, []):
            if not ANON_RE.match(name):
                continue
            us = {u for u, s, _ in refs.get(addr, []) if s == ".text" and u in order}
            if len(us) > 1:
                idx = sorted(order[u] for u in us)
                if idx[-1] - idx[0] <= len(idx) + 2:  # neighbours in link order
                    groups[frozenset(us)].append(addr)
    unaligned = sorted((a, u) for u, s, a, _ in ranges if s == ".sdata2" and a % 8)
    return groups, unaligned


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--region", default="us")
    ap.add_argument("--unit", help="show evidence for one unit (substring match)")
    ap.add_argument("--json", help="write full result to this path")
    ap.add_argument("--plan", action="store_true", help="print the split range edits the trusted votes imply")
    ap.add_argument("--tier", default="A", help="comma-separated tiers for --plan (A, B); default A")
    ap.add_argument("--apply", action="store_true", help="with --plan: rewrite splits.txt")
    ap.add_argument("--shared", action="store_true",
                    help="list literals shared by neighbouring units (text split suspects)")
    args = ap.parse_args()
    if args.shared:
        groups, unaligned = shared_literals(args.region)
        for us, addrs in sorted(groups.items(), key=lambda kv: -len(kv[1])):
            print(f"{len(addrs):4} shared literal(s): " + ", ".join(sorted(us)))
        print(f"\n{len(groups)} group(s); {len(unaligned)} unit(s) with an unaligned .sdata2 start")
        for a, u in unaligned:
            print(f"  0x{a:08X} {u}")
        return 0
    if args.plan:
        edits, conflicts, stats = plan(args.region, tuple(args.tier.split(",")))
        for (u, s), v in sorted(edits.items(), key=lambda kv: (kv[0][1], kv[1] or (0, 0))):
            print(f"  {s:8} {u}: " + ("remove" if v is None else f"0x{v[0]:08X}-0x{v[1]:08X}"))
        for sec, bad in conflicts:
            print(f"  conflict in {sec} (left alone): " + ", ".join(bad))
        print(f"{len(edits)} range edit(s); symbols moved: "
              + ", ".join(f"{s}/{t} {n}" for (s, t), n in sorted(stats.items())))
        if args.apply:
            print(f"applied {apply_plan(args.region, edits)} range edit(s) to config/{args.region}/splits.txt")
        return 0
    ranges, units, result = infer(args.region)
    moves = collections.Counter()
    for sec, rows in result.items():
        n_vote = sum(1 for r in rows if r.get("ok"))
        wrong = [r for r in rows if r.get("ok") and r["cur"] != r["vote"]]
        unowned = sum(1 for r in wrong if r["cur"] is None)
        dropped = sum(1 for r in rows if r["vote"] and not r.get("ok"))
        print(f"{sec:8} symbols {len(rows):6}  consistent votes {n_vote:6}  "
              f"disagree with splits {len(wrong):5} (unowned today {unowned})  order-inconsistent {dropped}")
        for r in wrong:
            moves[(sec, r["cur"], r["vote"])] += 1
    print(f"\n{len(moves)} (section, current owner -> evidenced owner) pairs")
    if args.unit:
        for sec, rows in result.items():
            for r in rows:
                if r.get("ok") and r["cur"] != r["vote"] and (
                        args.unit in (r["vote"] or "") or args.unit in (r["cur"] or "")):
                    print(f"  {sec:8} 0x{r['addr']:08X} {r['name']:28} {r['cur']} -> {r['vote']} ({r['kind']})")
    if args.json:
        Path(args.json).write_text(json.dumps(result, indent=1))
    return 0


if __name__ == "__main__":
    sys.exit(main())
