// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_bwav
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void ADX_DecodeInfoWav();
void ADXB_DecodeHeaderWav();
void ADXB_ExecOneWav16(void* self);
void ADXB_ExecOneWav8(void* self);
void ADXB_ExecOneWav4(void* self);
extern char lbl_eu_80517418[];
extern int memcmp(const void* a, const void* b, unsigned long n);

s32 ADXB_CheckWav(const u8* self) {
    if (memcmp(self, lbl_eu_80517418, 4) == 0 &&
        memcmp(self + 8, lbl_eu_80517418 + 5, 4) == 0)
        return 1;
    return 0;
}

void ADXB_ExecOneWav(void* self) {
    s32 codec = *(s16*)((u8*)self + 0x9c);
    if (codec == 2)
        ADXB_ExecOneWav4(self);
    else if (codec == 1)
        ADXB_ExecOneWav8(self);
    else if (codec == 0)
        ADXB_ExecOneWav16(self);
}

s32 ADX_ScanInfoCodeWav(const u8* codes, s32 size, u16* out) {
    s32 i;
    s32 found = 0x7FFFFFFF;
    for (i = 0; i < size - 3; i++) {
        if (memcmp(codes + i, lbl_eu_80517418, 4) == 0) {
            found = 0x7FFFFFFF;
            if (i < found)
                found = i;
            break;
        }
    }
    if (found != 0x7FFFFFFF) {
        *out = (u16)found;
        return 0;
    }
    *out = 0;
    return -1;
}
