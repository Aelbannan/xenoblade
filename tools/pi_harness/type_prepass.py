#!/usr/bin/env python3
"""
Type prepass proof-of-concept: analyze retail ASM to infer struct layouts.

Outputs suggested header changes without applying them. Run with:
    .venv/bin/python3 tools/pi_harness/type_prepass.py --tu kyoshin/cf/voice/cvsys/CVS_THREAD_DOWN
    .venv/bin/python3 tools/pi_harness/type_prepass.py --tu kyoshin/cf/voice/cvsys/CVS_THREAD_DOWN --all
"""

from __future__ import annotations

import argparse
import json
import re
import sys
from collections import defaultdict
from dataclasses import dataclass, field
from pathlib import Path
from typing import Optional

ROOT = Path(__file__).resolve().parents[2]

# ---------------------------------------------------------------------------
# Data structures
# ---------------------------------------------------------------------------

@dataclass
class FieldAccess:
    """A single field access in retail ASM."""
    symbol: str           # function containing this access
    offset: int           # byte offset from base register
    base_reg: str         # r3, r4, r31, etc.
    access_type: str      # "load" or "store"
    width: int            # 1, 2, or 4 bytes
    line_num: int         # line in ASM
    raw_line: str         # original ASM line

@dataclass
class FieldInfo:
    """Inferred information about a struct field."""
    offset: int
    width: int
    access_type: str  # "load", "store", "both"
    used_in_functions: set[str] = field(default_factory=set)
    
    @property
    def name(self) -> str:
        return f"field_0x{self.offset:04X}"
    
    @property
    def c_type(self) -> str:
        if self.width == 1:
            return "u8"
        elif self.width == 2:
            return "u16"
        elif self.width == 4:
            return "u32"
        return f"u8[{self.width}]"

@dataclass
class InferredStruct:
    """A struct inferred from field accesses."""
    name: str
    source_functions: set[str] = field(default_factory=set)
    fields: dict[int, FieldInfo] = field(default_factory=dict)
    base_register: str = ""  # which register holds the pointer
    
    @property
    def size(self) -> int:
        if not self.fields:
            return 0
        max_offset = max(self.fields.keys())
        max_field = self.fields[max_offset]
        return max_offset + max_field.width

@dataclass
class HeaderSuggestion:
    """A suggested change to a header file."""
    file: str
    action: str  # "create", "update", "add-field"
    struct_name: str
    fields: list[FieldInfo]
    reason: str

# ---------------------------------------------------------------------------
# ASM parsing
# ---------------------------------------------------------------------------

# Pattern to match field accesses: lwz/stw/lbz/lhz/stb/sth rX, N(rY)
FIELD_ACCESS_RE = re.compile(
    r'\b(lwz|stw|lbz|lhz|stb|sth|lwzu|stwu)\s+'
    r'(r\d+),\s*'
    r'(-?0x[0-9a-fA-F]+|-?\d+)\(r(\d+)\)'
)

# Pattern to match register aliases: mr rX, rY
MR_RE = re.compile(r'\bmr\s+(r\d+),\s*(r\d+)')

# Pattern to match function definition
FN_RE = re.compile(r'^\.fn\s+(\S+),')

# Stack frame offsets to ignore (r1-based)
STACK_OFFSETS = {0x08, 0x0C, 0x10, 0x14, 0x18, 0x1C, 0x20, 0x24, 0x28, 0x2C, 
                 0x30, 0x34, 0x38, 0x3C, 0x40, 0x44, 0x48, 0x4C, 0x50}

# Width mapping for access types
ACCESS_WIDTH = {
    'lbz': 1, 'stb': 1,
    'lhz': 2, 'sth': 2,
    'lwz': 4, 'stw': 4,
    'lwzu': 4, 'stwu': 4,
}

