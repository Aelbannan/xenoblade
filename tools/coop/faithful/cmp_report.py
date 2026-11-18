import json, sys
def load(p):
    d = json.load(open(p)); out = {}
    for u in d['units']:
        for f in u.get('functions', []):
            out[(u['name'], f['name'])] = float(f.get('fuzzy_match_percent', 0))
        for s in u.get('sections', []):
            out[(u['name'], '§' + s['name'])] = float(s.get('fuzzy_match_percent', 0))
    return out
a, b = load(sys.argv[1]), load(sys.argv[2])
worse = [(k, a[k], b[k]) for k in a if k in b and b[k] < a[k] - 1e-6]
better = [(k, a[k], b[k]) for k in a if k in b and b[k] > a[k] + 1e-6]
gone = [k for k in a if k not in b]; new = [k for k in b if k not in a]
full = lambda d: sum(1 for k, v in d.items() if not k[1].startswith('§') and v >= 100)
print('functions 100%%: %d -> %d | worse %d better %d | gone %d new %d' % (full(a), full(b), len(worse), len(better), len(gone), len(new)))
for k, x, y in sorted(worse, key=lambda t: t[1] - t[2], reverse=True)[:80]: print('  WORSE %s %s %.2f -> %.2f' % (k[0], k[1][:70], x, y))
if '-v' in sys.argv:
    for k in gone[:20]: print('  GONE', k)
