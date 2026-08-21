#include "kyoshin/plugin/ocThread.hpp"

// Script-facing "thread" plugin entry points. Each callback receives the
// running VM thread plus its script arguments and reports the result through
// vmRetValSet (return value 1 = a result was produced).

// thread(funcId, subType): resolve the function id to an OC instance id and
// return it as an OC-typed result; throws a VM exception if unresolved.
int thread(VMThread* pThread, int unused, u32 subType) {
    VMArg* arg = vmArgPtrGet(pThread, 1);
    u32 func = vmArgFunctionGet(2, arg);
    u32 outId;
    if (vmThreadGetOC(pThread, func, &outId) == 0) {
        vmOCExceptionThrow(pThread);
        return 0;
    }
    VMArg result;
    result.type = VM_TYPE_OC;
    // VMArg's u16 at offset 0x2 carries the OC subtype; the field is unnamed
    // in monolib's shared VMArg layout, so store it by offset (same pattern
    // as copyArg in yvm_util.h).
    *(u16*)((u8*)&result + 2) = (u16)subType;
    result.value.uintVal = outId;
    vmRetValSet(pThread, &result);
    return 1;
}

// start(threadId): resume a suspended VM thread.
int start_8003A784(VMThread* pThread, u32 threadId) {
    vmThreadStart(pThread, threadId);
    return 0;
}

// end(threadId): terminate a VM thread.
int end_8003A7A8(VMThread* pThread, u32 threadId) {
    vmThreadEnd(threadId);
    return 0;
}

// isAlive(threadId): returns true while the thread is runnable.
int isAlive_8003A7D0(VMThread* pThread, u32 threadId) {
    VMArg result;
    result.type = !vmThreadIsAlive(threadId) + 1;
    vmRetValSet(pThread, &result);
    return 1;
}

// join(threadId): block the calling thread until the target finishes.
int join_8003A820(VMThread* pThread, u32 threadId) {
    if (vmThreadIsAlive(threadId)) {
        vmWaitModeSet(pThread);
    }
    return 0;
}

// sleep(threadId) / wakeup(threadId): pause and resume a VM thread.
int sleep_8003A864(VMThread* pThread, u32 threadId) {
    vmThreadSleep(threadId);
    return 0;
}

int wakeup_8003A88C(VMThread* pThread, u32 threadId) {
    vmThreadWakeup(threadId);
    return 0;
}

// Registers this plugin's OC table with the script VM.
void ocThreadRegist() {
    vmOCRegist(&lbl_eu_80524C60);
}

OcThreadListHdr* func_8003A4E0() {
    return &lbl_eu_805704C8;
}

OcThreadListHdr* func_8003A4EC() {
    return &lbl_eu_80570538;
}

// Static initializer: clears both BSS list headers. The odd store order
// mirrors retail; capacity is set to 9 in each header.
void sinit_8003A4F8() {
    OcThreadListHdr* a = &lbl_eu_805704C8;
    OcThreadListHdr* b = &lbl_eu_80570538;

    // Retail store order (both headers).
    a->count = 0;
    a->wrap = 0;
    a->writeIdx = 0;
    a->readIdx = 0;
    a->capacity = 9;

    b->count = 0;
    b->wrap = 0;
    b->writeIdx = 0;
    b->readIdx = 0;
    b->capacity = 9;
}
