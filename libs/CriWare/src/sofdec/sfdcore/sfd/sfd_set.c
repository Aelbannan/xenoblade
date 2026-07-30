// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_set
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

s32 SFLIB_CheckHn(void* h);
s32 SFLIB_SetErr(s32 val, u32 err_code);
u32 SFD_GetHnStat(void* self) {
    if (SFLIB_CheckHn(self))
        SFLIB_SetErr(0, 0xff000111);
    return *(u32*)((u8*)self + 0x54);
}

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
