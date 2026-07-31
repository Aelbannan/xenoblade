// ADX Audio Unit functions
// libs/CriWare/src/adx/adxt/adx_bau.c

#include <harness_catalog.h>
#include <string.h>

extern const u16 lbl_eu_80565E50[];
extern const u8 lbl_eu_80519110[];

// Byte-swap helper
static u32 read_be32(const u8* p) {
    return ((u32)p[0] << 24) | ((u32)p[1] << 16) | ((u32)p[2] << 8) | (u32)p[3];
}

static u16 read_be16(const u8* p) {
    return ((u16)p[0] << 8) | (u16)p[1];
}

u8* AU_GetInfo(u8* data, s32 maxBytes, s32* outChannelCount, s32* outSampleRate, s32* outTotalSamples, s32* outBlockSize, u32* outFormat) {
    u32 sig, hdrSize;
    u32 version;
    u32 channelCount, sampleRate, totalSamples, blockSize;

    // Check signature "ds." or "dns."
    sig = read_be32(data);
    if (sig != 0x64732E00 && sig != 0x646E732E) {
        return NULL;
    }

    // Read header size
    hdrSize = read_be32(data + 4);
    if (hdrSize > (u32)maxBytes) {
        return NULL;
    }

    // Read format info
    version = read_be32(data + 8);
    channelCount = read_be32(data + 12);
    sampleRate = read_be32(data + 16);
    totalSamples = read_be32(data + 20);
    blockSize = read_be32(data + 24);

    if (version == 1) {
        *outFormat = 2;
        *outBlockSize = 8;
    } else if (version == 2) {
        *outFormat = 1;
        *outBlockSize = 8;
    } else if (version == 3) {
        *outFormat = 0;
        *outBlockSize = 16;
    } else {
        return NULL;
    }

    *outSampleRate = sampleRate;
    *outChannelCount = channelCount;

    // Calculate total samples based on format
    if (*outFormat == 2) {
        *outTotalSamples = totalSamples / blockSize;
    } else if (*outFormat == 1) {
        *outTotalSamples = totalSamples / blockSize;
    } else if (*outFormat == 0) {
        *outTotalSamples = (totalSamples / 2) / blockSize;
    } else {
        *outTotalSamples = 0x7FFFFFFF;
    }

    return data + hdrSize;
}

s32 ADXB_CheckAu(void* data) {
    if (memcmp(data, lbl_eu_80519110, 4) == 0) {
        return 1;
    }
    if (memcmp(data, lbl_eu_80519110 + 5, 4) == 0) {
        return 1;
    }
    return 0;
}

s32 ADX_DecodeInfoAu(u8* data, s32 maxBytes, s16* outHeaderSize, u8* outChannelCount, u8* outBlockSize, u8* outEncoding, s32* outSampleRate, s32* outTotalSamples, u32* outFormat) {
    s32 channelCount, sampleRate, totalSamples, blockSize;
    u32 format;
    u8 encoding;

    if (maxBytes < 8) {
        *outHeaderSize = 0;
        return -1;
    }

    u8* end = AU_GetInfo(data, maxBytes, &channelCount, &sampleRate, &totalSamples, &blockSize, &format);
    if (end == NULL) {
        return -1;
    }

    s16 consumed = (s16)(end - data);
    if (consumed <= 0) {
        return -1;
    }

    *outHeaderSize = consumed;
    *outBlockSize = blockSize;
    *outChannelCount = channelCount;
    *outSampleRate = sampleRate;
    *outEncoding = -1;

    // Calculate encoding bits per sample
    s32 encBits = (s32)(s8)*outChannelCount * (s32)(s8)*outBlockSize;
    *outEncoding = (u8)(encBits >> 3);

    *outTotalSamples = totalSamples;
    *outFormat = 1;

    return 0;
}

