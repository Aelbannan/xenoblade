// ocMsg plugin: script-facing "OcMsg" / "OcLog" object-code handlers for the
// monolib script VM. Each function is a getter/setter/selector callback
// registered with vmOCRegist() via the OCData tables at the bottom.

#include "kyoshin/plugin/ocMsg.hpp"
#include <types.h>

// ---------------------------------------------------------------------------
// Two-value ring (values live at obj+0x64 / obj+0x68)
// ---------------------------------------------------------------------------

// OC constructor: returns the shared OcMsg list header as a VM object value.
int OcMsgMakeListValue(VMThread* pThread, void*, s16 argType) {
    VMArg args;
    args.type = 9;
    // VMArg::unk2 carries the caller's package index.
    args.unk2 = static_cast<u16>(argType);
    args.value.pointerVal = getOcMsgList();
    vmRetValSet(pThread, &args);
    return 1;
}

int OcMsgGetValue0(VMThread* pThread, OcMsgTwoValueObj* target) {
    VMArg args;
    args.type = 3;
    args.value.uintVal = target->value0;
    vmRetValSet(pThread, &args);
    return 1;
}

int OcMsgGetValue1(VMThread* pThread, OcMsgTwoValueObj* target) {
    VMArg args;
    args.type = 3;
    args.value.uintVal = target->value1;
    vmRetValSet(pThread, &args);
    return 1;
}

int OcMsgSetValue0(VMThread* pThread, OcMsgTwoValueObj* target) {
    VMArg* prop = (VMArg*)vmOCPropertyGet(pThread);
    target->value0 = prop->value.uintVal;
    return 0;
}

int OcMsgSetValue1(VMThread* pThread, OcMsgTwoValueObj* target) {
    VMArg* prop = (VMArg*)vmOCPropertyGet(pThread);
    target->value1 = prop->value.uintVal;
    return 0;
}

// Ring reset: clears indices and sets capacity = 9.
s32 OcMsgRingReset(void*, OcMsgRingHdr* list) {
    list->count = 0;
    list->wrap = 0;
    list->writeIdx = 0;
    list->readIdx = 0;
    list->capacity = 9;
    return 0;
}

// ---------------------------------------------------------------------------
// Typeof-style probes on the counter at target+0x10
// ---------------------------------------------------------------------------
// type = 2 while the ring holds fewer than 10 messages, else 1.
int OcMsgRingProbeOverflow(VMThread* pThread, OcMsgRingHdr* target) {
    VMArg args;
    *(u8*)&args.type = 1 + ((int)target->count < 10 ? 1 : 0);
    vmRetValSet(pThread, &args);
    return 1;
}

// type = 1 when count has exactly one bit set (power of two), else 0:
// bit31(-count & ~count) is set iff count is not a power of two... inverted
// by the +1 borrow trick; matches retail's nand/neg/srawi sequence.
int OcMsgRingProbeIsPow2(VMThread* pThread, OcMsgRingHdr* target) {
    int count = (int)target->count;
    VMArg args;
    *(u8*)&args.type = 1 + ((u32)((-count) & ~count) >> 31);
    vmRetValSet(pThread, &args);
    return 1;
}

// ---------------------------------------------------------------------------
// Ring push/pop, two-value variant (8-byte slots)
// ---------------------------------------------------------------------------

// Push obj+0x64/0x68 into the next ring slot.
s32 OcMsgRingPushTwo(s32 ret, OcMsgRingHdr* list) {
    // Reuse `ret` for writeIdx then count so +1 lands in a distinct reg (retail addi r5,r3,1).
    // Signed compare → cmpw (u32 > emits cmplw).
    ret = list->writeIdx;
    s32 capacity = list->capacity;
    s32 newWriteIdx = ret + 1;
    ret = list->count;
    list->writeIdx = newWriteIdx;
    list->count = ret + 1;
    if (newWriteIdx > capacity) {
        list->writeIdx = list->wrap;
    }
    newWriteIdx = list->writeIdx;
    OcMsgTwoValueObj* obj = reinterpret_cast<OcMsgTwoValueObj*>(list);
    u32* slot = reinterpret_cast<u32*>(obj->slots + newWriteIdx * 8);
    slot[0] = obj->value0;
    slot[1] = obj->value1;
    return 0;
}

