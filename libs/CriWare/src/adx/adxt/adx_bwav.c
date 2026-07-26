// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_bwav
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void ADX_DecodeInfoWav() {}

void ADXB_DecodeHeaderWav() {}

void ADXB_ExecOneWav16() {}

void ADXB_ExecOneWav8() {}

void ADXB_ExecOneWav4() {}

void ADXB_CheckWav() {}

void ADXB_ExecOneWav(void* self) {
    s32 codec = *(s16*)((u8*)self + 0x9c);
    if (codec == 2) { ADXB_ExecOneWav16(); return; }
    if (codec == 1) { ADXB_ExecOneWav8(); return; }
    ADXB_ExecOneWav4();
}

void ADX_ScanInfoCodeWav() {}
