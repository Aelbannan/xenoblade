// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/mps/mps_dec
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

typedef struct MPS_WORK MPS_WORK;

/* MPS pack header, decoded by mpsdec_DecPackHd (work + 0x18) */
typedef struct MPS_PACKHD {
    u32 scr_h;   /* 0x18: SCR bits [31:15] (2 + 15 bits) */
    u32 scr_l;   /* 0x1C: SCR low 15 bits */
    u32 flag;    /* 0x20: set when the 2-bit marker is zero */
    u32 rate;    /* 0x24: 22-bit mux rate */
} MPS_PACKHD;

/* MPS packet header, decoded by mpsdec_DecPketHd (work + 0xA8) */
typedef struct MPS_PKETHD {
    u32 pts_lo;     /* 0xA8 */
    u32 pts_hi;     /* 0xAC */
    u32 dts_lo;     /* 0xB0 */
    u32 dts_hi;     /* 0xB4 */
    u32 stream_id;  /* 0xB8 */
    u32 type;       /* 0xBC */
    u32 sub;        /* 0xC0 */
    u32 length;     /* 0xC4 */
    u32 mpeg1_pts;  /* 0xC8 */
    u32 payload;    /* 0xCC */
} MPS_PKETHD;

struct MPS_WORK {
    u8         pad0[0x18];      /* 0x00 */
    MPS_PACKHD pack;            /* 0x18 */
    u8         pad1[0x80];      /* 0x28 (system header area) */
    MPS_PKETHD pket;            /* 0xA8 */
    u8         pad2[0x0C];      /* 0xD0 */
    void      *dec_fn;          /* 0xD4 */
    u8         pad3[0x0C];      /* 0xD8 */
    void      *sys_fn;          /* 0xE4 */
    void      *sys_obj;         /* 0xE8 */
    void      *psmap_fn;        /* 0xEC */
    void      *psmap_obj;       /* 0xF0 */
    void      *pes_fn;          /* 0xF4 */
    void      *pes_obj;         /* 0xF8 */
};

int MPSLIB_CheckHn(MPS_WORK*);

void MPS_SetSystemFn(MPS_WORK* hn, void* fn, void* obj) {
    if (MPSLIB_CheckHn(hn) == 0) {
        *(void**)((u8*)hn + 0xE4) = fn;
        *(void**)((u8*)hn + 0xE8) = obj;
    }
}

void MPS_SetPsMapFn(MPS_WORK* hn, void* fn, void* obj) {
    if (MPSLIB_CheckHn(hn) == 0) {
        *(void**)((u8*)hn + 0xEC) = fn;
        *(void**)((u8*)hn + 0xF0) = obj;
    }
}

void MPS_SetPesFn(MPS_WORK *hn, void *pes_fn, void *pes_obj) {
    if (MPSLIB_CheckHn(hn) == 0) {
        hn->pes_fn = pes_fn;
        hn->pes_obj = pes_obj;
    }
}

extern int MPSLIB_SetErr(void *, int);

typedef void (*mps_dechd_fn)(void *, void *, void *, void *, void *);

void MPS_DecHd(void *handle, void *arg1, void *arg2, void *out1, void *out2) {
    mps_dechd_fn fn;

    *(int *)out1 = 0;
    *(int *)out2 = 0;

    if (MPSLIB_CheckHn((MPS_WORK*)handle)) {
        MPSLIB_SetErr(NULL, 0xFF020301);
        return;
    }

    fn = *(mps_dechd_fn *)((u8 *)handle + 0xD4);
    fn(handle, arg1, arg2, out1, out2);
}

void MPSDEC_DecHdMpeg1() {}

/*
 * MPS bit reader: `cur` holds the unread bits at the top of the register
 * (bit 31 is the next bit), `next` is the following word, `bitpos` is the
 * number of bits consumed from the current 64-bit window (0..31) and `p`
 * points just past the words consumed so far.  `MPS_GETBITS` reads and
 * consumes `n` bits; `MPS_SKIPBITS` only consumes.
 */
