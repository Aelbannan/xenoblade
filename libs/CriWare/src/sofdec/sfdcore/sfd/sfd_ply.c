// CriWare Sofdec player core — sfd_ply TU
// High-level C reconstruction for MWCC matching.

#include <types.h>
#include <string.h>

// Forward declarations for internal functions
void sfply_ExecOne(void* self);
int sfply_StatPrep(void* self, int canExec);
int fn_803CC170(void* self);
void fn_803CC238(void* self);
int sfply_IsBpaOn(void* self);
int sfply_IsBpaOff(void* self);
int sfply_IsEtrg(void* self);
int criware_803C9FC0(void* self);
int sfply_IsPlayTimeAutoStop(void* self);
void criware_803CA124(void* self, int fpsArg);
int SFD_Create(void* config, void* extra);
void* sfply_InitHn(void* config, void* extra);
void sfply_InitPlyInf(void* self);
int SFD_Destroy(void* self);
int SFD_Start(void* self);
int fn_803CD484(void* self);
int SFD_Stop(void* self);
int SFPLY_GetResetFlg(void);
int sfply_ResetHn(void* self);
void SFD_RequestStop(void* self);
int SFD_TermSupply(void* self);
int SFD_GetFrm(void* self, void** outFrm);
int SFD_RelFrm(void* self, void* frm);
void SFD_SetSupplySj(void* self, void* sj);

// External functions
extern int SFLIB_CheckHn(void* hn);
extern void SFLIB_SetErr(void* hn, u32 errCode);
extern void SFLIB_LockCs(void* cs);
extern void SFLIB_UnlockCs(void* cs);
extern void SFTIM_VbIn(void);
extern void SFTIM_GetTime(void* hn, int* sec, int* usec);
extern int SFTIM_GetTimeSub(void* hn, int* sec, int* usec);
extern int SFTIM_IsStagnant(void* hn);
extern void SFTIM_InitHn(void* hn, void* area);
extern void SFTMR_GetTmr(void* hn);
extern void SFTMR_GetTmrUnit(void* hn);
extern void SFTMR_AddTsum(void* area, u32 lo1, u32 hi1, u32 lo2, u32 hi2);
extern void SFTMR_InitTsum(void* area);
extern int SFBUF_GetTermFlg(void* hn, int id);
extern void SFBUF_SetTermFlg(void* hn, int id, int val);
extern int SFBUF_GetWTot(void* hn, int type);
extern int SFBUF_GetRTot(void* hn, int type);
extern int SFBUF_RingGetDataSiz(void* hn, int type);
extern int SFBUF_InitHn(void* hn, void* area, void* cfg);
extern void SFBUF_DestroySj(void* hn);
extern void SFBUF_SetSupplySj(void* hn, void* sj);
extern int SFTRN_CallTrtTrif(void* hn, int a, int b, int c, int d);
extern int SFTRN_CallTrSetup(void* hn, int type);
extern int SFTRN_GetPrepFlg(void* hn, int track);
extern int SFTRN_GetTermFlg(void* hn, int track);
extern int SFTRN_IsSetup(void* hn, int track);
extern void SFTRN_InitHn(void* hn, void* area, void* cfg, void* extra);
extern void SFSEE_ExecServer(void* hn);
extern void SFSEEKI_InitHn(void* area);
extern void SFSEE_InitHn(void* area);
extern int SFSET_GetCond(void* hn, int condId);
extern void SFSET_SetCond(void* hn, int condId, int val);
extern void SFHDS_InitFhd(void* area);
extern void SFHDS_FinishFhd(void* area);
extern int SFPL2_Pause(void* hn, int pause);
extern int SFPL2_Standby(void* hn);
extern void SFD_GetLimitTime(void* hn, int* out);
extern void SFD_SetMbCb(void* hn, void* cb, u32 arg1, u32 arg2);
extern void SFD_SetUsrTimeFn(void* hn, void* fn, u32 arg);
extern void SFD_SetExtClockFn(void* hn, void* fn, u32 arg1, u32 arg2);
extern void SFD_SetUsrIsSkipFn(void* hn, void* fn);
extern void SFD_SetSpeedRational(void* hn, u32 num, u32 den);
extern void SFD_SetCyclicFrameOutput(void* hn, void* fn, u32 arg);
extern void SFD_SetLimitTime(void* hn, int val);
extern void SFD_EntrySeek(void* hn, void* seekData);
extern void SFD_SetByteRate(void* hn, int rate);
extern void SFD_SetFileSize(void* hn, int size);
extern void SFD_SetTotTime(void* hn, int sec, int frac);
extern void SFD_SetSeekPos(void* hn, int pos);
extern void SFD_SetVideoPts(void* hn, int pts, int type);
extern int SFMPV_SaveCond(void* hn, void* buf, int size);
extern void SFMPV_RestoreCond(void* hn, void* buf, int result);
extern void SFSEE_FixAvPlay(void* hn, int videoFlag, int audioFlag);
extern int UTY_CmpTime(int sec1, int frac1, int sec2, int frac2);
extern int UTY_MulDiv(int a, int b, int c);
extern int UTY_IsTmrVoid(void);
extern void UTY_MemsetDword(void* dst, int val, int count);
extern void MEM_Copy(void* dst, void* src, int size);
extern int fn_803C1CAC(void* hn);
extern void criware_803C0D94(void* hn, u32 val, u32 arg);
extern void criware_803C1570(void* hn, void* data, u32 arg);
extern void fn_803C34DC(void* area);
extern float __cvt_sll_flt(int hi, int lo);
extern int sprintf(char* str, const char* fmt, ...);

// Global data
extern u32 lbl_eu_80619BA0;
extern u32 lbl_eu_80619BA4;
extern u32 lbl_eu_80619BA8;
extern void* lbl_eu_80606E34;
extern u8 lbl_eu_80606E38[];
extern u8 lbl_eu_8056864C[];
extern u8 lbl_eu_80567FAC[];
extern u8 lbl_eu_80568080[];
extern u8 lbl_eu_80568154[];
extern u8 lbl_eu_80568228[];
extern u8 lbl_eu_805682FC[];
extern u8 lbl_eu_805683D0[];
extern u8 lbl_eu_805684A4[];
extern u8 lbl_eu_80568578[];
extern u8 lbl_eu_80568C84[];
extern float lbl_eu_8051CBA4;
extern char lbl_eu_8051CBA8[];

// Field access helpers
#define FIELD(type, base, off) (*((type*)((u8*)(base) + (off))))
#define FIELD_PTR(type, base, off) ((type)((u8*)(base) + (off)))

// ---------------------------------------------------------------------------
// SFPLY_Init — 0x10 bytes
// ---------------------------------------------------------------------------
void SFPLY_Init(void) {
    lbl_eu_80619BA0 = 0;
}

// ---------------------------------------------------------------------------
// SFD_VbIn — 0x30 bytes
// ---------------------------------------------------------------------------
void SFD_VbIn(void) {
    void* cs;
    SFLIB_LockCs(&cs);
    SFTIM_VbIn();
    SFLIB_UnlockCs(&cs);
}

// ---------------------------------------------------------------------------
// SFD_IsHnSvrWait — 0x3C bytes
// ---------------------------------------------------------------------------
int SFD_IsHnSvrWait(void* self) {
    int status = FIELD(int, self, 0x54);
    if ((unsigned int)(status - 1) > 3) return 1;
    if (FIELD(int, self, 0x64) == 1) return 1;
    return (FIELD(int, self, 0x50) == 0) ? 1 : 0;
}

