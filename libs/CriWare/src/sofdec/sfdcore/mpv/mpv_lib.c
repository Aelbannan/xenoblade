// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/mpv/mpv_lib
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-803ab68c
void MPV_Init() {}
// LLM-HARNESS-END: us-803ab68c

// LLM-HARNESS-BEGIN: us-803ab828
void mpvlib_ChkFatal() {}
// LLM-HARNESS-END: us-803ab828

// LLM-HARNESS-BEGIN: us-803ab8e0
void mpvlib_InitClip0255() {}
// LLM-HARNESS-END: us-803ab8e0

// LLM-HARNESS-BEGIN: us-803abbbc
void mpvlib_InitPicAtr() {}
// LLM-HARNESS-END: us-803abbbc

// LLM-HARNESS-BEGIN: us-803abca4
void MPV_Finish() {}
// LLM-HARNESS-END: us-803abca4

// LLM-HARNESS-BEGIN: us-803abcfc
void MPV_Create() {}
// LLM-HARNESS-END: us-803abcfc

// LLM-HARNESS-BEGIN: us-803abd7c
void mpvlib_InitHn() {}
// LLM-HARNESS-END: us-803abd7c

// LLM-HARNESS-BEGIN: us-803abf34
void MPV_GetDctCnt(void* self, u32* out1, u32* out2) {
    *out1 = *(u32*)((u8*)self + 0xa10);
    *out2 = *(u32*)((u8*)self + 0xa14);
}
// LLM-HARNESS-END: us-803abf34

// LLM-HARNESS-BEGIN: us-803abf48
void MPV_Destroy() {}
// LLM-HARNESS-END: us-803abf48

// LLM-HARNESS-BEGIN: us-803ac004
void MPV_SetCond() {}
// LLM-HARNESS-END: us-803ac004

// LLM-HARNESS-BEGIN: us-803ac0d0
void MPV_GetCond() {}
// LLM-HARNESS-END: us-803ac0d0

// LLM-HARNESS-BEGIN: us-803ac140
void MPVM2V_SetMbCb(void* self, void* a, void* b, void* c);
void MPV_SetMbCb(void* self, void* a, void* b, void* c) {
    *(void**)((u8*)self + 0xb50) = a;
    *(void**)((u8*)self + 0xb54) = b;
    *(void**)((u8*)self + 0xb58) = c;
    MPVM2V_SetMbCb(self, a, b, c);
}
// LLM-HARNESS-END: us-803ac140

// LLM-HARNESS-BEGIN: us-803ac150
void MPVLIB_CheckHn() {}
// LLM-HARNESS-END: us-803ac150
