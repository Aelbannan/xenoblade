#!/usr/bin/env python3
import re, pathlib, json, struct

spec_path = pathlib.Path(".scratch/criware_data_spec.txt")
plan_path = pathlib.Path(".scratch/criware_data_plan.json")
import json as js
plan = js.loads(plan_path.read_text())
dol_path = pathlib.Path("orig/us/sys/main.dol")
# Load DolImage
import importlib.util, sys
# Use the DolImage from gen_data_defs
spec = pathlib.Path("tools/port/gen_data_defs.py")
# Instead of importing, just implement minimal Dol reader here
def load_dol(path):
    raw = path.read_bytes()
    hdr = raw[:0x100]
    off = struct.unpack(">18I", hdr[0x00:0x48])
    addr = struct.unpack(">18I", hdr[0x48:0x90])
    size = struct.unpack(">18I", hdr[0x90:0xD8])
    bss_a, bss_s = struct.unpack(">II", hdr[0xD8:0xE0])
    segments = [(a, o, s) for o, a, s in zip(off, addr, size) if s]
    return {"raw": raw, "segs": segments, "bss_a": bss_a, "bss_s": bss_s}

dol = load_dol(dol_path)
def dol_read(vaddr, size):
    raw = dol["raw"]
    for a, o, s in dol["segs"]:
        if a <= vaddr and vaddr+size <= a+s:
            return raw[o + (vaddr - a) : o + (vaddr - a) + size]
    return None

# Parse spec
text = spec_path.read_text().splitlines()
units = {}
cur_unit = None
cur_sec = None
for line in text:
    # unit header
    if line.startswith("CriWare/src/") and line.strip().endswith(".c"):
        cur_unit = line.strip()
        if cur_unit not in units:
            units[cur_unit] = {}
        cur_sec = None
        continue
    m = re.match(r"\s*\[\.(rodata|data|bss)\]\s+0x([0-9A-Fa-f]+)-0x([0-9A-Fa-f]+)\s+\(([0-9A-Fa-f]+)\s+B\)", line)
    if m and cur_unit is not None:
        sec = "."+m.group(1)
        start = int(m.group(2),16)
        end = int(m.group(3),16)
        size = int(m.group(4),16)
        if sec not in units[cur_unit]:
            units[cur_unit][sec] = {"start": start, "end": end, "size": size, "symbols": []}
        else:
            # Should not happen, but if it does, keep first
            pass
        cur_sec = sec
        continue
    m = re.match(r"\s*\+([0-9A-Fa-f]+)\s+size:([0-9A-Fa-f]+)\s+(\S+)", line)
    if m and cur_unit is not None and cur_sec is not None:
        off = int(m.group(1),16)
        sz = int(m.group(2),16)
        name = m.group(3)
        # cur_sec may be None if we had duplicate sec names? Use last sec
        if cur_sec in units[cur_unit]:
            units[cur_unit][cur_sec]["symbols"].append((off, sz, name))
        continue

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
    return ", ".join(f"0x{b:02X}" for b in arr)