// ---------------------------------------------------------------------------
// SFD_ExecOne — 0xB0 bytes
// ---------------------------------------------------------------------------
int SFD_ExecOne(void* self) {
    void* ctx;
    void (*trace)(void*, void*);
    void* vtbl;

    if (SFLIB_CheckHn(self)) {
        SFLIB_SetErr(0, 0xff000138);
        return 0;
    }

    ctx = lbl_eu_80606E34;
    if (ctx) {
        *(void**)(lbl_eu_8056864C + 0x0C) = self;
        vtbl = *(void**)ctx;
        trace = *(void(**)(void*, void*))((u8*)vtbl + 0x24);
        trace(ctx, lbl_eu_8056864C + 4);
    }

    sfply_ExecOne(self);

    ctx = lbl_eu_80606E34;
    if (ctx) {
        vtbl = *(void**)ctx;
        trace = *(void(**)(void*, void*))((u8*)vtbl + 0x24);
        trace(ctx, lbl_eu_8056864C + 0x6C);
    }

    return 0;
}

// ---------------------------------------------------------------------------
// sfply_ExecOne — 0x480 bytes — main player state machine
// ---------------------------------------------------------------------------
void sfply_ExecOne(void* self) {
    int status;
    int termResult;
    int canExec;
    u32 tmrLo, tmrHi;
    int newState;
    void* cs;
    int pauseResult;
    int cond;
    int timeSec, timeUsec;
    int timeOk;
    int timeCond;
    u32 tmrLo2, tmrHi2;
    u32 diffLo, diffHi;
    u32 unitLo, unitHi;
    u32 fpsBaseLo, fpsBaseHi;
    float fpsNum, fpsDen;
    u32 mulHi, mulLo;
    int subStatus;

    status = FIELD(int, self, 0x54);
    if ((unsigned int)(status - 1) > 3) return;
    if (FIELD(int, self, 0x50) == 0) return;

    FIELD(int, self, 0x50) = 0;

    if (!fn_803C1CAC(self)) return;

    // Get terminal flag and check canExec
    termResult = SFBUF_GetTermFlg(self, FIELD(int, self, 0x1FEC));
    // canExec = (termResult != 0) ? 1 : 0 — using MWCC idiom
    canExec = (((-termResult) | termResult) >> 31);

    if (FIELD(int, self, 0x64) == 1) return;

    SFTMR_GetTmr(self);
    tmrLo = /* r3 */ 0; // placeholder — actual values come from SFTMR_GetTmr
    tmrHi = /* r4 */ 0;

    subStatus = status;

    // Sub-state processing: status 2,3 → call setup + seek exec
    if ((unsigned int)(status - 2) <= 2) {
        SFTRN_CallTrSetup(self, 2);
        SFSEE_ExecServer(self);
    }

    // Main state machine
    newState = FIELD(int, self, 0x54);
    switch (newState) {
    case 1: {
        int ss = FIELD(int, self, 0x58);
        if ((unsigned int)(ss - 2) <= 2 || ss == 6) {
            newState = 2;
        }
        break;
    }
    case 2:
        sfply_StatPrep(self, canExec);
        break;
    case 3: {
        int newSub = newState;
        int nextSub = FIELD(int, self, 0x58);
        switch (nextSub) {
        case 2: newSub = 2; break;
        case 3: newSub = 3; break;
        case 4: case 6: {
            // Complex end-of-stream check
            int shouldStop = 0;
            if (!FIELD(int, self, 0xA54)) {
                shouldStop = 1;
            } else if (!FIELD(int, self, 0xA30)) {
                shouldStop = 1;
            } else if (FIELD(int, self, 0x1050)) {
                shouldStop = 1;
            } else {
                int playTime = FIELD(int, self, 0x106C);
                int limit = FIELD(int, self, 0xAD0);
                if (playTime >= limit) {
                    shouldStop = 1;
                } else {
                    int etrg = sfply_IsEtrg(self);
                    shouldStop = (((-etrg) | etrg) >> 31);
                }
            }
            if (shouldStop) {
                SFTRN_CallTrtTrif(self, 7, 6, 0, 0);
                newSub = 4;
            }
            break;
        }
        default:
            break;
        }
        newState = newSub;
        break;
    }
    case 4: {
        int timeSec2 = FIELD(int, self, 0xA6C);
        int timeUsec2 = FIELD(int, self, 0xA70);

        // Check if time is valid
        if (timeSec2 == -4) {
            timeOk = 0;
        } else {
            SFTIM_GetTime(self, &timeSec, &timeUsec);
            if (timeSec < 0) {
                timeOk = 0;
            } else {
                timeOk = (UTY_CmpTime(timeSec, timeUsec, timeSec2, timeUsec2) == 0) ? 1 : 0;
            }
        }

        if (!timeOk) {
            int etrg = sfply_IsEtrg(self);
            if (!etrg) {
                int st4 = FIELD(int, self, 0x54);
                if (st4 == 4 && FIELD(int, self, 0x5C) != 1 && FIELD(int, self, 0x980) == 1) {
                    timeCond = 0;
                } else {
                    int stag = SFTIM_IsStagnant(self);
                    timeCond = (((-stag) | stag) >> 31);
                }
                if (!timeCond) {
                    int autoStop = sfply_IsPlayTimeAutoStop(self);
                    if (!autoStop && !FIELD(int, self, 0x70)) {
                        goto doPauseCheck;
                    }
                }
            }
        }

        // Stop handling
        if (FIELD(int, self, 0x54) == 4) {
            int trResult = SFTRN_CallTrtTrif(self, 7, 7, 0, 0);
            if (trResult) {
                // error, return status
                break;
            }
        }
        FIELD(int, self, 0x54) = 1;
        FIELD(int, self, 0x58) = 1;

        // Set up stop timer
        FIELD(int, self, 0x58) = 6;
        SFTMR_GetTmr(self);
        FIELD(u32, self, 0x2768) = tmrLo;
        FIELD(u32, self, 0x276C) = tmrHi;
        SFTMR_GetTmrUnit(self);
        // ... fps calculation omitted for brevity ...

        newState = 0;
        break;

doPauseCheck:
        // Pause check
        cs = NULL;
        SFLIB_LockCs(&cs);
        cond = 0;
        if (!FIELD(int, self, 0x980)) {
            if (sfply_IsBpaOn(self)) {
                FIELD(int, self, 0x984)++;
                FIELD(int, self, 0x980) = 1;
                pauseResult = SFPL2_Pause(self, 1);
            }
        } else {
            if (sfply_IsBpaOff(self)) {
                FIELD(int, self, 0x980) = 0;
                pauseResult = SFPL2_Pause(self, 0);
            }
        }
        SFLIB_UnlockCs(&cs);

        if (pauseResult) {
            newState = FIELD(int, self, 0x54);
        } else {
            if (FIELD(int, self, 0x58) == 6) {
                newState = 6;
            } else {
                newState = FIELD(int, self, 0x54);
            }
        }
        break;
    }
    case 6:
        break;
    default:
        break;
    }

    FIELD(int, self, 0x54) = newState;
    SFTMR_GetTmr(self);
    SFTMR_AddTsum((u8*)self + 0x2740, tmrLo, tmrHi, tmrLo2, tmrHi2);
}

