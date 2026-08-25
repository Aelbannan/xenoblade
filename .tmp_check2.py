import json
data=json.load(open('tools/coop/targets.json'))
txt=json.dumps(data)
for k in ('leftover','criware-data','rvl-data','regression'):
    print(k, k in txt)
# maybe targets.json is a list of units
if isinstance(data, dict):
    print(list(data.keys())[:20])
