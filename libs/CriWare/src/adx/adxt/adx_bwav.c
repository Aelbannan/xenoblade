// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_bwav
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern u32 lbl_eu_80560050;
extern u32 lbl_eu_80560054;

// ADXB (ADX Buffer) WAV decode object. Byte layout mirrors the sibling
// AdxBsp in adx_bsps.c (same shared ADXB context); recovered from the
// retail ADXB_DecodeHeaderWav / ADXB_ExecOneWav* ASM.
struct AdxBwav {
    char field_0x0[2];  // 0x00
    s16 field_0x2;      // 0x02
    s32 field_0x4;      // 0x04 - decode state
    void* field_0x8;    // 0x08 - ADXPD handle
    s8 field_0xC;       // 0x0C - outInfo (written via s32* by DecodeInfo)
    u8 field_0xD;       // 0x0D - outVer
    s8 field_0xE;       // 0x0E - channel count / mode selector
    s8 field_0xF;       // 0x0F - bits per sample
    u32 field_0x10;     // 0x10 - outBlk
    u32 field_0x14;     // 0x14 - outNum
    u32 field_0x18;     // 0x18 - outSmp
    s16 field_0x1C;     // 0x1C
    char field_0x1E[2]; // 0x1E
    u32 field_0x20;     // 0x20
    s16 field_0x24;     // 0x24
    s16 field_0x26;     // 0x26
    u32 field_0x28;     // 0x28
    u32 field_0x2C;     // 0x2C
    u32 field_0x30;     // 0x30
    u32 field_0x34;     // 0x34
    char field_0x38[4]; // 0x38
    u32 field_0x3C;     // 0x3C
    u32 field_0x40;     // 0x40
    u32 field_0x44;     // 0x44
    void* field_0x48;   // 0x48 - source data ptr
    s32 field_0x4C;     // 0x4C - maxBlks
    s32 field_0x50;     // 0x50
    s32 field_0x54;     // 0x54
    u32 field_0x58;     // 0x58
    u32 field_0x5C;     // 0x5C - pcmBase
    u32 field_0x60;     // 0x60
    u32 field_0x64;     // 0x64
    u32 field_0x68;     // 0x68 - pcmOfst
    u32 field_0x6C;     // 0x6C - availWrPos
    u32 field_0x70;     // 0x70 - wrPos
    char field_0x74[4]; // 0x74
    void* field_0x78;   // 0x78 - getWr callback
    void* field_0x7C;   // 0x7C - getWr context
    void* field_0x80;   // 0x80 - end-of-stream callback
    void* field_0x84;   // 0x84 - eos context
    u32 field_0x88;     // 0x88
    u32 field_0x8C;     // 0x8C
    u32 field_0x90;     // 0x90 - decSmpl
    u32 field_0x94;     // 0x94 - decDtLen
    s16 field_0x98;     // 0x98
    char field_0x9A[2]; // 0x9A
    s16 field_0x9C;     // 0x9C - codec
};

s32 ADX_DecodeInfoWav(u8* src, s32 size, s16* out1, s8* out2, s8* out3, s8* out4,
                      s8* out5, s32* out6, s32* out7, s32* out8, s16* out9) {
    s32 i;
    u8 buf[0x14];
    u32 dataSize;
    const void* tag1 = (const void*)lbl_eu_80560050;
    const void* tag2;
    s32 r;
    for (i = 0; i < size; i++) {
        if (memcmp(src + i, tag1, 4) == 0)
            break;
    }
    if (i == size)
        return -1;
    {
        u8* p = src + i + 8;
        memcpy(buf, p, 0x14);
    }
    if ((s16)(((*(u16*)buf & 0xFF00) >> 8) | ((*(u16*)buf & 0xFF) << 8)) > 1)
        return -1;
    tag2 = (const void*)lbl_eu_80560054;
    for (i = 0; i < size; i++) {
        if (memcmp(src + i, tag2, 4) == 0)
            break;
    }
    if (i == size)
        return -1;
    dataSize = (u32)src[i + 4] | ((u32)src[i + 5] << 8) | ((u32)src[i + 6] << 16) |
               ((u32)src[i + 7] << 24);
    *out1 = (s16)(i + 8);
    *out2 = -1;
    *out6 = (s32)(((*(u32*)((u8*)buf + 4) & 0xFF000000u) >> 24) |
                  ((*(u32*)((u8*)buf + 4) & 0xFF0000u) >> 8) |
                  ((*(u32*)((u8*)buf + 4) & 0xFF00u) << 8) |
                  ((*(u32*)((u8*)buf + 4) & 0xFFu) << 24));
    *out5 = (s8)(((*(u16*)((u8*)buf + 2) & 0xFF00) >> 8) | ((*(u16*)((u8*)buf + 2) & 0xFF) << 8));
    *out3 = (s8)(((*(u16*)((u8*)buf + 14) & 0xFF00) >> 8) | ((*(u16*)((u8*)buf + 14) & 0xFF) << 8));
    {
        u16 blkAlign = (u16)(((*(u16*)((u8*)buf + 12) & 0xFF00) >> 8) | ((*(u16*)((u8*)buf + 12) & 0xFF) << 8));
        *out4 = (s8)(blkAlign & 0xFF);
        *out7 = (s32)dataSize / (s8)(blkAlign & 0xFF);
    }
    *out8 = 1;
    if ((s8)*out3 == 16) {
        *out9 = 0;
    } else if ((s8)*out3 == 8) {
        *out9 = 1;
    } else if ((s8)*out3 == 4) {
        *out4 = (s8)((s8)*out5 * 2);
        *out8 = 4;
        *out7 = ((s32)(dataSize + (dataSize >> 31)) >> 1) / (s8)*out5;
        *out3 = (s8)0x10;
        *out9 = 2;
    }
    if ((s8)*out3 == 0)
        return -1;
    if ((s8)*out4 == 0)
        return -1;
    if ((u32)((u8)((s8)*out5 - 1)) > 1)
        return -1;
    r = *out6;
    return (r == 0) ? -1 : 0;
}

