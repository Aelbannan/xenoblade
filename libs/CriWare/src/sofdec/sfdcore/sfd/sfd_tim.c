// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_tim
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

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
extern char lbl_eu_80568EA0[];   /* callback struct */
extern u32 lbl_eu_8051CBF8[];    /* fps denominator table */
extern u32 lbl_eu_8051CC20[];    /* tc2time function table */
extern u32 lbl_eu_80619BB0[];    /* debug globals */
extern u32 lbl_eu_80619BB8[];    /* debug globals */

/* Forward declarations for function pointers */
s32 sftim_GetTimeNone(void* self, s32* out1, s32* out2);
s32 sftim_GetTimeVsync(void* self, s32* out1, s32* out2);
s32 sftim_GetTimeUfrm(void* self, s32* out1, s32* out2);
s32 sftim_GetTimeUtim(void* self, s32* out1, s32* out2);
s32 sftim_GetTimeExtClock(void* self, s32* out1, s32* out2);
void sftim_IsGrExecTime(void* self, s32 cmpA, s32 cmpB, s32 timeVal, s32 timeUnit, s32* out);
void SFTIM_IsExecTime(void* self, s32 cmpA, s32 cmpB, s32* out, s32 param7);

typedef void (*Tc2TimeFn)(s32 a, void* tcdata, s32* out1, s32* out2, s32 rate);

typedef struct SFTIM {
    int field_0;
    int field_4;
    int field_8;
    int field_C;
    int field_10;
} SFTIM;

void SFTIM_Init(SFTIM *sftim, int val) {
    sftim->field_0 = 0;
    sftim->field_4 = 0;
    sftim->field_8 = val;
    if (val % 10 == 0) {
        sftim->field_C = val / 10;
        sftim->field_10 = 100;
    } else {
        sftim->field_C = val;
        sftim->field_10 = 1000;
    }
}

