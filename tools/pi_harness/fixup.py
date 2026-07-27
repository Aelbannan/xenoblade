#!/usr/bin/env python3
"""Fixup: strip extern-C from function stubs, add missing header declarations.

Handles:
  1. Individual ``extern "C"`` function definitions (NOT block-wrapped)
  2. Converts to proper C++ member functions when the class exists in a header
  3. Adds missing method declarations to existing class headers
  4. Removes ``#pragma push/auto_inline/pop`` lines
  5. Strips ``self`` parameter, rewrites ``self->`` → ``this->``

Does NOT touch ``extern "C" { ... }`` blocks — those are left intact.

Usage:
    python3 tools/pi_harness/fixup.py --source <path> --all-unmatched [--dry-run]
"""

from __future__ import annotations

import argparse, json, os, re, sys
from collections import defaultdict
from dataclasses import dataclass
from typing import Optional

REPO_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
sys.path.insert(0, REPO_ROOT)
from tools.symbolrecover.lib.mwcc import demangle_symbol, DemangledSymbol


@dataclass
class FixupTarget:
    target_id: str
    symbol: str
    demangled: DemangledSymbol
    cpp_sig: Optional[str]
    status: str


# ---------------------------------------------------------------------------
# Loading
# ---------------------------------------------------------------------------

def _build_cpp_sig(d: DemangledSymbol, raw: str = "") -> Optional[str]:
    ns = f"{d.namespace}::" if d.namespace else ""
    cls = d.class_name or ""
    # Heuristic: __ct__/__dt__ without length prefix
    if not cls and d.is_ctor and raw.startswith("__ct__"):
        m = re.match(r"__ct__([A-Z]\w*)", raw)
        if m: cls = m.group(1)
    if not cls and d.is_dtor and raw.startswith("__dt__"):
        m = re.match(r"__dt__([A-Z]\w*)", raw)
        if m: cls = m.group(1)
    if not cls:
        return None
    if d.is_ctor:   return f"{ns}{cls}::{cls}"
    if d.is_dtor:   return f"{ns}{cls}::~{cls}"
    return f"{ns}{cls}::{d.function}"


def load_targets(repo_root: str, source_path: str) -> list[FixupTarget]:
    targets_path = os.path.join(repo_root, "tools", "coop", "targets.json")
    with open(targets_path) as f:
        data = json.load(f)
    src_rel = source_path[len(repo_root):].lstrip("/") if source_path.startswith(repo_root) else source_path
    results = []
    for t in data.get("targets", []):
        if t.get("kind") != "function":
            continue
        if t.get("source") not in (src_rel, source_path):
            continue
        d = demangle_symbol(t["symbol"])
        results.append(FixupTarget(t["id"], t["symbol"], d, _build_cpp_sig(d, t["symbol"]), t.get("status", "")))
    return results


# ---------------------------------------------------------------------------
# Header helpers
# ---------------------------------------------------------------------------

def _resolve_header(source_path: str) -> Optional[str]:
    hpp = source_path.replace(".cpp", ".hpp").replace(".c", ".hpp")
    if os.path.exists(hpp): return hpp
    m = re.match(r"((?:libs|src)/([^/]+))/src/(.+)\.(?:cpp|c)", source_path)
    if m:
        alt = f"{m.group(1)}/include/{m.group(2)}/{m.group(3)}.hpp"
        if os.path.exists(alt): return alt
    return None


