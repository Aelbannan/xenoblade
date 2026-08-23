// ADX Audio Unit functions
// libs/CriWare/src/adx/adxt/adx_bau.c

#include <harness_catalog.h>
#include <string.h>

extern const u16 lbl_eu_80565E50[];
extern const u8 lbl_eu_80519110[];

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
