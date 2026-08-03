// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/mpv/mpv_hdec
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern void SJ_SplitChunk(const void* src, int size, void* dst1, void* dst2);

typedef struct MpvSjChunk {
    const u8* p;
    s32 size;
} MpvSjChunk;

/* MPV header-decode work structure (retail layout, offsets from self). */
typedef struct MpvHdec {
    u8  pad00[0xB14];
    s32 b14;                  /* 0xB14 */
    u8  padB18[0xB78 - 0xB18];
    s32 b78;                  /* 0xB78 */
    s32 b7c;                  /* 0xB7C */
    s32 b80;                  /* 0xB80 */
    s32 b84;                  /* 0xB84 */
    s32 b88;                  /* 0xB88 */
    s32 b8c;                  /* 0xB8C */
    u8  padB90[0xBE8 - 0xB90];
    s32 be8;                  /* 0xBE8 */
    s32 bec;                  /* 0xBEC */
    u8  padBF0[0xCFC - 0xBF0];
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
    s32 d58;                  /* 0xD58 */
    void (*d5c[3])(void);     /* 0xD5C */
    u8  padD68[0xD8C - 0xD68];
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
    memset(&tbl->a, 0, sizeof(tbl->a));
    memset(&tbl->b, 0, sizeof(tbl->b));
    memset(&tbl->c, 0, sizeof(tbl->c));
    memset(&tbl->d, 0, sizeof(tbl->d));
    memset(&tbl->e, 0, sizeof(tbl->e));
    tbl->a[1] = MPVUMC_PpicSkipped;
    tbl->a[2] = MPVUMC_PpicSkipped;
    tbl->a[3] = MPVUMC_BpicSkipped;
    tbl->b[1] = fn_803AFDB0;
    tbl->b[2] = fn_803AFDB0;
    tbl->b[3] = fn_803AFDB0;
    tbl->b[4] = fn_803AFDB0;
    tbl->b[11] = fn_803AFDB0;
    tbl->b[12] = fn_803AFDB0;
    tbl->b[13] = fn_803AFDB0;
    tbl->b[14] = fn_803AFDB0;
    tbl->c[2] = fn_803AFFB4;
    tbl->c[3] = fn_803AFFB4;
    tbl->d[3] = fn_803B00B8;
    tbl->e[3] = fn_803B01BC;
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
extern s32 mpvhdec_DecGscSj(void* self, void* sj, s32 x);
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
    ret = (v == 0) ? -2 : -3;
    for (;;) {
        s32 t = MPV_GoNextDelimSj(sj);
        if (t == 0)
            break;
        if ((t & mask) == 0) {
            ((void (*)(void*, s32, s32, s32*))*(void**)((u8*)*(void**)((u8*)sj) + 0x18))(sj, 1, 4, sp);
            ((void (*)(void*, s32, s32*))*(void**)((u8*)*(void**)((u8*)sj) + 0x20))(sj, 0, sp);
            if (sp[1] != 4)
                break;
        } else {
            ret = 0;
            break;
        }
    }
    return ret;
}

extern void* SJMEM_Create(const void* p, s32 size);
extern s32 MPV_DecodePicAtrSj(void* self, void* sj);

s32 MPV_DecodePicAtr(void* self, const MpvSjChunk* chunk, s32* out) {
    s32 ret;
    void* sj;
    sj = SJMEM_Create(chunk->p, chunk->size);
    if (sj == NULL)
        return -1;
    ret = MPV_DecodePicAtrSj(self, sj);
    {
        s32 n = chunk->size - ((s32 (*)(void*, s32))*((void**)*(void**)sj + 9))(sj, 1);
        *out = n;
    }
    ((void (*)(void*))*((void**)*(void**)sj + 3))(sj);
    return ret;
}