def parse_asm_field_accesses(asm: str, symbol: str) -> list[FieldAccess]:
    """Extract field accesses from a function's retail ASM."""
    accesses = []
    lines = asm.split('\n')
    
    # Track register aliases (mr rX, rY)
    aliases: dict[str, str] = {}
    
    for i, line in enumerate(lines):
        # Check for register alias
        mr_match = MR_RE.search(line)
        if mr_match:
            dst, src = mr_match.groups()
            aliases[dst] = aliases.get(src, src)
        
        # Check for field access
        match = FIELD_ACCESS_RE.search(line)
        if not match:
            continue
        
        op, reg, offset_str, base_reg = match.groups()
        
        # Parse offset
        try:
            offset = int(offset_str, 0) if offset_str.startswith('0x') or offset_str.startswith('-0x') else int(offset_str)
        except ValueError:
            continue
        
        # Skip stack-based accesses (r1)
        if base_reg == '1':
            continue
        
        # Skip small offsets on any register (likely vtable or small struct)
        # But keep offsets > 0x10 which are likely real fields
        if abs(offset) < 0x10:
            continue
        
        # Resolve alias
        actual_reg = aliases.get(base_reg, base_reg)
        
        width = ACCESS_WIDTH.get(op, 4)
        access_type = 'load' if op.startswith('l') else 'store'
        
        accesses.append(FieldAccess(
            symbol=symbol,
            offset=offset,
            base_reg=actual_reg,
            access_type=access_type,
            width=width,
            line_num=i,
            raw_line=line.strip()
        ))
    
    return accesses

# ---------------------------------------------------------------------------
# Struct deduction
# ---------------------------------------------------------------------------

def deduce_structs(accesses: list[FieldAccess]) -> dict[str, InferredStruct]:
    """Group field accesses into inferred structs."""
    
    # Group by (function, base_register) to identify distinct objects
    # Then merge groups that access the same offsets
    
    # Step 1: Group by (symbol, base_reg)
    groups: dict[tuple[str, str], list[FieldAccess]] = defaultdict(list)
    for acc in accesses:
        groups[(acc.symbol, acc.base_reg)].append(acc)
    
    # Step 2: For each group, collect the set of offsets
    offset_sets: dict[tuple[str, str], set[int]] = {}
    for key, accs in groups.items():
        offset_sets[key] = {a.offset for a in accs}
    
    # Step 3: Merge groups with overlapping offset sets
    # This is a simple heuristic - groups sharing 50%+ of offsets are merged
    merged: list[set[tuple[str, str]]] = []
    keys = list(offset_sets.keys())
    used = set()
    
    for i, key1 in enumerate(keys):
        if key1 in used:
            continue
        
        cluster = {key1}
        used.add(key1)
        
        for j, key2 in enumerate(keys[i+1:], i+1):
            if key2 in used:
                continue
            
            # Check overlap
            set1 = offset_sets[key1]
            set2 = offset_sets[key2]
            if not set1 or not set2:
                continue
            
            overlap = len(set1 & set2)
            min_size = min(len(set1), len(set2))
            
            if overlap >= min_size * 0.5:  # 50% overlap threshold
                cluster.add(key2)
                used.add(key2)
        
        merged.append(cluster)
    
    # Step 4: Build InferredStruct for each cluster
    structs: dict[str, InferredStruct] = {}
    
    for cluster_idx, cluster in enumerate(merged):
        # Collect all accesses for this cluster
        cluster_accesses: list[FieldAccess] = []
        functions: set[str] = set()
        
        for symbol, base_reg in cluster:
            cluster_accesses.extend(groups[(symbol, base_reg)])
            functions.add(symbol)
        
        # Determine the most common base register
        reg_counts: dict[str, int] = defaultdict(int)
        for acc in cluster_accesses:
            reg_counts[acc.base_reg] += 1
        primary_reg = max(reg_counts, key=reg_counts.get)
        
        # Build field info
        fields: dict[int, FieldInfo] = {}
        for acc in cluster_accesses:
            if acc.offset not in fields:
                fields[acc.offset] = FieldInfo(
                    offset=acc.offset,
                    width=acc.width,
                    access_type=acc.access_type,
                    used_in_functions={acc.symbol}
                )
            else:
                existing = fields[acc.offset]
                existing.used_in_functions.add(acc.symbol)
                if acc.access_type != existing.access_type:
                    existing.access_type = "both"
                # Keep the wider access
                if acc.width > existing.width:
                    existing.width = acc.width
        
        # Name the struct based on context
        struct_name = f"UnknownStruct_{cluster_idx}"
        
        structs[struct_name] = InferredStruct(
            name=struct_name,
            source_functions=functions,
            fields=fields,
            base_register=primary_reg
        )
    
    return structs

# ---------------------------------------------------------------------------
# Header comparison
# ---------------------------------------------------------------------------

def load_existing_header(header_path: Path) -> Optional[str]:
    """Load an existing header file."""
    if header_path.exists():
        return header_path.read_text()
    return None

