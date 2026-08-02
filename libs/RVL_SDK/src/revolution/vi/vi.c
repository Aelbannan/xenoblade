// RVL_SDK 4.2.145 VI driver (private fork)
// High-level C reconstruction of retail vi.c (release build Feb 27 2009).

#include <revolution/OS.h>
#include <revolution/SC.h>
#include <revolution/VI.h>
#include <revolution/vi/vihardware.h>

// Defined in vi3in1.c (not yet matched in this TU).
extern void VISetRGBModeImm(void);
extern void __VISetGamma1_0(void);
extern void __VISetRevolutionModeSimple(void);

// Defined in this TU (retail .sbss / .sdata).
extern void* PreCB;
extern void* PostCB;
const char* __VIVersion = "<< RVL_SDK - VI \trelease build: Feb 27 2009 10:04:46 (0x4302_145) >>";

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
volatile u64 shdwChanged;
volatile u32 changeMode;
volatile u32 shdwChangeMode;
volatile u32 FBSet;
volatile u32 NextBufAddr;
u32 CurrTvMode;
s32 encoderType;
volatile u32 flushFlag3in1;
volatile u32 flushFlag;
volatile u32 NEW_TIME_TO_DIMMING;
volatile u32 THD_TIME_TO_DVD_STOP;
volatile u32 _gIdleCount_dvd;
volatile s32 g_current_time_to_dim;
volatile u32 __VIDimming_All_Clear;
volatile u32 __VIDimmingState;
volatile u32 __VIDVDStopFlag_Enable;
volatile u32 __VIDimmingFlag_Enable;
u16 shdwRegs[59];

static BOOL IsInitialized = FALSE;
s16 displayOffsetH;
s16 displayOffsetV;

u16 taps[26] = {
    0x1F0, 0x1DC, 0x1AE, 0x174, 0x129, 0x0DB, 0x08E, 0x046, 0x00C, 0x0E2,
    0x0CB, 0x0C0, 0x0C4, 0x0CF, 0x0DE, 0x0EC, 0x0FC, 0x008, 0x00F, 0x013,
    0x013, 0x00F, 0x00C, 0x008, 0x001, 0x000,
};

// Index (from the top bit) of the highest set bit of a 64-bit value.
static s32 cntlzd(u64 bit) {
    u32 hi;
    u32 lo;
    s32 value;

    hi = (u32)(bit >> 32);
    lo = (u32)(bit & 0xFFFFFFFFULL);
    value = __cntlzw(hi);
    if (value < 32) {
        return value;
    }
    return __cntlzw(lo) + 32;
}
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

static void flushRegs(BOOL enabled);

static BOOL OnShutdown(int final, unsigned long event) {
    BOOL retval;
    BOOL enabled;
    s32 regIndex;
    static BOOL first = TRUE;
    static u32 count;

    if (final == FALSE) {
        switch (event) {
        case 1:
        case 2:
        case 3:
            if (first) {
                VISetRGBModeImm();
                enabled = OSDisableInterrupts();
                flushRegs(enabled);
                count = retraceCount;
                first = FALSE;
                retval = FALSE;
            } else {
                if (count == retraceCount) {
                    retval = FALSE;
                } else {
                    retval = TRUE;
                }
            }
            break;
        case 0:
        case 4:
        case 5:
        case 6:
            __VISetGamma1_0();
            retval = TRUE;
            break;
        default:
            break;
        }
    } else {
        retval = TRUE;
    }

    return retval;
}


static void flushRegs(BOOL enabled) {
    s32 regIndex;

    shdwChangeMode |= changeMode;
    changeMode = 0;
    shdwChanged |= changed;

    while (changed != 0) {
        regIndex = cntlzd(changed);
        shdwRegs[regIndex] = regs[regIndex];
        changed &= ~((u64)1 << (63 - regIndex));
    }

    flushFlag = 1;
    flushFlag3in1 = 1;
    NextBufAddr = HorVer.bufAddr;
    OSRestoreInterrupts(enabled);
}

static OSShutdownFunctionInfo ShutdownFunctionInfo = { OnShutdown, 127, NULL, NULL };

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

