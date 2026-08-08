// Decompiled functions for kyoshin/plugin/pluginHelp.
// Help system plugin registrations - checks resource availability,
// creates help entries, and manages pad input gating.

#include "kyoshin/plugin/pluginHelp.hpp"
#include "monolib/vm/yvm2.h"

extern "C" {
    extern u32 func_8009CF8C(u32 resourceId);
    extern void func_8009D018(u32 destination, u32 value);
    extern void func_80134D18(u32 param0, u32 param1, u32 param2);
    extern u32 func_8029A658();
    extern void func_8013E8E0(u32 param0);
    extern void enablePadFlags__Q22cf13CfGameManagerFUlb(int, int);
}

/// Script command: open the help system for a specific resource (0x3340).
/// If the resource is not yet loaded, creates a help entry and loads it.
/// Returns 0 and sets wait mode if the help system is active.
int func_802AEBC4(VMThread* pThread) {
    if (func_8009CF8C(0x3340) == 0) {
        func_80134D18(1, 0, 0);
        func_8009D018(0x3340, 1);
    }

    if (func_8029A658() != 0) {
        vmWaitModeSet(pThread);
    }

    return 0;
}

/// Script command: open the help system for a specific resource (0x337D).
/// Mirrors func_802AEBC4 but with different resource/help IDs.
int func_802AEC30(VMThread* pThread) {
    if (func_8009CF8C(0x337D) == 0) {
        func_80134D18(0x3E, 0, 0);
        func_8009D018(0x337D, 1);
    }

    if (func_8029A658() != 0) {
        vmWaitModeSet(pThread);
    }

    return 0;
}

/// Script command: close the help system. If the help system is currently
/// active, sets wait mode and returns 0. Otherwise, re-enables pad input
/// flags and clears the help state.
int func_802AEC9C(VMThread* pThread) {
    if (func_8029A658() != 0) {
        vmWaitModeSet(pThread);
        return 0;
    }

    enablePadFlags__Q22cf13CfGameManagerFUlb(-1, 1);
    func_8013E8E0(0);
    return 0;
}

extern "C" void pluginHelpRegist() {
    extern void vmPluginRegist(void*, void*);
    extern char lbl_eu_8051347C[];
    extern char lbl_eu_8053A498[];
    vmPluginRegist((void*)lbl_eu_8051347C, (void*)lbl_eu_8053A498);
}