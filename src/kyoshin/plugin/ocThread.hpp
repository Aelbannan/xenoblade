#pragma once

#include "monolib/vm/yvm2.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Plugin entry points (script-facing); bodies in ocThread.cpp. */
int thread(VMThread* pThread, int unused, u32 unk2);
int start_8003A784(VMThread* pThread, u32 r4);
int end_8003A7A8(VMThread* pThread, u32 threadId);
int isAlive_8003A7D0(VMThread* pThread, u32 threadId);
int join_8003A820(VMThread* pThread, u32 threadId);
int sleep_8003A864(VMThread* pThread, u32 threadId);
int wakeup_8003A88C(VMThread* pThread, u32 threadId);
void ocThreadRegist();

#ifdef __cplusplus
}
#endif
