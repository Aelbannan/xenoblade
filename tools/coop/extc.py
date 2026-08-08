#!/usr/bin/env python3
"""
extc — extern "C" declaration classifier and member-conversion planner.

Why: the hand-written extern "C" pseudo-import pattern across src/libs is a
legacy smell (docs/CODE_SMELLS.md) and a drift source.  The retail symbol
table (config/<region>/symbols.txt) is the source of truth for what name each
retail reloc references.  This tool:

  scan                      — classify every extern "C" declaration in
                              src/** and libs/** against the retail table:
                              retail-exact / retail-drift / invented /
                              member-candidate (+ JP-stale-address notes)
  plan <class-or-token>     — member-conversion plan for extern "C"
                              functions that reference a class (void* self
                              free functions that should be real members):
                              retail rename target (MWCC member mangling),
                              call sites, affected TUs, ceremony checklist

Usage (from repository root):
  python3 tools/coop/run.py extc scan
  python3 tools/coop/run.py extc scan --json /tmp/extc.json
  python3 tools/coop/run.py extc plan CExchangeWin
  python3 tools/coop/run.py extc plan CExchangeWin --json /tmp/plan.json
"""

from __future__ import annotations

import argparse
import json
import re
import sys
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent.parent
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from tools.coop.lib.config import load_config  # noqa: E402
from tools.coop.lib.project import Project  # noqa: E402
from tools.symbolrecover.lib.parser import load_symbols  # noqa: E402

# ── scanner (declaration extraction, hardened) ─────────────────────────────

RE_EXT_C = re.compile(r'extern\s+"C"')
RE_DECLSPEC = re.compile(r'__declspec\s*\([^)]*\)')
RE_PTR_VAR = re.compile(r'\(\s*\*\s*(?:[A-Za-z_]\w*\s+)*([A-Za-z_]\w*)\s*\)')
RE_SKIP_DECL_PREFIX = re.compile(r'^(?:typedef|using)\b')
RE_ADDR_ANY = re.compile(r'_?([0-9A-Fa-f]{8})\b')
RE_HEXNAME = re.compile(r'^(?:func|lbl|data)_[0-9A-Fa-f]{8}$')
RE_EUNAME = re.compile(r'^(?:func|lbl|data)_eu_[0-9A-Fa-f]+$')
RE_FUNC_NAME = re.compile(r'([A-Za-z_]\w*)\s*\(')
RE_SELF_FIRST = re.compile(
    r'\(\s*(?:[A-Za-z_:<>]+|void)\s*\*\s*(?:self|_this|p)\b|\(\s*void\s*\*\s*self'
)
RE_CLASS_CAST = re.compile(r'\(\(\s*([A-Za-z_]\w*)\s*\*+\s*\)\s*(?:self|_this)\s*\)')


def strip_comment(line: str) -> str:
    line = re.sub(r'/\*.*?\*/', ' ', line)
    i = line.find('//')
    return line[:i] if i >= 0 else line


def name_from(body: str) -> str | None:
    """Extract the declared symbol name from a decl body (no extern "C" prefix)."""
    body = RE_DECLSPEC.sub('', body).strip().rstrip(';').rstrip()
    body = re.sub(r'\s+[A-Z_][A-Z0-9_]*$', '', body).rstrip()  # attribute macros
    body = re.sub(r'__attribute__\s*\(\([^)]*\)\)', '', body).strip()
    # pointer-to-function / pointer-to-member variable:
    # the FIRST '(' carries `[Class::]* [cv] name )` (e.g. `int (*cb)(...)`,
    # `int (CCtrlMovePC::*const lbl)(...)`) — otherwise the first '(' is the
    # argument list of a normal function declaration.
    m = re.match(
        r'^[^(\n]*\(\s*(?:[A-Za-z_]\w*\s*::\s*)?\*\s*(?:[A-Za-z_]\w*\s+)*'
        r'([A-Za-z_]\w*)\s*\)',
        body,
    )
    if m:
        return m.group(1)
    if body.endswith(')'):
        m = RE_FUNC_NAME.search(body)
        return m.group(1) if m else None
    m = re.search(r'([A-Za-z_]\w*)\s*(?:\[[^\]]*\])?$', body)
    return m.group(1) if m else None


