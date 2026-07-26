// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_bau
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void AU_GetInfo() {}

void ADXB_CheckAu() {}

void ADX_DecodeInfoAu() {}

void ADXB_DecodeHeaderAu() {}

void ADXB_ExecOneAu16(void* self) {}

void ADXB_ExecOneAu8(void* self) {}

void ADXB_ExecOneAuUlaw(void* self) {}

void ADXB_ExecOneAu(void* self) {
    s16 fmt = *(s16*)((u8*)self + 0x9c);
    if (fmt == 2) {
        ADXB_ExecOneAuUlaw(self);
    } else if (fmt == 1) {
        ADXB_ExecOneAu8(self);
    } else {
        ADXB_ExecOneAu16(self);
    }
}
