import json, subprocess, sys

data = json.loads(subprocess.run(
    [".venv/bin/python3", "tools/coop/run.py", "targets", "status", "--format", "json"],
    capture_output=True, text=True, check=True).stdout)

units = {}
for t in data["targets"]:
    u = t["unit"]
    s = units.setdefault(u, {"total": 0, "fm": 0, "em": 0})
    s["total"] += 1
    if t["match_status"] == "FULL_MATCH":
        s["fm"] += 1
    elif t["match_status"] == "EQUIVALENT_MATCH":
        s["em"] += 1

agents = [
    ("64afe1c", "idle",  "criware-data-0"),
    ("bd99c6e", "idle",  "kyoshin/cf/CtrlMoveEne"),
    ("3706e33", "idle",  "kyoshin/action/CActParamAnim"),
    ("36e2c8a", "error", "kyoshin-leftover-0"),
    ("eacb545", "error", "kyoshin/cf/CfCamEvent_1"),
    ("e1b7d53", "error", "kyoshin/CUICfManager"),
    ("3b43f85", "error", "kyoshin/CUIWindowManager"),
    ("a6a0f12", "error", "RVL_SDK/src/revolution/wud/WUD"),
    ("a08d36c", "error", "kyoshin/cf/CVision"),
    ("496b293", "error", "monolib/src/scn/UnkClass_8047E110"),
    ("bd26574", "error", "kyoshin/action/CActParamData"),
    ("4435c4e", "error", "kyoshin/cf/CfGameManager"),
    ("7617e7b", "error", "kyoshin/cf/CtrlObjectParam"),
    ("0381707", "error", "monolib/src/core/code_804DEDA8"),
    ("6d447ce", "error", "kyoshin/code_800B06A4"),
    ("1ab67a2", "error", "kyoshin/CItemBoxGrid"),
    ("5843da8", "error", "kyoshin/cf/CfCam"),
    ("5de6d9d", "error", "kyoshin/code_80135FDC"),
    ("8afdd2d", "error", "kyoshin/cf/CItem"),
    ("cfee3bc", "error", "kyoshin/CEquipItemBox"),
    ("79b9ac6", "error", "kyoshin/cf/CBattleManager"),
    ("25c48db", "error", "kyoshin/makecrystal/CModelDispMakeCrystal"),
    ("519cdd4", "error", "monolib/src/scn/CScnEnvLgtCtrl"),
    ("6e53c80", "error", "kyoshin/CFloorMap"),
    ("9dec0bb", "error", "kyoshin/CArtsInfo"),
    ("48019cf", "error", "kyoshin/CItemBoxInfo"),
    ("2089ef3", "error", "monolib/src/coli/code_804A6C60"),
    ("c147ef1", "error", "monolib/src/effect/code_804CC2B8"),
    ("81d1f90", "error", "kyoshin/CItemBoxLine"),
    ("9ffd99e", "error", "monolib-regressions"),
    ("e5ec02c", "error", "rvl-data-2"),
]

exact = set(units)
for sid, status, unit in agents:
    if unit in exact:
        s = units[unit]
        rem = s["total"] - s["fm"] - s["em"]
        print(f"{sid} {status:6s} {unit}: {rem} remaining of {s['total']} (FM={s['fm']}, EM={s['em']})")
    else:
        cands = [u for u in sorted(units) if unit.split("/")[-1] == u.split("/")[-1] or u.startswith(unit)]
        print(f"{sid} {status:6s} {unit}: NOT EXACT UNIT — candidates: {cands[:10]}")
