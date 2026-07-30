// Auto-scaffolded catalog TU for CriWare/src/sofdec/mwply/mwsfdcre
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern void* MWSFLIB_GetLibWorkPtr(void);

void mwsfcre_CalcWorkStmBuf() {}

void mwPlyCalcWorkCprmSfd() {}

void mwsfcre_IsOuterFrmPoolUsed() {}

void MWSFCRE_SetSupplySj() {}

void mwsfcre_MallocRfb() {}

void mwsfcre_MallocTab() {}

void criware_eu_803A29E0() {}

void mwsfcre_CreateSfd() {}

void mwsfcre_AttachPicUsrBuf() {}

void MWSFCRE_SetCondSfd() {}

void MWSFCRE_ResetSfdHn() {}

void criware_8039FF34() {}

void mwPlyCreateSofdec() {}

void mwsfcre_MallocCompoWork() {}

void criware_803A09B4() {}

void mwply_Destroy() {}

uint32_t criware_803A0BA8(void* r3_this, uint32_t r4) {
    uint32_t a = *(uint32_t*)((char*)r3_this + 0x54c);
    uint32_t limit = *(uint32_t*)((char*)r3_this + 0x544);
    uint32_t sum = a + r4;
    if (sum > limit) {
        return 0;
    }
    uint32_t b = *(uint32_t*)((char*)r3_this + 0x548);
    *(uint32_t*)((char*)r3_this + 0x54c) = sum;
    *(uint32_t*)((char*)r3_this + 0x548) = b + r4;
    return b;
}

void criware_803A0BDC(void* self) {
    void* work = MWSFLIB_GetLibWorkPtr();
    void (*cb)(void*, void*) = (void(*)(void*, void*))*(void**)((u8*)work + 0x28);
    void* param = *(void**)((u8*)work + 0x30);
    cb(param, self);
}

void criware_803A0C1C() {}

void criware_803A0C20(void* self) {
    void* work = MWSFLIB_GetLibWorkPtr();
    void (*cb)(void*, void*) = (void(*)(void*, void*))*(void**)((u8*)work + 0x2C);
    void* param = *(void**)((u8*)work + 0x30);
    cb(param, self);
}