// Pop the current ring slot into obj+0x64/0x68.
s32 OcMsgRingPopTwo(s32 ret, OcMsgRingHdr* list) {
    ret = list->readIdx;
    s32 capacity = list->capacity;
    s32 newReadIdx = ret + 1;
    ret = list->count;
    list->readIdx = newReadIdx;
    list->count = ret - 1;
    if (newReadIdx > capacity) {
        list->readIdx = list->wrap;
    }
    newReadIdx = list->readIdx;
    OcMsgTwoValueObj* obj = reinterpret_cast<OcMsgTwoValueObj*>(list);
    u32* slot = reinterpret_cast<u32*>(obj->slots + newReadIdx * 8);
    obj->value0 = slot[0];
    obj->value1 = slot[1];
    return 0;
}

// ---------------------------------------------------------------------------
// Three-value ring (values live at obj+0x8c / obj+0x90 / obj+0x94)
// ---------------------------------------------------------------------------

// OC constructor: returns the shared OcLog list header as a VM object value.
int OcLogMakeListValue(VMThread* pThread, void*, s16 argType) {
    VMArg args;
    args.type = 9;
    // VMArg::unk2 carries the caller's package index.
    args.unk2 = static_cast<u16>(argType);
    args.value.pointerVal = getOcLogList();
    vmRetValSet(pThread, &args);
    return 1;
}

int OcLogGetValue0(VMThread* pThread, OcMsgThreeValueObj* target) {
    VMArg args;
    args.type = 3;
    args.value.uintVal = target->value0;
    vmRetValSet(pThread, &args);
    return 1;
}

int OcLogGetValue1(VMThread* pThread, OcMsgThreeValueObj* target) {
    VMArg args;
    args.type = 3;
    args.value.uintVal = target->value1;
    vmRetValSet(pThread, &args);
    return 1;
}

int OcLogGetValue2(VMThread* pThread, OcMsgThreeValueObj* target) {
    VMArg args;
    args.type = 3;
    args.value.uintVal = target->value2;
    vmRetValSet(pThread, &args);
    return 1;
}

int OcLogSetValue0(VMThread* pThread, OcMsgThreeValueObj* target) {
    VMArg* prop = (VMArg*)vmOCPropertyGet(pThread);
    target->value0 = prop->value.uintVal;
    return 0;
}

int OcLogSetValue1(VMThread* pThread, OcMsgThreeValueObj* target) {
    VMArg* prop = (VMArg*)vmOCPropertyGet(pThread);
    target->value1 = prop->value.uintVal;
    return 0;
}

int OcLogSetValue2(VMThread* pThread, OcMsgThreeValueObj* target) {
    VMArg* prop = (VMArg*)vmOCPropertyGet(pThread);
    target->value2 = prop->value.uintVal;
    return 0;
}

// Push obj+0x8c/0x90/0x94 into the next 12-byte ring slot.
s32 OcLogRingPushThree(s32 ret, OcMsgRingHdr* list) {
    ret = list->writeIdx;
    s32 capacity = list->capacity;
    s32 newWriteIdx = ret + 1;
    ret = list->count;
    list->writeIdx = newWriteIdx;
    list->count = ret + 1;
    if (newWriteIdx > capacity) {
        list->writeIdx = list->wrap;
    }
    newWriteIdx = list->writeIdx;
    OcMsgThreeValueObj* obj = reinterpret_cast<OcMsgThreeValueObj*>(list);
    u32* slot = reinterpret_cast<u32*>(obj->slots + newWriteIdx * 12);
    slot[0] = obj->value0;
    slot[1] = obj->value1;
    slot[2] = obj->value2;
    return 0;
}

// Pop the current 12-byte ring slot into obj+0x8c/0x90/0x94.
s32 OcLogRingPopThree(s32 ret, OcMsgRingHdr* list) {
    ret = list->readIdx;
    s32 capacity = list->capacity;
    s32 newReadIdx = ret + 1;
    ret = list->count;
    list->readIdx = newReadIdx;
    list->count = ret - 1;
    if (newReadIdx > capacity) {
        list->readIdx = list->wrap;
    }
    newReadIdx = list->readIdx;
    OcMsgThreeValueObj* obj = reinterpret_cast<OcMsgThreeValueObj*>(list);
    u32* slot = reinterpret_cast<u32*>(obj->slots + newReadIdx * 12);
    obj->value0 = slot[0];
    obj->value1 = slot[1];
    obj->value2 = slot[2];
    return 0;
}

// ---------------------------------------------------------------------------
// Plugin registration
// ---------------------------------------------------------------------------

void ocMsgRegist() {
    vmOCRegist(lbl_eu_80524CE8);
    vmOCRegist(lbl_eu_80524D80);
}

