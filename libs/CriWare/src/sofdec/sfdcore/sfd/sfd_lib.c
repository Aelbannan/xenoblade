// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_lib
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-803c3310
u32 SFD_IsVersionCompatible(void* self, u32 version) {
    return (version == 0x39b0) ? 1 : 0;
}
// LLM-HARNESS-END: us-803c3310

// LLM-HARNESS-BEGIN: us-803c3320
void fn_803C3320() {}
// LLM-HARNESS-END: us-803c3320

// LLM-HARNESS-BEGIN: us-803c3400
void fn_803C3400() {}
// LLM-HARNESS-END: us-803c3400

// LLM-HARNESS-BEGIN: us-803c34dc
void fn_803C34DC(void* self) {
    *(u32*)((u8*)self) = 0;
    *(u32*)((u8*)self + 4) = 0;
    *(u32*)((u8*)self + 8) = 0;
    *(u32*)((u8*)self + 0xc) = 0;
    *(u32*)((u8*)self + 0x10) = 0;
}
// LLM-HARNESS-END: us-803c34dc

// LLM-HARNESS-BEGIN: us-803c34f8
void fn_803C34F8(void) {}
// LLM-HARNESS-END: us-803c34f8

// LLM-HARNESS-BEGIN: us-803c34fc
extern u32 lbl_eu_80606E34;
extern u32 lbl_eu_80568720[];
extern u32 lbl_eu_80606E20[];
s32 SFLIB_SetErr(s32 val, u32 err_code) {
    if (err_code == 0) return 0;
    {
        u32* cb = (u32*)lbl_eu_80606E34;
        if (cb) {
            lbl_eu_80568720[3] = val;  // offset 0xc
            ((void (*)(u32*, u32*))(*cb ? *(void**)(*((u32*)*cb) + 0x24) : 0))(cb, &lbl_eu_80568720[1]);
        }
    }
    if (val == 0) {
        u32* ctx = lbl_eu_80606E20;
        if (ctx[2] == 0) ctx[2] = err_code;
        if (err_code) {
            void (*cb)(u32) = (void (*)(u32))ctx[0];
            if (cb) cb(ctx[1]);
        }
    } else {
        u8* h = (u8*)(long)val;
        if (*(u32*)(h + 0xa10) == 0) *(u32*)(h + 0xa10) = err_code;
        if (err_code) {
            void (*cb)(u32) = (void (*)(u32))(*(u32*)(h + 0xa08));
            if (cb) cb(*(u32*)(h + 0xa0c));
        }
        if (*(s32*)(h + 0x54) > 0) *(s32*)(h + 0x54) = -*(s32*)(h + 0x54);
    }
    {
        u32* cb = (u32*)lbl_eu_80606E34;
        if (cb) {
            void (*fn)(u32*, u32*) = (void (*)(u32*, u32*))(*(void**)*cb ? *(void**)(*((u32*)*cb) + 0x24) : 0);
            fn(cb, &lbl_eu_80568720[27]);  // offset 0x6c
        }
    }
    return err_code;
}
// LLM-HARNESS-END: us-803c34fc

// LLM-HARNESS-BEGIN: us-803c363c
void criware_803C0D94() {}
// LLM-HARNESS-END: us-803c363c

// LLM-HARNESS-BEGIN: us-803c36ac
extern void* lbl_eu_8060715C;
s32 SFLIB_CheckHn(void* h) {
    if (h == NULL) return -1;
    if (*(s32*)((u8*)h + 0x54) == 0) return -1;
    lbl_eu_8060715C = h;
    return 0;
}
// LLM-HARNESS-END: us-803c36ac

// LLM-HARNESS-BEGIN: us-803c36e0
void SVM_Lock(void);
void SFLIB_LockCs(void* cs) {
    SVM_Lock();
}
// LLM-HARNESS-END: us-803c36e0

// LLM-HARNESS-BEGIN: us-803c36e4
void SVM_Unlock(void);
void SFLIB_UnlockCs(void* cs) {
    SVM_Unlock();
}
// LLM-HARNESS-END: us-803c36e4
