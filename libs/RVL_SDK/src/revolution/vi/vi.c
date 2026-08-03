// RVL_SDK 4.2.145 VI driver (private fork)
// High-level C reconstruction of retail vi.c (release build Feb 27 2009).

#include <revolution/DVD.h>
#include <revolution/OS.h>
#include <revolution/SC.h>
#include <revolution/SI.h>
#include <revolution/VI.h>
#include <revolution/vi/vihardware.h>

// Defined in vi3in1.c (not yet matched in this TU).
extern void VISetRGBModeImm(void);
extern void __VISetGamma1_0(void);
extern void __VISetRevolutionModeSimple(void);
extern void __VISetYUVSEL(u32 sel);
extern void __VISetFilter4EURGB60(u8 sel);
extern void __VISetCGMS(void);
extern void __VISetWSS(void);
extern void __VISetClosedCaption(void);
extern void __VISetMacrovision(void);
extern void __VISetGamma(void);
extern void __VISetTrapFilter(void);
extern void __VISetRGBOverDrive(void);
extern u32 Vdac_Flag_Changed;
extern DVDCommandBlock __DVDStopMotorCommandBlock;

// Defined in this TU (retail .sbss / .sdata).
extern void* PreCB;
extern void* PostCB;
const char* __VIVersion = "<< RVL_SDK - VI \trelease build: Feb 27 2009 10:04:46 (0x4302_145) >>";

#define CLAMP_VI(x, l, h) (((x) > (h)) ? (h) : (((x) < (l)) ? (l) : (x)))

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
volatile u32 __VIDimmingFlag_SI_IDLE;
u32 vsync_timing_err_cnt;
u32 vsync_timing_test_flag;
u32 CurrBufAddr;
VIPositionCallback PositionCallback;
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

void __VIDisplayPositionToXY(u32 hct, u32 vct, s16* x, s16* y);

