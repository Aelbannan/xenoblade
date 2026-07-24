#include "kyoshin/plugin/pluginWait.hpp"
#include "monolib/device/CDeviceVI.hpp"

extern "C" {
    // Plugin name string ("wait") in .rodata
    extern char lbl_eu_804FA438[];
    // Function registration table: maps "wait_frame" script name to our handler
    extern PluginFuncData lbl_eu_80524BB8[];
}

/**
 * VM plugin: wait_frame for SB scripts.
 * Suspends the calling script thread for N frames.
 *
 * Uses a scaled counter in work slot 0 to track remaining time.
 * On first call: initialises counter = frameCount << 12.
 * On subsequent calls: decrements counter by 0x1000 (NTSC 60fps) or
 * 0x1333 (PAL 50fps), keeping the thread asleep while counter > 0.
 * The 0x1000/0x1333 ratio (≈ 50/60) normalises real time across
 * the two display standards.
 */
int wait_frame(VMThread* pThread) {
    int frameCount = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    int wkIdx = vmWkIdxGet(pThread);

    if (wkIdx == 0) {
        u32* wk = vmWkGet(pThread, 0);
        *wk = frameCount << 0xC;
        vmWkIdxSet(pThread, wkIdx + 1);
        vmWaitModeSet(pThread);
    } else {
        u32* wk = vmWkGet(pThread, 0);
        if (!CDeviceVI::isTvFormatPal()) {
            *wk -= 0x1000;
        } else {
            *wk -= 0x1333;
        }
        if ((s32)*wk > 0) {
            vmWaitModeSet(pThread);
        }
    }
    return 0;
}

// Registers the wait plugin with the VM, linking script function names
// to our C handler implementations.
void pluginWaitRegist(){
    vmPluginRegist(lbl_eu_804FA438, lbl_eu_80524BB8);
}
