#!/usr/bin/env python3
import pathlib, json, struct, re

plan_path = pathlib.Path(".scratch/criware_data_plan.json")
dol_path = pathlib.Path("orig/us/sys/main.dol")
plan = json.loads(plan_path.read_text())

def load_dol(path):
    raw = path.read_bytes()
    hdr = raw[:0x100]
    off = struct.unpack(">18I", hdr[0x00:0x48])
    addr = struct.unpack(">18I", hdr[0x48:0x90])
    size = struct.unpack(">18I", hdr[0x90:0xD8])
    segments = [(a, o, s) for o, a, s in zip(off, addr, size) if s]
    return {"raw": raw, "segs": segments}

dol = load_dol(dol_path)
def dol_read(vaddr, size):
    raw = dol["raw"]
    for a, o, s in dol["segs"]:
        if a <= vaddr and vaddr+size <= a+s:
            return raw[o + (vaddr - a) : o + (vaddr - a) + size]
    return None

def fmt_bytes(arr):
    return ", ".join(f"0x{b:02X}" for b in arr)

failing = [
    "CriWare/src/sofdec/sfdcore/mpv/mpv_hdec.c",
    "CriWare/src/sofdec/sfdcore/mpv/mpv_lib.c",
    "CriWare/src/sofdec/sfdcore/mpv/mpv_mc.c",
    "CriWare/src/sofdec/sfdcore/mpv/mpv_mcy.c",
    "CriWare/src/sofdec/sfdcore/mpv/mpvabdec.c",
    "CriWare/src/sofdec/sfdcore/sfd/sfd_adxt.c",
    "CriWare/src/sofdec/sfdcore/sfd/sfd_aoap.c",
    "CriWare/src/sofdec/sfdcore/sfd/sfd_lib.c",
    "CriWare/src/sofdec/sfdcore/sfd/sfd_ply.c",
]

for u in failing:
    src_path = pathlib.Path("libs") / u
    original = src_path.read_text(encoding="utf-8", errors="replace")
    # Find insertion point after last #include
    lines = original.splitlines()
    insert_idx = 0
    for i, line in enumerate(lines):
        if line.strip().startswith("#include"):
            insert_idx = i + 1
    generated = []
    # For each section in plan for this unit
    if u in plan:
        for secinfo in plan[u]:
            sec = secinfo["section"]
            start = int(secinfo["start"],16)
            end = int(secinfo["end"],16)
            size = end-start
            is_bss = sec in (".bss",".sbss",".sbss2")
            if is_bss:
                # For bss, define a large array in that section
                # Use unsigned char with aligned(8)
                # Use a unique name based on section and unit
                name = f"__large_{sec[1:]}_for_{u.replace('/', '_').replace('.', '_')}"
                # Ensure name is not too long and is valid
                name = re.sub(r'[^A-Za-z0-9_]', '_', name)[:50]
                generated.append(f'__attribute__((aligned(8))) unsigned char {name}[0x{size:X}]; // {sec} 0x{size:X} at 0x{start:08X} large bss')
                # Need to force it into the correct section: use __declspec
                # Actually for bss, we need __declspec(section ".bss")
                generated[-1] = f'__declspec(section "{sec}") __attribute__((aligned(8))) unsigned char {name}[0x{size:X}]; // {sec} 0x{size:X} at 0x{start:08X} large bss'
            else:
                data = dol_read(start, size)
                if data is None:
                    print(f"no dol data for {u} {sec} at 0x{start:08X}")
                    data = bytes([0]*size)
                # For file-backed, use const unsigned char with section
                name = f"__large_{sec[1:]}_for_{u.replace('/', '_').replace('.', '_')}"
                name = re.sub(r'[^A-Za-z0-9_]', '_', name)[:50]
                # Use const for rodata, non-const for data, but both with declspec will go to correct section
                # Use const for rodata to ensure it's in rodata, but declspec will force anyway
                # Use unsigned char for data, const for rodata
                if sec == ".rodata":
                    generated.append(f'__declspec(section "{sec}") __attribute__((aligned(8))) const unsigned char {name}[0x{size:X}] = {{{fmt_bytes(data)}}}; // {sec} 0x{size:X} at 0x{start:08X}')
                elif sec == ".data":
                    generated.append(f'__declspec(section "{sec}") __attribute__((aligned(8))) unsigned char {name}[0x{size:X}] = {{{fmt_bytes(data)}}}; // {sec} 0x{size:X} at 0x{start:08X}')
                else:
                    generated.append(f'__declspec(section "{sec}") __attribute__((aligned(8))) const unsigned char {name}[0x{size:X}] = {{{fmt_bytes(data)}}}; // {sec} 0x{size:X} at 0x{start:08X}')
    header = "\n// Large section arrays for data diff - generated\n"
    block = "\n".join(generated)
    lines.insert(insert_idx, header + block)
    new_content = "\n".join(lines) + "\n"
    src_path.write_text(new_content, encoding="utf-8", errors="replace")
    print(f"fixed {u} with {len(generated)} large arrays")

print("done fix_remaining2")
