import json, sys
from collections import defaultdict

data = json.load(sys.stdin)
items = data if isinstance(data, list) else next(v for v in data.values() if isinstance(v, list))
units = defaultdict(lambda: {'total': 0, 'matched': 0})
for it in items:
    u = it['unit']
    units[u]['total'] += 1
    if it.get('match_status') in ('FULL_MATCH', 'EQUIVALENT_MATCH'):
        units[u]['matched'] += 1

targets = [
    'kyoshin/cf/CtrlMoveEne',
    'monolib/src/scn/UnkClass_8047E110',
    'kyoshin/action/CActParamAnim',
    'kyoshin-leftover-1',
    'kyoshin-leftover-0',
    'criware-data-0',
    'monolib-regression-sweep',
    'kyoshin/cf/CfCamEvent_1',
    'kyoshin/CUICfManager',
    'kyoshin/cf/CtrlAct',
    'kyoshin/action/CActParamData',
    'kyoshin/cf/CfGameManager',
    'kyoshin/cf/CtrlObjectParam',
    'monolib/src/core/code_804DEDA8',
    'monolib/src/coli/code_804B59C8',
    'kyoshin/code_800B06A4',
    'kyoshin/CItemBoxGrid',
    'kyoshin/cf/CfCam',
    'kyoshin/code_80135FDC',
    'kyoshin/cf/CItem',
    'kyoshin/CEquipItemBox',
    'kyoshin/cf/CBattleManager',
    'kyoshin/makecrystal/CModelDispMakeCrystal',
    'monolib/src/scn/CScnEnvLgtCtrl',
    'kyoshin/CFloorMap',
    'kyoshin/CArtsInfo',
    'kyoshin/CItemBoxInfo',
    'monolib/src/coli/code_804A6C60',
    'monolib/src/effect/code_804CC2B8',
    'monolib-regressions',
    'rvl-data-2',
]

for t in targets:
    exact = {u: s for u, s in units.items() if u == t}
    sub = {u: s for u, s in units.items() if t in u or u.endswith('/' + t)}
    m = exact or sub
    if not m:
        print(f'{t}: NO UNIT FOUND')
    else:
        for u, s in sorted(m.items()):
            status = 'DONE' if s['matched'] == s['total'] else f"REMAINS {s['total']-s['matched']}/{s['total']} unmatched"
            print(f"{t} -> {u}: {s['matched']}/{s['total']} matched [{status}]")