s32 ADXB_DecodeHeaderAu(void* self, u8* data, s32 maxBytes) {
    void* sjd = self;
    s16 headerSize;
    u8 channelCount, blockSize, encoding;
    s32 sampleRate, totalSamples;
    u32 format;
    s32 result;

    // Set version
    *(s16*)((u8*)sjd + 2) = 1;

    result = ADX_DecodeInfoAu(data, maxBytes, &headerSize,
        (u8*)((u8*)sjd + 0x0D),
        (u8*)((u8*)sjd + 0x0C),
        (u8*)((u8*)sjd + 0x0F),
        (s32*)((u8*)sjd + 0x10),
        (s32*)((u8*)sjd + 0x14),
        (u32*)((u8*)sjd + 0x18));

    if (result < 0) {
        return 0;
    }

    // Initialize state fields
    channelCount = *(u8*)((u8*)sjd + 0x0E);
    blockSize = *(u8*)((u8*)sjd + 0x0F);
    sampleRate = *(s32*)((u8*)sjd + 0x10);

    *(s16*)((u8*)sjd + 0x1C) = 0;
    *(s16*)((u8*)sjd + 0x26) = 0;
    *(s16*)((u8*)sjd + 0x24) = 0;
    *(s32*)((u8*)sjd + 0x34) = 0;
    *(s32*)((u8*)sjd + 0x30) = 0;
    *(s32*)((u8*)sjd + 0x2C) = 0;
    *(s32*)((u8*)sjd + 0x28) = 0;
    *(s32*)((u8*)sjd + 0x20) = 0;

    *(s32*)((u8*)sjd + 0x50) = (s32)(s8)channelCount;
    *(s32*)((u8*)sjd + 0x54) = (s32)(s8)blockSize;
    *(s32*)((u8*)sjd + 0x58) = sampleRate;
    *(s32*)((u8*)sjd + 0x5C) = *(s32*)((u8*)sjd + 0x3C);
    *(s32*)((u8*)sjd + 0x60) = *(s32*)((u8*)sjd + 0x40);
    *(s32*)((u8*)sjd + 0x64) = *(s32*)((u8*)sjd + 0x44);
    *(s32*)((u8*)sjd + 0x8C) = 0;
    *(s32*)((u8*)sjd + 0x88) = 0;
    *(s16*)((u8*)sjd + 0x98) = 4;
    *(s16*)((u8*)sjd + 0x9C) = *(s16*)((u8*)sjd + 0x1C);

    return headerSize;
}

void ADXB_ExecOneAu16(void* self) {
    u8* au = (u8*)self;
    u16* pcmBuf;
    s32 state, avail, count, i;
    s32 numChan;

    pcmBuf = (u16*)*(u32*)(au + 0x48);
    state = *(s32*)(au + 0x04);

    if (state == 1) {
        if (ADXPD_GetStat(*(u32*)(au + 0x08)) != 0) {
            goto done;
        }

        // Call get data callback
        ((void (*)(void*, u32*, u32*, u32*))(*(u32**)(au + 0x78))[0])(
            *(void**)(au + 0x7C),
            (u32*)(au + 0x68),
            (u32*)(au + 0x6C),
            (u32*)(au + 0x70));

        u32 dataOfst = *(u32*)(au + 0x68);
        u32 dataEnd = *(u32*)(au + 0x60);
        u32 dataLen = *(u32*)(au + 0x6C);

        s32 remain = dataEnd - dataOfst;
        if (remain > (s32)dataLen) {
            remain = dataLen;
        }

        u32 maxSmpl = *(u32*)(au + 0x4C);
        if (remain > (s32)maxSmpl) {
            remain = maxSmpl;
        }

        s8 encoding = *(s8*)(au + 0x0E);
        u16* dstL = (u16*)(*(u32*)(au + 0x5C) + dataOfst * 2);
        u16* dstR = dstL + *(u32*)(au + 0x64);
        u16* src = pcmBuf;

        if (encoding == 2) {
            // Stereo deinterleave with byte swap
            s32 count8 = remain - 8;
            if (remain > 0 && count8 > 0) {
                for (i = 0; i < count8; i += 8) {
                    u16 s0 = src[0]; u16 s1 = src[1];
                    dstL[0] = (s0 >> 8) | (s0 << 8);
                    dstR[0] = (s1 >> 8) | (s1 << 8);
                    s0 = src[2]; s1 = src[3];
                    dstL[1] = (s0 >> 8) | (s0 << 8);
                    dstR[1] = (s1 >> 8) | (s1 << 8);
                    s0 = src[4]; s1 = src[5];
                    dstL[2] = (s0 >> 8) | (s0 << 8);
                    dstR[2] = (s1 >> 8) | (s1 << 8);
                    s0 = src[6]; s1 = src[7];
                    dstL[3] = (s0 >> 8) | (s0 << 8);
                    dstR[3] = (s1 >> 8) | (s1 << 8);
                    src += 8;
                    dstL += 4;
                    dstR += 4;
                }
            }
            // Remaining samples
            for (; i < remain; i++) {
                u16 s = *src++;
                *dstL++ = (s >> 8) | (s << 8);
                s = *src++;
                *dstR++ = (s >> 8) | (s << 8);
            }
        } else {
            // Mono byte swap
            for (i = 0; i < remain; i++) {
                u16 s = *src++;
                *dstL++ = (s >> 8) | (s << 8);
            }
        }

        *(u32*)(au + 0x90) = remain;
        *(u32*)(au + 0x04) = 2;
        *(u32*)(au + 0x94) = (s32)(s8)encoding * (remain * 2);
    }

done:
    if (*(s32*)(au + 0x04) == 2) {
        ((void (*)(void*, u32, u32))(*(u32**)(au + 0x80))[0])(
            *(void**)(au + 0x84),
            *(u32*)(au + 0x94),
            *(u32*)(au + 0x90));
        *(s32*)(au + 0x04) = 3;
    }
}

