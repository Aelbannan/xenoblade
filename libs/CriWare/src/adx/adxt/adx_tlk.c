// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_tlk
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

typedef void (*ADX_VFN1)(void*);
typedef s32 (*ADX_VFN2)(void*, s32);

extern void ADXCRS_Enter(void);
extern void ADXCRS_Leave(void);
extern void ADXCRS_Lock(void);
extern void ADXCRS_Unlock(void);
extern void ADXERR_CallErrFunc1_(const char*);
extern char lbl_eu_805162F8[];
extern float lbl_eu_805162F0;
extern u8 lbl_eu_805E26E8[];

extern void ADXSTM_ReleaseFileNw(void*);
extern void LSC_Stop(void*);
extern void ADXAMP_Stop(void*);
extern u32 ADXSJD_GetSfreq(void*);
extern u32 ADXSJD_GetTotalNumSmpl(void*);
extern u32 ADXSJD_GetNumChan(void*);
extern void ADXSJD_Stop(void*);
extern void ADXRNA_SetTransSw(void*, s32);
extern void ADXRNA_SetPlaySw(void*, s32);
extern void ADXRNA_SetOutVol(void*, s32);
extern s32 ADX_DecodeInfo(const u8* info, s32 size, s16* outA, u8* outB, u8* outC,
                         u8* outD, u8* outE, u32* outF, u32* outG, u32* outH);

extern void* adxt_Create(void* work, void* buf, void* arg3);

void* ADXT_Create(void* work, void* buf, void* arg3) {
    void* ret;
    ADXCRS_Enter();
    ret = adxt_Create(work, buf, arg3);
    ADXCRS_Leave();
    return ret;
}

extern void adxt_Destroy(void* self);

void ADXT_Destroy(void* self) {
    ADXCRS_Enter();
    adxt_Destroy(self);
    ADXCRS_Leave();
}

void ADXT_DestroyAll() {
    s32 i;
    u8* p;
    ADXCRS_Enter();
    p = lbl_eu_805E26E8;
    for (i = 0; i < 16; i++) {
        if ((s8)*p == 1) {
            adxt_Destroy(p);
        }
        p += 0xC4;
    }
    ADXCRS_Leave();
}

extern void adxt_start_sj(void* self, void* sj);

void adxt_start_stm() {}

void adxt_StopWithoutLsc(void* self);

void ADXT_StartSj(void* self, void* sj) {
    ADXCRS_Enter();
    if (self == NULL || sj == NULL) {
        ADXERR_CallErrFunc1_(lbl_eu_805162F8 + 0xCC);
    } else {
        if (self == NULL) {
            ADXERR_CallErrFunc1_(lbl_eu_805162F8 + 0xA7);
        } else {
            if (((void**)self)[2] != NULL) {
                ADXSTM_ReleaseFileNw(((void**)self)[2]);
            }
            ADXCRS_Lock();
            if ((s8)((u8*)self)[2] == 4) {
                LSC_Stop(((void**)self)[0x25]);
                if (((void**)self)[5] != NULL) {
                    ADX_VFN1 fn = ((ADX_VFN1*)*(void**)((void**)self)[5])[5];
                    fn(((void**)self)[5]);
                }
            }
            adxt_StopWithoutLsc(self);
            ADXCRS_Unlock();
        }
        ADXCRS_Lock();
        ((u8*)self)[2] = 3;
        adxt_start_sj(self, sj);
        ((u8*)self)[0x98] = 1;
        if (((void**)self)[1] != NULL) {
            ADXSJD_SetLnkSw(((void**)self)[1], 1);
        }
        ADXCRS_Unlock();
    }
    ADXCRS_Leave();
}

void adxt_StopWithoutLsc(void* self) {
    ADXCRS_Lock();
    ADXRNA_SetTransSw(((void**)self)[3], 0);
    ADXRNA_SetPlaySw(((void**)self)[3], 0);
    ADXSJD_Stop(((void**)self)[1]);
    if ((s8)((u8*)self)[2] == 2) {
        if (((void**)self)[5] != NULL) {
            void* obj = ((void**)self)[5];
            ((void**)self)[5] = NULL;
            ((ADX_VFN1*)*(void**)obj)[3](obj);
        }
    }
    if (((void**)self)[0x1D] != NULL) {
        ADXAMP_Stop(((void**)self)[0x1D]);
    }
    ((void**)self)[5] = NULL;
    ((u8*)self)[1] = 0;
    ((u8*)self)[0xA8] = 0;
    ADXCRS_Unlock();
}

