// Auto-scaffolded catalog TU for kyoshin/plugin/ocMsg
// Mangled extern stubs for llm-harness / coop selection.
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

// LLM-HARNESS-BEGIN: us-8003a91c
extern "C" void func_8003A53C() {}
// LLM-HARNESS-END: us-8003a91c

// LLM-HARNESS-BEGIN: us-8003a968
extern "C" void func_8003A588() {}
// LLM-HARNESS-END: us-8003a968

// LLM-HARNESS-BEGIN: us-8003a9a0
extern "C" void func_8003A5C0() {}
// LLM-HARNESS-END: us-8003a9a0

// LLM-HARNESS-BEGIN: us-8003a9d8
extern "C" void func_8003A5F8() {}
// LLM-HARNESS-END: us-8003a9d8

// LLM-HARNESS-BEGIN: us-8003aa10
extern "C" void func_8003A630() {}
// LLM-HARNESS-END: us-8003aa10

// LLM-HARNESS-BEGIN: us-8003aa48
extern "C" s32 func_8003A668(void*, OcMsgRingHdr* list) {
    list->count = 0;
    list->wrap = 0;
    list->writeIdx = 0;
    list->readIdx = 0;
    list->capacity = 9;
    return 0;
}
// LLM-HARNESS-END: us-8003aa48

// LLM-HARNESS-BEGIN: us-8003aa6c
extern "C" void func_8003A68C() {}
// LLM-HARNESS-END: us-8003aa6c

// LLM-HARNESS-BEGIN: us-8003aab4
extern "C" void func_8003A6D4() {}
// LLM-HARNESS-END: us-8003aab4

// LLM-HARNESS-BEGIN: us-8003aaf4
extern "C" s32 func_8003A714(s32 ret, OcMsgRingHdr* list) {
    // Reuse `ret` for writeIdx then count so +1 lands in a distinct reg (retail addi r5,r3,1).
    ret = list->writeIdx;
    u32 capacity = list->capacity;
    u32 newWriteIdx = ret + 1;
    ret = list->count;
    list->writeIdx = newWriteIdx;
    list->count = ret + 1;
    if (newWriteIdx > capacity) {
        list->writeIdx = list->wrap;
    }
    newWriteIdx = list->writeIdx;
    ret = 0;
    char* obj = reinterpret_cast<char*>(list);
    char* slotBase = obj + newWriteIdx * 8;
    *reinterpret_cast<u32*>(slotBase + 0x14) = *reinterpret_cast<u32*>(obj + 0x64);
    *reinterpret_cast<u32*>(slotBase + 0x18) = *reinterpret_cast<u32*>(obj + 0x68);
    return ret;
}
// LLM-HARNESS-END: us-8003aaf4

// LLM-HARNESS-BEGIN: us-8003ab44
extern "C" s32 func_8003A764(s32 ret, OcMsgRingHdr* list) {
    ret = list->readIdx;
    u32 capacity = list->capacity;
    u32 newReadIdx = ret + 1;
    ret = list->count;
    list->readIdx = newReadIdx;
    list->count = ret - 1;
    if (newReadIdx > capacity) {
        list->readIdx = list->wrap;
    }
    newReadIdx = list->readIdx;
    ret = 0;
    char* obj = reinterpret_cast<char*>(list);
    char* slotBase = obj + newReadIdx * 8;
    *reinterpret_cast<u32*>(obj + 0x64) = *reinterpret_cast<u32*>(slotBase + 0x14);
    *reinterpret_cast<u32*>(obj + 0x68) = *reinterpret_cast<u32*>(slotBase + 0x18);
    return ret;
}
// LLM-HARNESS-END: us-8003ab44

// LLM-HARNESS-BEGIN: us-8003ab94
extern "C" void func_8003A7B4() {}
// LLM-HARNESS-END: us-8003ab94

// LLM-HARNESS-BEGIN: us-8003abe0
extern "C" void func_8003A800() {}
// LLM-HARNESS-END: us-8003abe0

// LLM-HARNESS-BEGIN: us-8003ac18
extern "C" void func_8003A838() {}
// LLM-HARNESS-END: us-8003ac18

// LLM-HARNESS-BEGIN: us-8003ac50
extern "C" void func_8003A870() {}
// LLM-HARNESS-END: us-8003ac50

// LLM-HARNESS-BEGIN: us-8003ac88
extern "C" void func_8003A8A8() {}
// LLM-HARNESS-END: us-8003ac88

// LLM-HARNESS-BEGIN: us-8003acc0
extern "C" void func_8003A8E0() {}
// LLM-HARNESS-END: us-8003acc0

// LLM-HARNESS-BEGIN: us-8003acf8
extern "C" void func_8003A918() {}
// LLM-HARNESS-END: us-8003acf8

// LLM-HARNESS-BEGIN: us-8003ad30
extern "C" s32 func_8003A950(s32 ret, OcMsgRingHdr* list) {
    ret = list->writeIdx;
    u32 capacity = list->capacity;
    u32 newWriteIdx = ret + 1;
    ret = list->count;
    list->writeIdx = newWriteIdx;
    list->count = ret + 1;
    if (newWriteIdx > capacity) {
        list->writeIdx = list->wrap;
    }
    newWriteIdx = list->writeIdx;
    ret = 0;
    char* obj = reinterpret_cast<char*>(list);
    char* slotBase = obj + newWriteIdx * 12;
    *reinterpret_cast<u32*>(slotBase + 0x14) = *reinterpret_cast<u32*>(obj + 0x8c);
    *reinterpret_cast<u32*>(slotBase + 0x18) = *reinterpret_cast<u32*>(obj + 0x90);
    *reinterpret_cast<u32*>(slotBase + 0x1c) = *reinterpret_cast<u32*>(obj + 0x94);
    return ret;
}
// LLM-HARNESS-END: us-8003ad30

// LLM-HARNESS-BEGIN: us-8003ad88
extern "C" s32 func_8003A9A8(s32 ret, OcMsgRingHdr* list) {
    ret = list->readIdx;
    u32 capacity = list->capacity;
    u32 newReadIdx = ret + 1;
    ret = list->count;
    list->readIdx = newReadIdx;
    list->count = ret - 1;
    if (newReadIdx > capacity) {
        list->readIdx = list->wrap;
    }
    newReadIdx = list->readIdx;
    ret = 0;
    char* obj = reinterpret_cast<char*>(list);
    char* slotBase = obj + newReadIdx * 12;
    *reinterpret_cast<u32*>(obj + 0x8c) = *reinterpret_cast<u32*>(slotBase + 0x14);
    *reinterpret_cast<u32*>(obj + 0x90) = *reinterpret_cast<u32*>(slotBase + 0x18);
    *reinterpret_cast<u32*>(obj + 0x94) = *reinterpret_cast<u32*>(slotBase + 0x1c);
    return ret;
}
// LLM-HARNESS-END: us-8003ad88

// LLM-HARNESS-BEGIN: us-8003ade0
extern "C" void ocMsgRegist() {}
// LLM-HARNESS-END: us-8003ade0
