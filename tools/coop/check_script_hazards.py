#!/usr/bin/env python3
"""Script-hazard lint: object tools must never be called in place.

`powerpc-eabi-objcopy` treats a **missing output file** as in-place conversion:
`objcopy -O binary --only-section=.text foo.o` rewrites `foo.o` as a raw dump of
that section. A wave-DL verification script did exactly that and destroyed 66
retail + 20 decomp objects in one minute, which then looked like a 600-symbol
regression in the link frontier.

This checker scans Python and shell sources for `objcopy` invocations that do
not carry exactly two file operands (one input, one output) and reports them.
Legitimate in-place uses can be marked with a `hazard-lint: allow` comment on
the invocation or the line before it.

Usage:
    python3 tools/coop/check_script_hazards.py [paths ...] [--json]
Exit code 0 = clean, 1 = hazards found. Default paths: tools/ and .scratch/.
"""
from __future__ import annotations

import argparse
import json
import pathlib
import re

ROOT = pathlib.Path(__file__).resolve().parents[2]

# objcopy options that consume the *next* token as their value
VALUE_FLAGS = {
    "-I", "-O", "-F", "-b", "-i", "-j", "-R", "-N", "-G", "-L", "-W", "-B",
    "--input-target", "--output-target", "--target", "--architecture",
    "--binary-architecture", "--interleave", "--only-section", "--remove-section",
    "--strip-symbol", "--keep-symbol", "--localize-symbol", "--weaken-symbol",
    "--globalize-symbol", "--add-section", "--set-section-flags",
    "--set-section-alignment", "--rename-section", "--change-section-address",
    "--change-section-lma", "--change-section-vma", "--redefine-sym",
    "--redefine-syms", "--prefix-symbols", "--prefix-sections",
    "--prefix-alloc-sections", "--gap-fill", "--pad-to", "--set-start",
    "--change-start", "--adjust-section-vma", "--adjust-section-lma",
    "--adjust-start", "--change-addresses", "--file-alignment",
    "--section-alignment", "--heap", "--image-base", "--stack", "--srec-len",
    "--extract-symbol", "--keep-file-symbols", "--dump-section",
    "--update-section", "--add-symbol", "--add-gnu-debuglink",
    "--remove-section", "--keep-section", "--keep-symbols",
}
ALLOW = "hazard-lint: allow"
COMMAND_RE = re.compile(r"objcopy")
COMMAND_WORD = re.compile(r"(?i)^(?:str\(|\{)?[\w./-]*objcopy\}?\)?$")


def _logical_command(text: str, start: int) -> str:
    """Return the argument text of the invocation containing ``start``.

    Python list-style calls: walk back to the innermost enclosing bracket and
    return its contents (so multi-line lists and ``str(obj)`` survive).
    Shell: return the line (with continuations) cut at shell operators.
    """
    depth, i, open_idx = 0, start, None
    while i >= 0:
        ch = text[i]
        if ch in ")]}":
            depth += 1
        elif ch in "[({":
            if depth == 0:
                open_idx = i
                break
            depth -= 1
        i -= 1
    if open_idx is not None:
        depth2, out, j = 0, [], open_idx
        while j < len(text):
            ch = text[j]
            out.append(ch)
            if ch in "[({":
                depth2 += 1
            elif ch in ")]}":
                depth2 -= 1
                if depth2 == 0:
                    break
            j += 1
        return "".join(out)[1:-1]
    line_start = text.rfind("\n", 0, start) + 1
    line_end = text.find("\n", start)
    stmt = text[line_start:line_end if line_end != -1 else len(text)]
    stmt = re.split(r"\|\||&&|[|;&<>]", stmt)[0]
    return re.sub(r"\s+[12]\s*$", "", stmt)


def _arguments(cmd: str) -> list[str]:
    """Depth-aware split of a captured command into arguments.

    Keeps ``str(obj)`` / ``tmp.name`` / quoted paths as single arguments so a
    Python list-style call counts one file operand per element.
    """
    args, cur, depth, quote = [], "", 0, None
    for ch in cmd:
        if quote:
            cur += ch
            if ch == quote:
                quote = None
            continue
        if ch in "\"'":
            quote = ch
            cur += ch
            continue
        if ch in "([{":
            depth += 1
            cur += ch
            continue
        if ch in ")]}":
            depth -= 1
            if depth < 0:
                break
            cur += ch
            continue
        if depth == 0 and (ch == "," or ch.isspace()):
            if cur:
                args.append(cur)
                cur = ""
            continue
        cur += ch
    if cur:
        args.append(cur)
    return [a.strip().strip(",").strip("\"'") for a in args if a.strip().strip(",").strip("\"'")]


