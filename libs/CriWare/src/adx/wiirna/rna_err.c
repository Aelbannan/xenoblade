// Auto-scaffolded catalog TU for CriWare/src/adx/wiirna/rna_err
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-8039c318
extern u32 lbl_eu_805F2AF8;
extern u32 lbl_eu_805F2AFC;
void RNAERR_EntryErrFunc(void* a, void* b) {
    lbl_eu_805F2AF8 = (u32)a;
    lbl_eu_805F2AFC = (u32)b;
}
// LLM-HARNESS-END: us-8039c318

// LLM-HARNESS-BEGIN: us-8039c32c
char* strncpy(char* dst, const char* src, size_t n);
extern u32 lbl_eu_805F2AF8;
extern u32 lbl_eu_805F2AFC;
void RNAERR_CallErrFunc(const char* msg) {
    void (*cb)(u32, char*) = (void (*)(u32, char*))lbl_eu_805F2AF8;
    char* buf = (char*)&lbl_eu_805F2AFC + 4;
    strncpy(buf, msg, 0xFF);
    if (cb)
        cb(lbl_eu_805F2AFC, buf);
}
// LLM-HARNESS-END: us-8039c32c