def parse_existing_fields(header_content: str) -> dict[int, int]:
    """Parse existing field offsets and widths from a header."""
    fields: dict[int, int] = {}
    
    # Match field_0xNN patterns
    field_re = re.compile(r'field_0x([0-9a-fA-F]+)\s*;')
    for match in field_re.finditer(header_content):
        offset = int(match.group(1), 16)
        # Assume 4 bytes for now (could be improved)
        fields[offset] = 4
    
    return fields

def compare_with_existing(
    inferred: dict[str, InferredStruct],
    header_path: Path,
    struct_name: str
) -> list[HeaderSuggestion]:
    """Compare inferred structs with existing header."""
    suggestions: list[HeaderSuggestion] = []
    
    header_content = load_existing_header(header_path)
    if not header_content:
        # Header doesn't exist - would need to create it
        for name, struct in inferred.items():
            if struct.fields:
                suggestions.append(HeaderSuggestion(
                    file=str(header_path),
                    action="create",
                    struct_name=name,
                    fields=list(struct.fields.values()),
                    reason=f"Header doesn't exist. Struct has {len(struct.fields)} fields."
                ))
        return suggestions
    
    # Parse existing fields
    existing_fields = parse_existing_fields(header_content)
    
    # Check each inferred struct
    for name, struct in inferred.items():
        missing_fields = []
        
        for offset, field_info in struct.fields.items():
            if offset not in existing_fields:
                missing_fields.append(field_info)
        
        if missing_fields:
            suggestions.append(HeaderSuggestion(
                file=str(header_path),
                action="add-field",
                struct_name=name,
                fields=missing_fields,
                reason=f"Missing {len(missing_fields)} fields: {', '.join('field_0x' + format(f.offset, '04X') for f in missing_fields[:5])}"
            ))
    
    return suggestions

# ---------------------------------------------------------------------------
# Special pattern detection
# ---------------------------------------------------------------------------

@dataclass
class SpecialPattern:
    """A special pattern detected in ASM."""
    pattern_type: str  # "vtable_call", "bit_test", "null_check", etc.
    symbol: str
    offset: int
    details: str

def detect_special_patterns(asm: str, symbol: str) -> list[SpecialPattern]:
    """Detect special patterns like vtable calls, bit tests, etc."""
    patterns = []
    lines = asm.split('\n')
    
    for i, line in enumerate(lines):
        # Vtable call pattern: lwz r12, N(rX); lwz r12, M(r12); mtctr r12; bctrl
        if 'lwz r12,' in line and i + 3 < len(lines):
            if 'lwz r12,' in lines[i+1] and 'mtctr r12' in lines[i+2] and 'bctrl' in lines[i+3]:
                # Extract vtable offset
                match = re.search(r'lwz r12,\s*(-?0x[0-9a-fA-F]+|-?\d+)\(r(\d+)\)', line)
                if match:
                    offset_str, reg = match.groups()
                    offset = int(offset_str, 0)
                    patterns.append(SpecialPattern(
                        pattern_type="vtable_load",
                        symbol=symbol,
                        offset=offset,
                        details=f"Load vtable pointer from r{reg}+0x{offset:X}"
                    ))
        
        # Bit test pattern: rlwinm. rX, rY, 0, N, N
        if 'rlwinm.' in line:
            match = re.search(r'rlwinm\.\s+r(\d+),\s*r(\d+),\s*0,\s*(\d+),\s*(\d+)', line)
            if match:
                dst, src, bit_start, bit_end = match.groups()
                if bit_start == bit_end:
                    patterns.append(SpecialPattern(
                        pattern_type="bit_test",
                        symbol=symbol,
                        offset=0,  # Would need to track the source register
                        details=f"Test bit {bit_start} of r{src}"
                    ))
        
        # Null check: cmpwi rX, 0; beq/bne
        if 'cmpwi' in line and i + 1 < len(lines):
            if 'beq' in lines[i+1] or 'bne' in lines[i+1]:
                match = re.search(r'cmpwi\s+r(\d+),\s*0', line)
                if match:
                    patterns.append(SpecialPattern(
                        pattern_type="null_check",
                        symbol=symbol,
                        offset=0,
                        details=f"Check if r{match.group(1)} is null"
                    ))
    
    return patterns

# ---------------------------------------------------------------------------
# Main analysis
# ---------------------------------------------------------------------------