def _operands(cmd: str) -> list[str]:
    args = _arguments(cmd)
    idx = next((i for i, a in enumerate(args) if COMMAND_WORD.match(a)), None)
    args = args[idx + 1:] if idx is not None else []
    operands: list[str] = []
    skip_next = False
    for tok in args:
        if skip_next:
            skip_next = False
            continue
        if tok.startswith("-"):
            if "=" not in tok and tok in VALUE_FLAGS:
                skip_next = True
            continue
        operands.append(tok)
    return operands


def scan_text(text: str, name: str = "<text>") -> list[dict]:
    """Return hazard records for one source blob."""
    hazards: list[dict] = []
    for m in COMMAND_RE.finditer(text):
        line_start = text.rfind("\n", 0, m.start()) + 1
        prev_line_start = text.rfind("\n", 0, max(0, line_start - 1)) + 1
        end = text.find("\n", m.start())
        line = text[line_start:end if end != -1 else len(text)]
        prev = text[prev_line_start:line_start]
        if line.lstrip().startswith("#"):        # prose comment, not a command
            continue
        cmd = _logical_command(text, m.start())
        if ALLOW in cmd or ALLOW in line or ALLOW in prev:
            continue
        args = _arguments(cmd)
        if not any(COMMAND_WORD.match(a) for a in args):
            continue                            # not an invocation of the tool
        # --dump-section writes the section into the "name=file" mapping, so a
        # single file operand (the input object) is correct usage, not in-place
        # conversion.
        if any(a.startswith("--dump-section") for a in args):
            continue
        _a = _arguments(cmd)
        _i = next((i for i, a in enumerate(_a) if COMMAND_WORD.match(a)), None)
        args_after = _a[_i + 1:] if _i is not None else []
        if not any(a.startswith("-") for a in args_after):
            continue                            # not an objcopy invocation (helper-call argument)
        ops = _operands(cmd)
        # The hazard is exactly one file operand: objcopy then rewrites that
        # file in place. Zero operands is prose/misparse, 2 is correct usage,
        # >2 is a malformed invocation we do not try to diagnose here.
        if len(ops) != 1:
            continue
        hazards.append({
            "file": name,
            "line": text.count("\n", 0, m.start()) + 1,
            "reason": "in-place conversion (missing output operand)",
            "command": " ".join(cmd.split())[:160],
            "operands": ops,
        })
    return hazards


def scan_file(path: pathlib.Path) -> list[dict]:
    try:
        text = path.read_text(errors="replace")
    except OSError:
        return []
    if "objcopy" not in text:
        return []
    label = str(path.relative_to(ROOT)) if str(path).startswith(str(ROOT)) else str(path)
    return scan_text(text, label)


def iter_sources(roots: list[pathlib.Path]):
    for root in roots:
        if root.is_file():
            yield root
            continue
        for pat in ("*.py", "*.sh", "*.bash"):
            for p in root.rglob(pat):
                if any(part in {".venv", "__pycache__", "node_modules", ".git"} for part in p.parts):
                    continue
                yield p


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("paths", nargs="*", default=None, help="files/dirs to scan (default: tools/ .scratch/)")
    ap.add_argument("--json", action="store_true")
    args = ap.parse_args()
    roots = [pathlib.Path(p) for p in (args.paths or ["tools", ".scratch"])]
    roots = [r if r.is_absolute() else ROOT / r for r in roots]
    hazards: list[dict] = []
    for p in iter_sources(roots):
        hazards += scan_file(p)
    if args.json:
        print(json.dumps(hazards, indent=1))
    else:
        for h in hazards:
            print(f"{h['file']}:{h['line']}: {h['reason']}\n    {h['command']}")
        print(f"{len(hazards)} hazard(s) in {len(roots)} root(s)")
    return 1 if hazards else 0


if __name__ == "__main__":
    raise SystemExit(main())
