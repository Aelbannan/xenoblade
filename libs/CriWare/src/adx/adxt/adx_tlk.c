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
extern s16 ADXSJD_GetDefPan(void*, s32);
extern void ADXSJD_SetLnkSw(void*, u32);
extern void ADXSJD_SetMaxDecSmpl(void*, u32);
extern void ADXSJD_TermSupply(void*);
extern void ADXSJD_EntryFltFunc(void*, void*, void*);
extern void ADXSJD_ExecServer(void);
extern u32 ADXRNA_GetNumData(void*);
extern u32 ADXRNA_GetNumRoom(void*);
extern void ADXRNA_Destroy(void*);
extern void ADXSJD_Destroy(void*);
extern void ADXSTM_Destroy(void*);
extern s32 ADXSTM_GetFileSct(void*);
extern s32 ADXSTM_Tell(void*);
extern void LSC_Destroy(void*);
extern void ADXAMP_Destroy(void*);
extern u32 ADXRNA_DiscardData(void*, u32);
extern void ADXRNA_SetTransSw(void*, s32);
extern void ADXRNA_SetPlaySw(void*, s32);
extern void ADXRNA_SetOutVol(void*, s32);
extern void ADXRNA_SetOutPan(void*, s32, s32);
extern u32 lbl_eu_805E26D4;
extern void ADXRNA_ExecServer(void);
extern u32 ADXT_ExecHndl(void*);
extern void SJ_SplitChunk(void*, u32, void*, void*);
extern u32 __cvt_fp2unsigned(float);
extern u32 lbl_eu_805E26DC;
extern void (*lbl_eu_805E4F20)(void);
extern void (*lbl_eu_805E4F24)(void*);
extern u32 lbl_eu_805E4F10;
extern u32 lbl_eu_805E4F18;
extern u32 lbl_eu_805E4F1C;
extern u32 lbl_eu_805E4F28;
extern u32 lbl_eu_805E4EF0;
extern u32 lbl_eu_80560044;
extern u32 lbl_eu_80560040;
/* rodata constant pool used by the adxt time/sfreq helpers */
extern f32 lbl_eu_805162D0;
extern f64 lbl_eu_805162D8;
extern s32 ADX_DecodeInfo(const u8* info, s32 size, s16* outA, u8* outB, u8* outC,
                         u8* outD, u8* outE, u32* outF, u32* outG, u32* outH);
extern void* memset(void*, int, u32);
extern void* SJRBF_Create(void* pool_mem, u32 buf_size, u32 xtr_size);
extern void* ADXSTM_Create(void* fileName, int type);
extern void* ADXSJD_Create(void* stream, s32 numChan, void* sjArray);
extern void* ADXRNA_Create(void* sjArray, s32 numChan, void* work);
extern void* LSC_Create(void* handle);
extern void LSC_SetStmHndl(void* entry, void* stm);

void* adxt_Create(s32 numChan, void* work, s32 workEnd);

typedef struct ADXT_ {
    u8 _pad0[0x38];
    s32 sfreq; /* 0x38 */
    u8 _pad1[0x48 - 0x3C];
    s32 maxDec; /* 0x48 */
} ADXT_;

/* Server control block rooted at lbl_eu_805E4EF0 (0x805E4EF0..0x805E4F44) */
typedef struct ADXT_SVR_ {
    void (*callback0)(void*); /* 0x00 */
    void* callback0Arg;       /* 0x04 */
    void (*callback1)(void*); /* 0x08 */
    void* callback1Arg;       /* 0x0C */
    void (*callback2)(void*); /* 0x10 */
    void* callback2Arg;       /* 0x14 */
    void (*callback3)(void*); /* 0x18 */
    void* callback3Arg;       /* 0x1C */
    u32 active;               /* 0x20 */
    u32 state;                /* 0x24 */
    u8 _pad0[0x38 - 0x28];
    u32 svrFreq;              /* 0x38 */
    u32 recalcCount;          /* 0x3C */
    f32 spd;                  /* 0x40 */
} ADXT_SVR_;

