#!/usr/bin/env python3
"""
Decompile the code_800B06A4 unit from retail assembly.

Reads the retail assembly file, analyzes each function, and generates
a high-level C/C++ source file.
"""

import re
import sys
from pathlib import Path
from typing import Optional


def parse_asm_file(path: str) -> list[dict]:
    """Parse the retail assembly file into function blocks."""
    with open(path) as f:
        lines = f.readlines()

    functions = []
    current_fn = None
    current_asm = []
    fn_start = None

    for line in lines:
        # .fn directive
        m = re.match(r'\.fn\s+(\S+),\s*(\w+)', line)
        if m:
            if current_fn:
                current_fn['asm'] = current_asm
                if current_fn.get('start_line') is not None:
                    functions.append(current_fn)
            current_fn = {
                'name': m.group(1),
                'scope': m.group(2),
                'asm': [],
                'start_line': None,
            }
            current_asm = []
            continue

        # .endfn directive
        if re.match(r'\.endfn', line):
            if current_fn:
                current_fn['asm'] = current_asm
                if current_fn.get('start_line') is not None:
                    functions.append(current_fn)
            current_fn = None
            current_asm = []
            continue

        # Comment with offset
        m = re.match(r'/\*\s*([0-9A-Fa-f]+)\s+[0-9A-Fa-f]+\s+([0-9A-Fa-f]+)\s+\*/\s*(.*)', line)
        if m:
            asm_line = {
                'address': int(m.group(1), 16),
                'bytes': m.group(2),
                'text': m.group(3).strip(),
            }
            if current_fn is not None:
                if current_fn['start_line'] is None:
                    current_fn['start_line'] = asm_line
                current_asm.append(asm_line)
            continue

        # Comment with offset and size
        m = re.match(r'#\s*\.text:([0-9A-Fa-f]+)\s*\|\s*([0-9A-Fa-f]+)\s*\|\s*size:\s*([0-9A-Fa-f]+)', line)
        if m:
            if current_fn and current_fn['start_line'] is None:
                current_fn['text_offset'] = int(m.group(1), 16)
                current_fn['address'] = int(m.group(2), 16)
                current_fn['size'] = int(m.group(3), 16)

    return functions


def categorize_asm(asm_lines: list[dict]) -> str:
    """Categorize a function by its assembly pattern."""
    if not asm_lines:
        return 'empty'

    # Check for common patterns
    texts = [l['text'] for l in asm_lines]

    # blr only (empty function)
    if len(asm_lines) == 1 and 'blr' in texts[0]:
        return 'empty'

    # Simple getter: lwz r3, X(r3) + blr
    if len(asm_lines) == 2 and 'lwz' in texts[0] and 'blr' in texts[1]:
        return 'getter_u32'

    # Simple getter: lhz r3, X(r3) + blr
    if len(asm_lines) == 2 and 'lhz' in texts[0] and 'blr' in texts[1]:
        return 'getter_u16'

    # Simple getter: lbz r3, X(r3) + blr
    if len(asm_lines) == 2 and 'lbz' in texts[0] and 'blr' in texts[1]:
        return 'getter_u8'

    # Return 0: li r3, 0 + blr
    if len(asm_lines) == 2 and 'li r3, 0' in texts[0] and 'blr' in texts[1]:
        return 'return_0'

    # Return pointer: addi r3, rS, X + blr
    if len(asm_lines) == 2 and 'addi' in texts[0] and 'blr' in texts[1]:
        return 'return_ptr'

    # Return value: mr r3, rX + blr
    if len(asm_lines) == 2 and 'mr r3,' in texts[0] and 'blr' in texts[1]:
        return 'return_reg'

    # Simple setter: stw r3, X(r3) + blr (or similar)
    if len(asm_lines) == 2 and 'stw' in texts[0] and 'blr' in texts[1]:
        return 'setter_u32'

    # no-op: just blr
    if len(asm_lines) == 1 and 'blr' in texts[0]:
        return 'nop'

    return 'complex'