#define MPS_GETBITS(n, out)                                                     \
    do {                                                                        \
        if (bitpos >= 32 - (n)) {                                               \
            bitpos -= 32 - (n);                                                 \
            if (bitpos) {                                                       \
                cur |= next >> ((n) - bitpos);                                  \
                (out) = cur >> (32 - (n));                                      \
                cur = next << bitpos;                                           \
            } else {                                                            \
                (out) = cur >> (32 - (n));                                      \
                cur = next;                                                     \
            }                                                                   \
            next = *p++;                                                        \
        } else {                                                                \
            (out) = cur >> (32 - (n));                                          \
            cur <<= (n);                                                        \
            bitpos += (n);                                                      \
        }                                                                       \
    } while (0)

#define MPS_SKIPBITS(n)                                                         \
    do {                                                                        \
        bitpos += (n);                                                          \
        if (bitpos >= 32) {                                                     \
            bitpos -= 32;                                                       \
            cur = next << bitpos;                                               \
            next = *p++;                                                        \
        } else {                                                                \
            cur <<= (n);                                                        \
        }                                                                       \
    } while (0)

void mpsdec_DecPackHd(MPS_WORK *hn, const u8 *src, u32 *size_out) {
    u32 addr = (u32)src + 4;
    u32 *p = (u32 *)(addr & ~3u);
    int bitpos = (int)((addr - (u32)p) << 3);
    u32 cur = p[0] << bitpos;
    u32 next = p[1];
    u32 f0, f1, f2, f3, f4;

    p += 2;

    /* first field: 2 bits; bitpos is a multiple of 8 here so the cross
     * case can only be bitpos == 31, borrowing exactly one bit */
    if (bitpos >= 30) {
        bitpos -= 30;
        if (bitpos) {
            cur |= next >> 1;
            f0 = cur >> 30;
            cur = next << 1;
        } else {
            f0 = cur >> 30;
            cur = next;
        }
        next = *p++;
    } else {
        f0 = cur >> 30;
        cur <<= 2;
        bitpos += 2;
    }
    MPS_SKIPBITS(2);
    MPS_GETBITS(3, f1);
    MPS_SKIPBITS(1);
    MPS_GETBITS(15, f2);
    MPS_SKIPBITS(1);
    MPS_GETBITS(15, f3);
    MPS_SKIPBITS(1);
    MPS_GETBITS(22, f4);

    hn->pack.flag = (f0 == 0) ? 1 : 0;
    hn->pack.scr_l = f3 | (f1 << 30) | (f2 << 15);
    hn->pack.scr_h = ((f1 & 3) << 30) | ((f2 & 0x7FFF) << 15);
    hn->pack.rate = f4;
    *size_out = 12;
}

