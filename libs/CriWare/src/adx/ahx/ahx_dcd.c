// Auto-scaffolded catalog TU for CriWare/src/adx/ahx/ahx_dcd
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern s32 AHXBSR_GetBitStm(void* self, s32 bits);


void AHXSJD_SetupAtbl(u32 val) { AHXDCD_SetupAtbl(val); }

extern u32 lbl_eu_805E64D0;
extern char lbl_eu_80517638[];
extern u32 lbl_eu_805E64D4;
void AHXSJD_SetupMtbl(u32 val) { lbl_eu_805E64D4 = val; }

void AHXSJD_SetupFtbl(u32 val) { AHXDCD_SetupFtbl(val); }

void AHXSJD_SetupWtbl(void) { AHXDCD_SetupWtbl(); }

extern s32 lbl_eu_805E64D8;
extern void AHXSBF_Finish(void);

void AHXDCD_Init(void) {
    if (lbl_eu_805E64D8 == 0) {
        AHXSBF_Init();
    }
    lbl_eu_805E64D8++;
}

void AHXDCD_Finish(void) {
    if (--lbl_eu_805E64D8 != 0) return;
    AHXSBF_Finish();
}

extern void* AHXSBF_Create(void* self, u32 size);

void* AHXDCD_Create(u8* buf, u32 size) {
    u8* base = (u8*)(((u32)buf + 7) & ~7);
    u8* sbf;
    memset(base, 0, 3020);
    sbf = (u8*)(((u32)base + 3027) & ~7);
    if ((u32)(sbf + 4124) < size)
        return 0;
    sbf = (u8*)AHXSBF_Create((void*)sbf, 4124);
    *(s32*)((u8*)buf + 852) = (s32)sbf;
    if (sbf == 0)
        return 0;
    *(u32*)buf = (u32)(((u32)buf + 35) & ~31);
    memset((u8*)buf + 904, 0, 8);
    *(s32*)((u8*)buf + 836) = lbl_eu_805E64D4;
    *(u8*)((u8*)buf + 843) = 16;
    *(u8*)((u8*)buf + 840) = 1;
    return buf;
}

void AHXDCD_Destroy(void* p)
{
    void** sbf = (void**)((char*)p + 0x354);
    if (*sbf) {
        void* tmp = *sbf;
        *sbf = 0;
        AHXSBF_Destroy(tmp);
    }
    memset(p, 0, 0xbcc);
}

void AHXDCD_Reset(void* self) {
    u8* p = (u8*)self;
    void* sbf = *(void**)(p + 0x354);

    memset(p + 0x358, 0, 0x30);
    memset(p + 0x388, 0, 0x3c);
    memset(p + 0x3c4, 0, 0x100);
    memset(p + 0x4c4, 0, 0x100);
    memset(p + 0x5c4, 0, 0x300);
    memset(*(u8**)p, 0, 0x300);
    memset(p + 0x8c4, 0, 0x300);

    *(u8*)(p + 0x349) = 0;

    /* re-read the field: MWCC keeps this copy in r3 for the test + Destroy */
    {
        void* cur = *(void**)(p + 0x354);

        *(u8*)(p + 0x349) = 0;
        *(u32*)(p + 0x34c) = 0;

        if (cur != NULL) {
            *(void**)(p + 0x354) = NULL;
            AHXSBF_Destroy(cur);
            *(void**)(p + 0x354) = AHXSBF_Create(sbf, 0x101c);
        }
    }
}

void AHXDCD_SetBsr(void* self, void* bsr) {
    AHXDCD_Reset(self);
    *(void**)((u8*)self + 0x350) = bsr;
}

extern s32 AHXBSR_GetBitStm(void* bs, s32 nbits);
extern s32 ADX_DecodeHeader(void* tbl, s32 size, void* out, void* out2);
extern s16 lbl_eu_80516B30[];

