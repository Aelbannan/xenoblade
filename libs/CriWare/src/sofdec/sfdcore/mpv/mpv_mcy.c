// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/mpv/mpv_mcy
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void MPVMC16_Init(void* this_ptr) {
    extern const unsigned int lbl_eu_8051C368[4];
    unsigned int* base = (unsigned int*)this_ptr;
    base[0x34 / 4] = lbl_eu_8051C368[0];
    base[0x38 / 4] = lbl_eu_8051C368[1];
    base[0x3c / 4] = lbl_eu_8051C368[2];
    base[0x40 / 4] = lbl_eu_8051C368[3];
}

void MPVMC16_OneRef1p_TuneC() {}

void MPVMC16_OneRefV2_TuneC() {}

void MPVMC16_OneRefH2_TuneC() {}

void MPVMC16_OneRef4p_TuneC() {}
