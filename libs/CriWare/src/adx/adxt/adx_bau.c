// ADX Audio Unit functions
// libs/CriWare/src/adx/adxt/adx_bau.c

#include <harness_catalog.h>
#include <string.h>

__declspec(section ".data") __attribute__((aligned(8))) const u16 lbl_eu_80565E50[256] = {0x8284,0x8684,0x8A84,0x8E84,0x9284,0x9684,0x9A84,0x9E84,0xA284,0xA684,0xAA84,0xAE84,0xB284,0xB684,0xBA84,0xBE84,0xC184,0xC384,0xC584,0xC784,0xC984,0xCB84,0xCD84,0xCF84,0xD184,0xD384,0xD584,0xD784,0xD984,0xDB84,0xDD84,0xDF84,0xE104,0xE204,0xE304,0xE404,0xE504,0xE604,0xE704,0xE804,0xE904,0xEA04,0xEB04,0xEC04,0xED04,0xEE04,0xEF04,0xF004,0xF0C4,0xF144,0xF1C4,0xF244,0xF2C4,0xF344,0xF3C4,0xF444,0xF4C4,0xF544,0xF5C4,0xF644,0xF6C4,0xF744,0xF7C4,0xF844,0xF8A4,0xF8E4,0xF924,0xF964,0xF9A4,0xF9E4,0xFA24,0xFA64,0xFAA4,0xFAE4,0xFB24,0xFB64,0xFBA4,0xFBE4,0xFC24,0xFC64,0xFC94,0xFCB4,0xFCD4,0xFCF4,0xFD14,0xFD34,0xFD54,0xFD74,0xFD94,0xFDB4,0xFDD4,0xFDF4,0xFE14,0xFE34,0xFE54,0xFE74,0xFE8C,0xFE9C,0xFEAC,0xFEBC,0xFECC,0xFEDC,0xFEEC,0xFEFC,0xFF0C,0xFF1C,0xFF2C,0xFF3C,0xFF4C,0xFF5C,0xFF6C,0xFF7C,0xFF88,0xFF90,0xFF98,0xFFA0,0xFFA8,0xFFB0,0xFFB8,0xFFC0,0xFFC8,0xFFD0,0xFFD8,0xFFE0,0xFFE8,0xFFF0,0xFFF8,0x0000,0x7D7C,0x797C,0x757C,0x717C,0x6D7C,0x697C,0x657C,0x617C,0x5D7C,0x597C,0x557C,0x517C,0x4D7C,0x497C,0x457C,0x417C,0x3E7C,0x3C7C,0x3A7C,0x387C,0x367C,0x347C,0x327C,0x307C,0x2E7C,0x2C7C,0x2A7C,0x287C,0x267C,0x247C,0x227C,0x207C,0x1EFC,0x1DFC,0x1CFC,0x1BFC,0x1AFC,0x19FC,0x18FC,0x17FC,0x16FC,0x15FC,0x14FC,0x13FC,0x12FC,0x11FC,0x10FC,0x0FFC,0x0F3C,0x0EBC,0x0E3C,0x0DBC,0x0D3C,0x0CBC,0x0C3C,0x0BBC,0x0B3C,0x0ABC,0x0A3C,0x09BC,0x093C,0x08BC,0x083C,0x07BC,0x075C,0x071C,0x06DC,0x069C,0x065C,0x061C,0x05DC,0x059C,0x055C,0x051C,0x04DC,0x049C,0x045C,0x041C,0x03DC,0x039C,0x036C,0x034C,0x032C,0x030C,0x02EC,0x02CC,0x02AC,0x028C,0x026C,0x024C,0x022C,0x020C,0x01EC,0x01CC,0x01AC,0x018C,0x0174,0x0164,0x0154,0x0144,0x0134,0x0124,0x0114,0x0104,0x00F4,0x00E4,0x00D4,0x00C4,0x00B4,0x00A4,0x0094,0x0084,0x0078,0x0070,0x0068,0x0060,0x0058,0x0050,0x0048,0x0040,0x0038,0x0030,0x0028,0x0020,0x0018,0x0010,0x0008,0x0000};
__declspec(section ".rodata") __attribute__((aligned(8))) const u8 lbl_eu_80519110[16] = {0x2E,0x73,0x6E,0x64,0x00,0x2E,0x73,0x64,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
__declspec(section ".rodata") __attribute__((aligned(8))) const char lbl_eu_80519120[44] = "\nAXRNA Ver.1.15 Build:Oct 28 2009 21:11:24\n\x00";

// Byte-swap helper
static u32 read_be32(const u8* p) {
    u32 v = p[0] | (p[1] << 8) | (p[2] << 16) | ((u32)p[3] << 24);
    return (v >> 24) | ((v >> 8) & 0xFF00) | ((v << 8) & 0xFF0000) | (v << 24);
}

static u16 read_be16(const u8* p) {
    return ((u16)p[0] << 8) | (u16)p[1];
}

// Workspace struct for the ADX banked AU decoder (offsets from retail)
typedef struct ADXB_AU ADXB_AU;
struct ADXB_AU {
    u8 field_0x00[2];
    s16 field_0x02;
    s32 field_0x04;
    void* field_0x08;
    u8 field_0x0C;
    u8 field_0x0D;
    u8 field_0x0E;
    u8 field_0x0F;
    s32 field_0x10;
    s32 field_0x14;
    s32 field_0x18;
    s16 field_0x1C;
    u8 pad_0x1E[2];
    s32 field_0x20;
    s16 field_0x24;
    s16 field_0x26;
    s32 field_0x28;
    s32 field_0x2C;
    s32 field_0x30;
    s32 field_0x34;
    u8 pad_0x38[4];
    s32 field_0x3C;
    s32 field_0x40;
    s32 field_0x44;
    void* field_0x48;
    s32 field_0x4C;
    s32 field_0x50;
    s32 field_0x54;
    s32 field_0x58;
    u8* field_0x5C;
    s32 field_0x60;
    s32 field_0x64;
    s32 field_0x68;
    s32 field_0x6C;
    s32 field_0x70;
    u8 pad_0x74[4];
    void (*field_0x78)(void*, u32*, u32*, u32*);
    void* field_0x7C;
    void (*field_0x80)(void*, u32, u32);
    void* field_0x84;
    s32 field_0x88;
    s32 field_0x8C;
    s32 field_0x90;
    s32 field_0x94;
    s16 field_0x98;
    s16 field_0x9C;
};

extern u32 ADXPD_GetStat(void* pd);

u8* AU_GetInfo(u8* data, s32 maxBytes, s32* outSampleRate, s32* outTotalSamples,
               s32* outBlockSize, s32* outChannelCount, s32* outFormat) {
    // Big-endian AU (.snd) header: magic@0, dataOfst@4, encoding@0xC, dataSize@8,
    // sampleRate@0x10, channelCount@0x14
    u32 sig = data[0] | (data[1] << 8) | (data[2] << 16) | ((u32)data[3] << 24);
    if (sig != 0x0064732E && sig != 0x646E732E) {
        return NULL;
    }

    u32 hdrSize = read_be32(data + 4);
    if ((s32)hdrSize > maxBytes) {
        return NULL;
    }

    u32 formatId = read_be32(data + 0xC);
    u32 dataSize = read_be32(data + 8);

    if (formatId == 1) {
        *outFormat = 2;
        *outBlockSize = 8;
    } else if (formatId == 2) {
        *outFormat = 1;
        *outBlockSize = 8;
    } else if (formatId == 3) {
        *outFormat = 0;
        *outBlockSize = 0x10;
    } else {
        return NULL;
    }

    s32 sampleRate = (s32)read_be32(data + 0x10);
    s32 chanCount = (s32)read_be32(data + 0x14);
    *outSampleRate = sampleRate;
    *outTotalSamples = chanCount;

    s32 fmt = *outFormat;
    if (fmt == 2) {
        *outChannelCount = (s32)dataSize / chanCount;
    } else if (fmt == 1) {
        *outChannelCount = (s32)dataSize / chanCount;
    } else if (fmt == 0) {
        // 16-bit samples: bytes/2 then per-channel
        *outChannelCount = ((s32)dataSize / 2) / chanCount;
    } else {
        *outChannelCount = 0x7FFF;
    }

    return data + hdrSize;
}

s32 ADXB_CheckAu(void* data) {
    if (memcmp(data, lbl_eu_80519110, 4) == 0)
        goto ok;
    if (memcmp(data, lbl_eu_80519110 + 5, 4) != 0)
        goto fail;
ok:
    return 1;
fail:
    return 0;
}

s32 ADX_DecodeInfoAu(u8* data, s32 maxBytes, s16* outHeaderSize,
                     s8* field_C, s8* field_D, s8* field_F, s8* field_E,
                     s32* field_14, s32* field_18, s32* field_10, s32* outFormat) {
    s32 sampleRate = 0;
    s32 chanCount = 0;
    s32 blockSize = 0;
    s32 dataSize = 0;

    if (maxBytes < 8) {
        *outHeaderSize = 0;
        return -1;
    }

    // AU_GetInfo writes: sampleRate, chanCount, blockSize, dataSize and the
    // raw encoding id through outFormat.
    u8* end = AU_GetInfo(data, maxBytes, &sampleRate, &chanCount, &blockSize,
                         &dataSize, outFormat);
    if (end == NULL) {
        return -1;
    }

    s16 consumed = (s16)(end - data);
    *outHeaderSize = consumed;
    if (consumed <= 0) {
        return -1;
    }

    *field_14 = sampleRate;
    *field_E = (s8)chanCount;
    *field_D = (s8)blockSize;
    *field_18 = dataSize;
    *field_C = -1;
    // bits per sample / 8: signed chars multiplied, rounded division by 8
    *field_F = *field_E * *field_D / 8;
    *field_10 = 1;

    return 0;
}

s16 ADXB_DecodeHeaderAu(void* self, u8* data, s32 maxBytes) {
    ADXB_AU* p = (ADXB_AU*)self;
    s16 headerSize;
    s32 fmtLocal;

    p->field_0x02 = 1;

    s32 result = ADX_DecodeInfoAu(data, maxBytes, &headerSize,
        (s8*)&p->field_0x0C, (s8*)&p->field_0x0D, (s8*)&p->field_0x0F,
        (s8*)&p->field_0x0E,
        &p->field_0x14, &p->field_0x18, &p->field_0x10, &fmtLocal);

    if (result < 0) {
        return 0;
    }

    // Reset decoder state fields
    p->field_0x1C = 0;
    p->field_0x26 = 0;
    p->field_0x24 = 0;
    p->field_0x34 = 0;
    p->field_0x30 = 0;
    p->field_0x2C = 0;
    p->field_0x28 = 0;
    p->field_0x20 = 0;

    s32 ch = (s8)p->field_0x0E;
    s32 blk = (s8)p->field_0x0F;
    s32 v10 = p->field_0x10;
    s32 v3c = p->field_0x3C;
    s32 v40 = p->field_0x40;
    s32 v44 = p->field_0x44;

    p->field_0x50 = ch;
    p->field_0x54 = blk;
    p->field_0x58 = v10;
    p->field_0x5C = (u8*)v3c;
    p->field_0x60 = v40;
    p->field_0x64 = v44;
    p->field_0x8C = 0;
    p->field_0x88 = 0;
    p->field_0x98 = 4;
    p->field_0x9C = (s16)fmtLocal;

    return headerSize;
}

void ADXB_ExecOneAu16(void* self) {
    ADXB_AU* p = (ADXB_AU*)self;
    u16* src = (u16*)p->field_0x48;
    s32 i;

    if (p->field_0x04 == 1) {
        if (ADXPD_GetStat(p->field_0x08) != 0) {
            goto done;
        }

        // Pull a block of source samples via callback
        p->field_0x78(p->field_0x7C, (u32*)&p->field_0x68, (u32*)&p->field_0x6C,
                      (u32*)&p->field_0x70);

        s32 remain = p->field_0x60 - p->field_0x68;
        if (remain > p->field_0x6C) {
            remain = p->field_0x6C;
        }
        if (remain > p->field_0x4C) {
            remain = p->field_0x4C;
        }

        s8 chanCount = p->field_0x0E;
        u16* dstL = (u16*)(p->field_0x5C + p->field_0x68 * 2);

        if (chanCount == 2) {
            // Stereo: deinterleave L/R and byte-swap big-endian s16 pairs
            u16* dstR = (u16*)(p->field_0x5C + (p->field_0x68 + p->field_0x64) * 2);
            for (i = 0; i < remain; i++) {
                s16 l = *src++;
                s16 r = *src++;
                *dstL++ = (u16)((l << 8) | (l >> 8));
                *dstR++ = (u16)((r << 8) | (r >> 8));
            }
        } else {
            // Mono: byte-swap big-endian s16 in place
            for (i = 0; i < remain; i++) {
                s16 v = *src++;
                *dstL++ = (u16)((v << 8) | (v >> 8));
            }
        }

        p->field_0x90 = remain;
        p->field_0x04 = 2;
        p->field_0x94 = chanCount * (remain * 2);
    }

done:
    if (p->field_0x04 == 2) {
        p->field_0x80(p->field_0x84, p->field_0x94, p->field_0x90);
        p->field_0x04 = 3;
    }
}

void ADXB_ExecOneAu8(void* self) {
    ADXB_AU* p = (ADXB_AU*)self;
    u8* src = (u8*)p->field_0x48;
    s32 i;

    if (p->field_0x04 == 1) {
        if (ADXPD_GetStat(p->field_0x08) != 0) {
            goto done;
        }

        // Pull a block of source samples via callback
        p->field_0x78(p->field_0x7C, (u32*)&p->field_0x68, (u32*)&p->field_0x6C,
                      (u32*)&p->field_0x70);

        s32 remain = p->field_0x60 - p->field_0x68;
        if (remain > p->field_0x6C) {
            remain = p->field_0x6C;
        }
        if (remain > p->field_0x4C) {
            remain = p->field_0x4C;
        }

        s8 chanCount = p->field_0x0E;
        u16* dstL = (u16*)(p->field_0x5C + p->field_0x68 * 2);

        if (chanCount == 2) {
            // Stereo: deinterleave and expand 8-bit to signed 16-bit
            u16* dstR = (u16*)(p->field_0x5C + (p->field_0x68 + p->field_0x64) * 2);
            for (i = 0; i < remain; i++) {
                dstL[i] = (u16)(src[i * 2] << 8);
                dstR[i] = (u16)(src[i * 2 + 1] << 8);
            }
        } else {
            // Mono: expand 8-bit to signed 16-bit
            for (i = 0; i < remain; i++) {
                dstL[i] = (u16)(src[i] << 8);
            }
        }

        p->field_0x90 = remain;
        p->field_0x04 = 2;
        p->field_0x94 = chanCount * remain;
    }

done:
    if (p->field_0x04 == 2) {
        p->field_0x80(p->field_0x84, p->field_0x94, p->field_0x90);
        p->field_0x04 = 3;
    }
}

// Ulaw decoder: pulls a block via callback, expands 8-bit ulaw samples to
// 16-bit PCM through the lookup table. MWCC unrolls the copy loops x8.
void ADXB_ExecOneAuUlaw(void* self) {
    ADXB_AU* p = (ADXB_AU*)self;
    u8* src = p->field_0x48;
    s32 i;
    s32 j;

    if (p->field_0x04 == 1) {
        if (ADXPD_GetStat(p->field_0x08) != 0) {
            goto done;
        }

        // Pull a block of source samples via callback
        p->field_0x78(p->field_0x7C, (u32*)&p->field_0x68, (u32*)&p->field_0x6C,
                      (u32*)&p->field_0x70);

        s32 remain = p->field_0x60 - p->field_0x68;
        remain = remain > p->field_0x6C ? p->field_0x6C : remain;
        remain = remain > p->field_0x4C ? p->field_0x4C : remain;

        u16* dstL = (u16*)(p->field_0x5C + p->field_0x68 * 2);

        if ((s8)p->field_0x0E == 2) {
            // Stereo: deinterleave ulaw sample pairs through the decode table
            u16* dstR = (u16*)(p->field_0x5C + (p->field_0x64 + p->field_0x68) * 2);
            for (i = 0; i < remain; i++) {
                dstL[i] = lbl_eu_80565E50[src[i * 2]];
                dstR[i] = lbl_eu_80565E50[src[i * 2 + 1]];
            }
        } else {
            // Mono ulaw decode
            for (j = 0; j < remain; j++) {
                dstL[j] = lbl_eu_80565E50[src[j]];
            }
        }

        p->field_0x90 = remain;
        p->field_0x04 = 2;
        p->field_0x94 = p->field_0x0E * remain;
    }

done:
    if (p->field_0x04 == 2) {
        p->field_0x80(p->field_0x84, p->field_0x94, p->field_0x90);
        p->field_0x04 = 3;
    }
}

void ADXB_ExecOneAu(void* self) {
    s16 fmt = *(s16*)((u8*)self + 0x9C);
    if (fmt == 2) {
        ADXB_ExecOneAuUlaw(self);
    } else if (fmt == 1) {
        ADXB_ExecOneAu8(self);
    } else {
        ADXB_ExecOneAu16(self);
    }
}
