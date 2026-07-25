#!/usr/bin/env python3
"""
Frontier references: recover the unknown symbols (UnkClass_*, lbl_*, func_*)
that the ready frontier of NOT_STARTED functions actually references.

Loads tools/coop/targets.json, builds the ready frontier (leaves + functions
whose callees are all FULL_MATCH/EQUIVALENT_MATCH), scans each frontier
function's retail asm section (or whole-unit fallback) for unknown-symbol
operands, and ranks them by distinct frontier-function reference count.

Usage:
  python3 tools/coop/frontier_refs.py report --top 100
  python3 tools/coop/frontier_refs.py report --json out.json
  python3 tools/coop/frontier_refs.py report --top 50 --json out.json

Output:
  Console: ranked table with symbol, address, ref-count, referencing units.
  --json: machine-readable JSON with the same data.

Stdlib only. Follows tools/coop/ style.
"""

from __future__ import annotations

import argparse
import json
import re
import sys
from collections import Counter, defaultdict
from pathlib import Path
from typing import Any, Dict, List, Optional, Tuple

# ---------------------------------------------------------------------------
# Paths
# ---------------------------------------------------------------------------
ROOT = Path(__file__).resolve().parents[2]
TARGETS_PATH = ROOT / "tools" / "coop" / "targets.json"
ASM_DIR = ROOT / "build" / "us" / "asm"

# ---------------------------------------------------------------------------
# Patterns
# ---------------------------------------------------------------------------
# Match unknown symbol references in asm instruction operands:
#   bl func_80164910__Fv
#   lwz r3, lbl_80666604__CGame@sda21(r0)
#   bl __ct__17UnkClass_8045F564Fv
#   bl createRegion__17UnkClass_8045F564FiiPCci
# We extract the base symbol: UnkClass_<addr>, lbl_<addr>, func_<addr>
# Match unknown symbol references.
# lbl_* and func_* are followed by 8 hex digits; UnkClass_* can appear
# after a mangled digit prefix ("17UnkClass_8045F564") so we allow any
# preceding character. The address hex part is exactly 8 hex digits.
UNKNOWN_SYM_RE = re.compile(
    r'(?:(?<=[^a-zA-Z0-9_])|(?<=^))(?:lbl_|func_)[0-9A-Fa-f]{8}'
    r'|UnkClass_[0-9A-Fa-f]{8}'
)

# Match a .text section header in asm:
#   # .text:0x0 | 0x800395A0 | size: 0xA8
TEXT_SECTION_RE = re.compile(
    r'^#\s+\.text:0x[0-9A-Fa-f]+\s+\|\s+(0x[0-9A-Fa-f]+)\s+\|\s+size:\s+(0x[0-9A-Fa-f]+)'
)

# Match .fn / .endfn function boundaries in asm
FN_START_RE = re.compile(r'^\.fn\s+"?([^\s,"]+)"?')  # quotes are optional, strip them
FN_END_RE = re.compile(r'^\.endfn\s+"?([^\s,"]+)"?')

# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

def load_targets() -> dict:
    """Load tools/coop/targets.json."""
    with open(TARGETS_PATH) as f:
        return json.load(f)


def base_symbol(name: str) -> str:
    """
    Strip C++ mangling suffix to get the base unknown symbol.
    
    Examples:
      UnkClass_8045F564     -> UnkClass_8045F564
      UnkClass_8045F564F     -> UnkClass_8045F564  (trailing hex chars)
      func_80164910__Fv     -> func_80164910
      func_8045D5C8__7CLibHbmFb -> func_8045D5C8
      lbl_80666604__CGame   -> lbl_80666604
    """
    # For UnkClass_*, extract base address (8 hex digits)
    m = re.search(r'(UnkClass_[0-9A-Fa-f]{8})', name)
    if m:
        return m.group(1)
    # For lbl_* and func_*, extract base (8 hex digits)
    m = re.search(r'((?:lbl|func)_[0-9A-Fa-f]{8})', name)
    if m:
        return m.group(1)
    return name