def load_targets(repo_root: Path, region: str, unit: str) -> list[dict]:
    """Load targets for a unit from targets.json."""
    targets_path = repo_root / "tools" / "coop" / "targets.json"
    with open(targets_path) as f:
        data = json.load(f)
    
    targets = []
    for t in data.get("targets", []):
        if t.get("unit") == unit:
            if t.get("region") and t.get("region") != region:
                continue
            targets.append(t)
    
    return targets

def extract_retail_asm(repo_root: Path, region: str, unit: str, symbol: str) -> str:
    """Extract retail ASM for a symbol."""
    # Try to find the ASM listing
    # The ASM files are in build/<region>/asm/<unit>.s
    asm_path = repo_root / "build" / region / "asm" / f"{unit}.s"
    
    if not asm_path.exists():
        # Try with .s extension
        asm_path = repo_root / "build" / region / "asm" / f"{unit}.s"
    
    if not asm_path.exists():
        return ""
    
    content = asm_path.read_text()
    
    # Find the function
    start_pattern = re.compile(rf'^\.fn\s+"?{re.escape(symbol)}"?\s*,.*$', re.MULTILINE)
    start = start_pattern.search(content)
    if not start:
        return ""
    
    end_pattern = re.compile(rf'^\.endfn\s+"?{re.escape(symbol)}"?\s*$', re.MULTILINE)
    end = end_pattern.search(content, start.end())
    if not end:
        return ""
    
    return content[start.start():end.end()]

def analyze_unit(repo_root: Path, region: str, unit: str, verbose: bool = False) -> dict:
    """Analyze a unit and return inferred types."""
    
    targets = load_targets(repo_root, region, unit)
    if not targets:
        return {"error": f"No targets found for {unit}"}
    
    all_accesses: list[FieldAccess] = []
    all_patterns: list[SpecialPattern] = []
    functions_analyzed: list[str] = []
    
    for target in targets:
        symbol = target.get("symbol", "")
        if not symbol:
            continue
        
        asm = extract_retail_asm(repo_root, region, unit, symbol)
        if not asm:
            if verbose:
                print(f"  [skip] {symbol}: no retail ASM found")
            continue
        
        if verbose:
            print(f"  [analyze] {symbol}")
        
        accesses = parse_asm_field_accesses(asm, symbol)
        all_accesses.extend(accesses)
        functions_analyzed.append(symbol)
        
        patterns = detect_special_patterns(asm, symbol)
        all_patterns.extend(patterns)
    
    if not all_accesses:
        return {
            "unit": unit,
            "functions_analyzed": functions_analyzed,
            "error": "No field accesses found"
        }
    
    # Deduce structs
    structs = deduce_structs(all_accesses)
    
    # Load existing headers
    unit_parts = unit.split("/")
    header_dir = repo_root / "src" / "/".join(unit_parts[:-1])
    header_name = unit_parts[-1] + ".hpp"
    header_path = header_dir / header_name
    
    # Compare with existing
    suggestions = compare_with_existing(structs, header_path, unit_parts[-1])
    
    # Build result
    result = {
        "unit": unit,
        "functions_analyzed": functions_analyzed,
        "total_accesses": len(all_accesses),
        "structs": {},
        "special_patterns": [],
        "suggestions": [],
        "existing_header": str(header_path) if header_path.exists() else None,
    }
    
    for name, struct in structs.items():
        result["structs"][name] = {
            "size": struct.size,
            "field_count": len(struct.fields),
            "fields": [
                {
                    "offset": f"0x{f.offset:04X}",
                    "width": f.width,
                    "type": f.c_type,
                    "access": f.access_type,
                    "name": f.name,
                    "used_in": list(f.used_in_functions),
                }
                for f in sorted(struct.fields.values(), key=lambda x: x.offset)
            ],
            "source_functions": list(struct.source_functions),
            "base_register": struct.base_register,
        }
    
    for pattern in all_patterns:
        result["special_patterns"].append({
            "type": pattern.pattern_type,
            "symbol": pattern.symbol,
            "offset": f"0x{pattern.offset:X}" if pattern.offset else None,
            "details": pattern.details,
        })
    
    for suggestion in suggestions:
        result["suggestions"].append({
            "file": suggestion.file,
            "action": suggestion.action,
            "struct": suggestion.struct_name,
            "field_count": len(suggestion.fields),
            "fields": [f.name for f in suggestion.fields],
            "reason": suggestion.reason,
        })
    
    return result

# ---------------------------------------------------------------------------
# Output formatting
# ---------------------------------------------------------------------------

