#!/usr/bin/env python3
import pathlib, struct, json, re

# Test for mpv_hdec
dol_path = pathlib.Path("orig/us/sys/main.dol")
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

# For mpv_hdec, plan says .rodata 0x8051C1D8 size 0x28
# spec says 2 symbols at 0 and 0x18
# Let's generate asm block
plan = json.loads(pathlib.Path(".scratch/criware_data_plan.json").read_text())
u = "CriWare/src/sofdec/sfdcore/mpv/mpv_hdec.c"
# Get spec
spec_text = pathlib.Path(".scratch/criware_data_spec.txt").read_text().splitlines()
# Find the block for mpv_hdec
# Instead, just use plan and dol to generate
for secinfo in plan[u]:
    sec = secinfo["section"]
    start = int(secinfo["start"],16)
    end = int(secinfo["end"],16)
    size = end-start
    print(sec, hex(start), hex(size))
    data = dol_read(start, size)
    print(data[:64].hex() if data else "none")
    # Generate asm
    asm = []
    asm.append(f'.section {sec}')
    asm.append('.balign 8')
    # Need to handle symbols
    # For now, just emit the whole section as one blob with a single symbol at start?
    # But we need individual symbols
    # Let's just emit the whole section as raw bytes with a single global at start and fill
    # For testing, emit as:
    # .global lbl_eu_8051C1D8
    # lbl_eu_8051C1D8:
    # .byte ...
    # But we need to know symbol offsets
    # For mpv_hdec, symbols are at 0 and 0x18
    # We can emit:
    # lbl_eu_8051C1D8: .space 0x18
    # lbl_eu_8051C1F0: .space 0x10
    # But need bytes
    # Let's just emit the whole section as:
    # .global lbl_eu_8051C1D8
    # lbl_eu_8051C1D8: .byte ... (all 0x28 bytes)
    # And then define lbl_eu_8051C1F0 as an alias to lbl_eu_8051C1D8+0x18
    # We can do: .global lbl_eu_8051C1F0; lbl_eu_8051C1F0 = lbl_eu_8051C1D8 + 0x18
    # That would make both symbols point to correct offsets within the same section blob
    # This way, we only need one blob for the whole section, and the other symbols are aliases
    print("asm for whole section:")
    print(f'.global lbl_eu_8051C1D8')
    print(f'lbl_eu_8051C1D8:')
    # Emit bytes
    for i in range(0, len(data), 16):
        chunk = data[i:i+16]
        print(f'.byte {", ".join(f"0x{b:02X}" for b in chunk)}')
    print(f'.global lbl_eu_8051C1F0')
    print(f'lbl_eu_8051C1F0 = lbl_eu_8051C1D8 + 0x18')
