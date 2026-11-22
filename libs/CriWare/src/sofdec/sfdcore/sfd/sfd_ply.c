// CriWare Sofdec player core - sfd_ply TU
// High-level C reconstruction for MWCC matching.

#include <types.h>
#include "libs/CriWare/src/sofdec/sfdcore/sfd/sfd_types.h"
#include <string.h>
/* JP map: SFPLY_cond_dfl */
const s32 SFPLY_cond_dfl[101] = {1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, -3, 1, -4, 1, 0, 4, 4096, 0, 1, 60, 1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 1, 0, -30000, -14000, 8000, 59940, 4000, 4000, 41, 0, 0, 0, 0, 5, 0, 5, -1, 0, 0, 0, 2147483647, 0, 0, 1, 10, 16683, 200000, 0, 0, 1, 66732, 133464, 512000, 1, 1, 0, 0, -1, -1, -1, 1, 1, 1, -1, -1, 0, 0, 0, 4, 0, 1515870810, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};



typedef struct SfdBlk5C {
    u32 w[0x17];
} SfdBlk5C;


void sfply_ExecOne(void* self);
int sfply_StatPrep(SfdHandle* self, int canExec);
int fn_803CC170(void* self);
void fn_803CC238(void* self);
int sfply_IsBpaOn(void* self);
int sfply_IsBpaOff(void* self);
int sfply_IsEtrg(void* self);
int criware_803C9FC0(void* self);
int sfply_IsPlayTimeAutoStop(void* self);
void criware_803CA124(void* self, int fpsArg);
void* sfply_InitHn(SfdCreateCfg* config, void* extra);
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
extern void SFTMR_AddTsum(void* area, u32 nowLo, s64 elapsed);
extern void SFTMR_InitTsum(void* area);
extern int SFBUF_GetTermFlg(void* hn, int id);
extern int SFBUF_SetTermFlg(void* hn, int id, int val);
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
extern void SFSEE_InitHn(SfdSee* area);
extern int SFSET_GetCond(void* hn, int condId);
extern void SFSET_SetCond(void* hn, int condId, int val);
extern void SFHDS_InitFhd(SfdFhd* area);
extern void SFHDS_FinishFhd(SfdFhd* area);
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
extern void (*gpSfdFrameCallback)(void*, void*);
extern u32 sSfdWorkMemSize;
extern u32 gpSfdEventCallback;
extern void* gpSfdCoreTraceCb;
extern u8 sSfdGlob[];
extern SfdTrcRec sTraceSfdExecOne;
extern SfdTrcRec sTraceSfdCreate;
extern SfdTrcRec sTraceSfdDestroy;
extern SfdTrcRec sTraceSfdStart;
extern SfdTrcRec sTraceSfdStop;
extern SfdTrcRec sTraceSfdRequestStop;
extern SfdTrcRec sTraceSfdTermSupply;
extern SfdTrcRec sTraceSfdGetFrm;
extern SfdTrcRec sTraceSfdRelFrm;
extern SfdTrcRecIn sTraceAdjustPrepEnd;

// Field access helper
#define FIELD(type, base, off) (*((type*)((u8*)(base) + (off))))

// SofDec global work area (sSfdGlob): canonical process context from sfd_types.h
#define SFD_GLOB ((SfdSys*)sSfdGlob)

// Trace helper - calls ctx->vtable[9](ctx, traceblock + offset)
static void traceCall(void* traceBlock, int offset) {
    void* ctx = gpSfdCoreTraceCb;
    if (ctx) {
        ((SfdTraceCtx*)ctx)->vtbl->trace(ctx, (u8*)traceBlock + offset);
    }
}

// ---------------------------------------------------------------------------
// SFPLY_Init
// ---------------------------------------------------------------------------
void SFPLY_Init(void) {
    gpSfdFrameCallback = NULL;
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

    if (SFLIB_CheckHn(self)) {
        return SFLIB_SetErr(0, 0xff000138);
    } else {
        ctx = gpSfdCoreTraceCb;
        if (ctx) {
            sTraceSfdExecOne.entry.args[0].value = self;
            ((SfdTraceCtx*)ctx)->vtbl->trace(ctx, &sTraceSfdExecOne.entry);
        }

        sfply_ExecOne(self);

        ctx = gpSfdCoreTraceCb;
        if (ctx) {
            ((SfdTraceCtx*)ctx)->vtbl->trace(ctx, &sTraceSfdExecOne.exit);
        }
        return 0;
    }
}

