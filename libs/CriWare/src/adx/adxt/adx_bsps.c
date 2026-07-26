// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_bsps
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void ADX_DecodeInfoSpsd() {}

void ADXB_DecodeHeaderSpsd() {}

void ADXB_ExecOneSpsd() {}

s32 memcmp(const void* s1, const void* s2, size_t n);
extern const u8 lbl_eu_80519108[4];
s32 ADXB_CheckSpsd(const void* buf) {
    return memcmp(buf, lbl_eu_80519108, 4) == 0 ? 1 : 0;
}
