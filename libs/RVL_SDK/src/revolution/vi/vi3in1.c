// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/vi/vi3in1
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void __VISetYUVSEL() {}

void __VISetFilter4EURGB60(u8 param1) {
    u8 buf[2];
    buf[0] = 0x6e;
    buf[1] = param1;
    __VISendI2CData(0xe0, buf, 2);
    WaitMicroTime(2);
}

void __VISetCGMS() {}

void __VISetWSS() {}

void __VISetClosedCaption() {}

void __VISetMacrovision() {}

void __VISetGammaImm() {}

void __VISetGamma1_0() {}

void __VISetGamma() {}

void VISetGamma() {}

void __VISetTrapFilter() {}

void VISetTrapFilter() {}

void __VISetRGBOverDrive() {}

void VISetRGBModeImm() {}

void __VISetRGBModeImm() {}

void __VISetRevolutionModeSimple() {}
