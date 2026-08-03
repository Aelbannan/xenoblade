#!/usr/bin/env python3
"""TU code-smell scanner for the decomp fork.

Scans .cpp/.hpp files for the smell families the team wants to track:
  extern_c      - extern "C" blocks, split into defs vs decls, lbl_* vs other
  self_param    - free functions whose first param is named `self` (i.e. should
                  be a member function) OR `self->` member-style access on a
                  free function
  void_ptr      - void* in params/locals/casts
  ptr_arith     - raw pointer offset arithmetic: (char*)p + N, *(u32*)(p+0x..)
  asm_code      - inline asm / register bindings / volatile fake stacks
  fake_this     - u32/first-arg named r3..r31 used as a fake this/register
  globals_in_c  - data symbols declared extern "C" that are NOT lbl_eu_* (real
                  globals that could come from a header)
Output: markdown table grouped by directory, sorted by severity.
"""
from __future__ import annotations

import argparse
import os
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]

RE_EXTERN_C = re.compile(r'extern\s*"C"')
RE_LBL = re.compile(r'\blbl_eu_\w+|(?:^|\s)lbl_[A-Za-z0-9_]+')
RE_VOID_PTR = re.compile(r'\bvoid\s*\*\s*\w+')
RE_VOID_PTR_CAST = re.compile(r'\(\s*void\s*\*\s*\)')
RE_SELF_PARAM = re.compile(r'^\s*[A-Za-z_:<>0-9]+\s*(\*\s*)?self\s*(?:,|\))')
RE_PARAM_BY_NAME = re.compile(
    r'^\s*[^;{}]*?\b(self|r3|r4|r5|r6|r7|r8|r9|r10|r11|r12|r13|r14|r15|r16|r17|r18|r19|r20|r21|r22|r23|r24|r25|r26|r27|r28|r29|r30|r31)\b\s*(?:,|\))')
RE_SELF_ACCESS = re.compile(r'\bself\s*->')
RE_CAST = re.compile(r'\(\s*(?:char|u8|u16|u32|s8|s16|s32|int|short|long|float|u64)\s*\*\s*\)')
RE_HEX_OFF = re.compile(r'[+]\s*0x[0-9A-Fa-f]+')
RE_DEC_OFF = re.compile(r'[+]\s*[0-9]{1,3}\s*(?:\)|,|;|\])')
RE_DEREF_ARITH = re.compile(r'\*\(\s*\w+\s*\*\)\s*\(\s*\(?\s*(?:char|u8|u16|u32|u64|int|s8|s16|s32|s64|float|double)\s*\*?\s*\)?[^)]*[+][^)]*\)')
RE_ASM = re.compile(r'\basm\b|__asm|\bregister\b')
# asm entries whose bodies must be skipped by scan_file(skip_asm_bodies=True):
#   - asm function decls, incl. macro-prefixed forms
#     (``DECL_SECTION(".init") DECL_WEAK asm void f(register ...) {``)
#   - ``asm { ... }`` block statements
#   - ``ASM_VOLATILE( ... )`` / ``ASM( ... )`` paired-single blocks (GX, mtx, OS)
RE_ASM_ENTRY = re.compile(
    r'^\s*(?!/?\*).*\basm\s+(?:void|const\s+void|[A-Za-z_]\w*(?:\s*\*)?)\s+'
    r'[A-Za-z_]\w*\s*\(|^\s*asm\s*\{|\b(?:ASM_VOLATILE|ASM)\s*\(\s*$'
)
RE_ASM_PAREN = re.compile(r'\b(?:ASM_VOLATILE|ASM)\s*\(')
RE_FAKE_STACK = re.compile(r'\bvolatile\b[^;]*\b(?:char|u8)\b[^;]*\[|\b(?:u8|char)\s+(?:sp|stack)\d*\s*\[')
RE_RN_PARAM = re.compile(r'\b(?:r3|r4|r5|r6|r7|r8|r9|r10|r11|r12|r13|r14|r15|r16|r17|r18|r19|r20|r21|r22|r23|r24|r25|r26|r27|r28|r29|r30|r31)\s*[,)]')
RE_GOTO = re.compile(r'\bgoto\b')
RE_DECOMP_MACRO = re.compile(r'DECOMP_(?:PPC|FORCELITERAL|FORCEACTIVE)')
RE_PRAGMA = re.compile(r'^\s*#\s*pragma')
RE_IF0 = re.compile(r'^\s*#\s*if\s+0')
RE_EXTERN_C_BLOCK = re.compile(r'extern\s*"C"\s*\{')
RE_INCLUDE = re.compile(r'^\s*#\s*include\s*[<"]([^>"]+)')


class Stats:
    def __init__(self):
        self.extern_c_total = 0
        self.extern_c_lbl = 0
        self.extern_c_nonlbl_decl = 0
        self.extern_c_nonlbl_def = 0
        self.extern_c_other = 0
        self.self_params = 0
        self.self_access = 0
        self.void_ptr = 0
        self.void_ptr_cast = 0
        self.ptr_arith = 0
        self.deref_arith = 0
        self.asm_code = 0
        self.fake_stack = 0
        self.rn_params = 0
        self.goto_count = 0
        self.decomp_macro = 0
        self.pragma = 0
        self.if0 = 0
        self.includes = 0


