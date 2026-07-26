// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/dct/dct_isr
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void DCT_IsrInit() {}

extern char lbl_eu_80604CC8[];
void* memcpy(void* dst, const void* src, size_t n);
void DCT_IsrInitScaleTbl(void* dst) {
    memcpy(dst, lbl_eu_80604CC8, 0x80);
}

void MSID_JRevDctDense();
void MSID_JRevDctSparse();
void DCT_IsrTrans(void* self, s32 val) {
    if (val < 0) {
        ((void(*)(void*,u32))MSID_JRevDctDense)(self, -val);
    } else {
        ((void(*)(void*,u32))MSID_JRevDctSparse)(self, val);
    }
}

void initSparseTbl() {}

void MSID_JRevDctSparse() {}

void MSID_JRevDctDense() {}