s32 SKG_GenerateKey(u8* key, s32 count, s16* out1, s16* out2, s16* out3) {
    s16 x1, x2, x3;
    s32 i;

    if (lbl_eu_805E64D0 == 0) {
        lbl_eu_805E64D0 = lbl_eu_805E64D0 + 1;
    }

    *out1 = 0;
    *out2 = 0;
    *out3 = 0;

    if (key == NULL && count <= 0) {
        return 0;
    }

    x1 = lbl_eu_80516B30[0x100];
    for (i = 0; i < count; i++) {
        s32 m = x1 * lbl_eu_80516B30[0x80 + (s8)key[i]];
        x1 = lbl_eu_80516B30[m % 1024];
    }

    x2 = lbl_eu_80516B30[0x200];
    for (i = 0; i < count; i++) {
        s32 m = x2 * lbl_eu_80516B30[0x80 + (s8)key[i]];
        x2 = lbl_eu_80516B30[m % 1024];
    }

    x3 = lbl_eu_80516B30[0x300];
    for (i = 0; i < count; i++) {
        s32 m = x3 * lbl_eu_80516B30[0x80 + (s8)key[i]];
        x3 = lbl_eu_80516B30[m % 1024];
    }

    *out1 = x1;
    *out2 = x2;
    *out3 = x3;

    return 0;
}

extern u8 lbl_eu_805E64E0[];

s32 AHXDCD_DecodeHeader(void* self) {
    u8 buf[0x40];
    s32 n;
    s32 ret;
    s8 v;
    void* bsr;
    u8* dst;
    u8* end;

    bsr = *(void**)((u8*)self + 0x350);
    // shared exit: on early-out paths r3 is left untouched, like retail
    if (bsr != NULL) {
        memset(buf, 0, 0x40);

        dst = lbl_eu_805E64E0;
        n = 0;
        end = dst + 4;
        while (dst != end)
            *dst++ = (u8)AHXBSR_GetBitStm(bsr, 8);
        ADX_DecodeHeader(lbl_eu_805E64E0, 4, &n, NULL);

        end += n;
        while (dst != end)
            *dst++ = (u8)AHXBSR_GetBitStm(bsr, 8);

        ret = ADX_DecodeHeader(lbl_eu_805E64E0, 0x200, &n, buf);
        if (ret >= 0) {
            *(s32*)((u8*)self + 0x38c) = (s8)buf[0x03];
            *(s32*)((u8*)self + 0x390) = *(s32*)(buf + 0x04);
            *(s32*)((u8*)self + 0x394) = *(s32*)(buf + 0x08);
            {
                s32 t0 = *(s32*)(buf + 0x24);
                s32 t1 = *(s32*)(buf + 0x28);
                *(s32*)((u8*)self + 0x3a8) = t1;
                *(s32*)((u8*)self + 0x3a4) = t0;
            }
            *(s32*)((u8*)self + 0x3ac) = *(s32*)(buf + 0x2c);
            {
                s32 t2 = *(s32*)(buf + 0x30);
                s32 t3 = *(s32*)(buf + 0x34);
                *(s32*)((u8*)self + 0x3b4) = t3;
                *(s32*)((u8*)self + 0x3b0) = t2;
            }
            *(s32*)((u8*)self + 0x3b8) = *(s32*)(buf + 0x38);
            *(s32*)((u8*)self + 0x388) = 1;

            v = (s8)buf[0x3d];
        if (v >= 0x10) {
            // extended header: zero the key and regenerate it once
            *(u16*)((u8*)self + 0x3bc) = 0;
            if (lbl_eu_805E64D0 == 0) {
                lbl_eu_805E64D0 = lbl_eu_805E64D0 + 1;
            }
            SKG_GenerateKey((u8*)lbl_eu_80517638, 6, (s16*)((u8*)self + 0x3be),
                            (s16*)((u8*)self + 0x3c0), (s16*)((u8*)self + 0x3c2));
        } else if (v >= 8) {
            memcpy((u8*)self + 0x3bc, (u8*)self + 0xbc4, 8);
        } else {
            memset((u8*)self + 0x3bc, 0, 8);
        }
        }
    }
}

