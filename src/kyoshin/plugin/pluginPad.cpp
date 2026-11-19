#include "kyoshin/plugin/pluginPad.hpp"
#include "monolib/core.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

extern "C" {
    // Retail data sections (all defined by this TU):
    // .rodata "pad" (plugin name) + .sdata2 "get"/"enable" (entry names)
    // + .data PluginFuncData table {name, func} terminated by {NULL, NULL}.
    __declspec(section ".rodata") __attribute__((used)) char lbl_eu_804FB1D8[0x8] = "pad";
    __declspec(section ".sdata2") __attribute__((used)) char lbl_eu_806661E0[0x4] = "get";
    __declspec(section ".sdata2") __attribute__((used)) char lbl_eu_806661E4[0xC] = "enable";
    __declspec(section ".data") __attribute__((used)) PluginFuncData lbl_eu_80526690[3] = {
        { lbl_eu_806661E0, pad_get },
        { lbl_eu_806661E4, pad_enable },
        { 0, 0 },
    };
    extern u32 gmPauseFlags;
}

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
    vmPluginRegist(lbl_eu_804FB1D8, lbl_eu_80526690);
}
