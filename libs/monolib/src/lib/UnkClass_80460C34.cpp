// zlib 1.2.3 inflate -- see UnkClass_80460C34.hpp for layout/rationale.
#include "monolib/lib/UnkClass_80460C34.hpp"

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
    int end;
    z_ushort count[Z_MAXBITS + 1];
    z_ushort offs[Z_MAXBITS + 1];

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

    const z_ushort* extra;
    const z_ushort* base;
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
        if (func_80460DDC__17UnkClass_80460C34Fv(strm, out)) {
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
