// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfx/sfx_lib
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void SFX_Init() {}

void SFX_SetErrFn(u32 a, u32 b) {
    extern u32 lbl_eu_80619C10[];
    lbl_eu_80619C10[2] = a;
    lbl_eu_80619C10[3] = b;
}

void SFX_Create() {}

void sfx_InitHn() {}

void SFX_Destroy() {}

void SFXLIB_Error(void* a, void* b, void* msg) {
    (void)a;
    (void)b;
    extern u32 lbl_eu_80619C10[];
    u32* base = lbl_eu_80619C10;
    void (*cb)(u32, void*) = (void (*)(u32, void*))base[2];
    u32 arg = base[3];
    u32 cnt = base[4];
    base[4] = cnt + 1;
    if (cb) {
        cb(arg, msg);
    }
}

u32 SFX_SetCcirFx(void) {
    extern u32 lbl_eu_80619C10[];
    return lbl_eu_80619C10[5];
}

extern u32 lbl_eu_80619C04;
u32 SFX_GetForceSplitField(void) { return lbl_eu_80619C04; }
