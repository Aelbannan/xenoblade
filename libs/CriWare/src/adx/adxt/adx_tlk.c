// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_tlk
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern void ADXCRS_Enter(void);
extern void ADXCRS_Leave(void);

void ADXT_Create() {}

void adxt_Create() {}

void adxt_Destroy(void* self) {}

void ADXT_Destroy(void* self) {
    ADXCRS_Enter();
    adxt_Destroy(self);
    ADXCRS_Leave();
}

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

extern u32 lbl_eu_805E4F18;
extern u32 lbl_eu_805E4F1C;

void ADXT_SetDefSvrFreq(void* self) {
    ADXCRS_Enter();
    lbl_eu_805E4F18 = (u32)self;
    lbl_eu_805E4F1C = (u32)self;
    ADXCRS_Leave();
}

void ADXT_SetSvrFreq() {}

void ADXT_GetNumSctIbuf() {}

void ADXT_GetNumSmplObuf() {}

void ADXT_SetAutoRcvr(void* self, u32 val) {
    ADXCRS_Enter();
    *(u8*)((u8*)self + 0x6D) = (u8)val;
    ADXCRS_Leave();
}

extern void adxt_ExecServer(void);

void ADXT_ExecServer(void) {
    ADXCRS_Enter();
    adxt_ExecServer();
    ADXCRS_Leave();
}

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

void ADXT_TermSupply(void* self) {
    ADXCRS_Enter();
    ADXSJD_TermSupply(*(void**)((u8*)self + 4));
    ADXCRS_Leave();
}

void ADXT_DiscardSmpl() {}

void ADXT_SetTimeOfst(void* self, u32 val) {
    ADXCRS_Enter();
    *(u32*)((u8*)self + 0x88) = val;
    ADXCRS_Leave();
}

void ADXT_SetLnkSw(void* self, u32 val) {
    ADXCRS_Enter();
    *(u8*)((u8*)self + 0x98) = (u8)val;
    if (*(void**)((u8*)self + 4) != NULL) {
        ADXSJD_SetLnkSw(*(void**)((u8*)self + 4), val);
    }
    ADXCRS_Leave();
}

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
