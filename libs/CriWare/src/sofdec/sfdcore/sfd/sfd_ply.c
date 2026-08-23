// CriWare Sofdec player core - sfd_ply TU
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
void* sfply_InitHn(void* config, void* extra);
void sfply_InitPlyInf(void* self);
int sfply_ResetHn(void* self);

// External functions
extern int SFLIB_CheckHn(void* hn);
extern s32 SFLIB_SetErr(void* hn, u32 errCode);
extern void SFLIB_LockCs(void* cs);
extern void SFLIB_UnlockCs(void* cs);
extern void SFTIM_VbIn(void);
extern void SFTIM_GetTime(void* hn, int* sec, int* usec);
extern int SFTIM_GetTimeSub(void* hn, int* sec, int* usec);
extern int SFTIM_IsStagnant(void* hn);
extern void SFTIM_InitHn(void* hn, void* area);
extern u64 SFTMR_GetTmr(void* hn);
extern u64 SFTMR_GetTmrUnit(void* hn);
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
extern void fn_803C0D94(void* hn, u32 val, u32 arg);
extern void fn_803C1570(void* hn, void* data, u32 arg);
extern void fn_803C34DC(void* area);
extern int sprintf(char* str, const char* fmt, ...);

// Global data
extern void (*lbl_eu_80619BA0)(void*, void*);
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

// Field access helper
#define FIELD(type, base, off) (*((type*)((u8*)(base) + (off))))

// SofDec global work area (lbl_eu_80606E38)
typedef struct SfdGlob {
    u8 pad0[0x1F0];
    int resetFlg;       /* 0x1F0 */
    u8 pad1[0x8];
    void* hnSlots[8];   /* 0x1FC */
} SfdGlob;
#define SFD_GLOB ((SfdGlob*)lbl_eu_80606E38)

// Trace helper - calls ctx->vtable[9](ctx, traceblock + offset)
static void traceCall(void* traceBlock, int offset) {
    void* ctx = lbl_eu_80606E34;
    if (ctx) {
        void* vtbl = *(void**)ctx;
        void (*trace)(void*, void*) = *(void(**)(void*, void*))((u8*)vtbl + 0x24);
        trace(ctx, (u8*)traceBlock + offset);
    }
}

// ---------------------------------------------------------------------------
// SFPLY_Init
// ---------------------------------------------------------------------------
void SFPLY_Init(void) {
    lbl_eu_80619BA0 = NULL;
}

// ---------------------------------------------------------------------------
// SFD_VbIn
// ---------------------------------------------------------------------------
void SFD_VbIn(void) {
    void* cs;
    SFLIB_LockCs(&cs);
    SFTIM_VbIn();
    SFLIB_UnlockCs(&cs);
}

// ---------------------------------------------------------------------------
// SFD_IsHnSvrWait
// ---------------------------------------------------------------------------
int SFD_IsHnSvrWait(void* self) {
    int status = FIELD(int, self, 0x54);
    if ((unsigned int)(status - 1) > 3) return 1;
    if (FIELD(int, self, 0x64) == 1) return 1;
    return (FIELD(int, self, 0x50) == 0) ? 1 : 0;
}

