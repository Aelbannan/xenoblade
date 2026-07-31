#!/usr/bin/env python3
"""Rewrite src/kyoshin/code_800B06A4.cpp with functions in retail order."""
import re, struct, sys
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
from tools.ppc_equivalence.elf_symbols import list_text_functions

ASM_PATH = Path('build/us/asm/kyoshin/code_800B06A4.s')
OUT_PATH = Path('src/kyoshin/code_800B06A4.cpp.new')
RETAIL_OBJ = Path('build/us/obj/kyoshin/code_800B06A4.o')


def parse_asm():
    """Parse the retail assembly file, return list of function dicts."""
    text = ASM_PATH.read_text().split('\n')
    fns = []
    i = 0
    while i < len(text):
        m = re.match(r'\.fn\s+(\S+),\s*(\w+)', text[i])
        if m:
            name = m.group(1)
            offset = addr = size = 0
            for j in range(i - 1, max(i - 6, -1), -1):
                cm = re.match(
                    r'#\s*\.text:([0-9A-Fa-f]+)\s*\|\s*([0-9A-Fa-f]+)\s*\|\s*size:\s*([0-9A-Fa-f]+)',
                    text[j],
                )
                if cm:
                    offset = int(cm.group(1), 16)
                    addr = int(cm.group(2), 16)
                    size = int(cm.group(3), 16)
                    break
            asm = []
            j = i + 1
            while j < len(text) and not text[j].startswith('.endfn'):
                am = re.match(
                    r'/\*\s*[0-9A-Fa-f]+\s+[0-9A-Fa-f]+\s+[0-9A-Fa-f]+\s+\*/\s*(.*)',
                    text[j],
                )
                if am:
                    asm.append(am.group(1).strip())
                j += 1
            fns.append({
                'name': name, 'offset': offset, 'addr': addr,
                'size': size, 'asm': asm,
            })
            i = j
        else:
            i += 1
    return fns


def gen_impl(fb, name, asm):
    """Generate C/C++ for a function, or None if complex."""
    code = fb.code
    instrs = [struct.unpack('>I', code[i:i+4])[0] for i in range(0, len(code), 4)]

    # Just blr
    if len(instrs) == 1 and instrs[0] == 0x4E800020:
        return f'void {name}() {{}}'

    # li r3, 0 + blr
    if len(instrs) == 2 and instrs[0] == 0x38600000 and instrs[1] == 0x4E800020:
        return f'int {name}() {{ return 0; }}'

    # 2-instruction patterns: insn + blr
    if len(instrs) >= 2 and instrs[-1] == 0x4E800020:
        i0 = instrs[0]
        op = (i0 >> 26) & 0x3F
        rd = (i0 >> 21) & 0x1F
        ra = (i0 >> 16) & 0x1F
        imm = i0 & 0xFFFF
        if imm >= 0x8000:
            imm -= 0x10000

        # lwz r3, imm(r3/r31) -> u32 getter
        if op == 32 and rd == 3 and ra in (3, 31):
            return f'u32 {name}() {{ return *(u32*)((u8*)this + {imm}); }}'
        # lhz r3, imm(r3/r31) -> u16 getter
        if op == 40 and rd == 3 and ra in (3, 31):
            return f'u16 {name}() {{ return *(u16*)((u8*)this + {imm}); }}'
        # lbz r3, imm(r3/r31) -> u8 getter
        if op == 34 and rd == 3 and ra in (3, 31):
            return f'u8 {name}() {{ return *(u8*)((u8*)this + {imm}); }}'
        # addi r3, r3/r31, imm -> void* getter
        if op == 14 and rd == 3 and ra in (3, 31):
            return f'void* {name}() {{ return (u8*)this + {imm}; }}'
        # stw rX, imm(r3/r31) -> setter/clear
        if op == 36 and ra in (3, 31):
            if rd == 0:
                return f'void {name}() {{ *(u32*)((u8*)this + {imm}) = 0; }}'
            return f'void {name}(u32 val) {{ *(u32*)((u8*)this + {imm}) = val; }}'
        # stb rX, imm(r3) -> u8 setter
        if op == 38 and ra == 3:
            return f'void {name}(u8 val) {{ *(u8*)((u8*)this + {imm}) = val; }}'
        # sth rX, imm(r3) -> u16 setter
        if op == 44 and ra == 3:
            return f'void {name}(u16 val) {{ *(u16*)((u8*)this + {imm}) = val; }}'
        # stfs fX, imm(r3) -> float setter
        if op == 52 and ra == 3:
            return f'void {name}(float val) {{ *(float*)((u8*)this + {imm}) = val; }}'
        # lfs f1, imm(r3) -> float getter
        if op == 48 and rd == 1 and ra == 3:
            return f'float {name}() {{ return *(float*)((u8*)this + {imm}); }}'

    # 3-instruction: li r3, 0 + insn + blr (return 0 with extra)
    if len(instrs) >= 3 and instrs[0] == 0x38600000 and instrs[-1] == 0x4E800020:
        return f'int {name}() {{ return 0; }}'

    return None


def main():
    fns = parse_asm()
    print(f'Parsed {len(fns)} functions')

    retail_fn = list_text_functions(RETAIL_OBJ)
    retail_by_off = {f.value: f for f in retail_fn}

    out = [
        '// Auto-generated retail-ordered source for kyoshin/code_800B06A4',
        '// Replace stubs with high-level C/C++ during decomp.',
        '',
        '#include "kyoshin/harness_catalog.hpp"',
        '#include "kyoshin/code_800B06A4.hpp"',
        '',
        'UnkClass_805764CC* func_800B07E8();',
        'u32 func_800AA2BC(u32 a, u32 b);',
    ]

    emitted = set()
    impl = stub = cmplx = 0

    for fn in fns:
        name = fn['name']
        if name in emitted:
            continue
        emitted.add(name)

        off = fn['offset']
        fb = retail_by_off.get(off)

        out.append('')
        out.append(f'// {name} at +0x{off:04X}, size 0x{fn["size"]:X}')

        if fb is None:
            out.append(f'void {name}() {{}}')
            stub += 1
            continue

        code = gen_impl(fb, name, fn['asm'])
        if code:
            out.append(code)
            impl += 1
        else:
            out.append(f'void {name}() {{')
            for a in fn['asm'][:8]:
                out.append(f'    // {a}')
            if len(fn['asm']) > 8:
                out.append(f'    // ... ({len(fn["asm"])} total)')
            out.append('}')
            cmplx += 1

    out.append('')
    OUT_PATH.write_text('\n'.join(out))
    print(f'Wrote: {OUT_PATH}')
    print(f'Simple: {impl}, Stubs: {stub}, Complex: {cmplx}')
    print(f'Total: {len(emitted)}')


if __name__ == '__main__':
    main()