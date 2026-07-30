// Auto-scaffolded catalog TU for CriWare/src/adx/ahx/ahx_sbf
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern u32 lbl_eu_805E64B8;
void AHXDCD_SetupFtbl(u32 val) { lbl_eu_805E64B8 = val; }

extern u32 lbl_eu_805E64BC;
void AHXDCD_SetupWtbl(u32 val) { lbl_eu_805E64BC = val; }

extern u32 lbl_eu_805E64B0;

void ahxsbf_init_filter(void);

void AHXSBF_Init(void) {
    if (lbl_eu_805E64B0 == 0) {
        ahxsbf_init_filter();
    }
    lbl_eu_805E64B0++;
}

void ahxsbf_init_filter(void) {}

void AHXSBF_Finish(void) {
    lbl_eu_805E64B0--;
}

void AHXSBF_Create() {}

void AHXSBF_Destroy(void* self) { memset(self, 0, 0x1014); }