// ---------------------------------------------------------------------------
// sfply_ExecOne - main player state machine
// ---------------------------------------------------------------------------
void sfply_ExecOne(void* self) {
    s32 status;
    s32 result;
    s32 canExec;
    u64 startTmr;
    void* cs;
    int timeSec, timeUsec;

    status = FIELD(s32, self, 0x54);
    if ((u32)(status - 1) > 3) return;
    if (FIELD(s32, self, 0x50) == 0) return;

    FIELD(s32, self, 0x50) = 0;
    if (!fn_803C1CAC(self)) return;

    {
        s32 term = SFBUF_GetTermFlg(self, FIELD(s32, self, 0x1FEC));
        canExec = (term != 0);
    }

    if (FIELD(s32, self, 0x64) == 1) return;

    startTmr = SFTMR_GetTmr(self);

    // Sub-state processing
    if ((u32)(status - 2) <= 2) {
        SFTRN_CallTrSetup(self, 2);
        SFSEE_ExecServer(self);
    }

    switch (FIELD(s32, self, 0x54)) {
    case 1: {
        s32 ss = FIELD(s32, self, 0x58);
        result = FIELD(s32, self, 0x54);
        if ((u32)(ss - 2) <= 2 || ss == 6) {
            result = 2;
        }
        break;
    }
    case 2:
        result = sfply_StatPrep(self, canExec);
        break;
    case 3: {
        s32 nextSub = FIELD(s32, self, 0x58);
        result = FIELD(s32, self, 0x54);
        switch (nextSub) {
        case 2: result = 2; break;
        case 3: break;
        case 4: case 6: {
            /* any stall condition or user event forces a transport teardown */
            s32 shouldStop;
            if (!FIELD(s32, self, 0xA54)) shouldStop = 1;
            else if (!FIELD(s32, self, 0xA30)) shouldStop = 1;
            else if (FIELD(s32, self, 0x1050)) shouldStop = 1;
            else if (FIELD(s32, self, 0x106C) >= FIELD(s32, self, 0xAD0)) shouldStop = 1;
            else {
                s32 etrg = sfply_IsEtrg(self);
                shouldStop = (etrg != 0);
            }
            if (shouldStop) {
                SFTRN_CallTrtTrif(self, 7, 6, 0, 0);
                result = 4;
            }
            break;
        }
        default: break;
        }
        break;
    }
    case 4: {
        /* playback monitor: enforce the seek deadline, event triggers,
           stagnation and auto-stop; otherwise service pause requests */
        status = FIELD(s32, self, 0xA6C);
        canExec = FIELD(s32, self, 0xA70);
        if (status != -4) {
            SFTIM_GetTime(self, &timeSec, &timeUsec);
            if (timeSec >= 0 && UTY_CmpTime(timeSec, timeUsec, status, canExec) == 0) {
                goto stop_path;
            }
        }
        if (sfply_IsEtrg(self)) {
            goto stop_path;
        }
        {
            s32 stag;
            if (FIELD(s32, self, 0x54) == 4 && FIELD(s32, self, 0x5C) != 1 &&
                FIELD(s32, self, 0x980) != 1) {
                s32 s = SFTIM_IsStagnant(self);
                stag = (s != 0);
            } else {
                stag = 0;
            }
            if (stag || sfply_IsPlayTimeAutoStop(self) || FIELD(s32, self, 0x70) != 0) {
                goto stop_path;
            }
        }
        goto do_pause;

    stop_path:
        {
            s32 tr = 1;
            if (FIELD(s32, self, 0x54) == 4) {
                tr = SFTRN_CallTrtTrif(self, 7, 7, 0, 0);
                if (tr == 0) {
                    FIELD(s32, self, 0x54) = 1;
                    FIELD(s32, self, 0x58) = 1;
                }
            } else {
                FIELD(s32, self, 0x54) = 1;
                FIELD(s32, self, 0x58) = 1;
                tr = 0;
            }
            if (tr == 0) {
                /* park in sub-state 6 and refresh the frame-rate estimate */
                u64 unit;
                s64 diff;
                FIELD(s32, self, 0x58) = 6;
                FIELD(u64, self, 0x2768) = SFTMR_GetTmr(self);
                unit = SFTMR_GetTmrUnit(self);
                diff = (s64)(FIELD(u64, self, 0x2768) - FIELD(u64, self, 0x2760));
                FIELD(u64, self, 0x2770) = unit;
                FIELD(s32, self, 0x2778) = FIELD(s32, self, 0x978);
                if (diff != 0) {
                    FIELD(f32, self, 0x277C) =
                        (f32)((s64)FIELD(u64, self, 0x2770) * FIELD(s32, self, 0x2778)) /
                        (f32)diff;
                }
                tr = 0;
            }
            result = tr;
        }
        break;

    do_pause:
        /* pause servicing under the player critical section */
        canExec = 0;
        SFLIB_LockCs(&cs);
        if (FIELD(s32, self, 0x980) == 0) {
            if (sfply_IsBpaOn(self)) {
                FIELD(s32, self, 0x980) = 1;
                FIELD(s32, self, 0x984) = FIELD(s32, self, 0x984) + 1;
                canExec = SFPL2_Pause(self, 1);
            }
        } else {
            if (sfply_IsBpaOff(self)) {
                FIELD(s32, self, 0x980) = 0;
                canExec = SFPL2_Pause(self, 0);
            }
        }
        SFLIB_UnlockCs(&cs);
        if (canExec != 0) {
            result = FIELD(s32, self, 0x54);
        } else {
            result = (FIELD(s32, self, 0x58) == 6) ? 6 : FIELD(s32, self, 0x54);
        }
        break;
    }
    case 6:
    default:
        result = FIELD(s32, self, 0x54);
        break;
    }

    FIELD(s32, self, 0x54) = result;
    {
        u64 now = SFTMR_GetTmr(self);
        SFTMR_AddTsum(((SfdHandle*)self)->field_0x2740, (u32)now, (s64)(now - startTmr));
    }
}