# Generate for each
for u in assigned:
    spec_key = u
    # plan key is with .c
    plan_key = u
    print(f"=== {u} ===")
    # plan sections
    plan_secs = {}
    if plan_key in plan:
        for secinfo in plan[plan_key]:
            sec = secinfo["section"]
            start = int(secinfo["start"],16)
            end = int(secinfo["end"],16)
            plan_secs[sec] = (start, end, end-start)
            print(f"  plan {sec} 0x{start:08X}-0x{end:08X} size 0x{end-start:X}")
    # spec sections
    if spec_key in units:
        for sec, info in units[spec_key].items():
            print(f"  spec {sec} start 0x{info['start']:08X} size 0x{info['size']:X} symbols {len(info['symbols'])}")
            for off, sz, name in info["symbols"][:5]:
                print(f"    {name} off 0x{off:X} sz 0x{sz:X}")
            if len(info["symbols"])>5:
                print(f"    ... {len(info['symbols'])-5} more")
            # also check plan vs spec size match
            if sec in plan_secs:
                psize = plan_secs[sec][2]
                if psize != info["size"]:
                    print(f"    MISMATCH plan size 0x{psize:X} vs spec 0x{info['size']:X}")
    else:
        print("  no spec")
    # Now generate C snippet for this unit
    # We will collect per section
    print("--- C generation preview ---")
    if spec_key in units:
        for sec, info in units[spec_key].items():
            is_bss = sec in (".bss", ".sbss", ".sbss2")
            symbols = sorted(info["symbols"])  # by off
            sec_start = info["start"]
            sec_size = info["size"]
            # For bss, we need to handle gaps
            if is_bss:
                # generate with gaps
                offset = 0
                dummy_idx = 0
                for off, sz, name in symbols:
                    gap = off - offset
                    if gap > 0:
                        # dummy filler
                        print(f"  // BSS gap {gap} bytes at 0x{offset:X}")
                        print(f'  __declspec(section "{sec}") unsigned char __bss_fill_{dummy_idx}[0x{gap:X}]; // filler')
                        dummy_idx += 1
                    # now symbol
                    # first symbol in section should be aligned(8)
                    is_first = (offset == 0 and gap==0) or (dummy_idx==0 and off==0)
                    # Actually first real symbol after leading gap is at off, but section start is 0, so if off>0, first filler was at 0, so real symbol not first
                    # Determine if this is the first object in section (offset 0)
                    if off == 0:
                        print(f'  __declspec(section "{sec}") __attribute__((aligned(8))) unsigned char {name}[0x{sz:X}]; // bss off 0x{off:X}')
                    else:
                        print(f'  __declspec(section "{sec}") unsigned char {name}[0x{sz:X}]; // bss off 0x{off:X}')
                    offset = off + sz
                # trailing gap
                gap = sec_size - offset
                if gap > 0:
                    print(f"  // BSS trailing gap {gap} at 0x{offset:X}")
                    print(f'  __declspec(section "{sec}") unsigned char __bss_fill_{dummy_idx}[0x{gap:X}];')
            else:
                # file-backed
                for idx, (off, sz, name) in enumerate(symbols):
                    abs_addr = sec_start + off
                    data = dol_read(abs_addr, sz)
                    if data is None:
                        print(f"  // WARNING: no DOL data for {name} at 0x{abs_addr:08X} sz 0x{sz:X}")
                        data = bytes([0]*sz)
                    else:
                        # verify size
                        if len(data) != sz:
                            print(f"  // size mismatch for {name}")
                    # generate
                    # Use const unsigned char for .rodata, unsigned char for .data? Use const for rodata, non-const for data?
                    # For .rodata, const; for .data, non-const? But we can use const for both with section attribute, it will still go to correct section due to declspec
                    # Use `const unsigned char` for .rodata, `unsigned char` for .data
                    is_rodata = sec == ".rodata"
                    # Determine if first in section
                    if idx == 0:
                        # first gets aligned(8)
                        if is_rodata:
                            print(f'  __declspec(section "{sec}") __attribute__((aligned(8))) const unsigned char {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X}')
                        else:
                            # .data
                            # For .data, need non-const? Use unsigned char
                            # If data contains relocs, we still emit raw bytes; use const to force .rodata? No, declspec forces section, so const vs non-const doesn't matter for section, but we should match original file-backed's writability? .data is writable, .rodata is not. So for .data, use non-const? But with declspec(section ".data"), it will go to .data regardless of const.
                            # Use unsigned char for .data
                            print(f'  __declspec(section "{sec}") __attribute__((aligned(8))) unsigned char {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X}')
                    else:
                        if is_rodata:
                            print(f'  __declspec(section "{sec}") const unsigned char {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X}')
                        else:
                            print(f'  __declspec(section "{sec}") unsigned char {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X}')
                # Note: for file-backed, gaps are already covered by gap symbols, so no extra dummy needed
    else:
        # No spec, use plan only: generate single array per section
        if plan_key in plan:
            for secinfo in plan[plan_key]:
                sec = secinfo["section"]
                start = int(secinfo["start"],16)
                end = int(secinfo["end"],16)
                size = end-start
                is_bss = sec in (".bss",".sbss",".sbss2")
                if is_bss:
                    print(f'  __declspec(section "{sec}") __attribute__((aligned(8))) unsigned char __bss_{sec[1:]}[0x{size:X}]; // fallback')
                else:
                    data = dol_read(start, size)
                    if data is None:
                        data = bytes([0]*size)
                    print(f'  __declspec(section "{sec}") __attribute__((aligned(8))) const unsigned char __{sec[1:]}_fallback[0x{size:X}] = {{{fmt_bytes(data[:32])}...}};')
    print()