// ----- Retail data sections (all defined by this TU) -----
// .sdata2 entry names (each sized to its retail slot for a deterministic
// 0x50), .data OC property/selector tables + the two OCData roots (80524CE8
// "msgYuka", 80524D80 "msgNpc", the frontier labels). Length words are 0
// in retail; null entries terminate each table. .data pieces after the first
// land on 8-byte boundaries (retail layout; MWCC places them so). Casts
// bridge the VM table types and the concrete handlers; only addresses emit.
__declspec(section ".sdata2") __attribute__((used))
char lbl_eu_80665BD0[] = "atr";
__declspec(section ".sdata2") __attribute__((used))
char lbl_eu_80665BD4[] = "no";
__declspec(section ".sdata2") __attribute__((used))
char lbl_eu_80665BD8[] = "send";
__declspec(section ".sdata2") __attribute__((used))
char lbl_eu_80665BE0[] = "receive";
__declspec(section ".sdata2") __attribute__((used))
char lbl_eu_80665BE8[] = "init";
__declspec(section ".sdata2") __attribute__((used))
char lbl_eu_80665BF0[] = "isFull";
__declspec(section ".sdata2") __attribute__((used))
char lbl_eu_80665BF8[] = "isEmpty";
__declspec(section ".sdata2") __attribute__((used))
char lbl_eu_80665C00[] = "msgYuka";
__declspec(section ".sdata2") __attribute__((used))
char lbl_eu_80665C08[] = "id";
__declspec(section ".sdata2") __attribute__((used))
char lbl_eu_80665C0C[] = "talk";
__declspec(section ".sdata2") __attribute__((used))
char lbl_eu_80665C14[] = "dir";
__declspec(section ".sdata2") __attribute__((used)) char lbl_eu_80665C18[8] = "msgNpc";
__declspec(section ".data") __attribute__((used)) OCProperty lbl_eu_80524C70[3] = {
    { lbl_eu_80665BD0, (OCGetSetFunc)OcMsgGetValue0, (OCGetSetFunc)OcMsgSetValue0, 0 },
    { lbl_eu_80665BD4, (OCGetSetFunc)OcMsgGetValue1, (OCGetSetFunc)OcMsgSetValue1, 0 },
    { 0, 0, 0, 0 },
};
__declspec(section ".data") __attribute__((used)) OCSelector lbl_eu_80524CA0[6] = {
    { lbl_eu_80665BD8, (OCSelectorFunc)OcMsgRingPushTwo, 0 },
    { lbl_eu_80665BE0, (OCSelectorFunc)OcMsgRingPopTwo, 0 },
    { lbl_eu_80665BE8, (OCSelectorFunc)OcMsgRingReset, 0 },
    { lbl_eu_80665BF0, (OCSelectorFunc)OcMsgRingProbeOverflow, 0 },
    { lbl_eu_80665BF8, (OCSelectorFunc)OcMsgRingProbeIsPow2, 0 },
    { 0, 0, 0 },
};
__declspec(section ".data") __attribute__((used)) OCData lbl_eu_80524CE8[1] = {
    { lbl_eu_80665C00, (OCCtorFunc)OcMsgMakeListValue, lbl_eu_80524C70, lbl_eu_80524CA0 },
};
__declspec(section ".data") __attribute__((used)) OCProperty lbl_eu_80524CF8[4] = {
    { lbl_eu_80665C08, (OCGetSetFunc)OcLogGetValue0, (OCGetSetFunc)OcLogSetValue0, 0 },
    { lbl_eu_80665C0C, (OCGetSetFunc)OcLogGetValue1, (OCGetSetFunc)OcLogSetValue1, 0 },
    { lbl_eu_80665C14, (OCGetSetFunc)OcLogGetValue2, (OCGetSetFunc)OcLogSetValue2, 0 },
    { 0, 0, 0, 0 },
};
__declspec(section ".data") __attribute__((used)) OCSelector lbl_eu_80524D38[6] = {
    { lbl_eu_80665BD8, (OCSelectorFunc)OcLogRingPushThree, 0 },
    { lbl_eu_80665BE0, (OCSelectorFunc)OcLogRingPopThree, 0 },
    { lbl_eu_80665BE8, (OCSelectorFunc)OcMsgRingReset, 0 },
    { lbl_eu_80665BF0, (OCSelectorFunc)OcMsgRingProbeOverflow, 0 },
    { lbl_eu_80665BF8, (OCSelectorFunc)OcMsgRingProbeIsPow2, 0 },
    { 0, 0, 0 },
};
__declspec(section ".data") __attribute__((used)) OCData lbl_eu_80524D80[1] = {
    { lbl_eu_80665C18, (OCCtorFunc)OcLogMakeListValue, lbl_eu_80524CF8, lbl_eu_80524D38 },
};
