// RVL_SDK 4.2.145 VI driver (private fork)
// High-level C reconstruction of retail vi.c (release build Feb 27 2009).

#include <revolution/OS.h>
#include <revolution/VI.h>
#include <revolution/vi/vihardware.h>

// TV modes missing from the public vitypes.h enum (retail VITVMode)
#define VI_TVMODE_NTSC_3D 3
#define VI_TVMODE_PAL_PROG 6
#define VI_TVMODE_GCA_INT 24
#define VI_TVMODE_GCA_PROG 26
#define VI_TVMODE_EXTRA_INT 28
#define VI_TVMODE_EXTRA_DS 29
#define VI_TVMODE_EXTRA_PROG 30
#define VI_TVMODE_HD720_PROG 34

#define VI_INTERLACE 0
#define VI_NON_INTERLACE 1
#define VI_PROGRESSIVE 2

typedef struct {
    u8 equ;
    u16 acv;
    u16 prbOdd;
    u16 prbEven;
    u16 psbOdd;
    u16 psbEven;
    u8 bs1;
    u8 bs2;
    u8 bs3;
    u8 bs4;
    u16 be1;
    u16 be2;
    u16 be3;
    u16 be4;
    u16 nhlines;
    u16 hlw;
    u8 hsy;
    u8 hcs;
    u8 hce;
    u8 hbe640;
    u16 hbs640;
    u8 hbeCCIR656;
    u16 hbsCCIR656;
} timing_s;

typedef struct {
    u16 DispPosX;
    u16 DispPosY;
    u16 DispSizeX;
    u16 DispSizeY;
    u16 AdjustedDispPosX;
    u16 AdjustedDispPosY;
    u16 AdjustedDispSizeY;
    u16 AdjustedPanPosY;
    u16 AdjustedPanSizeY;
    u16 FBSizeX;
    u16 FBSizeY;
    u16 PanPosX;
    u16 PanPosY;
    u16 PanSizeX;
    u16 PanSizeY;
    VIXFBMode FBMode;
    u32 nonInter;
    u32 tv;
    u8 wordPerLine;
    u8 std;
    u8 wpl;
    u32 bufAddr;
    u32 tfbb;
    u32 bfbb;
    u8 xof;
    BOOL black;
    BOOL threeD;
    u32 rbufAddr;
    u32 rtfbb;
    u32 rbfbb;
    timing_s* timing;
} SomeVIStruct;

#define MARK_CHANGED(index) (changed |= 1LL << (63 - (index)))

volatile u64 changed = 0;
timing_s timing[11] = {
    { 6, 240, 24, 25, 3, 2, 12, 13, 12, 13, 520, 519, 520, 519, 525, 429, 64, 71, 105, 162, 373, 122, 412 },
    { 6, 240, 24, 24, 4, 4, 12, 12, 12, 12, 520, 520, 520, 520, 526, 429, 64, 71, 105, 162, 373, 122, 412 },
    { 5, 287, 35, 36, 1, 0, 13, 12, 11, 10, 619, 618, 617, 620, 625, 432, 64, 75, 106, 172, 380, 133, 420 },
    { 5, 287, 33, 33, 2, 2, 13, 11, 13, 11, 619, 621, 619, 621, 624, 432, 64, 75, 106, 172, 380, 133, 420 },
    { 6, 240, 24, 25, 3, 2, 16, 15, 14, 13, 518, 517, 516, 519, 525, 429, 64, 78, 112, 162, 373, 122, 412 },
    { 6, 240, 24, 24, 4, 4, 16, 14, 16, 14, 518, 520, 518, 520, 526, 429, 64, 78, 112, 162, 373, 122, 412 },
    { 12, 480, 48, 48, 6, 6, 24, 24, 24, 24, 1038, 1038, 1038, 1038, 1050, 429, 64, 71, 105, 162, 373, 122, 412 },
    { 12, 480, 44, 44, 10, 10, 24, 24, 24, 24, 1038, 1038, 1038, 1038, 1050, 429, 64, 71, 105, 168, 379, 122, 412 },
    { 6, 241, 24, 25, 1, 0, 12, 13, 12, 13, 520, 519, 520, 519, 525, 429, 64, 71, 105, 159, 370, 122, 412 },
    { 12, 480, 48, 48, 6, 6, 24, 24, 24, 24, 1038, 1038, 1038, 1038, 1050, 429, 64, 71, 105, 180, 391, 122, 412 },
    { 10, 576, 62, 62, 6, 6, 20, 20, 20, 20, 1240, 1240, 1240, 1240, 1250, 432, 64, 75, 106, 172, 380, 122, 412 },
};

