// Auto-scaffolded catalog TU for CriWare/src/sofdec/sfdcore/mpv/mpvabdec
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern char lbl_eu_8051C378[];
extern u32 lbl_eu_806046A8[];
void MPVABDEC_Init(void) {
    lbl_eu_806046A8[2] = (u32)lbl_eu_8051C378;
}

void mpvabdec_IntraBlock_Isr() {}

void mpvabdec_IntraBlockDc11_Isr() {}

void mpvabdec_NintraBlock_Isr() {}