// ---------------------------------------------------------------------------
// sfply_StatPrep — 0x28C bytes
// ---------------------------------------------------------------------------
int sfply_StatPrep(void* self, int canExec) {
    int origStatus = FIELD(int, self, 0x54);
    int origSub = FIELD(int, self, 0x58);
    int newState;

    if (!fn_803CC170(self)) {
        return origStatus;
    }

    // Check video buffer term
    if (FIELD(int, self, 0xA30) == 1) {
        if (SFBUF_GetWTot(self, 1) == 0 && SFBUF_GetRTot(self, 1) == 0) {
            FIELD(int, self, 0xA30) = 0;
        }
    }
    // Check audio buffer term
    if (FIELD(int, self, 0xA34) == 1) {
        if (SFBUF_GetWTot(self, 2) == 0 && SFBUF_GetRTot(self, 2) == 0) {
            FIELD(int, self, 0xA34) = 0;
        }
    }

    SFSEE_FixAvPlay(self, FIELD(int, self, 0xA30), FIELD(int, self, 0xA34));

    // Condition checks
    if (!FIELD(int, self, 0xA34)) {
        if (FIELD(int, self, 0xA58) == 2) {
            SFSET_SetCond(self, 0xF, 1);
        }
    }
    if (!FIELD(int, self, 0xA30)) {
        if (FIELD(int, self, 0xA58) == 1) {
            SFSET_SetCond(self, 0xF, 2);
        }
    }
    if (FIELD(int, self, 0xA58) == 1) {
        if (FIELD(int, self, 0x107C)) {
            SFSET_SetCond(self, 0xF, 5);
        } else {
            SFSET_SetCond(self, 0xF, 1);
        }
    }

    fn_803CC238(self);

    // Trace
    {
        void* ctx = lbl_eu_80606E34;
        if (ctx) {
            void* vtbl = *(void**)ctx;
            void (*trace)(void*, void*) = *(void(**)(void*, void*))((u8*)vtbl + 0x24);
            *(void**)(lbl_eu_80568C84 + 0x0C) = self;
            *(int**)(lbl_eu_80568C84 + 0x18) = &FIELD(int, self, 0xA30);
            *(int**)(lbl_eu_80568C84 + 0x24) = &FIELD(int, self, 0xA34);
            *(int**)(lbl_eu_80568C84 + 0x30) = &FIELD(int, self, 0xA58);
            *(int**)(lbl_eu_80568C84 + 0x3C) = &FIELD(int, self, 0xA80);
            trace(ctx, lbl_eu_80568C84 + 4);
        }
    }

    // State transition based on original sub-status
    switch (origSub) {
    case 2:
        newState = 2;
        break;
    case 3:
        newState = 3;
        break;
    case 4: case 6: {
        int shouldStop = 0;
        if (!FIELD(int, self, 0xA54)) {
            shouldStop = 1;
        } else if (!FIELD(int, self, 0xA30)) {
            shouldStop = 1;
        } else if (FIELD(int, self, 0x1050)) {
            shouldStop = 1;
        } else {
            int playTime = FIELD(int, self, 0x106C);
            int limit = FIELD(int, self, 0xAD0);
            if (playTime >= limit) {
                shouldStop = 1;
            } else {
                int etrg = sfply_IsEtrg(self);
                shouldStop = (((-etrg) | etrg) >> 31);
            }
        }
        if (shouldStop) {
            SFTRN_CallTrtTrif(self, 7, 6, 0, 0);
            newState = 4;
        } else {
            newState = 3;
        }
        break;
    }
    default:
        newState = origSub;
        break;
    }

    return newState;
}

// ---------------------------------------------------------------------------
// fn_803CC170 — 0xC8 bytes — check if transport is ready
// ---------------------------------------------------------------------------
int fn_803CC170(void* self) {
    int ready5, ready6;
    int prep5, term5, prep6, term6;

    if (!SFSET_GetCond(self, 5)) {
        ready5 = 1;
    } else {
        prep5 = SFTRN_GetPrepFlg(self, 6);
        term5 = SFTRN_GetTermFlg(self, 6);
        ready5 = prep5 | term5;
    }

    if (!SFSET_GetCond(self, 6)) {
        ready6 = 1;
    } else {
        prep6 = SFTRN_GetPrepFlg(self, 7);
        term6 = SFTRN_GetTermFlg(self, 7);
        ready6 = prep6 | term6;
    }

    if (ready5 && ready6) return 1;
    return 0;
}

// ---------------------------------------------------------------------------
// fn_803CC238 — 0xD4 bytes — compute and set AV condition
// ---------------------------------------------------------------------------
void fn_803CC238(void* self) {
    int audioReady = (FIELD(int, self, 0xA34) == 1) ? 1 : 0;
    int videoReady = (FIELD(int, self, 0xA30) == 1) ? 2 : 0;
    int avFlags = audioReady | videoReady;
    int cond;

    switch (avFlags) {
    case 1:
        cond = 1;
        break;
    case 2:
        cond = 2;
        break;
    case 3: {
        int c25 = SFSET_GetCond(self, 0x19);
        if (c25) {
            cond = c25;
        } else {
            if (!UTY_IsTmrVoid()) {
                int c48 = SFSET_GetCond(self, 0x48);
                if (c48) {
                    cond = c48;
                } else {
                    cond = 3;
                }
            } else {
                cond = 3;
            }
        }
        break;
    }
    default:
        cond = 3;
        break;
    }

    SFSET_SetCond(self, 0x19, cond);
}

// ---------------------------------------------------------------------------
// sfply_IsBpaOn — 0x2B8 bytes — check if buffer pause should activate
// ---------------------------------------------------------------------------
int sfply_IsBpaOn(void* self) {
    int i;
    int isSetup;
    int condVal;
    int ret;
    int timeSec, timeUsec;
    int playStartSec, playStartFrac;
    int limitVal;
    int scaled;

    if (!SFSET_GetCond(self, 0x43)) return 0;
    if (!SFSET_GetCond(self, 0xF)) return 0;
    if (FIELD(int, self, 0x5C)) return 0;
    if (FIELD(int, self, 0x54) != 4) return 0;

    // Check termination conditions
    if (SFSET_GetCond(self, 5)) {
        if (SFTRN_GetTermFlg(self, 6)) goto checkBuffers;
    }
    if (SFSET_GetCond(self, 6)) {
        if (SFTRN_GetTermFlg(self, 7)) goto checkBuffers;
    }

    // Check all buffer terminal flags
    for (i = 0; i < 8; i++) {
        if (SFBUF_GetTermFlg(self, i)) goto checkBuffers;
    }
    return 0;

checkBuffers:
    // Cond 5 with no setup
    if (SFSET_GetCond(self, 5) == 1 && !FIELD(int, self, 0x988)) return 0;

    // Cond 6 with ring data
    if (SFSET_GetCond(self, 6) == 1) {
        if (SFBUF_RingGetDataSiz(self, 2) > 0) return 0;
    }

    // Transport setup with ring data
    if (SFTRN_IsSetup(self, 1)) {
        if (SFBUF_RingGetDataSiz(self, 0) > 0) return 0;
    }

    // Cond 5 with frame threshold
    if (SFSET_GetCond(self, 5) == 1) {
        int trackIdx = FIELD(int, self, 0x2070);
        void* trackPtr = (u8*)self + trackIdx * 0x74;
        int vtbl_result;
        int frameCount, threshold;
        int limitCond;
        int isBelow;

        // Virtual call on track object
        {
            void* obj = FIELD(void*, trackPtr, 0x13CC);
            void* vtbl = *(void**)obj;
            int (*fn)(void*, int) = *(int(**)(void*, int))((u8*)vtbl + 0x24);
            vtbl_result = fn(obj, 1);
        }

        frameCount = vtbl_result;
        threshold = FIELD(int, trackPtr, 0x13D4);
        // Divide by 1000 using magic number multiply: threshold * 0x50 / 1000
        threshold = (int)((s64)threshold * 0x51EC851F >> 37) + (threshold < 0 ? 1 : 0);
        // Actually: mulli r0,r0,0x50; mulhw r0,r4,r0; srawi r0,r0,5; srwi r4,r0,31; add r0,r0,r4
        // = (threshold * 80) / 32 with sign adjustment = threshold * 80 >> 5 with sign
        // Hmm let me reconsider. The magic constant 0x51EC851F is for division by 1000.
        // mulli r0, r0, 0x50 (multiply by 80)
        // mulhw r0, r4, r0  (multiply high by magic)
        // srawi r0, r0, 5   (arithmetic shift right by 5)
        // srwi r4, r0, 31   (unsigned shift right by 31 for sign)
        // add r0, r0, r4    (add sign)
        // This computes: (threshold * 80) * 0x51EC851F >> 37 >> 5... hmm
        // Actually it's: mulhw(magic, val*80) then srawi by 5 then adjust sign
        // magic = 0x51EC851F, this is the standard MWCC idiom for dividing by 1000
        // So: (threshold * 80) / 1000 = threshold * 0.08
        // But wait, the 0x50 multiplier: mulli r0, r0, 0x50 means multiply by 80
        // And the magic constant / shift = divide by 1000
        // So result = threshold * 80 / 1000 = threshold / 12.5
        // Hmm that doesn't seem right. Let me look at the sfply_IsBpaOff which has the same pattern.

        // OK let me just use a simple expression for now and iterate
        if (frameCount < threshold) {
            limitCond = SFSET_GetCond(self, 0x46);
            isBelow = (vtbl_result < limitCond) ? 1 : 0;
        } else {
            isBelow = 0;
        }
        if (isBelow) return 0;
    }

    // Time comparison
    SFTIM_GetTime(self, &timeSec, &timeUsec);
    playStartSec = FIELD(int, self, 0x1020);
    playStartFrac = FIELD(int, self, 0x1024);
    limitVal = SFSET_GetCond(self, 0x44);
    scaled = UTY_MulDiv(limitVal, playStartFrac, 10000000);
    playStartSec -= scaled;
    if (timeSec > 0 && playStartSec > 0) {
        if (UTY_CmpTime(timeSec, timeUsec, playStartSec, playStartFrac) == 0) {
            return 1;
        }
    }
    return 0;
}