// ---------------------------------------------------------------------------
// sfply_StatPrep
// ---------------------------------------------------------------------------
int sfply_StatPrep(SfdHandle* self, int canExec) {
    s32 origStatus = self->errorState; /* 0x54 */
    s32 origSub = self->substatus;     /* 0x58 */
    s32 newState;

    if (!fn_803CC170(self)) return origStatus;

    /* clear a pending pause request once its buffer queue has fully drained */
    if ((s32)self->cond[5] == 1) {
        if (SFBUF_GetWTot(self, 1) == 0 && SFBUF_GetRTot(self, 1) == 0)
            self->cond[5] = 0;
    }
    if ((s32)self->cond[6] == 1) {
        if (SFBUF_GetWTot(self, 2) == 0 && SFBUF_GetRTot(self, 2) == 0)
            self->cond[6] = 0;
    }

    SFSEE_FixAvPlay(self, (s32)self->cond[5], (s32)self->cond[6]);

    if (!(s32)self->cond[6] && (s32)self->cond[15] == 2)
        SFSET_SetCond(self, 0xF, 1);
    if (!(s32)self->cond[5] && (s32)self->cond[15] == 1)
        SFSET_SetCond(self, 0xF, 2);
    if ((s32)self->cond[15] == 1) {
        if (self->tim.extClockFn)
            SFSET_SetCond(self, 0xF, 5);
        else
            SFSET_SetCond(self, 0xF, 1);
    }

    fn_803CC238(self);

    /* Trace: stash handle + pause/mode field pointers into the static record,
       then invoke the debug context's vtable entry */
 {
        void* ctx = gpSfdCoreTraceCb;
        if (ctx) {
            sTraceAdjustPrepEnd.entry.args[0].value = self;
            sTraceAdjustPrepEnd.entry.args[1].value = &self->cond[5];
            sTraceAdjustPrepEnd.entry.args[2].value = &self->cond[6];
            sTraceAdjustPrepEnd.entry.args[3].value = &self->cond[15];
            sTraceAdjustPrepEnd.entry.args[4].value = &self->cond[0x19];
            {
                ((SfdTraceCtx*)ctx)->vtbl->trace(ctx, &sTraceAdjustPrepEnd.entry);
            }
        }
    }

    switch (origSub) {
    case 2: newState = 2; break;
    case 3: newState = 3; break;
    case 4:
    case 6: {
        /* decide whether playback should stop (any stall condition or user event) */
        s32 shouldStop;
        if (!(s32)self->cond[14]) shouldStop = 1;
        else if (!(s32)self->cond[5]) shouldStop = 1;
        else if (self->tim.field_0x2b8) shouldStop = 1;
        else if (self->tim.field_0x2d4 >= (s32)self->cond[45]) shouldStop = 1;
        else {
            s32 etrg = sfply_IsEtrg(self);
            shouldStop = (etrg != 0);
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
    int sec, usec;
    int playSec;
    int playFrac;

    if (!SFSET_GetCond(self, 0x43)) return 0;
    if (!SFSET_GetCond(self, 0xF)) return 0;
    if (FIELD(int, self, 0x5C)) return 0;
    if (FIELD(int, self, 0x54) != 4) return 0;

    /* any active transport/buffer still terminating disqualifies BPA-on */
    if (SFSET_GetCond(self, 5) && SFTRN_GetTermFlg(self, 6)) {
        flag = 1;
    } else if (SFSET_GetCond(self, 6) && SFTRN_GetTermFlg(self, 7)) {
        flag = 1;
    } else {
        for (i = 0; i < 8; i++) {
            if (SFBUF_GetTermFlg(self, i)) {
                flag = 1;
                goto flagDone;
            }
        }
        flag = 0;
flagDone:;
    }
    if (flag) return 0;

    if (SFSET_GetCond(self, 5) == 1 && FIELD(int, self, 0x988) == 0) return 0;
    if (SFSET_GetCond(self, 6) == 1 && SFBUF_RingGetDataSiz(self, 2) > 0) return 0;
    if (SFTRN_IsSetup(self, 1) && SFBUF_RingGetDataSiz(self, 0) > 0) return 0;

    if (SFSET_GetCond(self, 5) == 1) {
        /* video track fill level: obj->vtable[9](obj, 1) vs field*80/100 and
           the configured cap; over-full means BPA cannot engage */
        u8* trk = (u8*)self + FIELD(int, self, 0x2070) * 0x74;
        void* obj = FIELD(void*, trk, 0x13CC);
        int result = ((SfdSjObj*)obj)->vtbl->getAvail(obj, 1);
        int thr = FIELD(int, trk, 0x13D4) * 80 / 100;
        int over;

        if (result >= thr) {
            over = 1;
        } else if (result < SFSET_GetCond(self, 0x46)) {
            over = 0;
        } else {
            over = 1;
        }
        if (over) return 0;
    }

    {
        SFTIM_GetTime(self, &sec, &usec);
        playSec = FIELD(int, self, 0x1020);
        playFrac = FIELD(int, self, 0x1024);
        playSec -= UTY_MulDiv(SFSET_GetCond(self, 0x44), playFrac, 1000000);
        if (sec <= 0 || playSec <= 0) return 0;
        return !UTY_CmpTime(sec, usec, playSec, playFrac);
    }
}

// ---------------------------------------------------------------------------
// sfply_IsBpaOff
// ---------------------------------------------------------------------------
int sfply_IsBpaOff(void* self) {
    int i;
    int ended;
    int timeSec, timeUsec;
    int playStartSec, playStartFrac;
    int limitVal, scaled;

    /* any transport/buffer still running counts as BPA-active */
    if (SFSET_GetCond(self, 5) && SFTRN_GetTermFlg(self, 6)) {
        ended = 1;
    } else if (SFSET_GetCond(self, 6) && SFTRN_GetTermFlg(self, 7)) {
        ended = 1;
    } else {
        ended = 0;
        for (i = 0; i < 8; i++) {
            if (SFBUF_GetTermFlg(self, i)) {
                ended = 1;
                break;
            }
        }
    }
    if (ended) return 1;

    if (SFSET_GetCond(self, 5) == 1) {
        u8* trackPtr = (u8*)self + FIELD(int, self, 0x2070) * 0x74;
        void* obj = FIELD(void*, trackPtr, 0x13CC);
        int result = ((SfdSjObj*)obj)->vtbl->getAvail(obj, 1);
        /* field_0x13D4 fill level compared against an 80% threshold */
        int over = result >= FIELD(int, trackPtr, 0x13D4) * 80 / 100 ||
                   result >= SFSET_GetCond(self, 0x46);
        if (over) return 1;
    }

    if (SFSET_GetCond(self, 6) == 1) {
        u8* trackPtr = (u8*)self + FIELD(int, self, 0x20B4) * 0x74;
        void* obj = FIELD(void*, trackPtr, 0x13CC);
        int result = ((SfdSjObj*)obj)->vtbl->getAvail(obj, 1);
        if (result >= FIELD(int, trackPtr, 0x13D4) * 80 / 100) return 1;
    }

    SFTIM_GetTime(self, &timeSec, &timeUsec);
    playStartSec = FIELD(int, self, 0x1020);
    playStartFrac = FIELD(int, self, 0x1024);
    limitVal = SFSET_GetCond(self, 0x45);
    scaled = UTY_MulDiv(limitVal, playStartFrac, 1000000);
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
        int gf = ((SfdSys*)sSfdGlob)->tim.field_0x08;
        int gl;
        if (gf >= 1000) gl = (1 - (0x7FFFFFFF / gf)) * -1000;
        else gl = 0x7FFFFFFF - 1000;
        if (limit > gl) limit = gl;
    }
    SFSET_SetCond(self, 0x36, limit);
}

// ---------------------------------------------------------------------------
// SFD_Create - create a player handle from the given config, register it in
// the global handle table, tracing entry/exit through the debug context.
// ---------------------------------------------------------------------------
int SFD_Create(void* config, void* extra) {
    char buf[0x200];
    void** hnTable;
    int err;
    int slot;
    int hn;

    /* trace-enter: dump the config through sprintf into the scratch buffer */
    if (gpSfdCoreTraceCb != NULL) {
        sprintf(buf, "%p, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %d, %p, %ld",
                FIELD(u32, config, 0x04), FIELD(u32, config, 0x08),
                FIELD(u32, config, 0x0C), FIELD(u32, config, 0x10),
                FIELD(u32, config, 0x14), FIELD(u32, config, 0x18),
                FIELD(u32, config, 0x1C), FIELD(u32, config, 0x20),
                FIELD(u32, config, 0x24), FIELD(u32, config, 0x2C),
                FIELD(u32, config, 0x30), FIELD(u32, config, 0x38),
                FIELD(u32, config, 0x3C), FIELD(u32, config, 0x40),
                FIELD(u32, config, 0x44), FIELD(u32, config, 0x48));

        /* sprintf is an external call, so the ctx global is re-checked */
        if (gpSfdCoreTraceCb != NULL) {
            void* ctx = gpSfdCoreTraceCb;
            sTraceSfdCreate.entry.args[0].value = buf;
            ((SfdTraceCtx*)ctx)->vtbl->trace(ctx, &sTraceSfdCreate.entry);
        }
    }

    hnTable = (void**)(sSfdGlob + 0x1FC);

    /* validate config: stream type and minimum work size */
    if (FIELD(u32, config, 0x04) == 0) {
        err = SFLIB_SetErr(0, 0xFF000204);
    } else if (FIELD(u32, config, 0x48) < 0x39D0) {
        err = SFLIB_SetErr(0, 0xFF000205);
    } else {
        err = 0;
    }

    if (err != 0) {
        hn = 0;
    } else {
        /* find a free handle-table slot (unrolled 8-entry scan) */
        slot = SFD_GLOB->field_0x1fc[0] == 0 ? 0 :
               SFD_GLOB->field_0x1fc[1] == 0 ? 1 :
               SFD_GLOB->field_0x1fc[2] == 0 ? 2 :
               SFD_GLOB->field_0x1fc[3] == 0 ? 3 :
               SFD_GLOB->field_0x1fc[4] == 0 ? 4 :
               SFD_GLOB->field_0x1fc[5] == 0 ? 5 :
               SFD_GLOB->field_0x1fc[6] == 0 ? 6 :
               SFD_GLOB->field_0x1fc[7] == 0 ? 7 : -1;

        if (slot == -1) {
            SFLIB_SetErr(0, 0xFF000206);
            hn = 0;
        } else {
            hn = (int)sfply_InitHn(config, extra);
            hnTable[slot] = (void*)hn;
        }
    }

    /* trace-exit */
    {
        void* ctx = gpSfdCoreTraceCb;
        if (ctx != NULL) {
            sTraceSfdCreate.exit.args[0].value = (void*)hn;
            ((SfdTraceCtx*)ctx)->vtbl->trace(ctx, &sTraceSfdCreate.exit);
        }
    }
    return hn;
}

// ---------------------------------------------------------------------------
// sfply_InitHn
// ---------------------------------------------------------------------------
void* sfply_InitHn(SfdCreateCfg* config, void* extra) {
    u8* work = config->work;
    u32 size = config->workSize;
    /* evaluated before the guards in retail: MWCC keeps it at decl point */
    u32 words = size >> 2;

    if (work == NULL) return NULL;
    if ((s32)size <= 0 || size > 0x73A0) return NULL;
    if ((int)sSfdWorkMemSize != 0 && (int)sSfdWorkMemSize != (int)size) return NULL;
    sSfdWorkMemSize = size;

    UTY_MemsetDword(work, 0, words);

    {
        SfdHandle* hn;
        f32 fps;
        int i;

        hn = (SfdHandle*)(((u32)work + 0x1F) & ~0x1F);

        /* the handle's first 0x50 bytes mirror the config block; the
           stream-position field of the config is 32-byte aligned first */
        hn->substatus = 0;
        hn->errorState = 0;
        config->stmBuf = (void*)((((u32)config->stmBuf) + 0x1F) & ~0x1F);
        *(SfdCreateCfg*)hn = *config;

        hn->field_0x0050 = 1;
        hn->field_0x005c = 0;
        hn->field_0x0060 = 0;
        hn->field_0x0064 = 0;
        hn->field_0x0068 = 0;
        hn->displayCount = 0;
        hn->field_0x0070 = 0;

        SFHDS_InitFhd(&hn->fhd);

        UTY_MemsetDword(&hn->imageWidth, 0, 0x10);
        hn->imageWidth = 0;
        hn->imageHeight = 0;
        hn->field_0x0924 = 0;
        hn->field_0x0928 = 0;
        hn->field_0x092c = 0;
        hn->field_0x0930 = 0;
        hn->field_0x0934 = 0;
        hn->field_0x0938 = 1;
        hn->field_0x093c = 0;
        hn->field_0x0940 = -1;
        hn->field_0x0944 = -1;
        hn->field_0x0948 = -1;

        sfply_InitPlyInf(&hn->field_0x0960);

        UTY_MemsetDword(hn->field_0x26a0, 0, 0x38);
        {
            u8* tmr = hn->field_0x26a0;
            for (i = 0; i < 5; i++) {
                SFTMR_InitTsum(tmr);
                tmr += 0x20;
            }
        }
        SFTMR_InitTsum(hn->field_0x2740);

        hn->field_0x2760 = 0;
        fps = 0.0f; /* retail .rodata word 0x00000000 */
        hn->field_0x2768 = 0;
        hn->field_0x2770 = 0;
        hn->field_0x2778 = 0;
        hn->field_0x277c = fps;

        fn_803C34DC(&hn->errFn);
        MEM_Copy(hn->cond, sSfdGlob, 0x190);
        MEM_Copy(hn->condBak, sSfdGlob, 0x190);
        memset(&hn->hdrCb, 0, 0x5C);
        hn->field_0x0d8c = (void*)0x7FFFFFFF;
        SFTIM_InitHn(hn, &hn->tim);

        if (SFBUF_InitHn(hn, hn->buf, config)) return NULL;

        SFTRN_InitHn(hn, hn->trn, config, extra);
        SFSEEKI_InitHn(&hn->seeki);
        SFSEE_InitHn(&hn->see);
        if (SFTRN_CallTrSetup(hn, 3)) return NULL;

        hn->substatus = 1;
        hn->errorState = 1;
        return hn;
    }
}

// ---------------------------------------------------------------------------
// sfply_InitPlyInf
// ---------------------------------------------------------------------------
// TEMP EXPERIMENT: renamed to test IPA-inlining hypothesis
void sfply_InitPlyInf_x(void* self) {
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
    /* decoded-picture counter bump + optional notify callback */
    SfdHandle* hn = (SfdHandle*)self;
    void (*cb)(void*, void*, u32*) = hn->field_0x0d6c;
    hn->field_0x0960 += delta;
    if (cb != NULL) {
        cb(hn->field_0x0d70, param, (u32*)&hn->field_0x0960);
    }
}

// ---------------------------------------------------------------------------
// SFPLY_AddSkipPic
// ---------------------------------------------------------------------------
void SFPLY_AddSkipPic(void* self, int delta, void* param) {
    SfdHandle* hn = (SfdHandle*)self;
    void (*cb)(void*, void*, u32*) = hn->field_0x0d74;
    u32 count = hn->field_0x0964 + delta;
    hn->field_0x0964 = count;
    if (cb != NULL) {
        cb(hn->field_0x0d78, param, (u32*)&hn->field_0x0960);
    }
}

// ---------------------------------------------------------------------------
// SFD_Destroy
// ---------------------------------------------------------------------------
int SFD_Destroy(void* self) {
    SfdSys* g;
    int result;
    void* cs;
    void* ctx;
    if (SFLIB_CheckHn(self)) {
        return SFLIB_SetErr(0, 0xff000131);
    }

    ctx = gpSfdCoreTraceCb;
    if (ctx) {
        sTraceSfdDestroy.entry.args[0].value = self;
        ((SfdTraceCtx*)ctx)->vtbl->trace(ctx, &sTraceSfdDestroy.entry);
    }

    if (FIELD(int, self, 0x54) == 1) {
        /* stopping from idle: cancel the pending buffer term, no reset */
        SFBUF_SetTermFlg(self, FIELD(int, self, 0x1FEC), 0);
    } else {
        int tr;
        if (FIELD(int, self, 0x54) == 4) {
            tr = SFTRN_CallTrtTrif(self, 7, 7, 0, 0);
            if (tr != 0) goto trJoin;
            goto parkFields;
        }
parkFields:
        /* park in state 1, then run the protected reset */
        FIELD(int, self, 0x54) = 1;
        tr = 0;
        FIELD(int, self, 0x58) = 1;
trJoin:
        if (tr == 0) {
            g = SFD_GLOB;
            FIELD(int, self, 0x58) = 0;
            FIELD(int, self, 0x54) = 0;
            g->field_0x1f0 = 1;
            sfply_ResetHn(self);
            g->field_0x1f0 = 0;
        }
    }
    SFHDS_FinishFhd(&((SfdHandle*)self)->fhd);
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
            if ((void*)SFD_GLOB->field_0x1fc[i] == self) SFD_GLOB->field_0x1fc[i] = 0;
        }
    }
    SFLIB_UnlockCs(&cs);

    ctx = gpSfdCoreTraceCb;
    if (ctx) {
        ((SfdTraceCtx*)ctx)->vtbl->trace(ctx, &sTraceSfdDestroy.exit);
    }
    return result;
}