void SFTIM_InitHn(void* ctx, void* h) {
    u8* c = (u8*)ctx;
    u8* p = (u8*)h;

    /* Function pointer table */
    *(void**)(c + 0xD98) = (void*)sftim_GetTimeNone;
    *(void**)(c + 0xD9C) = (void*)sftim_GetTimeVsync;
    *(void**)(c + 0xDA0) = NULL;
    *(void**)(c + 0xDA4) = (void*)sftim_GetTimeUfrm;
    *(void**)(c + 0xDA8) = (void*)sftim_GetTimeUtim;
    *(void**)(c + 0xDAC) = (void*)sftim_GetTimeExtClock;

    /* Handle initialization - block 1 */
    *(u32*)(p + 0x18) = 0;
    *(u32*)(p + 0x1C) = 0;
    *(u32*)(p + 0x20) = 0;
    *(u32*)(p + 0x24) = 0;
    *(u32*)(p + 0x28) = 0;
    *(u32*)(p + 0x2C) = 0;
    *(u32*)(p + 0x30) = 0;
    *(u32*)(p + 0x34) = 0;
    *(u16*)(p + 0x38) = 0;
    *(u16*)(p + 0x3A) = 0;

    /* Block 2 */
    *(u32*)(p + 0xC0) = 0;
    *(u32*)(p + 0xC4) = 0;
    *(u32*)(p + 0xC8) = 0;
    *(u32*)(p + 0xCC) = 0;
    *(u32*)(p + 0xD0) = 0;
    *(u32*)(p + 0xD4) = 0;
    *(u32*)(p + 0xD8) = 0;
    *(u32*)(p + 0xDC) = 0;
    *(u16*)(p + 0xE0) = 0;
    *(u16*)(p + 0xE2) = 0;
    *(u32*)(p + 0xE4) = 0;
    *(u32*)(p + 0xE8) = 1;

    /* Block 3 */
    *(u32*)(p + 0x3C) = 0;
    *(u32*)(p + 0x40) = 0;
    *(u32*)(p + 0x44) = 0;
    *(u32*)(p + 0x48) = 0;
    *(u32*)(p + 0x4C) = 0;
    *(u32*)(p + 0x50) = 0;
    *(u32*)(p + 0x54) = 0;
    *(u32*)(p + 0x58) = 0;
    *(u16*)(p + 0x5C) = 0;
    *(u16*)(p + 0x5E) = 0;
    *(u32*)(p + 0x60) = 0x7FFFFFFF;
    *(u32*)(p + 0x64) = 1;

    /* Block 4 */
    *(u32*)(p + 0x68) = 0;
    *(u32*)(p + 0x6C) = 0;
    *(u32*)(p + 0x70) = 0;
    *(u32*)(p + 0x74) = 0;
    *(u32*)(p + 0x78) = 0;
    *(u32*)(p + 0x7C) = 0;
    *(u32*)(p + 0x80) = 0;
    *(s32*)(p + 0x8C) = -1;
    *(u32*)(p + 0x90) = 1;
    *(s32*)(p + 0xB8) = -1;
    *(u32*)(p + 0xBC) = 1;
    *(s32*)(p + 0x110) = -1;
    *(u32*)(p + 0x114) = 1;
    *(u32*)(p + 0x13C) = 0x7FFFFFFF;
    *(u32*)(p + 0x140) = 1;
    *(s32*)(p + 0x154) = -1;
    *(s32*)(p + 0x150) = -1;
    *(s32*)(p + 0x15C) = -1;
    *(s32*)(p + 0x158) = -1;

    /* Block 5 */
    *(u32*)(p + 0x84) = 0;
    *(u16*)(p + 0x88) = 0;
    *(u16*)(p + 0x8A) = 0;
    *(u32*)(p + 0x94) = 0;
    *(u32*)(p + 0x98) = 0;
    *(u32*)(p + 0x9C) = 0;
    *(u32*)(p + 0xA0) = 0;
    *(u32*)(p + 0xA4) = 0;
    *(u32*)(p + 0xA8) = 0;
    *(u32*)(p + 0xAC) = 0;
    *(u32*)(p + 0xB0) = 0;
    *(u16*)(p + 0xB4) = 0;
    *(u16*)(p + 0xB6) = 0;

    /* Block 6 */
    *(u32*)(p + 0xEC) = 0;
    *(u32*)(p + 0xF0) = 0;
    *(u32*)(p + 0xF4) = 0;
    *(u32*)(p + 0xF8) = 0;
    *(u32*)(p + 0xFC) = 0;
    *(u32*)(p + 0x100) = 0;
    *(u32*)(p + 0x104) = 0;
    *(u32*)(p + 0x108) = 0;
    *(u16*)(p + 0x10C) = 0;
    *(u16*)(p + 0x10E) = 0;

    /* Block 7 */
    *(u32*)(p + 0x118) = 0;
    *(u32*)(p + 0x11C) = 0;
    *(u32*)(p + 0x120) = 0;
    *(u32*)(p + 0x124) = 0;
    *(u32*)(p + 0x128) = 0;
    *(u32*)(p + 0x12C) = 0;
    *(u32*)(p + 0x130) = 0;
    *(u32*)(p + 0x134) = 0;
    *(u16*)(p + 0x138) = 0;
    *(u16*)(p + 0x13A) = 0;

    /* Block 8 */
    *(u32*)(p + 0x144) = 0;
    *(u32*)(p + 0x148) = 0;

    /* Block 9 - large zero region */
    *(u32*)(p + 0x160) = 0;
    *(u32*)(p + 0x164) = 0;
    *(u32*)(p + 0x168) = 0;
    *(u32*)(p + 0x16C) = 0;
    *(u32*)(p + 0x170) = 0;
    *(u32*)(p + 0x174) = 0;
    *(u32*)(p + 0x178) = 0;
    *(u32*)(p + 0x17C) = 0;
    *(u32*)(p + 0x180) = 0;
    *(u32*)(p + 0x184) = 0;
    *(u32*)(p + 0x188) = 0;
    *(u32*)(p + 0x18C) = 0;
    *(u32*)(p + 0x190) = 0;
    *(u32*)(p + 0x194) = 0;
    *(u32*)(p + 0x198) = 0;
    *(u32*)(p + 0x19C) = 0;
    *(u32*)(p + 0x1A0) = 0;
    *(u32*)(p + 0x1A4) = 0;
    *(u32*)(p + 0x1A8) = 0;
    *(u32*)(p + 0x1AC) = 0;
    *(u32*)(p + 0x1B0) = 0;
    *(u32*)(p + 0x1B4) = 0;
    *(u32*)(p + 0x1B8) = 0;
    *(u32*)(p + 0x1BC) = 0;
    *(u32*)(p + 0x1C0) = 0;
    *(u32*)(p + 0x1C4) = 0;
    *(u32*)(p + 0x1C8) = 0;
    *(u32*)(p + 0x1CC) = 0;
    *(u32*)(p + 0x1D0) = 0;
    *(u32*)(p + 0x1D4) = 0;
    *(u32*)(p + 0x1D8) = 0;
    *(u32*)(p + 0x1DC) = 0;
    *(u32*)(p + 0x1E0) = 0;
    *(u32*)(p + 0x1E4) = 0;
    *(u32*)(p + 0x1E8) = 0;
    *(u32*)(p + 0x1EC) = 1;

    /* Block 10 */
    *(u32*)(p + 0x1F0) = 0;
    *(u32*)(p + 0x1F4) = 0;
    *(u32*)(p + 0x1F8) = 0;
    *(u32*)(p + 0x1FC) = 0;
    *(u32*)(p + 0x200) = 0;
    *(u32*)(p + 0x204) = 0;
    *(u32*)(p + 0x208) = 0;
    *(u32*)(p + 0x20C) = 0;
    *(u32*)(p + 0x210) = 0;
    *(u32*)(p + 0x214) = 0;
    *(u32*)(p + 0x218) = 0;
    *(u32*)(p + 0x21C) = 0;
    *(u32*)(p + 0x220) = 0;
    *(u32*)(p + 0x224) = 0;
    *(u32*)(p + 0x228) = 0;
    *(u32*)(p + 0x22C) = 0;
    *(u32*)(p + 0x230) = 0;
    *(u32*)(p + 0x234) = 0;
    *(u32*)(p + 0x238) = 0;
    *(u32*)(p + 0x23C) = 0;
    *(u32*)(p + 0x240) = 0;
    *(u32*)(p + 0x244) = 0;
    *(u32*)(p + 0x248) = 0;
    *(u32*)(p + 0x24C) = 0;
    *(u32*)(p + 0x250) = 0;
    *(u32*)(p + 0x254) = 0;
    *(u32*)(p + 0x258) = 0;
    *(u32*)(p + 0x25C) = 0;
    *(u32*)(p + 0x260) = 0;
    *(u32*)(p + 0x264) = 0;
    *(u32*)(p + 0x268) = 0;
    *(u32*)(p + 0x26C) = 0;
    *(u32*)(p + 0x270) = 0;
    *(u32*)(p + 0x274) = 0;
    *(u32*)(p + 0x278) = 0;
    *(u32*)(p + 0x27C) = 0;

    /* Block 11 - timing state */
    *(u32*)(p + 0x2AC) = 0;
    *(s32*)(p + 0x2D4) = -1;
    {
        u32 tmp = *(u32*)(p + 0x2AC);
        *(s32*)(p + 0x2E8) = -5;
        *(u32*)(p + 0x2EC) = 0;
        *(u32*)(p + 0x2F0) = 1;
        *(s32*)(p + 0x280) = -5;
        *(u32*)(p + 0x284) = 1;
        *(s32*)(p + 0x288) = -5;
        *(u32*)(p + 0x28C) = 1;
        *(s32*)(p + 0x290) = -1;
        *(u32*)(p + 0x294) = 1;
        *(s32*)(p + 0x298) = -5;
        *(u32*)(p + 0x29C) = 0x7FFFFFFF;
        *(u32*)(p + 0x2A0) = 0;
        *(u32*)(p + 0x2A4) = 0x7FFFFFFF;
        *(u32*)(p + 0x2A8) = 0;
        *(u32*)(p + 0x2B0) = 1;
        *(u32*)(p + 0x2B4) = 1;
        *(u32*)(p + 0x2B8) = 0;
        *(u32*)(p + 0x2BC) = 0;
        *(u32*)(p + 0x2C0) = 1;
        *(u32*)(p + 0x2C4) = 100;
        *(s32*)(p + 0x2C8) = -1;
        *(u32*)(p + 0x2CC) = 0;
        *(s32*)(p + 0x2D0) = -1;
        *(u32*)(p + 0x2D8) = tmp;
        *(u32*)(p + 0x2E4) = 0;
        *(s32*)(p + 0x2F4) = -1;
        *(u32*)(p + 0x2F8) = 0;
    }

    /* Block 12 - tail */
    *(u32*)(p + 0x5D0) = 0;
    *(u32*)(p + 0x5D4) = 0;
    *(u32*)(p + 0x5D8) = 0;
    *(u32*)(p + 0x5DC) = 0;
    *(u32*)(p + 0x5E0) = 0;
    *(u32*)(p + 0x5E4) = 0;
    *(u32*)(p + 0x5E8) = 0;
    *(u32*)(p + 0x5EC) = 0;
    *(u32*)(p + 0x5F0) = 0;
    *(u32*)(p + 0x5F4) = 0;
    *(s32*)(p + 0x5F8) = -1;
}