def build_ready_frontier(targets_data: dict) -> List[dict]:
    """
    Build the ready frontier:
    NOT_STARTED functions that are either:
    - leaves (empty called_functions, empty unresolved_called_functions, not has_indirect_calls)
    - OR all their called_functions are FULL_MATCH/EQUIVALENT_MATCH
      (and no unresolved_called_functions, no has_indirect_calls)
    """
    targets_list = targets_data.get("targets", [])
    if not targets_list:
        return []
    
    # Build status lookup
    status_map: Dict[str, str] = {}
    for entry in targets_list:
        tid = entry.get("id")
        if tid:
            status_map[tid] = entry.get("status", "")
    
    matched_statuses = {"FULL_MATCH", "EQUIVALENT_MATCH"}
    
    frontier = []
    for entry in targets_list:
        if entry.get("status") != "NOT_STARTED":
            continue
        
        cfs = entry.get("called_functions") or []
        unresolved = entry.get("unresolved_called_functions") or []
        has_indirect = entry.get("has_indirect_calls", False)
        
        is_leaf = not cfs and not unresolved and not has_indirect
        
        callees_accepted = False
        if cfs and not unresolved and not has_indirect:
            callees_accepted = all(
                status_map.get(cf) in matched_statuses for cf in cfs
            )
        
        if is_leaf or callees_accepted:
            frontier.append(entry)
    
    return frontier


def build_asm_address_index() -> Dict[str, List[Tuple[int, int, str]]]:
    """
    Scan all asm files and build an index mapping
    asm_file_path -> [(address_int, section_start_line, section_end_line, fn_symbol)]
    
    Uses the .text section header comments for address matching:
      # .text:0x0 | 0x800395A0 | size: 0xA8
    
    Each section spans from its .text header to the next .text header (or EOF).
    We also extract the .fn symbol from within each section.
    """
    addr_index: Dict[str, List[Tuple[int, int, int, str]]] = {}
    
    if not ASM_DIR.exists():
        print(f"Warning: asm directory {ASM_DIR} not found", file=sys.stderr)
        return addr_index
    
    asm_files = sorted(ASM_DIR.rglob("*.s"))
    print(f"Scanning {len(asm_files)} asm files for address-indexed sections...",
          file=sys.stderr)
    
    for asm_path in asm_files:
        try:
            text = asm_path.read_text()
        except Exception as e:
            print(f"  Warning: cannot read {asm_path}: {e}", file=sys.stderr)
            continue
        
        lines = text.splitlines()
        # Use path relative to ROOT for consistent matching with callgraph_source
        asm_str = str(asm_path.relative_to(ROOT))
        sections: List[Tuple[int, int, int, str]] = []
        
        # Find all .text section headers with addresses
        text_starts: List[Tuple[int, int, str]] = []  # (line_num, address_int, fn_symbol)
        
        for i, line in enumerate(lines):
            tm = TEXT_SECTION_RE.match(line)
            if tm:
                addr = int(tm.group(1), 16)
                # Look for the .fn symbol that follows (usually right after)
                fn_sym = ""
                for j in range(i + 1, min(i + 5, len(lines))):
                    fm = FN_START_RE.match(lines[j])
                    if fm:
                        fn_sym = fm.group(1)
                        break
                text_starts.append((i, addr, fn_sym))
        
        # Build sections from consecutive pairs
        for idx in range(len(text_starts)):
            start_line, addr, fn_sym = text_starts[idx]
            if idx + 1 < len(text_starts):
                end_line = text_starts[idx + 1][0]
            else:
                end_line = len(lines)
            sections.append((addr, start_line, end_line, fn_sym))
        
        if sections:
            addr_index[asm_str] = sections
    
    return addr_index


