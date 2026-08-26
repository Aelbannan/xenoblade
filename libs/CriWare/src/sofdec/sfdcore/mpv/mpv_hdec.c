// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/mpv/mpv_hdec
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern void SJ_SplitChunk(const void* src, int size, void* dst1, void* dst2);

typedef struct MpvSjChunk {
    const u8* p;
    s32 size;
} MpvSjChunk;

/* Per-codec user-data handler record (retail stride 12 from 0xD5C). */
typedef struct MpvUdEntry {
    void* udsc;               /* SJ chunk-reader object (0xD5C + 12*i) */
    void* notify;             /* post-decode callback (0xD60 + 12*i) */
    void* ctx;                /* callback context (0xD64 + 12*i) */
} MpvUdEntry;

/* MPV header-decode work structure (retail layout, offsets from self). */
typedef struct MpvHdec {
    u8  pad00[0x9AC];
    u32 t9ac;                 /* 0x9AC: quant-table id/base */
    u8  pad9B0[0xB14 - 0x9B0];
    s32 b14;                  /* 0xB14 */
    u8  padB18[0xB20 - 0xB18];
    s32 b20;                  /* 0xB20 */
    u8  padB24[0xB28 - 0xB24];
    s32 b28;                  /* 0xB28 */
    u8  padB2C[0xB5C - 0xB2C];
    s32 b5c;                  /* 0xB5C */
    s32 b60;                  /* 0xB60 */
    s32 b64;                  /* 0xB64 */
    s32 b68;                  /* 0xB68 */
    s32 b6c;                  /* 0xB6C */
    s32 b70;                  /* 0xB70 */
    s32 b74;                  /* 0xB74 */
    s32 b78;                  /* 0xB78 */
    s32 b7c;                  /* 0xB7C */
    s32 b80;                  /* 0xB80 */
    s32 b84;                  /* 0xB84 */
    s32 b88;                  /* 0xB88 */
    s32 b8c;                  /* 0xB8C */
    s32 b90;                  /* 0xB90 */
    u8  padB94[0xBA4 - 0xB94];
    s32 ba4;                  /* 0xBA4 */
    s32 ba8;                  /* 0xBA8 */
    u8  padBAC[0xBB5 - 0xBAC];
    u8  bb5;                  /* 0xBB5 */
    u8  bb6;                  /* 0xBB6 */
    u8  padBB7[0xBC4 - 0xBB7];
    s32 bc4;                  /* 0xBC4 */
    u8  padBC8[0xBE8 - 0xBC8];
    s32 be8;                  /* 0xBE8 */
    s32 bec;                  /* 0xBEC */
    u8  padBF0[0xC44 - 0xBF0];
    u32 c44;                  /* 0xC44 */
    s32 c48;                  /* 0xC48 */
    s32 c4c;                  /* 0xC4C */
    s32 c50;                  /* 0xC50 */
    s32 c54;                  /* 0xC54 */
    s32 c58;                  /* 0xC58 */
    s32 c5c;                  /* 0xC5C */
    s32 c60;                  /* 0xC60 */
    s32 c64;                  /* 0xC64 */
    u8  padC68[0xC70 - 0xC68];
    s32 c70;                  /* 0xC70 */
    s32 c74;                  /* 0xC74 */
    s32 c78;                  /* 0xC78 */
    s32 c7c;                  /* 0xC7C */
    s32 c80;                  /* 0xC80 */
    u8  padC84[0xC90 - 0xC84];
    s32 c90;                  /* 0xC90 */
    s32 c94;                  /* 0xC94 */
    s32 c98;                  /* 0xC98 */
    s32 c9c;                  /* 0xC9C */
    u8  padCA0[0xCB4 - 0xCA0];
    s32 cb4;                  /* 0xCB4 */
    s32 cb8;                  /* 0xCB8 */
    s32 cbc;                  /* 0xCBC */
    s32 cc0;                  /* 0xCC0 */
    u8  padCC4[0xCE4 - 0xCC4];
    s32 ce4;                  /* 0xCE4 */
    u8  padCE8[0xCFC - 0xCE8];
    s32 cfc;                  /* 0xCFC */
    s32 d00;                  /* 0xD00 */
    s32 d04;                  /* 0xD04 */
    s32 d08;                  /* 0xD08 */
    u8  padD0C[0xD24 - 0xD0C];
    s32 d24;                  /* 0xD24 */
    s32 d28;                  /* 0xD28 */
    MpvSjChunk chunk;         /* 0xD2C */
    u8  padD34[0xD38 - 0xD34];
    s32 d38;                  /* 0xD38 */
    u8  padD3C[0xD58 - 0xD3C];
    s32 d58;                  /* 0xD58: active user-data slot count */
    MpvUdEntry ud[4];         /* 0xD5C: one record per codec slot */
    void* d8c;                /* 0xD8C */
    void* d90;                /* 0xD90 */
    s32 d94;                  /* 0xD94 */
    u8  padD98[0xD9C - 0xD98];
    s32 d9c;                  /* 0xD9C */
    s32 da0;                  /* 0xDA0 */
    u8  padDA4[0xDA8 - 0xDA4];
    s32 da8;                  /* 0xDA8 */
} MpvHdec;