void __VIRetraceHandler(s16 intrType, OSContext* ctx) {
    OSContext exceptionContext;
    u16 reg;
    u32 intrMask = 0;
    BOOL flushNow;
    BOOL enabled;
    u32 field;
    u32 hcount;
    u32 vcount;
    u32 prev;
    u32 halfLine;
    u32 nh;
    u32 dtvStatus;
    u32 tvtype;
    s16 x;
    s16 y;
    s32 regIndex;
    u32 bit;
    static u32 old_dtvStatus = 0x3E7;
    static u32 old_tvtype = 0x3E7;
    static u32 __VIDimmingFlag_Enable_old = 1;
    static u32 __VIDVDStopFlag_Enable_old = 1;
    static u32 DimmingON_Pending;
    static u32 DimmingOFF_Pending;

    reg = VI_HW_REGS[VI_DI0_H];
    if (reg & 0x8000) {
        VI_HW_REGS[VI_DI0_H] = (u16)(reg & ~0x8000);
        intrMask |= 1;
    }

    reg = VI_HW_REGS[VI_DI1_H];
    if (reg & 0x8000) {
        VI_HW_REGS[VI_DI1_H] = (u16)(reg & ~0x8000);
        intrMask |= 2;
    }

    reg = VI_HW_REGS[VI_DI2_H];
    if (reg & 0x8000) {
        VI_HW_REGS[VI_DI2_H] = (u16)(reg & ~0x8000);
        intrMask |= 4;
    }

    reg = VI_HW_REGS[VI_DI3_H];
    if (reg & 0x8000) {
        VI_HW_REGS[VI_DI3_H] = (u16)(reg & ~0x8000);
        intrMask |= 8;
    }

    if ((intrMask & 4) || (intrMask & 8)) {
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(&exceptionContext);

        if (PositionCallback) {
            hcount = VI_HW_REGS[VI_DPV] & 0x7FF;
            do {
                vcount = VI_HW_REGS[VI_DPH] & 0x7FF;
                prev = hcount;
                hcount = VI_HW_REGS[VI_DPV] & 0x7FF;
            } while (prev != hcount);
            __VIDisplayPositionToXY(vcount, hcount, &x, &y);
            PositionCallback(x, y);
        }

        OSClearContext(&exceptionContext);
        OSSetCurrentContext(ctx);
        return;
    }

    retraceCount++;

    OSClearContext(&exceptionContext);
    OSSetCurrentContext(&exceptionContext);

    if (PreCB) {
        ((VIRetraceCallback)PreCB)(retraceCount);
    }

    if (vsync_timing_test_flag) {
        hcount = VI_HW_REGS[VI_DPV] & 0x7FF;
        do {
            vcount = VI_HW_REGS[VI_DPH] & 0x7FF;
            prev = hcount;
            hcount = VI_HW_REGS[VI_DPV] & 0x7FF;
        } while (prev != hcount);

        if (hcount != 1 && hcount != ((u32)CurrTiming->nhlines >> 1) + 1) {
            vsync_timing_err_cnt++;
        }
    }

    if (flushFlag) {
        if (shdwChangeMode == 1) {
            hcount = VI_HW_REGS[VI_DPV] & 0x7FF;
            do {
                vcount = VI_HW_REGS[VI_DPH] & 0x7FF;
                prev = hcount;
                hcount = VI_HW_REGS[VI_DPV] & 0x7FF;
            } while (prev != hcount);

            halfLine = ((hcount - 1) << 1) + (vcount - 1) / CurrTiming->hlw;
            nh = CurrTiming->nhlines;
            field = ((nh << __cntlzw(nh ^ halfLine)) >> 31);
            if (field == 0) {
                flushNow = FALSE;
                goto flushCheck;
            }
        }

        while (shdwChanged != 0) {
            regIndex = cntlzd(shdwChanged);
            VI_HW_REGS[regIndex] = shdwRegs[regIndex];
            shdwChanged &= ~((u64)1 << (63 - regIndex));
        }

        shdwChangeMode = 0;
        CurrTiming = HorVer.timing;
        CurrTvMode = HorVer.tv;
        CurrBufAddr = NextBufAddr;
        flushNow = TRUE;

    flushCheck:
        if (flushNow) {
            flushFlag = 0;
            SIRefreshSamplingRate();
        }
    }

    enabled = OSDisableInterrupts();
    reg = VI_HW_REGS[VI_VISEL] & 3;
    OSRestoreInterrupts(enabled);
    dtvStatus = reg & 1;
    if (dtvStatus != old_dtvStatus) {
        __VISetYUVSEL(dtvStatus);
    }
    old_dtvStatus = dtvStatus;

    enabled = OSDisableInterrupts();
    switch (CurrTvMode) {
    case VI_TVFORMAT_NTSC:
    case VI_TVFORMAT_DEBUG:
    case 6:
    case 7:
    case 8:
        tvtype = 0;
        break;
    case VI_TVFORMAT_PAL:
    case VI_TVFORMAT_DEBUG_PAL:
        tvtype = 1;
        break;
    case VI_TVFORMAT_MPAL:
    case VI_TVFORMAT_EURGB60:
        tvtype = CurrTvMode;
        break;
    }
    OSRestoreInterrupts(enabled);

    if (tvtype != old_tvtype) {
        old_tvtype = tvtype;

        if (tvtype == 5) {
            __VISetFilter4EURGB60(1);
        } else {
            __VISetFilter4EURGB60(0);
        }

        if (tvtype == 1) {
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
            THD_TIME_TO_DVD_STOP = 90000;
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
            THD_TIME_TO_DVD_STOP = 108000;
        }

        _gIdleCount_dimming = 0;
        _gIdleCount_dvd = 0;
    }

    if (flushFlag3in1) {
        while (Vdac_Flag_Changed != 0) {
            bit = (u32)1 << (31 - __cntlzw(Vdac_Flag_Changed));
            switch (bit) {
            case 0x01:
                __VISetCGMS();
                break;
            case 0x02:
                __VISetWSS();
                break;
            case 0x04:
                __VISetClosedCaption();
                break;
            case 0x08:
                __VISetMacrovision();
                break;
            case 0x10:
                __VISetGamma();
                break;
            case 0x20:
                __VISetTrapFilter();
                break;
            case 0x40:
                __VISetRGBOverDrive();
                break;
            case 0x80:
                __VISetRGBModeImm();
                break;
            }
            Vdac_Flag_Changed &= ~bit;
        }
        flushFlag3in1 = 0;
    }

    if (PostCB) {
        OSClearContext(&exceptionContext);
        ((VIRetraceCallback)PostCB)(retraceCount);
    }

    OSWakeupThread(&retraceQueue);
    OSClearContext(&exceptionContext);
    OSSetCurrentContext(ctx);

    if (__VIDimming_All_Clear == 1) {
        if (__OSSetVIForceDimming(0, 0, 0) == 1) {
            __VIDimming_All_Clear = 0;
            _gIdleCount_dimming = 0;
        }
    }

    for (regIndex = 0; regIndex < 10; regIndex++) {
        if (__VIDimmingFlag_DEV_IDLE[regIndex] == 0) {
            __VIDimmingFlag_DEV_IDLE[0] = 0;
            break;
        }
    }

    if (__VIDimmingFlag_RF_IDLE != 0 && __VIDimmingFlag_SI_IDLE != 0 &&
        __VIDimmingFlag_DEV_IDLE[0] != 0) {
        if (__VIDimmingFlag_Enable == 1) {
            if (_gIdleCount_dimming < 0xFFFFFFFF) {
                _gIdleCount_dimming++;
            }
        }
        if (__VIDVDStopFlag_Enable == 1) {
            if (_gIdleCount_dvd < 0xFFFFFFFF) {
                _gIdleCount_dvd++;
            }
        }
    } else {
        if (_gIdleCount_dimming >= THD_TIME_TO_DIMMING) {
            DimmingOFF_Pending = 1;
        }
        if (_gIdleCount_dvd >= THD_TIME_TO_DVD_STOP) {
            __DVDRestartMotor();
        }
        _gIdleCount_dimming = 0;
        _gIdleCount_dvd = 0;
        THD_TIME_TO_DIMMING = NEW_TIME_TO_DIMMING;
    }

    if (__VIDimmingFlag_Enable_old != __VIDimmingFlag_Enable) {
        if (__VIDimmingFlag_Enable == 0) {
            if (_gIdleCount_dimming >= THD_TIME_TO_DIMMING) {
                DimmingOFF_Pending = 1;
            }
        }
        _gIdleCount_dimming = 0;
        THD_TIME_TO_DIMMING = NEW_TIME_TO_DIMMING;
    }

    if (_gIdleCount_dimming == THD_TIME_TO_DIMMING) {
        DimmingON_Pending = 1;
    }

    if (DimmingOFF_Pending != 0) {
        if (__OSSetVIForceDimming(0, 2, 2) == 1) {
            DimmingOFF_Pending = 0;
            __VIDimmingState = 0;
        }
    }

    if (DimmingON_Pending != 0) {
        if (__OSSetVIForceDimming(1, 2, 2) == 1) {
            DimmingON_Pending = 0;
            __VIDimmingState = 1;
        }
    }

    if (__VIDVDStopFlag_Enable_old != __VIDVDStopFlag_Enable) {
        if (__VIDVDStopFlag_Enable == 0) {
            if (_gIdleCount_dvd >= THD_TIME_TO_DVD_STOP) {
                __DVDRestartMotor();
            }
        }
        _gIdleCount_dvd = 0;
    }

    if (_gIdleCount_dvd == THD_TIME_TO_DVD_STOP) {
        __DVDStopMotorAsync(&__DVDStopMotorCommandBlock, 0);
    }

    __VIDimmingFlag_RF_IDLE = 1;
    __VIDimmingFlag_SI_IDLE = 1;
    __VIDimmingFlag_DEV_IDLE[0] = 1;
    __VIDimmingFlag_DEV_IDLE[1] = 1;
    __VIDimmingFlag_DEV_IDLE[2] = 1;
    __VIDimmingFlag_DEV_IDLE[3] = 1;
    __VIDimmingFlag_DEV_IDLE[4] = 1;
    __VIDimmingFlag_DEV_IDLE[5] = 1;
    __VIDimmingFlag_DEV_IDLE[6] = 1;
    __VIDimmingFlag_DEV_IDLE[7] = 1;
    __VIDimmingFlag_DEV_IDLE[8] = 1;
    __VIDimmingFlag_DEV_IDLE[9] = 1;

    __VIDimmingFlag_Enable_old = __VIDimmingFlag_Enable;
    __VIDVDStopFlag_Enable_old = __VIDVDStopFlag_Enable;

    if (NEW_TIME_TO_DIMMING > _gIdleCount_dimming) {
        if (__VIDimmingState == 0) {
            THD_TIME_TO_DIMMING = NEW_TIME_TO_DIMMING;
        }
    }
}

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

