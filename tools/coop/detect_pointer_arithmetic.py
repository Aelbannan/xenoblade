#!/usr/bin/env python3
"""
Detect raw pointer arithmetic in C/C++ source files.
6 categories: cast_byte_offset_deref, cast_byte_ptr_arith, cast_int_arith,
subscript_on_cast, ptr_offset_deref, reinterpret_arith.
Default: JSON to stdout.
"""

import argparse, json, os, re, sys, time
from collections import defaultdict
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parent.parent.parent

BYTE_T = r'(?:char|u8|uint8_t|unsigned\s+char|s8|int8_t|byte)'
INT_T  = r'(?:u32|uint32_t|unsigned\s+int|unsigned|uint|size_t|uintptr_t|s32|int32_t)'

CAST_BYTE_DEREF = re.compile(
    r'\*\s*\(\s*([a-zA-Z_]\w*(?:\s*\*)+)\s*\)\s*'
    r'\(\s*\(\s*' + BYTE_T + r'\s*\*+\s*\)\s*'
    r'([^)]+?)\)\s*[+\-]\s*'
    r'([^)]+)\)', re.IGNORECASE)

CAST_BYTE_ARITH = re.compile(
    r'\(\s*' + BYTE_T + r'\s*\*+\s*\)\s*'
    r'([^)]+)\)\s*[+\-]\s*'
    r'([^;,)]+)', re.IGNORECASE)

CAST_INT_ARITH = re.compile(
    r'\(\s*([a-zA-Z_]\w*(?:\s*\*)+)\s*\)\s*'
    r'\(\s*\(\s*' + INT_T + r'\s*\)\s*'
    r'(.+?)\s*[+\-]\s*'
    r'(.+?)\s*\)', re.IGNORECASE)

SUBSCRIPT_CAST = re.compile(
    r'\(\s*\(\s*([a-zA-Z_]\w*(?:\s*\*)+)\s*\)\s*([^)]+)\)\s*\[', re.IGNORECASE)

PTR_OFFSET_DEREF = re.compile(
    r'\*\s*\(\s*([a-zA-Z_]\w*(?:\s*\*)+)\s*\)\s*'
    r'\(\s*([a-zA-Z_]\w*)\s*[+\-]\s*'
    r'([^)]+?)\s*\)', re.IGNORECASE)

REINTERPRET_ARITH = re.compile(
    r'reinterpret_cast\s*<\s*([^>]+)\s*>\s*\(\s*'
    r'reinterpret_cast\s*<\s*' + BYTE_T + r'\s*\*+\s*>\s*'
    r'\(([^)]+)\)\s*[+\-]\s*([^)]+)\s*\)', re.IGNORECASE)

REINTERPRET_BYTE_ARITH = re.compile(
    r'reinterpret_cast\s*<\s*' + BYTE_T + r'\s*\*+\s*>\s*'
    r'\(([^)]+)\)\s*[+\-]\s*([^;,)]+)', re.IGNORECASE)

HAS_PTR_CAST = re.compile(r'\(\s*(?:' + BYTE_T + r'|[a-zA-Z_]\w*\s*\*)\s*\*?\s*\)')
HAS_REINTERPRET = re.compile(r'reinterpret_cast')

CATEGORIES = [
    'cast_byte_offset_deref', 'cast_byte_ptr_arith', 'cast_int_arith',
    'subscript_on_cast', 'ptr_offset_deref', 'reinterpret_arith',
]

CATEGORY_LABELS = {
    'cast_byte_offset_deref': '*(T*)((byte*)p +/- N)  [manual field access]',
    'cast_byte_ptr_arith':    '(byte*)p +/- N  [byte-pointer arithmetic]',
    'cast_int_arith':         '(T*)((int)p +/- N)  [int-cast arithmetic]',
    'subscript_on_cast':      '((T*)p)[N]  [subscript on cast ptr]',
    'ptr_offset_deref':       '*(T*)(p +/- N)  [raw offset deref]',
    'reinterpret_arith':      'reinterpret_cast<T*>(reint<byte*>(p)+/-N)',
}