timing_s* timingExtra;
u16 regs[59];
SomeVIStruct HorVer;
timing_s* CurrTiming;
volatile u32 retraceCount;
OSThreadQueue retraceQueue;
volatile u32 THD_TIME_TO_DIMMING = 0;
volatile u32 _gIdleCount_dimming = 0;
volatile u32 __VIDimmingFlag_RF_IDLE;
volatile u32 __VIDimmingFlag_DEV_IDLE[10];

void OnShutdown(void) {}

void __VIRetraceHandler() {}

VIRetraceCallback VISetPreRetraceCallback(VIRetraceCallback callback) {
    extern void* PreCB;
    VIRetraceCallback prev = (VIRetraceCallback)PreCB;
    BOOL enabled = OSDisableInterrupts();
    PreCB = callback;
    OSRestoreInterrupts(enabled);
    return prev;
}

VIRetraceCallback VISetPostRetraceCallback(VIRetraceCallback callback) {
    extern void* PostCB;
    VIRetraceCallback old = (VIRetraceCallback)PostCB;
    BOOL level = OSDisableInterrupts();
    PostCB = callback;
    OSRestoreInterrupts(level);
    return old;
}

#pragma dont_inline on
timing_s* getTiming(VITVMode mode) {
    switch (mode) {
    case VI_TVMODE_NTSC_INT:
        return &timing[0];
    case VI_TVMODE_NTSC_DS:
        return &timing[1];
    case VI_TVMODE_PAL_INT:
        return &timing[2];
    case VI_TVMODE_PAL_DS:
        return &timing[3];
    case VI_TVMODE_EURGB60_INT:
        return &timing[0];
    case VI_TVMODE_EURGB60_DS:
        return &timing[1];
    case VI_TVMODE_MPAL_INT:
        return &timing[4];
    case VI_TVMODE_MPAL_DS:
        return &timing[5];
    case VI_TVMODE_NTSC_PROG:
    case VI_TVMODE_MPAL_PROG:
    case VI_TVMODE_EURGB60_PROG:
        return &timing[6];
    case VI_TVMODE_NTSC_3D:
        return &timing[7];
    case VI_TVMODE_DEBUG_PAL_INT:
        return &timing[2];
    case VI_TVMODE_DEBUG_PAL_DS:
        return &timing[3];
    case VI_TVMODE_GCA_INT:
        return &timing[8];
    case VI_TVMODE_GCA_PROG:
        return &timing[9];
    case VI_TVMODE_PAL_PROG:
        return &timing[10];
    case VI_TVMODE_EXTRA_INT:
    case VI_TVMODE_EXTRA_DS:
    case VI_TVMODE_EXTRA_PROG:
    case VI_TVMODE_HD720_PROG:
        return timingExtra;
    default:
        return NULL;
    }
}
#pragma dont_inline reset

void __VIInit(VITVMode mode) {}

void VIInit(void) {}

void VIWaitForRetrace(void) {
    BOOL enabled;
    u32 count;

    enabled = OSDisableInterrupts();
    count = retraceCount;
    do {
        OSSleepThread(&retraceQueue);
    } while (count == retraceCount);
    OSRestoreInterrupts(enabled);
}

static void calcFbbs(u32 bufAddr, u16 panPosX, u16 panPosY, u8 wordPerLine, VIXFBMode xfbMode,
                     u16 dispPosY, u32* tfbb, u32* bfbb) {
    u32 bytesPerLine;
    u32 xoffInWords;

    xoffInWords = (u32)panPosX / 16;
    bytesPerLine = (u32)wordPerLine * 32;
    *tfbb = bufAddr + (xoffInWords << 5) + (bytesPerLine * panPosY);
    *bfbb = (xfbMode == VI_XFBMODE_SF) ? *tfbb : *tfbb + bytesPerLine;

    if (dispPosY % 2 == 1) {
        u32 tmp = *tfbb;
        *tfbb = *bfbb;
        *bfbb = tmp;
    }

    *tfbb &= 0x3FFFFFFF;
    *bfbb &= 0x3FFFFFFF;
}

