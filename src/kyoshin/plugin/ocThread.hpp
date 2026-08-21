#pragma once

#include "monolib/vm/yvm2.h"

/* BSS list headers (retail .bss); sinit fills the leading fields. */
/* Ring/list header shared with the ocMsg plugin (same layout as
 * OcMsgRingHdr): capacity is pre-set to 9 by the static initializer. */
struct OcThreadListHdr {
    u32 wrap;
    u32 capacity;
    u32 readIdx;
    u32 writeIdx;
    u32 count;
};

#ifdef __cplusplus
extern "C" {
#endif

/* VM library function not yet exposed by yvm2.h. */
BOOL vmOCRegist(OCData* pOC);

/* Plugin entry points (script-facing); bodies in ocThread.cpp. */
int thread(VMThread* pThread, int unused, u32 subType);
int start_8003A784(VMThread* pThread, u32 threadId);
int end_8003A7A8(VMThread* pThread, u32 threadId);
int isAlive_8003A7D0(VMThread* pThread, u32 threadId);
int join_8003A820(VMThread* pThread, u32 threadId);
int sleep_8003A864(VMThread* pThread, u32 threadId);
int wakeup_8003A88C(VMThread* pThread, u32 threadId);
void ocThreadRegist();

/* List-head accessors used by the script VM's thread bookkeeping. */
OcThreadListHdr* func_8003A4E0();
OcThreadListHdr* func_8003A4EC();
void sinit_8003A4F8();

#ifdef __cplusplus
}
#endif

extern OCData lbl_eu_80524C60;
extern OcThreadListHdr lbl_eu_805704C8;
extern OcThreadListHdr lbl_eu_80570538;