// ---------------------------------------------------------------------------
// SFD_Start
// ---------------------------------------------------------------------------
int SFD_Start(void* self) {
    int result;
    void* ctx;

    if (SFLIB_CheckHn(self)) {
        return SFLIB_SetErr(0, 0xff000132);
    }
    ctx = gpSfdCoreTraceCb;
    if (ctx) {
        sTraceSfdStart.entry.args[0].value = self;
        ((SfdTraceCtx*)ctx)->vtbl->trace(ctx, &sTraceSfdStart.entry);
    }

    if (SFSET_GetCond(self, 0x2F) == 1) {
        result = SFPL2_Standby(self);
    } else {
        FIELD(int, self, 0x58) = 4;
        result = 0;
    }
    FIELD(int, self, 0x50) = 1;

    ctx = gpSfdCoreTraceCb;
    if (ctx) {
        ((SfdTraceCtx*)ctx)->vtbl->trace(ctx, &sTraceSfdStart.exit);
    }
    return result;
}

// ---------------------------------------------------------------------------
// fn_803CD484
// ---------------------------------------------------------------------------
int fn_803CD484(void* self) {
    int result = SFTRN_CallTrtTrif(self, 2, 6, 0, 0);
    int ret = 0;
    if (result != 0) {
        ret = result;
    }
    return ret;
}

