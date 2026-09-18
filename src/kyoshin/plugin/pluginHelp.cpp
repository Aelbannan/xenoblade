// Decompiled functions for kyoshin/plugin/pluginHelp.
// Help system plugin registrations - checks resource availability,
// creates help entries, and manages pad input gating.

#include "kyoshin/plugin/pluginHelp.hpp"
#include "monolib/vm/yvm2.h"

extern "C" {
    extern u32 CtrlRemote_TouchBitByArg(u32 resourceId);
    extern void CtrlRemote_SetSharedBit(u32 destination, u32 value);
    extern void CUICfManager_queueTutorialMenu(u32 param0, u32 param1, u32 param2);
    extern u32 MenuTutorialIsCreated();
    extern void UIWin_CreatePTChange(u32 param0);
    extern void enablePadFlags__Q22cf13CfGameManagerFUlb(int, int);
}

/// Script command: open the help system for a specific resource (0x3340).
/// If the resource is not yet loaded, creates a help entry and loads it.
/// Returns 0 and sets wait mode if the help system is active.
int PlugHelp_QueueTutorial1(VMThread* pThread) {
    if (CtrlRemote_TouchBitByArg(0x3340) == 0) {
        CUICfManager_queueTutorialMenu(1, 0, 0);
        CtrlRemote_SetSharedBit(0x3340, 1);
    }

    if (MenuTutorialIsCreated() != 0) {
        vmWaitModeSet(pThread);
    }

    return 0;
}

/// Script command: open the help system for a specific resource (0x337D).
/// Mirrors PlugHelp_QueueTutorial1 but with different resource/help IDs.
int PlugHelp_QueueTutorial3E(VMThread* pThread) {
    if (CtrlRemote_TouchBitByArg(0x337D) == 0) {
        CUICfManager_queueTutorialMenu(0x3E, 0, 0);
        CtrlRemote_SetSharedBit(0x337D, 1);
    }

    if (MenuTutorialIsCreated() != 0) {
        vmWaitModeSet(pThread);
    }

    return 0;
}

/// Script command: close the help system. If the help system is currently
/// active, sets wait mode and returns 0. Otherwise, re-enables pad input
/// flags and clears the help state.
int PlugHelp_OpenPTChangeMenu(VMThread* pThread) {
    if (MenuTutorialIsCreated() != 0) {
        vmWaitModeSet(pThread);
        return 0;
    }

    enablePadFlags__Q22cf13CfGameManagerFUlb(-1, 1);
    UIWin_CreatePTChange(0);
    return 0;
}

extern "C" void pluginHelpRegist() {
    extern void vmPluginRegist(void*, void*);
    extern char lbl_eu_8051347C[];
    extern char lbl_eu_8053A498[];
    vmPluginRegist((void*)lbl_eu_8051347C, (void*)lbl_eu_8053A498);
}