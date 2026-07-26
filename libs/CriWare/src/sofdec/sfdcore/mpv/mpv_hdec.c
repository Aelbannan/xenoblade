// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/mpv/mpv_hdec
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

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

void mpvhdec_GetCodec() {}

void mpvhdec_DecShcSj() {}

void mpvhdec_DecGscSj() {}

void mpvhdec_DecPscSj() {}

void mpvhdec_DecEscSj() {}

void mpvhdec_DecUdscSj() {}

void mpvhdec_AnalyUd() {}

void mpvhdec_DecSeqUdsc() {}

void MPV_GoNextDelimSj() {}

void MPVHDEC_RecoverSj() {}

void MPV_MoveChunk() {}
