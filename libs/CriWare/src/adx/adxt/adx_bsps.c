// Auto-scaffolded catalog TU for CriWare/src/adx/adxt/adx_bsps
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

// ADXB (ADX Buffer) SPSD decode object. Byte layout recovered from
// ADXB_DecodeHeaderSpsd / ADXB_ExecOneSpsd retail ASM and cross-checked
// against the sibling ADXB decode helpers in adx_bsc.c.
struct AdxBsp {
    char field_0x0[2]; // 0x00
    s16 field_0x2;     // 0x02
    s32 field_0x4;     // 0x04 - decode state
    void* field_0x8;   // 0x08 - ADXPD handle
    s8 field_0xC;      // 0x0C - outCodec
    u8 field_0xD;      // 0x0D - outVer
    s8 field_0xE;      // 0x0E - outX / channel-mode selector
    s8 field_0xF;      // 0x0F - outCh (channel count)
    u32 field_0x10;    // 0x10 - outBlk
    u32 field_0x14;    // 0x14 - outNum
    u32 field_0x18;    // 0x18 - outSmp
    s16 field_0x1C;    // 0x1C
    char field_0x1E[2];// 0x1E
    u32 field_0x20;    // 0x20
    s16 field_0x24;    // 0x24
    s16 field_0x26;    // 0x26
    u32 field_0x28;    // 0x28
    u32 field_0x2C;    // 0x2C
    u32 field_0x30;    // 0x30
    u32 field_0x34;    // 0x34
    char field_0x38[4];// 0x38
    u32 field_0x3C;    // 0x3C
    u32 field_0x40;    // 0x40
    u32 field_0x44;    // 0x44
    void* field_0x48;  // 0x48 - source data ptr
    s32 field_0x4C;    // 0x4C - maxBlks
    s32 field_0x50;    // 0x50 - blkSmpl
    s32 field_0x54;    // 0x54 - numBlkSmpl
    u32 field_0x58;    // 0x58 - blkSize
    u32 field_0x5C;    // 0x5C - pcmBase
    s32 field_0x60;    // 0x60 - pcmBufSize
    s32 field_0x64;    // 0x64 - workBufSize
    s32 field_0x68;    // 0x68 - pcmOfst
    s32 field_0x6C;    // 0x6C - availWrPos
    s32 field_0x70;    // 0x70 - wrPos
    char field_0x74[4];// 0x74
    // getWr callback: reports (pcmOfst, availWrPos, wrPos) to the caller context.
    // Both callbacks receive an opaque, caller-owned context object.
    void (*field_0x78)(void*, s32*, s32*, s32*);  // 0x78 - getWr callback
    struct AdxBspCbCtx* field_0x7C;               // 0x7C - getWr context
    void (*field_0x80)(void*, u32, u32);          // 0x80 - end-of-stream callback
    struct AdxBspCbCtx* field_0x84;               // 0x84 - eos context
    u32 field_0x88;    // 0x88
    u32 field_0x8C;    // 0x8C
    u32 field_0x90;    // 0x90 - decSmpl
    u32 field_0x94;    // 0x94 - decDtLen
    s16 field_0x98;    // 0x98
    char field_0x9A[2];// 0x9A
    s16 field_0x9C;    // 0x9C - outX2
};

// Opaque caller-owned context passed back through the getWr / EOS callbacks;
// the layout is private to the registering module.
struct AdxBspCbCtx;

// ADX SPSD info block layout (subset consumed by ADX_DecodeInfoSpsd).
struct AdxsInfo {
    char field_0x0[7]; // 0x00
    u8 field_0x7;      // 0x07 - block size low bits
    u8 field_0x8;      // 0x08 - codec/version selector
    u8 field_0x9;      // 0x09 - channel flags
    char field_0xA[2]; // 0x0A
    s32 field_0xC;     // 0x0C - total sample count
    char field_0x10[0x1A];
    u16 field_0x2A;    // 0x2A
};

