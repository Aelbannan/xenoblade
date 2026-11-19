#include <harness_catalog.h>

__declspec(section ".data") __attribute__((aligned(8))) const unsigned char ahx_dcd_data[0x78] = {0x00,0x00,0x56,0x22,0x00,0x00,0x5D,0xC0,0x00,0x00,0x3E,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0xAC,0x44,0x00,0x00,0xBB,0x80,0x00,0x00,0x7D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x0C,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x0C,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x0F,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x7F,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00};
/* .bss objects, in retail address order (0x805E64D0); defined after the last
 * function so every function addresses each one through its own @ha/@l symbol,
 * like retail (see the note above the definitions at the end of the file). */
extern u32 ahxdcd_key_init;             /* 0x805E64D0 key-table init guard */
extern u32 ahxdcd_mtbl;                 /* 0x805E64D4 selected mtbl */
extern s32 ahxdcd_refcnt;               /* 0x805E64D8 init/finish refcount */
extern u8 ahxdcd_hdrbuf[];              /* 0x805E64E0 0x200-byte decode buffer */

extern s32 AHXBSR_GetBitStm(void* self, s32 bits);


void AHXSJD_SetupAtbl(u32 val) { AHXDCD_SetupAtbl(val); }

extern const char ahxdcd_key_material[];
void AHXSJD_SetupMtbl(u32 val) { ahxdcd_mtbl = val; }

void AHXSJD_SetupFtbl(u32 val) { AHXDCD_SetupFtbl(val); }

void AHXSJD_SetupWtbl(void) { AHXDCD_SetupWtbl(); }

extern void AHXSBF_Finish(void);

void AHXDCD_Init(void) {
    if (ahxdcd_refcnt == 0) {
        AHXSBF_Init();
    }
    ahxdcd_refcnt++;
}

void AHXDCD_Finish(void) {
    if (--ahxdcd_refcnt != 0) return;
    AHXSBF_Finish();
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
    s32 field_0x00;   /* +0x00 */
    s32 chCount;      /* +0x04 */
    s32 field_0x08;   /* +0x08 */
    u32 totalNumSmpl; /* +0x0c */
    s32 maxLev;       /* +0x10 */
    s32 field_0x14;   /* +0x14 */
    struct AHXDcdCoef* coefs; /* +0x18 */
    s32 field_0x1c;   /* +0x1c */
    s32 field_0x20;   /* +0x20 */
    s32 field_0x24;   /* +0x24 */
    s32 field_0x28;   /* +0x28 */
    s32 field_0x2c;   /* +0x2c */
    s32 field_0x30;   /* +0x30 */
    s16 key[4];       /* +0x34 */
} AHXDcdInf;          /* 0x3c */

/* Block header decoded into the handle's +0x358 work area. */
typedef struct AHXDcdBhdr {
    s32 field_0x00;
    s32 field_0x04;
    s32 field_0x08;
    s32 field_0x0c;
    s32 field_0x10;
    s32 field_0x14;
    s32 field_0x18;
    s32 config;        /* +0x1c */
    s32 field_0x20;
    s32 field_0x24;
    s32 field_0x28;
    s32 field_0x2c;
} AHXDcdBhdr;              /* 0x30 */

/* The full AHX decoder handle: every sub-work area is a member at its retail
 * offset, so the decoder never has to reach through a byte offset cast. */
typedef struct AHXDCDState {
    void* pcm;                 /* 0x000 caller PCM base */
    u8 _04[0x340];             /* 0x004..0x343 */
    u32 mtbl;                  /* 0x344 selected mtbl */
    u8 field_0x348;            /* 0x348 */
    s8 sampleReady;            /* 0x349 */
    u8 field_0x34a;            /* 0x34a */
    s8 outputReady;            /* 0x34b */
    u32 eofState;              /* 0x34c */
    void* bsr;                 /* 0x350 bitstream reader */
    void* sbf;                 /* 0x354 synth buffer filter */
    AHXDcdBhdr bhdr;           /* 0x358 decoded block header */
    AHXDcdInf inf;             /* 0x388 frame info */
    s32 bitalloc[2][32];       /* 0x3c4 */
    s32 scale2[2][32];         /* 0x4c4 */
    s32 scale3[2][96];         /* 0x5c4 */
    u8 pcmWork[0x300];         /* 0x8c4 */
    u8 extPrm[8];              /* 0xbc4 */
} AHXDCDState;                 /* 0xbcc */

extern void* AHXSBF_Create(void* self, u32 size);

