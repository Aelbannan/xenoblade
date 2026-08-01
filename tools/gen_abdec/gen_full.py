#!/usr/bin/env python3
"""Generate the complete high-level C reconstruction of the three
mpvabdec_*_Isr functions from extracted per-body parameters."""
import sys

# ---------------------------------------------------------------- params ---
IB = {}
NIB = {}
exec(open('/tmp/body_ib.py').read(), IB)
exec(open('/tmp/body_nib.py').read(), NIB)
IB = IB['BODIES']
NIB = NIB['BODIES']

# special bodies (idx -> kind), same for both functions
SPECIALS = {
    0: 'escape_long',
    1: 'huff13',
    2: 'huff11',
    3: 'huff11',
    4: 'escape', 5: 'escape', 6: 'escape', 7: 'escape',
    32: 'huff990',
}
for i in range(33, 40):
    SPECIALS[i] = 'huff990'
for i in range(128, 192):
    SPECIALS[i] = 'eob'

# function-specific special body offsets
IB_SPECIAL_OFF = {0: 0x2B34, 1: 0x2A70, 2: 0x29AC, 4: 0x28C0, 32: 0x202C, 128: 0x305C}
NIB_SPECIAL_OFF = {0: 0x2F18, 1: 0x2E50, 2: 0x2D88, 4: 0x2C98, 32: 0x23C0, 128: 0x346C}

# -------------------------------------------------------------- helpers ----
def case_labels(idxs):
    return ' '.join(f'case {v}:' for v in idxs)


def coeff_lines(cvar, off, mult, neg, indent, ctxname):
    """Emit the coefficient compute/store block reading p[off]."""
    L = []
    L.append(f"{indent}s32 {cvar} = (s8)p[{off}];")
    L.append(f"{indent}blk->last = {cvar};")
    L.append(f"{indent}s32 s{cvar} = (({mult} * (s32)blk->quant) * (s32)blk->clip[{cvar}]) >> 4;")
    if neg:
        L.append(f"{indent}s{cvar} = -((s{cvar} - 1) | 1);")
    else:
        L.append(f"{indent}s{cvar} = (s{cvar} - 1) | 1;")
    L.append(f"{indent}blk->block[{cvar}] = (s16)((s{cvar} * (s32){ctxname}->tblqt[{cvar}] + 0x400) >> 11);")
    return L


def refill(bits, indent):
    if bits == 'codelen':
        return [
            f"{indent}bc += (s32)blk->codelen;",
            f"{indent}if (bc >= 0x20) {{",
            f"{indent}    bc -= 0x20;",
            f"{indent}    hi = lo << bc;",
            f"{indent}    lo = *ptr++;",
            f"{indent}    }} else {{",
            f"{indent}    hi <<= (s32)blk->codelen;",
            f"{indent}    }}",
        ]
    return [
        f"{indent}bc += {bits};",
        f"{indent}if (bc >= 0x20) {{",
        f"{indent}    bc -= 0x20;",
        f"{indent}    hi = lo << bc;",
        f"{indent}    lo = *ptr++;",
        f"{indent}    }} else {{",
        f"{indent}    hi <<= {bits};",
        f"{indent}    }}",
    ]


def common_tail(indent, ctxname, terminator):
    """The shared post-decode coefficient computation."""
    L = [
        f"{indent}p += blk->f00;",
        f"{indent}s32 c = (s8)*++p;",
        f"{indent}blk->last = c;",
        f"{indent}s32 s = ((2 * (s32)blk->f04 * (s32)blk->quant) * (s32)blk->clip[c]) >> 4;",
        f"{indent}if (blk->f08 != 0) {{",
        f"{indent}    s = -((s - 1) | 1);",
        f"{indent}    }} else {{",
        f"{indent}    s = (s - 1) | 1;",
        f"{indent}    }}",
        f"{indent}blk->block[c] = (s16)((s * (s32){ctxname}->tblqt[c] + 0x400) >> 11);",
        f"{indent}{terminator}",
    ]
    return L


