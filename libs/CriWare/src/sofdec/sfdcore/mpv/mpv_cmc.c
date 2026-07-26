// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/mpv/mpv_cmc
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void MPVCMC_InitObj() {}

void mpvcmc_InitMcOiTa() {}

void MPVCMC_InitMcOiRt() {}

void MPVCMC_SetCcnt(void* self) {
    u32 val = *(u32*)((u8*)self + 0xb24);
    int r4;
    if (val != 0) {
        r4 = 4;
    } else {
        r4 = -1;
    }
    *(u32*)((u8*)self + 0xad4) = r4;
    *(u32*)((u8*)self + 0xaa0) = r4;
}
