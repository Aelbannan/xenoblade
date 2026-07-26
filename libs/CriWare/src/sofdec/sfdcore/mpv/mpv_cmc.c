// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/mpv/mpv_cmc
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-803a7da8
void MPVCMC_InitObj() {}
// LLM-HARNESS-END: us-803a7da8

// LLM-HARNESS-BEGIN: us-803a7de8
void mpvcmc_InitMcOiTa() {}
// LLM-HARNESS-END: us-803a7de8

// LLM-HARNESS-BEGIN: us-803a7e40
void MPVCMC_InitMcOiRt() {}
// LLM-HARNESS-END: us-803a7e40

// LLM-HARNESS-BEGIN: us-803a7e80
void MPVCMC_SetCcnt(void* self) {
    u32 val = *(u32*)((u8*)self + 0xb24);
    int r4;
    if (val != 0) {
        r4 = 4;
    } else {
        r4 = -1;
    }
    *(u32*)((u8*)self + 0xad4) = r4;
    *(u32*)((u8*)self + 0xaa0) = r4;
}
// LLM-HARNESS-END: us-803a7e80