// ---------------------------------------------------------------------------
// SFD_ExecOne
// ---------------------------------------------------------------------------
int SFD_ExecOne(void* self) {
    void* ctx;
    void* vtbl;
    void (*trace)(void*, void*);

    if (SFLIB_CheckHn(self)) {
        return SFLIB_SetErr(0, 0xff000138);
    } else {
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
}

// ---------------------------------------------------------------------------
// sfply_ExecOne - main player state machine
// ---------------------------------------------------------------------------
void sfply_ExecOne(void* self) {
    int status;
    int canExec;
    int newState;
    void* cs;
    int pauseResult;

    status = FIELD(int, self, 0x54);
    if ((unsigned int)(status - 1) > 3) return;
    if (FIELD(int, self, 0x50) == 0) return;

    FIELD(int, self, 0x50) = 0;
    if (!fn_803C1CAC(self)) return;

    {
        int ret = SFBUF_GetTermFlg(self, FIELD(int, self, 0x1FEC));
        canExec = (((-ret) | ret) >> 31);
    }

    if (FIELD(int, self, 0x64) == 1) return;

    // Sub-state processing
    if ((unsigned int)(status - 2) <= 2) {
        SFTRN_CallTrSetup(self, 2);
        SFSEE_ExecServer(self);
    }

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
        int nextSub = FIELD(int, self, 0x58);
        switch (nextSub) {
        case 2: newState = 2; break;
        case 3: newState = 3; break;
        case 4: case 6: {
            int shouldStop = 0;
            if (!FIELD(int, self, 0xA54)) shouldStop = 1;
            else if (!FIELD(int, self, 0xA30)) shouldStop = 1;
            else if (FIELD(int, self, 0x1050)) shouldStop = 1;
            else if (FIELD(int, self, 0x106C) >= FIELD(int, self, 0xAD0)) shouldStop = 1;
            else {
                int etrg = sfply_IsEtrg(self);
                shouldStop = (((-etrg) | etrg) >> 31);
            }
            if (shouldStop) {
                SFTRN_CallTrtTrif(self, 7, 6, 0, 0);
                newState = 4;
            }
            break;
        }
        default: break;
        }
        break;
    }
    case 4: {
        int timeSec, timeUsec;
        int timeOk = 0;
        int timeSec2 = FIELD(int, self, 0xA6C);
        int timeUsec2 = FIELD(int, self, 0xA70);

        if (timeSec2 != -4) {
            SFTIM_GetTime(self, &timeSec, &timeUsec);
            if (timeSec >= 0) {
                timeOk = (UTY_CmpTime(timeSec, timeUsec, timeSec2, timeUsec2) == 0) ? 1 : 0;
            }
        }

        if (!timeOk) {
            int etrg = sfply_IsEtrg(self);
            if (!etrg) {
                int doStop = 0;
                if (FIELD(int, self, 0x54) == 4 && FIELD(int, self, 0x5C) != 1 && FIELD(int, self, 0x980) == 1) {
                    // check stagnant
                } else {
                    int stag = SFTIM_IsStagnant(self);
                    if ((((-stag) | stag) >> 31)) doStop = 1;
                }
                if (!doStop) {
                    int autoStop = sfply_IsPlayTimeAutoStop(self);
                    if (!autoStop && !FIELD(int, self, 0x70)) {
                        goto doPauseCheck;
                    }
                }
            }
        }

        // Stop path
        if (FIELD(int, self, 0x54) == 4) {
            int tr = SFTRN_CallTrtTrif(self, 7, 7, 0, 0);
            if (tr) { newState = FIELD(int, self, 0x54); break; }
        }
        FIELD(int, self, 0x54) = 1;
        FIELD(int, self, 0x58) = 1;
        FIELD(int, self, 0x58) = 6;
        // Timer save would go here
        newState = 0;
        break;

doPauseCheck:
        cs = NULL;
        SFLIB_LockCs(&cs);
        pauseResult = 0;
        if (!FIELD(int, self, 0x980)) {
            if (sfply_IsBpaOn(self)) {
                FIELD(int, self, 0x980) = 1;
                FIELD(int, self, 0x984)++;
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
            newState = (FIELD(int, self, 0x58) == 6) ? 6 : FIELD(int, self, 0x54);
        }
        break;
    }
    case 6: break;
    default: break;
    }

    FIELD(int, self, 0x54) = newState;
    // Timer add would go here
}

// ---------------------------------------------------------------------------
// sfply_StatPrep
// ---------------------------------------------------------------------------
int sfply_StatPrep(void* self, int canExec) {
    int origStatus = FIELD(int, self, 0x54);
    int origSub = FIELD(int, self, 0x58);
    int newState;

    if (!fn_803CC170(self)) return origStatus;

    if (FIELD(int, self, 0xA30) == 1) {
        if (SFBUF_GetWTot(self, 1) == 0 && SFBUF_GetRTot(self, 1) == 0)
            FIELD(int, self, 0xA30) = 0;
    }
    if (FIELD(int, self, 0xA34) == 1) {
        if (SFBUF_GetWTot(self, 2) == 0 && SFBUF_GetRTot(self, 2) == 0)
            FIELD(int, self, 0xA34) = 0;
    }

    SFSEE_FixAvPlay(self, FIELD(int, self, 0xA30), FIELD(int, self, 0xA34));

    if (!FIELD(int, self, 0xA34) && FIELD(int, self, 0xA58) == 2)
        SFSET_SetCond(self, 0xF, 1);
    if (!FIELD(int, self, 0xA30) && FIELD(int, self, 0xA58) == 1)
        SFSET_SetCond(self, 0xF, 2);
    if (FIELD(int, self, 0xA58) == 1) {
        if (FIELD(int, self, 0x107C))
            SFSET_SetCond(self, 0xF, 5);
        else
            SFSET_SetCond(self, 0xF, 1);
    }

    fn_803CC238(self);

    // Trace
    {
        void* ctx = lbl_eu_80606E34;
        if (ctx) {
            void* vtbl = *(void**)ctx;
            void (*trace)(void*, void*) = *(void(**)(void*, void*))((u8*)vtbl + 0x24);
            *(void**)(lbl_eu_80568C84 + 0x0C) = self;
            *(int**)(lbl_eu_80568C84 + 0x18) = (int*)((u8*)self + 0xA30);
            *(int**)(lbl_eu_80568C84 + 0x24) = (int*)((u8*)self + 0xA34);
            *(int**)(lbl_eu_80568C84 + 0x30) = (int*)((u8*)self + 0xA58);
            *(int**)(lbl_eu_80568C84 + 0x3C) = (int*)((u8*)self + 0xA80);
            trace(ctx, lbl_eu_80568C84 + 4);
        }
    }

    switch (origSub) {
    case 2: newState = 2; break;
    case 3: newState = 3; break;
    case 4: case 6: {
        int shouldStop = 0;
        if (!FIELD(int, self, 0xA54)) shouldStop = 1;
        else if (!FIELD(int, self, 0xA30)) shouldStop = 1;
        else if (FIELD(int, self, 0x1050)) shouldStop = 1;
        else if (FIELD(int, self, 0x106C) >= FIELD(int, self, 0xAD0)) shouldStop = 1;
        else {
            int etrg = sfply_IsEtrg(self);
            shouldStop = (((-etrg) | etrg) >> 31);
        }
        if (shouldStop) {
            SFTRN_CallTrtTrif(self, 7, 6, 0, 0);
            newState = 4;
        } else {
            newState = 3;
        }
        break;
    }
    default: newState = origStatus; break;
    }
    return newState;
}

