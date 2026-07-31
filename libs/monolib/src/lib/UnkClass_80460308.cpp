// monolib zlib compression helpers.
//
// Recovered from retail asm (build/us/asm/monolib/src/lib/UnkClass_80460308.s):
//   - func_80460308__17UnkClass_80460308Fv: Adler-32 checksum (zlib adler32).
//   - func_80460728__17UnkClass_80460308Fv: inflate "fast" block decoder core
//     (zlib inflate_fast), driven by the inflate loop in UnkClass_80460C34.
//
// The retail symbols carry placeholder "Fv" mangling even though both
// functions take real arguments, so they are defined with their literal
// retail names via extern "C" (same pattern as CLibVM.cpp).

#include <harness_catalog.h>

/*
 * ============================ Adler-32 ============================
 */

#define ADLER_BASE 65521UL /* largest prime smaller than 65536 */
#define ADLER_NMAX 5552    /* largest n with 255n(n+1)/2 + (n+1)(BASE-1) < 2^32 */

#define ADLER_DO1(buf, i) { adler += (buf)[i]; sum2 += adler; }
#define ADLER_DO2(buf, i) ADLER_DO1(buf, i); ADLER_DO1(buf, i + 1);
#define ADLER_DO4(buf, i) ADLER_DO2(buf, i); ADLER_DO2(buf, i + 2);
#define ADLER_DO8(buf, i) ADLER_DO4(buf, i); ADLER_DO4(buf, i + 4);
#define ADLER_DO16(buf)   ADLER_DO8(buf, 0); ADLER_DO8(buf, 8);

extern "C" u32 func_80460308__17UnkClass_80460308Fv(u32 adler, const u8* buf, u32 len)
{
    u32 sum2;
    u32 n;

    /* split Adler-32 into component sums */
    sum2 = (adler >> 16) & 0xffff;
    adler &= 0xffff;

    /* in case user likes doing a byte at a time, keep it fast */
    if (len == 1) {
        adler += buf[0];
        if (adler >= ADLER_BASE)
            adler -= ADLER_BASE;
        sum2 += adler;
        if (sum2 >= ADLER_BASE)
            sum2 -= ADLER_BASE;
        return adler | (sum2 << 16);
    }

    /* initial Adler-32 value (deferred check for len == 1 speed) */
    if (buf == 0)
        return 1;

    /* in case short lengths are provided, keep it somewhat fast */
    if (len < 16) {
        while (len--) {
            adler += *buf++;
            sum2 += adler;
        }
        if (adler >= ADLER_BASE)
            adler -= ADLER_BASE;
        sum2 %= ADLER_BASE;
        return adler | (sum2 << 16);
    }

    /* do length NMAX blocks -- requires just one modulo operation */
    while (len >= ADLER_NMAX) {
        len -= ADLER_NMAX;
        n = ADLER_NMAX / 16; /* NMAX is divisible by 16 */
        do {
            ADLER_DO16(buf); /* 16 sums unrolled */
            buf += 16;
        } while (--n);
        adler %= ADLER_BASE;
        sum2 %= ADLER_BASE;
    }

    /* do remaining bytes (less than NMAX, still just one modulo) */
    if (len) { /* avoid modulos if none remaining */
        while (len >= 16) {
            len -= 16;
            ADLER_DO16(buf);
            buf += 16;
        }
        while (len--) {
            adler += *buf++;
            sum2 += adler;
        }
        adler %= ADLER_BASE;
        sum2 %= ADLER_BASE;
    }

    /* return recombined sums */
    return adler | (sum2 << 16);
}

/*
 * ======================= inflate fast core ========================
 */

/* Huffman code table entry (retail layout: op, bits, val). */
struct InfCode {
    u8 op;
    u8 bits;
    u16 val;
};