static void PrintDebugPalCaution(void) {
    static u32 message;

    if (!message) {
        message = 1;
        OSReport("***************************************\n");
        OSReport(" ! ! ! C A U T I O N ! ! !             \n");
        OSReport("This TV format \"DEBUG_PAL\" is only for \n");
        OSReport("temporary solution until PAL DAC board \n");
        OSReport("is available. Please do NOT use this   \n");
        OSReport("mode in real games!!!                  \n");
        OSReport("***************************************\n");
    }
}

void VIConfigure(const GXRenderModeObj* rmo) {
    timing_s* tm;
    BOOL enabled;
    u32 newNonInter;
    u32 newTvMode;
    u32 currTvMode;
    s32 coeff;
    s32 frac;
    u16 hct;
    u16 vct;
    u32 w;
    u32 scale;
    u8 wordPerLine;
    u8 std;
    u8 wpl;
    u8 xof;

    enabled = OSDisableInterrupts();

    newNonInter = rmo->viTVmode & 3;
    if (HorVer.nonInter != newNonInter) {
        changeMode = 1;
        HorVer.nonInter = newNonInter;
    }

    newTvMode = (u32)rmo->viTVmode >> 2;
    currTvMode = *(u32*)OSPhysicalToCached(0xCC);

    if (newTvMode == 4) {
        PrintDebugPalCaution();
    }

    if (((currTvMode != 1 && currTvMode != 5) && (newTvMode == 1 || newTvMode == 5)) ||
        ((currTvMode == 1 || currTvMode == 5) && (newTvMode != 1 && newTvMode != 5))) {
        OSPanic("vi.c", 0xA5B,
                "VIConfigure(): Tried to change mode from (%d) to (%d), which is forbidden\n",
                currTvMode, newTvMode);
    }

    if (newTvMode == 0 || newTvMode == 2) {
        HorVer.tv = currTvMode;
    } else {
        HorVer.tv = newTvMode;
    }

    HorVer.DispPosX = rmo->viXOrigin;
    HorVer.DispPosY =
        (u16)((HorVer.nonInter == 1) ? (u16)(rmo->viYOrigin << 1) : rmo->viYOrigin);
    HorVer.DispSizeX = rmo->viWidth;
    HorVer.FBSizeX = rmo->fbWidth;
    HorVer.FBSizeY = rmo->xfbHeight;
    HorVer.FBMode = rmo->xFBmode;
    HorVer.PanSizeX = rmo->fbWidth;
    HorVer.PanSizeY = rmo->xfbHeight;
    HorVer.PanPosX = 0;
    HorVer.PanPosY = 0;

    HorVer.DispSizeY = (HorVer.nonInter == 2)                    ? rmo->xfbHeight
                     : (HorVer.nonInter == 3)                    ? rmo->xfbHeight
                     : (HorVer.FBMode == VI_XFBMODE_SF)          ? (u16)(rmo->xfbHeight << 1)
                                                                 : rmo->xfbHeight;

    HorVer.threeD = (HorVer.nonInter == 3);

    tm = getTiming((VITVMode)((HorVer.tv << 2) + HorVer.nonInter));
    HorVer.timing = tm;

    HorVer.AdjustedDispPosX =
        (u16)CLAMP_VI((s16)HorVer.DispPosX + displayOffsetH, 0, 0x2D0 - HorVer.DispSizeX);

    coeff = (HorVer.FBMode == VI_XFBMODE_SF) ? 2 : 1;
    frac = HorVer.DispPosY & 1;

    HorVer.AdjustedDispPosY = (u16)MAX((s16)HorVer.DispPosY + displayOffsetV, frac);

    {
        s32 t0 = (s16)tm->acv * 2 - frac;
        s32 adjB = (s16)HorVer.DispPosY + (s16)HorVer.DispSizeY + displayOffsetV - t0;
        s32 adjA = (s16)HorVer.DispPosY + displayOffsetV - frac;

        HorVer.AdjustedDispSizeY = (u16)(HorVer.DispSizeY + MIN(adjA, 0) - MAX(adjB, 0));
        HorVer.AdjustedPanPosY = (u16)(HorVer.PanPosY - MIN(adjA, 0) / coeff);
        HorVer.AdjustedPanSizeY =
            (u16)(HorVer.PanSizeY + MIN(adjA, 0) / coeff - MAX(adjB, 0) / coeff);
    }

    {
        u16 odd = (u16)(tm->nhlines % 2);
        hct = odd ? tm->hlw : 0;
    }
    vct = (u16)(tm->nhlines / 2 + 1) | 0x1000;

    regs[VI_DI0_L] = (u16)(hct + 1);
    MARK_CHANGED(VI_DI0_L);
    regs[VI_DI0_H] = vct;
    MARK_CHANGED(VI_DI0_H);

    {
        u32 dcr = regs[VI_DCR];
        u32 viclk = regs[VI_VICLK];

        if (HorVer.nonInter == 2 || HorVer.nonInter == 3) {
            dcr = (dcr & ~0x4) | 0x4;
            if (HorVer.tv == 8) {
                viclk = viclk & ~0x1;
            } else {
                viclk = (viclk & ~0x1) | 0x1;
            }
        } else {
            dcr = (dcr & ~0x4) | ((HorVer.nonInter & 1) << 2);
            viclk = viclk & ~0x1;
        }

        dcr = (dcr & ~0x8) | (HorVer.threeD << 3);

        if (HorVer.tv == 1 || HorVer.tv == 2 || HorVer.tv == 3) {
            dcr = (dcr & ~0x300) | (HorVer.tv << 8);
        } else {
            dcr = dcr & ~0x300;
        }

        regs[VI_DCR] = (u16)dcr;
        MARK_CHANGED(VI_DCR);
        regs[VI_VICLK] = (u16)viclk;
        MARK_CHANGED(VI_VICLK);
    }

    {
        u16 panSizeX = HorVer.PanSizeX;
        w = (u16)(HorVer.threeD ? panSizeX * 2 : panSizeX);
    }

    if (w < HorVer.DispSizeX) {
        scale = (((u32)w << 8) + (u32)HorVer.DispSizeX - 1) / (u32)HorVer.DispSizeX;
        regs[VI_HSR] = (u16)(scale | 0x1000);
        MARK_CHANGED(VI_HSR);
        regs[VI_0x70] = (u16)w;
        MARK_CHANGED(VI_0x70);
    } else {
        regs[VI_HSR] = 0x100;
        MARK_CHANGED(VI_HSR);
    }

    setHorizontalRegs(tm, HorVer.AdjustedDispPosX, HorVer.DispSizeX);

    regs[VI_BBEI_L] = (u16)(tm->bs1 | (tm->be1 << 5));
    MARK_CHANGED(VI_BBEI_L);
    regs[VI_BBEI_H] = (u16)(tm->bs3 | (tm->be3 << 5));
    MARK_CHANGED(VI_BBEI_H);
    regs[VI_BBOI_L] = (u16)(tm->bs2 | (tm->be2 << 5));
    MARK_CHANGED(VI_BBOI_L);
    regs[VI_BBOI_H] = (u16)(tm->bs4 | (tm->be4 << 5));
    MARK_CHANGED(VI_BBOI_H);

    wordPerLine = (u8)((HorVer.FBSizeX + 15) / 16);
    if (HorVer.FBMode == VI_XFBMODE_SF) {
        std = wordPerLine;
    } else {
        std = (u8)(2 * wordPerLine);
    }
    xof = (u8)(HorVer.PanPosX % 16);

    HorVer.wordPerLine = wordPerLine;
    HorVer.std = std;
    HorVer.xof = xof;

    wpl = (u8)((xof + HorVer.PanSizeX + 15) / 16);
    HorVer.wpl = wpl;

    regs[VI_HSW] = (u16)((std) | (wpl << 8));
    MARK_CHANGED(VI_HSW);

    if (FBSet) {
        setFbbRegs(&HorVer, &HorVer.tfbb, &HorVer.bfbb, &HorVer.rtfbb, &HorVer.rbfbb);
    }

    setVerticalRegs(HorVer.AdjustedDispPosY, HorVer.AdjustedDispSizeY, tm->equ, tm->acv,
                    tm->prbOdd, tm->prbEven, tm->psbOdd, tm->psbEven, HorVer.black);
    OSRestoreInterrupts(enabled);
}


