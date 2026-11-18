#!/usr/bin/env python3
"""vt.py <retail obj rel to build/us/obj> : dump .data symbols with their relocations (vtables)."""
import sys
from elftools.elf.elffile import ELFFile
with open('build/us/obj/'+sys.argv[1],'rb') as f:
    e=ELFFile(f); st=e.get_section_by_name('.symtab'); secs=[s.name for s in e.iter_sections()]
    want = sys.argv[2:] or ['.data']
    for sec in want:
        di = secs.index(sec) if sec in secs else None
        if di is None: continue
        syms=sorted([(s['st_value'],s['st_size'],s.name) for s in st.iter_symbols() if s['st_shndx']==di and s.name], key=lambda x:x[0])
        rel=e.get_section_by_name('.rela'+sec)
        rl={}
        if rel:
            for r in rel.iter_relocations(): rl[r['r_offset']]=(st.get_symbol(r['r_info_sym']).name, r['r_addend'])
        data=e.get_section_by_name(sec).data()
        for v,sz,n in syms:
            print(f"{sec} {v:#x} {sz:#x} {n}")
            for o in range(v, v+sz, 4):
                if o in rl: print(f"    +{o-v:#04x} {rl[o][0]}{'+%#x'%rl[o][1] if rl[o][1] else ''}")
                else:
                    w=int.from_bytes(data[o:o+4],'big')
                    if w: print(f"    +{o-v:#04x} {w:#010x}")