void ADXT_Stop(void* self) {
    ADXCRS_Enter();
    if (self == NULL) {
        ADXERR_CallErrFunc1_(lbl_eu_805162F8 + 0xA7);
    } else {
        if (((void**)self)[2] != NULL) {
            ADXSTM_ReleaseFileNw(((void**)self)[2]);
        }
        ADXCRS_Lock();
        if ((s8)((u8*)self)[2] == 4) {
            LSC_Stop(((void**)self)[0x25]);
            if (((void**)self)[5] != NULL) {
                ADX_VFN1 fn = ((ADX_VFN1*)*(void**)((void**)self)[5])[5];
                fn(((void**)self)[5]);
            }
        }
        adxt_StopWithoutLsc(self);
        ADXCRS_Unlock();
    }
    ADXCRS_Leave();
}

s32 ADXT_GetStat(void* self) {
    s32 stat;
    ADXCRS_Enter();
    if (self == NULL) {
        ADXERR_CallErrFunc1_(lbl_eu_805162F8 + 0xF4);
        stat = -1;
    } else {
        stat = *(s8*)((u8*)self + 1);
    }
    ADXCRS_Leave();
    return stat;
}

void adxt_GetTimeSfreq2() {}

extern void adxt_GetTime(void*, void*, void*);

void ADXT_GetTime(void* a, void* b, void* c) {
    ADXCRS_Enter();
    adxt_GetTime(a, b, c);
    ADXCRS_Leave();
}

s32 ADXT_GetTimeReal(void* self) {
    s32 t2, t1;
    s32 ret;
    float ratio;
    ADXCRS_Enter();
    adxt_GetTime(self, &t1, &t2);
    ratio = (float)t1 / (float)t2;
    ret = (s32)(ratio * lbl_eu_805162F0);
    ADXCRS_Leave();
    return ret;
}

s32 ADXT_GetNumSmpl(void* self) {
    s32 ret;
    ADXCRS_Enter();
    if (self == NULL) {
        ADXERR_CallErrFunc1_(lbl_eu_805162F8 + 0x173);
        ret = -1;
    } else if ((s8)((u8*)self)[1] >= 2) {
        ret = (s32)ADXSJD_GetTotalNumSmpl(((void**)self)[1]);
    } else {
        ret = 0;
    }
    ADXCRS_Leave();
    return ret;
}

s32 ADXT_GetSfreq(void* self) {
    s32 ret;
    ADXCRS_Enter();
    if (self == NULL) {
        ADXERR_CallErrFunc1_(lbl_eu_805162F8 + 0x19E);
        ret = -1;
    } else if ((s8)((u8*)self)[1] >= 2) {
        ret = (s32)ADXSJD_GetSfreq(((void**)self)[1]);
    } else {
        ret = 0;
    }
    ADXCRS_Leave();
    return ret;
}

s32 ADXT_GetNumChan(void* self) {
    s32 ret;
    ADXCRS_Enter();
    if (self == NULL) {
        ADXERR_CallErrFunc1_(lbl_eu_805162F8 + 0x1C7);
        ret = -1;
    } else if ((s8)((u8*)self)[1] >= 2) {
        ret = (s32)ADXSJD_GetNumChan(((void**)self)[1]);
    } else {
        ret = 0;
    }
    ADXCRS_Leave();
    return ret;
}

extern void adxt_SetOutPan(void*, void*, void*);

void ADXT_SetOutPan(void* a, void* b, void* c) {
    ADXCRS_Enter();
    adxt_SetOutPan(a, b, c);
    ADXCRS_Leave();
}

s32 ADXT_GetOutPan(void* self, s32 ch) {
    s32 ret;
    ADXCRS_Enter();
    if (self == NULL) {
        ADXERR_CallErrFunc1_(lbl_eu_805162F8 + 0x2CA);
        ret = 0;
    } else {
        ret = *(s16*)((u8*)self + 0x42 + ch * 2);
    }
    ADXCRS_Leave();
    return ret;
}

