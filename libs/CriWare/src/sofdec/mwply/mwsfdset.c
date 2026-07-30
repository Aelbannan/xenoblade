// Auto-scaffolded catalog TU for CriWare/src/sofdec/mwply/mwsfdset
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void SFD_SetCond(void* a, u32 b, u32 c);
void MWSFD_SetAudioSw(void* self, u32 sw) {
    SFD_SetCond(*(void**)((u8*)self + 0x58), 6, sw);
}

void SFD_SetCond(void* a, u32 b, u32 c);
void MWSFD_SetVideoSw(void* self, u32 sw) {
    SFD_SetCond(*(void**)((u8*)self + 0x58), 5, sw);
}

void mwPlyGetSfdHn() {}

void SFD_SetCond();
void MWSFD_SetCond(void* self, u32 sw) {
    void* obj = (self != NULL) ? *(void**)((u8*)self + 0x58) : NULL;
    SFD_SetCond(obj, sw);
}

u32 SFD_GetCond(void* a);
u32 MWSFD_GetCond(void* self) {
    return SFD_GetCond(self ? *(void**)((u8*)self + 0x58) : NULL);
}

u32 MWSFD_GetStmHn(void* self) { return *(u32*)((u8*)self + 0x5c); }

void MWSTM_SetFlowLimit(void* h, u32 limit);
void MWSFLSC_SetFlowLimit(void* self, u32 limit);
void MWSFD_SetFlowLimit(void* self, u32 limit) {
    void* h = *(void**)((u8*)self + 0x5c);
    MWSTM_SetFlowLimit(h, limit);
    MWSFLSC_SetFlowLimit(self, limit);
}

u32 MWSFD_IsEnableHndl(void* self) {
    if (self == NULL) return 0;
    return *(u32*)self;
}

void mwPlyGetStat() {}

void mwPlyGetRareStat() {}

void MWSFSET_ExecSetCyclicFrameOutput() {}

void mwPlyGetTime() {}

void mwPlySetOutVol() {}

void mwPlyGetOutVol() {}

void criware_803A2258() {}
