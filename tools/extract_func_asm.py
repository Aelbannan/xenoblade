#!/usr/bin/env python3
"""
Extract PPC assembly for a specific function from a build/us/asm/<unit>.s file.
Used by the decomp workflow to feed assembly to subagents.
"""
import sys
import re
import json

def extract_function_assembly(asm_file, symbol):
    """
    Extract assembly for the function matching `symbol` from the .s file.
    Returns the text between .fn <symbol> and .endfn <symbol> (inclusive).
    """
    try:
        with open(asm_file, 'r') as f:
            lines = f.readlines()
    except FileNotFoundError:
        print(f"Assembly file not found: {asm_file}", file=sys.stderr)
        return None
    except OSError as e:
        print(f"Error reading {asm_file}: {e}", file=sys.stderr)
        return None

    escaped = re.escape(symbol)
    # Symbols with special chars (<> etc.) may be double-quoted in the .s file
    # e.g. .fn "__dt__Q34nw4r2ut36LinkList<...>Fv", weak
    fn_pattern = re.compile(r'^\.fn\s+"?"?' + escaped + r'"?\b')
    endfn_pattern = re.compile(r'^\.endfn\s+"?"?' + escaped + r'"?\b')

    start_idx = None
    end_idx = None

    for i, line in enumerate(lines):
        if start_idx is None and fn_pattern.match(line.strip()):
            start_idx = i
        elif start_idx is not None and endfn_pattern.match(line.strip()):
            end_idx = i
            break

    if start_idx is None:
        return None

    if end_idx is None:
        # Fallback: look for next .fn or end of file
        for i in range(start_idx + 1, len(lines)):
            if lines[i].strip().startswith('.fn ') or lines[i].strip().startswith('.endfn '):
                end_idx = i - 1
                break
        if end_idx is None:
            end_idx = len(lines) - 1

    return ''.join(lines[start_idx:end_idx + 1])


def extract_unit_assembly(build_dir, unit):
    """Build path to asm file for a unit."""
    return f"{build_dir}/{unit}.s"


if __name__ == '__main__':
    if len(sys.argv) < 3:
        print("Usage: extract_func_asm.py <asm_file> <symbol>", file=sys.stderr)
        sys.exit(1)

    asm_file = sys.argv[1]
    symbol = sys.argv[2]
    result = extract_function_assembly(asm_file, symbol)

    if result is None:
        print(f"Function '{symbol}' not found in {asm_file}", file=sys.stderr)
        sys.exit(1)

    print(result)
