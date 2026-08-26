import json

d = json.load(open('.targets_status_tmp.json'))
t = d['targets']
units = {}
for e in t:
    u = e.get('unit') or '(none)'
    units.setdefault(u, {'total': 0, 'matched': 0})
    units[u]['total'] += 1
    if e.get('match_status') in ('FULL_MATCH', 'EQUIVALENT_MATCH'):
        units[u]['matched'] += 1

agent_units = ['kyoshin/cf/CfCamEvent_1', 'kyoshin/cf/CtrlMoveEne', 'ScheduleList',
               'code_800B06A4', 'code_800BD8E8', 'kyoshin/CUICfManager', 'kyoshin/CUIWindowManager',
               'kyoshin/cf/CtrlAct', 'RVL_SDK/src/revolution/wud/WUD', 'kyoshin/cf/CVision',
               'monolib/src/scn/UnkClass_8047E110', 'kyoshin/cf/CPartsChange',
               'kyoshin/action/CActParamData', 'kyoshin/cf/CfGameManager',
               'kyoshin/cf/CtrlObjectParam', 'monolib/src/core/code_804DEDA8',
               'monolib/src/coli/code_804B59C8', 'kyoshin/code_800B06A4',
               'kyoshin/action/CActParamAnim', 'kyoshin/CItemBoxGrid', 'kyoshin/cf/CfCam',
               'kyoshin/code_80135FDC', 'kyoshin/cf/CItem', 'kyoshin/CEquipItemBox',
               'kyoshin/cf/CBattleManager', 'kyoshin/makecrystal/CModelDispMakeCrystal',
               'monolib/src/scn/CScnEnvLgtCtrl', 'kyoshin/CFloorMap', 'kyoshin/CArtsInfo',
               'kyoshin/CItemBoxInfo', 'monolib/src/coli/code_804A6C60',
               'monolib/src/effect/code_804CC2B8', 'kyoshin/CItemBoxLine', 'rvl-data-2']

total_unmatched_all = sum(v['total'] - v['matched'] for v in units.values())
print('TOTAL unmatched across project:', total_unmatched_all)
print()
for u in agent_units:
    cands = [k for k in units if k == u or k.endswith('/' + u) or u.endswith(k)]
    tot = sum(units[c]['total'] for c in cands)
    m = sum(units[c]['matched'] for c in cands)
    print(f'{u!r}: cands={cands} total={tot} matched={m} unmatched={tot-m}')
