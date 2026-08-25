import json
data=json.load(open('.tmp_targets_status.json'))
units=sorted({t['unit'] for t in data['targets']})
for u in units:
    lu=u.lower()
    if any(k in lu for k in ('leftover','criware','regress','rvl')):
        print('UNIT:', u)
