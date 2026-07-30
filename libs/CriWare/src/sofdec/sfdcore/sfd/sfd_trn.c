// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_trn
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void SFTRN_Init() {}

void SFTRN_InitHn() {}

void sftrn_BuildAll() {}

void sftrn_BuildSystem() {}

void SFTRN_CallTrSetup() {}

int SFTRN_CallTrtTrif(void* self, int idx, int funcIdx, int* arg4, int arg5) {
    void** table = *(void***)((u8*)self + idx * 0x44 + 0x1fe4);
    if (table == NULL) return 0;
    typedef int (*CallTrtFunc)(void*, int*, int, int);
    return ((CallTrtFunc)table[funcIdx])(self, arg4, arg5, 0);
}

void SFTRN_SetPrepFlg(void* self, u32 idx, u32 val) {
    *(u32*)((u8*)self + 0x1fd8 + idx * 0x44) = val;
}

u32 SFTRN_GetPrepFlg(void* self, u32 idx) {
    return *(u32*)((u8*)self + 0x1fd8 + idx * 0x44);
}

void SFTRN_SetTermFlg(void* self, u32 idx, u32 val) {
    *(u32*)((u8*)self + 0x1fdc + idx * 0x44) = val;
}

u32 SFTRN_GetTermFlg(void* self, u32 idx) {
    return *(u32*)((u8*)self + 0x1fdc + idx * 0x44);
}

u32 SFTRN_IsSetup(void* self, u32 idx) {
    s32 val = *(s32*)((u8*)self + idx * 0x44 + 0x1fe4);
    return val ? 1 : 0;
}