void __VIInit(VITVMode mode) {
    timing_s* tm;
    volatile u32 i;
    u32 scan = (u32)mode & 3;
    u32 m4 = (u32)mode >> 2;
    u32 fmt;
    u16 di0h;

    *(volatile u32*)0x800000CC = m4;
    tm = getTiming(mode);

    VI_HW_REGS[VI_DCR] = 2;

    for (i = 0; i < 1000; i++) {
    }

    VI_HW_REGS[VI_DCR] = 0;

    VI_HW_REGS[VI_HTR0_L] = tm->hlw;
    VI_HW_REGS[VI_HTR0_H] = (tm->hcs << 8) | tm->hce;
    VI_HW_REGS[VI_HTR1_L] = tm->hsy | (tm->hbe640 << 7);
    VI_HW_REGS[VI_HTR1_H] = tm->hbs640 << 1;

    if (encoderType == 0) {
        VI_HW_REGS[VI_HBE] = tm->hbeCCIR656 | 0x8000;
        VI_HW_REGS[VI_HBS] = tm->hbsCCIR656;
    }

    VI_HW_REGS[VI_VTR] = tm->equ;
    VI_HW_REGS[VI_VTO_L] = tm->prbOdd + 2 * tm->acv - 2;
    VI_HW_REGS[VI_VTO_H] = tm->psbOdd + 2;
    VI_HW_REGS[VI_VTE_L] = tm->prbEven + 2 * tm->acv - 2;
    VI_HW_REGS[VI_VTE_H] = tm->psbEven + 2;

    VI_HW_REGS[VI_BBEI_L] = tm->bs1 | (tm->be1 << 5);
    VI_HW_REGS[VI_BBEI_H] = tm->bs3 | (tm->be3 << 5);
    VI_HW_REGS[VI_BBOI_L] = tm->bs2 | (tm->be2 << 5);
    VI_HW_REGS[VI_BBOI_H] = tm->bs4 | (tm->be4 << 5);

    VI_HW_REGS[VI_HSW] = 0x2828;
    VI_HW_REGS[VI_DI1_L] = 1;
    VI_HW_REGS[VI_DI1_H] = 0x1001;
    di0h = (u16)(((u32)tm->nhlines >> 1) + 1);
    VI_HW_REGS[VI_DI0_L] = tm->hlw + 1;
    VI_HW_REGS[VI_DI0_H] = di0h | 0x1000;

    fmt = (m4 - 1 <= 2) ? m4 : 0;

    if (scan <= 1) {
        VI_HW_REGS[VI_DCR] = (fmt << 8) | 1 | ((scan & 1) << 2);
        VI_HW_REGS[VI_VICLK] = 0;
    } else {
        VI_HW_REGS[VI_DCR] = (fmt << 8) | 5;
        VI_HW_REGS[VI_VICLK] = 1;
    }
}

static void ImportAdjustingValues(void) {
    displayOffsetH = SCGetDisplayOffsetH();
    displayOffsetV = 0;
}

#define CLAMP_VI(x, l, h) (((x) > (h)) ? (h) : (((x) < (l)) ? (l) : (x)))

static void AdjustPosition(u16 acv) {
    s32 coeff;
    s32 frac;

    HorVer.AdjustedDispPosX =
        (u16)CLAMP_VI((s16)HorVer.DispPosX + displayOffsetH, 0, 720 - HorVer.DispSizeX);

    coeff = (HorVer.FBMode == VI_XFBMODE_SF) ? 2 : 1;
    frac = HorVer.DispPosY & 1;

    HorVer.AdjustedDispPosY = (u16)MAX((s16)HorVer.DispPosY + displayOffsetV, frac);

    HorVer.AdjustedDispSizeY =
        (u16)(HorVer.DispSizeY + MIN((s16)HorVer.DispPosY + displayOffsetV - frac, 0) -
              MAX((s16)HorVer.DispPosY + (s16)HorVer.DispSizeY + displayOffsetV -
                      ((s16)acv * 2 - frac),
                  0));

    HorVer.AdjustedPanPosY =
        (u16)(HorVer.PanPosY - MIN((s16)HorVer.DispPosY + displayOffsetV - frac, 0) / coeff);

    HorVer.AdjustedPanSizeY =
        (u16)(HorVer.PanSizeY + MIN((s16)HorVer.DispPosY + displayOffsetV - frac, 0) / coeff -
              MAX((s16)HorVer.DispPosY + (s16)HorVer.DispSizeY + displayOffsetV -
                      ((s16)acv * 2 - frac),
                  0) /
                  coeff);
}