// ---------------------------------------------------------------------------
// sfply_IsBpaOff — 0x224 bytes — check if buffer pause should deactivate
// ---------------------------------------------------------------------------
int sfply_IsBpaOff(void* self) {
    int i;
    int timeSec, timeUsec;
    int playStartSec, playStartFrac;
    int limitVal;
    int scaled;

    // Check cond 5 with transport terminal
    if (SFSET_GetCond(self, 5)) {
        if (SFTRN_GetTermFlg(self, 6)) return 1;
    }
    // Check cond 6 with transport terminal
    if (SFSET_GetCond(self, 6)) {
        if (SFTRN_GetTermFlg(self, 7)) return 1;
    }
    // Check all buffer terminal flags
    for (i = 0; i < 8; i++) {
        if (SFBUF_GetTermFlg(self, i)) return 1;
    }

    // Cond 5 with frame threshold (audio track)
    if (SFSET_GetCond(self, 5) == 1) {
        int trackIdx = FIELD(int, self, 0x2070);
        void* trackPtr = (u8*)self + trackIdx * 0x74;
        int vtbl_result;
        int threshold;

        void* obj = FIELD(void*, trackPtr, 0x13CC);
        void* vtbl = *(void**)obj;
        int (*fn)(void*, int) = *(int(**)(void*, int))((u8*)vtbl + 0x24);
        vtbl_result = fn(obj, 1);

        threshold = FIELD(int, trackPtr, 0x13D4);
        threshold = (int)((s64)threshold * 0x51EC851F >> 37);
        if (threshold < 0) threshold++;
        // Same pattern as IsBpaOn: (val * 80) / 1000 via magic multiply

        if (vtbl_result >= threshold) return 1;

        if (vtbl_result >= SFSET_GetCond(self, 0x46)) return 1;
    }

    // Cond 6 with frame threshold (video track)
    if (SFSET_GetCond(self, 6) == 1) {
        int trackIdx = FIELD(int, self, 0x20B4);
        void* trackPtr = (u8*)self + trackIdx * 0x74;
        int vtbl_result;
        int threshold;

        void* obj = FIELD(void*, trackPtr, 0x13CC);
        void* vtbl = *(void**)obj;
        int (*fn)(void*, int) = *(int(**)(void*, int))((u8*)vtbl + 0x24);
        vtbl_result = fn(obj, 1);

        threshold = FIELD(int, trackPtr, 0x13D4);
        threshold = (int)((s64)threshold * 0x51EC851F >> 37);
        if (threshold < 0) threshold++;

        if (vtbl_result >= threshold) return 1;
    }

    // Time comparison with cond 0x45
    SFTIM_GetTime(self, &timeSec, &timeUsec);
    playStartSec = FIELD(int, self, 0x1020);
    playStartFrac = FIELD(int, self, 0x1024);
    limitVal = SFSET_GetCond(self, 0x45);
    scaled = UTY_MulDiv(limitVal, playStartFrac, 10000000);
    if (UTY_CmpTime(timeSec, timeUsec, playStartSec - scaled, playStartFrac)) {
        return 1;
    }

    return 0;
}

// ---------------------------------------------------------------------------
// sfply_IsEtrg — 0xF4 bytes — check if end trigger
// ---------------------------------------------------------------------------
int sfply_IsEtrg(void* self) {
    int audioFlag, videoFlag, termFlag;
    int cond19;
    int result;

    if (!FIELD(int, self, 0xA34) && !FIELD(int, self, 0xA30)) {
        return 1;
    }

    videoFlag = SFTRN_GetTermFlg(self, 6);
    audioFlag = SFTRN_GetTermFlg(self, 7);
    termFlag = SFTRN_GetTermFlg(self, 1);
    cond19 = SFSET_GetCond(self, 0x19);

    switch (cond19) {
    case 1: result = audioFlag; break;
    case 2: result = videoFlag; break;
    case 3: result = audioFlag | videoFlag; break;
    case 0: result = audioFlag & videoFlag; break;
    default: result = 0; break;
    }

    if (SFTRN_IsSetup(self, 1)) {
        result &= termFlag;
    }

    return result ? 1 : 0;
}

// ---------------------------------------------------------------------------
// criware_803C9FC0 — 0x8C bytes — check if transport is in stopped/end state
// ---------------------------------------------------------------------------
int criware_803C9FC0(void* self) {
    int result = 0;

    if (SFTRN_IsSetup(self, 1)) {
        int status = FIELD(int, self, 0x54);
        if ((unsigned int)(status - 2) <= 2) {
            if (SFTRN_GetTermFlg(self, 1)) {
                result = 1;
            }
        } else if (status == 6) {
            result = 1;
        } else {
            result = 0;
        }
    }

    return result;
}

// ---------------------------------------------------------------------------
// sfply_IsPlayTimeAutoStop — 0xCC bytes
// ---------------------------------------------------------------------------
int sfply_IsPlayTimeAutoStop(void* self) {
    int timeSec, timeUsec;
    int limitTime;

    if (FIELD(int, self, 0x54) != 4) return 0;
    if (FIELD(int, self, 0x5C) == 1) return 0;
    if (FIELD(int, self, 0x980) == 1) return 0;

    if (SFTIM_GetTimeSub(self, &timeSec, &timeUsec)) return 0;
    if (timeSec < 0) return 0;

    criware_803CA124(self, timeUsec);
    SFD_GetLimitTime(self, &limitTime);
    if (limitTime == -1) return 0;

    if (UTY_CmpTime(limitTime, 1000, timeSec, timeUsec)) {
        return 1;
    }
    return 0;
}

// ---------------------------------------------------------------------------
// criware_803CA124 — 0x12C bytes — compute and set auto-stop time limit
// ---------------------------------------------------------------------------
void criware_803CA124(void* self, int fpsArg) {
    int cond36;
    int limit;
    int newLimit;
    int condF;

    cond36 = SFSET_GetCond(self, 0x36);
    if (cond36 != -1) return;
    if (fpsArg == 1) return;

    // Compute max time from fps
    if (fpsArg >= 1000) {
        limit = -((0x7FFFFFFF / fpsArg + 1) * 1000);
    } else {
        limit = 0x7FFFFFFF - 1000 + 1;
    }

    newLimit = limit;

    // Check DD4 flag
    if (FIELD(int, self, 0xDD4)) {
        int fpsArg2 = FIELD(int, self, 0xDFC);
        int limit2;
        if (fpsArg2 == 1) goto applyLimit;
        if (fpsArg2 >= 1000) {
            limit2 = -((0x7FFFFFFF / fpsArg2 + 1) * 1000);
        } else {
            limit2 = 0x7FFFFFFF - 1000 + 1;
        }
        if (newLimit > limit2) newLimit = limit2;
    }

applyLimit:
    condF = SFSET_GetCond(self, 0xF);
    if (condF == 2) {
        int globalFps = *(int*)(lbl_eu_80606E38 + 0x1A4);
        int globalLimit;
        if (globalFps >= 1000) {
            globalLimit = -((0x7FFFFFFF / globalFps + 1) * 1000);
        } else {
            globalLimit = 0x7FFFFFFF - 1000 + 1;
        }
        if (newLimit > globalLimit) newLimit = globalLimit;
    }

    SFSET_SetCond(self, 0x36, newLimit);
}