void setFbbRegs(SomeVIStruct* vi, u32* tfbb, u32* bfbb, u32* rtfbb, u32* rbfbb) {
    u32 shifted;

    calcFbbs(vi->bufAddr, vi->PanPosX, vi->AdjustedPanPosY, vi->wordPerLine, vi->FBMode,
             vi->AdjustedDispPosY, tfbb, bfbb);
    if (vi->threeD) {
        calcFbbs(vi->rbufAddr, vi->PanPosX, vi->AdjustedPanPosY, vi->wordPerLine, vi->FBMode,
                 vi->AdjustedDispPosY, rtfbb, rbfbb);
    }

    if (*tfbb < 0x01000000U && *bfbb < 0x01000000U && *rtfbb < 0x01000000U &&
        *rbfbb < 0x01000000U) {
        shifted = 0;
    } else {
        shifted = 1;
    }

    if (shifted) {
        *tfbb >>= 5;
        *bfbb >>= 5;
        *rtfbb >>= 5;
        *rbfbb >>= 5;
    }

    regs[15] = (u16)(*tfbb & 0xFFFF);
    MARK_CHANGED(15);
    regs[14] = (shifted << 12) | ((*tfbb >> 16) | (vi->xof << 8));
    MARK_CHANGED(14);
    regs[19] = (u16)(*bfbb & 0xFFFF);
    MARK_CHANGED(19);
    regs[18] = (*bfbb >> 16);
    MARK_CHANGED(18);

    if (vi->threeD) {
        regs[17] = (u16)(*rtfbb & 0xFFFF);
        MARK_CHANGED(17);
        regs[16] = *rtfbb >> 16;
        MARK_CHANGED(16);
        regs[21] = (u16)(*rbfbb & 0xFFFF);
        MARK_CHANGED(21);
        regs[20] = *rbfbb >> 16;
        MARK_CHANGED(20);
    }
}

void setHorizontalRegs(timing_s* tm, u16 dispPosX, u16 dispSizeX) {
    u32 hbe;
    u32 hbs;
    u32 hbeLo;
    u32 hbeHi;

    regs[3] = (u16)(u32)tm->hlw;
    MARK_CHANGED(3);
    regs[2] = tm->hce | (tm->hcs << 8);
    MARK_CHANGED(2);

    if (HorVer.tv == 8) {
        hbe = (u32)(tm->hbe640 + 172);
        hbs = tm->hbs640;
    } else {
        hbe = (u32)(tm->hbe640 - 40 + dispPosX);
        hbs = (u32)(tm->hbs640 + 40 + dispPosX - (720 - dispSizeX));
    }

    hbeLo = hbe & 0x1FF;
    hbeHi = hbe >> 9;

    regs[5] = tm->hsy | (hbeLo << 7);
    MARK_CHANGED(5);
    regs[4] = hbeHi | (hbs * 2);
    MARK_CHANGED(4);
}

void setVerticalRegs(u16 dispPosY, u16 dispSizeY, u8 equ, u16 acv, u16 prbOdd, u16 prbEven,
                            u16 psbOdd, u16 psbEven, BOOL black) {
    u16 actualPrbOdd;
    u16 actualPrbEven;
    u16 actualPsbOdd;
    u16 actualPsbEven;
    u16 actualAcv;
    u16 c;
    u16 d;

    if (HorVer.nonInter == 2 || HorVer.nonInter == 3) {
        c = 1;
        d = 2;
    } else {
        c = 2;
        d = 1;
    }

    if ((dispPosY % 2) == 0) {
        actualPrbOdd = prbOdd + (d * dispPosY);
        actualPsbOdd = psbOdd + (d * (((c * acv) - dispSizeY) - dispPosY));
        actualPrbEven = prbEven + (d * dispPosY);
        actualPsbEven = psbEven + (d * (((c * acv) - dispSizeY) - dispPosY));
    } else {
        actualPrbOdd = prbEven + (d * dispPosY);
        actualPsbOdd = psbEven + (d * (((c * acv) - dispSizeY) - dispPosY));
        actualPrbEven = prbOdd + (d * dispPosY);
        actualPsbEven = psbOdd + (d * (((c * acv) - dispSizeY) - dispPosY));
    }

    actualAcv = dispSizeY / c;

    if (black) {
        s32 d2 = 2 * actualAcv - 2;

        actualPrbOdd = (u16)(actualPrbOdd + d2);
        actualPsbOdd = (u16)(actualPsbOdd + 2);
        actualPrbEven = (u16)(actualPrbEven + d2);
        actualPsbEven = (u16)(actualPsbEven + 2);
        actualAcv = 0;
    }

    regs[0] = equ | (actualAcv << 4);
    MARK_CHANGED(0);
    regs[7] = (u16)(u32)actualPrbOdd;
    MARK_CHANGED(7);
    regs[6] = (u16)(u32)actualPsbOdd;
    MARK_CHANGED(6);
    regs[9] = (u16)(u32)actualPrbEven;
    MARK_CHANGED(9);
    regs[8] = (u16)(u32)actualPsbEven;
    MARK_CHANGED(8);
}

