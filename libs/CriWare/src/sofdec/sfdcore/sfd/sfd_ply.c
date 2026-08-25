// CriWare Sofdec player core - sfd_ply TU
// High-level C reconstruction for MWCC matching.

#include <types.h>
#include <string.h>

// Player configuration passed to SFD_Create / sfply_InitHn
typedef struct SfdPlyCfg {
    u8 pad00[4];
    u32 field_0x04;   /* stream position, aligned to 0x20 by InitHn */
    u8 pad08[0x3C];
    void* work;       /* 0x44: work area */
    u32 workSize;     /* 0x48: work area size */
} SfdPlyCfg;

// 0x50-byte header block cloned from the config into the handle
typedef struct SfdCopyBlk {
    u32 w[0x14];
} SfdCopyBlk;

// 0x5C-byte block saved/restored around the handle rebuild (self+0xD38)
typedef struct SfdBlk5C {
    u32 w[0x17];
} SfdBlk5C;

// Player handle laid out at the start of the (32B-aligned) work area
typedef struct SfdPlyHn {
    u8 pad0000[0x50];
    s32 active;       /* 0x50 */
    s32 status;       /* 0x54 */
    s32 substatus;    /* 0x58 */
    s32 field_0x5C;
    s32 field_0x60;
    s32 field_0x64;
    s32 field_0x68;
    s32 field_0x6C;
    s32 field_0x70;
    u8 pad0074[0x14];
    u8 fhd[0x894];                    /* 0x88 .. 0x91C */
    s32 field_0x91C;
    u8 pad0920[0x18];                 /* 0x920 .. 0x938 */
    s32 field_0x938;
    u8 pad093C[4];
    s32 field_0x940;
    s32 field_0x944;
    s32 field_0x948;
    u8 pad094C[0x3F0];                /* 0x94C .. 0xD3C */
    u8 field_0xD3C[0x50];             /* 0xD3C .. 0xD8C, memset 0 */
    s32 field_0xD8C;
    u8 pad0D90[8];
    u8 tim[0x620];                    /* 0xD98 .. 0x13B8 */
    u8 buf[0xC20];                    /* 0x13B8 .. 0x1FD8 */
    u8 trn[0x698];                    /* 0x1FD8 .. 0x2670 */
    u8 see[0x10];                     /* 0x2670 */
    u8 seeki[0x20];                   /* 0x2680 */
    u8 tsum[0xA0];                    /* 0x26A0 .. 0x2740: 5 x 0x20 */
    u8 tsumLast[0x20];                /* 0x2740 */
    u64 field_0x2760;                 /* last frame timer stamp */
    u64 field_0x2768;                 /* current timer stamp */
    u64 field_0x2770;                 /* timer unit */
    s32 field_0x2778;
    f32 field_0x277C;
} SfdPlyHn;

// Player handle view used by sfply_StatPrep (offsets verified against retail)
typedef struct SfdPlyStatView {
    u8 pad0000[0x50];
    s32 active;         /* 0x50 */
    s32 status;         /* 0x54 */
    s32 substatus;      /* 0x58 */
    u8 pad005C[0x9D4];
    s32 avPauseV;       /* 0xA30: video pause-request flag */
    s32 avPauseA;       /* 0xA34: audio pause-request flag */
    u8 pad0A38[0x1C];
    s32 field_0xA54;
    s32 playMode;       /* 0xA58 */
    u8 pad0A5C[0x74];
    s32 field_0xAD0;
    u8 pad0AD4[0x57C];
    s32 field_0x1050;
    u8 pad1054[0x18];
    s32 field_0x106C;
    u8 pad1070[0xC];
    s32 field_0x107C;
} SfdPlyStatView;

// Forward declarations for internal functions
void sfply_ExecOne(void* self);
int sfply_StatPrep(SfdPlyStatView* self, int canExec);
int fn_803CC170(void* self);
void fn_803CC238(void* self);
int sfply_IsBpaOn(void* self);
int sfply_IsBpaOff(void* self);
int sfply_IsEtrg(void* self);
int criware_803C9FC0(void* self);
int sfply_IsPlayTimeAutoStop(void* self);
void criware_803CA124(void* self, int fpsArg);
void* sfply_InitHn(SfdPlyCfg* config, void* extra);
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
        SFTMR_AddTsum((u8*)self + 0x2740, (u32)now, (s64)(now - startTmr));
    }
}