def body_special(kind, idxs, ctxname, indent, exit_term):
    L = []
    if kind == 'eob':
        L.append(f"{indent}bc += 2;")
        L.append(f"{indent}if (bc >= 0x20) {{")
        L.append(f"{indent}    bc -= 0x20;")
        L.append(f"{indent}    hi = lo << bc;")
        L.append(f"{indent}    lo = *ptr++;")
        L.append(f"{indent}    }} else {{")
        L.append(f"{indent}    hi <<= 2;")
        L.append(f"{indent}    }}")
        L.append(f"{indent}{exit_term}")
        return L
    if kind == 'huff11':
        L += [
            f"{indent}blk->codelen = 0x0B;",
            f"{indent}u32 e = (bits >> 21) & 0x3FF;",
            f"{indent}s16 t = {ctxname}->tbl994[(e & ~1u)];",
            f"{indent}blk->f00 = (u32)(t & 0xFF);",
            f"{indent}blk->f04 = (s32)(s8)((t >> 8) & 0xFF);",
            f"{indent}blk->f08 = e & 1;",
        ]
        L += refill(0x0B, indent)
        L += common_tail(indent, ctxname, 'continue;')
        return L
    if kind == 'huff13':
        L += [
            f"{indent}blk->codelen = 0x0D;",
            f"{indent}u32 e = (bits >> 19) & 0xFFF;",
            f"{indent}s16 t = {ctxname}->tbl998[(e & ~1u)];",
            f"{indent}blk->f00 = (u32)(t & 0xFF);",
            f"{indent}blk->f04 = (s32)(s8)((t >> 8) & 0xFF);",
            f"{indent}blk->f08 = e & 1;",
        ]
        L += refill(0x0D, indent)
        L += common_tail(indent, ctxname, 'continue;')
        return L
    if kind == 'escape_long':
        L += [
            f"{indent}u32 x = bits << 1;",
            f"{indent}s16 t;",
            f"{indent}u32 e;",
            f"{indent}if ((x >> 24) != 0) {{",
            f"{indent}    blk->codelen = 0x0E;",
            f"{indent}    e = x >> 19;",
            f"{indent}    t = {ctxname}->esc1[(e & ~1u)];",
            f"{indent}    }} else {{",
            f"{indent}    x <<= 8;",
            f"{indent}    if ((s32)x < 0) {{",
            f"{indent}        blk->codelen = 0x0F;",
            f"{indent}        e = (x >> 26) & 0x1F;",
            f"{indent}        t = {ctxname}->esc2[(e & ~1u)];",
            f"{indent}        }} else {{",
            f"{indent}        x <<= 1;",
            f"{indent}        if ((s32)x < 0) {{",
            f"{indent}            blk->codelen = 0x10;",
            f"{indent}            e = (x >> 26) & 0x1F;",
            f"{indent}            t = {ctxname}->esc3[(e & ~1u)];",
            f"{indent}            }} else {{",
            f"{indent}            blk->codelen = 0x11;",
            f"{indent}            e = (x >> 25) & 0x1F;",
            f"{indent}            t = {ctxname}->esc4[(e & ~1u)];",
            f"{indent}            }}",
            f"{indent}        }}",
            f"{indent}    }}",
            f"{indent}blk->f00 = (u32)(t & 0xFF);",
            f"{indent}blk->f04 = (s32)(s8)((t >> 8) & 0xFF);",
            f"{indent}blk->f08 = x & 1;",
        ]
        L += refill('codelen', indent)
        L += common_tail(indent, ctxname, 'continue;')
        return L
    if kind == 'escape':
        L += [
            f"{indent}u32 x = bits << 1;",
            f"{indent}blk->codelen = 0x14;",
            f"{indent}s32 v = (s32)((x >> 11) & 0xFFFF) >> 2;",
            f"{indent}blk->f00 = (u32)(s8)(v >> 8);",
            f"{indent}if (((x >> 13) & 0x7F) != 0) {{",
            f"{indent}    }} else {{",
            f"{indent}    blk->codelen = 0x1C;",
            f"{indent}    v = ((s32)(s8)((x >> 13) & 0xFF) << 1) | (s32)((x >> 5) & 0xFF);",
            f"{indent}    }}",
            f"{indent}if (v < 0) {{",
            f"{indent}    blk->f08 = 1;",
            f"{indent}    v = -v;",
            f"{indent}    }} else {{",
            f"{indent}    blk->f08 = 0;",
            f"{indent}    }}",
            f"{indent}blk->f04 = (s32)v;",
        ]
        L += refill('codelen', indent)
        L += common_tail(indent, ctxname, 'continue;')
        return L
    if kind == 'huff990':
        L += [
            f"{indent}u32 x = bits << 1;",
            f"{indent}u32 t = {ctxname}->tbl990[(x >> 25) & 0x7F];",
            f"{indent}blk->f00 = t & 0xFF;",
            f"{indent}if (blk->f00 == 0x40) {{",
            f"{indent}    blk->codelen = 0x14;",
            f"{indent}    s32 v = (s32)((x >> 11) & 0xFFFF) >> 2;",
            f"{indent}    blk->f00 = (u32)(s8)(v >> 8);",
            f"{indent}    if (((x >> 13) & 0x7F) != 0) {{",
            f"{indent}        }} else {{",
            f"{indent}        blk->codelen = 0x1C;",
            f"{indent}        v = ((s32)(s8)((x >> 13) & 0xFF) << 1) | (s32)((x >> 5) & 0xFF);",
            f"{indent}        }}",
            f"{indent}    if (v < 0) {{",
            f"{indent}        blk->f08 = 1;",
            f"{indent}        v = -v;",
            f"{indent}        }} else {{",
            f"{indent}        blk->f08 = 0;",
            f"{indent}        }}",
            f"{indent}    blk->f04 = (s32)v;",
            f"{indent}    }} else {{",
            f"{indent}    blk->codelen = t >> 16;",
            f"{indent}    blk->f04 = (s32)(s8)((t >> 8) & 0xFF);",
            f"{indent}    x >>= 0x21 - (s32)blk->codelen;",
            f"{indent}    blk->f08 = x & 1;",
            f"{indent}    }}",
        ]
        L += refill('codelen', indent)
        L += common_tail(indent, ctxname, 'continue;')
        return L
    raise ValueError(kind)