def extract_unknown_refs_from_lines(
    lines: List[str],
    start: int,
    end: int,
) -> Counter:
    """
    Scan asm lines [start, end) for unknown symbol references in instruction
    operands. Returns a Counter of {base_symbol_name: count}.
    """
    refs: Counter = Counter()
    for i in range(start, end):
        line = lines[i]
        # Only scan lines that look like instructions (with comment * /)
        if not line.strip().startswith("/*"):
            continue
        # The asm format is: /* address binary */ mnemonic operands
        # Extract the part after */ into operands
        comment_end = line.find("*/")
        if comment_end >= 0:
            operands_part = line[comment_end + 2:]
        else:
            operands_part = line
        
        for m in UNKNOWN_SYM_RE.finditer(operands_part):
            refs[base_symbol(m.group())] += 1
    
    return refs


def build_asm_ref_index(
    addr_index: Dict[str, List[Tuple[int, int, int, str]]]
) -> Dict[str, Counter]:
    """
    Pre-scan asm files to build a counter of unknown refs per function.
    Returns {asm_path: [(address_int, Counter_of_unknown_refs)]}
    """
    ref_index: Dict[str, List[Tuple[int, Counter]]] = {}
    
    for asm_path_str, sections in addr_index.items():
        asm_path = ROOT / asm_path_str
        try:
            text = asm_path.read_text()
        except Exception:
            continue
        lines = text.splitlines()
        
        func_refs: List[Tuple[int, Counter]] = []
        for addr, start, end, fn_sym in sections:
            refs = extract_unknown_refs_from_lines(lines, start, end)
            if refs:
                func_refs.append((addr, refs))
        
        if func_refs:
            ref_index[asm_path_str] = func_refs
    
    return ref_index


# ---------------------------------------------------------------------------
# Main logic
# ---------------------------------------------------------------------------