s32 MPV_GoNextDelimSj(void* self);
extern s32 mpvhdec_AnalyUd(void* self, const u8* p, s32 size);

extern void MPVUMC_PpicSkipped(void);
extern void MPVUMC_BpicSkipped(void);
extern void fn_803AFDB0(void);
extern void fn_803AFFB4(void);
extern void fn_803B00B8(void);
extern void fn_803B01BC(void);
extern u32 lbl_eu_80602A90[];
extern u32 lbl_eu_80602A50[];

/* MPV header-decode work table: five contiguous function-pointer blocks
 * (0x28 + 0x50 + 0x28 + 0x28 + 0x28 = 0xF0 bytes total). */
typedef struct MpvHdecTbl {
    void (*a[10])(void); /* 0x00 */
    void (*b[20])(void); /* 0x28 */
    void (*c[10])(void); /* 0x78 */
    void (*d[10])(void); /* 0xa0 */
    void (*e[10])(void); /* 0xc8 */
} MpvHdecTbl;

void MPVHDEC_Init(void) {
    MpvHdecTbl* tbl = (MpvHdecTbl*)lbl_eu_80602A90;
    void (*ppic)(void);
    void (*bpic)(void);
    void (*shc)(void);
    void (*gsc)(void);
    void (*esc)(void);
    void (*udsc)(void);

    memset(&tbl->a, 0, sizeof(tbl->a));
    memset(&tbl->b, 0, sizeof(tbl->b));
    memset(&tbl->c, 0, sizeof(tbl->c));
    memset(&tbl->d, 0, sizeof(tbl->d));
    memset(&tbl->e, 0, sizeof(tbl->e));

    ppic = MPVUMC_PpicSkipped;
    bpic = MPVUMC_BpicSkipped;
    shc = fn_803AFDB0;
    gsc = fn_803AFFB4;
    esc = fn_803B00B8;
    udsc = fn_803B01BC;
    tbl->a[1] = ppic;
    tbl->a[2] = ppic;
    tbl->a[3] = bpic;
    tbl->b[1] = shc;
    tbl->b[2] = shc;
    tbl->b[3] = shc;
    tbl->b[4] = shc;
    tbl->b[11] = shc;
    tbl->b[12] = shc;
    tbl->b[13] = shc;
    tbl->b[14] = shc;
    tbl->c[2] = gsc;
    tbl->c[3] = gsc;
    tbl->d[3] = esc;
    tbl->e[3] = udsc;
}

void MPV_SetUsrSj(void* self, u32 idx, u32 a, u32 b, u32 c) {
    void* base = (u8*)self + idx * 0xc;
    *(u32*)((u8*)base + 0xd5c) = a;
    *(u32*)((u8*)base + 0xd60) = b;
    *(u32*)((u8*)base + 0xd64) = c;
}

void MPV_SetPicUsrBuf(void* self, void* a, void* b) {
    *(void**)((u8*)self + 0xd8c) = a;
    *(void**)((u8*)self + 0xd90) = b;
    *(u32*)((u8*)self + 0xd94) = 0;
}

void MPV_GetPicUsr(void* self, u32* out_top, u32* out_bot) {
    if (out_top != NULL) {
        *out_top = *(u32*)((u8*)self + 0xd8c);
    }
    if (out_bot != NULL) {
        *out_bot = *(u32*)((u8*)self + 0xd94);
    }
}

extern s32 MPVLIB_CheckHn(void* h);
extern s32 MPVERR_SetCode(void* h, u32 code);
extern s32 MPVM2V_DecodePicAtr(void* self, void* sj);
extern s32 MPV_CheckDelim(const u8* p);
extern s32 mpvhdec_GetCodec(void* self, s32* param);
extern s32 mpvhdec_DecShcSj(void* self, void* sj);
s32 mpvhdec_DecGscSj(MpvHdec* self, void* sj);
extern s32 mpvhdec_DecPscSj(void* self, void* sj);
extern s32 mpvhdec_DecEscSj(void* self, void* sj, s32 codec);
extern s32 mpvhdec_DecUdscSj(void* self, void* sj, s32 codec);
extern s32 MPV_GoNextDelimSj(void* a);

