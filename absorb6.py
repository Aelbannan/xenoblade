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

def type_size(t):
    t2 = re.sub(r'\b(const|volatile|static|extern|__declspec\(.*?\)|__attribute__\(.*?\))\b', '', t).strip()
    t2 = re.sub(r'\s+', ' ', t2).strip()
    if "*" in t2:
        return 4
    if t2 in ("u8", "s8", "char", "unsigned char", "signed char", "uint8_t", "int8_t", "bool", "_Bool", "u8", "s8"):
        return 1
    if t2 in ("u16", "s16", "unsigned short", "short", "uint16_t", "int16_t"):
        return 2
    if t2 in ("u32", "s32", "unsigned int", "int", "unsigned long", "long", "uint32_t", "int32_t", "float", "f32", "size_t", "s32", "u32"):
        return 4
    if t2 in ("u64", "s64", "unsigned long long", "long long", "uint64_t", "int64_t", "double", "f64"):
        return 8
    return 1

for u in assigned:
    src_path = pathlib.Path("libs") / u
    if not src_path.exists():
        print(f"missing {src_path}")
        continue
    # Read as utf-8 (original is utf-8)
    original = src_path.read_text(encoding="utf-8", errors="replace")
    spec_key = u
    if spec_key not in units:
        print(f"no spec for {u}")
        continue
    info = units[spec_key]
    sym_map = {}
    for sec, sec_info in info.items():
        for off, sz, name in sec_info["symbols"]:
            sym_map[name] = (sec, sz, off, sec_info["start"])

    # Find externs
    extern_map = {}  # name -> (full_decl, typ, is_array, array_str, line)
    lines = original.splitlines()
    for line in lines:
        if "extern" not in line:
            continue
        for name in list(sym_map.keys()):
            if re.search(r"\b" + re.escape(name) + r"\b", line):
                # Extract the part between "extern" and ";"
                # Find name position
                try:
                    idx_name = line.index(name)
                    before = line[line.index("extern")+6:idx_name]
                    after = line[idx_name+len(name):line.index(";", idx_name) if ";" in line[idx_name:] else len(line)]
                except:
                    before = ""
                    after = ""
                # before contains type with possible * and spaces
                typ = before.strip()
                # Remove any trailing * that belongs to declarator? Keep it as part of typ
                # after contains array part like "[0x7]" or "[]"
                arr = None
                m_arr = re.search(r"\[.*?\]", after)
                if m_arr:
                    arr = m_arr.group(0)
                else:
                    # Check if after contains "[]"
                    if "[]" in after:
                        arr = "[]"
                # If typ ends with "*", it's part of type
                # For cases like "void *lbl", before is "void *", which is correct
                # For "u8 lbl[]", before is "u8", arr is "[]"
                extern_map[name] = (typ, arr, line)
                break

    # Build generated definitions
    # Determine sections in order
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

    generated = []
    for sec in sections_in_order:
        sec_info = info[sec]
        sec_start = sec_info["start"]
        sec_size = sec_info["size"]
        symbols = sorted(sec_info["symbols"])
        is_bss = sec in (".bss",".sbss",".sbss2")
        if is_bss:
            offset = 0
            dummy_idx = 0
            for off, sz, name in symbols:
                gap = off - offset
                if gap > 0:
                    filler_name = f"__bss_fill_{dummy_idx}"
                    while filler_name in sym_map:
                        dummy_idx += 1
                        filler_name = f"__bss_fill_{dummy_idx}"
                    if offset == 0:
                        generated.append(f'__attribute__((aligned(8))) unsigned char {filler_name}[0x{gap:X}]; // filler gap at 0x{offset:X} for {sec}')
                    else:
                        generated.append(f'unsigned char {filler_name}[0x{gap:X}]; // filler gap at 0x{offset:X} for {sec}')
                    dummy_idx += 1
                if name in extern_map:
                    typ, arr, _ = extern_map[name]
                    typ_clean = typ.strip()
                    # Remove duplicate spaces
                    typ_clean = re.sub(r'\s+', ' ', typ_clean).strip()
                    # For bss, remove const
                    typ_clean = typ_clean.replace("const","").strip()
                    typ_clean = re.sub(r'\s+', ' ', typ_clean).strip()
                    if not typ_clean:
                        typ_clean = "unsigned char"
                    elem_size = type_size(typ_clean)
                    is_array = arr is not None
                    if is_array:
                        if arr == "[]":
                            # Unknown size, use spec size
                            if elem_size == 1:
                                if off == 0 and gap==0 and offset==0:
                                    generated.append(f'__attribute__((aligned(8))) {typ_clean} {name}[0x{sz:X}]; // bss off 0x{off:X}')
                                else:
                                    generated.append(f'{typ_clean} {name}[0x{sz:X}]; // bss off 0x{off:X}')
                            else:
                                count = sz // elem_size if elem_size else sz
                                if sz % elem_size != 0:
                                    if off == 0 and gap==0 and offset==0:
                                        generated.append(f'__attribute__((aligned(8))) unsigned char {name}[0x{sz:X}]; // bss off 0x{off:X} (fallback byte, orig {typ} size mismatch)')
                                    else:
                                        generated.append(f'unsigned char {name}[0x{sz:X}]; // bss off 0x{off:X} (fallback byte, orig {typ})')
                                else:
                                    if off == 0 and gap==0 and offset==0:
                                        generated.append(f'__attribute__((aligned(8))) {typ_clean} {name}[0x{count:X}]; // bss off 0x{off:X}')
                                    else:
                                        generated.append(f'{typ_clean} {name}[0x{count:X}]; // bss off 0x{off:X}')
                        else:
                            # Has explicit size
                            # Keep as is if it matches spec, else use spec
                            # For now, just use the original line's array size if it matches, otherwise use spec
                            # Parse array size
                            msz = re.search(r"\[(.*)\]", arr)
                            if msz:
                                sz_str = msz.group(1).strip()
                                # Keep original
                                if off == 0 and gap==0 and offset==0:
                                    generated.append(f'__attribute__((aligned(8))) {typ_clean} {name}{arr}; // bss off 0x{off:X}')
                                else:
                                    generated.append(f'{typ_clean} {name}{arr}; // bss off 0x{off:X}')
                            else:
                                if off == 0 and gap==0 and offset==0:
                                    generated.append(f'__attribute__((aligned(8))) {typ_clean} {name}[0x{sz:X}]; // bss off 0x{off:X}')
                                else:
                                    generated.append(f'{typ_clean} {name}[0x{sz:X}]; // bss off 0x{off:X}')
                    else:
                        # Scalar
                        # For scalar, the spec size may be larger than elem_size (like u32 scalar with spec size 8)
                        # That indicates the symbol is actually 8 bytes but declared as scalar 4 bytes
                        # We'll define as scalar and add filler for the remaining bytes later via gap handling
                        # But our gap handling already accounts for spec size vs actual placed size
                        # For now, define as scalar
                        if off == 0 and gap==0 and offset==0:
                            generated.append(f'__attribute__((aligned(8))) {typ_clean} {name}; // bss off 0x{off:X} (scalar, spec 0x{sz:X})')
                        else:
                            generated.append(f'{typ_clean} {name}; // bss off 0x{off:X} (scalar, spec 0x{sz:X})')
                        # Adjust offset to reflect actual placed size, not spec size
                        # Actual size is elem_size
                        # So for next iteration, offset should be off + elem_size, not off + sz
                        # But we already set offset = off + sz at the end, which would be wrong for scalar with spec size > elem_size
                        # We need to handle this
                        # For scalar, actual_sz = elem_size
                        actual_sz = elem_size
                        # Update offset to off + actual_sz, and the remaining (sz - actual_sz) will be accounted as gap to next symbol
                        # But our next gap calculation uses offset, so we need to set offset correctly
                        # Let's do it here
                        offset = off + actual_sz
                        continue
                else:
                    # No extern, use unsigned char
                    if off == 0 and gap==0 and offset==0:
                        generated.append(f'__attribute__((aligned(8))) unsigned char {name}[0x{sz:X}]; // bss off 0x{off:X}')
                    else:
                        generated.append(f'unsigned char {name}[0x{sz:X}]; // bss off 0x{off:X}')
                # For non-scalar array case, offset is off+sz, for scalar we already handled
                if name in extern_map:
                    typ, arr, _ = extern_map[name]
                    is_array = arr is not None
                    if not is_array:
                        # Already handled offset for scalar above, skip
                        # But we already continued, so this is not reached
                        pass
                    else:
                        offset = off + sz
                else:
                    offset = off + sz
            # Need to handle offset correctly for the loop
            # The above loop already updates offset per iteration, but we need to handle the scalar case where we continued
            # For simplicity, recompute offset after loop for trailing gap
            # Let's recompute offset as max of all symbols' end, considering actual placed sizes
            # For now, just compute trailing gap based on last offset
            # The offset variable is already at last symbol's end (with actual sizes)
            # For trailing gap, use sec_size - offset
            gap = sec_size - offset
            if gap > 0:
                filler_name = f"__bss_fill_{dummy_idx}"
                generated.append(f'unsigned char {filler_name}[0x{gap:X}]; // trailing filler for {sec}')
        else:
            # file-backed
            for idx, (off, sz, name) in enumerate(symbols):
                abs_addr = sec_start + off
                data = dol_read(abs_addr, sz)
                if data is None:
                    print(f"WARNING no DOL data for {name} at 0x{abs_addr:08X} sz 0x{sz:X} in {u}")
                    data = bytes([0]*sz)
                if name in extern_map:
                    typ, arr, _ = extern_map[name]
                    typ_clean = typ.strip()
                    typ_clean = re.sub(r'\s+', ' ', typ_clean).strip()
                    elem_size = type_size(typ_clean.replace("const","").strip())
                    is_array = arr is not None
                    # For file-backed, if is_array is False (scalar), but sz is 6, it's mismatch, use byte array
                    if not is_array:
                        # Scalar with file-backed size > elem_size, use byte array
                        if sz != elem_size:
                            # Use byte array
                            if "const" not in typ_clean and sec == ".rodata":
                                # For rodata, original may be non-const but data is rodata, use const?
                                # Keep original typ's constness? For now, use unsigned char
                                if idx == 0:
                                    generated.append(f'__declspec(section "{sec}") __attribute__((aligned(8))) unsigned char {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X} (scalar {typ} size mismatch, byte array)')
                                else:
                                    generated.append(f'__declspec(section "{sec}") unsigned char {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X} (scalar {typ} size mismatch)')
                            else:
                                if idx == 0:
                                    generated.append(f'__declspec(section "{sec}") __attribute__((aligned(8))) {typ_clean} {name} = {{{fmt_bytes(data)}}}; // off 0x{off:X} scalar')
                                else:
                                    generated.append(f'__declspec(section "{sec}") {typ_clean} {name} = {{{fmt_bytes(data)}}}; // off 0x{off:X} scalar')
                        else:
                            # Size matches scalar
                            if idx == 0:
                                generated.append(f'__declspec(section "{sec}") __attribute__((aligned(8))) {typ_clean} {name} = {{{fmt_bytes(data)}}}; // off 0x{off:X} scalar')
                            else:
                                generated.append(f'__declspec(section "{sec}") {typ_clean} {name} = {{{fmt_bytes(data)}}}; // off 0x{off:X} scalar')
                    else:
                        # Array
                        if arr == "[]":
                            # Unknown size
                            if elem_size == 1:
                                if idx == 0:
                                    generated.append(f'__declspec(section "{sec}") __attribute__((aligned(8))) {typ_clean} {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X}')
                                else:
                                    generated.append(f'__declspec(section "{sec}") {typ_clean} {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X}')
                            else:
                                count = sz // elem_size if elem_size else sz
                                if sz % elem_size != 0:
                                    if idx == 0:
                                        generated.append(f'__declspec(section "{sec}") __attribute__((aligned(8))) unsigned char {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X} (fallback byte, orig {typ})')
                                    else:
                                        generated.append(f'__declspec(section "{sec}") unsigned char {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X} (fallback)')
                                else:
                                    vals = []
                                    for i in range(0, sz, elem_size):
                                        chunk = data[i:i+elem_size]
                                        if elem_size == 2:
                                            if "s16" in typ_clean or "signed" in typ_clean:
                                                val = struct.unpack(">h", chunk)[0]
                                                vals.append(f"{val}")
                                            else:
                                                val = struct.unpack(">H", chunk)[0]
                                                vals.append(f"0x{val:04X}")
                                        elif elem_size == 4:
                                            if "s32" in typ_clean or ("int" in typ_clean and "unsigned" not in typ_clean):
                                                val = struct.unpack(">i", chunk)[0]
                                                vals.append(f"{val}")
                                            else:
                                                val = struct.unpack(">I", chunk)[0]
                                                vals.append(f"0x{val:08X}")
                                        else:
                                            vals.append(f"0x{int.from_bytes(chunk, 'big'):X}")
                                    vals_str = ", ".join(vals)
                                    if idx == 0:
                                        generated.append(f'__declspec(section "{sec}") __attribute__((aligned(8))) {typ_clean} {name}[0x{count:X}] = {{{vals_str}}}; // off 0x{off:X}')
                                    else:
                                        generated.append(f'__declspec(section "{sec}") {typ_clean} {name}[0x{count:X}] = {{{vals_str}}}; // off 0x{off:X}')
                        else:
                            # Explicit array size
                            # Keep original arr
                            # Need to check if it matches spec
                            # For now, just use spec size in elements
                            elem_size = type_size(typ_clean.replace("const","").strip())
                            if elem_size == 1:
                                if idx == 0:
                                    generated.append(f'__declspec(section "{sec}") __attribute__((aligned(8))) {typ_clean} {name}{arr} = {{{fmt_bytes(data)}}}; // off 0x{off:X}')
                                else:
                                    generated.append(f'__declspec(section "{sec}") {typ_clean} {name}{arr} = {{{fmt_bytes(data)}}}; // off 0x{off:X}')
                            else:
                                count = sz // elem_size if elem_size else sz
                                if sz % elem_size != 0:
                                    if idx == 0:
                                        generated.append(f'__declspec(section "{sec}") __attribute__((aligned(8))) unsigned char {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X} (fallback)')
                                    else:
                                        generated.append(f'__declspec(section "{sec}") unsigned char {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X} (fallback)')
                                else:
                                    vals = []
                                    for i in range(0, sz, elem_size):
                                        chunk = data[i:i+elem_size]
                                        if elem_size == 2:
                                            if "s16" in typ_clean:
                                                val = struct.unpack(">h", chunk)[0]
                                                vals.append(f"{val}")
                                            else:
                                                val = struct.unpack(">H", chunk)[0]
                                                vals.append(f"0x{val:04X}")
                                        elif elem_size == 4:
                                            val = struct.unpack(">I", chunk)[0]
                                            vals.append(f"0x{val:08X}")
                                        else:
                                            vals.append(f"0x{int.from_bytes(chunk, 'big'):X}")
                                    vals_str = ", ".join(vals)
                                    if idx == 0:
                                        generated.append(f'__declspec(section "{sec}") __attribute__((aligned(8))) {typ_clean} {name}{arr} = {{{vals_str}}}; // off 0x{off:X}')
                                    else:
                                        generated.append(f'__declspec(section "{sec}") {typ_clean} {name}{arr} = {{{vals_str}}}; // off 0x{off:X}')
                else:
                    # No extern, use default
                    if sec == ".rodata":
                        if idx == 0:
                            generated.append(f'__declspec(section "{sec}") __attribute__((aligned(8))) const unsigned char {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X}')
                        else:
                            generated.append(f'__declspec(section "{sec}") const unsigned char {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X}')
                    else:
                        if idx == 0:
                            generated.append(f'__declspec(section "{sec}") __attribute__((aligned(8))) unsigned char {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X}')
                        else:
                            generated.append(f'__declspec(section "{sec}") unsigned char {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X}')

    # Now process original file: remove extern lines for these symbols
    lines = original.splitlines()
    new_lines = []
    for line in lines:
        if "extern" in line:
            found = False
            for name in list(sym_map.keys()):
                if re.search(r"\b" + re.escape(name) + r"\b", line):
                    found = True
                    break
            if found:
                continue
        new_lines.append(line)

    # Find insertion point after last #include
    insert_idx = 0
    for i, line in enumerate(new_lines):
        if line.strip().startswith("#include"):
            insert_idx = i + 1
    header = "\n// Absorbed from criware_data (retail bytes) - generated\n"
    block = "\n".join(generated)
    new_lines.insert(insert_idx, header + block)

    new_content = "\n".join(new_lines) + "\n"
    src_path.write_text(new_content, encoding="utf-8", errors="replace")
    print(f"absorbed {u}: {len(generated)} defs")

print("done absorb6")
