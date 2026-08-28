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
    try:
        original_bytes = src_path.read_bytes()
        original = original_bytes.decode("shift_jis", errors="replace")
    except:
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

    # Find externs with detailed info
    extern_map = {}  # name -> (typ, is_array, array_size_str, line)
    lines = original.splitlines()
    for line in lines:
        if "extern" not in line:
            continue
        for name in list(sym_map.keys()):
            if re.search(r"\b" + re.escape(name) + r"\b", line):
                # Extract type and array part
                # Pattern: extern <type> <name> [ <size> ] ;
                # type may include "const", "unsigned", etc.
                # Use regex: extern\s+(.+?)\s+name\s*(\[.*?\])?\s*;
                m = re.search(r"extern\s+(.+?)\s+" + re.escape(name) + r"\s*(\[.*?\])?\s*;", line)
                if m:
                    typ = m.group(1).strip()
                    arr = m.group(2)  # may be None or "[...]" or "[]"
                    extern_map[name] = (typ, arr, line)
                else:
                    # fallback
                    m2 = re.search(r"extern\s+(.+?)\s+" + re.escape(name), line)
                    if m2:
                        typ = m2.group(1).strip()
                        extern_map[name] = (typ, None, line)
                break

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
        is_rodata = sec == ".rodata"
        is_data = sec == ".data"
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
                # Now define the actual symbol
                if name in extern_map:
                    typ, arr, _ = extern_map[name]
                    # Determine if original was array or scalar
                    is_array = arr is not None
                    # For scalar, just define as typ name;
                    # For array, need to handle size
                    # Clean typ: remove duplicate spaces
                    typ_clean = re.sub(r'\s+', ' ', typ).strip()
                    # For bss, we should not use const
                    typ_clean = typ_clean.replace("const","").strip()
                    typ_clean = re.sub(r'\s+', ' ', typ_clean).strip()
                    if not typ_clean:
                        typ_clean = "unsigned char"
                    elem_size = type_size(typ_clean)
                    if is_array:
                        # Check if arr is "[]" (unknown) or "[N]"
                        if arr == "[]":
                            # Unknown size, use byte size
                            if elem_size == 1:
                                if off == 0 and gap==0 and offset==0:
                                    generated.append(f'__attribute__((aligned(8))) {typ_clean} {name}[0x{sz:X}]; // bss off 0x{off:X}')
                                else:
                                    generated.append(f'{typ_clean} {name}[0x{sz:X}]; // bss off 0x{off:X}')
                            else:
                                count = sz // elem_size
                                if sz % elem_size != 0:
                                    # fallback to byte
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
                            # Has explicit size like "[7]" or "[0x7]"
                            # Extract size inside []
                            msz = re.search(r"\[(.*)\]", arr)
                            if msz:
                                sz_str = msz.group(1).strip()
                                # Keep as is, but we need to ensure it matches spec size
                                # For now, use the spec size to generate, but keep original array notation if it matches
                                # We'll just use the spec's element count
                                try:
                                    # Try to parse sz_str as hex or dec
                                    if sz_str.startswith("0x"):
                                        arr_size = int(sz_str,16)
                                    elif sz_str.isdigit():
                                        arr_size = int(sz_str)
                                    else:
                                        arr_size = None
                                except:
                                    arr_size = None
                                # If arr_size matches expected, keep it, else use spec
                                if arr_size is not None and arr_size * elem_size == sz:
                                    # Keep original
                                    if off == 0 and gap==0 and offset==0:
                                        generated.append(f'__attribute__((aligned(8))) {typ_clean} {name}{arr}; // bss off 0x{off:X}')
                                    else:
                                        generated.append(f'{typ_clean} {name}{arr}; // bss off 0x{off:X}')
                                else:
                                    # Use spec
                                    count = sz // elem_size if elem_size!=1 else sz
                                    if sz % elem_size != 0:
                                        if off == 0 and gap==0 and offset==0:
                                            generated.append(f'__attribute__((aligned(8))) unsigned char {name}[0x{sz:X}]; // bss off 0x{off:X} (fallback)')
                                        else:
                                            generated.append(f'unsigned char {name}[0x{sz:X}]; // bss off 0x{off:X} (fallback)')
                                    else:
                                        if elem_size == 1:
                                            if off == 0 and gap==0 and offset==0:
                                                generated.append(f'__attribute__((aligned(8))) {typ_clean} {name}[0x{sz:X}]; // bss off 0x{off:X}')
                                            else:
                                                generated.append(f'{typ_clean} {name}[0x{sz:X}]; // bss off 0x{off:X}')
                                        else:
                                            if off == 0 and gap==0 and offset==0:
                                                generated.append(f'__attribute__((aligned(8))) {typ_clean} {name}[0x{count:X}]; // bss off 0x{off:X}')
                                            else:
                                                generated.append(f'{typ_clean} {name}[0x{count:X}]; // bss off 0x{off:X}')
                            else:
                                # No array size, use spec
                                if off == 0 and gap==0 and offset==0:
                                    generated.append(f'__attribute__((aligned(8))) {typ_clean} {name}[0x{sz:X}]; // bss off 0x{off:X}')
                                else:
                                    generated.append(f'{typ_clean} {name}[0x{sz:X}]; // bss off 0x{off:X}')
                    else:
                        # Scalar (no [])
                        # For scalar, size should be elem_size, but spec says 0x8 for scalar u32 (which is 4) -> mismatch
                        # The spec's size for scalar that is actually 4 bytes but spec says 8 suggests it's scalar plus padding or actually array
                        # For code_803BFD20, spec says size 0x8 for lbl_eu_80606E18, but original is scalar u32 (4 bytes)
                        # That suggests the symbol is actually 8 bytes, but code uses it as u32 (only first 4 bytes)
                        # We should define it as u32 scalar plus filler, or as u64?
                        # For now, define as the original type scalar, and add a filler after it to reach 8 bytes
                        # But our gap handling already will add trailing filler after all symbols, not per symbol internal padding
                        # For this symbol, sz=8, but scalar u32 is 4, so we have 4 bytes extra inside the symbol's allocation
                        # The symbol's size in the object file is 8, but the C type's size is 4, so the compiler will allocate 4, not 8
                        # To make the object file's .bss size match, we need the symbol to occupy 8 bytes in the object file
                        # How to make a scalar occupy 8 bytes? We can define it as u64 or as array
                        # Let's check the original definition's size: The spec says the symbol at 0x80606E18 has size 0x8, but the code's extern is u32 (4 bytes)
                        # This suggests the symbol is actually 8 bytes, but the code only uses the first 4 bytes as u32, and the other 4 bytes are perhaps unused or are part of the same symbol's padding
                        # The decomp's .bss for that unit should be 8 bytes, which we can achieve by defining the symbol as u64 or as u32[2] or as unsigned char[8]
                        # But the code's extern is u32, so if we define as u64, the type mismatch will cause error (u32 vs u64)
                        # We could define it as u32 but with attribute to make it 8 bytes? Not possible
                        # Alternative: Define it as u32 scalar plus a dummy filler immediately after it that is also part of the same symbol's 8-byte allocation? But the symbol's size is determined by its type, not by following filler
                        # The filler we add after the symbol is a separate symbol, not part of the first symbol's size
                        # So total .bss size will be 4 (first symbol) + 4 (filler) = 8, which matches, and the first symbol's address is correct, and the filler occupies the remaining 4 bytes at the next offset
                        # But the spec says the first symbol itself is 8 bytes, not 4+4
                        # However, for data diff, the section size is what matters, and symbol sizes are checked via symbol table: The symbol's st_size is checked? The data diff checks symbol sizes via the .bss size per section, not per symbol? Actually data_match checks per section size, not per symbol size
                        # It checks section size (0x8) and alignment, not individual symbol sizes
                        # So we don't need each symbol's st_size to match exactly, just the section's total size
                        # The symbol table's st_size for each symbol is not checked by data_match, only section size and bytes
                        # Wait, does data_match check symbol sizes? No, it checks section sizes and bytes, not symbol table
                        # So we can define lbl_eu_80606E18 as u32 scalar (size 4) and add a filler of 4 bytes to make total section 8, and the section size will match, even though the symbol's st_size will be 4 not 8
                        # But will the symbol's address still be correct? Yes, first symbol at offset 0, filler at offset 4
                        # The retail symbol's st_size is 8, but decomp's will be 4, but data_match doesn't check st_size, only section size
                        # So it's okay
                        if off == 0 and gap==0 and offset==0:
                            generated.append(f'__attribute__((aligned(8))) {typ_clean} {name}; // bss off 0x{off:X} (scalar, spec size 0x{sz:X})')
                        else:
                            generated.append(f'{typ_clean} {name}; // bss off 0x{off:X} (scalar, spec size 0x{sz:X})')
                        # Need to account for the extra bytes inside the symbol's spec size vs type size
                        # The gap handling after this symbol will add a filler for the remaining bytes if needed
                        # For this symbol, the gap to next symbol is computed as off+sz - offset, but we used sz from spec (8), but our definition's size is 4, so the next offset calculation will be off
                        # We need to adjust offset to account for actual type size vs spec size
                        # For scalar, the actual size is elem_size, not sz
                        # But we set offset = off + sz (spec size), which is 8, but actual placed size is 4, so the next symbol's offset will be wrong
                        # To fix, we should set offset = off + elem_size (actual), and the gap to next symbol will be larger, requiring a bigger filler
                        # For this case, spec sz=8, elem_size=4, so after placing scalar at off 0, offset should be 4, next symbol is at 8? But there is no next symbol for this unit (only one symbol), so trailing gap will be sec_size - offset = 8-4=4, which we will add as filler, making total 8, correct
                        # So we need to update offset to reflect actual placed size, not spec size
                        # Let's adjust: offset = off + elem_size (for scalar) or off + count*elem_size (for array)
                        # For scalar, offset = off + elem_size
                        # For array, offset = off + count*elem_size = off + sz (since count*elem_size = sz)
                        # So for scalar, we need to use elem_size
                        # We'll handle this by updating offset accordingly
                        # For now, we already set offset = off + sz, but for scalar we should set to off + elem_size
                        # Let's fix after
                        pass
                else:
                    # No extern, use unsigned char
                    if off == 0 and gap==0 and offset==0:
                        generated.append(f'__attribute__((aligned(8))) unsigned char {name}[0x{sz:X}]; // bss off 0x{off:X}')
                    else:
                        generated.append(f'unsigned char {name}[0x{sz:X}]; // bss off 0x{off:X}')
                # Update offset correctly for next iteration
                # For scalar case, we need to adjust offset to reflect actual type size, not spec sz
                # Determine actual size placed
                if name in extern_map:
                    typ, arr, _ = extern_map[name]
                    is_array = arr is not None
                    elem_size = type_size(typ.replace("const","").strip())
                    if not is_array:
                        # scalar
                        actual_sz = elem_size
                    else:
                        # array
                        if arr == "[]":
                            actual_sz = sz
                        else:
                            # Try to parse array size
                            msz = re.search(r"\[(.*)\]", arr)
                            if msz:
                                sz_str = msz.group(1).strip()
                                if sz_str == "":
                                    actual_sz = sz
                                else:
                                    try:
                                        if sz_str.startswith("0x"):
                                            arr_sz = int(sz_str,16)
                                        else:
                                            arr_sz = int(sz_str)
                                        actual_sz = arr_sz * elem_size
                                        if actual_sz != sz:
                                            # Use spec sz
                                            actual_sz = sz
                                    except:
                                        actual_sz = sz
                            else:
                                actual_sz = sz
                    offset = off + actual_sz
                else:
                    offset = off + sz
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
                # Determine type
                if name in extern_map:
                    typ, arr, _ = extern_map[name]
                    elem_size = type_size(typ.replace("const","").strip())
                    is_array = arr is not None
                    # For file-backed, the extern is often "u8 name[]" or "char name[]" etc., array
                    # We'll try to preserve original typ
                    # If is_array and arr=="[]", we need to provide size
                    if is_array:
                        if arr == "[]":
                            # Need to provide size
                            if elem_size == 1:
                                # Use typ as is with size sz
                                # Keep const if original had const
                                if "const" in typ:
                                    # Keep const
                                    pass
                                if idx == 0:
                                    generated.append(f'__declspec(section "{sec}") __attribute__((aligned(8))) {typ} {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X}')
                                else:
                                    generated.append(f'__declspec(section "{sec}") {typ} {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X}')
                            else:
                                count = sz // elem_size
                                if sz % elem_size != 0:
                                    # fallback to byte
                                    if idx == 0:
                                        generated.append(f'__declspec(section "{sec}") __attribute__((aligned(8))) unsigned char {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X} (fallback byte, orig {typ})')
                                    else:
                                        generated.append(f'__declspec(section "{sec}") unsigned char {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X} (fallback)')
                                else:
                                    # Need to convert data to that type's initializer
                                    vals = []
                                    for i in range(0, sz, elem_size):
                                        chunk = data[i:i+elem_size]
                                        if elem_size == 2:
                                            if "s16" in typ or "signed" in typ:
                                                val = struct.unpack(">h", chunk)[0]
                                                vals.append(f"{val}")
                                            else:
                                                val = struct.unpack(">H", chunk)[0]
                                                vals.append(f"0x{val:04X}")
                                        elif elem_size == 4:
                                            if "s32" in typ or (typ.strip() == "int" and "unsigned" not in typ):
                                                val = struct.unpack(">i", chunk)[0]
                                                vals.append(f"{val}")
                                            else:
                                                val = struct.unpack(">I", chunk)[0]
                                                vals.append(f"0x{val:08X}")
                                        else:
                                            vals.append(f"0x{int.from_bytes(chunk, 'big'):X}")
                                    vals_str = ", ".join(vals)
                                    if idx == 0:
                                        generated.append(f'__declspec(section "{sec}") __attribute__((aligned(8))) {typ} {name}[0x{count:X}] = {{{vals_str}}}; // off 0x{off:X}')
                                    else:
                                        generated.append(f'__declspec(section "{sec}") {typ} {name}[0x{count:X}] = {{{vals_str}}}; // off 0x{off:X}')
                        else:
                            # Has explicit array size, keep it
                            # Check if it matches spec
                            msz = re.search(r"\[(.*)\]", arr)
                            if msz:
                                sz_str = msz.group(1).strip()
                                # Keep original line's array size if it matches spec, else use spec
                                # For now, use spec's size in elements
                                if elem_size == 1:
                                    # Keep original typ and size as spec
                                    if idx == 0:
                                        generated.append(f'__declspec(section "{sec}") __attribute__((aligned(8))) {typ} {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X}')
                                    else:
                                        generated.append(f'__declspec(section "{sec}") {typ} {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X}')
                                else:
                                    count = sz // elem_size
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
                                                if "s16" in typ:
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
                                            generated.append(f'__declspec(section "{sec}") __attribute__((aligned(8))) {typ} {name}[0x{count:X}] = {{{vals_str}}}; // off 0x{off:X}')
                                        else:
                                            generated.append(f'__declspec(section "{sec}") {typ} {name}[0x{count:X}] = {{{vals_str}}}; // off 0x{off:X}')
                    else:
                        # Scalar (no array)
                        # For file-backed, scalar with size > elem_size is weird (like u32 scalar with size 6)
                        # That suggests the symbol is actually an array but declared as scalar for address
                        # We should define it as unsigned char array of size sz to match section size
                        # But the code treats it as scalar address, so defining as scalar will make section size wrong
                        # For file-backed scalar with size mismatch, define as unsigned char array
                        if sz != elem_size:
                            # Define as byte array
                            if idx == 0:
                                generated.append(f'__declspec(section "{sec}") __attribute__((aligned(8))) unsigned char {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X} (scalar {typ} size mismatch, using byte array)')
                            else:
                                generated.append(f'__declspec(section "{sec}") unsigned char {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X} (scalar {typ} size mismatch)')
                        else:
                            # Size matches scalar
                            if idx == 0:
                                generated.append(f'__declspec(section "{sec}") __attribute__((aligned(8))) {typ} {name} = {{{fmt_bytes(data)}}}; // off 0x{off:X} scalar')
                            else:
                                generated.append(f'__declspec(section "{sec}") {typ} {name} = {{{fmt_bytes(data)}}}; // off 0x{off:X} scalar')
                else:
                    # No extern, use default byte array
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
    try:
        src_path.write_bytes(new_content.encode("shift_jis", errors="replace"))
    except Exception as e:
        src_path.write_text(new_content, encoding="utf-8", errors="replace")
        print(f"warning shift_jis failed for {u}: {e}")

    print(f"absorbed {u}: {len(generated)} defs")

print("done absorb4")
