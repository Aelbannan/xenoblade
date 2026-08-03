// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_bsps
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

int ADX_DecodeInfoSpsd(const u8 *data, int size, u16 *outBps, s8 *outCodec,
    u8 *outVer, s8 *outCh, s8 *outX, u32 *outNum, u32 *outSmp,
    u32 *outBlk, s16 *outX2)
{
    *outBps = data[7] << 4;
    *outX = (data[9] & 3) + 1;
    *outNum = *(u16 *)&data[0x2A];

    if (data[8] == 0) {
        *outVer = 0x10;
        *outCh = *outX << 1;
        *outBlk = 1;
        *outSmp = *(s32 *)&data[0x0C] / 2;
        *outX2 = 0x10;
    } else if (data[8] == 1) {
        *outVer = 8;
        *outCh = *outX;
        *outBlk = 1;
        *outSmp = *(s32 *)&data[0x0C];
        *outX2 = 1;
    } else if ((u32)(data[8] - 2) <= 1) {
        *outVer = 4;
        *outCh = *outX;
        *outBlk = 2;
        *outSmp = *(s32 *)&data[0x0C] * 2;
        *outX2 = 2;
    }

    *outCh = 2;
    *outBlk = 1;
    *outSmp = *(s32 *)&data[0x0C] / 2;
    *outVer = 0x10;
    *outCodec = -1;
    return 0;
}

void ADXB_DecodeHeaderSpsd() {}

void ADXB_ExecOneSpsd() {}

s32 memcmp(const void* s1, const void* s2, size_t n);
extern const u8 lbl_eu_80519108[4];
s32 ADXB_CheckSpsd(const void* buf) {
    return memcmp(buf, lbl_eu_80519108, 4) == 0 ? 1 : 0;
}
