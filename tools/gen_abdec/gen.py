#!/usr/bin/env python3
"""Generate the high-level C reconstruction for mpvabdec_IntraBlock_Isr.

Reads per-body parameters from body_params.py and emits C case bodies in the
retail code layout order (loop bodies descending, then exit bodies descending).
"""
import sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from body_params import BODIES

MULT = {1: 2, 2: 4, 3: 6}  # mult_k -> quant multiplier

def case_labels(b):
    return ' '.join(f'case {v}:' for v in b['bytes'])

def coeff_block(cvar, off, mult, neg, indent):
    m = MULT[mult]
    s = []
    s.append(f"{indent}s32 {cvar} = (s8)p[{off}];")
    s.append(f"{indent}blk->last = {cvar};")
    s.append(f"{indent}s32 s{cvar} = (({m} * (s32)blk->quant) * (s32)blk->clip[{cvar}]) >> 4;")
    if neg:
        s.append(f"{indent}s{cvar} = -((s{cvar} - 1) | 1);")
    else:
        s.append(f"{indent}s{cvar} = (s{cvar} - 1) | 1;")
    s.append(f"{indent}s32 r{cvar} = (s{cvar} * (s32)ctx->tblqt[{cvar}] + 0x400) >> 11;")
    s.append(f"{indent}blk->block[{cvar}] = (s16)r{cvar};")
    return s

def refill_expr(amount):
    return (f"bc += {amount};\n"
            f"        if (bc >= 0x20) {{\n"
            f"            bc -= 0x20;\n"
            f"            hi = lo << bc;\n"
            f"            lo = *ptr++;\n"
            f"        }} else {{\n"
            f"            hi <<= {amount};\n"
            f"        }}")

def refill_codelen():
    return ("bc += (s32)blk->codelen;\n"
            "        if (bc >= 0x20) {\n"
            "            bc -= 0x20;\n"
            "            hi = lo << bc;\n"
            "            lo = *ptr++;\n"
            "        } else {\n"
            "            hi <<= (s32)blk->codelen;\n"
            "        }")

def body_simple(b):
    out = []
    if b['coeffs'] == 2:
        out += coeff_block('c1', b['off'], b['mult'], b['neg1'], '    ')
        out.append(f"    p += {b['off'] + 1};")
        out += coeff_block('c2', 0, b['mult'], b['neg2'], '    ')
    else:
        out.append(f"    p += {b['off']};")
        out += coeff_block('c1', 0, b['mult'], b['neg1'], '    ')
    out.append("    " + refill_expr(b['bits']))
    if b['exit'] == 'fall':
        pass
    elif b['exit']:
        out.append("    break;")
    else:
        out.append("    continue;")
    return out

# --- special bodies (written from retail) ---

def huff_cont(sign_expr):
    """Continuation shared by the huffman-style cases: coefficient decode."""
    return f"""        blk->f00 = (u32)(t & 0xFF);
        blk->f04 = (s32)(s8)(t >> 8);
        blk->f08 = (u32)({sign_expr});
        {refill_codelen()}
        p += blk->f00;
        s32 c = (s8)*++p;
        blk->last = c;
        s32 s = ((2 * (s32)blk->f04 * (s32)blk->quant) * (s32)blk->clip[c]) >> 4;
        if (blk->f08 != 0) {{
            s = -((s - 1) | 1);
        }} else {{
            s = (s - 1) | 1;
        }}
        s32 r = (s * (s32)ctx->tblqt[c] + 0x400) >> 11;
        blk->block[c] = (s16)r;
        continue;"""

def body_escape_long():
    return f"""        u32 x = bits << 1;
        s16 t;
        u32 e;
        if ((x >> 24) != 0) {{
            blk->codelen = 0x0E;
            e = x >> 19;
            t = ctx->esc1[(e & ~1u)];
        }} else {{
            x <<= 8;
            if ((s32)x < 0) {{
                blk->codelen = 0x0F;
                e = (x >> 1) & 0x1F;
                t = ctx->esc2[(e & ~1u)];
            }} else {{
                x <<= 1;
                if ((s32)x < 0) {{
                    blk->codelen = 0x10;
                    e = (x >> 1) & 0x1F;
                    t = ctx->esc3[(e & ~1u)];
                }} else {{
                    blk->codelen = 0x11;
                    e = (x >> 2) & 0x1F;
                    t = ctx->esc4[(e & ~1u)];
                }}
            }}
        }}
{huff_cont('e & 1')}"""

