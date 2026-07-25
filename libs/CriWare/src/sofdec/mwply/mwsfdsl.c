// Auto-scaffolded catalog TU for CriWare/src/sofdec/mwply/mwsfdsl
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-803a02d4
void mwPlyEntryFname() {}
// LLM-HARNESS-END: us-803a02d4

// LLM-HARNESS-BEGIN: us-803a03f8
void mwPlyStartSeamless() {}
// LLM-HARNESS-END: us-803a03f8

// LLM-HARNESS-BEGIN: us-803a0518
void mwPlySetSeamlessLp() {}
// LLM-HARNESS-END: us-803a0518

// LLM-HARNESS-BEGIN: us-803a05d8
void mwPlyStartFnameLp() {}
// LLM-HARNESS-END: us-803a05d8

// LLM-HARNESS-BEGIN: us-803a07a0
void mwPlyStartFnameRangeLp() {}
// LLM-HARNESS-END: us-803a07a0

// LLM-HARNESS-BEGIN: us-803a0938
void MWSFLSC_IsFsStatErr() {}
// LLM-HARNESS-END: us-803a0938

// LLM-HARNESS-BEGIN: us-803a0964
void LSC_SetFlowLimit(void* a);
void MWSFLSC_SetFlowLimit(void* self) {
    void* obj = *(void**)((u8*)self + 0x64);
    if (obj != NULL) {
        LSC_SetFlowLimit(obj);
    }
}
// LLM-HARNESS-END: us-803a0964

// LLM-HARNESS-BEGIN: us-803a0978
void LSC_Pause(void* a);
void MWSFLSC_Pause(void* self) {
    void* obj = *(void**)((u8*)self + 0x64);
    if (obj != NULL) {
        LSC_Pause(obj);
    }
}
// LLM-HARNESS-END: us-803a0978

// LLM-HARNESS-BEGIN: us-803a098c
void mwPlyLinkStm() {}
// LLM-HARNESS-END: us-803a098c

// LLM-HARNESS-BEGIN: us-803a0a04
void mwPlyExecInfiniteLoopHandle() {}
// LLM-HARNESS-END: us-803a0a04