/* Overlay over the read-only literal pool at lbl_eu_805162D0 */
typedef struct ADXT_CST_ {
    f32 sectorScale; /* 0x00 */
    f32 _pad1;       /* 0x04 */
    f64 conv;        /* 0x08: 2^52 bias for the int->double conversion trick */
    f32 zero;        /* 0x10 */
    f32 thousand;    /* 0x14 */
    f32 limitHi;     /* 0x18 */
    f32 limitLo;     /* 0x1C */
} ADXT_CST_;

/* Signed int -> float via the 0x4330-bias double trick: flip the sign bit,
 * add the 2^52 bias implicitly through the exponent, then subtract the
 * named 2^52 rodata constant to recover the signed value. */
#define ADXT_S2F(cst, v) \
    ((f32)((f64)(u32)((u32)(v) ^ 0x80000000u) - (cst)->conv))

/* Full adxt handle (0xC4 bytes, table at lbl_eu_805E26E8) */
typedef struct ADXT_OBJ_ {
    u8 active;      /* 0x00 */
    u8 stat;        /* 0x01 */
    u8 mode;        /* 0x02 */
    u8 numChan;     /* 0x03 */
    void* sjd;      /* 0x04 */
    void* stm;      /* 0x08 */
    void* rna;      /* 0x0C */
    void* mainSj;   /* 0x10 */
    void* inSj;     /* 0x14 */
    void* chans[2]; /* 0x18 */
    u8* workBase;   /* 0x20 */
    u32 workSize;   /* 0x24 */
    u32 f28;        /* 0x28 */
    u8* alignedWork;/* 0x2C */
    u32 f30;        /* 0x30 */
    u32 f34;        /* 0x34 */
    u32 svrFreq;    /* 0x38 */
    s16 numSector;  /* 0x3C */
    s16 sectorSize; /* 0x3E */
    s16 f40;        /* 0x40 */
    s16 pan[2];     /* 0x42 */
    s16 f46;        /* 0x46 */
    u8 _pad1[0x54 - 0x48];
    u32 f54;        /* 0x54 */
    u32 f58;        /* 0x58 */
    u32 f5C;        /* 0x5C */
    s16 f60;        /* 0x60 */
    u8 _pad2[0x64 - 0x62];
    u32 f64;        /* 0x64 */
    s16 f68;        /* 0x68 */
    s16 f6A;        /* 0x6A */
    u8 lpFlg;       /* 0x6C */
    u8 f6D;         /* 0x6D */
    u8 _pad3[0x72 - 0x6E];
    u8 pauseFlg;    /* 0x72 */
    u8 f73;         /* 0x73 */
    u8 _pad4[0x88 - 0x74];
    u32 timeOfst;   /* 0x88 */
    u8 _pad5[0x94 - 0x8C];
    void* lsc;      /* 0x94 */
    u8 f98;         /* 0x98 */
    u8 _pad6[0x9C - 0x99];
    u32 time;       /* 0x9C */
    u32 tick;       /* 0xA0 */
    u32 fA4;        /* 0xA4 */
    u8 fA8;         /* 0xA8 */
    u8 volSw;       /* 0xA9 */
    u8 _pad7[0xAC - 0xAA];
    u8* sjTail;     /* 0xAC */
} ADXT_OBJ_;

typedef struct SJ_CHUNK_ {
    u8* ptr;
    s32 size;
} SJ_CHUNK_;

void adxt_SetLpFlg(void* self, u32 flg);
void adxt_start_sj(void* self, void* sj);
void adxt_start_stm(void* self, void* fname, void* sctOfst, void* sctLen, s32 numChan);
void adxt_StopWithoutLsc(void* self);
void adxt_Destroy(void* self);
void adxt_GetTime(void* self, u32* time, u32* sfreq);
void adxt_SetOutPan(void* self, s32 ch, s32 pan);
void adxt_ExecServer(void);

