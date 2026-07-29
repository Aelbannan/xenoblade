// Decompiled functions for kyoshin/plugin/pluginTime.

#include "kyoshin/plugin/pluginTime.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/vm/yvm2.h"

extern "C" {
extern const char lbl_eu_80503818[];
extern PluginFuncData lbl_eu_80532348[];
}

// pluginTime getter: returns the value from CfGameManager::func_80086DBC() as an int.
int func_80185760(VMThread* pThread) {
    VMArg result;
    result.value.uintVal = cf::CfGameManager::func_80086DBC();
    result.type = VM_TYPE_INT;
    vmRetValSet(pThread, &result);
    return 1;
}

// pluginTime getter: returns the value from CfGameManager::func_80086DA0() masked to u16 as an int.
int func_801857A8(VMThread* pThread) {
    VMArg result;
    u16 val = (u16)cf::CfGameManager::func_80086DA0();
    result.value.uintVal = val;
    result.type = VM_TYPE_INT;
    vmRetValSet(pThread, &result);
    return 1;
}

void pluginTimeRegist() {
    vmPluginRegist(lbl_eu_80503818, lbl_eu_80532348);
}
