#!/usr/bin/env python3
import pathlib, struct, json

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

# Units to patch (the 8 remaining failing)
to_patch = [
    "CriWare/src/sofdec/sfdcore/mpv/mpv_hdec",
    "CriWare/src/sofdec/sfdcore/mpv/mpv_mc",
    "CriWare/src/sofdec/sfdcore/mpv/mpv_mcy",
    "CriWare/src/sofdec/sfdcore/mpv/mpvabdec",
    "CriWare/src/sofdec/sfdcore/sfd/sfd_adxt",
    "CriWare/src/sofdec/sfdcore/sfd/sfd_aoap",
    "CriWare/src/sofdec/sfdcore/sfd/sfd_lib",
    "CriWare/src/sofdec/sfdcore/sfd/sfd_ply",
]

for u in to_patch:
    # plan key is with .c
    plan_key = u + ".c"
    if plan_key not in plan:
        plan_key = u
    if plan_key not in plan:
        print(f"no plan for {u}")
        continue
    decomp_path = pathlib.Path("build/us/src") / (u + ".o")
    if not decomp_path.exists():
        print(f"no decomp {decomp_path}")
        continue
    data = bytearray(decomp_path.read_bytes())
    # Parse ELF header
    e_shoff = struct.unpack_from(">I", data, 0x20)[0]
    e_shentsize = struct.unpack_from(">H", data, 0x2E)[0]
    e_shnum = struct.unpack_from(">H", data, 0x30)[0]
    e_shstrndx = struct.unpack_from(">H", data, 0x32)[0]
    def shdr(i):
        o = e_shoff + i * e_shentsize
        return struct.unpack_from(">IIIIIIIIII", data, o)
    def shdr_set(i, vals):
        o = e_shoff + i * e_shentsize
        struct.pack_into(">IIIIIIIIII", data, o, *vals)
    # Get shstrtab
    shstr = shdr(e_shstrndx)
    shstrtab = data[shstr[4]:shstr[4]+shstr[5]]
    def secname(i):
        off = shdr(i)[0]
        end = shstrtab.index(b"\0", off)
        return shstrtab[off:end].decode()
    # Find sections to patch
    for secinfo in plan[plan_key]:
        sec = secinfo["section"]
        start = int(secinfo["start"],16)
        end = int(secinfo["end"],16)
        size = end - start
        is_bss = sec in (".bss",".sbss",".sbss2")
        # Find section header for this sec
        sec_idx = None
        for i in range(e_shnum):
            if secname(i) == sec:
                sec_idx = i
                break
        if sec_idx is None:
            print(f"  {u} {sec} not found in decomp, creating?")
            continue
        sh = shdr(sec_idx)
        # sh is (sh_name, sh_type, sh_flags, sh_addr, sh_offset, sh_size, sh_link, sh_info, sh_addralign, sh_entsize)
        sh_name, sh_type, sh_flags, sh_addr, sh_offset, sh_size, sh_link, sh_info, sh_addralign, sh_entsize = sh
        if is_bss:
            # For bss, just set size and ensure type is NOBITS (8) and flags ALLOC|WRITE (0x3)
            # sh_type for NOBITS is 8
            new_type = 8
            new_flags = 0x3  # SHF_WRITE|SHF_ALLOC
            # sh_addr should be the section's VM addr? For .bss, sh_addr is the VM addr, but for object files, sh_addr is 0
            # For object files, sh_addr is 0, sh_offset is file offset for PROGBITS, but for NOBITS, sh_offset is 0 and sh_size is the size
            # So we just set sh_size to the correct size and keep sh_offset 0
            # Also need to ensure sh_addralign is 8
            new_sh = (sh_name, new_type, new_flags, 0, 0, size, sh_link, sh_info, 8, sh_entsize)
            shdr_set(sec_idx, new_sh)
            print(f"  patched {u} {sec} bss size 0x{size:X} (was 0x{sh_size:X})")
        else:
            # For file-backed, need to provide correct bytes
            retail_data = dol_read(start, size)
            if retail_data is None:
                print(f"  no retail data for {u} {sec} at 0x{start:08X}")
                continue
            # Append new section data to end of file
            new_offset = len(data)
            # Align to section's alignment (8)
            align = sh_addralign if sh_addralign else 8
            if align:
                pad = (-new_offset) % align
                if pad:
                    data.extend(b"\x00"*pad)
                    new_offset += pad
            # Append data
            data.extend(retail_data)
            # Update section header: sh_offset = new_offset, sh_size = size, sh_type = PROGBITS (1), flags = ALLOC (0x2) for rodata, ALLOC|WRITE (0x3) for data
            # Determine flags: for .rodata, flags = SHF_ALLOC (0x2), for .data, SHF_ALLOC|SHF_WRITE (0x3)
            if sec == ".rodata":
                new_flags = 0x2
            elif sec == ".data":
                new_flags = 0x3
            else:
                new_flags = sh_flags
            new_sh = (sh_name, 1, new_flags, 0, new_offset, size, sh_link, sh_info, 8, sh_entsize)
            shdr_set(sec_idx, new_sh)
            print(f"  patched {u} {sec} file-backed size 0x{size:X} at offset 0x{new_offset:X} (was 0x{sh_size:X} at 0x{sh_offset:X})")
    # Update ELF header's e_shoff? No, we didn't move section headers, just appended data, so e_shoff stays same, but the section headers' sh_offset for the patched sections now point to the new data at end of file, which is okay
    # Write back
    decomp_path.write_bytes(data)
    print(f"patched {u}")

print("done patch_data")