// Build a playback handle in the first free slot of the 16-entry handle table.
// Layout: main SJ ring buffer, per-channel SJ ring buffers, SJD decoder,
// RNA transfer manager and LSC link, all carved out of the caller's work area.
void* adxt_Create(s32 numChan, void* work, s32 workEnd) {
    ADXT_OBJ_* h;
    s32 idx;
    s32 i;
    u8* p;
    u8* aligned = (u8*)(((u32)work + 0x3F) & ~0x3Fu);
    s32 pad = (s32)((u8*)aligned - (u8*)work);
    s32 end = workEnd - pad;

    if (numChan < 0 || work == NULL || workEnd < 0) {
        ADXERR_CallErrFunc1_(lbl_eu_805162F8);
        return NULL;
    }
    p = lbl_eu_805E26E8;
    for (idx = 0; idx < 16; idx++) {
        if ((s8)*p == 0) {
            break;
        }
        p += 0xC4;
    }
    if (idx == 16) {
        ADXERR_CallErrFunc1_(lbl_eu_805162F8 + 0x27);
        return NULL;
    }
    h = (ADXT_OBJ_*)p;
    memset(h, 0, 0xC4);
    /* carve the work area: per-channel blocks then the shared region */
    h->workBase = aligned + numChan * 0x60C0;
    h->workSize = (u32)(((end - numChan * 0x60C0) - 0x124) / 0x800) << 11;
    if ((s32)h->workSize < 0) {
        ADXERR_CallErrFunc1_(lbl_eu_805162F8 + 0x50);
        return NULL;
    }
    h->f28 = 0x24;
    h->sjTail = h->workBase + h->workSize + 0x24;
    h->alignedWork = aligned;
    h->f30 = 0x2000;
    h->f34 = 0x2060;
    h->inSj = NULL;
    h->mainSj = SJRBF_Create(h->workBase, h->workSize, 0x24);
    if (h->mainSj == NULL) {
        adxt_Destroy(h);
        return NULL;
    }
    h->stm = ADXSTM_Create(NULL, 0);
    if (h->stm == NULL) {
        adxt_Destroy(h);
        return NULL;
    }
    for (i = 0; i < numChan; i++) {
        h->chans[i] = SJRBF_Create(h->alignedWork + h->f34 * 2 * i, h->f30 * 2,
                                   (h->f34 - h->f30) * 2);
        if (h->chans[i] == NULL) {
            adxt_Destroy(h);
            return NULL;
        }
    }
    h->sjd = ADXSJD_Create(h->mainSj, numChan, h->chans);
    if (h->sjd == NULL) {
        adxt_Destroy(h);
        return NULL;
    }
    h->rna = ADXRNA_Create(h->chans, numChan, aligned + numChan * 0x40C0);
    if (h->rna == NULL) {
        adxt_Destroy(h);
        return NULL;
    }
    h->lsc = LSC_Create(h->mainSj);
    if (h->lsc == NULL) {
        adxt_Destroy(h);
        return NULL;
    }
    LSC_SetStmHndl(h->lsc, h->stm);
    ADXCRS_Lock();
    h->svrFreq = lbl_eu_805E4F18;
    h->numSector = (s16)((s32)h->workSize / 0x800);
    h->sectorSize = (s16)(lbl_eu_805162D0 *
                          (f32)((f64)(u32)((u32)(s32)h->numSector ^
                                           0x80000000u) -
                                lbl_eu_805162D8));
    h->f40 = 0;
    for (i = 0; i < numChan; i++) {
        h->pan[i] = -128;
    }
    h->f46 = 0;
    h->lpFlg = 1;
    h->f54 = 0;
    h->f58 = 0;
    h->f5C = 0;
    h->f60 = 0;
    h->f64 = 0;
    h->f68 = 0;
    h->f6A = 0;
    h->f6D = 1;
    h->pauseFlg = 0;
    h->timeOfst = 0;
    h->f98 = 0;
    if (h->sjd != NULL) {
        ADXSJD_SetLnkSw(h->sjd, 0);
    }
    h->volSw = 1;
    h->active = 1;
    ADXCRS_Unlock();
    return h;
}

void* ADXT_Create(void* a, void* b, s32 c) {
    void* ret;
    ADXCRS_Enter();
    ret = adxt_Create((s32)a, b, c);
    ADXCRS_Leave();
    return ret;
}

