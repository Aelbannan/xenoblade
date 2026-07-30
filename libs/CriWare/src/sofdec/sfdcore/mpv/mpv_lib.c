// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/mpv/mpv_lib
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void MPV_Init() {}

void mpvlib_ChkFatal() {}

void mpvlib_InitClip0255() {}

void mpvlib_InitPicAtr() {}

void MPVUMC_Finish(void);
void MPVSL_Finish(void);
void MPVM2V_Finish(void);

extern u32 lbl_eu_80602B88[];

void MPV_Finish(void) {
    MPVUMC_Finish();
    MPVSL_Finish();
    MPVM2V_Finish();
    if (lbl_eu_80602B88[0x48/4] & 0x10000000) {
        u32 base = lbl_eu_80602B88[0x50/4];
        u32 i;
        for (i = 0; i < 0xDF; i++) {
            __dcbi((void*)(base + i * 0x20));
        }
    }
}

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

extern u32 lbl_eu_80602FEC;

int MPVLIB_CheckHn(void* handle) {
    if (handle == NULL) return -1;
    if (*(int*)((u8*)handle + 0xb08) != 2) return -1;
    lbl_eu_80602FEC = (u32)handle;
    return 0;
}