s32 MPVHDEC_RecoverSj(void* self, s32 mask, void* sj) {
    s32 r = *(s32*)((u8*)self + 0xd24);
    s32 v = *(s32*)((u8*)self + 0xb14);
    s32 ret;
    s32 sp[2];
    if (r != 0) {
        *(s32*)((u8*)self + 0xd24) = 0;
        *(s32*)((u8*)self + 0xd28) += 1;
        *(s32*)((u8*)self + 0xbe8) += 1;
        if (v == 0)
            return -2;
        *(s32*)((u8*)self + 0xbec) += 1;
    }
    ret = -3;
    if (v == 0)
        ret = -2;
    for (;;) {
        s32 t = MPV_GoNextDelimSj(sj);
        if (t == 0)
            break;
        if ((t & mask) != 0) {
            ret = 0;
            break;
        }
        ((void (*)(void*, s32, s32, s32*))*(void**)((u8*)*(void**)((u8*)sj) + 0x18))(sj, 1, 4, sp);
        ((void (*)(void*, s32, s32*))*(void**)((u8*)*(void**)((u8*)sj) + 0x20))(sj, 0, sp);
        if (sp[1] != 4)
            break;
    }
    return ret;
}

extern void* SJMEM_Create(void* pool_mem, u32 flags);
extern s32 MPV_DecodePicAtrSj(MpvHdec* self, void* sj);

/* sj object method dispatch helpers (vtable calls at fixed indices) */
static inline s32 SjRead(void* sj) {
    return ((s32 (*)(void*, s32))((void**)*(void**)sj)[9])(sj, 1);
}
static inline void SjDelete(void* sj) {
    ((void (*)(void*))((void**)*(void**)sj)[3])(sj);
}

s32 MPV_DecodePicAtr(void* self, MpvSjChunk* chunk, s32* out) {
    s32 ret;
    void* sj;
    sj = SJMEM_Create((void*)chunk->p, chunk->size);
    if (sj == NULL)
        return -1;
    ret = MPV_DecodePicAtrSj(self, sj);
    *out = chunk->size - SjRead(sj);
    SjDelete(sj);
    return ret;
}

/* SJ object method table (same layout as the retail SJRBF/SJMEM vtable:
 * slot 6 = GetChunk @0x18, slot 7 = UngetChunk @0x1C). */
struct MpvSjVtbl {
    void (*reserved0)(void* self);
    void (*reserved1)(void* self);
    void (*reserved2)(void* self);
    void (*destroy)(void* self);
    void* (*getUuid)(void* self);
    void (*reset)(void* self);
    int (*getChunk)(void* self, int mode, int size, void* out);   /* 0x18 */
    int (*ungetChunk)(void* self, int mode, void* chunk);         /* 0x1C */
};

#define MPV_SJ_VT(obj) (*(struct MpvSjVtbl**)(obj))

s32 MPV_DecodePicAtrSj(MpvHdec* self, void* sj) {
    MpvSjChunk st;
    MpvSjChunk gc;
    s32 codec;
    s32 ret;
    if (MPVLIB_CheckHn(self))
        return MPVERR_SetCode(0, 0xFF03020C);
    self->d94 = 0;
    MPV_SJ_VT(sj)->getChunk(sj, 1, 0x7FFFFFFF, &st);
    MPV_SJ_VT(sj)->ungetChunk(sj, 1, &st);
    gc = st;
    if (mpvhdec_GetCodec(self, (s32*)&gc) == 2)
        return MPVM2V_DecodePicAtr(self, sj);
    codec = 0;
    for (;;) {
        ret = MPVHDEC_RecoverSj(self, -1, sj);
        if (ret != 0)
            return MPVERR_SetCode(self, ret);
        MPV_SJ_VT(sj)->getChunk(sj, 1, 0x7FFFFFFF, &st);
        MPV_SJ_VT(sj)->ungetChunk(sj, 1, &st);
        {
            s32 v = (st.size < 4) ? 0 : MPV_CheckDelim(st.p);
            if (v == 0 || (v & 3) != 0)
                return ret;
            if (v & 0x80) {
                ret = -2;
                return ret;
            }
            if (v == 64) {
                codec = 64;
                mpvhdec_DecShcSj(self, sj);
            } else if (v == 8) {
                codec = 8;
                mpvhdec_DecGscSj(self, sj);
            } else if (v == 4) {
                codec = 4;
                mpvhdec_DecPscSj(self, sj);
            } else if (v == 16) {
                mpvhdec_DecEscSj(self, sj, codec);
            } else if (v == 32) {
                mpvhdec_DecUdscSj(self, sj, codec);
            }
        }
    }
}