extern s32 AHXBSR_SearchSync(void* a);
extern s32 AHXBSR_IsDataAvailable(void* a, s32 b);
extern s32 AHXDCD_DecodeBhdr(void* self, s32* out);
extern void AHXDCD_BhdrToDinf(void* self, s32* out);
extern s32 AHXDCD_DecodeBitalloc2(void* self, void* inf, s32* out);

typedef struct AHXDCDState {
    u8 _00[0x349];
    s8 sampleReady;
    u8 _34A;
    s8 outputReady;
    u32 eofState;
    u8 _350[0x44];
    u32 totalNumSmpl;
} AHXDCDState;

u32 AHXDCD_IsEof(void* self) {
    return (((AHXDCDState*)self)->eofState == 0xc) ? 1 : 0;
}

// coefficient entry from the AHX quantization table (0x18-byte stride)
typedef struct AHXDcdCoef {
    s32 nbits;  /* +0x00 */
    s32 levels; /* +0x04 */
    s32 mode;   /* +0x08 */
    u8 _0C[4];
    f32 mul;    /* +0x10 */
    f32 add;    /* +0x14 */
} AHXDcdCoef;

// AHX decoder frame info block shared by the scale/sample decoders
typedef struct AHXDcdInf {
    s32 field_0x00;
    s32 chCount;      /* +0x04 */
    u8 _08[0x08];
    s32 maxLev;       /* +0x10 */
    s32 field_0x14;
    struct AHXDcdCoef* coefs; /* +0x18 */
    u8 _1C[0x18];
    s16 key[4];       /* +0x34 */
} AHXDcdInf;

extern void AHXDCD_GetSample_Dequantize_Denormalize(void* bsf, AHXDcdInf* inf, s32 (*bitalloc)[32],
                                                    s32 (*scale2)[32], s32 chOfs, f32* pcm,
                                                    f32* coefTbl);
extern void AHXSBF_Synthesize(s32* base, s32 pcm, s32 idx, s32* out);

s32 AHXDCD_DecodeData(void* self, u8* out) {
    s32 i;
    if (*(s32*)((u8*)self + 844) == 12)
        return 0;
    {
        s32 nblk = *(s32*)((u8*)self + 844) >> 2;
        AHXDCD_GetSample_Dequantize_Denormalize(
            (void*)*(u32*)((u8*)self + 848), (AHXDcdInf*)((u8*)self + 904), (s32 (*)[32])((u8*)self + 964),
            (s32 (*)[32])((u8*)self + 1476), nblk, (f32*)*(u32*)((u8*)self + 0),
            (f32*)*(u32*)((u8*)self + 836));
    }
    for (i = 0; i < 3; i++) {
        AHXSBF_Synthesize(*(s32**)((u8*)self + 852), *(s32*)((u8*)self + 0) + i * 128, 0, (s32*)(out + i * 64));
    }
    *(s32*)((u8*)self + 844) += 1;
    return 96;
}

s16 AHXDCD_GetOutBps(void* self) {
    return ((AHXDCDState*)self)->outputReady;
}

int AHXDCD_GetOutSmpl(void) { return 0x60; }

u32 AHXDCD_GetTotalNumSmpl(void* self) {
    AHXDCDState* state = (AHXDCDState*)self;
    if (state->sampleReady == 0) return 0;
    return state->totalNumSmpl;
}

void AHXDCD_SetExtPrm(void* self, void* prm) {
    memcpy((u8*)self + 0xbc4, prm, 8);
}


extern u32 AHXCMN_SetAlcInfTbl(s32 a, void* b);
extern u8 lbl_eu_80565AD8[];
extern u8 lbl_eu_80565AB8[];

