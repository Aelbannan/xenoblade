// RVL_SDK 4.2.145 VI 3-in-1 driver (private fork)
// High-level C reconstruction of retail vi3in1.c (release build Feb 27 2009).

#include <revolution/OS.h>
#include <revolution/VI.h>

// Defined in i2c.c.
s32 __VISendI2CData(u8 slaveAddr, u8* pData, s32 nBytes);
void WaitMicroTime(s32 usec);

// --- .sbss ---
u32 Vdac_Flag_Changed;
u32 __current_3in1_video_mode;
u32 __level;
s32 __gamma;
u32 __type;
s32 Vdac_Flag_Region;

// --- .sdata ---
u32 __tvType = 0xFF;
u8 __wd0 = 0xFF;
u8 __wd1 = 0xFF;
u8 __wd2 = 0xFF;
u8 __gp1 = 0xFF;
u8 __gp2 = 0xFF;
u8 __gp3 = 0xFF;
u8 __gp4 = 0xFF;
u8 __cc1 = 0xFF;
u8 __cc2 = 0xFF;
u8 __cc3 = 0xFF;
u8 __cc4 = 0xFF;
u8 __filter = 0xFF;

// --- .bss ---
u8 VIZeroACPType[0x30];

void __VISetYUVSEL(u32 sel) {
    u8 buf[2];
    u32 tvFormat;

    tvFormat = *(volatile u32*)0x800000CC;
    switch (tvFormat) {
    case 1:
    case 5:
        Vdac_Flag_Region = 2;
        break;
    case 2:
        Vdac_Flag_Region = 1;
        break;
    case 0:
        Vdac_Flag_Region = 0;
        break;
    default:
        Vdac_Flag_Region = 0;
        break;
    }

    buf[0] = 1;
    buf[1] = (u8)sel << 5 | (u8)Vdac_Flag_Region;
    __VISendI2CData(0xE0, buf, 2);
    WaitMicroTime(2);
}

void __VISetFilter4EURGB60(u8 param1) {
    u8 buf[2];

    buf[0] = 0x6e;
    buf[1] = param1;
    __VISendI2CData(0xE0, buf, 2);
    WaitMicroTime(2);
}

void __VISetCGMS(void) {
    u8 buf[3];

    buf[0] = 5;
    buf[1] = __wd0 & 3;
    buf[1] |= (__wd1 & 0xF) << 2;
    buf[2] = __wd2;
    __VISendI2CData(0xE0, buf, 3);
    WaitMicroTime(2);
}

void __VISetWSS(void) {
    u8 buf[3];

    buf[0] = 8;
    buf[1] = __gp1 & 0xF;
    buf[1] |= (__gp2 & 0xF) << 4;
    buf[2] = __gp3 & 7;
    buf[2] |= (__gp4 & 7) << 3;
    __VISendI2CData(0xE0, buf, 3);
    WaitMicroTime(2);
}

void __VISetClosedCaption(void) {
    u8 buf[5];

    buf[0] = 0x7A;
    buf[1] = __cc1 & 0x7F;
    buf[2] = __cc2 & 0x7F;
    buf[3] = __cc3 & 0x7F;
    buf[4] = __cc4 & 0x7F;
    __VISendI2CData(0xE0, buf, 5);
    WaitMicroTime(2);
}

void __VISetMacrovision() {}

void __VISetGammaImm() {}

void __VISetGamma1_0() {}

void __VISetGamma() {}

void VISetGamma(VIGamma gamma) {
    if (__gamma == gamma) {
        return;
    }
    __gamma = gamma;
    Vdac_Flag_Changed |= 0x10;
}

void __VISetTrapFilter(void) {
    u8 buf[2];

    buf[0] = 3;
    if (__filter == 1) {
        buf[1] = 0;
    } else {
        buf[1] = 1;
    }
    __VISendI2CData(0xE0, buf, 2);
    WaitMicroTime(2);
}

void VISetTrapFilter(VIBool filter) {
    if ((u32)__filter == (u32)filter) {
        return;
    }
    __filter = filter;
    Vdac_Flag_Changed |= 0x20;
}

void __VISetRGBOverDrive(void) {
    u8 buf[2];

    if (Vdac_Flag_Region == 3) {
        buf[0] = 0xA;
        buf[1] = (__level << 1) | 1;
        __VISendI2CData(0xE0, buf, 2);
        WaitMicroTime(2);
    } else {
        buf[0] = 0xA;
        buf[1] = 0;
        __VISendI2CData(0xE0, buf, 2);
        WaitMicroTime(2);
    }
}

void VISetRGBModeImm(void) {
    Vdac_Flag_Changed |= 0x80;
}

void __VISetRGBModeImm(void) {
    u8 buf[2];

    Vdac_Flag_Region = 3;
    buf[0] = 1;
    buf[1] = 3;
    __VISendI2CData(0xE0, buf, 2);
    WaitMicroTime(2);
}

void __VISetRevolutionModeSimple() {}
