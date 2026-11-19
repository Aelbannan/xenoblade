// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_tim
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "libs/CriWare/src/sofdec/sfdcore/sfd/sfd_types.h"

/* External functions */
extern s32 SFLIB_CheckHn(void* h);
extern s32 SFLIB_SetErr(s32 val, u32 err_code);
extern void SFLIB_LockCs(void* cs);
extern void SFLIB_UnlockCs(void* cs);
extern s32 SFSET_GetCond(void* h, u32 cond);
extern s32 UTY_MulDiv(s32 a, s32 b, s32 c);
extern s32 UTY_CmpTime(s32 a, s32 b, s32 c, s32 d);

/* External globals */
extern char lbl_eu_80606E38[];   /* SFD work area */
extern void* lbl_eu_80606E34;    /* callback table pointer */
extern u32 lbl_eu_8051CBF8[];    /* fps denominator table */
extern char lbl_eu_8051C888[];
extern char lbl_eu_8051C628[];
extern char lbl_eu_8051C89C[];
extern char lbl_eu_8051C8A4[];

/* Absorbed .bss 0x80619BB0-0x80619BC8 (0x18) */
__attribute__((aligned(8))) u32 lbl_eu_80619BB0[2];
u32 lbl_eu_80619BB8[1];
u32 lbl_eu_80619BBC[1];
__attribute__((aligned(8))) u32 lbl_eu_80619BC0[2];

