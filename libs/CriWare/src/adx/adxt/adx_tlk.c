// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_tlk
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void ADXT_Create() {}

void adxt_Create() {}

void ADXT_Destroy() {}

void adxt_Destroy() {}

void ADXT_DestroyAll() {}

void adxt_start_sj() {}

void adxt_start_stm() {}

void ADXT_StartSj() {}

void adxt_StopWithoutLsc() {}

void ADXT_Stop() {}

void ADXT_GetStat() {}

void adxt_GetTimeSfreq2() {}

void ADXT_GetTime() {}

void adxt_GetTime() {}

void ADXT_GetTimeReal() {}

void ADXT_GetNumSmpl() {}

void ADXT_GetSfreq() {}

void ADXT_GetNumChan() {}

void ADXT_SetOutPan() {}

void adxt_SetOutPan() {}

void ADXT_GetOutPan() {}

void ADXT_SetOutVol() {}

void ADXT_GetOutVol() {}

void ADXT_SetDefSvrFreq() {}

void ADXT_SetSvrFreq() {}

void ADXT_GetNumSctIbuf() {}

void ADXT_GetNumSmplObuf() {}

void ADXT_SetAutoRcvr() {}

void ADXT_ExecServer() {}

void adxt_ExecServer() {}

void ADXT_GetErrCode() {}

void adxt_SetLpFlg() {}

void ADXT_Pause() {}

void adxt_Pause() {}

void ADXT_SetTranspose(void* self) {
    ADXCRS_Enter();
    ADXCRS_Leave();
}

void ADXT_GetTranspose(void* self) {
    ADXCRS_Enter();
    ADXCRS_Leave();
}

void ADXT_TermSupply() {}

void ADXT_DiscardSmpl() {}

void ADXT_SetTimeOfst() {}

void ADXT_SetLnkSw() {}

void ADXT_EntryFltFunc(void* this_, void* arg1, void* arg2) {
    ADXCRS_Enter();
    ADXSJD_EntryFltFunc(*(void**)((char*)this_ + 4), arg1, arg2);
    ADXCRS_Leave();
}

void ADXT_IsHeader() {}

s32 ADXT_IsEndcode(void* self, s32 idx, u32* out) {
    if (idx < 2) return 0;
    if (*(u16*)self != 0x8001) return 0;
    *out = idx;
    return 1;
}

void ADXT_InsertSilence() {}

void adxt_InsertSilence() {}