// ---------------------------------------------------------------------------
// fn_803CC170
// ---------------------------------------------------------------------------
int fn_803CC170(void* self) {
    int ready5, ready6;
    if (!SFSET_GetCond(self, 5)) {
        ready5 = 1;
    } else {
        int p = SFTRN_GetPrepFlg(self, 6);
        int t = SFTRN_GetTermFlg(self, 6);
        ready5 = p | t;
    }
    if (!SFSET_GetCond(self, 6)) {
        ready6 = 1;
    } else {
        int p = SFTRN_GetPrepFlg(self, 7);
        int t = SFTRN_GetTermFlg(self, 7);
        ready6 = p | t;
    }
    if (!ready5) goto ret0;
    if (ready6) goto ret1;
ret0:
    return 0;
ret1:
    return 1;
}

// ---------------------------------------------------------------------------
// fn_803CC238
// ---------------------------------------------------------------------------
void fn_803CC238(void* self) {
    int avFlags = 0;
    int cond;
    if (FIELD(int, self, 0xA34) == 1) avFlags |= 1;
    if (FIELD(int, self, 0xA30) == 1) avFlags |= 2;

    if (avFlags == 1) goto case1;
    if (avFlags == 2) goto case2;
    if (avFlags == 3) goto case3;
    goto caseDefault;
case1:
    cond = 1;
    goto setCond;
case2:
    cond = 2;
    goto setCond;
case3:
    cond = SFSET_GetCond(self, 0x19);
    if (cond) goto setCond;
    if (UTY_IsTmrVoid()) goto set3;
    if (SFSET_GetCond(self, 0x48)) goto setCond;
set3:
    cond = 3;
    goto setCond;
caseDefault:
    cond = 3;
setCond:
    SFSET_SetCond(self, 0x19, cond);
}

// ---------------------------------------------------------------------------
// sfply_IsBpaOn
// ---------------------------------------------------------------------------
int sfply_IsBpaOn(void* self) {
    int flag;
    int i;

    if (!SFSET_GetCond(self, 0x43)) return 0;
    if (!SFSET_GetCond(self, 0xF)) return 0;
    if (FIELD(int, self, 0x5C)) return 0;
    if (FIELD(int, self, 0x54) != 4) return 0;

    /* any active transport/buffer disqualifies BPA-on */
    flag = 0;
    if (SFSET_GetCond(self, 5) && SFTRN_GetTermFlg(self, 6)) {
        flag = 1;
    } else if (SFSET_GetCond(self, 6) && SFTRN_GetTermFlg(self, 7)) {
        flag = 1;
    } else {
        for (i = 0; i < 8; i++) {
            if (SFBUF_GetTermFlg(self, i)) {
                flag = 1;
                break;
            }
        }
    }
    if (flag) return 0;

    if (SFSET_GetCond(self, 5) == 1 && !FIELD(int, self, 0x988)) return 0;
    if (SFSET_GetCond(self, 6) == 1 && SFBUF_RingGetDataSiz(self, 2) > 0) return 0;
    if (SFTRN_IsSetup(self, 1) && SFBUF_RingGetDataSiz(self, 0) > 0) return 0;

    if (SFSET_GetCond(self, 5) == 1) {
        /* video track fill-level check against per-mille thresholds */
        u8* trk = (u8*)self + FIELD(int, self, 0x2070) * 0x74;
        void* obj = FIELD(void*, trk, 0x13CC);
        void* vtbl = *(void**)obj;
        int (*getstat)(void*, int) = *(int (**)(void*, int))((u8*)vtbl + 0x24);
        int result = getstat(obj, 1);
        int thr = FIELD(int, trk, 0x13D4) / 100 * 80;
        int over;

        if (result >= thr || result >= SFSET_GetCond(self, 0x46)) {
            over = 1;
        } else {
            over = 0;
        }
        if (over) return 0;
    }

    {
        int sec, usec;
        int playSec, playFrac;
        int scaled;

        SFTIM_GetTime(self, &sec, &usec);
        playSec = FIELD(int, self, 0x1020);
        playFrac = FIELD(int, self, 0x1024);
        scaled = UTY_MulDiv(SFSET_GetCond(self, 0x44), playFrac, 1000000);
        playSec -= scaled;
        if (sec <= 0) return 0;
        if (playSec <= 0) return 0;
        return (UTY_CmpTime(sec, usec, playSec, playFrac) == 0) ? 1 : 0;
    }
}

