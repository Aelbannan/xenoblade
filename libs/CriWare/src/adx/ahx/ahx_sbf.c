// Auto-scaffolded catalog TU for CriWare/src/adx/ahx/ahx_sbf
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern u32 lbl_eu_805E64B8;
void AHXDCD_SetupFtbl(u32 val) { lbl_eu_805E64B8 = val; }

extern u32 lbl_eu_805E64BC;
void AHXDCD_SetupWtbl(u32 val) { lbl_eu_805E64BC = val; }

void AHXSBF_Init() {}

void ahxsbf_init_filter() {}

extern u32 lbl_eu_805E64B0;
void AHXSBF_Finish(void) {
    lbl_eu_805E64B0--;
}

void AHXSBF_Create() {}

void AHXSBF_Destroy(void* self) { memset(self, 0, 0x1014); }