void AHXDCD_BhdrToDinf(void* self, s32* out) {
    if (out[0] == 0) {
        if (*(s32*)((u8*)self + 28) == 3) {
            out[1] = 1;
        } else {
            out[1] = 2;
        }
    }
    if (*(s32*)((u8*)self + 4) == 2) {
        out[4] = AHXCMN_SetAlcInfTbl(0, (u8*)out + 24);
    } else {
        out[4] = 32;
    }
    if (*(s32*)((u8*)self + 28) == 1) {
        out[5] = *(u32*)(lbl_eu_80565AD8 + (*(s32*)((u8*)self + 4) << 4) + (*(s32*)((u8*)self + 32) << 2));
    } else {
        out[5] = out[4];
    }
    if (out[0] == 0) {
        out[2] = *(u32*)(lbl_eu_80565AB8 + (*(s32*)((u8*)self + 0) << 4) + (*(s32*)((u8*)self + 16) << 2));
    }
}


s32 AHXDCD_DecodeBhdr(void* self, s32* out) {
    out[0] = AHXBSR_GetBitStm(self, 1);
    out[1] = 4 - AHXBSR_GetBitStm(self, 2);
    out[2] = !AHXBSR_GetBitStm(self, 1);
    out[3] = AHXBSR_GetBitStm(self, 4);
    out[4] = AHXBSR_GetBitStm(self, 2);
    out[5] = AHXBSR_GetBitStm(self, 1);
    out[6] = AHXBSR_GetBitStm(self, 1);
    out[7] = AHXBSR_GetBitStm(self, 2);
    out[8] = AHXBSR_GetBitStm(self, 2);
    out[9] = AHXBSR_GetBitStm(self, 1);
    out[10] = AHXBSR_GetBitStm(self, 1);
    out[11] = AHXBSR_GetBitStm(self, 2);
    if (out[7] == 3 && out[8] == 0)
        return 0;
    return -1;
}
s32 AHXDCD_DecodeBitalloc2(void* self, void* inf, s32* out) {
    s32 i = 0;
    s32 j;
    s32* tbl = *(s32**)((u8*)inf + 0x18);
    s32 levs = *(s32*)((u8*)inf + 0x10);
    s32 nch = *(s32*)((u8*)inf + 4);
    s32* t;
    s32* p;
    s32 start = *(s32*)((u8*)inf + 0x14);

    // levels below block start: nch independent values per level
    t = tbl;
    for (; i < start; i++) {
        p = out + i;
        for (j = 0; j < nch; j++) {
            *p = AHXBSR_GetBitStm(self, *(t + 1));
            p += 32;
        }
        t += 96;
    }

    // coded levels above block start: one value covers both column halves
    p = out + start;
    t = tbl + start * 96;
    for (; start < levs; start++) {
        s32 v = AHXBSR_GetBitStm(self, *(t + 1));
        p[32] = v;
        p[0] = v;
        p++;
        t += 96;
    }

    // levels beyond the coded range: zero-fill
    for (; levs < 32; levs++) {
        j = 0;
        p = out + levs;
        if (nch > 0) {
            for (; j < nch; j++) {
                *p = 0;
                p += 32;
            }
        }
    }
    return 0;
}
extern void AHXDCD_GetSample_Dequantize_Denormalize(void* bsf, AHXDcdInf* inf, s32 (*bitalloc)[32],
                                                    s32 (*scale2)[32], s32 chOfs, f32* pcm,
                                                    f32* coefTbl);

