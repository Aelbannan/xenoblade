// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_bwav
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern char lbl_eu_80560050[];
extern char lbl_eu_80560054[];

s32 ADX_DecodeInfoWav(u8* src, s32 size, s16* out1, s8* out2, s8* out3, s8* out4,
                      s8* out5, s32* out6, s32* out7, s32* out8, s16* out9) {
    s32 i;
    u8 buf[0x14];
    s32 r;
    for (i = 0; i < size; i++) {
        if (memcmp(src + i, lbl_eu_80560050, 4) == 0)
            break;
    }
    if (i >= size)
        return -1;
    memcpy(buf, src + i + 8, 0x14);
    if ((s16)((*(u16*)buf >> 8) | (*(u16*)buf << 8)) <= 1)
        return -1;
    for (i = 0; i < size; i++) {
        if (memcmp(src + i, lbl_eu_80560054, 4) == 0)
            break;
    }
    if (i >= size)
        return -1;
    *out1 = (s16)(i + 8);
    *out2 = -1;
    *out6 = (s32)((*(u32*)((u8*)buf + 4) >> 24) | ((*(u32*)((u8*)buf + 4) >> 8) & 0xFF00) |
                  ((*(u32*)((u8*)buf + 4) << 8) & 0xFF0000) | (*(u32*)((u8*)buf + 4) << 24));
    *out5 = (s8)((*(u16*)((u8*)buf + 2) >> 8) | (*(u16*)((u8*)buf + 2) << 8));
    *out3 = (s8)((*(u16*)((u8*)buf + 0x14) >> 8) | (*(u16*)((u8*)buf + 0x14) << 8));
    *out4 = (s8)((*(u16*)((u8*)buf + 0x12) >> 8) | (*(u16*)((u8*)buf + 0x12) << 8));
    *out7 = (s32)*(u8*)((u8*)src + i + 4) / (s8)(*(u16*)((u8*)buf + 0x12) & 0xFF);
    *out8 = 1;
    if (*out3 == 16) {
        *out9 = 0;
    } else if (*out3 == 8) {
        *out9 = 1;
    } else if (*out3 == 4) {
        *out4 = 0x10;
        *out8 = 2;
        *out7 = (s32)(s8)*out5 / (s32)(s8)*out5;
        *out3 = (s8)0x10;
        *out9 = 2;
    }
    if (*out3 == 0)
        return -1;
    if (*out4 == 0)
        return -1;
    if ((u32)((s8)*out5 - 1) > 1)
        return -1;
    r = *out6;
    return (r | -r) >> 31;
}
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