static BOOL VIEnableDVDStopMotor(BOOL enable) {
    BOOL old = __VIDVDStopFlag_Enable;

    __VIDVDStopFlag_Enable = enable;
    return old;
}

void VIInit(void) {
    u16 dspCfg;
    u32 value;
    u32 tv;
    u32 tvInBootrom;

    if (IsInitialized) {
        return;
    }

    OSRegisterVersion(__VIVersion);
    IsInitialized = TRUE;

    if (!(VI_HW_REGS[VI_DCR] & 1)) {
        __VIInit(VI_TVMODE_NTSC_INT);
    }

    retraceCount = 0;
    changed = 0;
    shdwChanged = 0;
    changeMode = 0;
    shdwChangeMode = 0;
    flushFlag = 0;
    flushFlag3in1 = 0;

    VI_HW_REGS[VI_FCT0_L] = taps[0] | ((taps[1] & 0x3F) << 10);
    VI_HW_REGS[VI_FCT0_H] = (taps[1] >> 6) | (taps[2] << 4);
    VI_HW_REGS[VI_FCT1_L] = taps[3] | ((taps[4] & 0x3F) << 10);
    VI_HW_REGS[VI_FCT1_H] = (taps[4] >> 6) | (taps[5] << 4);
    VI_HW_REGS[VI_FCT2_L] = taps[6] | ((taps[7] & 0x3F) << 10);
    VI_HW_REGS[VI_FCT2_H] = (taps[7] >> 6) | (taps[8] << 4);
    VI_HW_REGS[VI_FCT3_L] = taps[9] | (taps[10] << 8);
    VI_HW_REGS[VI_FCT3_H] = taps[11] | (taps[12] << 8);
    VI_HW_REGS[VI_FCT4_L] = taps[13] | (taps[14] << 8);
    VI_HW_REGS[VI_FCT4_H] = taps[15] | (taps[16] << 8);
    VI_HW_REGS[VI_FCT5_L] = taps[17] | (taps[18] << 8);
    VI_HW_REGS[VI_FCT5_H] = taps[19] | (taps[20] << 8);
    VI_HW_REGS[VI_FCT6_L] = taps[21] | (taps[22] << 8);
    VI_HW_REGS[VI_FCT6_H] = taps[23] | (taps[24] << 8);
    VI_HW_REGS[VI_0x70] = 0x280;

    ImportAdjustingValues();

    tvInBootrom = *(u32*)OSPhysicalToCached(0xCC);
    dspCfg = VI_HW_REGS[VI_DCR];
    HorVer.nonInter = VIGetScanMode();
    HorVer.tv = ((dspCfg & 0x300) >> 8);
    if ((tvInBootrom == VI_TVFORMAT_EURGB60) ||
        ((tvInBootrom == VI_TVFORMAT_PAL) && (HorVer.tv == VI_TVFORMAT_NTSC))) {
        HorVer.tv = VI_TVFORMAT_EURGB60;
    }

    tv = (HorVer.tv == VI_TVFORMAT_DEBUG) ? VI_TVFORMAT_NTSC : HorVer.tv;
    HorVer.timing = getTiming((VITVMode)VI_TVMODE(tv, HorVer.nonInter));
    regs[1] = dspCfg;
    CurrTiming = HorVer.timing;
    CurrTvMode = HorVer.tv;
    HorVer.DispSizeX = 640;
    HorVer.DispSizeY = (u16)(CurrTiming->acv * 2);
    HorVer.DispPosX = (u16)((720 - HorVer.DispSizeX) / 2);
    HorVer.DispPosY = 0;
    AdjustPosition(CurrTiming->acv);
    HorVer.FBSizeX = 640;
    HorVer.FBSizeY = (u16)(CurrTiming->acv * 2);
    HorVer.PanPosX = 0;
    HorVer.PanPosY = 0;
    HorVer.PanSizeX = 640;
    HorVer.PanSizeY = (u16)(CurrTiming->acv * 2);
    HorVer.FBMode = VI_XFBMODE_SF;
    HorVer.wordPerLine = 40;
    HorVer.std = 40;
    HorVer.wpl = 40;
    HorVer.xof = 0;
    HorVer.black = TRUE;
    HorVer.threeD = FALSE;

    OSInitThreadQueue(&retraceQueue);

    value = VI_HW_REGS[VI_DI0_H];
    value = (value & ~0x8000) | (0 << 15);
    VI_HW_REGS[VI_DI0_H] = (u16)value;

    value = VI_HW_REGS[VI_DI1_H];
    value = (value & ~0x8000) | (0 << 15);
    VI_HW_REGS[VI_DI1_H] = (u16)value;

    PreCB = NULL;
    PostCB = NULL;
    __OSSetInterruptHandler(OS_INTR_PI_VI, (OSInterruptHandler)__VIRetraceHandler);
    __OSUnmaskInterrupts(OS_INTR_MASK(OS_INTR_PI_VI));
    OSRegisterShutdownFunction(&ShutdownFunctionInfo);

    switch ((u32)VIGetTvFormat()) {
    case VI_TVFORMAT_PAL:
        THD_TIME_TO_DIMMING = 15000;
        NEW_TIME_TO_DIMMING = 15000;
        THD_TIME_TO_DVD_STOP = 90000;
        break;
    default:
        THD_TIME_TO_DIMMING = 18000;
        NEW_TIME_TO_DIMMING = 18000;
        THD_TIME_TO_DVD_STOP = 108000;
        break;
    }

    _gIdleCount_dimming = 0;
    _gIdleCount_dvd = 0;
    g_current_time_to_dim = VI_DM_DEFAULT;
    __VIDimming_All_Clear = TRUE;
    __VIDimmingState = FALSE;
    VIEnableDimming(TRUE);
    VIEnableDVDStopMotor(FALSE);
    __VISetRevolutionModeSimple();
}

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

