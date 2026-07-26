// Auto-scaffolded catalog TU for CriWare/src/sofdec/mwply/mwsfdsfx
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void MWSFSFX_Init() {}

void mwsfsfx_SfxErrCbFn(void) {}

int MWSFSFX_CalcHnWorkSiz(void) { return 0x301f; }

void MWSFSFX_Create(void) { SFX_Create(); }

void MWSFSFX_Destroy(void) { SFX_Destroy(); }

void MWSFD_CnvFrmInfToSfx() {}

void MWSFD_IsColAdjFrame() {}

void MWSFD_IsColAdjFile() {}

void MWSFD_GetFxType() {}

void mwsfsfx_SetYcc420plnInfToSfx() {}

void SFX_SetCompoMode(void* a);
void MWSFSFX_SetCompoMode(void* self) {
    SFX_SetCompoMode(*(void**)((u8*)self + 0xC8));
}

void mwPlyFxGetCompoMode() {}

void mwPlyFxSetOutBufPitchHeight() {}

int MWSFTAG_IsUseAinfSj(void* self) {
    int v = *(s32*)((u8*)self + 0x20);
    if (v == 0 || v == 0x101) {
        return 1;
    }
    return 0;
}

void MWSFTAG_CreateAinfSj() {}

void MWSFTAG_DestroyAinfSj(void* self) {
    void* p = *(void**)((u8*)self + 0x4c0);
    if (p == NULL) return;
    void** vtable = *(void***)p;
    ((void (*)(void*))vtable[3])(p);
}

void MWSFTAG_SetAinfSj() {}

void MWSFTAG_InitTagInf(void* self) {
    *(u32*)((u8*)self + 0x4d8) = 0;
    *(u32*)((u8*)self + 0x4dc) = 0;
    *(u32*)((u8*)self + 0x4e0) = 0;
    *(u32*)((u8*)self + 0x4d4) = -1;
}

void MWSFTAG_ResetAinfSj(void* self) {
    void* p = *(void**)((u8*)self + 0x4c0);
    if (p == NULL) return;
    void** vtable = *(void***)p;
    ((void (*)(void*))vtable[5])(p);
}

void MWSFTAG_UpdateTagInf() {}

void mwsftag_GetAinfFromSj() {}

void MWSFD_GetZfrmRange() {}