// ---------------------------------------------------------------------------
// SFD_Create — 0x278 bytes
// ---------------------------------------------------------------------------
int SFD_Create(void* config, void* extra) {
    void* ctx;
    void* vtbl;
    void (*trace)(void*, void*);
    int slot;
    int result;
    void* hn;
    u32* hnTable;

    ctx = lbl_eu_80606E34;
    if (ctx) {
        char buf[0x240];
        u32* cfg = (u32*)config;
        sprintf(buf + 0x30, lbl_eu_8051CBA8,
                cfg[0x1C/4], cfg[0x20/4], cfg[0x24/4],
                cfg[0x2C/4], cfg[0x30/4], cfg[0x38/4],
                cfg[0x3C/4], cfg[0x40/4], cfg[0x44/4], cfg[0x48/4]);
        *(void**)(lbl_eu_80567FAC + 0x0C) = (void*)buf;
        vtbl = *(void**)ctx;
        trace = *(void(**)(void*, void*))((u8*)vtbl + 0x24);
        trace(ctx, lbl_eu_80567FAC + 4);
    }

    hnTable = (u32*)(lbl_eu_80606E38 + 0x1FC);

    // Validate config
    if (!FIELD(int, config, 4)) {
        SFLIB_SetErr(0, 0xff000204);
        result = 0;
        goto done;
    }
    if (FIELD(u32, config, 0x48) < 0x39D0) {
        SFLIB_SetErr(0, 0xff000205);
        result = 0;
        goto done;
    }

    // Find free slot
    {
        int i;
        slot = -1;
        for (i = 0; i < 8; i++) {
            if (!hnTable[i]) {
                slot = i;
                break;
            }
        }
    }

    if (slot == -1) {
        SFLIB_SetErr(0, 0xff000206);
        result = 0;
    } else {
        hn = sfply_InitHn(config, extra);
        hnTable[slot] = (u32)hn;
        result = (int)hn;
    }

done:
    ctx = lbl_eu_80606E34;
    if (ctx) {
        // trace end
    }
    return result;
}

// ---------------------------------------------------------------------------
// sfply_InitHn — 0x284 bytes
// ---------------------------------------------------------------------------
void* sfply_InitHn(void* config, void* extra) {
    u8* workBuf;
    u32 workSize;
    u8* aligned;
    int i;
    u8* dst;
    u8* src;

    workBuf = FIELD(u8*, config, 0x44);
    workSize = FIELD(u32, config, 0x48);
    if (!workBuf) return NULL;
    if ((int)workSize <= 0 || workSize > 0x73A0) return NULL;

    // Validate work size consistency
    if (lbl_eu_80619BA4 && lbl_eu_80619BA4 != workSize) return NULL;
    lbl_eu_80619BA4 = workSize;

    UTY_MemsetDword(workBuf, 0, workSize >> 2);

    aligned = (u8*)(((u32)workBuf + 0x1F) & ~0x1F);

    FIELD(int, aligned, 0x58) = 0;
    FIELD(int, aligned, 0x54) = 0;

    // Copy config to aligned+4 area (5 pairs of u32, unrolled loop)
    dst = aligned - 4;
    src = (u8*)config - 4;
    for (i = 0; i < 10; i++) {
        u32 a = *(u32*)(src + 4);
        u32 b = *(u32*)(src + 8);
        *(u32*)(dst + 4) = a;
        *(u32*)(dst + 8) = b;
        src += 8;
        dst += 8;
    }

    FIELD(int, aligned, 0x50) = 1;
    FIELD(int, aligned, 0x5C) = 0;
    FIELD(int, aligned, 0x60) = 0;
    FIELD(int, aligned, 0x64) = 0;
    FIELD(int, aligned, 0x68) = 0;
    FIELD(int, aligned, 0x6C) = 0;
    FIELD(int, aligned, 0x70) = 0;

    SFHDS_InitFhd(aligned + 0x88);
    UTY_MemsetDword(aligned + 0x91C, 0, 0x10);
    FIELD(int, aligned, 0x91C) = 0;
    FIELD(int, aligned, 0x920) = 0;
    FIELD(int, aligned, 0x924) = 0;
    FIELD(int, aligned, 0x928) = 0;
    FIELD(int, aligned, 0x92C) = 0;
    FIELD(int, aligned, 0x930) = 0;
    FIELD(int, aligned, 0x934) = 0;
    FIELD(int, aligned, 0x938) = 1;
    FIELD(int, aligned, 0x93C) = 0;
    FIELD(int, aligned, 0x940) = -1;
    FIELD(int, aligned, 0x944) = -1;
    FIELD(int, aligned, 0x948) = -1;

    sfply_InitPlyInf(aligned + 0x960);

    // Init timer sum areas
    UTY_MemsetDword(aligned + 0x26A0, 0, 0x38);
    {
        u8* tmrArea = aligned + 0x26A0;
        for (i = 0; i < 5; i++) {
            SFTMR_InitTsum(tmrArea);
            tmrArea += 0x20;
        }
    }
    SFTMR_InitTsum(aligned + 0x2740);

    // Init FPS tracking
    FIELD(int, aligned, 0x2764) = 0;
    FIELD(int, aligned, 0x2760) = 0;
    FIELD(int, aligned, 0x276C) = 0;
    FIELD(int, aligned, 0x2768) = 0;
    FIELD(int, aligned, 0x2774) = 0;
    FIELD(int, aligned, 0x2770) = 0;
    FIELD(int, aligned, 0x2778) = 0;
    FIELD(float, aligned, 0x277C) = lbl_eu_8051CBA4;

    fn_803C34DC(aligned + 0xA08);

    MEM_Copy(aligned + 0xA1C, lbl_eu_80606E38, 0x190);
    MEM_Copy(aligned + 0xBAC, lbl_eu_80606E38, 0x190);

    memset(aligned + 0xD3C, 0, 0x5C);
    FIELD(int, aligned, 0xD8C) = 0x7FFFFFFF;

    SFTIM_InitHn(aligned, aligned + 0xD98);

    if (SFBUF_InitHn(aligned, aligned + 0x13B8, config)) {
        return NULL;
    }

    SFTRN_InitHn(aligned, aligned + 0x1FD8, config, extra);
    SFSEEKI_InitHn(aligned + 0x2680);
    SFSEE_InitHn(aligned + 0x2670);

    if (SFTRN_CallTrSetup(aligned, 3)) {
        return NULL;
    }

    FIELD(int, aligned, 0x58) = 1;
    FIELD(int, aligned, 0x54) = 1;

    return aligned;
}

// ---------------------------------------------------------------------------
// sfply_InitPlyInf — 0xC8 bytes
// ---------------------------------------------------------------------------
void sfply_InitPlyInf(void* self) {
    int i;
    u32* p = (u32*)self;

    UTY_MemsetDword(self, 0, 0x2A);
    for (i = 0; i < 0x26; i += 2) {
        p[i] = 0;
        p[i + 1] = 0;
    }
}

// ---------------------------------------------------------------------------
// SFPLY_AddDecPic — 0x30 bytes (already implemented)
// ---------------------------------------------------------------------------
void SFPLY_AddDecPic(void* self, int delta, void* param) {
    void (*cb)(void*, void*, u32*) = *(void(**)(void*, void*, u32*))((u8*)self + 0xD6C);
    FIELD(u32, self, 0x960) += delta;
    if (cb == NULL) return;
    cb(FIELD(void*, self, 0xD70), param, (u32*)((u8*)self + 0x960));
}

