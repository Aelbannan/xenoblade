// Auto-scaffolded catalog TU for CriWare/src/adx/svm/svm
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

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

void SVM_CallErr() {}

void SVM_CallErr1() {}

void SVM_SetCbSvrWithString() {}

void svm_SetCbSvr() {}

void SVM_DelCbSvr() {}

void SVM_SetCbSvrIdWithString() {}

void svm_SetCbSvrId() {}

void SVM_SetCbBdr() {}

extern u32 lbl_eu_805F2A58[];
extern void (*lbl_eu_805F2A68)(s32);
void SVM_GotoSvrBorder(s32 idx) {
    lbl_eu_805F2A68(idx);
}

void SVM_SetCbErr() {}

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

extern SvmCbPair lbl_eu_805F2708;
void SVM_SetCbUnlock(void* cb, void* ctx) {
    SvmCbPair* p = &lbl_eu_805F2708;
    p->cb = cb;
    p->ctx = ctx;
}

void SVM_ExecSvrVint() {}

void SVM_ExecSvrUsrVsync() {}

void SVM_ExecSvrVsync() {}

void SVM_ExecSvrUhigh() {}

void SVM_ExecSvrFs() {}

void SVM_ExecSvrMain() {}

void SVM_ExecSvrMwIdle() {}

void SVM_ExecSvrUsrIdle() {}

void SVM_Init() {}

void SVM_Finish() {}

void SVM_TestAndSet() {}
