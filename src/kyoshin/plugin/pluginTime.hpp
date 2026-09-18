#pragma once

#include "monolib/vm/yvm2.h"

#ifdef __cplusplus
extern "C" {
#endif

int PlugTime_GetCurrentSlot(VMThread* pThread);
int PlugTime_GetControllerWord(VMThread* pThread);
void pluginTimeRegist();

#ifdef __cplusplus
}
#endif
