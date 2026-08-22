// ocMsg plugin: script-facing "OcMsg" / "OcLog" object-code handlers for the
// monolib script VM. Each function is a getter/setter/selector callback
// registered with vmOCRegist() via the OCData tables at the bottom.

#include "kyoshin/plugin/ocMsg.hpp"
#include <types.h>

// ---------------------------------------------------------------------------
// Two-value ring (values live at obj+0x64 / obj+0x68)
// ---------------------------------------------------------------------------

// OC constructor: returns the shared OcMsg list header as a VM object value.
int func_8003A53C(VMThread* pThread, void* r4, s16 r5) {
    VMArg args;
    args.type = 9;
    args.unk2 = r5;
    args.value.pointerVal = func_8003A4E0();
    vmRetValSet(pThread, &args);
    return 1;
}

int func_8003A588(VMThread* pThread, void* target) {
    VMArg args;
    args.type = 3;
    args.value.uintVal = *(u32*)((char*)target + 0x64);
    vmRetValSet(pThread, &args);
    return 1;
}

int func_8003A5C0(VMThread* pThread, void* target) {
    VMArg args;
    args.type = 3;
    args.value.uintVal = *(u32*)((char*)target + 0x68);
    vmRetValSet(pThread, &args);
    return 1;
}

int func_8003A5F8(VMThread* pThread, void* target) {
    void* prop = vmOCPropertyGet(pThread);
    *(u32*)((char*)target + 0x64) = *(u32*)((char*)prop + 4);
    return 0;
}

int func_8003A630(VMThread* pThread, void* target) {
    void* prop = vmOCPropertyGet(pThread);
    *(u32*)((char*)target + 0x68) = *(u32*)((char*)prop + 4);
    return 0;
}

