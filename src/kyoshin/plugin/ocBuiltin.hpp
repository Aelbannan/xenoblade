#pragma once

#include "monolib/vm/yvm2.h"

#ifdef __cplusplus
extern "C" {
#endif

int isExistProperty(VMThread* pThread);
int isExistSelector(VMThread* pThread);
int getOCName(VMThread* pThread);
void ocBuiltinRegist();

#ifdef __cplusplus
}
#endif
