#!/usr/bin/env python3
"""Extract target functions from ``extern "C" { ... }`` blocks and fix them.

For each ``extern "C" {`` block:
  1. Find function definitions/declarations matching unmatched targets
  2. Extract them from the block (move them after the block)
  3. Strip ``extern "C"``, optionally convert to member functions
  4. If the block becomes empty, remove the ``extern "C" {`` / ``}`` delimiters
  5. If only ``lbl_eu_*`` declarations remain, keep the block (they need C linkage)

Usage:
    python3 tools/pi_harness/extract_blocks.py --source <path> --all-unmatched [--dry-run]
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
class Target:
    target_id: str
    symbol: str
    demangled: DemangledSymbol
    cpp_sig: Optional[str]  # e.g. "CView::hasCurrent"


# ---------------------------------------------------------------------------
# Loading
# ---------------------------------------------------------------------------

def _build_cpp_sig(d: DemangledSymbol, raw: str = "") -> Optional[str]:
    ns = f"{d.namespace}::" if d.namespace else ""
    cls = d.class_name or ""
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


def _resolve_header(source_path: str) -> Optional[str]:
    hpp = source_path.replace(".cpp", ".hpp").replace(".c", ".hpp")
    if os.path.exists(hpp): return hpp
    m = re.match(r"((?:libs|src)/([^/]+))/src/(.+)\.(?:cpp|c)", source_path)
    if m:
        alt = f"{m.group(1)}/include/{m.group(2)}/{m.group(3)}.hpp"
        if os.path.exists(alt): return alt
    return None


def load_targets(repo_root: str, source_path: str) -> list[Target]:
    targets_path = os.path.join(repo_root, "tools", "coop", "targets.json")
    with open(targets_path) as f:
        data = json.load(f)
    skipped = {"FULL_MATCH", "EQUIVALENT_MATCH"}
    src_rel = source_path[len(repo_root):].lstrip("/") if source_path.startswith(repo_root) else source_path
    results = []
    for t in data.get("targets", []):
        if t.get("status") in skipped or t.get("kind") != "function":
            continue
        if t.get("source") not in (src_rel, source_path):
            continue
        d = demangle_symbol(t["symbol"])
        results.append(Target(t["id"], t["symbol"], d, _build_cpp_sig(d, t["symbol"])))
    return results


# ---------------------------------------------------------------------------
# Block processing
# ---------------------------------------------------------------------------

def _find_matching_brace(source: str, start: int) -> int:
    depth = 0
    for i in range(start, len(source)):
        if source[i] == '{': depth += 1
        elif source[i] == '}':
            depth -= 1
            if depth == 0: return i
    return -1


def _has_lbl_eu_decls(block_content: str) -> bool:
    """Check if block content has lbl_eu_* declarations (these need C linkage)."""
    return bool(re.search(r'\blbl_eu_\w+', block_content))


def _is_only_comments_or_blank(text: str) -> bool:
    """Check if text is only whitespace, comments, or blank lines."""
    for line in text.splitlines():
        stripped = line.strip()
        if stripped and not stripped.startswith("//") and not stripped.startswith("/*"):
            return False
    return True


def _extract_function_from_block(block_content: str, symbol: str) -> tuple[str, Optional[str]]:
    """Extract a function matching `symbol` from block content.
    Returns (new_block_content, extracted_function_text_or_None)."""
    esc = re.escape(symbol)

    # Match a function definition: <ret> <symbol>(<params>){<body>}
    def_pat = re.compile(
        r'([^\n]*' + esc + r'[^\n]*\{[^}]*\})'
    )
    dm = def_pat.search(block_content)
    if dm:
        extracted = dm.group(1).strip()
        new_content = block_content[:dm.start()] + block_content[dm.end():]
        return new_content, extracted

    # Match a forward declaration: <ret> <symbol>(<params>);
    decl_pat = re.compile(
        r'([^\n]*' + esc + r'[^\n]*;)'
    )
    dm = decl_pat.search(block_content)
    if dm:
        extracted = dm.group(1).strip()
        new_content = block_content[:dm.start()] + block_content[dm.end():]
        return new_content, extracted

    return block_content, None


def _strip_extern_c_from_line(line: str) -> str:
    """Strip extern \"C\" prefix from a single line."""
    return re.sub(r'extern\s+"C"\s+', '', line).strip()


