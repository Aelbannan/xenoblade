// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_bwav
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void ADX_DecodeInfoWav();
void ADXB_DecodeHeaderWav();
void ADXB_ExecOneWav16(void* self);
void ADXB_ExecOneWav8(void* self);
void ADXB_ExecOneWav4(void* self);
void ADXB_CheckWav();

void ADXB_ExecOneWav(void* self) {
    s32 codec = *(s16*)((u8*)self + 0x9c);
    if (codec == 2)
        ADXB_ExecOneWav4(self);
    else if (codec == 1)
        ADXB_ExecOneWav8(self);
    else if (codec == 0)
        ADXB_ExecOneWav16(self);
}

void ADX_ScanInfoCodeWav() {}
