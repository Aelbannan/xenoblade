// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/mpv/mpv_err
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void MPVERR_Init(void) {}

void MPVERR_InitErrInf(void* self) {
    *(u32*)((u8*)self) = 0;
    *(u32*)((u8*)self + 4) = 0;
    *(u32*)((u8*)self + 8) = 0;
    *(u32*)((u8*)self + 0xc) = 0;
    *(u32*)((u8*)self + 0x10) = 0;
}

void MPV_SetErrFunc() {}

void MPVERR_SetCode() {}