void AHXDCD_DecodeScale2(void* bsr, AHXDcdInf* inf, s32 (*bitalloc)[32], s32 (*scale2)[32],
                         s32 (*scale3)[96]) {
    s32 maxLev = inf->maxLev;
    s32 chCount = inf->chCount;
    s32 i, c;

    /* level 0 carries a raw 2-bit code selecting the predictor seed */
    if (bitalloc[0][0] != 0) {
        scale2[0][0] = AHXBSR_GetBitStm(bsr, 2);
    } else {
        scale2[0][0] = 0;
    }

    /* levels above 0: XOR-differential decode driven by the 4-entry key */
    {
        s16 scale = inf->key[scale2[0][0]];
        for (i = 1; i < maxLev; i++) {
            if (bitalloc[0][i] != 0) {
                s32 t = AHXBSR_GetBitStm(bsr, 2);
                scale2[0][i] = t ^ (scale & 3);
            }
            scale >>= 2;
        }
    }

    /* levels beyond the coded range carry no scale information */
    for (i = maxLev; i < 32; i++) {
        for (c = 0; c < chCount; c++) {
            scale2[c][i] = 0;
        }
    }

    /* expand per-level scale factors for every channel */
    for (i = 0; i < maxLev; i++) {
        for (c = 0; c < chCount; c++) {
            if (bitalloc[c][i] != 0) {
                switch ((u32)scale2[c][i]) {
                case 0:
                    scale3[c][i] = AHXBSR_GetBitStm(bsr, 6);
                    scale3[c][i + 32] = AHXBSR_GetBitStm(bsr, 6);
                    scale3[c][i + 64] = AHXBSR_GetBitStm(bsr, 6);
                    break;
                case 1: {
                    s16 t = AHXBSR_GetBitStm(bsr, 6);
                    scale3[c][i + 32] = t;
                    scale3[c][i] = t;
                    scale3[c][i + 64] = AHXBSR_GetBitStm(bsr, 6);
                    break;
                }
                case 3: {
                    s16 a = AHXBSR_GetBitStm(bsr, 6);
                    s16 b = AHXBSR_GetBitStm(bsr, 6);
                    scale3[c][i] = a;
                    scale3[c][i + 64] = b;
                    scale3[c][i + 32] = b;
                    break;
                }
                case 2: {
                    s16 t = AHXBSR_GetBitStm(bsr, 6);
                    scale3[c][i + 64] = t;
                    scale3[c][i + 32] = t;
                    scale3[c][i] = t;
                    break;
                }
                }
            } else {
                scale3[c][i] = scale3[c][i + 32] = scale3[c][i + 64] = 63;
            }
        }
    }

    for (i = maxLev; i < 32; i++) {
        for (c = 0; c < chCount; c++) {
            scale3[c][i] = 0;
        }
    }
}

extern u32 lbl_eu_80565B08[];   /* (1 << levels) - 1 mask table */
extern double lbl_eu_80517630;  /* 2^52 constant-pool entry */
extern f32 lbl_eu_80517628;     /* 0.0f constant-pool entry */

