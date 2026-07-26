// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/mpv/mpv_hdec
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-803a9e88
void MPVHDEC_Init() {}
// LLM-HARNESS-END: us-803a9e88

// LLM-HARNESS-BEGIN: us-803a9f8c
void MPV_SetUsrSj(void* self, u32 idx, u32 a, u32 b, u32 c) {
    void* base = (u8*)self + idx * 0xc;
    *(u32*)((u8*)base + 0xd5c) = a;
    *(u32*)((u8*)base + 0xd60) = b;
    *(u32*)((u8*)base + 0xd64) = c;
}
// LLM-HARNESS-END: us-803a9f8c

// LLM-HARNESS-BEGIN: us-803a9fa4
void MPV_SetPicUsrBuf(void* self, void* a, void* b) {
    *(void**)((u8*)self + 0xd8c) = a;
    *(void**)((u8*)self + 0xd90) = b;
    *(u32*)((u8*)self + 0xd94) = 0;
}
// LLM-HARNESS-END: us-803a9fa4

// LLM-HARNESS-BEGIN: us-803a9fb8
void MPV_GetPicUsr(void* self, u32* out_top, u32* out_bot) {
    if (out_top != NULL) {
        *out_top = *(u32*)((u8*)self + 0xd8c);
    }
    if (out_bot != NULL) {
        *out_bot = *(u32*)((u8*)self + 0xd94);
    }
}
// LLM-HARNESS-END: us-803a9fb8

// LLM-HARNESS-BEGIN: us-803a9fdc
void MPV_DecodePicAtrSj() {}
// LLM-HARNESS-END: us-803a9fdc

// LLM-HARNESS-BEGIN: us-803aa1e0
void MPV_DecodePicAtr() {}
// LLM-HARNESS-END: us-803aa1e0

// LLM-HARNESS-BEGIN: us-803aa294
void mpvhdec_GetCodec() {}
// LLM-HARNESS-END: us-803aa294

// LLM-HARNESS-BEGIN: us-803aa354
void mpvhdec_DecShcSj() {}
// LLM-HARNESS-END: us-803aa354

// LLM-HARNESS-BEGIN: us-803aa928
void mpvhdec_DecGscSj() {}
// LLM-HARNESS-END: us-803aa928

// LLM-HARNESS-BEGIN: us-803aaaf0
void mpvhdec_DecPscSj() {}
// LLM-HARNESS-END: us-803aaaf0

// LLM-HARNESS-BEGIN: us-803aaf50
void mpvhdec_DecEscSj() {}
// LLM-HARNESS-END: us-803aaf50

// LLM-HARNESS-BEGIN: us-803ab020
void mpvhdec_DecUdscSj() {}
// LLM-HARNESS-END: us-803ab020

// LLM-HARNESS-BEGIN: us-803ab118
void mpvhdec_AnalyUd() {}
// LLM-HARNESS-END: us-803ab118

// LLM-HARNESS-BEGIN: us-803ab2c8
void mpvhdec_DecSeqUdsc() {}
// LLM-HARNESS-END: us-803ab2c8

// LLM-HARNESS-BEGIN: us-803ab3c8
void MPV_GoNextDelimSj() {}
// LLM-HARNESS-END: us-803ab3c8

// LLM-HARNESS-BEGIN: us-803ab51c
void MPVHDEC_RecoverSj() {}
// LLM-HARNESS-END: us-803ab51c

// LLM-HARNESS-BEGIN: us-803ab620
void MPV_MoveChunk() {}
// LLM-HARNESS-END: us-803ab620
