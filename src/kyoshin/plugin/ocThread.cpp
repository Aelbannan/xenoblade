#include "kyoshin/plugin/ocThread.hpp"

extern "C" {
BOOL vmOCRegist(OCData* pOC);
extern OCData lbl_eu_80524C60;

// BSS list headers (retail .bss); sinit fills the leading fields.
struct OcThreadListHdr {
    u32 unk0;
    u32 unk4;
    u32 unk8;
    u32 unkC;
    u32 unk10;
};

extern OcThreadListHdr lbl_eu_805704C8;
extern OcThreadListHdr lbl_eu_80570538;
}

extern "C" int thread(VMThread* pThread, int unused, u32 unk2) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    u32 func = vmArgFunctionGet(2, arg);
    u32 outId;
    if (vmThreadGetOC(pThread, func, &outId) == 0) {
        vmOCExceptionThrow(pThread);
        return 0;
    }
    VMArg result;
    result.type = VM_TYPE_OC;
    result.unk2 = (u16)unk2;
    result.value.uintVal = outId;
    vmRetValSet(pThread, &result);
    return 1;
}

extern "C" int start_8003A784(VMThread* pThread, u32 r4) {
    vmThreadStart(pThread, r4);
    return 0;
}

extern "C" int end_8003A7A8(VMThread* pThread, u32 threadId) {
    vmThreadEnd(threadId);
    return 0;
}

extern "C" int isAlive_8003A7D0(VMThread* pThread, u32 threadId) {
    VMArg result;
    result.type = !vmThreadIsAlive(threadId) + 1;
    vmRetValSet(pThread, &result);
    return 1;
}

extern "C" int join_8003A820(VMThread* pThread, u32 threadId) {
    if (vmThreadIsAlive(threadId)) {
        vmWaitModeSet(pThread);
    }
    return 0;
}

extern "C" int sleep_8003A864(VMThread* pThread, u32 threadId) {
    vmThreadSleep(threadId);
    return 0;
}

extern "C" int wakeup_8003A88C(VMThread* pThread, u32 threadId) {
    vmThreadWakeup(threadId);
    return 0;
}

extern "C" void ocThreadRegist() {
    vmOCRegist(&lbl_eu_80524C60);
}

OcThreadListHdr* func_8003A4E0(){
    return &lbl_eu_805704C8;
}

OcThreadListHdr* func_8003A4EC(){
    return &lbl_eu_80570538;
}

extern "C" void sinit_8003A4F8() {
    OcThreadListHdr* a = &lbl_eu_805704C8;
    OcThreadListHdr* b = &lbl_eu_80570538;

    // Retail store order (both headers).
    a->unk10 = 0;
    a->unk0 = 0;
    a->unkC = 0;
    a->unk8 = 0;
    a->unk4 = 9;

    b->unk10 = 0;
    b->unk0 = 0;
    b->unkC = 0;
    b->unk8 = 0;
    b->unk4 = 9;
}
