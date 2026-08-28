#!/usr/bin/env python3
import re, pathlib, json, struct, sys

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
    # Remove qualifiers
    t2 = re.sub(r'\b(const|volatile|static|extern|__declspec\(.*?\)|__attribute__\(.*?\))\b', '', t).strip()
    t2 = re.sub(r'\s+', ' ', t2).strip()
    # Handle pointers? For now, assume base type
    # If t contains "*", size is pointer size 4
    if "*" in t2:
        return 4
    # Map
    if t2 in ("u8", "s8", "char", "unsigned char", "signed char", "uint8_t", "int8_t", "bool", "_Bool"):
        return 1
    if t2 in ("u16", "s16", "unsigned short", "short", "uint16_t", "int16_t", "wchar_t", "char16_t"):
        return 2
    if t2 in ("u32", "s32", "unsigned int", "int", "unsigned long", "long", "uint32_t", "int32_t", "float", "f32", "size_t"):
        return 4
    if t2 in ("u64", "s64", "unsigned long long", "long long", "uint64_t", "int64_t", "double", "f64"):
        return 8
    return 1

for u in assigned:
    src_path = pathlib.Path("libs") / u
    if not src_path.exists():
        print(f"missing {src_path}")
        continue
    # Read with shift_jis to preserve encoding
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
    # Build sym_map
    sym_map = {}
    for sec, sec_info in info.items():
        for off, sz, name in sec_info["symbols"]:
            sym_map[name] = (sec, sz, off, sec_info["start"])

    # Find extern declarations
    extern_map = {}
    lines = original.splitlines()
    for line in lines:
        if "extern" not in line:
            continue
        for name in list(sym_map.keys()):
            if re.search(r"\b" + re.escape(name) + r"\b", line):
                # Extract type between "extern" and name
                # Use regex to capture type
                # Pattern: extern <type> <name>
                # type may contain spaces, *, const, etc.
                # We need to handle cases like "extern u8 lbl_eu_..." or "extern const unsigned char lbl_eu_..."
                # Also "extern u32 lbl_eu_8051C2A8[4];" - we want type "u32" without array part
                # So capture up to name
                # Use: extern\s+(.+?)\s+name\b
                m = re.search(r"extern\s+(.+?)\s+" + re.escape(name) + r"\b", line)
                if m:
                    typ = m.group(1).strip()
                    # Remove any trailing * or spaces, keep as is
                    # Also need to handle array brackets after name? The type is before name, not after
                    extern_map[name] = (typ, line)
                else:
                    extern_map[name] = ("unsigned char", line)
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
    # For each section, generate definitions in offset order
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
                    # Ensure unique
                    while filler_name in sym_map:
                        dummy_idx += 1
                        filler_name = f"__bss_fill_{dummy_idx}"
                    if offset == 0:
                        generated.append(f'__attribute__((aligned(8))) unsigned char {filler_name}[0x{gap:X}]; // filler gap at 0x{offset:X} for {sec}')
                    else:
                        generated.append(f'unsigned char {filler_name}[0x{gap:X}]; // filler gap at 0x{offset:X} for {sec}')
                    dummy_idx += 1
                # Now define the actual symbol
                # Use original type if available
                if name in extern_map:
                    typ, _ = extern_map[name]
                    # For bss, the extern may be like "u32" or "u16" etc., need to handle array size
                    # The spec size is in bytes, but if typ is not byte, we need element count
                    elem_size = type_size(typ)
                    # Remove any existing array brackets from typ? The typ we captured is just the type part, not including brackets
                    # For extern like "u32 lbl_eu_..." with "[]", typ is "u32", which is fine
                    # For "const unsigned char", typ is "const unsigned char"
                    # Determine if we need to include "const" for bss? bss is writable, so not const
                    # Keep typ as is, but ensure it's not const for bss
                    typ_clean = typ.replace("const","").strip()
                    typ_clean = re.sub(r'\s+', ' ', typ_clean).strip()
                    if not typ_clean:
                        typ_clean = "unsigned char"
                    if elem_size == 1:
                        # Use byte array
                        if off == 0 and offset == 0 and gap==0:
                            generated.append(f'__attribute__((aligned(8))) {typ_clean} {name}[0x{sz:X}]; // bss off 0x{off:X}')
                        else:
                            generated.append(f'{typ_clean} {name}[0x{sz:X}]; // bss off 0x{off:X}')
                    else:
                        count = sz // elem_size
                        if sz % elem_size != 0:
                            # fallback to byte
                            if off == 0 and offset == 0 and gap==0:
                                generated.append(f'__attribute__((aligned(8))) unsigned char {name}[0x{sz:X}]; // bss off 0x{off:X} (fallback byte, orig typ {typ})')
                            else:
                                generated.append(f'unsigned char {name}[0x{sz:X}]; // bss off 0x{off:X} (fallback byte, orig typ {typ})')
                        else:
                            if off == 0 and offset == 0 and gap==0:
                                generated.append(f'__attribute__((aligned(8))) {typ_clean} {name}[0x{count:X}]; // bss off 0x{off:X}')
                            else:
                                generated.append(f'{typ_clean} {name}[0x{count:X}]; // bss off 0x{off:X}')
                else:
                    # No extern, use unsigned char
                    if off == 0 and offset == 0 and gap==0:
                        generated.append(f'__attribute__((aligned(8))) unsigned char {name}[0x{sz:X}]; // bss off 0x{off:X}')
                    else:
                        generated.append(f'unsigned char {name}[0x{sz:X}]; // bss off 0x{off:X}')
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
                is_rodata = sec == ".rodata"
                # Determine type
                if name in extern_map:
                    typ, _ = extern_map[name]
                    # For file-backed, we need to handle const vs non-const
                    # Keep original typ, but ensure it matches section: for .rodata, if original typ is non-const but data is in rodata, it's still okay to have non-const type in rodata section via declspec
                    # We'll use the original typ as is, but add section attribute
                    # Need to handle array size: original extern had "[]", we need to provide size
                    # Determine element size
                    elem_size = type_size(typ)
                    # Clean typ: remove extern, keep const etc.
                    # typ already is like "u8" or "const unsigned char" or "char" etc.
                    # For file-backed, if typ is byte-like, we can emit as byte array directly
                    # If typ is u32 etc., we need to emit as that type's array
                    # Let's try to emit with original typ
                    # Check if typ contains "char" or is u8/s8 - use byte array
                    is_byte_type = elem_size == 1
                    if is_byte_type:
                        # Use the original typ (which may be "u8" or "char" etc.) but need to ensure it matches data length
                        # For byte types, size in bytes is sz, so array size is sz
                        # Keep typ as is
                        if idx == 0:
                            generated.append(f'__declspec(section "{sec}") __attribute__((aligned(8))) {typ} {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X}')
                        else:
                            generated.append(f'__declspec(section "{sec}") {typ} {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X}')
                    else:
                        # For u16, u32 etc.
                        count = sz // elem_size
                        if sz % elem_size != 0:
                            # fallback to byte
                            if idx == 0:
                                generated.append(f'__declspec(section "{sec}") __attribute__((aligned(8))) unsigned char {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X} (fallback byte, orig typ {typ})')
                            else:
                                generated.append(f'__declspec(section "{sec}") unsigned char {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X} (fallback byte, orig typ {typ})')
                        else:
                            # Convert data to that type's values
                            vals = []
                            for i in range(0, sz, elem_size):
                                chunk = data[i:i+elem_size]
                                if elem_size == 2:
                                    val = struct.unpack(">H", chunk)[0]
                                    # Need to handle signedness: if typ is s16, need to interpret as signed?
                                    # For s16, the value may be negative, but we can just emit as hex
                                    # Use signed interpretation for s16
                                    if "s16" in typ or "signed" in typ:
                                        val = struct.unpack(">h", chunk)[0]
                                        vals.append(f"{val}")
                                    else:
                                        vals.append(f"0x{val:04X}")
                                elif elem_size == 4:
                                    val = struct.unpack(">I", chunk)[0]
                                    if "s32" in typ or "int" in typ and "unsigned" not in typ:
                                        # Check if signed
                                        if "s32" in typ or (typ.strip() == "int" and "unsigned" not in typ):
                                            sval = struct.unpack(">i", chunk)[0]
                                            vals.append(f"{sval}")
                                        else:
                                            vals.append(f"0x{val:08X}")
                                    else:
                                        vals.append(f"0x{val:08X}")
                                else:
                                    vals.append(f"0x{int.from_bytes(chunk, 'big'):X}")
                            vals_str = ", ".join(vals)
                            if idx == 0:
                                generated.append(f'__declspec(section "{sec}") __attribute__((aligned(8))) {typ} {name}[0x{count:X}] = {{{vals_str}}}; // off 0x{off:X}')
                            else:
                                generated.append(f'__declspec(section "{sec}") {typ} {name}[0x{count:X}] = {{{vals_str}}}; // off 0x{off:X}')
                else:
                    # No extern, use default
                    if is_rodata:
                        # For rodata without extern, use const unsigned char
                        if idx == 0:
                            generated.append(f'__declspec(section "{sec}") __attribute__((aligned(8))) const unsigned char {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X}')
                        else:
                            generated.append(f'__declspec(section "{sec}") const unsigned char {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X}')
                    else:
                        if idx == 0:
                            generated.append(f'__declspec(section "{sec}") __attribute__((aligned(8))) unsigned char {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X}')
                        else:
                            generated.append(f'__declspec(section "{sec}") unsigned char {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X}')

    # Now process original file: remove extern lines for these symbols, and insert generated block after includes
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
                # Skip this line (will be replaced by definition in generated block)
                continue
        new_lines.append(line)

    # Find insertion point after last #include
    insert_idx = 0
    for i, line in enumerate(new_lines):
        if line.strip().startswith("#include"):
            insert_idx = i + 1
    # Insert generated block
    header = "\n// Absorbed from criware_data (retail bytes) - generated\n"
    block = "\n".join(generated)
    new_lines.insert(insert_idx, header + block)

    new_content = "\n".join(new_lines) + "\n"
    # Write with shift_jis encoding to preserve original
    try:
        src_path.write_bytes(new_content.encode("shift_jis", errors="replace"))
    except:
        src_path.write_text(new_content, encoding="utf-8", errors="replace")
    print(f"absorbed {u}: {len(generated)} defs")

print("done absorb4")