def gen_bodies(BODIES, ctxname, loop_target, exit_target, is_nib):
    """Emit the switch cases in retail layout order."""
    out = []
    for boff in sorted(BODIES):
        b = BODIES[boff]
        idxs = b['idx']
        # exit vs continue: exit target == exit_target
        exit_body = b.get('exit') == 'fall' or (
            isinstance(b.get('exit'), tuple) and b['exit'][1] == exit_target)
        if exit_body:
            term = 'goto exit;'
        else:
            term = 'continue;'
        sp = None
        for i in idxs:
            if i in SPECIALS:
                sp = SPECIALS[i]
                break
        out.append(f"        {case_labels(idxs)} {{")
        if sp:
            out += body_special(sp, idxs, ctxname, '            ', term)
        else:
            mult = b['mult']
            if isinstance(mult, tuple):   # ('mulli', 6)
                mult = mult[1]
            if b['coeffs'] == 2:
                neg1 = b['neg'] >= 1
                neg2 = b['neg'] >= 2
                out += coeff_lines('c1', b['off'], mult, neg1, '            ', ctxname)
                out.append(f"            p += {b['off'] + 1};")
                out += coeff_lines('c2', 0, mult, neg2, '            ', ctxname)
            else:
                out.append(f"            p += {b['off']};")
                out += coeff_lines('c1', 0, mult, b['neg'] >= 1, '            ', ctxname)
            out += refill(b['bits'], '            ')
            out.append(f"            {term}")
        out.append("        }")
    return '\n'.join(out)