// Ring reset: clears indices and sets capacity = 9.
s32 func_8003A668(void*, OcMsgRingHdr* list) {
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

// Open item: bit-trick type computation (x = count^0xA; 1 + ((x>>1)-(x&0xA))>>31).
// Retail keeps x in r5 (count in r0); MWCC always reuses r0 for x (4-7 pure
// reg_swap depending on shape). Witness rejects the r0<->r5 permutation
// (register reuse in subf: dest == operand under rho). Refined 2026: the
// reversed inline source `(x & 0x0a) - (x >> 1)` reproduces retail's EXACT
// allocation and instruction stream (xori r5 / andi r0 / srawi r5) but the
// subf comes out `subf r0,r5,r0` (=r0-r5, NEGATED value) instead of retail's
// `subf r0,r0,r5` (=r5-r0) — a 1-instruction semantic trap. MWCC's subf
// encoding is source-driven: `a - b` -> `subf r0, b_reg, a_reg`. Retail =
// fI-shape ALLOCATION + forward source; the allocation is coupled to the
// subtraction operand order, so no correct-semantic forward shape yields
// x->r5 (probed: ~50 shapes, Wii/1.1 + GC/3.0a5.2 + GC/3.0a3.4, -O4,s,
// -ipa off, -func_align 16). Session 2 refutations: `h + (-(x&0xA))` is
// front-end canonicalized back to h-t (identical 4 swaps); `(x>>1) < (x&0xA)`
// compiles branchy at +12 bytes; keeping count live via h=(count>>1)^5 forces
// xori r5 but adds an extra xori (+4 OVER) and subf still encodes r0,r5,r0. Semantics: type = 1 + bit31((x>>1)-(x&0xA));
// "2 - bit31(reversed)" rescue breaks at v=0 (count 10/11).
// Session addendum (pi-batch-match): keeping count live past the xor DOES move
// x into r5 (h = (count>>1)^5 variant) but then h needs its own xori (+4 bytes,
// OVER budget) and subf still encodes r0,r5,r0. MWCC always schedules andi.
// first when both trees share x; with independent trees it preserves source
// order. Confirms coupling: unreachable under forward semantics.
// Open item (wall class 11): type = 1 + bit31((x>>1) - (x&0xA)), x = count^0xA.
// Retail keeps count in r0 and moves x to r5 (xori r5,r0), mask back into r0,
// shift into r5. MWCC's subf encoding is source-driven (a-b -> subf dst,b,a)
// and its allocation is coupled to the subtraction operand order: every
// correct-semantic forward shape yields x->r0 and subf r0,r5,r0 (4 pure reg
// swaps). Refuted across ~50 shapes / Wii 1.1 + GC 3.0a5.2/3.0a3.4, including:
// named temps m/h, compound xor-assign, independent CSE trees, reversed
// source (negates semantics), live-count variants (+4 bytes OVER). This body
// is the banked best draft (structural=0, mismatch=4).
int func_8003A68C(VMThread* pThread, void* target) {
    int count = *(int*)((char*)target + 0x10);
    int x = count ^ 0x0a;
    VMArg args;
    // Subtract into the t-slot: subf dest = t's register (retail writes r0).
    int t = x & 0x0a;
    t = (x >> 1) - t;
    *(u8*)&args.type = 1 + ((u32)t >> 31);
    vmRetValSet(pThread, &args);
    return 1;
}

// type = 1 when count has exactly one bit set (power of two), else 0:
// bit31(-count & ~count) is set iff count is not a power of two... inverted
// by the +1 borrow trick; matches retail's nand/neg/srawi sequence.
int func_8003A6D4(VMThread* pThread, void* target) {
    int count = *(int*)((char*)target + 0x10);
    VMArg args;
    *(u8*)&args.type = 1 + ((u32)((-count) & ~count) >> 31);
    vmRetValSet(pThread, &args);
    return 1;
}

// ---------------------------------------------------------------------------
// Ring push/pop, two-value variant (8-byte slots)
// ---------------------------------------------------------------------------

// Push obj+0x64/0x68 into the next ring slot.
s32 func_8003A714(s32 ret, OcMsgRingHdr* list) {
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
    char* obj = reinterpret_cast<char*>(list);
    char* slotBase = obj + newWriteIdx * 8;
    *reinterpret_cast<u32*>(slotBase + 0x14) = *reinterpret_cast<u32*>(obj + 0x64);
    *reinterpret_cast<u32*>(slotBase + 0x18) = *reinterpret_cast<u32*>(obj + 0x68);
    return 0;
}

// Pop the current ring slot into obj+0x64/0x68.
s32 func_8003A764(s32 ret, OcMsgRingHdr* list) {
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
    char* obj = reinterpret_cast<char*>(list);
    char* slotBase = obj + newReadIdx * 8;
    *reinterpret_cast<u32*>(obj + 0x64) = *reinterpret_cast<u32*>(slotBase + 0x14);
    *reinterpret_cast<u32*>(obj + 0x68) = *reinterpret_cast<u32*>(slotBase + 0x18);
    return 0;
}

// ---------------------------------------------------------------------------
// Three-value ring (values live at obj+0x8c / obj+0x90 / obj+0x94)
// ---------------------------------------------------------------------------

// OC constructor: returns the shared OcLog list header as a VM object value.
int func_8003A7B4(VMThread* pThread, void* r4, s16 r5) {
    VMArg args;
    args.type = 9;
    args.unk2 = r5;
    args.value.pointerVal = func_8003A4EC();
    vmRetValSet(pThread, &args);
    return 1;
}

int func_8003A800(VMThread* pThread, void* target) {
    VMArg args;
    args.type = 3;
    args.value.uintVal = *(u32*)((char*)target + 0x8C);
    vmRetValSet(pThread, &args);
    return 1;
}

int func_8003A838(VMThread* pThread, void* target) {
    VMArg args;
    args.type = 3;
    args.value.uintVal = *(u32*)((char*)target + 0x90);
    vmRetValSet(pThread, &args);
    return 1;
}

int func_8003A870(VMThread* pThread, void* target) {
    VMArg args;
    args.type = 3;
    args.value.uintVal = *(u32*)((char*)target + 0x94);
    vmRetValSet(pThread, &args);
    return 1;
}

int func_8003A8A8(VMThread* pThread, void* target) {
    void* prop = vmOCPropertyGet(pThread);
    *(u32*)((char*)target + 0x8C) = *(u32*)((char*)prop + 4);
    return 0;
}

int func_8003A8E0(VMThread* pThread, void* target) {
    void* prop = vmOCPropertyGet(pThread);
    *(u32*)((char*)target + 0x90) = *(u32*)((char*)prop + 4);
    return 0;
}

int func_8003A918(VMThread* pThread, void* target) {
    void* prop = vmOCPropertyGet(pThread);
    *(u32*)((char*)target + 0x94) = *(u32*)((char*)prop + 4);
    return 0;
}

// Push obj+0x8c/0x90/0x94 into the next 12-byte ring slot.
s32 func_8003A950(s32 ret, OcMsgRingHdr* list) {
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
    char* obj = reinterpret_cast<char*>(list);
    char* slotBase = obj + newWriteIdx * 12;
    *reinterpret_cast<u32*>(slotBase + 0x14) = *reinterpret_cast<u32*>(obj + 0x8c);
    *reinterpret_cast<u32*>(slotBase + 0x18) = *reinterpret_cast<u32*>(obj + 0x90);
    *reinterpret_cast<u32*>(slotBase + 0x1c) = *reinterpret_cast<u32*>(obj + 0x94);
    return 0;
}

// Pop the current 12-byte ring slot into obj+0x8c/0x90/0x94.
s32 func_8003A9A8(s32 ret, OcMsgRingHdr* list) {
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
    char* obj = reinterpret_cast<char*>(list);
    char* slotBase = obj + newReadIdx * 12;
    *reinterpret_cast<u32*>(obj + 0x8c) = *reinterpret_cast<u32*>(slotBase + 0x14);
    *reinterpret_cast<u32*>(obj + 0x90) = *reinterpret_cast<u32*>(slotBase + 0x18);
    *reinterpret_cast<u32*>(obj + 0x94) = *reinterpret_cast<u32*>(slotBase + 0x1c);
    return 0;
}

// ---------------------------------------------------------------------------
// Plugin registration
// ---------------------------------------------------------------------------

void ocMsgRegist() {
    vmOCRegist(lbl_eu_80524CE8);
    vmOCRegist(lbl_eu_80524D80);
}
