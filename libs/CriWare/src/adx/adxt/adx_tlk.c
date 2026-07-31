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
extern void ADXSTM_SetBufSize(void*, u32, u32);
extern void ADXSTM_SetEos(void*, void*);
extern void ADXSTM_EntryEosFunc(void*, void*, void*);
extern void ADXSTM_Seek(void*, s32);
extern void ADXSTM_StopNw(void*);
extern void ADXSTM_BindFileNw(void*, void*, void*, void*, u32, u32);
extern void ADXSTM_Start(void*);
extern void LSC_Stop(void*);
extern void ADXAMP_Stop(void*);
extern void ADXAMP_Start(void*);
extern void ADXSJD_SetInSj(void*, void*);
extern void ADXSJD_Start(void*);
extern u32 ADXSJD_GetSfreq(void*);
extern u32 ADXSJD_GetTotalNumSmpl(void*);
extern u32 ADXSJD_GetNumChan(void*);
extern u32 ADXSJD_GetDecNumSmpl(void*);
extern u32 ADXSJD_GetOutBps(void*);
extern u32 ADXSJD_GetDecDtLen(void*);
extern u32 ADXSJD_GetLpStartOfst(void*);
extern u32 ADXSJD_GetLpEndOfst(void*);
extern u32 ADXSJD_GetBlkSmpl(void*);
extern u32 ADXSJD_GetNumLoop(void*);
extern u32 ADXSJD_GetStat(void*);
extern void ADXSJD_Stop(void*);
extern void ADXSJD_SetLnkSw(void*, u32);
extern void ADXSJD_SetMaxDecSmpl(void*, u32);
extern void ADXSJD_TermSupply(void*);
extern void ADXSJD_EntryFltFunc(void*, void*, void*);
extern void ADXSJD_ExecServer(void);
extern u32 ADXRNA_GetNumData(void*);
extern u32 ADXRNA_GetNumRoom(void*);
extern u32 ADXRNA_DiscardData(void*, u32);
extern void ADXRNA_SetTransSw(void*, s32);
extern void ADXRNA_SetPlaySw(void*, s32);
extern void ADXRNA_SetOutVol(void*, s32);
extern void ADXRNA_ExecServer(void);
extern u32 ADXT_ExecHndl(void*);
extern void SJ_SplitChunk(void*, u32, void*, void*);
extern float __cvt_fp2unsigned(float);
extern u32 lbl_eu_805E26DC;
extern u32 lbl_eu_805E4F10;
extern u32 lbl_eu_805E4F18;
extern u32 lbl_eu_805E4F1C;
extern u32 lbl_eu_805E4F28;
extern u32 lbl_eu_805E4EF0;
extern u32 lbl_eu_80560044;
extern u32 lbl_eu_80560040;
extern float lbl_eu_805162D0;
extern double lbl_eu_805162D8;
extern s32 ADX_DecodeInfo(const u8* info, s32 size, s16* outA, u8* outB, u8* outC,
                         u8* outD, u8* outE, u32* outF, u32* outG, u32* outH);
extern void* memset(void*, int, u32);

extern void* adxt_Create(void* work, void* buf, void* arg3);

void adxt_SetLpFlg(void* self, u32 flg);
void adxt_start_sj(void* self, void* sj);
void adxt_start_stm(void* self, void* fname, void* sctOfst, void* sctLen, s32 numChan);
void adxt_StopWithoutLsc(void* self);
void adxt_Destroy(void* self);
void adxt_GetTime(void* self, u32* time, u32* sfreq);
void adxt_GetTimeSfreq2(void* self, u32* time, u32* sfreq);
void adxt_SetOutPan(void* a, void* b, void* c);
void adxt_Pause(void* self, int pause);
void adxt_ExecServer(void);

void* ADXT_Create(void* work, void* buf, void* arg3) {
    void* ret;
    ADXCRS_Enter();
    ret = adxt_Create(work, buf, arg3);
    ADXCRS_Leave();
    return ret;
}

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