// Tear down a handle: optional user callbacks, stop + release children,
// per-channel object cleanup, then wipe the whole 0xC4-byte handle.
void adxt_Destroy(void* self) {
    u8* p = (u8*)self;
    if (self == NULL) {
        ADXERR_CallErrFunc1_(lbl_eu_805162F8 + 0x7F);
        return;
    }
    if (lbl_eu_805E4F20 != NULL) {
        lbl_eu_805E4F20();
    }
    if (lbl_eu_805E4F24 != NULL) {
        lbl_eu_805E4F24(self);
    }
    if (p[0] == 1) {
        if (self == NULL) {
            ADXERR_CallErrFunc1_(lbl_eu_805162F8 + 0xA7);
        }
        if (*(void**)(p + 8) != NULL) {
            ADXSTM_ReleaseFileNw(*(void**)(p + 8));
        }
        ADXCRS_Lock();
        if ((s8)p[2] == 4) {
            LSC_Stop(*(void**)(p + 0x94));
            if (*(void**)(p + 0x14) != NULL) {
                void* sj = *(void**)(p + 0x14);
                ((ADX_VFN1*)*(void**)sj)[5](sj);
            }
        }
        adxt_StopWithoutLsc(self);
        ADXCRS_Unlock();
    }
    if (*(void**)(p + 0xC) != NULL) {
        void* rna = *(void**)(p + 0xC);
        *(void**)(p + 0xC) = NULL;
        ADXRNA_Destroy(rna);
    }
    if (*(void**)(p + 4) != NULL) {
        void* sjd = *(void**)(p + 4);
        *(void**)(p + 4) = NULL;
        ADXSJD_Destroy(sjd);
    }
    if (*(void**)(p + 8) != NULL) {
        void* stm = *(void**)(p + 8);
        *(void**)(p + 8) = NULL;
        ADXSTM_EntryEosFunc(stm, NULL, NULL);
        ADXSTM_Destroy(stm);
    }
    if (*(void**)(p + 0x94) != NULL) {
        void* lsc = *(void**)(p + 0x94);
        *(void**)(p + 0x94) = NULL;
        LSC_Destroy(lsc);
    }
    ADXCRS_Lock();
    if (*(void**)(p + 0x10) != NULL) {
        void* ob = *(void**)(p + 0x10);
        *(void**)(p + 0x10) = NULL;
        ((ADX_VFN1*)*(void**)ob)[3](ob);
    }
    {
        u8* ch = p;
        s32 i;
        for (i = 0; i < (s32)(s8)p[3]; i++) {
            void* o;
            o = *(void**)(ch + 0x18);
            if (o != NULL) {
                *(void**)(ch + 0x18) = NULL;
                ((ADX_VFN1*)*(void**)o)[3](o);
            }
            o = *(void**)(ch + 0x78);
            if (o != NULL) {
                *(void**)(ch + 0x78) = NULL;
                ((ADX_VFN1*)*(void**)o)[3](o);
            }
            o = *(void**)(ch + 0x80);
            if (o != NULL) {
                *(void**)(ch + 0x80) = NULL;
                ((ADX_VFN1*)*(void**)o)[3](o);
            }
            ch += 4;
        }
    }
    if (*(void**)(p + 0x74) != NULL) {
        void* amp = *(void**)(p + 0x74);
        *(void**)(p + 0x74) = NULL;
        ADXAMP_Destroy(amp);
    }
    memset(self, 0, 0xC4);
    p[0] = 0;
    ADXCRS_Unlock();
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
    u8* p;
    s32 i;
    p = (u8*)self;
    for (i = 0; i < (s32)(s8)((u8*)self)[3]; i++) {
        void* obuf = *(void**)(p + 0x18);
        void** vtbl = *(void***)obuf;
        ((void (*)(void*))vtbl[5])(obuf);
        p += 4;
    }
    ADXSJD_SetInSj(*(void**)((u8*)self + 4), sj);
    *(void**)((u8*)self + 0x14) = sj;
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

void adxt_Destroy(void* self);
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
    stat = ((u8*)self)[1];
    if ((u8)(stat - 3) <= 1) {
        u32 obufNumSmpl;
        u32 decNumSmpl;
        *sfreq = ADXSJD_GetSfreq(*(void**)((u8*)self + 4));
        decNumSmpl = ADXSJD_GetDecNumSmpl(*(void**)((u8*)self + 4));
        if (self == NULL) {
            ADXERR_CallErrFunc1_(lbl_eu_805162F8 + 0x11C);
            obufNumSmpl = (u32)-1;
        } else if (*(void**)((u8*)self + 0x18) != NULL) {
            void** vtbl = *(void***)*(void**)((u8*)self + 0x18);
            s32 v = ((s32 (*)(void*, s32))vtbl[9])(*(void**)((u8*)self + 0x18), 1);
            obufNumSmpl = (u32)(((v < 0) + v) >> 1);
        } else {
            obufNumSmpl = 0;
        }
        *time = *(u32*)((u8*)self + 0xA4) + (decNumSmpl - (obufNumSmpl + ADXRNA_GetNumData(*(void**)((u8*)self + 0xC))));
    } else if ((s8)stat == 5) {
        *time = ADXSJD_GetTotalNumSmpl(*(void**)((u8*)self + 4));
        *sfreq = ADXSJD_GetSfreq(*(void**)((u8*)self + 4));
        *time = *time * (16 / (s32)ADXSJD_GetOutBps(*(void**)((u8*)self + 4)));
        *time += *(u32*)((u8*)self + 0xA4);
    } else {
        *time = 0;
        *sfreq = 1;
    }
    *time = *time + *(u32*)((u8*)self + 0x88);
    if ((s32)*time < 0) {
        *time = 0;
    }
}