void VIConfigure(const GXRenderModeObj* rmo) {}

void VIConfigurePan(u16 x, u16 y, u16 w, u16 h) {}

void VIFlush(void) {}

void VISetNextFrameBuffer(void* fb) {}

void VISetBlack(BOOL black) {}

s32 VIGetRetraceCount(void) {
    return retraceCount;
}

u32 VIGetNextField(void) { return 0; }

u32 VIGetCurrentLine(void) { return 0; }

VITVFormat VIGetTvFormat(void) { return VI_TVFORMAT_NTSC; }

VIScanMode VIGetScanMode(void) { return VI_SCANMODE_INT; }

u32 VIGetDTVStatus(void) {
    u32 dtvStatus;
    BOOL enabled = OSDisableInterrupts();

    dtvStatus = VI_HW_REGS[VI_VISEL] & 3;
    OSRestoreInterrupts(enabled);
    return dtvStatus & 1;
}

void __VIDisplayPositionToXY(u32 hct, u32 vct, s16* x, s16* y) {
    u32 halfLine = ((vct - 1) << 1) + ((hct - 1) / CurrTiming->hlw);

    if (HorVer.nonInter == VI_INTERLACE) {
        if (halfLine < CurrTiming->nhlines) {
            if (halfLine < CurrTiming->equ * 3 + CurrTiming->prbOdd) {
                *y = -1;
            } else if (halfLine >= CurrTiming->nhlines - CurrTiming->psbOdd) {
                *y = -1;
            } else {
                *y = (s16)((halfLine - CurrTiming->equ * 3 - CurrTiming->prbOdd) & ~1);
            }
        } else {
            halfLine -= CurrTiming->nhlines;

            if (halfLine < CurrTiming->equ * 3 + CurrTiming->prbEven) {
                *y = -1;
            } else if (halfLine >= CurrTiming->nhlines - CurrTiming->psbEven) {
                *y = -1;
            } else {
                *y = (s16)(((halfLine - CurrTiming->equ * 3 - CurrTiming->prbEven) & ~1) + 1);
            }
        }
    } else if (HorVer.nonInter == VI_NON_INTERLACE) {
        if (halfLine >= CurrTiming->nhlines) {
            halfLine -= CurrTiming->nhlines;
        }

        if (halfLine < CurrTiming->equ * 3 + CurrTiming->prbOdd) {
            *y = -1;
        } else if (halfLine >= CurrTiming->nhlines - CurrTiming->psbOdd) {
            *y = -1;
        } else {
            *y = (s16)((halfLine - CurrTiming->equ * 3 - CurrTiming->prbOdd) & ~1);
        }
    } else if (HorVer.nonInter == VI_PROGRESSIVE) {
        if (halfLine < CurrTiming->nhlines) {
            if (halfLine < CurrTiming->equ * 3 + CurrTiming->prbOdd) {
                *y = -1;
            } else if (halfLine >= CurrTiming->nhlines - CurrTiming->psbOdd) {
                *y = -1;
            } else {
                *y = (s16)(halfLine - CurrTiming->equ * 3 - CurrTiming->prbOdd);
            }
        } else {
            halfLine -= CurrTiming->nhlines;

            if (halfLine < CurrTiming->equ * 3 + CurrTiming->prbEven) {
                *y = -1;
            } else if (halfLine >= CurrTiming->nhlines - CurrTiming->psbEven) {
                *y = -1;
            } else {
                *y = (s16)((halfLine - CurrTiming->equ * 3 - CurrTiming->prbEven) & ~1);
            }
        }
    }

    *x = (s16)(hct - 1);
}

u32 VIGetDimmingCount(void) {
    u32 count;

    if (_gIdleCount_dimming >= THD_TIME_TO_DIMMING) {
        count = 0;
    } else {
        count = (u32)THD_TIME_TO_DIMMING - _gIdleCount_dimming;
    }
    return count;
}

BOOL VIEnableDimming(BOOL enable) { return FALSE; }

s32 VISetTimeToDimming(s32 time) { return 0; }

BOOL VIResetDimmingCount(void) {
    __VIDimmingFlag_DEV_IDLE[0] = 0;
    return TRUE;
}

BOOL __VIResetRFIdle(void) {
    __VIDimmingFlag_RF_IDLE = 0;
    return TRUE;
}
