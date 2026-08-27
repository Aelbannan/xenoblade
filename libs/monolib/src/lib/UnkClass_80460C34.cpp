// zlib 1.2.3 inflate -- see UnkClass_80460C34.hpp for layout/rationale.
#include "monolib/lib/UnkClass_80460C34.hpp"

// ---------------------------------------------------------------------------
// inflateReset  (retail func_80460C34__17UnkClass_80460C34Fv, 0x80)
// ---------------------------------------------------------------------------
extern "C" int inflateReset__17UnkClass_80460C34Fv(z_stream* strm) {
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
    return inflateReset__17UnkClass_80460C34Fv(strm);
}

// ---------------------------------------------------------------------------
// inflateInit_  (retail func_80460DCC__17UnkClass_80460C34Fv, 0x10)
// ---------------------------------------------------------------------------
extern "C" int inflateInit___17UnkClass_80460C34Fv(
        z_stream* strm, const char* version, int stream_size) {
    return func_80460CB4__17UnkClass_80460C34Fv(strm, 15, version, stream_size);
}

// ---------------------------------------------------------------------------
// updatewindow  (retail local func_80460DDC__17UnkClass_80460C34Fv, 0x17C)
// ---------------------------------------------------------------------------
extern "C" int updateWindow__17UnkClass_80460C34Fv(z_stream* strm, z_uint out) {
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
extern "C" int inflateEnd__17UnkClass_80460C34Fv(z_stream* strm) {
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
    z_ushort count[Z_MAXBITS + 1];
    z_ushort offs[Z_MAXBITS + 1];
    z_uint len;
    z_uint sym;
    z_uint min;
    z_uint max;
    z_uint root;
    z_uint curr;
    z_uint drop;
    int left;
    z_uint used;
    z_uint huff;
    z_uint incr;
    z_uint fill;
    z_uint low;
    z_uint mask;
    z_code here;
    z_code* next;
    z_ushort* extra;
    const z_ushort* base;
    int end;

    for (len = 0; len <= Z_MAXBITS; len++)
        count[len] = 0;
    for (sym = 0; sym < codes; sym++)
        count[lens[sym]]++;

    root = *bits;
    for (max = Z_MAXBITS; max >= 1; max--)
        if (count[max] != 0) break;
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
        extra = work;
        base = (const z_ushort*)work;
        end = 19;
        break;
    case Z_LENS:
        base = (const z_ushort*)lbl_eu_80523C60 - 257;
        extra = (z_ushort*)lbl_eu_80523CA0 - 257;
        end = 256;
        break;
    default:
        base = (const z_ushort*)lbl_eu_80523CE0;
        extra = (z_ushort*)lbl_eu_80523D20;
        end = -1;
    }

    huff = 0;
    sym = 0;
    len = min;
    next = *table;
    curr = root;
    drop = 0;
    low = (z_uint)-1;
    used = 1U << root;
    mask = used - 1;
    if (type == Z_LENS && used >= 0x5b0)
        return 1;
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

// ---------------------------------------------------------------------------
// inflate  (retail func_80460F58__17UnkClass_80460C34Fv, 0x1088)
// zlib 1.2.3 inflate.c, with the retail build's non-gzip compact state set.
// ---------------------------------------------------------------------------
#define ZI_LOAD() do { \
    put = strm->next_out; \
    left = strm->avail_out; \
    next = strm->next_in; \
    have = strm->avail_in; \
    hold = state->hold; \
    bit_count = state->bits; \
} while (0)
#define ZI_RESTORE() do { \
    strm->next_out = put; \
    strm->avail_out = left; \
    strm->next_in = next; \
    strm->avail_in = have; \
    state->hold = hold; \
    state->bits = bit_count; \
} while (0)
#define ZI_INITBITS() do { hold = 0; bit_count = 0; } while (0)
#define ZI_PULLBYTE() do { \
    if (have == 0) goto inflate_leave; \
    have--; \
    hold += (z_ulong)(*next++) << bit_count; \
    bit_count += 8; \
} while (0)
#define ZI_NEEDBITS(n) do { \
    while (bit_count < (z_uint)(n)) ZI_PULLBYTE(); \
} while (0)
#define ZI_BITS(n) ((z_uint)hold & ((1U << (n)) - 1))
#define ZI_DROPBITS(n) do { \
    hold >>= (n); \
    bit_count -= (z_uint)(n); \
} while (0)
#define ZI_BYTEBITS() do { \
    hold >>= bit_count & 7; \
    bit_count -= bit_count & 7; \
} while (0)
#define ZI_REVERSE(q) ((((q) >> 24) & 0xff) + (((q) >> 8) & 0xff00) + \
    (((q) & 0xff00) << 8) + (((q) & 0xff) << 24))

#define message_base lbl_eu_80523AF0
#define order lbl_eu_80523AC8
#define fixed_lencode lbl_80526B70
#define fixed_distcode lbl_eu_80523A48