// ---------------------------------------------------------------------------
// sfply_IsBpaOff
// ---------------------------------------------------------------------------
int sfply_IsBpaOff(void* self) {
    int i;
    int timeSec, timeUsec;
    int playStartSec, playStartFrac;
    int limitVal, scaled;

    /* any transport/buffer still running counts as BPA-active */
    if (SFSET_GetCond(self, 5) && SFTRN_GetTermFlg(self, 6)) return 1;
    if (SFSET_GetCond(self, 6) && SFTRN_GetTermFlg(self, 7)) return 1;
    for (i = 0; i < 8; i++) {
        if (SFBUF_GetTermFlg(self, i)) return 1;
    }

    if (SFSET_GetCond(self, 5) == 1) {
        u8* trackPtr = (u8*)self + FIELD(int, self, 0x2070) * 0x74;
        void* obj = FIELD(void*, trackPtr, 0x13CC);
        void* vtbl = *(void**)obj;
        int (*fn)(void*, int) = *(int (**)(void*, int))((u8*)vtbl + 0x24);
        int result = fn(obj, 1);
        /* field_0x13D4 is a per-mille threshold scaled by 80/1000 */
        if (result >= FIELD(int, trackPtr, 0x13D4) * 80 / 1000 ||
            result >= SFSET_GetCond(self, 0x46)) return 1;
    }

    if (SFSET_GetCond(self, 6) == 1) {
        u8* trackPtr = (u8*)self + FIELD(int, self, 0x20B4) * 0x74;
        void* obj = FIELD(void*, trackPtr, 0x13CC);
        void* vtbl = *(void**)obj;
        int (*fn)(void*, int) = *(int (**)(void*, int))((u8*)vtbl + 0x24);
        int result = fn(obj, 1);
        if (result >= FIELD(int, trackPtr, 0x13D4) * 80 / 1000) return 1;
    }

    SFTIM_GetTime(self, &timeSec, &timeUsec);
    playStartSec = FIELD(int, self, 0x1020);
    playStartFrac = FIELD(int, self, 0x1024);
    limitVal = SFSET_GetCond(self, 0x45);
    scaled = UTY_MulDiv(limitVal, playStartFrac, 10000000);
    if (UTY_CmpTime(timeSec, timeUsec, playStartSec - scaled, playStartFrac))
        return 1;
    return 0;
}

// ---------------------------------------------------------------------------
// sfply_IsEtrg
// ---------------------------------------------------------------------------
int sfply_IsEtrg(void* self) {
    int vf, af, tf, c19;
    int result;
    if (!FIELD(int, self, 0xA34) && !FIELD(int, self, 0xA30)) return 1;
    result = 0;
    vf = SFTRN_GetTermFlg(self, 6);
    af = SFTRN_GetTermFlg(self, 7);
    tf = SFTRN_GetTermFlg(self, 1);
    c19 = SFSET_GetCond(self, 0x19);
    if (c19 == 1) goto case1;
    if (c19 == 2) goto case2;
    if (c19 == 3) goto case3;
    if (c19 == 0) goto case0;
    goto after;
case1:
    result = af;
    goto after;
case2:
    result = vf;
    goto after;
case3:
    result = af | vf;
    goto after;
case0:
    result = af & vf;
after:
    if (SFTRN_IsSetup(self, 1)) result &= tf;
    if (result) goto ret1;
    return 0;
ret1:
    return 1;
}

// ---------------------------------------------------------------------------
// criware_803C9FC0
// ---------------------------------------------------------------------------
int criware_803C9FC0(void* self) {
    int result = 0;
    if (SFTRN_IsSetup(self, 1)) {
        int st = FIELD(int, self, 0x54);
        if ((unsigned int)(st - 2) <= 2) goto callBlock;
        if (st == 6) goto set1;
        goto set0;
callBlock:
        if (SFTRN_GetTermFlg(self, 1)) result = 1;
        goto end;
set1:
        result = 1;
        goto end;
set0:
        result = 0;
end:;
    }
    return result;
}

// ---------------------------------------------------------------------------
// sfply_IsPlayTimeAutoStop
// ---------------------------------------------------------------------------
int sfply_IsPlayTimeAutoStop(void* self) {
    int timeSec, timeUsec;
    int limitTime;
    if (FIELD(int, self, 0x54) != 4) goto ret0;
    if (FIELD(int, self, 0x5C) == 1) goto ret0;
    if (FIELD(int, self, 0x980) != 1) goto cont;
ret0:
    return 0;
cont:
    if (SFTIM_GetTimeSub(self, &timeSec, &timeUsec)) return 0;
    if (timeSec < 0) return 0;
    criware_803CA124(self, timeUsec);
    SFD_GetLimitTime(self, &limitTime);
    if (limitTime == -1) return 0;
    return (UTY_CmpTime(limitTime, 1000, timeSec, timeUsec) != 0);
}