extern u32 ADXPD_GetStat(void* self);

// Decode the WAV header into the ADXB context. Returns the block size (out9
// from ADX_DecodeInfoWav) or the negative error code on failure.
s32 ADXB_DecodeHeaderWav(struct AdxBwav* self, const u8* data, s32 size)
{
    s16 outBps;
    s32 r;

    self->field_0x2 = 1;
    r = ADX_DecodeInfoWav((u8*)data, size, &outBps, &self->field_0xC,
            (s8*)&self->field_0xD, &self->field_0xF, &self->field_0xE,
            (s32*)&self->field_0x14, (s32*)&self->field_0x18,
            (s32*)&self->field_0x10, &self->field_0x9C);
    if (r < 0)
        return r;

    self->field_0x1C = 0;
    self->field_0x26 = 0;
    self->field_0x24 = 0;
    self->field_0x34 = 0;
    self->field_0x30 = 0;
    self->field_0x2C = 0;
    self->field_0x28 = 0;
    self->field_0x20 = 0;
    self->field_0x50 = self->field_0xE;
    self->field_0x54 = self->field_0xF;
    self->field_0x58 = self->field_0x10;
    self->field_0x5C = self->field_0x3C;
    self->field_0x60 = self->field_0x40;
    self->field_0x64 = self->field_0x44;
    self->field_0x8C = 0;
    self->field_0x88 = 0;
    self->field_0x98 = 1;
    return outBps;
}

// Deinterleave one block of 16-bit WAV PCM: source samples are little-endian,
// so each one is byte-swapped (sthbrx) on the way to the big-endian buffers.
void ADXB_ExecOneWav16(struct AdxBwav* self)
{
    u16* src;
    s32 w;
    s8 ch;

    src = (u16*)self->field_0x48;
    if (self->field_0x4 == 1) {
        if (ADXPD_GetStat(self->field_0x8) == 0) {
            void (*getWr)(void*, u32*, u32*, u32*);
            getWr = (void (*)(void*, u32*, u32*, u32*))self->field_0x78;
            getWr(self->field_0x7C, &self->field_0x68, &self->field_0x6C,
                  &self->field_0x70);

            w = (s32)self->field_0x60 - (s32)self->field_0x68;
            if (w > (s32)self->field_0x6C)
                w = (s32)self->field_0x6C;
            if (w > self->field_0x4C)
                w = self->field_0x4C;

            ch = self->field_0xE;
            if (ch == 2) {
                s16* dst1 = (s16*)((u8*)self->field_0x5C + (self->field_0x68 << 1));
                s16* dst2 = (s16*)((u8*)self->field_0x5C + ((self->field_0x68 + self->field_0x64) << 1));
                u16* s = src;
                s32 i;
                for (i = 0; i < w; i++) {
                    __sthbrx(*s, &dst1[i], 0);
                    __sthbrx(*(s + 1), &dst2[i], 0);
                    s += 2;
                }
            } else {
                s16* dst1 = (s16*)((u8*)self->field_0x5C + (self->field_0x68 << 1));
                s32 i;
                for (i = 0; i < w; i++)
                    __sthbrx(src[i], &dst1[i], 0);
            }
            self->field_0x90 = (u32)w;
            self->field_0x4 = 2;
            self->field_0x94 = (u32)(self->field_0xE * (w << 1));
        }
    }

    if (self->field_0x4 == 2) {
        void (*eosCb)(void*, u32, u32);
        eosCb = (void (*)(void*, u32, u32))self->field_0x80;
        eosCb(self->field_0x84, self->field_0x94, self->field_0x90);
        self->field_0x4 = 3;
    }
}

