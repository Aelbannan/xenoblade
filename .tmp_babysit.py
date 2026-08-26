import json, collections, subprocess, sys

d = json.loads(subprocess.run(
    [sys.executable, 'tools/coop/run.py', 'targets', 'status', '--format', 'json'],
    capture_output=True, text=True).stdout)
items = d if isinstance(d, list) else d.get('targets', d.get('units', d.get('items', [])))
per = {}
for it in items:
    t, m = per.get(it['unit'], (0, 0))
    nm = m + (1 if it['match_status'] in ('FULL_MATCH', 'EQUIVALENT_MATCH') else 0)
    per[it['unit']] = (t + 1, nm)

def remaining(unit):
    t, m = per.get(unit, [None, None])
    if t is None:
        return 'NO_UNIT'
    return t - m

direct = {
    'eacb545': 'kyoshin/cf/CfCamEvent_1',
    'e1b7d53': 'kyoshin/CUICfManager',
    'bf51f01': 'monolib/src/core/ScheduleList',
    '971d269': 'monolib/src/scn/code_804BD8E8',
    '1fac0cb': 'monolib/src/nand/CNReqtaskSave',
    'd4cc39f': 'kyoshin/code_800B06A4',
    '3b43f85': 'kyoshin/CUIWindowManager',
    'a2b9c2f': 'kyoshin/cf/CtrlAct',
    'a6a0f12': 'RVL_SDK/src/revolution/wud/WUD',
    'a08d36c': 'kyoshin/cf/CVision',
    '496b293': 'monolib/src/scn/UnkClass_8047E110',
    '86527bb': 'kyoshin/cf/CPartsChange',
    'bd26574': 'kyoshin/action/CActParamData',
    '4435c4e': 'kyoshin/cf/CfGameManager',
    '7617e7b': 'kyoshin/cf/CtrlObjectParam',
    '0381707': 'monolib/src/core/code_804DEDA8',
    '8b8f7e5': 'monolib/src/coli/code_804B59C8',
    '6d447ce': 'kyoshin/code_800B06A4',
    '3706e33': 'kyoshin/action/CActParamAnim',
    '1ab67a2': 'kyoshin/CItemBoxGrid',
    '5843da8': 'kyoshin/cf/CfCam',
    '5de6d9d': 'kyoshin/code_80135FDC',
    '8afdd2d': 'kyoshin/cf/CItem',
    'cfee3bc': 'kyoshin/CEquipItemBox',
    '79b9ac6': 'kyoshin/cf/CBattleManager',
    '25c48db': 'kyoshin/makecrystal/CModelDispMakeCrystal',
    '519cdd4': 'monolib/src/scn/CScnEnvLgtCtrl',
    '6e53c80': 'kyoshin/CFloorMap',
    '9dec0bb': 'kyoshin/CArtsInfo',
    '48019cf': 'kyoshin/CItemBoxInfo',
    '2089ef3': 'monolib/src/coli/code_804A6C60',
    'c147ef1': 'monolib/src/effect/code_804CC2B8',
    '81d1f90': 'kyoshin/CItemBoxLine',
}
for sid, u in sorted(direct.items()):
    print(f'{sid} {u}: remaining={remaining(u)}')

# scope agents (title has no single TU): prefix unmatched counts
scope = {
    '64afe1c': 'CriWare', '63bf9ff': 'CriWare',           # criware-data-0/1
    '9930788': 'monolib', '9ffd99e': 'monolib',           # regression sweep(s)
    '85044a9': 'kyoshin', '36e2c8a': 'kyoshin',           # leftovers
    'e5ec02c': 'RVL_SDK',                                  # rvl-data-2
    '1bda43b': '*', '146d87d': '*', '51ee978': '*',
    'baff171': '*', '6eeab45': '*', 'f59adc6': '*', '57ad996': '*', '4e32624': '*',
}
pref = collections.defaultdict(lambda: [0, 0])
for u, (t, m) in per.items():
    pref[u.split('/')[0]][0] += t
    pref[u.split('/')[0]][1] += m
print('\nScope agents:')
for sid, p in scope.items():
    if p == '*':
        un = sum(t - m for t, m in pref.values())
        print(f'{sid} (all-prefixes): unmatched={un}')
    else:
        t, m = pref[p]
        print(f'{sid} ({p}): unmatched={t-m}')