// ---------------------------------------------------------------------------
// criware_803CA124
// ---------------------------------------------------------------------------
void criware_803CA124(void* self, int fpsArg) {
    int limit;
    int condF;

    if (SFSET_GetCond(self, 0x36) != -1) return;
    if (fpsArg == 1) return;

    if (fpsArg >= 1000) {
        limit = (1 - (0x7FFFFFFF / fpsArg)) * -1000;
    } else {
        limit = 0x7FFFFFFF - 1000;
    }

    if (FIELD(int, self, 0xDD4)) {
        int limit2;
        fpsArg = FIELD(int, self, 0xDFC);
        if (fpsArg != 1) {
            if (fpsArg >= 1000) limit2 = (1 - (0x7FFFFFFF / fpsArg)) * -1000;
            else limit2 = 0x7FFFFFFF - 1000;
            if (limit > limit2) limit = limit2;
        }
    }

    condF = SFSET_GetCond(self, 0xF);
    if (condF == 2) {
        int gf = *(int*)(lbl_eu_80606E38 + 0x1A4);
        int gl;
        if (gf >= 1000) gl = (1 - (0x7FFFFFFF / gf)) * -1000;
        else gl = 0x7FFFFFFF - 1000;
        if (limit > gl) limit = gl;
    }
    SFSET_SetCond(self, 0x36, limit);
}

// ---------------------------------------------------------------------------
// SFD_Create
// ---------------------------------------------------------------------------
int SFD_Create(void* config, void* extra) {
    char buf[0x200];
    void* ctx;
    void* vtbl;
    void (*trace)(void*, void*);
    int err;
    int slot;
    int hn;

    ctx = lbl_eu_80606E34;
    if (ctx) {
        sprintf(buf, lbl_eu_8051CBA8,
                ((u32*)config)[1], ((u32*)config)[2], ((u32*)config)[3],
                ((u32*)config)[4], ((u32*)config)[5], ((u32*)config)[6],
                ((u32*)config)[7], ((u32*)config)[8], ((u32*)config)[9],
                ((u32*)config)[0xB], ((u32*)config)[0xC],
                ((u32*)config)[0xE], ((u32*)config)[0xF],
                ((u32*)config)[0x10], ((u32*)config)[0x11],
                ((u32*)config)[0x12]);
        *(void**)(lbl_eu_80567FAC + 0x0C) = buf;
        vtbl = *(void**)ctx;
        trace = *(void(**)(void*, void*))((u8*)vtbl + 0x24);
        trace(ctx, lbl_eu_80567FAC + 4);
    }

    /* validate config: stream type and minimum work size */
    err = 0;
    if (FIELD(int, config, 4) == 0) {
        err = SFLIB_SetErr(0, 0xff000204);
    } else if (FIELD(u32, config, 0x48) < 0x39D0) {
        err = SFLIB_SetErr(0, 0xff000205);
    }

    hn = 0;
    if (err == 0) {
        /* find a free handle-table slot (unrolled 8-entry scan) */
        u32* hnTable = (u32*)(lbl_eu_80606E38 + 0x1FC);
        if (hnTable[0] == 0) slot = 0;
        else if (hnTable[1] == 0) slot = 1;
        else if (hnTable[2] == 0) slot = 2;
        else if (hnTable[3] == 0) slot = 3;
        else if (hnTable[4] == 0) slot = 4;
        else if (hnTable[5] == 0) slot = 5;
        else if (hnTable[6] == 0) slot = 6;
        else if (hnTable[7] == 0) slot = 7;
        else slot = -1;
        if (slot == -1) {
            SFLIB_SetErr(0, 0xff000206);
            hn = 0;
        } else {
            hn = (int)sfply_InitHn(config, extra);
            hnTable[slot] = (u32)hn;
        }
    }

    ctx = lbl_eu_80606E34;
    if (ctx) {
        *(void**)(lbl_eu_80567FAC + 0x74) = (void*)hn;
        vtbl = *(void**)ctx;
        trace = *(void(**)(void*, void*))((u8*)vtbl + 0x24);
        trace(ctx, lbl_eu_80567FAC + 0x6C);
    }
    return hn;
}