def compute_frontier_refs(
    frontier: List[dict],
    ref_index: Dict[str, List[Tuple[int, Counter]]],
    addr_index: Dict[str, List[Tuple[int, int, int, str]]],
) -> List[dict]:
    """
    For each frontier function, find its unknown-symbol references by matching
    on the function's address (from targets.json) against address-indexed
    asm sections. Falls back to symbol-name matching, then whole-unit scan
    for functions whose address does not appear in the asm index.
    
    Returns a list of dicts per unique unknown symbol:
      {
        "symbol": "UnkClass_8045F564",
        "address": "0x8045F564",
        "ref_count": 42,
        "frontier_function_count": 17,
        "referencing_units": ["kyoshin/COption", ...],
        "referencing_functions": [list of frontier function IDs]
      }
    """
    # Build reverse index: asm_path -> {address_int -> Counter}
    addr_to_refs: Dict[str, Dict[int, Counter]] = defaultdict(dict)
    for asm_path, func_refs in ref_index.items():
        for addr, refs in func_refs:
            addr_to_refs[asm_path][addr] = refs
    
    # Build set of all known (asm_path, address_int) from the full index
    all_section_addrs: Dict[str, set] = defaultdict(set)
    for asm_path, sections in addr_index.items():
        for s_addr, _s_start, _s_end, _s_sym in sections:
            all_section_addrs[asm_path].add(s_addr)
    
    # Also build symbol-index: (asm_path, fn_symbol) -> Counter
    cgs_fn_map: Dict[str, Dict[str, Counter]] = defaultdict(dict)
    for asm_path, sections in addr_index.items():
        for s_addr, _s_start, _s_end, s_sym in sections:
            if not s_sym:
                continue
            refs = addr_to_refs.get(asm_path, {}).get(s_addr, Counter())
            cgs_fn_map[asm_path][s_sym] = refs
    
    # Unknown stats accumulator
    unknown_stats: Dict[str, dict] = {}
    attributed_ids: set = set()
    
    def _record_refs(base_sym: str, count: int, cgs: str, entry_id: str):
        stats = unknown_stats.setdefault(base_sym, {
            "symbol": base_sym,
            "address": "",
            "ref_count": 0,
            "frontier_function_count": 0,
            "referencing_units": set(),
            "referencing_function_ids": set(),
        })
        stats["ref_count"] += count
        stats["frontier_function_count"] += 1
        if cgs:
            stats["referencing_units"].add(cgs)
        if entry_id:
            stats["referencing_function_ids"].add(entry_id)
    
    # Phase 1: address-based per-function matching
    for entry in frontier:
        entry_id = entry.get("id", "")
        addr_str = entry.get("address", "")
        cgs = entry.get("callgraph_source", "")
        
        if not addr_str or not cgs:
            continue
        
        try:
            addr_int = int(addr_str, 16) if addr_str.startswith("0x") else int(addr_str, 16)
        except (ValueError, TypeError):
            continue
        
        # Check if this address is in the full section index
        if cgs in all_section_addrs and addr_int in all_section_addrs[cgs]:
            refs = addr_to_refs.get(cgs, {}).get(addr_int, Counter())
            for base_sym, count in refs.items():
                _record_refs(base_sym, count, cgs, entry_id)
            attributed_ids.add(entry_id)
    
    # Phase 2: symbol-name fallback for unattributed entries
    unattributed = [e for e in frontier if e.get("id", "") not in attributed_ids]
    if unattributed:
        print(f"  Symbol-name fallback for {len(unattributed)} unattributed"
              f" frontier functions...", file=sys.stderr)
        for entry in unattributed:
            entry_id = entry.get("id", "")
            fn_sym = entry.get("symbol", "")
            cgs = entry.get("callgraph_source", "")
            if not fn_sym or not cgs:
                continue
            if cgs in cgs_fn_map and fn_sym in cgs_fn_map[cgs]:
                refs = cgs_fn_map[cgs][fn_sym]
                for base_sym, count in refs.items():
                    _record_refs(base_sym, count, cgs, entry_id)
                attributed_ids.add(entry_id)
    
    # Phase 3: whole-unit fallback for still-unattributed entries
    unattributed2 = [e for e in frontier if e.get("id", "") not in attributed_ids]
    if unattributed2:
        print(f"  Whole-unit fallback for {len(unattributed2)} still-unattributed"
              f" frontier functions across"
              f" {len(set(e.get('callgraph_source','') for e in unattributed2))}"
              f" units...", file=sys.stderr)
        
        unit_groups: Dict[str, List[dict]] = defaultdict(list)
        for entry in unattributed2:
            cgs = entry.get("callgraph_source", "")
            if cgs:
                unit_groups[cgs].append(entry)
        
        for cgs, entries in unit_groups.items():
            asm_full_path = ROOT / cgs
            if not asm_full_path.exists():
                continue
            try:
                text = asm_full_path.read_text()
            except Exception:
                continue
            lines = text.splitlines()
            whole_refs = extract_unknown_refs_from_lines(lines, 0, len(lines))
            
            for base_sym, count in whole_refs.items():
                if count == 0:
                    continue
                per_fn_count = max(1, count // len(entries))
                for entry in entries:
                    _record_refs(base_sym, per_fn_count, cgs, entry.get("id", ""))
            for entry in entries:
                attributed_ids.add(entry.get("id", ""))
    
    # The address is embedded in the symbol name itself
    # e.g. UnkClass_804630C0 -> 0x804630C0, lbl_e -> 0x804FA1E0
    for stats in unknown_stats.values():
        base = stats["symbol"]
        m = re.search(r'([0-9A-Fa-f]{8})', base)
        if m:
            stats["address"] = f"0x{m.group(1)}"
        else:
            stats["address"] = ""
    
    # Convert sets to sorted lists for output
    result = []
    for stats in unknown_stats.values():
        result.append({
            "symbol": stats["symbol"],
            "address": stats["address"],
            "ref_count": stats["ref_count"],
            "frontier_function_count": stats["frontier_function_count"],
            "referencing_units": sorted(stats["referencing_units"]),
            "referencing_function_ids": sorted(stats["referencing_function_ids"]),
        })
    
    result.sort(key=lambda x: (-x["frontier_function_count"], -x["ref_count"]))
    return result


def format_report(
    refs: List[dict],
    top_n: Optional[int] = None,
    frontier_size: int = 0,
) -> str:
    """Format a human-readable report."""
    if top_n and top_n < len(refs):
        display = refs[:top_n]
    else:
        display = refs
    
    total_refs = sum(r["ref_count"] for r in refs)
    top_refs = sum(r["ref_count"] for r in display) if display else 0
    coverage_pct = (top_refs / total_refs * 100) if total_refs > 0 else 0
    
    lines = []
    lines.append("=" * 120)
    lines.append("FRONTIER UNKNOWN SYMBOL REFERENCES")
    lines.append("=" * 120)
    lines.append(f"Frontier size: {frontier_size} NOT_STARTED functions")
    lines.append(f"Unique unknown symbols: {len(refs)}")
    lines.append(f"Total reference occurrences: {total_refs}")
    if top_n:
        lines.append(f"Top {len(display)} coverage: {coverage_pct:.1f}% of all refs")
    lines.append("")
    lines.append(
        f"{'#':<5} {'Symbol':<40} {'Address':<14} {'Refs':<8} "
        f"{'Funcs':<8} {'Units':<30}"
    )
    lines.append("-" * 120)
    
    for i, r in enumerate(display, 1):
        unit_names = r.get("referencing_units", []) or []
        units_str = ", ".join(
            Path(u).stem for u in unit_names[:3]
        )
        if len(unit_names) > 3:
            units_str += f" …(+{len(unit_names)-3})"
        lines.append(
            f"{i:<5} {r['symbol']:<40} {r['address']:<14} "
            f"{r['ref_count']:<8} {r['frontier_function_count']:<8} "
            f"{units_str:<30}"
        )
    
    lines.append("-" * 120)
    return "\n".join(lines)


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------

def main():
    parser = argparse.ArgumentParser(
        description="Frontier unknown-symbol reference recovery tool"
    )
    subparsers = parser.add_subparsers(dest="command")
    
    report_parser = subparsers.add_parser("report", help="Generate report")
    report_parser.add_argument(
        "--top", type=int, default=50,
        help="Number of top unknowns to show (default: 50)"
    )
    report_parser.add_argument(
        "--json", type=str, default=None,
        help="Write JSON output to this file"
    )
    
    args = parser.parse_args()
    
    if args.command != "report":
        parser.print_help()
        sys.exit(1)
    
    # Load and build frontier
    print("Loading targets...", file=sys.stderr)
    targets_data = load_targets()
    frontier = build_ready_frontier(targets_data)
    print(f"Ready frontier: {len(frontier)} functions", file=sys.stderr)
    
    # Build address index and reference index from asm
    print("Building asm address index...", file=sys.stderr)
    addr_index = build_asm_address_index()
    total_sections = sum(len(v) for v in addr_index.values())
    print(f"  Indexed {total_sections} sections across {len(addr_index)} files",
          file=sys.stderr)
    
    print("Building asm reference index...", file=sys.stderr)
    ref_index = build_asm_ref_index(addr_index)
    total_func_refs = sum(len(v) for v in ref_index.values())
    print(f"  Indexed {total_func_refs} function sections with refs",
          file=sys.stderr)
    
    # Compute frontier references
    print("Computing frontier references...", file=sys.stderr)
    refs = compute_frontier_refs(frontier, ref_index, addr_index)
    
    # Report
    report = format_report(refs, top_n=args.top, frontier_size=len(frontier))
    print(report)
    
    # JSON output
    if args.json:
        out_path = Path(args.json)
        out_data = {
            "meta": {
                "frontier_size": len(frontier),
                "total_unknown_symbols": len(refs),
                "total_reference_occurrences": sum(r["ref_count"] for r in refs),
            },
            "unknowns": refs,
        }
        out_path.write_text(json.dumps(out_data, indent=2))
        print(f"\nJSON written to {out_path}", file=sys.stderr)
    
    return refs


if __name__ == "__main__":
    main()
