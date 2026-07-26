// Auto-scaffolded catalog TU for CriWare/src/sofdec/mwply/mwsfdsl
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void mwPlyEntryFname() {}

void mwPlyStartSeamless() {}

void mwPlySetSeamlessLp() {}

void mwPlyStartFnameLp() {}

void mwPlyStartFnameRangeLp() {}

int MWSFLSC_IsFsStatErr(void) { extern int LSC_GetStat(void); return (LSC_GetStat() == 3) ? 1 : 0; }

void LSC_SetFlowLimit(void* a);
void MWSFLSC_SetFlowLimit(void* self) {
    void* obj = *(void**)((u8*)self + 0x64);
    if (obj != NULL) {
        LSC_SetFlowLimit(obj);
    }
}

void LSC_Pause(void* a);
void MWSFLSC_Pause(void* self) {
    void* obj = *(void**)((u8*)self + 0x64);
    if (obj != NULL) {
        LSC_Pause(obj);
    }
}

void mwPlyLinkStm() {}

void mwPlyExecInfiniteLoopHandle() {}