extern "C" int func_80460F58__17UnkClass_80460C34Fv(
        z_stream* strm, int flush) {
    z_inflate_state* state;
    z_uchar* next;
    z_uchar* put;
    z_uint have, left;
    z_ulong hold;
    z_uint bit_count;
    z_uint in, out;
    z_uint copy;
    z_uchar* from;
    z_code current;
    z_code last_code;
    z_uint len;
    int ret;
    if (strm == 0 || strm->state == 0 || strm->next_out == 0 ||
        (strm->next_in == 0 && strm->avail_in != 0))
        return Z_STREAM_ERROR;

    state = strm->state;
    if (state->mode == TYPE) state->mode = TYPEDO;
    ZI_LOAD();
    in = have;
    out = left;
    ret = Z_OK;

    for (;;) {
        switch (state->mode) {
        case HEAD:
            if (state->wrap == 0) {
                state->mode = TYPEDO;
                break;
            }
            ZI_NEEDBITS(16);
            if ((((ZI_BITS(8) << 8) + (hold >> 8)) % 31) != 0) {
                strm->msg = (char*)(message_base + 6);
                state->mode = BAD;
                break;
            }
            if (ZI_BITS(4) != Z_DEFLATED) {
                strm->msg = (char*)(message_base + 0x1d);
                state->mode = BAD;
                break;
            }
            ZI_DROPBITS(4);
            len = ZI_BITS(4) + 8;
            if (len > state->wbits) {
                strm->msg = (char*)(message_base + 0x38);
                state->mode = BAD;
                break;
            }
            state->dmax = 1U << len;
            strm->adler = state->check =
                func_80460308__17UnkClass_80460308Fv(0, 0, 0);
            state->mode = (hold & 0x200) ? DICTID : TYPE;
            ZI_INITBITS();
            break;

        case DICTID:
            ZI_NEEDBITS(32);
            strm->adler = state->check = ZI_REVERSE(hold);
            ZI_INITBITS();
            state->mode = DICT;
            /* fall through */
        case DICT:
            if (state->havedict == 0) {
                ZI_RESTORE();
                return Z_NEED_DICT;
            }
            strm->adler = state->check =
                func_80460308__17UnkClass_80460308Fv(0, 0, 0);
            state->mode = TYPE;
            /* fall through */
        case TYPE:
            if (flush == Z_BLOCK) goto inflate_leave;
            /* fall through */
        case TYPEDO:
            if (state->last) {
                ZI_BYTEBITS();
                state->mode = CHECK;
                break;
            }
            ZI_NEEDBITS(3);
            state->last = ZI_BITS(1);
            ZI_DROPBITS(1);
            switch (ZI_BITS(2)) {
            case 0:
                state->mode = STORED;
                break;
            case 1:
                state->lencode = fixed_lencode;
                state->lenbits = 9;
                state->distcode = fixed_distcode;
                state->distbits = 5;
                state->mode = LEN;
                break;
            case 2:
                state->mode = TABLE;
                break;
            case 3:
                strm->msg = (char*)(message_base + 0x4c);
                state->mode = BAD;
                break;
            }
            ZI_DROPBITS(2);
            break;

        case STORED:
            ZI_BYTEBITS();
            ZI_NEEDBITS(32);
            if ((hold & 0xffff) != ((hold >> 16) ^ 0xffff)) {
                strm->msg = (char*)(message_base + 0x5f);
                state->mode = BAD;
                break;
            }
            state->length = (z_uint)hold & 0xffff;
            ZI_INITBITS();
            state->mode = COPY_;
            /* fall through */
        case COPY_:
            copy = state->length;
            if (copy) {
                if (copy > have) copy = have;
                if (copy > left) copy = left;
                if (copy == 0) goto inflate_leave;
                memcpy(put, next, copy);
                have -= copy;
                next += copy;
                left -= copy;
                put += copy;
                state->length -= copy;
                break;
            }
            state->mode = TYPE;
            break;

        case TABLE:
            ZI_NEEDBITS(14);
            state->nlen = ZI_BITS(5) + 257;
            ZI_DROPBITS(5);
            state->ndist = ZI_BITS(5) + 1;
            ZI_DROPBITS(5);
            state->ncode = ZI_BITS(4) + 4;
            ZI_DROPBITS(4);
            if (state->nlen > 286 || state->ndist > 30) {
                strm->msg = (char*)(message_base + 0x7c);
                state->mode = BAD;
                break;
            }
            state->have = 0;
            state->mode = LENLENS;
            /* fall through */
        case LENLENS:
            while (state->have < state->ncode) {
                ZI_NEEDBITS(3);
                state->lens[order[state->have++]] = (z_ushort)ZI_BITS(3);
                ZI_DROPBITS(3);
            }
            while (state->have < 19)
                state->lens[order[state->have++]] = 0;
            state->next = state->codes;
            state->lencode = state->next;
            state->lenbits = 7;
            ret = func_80462068__17UnkClass_80460C34Fv(
                Z_CODES, state->lens, 19, &state->next,
                &state->lenbits, state->work);
            if (ret) {
                strm->msg = (char*)(message_base + 0xa0);
                state->mode = BAD;
                break;
            }
            state->have = 0;
            state->mode = CODELENS;
            /* fall through */
        case CODELENS:
            while (state->have < state->nlen + state->ndist) {
                for (;;) {
                    current = state->lencode[ZI_BITS(state->lenbits)];
                    if ((z_uint)current.bits <= bit_count) break;
                    ZI_PULLBYTE();
                }
                if (current.val < 16) {
                    ZI_NEEDBITS(current.bits);
                    ZI_DROPBITS(current.bits);
                    state->lens[state->have++] = current.val;
                }
                else {
                    if (current.val == 16) {
                        ZI_NEEDBITS(current.bits + 2);
                        ZI_DROPBITS(current.bits);
                        if (state->have == 0) {
                            strm->msg = (char*)(message_base + 0xb9);
                            state->mode = BAD;
                            break;
                        }
                        len = state->lens[state->have - 1];
                        copy = 3 + ZI_BITS(2);
                        ZI_DROPBITS(2);
                    }
                    else if (current.val == 17) {
                        ZI_NEEDBITS(current.bits + 3);
                        ZI_DROPBITS(current.bits);
                        len = 0;
                        copy = 3 + ZI_BITS(3);
                        ZI_DROPBITS(3);
                    }
                    else {
                        ZI_NEEDBITS(current.bits + 7);
                        ZI_DROPBITS(current.bits);
                        len = 0;
                        copy = 11 + ZI_BITS(7);
                        ZI_DROPBITS(7);
                    }
                    if (state->have + copy > state->nlen + state->ndist) {
                        strm->msg = (char*)(message_base + 0xb9);
                        state->mode = BAD;
                        break;
                    }
                    while (copy--)
                        state->lens[state->have++] = (z_ushort)len;
                }
            }
            if (state->mode == BAD) break;
            state->next = state->codes;
            state->lencode = state->next;
            state->lenbits = 9;
            ret = func_80462068__17UnkClass_80460C34Fv(
                Z_LENS, state->lens, state->nlen, &state->next,
                &state->lenbits, state->work);
            if (ret) {
                strm->msg = (char*)(message_base + 0xd3);
                state->mode = BAD;
                break;
            }
            state->distcode = state->next;
            state->distbits = 6;
            ret = func_80462068__17UnkClass_80460C34Fv(
                Z_DISTS, state->lens + state->nlen, state->ndist,
                &state->next, &state->distbits, state->work);
            if (ret) {
                strm->msg = (char*)(message_base + 0xef);
                state->mode = BAD;
                break;
            }
            state->mode = LEN;
            /* fall through */
        case LEN:
            if (have >= 6 && left >= 258) {
                ZI_RESTORE();
                func_80460728__17UnkClass_80460308Fv(strm, out);
                ZI_LOAD();
                break;
            }
            for (;;) {
                current = state->lencode[ZI_BITS(state->lenbits)];
                if ((z_uint)current.bits <= bit_count) break;
                ZI_PULLBYTE();
            }
            if (current.op && (current.op & 0xf0) == 0) {
                last_code = current;
                for (;;) {
                    current = state->lencode[last_code.val +
                        (ZI_BITS(last_code.bits + last_code.op) >> last_code.bits)];
                    if ((z_uint)(last_code.bits + current.bits) <= bit_count)
                        break;
                    ZI_PULLBYTE();
                }
                ZI_DROPBITS(last_code.bits);
            }
            ZI_DROPBITS(current.bits);
            state->length = current.val;
            if ((int)current.op == 0) {
                state->mode = LIT;
                break;
            }
            if (current.op & 32) {
                state->mode = TYPE;
                break;
            }
            if (current.op & 64) {
                strm->msg = (char*)(message_base + 0x105);
                state->mode = BAD;
                break;
            }
            state->extra = (z_uint)current.op & 15;
            state->mode = LENEXT;
            /* fall through */
        case LENEXT:
            if (state->extra) {
                ZI_NEEDBITS(state->extra);
                state->length += ZI_BITS(state->extra);
                ZI_DROPBITS(state->extra);
            }
            state->mode = DIST;
            /* fall through */
        case DIST:
            for (;;) {
                current = state->distcode[ZI_BITS(state->distbits)];
                if ((z_uint)current.bits <= bit_count) break;
                ZI_PULLBYTE();
            }
            if ((current.op & 0xf0) == 0) {
                last_code = current;
                for (;;) {
                    current = state->distcode[last_code.val +
                        (ZI_BITS(last_code.bits + last_code.op) >> last_code.bits)];
                    if ((z_uint)(last_code.bits + current.bits) <= bit_count)
                        break;
                    ZI_PULLBYTE();
                }
                ZI_DROPBITS(last_code.bits);
            }
            ZI_DROPBITS(current.bits);
            if (current.op & 64) {
                strm->msg = (char*)(message_base + 0x121);
                state->mode = BAD;
                break;
            }
            state->offset = current.val;
            state->extra = (z_uint)current.op & 15;
            state->mode = DISTEXT;
            /* fall through */
        case DISTEXT:
            if (state->extra) {
                ZI_NEEDBITS(state->extra);
                state->offset += ZI_BITS(state->extra);
                ZI_DROPBITS(state->extra);
            }
            if (state->offset > state->whave + out - left) {
                strm->msg = (char*)(message_base + 0x137);
                state->mode = BAD;
                break;
            }
            state->mode = MATCH;
            /* fall through */
        case MATCH:
            if (left == 0) goto inflate_leave;
            copy = out - left;
            if (state->offset > copy) {
                copy = state->offset - copy;
                if (copy > state->wnext) {
                    copy -= state->wnext;
                    from = state->window + (state->wsize - copy);
                }
                else
                    from = state->window + (state->wnext - copy);
                if (copy > state->length) copy = state->length;
            }
            else {
                from = put - state->offset;
                copy = state->length;
            }
            if (copy > left) copy = left;
            left -= copy;
            state->length -= copy;
            do {
                *put++ = *from++;
            } while (--copy);
            if (state->length == 0) state->mode = LEN;
            break;

        case LIT:
            if (left == 0) goto inflate_leave;
            *put++ = (z_uchar)state->length;
            left--;
            state->mode = LEN;
            break;

        case CHECK:
            if (state->wrap) {
                ZI_NEEDBITS(32);
                out -= left;
                strm->total_out += out;
                state->total += out;
                if (out)
                    strm->adler = state->check =
                        func_80460308__17UnkClass_80460308Fv(
                            state->check, put - out, out);
                out = left;
                if (ZI_REVERSE(hold) != state->check) {
                    strm->msg = (char*)(message_base + 0x155);
                    state->mode = BAD;
                    break;
                }
                ZI_INITBITS();
            }
            state->mode = DONE;
            /* fall through */
        case DONE:
            ret = Z_STREAM_END;
            goto inflate_leave;
        case BAD:
            ret = Z_DATA_ERROR;
            goto inflate_leave;
        case MEM:
            return Z_MEM_ERROR;
        default:
            return Z_STREAM_ERROR;
        }
    }

inflate_leave:
    ZI_RESTORE();
    if (state->wsize || (state->mode < CHECK && out != strm->avail_out)) {
        if (updateWindow__17UnkClass_80460C34Fv(strm, out)) {
            state->mode = MEM;
            return Z_MEM_ERROR;
        }
    }
    in -= strm->avail_in;
    out -= strm->avail_out;
    strm->total_in += in;
    strm->total_out += out;
    state->total += out;
    if (state->wrap && out)
        strm->adler = state->check =
            func_80460308__17UnkClass_80460308Fv(
                state->check, strm->next_out - out, out);
    strm->data_type = state->bits + (state->last ? 64 : 0) +
        (state->mode == TYPE ? 128 : 0);
    if (((in == 0 && out == 0) || flush == Z_FINISH) && ret == Z_OK)
        ret = Z_BUF_ERROR;
    return ret;
}