extern s32 MPV_SearchDelim(const u8* data, s32 size, s32 limit);
extern s32 MPV_CheckDelim(const u8* p);

s32 mpvhdec_GetCodec(void* self, s32* param) {
    s32 p;
    if (*(s32*)((u8*)self + 0xCFC) != 0)
        return *(s32*)((u8*)self + 0xCFC);
    p = MPV_SearchDelim(*(const u8**)param, *(s32*)((u8*)param + 4), 64);
    if (p == 0)
        return *(s32*)((u8*)self + 0xCFC);
    p = MPV_SearchDelim((const u8*)p + 4,
                        *(s32*)((u8*)param + 4) - ((const u8*)p + 4 - *(const u8**)param),
                        -1);
    if (p == 0)
        return *(s32*)((u8*)self + 0xCFC);
    {
        s32 v = MPV_CheckDelim((const u8*)p);
        if (v & 0x10) {
            *(s32*)((u8*)self + 0xCFC) = 2;
        } else if (v != 0) {
            *(s32*)((u8*)self + 0xCFC) = 1;
        }
    }
    return *(s32*)((u8*)self + 0xCFC);
}

extern u32 lbl_eu_8051C1D8[];
extern u32 lbl_eu_80602A5C[];
extern void UTY_MemcpyDword(void* dst, const void* src, s32 n);
extern void UTY_MemsetDword(void* dst, u32 val, s32 n);

/*
 * Bit-reader helpers shared by the SJ header decoders.
 *
 * The payload is consumed MSB-first straight out of the chunk buffer,
 * skipping the 4-byte start code. Reader state is kept entirely in locals:
 * `hi` is the current 32-bit window, `lo` the pending next word, `sh` the
 * bit position and `ptr` the next word to load. A read of N bits first
 * tries the in-window fast path; when the field straddles a word boundary
 * it merges `lo` into `hi` before extracting.
 */
#define HDEC_READ(val, hi, lo, sh, ptr, n)                                   \
    do {                                                                     \
        const s32 nb_ = (n);                                                 \
        if ((sh) >= 32 - nb_) {                                              \
            (sh) -= 32 - nb_;                                                \
            if ((sh) == 0) {                                                 \
                (val) = (hi) >> (32 - nb_);                                  \
                (hi) = (lo);                                                 \
            } else {                                                         \
                (hi) |= (lo) >> (nb_ - (sh));                                \
                (val) = (hi) >> (32 - nb_);                                  \
                (hi) = (lo) << (sh);                                         \
            }                                                                \
        } else {                                                             \
            (val) = (hi) >> (32 - nb_);                                      \
            (hi) <<= nb_;                                                    \
            (sh) += nb_;                                                     \
        }                                                                    \
        (lo) = *(ptr)++;                                                     \
    } while (0)