def logical_line(lines: list[str], i: int) -> tuple[str, int, bool]:
    """Join continuation lines starting at i. Returns (text, end_idx, is_def)."""
    parts = [strip_comment(lines[i]).strip()]
    depth = parts[0].count('{') - parts[0].count('}')
    j = i
    while not parts[-1].rstrip().endswith(';') and depth <= 0 and j - i < 8:
        j += 1
        if j >= len(lines):
            break
        raw = lines[j]
        ln = strip_comment(raw).strip()
        if not ln or ln.startswith('#'):
            continue
        if not raw.startswith((' ', '\t')) and not parts[-1].rstrip().endswith(('\\', ',')):
            break  # col-0 line is a fresh statement, not a continuation
        parts.append(ln)
        depth += ln.count('{') - ln.count('}')
    return ' '.join(parts), j, '{' in ' '.join(parts)


def extract_entries(lines: list[str]):
    """Yield (name, kind, lineno, raw, body) for extern "C" decls/defs."""
    i, n = 0, len(lines)
    while i < n:
        line = strip_comment(lines[i]).strip()
        if not line:
            i += 1
            continue
        has_ext = bool(RE_EXT_C.search(line))
        rest0 = RE_EXT_C.sub('', line).strip()
        if has_ext and rest0 == '{':  # true `extern "C" {` block opener
            depth, j = 0, i
            first = True
            pending: list[str] = []
            pending_ln = 0
            pending_raw = ''
            while j < n:
                ln_raw = lines[j]
                ln = strip_comment(ln_raw)
                core = RE_EXT_C.sub('', ln).strip()
                if not first:
                    depth += ln.count('{') - ln.count('}')
                first = False
                if depth < 0:
                    break  # block close
                if depth != 0:
                    j += 1
                    continue  # inside a definition body
                if pending:
                    pending.append(core)
                    if pending[-1].rstrip().endswith(';'):
                        text = ' '.join(pending)
                        yield name_from(text), "decl", pending_ln, pending_raw, text
                        pending = []
                    j += 1
                    continue
                if core == '' or core.startswith('}') or core.startswith('#'):
                    j += 1
                    continue
                if core.endswith(';') and '{' not in core and '=' not in core and not RE_SKIP_DECL_PREFIX.search(core):
                    yield name_from(core), "decl", j + 1, ln_raw.strip(), core
                elif '{' not in core and '=' not in core and not RE_SKIP_DECL_PREFIX.search(core):
                    pending = [core]
                    pending_ln = j + 1
                    pending_raw = ln_raw.strip()
                j += 1
            i = j + 1
            continue
        if not has_ext:
            i += 1
            continue
        text, j, is_def = logical_line(lines, i)
        if is_def or not text.rstrip().endswith(';'):
            i = j + 1
            continue
        body = RE_EXT_C.sub('', text).strip()
        yield name_from(body), "decl", i + 1, lines[i].strip(), body
        i = j + 1


def extern_c_defs_with_bodies(lines: list[str]):
    """Yield (name, lineno, header, body) for extern "C" DEFINITIONS.

    Captures the full brace-balanced body so plan can inspect casts/params.
    """
    i, n = 0, len(lines)
    while i < n:
        line = strip_comment(lines[i]).strip()
        if not line or not RE_EXT_C.search(line):
            i += 1
            continue
        rest = RE_EXT_C.sub('', line).strip()
        if rest == '{' or '{' not in rest:
            i += 1
            continue
        before, after = rest.split('{', 1)
        depth = 1 + after.count('{') - after.count('}')
        body = [after]
        j = i
        while depth > 0 and j + 1 < n:
            j += 1
            ln = strip_comment(lines[j])
            depth += ln.count('{') - ln.count('}')
            if depth > 0:
                body.append(ln)
            else:
                idx = ln.find('}')
                body.append(ln[:idx] if idx >= 0 else ln)
        name = name_from(before.strip())
        if name:
            yield name, i + 1, before.strip(), ' '.join(body)
        i = j + 1


def _retail_tables(regions: list[str]):
    """Return (names, addr->[(name, region)], addr->name) merged across regions."""
    names: dict[str, str] = {}
    addr_hits: dict[int, list[tuple[str, str]]] = {}
    for reg in regions:
        path = ROOT / "config" / reg / "symbols.txt"
        if not path.is_file():
            continue
        for e in load_symbols(path):
            names.setdefault(e.name, reg)
            addr_hits.setdefault(e.address, []).append((e.name, reg))
    # retail reloc map retail_symbols are ground-truth names too
    rmap_path = ROOT / "tools" / "coop" / "retail_reloc_map.json"
    if rmap_path.is_file():
        rmap = json.loads(rmap_path.read_text(encoding="utf-8"))
        for sym, kinds in rmap.get("entries", {}).items():
            for v in kinds.values():
                rs = v.get("retail_symbol")
                if rs:
                    names.setdefault(rs, "relocmap")
    return names, addr_hits