#undef ZI_LOAD
#undef ZI_RESTORE
#undef ZI_INITBITS
#undef ZI_PULLBYTE
#undef ZI_NEEDBITS
#undef ZI_BITS
#undef ZI_DROPBITS
#undef ZI_BYTEBITS
#undef ZI_REVERSE
// [.data] 0x8056D600-0x8056D678 (120B): switch jumptable (116B) + pad.
// NOTE: MWCC auto-emits this jumptable from the switch in func_80460F58
// with identical relocs; this explicit table + pad is NOT added (the auto
// table already matches). See below for the 4-byte pad.
// [.rodata] 0x80523248-0x80523D60 (2840B): zlib inflate tables/strings.
const z_code lbl_80526B70[512] = {
    {96,7,0x0}, {0,8,0x50}, {0,8,0x10}, {20,8,0x73}, {18,7,0x1F}, {0,8,0x70}, {0,8,0x30}, {0,9,0xC0},
    {16,7,0xA}, {0,8,0x60}, {0,8,0x20}, {0,9,0xA0}, {0,8,0x0}, {0,8,0x80}, {0,8,0x40}, {0,9,0xE0},
    {16,7,0x6}, {0,8,0x58}, {0,8,0x18}, {0,9,0x90}, {19,7,0x3B}, {0,8,0x78}, {0,8,0x38}, {0,9,0xD0},
    {17,7,0x11}, {0,8,0x68}, {0,8,0x28}, {0,9,0xB0}, {0,8,0x8}, {0,8,0x88}, {0,8,0x48}, {0,9,0xF0},
    {16,7,0x4}, {0,8,0x54}, {0,8,0x14}, {21,8,0xE3}, {19,7,0x2B}, {0,8,0x74}, {0,8,0x34}, {0,9,0xC8},
    {17,7,0xD}, {0,8,0x64}, {0,8,0x24}, {0,9,0xA8}, {0,8,0x4}, {0,8,0x84}, {0,8,0x44}, {0,9,0xE8},
    {16,7,0x8}, {0,8,0x5C}, {0,8,0x1C}, {0,9,0x98}, {20,7,0x53}, {0,8,0x7C}, {0,8,0x3C}, {0,9,0xD8},
    {18,7,0x17}, {0,8,0x6C}, {0,8,0x2C}, {0,9,0xB8}, {0,8,0xC}, {0,8,0x8C}, {0,8,0x4C}, {0,9,0xF8},
    {16,7,0x3}, {0,8,0x52}, {0,8,0x12}, {21,8,0xA3}, {19,7,0x23}, {0,8,0x72}, {0,8,0x32}, {0,9,0xC4},
    {17,7,0xB}, {0,8,0x62}, {0,8,0x22}, {0,9,0xA4}, {0,8,0x2}, {0,8,0x82}, {0,8,0x42}, {0,9,0xE4},
    {16,7,0x7}, {0,8,0x5A}, {0,8,0x1A}, {0,9,0x94}, {20,7,0x43}, {0,8,0x7A}, {0,8,0x3A}, {0,9,0xD4},
    {18,7,0x13}, {0,8,0x6A}, {0,8,0x2A}, {0,9,0xB4}, {0,8,0xA}, {0,8,0x8A}, {0,8,0x4A}, {0,9,0xF4},
    {16,7,0x5}, {0,8,0x56}, {0,8,0x16}, {64,8,0x0}, {19,7,0x33}, {0,8,0x76}, {0,8,0x36}, {0,9,0xCC},
    {17,7,0xF}, {0,8,0x66}, {0,8,0x26}, {0,9,0xAC}, {0,8,0x6}, {0,8,0x86}, {0,8,0x46}, {0,9,0xEC},
    {16,7,0x9}, {0,8,0x5E}, {0,8,0x1E}, {0,9,0x9C}, {20,7,0x63}, {0,8,0x7E}, {0,8,0x3E}, {0,9,0xDC},
    {18,7,0x1B}, {0,8,0x6E}, {0,8,0x2E}, {0,9,0xBC}, {0,8,0xE}, {0,8,0x8E}, {0,8,0x4E}, {0,9,0xFC},
    {96,7,0x0}, {0,8,0x51}, {0,8,0x11}, {21,8,0x83}, {18,7,0x1F}, {0,8,0x71}, {0,8,0x31}, {0,9,0xC2},
    {16,7,0xA}, {0,8,0x61}, {0,8,0x21}, {0,9,0xA2}, {0,8,0x1}, {0,8,0x81}, {0,8,0x41}, {0,9,0xE2},
    {16,7,0x6}, {0,8,0x59}, {0,8,0x19}, {0,9,0x92}, {19,7,0x3B}, {0,8,0x79}, {0,8,0x39}, {0,9,0xD2},
    {17,7,0x11}, {0,8,0x69}, {0,8,0x29}, {0,9,0xB2}, {0,8,0x9}, {0,8,0x89}, {0,8,0x49}, {0,9,0xF2},
    {16,7,0x4}, {0,8,0x55}, {0,8,0x15}, {16,8,0x102}, {19,7,0x2B}, {0,8,0x75}, {0,8,0x35}, {0,9,0xCA},
    {17,7,0xD}, {0,8,0x65}, {0,8,0x25}, {0,9,0xAA}, {0,8,0x5}, {0,8,0x85}, {0,8,0x45}, {0,9,0xEA},
    {16,7,0x8}, {0,8,0x5D}, {0,8,0x1D}, {0,9,0x9A}, {20,7,0x53}, {0,8,0x7D}, {0,8,0x3D}, {0,9,0xDA},
    {18,7,0x17}, {0,8,0x6D}, {0,8,0x2D}, {0,9,0xBA}, {0,8,0xD}, {0,8,0x8D}, {0,8,0x4D}, {0,9,0xFA},
    {16,7,0x3}, {0,8,0x53}, {0,8,0x13}, {21,8,0xC3}, {19,7,0x23}, {0,8,0x73}, {0,8,0x33}, {0,9,0xC6},
    {17,7,0xB}, {0,8,0x63}, {0,8,0x23}, {0,9,0xA6}, {0,8,0x3}, {0,8,0x83}, {0,8,0x43}, {0,9,0xE6},
    {16,7,0x7}, {0,8,0x5B}, {0,8,0x1B}, {0,9,0x96}, {20,7,0x43}, {0,8,0x7B}, {0,8,0x3B}, {0,9,0xD6},
    {18,7,0x13}, {0,8,0x6B}, {0,8,0x2B}, {0,9,0xB6}, {0,8,0xB}, {0,8,0x8B}, {0,8,0x4B}, {0,9,0xF6},
    {16,7,0x5}, {0,8,0x57}, {0,8,0x17}, {64,8,0x0}, {19,7,0x33}, {0,8,0x77}, {0,8,0x37}, {0,9,0xCE},
    {17,7,0xF}, {0,8,0x67}, {0,8,0x27}, {0,9,0xAE}, {0,8,0x7}, {0,8,0x87}, {0,8,0x47}, {0,9,0xEE},
    {16,7,0x9}, {0,8,0x5F}, {0,8,0x1F}, {0,9,0x9E}, {20,7,0x63}, {0,8,0x7F}, {0,8,0x3F}, {0,9,0xDE},
    {18,7,0x1B}, {0,8,0x6F}, {0,8,0x2F}, {0,9,0xBE}, {0,8,0xF}, {0,8,0x8F}, {0,8,0x4F}, {0,9,0xFE},
    {96,7,0x0}, {0,8,0x50}, {0,8,0x10}, {20,8,0x73}, {18,7,0x1F}, {0,8,0x70}, {0,8,0x30}, {0,9,0xC1},
    {16,7,0xA}, {0,8,0x60}, {0,8,0x20}, {0,9,0xA1}, {0,8,0x0}, {0,8,0x80}, {0,8,0x40}, {0,9,0xE1},
    {16,7,0x6}, {0,8,0x58}, {0,8,0x18}, {0,9,0x91}, {19,7,0x3B}, {0,8,0x78}, {0,8,0x38}, {0,9,0xD1},
    {17,7,0x11}, {0,8,0x68}, {0,8,0x28}, {0,9,0xB1}, {0,8,0x8}, {0,8,0x88}, {0,8,0x48}, {0,9,0xF1},
    {16,7,0x4}, {0,8,0x54}, {0,8,0x14}, {21,8,0xE3}, {19,7,0x2B}, {0,8,0x74}, {0,8,0x34}, {0,9,0xC9},
    {17,7,0xD}, {0,8,0x64}, {0,8,0x24}, {0,9,0xA9}, {0,8,0x4}, {0,8,0x84}, {0,8,0x44}, {0,9,0xE9},
    {16,7,0x8}, {0,8,0x5C}, {0,8,0x1C}, {0,9,0x99}, {20,7,0x53}, {0,8,0x7C}, {0,8,0x3C}, {0,9,0xD9},
    {18,7,0x17}, {0,8,0x6C}, {0,8,0x2C}, {0,9,0xB9}, {0,8,0xC}, {0,8,0x8C}, {0,8,0x4C}, {0,9,0xF9},
    {16,7,0x3}, {0,8,0x52}, {0,8,0x12}, {21,8,0xA3}, {19,7,0x23}, {0,8,0x72}, {0,8,0x32}, {0,9,0xC5},
    {17,7,0xB}, {0,8,0x62}, {0,8,0x22}, {0,9,0xA5}, {0,8,0x2}, {0,8,0x82}, {0,8,0x42}, {0,9,0xE5},
    {16,7,0x7}, {0,8,0x5A}, {0,8,0x1A}, {0,9,0x95}, {20,7,0x43}, {0,8,0x7A}, {0,8,0x3A}, {0,9,0xD5},
    {18,7,0x13}, {0,8,0x6A}, {0,8,0x2A}, {0,9,0xB5}, {0,8,0xA}, {0,8,0x8A}, {0,8,0x4A}, {0,9,0xF5},
    {16,7,0x5}, {0,8,0x56}, {0,8,0x16}, {64,8,0x0}, {19,7,0x33}, {0,8,0x76}, {0,8,0x36}, {0,9,0xCD},
    {17,7,0xF}, {0,8,0x66}, {0,8,0x26}, {0,9,0xAD}, {0,8,0x6}, {0,8,0x86}, {0,8,0x46}, {0,9,0xED},
    {16,7,0x9}, {0,8,0x5E}, {0,8,0x1E}, {0,9,0x9D}, {20,7,0x63}, {0,8,0x7E}, {0,8,0x3E}, {0,9,0xDD},
    {18,7,0x1B}, {0,8,0x6E}, {0,8,0x2E}, {0,9,0xBD}, {0,8,0xE}, {0,8,0x8E}, {0,8,0x4E}, {0,9,0xFD},
    {96,7,0x0}, {0,8,0x51}, {0,8,0x11}, {21,8,0x83}, {18,7,0x1F}, {0,8,0x71}, {0,8,0x31}, {0,9,0xC3},
    {16,7,0xA}, {0,8,0x61}, {0,8,0x21}, {0,9,0xA3}, {0,8,0x1}, {0,8,0x81}, {0,8,0x41}, {0,9,0xE3},
    {16,7,0x6}, {0,8,0x59}, {0,8,0x19}, {0,9,0x93}, {19,7,0x3B}, {0,8,0x79}, {0,8,0x39}, {0,9,0xD3},
    {17,7,0x11}, {0,8,0x69}, {0,8,0x29}, {0,9,0xB3}, {0,8,0x9}, {0,8,0x89}, {0,8,0x49}, {0,9,0xF3},
    {16,7,0x4}, {0,8,0x55}, {0,8,0x15}, {16,8,0x102}, {19,7,0x2B}, {0,8,0x75}, {0,8,0x35}, {0,9,0xCB},
    {17,7,0xD}, {0,8,0x65}, {0,8,0x25}, {0,9,0xAB}, {0,8,0x5}, {0,8,0x85}, {0,8,0x45}, {0,9,0xEB},
    {16,7,0x8}, {0,8,0x5D}, {0,8,0x1D}, {0,9,0x9B}, {20,7,0x53}, {0,8,0x7D}, {0,8,0x3D}, {0,9,0xDB},
    {18,7,0x17}, {0,8,0x6D}, {0,8,0x2D}, {0,9,0xBB}, {0,8,0xD}, {0,8,0x8D}, {0,8,0x4D}, {0,9,0xFB},
    {16,7,0x3}, {0,8,0x53}, {0,8,0x13}, {21,8,0xC3}, {19,7,0x23}, {0,8,0x73}, {0,8,0x33}, {0,9,0xC7},
    {17,7,0xB}, {0,8,0x63}, {0,8,0x23}, {0,9,0xA7}, {0,8,0x3}, {0,8,0x83}, {0,8,0x43}, {0,9,0xE7},
    {16,7,0x7}, {0,8,0x5B}, {0,8,0x1B}, {0,9,0x97}, {20,7,0x43}, {0,8,0x7B}, {0,8,0x3B}, {0,9,0xD7},
    {18,7,0x13}, {0,8,0x6B}, {0,8,0x2B}, {0,9,0xB7}, {0,8,0xB}, {0,8,0x8B}, {0,8,0x4B}, {0,9,0xF7},
    {16,7,0x5}, {0,8,0x57}, {0,8,0x17}, {64,8,0x0}, {19,7,0x33}, {0,8,0x77}, {0,8,0x37}, {0,9,0xCF},
    {17,7,0xF}, {0,8,0x67}, {0,8,0x27}, {0,9,0xAF}, {0,8,0x7}, {0,8,0x87}, {0,8,0x47}, {0,9,0xEF},
    {16,7,0x9}, {0,8,0x5F}, {0,8,0x1F}, {0,9,0x9F}, {20,7,0x63}, {0,8,0x7F}, {0,8,0x3F}, {0,9,0xDF},
    {18,7,0x1B}, {0,8,0x6F}, {0,8,0x2F}, {0,9,0xBF}, {0,8,0xF}, {0,8,0x8F}, {0,8,0x4F}, {0,9,0xFF},
};

