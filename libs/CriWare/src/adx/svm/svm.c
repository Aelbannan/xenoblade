// Auto-scaffolded catalog TU for CriWare/src/adx/svm/svm
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-80399b0c
extern u32 lbl_eu_805F26F0[];
void SVM_Lock(void) {
    u32* b = lbl_eu_805F26F0;
    u32* cb = (u32*)((u8*)b + 0x10);
    void (*fn)(u32) = (void (*)(u32))*cb;
    if (!fn) return;
    fn(*(volatile u32*)(cb + 1));
    if (b[1] == 0) b[2] = 1;
    b[1]++;
}
// LLM-HARNESS-END: us-80399b0c

// LLM-HARNESS-BEGIN: us-80399b74
extern u32 lbl_eu_805F26F0[];
extern const char lbl_eu_80518F50[];
void SVM_CallErr();
void SVM_Unlock(void) {
    u32* b = lbl_eu_805F26F0;
    void (*fn)(u32) = (void (*)(u32))b[0x18/4];
    if (!fn) return;
    b[0x04/4]--;
    if (b[0x04/4] == 0) {
        if (b[0x08/4] != 1)
            SVM_CallErr(lbl_eu_80518F50, b[0x08/4], 1);
        b[0x08/4] = 0;
    }
    fn(b[0x1c/4]);
}
// LLM-HARNESS-END: us-80399b74

// LLM-HARNESS-BEGIN: us-80399c04
void SVM_CallErr() {}
// LLM-HARNESS-END: us-80399c04

// LLM-HARNESS-BEGIN: us-80399cdc
void SVM_CallErr1() {}
// LLM-HARNESS-END: us-80399cdc

// LLM-HARNESS-BEGIN: us-80399d64
void SVM_SetCbSvrWithString() {}
// LLM-HARNESS-END: us-80399d64

// LLM-HARNESS-BEGIN: us-80399e5c
void svm_SetCbSvr() {}
// LLM-HARNESS-END: us-80399e5c

// LLM-HARNESS-BEGIN: us-80399fb8
void SVM_DelCbSvr() {}
// LLM-HARNESS-END: us-80399fb8

// LLM-HARNESS-BEGIN: us-8039a194
void SVM_SetCbSvrIdWithString() {}
// LLM-HARNESS-END: us-8039a194

// LLM-HARNESS-BEGIN: us-8039a28c
void svm_SetCbSvrId() {}
// LLM-HARNESS-END: us-8039a28c

// LLM-HARNESS-BEGIN: us-8039a438
void SVM_SetCbBdr() {}
// LLM-HARNESS-END: us-8039a438

// LLM-HARNESS-BEGIN: us-8039a53c
extern u32 lbl_eu_805F2A58[];
extern void (*lbl_eu_805F2A68)(s32);
void SVM_GotoSvrBorder(s32 idx) {
    lbl_eu_805F2A68(idx);
}
// LLM-HARNESS-END: us-8039a53c

// LLM-HARNESS-BEGIN: us-8039a568
void SVM_SetCbErr() {}
// LLM-HARNESS-END: us-8039a568

// LLM-HARNESS-BEGIN: us-8039a658
typedef struct SvmCbPair {
    void* cb;
    void* ctx;
} SvmCbPair;
extern SvmCbPair lbl_eu_805F2700;
void SVM_SetCbLock(void* cb, void* ctx) {
    SvmCbPair* p = &lbl_eu_805F2700;
    p->cb = cb;
    p->ctx = ctx;
}
// LLM-HARNESS-END: us-8039a658

// LLM-HARNESS-BEGIN: us-8039a66c
extern SvmCbPair lbl_eu_805F2708;
void SVM_SetCbUnlock(void* cb, void* ctx) {
    SvmCbPair* p = &lbl_eu_805F2708;
    p->cb = cb;
    p->ctx = ctx;
}
// LLM-HARNESS-END: us-8039a66c

// LLM-HARNESS-BEGIN: us-8039a680
void SVM_ExecSvrVint() {}
// LLM-HARNESS-END: us-8039a680

// LLM-HARNESS-BEGIN: us-8039a704
void SVM_ExecSvrUsrVsync() {}
// LLM-HARNESS-END: us-8039a704

// LLM-HARNESS-BEGIN: us-8039a794
void SVM_ExecSvrVsync() {}
// LLM-HARNESS-END: us-8039a794

// LLM-HARNESS-BEGIN: us-8039a824
void SVM_ExecSvrUhigh() {}
// LLM-HARNESS-END: us-8039a824

// LLM-HARNESS-BEGIN: us-8039a8b4
void SVM_ExecSvrFs() {}
// LLM-HARNESS-END: us-8039a8b4

// LLM-HARNESS-BEGIN: us-8039a944
void SVM_ExecSvrMain() {}
// LLM-HARNESS-END: us-8039a944

// LLM-HARNESS-BEGIN: us-8039a9d4
void SVM_ExecSvrMwIdle() {}
// LLM-HARNESS-END: us-8039a9d4

// LLM-HARNESS-BEGIN: us-8039aa64
void SVM_ExecSvrUsrIdle() {}
// LLM-HARNESS-END: us-8039aa64

// LLM-HARNESS-BEGIN: us-8039aaf4
void SVM_Init() {}
// LLM-HARNESS-END: us-8039aaf4

// LLM-HARNESS-BEGIN: us-8039abac
void SVM_Finish() {}
// LLM-HARNESS-END: us-8039abac

// LLM-HARNESS-BEGIN: us-8039ac74
void SVM_TestAndSet() {}
// LLM-HARNESS-END: us-8039ac74