/* Decode the picture start-code (PSC) header. */
s32 mpvhdec_DecPscSj(void* selfp, void* sj) {
    MpvHdecTbl* tbl = (MpvHdecTbl*)lbl_eu_80602A90;
    MpvHdec* self = (MpvHdec*)selfp;
    MpvSjChunk rest;
    u32 p, base;
    u32 hi, lo;
    u32 v1, v2, v3;
    u32* ptr;
    s32 sh;
    u32 flag, off, idx;
    u32 mis;

    self->d58 = 3;
    ((void (*)(void*, s32, s32, MpvSjChunk*))*(void**)((char*)*(void**)sj + 0x18))(
        sj, 1, 0x7FFFFFFF, &self->chunk);

    p = (u32)(intptr_t)self->chunk.p;
    base = p & ~3u;
    mis = p - base;
    hi = *(u32*)(base + 4);
    if ((sh = mis * 8) != 0)
        hi <<= sh;
    lo = *(u32*)(base + 8);
    ptr = (u32*)(base + 12);

    /* temporal reference: 10 bits -> b70 */
    HDEC_READ(v1, hi, lo, sh, ptr, 10);
    self->b70 = v1;
    /* 3 bits -> b74 */
    HDEC_READ(v2, hi, lo, sh, ptr, 3);
    self->b74 = v2;
    /* 16 bits -> c5c */
    HDEC_READ(v3, hi, lo, sh, ptr, 16);
    self->c5c = v3;

    if (v2 == 1 || v2 == 2) {
        self->da0 = 0;
        self->d9c += 1;
        self->bc4 = self->d9c << 16;
    } else {
        self->d9c -= 1;
        self->da0 += 1;
        self->bc4 = (self->d9c << 16) | self->da0;
    }

    if (v2 == 1 || v2 == 2) {
        HDEC_READ(self->c90, hi, lo, sh, ptr, 1);
        HDEC_READ(v1, hi, lo, sh, ptr, 3);
        self->c94 = v1 - 1;
        self->c98 = 27 - (v1 - 1);
        self->c9c = 1 << (v1 - 1);
    }

    if (v2 == 3) {
        HDEC_READ(self->cb4, hi, lo, sh, ptr, 1);
        HDEC_READ(v1, hi, lo, sh, ptr, 3);
        self->cb8 = v1 - 1;
        self->cbc = 27 - (v1 - 1);
        self->cc0 = 1 << (v1 - 1);
    }

    /* picture-type dependent parameter tables */
    flag = (((u32)(3 - self->b28)) | ((u32)(self->b28 - 3))) >> 31; /* b28 != 3 */
    off = v2 * 4;
    idx = flag * 0x14;
    self->c60 = *(u32*)((char*)lbl_eu_8051C1D8 + off);
    self->c64 = *(u32*)((char*)tbl + off + idx);
    self->c70 = *(u32*)((char*)tbl + 0x28 + off + self->b20 * 0x28 + idx);
    self->c78 = *(u32*)((char*)tbl + 0xa0 + off + idx);
    self->c7c = *(u32*)((char*)tbl + 0x78 + off + idx);
    self->c80 = *(u32*)((char*)tbl + 0xc8 + off + idx);
    self->c74 = self->c7c;

    /* scan forward for the next slice start marker */
    for (;;) {
        if ((hi >> 31) != 0) {
            sh += 9;
            if (sh >= 32) {
                sh -= 32;
                hi = lo << sh;
                lo = *ptr++;
            } else {
                hi <<= 9;
            }
            if (self->chunk.size <=
                (s32)((u32)(intptr_t)ptr + ((sh + 7) >> 3) - 8) -
                    (s32)(intptr_t)p) {
                return -3;
            }
        } else {
            sh += 1;
            if (sh >= 32) {
                sh -= 32;
                ptr += 1;
            }
            break;
        }
    }

    SJ_SplitChunk(&self->chunk,
                  (s32)((u32)(intptr_t)ptr + ((sh + 7) >> 3) - 8) -
                      (s32)(intptr_t)self->chunk.p,
                  &self->chunk, &rest);
    ((void (*)(void*, s32, MpvSjChunk*))*(void**)((char*)*(void**)sj + 0x20))(
        sj, 0, &self->chunk);
    ((void (*)(void*, s32, MpvSjChunk*))*(void**)((char*)*(void**)sj + 0x1C))(
        sj, 1, &rest);
    return 0;
}