const z_code lbl_eu_80523A48[32] = {
    {16,5,0x1}, {23,5,0x101}, {19,5,0x11}, {27,5,0x1001}, {17,5,0x5}, {25,5,0x401}, {21,5,0x41}, {29,5,0x4001},
    {16,5,0x3}, {24,5,0x201}, {20,5,0x21}, {28,5,0x2001}, {18,5,0x9}, {26,5,0x801}, {22,5,0x81}, {64,5,0x0},
    {16,5,0x2}, {23,5,0x181}, {19,5,0x19}, {27,5,0x1801}, {17,5,0x7}, {25,5,0x601}, {21,5,0x61}, {29,5,0x6001},
    {16,5,0x4}, {24,5,0x301}, {20,5,0x31}, {28,5,0x3001}, {18,5,0xD}, {26,5,0xC01}, {22,5,0xC1}, {64,5,0x0},
};

const z_ushort lbl_eu_80523AC8[20] = {
    0x10, 0x11, 0x12, 0x0, 0x8, 0x7, 0x9, 0x6, 0xA, 0x5,
    0xB, 0x4, 0xC, 0x3, 0xD, 0x2, 0xE, 0x1, 0xF, 0x0,
};

const char lbl_eu_80523AF0[368] = {
    0x31, 0x2E, 0x32, 0x2E, 0x33, 0x00, 0x69, 0x6E, 0x63, 0x6F, 0x72, 0x72, 0x65, 0x63, 0x74, 0x20,
    0x68, 0x65, 0x61, 0x64, 0x65, 0x72, 0x20, 0x63, 0x68, 0x65, 0x63, 0x6B, 0x00, 0x75, 0x6E, 0x6B,
    0x6E, 0x6F, 0x77, 0x6E, 0x20, 0x63, 0x6F, 0x6D, 0x70, 0x72, 0x65, 0x73, 0x73, 0x69, 0x6F, 0x6E,
    0x20, 0x6D, 0x65, 0x74, 0x68, 0x6F, 0x64, 0x00, 0x69, 0x6E, 0x76, 0x61, 0x6C, 0x69, 0x64, 0x20,
    0x77, 0x69, 0x6E, 0x64, 0x6F, 0x77, 0x20, 0x73, 0x69, 0x7A, 0x65, 0x00, 0x69, 0x6E, 0x76, 0x61,
    0x6C, 0x69, 0x64, 0x20, 0x62, 0x6C, 0x6F, 0x63, 0x6B, 0x20, 0x74, 0x79, 0x70, 0x65, 0x00, 0x69,
    0x6E, 0x76, 0x61, 0x6C, 0x69, 0x64, 0x20, 0x73, 0x74, 0x6F, 0x72, 0x65, 0x64, 0x20, 0x62, 0x6C,
    0x6F, 0x63, 0x6B, 0x20, 0x6C, 0x65, 0x6E, 0x67, 0x74, 0x68, 0x73, 0x00, 0x74, 0x6F, 0x6F, 0x20,
    0x6D, 0x61, 0x6E, 0x79, 0x20, 0x6C, 0x65, 0x6E, 0x67, 0x74, 0x68, 0x20, 0x6F, 0x72, 0x20, 0x64,
    0x69, 0x73, 0x74, 0x61, 0x6E, 0x63, 0x65, 0x20, 0x73, 0x79, 0x6D, 0x62, 0x6F, 0x6C, 0x73, 0x00,
    0x69, 0x6E, 0x76, 0x61, 0x6C, 0x69, 0x64, 0x20, 0x63, 0x6F, 0x64, 0x65, 0x20, 0x6C, 0x65, 0x6E,
    0x67, 0x74, 0x68, 0x73, 0x20, 0x73, 0x65, 0x74, 0x00, 0x69, 0x6E, 0x76, 0x61, 0x6C, 0x69, 0x64,
    0x20, 0x62, 0x69, 0x74, 0x20, 0x6C, 0x65, 0x6E, 0x67, 0x74, 0x68, 0x20, 0x72, 0x65, 0x70, 0x65,
    0x61, 0x74, 0x00, 0x69, 0x6E, 0x76, 0x61, 0x6C, 0x69, 0x64, 0x20, 0x6C, 0x69, 0x74, 0x65, 0x72,
    0x61, 0x6C, 0x2F, 0x6C, 0x65, 0x6E, 0x67, 0x74, 0x68, 0x73, 0x20, 0x73, 0x65, 0x74, 0x00, 0x69,
    0x6E, 0x76, 0x61, 0x6C, 0x69, 0x64, 0x20, 0x64, 0x69, 0x73, 0x74, 0x61, 0x6E, 0x63, 0x65, 0x73,
    0x20, 0x73, 0x65, 0x74, 0x00, 0x69, 0x6E, 0x76, 0x61, 0x6C, 0x69, 0x64, 0x20, 0x6C, 0x69, 0x74,
    0x65, 0x72, 0x61, 0x6C, 0x2F, 0x6C, 0x65, 0x6E, 0x67, 0x74, 0x68, 0x20, 0x63, 0x6F, 0x64, 0x65,
    0x00, 0x69, 0x6E, 0x76, 0x61, 0x6C, 0x69, 0x64, 0x20, 0x64, 0x69, 0x73, 0x74, 0x61, 0x6E, 0x63,
    0x65, 0x20, 0x63, 0x6F, 0x64, 0x65, 0x00, 0x69, 0x6E, 0x76, 0x61, 0x6C, 0x69, 0x64, 0x20, 0x64,
    0x69, 0x73, 0x74, 0x61, 0x6E, 0x63, 0x65, 0x20, 0x74, 0x6F, 0x6F, 0x20, 0x66, 0x61, 0x72, 0x20,
    0x62, 0x61, 0x63, 0x6B, 0x00, 0x69, 0x6E, 0x63, 0x6F, 0x72, 0x72, 0x65, 0x63, 0x74, 0x20, 0x64,
    0x61, 0x74, 0x61, 0x20, 0x63, 0x68, 0x65, 0x63, 0x6B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const z_ushort lbl_eu_80523C60[32] = {
    0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA,
    0xB, 0xD, 0xF, 0x11, 0x13, 0x17, 0x1B, 0x1F,
    0x23, 0x2B, 0x33, 0x3B, 0x43, 0x53, 0x63, 0x73,
    0x83, 0xA3, 0xC3, 0xE3, 0x102, 0x0, 0x0, 0x0,
};

const z_ushort lbl_eu_80523CA0[32] = {
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x11, 0x11, 0x11, 0x11, 0x12, 0x12, 0x12, 0x12,
    0x13, 0x13, 0x13, 0x13, 0x14, 0x14, 0x14, 0x14,
    0x15, 0x15, 0x15, 0x15, 0x10, 0xC9, 0xC4, 0x0,
};

const z_ushort lbl_eu_80523CE0[32] = {
    0x1, 0x2, 0x3, 0x4, 0x5, 0x7, 0x9, 0xD,
    0x11, 0x19, 0x21, 0x31, 0x41, 0x61, 0x81, 0xC1,
    0x101, 0x181, 0x201, 0x301, 0x401, 0x601, 0x801, 0xC01,
    0x1001, 0x1801, 0x2001, 0x3001, 0x4001, 0x6001, 0x0, 0x0,
};

const z_ushort lbl_eu_80523D20[32] = {
    0x10, 0x10, 0x10, 0x10, 0x11, 0x11, 0x12, 0x12,
    0x13, 0x13, 0x14, 0x14, 0x15, 0x15, 0x16, 0x16,
    0x17, 0x17, 0x18, 0x18, 0x19, 0x19, 0x1A, 0x1A,
    0x1B, 0x1B, 0x1C, 0x1C, 0x1D, 0x1D, 0x40, 0x40,
};
