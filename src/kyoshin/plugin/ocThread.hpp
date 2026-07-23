#pragma once

#include "monolib/vm/yvm2.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Plugin entry points (script-facing); bodies in ocThread.cpp. */
int thread(VMThread* pThread);
int start(VMThread* pThread);
int end(VMThread* pThread);
int isAlive(VMThread* pThread);
int join(VMThread* pThread);
int sleep(VMThread* pThread);
int wakeup(VMThread* pThread);
void ocThreadRegist();

#ifdef __cplusplus
}
#endif
