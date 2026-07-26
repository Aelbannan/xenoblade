// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/dct/dct_isr
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// LLM-HARNESS-BEGIN: us-803bd574
void DCT_IsrInit() {}
// LLM-HARNESS-END: us-803bd574

// LLM-HARNESS-BEGIN: us-803bd6e0
extern char lbl_eu_80604CC8[];
void* memcpy(void* dst, const void* src, size_t n);
void DCT_IsrInitScaleTbl(void* dst) {
    memcpy(dst, lbl_eu_80604CC8, 0x80);
}
// LLM-HARNESS-END: us-803bd6e0

// LLM-HARNESS-BEGIN: us-803bd6f0
void MSID_JRevDctDense();
void MSID_JRevDctSparse();
void DCT_IsrTrans(void* self, s32 val) {
    if (val < 0) {
        ((void(*)(void*,u32))MSID_JRevDctDense)(self, -val);
    } else {
        ((void(*)(void*,u32))MSID_JRevDctSparse)(self, val);
    }
}
// LLM-HARNESS-END: us-803bd6f0

// LLM-HARNESS-BEGIN: us-803bd704
void initSparseTbl() {}
// LLM-HARNESS-END: us-803bd704

// LLM-HARNESS-BEGIN: us-803bda7c
void MSID_JRevDctSparse() {}
// LLM-HARNESS-END: us-803bda7c

// LLM-HARNESS-BEGIN: us-803bdc20
void MSID_JRevDctDense() {}
// LLM-HARNESS-END: us-803bdc20