void VIConfigurePan(u16 x, u16 y, u16 w, u16 h) {
    timing_s* tm;
    BOOL enabled;
    s32 coeff;
    s32 frac;
    u16 acv;
    u8 wordPerLine;
    u8 std;
    u8 xof;
    u8 wpl;
    u32 v;
    u32 scale;

    enabled = OSDisableInterrupts();

    HorVer.PanPosX = x;
    HorVer.PanPosY = y;
    HorVer.PanSizeX = w;
    HorVer.PanSizeY = h;

    HorVer.DispSizeY = (HorVer.nonInter == 2)              ? h
                     : (HorVer.nonInter == 3)              ? h
                     : (HorVer.FBMode == VI_XFBMODE_SF)    ? (u16)(h << 1)
                                                           : h;

    tm = HorVer.timing;
    acv = tm->acv;

    HorVer.AdjustedDispPosX =
        (u16)CLAMP_VI((s16)HorVer.DispPosX + displayOffsetH, 0, 0x2D0 - HorVer.DispSizeX);

    frac = HorVer.DispPosY & 1;
    coeff = (HorVer.FBMode == VI_XFBMODE_SF) ? 2 : 1;

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

    {
        u16 panSizeX = HorVer.PanSizeX;
        v = panSizeX;
        if (HorVer.threeD) {
            v = panSizeX * 2;
        }
    }

    if (v < HorVer.DispSizeX) {
        scale = (((u32)v << 8) + (u32)HorVer.DispSizeX - 1) / (u32)HorVer.DispSizeX;
        regs[VI_HSR] = (u16)(scale | 0x1000);
        MARK_CHANGED(VI_HSR);
        regs[VI_0x70] = (u16)v;
        MARK_CHANGED(VI_0x70);
    } else {
        regs[VI_HSR] = 0x100;
        MARK_CHANGED(VI_HSR);
    }

    wordPerLine = (u8)((HorVer.FBSizeX + 15) / 16);
    HorVer.wordPerLine = wordPerLine;
    if (HorVer.FBMode != VI_XFBMODE_SF) {
        std = (u8)(2 * wordPerLine);
    } else {
        std = (u8)wordPerLine;
    }
    xof = (u8)(HorVer.PanPosX % 16);

    wpl = (u8)((xof + HorVer.PanSizeX + 15) / 16);
    HorVer.std = std;
    HorVer.xof = xof;
    HorVer.wpl = wpl;

    regs[VI_HSW] = (u16)(std | (wpl << 8));
    MARK_CHANGED(VI_HSW);

    if (FBSet) {
        setFbbRegs(&HorVer, &HorVer.tfbb, &HorVer.bfbb, &HorVer.rtfbb, &HorVer.rbfbb);
    }

    setVerticalRegs(HorVer.AdjustedDispPosY, HorVer.DispSizeY, tm->equ, tm->acv, tm->prbOdd,
                    tm->prbEven, tm->psbOdd, tm->psbEven, HorVer.black);
    OSRestoreInterrupts(enabled);
}


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
