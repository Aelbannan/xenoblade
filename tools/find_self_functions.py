#!/usr/bin/env python3
"""
Find all functions where the first parameter is named 'self'.

These are very likely C++ class methods that haven't been fully converted
to member-function syntax yet (i.e., the 'this' pointer is passed explicitly
as the first argument instead of being implicit).

Usage:
  python3 tools/find_self_functions.py                     # default flat output
  python3 tools/find_self_functions.py -g                  # group by type of self
  python3 tools/find_self_functions.py -f                  # group by file
  python3 tools/find_self_functions.py -c                  # count only
  python3 tools/find_self_functions.py --json              # JSON output
  python3 tools/find_self_functions.py path/to/subdir      # search specific dir
"""

import os
import re
import sys
from collections import defaultdict


def _clean_rest(rest: str) -> str:
    """Strip leading comma and trailing closing paren from a rest-of-params string."""
    r = rest.strip()
    if r.startswith(','):
        r = r[1:].strip()
    if r.endswith(')'):
        r = r[:-1].strip()
    return r


def find_self_functions_multiline(root_dir: str) -> list[dict]:
    """
    Recursively search .cpp, .hpp, .h, .c files for function definitions
    whose first parameter is named 'self'.  Handles multi-line signatures.
    """
    results = []
    ext_re = re.compile(r'\.(?:cpp|hpp|h|c)$', re.IGNORECASE)

    # A line that looks like the start of a function definition (ends with open paren)
    func_start_re = re.compile(
        r'^\s*'
        r'(?P<ret>(?:static\s+|inline\s+|virtual\s+|extern\s+"C"\s+|const\s+)*'
        r'(?:unsigned\s+|signed\s+|short\s+|long\s+|volatile\s+|const\s+)*'
        r'[\w:<>*&\s]+?)\s*'
        r'(?P<name>(?:operator\s*[^\s(]+|~?[\w:<>&*]+))\s*'
        r'\(\s*$'
    )

    # A line (inside a param list) that starts with '<type> self'
    self_param_re = re.compile(
        r'^\s*(?:const\s+)?(?P<type>[\w:<>*&\s]+?)\s+self\b\s*(?P<rest>[),].*)?$'
    )

    # Single-line function def with 'self' as first param
    single_line_re = re.compile(
        r'^\s*'
        r'(?P<ret>(?:static\s+|inline\s+|virtual\s+|extern\s+"C"\s+|const\s+)*'
        r'(?:unsigned\s+|signed\s+|short\s+|long\s+|volatile\s+|const\s+)*'
        r'[\w:<>*&\s]+?)\s*'
        r'(?P<name>(?:operator\s*[^\s(]+|~?[\w:<>&*]+))\s*'
        r'\(\s*'
        r'(?:const\s+)?(?P<type>[\w:<>*&\s]+?)\s+'
        r'self\b'
        r'(?P<rest>[^)]*)\)'
    )

    for dirpath, dirnames, filenames in os.walk(root_dir):
        dirnames[:] = [d for d in dirnames if d not in {
            'build', 'orig', 'tools', '.git', 'venv', 'node_modules',
            '__pycache__', '.agents', '.pi'
        }]

        for fname in filenames:
            if not ext_re.search(fname):
                continue

            fpath = os.path.join(dirpath, fname)
            try:
                with open(fpath, 'r', encoding='utf-8', errors='replace') as f:
                    lines = f.readlines()
            except OSError:
                continue

            relpath = os.path.relpath(fpath, root_dir)
            i = 0
            while i < len(lines):
                line = lines[i]

                # --- single-line fast path ---
                sl_m = single_line_re.search(line)
                if sl_m and not (line.rstrip().endswith(';') and '{' not in line):
                    type_str = sl_m.group('type').strip()
                    ret = sl_m.group('ret').strip()
                    name = sl_m.group('name').strip()
                    rest = sl_m.group('rest').strip()
                    clean = _clean_rest(rest)
                    sig = f"{ret} {name}({type_str} self"
                    if clean:
                        sig += f", {clean}"
                    sig += ")"
                    results.append({
                        'file': relpath,
                        'line': i + 1,
                        'type_str': type_str,
                        'name': name,
                        'ret_type': ret,
                        'signature': sig,
                    })
                    i += 1
                    continue

                # --- multi-line check ---
                if func_start_re.search(line):
                    j = i + 1
                    found_self = False
                    self_type = ""
                    rest = ""
                    exceeded = 15
                    while j < len(lines) and exceeded > 0:
                        spm = self_param_re.search(lines[j])
                        if spm:
                            found_self = True
                            self_type = spm.group('type').strip() if spm.group('type') else ""
                            rest = spm.group('rest') or ""
                            j += 1
                            while j < len(lines) and ')' not in lines[j - 1] and exceeded > 0:
                                rest += lines[j].strip()
                                j += 1
                                exceeded -= 1
                            break
                        if ')' in lines[j] or '{' in lines[j] or ';' in lines[j]:
                            break
                        j += 1
                        exceeded -= 1

                    if found_self:
                        sl_m2 = re.compile(
                            r'^\s*'
                            r'(?P<ret>(?:static\s+|inline\s+|virtual\s+|extern\s+"C"\s+|const\s+)*'
                            r'(?:unsigned\s+|signed\s+|short\s+|long\s+|volatile\s+|const\s+)*'
                            r'[\w:<>*&\s]+?)\s*'
                            r'(?P<name>(?:operator\s*[^\s(]+|~?[\w:<>&*]+))\s*'
                            r'\('
                        ).search(line)
                        if sl_m2:
                            ret = sl_m2.group('ret').strip()
                            name = sl_m2.group('name').strip()
                            clean = _clean_rest(rest)
                            sig = f"{ret} {name}({self_type} self"
                            if clean:
                                sig += f", {clean}"
                            sig += ")"
                            results.append({
                                'file': relpath,
                                'line': i + 1,
                                'type_str': self_type,
                                'name': name,
                                'ret_type': ret,
                                'signature': sig,
                            })
                            i = j
                            continue

                i += 1

    return results


