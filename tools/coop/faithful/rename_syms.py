import json, os, re, sys
# usage: rename_syms.py old=new ...   (applies by name to us/eu/jp symbols.txt and targets.json names)


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


def main(argv):
    pairs = [a.split('=', 1) for a in argv]
    for reg in ('us', 'eu', 'jp'):
        p = f'config/{reg}/symbols.txt'; s = open(p).read(); n = 0
        for o, nw in pairs:
            pat = re.compile(r'^' + re.escape(o) + r' = ', re.M)
            if pat.search(s):
                s = pat.sub(lambda m: nw + ' = ', s, count=1); n += 1
        open(p, 'w').write(s); print(reg, n, 'of', len(pairs))
    p = 'tools/coop/targets.json'
    data = json.load(open(p))
    n = rename_targets(data, pairs)
    tmp = p + '.rename_tmp'
    # json.dump escapes backslashes (e.g. __sinit_\CTagProcessor_cpp) correctly.
    with open(tmp, 'w') as f:
        f.write(json.dumps(data, indent=2, ensure_ascii=False) + '\n')
    os.replace(tmp, p)
    print('targets', n)


if __name__ == '__main__':
    main(sys.argv[1:])