def _add_methods_to_header(header_path: str, class_name: str, targets: list[FixupTarget], source_path: str = None) -> int:
    """Add missing method declarations. Extracts parameter lists from source if available.
    Returns number added."""
    try:
        with open(header_path) as f:
            lines = f.readlines()
    except Exception:
        return 0

    # Read source to extract parameter lists from definitions
    source_content = ""
    if source_path:
        try:
            with open(source_path) as f:
                source_content = f.read()
        except Exception:
            pass

    # Find class definition (not forward declaration)
    class_pat = re.compile(r"^\s*class\s+" + re.escape(class_name) + r"\b(?!\s*;)")
    start = None
    for i, line in enumerate(lines):
        if class_pat.search(line):
            start = i; break
    if start is None:
        return 0

    # Find opening brace, public section, closing brace
    depth = 0; in_class = False; public_idx = None; end = None
    for i in range(start, len(lines)):
        line = lines[i]
        if '{' in line and not in_class:
            in_class = True
            depth += line.count('{') - line.count('}')
            public_idx = i + 1
            if depth <= 0: end = i; break
            continue
        if in_class:
            depth += line.count('{') - line.count('}')
            if public_idx is None and re.search(r'^\s*public\s*:', line):
                public_idx = i
            if depth <= 0: end = i; break
    if end is None or public_idx is None:
        return 0

    # Find insertion point
    insert_at = end
    for i in range(end - 1, public_idx, -1):
        stripped = lines[i].rstrip()
        if stripped and not stripped.startswith("//"):
            insert_at = i + 1; break

    # Add missing methods
    existing = "".join(lines[start:end])
    added = 0
    for ft in targets:
        d = ft.demangled
        search = f"~{class_name}" if d.is_dtor else (f"{class_name}(" if d.is_ctor else d.function)
        if search in existing:
            continue
        if d.is_ctor:      decl = f"    {class_name}();\n"
        elif d.is_dtor:    decl = f"    virtual ~{class_name}();\n"
        else:
            const = " const" if "CF" in ft.symbol else ""
            # Try to extract parameter list from source definition
            params = "()"
            if source_content:
                esc_fn = re.escape(d.function)
                param_pat = re.compile(
                    r'(?:void\s+)?' + re.escape(class_name) + r'::' + esc_fn + r'\s*\(([^)]*)\)'
                )
                pm = param_pat.search(source_content)
                if pm:
                    params = "(" + pm.group(1).strip() + ")"
            decl = f"    void {d.function}{params}{const};\n"
        lines.insert(insert_at, decl)
        insert_at += 1; added += 1

    if added:
        with open(header_path, "w", encoding="utf-8", newline="\n") as f:
            f.writelines(lines)
    return added


# ---------------------------------------------------------------------------
# Source transformation
# ---------------------------------------------------------------------------

def _find_matching_brace(source: str, start: int) -> int:
    depth = 0
    for i in range(start, len(source)):
        if source[i] == '{': depth += 1
        elif source[i] == '}':
            depth -= 1
            if depth == 0: return i
    return -1


def _strip_self(params: str) -> tuple[str, Optional[str]]:
    for pat in [r'(\w+)\s*\*\s*(self|_this|me)\s*,?\s*(.*)',
                r'(void\s*\*|const\s+\w+\s*\*)\s*(self|_this)\s*,?\s*(.*)']:
        m = re.match(pat, params)
        if m: return m.group(3).rstrip(", "), m.group(2)
    return params, None


def _fix_self_in_body(body: str, name: str) -> str:
    body = re.sub(r'\b' + re.escape(name) + r'\s*->', 'this->', body)
    body = re.sub(r'\(\s*(\w+)\s*\*\)\s*' + re.escape(name), r'(\1*)this', body)
    body = re.sub(r'\b' + re.escape(name) + r'\b(?!-)', 'this', body)
    return body


def _find_block_ranges(source: str) -> list[tuple[int, int]]:
    """Find (start, end) ranges of extern "C" { ... } blocks."""
    ranges = []
    pat = re.compile(r'extern\s+"C"\s*\{')
    for m in pat.finditer(source):
        brace_start = m.end() - 1
        close = _find_matching_brace(source, brace_start)
        if close >= 0:
            ranges.append((m.start(), close + 1))
    return ranges


def _in_block(pos: int, block_ranges: list[tuple[int, int]]) -> bool:
    for start, end in block_ranges:
        if start <= pos < end:
            return True
    return False


