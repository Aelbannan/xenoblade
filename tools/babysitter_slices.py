import json, subprocess

data = json.loads(subprocess.run(
    [".venv/bin/python3", "tools/coop/run.py", "targets", "status", "--format", "json"],
    capture_output=True, text=True, check=True).stdout)

units = {}
for t in data["targets"]:
    u = t["unit"]
    s = units.setdefault(u, {"total": 0, "matched": 0})
    s["total"] += 1
    if t["match_status"] in ("FULL_MATCH", "EQUIVALENT_MATCH"):
        s["matched"] += 1

print("== units with 'cri' in name ==")
for u, s in sorted(units.items()):
    if "cri" in u.lower():
        flag = "" if s["matched"] < s["total"] else " (DONE)"
        print(f"{u}: {s['matched']}/{s['total']}{flag}")

print("\n== unmatched monolib units ==")
for u, s in sorted(units.items()):
    if u.startswith("monolib") and s["matched"] < s["total"]:
        print(f"{u}: {s['matched']}/{s['total']}")

print("\n== unmatched RVL_SDK/rvl data-ish units ==")
for u, s in sorted(units.items()):
    if ("rvl" in u.lower() or "RVL_SDK" in u) and s["matched"] < s["total"]:
        print(f"{u}: {s['matched']}/{s['total']}")

print("\n== kyoshin leftover candidates (code_ / UnkClass_ with no class name, unmatched) ==")
import re
for u, s in sorted(units.items()):
    if u.startswith("kyoshin") and s["matched"] < s["total"]:
        base = u.rsplit("/", 1)[-1]
        if re.match(r"^(code_|UnkClass_|func_)", base):
            print(f"{u}: {s['matched']}/{s['total']}")
