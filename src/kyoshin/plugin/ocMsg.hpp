#pragma once

#include "monolib/vm/yvm2.h"
#include "kyoshin/plugin/ocThread.hpp"

#ifdef __cplusplus
extern "C" {
#endif

// ---------------------------------------------------------------------------
// ocMsg plugin: script-facing "OcMsg" / "OcLog" object-code handlers.
//
// These are VM opcode callbacks registered with the script VM through the
// OCData tables below (see ocMsgRegist). Retail keeps them as plain C-linkage
// symbols, hence the extern "C" block.
// ---------------------------------------------------------------------------

/* VM library function not yet exposed by yvm2.h. */
BOOL vmOCRegist(OCData* pOC);

/* Ring-buffer header shared by the message lists. The data slots follow the
 * header: `capacity`-many entries of 8 or 12 bytes starting at offset 0x14,
 * each holding one or three u32 values mirrored to/from the owner object at
 * offsets 0x64/0x68 (or 0x8c/0x90/0x94). */
struct OcMsgRingHdr {
    u32 wrap;
    u32 capacity;
    u32 readIdx;
    u32 writeIdx;
    u32 count;
};

/* Getters/setters for the two-value ring (offsets 0x64/0x68). */
int func_8003A53C(VMThread* pThread, void* r4, s16 r5);
int func_8003A588(VMThread* pThread, void* target);
int func_8003A5C0(VMThread* pThread, void* target);
int func_8003A5F8(VMThread* pThread, void* target);
int func_8003A630(VMThread* pThread, void* target);

/* Ring reset: clears indices and sets capacity = 9. */
s32 func_8003A668(void*, OcMsgRingHdr* list);

/* Typeof-style probes on the value at target+0x10. */
int func_8003A68C(VMThread* pThread, void* target);
int func_8003A6D4(VMThread* pThread, void* target);

/* Push/pop one 8-byte slot between the ring and obj+0x64/0x68. */
s32 func_8003A714(s32 ret, OcMsgRingHdr* list);
s32 func_8003A764(s32 ret, OcMsgRingHdr* list);

/* Same set for the three-value ring (offsets 0x8c/0x90/0x94). */
int func_8003A7B4(VMThread* pThread, void* r4, s16 r5);
int func_8003A800(VMThread* pThread, void* target);
int func_8003A838(VMThread* pThread, void* target);
int func_8003A870(VMThread* pThread, void* target);
int func_8003A8A8(VMThread* pThread, void* target);
int func_8003A8E0(VMThread* pThread, void* target);
int func_8003A918(VMThread* pThread, void* target);
s32 func_8003A950(s32 ret, OcMsgRingHdr* list);
s32 func_8003A9A8(s32 ret, OcMsgRingHdr* list);

void ocMsgRegist();

#ifdef __cplusplus
}
#endif

/* OCData registration tables (defined in another TU's data section). */
extern OCData lbl_eu_80524CE8[];
extern OCData lbl_eu_80524D80[];