void SFTIM_InitTtu(void* self, int val) {
    int zero = 0;
    int one = 1;
    *(int*)((u8*)self + 0x00) = zero;
    *(int*)((u8*)self + 0x04) = zero;
    *(int*)((u8*)self + 0x08) = zero;
    *(int*)((u8*)self + 0x0C) = zero;
    *(int*)((u8*)self + 0x10) = zero;
    *(int*)((u8*)self + 0x14) = zero;
    *(int*)((u8*)self + 0x18) = zero;
    *(int*)((u8*)self + 0x1C) = zero;
    *(u16*)((u8*)self + 0x20) = 0;
    *(u16*)((u8*)self + 0x22) = 0;
    *(int*)((u8*)self + 0x24) = val;
    *(int*)((u8*)self + 0x28) = one;
}

void SFTIM_UpdateItime(void* self, s32 itime) {
    u8* p = (u8*)self;
    s32 lastItime = *(s32*)(p + 0x298);

    if (lastItime == -5) {
        *(s32*)(p + 0x298) = itime;
        return;
    }

    s32 diff = itime - lastItime;
    if (diff == 0) {
        return;
    }

    s32 prevMax = *(s32*)(p + 0x2A0);
    s32 newMax = diff;
    *(s32*)(p + 0x298) = itime;
    if (prevMax > diff) {
        newMax = prevMax;
    }

    s32 prevMin = *(s32*)(p + 0x2A4);
    s32 newMin = diff;
    *(s32*)(p + 0x2A0) = newMax;
    if (prevMin < diff) {
        newMin = prevMin;
    }

    s32 target = *(s32*)(p + 0x29C);
    *(s32*)(p + 0x2A4) = newMin;

    if (target == 0x7FFFFFFF) {
        *(s32*)(p + 0x29C) = diff;
        return;
    }

    if (target <= diff) {
        *(s32*)(p + 0x29C) = diff;
        return;
    }

    {
        s32 delta = (target - diff) / 8;
        if (delta != 0) {
            *(s32*)(p + 0x29C) = target - delta;
        } else {
            *(s32*)(p + 0x29C) = diff;
        }
    }
}

