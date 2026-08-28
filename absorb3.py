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

# Helper to get type size
def type_size(t):
    t = t.strip()
    # Remove const, volatile, etc.
    t = re.sub(r'\b(const|volatile|static|extern)\b', '', t).strip()
    # Handle pointers? For now, assume array element type
    # Map
    if t in ("u8", "s8", "char", "unsigned char", "signed char", "uint8_t", "int8_t"):
        return 1
    if t in ("u16", "s16", "unsigned short", "short", "uint16_t", "int16_t", "wchar_t"):
        return 2
    if t in ("u32", "s32", "unsigned int", "int", "unsigned long", "long", "uint32_t", "int32_t", "float", "f32"):
        return 4
    if t in ("u64", "s64", "unsigned long long", "long long", "uint64_t", "int64_t", "double", "f64"):
        return 8
    # Default
    return 1

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
    # Build map of symbol -> (sec, sz, off)
    sym_map = {}
    for sec, sec_info in info.items():
        for off, sz, name in sec_info["symbols"]:
            sym_map[name] = (sec, sz, off, sec_info["start"])

    # Find extern declarations in original file
    # We'll parse lines with "extern" and extract type and name
    # Example: "extern u8 lbl_eu_8051C090[];" or "extern s16 lbl_eu_8051C190[];" or "extern u32 lbl_eu_80602A90[];"
    # Also "extern const unsigned char lbl_eu_...[];"
    extern_map = {}  # name -> (type_str, original_line)
    lines = original.splitlines()
    for line in lines:
        if "extern" not in line:
            continue
        # Find all symbols in this line that are in sym_map
        for name in list(sym_map.keys()):
            if re.search(r"\b" + re.escape(name) + r"\b", line):
                # Extract type: everything between "extern" and name
                # e.g., "extern u8 lbl_eu_..." -> type is "u8"
                # For "extern const unsigned char lbl_eu_..." -> type is "const unsigned char"
                # Use regex
                m = re.search(r"extern\s+(.+?)\s+" + re.escape(name) + r"\b", line)
                if m:
                    typ = m.group(1).strip()
                    extern_map[name] = (typ, line)
                else:
                    # fallback
                    extern_map[name] = ("unsigned char", line)
                break

    # Now we need to generate definitions
    # For each section, in order, generate definitions for symbols, with dummy fillers for bss gaps
    # We'll create a list of generated definition strings, in section order
    generated = []
    # Determine section order from plan if available, else spec order
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

    # For tracking which symbols were already handled via extern replacement (we will replace extern line with definition, not duplicate)
    # Instead, we will generate definitions for all symbols, but for those that had an extern, we will replace the extern line with the definition, and not also append at bottom
    # For those without extern, we will append at bottom (or near top)
    # To simplify, we will generate all definitions and then in the file processing, replace extern lines with definitions for those symbols, and for symbols without extern, append
    # So we need to know which symbols had extern

    # Build per-symbol definition strings
    definitions = {}  # name -> def string

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
                    # Need dummy filler before this symbol
                    # Generate a filler definition (not in sym_map, just a dummy)
                    filler_name = f"__bss_fill_{dummy_idx}_for_{name}"
                    # Use a unique name to avoid collision, but keep short
                    filler_name = f"__bss_fill_{dummy_idx}"
                    # Check if dummy name collides with existing symbol
                    while filler_name in sym_map:
                        dummy_idx += 1
                        filler_name = f"__bss_fill_{dummy_idx}"
                    # Determine if this filler is first in section (offset 0)
                    if offset == 0:
                        generated.append(f'__declspec(section "{sec}") __attribute__((aligned(8))) unsigned char {filler_name}[0x{gap:X}]; // filler gap at 0x{offset:X}')
                    else:
                        generated.append(f'__declspec(section "{sec}") unsigned char {filler_name}[0x{gap:X}]; // filler gap at 0x{offset:X}')
                    dummy_idx += 1
                # Now define the actual symbol
                # Determine type: if it had an extern, use that type, else use unsigned char
                if name in extern_map:
                    typ, _ = extern_map[name]
                    # For bss, the extern type may be like "u32" or "u16" etc., but we need to define with same type and size in elements
                    # Compute element count
                    elem_size = type_size(typ)
                    # If typ contains "char" or is u8, use byte size
                    # For bss, the size in spec is in bytes, but if typ is u32, the array should be [size/4]
                    # However, the extern's size was "[]", unknown, so we can define as [size] in bytes as unsigned char, but if we keep typ as u32, we need to adjust
                    # Let's keep the definition as unsigned char for simplicity, but we need to match the extern's type for the replacement
                    # Instead, we will use the original type for the definition, with element count
                    if elem_size == 1:
                        # Use as is
                        if off == 0 and offset == 0 and gap==0:
                            definitions[name] = f'__declspec(section "{sec}") __attribute__((aligned(8))) {typ} {name}[0x{sz:X}]; // bss off 0x{off:X}'
                        else:
                            definitions[name] = f'__declspec(section "{sec}") {typ} {name}[0x{sz:X}]; // bss off 0x{off:X}'
                    else:
                        # For u16, s16 etc., need element count
                        count = sz // elem_size
                        # If sz not multiple, fallback to byte
                        if sz % elem_size != 0:
                            definitions[name] = f'__declspec(section "{sec}") __attribute__((aligned(8))) unsigned char {name}[0x{sz:X}]; // bss off 0x{off:X} (fallback byte)'
                        else:
                            if off == 0 and offset == 0 and gap==0:
                                definitions[name] = f'__declspec(section "{sec}") __attribute__((aligned(8))) {typ} {name}[0x{count:X}]; // bss off 0x{off:X}'
                            else:
                                definitions[name] = f'__declspec(section "{sec}") {typ} {name}[0x{count:X}]; // bss off 0x{off:X}'
                    # For gap handling, we already added filler, so no extra
                else:
                    # No extern, use unsigned char
                    if off == 0 and offset == 0 and gap==0:
                        definitions[name] = f'__declspec(section "{sec}") __attribute__((aligned(8))) unsigned char {name}[0x{sz:X}]; // bss off 0x{off:X}'
                    else:
                        definitions[name] = f'__declspec(section "{sec}") unsigned char {name}[0x{sz:X}]; // bss off 0x{off:X}'
                offset = off + sz
            gap = sec_size - offset
            if gap > 0:
                filler_name = f"__bss_fill_{dummy_idx}"
                generated.append(f'__declspec(section "{sec}") unsigned char {filler_name}[0x{gap:X}]; // trailing filler')
                # Note: need to ensure this filler is added to generated, but we already appended per section? We'll handle separately
                # Actually we already are building generated incrementally, but for bss we built definitions dict, not generated list
                # Let's instead directly append to generated for bss case
                # For simplicity, we will later collect all bss defs in order
                pass
            # For bss, we have not yet added the symbol definitions to generated; we stored them in definitions dict
            # Now we need to add them to generated in order, interleaving fillers
            # Let's instead rebuild the bss section's generated list correctly
            # We'll redo: For bss, we should directly append to generated in order, not via definitions dict
            # To avoid complexity, we will handle bss separately: clear generated for this sec and rebuild
            # Simpler: For bss, directly append to a per-section list
            # Let's just clear and redo for this sec
            # We'll just handle bss via direct generation, not via definitions dict
            # But we already have definitions dict for bss, we can now generate the ordered list for this sec
            # Let's create a temporary list for this sec
            sec_generated = []
            offset = 0
            dummy_idx = 0
            for off, sz, name in symbols:
                gap = off - offset
                if gap > 0:
                    filler_name = f"__bss_fill_{dummy_idx}"
                    if offset == 0:
                        sec_generated.append(f'__declspec(section "{sec}") __attribute__((aligned(8))) unsigned char {filler_name}[0x{gap:X}]; // filler gap at 0x{offset:X}')
                    else:
                        sec_generated.append(f'__declspec(section "{sec}") unsigned char {filler_name}[0x{gap:X}]; // filler gap at 0x{offset:X}')
                    dummy_idx += 1
                # Use the definition from definitions dict
                sec_generated.append(definitions[name])
                offset = off + sz
            gap = sec_size - offset
            if gap > 0:
                filler_name = f"__bss_fill_{dummy_idx}"
                sec_generated.append(f'__declspec(section "{sec}") unsigned char {filler_name}[0x{gap:X}]; // trailing filler')
            # Now replace the generic generated entries for this sec with sec_generated
            # But we already appended earlier? Let's just set generated to include sec_generated in order
            # Instead of appending to global generated, we will directly extend global with sec_generated
            # But we already appended some entries to global earlier (the filler and definitions via definitions dict not yet added)
            # To avoid double, we will not have appended earlier for bss; we handled via definitions dict, so we should now extend global with sec_generated
            # Let's do: remove any previous entries for this sec from generated (we haven't added them yet, we only added to definitions dict)
            # So just extend
            generated.extend(sec_generated)
        else:
            # file-backed
            for idx, (off, sz, name) in enumerate(symbols):
                abs_addr = sec_start + off
                data = dol_read(abs_addr, sz)
                if data is None:
                    print(f"WARNING no DOL data for {name} at 0x{abs_addr:08X} sz 0x{sz:X} in {u}")
                    data = bytes([0]*sz)
                is_rodata = sec == ".rodata"
                # Determine type: if name in extern_map, use that type, but for file-backed rodata, the extern type may be u8, char, etc.
                # For gap symbols, they are like "gap_06_..." with no extern, use unsigned char
                if name in extern_map:
                    typ, _ = extern_map[name]
                    # For file-backed, the definition should be with same type but with initializer
                    # Need to handle element size
                    elem_size = type_size(typ)
                    # If typ is like "u8" or "char", we can just use unsigned char
                    # If typ is "u32" with size 0x6, that's not multiple of 4, so fallback
                    if elem_size != 1 and sz % elem_size != 0:
                        # fallback to byte
                        typ = "unsigned char"
                        elem_size = 1
                    if elem_size == 1:
                        # Use typ as is, but ensure const for rodata
                        if is_rodata and "const" not in typ:
                            typ = "const " + typ
                        # For data, ensure not const
                        if not is_rodata and "const" in typ:
                            typ = typ.replace("const","").strip()
                            typ = re.sub(r'\s+', ' ', typ)
                        count = sz
                        # But if typ is not byte, count is in elements, but we will emit as byte array for simplicity
                        # Instead, emit as typ array with element count
                        # For byte types, emit as bytes
                        if typ.strip() in ("unsigned char", "const unsigned char", "u8", "s8", "char", "const char"):
                            # Use byte array
                            if idx == 0:
                                definitions[name] = f'__declspec(section "{sec}") __attribute__((aligned(8))) {typ} {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X}'
                            else:
                                definitions[name] = f'__declspec(section "{sec}") {typ} {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X}'
                        else:
                            # For u16, s16, u32 etc., define as that type with element count
                            count = sz // elem_size
                            # Need to convert data bytes to that type's values
                            # For u16, data is big-endian, need to pack as 0x...
                            vals = []
                            for i in range(0, sz, elem_size):
                                chunk = data[i:i+elem_size]
                                if len(chunk) < elem_size:
                                    chunk = chunk + b'\x00'*(elem_size-len(chunk))
                                if elem_size == 2:
                                    val = struct.unpack(">H", chunk)[0]
                                    vals.append(f"0x{val:04X}")
                                elif elem_size == 4:
                                    val = struct.unpack(">I", chunk)[0]
                                    vals.append(f"0x{val:08X}")
                                else:
                                    vals.append(f"0x{chunk.hex()}")
                            vals_str = ", ".join(vals)
                            if idx == 0:
                                definitions[name] = f'__declspec(section "{sec}") __attribute__((aligned(8))) {typ} {name}[0x{count:X}] = {{{vals_str}}}; // off 0x{off:X}'
                            else:
                                definitions[name] = f'__declspec(section "{sec}") {typ} {name}[0x{count:X}] = {{{vals_str}}}; // off 0x{off:X}'
                    else:
                        # multi-byte type
                        count = sz // elem_size
                        vals = []
                        for i in range(0, sz, elem_size):
                            chunk = data[i:i+elem_size]
                            if elem_size == 2:
                                val = struct.unpack(">H", chunk)[0]
                                vals.append(f"0x{val:04X}")
                            elif elem_size == 4:
                                val = struct.unpack(">I", chunk)[0]
                                vals.append(f"0x{val:08X}")
                            else:
                                vals.append(f"0x{int.from_bytes(chunk, 'big'):X}")
                        vals_str = ", ".join(vals)
                        if is_rodata and "const" not in typ:
                            typ = "const " + typ
                        if idx == 0:
                            definitions[name] = f'__declspec(section "{sec}") __attribute__((aligned(8))) {typ} {name}[0x{count:X}] = {{{vals_str}}}; // off 0x{off:X}'
                        else:
                            definitions[name] = f'__declspec(section "{sec}") {typ} {name}[0x{count:X}] = {{{vals_str}}}; // off 0x{off:X}'
                else:
                    # No extern, use default types: for rodata const unsigned char, for data unsigned char
                    if is_rodata:
                        typ = "const unsigned char"
                        if idx == 0:
                            definitions[name] = f'__declspec(section "{sec}") __attribute__((aligned(8))) {typ} {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X}'
                        else:
                            definitions[name] = f'__declspec(section "{sec}") {typ} {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X}'
                    else:
                        typ = "unsigned char"
                        if idx == 0:
                            definitions[name] = f'__declspec(section "{sec}") __attribute__((aligned(8))) {typ} {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X}'
                        else:
                            definitions[name] = f'__declspec(section "{sec}") {typ} {name}[0x{sz:X}] = {{{fmt_bytes(data)}}}; // off 0x{off:X}'
            # For file-backed, add definitions to generated in order
            for off, sz, name in symbols:
                generated.append(definitions[name])

    # Now we have generated list in section order
    # Need to handle the file content: Replace extern lines with definitions for those symbols that had extern, and for symbols without extern, we will have definitions at bottom (already in generated)
    # But for symbols that had extern, we have a definition in generated as well, which would duplicate if we also keep the replaced extern line as a definition
    # Our plan: For symbols that had an extern, we will replace that extern line with the definition (so that definition appears at the original extern's location, before any use)
    # For symbols that did not have an extern, we will keep the generated definition to be appended at the end (but we need to ensure they are placed correctly for section layout)
    # However, for correct section layout, the order of definitions in the file matters: The linker will place section objects in the order they appear in the source file.
    # If we replace an extern at the top with a definition, and also have other definitions at the bottom, the order will be: top definitions (for symbols that had extern) first, then bottom definitions (for symbols without extern) later.
    # But the retail order is by offset, not by whether the symbol had an extern.
    # For example, in mpv_vlc, the bss symbols that had extern are at various offsets, but the ones without extern (maybe none) would be at bottom, disrupting order.
    # Safer to just generate all definitions in a single block at the top of the file (after includes), in correct offset order, and remove all original extern lines.
    # Then the order will be correct.

    # Let's do: Remove all extern lines for these symbols, and then insert a single block of definitions right after the includes (or at the top after the first #include)
    # Find the insertion point: after the last #include line
    lines = original.splitlines()
    new_lines = []
    # First, filter out extern lines for our symbols
    for line in lines:
        if "extern" in line:
            found = False
            for name in list(sym_map.keys()):
                if re.search(r"\b" + re.escape(name) + r"\b", line):
                    found = True
                    break
            if found:
                # Skip this line (remove)
                continue
        new_lines.append(line)

    # Find insertion point: after last #include
    insert_idx = 0
    for i, line in enumerate(new_lines):
        if line.strip().startswith("#include"):
            insert_idx = i + 1
    # Also skip any blank lines after includes
    # Insert generated block there
    header = "\n// Absorbed from criware_data (retail bytes) - generated\n"
    block = "\n".join(generated)
    new_lines.insert(insert_idx, header + block)

    new_content = "\n".join(new_lines) + "\n"
    src_path.write_text(new_content)
    print(f"absorbed {u}: {len(generated)} defs")

print("done absorb3")
