#!/usr/bin/env python3
import re, pathlib, json, struct

spec_path = pathlib.Path(".scratch/criware_data_spec.txt")
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

# Parse spec
text = spec_path.read_text(encoding="utf-8", errors="replace").splitlines()
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
    if not src_path.exists():
        print(f"missing {src_path}")
        continue
    original = src_path.read_text(encoding="utf-8", errors="replace")
    spec_key = u
    if spec_key not in units:
        print(f"no spec for {u}")
        continue
    info = units[spec_key]
    # Build asm block
    asm_lines = []
    asm_lines.append('__asm(')
    # For each section in plan order
    sections_in_order = []
    if u in plan:
        for secinfo in plan[u]:
            sec = secinfo["section"]
            if sec in info and sec not in sections_in_order:
                sections_in_order.append(sec)
        for sec in info:
            if sec not in sections_in_order:
                sections_in_order.append(sec)
    else:
        sections_in_order = list(info.keys())

    for sec in sections_in_order:
        sec_info = info[sec]
        sec_start = sec_info["start"]
        sec_size = sec_info["size"]
        symbols = sorted(sec_info["symbols"])
        is_bss = sec in (".bss",".sbss",".sbss2")
        asm_lines.append(f'".section {sec}\\n"')
        asm_lines.append(f'".balign 8\\n"')
        if is_bss:
            # For bss, emit as .space
            # Use a base label for the section start
            base_label = f"__bss_base_{sec[1:]}_{u.replace('/', '_').replace('.', '_')}"
            # Simplify: use a local label
            asm_lines.append(f'"{base_label}:\\n"')
            asm_lines.append(f'".space 0x{sec_size:X}\\n"')
            for off, sz, name in symbols:
                asm_lines.append(f'".global {name}\\n"')
                asm_lines.append(f'"{name} = {base_label} + 0x{off:X}\\n"')
        else:
            # file-backed: emit whole section bytes as one blob under first symbol or base
            data = dol_read(sec_start, sec_size)
            if data is None:
                print(f"no dol data for {u} {sec}")
                data = bytes([0]*sec_size)
            # Find first symbol at offset 0 if any
            first_at_0 = None
            for off, sz, name in symbols:
                if off == 0:
                    first_at_0 = name
                    break
            if first_at_0:
                base = first_at_0
                # Emit base symbol and bytes
                asm_lines.append(f'".global {base}\\n"')
                asm_lines.append(f'"{base}:\\n"')
                # Emit bytes
                # Use .byte for all, 16 per line
                for i in range(0, len(data), 16):
                    chunk = data[i:i+16]
                    asm_lines.append(f'".byte {", ".join(f"0x{b:02X}" for b in chunk)}\\n"')
                # Define other symbols as aliases
                for off, sz, name in symbols:
                    if name == base:
                        continue
                    asm_lines.append(f'".global {name}\\n"')
                    asm_lines.append(f'"{name} = {base} + 0x{off:X}\\n"')
            else:
                # No symbol at offset 0, need a base label
                base_label = f"__rodata_base_{sec[1:]}_{u.replace('/', '_').replace('.', '_')}"
                base_label = base_label.replace("-", "_")[:30]
                asm_lines.append(f'"{base_label}:\\n"')
                for i in range(0, len(data), 16):
                    chunk = data[i:i+16]
                    asm_lines.append(f'".byte {", ".join(f"0x{b:02X}" for b in chunk)}\\n"')
                for off, sz, name in symbols:
                    asm_lines.append(f'".global {name}\\n"')
                    asm_lines.append(f'"{name} = {base_label} + 0x{off:X}\\n"')
        # End section
    asm_lines.append(');')
    asm_block = "\n".join(asm_lines)

    # Remove any previous generated __asm blocks for this file that we added earlier?
    # Our previous files had no __asm, they had __declspec, so we need to remove those __declspec lines for these symbols
    # Instead, we will just remove all lines that contain "__declspec(section" for these symbols and also any __bss_fill etc.
    # Simpler: Find the header we added previously ("// Absorbed from criware_data") and remove everything after it, then append the new asm block
    # Check if file already has that header
    if "// Absorbed from criware_data" in original:
        # Truncate at that header
        idx = original.index("// Absorbed from criware_data")
        original = original[:idx].rstrip() + "\n"
    # Also need to remove any extern lines for these symbols? The __asm block will provide the definitions, but the C code still has extern declarations for those symbols (from the header or from the file's own externs)
    # The extern declarations are okay to keep, as they are just declarations, not definitions, and the __asm will provide the definitions
    # But we previously removed externs for these symbols in the last run, so they are already gone. Now we have no externs for them, but the code still references them as extern via the header's LBLS_ENTRY externs
    # That's fine, the __asm will provide the definitions
    # Append the asm block after the includes (or at end)
    # Find insertion point after last #include
    lines = original.splitlines()
    insert_idx = 0
    for i, line in enumerate(lines):
        if line.strip().startswith("#include"):
            insert_idx = i + 1
    # Insert after includes
    header = "\n// Absorbed via __asm (retail bytes)\n"
    block = header + asm_block + "\n"
    lines.insert(insert_idx, block)
    new_content = "\n".join(lines) + "\n"
    src_path.write_text(new_content, encoding="utf-8", errors="replace")
    print(f"fixed {u} with __asm")

print("done fix_remaining")
