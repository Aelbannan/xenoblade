#!/usr/bin/env python3
import re, pathlib, json, struct, sys

spec_path = pathlib.Path(".scratch/criware_data_spec.txt")
plan_path = pathlib.Path(".scratch/criware_data_plan.json")
import json as js
plan = js.loads(plan_path.read_text())

# parse spec
text = spec_path.read_text()
# split by "===" header
units_data = {}
current = None
section = None
bytes_acc = {}
symbol_bytes = {}
# We'll parse sequentially
lines = text.splitlines()
cur_unit = None
cur_sec = None
cur_symbols = []
cur_bytes_lines = []
# Use regex
unit_re = re.compile(r"^(CriWare/src/.+)$")
sec_re = re.compile(r"^\[\.(rodata|data|bss)\]\s+0x([0-9A-Fa-f]+)-0x([0-9A-Fa-f]+)\s+\((\d+) B\)")
sym_re = re.compile(r"\s*\+[0-9A-Fa-f]+\s+size:([0-9A-Fa-f]+)\s+(\S+)")
bytes_re = re.compile(r"\s*\+[0-9A-Fa-f]+:\s+((?:[0-9a-fA-F]{2}\s+)+)")
for line in lines:
    m = unit_re.match(line.strip())
    if m and "/" in line and line.startswith("CriWare"):
        cur_unit = m.group(1).strip()
        units_data[cur_unit] = {"sections": {}}
        continue
    m = sec_re.match(line)
    if m and cur_unit:
        sec_name = "."+m.group(1)
        start = m.group(2)
        end = m.group(3)
        size = int(m.group(4))
        cur_sec = sec_name
        if cur_sec not in units_data[cur_unit]["sections"]:
            units_data[cur_unit]["sections"][cur_sec] = {"start": start, "end": end, "size": size, "symbols": [], "bytes": bytearray()}
        else:
            # bss may have multiple entries? aggregate
            units_data[cur_unit]["sections"][cur_sec]["size"] += size
        continue
    m = sym_re.match(line)
    if m and cur_unit and cur_sec:
        sz = int(m.group(1),16)
        name = m.group(2)
        # store
        units_data[cur_unit]["sections"][cur_sec]["symbols"].append((name, sz))
        continue
    m = bytes_re.match(line)
    if m and cur_unit and cur_sec:
        hex_part = m.group(1).strip()
        for b in hex_part.split():
            if len(b)==2:
                try:
                    units_data[cur_unit]["sections"][cur_sec]["bytes"].append(int(b,16))
                except: pass
        continue

# Now for each assigned unit, print info and also generate C
assigned = [
    "CriWare/src/sofdec/sfdcore/mpv/mpv_deli.c",
    "CriWare/src/sofdec/sfdcore/mpv/mpv_emp.c",
    "CriWare/src/sofdec/sfdcore/mpv/mpv_hdec.c",
    "CriWare/src/sofdec/sfdcore/mpv/mpv_lib.c",
    "CriWare/src/sofdec/sfdcore/mpv/mpv_mc.c",
    "CriWare/src/sofdec/sfdcore/mpv/mpv_mcy.c",
    "CriWare/src/sofdec/sfdcore/mpv/mpv_vlc.c",
    "CriWare/src/sofdec/sfdcore/mpv/mpvabdec.c",
    "CriWare/src/sofdec/sfdcore/sfd/code_803BFD20.c",
    "CriWare/src/sofdec/sfdcore/sfd/sfd_adxt.c",
    "CriWare/src/sofdec/sfdcore/sfd/sfd_aoap.c",
    "CriWare/src/sofdec/sfdcore/sfd/sfd_buf.c",
    "CriWare/src/sofdec/sfdcore/sfd/sfd_lib.c",
    "CriWare/src/sofdec/sfdcore/sfd/sfd_mpv.c",
    "CriWare/src/sofdec/sfdcore/sfd/sfd_ply.c",
    "CriWare/src/sofdec/sfdcore/sfd/sfd_seeki.c",
]

for u in assigned:
    key = u
    if key not in units_data:
        # try without .c
        key2 = u.replace(".c","")
        if key2 in units_data:
            key = key2
        else:
            print(f"missing spec for {u}")
            continue
    data = units_data[key]
    print(f"== {u} ==")
    for sec, info in data["sections"].items():
        print(f"  {sec} size 0x{info['size']:X} symbols {info['symbols'][:3]} bytes len {len(info['bytes'])}")
        if info['bytes']:
            print("    bytes", bytes(info['bytes'][:32]).hex())
    print()

# Also check plan for bss sizes vs spec
for u in assigned:
    key = u.replace(".c","")
    # plan uses without?
    # plan key is "CriWare/src/.../foo.c"
    plan_key = u
    if plan_key in plan:
        print(f"plan {u} {plan[plan_key]}")