void* AHXDCD_Create(u8* buf, u32 size) {
    AHXDCDState* st = (AHXDCDState*)buf;
    u8* base = (u8*)(((u32)buf + 7) & ~7);
    u8* sbf;
    memset(base, 0, 3020);
    sbf = (u8*)(((u32)base + 3027) & ~7);
    if ((u32)(sbf + 4124) < size)
        return 0;
    sbf = (u8*)AHXSBF_Create((void*)sbf, 4124);
    st->sbf = sbf;
    if (sbf == 0)
        return 0;
    st->pcm = (void*)(((u32)buf + 35) & ~31);
    memset(&st->inf, 0, 8);
    st->mtbl = ahxdcd_mtbl;
    st->outputReady = 16;
    st->field_0x348 = 1;
    return buf;
}

void AHXDCD_Destroy(void* p)
{
    AHXDCDState* st = (AHXDCDState*)p;
    void** sbf = &st->sbf;
    if (*sbf) {
        void* tmp = *sbf;
        *sbf = 0;
        AHXSBF_Destroy(tmp);
    }
    memset(p, 0, 0xbcc);
}

void AHXDCD_Reset(void* self) {
    AHXDCDState* st = (AHXDCDState*)self;
    void* sbf = st->sbf;

    memset(&st->bhdr, 0, sizeof(st->bhdr));
    memset(&st->inf, 0, sizeof(st->inf));
    memset(st->bitalloc, 0, sizeof(st->bitalloc));
    memset(st->scale2, 0, sizeof(st->scale2));
    memset(st->scale3, 0, sizeof(st->scale3));
    memset(st->pcm, 0, 0x300);
    memset(st->pcmWork, 0, sizeof(st->pcmWork));

    st->sampleReady = 0;

    /* re-read the field: MWCC keeps this copy in r3 for the test + Destroy */
    {
        void* cur = st->sbf;

        st->sampleReady = 0;
        st->eofState = 0;

        if (cur != NULL) {
            st->sbf = NULL;
            AHXSBF_Destroy(cur);
            st->sbf = AHXSBF_Create(sbf, 0x101c);
        }
    }
}

void AHXDCD_SetBsr(void* self, void* bsr) {
    AHXDCD_Reset(self);
    ((AHXDCDState*)self)->bsr = bsr;
}

extern s32 AHXBSR_GetBitStm(void* bs, s32 nbits);
extern s32 ADX_DecodeHeader(void* tbl, s32 size, void* out, void* out2);
extern const s16 ahx_key_table[];

s32 SKG_GenerateKey(u8* key, s32 count, s16* out1, s16* out2, s16* out3) {
    s16 x1, x2, x3;
    s32 i;

    if (ahxdcd_key_init == 0) {
        ahxdcd_key_init = ahxdcd_key_init + 1;
    }

    *out1 = 0;
    *out2 = 0;
    *out3 = 0;

    if (key == NULL && count <= 0) {
        return 0;
    }

    x1 = ahx_key_table[0x100];
    for (i = 0; i < count; i++) {
        s32 m = x1 * ahx_key_table[0x80 + (s8)key[i]];
        x1 = ahx_key_table[m % 1024];
    }

    x2 = ahx_key_table[0x200];
    for (i = 0; i < count; i++) {
        s32 m = x2 * ahx_key_table[0x80 + (s8)key[i]];
        x2 = ahx_key_table[m % 1024];
    }

    x3 = ahx_key_table[0x300];
    for (i = 0; i < count; i++) {
        s32 m = x3 * ahx_key_table[0x80 + (s8)key[i]];
        x3 = ahx_key_table[m % 1024];
    }

    *out1 = x1;
    *out2 = x2;
    *out3 = x3;

    return 0;
}


