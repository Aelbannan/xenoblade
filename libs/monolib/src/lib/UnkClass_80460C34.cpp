// zlib 1.2.3 inflate -- see UnkClass_80460C34.hpp for layout/rationale.
#include "monolib/lib/UnkClass_80460C34.hpp"

extern "C" void* memcpy(void* dest, const void* src, unsigned long n);

// ---------------------------------------------------------------------------
// inflateReset  (retail func_80460C34__17UnkClass_80460C34Fv, 0x80)
// ---------------------------------------------------------------------------
extern "C" int func_80460C34__17UnkClass_80460C34Fv(z_stream* strm) {
    z_inflate_state* state;

    if (strm == 0 || strm->state == 0) return Z_STREAM_ERROR;
    state = strm->state;
    strm->total_in = strm->total_out = state->total = 0;
    strm->msg = 0;
    strm->adler = 1;
    state->mode = HEAD;
    state->last = 0;
    state->havedict = 0;
    state->dmax = 32768U;
    state->head = 0;
    state->wsize = 0;
    state->whave = 0;
    state->wnext = 0;
    state->hold = 0;
    state->bits = 0;
    state->lencode = state->distcode = state->next = state->codes;
    return Z_OK;
}

// ---------------------------------------------------------------------------
// inflateInit2_  (retail func_80460CB4__17UnkClass_80460C34Fv, 0x118)
// ---------------------------------------------------------------------------
extern "C" int func_80460CB4__17UnkClass_80460C34Fv(
        z_stream* strm, int windowBits, const char* version, int stream_size) {
    z_inflate_state* state;

    if (version == 0 || version[0] != lbl_eu_80523AF0[0] ||
        stream_size != (int)sizeof(z_stream))
        return Z_VERSION_ERROR;
    if (strm == 0) return Z_STREAM_ERROR;
    strm->msg = 0;
    state = (z_inflate_state*)((z_alloc_func)strm->zalloc)(
        strm->opaque, 1, sizeof(z_inflate_state));
    if (state == 0) return Z_MEM_ERROR;
    strm->state = state;
    if (windowBits < 0) {
        state->wrap = 0;
        windowBits = -windowBits;
    }
    else {
        state->wrap = (windowBits >> 4) + 1;
    }
    if ((z_uint)(windowBits - 8) > 7U) {
        ((z_free_func)strm->zfree)(strm->opaque, state);
        strm->state = 0;
        return Z_STREAM_ERROR;
    }
    state->wbits = (z_uint)windowBits;
    state->window = 0;
    return func_80460C34__17UnkClass_80460C34Fv(strm);
}

// ---------------------------------------------------------------------------
// inflateInit_  (retail func_80460DCC__17UnkClass_80460C34Fv, 0x10)
// ---------------------------------------------------------------------------
extern "C" int func_80460DCC__17UnkClass_80460C34Fv(
        z_stream* strm, const char* version, int stream_size) {
    return func_80460CB4__17UnkClass_80460C34Fv(strm, 15, version, stream_size);
}

// ---------------------------------------------------------------------------
// updatewindow  (retail local func_80460DDC__17UnkClass_80460C34Fv, 0x17C)
// ---------------------------------------------------------------------------
extern "C" int func_80460DDC__17UnkClass_80460C34Fv(z_stream* strm, z_uint out) {
    z_inflate_state* state;
    z_uint copy, dist;

    state = strm->state;
    if (state->window == 0) {
        state->window = (z_uchar*)((z_alloc_func)strm->zalloc)(
            strm->opaque, 1U << state->wbits, sizeof(z_uchar));
        if (state->window == 0) return 1;
    }
    if (state->wsize == 0) {
        state->wsize = 1U << state->wbits;
        state->wnext = 0;
        state->whave = 0;
    }
    copy = out - strm->avail_out;
    if (copy >= state->wsize) {
        memcpy(state->window, strm->next_out - state->wsize, state->wsize);
        state->wnext = 0;
        state->whave = state->wsize;
    }
    else {
        dist = state->wsize - state->wnext;
        if (dist > copy) dist = copy;
        memcpy(state->window + state->wnext, strm->next_out - copy, dist);
        copy -= dist;
        if (copy) {
            memcpy(state->window, strm->next_out - copy, copy);
            state->wnext = copy;
            state->whave = state->wsize;
        }
        else {
            state->wnext += dist;
            if (state->wnext == state->wsize) state->wnext = 0;
            if (state->whave < state->wsize) state->whave += dist;
        }
    }
    return 0;
}

// ---------------------------------------------------------------------------
// inflateEnd  (retail func_80461FE0__17UnkClass_80460C34Fv, 0x88)
// ---------------------------------------------------------------------------
extern "C" int func_80461FE0__17UnkClass_80460C34Fv(z_stream* strm) {
    if (strm == 0 || strm->state == 0 || strm->zfree == 0)
        return Z_STREAM_ERROR;
    if (strm->state->window != 0)
        ((z_free_func)strm->zfree)(strm->opaque, strm->state->window);
    ((z_free_func)strm->zfree)(strm->opaque, strm->state);
    strm->state = 0;
    return Z_OK;
}

