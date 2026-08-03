// Auto-scaffolded catalog TU for CriWare/src/adx/ahx/ahx_dcd
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

void SKG_GenerateKey() {}

void AHXSJD_SetupAtbl(u32 val) { AHXDCD_SetupAtbl(val); }

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

void AHXDCD_DecodeHeader() {}

extern s32 AHXBSR_SearchSync(s32 a);
extern s32 AHXBSR_IsDataAvailable(s32 a, s32 b);
extern s32 AHXDCD_DecodeBhdr(void* self, s32* out);
extern void AHXDCD_BhdrToDinf(void* self, s32* out);
extern s32 AHXDCD_DecodeBitalloc2(s32 a, void* b, void* c);
extern s32 AHXDCD_DecodeScale2(s32 a, void* b, void* c, void* d, void* e);

s32 AHXDCD_DecodeFrmHdr(void* self) {
    s32 r;
    if (*(s32*)((u8*)self + 848) == 0)
        return -1;
    r = AHXBSR_SearchSync(*(s32*)((u8*)self + 848));
    if (r == -1)
        return -1;
    if (r != 1)
        return 1;
    if (AHXBSR_IsDataAvailable(*(s32*)((u8*)self + 848), 20) == 0)
        return -1;
    if (AHXDCD_DecodeBhdr((void*)*(s32*)((u8*)self + 848), (s32*)((u8*)self + 856)) <= 0)
        return -1;
    AHXDCD_BhdrToDinf((s32*)((u8*)self + 856), (s32*)((u8*)self + 904));
    AHXDCD_DecodeBitalloc2(*(s32*)((u8*)self + 848), (u8*)self + 904, (u8*)self + 964);
    AHXDCD_DecodeScale2(*(s32*)((u8*)self + 848), (u8*)self + 904, (u8*)self + 964, (u8*)self + 1220, (u8*)self + 1476);
    *(u8*)((u8*)self + 841) = 1;
    *(s32*)((u8*)self + 844) = 0;
    return 1;
}

typedef struct AHXDCDState {
    u8 _00[0x349];
    s8 outputReady;
    u8 _34A[2];
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
    if (state->outputReady == 0) return 0;
    return state->totalNumSmpl;
}

void* memcpy(void* dest, const void* src, size_t n);
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




extern s32 AHXBSR_GetBitStm(void* self, s32 bits);

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
s32 AHXDCD_DecodeBitalloc2(s32 a, void* b, void* c) { return 0; }
s32 AHXDCD_DecodeScale2(s32 a, void* b, void* c, void* d, void* e) { return 0; }