def _fix_forward_decls(source: str, symbols: set[str]) -> str:
    """Strip extern-C from forward declarations of symbols being converted.
    Matches lines like: extern "C" void __dt__12CUICfManagerFv(void*);"""
    for sym in symbols:
        esc = re.escape(sym)
        pat = re.compile(
            r'^extern\s+"C"\s+([\w:*&<>\s]+' + esc + r'[\w:*&<>(),\s]*?;)\s*$',
            re.MULTILINE
        )
        for m in pat.finditer(source):
            # Don't touch if inside a block
            before = source[:m.start()]
            block_open = before.rfind('extern "C" {')
            block_close = before.rfind('}')
            if block_open > block_close:
                continue
            # Strip extern "C" — just remove the prefix
            source = source[:m.start()] + m.group(1).strip() + source[m.end():]
    return source


def _already_exists(source: str, cpp_sig: str) -> bool:
    """Check if the proper C++ signature already exists in the file."""
    # Extract method name from cpp_sig like "cf::CfGameManager::method"
    method = cpp_sig.rsplit("::", 1)[-1]
    # Look for the method definition pattern: ClassName::method( or ~ClassName(
    return bool(re.search(re.escape(method) + r'\s*\(', source))


def _fix_one_extern_c(source: str, ft: FixupTarget, cpp_sig: Optional[str], block_ranges: list[tuple[int, int]]) -> tuple[str, int]:
    """Fix a single extern-C function definition. Skips block-internal functions."""
    esc = re.escape(ft.symbol)
    d = ft.demangled

    # For FULL_MATCH/EQUIVALENT: check if proper C++ version already exists.
    # If so, this stub is dead code — delete it rather than converting.
    if ft.status in ("FULL_MATCH", "EQUIVALENT_MATCH") and cpp_sig and _already_exists(source, cpp_sig):
        # Delete the entire extern-C definition
        pat = re.compile(
            r'extern\s+"C"\s+[\w:*&<>\s]*?' + esc + r'[\w:*&<>(),\s]*?(\{|;)',
            re.DOTALL
        )
        m = pat.search(source)
        if m and not _in_block(m.start(), block_ranges):
            if m.group(1) == ';':
                # Forward declaration — just remove the line
                line_start = source.rfind('\n', 0, m.start()) + 1
                line_end = source.find('\n', m.end())
                if line_end < 0: line_end = len(source)
                source = source[:line_start] + source[line_end + 1:]
                return source, 1
            else:
                # Definition — remove from extern "C" to closing brace
                brace_start = m.start() + m.group().index('{')
                close_brace = _find_matching_brace(source, brace_start)
                if close_brace >= 0:
                    source = source[:m.start()] + source[close_brace + 1:]
                    return source, 1
        return source, 0

    # For unmatched targets: try member-function conversion (requires body with {)
    # SKIP for FULL/EQUIVALENT — their extern-C IS the matched code, can't change linkage
    if cpp_sig and d.class_name and ft.status not in ("FULL_MATCH", "EQUIVALENT_MATCH"):
        pat = re.compile(
            r'extern\s+"C"\s+'
            r'([A-Za-z_][\w:*&<>\s]*?)\s*'
            r'(' + esc + r')\s*'
            r'(\('
            r'((?:[^()]|\([^()]*\))*)'
            r'\))\s*'
            r'(\{)',
            re.DOTALL
        )
        m = pat.search(source)
        if m and not _in_block(m.start(), block_ranges):

            ret_type = m.group(1).strip()
            params_str = m.group(4).strip()
            brace_start = m.start(5)
            close_brace = _find_matching_brace(source, brace_start)
            if close_brace < 0:
                return source, 0

            body = source[brace_start:close_brace + 1]
            new_params, self_name = _strip_self(params_str)
            if self_name:
                body = _fix_self_in_body(body, self_name)

            const_q = " const" if "CF" in ft.symbol else ""
            ret_part = (ret_type + " ") if ret_type and not d.is_ctor else ""
            head = f"{ret_part}{cpp_sig}({new_params}){const_q} " if new_params else f"{ret_part}{cpp_sig}(){const_q} "
            source = source[:m.start()] + head + body + source[close_brace + 1:]
            return source, 1

    # Fallback: strip extern "C" from free function (not inside block)
    # SKIP for FULL/EQUIVALENT — extern-C linkage IS the matched code
    if ft.status in ("FULL_MATCH", "EQUIVALENT_MATCH"):
        return source, 0
    simple = re.compile(r'extern\s+"C"\s+(.*?' + esc + r'.*?)(\{|;)', re.DOTALL)
    m = simple.search(source)
    if not m or _in_block(m.start(), block_ranges):
        return source, 0

    if m.group(2) == ";":
        source = source[:m.start()] + m.group(1).rstrip() + ";" + source[m.end():]
        return source, 1

    brace_start = m.end() - 1
    close_brace = _find_matching_brace(source, brace_start)
    if close_brace >= 0:
        source = source[:m.start()] + m.group(1).rstrip() + source[brace_start:close_brace + 1] + source[close_brace + 1:]
    else:
        source = source[:m.start()] + m.group(1).rstrip() + source[m.end():]
    return source, 1


