// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_bsps
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-8039ad88
void ADX_DecodeInfoSpsd() {}
// LLM-HARNESS-END: us-8039ad88

// LLM-HARNESS-BEGIN: us-8039aea8
void ADXB_DecodeHeaderSpsd() {}
// LLM-HARNESS-END: us-8039aea8

// LLM-HARNESS-BEGIN: us-8039af94
void ADXB_ExecOneSpsd() {}
// LLM-HARNESS-END: us-8039af94

// LLM-HARNESS-BEGIN: us-8039b2b4
s32 memcmp(const void* s1, const void* s2, size_t n);
extern const u8 lbl_eu_80519108[4];
s32 ADXB_CheckSpsd(const void* buf) {
    return memcmp(buf, lbl_eu_80519108, 4) == 0 ? 1 : 0;
}
// LLM-HARNESS-END: us-8039b2b4
