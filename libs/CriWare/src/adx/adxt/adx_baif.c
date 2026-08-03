// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_baif
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void AIFF_GetInfo() {}

extern char lbl_eu_80516B20[];

int ADXB_CheckAiff(void* self) {
    if (memcmp(self, lbl_eu_80516B20, 4) == 0 && memcmp((u8*)self + 8, lbl_eu_80516B20 + 5, 4) == 0)
        return 1;
    return 0;
}

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