def main():
    import argparse
    ap = argparse.ArgumentParser(
        description="Find functions whose first parameter is named 'self' "
                    "(likely class methods in hiding)"
    )
    ap.add_argument('dir', nargs='?', default='src',
                    help='Root directory to search (default: src)')
    ap.add_argument('--group-by-class', '-g', action='store_true',
                    help='Group results by the type of self')
    ap.add_argument('--by-file', '-f', action='store_true',
                    help='Group results by file')
    ap.add_argument('--json', '-j', action='store_true',
                    help='Output as JSON')
    ap.add_argument('--count-only', '-c', action='store_true',
                    help='Only print the count')
    ap.add_argument('--min-count', '-m', type=int, default=0,
                    help='With -g: only show types with at least N functions')
    args = ap.parse_args()

    root = args.dir
    if not os.path.isdir(root):
        print(f"Error: '{root}' is not a directory", file=sys.stderr)
        sys.exit(1)

    results = find_self_functions_multiline(root)

    # Deduplicate by (file, line, name)
    seen = set()
    unique = []
    for r in results:
        key = (r['file'], r['line'], r['name'])
        if key not in seen:
            seen.add(key)
            unique.append(r)
    results = unique

    if args.count_only:
        print(f"Found {len(results)} function(s) with 'self' as first parameter")
        type_counts = defaultdict(int)
        for r in results:
            type_counts[r['type_str']] += 1
        print(f"\nUnique first-param types: {len(type_counts)}")
        if type_counts:
            print("\nTop types:")
            for t, n in sorted(type_counts.items(), key=lambda x: -x[1])[:20]:
                print(f"  {t}: {n}")
        return

    if args.json:
        import json
        print(json.dumps(results, indent=2))
        return

    if args.group_by_class:
        grouped = defaultdict(list)
        for r in results:
            grouped[r['type_str']].append(r)
        for type_str in sorted(grouped.keys()):
            funcs = grouped[type_str]
            if len(funcs) < args.min_count:
                continue
            print(f"\n{'='*60}")
            print(f"  self type: {type_str}  ({len(funcs)} function(s))")
            print(f"{'='*60}")
            for r in sorted(funcs, key=lambda x: (x['file'], x['line'])):
                print(f"  {r['file']}:{r['line']}  {r['name']}")
                print(f"    {r['signature'].strip()}")
    elif args.by_file:
        grouped = defaultdict(list)
        for r in results:
            grouped[r['file']].append(r)
        for fpath in sorted(grouped.keys()):
            funcs = grouped[fpath]
            print(f"\n--- {fpath} ({len(funcs)}) ---")
            for r in sorted(funcs, key=lambda x: x['line']):
                print(f"  L{r['line']:>5}:  {r['type_str']:30s}  {r['signature'].strip()}")
    else:
        for r in sorted(results, key=lambda x: (x['file'], x['line'])):
            print(f"{r['file']}:{r['line']}\t{r['type_str']}\t{r['signature'].strip()}")


if __name__ == '__main__':
    main()