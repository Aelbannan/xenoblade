// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/sfd/sfd_set
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-803cec14
void SFD_GetHnStat() {}
// LLM-HARNESS-END: us-803cec14

// LLM-HARNESS-BEGIN: us-803cec5c
void SFD_SetCond() {}
// LLM-HARNESS-END: us-803cec5c

// LLM-HARNESS-BEGIN: us-803cee4c
void SFSET_SetCond() {}
// LLM-HARNESS-END: us-803cee4c

// LLM-HARNESS-BEGIN: us-803ceef8
void SFD_GetCond() {}
// LLM-HARNESS-END: us-803ceef8

// LLM-HARNESS-BEGIN: us-803cef8c
u32 SFSET_GetCond(void* self, u32 idx) {
    return *(u32*)((u8*)self + 0xa1c + idx * 4);
}
// LLM-HARNESS-END: us-803cef8c

// LLM-HARNESS-BEGIN: us-803cef9c
void SFD_SetSfdHeaderFn(void* self, void* fn, void* ctx) {
    *(void**)((u8*)self + 0xd3c) = fn;
    *(void**)((u8*)self + 0xd40) = ctx;
}
// LLM-HARNESS-END: us-803cef9c

// LLM-HARNESS-BEGIN: us-803cefa8
void SFD_GetSofdecHeader() {}
// LLM-HARNESS-END: us-803cefa8