void adxt_GetTime(void* self_, u32* time, u32* sfreq) {
    u8* self = (u8*)self_;
    u32 time2;
    u32 sfreq2;
    ADXT_CST_* cst = (ADXT_CST_*)&lbl_eu_805162D0;
    ADXT_SVR_* svr = (ADXT_SVR_*)&lbl_eu_805E4EF0;

    if (self == NULL || time == NULL || sfreq == NULL) {
        ADXERR_CallErrFunc1_(lbl_eu_805162F8 + 0x14B);
        return;
    }
    if (svr->active == 0) {
        adxt_GetTimeSfreq2(self_, time, sfreq);
        return;
    }
    svr->spd = cst->zero;
    if ((u8)(((u8*)self)[1] - 3) <= 1) {
        /* decode-ahead extrapolation against the server clock */
        if ((s8)self[0x72] == 0) {
            *time = *(u32*)(self + 0x9C) +
                    (lbl_eu_805E26DC - *(u32*)(self + 0xA0)) * 100;
        } else {
            *time = *(u32*)(self + 0x9C);
        }
        adxt_GetTimeSfreq2(self_, &time2, &sfreq2);
        svr->spd = cst->thousand *
                   (ADXT_S2F(cst, time2) / ADXT_S2F(cst, sfreq2) -
                    ADXT_S2F(cst, *time) / ADXT_S2F(cst, svr->svrFreq));
        if (svr->spd > cst->limitHi || svr->spd < cst->limitLo) {
            /* drift too large: resync the sample counter via a clean re-query */
            if (lbl_eu_80560044 == 1) {
                u32 savedActive = svr->active;
                svr->active = 0;
                adxt_GetTime(self_, &time2, &sfreq2);
                svr->recalcCount = svr->recalcCount + 1;
                svr->active = savedActive;
            }
            *(u32*)(self + 0x9C) = __cvt_fp2unsigned(
                ADXT_S2F(cst, svr->svrFreq) *
                (ADXT_S2F(cst, time2) / ADXT_S2F(cst, sfreq2)));
            *(u32*)(self + 0xA0) = lbl_eu_805E26DC;
        }
    } else if ((s8)((u8*)self)[1] == 5) {
        /* note: locals here are intentionally spilled to stack in retail */
        u32 totalSmpl;
        u32 sf;
        u32 tmp;
        totalSmpl = ADXSJD_GetTotalNumSmpl(*(void**)(self + 4));
        sf = ADXSJD_GetSfreq(*(void**)(self + 4));
        tmp = totalSmpl * (16 / (s32)ADXSJD_GetOutBps(*(void**)(self + 4)));
        *time = (u32)(s32)(ADXT_S2F(cst, svr->svrFreq) *
                           (ADXT_S2F(cst, tmp) / ADXT_S2F(cst, sf)));
        *time = *time + 1 + *(u32*)(self + 0x9C);
    } else {
        *time = 0;
    }
    *time += *(u32*)(self + 0x88);
    *sfreq = svr->svrFreq;
}

void ADXT_GetTime(void* a, void* b, void* c) {
    ADXCRS_Enter();
    adxt_GetTime(a, b, c);
    ADXCRS_Leave();
}

