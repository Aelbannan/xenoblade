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

// LLM-HARNESS-BEGIN: us-8003a6f0
extern "C" int thread(VMThread* pThread) { return 0; }
// LLM-HARNESS-END: us-8003a6f0

// LLM-HARNESS-BEGIN: us-8003a784
extern "C" int start(VMThread* pThread) { return 0; }
// LLM-HARNESS-END: us-8003a784

// LLM-HARNESS-BEGIN: us-8003a7a8
extern "C" int end(VMThread* pThread) { return 0; }
// LLM-HARNESS-END: us-8003a7a8

// LLM-HARNESS-BEGIN: us-8003a7d0
extern "C" int isAlive(VMThread* pThread) { return 0; }
// LLM-HARNESS-END: us-8003a7d0

// LLM-HARNESS-BEGIN: us-8003a820
extern "C" int join(VMThread* pThread) { return 0; }
// LLM-HARNESS-END: us-8003a820

// LLM-HARNESS-BEGIN: us-8003a864
extern "C" int sleep(VMThread* pThread) { return 0; }
// LLM-HARNESS-END: us-8003a864

// LLM-HARNESS-BEGIN: us-8003a88c
extern "C" int wakeup(VMThread* pThread) { return 0; }
// LLM-HARNESS-END: us-8003a88c

// LLM-HARNESS-BEGIN: us-8003a8b4
extern "C" void ocThreadRegist() {
    vmOCRegist(&lbl_eu_80524C60);
}
// LLM-HARNESS-END: us-8003a8b4

// LLM-HARNESS-BEGIN: us-8003a8c0
extern "C" OcThreadListHdr* func_8003A4E0() {
    return &lbl_eu_805704C8;
}
// LLM-HARNESS-END: us-8003a8c0

// LLM-HARNESS-BEGIN: us-8003a8cc
extern "C" OcThreadListHdr* func_8003A4EC() {
    return &lbl_eu_80570538;
}
// LLM-HARNESS-END: us-8003a8cc

// LLM-HARNESS-BEGIN: us-8003a8d8
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
// LLM-HARNESS-END: us-8003a8d8
