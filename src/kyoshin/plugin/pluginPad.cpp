#include "kyoshin/plugin/pluginPad.hpp"
#include "monolib/core.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

extern "C" {
    extern u32 gmPauseFlags;
}

// name-guess: retail .data:0x80526690 is the pad plugin's VM command table.
static PluginFuncData sPadFuncData[3] = {
    { "get", pad_get },
    { "enable", pad_enable },
    { 0, 0 },
};

int pad_get(VMThread* pThread) {
    ButtonFlagsType type;
    VMArg arg;

    if (vmArgOmitChk(pThread, 1)) {
        type = BUTTON_FLAGS_PRESSED;
    } else {
        type = (ButtonFlagsType)vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    }

    switch (type) {
    case BUTTON_FLAGS_HELD:
        arg.type = VM_TYPE_INT;
        arg.value.uintVal = CPadManager::getMainPad()->mHeldButtonFlags;
        break;
    case BUTTON_FLAGS_PRESSED:
        arg.type = VM_TYPE_INT;
        arg.value.uintVal = CPadManager::getMainPad()->mPressedButtonFlags;
        break;
    case BUTTON_FLAGS_TURBO:
        arg.type = VM_TYPE_INT;
        arg.value.uintVal = CPadManager::getMainPad()->mTurboPressButtonFlags;
        break;
    default:
        vmPluginExceptionThrow(pThread);
        return 0;
    }

    vmRetValSet(pThread, &arg);
    return 1;
}

int pad_enable(VMThread* pThread) {
    u32 enableFlags = vmArgIntGet(2, vmArgPtrGet(pThread, 1));
    BOOL enable = vmArgBoolGet(3, vmArgPtrGet(pThread, 2));
    
    if (!(gmPauseFlags & 0x01000000)) {
        bool dontEnable = enable == false;
        cf::CfGameManager::enablePadFlags(enableFlags, !dontEnable);

        u32 newFlags = gmPauseFlags & ~(1 << 17);
        if (dontEnable) {
            newFlags = gmPauseFlags | (1 << 17);
        }
        gmPauseFlags = newFlags;
    }
    return 0;
}

void pluginPadRegist() {
    vmPluginRegist("pad", sPadFuncData);
}
