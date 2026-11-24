"""Context packs for cheap matching agents.

A pack is one self-contained markdown brief for a single function: the
acceptance rules, the current decomp source of the function, the retail vs
decomp side-by-side (hexdiff --symbol), and the exact commands the agent uses
to check itself.  The land gate is the only judge, so the pack ends with the
``land check`` command the orchestrator will run.

    python -m tools.coop.land pack <unit> <symbol> [--task T] [--out DIR]
"""
from __future__ import annotations

import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
PY = sys.executable
_ANSI = re.compile(r"\x1b\[[0-9;]*m")

RULES = """\
## Rules (the land gate rejects any violation; there is no partial credit)

- Goal: the ORIGINAL source shape. Match bytes by writing the code a Monolith
  programmer wrote in 2010, never by steering the compiler.
- Banned: asm, codegen #pragma, new noinline, intrinsics, #line, volatile,
  #define shields, pre-mangled names (foo__3BarFv) in calls/externs, hand
  vtables/RTTI/PTMF tables, view structs / overlays / offset casts
  (*(T*)((u8*)p + 0x10)), *View/*Sub types, void*/uintptr_t laundering, byte
  blobs, forced sections, dead stores, calls retail does not make, invented
  names without a `// name-guess:` comment.
- Also rejected (each was tried in wave 1 and held):
  - a local pointer to a scalar member/local used to re-read it
    (`const u32* p = &self->flags; if (*p & ...)`), or a pointer to a cast
    address of a local (`Vec* p = (Vec*)&tmp;`) — both only steer reloads /
    register allocation;
  - an extern under an address-suffixed name (`Foo_8066B550`): if data is
    "missing", report `BLOCKED data-layout` (it is a split/symbol issue the
    orchestrator fixes);
  - a new file-local `static inline` helper with a single call site invented
    to change inlining or register allocation;
  - moving a banned construct (an offset cast, a view) into a helper or
    another spelling instead of removing it — the ban counts per unit, and
    relocation is still a rejection on review;
  - reordering statements against the known order of an SDK/middleware
    original (nw4r, CRI, BTE, MSL, RVL SDK).
- If only register swaps remain after honest attempts, stop and report
  `BLOCKED regalloc`; that is a good result, not a failure.
- Members: access fields through the real class (add/rename members in the
  class header with evidence); do not add new lbl_ externs.
- Never edit: config/*/symbols.txt, splits.txt, configure.py, tools/, *.ctx.c,
  anything outside the files listed under "Files".  Never run git.
- Use `.venv/bin/python3`.  Scratch files go under /tmp/xb/<you>/ only.
- A function already at ✓ must stay ✓; data bytes must not drop.
- SDK / middleware code with a known public lineage (Broadcom BTE, nw4r,
  CRI, MSL, RVL SDK) keeps that code's idioms (its macros, its pointer-walk
  style).  Rewriting a known idiom into an unnatural form that happens to
  match (e.g. replacing `BE_STREAM_TO_UINT16(x, p)` advances with fixed
  `p + 4` offsets) is rejected; report BLOCKED `other: idiom` instead.
"""


def _hexdiff(unit: str, symbol: str) -> str:
    r = subprocess.run([PY, "tools/coop/hexdiff.py", unit, "--symbol", symbol, "--no-build"],
                       cwd=ROOT, capture_output=True, text=True, timeout=300)
    return _ANSI.sub("", r.stdout + r.stderr).strip()


def _source_for(unit: str) -> Path | None:
    for ext in (".cpp", ".c", ".cp"):
        for base in ("src", "libs"):
            p = ROOT / base / (unit.removeprefix("main/") + ext)
            if p.is_file():
                return p
    # libs/<lib>/src/... layout
    hits = list((ROOT / "libs").glob(f"*/src/{unit.removeprefix('main/')}.c*"))
    return hits[0] if hits else None


def _mangled_class(rest: str) -> str:
    """Innermost class name from a CodeWarrior qualifier: ``Q34nw4r3lyt8MaterialFv`` -> ``Material``."""
    m = re.match(r"Q(\d)", rest)
    count, i = (int(m.group(1)), 2) if m else (1, 0)
    name = ""
    for _ in range(count):
        d = re.match(r"\d+", rest[i:])
        if not d:
            return ""
        n = int(d.group(0))
        i += len(d.group(0))
        if i + n > len(rest):
            return ""  # not a length-prefixed name (e.g. ``__dt__8023E448``)
        name, i = rest[i:i + n], i + n
    return name if re.match(r"[A-Za-z_]\w*$", name) else ""


