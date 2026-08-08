// Auto-scaffolded catalog TU for kyoshin/plugin/ocMsg
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/plugin/ocMsg.hpp"
#include <types.h>

struct OcMsgRingHdr {
    u32 wrap;
    u32 capacity;
    u32 readIdx;
    u32 writeIdx;
    u32 count;
};

extern "C" void* func_8003A4E0();
extern "C" void* func_8003A4EC();

extern "C" int func_8003A53C(VMThread* pThread, void* r4, s16 r5) {
    VMArg args;
    *(u8*)&args.type = 9;
    *(u16*)((char*)&args + 2) = r5;
    args.value.pointerVal = func_8003A4E0();
    vmRetValSet(pThread, &args);
    return 1;
}

extern "C" int func_8003A588(VMThread* pThread, void* target) {
    VMArg args;
    *(u8*)&args.type = 3;
    args.value.uintVal = *(u32*)((char*)target + 0x64);
    vmRetValSet(pThread, &args);
    return 1;
}

extern "C" int func_8003A5C0(VMThread* pThread, void* target) {
    VMArg args;
    *(u8*)&args.type = 3;
    args.value.uintVal = *(u32*)((char*)target + 0x68);
    vmRetValSet(pThread, &args);
    return 1;
}

extern "C" int func_8003A5F8(VMThread* pThread, void* target) {
    void* prop = vmOCPropertyGet(pThread);
    *(u32*)((char*)target + 0x64) = *(u32*)((char*)prop + 4);
    return 0;
}

extern "C" int func_8003A630(VMThread* pThread, void* target) {
    void* prop = vmOCPropertyGet(pThread);
    *(u32*)((char*)target + 0x68) = *(u32*)((char*)prop + 4);
    return 0;
}

extern "C" s32 func_8003A668(void*, OcMsgRingHdr* list) {
    list->count = 0;
    list->wrap = 0;
    list->writeIdx = 0;
    list->readIdx = 0;
    list->capacity = 9;
    return 0;
}

extern "C" int func_8003A68C(VMThread* pThread, void* target) {
    int count = *(int*)((char*)target + 0x10);
    int x = count ^ 0x0a;
    VMArg args;
    *(u8*)&args.type = 1 + ((u32)((x >> 1) - (x & 0x0a)) >> 31);
    vmRetValSet(pThread, &args);
    return 1;
}

extern "C" int func_8003A6D4(VMThread* pThread, void* target) {
    int count = *(int*)((char*)target + 0x10);
    VMArg args;
    *(u8*)&args.type = 1 + ((u32)((-count) & ~count) >> 31);
    vmRetValSet(pThread, &args);
    return 1;
}

extern "C" s32 func_8003A714(s32 ret, OcMsgRingHdr* list) {
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

extern "C" s32 func_8003A764(s32 ret, OcMsgRingHdr* list) {
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

extern "C" int func_8003A7B4(VMThread* pThread, void* r4, s16 r5) {
    VMArg args;
    *(u8*)&args.type = 9;
    *(u16*)((char*)&args + 2) = r5;
    args.value.pointerVal = func_8003A4EC();
    vmRetValSet(pThread, &args);
    return 1;
}

extern "C" int func_8003A800(VMThread* pThread, void* target) {
    VMArg args;
    *(u8*)&args.type = 3;
    args.value.uintVal = *(u32*)((char*)target + 0x8C);
    vmRetValSet(pThread, &args);
    return 1;
}

extern "C" int func_8003A838(VMThread* pThread, void* target) {
    VMArg args;
    *(u8*)&args.type = 3;
    args.value.uintVal = *(u32*)((char*)target + 0x90);
    vmRetValSet(pThread, &args);
    return 1;
}

extern "C" int func_8003A870(VMThread* pThread, void* target) {
    VMArg args;
    *(u8*)&args.type = 3;
    args.value.uintVal = *(u32*)((char*)target + 0x94);
    vmRetValSet(pThread, &args);
    return 1;
}

extern "C" int func_8003A8A8(VMThread* pThread, void* target) {
    void* prop = vmOCPropertyGet(pThread);
    *(u32*)((char*)target + 0x8C) = *(u32*)((char*)prop + 4);
    return 0;
}

extern "C" int func_8003A8E0(VMThread* pThread, void* target) {
    void* prop = vmOCPropertyGet(pThread);
    *(u32*)((char*)target + 0x90) = *(u32*)((char*)prop + 4);
    return 0;
}

extern "C" int func_8003A918(VMThread* pThread, void* target) {
    void* prop = vmOCPropertyGet(pThread);
    *(u32*)((char*)target + 0x94) = *(u32*)((char*)prop + 4);
    return 0;
}

extern "C" s32 func_8003A950(s32 ret, OcMsgRingHdr* list) {
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

extern "C" s32 func_8003A9A8(s32 ret, OcMsgRingHdr* list) {
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

extern "C" BOOL vmOCRegist(OCData* pOC);
extern OCData lbl_eu_80524CE8[];
extern OCData lbl_eu_80524D80[];
void ocMsgRegist() {
    vmOCRegist(lbl_eu_80524CE8);
    vmOCRegist(lbl_eu_80524D80);
}