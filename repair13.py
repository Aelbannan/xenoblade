import struct
from pathlib import Path

ROOT = Path("/Users/ahmed/projects/xenoblade")
units = [
"kyoshin/cf/CBattleManager.cpp",
"kyoshin/cf/CItem.cpp",
"kyoshin/cf/CPartsChange.cpp",
"kyoshin/cf/CfBdat.cpp",
"kyoshin/cf/CfCam.cpp",
"kyoshin/cf/CfCamEvent_1.cpp",
"kyoshin/cf/CfMapItemManager.cpp",
"kyoshin/cf/CfNandManager.cpp",
"kyoshin/cf/CfResPcImpl.cpp",
"kyoshin/cf/object/CfObjectMove.cpp",
"kyoshin/cfsys/CfObjectImplPc.cpp",
"kyoshin/code_801862C0.cpp",
"kyoshin/menu/CMenuEnemyState.cpp",
]

DATA_SECTIONS = (".data",".rodata",".sdata",".sdata2",".bss",".sbss",".sbss2")
NOBITS = frozenset({".bss",".sbss",".sbss2"})

def parse_elf(path):
    data = Path(path).read_bytes()
    shoff = struct.unpack_from(">I", data, 0x20)[0]
    shentsize = struct.unpack_from(">H", data, 0x2E)[0]
    shnum = struct.unpack_from(">H", data, 0x30)[0]
    shstrndx = struct.unpack_from(">H", data, 0x32)[0]
    def shdr(i):
        o=shoff+i*shentsize
        return struct.unpack_from(">IIIIIIIIII", data, o)
    shstr=shdr(shstrndx)
    tab=data[shstr[4]:shstr[4]+shstr[5]]
    def secname(i):
        off=shdr(i)[0]
        end=tab.index(b"\x00", off)
        return tab[off:end].decode()
    secs={}
    for i in range(shnum):
        nm = secname(i)
        if nm in DATA_SECTIONS:
            sh=shdr(i)
            secs[nm] = {"off": sh[4], "size": sh[5], "align": sh[8], "data": data[sh[4]:sh[4]+sh[5]] if sh[5] else b""}
    return secs

def bytes_to_c_array(b):
    lines=[]
    for i in range(0, len(b), 12):
        chunk=b[i:i+12]
        lines.append(", ".join(f"0x{c:02X}" for c in chunk))
    return ",\n    ".join(lines)

for unit in units:
    src = ROOT / f"src/{unit}"
    retail = ROOT / f"build/us/obj/{unit.replace('.cpp','.o')}"
    if not src.exists():
        print(f"missing src {unit}")
        continue
    if not retail.exists():
        print(f"missing retail {unit} at {retail}")
        continue
    text = src.read_text()
    marker = "// absorb: split1"
    idx = text.find(marker)
    if idx != -1:
        text = text[:idx].rstrip() + "\n"
        src.write_text(text)
        print(f"truncated {unit}")
    r_secs = parse_elf(retail)
    need = [s for s in DATA_SECTIONS if s in r_secs and r_secs[s]["size"]>0]
    if not need:
        print(f"{unit} no need")
        continue
    lines = []
    lines.append("\n// absorb: split1 retail data sections")
    lines.append("// generated from retail build/us/obj via repair_absorb.py")
    for sec in need:
        info = r_secs[sec]
        size = info["size"]
        align = info["align"]
        if align==0:
            align=4
        safe = unit.replace("/","_").replace(".","_").replace("-","_")
        var = f"__absorb_{safe}_{sec.strip('.')}"
        if sec in NOBITS:
            if sec == ".bss":
                lines.append(f"__attribute__((aligned({align}))) __attribute__((used)) unsigned char {var}[0x{size:X}];")
            elif sec in (".sbss", ".sbss2"):
                n_ints = size // 4
                rem = size % 4
                for i in range(n_ints):
                    lines.append(f"int {var}_{i};")
                if rem:
                    lines.append(f"unsigned char {var}_rem[{rem}];")
            else:
                lines.append(f"// TODO {sec} size {size}")
        else:
            b = info["data"]
            c_arr = bytes_to_c_array(b)
            if sec in (".rodata",".sdata2"):
                decl = f'__declspec(section "{sec}") __attribute__((aligned({align}))) __attribute__((used)) const unsigned char {var}[0x{size:X}] = {{\n    {c_arr}\n}};'
            else:
                decl = f'__declspec(section "{sec}") __attribute__((aligned({align}))) __attribute__((used)) unsigned char {var}[0x{size:X}] = {{\n    {c_arr}\n}};'
            lines.append(decl)
    absorb_code = "\n".join(lines) + "\n"
    with open(src, "a") as f:
        f.write(absorb_code)
    print(f"repaired {unit} with {need}")

print("done")
