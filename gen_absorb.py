#!/usr/bin/env python3
import re, pathlib, struct
units = [
    "CriWare/src/sofdec/sfdcore/mpv/mpv_deli",
    "CriWare/src/sofdec/sfdcore/mpv/mpv_emp",
    "CriWare/src/sofdec/sfdcore/mpv/mpv_hdec",
    "CriWare/src/sofdec/sfdcore/mpv/mpv_lib",
    "CriWare/src/sofdec/sfdcore/mpv/mpv_mc",
    "CriWare/src/sofdec/sfdcore/mpv/mpv_mcy",
    "CriWare/src/sofdec/sfdcore/mpv/mpv_vlc",
    "CriWare/src/sofdec/sfdcore/mpv/mpvabdec",
    "CriWare/src/sofdec/sfdcore/sfd/code_803BFD20",
    "CriWare/src/sofdec/sfdcore/sfd/sfd_adxt",
    "CriWare/src/sofdec/sfdcore/sfd/sfd_aoap",
    "CriWare/src/sofdec/sfdcore/sfd/sfd_buf",
    "CriWare/src/sofdec/sfdcore/sfd/sfd_lib",
    "CriWare/src/sofdec/sfdcore/sfd/sfd_mpv",
    "CriWare/src/sofdec/sfdcore/sfd/sfd_ply",
    "CriWare/src/sofdec/sfdcore/sfd/sfd_seeki",
]
asm_root = pathlib.Path("build/us/asm")
def parse_asm(path):
    text = path.read_text()
    lines = text.splitlines()
    current_section = None
    section_objs = []
    cur_obj = None
    obj_re = re.compile(r"\.obj\s+(\S+),")
    for line in lines:
        stripped = line.strip()
        if stripped == ".rodata" or stripped.startswith("# .rodata"): current_section=".rodata"
        elif stripped == ".data" or stripped.startswith("# .data"): current_section=".data"
        elif stripped == ".bss" or stripped.startswith("# .bss"): current_section=".bss"
        elif stripped == ".sdata": current_section=".sdata"
        elif stripped == ".sdata2": current_section=".sdata2"
        elif stripped == ".sbss": current_section=".sbss"
        mobj = obj_re.search(line)
        if mobj:
            name = mobj.group(1).strip().rstrip(",")
            cur_obj = {"name": name, "section": current_section, "bytes": bytearray()}
            section_objs.append(cur_obj)
            continue
        if ".endobj" in line and cur_obj is not None:
            cur_obj=None
            continue
        if cur_obj is not None:
            ls = stripped
            if ls.startswith(".4byte"):
                hex_vals = re.findall(r"0x[0-9A-Fa-f]+", line)
                if hex_vals:
                    for hv in hex_vals:
                        cur_obj["bytes"].extend(struct.pack(">I", int(hv,16) & 0xFFFFFFFF))
                else:
                    if re.search(r"[A-Za-z_]", ls):
                        syms = [s.strip() for s in ls.split(".4byte",1)[1].split(",") if s.strip()]
                        cur_obj["bytes"].extend(b"\x00"*4*len(syms))
            elif ls.startswith(".2byte"):
                hex_vals = re.findall(r"0x[0-9A-Fa-f]+", line)
                if hex_vals:
                    for hv in hex_vals:
                        cur_obj["bytes"].extend(struct.pack(">H", int(hv,16) & 0xFFFF))
                else:
                    if re.search(r"[A-Za-z_]", ls):
                        syms = [s.strip() for s in ls.split(".2byte",1)[1].split(",") if s.strip()]
                        cur_obj["bytes"].extend(b"\x00"*2*len(syms))
            elif ls.startswith(".byte"):
                hex_vals = re.findall(r"0x[0-9A-Fa-f]+", line)
                for hv in hex_vals:
                    cur_obj["bytes"].append(int(hv,16) & 0xFF)
            elif ls.startswith(".string"):
                mstr = re.search(r'"(.*)"', line)
                if mstr:
                    s=mstr.group(1)
                    cur_obj["bytes"].extend(s.encode('utf-8')+b'\x00')
            elif ".rel" in ls:
                cur_obj["bytes"].extend(b"\x00\x00\x00\x00")
    return section_objs

for u in units:
    p = asm_root / (u+".s")
    if not p.exists():
        print(f"missing {p}")
        continue
    objs=parse_asm(p)
    print(f"== {u} ==")
    for o in objs:
        sec = o['section'] if o['section'] is not None else "?"
        hx=o['bytes'][:16].hex() if o['bytes'] else ""
        print(f"  {sec:8} {o['name']:35} size 0x{len(o['bytes']):X} {hx}")
    totals={}
    for o in objs:
        sec = o['section'] if o['section'] else "?"
        totals[sec]=totals.get(sec,0)+len(o['bytes'])
    print("  totals:", totals)
    print()
