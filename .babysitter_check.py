import json
from collections import Counter

d = json.load(open('.targets_status_tmp.json'))
units = Counter()
for t in d['targets']:
    if t['match_status'] not in ('FULL_MATCH', 'EQUIVALENT_MATCH'):
        units[t['unit']] += 1

def show(label, pred):
    hits = {u: c for u, c in units.items() if pred(u)}
    print(f'{label}: {sum(hits.values())} unmatched across {len(hits)} units; sample={list(hits.items())[:5]}')

show('monolib*', lambda u: u.startswith('monolib'))
show('*criware*', lambda u: 'criware' in u.lower())
show('rvl/RVL*', lambda u: u.startswith('RVL') or u.startswith('rvl'))
