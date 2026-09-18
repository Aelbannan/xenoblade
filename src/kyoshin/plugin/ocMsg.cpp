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

// Open item (EQUIVALENT_MATCH via register-renaming witness, 2 residual reg
// swaps): computes type = 1 + bit31((x>>1) - (x&0xA)) with x = count ^ 0xA.
// Retail allocates count->r0 and x->r5 (subf r0,r0,r5); MWCC's subf encoding
// is source-driven (a-b -> subf dst,b,a) and its register allocation is
// coupled to the subtraction operand order, so every correct-semantic forward
// shape tried (~50 across Wii 1.1 + GC 3.0a5.2/3.0a3.4) recolors x back to
// r0 (4 pure reg swaps). Refuted shapes include: named temps m/h, compound
// xor-assign, independent CSE trees, reversed source `(x & 0x0a) - (x >> 1)`
// (reproduces retail's instruction stream but negates the subtraction - a
// 1-instruction semantic trap), live-count variants keeping count alive past
// the xor (moves x to r5 but adds an extra xori: +4 bytes over budget),
// mask = ~count & 0xA (+4 over), and the comparison form (branchy, +12 bytes).
// Refuted 2026-08 (register-mapping dead-reuse angle, docs/register_mapping.md):
// binding the difference to a FRESH web born after t - both a named `int u =
// t - (x & 0x0a)` and an anonymous `(u32)(t - (x & 0xa)) >> 31` in the store -
// recolors the whole prefix (xori/andi/srawi all swap x into r0) => 4 reg
// swaps vs the banked draft's 2. The subf destination does land in r0 in both,
// but only at the cost of the prefix; consistent with the doc's birth-order
// rule: any non-compound difference form makes x the later-born web.
// This body is the banked best draft.
int func_8003A68C(VMThread* pThread, OcMsgRingHdr* target) {
    VMArg args;
    int count = (int)target->count;
    int x = count ^ 0x0a;
    // Shift-initializing temp + compound subtract is the best-known shape:
    // it reproduces retail's xori/andi/srawi stream; only the subf destination
    // and final shift source remain swapped (r5 vs retail r0).
    int t = x >> 1;
    t -= x & 0x0a;
    *(u8*)&args.type = 1 + ((u32)t >> 31);
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
