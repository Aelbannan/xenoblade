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
    spec_key = u
    if spec_key not in units:
        print(f"no spec for {u}")
        continue
    info = units[spec_key]
    # Collect all symbol names for this unit
    all_names = {}
    for sec, sec_info in info.items():
        for off, sz, name in sec_info["symbols"]:
            all_names[name] = (sec, sz, off)

    # For replacement, we need to handle extern lines
    # We'll process lines, replacing extern declarations for these names
    lines = original.splitlines()
    new_lines = []
    # Keep track of which names we've replaced extern for
    replaced = set()
    for line in lines:
        stripped = line.strip()
        # Check if this line declares an extern for any of our symbols
        # Pattern: extern ... name ...;
        if "extern" in line:
            found_name = None
            for name in all_names:
                # Use word boundary
                if re.search(r"\b" + re.escape(name) + r"\b", line):
                    found_name = name
                    break
            if found_name:
                sec, sz, off = all_names[found_name]
                # Determine if this is rodata (const) or not
                is_rodata = sec == ".rodata"
                # For bss/data, use unsigned char, for rodata use const unsigned char
                # Use same for all to keep simple: rodata const, others non-const
                if is_rodata:
                    new_line = f"extern const unsigned char {found_name}[0x{sz:X}]; // was: {line.strip()}"
                else:
                    # For .data and .bss, non-const
                    new_line = f"extern unsigned char {found_name}[0x{sz:X}]; // was: {line.strip()}"
                new_lines.append(new_line)
                replaced.add(found_name)
                continue
        new_lines.append(line)

    # For symbols that were not found as extern (like jumptables, gap symbols, or bss fillers not declared), we don't need forward extern, but we should ensure they are defined before use if they are used?
    # For those, the code may reference them without an extern (maybe via extern in another header). But if they are used in this TU and we didn't have an extern, the reference will be undefined after we remove extern? Actually if the symbol is used in this TU but there was no extern line for it, it must be declared elsewhere or via header. Let's check: For mpv_mc, jumptable is not declared as extern in the file, but it's referenced via `jumptable_eu_80567298` as a local? Actually in mpv_mc.c, the jumptable is referenced as `jumptable_eu_80567298` via `lis r4, jumptable_eu_80567298@ha` - that's a local jumptable defined in .data. The C code has `extern`? Let's see: It doesn't have extern for jumptable; the C code just has the jumptable as a local? In mpv_mc.c, the jumptable is not referenced as a C variable; it's used via inline assembly switch. So no need for extern.

    # Now generate definitions for all symbols (including those we replaced)
    generated = []
    # For each section in plan order (to ensure correct section order)
    # Use plan order if available
    sections_in_order = []
    if u in plan:
        for secinfo in plan[u]:
            sec = secinfo["section"]
            if sec in info:
                if sec not in sections_in_order:
                    sections_in_order.append(sec)
        # Add any remaining spec sections not in plan
        for sec in info:
            if sec not in sections_in_order:
                sections_in_order.append(sec)
    else:
        sections_in_order = list(info.keys())

    for sec in sections_in_order:
        sec_info = info[sec]
        sec_start = sec_info["start"]
        sec_size = sec_info["size"]
        symbols = sorted(sec_info["symbols"])  # by off
        is_bss = sec in (".bss",".sbss",".sbss2")
        if is_bss:
            offset = 0
            dummy_idx = 0
            for off, sz, name in symbols:
                gap = off - offset
                if gap > 0:
                    filler_name = f"__bss_fill_{dummy_idx}"
                    generated.append(f'__declspec(section "{sec}") unsigned char {filler_name}[0x{gap:X}]; // filler gap at 0x{offset:X} for {u}')
                    dummy_idx += 1
                # define symbol
                if off == 0 and dummy_idx == 0:
                    generated.append(f'__declspec(section "{sec}") __attribute__((aligned(8))) unsigned char {name}[0x{sz:X}]; // bss off 0x{off:X}')
                else:
                    generated.append(f'__declspec(section "{sec}") unsigned char {name}[0x{sz:X}]; // bss off 0x{off:X}')
                offset = off + sz
                # Mark as already handled for extern replacement? The extern was already replaced above with extern declaration, now we provide definition
            gap = sec_size - offset
            if gap > 0:
                filler_name = f"__bss_fill_{dummy_idx}"
                generated.append(f'__declspec(section "{sec}") unsigned char {filler_name}[0x{gap:X}]; // trailing filler')
        else:
            # file-backed
            # For file-backed, we need to ensure we handle gap symbols already as part of symbols, so no extra dummy needed
            for idx, (off, sz, name) in enumerate(symbols):
                abs_addr = sec_start + off
                data = dol_read(abs_addr, sz)
                if data is None:
                    print(f"WARNING no DOL data for {name} at 0x{abs_addr:08X} sz 0x{sz:X} in {u}")
                    data = bytes([0]*sz)
                is_rodata = sec == ".rodata"
                # For gap symbols that start with "gap_", they are filler with zeros, but we still define them
                # Use const for rodata, non-const for data
                if is_rodata:
                    if idx == 0:
                        generated.append(f'__declspec(section "{sec}") __attribute__((aligned(8))) const unsigned char {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X}')
                    else:
                        generated.append(f'__declspec(section "{sec}") const unsigned char {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X}')
                else:
                    if idx == 0:
                        generated.append(f'__declspec(section "{sec}") __attribute__((aligned(8))) unsigned char {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X}')
                    else:
                        generated.append(f'__declspec(section "{sec}") unsigned char {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X}')

    # Now, for symbols that were not in spec but are in plan (should not happen), we already handled via sections_in_order covering all spec secs, but if plan has a section not in spec, we need to handle it via fallback
    # Check for plan sections not in spec
    if u in plan:
        for secinfo in plan[u]:
            sec = secinfo["section"]
            if sec not in info:
                # No spec for this sec, generate fallback single array for whole section
                start = int(secinfo["start"],16)
                end = int(secinfo["end"],16)
                size = end-start
                is_bss = sec in (".bss",".sbss",".sbss2")
                if is_bss:
                    generated.append(f'__declspec(section "{sec}") __attribute__((aligned(8))) unsigned char __bss_{sec[1:]}_fallback[0x{size:X}]; // fallback for missing spec')
                else:
                    data = dol_read(start, size)
                    if data is None:
                        data = bytes([0]*size)
                    generated.append(f'__declspec(section "{sec}") __attribute__((aligned(8))) const unsigned char __{sec[1:]}_fallback[0x{size:X}] = {{{fmt_bytes(data)}}}; // fallback')

    # Now construct new file content: new_lines (with replaced externs) + generated definitions
    new_content = "\n".join(new_lines).rstrip() + "\n\n// Absorbed from criware_data (retail bytes) - generated\n" + "\n".join(generated) + "\n"
    src_path.write_text(new_content)
    print(f"absorbed {u}: {len(generated)} defs, replaced {len(replaced)} externs")

print("done absorb2")