def generate_function(fn: dict, header_path: str) -> str:
    """Generate C/C++ code for a function."""
    name = fn['name']
    asm = fn['asm']
    category = categorize_asm(asm)
    size = fn.get('size', 0)

    # Skip certain functions that are handled elsewhere
    if name.startswith('__dt__') or name.startswith('__ct__'):
        category = 'constructor_destructor'

    # Extract address for reference
    addr = fn.get('address', 0)
    offset = fn.get('text_offset', 0)

    code = f'\n\n// {name} at 0x{addr:X} (offset +0x{offset:X}, size 0x{size:X})\n'

    # Try to find a meaningful name based on pattern
    meaningful_name = name

    if category == 'empty':
        # Empty function
        code += f'void {name}() {{}}\n'
        return code

    elif category == 'return_0':
        code += f'int {name}() {{ return 0; }}\n'
        return code

    elif category == 'nop':
        code += f'void {name}() {{}}\n'
        return code

    elif category == 'getter_u32':
        # lwz r3, offset(r3)  +  blr
        m = re.search(r'lwz\s+r3,\s*([-0-9]+)\(r3\)', asm[0]['text'])
        if m:
            offset_val = int(m.group(1))
            code += f'u32 {name}() {{ return *(u32*)((u8*)this + {offset_val}); }}\n'
            return code
        # lwz r3, offset(rS) where S!=3
        m = re.search(r'lwz\s+r3,\s*([-0-9]+)\(r(\d+)\)', asm[0]['text'])
        if m:
            offset_val = int(m.group(1))
            reg = m.group(2)
            if reg == '31':
                code += f'u32 {name}() {{ u32 val = *(u32*)((u8*)this + {offset_val}); return val; }}\n'
                return code
            code += f'u32 {name}(void* self) {{ return *(u32*)((u8*)self + {offset_val}); }}\n'
            return code

    elif category == 'getter_u16':
        m = re.search(r'lhz\s+r3,\s*([-0-9]+)\(r3\)', asm[0]['text'])
        if m:
            offset_val = int(m.group(1))
            code += f'u16 {name}() {{ return *(u16*)((u8*)this + {offset_val}); }}\n'
            return code

    elif category == 'getter_u8':
        m = re.search(r'lbz\s+r3,\s*([-0-9]+)\(r3\)', asm[0]['text'])
        if m:
            offset_val = int(m.group(1))
            code += f'u8 {name}() {{ return *(u8*)((u8*)this + {offset_val}); }}\n'
            return code

    elif category == 'return_ptr':
        # addi r3, rS, offset  +  blr
        m = re.search(r'addi\s+r3,\s*r(\d+),\s*([-0-9]+)', asm[0]['text'])
        if m:
            reg = m.group(1)
            offset_val = int(m.group(2))
            if reg == '3':
                code += f'void* {name}() {{ return (u8*)this + {offset_val}; }}\n'
                return code
            code += f'void* {name}(void* self) {{ return (u8*)self + {offset_val}; }}\n'
            return code

    elif category == 'setter_u32':
        # stw rX, offset(rY)  +  blr
        m = re.search(r'stw\s+r(\d+),\s*([-0-9]+)\(r(\d+)\)', asm[0]['text'])
        if m:
            src_reg = m.group(1)
            offset_val = int(m.group(2))
            dst_reg = m.group(3)
            if dst_reg == '3' and src_reg == '4':
                code += f'void {name}(u32 val) {{ *(u32*)((u8*)this + {offset_val}) = val; }}\n'
                return code
            if dst_reg == '3' and src_reg == '0':
                code += f'void {name}() {{ *(u32*)((u8*)this + {offset_val}) = 0; }}\n'
                return code

    elif category == 'return_reg':
        m = re.search(r'mr\s+r3,\s*r(\d+)', asm[0]['text'])
        if m:
            reg = m.group(1)
            if reg == '3':
                return f'void* {name}(void* self) {{ return self; }}\n'
            # this is in r3, return value from another reg
            code += f'void* {name}(void* self) {{ return *(void**)((u8*)self); }}\n'
            return code

    elif category == 'constructor_destructor':
        # Skip - these are handled by the header
        return f'// {name} - constructor/destructor (handled elsewhere)\n'

    # Complex function - generate manual implementation with comments
    code += f'// Complex function, size 0x{size:X}\n'
    code += f'// TODO: implement {name}\n'
    code += f'void {name}() {{\n'
    code += f'    // Assembly:\n'
    for line in asm[:5]:
        code += f'    // {line["text"]}\n'
    if len(asm) > 5:
        code += f'    // ... ({len(asm)} instructions total)\n'
    code += '}\n'
    return code


def main():
    asm_path = 'build/us/asm/kyoshin/code_800B06A4.s'
    if not Path(asm_path).exists():
        print(f"ERROR: {asm_path} not found")
        sys.exit(1)

    functions = parse_asm_file(asm_path)
    print(f"Parsed {len(functions)} functions")

    # Generate source file
    output = '''// Auto-generated from retail assembly for kyoshin/code_800B06A4
// High-level C/C++ reconstruction

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/code_800B06A4.hpp"

// Forward declarations
UnkClass_805764CC* func_800B07E8();
u32 func_800AA2BC(u32 a, u32 b);

'''

    for fn in functions:
        name = fn['name']
        if name.startswith('__dt__') or name.startswith('__ct__'):
            # Skip constructors/destructors for now
            continue
        output += generate_function(fn, '')

    # Write output
    out_path = 'src/kyoshin/code_800B06A4.cpp.new'
    Path(out_path).write_text(output)
    print(f"Wrote {out_path}")


if __name__ == '__main__':
    main()