int SFTIM_GetNextItime(void* self, int time) {
    int a = *(int*)((u8*)self + 0x298);
    int b = *(int*)((u8*)self + 0x29c);
    int c = *(int*)((u8*)self + 0x2a0);
    int sum1 = a + b;
    int sum2 = a + c;
    if (time < sum1) return sum1;
    if (time >= sum2) return 0x7FFFFFFF;
    return sum2;
}

void SFTIM_VbIn(void) {
    u8* wk = (u8*)lbl_eu_80606E38;
    u8* cb = (u8*)lbl_eu_80568EA0;
    u32** handles = (u32**)(wk + 0x1FC);
    s32 i;
    s32 timeVal, timeUnit;

    *(u32*)(wk + 0x19C) = *(u32*)(wk + 0x19C) + 1;

    for (i = 0; i < 8; i++) {
        void* h = handles[i];
        if (SFLIB_CheckHn(h) == -1) {
            continue;
        }

        {
            u8* hp = (u8*)h;
            s32 speedNum = *(s32*)(hp + 0x1048);
            s32 speedMul = *(s32*)(wk + 0x1AC);
            s32 speedDen = *(s32*)(hp + 0x104C);
            s32 delta = (speedNum * speedMul) / speedDen;
            s32 state = *(s32*)(hp + 0x54);
            s32 active;

            if (state != 4) {
                active = 0;
            } else if (*(s32*)(hp + 0x5C) != 0) {
                active = 0;
            } else {
                active = (*(s32*)(hp + 0x980) == 0);
            }

            if (active) {
                *(s32*)(hp + 0x1044) += delta;
                if (*(s32*)(hp + 0x1384) != 0) {
                    char cs[8];
                    SFLIB_LockCs(cs);
                    if (*(s32*)(hp + 0x138C) != -1) {
                        s32 idx = *(s32*)(hp + 0x138C) + 1;
                        *(s32*)(hp + 0x138C) = idx;
                        if (idx >= *(s32*)(hp + 0x1384)) {
                            *(s32*)(hp + 0x138C) = 0;
                            *(s32*)(hp + 0x1388) = 0;
                        }
                    }
                    SFLIB_UnlockCs(cs);
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

        if (SFSET_GetCond(h, 0x47) != 1) {
            continue;
        }

        {
            u8* hp = (u8*)h;
            char cs[8];
            s32 mode;
            Tc2TimeFn fn;

            SFLIB_LockCs(cs);
            mode = SFSET_GetCond(h, 0x0F);
            fn = *(Tc2TimeFn*)(hp + 0xD98 + mode * 4);
            if (fn == NULL) {
                fn = (Tc2TimeFn)sftim_GetTimeNone;
            }
            fn((s32)h, &timeVal, &timeUnit, NULL, 0);
            SFLIB_UnlockCs(cs);

            if (*(s32*)(hp + 0x1028) != timeVal || *(s32*)(hp + 0x102C) != timeUnit) {
                if (SFSET_GetCond(h, 0x47) == 1) {
                    *(s32*)(hp + 0x1070) = *(s32*)(hp + 0x1044);
                } else {
                    *(s32*)(hp + 0x1070) = *(s32*)(hp + 0x1084);
                }
                *(s32*)(hp + 0x1028) = timeVal;
                *(s32*)(hp + 0x102C) = timeUnit;

                if (lbl_eu_80606E34 != NULL) {
                    u32* fnTbl;
                    *(void**)(cb + 0x0C) = h;
                    *(void**)(cb + 0x18) = &timeVal;
                    *(void**)(cb + 0x24) = &timeUnit;
                    fnTbl = *(u32**)lbl_eu_80606E34;
                    ((void (*)(void*, void*))fnTbl[9])(lbl_eu_80606E34, cb + 4);
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
            s32 elapsed;
            if (SFSET_GetCond(self, 0x47) == 1) {
                elapsed = (*(s32*)(p + 0x1044) - *(s32*)(p + 0x1070)) / *(s32*)(lbl_eu_80606E38 + 0x1A8);
            } else {
                elapsed = (*(s32*)(p + 0x1084) - *(s32*)(p + 0x1070)) / *(s32*)(p + 0x1088);
            }
            result = (elapsed > cond33);
        }
    }

    if (result != 0) {
        SFLIB_SetErr((s32)self, 0xFF000222);
        return 1;
    }
    return 0;
}

s32 SFTIM_GetAudioStartSample(void* self, s32 sampleRate) {
    s64 startTime = *(s64*)((u8*)self + 0x158);

    if (startTime < 0) {
        return -1;
    }

    {
        s32 result = (s32)(startTime * sampleRate / 90000);
        lbl_eu_80619BB0[0] = *(u32*)((u8*)self + 0x158);
        lbl_eu_80619BB0[1] = *(u32*)((u8*)self + 0x15C);
        *lbl_eu_80619BB8 = result;
        return result;
    }
}

void SFTIM_GetVideoStartSample(void* self, int* out1, int* out2) {
    int val = *(int*)((u8*)self + 0x118);
    *out2 = val;
    if (val != 0) {
        int a = *(int*)((u8*)self + 0x13C);
        int b = *(int*)((u8*)self + 0x140);
        int r = UTY_MulDiv(a, val, b);
        lbl_eu_80619BB0[2] = a;
        *lbl_eu_80619BB8 = r; /* reusing debug global */
        *out1 = r;
    } else {
        int a = *(int*)((u8*)self + 0x110);
        if (a >= 0) {
            int b = *(int*)((u8*)self + 0x114);
            int r = UTY_MulDiv(a, val, b);
            lbl_eu_80619BB0[2] = a;
            *lbl_eu_80619BB8 = r;
            *out1 = r;
        } else {
            *out1 = -1;
        }
    }
}

void SFTIM_SetStartTime(void* self, u32 a, u32 b) { *(u32*)((u8*)self + 0x144) = a; *(u32*)((u8*)self + 0x148) = b; }

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

s32 SFTIM_GetTimeSub(void* self, s32* out1, s32* out2) {
    u8* p = (u8*)self;
    u8* conv = p + 0xEB0;

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
            if (*(s32*)conv != 0) {
                *out1 += UTY_MulDiv(*(s32*)(conv + 0x24), unit, *(s32*)(conv + 0x28));
            }
        }
    }
    return 0;
}

u32 SFTIM_GetTime(void* self, u32* out1, u32* out2) {
    *out1 = *(u32*)((u8*)self + 0x1028);
    *out2 = *(u32*)((u8*)self + 0x102c);
    return 0;
}

s32 sftim_GetTimeNone(void *self, s32 *out1, s32 *out2) {
    int val = *(int*)((char*)self + 0x54);
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
    s32 val = *(s32*)((u8*)self + 0x54);
    s32 ok;

    if (val == 4 || val == -4 || val == 6 || val == -6) {
        ok = 1;
    } else {
        *out1 = -1;
        *out2 = 1;
        ok = 0;
    }

    if (!ok) {
        return 0;
    }

    *out1 = *(s32*)((u8*)self + 0x1044);
    *out2 = *(s32*)(lbl_eu_80606E38 + 0x1A8);
    return 0;
}

s32 sftim_GetTimeUfrm(void* self, s32* out1, s32* out2) {
    s32 val = *(s32*)((u8*)self + 0x54);
    if (!(val == 4 || val == -4 || val == 6 || val == -6)) {
        *out1 = -1;
        *out2 = 1;
    }
    return 0;
}

s32 sftim_GetTimeUtim(void* self, s32* out1, s32* out2) {
    s32 val = *(s32*)((u8*)self + 0x54);
    s32 ok;

    if (val == 4 || val == -4 || val == 6 || val == -6) {
        ok = 1;
    } else {
        *out1 = -1;
        *out2 = 1;
        ok = 0;
    }

    if (!ok) {
        return 0;
    }

    {
        s32 (*fn)(void*, s32*, s32*) = *(s32(**)(void*, s32*, s32*))((u8*)self + 0x1074);
        if (fn == NULL) {
            *out1 = -2;
            *out2 = 1;
            return 0;
        }
        return fn(*(void**)((u8*)self + 0x1078), out1, out2);
    }
}

s32 sftim_GetTimeExtClock(void* self, s32* out1, s32* out2) {
    u8* p = (u8*)self;
    s32 val = *(s32*)(p + 0x54);
    s32 ok;

    if (val == 4 || val == -4 || val == 6 || val == -6) {
        ok = 1;
    } else {
        *out1 = -1;
        *out2 = 1;
        ok = 0;
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
                s32 lastVal = *(s32*)(p + 0x1080);
                if (lastVal != -5) {
                    s32 diff = clockVal - lastVal;
                    if (diff < 0) {
                        diff += *(s32*)(p + 0x108C) + 1;
                    }
                    *(s32*)(p + 0x1084) += diff;
                }
            }
        }

        *(s32*)(p + 0x1080) = clockVal;
        *(s32*)(p + 0x1088) = clockUnit;
        *out1 = *(s32*)(p + 0x1084);
        *out2 = *(s32*)(p + 0x1088);
    }
    return 0;
}

int SFTIM_ChkRegularTime(const void *tim, int *a2, int *a3) {
    int val = *(const int *)((const char *)tim + 0x54);
    if (val == 4 || val == -4 || val == 6 || val == -6) {
        return 1;
    }
    *a2 = -1;
    *a3 = 1;
    return 0;
}

s32 SFD_SetUsrIsSkipFn(void* self, void* fn) {
    if (SFLIB_CheckHn(self) != 0) {
        return SFLIB_SetErr(0, 0xFF000124);
    }
    *(void**)((u8*)self + 0xDB0) = fn;
    return 0;
}

s32 SFD_SetUsrTimeFn(void* self, void* fn, void* arg) {
    if (SFLIB_CheckHn(self) != 0) {
        return SFLIB_SetErr(0, 0xFF000123);
    }
    *(void**)((u8*)self + 0x1074) = fn;
    *(void**)((u8*)self + 0x1078) = arg;
    return 0;
}

s32 SFD_SetExtClockFn(void* self, void* fn, s32 wrap, void* arg) {
    if (SFLIB_CheckHn(self) != 0) {
        return SFLIB_SetErr(0, 0xFF000129);
    }
    if (fn != NULL) {
        *(void**)((u8*)self + 0x107C) = fn;
        *(s32*)((u8*)self + 0x108C) = wrap;
        *(void**)((u8*)self + 0x1090) = arg;
    } else {
        *(void**)((u8*)self + 0x1090) = arg;
        *(s32*)((u8*)self + 0x108C) = wrap;
        *(void**)((u8*)self + 0x107C) = NULL;
    }
    return 0;
}

void SFTIM_SetTimeFn(void* self, void* fn, u32 idx) {
    *(void**)((u8*)self + 0xd98 + idx * 4) = fn;
}

void SFTIM_Tc2Time(void* tcdata, s32* out1, s32* out2) {
    s32 format = *(s32*)tcdata;
    s32 sub = *(s32*)((u8*)tcdata + 4);
    Tc2TimeFn fn = *(Tc2TimeFn*)((u8*)lbl_eu_8051CC20 + format * 8 + sub * 4);

    if (fn == NULL) {
        SFLIB_SetErr(0, 0xFF000221);
        *out1 = 0;
        *out2 = 1;
        return;
    }

    fn(lbl_eu_8051CBF8[format], tcdata, out1, out2, (format == 1) ? 4 : 10);
}

void sftim_Tc2TimeN(s32 tc, void* tcdata, s32* out1, s32* out2, s32 rate) {
    u8* td = (u8*)tcdata;
    s32 unit = 1000 / rate;
    s32 hour = *(s32*)(td + 0x8);
    s32 min = *(s32*)(td + 0xC);
    s32 sec = *(s32*)(td + 0x10);
    s32 frame = *(s32*)(td + 0x14);
    s32 frame2 = *(s32*)(td + 0x18);
    s16 field = *(s16*)(td + 0x1E);
    s32 totalFrame = frame + frame2;
    s32 totalSec = hour * 3600 + min * 60 + sec;
    s32 tcSec = tc / rate;
    

    *out1 = tcSec * totalSec + totalFrame * unit + field * (unit / 2);
    *out2 = tcSec;
}

void sftim_Tc2Time23N(s32 tc, void* tcdata, s32* out1, s32* out2, s32 rate) {
    u8* td = (u8*)tcdata;
    s32 unit60 = 24000 / rate;
    s32 unit = 1000 / rate;
    s32 hour = *(s32*)(td + 0x8);
    s32 min = *(s32*)(td + 0xC);
    s32 sec = *(s32*)(td + 0x10);
    s32 frame = *(s32*)(td + 0x14);
    s32 frame2 = *(s32*)(td + 0x18);
    s16 field = *(s16*)(td + 0x1E);
    s32 totalFrame = frame + frame2;
    s32 totalSec = hour * 3600 + min * 60 + sec;
    

    *out1 = totalSec * unit60 + totalFrame * unit + field * (unit / 2);
    *out2 = tc / rate;
}

void sftim_Tc2Time29N(s32 tc, void* tcdata, s32* out1, s32* out2, s32 rate) {
    u8* td = (u8*)tcdata;
    s32 unit60 = 30000 / rate;
    s32 unit = 1000 / rate;
    s32 hour = *(s32*)(td + 0x8);
    s32 min = *(s32*)(td + 0xC);
    s32 sec = *(s32*)(td + 0x10);
    s32 frame = *(s32*)(td + 0x14);
    s32 frame2 = *(s32*)(td + 0x18);
    s16 field = *(s16*)(td + 0x1E);
    s32 totalFrame = frame + frame2;
    s32 totalSec = hour * 3600 + min * 60 + sec;
    

    *out1 = totalSec * unit60 + totalFrame * unit + field * (unit / 2);
    *out2 = tc / rate;
}

void sftim_Tc2Time59N(s32 tc, void* tcdata, s32* out1, s32* out2, s32 rate) {
    u8* td = (u8*)tcdata;
    s32 unit60 = 60000 / rate;
    s32 unit = 1000 / rate;
    s32 hour = *(s32*)(td + 0x8);
    s32 min = *(s32*)(td + 0xC);
    s32 sec = *(s32*)(td + 0x10);
    s32 frame = *(s32*)(td + 0x14);
    s32 frame2 = *(s32*)(td + 0x18);
    s16 field = *(s16*)(td + 0x1E);
    s32 totalFrame = frame + frame2;
    s32 totalSec = hour * 3600 + min * 60 + sec;
    

    *out1 = totalSec * unit60 + totalFrame * unit + field * (unit / 2);
    *out2 = tc / rate;
}

void sftim_Tc2Time23D(s32 tc, void* tcdata, s32* out1, s32* out2, s32 rate) {
    u8* td = (u8*)tcdata;
    s32 unit = 1000 / rate;
    s32 hour = *(s32*)(td + 0x8);
    s32 min = *(s32*)(td + 0xC);
    s32 sec = *(s32*)(td + 0x10);
    s32 frame = *(s32*)(td + 0x14);
    s32 frame2 = *(s32*)(td + 0x18);
    s16 field = *(s16*)(td + 0x1E);
    s32 totalFrame = frame + frame2;
    s32 dropFrames = (min / 10) * 2;
    s32 totalFrames = hour * 86292 + dropFrames + min * 1438 + sec * 24 + totalFrame;
    

    *out1 = totalFrames * unit + field * (unit / 2);
    *out2 = tc / rate;
}

void sftim_Tc2Time29D(s32 tc, void* tcdata, s32* out1, s32* out2, s32 rate) {
    u8* td = (u8*)tcdata;
    s32 unit = 1000 / rate;
    s32 hour = *(s32*)(td + 0x8);
    s32 min = *(s32*)(td + 0xC);
    s32 sec = *(s32*)(td + 0x10);
    s32 frame = *(s32*)(td + 0x14);
    s32 frame2 = *(s32*)(td + 0x18);
    s16 field = *(s16*)(td + 0x1E);
    s32 totalFrame = frame + frame2;
    s32 dropFrames = (min / 10) * 2;
    s32 totalFrames = hour * 107892 + dropFrames + min * 1798 + sec * 30 + totalFrame;
    

    *out1 = totalFrames * unit + field * (unit / 2);
    *out2 = tc / rate;
}

void sftim_Tc2Time59D(s32 tc, void* tcdata, s32* out1, s32* out2, s32 rate) {
    u8* td = (u8*)tcdata;
    s32 unit = 1000 / rate;
    s32 hour = *(s32*)(td + 0x8);
    s32 min = *(s32*)(td + 0xC);
    s32 sec = *(s32*)(td + 0x10);
    s32 frame = *(s32*)(td + 0x14);
    s32 frame2 = *(s32*)(td + 0x18);
    s16 field = *(s16*)(td + 0x1E);
    s32 totalFrame = frame + frame2;
    s32 dropFrames = (min / 10) * 2;
    s32 totalFrames = hour * 215892 + dropFrames + min * 3598 + sec * 60 + totalFrame;
    

    *out1 = totalFrames * unit + field * (unit / 2);
    *out2 = tc / rate;
}

void SFTIM_Pause(void* self, s32 mode) {
    u8* p = (u8*)self;

    if (mode == 2) {
        s32 num, den;
        s32 fps = *(s32*)(p + 0x930);
        char cs1[8];
        char cs2[8];

        if (fps == 0) {
            num = 0;
            den = 0x7512;
        } else {
            num = 1000;
            den = lbl_eu_8051CBF8[fps];
        }

        {
            s32 delta = UTY_MulDiv(*(s32*)(lbl_eu_80606E38 + 0x1A8), num, den);
            SFLIB_LockCs(cs1);
            *(s32*)(p + 0x1044) += delta;
            *(s32*)(p + 0x106C) += delta;
            SFLIB_UnlockCs(cs1);
        }

        {
            s32 delta2 = UTY_MulDiv(*(s32*)(p + 0x1088), num, den);
            SFLIB_LockCs(cs2);
            *(s32*)(p + 0x1084) += delta2;
            SFLIB_UnlockCs(cs2);
        }
    }
}

void SFTIM_GetTimeOneFrmVideo(void* self, int* out1, int* out2) {
    int fps = *(int*)((u8*)self + 0x930);
    if (fps == 0) {
        *out1 = fps;
        *out2 = 0x7512;
    } else {
        *out1 = 1000;
        *out2 = lbl_eu_8051CBF8[fps];
    }
}

s32 SFD_GetFps(void* self, s32* out) {
    u8* p = (u8*)self;

    *out = -1;
    if (SFLIB_CheckHn(self) != 0) {
        return SFLIB_SetErr(0, 0xFF00011B);
    }

    {
        s32 fps = *(s32*)(p + 0x930);
        if (fps == 0) {
            return 0;
        }
        *out = lbl_eu_8051CBF8[fps];
        if (*(s32*)(p + 0x88) != 0) {
            s32 ovrFps = *(s32*)(p + 0xF4);
            if (ovrFps != -1) {
                *out = ovrFps;
            }
        }
    }
    return 0;
}

void SFTIM_IsGetFrmTime(void* self, void* frm, s32 cmpA, s32 cmpB) {
    if (frm == NULL) {
        return;
    }
    if (*(s32*)((u8*)self + 0xA54) != 0) {
        *(s32*)((u8*)frm + 0) = 1;
        return;
    }
    {
        s32 result;
        SFTIM_IsExecTime(self, cmpA, cmpB, &result, *(s32*)((u8*)self + 0xACC));
        *(s32*)((u8*)frm + 0) = result;
    }
}

s32 SFTIM_IsGetFrmTimeTunit(void* self, s32 cmpA, s32 cmpB) {
    if (*(s32*)((u8*)self + 0xA54) != 0) {
        return 1;
    }
    {
        s32 result;
        SFTIM_IsExecTime(self, cmpA, cmpB, &result, *(s32*)((u8*)self + 0xACC));
        return result;
    }
}

void SFTIM_IsExecTime(void* self, s32 cmpA, s32 cmpB, s32* out, s32 param7) {
    u8* p = (u8*)self;
    s32 mode = *(s32*)(p + 0xA58);

    if (mode == 0) {
        *out = 1;
        return;
    }

    {
        s32 timeVal = *(s32*)(p + 0x1028);
        s32 timeUnit = *(s32*)(p + 0x102C);

        if (timeUnit == 1) {
            s32 audioTime = *(s32*)(p + 0x106C);
            if (audioTime < 0) {
                *(s32*)(p + 0x106C) = 0;
                *out = 1;
                return;
            }
            *out = (UTY_CmpTime(cmpA, cmpB, audioTime, *(s32*)(lbl_eu_80606E38 + 0x1A8)) != 0);
        } else {
            s32 offset = (timeUnit * param7) / *(s32*)(lbl_eu_80606E38 + 0x1A4);
            timeVal += offset;
            if (mode != 1) {
                sftim_IsGrExecTime(self, cmpA, cmpB, timeVal, timeUnit, out);
            } else {
                *out = (UTY_CmpTime(cmpA, cmpB, timeVal, timeUnit) != 0);
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

void SFTIM_IsVideoTerm(void* self) {}

void SFTIM_SetSpeed(void* self, u32 a, u32 b) { *(u32*)((u8*)self + 0x1048) = a; *(u32*)((u8*)self + 0x104C) = b; }

void SFTIM_GetSpeed(void* self, u32* out1, u32* out2) {
    *out1 = *(u32*)((u8*)self + 0x1048);
    *out2 = *(u32*)((u8*)self + 0x104c);
}

void SFD_SetCyclicFrameOutput(void* self, s32 num, s32 den) {
    if (SFLIB_CheckHn(self) != 0) {
        SFLIB_SetErr(0, 0xFF00012B);
        return;
    }
    {
        char cs[8];
        SFLIB_LockCs(cs);
        *(s32*)((u8*)self + 0x1380) = num;
        *(s32*)((u8*)self + 0x1384) = den;
        if (den != 0) {
            *(s32*)((u8*)self + 0x138C) = -1;
        }
        SFLIB_UnlockCs(cs);
    }
}

s32 SFTIM_ExecCyclicFrameOutput(void* self) {
    u8* p = (u8*)self;
    s32 result;

    if (*(s32*)(p + 0x1384) == 0) {
        return 1;
    }

    {
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
    }
    return result;
}

void SFD_CalcCycleFromFps(s32 fps, s32* out1, s32* out2) {
    f32 vsyncFreq = (f32)*(s32*)(lbl_eu_80606E38 + 0x1A4);
    f32 ratio = vsyncFreq / (f32)fps;

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

void SFD_SetLimitTime(void* self, u32 val) { *(u32*)((u8*)self + 0x1390) = val; }

void SFD_GetLimitTime(void* self, s32* out) {
    s32 cond = SFSET_GetCond(self, 0x36);
    s32 limitTime = *(s32*)((u8*)self + 0x1390);

    if (limitTime == -1) {
        *out = cond;
    } else if (cond == -1) {
        *out = limitTime;
    } else {
        *out = (cond < limitTime) ? cond : limitTime;
    }
}