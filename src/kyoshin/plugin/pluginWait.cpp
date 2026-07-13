#include "kyoshin/plugin/pluginWait.hpp"
#include "monolib/device/CDeviceVI.hpp"

extern "C" {
    extern char lbl_eu_804FA438[];
    extern PluginFuncData lbl_eu_80524BB8[];
}

int wait_frame(VMThread* pThread) {
    int temp_r30 = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    int wkIdx = vmWkIdxGet(pThread);

    if (wkIdx == 0) {
        u32* wk = vmWkGet(pThread, 0);
        *wk = temp_r30 << 0xC;
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

void pluginWaitRegist(){
    vmPluginRegist(lbl_eu_804FA438, lbl_eu_80524BB8);
}