void mpsdec_DecPketHd(MPS_WORK *hn, const u8 *src, u32 *size_out, u32 count) {
    u32 addr = (u32)src + 3;
    u32 *p = (u32 *)(addr & ~3u);
    int bitpos = (int)((addr - (u32)p) << 3);
    u32 cur = p[0] << bitpos;
    u32 next = p[1];
    u32 b, type, sub, v, bit, a, c, d;
    u32 consumed;
    u32 end, sz;

    p += 2;

    MPS_GETBITS(8, b);
    hn->pket.stream_id = b;

    if ((u32)(b - 0xE0) <= 0xF) {
        type = 1;
        sub = b - 0xE0;
    } else if ((u32)(b - 0xC0) <= 0x1F) {
        type = 0;
        sub = b - 0xC0;
    } else if (b == 0xBD) {
        type = 2;
        sub = 1;
    } else if (b == 0xBF) {
        type = 2;
        sub = 2;
    } else if (b == 0xBE) {
        type = 3;
        sub = 0;
    } else {
        type = 4;
        sub = 0;
    }
    hn->pket.type = type;
    hn->pket.sub = sub;

    if (count == 2) {
        if (bitpos >= 16) {
            bitpos -= 16;
            if (bitpos) {
                cur |= next >> (16 - bitpos);
                hn->pket.length = cur >> 16;
                cur = next << bitpos;
            } else {
                hn->pket.length = cur >> 16;
                cur = next;
            }
            next = *p++;
        } else {
            hn->pket.length = cur >> 16;
            cur <<= 16;
            bitpos += 16;
        }
        consumed = 6;
    } else {
        if (bitpos) {
            cur |= next >> (32 - bitpos);
            hn->pket.length = cur;
            cur = next << bitpos;
        } else {
            hn->pket.length = cur;
            cur = next;
        }
        next = *p++;
        consumed = 8;
    }

    if ((u32)(b - 0xBE) <= 1) {
        *size_out = consumed;
        hn->pket.payload = hn->pket.length;
        return;
    }

    /* skip stuffing bytes (0xFF) */
    for (;;) {
        v = cur >> 24;
        if (bitpos > 24) {
            v |= next >> (56 - bitpos);
        }
        if (v != 0xFF) {
            break;
        }
        MPS_SKIPBITS(8);
    }

    v = cur >> 30;
    if (bitpos > 30) {
        v |= next >> (62 - bitpos);
    }
    if (v == 1) {
        /* MPEG-1 style PTS */
        MPS_SKIPBITS(2);
        MPS_GETBITS(1, bit);
        MPS_GETBITS(13, d);
        d <<= 7;
        if (bit) {
            d <<= 3;
        }
        hn->pket.mpeg1_pts = d;
    }

    v = cur >> 28;
    if (bitpos > 28) {
        v |= next >> (60 - bitpos);
    }
    if (v == 2) {
        /* MPEG-2 style: PTS only */
        MPS_SKIPBITS(4);
        MPS_GETBITS(3, a);
        MPS_SKIPBITS(1);
        MPS_GETBITS(15, c);
        MPS_SKIPBITS(1);
        MPS_GETBITS(15, d);
        MPS_SKIPBITS(1);
        hn->pket.pts_lo = (a << 30) | (c << 15);
        hn->pket.pts_hi = hn->pket.pts_lo | d;
        hn->pket.dts_lo = 0xFFFFFFFF;
        hn->pket.dts_hi = 0xFFFFFFFF;
    } else if (v == 3) {
        /* MPEG-2 style: PTS + DTS */
        MPS_SKIPBITS(4);
        MPS_GETBITS(3, a);
        MPS_SKIPBITS(1);
        MPS_GETBITS(15, c);
        MPS_SKIPBITS(1);
        MPS_GETBITS(15, d);
        MPS_SKIPBITS(1);
        hn->pket.pts_lo = (a << 30) | (c << 15);
        hn->pket.pts_hi = hn->pket.pts_lo | d;
        MPS_SKIPBITS(4);
        MPS_GETBITS(3, a);
        MPS_SKIPBITS(1);
        MPS_GETBITS(15, c);
        MPS_SKIPBITS(1);
        MPS_GETBITS(15, d);
        MPS_SKIPBITS(1);
        hn->pket.dts_lo = (a << 30) | (c << 15);
        hn->pket.dts_hi = hn->pket.dts_lo | d;
    } else {
        MPS_SKIPBITS(8);
        hn->pket.pts_lo = 0xFFFFFFFF;
        hn->pket.pts_hi = 0xFFFFFFFF;
        hn->pket.dts_lo = 0xFFFFFFFF;
        hn->pket.dts_hi = 0xFFFFFFFF;
    }

    end = (u32)p + ((bitpos + 7) >> 3) - 8;
    sz = end - (u32)src;
    *size_out = sz;
    hn->pket.payload = hn->pket.length + consumed - sz;
}