extern s16 ADXSJD_GetDefOutVol(void*);
extern void ADXRNA_SetOutPan(void*, s32);

void ADXT_SetOutVol(void* self, s32 vol) {
    s32 defvol;
    ADXCRS_Enter();
    if (self == NULL) {
        ADXERR_CallErrFunc1_(lbl_eu_805162F8 + 0x350);
    } else {
        *(s16*)((u8*)self + 0x40) = (s16)vol;
        if ((s8)((u8*)self)[0xA9] == 1) {
            defvol = ADXSJD_GetDefOutVol(((void**)self)[1]);
        } else {
            defvol = 0;
        }
        ADXRNA_SetOutVol(((void**)self)[3], *(s16*)((u8*)self + 0x40) + defvol);
    }
    ADXCRS_Leave();
}

s32 ADXT_GetOutVol(void* self) {
    s32 vol;
    ADXCRS_Enter();
    if (self == NULL) {
        ADXERR_CallErrFunc1_(lbl_eu_805162F8 + 0x37A);
        vol = 0;
    } else {
        vol = *(s16*)((u8*)self + 0x40);
    }
    ADXCRS_Leave();
    return vol;
}

extern u32 lbl_eu_805E4F18;
extern u32 lbl_eu_805E4F1C;

void ADXT_SetDefSvrFreq(void* self) {
    ADXCRS_Enter();
    lbl_eu_805E4F18 = (u32)self;
    lbl_eu_805E4F1C = (u32)self;
    ADXCRS_Leave();
}

void ADXT_SetSvrFreq() {}

s32 ADXT_GetNumSctIbuf(void* self) {
    s32 ret;
    if (self == NULL) {
        ADXERR_CallErrFunc1_(lbl_eu_805162F8 + 0x45A);
        ret = -1;
    } else if (((void**)self)[5] != NULL) {
        ADX_VFN2 fn = ((ADX_VFN2*)*(void**)((void**)self)[5])[9];
        ret = fn(((void**)self)[5], 1) / 2048;
    } else {
        ret = 0;
    }
    return ret;
}

s32 ADXT_GetNumSmplObuf(void* self, s32 ch) {
    void* n;
    if (self == NULL || ch < 0) {
        ADXERR_CallErrFunc1_(lbl_eu_805162F8 + 0x11C);
        return -1;
    }
    n = ((void**)self)[6 + ch];
    if (n != 0) {
        ADX_VFN2 fn = ((ADX_VFN2*)*(void**)n)[9];
        s32 v = fn((void*)n, 1);
        return (v + (v < 0)) >> 1;
    }
    return 0;
}

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

s32 ADXT_GetErrCode(void* self) {
    s32 err;
    ADXCRS_Enter();
    if (self == NULL) {
        ADXERR_CallErrFunc1_(lbl_eu_805162F8 + 0x513);
        err = -1;
    } else {
        err = *(s16*)((u8*)self + 0x60);
    }
    ADXCRS_Leave();
    return err;
}

void adxt_SetLpFlg() {}

extern void adxt_Pause(void* self, int pause);

void ADXT_Pause(void* self, int pause) {
    ADXCRS_Enter();
    adxt_Pause(self, pause);
    ADXCRS_Leave();
}

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

s32 ADXT_IsHeader(void* data, s32 size, s32* out) {
    s16 outA;
    u8 outB, outC, outD, outE;
    u32 outF, outG, outH;
    if (size < 2) return 0;
    if (*(u16*)data != 0x8000) return 0;
    if (ADX_DecodeInfo(data, size, &outA, &outB, &outC, &outD, &outE,
                      &outF, &outG, &outH) < 0) {
        return 0;
    }
    *out = outA;
    return 1;
}

s32 ADXT_IsEndcode(void* self, s32 idx, u32* out) {
    if (idx < 2) return 0;
    if (*(u16*)self != 0x8001) return 0;
    *out = idx;
    return 1;
}

extern s32 adxt_InsertSilence(void* self, s32 ch, s32 smpl);

s32 ADXT_InsertSilence(void* self, s32 ch, s32 smpl) {
    s32 ret;
    ADXCRS_Enter();
    ret = adxt_InsertSilence(self, ch, smpl);
    ADXCRS_Leave();
    return ret;
}