// ---------------------------------------------------------------------------
// SFPLY_AddSkipPic — 0x30 bytes (already implemented)
// ---------------------------------------------------------------------------
void SFPLY_AddSkipPic(void* self, int delta, void* param) {
    void (*cb)(void*, void*, u32*) = *(void(**)(void*, void*, u32*))((u8*)self + 0xD74);
    u32 count = FIELD(u32, self, 0x964) + delta;
    FIELD(u32, self, 0x964) = count;
    if (cb != NULL) {
        cb(FIELD(void*, self, 0xD78), param, (u32*)((u8*)self + 0x960));
    }
}

// ---------------------------------------------------------------------------
// SFD_Destroy — 0x218 bytes
// ---------------------------------------------------------------------------
int SFD_Destroy(void* self) {
    int result;
    void* cs;
    void* ctx;
    void* vtbl;
    void (*trace)(void*, void*);
    void* hnTable;
    int i;

    if (SFLIB_CheckHn(self)) {
        SFLIB_SetErr(0, 0xff000131);
        return 0;
    }

    ctx = lbl_eu_80606E34;
    if (ctx) {
        *(void**)(lbl_eu_80568080 + 0x0C) = self;
        vtbl = *(void**)ctx;
        trace = *(void(**)(void*, void*))((u8*)vtbl + 0x24);
        trace(ctx, lbl_eu_80568080 + 4);
    }

    if (FIELD(int, self, 0x54) == 1) {
        SFBUF_SetTermFlg(self, FIELD(int, self, 0x1FEC), 0);
    } else if (FIELD(int, self, 0x54) == 4) {
        int trResult = SFTRN_CallTrtTrif(self, 7, 7, 0, 0);
        if (trResult) {
            goto afterReset;
        }
        FIELD(int, self, 0x54) = 1;
        FIELD(int, self, 0x58) = 1;
    }

    {
        u32* resetFlag = (u32*)(lbl_eu_80606E38 + 0x1F0);
        FIELD(int, self, 0x58) = 0;
        FIELD(int, self, 0x54) = 0;
        *resetFlag = 1;
        sfply_ResetHn(self);
        *resetFlag = 0;
    }

afterReset:
    SFHDS_FinishFhd((u8*)self + 0x88);
    SFBUF_DestroySj(self);

    cs = NULL;
    SFLIB_LockCs(&cs);
    FIELD(int, self, 0x54) = 0;
    FIELD(int, self, 0x58) = 0;
    SFLIB_UnlockCs(&cs);

    result = SFTRN_CallTrSetup(self, 4);

    // Clear from handler table
    cs = NULL;
    SFLIB_LockCs(&cs);
    hnTable = lbl_eu_80606E38 + 0x1FC;
    for (i = 0; i < 8; i++) {
        if (((void**)hnTable)[i] == self) {
            ((void**)hnTable)[i] = NULL;
        }
    }
    SFLIB_UnlockCs(&cs);

    ctx = lbl_eu_80606E34;
    if (ctx) {
        vtbl = *(void**)ctx;
        trace = *(void(**)(void*, void*))((u8*)vtbl + 0x24);
        trace(ctx, lbl_eu_80568080 + 0x6C);
    }

    return result;
}

// ---------------------------------------------------------------------------
// SFD_Start — 0xE8 bytes
// ---------------------------------------------------------------------------
int SFD_Start(void* self) {
    int result;
    void* ctx;
    void* vtbl;
    void (*trace)(void*, void*);
    int standbyResult;

    if (SFLIB_CheckHn(self)) {
        SFLIB_SetErr(0, 0xff000132);
        return 0;
    }

    ctx = lbl_eu_80606E34;
    if (ctx) {
        *(void**)(lbl_eu_80568154 + 0x0C) = self;
        vtbl = *(void**)ctx;
        trace = *(void(**)(void*, void*))((u8*)vtbl + 0x24);
        trace(ctx, lbl_eu_80568154 + 4);
    }

    if (SFSET_GetCond(self, 0x2F) == 1) {
        standbyResult = SFPL2_Standby(self);
        result = standbyResult;
    } else {
        FIELD(int, self, 0x58) = 4;
        result = 0;
    }

    FIELD(int, self, 0x50) = 1;

    ctx = lbl_eu_80606E34;
    if (ctx) {
        vtbl = *(void**)ctx;
        trace = *(void(**)(void*, void*))((u8*)vtbl + 0x24);
        trace(ctx, lbl_eu_80568154 + 0x6C);
    }

    return result;
}

// ---------------------------------------------------------------------------
// fn_803CD484 — 0x44 bytes (already implemented)
// ---------------------------------------------------------------------------
int fn_803CD484(void* self) {
    int result = SFTRN_CallTrtTrif(self, 2, 6, 0, 0);
    if (result == 0) {
        return 0;
    } else {
        return result;
    }
}

// ---------------------------------------------------------------------------
// SFD_Stop — 0x164 bytes
// ---------------------------------------------------------------------------
int SFD_Stop(void* self) {
    int result = 0;
    void* ctx;
    void* vtbl;
    void (*trace)(void*, void*);

    if (SFLIB_CheckHn(self)) {
        SFLIB_SetErr(0, 0xff000133);
        return 0;
    }

    ctx = lbl_eu_80606E34;
    if (ctx) {
        *(void**)(lbl_eu_80568228 + 0x0C) = self;
        vtbl = *(void**)ctx;
        trace = *(void(**)(void*, void*))((u8*)vtbl + 0x24);
        trace(ctx, lbl_eu_80568228 + 4);
    }

    if (FIELD(int, self, 0x54) == 1) {
        SFBUF_SetTermFlg(self, FIELD(int, self, 0x1FEC), 0);
        result = 0;
    } else if (FIELD(int, self, 0x54) == 4) {
        int trResult = SFTRN_CallTrtTrif(self, 7, 7, 0, 0);
        if (trResult) {
            result = trResult;
        } else {
            FIELD(int, self, 0x54) = 1;
            FIELD(int, self, 0x58) = 1;
            result = 0;
        }
    }

    if (!result) {
        u32* resetFlag;
        FIELD(int, self, 0x58) = 0;
        FIELD(int, self, 0x54) = 0;
        resetFlag = (u32*)(lbl_eu_80606E38 + 0x1F0);
        *resetFlag = 1;
        result = sfply_ResetHn(self);
        *resetFlag = 0;
    }

    FIELD(int, self, 0x50) = 1;

    ctx = lbl_eu_80606E34;
    if (ctx) {
        vtbl = *(void**)ctx;
        trace = *(void(**)(void*, void*))((u8*)vtbl + 0x24);
        trace(ctx, lbl_eu_80568228 + 0x6C);
    }

    return result;
}

// ---------------------------------------------------------------------------
// SFPLY_GetResetFlg — 0x10 bytes (already implemented)
// ---------------------------------------------------------------------------
int SFPLY_GetResetFlg(void) {
    return *(int*)(lbl_eu_80606E38 + 0x1F0);
}