/* Decode the group-of-pictures (GSC) header. */
s32 mpvhdec_DecGscSj(MpvHdec* self, void* sj) {
    MpvSjChunk rest;
    s32 p, base;
    u32 hi, lo, nxt;
    u32* ptr;
    s32 sh;

    self->d58 = 2;
    self->b8c += 1;
    self->d9c = 0;
    self->da0 = 0;
    self->bc4 = 0;
    ((void (*)(void*, s32, s32, MpvSjChunk*))*(void**)((char*)*(void**)sj + 0x18))(
        sj, 1, 0x7FFFFFFF, &self->chunk);

    p = (s32)(intptr_t)self->chunk.p;
    base = p & ~3;
    hi = *(u32*)(base + 4);
    sh = (p - base) * 8;
    if (sh != 0) hi <<= sh;
    lo = *(u32*)(base + 8);
    ptr = (u32*)(base + 12);

    /* pull 25 bits; the GOP time code fields are peeled off the window below.
     * nxt is only fetched when we started mid-word (sh >= 7): otherwise the
     * 27 consumed bits all fit in the hi/lo window already in hand. */
    if (sh >= 7) {
        sh -= 7;
        if (sh != 0) {
            hi |= lo >> (25 - sh);
            hi >>= 7;
            lo <<= sh;
        } else {
            hi >>= 7;
        }
        nxt = *ptr++;
    } else {
        lo = hi << 25;
        hi >>= 7;
        sh += 25;
    }

    self->b88 = hi & 0x3F;
    hi >>= 6;
    self->b84 = hi & 0x3F;
    hi >>= 7;
    self->b80 = hi & 0x3F;
    hi >>= 6;
    self->b7c = hi & 0x1F;
    self->b78 = hi >> 5;
    /* bit -> c54 */
    self->c54 = lo >> 31;
    if (sh == 31) {
        lo = nxt;
        sh = 0;
        ptr += 1;
    } else {
        lo <<= 1;
        sh += 1;
    }
    self->c58 = lo >> 31;

    /* one more marker bit, value unused */
    if (sh == 31) {
        sh = 0;
        ptr += 1;
    } else {
        sh += 1;
    }

    SJ_SplitChunk(&self->chunk,
                  (s32)((u32)(intptr_t)ptr + ((sh + 7) >> 3) - 8) -
                      (s32)(intptr_t)self->chunk.p,
                  &self->chunk, &rest);
    ((void (*)(void*, s32, MpvSjChunk*))*(void**)((char*)*(void**)sj + 0x20))(
        sj, 0, &self->chunk);
    ((void (*)(void*, s32, MpvSjChunk*))*(void**)((char*)*(void**)sj + 0x1C))(
        sj, 1, &rest);
    return 0;
}

/* Decode the sequence header (SHC): sizes, aspect/rate and quant matrices. */
s32 mpvhdec_DecShcSj(void* selfp, void* sj) {
    MpvHdec* self = (MpvHdec*)selfp;
    MpvSjChunk rest;
    u32* ptr;
    u32 hi;
    s32 sh;
    u32 lo;
    u32 p, base;
    u32 mis;
    u32 v;
    s32 i;

    self->d58 = 1;
    self->d00 = 0;
    self->d38 = 0;
    self->t9ac = *(u32*)((char*)lbl_eu_80602A50 + 8);
    self->b90 += 1;
    self->d9c = 0;
    self->da0 = 0;
    self->bc4 = 0;
    ((void (*)(void*, s32, s32, MpvSjChunk*))*(void**)((char*)*(void**)sj + 0x18))(
        sj, 1, 0x7FFFFFFF, &self->chunk);

    p = (u32)(intptr_t)self->chunk.p;
    base = p & ~3u;
    mis = p - base;
    hi = *(u32*)(base + 4);
    if ((sh = mis * 8) != 0)
        hi <<= sh;
    lo = *(u32*)(base + 8);
    ptr = (u32*)(base + 12);

    HDEC_READ(self->b5c, hi, lo, sh, ptr, 12); /* horizontal size */
    HDEC_READ(self->b60, hi, lo, sh, ptr, 12); /* vertical size */
    HDEC_READ(self->c44, hi, lo, sh, ptr, 4);  /* aspect ratio */
    HDEC_READ(self->b6c, hi, lo, sh, ptr, 4);  /* frame rate code */
    HDEC_READ(self->c48, hi, lo, sh, ptr, 18); /* bit rate */

    /* marker bit: consumed but its value is never stored */
    HDEC_READ(v, hi, lo, sh, ptr, 1);

    /* optional intra-quantisation matrix, zig-zag stored via the table */
    HDEC_READ(self->c50, hi, lo, sh, ptr, 1);
    if (self->c50 != 0) {
        for (i = 0; i < 64; i += 2) {
            HDEC_READ(v, hi, lo, sh, ptr, 8);
            ((u8*)self + 0x300)[(s32)(s8)((u8*)(intptr_t)self->t9ac)[i]] = (u8)v;
            HDEC_READ(v, hi, lo, sh, ptr, 8);
            ((u8*)self + 0x300)[(s32)(s8)((u8*)(intptr_t)self->t9ac)[i + 1]] = (u8)v;
        }
    } else {
        UTY_MemcpyDword((u8*)self + 0x300, (const char*)lbl_eu_80602A5C + 8, 16);
    }

    /* optional non-intra-quantisation matrix (defaults to a flat 16 table) */
    HDEC_READ(v, hi, lo, sh, ptr, 1);
    if (v != 0) {
        for (i = 0; i < 64; i += 2) {
            HDEC_READ(v, hi, lo, sh, ptr, 8);
            ((u8*)self + 0x340)[(s32)(s8)((u8*)(intptr_t)self->t9ac)[i]] = (u8)v;
            HDEC_READ(v, hi, lo, sh, ptr, 8);
            ((u8*)self + 0x340)[(s32)(s8)((u8*)(intptr_t)self->t9ac)[i + 1]] = (u8)v;
        }
    } else {
        UTY_MemsetDword((u8*)self + 0x340, 0x10101010, 16);
    }

    self->b64 = (self->b5c + 15) >> 4; /* macroblocks per line */
    self->b68 = (self->b60 + 15) >> 4; /* macroblock lines */
    self->ce4 = self->b64 * self->b68 - 1;
    self->ba4 = self->c48;
    self->ba8 = self->c4c;
    self->bb5 = (u8)self->c44;
    self->bb6 = (u8)self->c50;

    SJ_SplitChunk(&self->chunk,
                  (s32)((u32)(intptr_t)ptr + ((sh + 7) >> 3) - 8) -
                      (s32)(intptr_t)self->chunk.p,
                  &self->chunk, &rest);
    ((void (*)(void*, s32, MpvSjChunk*))*(void**)((char*)*(void**)sj + 0x20))(
        sj, 0, &self->chunk);
    ((void (*)(void*, s32, MpvSjChunk*))*(void**)((char*)*(void**)sj + 0x1C))(
        sj, 1, &rest);
    return 0;
}

