// Auto-scaffolded catalog TU for CriWare/src/adx/ahx/ahx_sjd
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void AHXSJD_Init() {}

void AHXSJD_Finish() {}

void AHXSJD_Create() {}

void AHXSJD_Destroy() {}

int AHXSJD_GetStat(void* self) { return (signed char)((u8*)self)[0]; }

void AHXSJD_SetInSj(void* self, u32 val) { *(u32*)((u8*)self + 0x10) = val; }

void AHXSJD_Start() {}

void AHXSJD_Stop() {}

void criware_8038CB9C() {}

void AHXSJD_ExecHndl() {}

u32 AHXSJD_GetDecDtLen(void* self) { return *(u32*)((u8*)self + 0x24); }

u32 AHXSJD_GetDecNumSmpl(void* self) {
    return *(u32*)((u8*)self + 0x2c) + *(u32*)((u8*)self + 0x28);
}

void AHXSJD_EntryFltFunc(void* self, void* func, void* ctx) {
    *(void**)((u8*)self + 0x54) = func;
    *(void**)((u8*)self + 0x58) = ctx;
}

void AHXDCD_SetExtPrm(void* a);
void AHXSJD_SetExtPrm(void* self) {
    AHXDCD_SetExtPrm(*(void**)((u8*)self + 0));
}

void AHXSJD_SetDecSmpl(void* self, u32 val) { *(u32*)((u8*)self + 0x34) = val; }

void func_8006BEE4(void* p) {
    ((u32*)p)[1] = 0;
}

void AHXSJD_SetLnkSw(void* self, int val) {
    if (val == 0) {
        *(u32*)((u8*)self + 0x38) = 0;
    } else {
        *(u32*)((u8*)self + 0x38) = 1;
    }
}