C_KEYWORDS = frozenset({
    'if','while','for','return','sizeof','switch','case','default','break',
    'continue','goto','struct','class','enum','union','const','volatile',
    'static','extern','inline','unsigned','signed','void','char','int',
    'short','long','float','double','true','false','nullptr','NULL',
    'typedef','namespace','using','template','public','private','protected',
    'virtual','operator','new','delete','this','auto','register',
})


def find_source_files(dirs, include_ctx=False):
    for d in dirs:
        rd = REPO_ROOT / d
        if not rd.is_dir():
            continue
        for dirpath, _, filenames in os.walk(rd):
            for fn in filenames:
                if not fn.endswith(('.c','.cpp','.h','.hpp','.cxx','.hxx')):
                    continue
                if not include_ctx and '.ctx.' in fn:
                    continue
                full = Path(dirpath) / fn
                yield str(full.relative_to(REPO_ROOT)), str(full)


def scan_file(rel_path, abs_path, enabled):
    findings = []
    try:
        with open(abs_path, encoding='utf-8', errors='replace') as f:
            lines = f.readlines()
    except Exception:
        return findings

    for lineno, line in enumerate(lines, 1):
        s = line.strip()
        if not s or s.startswith('//') or s.startswith('/*') or s.startswith('*'):
            continue
        if s.startswith('#') or s.startswith('"') or s.startswith("'"):
            continue

        has_cast = HAS_PTR_CAST.search(s)
        has_reint = HAS_REINTERPRET.search(s)
        seen = set()

        def add(finding, start, end):
            for (cs, ce) in seen:
                if not (end <= cs or start >= ce):
                    return
            seen.add((start, end))
            findings.append(finding)

        if enabled.get('cast_byte_offset_deref') and has_cast:
            for m in CAST_BYTE_DEREF.finditer(s):
                add({'file':rel_path,'line':lineno,'content':s,
                     'category':'cast_byte_offset_deref',
                     'target_type':m.group(1).strip(),
                     'base_expr':m.group(2).strip(),
                     'offset':m.group(3).strip()}, m.start(), m.end())

        if enabled.get('cast_byte_ptr_arith') and has_cast:
            for m in CAST_BYTE_ARITH.finditer(s):
                add({'file':rel_path,'line':lineno,'content':s,
                     'category':'cast_byte_ptr_arith',
                     'base_expr':m.group(1).strip(),
                     'offset':m.group(2).strip()}, m.start(), m.end())

        if enabled.get('cast_int_arith') and has_cast:
            for m in CAST_INT_ARITH.finditer(s):
                add({'file':rel_path,'line':lineno,'content':s,
                     'category':'cast_int_arith',
                     'target_type':m.group(1).strip(),
                     'base_expr':m.group(2).strip(),
                     'offset':m.group(3).strip()}, m.start(), m.end())

        if enabled.get('subscript_on_cast') and has_cast:
            for m in SUBSCRIPT_CAST.finditer(s):
                add({'file':rel_path,'line':lineno,'content':s,
                     'category':'subscript_on_cast',
                     'cast_type':m.group(1).strip(),
                     'base_expr':m.group(2).strip()}, m.start(), m.end())

        if enabled.get('ptr_offset_deref'):
            for m in PTR_OFFSET_DEREF.finditer(s):
                vn = m.group(2)
                if vn.isdigit() or vn.startswith('0x') or vn in C_KEYWORDS:
                    continue
                add({'file':rel_path,'line':lineno,'content':s,
                     'category':'ptr_offset_deref',
                     'target_type':m.group(1).strip(),
                     'variable':vn,
                     'offset':m.group(3).strip()}, m.start(), m.end())

        if enabled.get('reinterpret_arith') and has_reint:
            for m in REINTERPRET_ARITH.finditer(s):
                add({'file':rel_path,'line':lineno,'content':s,
                     'category':'reinterpret_arith',
                     'target_type':m.group(1).strip(),
                     'base_expr':m.group(2).strip(),
                     'offset':m.group(3).strip()}, m.start(), m.end())
            for m in REINTERPRET_BYTE_ARITH.finditer(s):
                add({'file':rel_path,'line':lineno,'content':s,
                     'category':'reinterpret_arith',
                     'base_expr':m.group(1).strip(),
                     'offset':m.group(2).strip()}, m.start(), m.end())

    return findings


