// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/mpv/mpv_lib
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void MPV_Init() {}

void mpvlib_ChkFatal() {}

void mpvlib_InitClip0255() {}

void mpvlib_InitPicAtr() {}

void MPV_Finish() {}

void MPV_Create() {}

void mpvlib_InitHn() {}

void MPV_GetDctCnt(void* self, u32* out1, u32* out2) {
    *out1 = *(u32*)((u8*)self + 0xa10);
    *out2 = *(u32*)((u8*)self + 0xa14);
}

void MPV_Destroy() {}

void MPV_SetCond() {}

void MPV_GetCond() {}

void MPVM2V_SetMbCb(void* self, void* a, void* b, void* c);
void MPV_SetMbCb(void* self, void* a, void* b, void* c) {
    *(void**)((u8*)self + 0xb50) = a;
    *(void**)((u8*)self + 0xb54) = b;
    *(void**)((u8*)self + 0xb58) = c;
    MPVM2V_SetMbCb(self, a, b, c);
}

void MPVLIB_CheckHn() {}
