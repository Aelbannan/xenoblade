import contextlib, fcntl, json, os, re, sys
# usage: rename_syms.py old=new ...   (applies by name to us/eu/jp symbols.txt and targets.json names)
#        rename_syms.py --flags NAME@0xADDR 'scope:local data:string_table'
#            (appends missing flags to the symbols.txt line(s) of NAME at ADDR)
# All read-modify-writes of config/*/symbols.txt and targets.json happen under
# one repo-wide flock so concurrent agents never clobber each other's edits.

LOCK_PATH = 'build/.symbols_edit.lock'


@contextlib.contextmanager
def symbols_lock():
    os.makedirs(os.path.dirname(LOCK_PATH), exist_ok=True)
    with open(LOCK_PATH, 'w') as fh:
        fcntl.flock(fh, fcntl.LOCK_EX)
        try:
            yield
        finally:
            fcntl.flock(fh, fcntl.LOCK_UN)


def _write_atomic(path, text):
    tmp = path + '.rename_tmp'
    with open(tmp, 'w') as f:
        f.write(text)
    os.replace(tmp, path)


def rename_targets(data, pairs):
    """Rename exact "symbol"/"name" values in a parsed targets document; returns count."""
    mapping = dict(pairs)
    n = 0

    def walk(node):
        nonlocal n
        if isinstance(node, dict):
            for key in ('symbol', 'name'):
                v = node.get(key)
                if isinstance(v, str) and v in mapping:
                    node[key] = mapping[v]
                    n += 1
            for v in node.values():
                walk(v)
        elif isinstance(node, list):
            for v in node:
                walk(v)

    walk(data)
    return n


def set_flags(spec, flags):
    name, addr = spec.rsplit('@', 1)
    addr = int(addr, 16)
    want = flags.split()
    for reg in ('us', 'eu', 'jp'):
        p = f'config/{reg}/symbols.txt'
        lines = open(p).read().split('\n')
        n = 0
        for i, line in enumerate(lines):
            m = re.match(r'^(\S+) = \.\w+:0x([0-9A-Fa-f]+);(.*)$', line)
            if not m or m.group(1) != name or int(m.group(2), 16) != addr:
                continue
            have = m.group(3).split()
            extra = [w for w in want if w not in have]
            if extra:
                lines[i] = line.rstrip() + ' ' + ' '.join(extra)
            n += 1
        _write_atomic(p, '\n'.join(lines))
        print(reg, n, 'line(s)')


def main(argv):
    with symbols_lock():
        if argv and argv[0] == '--flags':
            set_flags(argv[1], argv[2])
            return
        pairs = [a.split('=', 1) for a in argv]
        for reg in ('us', 'eu', 'jp'):
            p = f'config/{reg}/symbols.txt'; s = open(p).read(); n = 0
            for o, nw in pairs:
                pat = re.compile(r'^' + re.escape(o) + r' = ', re.M)
                if pat.search(s):
                    s = pat.sub(lambda m: nw + ' = ', s, count=1); n += 1
            _write_atomic(p, s); print(reg, n, 'of', len(pairs))
        p = 'tools/coop/targets.json'
        data = json.load(open(p))
        n = rename_targets(data, pairs)
        # json.dump escapes backslashes (e.g. __sinit_\CTagProcessor_cpp) correctly.
        _write_atomic(p, json.dumps(data, indent=2, ensure_ascii=False) + '\n')
        print('targets', n)


if __name__ == '__main__':
    main(sys.argv[1:])