s32 ADXT_GetTimeReal(void* self) {
    u32 t2, t1;
    s32 ret;
    ADXCRS_Enter();
    adxt_GetTime(self, &t1, &t2);
extern float lbl_eu_805162F0;

    ret = (s32)((float)(s32)t1 / (float)(s32)t2 * lbl_eu_805162F0);
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

void adxt_SetOutPan(void* self, s32 ch, s32 pan) {
    s32 defpan;
    s32 outpan;
    if (self == NULL) {
        ADXERR_CallErrFunc1_(lbl_eu_805162F8 + 0x246);
        return;
    }
    if (ch != 0 && ch != 1) {
        ADXERR_CallErrFunc1_(lbl_eu_805162F8 + 0x277);
        return;
    }
    if (pan != -128) {
        if (pan < -15) {
            pan = -15;
        } else if (pan > 15) {
            pan = 15;
        }
    }
    if ((s8)((u8*)self)[0xA9] == 1) {
        defpan = (s16)ADXSJD_GetDefPan(*(void**)((u8*)self + 4), ch);
        if (defpan == -128) {
            defpan = 0;
        }
    } else {
        defpan = 0;
    }
    if (lbl_eu_805E26D4 == 0) {
        if (pan == -128) {
            if ((s32)ADXSJD_GetNumChan(*(void**)((u8*)self + 4)) == 2) {
                outpan = (ch == 0) ? -15 : 15;
            } else {
                outpan = 0;
            }
            outpan += defpan;
        } else {
            outpan = pan + defpan;
        }
    } else {
        outpan = 0;
    }
    *(s16*)((u8*)self + 0x42 + ch * 2) = (s16)pan;
    if (ch < (s32)(s8)((u8*)self)[3]) {
        ADXRNA_SetOutPan(*(void**)((u8*)self + 0xC), ch, outpan);
    } else {
        ADXERR_CallErrFunc1_(lbl_eu_805162F8 + 0x2A1);
    }
}

void ADXT_SetOutPan(void* self, s32 ch, s32 pan) {
    ADXCRS_Enter();
    adxt_SetOutPan(self, ch, pan);
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

void ADXT_SetSvrFreq(void* self, s32 freq) {
    ADXT_* adxt;
    void* sjd;
    s32 sfreq;
    s32 numLoop;
    s32 blkSmpl;
    s32 val;
    ADXCRS_Enter();
    if (self == NULL) {
        ADXERR_CallErrFunc1_(lbl_eu_805162F8 + 0x3A4);
    } else {
        adxt = (ADXT_*)self;
        adxt->sfreq = freq;
        lbl_eu_805E4F1C = (u32)freq;
        sjd = *(void**)((u8*)self + 4);
        if ((s32)ADXSJD_GetStat(sjd) == 2) {
            sfreq = (s32)ADXSJD_GetSfreq(sjd);
            numLoop = (s32)ADXSJD_GetNumLoop(sjd);
            if (numLoop > 0) {
                val = sfreq / adxt->sfreq;
                adxt->maxDec = val * 4 - val;
            } else {
                val = sfreq / adxt->sfreq;
                {
                    s32 x = val * 4 - val;
                    adxt->maxDec = ((x < 0) + x) >> 1;
                }
            }
            blkSmpl = (s32)ADXSJD_GetBlkSmpl(sjd);
            val = blkSmpl * 2;
            {
                s32 cur = adxt->maxDec;
                cur = (cur + val) / val * val;
                adxt->maxDec = cur;
            }
            ADXSJD_SetMaxDecSmpl(sjd, (u32)adxt->maxDec);
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
        return ((v < 0) + v) >> 1;
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
    u32* srv = (u32*)&lbl_eu_805E4EF0;
    u8* p;
    s32 i;
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
    {
        void (*fn)(void*) = *(void (**)(void*))&srv[4];
        srv[9] = 3;
        if (fn != NULL) {
            fn(*(void**)&srv[5]);
        }
    }
    ADXRNA_ExecServer();
    {
        void (*fn)(void*) = *(void (**)(void*))&srv[6];
        if (fn != NULL) {
            fn(*(void**)&srv[7]);
        }
    }
    {
        void (*fn)(void*) = *(void (**)(void*))&srv[2];
        srv[9] = 0;
        if (fn != NULL) {
            fn(*(void**)&srv[3]);
        }
    }
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
    s32 lpStart, lpEnd, lpSize;
    s32 decDtLen;
    if (self == NULL) {
        ADXERR_CallErrFunc1_(lbl_eu_805162F8 + 0x594);
        return;
    }
    if (*(void**)((u8*)self + 0x14) == NULL) {
        numData = 0;
    } else {
        void** vtbl = *(void***)*(void**)((u8*)self + 0x14);
        numData = ((s32 (*)(void*, s32))vtbl[9])(*(void**)((u8*)self + 0x14), 1);
    }
    if ((s8)((u8*)self)[2] != 4 && (s8)((u8*)self)[0x6C] == 1 && flg == 0) {
        decDtLen = (s32)ADXSJD_GetDecDtLen(*(void**)((u8*)self + 4));
        numData += decDtLen;
        lpStart = (s32)ADXSJD_GetLpStartOfst(*(void**)((u8*)self + 4));
        {
            s32 rounded = (lpStart + 0x7FF) / 0x800;
            lpStart = rounded << 11;
        }
        lpEnd = (s32)ADXSJD_GetLpEndOfst(*(void**)((u8*)self + 4));
        {
            s32 rounded = (lpEnd + 0x7FF) / 0x800;
            lpEnd = rounded << 11;
        }
        lpSize = lpEnd - lpStart;
        if (lpSize <= 0) {
            *(u32*)((u8*)self + 0xC0) = 0;
        } else {
            *(u32*)((u8*)self + 0xC0) = (u32)(lpStart + ((numData - lpStart) / lpSize) * lpSize);
        }
    }
    ((u8*)self)[0x6C] = (u8)flg;
}

void adxt_Pause(void* self, int pause) {
    s32 stat;
    s32 sctRemain;
    s32 numData;
    u32 savedSvr;
    u32 time, sfreq;
    if (self == NULL) {
        ADXERR_CallErrFunc1_(lbl_eu_805162F8 + 0x64A);
        return;
    }
    stat = (s32)(s8)((u8*)self)[1];
    if (pause == (s32)(s8)((u8*)self)[0x72]) {
        return;
    }
    ADXCRS_Lock();
    ((u8*)self)[0x72] = (u8)pause;
    if ((u8)(stat - 3) > 1) {
        ADXCRS_Unlock();
        return;
    }
    if (pause == 1) {
        ADXRNA_SetPlaySw(*(void**)((u8*)self + 0xC), 0);
    } else if (((u8*)self)[0x73] == 1) {
        void* stm = *(void**)((u8*)self + 8);
        void* rna = *(void**)((u8*)self + 0xC);
        void* sjd = *(void**)((u8*)self + 4);
        sctRemain = ADXSTM_GetFileSct(stm) - ADXSTM_Tell(stm);
        if (sctRemain > (s32)*(s16*)((u8*)self + 0x3E)) {
            sctRemain = (s32)*(s16*)((u8*)self + 0x3E);
        }
        /* inline of ADXT_GetNumSctIbuf: sectors left in the input buffer */
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
            /* inline of ADXT_GetNumSmplObuf for ch 0 */
            if (self == NULL) {
                ADXERR_CallErrFunc1_(lbl_eu_805162F8 + 0x11C);
                numData = -1;
            } else if (*(void**)((u8*)self + 0x18) != NULL) {
                void* ob = *(void**)((u8*)self + 0x18);
                numData = ((ADX_VFN2*)*(void**)ob)[9](ob, 1) / 2;
            } else {
                numData = 0;
            }
            if (numData > 0) {
                /* decode-ahead cap: min(maxDecSmpl, 0x2000) */
                s32 maxDec = *(s32*)((u8*)self + 0x48);
                s32 limit = 0x2000;
                u32 blkSmpl;
                s32 numRoom;
                if (maxDec < 0x2000) {
                    limit = maxDec;
                }
                numData = ADXRNA_GetNumData(rna);
                if (numData < limit) {
                    blkSmpl = ADXSJD_GetBlkSmpl(sjd);
                    numRoom = (s32)ADXRNA_GetNumRoom(rna);
                    if (numRoom > (s32)blkSmpl && ADXSJD_GetStat(sjd) != 3) {
                        goto update_time; /* decoder not idle: skip resume */
                    }
                }
            }
        }
        if ((s8)((u8*)self)[0x72] == 0) {
            ADXRNA_SetPlaySw(rna, 1);
            ((u8*)self)[0x73] = 0;
            *(u32*)((u8*)self + 0xA0) = lbl_eu_805E26DC;
        }
    } else {
        ADXRNA_SetPlaySw(*(void**)((u8*)self + 0xC), 1);
        *(u32*)((u8*)self + 0xA0) = lbl_eu_805E26DC;
    }
update_time:
    /* refresh the sample-position estimate against the server clock */
    savedSvr = lbl_eu_805E4F10;
    lbl_eu_805E4F10 = 0;
    adxt_GetTime(self, &time, &sfreq);
    *(u32*)((u8*)self + 0x9C) =
        __cvt_fp2unsigned((float)(s32)lbl_eu_805E4F28 * ((float)(s32)time / (float)(s32)sfreq));
    lbl_eu_805E4F10 = savedSvr;
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

s32 ADXT_DiscardSmpl(void* self, u32 num) {
    u32 savedLock;
    u32 result;
    u32 sfreq, time;
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
        *(u32*)((u8*)self + 0x9C) = __cvt_fp2unsigned((float)(s32)time / (float)(s32)sfreq * (float)(s32)lbl_eu_805E4F28);
        *(u32*)((u8*)self + 0xA0) = lbl_eu_805E26DC;
    }
    ADXCRS_Leave();
    return result;
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
    s16* pA;
    u8* pB;
    u8* pC;
    u8* pD;
    u8* pE;
    u32* pF;
    u32* pG;
    u32* pH;
    if (size < 2) return 0;
    if (*(u16*)data != 0x8000) return 0;
    pA = &outA;
    pB = &outB;
    pC = &outC;
    pD = &outD;
    pE = &outE;
    pF = &outF;
    pG = &outG;
    pH = &outH;
    if (ADX_DecodeInfo(data, size, pA, pB, pC, pD, pE, pF, pG, pH) < 0) {
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
    s32 chunkSize, numBytes, numBytes2;
    SJ_CHUNK_ data;
    SJ_CHUNK_ rest;
    sj = *(void**)((u8*)self + 0x14);
    if (sj == NULL) {
        return 0;
    }
    chunkSize = ch * 0x12;
    numBytes = (smpl / 32) * chunkSize;
    {
        void** vtbl = *(void***)sj;
        ((void (*)(void*, s32, s32, void**))vtbl[6])(sj, 0, numBytes, (void**)&data);
    }
    numBytes2 = (data.size / chunkSize) * chunkSize;
    memset(data.ptr, 0, numBytes2);
    SJ_SplitChunk(&data, numBytes2, &data, &rest);
    {
        void** vtbl = *(void***)sj;
        ((void (*)(void*, s32, void**))vtbl[8])(sj, 1, (void**)&data);
    }
    {
        void** vtbl = *(void***)sj;
        ((void (*)(void*, s32, void**))vtbl[7])(sj, 0, (void**)&rest);
    }
    {
        void** vtbl = *(void***)sj;
        ((void (*)(void*, s32, s32, void**))vtbl[6])(sj, 0, numBytes - numBytes2, (void**)&data);
    }
    numBytes = (data.size / chunkSize) * chunkSize;
    memset(data.ptr, 0, numBytes);
    SJ_SplitChunk(&data, numBytes, &data, &rest);
    {
        void** vtbl = *(void***)sj;
        ((void (*)(void*, s32, void**))vtbl[8])(sj, 1, (void**)&data);
    }
    {
        void** vtbl = *(void***)sj;
        ((void (*)(void*, s32, void**))vtbl[7])(sj, 0, (void**)&rest);
    }
    return ((numBytes2 + numBytes) / chunkSize) << 5;
}

s32 ADXT_InsertSilence(void* self, s32 ch, s32 smpl) {
    s32 ret;
    ADXCRS_Enter();
    ret = adxt_InsertSilence(self, ch, smpl);
    ADXCRS_Leave();
    return ret;
}