def print_analysis(result: dict, verbose: bool = False) -> None:
    """Print analysis results in a human-readable format."""
    
    if "error" in result:
        print(f"Error: {result['error']}")
        return
    
    unit = result["unit"]
    print(f"\n{'='*60}")
    print(f"Type Prepass Analysis: {unit}")
    print(f"{'='*60}")
    
    print(f"\nFunctions analyzed: {len(result['functions_analyzed'])}")
    for func in result["functions_analyzed"]:
        print(f"  - {func}")
    
    print(f"\nTotal field accesses: {result['total_accesses']}")
    
    # Print inferred structs
    if result["structs"]:
        print(f"\n{'─'*60}")
        print("Inferred Structs:")
        print(f"{'─'*60}")
        
        for name, struct_info in result["structs"].items():
            print(f"\n  {name} (size: 0x{struct_info['size']:X} = {struct_info['size']} bytes)")
            print(f"  Base register: {struct_info['base_register']}")
            print(f"  Source functions: {', '.join(struct_info['source_functions'])}")
            print(f"  Fields ({struct_info['field_count']}):")
            
            for field_info in struct_info["fields"]:
                print(f"    {field_info['offset']:>8s}  {field_info['type']:<6s}  {field_info['name']:<16s}  "
                      f"({field_info['access']})  used in: {', '.join(field_info['used_in'][:3])}")
    
    # Print special patterns
    if result["special_patterns"] and verbose:
        print(f"\n{'─'*60}")
        print("Special Patterns:")
        print(f"{'─'*60}")
        
        for pattern in result["special_patterns"]:
            offset_str = f" @ {pattern['offset']}" if pattern['offset'] else ""
            print(f"  [{pattern['type']}] {pattern['symbol']}{offset_str}")
            print(f"    {pattern['details']}")
    
    # Print suggestions
    if result["suggestions"]:
        print(f"\n{'─'*60}")
        print("Header Suggestions:")
        print(f"{'─'*60}")
        
        for suggestion in result["suggestions"]:
            print(f"\n  File: {suggestion['file']}")
            print(f"  Action: {suggestion['action']}")
            print(f"  Struct: {suggestion['struct']}")
            print(f"  Fields to add: {suggestion['field_count']}")
            print(f"  Reason: {suggestion['reason']}")
            
            if suggestion["fields"]:
                print(f"  Field names: {', '.join(suggestion['fields'][:10])}")
    
    # Print existing header info
    if result["existing_header"]:
        print(f"\n{'─'*60}")
        print(f"Existing header: {result['existing_header']}")
    else:
        print(f"\n{'─'*60}")
        print("No existing header found")

# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------

def main():
    parser = argparse.ArgumentParser(
        description="Type prepass: analyze retail ASM to infer struct layouts"
    )
    parser.add_argument(
        "--tu",
        help="Translation unit to analyze (e.g., kyoshin/cf/voice/cvsys/CVS_THREAD_DOWN)"
    )
    parser.add_argument(
        "--region", default="us",
        help="Region to analyze (default: us)"
    )
    parser.add_argument(
        "--all", action="store_true",
        help="Analyze all CVS_THREAD_* units"
    )
    parser.add_argument(
        "--verbose", "-v", action="store_true",
        help="Show detailed output"
    )
    parser.add_argument(
        "--json", action="store_true",
        help="Output as JSON"
    )
    
    args = parser.parse_args()
    
    if not args.tu and not args.all:
        parser.error("--tu or --all is required")
    
    repo_root = ROOT
    
    if args.all:
        # Analyze all CVS_THREAD units
        units = [
            "kyoshin/cf/voice/cvsys/CVS_THREAD_DOWN",
            "kyoshin/cf/voice/cvsys/CVS_THREAD_HAGE",
            "kyoshin/cf/voice/cvsys/CVS_THREAD_TENSION_UP",
            "kyoshin/cf/voice/cvsys/CVS_THREAD_VISION_BREAK",
            "kyoshin/cf/voice/cvsys/CVS_THREAD_VISION_TELL",
        ]
    else:
        units = [args.tu]
    
    all_results = []
    
    for unit in units:
        result = analyze_unit(repo_root, args.region, unit, verbose=args.verbose)
        all_results.append(result)
        
        if not args.json:
            print_analysis(result, verbose=args.verbose)
    
    if args.json:
        print(json.dumps(all_results, indent=2))

if __name__ == "__main__":
    main()
