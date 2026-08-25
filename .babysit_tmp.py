import json, subprocess

ls = json.loads(subprocess.run(
    ["/Applications/Paseo.app/Contents/Resources/bin/paseo", "ls", "--json"],
    capture_output=True, text=True).stdout)

ts = json.loads(subprocess.run(
    [".venv/bin/python3", "tools/coop/run.py", "targets", "status", "--format", "json"],
    capture_output=True, text=True).stdout)

# Aggregate per-unit stats
units = {}
items = ts if isinstance(ts, list) else ts.get("targets") or ts.get("units") or []
if isinstance(items, dict):
    # maybe nested
    for v in items.values():
        items = v; break
for it in items:
    u = it.get("unit")
    st = it.get("match_status")
    d = units.setdefault(u, {"total": 0, "matched": 0})
    d["total"] += 1
    if st in ("FULL_MATCH", "EQUIVALENT_MATCH"):
        d["matched"] += 1

agents = [a for a in ls if a.get("name","").startswith("decomp: ")]

def find_unit(name):
    tail = name[len("decomp: "):].strip()
    # exact match first
    if tail in units:
        return tail
    # suffix match on path components
    cands = [u for u in units if u == tail or u.endswith("/" + tail) or tail.endswith(u)]
    if len(cands) >= 1:
        return cands[0]
    return None

print("decomp agents found:", len(agents))
to_nudge = []
for a in sorted(agents, key=lambda x: x["name"]):
    name = a["name"]; status = a["status"]; sid = a["shortId"]
    unit = find_unit(name)
    if status == "running":
        print(f"RUNNING  {sid} {name}")
        continue
    if unit is None:
        print(f"{status.upper():8s} {sid} {name}  -> NO UNIT FOUND IN TARGETS (skipping)")
        continue
    d = units[unit]
    remaining = d["total"] - d["matched"]
    tag = "DONE" if remaining == 0 else f"REMAIN={remaining}/{d['total']}"
    print(f"{status.upper():8s} {sid} {name}  [{unit}] {tag}")
    if status in ("error", "idle") and remaining > 0:
        to_nudge.append((sid, name))

for sid, name in to_nudge:
    subprocess.run(["/Applications/Paseo.app/Contents/Resources/bin/paseo",
                    "send", "--no-wait", sid, "continue"],
                   capture_output=True, text=True)
    print(f"NUDGED   {sid} {name}")

r = sum(1 for _ in to_nudge)
run = sum(1 for a in agents if a["status"]=="running")
done = 0
for a in agents:
    if a["status"]=="running": continue
    u = find_unit(a["name"])
    if u and units[u]["total"] - units[u]["matched"] == 0: done += 1
print(f"\nSUMMARY: nudged={r} running={run} done={done} total_decomp={len(agents)}")
