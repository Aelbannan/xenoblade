// Auto-scaffolded catalog TU for CriWare/src/adx/svm/svm
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern u32 lbl_eu_805F26F0[];
void SVM_Lock(void) {
    u32 fn;
    u32 ctx;
    u32* b = lbl_eu_805F26F0;
    fn = b[4];
    if (fn == 0) return;
    ctx = b[5];
    ((void (*)(u32))fn)(ctx);
    if (b[1] == 0) b[2] = 1;
    b[1]++;
}

extern u32 lbl_eu_805F26F0[];
extern const char lbl_eu_80518F50[];
void SVM_CallErr(const char*, u32, ...);
void SVM_Unlock(void) {
    u32* b = lbl_eu_805F26F0;
    u32 fn = b[6];
    if (fn == 0) return;
    {
        u32 cnt = b[1];
        b[1] = cnt - 1;
        if (b[1] == 0) {
            if (b[2] != 1)
                SVM_CallErr(lbl_eu_80518F50, b[2], 1);
            b[2] = 0;
        }
    }
    ((void (*)(u32))b[6])(b[7]);
}

void SVM_CallErr(const char*, u32, ...) {}

void SVM_CallErr1() {}

void SVM_SetCbSvrWithString() {}

void svm_SetCbSvr() {}

void SVM_DelCbSvr() {}

void SVM_SetCbSvrIdWithString() {}

void svm_SetCbSvrId() {}

void SVM_SetCbBdr() {}

extern u32 lbl_eu_805F2A58[];
extern void (*lbl_eu_805F2A68)(s32);
extern u32 lbl_eu_805F2A58[];

void SVM_GotoSvrBorder(s32 idx) {
    u32* base = lbl_eu_805F2A58;
    u32 fn = base[idx * 2];
    if (fn == 0) return;
    ((void (*)(void*))fn)((void*)base[idx * 2 + 1]);
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