# ---------------------------------------------------------------- funcs ----
def gen_intra(BODIES, ctxname, fname, dc11):
    out = []
    out.append(f"s32 {fname}(MPVABDEC_CTX *ctx, MPVABDEC_BLK *blk) {{")
    out.append("    s32 bc = ctx->bc;")
    out.append("    u32 hi = ctx->hi;")
    out.append("    u32 lo = ctx->lo;")
    out.append("    u32 *ptr = ctx->ptr;")
    if not dc11:
        out.append("    u32 t = hi >> 16;")
        out.append("    if (bc > 0x10) t |= lo >> (0x30 - bc);")
        out.append("    u8 v = blk->dctbl[t >> 9];")
        out.append("    s32 size = v >> 4;")
        out.append("    u32 low = v & 0xF;")
        out.append("    s32 dc = 0;")
        out.append("    if (size != 0) {")
        out.append("        s16 *masktbl = ctx->masktbl;")
        out.append("        u32 mask = (u32)masktbl[low];")
        out.append("        low += size;")
        out.append("        u32 one = 1u << (size - 1);")
        out.append("        t &= mask;")
        out.append("        t >>= 16 - low;")
        out.append("        if ((t & one) == 0) t += 1 - (one << 1);")
        out.append("        dc = (s32)(t << 3);")
        out.append("    }")
        out.append("    bc += low;")
        out.append("    if (bc >= 0x20) {")
        out.append("        bc -= 0x20;")
        out.append("        hi = lo << bc;")
        out.append("        lo = *ptr++;")
        out.append("        } else {")
        out.append("        hi <<= low;")
        out.append("    }")
        out.append("    s32 *sum = blk->sum;")
        out.append("    dc += *sum;")
        out.append("    *sum = dc;")
        out.append("    blk->block[0] = (s16)(dc << 3);")
        out.append("    blk->first = 0;")
        out.append("    blk->last = 0;")
        out.append("    u8 *p = ctx->p9ac;")
        out.append("")
        out.append("    for (;;) {")
        out.append("        u32 bits = hi;")
        out.append("        if (bc != 0) bits |= lo >> (32 - bc);")
        out.append("        u32 idx = bits >> 24;")
        out.append("        if (idx > 0xFF) goto exit;")
        out.append("        switch (idx) {")
        out.append(gen_bodies(BODIES, ctxname, 0x100, 0x3608, False))
        out.append("        }")
        out.append("    }")
        out.append("exit:")
    else:
        out.append("    u32 bits = hi;")
        out.append("    if (bc != 0) bits |= lo >> (32 - bc);")
        out.append("    u8 v = blk->dctbl[bits >> 22];")
        out.append("    s32 size = v >> 4;")
        out.append("    u32 low = v & 0xF;")
        out.append("    s32 dc = 0;")
        out.append("    if (size != 0) {")
        out.append("        u32 x = bits << low;")
        out.append("        low += size;")
        out.append("        x = ((s32)x >> 1) ^ 0x80000000u;")
        out.append("        dc = (s32)((x >> 31) + ((s32)x >> (31 - size)));")
        out.append("    }")
        out.append("    bc += low;")
        out.append("    if (bc >= 0x20) {")
        out.append("        bc -= 0x20;")
        out.append("        hi = lo << bc;")
        out.append("        lo = *ptr++;")
        out.append("        } else {")
        out.append("        hi <<= low;")
        out.append("    }")
        out.append("    s32 *sum = blk->sum;")
        out.append("    dc += *sum;")
        out.append("    *sum = dc;")
        out.append("    blk->block[0] = (s16)(dc << 3);")
        out.append("    blk->first = 0;")
        out.append("    blk->last = 0;")
        out.append("    u8 *p = ctx->p9ac;")
        out.append("")
        out.append("    for (;;) {")
        out.append("        u32 bits = hi;")
        out.append("        if (bc != 0) bits |= lo >> (32 - bc);")
        out.append("        u32 idx = bits >> 24;")
        out.append("        if (idx > 0xFF) goto exit;")
        out.append("        switch (idx) {")
        out.append(gen_bodies(BODIES, ctxname, 0x100, 0x3608, False))
        out.append("        }")
        out.append("    }")
        out.append("exit:")
    out.append("    if (blk->last == blk->first) blk->last = -blk->last;")
    out.append("    ctx->hi = hi;")
    out.append("    ctx->lo = lo;")
    out.append("    ctx->bc = bc;")
    out.append("    ctx->ptr = ptr;")
    out.append("    return blk->last;")
    out.append("}")
    return '\n'.join(out)