s32 AHXDCD_DecodeHeader(void* self) {
    u8 buf[0x40];
    s32 n;
    s32 ret;
    s8 v;
    void* bsr;
    u8* dst;
    u8* end;
    AHXDCDState* st = (AHXDCDState*)self;

    bsr = st->bsr;
    // shared exit: on early-out paths r3 is left untouched, like retail
    if (bsr != NULL) {
        memset(buf, 0, 0x40);

        dst = ahxdcd_hdrbuf;
        n = 0;
        end = dst + 4;
        while (dst != end)
            *dst++ = (u8)AHXBSR_GetBitStm(bsr, 8);
        ADX_DecodeHeader(ahxdcd_hdrbuf, 4, &n, NULL);

        end += n;
        while (dst != end)
            *dst++ = (u8)AHXBSR_GetBitStm(bsr, 8);

        ret = ADX_DecodeHeader(ahxdcd_hdrbuf, 0x200, &n, buf);
        if (ret >= 0) {
            st->inf.chCount = (s8)buf[0x03];
            st->inf.field_0x08 = *(s32*)(buf + 0x04);
            st->inf.totalNumSmpl = *(u32*)(buf + 0x08);
            {
                s32 t0 = *(s32*)(buf + 0x24);
                s32 t1 = *(s32*)(buf + 0x28);
                st->inf.field_0x20 = t1;
                st->inf.field_0x1c = t0;
            }
            st->inf.field_0x24 = *(s32*)(buf + 0x2c);
            {
                s32 t2 = *(s32*)(buf + 0x30);
                s32 t3 = *(s32*)(buf + 0x34);
                st->inf.field_0x2c = t3;
                st->inf.field_0x28 = t2;
            }
            st->inf.field_0x30 = *(s32*)(buf + 0x38);
            st->inf.field_0x00 = 1;

            v = (s8)buf[0x3d];
        if (v >= 0x10) {
            // extended header: zero the key and regenerate it once
            st->inf.key[0] = 0;
            if (ahxdcd_key_init == 0) {
                ahxdcd_key_init = ahxdcd_key_init + 1;
            }
            SKG_GenerateKey((u8*)ahxdcd_key_material, 6, &st->inf.key[1],
                            &st->inf.key[2], &st->inf.key[3]);
        } else if (v >= 8) {
            memcpy(&st->inf.key[0], st->extPrm, 8);
        } else {
            memset(&st->inf.key[0], 0, 8);
        }
        }
    }
}

extern s32 AHXBSR_SearchSync(void* a);
extern s32 AHXBSR_IsDataAvailable(void* a, s32 b);
extern s32 AHXDCD_DecodeBhdr(void* self, s32* out);
extern void AHXDCD_BhdrToDinf(void* self, s32* out);
extern s32 AHXDCD_DecodeBitalloc2(void* self, void* inf, s32* out);



u32 AHXDCD_IsEof(void* self) {
    return (((AHXDCDState*)self)->eofState == 0xc) ? 1 : 0;
}


extern void AHXDCD_GetSample_Dequantize_Denormalize(void* bsf, AHXDcdInf* inf, s32 (*bitalloc)[32],
                                                    s32 (*scale2)[32], s32 chOfs, f32* pcm,
                                                    f32* coefTbl);
extern void AHXSBF_Synthesize(s32* base, s32 pcm, s32 idx, s32* out);

s32 AHXDCD_DecodeData(void* self, u8* out) {
    AHXDCDState* st = (AHXDCDState*)self;
    s32 i;
    if ((s32)st->eofState == 12)
        return 0;
    {
        s32 nblk = (s32)st->eofState >> 2;
        AHXDCD_GetSample_Dequantize_Denormalize(
            st->bsr, &st->inf, st->bitalloc, (s32 (*)[32])st->scale3, nblk, (f32*)st->pcm,
            (f32*)st->mtbl);
    }
    for (i = 0; i < 3; i++) {
        AHXSBF_Synthesize((s32*)st->sbf, *(s32*)&st->pcm + i * 128, 0, (s32*)(out + i * 64));
    }
    st->eofState += 1;
    return 96;
}

s16 AHXDCD_GetOutBps(void* self) {
    return ((AHXDCDState*)self)->outputReady;
}

int AHXDCD_GetOutSmpl(void) { return 0x60; }

u32 AHXDCD_GetTotalNumSmpl(void* self) {
    AHXDCDState* state = (AHXDCDState*)self;
    if (state->sampleReady == 0) return 0;
    return state->inf.totalNumSmpl;
}

void AHXDCD_SetExtPrm(void* self, void* prm) {
    memcpy(((AHXDCDState*)self)->extPrm, prm, 8);
}


extern u32 AHXCMN_SetAlcInfTbl(s32 a, void* b);
extern u8 lbl_eu_80565AD8[];
extern u8 lbl_eu_80565AB8[];

