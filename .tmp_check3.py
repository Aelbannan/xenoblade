import json
data=json.load(open('.tmp_targets_status.json'))
from collections import defaultdict
stats=defaultdict(lambda:{'total':0,'m':0})
for t in data['targets']:
    s=stats[t['unit']]; s['total']+=1
    if t['match_status'] in ('FULL_MATCH','EQUIVALENT_MATCH'): s['m']+=1

# criware-data-0 / rvl-data-2: likely data-unit batches. Find units with zero functions matched? Print unmatched-only CriWare/RVL units
for u in sorted(stats):
    lu=u.lower()
    if ('criware' in lu or 'rvl_sdk' in lu) and stats[u]['total']>0:
        pass
# print units where all targets are kind != function? check kinds
kinds=defaultdict(set)
for t in data['targets']:
    kinds[t['unit']].add(t['kind'])
# show sample of 'regression'-ish matches
import re
print('units containing "monolib" without slash:', [u for u in stats if u.startswith('monolib') and '/' not in u])
