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

/* Full retail BSS storage for the message/thread rings. Each derives from
 * OcThreadListHdr (base at offset 0, so derived-to-base conversion is
 * address-preserving and sinit/getOcMsgList/getOcLogList codegen is
 * unchanged), followed by ring slots and mirrored values (cf.
 * OcMsgTwoValueObj/OcMsgThreeValueObj in ocMsg.hpp). Sizes match the retail
 * .bss exactly (0x70/0x98); the message ring's trailing word is unobserved
 * by code and exists only to reach the retail size. */
struct OcThreadMsgStorage : OcThreadListHdr {
    u8 slots[0x50]; /* +0x14 ring slots (mirrors OcMsgTwoValueObj layout) */
    u32 value0;     /* +0x64 */
    u32 value1;     /* +0x68 */
    u32 _sizePad;   /* +0x6C: filler to retail 0x70 */
};
struct OcThreadLogStorage : OcThreadListHdr {
    u8 slots[0x78]; /* +0x14 ring slots (mirrors OcMsgThreeValueObj layout) */
    u32 value0;     /* +0x8C */
    u32 value1;     /* +0x90 */
    u32 value2;     /* +0x94 */
}; /* total 0x98, no pad needed */

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
OcThreadListHdr* getOcMsgList();
OcThreadListHdr* getOcLogList();
void sinit_8003A4F8();

#ifdef __cplusplus
}
#endif

extern OCData lbl_eu_80524C60;
extern OcThreadMsgStorage lbl_eu_805704C8;
extern OcThreadLogStorage lbl_eu_80570538;