void VIFlush(void) {
    BOOL enabled;
    s32 regIndex;

    enabled = OSDisableInterrupts();
    shdwChangeMode |= changeMode;
    changeMode = 0;
    shdwChanged |= changed;

    while (changed != 0) {
        regIndex = cntlzd(changed);
        shdwRegs[regIndex] = regs[regIndex];
        changed &= ~((u64)1 << (63 - regIndex));
    }

    flushFlag = 1;
    flushFlag3in1 = 1;
    NextBufAddr = HorVer.bufAddr;
    OSRestoreInterrupts(enabled);
}

void VISetNextFrameBuffer(void* fb) {
    BOOL enabled;

    enabled = OSDisableInterrupts();
    HorVer.bufAddr = (u32)fb;
    FBSet = 1;
    setFbbRegs(&HorVer, &HorVer.tfbb, &HorVer.bfbb, &HorVer.rtfbb, &HorVer.rbfbb);
    OSRestoreInterrupts(enabled);
}

void VISetBlack(BOOL black) {
    BOOL enabled;
    timing_s* tm;

    enabled = OSDisableInterrupts();
    HorVer.black = black;
    tm = HorVer.timing;
    setVerticalRegs(HorVer.AdjustedDispPosY, HorVer.DispSizeY, tm->equ, tm->acv, tm->prbOdd,
                    tm->prbEven, tm->psbOdd, tm->psbEven, black);
    OSRestoreInterrupts(enabled);
}

s32 VIGetRetraceCount(void) {
    return retraceCount;
}


static void GetCurrentDisplayPosition(u32* hct, u32* vct) {
    u32 hcount;
    u32 vcount;
    u32 prev;

    hcount = VI_HW_REGS[VI_DPV] & 0x7FF;
    do {
        vcount = VI_HW_REGS[VI_DPH] & 0x7FF;
        prev = hcount;
        hcount = VI_HW_REGS[VI_DPV] & 0x7FF;
    } while (prev != hcount);
    *hct = hcount;
    *vct = vcount;
}

static u32 getCurrentField(void) {
    u32 hcount;
    u32 vcount;
    u32 halfLine;
    u32 nh;

    GetCurrentDisplayPosition(&hcount, &vcount);
    halfLine = (hcount - 1) << 1;
    halfLine += (vcount - 1) / CurrTiming->hlw;
    nh = (u32)CurrTiming->nhlines;
    return ((nh << __cntlzw(nh ^ halfLine)) >> 31);
}