// ---------------------------------------------------------------------------
// sfply_InitHn
// ---------------------------------------------------------------------------
void* sfply_InitHn(void* config, void* extra) {
    u8* work = FIELD(u8*, config, 0x44);
    u32 workSize = FIELD(u32, config, 0x48);
    int i;

    if (!work) return NULL;
    if (workSize <= 0 || (u32)workSize > 0x73A0) return NULL;
    if (lbl_eu_80619BA4 != 0 && lbl_eu_80619BA4 != workSize) return NULL;
    lbl_eu_80619BA4 = workSize;

    UTY_MemsetDword(work, 0, workSize >> 2);

    {
        u8* hn = (u8*)(((u32)work + 0x1F) & ~0x1F);
        u32* src;
        u32* dst;
        u8* tmr;

        FIELD(int, hn, 0x58) = 0;
        FIELD(int, hn, 0x54) = 0;

        /* align the stream-position field in the config, then clone the
           config header (0x50 bytes) into the handle */
        FIELD(u32, config, 4) = (FIELD(u32, config, 4) + 0x1F) & ~0x1F;
        src = (u32*)((u8*)config - 4);
        dst = (u32*)((u8*)hn - 4);
        for (i = 0; i < 10; i++) {
            dst[1] = src[1];
            dst[2] = src[2];
            src += 2;
            dst += 2;
        }

        FIELD(int, hn, 0x50) = 1;
        FIELD(int, hn, 0x5C) = 0;
        FIELD(int, hn, 0x60) = 0;
        FIELD(int, hn, 0x64) = 0;
        FIELD(int, hn, 0x68) = 0;
        FIELD(int, hn, 0x6C) = 0;
        FIELD(int, hn, 0x70) = 0;

        SFHDS_InitFhd(hn + 0x88);

        UTY_MemsetDword(hn + 0x91C, 0, 0x10);
        FIELD(int, hn, 0x91C) = 0;
        FIELD(int, hn, 0x920) = 0;
        FIELD(int, hn, 0x924) = 0;
        FIELD(int, hn, 0x928) = 0;
        FIELD(int, hn, 0x92C) = 0;
        FIELD(int, hn, 0x930) = 0;
        FIELD(int, hn, 0x934) = 0;
        FIELD(int, hn, 0x938) = 1;
        FIELD(int, hn, 0x93C) = 0;
        FIELD(int, hn, 0x940) = -1;
        FIELD(int, hn, 0x944) = -1;
        FIELD(int, hn, 0x948) = -1;

        sfply_InitPlyInf(hn + 0x960);

        UTY_MemsetDword(hn + 0x26A0, 0, 0x38);
        tmr = hn + 0x26A0;
        for (i = 0; i < 5; i++) {
            SFTMR_InitTsum(tmr);
            tmr += 0x20;
        }
        SFTMR_InitTsum(hn + 0x2740);

        FIELD(int, hn, 0x2764) = 0;
        FIELD(int, hn, 0x2760) = 0;
        FIELD(int, hn, 0x276C) = 0;
        FIELD(int, hn, 0x2768) = 0;
        FIELD(int, hn, 0x2774) = 0;
        FIELD(int, hn, 0x2770) = 0;
        FIELD(int, hn, 0x2778) = 0;
        FIELD(float, hn, 0x277C) = lbl_eu_8051CBA4;

        fn_803C34DC(hn + 0xA08);
        MEM_Copy(hn + 0xA1C, lbl_eu_80606E38, 0x190);
        MEM_Copy(hn + 0xBAC, lbl_eu_80606E38, 0x190);
        memset(hn + 0xD3C, 0, 0x5C);
        FIELD(int, hn, 0xD8C) = 0x7FFFFFFF;
        SFTIM_InitHn(hn, hn + 0xD98);

        if (SFBUF_InitHn(hn, hn + 0x13B8, config)) return NULL;

        SFTRN_InitHn(hn, hn + 0x1FD8, config, extra);
        SFSEEKI_InitHn(hn + 0x2680);
        SFSEE_InitHn(hn + 0x2670);
        if (SFTRN_CallTrSetup(hn, 3)) return NULL;

        FIELD(int, hn, 0x58) = 1;
        FIELD(int, hn, 0x54) = 1;
        return hn;
    }
}

// ---------------------------------------------------------------------------
// sfply_InitPlyInf
// ---------------------------------------------------------------------------
void sfply_InitPlyInf(void* self) {
    u32 zero = 0;
    UTY_MemsetDword(self, 0, 0x2A);
    FIELD(u32, self, 0x00) = zero;
    FIELD(u32, self, 0x04) = zero;
    FIELD(u32, self, 0x08) = zero;
    FIELD(u32, self, 0x0C) = zero;
    FIELD(u32, self, 0x10) = zero;
    FIELD(u32, self, 0x18) = zero;
    FIELD(u32, self, 0x1C) = zero;
    FIELD(u32, self, 0x20) = zero;
    FIELD(u32, self, 0x24) = zero;
    FIELD(u32, self, 0x28) = zero;
    FIELD(u32, self, 0x2C) = zero;
    FIELD(u32, self, 0x30) = zero;
    FIELD(s64, self, 0x38) = 0;
    FIELD(s64, self, 0x40) = 0;
    FIELD(s64, self, 0x48) = 0;
    FIELD(s64, self, 0x50) = 0;
    FIELD(s64, self, 0x58) = 0;
    FIELD(s64, self, 0x60) = 0;
    FIELD(s64, self, 0x68) = 0;
    FIELD(s64, self, 0x70) = 0;
    FIELD(s64, self, 0x78) = 0;
    FIELD(s64, self, 0x80) = 0;
    FIELD(s64, self, 0x88) = 0;
    FIELD(s64, self, 0x90) = 0;
}