def body_huff(blen, tbl, codelen):
    return f"""        blk->codelen = {codelen};
        u32 x = bits;
        u32 e = (x >> 1) & {blen};
        s16 t = ctx->{tbl}[(e & ~1u)];
{huff_cont('e & 1')}"""

def body_escape():
    return f"""        u32 x = bits << 1;
        blk->codelen = 0x14;
        s32 v = (s32)((x >> 5) & 0xFFFF) >> 2;
        blk->f00 = (u32)(s8)(v >> 8);
        if (((x >> 12) & 0x7F) != 0) {{
            /* skip */
        }} else {{
            blk->codelen = 0x1C;
            v = ((s32)(s8)((x >> 11) & 0xFF) << 1) | (s32)((x >> 19) & 0xFF);
        }}
        if (v < 0) {{
            blk->f08 = 1;
            v = -v;
        }} else {{
            blk->f08 = 0;
        }}
        blk->f04 = (s32)v;
        {refill_codelen()}
        p += blk->f00;
        s32 c = (s8)*++p;
        blk->last = c;
        s32 s = ((2 * (s32)blk->f04 * (s32)blk->quant) * (s32)blk->clip[c]) >> 4;
        if (blk->f08 != 0) {{
            s = -((s - 1) | 1);
        }} else {{
            s = (s - 1) | 1;
        }}
        s32 r = (s * (s32)ctx->tblqt[c] + 0x400) >> 11;
        blk->block[c] = (s16)r;
        continue;"""

def body_huff990():
    return f"""        u32 x = bits << 1;
        u32 t = ctx->tbl990[(x >> 25) & 0x7F];
        blk->f00 = (u32)(t & 0xFF);
        if ((t & 0xFF) == 0x40) {{
            blk->codelen = 0x14;
            s32 v = (s32)((x >> 5) & 0xFFFF) >> 2;
            blk->f00 = (u32)(s8)(v >> 8);
            if (((x >> 12) & 0x7F) != 0) {{
                /* skip */
            }} else {{
                blk->codelen = 0x1C;
                v = ((s32)(s8)((x >> 11) & 0xFF) << 1) | (s32)((x >> 19) & 0xFF);
            }}
            if (v < 0) {{
                blk->f08 = 1;
                v = -v;
            }} else {{
                blk->f08 = 0;
            }}
            blk->f04 = (s32)v;
        }} else {{
            blk->codelen = (s32)(t >> 16);
            blk->f04 = (s32)(s8)((t >> 8) & 0xFF);
            x >>= (0x21 - (s32)(t >> 16));
            blk->f08 = x & 1;
        }}
        {refill_codelen()}
        p += blk->f00;
        s32 c = (s8)*++p;
        blk->last = c;
        s32 s = ((2 * (s32)blk->f04 * (s32)blk->quant) * (s32)blk->clip[c]) >> 4;
        if (blk->f08 != 0) {{
            s = -((s - 1) | 1);
        }} else {{
            s = (s - 1) | 1;
        }}
        s32 r = (s * (s32)ctx->tblqt[c] + 0x400) >> 11;
        blk->block[c] = (s16)r;
        continue;"""

def body_special(name):
    if name == 'eob':
        return ("    " + refill_expr(2)).split('\n') + ["    break;"]
    if name == 'escape_long':
        return body_escape_long().split('\n')
    if name == 'huff11':
        return body_huff('0x3FF', 'tbl994', '0x0B').split('\n')
    if name == 'huff13':
        return body_huff('0xFFF', 'tbl998', '0x0D').split('\n')
    if name == 'escape':
        return body_escape().split('\n')
    if name == 'huff990':
        return body_huff990().split('\n')
    raise ValueError(name)

def gen_switch():
    out = []
    for boff in sorted(BODIES):
        b = BODIES[boff]
        out.append(f"        {case_labels(b)} {{")
        if 'special' in b:
            out += body_special(b['special'])
        else:
            out += body_simple(b)
        out.append("        }")
    return '\n'.join(out)

if __name__ == '__main__':
    print(gen_switch())