def _strip_pragmas(source: str) -> tuple[str, int]:
    pat = re.compile(r'^[ \t]*#[ \t]*pragma[ \t]+(?:push|auto_inline[ \t]+off|pop)[ \t]*\n?', re.MULTILINE)
    cnt = len(list(pat.finditer(source)))
    return pat.sub("", source), cnt


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def fixup_file(source_path: str, targets: list[FixupTarget], dry_run: bool = False) -> list[str]:
    descriptions: list[str] = []

    with open(source_path, encoding="utf-8", errors="replace") as f:
        source = f.read()

    # 1. Group by class
    header_path = _resolve_header(source_path)
    by_class: dict[str, list[FixupTarget]] = defaultdict(list)
    for ft in targets:
        cls = ft.demangled.class_name
        if not cls and ft.cpp_sig:
            m = re.match(r"(?:\w+::)?(\w+)::", ft.cpp_sig)
            if m: cls = m.group(1)
        if cls and ft.cpp_sig:
            by_class[cls].append(ft)
    scaffolds = set(by_class.keys())

    # 2. Strip pragmas
    source, n = _strip_pragmas(source)
    if n: descriptions.append(f"removed {n} pragma line(s)")

    # 3. Fix extern-C functions
    member = free = 0
    block_ranges = _find_block_ranges(source)
    
    # Pre-scan symbols to fix
    symbols_to_fix: set[str] = set()
    for ft in targets:
        if ft.symbol in source:
            symbols_to_fix.add(ft.symbol)
    source = _fix_forward_decls(source, symbols_to_fix)
    
    for ft in targets:
        cls = ft.demangled.class_name
        if not cls and ft.cpp_sig:
            m = re.match(r"(?:\w+::)?(\w+)::", ft.cpp_sig)
            if m: cls = m.group(1)
        cpp_sig = ft.cpp_sig if (cls and cls in scaffolds) else None
        source, n_fixed = _fix_one_extern_c(source, ft, cpp_sig, block_ranges)
        if n_fixed:
            if cpp_sig: member += 1
            else: free += 1

    if member: descriptions.append(f"converted {member} extern-C → member function")
    if free: descriptions.append(f"stripped extern-C from {free} free function(s)")

    # 4. Add header declarations (after fixup so params are visible)
    if header_path and not dry_run:
        for cls, methods in by_class.items():
            if _add_methods_to_header(header_path, cls, methods, source_path):
                descriptions.append(f"added {cls} declarations to {os.path.basename(header_path)}")

    if not descriptions:
        return []

    if not dry_run:
        with open(source_path, "w", encoding="utf-8", newline="\n") as f:
            f.write(source)

    return descriptions


def main() -> int:
    p = argparse.ArgumentParser()
    p.add_argument("--source", required=True)
    p.add_argument("--all-unmatched", action="store_true")
    p.add_argument("--dry-run", action="store_true")
    args = p.parse_args()

    targets = load_targets(REPO_ROOT, args.source) if args.all_unmatched else []
    if not targets:
        print(f"No unmatched targets for {args.source}"); return 0

    descs = fixup_file(args.source, targets, args.dry_run)
    if not descs:
        print("No fixups needed"); return 0

    for d in descs: print(f"  FIX: {d}")
    if args.dry_run: print(f"\n--- DRY RUN ({len(descs)} action(s)) ---")
    else: print(f"\nApplied {len(descs)} action(s) to {args.source}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