/* Absorbed .data 0x80568EA0 (0x70) */
__declspec(section ".data") __attribute__((aligned(8))) const unsigned int lbl_eu_80568EA0[28] = {
    (unsigned int)lbl_eu_8051C888, 0, 3, 0, 0xB, (unsigned int)lbl_eu_8051C628, 0, 4,
    (unsigned int)lbl_eu_8051C89C, 0, 4, (unsigned int)lbl_eu_8051C8A4, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/* Absorbed .rodata 0x8051CC20 (0x78) - function table + float constants */
void sftim_Tc2Time23N(s32, void*, s32*, s32*, s32);
void sftim_Tc2Time23D(s32, void*, s32*, s32*, s32);
void sftim_Tc2TimeN(s32, void*, s32*, s32*, s32);
void sftim_Tc2Time29N(s32, void*, s32*, s32*, s32);
void sftim_Tc2Time29D(s32, void*, s32*, s32*, s32);
void sftim_Tc2Time59N(s32, void*, s32*, s32*, s32);
void sftim_Tc2Time59D(s32, void*, s32*, s32*, s32);
__declspec(section ".rodata") __attribute__((aligned(8))) const unsigned int lbl_eu_8051CC20[18] = {
    0, 0,
    (unsigned int)sftim_Tc2Time23N, (unsigned int)sftim_Tc2Time23D,
    (unsigned int)sftim_Tc2TimeN, (unsigned int)sftim_Tc2TimeN, (unsigned int)sftim_Tc2TimeN, (unsigned int)sftim_Tc2TimeN,
    (unsigned int)sftim_Tc2Time29N, (unsigned int)sftim_Tc2Time29D,
    (unsigned int)sftim_Tc2TimeN, (unsigned int)sftim_Tc2TimeN, (unsigned int)sftim_Tc2TimeN, (unsigned int)sftim_Tc2TimeN,
    (unsigned int)sftim_Tc2Time59N, (unsigned int)sftim_Tc2Time59D,
    (unsigned int)sftim_Tc2TimeN, (unsigned int)sftim_Tc2TimeN,
};

/* Forward declarations for function pointers */
s32 sftim_GetTimeNone(void* self, s32* out1, s32* out2);
s32 sftim_GetTimeVsync(void* self, s32* out1, s32* out2);
s32 sftim_GetTimeUfrm(void* self, s32* out1, s32* out2);
s32 sftim_GetTimeUtim(void* self, s32* out1, s32* out2);
s32 sftim_GetTimeExtClock(void* self, s32* out1, s32* out2);
void sftim_IsGrExecTime(void* self, s32 cmpA, s32 cmpB, s32 timeVal, s32 timeUnit, s32* out);
void SFTIM_IsExecTime(void* self, s32 cmpA, s32 cmpB, s32* out, s32 param7);

typedef void (*Tc2TimeFn)(s32 a, void* tcdata, s32* out1, s32* out2, s32 rate);

void SFTIM_Init(SfdTimCtx *sftim, int val) {
    sftim->field_0x00 = 0;
    sftim->field_0x04 = 0;
    sftim->field_0x08 = val;
    if (val % 10 == 0) {
        sftim->field_0x0c = val / 10;
        sftim->field_0x10 = 100;
    } else {
        sftim->field_0x0c = val;
        sftim->field_0x10 = 1000;
    }
}

void SFTIM_InitHn(SfdHandle* ctx, SfdTim* tim) {
    /* Function pointer table */
    ctx->tim.getTimeFn[0] = (void*)sftim_GetTimeNone;
    ctx->tim.getTimeFn[1] = (void*)sftim_GetTimeVsync;
    ctx->tim.getTimeFn[2] = NULL;
    ctx->tim.getTimeFn[3] = (void*)sftim_GetTimeUfrm;
    ctx->tim.getTimeFn[4] = (void*)sftim_GetTimeUtim;
    ctx->tim.getTimeFn[5] = (void*)sftim_GetTimeExtClock;

    /* Handle initialization - block 1 */
    tim->field_0x018 = 0;
    tim->tc[0] = 0;
    tim->tc[1] = 0;
    tim->tc[2] = 0;
    tim->tc[3] = 0;
    tim->tc[4] = 0;
    tim->tc[5] = 0;
    tim->tc[6] = 0;
    *(u16 *)&tim->tc[7] = 0;
    *((u16 *)&tim->tc[7] + 1) = 0;

    /* Block 2 */
    tim->tcC.flag = 0;
    tim->tcC.field_0x04 = 0;
    tim->tcC.hour = 0;
    tim->tcC.minute = 0;
    tim->tcC.second = 0;
    tim->tcC.frame = 0;
    tim->tcC.frame2 = 0;
    *(u32 *)&tim->tcC.field_0x1c = 0;
    *(u16 *)&tim->tcC.field_0x20 = 0;
    *((u16 *)&tim->tcC.field_0x20 + 1) = 0;
    tim->tcC.ttime = 0;
    tim->tcC.ftime = 1;

    /* Block 3 */
    tim->hdInfo.w[0] = 0;
    tim->hdInfo.w[1] = 0;
    tim->hdInfo.w[2] = 0;
    tim->hdInfo.w[3] = 0;
    tim->hdInfo.w[4] = 0;
    tim->hdInfo.w[5] = 0;
    tim->hdInfo.w[6] = 0;
    tim->hdInfo.w[7] = 0;
    *(u16 *)&tim->hdInfo.w[8] = 0;
    *((u16 *)&tim->hdInfo.w[8] + 1) = 0;
    tim->hdInfo.w[9] = 0x7FFFFFFF;
    tim->hdInfo.w[10] = 1;

    /* Block 4 */
    tim->tcA.flag = 0;
    tim->tcA.field_0x04 = 0;
    tim->tcA.hour = 0;
    tim->tcA.minute = 0;
    tim->tcA.second = 0;
    tim->tcA.frame = 0;
    tim->tcA.frame2 = 0;
    tim->tcA.ttime = -1;
    tim->tcA.ftime = 1;
    tim->tcB.ttime = -1;
    tim->tcB.ftime = 1;
    tim->tcD.ttime = -1;
    tim->tcD.ftime = 1;
    tim->limit = 0x7FFFFFFF;
    tim->limitUnit = 1;
    tim->streamPosLo = -1;
    tim->streamPosHi = -1;
    tim->posDeltaLo = -1;
    tim->posDeltaHi = -1;

    /* Block 5 */
    *(u32 *)&tim->tcA.field_0x1c = 0;
    *(u16 *)&tim->tcA.field_0x20 = 0;
    *((u16 *)&tim->tcA.field_0x20 + 1) = 0;
    tim->tcB.flag = 0;
    tim->tcB.field_0x04 = 0;
    tim->tcB.hour = 0;
    tim->tcB.minute = 0;
    tim->tcB.second = 0;
    tim->tcB.frame = 0;
    tim->tcB.frame2 = 0;
    *(u32 *)&tim->tcB.field_0x1c = 0;
    *(u16 *)&tim->tcB.field_0x20 = 0;
    *((u16 *)&tim->tcB.field_0x20 + 1) = 0;

    /* Block 6 */
    tim->tcD.flag = 0;
    tim->tcD.field_0x04 = 0;
    tim->tcD.hour = 0;
    tim->tcD.minute = 0;
    tim->tcD.second = 0;
    tim->tcD.frame = 0;
    tim->tcD.frame2 = 0;
    *(u32 *)&tim->tcD.field_0x1c = 0;
    *(u16 *)&tim->tcD.field_0x20 = 0;
    *((u16 *)&tim->tcD.field_0x20 + 1) = 0;

    /* Block 7 */
    tim->field_0x118[0] = 0;
    tim->field_0x118[1] = 0;
    tim->field_0x118[2] = 0;
    tim->field_0x118[3] = 0;
    tim->field_0x118[4] = 0;
    tim->field_0x118[5] = 0;
    tim->field_0x118[6] = 0;
    tim->field_0x118[7] = 0;
    *(u16 *)&tim->field_0x118[8] = 0;
    *((u16 *)&tim->field_0x118[8] + 1) = 0;

    /* Block 8 */
    tim->startTimeHi = 0;
    tim->startTimeLo = 0;

    /* Block 9 - large zero region */
    tim->field_0x160 = 0;
    tim->concat.time = 0;
    tim->concat.idx = 0;
    for (s32 i = 0; i < 32; ++i)
        tim->concat.timeQueue[i] = 0;
    tim->concat.value = 1;

    /* Block 10 */
    tim->concat.totalSmpl = 0;
    tim->concat.field_0x90 = 0;
    tim->concat.wr = 0;
    tim->concat.rd = 0;
    for (s32 i = 0; i < 32; ++i)
        tim->concat.smplQueue[i] = 0;

    /* Block 11 - timing state */
    tim->vsyncTime = 0;
    tim->field_0x2d4 = -1;
    {
        tim->field_0x2e8 = -5;
        tim->field_0x2ec = 0;
        tim->field_0x2f0 = 1;
        tim->curTime = -5;
        tim->curTimeUnit = 1;
        tim->maxTime = -5;
        tim->maxTimeUnit = 1;
        tim->time = -1;
        tim->timeUnit = 1;
        tim->lastItime = -5;
        tim->nextItime = 0x7FFFFFFF;
        tim->maxDelta = 0;
        tim->minDelta = 0x7FFFFFFF;
        tim->field_0x2a8 = 0;
        tim->speedNum = 1;
        tim->speedDen = 1;
        tim->field_0x2b8 = 0;
        tim->field_0x2bc = 0;
        tim->field_0x2c0 = 1;
        tim->field_0x2c4 = 100;
        tim->field_0x2c8 = -1;
        tim->field_0x2cc = 0;
        tim->field_0x2d0 = -1;
        tim->field_0x2d8 = tim->vsyncTime;
        tim->extClockFn = 0;
        tim->extClockWrap = -1;
        tim->extClockArg = 0;
    }

    /* Block 12 - tail */
    *(s32 *)&tim->ptsBlk0[0] = 0;
    *(s32 *)&tim->ptsBlk0[4] = 0;
    *(s32 *)&tim->ptsBlk0[8] = 0;
    *(s32 *)&tim->ptsBlk1[0] = 0;
    *(s32 *)&tim->ptsBlk1[4] = 0;
    *(s32 *)&tim->ptsBlk1[8] = 0;
    tim->field_0x5e8 = 0;
    tim->field_0x5ec = 0;
    tim->field_0x5f0 = 0;
    tim->field_0x5f4 = 0;
    tim->limitTime = -1;
}

void SFTIM_InitTtu(SfdTimTtu* ttu, int val) {
    int zero = 0;
    int one = 1;
    ttu->field_0x00 = zero;
    ttu->field_0x04 = zero;
    ttu->field_0x08 = zero;
    ttu->field_0x0c = zero;
    ttu->field_0x10 = zero;
    ttu->field_0x14 = zero;
    ttu->field_0x18 = zero;
    ttu->field_0x1c = zero;
    ttu->field_0x20 = 0;
    ttu->field_0x22 = 0;
    ttu->field_0x24 = val;
    ttu->field_0x28 = one;
}

void SFTIM_UpdateItime(SfdTim* tim, s32 itime) {
    s32 delta;
    s32 target;
    s32 newMin;
    s32 prevMin;
    s32 newMax;
    s32 prevMax;
    s32 diff;
    s32 lastItime;

    lastItime = tim->lastItime;
    if (lastItime == -5) {
        tim->lastItime = itime;
        return;
    }
    diff = itime - lastItime;
    if (diff == 0) {
        return;
    }
    prevMax = tim->maxDelta;
    newMax = diff;
    tim->lastItime = itime;
    if (prevMax > diff) {
        newMax = prevMax;
    }
    prevMin = tim->minDelta;
    newMin = diff;
    tim->maxDelta = newMax;
    if (prevMin < diff) {
        newMin = prevMin;
    }
    target = tim->nextItime;
    tim->minDelta = newMin;
    if (target == 0x7FFFFFFF) {
        tim->nextItime = diff;
        return;
    }
    if (target <= diff) {
        tim->nextItime = diff;
        return;
    }
    delta = (target - diff) / 8;
    if (delta != 0) {
        tim->nextItime = target - delta;
    } else {
        tim->nextItime = diff;
    }
}

int SFTIM_GetNextItime(SfdTim* tim, int time) {
    int a = tim->lastItime;
    int b = tim->nextItime;
    int c = tim->maxDelta;
    int sum1 = a + b;
    int sum2 = a + c;
    if (time < sum1) return sum1;
    if (time >= sum2) return 0x7FFFFFFF;
    return sum2;
}

void SFTIM_VbIn(void) {
    u8* wk;
    s32 delta;
    u32** ppHn;
    s32 i;
    void* h;
    u8* cb;
    s32 timeVal;
    s32 timeUnit;
    u32 csB;
    u32 csA;

    wk = (u8*)lbl_eu_80606E38;
    ppHn = (u32**)(wk + 0x1FC);
    *(u32*)(wk + 0x19C) = *(u32*)(wk + 0x19C) + 1;
    cb = (u8*)lbl_eu_80568EA0;

    for (i = 0; i < 8; i++, ppHn++) {
        h = (void*)*ppHn;
        if (SFLIB_CheckHn(h) == -1) {
            continue;
        }

        {
            u8* hp = (u8*)h;
            /* Delta advances even while paused/idle; gated below by state. */
            delta = (*(s32*)(hp + 0x1048) * *(s32*)(wk + 0x1AC)) / *(s32*)(hp + 0x104C);

            {
                s32 active;
                if (*(s32*)(hp + 0x54) != 4) {
                    active = 0;
                } else if (*(s32*)(hp + 0x5C) != 0) {
                    active = 0;
                } else {
                    active = (*(s32*)(hp + 0x980) == 0);
                }

                if (active) {
                    *(s32*)(hp + 0x1044) += delta;
                    if (*(s32*)(hp + 0x1384) != 0) {
                        SFLIB_LockCs(&csA);
                        if (*(s32*)(hp + 0x138C) != -1) {
                            *(s32*)(hp + 0x138C) += 1;
                            if (*(s32*)(hp + 0x138C) >= *(s32*)(hp + 0x1384)) {
                                /* Chained store keeps a zero temp live, as in retail */
                            *(s32*)(hp + 0x1388) = *(s32*)(hp + 0x138C) = 0;
                            }
                        }
                        SFLIB_UnlockCs(&csA);
                    }
                }

                {
                    s32 audioActive;
                    if (*(s32*)(hp + 0x106C) == -1) {
                        audioActive = 0;
                    } else {
                        audioActive = (*(s32*)(hp + 0x58) == 4);
                    }
                    if (audioActive) {
                        *(s32*)(hp + 0x106C) += delta;
                    }
                }
            }
        }

        if (SFSET_GetCond(h, 0x47) != 1) {
            continue;
        }

        {
            u8* hp = (u8*)h;
            s32 mode;
            s32 (*getTimeFn)(void*, s32*, s32*);

            SFLIB_LockCs(&csB);
            mode = SFSET_GetCond(h, 0x0F);
            getTimeFn = *(s32 (**)(void*, s32*, s32*))(hp + 0xD98 + mode * 4);
            if (getTimeFn == NULL) {
                getTimeFn = sftim_GetTimeNone;
            }
            getTimeFn(h, &timeVal, &timeUnit);
            SFLIB_UnlockCs(&csB);

            if (*(s32*)(hp + 0x1028) != timeVal || *(s32*)(hp + 0x102C) != timeUnit) {
                if (SFSET_GetCond(h, 0x47) == 1) {
                    *(s32*)(hp + 0x1070) = *(s32*)(hp + 0x1044);
                } else {
                    *(s32*)(hp + 0x1070) = *(s32*)(hp + 0x1084);
                }
                *(s32*)(hp + 0x1028) = timeVal;
                *(s32*)(hp + 0x102C) = timeUnit;

                if (lbl_eu_80606E34 != NULL) {
                    *(void**)(cb + 0x0C) = h;
                    *(void**)(cb + 0x18) = &timeVal;
                    *(void**)(cb + 0x24) = &timeUnit;
                    ((void (*)(void*, void*))(*(u32**)lbl_eu_80606E34)[9])(lbl_eu_80606E34, cb + 4);
                }
            }
            *(s32*)(hp + 0x50) = 1;
        }
    }
}

s32 SFTIM_IsStagnant(void* self) {
    u8* p = (u8*)self;
    s32 result;

    if (SFSET_GetCond(self, 6) == 0) {
        result = 0;
    } else {
        s32 cond33 = SFSET_GetCond(self, 0x33);
        if (cond33 == 0) {
            result = 0;
        } else {
            s32 num;
            s32 den;
            if (SFSET_GetCond(self, 0x47) == 1) {
                s32 base = *(s32*)(p + 0x1070);
                num = *(s32*)(p + 0x1044) - base;
                den = ((SfdSys *)lbl_eu_80606E38)->tim.field_0x0c;
            } else {
                num = *(s32*)(p + 0x1084) - *(s32*)(p + 0x1070);
                den = *(s32*)(p + 0x1088);
            }
            {
                s32 elapsed = num / den;
                result = (elapsed > cond33);
            }
        }
    }

    if (result != 0) {
        SFLIB_SetErr((s32)self, 0xFF000222);
        return 1;
    }
    return 0;
}

s32 SFTIM_GetAudioStartSample(SfdTim* tim, s32 sampleRate) {
    s64 startTime = *(s64 *)&tim->posDeltaHi;

    if (startTime < 0) {
        return -1;
    }

    {
        s32 result = (s32)(startTime * sampleRate / 90000);
        lbl_eu_80619BB0[0] = (u32)(startTime >> 32);
        lbl_eu_80619BB0[1] = (u32)startTime;
        *lbl_eu_80619BB8 = result;
        return result;
    }
}

s32 SFTIM_GetVideoStartSample(SfdTim* tim, s32 mul, s32* out2) {
    s32 val = tim->field_0x118[0];
    s32 num;
    s32 den;
    s32 r;

    *out2 = val;
    if (val != 0) {
        num = tim->limit;
        den = tim->limitUnit;
    } else {
        num = tim->tcD.ttime;
        if (num >= 0) {
            den = tim->tcD.ftime;
        } else {
            return -1;
        }
    }

    r = UTY_MulDiv(num, mul, den);
    lbl_eu_80619BBC[0] = num;
    lbl_eu_80619BC0[0] = r;
    return r;
}

void SFTIM_SetStartTime(SfdTim* tim, u32 a, u32 b) { tim->startTimeHi = a; tim->startTimeLo = b; }

s32 SFD_GetTime(void* self, s32* out1, s32* out2) {
    u8* p = (u8*)self;

    if (SFLIB_CheckHn(self) != 0) {
        return SFLIB_SetErr(0, 0xFF000121);
    }

    *out1 = *(s32*)(p + 0x1028);
    {
        s32 unit = *(s32*)(p + 0x102C);
        *out2 = unit;
        if (unit == 1) {
            return 0;
        }
        if (unit == *(s32*)(p + 0xEE0)) {
            *out1 += *(s32*)(p + 0xEDC);
        } else {
            u8* conv = p + 0xEB0;
            if (*(s32*)conv != 0) {
                *out1 += UTY_MulDiv(*(s32*)(conv + 0x24), unit, *(s32*)(conv + 0x28));
            }
        }
    }
    return 0;
}

/* Sample-rate conversion block (used when the requester unit is not the
 * native one): mode/numerator/denominator triple. */
typedef struct SftimConv {
    s32 mode;    /* 0x00 */
    u8 pad_0x04[0x20];
    s32 num;     /* 0x24 */
    s32 den;     /* 0x28 */
} SftimConv;

s32 SFTIM_GetTimeSub(void* self, s32* out1, s32* out2) {
    u8* p = (u8*)self;
    SftimConv* conv = (SftimConv*)(p + 0xEB0);

    *out1 = *(s32*)(p + 0x1028);
    {
        s32 unit = *(s32*)(p + 0x102C);
        *out2 = unit;
        if (unit == 1) {
            return 0;
        }
        if (unit == *(s32*)(p + 0xEE0)) {
            *out1 += *(s32*)(p + 0xEDC);
        } else {
            if (conv->mode != 0) {
                *out1 += UTY_MulDiv(conv->num, unit, conv->den);
            }
        }
    }
    return 0;
}

u32 SFTIM_GetTime(SfdHandle* h, u32* out1, u32* out2) {
    *out1 = h->tim.time;
    *out2 = h->tim.timeUnit;
    return 0;
}

s32 sftim_GetTimeNone(void *self, s32 *out1, s32 *out2) {
    int val = ((SfdHandle*)self)->errorState;
    int tmp;
    if (val == 4) goto good;
    if (val == -4) goto good;
    if (val == 6) goto good;
    if (val == -6) goto good;
    tmp = -1;
    *out1 = tmp;
    tmp = 1;
    *out2 = tmp;
    tmp = 0;
    goto check;
good:
    tmp = 1;
check:
    if (!tmp) return 0;
    tmp = -2;
    *out1 = tmp;
    tmp = 1;
    *out2 = tmp;
    return 0;
}

s32 sftim_GetTimeVsync(void* self, s32* out1, s32* out2) {
    s32 val = ((SfdHandle*)self)->errorState;
    s32 ok;

    if (val == 4) goto good;
    if (val == -4) goto good;
    if (val == 6) goto good;
    if (val == -6) goto good;
    *out1 = -1;
    *out2 = 1;
    ok = 0;
    goto check;
good:
    ok = 1;
check:
    if (!ok) {
        return 0;
    }
    *out1 = ((SfdHandle*)self)->tim.vsyncTime;
    *out2 = ((SfdSys *)lbl_eu_80606E38)->tim.field_0x0c;
    return 0;
}

s32 sftim_GetTimeUfrm(void* self, s32* out1, s32* out2) {
    s32 val = ((SfdHandle*)self)->errorState;
    if (!(val == 4 || val == -4 || val == 6 || val == -6)) {
        *out1 = -1;
        *out2 = 1;
    }
    return 0;
}

s32 sftim_GetTimeUtim(void* self, s32* out1, s32* out2) {
    s32 val = ((SfdHandle*)self)->errorState;
    s32 ok;

    if (val != 4 && val != -4 && val != 6 && val != -6) {
        *out1 = -1;
        *out2 = 1;
        ok = 0;
    } else {
        ok = 1;
    }

    if (!ok) {
        return 0;
    }

    {
        s32 (*fn)(void*, s32*, s32*) = (s32 (*)(void*, s32*, s32*))((SfdHandle*)self)->tim.usrTimeFn;
        if (fn == NULL) {
            *out1 = -2;
            *out2 = 1;
            return 0;
        }
        return fn(((SfdHandle*)self)->tim.usrTimeFnArg, out1, out2);
    }
}

s32 sftim_GetTimeExtClock(void* self, s32* out1, s32* out2) {
    u8* p = (u8*)self;
    s32 val = *(s32*)(p + 0x54);
    s32 ok = 0;

    if (val != 4 && val != -4 && val != 6 && val != -6) {
        *out1 = -1;
        *out2 = 1;
        ok = 0;
    } else {
        ok = 1;
    }

    if (!ok) {
        return 0;
    }

    {
        void (*fn)(void*, s32*, s32*) = *(void(**)(void*, s32*, s32*))(p + 0x107C);
        s32 clockVal, clockUnit;

        if (fn == NULL) {
            *out1 = -2;
            *out2 = 1;
            return 0;
        }

        fn(*(void**)(p + 0x1090), &clockVal, &clockUnit);

        {
            s32 playing;
            if (*(s32*)(p + 0x54) != 4) {
                playing = 0;
            } else if (*(s32*)(p + 0x5C) != 0) {
                playing = 0;
            } else {
                playing = (*(s32*)(p + 0x980) == 0);
            }

            if (playing) {
                if (*(s32*)(p + 0x1080) != -5) {
                    s32 diff = clockVal - *(volatile s32*)(p + 0x1080);
                    if (diff < 0) {
                        diff += *(s32*)(p + 0x108C) + 1;
                    }
                    *(s32*)(p + 0x1084) += diff;
                }
            }
        }

        /* Volatile to match retail: each access hits memory, no register CSE */
        *(volatile s32*)(p + 0x1080) = clockVal;
        *(volatile s32*)(p + 0x1088) = clockUnit;
        *out1 = *(volatile s32*)(p + 0x1084);
        *out2 = *(volatile s32*)(p + 0x1088);
    }
}

int SFTIM_ChkRegularTime(const void *tim, int *a2, int *a3) {
    int val = ((SfdHandle *)tim)->errorState;
    if (val != 4 && val != -4 && val != 6 && val != -6) {
        *a2 = -1;
        *a3 = 1;
        return 0;
    }
    return 1;
}

s32 SFD_SetUsrIsSkipFn(void* self, void* fn) {
    if (SFLIB_CheckHn(self) != 0) {
        return SFLIB_SetErr(0, 0xFF000124);
    }
    ((SfdHandle*)self)->tim.field_0x018 = fn;
    return 0;
}

s32 SFD_SetUsrTimeFn(void* self, void* fn, void* arg) {
    if (SFLIB_CheckHn(self) != 0) {
        return SFLIB_SetErr(0, 0xFF000123);
    }
    ((SfdHandle*)self)->tim.usrTimeFn = fn;
    ((SfdHandle*)self)->tim.usrTimeFnArg = arg;
    return 0;
}

s32 SFD_SetExtClockFn(void* self, void* fn, s32 wrap, void* arg) {
    if (SFLIB_CheckHn(self) != 0) {
        return SFLIB_SetErr(0, 0xFF000129);
    }
    if (fn != NULL) {
        ((SfdHandle*)self)->tim.extClockFn = fn;
        ((SfdHandle*)self)->tim.extClockWrap = wrap;
        ((SfdHandle*)self)->tim.extClockArg = arg;
    } else {
        ((SfdHandle*)self)->tim.extClockArg = arg;
        ((SfdHandle*)self)->tim.extClockWrap = wrap;
        ((SfdHandle*)self)->tim.extClockFn = NULL;
    }
    return 0;
}

void SFTIM_SetTimeFn(SfdHandle* h, void* fn, u32 idx) {
    h->tim.getTimeFn[idx] = fn;
}

void SFTIM_Tc2Time(void* tcdata, s32* out1, s32* out2) {
    s32 format = *(s32*)tcdata;
    Tc2TimeFn fn = *(Tc2TimeFn*)((u8*)lbl_eu_8051CC20 + format * 8 + *(s32*)((u8*)tcdata + 4) * 4);
    s32 rate;

    if (fn == NULL) {
        SFLIB_SetErr(0, 0xFF000221);
        *out1 = 0;
        *out2 = 1;
        return;
    }
    /* 23.976/29.97 timecode counts 4 fields per frame, the integer rates 10 */
    rate = 10;
    if (format == 1) {
        rate = 4;
    }
    fn(lbl_eu_8051CBF8[format], tcdata, out1, out2, rate);
}

/* Timecode block read by the Tc2Time converters: hours/minutes/seconds and the
 * frame counts are s32 words, the field counter is an s16 at the end. */
typedef struct SFTIM_TIMECODE {
    s32 unk00;    /* 0x00 */
    s32 unk04;    /* 0x04 */
    s32 hour;     /* 0x08 */
    s32 min;      /* 0x0C */
    s32 sec;      /* 0x10 */
    s32 frame;    /* 0x14 */
    s32 frame2;   /* 0x18 */
    u8 unk1C[2];  /* 0x1C */
    s16 field;    /* 0x1E */
} SFTIM_TIMECODE;

/* Timecode conversion helpers. Drop-frame variants add (min/10)*2 dropped
 * frames and use seconds-to-frames constants that account for the drop. */
void sftim_Tc2TimeN(s32 tc, void* tcdata, s32* out1, s32* out2, s32 rate) {
    SfdTc* tcd = (SfdTc*)tcdata;
    register s32 unit = 1000;
    SFTIM_TIMECODE* td = (SFTIM_TIMECODE*)tcdata;
    s32 fps;
    s32 min;
    s32 hour;
    s32 sec;
    s32 frame;
    s32 frame2;
    s16 field;

    unit /= rate;
    fps = tc / rate;
    min = td->min;
    hour = td->hour;
    sec = td->sec;
    frame = td->frame;
    frame2 = td->frame2;

    /* Division kept ahead of the frame sum so every field stays live at the
     * divw, matching retail's callee-saved allocation of the quotient. */
    s32 val;
    s32 frameSum;
    val = fps * (sec + min * 60 + hour * 3600);
    field = td->field;
    frameSum = frame + frame2;
    *out1 = val + frameSum * unit + field * (unit / 2);
    *out2 = fps;
}

void sftim_Tc2Time23N(s32 tc, void* tcdata, s32* out1, s32* out2, s32 rate) {
    SfdTc* tcd = (SfdTc*)tcdata;
    register s32 unit60 = 24000;
    register s32 unit = 1000;
    s32 fps;
    s32 min;
    s32 hour;
    s32 sec;
    s32 frame;
    s32 frame2;
    s16 field;
    s32 val;
    s32 frameSum;

    unit60 /= rate;
    unit /= rate;
    fps = tc / rate;
    min = tcd->minute;
    hour = tcd->hour;
    sec = tcd->second;
    frame = tcd->frame;
    frame2 = tcd->frame2;
    val = (sec + min * 60 + hour * 3600) * unit60;
    field = tcd->field;
    frameSum = frame + frame2;
    *out1 = val + frameSum * unit + field * (unit / 2);
    *out2 = fps;
}

void sftim_Tc2Time29N(s32 tc, void* tcdata, s32* out1, s32* out2, s32 rate) {
    SfdTc* tcd = (SfdTc*)tcdata;
    s32 unit60 = 30000;
    unit60 /= rate;
    s32 unit = 1000;
    unit /= rate;
    s32 min = tcd->minute;
    s32 hour = tcd->hour;
    s32 sec = tcd->second;
    s32 frame = tcd->frame;
    s32 frame2 = tcd->frame2;
    s32 field = tcd->field;
    s32 totalFrame = frame + frame2;
    s32 totalSec = sec + min * 60 + hour * 3600;
    s32 time = totalSec * unit60;
    time += totalFrame * unit;
    time += field * (unit / 2);
    *out1 = time;
    *out2 = tc / rate;
}

void sftim_Tc2Time59N(s32 tc, void* tcdata, s32* out1, s32* out2, s32 rate) {
    SfdTc* tcd = (SfdTc*)tcdata;
    s32 unit60 = 60000;
    unit60 /= rate;
    s32 unit = 1000;
    unit /= rate;
    s32 min = tcd->minute;
    s32 hour = tcd->hour;
    s32 sec = tcd->second;
    s32 frame = tcd->frame;
    s32 frame2 = tcd->frame2;
    *out1 = (sec + min * 60 + hour * 3600) * unit60 + (frame + frame2) * unit + tcd->field * (unit / 2);
    *out2 = tc / rate;
}

void sftim_Tc2Time23D(s32 tc, void* tcdata, s32* out1, s32* out2, s32 rate) {
    SfdTc* tcd = (SfdTc*)tcdata;
    s32 unit;
    s32 min;
    s32 drop;
    s32 frame;
    s32 frame2;
    s32 totalFrame;
    s32 hour;
    s32 sec;
    s32 field;

    unit = 1000;
    unit /= rate;
    min = tcd->minute;
    drop = min / 10;
    frame = tcd->frame;
    frame2 = tcd->frame2;
    totalFrame = frame + frame2;
    hour = tcd->hour;
    sec = tcd->second;
    field = tcd->field;
    /* 23.976fps drop-frame: each minute drops 2 ticks, compensating
     * the 1000/1001 ratio; per-unit constants absorb the same skew. */
    *out1 = (drop * 2 + hour * 86292 + (sec * 24 + min * 1438) + totalFrame) * unit
        + field * (unit / 2);
    *out2 = tc / rate;
}

/* SRTV timecode packet laid out as the decoder fills it. */
typedef struct SftimTimecode {
    u8 pad_0x00[0x08];
    s32 hour;   /* 0x08 */
    s32 min;    /* 0x0C */
    s32 sec;    /* 0x10 */
    s32 frame;  /* 0x14 */
    s32 frame2; /* 0x18 */
    u8 pad_0x1C[0x02];
    s16 field;  /* 0x1E */
} SftimTimecode;

void sftim_Tc2Time29D(s32 tc, void* tcdata, s32* out1, s32* out2, s32 rate) {
    SfdTc* tcd = (SfdTc*)tcdata;
    SftimTimecode* td = (SftimTimecode*)tcdata;
    register s32 unit = 1000;
    s32 min;
    s32 sec;
    s32 frame;
    s32 frame2;
    s32 hour;
    s32 drop;
    s32 half;
    s32 totalFrame;
    s16 field;

    /* Drop-frame: every 10th minute drops 2 ticks (1798 vs 1800 per minute,
     * 107892 vs 108000 per hour); half-unit compensates the field tick. */
    unit /= rate;
    min = td->min;
    sec = td->sec;
    frame = td->frame;
    frame2 = td->frame2;
    hour = td->hour;
    totalFrame = frame + frame2;
    drop = min / 10;
    field = td->field;

    s32 time = (drop * 2 + hour * 107892 + sec * 30 + min * 1798 + totalFrame) * unit;
    time += field * (unit / 2);
    *out1 = time;
    *out2 = tc / rate;
}

void sftim_Tc2Time59D(s32 tc, void* tcdata, s32* out1, s32* out2, s32 rate) {
    SfdTc* tcd = (SfdTc*)tcdata;
    register s32 unit = 1000;
    SftimTimecode* td = (SftimTimecode*)tcdata;
    s32 fps;
    s32 min;
    s32 drop;
    s32 frame;
    s32 totalFrame;

    unit /= rate;
    fps = tc / rate;
    min = td->min;
    drop = min / 10;
    frame = td->frame;
    totalFrame = frame + td->frame2;

    /* Drop-frame TC -> total ms: every 10th minute drops 2 ticks (3598 vs 3600
     * per minute, 215892 vs 216000 per hour); half-unit covers the field tick. */
    *out1 = (totalFrame + ((drop * 2 + td->hour * 215892) + (td->sec * 60 + min * 3598))) * unit
        + td->field * (unit / 2);
    *out2 = fps;
}

void SFTIM_Pause(void* self, s32 mode) {
    u8* p = (u8*)self;

    if (mode == 2) {
        s32 num, den;
        s32 fps = ((SfdHandle*)p)->field_0x0930;
        u32 cs1;
        u32 cs2;

        if (fps == 0) {
            num = 0;
            den = 0x7512;
        } else {
            num = 1000;
            den = lbl_eu_8051CBF8[fps];
        }

        {
            s32 delta = UTY_MulDiv(((SfdSys *)lbl_eu_80606E38)->tim.field_0x0c, num, den);
            s32 t;
            SFLIB_LockCs(&cs1);
            t = ((SfdHandle*)p)->tim.vsyncTime;
            t += delta;
            ((SfdHandle*)p)->tim.vsyncTime = t;
            t = ((SfdHandle*)p)->tim.field_0x2d4;
            t += delta;
            ((SfdHandle*)p)->tim.field_0x2d4 = t;
            SFLIB_UnlockCs(&cs1);
        }

        {
            s32 delta2 = UTY_MulDiv(((SfdHandle*)p)->tim.field_0x2f0, num, den);
            s32 t2;
            SFLIB_LockCs(&cs2);
            t2 = ((SfdHandle*)p)->tim.field_0x2ec;
            t2 += delta2;
            ((SfdHandle*)p)->tim.field_0x2ec = t2;
            SFLIB_UnlockCs(&cs2);
        }
    }
}

void SFTIM_GetTimeOneFrmVideo(void* self, int* out1, int* out2) {
    int fps = ((SfdHandle*)self)->field_0x0930;
    if (fps == 0) {
        *out1 = 0;
        *out2 = 0x7512;
    } else {
        *out1 = 1000;
        *out2 = lbl_eu_8051CBF8[fps];
    }
}

s32 SFD_GetFps(void* self, s32* out) {
    SfdHandle* h = (SfdHandle*)self;

    *out = -1;
    if (SFLIB_CheckHn(self) != 0) {
        return SFLIB_SetErr(0, 0xFF00011B);
    }

    {
        s32 fps = h->field_0x0930;
        if (fps == 0) {
            return 0;
        }
        *out = lbl_eu_8051CBF8[fps];
        if (h->fhd.info.active != 0) {
            s32 ovrFps = h->fhd.info.video[4];
            if (ovrFps != -1) {
                *out = ovrFps;
            }
        }
    }
    return 0;
}

s32 SFTIM_IsGetFrmTime(void* self, void* frm) {
    s32 cmpB;
    s32 cmpA;
    s32 result;

    if (frm == NULL) {
        return 0;
    }

    cmpB = ((SfdFrame*)frm)->ttu.field_0x00;
    cmpA = ((SfdFrame*)frm)->field_0x14;

    if (((SfdHandle*)self)->cond[14] != 0) {
        return 1;
    }

    SFTIM_IsExecTime(self, cmpA, cmpB, &result, ((SfdHandle*)self)->cond[44]);
    return result;
}

s32 SFTIM_IsGetFrmTimeTunit(void* self, s32 cmpA, s32 cmpB) {
    if (((SfdHandle*)self)->cond[14] != 0) {
        return 1;
    }
    {
        s32 result;
        SFTIM_IsExecTime(self, cmpA, cmpB, &result, ((SfdHandle*)self)->cond[44]);
        return result;
    }
}

void SFTIM_IsExecTime(void* self, s32 cmpA, s32 cmpB, s32* out, s32 param7) {
    u8* p = (u8*)self;
    u8* wk = (u8*)lbl_eu_80606E38;
    s32 mode = ((SfdHandle*)p)->cond[15];

    if (mode == 0) {
        *out = 1;
        return;
    }

    {
        s32 timeVal = ((SfdHandle*)p)->tim.time;
        s32 timeUnit = ((SfdHandle*)p)->tim.timeUnit;

        if (timeUnit == 1) {
            if (((SfdHandle*)p)->tim.field_0x2d4 < 0) {
                ((SfdHandle*)p)->tim.field_0x2d4 = 0;
                *out = 1;
                return;
            }
            {
                s32 audioTime = ((SfdHandle*)p)->tim.field_0x2d4;
                if (UTY_CmpTime(cmpA, cmpB, audioTime, ((SfdSys *)wk)->tim.field_0x0c) != 0) {
                    *out = 1;
                } else {
                    *out = 0;
                }
            }
        } else {
            s32 offset = (timeUnit * param7) / *(s32*)(wk + 0x1A4);
            timeVal += offset;
            if (mode != 1) {
                sftim_IsGrExecTime(self, cmpA, cmpB, timeVal, timeUnit, out);
            } else {
                if (UTY_CmpTime(cmpA, cmpB, timeVal, timeUnit) != 0) {
                    *out = 1;
                } else {
                    *out = 0;
                }
            }
        }
    }
}

void sftim_IsGrExecTime(void* self, s32 cmpA, s32 cmpB, s32 timeVal, s32 timeUnit, s32* out) {
    u8* p = (u8*)self;
    s32 grParam = *(s32*)(p + 0xAD4);
    s32 grOffset = (grParam * timeUnit) / 10000;

    if (UTY_CmpTime(cmpA, cmpB, timeVal + grOffset, timeUnit) == 0) {
        *out = 0;
        return;
    }

    if (UTY_CmpTime(cmpA, cmpB, timeVal - grOffset, timeUnit) != 0) {
        *out = 1;
        if (*(s32*)(p + 0x1068) != cmpA && *(s32*)(p + 0x1060) != cmpA) {
            *(s32*)(p + 0x1060) = cmpA;
            *(s32*)(p + 0x105C) = *(s32*)(p + 0x105C) + 1;
        }
        return;
    }

    {
        s32 playing;
        if (*(s32*)(p + 0x54) != 4) {
            playing = 0;
        } else if (*(s32*)(p + 0x5C) != 0) {
            playing = 0;
        } else {
            playing = (*(s32*)(p + 0x980) == 0);
        }

        if (!playing) {
            if (UTY_CmpTime(cmpA, cmpB, timeVal + grOffset / 2, timeUnit) != 0) {
                *out = 1;
                *(s32*)(p + 0x1060) = cmpA;
                *(s32*)(p + 0x105C) = *(s32*)(p + 0x105C) + 1;
            } else {
                *out = 0;
            }
            return;
        }

        {
            s32 counter = *(s32*)(p + 0x105C);
            s32 threshold = 0;

            if ((u32)*(s32*)(lbl_eu_80606E38 + 0x1A4) == 60004u) {
                if (*(s32*)(p + 0x930) <= 2) {
                    if (*(s32*)(p + 0x1048) == *(s32*)(p + 0x104C)) {
                        threshold = 1;
                    }
                }
            }

            if (counter <= threshold) {
                *out = *(s32*)(p + 0x1064);
            } else {
                *out = (UTY_CmpTime(cmpA, cmpB, timeVal, timeUnit) != 0);
            }

            *(s32*)(p + 0x105C) = 0;
            *(s32*)(p + 0x1064) = *out;
            *(s32*)(p + 0x1068) = cmpA;
        }
    }
}

s32 SFTIM_IsVideoTerm(void* self) {
    u8* p = (u8*)self;
    s32 cond = *(s32*)(p + 0x960);
    s32 v1018 = *(s32*)(p + 0x1018);
    s32 v101C = *(s32*)(p + 0x101C);

    if (cond == 0) {
        return 1;
    }

    if (v1018 == -5) {
        return 0;
    }

    return (UTY_CmpTime(v1018 + (v101C * 2000) / 59940,
                        v101C,
                        *(s32*)(p + 0x1028),
                        *(s32*)(p + 0x102C)) != 0);
}

void SFTIM_SetSpeed(SfdHandle* h, u32 a, u32 b) { h->tim.speedNum = a; h->tim.speedDen = b; }

void SFTIM_GetSpeed(void* self, u32* out1, u32* out2) {
    *out1 = ((SfdHandle*)self)->tim.speedNum;
    *out2 = ((SfdHandle*)self)->tim.speedDen;
}

s32 SFD_SetCyclicFrameOutput(void* self, s32 num, s32 den) {
    if (SFLIB_CheckHn(self) != 0) {
        return SFLIB_SetErr(0, 0xFF00012B);
    }
    {
        char cs[8];
        SFLIB_LockCs(cs);
        ((SfdHandle*)self)->tim.field_0x5e8 = num;
        ((SfdHandle*)self)->tim.field_0x5ec = den;
        if (den != 0) {
            ((SfdHandle*)self)->tim.field_0x5f4 = -1;
        }
        SFLIB_UnlockCs(cs);
    }
    return 0;
}

s32 SFTIM_ExecCyclicFrameOutput(void* self) {
    u8* p = (u8*)self;
    s32 result;

    if (*(s32*)(p + 0x1384) != 0) {
        char cs[8];
        SFLIB_LockCs(cs);
        if (*(s32*)(p + 0x138C) == -1) {
            *(s32*)(p + 0x138C) = 0;
            *(s32*)(p + 0x1388) = 0;
        }
        {
            s32 counter = *(s32*)(p + 0x1388);
            s32 den = *(s32*)(p + 0x1384);
            s32 idx = *(s32*)(p + 0x138C);
            s32 num = *(s32*)(p + 0x1380);
            if (counter * den <= idx * num) {
                *(s32*)(p + 0x1388) = counter + 1;
                result = 1;
            } else {
                result = 0;
            }
        }
        SFLIB_UnlockCs(cs);
    } else {
        result = 1;
    }
    return result;
}

void SFD_CalcCycleFromFps(s32 fps, s32* out1, s32* out2) {
    /* The playback cycle is picked from the ratio of the vsync interval to the
     * frame duration; MWCC pools the ten thresholds and the 0x4330000080000000
     * s32->f32 magic double right after the tc2time jump table in .rodata. */
    f32 ratio = (f32)*(s32*)(lbl_eu_80606E38 + 0x1A4) / (f32)fps;

    if (ratio < 0.4f) {
        *out1 = (s32)(0.5f + 1.0f / ratio);
        *out2 = 1;
    } else if (ratio < 0.75f) {
        *out1 = 2;
        *out2 = 1;
    } else if (ratio < 1.1f) {
        *out1 = 1;
        *out2 = 1;
    } else if (ratio < 1.5f) {
        *out1 = 5;
        *out2 = 6;
    } else if (ratio < 2.2f) {
        *out1 = 1;
        *out2 = 2;
    } else if (ratio < 2.45f) {
        *out1 = 5;
        *out2 = 12;
    } else if (ratio < 2.75f) {
        *out1 = 2;
        *out2 = 5;
    } else if (ratio < 3.5f) {
        *out1 = 1;
        *out2 = 3;
    } else {
        *out1 = 1;
        *out2 = (s32)(0.5f + ratio);
    }
}

void SFD_SetLimitTime(SfdHandle* h, u32 val) { h->tim.limitTime = val; }

void SFD_GetLimitTime(void* self, s32* out) {
    s32 cond = SFSET_GetCond(self, 0x36);
    s32 limitTime = ((SfdHandle*)self)->tim.limitTime;

    if (limitTime == -1) {
        *out = cond;
    } else if (cond == -1) {
        *out = limitTime;
    } else {
        *out = (cond < limitTime) ? cond : limitTime;
    }
}