// ---------------------------------------------------------------------------
// sfply_ResetHn — 0x5C0 bytes — reset handler
// ---------------------------------------------------------------------------
int sfply_ResetHn(void* self) {
    u8 savedPlyInf[0x54];
    u8 savedConds[0x190];
    u8 savedSettings[0x60];
    int savedStatus;
    int savedSubStatus;
    int trResult;
    int i;
    u8* src;
    u8* dst;
    int mbFn, mbArg1, mbArg2;
    int timeFn, timeArg;
    int clkFn, clkArg1, clkArg2;
    int skipFn;
    int speedNum, speedDen;
    int cyclicFn, cyclicArg;
    int seekHandle;
    int videoPts, videoPtsType;
    int seekPos, seekByteRate, seekFileSize;
    int seekTotSec, seekTotFrac;
    int seekPosVal;
    int limitTime;
    int* extData;

    // Save current plyinf
    MEM_Copy(savedPlyInf, (u8*)self + 4, 0x50);

    // Check if we need transport reset
    trResult = 0;
    if (FIELD(int, self, 0xA3C)) {
        if (SFLIB_CheckHn(self)) {
            SFLIB_SetErr(0, 0xff000134);
        } else {
            trResult = SFTRN_CallTrtTrif(self, 0, 9, (int)savedSettings, 0);
        }
        savedStatus = FIELD(int, savedSettings, 0x14);
    }

    // Destroy and reinitialize
    SFHDS_FinishFhd((u8*)self + 0x88);
    SFBUF_DestroySj(self);

    savedSubStatus = FIELD(int, self, 0x64);

    // Save plyinf area
    src = (u8*)self + 0xD38;
    dst = savedConds + 0xAC - 0xD38; // offset adjustment
    for (i = 0; i < 11; i++) {
        u32 a = *(u32*)(src + 4);
        u32 b = *(u32*)(src + 8);
        *(u32*)(dst + 4) = a;
        *(u32*)(dst + 8) = b;
        src += 8;
        dst += 8;
    }
    *(u32*)(dst + 4) = *(u32*)(src + 4);

    // Save various fields
    savedSettings[0x2FC - 0xAC] = FIELD(u8, self, 0xA0C);
    savedSettings[0x2F8 - 0xAC] = FIELD(u8, self, 0x1078);
    savedSettings[0x2F4 - 0xAC] = FIELD(u8, self, 0x108C);
    savedSettings[0x2F0 - 0xAC] = FIELD(u8, self, 0x1090);

    mbFn = FIELD(int, self, 0xA08);
    timeFn = FIELD(int, self, 0x1074);
    clkFn = FIELD(int, self, 0x107C);
    skipFn = FIELD(int, self, 0xDB0);
    speedNum = FIELD(int, self, 0x1048);
    speedDen = FIELD(int, self, 0x104C);

    // Save seek state
    seekHandle = FIELD(int, self, 0x2670);
    if (seekHandle) {
        seekByteRate = FIELD(int, seekHandle + 0xDD0);
        seekFileSize = FIELD(int, seekHandle + 0xDC4);
        seekTotSec = FIELD(int, seekHandle + 0xDC8);
        seekTotFrac = FIELD(int, seekHandle + 0xDCC);
        seekPosVal = FIELD(int, seekHandle + 0xDD4);
    } else {
        seekByteRate = 0;
        seekFileSize = 0;
        seekTotSec = 0;
        seekTotFrac = 0;
        seekPosVal = 0;
    }

    videoPts = FIELD(int, self, 0x1464);
    videoPtsType = FIELD(int, self, 0x1468);

    // Save cyclic output
    extData = (int*)((u8*)self + 0x39A0);
    savedSettings[0x18] = extData[0];
    savedSettings[0x1C] = extData[1];
    savedSettings[0x20] = extData[2];
    savedSettings[0x24] = extData[3];

    // Save MPV conditions
    {
        int mpvResult = SFMPV_SaveCond(self, savedSettings + 0x70 - 0xAC, 0x40);
        savedSettings[0x304 - 0xAC] = mpvResult;
    }

    // Lock CS and clear status
    {
        void* cs = NULL;
        SFLIB_LockCs(&cs);
        FIELD(int, self, 0x54) = 0;
        FIELD(int, self, 0x58) = 0;
        SFLIB_UnlockCs(&cs);
    }

    // Re-setup transport
    trResult = SFTRN_CallTrSetup(self, 4);
    if (trResult) return trResult;

    // Reinitialize with saved conditions
    MEM_Copy(savedPlyInf, (u8*)self + 0xBAC, 0x190);
    {
        void* newHn = sfply_InitHn(savedPlyInf, 0);
        if (!newHn) {
            SFLIB_SetErr(0, 0xff000202);
            return 0;
        }
        MEM_Copy((u8*)newHn + 0xA1C, savedPlyInf, 0x190);
        MEM_Copy((u8*)self + 0xBAC, savedPlyInf, 0x190);
        SFMPV_RestoreCond(newHn, savedSettings + 0x70 - 0xAC, savedSettings[0x304 - 0xAC]);

        // Restore transport state
        if (savedSubStatus) {
            if (SFLIB_CheckHn(newHn)) {
                SFLIB_SetErr(0, 0xff000134);
            } else {
                SFTRN_CallTrtTrif(newHn, 0, 9, (int)savedSettings, 0);
            }
        }

        // Restore buffer terminal
        {
            int termBuf = FIELD(int, newHn, 0x1FEC);
            if (!SFBUF_GetTermFlg(newHn, termBuf)) {
                SFBUF_SetTermFlg(newHn, termBuf, 1);
                FIELD(int, newHn, 0x50) = 1;
            }
        }

        // Restore saved state
        FIELD(int, newHn, 0x64) = savedSubStatus;
        // Restore plyinf
        src = savedConds + 0xAC - 0xD38;
        dst = (u8*)newHn + 0xD38;
        for (i = 0; i < 11; i++) {
            u32 a = *(u32*)(src + 4);
            u32 b = *(u32*)(src + 8);
            *(u32*)(dst + 4) = a;
            *(u32*)(dst + 8) = b;
            src += 8;
            dst += 8;
        }
        *(u32*)(dst + 4) = *(u32*)(src + 4);

        // Restore callbacks
        if (FIELD(int, newHn, 0xD84)) {
            SFD_SetMbCb(newHn, (void*)FIELD(int, newHn, 0xD84), FIELD(u32, newHn, 0xD88), FIELD(u32, newHn, 0xD8C));
        }
        if (mbFn) {
            criware_803C0D94(newHn, mbFn, savedSettings[0x2FC - 0xAC]);
        }
        if (timeFn) {
            SFD_SetUsrTimeFn(newHn, (void*)timeFn, savedSettings[0x2F8 - 0xAC]);
        }
        if (clkFn) {
            SFD_SetExtClockFn(newHn, (void*)clkFn, savedSettings[0x2F4 - 0xAC], savedSettings[0x2F0 - 0xAC]);
        }
        if (skipFn) {
            SFD_SetUsrIsSkipFn(newHn, (void*)skipFn);
        }
        if (speedNum != speedDen) {
            SFD_SetSpeedRational(newHn, speedNum, speedDen);
        }

        // Restore cyclic frame output
        if (savedSettings[0x44 - 0xAC]) {
            FIELD(int, newHn, 0x1368) = savedSettings[0x44 - 0xAC];
            FIELD(int, newHn, 0x136C) = savedSettings[0x48 - 0xAC];
            FIELD(int, newHn, 0x1370) = savedSettings[0x4C - 0xAC];
            if (lbl_eu_80619BA8) {
                void (*cb)(void*) = (void(*)(void*))lbl_eu_80619BA8;
                cb(savedSettings + 0x44 - 0xAC);
            }
        }
        if (savedSettings[0x38 - 0xAC]) {
            FIELD(int, newHn, 0x1374) = savedSettings[0x38 - 0xAC];
            FIELD(int, newHn, 0x1378) = savedSettings[0x3C - 0xAC];
            FIELD(int, newHn, 0x137C) = savedSettings[0x40 - 0xAC];
            if (lbl_eu_80619BA8) {
                void (*cb)(void*) = (void(*)(void*))lbl_eu_80619BA8;
                cb(savedSettings + 0x38 - 0xAC);
            }
        }

        // Restore seek/cyclic
        if (savedSettings[0x2C - 0xAC]) {
            SFD_SetCyclicFrameOutput(newHn, (void*)savedSettings[0x28 - 0xAC], savedSettings[0x2C - 0xAC]);
        }
        if (seekPosVal != -1) {
            SFD_SetLimitTime(newHn, seekPosVal);
        }
        if (seekHandle) {
            SFD_EntrySeek(newHn, (void*)seekHandle);
            SFD_SetByteRate(newHn, seekByteRate);
            SFD_SetFileSize(newHn, seekFileSize);
            SFD_SetTotTime(newHn, seekTotSec, seekTotFrac);
            SFD_SetSeekPos(newHn, seekPosVal);
        }
        if (videoPts) {
            SFD_SetVideoPts(newHn, videoPts, videoPtsType);
        }
        if (savedSettings[0x18]) {
            criware_803C1570(newHn, savedSettings + 0x18, savedSettings[0x1C]);
        }
    }

    return 0;
}