void AHXDCD_GetSample_Dequantize_Denormalize(void* bsf, AHXDcdInf* inf, s32 (*bitalloc)[32],
                                             s32 (*scale2)[32], s32 chOfs, f32* pcm,
                                             f32* coefTbl) {
    const double twoPow52 = lbl_eu_80517630;
    const f32 zero = lbl_eu_80517628;
    s32 chCount = inf->chCount;
    s32 maxLev = inf->maxLev;
    s32 thr = inf->field_0x14;
    AHXDcdCoef* coefs = inf->coefs;
    s32 blk, ch;

    for (blk = 0; blk < maxLev; blk++) {
        s32 n = (blk < thr) ? chCount : 1;
        for (ch = 0; ch < n; ch++) {
            s32 ba = bitalloc[ch][blk];
            f32* out = &pcm[ch * 96 + blk];
            if (ba != 0) {
                AHXDcdCoef* cf = &coefs[ba];
                f32 tv = coefTbl[scale2[ch][blk]];
                f32 fb = cf->add * tv;
                f32 fa = cf->mul * tv;

                if (cf->mode == 3) {
                    s32 levels = cf->levels;
                    if (levels <= 8) {
                        /* three levels packed into levels*3 bits */
                        s32 v = AHXBSR_GetBitStm(bsf, levels * 3);
                        u32 mask = lbl_eu_80565B08[levels];
                        s32 dA = v & mask;
                        s32 dB = (v >> levels) & mask;
                        s32 dC = (v >> levels >> levels) & mask;
                        out[64] = (f32)((double)dA - twoPow52) * fa + fb;
                        out[32] = (f32)((double)dB - twoPow52) * fa + fb;
                        out[0] = (f32)((double)dC - twoPow52) * fa + fb;
                    } else {
                        /* wide levels: three separate reads */
                        out[0] = (f32)((double)AHXBSR_GetBitStm(bsf, levels) - twoPow52) * fa + fb;
                        out[32] = (f32)((double)AHXBSR_GetBitStm(bsf, levels) - twoPow52) * fa + fb;
                        out[64] = (f32)((double)AHXBSR_GetBitStm(bsf, levels) - twoPow52) * fa + fb;
                    }
                } else {
                    /* base-N digit decomposition of one packed read */
                    s32 v = AHXBSR_GetBitStm(bsf, cf->nbits);
                    s32 levels = cf->levels;
                    s32 d0, d1, d2;
                    switch ((u32)levels) {
                    case 3u:
                        d0 = v % 3;
                        d1 = (v / 3) % 3;
                        d2 = (v / 3 / 3) % 3;
                        break;
                    case 5u:
                        d0 = v % 5;
                        d1 = (v / 5) % 5;
                        d2 = (v / 5 / 5) % 5;
                        break;
                    case 9u:
                        d0 = v % 9;
                        d1 = (v / 9) % 9;
                        d2 = (v / 9 / 9) % 9;
                        break;
                    default:
                        d0 = v % levels;
                        d1 = (v / levels) % levels;
                        d2 = (v / levels / levels) % levels;
                        break;
                    }
                    out[0] = (f32)((double)d0 - twoPow52) * fa + fb;
                    out[32] = (f32)((double)d1 - twoPow52) * fa + fb;
                    out[64] = (f32)((double)d2 - twoPow52) * fa + fb;
                }
            } else {
                out[0] = zero;
                out[32] = zero;
                out[64] = zero;
            }

            /* stereo: past the threshold block mirror front output to rear */
            if (chCount == 2 && blk >= thr) {
                f32* o = &pcm[blk];
                o[96] = o[0];
                o[128] = o[32];
                o[160] = o[64];
            }
        }
    }

    /* blocks beyond the coded frame stay silent on every channel */
    for (; blk < 32; blk++) {
        for (ch = 0; ch < chCount; ch++) {
            f32* o = &pcm[ch * 96 + blk];
            o[0] = zero;
            o[32] = zero;
            o[64] = zero;
        }
    }
}

s32 AHXDCD_DecodeFrmHdr(void* self) {
    u8* p = (u8*)self;
    void* bsr = *(void**)(p + 0x350);

    if (bsr == NULL)
        return -1;

    s32 sync = AHXBSR_SearchSync(bsr);
    if (sync == -1)
        return -1;

    // sync found but not the expected short-sync form
    if (sync != 1)
        return 1;

    if (AHXBSR_IsDataAvailable(*(void**)(p + 0x350), 0x14) == 0)
        return -1;

    if (AHXDCD_DecodeBhdr(*(void**)(p + 0x350), (s32*)(p + 0x358)) < 0)
        return -1;

    AHXDCD_BhdrToDinf(p + 0x358, (s32*)(p + 0x388));
    AHXDCD_DecodeBitalloc2(*(void**)(p + 0x350), p + 0x388, (s32*)(p + 0x3c4));
    AHXDCD_DecodeScale2(*(void**)(p + 0x350), (AHXDcdInf*)(p + 0x388), (s32 (*)[32])(p + 0x3c4),
                        (s32 (*)[32])(p + 0x4c4), (s32 (*)[96])(p + 0x5c4));

    *(u8*)(p + 0x349) = 1;
    *(u32*)(p + 0x34c) = 0;
    return 0;
}