/* Inflate working state (fields at retail offsets). */
struct InfState {
    u32 mode;               /* 0x00 */
    u32 gap04[9];           /* 0x04 */
    u32 wsize;              /* 0x28 */
    u32 whave;              /* 0x2C */
    u32 wnext;              /* 0x30 */
    u8* window;             /* 0x34 */
    u32 hold;               /* 0x38 */
    u32 bits;               /* 0x3C */
    u32 gap40[3];           /* 0x40 */
    const InfCode* lencode; /* 0x4C */
    const InfCode* distcode;/* 0x50 */
    u32 lenbits;            /* 0x54 */
    u32 distbits;           /* 0x58 */
};

/* zlib-style stream (fields at retail offsets). */
struct InfStream {
    u8* next_in;        /* 0x00 */
    u32 avail_in;       /* 0x04 */
    u32 total_in;       /* 0x08 */
    u8* next_out;       /* 0x0C */
    u32 avail_out;      /* 0x10 */
    u32 total_out;      /* 0x14 */
    char* msg;          /* 0x18 */
    InfState* state;    /* 0x1C */
};

/* inflate mode values used by this decoder. */
#define INF_MODE_TYPE 0xB
#define INF_MODE_BAD  0x1B

/* Retail error string pool (DOL .rodata string table). */
extern "C" const char lbl_eu_805231F8[];
#define INF_MSG_DIST_TOOFAR ((char*)(lbl_eu_805231F8 + 0x00))
#define INF_MSG_DIST_CODE   ((char*)(lbl_eu_805231F8 + 0x1E))
#define INF_MSG_LEN_CODE    ((char*)(lbl_eu_805231F8 + 0x34))

#define INF_OFF 1
#define INF_PUP(a) (*++(a))

#define INF_PULLBYTE()                       \
    do {                                     \
        hold += (u32)INF_PUP(in) << bits;    \
        bits += 8;                           \
    } while (0)

#define INF_DROPBITS(n)                      \
    do {                                     \
        hold >>= (n);                        \
        bits -= (n);                         \
    } while (0)

/*
 * Decode compressed blocks until input runs low (less than 5 bytes in
 * hand) or output gets within 257 bytes of full, updating the stream
 * and decoder state in place. `start` is the caller's initial
 * avail_out, used to recover the output run base.
 */