def classify(name: str, names: dict[str, str], addr_hits: dict[int, list[tuple[str, str]]]):
    """Return (category, resolved) for a declared name.

    category: exact | drift | invented
    """
    if name in names:
        return "exact", names[name]
    # 1. embedded address resolves to a retail entry
    for m in RE_ADDR_ANY.finditer(name):
        addr = int(m.group(1), 16)
        hits = addr_hits.get(addr)
        if hits and any(rn != name for rn, _ in hits):
            return "drift", hits[0][0]
    # 2. retail name is a prefix of declared (func_8006EF04 vs func_8006EF04__Fi)
    for rn in names:
        if len(rn) > len(name) and rn.startswith(name):
            return "drift", rn
    # 3. declared is prefix of retail
    for rn in names:
        if len(name) > len(rn) and name.startswith(rn):
            return "drift", rn
    # 4. mangled qualifier match: same class/namespace tail after last '__'
    if '__' in name:
        q = name.split('__')[-1]
        for rn in names:
            if '__' in rn and rn.split('__')[-1] == q:
                return "drift", rn
    return "invented", None


def embedded_addr(name: str) -> int | None:
    m = RE_ADDR_ANY.search(name)
    return int(m.group(1), 16) if m else None


def member_mangled(class_name: str, member_name: str) -> str:
    """MWCC mangling for a no-arg member: name__<len>ClassFv."""
    return f"{member_name}__{len(class_name)}{class_name}Fv"


# ── scan ───────────────────────────────────────────────────────────────────

def cmd_scan(args: argparse.Namespace) -> int:
    names, addr_hits = _retail_tables(args.regions)
    files = sorted(
        [p for p in (ROOT / "src").rglob("*.cpp")]
        + [p for p in (ROOT / "libs").rglob("*.cpp")]
    )
    exact, drift, invented, unparsed = [], [], [], []
    member_candidates = []  # mangled names or void* self decls
    jp_stale = []  # name in US table but embedded address != entry address
    for f in files:
        for name, kind, lineno, raw, body in extract_entries(
            f.read_text(encoding="utf-8", errors="replace").splitlines()
        ):
            rel = str(f.relative_to(ROOT))
            if name is None:
                unparsed.append((rel, lineno, raw))
                continue
            rec = [rel, lineno, name, kind]
            if name in names:
                exact.append(rec)
            else:
                cat, resolved = classify(name, names, addr_hits)
                if cat == "drift":
                    drift.append(rec + [resolved])
                else:
                    invented.append(rec)
            # member-candidate heuristics
            is_mangled = '__' in name
            self_style = bool(RE_SELF_FIRST.search(body))
            if is_mangled or self_style:
                member_candidates.append(rec + [bool(self_style)])
            # JP-stale embedded address (name correct, address is old-layout)
            if name in names:
                a = embedded_addr(name)
                if a is not None:
                    hits = addr_hits.get(a)
                    if hits and not any(rn == name for rn, _ in hits):
                        jp_stale.append(rec + [hex(a), names[name]])

    print(f"files scanned: {len(files)}")
    print(f"extern \"C\" declarations parsed: {len(exact) + len(drift) + len(invented)}")
    print(f"  retail-exact      : {len(exact)}  (extern \"C\" required, name correct)")
    print(f"  retail-drift      : {len(drift)}  (extern \"C\" required; name must be fixed)")
    print(f"  invented          : {len(invented)}  (cleanable: extern \"C\" removable)")
    print(f"  member-candidates : {len(member_candidates)}  (mangled or void* self — see plan)")
    print(f"  jp-stale-address  : {len(jp_stale)}  (name OK; embedded address is old-layout)")
    print(f"  unparsed          : {len(unparsed)}")

    print("\n== RETAIL-DRIFT ==")
    for rel, ln, n, kind, resolved in sorted(drift):
        print(f"  {rel}:{ln}  {n}  ->  {resolved}")
    print("\n== INVENTED ==")
    for rel, ln, n, kind in sorted(invented):
        print(f"  {rel}:{ln}  {n}")

    if args.json:
        out = {
            "exact": exact,
            "drift": drift,
            "invented": invented,
            "member_candidates": member_candidates,
            "jp_stale": jp_stale,
            "unparsed": unparsed,
        }
        Path(args.json).write_text(json.dumps(out, indent=1) + "\n")
        print(f"\nwrote {args.json}")
    return 0


