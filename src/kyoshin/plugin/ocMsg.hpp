#pragma once

#include "monolib/vm/yvm2.h"
#include "kyoshin/plugin/ocThread.hpp"

#ifdef __cplusplus
extern "C" {
#endif

// ---------------------------------------------------------------------------
// ocMsg plugin: script-facing "OcMsg" / "OcLog" object-code handlers.
//
// These are VM opcode callbacks registered with the script VM through OCData
// tables (see ocMsgRegist). Retail emits them as plain unmangled C-linkage
// symbols (they are addressed by name from data tables), so the declarations
// below use C linkage.
// ---------------------------------------------------------------------------

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

/* Owner object for the two-value ring: header + 8-byte slots + values at 0x64/0x68. */
struct OcMsgTwoValueObj {
    u32 wrap;       /* 0x00 */
    u32 capacity;   /* 0x04 */
    u32 readIdx;    /* 0x08 */
    u32 writeIdx;   /* 0x0C */
    u32 count;      /* 0x10 */
    u8 slots[0x50]; /* 0x14..0x63 (9*8 slots + pad to 0x64) */
    u32 value0;     /* 0x64 */
    u32 value1;     /* 0x68 */
};

/* Owner object for the three-value ring: values at 0x8C/0x90/0x94. */
struct OcMsgThreeValueObj {
    u32 wrap;       /* 0x00 */
    u32 capacity;   /* 0x04 */
    u32 readIdx;    /* 0x08 */
    u32 writeIdx;   /* 0x0C */
    u32 count;      /* 0x10 */
    u8 slots[0x78]; /* 0x14..0x8B (9*12 slots + pad to 0x8C) */
    u32 value0;     /* 0x8C */
    u32 value1;     /* 0x90 */
    u32 value2;     /* 0x94 */
};


/* Getters/setters for the two-value ring (offsets 0x64/0x68). */
int func_8003A53C(VMThread* pThread, void* r4, s16 r5);
int func_8003A588(VMThread* pThread, OcMsgTwoValueObj* target);
int func_8003A5C0(VMThread* pThread, OcMsgTwoValueObj* target);
int func_8003A5F8(VMThread* pThread, OcMsgTwoValueObj* target);
int func_8003A630(VMThread* pThread, OcMsgTwoValueObj* target);

/* Ring reset: clears indices and sets capacity = 9. */
s32 func_8003A668(void*, OcMsgRingHdr* list);

/* Typeof-style probes on the value at target+0x10. */
int func_8003A68C(VMThread* pThread, OcMsgRingHdr* target);
int func_8003A6D4(VMThread* pThread, OcMsgRingHdr* target);

/* Push/pop one 8-byte slot between the ring and obj+0x64/0x68. */
s32 func_8003A714(s32 ret, OcMsgRingHdr* list);
s32 func_8003A764(s32 ret, OcMsgRingHdr* list);

/* Same set for the three-value ring (offsets 0x8c/0x90/0x94). */
int func_8003A7B4(VMThread* pThread, void* r4, s16 r5);
int func_8003A800(VMThread* pThread, OcMsgThreeValueObj* target);
int func_8003A838(VMThread* pThread, OcMsgThreeValueObj* target);
int func_8003A870(VMThread* pThread, OcMsgThreeValueObj* target);
int func_8003A8A8(VMThread* pThread, OcMsgThreeValueObj* target);
int func_8003A8E0(VMThread* pThread, OcMsgThreeValueObj* target);
int func_8003A918(VMThread* pThread, OcMsgThreeValueObj* target);
s32 func_8003A950(s32 ret, OcMsgRingHdr* list);
s32 func_8003A9A8(s32 ret, OcMsgRingHdr* list);

void ocMsgRegist();

#ifdef __cplusplus
}
#endif

/* OCData registration tables (defined in another TU's data section). */
extern OCData lbl_eu_80524CE8[];
extern OCData lbl_eu_80524D80[];
