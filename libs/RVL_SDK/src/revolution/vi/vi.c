// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/vi/vi
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void OnShutdown() {}

void __VIRetraceHandler() {}

void* VISetPreRetraceCallback(void* callback) {
    extern void* PreCB;
    void* prev = PreCB;
    int enabled = OSDisableInterrupts();
    PreCB = callback;
    OSRestoreInterrupts(enabled);
    return prev;
}

void* VISetPostRetraceCallback(void* callback) {
    extern void* PostCB;
    void* old = PostCB;
    unsigned int level = OSDisableInterrupts();
    PostCB = callback;
    OSRestoreInterrupts(level);
    return old;
}

void getTiming() {}

void __VIInit() {}

void VIInit() {}

void VIWaitForRetrace() {}

void setFbbRegs() {}

void setHorizontalRegs() {}

void setVerticalRegs() {}

void VIConfigure() {}

void VIConfigurePan() {}

void VIFlush() {}

void VISetNextFrameBuffer() {}

void VISetBlack() {}

u32 VIGetRetraceCount(void) {
    extern u32 retraceCount;
    return retraceCount;
}

void VIGetNextField() {}

void VIGetCurrentLine() {}

void VIGetTvFormat() {}

void VIGetScanMode() {}

void VIGetDTVStatus() {}

void __VIDisplayPositionToXY() {}

void VIGetDimmingCount() {}

void VIEnableDimming() {}

void VISetTimeToDimming() {}

void VIResetDimmingCount() {}

void __VIResetRFIdle() {}