// ---------------------------------------------------------------------------
// SFD_Stop
// ---------------------------------------------------------------------------
int SFD_Stop(void* self) {
    void* ctx;
    SfdSys* g;
    int result;
    int status;

    if (SFLIB_CheckHn(self)) {
        return SFLIB_SetErr(0, 0xff000133);
    }
    ctx = gpSfdCoreTraceCb;
    if (ctx) {
        sTraceSfdStop.entry.args[0].value = self;
        ((SfdTraceCtx*)ctx)->vtbl->trace(ctx, &sTraceSfdStop.entry);
    }

    status = FIELD(int, self, 0x54);
    if (status == 1) {
        SFBUF_SetTermFlg(self, FIELD(int, self, 0x1FEC), 0);
        result = 0;
    } else {
        int tr;
        tr = 0;
        if (status == 4) {
            tr = SFTRN_CallTrtTrif(self, 7, 7, 0, 0);
            if (tr != 0) {
                goto stopCheck;
            }
        }
        /* park in state 1 */
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
            g->field_0x1f0 = 1;
            tr = sfply_ResetHn(self);
            g->field_0x1f0 = result;
            if (tr != 0) result = tr;
        }
    }
    FIELD(int, self, 0x50) = 1;

    ctx = gpSfdCoreTraceCb;
    if (ctx) {
        ((SfdTraceCtx*)ctx)->vtbl->trace(ctx, &sTraceSfdStop.exit);
    }
    return result;
}