# ── plan ───────────────────────────────────────────────────────────────────

def cmd_plan(args: argparse.Namespace) -> int:
    token = args.token
    names, addr_hits = _retail_tables(args.regions)

    # retail symbols for the token's class (mangled qualifier or name prefix)
    class_syms = []
    for n, reg in names.items():
        if token in n:
            class_syms.append((n, reg))
    class_syms.sort()

    # extern "C" definitions referencing the class across src/libs
    files = sorted(
        [p for p in (ROOT / "src").rglob("*.cpp")]
        + [p for p in (ROOT / "libs").rglob("*.cpp")]
    )
    hits = []
    for f in files:
        text = f.read_text(encoding="utf-8", errors="replace")
        if token not in text:
            continue
        lines = text.splitlines()
        # definitions with real bodies: inspect casts and params
        for name, lineno, header, body in extern_c_defs_with_bodies(lines):
            cast_cls = RE_CLASS_CAST.search(body)
            if cast_cls and cast_cls.group(1) == token:
                hits.append((str(f.relative_to(ROOT)), lineno, name, "def-selfcast", header))
            elif re.search(rf'\(\s*{token}\s*\*', header):
                hits.append((str(f.relative_to(ROOT)), lineno, name, "def-param", header))
        # plain declarations referencing the class token
        for name, kind, lineno, raw, body in extract_entries(lines):
            if name is None:
                continue
            if token in name and (name in names or classify(name, names, addr_hits)[0] != "invented"):
                hits.append((str(f.relative_to(ROOT)), lineno, name, "decl", raw))

    # targets.json status for the involved retail symbols
    targets = {}
    tpath = ROOT / "tools" / "coop" / "targets.json"
    if tpath.is_file():
        data = json.loads(tpath.read_text(encoding="utf-8"))
        for t in data.get("targets", []):
            targets[t.get("symbol")] = t.get("status")

    print(f"plan for class token: {token}")
    print(f"retail symbols containing '{token}': {len(class_syms)}")
    for n, reg in class_syms[:30]:
        print(f"  {n} [{reg}] status={targets.get(n, '-')}")

    print(f"\nextern \"C\" definitions/decls referencing '{token}': {len(hits)}")
    for rel, lineno, name, kind, raw in sorted(hits):
        print(f"  {rel}:{lineno}  [{kind}] {name}")
        print(f"      {raw[:110]}")
        a = embedded_addr(name)
        if a is not None and (a in addr_hits):
            print(f"      retail: {addr_hits[a][0][0]} @ {hex(a)} status={targets.get(addr_hits[a][0][0], '-')}")
        elif name in names:
            print(f"      retail: {name} status={targets.get(name, '-')}")

    print("\n== conversion recipe ==")
    for rel, lineno, name, kind, raw in sorted(hits):
        if kind in ("def-selfcast", "def-param"):
            mangled = member_mangled(token, name)
            print(f"  {name}:")
            print(f"    header : add `u8 {name}();` to class {token}")
            print(f"    cpp    : `{token}::{name}() {{ ... }}` emits {mangled}")
            print(f"    rename : symbols.txt {name} -> {mangled}  (all regions)")
            print(f"    callers: rg -l '\\b{name}\\b' src libs")

    if args.json:
        Path(args.json).write_text(
            json.dumps(
                {
                    "token": token,
                    "retail_symbols": class_syms,
                    "hits": hits,
                    "recipe": [
                        (rel, ln, name, member_mangled(token, name))
                        for rel, ln, name, kind, raw in sorted(hits)
                        if kind.startswith("def")
                    ],
                },
                indent=1,
            )
            + "\n"
        )
        print(f"\nwrote {args.json}")
    return 0


# ── member-check ────────────────────────────────────────────────────────────