s32 MPV_DecodePicAtrSj(void* self, void* sj) {
    MpvSjChunk st;
    MpvSjChunk gc;
    s32 ret;
    s32 codec;
    if (MPVLIB_CheckHn(self))
        return MPVERR_SetCode(0, 0xFF03020C);
    *(s32*)((u8*)self + 0xD94) = 0;
    ((void (*)(void*, s32, s32, MpvSjChunk*))*(void**)((char*)*(void**)sj + 0x18))(
        sj, 1, 0x7FFFFFFF, &st);
    ((void (*)(void*, s32, MpvSjChunk*))*(void**)((char*)*(void**)sj + 0x1C))(
        sj, 1, &st);
    gc = st;
    if (mpvhdec_GetCodec(self, (s32*)&gc) == 2)
        return MPVM2V_DecodePicAtr(self, sj);
    codec = 0;
    for (;;) {
        ret = MPVHDEC_RecoverSj(self, -1, sj);
        if (ret != 0)
            return MPVERR_SetCode(self, ret);
        ((void (*)(void*, s32, s32, MpvSjChunk*))*(void**)((char*)*(void**)sj + 0x18))(
            sj, 1, 0x7FFFFFFF, &st);
        ((void (*)(void*, s32, MpvSjChunk*))*(void**)((char*)*(void**)sj + 0x1C))(
            sj, 1, &st);
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
                mpvhdec_DecGscSj(self, sj, 0);
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

s32 mpvhdec_DecShcSj(void* self, void* sj) { return 0; }

s32 mpvhdec_DecGscSj(void* self, void* sj, s32 x) { return 0; }

s32 mpvhdec_DecPscSj(void* self, void* sj) { return 0; }

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
    MpvHdec* self = (MpvHdec*)selfp;
    MpvSjChunk rest;
    s32 r;
    s32 p;
    s32 a;
    ((void (*)(void*, s32, s32, MpvSjChunk*))*(void**)((char*)*(void**)sj + 0x18))(
        sj, 1, 0x7FFFFFFF, &self->chunk);
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
    s32 i;
    s32 found = 0;
    s32 ret = 0;
    s32 codec = *(s32*)((u8*)self + 0xD58);
    for (i = 4; i < size - 3; i++) {
        if (MPV_CheckDelim(p + i) != 0) break;
    }
    if (i == size - 3) found = -1;
    if (codec == 1) {
        ret = mpvhdec_DecSeqUdsc(self, p, i);
    }
    {
        void* fn = *(void**)((u8*)self + codec * 12 + 0xD5C);
        if (fn != NULL) {
            MpvSjChunk st;
            MpvSjChunk rest;
            ((void (*)(void*, s32, s32, MpvSjChunk*))*(void**)((char*)*(void**)fn + 0x18))(
                fn, 0, i, &st);
            memcpy((void*)st.p, p, st.size);
            ((void (*)(void*, s32, MpvSjChunk*))*(void**)((char*)*(void**)fn + 0x20))(
                fn, 1, &st);
            if (st.size < i) {
                ((void (*)(void*, s32, s32, MpvSjChunk*))*(void**)((char*)*(void**)fn + 0x18))(
                    fn, 0, i - st.size, &rest);
                memcpy((void*)rest.p, p + st.size, rest.size);
                ((void (*)(void*, s32, MpvSjChunk*))*(void**)((char*)*(void**)fn + 0x20))(
                    fn, 1, &rest);
            }
        }
    }
    {
        void* fn2 = *(void**)((u8*)self + codec * 12 + 0xD60);
        if (fn2 != NULL) {
            ((void (*)(void*, s32))fn2)(*(void**)((u8*)self + codec * 12 + 0xD64), codec);
        }
    }
    if (codec == 3) {
        if (*(void**)((u8*)self + 0xD8C) != NULL) {
            s32 n = *(s32*)((u8*)self + 0xD90);
            if (i < n) n = i;
            *(s32*)((u8*)self + 0xD94) = n;
            memcpy(*(void**)((u8*)self + 0xD8C), p, n);
        }
    }
    if (ret != 0) found = ret;
    return found;
}

extern char lbl_eu_8051C1F0[];
extern int strncmp(const char* a, const char* b, unsigned long n);
extern int atoi(const char* s);

s32 mpvhdec_DecSeqUdsc(void* self, const u8* p, s32 size) {
    s32 i = 0;
    s32 ret = 0;
    s32 limit = size - 4;
    while (i < limit) {
        const u8* q = p + i + 4;
        if (strncmp((const char*)q, lbl_eu_8051C1F0, 7) == 0) {
            if (atoi((const char*)q + 16) == 0)
                *(s32*)((u8*)self + 0xD38) = 0;
            else
                *(s32*)((u8*)self + 0xD38) = 3;
        }
        if (strncmp((const char*)q, lbl_eu_8051C1F0 + 8, 7) == 0) {
            *(s32*)((u8*)self + 0xD00) = atoi((const char*)q + 16);
            *(s32*)((u8*)self + 0xD04) = atoi((const char*)q + 24);
            *(s32*)((u8*)self + 0xD08) = atoi((const char*)q + 32);
        }
        if (MPV_CheckDelim(q) == 0)
            i++;
        else
            break;
    }
    if (*(s32*)((u8*)self + 0xD00) == 8 || *(s32*)((u8*)self + 0xD00) == 9)
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
