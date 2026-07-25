// Auto-scaffolded catalog TU for CriWare/src/sofdec/mwply/mwsfdlib
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-803a41c0
void* MWSFLIB_GetLibWorkPtr(void) { return 0; }
// LLM-HARNESS-END: us-803a41c0

// LLM-HARNESS-BEGIN: us-803a41cc
void mwsflib_LscErrFunc(void) {}
// LLM-HARNESS-END: us-803a41cc

// LLM-HARNESS-BEGIN: us-803a41d8
void mwPlyInitSfdFx() {}
// LLM-HARNESS-END: us-803a41d8

// LLM-HARNESS-BEGIN: us-803a4440
void mwsflib_InitLibWork() {}
// LLM-HARNESS-END: us-803a4440

// LLM-HARNESS-BEGIN: us-803a44f4
extern char lbl_eu_805FF3A8[];
u32 MWSFD_GetUsePicUsr(void) {
    return *(u32*)(lbl_eu_805FF3A8 + 0x38);
}
// LLM-HARNESS-END: us-803a44f4

// LLM-HARNESS-BEGIN: us-803a4504
extern char lbl_eu_805FF3A8[];
u32 MWSFD_GetPauseBdr(void) {
    return *(u32*)(lbl_eu_805FF3A8 + 0x3c);
}
// LLM-HARNESS-END: us-803a4504

// LLM-HARNESS-BEGIN: us-803a4514
u32 MWSFLIB_SetErrCode(u32 code) {
    u32 lz = __cntlzw(code);
    u32 bit = (lz >> 5) & 1;
    *(u32*)(lbl_eu_805FF3A8 + 0x68) = code;
    return code & ~(0 - bit);
}
// LLM-HARNESS-END: us-803a4514

// LLM-HARNESS-BEGIN: us-803a4534
void mwPlySfdInit() {}
// LLM-HARNESS-END: us-803a4534

// LLM-HARNESS-BEGIN: us-803a45f8
void MWSFLIB_SfdErrFunc() {}
// LLM-HARNESS-END: us-803a45f8