void AHXDCD_BhdrToDinf(void* self, s32* out) {
    AHXDcdBhdr* hdr = (AHXDcdBhdr*)self;
    if (out[0] == 0) {
        if (hdr->config == 3) {
            out[1] = 1;
        } else {
            out[1] = 2;
        }
    }
    if (hdr->field_0x04 == 2) {
        out[4] = AHXCMN_SetAlcInfTbl(0, (u8*)out + 24);
    } else {
        out[4] = 32;
    }
    if (hdr->config == 1) {
        out[5] = *(u32*)(lbl_eu_80565AD8 + (hdr->field_0x04 << 4) + (hdr->field_0x20 << 2));
    } else {
        out[5] = out[4];
    }
    if (out[0] == 0) {
        out[2] = *(u32*)(lbl_eu_80565AB8 + (hdr->field_0x00 << 4) + (hdr->field_0x10 << 2));
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
    AHXDcdInf* info = (AHXDcdInf*)inf;
    s32 i = 0;
    s32 j;
    s32* tbl = (s32*)info->coefs;
    s32 levs = info->maxLev;
    s32 nch = info->chCount;
    s32* t;
    s32* p;
    s32 start = info->field_0x14;

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
extern const double ahxdcd_two52;  /* 2^52 + 2^31 conversion bias */
extern const f32 ahxdcd_zero_f32;     /* 0.0f constant-pool entry */

/* s32 -> double without MWCC's pooled conversion magic: build the
 * 0x43300000_xxxxxxxx stack double with immediates (lis/stw) and subtract
 * the retail extern, so MWCC reuses ahxdcd_two52 instead of pooling a
 * duplicate @N double (raw +8). Mirrors retail's lis/stw + lfd + fsub. */
static inline double ahx_s32_to_double(s32 v) {
    union {
        double d;
        u32 w[2];
    } u;
    u.w[0] = 0x43300000u;
    u.w[1] = (u32)v ^ 0x80000000u;
    return u.d - ahxdcd_two52;
}

void AHXDCD_GetSample_Dequantize_Denormalize(void* bsf, AHXDcdInf* inf, s32 (*bitalloc)[32],
                                             s32 (*scale2)[32], s32 chOfs, f32* pcm,
                                             f32* coefTbl) {
    const f32 zero = ahxdcd_zero_f32;
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
                        out[64] = (f32)ahx_s32_to_double(dA) * fa + fb;
                        out[32] = (f32)ahx_s32_to_double(dB) * fa + fb;
                        out[0] = (f32)ahx_s32_to_double(dC) * fa + fb;
                    } else {
                        /* wide levels: three separate reads */
                        out[0] = (f32)ahx_s32_to_double(AHXBSR_GetBitStm(bsf, levels)) * fa + fb;
                        out[32] = (f32)ahx_s32_to_double(AHXBSR_GetBitStm(bsf, levels)) * fa + fb;
                        out[64] = (f32)ahx_s32_to_double(AHXBSR_GetBitStm(bsf, levels)) * fa + fb;
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
                    out[0] = (f32)ahx_s32_to_double(d0) * fa + fb;
                    out[32] = (f32)ahx_s32_to_double(d1) * fa + fb;
                    out[64] = (f32)ahx_s32_to_double(d2) * fa + fb;
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
    AHXDCDState* st = (AHXDCDState*)self;
    void* bsr = st->bsr;

    if (bsr == NULL)
        return -1;

    s32 sync = AHXBSR_SearchSync(bsr);
    if (sync == -1)
        return -1;

    // sync found but not the expected short-sync form
    if (sync != 1)
        return 1;

    if (AHXBSR_IsDataAvailable(st->bsr, 0x14) == 0)
        return -1;

    if (AHXDCD_DecodeBhdr(st->bsr, (s32*)&st->bhdr) < 0)
        return -1;

    AHXDCD_BhdrToDinf(&st->bhdr, (s32*)&st->inf);
    AHXDCD_DecodeBitalloc2(st->bsr, &st->inf, (s32*)st->bitalloc);
    AHXDCD_DecodeScale2(st->bsr, &st->inf, st->bitalloc, st->scale2, st->scale3);

    st->sampleReady = 1;
    st->eofState = 0;
    return 0;
}
/* .bss: defined after the last function so every function addresses each
 * object through its own @ha/@l symbol, as retail does.  MWCC emits .bss
 * objects in reverse definition order, so these are written in reverse retail
 * address order (0x010, 0x008, 0x004, 0x000 from 0x805E64D0). */
__attribute__((aligned(8))) u8 ahxdcd_hdrbuf[0x200];  /* 0x805E64E0 decode buffer */
s32 ahxdcd_refcnt;                     /* 0x805E64D8 init/finish refcount */
u32 ahxdcd_mtbl;                       /* 0x805E64D4 selected mtbl */
u32 ahxdcd_key_init;                   /* 0x805E64D0 key-table init guard */

/* AHX decoder literal pool: 0.0f, the 0x4330000080000000 double bias,
 * and the six-byte key material used for extended headers. */
const f32 ahxdcd_zero_f32 = 0.0f;
const double ahxdcd_two52 = 4503601774854144.0;
const char ahxdcd_key_material[8] = {
    0x43,0x52,0x49,0x2D,0x4D,0x57,0x00,0x00
};

