// Decompiled functions for kyoshin/plugin/pluginTime.

#include "kyoshin/plugin/pluginTime.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/vm/yvm2.h"

// pluginTime getter: returns the value from CfGameManager::getCurrentSlotIndex() as an int.
int PlugTime_GetCurrentSlot(VMThread* pThread) {
    VMArg result;
    result.value.uintVal = cf::CfGameManager::getCurrentSlotIndex();
    result.type = VM_TYPE_INT;
    vmRetValSet(pThread, &result);
    return 1;
}

// pluginTime getter: returns the value from CfGameManager::getControllerWordA33C() masked to u16 as an int.
int PlugTime_GetControllerWord(VMThread* pThread) {
    VMArg result;
    u16 val = (u16)cf::CfGameManager::getControllerWordA33C();
    result.type = VM_TYPE_INT;
    result.value.uintVal = val;
    vmRetValSet(pThread, &result);
    return 1;
}

// name-guess: retail .data:0x80532348; named per the JP sPlugin<X>Funcs convention.
PluginFuncData sPluginTimeFuncs[] = {
    { "range", PlugTime_GetCurrentSlot },
    { "hour", PlugTime_GetControllerWord },
    { 0, 0 },
};

void pluginTimeRegist() {
    vmPluginRegist("time", sPluginTimeFuncs);
}