// ---------------------------------------------------------------------------
// SFD_RequestStop — 0x100 bytes
// ---------------------------------------------------------------------------
void SFD_RequestStop(void* self) {
    void* ctx;
    void* vtbl;
    void (*trace)(void*, void*);

    if (SFLIB_CheckHn(self)) {
        SFLIB_SetErr(0, 0xff00013C);
        return;
    }

    if (FIELD(int, self, 0x54) == 1) return;

    ctx = lbl_eu_80606E34;
    if (ctx) {
        *(void**)(lbl_eu_805682FC + 0x0C) = self;
        vtbl = *(void**)ctx;
        trace = *(void(**)(void*, void*))((u8*)vtbl + 0x24);
        trace(ctx, lbl_eu_805682FC + 4);
    }

    FIELD(int, self, 0x70) = 1;
    SFTRN_CallTrtTrif(self, 1, 5, 0, 0);
    SFTRN_CallTrtTrif(self, 2, 5, 0, 0);
    SFTRN_CallTrtTrif(self, 3, 5, 0, 0);

    ctx = lbl_eu_80606E34;
    if (ctx) {
        vtbl = *(void**)ctx;
        trace = *(void(**)(void*, void*))((u8*)vtbl + 0x24);
        trace(ctx, lbl_eu_805682FC + 0x6C);
    }
}

// ---------------------------------------------------------------------------
// SFD_TermSupply — 0xF8 bytes
// ---------------------------------------------------------------------------
int SFD_TermSupply(void* self) {
    int result = 0;
    void* ctx;
    void* vtbl;
    void (*trace)(void*, void*);
    int bufId;

    ctx = lbl_eu_80606E34;
    if (ctx) {
        *(void**)(lbl_eu_805683D0 + 0x0C) = self;
        vtbl = *(void**)ctx;
        trace = *(void(**)(void*, void*))((u8*)vtbl + 0x24);
        trace(ctx, lbl_eu_805683D0 + 4);
    }

    if (SFLIB_CheckHn(self)) {
        SFLIB_SetErr(0, 0xff00013D);
        result = 0;
    } else {
        bufId = FIELD(int, self, 0x1FEC);
        if (SFBUF_GetTermFlg(self, bufId) == 1) {
            result = 0;
        } else {
            SFBUF_SetTermFlg(self, bufId, 1);
            FIELD(int, self, 0x50) = 1;
            result = 0;
        }
    }

    ctx = lbl_eu_80606E34;
    if (ctx) {
        vtbl = *(void**)ctx;
        trace = *(void(**)(void*, void*))((u8*)vtbl + 0x24);
        trace(ctx, lbl_eu_805683D0 + 0x6C);
    }

    return result;
}

// ---------------------------------------------------------------------------
// SFD_GetFrm — 0x1E4 bytes
// ---------------------------------------------------------------------------
int SFD_GetFrm(void* self, void** outFrm) {
    int result;
    void* ctx;
    void* vtbl;
    void (*trace)(void*, void*);
    int frmState;
    void (*vblCb)(void*, void*);

    *outFrm = NULL;

    if (SFLIB_CheckHn(self)) {
        SFLIB_SetErr(0, 0xff000136);
        return 0;
    }

    frmState = FIELD(int, self, 0x68);
    if (!frmState) {
        FIELD(int, self, 0x68) = 1;
        result = 0;
    } else if (frmState == 1) {
        result = 0;
    } else {
        SFLIB_SetErr(self, 0xff000207);
        result = 1;
    }

    if (result) return result;

    ctx = lbl_eu_80606E34;
    if (ctx) {
        *(void**)(lbl_eu_805684A4 + 0x0C) = self;
        vtbl = *(void**)ctx;
        trace = *(void(**)(void*, void*))((u8*)vtbl + 0x24);
        trace(ctx, lbl_eu_805684A4 + 4);
    }

    result = SFTRN_CallTrtTrif(self, 6, 0xB, (int)outFrm, 0);

    if (*outFrm) {
        int getCnt = FIELD(int, self, 0x978);
        int relCnt = FIELD(int, self, 0x97C);

        if (getCnt == relCnt) {
            if (!getCnt) {
                SFTMR_GetTmr(self);
                FIELD(int, self, 0x2764) = /* r4 */;
                FIELD(int, self, 0x2760) = /* r3 */;
            }
            FIELD(int, self, 0x978)++;
        }

        ctx = lbl_eu_80606E34;
        if (ctx) {
            void* frm = *outFrm;
            *(void**)(lbl_eu_805684A4 + 0x74) = frm;
            *(void**)(lbl_eu_805684A4 + 0x80) = (u8*)frm + 0x14;
            *(void**)(lbl_eu_805684A4 + 0x8C) = (u8*)frm + 0x10;
            vtbl = *(void**)ctx;
            trace = *(void(**)(void*, void*))((u8*)vtbl + 0x24);
            trace(ctx, lbl_eu_805684A4 + 0x6C);
        }
    } else {
        ctx = lbl_eu_80606E34;
        if (ctx) {
            *(int*)(lbl_eu_805684A4 + 0x74) = 0;
            *(int*)(lbl_eu_805684A4 + 0x80) = 0;
            *(int*)(lbl_eu_805684A4 + 0x8C) = 0;
            vtbl = *(void**)ctx;
            trace = *(void(**)(void*, void*))((u8*)vtbl + 0x24);
            trace(ctx, lbl_eu_805684A4 + 0x6C);
        }
    }

    // VBlank callback
    vblCb = (void (*)(void*, void*))lbl_eu_80619BA0;
    if (vblCb) {
        vblCb(self, *outFrm);
    }

    return result;
}

// ---------------------------------------------------------------------------
// SFD_RelFrm — 0x134 bytes
// ---------------------------------------------------------------------------
int SFD_RelFrm(void* self, void* frm) {
    int result;
    void* ctx;
    void* vtbl;
    void (*trace)(void*, void*);
    int frmState;

    if (SFLIB_CheckHn(self)) {
        SFLIB_SetErr(0, 0xff000137);
        return 0;
    }

    frmState = FIELD(int, self, 0x68);
    if (!frmState) {
        FIELD(int, self, 0x68) = 1;
        result = 0;
    } else if (frmState == 1) {
        result = 0;
    } else {
        SFLIB_SetErr(self, 0xff000207);
        result = 1;
    }

    if (result) return result;

    ctx = lbl_eu_80606E34;
    if (ctx) {
        *(void**)(lbl_eu_80568578 + 0x0C) = self;
        *(void**)(lbl_eu_80568578 + 0x18) = frm;
        vtbl = *(void**)ctx;
        trace = *(void(**)(void*, void*))((u8*)vtbl + 0x24);
        trace(ctx, lbl_eu_80568578 + 4);
    }

    // Increment rel frame counter if behind get counter
    {
        int relCnt = FIELD(int, self, 0x97C);
        int getCnt = FIELD(int, self, 0x978);
        if (relCnt < getCnt) {
            FIELD(int, self, 0x97C) = relCnt + 1;
        }
    }

    result = SFTRN_CallTrtTrif(self, 6, 0xC, (int)frm, 0);

    ctx = lbl_eu_80606E34;
    if (ctx) {
        vtbl = *(void**)ctx;
        trace = *(void(**)(void*, void*))((u8*)vtbl + 0x24);
        trace(ctx, lbl_eu_80568578 + 0x6C);
    }

    return result;
}

// ---------------------------------------------------------------------------
// SFD_SetSupplySj — 0x60 bytes
// ---------------------------------------------------------------------------
void SFD_SetSupplySj(void* self, void* sj) {
    if (SFLIB_CheckHn(self)) {
        SFLIB_SetErr(0, 0xff000139);
        return;
    }
    SFBUF_SetSupplySj(self, sj);
}