def format_text(findings, by_file, summary):
    w = 80
    print("=" * w)
    print("  POINTER ARITHMETIC DETECTION REPORT")
    print("=" * w)
    print(f"\n  Total: {summary['total_findings']}  |  Files: {summary['files_with_findings']}/{summary['files_scanned']}  |  Time: {summary.get('scan_time_sec','?'):.1f}s")
    for cat, count in sorted(summary['by_category'].items()):
        print(f"  {CATEGORY_LABELS.get(cat, cat):60s} {count:5d}")
    print("\n" + "-" * w)
    print("  TOP FILES")
    print("-" * w)
    for fp, ff in sorted(by_file.items(), key=lambda x: len(x[1]), reverse=True)[:30]:
        cats = defaultdict(int)
        for f in ff: cats[f['category']] += 1
        cs = '  '.join(f'{c}:{n}' for c,n in sorted(cats.items()))
        print(f"  {len(ff):4d}  {fp}     {cs}")
    print("\n" + "-" * w)
    print("  DETAIL")
    print("-" * w)
    for fp in sorted(by_file):
        ff = by_file[fp]
        print(f"\n  [{fp}]  ({len(ff)})")
        for f in ff:
            extra = '  '.join(f'{k}={f[k]}' for k in ('target_type','cast_type','variable','base_expr','offset') if k in f)
            c = f['content']
            if len(c) > 130: c = c[:127] + '...'
            print(f"    L{f['line']:5d} [{f['category']:28s}] {extra}\n           {c}")


def main():
    p = argparse.ArgumentParser(description='Detect raw pointer arithmetic in C/C++ source files')
    p.add_argument('--dirs', nargs='+', default=['src','libs'])
    p.add_argument('--text', action='store_true', help='Human-readable output')
    p.add_argument('--summary', action='store_true', help='Summary only')
    p.add_argument('--source', type=str, help='Filter by source file substring')
    p.add_argument('--category', type=str, choices=CATEGORIES)
    p.add_argument('--exclude', nargs='+', default=[])
    p.add_argument('--include-ctx', action='store_true')
    p.add_argument('--disable', nargs='+', default=[], choices=CATEGORIES)
    p.add_argument('--limit', type=int, default=0)
    args = p.parse_args()

    enabled = {c: c not in args.disable for c in CATEGORIES}
    t0 = time.time()
    all_files = list(find_source_files(args.dirs, include_ctx=args.include_ctx))
    total = len(all_files)

    all_findings = []
    for rel, abs_path in all_files:
        if any(ex in rel for ex in args.exclude):
            continue
        if args.source and args.source not in rel:
            continue
        all_findings.extend(scan_file(rel, abs_path, enabled))

    if args.category:
        all_findings = [f for f in all_findings if f['category'] == args.category]
    if args.limit:
        all_findings = all_findings[:args.limit]

    by_cat = defaultdict(int)
    by_file = defaultdict(list)
    for f in all_findings:
        by_cat[f['category']] += 1
        by_file[f['file']].append(f)

    summary = {
        'total_findings': len(all_findings),
        'files_with_findings': len(by_file),
        'files_scanned': total,
        'by_category': dict(by_cat),
        'scan_time_sec': round(time.time() - t0, 2),
    }

    if args.text:
        format_text(all_findings, dict(by_file), summary)
    else:
        out = {'summary': summary}
        if not args.summary:
            out['by_file'] = {k: v for k, v in sorted(by_file.items())}
            out['findings'] = all_findings
        print(json.dumps(out, indent=2, default=str))


if __name__ == '__main__':
    main()