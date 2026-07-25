// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_bahx
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-80388ea8
extern void (*lbl_eu_805E5348)(void*);
void ADXB_SetAhxInSj(void* self) {
    void* p = *(void**)((u8*)self + 0xb8);
    if (p == NULL) return;
    lbl_eu_805E5348(p);
}
// LLM-HARNESS-END: us-80388ea8

// LLM-HARNESS-BEGIN: us-80388ec8
void ADXB_SetAhxDecSmpl() {}
// LLM-HARNESS-END: us-80388ec8

// LLM-HARNESS-BEGIN: us-80388f48
extern char lbl_eu_805E5354[];
void ADXB_ExecOneAhx(void* self) {
    ((void (*)(void*))(*(void**)(lbl_eu_805E5354)))(self);
}
// LLM-HARNESS-END: us-80388f48

// LLM-HARNESS-BEGIN: us-80388f58
extern void (*lbl_eu_805E5350)(void*);
void ADXB_AhxTermSupply(void* self) {
    void* p = *(void**)((u8*)self + 0xb8);
    if (p == NULL) return;
    lbl_eu_805E5350(p);
}
// LLM-HARNESS-END: us-80388f58
