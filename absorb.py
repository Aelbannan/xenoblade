#!/usr/bin/env python3
import re, pathlib, json, struct, sys

spec_path = pathlib.Path(".scratch/criware_data_spec.txt")
plan_path = pathlib.Path(".scratch/criware_data_plan.json")
dol_path = pathlib.Path("orig/us/sys/main.dol")

plan = json.loads(plan_path.read_text())

# Dol loader
def load_dol(path):
    raw = path.read_bytes()
    hdr = raw[:0x100]
    off = struct.unpack(">18I", hdr[0x00:0x48])
    addr = struct.unpack(">18I", hdr[0x48:0x90])
    size = struct.unpack(">18I", hdr[0x90:0xD8])
    bss_a, bss_s = struct.unpack(">II", hdr[0xD8:0xE0])
    segments = [(a, o, s) for o, a, s in zip(off, addr, size) if s]
    return {"raw": raw, "segs": segments}

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
        cur_sec = sec
        continue
    m = re.match(r"\s*\+([0-9A-Fa-f]+)\s+size:([0-9A-Fa-f]+)\s+(\S+)", line)
    if m and cur_unit is not None and cur_sec is not None:
        off = int(m.group(1),16)
        sz = int(m.group(2),16)
        name = m.group(3)
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

for u in assigned:
    src_path = pathlib.Path("libs") / u
    if not src_path.exists():
        print(f"missing {src_path}")
        continue
    original = src_path.read_text()
    # collect symbols for this unit
    spec_key = u
    if spec_key not in units:
        print(f"no spec for {u}, using plan fallback")
        # fallback: use plan to generate single array per section
        if u in plan:
            # generate
            generated = []
            for secinfo in plan[u]:
                sec = secinfo["section"]
                start = int(secinfo["start"],16)
                end = int(secinfo["end"],16)
                size = end-start
                is_bss = sec in (".bss",".sbss",".sbss2")
                if is_bss:
                    generated.append(f'__declspec(section "{sec}") __attribute__((aligned(8))) unsigned char __bss_{sec[1:]}_fallback[0x{size:X}];')
                else:
                    data = dol_read(start, size)
                    if data is None:
                        data = bytes([0]*size)
                    generated.append(f'__declspec(section "{sec}") __attribute__((aligned(8))) const unsigned char __{sec[1:]}_fallback[0x{size:X}] = {{{fmt_bytes(data)}}};')
            # remove externs that match plan? skip for now
            # append
            new_content = original.rstrip() + "\n\n// Absorbed from criware_data (fallback)\n" + "\n".join(generated) + "\n"
            src_path.write_text(new_content)
            print(f"  wrote fallback for {u}")
        continue
    info = units[spec_key]
    # Build list of sections from plan order? Use plan order if available, else spec order
    plan_secs = {}
    if u in plan:
        for secinfo in plan[u]:
            plan_secs[secinfo["section"]] = (int(secinfo["start"],16), int(secinfo["end"],16))
    # For generation, iterate over spec sections in order of appearance (which is same as plan)
    # But we need to respect plan's section list order
    # Let's use plan order if available
    sections_in_order = []
    if u in plan:
        for secinfo in plan[u]:
            sec = secinfo["section"]
            if sec in info:
                sections_in_order.append(sec)
            else:
                # plan has section but spec doesn't (maybe bss with no symbols? but spec has it)
                # For cases where spec missing a section (like mpv_deli's bss none), skip
                pass
        # also add any spec sections not in plan (should not happen)
        for sec in info:
            if sec not in sections_in_order:
                sections_in_order.append(sec)
    else:
        sections_in_order = list(info.keys())

    generated_lines = []
    # For each section, generate definitions
    for sec in sections_in_order:
        sec_info = info[sec]
        sec_start = sec_info["start"]
        sec_size = sec_info["size"]
        symbols = sorted(sec_info["symbols"])  # by off
        is_bss = sec in (".bss",".sbss",".sbss2")
        if is_bss:
            # need to handle gaps with dummy fillers
            offset = 0
            dummy_idx = 0
            for off, sz, name in symbols:
                gap = off - offset
                if gap > 0:
                    # dummy filler for gap
                    # Use unique name per gap
                    filler_name = f"__bss_fill_{sec[1:]}_{dummy_idx}_for_{u.replace('/','_').replace('.','_')}"
                    # To avoid overly long, just use __bss_fill_N
                    filler_name = f"__bss_fill_{dummy_idx}"
                    # Ensure we use a unique per section to avoid duplicate across units? Since each file is separate TU, name collision across TUs is okay (they are separate objects), but within one TU, filler names must be unique.
                    generated_lines.append(f'__declspec(section "{sec}") unsigned char {filler_name}[0x{gap:X}]; // filler for gap at 0x{offset:X}')
                    dummy_idx += 1
                # now symbol
                if off == 0 and dummy_idx == 0:
                    # first object in section, need aligned(8)
                    generated_lines.append(f'__declspec(section "{sec}") __attribute__((aligned(8))) unsigned char {name}[0x{sz:X}]; // bss off 0x{off:X}')
                else:
                    generated_lines.append(f'__declspec(section "{sec}") unsigned char {name}[0x{sz:X}]; // bss off 0x{off:X}')
                offset = off + sz
            # trailing gap
            gap = sec_size - offset
            if gap > 0:
                filler_name = f"__bss_fill_{dummy_idx}"
                generated_lines.append(f'__declspec(section "{sec}") unsigned char {filler_name}[0x{gap:X}]; // trailing filler')
        else:
            # file-backed
            for idx, (off, sz, name) in enumerate(symbols):
                abs_addr = sec_start + off
                data = dol_read(abs_addr, sz)
                if data is None:
                    print(f"  WARNING no DOL data for {name} at 0x{abs_addr:08X} sz 0x{sz:X}")
                    data = bytes([0]*sz)
                else:
                    if len(data) != sz:
                        print(f"  size mismatch for {name}: expected {sz} got {len(data)}")
                # Choose const for rodata, non-const for data, but both with declspec
                is_rodata = sec == ".rodata"
                # Use const for rodata, unsigned char for data (non-const)
                # Need to handle gap symbols that start with "gap_" -> they are also file-backed but we still define them
                if is_rodata:
                    if idx == 0:
                        generated_lines.append(f'__declspec(section "{sec}") __attribute__((aligned(8))) const unsigned char {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X}')
                    else:
                        generated_lines.append(f'__declspec(section "{sec}") const unsigned char {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X}')
                else: # .data
                    if idx == 0:
                        generated_lines.append(f'__declspec(section "{sec}") __attribute__((aligned(8))) unsigned char {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X}')
                    else:
                        generated_lines.append(f'__declspec(section "{sec}") unsigned char {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X}')
            # No extra dummy needed for file-backed because gap symbols already cover gaps

    # Now need to remove existing extern declarations for these symbols from original file
    # Collect all symbol names for this unit
    all_names = []
    for sec, sec_info in info.items():
        for off, sz, name in sec_info["symbols"]:
            all_names.append(name)
    # Also need to handle gap names? They are not referenced as extern, so not needed to remove, but we will define them anyway
    # For bss dummy fillers, we don't need to remove
    # Remove lines that contain "extern" and any of these names
    lines = original.splitlines()
    new_lines = []
    for line in lines:
        # Check if line contains extern and any symbol name
        if "extern" in line:
            # Check if any name appears in line
            found = False
            for n in all_names:
                if n in line:
                    found = True
                    break
            if found:
                # Skip this line (remove)
                # Also need to handle multi-line extern? Assume single line
                continue
        new_lines.append(line)
    new_content = "\n".join(new_lines).rstrip() + "\n\n// Absorbed from criware_data (retail bytes)\n" + "\n".join(generated_lines) + "\n"
    src_path.write_text(new_content)
    print(f"absorbed {u}: {len(generated_lines)} definitions")

print("done")