// ---------------------------------------------------------------------------
// SFPLY_GetResetFlg
// ---------------------------------------------------------------------------
int SFPLY_GetResetFlg(void) {
    return SFD_GLOB->field_0x1f0;
}

// ---------------------------------------------------------------------------
// sfply_ResetHn - tear the player handle down and rebuild it in place,
// carrying over every user-configured callback/setting from the old handle.
// ---------------------------------------------------------------------------
int sfply_ResetHn(void* self) {
    SfdHandle* hn = (SfdHandle*)self;
    /* snapshot of the original config header (bytes 0x00..0x4F), reused as
       the config for the rebuilt handle */
    SfdCreateCfg cfgHdr;
    /* output blocks for the transport 'treat' callbacks */
    u32 trifOut[5];
    void* cs;
    u32 trifInOut[2];
    /* saved media-processor condition buffer */
    u8 mpvCond[0x40];
    /* temp copy of the 0x190-byte info block */
    u8 tmpInfo[0x190];

    s32 hadTrk;
    s32 savTermId = 0;
    s32 sav0x64;
    u32 savA0C;
    u32 sav1078;
    u32 sav108C;
    u32 sav1090;
    void* pA08;
    void* p1074;
    void* p107C;
    void* pDB0;
    u32 spdNum;
    u32 spdDen;
    s32 limitTime;
    void* seekData;
    u32 skByRate;
    u32 skFileSize;
    u32 skTotSec;
    u32 skTotFrac;
    u32 skPos;
    u32 vPts;
    u32 vPtsType;
    SfdBlk5C blkD38;
    u32 cbGrp1[3];
    u32 cbGrp2[3];
    u32 cycGrp[4];
    u32 tfGrp[4];

    cfgHdr = *(SfdCreateCfg*)hn;
    hadTrk = hn->cond[8];

    if (hadTrk != 0) {
        if (SFLIB_CheckHn(hn)) {
            SFLIB_SetErr(NULL, 0xFF000134);
        } else {
            SFTRN_CallTrtTrif(hn, 0, 9, (int)trifOut, 0);
        }
        savTermId = trifOut[5];
    }

    SFHDS_FinishFhd(&hn->fhd);
    SFBUF_DestroySj(hn);

    /* save every piece of runtime state that must survive the rebuild */
    {
        s32 mpvRet;
        SfdHandle* nu;

        sav0x64 = hn->field_0x0064;
        blkD38 = *(SfdBlk5C*)((u8*)hn + 0xD38);
        savA0C = hn->errFnArg;
        sav1078 = (u32)hn->tim.usrTimeFnArg;
        sav108C = hn->tim.extClockWrap;
        sav1090 = (u32)hn->tim.extClockArg;
        pA08 = (void*)hn->errFn;
        p1074 = hn->tim.usrTimeFn;
        p107C = hn->tim.extClockFn;
        pDB0 = hn->tim.field_0x018;
        spdNum = hn->tim.speedNum;
        spdDen = hn->tim.speedDen;
        cbGrp1[0] = FIELD(u32, hn, 0x1368);
        cbGrp1[1] = FIELD(u32, hn, 0x136C);
        cbGrp1[2] = FIELD(u32, hn, 0x1370);
        cbGrp2[0] = FIELD(u32, hn, 0x1374);
        cbGrp2[1] = FIELD(u32, hn, 0x1378);
        cbGrp2[2] = FIELD(u32, hn, 0x137C);
        limitTime = hn->tim.limitTime;
        seekData = hn->see.avplay;
        cycGrp[0] = FIELD(u32, hn, 0x1380);
        cycGrp[1] = FIELD(u32, hn, 0x1384);
        cycGrp[2] = FIELD(u32, hn, 0x1388);
        cycGrp[3] = FIELD(u32, hn, 0x138C);
        if (seekData != NULL) {
            skByRate = FIELD(u32, seekData, 0xDD0);
            skFileSize = FIELD(u32, seekData, 0xDC4);
            skTotSec = FIELD(u32, seekData, 0xDC8);
            skTotFrac = FIELD(u32, seekData, 0xDCC);
            skPos = FIELD(u32, seekData, 0xDD4);
        } else {
            skByRate = 0;
            skFileSize = 0;
            skTotSec = 0;
            skTotFrac = 0;
            skPos = 0;
        }
        vPts = FIELD(u32, hn, 0x1464);
        vPtsType = FIELD(u32, hn, 0x1468) << 4;
        tfGrp[0] = FIELD(u32, hn, 0x39A0);
        tfGrp[1] = FIELD(u32, hn, 0x39A4);
        tfGrp[2] = FIELD(u32, hn, 0x39A8);
        tfGrp[3] = FIELD(u32, hn, 0x39AC);
        mpvRet = SFMPV_SaveCond(hn, mpvCond, 0x40);
        hn->errorState = 0;
        hn->substatus = 0;
        SFLIB_UnlockCs(&cs);

        {
            int tr = SFTRN_CallTrSetup(hn, 4);
            if (tr != 0) return tr;
        }

        MEM_Copy(tmpInfo, hn->condBak, 0x190);

        {
            nu = (SfdHandle*)sfply_InitHn(&cfgHdr, NULL);
            if (nu == NULL) {
                return SFLIB_SetErr(NULL, 0xFF000202);
            }

            MEM_Copy(nu->cond, tmpInfo, 0x190);
            MEM_Copy(nu->condBak, tmpInfo, 0x190);
            SFMPV_RestoreCond(nu, mpvCond, mpvRet);

            if (hadTrk != 0) {
                if (SFLIB_CheckHn(nu)) {
                    SFLIB_SetErr(NULL, 0xFF000134);
                } else {
                    s32 tr9 = SFTRN_CallTrtTrif(nu, 0, 9, (int)trifOut, 0);
                    if (tr9 != 0) return tr9;
                }
                trifInOut[0] = savTermId;
                trifInOut[1] = trifOut[5];
                if (SFLIB_CheckHn(nu)) {
                    SFLIB_SetErr(NULL, 0xFF000135);
                } else {
                    s32 trA = SFTRN_CallTrtTrif(nu, 0, 0xA, (int)trifInOut, (int)(trifInOut + 1));
                    if (trA != 0) return trA;
                }
            }

            /* trace-enter */
            {
                void* ctx = gpSfdCoreTraceCb;
                if (ctx) {
                    sTraceSfdTermSupply.entry.args[0].value = nu;
                    ((SfdTraceCtx*)ctx)->vtbl->trace(ctx, &sTraceSfdTermSupply.entry);
                }
            }

            if (SFLIB_CheckHn(nu)) {
                SFLIB_SetErr(NULL, 0xFF00013D);
            } else {
                s32 termId = nu->trn[0].termIdx;
                if (SFBUF_GetTermFlg(nu, termId) != 1) {
                    SFBUF_SetTermFlg(nu, termId, 1);
                    nu->field_0x0050 = 1;
                }
            }

            /* trace-exit */
            {
                void* ctx = gpSfdCoreTraceCb;
                if (ctx) {
                    ((SfdTraceCtx*)ctx)->vtbl->trace(ctx, &sTraceSfdTermSupply.exit);
                }
            }

            nu->field_0x0064 = sav0x64;
            *(SfdBlk5C*)((u8*)nu + 0xD38) = blkD38;

            {
                u32 mbEn = nu->field_0x0d84;
                if (mbEn != 0) {
                    SFD_SetMbCb(nu, nu->field_0x0d8c, mbEn, nu->field_0x0d88);
                }
            }
            if (pA08 != NULL) fn_803C0D94(nu, (u32)pA08, savA0C);
            if (p1074 != NULL) SFD_SetUsrTimeFn(nu, p1074, sav1078);
            if (p107C != NULL) SFD_SetExtClockFn(nu, p107C, sav108C, sav1090);
            if (pDB0 != NULL) SFD_SetUsrIsSkipFn(nu, pDB0);
            if (spdNum != spdDen) SFD_SetSpeedRational(nu, spdNum, spdDen);

            if (cbGrp1[0] != 0) {
                FIELD(u32, nu, 0x1368) = cbGrp1[0];
                FIELD(u32, nu, 0x136C) = cbGrp1[1];
                FIELD(u32, nu, 0x1370) = cbGrp1[2];
                void (*fp)(void*) = (void (*)(void*))gpSfdEventCallback;
                if (fp != NULL) fp(cbGrp1);
            }
            if (cbGrp2[0] != 0) {
                FIELD(u32, nu, 0x1374) = cbGrp2[0];
                FIELD(u32, nu, 0x1378) = cbGrp2[1];
                FIELD(u32, nu, 0x137C) = cbGrp2[2];
                void (*fp)(void*) = (void (*)(void*))gpSfdEventCallback;
                if (fp != NULL) fp(cbGrp2);
            }

            if (cycGrp[1] != 0) SFD_SetCyclicFrameOutput(nu, (void*)cycGrp[0], cycGrp[1]);
            if (limitTime != -1) SFD_SetLimitTime(nu, limitTime);
            if (seekData != NULL) {
                SFD_EntrySeek(nu, seekData);
                SFD_SetByteRate(nu, skByRate);
                SFD_SetFileSize(nu, skFileSize);
                SFD_SetTotTime(nu, skTotSec, skTotFrac);
                SFD_SetSeekPos(nu, skPos);
            }
            if (vPts != 0) SFD_SetVideoPts(nu, vPts, vPtsType);
            if (tfGrp[0] != 0) fn_803C1570(nu, (void*)tfGrp[0], tfGrp[1]);
        }
    }
    return 0;
}

