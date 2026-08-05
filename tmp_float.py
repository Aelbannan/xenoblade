import struct

# Read the main.elf to find sdata2 float values
# The labels are at 0x80666DDC and 0x80666DD4 (EU/US virtual addresses)
# For US version: need to figure out file offset from VA

# Let's try reading the DOL directly
dol_path = "/Users/ahmedelbannan/Ahmed/xenoblade/build/us/main.dol"

with open(dol_path, "rb") as f:
    data = f.read()

# DOL header: 7 text sections, 11 data sections
# Each section has: offset[18], address[18], size[18]
import struct

# Read header
offsets = []
addresses = []
sizes = []
for i in range(18):
    off = struct.unpack_from(">I", data, i*4)[0]
    addr = struct.unpack_from(">I", data, 0x48 + i*4)[0]
    sz = struct.unpack_from(">I", data, 0x90 + i*4)[0]
    offsets.append(off)
    addresses.append(addr)
    sizes.append(sz)

# Find which section contains 0x80660000 region (sdata2 is typical around 0x8066xxxx)
print("DOL sections:")
for i in range(18):
    if sizes[i] > 0:
        print(f"  Section {i}: offset=0x{offsets[i]:08X} addr=0x{addresses[i]:08X} size=0x{sizes[i]:X}")

# sdata2 is typically the last data section before BSS
# Check which section covers 0x80666DD4 and 0x80666DDC
for va in [0x80666DD4, 0x80666DDC]:
    for i in range(18):
        if sizes[i] > 0 and addresses[i] <= va < addresses[i] + sizes[i]:
            file_off = offsets[i] + (va - addresses[i])
            val = struct.unpack_from(">f", data, file_off)[0]
            print(f"0x{va:08X}: file_off=0x{file_off:08X} float={val}")
            break
    else:
        print(f"0x{va:08X}: not found in DOL sections")