def process_blocks(source: str, targets: list[Target], header_path: Optional[str]) -> tuple[str, list[str]]:
    """Process all extern \"C\" { ... } blocks. Returns (new_source, descriptions)."""
    descriptions: list[str] = []
    symbol_to_target = {t.symbol: t for t in targets}

    # Build set of class names that have headers (for member conversion)
    classes_with_headers: set[str] = set()
    if header_path:
        with open(header_path) as f:
            hdr = f.read()
        for t in targets:
            cls = t.demangled.class_name
            if not cls and t.cpp_sig:
                m = re.match(r"(?:\w+::)?(\w+)::", t.cpp_sig)
                if m: cls = m.group(1)
            if cls and re.search(r'\bclass\s+' + re.escape(cls) + r'\b(?!\s*;)', hdr):
                classes_with_headers.add(cls)

    block_pat = re.compile(r'extern\s+"C"\s*\{')
    extracted_count = 0
    blocks_removed = 0

    # Process blocks from end to start to preserve positions
    blocks = list(block_pat.finditer(source))

    for m in reversed(blocks):
        brace_start = m.end() - 1
        close_brace = _find_matching_brace(source, brace_start)
        if close_brace < 0:
            continue

        block_content = source[brace_start + 1:close_brace]
        extracted_lines: list[str] = []
        modified = False

        # Check each target symbol
        for sym in list(symbol_to_target.keys()):
            if sym not in block_content:
                continue

            new_content, extracted = _extract_function_from_block(block_content, sym)
            if extracted is not None:
                block_content = new_content
                # Fix the extracted line
                fixed = _strip_extern_c_from_line(extracted)
                t = symbol_to_target[sym]

                # Try member function conversion if class has a header
                cls = t.demangled.class_name
                if not cls and t.cpp_sig:
                    mm = re.match(r"(?:\w+::)?(\w+)::", t.cpp_sig)
                    if mm: cls = mm.group(1)

                if t.cpp_sig and cls and cls in classes_with_headers:
                    # Replace mangled name with C++ method name
                    for other_sym in [t.symbol]:
                        if other_sym in fixed:
                            # Find and replace the mangled name in the function def
                            # Pattern: <ret> <mangled_name>( → <ret> <cpp_sig>(
                            esc2 = re.escape(other_sym)
                            fixed = re.sub(
                                r'\b' + esc2 + r'\b',
                                t.cpp_sig,
                                fixed
                            )
                    extracted_lines.append(fixed)
                else:
                    # Keep as free function (already stripped extern-C)
                    extracted_lines.append(fixed)

                modified = True
                extracted_count += 1

        if not modified:
            continue

        # Determine what to do with the remaining block
        remaining = block_content.strip()

        if _is_only_comments_or_blank(remaining):
            # Empty block — remove it entirely
            source = source[:m.start()] + source[close_brace + 1:]
            blocks_removed += 1
        elif _has_lbl_eu_decls(remaining) and not re.search(r'\b(void|int|char|short|long|float|double|u\d|s\d|f\d|bool)\s+\w+\s*\(', remaining):
            # Only lbl_eu_* variable declarations left — keep block intact
            source = source[:brace_start + 1] + "\n" + remaining + "\n" + source[close_brace:]
        else:
            # Mixed content — keep block with remaining content
            source = source[:brace_start + 1] + "\n" + remaining + "\n" + source[close_brace:]

        # Insert extracted functions after the (possibly modified/removed) block
        insert_pos = m.start()
        for line in extracted_lines:
            source = source[:insert_pos] + line + "\n" + source[insert_pos:]
            insert_pos += len(line) + 1

    if blocks_removed:
        descriptions.append(f"removed {blocks_removed} empty extern-C block(s)")
    if extracted_count:
        descriptions.append(f"extracted {extracted_count} function(s) from blocks")

    return source, descriptions


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------

def main() -> int:
    parser = argparse.ArgumentParser(description="Extract functions from extern-C blocks")
    parser.add_argument("--source", required=True)
    parser.add_argument("--all-unmatched", action="store_true")
    parser.add_argument("--dry-run", action="store_true")
    args = parser.parse_args()

    source_path = args.source
    targets = load_targets(REPO_ROOT, source_path)
    if not targets:
        print(f"No unmatched targets for {source_path}")
        return 0

    with open(source_path, encoding="utf-8", errors="replace") as f:
        original = f.read()

    header_path = _resolve_header(source_path)
    fixed, descriptions = process_blocks(original, targets, header_path)

    if not descriptions:
        print("No functions to extract from blocks")
        return 0

    for d in descriptions:
        print(f"  FIX: {d}")

    if args.dry_run:
        print("\n--- DRY RUN ---")
        return 0

    with open(source_path, "w", encoding="utf-8", newline="\n") as f:
        f.write(fixed)

    print(f"\nApplied to {source_path}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
