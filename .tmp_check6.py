import json
from collections import defaultdict
data=json.load(open('.tmp_targets_status.json'))
stats=defaultdict(lambda:{'total':0,'m':0})
for t in data['targets']:
    s=stats[t['unit']]; s['total']+=1
    if t['match_status'] in ('FULL_MATCH','EQUIVALENT_MATCH'): s['m']+=1

batches=json.load(open('.tmp_batches.json'))
for name,units in batches.items():
    tot=mt=0; missing=[]
    for u in units:
        u2=u[len('main/'):] if u.startswith('main/') else u
        # try exact then basename-ish fallbacks
        cands=[u2, u2.replace('main/','')]
        s=None; used=None
        for c in cands:
            if c in stats: s=stats[c]; used=c; break
        if s is None:
            missing.append(u2)
        else:
            tot+=s['total']; mt+=s['m']
    verdict = 'DONE' if (tot>0 and mt==tot and not missing) else ('REMAINS %d/%d unmatched'%(tot-mt,tot))
    print(f'{name}: {mt}/{tot} matched -> {verdict}' + (f'  MISSING:{missing}' if missing else ''))