def scan_file(path: Path, skip_asm_bodies: bool = False) -> Stats:
    """Scan one TU for smell families.

    With ``skip_asm_bodies=True`` (used by the RVL_SDK report), lines inside
    ``asm`` function/block bodies are not counted against the C-level metrics
    (rN/self/void*/arith). Their PPC mnemonic lines (``lis r3, ...``) would
    otherwise pollute those counts; the body lines are folded into
    ``asm_code`` only. The default ``False`` keeps the game-code scan
    behaviour byte-identical.
    """
    s = Stats()
    try:
        lines = path.read_text(errors="replace").splitlines()
    except OSError:
        return s
    in_extern_c = False
    in_asm = False
    amode = "brace"  # "brace" for asm fns/`asm {` bodies, "paren" for ASM_VOLATILE(
    depth = 0
    for raw in lines:
        line = raw.split("//", 1)[0]
        stripped = line.strip()
        if skip_asm_bodies:
            asm_entry = bool(RE_ASM_ENTRY.search(line))
            if not in_asm and asm_entry:
                in_asm = True
                s.asm_code += 1
                amode = "paren" if RE_ASM_PAREN.search(line) else "brace"
                depth = (line.count("(") - line.count(")")) if amode == "paren" else (
                    line.count("{") - line.count("}")
                )
                continue
            if in_asm:
                if amode == "brace":
                    depth += line.count("{") - line.count("}")
                    if stripped == "}" or depth < 0:
                        in_asm = False
                else:
                    depth += line.count("(") - line.count(")")
                    if depth <= 0:
                        in_asm = False
                continue
        if RE_EXTERN_C_BLOCK.search(line):
            in_extern_c = True
            depth = 0
            s.extern_c_total += 1
            continue
        if in_extern_c:
            depth += line.count("{") - line.count("}")
            if stripped == "}" or depth < 0:
                in_extern_c = False
                continue
        is_extern_c_line = bool(RE_EXTERN_C.search(line)) or in_extern_c
        if is_extern_c_line:
            s.extern_c_total += 1
            if RE_LBL.search(line):
                s.extern_c_lbl += 1
            elif "(" in line:
                if "{" in line or (")" in line and ") {" in raw):
                    s.extern_c_nonlbl_def += 1
                else:
                    s.extern_c_nonlbl_decl += 1
            else:
                s.extern_c_other += 1
        m = RE_INCLUDE.search(raw)
        if m:
            s.includes += 1
        if RE_VOID_PTR.search(line):
            s.void_ptr += 1
        if RE_VOID_PTR_CAST.search(line):
            s.void_ptr_cast += 1
        if RE_SELF_PARAM.search(line) or RE_PARAM_BY_NAME.search(line):
            s.self_params += 1
        if RE_SELF_ACCESS.search(line):
            s.self_access += 1
        if RE_CAST.search(line) and (RE_HEX_OFF.search(line) or RE_DEC_OFF.search(line)):
            s.ptr_arith += 1
        if RE_DEREF_ARITH.search(line):
            s.deref_arith += 1
        if RE_ASM.search(line):
            s.asm_code += 1
        if RE_FAKE_STACK.search(line):
            s.fake_stack += 1
        if RE_RN_PARAM.search(line):
            s.rn_params += 1
        if RE_GOTO.search(line):
            s.goto_count += 1
        if RE_DECOMP_MACRO.search(line):
            s.decomp_macro += 1
        if RE_PRAGMA.search(line):
            s.pragma += 1
        if RE_IF0.search(line):
            s.if0 += 1
    return s


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("paths", nargs="*", help="files or dirs; default: game-code src dirs")
    ap.add_argument("--all", action="store_true", help="include headers")
    args = ap.parse_args()

    if args.paths:
        roots = [Path(p) for p in args.paths]
    else:
        roots = [ROOT / "src" / "kyoshin", ROOT / "libs" / "monolib" / "src", ROOT / "libs" / "nw4r" / "src"]
    files = []
    for r in roots:
        if r.is_file():
            files.append(r)
        else:
            files.extend(r.rglob("*.cpp"))
            if args.all:
                files.extend(r.rglob("*.hpp"))
    files = sorted(set(files))

    rows = []
    for f in files:
        s = scan_file(f)
        rel = Path(os.path.relpath(f, ROOT))
        sev = (s.self_params + s.self_access + s.void_ptr + s.void_ptr_cast
               + s.ptr_arith + s.deref_arith + s.asm_code + s.fake_stack
               + s.rn_params)
        extern_bad = s.extern_c_nonlbl_decl + s.extern_c_nonlbl_def
        rows.append((rel, sev, s, extern_bad, s.extern_c_nonlbl_decl, s.extern_c_nonlbl_def))

    rows.sort(key=lambda r: -r[1])
    print(f"# Smell scan — {len(rows)} files\n")
    print("| TU | extC-total | extC-decl | extC-def | self | void* | ptr-arith | deref-arith | asm | rN | goto |")
    print("|---|---|---|---|---|---|---|---|---|---|---|")
    for rel, sev, s, extern_bad, d, df in rows:
        if sev == 0 and extern_bad == 0:
            continue
        print(f"| {rel} | {s.extern_c_total} | {d} | {df} | {s.self_params + s.self_access} | "
              f"{s.void_ptr + s.void_ptr_cast} | {s.ptr_arith} | {s.deref_arith} | "
              f"{s.asm_code} | {s.rn_params} | {s.goto_count} |")
    return 0


if __name__ == "__main__":
    sys.exit(main())