int ADX_DecodeInfoSpsd(struct AdxsInfo *hdr, int size, s16 *outBps, s8 *outCodec,
    u8 *outVer, s8 *outCh, s8 *outX, u32 *outNum, u32 *outSmp,
    u32 *outBlk, s16 *outX2)
{
    *outBps = hdr->field_0x7 << 4;
    *outX = (hdr->field_0x9 & 3) + 1;
    *outNum = hdr->field_0x2A;

    // Selector codec switch: MWCC emits a decision tree - merged cases 2/3
    // (4-bit family) are range-tested first, then equality tests for 0 and 1.
    switch (hdr->field_0x8) {
    case 0:
        *outVer = 0x10;
        *outCh = *outX << 1;
        *outBlk = 1;
        *outSmp = hdr->field_0xC / 2;
        *outX2 = 0;
        break;
    case 1:
        *outVer = 8;
        *outCh = *outX;
        *outBlk = 1;
        *outSmp = hdr->field_0xC;
        *outX2 = 1;
        break;
    case 2:
    case 3:
        *outVer = 4;
        *outCh = *outX;
        *outBlk = 2;
        *outSmp = hdr->field_0xC * 2;
        *outX2 = 2;
        break;
    }

    // SPSD output is always fixed stereo/16-bit regardless of the source codec.
    *outCh = 2;
    *outBlk = 1;
    *outSmp = hdr->field_0xC / 2;
    *outVer = 0x10;
    *outCodec = -1;
    return 0;
}

s16 ADXB_DecodeHeaderSpsd(struct AdxBsp* self, const u8* data, s32 size)
{
    u16 outBps;

    self->field_0x2 = 1;
    if (ADX_DecodeInfoSpsd((struct AdxsInfo*)data, size, (s16*)&outBps, &self->field_0xC,
            &self->field_0xD, &self->field_0xF, &self->field_0xE,
            &self->field_0x14, &self->field_0x18, &self->field_0x10,
            &self->field_0x9C) < 0) {
        return 0;
    }

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
    self->field_0x98 = 2;
    return outBps;
}

extern u32 ADXPD_GetStat(void* self);

void ADXB_ExecOneSpsd(struct AdxBsp* self)
{
    u16* dst1;
    s32 i;
    // Source sample buffer, kept live across the whole function (r31).
    u16* src = (u16*)self->field_0x48;
    s32 w;

    if (self->field_0x4 == 1 && ADXPD_GetStat(self->field_0x8) == 0) {
        // Query the write position via the registered getWr callback
        self->field_0x78(
            self->field_0x7C, &self->field_0x68, &self->field_0x6C, &self->field_0x70);

        // Clamp the decodable count: space to buffer end, writer distance, caller limit.
        w = self->field_0x60 - self->field_0x68;
        if (w > self->field_0x6C) w = self->field_0x6C;
        if (w > self->field_0x4C) w = self->field_0x4C;

        // dst1 = pcmBase + pos*2 bytes; stereo splits L/R into two half buffers.
        {
            dst1 = (u16*)(self->field_0x5C + (self->field_0x68 << 1));
            if (self->field_0xE == 2) {
                u16* dst2 = (u16*)(self->field_0x5C + ((self->field_0x64 + self->field_0x68) << 1));
                for (i = 0; i < w; i++) {
                    dst1[i] = src[i * 2];
                    dst2[i] = src[i * 2 + 1];
                }
            } else {
                for (i = 0; i < w; i++) {
                    dst1[i] = src[i];
                }
            }
        }
        self->field_0x90 = (u32)w;
        self->field_0x94 = (u32)(self->field_0xE * (w << 1));
        self->field_0x4 = 2;
    }

    if (self->field_0x4 == 2) {
        // End-of-stream callback with (decDtLen, decSmpl)
        self->field_0x80(self->field_0x84, self->field_0x94, self->field_0x90);
        self->field_0x4 = 3;
    }
}

s32 memcmp(const void* s1, const void* s2, size_t n);
extern const u8 lbl_eu_80519108[4];
s32 ADXB_CheckSpsd(const void* buf) {
    return memcmp(buf, lbl_eu_80519108, 4) == 0 ? 1 : 0;
}
