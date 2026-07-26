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

void SFXLIB_Error() {}

u32 SFX_SetCcirFx(void) {
    extern u32 lbl_eu_80619C10[];
    return lbl_eu_80619C10[5];
}

u32 SFX_GetForceSplitField(void) { return 0; }