void adxt_start_sj(void* self, void* sj) {
    s32 i;
    u8* p;
    s32 numChan;
    p = (u8*)self;
    numChan = (s32)(s8)((u8*)self)[3];
    for (i = 0; i < numChan; i++) {
        void* obuf = *(void**)(p + 0x18);
        void** vtbl = *(void***)obuf;
        ((void (*)(void*))vtbl[5])(obuf);
        p += 4;
    }
    ADXSJD_SetInSj(*(void**)((u8*)self + 4), sj);
    *(void**)((u8*)self + 0x10) = sj;
    ADXSJD_Start(*(void**)((u8*)self + 4));
    ((u8*)self)[1] = 1;
    *(u32*)((u8*)self + 0x4C) = 0;
    ((u8*)self)[0x71] = 0;
    *(u32*)((u8*)self + 0x8C) = 0x7FFFFFFF;
    *(u32*)((u8*)self + 0x90) = (u32)-1;
    *(u32*)((u8*)self + 0x9C) = 0;
    *(u32*)((u8*)self + 0xA4) = 0;
    *(u32*)((u8*)self + 0xA0) = lbl_eu_805E26DC;
    *(u32*)((u8*)self + 0xC0) = 0;
    if ((s8)((u8*)self)[2] == 4) {
        ADXCRS_Enter();
        adxt_SetLpFlg(self, 0);
        ADXCRS_Leave();
    }
    if (*(void**)((u8*)self + 0x74) != NULL) {
        ADXAMP_Start(*(void**)((u8*)self + 0x74));
    }
}

void adxt_start_stm(void* self, void* fname, void* sctOfst, void* sctLen, s32 numChan) {
    u32 signExt;
    u32 shiftVal;
    ADXSTM_SetBufSize(*(void**)((u8*)self + 8),
                      (u32)*(s16*)((u8*)self + 0x3E) << 11,
                      (u32)*(s16*)((u8*)self + 0x3C) << 11);
    ADXSTM_SetEos(*(void**)((u8*)self + 8), (void*)lbl_eu_80560040);
    ADXSTM_EntryEosFunc(*(void**)((u8*)self + 8), NULL, NULL);
    ADXSTM_Seek(*(void**)((u8*)self + 8), 0);
    ADXSTM_StopNw(*(void**)((u8*)self + 8));
    ADXSTM_ReleaseFileNw(*(void**)((u8*)self + 8));
    signExt = (u32)(numChan >> 31) << 11;
    shiftVal = __rlwimi(signExt, numChan, 11, 21, 31);
    ADXSTM_BindFileNw(*(void**)((u8*)self + 8), fname, sctOfst, sctLen,
                      shiftVal, numChan << 11);
    ADXSTM_Start(*(void**)((u8*)self + 8));
    adxt_start_sj(self, *(void**)((u8*)self + 0x10));
}

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

void adxt_GetTimeSfreq2(void* self, u32* time, u32* sfreq) {
    u8 stat;
    u32 obufNumSmpl;
    u32 numData;
    u32 decNumSmpl;
    stat = ((u8*)self)[1];
    if ((u8)(stat - 3) <= 1) {
        *sfreq = ADXSJD_GetSfreq(*(void**)((u8*)self + 4));
        decNumSmpl = ADXSJD_GetDecNumSmpl(*(void**)((u8*)self + 4));
        if (self == NULL) {
            ADXERR_CallErrFunc1_(lbl_eu_805162F8 + 0x11C);
            obufNumSmpl = (u32)-1;
        } else if (*(void**)((u8*)self + 0x18) != NULL) {
            void** vtbl = *(void***)*(void**)((u8*)self + 0x18);
            s32 v = ((s32 (*)(void*, s32))vtbl[9])(*(void**)((u8*)self + 0x18), 1);
            obufNumSmpl = (u32)((v + (v >> 31)) >> 1);
        } else {
            obufNumSmpl = 0;
        }
        numData = ADXRNA_GetNumData(*(void**)((u8*)self + 0xC));
        *time = decNumSmpl - (numData + obufNumSmpl) + *(u32*)((u8*)self + 0xA4);
    } else if ((s8)stat == 5) {
        *time = ADXSJD_GetTotalNumSmpl(*(void**)((u8*)self + 4));
        *sfreq = ADXSJD_GetSfreq(*(void**)((u8*)self + 4));
        {
            u32 bps = ADXSJD_GetOutBps(*(void**)((u8*)self + 4));
            *time = *time * (16 / bps);
        }
        *time += *(u32*)((u8*)self + 0xA4);
    } else {
        *time = 0;
        *sfreq = 1;
    }
}

