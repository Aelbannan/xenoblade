// Auto-scaffolded catalog TU for CriWare/src/adx/ahx/ahx_cmn
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// name-guess: `ahx_atbl.c` records `ahxdcd_atbl` as the old name of this
// selected-table state; JP keeps it as a 4-byte local (lbl_805E9B08).
static u32 ahxdcd_atbl;

void AHXDCD_SetupAtbl(u32 val) { ahxdcd_atbl = val; }

u32 AHXCMN_SetAlcInfTbl(void* self, u32* out) {
    *out = ahxdcd_atbl;
    return 0x1e;
}