// ---------------------------------------------------------------------------
// SFPLY_AddDecPic
// ---------------------------------------------------------------------------
void SFPLY_AddDecPic(void* self, int delta, void* param) {
    void (*cb)(void*, void*, u32*) = *(void (**)(void*, void*, u32*))((u8*)self + 0xD6C);
    volatile u32* pCount = (volatile u32*)((u8*)self + 0x960);
    u32 count = *pCount + delta;
    *pCount = count;
    if (cb != NULL) {
        cb(FIELD(void*, self, 0xD70), param, (u32*)((u8*)self + 0x960));
    }
}

// ---------------------------------------------------------------------------
// SFPLY_AddSkipPic
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
// SFD_Destroy
// ---------------------------------------------------------------------------
int SFD_Destroy(void* self) {
    SfdGlob* g;
    int result;
    void* cs;
    void* ctx;
    void* vtbl;
    void (*trace)(void*, void*);
    if (SFLIB_CheckHn(self)) {
        return SFLIB_SetErr(0, 0xff000131);
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
    } else {
        int tr;
        if (FIELD(int, self, 0x54) == 4) {
            tr = SFTRN_CallTrtTrif(self, 7, 7, 0, 0);
            if (tr == 0) {
                goto parkFields;
            }
            goto trCheck;
        }
        goto parkFields;
parkFields:
        /* park in state 1, then run the full reset inline */
        FIELD(int, self, 0x54) = 1;
        tr = 0;
        FIELD(int, self, 0x58) = 1;
trCheck:
        if (tr == 0) {
            g = SFD_GLOB;
            FIELD(int, self, 0x58) = 0;
            FIELD(int, self, 0x54) = 0;
            g->resetFlg = 1;
            sfply_ResetHn(self);
            g->resetFlg = 0;
        }
    }
    SFHDS_FinishFhd((u8*)self + 0x88);
    SFBUF_DestroySj(self);
    cs = NULL;
    SFLIB_LockCs(&cs);
    FIELD(int, self, 0x54) = 0;
    FIELD(int, self, 0x58) = 0;
    SFLIB_UnlockCs(&cs);
    result = SFTRN_CallTrSetup(self, 4);
    cs = NULL;
    SFLIB_LockCs(&cs);
    {
        /* clear any handle-table slot still pointing at us */
        int i;
        for (i = 0; i < 8; i++) {
            if (SFD_GLOB->hnSlots[i] == self) SFD_GLOB->hnSlots[i] = NULL;
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
// SFD_Start
// ---------------------------------------------------------------------------
int SFD_Start(void* self) {
    int result;
    void* ctx;
    void* vtbl;
    void (*trace)(void*, void*);

    if (SFLIB_CheckHn(self)) {
        return SFLIB_SetErr(0, 0xff000132);
    }
    ctx = lbl_eu_80606E34;
    if (ctx) {
        *(void**)(lbl_eu_80568154 + 0x0C) = self;
        vtbl = *(void**)ctx;
        trace = *(void(**)(void*, void*))((u8*)vtbl + 0x24);
        trace(ctx, lbl_eu_80568154 + 4);
    }

    if (SFSET_GetCond(self, 0x2F) == 1) {
        result = SFPL2_Standby(self);
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
// fn_803CD484
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
// SFD_Stop
// ---------------------------------------------------------------------------
int SFD_Stop(void* self) {
    SfdGlob* g;
    int result;
    void* ctx;
    void* vtbl;
    void (*trace)(void*, void*);

    if (SFLIB_CheckHn(self)) {
        return SFLIB_SetErr(0, 0xff000133);
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
    } else {
        int tr;
        if (FIELD(int, self, 0x54) == 4) {
            tr = SFTRN_CallTrtTrif(self, 7, 7, 0, 0);
            if (tr == 0) {
                goto stopFields;
            }
            goto stopCheck;
        }
        goto stopFields;
stopFields:
        FIELD(int, self, 0x54) = 1;
        tr = 0;
        FIELD(int, self, 0x58) = 1;
stopCheck:
        if (tr != 0) {
            result = tr;
        } else {
            /* run the protected reset */
            g = SFD_GLOB;
            result = 0;
            FIELD(int, self, 0x58) = 0;
            FIELD(int, self, 0x54) = 0;
            g->resetFlg = 1;
            {
                int ret = sfply_ResetHn(self);
                g->resetFlg = 0;
                if (ret != 0) result = ret;
            }
        }
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
// SFPLY_GetResetFlg
// ---------------------------------------------------------------------------
int SFPLY_GetResetFlg(void) {
    return *(int*)(lbl_eu_80606E38 + 0x1F0);
}

// ---------------------------------------------------------------------------
// sfply_ResetHn
// ---------------------------------------------------------------------------
int sfply_ResetHn(void* self) {
    // Simplified reset - the full implementation is very complex
    // For now, just reset transport and reinitialize
    void* hnTable;
    void* cs;
    int i;

    SFHDS_FinishFhd((u8*)self + 0x88);
    SFBUF_DestroySj(self);

    cs = NULL;
    SFLIB_LockCs(&cs);
    FIELD(int, self, 0x54) = 0;
    FIELD(int, self, 0x58) = 0;
    SFLIB_UnlockCs(&cs);

    SFTRN_CallTrSetup(self, 4);
    return 0;
}

// ---------------------------------------------------------------------------
// SFD_RequestStop
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
// SFD_TermSupply
// ---------------------------------------------------------------------------
int SFD_TermSupply(void* self) {
    void* ctx;
    void* vtbl;
    void (*trace)(void*, void*);
    int bufId;
    int result;

    ctx = lbl_eu_80606E34;
    if (ctx) {
        *(void**)(lbl_eu_805683D0 + 0x0C) = self;
        vtbl = *(void**)ctx;
        trace = *(void(**)(void*, void*))((u8*)vtbl + 0x24);
        trace(ctx, lbl_eu_805683D0 + 4);
    }

    if (SFLIB_CheckHn(self)) {
        result = SFLIB_SetErr(0, 0xff00013D);
        goto out;
    }
    bufId = FIELD(int, self, 0x1FEC);
    if (SFBUF_GetTermFlg(self, bufId) == 1) {
        result = 0;
        goto out;
    }
    SFBUF_SetTermFlg(self, bufId, 1);
    FIELD(int, self, 0x50) = 1;
    result = 0;
out:
    ctx = lbl_eu_80606E34;
    if (ctx) {
        vtbl = *(void**)ctx;
        trace = *(void(**)(void*, void*))((u8*)vtbl + 0x24);
        trace(ctx, lbl_eu_805683D0 + 0x6C);
    }
    return result;
}

// ---------------------------------------------------------------------------
// SFD_GetFrm
// ---------------------------------------------------------------------------
int SFD_GetFrm(void* self, void** outFrm) {
    void* ctx;
    void* vtbl;
    void (*trace)(void*, void*);
    int result = 0;

    *outFrm = NULL;
    if (SFLIB_CheckHn(self)) {
        return SFLIB_SetErr(0, 0xff000136);
    }

    {
        int frmState = FIELD(int, self, 0x68);
        if (frmState == 0) {
            FIELD(int, self, 0x68) = 1;
            result = 0;
        } else if (frmState == 1) {
            result = 0;
        } else {
            result = SFLIB_SetErr(self, 0xff000207);
        }
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
        int gc = FIELD(int, self, 0x978);
        if (gc == FIELD(int, self, 0x97C)) {
            if (gc == 0) {
                FIELD(u64, self, 0x2760) = SFTMR_GetTmr(self);
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
            *(void**)(lbl_eu_805684A4 + 0x74) = NULL;
            *(void**)(lbl_eu_805684A4 + 0x80) = NULL;
            *(void**)(lbl_eu_805684A4 + 0x8C) = NULL;
            vtbl = *(void**)ctx;
            trace = *(void(**)(void*, void*))((u8*)vtbl + 0x24);
            trace(ctx, lbl_eu_805684A4 + 0x6C);
        }
    }

    if (lbl_eu_80619BA0 != NULL) {
        lbl_eu_80619BA0(self, *outFrm);
    }
    return result;
}

// ---------------------------------------------------------------------------
// SFD_RelFrm
// ---------------------------------------------------------------------------
int SFD_RelFrm(void* self, void* frm) {
    int result;
    void* ctx;
    void* vtbl;
    void (*trace)(void*, void*);

    if (SFLIB_CheckHn(self)) {
        return SFLIB_SetErr(0, 0xff000137);
    }

    {
        int frmState = FIELD(int, self, 0x68);
        if (frmState == 0) {
            FIELD(int, self, 0x68) = 1;
            result = 0;
        } else if (frmState == 1) {
            result = 0;
        } else {
            result = SFLIB_SetErr(self, 0xff000207);
        }
    }
    if (result != 0) return result;

    ctx = lbl_eu_80606E34;
    if (ctx) {
        *(void**)(lbl_eu_80568578 + 0x0C) = self;
        *(void**)(lbl_eu_80568578 + 0x18) = frm;
        vtbl = *(void**)ctx;
        trace = *(void(**)(void*, void*))((u8*)vtbl + 0x24);
        trace(ctx, lbl_eu_80568578 + 4);
    }

    {
        int rc = FIELD(int, self, 0x97C);
        if (rc < FIELD(int, self, 0x978)) {
            FIELD(int, self, 0x97C) = rc + 1;
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
// SFD_SetSupplySj
// ---------------------------------------------------------------------------
void SFD_SetSupplySj(void* self, void* sj) {
    if (SFLIB_CheckHn(self)) {
        SFLIB_SetErr(0, 0xff000139);
        return;
    }
    SFBUF_SetSupplySj(self, sj);
}
