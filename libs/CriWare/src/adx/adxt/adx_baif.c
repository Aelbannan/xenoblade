// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_baif
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void AIFF_GetInfo() {}

void ADXB_CheckAiff() {}

void ADX_DecodeInfoAiff() {}

void ADXB_DecodeHeaderAiff() {}

void ADXB_ExecOneAiff16() {}

void ADXB_ExecOneAiff8() {}

void ADXB_ExecOneAiff(void* self) {
    if ((int)*(s16*)((u8*)self + 0x9c) == 1) {
        ((void(*)(void*))ADXB_ExecOneAiff8)(self);
    } else {
        ((void(*)(void*))ADXB_ExecOneAiff16)(self);
    }
}