def cmd_member_check(args: argparse.Namespace) -> int:
    from tools.coop import member_check as mc

    class_size = int(args.class_size, 16) if args.class_size else 0
    print("indexing retail asm...", file=sys.stderr)
    idx = mc.AsmIndex()
    print(f"indexed {len(idx.files)} asm files, {len(idx.fn_range)} functions, "
          f"{sum(len(v) for v in idx.calls.values())} call sites", file=sys.stderr)

    if args.all:
        targets = sorted(
            s for s in idx.calls if re.search(r"^func_[0-9A-F]{8}__Q\d", s)
        )
        if not targets:
            targets = sorted(s for s in idx.calls if "__Q" in s and "__Q" in s)
        print(f"auditing {len(targets)} func_XXXX__Q* symbols")
    else:
        targets = args.symbols
    if not targets:
        print("no symbols: pass names or --all")
        return 2

    results = []
    # one-pass data-pointer index for all target addresses
    addrs = {}
    for sym in targets:
        a = mc.symbol_address(sym, "us")
        if a:
            addrs[sym] = a
    data_hits = mc.build_data_hits(set(addrs.values()))
    for i, sym in enumerate(targets):
        r = mc.classify_symbol(sym, idx, class_size=class_size,
                               symbol_addr=addrs.get(sym), data_hits=data_hits)
        results.append(r)
        if args.all and args.limit and i + 1 >= args.limit:
            break

    from collections import Counter as _C

    verdict_counts: _C = _C()
    for r in results:
        v = r["verdict"].split("(")[0].strip()
        verdict_counts[v] += 1
        print(f"{r['symbol']}  ({r['call_sites']} calls)")
        prov = ",".join(f"{k}:{n}" for k, n in sorted(r["r3_provenance"].items()))
        print(f"    prov[{prov}] deref={r['callee'].get('deref')} "
              f"maxoff={r['callee'].get('deref_max_offset', 0):#x} "
              f"intonly={r['callee'].get('integer_only')} vtbl={r['callee'].get('vtable_dispatch')}")
        print(f"    => {r['verdict']}")
        if r.get("vtable_hints"):
            print(f"    [data-pointer hints] "
                  + "; ".join(f"{o} x{f}" for o, f in r["vtable_hints"][:4]))

    print()
    print("== summary ==")
    for v, n in verdict_counts.most_common():
        print(f"  {n:4d}  {v}")

    if args.json:
        Path(args.json).write_text(
            json.dumps(results, indent=1, default=str) + "\n"
        )
        print(f"\nwrote {args.json}")
    return 0


# ── CLI ────────────────────────────────────────────────────────────────────

def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(prog="extc", description=__doc__)
    parser.add_argument("--region", default="us", help="primary region (default us)")
    sub = parser.add_subparsers(dest="cmd", required=True)

    p_scan = sub.add_parser("scan", help="classify every extern \"C\" declaration")
    p_scan.add_argument("--json", metavar="PATH", help="write full classification JSON")

    p_plan = sub.add_parser("plan", help="member-conversion plan for a class token")
    p_plan.add_argument("token", help="class name or symbol substring (e.g. CExchangeWin)")
    p_plan.add_argument("--json", metavar="PATH", help="write plan JSON")

    p_mc = sub.add_parser(
        "member-check",
        help="ABI-level member-vs-free classification from retail asm (verified tiered rules)",
    )
    p_mc.add_argument(
        "symbols",
        nargs="*",
        help="retail symbol names to classify (e.g. func_8007EEE0__Q22cf13CfGameManagerFv)",
    )
    p_mc.add_argument(
        "--class-size",
        metavar="HEX",
        default="0",
        help="annotated class size for the N3 layout rule (e.g. 0xB8 for CfGameManager)",
    )
    p_mc.add_argument(
        "--all",
        action="store_true",
        help="audit every func_XXXX__Q* symbol found in the retail asm",
    )
    p_mc.add_argument(
        "--limit",
        type=int,
        default=0,
        help="cap on --all output (summary still prints full counts)",
    )
    p_mc.add_argument("--json", metavar="PATH", help="write verdicts JSON")

    args = parser.parse_args(argv)
    args.regions = ["us", "eu"] if args.region == "us" else [args.region]
    if args.cmd == "scan":
        return cmd_scan(args)
    if args.cmd == "plan":
        return cmd_plan(args)
    if args.cmd == "member-check":
        return cmd_member_check(args)
    parser.error(f"unknown subcommand: {args.cmd}")
    return 2


if __name__ == "__main__":
    raise SystemExit(main())