// One block of 8-bit WAV PCM: unsigned bytes are centered (b - 0x80) and
// stored in the high byte of each 16-bit output sample.
void ADXB_ExecOneWav8(struct AdxBwav* self)
{
    u8* src;
    s32 w;
    s8 ch;

    src = (u8*)self->field_0x48;
    if (self->field_0x4 == 1) {
        if (ADXPD_GetStat(self->field_0x8) == 0) {
            void (*getWr)(void*, u32*, u32*, u32*);
            getWr = (void (*)(void*, u32*, u32*, u32*))self->field_0x78;
            getWr(self->field_0x7C, &self->field_0x68, &self->field_0x6C,
                  &self->field_0x70);

            w = (s32)self->field_0x60 - (s32)self->field_0x68;
            if (w > (s32)self->field_0x6C)
                w = (s32)self->field_0x6C;
            if (w > self->field_0x4C)
                w = self->field_0x4C;

            ch = self->field_0xE;
            if (ch == 2) {
                s16* dst1 = (s16*)((u8*)self->field_0x5C + (self->field_0x68 << 1));
                s16* dst2 = (s16*)((u8*)self->field_0x5C + ((self->field_0x68 + self->field_0x64) << 1));
                s32 i;
                for (i = 0; i < w; i++) {
                    dst1[i] = (u16)(u8)(src[2 * i] - 0x80) << 8;
                    dst2[i] = (u16)(u8)(src[2 * i + 1] - 0x80) << 8;
                }
            } else {
                s16* dst1 = (s16*)((u8*)self->field_0x5C + (self->field_0x68 << 1));
                s32 i;
                for (i = 0; i < w; i++)
                    dst1[i] = (u16)(u8)(src[i] - 0x80) << 8;
            }
            self->field_0x90 = (u32)w;
            self->field_0x4 = 2;
            self->field_0x94 = (u32)(self->field_0xE * w);
        }
    }

    if (self->field_0x4 == 2) {
        void (*eosCb)(void*, u32, u32);
        eosCb = (void (*)(void*, u32, u32))self->field_0x80;
        eosCb(self->field_0x84, self->field_0x94, self->field_0x90);
        self->field_0x4 = 3;
    }
}

// One block of 4-bit WAV PCM: two 4-bit samples per byte, packed
// little-endian into the 16-bit output words.
void ADXB_ExecOneWav4(struct AdxBwav* self)
{
    u8* src;
    s32 w;
    s8 ch;

    src = (u8*)self->field_0x48;
    if (self->field_0x4 == 1) {
        if (ADXPD_GetStat(self->field_0x8) == 0) {
            void (*getWr)(void*, u32*, u32*, u32*);
            getWr = (void (*)(void*, u32*, u32*, u32*))self->field_0x78;
            getWr(self->field_0x7C, &self->field_0x68, &self->field_0x6C,
                  &self->field_0x70);

            w = (s32)self->field_0x60 - (s32)self->field_0x68;
            if (w > (s32)self->field_0x6C)
                w = (s32)self->field_0x6C;
            if (w > self->field_0x4C)
                w = self->field_0x4C;

            ch = self->field_0xE;
            if (ch == 2) {
                s16* dst1 = (s16*)((u8*)self->field_0x5C + (self->field_0x68 << 1));
                s16* dst2 = (s16*)((u8*)self->field_0x5C + ((self->field_0x68 + self->field_0x64) << 1));
                s32 i;
                for (i = 0; i < w; i++) {
                    dst1[i] = (u16)(src[4 * i] | (src[4 * i + 2] << 8));
                    dst2[i] = (u16)(src[4 * i + 1] | (src[4 * i + 3] << 8));
                }
            } else {
                s16* dst1 = (s16*)((u8*)self->field_0x5C + (self->field_0x68 << 1));
                s32 i;
                for (i = 0; i < w; i++)
                    dst1[i] = (u16)(src[2 * i] | (src[2 * i + 1] << 8));
            }
            self->field_0x90 = (u32)w;
            self->field_0x4 = 2;
            self->field_0x94 = (u32)(self->field_0xE * (w << 1));
        }
    }

    if (self->field_0x4 == 2) {
        void (*eosCb)(void*, u32, u32);
        eosCb = (void (*)(void*, u32, u32))self->field_0x80;
        eosCb(self->field_0x84, self->field_0x94, self->field_0x90);
        self->field_0x4 = 3;
    }
}

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