// ---------------------------------------------------------------------------
// SFD_RequestStop
// ---------------------------------------------------------------------------
void SFD_RequestStop(void* self) {
    void* ctx;

    if (SFLIB_CheckHn(self)) {
        SFLIB_SetErr(0, 0xff00013C);
        return;
    }
    if (FIELD(int, self, 0x54) == 1) return;

    ctx = gpSfdCoreTraceCb;
    if (ctx) {
        sTraceSfdRequestStop.entry.args[0].value = self;
        ((SfdTraceCtx*)ctx)->vtbl->trace(ctx, &sTraceSfdRequestStop.entry);
    }

    FIELD(int, self, 0x70) = 1;
    SFTRN_CallTrtTrif(self, 1, 5, 0, 0);
    SFTRN_CallTrtTrif(self, 2, 5, 0, 0);
    SFTRN_CallTrtTrif(self, 3, 5, 0, 0);

    ctx = gpSfdCoreTraceCb;
    if (ctx) {
        ((SfdTraceCtx*)ctx)->vtbl->trace(ctx, &sTraceSfdRequestStop.exit);
    }
}

// ---------------------------------------------------------------------------
// SFD_TermSupply
// ---------------------------------------------------------------------------
int SFD_TermSupply(void* self) {
    void* ctx;
    int bufId;
    int result;

    ctx = gpSfdCoreTraceCb;
    if (ctx) {
        sTraceSfdTermSupply.entry.args[0].value = self;
        ((SfdTraceCtx*)ctx)->vtbl->trace(ctx, &sTraceSfdTermSupply.entry);
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
    ctx = gpSfdCoreTraceCb;
    if (ctx) {
        ((SfdTraceCtx*)ctx)->vtbl->trace(ctx, &sTraceSfdTermSupply.exit);
    }
    return result;
}

// ---------------------------------------------------------------------------
// SFD_GetFrm
// ---------------------------------------------------------------------------
int SFD_GetFrm(void* self, void** outFrm) {
    int result = 0;
    void* ctx;

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

    ctx = gpSfdCoreTraceCb;
    if (ctx) {
        sTraceSfdGetFrm.entry.args[0].value = self;
        ((SfdTraceCtx*)ctx)->vtbl->trace(ctx, &sTraceSfdGetFrm.entry);
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
        ctx = gpSfdCoreTraceCb;
        if (ctx) {
            void* frm = *outFrm;
            sTraceSfdGetFrm.exit.args[0].value = frm;
            sTraceSfdGetFrm.exit.args[1].value = &((SfdFrmInfo*)frm)->time;
            sTraceSfdGetFrm.exit.args[2].value = &((SfdFrmInfo*)frm)->field_0x10;
            ((SfdTraceCtx*)ctx)->vtbl->trace(ctx, &sTraceSfdGetFrm.exit);
        }
    } else {
        ctx = gpSfdCoreTraceCb;
        if (ctx) {
            sTraceSfdGetFrm.exit.args[0].value = NULL;
            sTraceSfdGetFrm.exit.args[1].value = NULL;
            sTraceSfdGetFrm.exit.args[2].value = NULL;
            ((SfdTraceCtx*)ctx)->vtbl->trace(ctx, &sTraceSfdGetFrm.exit);
        }
    }

    if (gpSfdFrameCallback != NULL) {
        gpSfdFrameCallback(self, *outFrm);
    }
    return result;
}

// ---------------------------------------------------------------------------
// SFD_RelFrm
// ---------------------------------------------------------------------------
int SFD_RelFrm(void* self, void* frm) {
    int result;
    void* ctx;

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

    ctx = gpSfdCoreTraceCb;
    if (ctx) {
        sTraceSfdRelFrm.entry.args[0].value = self;
        sTraceSfdRelFrm.entry.args[1].value = frm;
        ((SfdTraceCtx*)ctx)->vtbl->trace(ctx, &sTraceSfdRelFrm.entry);
    }

    {
        int rc = FIELD(int, self, 0x97C);
        if (rc < FIELD(int, self, 0x978)) {
            FIELD(int, self, 0x97C) = rc + 1;
        }
    }

    result = SFTRN_CallTrtTrif(self, 6, 0xC, (int)frm, 0);

    ctx = gpSfdCoreTraceCb;
    if (ctx) {
        ((SfdTraceCtx*)ctx)->vtbl->trace(ctx, &sTraceSfdRelFrm.exit);
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
