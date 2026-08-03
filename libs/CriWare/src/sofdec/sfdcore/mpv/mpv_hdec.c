// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/mpv/mpv_hdec
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern void SJ_SplitChunk(const void* src, int size, void* dst1, void* dst2);

typedef struct MpvSjChunk {
    const u8* p;
    s32 size;
} MpvSjChunk;

s32 MPV_GoNextDelimSj(void* self);

void MPVHDEC_Init() {}

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

void MPV_DecodePicAtrSj() {}

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

void mpvhdec_DecShcSj() {}

void mpvhdec_DecGscSj() {}

void mpvhdec_DecPscSj() {}

s32 mpvhdec_DecEscSj(void* self, void* sj) {
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

void mpvhdec_DecUdscSj() {}

void mpvhdec_AnalyUd() {}

void mpvhdec_DecSeqUdsc() {}

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

void MPVHDEC_RecoverSj() {}

void MPV_MoveChunk() {}