void adxt_GetTime(void* self, u32* time, u32* sfreq) {
    u32 localTime, localSfreq;
    u8 stat;
    if (self == NULL || time == NULL || sfreq == NULL) {
        ADXERR_CallErrFunc1_(lbl_eu_805162F8 + 0x14B);
        return;
    }
    if (*(u32*)(lbl_eu_805E4EF0 + 0x20) == 0) {
        adxt_GetTimeSfreq2(self, time, sfreq);
        return;
    }
    *(float*)(lbl_eu_805E4EF0 + 0x40) = lbl_eu_805162D0;
    stat = ((u8*)self)[1];
    if ((u8)(stat - 3) <= 1) {
        if ((s8)((u8*)self)[0x72] == 0) {
            u32 diff = lbl_eu_805E26DC - *(u32*)((u8*)self + 0xA0);
            *time = *(u32*)((u8*)self + 0x9C) + diff * 100;
        } else {
            *time = *(u32*)((u8*)self + 0x9C);
        }
        adxt_GetTimeSfreq2(self, &localTime, &localSfreq);
        {
            float fTime = (float)(s32)localTime;
            float fSfreq = (float)(s32)localSfreq;
            float fSvrFreq = (float)(s32)*(u32*)(lbl_eu_805E4EF0 + 0x38);
            float fTime2 = (float)(s32)*time;
            float spd = *(float*)(lbl_eu_805E4EF0 + 0x40);
            float ratio1 = fTime / fSfreq;
            float ratio2 = fTime2 / fSvrFreq;
            *(float*)(lbl_eu_805E4EF0 + 0x40) = spd * (ratio1 - ratio2);
        }
        {
            float spd = *(float*)(lbl_eu_805E4EF0 + 0x40);
            if (spd > lbl_eu_805162D0 + 0x14 || spd < lbl_eu_805162D0 + 0x1C) {
                if (lbl_eu_80560044 == 1) {
                    u32 savedSvrFreq = *(u32*)(lbl_eu_805E4EF0 + 0x20);
                    *(u32*)(lbl_eu_805E4EF0 + 0x20) = 0;
                    adxt_GetTime(self, time, sfreq);
                    *(u32*)(lbl_eu_805E4EF0 + 0x20) = savedSvrFreq;
                    *(u32*)(lbl_eu_805E4EF0 + 0x3C) = *(u32*)(lbl_eu_805E4EF0 + 0x3C) + 1;
                }
                {
                    float fTime = (float)(s32)localTime;
                    float fSfreq = (float)(s32)localSfreq;
                    *(u32*)((u8*)self + 0x9C) = (u32)__cvt_fp2unsigned(fTime / fSfreq * (float)(s32)*time);
                    *(u32*)((u8*)self + 0xA0) = lbl_eu_805E26DC;
                }
            }
        }
    } else if ((s8)stat == 5) {
        u32 totalSmpl = ADXSJD_GetTotalNumSmpl(*(void**)((u8*)self + 4));
        u32 sf = ADXSJD_GetSfreq(*(void**)((u8*)self + 4));
        u32 bps = ADXSJD_GetOutBps(*(void**)((u8*)self + 4));
        u32 adjusted = totalSmpl * (16 / bps);
        *time = (u32)((float)(s32)adjusted / (float)(s32)sf * (float)(s32)*(u32*)(lbl_eu_805E4EF0 + 0x38));
        *time = *time + 1 + *(u32*)((u8*)self + 0x9C);
    } else {
        *time = 0;
    }
    *time += *(u32*)((u8*)self + 0x88);
    *sfreq = *(u32*)(lbl_eu_805E4EF0 + 0x38);
}

