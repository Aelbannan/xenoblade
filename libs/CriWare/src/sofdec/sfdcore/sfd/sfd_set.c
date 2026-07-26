// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_set
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void SFD_GetHnStat() {}

void SFD_SetCond() {}

void SFSET_SetCond() {}

void SFD_GetCond() {}

u32 SFSET_GetCond(void* self, u32 idx) {
    return *(u32*)((u8*)self + 0xa1c + idx * 4);
}

void SFD_SetSfdHeaderFn(void* self, void* fn, void* ctx) {
    *(void**)((u8*)self + 0xd3c) = fn;
    *(void**)((u8*)self + 0xd40) = ctx;
}

void SFD_GetSofdecHeader() {}