def _demangled_base(symbol: str) -> str:
    """Best-effort plain function name: ``foo__Q22cf3BarFv`` -> ``foo``;
    ctors/dtors -> ``Bar::Bar`` / ``Bar::~Bar``."""
    for tag, pre in (("__ct__", ""), ("__dt__", "~")):
        if symbol.startswith(tag):
            cls = _mangled_class(symbol[len(tag):])
            return f"{cls}::{pre}{cls}" if cls else ""
    return symbol.split("__", 1)[0] if not symbol.startswith("__") else symbol


def function_source(src: str, symbol: str) -> str:
    """The function definition for *symbol* in *src* ('' if not found)."""
    names = [n for n in (symbol, _demangled_base(symbol)) if n]
    for base in names:
        body = _find_def(src, base)
        if body:
            return body
    return ""


def _find_def(src: str, base: str) -> str:
    from tools.coop.lib.source_regions import matching_brace, signature_open_brace
    lead = r"(?<![\w~])" if not base.startswith("~") else ""
    for m in re.finditer(rf"(?m)^[^\n;{{}}]*{lead}{re.escape(base)}\s*\(", src):
        paren = m.end() - 1
        brace = signature_open_brace(src, paren)
        if brace is None:
            continue
        try:
            end = matching_brace(src, brace)
        except Exception:  # noqa: BLE001 - unbalanced: skip candidate
            continue
        start = src.rfind("\n", 0, m.start()) + 1
        # include the comment block directly above
        lines = src[:start].splitlines(keepends=True)
        k = len(lines)
        while k and lines[k - 1].lstrip().startswith("//"):
            k -= 1
        return "".join(lines[k:]) + src[start:end + 1]
    return ""


def build(unit: str, symbol: str, task: str) -> str:
    unit = unit.removeprefix("main/")
    src_path = _source_for(unit)
    src = src_path.read_text(encoding="utf-8", errors="replace") if src_path else ""
    hdr = src_path.with_suffix(".hpp") if src_path else None
    files = [str(p.relative_to(ROOT)) for p in (src_path, hdr) if p and p.is_file()]
    body = function_source(src, symbol) or "(not found by name; search the file)"
    premangled = bool(re.search(rf"\b{re.escape(symbol)}\s*\(", body)) and "__" in symbol.lstrip("_")
    note = ("\n**This function is currently defined under its pre-mangled retail name "
            "(banned).** Part of the task is to turn it into the real member/function "
            "it is (declared in the class header) so the compiler produces that name "
            "itself; update its callers in your files the same way.\n") if premangled else ""
    incl = "\n".join(l for l in src.splitlines() if l.startswith("#include"))
    rel = files[0] if files else "?"
    from tools.coop.land import shapes  # late: shapes imports ledger
    return f"""# Task {task}: match `{symbol}` in `{unit}`

{RULES}
{shapes.pack_section()}## Files

{chr(10).join('- ' + f for f in files)}
(only these files; if the fix needs a member or type in any other header, report `BLOCKED type-missing` naming the exact member and your evidence)

Includes of {rel}:
```
{incl}
```

## Current decomp source
{note}
```cpp
{body}
```

## Retail vs decomp (left = retail, right = decomp)

```
{_hexdiff(unit, symbol)}
```

## Check yourself

    .venv/bin/python3 tools/coop/hexdiff.py {unit} --symbol {symbol}
    .venv/bin/python3 tools/coop/hexdiff.py {unit} --all   # nothing else may regress

## Deliverable

Leave the edit in the working tree and reply with: `DONE {task}` plus a one-line
summary, or `BLOCKED {task}: <class>` where class is one of
`regalloc` (only reg-swaps left), `schedule` (instruction order only),
`type-missing` (needs a class/member that does not exist yet),
`callee-signature` (a callee's declaration is wrong), `data-layout`
(literal/pool/rodata order), `inline-unknown` (an inline helper is missing),
`other: <one line>`.  Partial progress that does not reach ✓ is still useful
if nothing regressed; say what changed.  Do NOT run `tools.coop.land` yourself
(it judges the whole tree, including other agents' unfinished work); the
orchestrator harvests your edit and lands it.
"""


def main(argv: list[str]) -> int:
    import argparse
    ap = argparse.ArgumentParser(prog="python -m tools.coop.land pack")
    ap.add_argument("unit")
    ap.add_argument("symbol")
    ap.add_argument("--task", default=None)
    ap.add_argument("--out", default="/tmp/xb/packs")
    a = ap.parse_args(argv)
    task = a.task or re.sub(r"[^A-Za-z0-9]+", "-", a.symbol)[:40].strip("-").upper()
    out = Path(a.out)
    out.mkdir(parents=True, exist_ok=True)
    p = out / f"{task}.md"
    p.write_text(build(a.unit, a.symbol, task))
    print(p)
    return 0
