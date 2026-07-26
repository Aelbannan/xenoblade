// Auto-scaffolded catalog TU for CriWare/src/sofdec/mwply/mwsfdset
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-803a54ac
void SFD_SetCond(void* a, u32 b, u32 c);
void MWSFD_SetAudioSw(void* self, u32 sw) {
    SFD_SetCond(*(void**)((u8*)self + 0x58), 6, sw);
}
// LLM-HARNESS-END: us-803a54ac

// LLM-HARNESS-BEGIN: us-803a54bc
void SFD_SetCond(void* a, u32 b, u32 c);
void MWSFD_SetVideoSw(void* self, u32 sw) {
    SFD_SetCond(*(void**)((u8*)self + 0x58), 5, sw);
}
// LLM-HARNESS-END: us-803a54bc

// LLM-HARNESS-BEGIN: us-803a54cc
void mwPlyGetSfdHn() {}
// LLM-HARNESS-END: us-803a54cc

// LLM-HARNESS-BEGIN: us-803a5524
void SFD_SetCond();
void MWSFD_SetCond(void* self, u32 sw) {
    void* obj = (self != NULL) ? *(void**)((u8*)self + 0x58) : NULL;
    SFD_SetCond(obj, sw);
}
// LLM-HARNESS-END: us-803a5524

// LLM-HARNESS-BEGIN: us-803a553c
u32 SFD_GetCond(void* a);
u32 MWSFD_GetCond(void* self) {
    return SFD_GetCond(self ? *(void**)((u8*)self + 0x58) : NULL);
}
// LLM-HARNESS-END: us-803a553c

// LLM-HARNESS-BEGIN: us-803a5554
u32 MWSFD_GetStmHn(void* self) { return *(u32*)((u8*)self + 0x5c); }
// LLM-HARNESS-END: us-803a5554

// LLM-HARNESS-BEGIN: us-803a555c
void MWSFD_SetFlowLimit() {}
// LLM-HARNESS-END: us-803a555c

// LLM-HARNESS-BEGIN: us-803a55a4
u32 MWSFD_IsEnableHndl(void* self) {
    if (self == NULL) return 0;
    return *(u32*)self;
}
// LLM-HARNESS-END: us-803a55a4

// LLM-HARNESS-BEGIN: us-803a55bc
void mwPlyGetStat() {}
// LLM-HARNESS-END: us-803a55bc

// LLM-HARNESS-BEGIN: us-803a5624
void mwPlyGetRareStat() {}
// LLM-HARNESS-END: us-803a5624

// LLM-HARNESS-BEGIN: us-803a56f0
void MWSFSET_ExecSetCyclicFrameOutput() {}
// LLM-HARNESS-END: us-803a56f0

// LLM-HARNESS-BEGIN: us-803a576c
void mwPlyGetTime() {}
// LLM-HARNESS-END: us-803a576c

// LLM-HARNESS-BEGIN: us-803a583c
void mwPlySetOutVol() {}
// LLM-HARNESS-END: us-803a583c

// LLM-HARNESS-BEGIN: us-803a58c8
void mwPlyGetOutVol() {}
// LLM-HARNESS-END: us-803a58c8

// LLM-HARNESS-BEGIN: us-803a59a0
void criware_803A2258() {}
// LLM-HARNESS-END: us-803a59a0