s32 mpvhdec_DecEscSj(void* selfp, void* sj, s32 codec) {
    MpvHdec* self = (MpvHdec*)selfp;
    MpvSjChunk rest;
    ((void (*)(void*, s32, s32, MpvSjChunk*))*(void**)((char*)*(void**)sj + 0x18))(
        sj, 1, 0x7FFFFFFF, &self->chunk);
    {
        s32 p = (s32)(intptr_t)self->chunk.p;
        s32 a = p & ~3;
        SJ_SplitChunk(&self->chunk, a + ((((p - a) * 8) + 7) >> 3) + 4 - p, &self->chunk, &rest);
    }
    ((void (*)(void*, s32, MpvSjChunk*))*(void**)((char*)*(void**)sj + 0x20))(
        sj, 0, &self->chunk);
    ((void (*)(void*, s32, MpvSjChunk*))*(void**)((char*)*(void**)sj + 0x1C))(
        sj, 1, &rest);
    MPV_GoNextDelimSj(sj);
    return 0;
}

s32 mpvhdec_DecUdscSj(void* selfp, void* sj, s32 codec) {
    MpvHdec* self;
    MpvSjChunk rest;
    s32 p;
    s32 a;
    s32 r;
    ((void (*)(void*, s32, s32, MpvSjChunk*))*(void**)((char*)*(void**)sj + 0x18))(
        sj, 1, 0x7FFFFFFF, &((MpvHdec*)selfp)->chunk);
    self = (MpvHdec*)selfp;
    p = (s32)(intptr_t)self->chunk.p;
    a = p & ~3;
    r = mpvhdec_AnalyUd(self, (const u8*)(intptr_t)p, self->chunk.size);
    SJ_SplitChunk(&self->chunk,
                  a + ((((p - a) * 8) + 7) >> 3) + 4 - (s32)(intptr_t)self->chunk.p,
                  &self->chunk, &rest);
    ((void (*)(void*, s32, MpvSjChunk*))*(void**)((char*)*(void**)sj + 0x20))(
        sj, 0, &self->chunk);
    ((void (*)(void*, s32, MpvSjChunk*))*(void**)((char*)*(void**)sj + 0x1C))(
        sj, 1, &rest);
    MPV_GoNextDelimSj(sj);
    return r;
}

extern s32 mpvhdec_DecSeqUdsc(void* self, const u8* p, s32 size);