// ---------------------------------------------------------------------------
// inflate_table  (retail func_80462068__17UnkClass_80460C34Fv, 0x770)
// zlib 1.2.3 inftrees.c
// ---------------------------------------------------------------------------
#define Z_MAXBITS 15

typedef enum { Z_CODES, Z_LENS, Z_DISTS } z_codetype;

extern "C" int func_80462068__17UnkClass_80460C34Fv(
        z_codetype type, z_ushort* lens, z_uint codes,
        z_code** table, z_uint* bits, z_ushort* work) {
    z_uint len, sym, min, max, root, curr, drop;
    int left;
    z_uint used, huff, incr, fill, low, mask;
    z_code here;
    z_code* next;
    const z_ushort* base;
    const z_ushort* extra;
    int end;
    z_ushort count[Z_MAXBITS + 1];
    z_ushort offs[Z_MAXBITS + 1];

    for (len = 0; len <= Z_MAXBITS; len++)
        count[len] = 0;
    for (sym = 0; sym < codes; sym++)
        count[lens[sym]]++;

    for (max = Z_MAXBITS; max >= 1; max--)
        if (count[max] != 0) break;
    root = *bits;
    if (root > max) root = max;
    if (max == 0) {
        here.op = 64;
        here.bits = 1;
        here.val = 0;
        *(*table)++ = here;
        *(*table)++ = here;
        *bits = 1;
        return 0;
    }
    for (min = 1; min <= Z_MAXBITS; min++)
        if (count[min] != 0) break;
    if (root < min) root = min;

    left = 1;
    for (len = 1; len <= Z_MAXBITS; len++) {
        left <<= 1;
        left -= count[len];
        if (left < 0) return -1;
    }
    if (left > 0 && (type == Z_CODES || max != 1))
        return -1;

    offs[1] = 0;
    for (len = 1; len < Z_MAXBITS; len++)
        offs[len + 1] = offs[len] + count[len];

    for (sym = 0; sym < codes; sym++)
        if (lens[sym] != 0) work[offs[lens[sym]]++] = (z_ushort)sym;

    switch (type) {
    case Z_CODES:
        base = extra = work;
        end = 19;
        break;
    case Z_LENS:
        base = lbl_eu_80523C60 - 257;
        extra = lbl_eu_80523CA0 - 257;
        end = 256;
        break;
    default:
        base = lbl_eu_80523CE0;
        extra = lbl_eu_80523D20;
        end = -1;
    }

    next = *table;
    used = 1U << root;
    curr = root;
    drop = 0;
    low = (z_uint)-1;
    mask = (1U << root) - 1;
    huff = 0;
    len = min;
    for (;;) {
        here.bits = (z_uchar)(len - drop);
        if ((int)work[sym] < end) {
            here.op = 0;
            here.val = work[sym];
        }
        else if ((int)work[sym] > end) {
            here.op = (z_uchar)extra[work[sym]];
            here.val = base[work[sym]];
        }
        else {
            here.op = 96;
            here.val = 0;
        }

        incr = 1U << (len - drop);
        fill = 1U << curr;
        min = fill;
        do {
            fill -= incr;
            next[(huff >> drop) + fill] = here;
        } while (fill != 0);

        incr = 1U << (len - 1);
        while (huff & incr)
            incr >>= 1;
        if (incr != 0) {
            huff &= incr - 1;
            huff += incr;
        }
        else
            huff = 0;

        sym++;
        if (--count[len] == 0) {
            if (len == max) break;
            len = lens[work[sym]];
        }

        if (len > root && (huff & mask) != low) {
            if (drop == 0)
                drop = root;
            next += min;
            curr = len - drop;
            left = (int)(1 << curr);
            while (curr + drop < max) {
                left -= count[curr + drop];
                if (left <= 0) break;
                curr++;
                left <<= 1;
            }
            used += 1U << curr;
            if (type == Z_LENS && used >= 0x5b0)
                return 1;
            low = huff & mask;
            (*table)[low].op = (z_uchar)curr;
            (*table)[low].bits = (z_uchar)root;
            (*table)[low].val = (z_ushort)(next - *table);
        }
    }

    here.op = 64;
    here.bits = (z_uchar)(len - drop);
    here.val = 0;
    while (huff != 0) {
        if (drop != 0 && (huff & mask) != low) {
            drop = 0;
            len = root;
            next = *table;
            here.bits = (z_uchar)len;
        }
        next[huff >> drop] = here;
        incr = 1U << (len - 1);
        while (huff & incr)
            incr >>= 1;
        if (incr != 0) {
            huff &= incr - 1;
            huff += incr;
        }
        else
            huff = 0;
    }

    *table += used;
    *bits = root;
    return 0;
}
