#pragma once

#include "monolib/vm/yvm2.h"

#ifdef __cplusplus
extern "C" {
#endif

int isExistProperty(VMThread* pThread, int r4, OCData* pOC);
int isExistSelector(VMThread* pThread, int r4, OCData* pOC);
int getOCName(VMThread* pThread, int r4, OCData* pOC);
void ocBuiltinRegist();

#ifdef __cplusplus
}
#endif