extern "C" void func_80460728__17UnkClass_80460308Fv(InfStream* strm, u32 start)
{
    InfState* state;
    u8* in;
    u8* last;
    u8* out;
    u8* beg;
    u8* end;
    u32 wsize;
    u32 whave;
    u32 wnext;
    u8* window;
    u32 hold;
    u32 bits;
    const InfCode* lcode;
    const InfCode* dcode;
    u32 lmask;
    u32 dmask;
    InfCode here;
    u32 op;
    u32 len;
    u32 dist;
    u8* from;

    /* copy state to local variables */
    state = strm->state;
    in = strm->next_in - INF_OFF;
    last = in + (strm->avail_in - 5);
    out = strm->next_out - INF_OFF;
    beg = out - (start - strm->avail_out);
    end = out + (strm->avail_out - 257);
    wsize = state->wsize;
    whave = state->whave;
    wnext = state->wnext;
    window = state->window;
    hold = state->hold;
    bits = state->bits;
    lcode = state->lencode;
    dcode = state->distcode;
    lmask = (1U << state->lenbits) - 1;
    dmask = (1U << state->distbits) - 1;

    /* decode literals and length/distance pairs */
    do {
        if (bits < 15) {
            INF_PULLBYTE();
            INF_PULLBYTE();
        }
        here = lcode[hold & lmask];
    dolen:
        op = here.op;
        INF_DROPBITS(here.bits);
        if (op == 0) {
            /* literal */
            INF_PUP(out) = (u8)here.val;
        }
        else if (op & 16) {
            /* length base */
            len = here.val;
            op &= 15; /* number of extra bits */
            if (op) {
                if (bits < op) {
                    INF_PULLBYTE();
                }
                len += hold & ((1U << op) - 1);
                INF_DROPBITS(op);
            }
            if (bits < 15) {
                INF_PULLBYTE();
                INF_PULLBYTE();
            }
            here = dcode[hold & dmask];
        dodist:
            op = here.op;
            INF_DROPBITS(here.bits);
            if (op & 16) {
                /* distance base */
                dist = here.val;
                op &= 15; /* number of distance bits */
                if (bits < op) {
                    INF_PULLBYTE();
                    if (bits < op) {
                        INF_PULLBYTE();
                    }
                }
                dist += hold & ((1U << op) - 1);
                INF_DROPBITS(op);
                op = (u32)(out - beg); /* max distance in output */
                if (dist > op) {
                    /* see if copy from window */
                    op = dist - op; /* distance back in window */
                    if (op > whave) {
                        strm->msg = INF_MSG_DIST_TOOFAR;
                        state->mode = INF_MODE_BAD;
                        break;
                    }
                    from = window - INF_OFF;
                    if (wnext == 0) {
                        /* very common case */
                        from += wsize - op;
                        if (op < len) {
                            /* some from window */
                            len -= op;
                            do {
                                INF_PUP(out) = INF_PUP(from);
                            } while (--op);
                            from = out - dist; /* rest from output */
                        }
                    }
                    else if (wnext < op) {
                        /* wrap around window */
                        from += wsize + wnext - op;
                        op -= wnext;
                        if (op < len) {
                            /* some from end of window */
                            len -= op;
                            do {
                                INF_PUP(out) = INF_PUP(from);
                            } while (--op);
                            from = window - INF_OFF;
                            if (wnext < len) {
                                /* some from start of window */
                                op = wnext;
                                len -= op;
                                do {
                                    INF_PUP(out) = INF_PUP(from);
                                } while (--op);
                                from = out - dist; /* rest from output */
                            }
                        }
                    }
                    else {
                        /* contiguous in window */
                        from += wnext - op;
                        if (op < len) {
                            /* some from window */
                            len -= op;
                            do {
                                INF_PUP(out) = INF_PUP(from);
                            } while (--op);
                            from = out - dist; /* rest from output */
                        }
                    }
                    while (len > 2) {
                        INF_PUP(out) = INF_PUP(from);
                        INF_PUP(out) = INF_PUP(from);
                        INF_PUP(out) = INF_PUP(from);
                        len -= 3;
                    }
                    if (len) {
                        INF_PUP(out) = INF_PUP(from);
                        if (len > 1)
                            INF_PUP(out) = INF_PUP(from);
                    }
                }
                else {
                    /* copy direct from output */
                    from = out - dist;
                    do {
                        /* minimum length is three */
                        INF_PUP(out) = INF_PUP(from);
                        INF_PUP(out) = INF_PUP(from);
                        INF_PUP(out) = INF_PUP(from);
                        len -= 3;
                    } while (len > 2);
                    if (len) {
                        INF_PUP(out) = INF_PUP(from);
                        if (len > 1)
                            INF_PUP(out) = INF_PUP(from);
                    }
                }
            }
            else if ((op & 64) == 0) {
                /* 2nd level distance code */
                here = dcode[here.val + (hold & ((1U << op) - 1))];
                goto dodist;
            }
            else {
                strm->msg = INF_MSG_DIST_CODE;
                state->mode = INF_MODE_BAD;
                break;
            }
        }
        else if ((op & 64) == 0) {
            /* 2nd level length code */
            here = lcode[here.val + (hold & ((1U << op) - 1))];
            goto dolen;
        }
        else if (op & 32) {
            /* end of block */
            state->mode = INF_MODE_TYPE;
            break;
        }
        else {
            strm->msg = INF_MSG_LEN_CODE;
            state->mode = INF_MODE_BAD;
            break;
        }
    } while (in < last && out < end);

    /* return unused bytes (on entry, bits < 8, so in won't go too far back) */
    len = bits >> 3;
    in -= len;
    bits -= len << 3;
    hold &= (1U << bits) - 1;

    /* update state and return */
    strm->next_in = in + INF_OFF;
    strm->next_out = out + INF_OFF;
    strm->avail_in = (u32)(in < last ? 5 + (last - in) : 5 - (in - last));
    strm->avail_out = (u32)(out < end ? 257 + (end - out) : 257 - (out - end));
    state->hold = hold;
    state->bits = bits;
}