// ---------------------------------------------------------------------------
// sfply_StatPrep
// ---------------------------------------------------------------------------
int sfply_StatPrep(SfdPlyStatView* self, int canExec) {
    s32 origStatus = self->status;    /* 0x54 */
    s32 origSub = self->substatus;    /* 0x58 */
    s32 newState;

    if (!fn_803CC170(self)) return origStatus;

    /* clear a pending pause request once its buffer queue has fully drained */
    if (self->avPauseV == 1) {
        if (SFBUF_GetWTot(self, 1) == 0 && SFBUF_GetRTot(self, 1) == 0)
            self->avPauseV = 0;
    }
    if (self->avPauseA == 1) {
        if (SFBUF_GetWTot(self, 2) == 0 && SFBUF_GetRTot(self, 2) == 0)
            self->avPauseA = 0;
    }

    SFSEE_FixAvPlay(self, self->avPauseV, self->avPauseA);

    if (!self->avPauseA && self->playMode == 2)
        SFSET_SetCond(self, 0xF, 1);
    if (!self->avPauseV && self->playMode == 1)
        SFSET_SetCond(self, 0xF, 2);
    if (self->playMode == 1) {
        if (self->field_0x107C)
            SFSET_SetCond(self, 0xF, 5);
        else
            SFSET_SetCond(self, 0xF, 1);
    }

    fn_803CC238(self);

    /* Trace: stash handle + pause/mode field pointers into the static record,
       then invoke the debug context's vtable entry */
 {
        void* ctx = lbl_eu_80606E34;
        if (ctx) {
            *(void**)(lbl_eu_80568C84 + 0x0C) = self;
            *(void**)(lbl_eu_80568C84 + 0x18) = &self->avPauseV;
            *(void**)(lbl_eu_80568C84 + 0x24) = &self->avPauseA;
            *(void**)(lbl_eu_80568C84 + 0x30) = &self->playMode;
            *(void**)(lbl_eu_80568C84 + 0x3C) = (u8*)self + 0xA80;
            {
                void* vtbl = *(void**)ctx;
                (*(void (**)(void*, void*))((u8*)vtbl + 0x24))(ctx, lbl_eu_80568C84 + 4);
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
        if (!self->field_0xA54) shouldStop = 1;
        else if (!self->avPauseV) shouldStop = 1;
        else if (self->field_0x1050) shouldStop = 1;
        else if (self->field_0x106C >= self->field_0xAD0) shouldStop = 1;
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
        int (*fn)(void*, int) = *(int (**)(void*, int))((u8*)*(void**)obj + 0x24);
        int result = fn(obj, 1);
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
        /* field_0x13D4 fill level compared against an 80% threshold */
        if (result >= FIELD(int, trackPtr, 0x13D4) * 80 / 100 ||
            result >= SFSET_GetCond(self, 0x46)) return 1;
    }

    if (SFSET_GetCond(self, 6) == 1) {
        u8* trackPtr = (u8*)self + FIELD(int, self, 0x20B4) * 0x74;
        void* obj = FIELD(void*, trackPtr, 0x13CC);
        void* vtbl = *(void**)obj;
        int (*fn)(void*, int) = *(int (**)(void*, int))((u8*)vtbl + 0x24);
        int result = fn(obj, 1);
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
        int gf = *(int*)(lbl_eu_80606E38 + 0x1A4);
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
    u32* hnTable;
    int err;
    int slot;
    int hn;

    /* trace-enter: dump the config through sprintf into the scratch buffer */
    if (lbl_eu_80606E34 != NULL) {
        sprintf(buf, lbl_eu_8051CBA8,
                FIELD(u32, config, 0x04), FIELD(u32, config, 0x08),
                FIELD(u32, config, 0x0C), FIELD(u32, config, 0x10),
                FIELD(u32, config, 0x14), FIELD(u32, config, 0x18),
                FIELD(u32, config, 0x1C), FIELD(u32, config, 0x20),
                FIELD(u32, config, 0x24), FIELD(u32, config, 0x2C),
                FIELD(u32, config, 0x30), FIELD(u32, config, 0x38),
                FIELD(u32, config, 0x3C), FIELD(u32, config, 0x40),
                FIELD(u32, config, 0x44), FIELD(u32, config, 0x48));

        /* sprintf is an external call, so the ctx global is re-checked */
        if (lbl_eu_80606E34 != NULL) {
            void* ctx = lbl_eu_80606E34;
            void** rec = (void**)lbl_eu_80567FAC;
            void* vt;
            rec[3] = (void*)buf;
            vt = *(void**)ctx;
            (*(void (**)(void*, void*))((u8*)vt + 0x24))(ctx, (char*)rec + 4);
        }
    }

    hnTable = (u32*)(lbl_eu_80606E38 + 0x1FC);

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
        slot = ((SfdGlob*)lbl_eu_80606E38)->hnSlots[0] == NULL ? 0 :
               ((SfdGlob*)lbl_eu_80606E38)->hnSlots[1] == NULL ? 1 :
               ((SfdGlob*)lbl_eu_80606E38)->hnSlots[2] == NULL ? 2 :
               ((SfdGlob*)lbl_eu_80606E38)->hnSlots[3] == NULL ? 3 :
               ((SfdGlob*)lbl_eu_80606E38)->hnSlots[4] == NULL ? 4 :
               ((SfdGlob*)lbl_eu_80606E38)->hnSlots[5] == NULL ? 5 :
               ((SfdGlob*)lbl_eu_80606E38)->hnSlots[6] == NULL ? 6 :
               ((SfdGlob*)lbl_eu_80606E38)->hnSlots[7] == NULL ? 7 : -1;

        if (slot == -1) {
            SFLIB_SetErr(0, 0xFF000206);
            hn = 0;
        } else {
            hn = (int)sfply_InitHn(config, extra);
            hnTable[slot] = (u32)hn;
        }
    }

    /* trace-exit */
    {
        void* ctx = lbl_eu_80606E34;
        if (ctx != NULL) {
            void** rec = (void**)lbl_eu_80567FAC;
            void* vt;
            rec[0x1D] = (void*)hn;
            vt = *(void**)ctx;
            (*(void (**)(void*, void*))((u8*)vt + 0x24))(ctx, (char*)rec + 0x6C);
        }
    }
    return hn;
}

// ---------------------------------------------------------------------------
// sfply_InitHn
// ---------------------------------------------------------------------------
void* sfply_InitHn(SfdPlyCfg* config, void* extra) {
    u8* work = config->work;
    u32 size = config->workSize;
    /* evaluated before the guards in retail: MWCC keeps it at decl point */
    u32 words = size >> 2;

    if (work == NULL) return NULL;
    if ((s32)size <= 0 || size > 0x73A0) return NULL;
    if ((int)lbl_eu_80619BA4 != 0 && (int)lbl_eu_80619BA4 != (int)size) return NULL;
    lbl_eu_80619BA4 = size;

    UTY_MemsetDword(work, 0, words);

    {
        SfdPlyHn* hn;
        f32 fps;
        int i;
        u32* src;
        u32* dst;

        hn = (SfdPlyHn*)(((u32)work + 0x1F) & ~0x1F);

        /* align the stream-position field in the config, then clone the
           config header (bytes 0x04..0x53) into the handle */
        hn->substatus = 0;
        hn->status = 0;
        src = (u32*)config - 1;
        dst = (u32*)hn - 1;
        config->field_0x04 = (config->field_0x04 + 0x1F) & ~0x1F;

        /* down-counter copy */
        {
            int n = 10;
            do {
                dst[1] = src[1];
                dst[2] = src[2];
                src += 2;
                dst += 2;
            } while (--n);
        }

        hn->active = 1;
        hn->field_0x5C = 0;
        hn->field_0x60 = 0;
        hn->field_0x64 = 0;
        hn->field_0x68 = 0;
        hn->field_0x6C = 0;
        hn->field_0x70 = 0;

        SFHDS_InitFhd(hn->fhd);

        UTY_MemsetDword(&hn->field_0x91C, 0, 0x10);
        hn->field_0x91C = 0;
        *(s32*)&hn->pad0920[0x00] = 0;
        *(s32*)&hn->pad0920[0x04] = 0;
        *(s32*)&hn->pad0920[0x08] = 0;
        *(s32*)&hn->pad0920[0x0C] = 0;
        *(s32*)&hn->pad0920[0x10] = 0;
        *(s32*)&hn->pad0920[0x14] = 0;
        hn->field_0x938 = 1;
        *(s32*)&hn->pad093C[0] = 0;
        hn->field_0x940 = -1;
        hn->field_0x944 = -1;
        hn->field_0x948 = -1;

        sfply_InitPlyInf((u8*)hn + 0x960);

        UTY_MemsetDword(hn->tsum, 0, 0x38);
        {
            u8* tmr = hn->tsum;
            for (i = 0; i < 5; i++) {
                SFTMR_InitTsum(tmr);
                tmr += 0x20;
            }
        }
        SFTMR_InitTsum(hn->tsumLast);

        hn->field_0x2760 = 0;
        fps = lbl_eu_8051CBA4;
        hn->field_0x2768 = 0;
        hn->field_0x2770 = 0;
        hn->field_0x2778 = 0;
        hn->field_0x277C = fps;

        fn_803C34DC((u8*)hn + 0xA08);
        MEM_Copy((u8*)hn + 0xA1C, lbl_eu_80606E38, 0x190);
        MEM_Copy((u8*)hn + 0xBAC, lbl_eu_80606E38, 0x190);
        memset(hn->field_0xD3C, 0, 0x5C);
        hn->field_0xD8C = 0x7FFFFFFF;
        SFTIM_InitHn(hn, hn->tim);

        if (SFBUF_InitHn(hn, hn->buf, config)) return NULL;

        SFTRN_InitHn(hn, hn->trn, config, extra);
        SFSEEKI_InitHn(hn->seeki);
        SFSEE_InitHn(hn->see);
        if (SFTRN_CallTrSetup(hn, 3)) return NULL;

        hn->substatus = 1;
        hn->status = 1;
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
    void (*cb)(void*, void*, u32*) = *(void (**)(void*, void*, u32*))((u8*)self + 0xD6C);
    FIELD(u32, self, 0x960) += delta;
    if (cb != NULL) {
        cb(FIELD(void*, self, 0xD70), param, (u32*)((s32)self + 0x960));
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
    void* ctx;
    void* vtbl;
    void (*trace)(void*, void*);
    SfdGlob* g;
    int result;
    int status;

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
            g->resetFlg = 1;
            tr = sfply_ResetHn(self);
            g->resetFlg = result;
            if (tr != 0) result = tr;
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
// sfply_ResetHn - tear the player handle down and rebuild it in place,
// carrying over every user-configured callback/setting from the old handle.
// ---------------------------------------------------------------------------
int sfply_ResetHn(void* self) {
    u8* hn = (u8*)self;
    /* snapshot of the original config header (bytes 0x00..0x4F), reused as
       the config for the rebuilt handle */
    SfdCopyBlk cfgHdr;
    /* output blocks for the transport 'treat' callbacks */
    u32 trifOut[5];
    void* cs;
    u32 trifInOut[2];
    /* saved media-processor condition buffer */
    u8 mpvCond[0x40];
    /* temp copy of the 0x190-byte info block */
    u8 tmpInfo[0x190];

    s32 hadTrk;
    s32 savTermId;
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

    savTermId = 0;

    cfgHdr = *(SfdCopyBlk*)hn;
    hadTrk = FIELD(s32, hn, 0xA3C);

    if (hadTrk != 0) {
        if (SFLIB_CheckHn(hn)) {
            SFLIB_SetErr(NULL, 0xFF000134);
        } else {
            SFTRN_CallTrtTrif(hn, 0, 9, (int)trifOut, 0);
        }
        savTermId = trifOut[5];
    }

    SFHDS_FinishFhd(hn + 0x88);
    SFBUF_DestroySj(hn);

    /* save every piece of runtime state that must survive the rebuild */
    {
        s32 mpvRet;
        SfdPlyHn* nu;

        sav0x64 = FIELD(s32, hn, 0x64);
        blkD38 = *(SfdBlk5C*)(hn + 0xD38);
        savA0C = FIELD(u32, hn, 0xA0C);
        sav1078 = FIELD(u32, hn, 0x1078);
        sav108C = FIELD(u32, hn, 0x108C);
        sav1090 = FIELD(u32, hn, 0x1090);
        pA08 = FIELD(void*, hn, 0xA08);
        p1074 = FIELD(void*, hn, 0x1074);
        p107C = FIELD(void*, hn, 0x107C);
        pDB0 = FIELD(void*, hn, 0xDB0);
        spdNum = FIELD(u32, hn, 0x1048);
        spdDen = FIELD(u32, hn, 0x104C);
        cbGrp1[0] = FIELD(u32, hn, 0x1368);
        cbGrp1[1] = FIELD(u32, hn, 0x136C);
        cbGrp1[2] = FIELD(u32, hn, 0x1370);
        cbGrp2[0] = FIELD(u32, hn, 0x1374);
        cbGrp2[1] = FIELD(u32, hn, 0x1378);
        cbGrp2[2] = FIELD(u32, hn, 0x137C);
        limitTime = FIELD(s32, hn, 0x1390);
        seekData = FIELD(void*, hn, 0x2670);
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
        FIELD(s32, hn, 0x54) = 0;
        FIELD(s32, hn, 0x58) = 0;
        SFLIB_UnlockCs(&cs);

        {
            int tr = SFTRN_CallTrSetup(hn, 4);
            if (tr != 0) return tr;
        }

        MEM_Copy(tmpInfo, hn + 0xBAC, 0x190);

        {
            nu = (SfdPlyHn*)sfply_InitHn((SfdPlyCfg*)&cfgHdr, NULL);
            if (nu == NULL) {
                return SFLIB_SetErr(NULL, 0xFF000202);
            }

            MEM_Copy((u8*)nu + 0xA1C, tmpInfo, 0x190);
            MEM_Copy((u8*)nu + 0xBAC, tmpInfo, 0x190);
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
                void* ctx = lbl_eu_80606E34;
                if (ctx) {
                    *(void**)(lbl_eu_805683D0 + 0x0C) = nu;
                    void* vtbl = *(void**)ctx;
                    void (*trace)(void*, void*) = *(void (**)(void*, void*))((u8*)vtbl + 0x24);
                    trace(ctx, lbl_eu_805683D0 + 4);
                }
            }

            if (SFLIB_CheckHn(nu)) {
                SFLIB_SetErr(NULL, 0xFF00013D);
            } else {
                s32 termId = FIELD(s32, nu, 0x1FEC);
                if (SFBUF_GetTermFlg(nu, termId) != 1) {
                    SFBUF_SetTermFlg(nu, termId, 1);
                    nu->active = 1;
                }
            }

            /* trace-exit */
            {
                void* ctx = lbl_eu_80606E34;
                if (ctx) {
                    void* vtbl = *(void**)ctx;
                    void (*trace)(void*, void*) = *(void (**)(void*, void*))((u8*)vtbl + 0x24);
                    trace(ctx, lbl_eu_805683D0 + 0x6C);
                }
            }

            nu->field_0x64 = sav0x64;
            *(SfdBlk5C*)((u8*)nu + 0xD38) = blkD38;

            {
                u32 mbEn = FIELD(u32, nu, 0xD84);
                if (mbEn != 0) {
                    SFD_SetMbCb(nu, FIELD(void*, nu, 0xD8C), mbEn, FIELD(u32, nu, 0xD88));
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
                void (*fp)(void*) = (void (*)(void*))lbl_eu_80619BA8;
                if (fp != NULL) fp(cbGrp1);
            }
            if (cbGrp2[0] != 0) {
                FIELD(u32, nu, 0x1374) = cbGrp2[0];
                FIELD(u32, nu, 0x1378) = cbGrp2[1];
                FIELD(u32, nu, 0x137C) = cbGrp2[2];
                void (*fp)(void*) = (void (*)(void*))lbl_eu_80619BA8;
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
    int result = 0;
    void* ctx;
    void* vtbl;
    void (*trace)(void*, void*);

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