s32 mpvhdec_AnalyUd(void* self, const u8* p, s32 size) {
    MpvHdec* h = (MpvHdec*)self;
    s32 found;
    s32 ret = 0;
    s32 codec = h->d58;
    s32 i;

    found = 0;

    /* scan past the user-data payload to the next start-code delimiter */
    for (i = 4; i < size - 3; i++) {
        if (MPV_CheckDelim(p + i) != 0)
            break;
    }
    if (i == size - 3)
        found = -1;

    if (codec == 1) {
        ret = mpvhdec_DecSeqUdsc(self, p, i);
    }
    {
        void* fn = h->ud[codec].udsc;
        if (fn != NULL) {
            MpvSjChunk st;
            MpvSjChunk rest;
            ((void (*)(void*, s32, s32, MpvSjChunk*))*(void**)((char*)*(void**)fn + 0x18))(
                fn, 0, i, &st);
            memcpy(st.p, p, st.size);
            ((void (*)(void*, s32, MpvSjChunk*))*(void**)((char*)*(void**)fn + 0x20))(
                fn, 1, &st);
            if (st.size < i) {
                ((void (*)(void*, s32, s32, MpvSjChunk*))*(void**)((char*)*(void**)fn + 0x18))(
                    fn, 0, i - st.size, &rest);
                memcpy(rest.p, p + st.size, rest.size);
                ((void (*)(void*, s32, MpvSjChunk*))*(void**)((char*)*(void**)fn + 0x20))(
                    fn, 1, &rest);
            }
            /* notify callback only runs when the reader object exists */
            if (h->ud[codec].notify != NULL) {
                ((void (*)(void*, s32))h->ud[codec].notify)(h->ud[codec].ctx, codec);
            }
        }
    }
    if (codec == 3) {
        if (h->d8c != NULL) {
            /* snapshot at most i bytes of the user data into the picture-user buffer */
            s32 n = (s32)h->d90;
            if (i < n)
                n = i;
            h->d94 = n;
            memcpy(h->d8c, p, n);
        }
    }
    if (ret != 0)
        found = ret;
    return found;
}

extern char lbl_eu_8051C1F0[];
extern int strncmp(const char* a, const char* b, unsigned long n);
extern int atoi(const char* s);

s32 mpvhdec_DecSeqUdsc(void* selfp, const u8* p, s32 size) {
    MpvHdec* self = (MpvHdec*)selfp;
    s32 ret = 0;
    s32 i = 0;
    while (i < size - 4) {
        if (strncmp((const char*)(p + i + 4), lbl_eu_8051C1F0, 7) == 0) {
            if (atoi((const char*)(p + i + 4) + 16) == 0)
                self->d38 = 0;
            else
                self->d38 = 3;
        }
        if (strncmp((const char*)(p + i + 4), lbl_eu_8051C1F0 + 8, 7) == 0) {
            self->d00 = atoi((const char*)(p + i + 4) + 16);
            self->d04 = atoi((const char*)(p + i + 4) + 24);
            self->d08 = atoi((const char*)(p + i + 4) + 32);
        }
        if (MPV_CheckDelim(p + i + 4) == 0)
            i++;
        else
            break;
    }
    if (self->d00 == 8)
        ret = -1;
    if (self->d00 == 9)
        ret = -1;
    return ret;
}

s32 MPV_GoNextDelimSj(void* self) {
    MpvSjChunk st;
    MpvSjChunk rest;
    s32 ret;
    for (;;) {
        ((void (*)(void*, s32, s32, MpvSjChunk*))*(void**)((char*)*(void**)self + 0x18))(
            self, 1, 0x7FFFFFFF, &st);
        if (st.size < 4) {
            ((void (*)(void*, s32, MpvSjChunk*))*(void**)((char*)*(void**)self + 0x1C))(
                self, 1, &st);
            ret = 0;
            break;
        }
        {
            s32 found = MPV_SearchDelim((const u8*)st.p, st.size, -1);
            if (found == 0) {
                SJ_SplitChunk(&st, st.size - 3, &st, &rest);
                ((void (*)(void*, s32, MpvSjChunk*))*(void**)((char*)*(void**)self + 0x20))(
                    self, 0, &st);
                ((void (*)(void*, s32, MpvSjChunk*))*(void**)((char*)*(void**)self + 0x1C))(
                    self, 1, &rest);
                continue;
            }
            {
                s32 r = MPV_CheckDelim((const u8*)found);
                SJ_SplitChunk(&st, found - (s32)(intptr_t)st.p, &st, &rest);
                ((void (*)(void*, s32, MpvSjChunk*))*(void**)((char*)*(void**)self + 0x20))(
                    self, 0, &st);
                ((void (*)(void*, s32, MpvSjChunk*))*(void**)((char*)*(void**)self + 0x1C))(
                    self, 1, &rest);
                ret = r;
                break;
            }
        }
    }
    return ret;
}


s32 MPV_MoveChunk(void* self, s32 b, s32 c) {
    s32 sp[2];
    s32 n = (b == 0) ? 1 : 0;
    ((s32 (*)(void*, s32, s32, s32*))*(void**)((u8*)*(void**)((u8*)self) + 0x18))(self, b, c, sp);
    ((s32 (*)(void*, s32, s32*))*(void**)((u8*)*(void**)((u8*)self) + 0x20))(self, n, sp);
    return sp[1];
}
