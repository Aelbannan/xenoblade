// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_crs
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

/* Ref-count + lock state live in the retail .bss slice owned by criware_data.s
 * (lbl_eu_805E6378/lbl_eu_805E637C at 0x805E6378); this TU owns no data in
 * retail, so declare extern (defining them here would add a bogus .bss). */
extern volatile s32 lbl_eu_805E637C;
extern s32 lbl_eu_805E6378;

void ADXCRS_Init(void) {
    ++lbl_eu_805E637C;
    if (lbl_eu_805E637C == 1) {
        s32 *p = &lbl_eu_805E6378;
        *p = 0;
    }
}

void ADXCRS_Finish(void) {
    --lbl_eu_805E637C;
    if (lbl_eu_805E637C == 0) {
        s32 *p = &lbl_eu_805E6378;
        *p = 0;
    }
}

void ADXCRS_Lock(void) {
    SVM_Lock();
}

void ADXCRS_Unlock(void) {
    SVM_Unlock();
}

void ADXCRS_Enter() {}

void ADXCRS_Leave() {}
