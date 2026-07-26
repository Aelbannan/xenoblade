// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/mpv/mpv_err
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-803a98e0
void MPVERR_Init(void) {}
// LLM-HARNESS-END: us-803a98e0

// LLM-HARNESS-BEGIN: us-803a98e4
void MPVERR_InitErrInf(void* self) {
    *(u32*)((u8*)self) = 0;
    *(u32*)((u8*)self + 4) = 0;
    *(u32*)((u8*)self + 8) = 0;
    *(u32*)((u8*)self + 0xc) = 0;
    *(u32*)((u8*)self + 0x10) = 0;
}
// LLM-HARNESS-END: us-803a98e4

// LLM-HARNESS-BEGIN: us-803a9900
void MPV_SetErrFunc() {}
// LLM-HARNESS-END: us-803a9900

// LLM-HARNESS-BEGIN: us-803a99a8
void MPVERR_SetCode() {}
// LLM-HARNESS-END: us-803a99a8
