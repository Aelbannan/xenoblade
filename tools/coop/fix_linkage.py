#!/usr/bin/env python3
"""Add extern "C" to definitions whose MWCC mangling differs from the retail
literal, with local safety guards:
  * skip names declared in any header the TU sees (linkage conflict)
  * skip names that also have a declaration in the same file
  * skip if the definition already carries extern "C"
  * after rebuilding, revert any patch that *removed* a previously defined
    symbol from the object, and revert patches named in compile errors
Usage: linkage_batch.py <src/unit.cpp> <splits-unit-path> [--dry]
"""
import argparse, json, pathlib, re, subprocess, sys

ROWS_JSON = 'build/us/main.elf.undefs.json'

ROOT = pathlib.Path('.')
ROWS: list = []  # loaded in main() from --rows (tools/coop/link_undefs.py --json)
NM = 'build/binutils/powerpc-eabi-nm'


def build(path: pathlib.Path) -> str:
    out = subprocess.run(['.venv/bin/python3', '.scratch/e_units/withlock.py', 'ninja', str(path)],
                         capture_output=True, text=True)
    return out.stdout + out.stderr


def obj_path(src: str) -> pathlib.Path:
    p = pathlib.Path(src)
    rel = p.relative_to('src').with_suffix('.o')
    return pathlib.Path('build/us/src') / rel


def defined_syms(obj: pathlib.Path) -> set[str]:
    if not obj.exists():
        return set()
    out = subprocess.run([NM, '--defined-only', str(obj)], capture_output=True, text=True).stdout
    return {l.split()[-1] for l in out.split('\n') if len(l.split()) >= 2 and l.split()[-2] not in ('U',)}


def header_idents() -> set[str]:
    ids = set()
    files = list(pathlib.Path('include').rglob('*.h*'))
    for lib in pathlib.Path('libs').glob('*/include'):
        files += list(lib.rglob('*.h*'))
    files += list(pathlib.Path('src').rglob('*.hpp'))
    for p in files:
        if p.is_file():
            try:
                ids.update(re.findall(r'[A-Za-z_]\w*', p.read_text(errors='replace')))
            except Exception:
                pass
    return ids


def def_line(lines, name):
    """index of the line that defines `name` (declaration-safe), or None."""
    for i, l in enumerate(lines):
        if not re.search(r'\b' + re.escape(name) + r'\b', l) or l.lstrip().startswith('//'):
            continue
        if l.rstrip().endswith(';'):
            return None
        if '{' in l:
            return i
        for j in range(i + 1, min(i + 6, len(lines))):
            s = lines[j].strip()
            if s.startswith('{'):
                return i
            if ';' in s or '{' in s:
                break
    return None


def declares(lines, name):
    for l in lines:
        if re.search(r'\b' + re.escape(name) + r'\s*\([^;{]*\)\s*;', l):
            return True
    return False


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('file', help='src/<unit>.cpp')
    ap.add_argument('owner', help='splits.txt unit path, e.g. kyoshin/cf/CVision.cpp')
    ap.add_argument('--rows', default=ROWS_JSON, help='JSON from tools/coop/link_undefs.py --json')
    ap.add_argument('--dry', action='store_true')
    a = ap.parse_args()
    global ROWS
    try:
        ROWS = json.load(open(a.rows))['missing']
    except FileNotFoundError:
        sys.exit(f'ERROR: {a.rows} not found - run: python3 tools/coop/link_undefs.py --json {a.rows}')
    src, owner, dry = a.file, a.owner, a.dry
    path = pathlib.Path(src)
    obj = obj_path(src)
    if not obj.exists():
        build(obj)
    before = defined_syms(obj)
    names = {r['symbol'] for r in ROWS if r['owner'] == owner}
    lines = path.read_bytes().decode('latin-1').split('\n')
    hdr = header_idents()
    patches = []            # (line index, original line)
    for n in sorted(names):
        i = def_line(lines, n)
        if i is None:
            continue
        l = lines[i]
        if 'extern "C"' in l or (i > 0 and lines[i - 1].strip() == 'extern "C"'):
            continue
        base = n.split('__')[0] or n  # header conflicts are caught by the build guard
        # patch the definition plus any in-file declaration (linkage must agree)
        targets = [i]
        for j, lj in enumerate(lines):
            if j == i or lj.lstrip().startswith('//'):
                continue
            if re.search(r'\b' + re.escape(base) + r'\s*\([^;{]*\)\s*;', lj) and 'extern "C"' not in lj:
                targets.append(j)
        for j in targets:
            if 'extern "C"' in lines[j]:
                continue
            patches.append((j, lines[j]))
            lines[j] = re.sub(r'^(\s*)', r'\1extern "C" ', lines[j], count=1)
    if dry or not patches:
        print(f'{src}: candidates={len(patches)} (dry={dry})')
        return
    path.write_bytes('\n'.join(lines).encode('latin-1'))
    out = build(obj)
    # revert patches named in compile errors
    reverts = 0
    for _ in range(3):
        err_lines = {int(m.group(1)) for m in re.finditer(r'#\s+(\d+):', out)}
        if not err_lines:
            break
        for i, orig in patches:
            if (i + 1) in err_lines:
                lines[i] = orig
                reverts += 1
        path.write_bytes('\n'.join(lines).encode('latin-1'))
        out = build(obj)
    if re.search(r'Error', out):
        # give up safely: restore everything
        for i, orig in patches:
            lines[i] = orig
        path.write_bytes('\n'.join(lines).encode('latin-1'))
        build(obj)
        print(f'{src}: ABORTED (compile errors), restored')
        return
    after = defined_syms(obj)
    retail = {l.split(' = ')[0].strip() for l in open('config/us/symbols.txt') if ' = ' in l}
    lost = {s for s in (before - after) if s in retail}   # only retail symbols matter
    if lost:
        # revert patches whose base name appears in a lost symbol
        for i, orig in list(patches):
            if any(split_base(s) == split_base(orig) or split_base(s) in orig for s in lost):
                lines[i] = orig
                patches.remove((i, orig))
                reverts += 1
        path.write_bytes('\n'.join(lines).encode('latin-1'))
        out = build(obj)
    print(f'{src}: patched={len(patches)} reverts={reverts} kept={len(patches)} lost_before_guard={len(lost)}')


def split_base(sym):
    return sym.split('__')[0]


if __name__ == '__main__':
    main()