void ADXB_ExecOneAu8(void* self) {
    u8* au = (u8*)self;
    u8* pcmBuf;
    s32 state, avail, count, i;
    s32 numChan;

    pcmBuf = (u8*)*(u32*)(au + 0x48);
    state = *(s32*)(au + 0x04);

    if (state == 1) {
        if (ADXPD_GetStat(*(u32*)(au + 0x08)) != 0) {
            goto done;
        }

        // Call get data callback
        ((void (*)(void*, u32*, u32*, u32*))(*(u32**)(au + 0x78))[0])(
            *(void**)(au + 0x7C),
            (u32*)(au + 0x68),
            (u32*)(au + 0x6C),
            (u32*)(au + 0x70));

        u32 dataOfst = *(u32*)(au + 0x68);
        u32 dataEnd = *(u32*)(au + 0x60);
        u32 dataLen = *(u32*)(au + 0x6C);

        s32 remain = dataEnd - dataOfst;
        if (remain > (s32)dataLen) {
            remain = dataLen;
        }

        u32 maxSmpl = *(u32*)(au + 0x4C);
        if (remain > (s32)maxSmpl) {
            remain = maxSmpl;
        }

        s8 encoding = *(s8*)(au + 0x0E);
        u16* dstL = (u16*)(*(u32*)(au + 0x5C) + dataOfst * 2);
        u16* dstR = dstL + *(u32*)(au + 0x64);
        u8* src = pcmBuf;

        if (encoding == 2) {
            // Stereo deinterleave with 8->16 expansion
            for (i = 0; i < remain; i++) {
                u8 s = *src++;
                *dstL++ = (u16)s << 8;
                s = *src++;
                *dstR++ = (u16)s << 8;
            }
        } else {
            // Mono 8->16 expansion
            for (i = 0; i < remain; i++) {
                u8 s = *src++;
                *dstL++ = (u16)s << 8;
            }
        }

        *(u32*)(au + 0x90) = remain;
        *(u32*)(au + 0x04) = 2;
        *(u32*)(au + 0x94) = (s32)(s8)encoding * remain;
    }

done:
    if (*(s32*)(au + 0x04) == 2) {
        ((void (*)(void*, u32, u32))(*(u32**)(au + 0x80))[0])(
            *(void**)(au + 0x84),
            *(u32*)(au + 0x94),
            *(u32*)(au + 0x90));
        *(s32*)(au + 0x04) = 3;
    }
}

void ADXB_ExecOneAuUlaw(void* self) {
    u8* au = (u8*)self;
    u8* pcmBuf;
    s32 state, avail, count, i;
    s32 numChan;

    pcmBuf = (u8*)*(u32*)(au + 0x48);
    state = *(s32*)(au + 0x04);

    if (state == 1) {
        if (ADXPD_GetStat(*(u32*)(au + 0x08)) != 0) {
            goto done;
        }

        // Call get data callback
        ((void (*)(void*, u32*, u32*, u32*))(*(u32**)(au + 0x78))[0])(
            *(void**)(au + 0x7C),
            (u32*)(au + 0x68),
            (u32*)(au + 0x6C),
            (u32*)(au + 0x70));

        u32 dataOfst = *(u32*)(au + 0x68);
        u32 dataEnd = *(u32*)(au + 0x60);
        u32 dataLen = *(u32*)(au + 0x6C);

        s32 remain = dataEnd - dataOfst;
        if (remain > (s32)dataLen) {
            remain = dataLen;
        }

        u32 maxSmpl = *(u32*)(au + 0x4C);
        if (remain > (s32)maxSmpl) {
            remain = maxSmpl;
        }

        s8 encoding = *(s8*)(au + 0x0E);
        u16* dstL = (u16*)(*(u32*)(au + 0x5C) + dataOfst * 2);
        u16* dstR = dstL + *(u32*)(au + 0x64);
        u8* src = pcmBuf;

        if (encoding == 2) {
            // Stereo deinterleave with ulaw decode
            for (i = 0; i < remain; i++) {
                u8 s = *src++;
                *dstL++ = lbl_eu_80565E50[s];
                s = *src++;
                *dstR++ = lbl_eu_80565E50[s];
            }
        } else {
            // Mono ulaw decode
            for (i = 0; i < remain; i++) {
                u8 s = *src++;
                *dstL++ = lbl_eu_80565E50[s];
            }
        }

        *(u32*)(au + 0x90) = remain;
        *(u32*)(au + 0x04) = 2;
        *(u32*)(au + 0x94) = (s32)(s8)encoding * remain;
    }

done:
    if (*(s32*)(au + 0x04) == 2) {
        ((void (*)(void*, u32, u32))(*(u32**)(au + 0x80))[0])(
            *(void**)(au + 0x84),
            *(u32*)(au + 0x94),
            *(u32*)(au + 0x90));
        *(s32*)(au + 0x04) = 3;
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
