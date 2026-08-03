// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/mpv/mpv_hdec
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern void SJ_SplitChunk(const void* src, int size, void* dst1, void* dst2);

typedef struct MpvSjChunk {
    const u8* p;
    s32 size;
} MpvSjChunk;

s32 MPV_GoNextDelimSj(void* self);
extern s32 mpvhdec_AnalyUd(void* self, const u8* p, s32 size);

extern void MPVUMC_PpicSkipped(void);
extern void MPVUMC_BpicSkipped(void);
extern void fn_803AFDB0(void);
extern void fn_803AFFB4(void);
extern void fn_803B00B8(void);
extern void fn_803B01BC(void);
extern u32 lbl_eu_80602A90[];

void MPVHDEC_Init(void) {
    memset(lbl_eu_80602A90, 0, 0x28);
    memset((u8*)lbl_eu_80602A90 + 0x28, 0, 0x50);
    memset((u8*)lbl_eu_80602A90 + 0x78, 0, 0x28);
    memset((u8*)lbl_eu_80602A90 + 0xa0, 0, 0x28);
    memset((u8*)lbl_eu_80602A90 + 0xc8, 0, 0x28);
    lbl_eu_80602A90[1] = (u32)MPVUMC_PpicSkipped;
    lbl_eu_80602A90[2] = (u32)MPVUMC_PpicSkipped;
    lbl_eu_80602A90[3] = (u32)MPVUMC_BpicSkipped;
    lbl_eu_80602A90[0xb] = (u32)fn_803AFDB0;
    lbl_eu_80602A90[0xc] = (u32)fn_803AFDB0;
    lbl_eu_80602A90[0xd] = (u32)fn_803AFDB0;
    lbl_eu_80602A90[0xe] = (u32)fn_803AFDB0;
    lbl_eu_80602A90[0x15] = (u32)fn_803AFDB0;
    lbl_eu_80602A90[0x16] = (u32)fn_803AFDB0;
    lbl_eu_80602A90[0x17] = (u32)fn_803AFDB0;
    lbl_eu_80602A90[0x18] = (u32)fn_803AFDB0;
    lbl_eu_80602A90[0x20] = (u32)fn_803AFFB4;
    lbl_eu_80602A90[0x21] = (u32)fn_803AFFB4;
    lbl_eu_80602A90[0x2b] = (u32)fn_803B00B8;
    lbl_eu_80602A90[0x35] = (u32)fn_803B01BC;
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
extern s32 MPVM2V_DecodePicAtr(void* self, s32 ret);
extern s32 MPV_CheckDelim(const u8* p);
extern s32 mpvhdec_GetCodec(void* self, s32* param);
extern s32 mpvhdec_DecShcSj(void* self, void* sj);
extern s32 mpvhdec_DecGscSj(void* self, s32 a, void* sj);
extern s32 mpvhdec_DecPscSj(void* self, void* sj);
extern s32 mpvhdec_DecEscSj(void* self, void* sj, s32 codec);
extern s32 mpvhdec_DecUdscSj(void* self, void* sj, s32 codec);
extern s32 MPVHDEC_RecoverSj(void* self, void* sj);

s32 MPV_DecodePicAtrSj(void* self, void* sj) {
    MpvSjChunk st;
    s32 ret = 0;
    s32 codec;
    if (MPVLIB_CheckHn(self)) {
        MPVERR_SetCode(0, 0xFF03020C);
        return ret;
    }
    *(s32*)((u8*)self + 0xD94) = 0;
    for (;;) {
        ((void (*)(void*, s32, s32, MpvSjChunk*))*(void**)((char*)*(void**)sj + 0x18))(
            sj, 1, 0x7FFFFFFF, &st);
        ((void (*)(void*, s32, MpvSjChunk*))*(void**)((char*)*(void**)sj + 0x1C))(
            sj, 1, &st);
        if (mpvhdec_GetCodec(self, (s32*)&st) != 2) {
            MPVHDEC_RecoverSj(self, sj);
            return ret;
        }
        codec = 0;
        ret = mpvhdec_DecGscSj(self, -1, sj);
        if (ret != 0) {
            MPVM2V_DecodePicAtr(self, ret);
            return ret;
        }
        ((void (*)(void*, s32, s32, MpvSjChunk*))*(void**)((char*)*(void**)sj + 0x18))(
            sj, 1, 0x7FFFFFFF, &st);
        ((void (*)(void*, s32, MpvSjChunk*))*(void**)((char*)*(void**)sj + 0x1C))(
            sj, 1, &st);
        {
            s32 v = (st.size < 4) ? 0 : MPV_CheckDelim(st.p);
            if (v != 0) {
                if ((v & 3) != 0) break;
                if (v & 0x80) {
                    ret = -2;
                    break;
                }
                if (v == 64) {
                    codec = 64;
                    mpvhdec_DecShcSj(self, sj);
                    continue;
                }
                if (v == 8) {
                    codec = 8;
                    mpvhdec_DecGscSj(self, (s32)sj, 0);
                    continue;
                }
                if (v == 4) {
                    codec = 4;
                    mpvhdec_DecPscSj(self, sj);
                    continue;
                }
                if (v == 16) {
                    codec = 16;
                    mpvhdec_DecEscSj(self, sj, codec);
                    continue;
                }
                if (v == 32) {
                    codec = 32;
                    mpvhdec_DecUdscSj(self, sj, codec);
                    continue;
                }
            }
        }
    }
    return ret;
}

void MPV_DecodePicAtr() {}

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

s32 mpvhdec_DecGscSj(void* self, s32 a, void* sj) { return 0; }

s32 mpvhdec_DecPscSj(void* self, void* sj) { return 0; }

s32 mpvhdec_DecEscSj(void* self, void* sj, s32 codec) {
    MpvSjChunk* chunk = (MpvSjChunk*)((u8*)self + 0xD2C);
    MpvSjChunk rest;
    ((void (*)(void*, s32, s32, MpvSjChunk*))*(void**)((char*)*(void**)sj + 0x18))(
        sj, 1, 0x7FFFFFFF, chunk);
    {
        s32 p = (s32)(intptr_t)chunk->p;
        SJ_SplitChunk(chunk, (p & ~3) + (((p & 3) * 8 + 7) / 8) + 4 - p, chunk, &rest);
    }
    ((void (*)(void*, s32, MpvSjChunk*))*(void**)((char*)*(void**)sj + 0x20))(sj, 0, chunk);
    ((void (*)(void*, s32, MpvSjChunk*))*(void**)((char*)*(void**)sj + 0x1C))(sj, 1, &rest);
    MPV_GoNextDelimSj(sj);
    return 0;
}

s32 mpvhdec_DecUdscSj(void* self, void* sj, s32 codec) {
    MpvSjChunk* chunk = (MpvSjChunk*)((u8*)self + 0xD2C);
    MpvSjChunk rest;
    s32 r;
    ((void (*)(void*, s32, s32, MpvSjChunk*))*(void**)((char*)*(void**)sj + 0x18))(
        sj, 1, 0x7FFFFFFF, chunk);
    r = mpvhdec_AnalyUd(self, chunk->p, chunk->size);
    {
        s32 p = (s32)(intptr_t)chunk->p;
        SJ_SplitChunk(chunk, (p & ~3) + (((r - (p & ~3)) * 8 + 7) / 8) + 4 - p,
                      chunk, &rest);
    }
    ((void (*)(void*, s32, MpvSjChunk*))*(void**)((char*)*(void**)sj + 0x20))(sj, 0, chunk);
    ((void (*)(void*, s32, MpvSjChunk*))*(void**)((char*)*(void**)sj + 0x1C))(sj, 1, &rest);
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

s32 MPVHDEC_RecoverSj(void* self, void* sj) { return 0; }

void MPV_MoveChunk() {}
