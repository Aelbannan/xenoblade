#!/usr/bin/env python3
import re, pathlib, json, struct

spec_path = pathlib.Path(".scratch/criware_data_spec.txt")
plan_path = pathlib.Path(".scratch/criware_data_plan.json")
import json as js
plan = js.loads(plan_path.read_text())

# Read spec into structured dict
text = spec_path.read_text().splitlines()
units = {}
cur_unit = None
cur_sec = None
# Use state machine
for idx, line in enumerate(text):
    # Detect unit header: line is exactly "CriWare/src/..." (no brackets)
    if line.startswith("CriWare/src/") and line.strip().endswith(".c"):
        cur_unit = line.strip()
        units[cur_unit] = {}
        cur_sec = None
        continue
    # Section header: [.rodata] 0x...-0x... (size B)
    m = re.match(r"\s*\[\.(rodata|data|bss)\]\s+0x([0-9A-Fa-f]+)-0x([0-9A-Fa-f]+)\s+\(([0-9A-Fa-f]+)\s+B\)", line)
    if m and cur_unit is not None:
        sec = "."+m.group(1)
        start = int(m.group(2),16)
        end = int(m.group(3),16)
        size = int(m.group(4),16)
        # init
        if sec not in units[cur_unit]:
            units[cur_unit][sec] = {"start": start, "end": end, "size": size, "symbols": [], "bytes": bytearray(), "symbol_offsets": []}
        else:
            # Sometimes spec has duplicate sec headers per unit? Should not happen, but if it does, merge?
            # For now, create new entry with suffix
            # Actually for some units, there are multiple sections of same type? No, only one per type per unit.
            pass
        cur_sec = sec
        continue
    # Symbol layout: +0000 size:0058  lbl_eu_8051C200
    m = re.match(r"\s*\+([0-9A-Fa-f]+)\s+size:([0-9A-Fa-f]+)\s+(\S+)", line)
    if m and cur_unit is not None and cur_sec is not None:
        off = int(m.group(1),16)
        sz = int(m.group(2),16)
        name = m.group(3)
        # store symbol with offset
        # find the sec dict
        # cur_sec may be None if we are in a bss with no bytes header? But we have cur_sec set
        if cur_sec in units[cur_unit]:
            units[cur_unit][cur_sec]["symbols"].append((off, sz, name))
        continue
    # Bytes: +0000: 0a 43 52 49 ...
    m = re.match(r"\s*\+([0-9A-Fa-f]+):\s+((?:[0-9a-fA-F]{2}\s+)+)", line)
    if m and cur_unit is not None and cur_sec is not None:
        hex_part = m.group(2).strip()
        for b in hex_part.split():
            if len(b)==2 and all(c in "0123456789abcdefABCDEF" for c in b):
                if cur_sec in units[cur_unit]:
                    units[cur_unit][cur_sec]["bytes"].append(int(b,16))
        continue

# For each assigned unit, generate C
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

def fmt_bytes(arr):
    # format as "0x0A,0x43, ..."
    return ",".join(f"0x{b:02X}" for b in arr)

# Helper to get plan section size for validation
for u in assigned:
    base = u  # with .c
    # plan key may be with .c
    plan_key = base
    if plan_key not in plan:
        plan_key = base.replace(".c","")
    # units key is without .c in spec
    spec_key = base.replace(".c","")
    if spec_key not in units:
        spec_key = base
    print(f"=== {u} ===")
    if spec_key in units:
        for sec, info in units[spec_key].items():
            print(f"  spec {sec} size 0x{info['size']:X} symbols {len(info['symbols'])} bytes {len(info['bytes'])}")
            # also check plan
            if plan_key in plan:
                for p in plan[plan_key]:
                    if p['section']==sec:
                        print(f"    plan {sec} 0x{p['start']}-0x{p['end']} size 0x{int(p['end'],16)-int(p['start'],16):X}")
            # show first few symbols
            for off, sz, name in info['symbols'][:5]:
                print(f"    {name} off 0x{off:X} size 0x{sz:X}")
            if len(info['symbols'])>5:
                print(f"    ... and {len(info['symbols'])-5} more")
    else:
        print("  no spec")
        if plan_key in plan:
            print(f"  plan {plan[plan_key]}")
    print()

# Now generate C for each
# We'll write to libs/CriWare/src/.../*.c by appending definitions
# For simplicity, generate a separate file per unit with definitions and then we will manually merge?

# Let's just print what we would generate for first few to validate
for u in assigned[:2]:
    spec_key = u.replace(".c","")
    if spec_key not in units:
        continue
    print(f"--- GENERATING {u} ---")
    for sec, info in units[spec_key].items():
        is_bss = sec in (".bss", ".sbss", ".sbss2")
        bytes_arr = info["bytes"]
        symbols = sorted(info["symbols"])  # by off
        # Validation: total bytes should equal sec size for file-backed, but for bss bytes len is 0
        print(f"sec {sec} size 0x{info['size']:X} symbols {symbols}")
        if not is_bss:
            # For file-backed, we can generate one big array per section or per symbol?
            # Let's generate per symbol with bytes sliced from bytes_arr
            # need to know each symbol's byte slice: bytes are sequential per section, not per symbol offset? Actually spec's bytes are for whole section sequential
            # So symbol at off 0x58 corresponds to bytes[0x58:0x58+sz]
            for off, sz, name in symbols:
                # skip gap symbols? they are also listed as symbols, but they are filler
                # For file-backed, gap symbols are also part of bytes, so include
                slice_bytes = bytes_arr[off:off+sz]
                print(f"  {name} [{sz}] bytes {slice_bytes[:8].hex() if slice_bytes else ''} ... total slice {len(slice_bytes)}")
        else:
            for off, sz, name in symbols:
                print(f"  {name} bss off 0x{off:X} size 0x{sz:X}")
    print()