void ADXT_GetTime(void* a, void* b, void* c) {
    ADXCRS_Enter();
    adxt_GetTime(a, b, c);
    ADXCRS_Leave();
}

s32 ADXT_GetTimeReal(void* self) {
    u32 t2, t1;
    s32 ret;
    float ratio;
    ADXCRS_Enter();
    adxt_GetTime(self, &t1, &t2);
    ratio = (float)(s32)t1 / (float)(s32)t2;
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

void ADXT_SetSvrFreq(void* self, u32 freq) {
    u32 sfreq;
    u32 numLoop;
    u32 blkSmpl;
    u32 val;
    ADXCRS_Enter();
    if (self == NULL) {
        ADXERR_CallErrFunc1_(lbl_eu_805162F8 + 0x3A4);
    } else {
        *(u32*)((u8*)self + 0x38) = freq;
        lbl_eu_805E4F1C = freq;
        sfreq = ADXSJD_GetSfreq(*(void**)((u8*)self + 4));
        if (ADXSJD_GetStat(*(void**)((u8*)self + 4)) == 2) {
            sfreq = ADXSJD_GetSfreq(*(void**)((u8*)self + 4));
            numLoop = ADXSJD_GetNumLoop(*(void**)((u8*)self + 4));
            if ((s32)numLoop > 0) {
                val = sfreq / freq;
                *(u32*)((u8*)self + 0x48) = val * 4 - val;
            } else {
                val = sfreq / freq;
                *(u32*)((u8*)self + 0x48) = (s32)(val * 4 - val + ((s32)(val * 4 - val) < 0)) >> 1;
            }
            blkSmpl = ADXSJD_GetBlkSmpl(*(void**)((u8*)self + 4));
            val = blkSmpl * 2;
            *(u32*)((u8*)self + 0x48) = ((*(u32*)((u8*)self + 0x48) + val) / val) * val;
            ADXSJD_SetMaxDecSmpl(*(void**)((u8*)self + 4), *(u32*)((u8*)self + 0x48));
        }
    }
    ADXCRS_Leave();
}

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

void ADXT_ExecServer(void) {
    ADXCRS_Enter();
    adxt_ExecServer();
    ADXCRS_Leave();
}

void adxt_ExecServer(void) {
    s32 i;
    u8* p;
    u32* srv = (u32*)lbl_eu_805E4EF0;
    ADXCRS_Lock();
    if (srv[9] != 0) {
        ADXCRS_Unlock();
        return;
    }
    srv[9] = 1;
    ADXCRS_Unlock();
    if (*(void (**)(void*))&srv[0] != NULL) {
        (*(void (**)(void*))&srv[0])(*(void**)&srv[1]);
    }
    ADXSJD_ExecServer();
    srv[9] = 2;
    p = lbl_eu_805E26E8;
    for (i = 0; i < 16; i++) {
        if ((s8)*p == 1) {
            ADXT_ExecHndl(p);
        }
        p += 0xC4;
    }
    srv[9] = 3;
    if (*(void (**)(void*))&srv[4] != NULL) {
        (*(void (**)(void*))&srv[4])(*(void**)&srv[5]);
    }
    ADXRNA_ExecServer();
    if (*(void (**)(void*))&srv[6] != NULL) {
        (*(void (**)(void*))&srv[6])(*(void**)&srv[7]);
    }
    if (*(void (**)(void*))&srv[2] != NULL) {
        (*(void (**)(void*))&srv[2])(*(void**)&srv[3]);
    }
    srv[9] = 0;
}

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

void adxt_SetLpFlg(void* self, u32 flg) {
    s32 numData;
    u32 lpStart, lpEnd, lpSize;
    u32 decDtLen;
    if (self == NULL) {
        ADXERR_CallErrFunc1_(lbl_eu_805162F8 + 0x594);
        return;
    }
    if (*(void**)((u8*)self + 0x14) != NULL) {
        void** vtbl = *(void***)*(void**)((u8*)self + 0x14);
        numData = ((s32 (*)(void*, s32))vtbl[9])(*(void**)((u8*)self + 0x14), 1);
    } else {
        numData = 0;
    }
    if ((s8)((u8*)self)[2] != 4 && ((u8*)self)[0x6C] == 1 && flg == 0) {
        decDtLen = ADXSJD_GetDecDtLen(*(void**)((u8*)self + 4));
        decDtLen += numData;
        lpStart = ADXSJD_GetLpStartOfst(*(void**)((u8*)self + 4));
        lpStart = (lpStart + 0x7FF) >> 11;
        lpStart <<= 11;
        lpEnd = ADXSJD_GetLpEndOfst(*(void**)((u8*)self + 4));
        lpEnd = (lpEnd + 0x7FF) >> 11;
        lpEnd <<= 11;
        lpSize = lpEnd - lpStart;
        if ((s32)lpSize <= 0) {
            *(u32*)((u8*)self + 0xC0) = 0;
        } else {
            *(u32*)((u8*)self + 0xC0) = lpStart + ((decDtLen - lpStart) / lpSize) * lpSize;
        }
    }
    ((u8*)self)[0x6C] = (u8)flg;
}

void adxt_Pause(void* self, int pause) {
    s32 stat;
    s32 sctRemain, numData;
    u32 blkSmpl;
    if (self == NULL) {
        ADXERR_CallErrFunc1_(lbl_eu_805162F8 + 0x64A);
        return;
    }
    stat = (s32)(s8)((u8*)self)[1];
    if (pause == (s32)(s8)((u8*)self)[0x72]) {
        return;
    }
    ADXCRS_Lock();
    ((u8*)self)[0x72] = (s8)pause;
    if ((u8)(stat - 3) > 1) {
        ADXCRS_Unlock();
        return;
    }
    if (pause == 1) {
        ADXRNA_SetPlaySw(*(void**)((u8*)self + 0xC), 0);
        ADXCRS_Unlock();
        return;
    }
    if (((u8*)self)[0x73] == 1) {
        void* stm = *(void**)((u8*)self + 8);
        void* rna = *(void**)((u8*)self + 0xC);
        void* sjd = *(void**)((u8*)self + 4);
        sctRemain = ADXSTM_GetFileSct(stm) - ADXSTM_Tell(stm);
        if ((s32)*(s16*)((u8*)self + 0x3E) < sctRemain) {
            sctRemain = (s32)*(s16*)((u8*)self + 0x3E);
        }
        if (self == NULL) {
            ADXERR_CallErrFunc1_(lbl_eu_805162F8 + 0x45A);
            numData = -1;
        } else if (*(void**)((u8*)self + 0x14) != NULL) {
            void** vtbl = *(void***)*(void**)((u8*)self + 0x14);
            numData = ((s32 (*)(void*, s32))vtbl[9])(*(void**)((u8*)self + 0x14), 1) / 2048;
        } else {
            numData = 0;
        }
        if (numData > sctRemain) {
            if (self == NULL || (s32)1 < 0) {
                ADXERR_CallErrFunc1_(lbl_eu_805162F8 + 0x11C);
                numData = -1;
            } else if (*(void**)((u8*)self + 0x18) != NULL) {
                void** vtbl = *(void***)*(void**)((u8*)self + 0x18);
                s32 v = ((s32 (*)(void*, s32))vtbl[9])(*(void**)((u8*)self + 0x18), 1);
                numData = (v + (v >> 31)) >> 1;
            } else {
                numData = 0;
            }
            if (numData > 0) {
                u32 maxDec = *(u32*)((u8*)self + 0x48);
                u32 limit = 0x2000;
                if ((s32)maxDec < 0x2000) {
                    limit = maxDec;
                }
                numData = ADXRNA_GetNumData(rna);
                if (numData < (s32)limit) {
                    blkSmpl = ADXSJD_GetBlkSmpl(sjd);
                    if (ADXRNA_GetNumRoom(rna) > (s32)blkSmpl) {
                        if (ADXSJD_GetStat(sjd) == 3) {
                            ADXCRS_Unlock();
                            return;
                        }
                    }
                }
                if ((s8)((u8*)self)[0x72] == 0) {
                    ADXRNA_SetPlaySw(rna, 1);
                }
            }
        }
    }
    ADXCRS_Unlock();
}

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

void adxt_GetTime(void* self, u32* time, u32* sfreq);

void ADXT_DiscardSmpl(void* self, u32 num) {
    u32 result;
    u32 savedLock;
    u32 time, sfreq;
    ADXCRS_Enter();
    if ((s8)((u8*)self)[0x72] == 0) {
        result = 0;
    } else {
        result = ADXRNA_DiscardData(*(void**)((u8*)self + 0xC), num);
        ADXCRS_Enter();
        adxt_ExecServer();
        ADXCRS_Leave();
        savedLock = lbl_eu_805E4F10;
        lbl_eu_805E4F10 = 0;
        adxt_GetTime(self, &time, &sfreq);
        lbl_eu_805E4F10 = savedLock;
        *(u32*)((u8*)self + 0x9C) = (u32)__cvt_fp2unsigned((float)(s32)time / (float)(s32)sfreq * (float)(s32)time);
        *(u32*)((u8*)self + 0xA0) = lbl_eu_805E26DC;
    }
    ADXCRS_Leave();
}

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

s32 adxt_InsertSilence(void* self, s32 ch, s32 smpl) {
    void* sj;
    u32 chunkSize;
    u32 numBytes;
    u32 got;
    void* data;
    u32 size;
    void* rest;
    sj = *(void**)((u8*)self + 0x14);
    if (sj == NULL) {
        return 0;
    }
    chunkSize = ch * 0x12;
    numBytes = (smpl >> 5) * chunkSize;
    {
        void** vtbl = *(void***)sj;
        ((void (*)(void*, s32, u32, void**))vtbl[6])(sj, 0, numBytes, &data);
    }
    got = *(u32*)((u8*)&data + 4);
    numBytes = (got / chunkSize) * chunkSize;
    memset(data, 0, numBytes);
    SJ_SplitChunk(&data, numBytes, &data, &rest);
    {
        void** vtbl = *(void***)sj;
        ((void (*)(void*, s32, void**))vtbl[8])(sj, 1, &data);
    }
    {
        void** vtbl = *(void***)sj;
        ((void (*)(void*, s32, void**))vtbl[7])(sj, 0, &rest);
    }
    {
        void** vtbl = *(void***)sj;
        ((void (*)(void*, s32, u32, void**))vtbl[6])(sj, 0, numBytes, &data);
    }
    got = *(u32*)((u8*)&data + 4);
    numBytes = (got / chunkSize) * chunkSize;
    memset(data, 0, numBytes);
    SJ_SplitChunk(&data, numBytes, &data, &rest);
    {
        void** vtbl = *(void***)sj;
        ((void (*)(void*, s32, void**))vtbl[8])(sj, 1, &data);
    }
    {
        void** vtbl = *(void***)sj;
        ((void (*)(void*, s32, void**))vtbl[7])(sj, 0, &rest);
    }
    return numBytes;
}

s32 ADXT_InsertSilence(void* self, s32 ch, s32 smpl) {
    s32 ret;
    ADXCRS_Enter();
    ret = adxt_InsertSilence(self, ch, smpl);
    ADXCRS_Leave();
    return ret;
}

