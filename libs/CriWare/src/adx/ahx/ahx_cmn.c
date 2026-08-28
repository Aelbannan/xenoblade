// Auto-scaffolded catalog TU for CriWare/src/adx/ahx/ahx_cmn
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

unsigned char lbl_eu_805E64C8[8];
void AHXDCD_SetupAtbl(u32 val) { *(u32*)lbl_eu_805E64C8 = val; }

u32 AHXCMN_SetAlcInfTbl(void* self, u32* out) {
    *out = *(u32*)lbl_eu_805E64C8;
    return 0x1e;
}