def gen_nintra(BODIES, ctxname, fname):
    out = []
    out.append(f"s32 {fname}(MPVABDEC_CTX *ctx, MPVABDEC_BLK *blk) {{")
    out.append("    u32 *b0 = (u32 *)blk->block;")
    out.append("    b0[0] = 0; b0[1] = 0; b0[2] = 0; b0[3] = 0;")
    out.append("    b0[4] = 0; b0[5] = 0; b0[6] = 0; b0[7] = 0;")
    out.append("    b0[8] = 0; b0[9] = 0; b0[10] = 0; b0[11] = 0;")
    out.append("    b0[12] = 0; b0[13] = 0; b0[14] = 0; b0[15] = 0;")
    out.append("    s32 bc = ctx->bc;")
    out.append("    u32 hi = ctx->hi;")
    out.append("    u32 lo = ctx->lo;")
    out.append("    u32 *ptr = ctx->ptr;")
    out.append("    u32 bits = hi;")
    out.append("    if (bc != 0) bits |= lo >> (32 - bc);")
    out.append("    if ((s32)bits < 0) {")
    out.append("        blk->f08 = (bits >> 30) & 1;")
    out.append("        blk->f04 = 1;")
    out.append("        blk->f00 = 0;")
    out.append("        blk->codelen = 2;")
    out.append("    } else {")
    out.append("        u32 x = bits << 1;")
    out.append("        u32 v = x >> 24;")
    out.append("        if ((v - 4) <= 3) {")
    out.append("            blk->codelen = 0x0B;")
    out.append("            x >>= 22;")
    out.append("            s16 t = ctx->tbl994[(x & ~1u)];")
    out.append("            blk->f00 = (u32)(t & 0xFF);")
    out.append("            blk->f04 = (s32)(s8)((t >> 8) & 0xFF);")
    out.append("            blk->f08 = x & 1;")
    out.append("        } else if ((v - 2) <= 1) {")
    out.append("            blk->codelen = 0x0D;")
    out.append("            x >>= 20;")
    out.append("            s16 t = ctx->tbl998[(x & ~1u)];")
    out.append("            blk->f00 = (u32)(t & 0xFF);")
    out.append("            blk->f04 = (s32)(s8)((t >> 8) & 0xFF);")
    out.append("            blk->f08 = x & 1;")
    out.append("        } else if (v == 1) {")
    out.append("            blk->codelen = 0x0E;")
    out.append("            x >>= 19;")
    out.append("            s16 t = ctx->esc1[(x & ~1u)];")
    out.append("            blk->f00 = (u32)(t & 0xFF);")
    out.append("            blk->f04 = (s32)(s8)((t >> 8) & 0xFF);")
    out.append("            blk->f08 = x & 1;")
    out.append("        } else if (v == 0) {")
    out.append("            x <<= 8;")
    out.append("            if ((s32)x < 0) {")
    out.append("                blk->codelen = 0x0F;")
    out.append("                x = (x >> 1) & 0x1F;")
    out.append("                s16 t = ctx->esc2[(x & ~1u)];")
    out.append("                blk->f00 = (u32)(t & 0xFF);")
    out.append("                blk->f04 = (s32)(s8)((t >> 8) & 0xFF);")
    out.append("                blk->f08 = x & 1;")
    out.append("            } else {")
    out.append("                x <<= 1;")
    out.append("                if ((s32)x < 0) {")
    out.append("                    blk->codelen = 0x10;")
    out.append("                    x = (x >> 1) & 0x1F;")
    out.append("                    s16 t = ctx->esc3[(x & ~1u)];")
    out.append("                    blk->f00 = (u32)(t & 0xFF);")
    out.append("                    blk->f04 = (s32)(s8)((t >> 8) & 0xFF);")
    out.append("                    blk->f08 = x & 1;")
    out.append("                } else {")
    out.append("                    blk->codelen = 0x11;")
    out.append("                    x = (x >> 2) & 0x1F;")
    out.append("                    s16 t = ctx->esc4[(x & ~1u)];")
    out.append("                    blk->f00 = (u32)(t & 0xFF);")
    out.append("                    blk->f04 = (s32)(s8)((t >> 8) & 0xFF);")
    out.append("                    blk->f08 = x & 1;")
    out.append("                }")
    out.append("            }")
    out.append("        } else {")
    out.append("            u32 t = ctx->tbl990[(v << 1) >> 2];")
    out.append("            blk->f00 = t & 0xFF;")
    out.append("            if (blk->f00 == 0x40) {")
    out.append("                blk->codelen = 0x14;")
    out.append("                s32 w = (s32)((x >> 11) & 0xFFFF) >> 2;")
    out.append("                blk->f00 = (u32)(s8)(w >> 8);")
    out.append("                if (((x >> 13) & 0x7F) != 0) {")
    out.append("                } else {")
    out.append("                    blk->codelen = 0x1C;")
    out.append("                    w = ((s32)(s8)((x >> 13) & 0xFF) << 1) | (s32)((x >> 5) & 0xFF);")
    out.append("                }")
    out.append("                if (w < 0) {")
    out.append("                    blk->f08 = 1;")
    out.append("                    w = -w;")
    out.append("                } else {")
    out.append("                    blk->f08 = 0;")
    out.append("                }")
    out.append("                blk->f04 = (s32)w;")
    out.append("            } else {")
    out.append("                blk->codelen = t >> 16;")
    out.append("                blk->f04 = (s32)(s8)((t >> 8) & 0xFF);")
    out.append("                x >>= 0x21 - (s32)blk->codelen;")
    out.append("                blk->f08 = x & 1;")
    out.append("            }")
    out.append("        }")
    out.append("    }")
    out.append("    bc += (s32)blk->codelen;")
    out.append("    if (bc >= 0x20) {")
    out.append("        bc -= 0x20;")
    out.append("        hi = lo << bc;")
    out.append("        lo = *ptr++;")
    out.append("        } else {")
    out.append("        hi <<= (s32)blk->codelen;")
    out.append("    }")
    out.append("    s32 *sum = blk->sum;")
    out.append("    u8 *p = ctx->p9ac + blk->f00;")
    out.append("    s32 c = (s8)p[0];")
    out.append("    blk->first = c;")
    out.append("    blk->last = c;")
    out.append("    s32 s = ((2 * (s32)blk->f04 + 1) * (s32)blk->quant * (s32)blk->clip[c]) >> 4;")
    out.append("    if (blk->f08 != 0) s = -((s - 1) | 1);")
    out.append("    blk->block[c] = (s16)((s * (s32)ctx->tblqt[c] + 0x400) >> 11);")
    out.append("")
    out.append("    for (;;) {")
    out.append("        u32 bits = hi;")
    out.append("        if (bc != 0) bits |= lo >> (32 - bc);")
    out.append("        u32 idx = bits >> 24;")
    out.append("        if (idx > 0xFF) goto exit;")
    out.append("        switch (idx) {")
    out.append(gen_bodies(BODIES, ctxname, 0x35C, 0x3A48, True))
    out.append("        }")
    out.append("    }")
    out.append("exit:")
    out.append("    if (blk->last == blk->first) blk->last = -blk->last;")
    out.append("    ctx->hi = hi;")
    out.append("    ctx->lo = lo;")
    out.append("    ctx->bc = bc;")
    out.append("    ctx->ptr = ptr;")
    out.append("    return blk->last;")
    out.append("}")
    return '\n'.join(out)


if __name__ == '__main__':
    which = sys.argv[1] if len(sys.argv) > 1 else 'ib'
    if which == 'ib':
        print(gen_intra(IB, 'ctx', 'mpvabdec_IntraBlock_Isr', False))
    elif which == 'ib11':
        print(gen_intra(IB, 'ctx', 'mpvabdec_IntraBlockDc11_Isr', True))
    elif which == 'nib':
        print(gen_nintra(NIB, 'ctx', 'mpvabdec_NintraBlock_Isr'))
