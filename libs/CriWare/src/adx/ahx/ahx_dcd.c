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

extern void AHXDCD_Reset(void* self);

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
        u32 m = (u32)((s32)x1 * (s32)lbl_eu_80516B30[0x80 + (s8)key[i]]);
        u32 b = m << 22;
        u32 s = m >> 31;
        u32 t = (b - s);
        t = (t << 10) | (t >> 22);
        t = t + s;
        x1 = lbl_eu_80516B30[t];
    }

    x2 = lbl_eu_80516B30[0x200];
    for (i = 0; i < count; i++) {
        u32 m = (u32)((s32)x2 * (s32)lbl_eu_80516B30[0x80 + (s8)key[i]]);
        u32 b = m << 22;
        u32 s = m >> 31;
        u32 t = (b - s);
        t = (t << 10) | (t >> 22);
        t = t + s;
        x2 = lbl_eu_80516B30[t];
    }

    x3 = lbl_eu_80516B30[0x300];
    for (i = 0; i < count; i++) {
        u32 m = (u32)((s32)x3 * (s32)lbl_eu_80516B30[0x80 + (s8)key[i]]);
        u32 b = m << 22;
        u32 s = m >> 31;
        u32 t = (b - s);
        t = (t << 10) | (t >> 22);
        t = t + s;
        x3 = lbl_eu_80516B30[t];
    }

    *out1 = x1;
    *out2 = x2;
    *out3 = x3;

    return 0;
}

s32 AHXDCD_DecodeHeader(void* self) {
    s32 ret;
    s32 n = 0;
    s32 m = 0;
    u8 tbl[8];
    u8 buf[0x40];
    s8 v;
    s32 i;
    if (*(void**)((u8*)self + 0x350) == NULL)
        return 0;
    memset(buf, 0, 0x40);
    for (i = 0; i < 4; i++) {
        tbl[i] = (u8)AHXBSR_GetBitStm(*(void**)((u8*)self + 0x350), 8);
    }
    ADX_DecodeHeader(tbl, 4, &n, NULL);
    m += n;
    for (i = 0; i < 8; i++) {
        tbl[i] = (u8)AHXBSR_GetBitStm(*(void**)((u8*)self + 0x350), 8);
    }
    ret = ADX_DecodeHeader(tbl, 0x200, &n, buf);
    if (ret <= 0)
        return 0;
    *(s32*)((u8*)self + 0x38c) = (s8)*(u8*)((u8*)buf + 0x13);
    *(s32*)((u8*)self + 0x390) = *(s32*)((u8*)buf + 0x14);
    *(s32*)((u8*)self + 0x394) = *(s32*)((u8*)buf + 0x18);
    *(s32*)((u8*)self + 0x3a8) = *(s32*)((u8*)buf + 0x38);
    *(s32*)((u8*)self + 0x3a4) = *(s32*)((u8*)buf + 0x34);
    *(s32*)((u8*)self + 0x3ac) = *(s32*)((u8*)buf + 0x3c);
    *(s32*)((u8*)self + 0x3b4) = *(s32*)((u8*)buf + 0x44);
    *(s32*)((u8*)self + 0x3b0) = *(s32*)((u8*)buf + 0x40);
    *(s32*)((u8*)self + 0x3b8) = *(s32*)((u8*)buf + 0x48);
    *(s32*)((u8*)self + 0x388) = n;
    v = (s8)*(u8*)((u8*)buf + 0x4d);
    if (v > 0x10) {
        *(u16*)((u8*)self + 0x3bc) = 0;
        if (lbl_eu_805E64D0 == 0) {
            lbl_eu_805E64D0 = 1;
            SKG_GenerateKey((u8*)lbl_eu_80517638, 6, (s16*)((u8*)self + 0x3be),
                            (s16*)((u8*)self + 0x3c0), (s16*)((u8*)self + 0x3c2));
        }
    }
    if (v > 8) {
        memcpy((u8*)self + 0x3bc, (u8*)self + 0xbc4, 8);
    } else {
        memset((u8*)self + 0x3bc, 0, 8);
    }
    return 0;
}

extern s32 AHXBSR_SearchSync(s32 a);
extern s32 AHXBSR_IsDataAvailable(s32 a, s32 b);
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

extern void AHXDCD_GetSample_Dequantize_Denormalize(s32 a, s32* b, s32* c, s32* d, u32 e, s32 f, s32 g);
extern void AHXSBF_Synthesize(s32* base, s32 pcm, s32 idx, s32* out);

s32 AHXDCD_DecodeData(void* self, u8* out) {
    s32 i;
    if (*(s32*)((u8*)self + 844) == 12)
        return 0;
    {
        s32 nblk = *(s32*)((u8*)self + 844) >> 2;
        AHXDCD_GetSample_Dequantize_Denormalize(
            *(s32*)((u8*)self + 848), (s32*)((u8*)self + 904), (s32*)((u8*)self + 964),
            (s32*)((u8*)self + 1476), nblk,
            *(s32*)((u8*)self + 0), *(s32*)((u8*)self + 836));
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
    AHXBSR_GetBitStm(self, 1);
    out[2] = (u32)out[1] >> 5;
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
    s32 nch = *(s32*)((u8*)inf + 4);
    s32 c = *(s32*)((u8*)inf + 0x10);
    s32 s = *(s32*)((u8*)inf + 0x14);
    s32 i, j, k;
    s32 v;
    for (i = 0; i < s; i++) {
        s32* p = out + i;
        for (j = 0; j < nch; j++) {
            *p = AHXBSR_GetBitStm(self, *(s32*)((u8*)inf + 0x1C + i * 384));
            p += 128;
        }
    }
    k = s;
    {
        s32* p = out + s;
        s32* q = (s32*)((u8*)inf + 0x18 + s * 384);
        for (; k < c; k++) {
            v = AHXBSR_GetBitStm(self, *(q + 1));
            p[32] = v;
            q += 96;
            p[0] = v;
            p++;
        }
    }
    for (k = 0; k < 32; k++) {
        if (nch > 0) {
            i = 0;
            if (nch >= 8) {
                j = (nch - 8 + 7) >> 3;
                while (j > 0) {
                    s32* p = out + k * 4 + i;
                    p[0] = v;
                    p[32] = v;
                    p[64] = v;
                    p[96] = v;
                    p[128] = v;
                    p[160] = v;
                    p[192] = v;
                    p[224] = v;
                    i += 8;
                    j--;
                }
            }
            while (i < nch) {
                out[k * 4 + i] = v;
                i++;
            }
        }
    }
    return 0;
}
s32 AHXDCD_DecodeScale2(s32 a, void* b, void* c, void* d, void* e) { return 0; }

void AHXDCD_DecodeFrmHdr() {}