u32 VIGetNextField(void) {
    u32 field;
    BOOL enabled;

    enabled = OSDisableInterrupts();
    field = getCurrentField();
    OSRestoreInterrupts(enabled);

    return ((field ^ 1) ^ (HorVer.AdjustedDispPosY & 1));
}

u32 VIGetCurrentLine(void) {
    u32 hcount;
    u32 vcount;
    u32 halfLine;
    timing_s* tm;
    BOOL enabled;

    tm = CurrTiming;
    enabled = OSDisableInterrupts();
    GetCurrentDisplayPosition(&hcount, &vcount);

    halfLine = ((hcount - 1) << 1) + ((vcount - 1) / CurrTiming->hlw);
    OSRestoreInterrupts(enabled);

    if (halfLine >= tm->nhlines) {
        halfLine -= tm->nhlines;
    }

    return halfLine >> 1;
}

VITVFormat VIGetTvFormat(void) {
    VITVFormat format;
    BOOL enabled;

    enabled = OSDisableInterrupts();

    switch (CurrTvMode) {
    case VI_TVFORMAT_NTSC:
    case VI_TVFORMAT_DEBUG:
    case 6:
    case 7:
    case 8:
        format = VI_TVFORMAT_NTSC;
        break;
    case VI_TVFORMAT_PAL:
    case VI_TVFORMAT_DEBUG_PAL:
        format = VI_TVFORMAT_PAL;
        break;
    case VI_TVFORMAT_MPAL:
    case VI_TVFORMAT_EURGB60:
        format = (VITVFormat)CurrTvMode;
        break;
    }

    OSRestoreInterrupts(enabled);
    return format;
}

VIScanMode VIGetScanMode(void) {
    VIScanMode scanMode;
    BOOL enabled;

    enabled = OSDisableInterrupts();

    if ((u32)(VI_HW_REGS[VI_VICLK] & 1) == 1) {
        scanMode = VI_SCANMODE_PROG;
    } else {
        scanMode = (VIScanMode)(((u32)(VI_HW_REGS[VI_DCR] & 4) >> 2) != 0);
    }

    OSRestoreInterrupts(enabled);
    return scanMode;
}

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

BOOL VIEnableDimming(BOOL enable) {
    BOOL old;

    old = __VIDimmingFlag_Enable;
    if (enable == TRUE) {
        if (SCGetScreenSaverMode() == 0) {
            enable = FALSE;
        }
    }
    __VIDimmingFlag_Enable = enable;
    return old;
}

s32 VISetTimeToDimming(s32 time) {
    u32 mode;
    s32 old;
    BOOL enabled;

    old = g_current_time_to_dim;
    g_current_time_to_dim = time;
    enabled = OSDisableInterrupts();

    switch (CurrTvMode) {
    case VI_TVFORMAT_NTSC:
    case VI_TVFORMAT_DEBUG:
    case 6:
    case 7:
    case 8:
        mode = 0;
        break;
    case VI_TVFORMAT_PAL:
    case VI_TVFORMAT_DEBUG_PAL:
        mode = 1;
        break;
    case VI_TVFORMAT_MPAL:
    case VI_TVFORMAT_EURGB60:
        mode = CurrTvMode;
        break;
    }

    OSRestoreInterrupts(enabled);

    if (mode == 1) {
        switch (g_current_time_to_dim) {
        case VI_DM_10M:
            NEW_TIME_TO_DIMMING = 30000;
            break;
        case VI_DM_15M:
            NEW_TIME_TO_DIMMING = 45000;
            break;
        default:
            NEW_TIME_TO_DIMMING = 15000;
            break;
        }
    } else {
        switch (g_current_time_to_dim) {
        case VI_DM_10M:
            NEW_TIME_TO_DIMMING = 36000;
            break;
        case VI_DM_15M:
            NEW_TIME_TO_DIMMING = 54000;
            break;
        default:
            NEW_TIME_TO_DIMMING = 18000;
            break;
        }
    }

    return old;
}

BOOL VIResetDimmingCount(void) {
    __VIDimmingFlag_DEV_IDLE[0] = 0;
    return TRUE;
}

BOOL __VIResetRFIdle(void) {
    __VIDimmingFlag_RF_IDLE = 0;
    return TRUE;
}
